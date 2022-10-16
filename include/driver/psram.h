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

#include <driver/psram_types.h>


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
bk_err_t bk_psram_init(void);

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
 * @brief     continue write data to psram
 *
 * This API will write more fast than dtim
 *
 * @param start_addr write to psram start addr
 * @param data_buf data buffer pointer
 * @param len data len need write to psram
 *
 * @attation 1. len uint byte
 *
 * @return void
 */
void bk_psram_memcpy(uint32_t start_addr, uint32_t *data_buf, uint32_t len);

/**
 * @brief     continue read data from psram
 *
 * This API will read more fast than dtim
 *
 * @param start_addr read from psram start addr
 * @param data_buf data buffer pointer
 * @param len data len need read from psram
 *
 * @attation 1. len uint byte
 *
 * @return void
 */
void bk_psram_memread(uint32_t start_addr, uint32_t *data_buf, uint32_t len);

/**
 * @brief     set psram clk
 *
 * This API used to set psram work clk
 *
 * @param clk:80/120/160/240
 *
 * @attation 1. current only support 80/120/160
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_psram_set_clk(psram_clk_t clk);

/**
 * @brief     set psram voltage
 *
 * This API used to set psram work voltage
 *
 * @param voltage 1.8/2.0/3.0/3.2
 *
 * @attation 1. current only used 1.8v
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_psram_set_voltage(psram_voltage_t voltage);

/**
 * @brief     string cat to psram
 *
 * This API is like strcat
 *
 * @param start_addr write to psram start addr
 * @param data_buf string buffer pointer
 *
 * @return psram start addr
 */
char *bk_psram_strcat(char *start_addr, const char *data_buf);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

