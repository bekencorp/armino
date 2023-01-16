// Copyright 2022-2023 Beken
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
#include <components/system.h>
#include <driver/yuv_buf.h>
#include <driver/int.h>
#include <os/os.h>
#include <os/mem.h>
#include <modules/pm.h>
#include "sys_driver.h"
#include "yuv_buf_driver.h"
#include "yuv_buf_hal.h"

typedef struct {
	yuv_buf_isr_t isr_handler;
	void *param;
} yuv_buf_isr_handler_t;

typedef struct {
	yuv_buf_hal_t hal;
	yuv_buf_isr_handler_t yuv_buf_isr_handler[YUV_BUF_ISR_MAX];
} yuv_buf_driver_t;

#define YUV_BUF_RETURN_ON_DRIVER_NOT_INIT() do {\
	if (!s_yuv_buf_driver_is_init) {\
		return BK_ERR_YUV_BUF_DRIVER_NOT_INIT;\
	}\
} while(0)

static yuv_buf_driver_t s_yuv_buf = {0};
static bool s_yuv_buf_driver_is_init = false;

static void yuv_buf_isr(void);

static void yuv_buf_init_common(void)
{
	/* 1) power on yuv_buf
	 * 2) enable yuv_buf system interrupt
	 */
	sys_drv_yuv_buf_pwr_up();
	sys_drv_int_group2_enable(YUV_BUF_INTERRUPT_CTRL_BIT);
}

static void yuv_buf_deinit_common(void)
{
	yuv_buf_hal_reset_config_to_default(&s_yuv_buf.hal);
	/* 1) power off yuv_buf
	 * 2) disable yuv_buf system interrupt
	 */
	sys_drv_yuv_buf_pwr_down();
	sys_drv_int_group2_disable(YUV_BUF_INTERRUPT_CTRL_BIT);
}

bk_err_t bk_yuv_buf_driver_init(void)
{
	if (s_yuv_buf_driver_is_init) {
		return BK_OK;
	}

	os_memset(&s_yuv_buf, 0, sizeof(s_yuv_buf));
	bk_int_isr_register(INT_SRC_YUVB, yuv_buf_isr, NULL);
	yuv_buf_hal_init(&s_yuv_buf.hal);
	s_yuv_buf_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_yuv_buf_driver_deinit(void)
{
	if (!s_yuv_buf_driver_is_init) {
		return BK_OK;
	}
	os_memset(&s_yuv_buf, 0, sizeof(s_yuv_buf));
	bk_int_isr_unregister(INT_SRC_YUVB);
	yuv_buf_deinit_common();
	s_yuv_buf_driver_is_init = false;

	return BK_OK;
}

bk_err_t bk_yuv_buf_init(const yuv_buf_config_t *config)
{
	BK_RETURN_ON_NULL(config);
	YUV_BUF_RETURN_ON_DRIVER_NOT_INIT();

	yuv_buf_init_common();

	switch (config->work_mode) {
	case YUV_BUF_MODE_JPEG:
		yuv_buf_hal_set_jpeg_mode_config(&s_yuv_buf.hal, config);
		break;
	case YUV_BUF_MODE_YUV:
		yuv_buf_hal_set_yuv_mode_config(&s_yuv_buf.hal, config);
		break;
	case YUV_BUF_MODE_H264:
		yuv_buf_hal_set_h264_mode_config(&s_yuv_buf.hal, config);
		break;
	default:
		break;
	}

	return BK_OK;
}

bk_err_t bk_yuv_buf_deinit(void)
{
	YUV_BUF_RETURN_ON_DRIVER_NOT_INIT();

	yuv_buf_deinit_common();

	return BK_OK;
}

bk_err_t bk_yuv_buf_start(yuv_buf_work_mode_t work_mode)
{
	switch (work_mode) {
	case YUV_BUF_MODE_YUV:
		yuv_buf_hal_start_yuv_mode(&s_yuv_buf.hal);
		break;
	case YUV_BUF_MODE_JPEG:
		yuv_buf_hal_start_jpeg_mode(&s_yuv_buf.hal);
		break;
	case YUV_BUF_MODE_H264:
		yuv_buf_hal_start_h264_mode(&s_yuv_buf.hal);
		break;
	default:
		break;
	}

	return BK_OK;
}

bk_err_t bk_yuv_buf_stop(yuv_buf_work_mode_t work_mode)
{
	switch (work_mode) {
	case YUV_BUF_MODE_YUV:
		yuv_buf_hal_stop_yuv_mode(&s_yuv_buf.hal);
		break;
	case YUV_BUF_MODE_JPEG:
		yuv_buf_hal_stop_jpeg_mode(&s_yuv_buf.hal);
		break;
	case YUV_BUF_MODE_H264:
		yuv_buf_hal_stop_h264_mode(&s_yuv_buf.hal);
		break;
	default:
		break;
	}

	return BK_OK;
}

bk_err_t bk_yuv_buf_send_data_to_h264(void)
{
	yuv_buf_hal_send_data_to_h264(&s_yuv_buf.hal);

	return BK_OK;
}

bk_err_t bk_yuv_buf_enable_h264_nosensor_mode(void)
{
	yuv_buf_hal_enable_h264_nosensor_mode(&s_yuv_buf.hal);

	return BK_OK;
}

bk_err_t bk_yuv_buf_enable_mclk(void)
{
	sys_drv_yuv_buf_pwr_up();
	bk_pm_clock_ctrl(PM_CLK_ID_JPEG, CLK_PWR_CTRL_PWR_UP);
	sys_drv_h264_pwr_up();

	sys_drv_set_jpeg_clk_sel(1); //480M
	sys_drv_set_clk_div_mode1_clkdiv_jpeg(3); // freq_div=4
	yuv_buf_hal_set_mclk_div(&s_yuv_buf.hal, YUV_MCLK_DIV_3); //480M/4/2/3=20M
	yuv_buf_hal_start_yuv_mode(&s_yuv_buf.hal);

	return BK_OK;
}

bk_err_t bk_yuv_buf_set_em_base_addr(uint32_t em_base_addr)
{
	YUV_BUF_RETURN_ON_DRIVER_NOT_INIT();
	yuv_buf_hal_set_em_base_addr(&s_yuv_buf.hal, em_base_addr);
	return BK_OK;
}

bk_err_t bk_yuv_buf_register_isr(yuv_buf_isr_type_t type_id, yuv_buf_isr_t isr, void *param)
{
	uint32_t int_level = rtos_disable_int();
	s_yuv_buf.yuv_buf_isr_handler[type_id].isr_handler = isr;
	s_yuv_buf.yuv_buf_isr_handler[type_id].param = param;
	rtos_enable_int(int_level);

	return BK_OK;
}

bk_err_t bk_yuv_buf_unregister_isr(yuv_buf_isr_type_t type_id)
{
	uint32_t int_level = rtos_disable_int();
	s_yuv_buf.yuv_buf_isr_handler[type_id].isr_handler = NULL;
	s_yuv_buf.yuv_buf_isr_handler[type_id].param = NULL;
	rtos_enable_int(int_level);

	return BK_OK;
}

static void yuv_buf_isr(void)
{
	yuv_buf_hal_t *hal = &s_yuv_buf.hal;
	uint32_t int_status = yuv_buf_hal_get_interrupt_status(hal);

	YUV_BUF_LOGD("[yuv_buf_isr] int_status:%x\r\n", int_status);
	yuv_buf_hal_clear_interrupt_status(hal, int_status);

	if (yuv_buf_hal_is_vsync_negedge_int_triggered(hal, int_status)) {
		if (s_yuv_buf.yuv_buf_isr_handler[YUV_BUF_VSYNC_NEGEDGE].isr_handler) {
			s_yuv_buf.yuv_buf_isr_handler[YUV_BUF_VSYNC_NEGEDGE].isr_handler(0, s_yuv_buf.yuv_buf_isr_handler[YUV_BUF_VSYNC_NEGEDGE].param);
		}
	}

	if (yuv_buf_hal_is_yuv_arv_int_triggered(hal, int_status)) {
		if (s_yuv_buf.yuv_buf_isr_handler[YUV_BUF_YUV_ARV].isr_handler) {
			s_yuv_buf.yuv_buf_isr_handler[YUV_BUF_YUV_ARV].isr_handler(0, s_yuv_buf.yuv_buf_isr_handler[YUV_BUF_YUV_ARV].param);
		}
	}

	if (yuv_buf_hal_is_sm0_wr_int_triggered(hal, int_status)) {
		if (s_yuv_buf.yuv_buf_isr_handler[YUV_BUF_SM0_WR].isr_handler) {
			s_yuv_buf.yuv_buf_isr_handler[YUV_BUF_SM0_WR].isr_handler(0, s_yuv_buf.yuv_buf_isr_handler[YUV_BUF_SM0_WR].param);
		}
	}

	if (yuv_buf_hal_is_sm1_wr_int_triggered(hal, int_status)) {
		if (s_yuv_buf.yuv_buf_isr_handler[YUV_BUF_SM1_WR].isr_handler) {
			s_yuv_buf.yuv_buf_isr_handler[YUV_BUF_SM1_WR].isr_handler(0, s_yuv_buf.yuv_buf_isr_handler[YUV_BUF_SM1_WR].param);
		}
	}

	if (yuv_buf_hal_is_fifo_full_int_triggered(hal, int_status)) {
		YUV_BUF_LOGE("yuv internal fifo is full\r\n");
	}

	if (yuv_buf_hal_is_enc_line_done_int_triggered(hal, int_status)) {
		if (s_yuv_buf.yuv_buf_isr_handler[YUV_BUF_ENC_LINE].isr_handler) {
			s_yuv_buf.yuv_buf_isr_handler[YUV_BUF_ENC_LINE].isr_handler(0, s_yuv_buf.yuv_buf_isr_handler[YUV_BUF_ENC_LINE].param);
		}
	}

	if (yuv_buf_hal_is_sensor_resolution_err_int_triggered(hal, int_status)) {
		YUV_BUF_LOGE("sensor's yuyv data resoltion is not right\r\n");
	}

	if (yuv_buf_hal_is_h264_err_int_triggered(hal, int_status)) {
		YUV_BUF_LOGE("h264 error\r\n");
	}

	if (yuv_buf_hal_is_enc_slow_int_triggered(hal, int_status)) {
		YUV_BUF_LOGE("jpeg code rate is slow than sensor's data rate\r\n");
	}
}

