#ifndef __DOORBELL_DEVICES_H__
#define __DOORBELL_DEVICES_H__

#include "doorbell_transmission.h"

typedef enum
{
    CODEC_FORMAT_UNKNOW = 0,
    CODEC_FORMAT_G711A = 1,
    CODEC_FORMAT_PCM = 2,
    CODEC_FORMAT_G711U = 3,
} codec_format_t;

typedef struct
{
	uint16_t id;
	uint16_t width;
	uint16_t height;
	uint16_t format;
	uint16_t protocol;
} camera_parameters_t;

typedef struct
{
	uint8_t aec;
	uint8_t uac;
	uint8_t rmt_recoder_fmt; /* codec_format_t */
	uint8_t rmt_player_fmt; /* codec_format_t */
	uint32_t rmt_recorder_sample_rate;
	uint32_t rmt_player_sample_rate;
} audio_parameters_t;


typedef enum
{
	AA_UNKNOWN = 0,
    AA_ECHO_DEPTH = 1,
    AA_MAX_AMPLITUDE = 2,
    AA_MIN_AMPLITUDE = 3,
    AA_NOISE_LEVEL = 4,
    AA_NOISE_PARAM = 5,
} audio_acoustics_t;


int doorbell_get_supported_camera_devices(int opcode, db_channel_t *channel, doorbell_transmission_send_t cb);
int doorbell_get_supported_lcd_devices(int opcode, db_channel_t *channel, doorbell_transmission_send_t cb);
void doorbell_devices_deinit(void);
int doorbell_devices_init(void);

int doorbell_devices_set_camera_transfer_callback(const void *cb);
int doorbell_devices_set_audio_transfer_callback(const void *cb);

int doorbell_camera_turn_on(camera_parameters_t *parameters);
int doorbell_camera_turn_off(void);

int doorbell_audio_turn_on(audio_parameters_t *parameters);
int doorbell_audio_turn_off(void);
int doorbell_audio_acoustics(uint32_t index, uint32_t param);
void doorbell_audio_data_callback(uint8_t *data, uint32_t length);

int doorbell_display_turn_on(uint16_t id, uint16_t rotate);
int doorbell_display_turn_off(void);

#endif
