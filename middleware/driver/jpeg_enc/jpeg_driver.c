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
#include <os/mem.h>
#include <driver/jpeg_enc.h>
#include "clock_driver.h"
#include "gpio_driver.h"
#include "jpeg_driver.h"
#include "jpeg_hal.h"
#include "power_driver.h"
#include "sys_driver.h"
#include <modules/pm.h>

extern void delay(int num);//TODO fix me

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

#define JPEG_RX_DMA_CHANNEL    DMA_ID_4

static jpeg_driver_t s_jpeg = {0};
static bool s_jpeg_driver_is_init = false;

static void jpeg_isr(void);

#if (CONFIG_SYSTEM_CTRL)
static void jpeg_power_config_set(const jpeg_config_t *config)
{
	sys_drv_set_jpeg_clk_sel(1);
	sys_drv_set_clk_div_mode1_clkdiv_jpeg(config->sys_clk_div);
	sys_drv_set_jpeg_disckg(1);
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_JPEG, CLK_PWR_CTRL_PWR_UP);
}
#endif

static void jpeg_init_gpio(void)
{
	jpeg_gpio_map_t jpeg_gpio_map_table[] = JPEG_GPIO_MAP;
#if (CONFIG_SYSTEM_CTRL)

	for (uint32_t i = 0; i < 2; i++) {
		gpio_dev_unmap(jpeg_gpio_map_table[i].gpio_id);
		gpio_dev_map(jpeg_gpio_map_table[i].gpio_id, jpeg_gpio_map_table[i].dev);
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

		BK_LOG_ON_ERR(bk_dma_init(JPEG_RX_DMA_CHANNEL, &dma_config));
		BK_LOG_ON_ERR(bk_dma_set_transfer_len(JPEG_RX_DMA_CHANNEL, config->node_len));
		BK_LOG_ON_ERR(bk_dma_register_isr(JPEG_RX_DMA_CHANNEL, NULL, config->dma_rx_finish_handler));
		BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(JPEG_RX_DMA_CHANNEL));
		BK_LOG_ON_ERR(bk_dma_enable_half_finish_interrupt(JPEG_RX_DMA_CHANNEL));
		BK_LOG_ON_ERR(bk_dma_start(JPEG_RX_DMA_CHANNEL));
	}
}

static void jpeg_dma_rx_deinit(void)
{
	BK_LOG_ON_ERR(bk_dma_stop(JPEG_RX_DMA_CHANNEL));
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
	jpeg_hal_stop_common(&s_jpeg.hal);
	jpeg_hal_reset_config_to_default(&s_jpeg.hal);
#if (CONFIG_SYSTEM_CTRL)
	sys_hal_set_jpeg_disckg(0);
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_JPEG, CLK_PWR_CTRL_PWR_DOWN);
	sys_drv_int_disable(JPEGENC_INTERRUPT_CTRL_BIT);
#else
	icu_disable_jpeg_interrupt();
	power_jpeg_pwr_down();
#endif

	jpeg_dma_rx_deinit();
	jpeg_deinit_gpio();
}

static void jpeg_cli_deinit_common(void)
{
	jpeg_hal_stop_common(&s_jpeg.hal);
	jpeg_hal_reset_config_to_default(&s_jpeg.hal);
#if (CONFIG_SYSTEM_CTRL)
	sys_hal_set_jpeg_disckg(0);
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_JPEG, CLK_PWR_CTRL_PWR_DOWN);
	sys_drv_int_disable(JPEGENC_INTERRUPT_CTRL_BIT);
#endif
	jpeg_deinit_gpio();
}

bk_err_t bk_jpeg_enc_driver_init(void)
{
	if (s_jpeg_driver_is_init) {
		return BK_OK;
	}
#if CONFIG_SYSTEM_CTRL
	//power on
	pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_VIDP, PM_POWER_MODULE_STATE_ON);
	sys_drv_video_power_en(0);    //temp used
	delay(100);
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
	pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_VIDP, PM_POWER_MODULE_STATE_OFF);
	sys_drv_video_power_en(1);    //temp used
	delay(100);
#endif

	s_jpeg_driver_is_init = false;

	return BK_OK;
}

bk_err_t bk_jpeg_enc_init(const jpeg_config_t *config)
{
	BK_RETURN_ON_NULL(config);
	JPEG_RETURN_ON_NOT_INIT();

	jpeg_init_common(config);
	jpeg_hal_configure(&s_jpeg.hal, config);
	jpeg_dma_rx_init(config);

	return BK_OK;
}

bk_err_t bk_jpeg_enc_deinit(void)
{
	jpeg_deinit_common();

	return BK_OK;
}

bk_err_t bk_jpeg_enc_dvp_init(const jpeg_config_t *config)
{
	BK_RETURN_ON_NULL(config);
	JPEG_RETURN_ON_NOT_INIT();

	jpeg_init_common(config);
	jpeg_hal_configure(&s_jpeg.hal, config);

	return BK_OK;
}

bk_err_t bk_jpeg_enc_dvp_deinit(void)
{
	jpeg_cli_deinit_common();

	return BK_OK;
}

bk_err_t bk_jpeg_enc_set_yuv_mode(uint32_t mode)
{
	JPEG_RETURN_ON_NOT_INIT();
	jpeg_hal_set_yuv_mode(&s_jpeg.hal, mode);

	return BK_OK;

}

bk_err_t bk_jpeg_enc_set_enable(uint8_t enable)
{
	JPEG_RETURN_ON_NOT_INIT();

	if (enable) {
		jpeg_hal_start_common(&s_jpeg.hal);
	} else {
		jpeg_hal_stop_common(&s_jpeg.hal);
	}

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

