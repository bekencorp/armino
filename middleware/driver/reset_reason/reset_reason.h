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

#ifdef __cplusplus
extern "C" {
#endif

#include "bk_private/bk_sys_ctrl.h"

#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236A) || (CONFIG_SOC_BK7256XX)
#define     START_TYPE_ADDR        (0x00800000 + 84 * 4)
#else
#define     START_TYPE_ADDR        (0x0080a080)
#endif

#define     CRASH_ILLEGAL_JUMP_VALUE      0xbedead00
#define     CRASH_UNDEFINED_VALUE         0xbedead01
#define     CRASH_PREFETCH_ABORT_VALUE    0xbedead02
#define     CRASH_DATA_ABORT_VALUE        0xbedead03
#define     CRASH_UNUSED_VALUE            0xbedead04

RESET_SOURCE_STATUS bk_misc_get_start_type();
void show_reset_reason(void);
RESET_SOURCE_STATUS reset_reason_init(void);
void bk_misc_update_set_type(uint32_t type);


#if (CONFIG_SOC_BK7256XX)
#define MCAUSE_CAUSE_WATCHDOG (0x1)
#define REBOOT_TAG_ADDR     (0x20007FF8)   //DTCM last 8 byte
#define REBOOT_TAG_REQ      (0xAA55AA55)   //4 byte
#endif
void set_reboot_tag(uint32_t tag);
uint32_t get_reboot_tag(void);

#ifdef __cplusplus
}
#endif
