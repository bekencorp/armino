#include <stdio.h>
#include <stdlib.h>
#include <common/bk_include.h>
#include "cm_backtrace.h"

#define FIREWARE_NAME    "app"
#define HARDWARE_VERSION "V1.0.0"
#define SOFTWARE_VERSION "V1.0.0"

#define BK7236_MPW

#define write_data(addr,val)                 *((volatile uint32_t *)(addr)) = val
#define read_data(addr,val)                  val = *((volatile uint32_t *)(addr))
#define get_addr_data(addr)                  *((volatile uint32_t *)(addr))

void close_wdt(void)
{
	REG_WRITE(0x44000600, 0x5A0000);
	REG_WRITE(0x44000600, 0xA50000);
	REG_SET(0x44800000 + 4 * 2, 1, 1, 1);
	REG_WRITE(0x44800000 + 4 * 4, 0x5A0000);
	REG_WRITE(0x44800000 + 4 * 4, 0xA50000);
}

void write_ana_reg(uint8_t reg_idx,uint32_t data)
{
	write_data(0x44010000+(0x40+reg_idx)*4,data);
#ifndef	BK7236_MPW
	while(!((get_addr_data(0x44010000+0x3a*4)) >> (reg_idx)));
#else
	while(((get_addr_data(0x44010000+0xb*4)) >> (reg_idx+8)));
#endif
}

uint32_t read_ana_reg(uint8_t reg_idx)
{
	uint32_t data = 0;
	data = get_addr_data(0x44010000+(0x40+reg_idx)*4);
	return data;
}

void Delay1(volatile uint32_t times)
{
	while(times--);
}

#define addSYSTEM_Reg0xc                                        *((volatile unsigned long *) (0x44010000+0xc*4))
#define set_SYSTEM_Reg0xc(val)                                  addSYSTEM_Reg0xc = (val)

//addSYSTEM_Reg0x8
#define addSYSTEM_Reg0x8                                        *((volatile unsigned long *) (0x44010000+0x8*4))
#define posSYSTEM_Reg0x8_clkdiv_core                            0
#define bitSYSTEM_Reg0x8_clkdiv_core                            0xF
#define set_SYSTEM_Reg0x8_clkdiv_core(val)                      addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0xF)) | ((val) << 0))
#define get_SYSTEM_Reg0x8_clkdiv_core                           (addSYSTEM_Reg0x8 & 0xF)

#define posSYSTEM_Reg0x8_cksel_core                             4
#define bitSYSTEM_Reg0x8_cksel_core                             0x30
#define set_SYSTEM_Reg0x8_cksel_core(val)                       addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0x30)) | ((val) << 4))
#define get_SYSTEM_Reg0x8_cksel_core                            ((addSYSTEM_Reg0x8 & 0x30) >> 4)

#define addSYSTEM_Reg0x9                                        *((volatile unsigned long *) (0x44010000+0x9*4))
#define set_SYSTEM_Reg0x9_cksel_flash(val)                      addSYSTEM_Reg0x9 = ((addSYSTEM_Reg0x9 & (~0x3000000)) | ((val) << 24))

#define addSYSTEM_Reg0x40                                        *((volatile unsigned long *) (0x44010000+0x40*4))
#define set_SYSTEM_Reg0x40(val)                                  addSYSTEM_Reg0x40 = (val)

#define addSYSTEM_Reg0x43                                        *((volatile unsigned long *) (0x44010000+0x43*4))
#define set_SYSTEM_Reg0x43(val)                                  addSYSTEM_Reg0x43 = (val)

#define addSYSTEM_Reg0x48                                        *((volatile unsigned long *) (0x44010000+0x48*4))
#define set_SYSTEM_Reg0x48(val)                                  addSYSTEM_Reg0x48 = (val)

#define addSYSTEM_Reg0x49                                        *((volatile unsigned long *) (0x44010000+0x49*4))
#define set_SYSTEM_Reg0x49(val)                                  addSYSTEM_Reg0x49 = (val)

void bk7236_sys_init(void)
{
	uint32_t val = read_ana_reg(0x5);//ffe7 31a7
	val |=  (0x1 << 14) | (0x1 << 5) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1);
	write_ana_reg(0x5,val);

	val = read_ana_reg(0x0);//ffe7 31a7
	val |= (0x13 << 20) ;
	write_ana_reg(0x0,val);

	set_SYSTEM_Reg0x40(0xF1305B57);  // triger dpll
	set_SYSTEM_Reg0x40(0xF5305B57);
	set_SYSTEM_Reg0x40(0xF1305B57);

	*((volatile unsigned long *) (0x44010000+0x42*4)) = 0x7E003450; //wangjian20221110 xtal=0x50
	set_SYSTEM_Reg0xc(0x0c008084);
	set_SYSTEM_Reg0x43(0xC5F00B88);
	set_SYSTEM_Reg0x48(0x57E627FA);
	set_SYSTEM_Reg0x49(0x787FC6A4);

	set_SYSTEM_Reg0x9_cksel_flash(0x2);

	/* clk_divd 120MHz, 
	 * 1, the core clock is depended on CONFIG_CPU_FREQ_HZ and configSYSTICK_CLOCK_HZ.
	 *    Pay attention to bk_pm_module_vote_cpu_freq,and the routine will switch core
	 *    clock;
	 * 2, sysTick module's clock source is processor clock now;
	 */
	set_SYSTEM_Reg0x8_clkdiv_core(0x3);
	Delay1(10000);
	set_SYSTEM_Reg0x8_cksel_core(0x3);/*clock source: DPLL, 480M*/
}

void backtrace_init(void)
{
	cm_backtrace_init(FIREWARE_NAME, HARDWARE_VERSION, SOFTWARE_VERSION);
}
