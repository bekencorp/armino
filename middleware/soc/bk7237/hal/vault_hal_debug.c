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
#include "vault_hw.h"
#include "vault_hal.h"
#include "vault_ll.h"

#if CFG_HAL_DEBUG_VAULT

void vault_struct_dump(void)
{
	vault_hw_t *hw = (vault_hw_t *)VAULT_LL_REG_BASE(0);
	SOC_LOGI("vault base=%x\r\n", (uint32_t)hw);

	SOC_LOGI("\n");
	SOC_LOGI("  debug addr=0x%x value=0x%x\r\n", &hw->debug, hw->debug.v);

	SOC_LOGI("\n");
	SOC_LOGI("  ctrl addr=0x%x value=0x%x\r\n", &hw->ctrl, hw->ctrl.v);
	SOC_LOGI("    abort_req:		%x\r\n", hw->ctrl.abort_reg);
	SOC_LOGI("    big_end:			%x\r\n", hw->ctrl.big_end);
	SOC_LOGI("    clk_man_reset_n:	%x\r\n", hw->ctrl.clk_man_reset_n);
	SOC_LOGI("    cm_reset_n:		%x\r\n", hw->ctrl.cm_reset_n);
	SOC_LOGI("    ctr_reset_n:		%x\r\n", hw->ctrl.ctr_reset_n);
	SOC_LOGI("    slv_reset_n:		%x\r\n", hw->ctrl.slv_reset_n);
	SOC_LOGI("    soft_reset:		%x\r\n", hw->ctrl.soft_reset);
	SOC_LOGI("    pos_disable:		%x\r\n", hw->ctrl.pos_disable);
	SOC_LOGI("    power_mode_to_vault:	%x\r\n", hw->ctrl.power_mode_to_vault);
	SOC_LOGI("    tst_fro_ctrl_en:		%x\r\n", hw->ctrl.tst_fro_ctrl_en);
	SOC_LOGI("    tst_fro_delay:		%x\r\n", hw->ctrl.tst_fro_delay);
	SOC_LOGI("    tst_fro_enable:		%x\r\n", hw->ctrl.tst_fro_enable);
	SOC_LOGI("    tst_fro_iddq:			%x\r\n", hw->ctrl.tst_fro_iddq);
	SOC_LOGI("    tst_fro_select:		%x\r\n", hw->ctrl.tst_fro_select);
	SOC_LOGI("    Vault_reset_n:		%x\r\n", hw->ctrl.Vault_reset_n);

	SOC_LOGI("\n");
	SOC_LOGI("  status addr=0x%x value=0x%x\r\n", &hw->status, hw->data.v);
	SOC_LOGI("    debug_ctrl:		%x\r\n", hw->status.debug_ctrl);
	SOC_LOGI("    abort_ack:		%x\r\n", hw->status.abort_ack);
	SOC_LOGI("    cm_clk_busy:		%x\r\n", hw->status.cm_clk_busy);
	SOC_LOGI("    ctr_clk_busy:		%x\r\n", hw->status.ctr_clk_busy);
	SOC_LOGI("    slv_clk_busy:		%x\r\n", hw->status.slv_clk_busy);
	SOC_LOGI("    fips_mode:		%x\r\n", hw->status.fips_mode);
	SOC_LOGI("    nonfips_mode:		%x\r\n", hw->status.nonfips_mode);
	SOC_LOGI("    fatal_error:		%x\r\n", hw->status.power_mode_write);
	SOC_LOGI("    power_mode_write:	%x\r\n", hw->status.ctr_clk_busy);
	SOC_LOGI("    power_mode:		%x\r\n", hw->status.power_mode);
}

#endif
