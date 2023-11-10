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
#include "modules/pm.h"

#if CONFIG_CACHE_ENABLE
#include "cache.h"
#endif
extern void delay_us(UINT32 us);
static inline bool is_lpo_src_26m32k(void)
{
	return (aon_pmu_ll_get_r41_lpo_config() == SYS_LPO_SRC_26M32K);
}

static inline bool is_lpo_src_ext32k(void)
{
	return (aon_pmu_ll_get_r41_lpo_config() == SYS_LPO_SRC_EXTERNAL_32K);
}

static inline bool is_wifi_ws_enabled(uint8_t ena_bits)
{
	return !!(ena_bits & WS_WIFI);
}

static inline bool is_gpio_ws_enabled(uint8_t ena_bits)
{
	return !!(ena_bits & WS_GPIO);
}

static inline bool is_rtc_ws_enabled(uint8_t ena_bits)
{
	return !!(ena_bits & WS_RTC);
}

static inline bool is_bt_ws_enabled(uint8_t ena_bits)
{
	return !!(ena_bits & WS_BT);
}

static inline bool is_usbplug_ws_enabled(uint8_t ena_bits)
{
	return !!(ena_bits & WS_USBPLUG);
}

static inline bool is_touch_ws_enabled(uint8_t ena_bits)
{
	return !!(ena_bits & WS_TOUCH);
}

static inline bool is_wifi_pd_poweron(uint8_t pd_bits)
{
	return !(pd_bits & PD_WIFI);
}

static inline bool is_btsp_pd_poweron(uint8_t pd_bits)
{
	return !(pd_bits & PD_BTSP);
}

static inline void sys_hal_backup_disable_int(volatile uint32_t *int_state1, volatile uint32_t *int_state2)
{
//	*int_state1 = sys_ll_get_cpu0_int_0_31_en_value();
//	*int_state2 = sys_ll_get_cpu0_int_32_63_en_value();
	PM_HAL_LOGD("backup int config: int0=%x, int1=%x\r\n", *int_state1, *int_state2);

	uint8_t ws_ena = aon_pmu_ll_get_r41_wakeup_ena();
	uint32_t int0_enabled_ws_ints = 0;
	uint32_t int1_enabled_ws_ints = 0;

	PM_HAL_LOGD("enabled wakesource: %x\r\n", ws_ena);

	if (is_wifi_ws_enabled(ws_ena)) {
		int0_enabled_ws_ints |= WS_WIFI_INT0;
		int1_enabled_ws_ints |= WS_WIFI_INT1;
	}

	if (is_gpio_ws_enabled(ws_ena)) {
		int1_enabled_ws_ints |= WS_GPIO_INT;
	}

	if (is_rtc_ws_enabled(ws_ena)) {
		int1_enabled_ws_ints |= WS_RTC_INT;
	}

	if (is_bt_ws_enabled(ws_ena)) {
		int1_enabled_ws_ints |= WS_BT_INT;
	}

	if (is_usbplug_ws_enabled(ws_ena)) {
		int1_enabled_ws_ints |= WS_USBPLUG_INT;
	}

	if (is_touch_ws_enabled(ws_ena)) {
		int1_enabled_ws_ints |= WS_TOUCH_INT;
	}

        sys_ll_set_cpu0_int_0_31_en_value(*int_state1 & int0_enabled_ws_ints);
        sys_ll_set_cpu0_int_32_63_en_value(*int_state2 & int1_enabled_ws_ints);
	PM_HAL_LOGD("disable all except ws: int0=%x, int1=%x\r\n",
		sys_ll_get_cpu0_int_0_31_en_value(), sys_ll_get_cpu0_int_32_63_en_value());
}

static inline void sys_hal_restore_int(volatile uint32_t int_state1, volatile uint32_t int_state2)
{
	PM_HAL_LOGD("restore int: int0=%x, int1=%x\r\n", int_state1, int_state2);
        sys_ll_set_cpu0_int_0_31_en_value(int_state1);
        sys_ll_set_cpu0_int_32_63_en_value(int_state2);
}

static inline void sys_hal_enable_wakeup_int(void)
{
}

static inline void sys_hal_set_core_freq(volatile uint8_t cksel_core, volatile uint8_t clkdiv_core, volatile uint8_t clkdiv_bus)
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

static inline void sys_hal_backup_set_core_26m(volatile uint8_t *cksel_core, volatile uint8_t *clkdiv_core, volatile uint8_t *clkdiv_bus)
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

static inline void sys_hal_restore_core_freq(volatile uint8_t cksel_core, volatile uint8_t clkdiv_core, volatile uint8_t clkdiv_bus)
{
	IF_LV_CTRL_CORE() {
		PM_HAL_LOGD("restore core freq, cksel_core=%u, clkdiv_core=%u, clkdiv_bus=%u\r\n",
			cksel_core, clkdiv_core, clkdiv_bus);
        	sys_ll_set_cpu_clk_div_mode1_clkdiv_bus(clkdiv_bus);
        	sys_ll_set_cpu_clk_div_mode1_clkdiv_core(clkdiv_core);
        	sys_ll_set_cpu_clk_div_mode1_cksel_core(cksel_core);
	}
}

static inline void sys_hal_set_flash_freq(volatile uint8_t cksel_flash, volatile uint8_t ckdiv_flash)
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

static inline void sys_hal_backup_set_flash_26m(volatile uint8_t *cksel_flash, volatile  uint8_t *ckdiv_flash)
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

static inline void sys_hal_restore_flash_freq(volatile uint8_t cksel_flash, volatile uint8_t ckdiv_flash)
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

		val &= ~EN_ALL;
		if (is_lpo_src_ext32k()) {
			PM_HAL_LOGD("not disable 32K xtal clock\r\n");
			val |= EN_XTAL;
		}
		sys_ll_set_ana_reg5_value(val);
		sys_ll_set_ana_reg5_en_cb(0);
		PM_HAL_LOGD("disable hf clock, old=%x, current=%x\r\n", ret_val, val);
		return ret_val;
	}
	return BK_OK;
}

static inline void sys_hal_restore_hf_clock(volatile uint32_t val)
{
	sys_ll_set_ana_reg5_en_cb(1);
	sys_ll_set_ana_reg5_value(val);
}

static inline void sys_hal_deep_sleep_set_buck(void)
{
	sys_ll_set_ana_reg11_aldosel(1);
	sys_ll_set_ana_reg12_dldosel(1);
	// sys_ll_set_ana_reg11_enpowa(0); //20230423 tenglong
	sys_ll_set_ana_reg13_pwdovp1v(1);
}

static inline void sys_hal_deep_sleep_set_vldo(void)
{
	sys_ll_set_ana_reg8_valoldosel(0x4); //0.8V
	sys_ll_set_ana_reg8_violdosel(0x0); //2.9V
	sys_ll_set_ana_reg8_aldohp(0); //20230423 tenglong
	sys_ll_set_ana_reg8_bgcal(0x2f);
}

static inline void sys_hal_clear_wakeup_source(void)
{
	aon_pmu_ll_set_r43_clr_wakeup(1);
	aon_pmu_ll_set_r43_clr_wakeup(0);
}

static inline void sys_hal_set_halt_config(void)
{
	uint32_t v = aon_pmu_ll_get_r41();

	//halt_lpo = 1
	//halt_busrst = 0
	//halt_busiso = 1, halt_buspwd = 1
	//halt_blpiso = 1, halt_blppwd = 1
	//halt_wlpiso = 1, halt_wlppwd = 1
	v |= (0xFD << 24);
	aon_pmu_ll_set_r41(v);
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
 	 *  r40[25] halt_xtal = 1 //If LPO is 26M32K, halt_xtal = 0
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
		if (is_lpo_src_26m32k()) {
			aon_pmu_ll_set_r40(0x69111111); //TODO halt_rosc???
		} else {
			aon_pmu_ll_set_r40(0x6B111111);
		}
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

static inline void sys_hal_power_down_pd(volatile uint32_t *pd_reg_v)
{
	IF_LV_CTRL_PD() {
		uint32_t v = sys_ll_get_cpu_power_sleep_wakeup_value();
		*pd_reg_v = v;

		v |= PD_DOWN_DOMAIN;
		sys_hal_set_halt_config();

		if (is_wifi_pd_poweron(v)) {
			PM_HAL_LOGD("enable WiFi isolation\r\n");
			aon_pmu_ll_set_r41_halt_wlpiso(1);
			aon_pmu_ll_set_r41_halt_wlppwd(0);
		}
		else {
			aon_pmu_ll_set_r41_halt_wlpiso(1);
			aon_pmu_ll_set_r41_halt_wlppwd(1);
		}

		if (is_btsp_pd_poweron(v)) {
			PM_HAL_LOGD("enable BT isolation\r\n");
			aon_pmu_ll_set_r41_halt_blpiso(1);
			aon_pmu_ll_set_r41_halt_blppwd(0);
		}
		else {
			aon_pmu_ll_set_r41_halt_blpiso(1);
			aon_pmu_ll_set_r41_halt_blppwd(1);
		}

		PM_HAL_LOGD("power down pd, new=%x, old=%x\r\n", v, *pd_reg_v);
		sys_ll_set_cpu_power_sleep_wakeup_value(v);

#if CONFIG_CPU0_SUB_POWER_DOMAIN_ENA
		PM_HAL_LOGD("power down cpu0 sub-pd\r\n");
#if CONFIG_CACHE_ENABLE
		flush_all_dcache();
		enable_dcache(0);
		invalidate_icache();
#endif
		sys_ll_set_cpu_power_sleep_wakeup_cpu0_subpwdm_en(1);
#endif

		PM_HAL_LOGD("power down cpu1\r\n");
		sys_ll_set_cpu1_int_halt_clk_op_cpu1_pwr_dw(1);
	}
}

static inline void sys_hal_power_on_pd(volatile uint32_t v_sys_r10)
{
	IF_LV_CTRL_PD() {
		PM_HAL_LOGD("power on pd\r\n");
		sys_ll_set_cpu_power_sleep_wakeup_value(v_sys_r10);

#if CONFIG_CPU0_SUB_POWER_DOMAIN_ENA
		PM_HAL_LOGD("power on cpu0 sub-pd\r\n");
		sys_ll_set_cpu_power_sleep_wakeup_cpu0_subpwdm_en(0);
#if CONFIG_CACHE_ENABLE
		enable_dcache(1);
#endif
#endif
	}
}

static inline void sys_hal_set_wakeup_source(void)
{
	aon_pmu_ll_set_r41_wakeup_ena(0x3f);
}

static inline void sys_hal_clear_wakeup_status(void)
{
	aon_pmu_ll_set_r43_clr_wakeup(1);
	aon_pmu_ll_set_r43_clr_wakeup(0);
}

__attribute__((section(".itcm_sec_code"))) void sys_hal_enter_deep_sleep(void *param)
{
	volatile uint32_t int_state1, int_state2;
    uint32_t pmu_val = 0;
	//workaround to fix that the BT wakesource cause deepsleep wakeup soon
	uint8_t wakesource_ena = aon_pmu_ll_get_r41_wakeup_ena();
	wakesource_ena &= ~BIT(WAKEUP_SOURCE_INT_BT);
	aon_pmu_ll_set_r41_wakeup_ena(wakesource_ena);

	int_state1 = sys_ll_get_cpu0_int_0_31_en_value();
	int_state2 = sys_ll_get_cpu0_int_32_63_en_value();
	sys_ll_set_cpu0_int_0_31_en_value(0x0);
	sys_ll_set_cpu0_int_32_63_en_value(0x0);
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	__asm volatile( "nop" );

	/*confirm here hasn't external interrupt*/
	if(check_IRQ_pending())
	{
		sys_ll_set_cpu0_int_0_31_en_value(int_state1);
		sys_ll_set_cpu0_int_32_63_en_value(int_state2);
		return;
	}

	sys_hal_mask_cpu0_int();
	sys_hal_set_core_26m();
	sys_hal_set_flash_26m();

	sys_hal_enable_spi_latch();
	sys_hal_disable_hf_clock();
	sys_hal_deep_sleep_set_buck();
	sys_hal_deep_sleep_set_vldo();
	sys_hal_disable_spi_latch();

	sys_hal_set_halt_config();
	sys_hal_set_power_parameter(true);
	sys_hal_clear_wakeup_status();
	sys_hal_set_sleep_condition();

	/*enable several interrupt for wakeup*/
	sys_ll_set_cpu0_int_32_63_en_cpu0_gpio_int_en(0x1);
	sys_ll_set_cpu0_int_32_63_en_cpu0_rtc_int_en(0x1);
	sys_ll_set_cpu0_int_32_63_en_cpu0_touched_int_en(0x1);

    /*set enter deep sleep mode flag*/
    pmu_val =  aon_pmu_hal_reg_get(PMU_REG2);
	pmu_val |= BIT(BIT_SLEEP_FLAG_DEEP_SLEEP);
	aon_pmu_hal_reg_set(PMU_REG2,pmu_val);
#if CONFIG_GPIO_WAKEUP_SUPPORT
	extern bk_err_t gpio_enable_interrupt_mult_for_wake(void);
	gpio_enable_interrupt_mult_for_wake();
#endif

	arch_deep_sleep();
}

static inline void sys_hal_set_low_voltage(volatile uint32_t *ana_r8, volatile uint32_t *core_low_voltage)
{
	sys_hal_enable_spi_latch();

	*ana_r8 = sys_ll_get_ana_reg8_value();
	*core_low_voltage = sys_ll_get_ana_reg9_vcorelsel();
	PM_HAL_LOGD("backup ana r8=%x, vcoresel=%x\r\n", *ana_r8, *core_low_voltage);
	sys_ll_set_ana_reg8_t_vanaldosel(0);//tenglong20230417
	sys_ll_set_ana_reg8_r_vanaldosel(0);//tenglong20230417

	PM_HAL_LOGD("set always on to 0.8V\r\n");
	sys_ll_set_ana_reg8_valoldosel(4); //0.8V

	PM_HAL_LOGD("set vcore to 0.525v\r\n");
	sys_ll_set_ana_reg9_vcorelsel(3); //0.525V
	sys_ll_set_ana_reg9_vlden(1);
	sys_ll_set_ana_reg9_vdd12lden(1);

	sys_ll_set_ana_reg8_ioldo_lp(1);
	sys_ll_set_ana_reg8_aloldohp(0);

#if CONFIG_LDO_SELF_LOW_POWER_MODE_ENA
	PM_HAL_LOGD("enable LDO self-lp\r\n");
	sys_ll_set_ana_reg8_coreldo_hp(0);
	sys_ll_set_ana_reg8_dldohp(0);
	sys_ll_set_ana_reg8_aldohp(0);//tenglong20230417 suggest to config to 1,but will make current 12uA higher in ldo mode
#endif

	sys_hal_disable_spi_latch();
}

static inline void sys_hal_restore_voltage(volatile uint32_t ana_r8, volatile uint32_t core_low_voltage)
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
		//PM_HAL_LOGD("set buck clock source to ROSC\r\n");
		//sys_ll_set_ana_reg10_ckintsel(1);//tenglong suggest zhangheng set it to 1 when power on
		PM_HAL_LOGD("forcepfm\r\n");
		sys_hal_enable_spi_latch();
		sys_ll_set_ana_reg11_aforcepfm(1);//tenglong20230417
		sys_ll_set_ana_reg12_dforcepfm(1);//tenglong20230417
		sys_hal_disable_spi_latch();

	}
}

static inline void sys_hal_lv_restore_buck(void)
{
	if (sys_ll_get_ana_reg11_aldosel() == 0) {
		//PM_HAL_LOGD("set buck clock source to divider\r\n");
		//sys_ll_set_ana_reg10_ckintsel(0);//tenglong suggest zhangheng set it to 1 when power on, keep to 1 all the time
		PM_HAL_LOGD("disable forcepfm\r\n");
		sys_hal_enable_spi_latch();
		sys_ll_set_ana_reg11_aforcepfm(0);//tenglong20230417
		sys_ll_set_ana_reg12_dforcepfm(0);//tenglong20230417
		sys_hal_disable_spi_latch();
	}
}

__attribute__((section(".itcm_sec_code"))) void sys_hal_enter_low_voltage(void)
{
	volatile uint32_t int_state1, int_state2;
	volatile uint8_t cksel_core = 0, clkdiv_core = 0, clkdiv_bus = 0;
	volatile uint8_t cksel_flash = 0, clkdiv_flash = 0;
	volatile uint32_t v_ana_r8, core_low_voltage;
	volatile uint32_t v_sys_r10 = 0;
    uint32_t pmu_val = 0;
	uint64_t  previous_tick = 0;
	uint64_t  current_tick = 0;
	uint64_t  wakeup_time = 0; __maybe_unused_var(wakeup_time);

	sys_hal_debug_gpio_up(LV_DEBUG_GPIO);
	sys_hal_debug_get_sys_regs_before_sleep();

	int_state1 = sys_ll_get_cpu0_int_0_31_en_value();
	int_state2 = sys_ll_get_cpu0_int_32_63_en_value();
	sys_ll_set_cpu0_int_0_31_en_value(0x0);
	sys_ll_set_cpu0_int_32_63_en_value(0x0);
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	if(check_IRQ_pending())
	{
		sys_ll_set_cpu0_int_0_31_en_value(int_state1);
		sys_ll_set_cpu0_int_32_63_en_value(int_state2);
		return;
	}

	sys_hal_mask_cpu0_int();

	sys_hal_backup_disable_int(&int_state1, &int_state2);
	sys_hal_backup_set_core_26m(&cksel_core, &clkdiv_core, &clkdiv_bus);
	sys_hal_backup_set_flash_26m(&cksel_flash, &clkdiv_flash);

	uint32_t hf_reg_v = sys_hal_disable_hf_clock();

	sys_hal_lv_set_buck();
	sys_hal_set_low_voltage(&v_ana_r8, &core_low_voltage);
	sys_hal_set_power_parameter(false);
	sys_hal_set_sleep_condition();
	sys_hal_power_down_pd(&v_sys_r10);
	sys_hal_debug_gpio_down(LV_DEBUG_GPIO);

    /*set enter low voltage sleep mode flag*/
    pmu_val =  aon_pmu_hal_reg_get(PMU_REG2);
	pmu_val |= BIT(BIT_SLEEP_FLAG_LOW_VOLTAGE);
	aon_pmu_hal_reg_set(PMU_REG2,pmu_val);

#if CONFIG_GPIO_WAKEUP_SUPPORT
	extern bk_err_t gpio_enable_interrupt_mult_for_wake(void);
	gpio_enable_interrupt_mult_for_wake();
#endif

	arch_deep_sleep();

	wakeup_time = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);

	sys_hal_debug_gpio_up(LV_DEBUG_GPIO);

	sys_hal_power_on_pd(v_sys_r10);
	sys_hal_restore_voltage(v_ana_r8, core_low_voltage);
	sys_hal_lv_restore_buck();
	sys_hal_restore_hf_clock(hf_reg_v);

	/* wait dpll stable,during waiting ,can do some other operation*/
	previous_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	extern uint32_t pm_wake_int_flag2;
	// pm_wake_int_flag1 = sys_hal_get_int_status();
	pm_wake_int_flag2 = sys_hal_get_int_group2_status();
	if(pm_wake_int_flag2&(WIFI_MAC_GEN_INT_BIT))
	{
#if CONFIG_WIFI_ENABLE
		extern uint64_t ps_mac_wakeup_from_lowvol;
		ps_mac_wakeup_from_lowvol = wakeup_time;
#endif
	}
	current_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	while(((current_tick - previous_tick)) < (LOW_POWER_DPLL_STABILITY_DELAY_TIME*bk_rtc_get_ms_tick_count()))
	{
		current_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	}

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
#if 0
	aon_pmu_ll_set_r1_touch_select(index);
	aon_pmu_ll_set_r1_touch_int_en(BIT(index));
	sys_hal_touch_power_down(0);
	aon_pmu_hal_set_wakeup_source(WAKEUP_SOURCE_INT_TOUCHED);
	sys_hal_touch_int_enable(1);
#endif
}

void sys_hal_usb_wakeup_enable(uint8_t index)
{
	aon_pmu_ll_set_r1_usbplug_int_en(1);
	sys_ll_set_cpu0_int_32_63_en_cpu0_usbplug_int_en(1);
	aon_pmu_hal_set_wakeup_source(WAKEUP_SOURCE_INT_USBPLUG);
}

void sys_hal_rtc_wakeup_enable(uint32_t value)
{
	aon_pmu_hal_set_wakeup_source(WAKEUP_SOURCE_INT_RTC);
	sys_ll_set_cpu0_int_32_63_en_cpu0_rtc_int_en(1);
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
    uint8_t wakeup_ena = aon_pmu_ll_get_r41_wakeup_ena();
	wakeup_ena |= BIT(WAKEUP_SOURCE_INT_WIFI);
	aon_pmu_ll_set_r41_wakeup_ena(wakeup_ena);
}

void sys_hal_enable_bt_wakeup_source()
{
	uint8_t wakeup_ena = aon_pmu_ll_get_r41_wakeup_ena();
	wakeup_ena |= BIT(WAKEUP_SOURCE_INT_BT);
	aon_pmu_ll_set_r41_wakeup_ena(wakeup_ena);
}

void sys_hal_wakeup_interrupt_clear(wakeup_source_t interrupt_source)
{
	if (interrupt_source == WAKEUP_SOURCE_INT_USBPLUG) {
		aon_pmu_ll_set_r43_clr_int_usbplug(1);
		aon_pmu_ll_set_r43_clr_int_usbplug(0);
	} else if (interrupt_source == WAKEUP_SOURCE_INT_TOUCHED) {
		aon_pmu_ll_set_r43_clr_int_touched(1);
		aon_pmu_ll_set_r43_clr_int_touched(0);
	}
}

int sys_hal_set_lpo_src(sys_lpo_src_t src)
{
	PM_HAL_LOGD("set lpo src: %u\r\n", src);
	//TODO
	return BK_OK;
}

void sys_hal_low_power_hardware_init()
{
    uint32_t param = 0;
    volatile uint8_t cksel_core = 0, clkdiv_core = 0, clkdiv_bus = 0;

    param = aon_pmu_hal_reg_get(PMU_REG0);
    param = 0x1; //memcheck bypass
    aon_pmu_hal_reg_set(PMU_REG0,param);
    aon_pmu_ll_set_r0_fast_boot(1);

    /*set wakeup source*/
    aon_pmu_ll_set_r41_wakeup_ena(0x23);//enable wakeup source: int_touched,int_rtc,int_gpio,wifi wake(bt or wifi wakeup source enable when bt or wifi sleep)

    /*config the buck*/
    sys_hal_backup_set_core_26m(&cksel_core, &clkdiv_core, &clkdiv_bus);//let the cpu frequency to 26m, in order to be successfully switch voltage provide from ldo to buck

    sys_hal_enable_spi_latch();
    sys_ll_set_ana_reg11_aldosel(0);
    sys_ll_set_ana_reg12_dldosel(0);
    delay_us(1);
    /*let the ioldo low power mode*/
    sys_ll_set_ana_reg8_ioldo_lp(1);
    sys_hal_disable_spi_latch();

    sys_hal_restore_core_freq(cksel_core, clkdiv_core, clkdiv_bus);

	/*select lowpower lpo clk source*/
#if CONFIG_EXTERN_32K
    sys_ll_set_ana_reg5_itune_xtall(0x0);//0x0 provide highest current for external 32k,because the signal path long
    sys_ll_set_ana_reg5_en_xtall(0x1);
    aon_pmu_hal_lpo_src_set(PM_LPO_SRC_X32K);
#else
    aon_pmu_hal_lpo_src_set(PM_LPO_SRC_ROSC);
#endif
    /*default to close the power domain*/
    sys_hal_module_power_ctrl(POWER_MODULE_NAME_ENCP,POWER_MODULE_STATE_OFF);
    //sys_drv_module_power_ctrl(POWER_MODULE_NAME_BAKP,POWER_MODULE_STATE_OFF);
    sys_hal_module_power_ctrl(POWER_MODULE_NAME_AUDP,POWER_MODULE_STATE_OFF);
    sys_hal_module_power_ctrl(POWER_MODULE_NAME_VIDP,POWER_MODULE_STATE_OFF);
    sys_hal_module_power_ctrl(POWER_MODULE_NAME_BTSP,POWER_MODULE_STATE_OFF);
    sys_hal_module_power_ctrl(POWER_MODULE_NAME_WIFIP_MAC,POWER_MODULE_STATE_OFF);
    sys_hal_module_power_ctrl(POWER_MODULE_NAME_WIFI_PHY,POWER_MODULE_STATE_OFF);
    sys_hal_module_power_ctrl(POWER_MODULE_NAME_CPU1,POWER_MODULE_STATE_OFF);

}
