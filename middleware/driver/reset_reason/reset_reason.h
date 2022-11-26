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

#include "sdkconfig.h"


#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236A)
	#define     START_TYPE_ADDR        (0x00800000 + 84 * 4)
	/* 1. For bk7231n/7236, persist memory lost after power on
	 * 2. For other platform, persist memory lost after interrupt watchdog or power on
	 * */
	#define PERSIST_MEMORY_ADDR (0x0040001c)
#elif (CONFIG_SOC_BK7256XX)
	#define START_TYPE_ADDR        (0x44000000 + 0x3 * 4)    //aon_pmu reg03
	#define PERSIST_MEMORY_ADDR    (0x20007FF0)
	#define REBOOT_TAG_ADDR        (0x20007FF4)
	#define RTC_TIME_SEC_ADDR      (0x20007FF8)
	#define RTC_TIME_USEC_ADDR     (0x20007FFC)
	#define REBOOT_TAG_REQ         (0xAA55AA55)   //4 byte
	#define MCAUSE_CAUSE_WATCHDOG  (0x1)
#else
	#define     START_TYPE_ADDR        (0x0080a080)
	/* 1. For bk7231n/7236, persist memory lost after power on
	 * 2. For other platform, persist memory lost after interrupt watchdog or power on
	 * */
	#define PERSIST_MEMORY_ADDR (0x0040001c)
#endif


#define     CRASH_ILLEGAL_JUMP_VALUE      0xbedead00
#define     CRASH_UNDEFINED_VALUE         0xbedead01
#define     CRASH_PREFETCH_ABORT_VALUE    0xbedead02
#define     CRASH_DATA_ABORT_VALUE        0xbedead03
#define     CRASH_UNUSED_VALUE            0xbedead04


void show_reset_reason(void);
uint32_t reset_reason_init(void);


void set_reboot_tag(uint32_t tag);
uint32_t get_reboot_tag(void);

#ifdef __cplusplus
}
#endif
