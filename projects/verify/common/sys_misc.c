#include <stdio.h>
#include <stdlib.h>
#include <common/bk_include.h>
#include "sys_hal.h"
#include "system_hw.h"
#include "cm_backtrace.h"

#define FIREWARE_NAME    "app"
#define HARDWARE_VERSION "V1.0.0"
#define SOFTWARE_VERSION "V1.0.0"

void close_wdt(void)
{
	REG_WRITE(SOC_AON_WDT_REG_BASE, 0x5A0000);
	REG_WRITE(SOC_AON_WDT_REG_BASE, 0xA50000);
	REG_SET(SOC_WDT_REG_BASE + 4 * 2, 1, 1, 1);
	REG_WRITE(SOC_WDT_REG_BASE + 4 * 4, 0x5A0000);
	REG_WRITE(SOC_WDT_REG_BASE + 4 * 4, 0xA50000);
}

void Delay1(volatile uint32_t times)
{
	while(times--);
}

void bk7236_sys_init(void)
{
	uint32_t val = sys_hal_analog_get(0x5);//ffe7 31a7
	val |=  (0x1 << 14) | (0x1 << 5) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1);
	sys_hal_analog_set(0x5,val);

	val = sys_hal_analog_get(ANALOG_REG0);//ffe7 31a7
	val |= (0x13 << 20) ;
	sys_hal_analog_set(ANALOG_REG0,val);

	sys_hal_analog_set(ANALOG_REG0, 0xF1305B57);  // triger dpll
	sys_ll_set_ana_reg0_dsptrig(1);
	sys_ll_set_ana_reg0_dsptrig(0);

	sys_hal_analog_set(ANALOG_REG2, 0x7E003450); //wangjian20221110 xtal=0x50
	//sys_ll_set_cpu_device_clk_enable_value(0x0c008084);
	sys_hal_analog_set(ANALOG_REG3, 0xC5F00B88);
	sys_hal_analog_set(ANALOG_REG8, 0x57E62FFE);
	sys_hal_analog_set(ANALOG_REG9, 0x787BC6A4);
	sys_hal_analog_set(ANALOG_REG10, 0xB215C3A7);
	sys_hal_analog_set(ANALOG_REG11, 0x9FEF31FF);
	sys_hal_analog_set(ANALOG_REG12, 0x9F03EF6F);
	sys_hal_analog_set(ANALOG_REG13, 0x1F6FB3FE);

	sys_hal_flash_set_clk(0x2);

	/* clk_divd 120MHz,
	 * 1, the core clock is depended on CONFIG_CPU_FREQ_HZ and configSYSTICK_CLOCK_HZ.
	 *    Pay attention to bk_pm_module_vote_cpu_freq,and the routine will switch core
	 *    clock;
	 * 2, sysTick module's clock source is processor clock now;
	 */
	sys_hal_mclk_div_set(480000000/CONFIG_CPU_FREQ_HZ - 1);
	Delay1(10000);
	sys_hal_mclk_mux_set(0x3);/*clock source: DPLL, 480M*/
}

void backtrace_init(void)
{
	cm_backtrace_init(FIREWARE_NAME, HARDWARE_VERSION, SOFTWARE_VERSION);
}
