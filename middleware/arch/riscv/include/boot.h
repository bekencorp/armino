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

#include <common/sys_config.h>


#define  CPU_BOOT_TIME_ADDR   (0x20007FF0)

typedef enum {
    CPU_BOOT_TIME = 0x0,
    CPU_INIT_MEM_TIME = 0x1,
    CPU_MAIN_ENTRY_TIME = 0x2,
    CPU_INIT_DRIVER_TIME = 0x3,
    CPU_APP_ENTRY_TIME = 0x4,
    CPU_START_SCHE_TIME = 0x5,
    CPU_START_WIFI_INIT_TIME = 0x6,
    CPU_FINISH_WIFI_INIT_TIME = 0x7,
    CPU_APP_FINISH_TIME = 0x8,
    CPU_MIAN_FINISH_TIME = 0x9,
    CPU_START_CONNECT_TIME = 0xa,
    CPU_CONNECTED_TIME = 0xb,
    CPU_SAVED_TIME_MAX = 0xc
} CPU_BOOT_TIME_POINT;

void save_mtime_point(uint32_t time_point);
void show_saved_mtime_info(void);
void show_current_time_point(const char *info);


#ifdef __cplusplus
}
#endif
