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
#include "driver/dvp_camera_types.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Init the camera
 *
 * This API will auto detect dvp sensor, and init sensor, jpeg module, i2c module, psram, dma, ect.
 *
 * @param config process for sensor data
 *
 * @attation 1. you need make sure upper module exist.
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */

bk_err_t bk_dvp_camera_driver_init(dvp_camera_config_t *config);

/**
 * @brief     Deinit the camera
 *
 * This API will deinit  sensor, jpeg module, i2c module, psram, dma, ect.
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_dvp_camera_driver_deinit(void);

/**
 * @brief     Get current use camera
 *
 * This API will called after bk_dvp_camera_driver_init
 *
 * @return
 *    - dvp_camera_device_t *: succeed
 *    - NULL: current no sensor work.
 */
dvp_camera_device_t *bk_dvp_camera_get_device(void);

/**
 * @brief     Read sensor register value
 *
 * This API will called after bk_i2c_init
 *
 * @param addr sensor read address
 *
 * @param reg sensor register address
 *
 * @param value sensor register value
 *
 * @return
 *    - 0 : succeed
 *    - other: other errors.
 */
int dvp_camera_i2c_read_uint8(uint8_t addr, uint8_t reg, uint8_t *value);

/**
 * @brief     Read sensor register value
 *
 * This API will called after bk_i2c_init
 *
 * @param addr sensor read address
 *
 * @param reg sensor register address
 *
 * @param value sensor register value
 *
 * @return
 *    - 0 : succeed
 *    - other: other errors.
 */
int dvp_camera_i2c_read_uint16(uint8_t addr, uint16_t reg, uint8_t *value);

/**
 * @brief     Write sensor register value
 *
 * This API will called after bk_i2c_init
 *
 * @param addr sensor read address
 *
 * @param reg sensor register address
 *
 * @param value sensor register value
 *
 * @return
 *    - 0 : succeed
 *    - other: other errors.
 */
int dvp_camera_i2c_write_uint8(uint8_t addr, uint8_t reg, uint8_t value);

/**
 * @brief     Write sensor register value
 *
 * This API will called after bk_i2c_init
 *
 * @param addr sensor write address
 *
 * @param reg sensor register address
 *
 * @param value sensor register value
 *
 * @return
 *    - 0 : succeed
 *    - other: other errors.
 */
int dvp_camera_i2c_write_uint16(uint8_t addr, uint16_t reg, uint8_t value);

/**
 * @brief     jpeg encode config
 *
 * This API will called after bk_dvp_camera_driver_init
 *
 * @param auto_ctrl 0/1:disable/enable auto jpeg encode
 *
 * @param up_size jpeg encode output image upper size, only effect auto_ctrl = 1
 *
 * @param low_size jpeg encode output image lower size, only effect auto_ctrl = 1
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_dvp_camera_encode_config(uint8_t auto_ctrl, uint32_t up_size, uint32_t low_size);

#ifdef __cplusplus
}
#endif
