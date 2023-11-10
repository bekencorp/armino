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

#include <driver/sys_pm_types.h>
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
int32 sys_drv_bandgap_cali_set(uint32_t value)//increase or decrease the dvdddig voltage
{
	sys_hal_bandgap_cali_set(value);
	return 0;
}
uint32_t sys_drv_bandgap_cali_get()
{
	return sys_hal_bandgap_cali_get();
}
bk_err_t sys_drv_switch_cpu_bus_freq(pm_cpu_freq_e cpu_bus_freq)
{
    return sys_hal_switch_cpu_bus_freq(cpu_bus_freq);
}
bk_err_t sys_drv_core_bus_clock_ctrl(uint32_t cksel_core, uint32_t ckdiv_core,uint32_t ckdiv_bus, uint32_t ckdiv_cpu0,uint32_t ckdiv_cpu1)
{
    return sys_hal_core_bus_clock_ctrl(cksel_core, ckdiv_core,ckdiv_bus, ckdiv_cpu0,ckdiv_cpu1);
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
int32 sys_drv_rosc_calibration(uint32_t rosc_cali_mode, uint32_t cali_interval)
{
	return sys_hal_rosc_calibration(rosc_cali_mode,cali_interval);
}

int sys_drv_rosc_test_mode(bool enabled)
{
#if CONFIG_ROSC_TEST_MODE
	return sys_hal_rosc_test_mode(enabled);
#else
	return BK_FAIL;
#endif
}
/*wake up control end*/

int sys_pm_set_buck(sys_buck_type_t buck, bool ena)
{
	return sys_hal_set_buck(buck, ena);
}

int sys_pm_set_buck_pfm(sys_buck_type_t buck, bool ena)
{
	return sys_hal_set_buck_pfm(buck, ena);
}

int sys_pm_set_buck_burst(sys_buck_type_t buck, bool ena)
{
	return sys_hal_set_buck_burst(buck, ena);
}

int sys_pm_set_buck_mpo(sys_buck_type_t buck, bool ena)
{
	return sys_hal_set_buck_mpo(buck, ena);
}

int sys_pm_set_ldo_self_lp(sys_ldo_type_t ldo, bool ena)
{
	return sys_hal_set_ldo_self_lp(ldo, ena);
}

int sys_pm_set_ldo_current_limit(sys_ldo_type_t ldo, bool ena)
{
	return sys_hal_set_ldo_current_limit(ldo, ena);
}

int sys_pm_set_aon_power(sys_aon_power_t power)
{
	return sys_hal_set_aon_power(power);
}

int sys_pm_set_aon_ldo_volt(uint32_t volt)
{
	return sys_hal_set_aon_ldo_volt(volt);
}

int sys_pm_set_io_ldo_volt(uint32_t volt)
{
	return sys_hal_set_io_ldo_volt(volt);
}

int sys_pm_set_ana_ldo_volt(bool trsw_ena, uint32_t rx_volt, uint32_t tx_volt)
{
	return sys_hal_set_ana_ldo_volt(trsw_ena, rx_volt, tx_volt);
}

int sys_pm_set_digital_ldo_volt(bool lp_ena, uint32_t low_volt, uint32_t high_volt)
{
	return sys_hal_set_digital_ldo_volt(lp_ena, low_volt, high_volt);
}

int sys_pm_set_core_ldo_volt(bool lp_ena, uint32_t low_volt, uint32_t high_volt)
{
	return sys_hal_set_core_ldo_volt(lp_ena, low_volt, high_volt);
}

int sys_pm_set_lv_ctrl_pd(bool ena)
{
	return sys_hal_set_lv_ctrl_pd(ena);
}

int sys_pm_set_lv_ctrl_hf(bool ena)
{
	return sys_hal_set_lv_ctrl_hf(ena);
}

int sys_pm_set_lv_ctrl_flash(bool ena)
{
	return sys_hal_set_lv_ctrl_flash(ena);
}

int sys_pm_set_lv_ctrl_core(bool ena)
{
	return sys_hal_set_lv_ctrl_core(ena);
}

int sys_pm_set_lpo_src(sys_lpo_src_t src)
{
	return sys_hal_set_lpo_src(src);
}

void sys_pm_dump_ctrl(void)
{
	sys_hal_dump_ctrl();
}

void sys_pm_set_power(power_module_name_t module, power_module_state_t state)
{
	sys_drv_module_power_ctrl(module, state);
}
uint32_t sys_pm_get_power(power_module_name_t module)
{
	return sys_drv_module_power_state_get(module);
}
