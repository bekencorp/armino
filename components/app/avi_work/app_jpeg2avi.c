#include <common/bk_include.h>
#include "bk_arm_arch.h"
#include "bk_uart.h"
#include <os/os.h>
#include <common/bk_kernel_err.h>
#include "app_jpeg2avi.h"
#include <os/mem.h>

#define FORMAT_AUDIO   WAVE_FORMAT_PCM

typedef struct jpeg2Avi_st {
	//file handle
	avi_t *m_pAviHandle;

	//video param
	int	   m_nWidth;
	int	   m_nHeight;
	int    m_nFps;

	//audio param
	int    m_nChannel;  //   or 2
	int	   m_nSamplePerSec;//8000 or 160000 or 24000 or 32000
	int	   m_nBit;//8 or 16 or 32bit

	bool   m_bStartRecord;
	bool   m_bStartRead;

	bool   m_bSetAudio;
	bool   m_bSetVideo;
	bool   m_bStartAudio;

	//SWaveHeader	m_header;
	//unsigned int m_dTimeLastFrame;
	//int		  m_dTickPerFrame;

	beken_mutex_t m_csLock;
} JPEG2AVI_ST, *JPEG2AVI_PTR;

#define J2AVI_MALLOC            os_malloc   // sdram_malloc         os_malloc       
#define J2AVI_FREE              os_free     // sdram_free           sdram_free
#define J2AVI_PRINT             os_printf
#define J2AVI_TRUE              1
#define J2AVI_FALSE             0


static volatile JPEG2AVI_PTR g_jpeg2avi = NULL;

static void jpeg2avi_lock(void)
{
	rtos_lock_mutex(&g_jpeg2avi->m_csLock);
}
static void jpeg2avi_unlock(void)
{
	rtos_unlock_mutex(&g_jpeg2avi->m_csLock);
}

#define J2AVI_LOCK          jpeg2avi_lock
#define J2AVI_UNLOCK        jpeg2avi_unlock

int jpeg2avi_init(void)
{
	if (g_jpeg2avi != NULL) {
		J2AVI_PRINT("jpeg2avi_init aready done\r\n");
		return 1;
	}

	g_jpeg2avi = (JPEG2AVI_PTR)J2AVI_MALLOC(sizeof(JPEG2AVI_ST));
	if (g_jpeg2avi == NULL) {
		J2AVI_PRINT("jpeg2avi_init no mem\r\n");
		return 0;
	}
	os_memset(g_jpeg2avi, 0, sizeof(JPEG2AVI_ST));

	if (rtos_init_mutex(&g_jpeg2avi->m_csLock) != kNoErr) {
		J2AVI_PRINT("jpeg2avi_init mutex failed\n");
		J2AVI_FREE(g_jpeg2avi);
		g_jpeg2avi = NULL;
		return 0;
	}

	g_jpeg2avi->m_pAviHandle = NULL;

	//video param
	g_jpeg2avi->m_nWidth = 320;
	g_jpeg2avi->m_nHeight = 240;
	g_jpeg2avi->m_nFps = 15;

	//audio param
	g_jpeg2avi->m_nChannel = 1;
	g_jpeg2avi->m_nSamplePerSec = 8000;
	g_jpeg2avi->m_nBit = 16;

	g_jpeg2avi->m_bSetVideo = 0;
	g_jpeg2avi->m_bSetAudio = 0;

	g_jpeg2avi->m_bStartAudio = 0;
	g_jpeg2avi->m_bStartRecord = 0;
	g_jpeg2avi->m_bStartRead = 0;

	//g_jpeg2avi->m_dTimeLastFrame = 0;
	//g_jpeg2avi->m_dTickPerFrame = 60;

	jpeg2avi_set_video_param(320, 240, 10);
	jpeg2avi_set_audio_param(1, 8000, 16);

	return 1;
}

void jpeg2avi_set_video_param(int nWidth, int nHeight, int nFps)
{
	if (g_jpeg2avi == NULL)
		return;

	g_jpeg2avi->m_nWidth = nWidth;
	g_jpeg2avi->m_nHeight = nHeight;
	g_jpeg2avi->m_nFps = nFps;
	g_jpeg2avi->m_bSetVideo = 1;

	//if (g_jpeg2avi->m_nFps > 0)
	//{
	//    g_jpeg2avi->m_dTickPerFrame = 1000 / g_jpeg2avi->m_nFps;
	// }
}

void jpeg2avi_set_audio_param(int nChannel, int nSamplePerSec, int nBit)
{
	if (g_jpeg2avi == NULL)
		return;

	g_jpeg2avi->m_nChannel = nChannel;
	g_jpeg2avi->m_nSamplePerSec = nSamplePerSec;
	g_jpeg2avi->m_nBit = nBit;
	g_jpeg2avi->m_bStartAudio = 1;
	g_jpeg2avi->m_bSetAudio = 1;
}

int jpeg2avi_audio_format_get(void)
{
	return FORMAT_AUDIO;
}

int jpeg2avi_start_record(const char *szFileName)
{
	if (g_jpeg2avi == NULL)
		return 0;

	if (g_jpeg2avi->m_bStartRecord)
		return 1;

	if (g_jpeg2avi->m_bStartRead)
		return 0;

	// update system time before create/open a file
	//ntp_sync_to_rtc();

	J2AVI_LOCK();

	AVI_open_output_file(&(g_jpeg2avi->m_pAviHandle), (char *) szFileName);
	if (!g_jpeg2avi->m_pAviHandle) {
		J2AVI_UNLOCK();
		return 0;
	}
	if (g_jpeg2avi->m_bSetVideo) {
		char VideoFmtMJPG[4] = { 'M', 'J', 'P', 'G' };
		//	AVI_set_video(g_jpeg2avi->m_pAviHandle, g_jpeg2avi->m_nWidth,
		//   g_jpeg2avi->m_nHeight, g_jpeg2avi->m_nFps, VideoFmtMJPG);

		AVI_set_video(g_jpeg2avi->m_pAviHandle, g_jpeg2avi->m_nWidth,
					  g_jpeg2avi->m_nHeight, g_jpeg2avi->m_nFps, VideoFmtMJPG);

	}
	if (g_jpeg2avi->m_bSetAudio) {
		AVI_set_audio(g_jpeg2avi->m_pAviHandle, g_jpeg2avi->m_nChannel,
					  g_jpeg2avi->m_nSamplePerSec, g_jpeg2avi->m_nBit, jpeg2avi_audio_format_get());/////////////WAVE_FORMAT_PCM WAVE_FORMAT_DVI_ADPCM
	}
	g_jpeg2avi->m_bStartRecord = 1;

	J2AVI_UNLOCK();

	return 1;
}

int jpeg2avi_input_data(void *pData, const int nLength, int eType)
{
	int ret;

	if (g_jpeg2avi == NULL)
		return 0;

	J2AVI_LOCK();

	if (!g_jpeg2avi->m_bStartRecord || !g_jpeg2avi->m_pAviHandle || !pData || (nLength < 0)) {
		J2AVI_UNLOCK();
		return 0;
	}
	if (eType == eTypeVideo)
		ret = AVI_write_frame(g_jpeg2avi->m_pAviHandle, (char *) pData, nLength);
	else
		ret = AVI_write_audio(g_jpeg2avi->m_pAviHandle, (char *) pData, nLength);

	J2AVI_UNLOCK();

	return ret;
}

void jpeg2avi_stop_record(void)
{
	if (g_jpeg2avi == NULL)
		return;

	J2AVI_LOCK();

	g_jpeg2avi->m_bStartRecord = 0;
	//	g_jpeg2avi->m_bSetAudio = 0;
	//	g_jpeg2avi->m_bSetVideo = 0;

	if (g_jpeg2avi->m_pAviHandle) {
		AVI_close(g_jpeg2avi->m_pAviHandle);
		g_jpeg2avi->m_pAviHandle = NULL;
	}
	J2AVI_UNLOCK();
}

int jpeg2avi_start_read(const char *szFileName)
{
	//LOGD("START RECORD");
	if (g_jpeg2avi == NULL)
		return 0;

	if (g_jpeg2avi->m_bStartRecord)
		return 0;

	if (g_jpeg2avi->m_bStartRead)
		return 1;

	g_jpeg2avi->m_pAviHandle = AVI_open_input_file((char *) szFileName, 1);
	if (!g_jpeg2avi->m_pAviHandle)
		return 0;

	g_jpeg2avi->m_bStartRead = 1;

	return 1;
}

int jpeg2avi_read_data(void *pData, const int nLength, int eType)
{
	int ret;
	if (g_jpeg2avi == NULL)
		return 0;

	J2AVI_LOCK();
	if (!g_jpeg2avi->m_bStartRead || !g_jpeg2avi->m_pAviHandle || !pData || nLength < 0) {
		J2AVI_UNLOCK();
		return 0;
	}

	if (eType == eTypeVideo)
		ret = AVI_read_frame(g_jpeg2avi->m_pAviHandle, (char *) pData, nLength);
	else
		ret = AVI_read_audio(g_jpeg2avi->m_pAviHandle, (char *) pData, nLength);

	J2AVI_UNLOCK();

	return ret;
}

void jpeg2avi_stop_read(void)
{
	if (g_jpeg2avi == NULL)
		return;

	if (g_jpeg2avi->m_bStartRecord)
		return;

	if (!g_jpeg2avi->m_bStartRead)
		return;

	J2AVI_LOCK();
	g_jpeg2avi->m_bStartRead = 0;
	if (g_jpeg2avi->m_pAviHandle) {
		AVI_close(g_jpeg2avi->m_pAviHandle);
		g_jpeg2avi->m_pAviHandle = NULL;
	}

	J2AVI_UNLOCK();
}

int jpeg2avi_deinit(void)
{
	if (g_jpeg2avi == NULL)
		return 0;

	jpeg2avi_stop_record();
	jpeg2avi_stop_read();

	rtos_deinit_mutex(&g_jpeg2avi->m_csLock);

	J2AVI_FREE(g_jpeg2avi);
	g_jpeg2avi = NULL;

	return 0;
}

int jpeg2avi_is_create(void)
{
	if (g_jpeg2avi == NULL)
		return 0;
	else
		return 1;
}


#include "ff.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static beken_thread_t aud_thread_handle = NULL;
static void aud_task( void *para )
{
	void *pdata = (void *)(0x00001000);
	uint32_t ldata = 320;

	uint32_t loop_cnt = 600;
	int ret = 0;

	while (loop_cnt --) 
	{
		ret = jpeg2avi_input_data(pdata, ldata, eTypeAudio);
		if (ret <= 0) 
		{
			J2AVI_PRINT("jpeg2avi_input audio failed\r\n");
			break;
		}

		rtos_delay_milliseconds(40);
	}

	rtos_delete_thread(NULL);

}

static void aud_task_init(void)
{
	rtos_create_thread(&aud_thread_handle,
						 3,
						 "aud",
						 (beken_thread_function_t)aud_task /*cli_main*/,
						 4096,
						 0);
}

#include <driver/hal/hal_aon_rtc_types.h>
#include "../time/sys_time.h"
static void j2avi_test(int argc, char **argv)
{
	if(0 == strcmp("avi",argv[1]))
	{
		int ret;

		if (jpeg2avi_init() != 1) {
			J2AVI_PRINT("jpeg2avi_init failed\r\n");
			return;
		}

		jpeg2avi_set_video_param(640, 480, 20);
		jpeg2avi_set_audio_param(1, 8000, 16);

#define DEFAULT_avi_FILE_NAME	   "test.avi"
		
		char cFileName[FF_MAX_LFN] = {0};

		if(argv[2])
			sprintf(cFileName, "%d:/%s", 1, argv[2]);
		else
			sprintf(cFileName, "%d:/%s", 1, DEFAULT_avi_FILE_NAME);

		J2AVI_PRINT("****** avi fileName:%s **********\"\r\n", cFileName);

		jpeg2avi_start_record(cFileName);

		//aud_task_init();
		(void)aud_task_init;


		void *pdata = (void *)(0x00001000);
		uint32_t ldata = 1024 * 30 - 3;

		uint32_t write_bytes = (uint32_t)atoi(argv[3]);
		ldata = write_bytes;
		if(0 == ldata)
		{
			ldata = 1024 * 30 - 3;
		}
		J2AVI_PRINT("write_bytes:%d\r\n", ldata);

		uint32_t loop_cnt = 600;
		loop_cnt = (uint32_t)atoi(argv[4]);
		J2AVI_PRINT("loop_cnt:%d\r\n", loop_cnt);


		while (loop_cnt --) {
			//REG_WRITE((0x00802800+(14*4)), 0x02);
			ret = jpeg2avi_input_data(pdata, ldata, eTypeVideo);
			//REG_WRITE((0x00802800+(14*4)), 0x00);
			if (ret <= 0) {
				J2AVI_PRINT("jpeg2avi_input video failed\r\n");
				goto j2avi_exit;
			}

			rtos_delay_milliseconds(50);
		}

j2avi_exit:
		J2AVI_PRINT("\r\n\r\njpeg2avi_stop_record\"\r\n");

		jpeg2avi_stop_record();
		jpeg2avi_deinit();
	}
	else if(0 == strcmp("n_avi",argv[1]))
	{
#define DEFAULT_TXT_FILE_NAME	   "test.txt"

		FIL file = {0};
		FRESULT fr = 0;
		char cFileName[FF_MAX_LFN] = {0};
		unsigned int uiTemp = 0;

		if(argv[2])
			sprintf(cFileName, "%d:/%s", 1, argv[2]);
		else
			sprintf(cFileName, "%d:/%s", 1, DEFAULT_TXT_FILE_NAME);

		J2AVI_PRINT("=========cFileName:%s========\"\r\n", cFileName);

		fr = f_open(&file, cFileName, FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
		if (fr != FR_OK)
		{
			J2AVI_PRINT("f_open failed 1 fr = %d\r\n", fr);
			goto exit;
		}

		void *pdata = (void *)(0x00001000);
		uint32_t ldata = 30*1024;
		uint32_t loop_cnt = 6;

		//write:one time write all contents
		while (loop_cnt --)
		{
			//J2AVI_PRINT("f_write len:%d\r\n",ldata);
			fr = f_write(&file, (uint8_t *)pdata, ldata, &uiTemp);
			if (fr != FR_OK)
			{
				J2AVI_PRINT("f_write fail 1 fr = %d\r\n", fr);
				goto exit;
			}

			//J2AVI_PRINT("\r\n");
		}

		fr = f_close(&file);
		if (fr != FR_OK)
		{
			J2AVI_PRINT("f_close fail 1 fr = %d\r\n", fr);
			goto exit;
		}
	}
exit:
	return;
}

static void cli_avi_intf_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	j2avi_test(argc,argv);
}

#include "time/time.h"
#include <components/app_time_intf.h>
#include <os/os.h>

static void psram_api_test(int argc, char **argv)
{
	static uint8_t *ptr = NULL;
	static uint8_t *avi_index_ptr = NULL;


	if(0 == strcmp("malloc",argv[1]))
	{
		uint32_t size = (uint32_t)atoi(argv[2]);
		J2AVI_PRINT("psram_malloc size = %d\r\n", size);
		ptr = psram_malloc(size);
		if(NULL == ptr)
		{
			J2AVI_PRINT("psram_malloc fail!\r\n");
		}
		else
		{
			J2AVI_PRINT("psram_malloc ok %p\r\n",ptr);
			memset(ptr,0,size);
		}
	}
	else if(0 == strcmp("free",argv[1]))
	{
		if(ptr)
		{
			psram_free(ptr);
			J2AVI_PRINT("psram_free ok %p\r\n",ptr);
			ptr = NULL;
		}
	}
	else if(0 == strcmp("avi_index_malloc",argv[1]))
	{
		avi_index_ptr = psram_malloc(AVI_INDEX_COUNT);
		J2AVI_PRINT("psram_malloc avi_index_ptr: %p\r\n",avi_index_ptr);
	}
	else if(0 == strcmp("avi_index_free",argv[1]))
	{
		psram_free(avi_index_ptr);
		avi_index_ptr = NULL;
		J2AVI_PRINT("psram_free avi_index_ptr: %p\r\n",avi_index_ptr);
	}
	else if(0 == strcmp("get_systime",argv[1]))
	{
		J2AVI_PRINT("sys running current time:%ldms\r\n",rtos_get_time());
	}
	else if(0 == strcmp("gettime",argv[1]))
	{
		char datetime[16] = {0};
		app_time_timestr_get(datetime,16);

		user_datetime_t user_datetime;
		app_time_datetime_get(&user_datetime);
	}
}

static void cli_psram_api_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	psram_api_test(argc,argv);
}

#include "cli.h"
#define AVI_INTF_CMD_CNT (sizeof(s_avi_intf_commands) / sizeof(struct cli_command))
static const struct cli_command s_avi_intf_commands[] = {
	{"avi_intf_test", "", cli_avi_intf_cmd},
	{"psram_api_test", "", cli_psram_api_test_cmd},
};

int cli_avi_intf_init(void)
{
	return cli_register_commands(s_avi_intf_commands, AVI_INTF_CMD_CNT);
}

