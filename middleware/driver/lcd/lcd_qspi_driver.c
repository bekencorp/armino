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
#include <os/str.h>
#include <driver/dma2d.h>
#include <driver/psram.h>
#include <driver/psram_types.h>
#include <driver/qspi.h>
#include <driver/qspi_types.h>
#include <driver/lcd_qspi_types.h>
#include <driver/media_types.h>
#include "gpio_driver.h"
#include <driver/gpio.h>
#include "bk_misc.h"


#define LCD_QSPI_DISPLAY_PSRAM_ADDR		0x68000000

#if CONFIG_LCD_QSPI_SH8601A
extern lcd_qspi_device_t lcd_qspi_device_sh8601a;
#endif

#if CONFIG_LCD_QSPI_ST77903
extern lcd_qspi_device_t lcd_qspi_device_st77903;
#endif

static qspi_driver_t s_lcd_qspi = {0};


#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
beken_semaphore_t lcd_qspi_semaphore = NULL;

static void dma2d_transfer_complete_isr(void)
{
	bk_err_t ret = BK_OK;

	bk_dma2d_int_status_clear(DMA2D_TRANS_COMPLETE_STATUS);
	ret = rtos_set_semaphore(&lcd_qspi_semaphore);
	if (ret != kNoErr) {
		os_printf("lcd qspi semaphore set failed\r\n");
		return;
	}
}
#endif

static bk_err_t bk_lcd_qspi_driver_init(void)
{
	os_memset(&s_lcd_qspi, 0, sizeof(s_lcd_qspi));
	qspi_hal_init(&s_lcd_qspi.hal);

	return BK_OK;
}

static bk_err_t bk_lcd_qspi_hardware_reset(void)
{
	gpio_dev_unmap(GPIO_47);
	gpio_dev_map(GPIO_47, 0);
	bk_gpio_enable_pull(GPIO_47);
	bk_gpio_pull_up(GPIO_47);
	delay_ms(10);
	bk_gpio_pull_down(GPIO_47);
	delay_ms(10);
	bk_gpio_pull_up(GPIO_47);
	delay_ms(120);

	return BK_OK;
}

static bk_err_t bk_lcd_qspi_quad_write_enable(void)
{
	qspi_hal_set_cmd_a_l(&s_lcd_qspi.hal, 0x00002C00);
	qspi_hal_set_cmd_a_h(&s_lcd_qspi.hal, 0x10000100);
	qspi_hal_set_cmd_a_cfg1(&s_lcd_qspi.hal, 0xEAAA);
	qspi_hal_set_cmd_a_cfg2(&s_lcd_qspi.hal, 0x80008000);

	return BK_OK;
}

static bk_err_t bk_lcd_qspi_quad_write_start(lcd_qspi_write_config_t *reg_config, bool addr_is_4wire)
{
	uint32_t cmd_c_h = 0;

	qspi_hal_force_spi_cs_low_enable(&s_lcd_qspi.hal);

	qspi_hal_set_cmd_c_h(&s_lcd_qspi.hal, 0);

	for (uint8_t i = 0; i < reg_config->cmd_len; i++) {
		cmd_c_h = qspi_hal_get_cmd_c_h(&s_lcd_qspi.hal);
		cmd_c_h |= ((reg_config->cmd[i]) << i * 8);
		qspi_hal_set_cmd_c_h(&s_lcd_qspi.hal, cmd_c_h);
	}

	if (addr_is_4wire) {
		qspi_hal_set_cmd_c_cfg1(&s_lcd_qspi.hal, 0x3A8);
	} else {
		qspi_hal_set_cmd_c_cfg1(&s_lcd_qspi.hal, 0x300);
	}
	qspi_hal_cmd_c_start(&s_lcd_qspi.hal);
	qspi_hal_wait_cmd_done(&s_lcd_qspi.hal);

	qspi_hal_disable_cmd_sck_enable(&s_lcd_qspi.hal);

	return BK_OK;
}

static bk_err_t bk_lcd_qspi_quad_write_stop(void)
{
	qspi_hal_disable_cmd_sck_disable(&s_lcd_qspi.hal);
	qspi_hal_force_spi_cs_low_disable(&s_lcd_qspi.hal);

	return BK_OK;
}

static bk_err_t bk_lcd_qspi_send_cmd(uint8_t write_cmd, uint8_t cmd, const uint8_t *data, uint8_t data_len)
{
	qspi_hal_set_cmd_c_l(&s_lcd_qspi.hal, 0);
	qspi_hal_set_cmd_c_h(&s_lcd_qspi.hal, 0);
	qspi_hal_set_cmd_c_cfg1(&s_lcd_qspi.hal, 0);
	qspi_hal_set_cmd_c_cfg2(&s_lcd_qspi.hal, 0);

	if (data_len == 0) {
		qspi_hal_set_cmd_c_h(&s_lcd_qspi.hal, (cmd << 16 | write_cmd) & 0xFF00FF);
	} else if (data_len > 0 && data_len <= 4) {
		uint32_t value = 0;
		for (uint8_t i = 0; i < data_len; i++) {
			value = value | (data[i] << (i * 8));
		}
		qspi_hal_set_cmd_c_l(&s_lcd_qspi.hal, value);
		qspi_hal_set_cmd_c_h(&s_lcd_qspi.hal, (cmd << 16 | write_cmd) & 0xFF00FF);
	} else if (data_len > 4) {
		qspi_hal_set_cmd_c_h(&s_lcd_qspi.hal, (cmd << 16 | write_cmd) & 0xFF00FF);
		qspi_hal_set_cmd_c_cfg1(&s_lcd_qspi.hal, 0x300);
		qspi_hal_set_cmd_c_cfg2(&s_lcd_qspi.hal, data_len << 2);
		bk_qspi_write(data, data_len);
		qspi_hal_cmd_c_start(&s_lcd_qspi.hal);
		qspi_hal_wait_cmd_done(&s_lcd_qspi.hal);
		qspi_hal_set_cmd_c_cfg1(&s_lcd_qspi.hal, 0);
		qspi_hal_set_cmd_c_cfg2(&s_lcd_qspi.hal, 0);
		return BK_OK;
	}

	qspi_hal_set_cmd_c_cfg1(&s_lcd_qspi.hal, 0x3 << ((data_len + 4) * 2));
	qspi_hal_cmd_c_start(&s_lcd_qspi.hal);
	qspi_hal_wait_cmd_done(&s_lcd_qspi.hal);

	return BK_OK;
}

static bk_err_t bk_lcd_qspi_common_init(void)
{
	bk_err_t ret = BK_OK;

	bk_lcd_qspi_hardware_reset();

	bk_lcd_qspi_driver_init();

#if (CONFIG_PSRAM)
	ret = bk_psram_init();
	if (ret != BK_OK) {
		os_printf("psram init failed!\r\n");
		return BK_FAIL;
	}
#endif

	ret = bk_dma2d_driver_init();
	if (ret != BK_OK) {
		os_printf("dma2d driver init failed!\r\n");
		return BK_FAIL;
	}

#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
	ret = rtos_init_semaphore(&lcd_qspi_semaphore, 1);
	if (ret != kNoErr) {
		os_printf("lcd qspi semaphore init failed.\r\n");
		return BK_FAIL;
	}

	bk_dma2d_register_int_callback_isr(DMA2D_TRANS_COMPLETE_ISR, dma2d_transfer_complete_isr);
	if (ret != BK_OK) {
		os_printf("dma2d interrupt register failed!\r\n");
		return BK_FAIL;
	}
	bk_dma2d_int_enable(DMA2D_TRANS_COMPLETE, 1);
#endif

	bk_lcd_qspi_quad_write_enable();

	return BK_OK;
}

static bk_err_t bk_lcd_qspi_common_deinit(void)
{
#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)

	bk_err_t ret = BK_OK;

	ret = rtos_deinit_semaphore(&lcd_qspi_semaphore);
	if (ret != kNoErr) {
		os_printf("lcd qspi semaphore deinit failed.\r\n");
		return BK_FAIL;
	}
#endif

#if (CONFIG_PSRAM)
	bk_psram_deinit();
#endif

	bk_dma2d_driver_deinit();

	return BK_OK;
}

static void bk_lcd_qspi_dma2d_fill(uint32_t width, uint32_t height, uint32_t color, bool data_reverse)
{
	dma2d_config_t dma2d_config = {0};

	dma2d_config.init.mode   		 = DMA2D_R2M;              /**< Mode Register to Memory */
	dma2d_config.init.color_mode	 = DMA2D_OUTPUT_RGB888;    /**< DMA2D Output color mode is RGB888 */
	dma2d_config.init.output_offset  = 0;                      /**< offset in output */
	dma2d_config.init.red_blue_swap  = DMA2D_RB_REGULAR;       /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;    /**< No alpha inversion for the output image */
	dma2d_config.init.data_reverse   = data_reverse;
	bk_dma2d_init(&dma2d_config);

	bk_dma2d_start_transfer(&dma2d_config, color, LCD_QSPI_DISPLAY_PSRAM_ADDR, width, height);
}

static void bk_lcd_qspi_dma2d_memcpy(uint32_t *Psrc, uint32_t xsize, uint32_t ysize, bool data_reverse)
{
	dma2d_config_t dma2d_config = {0};

#if CONFIG_LVGL
	dma2d_config.init.mode         = DMA2D_M2M_PFC;
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_ARGB8888;
	dma2d_config.init.data_reverse   = data_reverse;
#else
	dma2d_config.init.mode         = DMA2D_M2M;             /**< Mode Memory To Memory */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_RGB888;	 /**< Input color is RGB888 */
#endif

	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].red_blue_swap   = DMA2D_RB_REGULAR;		/**< No R&B swap for the input image */
	dma2d_config.init.color_mode    = DMA2D_OUTPUT_RGB888; /**< Output color mode is RGB888 */
	dma2d_config.init.output_offset = 0;                   /**< No offset on output */
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;     /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;  /**< No alpha inversion for the output image */

	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_mode = DMA2D_NO_MODIF_ALPHA;      /**< Keep original Alpha from RGB888 input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_alpha = 0xFF;                     /**< Fully opaque */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_offset = 0;                     /**< No offset in input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_inverted = DMA2D_REGULAR_ALPHA;   /**< No alpha inversion for the input image */

	bk_dma2d_init(&dma2d_config);
	bk_dma2d_layer_config(&dma2d_config, DMA2D_FOREGROUND_LAYER);

	bk_dma2d_start_transfer(&dma2d_config, (uint32_t)Psrc, LCD_QSPI_DISPLAY_PSRAM_ADDR, xsize, ysize);
}

lcd_qspi_device_t *lcd_qspi_devices[] = {
#if CONFIG_LCD_QSPI_SH8601A
	&lcd_qspi_device_sh8601a,
#endif

#if CONFIG_LCD_QSPI_ST77903
	&lcd_qspi_device_st77903,
#endif
};

static bk_err_t bk_lcd_qspi_reg_data_convert(uint8_t *data, uint32_t data_len)
{
	uint32_t data_buffer[data_len];
	uint8_t i, j, data_tmp1;
	uint8_t data_tmp2[4];

	bk_qspi_read(data_buffer, data_len * 4);
	for (i = 0; i < data_len; i++) {
		for (j = 0; j < 4; j++) {
			data_tmp1 = (data_buffer[i] >> ((j * 8) + 4)) & 0x1;
			data_tmp2[j] = (data_tmp1 << 1) | ((data_buffer[i] >> (j * 8)) & 0x1);
		}

		data[i] = (data_tmp2[0] << 6) | (data_tmp2[1] << 4) | (data_tmp2[2] << 2) | (data_tmp2[3]);
//		os_printf("reg_data[%d]: 0x%02x\r\n", i, data[i]);
	}

	return BK_OK;
}

bk_err_t bk_lcd_qspi_read_data(uint8_t *data, lcd_qspi_device_t *device)
{
	qspi_hal_set_cmd_d_h(&s_lcd_qspi.hal, 0);
	qspi_hal_set_cmd_d_cfg1(&s_lcd_qspi.hal, 0);
	qspi_hal_set_cmd_d_cfg2(&s_lcd_qspi.hal, 0);

	qspi_hal_set_cmd_d_h(&s_lcd_qspi.hal, (device->reg_read_config.cmd << 16 | device->reg_read_cmd) & 0xFF00FF);
	qspi_hal_set_cmd_d_cfg1(&s_lcd_qspi.hal, 0x300);

	qspi_hal_set_cmd_d_data_line(&s_lcd_qspi.hal, 2);
	qspi_hal_set_cmd_d_data_length(&s_lcd_qspi.hal, device->reg_read_config.data_len * 4);
	qspi_hal_set_cmd_d_dummy_clock(&s_lcd_qspi.hal, device->reg_read_config.dummy_clk);
	qspi_hal_set_cmd_d_dummy_mode(&s_lcd_qspi.hal, device->reg_read_config.dummy_mode);

	qspi_hal_cmd_d_start(&s_lcd_qspi.hal);
	qspi_hal_wait_cmd_done(&s_lcd_qspi.hal);

	bk_lcd_qspi_reg_data_convert(data, device->reg_read_config.data_len);

	return BK_OK;
}

lcd_qspi_device_t *bk_lcd_qspi_get_device_by_name(char *name)
{
	uint32_t i, size = sizeof(lcd_qspi_devices) / sizeof(lcd_qspi_device_t *);

	for (i = 0; i < size; i++)
	{
		if (os_memcmp(lcd_qspi_devices[i]->name, name, os_strlen(name)) == 0)
		{
			return lcd_qspi_devices[i];
		}
	}

	return NULL;
}

lcd_qspi_device_t *bk_lcd_qspi_get_device_by_ppi(media_ppi_t ppi)
{
	uint32_t i, size = sizeof(lcd_qspi_devices) / sizeof(lcd_qspi_device_t *);

	for (i = 0; i < size; i++)
	{
		if (lcd_qspi_devices[i]->ppi == ppi)
		{
			return lcd_qspi_devices[i];
		}
	}

	return NULL;
}

bk_err_t bk_lcd_qspi_init(lcd_qspi_device_t *device)
{
	bk_err_t ret = BK_OK;
	
	if (device == NULL) {
		os_printf("lcd qspi device not found\r\n");
		return BK_FAIL;
	}

	ret = bk_lcd_qspi_common_init();
	if (ret == BK_FAIL) {
		os_printf("lcd qspi common init failed!\r\n");
		return ret;
	}

	if (device->backlight_init != NULL) {
		ret = device->backlight_init(100);
		if (ret != BK_OK) {
			os_printf("lcd qspi backlight init failed!\r\n");
			return ret;
		}
	}

	if (device->init_cmd != NULL) {
		const lcd_qspi_init_cmd_t *init = device->init_cmd;
		for (uint32_t i = 0; i < device->device_init_cmd_len; i++) {
			if (init->cmd == 0xff) {
				delay_ms(init->data[0]);
			} else {
				bk_lcd_qspi_send_cmd(device->reg_write_cmd, init->cmd, init->data, init->data_len);
			}
			init++;
		}
	} else {
		os_printf("lcd qspi device don't init\r\n");
		return BK_FAIL;
	}

	return BK_OK;
}

bk_err_t bk_lcd_qspi_deinit(lcd_qspi_device_t *device)
{
	bk_err_t ret = BK_OK;

	ret = bk_lcd_qspi_common_deinit();
	if (ret == BK_FAIL) {
		os_printf("lcd qspi common deinit failed!\r\n");
		return ret;
	}

	if (device->backlight_deinit != NULL) {
		ret = device->backlight_deinit();
		if (ret != BK_OK) {
			os_printf("lcd qspi backlight deinit failed!\r\n");
			return ret;
		}
	}

	return BK_OK;
}

bk_err_t bk_lcd_qspi_send_data(lcd_qspi_device_t *device, uint32_t *data, uint32_t data_len)
{
	bk_err_t ret = BK_OK;

	if (device->refresh_method == LCD_QSPI_REFRESH_BY_LINE) {
		for (uint16_t i = 0; i < device->refresh_config.vsw; i++) {
			bk_lcd_qspi_send_cmd(device->reg_write_cmd, device->refresh_config.vsync_cmd, NULL, 0);
			delay_us(35);
		}

		for (uint16_t i = 0; i < device->refresh_config.hfp; i++) {
			bk_lcd_qspi_send_cmd(device->reg_write_cmd, device->refresh_config.hsync_cmd, NULL, 0);
			delay_us(35);
		}

		for (uint16_t i = 0; i < device->ppi >> 16; i++) {
			bk_lcd_qspi_quad_write_start(&(device->pixel_write_config), 0);
			if (data_len == 1) {
				bk_lcd_qspi_dma2d_fill(device->ppi >> 16, 1, *data, 0);
			} else {
#if CONFIG_LVGL
				bk_lcd_qspi_dma2d_memcpy(data + i * (device->ppi >> 16), device->ppi >> 16, 1, 0);
#else
				bk_lcd_qspi_dma2d_memcpy(data + i * ((device->ppi >> 16) * 3 / 4), device->ppi >> 16, 1, 0);
#endif			
			}
#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
			ret = rtos_get_semaphore(&lcd_qspi_semaphore, 5);
			if (ret) {
				os_printf("ret = %d, lcd qspi get semaphore failed!\r\n", ret);
				return BK_FAIL;
			}
#endif
			bk_lcd_qspi_quad_write_stop();
		}

		for (uint16_t i = 0; i < device->refresh_config.hbp; i++) {
			bk_lcd_qspi_send_cmd(device->reg_write_cmd, device->refresh_config.hsync_cmd, NULL, 0);
			delay_us(30);
		}
	} else if (device->refresh_method == LCD_QSPI_REFRESH_BY_FRAME) {
		bk_lcd_qspi_quad_write_start(&(device->pixel_write_config), 1);

		if (data_len == 1) {
			bk_lcd_qspi_dma2d_fill(device->ppi >> 16, device->ppi & 0xFFFF, *data, 1);
		} else {
			bk_lcd_qspi_dma2d_memcpy(data, device->ppi >> 16, device->ppi & 0xFFFF, 1);
		}
#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
		ret = rtos_get_semaphore(&lcd_qspi_semaphore, 30);
		if (ret) {
			os_printf("ret = %d, lcd qspi get semaphore failed!\r\n", ret);
			return BK_FAIL;
		}
#endif
		bk_lcd_qspi_quad_write_stop();
	} else {
		os_printf("invalid lcd qspi refresh method\r\n");
		return BK_FAIL;
	}

	return BK_OK;
}

