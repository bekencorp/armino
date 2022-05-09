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
#include <os/os.h>
#include <os/mem.h>
#include <driver/wdt.h>
#include "wdt_driver.h"
#include "wdt_hal.h"
#include "bk_private/reset_reason.h"
#include "bk_fake_clock.h"
#include "icu_driver.h"
#include "power_driver.h"
#include "bk_rtos_debug.h"
#include <components/system.h>
#include "sys_driver.h"

typedef struct {
	wdt_hal_t hal;
	uint8_t init_bits;
} wdt_driver_t;

#define DUMP_THREAD_WHEN_TASK_WDG_TIGGERED 1
#define DUMP_STACK_WHEN_TASK_WDG_TIGGERED 1

#define INT_WDG_FEED_PERIOD_TICK ((BK_MS_TO_TICKS(CONFIG_INT_WDT_PERIOD_MS)) >> 4)
#define TASK_WDG_PERIOD_TICK (BK_MS_TO_TICKS(CONFIG_TASK_WDT_PERIOD_MS))

#define WDT_RETURN_ON_DRIVER_NOT_INIT() do {\
		if (!s_wdt_driver_is_init) {\
			WDT_LOGE("WDT driver not init\r\n");\
			return BK_ERR_WDT_DRIVER_NOT_INIT;\
		}\
	} while(0)

#define WDT_RETURN_ON_NOT_INIT() do {\
		if (!(s_wdt.init_bits & BIT(0))) {\
			return BK_ERR_WDT_NOT_INIT;\
		}\
	} while(0)

#define WDT_RETURN_ON_INVALID_PERIOD(timeout) do {\
		if ((timeout) > WDT_F_PERIOD_V) {\
			WDT_LOGE("WDT invalid timeout\r\n");\
			return BK_ERR_WDT_INVALID_PERIOD;\
		}\
	} while(0)

static wdt_driver_t s_wdt = {0};
static bool s_wdt_driver_is_init = false;
static uint32_t s_wdt_period = CONFIG_INT_WDT_PERIOD_MS;

#if (CONFIG_TASK_WDT)
static uint64_t s_last_task_wdt_feed_tick = 0;
static uint64_t s_last_task_wdt_log_tick = 0;
static bool s_task_wdt_enabled = true;
#endif

static void wdt_init_common(void)
{
#if (CONFIG_SYSTEM_CTRL)
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_WDG_CPU, CLK_PWR_CTRL_PWR_UP);
#else
	power_up_wdt();
#endif
}

static void wdt_deinit_common(void)
{
	s_wdt_period = CONFIG_INT_WDT_PERIOD_MS;
	wdt_hal_reset_config_to_default(&s_wdt.hal);
#if (CONFIG_SYSTEM_CTRL)
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_WDG_CPU, CLK_PWR_CTRL_PWR_DOWN);
#else
	power_down_wdt();
#endif
}

bk_err_t bk_wdt_driver_init(void)
{
	if (s_wdt_driver_is_init) {
		return BK_OK;
	}

	os_memset(&s_wdt, 0, sizeof(s_wdt));
	wdt_hal_init(&s_wdt.hal);
	s_wdt_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_wdt_driver_deinit(void)
{
	if (!s_wdt_driver_is_init) {
		return BK_OK;
	}
	wdt_deinit_common();
	s_wdt_driver_is_init = false;

	return BK_OK;
}

/* 1. power up wdt
 * 2. set period
 */
bk_err_t bk_wdt_start(uint32_t timeout_ms)
{
	WDT_RETURN_ON_DRIVER_NOT_INIT();
	WDT_RETURN_ON_INVALID_PERIOD(timeout_ms);

	if (!timeout_ms) {
		timeout_ms = CONFIG_INT_WDT_PERIOD_MS;
	}

	s_wdt_period = timeout_ms;
	wdt_init_common();
	wdt_hal_init_wdt(&s_wdt.hal, timeout_ms);
	s_wdt.init_bits |= BIT(0);
	WDT_LOGI("bk_wdt_start, s_wdt.init_bits:%x\n", s_wdt.init_bits);

	return BK_OK;
}

bk_err_t bk_wdt_stop(void)
{
	WDT_RETURN_ON_DRIVER_NOT_INIT();
	wdt_deinit_common();
	s_wdt.init_bits &= ~BIT(0);
	WDT_LOGI("bk_wdt_stop, s_wdt.init_bits:%x\n", s_wdt.init_bits);
	return BK_OK;
}

bk_err_t bk_wdt_feed(void)
{
	WDT_RETURN_ON_DRIVER_NOT_INIT();
	WDT_RETURN_ON_NOT_INIT();

	wdt_hal_init_wdt(&s_wdt.hal, s_wdt_period);
	bk_misc_update_set_type(RESET_SOURCE_WATCHDOG);

	return BK_OK;
}

#if (CONFIG_INT_WDT)
void bk_int_wdt_feed(void)
{
	static uint64_t s_last_int_wdt_feed_tick = 0;
	uint64_t current_tick = bk_get_tick();

	if ((current_tick - s_last_int_wdt_feed_tick) >= INT_WDG_FEED_PERIOD_TICK) {
		bk_wdt_feed();
		s_last_int_wdt_feed_tick = current_tick;
		// WDT_LOGD("feed interrupt watchdog\n");
	}
}
#endif

#if (CONFIG_TASK_WDT)
void bk_task_wdt_start()
{
	s_task_wdt_enabled = true;
}

void bk_task_wdt_stop()
{
	s_task_wdt_enabled = false;
}

void bk_task_wdt_feed(void)
{
	s_last_task_wdt_feed_tick = bk_get_tick();
}

void bk_task_wdt_timeout_check(void)
{
	if (s_last_task_wdt_feed_tick && s_task_wdt_enabled) {
		uint64_t current_tick = bk_get_tick();
		if ((current_tick - s_last_task_wdt_feed_tick) > TASK_WDG_PERIOD_TICK) {
			if ((current_tick - s_last_task_wdt_log_tick) > TASK_WDG_PERIOD_TICK) {
				WDT_LOGW("task watchdog tiggered\n");
				s_last_task_wdt_log_tick = current_tick;
#if DUMP_THREAD_WHEN_TASK_WDG_TIGGERED
				rtos_dump_task_list();
#endif
#if DUMP_STACK_WHEN_TASK_WDG_TIGGERED
				rtos_dump_backtrace();
#endif
			}
		}
	}
}
#endif

