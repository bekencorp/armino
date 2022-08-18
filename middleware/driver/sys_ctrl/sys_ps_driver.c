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

void sys_drv_enter_deep_sleep(void *param)
{
	uint32_t int_level = rtos_disable_int();
	sys_hal_enter_deep_sleep(param);
	rtos_enable_int(int_level);
}

void sys_drv_enter_normal_sleep(uint32_t peri_clk)
{
	uint32_t int_level = rtos_disable_int();
	sys_hal_enter_normal_sleep(peri_clk);
	rtos_enable_int(int_level);
}

void sys_drv_enter_normal_wakeup()
{
	uint32_t int_level = rtos_disable_int();
	sys_hal_enter_normal_wakeup();
	rtos_enable_int(int_level);
}

void sys_drv_enter_low_voltage()
{
	uint32_t int_level = rtos_disable_int();
	sys_hal_enter_low_voltage();
	rtos_enable_int(int_level);
}

/*for low power function start*/
void sys_drv_module_power_ctrl(power_module_name_t module, power_module_state_t power_state)
{
	uint32_t int_level = rtos_disable_int();
	sys_hal_module_power_ctrl(module,power_state);
	rtos_enable_int(int_level);
}

void sys_drv_module_RF_power_ctrl (module_name_t module, power_module_state_t power_state)
{
	uint32_t int_level = rtos_disable_int();
	sys_hal_module_RF_power_ctrl (module, power_state);
	rtos_enable_int(int_level);
}

void sys_drv_core_bus_clock_ctrl(high_clock_module_name_t core, uint32_t clksel, uint32_t clkdiv, high_clock_module_name_t bus, uint32_t bus_clksel, uint32_t bus_clkdiv)
{
	uint32_t int_level = rtos_disable_int();
	sys_hal_core_bus_clock_ctrl(core, clksel, clkdiv, bus, bus_clksel, bus_clkdiv);
	rtos_enable_int(int_level);
}

void sys_drv_cpu0_main_int_ctrl(dev_clk_pwr_ctrl_t clock_state)
{
	uint32_t int_level = rtos_disable_int();
	sys_hal_cpu0_main_int_ctrl(clock_state);
	rtos_enable_int(int_level);
}

void sys_drv_cpu1_main_int_ctrl(dev_clk_pwr_ctrl_t clock_state)
{
	uint32_t int_level = rtos_disable_int();
	sys_hal_cpu1_main_int_ctrl(clock_state);
	rtos_enable_int(int_level);
}

void sys_drv_set_cpu1_boot_address_offset(uint32_t address_offset)
{
	uint32_t int_level = rtos_disable_int();
	sys_hal_set_cpu1_boot_address_offset(address_offset);
	rtos_enable_int(int_level);
}

void sys_drv_set_cpu1_reset(uint32_t reset_value)
{
	uint32_t int_level = rtos_disable_int();
	/* 1:reset release, means cpu1 start to run; 0:reset means cpu1 at reset status */
	sys_hal_set_cpu1_reset(reset_value);
	rtos_enable_int(int_level);
}

void sys_drv_enable_mac_wakeup_source()
{
 	uint32_t int_level = rtos_disable_int();

	sys_hal_enable_mac_wakeup_source();

	rtos_enable_int(int_level);
}

void sys_drv_enable_bt_wakeup_source()
{
	uint32_t int_level = rtos_disable_int();
	sys_hal_enable_bt_wakeup_source();
	rtos_enable_int(int_level);
}

void sys_drv_all_modules_clk_div_set(clk_div_reg_e reg, uint32_t value)
{
    sys_hal_all_modules_clk_div_set( reg, value);
}

uint32_t sys_drv_all_modules_clk_div_get(clk_div_reg_e reg)
{
	return sys_hal_all_modules_clk_div_get(reg);
}

void sys_drv_wakeup_interrupt_clear(wakeup_source_t interrupt_source)
{
    //sys_hal_wakeup_interrupt_clear(interrupt_source);
}

void sys_drv_wakeup_interrupt_set(wakeup_source_t interrupt_source)
{

}

void sys_drv_touch_wakeup_enable(uint8_t index)
{
    uint32_t int_level = rtos_disable_int();
    sys_hal_touch_wakeup_enable(index);
	rtos_enable_int(int_level);
}

void sys_drv_usb_wakeup_enable(uint8_t index)
{
    sys_hal_usb_wakeup_enable(index);
}

void sys_drv_cpu_clk_div_set(uint32_t core_index, uint32_t value)
{
	sys_hal_cpu_clk_div_set(core_index,value);
}

uint32_t sys_drv_cpu_clk_div_get(uint32_t core_index)
{
	return sys_hal_cpu_clk_div_get(core_index);
}

void sys_drv_low_power_hardware_init()
{
	sys_hal_low_power_hardware_init();
}

int32 sys_drv_lp_vol_set(uint32_t value)
{
	int32 ret = 0;
	ret = sys_hal_lp_vol_set(value);
	return ret;
}

uint32_t sys_drv_lp_vol_get()
{
	return sys_hal_lp_vol_get();
}
/*for low power function end*/

/*wake up control start*/
//sys_ctrl CMD: CMD_ARM_WAKEUP_ENABLE
void sys_drv_arm_wakeup_enable(uint32_t param)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_arm_wakeup_enable(param);

	rtos_enable_int(int_level);
}

//sys_ctrl CMD: CMD_ARM_WAKEUP_DISABLE
void sys_drv_arm_wakeup_disable(uint32_t param)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_arm_wakeup_disable(param);

	rtos_enable_int(int_level);
}

//sys_ctrl CMD: CMD_GET_ARM_WAKEUP
uint32_t sys_drv_get_arm_wakeup(void)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_get_arm_wakeup();
	rtos_enable_int(int_level);

	return ret;
}
int32 sys_drv_module_power_state_get(power_module_name_t module)
{
	return sys_hal_module_power_state_get(module);
}

/*wake up control end*/

