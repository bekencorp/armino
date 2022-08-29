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
#include "aon_wdt_hw.h"
#include <driver/hal/hal_aon_wdt_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AON_WDT_LL_REG_BASE(_aon_wdt_unit_id)    (SOC_AON_WDT_REG_BASE)

static inline void aon_wdt_ll_reset_config_to_default(aon_wdt_hw_t *hw)
{
	hw->ctrl.key = 0;
	hw->ctrl.period = AON_WDT_V_PERIOD_DEFAULT_VALUE;
}

static inline void aon_wdt_ll_init(aon_wdt_hw_t *hw)
{
	aon_wdt_ll_reset_config_to_default(hw);
}

static inline void aon_wdt_ll_set_1st_key(aon_wdt_hw_t *hw)
{
	hw->ctrl.key = AON_WDT_V_KEY_1ST;
}

static inline void aon_wdt_ll_set_2nd_key(aon_wdt_hw_t *hw)
{
	hw->ctrl.key = AON_WDT_V_KEY_2ND;
}

static inline void aon_wdt_ll_set_period(aon_wdt_hw_t *hw, uint32_t period)
{
	uint32_t ctrl_val = (period & AON_WDT_F_PERIOD_M) | (AON_WDT_V_KEY_1ST << AON_WDT_F_KEY_S);
	REG_WRITE(AON_WDT_R_CTRL, ctrl_val);

	ctrl_val = (period & AON_WDT_F_PERIOD_M) | (AON_WDT_V_KEY_2ND << AON_WDT_F_KEY_S);
	REG_WRITE(AON_WDT_R_CTRL, ctrl_val);
}

#ifdef __cplusplus
}
#endif¸¼æ}