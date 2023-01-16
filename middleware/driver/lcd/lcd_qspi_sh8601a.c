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


#include <common/bk_include.h>
#include <driver/lcd_qspi_types.h>
#include <driver/pwm.h>
#include <driver/pwm_types.h>
#include "bk_misc.h"

#define LCD_QSPI_SHA8601A_REGISTER_WRITE_COMMAND		0x02
#define LCD_QSPI_SHA8601A_REGISTER_READ_COMMAND			0x03

#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
extern beken_semaphore_t lcd_qspi_semaphore;
#endif

static const lcd_qspi_init_cmd_t sh8601a_init_cmds[] = {
	{0x11, {0x00}, 0},
	{0xff, {0x05}, 1},
	{0x2a, {0x00, 0x00, 0x01, 0xc5}, 4},
	{0x2b, {0x00, 0x00, 0x01, 0xc5}, 4},
	{0x44, {0x01, 0xc2}, 2},
	{0x35, {0x00}, 1},
	{0x53, {0x28}, 1},
	{0xc4, {0x84}, 1},
	{0x29, {0x00}, 0},
};

static uint8_t sh8601a_cmd[4] = {0x12, 0x00, 0x2c, 0x00};

#if CONFIG_PWM
static bk_err_t bk_lcd_qspi_sh8601a_backlight_init(uint8_t percent)
{
	pwm_init_config_t config = {0};
	BK_LOG_ON_ERR(bk_pwm_driver_init());

	if (percent > 100)
	{
		percent = 100;
	}

	config.period_cycle = 100;
	config.duty_cycle = percent;
	BK_LOG_ON_ERR(bk_pwm_init(PWM_ID_2, &config));
	BK_LOG_ON_ERR(bk_pwm_start(PWM_ID_2));

	return BK_OK;
}

static bk_err_t bk_lcd_qspi_sh8601a_backlight_deinit(void)
{
	BK_LOG_ON_ERR(bk_pwm_stop(PWM_ID_2));
	BK_LOG_ON_ERR(bk_pwm_deinit(PWM_ID_2));

	return BK_OK;
}

static bk_err_t bk_lcd_qspi_sh8601a_set_backlight(uint8_t percent)
{
	pwm_period_duty_config_t config = {0};

	if (percent > 100)
	{
		percent = 100;
	}

	config.period_cycle = 100;
	config.duty_cycle = percent;

	bk_pwm_set_period_duty(PWM_ID_2, &config);

	return BK_OK;
}
#endif

const lcd_qspi_device_t lcd_qspi_device_sh8601a =
{
	.id = LCD_QSPI_DEVICE_SH8601A,
	.name = "sh8601a",
	.ppi = PPI_454X454,
	.refresh_method = LCD_QSPI_REFRESH_BY_FRAME,
	.reg_write_cmd = LCD_QSPI_SHA8601A_REGISTER_WRITE_COMMAND,
	.reg_read_config = {0},
	.pixel_write_config.cmd = sh8601a_cmd,
	.pixel_write_config.cmd_len = sizeof(sh8601a_cmd),
	.init_cmd = sh8601a_init_cmds,
	.device_init_cmd_len = sizeof(sh8601a_init_cmds) / sizeof(lcd_qspi_init_cmd_t),
	.backlight_init = bk_lcd_qspi_sh8601a_backlight_init,
	.backlight_set = bk_lcd_qspi_sh8601a_set_backlight,
	.backlight_deinit = bk_lcd_qspi_sh8601a_backlight_deinit,
	.refresh_config = {0},
	.frame_len = (PPI_454X454 >> 16) * (PPI_454X454 & 0xFFFF) * 3,
};


