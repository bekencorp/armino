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
#include "lcd_devices.h"
#include "gpio_map.h"
#include <driver/lcd.h>

#if CONFIG_PWM
#include <driver/pwm.h>
#endif

static const lcd_rgb_t lcd_rgb =
{
	.clk = LCD_32M,
	.data_out_clk_edge = POSEDGE_OUTPUT,

	.hsync_pulse_width = 2,
	.vsync_pulse_width = 2,
	.hsync_back_porch = 40,
	.hsync_front_porch = 48,
	.vsync_back_porch = 32,
	.vsync_front_porch = 13,
};

static void lcd_backlight_open(void)
{
	os_printf("lcd_md0430r: backlight open.\r\n");
#if CONFIG_PWM
	os_printf("lcd_md0430r: backlight pwm init.\r\n");
	lcd_driver_backlight_init(LCD_RGB_PWM_BACKLIGHT, 100);
#endif
}

static void lcd_set_backlight(uint8_t percent)
{
#if CONFIG_PWM
	pwm_period_duty_config_t config = {0};

	if (percent > 100)
	{
		percent  = 100;
	}

	config.period_cycle = 100;
	config.duty_cycle = percent;

	bk_pwm_set_period_duty(LCD_RGB_PWM_BACKLIGHT, &config);
#endif
}

static void lcd_backlight_close(void)
{
#if CONFIG_PWM
	lcd_driver_backlight_deinit(LCD_RGB_PWM_BACKLIGHT);
#endif
}

const lcd_device_t lcd_device_md0430r =
{
	.id = LCD_DEVICE_MD0430R,
	.name = "md0430r",
	.type = LCD_TYPE_RGB565,
	.ppi = PPI_800X480,
	.rgb = &lcd_rgb,
	.backlight_open = lcd_backlight_open,
	.backlight_set = lcd_set_backlight,
	.init = NULL,
	.backlight_close = lcd_backlight_close,
	.lcd_off = NULL,
};

