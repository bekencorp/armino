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

#include <driver/dma.h>
#include <driver/int.h>
#include <driver/gpio.h>
#include <os/mem.h>
#include <driver/jpeg_enc.h>
#include "clock_driver.h"
#include "gpio_driver.h"
#include "jpeg_driver.h"
#include "jpeg_hal.h"
#include "power_driver.h"
#include "sys_driver.h"
#include <modules/pm.h>

typedef struct {
	jpeg_isr_t isr_handler;
	void *param;
} jpeg_isr_handler_t;

typedef struct {
	jpeg_hal_t hal;
	jpeg_isr_handler_t jpeg_isr_handler[JPEG_ISR_MAX];
} jpeg_driver_t;

typedef struct {
	uint8_t  set_enable  : 1; // 0:1/enable:disable config jpeg_encode
	uint8_t  auto_enable : 1; // enable/disable set encode size
	uint16_t up_size;
	uint16_t low_size;
} jpeg_encode_size_t;

#define JPEG_RETURN_ON_NOT_INIT() do {\
	if (!s_jpeg_driver_is_init) {\
		return BK_ERR_JPEG_NOT_INIT;\
	}\
} while(0)

static jpeg_driver_t s_jpeg = {0};
static bool s_jpeg_driver_is_init = false;
static jpeg_encode_size_t jpeg_encode_size =
{
	.set_enable = false,
	.auto_enable = false,
	.up_size = 40 * 1024,
	.low_size = 10 * 1024,
};

static void jpeg_isr(void);

static void jpeg_clk_mclk_config_set(const jpeg_config_t *config)
{
	switch (config->clk)
	{
		case JPEG_96M_MCLK_16M:
			sys_drv_set_jpeg_clk_sel(JPEG_SYS_CLK_480M);
			sys_drv_set_clk_div_mode1_clkdiv_jpeg(4);
			jpeg_hal_set_mclk_div(&s_jpeg.hal, JPEG_MCLK_DIV_6);
			break;

		case JPEG_96M_MCLK_24M:
			sys_drv_set_jpeg_clk_sel(JPEG_SYS_CLK_480M);
			sys_drv_set_clk_div_mode1_clkdiv_jpeg(4);
			jpeg_hal_set_mclk_div(&s_jpeg.hal, JPEG_MCLK_DIV_4);
			break;

		case JPEG_120M_MCLK_20M:
			sys_drv_set_jpeg_clk_sel(JPEG_SYS_CLK_480M);
			sys_drv_set_clk_div_mode1_clkdiv_jpeg(3);
			jpeg_hal_set_mclk_div(&s_jpeg.hal, JPEG_MCLK_DIV_6);
			break;

		case JPEG_120M_MCLK_30M:
			sys_drv_set_jpeg_clk_sel(JPEG_SYS_CLK_480M);
			sys_drv_set_clk_div_mode1_clkdiv_jpeg(3);
			jpeg_hal_set_mclk_div(&s_jpeg.hal, JPEG_MCLK_DIV_4);
			break;

		default:
			JPEG_LOGI("use JPEG_96M_MCLK_24M set\r\n");
			sys_drv_set_jpeg_clk_sel(JPEG_SYS_CLK_480M);
			sys_drv_set_clk_div_mode1_clkdiv_jpeg(4);
			jpeg_hal_set_mclk_div(&s_jpeg.hal, JPEG_MCLK_DIV_4);
	}

	// bus clk always on
	sys_drv_set_jpeg_disckg(1);
}

static void jpeg_init_common(const jpeg_config_t *config)
{
	/*
	* 1) set jpeg_clk, mclk and bus clk always on for bk7256
	* 2) enable power of jpeg
	* 3) enable jpeg ststem interrupt
	* 4) enable dvp io of clk(mclk/pclk, may be use auxs_clk as mclk, need attenation)
	*/
#if (!CONFIG_YUV_BUF)
	jpeg_clk_mclk_config_set(config);
#endif

	bk_pm_clock_ctrl(PM_CLK_ID_JPEG, CLK_PWR_CTRL_PWR_UP);
	sys_drv_int_enable(JPEGENC_INTERRUPT_CTRL_BIT);
	bk_jpeg_enc_set_gpio_enable(1, JPEG_GPIO_CLK);
}

static void jpeg_deinit_common(void)
{
	/*
	* 1) stop jpeg encode mode
	* 2) stop jpeg yuv mode only for bk7256
	* 3) close all jpeg enable interrupt
	* 4) power off jpeg clk
	* 5) power off auxs clk if mclk from auxs
	* 6) bus clk enable on module of jpeg enable only for bk7256
	* 7) close jpeg interrupt of system
	* 8) unmap all dvp io
	*/
	jpeg_hal_stop_common(&s_jpeg.hal, JPEG_ENC_MODE);
#if (!CONFIG_YUV_BUF)
	jpeg_hal_stop_common(&s_jpeg.hal, JPEG_YUV_MODE);
#endif
	jpeg_hal_reset_config_to_default(&s_jpeg.hal);
	bk_pm_clock_ctrl(PM_CLK_ID_JPEG, CLK_PWR_CTRL_PWR_DOWN);
	bk_pm_clock_ctrl(PM_CLK_ID_AUXS, CLK_PWR_CTRL_PWR_DOWN);
	sys_drv_set_jpeg_disckg(0);
	sys_drv_int_disable(JPEGENC_INTERRUPT_CTRL_BIT);

	bk_jpeg_enc_set_gpio_enable(0, JPEG_GPIO_ALL);
}

bk_err_t bk_jpeg_enc_driver_init(void)
{
	if (s_jpeg_driver_is_init) {
		return BK_OK;
	}

	bk_jpeg_enc_set_gpio_enable(0, JPEG_GPIO_ALL);

	os_memset(&s_jpeg, 0, sizeof(s_jpeg));
	jpeg_hal_init(&s_jpeg.hal);
	bk_int_isr_register(INT_SRC_JPEG, jpeg_isr, NULL);
	s_jpeg_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_jpeg_enc_driver_deinit(void)
{
	if (!s_jpeg_driver_is_init) {
		return BK_OK;
	}

	bk_int_isr_unregister(INT_SRC_JPEG);

	os_memset(&s_jpeg, 0, sizeof(s_jpeg));
	s_jpeg_driver_is_init = false;

	return BK_OK;
}

bk_err_t bk_jpeg_enc_init(const jpeg_config_t *config)
{
	BK_RETURN_ON_NULL(config);
	JPEG_RETURN_ON_NOT_INIT();

	// set cpu frequent to 320M
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_JPEG, PM_CPU_FRQ_320M);
	//power on
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_VIDP_JPEG_EN, PM_POWER_MODULE_STATE_ON);

	jpeg_init_common(config);

	jpeg_hal_switch_mode(&s_jpeg.hal, config);

	return BK_OK;
}

bk_err_t bk_jpeg_enc_deinit(void)
{
	JPEG_RETURN_ON_NOT_INIT();

	jpeg_deinit_common();

	// set cpu frequent to default value
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_JPEG, PM_CPU_FRQ_DEFAULT);
	// power off
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_VIDP_JPEG_EN, PM_POWER_MODULE_STATE_OFF);

	return BK_OK;
}

bk_err_t bk_jpeg_enc_partial_display_init(const jpeg_partial_offset_config_t *offset_config)
{
	JPEG_RETURN_ON_NOT_INIT();
	jpeg_hal_partial_display_offset_config(&s_jpeg.hal, offset_config);
	jpeg_hal_enable_partial_display(&s_jpeg.hal, 1);

	return BK_OK;
}

bk_err_t bk_jpeg_enc_partial_display_deinit(void)
{
	JPEG_RETURN_ON_NOT_INIT();
	jpeg_hal_enable_partial_display(&s_jpeg.hal, 0);

	return BK_OK;
}

bk_err_t bk_jpeg_enc_mode_switch(jpeg_config_t *config)
{
	JPEG_RETURN_ON_NOT_INIT();

	jpeg_hal_switch_mode(&s_jpeg.hal, config);

	return BK_OK;
}

bk_err_t bk_jpeg_enc_set_enable(uint8_t enable, uint8_t mode)
{
	JPEG_RETURN_ON_NOT_INIT();

	if (enable) {
		jpeg_hal_start_common(&s_jpeg.hal, mode);
	} else {
		jpeg_hal_stop_common(&s_jpeg.hal, mode);
	}

	return BK_OK;
}

bk_err_t bk_jpeg_set_em_base_addr(uint8_t *address)
{
	JPEG_RETURN_ON_NOT_INIT();
	jpeg_hal_set_em_base_addr(&s_jpeg.hal, address);
	return BK_OK;
}

uint32_t bk_jpeg_get_em_base_addr(void)
{
	return jpeg_hal_get_jpeg_share_mem_addr();
}

bk_err_t bk_jpeg_enc_yuv_fmt_sel(uint32_t value)
{
	JPEG_RETURN_ON_NOT_INIT();

	jpeg_hal_yuv_fmt_sel(&s_jpeg.hal, value);

	return BK_OK;
}

uint32_t bk_jpeg_enc_get_frame_size(void)
{
	JPEG_RETURN_ON_NOT_INIT();

	return jpeg_hal_get_frame_byte_number(&s_jpeg.hal);
}

bk_err_t bk_jpeg_enc_register_isr(jpeg_isr_type_t type_id, jpeg_isr_t isr, void *param)
{
	JPEG_RETURN_ON_NOT_INIT();
	uint32_t int_level = rtos_disable_int();
	s_jpeg.jpeg_isr_handler[type_id].isr_handler = isr;
	s_jpeg.jpeg_isr_handler[type_id].param = param;

	switch (type_id)
	{
		case JPEG_EOY:
			jpeg_hal_enable_end_yuv_int(&s_jpeg.hal);
			break;
		case JPEG_HEAD_OUTPUT:
			jpeg_hal_enable_head_output_int(&s_jpeg.hal);
			break;
		case JPEG_SOF:
			jpeg_hal_enable_start_frame_int(&s_jpeg.hal);
			break;
		case JPEG_EOF:
			jpeg_hal_enable_end_frame_int(&s_jpeg.hal);
			break;
		case JPEG_VSYNC_NEGEDGE:
			jpeg_hal_enable_vsync_negedge_int(&s_jpeg.hal);
			break;
		case JPEG_LINE_CLEAR:
			jpeg_hal_enable_line_clear_int(&s_jpeg.hal);
			break;
		case JPEG_FRAME_ERR:
			jpeg_hal_enable_frame_error_int(&s_jpeg.hal);
			break;
		default:
			break;
	}

	rtos_enable_int(int_level);

	return BK_OK;
}

bk_err_t bk_jpeg_enc_unregister_isr(jpeg_isr_type_t type_id)
{
	JPEG_RETURN_ON_NOT_INIT();
	uint32_t int_level = rtos_disable_int();
	s_jpeg.jpeg_isr_handler[type_id].isr_handler = NULL;
	s_jpeg.jpeg_isr_handler[type_id].param = NULL;

	switch (type_id)
	{
		case JPEG_EOY:
			jpeg_hal_disable_end_yuv_int(&s_jpeg.hal);
			break;
		case JPEG_HEAD_OUTPUT:
			jpeg_hal_disable_head_output_int(&s_jpeg.hal);
			break;
		case JPEG_SOF:
			jpeg_hal_disable_start_frame_int(&s_jpeg.hal);
			break;
		case JPEG_EOF:
			jpeg_hal_disable_end_frame_int(&s_jpeg.hal);
			break;
		case JPEG_VSYNC_NEGEDGE:
			jpeg_hal_disable_vsync_negedge_int(&s_jpeg.hal);
			break;
		case JPEG_LINE_CLEAR:
			jpeg_hal_disable_line_clear_int(&s_jpeg.hal);
			break;
		case JPEG_FRAME_ERR:
			jpeg_hal_disable_frame_error_int(&s_jpeg.hal);
			break;
		default:
			break;
	}

	rtos_enable_int(int_level);
	return BK_OK;
}

bk_err_t bk_jpeg_enc_set_gpio_enable(uint8_t enable, jpeg_gpio_mode_t mode)
{
	jpeg_gpio_map_t jpeg_gpio_map_table[] = JPEG_GPIO_MAP;

	if (enable)
	{
		if (mode == JPEG_GPIO_CLK)
		{
			for (uint32_t i = 0; i < 2; i++) {
				gpio_dev_unmap(jpeg_gpio_map_table[i].gpio_id);
				gpio_dev_map(jpeg_gpio_map_table[i].gpio_id, jpeg_gpio_map_table[i].dev);
			}
		}
		else if (mode == JPEG_GPIO_DATA)
		{
			for (uint32_t i = 2; i < JPEG_GPIO_PIN_NUMBER; i++) {
				gpio_dev_unmap(jpeg_gpio_map_table[i].gpio_id);
				gpio_dev_map(jpeg_gpio_map_table[i].gpio_id, jpeg_gpio_map_table[i].dev);
			}
		}
		else if (mode == JPEG_GPIO_HSYNC_DATA)
		{
			gpio_dev_unmap(jpeg_gpio_map_table[2].gpio_id);
			gpio_dev_map(jpeg_gpio_map_table[2].gpio_id, jpeg_gpio_map_table[2].dev);
			for (uint32_t i = 4; i < JPEG_GPIO_PIN_NUMBER; i++) {
				gpio_dev_unmap(jpeg_gpio_map_table[i].gpio_id);
				gpio_dev_map(jpeg_gpio_map_table[i].gpio_id, jpeg_gpio_map_table[i].dev);
			}
		}
		else // JPEG_GPIO_ALL
		{
			for (uint32_t i = 0; i < JPEG_GPIO_PIN_NUMBER; i++) {
				gpio_dev_unmap(jpeg_gpio_map_table[i].gpio_id);
				gpio_dev_map(jpeg_gpio_map_table[i].gpio_id, jpeg_gpio_map_table[i].dev);
			}
		}
	}
	else
	{
		if (mode == JPEG_GPIO_CLK)
		{
			for (uint32_t i = 0; i < 2; i++) {
				gpio_dev_unmap(jpeg_gpio_map_table[i].gpio_id);
			}
		}
		else if (mode == JPEG_GPIO_DATA)
		{
			for (uint32_t i = 2; i < JPEG_GPIO_PIN_NUMBER; i++) {
				gpio_dev_unmap(jpeg_gpio_map_table[i].gpio_id);
			}
		}
		else if (mode == JPEG_GPIO_HSYNC_DATA)
		{
			gpio_dev_unmap(jpeg_gpio_map_table[2].gpio_id);
			for (uint32_t i = 4; i < JPEG_GPIO_PIN_NUMBER; i++) {
				gpio_dev_unmap(jpeg_gpio_map_table[i].gpio_id);
			}
		}
		else //JPEG_GPIO_ALL
		{
			for (uint32_t i = 0; i < JPEG_GPIO_PIN_NUMBER; i++) {
				gpio_dev_unmap(jpeg_gpio_map_table[i].gpio_id);
			}
		}
	}

	return BK_OK;
}


bk_err_t bk_jpeg_enc_get_fifo_addr(uint32_t *fifo_addr)
{
	*fifo_addr = JPEG_R_RX_FIFO;
	return BK_OK;
}

bk_err_t bk_jpeg_enc_set_auxs(uint32_t cksel, uint32_t ckdiv)
{
	gpio_dev_unmap(JPEG_GPIO_AUXS);
	gpio_dev_map(JPEG_GPIO_AUXS, GPIO_DEV_CLK_AUXS);
	sys_hal_set_auxs_clk_sel(cksel);
	sys_hal_set_auxs_clk_div(ckdiv);
	bk_pm_clock_ctrl(PM_CLK_ID_AUXS, CLK_PWR_CTRL_PWR_UP);

	return BK_OK;
}

bk_err_t bk_jpeg_enc_mclk_enable(void)
{
	JPEG_RETURN_ON_NOT_INIT();

	//power on
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_VIDP_JPEG_EN, PM_POWER_MODULE_STATE_ON);

	sys_drv_set_jpeg_clk_sel(JPEG_SYS_CLK_480M);
	sys_drv_set_clk_div_mode1_clkdiv_jpeg(4);
	sys_drv_set_jpeg_disckg(1);

	jpeg_hal_enable_clk(&s_jpeg.hal, 0);
	bk_pm_clock_ctrl(PM_CLK_ID_JPEG, CLK_PWR_CTRL_PWR_UP);

	bk_jpeg_enc_set_gpio_enable(1, JPEG_GPIO_CLK);

	return BK_OK;
}

bk_err_t bk_jpeg_enc_encode_config(uint8_t enable, uint16_t up_size, uint16_t low_size)
{
	JPEG_RETURN_ON_NOT_INIT();

	uint32_t int_level = rtos_disable_int();

	if (enable && low_size > 0 && up_size > low_size)
	{
		jpeg_encode_size.up_size = up_size;
		jpeg_encode_size.low_size = low_size;
	}

	jpeg_encode_size.set_enable = true;

	jpeg_encode_size.auto_enable = enable;

	rtos_enable_int(int_level);

	return BK_OK;
}

bk_err_t bk_jpeg_enc_soft_reset(void)
{
	JPEG_RETURN_ON_NOT_INIT();

	uint32_t int_level = rtos_disable_int();

	jpeg_hal_soft_reset(&s_jpeg.hal);

	rtos_enable_int(int_level);

	return BK_OK;
}

static void jpeg_isr(void)
{
	jpeg_hal_t *hal = &s_jpeg.hal;
	uint32_t int_status = jpeg_hal_get_interrupt_status(hal);
	JPEG_LOGD("[jpeg_isr] int_status:%x\r\n", int_status);

	jpeg_hal_clear_interrupt_status(hal, int_status);

	if (jpeg_hal_is_frame_start_int_triggered(hal, int_status)) {
		if (s_jpeg.jpeg_isr_handler[JPEG_SOF].isr_handler) {
			s_jpeg.jpeg_isr_handler[JPEG_SOF].isr_handler(0, s_jpeg.jpeg_isr_handler[JPEG_SOF].param);
		}
	}

	if (jpeg_hal_is_frame_end_int_triggered(hal, int_status)) {
		if (s_jpeg.jpeg_isr_handler[JPEG_EOF].isr_handler) {
			s_jpeg.jpeg_isr_handler[JPEG_EOF].isr_handler(0, s_jpeg.jpeg_isr_handler[JPEG_EOF].param);
		}

		if (jpeg_encode_size.set_enable)
		{
			if (jpeg_encode_size.auto_enable)
			{
				jpeg_hal_enable_bitrate_ctrl(&s_jpeg.hal, 1);

				jpeg_hal_set_target_size(&s_jpeg.hal, jpeg_encode_size.up_size, jpeg_encode_size.low_size);
			}
			else
			{
				jpeg_hal_enable_bitrate_ctrl(&s_jpeg.hal, 0);
			}

			jpeg_encode_size.set_enable = false;
		}
	}

	if (jpeg_hal_is_yuv_end_int_triggered(hal, int_status)) {
		if (s_jpeg.jpeg_isr_handler[JPEG_EOY].isr_handler) {
			s_jpeg.jpeg_isr_handler[JPEG_EOY].isr_handler(0, s_jpeg.jpeg_isr_handler[JPEG_EOY].param);
		}
	}

	if (jpeg_hal_is_head_output_int_triggered(hal, int_status)) {
		if (s_jpeg.jpeg_isr_handler[JPEG_HEAD_OUTPUT].isr_handler) {
			s_jpeg.jpeg_isr_handler[JPEG_HEAD_OUTPUT].isr_handler(0, s_jpeg.jpeg_isr_handler[JPEG_HEAD_OUTPUT].param);
		}
	}

	if (jpeg_hal_is_sync_negedge_int_triggered(hal, int_status)) {
		if (s_jpeg.jpeg_isr_handler[JPEG_VSYNC_NEGEDGE].isr_handler) {
			s_jpeg.jpeg_isr_handler[JPEG_VSYNC_NEGEDGE].isr_handler(0, s_jpeg.jpeg_isr_handler[JPEG_VSYNC_NEGEDGE].param);
		}
	}

	if (jpeg_hal_is_line_clear_int_triggered(hal, int_status)) {
		if (s_jpeg.jpeg_isr_handler[JPEG_LINE_CLEAR].isr_handler) {
			s_jpeg.jpeg_isr_handler[JPEG_LINE_CLEAR].isr_handler(0, s_jpeg.jpeg_isr_handler[JPEG_LINE_CLEAR].param);
		}
	}

	if (jpeg_hal_is_frame_error_int_triggered(hal, int_status)) {
		JPEG_LOGD("jpeg decode error!\r\n");
		if (s_jpeg.jpeg_isr_handler[JPEG_FRAME_ERR].isr_handler) {
			s_jpeg.jpeg_isr_handler[JPEG_FRAME_ERR].isr_handler(0, s_jpeg.jpeg_isr_handler[JPEG_FRAME_ERR].param);
		}
	}
}

