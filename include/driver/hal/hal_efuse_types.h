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

/**
 * @brief EFUSE defines
 * @defgroup bk_api_efuse_defs macos
 * @ingroup bk_api_efuse
 * @{
 */

#define BK_ERR_EFUSE_DRIVER_NOT_INIT    (BK_ERR_EFUSE_BASE - 1) /**< EFUSE driver not init */
#define BK_ERR_EFUSE_ADDR_OUT_OF_RANGE  (BK_ERR_EFUSE_BASE - 2) /**< EFUSE address is out of range */
#define BK_ERR_EFUSE_CANNOT_WRTIE       (BK_ERR_EFUSE_BASE - 3) /**< EFUSE cannot write, cannot change bit from 1 to 0 */
#define BK_ERR_EFUSE_WRTIE_NOT_EQUAL    (BK_ERR_EFUSE_BASE - 4) /**< EFUSE data read is not equal to the data written */

typedef uint8_t efuse_unit_t; /**< efuse uint id */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

