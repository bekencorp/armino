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

#include <common/bk_include.h>

#ifdef CONFIG_PM_STATS
void pm_stats_start(void);
void pm_stats_stop(void);
void pm_stats_update(pm_state_t state, uint32_t ticks);
void pm_stats_inc_lock_cnt(pm_state_t state);
void pm_stats_inc_latency_cnt(pm_state_t state);
void pm_stats_inc_residency_cnt(pm_state_t state);
void pm_stats_inc_flag_cnt(pm_state_t state);
#else
#define pm_stats_start()
#define pm_stats_stop()
#define pm_stats_update(state, ticks)
#define pm_stats_inc_lock_cnt(state)
#define pm_stats_inc_latency_cnt(state)
#define pm_stats_inc_residency_cnt(state)
#define pm_stats_inc_flag_cnt(state)
#endif
