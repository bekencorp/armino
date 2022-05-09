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
#include "driver/jpeg_dec_types.h"


#ifdef __cplusplus
extern "C" {
#endif


 /**
 * @brief     this api open the jpeg decode clk
 *
 */
bk_err_t bk_jpeg_dec_driver_init(void);


/**
* @brief	 this api close the jpeg decode clk
*          - disable jpeg decode
*/
bk_err_t bk_jpeg_dec_driver_deinit(void);


/**
 * @brief    This API init dec
 *
 * @param
 *     - dec_src_addr: decode data src addr
 *     - dec_dest_addr: decode data dest addr
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_jpeg_dec_init(uint32_t * dec_src_addr,  uint32_t *dec_dest_addr);

/**
 * @brief    This API start dec
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */

bk_err_t bk_jpeg_dec_start(void);

#if (USE_JPEG_DEC_COMPLETE_CALLBACKS == 1)

/**
 * @brief    This API is jpeg dec complete callback
 *
 * @param   isr: isr function
 *         - param isr parameters
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_jpeg_dec_complete_cb(jpeg_dec_isr_t isr, void *param);
#else

/**
 * @brief    This API is direct registerjpeg  dec cpu isr
 *
 * @param   isr: isr function
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t  bk_jpeg_dec_isr_register(jpeg_dec_isr_t jpeg_dec_isr);
#endif


#ifdef __cplusplus
}
#endif









