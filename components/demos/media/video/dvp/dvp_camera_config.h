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

#ifdef __cplusplus
extern "C" {
#endif

// PAS6329_DEV
#define PAS6329_SET_PAGE0  camera_intf_sccb_write_byte(0xef, 0x00)
#define PAS6329_SET_PAGE1  camera_intf_sccb_write_byte(0xef, 0x01)
#define PAS6329_SET_PAGE2  camera_intf_sccb_write_byte(0xef, 0x02)
#define GC_QVGA_USE_SUBSAMPLE          1   // the angle of view is not changed when use QVGA

void camera_intf_set_sener_cfg_value(uint32_t cfg);

void camera_intf_set_device(uint32_t dev);

#ifdef __cplusplus
}
#endif

