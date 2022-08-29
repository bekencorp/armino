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
//#include "platform.h"

static sys_hal_t s_sys_hal;
uint32 sys_hal_get_int_group2_status(void);
/**  Platform Start **/
//Platform


/** Platform Misc Start **/
bk_err_t sys_hal_init()
{
	s_sys_hal.hw = (sys_hw_t *)SYS_LL_REG_BASE;
	return BK_OK;
}
/** Platform Misc End **/


//sys_hal_主语(模块名)_谓语(动作:set/get/enable等)_宾语(status/value)
//该函数在每个芯片目录中都有一份
void sys_hal_usb_enable_clk(bool en)
{
	//ll层命名规范跟随ASIC的Address Mapping走，可以修改Address Mapping的注释
	//这个注释ASIC的verilog也在使用
	sys_ll_set_cpu_device_clk_enable_usb_cken(en);
}

void sys_hal_usb_analog_phy_en(bool en)
{
	sys_ll_set_ana_reg6_en_usb(en);
}

void sys_hal_usb_analog_speed_en(bool en)
{
	sys_ll_set_ana_reg9_usb_speed(en);
}

void sys_hal_usb_analog_ckmcu_en(bool en)
{
	sys_ll_set_ana_reg11_ck2mcu(en);
}

void sys_hal_usb_enable_charge(bool en)
{
	sys_ll_set_ana_reg5_vctrl_dpllldo(en);
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

/*sleep feature start*/
void sys_hal_enter_deep_sleep(void * param)
{
    uint32_t modules_power_state=0;
	uint32_t  clock_value = 0;
	uint32_t  pmu_val2 = 0;

	/*mask all interner interrupt*/
	sys_ll_set_cpu0_int_halt_clk_op_cpu0_halt(1);
	/*1.switch cpu clock to xtal26m*/
	clock_value = sys_ll_get_cpu_clk_div_mode1_value();
	clock_value &= ~(SYS_CPU_CLK_DIV_MODE1_CLKDIV_CORE_MASK << SYS_CPU_CLK_DIV_MODE1_CLKDIV_CORE_POS);
	clock_value &= ~(SYS_CPU_CLK_DIV_MODE1_CKSEL_CORE_MASK << SYS_CPU_CLK_DIV_MODE1_CKSEL_CORE_POS);
	sys_ll_set_cpu_clk_div_mode1_value(clock_value);

	/*2.switch flash clock to xtal26m*/
	clock_value = 0;
	clock_value = sys_ll_get_cpu_clk_div_mode2_value();
	clock_value &= ~(SYS_CPU_CLK_DIV_MODE2_CKSEL_FLASH_MASK << SYS_CPU_CLK_DIV_MODE2_CKSEL_FLASH_POS);
	clock_value &= ~(SYS_CPU_CLK_DIV_MODE2_CKDIV_FLASH_MASK << SYS_CPU_CLK_DIV_MODE2_CKDIV_FLASH_POS);
	sys_ll_set_cpu_clk_div_mode2_value(clock_value);

	/*3.close high frequncy clock*/
	clock_value = 0;
    clock_value = sys_ll_get_ana_reg6_value();
	clock_value |= (1 << SYS_ANA_REG6_EN_SLEEP_POS);
	clock_value &= ~((1 << SYS_ANA_REG6_EN_DPLL_POS)|(1 << SYS_ANA_REG6_EN_AUDPLL_POS)|(1 << SYS_ANA_REG6_EN_PSRAM_LDO_POS)|(1 << SYS_ANA_REG6_EN_DCO_POS)|(1 << SYS_ANA_REG6_EN_USB_POS));
    sys_ll_set_ana_reg6_value(clock_value);

	clock_value = 0;
    clock_value = sys_ll_get_ana_reg5_value();
	clock_value &= ~(1 << SYS_ANA_REG5_ENCB_POS);//global central bias enable
    sys_ll_set_ana_reg5_value(clock_value);

	clock_value = 0;
    sys_ll_set_ana_reg19_value(clock_value);

	/*4.set PMU parameters*/
    aon_pmu_hal_set_sleep_parameters(0x4e111111);

    /*5.set power flag*/
	modules_power_state = sys_ll_get_cpu_power_sleep_wakeup_value();
	modules_power_state |= 0xa0000;
	sys_ll_set_cpu_power_sleep_wakeup_value(modules_power_state);

	/*6.set sleep flag*/
	pmu_val2 =  aon_pmu_hal_reg_get(PMU_REG2);
	pmu_val2 |= BIT(BIT_SLEEP_FLAG_DEEP_SLEEP);
	aon_pmu_hal_reg_set(PMU_REG2,pmu_val2);

	/*7.mask all interner interrupt*/
	//sys_ll_set_cpu0_int_halt_clk_op_cpu0_halt(1);

	/*8.WFI*/
	//__asm volatile( "wfi" );

}
void sys_hal_exit_low_voltage()
{
	uint32_t modules_power_state=0;
	uint32_t wakeup_souce = 0x1F0;
	uint32_t reg_val = 0;
	uint32_t clock_value = 0;

	/*1.exit sleep*/
	modules_power_state = sys_ll_get_cpu_power_sleep_wakeup_value();
	modules_power_state &= ~0xf0000;
	sys_ll_set_cpu_power_sleep_wakeup_value(modules_power_state);

	/*2.disable wakeup source*/
	reg_val = aon_pmu_hal_get_wakeup_source_reg();
	reg_val &= ~wakeup_souce;
	aon_pmu_hal_set_wakeup_source_reg(reg_val);

	/*3.open high frequncy clock*/
	clock_value = 0;
    clock_value = sys_ll_get_ana_reg6_value();
	clock_value |= (1 << 12);//en_dpll
	clock_value |= (1 << 8);//en_dco
    sys_ll_set_ana_reg6_value(clock_value);

}
//uint32_t  g_previous_tick = 0;
//uint32_t  g_wifi_previous_tick = 0;
#define BIT_AON_PMU_WAKEUP_ENA      (0x1F0U)
void sys_hal_enter_low_voltage(void)
{
	uint32_t  modules_power_state = 0;
	uint32_t  clock_value = 0;
	uint32_t  clk_div_val0= 0, clk_div_val1 = 0, clk_div_val2 = 0;
	uint32_t  pmu_val2 = 0;
	//uint32_t  pmu_state = 0;
	uint32_t  previous_tick = 0;
	uint32_t  current_tick = 0;
	uint32_t  clk_div_temp = 0;
	uint32_t  int_state1 = 0;
	uint32_t  int_state2 = 0;

	int_state1 = sys_ll_get_cpu0_int_0_31_en_value();
	int_state2 = sys_ll_get_cpu0_int_32_63_en_value();
	sys_ll_set_cpu0_int_0_31_en_value(0x0);
	sys_ll_set_cpu0_int_32_63_en_value(0x0);
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	__asm volatile( "nop" );

      /*mask all interner interrupt*/
	sys_ll_set_cpu0_int_halt_clk_op_cpu0_int_mask(1);

	//gpio_disable_output();
	clk_div_val0 =  sys_hal_all_modules_clk_div_get(CLK_DIV_REG0);
	clk_div_val1 =  sys_hal_all_modules_clk_div_get(CLK_DIV_REG1);
	clk_div_val2 =  sys_hal_all_modules_clk_div_get(CLK_DIV_REG2);

	/*1.switch cpu clock to xtal26m*/
	sys_ll_set_cpu_clk_div_mode1_cksel_core(0);
	sys_ll_set_cpu_clk_div_mode1_clkdiv_core(0);
	sys_ll_set_cpu_clk_div_mode1_clkdiv_bus(0);

    //__asm volatile( "j ." );

	/*2.switch flash clock to xtal26m*/
	clock_value = 0;
	clock_value = sys_ll_get_cpu_clk_div_mode2_value();
	clock_value &= ~(SYS_CPU_CLK_DIV_MODE2_CKSEL_FLASH_MASK << SYS_CPU_CLK_DIV_MODE2_CKSEL_FLASH_POS);
	clock_value &= ~(SYS_CPU_CLK_DIV_MODE2_CKDIV_FLASH_MASK << SYS_CPU_CLK_DIV_MODE2_CKDIV_FLASH_POS);
	sys_ll_set_cpu_clk_div_mode2_value(clock_value);

	/*3.close analog clk */
	clock_value = 0;
	clock_value = sys_ll_get_ana_reg6_value();
	/*temp close analog clk solution, we will record the opened clk,then it will close them ,when wakeup will open them*/
	//clock_value |= (1 << SYS_ANA_REG6_EN_SLEEP_POS);//enable xtal26m sleep
	//clock_value &= ~((1 << SYS_ANA_REG6_XTAL_LPMODE_CTRL_POS)|(1 << SYS_ANA_REG6_EN_DPLL_POS)|(1 << SYS_ANA_REG6_EN_AUDPLL_POS)|(1 << SYS_ANA_REG6_EN_PSRAM_LDO_POS)|(1 << SYS_ANA_REG6_EN_DCO_POS)|(1 << SYS_ANA_REG6_EN_XTALL_POS)|(1 << SYS_ANA_REG6_EN_USB_POS));
	clock_value &= ~((1 << SYS_ANA_REG6_EN_DPLL_POS)|(1 << SYS_ANA_REG6_EN_USB_POS)|(1 << SYS_ANA_REG6_EN_AUDPLL_POS)|(1 << SYS_ANA_REG6_EN_PSRAM_LDO_POS));
	sys_ll_set_ana_reg6_value(clock_value);


	/*4.set sleep parameters*/
	aon_pmu_hal_set_sleep_parameters(0x2B111FFF);//a)close isolat temp for bt/wifi wakeup in low voltage;b)let 1.5ms time for xtal 26m stability

	/*5.set power flag*/
	modules_power_state = 0;
	modules_power_state = sys_ll_get_cpu_power_sleep_wakeup_value();
	modules_power_state |= 0xa0000;
	sys_ll_set_cpu_power_sleep_wakeup_value(modules_power_state);

	pmu_val2 =  aon_pmu_hal_reg_get(PMU_REG2);
	pmu_val2 |= BIT(BIT_SLEEP_FLAG_LOW_VOLTAGE);
	aon_pmu_hal_reg_set(PMU_REG2,pmu_val2);


	sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_gen_en(0x1);
	sys_ll_set_cpu0_int_32_63_en_cpu0_gpio_int_en(0x1);
	sys_ll_set_cpu0_int_32_63_en_cpu0_rtc_int_en(0x1);
	sys_ll_set_cpu0_int_32_63_en_cpu0_touched_int_en(0x1);
	sys_ll_set_cpu0_int_32_63_en_cpu0_dm_irq_en(0x1);
//	set_csr(NDS_MIE, MIP_MTIP);
	/*6.mask all interner interrupt*/
	//sys_ll_set_cpu0_int_halt_clk_op_cpu0_int_mask(1);
	//__asm volatile( "wfi" );
       /*get interrupt */
	while(1)
	{
		/*6.WFI*/
		//__asm volatile( "wfi" );
		extern u32 arch_get_int_status(void);

		if(arch_get_int_status() != 0)
		{
			break;
		}
	}

	extern uint32_t pm_wake_int_flag1, pm_wake_int_flag2;
	extern uint32_t pm_wake_gpio_flag1, pm_wake_gpio_flag2;
	extern gpio_driver_t s_gpio;
	{
		pm_wake_int_flag1 = sys_hal_get_int_status();
		pm_wake_int_flag2 = sys_hal_get_int_group2_status();

		gpio_hal_t *hal = &s_gpio.hal;
		gpio_interrupt_status_t gpio_status;

		gpio_hal_get_interrupt_status(hal, &gpio_status);
		pm_wake_gpio_flag1 = gpio_status.gpio_0_31_int_status;
		pm_wake_gpio_flag2 = gpio_status.gpio_32_64_int_status;
	}

	/*add delay for xtal 26m, analog suggest 1.5ms,we add protect time to 2ms(1.5ms(hardware delay,0.5ms software delay))*/
	previous_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	current_tick = previous_tick;
	while(((uint32_t)(current_tick - previous_tick)) < (uint32_t)(LOW_POWER_XTAL_26M_STABILITY_DELAY_TIME*RTC_TICKS_PER_1MS))/*32*0.5*/
	{
		current_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	}

	/*7.restore state before low voltage*/
	modules_power_state = 0;
	modules_power_state = sys_ll_get_cpu_power_sleep_wakeup_value();
	modules_power_state &= ~0xf0000;
	sys_ll_set_cpu_power_sleep_wakeup_value(modules_power_state);

	/*8.restore the analog clk*/
	clock_value = 0;
	clock_value = sys_ll_get_ana_reg6_value();
	clock_value |= ((1 << SYS_ANA_REG6_EN_DPLL_POS)|(1 << SYS_ANA_REG6_EN_USB_POS)|(1 << SYS_ANA_REG6_EN_PSRAM_LDO_POS));//en_dpll, en_usb,en_PSRAM_LDO
	clock_value &= ~(1 << SYS_ANA_REG6_EN_SLEEP_POS);//disable xtal26m sleep
	sys_ll_set_ana_reg6_value(clock_value);
	//os_printf("low voltage wake up 123456\r\n");
	/*add delay for xtal 26m, analog suggest 800us,we add protect time to 1ms*/

	previous_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	//g_previous_tick = previous_tick;
	//g_wifi_previous_tick = previous_tick;
	current_tick = previous_tick;
	while(((uint32_t)(current_tick - previous_tick)) < (uint32_t)(LOW_POWER_DPLL_STABILITY_DELAY_TIME*RTC_TICKS_PER_1MS))/*32*1*/
	{
		current_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	}

	/*9.restore clk div*/
    clk_div_temp = clk_div_val0;
	clk_div_temp &= SYS_CPU_CLK_DIV_MODE1_CLKDIV_CORE_MASK << SYS_CPU_CLK_DIV_MODE1_CLKDIV_CORE_POS;
	sys_ll_set_cpu_clk_div_mode1_clkdiv_core(clk_div_temp);
	clk_div_temp = clk_div_val0;
	clk_div_temp &= SYS_CPU_CLK_DIV_MODE1_CLKDIV_BUS_MASK << SYS_CPU_CLK_DIV_MODE1_CLKDIV_BUS_POS;
	sys_ll_set_cpu_clk_div_mode1_clkdiv_bus(clk_div_temp);
	clk_div_temp = clk_div_val0;
	clk_div_temp &= SYS_CPU_CLK_DIV_MODE1_CKSEL_CORE_MASK << SYS_CPU_CLK_DIV_MODE1_CKSEL_CORE_POS;
	sys_ll_set_cpu_clk_div_mode1_cksel_core(clk_div_temp);

	sys_hal_all_modules_clk_div_set(CLK_DIV_REG0, clk_div_val0);
	sys_hal_all_modules_clk_div_set(CLK_DIV_REG1, clk_div_val1);
	sys_hal_all_modules_clk_div_set(CLK_DIV_REG2, clk_div_val2);

    /*10. not disable wakeup source*/
#if 0
	pmu_state =  aon_pmu_hal_reg_get(PMU_REG0x41);
	pmu_state &= ~ BIT_AON_PMU_WAKEUP_ENA;
	aon_pmu_hal_reg_set(PMU_REG0x41,pmu_state);
#endif
	sys_ll_set_cpu0_int_0_31_en_value(int_state1);
	sys_ll_set_cpu0_int_32_63_en_value(int_state2);
	//set_csr(NDS_MIE, MIP_MTIP);
	//gpio_restore();

}

void sys_hal_touch_wakeup_enable(uint8_t index)
{
	uint32_t  pmu_state = 0;
	wakeup_source_t wakeup_source = WAKEUP_SOURCE_INT_TOUCHED;
    pmu_state =  aon_pmu_hal_reg_get(PMU_REG1);
	pmu_state |= BIT(index) << 4;
	pmu_state |= index << 0;
	aon_pmu_hal_reg_set(PMU_REG1,pmu_state);
    sys_hal_touch_power_down(0);

	aon_pmu_hal_set_wakeup_source(wakeup_source);

	sys_hal_touch_int_enable(1);
}

void sys_hal_usb_wakeup_enable(uint8_t index)
{
	uint32_t wakeup_usb_int_en = 0x1;
	uint32_t system_usb_int_en = 0x1 << 21;
	wakeup_source_t wakeup_source = WAKEUP_SOURCE_INT_USBPLUG;
	aon_pmu_hal_usbplug_int_en(wakeup_usb_int_en);
	aon_pmu_hal_set_wakeup_source(wakeup_source);

	sys_hal_int_group2_enable(system_usb_int_en);
}

void sys_hal_rtc_wakeup_enable(uint32_t value)
{
    uint32_t system_rtc_int_en = 0x1 << 21;
	wakeup_source_t wakeup_source = WAKEUP_SOURCE_INT_RTC;
	aon_pmu_hal_set_wakeup_source(wakeup_source);

	sys_hal_int_group2_enable(system_rtc_int_en);
}

void sys_hal_wifiorbt_wakeup_enable(uint32_t type)
{
	//wakeup_source_t wakeup_source = WAKEUP_SOURCE_INT_SYSTEM_WAKE;
	//wifi: type=0,  BT: type=1
	if(type == 0){
		sys_ll_set_cpu_power_sleep_wakeup_wifi_wakeup_platform_en(1);
	}else{
		sys_ll_set_cpu_power_sleep_wakeup_bts_wakeup_platform_en(1);
	}

	//aon_pmu_hal_set_wakeup_source(wakeup_source);
}

gpio_driver_t s_gpio_base = {0};
void sys_hal_gpio_wakeup_enable(uint32_t index, gpio_int_type_t type)
{
	gpio_hal_init(&s_gpio_base.hal);
	gpio_hal_t *hal = &s_gpio_base.hal;
	wakeup_source_t wakeup_source = WAKEUP_SOURCE_INT_GPIO;
	/*1.clear gpio int enable  */
	gpio_hal_disable_interrupt(hal, index);
	/*2. gpio setting */
	gpio_hal_input_enable(hal, index, 1);
	gpio_hal_set_int_type(hal, index, type);
	/*3. clear gpio int firstly */
	gpio_hal_clear_chan_interrupt_status(hal, index);
	/*4. open gpio int enable */
	gpio_hal_enable_interrupt(hal, index);
	/*5. set gpio wakeup source */
	aon_pmu_hal_set_wakeup_source(wakeup_source);

}
void sys_hal_enter_normal_sleep(uint32_t peri_clk)
{
    sys_ll_set_cpu0_int_halt_clk_op_cpu0_halt(1);
    //__asm volatile( "wfi" );
}

void sys_hal_enter_normal_wakeup()
{

}
/*for low power function start*/
void sys_hal_module_power_ctrl(power_module_name_t module,power_module_state_t power_state)
{
    uint32_t value = 0;
	if((module >= POWER_MODULE_NAME_MEM1) && (module <= POWER_MODULE_NAME_WIFI_PHY))
	{
	    value = 0;
    	value = sys_ll_get_cpu_power_sleep_wakeup_value();
        if(power_state == POWER_MODULE_STATE_ON)//power on
        {
            value &= ~(1 << module);
        }
    	else //power down
    	{
    	    value |= (1 << module);
    	}
		sys_ll_set_cpu_power_sleep_wakeup_value(value);
	}
    else if(module == POWER_MODULE_NAME_CPU1)
    {
		if(power_state == POWER_MODULE_STATE_ON)
		{
			//power on and then support clock
			sys_ll_set_cpu1_int_halt_clk_op_cpu1_pwr_dw(POWER_MODULE_STATE_ON);
			sys_ll_set_cpu1_int_halt_clk_op_cpu1_halt(0);

			//wait halt really cleared,clock support finish
			for(int i = 0; i < 1000; i++);
		}
		else
		{
			//un-support clock and then power down
			sys_ll_set_cpu1_int_halt_clk_op_cpu1_halt(1);
			//here should wait halt really finish and then power down
			for(int i = 0; i < 1000; i++);

			sys_ll_set_cpu1_int_halt_clk_op_cpu1_pwr_dw(POWER_MODULE_STATE_OFF);
		}
    }
	else
	{
	   ;//do something
	}

}
int32 sys_hal_module_power_state_get(power_module_name_t module)
{
	return 0;
}
void sys_hal_module_RF_power_ctrl (module_name_t module,power_module_state_t power_state)
{
    uint32_t value = 0;
	value = sys_ll_get_ana_reg6_value();
    if(power_state == POWER_MODULE_STATE_ON)//power on
    {
		value |= ((1 << SYS_ANA_REG6_EN_SYSLDO_POS)|(1 << SYS_ANA_REG6_EN_DPLL_POS)|(1 << SYS_ANA_REG6_EN_TEMPDET_POS));//en_sysldo,en_dpll
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
void sys_hal_core_bus_clock_ctrl(high_clock_module_name_t core, uint32_t clksel,uint32_t clkdiv, high_clock_module_name_t bus,uint32_t bus_clksel,uint32_t bus_clkdiv)
{
    uint32_t clock_value = 0;
	clock_value = sys_ll_get_cpu_clk_div_mode1_value();
    /*core:0: clk_DCO      1 : XTAL      2 : 320M      3 : 480M*/
	clock_value &= ~(0x7F);
    /*1.cpu0:120m ,maxtrix:120m*/
	if((core == HIGH_FREQUECY_CLOCK_MODULE_CPU0) &&(clksel == 3))
	{
		clock_value |=  0x3 << 4; // select 480m
		clock_value |=  0x3 << 0; //4//  480m/4 = 120m
		//clock_value |=  0x1 << 6; //bus 120m
	}/*2.cpu0:320m ,maxtrix:160m*/
	else if((core == HIGH_FREQUECY_CLOCK_MODULE_CPU0) &&(clksel == 2))
	{
        clock_value |=  0x2 << 4;
        clock_value |=  0x1 << 6;
	}/*3.cpu0:240m ,maxtrix:120m*/
	else if((core == HIGH_FREQUECY_CLOCK_MODULE_CPU0) &&(clksel == 0))
	{
		clock_value |=  0x3 << 4;
		clock_value |=  0x1 << 0;
		clock_value |=  0x1 << 6; //bus 120m
	}/*3.cpu0:26m ,maxtrix:26m*/
	else if((core == HIGH_FREQUECY_CLOCK_MODULE_CPU0) &&(clksel == 1))
	{
        clock_value |=  0x1 << 4;
        clock_value |=  0x0 << 6;
	}
	else
	{
        clock_value |=  0x0 << 4;
        clock_value |=  0x0 << 6;
	}

	sys_ll_set_cpu_clk_div_mode1_value(clock_value);

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
void sys_hal_enable_mac_wakeup_source()
{
   module_name_t module_name = MODULE_NAME_WIFI;
   //wakeup_source_t wakeup_source = WAKEUP_SOURCE_INT_SYSTEM_WAKE;
   sys_hal_wifiorbt_wakeup_enable(module_name);
   //aon_pmu_hal_set_wakeup_source(wakeup_source);
}
void sys_hal_enable_bt_wakeup_source()
{
   module_name_t module_name = MODULE_NAME_BT;
   //wakeup_source_t wakeup_source = WAKEUP_SOURCE_INT_SYSTEM_WAKE;
   sys_hal_wifiorbt_wakeup_enable(module_name);
   //aon_pmu_hal_set_wakeup_source(wakeup_source);
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
void sys_hal_wakeup_interrupt_clear(wakeup_source_t interrupt_source)
{

}
void sys_hal_cpu_clk_div_set(uint32_t core_index, uint32_t value)
{
	if(core_index == 0)
	{
		sys_ll_set_cpu0_int_halt_clk_op_cpu0_clk_div(value);
	}
	else
	{
		sys_ll_set_cpu1_int_halt_clk_op_cpu1_clk_div(value);
	}
}
uint32_t sys_hal_cpu_clk_div_get(uint32_t core_index)
{
	if(core_index == 0)
	{
		return sys_ll_get_cpu0_int_halt_clk_op_cpu0_clk_div();
	}
	else
	{
		return sys_ll_get_cpu1_int_halt_clk_op_cpu1_clk_div();
	}
}
void sys_hal_low_power_hardware_init()
{
	uint32_t param = 0;
	uint32_t  pmu_state = 0;

	param = aon_pmu_hal_reg_get(PMU_REG0);
	param = 0x1; //memcheck bypass
	aon_pmu_hal_reg_set(PMU_REG0,param);

	param = 0;
	param = sys_ll_get_ana_reg5_value();
	param |= (1 << SYS_ANA_REG5_ENCB_POS);//global central bias enable
    sys_ll_set_ana_reg5_value(param);

	param = 0;
	param = sys_ll_get_ana_reg6_value();
	param &= ~((0x1 << SYS_ANA_REG6_EN_SLEEP_POS)|(1 << SYS_ANA_REG6_XTAL_LPMODE_CTRL_POS));
	sys_ll_set_ana_reg6_value(param);

	param = 0;
	param = sys_ll_get_ana_reg6_value();
	param |= ((0x7 << SYS_ANA_REG6_RXTAL_LP_POS)|(0x7 << SYS_ANA_REG6_RXTAL_HP_POS));
	sys_ll_set_ana_reg6_value(param);

	/*set wakeup source*/
	pmu_state =  aon_pmu_hal_reg_get(PMU_REG0x41);
	pmu_state |= BIT_AON_PMU_WAKEUP_ENA;
	aon_pmu_hal_reg_set(PMU_REG0x41,pmu_state);

}
int32 sys_hal_lp_vol_set(uint32_t value)
{
	return 0;
}
uint32_t sys_hal_lp_vol_get()
{
	return 0;
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

	reg = sys_ll_get_cpu0_int_0_31_en_value();
	reg &= ~(param);
	sys_ll_set_cpu0_int_0_31_en_value(reg);

	return 0;
}

int32 sys_hal_int_enable(uint32 param) //CMD_ICU_INT_ENABLE
{
	uint32 reg = 0;

	reg = sys_ll_get_cpu0_int_0_31_en_value();
	reg |= (param);
	sys_ll_set_cpu0_int_0_31_en_value(reg);

	return 0;
}

//NOTICE:Temp add for BK7256 product which has more then 32 Interrupt sources
int32 sys_hal_int_group2_disable(uint32 param)
{
	uint32 reg = 0;

	reg = sys_ll_get_cpu0_int_32_63_en_value();
	reg &= ~(param);
	sys_ll_set_cpu0_int_32_63_en_value(reg);

	return 0;
}

//NOTICE:Temp add for BK7256 product which has more then 32 Interrupt sources
int32 sys_hal_int_group2_enable(uint32 param)
{
	uint32 reg = 0;

	reg = sys_ll_get_cpu0_int_32_63_en_value();
	reg |= (param);
	sys_ll_set_cpu0_int_32_63_en_value(reg);

	return 0;
}

int32 sys_hal_fiq_disable(uint32 param)
{
	uint32 reg = 0;

	reg = sys_ll_get_cpu0_int_32_63_en_value();
	reg &= ~(param);
	sys_ll_set_cpu0_int_32_63_en_value(reg);

	return 0;
}

int32 sys_hal_fiq_enable(uint32 param)
{
	uint32 reg = 0;

	reg = sys_ll_get_cpu0_int_32_63_en_value();
	reg |= (param);
	sys_ll_set_cpu0_int_32_63_en_value(reg);

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
	return sys_ll_get_cpu0_int_0_31_status_value();
}

uint32 sys_hal_get_int_group2_status(void)
{
	return sys_ll_get_cpu0_int_32_63_status_value();
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

	reg = sys_ll_get_cpu0_int_32_63_status_value();
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
	uint32 v = sys_ll_get_cpu_device_clk_enable_value();

	if(CLK_PWR_CTRL_PWR_UP == power_up)
		v |= (1 << dev);
	else
		v &= ~(1 << dev);

	sys_ll_set_cpu_device_clk_enable_value(v);
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
				sys_ll_set_cpu_clk_div_mode1_cksel_timer0(sel_xtal);
			else
				sys_ll_set_cpu_clk_div_mode1_cksel_timer0(sel_clk32);
			break;
		case SYS_SEL_TIMER1:
			if(mode == TIMER_SCLK_XTAL)
				sys_ll_set_cpu_clk_div_mode1_cksel_timer1(sel_xtal);
			else
				sys_ll_set_cpu_clk_div_mode1_cksel_timer1(sel_clk32);
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
            ret = sys_ll_get_cpu_clk_div_mode1_cksel_timer0();
            break;
        }
        case SYS_SEL_TIMER1:
        {
            ret = sys_ll_get_cpu_clk_div_mode1_cksel_timer1();
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

/*wake up control start*/
void sys_hal_arm_wakeup_enable(uint32_t param)
{
	/*
	uint32_t reg = 0;

	reg = aon_pmu_hal_get_wakeup_source_reg();
	reg |= param;
	aon_pmu_hal_set_wakeup_source_reg(reg);
	*/
}

void sys_hal_arm_wakeup_disable(uint32_t param)
{
	/*
	uint32_t reg = 0;

	reg = aon_pmu_hal_get_wakeup_source_reg();
	reg &= ~(param);
	aon_pmu_hal_set_wakeup_source_reg(reg);
	*/
}

uint32_t sys_hal_get_arm_wakeup(void)
{
	return 0;
	//return aon_pmu_hal_get_wakeup_source_reg();
}
/*wake up control end*/

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
}

void sys_hal_sadc_pwr_down(void)
{
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
    sys_ll_set_ana_reg6_en_tempdet(value);
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
        sys_ll_set_cpu_power_sleep_wakeup_bts_sleep_exit_req(1);
    }
    else
    {
        sys_ll_set_cpu_power_sleep_wakeup_bts_sleep_exit_req(0);
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
void sys_hal_lcd_disp_clk_en(uint8_t clk_src_sel, uint8_t clk_div_l, uint8_t clk_div_h, uint8_t int_en,uint8_t clk_always_on)
{
	sys_ll_set_cpu_clk_div_mode1_clkdiv_disp_l(clk_div_l);
	sys_ll_set_cpu_clk_div_mode2_clkdiv_disp_h(clk_div_h);
	sys_ll_set_cpu_clk_div_mode2_cksel_disp( clk_src_sel);
	//sys_ll_set_cpu1_int_0_31_en_cpu1_lcd_int_en( 1);
	sys_ll_set_cpu0_int_0_31_en_cpu0_lcd_int_en( int_en);
	sys_ll_set_cpu_device_clk_enable_disp_cken(1);
	sys_ll_set_cpu_mode_disckg2_disp_disckg(clk_always_on);
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
	sys_ll_set_cpu0_int_0_31_en_cpu0_lcd_int_en(0);
	sys_ll_set_cpu_device_clk_enable_disp_cken(0);
	sys_ll_set_cpu_mode_disckg2_disp_disckg(0);
}

/**
  * @brief	dma2d system config
  * param1: clk source sel 0:clk_320M	   1:clk_480M,
  * param2: clk_always_on  ENABLE,0: bus clock auto open when module is select,1:bus clock always open
  * param1: int_en eanble lcd cpu int
  * return none
  */
void sys_hal_dma2d_clk_en(uint8_t clk_always_on, uint8_t sys_int_en)
{
	sys_ll_set_cpu_mode_disckg2_dma2d_disckg(clk_always_on);
	sys_ll_set_cpu0_int_0_31_en_cpu0_dma2d_int_en(sys_int_en);
}

void sys_hal_jpeg_dec_ctrl(bool clk_always_on, bool int_en)
{
	sys_ll_set_cpu_mode_disckg1_jpeg_dec_disckg(clk_always_on);
	sys_ll_set_cpu0_int_0_31_en_cpu0_jpegdec_int_en(int_en);
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
	//TODO
}

uint32_t sys_hal_get_vdd_value(void)
{
	//TODO reg0x43 Write only
	return 4;
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
    sys_ll_set_ana_reg5_xtalh_ctune(value);
}

void sys_hal_analog_set(analog_reg_t reg, uint32_t value)
{
    analog_address_map_t analog_address_map_table[] = ANALOG_ADDRESS_MAP;
    analog_address_map_t *analog_addr_map = &analog_address_map_table[reg];

    uint32_t analog_reg_address = analog_addr_map->analog_reg_address;

	sys_ll_set_analog_reg_value(analog_reg_address, value);
}
uint32_t sys_hal_analog_get(analog_reg_t reg)
{
	analog_address_map_t analog_address_map_table[] = ANALOG_ADDRESS_MAP;
	analog_address_map_t *analog_addr_map = &analog_address_map_table[reg];

	uint32_t analog_reg_address = analog_addr_map->analog_reg_address;

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
    return sys_ll_get_ana_reg5_xtalh_ctune();
}

uint32_t sys_hal_get_bgcalm(void)
{
    return sys_ll_get_ana_reg5_bgcalm();
}

void sys_hal_set_bgcalm(uint32_t value)
{
    sys_ll_set_ana_reg5_bgcalm(value);
}

void sys_hal_set_audioen(uint32_t value)
{
	sys_ll_set_ana_reg11_audioen(value);
}

void sys_hal_set_dpll_div_cksel(uint32_t value)
{
    sys_ll_set_ana_reg11_cksel(value);
}

void sys_hal_set_dpll_reset(uint32_t value)
{
    sys_ll_set_ana_reg11_reset(value);
}

void sys_hal_set_gadc_ten(uint32_t value)
{
    sys_ll_set_ana_reg7_gadc_ten(value);
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

void sys_hal_aud_vdd1v_en(uint32_t value)
{
	sys_ll_set_ana_reg12_enaudvdd1v(value);
}

void sys_hal_aud_vdd1v5_en(uint32_t value)
{
	sys_ll_set_ana_reg12_enaudvdd1v5(value);
}

void sys_hal_aud_mic1_en(uint32_t value)
{
	sys_ll_set_ana_reg14_micen(value);
}

void sys_hal_aud_mic2_en(uint32_t value)
{
	sys_ll_set_ana_reg15_micen(value);
}

void sys_hal_aud_audpll_en(uint32_t value)
{
	sys_ll_set_ana_reg6_en_audpll(value);
}

void sys_hal_aud_dacdrv_en(uint32_t value)
{
	sys_ll_set_ana_reg16_dacdrven(value);
}

void sys_hal_aud_bias_en(uint32_t value)
{
	sys_ll_set_ana_reg17_enbias(value);
}

void sys_hal_aud_dacr_en(uint32_t value)
{
	sys_ll_set_ana_reg16_dacren(value);
}

void sys_hal_aud_dacl_en(uint32_t value)
{
	sys_ll_set_ana_reg16_daclen(value);
}

void sys_hal_aud_rvcmd_en(uint32_t value)
{
	sys_ll_set_ana_reg16_renvcmd(value);
}

void sys_hal_aud_lvcmd_en(uint32_t value)
{
	sys_ll_set_ana_reg16_lenvcmd(value);
}

void sys_hal_aud_micbias1v_en(uint32_t value)
{
	sys_ll_set_ana_reg12_enmicbias1v(value);
}

void sys_hal_aud_micbias_trim_set(uint32_t value)
{
	sys_ll_set_ana_reg12_micbias_trim(value);
}

void sys_hal_aud_mic_rst_set(uint32_t value)
{
	sys_ll_set_ana_reg13_rst(value);
}

void sys_hal_aud_mic1_gain_set(uint32_t value)
{
	sys_ll_set_ana_reg14_micgain(value);
}

void sys_hal_aud_mic2_gain_set(uint32_t value)
{
	sys_ll_set_ana_reg15_micgain(value);
}

void sys_hal_aud_int_en(uint32_t value)
{
	sys_ll_set_cpu0_int_0_31_en_cpu0_aud_int_en(value);
}

void sys_hal_aud_power_en(uint32_t value)
{
	sys_ll_set_cpu_power_sleep_wakeup_pwd_audp(value);
}

/**  Audio End  **/

/**  FFT Start  **/

void sys_hal_fft_disckg_set(uint32_t value)
{
	sys_ll_set_cpu_mode_disckg1_fft_disckg(value);
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

void sys_hal_i2s_disckg_set(uint32_t value)
{
	sys_ll_set_cpu_mode_disckg1_i2s_disckg(value);
}

void sys_hal_i2s_int_en(uint32_t value)
{
	sys_ll_set_cpu0_int_0_31_en_cpu0_i2s_int_en(value);
}

void sys_hal_apll_en(uint32_t value)
{
	sys_ll_set_ana_reg6_en_audpll(value);
}

void sys_hal_cb_manu_val_set(uint32_t value)
{
	sys_ll_set_ana_reg4_cb_manu_val(value);
}

void sys_hal_ana_reg11_vsel_set(uint32_t value)
{
	sys_ll_set_ana_reg11_vsel(value);
}

void sys_hal_ana_reg10_sdm_val_set(uint32_t value)
{
	sys_ll_set_ana_reg10_sdm_val(value);
}

void sys_hal_ana_reg11_spi_trigger_set(uint32_t value)
{
	sys_ll_set_ana_reg11_spi_trigger(value);
}

/**  I2S End  **/

/**  Touch Start **/
void sys_hal_touch_power_down(uint32_t value)
{
	sys_ll_set_ana_reg8_pwd_td(value);
}

void sys_hal_touch_sensitivity_level_set(uint32_t value)
{
	sys_ll_set_ana_reg8_gain_s(value);
}

void sys_hal_touch_scan_mode_enable(uint32_t value)
{
	sys_ll_set_ana_reg8_en_scm(value);
}

void sys_hal_touch_detect_threshold_set(uint32_t value)
{
	sys_ll_set_ana_reg9_vrefs(value);
}

void sys_hal_touch_detect_range_set(uint32_t value)
{
	sys_ll_set_ana_reg9_crg(value);
}

void sys_hal_touch_calib_enable(uint32_t value)
{
	sys_ll_set_ana_reg9_en_cal(value);
}

void sys_hal_touch_manul_mode_calib_value_set(uint32_t value)
{
	sys_ll_set_ana_reg8_cap_calspi(value);
}

void sys_hal_touch_manul_mode_enable(uint32_t value)
{
	sys_ll_set_ana_reg9_man_mode(value);
}

void sys_hal_touch_scan_mode_chann_set(uint32_t value)
{
	sys_ll_set_ana_reg8_chs_scan(value);
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
	sys_ll_set_cpu_mode_disckg1_jpeg_disckg(value);
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

/** jpeg end **/

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

void sys_hal_psram_power_enable(void)
{
	uint32_t value = sys_ll_get_ana_reg6_value();
	value |= (0x1 << 12) | (0x1 << 7) | (0x1 << 5) | (0x1 << 8) | (0x1 << 9);
	sys_ll_set_ana_reg6_value(value);
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
	sys_ll_set_ana_reg5_trxt_tst_enable(value);
}


void sys_hal_set_ana_scal_en(uint32_t value)
{
	sys_ll_set_ana_reg7_scal_en(value);
}


void sys_hal_set_ana_gadc_buf_ictrl(uint32_t value)
{
   sys_ll_set_ana_reg7_gadc_buf_ictrl(value);
}

void sys_hal_set_ana_gadc_cmp_ictrl(uint32_t value)
{
    sys_ll_set_ana_reg7_gadc_cmp_ictrl(value);
}

void sys_hal_set_ana_pwd_gadc_buf(uint32_t value)
{
	sys_ll_set_ana_reg6_pwd_gadc_buf(value);
}

void sys_hal_set_ana_vref_sel(uint32_t value)
{
	sys_ll_set_ana_reg7_vref_sel(value);
}
void sys_hal_set_ana_cb_cal_manu(uint32_t value)
{
    sys_ll_set_ana_reg4_cb_cal_manu(value);
}

void sys_hal_set_ana_cb_cal_trig(uint32_t value)
{
    sys_ll_set_ana_reg4_cb_cal_trig(value);
}
void sys_hal_set_ana_vlsel_ldodig(uint32_t value)
{
    sys_ll_set_ana_reg3_vlsel_ldodig(value);
}
void sys_hal_set_ana_vhsel_ldodig(uint32_t value)
{
    sys_ll_set_ana_reg3_vhsel_ldodig(value);
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
    sys_ll_set_ana_reg5_vctrl_sysldo(value);
}

