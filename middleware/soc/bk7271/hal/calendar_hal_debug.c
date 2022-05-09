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
#include "calendar_hw.h"
#include "calendar_hal.h"
#include "calendar_ll.h"

#if CFG_HAL_DEBUG_CALENDAR

void calendar_struct_dump(void)
{
	calendar_hw_t *hw = (calendar_hw_t *)CALENDAR_LL_REG_BASE(0);
	SOC_LOGI("base=%x\r\n", (uint32_t)hw);

	SOC_LOGI("  init_val addr=0x%x value=0x%x\r\n", &hw->init_val, hw->init_val);
	SOC_LOGI("\r\n");

	SOC_LOGI("  wake_up_period addr=0x%x value=0x%x\r\n", &hw->wake_up_period, hw->wake_up_period);
	SOC_LOGI("\r\n");

	SOC_LOGI("  cfg=0x%x value=0x%x\r\n", &hw->cfg, hw->cfg.v);
	SOC_LOGI("    en:                 %x\r\n", hw->cfg.en);
	SOC_LOGI("    wake_en:            %x\r\n", hw->cfg.wake_en);
	SOC_LOGI("    reserved0:          %x\r\n", hw->cfg.reserved0);
	SOC_LOGI("    wake_up_int_status: %x\r\n", hw->cfg.wake_up_int_status);
	SOC_LOGI("    reserved1:          %x\r\n", hw->cfg.reserved1);
	SOC_LOGI("\r\n");

	SOC_LOGI("  cnt_val_s addr=0x%x value=0x%x\r\n", &hw->cnt_val_s, hw->cnt_val_s);
	SOC_LOGI("\r\n");

	SOC_LOGI("  cnt_val_us=0x%x value=0x%x\r\n", &hw->cnt_val_us, hw->cnt_val_us.v);
	SOC_LOGI("    cnt_31_25_us_val: 0x%x\r\n", hw->cnt_val_us.cnt_31_25_us_val);
	SOC_LOGI("    reserved:         0x%x\r\n", hw->cnt_val_us.reserved);
}

#endif

