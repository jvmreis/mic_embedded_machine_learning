/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sai.h"
#include "app.h"
#include "MPU6050.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef DEMO_CODEC_VOLUME
#define DEMO_CODEC_VOLUME 100
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if defined DEMO_SDCARD
#include "ff.h"
#include "diskio.h"
#include "fsl_sd.h"
#include "sdmmc_config.h"

/*!
 * @brief wait card insert function.
 */
static status_t sdcardWaitCardInsert(void);
static int SD_FatFsInit(void);
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t audioBuff[BUFFER_SIZE * BUFFER_NUM], 4);
extern codec_config_t boardCodecConfig;
volatile bool istxFinished     = false;
volatile bool isrxFinished     = false;
volatile uint32_t beginCount   = 0;
volatile uint32_t sendCount    = 0;
volatile uint32_t receiveCount = 0;
volatile bool sdcard           = false;
volatile uint32_t fullBlock    = 0;
volatile uint32_t emptyBlock   = BUFFER_NUM;
#if defined DEMO_SDCARD
/* static values for fatfs */

volatile uint8_t fifo_buffer[6]; // 6 bytes = 3 eixos de aceleração

AT_NONCACHEABLE_SECTION(FATFS g_fileSystem); /* File system object */
AT_NONCACHEABLE_SECTION(FIL g_fileObject);   /* File object */
AT_NONCACHEABLE_SECTION(BYTE work[FF_MAX_SS]);
extern sd_card_t g_sd; /* sd card descriptor */
#endif
codec_handle_t codecHandle;

#define BOARD_USER_LED_GPIO_PIN_MASK (1u << BOARD_USER_LED_GPIO_PIN)
#define BOARD_USER_MPU6050_INT_PIN_MASK (1u << BOARD_LCDIF_D15_PIN)



/*******************************************************************************
 * Code
 ******************************************************************************/
void txCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    sendCount++;
    emptyBlock++;

    if (sendCount == beginCount)
    {
        istxFinished = true;
        SAI_TransferTerminateSendEDMA(base, handle);
        sendCount = 0;
    }
}

void rxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    receiveCount++;
    fullBlock++;

    if (receiveCount == beginCount)
    {
        isrxFinished = true;
        SAI_TransferTerminateReceiveEDMA(base, handle);
        receiveCount = 0;
    }
}

#if defined DEMO_SDCARD
static status_t sdcardWaitCardInsert(void)
{
    BOARD_SD_Config(&g_sd, NULL, BOARD_SDMMC_SD_HOST_IRQ_PRIORITY, NULL);

    /* SD host init function */
    if (SD_HostInit(&g_sd) != kStatus_Success)
    {
        PRINTF("\r\nSD host init fail\r\n");
        return kStatus_Fail;
    }
    /* power off card */
    SD_SetCardPower(&g_sd, false);
    /* wait card insert */
    if (SD_PollingCardInsert(&g_sd, kSD_Inserted) == kStatus_Success)
    {
        PRINTF("\r\nCard inserted.\r\n");
        /* power on the card */
        SD_SetCardPower(&g_sd, true);
    }
    else
    {
        PRINTF("\r\nCard detect fail.\r\n");
        return kStatus_Fail;
    }

    return kStatus_Success;
}

int SD_FatFsInit()
{
    /* If there is SDCard, Initialize SDcard and Fatfs */
    FRESULT error;

    static const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
    static const TCHAR recordpathBuffer[]     = DEMO_RECORD_PATH;

    PRINTF("\r\nPlease insert a card into board.\r\n");

    if (sdcardWaitCardInsert() != kStatus_Success)
    {
        return -1;
    }
    error = f_mount(&g_fileSystem, driverNumberBuffer, 1U);
    if (error == FR_OK)
    {
        PRINTF("Mount volume Successfully.\r\n");
    }
    else if (error == FR_NO_FILESYSTEM)
    {
#if FF_USE_MKFS
        PRINTF("\r\nMake file system......The time may be long if the card capacity is big.\r\n");
        if (f_mkfs(driverNumberBuffer, 0, work, sizeof work) != FR_OK)
        {
            PRINTF("Make file system failed.\r\n");
            return -1;
        }
#else
        PRINTF("No file system detected, Please check.\r\n");
        return -1;
#endif /* FF_USE_MKFS */
    }
    else
    {
        PRINTF("Mount volume failed.\r\n");
        return -1;
    }

#if (FF_FS_RPATH >= 2U)
    error = f_chdrive((char const *)&driverNumberBuffer[0U]);
    if (error)
    {
        PRINTF("Change drive failed.\r\n");
        return -1;
    }
#endif

    PRINTF("\r\nCreate directory......\r\n");
    error = f_mkdir((char const *)&recordpathBuffer[0U]);
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("Directory exists.\r\n");
        }
        else
        {
            PRINTF("Make directory failed.\r\n");
            return -1;
        }
    }

    return 0;
}
#endif /* DEMO_SDCARD */

void BOARD_USER_BUTTON_callback(void *param)
{
    GPIO_PortToggle(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN_MASK);
    GPIO_PortClearInterruptFlags(BOARD_USER_LED_GPIO, BOARD_USER_BUTTON_GPIO_PIN_MASK);
}

void BOARD_MPU6050_int_callback(void *param)
{

    GPIO_PortToggle(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN_MASK);

//    MPU6050_getFIFOBytes(fifo_buffer, 6);
//    MPU6050_setInterruptLatch(true);
//    MPU6050_setInterruptLatchClear(true); // Para limpar lendo o status
    MPU6050_resetFIFO();
    GPIO_PortClearInterruptFlags(BOARD_LCDIF_D15_PORT, BOARD_USER_MPU6050_INT_PIN_MASK);

}

/*!
 * @brief Main function
 */
int main(void)
{
    char input       = '1';
    uint8_t userItem = 1U;

    BOARD_InitHardware();

    PRINTF("SAI Demo started!\n\r");

    /* Use default setting to init codec */
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }
    if (CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight,
                        DEMO_CODEC_VOLUME) != kStatus_Success)
    {
        assert(false);
    }
    /* Enable interrupt to handle FIFO error */
    SAI_TxEnableInterrupts(DEMO_SAI_PERIPHERAL, kSAI_FIFOErrorInterruptEnable);
    SAI_RxEnableInterrupts(DEMO_SAI_PERIPHERAL, kSAI_FIFOErrorInterruptEnable);
    EnableIRQ(DEMO_SAI_TX_IRQ);
    EnableIRQ(DEMO_SAI_RX_IRQ);

    // MPU6050
    MPU6050(MPU6050_ADDRESS_AD0_LOW);

    // Initialize device
    PRINTF("Initializing I2C devices...\r\n");
    MPU6050_initialize();

    SDK_DelayAtLeastUs(2000, CLOCK_GetFreq(kCLOCK_CoreSysClk));

    // Verify connection
    PRINTF("Testing device connections...\r\n");
    PRINTF(MPU6050_testConnection() ? "MPU6050 connection successful\r\n" :
        "MPU6050 connection failed\r\n");

    PRINTF("Reading internal sensor offsets...\r\n");
    PRINTF("%d\t", MPU6050_getXAccelOffset());
    PRINTF("%d\t", MPU6050_getYAccelOffset());
    PRINTF("%d\t", MPU6050_getZAccelOffset());
    PRINTF("%d\t", MPU6050_getXGyroOffset());
    PRINTF("%d\t", MPU6050_getYGyroOffset());
    PRINTF("%d\t\n", MPU6050_getZGyroOffset());


    uint8_t result_data[6]={0,0,0,0,0,0};
    uint8_t val=255;

    MPU6050_Measurement sensor_data = {0};

    // Configure escalas uma única vez
    MPU6050_configScale(&sensor_data);

    BOARD_LPI2C_Receive(BOARD_ACCEL_I2C_BASEADDR,0x68 , 0x75, 1, result_data, 6);

    if (MPU6050_getMeasurement(&sensor_data)) {
    	PRINTF("Ax=%.2fg Ay=%.2fg Az=%.2fg | Gx=%.2f Gy=%.2f Gz=%.2f | T=%.2f C\r\n",
    	       sensor_data.accel_g[0], sensor_data.accel_g[1], sensor_data.accel_g[2],
    	       sensor_data.gyro_dps[0], sensor_data.gyro_dps[1], sensor_data.gyro_dps[2],
    	       sensor_data.temperature);
    }


    MPU6050_enableFIFOandInterrupts();



#if defined DEMO_SDCARD
    /* Init SDcard and FatFs */
    if (SD_FatFsInit() != 0)
    {
        PRINTF("SDCARD init failed !\r\n");
    }
#endif /* DEMO_SDCARD */

    PRINTF("\n\rPlease choose the option :\r\n");
    while (1)
    {
        PRINTF("\r%d. Record and playback at same time\r\n", userItem++);
        PRINTF("\r%d. Playback sine wave\r\n", userItem++);
#if defined DEMO_SDCARD
        PRINTF("\r%d. Record to SDcard, after record playback it\r\n", userItem++);
#endif /* DEMO_SDCARD */
#if defined DIG_MIC
        PRINTF("\r%d. Record using digital mic and playback at the same time\r\n", userItem++);
#endif
        PRINTF("\r%d. Quit\r\n", userItem);

        input = GETCHAR();
        PUTCHAR(input);
        PRINTF("\r\n");

        if (input == (userItem + 48U))
        {
            break;
        }

        switch (input)
        {
            case '1':
#if defined DIG_MIC
                /* Set the audio input source to AUX */
                DA7212_ChangeInput((da7212_handle_t *)((uint32_t)(codecHandle.codecDevHandle)), kDA7212_Input_AUX);
#endif
                RecordPlayback(DEMO_SAI_PERIPHERAL, 30);
                break;
            case '2':
                //PlaybackSine(DEMO_SAI_PERIPHERAL, 250, 5);
                PlayMarioTheme(DEMO_SAI_PERIPHERAL);

                break;
#if defined DEMO_SDCARD
            case '3':
                RecordSDCard(DEMO_SAI_PERIPHERAL, 5);
                RecordAcceSDCard(BOARD_ACCEL_I2C_BASEADDR, 5);
                break;
#endif
#if defined DIG_MIC
            case userItem - 1U + 48U:
                /* Set the audio input source to DMIC */
                DA7212_ChangeInput((da7212_handle_t *)((uint32_t)(codecHandle.codecDevHandle)), kDA7212_Input_MIC1_Dig);
                RecordPlayback(DEMO_SAI_PERIPHERAL, 30);
                break;
#endif
            default:
                PRINTF("\rInvallid Input Parameter, please re-enter\r\n");
                break;
        }
        userItem = 1U;
    }

    if (CODEC_Deinit(&codecHandle) != kStatus_Success)
    {
        assert(false);
    }
    PRINTF("\n\r SAI demo finished!\n\r ");
    while (1)
    {
    }
}

void SAI_UserTxIRQHandler(void)
{
    /* Clear the FEF flag */
    SAI_TxClearStatusFlags(DEMO_SAI_PERIPHERAL, kSAI_FIFOErrorFlag);
    SAI_TxSoftwareReset(DEMO_SAI_PERIPHERAL, kSAI_ResetTypeFIFO);
    SDK_ISR_EXIT_BARRIER;
}

void SAI_UserRxIRQHandler(void)
{
    SAI_RxClearStatusFlags(DEMO_SAI_PERIPHERAL, kSAI_FIFOErrorFlag);
    SAI_RxSoftwareReset(DEMO_SAI_PERIPHERAL, kSAI_ResetTypeFIFO);
    SDK_ISR_EXIT_BARRIER;
}

void SAI_UserIRQHandler(void)
{
    if (SAI_TxGetStatusFlag(DEMO_SAI_PERIPHERAL) & kSAI_FIFOErrorFlag)
    {
        SAI_UserTxIRQHandler();
    }

    if (SAI_RxGetStatusFlag(DEMO_SAI_PERIPHERAL) & kSAI_FIFOErrorFlag)
    {
        SAI_UserRxIRQHandler();
    }
    SDK_ISR_EXIT_BARRIER;
}
