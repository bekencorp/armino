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

#include <components/log.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CAMERA_TAG "camera"
#define CAMERA_LOGI(...) BK_LOGI(CAMERA_TAG, ##__VA_ARGS__)
#define CAMERA_LOGW(...) BK_LOGW(CAMERA_TAG, ##__VA_ARGS__)
#define CAMERA_LOGE(...) BK_LOGE(CAMERA_TAG, ##__VA_ARGS__)
#define CAMERA_LOGD(...) BK_LOGD(CAMERA_TAG, ##__VA_ARGS__)


#define PAS6329_DEV             (0xABC00)  /**< sensor flag value */
#define OV_7670_DEV             (0xABC01)
#define PAS6375_DEV             (0xABC02)
#define GC0328C_DEV             (0xABC03)
#define BF_2013_DEV             (0xABC04)
#define GC0308C_DEV             (0xABC05)
#define HM_1055_DEV             (0xABC06)
#define GC_2145_DEV             (0xABC07)


#define PAS6329_DEV_ID          (0x40)
#define OV_7670_DEV_ID          (0x21)
#define PAS6375_DEV_ID          (0x40)
#define GC0328C_DEV_ID          (0x21)      /**< slave_address:0x42 */
#define BF_2013_DEV_ID          (0x6e)
#define GC0308C_DEV_ID          (0x21)
#define HM_1055_DEV_ID          (0x24)      /**< slave_address:0x48 */
#define GC_2145_DEV_ID          (0x3C)

#ifdef __cplusplus
}
#endif