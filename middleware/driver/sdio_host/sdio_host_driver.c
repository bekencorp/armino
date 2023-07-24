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
#if (!CONFIG_SYSTEM_CTRL)
#include "bk_sys_ctrl.h"
#endif

#include "bk_misc.h"

#if (CONFIG_SDIO_V2P0)
/*
 * TODO: until now(2022-08-20),we use specific value for timeout.
 *       The right way is read the SDCARD speed mode and SDIO clock frequency, and then set a suit value.
 *       But sometimes, the special SDCARD maybe at busy status(programming data to memory) too much time.
 */
#define SDIO_MAX_TX_WAIT_TIME (512)
#define SDIO_MAX_RX_WAIT_TIME (512)
#define SDIO_CMD_WAIT_TIME    (4)	//one cycle:try 128 counts
#define SDIO_BLOCK_SIZE (0x200)
#define APP_RSP_CMD               63
#endif
typedef struct {
	sdio_host_hal_t hal;
	uint32_t int_status;
	beken_queue_t irq_cmd_msg;	//hasn't event in freertos
	bool is_tx_blocked;
	beken_semaphore_t tx_sema;

	beken_semaphore_t rx_sema;
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

#if (!CONFIG_GPIO_DEFAULT_SET_SUPPORT)
static void sdio_host_init_gpio(void)
{
	const sdio_host_gpio_map_t sdio_host_gpio_map_table[] = SDIO_HOST_GPIO_MAP;

	/* set gpio sdio host map: clk,cmd,data0 */
	for (uint32_t i = SDIO_HOST_GPIO_CLK_INDEX; i < SDIO_HOST_GPIO_PIN_NUMBER; i++) {
		gpio_dev_unmap(sdio_host_gpio_map_table[i].gpio_id);
	}

#if (CONFIG_SYSTEM_CTRL)
#if CONFIG_SDIO_4LINES_EN
	gpio_sdio_sel(GPIO_SDIO_MAP_MODE0);
#else
	gpio_sdio_one_line_sel(GPIO_SDIO_MAP_MODE0);
#endif
#else
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

#if CONFIG_SDIO_4LINES_EN
	/* sdio host data1,data2,data3 */
	for (uint32_t i = SDIO_HOST_GPIO_DATA1_INDEX; i < SDIO_HOST_GPIO_PIN_NUMBER; i++) {
		bk_gpio_pull_up(sdio_host_gpio_map_table[i].gpio_id);
		bk_gpio_set_capacity(sdio_host_gpio_map_table[i].gpio_id, 3);
	}
#endif
}
#endif
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
	sys_drv_int_enable(SDIO_INTERRUPT_CTRL_BIT);
#if CONFIG_SDIO_V2P0
	sdio_host_hal_enable_all_mask(&s_sdio_host.hal);
#endif
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

#if CONFIG_GPIO_DEFAULT_SET_SUPPORT
	/*
	 * GPIO info is setted in GPIO_DEFAULT_DEV_CONFIG and inited in bk_gpio_driver_init->gpio_hal_default_map_init.
	 * If needs to re-config GPIO, can deal it here.
	 */
#else
	/* config sdio host gpio */
	sdio_host_init_gpio();
#endif
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
	bk_err_t ret = BK_OK;
	if (s_sdio_host_driver_is_init) {
		SDIO_HOST_LOGD("bk_sdio_host_driver_init has inited\r\n");
		return BK_OK;
	}

	os_memset(&s_sdio_host, 0, sizeof(s_sdio_host));
	sdio_host_hal_init(&s_sdio_host.hal);
	bk_int_isr_register(INT_SRC_SDIO, sdio_host_isr, NULL);

	if (s_sdio_host.irq_cmd_msg == NULL) {
		ret = rtos_init_queue(
								&s_sdio_host.irq_cmd_msg,
								"sdio_host_queue",
								4,
								16
							);
		BK_ASSERT(kNoErr == ret); /* ASSERT VERIFIED */
	}
	if (s_sdio_host.tx_sema == NULL) {
		int ret = rtos_init_semaphore(&(s_sdio_host.tx_sema), 1);
		BK_ASSERT(kNoErr == ret); /* ASSERT VERIFIED */
	}
	s_sdio_host.is_tx_blocked = false;

	if (s_sdio_host.rx_sema == NULL) {
		int ret = rtos_init_semaphore(&(s_sdio_host.rx_sema), 1);
		BK_ASSERT(kNoErr == ret); /* ASSERT VERIFIED */
	}
	s_sdio_host_driver_is_init = true;
	return BK_OK;
}

bk_err_t bk_sdio_host_driver_deinit(void)
{
	bk_err_t ret = BK_OK;
	if (!s_sdio_host_driver_is_init) {
		return BK_OK;
	}
	sdio_host_deinit_common();
	bk_int_isr_unregister(INT_SRC_SDIO);

	if (s_sdio_host.irq_cmd_msg) {
		ret = rtos_deinit_queue(&s_sdio_host.irq_cmd_msg);
		BK_ASSERT(kNoErr == ret); /* ASSERT VERIFIED */

		s_sdio_host.irq_cmd_msg = NULL;
	}

	if (s_sdio_host.tx_sema) {
		int ret = rtos_deinit_semaphore(&(s_sdio_host.tx_sema));
		BK_ASSERT(kNoErr == ret); /* ASSERT VERIFIED */

		s_sdio_host.tx_sema = NULL;
	}
	if (s_sdio_host.rx_sema) {
		int ret = rtos_deinit_semaphore(&(s_sdio_host.rx_sema));
		BK_ASSERT(kNoErr == ret); /* ASSERT VERIFIED */

		s_sdio_host.rx_sema = NULL;
	}

	s_sdio_host_driver_is_init = false;
	return BK_OK;
}

bk_err_t bk_sdio_host_init(const sdio_host_config_t *config)
{
	BK_RETURN_ON_NULL(config);
	SDIO_HOST_RETURN_ON_NOT_INIT();

#if (CONFIG_SDIO_V2P0)
	/* reset sdio host register */
	sdio_host_hal_reset_config_to_default(&s_sdio_host.hal);

	sdio_host_init_common();
#else
	sdio_host_init_common();

	/* reset sdio host register */
	sdio_host_hal_reset_config_to_default(&s_sdio_host.hal);
#endif
#if (CONFIG_SDIO_V2P0)
	bk_sdio_host_set_clock_freq(config->clock_freq);
#else
	/* set sdio host clock frequence */
	//sdio_host_hal_set_clk_freq(&s_sdio_host.hal, CONFIG_SDIO_HOST_DEFAULT_CLOCK_FREQ);
	sdio_host_hal_set_clk_freq(&s_sdio_host.hal, config->clock_freq);
#endif
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

#if CONFIG_SDIO_V2P0
/*
 * WARNING:
 * Reset the sdio states:TX FIFO, RX FIFO, SDIO state.
 * TX FIFO/RX FIFO effects the clock gate function.
 * Just be called in SDK,APP don't use it.
 * If error called this API maybe cause error clock output to sdcard,
 * because sdcard read/write data and command response bases on clock from BK7256.
 */
void bk_sdio_host_reset_sd_state(void)
{
	sdio_host_hal_reset_sd_state(&s_sdio_host.hal);
}

/*
 * WARNING: CLOCK(Enable/Disbale/Gate) API are only called in SDK,not for APP.
 * BK7256 Clock scheme:
 * 1.bk_sdio_clock_en: sdio asic module clock enable
 * 2.bk_sdio_clk_gate_config:
 *   a. enable means the sdio clock always on, not gated;
 *   b. disable means the clock will be controlled by sdio fifo status(read/write).
 * 3.bk_sdio_tx_fifo_clk_gate_config
 *   a. at WRITE status(bk7256 write data to sdio fifo),if the fifo is empty,the clock will be gated
 *      clock doesn't output to sdcard
 * 4.If at READ status, the clock gate will be controlled by sdio asic, SW can't control it.
 *   a. FIFO full or read data finish,the clock will be gated, else clock output to sdcard
 */
void bk_sdio_clock_en(uint32_t enable)
{
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_SDIO, enable);
}

/*
 * WARNING: CLOCK(Enable/Disbale/Gate) API are only called in SDK,not for APP.
 * BK7256 Clock scheme:
 * 1.bk_sdio_clock_en: sdio asic module clock enable
 * 2.bk_sdio_clk_gate_config:
 *   a. enable means the sdio clock always on, not gated;
 *   b. disable means the clock will be controlled by sdio fifo status(read/write).
 * 3.bk_sdio_tx_fifo_clk_gate_config
 *   a. at WRITE status(bk7256 write data to sdio fifo),if the fifo is empty,the clock will be gated
 *      clock doesn't output to sdcard
 * 4.If at READ status, the clock gate will be controlled by sdio asic, SW can't control it.
 *   a. FIFO full or read data finish,the clock will be gated, else clock output to sdcard
 */
void bk_sdio_clk_gate_config(uint32_t enable)
{
	sdio_host_hal_set_clock_gate(&s_sdio_host.hal, enable);
}

/*
 * WARNING: CLOCK(Enable/Disbale/Gate) API are only called in SDK,not for APP.
 * BK7256 Clock scheme:
 * 1.bk_sdio_clock_en: sdio asic module clock enable
 * 2.bk_sdio_clk_gate_config:
 *   a. enable means the sdio clock always on, not gated;
 *   b. disable means the clock will be controlled by sdio fifo status(read/write).
 * 3.bk_sdio_tx_fifo_clk_gate_config
 *   a. at WRITE status(bk7256 write data to sdio fifo),if the fifo is empty,the clock will be gated
 *      clock doesn't output to sdcard
 * 4.If at READ status, the clock gate will be controlled by sdio asic, SW can't control it.
 *   a. FIFO full or read data finish,the clock will be gated, else clock output to sdcard
 */
void bk_sdio_tx_fifo_clk_gate_config(uint32_t enable)
{
	sdio_hal_host_set_tx_fifo_need_write_mask_cg(&s_sdio_host.hal, enable);
}

bk_err_t bk_sdio_host_set_clock_freq(sdio_host_clock_freq_t clock_freq)
{
	SDIO_HOST_RETURN_ON_NOT_INIT();

	sdio_host_hal_set_clk_freq(&s_sdio_host.hal, clock_freq);

	return BK_OK;
}

#else
bk_err_t bk_sdio_host_set_clock_freq(sdio_host_clock_freq_t clock_freq)
{
	SDIO_HOST_RETURN_ON_NOT_INIT();

	sdio_host_hal_set_clk_freq(&s_sdio_host.hal, clock_freq);

	return BK_OK;
}
#endif

static void sdio_dump_cmd_info(const sdio_host_cmd_cfg_t *command)
{
#if 0
	SDIO_HOST_LOGD("cmd_index=%d\r\n", command->cmd_index);
	SDIO_HOST_LOGD("argument=0x%x\r\n", command->argument);
	SDIO_HOST_LOGD("response=0x%x\r\n", command->response);
	SDIO_HOST_LOGD("wait_rsp_timeout=%d\r\n", command->wait_rsp_timeout);
	SDIO_HOST_LOGD("crc_check=%d\r\n", command->crc_check);
#endif
}

bk_err_t bk_sdio_host_send_command(const sdio_host_cmd_cfg_t *command)
{
	BK_RETURN_ON_NULL(command);
	SDIO_HOST_RETURN_ON_NOT_INIT();

	sdio_dump_cmd_info(command);

	//default:need crc check, if no need crc check, please set it in command.
	//sdio_host_hal_set_cmd_crc_check(&s_sdio_host.hal, true);
	BK_RETURN_ON_ERR(sdio_host_hal_init_commad(&s_sdio_host.hal, command));
	sdio_host_hal_start_send_command(&s_sdio_host.hal);
	return BK_OK;
}

bk_err_t bk_sdio_host_wait_cmd_response(uint32_t cmd_index)
{
#if CONFIG_SDIO_V2P0
	bk_err_t ret = BK_OK;
	uint32_t msg = 0;
#endif

	SDIO_HOST_RETURN_ON_NOT_INIT();

	uint32_t int_status = sdio_host_hal_get_interrupt_status(&s_sdio_host.hal);

#if CONFIG_SDIO_V2P0
	{
		//TODO:Timeout and CRC fail should use different case
		//FreeRTOS doesn't use EVENT,so use msg queue.
		ret = rtos_pop_from_queue(&s_sdio_host.irq_cmd_msg, &msg, SDIO_CMD_WAIT_TIME);
		if(ret)
		{
			SDIO_HOST_LOGD("sdio wait slave CMD%d timeout, int_status=0x%x, ret=%d\r\n", cmd_index, int_status, ret);
			return BK_ERR_SDIO_HOST_CMD_RSP_TIMEOUT;
		}
	}
#else
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
#endif
	return BK_OK;
}

uint32_t bk_sdio_host_get_cmd_rsp_argument(sdio_host_response_t argument_index)
{
	SDIO_HOST_RETURN_ON_NOT_INIT();

	return sdio_host_hal_get_cmd_rsp_argument(&s_sdio_host.hal, argument_index);
}

#if (CONFIG_SDIO_V2P0)
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
		//TODO:Interrupt status should only be cleared in ISR
		//sdio_host_hal_clear_interrupt_status(&s_sdio_host.hal);
		sdio_host_hal_set_rx_fifo_threshold(&s_sdio_host.hal, 0x1);

		sdio_host_hal_set_read_multi_block_data(&s_sdio_host.hal, data_config->data_block_size);
		sdio_host_hal_enable_rx_end_mask(&s_sdio_host.hal);
		//TODO:disable it or set a big value of RX threshhold?
		//sdio_host_hal_enable_rx_need_read_mask(&s_sdio_host.hal);
		sdio_host_hal_start_receive_data(&s_sdio_host.hal);

		sdio_host_hal_set_data_timeout(&s_sdio_host.hal, data_config->data_timeout);
	} else {
		//TODO:Interrupt status should only be cleared in ISR
 		//sdio_host_hal_clear_interrupt_status(&s_sdio_host.hal);
		sdio_host_hal_set_tx_fifo_threshold(&s_sdio_host.hal, 0x1);
		sdio_host_hal_set_write_multi_block_data(&s_sdio_host.hal, data_config->data_block_size);
	}
	return BK_OK;
}
#else
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

#endif
bk_err_t bk_sdio_host_set_data_timeout(uint32_t timeout)
{
	SDIO_HOST_RETURN_ON_NOT_INIT();
	sdio_host_hal_set_data_timeout(&s_sdio_host.hal, timeout);
	return BK_OK;
}

#if CONFIG_SDIO_V2P0
bk_err_t bk_sdio_host_write_fifo(const uint8_t *write_data, uint32_t data_size)
{
	BK_RETURN_ON_NULL(write_data);
	SDIO_HOST_RETURN_ON_NOT_INIT();

	sdio_host_hal_t *hal = &s_sdio_host.hal;
	uint32_t index = 0;
	uint32 data_tmp = 0;
	bk_err_t error_state = BK_OK;
	uint32_t i = 0;

	while ((index < data_size)) {
		//confirm can write data to fifo(fifo isn't at busy/full status)
		while((sdio_host_hal_is_tx_fifo_write_ready(hal)) == 0)
		{
			i++;
			if(i % 0x1000000 == 0)
				SDIO_HOST_LOGE("FIFO can't write i=0x%08x", i);

			//avoid dead in while
			if(i == 0x1000000 * 16) {
				SDIO_HOST_LOGE("FIFO write fail,the write data is invalid");
				break;
			}
		}

		//switch byte sequence, as the SDIO transfer data with big-endian
		data_tmp = ((write_data[index] << 24) | (write_data[index + 1] << 16) | (write_data[index + 2] << 8) | write_data[index + 3]);
		sdio_host_hal_write_fifo(hal, data_tmp);
		index += 4;

#if (CONFIG_SDIO_DEBUG_SUPPORT)
		if(index < 16)
		{
			SDIO_HOST_LOGD("data_tmp=0x%08x", data_tmp);
		}
		if(index == 16)
		{
			SDIO_HOST_LOGD("0x%08x\r\n", data_tmp);
		}
#endif
		//first block finish, enable tx fifo clock gate and then start write data to sdio wires(sdcard)
		if((index == SDIO_BLOCK_SIZE))
		{
			bk_sdio_tx_fifo_clk_gate_config(1);
			sdio_host_hal_start_send_data(&s_sdio_host.hal);
		}

		//one block write to sdcard fifo finish
		if((index % SDIO_BLOCK_SIZE == 0) || (index == data_size))	//maybe isn't a full block(though sdcard always use full block)
		{
			/*
			 * wait write end int which means the data has sent to sdcard and get the sdcard's response.
			 * empty mask just mean host FIFO is empty but can't indicate that sdcard has dealed data finish.
			 * write status fail will not set semaphore yet, so it cause here timeout, here set it as tx fail.
			 */
			s_sdio_host.is_tx_blocked = true;
			error_state = rtos_get_semaphore(&(s_sdio_host.tx_sema), SDIO_MAX_TX_WAIT_TIME);
			if(error_state)
			{
				s_sdio_host.is_tx_blocked = false;
				SDIO_HOST_LOGE("TODO:sdio tx data fail:index=%d!\r\n", index);
			}
		}
	}

	return error_state;
}

/*
 * Internal API only for SD-CARD Driver.
 * SDIO has pre-read feature, it cause SD-CARD driver gets one more sema if the read block
 * isn't continious addr with previous block address.
 * So add one special API for SD-CARD to discard the previous rx data sema.
 */
void bk_sdio_host_discard_previous_receive_data_sema(void)
{
	rtos_get_semaphore(&(s_sdio_host.rx_sema), 0);
}

bk_err_t bk_sdio_host_wait_receive_data(void)
{
	SDIO_HOST_RETURN_ON_NOT_INIT();
	bk_err_t error_state = BK_FAIL;

	//CRC err will not set semaphore yet, so it cause here timeout, here set it as rx fail.
	error_state = rtos_get_semaphore(&(s_sdio_host.rx_sema), SDIO_MAX_RX_WAIT_TIME);
	if(error_state != BK_OK)
	{
		SDIO_HOST_LOGI("rx fail\r\n");
	}

	return error_state;
}
#else
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
#endif

void bk_sdio_host_start_read(void)
{
	sdio_host_hal_start_receive_data(&s_sdio_host.hal);
}

bk_err_t bk_sdio_host_read_fifo(uint32_t *save_v_p)
{
#define SDIO_READ_FIFO_TIMEOUT (400)	//us, one word transfer in 13M clock nearly 10us with single wire
	uint32_t i = 0;

	while (!sdio_host_hal_is_rx_fifo_read_ready(&s_sdio_host.hal))
	{
		i++;
		delay_us(1);
		if(i == SDIO_READ_FIFO_TIMEOUT) {
			SDIO_HOST_LOGE("FIFO read fail,the return data is invalid\r\n");
			return BK_ERR_SDIO_HOST_READ_DATA_FAIL;
		}
	}

	*save_v_p = sdio_host_hal_read_fifo(&s_sdio_host.hal);

	return BK_OK;
}

#if (CONFIG_SDIO_V2P0)
static void sdio_host_isr(void)
{
	sdio_host_hal_t *hal = &s_sdio_host.hal;
	uint32_t int_status = sdio_host_hal_get_interrupt_status(&s_sdio_host.hal);
	uint32_t cmd_index = sdio_host_hal_get_cmd_index_interrupt_status(&s_sdio_host.hal, int_status);
	s_sdio_host.int_status = int_status;

	//sdio_host_hal_clear_data_interrupt_status(hal, int_status);

	//TODO:WARNING:sdio_host_hal_is_data_crc_fail_int_triggered should check
	SDIO_HOST_LOGD("sdio isr, cmd_index=%d,int_status:%x\r\n", cmd_index, int_status);

	//CMD:RESP, NO RESP, TIMEOUT
	if(sdio_host_hal_is_cmd_rsp_interrupt_triggered(hal, int_status))
	{
		//CMD END(Some CMD has no response, so has not CRC)
		if(sdio_host_hal_is_cmd_end_interrupt_triggered(hal, int_status))
		{
			uint32_t cmd_rsp_ok = true;
			if(sdio_host_hal_is_cmd_rsp_crc_ok_interrupt_triggered(hal, int_status))	//CRC OK
			{

			}
			else if(sdio_host_hal_is_cmd_rsp_crc_fail_interrupt_triggered(hal, int_status))	//CRC Fail
			{
				//SDIO Host driver no need to care about SDCARD CMD Index,SDCARD driver needs to set
				//the speific CMD Index whether needs CRC check.
				/* if ((cmd_index != SD_APP_OP_COND) &&
					(cmd_index != ALL_SEND_CID) &&
					(cmd_index != SEND_CSD) &&
					(cmd_index != SEND_OP_COND) &&
					(cmd_index != APP_RSP_CMD)) */ {
					SDIO_HOST_LOGW("sdio receive CMD%d crc fail\r\n", cmd_index);
					cmd_rsp_ok = false;
					//return BK_ERR_SDIO_HOST_CMD_RSP_CRC_FAIL;
				}
			}
			else	//some CMD has no response,so has no CRC check.
			{

			}

			if (/*s_sdio_host.is_cmd_blocked &&*/ cmd_rsp_ok) {
				if(rtos_push_to_queue(&s_sdio_host.irq_cmd_msg, &int_status, 0))
				{
					SDIO_HOST_LOGE("sdio push cmd msg fail\r\n");
				}
				//s_sdio_host.is_cmd_blocked = false;
			}
		}
		else if(sdio_host_hal_is_cmd_rsp_timeout_interrupt_triggered(hal, int_status))	//timeout
		{
			if (cmd_index != SEND_OP_COND) {
				SDIO_HOST_LOGD("isr sdio wait CMD RSP timeout, int_status=0x%x\r\n", int_status);
			}
		}

		//clear cmd int status
		sdio_host_hal_clear_cmd_rsp_interrupt_status(hal, int_status);
	}
	//TX(host write) DATA
	else if(sdio_host_hal_is_data_write_end_int_triggered(hal, int_status) /* ||
		sdio_host_hal_is_fifo_empty_int_triggered(hal, int_status) ||
		sdio_host_hal_is_tx_fifo_need_write_int_triggered(hal, int_status) */)
	{
		/*
		if(sdio_host_hal_is_fifo_empty_int_triggered(hal, int_status))
		{
			sdio_host_hal_disable_tx_fifo_empty_mask(hal);
		}
		*/

		/*
		if(sdio_host_hal_is_tx_fifo_need_write_int_triggered(hal, int_status))
		{
			sdio_host_hal_disable_tx_fifo_need_write_mask(hal);
		}
		*/

		//TODO:check write status
		if (sdio_host_hal_get_wr_status(&s_sdio_host.hal) != 2) {
			SDIO_HOST_LOGE("TODO:write data error!!!\r\n");
		}

		{
			SDIO_HOST_LOGD("write blk end\r\n");
			rtos_set_semaphore(&s_sdio_host.tx_sema);
		}
#if (CONFIG_SDIO_HOST_CLR_WRITE_INT)
		bk_sdio_tx_fifo_clk_gate_config(1);
		sdio_host_hal_clear_write_data_interrupt_status(hal, int_status);
		sdio_host_hal_clear_write_data_interrupt_status(hal, int_status);
		sdio_host_hal_clear_write_data_interrupt_status(hal, int_status);
		bk_sdio_tx_fifo_clk_gate_config(0);//clr 3 times to wait sd_card a cycle;
#else
		sdio_host_hal_clear_write_data_interrupt_status(hal, int_status);
#endif
	}
	//RX(host read) DATA
	else if(sdio_host_hal_is_data_recv_end_int_triggered(hal, int_status) ||
		/* sdio_host_hal_is_data_recv_need_read_int_triggered(hal, int_status) ||
		sdio_host_hal_is_data_recv_overflow_int_triggered(hal, int_status) || */
		sdio_host_hal_is_data_timeout_int_triggered(hal, int_status))
	{
		if(sdio_host_hal_is_data_crc_ok_int_triggered(hal, int_status)) //CRC OK
		{
			rtos_set_semaphore(&s_sdio_host.rx_sema);
			sdio_host_hal_clear_read_data_interrupt_status(hal, int_status);
		}
		else if(sdio_host_hal_is_data_crc_fail_int_triggered(hal, int_status))	//CRC Fail
		{
			//multi-block read:the sdio has pre-read feature(after read finish, the sdio still reads one block data),
			//so it always read more data then SW requires,
			//but the data is invalid,so reset the sd state if CRC fail.

			bk_sdio_host_reset_sd_state();

			//TODO:If the data is really CRC fail, should notify APP the data received is error.
			SDIO_HOST_LOGE("TODO:read data crc error!!!\r\n");
#if 0	//just not set sema cause rx data timeout, which cause rx fail.
			rtos_set_semaphore(&s_sdio_host.rx_sema);
#endif
			sdio_host_hal_clear_read_data_interrupt_status(hal, int_status);
		}
		else if(sdio_host_hal_is_data_timeout_int_triggered(hal, int_status))	//timeout
		{
			SDIO_HOST_LOGE("TODO:read data timeout error!!!\r\n");
			sdio_host_hal_clear_read_data_timeout_interrupt_status(hal, int_status);
		}
		/*
		 * !!!NOTES!!! SDIO pre-read feature time-sequence issue!!!
		 * If read data finish,SW will clear FIFO(bk_sdio_host_reset_sd_state) when stop read,
		 * but maybe the ASIC have pre-read one block data finished but SDIO ISR doesn't response.
		 * After FIFO reset, ISR response found the CRC isn't OK and isn't ERROR,
		 * which cause ISR status isn't cleared, so the SDIO ISR will entry and exit forever
		 * cause system abnormall(watchdog timeout reset)
		 */
		else
		{
			SDIO_HOST_LOGE("read data status err:int_status=0x%08x!!!\r\n", int_status);
			sdio_host_hal_clear_read_data_interrupt_status(hal, int_status);
		}
	}
	else {
		SDIO_HOST_LOGD("sdio isr no deal:cmd_index=%d,int_status=%x\r\n", cmd_index, int_status);
	}
}

#else
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
#endif
