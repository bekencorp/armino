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
#include "flash_hw.h"
#include <driver/hal/hal_flash_types.h>
#include "icu_hw.h"
#include "icu_ll.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FLASH_LL_REG_BASE(_flash_unit_id)    (SOC_FLASH_REG_BASE)

static inline void flash_ll_init(flash_hw_t *hw)
{

}

static inline bool flash_ll_is_busy(flash_hw_t *hw)
{
	return hw->op_sw.busy_sw;
}

static inline uint32_t flash_ll_read_flash_id(flash_hw_t *hw)
{
	return hw->rd_flash_id;
}

static inline void flash_ll_set_op_cmd(flash_hw_t *hw, flash_op_cmd_t cmd)
{
	hw->op_sw.op_type_sw = cmd;
	hw->op_sw.op_sw = 1;
	hw->op_sw.wp_value = 1;
}

static inline uint32_t flash_ll_get_id(flash_hw_t *hw)
{
	flash_ll_set_op_cmd(hw, FLASH_OP_CMD_RDID);
	while (flash_ll_is_busy(hw));
	return flash_ll_read_flash_id(hw);
}

static inline uint32_t flash_ll_get_mid(flash_hw_t *hw)
{
	while (flash_ll_is_busy(hw));
	hw->op_sw.op_type_sw = FLASH_OP_CMD_RDID;
	hw->op_sw.op_sw = 1;
	while (flash_ll_is_busy(hw));
	return flash_ll_read_flash_id(hw);
}

static inline void flash_ll_write_status_reg(flash_hw_t *hw, uint8_t sr_width, uint16_t sr_data)
{
	while (flash_ll_is_busy(hw));
	hw->config.wrsr_data = sr_data;
	while (flash_ll_is_busy(hw));

	if (sr_width == 1) {
		flash_ll_set_op_cmd(hw, FLASH_OP_CMD_WRSR);
	} else if (sr_width == 2) {
		flash_ll_set_op_cmd(hw, FLASH_OP_CMD_WRSR2);
	}
	while (flash_ll_is_busy(hw));
}

static inline void flash_ll_set_qe(flash_hw_t *hw, uint8_t qe_bit, uint8_t qe_bit_post)
{
	hw->config.v |= qe_bit << qe_bit_post;
}

static inline uint16_t flash_ll_read_status_reg(flash_hw_t *hw, uint8_t sr_width)
{
	uint16_t state_reg_data = 0;

	while (flash_ll_is_busy(hw));
	flash_ll_set_op_cmd(hw, FLASH_OP_CMD_RDSR);
	while (flash_ll_is_busy(hw));

	state_reg_data = hw->state.status_reg;

	if (sr_width == 2) {
		flash_ll_set_op_cmd(hw, FLASH_OP_CMD_RDSR2);
		while (flash_ll_is_busy(hw));
		state_reg_data |= hw->state.status_reg << 8;
	}
	return state_reg_data;
}

static inline void flash_ll_enable_cpu_data_wr(flash_hw_t *hw)
{
	hw->config.cpu_data_wr_en = 1;
}

static inline void flash_ll_disable_cpu_data_wr(flash_hw_t *hw)
{
	hw->config.cpu_data_wr_en = 0;
}

static inline void flash_ll_clear_qwfr(flash_hw_t *hw)
{
	hw->config.mode_sel = 0;
	hw->op_sw.addr_sw_reg = 0;
	flash_ll_set_op_cmd(hw, FLASH_OP_CMD_CRMR);
	while (flash_ll_is_busy(hw));
}

static inline void flash_ll_set_mode(flash_hw_t *hw, uint8_t mode_sel)
{
	hw->config.mode_sel = mode_sel;
}

static inline void flash_ll_set_dual_mode(flash_hw_t *hw)
{
	hw->config.mode_sel = FLASH_MODE_DUAL;
}

static inline void flash_ll_set_quad_m_value(flash_hw_t *hw, uint32_t m_value)
{

}

static inline void flash_ll_erase_sector(flash_hw_t *hw, uint32_t erase_addr)
{
	while (flash_ll_is_busy(hw));
	hw->op_sw.addr_sw_reg = erase_addr;
	hw->op_sw.op_type_sw = FLASH_OP_CMD_SE;
	hw->op_sw.op_sw = 1;
	while (flash_ll_is_busy(hw));
}

static inline void flash_ll_set_op_cmd_read(flash_hw_t *hw, uint32_t read_addr)
{
	hw->op_sw.addr_sw_reg = read_addr;
	hw->op_sw.op_type_sw = FLASH_OP_CMD_READ;
	hw->op_sw.op_sw = 1;
	while (flash_ll_is_busy(hw));
}

static inline uint32_t flash_ll_read_data(flash_hw_t *hw)
{
	return hw->data_flash_sw;
}

static inline void flash_ll_set_op_cmd_write(flash_hw_t *hw, uint32_t write_addr)
{
	hw->op_sw.addr_sw_reg = write_addr;
	hw->op_sw.op_type_sw = FLASH_OP_CMD_PP;
	hw->op_sw.op_sw = 1;
	while (flash_ll_is_busy(hw));
}

static inline void flash_ll_write_data(flash_hw_t *hw, uint32_t data)
{
	hw->data_sw_flash = data;
}

static inline void flash_ll_set_clk(flash_hw_t *hw, uint8_t clk_cfg)
{
	hw->config.clk_cfg = clk_cfg;

#if CONFIG_JTAG
	hw->config.crc_en = 0;
#endif
}

static inline void flash_ll_set_default_clk(flash_hw_t *hw)
{
	flash_ll_set_clk(hw, 0x5);
}

static inline void flash_ll_set_clk_dpll(flash_hw_t *hw)
{
	hw->config.clk_cfg = 5;
}

static inline void flash_ll_set_clk_dco(flash_hw_t *hw, bool ate_enabled)
{
	if (ate_enabled) {
		hw->config.clk_cfg = 0xB;
	} else {
		hw->config.clk_cfg = 0x9;
	}
}

static inline void flash_ll_write_enable(flash_hw_t *hw)
{
	flash_ll_set_op_cmd(hw, FLASH_OP_CMD_WREN);
	while (flash_ll_is_busy(hw));
}

static inline void flash_ll_write_disable(flash_hw_t *hw)
{
	flash_ll_set_op_cmd(hw, FLASH_OP_CMD_WRDI);
	while (flash_ll_is_busy(hw));
}

#ifdef __cplusplus
}
#endif


