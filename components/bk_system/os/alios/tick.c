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

#include "bk_ps_time.h"
#include "bk_wdt.h"
#include "sys_rtos.h"
#include "k_api.h"
#include "bk_fake_clock.h"
#include <components/system.h>

void bk_tick_handle(uint8_t arg)
{
#if CONFIG_TICK_CALI
	if (!mcu_ps_need_pstick())
		return;
#endif
	krhino_tick_proc();
}

int bk_update_tick(uint32_t tick)
{
	krhino_update_sys_tick((uint64_t)tick);
	return 0;
}

uint64_t bk_get_tick(void)
{
	return krhino_sys_tick_get();
}

uint32_t bk_get_second(void)
{
	return (krhino_sys_tick_get() / bk_get_ticks_per_second());
}

uint32_t bk_get_ticks_per_second(void)
{
	return (RHINO_CONFIG_TICKS_PER_SECOND);
}
    
uint32_t bk_get_ms_per_tick(void)
{
	return (1000/RHINO_CONFIG_TICKS_PER_SECOND);
}


