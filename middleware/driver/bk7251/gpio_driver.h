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
	GPIO_SDIO_MAP_MODE0 = 0,		/**<GPIO14~GPIO19 is used for SDIO_DMA module */
	GPIO_SDIO_MAP_MODE1,			/**<GPIO14~GPIO19 is used for SDIO_HOST module*/
	GPIO_SDIO_MAP_MODE2,			/**<GPIO34~GPIO39 is used for SDIO_DMA module */
	GPIO_SDIO_MAP_MODE3,			/**<GPIO34~GPIO39 is used for SDIO_HOST module*/
	GPIO_SDIO_MAP_MODE_MAX,			/**< Invalid sdio mode */
} gpio_sdio_map_mode_t;


typedef enum {
	GPIO_SPI_MAP_MODE0 = 0,			/**<GPIO14~GPIO17 is used for SPI_DMA module */
	GPIO_SPI_MAP_MODE1,			/**<GPIO14~GPIO17 is used for SPI module*/
	GPIO_SPI_MAP_MODE2,			/**<GPIO30~GPIO33 is used for SPI_DMA module */
	GPIO_SPI_MAP_MODE3,			/**<GPIO30~GPIO33 is used for SPI module */
	GPIO_SPI_MAP_MODE_MAX,		       /**< Invalid spi mode */
} gpio_spi_map_mode_t;


bk_err_t gpio_dev_map(gpio_id_t gpio_id, gpio_dev_t dev);
bk_err_t gpio_dev_unmap(gpio_id_t gpio_id);
bk_err_t gpio_spi_sel(gpio_spi_map_mode_t gpio_spi_sel_mode);
bk_err_t gpio_sdio_sel(gpio_sdio_map_mode_t gpio_sdio_sel_mode);


