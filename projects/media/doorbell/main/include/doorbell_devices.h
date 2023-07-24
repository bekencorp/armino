#ifndef __DOORBELL_DEVICES_H__
#define __DOORBELL_DEVICES_H__

#include "doorbell_transmission.h"


int doorbell_get_supported_camera_devices(int opcode, db_channel_t *channel, doorbell_transmission_send_t cb);
int doorbell_get_supported_lcd_devices(int opcode, db_channel_t *channel, doorbell_transmission_send_t cb);
void doorbell_devices_deinit(void);
int doorbell_devices_init(void);

int doorbell_devices_set_camera_transfer_callback(const void *cb);
int doorbell_devices_set_audio_transfer_callback(const void *cb);

int doorbell_camera_turn_on(uint16_t id, uint16_t width, uint16_t height, uint16_t image_format);
int doorbell_camera_turn_off(void);

int doorbell_audio_turn_on(BOOL aec, BOOL uac, int sample_rate);
int doorbell_audio_turn_off(void);
void doorbell_audio_data_callback(uint8_t *data, uint32_t length);

int doorbell_display_turn_on(uint16_t id, uint16_t rotate);
int doorbell_display_turn_off(void);

#endif
