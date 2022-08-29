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

#include "aon_wdt_hal.h"
#include "aon_wdt_ll.h"

bk_err_t aon_wdt_hal_init(aon_wdt_hal_t *hal)
{
	hal->hw = (aon_wdt_hw_t *)AON_WDT_LL_REG_BASE(hal->id);
	aon_wdt_ll_init(hal->hw);
	return BK_OK;
}

bk_err_t aon_wdt_hal_init_wdt(aon_wdt_hal_t *hal, uint32_t timeout)
{
	aon_wdt_ll_set_period(hal->hw, timeout);
	return BK_OK;
}


