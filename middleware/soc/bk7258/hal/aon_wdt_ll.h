// Copyright 2021-2022 Beken
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
#include <common/bk_err.h>

#include "aon_wdt_reg.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AON_WDT_LL_REG_BASE(_aon_wdt_unit_id)    (SOC_AON_WDT_REG_BASE)
#define AON_WDT_CONFIG_WD_KEY_POS (0)
#define AON_WDT_CONFIG_WD_KEY_MASK (0xffff)

#define AON_WDT_CONFIG_WD_PERIOD_POS (16)
#define AON_WDT_CONFIG_WD_PERIOD_MASK (0xff)

static inline uint32_t aon_wdt_ll_get_wd_key(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_WDT_R_CTRL);
    reg_value = ((reg_value >> AON_WDT_CONFIG_WD_KEY_POS) & AON_WDT_CONFIG_WD_KEY_MASK);
    return reg_value;
}

static inline uint32_t aon_wdt_ll_get_wd_period(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_WDT_R_CTRL);
    reg_value = ((reg_value >> AON_WDT_CONFIG_WD_PERIOD_POS) & AON_WDT_CONFIG_WD_PERIOD_MASK);
    return reg_value;
}

static inline void aon_wdt_ll_set_period(uint32_t period)
{
	uint32_t ctrl_val = (period & AON_WDT_F_PERIOD_M) | (AON_WDT_V_KEY_1ST << AON_WDT_F_KEY_S);
	REG_WRITE(AON_WDT_R_CTRL, ctrl_val);

	ctrl_val = (period & AON_WDT_F_PERIOD_M) | (AON_WDT_V_KEY_2ND << AON_WDT_F_KEY_S);
	REG_WRITE(AON_WDT_R_CTRL, ctrl_val);
}

#ifdef __cplusplus
}
#endif