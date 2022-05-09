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

#include "hal_config.h"
#include "flash_hal.h"
#include "flash_hw.h"
#include "flash_ll.h"

#if CFG_HAL_DEBUG_FLASH

void flash_struct_dump(void)
{
	flash_hw_t *hw = (flash_hw_t *)FLASH_LL_REG_BASE(0);
	SOC_LOGI("base=%x\r\n", (uint32_t)hw);

	SOC_LOGI("  op_sw=0x%x value=0x%x\n", &hw->op_sw, hw->op_sw.v);
	SOC_LOGI("    addr_sw_reg: %x\n", hw->op_sw.addr_sw_reg);
	SOC_LOGI("    op_type_sw:  %x\n", hw->op_sw.op_type_sw);
	SOC_LOGI("    op_sw:       %x\n", hw->op_sw.op_sw);
	SOC_LOGI("    wp_value:    %x\n", hw->op_sw.wp_value);
	SOC_LOGI("    busy_sw:     %x\n", hw->op_sw.busy_sw);

	SOC_LOGI("  data_sw_flash addr=0x%x value=0x%x\r\n", &hw->data_sw_flash, hw->data_sw_flash);
	SOC_LOGI("\r\n");

	SOC_LOGI("  data_flash_sw addr=0x%x value=0x%x\r\n", &hw->data_flash_sw, hw->data_flash_sw);
	SOC_LOGI("\r\n");

	SOC_LOGI("  reserved0 addr=0x%x value=0x%x\r\n", &hw->reserved0, hw->reserved0);
	SOC_LOGI("\r\n");

	SOC_LOGI("  rd_flash_id addr=0x%x value=0x%x\r\n", &hw->rd_flash_id, hw->rd_flash_id);
	SOC_LOGI("\r\n");

	SOC_LOGI("  state=0x%x value=0x%x\n", &hw->state, hw->state.v);
	SOC_LOGI("    state:       %x\n", hw->state.status_reg);
	SOC_LOGI("    crc_err_num: %x\n", hw->state.crc_err_num);
	SOC_LOGI("    byte_sel_wr: %x\n", hw->state.byte_sel_wr);
	SOC_LOGI("    byte_sel_rd: %x\n", hw->state.byte_sel_rd);
	SOC_LOGI("\r\n");

	SOC_LOGI("  reserved1 addr=0x%x value=0x%x\r\n", &hw->reserved1, hw->reserved1);
	SOC_LOGI("\r\n");

	SOC_LOGI("  config=0x%x value=0x%x\n", &hw->config, hw->config.v);
	SOC_LOGI("    clk_cfg:        %x\n", hw->config.clk_cfg);
	SOC_LOGI("    mode_sel:       %x\n", hw->config.mode_sel);
	SOC_LOGI("    cpu_data_wr_en: %x\n", hw->config.cpu_data_wr_en);
	SOC_LOGI("    wrsr_data:      %x\n", hw->config.wrsr_data);
	SOC_LOGI("    crc_en:         %x\n", hw->config.crc_en);
	SOC_LOGI("    reserved:       %x\n", hw->config.reserved);
	SOC_LOGI("\r\n");
}

#endif

