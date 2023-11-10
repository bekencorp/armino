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

#pragma once

#include <soc/soc.h>
#include "uart_hw.h"
#include "hal_port.h"
#include "sys_ll_op_if.h"
#include <driver/hal/hal_uart_types.h>
#include <driver/hal/hal_gpio_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UART_LL_REG_BASE(_uart_unit_id) (uart_ll_reg_base_select(_uart_unit_id))
#define CASE_PARITY(parity) case UART_PARITY_##parity: return UART_V_PARITY_##parity
#define CASE_D() default: return 0

#define UART1_LL_TX_PIN  GPIO_11
#define UART1_LL_RX_PIN  GPIO_10
#define UART1_LL_CTS_PIN  GPIO_12
#define UART1_LL_RTS_PIN  GPIO_13


#define UART2_LL_TX_PIN  GPIO_0
#define UART2_LL_RX_PIN  GPIO_1

#define UART3_LL_TX_PIN  GPIO_41
#define UART3_LL_RX_PIN  GPIO_40

static inline void uart_ll_init(uart_hw_t *hw)
{

}

static inline gpio_id_t uart_ll_get_tx_pin(uart_id_t id)
{
	switch (id)
	{
		case UART_ID_0:
			return UART1_LL_TX_PIN;
		case UART_ID_1:
			return UART2_LL_TX_PIN;
		case UART_ID_2:
			return UART3_LL_TX_PIN;
		default:
			return SOC_GPIO_NUM;
	}
}

static inline gpio_id_t uart_ll_get_rx_pin(uart_id_t id)
{
	switch (id)
	{
		case UART_ID_0:
			return UART1_LL_RX_PIN;
		case UART_ID_1:
			return UART2_LL_RX_PIN;
		case UART_ID_2:
			return UART3_LL_RX_PIN;
		default:
			return SOC_GPIO_NUM;
	}
}

static inline gpio_id_t uart_ll_get_cts_pin(uart_id_t id)
{
	switch (id)
	{
		case UART_ID_0:
			return UART1_LL_CTS_PIN;
		default:
			return SOC_GPIO_NUM;
	}
}

static inline gpio_id_t uart_ll_get_rts_pin(uart_id_t id)
{
	switch (id)
	{
		case UART_ID_0:
			return UART1_LL_RTS_PIN;
		default:
			return SOC_GPIO_NUM;
	}
}

static inline uint32_t uart_ll_reg_base_select(uart_id_t id)
{
	switch (id)
	{
		case UART_ID_0:
			return UART1_R_BASE;
		case UART_ID_1:
			return UART2_R_BASE;
		case UART_ID_2:
			return UART3_R_BASE;
		default:
			return BK_ERR_UART_BASE;
			break;
	}
}

static inline uint32_t uart_ll_to_reg_parity(uart_parity_t parity)
{
    switch (parity)
    {
        CASE_PARITY(ODD);
        CASE_PARITY(EVEN);
        CASE_D();
    }
}

static inline bool uart_ll_get_tx_enable(uart_hw_t *hw)
{
	return hw->config.tx_enable;
}

static inline void uart_ll_set_tx_enable(uart_hw_t *hw, uart_id_t id, uint32_t value)
{
	hw->config.tx_enable = value & 0x01;
}

static inline void uart_ll_enable_tx(uart_hw_t *hw, uart_id_t id)
{
	uart_ll_set_tx_enable(hw, id, 1);
}

static inline void uart_ll_disable_tx(uart_hw_t *hw, uart_id_t id)
{
	uart_ll_set_tx_enable(hw, id, 0);
}

static inline bool uart_ll_get_rx_enable(uart_hw_t *hw)
{
	return hw->config.rx_enable;
}

static inline void uart_ll_set_rx_enable(uart_hw_t *hw, uart_id_t id, uint32_t value)
{
	hw->config.rx_enable = value & 0x1;
}

static inline void uart_ll_enable_rx(uart_hw_t *hw, uart_id_t id)
{
	uart_ll_set_rx_enable(hw, id, 1);
}

static inline void uart_ll_set_frame_mode(uart_hw_t *hw, uart_id_t id, uart_frame_mode_t mode)
{
	hw->config.mode = mode;
}

static inline void uart_ll_set_mode_uart(uart_hw_t *hw, uart_id_t id)
{
	hw->config.mode = UART_V_MODE_UART;
}

static inline void uart_ll_set_mode_irda(uart_hw_t *hw, uart_id_t id)
{
	hw->config.mode = UART_V_MODE_IRDA;
}

static inline void uart_ll_disable_rx(uart_hw_t *hw, uart_id_t id)
{
	uart_ll_set_rx_enable(hw, id, 0);
}

static inline void uart_ll_set_data_bits(uart_hw_t *hw, uart_id_t id, uart_data_bits_t value)
{
	hw->config.data_bits = value & 0x3;
}

static inline void uart_ll_enable_parity(uart_hw_t *hw, uart_id_t id)
{
	hw->config.parity_en = 1;
}

static inline void uart_ll_disable_parity(uart_hw_t *hw, uart_id_t id)
{
	hw->config.parity_en = 0;
}

static inline void uart_ll_set_parity(uart_hw_t *hw, uart_id_t id, uart_parity_t value)
{
	hw->config.parity = uart_ll_to_reg_parity(value);
}

static inline void uart_ll_set_stop_bits(uart_hw_t *hw, uart_id_t id, uart_stop_bits_t value)
{
	hw->config.stop_bits = value;
}

static inline void uart_ll_set_clk_div(uart_hw_t *hw, uart_id_t id, uint32_t value)
{
	hw->config.clk_div = value & UART_F_CLK_DIV_M;
}

static inline void uart_ll_set_tx_fifo_threshold(uart_hw_t *hw, uart_id_t id, uint32_t value)
{
	hw->fifo_config.tx_fifo_threshold = value & 0xff;
}

static inline void uart_ll_set_rx_fifo_threshold(uart_hw_t *hw, uart_id_t id, uint32_t value)
{
	hw->fifo_config.rx_fifo_threshold = value & 0xff;
}

static inline void uart_ll_set_rx_stop_detect_time(uart_hw_t *hw, uart_id_t id, uint32_t value)
{
	hw->fifo_config.rx_stop_detect_time = value & 0x3;
}

static inline void uart_ll_reset_config_to_default(uart_hw_t *hw, uart_id_t id)
{
	hw->config.v = 0;
	hw->fifo_config.v = 0;
}

static inline uint32_t uart_ll_get_tx_fifo_cnt(uart_hw_t *hw, uart_id_t id)
{
	return hw->fifo_status.tx_fifo_count;
}

static inline uint32_t uart_ll_get_rx_fifo_cnt(uart_hw_t *hw, uart_id_t id)
{
	return hw->fifo_status.rx_fifo_count;
}

static inline bool uart_ll_is_tx_fifo_full(uart_hw_t *hw, uart_id_t id)
{
	return !!(hw->fifo_status.tx_fifo_full);
}

static inline bool uart_ll_is_tx_fifo_empty(uart_hw_t *hw, uart_id_t id)
{
	return !!(hw->fifo_status.tx_fifo_empty);
}

static inline bool uart_ll_is_rx_fifo_full(uart_hw_t *hw, uart_id_t id)
{
	return !!(hw->fifo_status.rx_fifo_full);
}

static inline bool uart_ll_is_rx_fifo_empty(uart_hw_t *hw, uart_id_t id)
{
	return !!(hw->fifo_status.rx_fifo_empty);
}

static inline bool uart_ll_is_fifo_write_ready(uart_hw_t *hw, uart_id_t id)
{
	return !!(hw->fifo_status.fifo_wr_ready);
}

static inline bool uart_ll_is_fifo_read_ready(uart_hw_t *hw, uart_id_t id)
{
	return !!(hw->fifo_status.fifo_rd_ready);
}

static inline void uart_ll_reset_fifo_port_to_default(uart_hw_t *hw, uart_id_t id)
{
	hw->fifo_port.v = 0xffff;
}

static inline void uart_ll_write_byte(uart_hw_t *hw, uart_id_t id, uint8_t data)
{
	/* note: hw->fifo_port.tx_fifo_data_in = data; is incorrect */
	hw->fifo_port.v = data & 0xff;
}

static inline uint8_t uart_ll_read_byte(uart_hw_t *hw, uart_id_t id)
{
	return hw->fifo_port.rx_fifo_data_out;
}

static inline void uart_ll_reset_int_en_to_default(uart_hw_t *hw, uart_id_t id)
{
	hw->int_enable.v = 0;
}

static inline void uart_ll_enable_rx_interrupt(uart_hw_t *hw, uart_id_t id)
{
	hw->int_enable.rx_fifo_need_read = 1;
	hw->int_enable.rx_finish = 1;
}

static inline void uart_ll_disable_rx_interrupt(uart_hw_t *hw, uart_id_t id)
{
	hw->int_enable.rx_fifo_need_read = 0;
	hw->int_enable.rx_finish = 0;
}

static inline void uart_ll_enable_tx_interrupt(uart_hw_t *hw, uart_id_t id)
{
	hw->int_enable.tx_fifo_need_write = 1;
}

static inline void uart_ll_disable_tx_interrupt(uart_hw_t *hw, uart_id_t id)
{
	hw->int_enable.tx_fifo_need_write = 0;
}

static inline uint32_t uart_ll_get_int_enable_status(uart_hw_t *hw, uart_id_t id)
{
	return hw->int_enable.v & 0xff;
}

static inline void uart_ll_set_int_enable_status(uart_hw_t *hw, uart_id_t id, uint32_t value)
{
	hw->int_enable.v = value;
}

static inline uint32_t uart_ll_get_interrupt_status(uart_hw_t *hw, uart_id_t id)
{
	return hw->int_status.v & 0xff;
}

static inline void uart_ll_clear_interrupt_status(uart_hw_t *hw, uart_id_t id, uint32_t status)
{
	hw->int_status.v = status & 0xff;
}

static inline void uart_ll_clear_id_interrupt_status(uart_hw_t *hw, uart_id_t id)
{
	hw->int_status.v = 0xff;
}

static inline void uart_ll_clear_id_tx_interrupt_status(uart_hw_t *hw, uart_id_t id)
{
	/* 
	 * WARNING:This REG has many bits which attribute is write 1 to clear.
	 * If uses union bit operation, the asm codes will read data from REG
	 * and write it back, maybe the other bits is 1 and cleared by this function.
	 */
#if 0
	hw->int_status.tx_fifo_need_write = 1;
	hw->int_status.tx_finish = 1;
#endif
	//BIT(0):need write, BIT(5): tx finish
	hw->int_status.v = (BIT(0) | BIT(5));
}

static inline void uart_ll_clear_id_rx_interrupt_status(uart_hw_t *hw, uart_id_t id)
{
	/* 
	 * WARNING:This REG has many bits which attribute is write 1 to clear.
	 * If uses union bit operation, the asm codes will read data from REG
	 * and write it back, maybe the other bits is 1 and cleared by this function.
	 */
#if 0
	hw->int_status.rx_fifo_need_read = 1;
	hw->int_status.rx_finish = 1;
#endif
	//BIT(1):need read, BIT(6): rx finish
	hw->int_status.v = (BIT(1) | BIT(6));
}

static inline bool uart_ll_is_rx_interrupt_triggered(uart_hw_t *hw, uart_id_t id, uint32_t status)
{
	return !!(status & (BIT(1) | BIT(6)));
}

static inline bool uart_ll_is_rx_recv_fini_int_triggered(uart_hw_t *hw, uart_id_t id, uint32_t status)
{
	return !!(status & BIT(6));
}

static inline bool uart_ll_is_tx_interrupt_triggered(uart_hw_t *hw, uart_id_t id, uint32_t status)
{
	return !!(status & BIT(0));
}

static inline bool uart_ll_is_rx_parity_err_int_triggered(uart_hw_t *hw, uart_id_t id, uint32_t status)
{
	return !!(status & BIT(3));
}

static inline void uart_ll_reset_flow_control_to_default(uart_hw_t *hw, uart_id_t id)
{
	hw->flow_ctrl_config.v = 0;
}

static inline void uart_ll_enable_flow_control(uart_hw_t *hw, uart_id_t id)
{
	hw->flow_ctrl_config.flow_ctrl_en = 1;
}

static inline void uart_ll_disable_flow_control(uart_hw_t *hw, uart_id_t id)
{
	hw->flow_ctrl_config.flow_ctrl_en = 0;
}

static inline bool uart_ll_is_flow_control_enabled(uart_hw_t *hw, uart_id_t id)
{
	return !!(hw->flow_ctrl_config.flow_ctrl_en);
}

static inline void uart_ll_set_flow_control_low_cnt(uart_hw_t *hw, uart_id_t id, uint32_t cnt)
{
	hw->flow_ctrl_config.flow_ctrl_low_cnt = cnt & 0xff;
}

static inline void uart_ll_set_flow_control_high_cnt(uart_hw_t *hw, uart_id_t id, uint32_t cnt)
{
	hw->flow_ctrl_config.flow_ctrl_high_cnt = cnt & 0xff;
}

static inline void uart_ll_set_rts_polarity(uart_hw_t *hw, uart_id_t id, uint32_t value)
{
	hw->flow_ctrl_config.rts_polarity_sel = value & 0x1;
}

static inline void uart_ll_set_cts_polarity(uart_hw_t *hw, uart_id_t id, uint32_t value)
{
	hw->flow_ctrl_config.cts_polarity_sel = value & 0x1;
}

static inline void uart_ll_reset_wake_config_to_default(uart_hw_t *hw, uart_id_t id)
{
	hw->wake_config.v = 0;
}

static inline void uart_ll_enable_fifo_over_flow_interrupt(uart_hw_t *hw, uart_id_t id)
{
	hw->int_enable.rx_fifo_overflow = 1;
}

static inline void uart_ll_disable_fifo_over_flow_interrupt(uart_hw_t *hw, uart_id_t id)
{
	hw->int_enable.rx_fifo_overflow = 0;
}

static inline void uart_ll_enable_parity_err_interrupt(uart_hw_t *hw, uart_id_t id)
{
	hw->int_enable.rx_parity_err = 1;
}

static inline void uart_ll_disable_parity_err_interrupt(uart_hw_t *hw, uart_id_t id)
{
	hw->int_enable.rx_parity_err = 0;
}

static inline void uart_ll_enable_stop_bit_err_interrupt(uart_hw_t *hw, uart_id_t id)
{
	hw->int_enable.rx_stop_bits_err = 1;
}

static inline bool uart_ll_is_rx_over_flow_int_triggered(uart_hw_t *hw, uart_id_t id, uint32_t status)
{
	return !!(status & BIT(2));
}

static inline bool uart_ll_is_rx_stop_bits_err_int_triggered(uart_hw_t *hw, uart_id_t id, uint32_t status)
{
	return !!(status & BIT(4));
}

static inline uint32_t uart_ll_wait_tx_over(void)
{
	uint32_t uart_wait_us;
	uint32_t baudrate1;
	uint32_t baudrate2;
	uart_hw_t *hw1 = (uart_hw_t *)UART_LL_REG_BASE(0);
	uart_hw_t *hw2 = (uart_hw_t *)UART_LL_REG_BASE(1);

	baudrate1 = UART_CLOCK / (hw1->config.clk_div + 1);
	baudrate2 = UART_CLOCK / (hw2->config.clk_div + 1);

	uart_wait_us = 1000000 * hw2->fifo_status.tx_fifo_count * 10 / baudrate2
				 + 1000000 * hw1->fifo_status.tx_fifo_count * 10 / baudrate1;

	while (!hw2->fifo_status.tx_fifo_empty);
	while (!hw1->fifo_status.tx_fifo_empty);

	return uart_wait_us;
}

#ifdef __cplusplus
}
#endif

