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

#include <common/bk_include.h>
#include "bk_intc.h"
#include "bk_icu.h"
#include "gpio_hal.h"
#include "gpio_map.h"
#include "gpio_driver.h"
#include <driver/gpio_types.h>


typedef struct {
	gpio_hal_t hal;
} gpio_driver_t;

extern gpio_driver_t s_gpio;

#define GPIO_RETURN_ON_INVALID_PERIAL_MODE(mode) do {\
				if ((mode) >= 4) {\
					return BK_ERR_GPIO_SET_INVALID_FUNC_MODE;\
				}\
			} while(0)

bk_err_t gpio_set_sdio_map(gpio_hal_t *hal, gpio_sdio_map_mode_t mode)
{
	gpio_ll_sdio_set_mode(hal->hw, mode);

	return BK_OK;
}

bk_err_t gpio_set_spi_map(gpio_hal_t *hal, gpio_spi_map_mode_t mode)
{
	gpio_ll_spi_set_mode(hal->hw, mode);

	return BK_OK;
}


bk_err_t gpio_dev_map(gpio_id_t gpio_id, gpio_dev_t dev)
{
	gpio_hal_func_map(&s_gpio.hal, gpio_id, dev);

	return BK_OK;
}

bk_err_t gpio_dev_unmap(gpio_id_t gpio_id)
{
	gpio_hal_func_unmap(&s_gpio.hal, gpio_id);

	return BK_OK;
}

bk_err_t gpio_spi_sel(gpio_spi_map_mode_t mode)
{
	GPIO_RETURN_ON_INVALID_PERIAL_MODE(mode);

	GPIO_MAP_TABLE(GPIO_SPI_USED_GPIO_NUM, GPIO_SPI_MAP_MODE_MAX, spi_gpio_map) = GPIO_SPI_MAP_TABLE;

	gpio_hal_devs_map(&s_gpio.hal, spi_gpio_map[mode].gpio_bits, spi_gpio_map[mode].devs, GPIO_SPI_USED_GPIO_NUM);

	gpio_set_spi_map(&s_gpio.hal, mode);

	return BK_OK;
}


bk_err_t gpio_sdio_sel(gpio_sdio_map_mode_t mode)
{
	GPIO_RETURN_ON_INVALID_PERIAL_MODE(mode);

	GPIO_MAP_TABLE(GPIO_SDIO_USED_GPIO_NUM, GPIO_SDIO_MAP_MODE_MAX, sdio_gpio_map) = GPIO_SDIO_MAP_TABLE;

	gpio_hal_devs_map(&s_gpio.hal, sdio_gpio_map[mode].gpio_bits, sdio_gpio_map[mode].devs, GPIO_SDIO_USED_GPIO_NUM);

	gpio_set_sdio_map(&s_gpio.hal, mode);

	return BK_OK;
}

