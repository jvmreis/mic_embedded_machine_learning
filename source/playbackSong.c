/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sai.h"
#include "pitches.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static float32_t ffData[2 * BUFFER_SIZE];
static float32_t ffResult[BUFFER_SIZE];
extern sai_edma_handle_t txHandle;
extern sai_edma_handle_t rxHandle;
extern uint8_t audioBuff[BUFFER_SIZE * BUFFER_NUM];
extern volatile bool istxFinished;
extern volatile bool isrxFinished;
extern volatile uint32_t beginCount;
extern volatile uint32_t sendCount;
extern volatile uint32_t receiveCount;
extern bool sdcard;
extern volatile uint32_t fullBlock;
extern volatile uint32_t emptyBlock;
/*******************************************************************************
 * Code
 ******************************************************************************/

typedef struct {
    uint32_t frequency;
    uint32_t duration_ms;
} Note;

const Note mario_melody[] = {
    {NOTE_E5, 200}, {NOTE_E5, 200}, {REST, 200},
    {NOTE_E5, 200}, {REST, 200}, {NOTE_C5, 200},
    {NOTE_E5, 200}, {REST, 200}, {NOTE_G5, 400},
    {REST, 400}, {NOTE_G4, 400}, {REST, 400},
    // (adicione mais notas conforme desejar)
};
const size_t mario_melody_length = sizeof(mario_melody) / sizeof(Note);

void PlayNote(I2S_Type *base, uint32_t freq_hz, uint32_t duration_ms)
{
    uint32_t i, val;
    sai_transfer_t xfer = {0};
    uint32_t count = DEMO_AUDIO_SAMPLE_RATE / freq_hz;
    uint32_t bufferLen = 4 * count;
    uint32_t repeatCount = (BUFFER_SIZE * BUFFER_NUM) / bufferLen;

    /* Geração da senoide de uma única onda */
    for (i = 0; i < count; i++)
    {
        val = arm_sin_q15(0x8000 * i / count);
        audioBuff[4 * i] = val & 0xFFU;
        audioBuff[4 * i + 1] = (val >> 8U) & 0xFFU;
        audioBuff[4 * i + 2] = val & 0xFFU;
        audioBuff[4 * i + 3] = (val >> 8U) & 0xFFU;
    }

    /* Preenche o buffer com repetições da senoide */
    for (i = 1; i < repeatCount; i++)
    {
        memcpy(audioBuff + (i * bufferLen), audioBuff, bufferLen);
    }

    /* Define o número de blocos a serem enviados de acordo com o tempo da nota */
    beginCount = DEMO_AUDIO_SAMPLE_RATE * duration_ms / 1000 * 4 / BUFFER_SIZE;

    /* Reset estado */
    istxFinished = false;
    emptyBlock = BUFFER_NUM;
    sendCount = 0;

    /* Reset hardware SAI */
    SAI_TxSoftwareReset(base, kSAI_ResetTypeSoftware);

    uint32_t totalNum = 0;
    uint32_t index = 0;

    xfer.dataSize = BUFFER_SIZE;

    while (totalNum < beginCount)
    {
        if (emptyBlock > 0)
        {
            xfer.data = audioBuff + (index * BUFFER_SIZE);
            if (SAI_TransferSendEDMA(base, &txHandle, &xfer) == kStatus_Success)
            {
                index = (index + 1U) % BUFFER_NUM;
                totalNum++;
                emptyBlock--;
            }
        }
    }

    while (!istxFinished)
    {
    }
}
void PlayMarioTheme(I2S_Type *base)
{
    for (size_t i = 0; i < mario_melody_length; ++i)
    {
        if (mario_melody[i].frequency == REST )
        {
            SDK_DelayAtLeastUs(mario_melody[i].duration_ms * 1000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        }
        else
        {
            PlayNote(base, mario_melody[i].frequency, mario_melody[i].duration_ms);
        }
    }
}
