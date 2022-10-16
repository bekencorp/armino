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
#include <driver/uvc_camera_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	APP_CAMERA_DVP,
	APP_CAMERA_YUV,
	APP_CAMERA_MIX,
	APP_CAMERA_UVC,
	APP_CAMERA_INVALIED,
} app_camera_type_t;

typedef enum
{
	APP_LCD_RGB,
	APP_LCD_MCU,
} app_lcd_type_t;

typedef enum
{
	APP_LCD_NO_USE_GUI = 0,
	APP_LCD_USE_GUI = 1,
} app_lcd_open_type_t;

bk_err_t media_app_camera_open(app_camera_type_t type, media_ppi_t ppi);
bk_err_t media_app_camera_close(app_camera_type_t type);
bk_err_t media_app_transfer_open(void *setup_cfg);
bk_err_t media_app_transfer_close(void);
bk_err_t media_app_lcd_open(void *lcd_open);
bk_err_t media_app_lcd_open_withgui(void *lcd_open);
bk_err_t media_app_lcd_close(void);
bk_err_t media_app_capture(char *name);
bk_err_t media_app_lcd_set_backlight(uint8_t level);
bk_err_t media_app_mailbox_test(void);
bk_err_t media_app_lcd_rotate(bool enable);
bk_err_t media_app_dump_display_frame(void);
bk_err_t media_app_dump_decoder_frame(void);
bk_err_t media_app_dump_jpeg_frame(void);
bk_err_t media_app_lcd_step_mode(bool enable);
bk_err_t media_app_lcd_step_trigger(void);
bk_err_t media_app_transfer_pause(bool pause);
bk_err_t media_app_lcd_display(char *name, uint32_t lcd_ppi);
bk_err_t media_app_lcd_display_beken(void);


#ifdef __cplusplus
}
#endif
