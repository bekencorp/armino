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
	jpeg_isr_handler_t jpeg_isr_handler[ISR_MAX];
} jpeg_driver_t;

#define JPEG_RETURN_ON_NOT_INIT() do {\
	if (!s_jpeg_driver_is_init) {\
		return BK_ERR_JPEG_NOT_INIT;\
	}\
} while(0)

static uint8_t jpeg_dma_rx_id = 0;
static jpeg_driver_t s_jpeg = {0};
static bool s_jpeg_driver_is_init = false;

static void jpeg_isr(void);

#if (CONFIG_SYSTEM_CTRL)
static void jpeg_power_config_set(const jpeg_config_t *config)
{
	sys_drv_set_jpeg_clk_sel(1);
	sys_drv_set_clk_div_mode1_clkdiv_jpeg(config->sys_clk_div);
	sys_drv_set_jpeg_disckg(1);
	bk_pm_clock_ctrl(PM_CLK_ID_JPEG, CLK_PWR_CTRL_PWR_UP);
}
#endif

static void jpeg_init_gpio(void)
{
	jpeg_gpio_map_t jpeg_gpio_map_table[] = JPEG_GPIO_MAP;
#if (CONFIG_SYSTEM_CTRL)

	for (uint32_t i = 0; i < 2; i++) {
		gpio_dev_unmap(jpeg_gpio_map_table[i].gpio_id);
		gpio_dev_map(jpeg_gpio_map_table[i].gpio_id, jpeg_gpio_map_table[i].dev);
		if (i == 0) {
			// enhance mclk capability
			bk_gpio_set_capacity(jpeg_gpio_map_table[i].gpio_id, 3);
		}
	}
#else
	for(uint32_t i = 0; i < JPEG_GPIO_PIN_NUMBER; i++) {
		gpio_dev_unmap(jpeg_gpio_map_table[i].gpio_id);
		gpio_dev_map(jpeg_gpio_map_table[i].gpio_id, jpeg_gpio_map_table[i].dev);
	}
#endif
}

static void jpeg_deinit_gpio(void)
{
	jpeg_gpio_map_t jpeg_gpio_map_table[] = JPEG_GPIO_MAP;

	for(uint32_t i = 0; i < JPEG_GPIO_PIN_NUMBER; i++) {
		gpio_dev_unmap(jpeg_gpio_map_table[i].gpio_id);
	}
}

static void jpeg_dma_rx_init(const jpeg_config_t *config)
{
	dma_config_t dma_config = {0};

	if (config->yuv_mode == 0) {
		dma_config.mode = DMA_WORK_MODE_REPEAT;
		dma_config.chan_prio = 0;
		dma_config.src.dev = DMA_DEV_JPEG;
		dma_config.src.width = DMA_DATA_WIDTH_32BITS;
		dma_config.src.start_addr = JPEG_R_RX_FIFO;
		dma_config.dst.dev = DMA_DEV_DTCM;
		dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
		dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
		dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
		dma_config.dst.start_addr = (uint32_t)config->rx_buf;
		dma_config.dst.end_addr = (uint32_t)(config->rx_buf + config->rx_buf_len);
		jpeg_dma_rx_id = config->dma_channel;

		BK_LOG_ON_ERR(bk_dma_init(jpeg_dma_rx_id, &dma_config));
		BK_LOG_ON_ERR(bk_dma_set_transfer_len(jpeg_dma_rx_id, config->node_len));
		BK_LOG_ON_ERR(bk_dma_register_isr(jpeg_dma_rx_id, NULL, config->dma_rx_finish_handler));
		BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(jpeg_dma_rx_id));
		BK_LOG_ON_ERR(bk_dma_start(jpeg_dma_rx_id));
	}
}

static void jpeg_dma_rx_deinit(void)
{
	BK_LOG_ON_ERR(bk_dma_stop(jpeg_dma_rx_id));
	jpeg_dma_rx_id = 0;
}

static void jpeg_init_common(const jpeg_config_t *config)
{
#if (CONFIG_SYSTEM_CTRL)
	jpeg_power_config_set(config);
	sys_drv_int_enable(JPEGENC_INTERRUPT_CTRL_BIT);
#else
	power_jpeg_pwr_up();
	icu_enable_jpeg_interrupt();
#endif

	jpeg_init_gpio();
}

static void jpeg_deinit_common(void)
{
	jpeg_hal_stop_common(&s_jpeg.hal, JPEG_ENC_MODE);
	jpeg_hal_reset_config_to_default(&s_jpeg.hal);
#if (CONFIG_SYSTEM_CTRL)
	bk_pm_clock_ctrl(PM_CLK_ID_JPEG, CLK_PWR_CTRL_PWR_DOWN);
	sys_hal_set_jpeg_disckg(0);
	sys_drv_int_disable(JPEGENC_INTERRUPT_CTRL_BIT);
#else
	icu_disable_jpeg_interrupt();
	power_jpeg_pwr_down();
#endif

	jpeg_dma_rx_deinit();
	jpeg_deinit_gpio();
}

bk_err_t bk_jpeg_enc_driver_init(void)
{
	if (s_jpeg_driver_is_init) {
		return BK_OK;
	}
#if CONFIG_SYSTEM_CTRL
	//power on
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_VIDP_JPEG_EN, PM_POWER_MODULE_STATE_ON);
#endif

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
#if CONFIG_SYSTEM_CTRL
	// power off
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_VIDP_JPEG_EN, PM_POWER_MODULE_STATE_OFF);
#endif

	s_jpeg_driver_is_init = false;

	return BK_OK;
}

bk_err_t bk_jpeg_enc_init(const jpeg_config_t *config)
{
	BK_RETURN_ON_NULL(config);
	JPEG_RETURN_ON_NOT_INIT();

	jpeg_init_common(config);

	if (config->yuv_mode) {
		jpeg_hal_set_yuv_config(&s_jpeg.hal, config);
	} else {
		jpeg_hal_set_encode_config(&s_jpeg.hal, config);
	}

	jpeg_dma_rx_init(config);

	return BK_OK;
}

bk_err_t bk_jpeg_enc_deinit(void)
{
	jpeg_deinit_common();

	return BK_OK;
}

bk_err_t bk_jpeg_enc_partial_display_init(const jpeg_partial_offset_config_t *offset_config)
{
	JPEG_RETURN_ON_NOT_INIT();
	jpeg_hal_partial_display_offset_config(&s_jpeg.hal, offset_config);
	jpeg_hal_enable_partial_display(&s_jpeg.hal, 1);

	return BK_OK;
}

bk_err_t bk_jpeg_enc_partial_display_deinit(const jpeg_partial_offset_config_t *offset_config)
{
	jpeg_hal_enable_partial_display(&s_jpeg.hal, 0);;
	os_memset((void *)offset_config, 0, sizeof(jpeg_partial_offset_config_t));
	jpeg_hal_partial_display_offset_config(&s_jpeg.hal, offset_config);

	return BK_OK;
}

bk_err_t bk_jpeg_enc_dvp_init(const jpeg_config_t *config)
{
	BK_RETURN_ON_NULL(config);
	JPEG_RETURN_ON_NOT_INIT();

	jpeg_init_common(config);

	if (config->yuv_mode) {
		jpeg_hal_set_yuv_config(&s_jpeg.hal, config);
	} else {
		jpeg_hal_set_encode_config(&s_jpeg.hal, config);
	}

	return BK_OK;
}

bk_err_t bk_jpeg_enc_dvp_deinit(void)
{
	jpeg_hal_stop_common(&s_jpeg.hal, JPEG_ENC_MODE);
	jpeg_hal_stop_common(&s_jpeg.hal, JPEG_YUV_MODE);
	jpeg_hal_reset_config_to_default(&s_jpeg.hal);

	bk_pm_clock_ctrl(PM_CLK_ID_JPEG, CLK_PWR_CTRL_PWR_DOWN);
	sys_hal_set_jpeg_disckg(0);
#if (CONFIG_SYSTEM_CTRL)
	sys_drv_int_disable(JPEGENC_INTERRUPT_CTRL_BIT);
#endif
	jpeg_deinit_gpio();


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
	uint32_t int_level = rtos_disable_int();
	s_jpeg.jpeg_isr_handler[type_id].isr_handler = isr;
	s_jpeg.jpeg_isr_handler[type_id].param = param;

	rtos_enable_int(int_level);
	return BK_OK;
}

bk_err_t bk_jpeg_enc_dvp_gpio_enable(void)
{
	jpeg_gpio_map_t jpeg_gpio_map_table[] = JPEG_GPIO_MAP;
	for (uint32_t i = 2; i < JPEG_GPIO_PIN_NUMBER; i++) {
		gpio_dev_unmap(jpeg_gpio_map_table[i].gpio_id);
		gpio_dev_map(jpeg_gpio_map_table[i].gpio_id, jpeg_gpio_map_table[i].dev);
	}

	return BK_OK;
}

bk_err_t bk_jpeg_enc_get_fifo_addr(uint32_t *fifo_addr)
{
	JPEG_RETURN_ON_NOT_INIT();
	*fifo_addr = JPEG_R_RX_FIFO;
	return BK_OK;
}

bk_err_t bk_jpeg_enc_enable_int(uint32_t type)
{
	JPEG_RETURN_ON_NOT_INIT();
	if (type & JPEG_END_YUV_INT) {
		jpeg_hal_enable_end_yuv_int(&s_jpeg.hal);
	}

	if (type & JPEG_HEAD_OUTPUT_INT) {
		jpeg_hal_enable_head_output_int(&s_jpeg.hal);
	}

	if (type & JPEG_START_FRAME_INT) {
		jpeg_hal_enable_start_frame_int(&s_jpeg.hal);
	}

	if (type & JPEG_END_FRAME_INT) {
		jpeg_hal_enable_end_frame_int(&s_jpeg.hal);
	}

	if (type & JPEG_VSYNC_NEGEDGE_INT) {
		jpeg_hal_enable_vsync_negedge_int(&s_jpeg.hal);
	}

	return BK_OK;
}

bk_err_t bk_jpeg_enc_disable_int(uint32_t type)
{
	JPEG_RETURN_ON_NOT_INIT();
	if (type & JPEG_END_YUV_INT) {
		jpeg_hal_disable_end_yuv_int(&s_jpeg.hal);
	}

	if (type & JPEG_HEAD_OUTPUT_INT) {
		jpeg_hal_disable_head_output_int(&s_jpeg.hal);
	}

	if (type & JPEG_START_FRAME_INT) {
		jpeg_hal_disable_start_frame_int(&s_jpeg.hal);
	}

	if (type & JPEG_END_FRAME_INT) {
		jpeg_hal_disable_end_frame_int(&s_jpeg.hal);
	}

	if (type & JPEG_VSYNC_NEGEDGE_INT) {
		jpeg_hal_disable_vsync_negedge_int(&s_jpeg.hal);
	}

	return BK_OK;
}

bk_err_t bk_jpeg_enc_set_auxs(uint32_t cksel, uint32_t ckdiv)
{
#if CONFIG_SYSTEM_CTRL
	sys_hal_set_auxs_clk_sel(cksel);
	sys_hal_set_auxs_clk_div(ckdiv);
	return BK_OK;
#else
	return BK_FAIL;
#endif
}

bk_err_t bk_jpeg_enc_mclk_enable(void)
{
	JPEG_RETURN_ON_NOT_INIT();

	sys_drv_set_jpeg_clk_sel(1);
	sys_drv_set_clk_div_mode1_clkdiv_jpeg(4);
	sys_drv_set_jpeg_disckg(1);

	jpeg_hal_enable_clk(&s_jpeg.hal, 0);
	bk_pm_clock_ctrl(PM_CLK_ID_JPEG, CLK_PWR_CTRL_PWR_UP);

	return BK_OK;
}

bk_err_t bk_jpeg_enc_enable_encode_auto_ctrl(uint8_t enable)
{
	JPEG_RETURN_ON_NOT_INIT();

	jpeg_hal_enable_bitrate_ctrl(&s_jpeg.hal, enable);

	return BK_OK;
}

bk_err_t bk_jpeg_enc_set_target_size(uint32_t up_size, uint32_t low_size)
{
	JPEG_RETURN_ON_NOT_INIT();

	jpeg_hal_set_target_size(&s_jpeg.hal, up_size, low_size);

	return BK_OK;
}

static void jpeg_isr(void)
{
	jpeg_hal_t *hal = &s_jpeg.hal;
	uint32_t int_status = jpeg_hal_get_interrupt_status(hal);
	jpeg_hal_clear_interrupt_status(hal, int_status);

	if (jpeg_hal_is_frame_start_int_triggered(hal, int_status)) {
		if (s_jpeg.jpeg_isr_handler[START_OF_FRAME].isr_handler) {
			s_jpeg.jpeg_isr_handler[START_OF_FRAME].isr_handler(0, s_jpeg.jpeg_isr_handler[START_OF_FRAME].param);
		}
	}

	if (jpeg_hal_is_frame_end_int_triggered(hal, int_status)) {
		if (s_jpeg.jpeg_isr_handler[END_OF_FRAME].isr_handler) {
			s_jpeg.jpeg_isr_handler[END_OF_FRAME].isr_handler(0, s_jpeg.jpeg_isr_handler[END_OF_FRAME].param);
		}
	}

	if (jpeg_hal_is_yuv_end_int_triggered(hal, int_status)) {
		if (s_jpeg.jpeg_isr_handler[END_OF_YUV].isr_handler) {
			s_jpeg.jpeg_isr_handler[END_OF_YUV].isr_handler(0, s_jpeg.jpeg_isr_handler[END_OF_YUV].param);
		}
	}

	if (jpeg_hal_is_head_output_int_triggered(hal, int_status)) {
		if (s_jpeg.jpeg_isr_handler[HEAD_OUTPUT].isr_handler) {
			s_jpeg.jpeg_isr_handler[HEAD_OUTPUT].isr_handler(0, s_jpeg.jpeg_isr_handler[HEAD_OUTPUT].param);
		}
	}

	if (jpeg_hal_is_sync_negedge_int_triggered(hal, int_status)) {
		if (s_jpeg.jpeg_isr_handler[VSYNC_NEGEDGE].isr_handler) {
			s_jpeg.jpeg_isr_handler[VSYNC_NEGEDGE].isr_handler(0, s_jpeg.jpeg_isr_handler[VSYNC_NEGEDGE].param);
		}
	}
}

