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
#include "icu_driver.h"
#include <driver/gpio_types.h>


typedef struct {
	gpio_hal_t hal;
} gpio_driver_t;

extern gpio_driver_t s_gpio;

#define GPIO_RETURN_ON_INVALID_PERIAL_MODE(mode) do {\
				if ((mode) >= 2) {\
					return BK_ERR_GPIO_SET_INVALID_FUNC_MODE;\
				}\
			} while(0)

#define GPIO_RETURN_ON_INVALID_PWMS_MAP(mode) do {\
						if ((mode) >= GPIO_PWMS_MAP_MODE_MAX) {\
							return BK_ERR_GPIO_SET_INVALID_FUNC_MODE;\
						}\
					} while(0)

#define GPIO_RETURN_ON_INVALID_PWMS_CHAN(chan) do {\
		if (((chan) >= 10) ||((chan) <= 5)) {\
			return BK_ERR_GPIO_MAP_PWMS_CHAN;\
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

bk_err_t gpio_uart2_sel(gpio_uart2_map_mode_t mode)
{
	GPIO_RETURN_ON_INVALID_PERIAL_MODE(mode);

	GPIO_MAP_TABLE(GPIO_UART2_USED_GPIO_NUM, GPIO_UART2_MAP_MODE_MAX, uart2_gpio_map) = GPIO_UART2_MAP_TABLE;

	gpio_hal_devs_map(&s_gpio.hal, uart2_gpio_map[mode].gpio_bits, uart2_gpio_map[mode].devs, GPIO_UART2_USED_GPIO_NUM);

	icu_uart2_gpio_sel( mode);

	return BK_OK;
}

bk_err_t gpio_spi_sel(gpio_spi3_map_mode_t mode)
{
	GPIO_RETURN_ON_INVALID_PERIAL_MODE(mode);

	GPIO_MAP_TABLE(GPIO_SPI_USED_GPIO_NUM, GPIO_SPI_MAP_MODE_MAX, spi_gpio_map) = GPIO_SPI_MAP_TABLE;

	gpio_hal_devs_map(&s_gpio.hal, spi_gpio_map[mode].gpio_bits, spi_gpio_map[mode].devs, GPIO_SPI_USED_GPIO_NUM);

	icu_spi3_gpio_sel(mode);

	return BK_OK;
}


bk_err_t gpio_sdio_sel(gpio_sdio_map_mode_t mode)
{
	GPIO_RETURN_ON_INVALID_PERIAL_MODE(mode);

	GPIO_MAP_TABLE(GPIO_SDIO_USED_GPIO_NUM, GPIO_SDIO_MAP_MODE_MAX, sdio_gpio_map) = GPIO_SDIO_MAP_TABLE;

	gpio_hal_devs_map(&s_gpio.hal, sdio_gpio_map[mode].gpio_bits, sdio_gpio_map[mode].devs, GPIO_SDIO_USED_GPIO_NUM);

	icu_sdio_gpio_sel( mode);

	return BK_OK;
}

bk_err_t gpio_pwms_sel(uint32 pwm_channel, gpio_pwms_map_mode_t mode)
{
	GPIO_RETURN_ON_INVALID_PWMS_MAP(mode);
	GPIO_RETURN_ON_INVALID_PWMS_CHAN(pwm_channel);

	icu_pwms_gpio_sel(pwm_channel, mode);

	if (pwm_channel == 6) {
		GPIO_MAP_TABLE(GPIO_PWM6_USED_GPIO_NUM, GPIO_PWMS_MAP_MODE_MAX, pwm_gpio_map) = GPIO_PWM6_MAP_TABLE;

		gpio_hal_devs_map(&s_gpio.hal, pwm_gpio_map[mode].gpio_bits, pwm_gpio_map[mode].devs, GPIO_PWMS_USED_GPIO_NUM);
	} else if (pwm_channel == 7) {

		GPIO_MAP_TABLE(GPIO_PWM7_USED_GPIO_NUM, GPIO_PWMS_MAP_MODE_MAX, pwm_gpio_map) = GPIO_PWM7_MAP_TABLE;

		gpio_hal_devs_map(&s_gpio.hal, pwm_gpio_map[mode].gpio_bits, pwm_gpio_map[mode].devs, GPIO_PWMS_USED_GPIO_NUM);
	}else if (pwm_channel == 8) {

		GPIO_MAP_TABLE(GPIO_PWM8_USED_GPIO_NUM, GPIO_PWMS_MAP_MODE_MAX, pwm_gpio_map) = GPIO_PWM8_MAP_TABLE;

		gpio_hal_devs_map(&s_gpio.hal, pwm_gpio_map[mode].gpio_bits, pwm_gpio_map[mode].devs, GPIO_PWMS_USED_GPIO_NUM);
	}else {

		GPIO_MAP_TABLE(GPIO_PWM9_USED_GPIO_NUM, GPIO_PWMS_MAP_MODE_MAX, pwm_gpio_map) = GPIO_PWM9_MAP_TABLE;

		gpio_hal_devs_map(&s_gpio.hal, pwm_gpio_map[mode].gpio_bits, pwm_gpio_map[mode].devs, GPIO_PWMS_USED_GPIO_NUM);
	}

	return BK_OK;
}


