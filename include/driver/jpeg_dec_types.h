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

#include <common/bk_err.h>


#ifdef __cplusplus
extern "C" {
#endif

#define USE_JPEG_DEC_COMPLETE_CALLBACKS 1 /**< set 1, register jpeg decode complete callback, set 0, register jpeg dec cpu isr*/

#if (USE_JPEG_DEC_COMPLETE_CALLBACKS == 1)
typedef void (*jpeg_dec_isr_t)(void *param);  /**< jpegdec complete callback func type */
#else
typedef void (*jpeg_dec_isr_t)(void);               /**< jpegdec int isr register func type */
#endif

/*
 * @}
 */

#ifdef __cplusplus
}
#endif


