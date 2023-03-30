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

#include "log.h"

#if CONFIG_PM_DEBUG

uint8_t g_pm_log_level = PM_LOG_LEVEL_INFO;

static const char* pm_get_log_level_str(uint8_t level)
{
	if (level == PM_LOG_LEVEL_INFO) {
		return "info";
	} else if (level == PM_LOG_LEVEL_DEBUG) {
		return "debug";
	} else {
		return "verbose";
	}
}

int pm_set_log_level(uint8_t level)
{
	g_pm_log_level = level;
	PM_LOGI("pm set log level to: %s\r\n", pm_get_log_level_str(level));
	return BK_OK;
}

#endif
