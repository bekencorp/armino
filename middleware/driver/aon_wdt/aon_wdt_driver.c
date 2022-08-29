// Copyright 2020-2022 Beken
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

#include <driver/aon_wdt.h>
#include "driver/aon_wdt_types.h"
#include "aon_wdt_driver.h"
#include "aon_wdt_hal.h"

#include "sys_driver.h"
#include "power_driver.h"

#include "reset_reason.h"
#include "bk_fake_clock.h"
#include "bk_rtos_debug.h"
#include <driver/timer.h>


typedef struct {
	aon_wdt_hal_t hal;
	uint8_t init_bits;
} aon_wdt_driver_t;

#define AON_WDT_BARK_TIME_MS    200

#define INT_AON_WDG_FEED_PERIOD_TICK ((BK_MS_TO_TICKS(CONFIG_INT_AON_WDT_PERIOD_MS)) >> 4)

static bool s_aon_wdt_driver_is_init = false;
static bool s_aon_wdt_debug_enabled = true;

static aon_wdt_driver_t s_aon_wdt = {0};
static uint32_t s_aon_wdt_period = CONFIG_INT_AON_WDT_PERIOD_MS;

#if (CONFIG_INT_AON_WDT)
	static uint32_t s_feed_aon_watchdog_time = INT_AON_WDG_FEED_PERIOD_TICK;
#endif

#define AON_WDT_RETURN_ON_DRIVER_NOT_INIT() do {\
		if (!s_aon_wdt_driver_is_init) {\
			AON_WDT_LOGE("AON_WDT driver not init\r\n");\
			return BK_ERR_AON_WDT_DRIVER_NOT_INIT;\
		}\
	} while(0)

#define AON_WDT_RETURN_ON_NOT_INIT() do {\
		if (!s_aon_wdt.init_bits &BIT(0)) {\
			return BK_ERR_AON_WDT_NOT_INIT;\
		}\
	} while(0)

#define AON_WDT_RETURN_ON_INVALID_PERIOD(timeout) do {\
		if ((timeout) > AON_WDT_F_PERIOD_V) {\
			AON_WDT_LOGE("AON_WDT invalid timeout\r\n");\
			return BK_ERR_AON_WDT_INVALID_PERIOD;\
		}\
	} while(0)

bool bk_aon_wdt_is_driver_inited()
{
	return s_aon_wdt_driver_is_init;
}

bk_err_t bk_aon_wdt_driver_init(void)
{
	if (s_aon_wdt_driver_is_init) {
		return BK_OK;
	}
	os_memset(&s_aon_wdt, 0, sizeof(s_aon_wdt));
	aon_wdt_hal_init(&s_aon_wdt.hal);
#if (CONFIG_SYSTEM_CTRL)
	bk_timer_start(TIMER_ID3, AON_WDT_BARK_TIME_MS, (timer_isr_t)bk_wdt_feed_handle);//what's this used for?
#endif
	s_aon_wdt_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_aon_wdt_driver_deinit(void)
{
	if (!s_aon_wdt_driver_is_init) {
		return BK_OK;
	}
	aon_wdt_deinit_common();
#if (CONFIG_SYSTEM_CTRL)
	bk_timer_stop(TIMER_ID3);
#endif
	s_aon_wdt_driver_is_init = false;

	return BK_OK;
}

static void aon_wdt_init_common(void)
{

}

static void aon_wdt_deinit_common(void)
{
	s_aon_wdt_period = CONFIG_INT_AON_WDT_PERIOD_MS;
	aon_wdt_hal_reset_config_to_default(&s_aon_wdt.hal);
#if (CONFIG_SYSTEM_CTRL)
	extern void close_wdt(void);
	close_wdt();
#else
	power_down_wdt();
#endif
}

bk_err_t bk_aon_wdt_start(uint32_t timeout_ms)
{
	AON_WDT_RETURN_ON_DRIVER_NOT_INIT();
	AON_WDT_RETURN_ON_INVALID_PERIOD(timeout_ms);

	if (!s_aon_wdt_debug_enabled) {
		return BK_ERR_AON_WDT_DEBUG_DISABLED;
	}

	if (!timeout_ms) {
		timeout_ms = CONFIG_INT_AON_WDT_PERIOD_MS;
	}

	s_aon_wdt_period = timeout_ms;
	aon_wdt_init_common();
	aon_wdt_hal_init_aon_wdt(&s_aon_wdt.hal, timeout_ms);
	s_aon_wdt.init_bits |= BIT(0);
	AON_WDT_LOGD("bk_wdt_start, s_aon_wdt.init_bits:%x\n", s_aon_wdt.init_bits);

	return BK_OK;
}

bk_err_t bk_aon_wdt_stop(void)
{
	AON_WDT_RETURN_ON_DRIVER_NOT_INIT();
	aon_wdt_deinit_common();
	s_aon_wdt.init_bits &= ~BIT(0);
	AON_WDT_LOGD("bk_aon_wdt_stop, s_aon_wdt.init_bits:%x\n", s_aon_wdt.init_bits);

	return BK_OK;
}

bk_err_t bk_aon_wdt_feed(void)
{
	AON_WDT_RETURN_ON_DRIVER_NOT_INIT();
	AON_WDT_RETURN_ON_NOT_INIT();
	aon_wdt_hal_init_aon_wdt(&s_aon_wdt.hal, s_aon_wdt_period);
	bk_misc_update_set_type(RESET_SOURCE_WATCHDOG);

	return BK_OK;
}

#if (CONFIG_INT_AON_WDT)
void bk_int_aon_wdt_feed(void)
{
	static uint64_t s_last_int_aon_wdt_feed_tick = 0;
	uint64_t current_tick = bk_get_tick();
	if ((current_tick - s_last_int_aon_wdt_feed_tick) >= s_feed_aon_watchdog_time) {
		bk_aon_wdt_feed();
		s_last_int_aon_wdt_feed_tick = current_tick;
		AON_WDT_LOGD("feed interrupt watchdog\n");
	}
}

uint32_t bk_aon_wdt_get_feed_time()
{
	return s_feed_aon_watchdog_time;
}

void bk_aon_wdt_set_feed_time(uint32_t dw_set_time)
{
	s_feed_aon_watchdog_time = dw_set_time;
}

#endif/*#if (CONFIG_INT_AON_WDT)*/

void bk_aon_wdt_feed_handle(void)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();

#if (CONFIG_INT_AON_WDT)
	bk_int_aon_wdt_feed();
#endif

	GLOBAL_INT_RESTORE();
}

void aon_wdt_debug_enable(void)
{
	s_aon_wdt_debug_enabled = true;
}

void aon_wdt_debug_disable(void)
{
	s_aon_wdt_debug_enabled = false;
}
