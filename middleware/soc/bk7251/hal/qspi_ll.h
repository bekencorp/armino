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

#include "bk_sys_ctrl.h"
#include "drv_model_pub.h"
#include "hal_port.h"
#include "qspi_hw.h"
#include <driver/hal/hal_qspi_types.h>
#include <soc/soc.h>
#include "sys_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define QSPI_LL_REG_BASE(_qspi_unit_id)    (QSPI_R_BASE)

#define QSPI_LL_RAM_CSN_PIN      GPIO_26
#define QSPI_LL_RAM_CLK_PIN      GPIO_24
#define QSPI_LL_FLASH_CSN_PIN    GPIO_15
#define QSPI_LL_FLASH_CLK_PIN    GPIO_14
#define QSPI_LL_IO0_PIN          GPIO_16
#define QSPI_LL_IO1_PIN          GPIO_17
#define QSPI_LL_IO2_PIN          GPIO_18
#define QSPI_LL_IO3_PIN          GPIO_19

#define QSPI_LL_GE0_DEP    0x10
#define QSPI_LL_GE1_DEP    0x10

static inline void qspi_ll_init(qspi_hw_t *hw)
{

}

static inline void qspi_ll_set_sw_cmd_bw(qspi_hw_t *hw, uint32_t sw_cmd_bw)
{
	hw->sw_cmd.sw_cmd_bw = sw_cmd_bw;
}

static inline void qspi_ll_set_sw_cmd(qspi_hw_t *hw, uint32_t sw_cmd)
{
	hw->sw_cmd.sw_cmd = sw_cmd & QSPI_F_SW_CMD_M;
}

static inline void qspi_ll_set_sw_cmd_cnt(qspi_hw_t *hw, uint32_t sw_cmd_cnt)
{
	hw->sw_cmd.sw_cmd_cnt = sw_cmd_cnt;
}

static inline void qspi_ll_enable_sw_cmd(qspi_hw_t *hw)
{
	hw->sw_cmd.sw_cmd_en = 1;
}

static inline void qspi_ll_disable_sw_cmd(qspi_hw_t *hw)
{
	hw->sw_cmd.sw_cmd_en = 0;
}

static inline void qspi_ll_enable_sw_addr(qspi_hw_t *hw)
{
	hw->sw_addr.sw_addr_en = 1;
}

static inline void qspi_ll_disable_sw_addr(qspi_hw_t *hw)
{
	hw->sw_addr.sw_addr_en = 0;
}

static inline void qspi_ll_set_sw_addr_bw(qspi_hw_t *hw, uint32_t sw_addr_bw)
{
	hw->sw_addr.sw_addr_bw = sw_addr_bw;
}

static inline void qspi_ll_set_sw_addr(qspi_hw_t *hw, uint32_t sw_addr)
{
	hw->sw_addr.sw_addr = sw_addr & QSPI_F_SW_ADDR_M;
}

static inline void qspi_ll_set_sw_addr_cnt(qspi_hw_t *hw, uint32_t sw_addr_cnt)
{
	hw->sw_addr.sw_addr_cnt = sw_addr_cnt;
}

static inline void qspi_ll_enable_sw_dummy(qspi_hw_t *hw)
{
	hw->sw_dum.sw_dum_en = 1;
}

static inline void qspi_ll_disable_sw_dummy(qspi_hw_t *hw)
{
	hw->sw_dum.sw_dum_en = 0;
}

static inline void qspi_ll_set_sw_dummy_bw(qspi_hw_t *hw, uint32_t sw_dummy_bw)
{
	hw->sw_dum.sw_dum_bw = sw_dummy_bw;
}

static inline void qspi_ll_set_sw_dummy_cnt(qspi_hw_t *hw, uint32_t sw_dummy_cnt)
{
	hw->sw_dum.sw_dum_cnt = sw_dummy_cnt & QSPI_F_SW_DUMMY_M;
}

static inline void qspi_ll_enable_sw_data(qspi_hw_t *hw)
{
	hw->sw_dat.sw_dat_en = 1;
}

static inline void qspi_ll_disable_sw_data(qspi_hw_t *hw)
{
	hw->sw_dat.sw_dat_en = 0;
}

static inline void qspi_ll_set_sw_data_bw(qspi_hw_t *hw, uint32_t sw_data_bw)
{
	hw->sw_dat.sw_dat_bw = sw_data_bw;
}

static inline void qspi_ll_set_sw_data_size(qspi_hw_t *hw, uint32_t sw_data_size)
{
	hw->sw_dat.sw_dat_cnt = sw_data_size;
}

static inline void qspi_ll_set_sw_data_direction(qspi_hw_t *hw, qspi_op_t sw_data_dir)
{
	hw->sw_dat.sw_dat_dir = sw_data_dir;
}

static inline void qspi_ll_set_sw_op(qspi_hw_t *hw, uint32_t sw_op)
{
	hw->sw_op.v = sw_op;
}

static inline void qspi_ll_set_sw_op_type(qspi_hw_t *hw, qspi_sw_op_type_t op_type)
{
	hw->sw_op.sw_op_type = op_type;
}

static inline void qspi_ll_start_sw_op(qspi_hw_t *hw)
{
	hw->sw_op.sw_start = 1;
}

static inline void qspi_ll_stop_sw_op(qspi_hw_t *hw)
{
	hw->sw_op.sw_start = 0;
}

static inline void qspi_ll_clear_sw_op_int(qspi_hw_t *hw)
{
	hw->sw_op.int_sw_op_clr = 1;
}

static inline void qspi_ll_enable_sw_op_int_mask(qspi_hw_t *hw)
{
	hw->sw_op.int_mask |= BIT(16);
}

static inline void qspi_ll_disable_sw_op_int_mask(qspi_hw_t *hw)
{
	hw->sw_op.int_mask &= ~BIT(16);
}


static inline void qspi_ll_set_ge0_fifo_data(qspi_hw_t *hw, uint32_t fifo_data)
{
	hw->ge0_trxfifo_data = fifo_data;
}

static inline void qspi_ll_set_ge0_wr_th(qspi_hw_t *hw, uint32_t ge0_wr_th)
{
	hw->ge0_th.ge0wr_th = ge0_wr_th;
}

static inline void qspi_ll_set_ge0_rd_th(qspi_hw_t *hw, uint32_t ge0_rd_th)
{
	hw->ge0_th.ge0rd_th = ge0_rd_th;
}

static inline void qspi_ll_set_ge1_wr_th(qspi_hw_t *hw, uint32_t ge1_wr_th)
{
	hw->ge1_th.ge1wr_th = ge1_wr_th;
}

static inline void qspi_ll_set_ge1_rd_th(qspi_hw_t *hw, uint32_t ge1_rd_th)
{
	hw->ge1_th.ge1rd_th = ge1_rd_th;
}

static inline void qspi_ll_set_ge0_wr_dep(qspi_hw_t *hw, uint32_t ge0_wr_dep)
{
	hw->ge0_dep.ge0wr_dep = ge0_wr_dep;
}

static inline void qspi_ll_set_ge0_rd_dep(qspi_hw_t *hw, uint32_t ge0_rd_dep)
{
	hw->ge0_dep.ge0rd_dep = ge0_rd_dep;
}

static inline void qspi_ll_set_ge1_wr_dep(qspi_hw_t *hw, uint32_t ge1_wr_dep)
{
	hw->ge1_dep.ge1wr_dep = ge1_wr_dep;
}

static inline void qspi_ll_set_ge1_rd_dep(qspi_hw_t *hw, uint32_t ge1_rd_dep)
{
	hw->ge1_dep.ge1rd_dep = ge1_rd_dep;
}

static inline void qspi_ll_enable_fls_tx(qspi_hw_t *hw)
{
	hw->enable.fls_tx_enable = 1;
}

static inline void qspi_ll_disable_fls_tx(qspi_hw_t *hw)
{
	hw->enable.fls_tx_enable = 0;
}

static inline void qspi_ll_enable_ge1_rx(qspi_hw_t *hw)
{
	hw->enable.ge1_rx_enable = 1;
}

static inline void qspi_ll_disable_ge1_rx(qspi_hw_t *hw)
{
	hw->enable.ge1_rx_enable = 0;
}

static inline void qspi_ll_enable_ge1_tx(qspi_hw_t *hw)
{
	hw->enable.ge1_tx_enable = 1;
}

static inline void qspi_ll_disable_ge1_tx(qspi_hw_t *hw)
{
	hw->enable.ge1_tx_enable = 0;
}

static inline void qspi_ll_enable_ge0_rx(qspi_hw_t *hw)
{
	hw->enable.ge0_rx_enable = 1;
}

static inline void qspi_ll_disable_ge0_rx(qspi_hw_t *hw)
{
	hw->enable.ge0_rx_enable = 0;
}

static inline void qspi_ll_enable_ge0_tx(qspi_hw_t *hw)
{
	hw->enable.ge0_tx_enable = 1;
}

static inline void qspi_ll_disable_ge0_tx(qspi_hw_t *hw)
{
	hw->enable.ge0_tx_enable = 0;
}

static inline void qspi_ll_enable_vid_tx(qspi_hw_t *hw)
{
	hw->enable.vid_tx_enable = 1;
}

static inline void qspi_ll_disable_vid_tx(qspi_hw_t *hw)
{
	hw->enable.vid_tx_enable = 0;
}

static inline void qspi_ll_enable_vid_rx(qspi_hw_t *hw)
{
	hw->enable.vid_rx_enable = 1;
}

static inline void qspi_ll_disable_vid_rx(qspi_hw_t *hw)
{
	hw->enable.vid_rx_enable = 0;
}

static inline void qspi_ll_set_clk_div(qspi_hw_t *hw, uint32_t clk_div)
{
	hw->ctrl.clk_div = clk_div & QSPI_F_CLK_DIV_M;
}

static inline void qspi_ll_enable_dcache_rd_cmd(qspi_hw_t *hw)
{
	hw->dcard_cmd.dcard_cmd_en = 1;
}

static inline void qspi_ll_disable_dcache_rd_cmd(qspi_hw_t *hw)
{
	hw->dcard_cmd.dcard_cmd_en = 0;
}

static inline void qspi_ll_set_dcache_rd_cmd_bw(qspi_hw_t *hw, uint32_t dcache_rd_cmd_bw)
{
	hw->dcard_cmd.dcard_cmd_bw = dcache_rd_cmd_bw;
}

static inline void qspi_ll_set_dcache_rd_cmd(qspi_hw_t *hw, uint32_t dcache_rd_cmd)
{
	hw->dcard_cmd.dcard_cmd = dcache_rd_cmd;
}

static inline void qspi_ll_set_dcache_rd_cmd_cnt(qspi_hw_t *hw, uint32_t dcache_rd_cmd_cnt)
{
	hw->dcard_cmd.dcard_cmd_cnt = dcache_rd_cmd_cnt;
}

static inline void qspi_ll_enable_dcache_rd_addr(qspi_hw_t *hw)
{
	hw->dcard_addr.dcard_addr_en = 1;
}

static inline void qspi_ll_disable_dcache_rd_addr(qspi_hw_t *hw)
{
	hw->dcard_addr.dcard_addr_en = 0;
}

static inline void qspi_ll_set_dcache_rd_addr_bw(qspi_hw_t *hw, uint32_t dcache_rd_addr_bw)
{
	hw->dcard_addr.dcard_addr_bw = dcache_rd_addr_bw;
}

static inline void qspi_ll_set_dcache_rd_addr_cnt(qspi_hw_t *hw, uint32_t dcache_rd_addr_cnt)
{
	hw->dcard_addr.dcard_addr_cnt = dcache_rd_addr_cnt;
}

static inline void qspi_ll_enable_dcache_rd_dummy(qspi_hw_t *hw)
{
	hw->dcard_dum.dcard_dum_en = 1;
}

static inline void qspi_ll_disable_dcache_rd_dummy(qspi_hw_t *hw)
{
	hw->dcard_dum.dcard_dum_en = 0;
}

static inline void qspi_ll_set_dcache_rd_dummy_bw(qspi_hw_t *hw, uint32_t dcache_rd_dummy_bw)
{
	hw->dcard_dum.dcard_dum_bw = dcache_rd_dummy_bw;
}

static inline void qspi_ll_set_dcache_rd_dummy_cnt(qspi_hw_t *hw, uint32_t dcache_rd_dummy_cnt)
{
	hw->dcard_dum.dcard_dum_cnt = dcache_rd_dummy_cnt;
}

static inline void qspi_ll_enable_dcache_rd_data(qspi_hw_t *hw)
{
	hw->dcard_dat.dcard_dat_en = 1;
}

static inline void qspi_ll_disable_dcache_rd_data(qspi_hw_t *hw)
{
	hw->dcard_dat.dcard_dat_en = 0;
}

static inline void qspi_ll_set_dcache_rd_data_bw(qspi_hw_t *hw, uint32_t dcache_rd_data_bw)
{
	hw->dcard_dat.dcard_dat_bw = dcache_rd_data_bw;
}

static inline void qspi_ll_set_dcache_rd_data_cnt(qspi_hw_t *hw, uint32_t dcache_rd_data_cnt)
{
	hw->dcard_dat.dcard_dat_cnt = dcache_rd_data_cnt;
}

static inline void qspi_ll_set_dcache_rd_data_direction(qspi_hw_t *hw, qspi_op_t dcache_rd_data_dir)
{
	hw->dcard_dat.dcard_dat_dir = dcache_rd_data_dir;
}

static inline void qspi_ll_enable_dcache_wr_cmd(qspi_hw_t *hw)
{
	hw->dcawr_cmd.dcawr_cmd_en = 1;
}

static inline void qspi_ll_disable_dcache_wr_cmd(qspi_hw_t *hw)
{
	hw->dcawr_cmd.dcawr_cmd_en = 0;
}

static inline void qspi_ll_set_dcache_wr_cmd_bw(qspi_hw_t *hw, uint32_t dcache_wr_cmd_bw)
{
	hw->dcawr_cmd.dcawr_cmd_bw = dcache_wr_cmd_bw;
}

static inline void qspi_ll_set_dcache_wr_cmd(qspi_hw_t *hw, uint32_t dcache_wr_cmd)
{
	hw->dcawr_cmd.dcawr_cmd = dcache_wr_cmd;
}

static inline void qspi_ll_set_dcache_wr_cmd_cnt(qspi_hw_t *hw, uint32_t dcache_wr_cmd_cnt)
{
	hw->dcawr_cmd.dcawr_cmd_cnt = dcache_wr_cmd_cnt;
}

static inline void qspi_ll_enable_dcache_wr_addr(qspi_hw_t *hw)
{
	hw->dcawr_addr.dcawr_addr_en = 1;
}

static inline void qspi_ll_disable_dcache_wr_addr(qspi_hw_t *hw)
{
	hw->dcawr_addr.dcawr_addr_en = 0;
}

static inline void qspi_ll_set_dcache_wr_addr_bw(qspi_hw_t *hw, uint32_t dcache_wr_addr_bw)
{
	hw->dcawr_addr.dcawr_addr_bw = dcache_wr_addr_bw;
}

static inline void qspi_ll_set_dcache_wr_addr_cnt(qspi_hw_t *hw, uint32_t dcache_wr_addr_cnt)
{
	hw->dcawr_addr.dcawr_addr_cnt = dcache_wr_addr_cnt;
}

static inline void qspi_ll_enable_dcache_wr_dummy(qspi_hw_t *hw)
{
	hw->dcawr_dum.dcawr_dum_en = 1;
}

static inline void qspi_ll_disable_dcache_wr_dummy(qspi_hw_t *hw)
{
	hw->dcawr_dum.dcawr_dum_en = 0;
}

static inline void qspi_ll_set_dcache_wr_dummy_bw(qspi_hw_t *hw, uint32_t dcache_wr_dummy_bw)
{
	hw->dcawr_dum.dcawr_dum_bw = dcache_wr_dummy_bw;
}

static inline void qspi_ll_set_dcache_wr_dummy_cnt(qspi_hw_t *hw, uint32_t dcache_wr_dummy_cnt)
{
	hw->dcawr_dum.dcawr_dum_cnt = dcache_wr_dummy_cnt;
}

static inline void qspi_ll_enable_dcache_wr_data(qspi_hw_t *hw)
{
	hw->dcawr_dat.dcawr_dat_en = 1;
}

static inline void qspi_ll_disable_dcache_wr_data(qspi_hw_t *hw)
{
	hw->dcawr_dat.dcawr_dat_en = 0;
}

static inline void qspi_ll_set_dcache_wr_data_bw(qspi_hw_t *hw, uint32_t dcache_wr_data_bw)
{
	hw->dcawr_dat.dcawr_dat_bw = dcache_wr_data_bw;
}

static inline void qspi_ll_set_dcache_wr_data_cnt(qspi_hw_t *hw, uint32_t dcache_wr_data_cnt)
{
	hw->dcawr_dat.dcawr_dat_cnt = dcache_wr_data_cnt;
}

static inline void qspi_ll_set_dcache_wr_data_direction(qspi_hw_t *hw, qspi_op_t dcache_wr_data_dir)
{
	hw->dcawr_dat.dcawr_dat_dir = dcache_wr_data_dir;
}

static inline void qspi_ll_enable_dcache_req(qspi_hw_t *hw)
{
	hw->rw_req_msk.rw_req_msk = 1;
}

static inline void qspi_ll_disable_dcache_req(qspi_hw_t *hw)
{
	hw->rw_req_msk.rw_req_msk = 0;
}

static inline uint32_t qspi_ll_get_interrupt_status_before_mask(qspi_hw_t *hw)
{
	return hw->int_status.v;
}

static inline void qspi_ll_clear_interrupt_status(qspi_hw_t *hw, uint32_t status)
{
	hw->int_status.v = status;
}

static inline uint32_t qspi_ll_get_interrupt_status_after_mask(qspi_hw_t *hw)
{
	uint32_t mask = (QSPI_F_INT_STATUS_M << QSPI_F_INT_STATUS_S) | (QSPI_F_INT_SW_OP_M << QSPI_F_INT_SW_OP_S);
	return (hw->int_status.v) & (hw->sw_op.v >> 7) & mask;
}

static inline bool qspi_ll_is_sw_op_int_triggered(qspi_hw_t *hw, uint32_t status)
{
	return !!(status & BIT(16));
}

static inline void qspi_ll_init_dcache_wr_cmd(qspi_hw_t *hw, qspi_wire_mode_t wire_mode, uint32_t cmd)
{
	if (wire_mode == QSPI_1WIRE) {
		/* 0x2009 */
		qspi_ll_set_dcache_wr_cmd_bw(hw, 0x0);
		qspi_ll_set_dcache_wr_cmd_cnt(hw, 0x8);
	} else {
		/* 0x8e3 */
		qspi_ll_set_dcache_wr_cmd_bw(hw, 0x1);
		qspi_ll_set_dcache_wr_cmd_cnt(hw, 0x2);
	}
	qspi_ll_set_dcache_wr_cmd(hw, cmd);
	qspi_ll_enable_dcache_wr_cmd(hw);
}

static inline void qspi_ll_init_dcache_wr_addr(qspi_hw_t *hw, qspi_wire_mode_t wire_mode)
{
	if (wire_mode == QSPI_1WIRE) {
		/* 0x60000001 */
		qspi_ll_set_dcache_wr_addr_bw(hw, 0x0);
		qspi_ll_set_dcache_wr_addr_cnt(hw, 0x18);
	} else {
		/* 0x18000003 */
		qspi_ll_set_dcache_wr_addr_bw(hw, 0x1);
		qspi_ll_set_dcache_wr_addr_cnt(hw, 0x6);
	}
	qspi_ll_enable_dcache_wr_addr(hw);
}

static inline void qspi_ll_init_dcache_wr_dummy_size(qspi_hw_t *hw, qspi_wire_mode_t wire_mode, uint32_t dummy_size)
{
	if (wire_mode == QSPI_1WIRE) {
		/* 0x0 */
		qspi_ll_set_dcache_wr_dummy_bw(hw, 0x0);
	} else {
		/* 0x2 */
		qspi_ll_set_dcache_wr_dummy_bw(hw, 0x1);
	}
	if (dummy_size) {
		qspi_ll_enable_dcache_wr_dummy(hw);
	} else {
		qspi_ll_disable_dcache_wr_dummy(hw);
	}
	qspi_ll_set_dcache_wr_dummy_cnt(hw, dummy_size);
}

static inline void qspi_ll_init_dcache_wr_data(qspi_hw_t *hw, qspi_wire_mode_t wire_mode)
{
	if (wire_mode == QSPI_1WIRE) {
		/* 0x4201 */
		qspi_ll_set_dcache_wr_data_bw(hw, 0x0);
		qspi_ll_set_dcache_wr_data_cnt(hw, 0x80);
	} else {
		/* 0x4083 */
		qspi_ll_set_dcache_wr_data_bw(hw, 0x1);
		qspi_ll_set_dcache_wr_data_cnt(hw, 0x20);
	}
	qspi_ll_set_dcache_wr_data_direction(hw, QSPI_WRITE);
	qspi_ll_enable_dcache_wr_data(hw);
}

static inline void qspi_ll_init_dcache_rd_cmd(qspi_hw_t *hw, qspi_wire_mode_t wire_mode, uint32_t cmd)
{
	if (wire_mode == QSPI_1WIRE) {
		/* 0x200d */
		qspi_ll_set_dcache_rd_cmd_bw(hw, 0x0);
		qspi_ll_set_dcache_rd_cmd_cnt(hw, 0x8);
	} else {
		/* 0xbaf */
		qspi_ll_set_dcache_rd_cmd_bw(hw, 0x1);
		qspi_ll_set_dcache_rd_cmd_cnt(hw, 0x2);
	}
	qspi_ll_set_dcache_rd_cmd(hw, cmd);
	qspi_ll_enable_dcache_rd_cmd(hw);
}

static inline void qspi_ll_init_dcache_rd_addr(qspi_hw_t *hw, qspi_wire_mode_t wire_mode)
{
	if (wire_mode == QSPI_1WIRE) {
		/* 0x60000001 */
		qspi_ll_set_dcache_rd_addr_bw(hw, 0x0);
		qspi_ll_set_dcache_rd_addr_cnt(hw, 0x18);
	} else {
		/* 0x18000003 */
		qspi_ll_set_dcache_rd_addr_bw(hw, 0x1);
		qspi_ll_set_dcache_rd_addr_cnt(hw, 0x6);
	}
	qspi_ll_enable_dcache_rd_addr(hw);
}

static inline void qspi_ll_init_dcache_rd_dummy_size(qspi_hw_t *hw, qspi_wire_mode_t wire_mode, uint32_t dummy_size)
{
	if (wire_mode == QSPI_1WIRE) {
		/* 0x0 */
		qspi_ll_set_dcache_rd_dummy_bw(hw, 0x0);
	} else {
		/* 0x1b */
		qspi_ll_set_dcache_rd_dummy_bw(hw, 0x1);
	}
	if (dummy_size) {
		qspi_ll_enable_dcache_rd_dummy(hw);
	} else {
		qspi_ll_disable_dcache_rd_dummy(hw);
	}
	qspi_ll_set_dcache_rd_dummy_cnt(hw, dummy_size);
}

static inline void qspi_ll_init_dcache_rd_data(qspi_hw_t *hw, qspi_wire_mode_t wire_mode)
{
	if (wire_mode == QSPI_1WIRE) {
		/* 0x201 */
		qspi_ll_set_dcache_rd_data_bw(hw, 0x0);
		qspi_ll_set_dcache_rd_data_cnt(hw, 0x80);
	} else {
		/* 0x83 */
		qspi_ll_set_dcache_rd_data_bw(hw, 0x1);
		qspi_ll_set_dcache_rd_data_cnt(hw, 0x20);
	}
	qspi_ll_set_dcache_rd_data_direction(hw, QSPI_READ);
	qspi_ll_enable_dcache_rd_data(hw);
}

static inline void qspi_ll_init_dcache(qspi_hw_t *hw, const qspi_cmd_t *cmd)
{
	if (cmd->op == QSPI_READ) {
		qspi_ll_init_dcache_rd_cmd(hw, cmd->wire_mode, cmd->cmd);
		qspi_ll_init_dcache_rd_addr(hw, cmd->wire_mode);
		qspi_ll_init_dcache_rd_dummy_size(hw, cmd->wire_mode, cmd->dummy_cycle);
		qspi_ll_init_dcache_rd_data(hw, cmd->wire_mode);
	} else {
		qspi_ll_init_dcache_wr_cmd(hw, cmd->wire_mode, cmd->cmd);
		qspi_ll_init_dcache_wr_addr(hw, cmd->wire_mode);
		qspi_ll_init_dcache_wr_dummy_size(hw, cmd->wire_mode, cmd->dummy_cycle);
		qspi_ll_init_dcache_wr_data(hw, cmd->wire_mode);
	}
}

/* 1. disable qspi
 * 2. icu enbale psram interrupt
 * 3. set gpio
 * 4. qspi clk_div
 * 5. qspi sw cmd
 * 6. qspi sw addr
 * 7. qspi sw dummy
 * 8. qspi sw data
 * 9. qspi sw op
 * 10. qspi set ge0 dep, ge1 dep
 * 11. qspi set ge0 th, ge1 th
 * 12. qspi enable ge0/vid tx
 */
static inline void qspi_ll_init_sw_command(qspi_hw_t *hw, const qspi_cmd_t *cmd)
{
	uint8_t uc_bw = 0;
	uint8_t uc_cnt = 0;

	if (cmd->wire_mode == QSPI_1WIRE) {
		uc_bw = 0x0;
		uc_cnt = 0x8;
	} else {
		uc_bw = 0x1;
		uc_cnt = 0x2;
	}
	qspi_ll_disable_ge1_rx(hw);
	qspi_ll_disable_ge1_tx(hw);
	qspi_ll_disable_ge0_rx(hw);
	qspi_ll_disable_ge0_tx(hw);

	/* set sw cmd */
	qspi_ll_set_sw_cmd(hw, cmd->cmd);
	qspi_ll_set_sw_cmd_bw(hw, uc_bw);
	qspi_ll_set_sw_cmd_cnt(hw, uc_cnt);
	qspi_ll_enable_sw_cmd(hw);

	/* set sw addr */
	qspi_ll_set_sw_addr(hw, cmd->addr);
	qspi_ll_set_sw_addr_cnt(hw, uc_cnt * (24 / 8));
	qspi_ll_set_sw_addr_bw(hw, uc_bw);
	if (cmd->addr & BIT(31)) {
		qspi_ll_enable_sw_addr(hw);
	} else {
		qspi_ll_disable_sw_addr(hw);
	}

	/* set sw dummy */
	qspi_ll_set_sw_dummy_cnt(hw, cmd->dummy_cycle);
	qspi_ll_set_sw_dummy_bw(hw, uc_bw);
	if (cmd->dummy_cycle) {
		qspi_ll_enable_sw_dummy(hw);
	} else {
		qspi_ll_disable_sw_dummy(hw);
	}

	/* set sw data */
	qspi_ll_set_sw_data_direction(hw, cmd->op);
	qspi_ll_set_sw_data_bw(hw, uc_bw);

	/* set sw op */
	qspi_ll_set_sw_op(hw, 6 - cmd->op);

	/* set ge0 dep */
	qspi_ll_set_ge0_wr_dep(hw, QSPI_LL_GE0_DEP);
	qspi_ll_set_ge0_rd_dep(hw, QSPI_LL_GE0_DEP);

	/* set ge1 dep */
	qspi_ll_set_ge1_wr_dep(hw, QSPI_LL_GE1_DEP);
	qspi_ll_set_ge1_rd_dep(hw, QSPI_LL_GE1_DEP);

	if (cmd->op == QSPI_WRITE) {
		qspi_ll_enable_ge0_tx(hw);
		qspi_ll_enable_vid_tx(hw);
	} else {
		qspi_ll_enable_ge0_rx(hw);
		qspi_ll_enable_vid_rx(hw);
	}

	qspi_ll_enable_sw_op_int_mask(hw);
	qspi_ll_start_sw_op(hw);
}

static inline void qspi_ll_init_command(qspi_hw_t *hw, const qspi_cmd_t *cmd)
{
	if (cmd->work_mode == INDIRECT_MODE) {
		qspi_ll_init_sw_command(hw, cmd);
	} else {
		qspi_ll_init_dcache(hw, cmd);
		/* must enable, else dcache mode will not work */
		qspi_ll_enable_dcache_req(hw);
	}
}

static inline void qspi_ll_direct_write(uint32_t base_addr, const void *data, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++) {
		*((volatile uint8_t *)(QSPI_DCACHE_BASE_ADDR + base_addr + i)) = ((uint8_t *)data)[i];
	}
}

static inline void qspi_ll_direct_read(uint32_t base_addr, void *data, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++) {
		((uint8_t *)data)[i] = *((volatile uint8_t *)(QSPI_DCACHE_BASE_ADDR + base_addr + i));
	}
}

#define qspi_ll_io_write(hw, data, size)
#define qspi_ll_io_read(hw, data, size)

static inline void qspi_ll_set_voltage(qspi_voltage_level_t vol_level)
{
	//TODO optimize it when new sys_ctrl finished
	uint32_t vol  = (uint32_t)vol_level;
	//sddev_control(DD_DEV_TYPE_SCTRL, CMD_QSPI_IO_VOLTAGE, &vol);
	sys_hal_set_qspi_io_voltage(vol);
	//sddev_control(DD_DEV_TYPE_SCTRL, CMD_QSPI_VDDRAM_VOLTAGE, &vol);
	sys_hal_set_qspi_vddram_voltage(vol);
}

static inline void qspi_ll_init_common(qspi_hw_t *hw)
{
	qspi_ll_set_voltage(QSPI_VOL_3_3V);
	uint32_t param = BLK_BIT_MIC_QSPI_RAM_OR_FLASH;
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_BLK_ENABLE, &param);
}

static inline void qspi_ll_deinit_common(qspi_hw_t *hw)
{
	uint32_t param = ~BLK_BIT_MIC_QSPI_RAM_OR_FLASH;
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_BLK_ENABLE, &param);
}

#ifdef __cplusplus
}
#endif

