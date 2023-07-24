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
#include "sys_hal.h"
#include "sys_ll.h"
#include "aon_pmu_hal.h"
#include "gpio_hal.h"
#include "gpio_driver_base.h"
#include "sys_types.h"
#include <driver/aon_rtc.h>
#include <driver/hal/hal_spi_types.h>
#include "gpio_hal.h"
#include "timer_hal.h"

#define PM_CLKSEL_CORE_320M        (2)
#define PM_CLKSEL_CORE_480M        (3)
#define PM_CLKDIV_CORE_0           (0)
#define PM_CLKDIV_CORE_1           (1)
#define PM_VDDDIG_H_VOL_0v9        (0xC)
#define PM_CLKDV_CPU1_1            (0x1)
static sys_hal_t s_sys_hal;
uint32 sys_hal_get_int_group2_status(void);
bk_err_t sys_hal_ctrl_vdddig_h_vol(uint32_t vol_value);
uint32_t sys_hal_vdddig_h_vol_get();
/**  Platform Start **/
//Platform


/** Platform Misc Start **/
bk_err_t sys_hal_init()
{
	s_sys_hal.hw = (sys_hw_t *)SYS_LL_REG_BASE;
	return BK_OK;
}
/** Platform Misc End **/


void sys_hal_usb_enable_clk(bool en)
{
	sys_ll_set_cpu_device_clk_enable_usb_cken(en);
}

void sys_hal_usb_analog_phy_en(bool en)
{
	sys_ll_set_ana_reg5_en_usb(en);
}

void sys_hal_usb_analog_speed_en(bool en)
{
	sys_ll_set_ana_reg5_usb_speed(en);
}

void sys_hal_usb_analog_ckmcu_en(bool en)
{
	//sys_ll_set_ana_reg5_en_dpll(en);
}

void sys_hal_usb_analog_deepsleep_en(bool en)
{
	sys_ll_set_ana_reg5_spideepsleep(en);
}

void sys_hal_set_usb_analog_dp_capability(uint8_t capability)
{
	sys_ll_set_ana_reg3_usbpen(capability);
}

void sys_hal_set_usb_analog_dn_capability(uint8_t capability)
{
	sys_ll_set_ana_reg3_usbnen(capability);
}

void sys_hal_usb_enable_charge(bool en)
{
	//sys_ll_set_ana_reg5_vctrl_dpllldo(en);
}

void sys_hal_usb_charge_vlcf_cal()
{

}
void sys_hal_usb_charge_icp_cal()
{}
void sys_hal_usb_charge_vcv_cal()
{}
void sys_hal_usb_charge_get_cal()
{}

/** Platform PWM Start **/

/** Platform PWM End **/

void sys_hal_flash_set_dco(void)
{
	sys_ll_set_cpu_clk_div_mode2_cksel_flash(FLASH_CLK_DPLL);
}

void sys_hal_flash_set_dpll(void)
{
	sys_ll_set_cpu_clk_div_mode2_cksel_flash(FLASH_CLK_APLL);
}

void sys_hal_flash_set_clk(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode2_cksel_flash(value);
}

void sys_hal_flash_set_clk_div(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode2_ckdiv_flash(value);
}

/* REG_0x09:cpu_clk_div_mode2->cksel_flash:0:XTAL  1:APLL  1x :clk_120M,R/W,0x9[25:24]*/
uint32_t sys_hal_flash_get_clk_sel(void)
{
	return sys_ll_get_cpu_clk_div_mode2_cksel_flash();
}

/* REG_0x09:cpu_clk_div_mode2->ckdiv_flash:0:/1  1:/2  2:/4  3:/8,R/W,0x9[27:26]*/
uint32_t sys_hal_flash_get_clk_div(void)
{
	return sys_ll_get_cpu_clk_div_mode2_ckdiv_flash();
}

/** Flash end **/

/*for low power function start*/
void sys_hal_module_power_ctrl(power_module_name_t module,power_module_state_t power_state)
{
	uint32_t value = 0;

	if((module >= POWER_MODULE_NAME_MEM1) && (module <= POWER_MODULE_NAME_ROM_PGEN)) {
		value = 0;
		value = sys_ll_get_cpu_power_sleep_wakeup_value();
		if(power_state == POWER_MODULE_STATE_ON) {
			value &= ~(1 << module);
        	} else {
			value |= (1 << module);
		}
		sys_ll_set_cpu_power_sleep_wakeup_value(value);
	} else if(module == POWER_MODULE_NAME_CPU1) {

		if(power_state == POWER_MODULE_STATE_ON) {
			//power on and then support clock
			sys_ll_set_cpu1_int_halt_clk_op_cpu1_pwr_dw(POWER_MODULE_STATE_ON);
			sys_ll_set_cpu1_int_halt_clk_op_cpu1_halt(0);

			//wait halt really cleared,clock support finish
			for(int i = 0; i < 1000; i++);
		} else {
			//un-support clock and then power down
			sys_ll_set_cpu1_int_halt_clk_op_cpu1_halt(1);
			//here should wait halt really finish and then power down
			for(int i = 0; i < 1000; i++);

			sys_ll_set_cpu1_int_halt_clk_op_cpu1_pwr_dw(POWER_MODULE_STATE_OFF);
		}
	} else if(module == POWER_MODULE_NAME_TCM1_PGEN) {
		if(power_state == POWER_MODULE_STATE_ON) {
			sys_ll_set_cpu_power_sleep_wakeup_tcm1_pgen(0);
		} else {
			sys_ll_set_cpu_power_sleep_wakeup_tcm1_pgen(1);
		}
	} else {
		;//do something
	}
}

int32 sys_hal_module_power_state_get(power_module_name_t module)
{
	uint32_t value = 0;

	if((module >= POWER_MODULE_NAME_MEM1) && (module <= POWER_MODULE_NAME_ROM_PGEN)) {
		value = sys_ll_get_cpu_power_sleep_wakeup_value();
		value = ((value >> module) & 0x1);
		return value;
	} else if(module == POWER_MODULE_NAME_CPU1) {
		return sys_ll_get_cpu_current_run_status_cpu1_pwr_dw_state();
	} else if (module == POWER_MODULE_NAME_TCM1_PGEN) {
		return sys_ll_get_cpu_power_sleep_wakeup_tcm1_pgen();
	}

	return 0;
}

int sys_hal_rosc_calibration(uint32_t rosc_cali_mode, uint32_t cali_interval)
{
	if (rosc_cali_mode == 0) { //Auto
		sys_ll_set_ana_reg6_spi_trig(1);
		sys_ll_set_ana_reg6_calib_interval(cali_interval);
		sys_ll_set_ana_reg6_modifi_auto(0);
		sys_ll_set_ana_reg6_modify_interval(0x20);
		sys_ll_set_ana_reg6_xtal_wakeup_time(8);
		sys_ll_set_ana_reg6_cal_mode(1); //1: 32K, 0: 31.25K
		sys_ll_set_ana_reg6_calib_auto(1);
	} else if (rosc_cali_mode == 1) { //Manual
		sys_ll_set_ana_reg6_manu_ena(0);
		sys_ll_set_ana_reg6_spi_trig(0);
		sys_ll_set_ana_reg6_spi_trig(1);
	} else if (rosc_cali_mode == 2) { //Disable
	} else {
	}

	return BK_OK;
}

int sys_hal_rosc_test_mode(bool enabled)
{
	if (enabled) {
		sys_ll_set_ana_reg4_ck_tst_enbale(1);
		sys_ll_set_ana_reg4_cktst_sel(0);
		sys_ll_set_ana_reg5_rosc_tsten(1);
		sys_ll_set_ana_reg6_cal_mode(1);
		//sys_ll_set_ana_reg6_calib_auto(0);
		//sys_ll_set_ana_reg6_calib_auto(1);
		REG_WRITE((SOC_AON_GPIO_REG_BASE + (24 << 2)), 0x40);
	} else {
		sys_ll_set_ana_reg4_ck_tst_enbale(0);
		sys_ll_set_ana_reg4_cktst_sel(0);
		sys_ll_set_ana_reg5_rosc_tsten(0);
		sys_ll_set_ana_reg6_cal_mode(0);
		sys_ll_set_ana_reg6_calib_auto(0);
	}
	return BK_OK;
}

void sys_hal_module_RF_power_ctrl (module_name_t module,power_module_state_t power_state)
{
    uint32_t value = 0;
	value = sys_ll_get_ana_reg6_value();
    if(power_state == POWER_MODULE_STATE_ON)//power on
    {
		//value |= ((1 << SYS_ANA_REG6_EN_SYSLDO_POS)|(1 << SYS_ANA_REG6_EN_DPLL_POS)|(1 << SYS_ANA_REG6_EN_TEMPDET_POS));//en_sysldo,en_dpll
		//value &= ~(1 << SYS_ANA_REG6_XTAL_LPMODE_CTRL_POS);//when using the xtal as the 32k,it need close the xtal low power mode
		//value |= (1 << 11);//en_audpll //temp close,we will open when be neeeded
		//value |= (1 << 8);//en_dco     //now no module using,temp close,we will open when be neeeded
		//value |= (1 << 7);//en_xtall   //now no module using,temp close,we will open when be neeeded
    }
	else //power down
	{
		value &= ~(1 << 12);//en_dpll
		value &= ~(1 << 11);//en_audpll
		value &= ~(1 << 8);//en_dco
		value &= ~(1 << 7);//en_xtall
	}

	sys_ll_set_ana_reg6_value(value);

}
void sys_hal_cpu0_main_int_ctrl(dev_clk_pwr_ctrl_t clock_state)
{
    sys_ll_set_cpu0_int_halt_clk_op_cpu0_int_mask( clock_state);
}
void sys_hal_cpu1_main_int_ctrl(dev_clk_pwr_ctrl_t clock_state)
{
    sys_ll_set_cpu1_int_halt_clk_op_cpu1_int_mask( clock_state);
}
void sys_hal_set_cpu1_boot_address_offset(uint32_t address_offset)
{
    sys_ll_set_cpu1_int_halt_clk_op_cpu1_offset(address_offset);
}
void sys_hal_set_cpu1_reset(uint32_t reset_value)
{
    /*1:reset ; 0:not reset*/
    sys_ll_set_cpu1_int_halt_clk_op_cpu1_sw_rst(reset_value);
}
void sys_hal_all_modules_clk_div_set(clk_div_reg_e reg, uint32_t value)
{
    clk_div_address_map_t clk_div_address_map_table[] = CLK_DIV_ADDRESS_MAP;
    clk_div_address_map_t *clk_div_addr = &clk_div_address_map_table[reg];

    uint32_t clk_div_reg_address = clk_div_addr->reg_address;

	REG_WRITE(clk_div_reg_address, value);
}
uint32_t sys_hal_all_modules_clk_div_get(clk_div_reg_e reg)
{
    clk_div_address_map_t clk_div_address_map_table[] = CLK_DIV_ADDRESS_MAP;
    clk_div_address_map_t *clk_div_addr = &clk_div_address_map_table[reg];

	 uint32_t clk_div_reg_address = clk_div_addr->reg_address;

	return REG_READ(clk_div_reg_address);
}
void sys_hal_cpu_clk_div_set(uint32_t core_index, uint32_t value)
{
	if(core_index == 0)
	{
		//sys_ll_set_cpu0_int_halt_clk_op_cpu0_clk_div(value);
	}
	else
	{
		sys_ll_set_cpu1_int_halt_clk_op_cpu0_bus_clk_2div(value);
	}
}
uint32_t sys_hal_cpu_clk_div_get(uint32_t core_index)
{
	if(core_index == 0)
	{
		//return sys_ll_get_cpu0_int_halt_clk_op_cpu0_clk_div();
		uint32_t reg_value;
    	reg_value = REG_READ(0x44010000+0x4*4);
    	reg_value = ((reg_value >> 4) & 0xf);
    	return reg_value;
	}
	else
	{
		//return sys_ll_get_cpu1_int_halt_clk_op_cpu0_bus_clk_2div();
		uint32_t reg_value;
    	reg_value = REG_READ(0x44010000+0x5*4);
    	reg_value = ((reg_value >> 4) & 0xf);
    	return reg_value;
	}
}
int32 sys_hal_lp_vol_set(uint32_t value)
{
	sys_ll_set_ana_reg9_spi_latch1v(1);
	sys_ll_set_ana_reg9_vcorelsel(value);
	sys_ll_set_ana_reg9_spi_latch1v(0);

	return 0;
}
uint32_t sys_hal_lp_vol_get()
{
	return sys_ll_get_ana_reg9_vcorelsel();
}
int32 sys_hal_rf_tx_vol_set(uint32_t value)
{
	sys_ll_set_ana_reg9_spi_latch1v(1);
	sys_ll_set_ana_reg8_t_vanaldosel(value);
	sys_ll_set_ana_reg9_spi_latch1v(0);
	return 0;
}
uint32_t sys_hal_rf_tx_vol_get()
{
	return sys_ll_get_ana_reg8_t_vanaldosel();
}
int32 sys_hal_rf_rx_vol_set(uint32_t value)
{
	sys_ll_set_ana_reg9_spi_latch1v(1);
	sys_ll_set_ana_reg8_r_vanaldosel(value);
	sys_ll_set_ana_reg9_spi_latch1v(0);
	return 0;
}
uint32_t sys_hal_rf_rx_vol_get()
{
	return sys_ll_get_ana_reg8_r_vanaldosel();
}
int32 sys_hal_bandgap_cali_set(uint32_t value)//increase or decrease the dvdddig voltage
{
	return 0;
}
uint32_t sys_hal_bandgap_cali_get()
{
	return 0;
}
bk_err_t sys_hal_core_bus_clock_ctrl(uint32_t cksel_core, uint32_t ckdiv_core,uint32_t ckdiv_bus, uint32_t ckdiv_cpu0,uint32_t ckdiv_cpu1)
{
	uint32_t clk_param  = 0;
	uint32_t     h_vol  = 0;
	if(cksel_core > 3)
	{
		os_printf("set dvfs cksel core > 3 invalid %d\r\n",cksel_core);
		return BK_FAIL;
	}

	if((ckdiv_core > PM_FREQUNCY_DIV_MAX) || (ckdiv_bus > PM_FREQUNCY_DIV_BUS_MAX)||(ckdiv_cpu1 > PM_FREQUNCY_DIV_CPU1_MAX))
	{
		os_printf("set dvfs ckdiv_core ckdiv_bus ckdiv_cpu0  ckdiv_cpu0  > 15 invalid\r\n");
		return BK_FAIL;
	}
	if((cksel_core == PM_CLKSEL_CORE_320M)&&(ckdiv_core == PM_CLKDIV_CORE_0)&&(ckdiv_cpu1 != PM_CLKDV_CPU1_1))
	{
		os_printf("unsupport the cpu freq setting %d %d %d\r\n",cksel_core,ckdiv_core,ckdiv_cpu1);
		return BK_FAIL;
	}

	if((cksel_core == PM_CLKSEL_CORE_480M)&&(ckdiv_core == PM_CLKDIV_CORE_0))
	{
		os_printf("unsupport the cpu freq setting %d %d %d\r\n",cksel_core,ckdiv_core,ckdiv_cpu1);
		return BK_FAIL;
	}

	if((cksel_core == PM_CLKSEL_CORE_480M)&&(ckdiv_core == PM_CLKDIV_CORE_1)&&(ckdiv_cpu1 != PM_CLKDV_CPU1_1))
	{
		os_printf("unsupport the cpu freq setting %d %d %d\r\n",cksel_core,ckdiv_core,ckdiv_cpu1);
		return BK_FAIL;
	}

	if((cksel_core == PM_CLKSEL_CORE_320M)&&(ckdiv_core == PM_CLKDIV_CORE_0))
	{
		h_vol = sys_hal_vdddig_h_vol_get();
		if(h_vol < PM_VDDDIG_H_VOL_0v9)
		{
			sys_hal_ctrl_vdddig_h_vol(PM_VDDDIG_H_VOL_0v9);
		}
	}
	clk_param = 0;
	clk_param = sys_hal_all_modules_clk_div_get(CLK_DIV_REG0);
	if(((clk_param >> 0x4)&0x3) > cksel_core)//when it from the higher frequency to lower frequency
	{
		/*1.core clk select*/
		clk_param = 0;
		clk_param = sys_hal_all_modules_clk_div_get(CLK_DIV_REG0);
		clk_param &=  ~(0x3 << 4);
		clk_param |=  cksel_core << 4;
		sys_hal_all_modules_clk_div_set(CLK_DIV_REG0,clk_param);

		/*2.config bus and core clk div*/
		clk_param = 0;
		clk_param = sys_hal_all_modules_clk_div_get(CLK_DIV_REG0);
		clk_param &=  ~((0x1 << 6)|(0xF << 0));
		clk_param |=  ckdiv_core << 0;
		clk_param |=  (ckdiv_bus&0x1) << 6;
		sys_hal_all_modules_clk_div_set(CLK_DIV_REG0,clk_param);

		/*3.config cpu clk div*/
		sys_hal_cpu_clk_div_set(1,ckdiv_cpu1);

	}
	else//when it from the lower frequency to higher frequency
	{
		/*1.config bus and core clk div*/
		clk_param = 0;
		clk_param = sys_hal_all_modules_clk_div_get(CLK_DIV_REG0);
		clk_param &=  ~(0xF << 0);
		clk_param |=  ckdiv_core << 0;
		sys_hal_all_modules_clk_div_set(CLK_DIV_REG0,clk_param);

		clk_param = 0;
		clk_param = sys_hal_all_modules_clk_div_get(CLK_DIV_REG0);
		clk_param &=  ~(0x1 << 6);
		clk_param |=  (ckdiv_bus&0x1) << 6;
		sys_hal_all_modules_clk_div_set(CLK_DIV_REG0,clk_param);

		/*2.config cpu clk div*/
		sys_hal_cpu_clk_div_set(1,ckdiv_cpu1);

		/*3.core clk select*/

		clk_param = 0;
		clk_param = sys_hal_all_modules_clk_div_get(CLK_DIV_REG0);
		clk_param &=  ~(0x3 << 4);
		clk_param |=  cksel_core << 4;
		sys_hal_all_modules_clk_div_set(CLK_DIV_REG0,clk_param);
	}

	return BK_OK;
}
bk_err_t sys_hal_ctrl_vdddig_h_vol(uint32_t vol_value)
{
	sys_ll_set_ana_reg9_spi_latch1v(1);
	sys_ll_set_ana_reg9_vcorehsel(vol_value);
	sys_ll_set_ana_reg9_spi_latch1v(0);
	return BK_OK;
}
uint32_t sys_hal_vdddig_h_vol_get()
{
	return sys_ll_get_ana_reg9_vcorehsel();
}
/*
	//For BK7256 ckdiv_bus is 0x8[6]
	//For BK7236 ckdiv_bus is the same as ckdiv_cpu1 0x5[4]

	//cpu0:160m;cpu1:160m;bus:160m
	setcpufreq 3 2 0 0 0

	//cpu0:160m;cpu1:320m;bus:160m
	setcpufreq 2 0 0 0 1

	//cpu0:120m;cpu1:120m;bus:120m
	setcpufreq 3 3 0 0 0

	//cpu0:120m;cpu1:240m;bus:120m
	setcpufreq 3 1 0 0 1
*/
bk_err_t sys_hal_switch_cpu_bus_freq(pm_cpu_freq_e cpu_bus_freq)
{
    bk_err_t ret = BK_OK;
    switch(cpu_bus_freq)
    {
		case PM_CPU_FRQ_320M://cpu0:160m;cpu1:320m;bus:160m
		   sys_hal_ctrl_vdddig_h_vol(0xC);//0.9V
		   ret = sys_hal_core_bus_clock_ctrl(0x2,0x0,0x0,0x0,0x1);
			break;
		case PM_CPU_FRQ_240M://cpu0:120m;cpu1:240m;bus:120m
		    sys_hal_ctrl_vdddig_h_vol(0x8);//0.8V
			ret = sys_hal_core_bus_clock_ctrl(0x3,0x1,0x0,0x0,0x1);
			break;
		case PM_CPU_FRQ_120M://cpu0:120m;cpu1:120m;bus:120m
		    sys_hal_ctrl_vdddig_h_vol(0x8);//0.8V
			ret = sys_hal_core_bus_clock_ctrl(0x3,0x3,0x0,0x0,0x0);
			break;
		case PM_CPU_FRQ_80M://cpu0:80m;cpu1:80m;bus:80m
		    sys_hal_ctrl_vdddig_h_vol(0x7);//0.775V
			ret = sys_hal_core_bus_clock_ctrl(0x3,0x5,0x0,0x0,0x0);
			break;
		case PM_CPU_FRQ_60M://cpu0:60m;cpu1:60m;bus:60m
		    sys_hal_ctrl_vdddig_h_vol(0x6);//0.75V
			ret = sys_hal_core_bus_clock_ctrl(0x3,0x7,0x0,0x0,0x0);
			break;
		case PM_CPU_FRQ_26M://cpu0:26m;cpu1:26m;bus:26m
		    sys_hal_ctrl_vdddig_h_vol(0x5);//0.725V
			ret = sys_hal_core_bus_clock_ctrl(0x0,0x0,0x0,0x0,0x0);
			break;
		default:
			break;
    }

	return ret;
}

/*for low power function end*/
/*sleep feature end*/

uint32 sys_hal_get_chip_id(void)
{
	return sys_ll_get_version_id_versionid();
}

uint32 sys_hal_get_device_id(void)
{
	return sys_ll_get_device_id_deviceid();
}


int32 sys_hal_int_disable(uint32 param) //CMD_ICU_INT_DISABLE
{
	uint32 reg = 0;
	uint32 value = 0;

#if !CONFIG_SLAVE_CORE
	reg = sys_ll_get_cpu0_int_0_31_en_value();
	value = reg;
	reg &= ~(param);
	sys_ll_set_cpu0_int_0_31_en_value(reg);
#else
	reg = sys_ll_get_cpu1_int_0_31_en_value();
	value = reg;
	reg &= ~(param);
	sys_ll_set_cpu1_int_0_31_en_value(reg);
#endif

	return value;
}

int32 sys_hal_int_enable(uint32 param) //CMD_ICU_INT_ENABLE
{
	uint32 reg = 0;

#if !CONFIG_SLAVE_CORE
	reg = sys_ll_get_cpu0_int_0_31_en_value();
	reg |= (param);
	sys_ll_set_cpu0_int_0_31_en_value(reg);
#else
	reg = sys_ll_get_cpu1_int_0_31_en_value();
	reg |= (param);
	sys_ll_set_cpu1_int_0_31_en_value(reg);
#endif

	return 0;
}

//NOTICE:Temp add for BK7256 product which has more then 32 Interrupt sources
int32 sys_hal_int_group2_disable(uint32 param)
{
	uint32 reg = 0;
	uint32 value = 0;

#if !CONFIG_SLAVE_CORE
	reg = sys_ll_get_cpu0_int_32_63_en_value();
	value = reg;
	reg &= ~(param);
	sys_ll_set_cpu0_int_32_63_en_value(reg);
#else
	reg = sys_ll_get_cpu1_int_32_63_en_value();
	value = reg;
	reg &= ~(param);
	sys_ll_set_cpu1_int_32_63_en_value(reg);
#endif

	return value;
}

//NOTICE:Temp add for BK7256 product which has more then 32 Interrupt sources
int32 sys_hal_int_group2_enable(uint32 param)
{
	uint32 reg = 0;

#if !CONFIG_SLAVE_CORE
	reg = sys_ll_get_cpu0_int_32_63_en_value();
	reg |= (param);
	sys_ll_set_cpu0_int_32_63_en_value(reg);
#else
	reg = sys_ll_get_cpu1_int_32_63_en_value();
	reg |= (param);
	sys_ll_set_cpu1_int_32_63_en_value(reg);
#endif

	return 0;
}

int32 sys_hal_fiq_disable(uint32 param)
{
	uint32 reg = 0;
	uint32 value = 0;

#if !CONFIG_SLAVE_CORE
	reg = sys_ll_get_cpu0_int_32_63_en_value();
	value = reg;
	reg &= ~(param);
	sys_ll_set_cpu0_int_32_63_en_value(reg);
#else
	reg = sys_ll_get_cpu1_int_32_63_en_value();
	value = reg;
	reg &= ~(param);
	sys_ll_set_cpu1_int_32_63_en_value(reg);
#endif

	return value;
}

int32 sys_hal_fiq_enable(uint32 param)
{
	uint32 reg = 0;

#if !CONFIG_SLAVE_CORE
	reg = sys_ll_get_cpu0_int_32_63_en_value();
	reg |= (param);
	sys_ll_set_cpu0_int_32_63_en_value(reg);
#else
	reg = sys_ll_get_cpu1_int_32_63_en_value();
	reg |= (param);
	sys_ll_set_cpu1_int_32_63_en_value(reg);
#endif

	return 0;
}

//	GLOBAL_INT_DECLARATION();	GLOBAL_INT_DISABLE();
int32 sys_hal_global_int_disable(uint32 param)
{
	int32 ret = 0;

	return ret;
}

// GLOBAL_INT_RESTORE();
int32 sys_hal_global_int_enable(uint32 param)
{
	int32 ret = 0;

	return ret;
}

uint32 sys_hal_get_int_status(void)
{
#if !CONFIG_SLAVE_CORE
	return sys_ll_get_cpu0_int_0_31_status_value();
#else
	return sys_ll_get_cpu1_int_0_31_status_value();
#endif
}

uint32 sys_hal_get_int_group2_status(void)
{
#if !CONFIG_SLAVE_CORE
	return sys_ll_get_cpu0_int_32_63_status_value();
#else
	return sys_ll_get_cpu1_int_32_63_status_value();
#endif
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_gpio_int_st: ,R,0x29[22]*/
uint32_t sys_hal_get_cpu0_gpio_int_st(void)
{
    return sys_ll_get_cpu0_int_32_63_status_cpu0_gpio_int_st();
}

//NOTICE:INT source status is read only and can't be set, other projects is error, we'll delete them.
int32 sys_hal_set_int_status(uint32 param)
{
	return 0;
}

uint32 sys_hal_get_fiq_reg_status(void)
{
	uint32 reg = 0;

#if !CONFIG_SLAVE_CORE
	reg = sys_ll_get_cpu0_int_32_63_status_value();
#else
	reg = sys_ll_get_cpu1_int_32_63_status_value();
#endif

	return reg;
}

uint32 sys_hal_set_fiq_reg_status(uint32 param)
{
	uint32 reg = 0;

	///TODO:this reg is read only

	return reg;
}

uint32 sys_hal_get_intr_raw_status(void)
{
	uint32 reg = 0;

	///TODO:
	reg = sys_ll_get_cpu0_int_0_31_status_value();

	return reg;
}

uint32 sys_hal_set_intr_raw_status(uint32 param)
{
	uint32 reg = 0;

	///TODO:this reg is read only

	return reg;
}

int32 sys_hal_set_jtag_mode(uint32 param)
{
	int32 ret = 0;
	sys_ll_set_cpu_storage_connect_op_select_jtag_core_sel(param);
	return ret;
}

uint32 sys_hal_get_jtag_mode(void)
{
	uint32 reg = 0;
	reg = sys_ll_get_cpu_storage_connect_op_select_jtag_core_sel();
	return reg;
}

/* NOTICE: NOTICE: NOTICE: NOTICE: NOTICE: NOTICE: NOTICE
 * BK7256 clock, power is different with previous products(2022-01-10).
 * Previous products peripheral devices use only one signal of clock enable.
 * BK7256 uses clock and power signal to control one device,
 * This function only enable clock signal, we needs to enable power signal also
 * if we want to enable one device.
 */
void sys_hal_clk_pwr_ctrl(dev_clk_pwr_id_t dev, dev_clk_pwr_ctrl_t power_up)
{
	uint32_t v = 0;
	uint32_t offset = 0;

	if (dev >= CLK_PWR_ID_H264) {
		offset += CLK_PWR_ID_H264;
		v = sys_ll_get_reserver_reg0xd_value();
	} else {
		offset = 0;
		v = sys_ll_get_cpu_device_clk_enable_value();
	}

	if(CLK_PWR_CTRL_PWR_UP == power_up)
		v |= (1 << (dev - offset));
	else
		v &= ~(1 << (dev - offset));

	if (dev >= CLK_PWR_ID_H264) {
		sys_ll_set_reserver_reg0xd_value(v);
	} else {
		sys_ll_set_cpu_device_clk_enable_value(v);
	}
}

/* UART select clock **/
void sys_hal_uart_select_clock(uart_id_t id, uart_src_clk_t mode)
{
	int sel_xtal = 0;
	int sel_appl = 1;

	switch(id)
	{
		case UART_ID_0:
			{
				if(mode == UART_SCLK_APLL)
					sys_ll_set_cpu_clk_div_mode1_clksel_uart0(sel_appl);
				else
					sys_ll_set_cpu_clk_div_mode1_clksel_uart0(sel_xtal);
				break;
			}
		case UART_ID_1:
			{
				if(mode == UART_SCLK_APLL)
					sys_ll_set_cpu_clk_div_mode1_cksel_uart1(sel_appl);
				else
					sys_ll_set_cpu_clk_div_mode1_cksel_uart1(sel_xtal);
				break;
			}
		case UART_ID_2:
			{
				if(mode == UART_SCLK_APLL)
					sys_ll_set_cpu_clk_div_mode1_cksel_uart2(sel_appl);
				else
					sys_ll_set_cpu_clk_div_mode1_cksel_uart2(sel_xtal);
				break;
			}
		default:
			break;
	}

}

void sys_hal_pwm_select_clock(sys_sel_pwm_t num, pwm_src_clk_t mode)
{
	int sel_clk32 = 0;
	int sel_xtal = 1;

	switch(num)
	{
		case SYS_SEL_PWM0:
			if(mode == PWM_SCLK_XTAL)
				sys_ll_set_cpu_clk_div_mode1_cksel_pwm0(sel_xtal);
			else
				sys_ll_set_cpu_clk_div_mode1_cksel_pwm0(sel_clk32);
			break;
		case SYS_SEL_PWM1:
			if(mode == PWM_SCLK_XTAL)
				sys_ll_set_cpu_clk_div_mode1_cksel_pwm1(sel_xtal);
			else
				sys_ll_set_cpu_clk_div_mode1_cksel_pwm1(sel_clk32);
			break;

		default:
			break;
	}
}

void sys_hal_timer_select_clock(sys_sel_timer_t num, timer_src_clk_t mode)
{
	int sel_clk32 = 0;
	int sel_xtal = 1;

	switch(num)
	{
		case SYS_SEL_TIMER0:
			if(mode == TIMER_SCLK_XTAL)
				sys_ll_set_cpu_clk_div_mode1_cksel_tim0(sel_xtal);
			else
				sys_ll_set_cpu_clk_div_mode1_cksel_tim0(sel_clk32);
			break;
		case SYS_SEL_TIMER1:
			if(mode == TIMER_SCLK_XTAL)
				sys_ll_set_cpu_clk_div_mode1_cksel_tim1(sel_xtal);
			else
				sys_ll_set_cpu_clk_div_mode1_cksel_tim1(sel_clk32);
			break;

		default:
			break;
	}
}

uint32_t sys_hal_timer_select_clock_get(sys_sel_timer_t id)
{
    uint32_t ret = 0;

    switch(id)
    {
        case SYS_SEL_TIMER0:
        {
            ret = sys_ll_get_cpu_clk_div_mode1_cksel_tim0();
            break;
        }
        case SYS_SEL_TIMER1:
        {
            ret = sys_ll_get_cpu_clk_div_mode1_cksel_tim1();
            break;
        }
        default:
            break;
    }

    ret = (ret)?TIMER_SCLK_XTAL:TIMER_SCLK_CLK32;

    return ret;
}

void sys_hal_spi_select_clock(spi_id_t num, spi_src_clk_t mode)
{
	int sel_xtal = 0;
	int sel_apll = 1;

	switch(num)
	{
		case SPI_ID_0:
			if(mode == SPI_CLK_XTAL)
				sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi0(sel_xtal);
			else
				sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi0(sel_apll);
			break;
#if (SOC_SPI_UNIT_NUM > 1)
		case SPI_ID_1:
			if(mode == SPI_CLK_XTAL)
				sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi1(sel_xtal);
			else
				sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi1(sel_apll);
			break;
#endif
		default:
			break;
	}
}

void sys_hal_qspi_clk_sel(uint32_t param)
{

}

void sys_hal_qspi_set_src_clk_div(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode2_ckdiv_qspi0(value);
}

#if 1	//tmp build
void sys_hal_set_clk_select(dev_clk_select_id_t dev, dev_clk_select_t clk_sel)
{
	//tmp build
}

dev_clk_select_t sys_hal_get_clk_select(dev_clk_select_id_t dev)
{
	//tmp build
	return 0;
}

//DCO divider is valid for all of the peri-devices.
void sys_hal_set_dco_div(dev_clk_dco_div_t div)
{
	//tmp build
}

//DCO divider is valid for all of the peri-devices.
dev_clk_dco_div_t sys_hal_get_dco_div(void)
{
	//tmp build
	return 0;
}
#endif	//temp build

/*clock power control end*/

void sys_hal_set_cksel_sadc(uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_cksel_sadc(value);
}

void sys_hal_set_cksel_pwm0(uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_cksel_pwm0(value);
}

void sys_hal_set_cksel_pwm1(uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_cksel_pwm1(value);
}

void sys_hal_set_cksel_pwm(uint32_t value)
{
    sys_hal_set_cksel_pwm0(value);
    sys_hal_set_cksel_pwm1(value);
}

uint32_t sys_hal_uart_select_clock_get(uart_id_t id)
{
    uint32_t ret = 0;

    switch(id)
    {
        case UART_ID_0:
        {
            ret = sys_ll_get_cpu_clk_div_mode1_clksel_uart0();
            break;
        }
        case UART_ID_1:
        {
            ret = sys_ll_get_cpu_clk_div_mode1_cksel_uart1();
            break;
        }
        case UART_ID_2:
        {
            ret = sys_ll_get_cpu_clk_div_mode1_cksel_uart1();
            break;
        }
        default:
            break;
    }

    ret = (!ret)?UART_SCLK_XTAL_26M:UART_SCLK_APLL;

    return ret;
}

void sys_hal_sadc_int_enable(void)
{
    sys_hal_int_enable(SADC_INTERRUPT_CTRL_BIT);
}

void sys_hal_sadc_int_disable(void)
{
    sys_hal_int_disable(SADC_INTERRUPT_CTRL_BIT);
}

void sys_hal_sadc_pwr_up(void)
{
	sys_ll_set_cpu_device_clk_enable_sadc_cken(1);
}

void sys_hal_sadc_pwr_down(void)
{
	sys_ll_set_cpu_device_clk_enable_sadc_cken(0);
}

void sys_hal_set_clksel_spi0(uint32_t value)
{
    sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi0(value);
}

void sys_hal_set_clksel_spi1(uint32_t value)
{
    sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi1(value);
}

void sys_hal_set_clksel_spi(uint32_t value)
{
    if((SPI_CLK_SRC_XTAL == value) || (SPI_CLK_SRC_APLL == value))
    {
        sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi0(value);
        sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi1(value);
    }
    else
    {
        //os_printf("spi cksel is not support on BK7256 in function:%s, line:%d\n", __FUNCTION__, __LINE__);
    }

}

void sys_hal_en_tempdet(uint32_t value)
{
    sys_ll_set_ana_reg5_en_temp(value);
}

uint32_t sys_hal_mclk_mux_get(void)
{
	UINT32 ret = 0;

	ret = sys_ll_get_cpu_clk_div_mode1_cksel_core();

	return ret;
}

void sys_hal_mclk_mux_set(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode1_cksel_core(value);
}

uint32_t sys_hal_mclk_div_get(void)
{
	UINT32 ret = 0;

	ret = sys_ll_get_cpu_clk_div_mode1_clkdiv_core();

	return ret;
}

void sys_hal_mclk_div_set(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode1_clkdiv_core(value);
}

/**  Platform End **/




/**  BT Start **/
//BT
void sys_hal_bt_power_ctrl(bool power_up)
{
    if (power_up)
    {
        sys_ll_set_cpu_power_sleep_wakeup_pwd_btsp(0);
    }
    else
    {
        sys_ll_set_cpu_power_sleep_wakeup_pwd_btsp(1);
    }
}

void sys_hal_bt_clock_ctrl(bool en)
{
    if (en)
    {
        sys_ll_set_cpu_device_clk_enable_btdm_cken(1);
    }
    else
    {
        sys_ll_set_cpu_device_clk_enable_btdm_cken(0);
    }
}

void sys_hal_xvr_clock_ctrl(bool en)
{
    if (en)
    {
        sys_ll_set_cpu_device_clk_enable_xvr_cken(1);
    }
    else
    {
        sys_ll_set_cpu_device_clk_enable_xvr_cken(0);
    }
}

void sys_hal_btdm_interrupt_ctrl(bool en)
{
    if (en)
    {
        sys_ll_set_cpu0_int_32_63_en_cpu0_dm_irq_en(1);
            //TODO enable PLIC Int Enable Registers
    }
    else
    {
        sys_ll_set_cpu0_int_32_63_en_cpu0_dm_irq_en(0);
            //TODO disable PLIC Int Enable Registers
    }
}

void sys_hal_ble_interrupt_ctrl(bool en)
{
    if (en)
    {
        sys_ll_set_cpu0_int_32_63_en_cpu0_ble_irq_en(1);
            //TODO enable PLIC Int Enable Registers
    }
    else
    {
        sys_ll_set_cpu0_int_32_63_en_cpu0_ble_irq_en(0);
            //TODO disable PLIC Int Enable Registers
    }
}

void sys_hal_bt_interrupt_ctrl(bool en)
{
    if (en)
    {
        sys_ll_set_cpu0_int_32_63_en_cpu0_bt_irq_en(1);
            //TODO enable PLIC Int Enable Registers
    }
    else
    {
        sys_ll_set_cpu0_int_32_63_en_cpu0_bt_irq_en(0);
            //TODO disable PLIC Int Enable Registers
    }
}

void sys_hal_bt_rf_ctrl(bool en)
{
    if (en)
    {
        //todo
    }
    else
    {
        //todo
    }
}

uint32_t sys_hal_bt_rf_status_get(void)
{
    //todo
    return 0;
}

void sys_hal_bt_sleep_exit_ctrl(bool en)
{
    if (en)
    {
        sys_ll_set_cpu_power_sleep_wakeup_bts_soft_wakeup_req(1);
    }
    else
    {
        sys_ll_set_cpu_power_sleep_wakeup_bts_soft_wakeup_req(0);
    }
}

#if 1
void sys_hal_set_bts_wakeup_platform_en(bool value)
{
	sys_ll_set_cpu_power_sleep_wakeup_bts_wakeup_platform_en(value);
}

uint32 sys_hal_get_bts_wakeup_platform_en()
{
	return sys_ll_get_cpu_power_sleep_wakeup_bts_wakeup_platform_en();
}
#endif

/**  BT End **/




/**  Audio Start **/
//Audio
/**  Audio End **/




/**  Video Start **/
/**
  * @brief	lcd_disp  system config
  * param1: clk source sel 0:clk_320M      1:clk_480M,
  * param2: clk_div  F/(1+clkdiv_disp_l+clkdiv_disp_h*2)
  * param1: int_en eanble lcd cpu int
  * param2: clk_always_on, BUS_CLK ENABLE,0: bus clock open when module is select,1:bus clock always open,  0 by defult
  * return none
  */
void sys_hal_lcd_disp_clk_en(uint8_t clk_src_sel, uint8_t clk_div_l, uint8_t clk_div_h, uint8_t clk_always_on)
{
	sys_ll_set_cpu_clk_div_mode1_clkdiv_disp_l(clk_div_l);
	sys_ll_set_cpu_clk_div_mode2_clkdiv_disp_h(clk_div_h);
	sys_ll_set_cpu_clk_div_mode2_cksel_disp( clk_src_sel);
//	sys_ll_set_cpu1_int_0_31_en_cpu1_lcd_display_int_en( 1);
//	sys_ll_set_cpu0_int_0_31_en_cpu0_lcd_display_int_en(int_en);
	sys_ll_set_cpu_device_clk_enable_disp_cken(1);
}

/**
  * @brief	lcd clk close and int disable, reg value recover default.
  * return none
  */
void sys_hal_lcd_disp_close(void)
{
	sys_ll_set_cpu_clk_div_mode1_clkdiv_disp_l(0);
	sys_ll_set_cpu_clk_div_mode2_clkdiv_disp_h(0);
	sys_ll_set_cpu_clk_div_mode2_cksel_disp(0);
	//sys_ll_set_cpu1_int_0_31_en_cpu1_lcd_int_en(&s_sys_hal, 1);
	//sys_ll_set_cpu0_int_0_31_en_cpu0_lcd_int_en(0);
	sys_ll_set_cpu_device_clk_enable_disp_cken(0);
	//sys_ll_set_cpu_mode_disckg2_disp_disckg(0);
}

/**
  * @brief	dma2d system config
  * param1: clk source sel 0:clk_320M	   1:clk_480M,
  * param2: clk_always_on  ENABLE,0: bus clock auto open when module is select,1:bus clock always open
  * param1: int_en eanble lcd cpu int
  * return none
  */
void sys_hal_dma2d_clk_en(uint8_t clk_always_on)
{
	//sys_ll_set_cpu_mode_disckg2_dma2d_disckg(clk_always_on);
//	sys_ll_set_cpu0_int_0_31_en_cpu0_dma2d_int_en(sys_int_en);
}

void sys_hal_set_jpeg_dec_disckg(uint32_t value)
{
	//sys_ll_set_cpu_mode_disckg1_jpeg_dec_disckg(value);
}


/**  Video End **/




/**  WIFI Start **/
//WIFI

//Yantao Add Start
void sys_hal_modem_core_reset(void)
{
	//TODO, 7256 NO modem core reset
}

void sys_hal_mpif_invert(void)
{
	//TODO, 7256 NO mpif_invert
}

void sys_hal_modem_subsys_reset(void)
{
	//TODO, 7256 NO subsys reset
}
void sys_hal_mac_subsys_reset(void)
{
	//TODO, 7256 NO subsys reset
}
void sys_hal_usb_subsys_reset(void)
{
	//TODO, 7256 NO subsys reset
}
void sys_hal_dsp_subsys_reset(void)
{
	//TODO, 7256 NO subsys reset
}
void sys_hal_mac_power_ctrl(bool power_up)
{
	//WARNING:the low-level is power-down
	sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_mac(!(power_up));
}

void sys_hal_modem_power_ctrl(bool power_up)
{
	sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_phy(!(power_up));
}

void sys_hal_pta_ctrl(bool pta_en)
{
	//TODO, 7256 NO pta enable
}

void sys_hal_modem_bus_clk_ctrl(bool clk_en)
{
	//TODO, 7256 no bus clock enable
}

void sys_hal_modem_clk_ctrl(bool clk_en)
{
	sys_ll_set_cpu_device_clk_enable_phy_cken(clk_en);
}


void sys_hal_mac_bus_clk_ctrl(bool clk_en)
{
	//TODO, 7256 no bus clock enable
}

void sys_hal_mac_clk_ctrl(bool clk_en)
{
	sys_ll_set_cpu_device_clk_enable_mac_cken(clk_en);
}



void sys_hal_set_vdd_value(uint32_t param)
{
	sys_hal_ctrl_vdddig_h_vol(param);
}

uint32_t sys_hal_get_vdd_value(void)
{
	//TODO reg0x43 Write only
	return sys_ll_get_ana_reg9_vcorehsel();
}

//CMD_SCTRL_BLOCK_EN_MUX_SET
void sys_hal_block_en_mux_set(uint32_t param)
{
	//TODO 7256 no block en mux
}
void sys_hal_enable_mac_gen_int(void)
{
	sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_gen_en(1);
}
void sys_hal_enable_mac_prot_int(void)
{
	sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_prot_trigger_en(1);
}
void sys_hal_enable_mac_tx_trigger_int(void)
{
	sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_trigger_en(1);
}
void sys_hal_enable_mac_rx_trigger_int(void)
{
	sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_rx_trigger_en(1);
}
void sys_hal_enable_mac_txrx_misc_int(void)
{
	sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_rx_misc_en(1);
}
void sys_hal_enable_mac_txrx_timer_int(void)
{
	sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_timer_en(1);
}

void sys_hal_enable_modem_int(void)
{
	sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_int_phy_mpb_en(1);
}
void sys_hal_enable_modem_rc_int(void)
{
	sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_int_phy_riu_en(1);
}

void sys_hal_enable_hsu_int(void)
{
	sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_hsu_irq_en(1);
}


void sys_hal_disable_hsu_int(void)
{
	sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_hsu_irq_en(0);
}

//Yantao Add End

void sys_hal_cali_dpll_spi_trig_disable(void)
{
    sys_ll_set_ana_reg0_spitrig(0);
}

void sys_hal_cali_dpll_spi_trig_enable(void)
{
    sys_ll_set_ana_reg0_spitrig(1);
}

void sys_hal_cali_dpll_spi_detect_disable(void)
{
    sys_ll_set_ana_reg0_spideten(0);
}

void sys_hal_cali_dpll_spi_detect_enable(void)
{
    sys_ll_set_ana_reg0_spideten(1);
}

void sys_hal_set_xtalh_ctune(uint32_t value)
{
    sys_ll_set_ana_reg2_xtalh_ctune(value);
}

void sys_hal_analog_set(analog_reg_t reg, uint32_t value)
{
    uint32_t analog_reg_address;

    if ((reg < ANALOG_REG0) || (reg >= ANALOG_MAX)) {
        return;
    }

    analog_reg_address = SYS_ANA_REG0_ADDR + (reg - ANALOG_REG0) * 4;

	sys_ll_set_analog_reg_value(analog_reg_address, value);
}
uint32_t sys_hal_analog_get(analog_reg_t reg)
{
    uint32_t analog_reg_address;

    if ((reg < ANALOG_REG0) || (reg >= ANALOG_MAX)) {
        return 0;
    }

    analog_reg_address = SYS_ANA_REG0_ADDR + (reg - ANALOG_REG0) * 4;

	return sys_ll_get_analog_reg_value(analog_reg_address);
}
void sys_hal_set_ana_reg1_value(uint32_t value)
{
    sys_ll_set_ana_reg1_value(value);
}

void sys_hal_set_ana_reg2_value(uint32_t value)
{
    sys_ll_set_ana_reg2_value(value);
}

void sys_hal_set_ana_reg3_value(uint32_t value)
{
    sys_ll_set_ana_reg3_value(value);
}

void sys_hal_set_ana_reg4_value(uint32_t value)
{
    sys_ll_set_ana_reg4_value(value);
}

void sys_hal_set_ana_reg12_value(uint32_t value)
{
    sys_ll_set_ana_reg12_value(value);
}

void sys_hal_set_ana_reg13_value(uint32_t value)
{
    sys_ll_set_ana_reg13_value(value);
}

void sys_hal_set_ana_reg14_value(uint32_t value)
{
    sys_ll_set_ana_reg14_value(value);
}

void sys_hal_set_ana_reg15_value(uint32_t value)
{
    sys_ll_set_ana_reg15_value(value);
}

void sys_hal_set_ana_reg16_value(uint32_t value)
{
    sys_ll_set_ana_reg16_value(value);
}

void sys_hal_set_ana_reg17_value(uint32_t value)
{
    sys_ll_set_ana_reg17_value(value);
}

void sys_hal_set_ana_reg18_value(uint32_t value)
{
    sys_ll_set_ana_reg18_value(value);
}

void sys_hal_set_ana_reg19_value(uint32_t value)
{
    sys_ll_set_ana_reg19_value(value);
}

void sys_hal_set_ana_reg20_value(uint32_t value)
{
    sys_ll_set_ana_reg20_value(value);
}

void sys_hal_set_ana_reg21_value(uint32_t value)
{
    sys_ll_set_ana_reg21_value(value);
}

uint32_t sys_hal_bias_reg_read(void)
{
	///TODO
    return 0;
}
uint32_t sys_hal_bias_reg_write(uint32_t param)
{
	///TODO

	return 0;

}

uint32_t sys_hal_analog_reg2_get(void)
{
	///TODO

    return 0;
}

uint32_t sys_hal_bias_reg_set(uint32_t param)
{
	///TODO

	return 0;
}

uint32_t sys_hal_bias_reg_clean(uint32_t param)
{
	///TODO

	return 0;
}


uint32_t sys_hal_get_xtalh_ctune(void)
{
    return sys_ll_get_ana_reg2_xtalh_ctune();
}

uint32_t sys_hal_get_bgcalm(void)
{
    //	TODO
    //return sys_ll_get_ana_reg5_bgcalm();
    return 0;
}

void sys_hal_set_bgcalm(uint32_t value)
{
    //sys_ll_set_ana_reg5_bgcalm(value);
}

void sys_hal_set_audioen(uint32_t value)
{
	//sys_ll_set_ana_reg11_audioen(value);
}

void sys_hal_set_dpll_div_cksel(uint32_t value)
{
    //sys_ll_set_ana_reg11_cksel(value);
}

void sys_hal_set_dpll_reset(uint32_t value)
{
    //sys_ll_set_ana_reg11_reset(value);
}

void sys_hal_set_gadc_ten(uint32_t value)
{
    //sys_ll_set_ana_reg7_gadc_ten(value);
}
/**  WIFI End **/

/**  Audio Start  **/

void sys_hal_aud_select_clock(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode1_cksel_aud(value);
}

void sys_hal_aud_clock_en(uint32_t value)
{
	sys_ll_set_cpu_device_clk_enable_aud_cken(value);
}

void sys_hal_aud_mic1_en(uint32_t value)
{
	sys_ll_set_ana_reg19_micen(value);
}

void sys_hal_aud_dacl_en(uint32_t value)
{
	sys_ll_set_ana_reg20_daclen(value);
}

void sys_hal_aud_diffen_en(uint32_t value)
{
	sys_ll_set_ana_reg20_diffen(value);
}

void sys_hal_aud_mic_rst_set(uint32_t value)
{
	sys_ll_set_ana_reg19_rst(value);
}

void sys_hal_aud_mic1_gain_set(uint32_t value)
{
	sys_ll_set_ana_reg19_micgain(value);
}

void sys_hal_aud_dcoc_en(uint32_t value)
{
	sys_ll_set_ana_reg20_lendcoc(value);
}

void sys_hal_aud_lmdcin_set(uint32_t value)
{
	sys_ll_set_ana_reg21_lmdcin(value);
}

void sys_hal_aud_audbias_en(uint32_t value)
{
	sys_ll_set_ana_reg18_enaudbias(value);
}

void sys_hal_aud_adcbias_en(uint32_t value)
{
	sys_ll_set_ana_reg18_enadcbias(value);
}

void sys_hal_aud_micbias_en(uint32_t value)
{
	sys_ll_set_ana_reg18_enmicbias(value);
}

void sys_hal_aud_bias_en(uint32_t value)
{
	sys_ll_set_ana_reg21_enbs(value);
}

void sys_hal_aud_idac_en(uint32_t value)
{
	sys_ll_set_ana_reg21_enidacl(value);
}

void sys_hal_aud_dacdrv_en(uint32_t value)
{
	sys_ll_set_ana_reg20_dacdrven(value);
}

void sys_hal_aud_mic1_single_en(uint32_t value)
{
	//sys_ll_set_ana_reg14_micsingleen(value);
}

void sys_hal_aud_mic2_single_en(uint32_t value)
{
	//sys_ll_set_ana_reg15_micsingleen(value);
}

void sys_hal_aud_int_en(uint32_t value)
{
	sys_ll_set_cpu0_int_0_31_en_cpu0_aud_int_en(value);
}

void sys_hal_aud_power_en(uint32_t value)
{
	sys_ll_set_cpu_power_sleep_wakeup_pwd_audp(value);
}

void sys_hal_aud_lvcmd_en(uint32_t value)
{
	sys_ll_set_ana_reg20_lenvcmd(value);
}

void sys_hal_aud_micbias1v_en(uint32_t value)
{
	sys_ll_set_ana_reg18_micbias_voc(value);
}

void sys_hal_aud_micbias_trim_set(uint32_t value)
{
	sys_ll_set_ana_reg18_micbias_trm(value);
}

void sys_hal_aud_audpll_en(uint32_t value)
{
}

void sys_hal_aud_dacr_en(uint32_t value)
{
}

void sys_hal_aud_vdd1v_en(uint32_t value)
{
}

void sys_hal_aud_vdd1v5_en(uint32_t value)
{
}

void sys_hal_aud_mic2_en(uint32_t value)
{
}

void sys_hal_aud_mic2_gain_set(uint32_t value)
{
}

void sys_hal_aud_dacg_set(uint32_t value)
{
	sys_ll_set_ana_reg20_dacg(value);
}

void sys_hal_aud_aud_en(uint32_t value)
{
}

void sys_hal_aud_rvcmd_en(uint32_t value)
{
}

/**  Audio End  **/

/**  FFT Start  **/

void sys_hal_fft_disckg_set(uint32_t value)
{
	//sys_ll_set_cpu_mode_disckg1_fft_disckg(value);
}

void sys_hal_cpu_fft_int_en(uint32_t value)
{
	sys_ll_set_cpu0_int_0_31_en_cpu0_fft_int_en(value);
}

/**  FFT End  **/

/**  I2S Start  **/
void sys_hal_i2s_select_clock(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode1_cksel_i2s(value);
}

void sys_hal_i2s_clock_en(uint32_t value)
{
	sys_ll_set_cpu_device_clk_enable_i2s_cken(value);
}

void sys_hal_i2s1_clock_en(uint32_t value)
{
	sys_ll_set_reserver_reg0xd_i2s1_cken(value);
}

void sys_hal_i2s2_clock_en(uint32_t value)
{
	sys_ll_set_reserver_reg0xd_i2s2_cken(value);
}

void sys_hal_i2s_int_en(uint32_t value)
{
	sys_ll_set_cpu0_int_0_31_en_cpu0_i2s0_int_en(value);
}

void sys_hal_i2s1_int_en(uint32_t value)
{
	sys_ll_set_cpu0_int_32_63_en_cpu0_i2s1_int_en(value);
}

void sys_hal_i2s2_int_en(uint32_t value)
{
	sys_ll_set_cpu0_int_32_63_en_cpu0_i2s2_int_en(value);
}

void sys_hal_i2s_disckg_set(uint32_t value)
{
	//NOT SUPPORT
}

void sys_hal_apll_en(uint32_t value)
{
	//NOT SUPPORT
}

void sys_hal_cb_manu_val_set(uint32_t value)
{
	//NOT SUPPORT
}

void sys_hal_ana_reg11_vsel_set(uint32_t value)
{
	//NOT SUPPORT
}

void sys_hal_ana_reg10_sdm_val_set(uint32_t value)
{
	//NOT SUPPORT
}

void sys_hal_ana_reg11_spi_trigger_set(uint32_t value)
{
	//NOT SUPPORT
}

void sys_hal_i2s0_ckdiv_set(uint32_t value)
{
	//NOT SUPPORT
}

/**  I2S End  **/

/**  Touch Start **/
void sys_hal_touch_power_down(uint32_t value)
{
	sys_ll_set_ana_reg14_pwd_td(value);
}

void sys_hal_touch_sensitivity_level_set(uint32_t value)
{
	sys_ll_set_ana_reg14_gain_s(value);
}

void sys_hal_touch_scan_mode_enable(uint32_t value)
{
	//sys_ll_set_ana_reg8_en_scm(value);
}

void sys_hal_touch_detect_threshold_set(uint32_t value)
{
	sys_ll_set_ana_reg14_vrefs(value);
}

void sys_hal_touch_detect_range_set(uint32_t value)
{
	sys_ll_set_ana_reg14_crg(value);
}

void sys_hal_touch_calib_enable(uint32_t value)
{
	sys_ll_set_ana_reg15_en_cal_force1v(value);
}

void sys_hal_touch_manul_mode_calib_value_set(uint32_t value)
{
	//sys_ll_set_ana_reg8_cap_calspi(value);
}

void sys_hal_touch_manul_mode_enable(uint32_t value)
{
	//sys_ll_set_ana_reg9_man_mode(value);
}

void sys_hal_touch_scan_mode_chann_set(uint32_t value)
{
	sys_ll_set_ana_reg15_chs(value);
}

void sys_hal_touch_scan_mode_chann_sel(uint32_t value)
{
	sys_ll_set_ana_reg15_chs_sel_cal1v(value);
}

void sys_hal_touch_serial_cap_enable(void)
{
	sys_ll_set_ana_reg14_en_seri_cap(1);
}

void sys_hal_touch_serial_cap_disable(void)
{
	sys_ll_set_ana_reg14_en_seri_cap(0);
}

void sys_hal_touch_serial_cap_sel(uint32_t value)
{
	sys_ll_set_ana_reg14_sel_seri_cap(value);
}

void sys_hal_touch_spi_lock(void)
{
	sys_ll_set_ana_reg14_td_latch1v(0);
}

void sys_hal_touch_spi_unlock(void)
{
	sys_ll_set_ana_reg14_td_latch1v(1);
}

void sys_hal_touch_test_period_set(uint32_t value)
{
	sys_ll_set_ana_reg15_test_period1v(value);
}

void sys_hal_touch_test_number_set(uint32_t value)
{
	sys_ll_set_ana_reg15_test_number1v(value);
}

void sys_hal_touch_calib_period_set(uint32_t value)
{
	sys_ll_set_ana_reg16_cal_period1v(value);
}

void sys_hal_touch_calib_number_set(uint32_t value)
{
	sys_ll_set_ana_reg16_cal_number1v(value);
}

void sys_hal_touch_int_set(uint32_t value)
{
	sys_ll_set_ana_reg16_int_en(value);
}

void sys_hal_touch_int_clear(uint32_t value)
{
	sys_ll_set_ana_reg17_int_clr(value);
}

void sys_hal_touch_int_enable(uint32_t value)
{
	sys_ll_set_cpu0_int_32_63_en_cpu0_touched_int_en(value);
}

/**  Touch End **/


/** jpeg start **/
void sys_hal_set_jpeg_clk_sel(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode1_cksel_jpeg(value);
}

void sys_hal_set_clk_div_mode1_clkdiv_jpeg(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode1_clkdiv_jpeg(value);
}

void sys_hal_set_jpeg_disckg(uint32_t value)
{
	//sys_ll_set_cpu_mode_disckg1_jpeg_disckg(value);
}

void sys_hal_set_cpu_clk_div_mode1_clkdiv_bus(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode1_clkdiv_bus(value);
}

void sys_hal_video_power_en(uint32_t value)
{
	sys_ll_set_cpu_power_sleep_wakeup_pwd_vidp(value);
}

void sys_hal_set_auxs_clk_sel(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode2_cksel_auxs(value);
}

void sys_hal_set_auxs_clk_div(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode2_ckdiv_auxs(value);
}

void sys_hal_set_jpeg_clk_en(uint32_t value)
{
	sys_ll_set_cpu_device_clk_enable_jpeg_cken(value);
}

/** jpeg end **/

/** h264 Start **/
void sys_hal_set_h264_clk_en(uint32_t value)
{
	sys_ll_set_reserver_reg0xd_h264_cken(value);
}

/** h264 End **/

/**  psram Start **/
void sys_hal_psram_volstage_sel(uint32_t enable)
{
	uint32_t value = sys_ll_get_ana_reg6_value();

	if (enable)
		value |= (0x1 << 5);
	else
		value &= ~(0x1 << 5);

	sys_ll_set_ana_reg6_value(value);
}

void sys_hal_psram_xtall_osc_enable(uint32_t enable)
{
	uint32_t value = sys_ll_get_ana_reg6_value();

	if (enable)
		value |= (0x1 << 7);
	else
		value &= ~(0x1 << 7);

	sys_ll_set_ana_reg6_value(value);
}

void sys_hal_psram_doc_enable(uint32_t enable)
{
	uint32_t value = sys_ll_get_ana_reg6_value();

	if (enable)
		value |= (0x1 << 8);
	else
		value &= ~(0x1 << 8);

	sys_ll_set_ana_reg6_value(value);
}

void sys_hal_psram_ldo_enable(uint32_t enable)
{
	uint32_t value = sys_ll_get_ana_reg6_value();

	if (enable)
		value |= (0x1 << 9);
	else
		value &= ~(0x1 << 9);

	sys_ll_set_ana_reg6_value(value);
}

void sys_hal_psram_dpll_enable(uint32_t enable)
{
	uint32_t value = sys_ll_get_ana_reg6_value();

	if (enable)
		value |= (0x1 << 12);
	else
		value &= ~(0x1 << 12);

	sys_ll_set_ana_reg6_value(value);
}

void sys_hal_psram_clk_sel(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode2_cksel_psram(value);
}

void sys_hal_psram_set_clkdiv(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode2_ckdiv_psram(value);
}

/**  psram End **/

/* REG_0x03:cpu_storage_connect_op_select->flash_sel:0: normal flash operation 1:flash download by spi,R/W,0x3[9]*/
uint32_t sys_hal_get_cpu_storage_connect_op_select_flash_sel(void)
{
    return sys_ll_get_cpu_storage_connect_op_select_flash_sel();
}

void sys_hal_set_cpu_storage_connect_op_select_flash_sel(uint32_t value)
{
    sys_ll_set_cpu_storage_connect_op_select_flash_sel(value);
}

/**  Misc Start **/
//Misc
/**  Misc End **/

void sys_hal_set_ana_trxt_tst_enable(uint32_t value)
{
	//sys_ll_set_ana_reg5_trxt_tst_enable(value);
}


void sys_hal_set_ana_scal_en(uint32_t value)
{
	//sys_ll_set_ana_reg7_scal_en(value);
}


void sys_hal_set_ana_gadc_buf_ictrl(uint32_t value)
{
   //sys_ll_set_ana_reg7_gadc_buf_ictrl(value);
}

void sys_hal_set_ana_gadc_cmp_ictrl(uint32_t value)
{
    //sys_ll_set_ana_reg7_gadc_cmp_ictrl(value);
}

void sys_hal_set_ana_pwd_gadc_buf(uint32_t value)
{
	//sys_ll_set_ana_reg6_pwd_gadc_buf(value);
}

void sys_hal_set_ana_vref_sel(uint32_t value)
{
	//sys_ll_set_ana_reg7_vref_sel(value);
}
void sys_hal_set_ana_cb_cal_manu(uint32_t value)
{
    sys_ll_set_ana_reg5_bcal_en(value);
}

void sys_hal_set_ana_cb_cal_trig(uint32_t value)
{
    sys_ll_set_ana_reg5_bcal_start(value);
}
void sys_hal_set_ana_vlsel_ldodig(uint32_t value)
{
    //sys_ll_set_ana_reg3_vlsel_ldodig(value);
}
void sys_hal_set_ana_vhsel_ldodig(uint32_t value)
{
    //sys_ll_set_ana_reg3_vhsel_ldodig(value);
}

void sys_hal_set_sdio_clk_en(uint32_t value)
{
	sys_ll_set_cpu_device_clk_enable_sdio_cken(value);
}

void sys_hal_set_cpu0_sdio_int_en(uint32_t value)
{
	sys_ll_set_cpu0_int_0_31_en_cpu0_sdio_int_en(value);
}

void sys_hal_set_cpu1_sdio_int_en(uint32_t value)
{
	sys_ll_set_cpu1_int_0_31_en_cpu1_sdio_int_en(value);
}

void sys_hal_set_sdio_clk_div(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode2_ckdiv_sdio(value);
}

uint32_t sys_hal_get_sdio_clk_div()
{
	return sys_ll_get_cpu_clk_div_mode2_ckdiv_sdio();
}

void sys_hal_set_sdio_clk_sel(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode2_cksel_sdio(value);
}

uint32_t sys_hal_get_sdio_clk_sel()
{
	return sys_ll_get_cpu_clk_div_mode2_cksel_sdio();
}


void sys_hal_set_ana_vctrl_sysldo(uint32_t value)
{
    //sys_ll_set_ana_reg5_vctrl_sysldo(value);
}

void sys_hal_set_yuv_buf_clock_en(uint32_t value)
{
	sys_ll_set_reserver_reg0xd_yuv_cken(value);
}

void sys_hal_set_h264_clock_en(uint32_t value)
{
	sys_ll_set_reserver_reg0xd_h264_cken(value);
}

void sys_hal_nmi_wdt_set_clk_div(uint32_t value)
{
	sys_ll_set_cpu_26m_wdt_clk_div_ckdiv_wdt(value);
}

uint32_t sys_hal_nmi_wdt_get_clk_div(void)
{
	return sys_ll_get_cpu_26m_wdt_clk_div_ckdiv_wdt();
}


void sys_hal_set_ana_cb_cal_manu_val(uint32_t value)
{
    sys_ll_set_ana_reg5_vbias(value);
}


static void sys_hal_delay(volatile uint32_t times)
{
        while(times--);
}

//TODO the module owner can put the sys init to better place
void sys_hal_early_init(void)
{
	uint32_t chip_id = aon_pmu_hal_get_chipid();

	uint32_t val = sys_hal_analog_get(ANALOG_REG5);
	val |= (0x1 << 14) | (0x1 << 5) | (0x1 << 3) | (0x1 << 2);
	sys_hal_analog_set(ANALOG_REG5,val);

	val = sys_hal_analog_get(ANALOG_REG0);
	val |= (0x13 << 20) ;
	sys_hal_analog_set(ANALOG_REG0,val);

	sys_hal_analog_set(ANALOG_REG0, 0xF1305B57);  // triger dpll
	sys_ll_set_ana_reg0_dsptrig(1);
	sys_ll_set_ana_reg0_dsptrig(0);

	sys_hal_analog_set(ANALOG_REG2, 0x7E003450); //wangjian20221110 xtal=0x50
	//sys_ll_set_cpu_device_clk_enable_value(0x0c008084);
	sys_hal_analog_set(ANALOG_REG3, 0xC5F00B88);

	/**
	 * attention:
	 * SPI latch must be enable before ana_reg[8~13] modification
	 * and don't forget disable it after that.
	 */
	sys_ll_set_ana_reg9_spi_latch1v(1);
	if ((chip_id & PM_CHIP_ID_MASK) == (PM_CHIP_ID_MPW_V2_3 & PM_CHIP_ID_MASK)) {
		sys_hal_analog_set(ANALOG_REG8, 0x57E62FFE);
	} else {
		sys_hal_analog_set(ANALOG_REG8, 0x57E62F26);//shuguang20230414[8:3]7->4: for evm
	}
	sys_hal_analog_set(ANALOG_REG9, 0x787BC34A);
	sys_hal_analog_set(ANALOG_REG10, 0xC35543C7);//tenglong20230417:rosc config for buck in lowpower
	if ((chip_id & PM_CHIP_ID_MASK) == (PM_CHIP_ID_MPW_V2_3 & PM_CHIP_ID_MASK)) {
		sys_hal_analog_set(ANALOG_REG11, 0x9FEF31F7);
		sys_hal_analog_set(ANALOG_REG12, 0x9F03EF6F);
		sys_hal_analog_set(ANALOG_REG13, 0x1F6FB3FF);
	} else {
		sys_hal_analog_set(ANALOG_REG11, 0xD77EB9FF);
		sys_hal_analog_set(ANALOG_REG12, 0xD77ECA4A);
		sys_hal_analog_set(ANALOG_REG13, 0x547AB0F5);
	}
	sys_ll_set_ana_reg9_spi_latch1v(0);

	sys_hal_flash_set_clk(0x2);

	/* clk_divd 120MHz,
	 * 1, the core clock is depended on CONFIG_CPU_FREQ_HZ and configSYSTICK_CLOCK_HZ.
	 *    Pay attention to bk_pm_module_vote_cpu_freq,and the routine will switch core
	 *    clock;
	 * 2, sysTick module's clock source is processor clock now;
	 */
	sys_hal_mclk_div_set(480000000/CONFIG_CPU_FREQ_HZ - 1);
	sys_hal_delay(10000);
	sys_hal_mclk_mux_set(0x3);/*clock source: DPLL, 480M*/

	timer_hal_us_init();
}
