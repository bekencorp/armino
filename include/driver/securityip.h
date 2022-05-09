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

#include "securityip_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/* @brief Overview about this API header
 *
 */

/**
 * @brief VAULT API
 * @defgroup bk_api_vault VAULT API group
 * @{
 */

/**
 * @brief     Init the VAULT driver
 *
 * This API init the resoure common:
 *   - Init VAULT driver control memory
 *
 * @attention 1. This API should be called before any other VAULT APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_VAULT_DRIVER_INIT_FAIL: vault driver init failed
 *    - others: other errors.
 */
bk_err_t bk_vault_driver_init(void);

/**
 * @brief     Deinit the VAULT driver
 *
 * This API free all resource related to VAULT and disable VAULT.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_vault_driver_deinit(void);

#ifdef __cplusplus
}
#endif


