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

#define PM_TAG "pm"

enum {
	PM_LOG_LEVEL_INFO = 0,
	PM_LOG_LEVEL_DEBUG,
	PM_LOG_LEVEL_VERBOSE
};

#if CONFIG_PM_DEBUG
extern uint8_t g_pm_log_level;
#define PM_LOGD(...) if (g_pm_log_level >= PM_LOG_LEVEL_DEBUG)  BK_LOGI(PM_TAG, ##__VA_ARGS__)
#define PM_LOGV(...) if (g_pm_log_level >= PM_LOG_LEVEL_VERBOSE)  BK_LOGI(PM_TAG, ##__VA_ARGS__)
#else
#define PM_LOGD(...)
#define PM_LOGV(...)
#endif

#define PM_LOGI(...) BK_LOGI(PM_TAG, ##__VA_ARGS__)
#define PM_LOGW(...) BK_LOGW(PM_TAG, ##__VA_ARGS__)
#define PM_LOGE(...) BK_LOGE(PM_TAG, ##__VA_ARGS__)
