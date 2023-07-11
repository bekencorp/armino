// Copyright 2022-2023 Beken
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
#include "bk_arch.h"
#include "hal_port.h"
#include <os/os.h>
#include "sys_pm_hal.h"
#include "sys_pm_hal_ctrl.h"

static inline void sys_hal_backup_disable_int(uint32_t *int_state1, uint32_t *int_state2)
{
        *int_state1 = sys_ll_get_cpu0_int_0_31_en_value();
        *int_state2 = sys_ll_get_cpu0_int_32_63_en_value();
	PM_HAL_LOGD("backup int config: int0=%x, int1=%x\r\n", *int_state1, *int_state2);

        sys_ll_set_cpu0_int_0_31_en_value(*int_state1 & WS_CPU0_INT_0_31_MASK);
        sys_ll_set_cpu0_int_32_63_en_value(*int_state2 & WS_CPU0_INT_32_64_MASK);
	PM_HAL_LOGD("disable all except ws: int0=%x, int1=%x\r\n",
		sys_ll_get_cpu0_int_0_31_en_value(), sys_ll_get_cpu0_int_32_63_en_value());
}

static inline void sys_hal_restore_int(uint32_t int_state1, uint32_t int_state2)
{
	PM_HAL_LOGD("restore int: int0=%x, int1=%x\r\n", int_state1, int_state2);
        sys_ll_set_cpu0_int_0_31_en_value(int_state1);
        sys_ll_set_cpu0_int_32_63_en_value(int_state2);
}

static inline void sys_hal_enable_wakeup_int(void)
{
}

static inline void sys_hal_set_core_freq(uint8_t cksel_core, uint8_t clkdiv_core, uint8_t clkdiv_bus)
{
	PM_HAL_LOGD("set core freq, cksel_core=%u, clkdiv_core=%u, clkdiv_bus=%u\r\n",
		cksel_core, clkdiv_core, clkdiv_bus);
        sys_ll_set_cpu_clk_div_mode1_cksel_core(cksel_core);
        sys_ll_set_cpu_clk_div_mode1_clkdiv_core(clkdiv_core);
        sys_ll_set_cpu_clk_div_mode1_clkdiv_bus(clkdiv_bus);
}

static inline void sys_hal_set_core_26m(void)
{
	PM_HAL_LOGD("set core to 26M\r\n");
	sys_hal_set_core_freq(0, 0, 0);
}

static inline void sys_hal_backup_set_core_26m(uint8_t *cksel_core, uint8_t *clkdiv_core, uint8_t *clkdiv_bus)
{

	IF_LV_CTRL_CORE() {
		*cksel_core = sys_ll_get_cpu_clk_div_mode1_cksel_core();
	        *clkdiv_core = sys_ll_get_cpu_clk_div_mode1_clkdiv_core();
	        *clkdiv_bus = sys_ll_get_cpu_clk_div_mode1_clkdiv_bus();
		PM_HAL_LOGD("backup core config: cksel_core=%u, clkdiv_core=%u, clkdiv_bus=%u\r\n",
			*cksel_core, *clkdiv_core, *clkdiv_bus);
		sys_hal_set_core_freq(0, 0, 0);
	}
}

static inline void sys_hal_restore_core_freq(uint8_t cksel_core, uint8_t clkdiv_core, uint8_t clkdiv_bus)
{
	IF_LV_CTRL_CORE() {
		PM_HAL_LOGD("restore core freq, cksel_core=%u, clkdiv_core=%u, clkdiv_bus=%u\r\n",
			cksel_core, clkdiv_core, clkdiv_bus);
        	sys_ll_set_cpu_clk_div_mode1_clkdiv_bus(clkdiv_bus);
        	sys_ll_set_cpu_clk_div_mode1_clkdiv_core(clkdiv_core);
        	sys_ll_set_cpu_clk_div_mode1_cksel_core(cksel_core);
	}
}

static inline void sys_hal_set_flash_freq(uint8_t cksel_flash, uint8_t ckdiv_flash)
{
	PM_HAL_LOGD("set flash freq: cksel_flash=%u, ckdiv_flash=%u\r\n", cksel_flash, ckdiv_flash);
	sys_ll_set_cpu_clk_div_mode2_cksel_flash(cksel_flash);
	sys_ll_set_cpu_clk_div_mode2_ckdiv_flash(ckdiv_flash);
}

static inline void sys_hal_set_flash_26m(void)
{
	PM_HAL_LOGD("set flash to 26M\r\n");
	sys_hal_set_flash_freq(0, 0);
}

static inline void sys_hal_backup_set_flash_26m(uint8_t *cksel_flash, uint8_t *ckdiv_flash)
{
	IF_LV_CTRL_FLASH() {
		*cksel_flash = sys_ll_get_cpu_clk_div_mode2_cksel_flash();
		*ckdiv_flash = sys_ll_get_cpu_clk_div_mode2_ckdiv_flash();
		PM_HAL_LOGD("back up flash config: cksel_flash=%u, ckdiv_flash=%u\r\n", *cksel_flash, *ckdiv_flash);
		sys_hal_set_flash_freq(0, 0);
	}
}

static inline void sys_hal_set_flash_120m(void)
{
	PM_HAL_LOGD("set flash to 120M\r\n");
	sys_ll_set_cpu_clk_div_mode2_cksel_flash(2);
}

static inline void sys_hal_set_anaspi_freq(uint8_t anaspi_freq)
{
	PM_HAL_LOGD("set anaspi freq: %d\r\n", anaspi_freq);
	sys_ll_set_cpu_anaspi_freq_value(anaspi_freq);
}

static inline void sys_hal_backup_set_anaspi_freq_26m(uint8_t *anaspi_freq)
{
	*anaspi_freq = sys_ll_get_cpu_anaspi_freq_value();
	PM_HAL_LOGD("backup anaspi freq: %d\r\n", *anaspi_freq);
	sys_hal_set_anaspi_freq(1); //26M/4
}

static inline void sys_hal_restore_anaspi_freq(uint8_t anaspi_freq)
{
	sys_hal_set_anaspi_freq(anaspi_freq);
}

static inline void sys_hal_restore_flash_freq(uint8_t cksel_flash, uint8_t ckdiv_flash)
{
	IF_LV_CTRL_FLASH() {
		sys_hal_set_flash_freq(cksel_flash, ckdiv_flash);
	}
}
	
static inline void sys_hal_mask_cpu0_int(void)
{
	PM_HAL_LOGD("mask cpu0 int\r\n");
	sys_ll_set_cpu0_int_halt_clk_op_cpu0_int_mask(1);
}

static inline void sys_hal_enable_spi_latch(void)
{
	PM_HAL_LOGD("enable spi latch\r\n");
	sys_ll_set_ana_reg9_spi_latch1v(1);
}

static inline void sys_hal_disable_spi_latch(void)
{
	PM_HAL_LOGD("disable spi latch\r\n");
	sys_ll_set_ana_reg9_spi_latch1v(0);
}

static inline uint32_t sys_hal_disable_hf_clock(void)
{
	IF_LV_CTRL_HF() {
		uint32_t val = sys_ll_get_ana_reg5_value();
		uint32_t ret_val = val;
	
		//TODO need to optimize it
		val &= ~(BIT(1) | BIT(2) | BIT(3) | BIT(5));
		sys_ll_set_ana_reg5_value(val);
		sys_ll_set_ana_reg5_en_cb(0);
		PM_HAL_LOGD("disable hf clock: %x\r\n", ret_val);
		return ret_val;
	}
	return BK_OK;
}

static inline void sys_hal_restore_hf_clock(uint32_t val)
{
	sys_ll_set_ana_reg5_en_cb(1);
	uint32_t v = sys_ll_get_ana_reg5_value();
	v |= (BIT(1) | BIT(2) | BIT(3) | BIT(5));
	sys_ll_set_ana_reg5_value(v);
}

static inline void sys_hal_deep_sleep_set_buck(void)
{
	sys_ll_set_ana_reg11_enpowa(0);
	sys_ll_set_ana_reg13_pwdovp1v(1);
}

static inline void sys_hal_clear_wakeup_source(void)
{
	aon_pmu_ll_set_r43_clr_wakeup(1);
	aon_pmu_ll_set_r43_clr_wakeup(0);
}

static inline void sys_hal_set_lpo_src_rosc(void)
{
	//RTC clock to ROSC, TODO - use macro
	uint32_t v = aon_pmu_ll_get_r41();
	v |= 0xFD << 24;
	aon_pmu_ll_set_r41(v);

	aon_pmu_ll_set_r41_lpo_config(2);
}

static inline void sys_hal_set_power_parameter(bool is_deep_sleep)
{
	/*  r40[3:0]   wake1_delay = 0x1;
 	 *  r40[7:4]   wake2_delay = 0x1; 	
 	 *  r40[11:8]  wake3_delay = 0x1; 	
 	 *  r40[15:12] halt1_delay = 0x1;
 	 *  r40[19:16] halt2_delay = 0x1;
 	 *  r40[23:20] halt3_delay = 0x1;
 	 *  r40[24] halt_volt: deep = 0, lv = 1
 	 *  r40[25] halt_xtal = 1
 	 *  r40[26] halt_core: deep = 1, lv = 0
 	 *  r40[27] halt_flash = 1
 	 *  r40[28] halt_rosc = 0
 	 *  r40[29] halt_resten: deep = 0, lv = 1
 	 *  r40[30] halt_isolat = 1
 	 *  r40[31] halt_clkena = 0
 	 **/
	if (is_deep_sleep) {
		aon_pmu_ll_set_r40(0x4E111111);
	} else {
		aon_pmu_ll_set_r40(0x6B111111);
	}
}

static inline void sys_hal_set_sleep_condition(void)
{
	uint32_t v = sys_ll_get_cpu_power_sleep_wakeup_value();

	//sleep_en_global = 1
	//sleep_en_need_cpu0_wfi = 0
	//sleep_en_need_cpu1_wfi = 1
	//sleep_en_need_flash_idle = 0
	v |= (0xa << 16);
	sys_ll_set_cpu_power_sleep_wakeup_value(v);
}

static inline void sys_hal_power_down_pd(uint32_t *pd_reg_v)
{
	IF_LV_CTRL_PD() {
		uint32_t v = sys_ll_get_cpu_power_sleep_wakeup_value();

		PM_HAL_LOGD("power down pd\r\n");
		*pd_reg_v = v;
		//v |= 0x0200f7fc; //mem0/1/2 on, others off
		v |= 0xa7f8; //mem0/1/2 on, others off
		sys_ll_set_cpu_power_sleep_wakeup_value(v);
	
		PM_HAL_LOGD("power down cpu0 sub-pd\r\n");
		sys_ll_set_cpu_power_sleep_wakeup_cpu0_subpwdm_en(1);
	
		PM_HAL_LOGD("enable isolation\r\n");
		v = aon_pmu_ll_get_r41();
		v |= 0x55 << 24;
		aon_pmu_ll_set_r41(v);
	
		PM_HAL_LOGD("power down cpu1\r\n");
		sys_ll_set_cpu1_int_halt_clk_op_cpu1_pwr_dw(1);
	}
}

static inline void sys_hal_power_on_pd(uint32_t v_sys_r10)
{
	IF_LV_CTRL_PD() {
		PM_HAL_LOGD("power on pd\r\n");
		sys_ll_set_cpu_power_sleep_wakeup_value(v_sys_r10);
	}
}

static inline void sys_hal_set_wakeup_source(void)
{
	aon_pmu_ll_set_r41_wakeup_ena(0x3f);
}

static inline void sys_hal_clear_wakeup_status(void)
{
	aon_pmu_ll_set_r43_clr_wakeup(1);
}

void sys_hal_enter_deep_sleep(void *param)
{
	sys_hal_mask_cpu0_int();
	sys_hal_set_core_26m();
	sys_hal_set_flash_26m();
	sys_hal_enable_spi_latch();
	sys_hal_disable_hf_clock();
	sys_hal_deep_sleep_set_buck();
	sys_hal_disable_spi_latch();
	sys_hal_set_lpo_src_rosc();
	sys_hal_set_power_parameter(true);
	sys_hal_clear_wakeup_status();
	sys_hal_set_sleep_condition();
	arch_deep_sleep();
}

static inline void sys_hal_set_low_voltage(uint32_t *ana_r8, uint32_t *core_low_voltage)
{
	sys_hal_enable_spi_latch();

	*ana_r8 = sys_ll_get_ana_reg8_value();
	*core_low_voltage = sys_ll_get_ana_reg9_vcorelsel();
	PM_HAL_LOGD("backup ana r8=%x, vcoresel=%x\r\n", *ana_r8, *core_low_voltage);

	PM_HAL_LOGD("set always on to 0.8V\r\n");
	sys_ll_set_ana_reg8_valoldosel(4); //0.8V

	PM_HAL_LOGD("set vcore to 0.55v\r\n");
	sys_ll_set_ana_reg9_vcorelsel(4); //0.55V, TODO 4->3
	sys_ll_set_ana_reg9_vlden(1);
	sys_ll_set_ana_reg9_vdd12lden(1);

	PM_HAL_LOGD("enable LDO self-lp\r\n");
	sys_ll_set_ana_reg8_ioldo_lp(1);
	sys_ll_set_ana_reg8_coreldo_hp(0);
	sys_ll_set_ana_reg8_dldohp(0);
	sys_ll_set_ana_reg8_aldohp(0);
	sys_ll_set_ana_reg8_aloldohp(0)
;
	sys_hal_disable_spi_latch();
}

static inline void sys_hal_restore_voltage(uint32_t ana_r8, uint32_t core_low_voltage)
{
	PM_HAL_LOGD("restore AON voltage to 0.95V\r\n");
	sys_hal_enable_spi_latch();
	sys_ll_set_ana_reg8_value(ana_r8);

	sys_ll_set_ana_reg9_vcorelsel(core_low_voltage & 0x7);
	sys_ll_set_ana_reg9_vlden(0);
	sys_ll_set_ana_reg9_vdd12lden(0);

	sys_hal_disable_spi_latch();
}

static void sys_hal_delay(volatile uint32_t times)
{
        while(times--);
}

void sys_hal_exit_low_voltage(void)
{
}

static inline void sys_hal_lv_set_buck(void)
{
	if (sys_ll_get_ana_reg11_aldosel() == 0) {
		PM_HAL_LOGD("set buck clock source to ROSC\r\n");
		sys_ll_set_ana_reg10_ckintsel(1);
	}
}

static inline void sys_hal_lv_restore_buck(void)
{
	if (sys_ll_get_ana_reg11_aldosel() == 0) {
		PM_HAL_LOGD("set buck clock source to divider\r\n");
		sys_ll_set_ana_reg10_ckintsel(0);
	}
}

__attribute__((section(".iram"))) void sys_hal_enter_low_voltage(void)
{
	uint32_t int_state1, int_state2;
	uint8_t cksel_core = 0, clkdiv_core = 0, clkdiv_bus = 0;
	uint8_t cksel_flash = 0, clkdiv_flash = 0;
	uint32_t v_ana_r8, core_low_voltage;
	uint32_t v_sys_r10 = 0;

	sys_hal_debug_gpio_up(LV_DEBUG_GPIO);
	sys_hal_debug_get_sys_regs_before_sleep();
	sys_hal_mask_cpu0_int();

	sys_hal_set_lpo_src_rosc();

	sys_hal_backup_disable_int(&int_state1, &int_state2);
	sys_hal_backup_set_core_26m(&cksel_core, &clkdiv_core, &clkdiv_bus);
	sys_hal_backup_set_flash_26m(&cksel_flash, &clkdiv_flash);

	uint32_t hf_reg_v = sys_hal_disable_hf_clock();

	sys_hal_lv_set_buck();
	sys_hal_set_low_voltage(&v_ana_r8, &core_low_voltage);
	sys_hal_clear_wakeup_status();
	sys_hal_set_power_parameter(false);
	sys_hal_set_sleep_condition();
	sys_hal_power_down_pd(&v_sys_r10);
	sys_hal_debug_gpio_down(LV_DEBUG_GPIO);

	arch_deep_sleep();

	sys_hal_debug_gpio_up(LV_DEBUG_GPIO);

	sys_hal_power_on_pd(v_sys_r10);
	sys_hal_restore_voltage(v_ana_r8, core_low_voltage);
	sys_hal_lv_restore_buck();
	sys_hal_restore_hf_clock(hf_reg_v);

	sys_hal_restore_flash_freq(cksel_flash, clkdiv_flash);
	sys_hal_restore_core_freq(cksel_core, clkdiv_core, clkdiv_bus);
	sys_hal_restore_int(int_state1, int_state2);
	sys_hal_debug_get_sys_regs_after_waked();
	sys_hal_debug_check_sys_regs();
	sys_hal_debug_gpio_down(LV_DEBUG_GPIO);
	PM_HAL_LOGD("waked\r\n");
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
	// TODO - find out the reason:
	// When sys_ll_set_cpu0_int_halt_clk_op_cpu0_int_mask() is called, the normal sleep can't wakedup,
	// need to find out!!!
	// sys_ll_set_cpu0_int_halt_clk_op_cpu0_halt(1);
	arch_sleep();
}

void sys_hal_enter_normal_wakeup()
{
}

void sys_hal_enable_mac_wakeup_source()
{
}

void sys_hal_enable_bt_wakeup_source()
{
}

void sys_hal_wakeup_interrupt_clear(wakeup_source_t interrupt_source)
{

}

void sys_hal_arm_wakeup_enable(uint32_t param)
{
}

void sys_hal_arm_wakeup_disable(uint32_t param)
{
}

uint32_t sys_hal_get_arm_wakeup(void)
{
	return 0;
}

#define BIT_AON_PMU_WAKEUP_ENA      (0x1F0U)
void sys_hal_low_power_hardware_init()
{
	uint32_t param = 0;
	uint32_t  pmu_state = 0;

	param = aon_pmu_hal_reg_get(PMU_REG0);
	param = 0x1; //memcheck bypass
	aon_pmu_hal_reg_set(PMU_REG0,param);

	param = 0;
	param = sys_ll_get_ana_reg5_value();
	//param |= (1 << SYS_ANA_REG5_ENCB_POS);//global central bias enable
    sys_ll_set_ana_reg5_value(param);

	param = 0;
	param = sys_ll_get_ana_reg6_value();
	//param &= ~((0x1 << SYS_ANA_REG6_EN_SLEEP_POS)|(1 << SYS_ANA_REG6_XTAL_LPMODE_CTRL_POS));
	sys_ll_set_ana_reg6_value(param);

	param = 0;
	param = sys_ll_get_ana_reg6_value();
	//param |= ((0x7 << SYS_ANA_REG6_RXTAL_LP_POS)|(0x7 << SYS_ANA_REG6_RXTAL_HP_POS));
	sys_ll_set_ana_reg6_value(param);

	/*set wakeup source*/
	pmu_state =  aon_pmu_hal_reg_get(PMU_REG0x41);
	pmu_state |= BIT_AON_PMU_WAKEUP_ENA;
	aon_pmu_hal_reg_set(PMU_REG0x41,pmu_state);

}
