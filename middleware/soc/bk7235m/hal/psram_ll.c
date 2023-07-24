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

#include "sys_driver.h"
#include "psram_ll.h"
#include "psram_hal.h"


bk_err_t psram_hal_power_enable(uint8_t enable)
{
	return BK_FAIL;
}

bk_err_t psram_hal_config_init(void)
{
	return BK_FAIL;
}

bk_err_t psram_hal_clk_power_enable(uint8_t enable)
{
	return BK_FAIL;
}

void psram_hal_continue_write(uint32_t start_addr, uint32_t *data_buf, uint32_t len)
{
}

void psram_hal_set_clk(psram_clk_t clk)
{
}

void psram_hal_set_voltage(psram_voltage_t voltage)
{
}