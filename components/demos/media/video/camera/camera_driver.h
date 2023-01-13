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

/**
 * @brief     Init the dvp camera
 *
 * This API init jpeg and i2c, enable camera at last
.*
 * @param data configure for camera and jpeg
 *
 * @attention 1. work for imaging transfer
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_dvp_camera_init(void *data);

/**
 * @brief     deinit the camera
 *
 * This API will close camera function
 *
 * @attention 1. work for imaging transfer
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_dvp_camera_deinit(void);

/**
 * @brief     Init the uvc camera
 *
 * This API power up uvc, config, start
.*
 * @param data configure for uvc camera and dma data process
 *
 * @attention 1. work for imaging transfer
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t _bk_uvc_camera_init(void *data);

/**
 * @brief     deinit the uvc camera
 *
 * This API will close camera function
 *
 * @attention 1. work for imaging transfer
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t _bk_uvc_camera_deinit(void);

#ifdef __cplusplus
}
#endif

