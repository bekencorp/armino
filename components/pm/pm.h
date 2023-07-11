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

#include <components/log.h>
#include "log.h"
#include "stats.h"

enum pm_module_t {
	PM_MODULE_PM = 0,
	PM_MODULE_DEVICE,
	PM_MODULE_DEVICE_VOTE,
	PM_MODULE_DEPEND,
	PM_MODULE_POLICY,
};
typedef enum pm_module_t pm_module_t;

int pm_lock(pm_module_t module);
int pm_unlock(pm_module_t module);

const char* pm_get_state_str(pm_state_t state);
const char* pm_get_module_str(pm_module_t module);
