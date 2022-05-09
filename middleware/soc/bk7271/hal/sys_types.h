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
//< bit[0], UART1               
//< bit[1], UART2               
//< bit[2], UART3               
//< bit[3], FM_I2C1             
//< bit[4], I2C1                
//< bit[5], I2C2                
//< bit[6], SDIO host           
//< bit[7]                      
//< bit[8], SPI1                
//< bit[9], SPI2                
//< bit[10], SPI3               
//< bit[11], IRDA               
//< bit[12:13], timers          
//< bit[14], pwm group0[0/1/2]  
//< bit[15], pwm group1[3/4/5]  
//< bit[16], pwm group2[6/7/8]  
//< bit[17], pwm group3[9/10/11]
//< bit[18], SARADC             
//< bit[19], CEC                
//< bit[20:31] 

//group2 - Bit[32~63]
//arm_watchdog: 1;       < bit[0] 
//usb1:         1;       < bit[1] 
//usb2:         1;       < bit[2] 
//reserved_1:   1;       < bit[3] 
//qspi:         1;       < bit[4] 
//security:     1;       < bit[5] 
//sdio_dma:     1;       < bit[6] 

*/
typedef enum	//SYS TYPES index is from 1~X
{
	CLK_PWR_ID_UART1 = 0,
	CLK_PWR_ID_UART2,
	CLK_PWR_ID_UART3,
	CLK_PWR_ID_I2C_FM,	//For FM
	CLK_PWR_ID_I2C1,
	CLK_PWR_ID_I2C2,
	CLK_PWR_ID_SDIO_HOST,
	CLK_PWR_ID_RESERVED_1,
	CLK_PWR_ID_SPI_1,
	CLK_PWR_ID_SPI_2,
	CLK_PWR_ID_SPI_3,
	CLK_PWR_ID_IRDA,
	CLK_PWR_ID_TIMER_1,
	CLK_PWR_ID_TIMER_2,
	CLK_PWR_ID_PWM_GROUP_1,	//pwm[0/1/2]
	CLK_PWR_ID_PWM_GROUP_2,
	CLK_PWR_ID_PWM_GROUP_3,
	CLK_PWR_ID_PWM_GROUP_4,
	CLK_PWR_ID_SARADC,
	CLK_PWR_ID_CEC,

	CLK_PWR_ID_GROUP_2_START = 32,
	CLK_PWR_ID_WDG_CPU = CLK_PWR_ID_GROUP_2_START,
	CLK_PWR_ID_USB_1,
	CLK_PWR_ID_USB_2,
	CLK_PWR_ID_RESERVED_2,
	CLK_PWR_ID_QSPI_1,
	CLK_PWR_ID_SDIO_DMA,
	CLK_PWR_ID_NONE
}dev_clk_pwr_id_t;

typedef enum
{
	CLK_PWR_CTRL_PWR_DOWN = 0,
	CLK_PWR_CTRL_PWR_UP,
}dev_clk_pwr_ctrl_t;

/*
clock select for periphral unit
uart1: 1; < bit[0], UART1 clock select, 1-CLK_26M, 0-DCO Divided clk_mux 					  
uart2: 1; < bit[1], UART2 clock select, 1-CLK_26M, 0-DCO Divided clk_mux 					  
uart3: 1; < bit[2], UART3 clock select, 1-CLK_26M, 0-DCO Divided clk_mux 					 
irda:  1; < bit[3], IRDA clock select, 1-CLK_26M, 0-DCO Divided clk_mux 					   
fm_i2c:1; < bit[4], I2C2 clock select, 1-CLK_26M, 0-DCO Divided clk_mux 					  
i2c1:  1; < bit[5], I2C1 clock select, 1-CLK_26M, 0-DCO Divided clk_mux 						  
i2c2:  1; < bit[6], I2C2 clock select, 1-CLK_26M, 0-DCO Divided clk_mux 						  
spi1:  1; < bit[7], SPI1 clock select, 1-CLK_26M, 0-DCO Divided clk_mux 					   
spi2:  1; < bit[8], SPI2 clock select, 1-CLK_26M, 0-DCO Divided clk_mux 					   
spi3:  1; < bit[9], SPI3 clock select, 1-CLK_26M, 0-DCO Divided clk_mux 					   
pwms:  1; < bit[10], PWMs clock select Audio PLL, if saradc is 0 								
sdio:  1; < bit[11], SDIO host clock select Audio PLL, if saradc is 0 							
efuse: 1; < bit[12], EFUSE host clock select Audio PLL, if saradc is 0 						
cec:   1; < bit[13], CEC clock select Audio PLL, if saradc is 0  								
reserved_1: 2;	 < bit[14:15]  																
qspi: 2;  < bit[16:17] QSPI clock division, 0: 1-div, 1: 2-div, 2: 4-div, 3: 8-div 			
*/
typedef enum	//SYS TYPES index is from 1~X
{
	CLK_SEL_ID_UART1 = 0, //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M
	CLK_SEL_ID_UART2,	  //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M
	CLK_SEL_ID_UART3,	  //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M
	CLK_SEL_ID_IRDA,	  //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M
	CLK_SEL_ID_I2C_FM,	  //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M
	CLK_SEL_ID_I2C1,	  //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M
	CLK_SEL_ID_I2C2,	  //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M
	CLK_SEL_ID_SPI_1,	  //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M
	CLK_SEL_ID_SPI_2,	  //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M
	CLK_SEL_ID_SPI_3,	  //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M

	//WARNING: Below ID uses DCO or AUDIO PLL?icu_struct.h isn't match with BK7271 address mapping.
	CLK_SEL_ID_PWMS,	  //1:??? The codes in icu_struct.h isn't match with BK7271 address mapping.
	CLK_SEL_ID_SDIO,	  //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M
	CLK_SEL_ID_EFUSE,	  //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M
	CLK_SEL_ID_CEC,	  	  //0:CLK_SEL_DCO,1:CLK_SEL_XTL_26M

	CLK_SEL_ID_RESERVE_1,
	CLK_SEL_ID_RESERVE_2,

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
	CHARGE_STEP1 = 0,
	CHARGE_STEP2,
	CHARGE_STEP3,
	CHARGE_STEP4,
} sys_drv_charge_step_t;
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
