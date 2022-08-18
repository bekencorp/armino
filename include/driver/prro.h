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

#include <driver/prro_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Init the PRRO driver
 *
 * This API init the resoure common:
 *   - Init PRRO driver control memory
 *
 * @attention 1. This API should be called before any other PRRO APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_prro_driver_init(void);

/**
 * @brief     Deinit the PRRO driver
 *
 * This API free all resource related to PRRO.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_prro_driver_deinit(void);

/**
 * @brief     Set peripherals privilege attribute
 *
 * @param dev the peripherals
 * @param privilege_type the privilege type, with each element must be PRRO_UNPRIVILEGED or PRRO_PRIVILEGED
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_MPC_DRIVER_NOT_INIT: MPC driver is not init
 */
bk_err_t bk_prro_set_privilege_attribute(prro_dev_t dev, prro_privilege_type_t privilege_type);

/**
 * @brief     Set peripherals secure attribute
 *
 * @param dev the peripherals
 * @param secure_type the secure type, with each element must be PRRO_SECURE or PRRO_NON_SECURE
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_MPC_DRIVER_NOT_INIT: MPC driver is not init
 */
bk_err_t bk_prro_set_secure_attribute(prro_dev_t dev, prro_secure_type_t secure_type);

#ifdef __cplusplus
}
#endif

