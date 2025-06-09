/*
 * ------------------------------------------------------------------------------
 * MODULE: Accelerometer Logging and Anomaly Detection
 * DESCRIPTION:
 * This file contains the implementation for:
 *   - Logging accelerometer data from MPU6050 to CSV for use with NXP eIQ
 *   - Anomaly detection using the NXP Time Series library (TSS)
 *   - Optional SAI audio recording and playback to WAV format (demo purposes)
 *
 * The output CSV format is compatible with eIQ's training pipeline.
 * ------------------------------------------------------------------------------
 */


#include "ff.h"
#include "diskio.h"
#include "fsl_sd.h"
#include "sai.h"
#include "MPU6050.h"
#include <stdio.h>

#include "stdio.h"
#include "TimeSeries.h"

float data_input[TSS_INPUT_DATA_LEN * TSS_INPUT_DATA_DIM];
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_RECORD_CSV_PATH                                                                                         \
    {                                                                                                                \
        SDDISK + '0', ':', '/', 'r', 'e', 'c', 'o', 'r', 'd', '/', 'a', 'c', 'c', 'e', 'l', 'e', '.', 'c', 's', 'v', \
            '\0'                                                                                                     \
    }

#define ACC_BUFFER_SIZE 512
#define AXIS_NUM 3
#define SAMPLES_PER_LINE 128  // Number of samples per line before inserting a newline
#define ACQUISITION_TIME 5 //seconds

//AT_NONCACHEABLE_SECTION_ALIGN(uint8_t acccBuff[ACC_BUFFER_SIZE * AXIS_NUM], 4);


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void wav_header(uint8_t *header, uint32_t sampleRate, uint32_t bitsPerFrame, uint8_t channels, uint32_t fileSize);
/*******************************************************************************
 * Variables
 ******************************************************************************/
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
extern FIL g_fileObject;

extern volatile bool i2c_new_data;


/*
 * Buffers for logging raw acceleration data
 */
static int16_t ax_buffer[ 1000 * ACQUISITION_TIME];
static int16_t ay_buffer[ 1000 * ACQUISITION_TIME];
static int16_t az_buffer[ 1000 * ACQUISITION_TIME];

/*******************************************************************************
 * Code
 ******************************************************************************/
void wav_header(uint8_t *header, uint32_t sampleRate, uint32_t bitsPerFrame, uint8_t channels, uint32_t fileSize)
{
    uint32_t totalDataLen = fileSize - 8U;
    uint32_t audioDataLen = fileSize - 44U;
    uint32_t byteRate     = sampleRate * (bitsPerFrame / 8U) * channels;
    header[0]             = 'R';
    header[1]             = 'I';
    header[2]             = 'F';
    header[3]             = 'F';
    header[4]             = (totalDataLen & 0xff); /* file-size (equals file-size - 8) */
    header[5]             = ((totalDataLen >> 8U) & 0xff);
    header[6]             = ((totalDataLen >> 16U) & 0xff);
    header[7]             = ((totalDataLen >> 24U) & 0xff);
    header[8]             = 'W'; /* Mark it as type "WAVE" */
    header[9]             = 'A';
    header[10]            = 'V';
    header[11]            = 'E';
    header[12]            = 'f'; /* Mark the format section 'fmt ' chunk */
    header[13]            = 'm';
    header[14]            = 't';
    header[15]            = ' ';
    header[16]            = 16; /* 4 bytes: size of 'fmt ' chunk, Length of format data.  Always 16 */
    header[17]            = 0;
    header[18]            = 0;
    header[19]            = 0;
    header[20]            = 1; /* format = 1 ,Wave type PCM */
    header[21]            = 0;
    header[22]            = channels; /* channels */
    header[23]            = 0;
    header[24]            = (sampleRate & 0xff);
    header[25]            = ((sampleRate >> 8U) & 0xff);
    header[26]            = ((sampleRate >> 16U) & 0xff);
    header[27]            = ((sampleRate >> 24U) & 0xff);
    header[28]            = (byteRate & 0xff);
    header[29]            = ((byteRate >> 8U) & 0xff);
    header[30]            = ((byteRate >> 16U) & 0xff);
    header[31]            = ((byteRate >> 24U) & 0xff);
    header[32]            = (channels * bitsPerFrame / 8); /* block align */
    header[33]            = 0;
    header[34]            = bitsPerFrame; /* bits per sample */
    header[35]            = 0;
    header[36]            = 'd'; /*"data" marker */
    header[37]            = 'a';
    header[38]            = 't';
    header[39]            = 'a';
    header[40]            = (audioDataLen & 0xff); /* data-size (equals file-size - 44).*/
    header[41]            = ((audioDataLen >> 8) & 0xff);
    header[42]            = ((audioDataLen >> 16) & 0xff);
    header[43]            = ((audioDataLen >> 24) & 0xff);
}

void RecordSDCard(I2S_Type *base, uint32_t time_s)
{
    uint32_t i            = 0;
    uint32_t bytesWritten = 0;
    uint32_t bytesRead    = 0;
    uint32_t txindex      = 0;
    uint32_t rxindex      = 0;
    uint32_t sdReadCount  = 0;
    uint8_t header[44]    = {0};
    uint32_t fileSize     = time_s * DEMO_AUDIO_SAMPLE_RATE * 2U * 2U + 44U;
    FRESULT error;
    sai_transfer_t xfer                = {0};
    static const TCHAR wavpathBuffer[] = DEMO_RECORD_WAV_PATH;

    /* Clear the status */
    isrxFinished = false;
    receiveCount = 0;
    istxFinished = false;
    sendCount    = 0;
    sdcard       = true;

    PRINTF("\r\nBegin to record......\r\n");
    PRINTF("\r\nFile path is record/music1.wav\r\n");
    error = f_open(&g_fileObject, (char const *)wavpathBuffer, (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("File exists.\r\n");
        }
        else
        {
            PRINTF("Open file failed.\r\n");
            return;
        }
    }

    /* Write the data into the sdcard */
    wav_header(header, DEMO_AUDIO_SAMPLE_RATE, 16, 2, fileSize);

    /* Write the wav header */
    error = f_write(&g_fileObject, (void *)header, 44U, (UINT *)&bytesWritten);
    if ((error) || (bytesWritten != 44))
    {
        PRINTF("Write file failed. \r\n");
    }

    /* Reset SAI internal logic */
    SAI_TxSoftwareReset(base, kSAI_ResetTypeSoftware);
    SAI_RxSoftwareReset(base, kSAI_ResetTypeSoftware);

    /* Start to record */
    beginCount = time_s * DEMO_AUDIO_SAMPLE_RATE * 2U * 2U / BUFFER_SIZE;

    /* Start record first */
    memset(audioBuff, 0, BUFFER_SIZE * BUFFER_NUM);
    xfer.dataSize = BUFFER_SIZE;
    for (i = 0; i < BUFFER_NUM; i++)
    {
        xfer.data = audioBuff + i * BUFFER_SIZE;
        SAI_TransferReceiveEDMA(base, &rxHandle, &xfer);
    }

    emptyBlock = 0;
    while ((isrxFinished != true) || (fullBlock != 0))
    {
        if (fullBlock > 0)
        {
            error = f_write(&g_fileObject, audioBuff + txindex * BUFFER_SIZE, BUFFER_SIZE, (UINT *)&bytesWritten);
            if ((error) || (bytesWritten != BUFFER_SIZE))
            {
                PRINTF("Write file failed. \r\n");
                return;
            }

            txindex = (txindex + 1U) % BUFFER_NUM;
            fullBlock--;
            emptyBlock++;
        }

        if ((emptyBlock > 0) && (isrxFinished == false))
        {
            xfer.data = audioBuff + rxindex * BUFFER_SIZE;
            rxindex   = (rxindex + 1U) % BUFFER_NUM;
            SAI_TransferReceiveEDMA(base, &rxHandle, &xfer);
            emptyBlock--;
        }
    }
    f_close(&g_fileObject);
    PRINTF("\r\nRecord is finished!\r\n");

    /* Playback the record file */
    PRINTF("\r\nPlayback the recorded file...");
    txindex    = 0;
    rxindex    = 0;
    emptyBlock = 0;
    fullBlock  = 0;
    memset(audioBuff, 0, BUFFER_SIZE * BUFFER_NUM);
    f_open(&g_fileObject, (char const *)wavpathBuffer, (FA_READ));
    if (f_lseek(&g_fileObject, 44U))
    {
        PRINTF("Set file pointer position failed. \r\n");
    }

    for (i = 0; i < BUFFER_NUM; i++)
    {
        error = f_read(&g_fileObject, (void *)(audioBuff + i * BUFFER_SIZE), BUFFER_SIZE, (UINT *)&bytesRead);
        if ((error) || (bytesRead != BUFFER_SIZE))
        {
            PRINTF("Read file failed. \r\n");
            return;
        }

        sdReadCount++;
        fullBlock++;
    }

    /* Wait for playback finished */
    while (istxFinished != true)
    {
        if ((emptyBlock > 0) && (sdReadCount < beginCount))
        {
            error = f_read(&g_fileObject, (void *)(audioBuff + rxindex * BUFFER_SIZE), BUFFER_SIZE, (UINT *)&bytesRead);
            if ((error) || (bytesRead != BUFFER_SIZE))
            {
                PRINTF("Read file failed. \r\n");
                return;
            }

            rxindex = (rxindex + 1U) % BUFFER_NUM;
            emptyBlock--;
            fullBlock++;
            sdReadCount++;
        }

        if (fullBlock > 0)
        {
            xfer.data = audioBuff + txindex * BUFFER_SIZE;
            txindex   = (txindex + 1U) % BUFFER_NUM;
            SAI_TransferSendEDMA(base, &txHandle, &xfer);
            fullBlock--;
        }
    }
    f_close(&g_fileObject);
    PRINTF("\r\nPlayback is finished!\r\n");
}
/*
 * @brief Clears all files from the /record directory on the SD card
 */
FRESULT ClearRecordFolder(void)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    char path[64];           // Pasta onde os arquivos estão
    char file_path[128];

    sprintf(path, "%c:/record", SDDISK + '0');

    // Abre o diretório
    res = f_opendir(&dir, path);
    if (res != FR_OK)
    {
        // Se não existir, cria a pasta
        if (res == FR_NO_PATH)
        {
            res = f_mkdir(path);
            if (res != FR_OK)
            {
                PRINTF("[ERROR] Falha ao criar a pasta /record. Código: %d\r\n", res);
                return res;
            }
            return FR_OK;
        }
        else
        {
            PRINTF("[ERROR] Falha ao abrir a pasta /record. Código: %d\r\n", res);
            return res;
        }
    }

    // Percorre todos os arquivos na pasta
    for (;;)
    {
        res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0) break;  // Erro ou fim da pasta
        if (fno.fattrib & AM_DIR) continue;           // Pula subpastas

        // Monta caminho completo: "0:/record/arquivo.csv"
        sprintf(file_path, "%s/%s", path, fno.fname);

        // Apaga o arquivo
        res = f_unlink(file_path);
        if (res != FR_OK)
        {
            PRINTF("[WARNING] Falha ao apagar %s. Código: %d\r\n", file_path, res);
        }
        else
        {
            PRINTF("[INFO] Apagado: %s\r\n", file_path);
        }
    }

    f_closedir(&dir);
    return FR_OK;
}

/*
 * @brief Writes collected accelerometer data into a CSV file
 *        Compatible with eIQ training format. Creates a new numbered file.
 */
void PrintAccelerometerBuffer(int16_t *ax_buffer, int16_t *ay_buffer,int16_t *az_buffer, uint16_t sample_count) {

	char buffer[100];
	FRESULT error;
	static const TCHAR csvpathBuffer[] = DEMO_RECORD_CSV_PATH;
	static int file_counter = 0;
	char filename[64];

	// Gera nome como: "0:/record/accele_0.csv"
	sprintf(filename, "%c:/record/accele_%d.csv", SDDISK + '0', file_counter++);

	error = f_open(&g_fileObject, filename,(FA_WRITE | FA_READ | FA_CREATE_ALWAYS));

	if (error) {
		PRINTF("[ERROR] Failed to open CSV file. Error code: %d\r\n", error);
		return;
	}
	uint16_t max_samples = (sample_count / SAMPLES_PER_LINE) * SAMPLES_PER_LINE;

	for (int i = 0; i < max_samples; i++) {
		//PRINTF("%d %d %d ", (int16_t)ax_buffer[i], (int16_t)ay_buffer[i], (int16_t)az_buffer[i]);

		if ((i + 1) % SAMPLES_PER_LINE == 0) {
			sprintf(buffer, "%d %d %d\n", (int16_t) ax_buffer[i],(int16_t) ay_buffer[i], (int16_t) az_buffer[i]);
			PRINTF("%s", buffer);
			f_printf(&g_fileObject, "%s", buffer);
		} else {
			sprintf(buffer, "%d %d %d ", (int16_t) ax_buffer[i],(int16_t) ay_buffer[i], (int16_t) az_buffer[i]);
			PRINTF("%s", buffer);
			f_printf(&g_fileObject, "%s", buffer);
		}
	}

	f_close(&g_fileObject);
}

/*
 * @brief Records audio to SD card and plays back (demo feature, optional)
 */
void RecordAcceSDCard()
{

    PRINTF("\r\n[INFO] Begin to record accelerometer data...\r\n");

    uint32_t collected = 0;
    const uint32_t target_samples =  1000 * ACQUISITION_TIME; // 2 kHz

    static int16_t ax =0,az=0,ay=0;
    uint8_t fifo_buffer[6];


    memset(ax_buffer, 0, sizeof(ax_buffer));
    memset(ay_buffer, 0, sizeof(ay_buffer));
    memset(az_buffer, 0, sizeof(az_buffer));

    while (collected < target_samples)
    {

        while (i2c_new_data && (collected < target_samples))
        {

        	MPU6050_getRAWAcceleration(fifo_buffer, 6);
        	 ax_buffer[collected] = (int16_t)((fifo_buffer[0] << 8) | fifo_buffer[1]);
        	 ay_buffer[collected] = (int16_t)((fifo_buffer[2] << 8) | fifo_buffer[3]);
             az_buffer[collected] = (int16_t)((fifo_buffer[4] << 8) | fifo_buffer[5]);


//          sprintf(buffer, "%d %d %d ", (int16_t)ax, (int16_t)ay, (int16_t)az);
//        	PRINTF("%s", buffer);
            collected++;
            i2c_new_data=false;
        }
    }

    //fazer o prinf de ("%d %d %d ", ax_buffer, ay_buffer, az_buffer); e a cada 128 amostras tem q dar um /n
    PrintAccelerometerBuffer(ax_buffer,ay_buffer,az_buffer,target_samples);

    PRINTF("[INFO] Recording complete. Total samples: %d\r\n", collected);
}


/*
 * @brief Collects one full line of acceleration samples for ML input
 *        Each line has SAMPLES_PER_LINE vectors of (X,Y,Z) data
 */
void sample_data(float data_buffer[])
{
    uint32_t sample_idx = 0;  // Conta amostras completas (x,y,z)
    uint8_t fifo_buffer[6];

    while (sample_idx < SAMPLES_PER_LINE)
    {
        if (i2c_new_data)
        {
            MPU6050_getRAWAcceleration(fifo_buffer, 6);

            data_buffer[sample_idx * 3 + 0] = (float)(int16_t)((fifo_buffer[0] << 8) | fifo_buffer[1]); // X
            data_buffer[sample_idx * 3 + 1] = (float)(int16_t)((fifo_buffer[2] << 8) | fifo_buffer[3]); // Y
            data_buffer[sample_idx * 3 + 2] = (float)(int16_t)((fifo_buffer[4] << 8) | fifo_buffer[5]); // Z

            sample_idx++;
            i2c_new_data = false;
        }
    }
}

/*
 * @brief Runs eIQ Time Series anomaly detection
 *        Trains model (if ODL enabled), then continuously predicts
 */
int ml_anmaly_detection(void)
{
    tss_status status;
    float probability;
    uint32_t cycleCnt;

#ifdef SUPPORT_ODL
    status = tss_ad_init(NULL);
    if (status != TSS_SUCCESS)
    {
        /* Handle the initialization failure cases */
    }

    /*The learning number is customizable, but we recommend the number greater than TSS_RECOMMEND_LEARNING_SAMPLE_NUM to get better results.*/
    int learning_num = TSS_RECOMMEND_LEARNING_SAMPLE_NUM;
    for (int i = 0; i < learning_num; i++)
    {
        sample_data(data_input);
        status = tss_ad_learn(data_input);
        if (status != TSS_LEARNING_NOT_ENOUGH && status != TSS_RECOMMEND_LEARNING_DONE)
        {
            /* Handle the learning failure cases */
        }
    }
#else
    status = tss_ad_init();
    if (status != TSS_SUCCESS)
    {
        /* Handle the initialization failure cases */
        PRINTF("[DEBUG] ML init error %d\r\n", status);

    }
#endif

    while (1)
    {
        sample_data(data_input);
        DWT->CYCCNT=0;
        status = tss_ad_predict(data_input, &probability);
        cycleCnt = DWT->CYCCNT;

        if (status != TSS_SUCCESS)
        {
            /* Handle the prediction failure cases */
            PRINTF("error %d\r\n");

        }else{
            PRINTF("anomaly detection %f %d\r\n", probability,cycleCnt);
        }

        /* Handle the prediction result */
    }

    return 0;
}
