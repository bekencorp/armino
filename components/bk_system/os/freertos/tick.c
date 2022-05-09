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

#include <common/bk_include.h>
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "bk_ps_time.h"
#include "bk_wdt.h"
#include <components/system.h>

extern void mcu_ps_increase_clr(void);

void bk_tick_handle(uint8_t arg)
{
	GLOBAL_INT_DECLARATION();

#if CONFIG_TICK_CALI
	if (!mcu_ps_need_pstick())
		return;
#endif

	GLOBAL_INT_DISABLE();

#if (CONFIG_INT_WDT)
	bk_int_wdt_feed();
#endif

#if (CONFIG_TASK_WDT)
	bk_task_wdt_timeout_check();
#endif

	if (xTaskIncrementTick() != pdFALSE) {
		vTaskSwitchContext();
	}
	GLOBAL_INT_RESTORE();
}

int bk_update_tick(uint32_t tick)
{
	GLOBAL_INT_DECLARATION();

	if (tick == 0)
		return 0;

	GLOBAL_INT_DISABLE();
	mcu_ps_increase_clr();
	vTaskStepTick(tick);
	GLOBAL_INT_RESTORE();
	return 0;
}

uint64_t bk_get_tick(void)
{
	if(platform_is_in_interrupt_context() == RTOS_SUCCESS) {
		return xTaskGetTickCountFromISR();
	} else {
		return xTaskGetTickCount();
	}
}

uint32_t bk_get_second(void)
{
	return (bk_get_tick() / bk_get_ticks_per_second());
}

uint32_t bk_get_ticks_per_second(void)
{
	return configTICK_RATE_HZ;
}
    
uint32_t bk_get_ms_per_tick(void)
{
	return (1000/configTICK_RATE_HZ);
}
