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
#include <common/bk_compiler.h>
#include <os/mem.h>
#include "uart_driver.h"
#include "uart_hal.h"
#include <driver/uart.h>
#include "bk_uart.h"
#include "gpio_driver.h"
#include <driver/gpio.h>
#include "bk_fifo.h"
#include <os/os.h>
#include "uart_statis.h"
#include <driver/int.h>
#include "icu_driver.h"
#include "power_driver.h"
#include "clock_driver.h"
#include <os/os.h>
#include "bk_arch.h"
#include <components/system.h>
#if (CONFIG_SYSTEM_CTRL)
#include "sys_driver.h"
#include <modules/pm.h>
#endif
#if (CONFIG_UART_RX_DMA)
#include <driver/dma.h>
#endif

static void uart_isr_common(uart_id_t id) __BK_SECTION(".itcm");
static uint32_t uart_id_read_fifo_frame(uart_id_t id, const kfifo_ptr_t rx_ptr) __BK_SECTION(".itcm");

typedef struct {
	uart_hal_t hal;
	uint8_t id_init_bits;
	uint8_t id_sw_fifo_enable_bits;
#if CONFIG_UART_RX_DMA
	bool rx_dma_enable;
	dma_id_t rx_dma_id;
#endif
} uart_driver_t;

typedef struct {
	bool rx_blocked;
	beken_semaphore_t rx_int_sema;
} uart_sema_t;

typedef struct
{
	uart_isr_t callback;
	void *param;
} uart_callback_t;

#define CONFIG_UART_MIN_BAUD_RATE (UART_CLOCK / (0x1fff + 1))
#define CONFIG_UART_MAX_BAUD_RATE (UART_CLOCK / (4 + 1))
#ifndef CONFIG_PRINTF_BUF_SIZE
#define CONFIG_PRINTF_BUF_SIZE    (128)
#endif

static uart_driver_t s_uart[SOC_UART_ID_NUM_PER_UNIT] = {0};
static bool s_uart_driver_is_init = false;
static uart_callback_t s_uart_rx_isr[SOC_UART_ID_NUM_PER_UNIT] = {NULL};
static uart_callback_t s_uart_tx_isr[SOC_UART_ID_NUM_PER_UNIT] = {NULL};
static kfifo_ptr_t s_uart_rx_kfifo[SOC_UART_ID_NUM_PER_UNIT] = {NULL};
static uart_sema_t s_uart_sema[SOC_UART_ID_NUM_PER_UNIT] = {0};

#define UART_RETURN_ON_NOT_INIT() do {\
		if (!s_uart_driver_is_init) {\
			return BK_ERR_UART_NOT_INIT;\
		}\
	} while(0)

#define UART_RETURN_ON_INVALID_ID(id) do {\
		if ((id) >= SOC_UART_ID_NUM_PER_UNIT) {\
			return BK_ERR_UART_INVALID_ID;\
		}\
	} while(0)

#define UART_RETURN_ON_ID_NOT_INIT(id) do {\
		if (!(s_uart[id].id_init_bits & BIT((id)))) {\
			return BK_ERR_UART_ID_NOT_INIT;\
		}\
	} while(0)

#define UART_RETURN_ON_BAUD_RATE_NOT_SUPPORT(baud_rate) do {\
		if ((baud_rate) < CONFIG_UART_MIN_BAUD_RATE ||\
			(baud_rate) > CONFIG_UART_MAX_BAUD_RATE) {\
			return BK_ERR_UART_BAUD_RATE_NOT_SUPPORT;\
		}\
	} while(0)

#if (CONFIG_DEBUG_FIRMWARE)
#define DEAD_WHILE() do{\
		while(1);\
	} while(0)
#else
#define DEAD_WHILE() do{\
		os_printf("dead\r\n");\
	} while(0)
#endif

#if (CONFIG_SYSTEM_CTRL)
void uart_clock_enable(uart_id_t id)
{
	switch(id)
	{
		case UART_ID_0:
			sys_drv_dev_clk_pwr_up(CLK_PWR_ID_UART1, CLK_PWR_CTRL_PWR_UP);
			break;
		case UART_ID_1:
			sys_drv_dev_clk_pwr_up(CLK_PWR_ID_UART2, CLK_PWR_CTRL_PWR_UP);
			break;
		case UART_ID_2:
			sys_drv_dev_clk_pwr_up(CLK_PWR_ID_UART3, CLK_PWR_CTRL_PWR_UP);
			break;
		default:
			break;
	}
}

void uart_clock_disable(uart_id_t id)
{
	switch(id)
	{
		case UART_ID_0:
			sys_drv_dev_clk_pwr_up(CLK_PWR_ID_UART1, CLK_PWR_CTRL_PWR_DOWN);
			break;
		case UART_ID_1:
			sys_drv_dev_clk_pwr_up(CLK_PWR_ID_UART2, CLK_PWR_CTRL_PWR_DOWN);
			break;
		case UART_ID_2:
			sys_drv_dev_clk_pwr_up(CLK_PWR_ID_UART3, CLK_PWR_CTRL_PWR_DOWN);
			break;
		default:
			break;
	}
}

static void uart_interrupt_enable(uart_id_t id)
{
	switch(id)
	{
		case UART_ID_0:
			sys_drv_int_enable(UART0_INTERRUPT_CTRL_BIT);
			break;
		case UART_ID_1:
			sys_drv_int_enable(UART1_INTERRUPT_CTRL_BIT);
			break;
		case UART_ID_2:
			sys_drv_int_enable(UART2_INTERRUPT_CTRL_BIT);
			break;
		default:
			break;
	}
}

static void uart_interrupt_disable(uart_id_t id)
{
	switch(id)
	{
		case UART_ID_0:
			sys_drv_int_disable(UART0_INTERRUPT_CTRL_BIT);
			break;
		case UART_ID_1:
			sys_drv_int_disable(UART1_INTERRUPT_CTRL_BIT);
			break;
		case UART_ID_2:
			sys_drv_int_disable(UART2_INTERRUPT_CTRL_BIT);
			break;
		default:
			break;
	}
}
#endif

static void uart_init_gpio(uart_id_t id)
{
	switch (id)
	{
		case UART_ID_0:
		{
			gpio_dev_unmap(uart_hal_get_tx_pin(id));
			gpio_dev_unmap(uart_hal_get_rx_pin(id));
			gpio_dev_map(uart_hal_get_tx_pin(id), GPIO_DEV_UART1_TXD);
			gpio_dev_map(uart_hal_get_rx_pin(id), GPIO_DEV_UART1_RXD);
			bk_gpio_pull_up(uart_hal_get_tx_pin(id));
			bk_gpio_pull_up(uart_hal_get_rx_pin(id));
#if CONFIG_UART1_FLOW_CTRL
			//NOTICE:BEKEN ASIC CTS PIN really function is RTS.
			gpio_dev_map(uart_hal_get_cts_pin(id), GPIO_DEV_UART1_CTS);
			bk_gpio_enable_output(uart_hal_get_cts_pin(id));
			bk_gpio_pull_down(uart_hal_get_cts_pin(id));

			gpio_dev_map(uart_hal_get_rts_pin(id), GPIO_DEV_UART1_RTS);
			bk_gpio_enable_input(uart_hal_get_rts_pin(id));
			bk_gpio_pull_down(uart_hal_get_rts_pin(id));
			bk_uart_set_hw_flow_ctrl(id, UART1_FLOW_CTRL_CNT);
#endif
			break;
		}
		case UART_ID_1:
		{
			gpio_dev_unmap(uart_hal_get_tx_pin(id));
			gpio_dev_unmap(uart_hal_get_rx_pin(id));
			gpio_dev_map(uart_hal_get_tx_pin(id), GPIO_DEV_UART2_TXD);
			gpio_dev_map(uart_hal_get_rx_pin(id), GPIO_DEV_UART2_RXD);
			bk_gpio_pull_up(uart_hal_get_tx_pin(id));
			bk_gpio_pull_up(uart_hal_get_rx_pin(id));
			break;
		}
		case UART_ID_2:
		{
			gpio_dev_unmap(uart_hal_get_tx_pin(id));
			gpio_dev_unmap(uart_hal_get_rx_pin(id));
			gpio_dev_map(uart_hal_get_tx_pin(id), GPIO_DEV_UART3_TXD);
			gpio_dev_map(uart_hal_get_rx_pin(id), GPIO_DEV_UART3_RXD);
			bk_gpio_pull_up(uart_hal_get_tx_pin(id));
			bk_gpio_pull_up(uart_hal_get_rx_pin(id));
			break;
		}
		default:
			break;
	}
}

static void uart_deinit_tx_gpio(uart_id_t id)
{
	switch (id)
	{
		case UART_ID_0:
		{
			gpio_dev_unmap(uart_hal_get_tx_pin(id));
			bk_gpio_pull_up(uart_hal_get_tx_pin(id));
			break;
		}
		case UART_ID_1:
		{
			gpio_dev_unmap(uart_hal_get_tx_pin(id));
			bk_gpio_pull_up(uart_hal_get_tx_pin(id));
			break;
		}
		case UART_ID_2:
		{
			gpio_dev_unmap(uart_hal_get_tx_pin(id));
			bk_gpio_pull_up(uart_hal_get_tx_pin(id));
			break;
		}
		default:
			break;
	}
}

static void uart_deinit_rx_gpio(uart_id_t id)
{
	switch (id)
	{
		case UART_ID_0:
		{
			gpio_dev_unmap(uart_hal_get_rx_pin(id));
			bk_gpio_pull_up(uart_hal_get_rx_pin(id));
			break;
		}
		case UART_ID_1:
		{
			gpio_dev_unmap(uart_hal_get_rx_pin(id));
			bk_gpio_pull_up(uart_hal_get_rx_pin(id));
			break;
		}
		case UART_ID_2:
		{
			gpio_dev_unmap(uart_hal_get_rx_pin(id));
			bk_gpio_pull_up(uart_hal_get_rx_pin(id));
			break;
		}
		default:
			break;
	}
}

static bk_err_t uart_id_init_kfifo(uart_id_t id)
{
	uint32_t fifo_size = CONFIG_KFIFO_SIZE;
//RX DMA needs bigger FIFO size when erase flash.
#if (CONFIG_UART_RX_DMA)
	if(s_uart[id].rx_dma_enable)
		fifo_size = CONFIG_UART_RX_DMA_KFIFO_SIZE;
#endif

	if (!s_uart_rx_kfifo[id]) {
		s_uart_rx_kfifo[id] = kfifo_alloc(fifo_size);
		if (!s_uart_rx_kfifo[id]) {
			UART_LOGE("uart(%d) rx kfifo alloc failed\n", id);
			return BK_ERR_NULL_PARAM;
		}
	}
	return BK_OK;
}

static void uart_id_deinit_kfifo(uart_id_t id)
{
	if (s_uart_rx_kfifo[id]) {
		kfifo_free(s_uart_rx_kfifo[id]);
	}
	s_uart_rx_kfifo[id] = NULL;
}

/* 1. power up uart
 * 2. set clock
 * 3. set gpio as uart
 */
static bk_err_t uart_id_init_common(uart_id_t id)
{
	bk_err_t ret = 0;

#if (CONFIG_SYSTEM_CTRL)
	uart_clock_enable(id);
	sys_drv_uart_select_clock(id, UART_SCLK_XTAL_26M);
#else
	power_uart_pwr_up(id);
	clk_set_uart_clk_26m(id);
#endif

	uart_init_gpio(id);
	ret = uart_id_init_kfifo(id);
	uart_statis_id_init(id);

	s_uart_sema[id].rx_blocked = false;
	if (s_uart_sema[id].rx_int_sema == NULL) {
		ret = rtos_init_semaphore(&(s_uart_sema[id].rx_int_sema), 1);
		BK_ASSERT(kNoErr == ret); /* ASSERT VERIFIED */
	}
	s_uart[id].id_init_bits |= BIT(id);
	s_uart[id].id_sw_fifo_enable_bits |= BIT(id);

#if CONFIG_UART_RX_DMA
	//default not enable rx dma
	s_uart[id].rx_dma_enable = 0;
#endif
	return ret;
}

static void uart_id_deinit_common(uart_id_t id)
{
	s_uart[id].id_init_bits &= ~BIT(id);
#if CONFIG_UART_RX_DMA
	s_uart[id].rx_dma_enable = 0;
#endif
	uart_hal_stop_common(&s_uart[id].hal, id);
	uart_hal_reset_config_to_default(&s_uart[id].hal, id);
#if (CONFIG_SYSTEM_CTRL)
	uart_interrupt_disable(id);
	uart_clock_disable(id);
#else
	icu_disable_uart_interrupt(id);
	power_uart_pwr_down(id);
#endif
	uart_id_deinit_kfifo(id);
	if(s_uart_sema[id].rx_int_sema)
	{
		rtos_deinit_semaphore(&(s_uart_sema[id].rx_int_sema));
		s_uart_sema[id].rx_int_sema = NULL;
	}
}

static inline bool uart_id_is_sw_fifo_enabled(uart_id_t id)
{
	return !!(s_uart[id].id_sw_fifo_enable_bits & BIT(id));
}

#if CONFIG_UART_RX_DMA
static uint32_t uart_id_dma_read_fifo_frame(uart_id_t id, const kfifo_ptr_t rx_ptr)
{
	//DMA stop
	bk_dma_stop(s_uart[id].rx_dma_id);

	//update WRITE-Pointer by DMA write length
	uint16_t actual_trans_len = (s_uart_rx_kfifo[id]->size - bk_dma_get_remain_len(s_uart[id].rx_dma_id));
	//buffer over-wrap:
	//i.e:when erase flash,CPU can't get instruction from flash then can't handle this function.
	//after Flash erase complete, UART handler come but DMA has copy more then s_uart_rx_kfifo[id]->size bytes data
	if(actual_trans_len > s_uart_rx_kfifo[id]->size)
	{
		//TODO:
		BK_ASSERT(0);
	}
	else
	{
		__attribute__((__unused__)) uart_statis_t *uart_statis = uart_statis_get_statis(id);

		//data has been saved in buffer by DMA, only update write pointer
		rx_ptr->in += actual_trans_len;

		UART_STATIS_SET(uart_statis->kfifo_status.in, rx_ptr->in);
	}

	bk_dma_start(s_uart[id].rx_dma_id);

	return actual_trans_len;
}
#endif

static uint32_t uart_id_read_fifo_frame(uart_id_t id, const kfifo_ptr_t rx_ptr)
{
	uint8_t read_val = 0;
	uint32_t rx_count = sizeof(read_val);
	uint32_t unused = kfifo_unused(rx_ptr);
	uint32_t kfifo_put_cnt = 0;
	__attribute__((__unused__)) uart_statis_t *uart_statis = uart_statis_get_statis(id);

	//TODO: optimize flow ctrl
	while (uart_hal_is_fifo_read_ready(&s_uart[id].hal, id)) {
		/* must read when fifo read ready, otherwise will loop forever */
		read_val = uart_hal_read_byte(&s_uart[id].hal, id);
		UART_LOGD("read val:0x%x, rx_count/unused: %d/%d\n", read_val, rx_count, unused);
		if (rx_count > unused) {
			if (!uart_hal_is_flow_control_enabled(&s_uart[id].hal, id)) {
				UART_LOGW("rx kfifo is full, out/in:%d/%d, unused:%d\n", rx_ptr->out, rx_ptr->in, unused);
				UART_STATIS_INC(uart_statis->kfifo_status.full_cnt);
#if CFG_CLI_DEBUG
				extern void cli_show_running_command(void);
				cli_show_running_command();
#endif
			}
		} else {
			kfifo_put_cnt += kfifo_put(rx_ptr, &read_val, sizeof(read_val));
			UART_STATIS_INC(uart_statis->kfifo_status.put_cnt);
			UART_STATIS_SET(uart_statis->kfifo_status.last_value, read_val);
		}
		unused = kfifo_unused(rx_ptr);
	}
	UART_STATIS_SET(uart_statis->kfifo_status.in, rx_ptr->in);
	UART_STATIS_SET(uart_statis->kfifo_status.out, rx_ptr->out);

	return kfifo_put_cnt;
}

void print_hex_dump(const char *prefix, const void *buf, int len)
{
	int i;
	const u8 *b = buf;

	if (prefix)
		BK_LOG_RAW("%s", prefix);
	for (i = 0; i < len; i++)
		BK_LOG_RAW("%02X ", b[i]);
	BK_LOG_RAW("\n");
}

bk_err_t uart_write_byte(uart_id_t id, uint8_t data)
{
	/* wait for fifo write ready
	 * optimize it when write very fast
	 * wait for fifo write ready will waste CPU performance
	 */
	BK_WHILE (!uart_hal_is_fifo_write_ready(&s_uart[id].hal, id));
	uart_hal_write_byte(&s_uart[id].hal, id, data);
	return BK_OK;
}

void uart_write_byte_for_ate(uart_id_t id, uint8_t *data, uint8_t cnt)
{
    int i;
    for(i = 0; i < cnt; i ++)
    {
        BK_WHILE (!uart_hal_is_fifo_write_ready(&s_uart[id].hal, id));
        uart_hal_write_byte(&s_uart[id].hal, id, data[i]);
    }
}

bk_err_t uart_write_ready(uart_id_t id)
{
	/* wait for fifo write ready
	 * optimize it when write very fast
	 * wait for fifo write ready will waste CPU performance
	 */
	if (!uart_hal_is_fifo_write_ready(&s_uart[id].hal, id)) {
		return BK_FAIL;
	}

	return BK_OK;
}

bk_err_t uart_write_string(uart_id_t id, const char *string)
{
	const char *p = string;

	while (*string) {
		if (*string == '\n') {
			if (p == string || *(string - 1) != '\r')
				uart_write_byte(id, '\r'); /* append '\r' */
		}
		uart_write_byte(id, *string++);
	}

	return BK_OK;
}

bk_err_t uart_read_ready(uart_id_t id)
{
	if (!uart_hal_is_fifo_read_ready(&s_uart[id].hal, id)) {
		return BK_FAIL;
	}

	return BK_OK;
}

int uart_read_byte(uart_id_t id)
{
	int val = -1;
	if (uart_hal_is_fifo_read_ready(&s_uart[id].hal, id)) {
		val = uart_hal_read_byte(&s_uart[id].hal, id);
	}
	return val;
}

int uart_read_byte_ex(uart_id_t id, uint8_t *ch)
{
	int val = -1;
	if (uart_hal_is_fifo_read_ready(&s_uart[id].hal, id)) {
		*ch = uart_hal_read_byte(&s_uart[id].hal, id);
		val = 0;
	}
	return val;
}

uint32_t uart_get_length_in_buffer(uart_id_t id)
{
	return kfifo_data_size(s_uart_rx_kfifo[id]);
}

static void uart_isr_register_functions(uart_id_t id)
{
	switch(id)
	{
		case UART_ID_0:
			bk_int_isr_register(INT_SRC_UART0, uart0_isr, NULL);
			break;
		case UART_ID_1:
			bk_int_isr_register(INT_SRC_UART1, uart1_isr, NULL);
			break;
		case UART_ID_2:
			bk_int_isr_register(INT_SRC_UART2, uart2_isr, NULL);
			break;
		default:
			break;
	}
}

#if 0 // CONFIG_UART_LOW_VOLTAGE_SUPPORT
typedef struct
{
	bool tx_status;
	bool rx_status;
	bool sys_int_en_status;
	uint32_t int_en_status;
	uart_id_t uart_id;
} uart_lowpower_sleep_status;
static uart_lowpower_sleep_status s_uart_lvsleep_status[SOC_UART_ID_NUM_PER_UNIT] = {0};

static void bk_uart_before_lvsleep_record_status(uart_id_t id)
{
	if(uart_hal_get_tx_enable(&s_uart[id].hal))
		s_uart_lvsleep_status[id].tx_status = true;
	else
		s_uart_lvsleep_status[id].tx_status = false;

	if(uart_hal_get_rx_enable(&s_uart[id].hal))
		s_uart_lvsleep_status[id].rx_status = true;
	else
		s_uart_lvsleep_status[id].rx_status = false;

	if(uart_hal_get_system_interrput_en_status(id))
		s_uart_lvsleep_status[id].sys_int_en_status = true;
	else
		s_uart_lvsleep_status[id].sys_int_en_status = false;

	s_uart_lvsleep_status[id].int_en_status = uart_hal_get_int_enable_status(&s_uart[id].hal, id);

}

static void bk_uart_lvsleep_restore_status(uart_id_t id)
{
	if(s_uart_lvsleep_status[id].tx_status) {
		uart_hal_set_tx_enable(&s_uart[id].hal, id, 1);
	}

	if(s_uart_lvsleep_status[id].rx_status) {
		uart_hal_set_rx_enable(&s_uart[id].hal, id, 1);
	}
}

static void bk_uart_exit_lvsleep_enable_config(uart_id_t id)
{
	if(s_uart_lvsleep_status[id].sys_int_en_status) {
		sys_drv_int_enable(id);
	}

	if(s_uart_lvsleep_status[id].int_en_status) {
		uart_hal_set_int_enable_status(&s_uart[id].hal, id, s_uart_lvsleep_status[id].int_en_status);
	}
}

static void bk_uart_enter_lvsleep(uint64_t sleep_time, void *args)
{
	uint32_t *uart_id = args;

	switch((uart_id_t)*uart_id)
	{
		case UART_ID_0:
			s_uart[UART_ID_0].hal.id = UART_ID_0;
			uart_hal_init(&s_uart[UART_ID_0].hal);
			bk_uart_before_lvsleep_record_status(UART_ID_0);
			uart_hal_stop_common(&s_uart[UART_ID_0].hal, UART_ID_0);
			break;
		case UART_ID_1:
			s_uart[UART_ID_1].hal.id = UART_ID_1;
			uart_hal_init(&s_uart[UART_ID_1].hal);
			bk_uart_before_lvsleep_record_status(UART_ID_1);
			uart_hal_stop_common(&s_uart[UART_ID_1].hal, UART_ID_1);
			break;
		case UART_ID_2:
			s_uart[UART_ID_2].hal.id = UART_ID_2;
			uart_hal_init(&s_uart[UART_ID_2].hal);
			bk_uart_before_lvsleep_record_status(UART_ID_2);
			uart_hal_stop_common(&s_uart[UART_ID_2].hal, UART_ID_2);
			break;
		default:
			break;
	}

	return;
}

static void bk_uart_exit_lvsleep(uint64_t sleep_time, void *args)
{
	uint32_t *uart_id = args;

	switch((uart_id_t)*uart_id)
	{
		case UART_ID_0:
			bk_uart_lvsleep_restore_status(UART_ID_0);
			bk_uart_exit_lvsleep_enable_config(UART_ID_0);
			break;
		case UART_ID_1:
			bk_uart_lvsleep_restore_status(UART_ID_1);
			bk_uart_exit_lvsleep_enable_config(UART_ID_1);
			break;
		case UART_ID_2:
			bk_uart_lvsleep_restore_status(UART_ID_2);
			bk_uart_exit_lvsleep_enable_config(UART_ID_2);
			break;
		default:
			break;
	}

	return;
}

static void uart_register_lvsleep_cb(void *param, uart_id_t id)
{
	pm_cb_conf_t uart_enter_config = {
			.cb = (pm_cb)bk_uart_enter_lvsleep,
			.args = param
			};
	pm_cb_conf_t uart_exit_config = {
			.cb = (pm_cb)bk_uart_exit_lvsleep,
			.args = param
			};

	switch(id)
	{
		case UART_ID_0:
			bk_pm_sleep_register_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_UART1, &uart_enter_config, &uart_exit_config);
			break;
		case UART_ID_1:
			bk_pm_sleep_register_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_UART2, &uart_enter_config, &uart_exit_config);
			break;
		case UART_ID_2:
			bk_pm_sleep_register_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_UART3, &uart_enter_config, &uart_exit_config);
			break;
		default:
			break;
	}
}

static void uart_unregister_lvsleep_cb(uart_id_t id)
{
	switch(id)
	{
		case UART_ID_0:
			bk_pm_sleep_unregister_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_UART1, true, true);
			break;
		case UART_ID_1:
			bk_pm_sleep_unregister_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_UART2, true, true);
			break;
		case UART_ID_2:
			bk_pm_sleep_unregister_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_UART3, true, true);
			break;
		default:
			break;
	}

}

static void uart_lvsleep_init(void)
{
	for(uart_id_t id = UART_ID_0; id < SOC_UART_ID_NUM_PER_UNIT; id++) {
		s_uart_lvsleep_status[id].tx_status = false;
		s_uart_lvsleep_status[id].rx_status = false;
		s_uart_lvsleep_status[id].sys_int_en_status = false;
		s_uart_lvsleep_status[id].uart_id = id;
		uart_register_lvsleep_cb(&(s_uart_lvsleep_status[id].uart_id), id);
	}
}

static void uart_lvsleep_deinit(void)
{
	for(uart_id_t id = UART_ID_0; id < SOC_UART_ID_NUM_PER_UNIT; id++) {
		s_uart_lvsleep_status[id].tx_status = false;
		s_uart_lvsleep_status[id].rx_status = false;
		s_uart_lvsleep_status[id].sys_int_en_status = false;
		s_uart_lvsleep_status[id].uart_id = UART_ID_MAX;
		uart_unregister_lvsleep_cb(id);
	}
}

#endif

bk_err_t bk_uart_driver_init(void)
{
	if (s_uart_driver_is_init) {
		return BK_OK;
	}

	os_memset(&s_uart, 0, sizeof(s_uart));
	os_memset(&s_uart_rx_isr, 0, sizeof(s_uart_rx_isr));
	os_memset(&s_uart_tx_isr, 0, sizeof(s_uart_tx_isr));
	for(uart_id_t id = UART_ID_0; id < SOC_UART_ID_NUM_PER_UNIT; id++)
	{
#if (0 == CONFIG_SOC_BK7236XX)
		uart_isr_register_functions(id);
		s_uart[id].hal.id = id;
		uart_hal_init(&s_uart[id].hal);
#endif
	}

	uart_statis_init();
	s_uart_driver_is_init = true;

#if CONFIG_UART_LOW_VOLTAGE_SUPPORT
//	uart_lvsleep_init();
#endif

#ifndef CONFIG_BK_PRINTF_DISABLE
	bk_printf_init();
#endif


	return BK_OK;
}

bk_err_t bk_uart_driver_deinit(void)
{
	if (!s_uart_driver_is_init)
		return BK_OK;

	for (uart_id_t id = UART_ID_0; id < SOC_UART_ID_NUM_PER_UNIT; id++) {
		uart_id_deinit_common(id);
	}

#if CONFIG_UART_LOW_VOLTAGE_SUPPORT
//	uart_lvsleep_deinit();
#endif
	s_uart_driver_is_init = false;

	return BK_OK;
}

int bk_uart_is_in_used(uart_id_t id)
{
	return (s_uart[id].id_init_bits & BIT((id)));
}

#if (CONFIG_UART_RX_DMA)
static void uart_rx_dma_fifo_full(dma_id_t dma_id)
{
	bk_printf("WARN:%s:dma_id=%d\r\n", __func__, dma_id);
}

static inline dma_dev_t uart_id_to_dma_dev(uart_id_t id, bool rx)
{
	uint32_t rx_id_offset = 0;
	if(rx)
		rx_id_offset = 1;
	switch(id)
	{
		case UART_ID_0:
			return DMA_DEV_UART1 + rx_id_offset;

		case UART_ID_1:
			return DMA_DEV_UART2 + rx_id_offset;

		case UART_ID_2:
			return DMA_DEV_UART3 + rx_id_offset;

		default:
			return DMA_DEV_UART1 + rx_id_offset;
	}
}

static inline void uart_rx_dma_src_port_config(uart_id_t id, dma_port_config_t *cfg_ptr)
{
	cfg_ptr->width = DMA_DATA_WIDTH_8BITS;
	cfg_ptr->addr_inc_en = DMA_ADDR_INC_DISABLE,
	cfg_ptr->addr_loop_en = DMA_ADDR_LOOP_DISABLE,
	cfg_ptr->dev = uart_id_to_dma_dev(id, 1);
	bk_printf("src dev=%d\r\n", cfg_ptr->dev);
	cfg_ptr->end_addr = cfg_ptr->start_addr = (uint32_t)uart_hal_get_read_data_addr(&s_uart[id].hal, id) & 0xffffffffc;
}

static bk_err_t uart_rx_dma_init(uart_id_t id)
{
	bk_err_t ret = BK_OK;
	dma_config_t dma_cfg;

	//DMA DST config:Memory
	dma_port_config_t dma_mem_port_config = {
						.dev = DMA_DEV_DTCM,
						.width = DMA_DATA_WIDTH_32BITS,
						.addr_inc_en = DMA_ADDR_INC_ENABLE,
						.addr_loop_en = DMA_ADDR_LOOP_ENABLE,
						.start_addr = 0,
						.end_addr = 0,
					};

	dma_cfg.dst = dma_mem_port_config;
	//force enable sw fifo
	bk_uart_enable_sw_fifo(id);
	if(s_uart_rx_kfifo[id] == NULL)
	{
		ret = uart_id_init_kfifo(id);
		if(ret != BK_OK)
			return ret;
	}
	dma_cfg.dst.start_addr = (uint32_t)s_uart_rx_kfifo[id]->buffer;
	if(dma_cfg.dst.start_addr == 0)
	{
		//TODO:
		BK_ASSERT(0);
	}
	dma_cfg.dst.end_addr = dma_cfg.dst.start_addr + s_uart_rx_kfifo[id]->size;

	//DMA SRC config:UART RX read port
	uart_rx_dma_src_port_config(id, &dma_cfg.src);

	dma_cfg.mode = DMA_WORK_MODE_SINGLE;
	dma_cfg.chan_prio = 0;	//UART speed is slow, so no need high priority

	//init DMA config
	dma_id_t dma_id = bk_dma_alloc(uart_id_to_dma_dev(id, 1));
	if(dma_id < DMA_ID_MAX)
	{
		BK_LOG_ON_ERR(bk_dma_init(dma_id, &dma_cfg));
		BK_LOG_ON_ERR(bk_dma_set_transfer_len(dma_id, s_uart_rx_kfifo[id]->size));
#if (CONFIG_SPE)
		BK_LOG_ON_ERR(bk_dma_set_dest_sec_attr(dma_id, DMA_ATTR_SEC));
		BK_LOG_ON_ERR(bk_dma_set_src_sec_attr(dma_id, DMA_ATTR_SEC));
#endif
		bk_dma_register_isr(dma_id, NULL, uart_rx_dma_fifo_full);
		BK_LOG_ON_ERR(bk_dma_start(dma_id));
	}
	else
	{
		bk_printf("Err:uart rx dma alloc fail\r\n");
		return BK_FAIL;
	}

	s_uart[id].rx_dma_id = dma_id;
	s_uart[id].rx_dma_enable = 1;

	return BK_OK;
}

static bk_err_t uart_rx_dma_deinit(uart_id_t id)
{
	dma_id_t dma_id = s_uart[id].rx_dma_id;
	s_uart[id].rx_dma_id = 0;
	s_uart[id].rx_dma_enable = 0;
	return bk_dma_free(uart_id_to_dma_dev(id, 1), dma_id);
}
#endif

bk_err_t bk_uart_init(uart_id_t id, const uart_config_t *config)
{
	UART_RETURN_ON_NOT_INIT();
	BK_RETURN_ON_NULL(config);
	UART_RETURN_ON_INVALID_ID(id);
	UART_RETURN_ON_BAUD_RATE_NOT_SUPPORT(config->baud_rate);

#if CONFIG_SOC_BK7236XX
	uart_isr_register_functions(id);
	s_uart[id].hal.id = id;
	uart_hal_init(&s_uart[id].hal);
#endif

	uart_id_init_common(id);
#if (CONFIG_SYSTEM_CTRL)
	if (config->src_clk == UART_SCLK_APLL)
		sys_drv_uart_select_clock(id, UART_SCLK_APLL);
	else
		sys_drv_uart_select_clock(id, UART_SCLK_XTAL_26M);

#else
	if (config->src_clk == UART_SCLK_DCO) {
		clk_set_uart_clk_dco(id);
	} else {
		clk_set_uart_clk_26m(id);
	}
#endif

#if CONFIG_UART_RX_DMA
	if(config->rx_dma_en)
	{
		//DMA init RX
		uart_rx_dma_init(id);
	}
#endif
	uart_hal_init_uart(&s_uart[id].hal, id, config);
	uart_hal_start_common(&s_uart[id].hal, id);

	return BK_OK;
}

bk_err_t bk_uart_deinit(uart_id_t id)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);
#if CONFIG_UART_RX_DMA
	uart_rx_dma_deinit(id);
#endif
	uart_id_deinit_common(id);
	return BK_OK;
}

bk_err_t bk_uart_set_baud_rate(uart_id_t id, uint32_t baud_rate)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);
	UART_RETURN_ON_BAUD_RATE_NOT_SUPPORT(baud_rate);
	uint32_t uart_clk = clk_get_uart_clk(id);
	uart_hal_set_baud_rate(&s_uart[id].hal, id, uart_clk, baud_rate);
	return BK_OK;
}

bk_err_t bk_uart_set_data_bits(uart_id_t id, uart_data_bits_t data_bits)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);
	uart_hal_set_data_bits(&s_uart[id].hal, id, data_bits);
	return BK_OK;
}

bk_err_t bk_uart_set_stop_bits(uart_id_t id, uart_stop_bits_t stop_bits)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);
	uart_hal_set_stop_bits(&s_uart[id].hal, id, stop_bits);
	return BK_OK;
}

bk_err_t bk_uart_set_parity(uart_id_t id, uart_parity_t partiy)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);
	uart_hal_set_parity(&s_uart[id].hal, id, partiy);
	return BK_OK;
}

bk_err_t bk_uart_enable_tx_interrupt(uart_id_t id)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);
#if (CONFIG_SYSTEM_CTRL)
	uart_interrupt_enable(id);
#else
	icu_enable_uart_interrupt(id);
#endif
	uart_hal_enable_tx_interrupt(&s_uart[id].hal, id);
	return BK_OK;
}

bk_err_t bk_uart_disable_tx_interrupt(uart_id_t id)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);
	uart_hal_disable_tx_interrupt(&s_uart[id].hal, id);
	uart_hal_clear_id_tx_interrupt_status(&s_uart[id].hal, id);
	return BK_OK;
}

bk_err_t bk_uart_enable_rx_interrupt(uart_id_t id)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);
#if (CONFIG_SYSTEM_CTRL)
	uart_interrupt_enable(id);
#else
	icu_enable_uart_interrupt(id);
#endif
	uart_hal_enable_rx_interrupt(&s_uart[id].hal, id);
	return BK_OK;
}

bk_err_t bk_uart_disable_rx_interrupt(uart_id_t id)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);
	uart_hal_disable_rx_interrupt(&s_uart[id].hal, id);
	uart_hal_clear_id_rx_interrupt_status(&s_uart[id].hal, id);
	return BK_OK;
}

bk_err_t bk_uart_register_rx_isr(uart_id_t id, uart_isr_t isr, void *param)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);

	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	s_uart_rx_isr[id].callback = isr;
	s_uart_rx_isr[id].param = param;
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

bk_err_t bk_uart_register_tx_isr(uart_id_t id, uart_isr_t isr, void *param)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);

	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	s_uart_tx_isr[id].callback = isr;
	s_uart_tx_isr[id].param = param;
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

static uart_callback_t s_last_uart_rx_isr = {0};

bk_err_t bk_uart_take_rx_isr(uart_id_t id, uart_isr_t isr, void *param)
{
	s_last_uart_rx_isr.callback = s_uart_rx_isr[id].callback;
	s_last_uart_rx_isr.param = s_uart_rx_isr[id].param;

	BK_RETURN_ON_ERR(bk_uart_disable_sw_fifo(id));
	BK_RETURN_ON_ERR(bk_uart_register_rx_isr(id, isr, param));

	return BK_OK;
}

bk_err_t bk_uart_recover_rx_isr(uart_id_t id)
{
	bk_uart_register_rx_isr(id, s_last_uart_rx_isr.callback, s_last_uart_rx_isr.param);

#if (!CONFIG_SHELL_ASYNCLOG)
	bk_uart_enable_sw_fifo(id);
#else
	if (id != bk_get_printf_port()) {
		bk_uart_enable_sw_fifo(id);
	}
#endif

	s_last_uart_rx_isr.callback = NULL;
	s_last_uart_rx_isr.param = NULL;

	return BK_OK;
}

bk_err_t bk_uart_write_bytes(uart_id_t id, const void *data, uint32_t size)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);
	UART_RETURN_ON_ID_NOT_INIT(id);
	for (int i = 0; i < size; i++) {
		uart_write_byte(id, ((uint8 *)data)[i]);
	}
	return BK_OK;
}

bk_err_t bk_uart_read_bytes(uart_id_t id, void *data, uint32_t size, uint32_t timeout_ms)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);
	UART_RETURN_ON_ID_NOT_INIT(id);

	if (uart_id_is_sw_fifo_enabled(id)) {
		__attribute__((__unused__)) uart_statis_t* uart_statis = uart_statis_get_statis(id);
		GLOBAL_INT_DECLARATION();
		GLOBAL_INT_DISABLE();
		uint32_t kfifo_data_len = kfifo_data_size(s_uart_rx_kfifo[id]);
		/* Only kfifo_data_len=0, wait for semaphore */
		if (kfifo_data_len == 0) {
			UART_LOGD("kfifo is empty, wait for recv data\r\n");
			/* when sema_cnt=0, rx_blocked=true, otherwise rx_blocked=false */
			s_uart_sema[id].rx_blocked = true;
			GLOBAL_INT_RESTORE();
#if CONFIG_RTC_TIMER_PRECISION_TEST
			uint32_t ret = rtos_get_semaphore(&(s_uart_sema[id].rx_int_sema), BEKEN_WAIT_FOREVER);
#else
			uint32_t ret = rtos_get_semaphore(&(s_uart_sema[id].rx_int_sema), timeout_ms);
#endif
			if (ret == kTimeoutErr) {
				GLOBAL_INT_DISABLE();
				if (!s_uart_sema[id].rx_blocked) {
					rtos_get_semaphore(&(s_uart_sema[id].rx_int_sema), 0);
				}
				s_uart_sema[id].rx_blocked = false;
				GLOBAL_INT_RESTORE();
				UART_LOGW("recv data timeout:%d\n", timeout_ms);
				UART_STATIS_INC(uart_statis->recv_timeout_cnt);
				return BK_ERR_UART_RX_TIMEOUT;
			}
		} else {
			GLOBAL_INT_RESTORE();
		}

		kfifo_data_len = kfifo_data_size(s_uart_rx_kfifo[id]); /* updata kfifo data size */
		if (size > kfifo_data_len) {
			if (kfifo_data_len) {
				kfifo_get(s_uart_rx_kfifo[id], (uint8_t *)data, kfifo_data_len);
			} else {
				UART_LOGW("kfifo data is empty\n");
				UART_STATIS_INC(uart_statis->kfifo_status.empty_cnt);
			}
			UART_STATIS_SET(uart_statis->kfifo_status.in, s_uart_rx_kfifo[id]->in);
			UART_STATIS_SET(uart_statis->kfifo_status.out, s_uart_rx_kfifo[id]->out);

			return kfifo_data_len;
		}
		kfifo_get(s_uart_rx_kfifo[id], (uint8_t *)data, size);
		UART_STATIS_SET(uart_statis->kfifo_status.in, s_uart_rx_kfifo[id]->in);
		UART_STATIS_SET(uart_statis->kfifo_status.out, s_uart_rx_kfifo[id]->out);

		return size;
	}else {
		int ret = 0;
		uint8_t rx_data;
		int read_count = 0;
		uint8_t *read_buffer = (uint8_t *)data;
	    int actual_bytes_to_read = size;

		/* read all data from rx-FIFO. */
	 	while (actual_bytes_to_read) {
			ret = uart_read_byte_ex(id, &rx_data);
			if (ret == -1)
				break;

			read_buffer[read_count] = rx_data;
			read_count++;

			actual_bytes_to_read--;
		}

		return read_count;
	}
}

bk_err_t bk_uart_set_rx_full_threshold(uart_id_t id, uint8_t threshold)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);
	uart_hal_set_rx_fifo_threshold(&s_uart[id].hal, id, threshold);
	return BK_OK;
}

bk_err_t bk_uart_set_tx_empty_threshold(uart_id_t id, uint8_t threshold)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);
	uart_hal_set_tx_fifo_threshold(&s_uart[id].hal, id, threshold);
	return BK_OK;
}

bk_err_t bk_uart_set_rx_timeout(uart_id_t id, uart_rx_stop_detect_time_t timeout_thresh)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);
	uart_hal_set_rx_stop_detect_time(&s_uart[id].hal, id, timeout_thresh);
	return BK_OK;
}

bk_err_t bk_uart_disable_rx(uart_id_t id)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);
	uart_hal_disable_rx(&s_uart[id].hal, id);
	uart_deinit_rx_gpio(id);

	return BK_OK;
}

bk_err_t bk_uart_disable_tx(uart_id_t id)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);
	uart_hal_disable_tx(&s_uart[id].hal, id);
	uart_deinit_tx_gpio(id);

	return BK_OK;
}

bk_err_t bk_uart_set_enable_rx(uart_id_t id, bool enable)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);
	uart_hal_set_rx_enable(&s_uart[id].hal, id, enable);

	return BK_OK;
}

bk_err_t bk_uart_set_enable_tx(uart_id_t id, bool enable)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);
	uart_hal_set_tx_enable(&s_uart[id].hal, id, enable);

	return BK_OK;
}

bk_err_t bk_uart_set_hw_flow_ctrl(uart_id_t id, uint8_t rx_threshold)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);
	uart_hal_set_hw_flow_ctrl(&s_uart[id].hal, id, rx_threshold);
	uart_hal_enable_flow_control(&s_uart[id].hal, id);
	return BK_OK;
}

bk_err_t bk_uart_disable_hw_flow_ctrl(uart_id_t id)
{
	UART_RETURN_ON_NOT_INIT();
	UART_RETURN_ON_INVALID_ID(id);
	uart_hal_disable_hw_flow_ctrl(&s_uart[id].hal, id);
	return BK_OK;
}

bk_err_t bk_uart_enable_sw_fifo(uart_id_t id)
{
	UART_RETURN_ON_INVALID_ID(id);
	s_uart[id].id_sw_fifo_enable_bits |= BIT(id);
	return BK_OK;
}

bk_err_t bk_uart_disable_sw_fifo(uart_id_t id)
{
	UART_RETURN_ON_INVALID_ID(id);
	s_uart[id].id_sw_fifo_enable_bits &= ~BIT(id);
	return BK_OK;
}

uint32_t bk_uart_get_ate_detect_gpio(void)
{
	return uart_hal_get_tx_pin(CONFIG_UART_ATE_PORT);
}

gpio_id_t bk_uart_get_rx_gpio(uart_id_t id)
{
	return uart_hal_get_rx_pin(id);
}

bool bk_uart_is_tx_over(uart_id_t id)
{
	return uart_hal_is_tx_fifo_empty(&s_uart[id].hal, id);
}

uint32_t uart_wait_tx_over(void)
{
	return uart_hal_wait_tx_over();
}

uint32_t uart_get_int_enable_status(uart_id_t id)
{
	return uart_hal_get_int_enable_status(&s_uart[id].hal, id);
}

uint32_t uart_get_interrupt_status(uart_id_t id)
{
	return uart_hal_get_interrupt_status(&s_uart[id].hal, id);
}

void uart_clear_interrupt_status(uart_id_t id, uint32_t int_status)
{
	uart_hal_clear_interrupt_status(&s_uart[id].hal, id, int_status);
}

/* read int enable status
 * read int status
 * clear int status
 */
static void uart_isr_common(uart_id_t id)
{
	uint32_t int_status = 0;
	uint32_t int_enable_status = 0;
	uint32_t status = 0;
	UART_STATIS_DEC();

	int_status = uart_hal_get_interrupt_status(&s_uart[id].hal, id);
	int_enable_status = uart_hal_get_int_enable_status(&s_uart[id].hal, id);
	status = int_status & int_enable_status;
	uart_hal_clear_interrupt_status(&s_uart[id].hal, id, int_status);
	UART_STATIS_GET(uart_statis, id);
	UART_STATIS_INC(uart_statis->uart_isr_cnt);

	if (uart_hal_is_rx_interrupt_triggered(&s_uart[id].hal, id, status))	//rx end or rx fifo full
	{
		//overflow,rx_para_err,rx_stop_err
		if(int_status & (BIT(2) | BIT(3) | BIT(4)))
		{
#if (CONFIG_UART_RX_DMA)
			//TODO:Discard the RX FIFO data,set WR_PTR to RD_PTR
			if(s_uart[id].rx_dma_enable)
			{
				uint32_t discard_len = s_uart_rx_kfifo[id]->out - s_uart_rx_kfifo[id]->in;
				s_uart_rx_kfifo[id]->in = s_uart_rx_kfifo[id]->out;
#if CONFIG_UART_ERR_INTERRUPT
				UART_LOGW("uart rx error(0x%x)!,discard_len=%d\r\n", (status & (BIT(2) | BIT(3) | BIT(4))), discard_len);
#endif
			}
			else
#endif
			{
				int ret = 0;
				uint8_t rx_data;

				/* read all data from rx-FIFO. */
				while (1)
				{
					ret = uart_read_byte_ex(id, &rx_data);
					if (ret == -1)
					{
						#if CONFIG_UART_ERR_INTERRUPT
						UART_LOGW("uart rx error(0x%x) triggered!\r\n", (status & (BIT(2) | BIT(3) | BIT(4))));
						#endif
						break;
					}
				}
			}
		}

		UART_STATIS_INC(uart_statis->rx_isr_cnt);
		UART_STATIS_SET(uart_statis->rx_fifo_cnt, uart_hal_get_rx_fifo_cnt(&s_uart[id].hal, id));
		if (uart_id_is_sw_fifo_enabled(id))
		{
#if CONFIG_UART_RX_DMA
			if(s_uart[id].rx_dma_enable)
			{
				//force all of the UART RX FIFO data to DMA RAM space
				bk_dma_flush_src_buffer(s_uart[id].rx_dma_id);
				if (uart_id_dma_read_fifo_frame(id, s_uart_rx_kfifo[id]) > 0)
				{
					if (s_uart_sema[id].rx_int_sema && s_uart_sema[id].rx_blocked)
					{
						rtos_set_semaphore(&(s_uart_sema[id].rx_int_sema));
						s_uart_sema[id].rx_blocked = false;
					}
				}
			}
			else
#endif
			if (uart_id_read_fifo_frame(id, s_uart_rx_kfifo[id]) > 0)
			{
				if (s_uart_sema[id].rx_int_sema && s_uart_sema[id].rx_blocked)
				{
					rtos_set_semaphore(&(s_uart_sema[id].rx_int_sema));
					s_uart_sema[id].rx_blocked = false;
				}
			}
		}
		else if (s_uart_rx_isr[id].callback)
		{
				s_uart_rx_isr[id].callback(id, s_uart_rx_isr[id].param);
		}
		else
		{
			int ret = 0;
			uint8_t rx_data;

			/* read all data from rx-FIFO. */
		 	while (1)
			{
				ret = uart_read_byte_ex(id, &rx_data);
				if (ret == -1)
				{
					break;
				}
			}
		}
	}

	if (uart_hal_is_tx_interrupt_triggered(&s_uart[id].hal, id, status))
	{
		if (s_uart_tx_isr[id].callback)
		{
			s_uart_tx_isr[id].callback(id, s_uart_tx_isr[id].param);
		}
	}
}

void uart0_isr(void)
{
	uart_isr_common(UART_ID_0);
}

void uart1_isr(void)
{
	uart_isr_common(UART_ID_1);
}

void uart2_isr(void)
{
	uart_isr_common(UART_ID_2);
}

