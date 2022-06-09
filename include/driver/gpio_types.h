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
//
#pragma once
#include <common/bk_include.h>
#include <driver/hal/hal_gpio_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief GPIO APIs
 * @addtogroup bk_api_gpio GPIO API group
 * @{
 */

/**
 * @brief GPIO defines
 * @defgroup bk_api_gpio_defs macos
 * @ingroup bk_api_gpio
 * @{
 */
#define BK_ERR_GPIO_CHAN_ID			(BK_ERR_GPIO_BASE - 1) /**< gpio channel number is invalid */
#define BK_ERR_GPIO_INVALID_MODE		(BK_ERR_GPIO_BASE - 2) /**< gpio mode is invalid  */
#define BK_ERR_GPIO_NOT_INPUT_MODE		(BK_ERR_GPIO_BASE - 3) /**< gpio is not in input mode */
#define BK_ERR_GPIO_SET_INVALID_FUNC_MODE	(BK_ERR_GPIO_BASE - 4) /**< gpio perial mode is invalid or was be set as 2nd func*/
#define BK_ERR_GPIO_INVALID_INT_TYPE		(BK_ERR_GPIO_BASE - 5) /**< gpio int type is invalid*/
#define BK_ERR_GPIOS_MAP_NONE			(BK_ERR_GPIO_BASE - 6) /**< gpio map device is none*/
#define BK_ERR_GPIO_NOT_OUTPUT_MODE		(BK_ERR_GPIO_BASE - 7) /**< gpio is not in output mode */
#define BK_ERR_GPIO_BITS_NUM			(BK_ERR_GPIO_BASE - 8) /**< gpio map bit num is error */
#define BK_ERR_GPIO_INTERNAL_USED		(BK_ERR_GPIO_BASE - 9) /**< gpio map was be map to a device */
#define BK_ERR_GPIO_MAP_PWMS_CHAN		(BK_ERR_GPIO_BASE - 10) /**< gpio map to pwm pwms' channel is invalid*/
#define BK_ERR_GPIO_INVALID_ID		(BK_ERR_GPIO_BASE - 11) /**< gpio id is invalid */
#define BK_ERR_GPIO_WAKESOURCE_OVER_MAX_CNT (BK_ERR_GPIO_BASE - 12) /**< too much GPIO is register to wakeup source */

/**
 * @brief GPIO interrupt service routine
 */
typedef void (*gpio_isr_t)(gpio_id_t gpio_id);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
