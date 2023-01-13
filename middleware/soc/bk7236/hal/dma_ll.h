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
#include "dma_hw.h"
#include <driver/hal/hal_dma_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DMA_LL_REG_BASE(_dma_unit_id) (SOC_GENER_DMA_REG_BASE)
#define CASE_DEV(dev) case DMA_DEV_##dev: return DMA_V_REQ_MUX_##dev
#define CASE_D() default: return 0

static inline void dma_ll_init(dma_hw_t *hw)
{
	int dma_id;

	hw->prio_mode.v = 0;
	hw->secure_attr.v = 0xFFF;
	hw->privileged_attr.v = 0xFFF;
	for (dma_id = 0; dma_id < SOC_DMA_CHAN_NUM_PER_UNIT; dma_id++) {
		hw->config_group[dma_id].ctrl.v = 0;
		hw->config_group[dma_id].dest_start_addr = 0;
		hw->config_group[dma_id].src_start_addr = 0;
		hw->config_group[dma_id].dest_loop_end_addr = 0;
		hw->config_group[dma_id].dest_loop_start_addr = 0;
		hw->config_group[dma_id].src_loop_end_addr = 0;
		hw->config_group[dma_id].src_loop_start_addr = 0;
		hw->config_group[dma_id].req_mux.v = 0;
		hw->config_group[dma_id].src_pause_addr = 0;
		hw->config_group[dma_id].dest_pause_addr = 0;
		hw->config_group[dma_id].status.v = 0;
	}
}

static inline uint32_t dma_ll_dev_to_req_mux(uint32 req_mux)
{
	switch (req_mux) {
		CASE_DEV(DTCM);
		CASE_DEV(AHB_MEM);
		CASE_DEV(UART1);
		CASE_DEV(UART1_RX);
		CASE_DEV(UART2);
		CASE_DEV(UART2_RX);
		CASE_DEV(UART3);
		CASE_DEV(UART3_RX);
		CASE_DEV(GSPI0);
		CASE_DEV(GSPI0_RX);
		CASE_DEV(GSPI1);
		CASE_DEV(GSPI1_RX);
		CASE_DEV(SDIO);
		CASE_DEV(SDIO_RX);
		CASE_DEV(AUDIO);
		CASE_DEV(AUDIO_RX);
		CASE_DEV(I2S);
		CASE_DEV(I2S_CH1);
		CASE_DEV(I2S_CH2);
		CASE_DEV(I2S_RX);
		CASE_DEV(I2S_RX_CH1);
		CASE_DEV(I2S_RX_CH2);
		CASE_DEV(I2S1);
		CASE_DEV(I2S1_RX);
		CASE_DEV(I2S2);
		CASE_DEV(I2S2_RX);
		CASE_DEV(JPEG);
		CASE_DEV(DISP_RX);
		CASE_DEV(SDMADC_RX);
		CASE_DEV(H264);
		CASE_D();
	}
}

static inline void dma_ll_enable(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.enable = 1;
}

static inline void dma_ll_disable(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.enable = 0;
}

static inline uint32_t dma_ll_get_enable_status(dma_hw_t *hw, dma_id_t id)
{
	return hw->config_group[id].ctrl.enable;
}

static inline bool dma_ll_is_id_started(dma_hw_t *hw, dma_id_t id)
{
	return !!(hw->config_group[id].ctrl.enable == 1);
}

static inline void dma_ll_enable_finish_interrupt(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.finish_int_en = 1;
}

static inline void dma_ll_disable_finish_interrupt(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.finish_int_en = 0;
}

static inline void dma_ll_enable_half_finish_interrupt(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.half_finish_int_en = 1;
}

static inline void dma_ll_disable_half_finish_interrupt(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.half_finish_int_en = 0;
}

static inline void dma_ll_enable_interrupt(dma_hw_t *hw, dma_id_t id)
{
	dma_ll_enable_half_finish_interrupt(hw, id);
	dma_ll_enable_finish_interrupt(hw, id);
}

static inline void dma_ll_disable_interrupt(dma_hw_t *hw, dma_id_t id)
{
	dma_ll_disable_half_finish_interrupt(hw, id);
	dma_ll_disable_finish_interrupt(hw, id);
}

static inline void dma_ll_clear_finish_interrupt_status(dma_hw_t *hw, dma_id_t id)
{
	/*other interrupt bit also wirte 1 to clear, so should not effect other bits*/
	hw->config_group[id].status.v = BIT(DMA_FINISH_INT_POS);
}

static inline void dma_ll_clear_half_finish_interrupt_status(dma_hw_t *hw, dma_id_t id)
{
	/*other interrupt bit also wirte 1 to clear, so should not effect other bits*/
	hw->config_group[id].status.v = BIT(DMA_HALF_FINISH_INT_POS);
}

static inline void dma_ll_clear_interrupt_status(dma_hw_t *hw, dma_id_t id)
{
	dma_ll_clear_half_finish_interrupt_status(hw, id);
	dma_ll_clear_finish_interrupt_status(hw, id);
}

static inline bool dma_ll_is_finish_interrupt_triggered(dma_hw_t *hw, dma_id_t id)
{
	return !!(hw->config_group[id].status.finish_int & 0x1);
}

static inline bool dma_ll_is_half_finish_interrupt_triggered(dma_hw_t *hw, dma_id_t id)
{
	return !!(hw->config_group[id].status.half_finish_int & 0x1);
}

static inline void dma_ll_reset_config_to_default(dma_hw_t *hw, volatile dma_id_t id)
{
	hw->config_group[id].ctrl.v = 0;
	hw->config_group[id].dest_start_addr = 0;
	hw->config_group[id].src_start_addr = 0;
	hw->config_group[id].dest_loop_end_addr = 0;
	hw->config_group[id].dest_loop_start_addr = 0;
	hw->config_group[id].src_loop_end_addr = 0;
	hw->config_group[id].src_loop_start_addr = 0;
	hw->config_group[id].req_mux.v = 0;
	hw->config_group[id].src_pause_addr = 0;
	hw->config_group[id].dest_pause_addr = 0;
}

static inline void dma_ll_set_work_mode(dma_hw_t *hw, dma_id_t id, uint32_t mode)
{
	hw->config_group[id].ctrl.mode = mode & 0x01;
}

#define dma_ll_set_mode_single(hw, id) dma_ll_set_work_mode(hw, id, DMA_V_WORK_MODE_SINGLE)
#define dma_ll_set_mode_repeat(hw, id) dma_ll_set_work_mode(hw, id, DMA_V_WORK_MODE_REPEAT)

static inline void dma_ll_set_chan_prio(dma_hw_t *hw, dma_id_t id, uint32_t chan_prio)
{
	hw->config_group[id].ctrl.chan_prio = chan_prio & 0x07;
}

static inline void dma_ll_set_dest_data_width(dma_hw_t *hw, dma_id_t id, uint32_t data_width)
{
	hw->config_group[id].ctrl.dest_data_width = data_width & 0x03;
}

static inline void dma_ll_set_src_data_width(dma_hw_t *hw, dma_id_t id, uint32_t data_width)
{
	hw->config_group[id].ctrl.src_data_width = data_width & 0x03;
}

static inline void dma_ll_set_dest_req_mux(dma_hw_t *hw, dma_id_t id, uint32_t req_mux)
{
	hw->config_group[id].req_mux.dest_req_mux = (dma_ll_dev_to_req_mux(req_mux)) & 0x1f;
}

static inline void dma_ll_set_src_req_mux(dma_hw_t *hw, dma_id_t id, uint32_t req_mux)
{
	hw->config_group[id].req_mux.src_req_mux = (dma_ll_dev_to_req_mux(req_mux)) & 0x1f;
}

static inline void dma_ll_enable_src_addr_inc(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.src_addr_inc_en = 1;
}

static inline void dma_ll_disable_src_addr_inc(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.src_addr_inc_en = 0;
}

static inline void dma_ll_enable_dest_addr_inc(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.dest_addr_inc_en = 1;
}

static inline void dma_ll_disable_dest_addr_inc(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.dest_addr_inc_en = 0;
}

static inline void dma_ll_enable_src_addr_loop(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.src_addr_loop_en = 1;
}

static inline void dma_ll_disable_src_addr_loop(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.src_addr_loop_en = 0;
}

static inline void dma_ll_enable_dest_addr_loop(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.dest_addr_loop_en = 1;
}

static inline void dma_ll_disable_dest_addr_loop(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.dest_addr_loop_en = 0;
}

static inline void dma_ll_set_dest_start_addr(dma_hw_t *hw, volatile dma_id_t id, uint32_t addr)
{
	hw->config_group[id].dest_start_addr = addr;
}

static inline void dma_ll_set_src_start_addr(dma_hw_t *hw, dma_id_t id, uint32_t addr)
{
	hw->config_group[id].src_start_addr = addr;
}

static inline void dma_ll_set_dest_loop_addr(dma_hw_t *hw, dma_id_t id, uint32_t start_addr, uint32_t end_addr)
{
	hw->config_group[id].dest_loop_start_addr = start_addr;
	hw->config_group[id].dest_loop_end_addr = end_addr;
}

static inline void dma_ll_set_src_loop_addr(dma_hw_t *hw, dma_id_t id, uint32_t start_addr, uint32_t end_addr)
{
	hw->config_group[id].src_loop_start_addr = start_addr;
	hw->config_group[id].src_loop_end_addr = end_addr;
}

static inline void dma_ll_set_transfer_len(dma_hw_t *hw, dma_id_t id, uint32_t tran_len)
{
	hw->config_group[id].ctrl.transfer_len = (tran_len - 1) & 0xffff;
}

static inline uint32_t dma_ll_get_remain_len(dma_hw_t *hw, dma_id_t id)
{
	return (uint32_t)hw->config_group[id].status.remain_len;
}

static inline void dma_ll_set_prio_mode(dma_hw_t *hw, uint32_t prio_mode)
{
	hw->prio_mode.prio_mode = prio_mode & 0x1;
}

#define dma_ll_set_prio_mode_round_robin(hw) dma_ll_set_prio_mode(hw, DMA_V_PRIO_MODE_ROUND_ROBIN)
#define dma_ll_set_prio_mode_fixed_prio(hw) dma_ll_set_prio_mode(hw, DMA_V_PRIO_MODE_FIXED_PRIO)

static inline void dma_ll_set_secure_attr(dma_hw_t *hw, dma_id_t id, dma_sec_attr_t attr)
{
	if (attr == DMA_ATTR_NON_SEC) {
		hw->secure_attr.attr &= ~(BIT(id) & 0xFFF);
	} else if (attr == DMA_ATTR_SEC) {
		hw->secure_attr.attr |= (BIT(id) & 0xFFF);
	}
}

static inline void dma_ll_set_privileged_attr(dma_hw_t *hw, dma_id_t id, dma_sec_attr_t attr)
{
	if (attr == DMA_ATTR_NON_SEC) {
		hw->privileged_attr.attr &= ~(BIT(id) & 0xFFF);
	} else if (attr == DMA_ATTR_SEC) {
		hw->privileged_attr.attr |= (BIT(id) & 0xFFF);
	}
}

static inline void dma_ll_set_src_pause_addr(dma_hw_t *hw, dma_id_t id, uint32_t addr)
{
	hw->config_group[id].src_pause_addr = addr;
}

static inline void dma_ll_set_dest_pause_addr(dma_hw_t *hw, dma_id_t id, uint32_t addr)
{
	hw->config_group[id].dest_pause_addr = addr;
}

static inline uint32_t dma_ll_get_src_read_addr(dma_hw_t *hw, dma_id_t id)
{
	return hw->config_group[id].src_rd_addr;
}

static inline uint32_t dma_ll_get_dest_write_addr(dma_hw_t *hw, dma_id_t id)
{
	return hw->config_group[id].dest_wr_addr;
}

static inline void dma_ll_set_dest_sec_attr(dma_hw_t *hw, dma_id_t id, uint32_t attr)
{
	hw->config_group[id].req_mux.dest_sec_attr = attr & 0x1;
}

static inline void dma_ll_set_src_sec_attr(dma_hw_t *hw, dma_id_t id, uint32_t attr)
{
	hw->config_group[id].req_mux.src_sec_attr = attr & 0x1;
}

#ifdef __cplusplus
}
#endif

