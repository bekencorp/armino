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
#include "amp_lock_api.h"
#include <driver/gpio_types.h>

extern gpio_driver_t s_gpio;

#define GPIO_REG_LOCK_WAIT_TIME_MS  6
#define GPIO_RETURN_ON_INVALID_PERIAL_MODE(mode, mode_max) do {\
				if ((mode) >= (mode_max)) {\
					return BK_ERR_GPIO_SET_INVALID_FUNC_MODE;\
				}\
			} while(0)

bk_err_t gpio_dev_map(gpio_id_t gpio_id, gpio_dev_t dev)
{
	uint32_t ret_val = 1;
	
	ret_val = amp_res_acquire(AMP_RES_ID_GPIO, GPIO_REG_LOCK_WAIT_TIME_MS);
	GPIO_LOGD("amp_res_acquire:ret=%d\r\n", ret_val);
	if(ret_val != BK_OK)
		return ret_val;

	gpio_hal_func_map(&s_gpio.hal, gpio_id, dev);

	ret_val = amp_res_release(AMP_RES_ID_GPIO);
	GPIO_LOGD("amp res release:ret=%d\r\n", ret_val);
	if(ret_val != BK_OK)
		return ret_val;

	return BK_OK;
}

bk_err_t gpio_dev_unmap(gpio_id_t gpio_id)
{
	uint32_t ret_val = 1;

	ret_val = amp_res_acquire(AMP_RES_ID_GPIO, GPIO_REG_LOCK_WAIT_TIME_MS);
	GPIO_LOGD("amp_res_acquire:ret=%d\r\n", ret_val);
	if(ret_val != BK_OK)
		return ret_val;

	gpio_hal_func_unmap(&s_gpio.hal, gpio_id);

	ret_val = amp_res_release(AMP_RES_ID_GPIO);
	GPIO_LOGD("amp res release:ret=%d\r\n", ret_val);
	if(ret_val != BK_OK)
		return ret_val;

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

bk_err_t gpio_i2s_sel(gpio_i2s_map_mode_t mode)
{
	GPIO_RETURN_ON_INVALID_PERIAL_MODE(mode, GPIO_I2S_MAP_MODE_MAX);

	GPIO_MAP_TABLE(GPIO_I2S_USED_GPIO_NUM, GPIO_I2S_MAP_MODE_MAX, i2s_gpio_map) = GPIO_I2S_MAP_TABLE;

	gpio_hal_devs_map(&s_gpio.hal, i2s_gpio_map[mode].gpio_bits, i2s_gpio_map[mode].devs, GPIO_I2S_USED_GPIO_NUM);

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

bk_err_t gpio_sdio_sel(gpio_sdio_map_mode_t mode)
{
	GPIO_RETURN_ON_INVALID_PERIAL_MODE(mode, GPIO_SDIO_MAP_MODE_MAX);

	GPIO_MAP_TABLE(GPIO_SDIO_USED_GPIO_NUM, GPIO_SDIO_MAP_MODE_MAX, sdio_gpio_map) = GPIO_SDIO_MAP_TABLE;

	gpio_hal_devs_map(&s_gpio.hal, sdio_gpio_map[mode].gpio_bits, sdio_gpio_map[mode].devs, GPIO_SDIO_USED_GPIO_NUM);

	GPIO_LOGI("Warning bk7256 USE PLIC  NOT icu\n");
	//icu_sdio_gpio_sel(mode);

	return BK_OK;
}

bk_err_t gpio_sdio_one_line_sel(gpio_sdio_map_mode_t mode)
{
	GPIO_RETURN_ON_INVALID_PERIAL_MODE(mode, GPIO_SDIO_MAP_MODE_MAX);

	GPIO_MAP_TABLE(GPIO_SDIO_ONE_LINE_USED_GPIO_NUM, GPIO_SDIO_MAP_MODE_MAX, sdio_gpio_map) = GPIO_SDIO_ONE_LINE_MAP_TABLE;

	gpio_hal_devs_map(&s_gpio.hal, sdio_gpio_map[mode].gpio_bits, sdio_gpio_map[mode].devs, GPIO_SDIO_ONE_LINE_USED_GPIO_NUM);

	GPIO_LOGI("Warning bk7256 USE PLIC  NOT icu\n");
	//icu_sdio_gpio_sel(mode);

	return BK_OK;
}

