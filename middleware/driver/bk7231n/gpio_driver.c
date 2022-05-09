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

typedef struct {
	gpio_hal_t hal;
} gpio_driver_t;

extern gpio_driver_t s_gpio;



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



