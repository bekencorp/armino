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
#include "pm.h"

#if CONFIG_AON_RTC
#include <driver/aon_rtc.h>
#include <driver/aon_rtc_types.h>
#endif

#if CONFIG_PM_STATS
uint64_t s_stats_start = 0;
uint64_t s_stats_stop = 0;
typedef struct {
	uint32_t total_suspend_time;
	uint32_t max_suspend_time;
	uint32_t max_expected_suspend_time;
	uint32_t suspend_cnt;
	uint32_t block_by_lock_cnt;
	uint32_t block_by_latency_cnt;
	uint32_t block_by_residency_cnt;
	uint32_t block_by_flag_cnt;
} pm_stats_state_t;
static pm_stats_state_t s_stats_state[PM_STATE_COUNT] = {0};

void pm_stats_inc_lock_cnt(pm_state_t state)
{
	s_stats_state[state].block_by_lock_cnt ++;
}

void pm_stats_inc_latency_cnt(pm_state_t state)
{
	s_stats_state[state].block_by_latency_cnt ++;
}

void pm_stats_inc_residency_cnt(pm_state_t state)
{
	s_stats_state[state].block_by_residency_cnt ++;
}

void pm_stats_inc_flag_cnt(pm_state_t state)
{
	s_stats_state[state].block_by_flag_cnt ++;
}

//TODO add stats to count slept duration:
// < 50ms, <100ms, < 500ms, <1s, >3s
uint64_t pm_stats_get_time(void)
{
	//TODO use more precise timer
#if CONFIG_AON_RTC
	return bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
#else
	return 0;
#endif

}

void pm_stats_start(void)
{
	s_stats_start = pm_stats_get_time();
}

void pm_stats_stop(void)
{
	s_stats_stop = pm_stats_get_time();
}

void pm_stats_update(pm_state_t state, uint32_t ticks)
{
	uint32_t diff = s_stats_stop - s_stats_start;

	if (s_stats_state[state].max_suspend_time < diff) {
		s_stats_state[state].max_suspend_time = diff;
	}

	if (s_stats_state[state].max_expected_suspend_time < ticks) {
		s_stats_state[state].max_expected_suspend_time = ticks;
	}

	s_stats_state[state].total_suspend_time += diff;
	s_stats_state[state].suspend_cnt ++;

}

#define RTC_CYCLE_TO_TICK
void pm_stats_dump(void)
{
	uint64_t current_time = pm_stats_get_time();
	uint32_t percent0 = 0, percent1 = 0, percent2 = 0;

	PM_LOGI("%-16s %-10s %-10s %-10s\r\n", "stats", "normal", "lv", "deep");
	PM_LOGI("%-16s %-10s %-10s %-10s\r\n", "----------------", "----------", "----------", "----------");
	PM_LOGI("%-16s %-10d %-10d %-10d\r\n", "total_suspend", s_stats_state[1].total_suspend_time >> 5,
		s_stats_state[2].total_suspend_time >> 5, s_stats_state[3].total_suspend_time >> 5);
	PM_LOGI("%-16s %-10d %-10d %-10d\r\n", "max_expect", s_stats_state[1].max_expected_suspend_time, 
		s_stats_state[2].max_expected_suspend_time, s_stats_state[3].max_expected_suspend_time);
	PM_LOGI("%-16s %-10d %-10d %-10d\r\n", "max_suspend", s_stats_state[1].max_suspend_time >> 5,
		s_stats_state[2].max_suspend_time >> 5, s_stats_state[3].max_suspend_time >> 5);
	PM_LOGI("%-16s %-10d %-10d %-10d\r\n", "suspend_cnt", s_stats_state[1].suspend_cnt,
		s_stats_state[2].suspend_cnt, s_stats_state[3].suspend_cnt);

	percent0 = ((s_stats_state[1].total_suspend_time >> 5) * 100) / (current_time >> 5);
	percent1 = ((s_stats_state[2].total_suspend_time >> 5) * 100) / (current_time >> 5);
	percent2 = ((s_stats_state[3].total_suspend_time >> 5) * 100) / (current_time >> 5);
	PM_LOGI("%-16s %-10d %-10d %-10d\r\n", "suspend_percent", percent0, percent1, percent2);

	PM_LOGI("%-16s %-10d %-10d %-10d\r\n", "lock", s_stats_state[1].block_by_lock_cnt,
		s_stats_state[2].block_by_lock_cnt, s_stats_state[3].block_by_lock_cnt);
	PM_LOGI("%-16s %-10d %-10d %-10d\r\n", "latency", s_stats_state[1].block_by_latency_cnt,
		s_stats_state[2].block_by_latency_cnt, s_stats_state[3].block_by_latency_cnt);
	PM_LOGI("%-16s %-10d %-10d %-10d\r\n", "residency", s_stats_state[1].block_by_residency_cnt,
		s_stats_state[2].block_by_residency_cnt, s_stats_state[3].block_by_residency_cnt);
	PM_LOGI("%-16s %-10d %-10d %-10d\r\n", "flag", s_stats_state[1].block_by_flag_cnt,
		s_stats_state[2].block_by_flag_cnt, s_stats_state[3].block_by_flag_cnt);
}
#endif

#if CONFIG_PM_DEVICE_STATS
//TODO
void pm_device_stats_start(pm_device_t *pm)
{
	if (pm) {
	}
}

void pm_device_stats_stop(pm_device_t *pm)
{
	if (pm) {
	}
}

void pm_device_stats_update(pm_device_t *pm)
{
	if (pm) {
	}
}

#endif
