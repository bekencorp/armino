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
#include <driver/int.h>
#include <driver/gpio.h>
#include <driver/sdio_host.h>
#include "clock_driver.h"
#include "gpio_driver.h"
#include "gpio_map.h"
#include "pmu.h"
#include "power_driver.h"
#include "sdio_host_driver.h"
#include "sdio_host_hal.h"
#include "sys_driver.h"
#include "icu_driver.h"
#include "drv_model_pub.h"
#include "bk_sys_ctrl.h"

typedef struct {
	sdio_host_hal_t hal;
	uint32_t int_status;
	bool is_tx_blocked;
	beken_semaphore_t tx_sema;
} sdio_host_driver_t;

#define SDIO_HOST_RETURN_ON_NOT_INIT() do {\
	if (!s_sdio_host_driver_is_init) {\
		SDIO_HOST_LOGE("sdio host driver not init\r\n");\
		return BK_ERR_SDIO_HOST_NOT_INIT;\
	}\
} while(0)

static sdio_host_driver_t s_sdio_host = {0};
static bool s_sdio_host_driver_is_init = false;

static void sdio_host_isr(void);

static void sdio_host_init_gpio(void)
{
	const sdio_host_gpio_map_t sdio_host_gpio_map_table[] = SDIO_HOST_GPIO_MAP;

	/* set gpio sdio host map: clk,cmd,data0 */
	for (uint32_t i = SDIO_HOST_GPIO_CLK_INDEX; i < SDIO_HOST_GPIO_PIN_NUMBER; i++) {
		gpio_dev_unmap(sdio_host_gpio_map_table[i].gpio_id);
	}

#if (!CONFIG_SYSTEM_CTRL)
	gpio_sdio_sel(GPIO_SDIO_MAP_MODE0);
	icu_sdio_host_enable();
#endif

	/* sdio host clk */
	bk_gpio_pull_up(sdio_host_gpio_map_table[SDIO_HOST_GPIO_CLK_INDEX].gpio_id);
	bk_gpio_set_capacity(sdio_host_gpio_map_table[SDIO_HOST_GPIO_CLK_INDEX].gpio_id, 3);
	/* sdio host cmd */
	bk_gpio_pull_up(sdio_host_gpio_map_table[SDIO_HOST_GPIO_CMD_INDEX].gpio_id);
	bk_gpio_set_capacity(sdio_host_gpio_map_table[SDIO_HOST_GPIO_CMD_INDEX].gpio_id, 3);
	/* sdio host data0 */
	bk_gpio_pull_up(sdio_host_gpio_map_table[SDIO_HOST_GPIO_DATA0_INDEX].gpio_id);
	bk_gpio_set_capacity(sdio_host_gpio_map_table[SDIO_HOST_GPIO_DATA0_INDEX].gpio_id, 3);

#if CONFIG_SDCARD_BUSWIDTH_4LINE
	/* sdio host data1,data2,data3 */
	for (uint32_t i = SDIO_HOST_GPIO_DATA1_INDEX; i < SDIO_HOST_GPIO_PIN_NUMBER; i++) {
		bk_gpio_pull_up(sdio_host_gpio_map_table[i].gpio_id);
		bk_gpio_set_capacity(sdio_host_gpio_map_table[i].gpio_id, 3);
	}
#endif
}

/* 1. power up sdio host
 * 2. set clock
 * 3. enable sdio host interrupt
 * 4. map gpio
 */
static void sdio_host_init_common(void)
{
#if (CONFIG_SYSTEM_CTRL)
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_SDIO, CLK_PWR_CTRL_PWR_UP);
	sys_hal_set_sdio_clk_sel(0); // set sdio source clock as XTAL 26M
	//sdio_host_interrupt_enable(id);
#else
	power_sdio_pwr_up();
	clk_set_sdio_clk_26m();
	icu_enable_sdio_interrupt();
#endif

// Temp code : if not set this, bk7271 sdio cmd line will send CMD73 forever
#if (CONFIG_SOC_BK7271)
	UINT32 param;

	param = BLK_BIT_MIC_QSPI_RAM_OR_FLASH;
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_BLK_ENABLE, &param);

	param = PSRAM_VDD_3_3V;
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_QSPI_VDDRAM_VOLTAGE, &param);
#endif

	/* config sdio host gpio */
	sdio_host_init_gpio();
}

static void sdio_host_deinit_common(void)
{
	sdio_host_hal_reset_config_to_default(&s_sdio_host.hal);

#if (CONFIG_SYSTEM_CTRL)
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_SDIO, CLK_PWR_CTRL_PWR_DOWN);
#else
	power_sdio_pwr_down();
	icu_disable_sdio_interrupt();
#endif
}

bk_err_t bk_sdio_host_driver_init(void)
{
	if (s_sdio_host_driver_is_init) {
		return BK_OK;
	}

	os_memset(&s_sdio_host, 0, sizeof(s_sdio_host));
	sdio_host_hal_init(&s_sdio_host.hal);
	bk_int_isr_register(INT_SRC_SDIO, sdio_host_isr, NULL);
	if (s_sdio_host.tx_sema == NULL) {
		int ret = rtos_init_semaphore(&(s_sdio_host.tx_sema), 1);
		BK_ASSERT(kNoErr == ret);
	}
	s_sdio_host.is_tx_blocked = true;
	s_sdio_host_driver_is_init = true;
	return BK_OK;
}

bk_err_t bk_sdio_host_driver_deinit(void)
{
	if (!s_sdio_host_driver_is_init) {
		return BK_OK;
	}
	sdio_host_deinit_common();
	bk_int_isr_unregister(INT_SRC_SDIO);
	if (s_sdio_host.tx_sema) {
		int ret = rtos_deinit_semaphore(&(s_sdio_host.tx_sema));
		BK_ASSERT(kNoErr == ret);
	}
	s_sdio_host_driver_is_init = false;
	return BK_OK;
}

bk_err_t bk_sdio_host_init(const sdio_host_config_t *config)
{
	BK_RETURN_ON_NULL(config);
	SDIO_HOST_RETURN_ON_NOT_INIT();

	sdio_host_init_common();

	/* reset sdio host register */
	sdio_host_hal_reset_config_to_default(&s_sdio_host.hal);

	/* set sdio host clock frequence */
	//sdio_host_hal_set_clk_freq(&s_sdio_host.hal, CONFIG_SDIO_HOST_DEFAULT_CLOCK_FREQ);
	sdio_host_hal_set_clk_freq(&s_sdio_host.hal, config->clock_freq);

	/* set sdio host bus width */
	sdio_host_hal_set_bus_width(&s_sdio_host.hal, config->bus_width);

	return BK_OK;
}

bk_err_t bk_sdio_host_deinit(void)
{
	SDIO_HOST_RETURN_ON_NOT_INIT();

	sdio_host_deinit_common();

	return BK_OK;
}

bk_err_t bk_sdio_host_set_clock_freq(sdio_host_clock_freq_t clock_freq)
{
	SDIO_HOST_RETURN_ON_NOT_INIT();

	sdio_host_hal_set_clk_freq(&s_sdio_host.hal, clock_freq);

	return BK_OK;
}

bk_err_t bk_sdio_host_send_command(const sdio_host_cmd_cfg_t *command)
{
	BK_RETURN_ON_NULL(command);
	SDIO_HOST_RETURN_ON_NOT_INIT();

	BK_RETURN_ON_ERR(sdio_host_hal_init_commad(&s_sdio_host.hal, command));
	sdio_host_hal_start_send_command(&s_sdio_host.hal);
	return BK_OK;
}

bk_err_t bk_sdio_host_wait_cmd_response(uint32_t cmd_index)
{
	SDIO_HOST_RETURN_ON_NOT_INIT();

	uint32_t int_status = sdio_host_hal_get_interrupt_status(&s_sdio_host.hal);

	/* wait until cmd response */
	BK_WHILE (!sdio_host_hal_is_cmd_rsp_interrupt_triggered(&s_sdio_host.hal, int_status)) {
		int_status = sdio_host_hal_get_interrupt_status(&s_sdio_host.hal);
	}

	/* clear command response int status */
	sdio_host_hal_clear_cmd_rsp_interrupt_status(&s_sdio_host.hal, int_status);

	if (sdio_host_hal_is_cmd_rsp_timeout_interrupt_triggered(&s_sdio_host.hal, int_status)) {
		if (cmd_index != SEND_OP_COND) {
			SDIO_HOST_LOGW("sdio wait slave CMD%d timeout\r\n", cmd_index);
			return BK_ERR_SDIO_HOST_CMD_RSP_TIMEOUT;
		}
	}

	if (sdio_host_hal_is_cmd_rsp_crc_fail_interrupt_triggered(&s_sdio_host.hal, int_status)) {
		if ((cmd_index != SD_APP_OP_COND) &&
			(cmd_index != ALL_SEND_CID) &&
			(cmd_index != SEND_CSD) &&
			(cmd_index != SEND_OP_COND)) {
			SDIO_HOST_LOGW("sdio receive slave CMD%d crc fail\r\n", cmd_index);
			return BK_ERR_SDIO_HOST_CMD_RSP_CRC_FAIL;
		}
	}

	return BK_OK;
}

uint32_t bk_sdio_host_get_cmd_rsp_argument(sdio_host_response_t argument_index)
{
	SDIO_HOST_RETURN_ON_NOT_INIT();

	return sdio_host_hal_get_cmd_rsp_argument(&s_sdio_host.hal, argument_index);
}

bk_err_t bk_sdio_host_config_data(const sdio_host_data_config_t *data_config)
{
	BK_RETURN_ON_NULL(data_config);
	SDIO_HOST_RETURN_ON_NOT_INIT();

	if (data_config->data_dir == SDIO_HOST_DATA_DIR_RD) {
		/* 1) set data timer
		 * 2) clear int status(REG9)
		 * 3) set rx fifo threshold
		 * 4) reset read fifo , reset sdcard command and data state
		 * 5) set data register
 		 */
 		sdio_host_hal_clear_interrupt_status(&s_sdio_host.hal);
		sdio_host_hal_set_rx_fifo_threshold(&s_sdio_host.hal, 0x1);
		sdio_host_hal_set_read_data(&s_sdio_host.hal, data_config->data_block_size);
		sdio_host_hal_start_receive_data(&s_sdio_host.hal);
	} else {
 		sdio_host_hal_clear_interrupt_status(&s_sdio_host.hal);
		sdio_host_hal_set_tx_fifo_threshold(&s_sdio_host.hal, 0x1);
		sdio_host_hal_set_write_data(&s_sdio_host.hal, data_config->data_block_size);
		//sdio_host_hal_start_send_data(&s_sdio_host.hal);
	}
	sdio_host_hal_set_data_timeout(&s_sdio_host.hal, data_config->data_timeout);
	return BK_OK;
}

bk_err_t bk_sdio_host_set_data_timeout(uint32_t timeout)
{
	SDIO_HOST_RETURN_ON_NOT_INIT();
	sdio_host_hal_set_data_timeout(&s_sdio_host.hal, timeout);
	return BK_OK;
}

bk_err_t bk_sdio_host_write_fifo(const uint8_t *write_data, uint32_t data_size)
{
	BK_RETURN_ON_NULL(write_data);
	SDIO_HOST_RETURN_ON_NOT_INIT();

	sdio_host_hal_t *hal = &s_sdio_host.hal;
	uint32_t index = 0;
	uint32 data_tmp = 0;
	uint32_t int_level = 0;
	bk_err_t error_state = BK_OK;

	while ((sdio_host_hal_is_tx_fifo_write_ready(hal)) && (index < data_size)) {
		data_tmp = ((write_data[index] << 24) | (write_data[index + 1] << 16) | (write_data[index + 2] << 8) | write_data[index + 3]);
		index += 4;
		sdio_host_hal_write_fifo(hal, data_tmp);
	}

	int_level = rtos_disable_int();
	sdio_host_hal_enable_tx_fifo_empty_mask(hal);
	sdio_host_hal_start_send_data(&s_sdio_host.hal);
	s_sdio_host.is_tx_blocked = true;
	rtos_enable_int(int_level);

	rtos_get_semaphore(&(s_sdio_host.tx_sema), BEKEN_NEVER_TIMEOUT);

	int_level = rtos_disable_int();
	sdio_host_hal_disable_tx_fifo_empty_mask(hal);
	if (sdio_host_hal_is_data_crc_fail_int_triggered(hal, s_sdio_host.int_status)) {
		error_state = BK_ERR_SDIO_HOST_DATA_CRC_FAIL;
	}
	if (sdio_host_hal_is_data_timeout_int_triggered(hal, s_sdio_host.int_status)) {
		error_state = BK_ERR_SDIO_HOST_DATA_TIMEOUT;
	}
	rtos_enable_int(int_level);

	return error_state;
}

bk_err_t bk_sdio_host_wait_receive_data(void)
{
	SDIO_HOST_RETURN_ON_NOT_INIT();

	bk_err_t error_state = BK_FAIL;
	uint32_t int_status = 0;
	sdio_host_hal_t *hal = &s_sdio_host.hal;

	/* wait for recv data interrupt triggered */
	do {
		int_status = sdio_host_hal_get_interrupt_status(hal);
		sdio_host_hal_clear_data_interrupt_status(hal, int_status);
		SDIO_HOST_LOGI("int_status:%x\r\n", int_status);
	} while (!sdio_host_hal_is_recv_data_interrupt_triggered(hal, int_status));

	if (sdio_host_hal_is_data_recv_end_int_triggered(hal, int_status)) {
		if (sdio_host_hal_is_data_crc_fail_int_triggered(hal, int_status)) {
			error_state = BK_ERR_SDIO_HOST_DATA_CRC_FAIL;
		} else {
			error_state = BK_OK;
		}
	} else {
		if (sdio_host_hal_is_data_timeout_int_triggered(hal, int_status)) {
			error_state = BK_ERR_SDIO_HOST_DATA_TIMEOUT;
		}
	}

	return error_state;
}

uint32_t bk_sdio_host_read_fifo(void)
{
	while (!sdio_host_hal_is_rx_fifo_read_ready(&s_sdio_host.hal));
	return sdio_host_hal_read_fifo(&s_sdio_host.hal);
}

static void sdio_host_isr(void)
{
	sdio_host_hal_t *hal = &s_sdio_host.hal;
	uint32_t int_status = sdio_host_hal_get_interrupt_status(hal);
	s_sdio_host.int_status = int_status;

	SDIO_HOST_LOGD("enter sdio_host isr, int_status:%x\r\n", int_status);
	sdio_host_hal_clear_data_interrupt_status(hal, int_status);

	if (sdio_host_hal_is_data_write_end_int_triggered(hal, int_status) ||
		sdio_host_hal_is_fifo_empty_int_triggered(hal, int_status) ||
		sdio_host_hal_is_data_crc_fail_int_triggered(hal, int_status) ||
		sdio_host_hal_is_data_timeout_int_triggered(hal, int_status)) {
		/* need clear tx fifo empty mask, otherwise sdio_isr will triggered forever */
		sdio_host_hal_disable_tx_fifo_empty_mask(hal);
		if (s_sdio_host.is_tx_blocked) {
			rtos_set_semaphore(&s_sdio_host.tx_sema);
			s_sdio_host.is_tx_blocked = false;
		}
	}
}

