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

#include <components/system.h>
#include <pm/device.h>
#include <pm/pm.h>
#include "pm.h"

#define PM_POLICY_LOCK()     pm_lock(PM_MODULE_POLICY)
#define PM_POLICY_UNLOCK()   pm_unlock(PM_MODULE_POLICY)

static int8_t s_pm_policy_lock_cnt[PM_STATE_COUNT] = {0};
static uint64_t s_pm_policy_device_lock_bitmap[PM_STATE_COUNT] = {0};

int pm_policy_lock_state(pm_state_t state)
{
	s_pm_policy_lock_cnt[state]++;
	return BK_OK;
}

int pm_policy_unlock_state(pm_state_t state)
{
	s_pm_policy_lock_cnt[state]--;
	return BK_OK;
}
	
int pm_policy_device_lock_state(const device_t *dev, pm_state_t state)
{
	if (!dev) {
		return BK_ERR_PM_DEVICE_NULL_DEV;
	}

	device_handle_t handle = device_get_handle(dev);
	s_pm_policy_device_lock_bitmap[state] |= BIT(handle);
	PM_LOGV("lock bitmap[%s]: %x.%x, handle=%u\r\n", pm_get_state_str(state), 
		(uint32_t)(s_pm_policy_device_lock_bitmap[state] >> 32), (uint32_t)(s_pm_policy_device_lock_bitmap[state]), handle);
	return BK_OK;
}

int pm_policy_device_unlock_state(const device_t *dev, pm_state_t state)
{
	if (!dev) {
		return BK_ERR_PM_DEVICE_NULL_DEV;
	}

	device_handle_t handle = device_get_handle(dev);
	s_pm_policy_device_lock_bitmap[state] &= ~BIT(handle);
	PM_LOGV("unlock bitmap[%s]: %x.%x, handle=%u\r\n", pm_get_state_str(state), 
		(uint32_t)(s_pm_policy_device_lock_bitmap[state] >> 32), (uint32_t)(s_pm_policy_device_lock_bitmap[state]), handle);
	return BK_OK;
}

static bool pm_policy_is_state_locked(pm_state_t state)
{
	return ((s_pm_policy_device_lock_bitmap[state] != 0) || (s_pm_policy_lock_cnt[state] != 0));
}
	
// Different SoC or Customer can overwrite their own PM policy
// by disabling CONFIG_PM_DEFAULT_POLICY and provide pm_policy_next_state
#if CONFIG_PM_DEFAULT_POLICY
DEVICE_EXPORT(sys);
static uint32_t s_max_latency_us = -1;

//TODO optimize it per actual wakeup-latency
static const pm_state_info_t s_states[] = {
	{PM_STATE_ACTIVE,        0,         0},
#if CONFIG_PM_NS
	{PM_STATE_NORMAL_SLEEP, CONFIG_PM_NS_RESIDENCY, CONFIG_PM_NS_LATENCY},
#endif
#if CONFIG_PM_LV
	{PM_STATE_LOW_VOLTAGE,  CONFIG_PM_LV_RESIDENCY, CONFIG_PM_LV_LATENCY},
#endif
#if CONFIG_PM_DS
	{PM_STATE_DEEP_SLEEP,   CONFIG_PM_DS_RESIDENCY, CONFIG_PM_DS_LATENCY},
#endif
};

const pm_state_info_t *pm_policy_next_state(uint32_t ticks)
{
	uint32_t sleep_us = 1000 * bk_get_ms_per_tick() * ticks;
	const device_t *sys = DEVICE_ID2PTR(sys);

	for (int16_t i = sizeof(s_states)/sizeof(pm_state_info_t) - 1; i >= 0; i--) {
		const pm_state_info_t *state = &s_states[i];

		if (pm_policy_is_state_locked(state->state)) {
			pm_stats_inc_lock_cnt(state->state);
			continue;
		}

		if (state->exit_latency_us >= s_max_latency_us) {
			pm_stats_inc_latency_cnt(state->state);
			continue;
		}

		if (sleep_us < (state->min_residency_us + state->exit_latency_us)) {
			pm_stats_inc_residency_cnt(state->state);
			continue;
		}

		if (pm_device_is_suspend_allowed(sys, state->state)) {
			pm_stats_inc_flag_cnt(state->state);
			return &s_states[i];
		}
	}

	return NULL;
}

#endif

#if CONFIG_PM_DEBUG
static bool pm_policy_is_enabled(pm_state_t state)
{
	for (int i = 0; i < sizeof(s_states)/sizeof(s_states[0]); i++) {
		if (state == s_states[i].state) {
			return true;
		}
	}
	return false;
}

static const char* pm_policy_get_enabled_str(pm_state_t state)
{
	if (pm_policy_is_enabled(state)) {
		return "Y";
	} else {
		return "N";
	}
}

static uint32_t pm_policy_get_residency(pm_state_t state)
{
	for (int i = 0; i < sizeof(s_states)/sizeof(s_states[0]); i++) {
		if (state == s_states[i].state) {
			return s_states[i].min_residency_us;
		}
	}
	return 0;
}

static uint32_t pm_policy_get_latency(pm_state_t state)
{
	for (int i = 0; i < sizeof(s_states)/sizeof(s_states[0]); i++) {
		if (state == s_states[i].state) {
			return s_states[i].exit_latency_us;
		}
	}
	return 0;
}

void pm_policy_dump(void)
{
	PM_LOGI("%-16s %-10s %-10s %-10s\r\n", "item", "normal", "lv", "deep");
	PM_LOGI("%-16s %-10s %-10s %-10s\r\n", "----------------", "----------", "----------", "----------");

	PM_LOGI("%-16s %-10s %-10s %-10s\r\n", "enabled", pm_policy_get_enabled_str(PM_STATE_NORMAL_SLEEP),
		pm_policy_get_enabled_str(PM_STATE_LOW_VOLTAGE), pm_policy_get_enabled_str(PM_STATE_DEEP_SLEEP));
	PM_LOGI("%-16s %-10u %-10u %-10u\r\n", "residency", pm_policy_get_residency(PM_STATE_NORMAL_SLEEP),
		pm_policy_get_residency(PM_STATE_LOW_VOLTAGE), pm_policy_get_residency(PM_STATE_DEEP_SLEEP));
	PM_LOGI("%-16s %-10u %-10u %-10u\r\n", "latency", pm_policy_get_latency(PM_STATE_NORMAL_SLEEP),
		pm_policy_get_latency(PM_STATE_LOW_VOLTAGE), pm_policy_get_latency(PM_STATE_DEEP_SLEEP));
	PM_LOGI("%-16s %-10d %-10d %-10d\r\n", "lock_cnt", s_pm_policy_lock_cnt[PM_STATE_NORMAL_SLEEP],
		s_pm_policy_lock_cnt[PM_STATE_LOW_VOLTAGE], s_pm_policy_lock_cnt[PM_STATE_DEEP_SLEEP]);
	PM_LOGI("%-16s %-10x %-10x %-10x\r\n", "lock_device_h", s_pm_policy_device_lock_bitmap[PM_STATE_NORMAL_SLEEP] >> 32,
		s_pm_policy_device_lock_bitmap[PM_STATE_LOW_VOLTAGE] >> 32, s_pm_policy_device_lock_bitmap[PM_STATE_DEEP_SLEEP] >> 32);
	PM_LOGI("%-16s %-10x %-10x %-10x\r\n", "lock_device_l", s_pm_policy_device_lock_bitmap[PM_STATE_NORMAL_SLEEP] & 0xFFFFFFFF,
		s_pm_policy_device_lock_bitmap[PM_STATE_LOW_VOLTAGE] & 0xFFFFFFFF, s_pm_policy_device_lock_bitmap[PM_STATE_DEEP_SLEEP] & 0xFFFFFFFF);
}
#endif
