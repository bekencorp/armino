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

#include "aon_pmu_hal.h"
#include "aon_pmu_ll.h"

static aon_pmu_hal_t s_aon_pmu_hal = {0};

bk_err_t aon_pmu_hal_init(void)
{
	s_aon_pmu_hal.hw = (aon_pmu_hw_t *)AON_PMU_LL_REG_BASE(0);
	return BK_OK;
}

int aon_pmu_hal_set_sleep_parameters(uint32_t value)
{
	aon_pmu_ll_set_reg40_value(s_aon_pmu_hal.hw, value);
	return 0;
}

void aon_pmu_hal_set_wakeup_source_reg(uint32_t value)
{
	aon_pmu_ll_set_reg41_value(s_aon_pmu_hal.hw, value);
}

uint32_t aon_pmu_hal_get_wakeup_source_reg(void)
{
	return aon_pmu_ll_get_reg41_value(s_aon_pmu_hal.hw);
}

void aon_pmu_hal_clear_wakeup_source(wakeup_source_t value)
{
	uint32_t wakeup_source = 0;
	wakeup_source = aon_pmu_ll_get_reg41_wakeup_ena(s_aon_pmu_hal.hw);
	wakeup_source &= ~(0x1 << value);
	aon_pmu_ll_set_reg41_wakeup_ena(s_aon_pmu_hal.hw, wakeup_source);
}

void aon_pmu_hal_set_wakeup_source(wakeup_source_t value)
{
	uint32_t wakeup_source = 0;
	wakeup_source = aon_pmu_ll_get_reg41_wakeup_ena(s_aon_pmu_hal.hw);
	wakeup_source |= (0x1 << value);
	aon_pmu_ll_set_reg41_wakeup_ena(s_aon_pmu_hal.hw, wakeup_source);
}

void aon_pmu_hal_usbplug_int_en(uint32_t value)
{
	uint32_t int_state = 0;
	int_state = aon_pmu_ll_get_reg1_usbplug_int_en(s_aon_pmu_hal.hw);
	int_state |= value;
	aon_pmu_ll_set_reg1_usbplug_int_en(s_aon_pmu_hal.hw, int_state);
}

void aon_pmu_hal_touch_int_en(uint32_t value)
{
	uint32_t int_state = 0;
	int_state = aon_pmu_ll_get_reg1_touch_int_en(s_aon_pmu_hal.hw);
	int_state |= value;
	aon_pmu_ll_set_reg1_touch_int_en(s_aon_pmu_hal.hw, int_state);
}

uint32_t aon_pmu_hal_get_adc_cal()
{
	return aon_pmu_ll_get_reg7d_adc_cal(s_aon_pmu_hal.hw);
}

uint32_t aon_pmu_hal_lpo_src_get()                                                                                                                                                                             
{
	return 0;
}

