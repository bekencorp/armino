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

#include <components/log.h>
#include <os/mem.h>
#include <driver/int.h>
#include "gpio_hal.h"
#include "gpio_driver_base.h"

#define TAG "icu_example"

gpio_driver_t s_gpio;

static void cli_int_service_gpio_isr(void)
{
	gpio_hal_t *hal = &s_gpio.hal;
	gpio_interrupt_status_t gpio_status;
	int gpio_id;

	gpio_hal_get_interrupt_status(hal, &gpio_status);

	gpio_hal_clear_interrupt_status(hal, &gpio_status);

	for (gpio_id = 0; gpio_id < GPIO_NUM; gpio_id++) {
		if (gpio_hal_is_interrupt_triggered(hal, gpio_id, &gpio_status)) {
			BK_LOGI(TAG, "gpio change register  isr test :gpio[%d] isr\r\n", gpio_id);
		}
	}

}

int main(void)
{
	BK_LOG_ON_ERR(bk_int_isr_register(INT_SRC_GPIO, cli_int_service_gpio_isr, NULL));
	BK_LOGI(TAG, "gpio register isr changing test\n");

#if CONFIG_INT_STATIS
	int_statis_t *int_st = {0};
	int_st = bk_get_int_statis();
	BK_LOG_ON_ERR(bk_dump_int_statis(int_st));
	BK_LOGI(TAG, "icu get interrupt statistic\n");
#endif

	return 0;
}

