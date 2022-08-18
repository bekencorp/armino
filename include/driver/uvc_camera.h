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

#include <driver/uvc_camera_types.h>

#ifdef __cplusplus
extern "C" {
#endif

bk_err_t bk_uvc_camera_driver_init(const uvc_camera_config_t * config);
bk_err_t bk_uvc_camera_driver_deinit(void);
bk_err_t bk_uvc_camera_get_config(uvc_camera_device_t *param, uint16_t count);
bk_err_t bk_uvc_camera_set_config(uvc_camera_device_t * config);
bk_err_t bk_uvc_camera_driver_start(void);
bk_err_t bk_uvc_camera_driver_stop(void);
uvc_camera_device_t *bk_uvc_camera_get_device(void);

#ifdef __cplusplus
}
#endif

