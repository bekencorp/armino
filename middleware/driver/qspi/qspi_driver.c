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

#include <driver/int.h>
#include <os/mem.h>
#include <driver/qspi.h>
#include "bk_drv_model.h"
#include "bk_sys_ctrl.h"
#include "clock_driver.h"
#include "gpio_driver.h"
#include "power_driver.h"
#include "qspi_driver.h"
#include "qspi_hal.h"
#include "qspi_statis.h"

typedef struct {
	qspi_hal_t hal;
	uint8_t id_init_bits;
} qspi_driver_t;

typedef struct {
	qspi_isr_t callback;
	void *param;
} qspi_callback_t;

#define QSPI_RETURN_ON_NOT_INIT() do {\
		if (!s_qspi_driver_is_init) {\
			QSPI_LOGE("QSPI driver not init\r\n");\
			return BK_ERR_QSPI_NOT_INIT;\
		}\
	} while(0)

#define QSPI_RETURN_ON_ID_NOT_INIT() do {\
		if (!s_qspi.id_init_bits) {\
			QSPI_LOGE("QSPI not init\r\n");\
			return BK_ERR_QSPI_ID_NOT_INIT;\
		}\
	} while(0)

static qspi_driver_t s_qspi = {0};
static bool s_qspi_driver_is_init = false;
static qspi_callback_t s_qspi_tx_isr = {NULL};
static qspi_callback_t s_qspi_rx_isr = {NULL};

static void qspi_init_gpio(void)
{
	gpio_dev_map(QSPI_LL_RAM_CSN_PIN, GPIO_DEV_QSPI_RAM_CSN);
	gpio_dev_map(QSPI_LL_RAM_CLK_PIN, GPIO_DEV_QSPI_RAM_CLK);
	gpio_dev_map(QSPI_LL_IO0_PIN, GPIO_DEV_QSPI_IO0);
	gpio_dev_map(QSPI_LL_IO1_PIN, GPIO_DEV_QSPI_IO1);
	gpio_dev_map(QSPI_LL_IO2_PIN, GPIO_DEV_QSPI_IO2);
	gpio_dev_map(QSPI_LL_IO3_PIN, GPIO_DEV_QSPI_IO3);
}

/* 1. power up qspi
 * 2. set clock
 * 3. set gpio as qspi
 * 4. icu enable psram interrupt(fiq_int_enable)
 * 5. sys_ctrl set psram voltage
 * 6. sys_ctrl block_en
 */
static void qspi_id_init_common(void)
{
	power_qspi_pwr_up();
	clk_set_qspi_clk_26m();
	icu_enable_qspi_interrupt();
	qspi_init_gpio();
	qspi_hal_init_common();
}

static void qspi_id_deinit_common(void)
{
	icu_disable_qspi_interrupt();
	power_qspi_pwr_down();
	qspi_hal_deinit_common();
}

static void qspi_isr(void);

bk_err_t bk_qspi_driver_init(void)
{
	if (s_qspi_driver_is_init) {
		return BK_OK;
	}

	os_memset(&s_qspi, 0, sizeof(s_qspi));
	bk_int_isr_register(INT_SRC_PSRAM, qspi_isr, NULL);
	qspi_hal_init(&s_qspi.hal);
	qspi_statis_init();
	s_qspi_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_qspi_driver_deinit(void)
{
	if (!s_qspi_driver_is_init) {
		return BK_OK;
	}
	qspi_id_deinit_common();
	bk_int_isr_unregister(INT_SRC_PSRAM);
	s_qspi_driver_is_init = false;

	return BK_OK;
}

bk_err_t bk_qspi_init(const qspi_config_t *config)
{
	BK_RETURN_ON_NULL(config);
	QSPI_RETURN_ON_NOT_INIT();

	qspi_id_init_common();
	switch (config->src_clk) {
	case QSPI_SCLK_DCO:
		clk_set_qspi_clk_dco();
		break;
	case QSPI_SCLK_80M:
		clk_set_qspi_clk_80m();
		break;
	case QSPI_SCLK_120M:
		clk_set_qspi_clk_120m();
		break;
	case QSPI_SCLK_XTAL_26M:
	default:
		clk_set_qspi_clk_26m();
		break;
	}
	qspi_hal_set_clk_div(&s_qspi.hal, config->clk_div);
	s_qspi.id_init_bits |= BIT(0);
	return BK_OK;
}

bk_err_t bk_qspi_deinit(void)
{
	qspi_id_deinit_common();
	s_qspi.id_init_bits &= ~BIT(0);
	return BK_OK;
}

bk_err_t bk_qspi_command(const qspi_cmd_t *cmd)
{
	BK_RETURN_ON_NULL(cmd);
	QSPI_RETURN_ON_ID_NOT_INIT();
	qspi_hal_command(&s_qspi.hal, cmd);
	return BK_OK;
}

bk_err_t bk_qspi_write(uint32_t base_addr, const void *data, uint32_t size)
{
	//TODO bk7256 support
	BK_RETURN_ON_NULL(data);
	QSPI_RETURN_ON_ID_NOT_INIT();
	return BK_OK;
}

bk_err_t bk_qspi_read(uint32_t base_addr, void *data, uint32_t size)
{
	//TODO bk7256 support
	BK_RETURN_ON_NULL(data);
	QSPI_RETURN_ON_ID_NOT_INIT();
	return BK_OK;
}

bk_err_t bk_qspi_register_tx_isr(qspi_isr_t isr, void *param)
{
	uint32_t int_level = rtos_disable_int();
	s_qspi_tx_isr.callback = isr;
	s_qspi_tx_isr.param = param;
	rtos_enable_int(int_level);
	return BK_OK;
}

bk_err_t bk_qspi_register_rx_isr(qspi_isr_t isr, void *param)
{
	uint32_t int_level = rtos_disable_int();
	s_qspi_rx_isr.callback = isr;
	s_qspi_rx_isr.param = param;
	rtos_enable_int(int_level);
	return BK_OK;
}

static void qspi_isr(void)
{
	qspi_hal_t *hal = &s_qspi.hal;
	uint32_t int_status = 0;
	QSPI_STATIS_DEC();
	QSPI_STATIS_GET(qspi_statis);
	QSPI_STATIS_INC(qspi_statis->qspi_int_cnt);

	int_status = qspi_hal_get_interrupt_status_before_mask(hal);
	QSPI_LOGD("int_status before mask:%x\r\n", int_status);

	int_status = qspi_hal_get_interrupt_status_after_mask(hal);
	QSPI_LOGD("int_status after mask:%x\r\n", int_status);
	qspi_hal_clear_interrupt_status(hal, int_status);

	if (qspi_hal_is_sw_op_int_triggered(hal, int_status)) {
		QSPI_STATIS_INC(qspi_statis->sw_op_int_cnt);
		QSPI_LOGD("sw op int triggered\r\n");
		qspi_hal_clear_sw_op_int(hal);
		qspi_hal_stop_sw_op(hal);
		if (qspi_hal_is_cur_sw_op_write_data()) {
			qspi_hal_disable_ge0_tx(hal);
			if (s_qspi_tx_isr.callback) {
				s_qspi_tx_isr.callback(0, s_qspi_tx_isr.param);
			}
		}

		if (qspi_hal_is_cur_sw_op_read_data()) {
			qspi_hal_disable_ge1_rx(hal);
			if (s_qspi_rx_isr.callback) {
				s_qspi_rx_isr.callback(0, s_qspi_rx_isr.param);
			}
		}
	}
}

