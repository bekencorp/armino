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


#define LCD_QSPI_ST77903_SAT61478M_REGISTER_WRITE_COMMAND		0xDE
#define LCD_QSPI_ST77903_SAT61478M_REGISTER_READ_COMMAND		0xDD
#define LCD_QSPI_ST77903_SAT61478M_HSYNC_COMMAND				0x60
#define LCD_QSPI_ST77903_SAT61478M_VSYNC_COMMAND				0x61
#define LCD_QSPI_ST77903_SAT61478M_VSW	(2)
#define LCD_QSPI_ST77903_SAT61478M_HFP	(4)
#define LCD_QSPI_ST77903_SAT61478M_HBP	(4)


static const lcd_qspi_init_cmd_t st77903_sat61478m_init_cmds[] = {
	{0xf0, {0xc3}, 1},
	{0xf0, {0x96}, 1},
	{0xf0, {0xa5}, 1},
	{0xc1, {0x11, 0x08, 0xad, 0x13}, 4},
	{0xc2, {0x11, 0x08, 0xad, 0x13}, 4},
	{0xc3, {0x44, 0x04, 0x44, 0x04}, 4},
	{0xc4, {0x44, 0x04, 0x44, 0x04}, 4},
	{0xc5, {0x48, 0x80}, 2},
	{0xd6, {0x00}, 1},
	{0xd7, {0x00}, 1},
	{0xe0, {0xd0, 0x17, 0x1c, 0x0b, 0x08, 0x06, 0x3b, 0x44, 0x4f, 0x07, 0x13, 0x14, 0x2e, 0x33}, 14},
	{0xe1, {0xd0, 0x18, 0x1c, 0x0b, 0x07, 0x05, 0x3b, 0x33, 0x4f, 0x07, 0x14, 0x14, 0x2e, 0x33}, 14},
	{0xe5, {0x58, 0xf5, 0x66, 0x33, 0x22, 0x25, 0x10, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77}, 14},
	{0xe6, {0x58, 0xf5, 0x66, 0x33, 0x22, 0x25, 0x10, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77}, 14},
	{0xec, {0x00, 0x55, 0x00, 0x00, 0x00, 0x08}, 6},
	{0x36, {0x0c}, 1},
#if (CONFIG_LCD_QSPI_PIXEL_DEPTH_BYTE == 2)
	{0x3a, {0x05}, 1},	// RGB565
#elif (CONFIG_LCD_QSPI_PIXEL_DEPTH_BYTE == 3)
	{0x3a, {0x07}, 1},	// RGB888
#endif
	{0xb2, {0x09}, 1},
	{0xb3, {0x01}, 1},
	{0xb4, {0x01}, 1},
	{0xb5, {0x00, 0x08, 0x00, 0x08}, 4},
	{0xb6, {0xef, 0x2c}, 2},
	{0xa4, {0xc0, 0x63}, 2},
	{0xa5, {0x00, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x2a, 0xba, 0x02}, 9},
	{0xa6, {0x00, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x2a, 0xba, 0x02}, 9},
	{0xba, {0x5a, 0x1a, 0x45, 0x01, 0x23, 0x01, 0x00}, 7},
	{0xbb, {0x00, 0x20, 0x00, 0x25, 0x83, 0x87, 0x18, 0x00}, 8},
	{0xbc, {0x00, 0x24, 0x00, 0x25, 0x83, 0x87, 0x18, 0x00}, 8},
	{0xbd, {0x21, 0x12, 0x99, 0xff, 0x67, 0x58, 0x85, 0x76, 0xab, 0xff, 0x03}, 11},
	{0xed, {0xc3}, 1},
	{0xe4, {0x40, 0x08 ,0x2f}, 3},
	{0x35, {0x00}, 1},
	{0x21, {0x00}, 0},
	{0x11, {0x00}, 0},
	{0xff, {0x78}, 1},
	{0x29, {0x00}, 0},
};

static uint8_t st77903_sat61478m_cmd[4] = {0xDE, 0x00, 0x60, 0x00};

#if (CONFIG_MASTER_CORE) && (CONFIG_PWM)
#define LCD_QSPI_ST77903_SAT61478M_BACKLIGHT_PWM_ID		PWM_ID_0

static bk_err_t bk_lcd_qspi_st77903_sat61478m_backlight_init(uint8_t percent)
{
	pwm_init_config_t config = {0};
	BK_LOG_ON_ERR(bk_pwm_driver_init());

	if (percent > 100)
	{
		percent = 100;
	}

	config.period_cycle = 100;
	config.duty_cycle = percent;
	BK_LOG_ON_ERR(bk_pwm_init(LCD_QSPI_ST77903_SAT61478M_BACKLIGHT_PWM_ID, &config));
	BK_LOG_ON_ERR(bk_pwm_start(LCD_QSPI_ST77903_SAT61478M_BACKLIGHT_PWM_ID));

	return BK_OK;
}

static bk_err_t bk_lcd_qspi_st77903_sat61478m_backlight_deinit(void)
{
	BK_LOG_ON_ERR(bk_pwm_stop(LCD_QSPI_ST77903_SAT61478M_BACKLIGHT_PWM_ID));
	BK_LOG_ON_ERR(bk_pwm_deinit(LCD_QSPI_ST77903_SAT61478M_BACKLIGHT_PWM_ID));

	return BK_OK;
}

static bk_err_t bk_lcd_qspi_st77903_sat61478m_set_backlight(uint8_t percent)
{
	pwm_period_duty_config_t config = {0};

	if (percent > 100)
	{
		percent = 100;
	}

	config.period_cycle = 100;
	config.duty_cycle = percent;

	bk_pwm_set_period_duty(LCD_QSPI_ST77903_SAT61478M_BACKLIGHT_PWM_ID, &config);

	return BK_OK;
}
#endif


//st77903 screen without display ram, and need to send data continuously.
const lcd_qspi_device_t lcd_qspi_device_st77903_sat61478m =
{
	.id = LCD_QSPI_DEVICE_ST77903_SAT61478M,
	.name = "st77903_sat61478m",
	.ppi = PPI_360X480,
	.refresh_method = LCD_QSPI_REFRESH_BY_LINE,
	.reg_write_cmd = LCD_QSPI_ST77903_SAT61478M_REGISTER_WRITE_COMMAND,
	.reg_read_cmd = LCD_QSPI_ST77903_SAT61478M_REGISTER_READ_COMMAND,
	.reg_read_config = {0},
	.pixel_write_config.cmd = st77903_sat61478m_cmd,
	.pixel_write_config.cmd_len = sizeof(st77903_sat61478m_cmd),
	.init_cmd = st77903_sat61478m_init_cmds,
	.device_init_cmd_len = sizeof(st77903_sat61478m_init_cmds) / sizeof(lcd_qspi_init_cmd_t),
#if (CONFIG_MASTER_CORE) && (CONFIG_PWM)
	.backlight_init = bk_lcd_qspi_st77903_sat61478m_backlight_init,
	.backlight_set = bk_lcd_qspi_st77903_sat61478m_set_backlight,
	.backlight_deinit = bk_lcd_qspi_st77903_sat61478m_backlight_deinit,
#else
	.backlight_init = NULL,
	.backlight_set = NULL,
	.backlight_deinit = NULL,
#endif
	.refresh_config.hsync_cmd = LCD_QSPI_ST77903_SAT61478M_HSYNC_COMMAND,
	.refresh_config.vsync_cmd = LCD_QSPI_ST77903_SAT61478M_VSYNC_COMMAND,
	.refresh_config.vsw = LCD_QSPI_ST77903_SAT61478M_VSW,
	.refresh_config.hfp = LCD_QSPI_ST77903_SAT61478M_HFP,
	.refresh_config.hbp = LCD_QSPI_ST77903_SAT61478M_HBP,
#if (CONFIG_LCD_QSPI_PIXEL_DEPTH_BYTE == 2)
	.frame_len = (PPI_360X480 >> 16) * (PPI_360X480 & 0xFFFF) * 2,
#else
	.frame_len = (PPI_360X480 >> 16) * (PPI_360X480 & 0xFFFF) * 3,
#endif
};



