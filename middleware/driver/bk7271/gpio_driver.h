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
       GPIO_SDIO_MAP_MODE0 = 0,			/**<GPIO8~GPIO10  GPIO34~GPIO36 is used for SDIO   HOST/DMA */
       GPIO_SDIO_MAP_MODE1,			/**<GPIO34~GPIO39 is used for SDIO HOST/DMA */
       GPIO_SDIO_MAP_MODE_MAX,			/**< Invalid sdio mode */
} gpio_sdio_map_mode_t;


typedef enum {
       GPIO_SPI_MAP_MODE0 = 0,		       /**<GPIO30~GPIO33 is used for SPI module */
       GPIO_SPI_MAP_MODE1,		       /**<GPIO36~GPIO39 is used for SPI module*/
       GPIO_SPI_MAP_MODE_MAX ,		       /**< Invalid spi mode */
} gpio_spi3_map_mode_t;


typedef enum {
       GPIO_UART2_MAP_MODE0 = 0,	       /**<GPIO6~GPIO7 is used for UART2 */
       GPIO_UART2_MAP_MODE1,		       /**<GPIO16~GPIO17 is used for UART2*/
       GPIO_UART2_MAP_MODE_MAX,			/**< Invalid mode*/
} gpio_uart2_map_mode_t;

typedef enum {
       GPIO_PWMS_MAP_MODE0 = 0,			/**<GPIO18/19/20/21 is used for PWM6/7/8/9 */
       GPIO_PWMS_MAP_MODE1,			/**<GPIO26/27/28/29 is used for PWM6/7/8/9 */
       GPIO_PWMS_MAP_MODE2,		       /**<GPIO30/31/32/33 is used for PWM6/7/8/9 */
       GPIO_PWMS_MAP_MODE_MAX,		       /**< Invalid mode*/
} gpio_pwms_map_mode_t;

typedef enum {
       GPIO_SPDIF_MAP_MODE0 = 0,	       /**<GPIO11is used for SPDIF */
       GPIO_SPDIF_MAP_MODE1,		       /**<GPIO12 is used for SPDIF */
       GPIO_SPDIF_MAP_MODE2,		       /**<GPIO13 is used for SPDIF */
       GPIO_SPDIF_MAP_MODE_MAX,		       /**< Invalid mode*/
} gpio_spdif_map_mode_t;


bk_err_t gpio_dev_map(gpio_id_t gpio_id, gpio_dev_t dev);
bk_err_t gpio_dev_unmap(gpio_id_t gpio_id);
bk_err_t gpio_spi_sel(gpio_spi3_map_mode_t gpio_spi_sel_mode);
bk_err_t gpio_sdio_sel(gpio_sdio_map_mode_t gpio_sdio_sel_mode);
bk_err_t gpio_uart2_sel(gpio_uart2_map_mode_t mode);
bk_err_t gpio_pwms_sel(uint32 pwm_channel, gpio_pwms_map_mode_t mode);


