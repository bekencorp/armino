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
#include "platform.h"
#include <arch_interrupt.h>
#include "modules/pm.h"
#include "bk_pm_control.h"

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
	sys_ll_set_cpu_clk_div_mode2_cksel_flash(FLASH_CLK_120M);
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
__attribute__((section(".itcm_sec_code"))) void sys_hal_enter_deep_sleep(void * param)
{
    uint32_t modules_power_state=0;
	uint32_t  clock_value = 0;
	uint32_t  pmu_val2 = 0;
	int       ret = 0;
	/*mask all interner interrupt*/
	sys_ll_set_cpu0_int_halt_clk_op_cpu0_int_mask(1);

	/*1.switch cpu clock to xtal26m*/
	sys_ll_set_cpu_clk_div_mode1_cksel_core(0);
	sys_ll_set_cpu_clk_div_mode1_clkdiv_core(0);
	sys_ll_set_cpu_clk_div_mode1_clkdiv_bus(0);

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

	clock_value &= ~((1 << SYS_ANA_REG6_EN_SYSLDO_POS)|(1 << SYS_ANA_REG6_PWD_GADC_BUF_POS)|(1 << SYS_ANA_REG6_EN_TEMPDET_POS));//0:vddaon drop enable

	sys_ll_set_ana_reg6_value(clock_value);

	clock_value = 0;
	clock_value = sys_ll_get_ana_reg5_value();
	clock_value &= ~(1 << SYS_ANA_REG5_ENCB_POS);//global central bias enable
	sys_ll_set_ana_reg5_value(clock_value);

	clock_value = 0;
	clock_value = sys_ll_get_ana_reg9_value();
	clock_value &= ~(1 << 5);
	sys_ll_set_ana_reg9_value(clock_value);

	clock_value = 0;
    sys_ll_set_ana_reg19_value(clock_value);

	/*4.set PMU parameters*/
    ret = aon_pmu_hal_set_sleep_parameters(0x2);
	if(ret== -1)
	{
		return;
	}
    /*5.set power flag*/
	modules_power_state = sys_ll_get_cpu_power_sleep_wakeup_value();
	modules_power_state |= 0xa0000;
	sys_ll_set_cpu_power_sleep_wakeup_value(modules_power_state);

	/*6.set sleep flag*/
	pmu_val2 =  aon_pmu_hal_reg_get(PMU_REG2);
	pmu_val2 |= BIT(BIT_SLEEP_FLAG_DEEP_SLEEP);
	aon_pmu_hal_reg_set(PMU_REG2,pmu_val2);

	/*8.WFI*/
	__asm volatile( "wfi" );

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

#define BIT_AON_PMU_WAKEUP_ENA                   (0x1F0U)
#define PM_HARDEARE_DELAY_TIME_FROM_LOWVOL_WAKE  (0x4)
#define PM_EXIT_WFI_FROM_LOWVOL_WAIT_COUNT       (100)// about 4us(using 26m clock)
//specify:low voltage process can't be interrupt or the system can't response external interrupt after wakeup.
#if 1
#define MTIMER_LOW_VOLTAGE_MINIMUM_TICK (10400)	//26M, 400 us
extern void mtimer_reset_next_tick(uint32_t minimum_offset);
#define CONFIG_LOW_VOLTAGE_DEBUG 0
#if CONFIG_LOW_VOLTAGE_DEBUG
uint64_t g_low_voltage_tick = 0;
extern u64 riscv_get_mtimer(void);
#endif
#endif
extern uint32_t s_pm_wakeup_from_lowvol_consume_time;
__attribute__((section(".itcm_sec_code"))) void sys_hal_enter_low_voltage(void)
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
	uint32_t  h_vol = 0;
	int       ret = 0;
	uint32_t  analog_clk = 0;
	uint32_t  center_bias = 0;
	uint32_t  en_bias_5u = 0;
	//uint32_t  count = 0;

#if CONFIG_LOW_VOLTAGE_DEBUG
	uint64_t start_tick = riscv_get_mtimer();
#endif

	clear_csr(NDS_MIE, MIP_MTIP);

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

	//confirm here hasn't mtimer/external interrupt
	if(arch_get_plic_pending_status() ||
		mtimer_is_timeout())
	{
		sys_ll_set_cpu0_int_0_31_en_value(int_state1);
		sys_ll_set_cpu0_int_32_63_en_value(int_state2);

		return;
	}

	//below interval time is about 5240(maybe CPU/Flash clock changes):
	//after sys_ll_set_cpu0_int_halt_clk_op_cpu0_int_mask
	//before WFI
	mtimer_reset_next_tick(MTIMER_LOW_VOLTAGE_MINIMUM_TICK);

    /*mask all external interrupt*/
	sys_ll_set_cpu0_int_halt_clk_op_cpu0_int_mask(1);

	//gpio_disable_output();
	clk_div_val0 =  sys_hal_all_modules_clk_div_get(CLK_DIV_REG0);
	clk_div_val1 =  sys_hal_all_modules_clk_div_get(CLK_DIV_REG1);
	clk_div_val2 =  sys_hal_all_modules_clk_div_get(CLK_DIV_REG2);

	sys_ll_set_ana_reg2_spi_latchb(0x1);
	h_vol = sys_ll_get_ana_reg3_vhsel_ldodig();
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
	analog_clk  = clock_value;

	clock_value |= (1 << SYS_ANA_REG6_EN_SLEEP_POS);//enable xtal26m sleep
	//clock_value &= ~((1 << SYS_ANA_REG6_XTAL_LPMODE_CTRL_POS)|(1 << SYS_ANA_REG6_EN_DPLL_POS)|(1 << SYS_ANA_REG6_EN_AUDPLL_POS)|(1 << SYS_ANA_REG6_EN_PSRAM_LDO_POS)|(1 << SYS_ANA_REG6_EN_DCO_POS)|(1 << SYS_ANA_REG6_EN_XTALL_POS)|(1 << SYS_ANA_REG6_EN_USB_POS));
	clock_value &= ~((1 << SYS_ANA_REG6_EN_DPLL_POS)|(1 << SYS_ANA_REG6_EN_USB_POS)|(1 << SYS_ANA_REG6_EN_AUDPLL_POS)|(1 << SYS_ANA_REG6_EN_PSRAM_LDO_POS)|(1 << SYS_ANA_REG6_EN_DCO_POS));

	clock_value &= ~((1 << SYS_ANA_REG6_EN_SYSLDO_POS)|(1 << SYS_ANA_REG6_PWD_GADC_BUF_POS)|(1 << SYS_ANA_REG6_EN_TEMPDET_POS));

	sys_ll_set_ana_reg6_value(clock_value);


	clock_value = 0;
	clock_value = sys_ll_get_ana_reg5_value();
	center_bias = clock_value;
	clock_value &= ~(1 << SYS_ANA_REG5_ENCB_POS);//global central bias enable
	sys_ll_set_ana_reg5_value(clock_value);

	clock_value = 0;
	clock_value = sys_ll_get_ana_reg9_value();
	en_bias_5u  = clock_value;
	clock_value &= ~(1 << 5);
	sys_ll_set_ana_reg9_value(clock_value);


	/*4.set sleep parameters*/
	ret = aon_pmu_hal_set_sleep_parameters(0x1);
	if(ret== -1)
	{
		return;
	}

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

//just debug:maybe some guys changed the CPU clock or Flash clock caused the time of
//MTIMER_LOW_VOLTAGE_MINIMUM_TICK isn't enough.
//here can statistic the MAX time value.
#if CONFIG_LOW_VOLTAGE_DEBUG
	if(g_low_voltage_tick < riscv_get_mtimer() - start_tick)
		g_low_voltage_tick = riscv_get_mtimer() - start_tick;
#endif

	/*6.WFI*/
	while(1)
	{
		/*6.WFI*/
		__asm volatile( "wfi" );
		extern u32 arch_get_int_status(void);
		if(arch_get_int_status() != 0)
		{
			break;
		}
	}
#if 0
	__asm volatile( "wfi" );
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
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	for(count = PM_EXIT_WFI_FROM_LOWVOL_WAIT_COUNT; count > 0; count--);//for protect stability when exit wfi or halt cpu
#endif
	s_pm_wakeup_from_lowvol_consume_time = 0;
	s_pm_wakeup_from_lowvol_consume_time = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	sys_ll_set_ana_reg2_spi_latchb(0x1);
	sys_ll_set_ana_reg3_vhsel_ldodig(h_vol);
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
	s_pm_wakeup_from_lowvol_consume_time -= ((aon_pmu_ll_get_reg40_wake1_delay()+1)+(aon_pmu_ll_get_reg40_wake2_delay()+1)+(aon_pmu_ll_get_reg40_wake3_delay()+1))+PM_HARDEARE_DELAY_TIME_FROM_LOWVOL_WAKE;
	/*7.restore state before low voltage*/
	modules_power_state = 0;
	modules_power_state = sys_ll_get_cpu_power_sleep_wakeup_value();
	modules_power_state &= ~0xf0000;
	sys_ll_set_cpu_power_sleep_wakeup_value(modules_power_state);

	/*8.restore the analog clk*/
	clock_value = 0;
	clock_value = sys_ll_get_ana_reg6_value();
	//clock_value |= ((1 << SYS_ANA_REG6_EN_DPLL_POS)|(1 << SYS_ANA_REG6_EN_USB_POS)|(1 << SYS_ANA_REG6_EN_PSRAM_LDO_POS));//en_dpll, en_usb,en_PSRAM_LDO
	clock_value |= analog_clk;
	clock_value &= ~(1 << SYS_ANA_REG6_EN_SLEEP_POS);//disable xtal26m sleep
	sys_ll_set_ana_reg6_value(clock_value);

	clock_value = 0;
	clock_value = sys_ll_get_ana_reg5_value();
	clock_value |= center_bias;
	sys_ll_set_ana_reg5_value(clock_value);

	clock_value = 0;
	clock_value = sys_ll_get_ana_reg9_value();
	clock_value |= en_bias_5u;
	sys_ll_set_ana_reg9_value(clock_value);


	previous_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);

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

	if(pm_wake_int_flag2&(WIFI_MAC_GEN_INT_BIT))
	{
		ps_switch(PS_UNALLOW, PS_EVENT_STA, PM_RF_BIT);
		bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_PHY_WIFI,PM_POWER_MODULE_STATE_ON);
	}

	sys_ll_set_cpu0_int_0_31_en_value(int_state1);
	sys_ll_set_cpu0_int_32_63_en_value(int_state2);

	set_csr(NDS_MIE, MIP_MTIP);

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
    __asm volatile( "wfi" );
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
	uint32_t value = 0;
	if((module >= POWER_MODULE_NAME_MEM1) && (module <= POWER_MODULE_NAME_WIFI_PHY))
	{
		value = 0;
		value = sys_ll_get_cpu_power_sleep_wakeup_value();
		value = ((value >> module) & 0x1);
		return value;
	}
    else if(module == POWER_MODULE_NAME_CPU1)
    {
    	//sys_ll_get_cpu1_int_halt_clk_op_cpu1_pwr_dw();
    	value = 0;
    	return sys_ll_get_cpu_current_run_status_cpu1_pwr_dw_state();
    }
	return -1;
}
void sys_hal_module_RF_power_ctrl (module_name_t module,power_module_state_t power_state)
{
    uint32_t value = 0;
	value = sys_ll_get_ana_reg6_value();
    if(power_state == POWER_MODULE_STATE_ON)//power on
    {
		value |= ((1 << SYS_ANA_REG6_EN_SYSLDO_POS)|(1 << SYS_ANA_REG6_EN_DPLL_POS)|(1 << SYS_ANA_REG6_EN_DCO_POS));//en_sysldo,en_dpll
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

	param = 0x4;
	sys_ll_set_ana_reg2_spi_latchb(0x1);
	sys_ll_set_ana_reg3_vhsel_ldodig(param);

	sys_ll_set_ana_reg0_band(0x13);//open the dpll 320m

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
	sys_ll_set_ana_reg2_spi_latchb(0x1);
	sys_ll_set_ana_reg3_vlsel_ldodig(value);
	return 0;
}
uint32_t sys_hal_lp_vol_get()
{
	sys_ll_set_ana_reg2_spi_latchb(0x1);
	return sys_ll_get_ana_reg3_vlsel_ldodig();
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
		case SPI_ID_1:
			if(mode == SPI_CLK_XTAL)
				sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi1(sel_xtal);
			else
				sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi1(sel_apll);
			break;

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
	uint32_t reg = 0;

	reg = aon_pmu_hal_get_wakeup_source_reg();
	reg |= param;
	aon_pmu_hal_set_wakeup_source_reg(reg);
}

void sys_hal_arm_wakeup_disable(uint32_t param)
{
	uint32_t reg = 0;

	reg = aon_pmu_hal_get_wakeup_source_reg();
	reg &= ~(param);
	aon_pmu_hal_set_wakeup_source_reg(reg);
}

uint32_t sys_hal_get_arm_wakeup(void)
{
	return aon_pmu_hal_get_wakeup_source_reg();
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

uint32_t sys_hal_nmi_wdt_get_clk_div(void)
{
    return sys_ll_get_cpu_26m_wdt_clk_div_ckdiv_wdt();
}

void sys_hal_nmi_wdt_set_clk_div(uint32_t value)
{
    sys_ll_set_cpu_26m_wdt_clk_div_ckdiv_wdt(value);
}

void sys_hal_trng_disckg_set(uint32_t value)
{
	sys_ll_set_cpu_mode_disckg1_trng_disckg(value);
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
	//sys_ll_set_cpu_device_clk_enable_disp_cken(1);
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
	//sys_ll_set_cpu_device_clk_enable_disp_cken(0);
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
// 		sys_ll_set_ana_reg7_gadc_ten(value);
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

void sys_hal_aud_aud_en(uint32_t value)
{
	sys_ll_set_ana_reg6_en_aud(value);
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
	value |= (0x1 << 9);
	//value &= ~(0x1 << 5);
	sys_ll_set_ana_reg6_value(value);
}

void sys_hal_psram_psldo_vsel(uint32_t value)
{
	sys_ll_set_ana_reg9_psldo_vsel(value);
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


/* generated codes by python tool with address mapping */
#if 0	//just for reference
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

bk_err_t sys_hal_init(void)
{
    s_sys_hal.hw = (sys_hw_t *)SYS_LL_REG_BASE;
    return BK_OK;
}

/* REG_0x00 */

uint32_t sys_hal_get_device_id_value(sys_hw_t *hw)
{
    return sys_ll_get_device_id_value(hw);
}

/* REG_0x00:device_id->DeviceID: ,RO,0x0[31:0]*/
uint32_t sys_hal_get_device_id_deviceid(sys_hw_t *hw)
{
    return sys_ll_get_device_id_deviceid(hw);
}

/* REG_0x01 */

uint32_t sys_hal_get_version_id_value(sys_hw_t *hw)
{
    return sys_ll_get_version_id_value(hw);
}

/* REG_0x01:version_id->VersionID: ,RO,0x1[31:0]*/
uint32_t sys_hal_get_version_id_versionid(sys_hw_t *hw)
{
    return sys_ll_get_version_id_versionid(hw);
}

/* REG_0x02 */

uint32_t sys_hal_get_cpu_current_run_status_value(sys_hw_t *hw)
{
    return sys_ll_get_cpu_current_run_status_value(hw);
}

/* REG_0x02:cpu_current_run_status->core0_halted:core0 halt indicate,RO,0x2[0]*/
uint32_t sys_hal_get_cpu_current_run_status_core0_halted(sys_hw_t *hw)
{
    return sys_ll_get_cpu_current_run_status_core0_halted(hw);
}

/* REG_0x02:cpu_current_run_status->core1_halted:core1 halt indicate,RO,0x2[1]*/
uint32_t sys_hal_get_cpu_current_run_status_core1_halted(sys_hw_t *hw)
{
    return sys_ll_get_cpu_current_run_status_core1_halted(hw);
}

/* REG_0x02:cpu_current_run_status->cpu0_sw_reset:cpu0_sw_reset indicate,RO,0x2[4]*/
uint32_t sys_hal_get_cpu_current_run_status_cpu0_sw_reset(sys_hw_t *hw)
{
    return sys_ll_get_cpu_current_run_status_cpu0_sw_reset(hw);
}

/* REG_0x02:cpu_current_run_status->cpu1_sw_reset:cpu1_sw_reset indicate,RO,0x2[5]*/
uint32_t sys_hal_get_cpu_current_run_status_cpu1_sw_reset(sys_hw_t *hw)
{
    return sys_ll_get_cpu_current_run_status_cpu1_sw_reset(hw);
}

/* REG_0x02:cpu_current_run_status->cpu0_pwr_dw_state:cpu0_pwr_dw_state,RO,0x2[8]*/
uint32_t sys_hal_get_cpu_current_run_status_cpu0_pwr_dw_state(sys_hw_t *hw)
{
    return sys_ll_get_cpu_current_run_status_cpu0_pwr_dw_state(hw);
}

/* REG_0x02:cpu_current_run_status->cpu1_pwr_dw_state:cpu1_pwr_dw_state,RO,0x2[9]*/
uint32_t sys_hal_get_cpu_current_run_status_cpu1_pwr_dw_state(sys_hw_t *hw)
{
    return sys_ll_get_cpu_current_run_status_cpu1_pwr_dw_state(hw);
}

/* REG_0x03 */

uint32_t sys_hal_get_cpu_storage_connect_op_select_value(sys_hw_t *hw)
{
    return sys_ll_get_cpu_storage_connect_op_select_value(hw);
}

void sys_hal_set_cpu_storage_connect_op_select_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_storage_connect_op_select_value(hw, value);
}

/* REG_0x03:cpu_storage_connect_op_select->boot_mode:0:ROM boot 1:FLASH boot,R/W,0x3[0]*/
uint32_t sys_hal_get_cpu_storage_connect_op_select_boot_mode(sys_hw_t *hw)
{
    return sys_ll_get_cpu_storage_connect_op_select_boot_mode(hw);
}

void sys_hal_set_cpu_storage_connect_op_select_boot_mode(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_storage_connect_op_select_boot_mode(hw, value);
}

/* REG_0x03:cpu_storage_connect_op_select->jtag_core_sel:0:jtag connect core0, 1:jtag connect core1,R/W,0x3[8]*/
uint32_t sys_hal_get_cpu_storage_connect_op_select_jtag_core_sel(sys_hw_t *hw)
{
    return sys_ll_get_cpu_storage_connect_op_select_jtag_core_sel(hw);
}

void sys_hal_set_cpu_storage_connect_op_select_jtag_core_sel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_storage_connect_op_select_jtag_core_sel(hw, value);
}

/* REG_0x03:cpu_storage_connect_op_select->flash_sel:0: normal flash operation 1:flash download by spi,R/W,0x3[9]*/
uint32_t sys_hal_get_cpu_storage_connect_op_select_flash_sel(sys_hw_t *hw)
{
    return sys_ll_get_cpu_storage_connect_op_select_flash_sel(hw);
}

void sys_hal_set_cpu_storage_connect_op_select_flash_sel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_storage_connect_op_select_flash_sel(hw, value);
}

/* REG_0x04 */

uint32_t sys_hal_get_cpu0_int_halt_clk_op_value(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_halt_clk_op_value(hw);
}

void sys_hal_set_cpu0_int_halt_clk_op_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_halt_clk_op_value(hw, value);
}

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_sw_rst:reserved,R/W,0x4[0]*/
uint32_t sys_hal_get_cpu0_int_halt_clk_op_cpu0_sw_rst(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_halt_clk_op_cpu0_sw_rst(hw);
}

void sys_hal_set_cpu0_int_halt_clk_op_cpu0_sw_rst(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_halt_clk_op_cpu0_sw_rst(hw, value);
}

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_pwr_dw:reserved,R/W,0x4[1]*/
uint32_t sys_hal_get_cpu0_int_halt_clk_op_cpu0_pwr_dw(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_halt_clk_op_cpu0_pwr_dw(hw);
}

void sys_hal_set_cpu0_int_halt_clk_op_cpu0_pwr_dw(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_halt_clk_op_cpu0_pwr_dw(hw, value);
}

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_int_mask:cpu0 int mask,R/W,0x4[2]*/
uint32_t sys_hal_get_cpu0_int_halt_clk_op_cpu0_int_mask(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_halt_clk_op_cpu0_int_mask(hw);
}

void sys_hal_set_cpu0_int_halt_clk_op_cpu0_int_mask(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_halt_clk_op_cpu0_int_mask(hw, value);
}

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_halt:core0 halt indicate,R/W,0x4[3]*/
uint32_t sys_hal_get_cpu0_int_halt_clk_op_cpu0_halt(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_halt_clk_op_cpu0_halt(hw);
}

void sys_hal_set_cpu0_int_halt_clk_op_cpu0_halt(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_halt_clk_op_cpu0_halt(hw, value);
}

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_clk_div:Frequency division : F/(1+N), N is the data of the reg value,R/W,0x4[7:4]*/
uint32_t sys_hal_get_cpu0_int_halt_clk_op_cpu0_clk_div(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_halt_clk_op_cpu0_clk_div(hw);
}

void sys_hal_set_cpu0_int_halt_clk_op_cpu0_clk_div(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_halt_clk_op_cpu0_clk_div(hw, value);
}

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_offset:reserved,RO,0x4[31:8]*/
uint32_t sys_hal_get_cpu0_int_halt_clk_op_cpu0_offset(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_halt_clk_op_cpu0_offset(hw);
}

/* REG_0x05 */

uint32_t sys_hal_get_cpu1_int_halt_clk_op_value(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_halt_clk_op_value(hw);
}

void sys_hal_set_cpu1_int_halt_clk_op_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_halt_clk_op_value(hw, value);
}

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_sw_rst:reserved,R/W,0x5[0]*/
uint32_t sys_hal_get_cpu1_int_halt_clk_op_cpu1_sw_rst(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_halt_clk_op_cpu1_sw_rst(hw);
}

void sys_hal_set_cpu1_int_halt_clk_op_cpu1_sw_rst(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_halt_clk_op_cpu1_sw_rst(hw, value);
}

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_pwr_dw:reserved,R/W,0x5[1]*/
uint32_t sys_hal_get_cpu1_int_halt_clk_op_cpu1_pwr_dw(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_halt_clk_op_cpu1_pwr_dw(hw);
}

void sys_hal_set_cpu1_int_halt_clk_op_cpu1_pwr_dw(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_halt_clk_op_cpu1_pwr_dw(hw, value);
}

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_int_mask:cpu1 int mask,R/W,0x5[2]*/
uint32_t sys_hal_get_cpu1_int_halt_clk_op_cpu1_int_mask(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_halt_clk_op_cpu1_int_mask(hw);
}

void sys_hal_set_cpu1_int_halt_clk_op_cpu1_int_mask(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_halt_clk_op_cpu1_int_mask(hw, value);
}

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_halt:core1 halt indicate,R/W,0x5[3]*/
uint32_t sys_hal_get_cpu1_int_halt_clk_op_cpu1_halt(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_halt_clk_op_cpu1_halt(hw);
}

void sys_hal_set_cpu1_int_halt_clk_op_cpu1_halt(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_halt_clk_op_cpu1_halt(hw, value);
}

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_clk_div:Frequency division : F/(1+N), N is the data of the reg value,R/W,0x5[7:4]*/
uint32_t sys_hal_get_cpu1_int_halt_clk_op_cpu1_clk_div(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_halt_clk_op_cpu1_clk_div(hw);
}

void sys_hal_set_cpu1_int_halt_clk_op_cpu1_clk_div(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_halt_clk_op_cpu1_clk_div(hw, value);
}

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_offset:reserved,R/W,0x5[31:8]*/
uint32_t sys_hal_get_cpu1_int_halt_clk_op_cpu1_offset(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_halt_clk_op_cpu1_offset(hw);
}

void sys_hal_set_cpu1_int_halt_clk_op_cpu1_offset(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_halt_clk_op_cpu1_offset(hw, value);
}

/* REG_0x06 */

/* REG_0x08 */

uint32_t sys_hal_get_cpu_clk_div_mode1_value(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_value(hw);
}

void sys_hal_set_cpu_clk_div_mode1_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_value(hw, value);
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_core:Frequency division : F/(1+N), N is the data of the reg value,R/W,0x8[3:0]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_clkdiv_core(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_clkdiv_core(hw);
}

void sys_hal_set_cpu_clk_div_mode1_clkdiv_core(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_clkdiv_core(hw, value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_core:0: clk_DCO      1 : XTAL      2 : 320M      3 : 480M,R/W,0x8[5:4]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_core(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_cksel_core(hw);
}

void sys_hal_set_cpu_clk_div_mode1_cksel_core(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_cksel_core(hw, value);
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_bus:Frequency division : F/(1+N), N is the data of the reg value,R/W,0x8[6]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_clkdiv_bus(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_clkdiv_bus(hw);
}

void sys_hal_set_cpu_clk_div_mode1_clkdiv_bus(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_clkdiv_bus(hw, value);
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_uart0:Frequency division :    0:/1  1:/2  2:/4  3:/8,R/W,0x8[9:8]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_clkdiv_uart0(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_clkdiv_uart0(hw);
}

void sys_hal_set_cpu_clk_div_mode1_clkdiv_uart0(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_clkdiv_uart0(hw, value);
}

/* REG_0x08:cpu_clk_div_mode1->clksel_uart0:0:XTAL              1:APLL,R/W,0x8[10]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_clksel_uart0(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_clksel_uart0(hw);
}

void sys_hal_set_cpu_clk_div_mode1_clksel_uart0(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_clksel_uart0(hw, value);
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_uart1:Frequency division :    0:/1  1:/2  2:/4  3:/8,R/W,0x8[12:11]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_clkdiv_uart1(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_clkdiv_uart1(hw);
}

void sys_hal_set_cpu_clk_div_mode1_clkdiv_uart1(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_clkdiv_uart1(hw, value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_uart1:0:XTAL              1:APLL,R/W,0x8[13]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_uart1(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_cksel_uart1(hw);
}

void sys_hal_set_cpu_clk_div_mode1_cksel_uart1(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_cksel_uart1(hw, value);
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_uart2:Frequency division :    0:/1  1:/2  2:/4  3:/8,R/W,0x8[15:14]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_clkdiv_uart2(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_clkdiv_uart2(hw);
}

void sys_hal_set_cpu_clk_div_mode1_clkdiv_uart2(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_clkdiv_uart2(hw, value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_uart2:0:XTAL              1:APLL,R/W,0x8[16]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_uart2(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_cksel_uart2(hw);
}

void sys_hal_set_cpu_clk_div_mode1_cksel_uart2(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_cksel_uart2(hw, value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_sadc:0:XTAL              1:APLL,R/W,0x8[17]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_sadc(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_cksel_sadc(hw);
}

void sys_hal_set_cpu_clk_div_mode1_cksel_sadc(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_cksel_sadc(hw, value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_pwm0:0:clk32              1:XTAL,R/W,0x8[18]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_pwm0(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_cksel_pwm0(hw);
}

void sys_hal_set_cpu_clk_div_mode1_cksel_pwm0(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_cksel_pwm0(hw, value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_pwm1:0:clk32              1:XTAL,R/W,0x8[19]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_pwm1(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_cksel_pwm1(hw);
}

void sys_hal_set_cpu_clk_div_mode1_cksel_pwm1(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_cksel_pwm1(hw, value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_timer0:0:clk32              1:XTAL,R/W,0x8[20]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_timer0(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_cksel_timer0(hw);
}

void sys_hal_set_cpu_clk_div_mode1_cksel_timer0(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_cksel_timer0(hw, value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_timer1:0:clk32              1:XTAL,R/W,0x8[21]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_timer1(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_cksel_timer1(hw);
}

void sys_hal_set_cpu_clk_div_mode1_cksel_timer1(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_cksel_timer1(hw, value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_timer2:0:clk32              1:XTAL,R/W,0x8[22]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_timer2(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_cksel_timer2(hw);
}

void sys_hal_set_cpu_clk_div_mode1_cksel_timer2(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_cksel_timer2(hw, value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_i2s:0:XTAL              1:APLL,R/W,0x8[24]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_i2s(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_cksel_i2s(hw);
}

void sys_hal_set_cpu_clk_div_mode1_cksel_i2s(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_cksel_i2s(hw, value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_aud:0:XTAL              1:APLL,R/W,0x8[25]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_aud(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_cksel_aud(hw);
}

void sys_hal_set_cpu_clk_div_mode1_cksel_aud(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_cksel_aud(hw, value);
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_jpeg:Frequency division : F/(1+N), N is the data of the reg value,R/W,0x8[29:26]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_clkdiv_jpeg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_clkdiv_jpeg(hw);
}

void sys_hal_set_cpu_clk_div_mode1_clkdiv_jpeg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_clkdiv_jpeg(hw, value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_jpeg:0:clk_320M      1:clk_480M,R/W,0x8[30]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_jpeg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_cksel_jpeg(hw);
}

void sys_hal_set_cpu_clk_div_mode1_cksel_jpeg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_cksel_jpeg(hw, value);
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_disp_l:Frequency division : F/(1+clkdiv_disp_l+clkdiv_disp_h*2),R/W,0x8[31]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_clkdiv_disp_l(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode1_clkdiv_disp_l(hw);
}

void sys_hal_set_cpu_clk_div_mode1_clkdiv_disp_l(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode1_clkdiv_disp_l(hw, value);
}

/* REG_0x09 */

uint32_t sys_hal_get_cpu_clk_div_mode2_value(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode2_value(hw);
}

void sys_hal_set_cpu_clk_div_mode2_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode2_value(hw, value);
}

/* REG_0x09:cpu_clk_div_mode2->clkdiv_disp_h:Frequency division : F/(1+clkdiv_disp_l+clkdiv_disp_h*2),R/W,0x9[2:0]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_clkdiv_disp_h(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode2_clkdiv_disp_h(hw);
}

void sys_hal_set_cpu_clk_div_mode2_clkdiv_disp_h(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode2_clkdiv_disp_h(hw, value);
}

/* REG_0x09:cpu_clk_div_mode2->cksel_disp:0:clk_320M      1:clk_480M,R/W,0x9[3]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_cksel_disp(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode2_cksel_disp(hw);
}

void sys_hal_set_cpu_clk_div_mode2_cksel_disp(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode2_cksel_disp(hw, value);
}

/* REG_0x09:cpu_clk_div_mode2->ckdiv_psram:Frequency division : F/(1+N), N is the data of the reg value,R/W,0x9[4]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_ckdiv_psram(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode2_ckdiv_psram(hw);
}

void sys_hal_set_cpu_clk_div_mode2_ckdiv_psram(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode2_ckdiv_psram(hw, value);
}

/* REG_0x09:cpu_clk_div_mode2->cksel_psram:0:clk_320M      1:clk_480M,R/W,0x9[5]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_cksel_psram(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode2_cksel_psram(hw);
}

void sys_hal_set_cpu_clk_div_mode2_cksel_psram(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode2_cksel_psram(hw, value);
}

/* REG_0x09:cpu_clk_div_mode2->ckdiv_qspi0:Frequency division : F/(1+N), N is the data of the reg value,R/W,0x9[9:6]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_ckdiv_qspi0(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode2_ckdiv_qspi0(hw);
}

void sys_hal_set_cpu_clk_div_mode2_ckdiv_qspi0(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode2_ckdiv_qspi0(hw, value);
}

/* REG_0x09:cpu_clk_div_mode2->ckdiv_sdio:0:/1  1:/2  2:/4  3:/8  4:/16  5:/32  6:/64  7:/256,R/W,0x9[16:14]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_ckdiv_sdio(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode2_ckdiv_sdio(hw);
}

void sys_hal_set_cpu_clk_div_mode2_ckdiv_sdio(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode2_ckdiv_sdio(hw, value);
}

/* REG_0x09:cpu_clk_div_mode2->cksel_sdio:0：XTAL          1：320M,R/W,0x9[17]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_cksel_sdio(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode2_cksel_sdio(hw);
}

void sys_hal_set_cpu_clk_div_mode2_cksel_sdio(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode2_cksel_sdio(hw, value);
}

/* REG_0x09:cpu_clk_div_mode2->ckdiv_auxs:Frequency division : F/(1+N), N is the data of the reg value,R/W,0x9[21:18]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_ckdiv_auxs(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode2_ckdiv_auxs(hw);
}

void sys_hal_set_cpu_clk_div_mode2_ckdiv_auxs(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode2_ckdiv_auxs(hw, value);
}

/* REG_0x09:cpu_clk_div_mode2->cksel_auxs:0:DCO              1:APLL                2:320M                     4:480M,R/W,0x9[23:22]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_cksel_auxs(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode2_cksel_auxs(hw);
}

void sys_hal_set_cpu_clk_div_mode2_cksel_auxs(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode2_cksel_auxs(hw, value);
}

/* REG_0x09:cpu_clk_div_mode2->cksel_flash:0:XTAL              1:APLL               1x :clk_120M,R/W,0x9[25:24]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_cksel_flash(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode2_cksel_flash(hw);
}

void sys_hal_set_cpu_clk_div_mode2_cksel_flash(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode2_cksel_flash(hw, value);
}

/* REG_0x09:cpu_clk_div_mode2->ckdiv_flash:0:/1  1:/2  2:/4  3:/8,R/W,0x9[27:26]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_ckdiv_flash(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode2_ckdiv_flash(hw);
}

void sys_hal_set_cpu_clk_div_mode2_ckdiv_flash(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode2_ckdiv_flash(hw, value);
}

/* REG_0x09:cpu_clk_div_mode2->ckdiv_i2s0:0:/1  1:/2  2:/4  3:/8  4:/16  5:/32  6:/64  7:/256,R/W,0x9[30:28]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_ckdiv_i2s0(sys_hw_t *hw)
{
    return sys_ll_get_cpu_clk_div_mode2_ckdiv_i2s0(hw);
}

void sys_hal_set_cpu_clk_div_mode2_ckdiv_i2s0(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_clk_div_mode2_ckdiv_i2s0(hw, value);
}

/* REG_0x0a */

uint32_t sys_hal_get_cpu_26m_wdt_clk_div_value(sys_hw_t *hw)
{
    return sys_ll_get_cpu_26m_wdt_clk_div_value(hw);
}

void sys_hal_set_cpu_26m_wdt_clk_div_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_26m_wdt_clk_div_value(hw, value);
}

/* REG_0x0a:cpu_26m_wdt_clk_div->ckdiv_26m:0:/1  1:/2  2:/4  3:/8,R/W,0xa[1:0]*/
uint32_t sys_hal_get_cpu_26m_wdt_clk_div_ckdiv_26m(sys_hw_t *hw)
{
    return sys_ll_get_cpu_26m_wdt_clk_div_ckdiv_26m(hw);
}

void sys_hal_set_cpu_26m_wdt_clk_div_ckdiv_26m(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_26m_wdt_clk_div_ckdiv_26m(hw, value);
}

/* REG_0x0a:cpu_26m_wdt_clk_div->ckdiv_wdt:0:/2 1:/4 2:/8 3:/16,R/W,0xa[3:2]*/
uint32_t sys_hal_get_cpu_26m_wdt_clk_div_ckdiv_wdt(sys_hw_t *hw)
{
    return sys_ll_get_cpu_26m_wdt_clk_div_ckdiv_wdt(hw);
}

void sys_hal_set_cpu_26m_wdt_clk_div_ckdiv_wdt(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_26m_wdt_clk_div_ckdiv_wdt(hw, value);
}

/* REG_0x0b */

uint32_t sys_hal_get_cpu_anaspi_freq_value(sys_hw_t *hw)
{
    return sys_ll_get_cpu_anaspi_freq_value(hw);
}

void sys_hal_set_cpu_anaspi_freq_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_anaspi_freq_value(hw, value);
}

/* REG_0x0b:cpu_anaspi_freq->anaspi_freq: ,R/W,0xb[5:0]*/
uint32_t sys_hal_get_cpu_anaspi_freq_anaspi_freq(sys_hw_t *hw)
{
    return sys_ll_get_cpu_anaspi_freq_anaspi_freq(hw);
}

void sys_hal_set_cpu_anaspi_freq_anaspi_freq(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_anaspi_freq_anaspi_freq(hw, value);
}

/* REG_0x0c */

uint32_t sys_hal_get_cpu_device_clk_enable_value(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_value(hw);
}

void sys_hal_set_cpu_device_clk_enable_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_value(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->i2c0_cken:1:i2c0_clk enable,R/W,0xc[0]*/
uint32_t sys_hal_get_cpu_device_clk_enable_i2c0_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_i2c0_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_i2c0_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_i2c0_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->spi0_cken:1:spi0_clk enable ,R/W,0xc[1]*/
uint32_t sys_hal_get_cpu_device_clk_enable_spi0_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_spi0_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_spi0_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_spi0_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->uart0_cken:1:uart0_clk enable,R/W,0xc[2]*/
uint32_t sys_hal_get_cpu_device_clk_enable_uart0_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_uart0_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_uart0_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_uart0_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->pwm0_cken:1:pwm0_clk enable ,R/W,0xc[3]*/
uint32_t sys_hal_get_cpu_device_clk_enable_pwm0_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_pwm0_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_pwm0_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_pwm0_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->tim0_cken:1:tim0_clk enable ,R/W,0xc[4]*/
uint32_t sys_hal_get_cpu_device_clk_enable_tim0_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_tim0_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_tim0_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_tim0_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->sadc_cken:1:sadc_clk enable ,R/W,0xc[5]*/
uint32_t sys_hal_get_cpu_device_clk_enable_sadc_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_sadc_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_sadc_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_sadc_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->irda_cken:1:irda_clk enable ,R/W,0xc[6]*/
uint32_t sys_hal_get_cpu_device_clk_enable_irda_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_irda_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_irda_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_irda_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->efuse_cken:1:efuse_clk enable,R/W,0xc[7]*/
uint32_t sys_hal_get_cpu_device_clk_enable_efuse_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_efuse_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_efuse_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_efuse_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->i2c1_cken:1:i2c1_clk enable ,R/W,0xc[8]*/
uint32_t sys_hal_get_cpu_device_clk_enable_i2c1_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_i2c1_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_i2c1_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_i2c1_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->spi1_cken:1:spi1_clk enable ,R/W,0xc[9]*/
uint32_t sys_hal_get_cpu_device_clk_enable_spi1_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_spi1_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_spi1_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_spi1_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->uart1_cken:1:uart1_clk enable,R/W,0xc[10]*/
uint32_t sys_hal_get_cpu_device_clk_enable_uart1_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_uart1_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_uart1_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_uart1_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->uart2_cken:1:uart2_clk enable,R/W,0xc[11]*/
uint32_t sys_hal_get_cpu_device_clk_enable_uart2_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_uart2_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_uart2_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_uart2_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->pwm1_cken:1:pwm1_clk enable ,R/W,0xc[12]*/
uint32_t sys_hal_get_cpu_device_clk_enable_pwm1_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_pwm1_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_pwm1_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_pwm1_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->tim1_cken:1:tim1_clk enable ,R/W,0xc[13]*/
uint32_t sys_hal_get_cpu_device_clk_enable_tim1_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_tim1_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_tim1_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_tim1_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->tim2_cken:1:tim2_clk enable ,R/W,0xc[14]*/
uint32_t sys_hal_get_cpu_device_clk_enable_tim2_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_tim2_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_tim2_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_tim2_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->otp_cken:1:otp_clk enable  ,R/W,0xc[15]*/
uint32_t sys_hal_get_cpu_device_clk_enable_otp_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_otp_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_otp_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_otp_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->i2s_cken:1:i2s_clk enable  ,R/W,0xc[16]*/
uint32_t sys_hal_get_cpu_device_clk_enable_i2s_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_i2s_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_i2s_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_i2s_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->usb_cken:1:usb_clk enable  ,R/W,0xc[17]*/
uint32_t sys_hal_get_cpu_device_clk_enable_usb_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_usb_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_usb_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_usb_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->can_cken:1:can_clk enable  ,R/W,0xc[18]*/
uint32_t sys_hal_get_cpu_device_clk_enable_can_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_can_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_can_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_can_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->psram_cken:1:psram_clk enable,R/W,0xc[19]*/
uint32_t sys_hal_get_cpu_device_clk_enable_psram_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_psram_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_psram_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_psram_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->qspi0_cken:1:qspi0_clk enable,R/W,0xc[20]*/
uint32_t sys_hal_get_cpu_device_clk_enable_qspi0_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_qspi0_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_qspi0_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_qspi0_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->qspi1_cken:1:qspi1_clk enable,R/W,0xc[21]*/
uint32_t sys_hal_get_cpu_device_clk_enable_qspi1_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_qspi1_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_qspi1_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_qspi1_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->sdio_cken:1:sdio_clk enable ,R/W,0xc[22]*/
uint32_t sys_hal_get_cpu_device_clk_enable_sdio_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_sdio_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_sdio_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_sdio_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->auxs_cken:1:auxs_clk enable ,R/W,0xc[23]*/
uint32_t sys_hal_get_cpu_device_clk_enable_auxs_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_auxs_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_auxs_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_auxs_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->btdm_cken:1:btdm_clk enable ,R/W,0xc[24]*/
uint32_t sys_hal_get_cpu_device_clk_enable_btdm_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_btdm_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_btdm_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_btdm_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->xvr_cken:1:xvr_clk enable  ,R/W,0xc[25]*/
uint32_t sys_hal_get_cpu_device_clk_enable_xvr_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_xvr_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_xvr_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_xvr_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->mac_cken:1:mac_clk enable  ,R/W,0xc[26]*/
uint32_t sys_hal_get_cpu_device_clk_enable_mac_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_mac_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_mac_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_mac_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->phy_cken:1:phy_clk enable  ,R/W,0xc[27]*/
uint32_t sys_hal_get_cpu_device_clk_enable_phy_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_phy_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_phy_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_phy_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->jpeg_cken:1:jpeg_clk enable ,R/W,0xc[28]*/
uint32_t sys_hal_get_cpu_device_clk_enable_jpeg_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_jpeg_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_jpeg_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_jpeg_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->disp_cken:1:disp_clk enable ,R/W,0xc[29]*/
uint32_t sys_hal_get_cpu_device_clk_enable_disp_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_disp_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_disp_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_disp_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->aud_cken:1:aud_clk enable  ,R/W,0xc[30]*/
uint32_t sys_hal_get_cpu_device_clk_enable_aud_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_aud_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_aud_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_aud_cken(hw, value);
}

/* REG_0x0c:cpu_device_clk_enable->wdt_cken:1:wdt_clk enable  ,R/W,0xc[31]*/
uint32_t sys_hal_get_cpu_device_clk_enable_wdt_cken(sys_hw_t *hw)
{
    return sys_ll_get_cpu_device_clk_enable_wdt_cken(hw);
}

void sys_hal_set_cpu_device_clk_enable_wdt_cken(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_device_clk_enable_wdt_cken(hw, value);
}

/* REG_0x0d */

/* REG_0x0e */

uint32_t sys_hal_get_cpu_mode_disckg1_value(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_value(hw);
}

void sys_hal_set_cpu_mode_disckg1_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_value(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->aon_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[0]*/
uint32_t sys_hal_get_cpu_mode_disckg1_aon_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_aon_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_aon_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_aon_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->sys_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[1]*/
uint32_t sys_hal_get_cpu_mode_disckg1_sys_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_sys_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_sys_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_sys_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->dma_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[2]*/
uint32_t sys_hal_get_cpu_mode_disckg1_dma_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_dma_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_dma_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_dma_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->flash_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[3]*/
uint32_t sys_hal_get_cpu_mode_disckg1_flash_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_flash_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_flash_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_flash_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->wdt_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[4]*/
uint32_t sys_hal_get_cpu_mode_disckg1_wdt_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_wdt_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_wdt_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_wdt_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->tim_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[5]*/
uint32_t sys_hal_get_cpu_mode_disckg1_tim_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_tim_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_tim_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_tim_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->urt_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[6]*/
uint32_t sys_hal_get_cpu_mode_disckg1_urt_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_urt_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_urt_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_urt_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->pwm_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[7]*/
uint32_t sys_hal_get_cpu_mode_disckg1_pwm_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_pwm_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_pwm_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_pwm_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->i2c_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[8]*/
uint32_t sys_hal_get_cpu_mode_disckg1_i2c_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_i2c_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_i2c_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_i2c_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->spi_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[9]*/
uint32_t sys_hal_get_cpu_mode_disckg1_spi_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_spi_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_spi_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_spi_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->sadc_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[10]*/
uint32_t sys_hal_get_cpu_mode_disckg1_sadc_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_sadc_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_sadc_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_sadc_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->efs_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[11]*/
uint32_t sys_hal_get_cpu_mode_disckg1_efs_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_efs_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_efs_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_efs_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->irda_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[12]*/
uint32_t sys_hal_get_cpu_mode_disckg1_irda_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_irda_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_irda_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_irda_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->trng_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[13]*/
uint32_t sys_hal_get_cpu_mode_disckg1_trng_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_trng_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_trng_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_trng_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->sdio_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[14]*/
uint32_t sys_hal_get_cpu_mode_disckg1_sdio_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_sdio_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_sdio_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_sdio_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->LA_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[15]*/
uint32_t sys_hal_get_cpu_mode_disckg1_la_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_la_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_la_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_la_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->tim1_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[16]*/
uint32_t sys_hal_get_cpu_mode_disckg1_tim1_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_tim1_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_tim1_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_tim1_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->urt1_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[17]*/
uint32_t sys_hal_get_cpu_mode_disckg1_urt1_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_urt1_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_urt1_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_urt1_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->urt2_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[18]*/
uint32_t sys_hal_get_cpu_mode_disckg1_urt2_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_urt2_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_urt2_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_urt2_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->pwm1_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[19]*/
uint32_t sys_hal_get_cpu_mode_disckg1_pwm1_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_pwm1_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_pwm1_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_pwm1_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->i2c1_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[20]*/
uint32_t sys_hal_get_cpu_mode_disckg1_i2c1_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_i2c1_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_i2c1_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_i2c1_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->spi1_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[21]*/
uint32_t sys_hal_get_cpu_mode_disckg1_spi1_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_spi1_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_spi1_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_spi1_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->usb_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[22]*/
uint32_t sys_hal_get_cpu_mode_disckg1_usb_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_usb_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_usb_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_usb_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->can_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[23]*/
uint32_t sys_hal_get_cpu_mode_disckg1_can_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_can_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_can_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_can_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->qspi0_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[24]*/
uint32_t sys_hal_get_cpu_mode_disckg1_qspi0_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_qspi0_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_qspi0_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_qspi0_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->psram_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[25]*/
uint32_t sys_hal_get_cpu_mode_disckg1_psram_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_psram_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_psram_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_psram_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->fft_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[26]*/
uint32_t sys_hal_get_cpu_mode_disckg1_fft_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_fft_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_fft_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_fft_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->sbc_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[27]*/
uint32_t sys_hal_get_cpu_mode_disckg1_sbc_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_sbc_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_sbc_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_sbc_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->aud_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[28]*/
uint32_t sys_hal_get_cpu_mode_disckg1_aud_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_aud_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_aud_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_aud_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->i2s_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[29]*/
uint32_t sys_hal_get_cpu_mode_disckg1_i2s_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_i2s_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_i2s_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_i2s_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->jpeg_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[30]*/
uint32_t sys_hal_get_cpu_mode_disckg1_jpeg_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_jpeg_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_jpeg_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_jpeg_disckg(hw, value);
}

/* REG_0x0e:cpu_mode_disckg1->jpeg_dec_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[31]*/
uint32_t sys_hal_get_cpu_mode_disckg1_jpeg_dec_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg1_jpeg_dec_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg1_jpeg_dec_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg1_jpeg_dec_disckg(hw, value);
}

/* REG_0x0f */

uint32_t sys_hal_get_cpu_mode_disckg2_value(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg2_value(hw);
}

void sys_hal_set_cpu_mode_disckg2_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg2_value(hw, value);
}

/* REG_0x0f:cpu_mode_disckg2->disp_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xf[0]*/
uint32_t sys_hal_get_cpu_mode_disckg2_disp_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg2_disp_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg2_disp_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg2_disp_disckg(hw, value);
}

/* REG_0x0f:cpu_mode_disckg2->dma2d_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xf[1]*/
uint32_t sys_hal_get_cpu_mode_disckg2_dma2d_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg2_dma2d_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg2_dma2d_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg2_dma2d_disckg(hw, value);
}

/* REG_0x0f:cpu_mode_disckg2->btdm_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xf[3]*/
uint32_t sys_hal_get_cpu_mode_disckg2_btdm_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg2_btdm_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg2_btdm_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg2_btdm_disckg(hw, value);
}

/* REG_0x0f:cpu_mode_disckg2->xver_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xf[4]*/
uint32_t sys_hal_get_cpu_mode_disckg2_xver_disckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg2_xver_disckg(hw);
}

void sys_hal_set_cpu_mode_disckg2_xver_disckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg2_xver_disckg(hw, value);
}

/* REG_0x0f:cpu_mode_disckg2->btdm_bps_ckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xf[8:5]*/
uint32_t sys_hal_get_cpu_mode_disckg2_btdm_bps_ckg(sys_hw_t *hw)
{
    return sys_ll_get_cpu_mode_disckg2_btdm_bps_ckg(hw);
}

void sys_hal_set_cpu_mode_disckg2_btdm_bps_ckg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_mode_disckg2_btdm_bps_ckg(hw, value);
}

/* REG_0x10 */

uint32_t sys_hal_get_cpu_power_sleep_wakeup_value(sys_hw_t *hw)
{
    return sys_ll_get_cpu_power_sleep_wakeup_value(hw);
}

void sys_hal_set_cpu_power_sleep_wakeup_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_power_sleep_wakeup_value(hw, value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_mem1:0:power on of mem1      ,RW,0x10[0]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_mem1(sys_hw_t *hw)
{
    return sys_ll_get_cpu_power_sleep_wakeup_pwd_mem1(hw);
}

void sys_hal_set_cpu_power_sleep_wakeup_pwd_mem1(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_power_sleep_wakeup_pwd_mem1(hw, value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_mem2:0:power on of mem2      ,RW,0x10[1]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_mem2(sys_hw_t *hw)
{
    return sys_ll_get_cpu_power_sleep_wakeup_pwd_mem2(hw);
}

void sys_hal_set_cpu_power_sleep_wakeup_pwd_mem2(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_power_sleep_wakeup_pwd_mem2(hw, value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_mem3:0:power on of mem3      ,RW,0x10[2]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_mem3(sys_hw_t *hw)
{
    return sys_ll_get_cpu_power_sleep_wakeup_pwd_mem3(hw);
}

void sys_hal_set_cpu_power_sleep_wakeup_pwd_mem3(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_power_sleep_wakeup_pwd_mem3(hw, value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_encp:0:power on of encp      ,RW,0x10[3]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_encp(sys_hw_t *hw)
{
    return sys_ll_get_cpu_power_sleep_wakeup_pwd_encp(hw);
}

void sys_hal_set_cpu_power_sleep_wakeup_pwd_encp(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_power_sleep_wakeup_pwd_encp(hw, value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_bakp:0:power on of bakp      ,RW,0x10[4]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_bakp(sys_hw_t *hw)
{
    return sys_ll_get_cpu_power_sleep_wakeup_pwd_bakp(hw);
}

void sys_hal_set_cpu_power_sleep_wakeup_pwd_bakp(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_power_sleep_wakeup_pwd_bakp(hw, value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_ahbp:0:power on of ahbp      ,RW,0x10[5]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_ahbp(sys_hw_t *hw)
{
    return sys_ll_get_cpu_power_sleep_wakeup_pwd_ahbp(hw);
}

void sys_hal_set_cpu_power_sleep_wakeup_pwd_ahbp(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_power_sleep_wakeup_pwd_ahbp(hw, value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_audp:0:power on of audp      ,RW,0x10[6]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_audp(sys_hw_t *hw)
{
    return sys_ll_get_cpu_power_sleep_wakeup_pwd_audp(hw);
}

void sys_hal_set_cpu_power_sleep_wakeup_pwd_audp(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_power_sleep_wakeup_pwd_audp(hw, value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_vidp:0:power on of vidp      ,RW,0x10[7]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_vidp(sys_hw_t *hw)
{
    return sys_ll_get_cpu_power_sleep_wakeup_pwd_vidp(hw);
}

void sys_hal_set_cpu_power_sleep_wakeup_pwd_vidp(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_power_sleep_wakeup_pwd_vidp(hw, value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_btsp:0:power on of btsp      ,RW,0x10[8]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_btsp(sys_hw_t *hw)
{
    return sys_ll_get_cpu_power_sleep_wakeup_pwd_btsp(hw);
}

void sys_hal_set_cpu_power_sleep_wakeup_pwd_btsp(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_power_sleep_wakeup_pwd_btsp(hw, value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_wifp_mac:0:power on of wifp_mac  ,RW,0x10[9]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_wifp_mac(sys_hw_t *hw)
{
    return sys_ll_get_cpu_power_sleep_wakeup_pwd_wifp_mac(hw);
}

void sys_hal_set_cpu_power_sleep_wakeup_pwd_wifp_mac(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_mac(hw, value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_wifp_phy:0:power on of wifp_phy  ,RW,0x10[10]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_wifp_phy(sys_hw_t *hw)
{
    return sys_ll_get_cpu_power_sleep_wakeup_pwd_wifp_phy(hw);
}

void sys_hal_set_cpu_power_sleep_wakeup_pwd_wifp_phy(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_phy(hw, value);
}

/* REG_0x10:cpu_power_sleep_wakeup->sleep_en_need_flash_idle:0:sleep_en of flash_idle,RW,0x10[16]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_sleep_en_need_flash_idle(sys_hw_t *hw)
{
    return sys_ll_get_cpu_power_sleep_wakeup_sleep_en_need_flash_idle(hw);
}

void sys_hal_set_cpu_power_sleep_wakeup_sleep_en_need_flash_idle(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_power_sleep_wakeup_sleep_en_need_flash_idle(hw, value);
}

/* REG_0x10:cpu_power_sleep_wakeup->sleep_en_need_cpu1_wfi:0:sleep_en of cpu1_wfi  ,RW,0x10[17]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_sleep_en_need_cpu1_wfi(sys_hw_t *hw)
{
    return sys_ll_get_cpu_power_sleep_wakeup_sleep_en_need_cpu1_wfi(hw);
}

void sys_hal_set_cpu_power_sleep_wakeup_sleep_en_need_cpu1_wfi(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_power_sleep_wakeup_sleep_en_need_cpu1_wfi(hw, value);
}

/* REG_0x10:cpu_power_sleep_wakeup->sleep_en_need_cpu0_wfi:0:sleep_en of cpu0_wfi  ,RW,0x10[18]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_sleep_en_need_cpu0_wfi(sys_hw_t *hw)
{
    return sys_ll_get_cpu_power_sleep_wakeup_sleep_en_need_cpu0_wfi(hw);
}

void sys_hal_set_cpu_power_sleep_wakeup_sleep_en_need_cpu0_wfi(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_power_sleep_wakeup_sleep_en_need_cpu0_wfi(hw, value);
}

/* REG_0x10:cpu_power_sleep_wakeup->sleep_en_global:0:sleep_en of global    ,RW,0x10[19]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_sleep_en_global(sys_hw_t *hw)
{
    return sys_ll_get_cpu_power_sleep_wakeup_sleep_en_global(hw);
}

void sys_hal_set_cpu_power_sleep_wakeup_sleep_en_global(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_power_sleep_wakeup_sleep_en_global(hw, value);
}

/* REG_0x10:cpu_power_sleep_wakeup->wifi_wakeup_platform_en:0:wifi_wakeup_en        ,RW,0x10[20]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_wifi_wakeup_platform_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu_power_sleep_wakeup_wifi_wakeup_platform_en(hw);
}

void sys_hal_set_cpu_power_sleep_wakeup_wifi_wakeup_platform_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_power_sleep_wakeup_wifi_wakeup_platform_en(hw, value);
}

/* REG_0x10:cpu_power_sleep_wakeup->bts_wakeup_platform_en:0:bts_wakeup_en         ,RW,0x10[21]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_bts_wakeup_platform_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu_power_sleep_wakeup_bts_wakeup_platform_en(hw);
}

void sys_hal_set_cpu_power_sleep_wakeup_bts_wakeup_platform_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_power_sleep_wakeup_bts_wakeup_platform_en(hw, value);
}

/* REG_0x10:cpu_power_sleep_wakeup->bts_sleep_exit_req:0:bt sleep exit request ,RW,0x10[22]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_bts_sleep_exit_req(sys_hw_t *hw)
{
    return sys_ll_get_cpu_power_sleep_wakeup_bts_sleep_exit_req(hw);
}

void sys_hal_set_cpu_power_sleep_wakeup_bts_sleep_exit_req(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu_power_sleep_wakeup_bts_sleep_exit_req(hw, value);
}

/* REG_0x11 */

/* REG_0x20 */

uint32_t sys_hal_get_cpu0_int_0_31_en_value(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_value(hw);
}

void sys_hal_set_cpu0_int_0_31_en_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_value(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_bmc32_int_en: ,R/W,0x20[0]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_bmc32_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_bmc32_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_bmc32_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_bmc32_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_host_0_irq_en: ,R/W,0x20[1]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_host_0_irq_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_host_0_irq_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_host_0_irq_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_host_0_irq_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_host_0_sec_irq_en: ,R/W,0x20[2]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_host_0_sec_irq_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_host_0_sec_irq_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_host_0_sec_irq_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_host_0_sec_irq_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_timer_int_en: ,R/W,0x20[3]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_timer_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_timer_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_timer_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_timer_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_uart_int_en: ,R/W,0x20[4]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_uart_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_uart_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_uart_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_uart_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_pwm_int_en: ,R/W,0x20[5]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_pwm_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_pwm_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_pwm_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_pwm_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_i2c_int_en: ,R/W,0x20[6]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_i2c_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_i2c_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_i2c_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_i2c_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_spi_int_en: ,R/W,0x20[7]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_spi_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_spi_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_spi_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_spi_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_sadc_int_en: ,R/W,0x20[8]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_sadc_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_sadc_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_sadc_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_sadc_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_irda_int_en: ,R/W,0x20[9]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_irda_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_irda_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_irda_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_irda_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_sdio_int_en: ,R/W,0x20[10]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_sdio_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_sdio_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_sdio_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_sdio_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_gdma_int_en: ,R/W,0x20[11]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_gdma_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_gdma_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_gdma_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_gdma_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_la_int_en: ,R/W,0x20[12]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_la_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_la_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_la_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_la_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_timer1_int_en: ,R/W,0x20[13]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_timer1_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_timer1_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_timer1_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_timer1_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_i2c1_int_en: ,R/W,0x20[14]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_i2c1_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_i2c1_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_i2c1_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_i2c1_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_uart1_int_en: ,R/W,0x20[15]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_uart1_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_uart1_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_uart1_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_uart1_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_uart2_int_en: ,R/W,0x20[16]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_uart2_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_uart2_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_uart2_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_uart2_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_spi1_int_en: ,R/W,0x20[17]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_spi1_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_spi1_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_spi1_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_spi1_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_can_int_en: ,R/W,0x20[18]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_can_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_can_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_can_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_can_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_usb_int_en: ,R/W,0x20[19]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_usb_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_usb_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_usb_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_usb_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_qspi_int_en: ,R/W,0x20[20]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_qspi_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_qspi_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_qspi_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_qspi_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_fft_int_en: ,R/W,0x20[21]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_fft_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_fft_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_fft_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_fft_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_sbc_int_en: ,R/W,0x20[22]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_sbc_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_sbc_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_sbc_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_sbc_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_aud_int_en: ,R/W,0x20[23]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_aud_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_aud_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_aud_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_aud_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_i2s_int_en: ,R/W,0x20[24]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_i2s_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_i2s_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_i2s_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_i2s_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_jpegenc_int_en: ,R/W,0x20[25]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_jpegenc_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_jpegenc_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_jpegenc_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_jpegenc_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_jpegdec_int_en: ,R/W,0x20[26]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_jpegdec_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_jpegdec_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_jpegdec_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_jpegdec_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_lcd_int_en: ,R/W,0x20[27]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_lcd_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_lcd_int_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_lcd_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_lcd_int_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_wifi_int_phy_en: ,R/W,0x20[29]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_wifi_int_phy_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_wifi_int_phy_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_wifi_int_phy_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_int_phy_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_wifi_mac_int_tx_rx_timer_en: ,R/W,0x20[30]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_timer_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_timer_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_timer_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_timer_en(hw, value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_wifi_mac_int_tx_rx_misc_en: ,R/W,0x20[31]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_misc_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_misc_en(hw);
}

void sys_hal_set_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_misc_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_misc_en(hw, value);
}

/* REG_0x21 */

uint32_t sys_hal_get_cpu0_int_32_63_en_value(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_en_value(hw);
}

void sys_hal_set_cpu0_int_32_63_en_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_32_63_en_value(hw, value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_mac_int_rx_trigger_en: ,R/W,0x21[0]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_rx_trigger_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_rx_trigger_en(hw);
}

void sys_hal_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_rx_trigger_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_rx_trigger_en(hw, value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_mac_int_tx_trigger_en: ,R/W,0x21[1]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_trigger_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_trigger_en(hw);
}

void sys_hal_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_trigger_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_trigger_en(hw, value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_mac_port_trigger_en: ,R/W,0x21[2]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_wifi_mac_port_trigger_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_port_trigger_en(hw);
}

void sys_hal_set_cpu0_int_32_63_en_cpu0_wifi_mac_port_trigger_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_port_trigger_en(hw, value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_mac_int_gen_en: ,R/W,0x21[3]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_gen_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_gen_en(hw);
}

void sys_hal_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_gen_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_gen_en(hw, value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_hsu_irq_en: ,R/W,0x21[4]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_wifi_hsu_irq_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_hsu_irq_en(hw);
}

void sys_hal_set_cpu0_int_32_63_en_cpu0_wifi_hsu_irq_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_hsu_irq_en(hw, value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_int_mac_wakeup_en: ,R/W,0x21[5]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_wifi_int_mac_wakeup_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_int_mac_wakeup_en(hw);
}

void sys_hal_set_cpu0_int_32_63_en_cpu0_wifi_int_mac_wakeup_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_int_mac_wakeup_en(hw, value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_dm_irq_en: ,R/W,0x21[7]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_dm_irq_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_en_cpu0_dm_irq_en(hw);
}

void sys_hal_set_cpu0_int_32_63_en_cpu0_dm_irq_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_32_63_en_cpu0_dm_irq_en(hw, value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_ble_irq_en: ,R/W,0x21[8]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_ble_irq_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_en_cpu0_ble_irq_en(hw);
}

void sys_hal_set_cpu0_int_32_63_en_cpu0_ble_irq_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_32_63_en_cpu0_ble_irq_en(hw, value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_bt_irq_en: ,R/W,0x21[9]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_bt_irq_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_en_cpu0_bt_irq_en(hw);
}

void sys_hal_set_cpu0_int_32_63_en_cpu0_bt_irq_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_32_63_en_cpu0_bt_irq_en(hw, value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_mbox0_int_en: ,R/W,0x21[15]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_mbox0_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_en_cpu0_mbox0_int_en(hw);
}

void sys_hal_set_cpu0_int_32_63_en_cpu0_mbox0_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_32_63_en_cpu0_mbox0_int_en(hw, value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_mbox1_int_en: ,R/W,0x21[16]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_mbox1_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_en_cpu0_mbox1_int_en(hw);
}

void sys_hal_set_cpu0_int_32_63_en_cpu0_mbox1_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_32_63_en_cpu0_mbox1_int_en(hw, value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_bmc64_int_en: ,R/W,0x21[17]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_bmc64_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_en_cpu0_bmc64_int_en(hw);
}

void sys_hal_set_cpu0_int_32_63_en_cpu0_bmc64_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_32_63_en_cpu0_bmc64_int_en(hw, value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_touched_int_en: ,R/W,0x21[19]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_touched_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_en_cpu0_touched_int_en(hw);
}

void sys_hal_set_cpu0_int_32_63_en_cpu0_touched_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_32_63_en_cpu0_touched_int_en(hw, value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_usbplug_int_en: ,R/W,0x21[20]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_usbplug_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_en_cpu0_usbplug_int_en(hw);
}

void sys_hal_set_cpu0_int_32_63_en_cpu0_usbplug_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_32_63_en_cpu0_usbplug_int_en(hw, value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_rtc_int_en: ,R/W,0x21[21]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_rtc_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_en_cpu0_rtc_int_en(hw);
}

void sys_hal_set_cpu0_int_32_63_en_cpu0_rtc_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_32_63_en_cpu0_rtc_int_en(hw, value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_gpio_int_en: ,R/W,0x21[22]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_gpio_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_en_cpu0_gpio_int_en(hw);
}

void sys_hal_set_cpu0_int_32_63_en_cpu0_gpio_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu0_int_32_63_en_cpu0_gpio_int_en(hw, value);
}

/* REG_0x22 */

uint32_t sys_hal_get_cpu1_int_0_31_en_value(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_value(hw);
}

void sys_hal_set_cpu1_int_0_31_en_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_value(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_bmc32_int_en: ,R/W,0x22[0]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_bmc32_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_bmc32_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_bmc32_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_bmc32_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_host_0_irq_en: ,R/W,0x22[1]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_host_0_irq_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_host_0_irq_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_host_0_irq_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_host_0_irq_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_host_0_sec_irq_en: ,R/W,0x22[2]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_host_0_sec_irq_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_host_0_sec_irq_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_host_0_sec_irq_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_host_0_sec_irq_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_timer_int_en: ,R/W,0x22[3]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_timer_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_timer_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_timer_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_timer_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_uart_int_en: ,R/W,0x22[4]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_uart_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_uart_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_uart_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_uart_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_pwm_int_en: ,R/W,0x22[5]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_pwm_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_pwm_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_pwm_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_pwm_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_i2c_int_en: ,R/W,0x22[6]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_i2c_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_i2c_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_i2c_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_i2c_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_spi_int_en: ,R/W,0x22[7]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_spi_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_spi_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_spi_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_spi_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_sadc_int_en: ,R/W,0x22[8]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_sadc_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_sadc_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_sadc_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_sadc_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_irda_int_en: ,R/W,0x22[9]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_irda_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_irda_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_irda_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_irda_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_sdio_int_en: ,R/W,0x22[10]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_sdio_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_sdio_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_sdio_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_sdio_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_gdma_int_en: ,R/W,0x22[11]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_gdma_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_gdma_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_gdma_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_gdma_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_la_int_en: ,R/W,0x22[12]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_la_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_la_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_la_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_la_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_timer1_int_en: ,R/W,0x22[13]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_timer1_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_timer1_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_timer1_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_timer1_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_i2c1_int_en: ,R/W,0x22[14]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_i2c1_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_i2c1_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_i2c1_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_i2c1_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_uart1_int_en: ,R/W,0x22[15]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_uart1_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_uart1_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_uart1_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_uart1_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_uart2_int_en: ,R/W,0x22[16]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_uart2_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_uart2_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_uart2_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_uart2_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_spi1_int_en: ,R/W,0x22[17]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_spi1_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_spi1_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_spi1_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_spi1_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_can_int_en: ,R/W,0x22[18]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_can_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_can_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_can_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_can_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_usb_int_en: ,R/W,0x22[19]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_usb_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_usb_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_usb_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_usb_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_qspi_int_en: ,R/W,0x22[20]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_qspi_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_qspi_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_qspi_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_qspi_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_fft_int_en: ,R/W,0x22[21]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_fft_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_fft_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_fft_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_fft_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_sbc_int_en: ,R/W,0x22[22]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_sbc_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_sbc_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_sbc_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_sbc_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_aud_int_en: ,R/W,0x22[23]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_aud_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_aud_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_aud_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_aud_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_i2s_int_en: ,R/W,0x22[24]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_i2s_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_i2s_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_i2s_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_i2s_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_jpegenc_int_en: ,R/W,0x22[25]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_jpegenc_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_jpegenc_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_jpegenc_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_jpegenc_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_jpegdec_int_en: ,R/W,0x22[26]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_jpegdec_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_jpegdec_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_jpegdec_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_jpegdec_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_lcd_int_en: ,R/W,0x22[27]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_lcd_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_lcd_int_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_lcd_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_lcd_int_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_wifi_int_phy_en: ,R/W,0x22[29]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_wifi_int_phy_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_wifi_int_phy_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_wifi_int_phy_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_wifi_int_phy_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_wifi_mac_int_tx_rx_timer_en: ,R/W,0x22[30]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_timer_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_timer_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_timer_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_timer_en(hw, value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_wifi_mac_int_tx_rx_misc_en: ,R/W,0x22[31]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_misc_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_misc_en(hw);
}

void sys_hal_set_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_misc_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_misc_en(hw, value);
}

/* REG_0x23 */

uint32_t sys_hal_get_cpu1_int_32_63_en_value(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_en_value(hw);
}

void sys_hal_set_cpu1_int_32_63_en_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_32_63_en_value(hw, value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_mac_int_rx_trigger_en: ,R/W,0x23[0]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_rx_trigger_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_rx_trigger_en(hw);
}

void sys_hal_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_rx_trigger_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_rx_trigger_en(hw, value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_mac_int_tx_trigger_en: ,R/W,0x23[1]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_trigger_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_trigger_en(hw);
}

void sys_hal_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_trigger_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_trigger_en(hw, value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_mac_port_trigger_en: ,R/W,0x23[2]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_wifi_mac_port_trigger_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_port_trigger_en(hw);
}

void sys_hal_set_cpu1_int_32_63_en_cpu1_wifi_mac_port_trigger_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_port_trigger_en(hw, value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_mac_int_gen_en: ,R/W,0x23[3]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_gen_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_gen_en(hw);
}

void sys_hal_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_gen_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_gen_en(hw, value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_hsu_irq_en: ,R/W,0x23[4]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_wifi_hsu_irq_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_hsu_irq_en(hw);
}

void sys_hal_set_cpu1_int_32_63_en_cpu1_wifi_hsu_irq_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_hsu_irq_en(hw, value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_int_mac_wakeup_en: ,R/W,0x23[5]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_wifi_int_mac_wakeup_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_int_mac_wakeup_en(hw);
}

void sys_hal_set_cpu1_int_32_63_en_cpu1_wifi_int_mac_wakeup_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_int_mac_wakeup_en(hw, value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_dm_irq_en: ,R/W,0x23[7]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_dm_irq_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_en_cpu1_dm_irq_en(hw);
}

void sys_hal_set_cpu1_int_32_63_en_cpu1_dm_irq_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_32_63_en_cpu1_dm_irq_en(hw, value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_ble_irq_en: ,R/W,0x23[8]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_ble_irq_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_en_cpu1_ble_irq_en(hw);
}

void sys_hal_set_cpu1_int_32_63_en_cpu1_ble_irq_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_32_63_en_cpu1_ble_irq_en(hw, value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_bt_irq_en: ,R/W,0x23[9]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_bt_irq_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_en_cpu1_bt_irq_en(hw);
}

void sys_hal_set_cpu1_int_32_63_en_cpu1_bt_irq_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_32_63_en_cpu1_bt_irq_en(hw, value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_mbox0_int_en: ,R/W,0x23[15]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_mbox0_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_en_cpu1_mbox0_int_en(hw);
}

void sys_hal_set_cpu1_int_32_63_en_cpu1_mbox0_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_32_63_en_cpu1_mbox0_int_en(hw, value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_mbox1_int_en: ,R/W,0x23[16]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_mbox1_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_en_cpu1_mbox1_int_en(hw);
}

void sys_hal_set_cpu1_int_32_63_en_cpu1_mbox1_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_32_63_en_cpu1_mbox1_int_en(hw, value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_bmc64_int_en: ,R/W,0x23[17]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_bmc64_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_en_cpu1_bmc64_int_en(hw);
}

void sys_hal_set_cpu1_int_32_63_en_cpu1_bmc64_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_32_63_en_cpu1_bmc64_int_en(hw, value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_touched_int_en: ,R/W,0x23[19]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_touched_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_en_cpu1_touched_int_en(hw);
}

void sys_hal_set_cpu1_int_32_63_en_cpu1_touched_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_32_63_en_cpu1_touched_int_en(hw, value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_usbplug_int_en: ,R/W,0x23[20]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_usbplug_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_en_cpu1_usbplug_int_en(hw);
}

void sys_hal_set_cpu1_int_32_63_en_cpu1_usbplug_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_32_63_en_cpu1_usbplug_int_en(hw, value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_rtc_int_en: ,R/W,0x23[21]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_rtc_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_en_cpu1_rtc_int_en(hw);
}

void sys_hal_set_cpu1_int_32_63_en_cpu1_rtc_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_32_63_en_cpu1_rtc_int_en(hw, value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_gpio_int_en: ,R/W,0x23[22]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_gpio_int_en(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_en_cpu1_gpio_int_en(hw);
}

void sys_hal_set_cpu1_int_32_63_en_cpu1_gpio_int_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_cpu1_int_32_63_en_cpu1_gpio_int_en(hw, value);
}

/* REG_0x28 */

uint32_t sys_hal_get_cpu0_int_0_31_status_value(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_value(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_bmc32_int_st: ,R,0x28[0]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_bmc32_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_bmc32_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_host_0_irq_st: ,R,0x28[1]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_host_0_irq_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_host_0_irq_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_host_0_sec_irq_st: ,R,0x28[2]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_host_0_sec_irq_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_host_0_sec_irq_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_timer_int_st: ,R,0x28[3]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_timer_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_timer_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_uart_int_st: ,R,0x28[4]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_uart_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_uart_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_pwm_int_st: ,R,0x28[5]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_pwm_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_pwm_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_i2c_int_st: ,R,0x28[6]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_i2c_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_i2c_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_spi_int_st: ,R,0x28[7]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_spi_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_spi_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_sadc_int_st: ,R,0x28[8]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_sadc_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_sadc_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_irda_int_st: ,R,0x28[9]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_irda_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_irda_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_sdio_int_st: ,R,0x28[10]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_sdio_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_sdio_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_gdma_int_st: ,R,0x28[11]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_gdma_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_gdma_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_la_int_st: ,R,0x28[12]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_la_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_la_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_timer1_int_st: ,R,0x28[13]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_timer1_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_timer1_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_i2c1_int_st: ,R,0x28[14]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_i2c1_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_i2c1_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_uart1_int_st: ,R,0x28[15]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_uart1_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_uart1_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_uart2_int_st: ,R,0x28[16]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_uart2_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_uart2_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_spi1_int_st: ,R,0x28[17]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_spi1_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_spi1_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_can_int_st: ,R,0x28[18]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_can_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_can_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_usb_int_st: ,R,0x28[19]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_usb_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_usb_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_qspi_int_st: ,R,0x28[20]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_qspi_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_qspi_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_fft_int_st: ,R,0x28[21]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_fft_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_fft_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_sbc_int_st: ,R,0x28[22]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_sbc_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_sbc_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_aud_int_st: ,R,0x28[23]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_aud_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_aud_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_i2s_int_st: ,R,0x28[24]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_i2s_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_i2s_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_jpegenc_int_st: ,R,0x28[25]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_jpegenc_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_jpegenc_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_jpegdec_int_st: ,R,0x28[26]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_jpegdec_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_jpegdec_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_lcd_int_st: ,R,0x28[27]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_lcd_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_lcd_int_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_wifi_int_phy_st: ,R,0x28[29]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_wifi_int_phy_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_wifi_int_phy_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_wifi_mac_int_tx_rx_timer_st: ,R,0x28[30]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_wifi_mac_int_tx_rx_timer_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_wifi_mac_int_tx_rx_timer_st(hw);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_wifi_mac_int_tx_rx_misc_st: ,R,0x28[31]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_wifi_mac_int_tx_rx_misc_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_0_31_status_cpu0_wifi_mac_int_tx_rx_misc_st(hw);
}

/* REG_0x29 */

uint32_t sys_hal_get_cpu0_int_32_63_status_value(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_status_value(hw);
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_mac_int_rx_trigger_st: ,R,0x29[0]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_rx_trigger_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_rx_trigger_st(hw);
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_mac_int_tx_trigger_st: ,R,0x29[1]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_tx_trigger_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_tx_trigger_st(hw);
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_mac_port_trigger_st: ,R,0x29[2]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_wifi_mac_port_trigger_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_port_trigger_st(hw);
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_mac_int_gen_st: ,R,0x29[3]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_gen_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_gen_st(hw);
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_hsu_irq_st: ,R,0x29[4]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_wifi_hsu_irq_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_hsu_irq_st(hw);
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_int_mac_wakeup_st: ,R,0x29[5]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_wifi_int_mac_wakeup_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_int_mac_wakeup_st(hw);
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_dm_irq_st: ,R,0x29[7]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_dm_irq_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_status_cpu0_dm_irq_st(hw);
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_ble_irq_st: ,R,0x29[8]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_ble_irq_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_status_cpu0_ble_irq_st(hw);
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_bt_irq_st: ,R,0x29[9]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_bt_irq_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_status_cpu0_bt_irq_st(hw);
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_mbox0_int_st: ,R,0x29[15]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_mbox0_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_status_cpu0_mbox0_int_st(hw);
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_mbox1_int_st: ,R,0x29[16]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_mbox1_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_status_cpu0_mbox1_int_st(hw);
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_bmc64_int_st: ,R,0x29[17]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_bmc64_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_status_cpu0_bmc64_int_st(hw);
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_touched_int_st: ,R,0x29[19]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_touched_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_status_cpu0_touched_int_st(hw);
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_usbplug_int_st: ,R,0x29[20]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_usbplug_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_status_cpu0_usbplug_int_st(hw);
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_rtc_int_st: ,R,0x29[21]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_rtc_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_status_cpu0_rtc_int_st(hw);
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_gpio_int_st: ,R,0x29[22]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_gpio_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu0_int_32_63_status_cpu0_gpio_int_st(hw);
}

/* REG_0x2a */

uint32_t sys_hal_get_cpu1_int_0_31_status_value(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_value(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_bmc32_int_st: ,R,0x2a[0]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_bmc32_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_bmc32_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_host_0_irq_st: ,R,0x2a[1]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_host_0_irq_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_host_0_irq_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_host_0_sec_irq_st: ,R,0x2a[2]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_host_0_sec_irq_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_host_0_sec_irq_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_timer_int_st: ,R,0x2a[3]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_timer_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_timer_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_uart_int_st: ,R,0x2a[4]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_uart_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_uart_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_pwm_int_st: ,R,0x2a[5]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_pwm_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_pwm_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_i2c_int_st: ,R,0x2a[6]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_i2c_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_i2c_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_spi_int_st: ,R,0x2a[7]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_spi_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_spi_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_sadc_int_st: ,R,0x2a[8]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_sadc_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_sadc_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_irda_int_st: ,R,0x2a[9]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_irda_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_irda_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_sdio_int_st: ,R,0x2a[10]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_sdio_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_sdio_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_gdma_int_st: ,R,0x2a[11]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_gdma_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_gdma_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_la_int_st: ,R,0x2a[12]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_la_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_la_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_timer1_int_st: ,R,0x2a[13]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_timer1_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_timer1_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_i2c1_int_st: ,R,0x2a[14]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_i2c1_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_i2c1_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_uart1_int_st: ,R,0x2a[15]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_uart1_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_uart1_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_uart2_int_st: ,R,0x2a[16]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_uart2_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_uart2_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_spi1_int_st: ,R,0x2a[17]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_spi1_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_spi1_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_can_int_st: ,R,0x2a[18]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_can_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_can_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_usb_int_st: ,R,0x2a[19]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_usb_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_usb_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_qspi_int_st: ,R,0x2a[20]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_qspi_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_qspi_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_fft_int_st: ,R,0x2a[21]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_fft_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_fft_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_sbc_int_st: ,R,0x2a[22]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_sbc_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_sbc_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_aud_int_st: ,R,0x2a[23]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_aud_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_aud_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_i2s_int_st: ,R,0x2a[24]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_i2s_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_i2s_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_jpegenc_int_st: ,R,0x2a[25]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_jpegenc_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_jpegenc_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_jpegdec_int_st: ,R,0x2a[26]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_jpegdec_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_jpegdec_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_lcd_int_st: ,R,0x2a[27]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_lcd_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_lcd_int_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_wifi_int_phy_st: ,R,0x2a[29]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_wifi_int_phy_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_wifi_int_phy_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_wifi_mac_int_tx_rx_timer_st: ,R,0x2a[30]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_wifi_mac_int_tx_rx_timer_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_wifi_mac_int_tx_rx_timer_st(hw);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_wifi_mac_int_tx_rx_misc_st: ,R,0x2a[31]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_wifi_mac_int_tx_rx_misc_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_0_31_status_cpu1_wifi_mac_int_tx_rx_misc_st(hw);
}

/* REG_0x2b */

uint32_t sys_hal_get_cpu1_int_32_63_status_value(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_status_value(hw);
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_mac_int_rx_trigger_st: ,R,0x2b[0]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_rx_trigger_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_rx_trigger_st(hw);
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_mac_int_tx_trigger_st: ,R,0x2b[1]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_tx_trigger_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_tx_trigger_st(hw);
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_mac_port_trigger_st: ,R,0x2b[2]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_wifi_mac_port_trigger_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_port_trigger_st(hw);
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_mac_int_gen_st: ,R,0x2b[3]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_gen_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_gen_st(hw);
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_hsu_irq_st: ,R,0x2b[4]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_wifi_hsu_irq_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_hsu_irq_st(hw);
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_int_mac_wakeup_st: ,R,0x2b[5]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_wifi_int_mac_wakeup_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_int_mac_wakeup_st(hw);
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_dm_irq_st: ,R,0x2b[7]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_dm_irq_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_status_cpu1_dm_irq_st(hw);
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_ble_irq_st: ,R,0x2b[8]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_ble_irq_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_status_cpu1_ble_irq_st(hw);
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_bt_irq_st: ,R,0x2b[9]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_bt_irq_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_status_cpu1_bt_irq_st(hw);
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_mbox0_int_st: ,R,0x2b[15]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_mbox0_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_status_cpu1_mbox0_int_st(hw);
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_mbox1_int_st: ,R,0x2b[16]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_mbox1_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_status_cpu1_mbox1_int_st(hw);
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_bmc64_int_st: ,R,0x2b[17]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_bmc64_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_status_cpu1_bmc64_int_st(hw);
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_touched_int_st: ,R,0x2b[19]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_touched_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_status_cpu1_touched_int_st(hw);
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_usbplug_int_st: ,R,0x2b[20]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_usbplug_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_status_cpu1_usbplug_int_st(hw);
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_rtc_int_st: ,R,0x2b[21]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_rtc_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_status_cpu1_rtc_int_st(hw);
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_gpio_int_st: ,R,0x2b[22]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_gpio_int_st(sys_hw_t *hw)
{
    return sys_ll_get_cpu1_int_32_63_status_cpu1_gpio_int_st(hw);
}

/* REG_0x30 */

uint32_t sys_hal_get_gpio_config0_value(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config0_value(hw);
}

void sys_hal_set_gpio_config0_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config0_value(hw, value);
}

/* REG_0x30:gpio_config0->sys_gpio0:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x30[3:0]*/
uint32_t sys_hal_get_gpio_config0_sys_gpio0(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config0_sys_gpio0(hw);
}

void sys_hal_set_gpio_config0_sys_gpio0(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config0_sys_gpio0(hw, value);
}

/* REG_0x30:gpio_config0->sys_gpio1:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x30[7:4]*/
uint32_t sys_hal_get_gpio_config0_sys_gpio1(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config0_sys_gpio1(hw);
}

void sys_hal_set_gpio_config0_sys_gpio1(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config0_sys_gpio1(hw, value);
}

/* REG_0x30:gpio_config0->sys_gpio2:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x30[11:8]*/
uint32_t sys_hal_get_gpio_config0_sys_gpio2(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config0_sys_gpio2(hw);
}

void sys_hal_set_gpio_config0_sys_gpio2(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config0_sys_gpio2(hw, value);
}

/* REG_0x30:gpio_config0->sys_gpio3:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x30[15:12]*/
uint32_t sys_hal_get_gpio_config0_sys_gpio3(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config0_sys_gpio3(hw);
}

void sys_hal_set_gpio_config0_sys_gpio3(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config0_sys_gpio3(hw, value);
}

/* REG_0x30:gpio_config0->sys_gpio4:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x30[19:16]*/
uint32_t sys_hal_get_gpio_config0_sys_gpio4(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config0_sys_gpio4(hw);
}

void sys_hal_set_gpio_config0_sys_gpio4(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config0_sys_gpio4(hw, value);
}

/* REG_0x30:gpio_config0->sys_gpio5:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x30[23:20]*/
uint32_t sys_hal_get_gpio_config0_sys_gpio5(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config0_sys_gpio5(hw);
}

void sys_hal_set_gpio_config0_sys_gpio5(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config0_sys_gpio5(hw, value);
}

/* REG_0x30:gpio_config0->sys_gpio6:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x30[27:24]*/
uint32_t sys_hal_get_gpio_config0_sys_gpio6(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config0_sys_gpio6(hw);
}

void sys_hal_set_gpio_config0_sys_gpio6(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config0_sys_gpio6(hw, value);
}

/* REG_0x30:gpio_config0->sys_gpio7:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x30[31:28]*/
uint32_t sys_hal_get_gpio_config0_sys_gpio7(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config0_sys_gpio7(hw);
}

void sys_hal_set_gpio_config0_sys_gpio7(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config0_sys_gpio7(hw, value);
}

/* REG_0x31 */

uint32_t sys_hal_get_gpio_config1_value(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config1_value(hw);
}

void sys_hal_set_gpio_config1_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config1_value(hw, value);
}

/* REG_0x31:gpio_config1->sys_gpio8:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x31[3:0]*/
uint32_t sys_hal_get_gpio_config1_sys_gpio8(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config1_sys_gpio8(hw);
}

void sys_hal_set_gpio_config1_sys_gpio8(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config1_sys_gpio8(hw, value);
}

/* REG_0x31:gpio_config1->sys_gpio9:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x31[7:4]*/
uint32_t sys_hal_get_gpio_config1_sys_gpio9(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config1_sys_gpio9(hw);
}

void sys_hal_set_gpio_config1_sys_gpio9(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config1_sys_gpio9(hw, value);
}

/* REG_0x31:gpio_config1->sys_gpio10:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x31[11:8]*/
uint32_t sys_hal_get_gpio_config1_sys_gpio10(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config1_sys_gpio10(hw);
}

void sys_hal_set_gpio_config1_sys_gpio10(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config1_sys_gpio10(hw, value);
}

/* REG_0x31:gpio_config1->sys_gpio11:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x31[15:12]*/
uint32_t sys_hal_get_gpio_config1_sys_gpio11(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config1_sys_gpio11(hw);
}

void sys_hal_set_gpio_config1_sys_gpio11(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config1_sys_gpio11(hw, value);
}

/* REG_0x31:gpio_config1->sys_gpio12:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x31[19:16]*/
uint32_t sys_hal_get_gpio_config1_sys_gpio12(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config1_sys_gpio12(hw);
}

void sys_hal_set_gpio_config1_sys_gpio12(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config1_sys_gpio12(hw, value);
}

/* REG_0x31:gpio_config1->sys_gpio13:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x31[23:20]*/
uint32_t sys_hal_get_gpio_config1_sys_gpio13(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config1_sys_gpio13(hw);
}

void sys_hal_set_gpio_config1_sys_gpio13(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config1_sys_gpio13(hw, value);
}

/* REG_0x31:gpio_config1->sys_gpio14:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x31[27:24]*/
uint32_t sys_hal_get_gpio_config1_sys_gpio14(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config1_sys_gpio14(hw);
}

void sys_hal_set_gpio_config1_sys_gpio14(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config1_sys_gpio14(hw, value);
}

/* REG_0x31:gpio_config1->sys_gpio15:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x31[31:28]*/
uint32_t sys_hal_get_gpio_config1_sys_gpio15(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config1_sys_gpio15(hw);
}

void sys_hal_set_gpio_config1_sys_gpio15(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config1_sys_gpio15(hw, value);
}

/* REG_0x32 */

uint32_t sys_hal_get_gpio_config2_value(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config2_value(hw);
}

void sys_hal_set_gpio_config2_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config2_value(hw, value);
}

/* REG_0x32:gpio_config2->sys_gpio16:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x32[3:0]*/
uint32_t sys_hal_get_gpio_config2_sys_gpio16(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config2_sys_gpio16(hw);
}

void sys_hal_set_gpio_config2_sys_gpio16(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config2_sys_gpio16(hw, value);
}

/* REG_0x32:gpio_config2->sys_gpio17:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x32[7:4]*/
uint32_t sys_hal_get_gpio_config2_sys_gpio17(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config2_sys_gpio17(hw);
}

void sys_hal_set_gpio_config2_sys_gpio17(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config2_sys_gpio17(hw, value);
}

/* REG_0x32:gpio_config2->sys_gpio18:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x32[11:8]*/
uint32_t sys_hal_get_gpio_config2_sys_gpio18(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config2_sys_gpio18(hw);
}

void sys_hal_set_gpio_config2_sys_gpio18(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config2_sys_gpio18(hw, value);
}

/* REG_0x32:gpio_config2->sys_gpio19:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x32[15:12]*/
uint32_t sys_hal_get_gpio_config2_sys_gpio19(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config2_sys_gpio19(hw);
}

void sys_hal_set_gpio_config2_sys_gpio19(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config2_sys_gpio19(hw, value);
}

/* REG_0x32:gpio_config2->sys_gpio20:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x32[19:16]*/
uint32_t sys_hal_get_gpio_config2_sys_gpio20(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config2_sys_gpio20(hw);
}

void sys_hal_set_gpio_config2_sys_gpio20(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config2_sys_gpio20(hw, value);
}

/* REG_0x32:gpio_config2->sys_gpio21:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x32[23:20]*/
uint32_t sys_hal_get_gpio_config2_sys_gpio21(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config2_sys_gpio21(hw);
}

void sys_hal_set_gpio_config2_sys_gpio21(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config2_sys_gpio21(hw, value);
}

/* REG_0x32:gpio_config2->sys_gpio22:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x32[27:24]*/
uint32_t sys_hal_get_gpio_config2_sys_gpio22(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config2_sys_gpio22(hw);
}

void sys_hal_set_gpio_config2_sys_gpio22(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config2_sys_gpio22(hw, value);
}

/* REG_0x32:gpio_config2->sys_gpio23:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x32[31:28]*/
uint32_t sys_hal_get_gpio_config2_sys_gpio23(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config2_sys_gpio23(hw);
}

void sys_hal_set_gpio_config2_sys_gpio23(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config2_sys_gpio23(hw, value);
}

/* REG_0x33 */

uint32_t sys_hal_get_gpio_config3_value(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config3_value(hw);
}

void sys_hal_set_gpio_config3_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config3_value(hw, value);
}

/* REG_0x33:gpio_config3->sys_gpio24:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x33[3:0]*/
uint32_t sys_hal_get_gpio_config3_sys_gpio24(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config3_sys_gpio24(hw);
}

void sys_hal_set_gpio_config3_sys_gpio24(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config3_sys_gpio24(hw, value);
}

/* REG_0x33:gpio_config3->sys_gpio25:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x33[7:4]*/
uint32_t sys_hal_get_gpio_config3_sys_gpio25(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config3_sys_gpio25(hw);
}

void sys_hal_set_gpio_config3_sys_gpio25(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config3_sys_gpio25(hw, value);
}

/* REG_0x33:gpio_config3->sys_gpio26:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x33[11:8]*/
uint32_t sys_hal_get_gpio_config3_sys_gpio26(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config3_sys_gpio26(hw);
}

void sys_hal_set_gpio_config3_sys_gpio26(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config3_sys_gpio26(hw, value);
}

/* REG_0x33:gpio_config3->sys_gpio27:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x33[15:12]*/
uint32_t sys_hal_get_gpio_config3_sys_gpio27(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config3_sys_gpio27(hw);
}

void sys_hal_set_gpio_config3_sys_gpio27(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config3_sys_gpio27(hw, value);
}

/* REG_0x33:gpio_config3->sys_gpio28:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x33[19:16]*/
uint32_t sys_hal_get_gpio_config3_sys_gpio28(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config3_sys_gpio28(hw);
}

void sys_hal_set_gpio_config3_sys_gpio28(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config3_sys_gpio28(hw, value);
}

/* REG_0x33:gpio_config3->sys_gpio29:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x33[23:20]*/
uint32_t sys_hal_get_gpio_config3_sys_gpio29(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config3_sys_gpio29(hw);
}

void sys_hal_set_gpio_config3_sys_gpio29(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config3_sys_gpio29(hw, value);
}

/* REG_0x33:gpio_config3->sys_gpio30:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x33[27:24]*/
uint32_t sys_hal_get_gpio_config3_sys_gpio30(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config3_sys_gpio30(hw);
}

void sys_hal_set_gpio_config3_sys_gpio30(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config3_sys_gpio30(hw, value);
}

/* REG_0x33:gpio_config3->sys_gpio31:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x33[31:28]*/
uint32_t sys_hal_get_gpio_config3_sys_gpio31(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config3_sys_gpio31(hw);
}

void sys_hal_set_gpio_config3_sys_gpio31(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config3_sys_gpio31(hw, value);
}

/* REG_0x34 */

uint32_t sys_hal_get_gpio_config4_value(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config4_value(hw);
}

void sys_hal_set_gpio_config4_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config4_value(hw, value);
}

/* REG_0x34:gpio_config4->sys_gpio32:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x34[3:0]*/
uint32_t sys_hal_get_gpio_config4_sys_gpio32(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config4_sys_gpio32(hw);
}

void sys_hal_set_gpio_config4_sys_gpio32(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config4_sys_gpio32(hw, value);
}

/* REG_0x34:gpio_config4->sys_gpio33:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x34[7:4]*/
uint32_t sys_hal_get_gpio_config4_sys_gpio33(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config4_sys_gpio33(hw);
}

void sys_hal_set_gpio_config4_sys_gpio33(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config4_sys_gpio33(hw, value);
}

/* REG_0x34:gpio_config4->sys_gpio34:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x34[11:8]*/
uint32_t sys_hal_get_gpio_config4_sys_gpio34(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config4_sys_gpio34(hw);
}

void sys_hal_set_gpio_config4_sys_gpio34(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config4_sys_gpio34(hw, value);
}

/* REG_0x34:gpio_config4->sys_gpio35:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x34[15:12]*/
uint32_t sys_hal_get_gpio_config4_sys_gpio35(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config4_sys_gpio35(hw);
}

void sys_hal_set_gpio_config4_sys_gpio35(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config4_sys_gpio35(hw, value);
}

/* REG_0x34:gpio_config4->sys_gpio36:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x34[19:16]*/
uint32_t sys_hal_get_gpio_config4_sys_gpio36(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config4_sys_gpio36(hw);
}

void sys_hal_set_gpio_config4_sys_gpio36(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config4_sys_gpio36(hw, value);
}

/* REG_0x34:gpio_config4->sys_gpio37:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x34[23:20]*/
uint32_t sys_hal_get_gpio_config4_sys_gpio37(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config4_sys_gpio37(hw);
}

void sys_hal_set_gpio_config4_sys_gpio37(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config4_sys_gpio37(hw, value);
}

/* REG_0x34:gpio_config4->sys_gpio38:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x34[27:24]*/
uint32_t sys_hal_get_gpio_config4_sys_gpio38(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config4_sys_gpio38(hw);
}

void sys_hal_set_gpio_config4_sys_gpio38(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config4_sys_gpio38(hw, value);
}

/* REG_0x34:gpio_config4->sys_gpio39:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x34[31:28]*/
uint32_t sys_hal_get_gpio_config4_sys_gpio39(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config4_sys_gpio39(hw);
}

void sys_hal_set_gpio_config4_sys_gpio39(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config4_sys_gpio39(hw, value);
}

/* REG_0x35 */

uint32_t sys_hal_get_gpio_config5_value(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config5_value(hw);
}

void sys_hal_set_gpio_config5_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config5_value(hw, value);
}

/* REG_0x35:gpio_config5->sys_gpio40:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x35[3:0]*/
uint32_t sys_hal_get_gpio_config5_sys_gpio40(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config5_sys_gpio40(hw);
}

void sys_hal_set_gpio_config5_sys_gpio40(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config5_sys_gpio40(hw, value);
}

/* REG_0x35:gpio_config5->sys_gpio41:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x35[7:4]*/
uint32_t sys_hal_get_gpio_config5_sys_gpio41(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config5_sys_gpio41(hw);
}

void sys_hal_set_gpio_config5_sys_gpio41(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config5_sys_gpio41(hw, value);
}

/* REG_0x35:gpio_config5->sys_gpio42:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x35[11:8]*/
uint32_t sys_hal_get_gpio_config5_sys_gpio42(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config5_sys_gpio42(hw);
}

void sys_hal_set_gpio_config5_sys_gpio42(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config5_sys_gpio42(hw, value);
}

/* REG_0x35:gpio_config5->sys_gpio43:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x35[15:12]*/
uint32_t sys_hal_get_gpio_config5_sys_gpio43(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config5_sys_gpio43(hw);
}

void sys_hal_set_gpio_config5_sys_gpio43(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config5_sys_gpio43(hw, value);
}

/* REG_0x35:gpio_config5->sys_gpio44:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x35[19:16]*/
uint32_t sys_hal_get_gpio_config5_sys_gpio44(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config5_sys_gpio44(hw);
}

void sys_hal_set_gpio_config5_sys_gpio44(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config5_sys_gpio44(hw, value);
}

/* REG_0x35:gpio_config5->sys_gpio45:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x35[23:20]*/
uint32_t sys_hal_get_gpio_config5_sys_gpio45(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config5_sys_gpio45(hw);
}

void sys_hal_set_gpio_config5_sys_gpio45(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config5_sys_gpio45(hw, value);
}

/* REG_0x35:gpio_config5->sys_gpio46:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x35[27:24]*/
uint32_t sys_hal_get_gpio_config5_sys_gpio46(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config5_sys_gpio46(hw);
}

void sys_hal_set_gpio_config5_sys_gpio46(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config5_sys_gpio46(hw, value);
}

/* REG_0x35:gpio_config5->sys_gpio47:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x35[31:28]*/
uint32_t sys_hal_get_gpio_config5_sys_gpio47(sys_hw_t *hw)
{
    return sys_ll_get_gpio_config5_sys_gpio47(hw);
}

void sys_hal_set_gpio_config5_sys_gpio47(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_gpio_config5_sys_gpio47(hw, value);
}

/* REG_0x38 */

uint32_t sys_hal_get_sys_debug_config0_value(sys_hw_t *hw)
{
    return sys_ll_get_sys_debug_config0_value(hw);
}

void sys_hal_set_sys_debug_config0_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_sys_debug_config0_value(hw, value);
}

/* REG_0x38:sys_debug_config0->dbug_config0: ,R/W,0x38[31:0]*/
uint32_t sys_hal_get_sys_debug_config0_dbug_config0(sys_hw_t *hw)
{
    return sys_ll_get_sys_debug_config0_dbug_config0(hw);
}

void sys_hal_set_sys_debug_config0_dbug_config0(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_sys_debug_config0_dbug_config0(hw, value);
}

/* REG_0x39 */

uint32_t sys_hal_get_sys_debug_config1_value(sys_hw_t *hw)
{
    return sys_ll_get_sys_debug_config1_value(hw);
}

void sys_hal_set_sys_debug_config1_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_sys_debug_config1_value(hw, value);
}

/* REG_0x39:sys_debug_config1->dbug_config1:0: btsp_debug[0:32]        1: btsp_debug[32+:32]           2: btsp_debug[64+:32]  4:btsp_debug[96+:6]       5:wifip_mac_dbg[31:0]           6: wifip_phy_dbg[31:0]                            default:  dbug_config0                   ,R/W,0x39[31:0]*/
uint32_t sys_hal_get_sys_debug_config1_dbug_config1(sys_hw_t *hw)
{
    return sys_ll_get_sys_debug_config1_dbug_config1(hw);
}

void sys_hal_set_sys_debug_config1_dbug_config1(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_sys_debug_config1_dbug_config1(hw, value);
}

/* REG_0x40 */

void sys_hal_set_ana_reg0_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg0_value(hw, value);
}

/* REG_0x40:ana_reg0->ck2652sel:1:26MHz/0:52MHz,W,0x40[0]*/
void sys_hal_set_ana_reg0_ck2652sel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg0_ck2652sel(hw, value);
}

/* REG_0x40:ana_reg0->cp:cp curent control 0to 350uA 50uA step,W,0x40[3:1]*/
void sys_hal_set_ana_reg0_cp(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg0_cp(hw, value);
}

/* REG_0x40:ana_reg0->spideten:unlock detect enable fron spi 1:enable,W,0x40[4]*/
void sys_hal_set_ana_reg0_spideten(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg0_spideten(hw, value);
}

/* REG_0x40:ana_reg0->hvref:high vth control for unlock detect 00:0.85V;01:0.9V;10:0.95V;11:1.05V,W,0x40[6:5]*/
void sys_hal_set_ana_reg0_hvref(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg0_hvref(hw, value);
}

/* REG_0x40:ana_reg0->lvref:low vth control for unlock detect 00:0.2V;01:0.3V;10:0.35V;11:0.4V,W,0x40[8:7]*/
void sys_hal_set_ana_reg0_lvref(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg0_lvref(hw, value);
}

/* REG_0x40:ana_reg0->Rzctrl26M:Rz ctrl in 26M mode:1:normal;0:add 14K,W,0x40[9]*/
void sys_hal_set_ana_reg0_rzctrl26m(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg0_rzctrl26m(hw, value);
}

/* REG_0x40:ana_reg0->LoopRzctrl:Rz ctrl:2K to 17K,1K step,W,0x40[13:10]*/
void sys_hal_set_ana_reg0_looprzctrl(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg0_looprzctrl(hw, value);
}

/* REG_0x40:ana_reg0->rpc:second pole Rp ctrl:00:30K;01:10K;10:22K;11:2K,W,0x40[15:14]*/
void sys_hal_set_ana_reg0_rpc(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg0_rpc(hw, value);
}

/* REG_0x40:ana_reg0->nsyn:N divider rst,W,0x40[16]*/
void sys_hal_set_ana_reg0_nsyn(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg0_nsyn(hw, value);
}

/* REG_0x40:ana_reg0->cksel:0:26M;1:40M;2:24M;3:19.2M,W,0x40[18:17]*/
void sys_hal_set_ana_reg0_cksel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg0_cksel(hw, value);
}

/* REG_0x40:ana_reg0->spitrig:SPI band selection trigger signal,W,0x40[19]*/
void sys_hal_set_ana_reg0_spitrig(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg0_spitrig(hw, value);
}

/* REG_0x40:ana_reg0->band:band manual value/band[0] ,W,0x40[24:20]*/
void sys_hal_set_ana_reg0_band(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg0_band(hw, value);
}

/* REG_0x40:ana_reg0->bandmanual:1:band manual;0:band auto,W,0x40[25]*/
void sys_hal_set_ana_reg0_bandmanual(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg0_bandmanual(hw, value);
}

/* REG_0x40:ana_reg0->dsptrig:band selection trigger signal,W,0x40[26]*/
void sys_hal_set_ana_reg0_dsptrig(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg0_dsptrig(hw, value);
}

/* REG_0x40:ana_reg0->lpen_dpll:dpll low power mode enable,W,0x40[27]*/
void sys_hal_set_ana_reg0_lpen_dpll(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg0_lpen_dpll(hw, value);
}

/* REG_0x40:ana_reg0->xamp:xtal OSC amp control/xamp<0> shared with pll_cktst_en,W,0x40[31:28]*/
void sys_hal_set_ana_reg0_xamp(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg0_xamp(hw, value);
}

/* REG_0x41 */

void sys_hal_set_ana_reg1_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg1_value(hw, value);
}

/* REG_0x41:ana_reg1->dpll_vrefsel:dpll ldo reference voltage selection  0:vbg_aon/1:vbg_cal,W,0x41[1]*/
void sys_hal_set_ana_reg1_dpll_vrefsel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg1_dpll_vrefsel(hw, value);
}

/* REG_0x41:ana_reg1->msw:set the frequency of DCO manual,W,0x41[10:2]*/
void sys_hal_set_ana_reg1_msw(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg1_msw(hw, value);
}

/* REG_0x41:ana_reg1->ictrl:controlling the bias cuttent of DCO core,W,0x41[13:11]*/
void sys_hal_set_ana_reg1_ictrl(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg1_ictrl(hw, value);
}

/* REG_0x41:ana_reg1->osc_trig:reset the DCO core by spi to make it oscillate again,W,0x41[14]*/
void sys_hal_set_ana_reg1_osc_trig(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg1_osc_trig(hw, value);
}

/* REG_0x41:ana_reg1->osccal_trig:trigger the action of callibration in the DCO,W,0x41[15]*/
void sys_hal_set_ana_reg1_osccal_trig(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg1_osccal_trig(hw, value);
}

/* REG_0x41:ana_reg1->cnti:set the controlling work of calibration in the DCO block to get the different frequency,W,0x41[24:16]*/
void sys_hal_set_ana_reg1_cnti(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg1_cnti(hw, value);
}

/* REG_0x41:ana_reg1->spi_rst:reset the calibration block of DCO by spi,W,0x41[25]*/
void sys_hal_set_ana_reg1_spi_rst(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg1_spi_rst(hw, value);
}

/* REG_0x41:ana_reg1->amsel:disable the calibration function of the DCO,set the frequency of DCO manual,W,0x41[26]*/
void sys_hal_set_ana_reg1_amsel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg1_amsel(hw, value);
}

/* REG_0x41:ana_reg1->divctrl:controlling the value of divider in the DCO to get the different frequency,W,0x41[29:27]*/
void sys_hal_set_ana_reg1_divctrl(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg1_divctrl(hw, value);
}

/* REG_0x41:ana_reg1->dco_tsten:dco test enable,W,0x41[30]*/
void sys_hal_set_ana_reg1_dco_tsten(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg1_dco_tsten(hw, value);
}

/* REG_0x41:ana_reg1->rosc_tsten:rosc test enable,W,0x41[31]*/
void sys_hal_set_ana_reg1_rosc_tsten(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg1_rosc_tsten(hw, value);
}

/* REG_0x42 */

void sys_hal_set_ana_reg2_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg2_value(hw, value);
}

/* REG_0x42:ana_reg2->pwmscmen:buck nmos disable,W,0x42[0]*/
void sys_hal_set_ana_reg2_pwmscmen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg2_pwmscmen(hw, value);
}

/* REG_0x42:ana_reg2->buck_fasten:buck EA fast transient enable(=1),W,0x42[1]*/
void sys_hal_set_ana_reg2_buck_fasten(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg2_buck_fasten(hw, value);
}

/* REG_0x42:ana_reg2->cls:buck current limit setting,W,0x42[4:2]*/
void sys_hal_set_ana_reg2_cls(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg2_cls(hw, value);
}

/* REG_0x42:ana_reg2->pfms:buck freewheeling damping enable(=1) ,W,0x42[9:5]*/
void sys_hal_set_ana_reg2_pfms(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg2_pfms(hw, value);
}

/* REG_0x42:ana_reg2->ripc:buck pfm mode voltage ripple control setting,W,0x42[12:10]*/
void sys_hal_set_ana_reg2_ripc(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg2_ripc(hw, value);
}

/* REG_0x42:ana_reg2->rampc:buck ramping compensation setting,W,0x42[16:13]*/
void sys_hal_set_ana_reg2_rampc(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg2_rampc(hw, value);
}

/* REG_0x42:ana_reg2->rampcen:buck ramping compensation enable(=1),W,0x42[17]*/
void sys_hal_set_ana_reg2_rampcen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg2_rampcen(hw, value);
}

/* REG_0x42:ana_reg2->dpfmen:buck pfm mode current reduce enable(=1),W,0x42[18]*/
void sys_hal_set_ana_reg2_dpfmen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg2_dpfmen(hw, value);
}

/* REG_0x42:ana_reg2->pfmen:buck pfm mode enable(=1),W,0x42[19]*/
void sys_hal_set_ana_reg2_pfmen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg2_pfmen(hw, value);
}

/* REG_0x42:ana_reg2->forcepfm:buck force pfm mode(=1),W,0x42[20]*/
void sys_hal_set_ana_reg2_forcepfm(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg2_forcepfm(hw, value);
}

/* REG_0x42:ana_reg2->swrsten:buck freewheeling damping enable(=1) ,W,0x42[21]*/
void sys_hal_set_ana_reg2_swrsten(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg2_swrsten(hw, value);
}

/* REG_0x42:ana_reg2->tmposel:buck mpo pulse width control 0--shortest   3---longest,W,0x42[23:22]*/
void sys_hal_set_ana_reg2_tmposel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg2_tmposel(hw, value);
}

/* REG_0x42:ana_reg2->mpoen:buck mpo mode enable( =1),W,0x42[24]*/
void sys_hal_set_ana_reg2_mpoen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg2_mpoen(hw, value);
}

/* REG_0x42:ana_reg2->spi_latchb:spi latch disable 0:latch;1:no latch,W,0x42[25]*/
void sys_hal_set_ana_reg2_spi_latchb(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg2_spi_latchb(hw, value);
}

/* REG_0x42:ana_reg2->ldosel:ldo/buck select, 0:buck;1:LDO,W,0x42[26]*/
void sys_hal_set_ana_reg2_ldosel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg2_ldosel(hw, value);
}

/* REG_0x42:ana_reg2->iovoc:ioldo output voltage select 0:2.9V,….7:3.6V,W,0x42[29:27]*/
void sys_hal_set_ana_reg2_iovoc(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg2_iovoc(hw, value);
}

/* REG_0x42:ana_reg2->vbpbuf_hp:vbspbuffer high power enable,W,0x42[30]*/
void sys_hal_set_ana_reg2_vbpbuf_hp(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg2_vbpbuf_hp(hw, value);
}

/* REG_0x42:ana_reg2->bypassen:ioldo bypass enable,W,0x42[31]*/
void sys_hal_set_ana_reg2_bypassen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg2_bypassen(hw, value);
}

/* REG_0x43 */

void sys_hal_set_ana_reg3_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg3_value(hw, value);
}

/* REG_0x43:ana_reg3->zcdta:buck zcd delay tune setting,W,0x43[4:0]*/
void sys_hal_set_ana_reg3_zcdta(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg3_zcdta(hw, value);
}

/* REG_0x43:ana_reg3->zcdcala:buck zcd offset cali setting,W,0x43[10:5]*/
void sys_hal_set_ana_reg3_zcdcala(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg3_zcdcala(hw, value);
}

/* REG_0x43:ana_reg3->zcdmen:buck zcd manual cali enable(=1),W,0x43[11]*/
void sys_hal_set_ana_reg3_zcdmen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg3_zcdmen(hw, value);
}

/* REG_0x43:ana_reg3->zcdcalen:buck zcd calibration enable(=1),W,0x43[12]*/
void sys_hal_set_ana_reg3_zcdcalen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg3_zcdcalen(hw, value);
}

/* REG_0x43:ana_reg3->zcdcal_tri:buck zcd auto cali triggle(0-->1),W,0x43[13]*/
void sys_hal_set_ana_reg3_zcdcal_tri(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg3_zcdcal_tri(hw, value);
}

/* REG_0x43:ana_reg3->mroscsel:buck oscillator manual cali. enable(=1),W,0x43[14]*/
void sys_hal_set_ana_reg3_mroscsel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg3_mroscsel(hw, value);
}

/* REG_0x43:ana_reg3->mfsel:buck oscillator manual fsel  ,W,0x43[17:15]*/
void sys_hal_set_ana_reg3_mfsel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg3_mfsel(hw, value);
}

/* REG_0x43:ana_reg3->mroscbcal:buck oscillator manual cap_cal  0xA---500k 0xB--1M 0x9---2M,W,0x43[21:18]*/
void sys_hal_set_ana_reg3_mroscbcal(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg3_mroscbcal(hw, value);
}

/* REG_0x43:ana_reg3->osccaltrig:buck oscillator manual cali. enable(=1),W,0x43[22]*/
void sys_hal_set_ana_reg3_osccaltrig(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg3_osccaltrig(hw, value);
}

/* REG_0x43:ana_reg3->ckintsel:buck clock source select  1-- ring oscillator   0--divider,W,0x43[23]*/
void sys_hal_set_ana_reg3_ckintsel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg3_ckintsel(hw, value);
}

/* REG_0x43:ana_reg3->ckfs:buck output clock freq. select   0--500k 1---1M  2--2M  3--4M,W,0x43[25:24]*/
void sys_hal_set_ana_reg3_ckfs(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg3_ckfs(hw, value);
}

/* REG_0x43:ana_reg3->vlsel_ldodig:digldo output voltage select(low power)  0:0.6V,…..7:1.4V,W,0x43[28:26]*/
void sys_hal_set_ana_reg3_vlsel_ldodig(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg3_vlsel_ldodig(hw, value);
}

/* REG_0x43:ana_reg3->vhsel_ldodig:digldo output voltage select(high power)  0:0.6V,…..7:1.4V,W,0x43[31:29]*/
void sys_hal_set_ana_reg3_vhsel_ldodig(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg3_vhsel_ldodig(hw, value);
}

/* REG_0x44 */

void sys_hal_set_ana_reg4_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg4_value(hw, value);
}

/* REG_0x44:ana_reg4->cb_manu_val:CB Calibration Manual Value,W,0x44[9:5]*/
void sys_hal_set_ana_reg4_cb_manu_val(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg4_cb_manu_val(hw, value);
}

/* REG_0x44:ana_reg4->cb_cal_trig:CB Calibration Trigger,W,0x44[10]*/
void sys_hal_set_ana_reg4_cb_cal_trig(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg4_cb_cal_trig(hw, value);
}

/* REG_0x44:ana_reg4->cb_cal_manu:CB Calibration Manual Mode ,W,0x44[11]*/
void sys_hal_set_ana_reg4_cb_cal_manu(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg4_cb_cal_manu(hw, value);
}

/* REG_0x44:ana_reg4->rosc_cal_intval:Rosc Calibration Interlval 0.25s~2s,W,0x44[14:12]*/
void sys_hal_set_ana_reg4_rosc_cal_intval(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg4_rosc_cal_intval(hw, value);
}

/* REG_0x44:ana_reg4->manu_cin:Rosc Calibration Manual Cin,W,0x44[21:15]*/
void sys_hal_set_ana_reg4_manu_cin(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg4_manu_cin(hw, value);
}

/* REG_0x44:ana_reg4->manu_fin:Rosc Calibration Manual Fin,W,0x44[26:22]*/
void sys_hal_set_ana_reg4_manu_fin(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg4_manu_fin(hw, value);
}

/* REG_0x44:ana_reg4->rosc_cal_mode:Rosc Calibration Mode:; 0x1: 32K; 0x0: 31.25K,W,0x44[27]*/
void sys_hal_set_ana_reg4_rosc_cal_mode(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg4_rosc_cal_mode(hw, value);
}

/* REG_0x44:ana_reg4->rosc_cal_trig:Rosc Calibration Trigger,W,0x44[28]*/
void sys_hal_set_ana_reg4_rosc_cal_trig(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg4_rosc_cal_trig(hw, value);
}

/* REG_0x44:ana_reg4->rosc_cal_en:Rosc Calibration Enable,W,0x44[29]*/
void sys_hal_set_ana_reg4_rosc_cal_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg4_rosc_cal_en(hw, value);
}

/* REG_0x44:ana_reg4->rosc_manu_en:Rosc Calibration Manual Mode ,W,0x44[30]*/
void sys_hal_set_ana_reg4_rosc_manu_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg4_rosc_manu_en(hw, value);
}

/* REG_0x44:ana_reg4->rosc_tsten:Rosc test enable,W,0x44[31]*/
void sys_hal_set_ana_reg4_rosc_tsten(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg4_rosc_tsten(hw, value);
}

/* REG_0x45 */

void sys_hal_set_ana_reg5_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg5_value(hw, value);
}

/* REG_0x45:ana_reg5->vref_scale:gadc reference voltage scale enable,W,0x45[0]*/
void sys_hal_set_ana_reg5_vref_scale(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg5_vref_scale(hw, value);
}

/* REG_0x45:ana_reg5->dccal_en:gadc DC calibration enable,W,0x45[1]*/
void sys_hal_set_ana_reg5_dccal_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg5_dccal_en(hw, value);
}

/* REG_0x45:ana_reg5->xtalh_ctune:xtalh load cap tuning,W,0x45[8:2]*/
void sys_hal_set_ana_reg5_xtalh_ctune(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg5_xtalh_ctune(hw, value);
}

/* REG_0x45:ana_reg5->cktst_sel:clock test signal selection rosc/xtall/dco/dpll,W,0x45[10:9]*/
void sys_hal_set_ana_reg5_cktst_sel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg5_cktst_sel(hw, value);
}

/* REG_0x45:ana_reg5->ck_tst_enbale:system clock test enable,W,0x45[11]*/
void sys_hal_set_ana_reg5_ck_tst_enbale(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg5_ck_tst_enbale(hw, value);
}

/* REG_0x45:ana_reg5->trxt_tst_enable:wifi trx test enable,W,0x45[12]*/
void sys_hal_set_ana_reg5_trxt_tst_enable(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg5_trxt_tst_enable(hw, value);
}

/* REG_0x45:ana_reg5->encb:global central bias enable,W,0x45[13]*/
void sys_hal_set_ana_reg5_encb(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg5_encb(hw, value);
}

/* REG_0x45:ana_reg5->vctrl_dpllldo:dpll ldo output selection,W,0x45[15:14]*/
void sys_hal_set_ana_reg5_vctrl_dpllldo(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg5_vctrl_dpllldo(hw, value);
}

/* REG_0x45:ana_reg5->vctrl_sysldo:sys ldo output selection,W,0x45[17:16]*/
void sys_hal_set_ana_reg5_vctrl_sysldo(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg5_vctrl_sysldo(hw, value);
}

/* REG_0x45:ana_reg5->temptst_en:tempdet test enable,W,0x45[18]*/
void sys_hal_set_ana_reg5_temptst_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg5_temptst_en(hw, value);
}

/* REG_0x45:ana_reg5->gadc_tsel:gadc test signal selection,W,0x45[21:19]*/
void sys_hal_set_ana_reg5_gadc_tsel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg5_gadc_tsel(hw, value);
}

/* REG_0x45:ana_reg5->xtalh_ictrl:xtalh current control,W,0x45[22]*/
void sys_hal_set_ana_reg5_xtalh_ictrl(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg5_xtalh_ictrl(hw, value);
}

/* REG_0x45:ana_reg5->bgcalm:bandgap calibration manual setting,W,0x45[28:23]*/
void sys_hal_set_ana_reg5_bgcalm(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg5_bgcalm(hw, value);
}

/* REG_0x45:ana_reg5->bgcal_trig:bandgap calibrarion trig,W,0x45[29]*/
void sys_hal_set_ana_reg5_bgcal_trig(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg5_bgcal_trig(hw, value);
}

/* REG_0x45:ana_reg5->bgcal_manu:bandgap calibration manual mode enable,W,0x45[30]*/
void sys_hal_set_ana_reg5_bgcal_manu(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg5_bgcal_manu(hw, value);
}

/* REG_0x45:ana_reg5->bgcal_en:bandgap calibration enable,W,0x45[31]*/
void sys_hal_set_ana_reg5_bgcal_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg5_bgcal_en(hw, value);
}

/* REG_0x46 */

void sys_hal_set_ana_reg6_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_value(hw, value);
}

/* REG_0x46:ana_reg6->itune_xtall:xtall core current control,W,0x46[3:0]*/
void sys_hal_set_ana_reg6_itune_xtall(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_itune_xtall(hw, value);
}

/* REG_0x46:ana_reg6->xtall_ten:xtall test enable,W,0x46[4]*/
void sys_hal_set_ana_reg6_xtall_ten(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_xtall_ten(hw, value);
}

/* REG_0x46:ana_reg6->psldo_vsel:ps ldo output voltage selection,0:VIO /1:1.8V,W,0x46[5]*/
void sys_hal_set_ana_reg6_psldo_vsel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_psldo_vsel(hw, value);
}

/* REG_0x46:ana_reg6->en_usb:usb phy enable,W,0x46[6]*/
void sys_hal_set_ana_reg6_en_usb(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_en_usb(hw, value);
}

/* REG_0x46:ana_reg6->en_xtall:xtall oscillator enable,W,0x46[7]*/
void sys_hal_set_ana_reg6_en_xtall(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_en_xtall(hw, value);
}

/* REG_0x46:ana_reg6->en_dco:dco enable,W,0x46[8]*/
void sys_hal_set_ana_reg6_en_dco(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_en_dco(hw, value);
}

/* REG_0x46:ana_reg6->en_psram_ldo:psram ldo enable,W,0x46[9]*/
void sys_hal_set_ana_reg6_en_psram_ldo(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_en_psram_ldo(hw, value);
}

/* REG_0x46:ana_reg6->en_tempdet:tempreture det enable,W,0x46[10]*/
void sys_hal_set_ana_reg6_en_tempdet(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_en_tempdet(hw, value);
}

/* REG_0x46:ana_reg6->en_audpll:audio pll enable,W,0x46[11]*/
void sys_hal_set_ana_reg6_en_audpll(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_en_audpll(hw, value);
}

/* REG_0x46:ana_reg6->en_dpll:dpll enable,W,0x46[12]*/
void sys_hal_set_ana_reg6_en_dpll(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_en_dpll(hw, value);
}

/* REG_0x46:ana_reg6->en_sysldo:sysldo enable,W,0x46[13]*/
void sys_hal_set_ana_reg6_en_sysldo(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_en_sysldo(hw, value);
}

/* REG_0x46:ana_reg6->pwd_gadc_buf:gadc input buffer pwd,W,0x46[17]*/
void sys_hal_set_ana_reg6_pwd_gadc_buf(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_pwd_gadc_buf(hw, value);
}

/* REG_0x46:ana_reg6->xtal_hpsrr_en:xtal high psrr buffer enable,W,0x46[18]*/
void sys_hal_set_ana_reg6_xtal_hpsrr_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_xtal_hpsrr_en(hw, value);
}

/* REG_0x46:ana_reg6->en_xtal2rf:xtal clock to rfpll gate enable ,W,0x46[19]*/
void sys_hal_set_ana_reg6_en_xtal2rf(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_en_xtal2rf(hw, value);
}

/* REG_0x46:ana_reg6->en_sleep:xtal sleep enable,W,0x46[20]*/
void sys_hal_set_ana_reg6_en_sleep(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_en_sleep(hw, value);
}

/* REG_0x46:ana_reg6->clkbuf_hd:xtal lpsrr clock buffer high power mode ,W,0x46[21]*/
void sys_hal_set_ana_reg6_clkbuf_hd(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_clkbuf_hd(hw, value);
}

/* REG_0x46:ana_reg6->clkbuf_dsel_manu:xtal lpsrr clock buffer power mode selection 0: auto /1:manu ,W,0x46[22]*/
void sys_hal_set_ana_reg6_clkbuf_dsel_manu(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_clkbuf_dsel_manu(hw, value);
}

/* REG_0x46:ana_reg6->xtal_lpmode_ctrl:xtal core low power mode enable,W,0x46[23]*/
void sys_hal_set_ana_reg6_xtal_lpmode_ctrl(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_xtal_lpmode_ctrl(hw, value);
}

/* REG_0x46:ana_reg6->rxtal_lp:xtal bias current setting at low power mode ,W,0x46[27:24]*/
void sys_hal_set_ana_reg6_rxtal_lp(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_rxtal_lp(hw, value);
}

/* REG_0x46:ana_reg6->rxtal_hp:xtal26m bias current setting at high power mode ,W,0x46[31:28]*/
void sys_hal_set_ana_reg6_rxtal_hp(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg6_rxtal_hp(hw, value);
}

/* REG_0x47 */

void sys_hal_set_ana_reg7_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg7_value(hw, value);
}

/* REG_0x47:ana_reg7->rng_tstck_sel:trng setting,W,0x47[0]*/
void sys_hal_set_ana_reg7_rng_tstck_sel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg7_rng_tstck_sel(hw, value);
}

/* REG_0x47:ana_reg7->rng_tsten:trng setting,W,0x47[1]*/
void sys_hal_set_ana_reg7_rng_tsten(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg7_rng_tsten(hw, value);
}

/* REG_0x47:ana_reg7->itune_ref:trng setting,W,0x47[4:2]*/
void sys_hal_set_ana_reg7_itune_ref(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg7_itune_ref(hw, value);
}

/* REG_0x47:ana_reg7->itune_opa:trng setting,W,0x47[7:5]*/
void sys_hal_set_ana_reg7_itune_opa(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg7_itune_opa(hw, value);
}

/* REG_0x47:ana_reg7->itune_cmp:trng setting,W,0x47[10:8]*/
void sys_hal_set_ana_reg7_itune_cmp(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg7_itune_cmp(hw, value);
}

/* REG_0x47:ana_reg7->Rnooise_sel:trng setting,W,0x47[11]*/
void sys_hal_set_ana_reg7_rnooise_sel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg7_rnooise_sel(hw, value);
}

/* REG_0x47:ana_reg7->Fslow_sel:trng setting,W,0x47[14:12]*/
void sys_hal_set_ana_reg7_fslow_sel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg7_fslow_sel(hw, value);
}

/* REG_0x47:ana_reg7->Ffast_sel:trng setting,W,0x47[18:15]*/
void sys_hal_set_ana_reg7_ffast_sel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg7_ffast_sel(hw, value);
}

/* REG_0x47:ana_reg7->gadc_cal_sel:gadc calibration mode selection,W,0x47[20:19]*/
void sys_hal_set_ana_reg7_gadc_cal_sel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg7_gadc_cal_sel(hw, value);
}

/* REG_0x47:ana_reg7->gadc_ten:gadc test enable,W,0x47[21]*/
void sys_hal_set_ana_reg7_gadc_ten(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg7_gadc_ten(hw, value);
}

/* REG_0x47:ana_reg7->gadc_cmp_ictrl:gadc comparaor current select ,W,0x47[25:22]*/
void sys_hal_set_ana_reg7_gadc_cmp_ictrl(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg7_gadc_cmp_ictrl(hw, value);
}

/* REG_0x47:ana_reg7->gadc_buf_ictrl:gadc buffer current select ,W,0x47[29:26]*/
void sys_hal_set_ana_reg7_gadc_buf_ictrl(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg7_gadc_buf_ictrl(hw, value);
}

/* REG_0x47:ana_reg7->vref_sel:gadc input reference select, 0：bandgap signal 1:GPIO voltage divided,W,0x47[30]*/
void sys_hal_set_ana_reg7_vref_sel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg7_vref_sel(hw, value);
}

/* REG_0x47:ana_reg7->scal_en:gadc reference scale enable, 0:normal mode,1: scale mode ,W,0x47[31]*/
void sys_hal_set_ana_reg7_scal_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg7_scal_en(hw, value);
}

/* REG_0x48 */

void sys_hal_set_ana_reg8_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg8_value(hw, value);
}

/* REG_0x48:ana_reg8->cap_calspi:manul mode ,input cap calibretion value,W,0x48[8:0]*/
void sys_hal_set_ana_reg8_cap_calspi(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg8_cap_calspi(hw, value);
}

/* REG_0x48:ana_reg8->gain_s:Sensitivity level selection,W,0x48[10:9]*/
void sys_hal_set_ana_reg8_gain_s(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg8_gain_s(hw, value);
}

/* REG_0x48:ana_reg8->pwd_td:power down touch module,W,0x48[11]*/
void sys_hal_set_ana_reg8_pwd_td(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg8_pwd_td(hw, value);
}

/* REG_0x48:ana_reg8->en_fsr:low power mode ,enable fast response,W,0x48[12]*/
void sys_hal_set_ana_reg8_en_fsr(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg8_en_fsr(hw, value);
}

/* REG_0x48:ana_reg8->en_scm:scan mode enable,W,0x48[13]*/
void sys_hal_set_ana_reg8_en_scm(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg8_en_scm(hw, value);
}

/* REG_0x48:ana_reg8->en_adcmode:adc mode enable,W,0x48[14]*/
void sys_hal_set_ana_reg8_en_adcmode(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg8_en_adcmode(hw, value);
}

/* REG_0x48:ana_reg8->en_lpmode:low power mode enable,W,0x48[15]*/
void sys_hal_set_ana_reg8_en_lpmode(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg8_en_lpmode(hw, value);
}

/* REG_0x48:ana_reg8->chs_scan:scan mode chan selection,W,0x48[31:16]*/
void sys_hal_set_ana_reg8_chs_scan(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg8_chs_scan(hw, value);
}

/* REG_0x49 */

void sys_hal_set_ana_reg9_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg9_value(hw, value);
}

/* REG_0x49:ana_reg9->en_otp_spi:otp ldo spi enable,W,0x49[0]*/
void sys_hal_set_ana_reg9_en_otp_spi(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg9_en_otp_spi(hw, value);
}

/* REG_0x49:ana_reg9->digovr_en:digldo over voltage reset enable,W,0x49[13]*/
void sys_hal_set_ana_reg9_digovr_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg9_digovr_en(hw, value);
}

/* REG_0x49:ana_reg9->usbpen:usb dp driver capability control,W,0x49[17:14]*/
void sys_hal_set_ana_reg9_usbpen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg9_usbpen(hw, value);
}

/* REG_0x49:ana_reg9->usbnen:usb dn driver capability control,W,0x49[21:18]*/
void sys_hal_set_ana_reg9_usbnen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg9_usbnen(hw, value);
}

/* REG_0x49:ana_reg9->usb_speed:usb speed selection,W,0x49[22]*/
void sys_hal_set_ana_reg9_usb_speed(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg9_usb_speed(hw, value);
}

/* REG_0x49:ana_reg9->usb_deepsleep:usb deepsleep mode enable by spi,W,0x49[23]*/
void sys_hal_set_ana_reg9_usb_deepsleep(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg9_usb_deepsleep(hw, value);
}

/* REG_0x49:ana_reg9->man_mode:manul mode enable,W,0x49[24]*/
void sys_hal_set_ana_reg9_man_mode(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg9_man_mode(hw, value);
}

/* REG_0x49:ana_reg9->crg:detect range selection :8pF/12pF/19pF/27pF,W,0x49[26:25]*/
void sys_hal_set_ana_reg9_crg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg9_crg(hw, value);
}

/* REG_0x49:ana_reg9->vrefs:detect threshold selection ,W,0x49[29:27]*/
void sys_hal_set_ana_reg9_vrefs(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg9_vrefs(hw, value);
}

/* REG_0x49:ana_reg9->en_cal:calibretion enable,W,0x49[31]*/
void sys_hal_set_ana_reg9_en_cal(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg9_en_cal(hw, value);
}

/* REG_0x4a */

void sys_hal_set_ana_reg10_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg10_value(hw, value);
}

/* REG_0x4a:ana_reg10->sdm_val:audio pll sdm value,W,0x4a[29:0]*/
void sys_hal_set_ana_reg10_sdm_val(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg10_sdm_val(hw, value);
}

/* REG_0x4a:ana_reg10->vco_hfreq_enb:audio pll vco high frequency enb,W,0x4a[30]*/
void sys_hal_set_ana_reg10_vco_hfreq_enb(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg10_vco_hfreq_enb(hw, value);
}

/* REG_0x4a:ana_reg10->cal_refen:cal_ref enable of audio pll,W,0x4a[31]*/
void sys_hal_set_ana_reg10_cal_refen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg10_cal_refen(hw, value);
}

/* REG_0x4b */

void sys_hal_set_ana_reg11_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_value(hw, value);
}

/* REG_0x4b:ana_reg11->int_mod:DPLL integer mode enable; 0: fractional mode; 1: integer mode,W,0x4b[0]*/
void sys_hal_set_ana_reg11_int_mod(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_int_mod(hw, value);
}

/* REG_0x4b:ana_reg11->Nsyn:DPLL Ncoutner reset ,W,0x4b[1]*/
void sys_hal_set_ana_reg11_nsyn(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_nsyn(hw, value);
}

/* REG_0x4b:ana_reg11->selpol:DPLL PFD polarity control,W,0x4b[2]*/
void sys_hal_set_ana_reg11_selpol(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_selpol(hw, value);
}

/* REG_0x4b:ana_reg11->reset:DPLL reset,W,0x4b[3]*/
void sys_hal_set_ana_reg11_reset(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_reset(hw, value);
}

/* REG_0x4b:ana_reg11->Ioffset:DPLL  charge pump offset current control,W,0x4b[6:4]*/
void sys_hal_set_ana_reg11_ioffset(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_ioffset(hw, value);
}

/* REG_0x4b:ana_reg11->LPFRz:DPLL Rz control of LPF,W,0x4b[10:7]*/
void sys_hal_set_ana_reg11_lpfrz(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_lpfrz(hw, value);
}

/* REG_0x4b:ana_reg11->Rp2:DPLL Rp control of LPF,W,0x4b[13:11]*/
void sys_hal_set_ana_reg11_rp2(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_rp2(hw, value);
}

/* REG_0x4b:ana_reg11->vsel_cal:DPLL vtrl selection during VCO band calibration,W,0x4b[14]*/
void sys_hal_set_ana_reg11_vsel_cal(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_vsel_cal(hw, value);
}

/* REG_0x4b:ana_reg11->kctrl:DPLL Kvco control,W,0x4b[16:15]*/
void sys_hal_set_ana_reg11_kctrl(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_kctrl(hw, value);
}

/* REG_0x4b:ana_reg11->ckref_loop_sel:polarity selection of referenc clock  to SDM,W,0x4b[17]*/
void sys_hal_set_ana_reg11_ckref_loop_sel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_ckref_loop_sel(hw, value);
}

/* REG_0x4b:ana_reg11->spi_trigger:DPLL band calibration spi trigger,W,0x4b[18]*/
void sys_hal_set_ana_reg11_spi_trigger(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_spi_trigger(hw, value);
}

/* REG_0x4b:ana_reg11->manual:DPLL VCO band manual enable; 0: auto mode; 1: manual mode,W,0x4b[19]*/
void sys_hal_set_ana_reg11_manual(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_manual(hw, value);
}

/* REG_0x4b:ana_reg11->Icp:DPLL charge pump current control; 000: 5uA; 001: 10uA; 010: 15uA; 011: 20uA; 100: 25uA; 101: 30uA; 110: 35uA; 111: 40uA,W,0x4b[22:20]*/
void sys_hal_set_ana_reg11_icp(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_icp(hw, value);
}

/* REG_0x4b:ana_reg11->Rsel:DPLL reference clock selection; 0: 13M; 1: 6.5M,W,0x4b[23]*/
void sys_hal_set_ana_reg11_rsel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_rsel(hw, value);
}

/* REG_0x4b:ana_reg11->ck26Men:xtal26M clock for audio enable,W,0x4b[24]*/
void sys_hal_set_ana_reg11_ck26men(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_ck26men(hw, value);
}

/* REG_0x4b:ana_reg11->ckaudio_outen:DPLL clock output to PAD enable,W,0x4b[25]*/
void sys_hal_set_ana_reg11_ckaudio_outen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_ckaudio_outen(hw, value);
}

/* REG_0x4b:ana_reg11->divctrl:DPLL divider control; 000: div1; 001: div2; 010: div4; 011: div8; 1xx: div16,W,0x4b[28:26]*/
void sys_hal_set_ana_reg11_divctrl(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_divctrl(hw, value);
}

/* REG_0x4b:ana_reg11->cksel:DPLL divider control; 0: div3; 1: div4,W,0x4b[29]*/
void sys_hal_set_ana_reg11_cksel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_cksel(hw, value);
}

/* REG_0x4b:ana_reg11->usben:DPLL clock for USB enable,W,0x4b[30]*/
void sys_hal_set_ana_reg11_usben(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_usben(hw, value);
}

/* REG_0x4b:ana_reg11->audioen:DPLL clock for audio enable,W,0x4b[31]*/
void sys_hal_set_ana_reg11_audioen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg11_audioen(hw, value);
}

/* REG_0x4c */

void sys_hal_set_ana_reg12_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg12_value(hw, value);
}

/* REG_0x4c:ana_reg12->digmic_ckinv:digmic clock inversion enable,W,0x4c[2]*/
void sys_hal_set_ana_reg12_digmic_ckinv(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg12_digmic_ckinv(hw, value);
}

/* REG_0x4c:ana_reg12->enmicdig:digmic enable,w,0x4c[3]*/
void sys_hal_set_ana_reg12_enmicdig(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg12_enmicdig(hw, value);
}

/* REG_0x4c:ana_reg12->audck_rlcen:audio clock re-latch enable,W,0x4c[4]*/
void sys_hal_set_ana_reg12_audck_rlcen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg12_audck_rlcen(hw, value);
}

/* REG_0x4c:ana_reg12->lchckinven:audio clock re-latch clock inversion enable,W,0x4c[5]*/
void sys_hal_set_ana_reg12_lchckinven(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg12_lchckinven(hw, value);
}

/* REG_0x4c:ana_reg12->ldo1v_vsel1v:audio 1.0V LDO selection, 000=0.8, 1X1=1.0,W,0x4c[8:6]*/
void sys_hal_set_ana_reg12_ldo1v_vsel1v(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg12_ldo1v_vsel1v(hw, value);
}

/* REG_0x4c:ana_reg12->ldo1v_adj:audio 1.0V LDO output trimming, 00000=min, 11111=max,W,0x4c[13:9]*/
void sys_hal_set_ana_reg12_ldo1v_adj(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg12_ldo1v_adj(hw, value);
}

/* REG_0x4c:ana_reg12->audvdd_trm1v:audio 1.5V LDO selection, 00=min, 11=max,W,0x4c[15:14]*/
void sys_hal_set_ana_reg12_audvdd_trm1v(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg12_audvdd_trm1v(hw, value);
}

/* REG_0x4c:ana_reg12->audvdd_voc1v:audio 1.5V LDO output trimming, 00000=min, 11111=max,W,0x4c[20:16]*/
void sys_hal_set_ana_reg12_audvdd_voc1v(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg12_audvdd_voc1v(hw, value);
}

/* REG_0x4c:ana_reg12->enaudvdd1v:audio 1.0V LDO enable,W,0x4c[21]*/
void sys_hal_set_ana_reg12_enaudvdd1v(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg12_enaudvdd1v(hw, value);
}

/* REG_0x4c:ana_reg12->loadhp:audio 1.5V LDO, 1=good stability with small loading,W,0x4c[22]*/
void sys_hal_set_ana_reg12_loadhp(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg12_loadhp(hw, value);
}

/* REG_0x4c:ana_reg12->enaudvdd1v5:audio 1.5V LDO enable,W,0x4c[23]*/
void sys_hal_set_ana_reg12_enaudvdd1v5(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg12_enaudvdd1v5(hw, value);
}

/* REG_0x4c:ana_reg12->enmicbias1v:micbias enable,W,0x4c[24]*/
void sys_hal_set_ana_reg12_enmicbias1v(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg12_enmicbias1v(hw, value);
}

/* REG_0x4c:ana_reg12->micbias_trim:micbias output selection, 00=min, 11=max,W,0x4c[26:25]*/
void sys_hal_set_ana_reg12_micbias_trim(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg12_micbias_trim(hw, value);
}

/* REG_0x4c:ana_reg12->micbias_voc1v:micbias output trimming, 00000=min, 11111=max,W,0x4c[31:27]*/
void sys_hal_set_ana_reg12_micbias_voc1v(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg12_micbias_voc1v(hw, value);
}

/* REG_0x4d */

void sys_hal_set_ana_reg13_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg13_value(hw, value);
}

/* REG_0x4d:ana_reg13->byp_dwaadc:adc dwa pass enable,W,0x4d[8]*/
void sys_hal_set_ana_reg13_byp_dwaadc(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg13_byp_dwaadc(hw, value);
}

/* REG_0x4d:ana_reg13->rst:rst,W,0x4d[9]*/
void sys_hal_set_ana_reg13_rst(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg13_rst(hw, value);
}

/* REG_0x4d:ana_reg13->adcdwa_mode:adc dwa model sel,W,0x4d[10]*/
void sys_hal_set_ana_reg13_adcdwa_mode(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg13_adcdwa_mode(hw, value);
}

/* REG_0x4d:ana_reg13->vodadjspi:adc reference manual spi control,W,0x4d[15:11]*/
void sys_hal_set_ana_reg13_vodadjspi(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg13_vodadjspi(hw, value);
}

/* REG_0x4d:ana_reg13->refvsel:0= high reference; 1=small reference,W,0x4d[21]*/
void sys_hal_set_ana_reg13_refvsel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg13_refvsel(hw, value);
}

/* REG_0x4d:ana_reg13->capsw1v:munual value for cap trimming,W,0x4d[27:23]*/
void sys_hal_set_ana_reg13_capsw1v(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg13_capsw1v(hw, value);
}

/* REG_0x4d:ana_reg13->adcckinven:audio adc clock inversion enable,W,0x4d[30]*/
void sys_hal_set_ana_reg13_adcckinven(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg13_adcckinven(hw, value);
}

/* REG_0x4e */

void sys_hal_set_ana_reg14_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg14_value(hw, value);
}

/* REG_0x4e:ana_reg14->isel:adc bias trimming,W,0x4e[1:0]*/
void sys_hal_set_ana_reg14_isel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg14_isel(hw, value);
}

/* REG_0x4e:ana_reg14->micdcocdin:adc micmode dcoc din,W,0x4e[9:2]*/
void sys_hal_set_ana_reg14_micdcocdin(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg14_micdcocdin(hw, value);
}

/* REG_0x4e:ana_reg14->micdcocvc:adc micmode dcoc control,W,0x4e[11:10]*/
void sys_hal_set_ana_reg14_micdcocvc(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg14_micdcocvc(hw, value);
}

/* REG_0x4e:ana_reg14->micdcocen_n:adc micmode dcoc enable,W,0x4e[12]*/
void sys_hal_set_ana_reg14_micdcocen_n(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg14_micdcocen_n(hw, value);
}

/* REG_0x4e:ana_reg14->micdcocen_p:adc micmode dcoc enable,W,0x4e[13]*/
void sys_hal_set_ana_reg14_micdcocen_p(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg14_micdcocen_p(hw, value);
}

/* REG_0x4e:ana_reg14->micsingleEn:adc micmode, single_end enable,W,0x4e[14]*/
void sys_hal_set_ana_reg14_micsingleen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg14_micsingleen(hw, value);
}

/* REG_0x4e:ana_reg14->micGain:adc micmode gain, 0=0dB(17.9K), F=24dB(267.6K), 2dB/step,W,0x4e[18:15]*/
void sys_hal_set_ana_reg14_micgain(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg14_micgain(hw, value);
}

/* REG_0x4e:ana_reg14->micdacen:adc micmode micdac enable,W,0x4e[19]*/
void sys_hal_set_ana_reg14_micdacen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg14_micdacen(hw, value);
}

/* REG_0x4e:ana_reg14->micdaciH:adc micmode, micdac input ,W,0x4e[27:20]*/
void sys_hal_set_ana_reg14_micdacih(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg14_micdacih(hw, value);
}

/* REG_0x4e:ana_reg14->micdacit:adc micmode, mic_dac Iout Full-range, 00=280uA, 01=320uA, 1X=447uA,W,0x4e[29:28]*/
void sys_hal_set_ana_reg14_micdacit(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg14_micdacit(hw, value);
}

/* REG_0x4e:ana_reg14->hcen:adc 1stg op current trimming,W,0x4e[30]*/
void sys_hal_set_ana_reg14_hcen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg14_hcen(hw, value);
}

/* REG_0x4e:ana_reg14->micEn:mic1 mode enable,W,0x4e[31]*/
void sys_hal_set_ana_reg14_micen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg14_micen(hw, value);
}

/* REG_0x4f */

void sys_hal_set_ana_reg15_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg15_value(hw, value);
}

/* REG_0x4f:ana_reg15->isel:adc bias trimming,W,0x4f[1:0]*/
void sys_hal_set_ana_reg15_isel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg15_isel(hw, value);
}

/* REG_0x4f:ana_reg15->micdcocdin:adc micmode dcoc din,W,0x4f[9:2]*/
void sys_hal_set_ana_reg15_micdcocdin(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg15_micdcocdin(hw, value);
}

/* REG_0x4f:ana_reg15->micdcocvc:adc micmode dcoc control,W,0x4f[11:10]*/
void sys_hal_set_ana_reg15_micdcocvc(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg15_micdcocvc(hw, value);
}

/* REG_0x4f:ana_reg15->micdcocen_n:adc micmode dcoc enable,W,0x4f[12]*/
void sys_hal_set_ana_reg15_micdcocen_n(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg15_micdcocen_n(hw, value);
}

/* REG_0x4f:ana_reg15->micdcocen_p:adc micmode dcoc enable,W,0x4f[13]*/
void sys_hal_set_ana_reg15_micdcocen_p(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg15_micdcocen_p(hw, value);
}

/* REG_0x4f:ana_reg15->micsingleEn:adc micmode, single_end enable,W,0x4f[14]*/
void sys_hal_set_ana_reg15_micsingleen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg15_micsingleen(hw, value);
}

/* REG_0x4f:ana_reg15->micGain:adc micmode gain, 0=0dB(17.9K), F=24dB(267.6K), 2dB/step,W,0x4f[18:15]*/
void sys_hal_set_ana_reg15_micgain(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg15_micgain(hw, value);
}

/* REG_0x4f:ana_reg15->micdacen:adc micmode micdac enable,W,0x4f[19]*/
void sys_hal_set_ana_reg15_micdacen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg15_micdacen(hw, value);
}

/* REG_0x4f:ana_reg15->micdaciH:adc micmode, micdac input ,W,0x4f[27:20]*/
void sys_hal_set_ana_reg15_micdacih(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg15_micdacih(hw, value);
}

/* REG_0x4f:ana_reg15->micdacit:adc micmode, mic_dac Iout Full-range, 00=280uA, 01=320uA, 1X=447uA,W,0x4f[29:28]*/
void sys_hal_set_ana_reg15_micdacit(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg15_micdacit(hw, value);
}

/* REG_0x4f:ana_reg15->hcen:adc 1stg op current trimming,W,0x4f[30]*/
void sys_hal_set_ana_reg15_hcen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg15_hcen(hw, value);
}

/* REG_0x4f:ana_reg15->micEn:mic2 mode enable,W,0x4f[31]*/
void sys_hal_set_ana_reg15_micen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg15_micen(hw, value);
}

/* REG_0x50 */

void sys_hal_set_ana_reg16_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_value(hw, value);
}

/* REG_0x50:ana_reg16->hpdac:class ab driver high current mode. "1" high current. ,W,0x50[0]*/
void sys_hal_set_ana_reg16_hpdac(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_hpdac(hw, value);
}

/* REG_0x50:ana_reg16->vcmsdac:1stg OP input common model voltage selection. "1" low common mode voltage,W,0x50[1]*/
void sys_hal_set_ana_reg16_vcmsdac(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_vcmsdac(hw, value);
}

/* REG_0x50:ana_reg16->oscdac:threshold current setting for over current protection . "3" maximum current. "0" minimum current,W,0x50[3:2]*/
void sys_hal_set_ana_reg16_oscdac(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_oscdac(hw, value);
}

/* REG_0x50:ana_reg16->ocendac:over current protection enable. "1" enable.,W,0x50[4]*/
void sys_hal_set_ana_reg16_ocendac(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_ocendac(hw, value);
}

/* REG_0x50:ana_reg16->isel_idac:idac current sel,W,0x50[5]*/
void sys_hal_set_ana_reg16_isel_idac(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_isel_idac(hw, value);
}

/* REG_0x50:ana_reg16->adjdacref:audio dac reference voltage adjust.,W,0x50[10:6]*/
void sys_hal_set_ana_reg16_adjdacref(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_adjdacref(hw, value);
}

/* REG_0x50:ana_reg16->dcochg:dcoc high gain selection. "1" high gain,W,0x50[12]*/
void sys_hal_set_ana_reg16_dcochg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_dcochg(hw, value);
}

/* REG_0x50:ana_reg16->diffen:enable differential mode. "1" enable,W,0x50[13]*/
void sys_hal_set_ana_reg16_diffen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_diffen(hw, value);
}

/* REG_0x50:ana_reg16->endaccal:enable offset calibration process. "1" enable.,W,0x50[14]*/
void sys_hal_set_ana_reg16_endaccal(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_endaccal(hw, value);
}

/* REG_0x50:ana_reg16->rendcoc:R-channel dcoc dac enablel. "1" enable,W,0x50[15]*/
void sys_hal_set_ana_reg16_rendcoc(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_rendcoc(hw, value);
}

/* REG_0x50:ana_reg16->lendcoc:L-channel Dcoc dac enable. "1" enable,W,0x50[16]*/
void sys_hal_set_ana_reg16_lendcoc(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_lendcoc(hw, value);
}

/* REG_0x50:ana_reg16->renvcmd:R-channel common mode output buffer enable."1" enable,W,0x50[17]*/
void sys_hal_set_ana_reg16_renvcmd(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_renvcmd(hw, value);
}

/* REG_0x50:ana_reg16->lenvcmd:L-channel common mode output buffer enable. "1" enable,W,0x50[18]*/
void sys_hal_set_ana_reg16_lenvcmd(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_lenvcmd(hw, value);
}

/* REG_0x50:ana_reg16->dacdrven:dac output driver enable."1" enable,W,0x50[19]*/
void sys_hal_set_ana_reg16_dacdrven(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_dacdrven(hw, value);
}

/* REG_0x50:ana_reg16->dacRen:dac R-channel enable. "1"  enable,W,0x50[20]*/
void sys_hal_set_ana_reg16_dacren(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_dacren(hw, value);
}

/* REG_0x50:ana_reg16->dacLen:dac L-channel enable. "1" enable,W,0x50[21]*/
void sys_hal_set_ana_reg16_daclen(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_daclen(hw, value);
}

/* REG_0x50:ana_reg16->dacG:dac gain setting: 000=0dB, 111=8dB,W,0x50[24:22]*/
void sys_hal_set_ana_reg16_dacg(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_dacg(hw, value);
}

/* REG_0x50:ana_reg16->ck4xsel:dac clock sel ,W,0x50[25]*/
void sys_hal_set_ana_reg16_ck4xsel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_ck4xsel(hw, value);
}

/* REG_0x50:ana_reg16->dacmute:dac mute enable. "1" mute enable,W,0x50[26]*/
void sys_hal_set_ana_reg16_dacmute(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_dacmute(hw, value);
}

/* REG_0x50:ana_reg16->dwamode:dac dwa mode sel,W,0x50[27]*/
void sys_hal_set_ana_reg16_dwamode(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_dwamode(hw, value);
}

/* REG_0x50:ana_reg16->ckposel:dac sample clock edge selection,W,0x50[28]*/
void sys_hal_set_ana_reg16_ckposel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_ckposel(hw, value);
}

/* REG_0x50:ana_reg16->byldo:bypass 1v8 LDO,W,0x50[31]*/
void sys_hal_set_ana_reg16_byldo(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg16_byldo(hw, value);
}

/* REG_0x51 */

void sys_hal_set_ana_reg17_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg17_value(hw, value);
}

/* REG_0x51:ana_reg17->lmdcin:l-cnannel offset cancel dac maumual input.,W,0x51[7:0]*/
void sys_hal_set_ana_reg17_lmdcin(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg17_lmdcin(hw, value);
}

/* REG_0x51:ana_reg17->rmdcin:r-channel offset cancel dac manmual input ,W,0x51[15:8]*/
void sys_hal_set_ana_reg17_rmdcin(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg17_rmdcin(hw, value);
}

/* REG_0x51:ana_reg17->spirst_ovc:ovc rst,W,0x51[16]*/
void sys_hal_set_ana_reg17_spirst_ovc(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg17_spirst_ovc(hw, value);
}

/* REG_0x51:ana_reg17->hc2s0v9:0=current is half,W,0x51[20]*/
void sys_hal_set_ana_reg17_hc2s0v9(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg17_hc2s0v9(hw, value);
}

/* REG_0x51:ana_reg17->lvcmsel:low vcm sel,W,0x51[21]*/
void sys_hal_set_ana_reg17_lvcmsel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg17_lvcmsel(hw, value);
}

/* REG_0x51:ana_reg17->loop2sel:2rd loop sel,W,0x51[22]*/
void sys_hal_set_ana_reg17_loop2sel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg17_loop2sel(hw, value);
}

/* REG_0x51:ana_reg17->enbias:dac bias enable,W,0x51[23]*/
void sys_hal_set_ana_reg17_enbias(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg17_enbias(hw, value);
}

/* REG_0x51:ana_reg17->calck_sel0v9:offset calibration clock selection. "1" high clock.,W,0x51[24]*/
void sys_hal_set_ana_reg17_calck_sel0v9(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg17_calck_sel0v9(hw, value);
}

/* REG_0x51:ana_reg17->bpdwa0v9:bypss audio dac dwa. "1" bypass.,W,0x51[25]*/
void sys_hal_set_ana_reg17_bpdwa0v9(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg17_bpdwa0v9(hw, value);
}

/* REG_0x51:ana_reg17->looprst0v9:audio dac integrator capacitor reset. "1" reset.,W,0x51[26]*/
void sys_hal_set_ana_reg17_looprst0v9(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg17_looprst0v9(hw, value);
}

/* REG_0x51:ana_reg17->oct0v9:over current delay time setting."11" maximum time. "00" minimum current.,W,0x51[28:27]*/
void sys_hal_set_ana_reg17_oct0v9(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg17_oct0v9(hw, value);
}

/* REG_0x51:ana_reg17->sout0v9:short output with 600ohm resistor. "1" short output.,W,0x51[29]*/
void sys_hal_set_ana_reg17_sout0v9(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg17_sout0v9(hw, value);
}

/* REG_0x51:ana_reg17->hc0v9:dac current trimming, 00=minimum current, 11=maximum current,W,0x51[31:30]*/
void sys_hal_set_ana_reg17_hc0v9(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg17_hc0v9(hw, value);
}

/* REG_0x52 */

void sys_hal_set_ana_reg18_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg18_value(hw, value);
}

/* REG_0x52:ana_reg18->ictrl_dsppll:26M PLL setting,W,0x52[3:0]*/
void sys_hal_set_ana_reg18_ictrl_dsppll(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg18_ictrl_dsppll(hw, value);
}

/* REG_0x52:ana_reg18->FBdivN:26M PLL setting,W,0x52[13:4]*/
void sys_hal_set_ana_reg18_fbdivn(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg18_fbdivn(hw, value);
}

/* REG_0x52:ana_reg18->N_mcudsp:26M PLL setting,W,0x52[18:14]*/
void sys_hal_set_ana_reg18_n_mcudsp(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg18_n_mcudsp(hw, value);
}

/* REG_0x52:ana_reg18->mode:26M PLL setting,W,0x52[19]*/
void sys_hal_set_ana_reg18_mode(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg18_mode(hw, value);
}

/* REG_0x52:ana_reg18->iamsel:26M PLL setting,W,0x52[20]*/
void sys_hal_set_ana_reg18_iamsel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg18_iamsel(hw, value);
}

/* REG_0x52:ana_reg18->hvref:26M PLL setting,W,0x52[22:21]*/
void sys_hal_set_ana_reg18_hvref(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg18_hvref(hw, value);
}

/* REG_0x52:ana_reg18->lvref:26M PLL setting,W,0x52[24:23]*/
void sys_hal_set_ana_reg18_lvref(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg18_lvref(hw, value);
}

/* REG_0x53 */

void sys_hal_set_ana_reg19_value(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg19_value(hw, value);
}

/* REG_0x53:ana_reg19->amsel:26M PLL setting,W,0x53[0]*/
void sys_hal_set_ana_reg19_amsel(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg19_amsel(hw, value);
}

/* REG_0x53:ana_reg19->msw:26M PLL setting,W,0x53[9:1]*/
void sys_hal_set_ana_reg19_msw(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg19_msw(hw, value);
}

/* REG_0x53:ana_reg19->tstcken_dpll:26M PLL setting,W,0x53[10]*/
void sys_hal_set_ana_reg19_tstcken_dpll(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg19_tstcken_dpll(hw, value);
}

/* REG_0x53:ana_reg19->osccal_trig:26M PLL setting,W,0x53[11]*/
void sys_hal_set_ana_reg19_osccal_trig(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg19_osccal_trig(hw, value);
}

/* REG_0x53:ana_reg19->cnti:26M PLL setting,W,0x53[20:12]*/
void sys_hal_set_ana_reg19_cnti(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg19_cnti(hw, value);
}

/* REG_0x53:ana_reg19->spi_rst:26M PLL setting,W,0x53[22]*/
void sys_hal_set_ana_reg19_spi_rst(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg19_spi_rst(hw, value);
}

/* REG_0x53:ana_reg19->closeloop_en:26M PLL setting,W,0x53[23]*/
void sys_hal_set_ana_reg19_closeloop_en(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg19_closeloop_en(hw, value);
}

/* REG_0x53:ana_reg19->caltime:26M PLL setting,W,0x53[24]*/
void sys_hal_set_ana_reg19_caltime(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg19_caltime(hw, value);
}

/* REG_0x53:ana_reg19->LPFRz:26M PLL setting,W,0x53[26:25]*/
void sys_hal_set_ana_reg19_lpfrz(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg19_lpfrz(hw, value);
}

/* REG_0x53:ana_reg19->ICP:26M PLL setting,W,0x53[30:27]*/
void sys_hal_set_ana_reg19_icp(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg19_icp(hw, value);
}

/* REG_0x53:ana_reg19->CP2ctrl:26M PLL setting,W,0x53[31]*/
void sys_hal_set_ana_reg19_cp2ctrl(sys_hw_t *hw, uint32_t value)
{
    sys_ll_set_ana_reg19_cp2ctrl(hw, value);
}


#endif

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

void sys_hal_set_ana_cb_cal_manu_val(uint32_t value)
{
    sys_ll_set_ana_reg4_cb_manu_val(value);
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

void sys_hal_set_ana_vtempsel(uint32_t value)
{
    sys_ll_set_ana_reg9_vtempsel(value);
}



