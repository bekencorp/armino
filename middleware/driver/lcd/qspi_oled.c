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
#include <os/mem.h>
#include <driver/qspi.h>
#include <driver/dma2d.h>
#include <driver/psram.h>
#include <driver/psram_types.h>
#include "gpio_driver.h"
#include <driver/gpio.h>
#include <driver/qspi_oled.h>
#include <driver/qspi_oled_types.h>
#include <driver/pwm.h>
#include <driver/pwm_types.h>
#include "sys_driver.h"
#include "psram_hal.h"
#include "qspi_hal.h"


typedef struct {
	qspi_hal_t hal;
	uint8_t id_init_bits;
} qspi_driver_t;

#define QSPI_OLED_DISPLAY_PSRAM_ADDR		0x68000000
#define QSPI_OLED_X_PIXEL					0x1C6
#define QSPI_OLED_Y_PIXEL					0x1C6

static qspi_driver_t s_qspi_oled = {0};


static bk_err_t bk_qspi_oled_device_register_write(qspi_register_write_config_t *reg_config)
{
	uint8_t i = 0;
	uint32_t cmd_c_l = 0;
	uint32_t cmd_c_h = 0;

	qspi_hal_set_cmd_c_l(&s_qspi_oled.hal, 0);
	qspi_hal_set_cmd_c_h(&s_qspi_oled.hal, 0);

	for (i = 0; i < reg_config->cmd_len; i++) {
		if (i <= 3) {
			cmd_c_h = qspi_hal_get_cmd_c_h(&s_qspi_oled.hal);
			cmd_c_h |= ((reg_config->cmd[i]) << i * 8);
			qspi_hal_set_cmd_c_h(&s_qspi_oled.hal, cmd_c_h);

		} else {
			cmd_c_l = qspi_hal_get_cmd_c_l(&s_qspi_oled.hal);
			cmd_c_l |= ((reg_config->cmd[i]) << (i - 4) * 8);
			qspi_hal_set_cmd_c_l(&s_qspi_oled.hal, (reg_config->cmd[i]) << (i - 4) * 8);
		}
	}

	qspi_hal_set_cmd_c_cfg1(&s_qspi_oled.hal, 0x3A8);
	qspi_hal_cmd_c_start(&s_qspi_oled.hal);
	qspi_hal_wait_cmd_done(&s_qspi_oled.hal);

	return BK_OK;
}

static bk_err_t bk_qspi_oled_device_init(void)
{
	qspi_hal_set_cmd_c_h(&s_qspi_oled.hal, 0x00110002);
	qspi_hal_set_cmd_c_cfg1(&s_qspi_oled.hal, 0x300);
	qspi_hal_cmd_c_start(&s_qspi_oled.hal);
	qspi_hal_wait_cmd_done(&s_qspi_oled.hal);

	rtos_delay_milliseconds(5);

	qspi_hal_set_cmd_c_l(&s_qspi_oled.hal, 0xC5010000);
	qspi_hal_set_cmd_c_h(&s_qspi_oled.hal, 0x002A0002);
	qspi_hal_set_cmd_c_cfg1(&s_qspi_oled.hal, 0x30000);
	qspi_hal_cmd_c_start(&s_qspi_oled.hal);
	qspi_hal_wait_cmd_done(&s_qspi_oled.hal);

	qspi_hal_set_cmd_c_l(&s_qspi_oled.hal, 0xC5010000);
	qspi_hal_set_cmd_c_h(&s_qspi_oled.hal, 0x002B0002);
	qspi_hal_set_cmd_c_cfg1(&s_qspi_oled.hal, 0x30000);
	qspi_hal_cmd_c_start(&s_qspi_oled.hal);
	qspi_hal_wait_cmd_done(&s_qspi_oled.hal);

	qspi_hal_set_cmd_c_l(&s_qspi_oled.hal, 0x0000C201);
	qspi_hal_set_cmd_c_h(&s_qspi_oled.hal, 0x00440002);
	qspi_hal_set_cmd_c_cfg1(&s_qspi_oled.hal, 0x3000);
	qspi_hal_cmd_c_start(&s_qspi_oled.hal);
	qspi_hal_wait_cmd_done(&s_qspi_oled.hal);

	qspi_hal_set_cmd_c_l(&s_qspi_oled.hal, 0x0);
	qspi_hal_set_cmd_c_h(&s_qspi_oled.hal, 0x00350002);
	qspi_hal_set_cmd_c_cfg1(&s_qspi_oled.hal, 0xC00);
	qspi_hal_cmd_c_start(&s_qspi_oled.hal);
	qspi_hal_wait_cmd_done(&s_qspi_oled.hal);

	qspi_hal_set_cmd_c_l(&s_qspi_oled.hal, 0x28);
	qspi_hal_set_cmd_c_h(&s_qspi_oled.hal, 0x00530002);
	qspi_hal_set_cmd_c_cfg1(&s_qspi_oled.hal, 0xC00);
	qspi_hal_cmd_c_start(&s_qspi_oled.hal);
	qspi_hal_wait_cmd_done(&s_qspi_oled.hal);

	qspi_hal_set_cmd_c_l(&s_qspi_oled.hal, 0x84);
	qspi_hal_set_cmd_c_h(&s_qspi_oled.hal, 0x00C40002);
	qspi_hal_set_cmd_c_cfg1(&s_qspi_oled.hal, 0xC00);
	qspi_hal_cmd_c_start(&s_qspi_oled.hal);
	qspi_hal_wait_cmd_done(&s_qspi_oled.hal);

	return BK_OK;
}


#if CONFIG_PWM
static bk_err_t bk_qspi_oled_backlight_init(uint8_t percent)
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

static bk_err_t bk_qspi_oled_backlight_deinit(void)
{
	BK_LOG_ON_ERR(bk_pwm_stop(PWM_ID_2));
	BK_LOG_ON_ERR(bk_pwm_deinit(PWM_ID_2));

	return BK_OK;
}

bk_err_t bk_qspi_oled_set_backlight(uint8_t percent)
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

static bk_err_t bk_qspi_oled_display_on(void)
{
	qspi_hal_set_cmd_c_h(&s_qspi_oled.hal, 0x00290002);
	qspi_hal_set_cmd_c_cfg1(&s_qspi_oled.hal, 0x300);
	qspi_hal_cmd_c_start(&s_qspi_oled.hal);
	qspi_hal_wait_cmd_done(&s_qspi_oled.hal);

	return BK_OK;
}

static bk_err_t bk_qspi_oled_quad_write_enable(void)
{
	qspi_hal_set_cmd_a_l(&s_qspi_oled.hal, 0x00002C00);
	qspi_hal_set_cmd_a_h(&s_qspi_oled.hal, 0x10000100);
	qspi_hal_set_cmd_a_cfg1(&s_qspi_oled.hal, 0xEAAA);
	qspi_hal_set_cmd_a_cfg2(&s_qspi_oled.hal, 0x80008000);

	return BK_OK;
}

static bk_err_t bk_qspi_oled_driver_init(void)
{
	os_memset(&s_qspi_oled, 0, sizeof(s_qspi_oled));
	qspi_hal_init(&s_qspi_oled.hal);

	return BK_OK;
}

bk_err_t bk_qspi_oled_init(void)
{
	bk_err_t ret = BK_OK;

	gpio_dev_unmap(GPIO_47);
	gpio_dev_map(GPIO_47, 0);
	bk_gpio_enable_pull(GPIO_47);
	bk_gpio_pull_up(GPIO_47);
	rtos_delay_milliseconds(10);

	ret = bk_qspi_oled_backlight_init(100);
	if (ret != BK_OK) {
		os_printf("qspi oled backlight init failed!\r\n");
		return ret;
	}
	bk_qspi_oled_driver_init();

	ret = bk_psram_init();
	if (ret != BK_OK) {
		os_printf("qspi oled psram init failed!\r\n");
		return ret;
	}
	os_printf("psram init complete!\r\n");

	bk_qspi_oled_quad_write_enable();

	bk_qspi_oled_device_init();

	bk_qspi_oled_display_on();

	bk_dma2d_driver_init();

	os_printf("qspi oled init complete!\r\n");

	return BK_OK;
}

bk_err_t bk_qspi_oled_deinit(void)
{
	bk_qspi_oled_quad_write_stop();
	bk_psram_deinit();
	bk_qspi_oled_backlight_deinit();

	return BK_OK;
}

bk_err_t bk_qspi_oled_quad_write_start(void)
{
	uint8_t oled_cmd[4] = {0x12, 0x00, 0x2c, 0x00};

	qspi_hal_force_spi_cs_low_enable(&s_qspi_oled.hal);

	qspi_register_write_config_t oled_write_cmd;
	oled_write_cmd.cmd = oled_cmd;
	oled_write_cmd.cmd_len = sizeof(oled_cmd);
	bk_qspi_oled_device_register_write(&oled_write_cmd);

	qspi_hal_disable_cmd_sck_enable(&s_qspi_oled.hal);

	return BK_OK;
}

bk_err_t bk_qspi_oled_quad_write_stop(void)
{
	qspi_hal_force_spi_cs_low_disable(&s_qspi_oled.hal);
	qspi_hal_disable_cmd_sck_disable(&s_qspi_oled.hal);

	return BK_OK;
}

void bk_qspi_oled_dma2d_fill(uint32_t width, uint32_t height, uint32_t color)
{
	dma2d_config_t dma2d_config = {0};

	dma2d_config.init.mode   		 = DMA2D_R2M;              /**< Mode Register to Memory */
	dma2d_config.init.color_mode	 = DMA2D_OUTPUT_RGB888;    /**< DMA2D Output color mode is RGB888 */
	dma2d_config.init.output_offset  = 0;                      /**< offset in output */
	dma2d_config.init.red_blue_swap  = DMA2D_RB_REGULAR;       /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;    /**< No alpha inversion for the output image */
	dma2d_config.init.data_reverse   = 1;
	bk_dma2d_init(&dma2d_config);


	if (width == 0) {
		width = QSPI_OLED_X_PIXEL / 2;
	}

	if (height == 0) {
		height = QSPI_OLED_Y_PIXEL / 2;
	}

	bk_dma2d_start_transfer(&dma2d_config, color, QSPI_OLED_DISPLAY_PSRAM_ADDR, width, height);
	while (bk_dma2d_is_transfer_busy()) {
	}
}

void bk_qspi_oled_dma2d_memcpy(uint32_t *Psrc, uint32_t xsize, uint32_t ysize)
{
	dma2d_config_t dma2d_config = {0};

	dma2d_config.init.mode         = DMA2D_M2M;             /**< Mode Memory To Memory */
	dma2d_config.init.color_mode    = DMA2D_OUTPUT_RGB888; /**< Output color mode is RGB888 */
	dma2d_config.init.output_offset = 0;                   /**< No offset on output */
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;     /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;  /**< No alpha inversion for the output image */

	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_mode = DMA2D_NO_MODIF_ALPHA;      /**< Keep original Alpha from RGB888 input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_alpha = 0xFF;                     /**< Fully opaque */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_RGB888; /**< Input color is RGB888 */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_offset = 0;                     /**< No offset in input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].red_blue_swap   = DMA2D_RB_REGULAR;      /**< No R&B swap for the input image */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_inverted = DMA2D_REGULAR_ALPHA;   /**< No alpha inversion for the input image */

	bk_dma2d_init(&dma2d_config);
	bk_dma2d_layer_config(&dma2d_config, DMA2D_FOREGROUND_LAYER);

	bk_dma2d_start_transfer(&dma2d_config, (uint32_t)Psrc, QSPI_OLED_DISPLAY_PSRAM_ADDR, xsize, ysize); 
	while (bk_dma2d_is_transfer_busy()) {}
}

