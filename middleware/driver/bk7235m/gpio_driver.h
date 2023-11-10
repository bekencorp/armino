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
#include "gpio_hal.h"

typedef enum {
       GPIO_SDIO_MAP_MODE0 = 0,			/**<GPIO2~GPIO5  GPIO10~GPIO11 is used for SDIO   HOST/SLAVE */
       GPIO_SDIO_MAP_MODE1,			    /**<GPIO14~GPIO19 is used for SDIO HOST/SLAVE */
       GPIO_SDIO_MAP_MODE_MAX,			/**< Invalid sdio mode */
} gpio_sdio_map_mode_t;


typedef enum {
       GPIO_SPI_MAP_MODE0 = 0,		       /**<GPIO14~GPIO17 is used for SPI module */
       GPIO_SPI_MAP_MODE1,                 /**<GPIO44~GPIO47 is used for SPI module*/
       GPIO_SPI_MAP_MODE_MAX ,		       /**< Invalid spi mode */
} gpio_spi1_map_mode_t;

typedef enum {
       GPIO_I2C1_MAP_MODE0 = 0,			/**<GPIO0~GPIO1 is used for I2C1 */
       GPIO_I2C1_MAP_MODE1,			/**<GPIO42~GPIO43 is used for I2C1 */
       GPIO_I2C1_MAP_MODE_MAX,		       /**< Invalid mode*/
} gpio_i2c1_map_mode_t;

typedef enum {
       GPIO_I2S_MAP_MODE = 0,	       /**<GPIO6~GPIO9 is used for I2S */
       GPIO_I2S_MAP_MODE1,		       /**<GPIO40~GPIO43 is used for I2S */
       GPIO_I2S_MAP_MODE_MAX,			/**< Invalid mode*/
} gpio_i2s_map_mode_t;

typedef enum {
       GPIO_JTAG_MAP_GROUP0 = 0,	       /**<GPIO20~GPIO21 is used for jtag */
       GPIO_JTAG_MAP_GROUP1,		       /**<GPIO0~GPIO1 is used for jtag */
       GPIO_JTAG_MAP_GROUP_MAX,			/**< Invalid mode*/
} gpio_jtag_map_group_t;


bk_err_t gpio_dev_map(gpio_id_t gpio_id, gpio_dev_t dev);
bk_err_t gpio_dev_unmap(gpio_id_t gpio_id);
bk_err_t gpio_dev_unprotect_map(gpio_id_t gpio_id, gpio_dev_t dev);
bk_err_t gpio_dev_unprotect_unmap(gpio_id_t gpio_id);
bk_err_t gpio_spi_sel(gpio_spi1_map_mode_t gpio_spi_sel_mode);
bk_err_t gpio_sdio_sel(gpio_sdio_map_mode_t gpio_sdio_sel_mode);
bk_err_t gpio_sdio_one_line_sel(gpio_sdio_map_mode_t gpio_sdio_sel_mode);
bk_err_t gpio_i2c1_sel(gpio_i2c1_map_mode_t gpio_i2c1_sel_mode);
bk_err_t gpio_i2s_sel(gpio_i2s_map_mode_t gpio_i2s_sel_mode);
bk_err_t gpio_jtag_sel(gpio_jtag_map_group_t gpio_jtag_sel_mode);


#if CONFIG_GPIO_WAKEUP_SUPPORT
bk_err_t gpio_enter_low_power(void *param);
bk_err_t gpio_exit_low_power(void *param);
void gpio_get_interrupt_status(uint32_t *h_status, uint32_t *l_status);
bk_err_t gpio_enable_interrupt_mult_for_wake(void);
#endif

#if CONFIG_GPIO_SIMULATE_UART_WRITE
/**
 * @brief	  Uses specifies GPIO to simulate UART write data
 *
 * This API Uses specifies GPIO to simulate UART write data:
 *	 - Uses CPU poll wait to do delay, so it blocks CPU.
 *	 - The caller should confirm the specifies GPIO is not used by other APP.
 *
 * @param *buff  Which buffers will be write with GPIO.
 * @param len    How many bytes data will be wrote.
 * @param gpio_id  Which GPIO will be simulated as UART write data.
 * @param div    Baud rate == 1Mbps/(1+div)
 *
 * @attention 1. As this function just simulate uart write, it blocks the CPU,
 *               so please don't write too much data.
 *
 * @return
 */
void gpio_simulate_uart_write(unsigned char *buff, uint32_t len, gpio_id_t gpio_id, uint32_t div);
#endif

