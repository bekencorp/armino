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

#pragma once

//TODO remove it
#include "temp_detect.h"

#define TMP_DETECT_DEBUG    1

#if TMP_DETECT_DEBUG
#define TMP_DETECT_PRT      os_printf
#define TMP_DETECT_WARN     warning_prf
#define TMP_DETECT_FATAL    fatal_prf
#else
#define TMP_DETECT_PRT      os_printf
#define TMP_DETECT_WARN     null_prf
#define TMP_DETECT_FATAL    null_prf
#endif

#if CFG_USE_TEMPERATURE_DETECT && CFG_USE_VOLTAGE_DETECT
#define ADC_TMEP_DETECT_INTVAL_CHANGE               (60) // 2 mins
#else
#define ADC_TMEP_DETECT_INTVAL_CHANGE               (120) // 2 mins
#endif

#if (CONFIG_SOC_BK7256XX) ||(CONFIG_SOC_BK7236A) || (CONFIG_SOC_BK7231N)
#define ADC_VOLT_SENSER_CHANNEL                     0
#endif

#define BK_ERR_TEMPD_SAMPLE_NO_DATA                 BK_ERR_TEMPD_BASE

void temp_detect_change_configuration(uint32_t intval, uint32_t thre, uint32_t dist);
int temp_detect_init(uint32_t init_val);
int temp_detect_deinit(void);
int temp_detect_stop(void);
int temp_detect_start(void);
bool temp_detect_is_init(void);
int temp_detect_get_temperature(uint32_t *temperature);
UINT32 volt_single_get_current_voltage(UINT32 *volt_value);

