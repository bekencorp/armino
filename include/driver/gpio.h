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
#include <driver/gpio_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief GPIO API
 * @defgroup bk_api_gpio GPIO API group
 * @{
 */


/**
 * @brief     Init the GPIO driver
 *
 * This API init the resoure common to all GPIO channels:
 *   - Init GPIO driver control memory
 *
 * This API should be called before any other GPIO APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_gpio_driver_init(void);


/**
 * @brief     Deinit the GPIO driver
 *
 * This API free all resource related to GPIO and power down all GPIO channels.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_gpio_driver_deinit(void);


/**
 * @brief     enable GPIO output mode
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_enable_output(gpio_id_t gpio_id);


/**
 * @brief     disable GPIO output mode
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_disable_output(gpio_id_t gpio_id);


/**
 * @brief     enable GPIO input mode
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_enable_input(gpio_id_t gpio_id);


/**
 * @brief     disable GPIO input mode
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_disable_input(gpio_id_t gpio_id);


/**
 * @brief     enable GPIO pull mode
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_enable_pull(gpio_id_t gpio_id);


/**
 * @brief     disable gpio pull mode
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_disable_pull(gpio_id_t gpio_id);


/**
 * @brief     set GPIO as pull up mode
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_pull_up(gpio_id_t gpio_id);


/**
 * @brief     set GPIO as pull down mode
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_pull_down(gpio_id_t gpio_id);


/**
 * @brief     Config the GPIO mode
 *
 * This API config GPIO's mode
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_INVALID_MODE: invalid GPIO's io_mode/pull_mode
 *    - BK_ERR_GPIO_INTERNAL_USED:GPIO was map to another device
 *    - others: other errors.
 */
bk_err_t bk_gpio_set_config(gpio_id_t gpio_id, const gpio_config_t *config);


/**
 * @brief     Set the GPIO output high,
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_NOT_OUTPUT_MODE:GPIO not in output mode

 *    - others: other errors.
 */
bk_err_t bk_gpio_set_output_high(gpio_id_t gpio_id);

/**
 * @brief     Set the GPIO output low,
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_NOT_OUTPUT_MODE:GPIO not in output mode
 *    - others: other errors.
 */

bk_err_t bk_gpio_set_output_low(gpio_id_t gpio_id);

/**
 * @brief     Get the GPIO input value,
 *
 * This API get GPIO's input level: 0 :low_level 1:high_level.
 *
 * @return
 *    - input value
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_NOT_INPUT_MODE : GPIO is not input mode
 *    - others: other errors.
 */
bool bk_gpio_get_input(gpio_id_t gpio_id);

/**
 * @brief     Set the GPIO driver capacity.
 *
 * This API Set GPIO's output driver capacity which range is 0~3.
 *
 * @return
 *    - input value
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_NOT_INPUT_MODE : GPIO is not input mode
 *    - others: other errors.
 */
bool bk_gpio_set_capacity(gpio_id_t gpio_id, uint32 capacity);

/**
 * @brief     Config the GPIO intterrupt type mode  when use gpio intterrupt mode,
 *
 * This API config all GPIO channels' intterrupt mode, the mode included in gpio_int_type_t.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_INVALID_INT_TYPE: invalid GPIO int type
 *    - others: other errors.
 */
bk_err_t bk_gpio_set_interrupt_type(gpio_id_t gpio_id, gpio_int_type_t type);

/**
 * @brief     Enable GPIO intterrupt.
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_NOT_INPUT_MODE : GPIO is not input mode
 *    - others: other errors.
 */
bk_err_t bk_gpio_enable_interrupt(gpio_id_t id);

/**
 * @brief     Disable GPIO intterrupt.
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_disable_interrupt(gpio_id_t id);

/**
 * @brief     Register the interrupt service routine for GPIO channel
 *
 * This API regist gpio isr callback function.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid gpio channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_register_isr(gpio_id_t id, gpio_isr_t isr);
/**
 * @brief     Register save all gpio reg value
 *
 * This API save all gpio reg value function.
 *
 * @return
 *    - BK_OK: succeed
 *    
 *    - others: other errors.
 */
bk_err_t bk_gpio_reg_save(uint32_t*  gpio_cfg);
/**
 * @brief     Register restore all gpio reg value
 *
 * This API restore all gpio reg value function.
 *
 * @return
 *    - BK_OK: succeed
 *    - 
 *    - others: other errors.
 */
bk_err_t bk_gpio_reg_restore(uint32_t*  gpio_cfg);
/**
 * @brief     Register configue the gpio wakeup value
 *
 * This API configue the gpio wakeup value function.
 *
 * @return
 *    - BK_OK: succeed
 *    - B
 *    - others: other errors.
 */
bk_err_t bk_gpio_wakeup_enable(int64_t index, uint64_t type_l, uint64_t type_h);

/**
 * @brief Register clear wake up interrupt
 *
 * This API clear wake up interrupt function.
 *
 * @return
 *    - BK_OK: succeed
 *    - B
 *    - others: other errors.
 */
bk_err_t bk_gpio_wakeup_interrupt_clear();
/**
 * @}
 */

#ifdef __cplusplus
}
#endif
