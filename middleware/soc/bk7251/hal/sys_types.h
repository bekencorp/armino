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

#pragma once                 
                            
#include <soc/soc.h>          
#include "system_hw.h"        

#ifdef __cplusplus          
extern "C" {              
#endif                      

#define RTC_TICKS_PER_1MS                       (32)

typedef enum
{
	UART_CLK_DIV_1 = 0,
	UART_CLK_DIV_2,
	UART_CLK_DIV_4,
	UART_CLK_DIV_8,
}uart_clk_div_t;

/*
clock power down for periphral unit
[23]: qspi
[22]: jpeg
[21:20]: timers clock power down
[19]: fft
[18]: usb
[17]: sdio clock power down
[16]: watch dog clock power down
[15]: audio clock power down
[14:09]: pwms clock power down
[08]: arm watchdog clock power down
[07]: saradc clock power down
[06]: spi clock power down
[05]: i2c2 clock power down
[04]: i2s pcm  clock power down
[03]: irda clock power down
[02]: i2c1 clock power down
[01] uart2 clock power down
[00] uart1 clock power down
*/
typedef enum	//SYS TYPES index is from 1~X
{
	CLK_PWR_ID_UART1 = 0,
	CLK_PWR_ID_UART2,
	CLK_PWR_ID_I2C1,
	CLK_PWR_ID_IRDA,
	CLK_PWR_ID_I2S_1,
	CLK_PWR_ID_I2C2,
	CLK_PWR_ID_SPI_1,
	CLK_PWR_ID_SARADC,
	CLK_PWR_ID_WDG_CPU,		//watchdog for ARM
	CLK_PWR_ID_PWM_1,
	CLK_PWR_ID_PWM_2,
	CLK_PWR_ID_PWM_3,
	CLK_PWR_ID_PWM_4,
	CLK_PWR_ID_PWM_5,
	CLK_PWR_ID_PWM_6,
	CLK_PWR_ID_AUDIO,
	CLK_PWR_ID_WDG_T410,	//watchdog for T410
	CLK_PWR_ID_SDIO,
	CLK_PWR_ID_USB_1,
	CLK_PWR_ID_FFT,
	CLK_PWR_ID_TIMER_1,
	CLK_PWR_ID_TIMER_2,
	CLK_PWR_ID_JPEG,
	CLK_PWR_ID_QSPI_1,
	CLK_PWR_ID_NONE
}dev_clk_pwr_id_t;

typedef enum
{
	CLK_PWR_CTRL_PWR_DOWN = 0,
	CLK_PWR_CTRL_PWR_UP,
}dev_clk_pwr_ctrl_t;

/*
clock select for periphral unit
//[16:17] QSPI clock select, 0-DCO, 1-26M, 2-120M (DPLL division), 3-reserved
[15:10] reserved_1
[09]: SARADC clock select Audio PLL, if saradc is 0
[08]: sdio clock select
[07]: pwms clock select
[06]: spi clock select
[05]: saradc clock select
[04]: i2c2 clock select
[03]: irda clock select
[02]: i2c1 clock select
[01]: uart2 clock select
[00]: uart1 clock select
*/
typedef enum	//SYS TYPES index is from 1~X
{
	CLK_SEL_ID_UART1 = 0, //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M
	CLK_SEL_ID_UART2,	  //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M
	CLK_SEL_ID_I2C1,	  //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M
	CLK_SEL_ID_IRDA,	  //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M
	CLK_SEL_ID_I2C2,	  //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M
	CLK_SEL_ID_SARADC,	  //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M
	CLK_SEL_ID_SPI_1,	  //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M
	CLK_SEL_ID_PWMS,	  //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M(all of the PWM)
	CLK_SEL_ID_SDIO,	  //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M
	CLK_SEL_ID_SARADC_AUDIO_PLL, //1:SARADC uses Audio if CLK_SEL_ID_SARADC select DCO.
	CLK_SEL_ID_RESERVE_1,
	CLK_SEL_ID_RESERVE_2,
	CLK_SEL_ID_RESERVE_3,
	CLK_SEL_ID_RESERVE_4,
	CLK_SEL_ID_RESERVE_5,
	CLK_SEL_ID_RESERVE_6,

	/* NOTICE:QSPI uses 2 Bits for clock select.
	 we can't use normal API, so remove the QSPI ID.
	//CLK_SEL_ID_QSPI,
	*/
}dev_clk_select_id_t;

typedef enum
{
	CLK_SEL_DCO = 0,
	CLK_SEL_XTL_26M,
}dev_clk_select_t;

//Special clock select:QSPI doesn't use "dev_clk_select_t"
typedef enum
{
	QSPI_CLK_SEL_DCO = 0,
	QSPI_CLK_SEL_XTL_26M,
	QSPI_CLK_SEL_DPLL,	//DPLL:120M division
}qspi_clk_select_t;

typedef enum
{
	CLK_DCO_DIV_1 = 0,
	CLK_DCO_DIV_2,
	CLK_DCO_DIV_4,
	CLK_DCO_DIV_8
}dev_clk_dco_div_t;
/*power domain ctrl modules*/
typedef enum
{
	POWER_MODULE_NAME_MEM1 = 0,
	POWER_MODULE_NAME_MEM2 ,
	POWER_MODULE_NAME_MEM3 ,
	POWER_MODULE_NAME_ENCP ,
	POWER_MODULE_NAME_BAKP ,
	POWER_MODULE_NAME_AHBP ,
	POWER_MODULE_NAME_AUDP ,
	POWER_MODULE_NAME_VIDP ,
	POWER_MODULE_NAME_BTSP ,
	POWER_MODULE_NAME_WIFIP_MAC ,
	POWER_MODULE_NAME_WIFI_PHY ,
	POWER_MODULE_NAME_APP ,
	POWER_MODULE_NAME_NONE
}power_module_name_t;
typedef enum
{
	POWER_MODULE_STATE_ON = 0,
    POWER_MODULE_STATE_OFF,
	POWER_MODULE_STATE_NONE
}power_module_state_t;
typedef enum
{
	POWER_MODULE_STATE_LOWVOL_ON = 0,
    POWER_MODULE_STATE_LOWVOL_OFF,
	POWER_MODULE_STATE_LOWVOL_NONE
}lowvol_module_state_t;
typedef enum
{
	ENTER_DEEP_SLEEP_DISABLE = 0,
    ENTER_DEEP_SLEEP_ENABLE,
	ENTER_DEEP_SLEEP_NONE
}enter_deepsleep_state_t;
typedef enum
{
	ENTER_LOW_VOLTAGE_DISABLE = 0,
    ENTER_LOW_VOLTAGE_ENABLE,
	ENTER_LOW_VOLTAGE_NONE
}enter_lowvoltage_state_t;

/*RF using owner modules*/
typedef enum
{
    MODULE_NAME_WIFI = 0,
	MODULE_NAME_BT,
	MODULE_NAME_NONE
}module_name_t;
/*high frequecy modules*/
typedef enum
{
	HIGH_FREQUECY_CLOCK_MODULE_CPU0 = 0,
	HIGH_FREQUECY_CLOCK_MODULE_CPU1 ,
	HIGH_FREQUECY_CLOCK_MODULE_CPU0_MATRIX ,
	HIGH_FREQUECY_CLOCK_MODULE_CPU1_MATRIX ,
	HIGH_FREQUECY_CLOCK_MODULE_SDIO ,
	HIGH_FREQUECY_CLOCK_MODULE_QSPI ,
	HIGH_FREQUECY_CLOCK_MODULE_PSRAM ,
	HIGH_FREQUECY_CLOCK_MODULE_DISP ,
	HIGH_FREQUECY_CLOCK_MODULE_JPEG ,
	HIGH_FREQUECY_CLOCK_MODULE_FLASH ,
	HIGH_FREQUECY_CLOCK_MODULE_USB ,
	HIGH_FREQUECY_CLOCK_MODULE_NONE
}high_clock_module_name_t;
typedef enum
{
	WAKEUP_SOURCE_INT_GPIO = 0,
	WAKEUP_SOURCE_INT_RTC ,
	WAKEUP_SOURCE_INT_SYSTEM_WAKE ,
	WAKEUP_SOURCE_INT_USBPLUG ,
	WAKEUP_SOURCE_INT_TOUCHED ,
	WAKEUP_SOURCE_INT_NONE ,
}wakeup_source_t;
typedef enum
{
	ANALOG_REG0 = 0,
	ANALOG_REG1,
	ANALOG_REG2,
	ANALOG_REG3,
	ANALOG_REG4,
	ANALOG_REG5,
	ANALOG_REG6,
    ANALOG_REG7,
	ANALOG_REG8,
	ANALOG_REG9,
	ANALOG_REG10,
	ANALOG_REG11,
	ANALOG_REG12,
	ANALOG_REG13,
    ANALOG_REG14,
	ANALOG_REG15,
	ANALOG_REG16,
	ANALOG_REG17,
	ANALOG_REG18,
	ANALOG_REG19,
	ANALOG_NONE
} analog_reg_t;
#define ANALOG_ADDRESS_MAP  \
{	\
	{ANALOG_REG0, SYS_ANA_REG0_ADDR, }, \
	{ANALOG_REG1, SYS_ANA_REG1_ADDR, }, \
	{ANALOG_REG2, SYS_ANA_REG2_ADDR, }, \
	{ANALOG_REG3, SYS_ANA_REG3_ADDR, }, \
	{ANALOG_REG4, SYS_ANA_REG4_ADDR, }, \
	{ANALOG_REG5, SYS_ANA_REG5_ADDR, }, \
	{ANALOG_REG6, SYS_ANA_REG6_ADDR, }, \
	{ANALOG_REG7, SYS_ANA_REG7_ADDR, }, \
	{ANALOG_REG8, SYS_ANA_REG8_ADDR, }, \
	{ANALOG_REG9, SYS_ANA_REG9_ADDR, }, \
	{ANALOG_REG10, SYS_ANA_REG10_ADDR, }, \
	{ANALOG_REG11, SYS_ANA_REG11_ADDR, }, \
	{ANALOG_REG12, SYS_ANA_REG12_ADDR, }, \
	{ANALOG_REG13, SYS_ANA_REG13_ADDR, }, \
	{ANALOG_REG14, SYS_ANA_REG14_ADDR, }, \
	{ANALOG_REG15, SYS_ANA_REG15_ADDR, }, \
	{ANALOG_REG16, SYS_ANA_REG16_ADDR, }, \
	{ANALOG_REG17, SYS_ANA_REG17_ADDR, }, \
	{ANALOG_REG18, SYS_ANA_REG18_ADDR, }, \
	{ANALOG_REG19, SYS_ANA_REG19_ADDR, }, \
}
typedef struct {
	analog_reg_t analog_reg;
	uint32_t analog_reg_address;
} analog_address_map_t;
typedef enum
{
	CLK_DIV_REG0 = 0,
	CLK_DIV_REG1,
	CLK_DIV_REG2,
	CLK_DIV_NONE
} clk_div_reg_e;
#define CLK_DIV_ADDRESS_MAP  \
{	\
	{CLK_DIV_REG0, SYS_CPU_CLK_DIV_MODE1_ADDR, }, \
	{CLK_DIV_REG1, SYS_CPU_CLK_DIV_MODE2_ADDR, }, \
	{CLK_DIV_REG2, SYS_CPU_26M_WDT_CLK_DIV_ADDR, }, \
}
typedef enum
{
	PMU_REG0 = 0,
	PMU_REG1,
	PMU_REG2,
	PMU_REG3,
	PMU_REG0x40,
	PMU_REG0x41,
	PMU_REG0x42,
	PMU_REG0x43,
	PMU_REG0x70,
	PMU_REG0x71,
	PMU_NONE
} pmu_reg_e;
#define PMU_ADDRESS_MAP  \
{	\
	{PMU_REG0, AON_PMU_REG0_ADDR, }, \
	{PMU_REG1, AON_PMU_REG1_ADDR, }, \
	{PMU_REG2, AON_PMU_REG2_ADDR, }, \
	{PMU_REG3, AON_PMU_REG3_ADDR, }, \
	{PMU_REG0x40, AON_PMU_REG40_ADDR, }, \
	{PMU_REG0x41, AON_PMU_REG41_ADDR, }, \
	{PMU_REG0x42, AON_PMU_REG42_ADDR, }, \
	{PMU_REG0x43, AON_PMU_REG43_ADDR, }, \
	{PMU_REG0x70, AON_PMU_REG70_ADDR, }, \
	{PMU_REG0x71, AON_PMU_REG71_ADDR, }, \
}
typedef struct {
	pmu_reg_e pmu_reg;
	uint32_t reg_address;
}pmu_address_map_t;
typedef enum
{
	LOW_POWER_MODE_NORMAL_SLEEP = 0,
	LOW_POWER_MODE_LOW_VOLTAGE ,
	LOW_POWER_DEEP_SLEEP ,
	LOW_POWER_MODE_NONE
}low_power_sleep_mode_e;
#ifdef __cplusplus 
}                  
#endif             
