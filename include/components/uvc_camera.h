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

#include <components/uvc_camera_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief UVC_camera APIs Version 1.0
 * @{
 */


/**
 * @brief     Init the uvc
 *
 * This API init psram, dma and uvc param
.*
 * param data: configure for camera 
 *
 * @attention 1. work for uvc
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_init(void);

/**
 * @brief     Denit the uvc
 *
 * This API close the modules at init
.*
 * param data: configure for camera
 *
 * @attention 1. work for uvc
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_deinit(void);

/**
 * @brief     save uvc frame
 *
 * This API will save a frame to sdcard if sdcard_config enable
 *
 * param file_id: frame_name, 0-255
 *
 * @attention 1. work for uvc
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_save_frame(uint8_t file_id);

/**
 * @brief     set uvc pps and fps
 *
 * This API will set uvc pps and fps
 *
 * param ppi: image resolution
 * param fps: the number frame output every second
 *
 * @attention 1. need call before uvc_set_start
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_set_ppi_fps(uint16_t ppi, uint8_t fps);

/**
 * @brief     set uvc start
 *
 * This API will set uvc start
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_set_start(void);

/**
 * @brief     set uvc stop
 *
 * This API will set uvc stop
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_set_stop(void);

/**
 * @brief     register uvc frame end callback
 *
 * This API will only excute the at every frame transfer end if register have registered
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_register_frame_end_callback(void *cb);

/**
 * @brief     register frame capture callback
 *
 * This API will only excute the at every frame transfer end if register have registered
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_register_frame_capture_callback(void *cb);

/**
 * @brief     set uvc mem  status
 *
 * This API will enbale/disable save uvc data to psram
 *
 * @param type: 0/1:idle/busy, idle: save uvc data to psram; busy: not save uvc data to psram
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_set_mem_status(uint8_t type);


/**
 * @}
 */


#ifdef __cplusplus
}
#endif

