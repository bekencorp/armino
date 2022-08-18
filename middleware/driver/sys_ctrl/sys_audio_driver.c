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

#include "sys_hal.h"
#include "sys_driver.h"
#include "sys_driver_common.h"

/**  Audio Start  **/

uint32_t  sys_drv_aud_select_clock(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_aud_select_clock(value);

	if(!ret)
		ret = sys_amp_res_release();
	rtos_enable_int(int_level);

	return ret;
}

uint32_t  sys_drv_aud_clock_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_aud_clock_en(value);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
	return ret;
}

uint32_t sys_drv_aud_vdd1v_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_vdd1v_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_vdd1v5_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_vdd1v5_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_mic1_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_mic1_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_mic2_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_mic2_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_audpll_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_audpll_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_aud_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_aud_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_dacdrv_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_dacdrv_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_bias_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_bias_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_dacr_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_dacr_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_dacl_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_dacl_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_rvcmd_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_rvcmd_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_lvcmd_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_lvcmd_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_micbias1v_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_micbias1v_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_micbias_trim_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_micbias_trim_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_mic_rst_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_mic_rst_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_mic1_gain_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_mic1_gain_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_mic2_gain_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_mic2_gain_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_int_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_int_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_sbc_int_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_sbc_int_en(value);

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_power_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_aud_power_en(value);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
	return ret;
}

/**  Audio End  **/

/**  I2S Start  **/

uint32_t sys_drv_apll_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_apll_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_cb_manu_val_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_cb_manu_val_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_ana_reg11_vsel_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_ana_reg11_vsel_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_ana_reg10_sdm_val_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_ana_reg10_sdm_val_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_ana_reg11_spi_trigger_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_ana_reg11_spi_trigger_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

/**  I2S End  **/


