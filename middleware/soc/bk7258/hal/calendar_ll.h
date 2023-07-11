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
#include "calendar_hw.h"
#include <driver/hal/hal_calendar_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CALENDAR_LL_REG_BASE(_calendar_unit_id)    (SOC_CALENDAR_REG_BASE)

static inline void calendar_ll_init(calendar_hw_t *hw)
{
	hw->init_val = 0;
}

static inline void calendar_ll_enable(calendar_hw_t *hw)
{
	hw->enable.en = 1;
}

static inline void calendar_ll_disable(calendar_hw_t *hw)
{
	hw->enable.en = 0;
}

static inline void calendar_ll_set_init_value(calendar_hw_t *hw, uint32_t init_val)
{
	hw->init_val = init_val;
}

static inline void calendar_ll_reset_config_to_default(calendar_hw_t *hw)
{
	hw->init_val = 0;
}

static inline uint32_t calendar_ll_get_second_cnt(calendar_hw_t *hw)
{
	return hw->cnt_val_s;
}

static inline uint32_t calendar_ll_get_us_cnt(calendar_hw_t *hw)
{
	return hw->cnt_val_us.cnt_31_25_us_val;
}

#ifdef __cplusplus
}
#endif


