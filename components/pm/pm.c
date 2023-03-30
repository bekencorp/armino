// Copyright 2022-2023 Beken
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

#include <pm/device.h>
#include <pm/pm.h>
#include <pm/policy.h>
#include <components/system.h>
#include <driver/aon_rtc.h>
#include "pm.h"
#include "stats.h"
#include "sys_driver.h"
#include "aon_pmu_driver.h"
#include "sys.h"

static pm_state_t s_pm_state;
static pm_state_t s_pm_force_state = PM_STATE_ACTIVE;
static uint32_t s_pm_force_sleep_ms = 0;
static bool s_pm_init = false;

const char* pm_get_state_str(pm_state_t state)
{
	switch (state) {
	case PM_STATE_ACTIVE:
		return "ACTIVE";
	case PM_STATE_NORMAL_SLEEP:
		return "NORMAL_SLEEP";
	case PM_STATE_LOW_VOLTAGE:
		return "LOW_VOLTAGE";
	case PM_STATE_DEEP_SLEEP:
		return "DEEP_SLEEP";
	default:
		return "UNKNOWN";
	}
}

pm_state_t pm_get_state(void)
{
	return s_pm_state;
}

int pm_force_state_with_sleep_time(pm_state_t state, uint32_t sleep_ms)
{
	s_pm_force_state = state;
	s_pm_force_sleep_ms = sleep_ms;
	PM_LOGI("force state: %s, sleep_ms=%u\r\n", pm_get_state_str(state), sleep_ms);
	return BK_OK;
}

int pm_force_state(pm_state_t state)
{
	return pm_force_state_with_sleep_time(state, 0);
}

static void pm_rtc_wakeup_timer_cb(aon_rtc_id_t id, uint8_t *name_p, void *param)
{
#if CONFIG_PM_DEBUG
	static uint32_t s_rtc_wakeup_cnt = 0;
	if ((++s_rtc_wakeup_cnt % 20) == 0) {
		pm_stats_dump();
	}
#endif
}

static inline void pm_start_rtc_wakeup_timer(uint32_t sleep_ms)
{
	if (s_pm_state >= PM_STATE_LOW_VOLTAGE) {
		alarm_info_t pm_rtc_timer = {"pm", sleep_ms*RTC_TICKS_PER_1MS, 1, pm_rtc_wakeup_timer_cb, NULL};
		PM_LOGV("start RTC wakeup timer: %ums\r\n", sleep_ms);
		bk_alarm_unregister(AON_RTC_ID_1, pm_rtc_timer.name);
		bk_alarm_register(AON_RTC_ID_1, &pm_rtc_timer);
		aon_pmu_drv_set_wakeup_source(WAKEUP_SOURCE_INT_RTC);
	}
}

int pm_suspend(uint32_t ticks)
{
	if (s_pm_init == false) {
		return BK_FAIL;
	}

	s_pm_state = PM_STATE_ACTIVE;
	if (s_pm_force_state != PM_STATE_ACTIVE) {
		s_pm_state = s_pm_force_state;
		s_pm_force_state = PM_STATE_ACTIVE;
	} else {
		const pm_state_info_t *info = pm_policy_next_state(ticks);
		if (info != NULL) {
			s_pm_state = info->state;
		}
	}

	PM_LOGV("state: %s, ticks=%u\r\n", pm_get_state_str(s_pm_state), ticks);
	if (s_pm_state == PM_STATE_ACTIVE) {
		return BK_OK;
	}

	pm_stats_start();
	// TODO PM enter callback
#if CONFIG_PM_AUTO_RTC_WAKEUP
	if (s_pm_force_sleep_ms != 0xFFFFFFFF) {//Disable RTC timer
		uint32_t sleep_ms = ticks * bk_get_ms_per_tick();
		if (s_pm_force_sleep_ms > 0) {
			sleep_ms = s_pm_force_sleep_ms;
		}
		pm_start_rtc_wakeup_timer(sleep_ms);
	}
#endif
	if (BK_OK != pm_sys_suspend()) {
	}
	// TODO notify slept

	pm_stats_stop();
	pm_sys_resume();
	// TODO notify wakeup
	// TODO exit callback

	pm_stats_update(s_pm_state, ticks);

	return BK_OK;
}

int pm_init(void)
{
	if (s_pm_init == true) {
		return BK_OK;
	}

	pm_device_init();
	pm_policy_lock_state(PM_STATE_NORMAL_SLEEP);
	pm_policy_lock_state(PM_STATE_LOW_VOLTAGE);
	pm_policy_lock_state(PM_STATE_DEEP_SLEEP);
	s_pm_init = true;
	PM_LOGI("pm inited\r\n");
	return BK_OK;
}
