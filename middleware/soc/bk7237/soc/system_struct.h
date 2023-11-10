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

#ifdef __cplusplus
extern "C" {
#endif
#include "system_cap.h"

typedef volatile struct {

   /* REG_0x00 */
		union {
			uint32_t v;         // 0x0[31:0] Only read default: 0x53434647
		} device_id; 

   /* REG_0x01 */
		union {
			uint32_t v;        // 0x1[31:0] Only read default:0x72560001
		} version_id;


   /* REG_0x02 */
		union {
			struct {
				uint32_t core0_halted: 1;           // 0x2[0] core0 halt indicate Only read
				uint32_t core1_halted: 1;           // 0x2[1] core1 halt indicate Only read
				uint32_t reserved0:    2;          // < bit[2:3]
				uint32_t cpu0_sw_reset: 1;          // 0x2[4] cpu0_sw_reset indicate Only read
				uint32_t cpu1_sw_reset: 1;          // 0x2[5] cpu1_sw_reset indicate Only read
				uint32_t reserved1:     2;          // < bit[6:7]
				uint32_t cpu0_pwr_dw_state: 1;      // 0x2[8] cpu0_pwr_dw_state Only read
				uint32_t cpu1_pwr_dw_state: 1;      // 0x2[9] cpu1_pwr_dw_state Only read
				uint32_t reserved2:         22;     // < bit[10:31]
			};
			uint32_t v;
		} cpu_current_run_status;

   /* REG_0x03 */
		union {
			struct {
				uint32_t boot_mode:     1;          // 0x3[0] 0:ROM boot 1:FLASH boot
				uint32_t reserved1:     7;          // < bit[1:7]
				uint32_t jtag_core_sel: 1;          // 0x3[8] 0: jtag connect core0, 1:jtag connect core1
				uint32_t flash_sel:     1;          // 0x3[9] 0: normal flash operation 1:flash download by spi
				uint32_t reserved2:     22;          // < bit[10:31]
			};
			uint32_t v;
		} cpu_storage_connect_op_select;

   /* REG_0x04 */
		union {
			struct {
				uint32_t cpu0_sw_rst:   1;         // 0x4[0] reserved
				uint32_t cpu0_pwr_dw:   1;         // 0x4[1] reserved
				uint32_t cpu0_int_mask: 1;         // 0x4[2] cpu0 int mask
				uint32_t cpu0_halt:     1;         // 0x4[3] core0 halt indicate
				uint32_t cpu0_clk_div:  4;         // 0x4[4:7] Frequency division : F/(1+N), N is the data of the reg value
				uint32_t reserved:     24;         // < bit[8:31] reserved
			};
			uint32_t v;
		} cpu0_int_halt_clk_op;

   /* REG_0x05 */
		union {
			struct {
				uint32_t cpu1_sw_rst:      1;       // 0x5[0] reserved
				uint32_t cpu1_pwr_dw:      1;       // 0x5[1] reserved
				uint32_t cpu1_int_mask:    1;       // 0x5[2] cpu1 int mask
				uint32_t cpu1_halt:        1;       // 0x5[3] core1 halt indicate
				uint32_t cpu1_clk_div:     4;       // 0x5[4:7] 0: jtag connect core0, 1:jtag connect core1
				uint32_t reserved:        24;       // < bit[8:31] reserved
			};
			uint32_t v;
		} cpu1_int_halt_clk_op;

   /* REG_0x06 */
	uint32_t reserved_reg06;
   /* REG_0x07 */
	uint32_t reserver_reg07;

   /* REG_0x08 */
		union {
			struct {
				uint32_t clkdiv_core:  4;            // 0x8[0:3] Frequency division : F/(1+N), N is the data of the reg value
				uint32_t cksel_core:   2;            // 0x8[4:5]  0: clk_DCO      1 : XTAL
				uint32_t clkdiv_bus:   1;            // 0x8[6] Frequency division : F/(1+N), N is the data of the reg value
				uint32_t reserved1:    1;            // < bit[7] reserved
				uint32_t clkdiv_uart0: 2;            // 0x8[8:9] Frequency division :    0:/1  1:/2  2:/4  3:/8
				uint32_t cksel_uart0:  1;            // 0x8[10] 0:XTAL              1:APLL
				uint32_t clkdiv_uart1: 2;            // 0x8[11:12] Frequency division :  0:/1  1:/2  2:/4  3:/8
				uint32_t cksel_uart1:  1;            // 0x8[13]  0:XTAL              1:APLL
				uint32_t clkdiv_uart2: 2;            // 0x8[14:15] Frequency division :  0:/1  1:/2  2:/4  3:/8
				uint32_t cksel_uart2:  1;            // 0x8[16]  0:XTAL              1:APLL
				uint32_t cksel_sadc:   1;            // 0x8[17] 0:XTAL              1:APLL
				uint32_t cksel_pwm0:   1;            // 0x8[18]  0:clk32              1:XTAL
				uint32_t cksel_pwm1:   1;            // 0x8[19]  0:clk32              1:XTAL
				uint32_t cksel_timer0:   1;          // 0x8[20]  0:clk32              1:XTAL
				uint32_t cksel_timer1:   1;          // 0x8[21]  0:clk32              1:XTAL
				uint32_t cksel_timer2:   1;          // 0x8[22]  0:clk32              1:XTAL

				uint32_t reserved2:      1;          // < bit[23] reserved
				uint32_t cksel_i2s:      1;          // 0x8[24] 0:XTAL              1:APLL
				uint32_t cksel_aud:      1;          // 0x8[25] 0:XTAL              1:APLL
				uint32_t clkdiv_jpeg:    4;          // 0x8[26:29] Frequency division : F/(1+N), N is the data of the reg value
				uint32_t cksel_jpeg:     1;          // 0x8[30] 0:clk_320M      1:clk_480M
				uint32_t clkdiv_disp_l:  1;          // bit[31] Frequency division : F/(1+clkdiv_disp_l+clkdiv_disp_h*2)
			};
			uint32_t v;
		} cpu_clk_div_mode1;

   /* REG_0x09 */
		union {
			struct {
				uint32_t clkdiv_disp_h:   3;         // 0x9[0:2] Frequency division : F/(1+clkdiv_disp_l+clkdiv_disp_h*2)
				uint32_t cksel_disp:   1;            // 0x9[3] 0:clk_320M      1:clk_480M
				uint32_t ckdiv_psram:   1;           // 0x9[4] Frequency division : F/(1+N), N is the data of the reg value
				uint32_t cksel_psram:   1;           // 0x9[5] 0:clk_320M      1:clk_480M
				uint32_t ckdiv_qspi0:   4;           // 0x9[6:9] Frequency division : F/(1+N), N is the data of the reg value
				uint32_t reserved1:   4;             // bit[10:13] reserved

				uint32_t ckdiv_sdio:   3;            // 0x9[14:16]  0:/1  1:/2  2:/4  3:/8  4:/16  5:/32  6:/64  7:/256
				uint32_t cksel_sdio:   1;            // 0x9[17]  0：XTAL          1：320M
				uint32_t ckdiv_auxs:   4;            // 0x9[18:21] Frequency division : F/(1+N), N is the data of the reg value
				uint32_t cksel_auxs:   2;            // 0x9[22:23]  0:DCO     1:APLL      2:320M       4:480M
				uint32_t cksel_flash:   2;           // 0x9[24:25]  0:XTAL      1:APLL      1x :clk_120M
				uint32_t ckdiv_flash:   2;           // 0x9[26:27]  0:/1  1:/2  2:/4  3:/8
				uint32_t ckdiv_i2s0:   3;            // 0x9[28:30]  0:/1  1:/2  2:/4  3:/8  4:/16  5:/32  6:/64  7:/256
				uint32_t reserved2:  1;              // bit[31] reserved
			};
			uint32_t v;
		} cpu_clk_div_mode2;


   /* REG_0xa */
		union {
			struct {
				uint32_t ckdiv_26m: 2;               //0xa[1:0] 	 0:/1  1:/2  2:/4  3:/8
				uint32_t ckdiv_wdt: 2;               //0xa[3:2] 	 0:/2 1:/4 2:/8 3:/16
				uint32_t reserved:  28;              // bit[4:31] reserved
			};
			uint32_t v;
		} cpu_26m_wdt_clk_div;

   /* REG_0xb */
		union {
			struct {
				uint32_t anaspi_freq: 6;         //0xb[5:0]
				uint32_t reserved:  26;          // bit[6:31] reserved
			};
			uint32_t v;
		} cpu_anaspi_freq;

   /* REG_0xc */
		union {
			struct {
				uint32_t i2c0_cken: 1;            //0xc[0]   1:i2c0_clk enable
				uint32_t spi0_cken: 1;            //0xc[1]   1:spi0_clk enable
				uint32_t uart0_cken: 1;           //0xc[2]   1:uart0_clk enable
				uint32_t pwm0_cken: 1;            //0xc[3]   1:pwm0_clk enable
				uint32_t tim0_cken: 1;            //0xc[4]   1:tim0_clk enable
				uint32_t sadc_cken: 1;            //0xc[5]   1:sadc_clk enable
				uint32_t irda_cken: 1;            //0xc[6]   1:irda_clk enable
				uint32_t efuse_cken: 1;           //0xc[7]   1:efuse_clk enable
				uint32_t i2c1_cken: 1;            //0xc[8]   1:i2c1_clk enable
				uint32_t spi1_cken: 1;            //0xc[9]   1:spi1_clk enable
				uint32_t uart1_cken: 1;           //0xc[10]   1:uart1_clk enable
				uint32_t uart2_cken: 1;           //0xc[11]   1:uart2_clk enable
				uint32_t pwm1_cken: 1;            //0xc[12]   1:pwm1_clk enable
				uint32_t tim1_cken: 1;            //0xc[13]   1:tim1_clk enable
				uint32_t tim2_cken: 1;            //0xc[14]   1:tim2_clk enable
				uint32_t otp_cken: 1;             //0xc[15]   1:otp_clk enable
				uint32_t i2s_cken: 1;             //0xc[16]   1:i2s_clk enable
				uint32_t usb_cken: 1;             //0xc[17]   1:usb_clk enable
				uint32_t can_cken: 1;             //0xc[18]   1:can_clk enable
				uint32_t psram_cken: 1;           //0xc[19]   1:psram_clk enable
				uint32_t qspi0_cken: 1;           //0xc[20]   1:qspi0_clk enable
				uint32_t qspi1_cken: 1;           //0xc[21]   1:qspi1_clk enable
				uint32_t sdio_cken: 1;            //0xc[22]   1:sdio_clk enable
				uint32_t auxs_cken: 1;            //0xc[23]   1:auxs_clk enable
				uint32_t btdm_cken: 1;            //0xc[24]   1:btdm_clk enable
				uint32_t xvr_cken: 1;             //0xc[25]   1:xvr_clk enable
				uint32_t mac_cken: 1;             //0xc[26]   1:mac_clk enable
				uint32_t phy_cken: 1;             //0xc[27]   1:phy_clk enable
				uint32_t jpeg_cken: 1;            //0xc[28]   1:jpeg_clk enable
				uint32_t disp_cken: 1;            //0xc[29]   1:disp_clk enable
				uint32_t aud_cken: 1;             //0xc[30]   1:aud_clk enable
				uint32_t wdt_cken: 1;             //0xc[31]   1:wdt_clk enable
			};
			uint32_t v;
		} cpu_device_clk_enable;

   /* REG_0xd */
	  uint32_t reserver_reg0d;

   /* REG_0xe */
		union {
			struct {
				uint32_t  aon_disckg: 1;              //0xe[0]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  sys_disckg: 1;              //0xe[1]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  dma_disckg: 1;              //0xe[2]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  flash_disckg: 1;            //0xe[3]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  wdt_disckg: 1;              //0xe[4]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  tim_disckg: 1;              //0xe[5]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  urt_disckg: 1;              //0xe[6]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  pwm_disckg: 1;              //0xe[7]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  i2c_disckg: 1;              //0xe[8]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  spi_disckg: 1;              //0xe[9]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  sadc_disckg: 1;             //0xe[10]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  efs_disckg: 1;              //0xe[11]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  irda_disckg: 1;             //0xe[12]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  trng_disckg: 1;             //0xe[13]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  sdio_disckg: 1;             //0xe[14]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  la_disckg: 1;               //0xe[15]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  tim1_disckg: 1;             //0xe[16]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  urt1_disckg: 1;             //0xe[17]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  urt2_disckg: 1;             //0xe[18]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  pwm1_disckg: 1;             //0xe[19]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  i2c1_disckg: 1;             //0xe[20]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  spi1_disckg: 1;             //0xe[21]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t usb_disckg: 1;               //0xe[22]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t can_disckg: 1;               //0xe[23]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t qspi0_disckg: 1;             //0xe[24]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t psram_disckg: 1;             //0xe[25]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  fft_disckg: 1;              //0xe[26]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  sbc_disckg: 1;              //0xe[27]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  aud_disckg: 1;              //0xe[28]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  i2s_disckg: 1;              //0xe[29]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  jpeg_disckg: 1;             //0xe[30]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  jpeg_dec_disckg: 1;         //0xe[31]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
			};
			uint32_t v;
		} cpu_mode_disckg1;

   /* REG_0xf */
	union {
		struct {
				uint32_t  disp_disckg: 1;             //0xf[0]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t  dma2d_disckg: 1;            //0xf[1]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t reserved1: 1;
				uint32_t btdm_disckg: 1;              //0xf[3]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t xver_disckg: 1;              //0xf[4]     BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t btdm_bps_disckg: 4;          //0xf[8:5]   BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
				uint32_t reserver2: 23;
		};
		uint32_t v;
		} cpu_mode_disckg2;

   /* REG_0x10 */
		union {
			struct {
				uint32_t pwd_mem1: 1;                   //0x10[0]    1:power on of mem1
				uint32_t pwd_mem2: 1;                   //0x10[1]    1:power on of mem2
				uint32_t pwd_mem3: 1;                   //0x10[2]    1:power on of mem3
				uint32_t pwd_encp: 1;                   //0x10[3]    1:power on of encp
				uint32_t pwd_bakp: 1;                   //0x10[4]    1:power on of bakp
				uint32_t pwd_ahbp: 1;                   //0x10[5]    1:power on of ahbp
				uint32_t pwd_audp: 1;                   //0x10[6]    1:power on of audp
				uint32_t pwd_vidp: 1;                   //0x10[7]    1:power on of vidp
				uint32_t pwd_btsp: 1;                   //0x10[8]    1:power on of btsp
				uint32_t pwd_wifp_mac: 1;               //0x10[9]    1:power on of wifp_mac
				uint32_t pwd_wifp_phy: 1;               //0x10[10]    1:power on of wifp_phy
				uint32_t sleep_en_need_flash_idle: 1;    //0x10[16]    1:sleep_en of flash_idle
				uint32_t sleep_en_need_cpu1_wifi: 1;     //0x10[17]    1:sleep_en of cpu1_wfi
				uint32_t sleep_en_need_cpu0_wfi: 1;     //0x10[18]    1:sleep_en of cpu0_wfi
				uint32_t sleep_en_global: 1;            //0x10[19]    1:sleep_en of global
				uint32_t wifi_wakeup_platform_en: 1;    //0x10[20]    1:wifi_wakeup_en
				uint32_t bts_wakeup_platform_en: 1;     //0x10[21]    1:bts_wakeup_en
				uint32_t bts_sleep_exit_req: 1;         //0x10[22]    1:bt sleep exit request
				uint32_t reserver: 9;
			};
			uint32_t v;
		} cpu_power_sleep_wakeup;

   /* REG_0x11 */
	  uint32_t reserver_reg11;
   /* REG_0x12 */
	  uint32_t reserver_reg12;
   /* REG_0x13 */
	  uint32_t reserver_reg13;
   /* REG_0x14 */
	  uint32_t reserver_reg14;
   /* REG_0x15 */
	  uint32_t reserver_reg15;
   /* REG_0x16 */
	  uint32_t reserver_reg16;
   /* REG_0x17 */
	  uint32_t reserver_reg17;
   /* REG_0x18 */
	  uint32_t reserver_reg18;
   /* REG_0x19 */
	  uint32_t reserver_reg19;
   /* REG_0x1a */
	  uint32_t reserver_reg1a;
   /* REG_0x1b */
	  uint32_t reserver_reg1b;
   /* REG_0x1c */
	  uint32_t reserver_reg1c;
   /* REG_0x1d */
	  uint32_t reserver_reg1d;
   /* REG_0x1e */
	  uint32_t reserver_reg1e;
   /* REG_0x1f */
	  uint32_t reserver_reg1f;

   /* REG_0x20 */
		union {
			struct {
				uint32_t cpu0_bmc32_int_en: 1;       //0x20[0]
				uint32_t cpu0_host_0_irq_en: 1;      //0x20[1]
				uint32_t cpu0_host_0_sec_irq_en: 1;  //0x20[2]
				uint32_t cpu0_timer_int_en: 1;       //0x20[3]
				uint32_t cpu0_uart_int_en: 1;        //0x20[4]
				uint32_t cpu0_pwm_int_en: 1;         //0x20[5]
				uint32_t cpu0_i2c_int_en: 1;         //0x20[6]
				uint32_t cpu0_spi_int_en: 1;         //0x20[7]
				uint32_t cpu0_sadc_int_en: 1;        //0x20[8]
				uint32_t cpu0_irda_int_en: 1;        //0x20[9]
				uint32_t cpu0_sdio_int_en: 1;        //0x20[10]
				uint32_t cpu0_gdma_int_en: 1;        //0x20[11]
				uint32_t cpu0_la_int_en: 1;          //0x20[12]
				uint32_t cpu0_timer1_int_en: 1;      //0x20[13]
				uint32_t cpu0_i2c1_int_en: 1;        //0x20[14]
				uint32_t cpu0_uart1_int_en: 1;       //0x20[15]
				uint32_t cpu0_uart2_int_en: 1;       //0x20[16]
				uint32_t cpu0_spi1_int_en: 1;        //0x20[17]
				uint32_t cpu0_can_int_en: 1;         //0x20[18]
				uint32_t cpu0_usb_int_en: 1;         //0x20[19]
				uint32_t cpu0_qspi_int_en: 1;        //0x20[20]
				uint32_t cpu0_fft_int_en: 1;         //0x20[21]
				uint32_t cpu0_sbc_int_en: 1;         //0x20[22]
				uint32_t cpu0_aud_int_en: 1;         //0x20[23]
				uint32_t cpu0_i2s_int_en: 1;         //0x20[24]
				uint32_t cpu0_jpegenc_int_en: 1;     //0x20[25]
				uint32_t cpu0_jpegdec_int_en: 1;     //0x20[26]
				uint32_t cpu0_lcd_int_en: 1;         //0x20[27]
				uint32_t reserved: 1;                //0x20[28]

				uint32_t cpu0_wifi_int_phy_en: 1;                //0x20[29]
				uint32_t cpu0_wifi_mac_int_tx_rx_timer_en: 1;     //0x20[30]
				uint32_t cpu0_wifi_mac_int_tx_rx_misc_en: 1;      //0x20[31]
//0x20[31:0]   int source , you can see SUMMARY for detailed description
			};
			uint32_t v;
		} cpu0_int_0_31_en;

   /* REG_0x21 */
		union {
			struct {
				uint32_t cpu0_wifi_mac_int_rx_trigger_en: 1;      //0x21[0]
				uint32_t cpu0_wifi_mac_int_tx_trigger_en: 1;      //0x21[1]
				uint32_t cpu0_wifi_mac_port_trigger_en: 1;        //0x21[2]
				uint32_t cpu0_wifi_mac_int_gen_en: 1;             //0x21[3]
				uint32_t cpu0_wifi_hsu_irq_en: 1;                 //0x21[4]
				uint32_t cpu0_wifi_int_mac_wakeup_en: 1;          //0x21[5]
				uint32_t reserved1: 1;                            //0x21[6]

				uint32_t cpu0_dm_irq_en: 1;                       //0x21[7]
				uint32_t cpu0_ble_irq_en: 1;                      //0x21[8]
				uint32_t cpu0_bt_irq_en: 1;                       //0x21[9]
				uint32_t reserved2: 5;                            //0x21[10:14]

				uint32_t cpu0_mbox0_int_en: 1;                    //0x21[15]
				uint32_t cpu0_mbox1_int_en: 1;                    //0x21[16]
				uint32_t cpu0_bmc64_int_en: 1;                    //0x21[17]
				uint32_t reserved3: 1;                            //0x21[18]

				uint32_t cpu0_touched_int_en: 1;                  //0x21[19]
				uint32_t cpu0_usbplug_int_en: 1;                  //0x21[20]
				uint32_t cpu0_rtc_int_en: 1;                      //0x21[21]
				uint32_t cpu0_gpio_int_en: 1;                     //0x21[22]
				uint32_t reserved4: 8;                            //0x21[23:31]
//0x21[31:0]   int source , you can see SUMMARY for detailed description
			};
			uint32_t v;
		} cpu0_int_32_63_en;

   /* REG_0x22 */
		union {
			struct {
				uint32_t cpu1_bmc32_int_en: 1;       //0x22[0]
				uint32_t cpu1_host_0_irq_en: 1;      //0x22[1]
				uint32_t cpu1_host_0_sec_irq_en: 1;  //0x22[2]
				uint32_t cpu1_timer_int_en: 1;       //0x22[3]
				uint32_t cpu1_uart_int_en: 1;        //0x22[4]
				uint32_t cpu1_pwm_int_en: 1;         //0x22[5]
				uint32_t cpu1_i2c_int_en: 1;         //0x22[6]
				uint32_t cpu1_spi_int_en: 1;         //0x22[7]
				uint32_t cpu1_sadc_int_en: 1;        //0x22[8]
				uint32_t cpu1_irda_int_en: 1;        //0x22[9]
				uint32_t cpu1_sdio_int_en: 1;        //0x22[10]
				uint32_t cpu1_gdma_int_en: 1;        //0x22[11]
				uint32_t cpu1_la_int_en: 1;          //0x22[12]
				uint32_t cpu1_timer1_int_en: 1;      //0x22[13]
				uint32_t cpu1_i2c1_int_en: 1;        //0x22[14]
				uint32_t cpu1_uart1_int_en: 1;       //0x22[15]
				uint32_t cpu1_uart2_int_en: 1;       //0x22[16]
				uint32_t cpu1_spi1_int_en: 1;        //0x22[17]
				uint32_t cpu1_can_int_en: 1;         //0x22[18]
				uint32_t cpu1_usb_int_en: 1;         //0x22[19]
				uint32_t cpu1_qspi_int_en: 1;        //0x22[20]
				uint32_t cpu1_fft_int_en: 1;         //0x22[21]
				uint32_t cpu1_sbc_int_en: 1;         //0x22[22]
				uint32_t cpu1_aud_int_en: 1;         //0x22[23]
				uint32_t cpu1_i2s_int_en: 1;         //0x22[24]
				uint32_t cpu1_jpegenc_int_en: 1;     //0x22[25]
				uint32_t cpu1_jpegdec_int_en: 1;     //0x22[26]
				uint32_t cpu1_lcd_int_en: 1;         //0x22[27]
				uint32_t reserved: 1;                //0x22[28]

				uint32_t cpu1_wifi_int_phy_en: 1;                //0x22[29]
				uint32_t cpu1_wifi_mac_int_tx_rx_timer_en: 1;     //0x22[30]
				uint32_t cpu1_wifi_mac_int_tx_rx_misc_en: 1;      //0x22[31]

//uint32_t cpu1_inten1:32;        //0x22[31:0]   int source , you can see SUMMARY for detailed description
			};
			uint32_t v;
		} cpu1_int_0_31_en;

   /* REG_0x23 */
		union {
			struct {

				uint32_t cpu1_wifi_mac_int_rx_trigger_en: 1;        //0x23[0]
				uint32_t cpu1_wifi_mac_int_tx_trigger_en: 1;        //0x23[1]
				uint32_t cpu1_wifi_mac_port_trigger_en: 1;          //0x23[2]
				uint32_t cpu1_wifi_mac_int_gen_en: 1;               //0x23[3]
				uint32_t cpu1_wifi_hsu_irq_en: 1;                   //0x23[4]
				uint32_t cpu1_wifi_int_mac_wakeup_en: 1;            //0x23[5]
				uint32_t reserved1: 1;                              //0x23[6]

				uint32_t cpu1_dm_irq_en: 1;                         //0x23[7]
				uint32_t cpu1_ble_irq_en: 1;                        //0x23[8]
				uint32_t cpu1_bt_irq_en: 1;                         //0x23[9]
				uint32_t reserved2: 5;                              //0x23[10:14]

				uint32_t cpu1_mbox0_int_en: 1;                      //0x23[15]
				uint32_t cpu1_mbox1_int_en: 1;                      //0x23[16]
				uint32_t cpu1_bmc64_int_en: 1;                      //0x23[17]
				uint32_t reserved3: 1;                              //0x23[18]

				uint32_t cpu1_touched_int_en: 1;                    //0x23[19]
				uint32_t cpu1_usbplug_int_en: 1;                    //0x23[20]
				uint32_t cpu1_rtc_int_en: 1;                        //0x23[21]
				uint32_t cpu1_gpio_int_en: 1;                       //0x23[22]
				uint32_t reserved4: 8;                              //0x23[23:31]

//uint32_t cpu1_inten2:32;            //0x23[31:0]   int source , you can see SUMMARY for detailed description
			};
			uint32_t v;
		} cpu1_int_32_63_en;

   /* REG_0x24 */
	  uint32_t reserver_reg24;
   /* REG_0x25 */
	  uint32_t reserver_reg25;
   /* REG_0x26 */
	  uint32_t reserver_reg26;
   /* REG_0x27 */
	  uint32_t reserver_reg27;

   /* REG_0x28 */
		union {
			struct {
				uint32_t cpu0_bmc32_int_st: 1;       //0x28[0]
				uint32_t cpu0_host_0_irq_st: 1;      //0x28[1]
				uint32_t cpu0_host_0_sec_irq_st: 1;  //0x28[2]
				uint32_t cpu0_timer_int_st: 1;       //0x28[3]
				uint32_t cpu0_uart_int_st: 1;        //0x28[4]
				uint32_t cpu0_pwm_int_st: 1;         //0x28[5]
				uint32_t cpu0_i2c_int_st: 1;         //0x28[6]
				uint32_t cpu0_spi_int_st: 1;         //0x28[7]
				uint32_t cpu0_sadc_int_st: 1;        //0x28[8]
				uint32_t cpu0_irda_int_st: 1;        //0x28[9]
				uint32_t cpu0_sdio_int_st: 1;        //0x28[10]
				uint32_t cpu0_gdma_int_st: 1;        //0x28[11]
				uint32_t cpu0_la_int_st: 1;          //0x28[12]
				uint32_t cpu0_timer1_int_st: 1;      //0x28[13]
				uint32_t cpu0_i2c1_int_st: 1;        //0x28[14]
				uint32_t cpu0_uart1_int_st: 1;       //0x28[15]
				uint32_t cpu0_uart2_int_st: 1;       //0x28[16]
				uint32_t cpu0_spi1_int_st: 1;        //0x28[17]
				uint32_t cpu0_can_int_st: 1;         //0x28[18]
				uint32_t cpu0_usb_int_st: 1;         //0x28[19]
				uint32_t cpu0_qspi_int_st: 1;        //0x28[20]
				uint32_t cpu0_fft_int_st: 1;         //0x28[21]
				uint32_t cpu0_sbc_int_st: 1;         //0x28[22]
				uint32_t cpu0_aud_int_st: 1;         //0x28[23]
				uint32_t cpu0_i2s_int_st: 1;         //0x28[24]
				uint32_t cpu0_jpegenc_int_st: 1;     //0x28[25]
				uint32_t cpu0_jpegdec_int_st: 1;     //0x28[26]
				uint32_t cpu0_lcd_int_st: 1;         //0x28[27]
				uint32_t reserved: 1;                //0x28[28]

				uint32_t cpu0_wifi_int_phy_st: 1;                //0x20[29]
				uint32_t cpu0_wifi_mac_int_tx_rx_timer_st: 1;     //0x20[30]
				uint32_t cpu0_wifi_mac_int_tx_rx_misc_st: 1;      //0x20[31]
//uint32_t cpu0_ints_status0:32;            //0x28[31:0]   int state,0: no interrupt 1: interrupt happen
			};
			uint32_t v;
		} cpu0_int_0_31_status;

   /* REG_0x29 */
		union {
			struct {
				uint32_t cpu0_wifi_mac_int_rx_trigger_st: 1;      //0x29[0]
				uint32_t cpu0_wifi_mac_int_tx_trigger_st: 1;      //0x29[1]
				uint32_t cpu0_wifi_mac_port_trigger_st: 1;        //0x29[2]
				uint32_t cpu0_wifi_mac_int_gen_st: 1;             //0x29[3]
				uint32_t cpu0_wifi_hsu_irq_st: 1;                 //0x29[4]
				uint32_t cpu0_wifi_int_mac_wakeup_st: 1;          //0x29[5]
				uint32_t reserved1: 1;                            //0x29[6]

				uint32_t cpu0_dm_irq_st: 1;                       //0x29[7]
				uint32_t cpu0_ble_irq_st: 1;                      //0x29[8]
				uint32_t cpu0_bt_irq_st: 1;                       //0x29[9]
				uint32_t reserved2: 5;                            //0x29[10:14]

				uint32_t cpu0_mbox0_int_st: 1;                    //0x29[15]
				uint32_t cpu0_mbox1_int_st: 1;                    //0x29[16]
				uint32_t cpu0_bmc64_int_st: 1;                    //0x29[17]
				uint32_t reserved3: 1;                            //0x29[18]

				uint32_t cpu0_touched_int_st: 1;                  //0x29[19]
				uint32_t cpu0_usbplug_int_st: 1;                  //0x29[20]
				uint32_t cpu0_rtc_int_st: 1;                      //0x29[21]
				uint32_t cpu0_gpio_int_st: 1;                     //0x29[22]
				uint32_t reserved4: 8;                            //0x29[23:31]
//uint32_t cpu0_ints_status1:32;        //0x29[31:0]   int state,0: no interrupt 1: interrupt happen
			};
			uint32_t v;
		} cpu0_int_32_63_status;

   /* REG_0x2a */
		union {
			struct {
				uint32_t cpu1_bmc32_int_st: 1;       //0x2a[0]
				uint32_t cpu1_host_0_irq_st: 1;      //0x2a[1]
				uint32_t cpu1_host_0_sec_irq_st: 1;  //0x2a[2]
				uint32_t cpu1_timer_int_st: 1;       //0x2a[3]
				uint32_t cpu1_uart_int_st: 1;        //0x2a[4]
				uint32_t cpu1_pwm_int_st: 1;         //0x2a[5]
				uint32_t cpu1_i2c_int_st: 1;         //0x2a[6]
				uint32_t cpu1_spi_int_st: 1;         //0x2a[7]
				uint32_t cpu1_sadc_int_st: 1;        //0x2a[8]
				uint32_t cpu1_irda_int_st: 1;        //0x2a[9]
				uint32_t cpu1_sdio_int_st: 1;        //0x2a[10]
				uint32_t cpu1_gdma_int_st: 1;        //0x2a[11]
				uint32_t cpu1_la_int_st: 1;          //0x2a[12]
				uint32_t cpu1_timer1_int_st: 1;      //0x2a[13]
				uint32_t cpu1_i2c1_int_st: 1;        //0x2a[14]
				uint32_t cpu1_uart1_int_st: 1;       //0x2a[15]
				uint32_t cpu1_uart2_int_st: 1;       //0x2a[16]
				uint32_t cpu1_spi1_int_st: 1;        //0x2a[17]
				uint32_t cpu1_can_int_st: 1;         //0x2a[18]
				uint32_t cpu1_usb_int_st: 1;         //0x2a[19]
				uint32_t cpu1_qspi_int_st: 1;        //0x2a[20]
				uint32_t cpu1_fft_int_st: 1;         //0x2a[21]
				uint32_t cpu1_sbc_int_st: 1;         //0x2a[22]
				uint32_t cpu1_aud_int_st: 1;         //0x2a[23]
				uint32_t cpu1_i2s_int_st: 1;         //0x2a[24]
				uint32_t cpu1_jpegenc_int_st: 1;     //0x2a[25]
				uint32_t cpu1_jpegdec_int_st: 1;     //0x2a[26]
				uint32_t cpu1_lcd_int_st: 1;         //0x2a[27]
				uint32_t reserved: 1;                //0x2a[28]

				uint32_t cpu1_wifi_int_phy_st: 1;                //0x2a[29]
				uint32_t cpu1_wifi_mac_int_tx_rx_timer_st: 1;     //0x2a[30]
				uint32_t cpu1_wifi_mac_int_tx_rx_misc_st: 1;      //0x2a[31]
//uint32_t cpu1_ints_status2:32;        //0x2a[31:0]   int state,0: no interrupt 1: interrupt happen
			};
			uint32_t v;
		} cpu1_int_0_31_status;

   /* REG_0x2b */
		union {
			struct {
				uint32_t cpu1_wifi_mac_int_rx_trigger_st: 1;        //0x2b[0]
				uint32_t cpu1_wifi_mac_int_tx_trigger_st: 1;        //0x2b[1]
				uint32_t cpu1_wifi_mac_port_trigger_st: 1;          //0x2b[2]
				uint32_t cpu1_wifi_mac_int_gen_st: 1;               //0x2b[3]
				uint32_t cpu1_wifi_hsu_irq_st: 1;                   //0x2b[4]
				uint32_t cpu1_wifi_int_mac_wakeup_st: 1;            //0x2b[5]
				uint32_t reserved1: 1;                              //0x2b[6]

				uint32_t cpu1_dm_irq_st: 1;                         //0x2b[7]
				uint32_t cpu1_ble_irq_st: 1;                        //0x2b[8]
				uint32_t cpu1_bt_irq_st: 1;                         //0x2b[9]
				uint32_t reserved2: 5;                              //0x2b[10:14]

				uint32_t cpu1_mbox0_int_st: 1;                      //0x2b[15]
				uint32_t cpu1_mbox1_int_st: 1;                      //0x2b[16]
				uint32_t cpu1_bmc64_int_st: 1;                      //0x2b[17]
				uint32_t reserved3: 1;                              //0x2b[18]

				uint32_t cpu1_touched_int_st: 1;                    //0x2b[19]
				uint32_t cpu1_usbplug_int_st: 1;                    //0x2b[20]
				uint32_t cpu1_rtc_int_st: 1;                        //0x2b[21]
				uint32_t cpu1_gpio_int_st: 1;                       //0x2b[22]
				uint32_t reserved4: 8;                              //0x2b[23:31]
//uint32_t cpu1_ints_status3:32;          //0x2b[31:0]   int state,0: no interrupt 1: interrupt happen
			};
			uint32_t v;
		} cpu1_int_32_63_status;

   /* REG_0x2c */
	  uint32_t reserver_reg2c;
   /* REG_0x2d */
	  uint32_t reserver_reg2d;
   /* REG_0x2e */
	  uint32_t reserver_reg2e;
   /* REG_0x2f */
	  uint32_t reserver_reg2f;

   /* REG_0x30 0x31 0x32 0x33 0x34 0x35*/
		union {
			struct {
				uint32_t system_gpio0:  4; //< bit[0:3] 0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5 R/W
				uint32_t system_gpio1:  4; //< bit[4:7] 0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5 R/W
				uint32_t system_gpio2:  4; //< bit[8:11] 0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5 R/W
				uint32_t system_gpio3:  4; //< bit[12:15] 0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5 R/W
				uint32_t system_gpio4:  4; //< bit[16:19] 0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5 R/W
				uint32_t system_gpio5:  4; //< bit[20:23] 0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5 R/W
				uint32_t system_gpio6:  4; //< bit[24:27] 0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5 R/W
				uint32_t system_gpio7:  4; //< bit[28:32] 0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5 R/W
			};
			uint32_t v;
		} system_gpio_config[SOC_SYSTEM_GPIO_FUNC_GROUP];

   /* REG_0x36 */
	  uint32_t reserver_reg36;
   /* REG_0x37 */
	  uint32_t reserver_reg37;

   /* REG_0x38 */
		union {
			struct {
				uint32_t dbug_cfg0:32;          //0x38[31:0]
			};
			uint32_t v;
		} system_debug_config0;

   /* REG_0x39 */
		union {
			struct {
				/* 0: btsp_debug[0:32]
				 * 1: btsp_debug[32+:32]
				 * 2: btsp_debug[64+:32]
				 * 4: btsp_debug[96+:6]
				 * 5: wifip_mac_dbg[31:0]
				 * 6: wifip_phy_dbg[31:0]
				 * default:  dbug_config0
				 */
				uint32_t dbug_cfg1: 4;         //0x39[3:0]
				uint32_t reserved: 28;        //0x39[4:31]
			};
			uint32_t v;
		} system_debug_config1;

   /* REG_0x40 */
	  uint32_t ana_reg0;
   /* REG_0x41 */
	  uint32_t ana_reg1;
   /* REG_0x42 */
	  uint32_t ana_reg2;
   /* REG_0x43 */
	  uint32_t ana_reg3;
   /* REG_0x44 */
	  uint32_t ana_reg4;
   /* REG_0x45 */
	  uint32_t ana_reg5;
   /* REG_0x46 */
	  uint32_t ana_reg6;
   /* REG_0x47 */
	  uint32_t ana_reg7;
   /* REG_0x48 */
	  uint32_t ana_reg8;
   /* REG_0x49 */
	  uint32_t ana_reg9;
   /* REG_0x4a */
	  uint32_t ana_rega;
   /* REG_0x4b */
	  uint32_t ana_regb;
   /* REG_0x4c */
	  uint32_t ana_regc;
   /* REG_0x4d */
	  uint32_t ana_regd;
   /* REG_0x4e */
	  uint32_t ana_rege;
   /* REG_0x4f */
	  uint32_t ana_regf;

} system_hw_t;

#ifdef __cplusplus
}
#endif

