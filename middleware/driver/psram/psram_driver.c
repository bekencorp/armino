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

#include <driver/int.h>
#include <os/mem.h>
#include "clock_driver.h"
#include "sys_driver.h"
#include "psram_hal.h"
#include "aon_pmu_hal.h"
#include "driver/psram_types.h"


#define BK_ERR_PSRAM_SERVER_NOT_INIT       (BK_ERR_PSRAM_BASE - 1) /**< psram server not init */

static bool s_psram_server_is_init = false;

#define PSRAM_RETURN_ON_SERVER_NOT_INIT() do {\
				if (!s_psram_server_is_init) {\
					return BK_ERR_PSRAM_SERVER_NOT_INIT;\
				}\
			} while(0)

bk_err_t bk_psram_set_clk(psram_clk_t clk)
{
	bk_err_t ret = BK_OK;

	psram_hal_set_clk(clk);

	return ret;
}

bk_err_t bk_psram_set_voltage(psram_voltage_t voltage)
{
	bk_err_t ret = BK_OK;

	psram_hal_set_voltage(voltage);

	return ret;
}

bk_err_t bk_psram_set_transfer_mode(psram_tansfer_mode_t transfer_mode)
{
	bk_err_t ret = BK_OK;

	psram_hal_set_transfer_mode(transfer_mode);

	return ret;
}

bk_err_t bk_psram_init(void)
{
	if (s_psram_server_is_init) {
		return BK_OK;
	}

	// power up and clk config
	psram_hal_power_clk_enable(1);

	// psram config
	psram_hal_config_init();

	s_psram_server_is_init = true;

	return BK_OK;
}

bk_err_t bk_psram_deinit(void)
{
	if (!s_psram_server_is_init) {
		return BK_OK;
	}

	psram_hal_power_clk_enable(0);

	s_psram_server_is_init = false;

	return BK_OK;
}

bk_err_t bk_psram_resume(void)
{
	if (s_psram_server_is_init)
	{
		// power up and clk config
		psram_hal_power_clk_enable(1);

		// psram config
		psram_hal_config_init();
	}

	return BK_OK;
}

bk_err_t bk_psram_suspend(void)
{
	if (s_psram_server_is_init)
	{
		psram_hal_power_clk_enable(0);
	}

	return BK_OK;
}

void bk_psram_memcpy(uint32_t start_addr, uint32_t *data_buf, uint32_t len)
{
	psram_hal_continue_write(start_addr, data_buf, len);
}

void bk_psram_memread(uint32_t start_addr, uint32_t *data_buf, uint32_t len)
{
	psram_hal_continue_read(start_addr, data_buf, len);
}

char *bk_psram_strcat(char *start_addr, const char *data_buf)
{
#if (CONFIG_SOC_BK7256XX)
	return psram_strcat(start_addr, data_buf);
#else
	return NULL;
#endif
}



