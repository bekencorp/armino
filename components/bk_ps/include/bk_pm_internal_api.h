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

#ifndef _BK_PM_INTERNAL_API_H_
#define _BK_PM_INTERNAL_API_H_


#ifdef __cplusplus
extern "C" {
#endif
#include <modules/pm.h>

void pm_hardware_init();

int pm_module_wakeup_time_set(uint32_t module_name, uint32_t  wakeup_time);

bk_err_t pm_core_bus_clock_ctrl(uint32_t cksel_core, uint32_t ckdiv_core,uint32_t ckdiv_bus, uint32_t ckdiv_cpu0,uint32_t ckdiv_cpu1);

bk_err_t bk_pm_app_auto_vote_state_set(uint32_t value);

uint32_t bk_pm_app_auto_vote_state_get();

void pm_debug_ctrl(uint32_t debug_en);

#ifdef __cplusplus
}
#endif

#endif
