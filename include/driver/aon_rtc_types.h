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

#include <common/bk_include.h>
#include <driver/hal/hal_aon_rtc_types.h>
#include <driver/int_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief AON RTC interrupt service routine
 */

#define BK_ERR_AON_RTC_DRIVER_NOT_INIT    (BK_ERR_AON_RTC_BASE - 1) /**< AON_RTC driver not init */

typedef void (*aon_rtc_isr_t)(aon_rtc_id_t id, void *param);

typedef struct {
        aon_rtc_id_t id;
        icu_int_src_t int_src;
        int_group_isr_t isr;
} aon_rtc_int_config_t;

#ifdef __cplusplus
}
#endif


