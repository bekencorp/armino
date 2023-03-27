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

#include "wdt_hal.h"
#include "wdt_ll.h"

bk_err_t wdt_hal_init(wdt_hal_t *hal)
{
#if (CONFIG_SOC_BK7256XX)
#if CONFIG_DEBUG_FIRMWARE
        hal->id = 1;  //debug version use nmi_wdt to dump
#else
	if (aon_pmu_hal_is_chipid_later_than_version_C())
		hal->id = 0;  //chip version C -> aon_wdt
	else
		hal->id = 1;  //chip version A -> nmi_wdt
#endif
#endif
	hal->hw = (wdt_hw_t *)WDT_LL_REG_BASE(hal->id);
	wdt_ll_init(hal->hw);
	return BK_OK;
}

bk_err_t wdt_hal_init_wdt(wdt_hal_t *hal, uint32_t timeout)
{
	wdt_ll_set_period(hal->hw, timeout);
	return BK_OK;
}

