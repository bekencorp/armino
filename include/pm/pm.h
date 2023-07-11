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

#pragma once

#include <driver/device.h>
#include <pm/device.h>
#include <pm/dependency.h>

enum pm_state_t {
	PM_STATE_ACTIVE = 0,
	PM_STATE_NORMAL_SLEEP,
	PM_STATE_LOW_VOLTAGE,
	PM_STATE_DEEP_SLEEP,
	PM_STATE_COUNT,
};
typedef enum pm_state_t pm_state_t;

struct pm_state_info_t {
	pm_state_t state;
	uint32_t min_residency_us;
	uint32_t exit_latency_us;
};
typedef struct pm_state_info_t pm_state_info_t;

int pm_init(void);
int pm_suspend(uint32_t ticks);
pm_state_t pm_get_state(void);
int pm_force_state(pm_state_t state);
int pm_force_state_with_sleep_time(pm_state_t state, uint32_t slept_ms);
int pm_set_log_level(uint8_t level);
void pm_stats_dump(void);

#define BK_ERR_PM_BASE -100
#define BK_ERR_PM_DEVICE_NULL_API (BK_ERR_PM_BASE - 1)
#define BK_ERR_PM_DEVICE_NULL_DEV (BK_ERR_PM_BASE - 2)
#define BK_ERR_PM_DEVICE_SUSPENDED_NOT_ALLOWED (BK_ERR_PM_BASE - 3)
#define BK_ERR_PM_DEPEND_EXIST (BK_ERR_PM_BASE - 4)
#define BK_ERR_PM_DEPEND_FULL (BK_ERR_PM_BASE - 5)
#define BK_ERR_PM_MUTEX_INIT_FAIL (BK_ERR_PM_BASE - 6)
