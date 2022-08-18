#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	AUD_TRAS_SAMP_RATE_8K = 0,
	AUD_TRAS_SAMP_RATE_16K,
	AUD_TRAS_SAMP_RATE_MAX,
} audio_tras_samp_rate_t;

typedef struct {
	bool aec_enable;
	audio_tras_samp_rate_t samp_rate;
	int (*audio_send_mic_data)(unsigned char *data, unsigned int len);		/**< the api is called when collecting a frame mic packet data is complete */
} audio_tras_setup_t;

#ifdef __cplusplus
}
#endif

