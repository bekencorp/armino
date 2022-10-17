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
#include "vault_hw.h"
#include <driver/hal/hal_vault_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VAULT_LL_REG_BASE(_vault_unit_id)    (SOC_VAULT_REG_BASE)

static inline void vault_ll_init(vault_hw_t *hw)
{

}

static inline void vault_ll_abort_req(vault_hw_t *hw)
{
	hw->ctrl.abort_req = 1;
}

static inline void vault_ll_little_endian(vault_hw_t *hw)
{
	hw->ctrl.big_end = 0;
}

static inline void vault_ll_big_endian(vault_hw_t *hw)
{
	hw->ctrl.big_end = 1;
}

static inline void vault_ll_clk_man_reset(vault_hw_t *hw)
{
	hw->ctrl.clk_man_reset_n = 1;
}

static inline void vault_ll_cm_reset(vault_hw_t *hw)
{
	hw->ctrl.cm_reset_n = 1;
}

static inline void vault_ll_ctr_reset(vault_hw_t *hw)
{
	hw->ctrl.ctr_reset_n = 1;
}

static inline void vault_ll_slv_reset(vault_hw_t *hw)
{
	hw->ctrl.slv_reset_n = 1;
}

static inline void vault_ll_soft_reset_enable(vault_hw_t *hw)
{
	hw->ctrl.soft_reset = 1;
}

static inline void vault_ll_soft_reset_disable(vault_hw_t *hw)
{
	hw->ctrl.soft_reset = 0;
}

static inline uint8_t vault_ll_get_debug_ctrl_status(vault_hw_t *hw)
{
	return hw->status.debug_ctrl;
}

static inline bool vault_ll_get_abort_ack_status(vault_hw_t *hw)
{
	return !!(hw->status.abort_ack);
}

static inline bool vault_ll_get_cm_clk_busy_status(vault_hw_t *hw)
{
	return !!(hw->status.cm_clk_busy);
}

static inline bool vault_ll_get_ctr_clk_busy_status(vault_hw_t *hw)
{
	return !!(hw->status.ctr_clk_busy);
}

static inline bool vault_ll_get_slv_clk_busy_status(vault_hw_t *hw)
{
	return !!(hw->status.slv_clk_busy);
}

static inline bool vault_ll_get_fips_mode_status(vault_hw_t *hw)
{
	return hw->status.fips_mode;
}

static inline bool vault_ll_get_nonfips_mode_status(vault_hw_t *hw)
{
	return !!(hw->status.nonfips_mode);
}

static inline bool vault_ll_get_fatal_error_status(vault_hw_t *hw)
{
	return !!(hw->status.fatal_error);
}

static inline bool vault_ll_get_power_mode_write_status(vault_hw_t *hw)
{
	return !!(hw->status.power_mode_write);
}

static inline bool vault_ll_get_power_mode_out(vault_hw_t *hw)
{
	return !!(hw->status.power_mode);
}

static inline void vault_ll_set_OTP_power(vault_hw_t *hw)
{
	hw->OTP_power.enable_power = 1;
}

static inline void vault_ll_clr_OTP_power(vault_hw_t *hw)
{
	hw->OTP_power.enable_power = 0;
}

#ifdef __cplusplus
}
#endif

