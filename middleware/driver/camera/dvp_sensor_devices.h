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

extern const dvp_sensor_config_t dvp_sensor_gc0328c;
extern const dvp_sensor_config_t dvp_sensor_gc2145;
extern const dvp_sensor_config_t dvp_sensor_hm1055;
extern const dvp_sensor_config_t dvp_sensor_ov2640;
extern const dvp_sensor_config_t dvp_sensor_gc0308;
extern const dvp_sensor_config_t dvp_sensor_SC101;


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


#ifdef __cplusplus
}
#endif
