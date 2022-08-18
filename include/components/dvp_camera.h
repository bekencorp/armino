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
 * @brief     Init the camera
 *
 * This API init jpeg and i2c, enable camera at last
.*
 * param data: configure for camera and jpeg
 *
 * @attention 1. work for imaging transfer
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_camera_init(void *data);

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
bk_err_t bk_camera_deinit(void);

/**
 * @brief     set camera config
 *
 * This API will use i2c write camera_init_table, image_resolution_table and frame_rate_table
 *
 * @return
 *    - void
 */
void bk_camera_sensor_config(void);

/**
 * @brief	  set camera pps and fps
 *
 * This API will set jpeg pps and camera pps and fps
 *
 * param dev: dvp type
 * param cfg: frame rate:bit[0-15], image resolution:bit[31-16]
 *
 * @attention 1. need call before camera_intf_config_senser
 *
 * @return
 *	  - kNoErr: succeed
 *	  - others: other errors.
 */
bk_err_t bk_camera_set_param(uint32_t dev, uint32_t cfg);

/**
 * @brief config lcd display
 *
 * param   x_pixel, image  x pixel
 * @return
 *	  - kNoErr: succeed
 *	  - others: other errors.
 */
bk_err_t bk_video_lcd_init(uint16_t x_pixel);


#ifdef __cplusplus
}
#endif

