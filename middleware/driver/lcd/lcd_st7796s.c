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

#include <driver/gpio.h>
#include <driver/media_types.h>
#include <driver/lcd_types.h>
#include "lcd_disp_hal.h"

#include "lcd_devices.h"

#define SLEEP_OUT          0x11
#define COMMAND_1          0xf0
#define COMMAND_2          0xf0
#define PLOAR_CONVERT      0xb4
#define DISP_OUT_CTRL      0xe8
#define POWER_CTRL1        0xc1
#define POWER_CTRL2        0xc2
#define VCOM_CTRL          0xc5
#define CATHODE_CTRL       0xe0
#define ANODE_CTRL         0xe1
#define COLOR_MODE         0x3a
#define DISPLAY_ON         0x29
#define ROW_SET            0x2b
#define COLUMN_SET         0x2a
#define RAM_WRITE          0x2c
#define CONTINUE_WRITE     0x3c
#define MEM_ACCESS_CTRL    0x36

#define TAG "st7796s"
#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)


const static uint32_t param_sleep_out[1]  = {0x00};
const static uint32_t param_command1[1]   = {0xc3};
const static uint32_t param_command2[1]   = {0x96};
const static uint32_t param_memacess[1]   = {0x48};
const static uint32_t param_color_md[1]   = {0x05};
const static uint32_t param_polar_cv[1]   = {0x01};
const static uint32_t param_disp_out[8]   = {0x40, 0x8a, 0x00, 0x00, 0x29, 0x19, 0xa5, 0x33};
const static uint32_t param_power_1[1]    = {0x06};
const static uint32_t param_power_2[1]    = {0xa7};
const static uint32_t param_vcom_ctl[1]   = {0x18};
const static uint32_t param_cath_ctl1[14] = {0xf0, 0x09, 0x0b, 0x06, 0x04, 0x15, 0x2f, 0x54, 0x42, 0x3c, 0x17, 0x14, 0x18, 0x1b};
const static uint32_t param_cath_ctl2[14] = {0xf0, 0x09, 0x0b, 0x06, 0x04, 0x03, 0x2d, 0x43, 0x42, 0x3b, 0x16, 0x14, 0x17, 0x1b};
const static uint32_t param_command12[1]  = {0x3c};
const static uint32_t param_command22[1]  = {0x69};
const static uint32_t param_display_on[1] = {0x00 };

void lcd_st7796s_init(void)
{
	LOGI("%s\n", __func__);

	rtos_delay_milliseconds(131);
	rtos_delay_milliseconds(131);

	lcd_hal_8080_cmd_send(0, SLEEP_OUT, (uint32_t *)param_sleep_out);
	rtos_delay_milliseconds(120);

	lcd_hal_8080_cmd_send(1, COMMAND_1, (uint32_t *)param_command1);
	lcd_hal_8080_cmd_send(1, COMMAND_2, (uint32_t *)param_command2);
	lcd_hal_8080_cmd_send(1, MEM_ACCESS_CTRL, (uint32_t *)param_memacess);
	lcd_hal_8080_cmd_send(1, COLOR_MODE, (uint32_t *)param_color_md);
	lcd_hal_8080_cmd_send(1, PLOAR_CONVERT, (uint32_t *)param_polar_cv);
	lcd_hal_8080_cmd_send(8, DISP_OUT_CTRL, (uint32_t *)param_disp_out);
	lcd_hal_8080_cmd_send(1, POWER_CTRL1, (uint32_t *)param_power_1);
	lcd_hal_8080_cmd_send(1, POWER_CTRL2, (uint32_t *)param_power_2);
	lcd_hal_8080_cmd_send(1, VCOM_CTRL, (uint32_t *)param_vcom_ctl);
	lcd_hal_8080_cmd_send(14, CATHODE_CTRL, (uint32_t *)param_cath_ctl1);
	lcd_hal_8080_cmd_send(14, ANODE_CTRL, (uint32_t *)param_cath_ctl2);
	lcd_hal_8080_cmd_send(1, COMMAND_1, (uint32_t *)param_command12);
	lcd_hal_8080_cmd_send(1, COMMAND_2, (uint32_t *)param_command22);

	rtos_delay_milliseconds(120);
	lcd_hal_8080_cmd_send(0, DISPLAY_ON, (uint32_t *)param_display_on);
}

void lcd_st7796s_set_display_mem_area(uint16 xs, uint16 xe, uint16 ys, uint16 ye)
{
	uint16 xs_l, xs_h, xe_l, xe_h;
	uint16 ys_l, ys_h, ye_l, ye_h;

	xs_h = xs >> 8;
	xs_l = xs & 0xff;

	xe_h = xe >> 8;
	xe_l = xe & 0xff;

	ys_h = ys >> 8;
	ys_l = ys & 0xff;

	ye_h = ye >> 8;
	ye_l = ye & 0xff;

	uint32_t param_clumn[4] = {xs_h, xs_l, xe_h, xe_l};
	uint32_t param_row[4] = {ys_h, ys_l, ye_h, ye_l};

	lcd_hal_8080_cmd_send(4, 0x2a, param_clumn);
	lcd_hal_8080_cmd_send(4, 0x2b, param_row);
}

static const lcd_mcu_t lcd_mcu =
{
	.clk = LCD_60M,

	.set_display_area = lcd_st7796s_set_display_mem_area,
};

const lcd_device_t lcd_device_st7796s =
{
	.id = LCD_DEVICE_ST7796S,
	.name = "st7796s",
	.type = LCD_TYPE_MCU8080,
	.ppi = PPI_320X480,
	.mcu = &lcd_mcu,
	.init = lcd_st7796s_init,
};

