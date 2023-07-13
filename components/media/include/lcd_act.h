// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <common/bk_include.h>
#include <driver/media_types.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
	LCD_STATE_DISABLED,
	LCD_STATE_ENABLED,
	LCD_STATE_DISPLAY,
} lcd_state_t;


typedef enum
{
	DISPLAY_EVENT,
	LOAD_JPEG_EVENT,
	DISPLAY_EXIT_EVENT,
} display_event_t;

typedef enum
{
	SOFTWARE_DECODING_CPU0,
	SOFTWARE_DECODING_CPU1,
	HARDWARE_DECODING,
} decode_mode_t;


typedef struct
{
	uint8_t debug : 1;
	uint8_t step_mode : 1;
	uint8_t step_trigger : 1;
	uint8_t dma2d_blend : 1;
	uint8_t decode_mode;
	media_rotate_t rotate;
	lcd_state_t state;
} lcd_info_t;

typedef struct {
	uint32_t device_ppi;			/**< lcd open by ppi */
	char * device_name; 			/**< lcd open by lcd Driver IC name */
} lcd_open_t;


typedef struct {
	uint32_t image_addr;  /**< normally flash jpeg  addr */
	uint32_t img_length;
} lcd_display_t;

typedef enum {
	WIFI_LEVEL_0 = 0,
	WIFI_LEVEL_1,
	WIFI_LEVEL_2,
	WIFI_LEVEL_3,
	WIFI_LEVEL_FULL,
	WIFI_LEVEL_MAX
}WIFI_LEVVEL_T;

//added for blend 
#define MAX_BLEND_NAME_LEN 16
typedef struct {
	uint8_t time_data[MAX_BLEND_NAME_LEN]; 
	uint8_t ver_data[MAX_BLEND_NAME_LEN]; 
	uint8_t year_to_data[MAX_BLEND_NAME_LEN]; 
	uint8_t wifi_data; 
	uint8_t lcd_blend_type; 
} lcd_blend_data_t;

typedef enum {
	LCD_BLEND_NONE=0x0,
	LCD_BLEND_WIFI=0x01,
	LCD_BLEND_TIME=0x02,
	LCD_BLEND_VERSION=0x04,
	LCD_BLEND_DATA=0x08,
} lcd_blend_type_em;

typedef struct {
	uint8_t data[MAX_BLEND_NAME_LEN]; 
	uint8_t blend_on;
	lcd_blend_type_em lcd_blend_type; 
} lcd_blend_msg_t;




///added end

void lcd_event_handle(uint32_t event, uint32_t param);
lcd_state_t get_lcd_state(void);
void set_lcd_state(lcd_state_t state);
uint8_t get_decode_mode(void);
void set_decode_mode(uint8_t mode);

void lcd_init(void);
void lcd_frame_complete_notify(frame_buffer_t *buffer);

void lcd_act_rotate_degree90(uint32_t param);
void lcd_act_vuyy_resize(uint32_t param);
void lcd_jpeg_dec_sw(uint32_t param);

void lcd_calc_init(void);

void lcd_decoder_task_stop(void);
void camera_display_task_stop(void);
void camera_display_task_start(media_rotate_t rotate);
void jpeg_display_task_stop(void);
bk_err_t lcd_blend_handle(frame_buffer_t *frame);

void lcd_set_logo_on(int status);

#ifdef __cplusplus
}
#endif
