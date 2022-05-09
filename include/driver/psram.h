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

//#include <driver/qspi_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief psram API
 * @defgroup bk_api_psram PSRAM API group
 * @{
 */

/**
 * @brief     Init the PSRAM driver
 *
 * This API init the resoure common to PSRAM:
 *   - Init PSRAM driver control register
 *   - Configure PSRAM Clock and voltage selection
 *
 * This API should be called before any other use PSRAM.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_psram_driver_init(void);

/**
 * @brief     Deinit the PSRAM driver
 *
 * This API free the resoure common to PSRAM:
 *   - Init PSRAM driver control register
 *   - power down PSRAM voltage
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_psram_driver_deinit(void);

/**
 * @brief     Init the PSRAM HW
 *
 * This API init the resoure common to all PSRAM:
 *   - Configure the clock control register
 *   - Select a frequency multiple
 *   - Flash clock select
 *   - Enable the clock
 *
 * This API should be called before any other PSRAM APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_psram_init(uint32_t mode);

/**
 * @brief     Deinit the PSRAM HW
 *
 * This API free the resoure common to PSRAM:
 *   - Init PSRAM HW control register
 *   - power down PSRAM voltage
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_psram_deinit(void);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

