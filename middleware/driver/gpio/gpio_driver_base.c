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
#include "icu_driver.h"
#include <os/mem.h>
#include "gpio_hal.h"
#include "gpio_driver_base.h"
#include "gpio_driver.h"
#include <driver/gpio_types.h>
#include <driver/int.h>
#include "amp_lock_api.h"
#if (CONFIG_SYSTEM_CTRL)
#include "sys_driver.h"
#endif

gpio_driver_t s_gpio = {0};
static gpio_isr_t s_gpio_isr[SOC_GPIO_NUM] = {NULL};

#define GPIO_RETURN_ON_INVALID_ID(id) do {\
		if ((id) >= SOC_GPIO_NUM) {\
			return BK_ERR_GPIO_CHAN_ID;\
		}\
	} while(0)

#define GPIO_RETURN_ON_INVALID_INT_TYPE_MODE(mode) do {\
				if ((mode) >= GPIO_INT_TYPE_MAX) {\
					return BK_ERR_GPIO_INVALID_INT_TYPE;\
				}\
			} while(0)

#define GPIO_RETURN_ON_INVALID_IO_MODE(mode) do {\
				if (((mode)) >= GPIO_IO_INVALID) {\
					return BK_ERR_GPIO_INVALID_MODE;\
				}\
			} while(0)

#define GPIO_RETURN_ON_INVALID_PULL_MODE(mode) do {\
				if (((mode)) >= GPIO_PULL_INVALID) {\
					return BK_ERR_GPIO_INVALID_MODE;\
				}\
			} while(0)

static void gpio_isr(void);

bk_err_t bk_gpio_driver_init(void)
{
	os_memset(&s_gpio, 0, sizeof(s_gpio));

	gpio_hal_disable_jtag_mode(&s_gpio.hal);

	bk_int_isr_register(INT_SRC_GPIO, gpio_isr, NULL);

	gpio_hal_init(&s_gpio.hal);

	amp_res_init(AMP_RES_ID_GPIO);

	return BK_OK;
}

bk_err_t bk_gpio_driver_deinit(void)
{
#if (CONFIG_SYSTEM_CTRL)
	sys_drv_int_group2_disable(GPIO_INTERRUPT_CTRL_BIT);
#else
	icu_disable_gpio_interrupt();
#endif

	return BK_OK;
}


bk_err_t bk_gpio_enable_output(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_output_enable(&s_gpio.hal, gpio_id, 1);
}

bk_err_t bk_gpio_disable_output(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_output_enable(&s_gpio.hal, gpio_id, 0);
}

bk_err_t bk_gpio_enable_input(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_input_enable(&s_gpio.hal, gpio_id, 1);
}

bk_err_t bk_gpio_disable_input(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_input_enable(&s_gpio.hal, gpio_id, 0);
}

bk_err_t bk_gpio_enable_pull(gpio_id_t gpio_id )
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_pull_enable(&s_gpio.hal, gpio_id, 1);
}

bk_err_t bk_gpio_disable_pull(gpio_id_t gpio_id )
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_pull_enable(&s_gpio.hal, gpio_id, 0);
}

bk_err_t bk_gpio_pull_up(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	gpio_hal_pull_enable(&s_gpio.hal, gpio_id, 1);
	return gpio_hal_pull_up_enable(&s_gpio.hal, gpio_id, 1);
}

bk_err_t bk_gpio_pull_down(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	gpio_hal_pull_enable(&s_gpio.hal, gpio_id, 1);
	return gpio_hal_pull_up_enable(&s_gpio.hal, gpio_id, 0);
}



bk_err_t bk_gpio_set_output_high(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_set_output_value(&s_gpio.hal, gpio_id, 1);
}

bk_err_t bk_gpio_set_output_low(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_set_output_value(&s_gpio.hal, gpio_id, 0);
}

bool bk_gpio_get_input(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_get_iutput(&s_gpio.hal, gpio_id);
}

//MAX capactiy:3
bool bk_gpio_set_capacity(gpio_id_t gpio_id, uint32 capacity)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_set_capacity(&s_gpio.hal, gpio_id, capacity);
}

bk_err_t bk_gpio_set_config(gpio_id_t gpio_id, const gpio_config_t *config)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);
	GPIO_RETURN_ON_INVALID_IO_MODE(config->io_mode);
	GPIO_RETURN_ON_INVALID_PULL_MODE(config->pull_mode);

	return gpio_hal_set_config(&s_gpio.hal, gpio_id, config);
}


/* Enable GPIO  interrupt.
 */
bk_err_t bk_gpio_register_isr(gpio_id_t gpio_id, gpio_isr_t isr)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);
	s_gpio_isr[gpio_id] = isr;

	return BK_OK;
}

bk_err_t bk_gpio_enable_interrupt(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	if(!gpio_hal_enable_interrupt(&s_gpio.hal, gpio_id)) {
		GPIO_LOGI("Warning bk7256 USE PLIC  NOT icu\n");
#if (CONFIG_SYSTEM_CTRL)
		sys_drv_int_group2_enable(GPIO_INTERRUPT_CTRL_BIT);
#else
		icu_enable_gpio_interrupt();
#endif
	}

	return BK_OK;
}

bk_err_t bk_gpio_disable_interrupt(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	gpio_hal_disable_interrupt(&s_gpio.hal, gpio_id);

	GPIO_LOGI("Warning bk7256 USE PLIC  NOT icu\n");
#if (CONFIG_SYSTEM_CTRL)
	sys_drv_int_group2_disable(GPIO_INTERRUPT_CTRL_BIT);
#else
	icu_disable_gpio_interrupt();
#endif
	return BK_OK;
}


bk_err_t bk_gpio_set_interrupt_type(gpio_id_t gpio_id, gpio_int_type_t type)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);
	GPIO_RETURN_ON_INVALID_INT_TYPE_MODE(type);

	return gpio_hal_set_int_type(&s_gpio.hal, gpio_id, type);
}


static void gpio_isr(void)
{
	gpio_hal_t *hal = &s_gpio.hal;
	gpio_interrupt_status_t gpio_status;
	int gpio_id;

	gpio_hal_get_interrupt_status(hal, &gpio_status);

	gpio_hal_clear_interrupt_status(hal, &gpio_status);

	for (gpio_id = 0; gpio_id < GPIO_NUM; gpio_id++) {
		if (gpio_hal_is_interrupt_triggered(hal, gpio_id, &gpio_status)) {
			if (s_gpio_isr[gpio_id]) {
				GPIO_LOGD("gpio int: index:%d \r\n",gpio_id);
				s_gpio_isr[gpio_id](gpio_id);
			}
		}
	}

}

bk_err_t bk_gpio_reg_save(uint32_t*  gpio_cfg)
{
    return gpio_hal_reg_save(gpio_cfg);
}

bk_err_t bk_gpio_reg_restore(uint32_t*  gpio_cfg)
{
    return gpio_hal_reg_restore(gpio_cfg);
}

bk_err_t bk_gpio_wakeup_enable(int64_t index, uint64_t type_l, uint64_t type_h)
{
    return gpio_hal_wakeup_enable( index, type_l, type_h);
}

bk_err_t bk_gpio_wakeup_interrupt_clear()
{
    return gpio_hal_wakeup_interrupt_clear();
}


