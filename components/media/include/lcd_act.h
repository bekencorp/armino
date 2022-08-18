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
} lcd_state_t;


typedef struct
{
	uint8_t debug : 1;
	uint8_t rotate : 1;
	uint8_t step_mode : 1;
	uint8_t step_trigger : 1;
	lcd_state_t state;
	uint32_t param;
	uint16_t src_pixel_x;
	uint16_t src_pixel_y;
	uint16_t lcd_pixel_x;
	uint16_t lcd_pixel_y;
	uint32_t pixel_size;
	frame_buffer_t *jpeg_frame;
	frame_buffer_t *decoder_frame;
	frame_buffer_t *rotate_frame;
	frame_buffer_t *ping_frame;
	frame_buffer_t *pong_frame;
	frame_buffer_t *display_frame;
} lcd_info_t;

void lcd_event_handle(uint32_t event, uint32_t param);
lcd_state_t get_lcd_state(void);
void set_lcd_state(lcd_state_t state);
void lcd_init(void);
void lcd_frame_complete_notify(frame_buffer_t *buffer);

void lcd_act_rotate_degree90(uint32_t param);
void lcd_act_rotate_complete(frame_buffer_t *frame);


#ifdef __cplusplus
}
#endif
