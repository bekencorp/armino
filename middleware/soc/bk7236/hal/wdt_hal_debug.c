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
#include "wdt_hw.h"
#include "wdt_hal.h"
#include "wdt_ll.h"

#if CFG_HAL_DEBUG_WDT

void wdt_struct_dump(void)
{
	wdt_hw_t *hw = (wdt_hw_t *)WDT_LL_REG_BASE(0);
	SOC_LOGI("base=%x\r\n", (uint32_t)hw);

	SOC_LOGI("  ctrl=0x%x value=0x%x\n", &hw->ctrl, hw->ctrl.v);
	SOC_LOGI("    period: 0x%x\n", hw->ctrl.period);
	SOC_LOGI("    key: 0x%x\n", hw->ctrl.key);
}

#endif


