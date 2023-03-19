#ifndef _AV_AUDIO_COMMON_H__
#define _AV_AUDIO_COMMON_H__


typedef struct {
	int (*av_aud_voc_send_packet)(unsigned char *data, unsigned int len);
} av_aud_voc_setup_t;

void aud_voc_start(av_aud_voc_setup_t setup);
void aud_voc_stop(void);

#endif

