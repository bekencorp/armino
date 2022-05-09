#ifndef __APP_JPEG2AVI_H__
#define __APP_JPEG2AVI_H__

#include "avilib.h"

typedef enum in_typte {
	eTypeVideo     = 0,
	eTypeAudio,
} IN_TYPE;

int jpeg2avi_init(void);
int jpeg2avi_deinit(void);

// for record video & audio
void jpeg2avi_set_video_param(int nWidth, int nHeight, int nFps);
void jpeg2avi_set_audio_param(int nChannel, int nSamplePerSec, int nBit);

int jpeg2avi_start_record(const char *szFileName);
int jpeg2avi_input_data(void *pData, const int nLength, int eType);
void jpeg2avi_stop_record(void);

// for read
int jpeg2avi_start_read(const char *szFileName);
int jpeg2avi_read_data(void *pData, const int nLength, int eType);
void jpeg2avi_stop_read(void);
int jpeg2avi_audio_format_get(void);


#endif
