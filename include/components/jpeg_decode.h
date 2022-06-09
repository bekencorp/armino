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

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief JPEG_DEC APIs Version 1.0
 * @{
 */

/**
 * @brief     Init the jpeg_dec
 *
 * This API init the jpeg_dec
 *  - config jpeg_dec image resoult, and simple_rate
 *  - malloc buffer for jpeg_dec
 *
 * @param src_buf: input image address
 * @param dst_buf: output decode image address
 *
 * @attention 1. only for software jpeg_dec 
 *
 * @return
 *    - kNoErr: succeed
 *    - other: errors.
 */
bk_err_t bk_jpeg_dec_sw_init(uint8_t *src_buf, uint8_t *dst_buf);

/**
 * @brief     Deinit the jpeg_dec
 *
 * This API deinit the jpeg_dec
 *  - clear all configure for jpeg_dec
 *  - free buffer for jpeg_dec
 *
 * @attention 1. only for software jpeg_dec 
 *
 * @return
 *    - kNoErr: succeed
 *    - other: errors.
 */
bk_err_t bk_jpeg_dec_sw_deinit(void);

/**
 * @brief     jpeg_decoder_fun
 *
 * This API jpeg_decoder_fun
 *  - jpeg_prepare for jpeg_dec
 *  - start jpeg_dec
 *
 * @param frame_size: input image size
 *
 * @attention 1. only for software jpeg_dec 
 *
 * @return
 *    - kNoErr: succeed
 *    - other: errors.
 */
bk_err_t bk_jpeg_dec_sw_start(uint32_t frame_size);

/**
 * @brief     jpegdec register finish icallback_func
 *
 * This API jpeg_decoder_fun, the callback function wiil execute when decode success
 *
 * @param cb: finish jpeg_dec callback function
 *
 * @attention 1. only for software jpeg_dec 
 *
 * @return
 */
void bk_jpeg_dec_sw_register_finish_callback(void *cb);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif
