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
#include <os/mem.h>
#include "gpio_hal.h"
#include "gpio_map.h"
#include "gpio_driver.h"
#include "gpio_driver_base.h"
#include "icu_driver.h"
#include <driver/gpio_types.h>

extern gpio_driver_t s_gpio;


#define GPIO_RETURN_ON_INVALID_PERIAL_MODE(mode, mode_max) do {\
				if ((mode) >= (mode_max)) {\
					return BK_ERR_GPIO_SET_INVALID_FUNC_MODE;\
				}\
			} while(0)

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

bk_err_t gpio_i2c1_sel(gpio_i2c1_map_mode_t mode)
{
	GPIO_RETURN_ON_INVALID_PERIAL_MODE(mode, GPIO_I2C1_MAP_MODE_MAX);

	GPIO_MAP_TABLE(GPIO_I2C1_USED_GPIO_NUM, GPIO_I2C1_MAP_MODE_MAX, i2c1_gpio_map) = GPIO_I2C1_MAP_TABLE;

	gpio_hal_devs_map(&s_gpio.hal, i2c1_gpio_map[mode].gpio_bits, i2c1_gpio_map[mode].devs, GPIO_I2C1_USED_GPIO_NUM);

	GPIO_LOGI("Warning bk7256 USE PLIC  NOT icu\n");

	return BK_OK;
}

bk_err_t gpio_spi_sel(gpio_spi1_map_mode_t mode)
{
	GPIO_RETURN_ON_INVALID_PERIAL_MODE(mode, GPIO_SPI_MAP_MODE_MAX);

	GPIO_MAP_TABLE(GPIO_SPI0_USED_GPIO_NUM, GPIO_SPI_MAP_MODE_MAX, spi_gpio_map) = GPIO_SPI0_MAP_TABLE;

	gpio_hal_devs_map(&s_gpio.hal, spi_gpio_map[mode].gpio_bits, spi_gpio_map[mode].devs, GPIO_SPI0_USED_GPIO_NUM);

	GPIO_LOGI("Warning bk7256 USE PLIC  NOT icu\n");

	return BK_OK;
}

static gpio_jtag_map_mode_t gpio_jtag_default_mode = GPIO_JTAG_MAP_MODE;
bk_err_t gpio_jtag_sel(gpio_jtag_map_mode_t mode)
{
	GPIO_RETURN_ON_INVALID_PERIAL_MODE(mode, GPIO_JTAG_MAP_MODE_MAX);
	if(gpio_jtag_default_mode == mode)
		return BK_OK;
	else
		gpio_jtag_default_mode = mode;

	GPIO_MAP_TABLE(GPIO_JTAG_USED_GPIO_NUM, GPIO_JTAG_MAP_MODE_MAX, jtag_gpio_map) = GPIO_JTAG_MAP_TABLE;

	for(gpio_jtag_map_mode_t sel_mode = GPIO_JTAG_MAP_MODE; sel_mode < GPIO_JTAG_MAP_MODE_MAX; sel_mode++)
	{
		for(int gpio_index = 0; gpio_index < GPIO_NUM; gpio_index++)
		{
			if (jtag_gpio_map[sel_mode].gpio_bits & BIT64(gpio_index)) {
				GPIO_LOGI("gpio_jtag_sel unmap gpio_index = %d\r\n", gpio_index);
				gpio_hal_func_unmap(&s_gpio.hal, gpio_index);
			}
		}
	}

	gpio_hal_devs_map(&s_gpio.hal, jtag_gpio_map[mode].gpio_bits, jtag_gpio_map[mode].devs, GPIO_JTAG_USED_GPIO_NUM);

	GPIO_LOGD("Warning bk7256 USE PLIC  NOT icu\n");

	return BK_OK;
}

