#include <common/bk_include.h>
#include "bk_arm_arch.h"

#include "bk_uart.h"
#include <os/os.h>
#include <common/bk_kernel_err.h>

#include "avilib.h"
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

#define J2AVI_MALLOC            sdram_malloc   // sdram_malloc         os_malloc       
#define J2AVI_FREE              sdram_free     // sdram_free           sdram_free
#define J2AVI_PRINT             os_printf
#define J2AVI_TRUE              1
#define J2AVI_FALSE             0


JPEG2AVI_PTR g_jpeg2avi = NULL;

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

		AVI_set_video(g_jpeg2avi->m_pAviHandle, 1280,
					  720, g_jpeg2avi->m_nFps, VideoFmtMJPG);

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

	J2AVI_LOCK();

	jpeg2avi_stop_record();
	jpeg2avi_stop_read();

	J2AVI_UNLOCK();

	rtos_deinit_mutex(&g_jpeg2avi->m_csLock);

	J2AVI_FREE(g_jpeg2avi);
	g_jpeg2avi = NULL;

	return 0;
}

////////////////////////////////////////

void j2avi_test(int argc, char **argv)
{
	int ret;

	if (jpeg2avi_init() != 1) {
		J2AVI_PRINT("jpeg2avi_init failed\r\n");
		return;
	}

	jpeg2avi_set_video_param(320, 240, 20);
	jpeg2avi_set_audio_param(1, 8000, 16);

	jpeg2avi_start_record("sd/jpeg2avi_000.avi");

	void *pdata = (void *)(0x00001000);
	int ldata = 1024 * 30;

	//REG_WRITE((0x00802800+(14*4)), 0x02);
	//REG_WRITE((0x00802800+(14*4)), 0x00);

	//REG_WRITE((0x00802800+(15*4)), 0x02);
	//REG_WRITE((0x00802800+(15*4)), 0x00);

	int loop_cnt = 2;

	while (loop_cnt --) {
		//REG_WRITE((0x00802800+(14*4)), 0x02);
		ret = jpeg2avi_input_data(pdata, ldata, eTypeVideo);
		//REG_WRITE((0x00802800+(14*4)), 0x00);
		if (ret <= 0) {
			J2AVI_PRINT("jpeg2avi_input video failed\r\n");
			goto j2avi_exit;
		}

		//REG_WRITE((0x00802800+(15*4)), 0x02);
		ret = jpeg2avi_input_data(pdata, ldata, eTypeAudio);
		//REG_WRITE((0x00802800+(15*4)), 0x00);
		if (ret <= 0) {
			J2AVI_PRINT("jpeg2avi_input audio failed\r\n");
			goto j2avi_exit;
		}
	}

j2avi_exit:

	jpeg2avi_stop_record();
	jpeg2avi_deinit();

}
FINSH_FUNCTION_EXPORT_ALIAS(j2avi_test, __cmd_j2avi_test, j2avi test);


