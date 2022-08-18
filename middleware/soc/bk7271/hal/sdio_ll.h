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
#include "hal_port.h"
#include "sdio_hw.h"
#include <driver/hal/hal_sdio_host_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SDIO_LL_REG_BASE(_sdio_host_unit_id)    (SOC_SDIO_HOST_REG_BASE)
#define SDIO_HOST_DEFAULT_TX_FIFO_THRD      (0x01) // 16byte
#define SDIO_HOST_DEFAULT_RX_FIFO_THRD      (0x01)

static inline void sdio_host_ll_reset_config_to_default(sdio_hw_t *hw)
{
	/* clear cmd rsp int bit */
	uint32_t reg_val = REG_READ(SDIO_HOST_R_CMD_RSP_INT_SEL);
	REG_WRITE(SDIO_HOST_R_CMD_RSP_INT_SEL, reg_val);

	/* clear tx/rx fifo */
	hw->sd_fifo_threshold.rx_fifo_reset = 1;
	hw->sd_fifo_threshold.tx_fifo_reset = 1;
	hw->sd_fifo_threshold.sd_start_reset = 1;

	/* disabe all sdio interrupt */
	REG_WRITE(SDIO_HOST_R_CMD_RSP_INT_MASK, 0);

	/* config tx/rx fifo threshold */
	hw->sd_fifo_threshold.rx_fifo_threshold = SDIO_HOST_DEFAULT_RX_FIFO_THRD;
	hw->sd_fifo_threshold.tx_fifo_threshold = SDIO_HOST_DEFAULT_RX_FIFO_THRD;
}

static inline void sdio_host_ll_init(sdio_hw_t *hw)
{
	sdio_host_ll_reset_config_to_default(hw);
}

static inline void sdio_host_ll_set_cmd_index(sdio_hw_t *hw, uint32_t cmd_index)
{
	hw->sd_cmd_ctrl.cmd_index = cmd_index;
}

/* 1: need response, 0: not need response */
static inline void sdio_host_ll_set_cmd_rsp(sdio_hw_t *hw, uint32_t is_need_rsp)
{
	hw->sd_cmd_ctrl.rsp_en = is_need_rsp;
}

/* 1: need long response, 0: need short response */
static inline void sdio_host_ll_set_cmd_long_rsp(sdio_hw_t *hw, uint32_t is_long_rsp)
{
	hw->sd_cmd_ctrl.long_rsp_en = is_long_rsp;
}

/* 1: need check crc, 0: not check crc */
static inline void sdio_host_ll_set_cmd_crc_check(sdio_hw_t *hw, uint32_t is_need_crc_check)
{
	hw->sd_cmd_ctrl.crc_check_en = is_need_crc_check;
}

static inline void sdio_host_ll_start_send_cmd(sdio_hw_t *hw)
{
	hw->sd_fifo_threshold.tx_fifo_reset = 1;
	hw->sd_fifo_threshold.rx_fifo_reset = 1;
	hw->sd_fifo_threshold.sd_start_reset = 1;
	hw->sd_cmd_ctrl.start = 1;
}

static inline void sdio_host_ll_set_clk_div(sdio_hw_t *hw, uint32_t clk_div)
{
	hw->sd_fifo_threshold.sd_rate_mux = clk_div;
}

static inline void sdio_host_ll_set_clk_freq(sdio_hw_t *hw, sdio_host_clock_freq_t clk_freq)
{
	switch (clk_freq) {
	case SDIO_HOST_CLK_26M:
		sdio_host_ll_set_clk_div(hw, 0x0);
		break;
	case SDIO_HOST_CLK_13M:
		sdio_host_ll_set_clk_div(hw, 0x1);
		break;
	case SDIO_HOST_CLK_6_5M:
		sdio_host_ll_set_clk_div(hw, 0x2);
		break;
	case SDIO_HOST_CLK_200K:
	default:
		sdio_host_ll_set_clk_div(hw, 0x3);
		break;
	}
}

static inline void sdio_host_ll_set_bus_width(sdio_hw_t *hw, sdio_host_bus_width_t bus_width)
{
	if (bus_width == SDIO_HOST_BUS_WIDTH_4LINE) {
		hw->sd_data_ctrl.sd_data_bus = 1;
	} else {
		hw->sd_data_ctrl.sd_data_bus = 0;
	}
}

static inline void sdio_host_ll_set_send_cmd_argument(sdio_hw_t *hw, uint32_t cmd_argument)
{
	hw->cmd_argument = cmd_argument;
}

static inline void sdio_host_ll_set_cmd_rsp_timeout(sdio_hw_t *hw, uint32_t rsp_timeout)
{
	hw->sd_cmd_timer = rsp_timeout;
}

static inline uint32_t sdio_host_ll_get_interrupt_status(sdio_hw_t *hw)
{
	return REG_READ(SDIO_HOST_R_CMD_RSP_INT_SEL);
}

static inline void sdio_host_ll_clear_interrupt_status(sdio_hw_t *hw)
{
	REG_WRITE(SDIO_HOST_R_CMD_RSP_INT_SEL, 0xFFFFFFFF);
}

static inline bool sdio_host_ll_is_cmd_rsp_interrupt_triggered(sdio_hw_t *hw, uint32_t int_status)
{
	return (int_status & (SDIO_HOST_F_CMD_NO_RSP_END_INT | SDIO_HOST_F_CMD_RSP_END_INT | SDIO_HOST_F_CMD_RSP_TIMEOUT_INT));
}

static inline bool sdio_host_ll_is_cmd_rsp_timeout_interrupt_triggered(sdio_hw_t *hw, uint32_t int_status)
{
	return (int_status & SDIO_HOST_F_CMD_RSP_TIMEOUT_INT);
}

static inline bool sdio_host_ll_is_cmd_rsp_crc_fail_interrupt_triggered(sdio_hw_t *hw, uint32_t int_status)
{
	return (int_status & SDIO_HOST_F_CMD_RSP_CRC_FAIL);
}

static inline void sdio_host_ll_clear_cmd_rsp_interrupt_status(sdio_hw_t *hw, uint32_t int_status)
{
	int_status |= (SDIO_HOST_F_CMD_NO_RSP_END_INT |
					SDIO_HOST_F_CMD_RSP_END_INT |
					SDIO_HOST_F_CMD_RSP_TIMEOUT_INT |
					SDIO_HOST_F_CMD_RSP_CRC_FAIL);
	REG_WRITE(SDIO_HOST_R_CMD_RSP_INT_SEL, int_status);
}

static inline uint32_t sdio_host_ll_get_cmd_rsp_argument(sdio_hw_t *hw, sdio_host_response_t argument_index)
{
	uint32_t argument = 0;
	switch (argument_index) {
	case SDIO_HOST_RSP0:
		argument = hw->sd_rsp_agument_0;
		break;
	case SDIO_HOST_RSP1:
		argument = hw->sd_rsp_agument_1;
		break;
	case SDIO_HOST_RSP2:
		argument = hw->sd_rsp_agument_2;
		break;
	case SDIO_HOST_RSP3:
		argument = hw->sd_rsp_agument_3;
		break;
	default:
		break;
	}
	return argument;
}

static inline void sdio_host_ll_set_tx_fifo_threshold(sdio_hw_t *hw, uint32_t threshold)
{
	hw->sd_fifo_threshold.tx_fifo_threshold = threshold & 0xff;
}

static inline void sdio_host_ll_set_rx_fifo_threshold(sdio_hw_t *hw, uint32_t threshold)
{
	hw->sd_fifo_threshold.rx_fifo_threshold = threshold & 0xff;
}

static inline void sdio_host_ll_start_receive_data(sdio_hw_t *hw)
{
	hw->sd_data_ctrl.sd_data_en = 1;
}

static inline void sdio_host_ll_set_write_data(sdio_hw_t *hw, uint32_t size)
{
	hw->sd_fifo_threshold.tx_fifo_reset = 1;
	hw->sd_fifo_threshold.sd_start_reset = 1;
#ifdef CONFIG_SDCARD_BUSWIDTH_4LINE
	hw->sd_data_ctrl.sd_data_bus = 1;
#else
	hw->sd_data_ctrl.sd_data_bus = 0;
#endif
	hw->sd_data_ctrl.sd_data_mul_blk = 1;
	hw->sd_data_ctrl.sd_data_blk = size & SDIO_HOST_F_DATA_BLOCK_SIZE_M;
	hw->sd_data_ctrl.sd_byte_sel = 1;
}

static inline void sdio_host_ll_set_read_data(sdio_hw_t *hw, uint32_t block_size)
{
	hw->sd_fifo_threshold.sd_start_reset = 1;
	hw->sd_fifo_threshold.rx_fifo_reset = 1;
#ifdef CONFIG_SDCARD_BUSWIDTH_4LINE
	hw->sd_data_ctrl.sd_data_bus = 1;
#else
	hw->sd_data_ctrl.sd_data_bus = 0;
#endif
	hw->sd_data_ctrl.sd_data_mul_blk = 1;
	hw->sd_data_ctrl.sd_data_blk = block_size & SDIO_HOST_F_DATA_BLOCK_SIZE_M;
	hw->sd_data_ctrl.sd_byte_sel = 1;
}

static inline void sdio_host_ll_start_send_data(sdio_hw_t *hw)
{
	hw->sd_data_ctrl.sd_start_wr_en = 1;
}

static inline void sdio_host_ll_stop_send_data(sdio_hw_t *hw)
{
	hw->sd_data_ctrl.sd_start_wr_en = 0;
}

static inline void sdio_host_ll_set_data_timeout(sdio_hw_t *hw, uint32_t data_timeout)
{
	hw->sd_data_timer = data_timeout;
}

static inline bool sdio_host_ll_is_tx_fifo_write_ready(sdio_hw_t *hw)
{
	return hw->sd_fifo_threshold.tx_fifo_wr_ready;
}

static inline bool sdio_host_ll_is_rx_fifo_read_ready(sdio_hw_t *hw)
{
	return hw->sd_fifo_threshold.rx_fifo_rd_ready;
}

static inline void sdio_host_ll_write_fifo(sdio_hw_t *hw, uint32_t data)
{
	hw->tx_fifo_din = data;
}

static inline uint32_t sdio_host_ll_read_fifo(sdio_hw_t *hw)
{
	return hw->rx_fifo_dout;
}

static inline bool sdio_host_ll_is_recv_data_interrupt_triggered(sdio_hw_t *hw, uint32_t int_status)
{
	return (int_status & (SDIO_HOST_F_DATA_RECV_END_INT | SDIO_HOST_F_DATA_CRC_FAIL | SDIO_HOST_F_DATA_TIMEOUT_INT));
}

static inline void sdio_host_ll_clear_data_interrupt_status(sdio_hw_t *hw, uint32_t int_status)
{
	int_status |= (SDIO_HOST_F_DATA_RECV_END_INT |
					SDIO_HOST_F_DATA_CRC_FAIL |
					SDIO_HOST_F_DATA_WR_END_INT |
					SDIO_HOST_F_DATA_TIMEOUT_INT);
	REG_WRITE(SDIO_HOST_R_CMD_RSP_INT_SEL, int_status);
}

static inline bool sdio_host_ll_is_data_timeout_int_triggered(sdio_hw_t *hw, uint32_t int_status)
{
	return (int_status & SDIO_HOST_F_DATA_TIMEOUT_INT);
}

static inline bool sdio_host_ll_is_data_crc_fail_int_triggered(sdio_hw_t *hw, uint32_t int_status)
{
	return (int_status & SDIO_HOST_F_DATA_CRC_FAIL);
}

static inline bool sdio_host_ll_is_data_recv_end_int_triggered(sdio_hw_t *hw, uint32_t int_status)
{
	return (int_status & SDIO_HOST_F_DATA_RECV_END_INT);
}

static inline bool sdio_host_ll_is_data_write_end_int_triggered(sdio_hw_t *hw, uint32_t int_status)
{
	return (int_status & SDIO_HOST_F_DATA_WR_END_INT);
}

static inline bool sdio_host_ll_is_fifo_empty_int_triggered(sdio_hw_t *hw, uint32_t int_status)
{
	return (int_status & SDIO_HOST_F_FIFO_EMPTY_INT);
}

static inline void sdio_host_ll_enable_tx_fifo_empty_mask(sdio_hw_t *hw)
{
	hw->sd_cmd_rsp_int_mask.tx_fifo_empty_mask = 1;
}

static inline void sdio_host_ll_disable_tx_fifo_empty_mask(sdio_hw_t *hw)
{
	hw->sd_cmd_rsp_int_mask.tx_fifo_empty_mask = 0;
}

static inline bool sdio_host_ll_is_tx_fifo_need_write(sdio_hw_t *hw, uint32_t int_status)
{
	return (int_status & SDIO_HOST_F_TX_FIFO_NEED_WRITE);
	//return hw->sd_cmd_rsp_int_sel.tx_fifo_need_write;
}

#ifdef __cplusplus
}
#endif

