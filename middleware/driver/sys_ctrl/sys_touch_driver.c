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

/**  Touch Start **/
uint32_t sys_drv_touch_power_down(uint32_t enable)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_touch_power_down(enable);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_touch_sensitivity_level_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_touch_sensitivity_level_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_touch_scan_mode_enable(uint32_t enable)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_touch_scan_mode_enable(enable);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_touch_detect_threshold_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_touch_detect_threshold_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_touch_detect_range_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_touch_detect_range_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_touch_calib_enable(uint32_t enable)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_touch_calib_enable(enable);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_touch_manul_mode_calib_value_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_touch_manul_mode_calib_value_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_touch_manul_mode_enable(uint32_t enable)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_touch_manul_mode_enable(enable);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_touch_scan_mode_chann_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_touch_scan_mode_chann_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_touch_int_enable(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_touch_int_enable(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

/**  Touch End **/

