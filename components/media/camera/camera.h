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
#include "media_evt.h"
#include <driver/dvp_camera_types.h>

#ifdef __cplusplus
extern "C" {
#endif

bk_err_t bk_dvp_camera_open(media_ppi_t ppi, dvp_mode_t mode);
bk_err_t bk_dvp_camera_close(void);
bk_err_t bk_uvc_camera_open(media_ppi_t ppi);
bk_err_t bk_uvc_camera_start(void);
bk_err_t bk_uvc_camera_stop(void);
bk_err_t bk_uvc_camera_close(void);
bk_err_t bk_uvc_camera_param_set(param_pak_t *param);

#ifdef __cplusplus
}
#endif
