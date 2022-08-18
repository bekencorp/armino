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
#include "sys_ll_macro_def.h"

#ifdef __cplusplus          
extern "C" {              
#endif                      

//int_0_31_en DIRTY define
#define BMC32_INTERRUPT_CTRL_BIT       (1 << SYS_CPU0_INT_0_31_EN_CPU0_BMC32_INT_EN_POS)
#define HOST_0_INTERRUPT_CTRL_BIT      (1 << SYS_CPU0_INT_0_31_EN_CPU0_HOST_0_IRQ_EN_POS)
#define HOST_0_SEC_INTERRUPT_CTRL_BIT  (1 << SYS_CPU0_INT_0_31_EN_CPU0_HOST_0_SEC_IRQ_EN_POS)
#define TIMER_INTERRUPT_CTRL_BIT       (1 << SYS_CPU0_INT_0_31_EN_CPU0_TIMER_INT_EN_POS)
#define UART0_INTERRUPT_CTRL_BIT       (1 << SYS_CPU0_INT_0_31_EN_CPU0_UART_INT_EN_POS)
#define PWM_INTERRUPT_CTRL_BIT         (1 << SYS_CPU0_INT_0_31_EN_CPU0_PWM_INT_EN_POS)
#define I2C_INTERRUPT_CTRL_BIT         (1 << SYS_CPU0_INT_0_31_EN_CPU0_I2C1_INT_EN_POS)
#define SPI_INTERRUPT_CTRL_BIT         (1 << SYS_CPU0_INT_0_31_EN_CPU0_SPI_INT_EN_POS)
#define SADC_INTERRUPT_CTRL_BIT        (1 << SYS_CPU0_INT_0_31_EN_CPU0_SADC_INT_EN_POS)
#define IRDA_INTERRUPT_CTRL_BIT        (1 << SYS_CPU0_INT_0_31_EN_CPU0_IRDA_INT_EN_POS)
#define SDIO_INTERRUPT_CTRL_BIT        (1 << SYS_CPU0_INT_0_31_EN_CPU0_SDIO_INT_EN_POS)
#define GDMA_INTERRUPT_CTRL_BIT        (1 << SYS_CPU0_INT_0_31_EN_CPU0_GDMA_INT_EN_POS)
#define LA_INTERRUPT_CTRL_BIT          (1 << SYS_CPU0_INT_0_31_EN_CPU0_LA_INT_EN_POS)
#define TIMER1_INTERRUPT_CTRL_BIT      (1 << SYS_CPU0_INT_0_31_EN_CPU0_TIMER1_INT_EN_POS)
#define I2C1_INTERRUPT_CTRL_BIT        (1 << SYS_CPU0_INT_0_31_EN_CPU0_I2C_INT_EN_POS)
#define UART1_INTERRUPT_CTRL_BIT       (1 << SYS_CPU0_INT_0_31_EN_CPU0_UART1_INT_EN_POS)
#define UART2_INTERRUPT_CTRL_BIT       (1 << SYS_CPU0_INT_0_31_EN_CPU0_UART2_INT_EN_POS)
#define SPI1_INTERRUPT_CTRL_BIT        (1 << SYS_CPU0_INT_0_31_EN_CPU0_SPI1_INT_EN_POS)
#define CAN_INTERRUPT_CTRL_BIT         (1 << SYS_CPU0_INT_0_31_EN_CPU0_CAN_INT_EN_POS)
#define USB_INTERRUPT_CTRL_BIT         (1 << SYS_CPU0_INT_0_31_EN_CPU0_USB_INT_EN_POS)
#define QSPI_INTERRUPT_CTRL_BIT        (1 << SYS_CPU0_INT_0_31_EN_CPU0_QSPI_INT_EN_POS)
#define FFT_INTERRUPT_CTRL_BIT         (1 << SYS_CPU0_INT_0_31_EN_CPU0_FFT_INT_EN_POS)
#define SBC_INTERRUPT_CTRL_BIT         (1 << SYS_CPU0_INT_0_31_EN_CPU0_SBC_INT_EN_POS)
#define AUD_INTERRUPT_CTRL_BIT         (1 << SYS_CPU0_INT_0_31_EN_CPU0_AUD_INT_EN_POS)
#define I2S_INTERRUPT_CTRL_BIT         (1 << SYS_CPU0_INT_0_31_EN_CPU0_I2S_INT_EN_POS)
#define JPEGENC_INTERRUPT_CTRL_BIT     (1 << SYS_CPU0_INT_0_31_EN_CPU0_JPEGENC_INT_EN_POS)
#define JPEGDEC_INTERRUPT_CTRL_BIT     (1 << SYS_CPU0_INT_0_31_EN_CPU0_JPEGDEC_INT_EN_POS)
#define LCD_INTERRUPT_CTRL_BIT         (1 << SYS_CPU0_INT_0_31_EN_CPU0_LCD_INT_EN_POS)
#define WIFI_MODEM_EN                  (1 << SYS_CPU0_INT_0_31_EN_CPU0_WIFI_INT_PHY_MPB_EN_POS)
#define WIFI_MODEM_RC_EN               (1 << SYS_CPU0_INT_0_31_EN_CPU0_WIFI_INT_PHY_RIU_EN_POS)
#define WIFI_MAC_TX_RX_TIMER_INT_BIT   (1 << SYS_CPU0_INT_0_31_EN_CPU0_WIFI_MAC_INT_TX_RX_TIMER_EN_POS)

//int_32_63_en
#define WIFI_MAC_TX_RX_MISC_INT_BIT    (1 << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_TX_RX_MISC_EN_POS)
#define WIFI_MAC_RX_TRIGGER_INT_BIT    (1 << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_RX_TRIGGER_EN_POS)
#define WIFI_MAC_TX_TRIGGER_INT_BIT    (1 << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_TX_TRIGGER_EN_POS)
#define WIFI_MAC_PORT_TRIGGER_INT_BIT  (1 << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_PORT_TRIGGER_EN_POS)
#define WIFI_MAC_GEN_INT_BIT           (1 << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_GEN_EN_POS)
#define WIFI_HSU_INTERRUPT_CTRL_BIT    (1 << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_HSU_IRQ_EN_POS)
#define WIFI_MAC_WAKEUP_INT_BIT        (1 << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_INT_MAC_WAKEUP_EN_POS)
#define DM_INTERRUPT_CTRL_BIT          (1 << SYS_CPU0_INT_32_63_EN_CPU0_DM_IRQ_EN_POS)
#define BLE_INTERRUPT_CTRL_BIT         (1 << SYS_CPU0_INT_32_63_EN_CPU0_BLE_IRQ_EN_POS)
#define BT_INTERRUPT_CTRL_BIT          (1 << SYS_CPU0_INT_32_63_EN_CPU0_BT_IRQ_EN_POS)
#define MBOX0_INTERRUPT_CTRL_BIT       (1 << SYS_CPU0_INT_32_63_EN_CPU0_MBOX0_INT_EN_POS)
#define MBOX1_INTERRUPT_CTRL_BIT       (1 << SYS_CPU0_INT_32_63_EN_CPU0_MBOX1_INT_EN_POS)
#define BMC64_INTERRUPT_CTRL_BIT       (1 << SYS_CPU0_INT_32_63_EN_CPU0_BMC64_INT_EN_POS)
#define TOUCHED_INTERRUPT_CTRL_BIT     (1 << SYS_CPU0_INT_32_63_EN_CPU0_TOUCHED_INT_EN_POS)
#define USBPLUG_INTERRUPT_CTRL_BIT     (1 << SYS_CPU0_INT_32_63_EN_CPU0_USBPLUG_INT_EN_POS)
#define RTC_INTERRUPT_CTRL_BIT         (1 << SYS_CPU0_INT_32_63_EN_CPU0_RTC_INT_EN_POS)
#define GPIO_INTERRUPT_CTRL_BIT        (1 << SYS_CPU0_INT_32_63_EN_CPU0_GPIO_INT_EN_POS)

#define RTC_TICKS_PER_1MS                                (32)
#define LOW_POWER_DPLL_STABILITY_DELAY_TIME              (0.5) // 0.5ms
#define LOW_POWER_XTAL_26M_STABILITY_DELAY_TIME_HARDWARE (0.4) // ~0.4ms(the delay time config into hardware 2+2+2+4 = 10 tick = 1/32 * 10 ms)
#define LOW_POWER_XTAL_DPLL_STABILITY_DELAY_TIME  ((LOW_POWER_XTAL_26M_STABILITY_DELAY_TIME_HARDWARE+LOW_POWER_DPLL_STABILITY_DELAY_TIME)*1000)

typedef enum
{
	UART_CLK_DIV_1 = 0,
	UART_CLK_DIV_2,
	UART_CLK_DIV_4,
	UART_CLK_DIV_8,
}uart_clk_div_t;

typedef enum
{
	FLASH_CLK_XTAL = 0,
	FLASH_CLK_120M,
	FLASH_CLK_APLL,
}flash_clk_src_t;

/*clock power control start*/
/*
clock power enable for periphral unit
0xc[0],1:i2c0_clk enable,0,R/W   
0xc[1],1:spi0_clk enable ,0,R/W  
0xc[2],1:uart0_clk enable,0,R/W  
0xc[3],1:pwm0_clk enable ,0,R/W  
0xc[4],1:tim0_clk enable ,0,R/W  
0xc[5],1:sadc_clk enable ,0,R/W  
0xc[6],1:irda_clk enable ,0,R/W  
0xc[7],1:efuse_clk enable,0,R/W  
0xc[8],1:i2c1_clk enable ,0,R/W  
0xc[9],1:spi1_clk enable ,0,R/W  
0xc[10],1:uart1_clk enable,0,R/W 
0xc[11],1:uart2_clk enable,0,R/W 
0xc[12],1:pwm1_clk enable ,0,R/W 
0xc[13],1:tim1_clk enable ,0,R/W 
0xc[14],1:tim2_clk enable ,0,R/W 
0xc[15],1:otp_clk enable  ,1,R/W 
0xc[16],1:i2s_clk enable  ,0,R/W 
0xc[17],1:usb_clk enable  ,0,R/W 
0xc[18],1:can_clk enable  ,0,R/W 
0xc[19],1:psram_clk enable,0,R/W 
0xc[20],1:qspi0_clk enable,0,R/W 
0xc[21],1:qspi1_clk enable,0,R/W 
0xc[22],1:sdio_clk enable ,0,R/W 
0xc[23],1:auxs_clk enable ,0,R/W 
0xc[24],1:btdm_clk enable ,0,R/W 
0xc[25],1:xvr_clk enable  ,0,R/W 
0xc[26],1:mac_clk enable  ,0,R/W 
0xc[27],1:phy_clk enable  ,0,R/W 
0xc[28],1:jpeg_clk enable ,0,R/W 
0xc[29],1:disp_clk enable ,0,R/W 
0xc[30],1:aud_clk enable  ,0,R/W 
0xc[31],1:wdt_clk enable  ,0,R/W 
*/
typedef enum	//SYS TYPES index is from 1~X
{
	CLK_PWR_ID_I2C2 = 0,
	CLK_PWR_ID_SPI_1,
	CLK_PWR_ID_UART1,
	CLK_PWR_ID_PWM_1,
	CLK_PWR_ID_TIMER_1,
	CLK_PWR_ID_SARADC,
	CLK_PWR_ID_IRDA,
	CLK_PWR_ID_EFUSE,
	CLK_PWR_ID_I2C1,
	CLK_PWR_ID_SPI_2,
	CLK_PWR_ID_UART2,
	CLK_PWR_ID_UART3,
	CLK_PWR_ID_PWM_2,
	CLK_PWR_ID_TIMER_2,
	CLK_PWR_ID_TIMER_3,
	CLK_PWR_ID_OTP,
	CLK_PWR_ID_I2S_1,
	CLK_PWR_ID_USB_1,
	CLK_PWR_ID_CAN,
	CLK_PWR_ID_PSRAM,
	CLK_PWR_ID_QSPI_1,
	CLK_PWR_ID_QSPI_2,
	CLK_PWR_ID_SDIO,
	CLK_PWR_ID_AUXS,
	CLK_PWR_ID_BTDM,
	CLK_PWR_ID_XVR,
	CLK_PWR_ID_MAC,
	CLK_PWR_ID_PHY, 
	CLK_PWR_ID_JPEG,
	CLK_PWR_ID_DISP,
	CLK_PWR_ID_AUDIO,
	CLK_PWR_ID_WDG_CPU,

	CLK_PWR_ID_NONE
}dev_clk_pwr_id_t;

typedef enum
{
	CLK_PWR_CTRL_PWR_DOWN = 0,
	CLK_PWR_CTRL_PWR_UP,
}dev_clk_pwr_ctrl_t;

/*
clock select for periphral unit
*/
typedef enum	//Just for temp build
{
	CLK_SEL_ID_CORE = 4,
}dev_clk_select_id_t;

typedef enum
{
	CLK_SEL_DCO = 0,
	CLK_SEL_XTL_26M,
}dev_clk_select_t;

typedef enum
{
	CLK_DCO_DIV_1 = 0,
	CLK_DCO_DIV_2,
	CLK_DCO_DIV_4,
	CLK_DCO_DIV_8
}dev_clk_dco_div_t;


/*clock power control end*/

/*power domain ctrl modules*/
typedef enum
{
	POWER_MODULE_NAME_MEM1 = 0,
	POWER_MODULE_NAME_MEM2,
	POWER_MODULE_NAME_MEM3,
	POWER_MODULE_NAME_ENCP,
	POWER_MODULE_NAME_BAKP,
	POWER_MODULE_NAME_AHBP,
	POWER_MODULE_NAME_AUDP,
	POWER_MODULE_NAME_VIDP,
	POWER_MODULE_NAME_BTSP,      //8
	POWER_MODULE_NAME_WIFIP_MAC, //9
	POWER_MODULE_NAME_WIFI_PHY,
	POWER_MODULE_NAME_CPU1 ,     //11
	POWER_MODULE_NAME_APP ,      //12
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
typedef enum
{
	HIGH_FREQUECY_CLOCK_MODULE_CPU0 = 0,
	HIGH_FREQUECY_CLOCK_MODULE_CPU1,
	HIGH_FREQUECY_CLOCK_MODULE_CPU0_MATRIX,
	HIGH_FREQUECY_CLOCK_MODULE_CPU1_MATRIX,
	HIGH_FREQUECY_CLOCK_MODULE_SDIO,
	HIGH_FREQUECY_CLOCK_MODULE_QSPI,
	HIGH_FREQUECY_CLOCK_MODULE_PSRAM,
	HIGH_FREQUECY_CLOCK_MODULE_DISP,
	HIGH_FREQUECY_CLOCK_MODULE_JPEG,
	HIGH_FREQUECY_CLOCK_MODULE_FLASH,
	HIGH_FREQUECY_CLOCK_MODULE_USB,
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
typedef struct {
	clk_div_reg_e clk_div_reg;
	uint32_t reg_address;
} clk_div_address_map_t;
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
    BIT_SLEEP_FLAG_DEEP_SLEEP    = 16,
    BIT_SLEEP_FLAG_LOW_VOLTAGE      ,
    BIT_SLEEP_FLAG_NONE
} bit_sleep_flag_e;

typedef enum
{
    SARADC_CLK_SRC_XTAL = 0,
    SARADC_CLK_SRC_APLL,
    SARADC_CLK_SRC_UNKNOW = 0xff
} sys_saradc_clk_sel_t;

typedef enum
{
    SPI_CLK_SRC_XTAL = 0,
    SPI_CLK_SRC_APLL,
    SPI_CLK_SRC_UNKNOW = 0xff
} sys_spi_clk_sel_t;

typedef enum
{
    PWM_CLK_SRC_32K = 0,
    PWM_CLK_SRC_XTAL,
    PWM_CLK_SRC_UNKNOW = 0xff
} sys_pwm_clk_sel_t;

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
