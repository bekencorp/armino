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

/***********************************************************************************************************************************
* This file is generated from BK7256_ADDR Mapping_20211224_format_change_highlight_20220113_update.xlsm automatically                                
* Modify it manually is not recommended                                       
* CHIP ID:BK7256,GENARATE TIME:2022-03-17 20:29:38                                                 
************************************************************************************************************************************/

#pragma once                 
                            
#ifdef __cplusplus          
extern "C" {              
#endif                      

typedef volatile struct {

    /* REG_0x00 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  deviceid                       :32;  //0x0[31:0], ,0x53434647,RO
        }; 
        uint32_t v; 
    }device_id; 

    /* REG_0x01 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  versionid                      :32;  //0x1[31:0], ,0x72560001,RO
        }; 
        uint32_t v; 
    }version_id; 

    /* REG_0x02 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  core0_halted                   : 1;  //0x2[0],core0 halt indicate,0,RO
           volatile uint32_t  core1_halted                   : 1;  //0x2[1],core1 halt indicate,0,RO
           volatile uint32_t  reserved0                      : 2;  //0x2[3:2],NC,0,R
           volatile uint32_t  cpu0_sw_reset                  : 1;  //0x2[4],cpu0_sw_reset indicate,0,RO
           volatile uint32_t  cpu1_sw_reset                  : 1;  //0x2[5],cpu1_sw_reset indicate,0,RO
           volatile uint32_t  reserved1                      : 2;  //0x2[7:6],NC,0,R
           volatile uint32_t  cpu0_pwr_dw_state              : 1;  //0x2[8],cpu0_pwr_dw_state,0,RO
           volatile uint32_t  cpu1_pwr_dw_state              : 1;  //0x2[9],cpu1_pwr_dw_state,0,RO
           volatile uint32_t  reserved2                      :22;  //0x2[31:10],NC,0,R
        }; 
        uint32_t v; 
    }cpu_current_run_status; 

    /* REG_0x03 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  boot_mode                      : 1;  //0x3[0],0:ROM boot 1:FLASH boot,0,R/W
           volatile uint32_t  reserved0                      : 7;  //0x3[7:1],NC,0,R
           volatile uint32_t  jtag_core_sel                  : 1;  //0x3[8],0:jtag connect core0, 1:jtag connect core1,0,R/W
           volatile uint32_t  flash_sel                      : 1;  //0x3[9],0: normal flash operation 1:flash download by spi,0,R/W
           volatile uint32_t  reserved1                      :22;  //0x3[31:10],NC,0,R
        }; 
        uint32_t v; 
    }cpu_storage_connect_op_select; 

    /* REG_0x04 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  cpu0_sw_rst                    : 1;  //0x4[0],reserved,0,R/W
           volatile uint32_t  cpu0_pwr_dw                    : 1;  //0x4[1],reserved,0,R/W
           volatile uint32_t  cpu0_int_mask                  : 1;  //0x4[2],cpu0 int mask,0,R/W
           volatile uint32_t  cpu0_halt                      : 1;  //0x4[3],core0 halt indicate,0,R/W
           volatile uint32_t  cpu0_clk_div                   : 4;  //0x4[7:4],Frequency division : F/(1+N), N is the data of the reg value,0,R/W
           volatile uint32_t  cpu0_offset                    :24;  //0x4[31:8],reserved,0x0,RO
        }; 
        uint32_t v; 
    }cpu0_int_halt_clk_op; 

    /* REG_0x05 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  cpu1_sw_rst                    : 1;  //0x5[0],reserved,0,R/W
           volatile uint32_t  cpu1_pwr_dw                    : 1;  //0x5[1],reserved,0,R/W
           volatile uint32_t  cpu1_int_mask                  : 1;  //0x5[2],cpu1 int mask,0,R/W
           volatile uint32_t  cpu1_halt                      : 1;  //0x5[3],core1 halt indicate,0,R/W
           volatile uint32_t  cpu1_clk_div                   : 4;  //0x5[7:4],Frequency division : F/(1+N), N is the data of the reg value,0,R/W
           volatile uint32_t  cpu1_offset                    :24;  //0x5[31:8],reserved,0x0,R/W
        }; 
        uint32_t v; 
    }cpu1_int_halt_clk_op; 

    /* REG_0x06 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  reserved                       :32;  //0x6[31:0] , ,None,None
        }; 
        uint32_t v; 
    }reserved_reg0x6; 

    /* REG_0x07 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x7_reserved; 

    /* REG_0x08 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  clkdiv_core                    : 4;  //0x8[3:0],Frequency division : F/(1+N), N is the data of the reg value,0,R/W
           volatile uint32_t  cksel_core                     : 2;  //0x8[5:4],0:XTAL       1 : clk_DCO      2 : 320M      3 : 480M,0,R/W
           volatile uint32_t  clkdiv_bus                     : 1;  //0x8[6],Frequency division : F/(1+N), N is the data of the reg value,0,R/W
           volatile uint32_t  reserved0                      : 1;  //0x8[7],NC,0,R
           volatile uint32_t  clkdiv_uart0                   : 2;  //0x8[9:8],Frequency division :    0:/1  1:/2  2:/4  3:/8,0,R/W
           volatile uint32_t  clksel_uart0                   : 1;  //0x8[10],0:XTAL              1:APLL,0,R/W
           volatile uint32_t  clkdiv_uart1                   : 2;  //0x8[12:11],Frequency division :    0:/1  1:/2  2:/4  3:/8,0,R/W
           volatile uint32_t  cksel_uart1                    : 1;  //0x8[13],0:XTAL              1:APLL,0,R/W
           volatile uint32_t  clkdiv_uart2                   : 2;  //0x8[15:14],Frequency division :    0:/1  1:/2  2:/4  3:/8,0,R/W
           volatile uint32_t  cksel_uart2                    : 1;  //0x8[16],0:XTAL              1:APLL,0,R/W
           volatile uint32_t  cksel_sadc                     : 1;  //0x8[17],0:XTAL              1:APLL,0,R/W
           volatile uint32_t  cksel_pwm0                     : 1;  //0x8[18],0:clk32              1:XTAL,0,R/W
           volatile uint32_t  cksel_pwm1                     : 1;  //0x8[19],0:clk32              1:XTAL,0,R/W
           volatile uint32_t  cksel_timer0                   : 1;  //0x8[20],0:clk32              1:XTAL,0,R/W
           volatile uint32_t  cksel_timer1                   : 1;  //0x8[21],0:clk32              1:XTAL,0,R/W
           volatile uint32_t  cksel_timer2                   : 1;  //0x8[22],0:clk32              1:XTAL,0,R/W
           volatile uint32_t  reserved1                      : 1;  //0x8[23],NC,0,R
           volatile uint32_t  cksel_i2s                      : 1;  //0x8[24],0:XTAL              1:APLL,0,R/W
           volatile uint32_t  cksel_aud                      : 1;  //0x8[25],0:XTAL              1:APLL,0,R/W
           volatile uint32_t  clkdiv_jpeg                    : 4;  //0x8[29:26],Frequency division : F/(1+N), N is the data of the reg value,0,R/W
           volatile uint32_t  cksel_jpeg                     : 1;  //0x8[30],0:clk_320M      1:clk_480M,0,R/W
           volatile uint32_t  clkdiv_disp_l                  : 1;  //0x8[31],Frequency division : F/(1+clkdiv_disp_l+clkdiv_disp_h*2),0,R/W
        }; 
        uint32_t v; 
    }cpu_clk_div_mode1; 

    /* REG_0x09 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  clkdiv_disp_h                  : 3;  //0x9[2:0],Frequency division : F/(1+clkdiv_disp_l+clkdiv_disp_h*2),0,R/W
           volatile uint32_t  cksel_disp                     : 1;  //0x9[3],0:clk_320M      1:clk_480M,0,R/W
           volatile uint32_t  ckdiv_psram                    : 1;  //0x9[4],Frequency division : F/(1+N), N is the data of the reg value,0,R/W
           volatile uint32_t  cksel_psram                    : 1;  //0x9[5],0:clk_320M      1:clk_480M,0,R/W
           volatile uint32_t  ckdiv_qspi0                    : 4;  //0x9[9:6],Frequency division : F/(1+N), N is the data of the reg value,0,R/W
           volatile uint32_t  reserved0                      : 4;  //0x9[13:10], ,None,None
           volatile uint32_t  ckdiv_sdio                     : 3;  //0x9[16:14],0:/1  1:/2  2:/4  3:/8  4:/16  5:/32  6:/64  7:/256,0,R/W
           volatile uint32_t  cksel_sdio                     : 1;  //0x9[17],0：XTAL          1：320M,0,R/W
           volatile uint32_t  ckdiv_auxs                     : 4;  //0x9[21:18],Frequency division : F/(1+N), N is the data of the reg value,0,R/W
           volatile uint32_t  cksel_auxs                     : 2;  //0x9[23:22],0:DCO              1:APLL                2:320M                     4:480M,0,R/W
           volatile uint32_t  cksel_flash                    : 2;  //0x9[25:24],0:XTAL              1:APLL               1x :clk_120M,0,R/W
           volatile uint32_t  ckdiv_flash                    : 2;  //0x9[27:26],0:/1  1:/2  2:/4  3:/8,0,R/W
           volatile uint32_t  ckdiv_i2s0                     : 3;  //0x9[30:28],0:/1  1:/2  2:/4  3:/8  4:/16  5:/32  6:/64  7:/256,0,R/W
           volatile uint32_t  reserved1                      : 1;  //0x9[31],NC,0,R
        }; 
        uint32_t v; 
    }cpu_clk_div_mode2; 

    /* REG_0x0a */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ckdiv_26m                      : 2;  //0xa[1:0],0:/1  1:/2  2:/4  3:/8,0,R/W
           volatile uint32_t  ckdiv_wdt                      : 2;  //0xa[3:2],0:/2 1:/4 2:/8 3:/16,0,R/W
           volatile uint32_t  clksel_spi0                    : 1;  //0xa[4],0:XTAL              1:APLL,0,R/W
           volatile uint32_t  clksel_spi1                    : 1;  //0xa[5],0:XTAL              1:APLL,0,R/W
           volatile uint32_t  reserved                       :26;  //0xa[31:6],NC,0,R
        }; 
        uint32_t v; 
    }cpu_26m_wdt_clk_div; 

    /* REG_0x0b */
    union 
    { 
        struct 
        { 
           volatile uint32_t  anaspi_freq                    : 6;  //0xb[5:0], ,0,R/W
           volatile uint32_t  reserved0                      : 2;  //0xb[7:6],NC,0,R
           volatile uint32_t  anareg_state                   :20;  //0xb[27:8],analog register state:0x0: register write is idle;0x1: register write is busy; ,0,R
           volatile uint32_t  reserved                       : 4;  //0xb[31:28],NC,0,R
        }; 
        uint32_t v; 
    }cpu_anaspi_freq; 

    /* REG_0x0c */
    union 
    { 
        struct 
        { 
           volatile uint32_t  i2c0_cken                      : 1;  //0xc[0],1:i2c0_clk enable,0,R/W
           volatile uint32_t  spi0_cken                      : 1;  //0xc[1],1:spi0_clk enable ,0,R/W
           volatile uint32_t  uart0_cken                     : 1;  //0xc[2],1:uart0_clk enable,0,R/W
           volatile uint32_t  pwm0_cken                      : 1;  //0xc[3],1:pwm0_clk enable ,0,R/W
           volatile uint32_t  tim0_cken                      : 1;  //0xc[4],1:tim0_clk enable ,0,R/W
           volatile uint32_t  sadc_cken                      : 1;  //0xc[5],1:sadc_clk enable ,0,R/W
           volatile uint32_t  irda_cken                      : 1;  //0xc[6],1:irda_clk enable ,0,R/W
           volatile uint32_t  efuse_cken                     : 1;  //0xc[7],1:efuse_clk enable,0,R/W
           volatile uint32_t  i2c1_cken                      : 1;  //0xc[8],1:i2c1_clk enable ,0,R/W
           volatile uint32_t  spi1_cken                      : 1;  //0xc[9],1:spi1_clk enable ,0,R/W
           volatile uint32_t  uart1_cken                     : 1;  //0xc[10],1:uart1_clk enable,0,R/W
           volatile uint32_t  uart2_cken                     : 1;  //0xc[11],1:uart2_clk enable,0,R/W
           volatile uint32_t  pwm1_cken                      : 1;  //0xc[12],1:pwm1_clk enable ,0,R/W
           volatile uint32_t  tim1_cken                      : 1;  //0xc[13],1:tim1_clk enable ,0,R/W
           volatile uint32_t  tim2_cken                      : 1;  //0xc[14],1:tim2_clk enable ,0,R/W
           volatile uint32_t  otp_cken                       : 1;  //0xc[15],1:otp_clk enable  ,1,R/W
           volatile uint32_t  i2s_cken                       : 1;  //0xc[16],1:i2s_clk enable  ,0,R/W
           volatile uint32_t  usb_cken                       : 1;  //0xc[17],1:usb_clk enable  ,0,R/W
           volatile uint32_t  can_cken                       : 1;  //0xc[18],1:can_clk enable  ,0,R/W
           volatile uint32_t  psram_cken                     : 1;  //0xc[19],1:psram_clk enable,0,R/W
           volatile uint32_t  qspi0_cken                     : 1;  //0xc[20],1:qspi0_clk enable,0,R/W
           volatile uint32_t  qspi1_cken                     : 1;  //0xc[21],1:qspi1_clk enable,0,R/W
           volatile uint32_t  sdio_cken                      : 1;  //0xc[22],1:sdio_clk enable ,0,R/W
           volatile uint32_t  auxs_cken                      : 1;  //0xc[23],1:auxs_clk enable ,0,R/W
           volatile uint32_t  btdm_cken                      : 1;  //0xc[24],1:btdm_clk enable ,0,R/W
           volatile uint32_t  xvr_cken                       : 1;  //0xc[25],1:xvr_clk enable  ,0,R/W
           volatile uint32_t  mac_cken                       : 1;  //0xc[26],1:mac_clk enable  ,0,R/W
           volatile uint32_t  phy_cken                       : 1;  //0xc[27],1:phy_clk enable  ,0,R/W
           volatile uint32_t  jpeg_cken                      : 1;  //0xc[28],1:jpeg_clk enable ,0,R/W
           volatile uint32_t  disp_cken                      : 1;  //0xc[29],1:disp_clk enable ,0,R/W
           volatile uint32_t  aud_cken                       : 1;  //0xc[30],1:aud_clk enable  ,0,R/W
           volatile uint32_t  wdt_cken                       : 1;  //0xc[31],1:wdt_clk enable  ,0,R/W
        }; 
        uint32_t v; 
    }cpu_device_clk_enable; 

    /* REG_0x0d */
    union 
    { 
        struct 
        { 
           volatile uint32_t  reserved                       :32;  //0xd[31:0], ,None,None
        }; 
        uint32_t v; 
    }reserver_reg0xd; 

    /* REG_0x0e */
    union 
    { 
        struct 
        { 
           volatile uint32_t  aon_disckg                     : 1;  //0xe[0],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  sys_disckg                     : 1;  //0xe[1],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  dma_disckg                     : 1;  //0xe[2],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  flash_disckg                   : 1;  //0xe[3],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  wdt_disckg                     : 1;  //0xe[4],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  tim_disckg                     : 1;  //0xe[5],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  urt_disckg                     : 1;  //0xe[6],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  pwm_disckg                     : 1;  //0xe[7],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  i2c_disckg                     : 1;  //0xe[8],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  spi_disckg                     : 1;  //0xe[9],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  sadc_disckg                    : 1;  //0xe[10],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  efs_disckg                     : 1;  //0xe[11],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  irda_disckg                    : 1;  //0xe[12],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  trng_disckg                    : 1;  //0xe[13],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  sdio_disckg                    : 1;  //0xe[14],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  la_disckg                      : 1;  //0xe[15],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  tim1_disckg                    : 1;  //0xe[16],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  urt1_disckg                    : 1;  //0xe[17],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  urt2_disckg                    : 1;  //0xe[18],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  pwm1_disckg                    : 1;  //0xe[19],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  i2c1_disckg                    : 1;  //0xe[20],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  spi1_disckg                    : 1;  //0xe[21],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  usb_disckg                     : 1;  //0xe[22],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  can_disckg                     : 1;  //0xe[23],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  qspi0_disckg                   : 1;  //0xe[24],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  psram_disckg                   : 1;  //0xe[25],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  fft_disckg                     : 1;  //0xe[26],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  sbc_disckg                     : 1;  //0xe[27],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  aud_disckg                     : 1;  //0xe[28],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  i2s_disckg                     : 1;  //0xe[29],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  jpeg_disckg                    : 1;  //0xe[30],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  jpeg_dec_disckg                : 1;  //0xe[31],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
        }; 
        uint32_t v; 
    }cpu_mode_disckg1; 

    /* REG_0x0f */
    union 
    { 
        struct 
        { 
           volatile uint32_t  disp_disckg                    : 1;  //0xf[0],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  dma2d_disckg                   : 1;  //0xf[1],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  reserved0                      : 1;  //0xf[2],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  btdm_disckg                    : 1;  //0xf[3],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  xver_disckg                    : 1;  //0xf[4],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  btdm_bps_ckg                   : 4;  //0xf[8:5],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W
           volatile uint32_t  reserved1                      :23;  //0xf[31:9],NC,0,R
        }; 
        uint32_t v; 
    }cpu_mode_disckg2; 

    /* REG_0x10 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  pwd_mem1                       : 1;  //0x10[0],0:power on of mem1      ,0,RW
           volatile uint32_t  pwd_mem2                       : 1;  //0x10[1],0:power on of mem2      ,0,RW
           volatile uint32_t  pwd_mem3                       : 1;  //0x10[2],0:power on of mem3      ,0,RW
           volatile uint32_t  pwd_encp                       : 1;  //0x10[3],0:power on of encp      ,0,RW
           volatile uint32_t  pwd_bakp                       : 1;  //0x10[4],0:power on of bakp      ,0,RW
           volatile uint32_t  pwd_ahbp                       : 1;  //0x10[5],0:power on of ahbp      ,0,RW
           volatile uint32_t  pwd_audp                       : 1;  //0x10[6],0:power on of audp      ,0,RW
           volatile uint32_t  pwd_vidp                       : 1;  //0x10[7],0:power on of vidp      ,0,RW
           volatile uint32_t  pwd_btsp                       : 1;  //0x10[8],0:power on of btsp      ,0,RW
           volatile uint32_t  pwd_wifp_mac                   : 1;  //0x10[9],0:power on of wifp_mac  ,0,RW
           volatile uint32_t  pwd_wifp_phy                   : 1;  //0x10[10],0:power on of wifp_phy  ,0,RW
           volatile uint32_t  reserved                       : 5;  //0x10[15:11],NC,0,R
           volatile uint32_t  sleep_en_need_flash_idle       : 1;  //0x10[16],0:sleep_en of flash_idle,0,RW
           volatile uint32_t  sleep_en_need_cpu1_wfi         : 1;  //0x10[17],0:sleep_en of cpu1_wfi  ,0,RW
           volatile uint32_t  sleep_en_need_cpu0_wfi         : 1;  //0x10[18],0:sleep_en of cpu0_wfi  ,0,RW
           volatile uint32_t  sleep_en_global                : 1;  //0x10[19],0:sleep_en of global    ,0,RW
           volatile uint32_t  wifi_wakeup_platform_en        : 1;  //0x10[20],0:wifi_wakeup_en        ,0,RW
           volatile uint32_t  bts_wakeup_platform_en         : 1;  //0x10[21],0:bts_wakeup_en         ,0,RW
           volatile uint32_t  bts_sleep_exit_req             : 1;  //0x10[22],0:bt sleep exit request ,0,RW
           volatile uint32_t  reserved1                      : 9;  //0x10[31:23],NC,0,R
        }; 
        uint32_t v; 
    }cpu_power_sleep_wakeup; 

    /* REG_0x11 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  reserved                       :32;  //0x11[31:0], ,None,None
        }; 
        uint32_t v; 
    }reserver_reg0x11; 

    /* REG_0x12 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x12_reserved; 

    /* REG_0x13 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x13_reserved; 

    /* REG_0x14 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x14_reserved; 

    /* REG_0x15 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x15_reserved; 

    /* REG_0x16 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x16_reserved; 

    /* REG_0x17 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x17_reserved; 

    /* REG_0x18 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x18_reserved; 

    /* REG_0x19 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x19_reserved; 

    /* REG_0x1a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1a_reserved; 

    /* REG_0x1b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1b_reserved; 

    /* REG_0x1c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1c_reserved; 

    /* REG_0x1d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1d_reserved; 

    /* REG_0x1e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1e_reserved; 

    /* REG_0x1f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1f_reserved; 

    /* REG_0x20 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  cpu0_bmc32_int_en              : 1;  //0x20[0], ,0,R/W
           volatile uint32_t  cpu0_host_0_irq_en             : 1;  //0x20[1], ,0,R/W
           volatile uint32_t  cpu0_host_0_sec_irq_en         : 1;  //0x20[2], ,0,R/W
           volatile uint32_t  cpu0_timer_int_en              : 1;  //0x20[3], ,0,R/W
           volatile uint32_t  cpu0_uart_int_en               : 1;  //0x20[4], ,0,R/W
           volatile uint32_t  cpu0_pwm_int_en                : 1;  //0x20[5], ,0,R/W
           volatile uint32_t  cpu0_i2c_int_en                : 1;  //0x20[6], ,0,R/W
           volatile uint32_t  cpu0_spi_int_en                : 1;  //0x20[7], ,0,R/W
           volatile uint32_t  cpu0_sadc_int_en               : 1;  //0x20[8], ,0,R/W
           volatile uint32_t  cpu0_irda_int_en               : 1;  //0x20[9], ,0,R/W
           volatile uint32_t  cpu0_sdio_int_en               : 1;  //0x20[10], ,0,R/W
           volatile uint32_t  cpu0_gdma_int_en               : 1;  //0x20[11], ,0,R/W
           volatile uint32_t  cpu0_la_int_en                 : 1;  //0x20[12], ,0,R/W
           volatile uint32_t  cpu0_timer1_int_en             : 1;  //0x20[13], ,0,R/W
           volatile uint32_t  cpu0_i2c1_int_en               : 1;  //0x20[14], ,0,R/W
           volatile uint32_t  cpu0_uart1_int_en              : 1;  //0x20[15], ,0,R/W
           volatile uint32_t  cpu0_uart2_int_en              : 1;  //0x20[16], ,0,R/W
           volatile uint32_t  cpu0_spi1_int_en               : 1;  //0x20[17], ,0,R/W
           volatile uint32_t  cpu0_can_int_en                : 1;  //0x20[18], ,0,R/W
           volatile uint32_t  cpu0_usb_int_en                : 1;  //0x20[19], ,0,R/W
           volatile uint32_t  cpu0_qspi_int_en               : 1;  //0x20[20], ,0,R/W
           volatile uint32_t  cpu0_fft_int_en                : 1;  //0x20[21], ,0,R/W
           volatile uint32_t  cpu0_sbc_int_en                : 1;  //0x20[22], ,0,R/W
           volatile uint32_t  cpu0_aud_int_en                : 1;  //0x20[23], ,0,R/W
           volatile uint32_t  cpu0_i2s_int_en                : 1;  //0x20[24], ,0,R/W
           volatile uint32_t  cpu0_jpegenc_int_en            : 1;  //0x20[25], ,0,R/W
           volatile uint32_t  cpu0_jpegdec_int_en            : 1;  //0x20[26], ,0,R/W
           volatile uint32_t  cpu0_lcd_int_en                : 1;  //0x20[27], ,0,R/W
           volatile uint32_t  cpu0_dma2d_int_en              : 1;  //0x20[28], ,0,R/W
           volatile uint32_t  cpu0_wifi_int_phy_mpb_en       : 1;  //0x20[29], ,0,R/W
           volatile uint32_t  cpu0_wifi_int_phy_riu_en       : 1;  //0x20[30], ,0,R/W
           volatile uint32_t  cpu0_wifi_mac_int_tx_rx_timer_en : 1;  //0x20[31], ,0,R/W
        }; 
        uint32_t v; 
    }cpu0_int_0_31_en; 

    /* REG_0x21 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  cpu0_wifi_mac_int_tx_rx_misc_en : 1;  //0x21[0], ,0,R/W
           volatile uint32_t  cpu0_wifi_mac_int_rx_trigger_en : 1;  //0x21[1], ,0,R/W
           volatile uint32_t  cpu0_wifi_mac_int_tx_trigger_en : 1;  //0x21[2], ,0,R/W
           volatile uint32_t  cpu0_wifi_mac_int_prot_trigger_en : 1;  //0x21[3], ,0,R/W
           volatile uint32_t  cpu0_wifi_mac_int_gen_en       : 1;  //0x21[4], ,0,R/W
           volatile uint32_t  cpu0_wifi_hsu_irq_en           : 1;  //0x21[5], ,0,R/W
           volatile uint32_t  cpu0_wifi_int_mac_wakeup_en    : 1;  //0x21[6], ,0,R/W
           volatile uint32_t  cpu0_dm_irq_en                 : 1;  //0x21[7], ,0,R/W
           volatile uint32_t  cpu0_ble_irq_en                : 1;  //0x21[8], ,0,R/W
           volatile uint32_t  cpu0_bt_irq_en                 : 1;  //0x21[9], ,0,R/W
           volatile uint32_t  reserved2                      : 6;  //0x21[15:10], ,0,R/W
           volatile uint32_t  cpu0_mbox0_int_en              : 1;  //0x21[16], ,0,R/W
           volatile uint32_t  cpu0_mbox1_int_en              : 1;  //0x21[17], ,0,R/W
           volatile uint32_t  cpu0_bmc64_int_en              : 1;  //0x21[18], ,0,R/W
           volatile uint32_t  reserved3                      : 1;  //0x21[19], ,0,R/W
           volatile uint32_t  cpu0_touched_int_en            : 1;  //0x21[20], ,0,R/W
           volatile uint32_t  cpu0_usbplug_int_en            : 1;  //0x21[21], ,0,R/W
           volatile uint32_t  cpu0_rtc_int_en                : 1;  //0x21[22], ,0,R/W
           volatile uint32_t  cpu0_gpio_int_en               : 1;  //0x21[23], ,0,R/W
           volatile uint32_t  reserved4                      : 8;  //0x21[31:24], ,0,R/W
        }; 
        uint32_t v; 
    }cpu0_int_32_63_en; 

    /* REG_0x22 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  cpu1_bmc32_int_en              : 1;  //0x22[0], ,0,R/W
           volatile uint32_t  cpu1_host_0_irq_en             : 1;  //0x22[1], ,0,R/W
           volatile uint32_t  cpu1_host_0_sec_irq_en         : 1;  //0x22[2], ,0,R/W
           volatile uint32_t  cpu1_timer_int_en              : 1;  //0x22[3], ,0,R/W
           volatile uint32_t  cpu1_uart_int_en               : 1;  //0x22[4], ,0,R/W
           volatile uint32_t  cpu1_pwm_int_en                : 1;  //0x22[5], ,0,R/W
           volatile uint32_t  cpu1_i2c_int_en                : 1;  //0x22[6], ,0,R/W
           volatile uint32_t  cpu1_spi_int_en                : 1;  //0x22[7], ,0,R/W
           volatile uint32_t  cpu1_sadc_int_en               : 1;  //0x22[8], ,0,R/W
           volatile uint32_t  cpu1_irda_int_en               : 1;  //0x22[9], ,0,R/W
           volatile uint32_t  cpu1_sdio_int_en               : 1;  //0x22[10], ,0,R/W
           volatile uint32_t  cpu1_gdma_int_en               : 1;  //0x22[11], ,0,R/W
           volatile uint32_t  cpu1_la_int_en                 : 1;  //0x22[12], ,0,R/W
           volatile uint32_t  cpu1_timer1_int_en             : 1;  //0x22[13], ,0,R/W
           volatile uint32_t  cpu1_i2c1_int_en               : 1;  //0x22[14], ,0,R/W
           volatile uint32_t  cpu1_uart1_int_en              : 1;  //0x22[15], ,0,R/W
           volatile uint32_t  cpu1_uart2_int_en              : 1;  //0x22[16], ,0,R/W
           volatile uint32_t  cpu1_spi1_int_en               : 1;  //0x22[17], ,0,R/W
           volatile uint32_t  cpu1_can_int_en                : 1;  //0x22[18], ,0,R/W
           volatile uint32_t  cpu1_usb_int_en                : 1;  //0x22[19], ,0,R/W
           volatile uint32_t  cpu1_qspi_int_en               : 1;  //0x22[20], ,0,R/W
           volatile uint32_t  cpu1_fft_int_en                : 1;  //0x22[21], ,0,R/W
           volatile uint32_t  cpu1_sbc_int_en                : 1;  //0x22[22], ,0,R/W
           volatile uint32_t  cpu1_aud_int_en                : 1;  //0x22[23], ,0,R/W
           volatile uint32_t  cpu1_i2s_int_en                : 1;  //0x22[24], ,0,R/W
           volatile uint32_t  cpu1_jpegenc_int_en            : 1;  //0x22[25], ,0,R/W
           volatile uint32_t  cpu1_jpegdec_int_en            : 1;  //0x22[26], ,0,R/W
           volatile uint32_t  cpu1_lcd_int_en                : 1;  //0x22[27], ,0,R/W
           volatile uint32_t  cpu1_dma2d_int_en              : 1;  //0x22[28], ,0,R/W
           volatile uint32_t  cpu1_wifi_int_phy_mpb_en       : 1;  //0x22[29], ,0,R/W
           volatile uint32_t  cpu1_wifi_int_phy_riu_en       : 1;  //0x22[30], ,0,R/W
           volatile uint32_t  cpu1_wifi_mac_int_tx_rx_timer_en : 1;  //0x22[31], ,0,R/W
        }; 
        uint32_t v; 
    }cpu1_int_0_31_en; 

    /* REG_0x23 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  cpu1_wifi_mac_int_tx_rx_misc_en : 1;  //0x23[0], ,0,R/W
           volatile uint32_t  cpu1_wifi_mac_int_rx_trigger_en : 1;  //0x23[1], ,0,R/W
           volatile uint32_t  cpu1_wifi_mac_int_tx_trigger_en : 1;  //0x23[2], ,0,R/W
           volatile uint32_t  cpu1_wifi_mac_int_prot_trigger_en : 1;  //0x23[3], ,0,R/W
           volatile uint32_t  cpu1_wifi_mac_int_gen_en       : 1;  //0x23[4], ,0,R/W
           volatile uint32_t  cpu1_wifi_hsu_irq_en           : 1;  //0x23[5], ,0,R/W
           volatile uint32_t  cpu1_wifi_int_mac_wakeup_en    : 1;  //0x23[6], ,0,R/W
           volatile uint32_t  cpu1_dm_irq_en                 : 1;  //0x23[7], ,0,R/W
           volatile uint32_t  cpu1_ble_irq_en                : 1;  //0x23[8], ,0,R/W
           volatile uint32_t  cpu1_bt_irq_en                 : 1;  //0x23[9], ,0,R/W
           volatile uint32_t  reserved2                      : 6;  //0x23[15:10], ,0,R/W
           volatile uint32_t  cpu1_mbox0_int_en              : 1;  //0x23[16], ,0,R/W
           volatile uint32_t  cpu1_mbox1_int_en              : 1;  //0x23[17], ,0,R/W
           volatile uint32_t  cpu1_bmc64_int_en              : 1;  //0x23[18], ,0,R/W
           volatile uint32_t  reserved3                      : 1;  //0x23[19], ,0,R/W
           volatile uint32_t  cpu1_touched_int_en            : 1;  //0x23[20], ,0,R/W
           volatile uint32_t  cpu1_usbplug_int_en            : 1;  //0x23[21], ,0,R/W
           volatile uint32_t  cpu1_rtc_int_en                : 1;  //0x23[22], ,0,R/W
           volatile uint32_t  cpu1_gpio_int_en               : 1;  //0x23[23], ,0,R/W
           volatile uint32_t  reserved4                      : 8;  //0x23[31:24], ,0,R/W
        }; 
        uint32_t v; 
    }cpu1_int_32_63_en; 

    /* REG_0x24 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x24_reserved; 

    /* REG_0x25 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x25_reserved; 

    /* REG_0x26 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x26_reserved; 

    /* REG_0x27 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x27_reserved; 

    /* REG_0x28 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  cpu0_bmc32_int_st              : 1;  //0x28[0], ,0,R
           volatile uint32_t  cpu0_host_0_irq_st             : 1;  //0x28[1], ,0,R
           volatile uint32_t  cpu0_host_0_sec_irq_st         : 1;  //0x28[2], ,0,R
           volatile uint32_t  cpu0_timer_int_st              : 1;  //0x28[3], ,0,R
           volatile uint32_t  cpu0_uart_int_st               : 1;  //0x28[4], ,0,R
           volatile uint32_t  cpu0_pwm_int_st                : 1;  //0x28[5], ,0,R
           volatile uint32_t  cpu0_i2c_int_st                : 1;  //0x28[6], ,0,R
           volatile uint32_t  cpu0_spi_int_st                : 1;  //0x28[7], ,0,R
           volatile uint32_t  cpu0_sadc_int_st               : 1;  //0x28[8], ,0,R
           volatile uint32_t  cpu0_irda_int_st               : 1;  //0x28[9], ,0,R
           volatile uint32_t  cpu0_sdio_int_st               : 1;  //0x28[10], ,0,R
           volatile uint32_t  cpu0_gdma_int_st               : 1;  //0x28[11], ,0,R
           volatile uint32_t  cpu0_la_int_st                 : 1;  //0x28[12], ,0,R
           volatile uint32_t  cpu0_timer1_int_st             : 1;  //0x28[13], ,0,R
           volatile uint32_t  cpu0_i2c1_int_st               : 1;  //0x28[14], ,0,R
           volatile uint32_t  cpu0_uart1_int_st              : 1;  //0x28[15], ,0,R
           volatile uint32_t  cpu0_uart2_int_st              : 1;  //0x28[16], ,0,R
           volatile uint32_t  cpu0_spi1_int_st               : 1;  //0x28[17], ,0,R
           volatile uint32_t  cpu0_can_int_st                : 1;  //0x28[18], ,0,R
           volatile uint32_t  cpu0_usb_int_st                : 1;  //0x28[19], ,0,R
           volatile uint32_t  cpu0_qspi_int_st               : 1;  //0x28[20], ,0,R
           volatile uint32_t  cpu0_fft_int_st                : 1;  //0x28[21], ,0,R
           volatile uint32_t  cpu0_sbc_int_st                : 1;  //0x28[22], ,0,R
           volatile uint32_t  cpu0_aud_int_st                : 1;  //0x28[23], ,0,R
           volatile uint32_t  cpu0_i2s_int_st                : 1;  //0x28[24], ,0,R
           volatile uint32_t  cpu0_jpegenc_int_st            : 1;  //0x28[25], ,0,R
           volatile uint32_t  cpu0_jpegdec_int_st            : 1;  //0x28[26], ,0,R
           volatile uint32_t  cpu0_lcd_int_st                : 1;  //0x28[27], ,0,R
           volatile uint32_t  reserved                       : 1;  //0x28[28], ,0,R
           volatile uint32_t  cpu0_wifi_int_phy_mpb_st       : 1;  //0x28[29], ,0,R
           volatile uint32_t  cpu0_wifi_int_phy_riu_st       : 1;  //0x28[30], ,0,R
           volatile uint32_t  cpu0_wifi_mac_int_tx_rx_timer_st : 1;  //0x28[31], ,0,R
        }; 
        uint32_t v; 
    }cpu0_int_0_31_status; 

    /* REG_0x29 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  cpu0_wifi_mac_int_tx_rx_misc_st : 1;  //0x29[0], ,0,R
           volatile uint32_t  cpu0_wifi_mac_int_rx_trigger_st : 1;  //0x29[1], ,0,R
           volatile uint32_t  cpu0_wifi_mac_int_tx_trigger_st : 1;  //0x29[2], ,0,R
           volatile uint32_t  cpu0_wifi_mac_int_prot_trigger_st : 1;  //0x29[3], ,0,R
           volatile uint32_t  cpu0_wifi_mac_int_gen_st       : 1;  //0x29[4], ,0,R
           volatile uint32_t  cpu0_wifi_hsu_irq_st           : 1;  //0x29[5], ,0,R
           volatile uint32_t  cpu0_wifi_int_mac_wakeup_st    : 1;  //0x29[6], ,0,R
           volatile uint32_t  cpu0_dm_irq_st                 : 1;  //0x29[7], ,0,R
           volatile uint32_t  cpu0_ble_irq_st                : 1;  //0x29[8], ,0,R
           volatile uint32_t  cpu0_bt_irq_st                 : 1;  //0x29[9], ,0,R
           volatile uint32_t  reserved2                      : 6;  //0x29[15:10], ,0,R
           volatile uint32_t  cpu0_mbox0_int_st              : 1;  //0x29[16], ,0,R
           volatile uint32_t  cpu0_mbox1_int_st              : 1;  //0x29[17], ,0,R
           volatile uint32_t  cpu0_bmc64_int_st              : 1;  //0x29[18], ,0,R
           volatile uint32_t  reserved3                      : 1;  //0x29[19], ,0,R
           volatile uint32_t  cpu0_touched_int_st            : 1;  //0x29[20], ,0,R
           volatile uint32_t  cpu0_usbplug_int_st            : 1;  //0x29[21], ,0,R
           volatile uint32_t  cpu0_rtc_int_st                : 1;  //0x29[22], ,0,R
           volatile uint32_t  cpu0_gpio_int_st               : 1;  //0x29[23], ,0,R
           volatile uint32_t  reserved4                      : 8;  //0x29[31:24], ,0,R
        }; 
        uint32_t v; 
    }cpu0_int_32_63_status; 

    /* REG_0x2a */
    union 
    { 
        struct 
        { 
           volatile uint32_t  cpu1_bmc32_int_st              : 1;  //0x2a[0], ,0,R
           volatile uint32_t  cpu1_host_0_irq_st             : 1;  //0x2a[1], ,0,R
           volatile uint32_t  cpu1_host_0_sec_irq_st         : 1;  //0x2a[2], ,0,R
           volatile uint32_t  cpu1_timer_int_st              : 1;  //0x2a[3], ,0,R
           volatile uint32_t  cpu1_uart_int_st               : 1;  //0x2a[4], ,0,R
           volatile uint32_t  cpu1_pwm_int_st                : 1;  //0x2a[5], ,0,R
           volatile uint32_t  cpu1_i2c_int_st                : 1;  //0x2a[6], ,0,R
           volatile uint32_t  cpu1_spi_int_st                : 1;  //0x2a[7], ,0,R
           volatile uint32_t  cpu1_sadc_int_st               : 1;  //0x2a[8], ,0,R
           volatile uint32_t  cpu1_irda_int_st               : 1;  //0x2a[9], ,0,R
           volatile uint32_t  cpu1_sdio_int_st               : 1;  //0x2a[10], ,0,R
           volatile uint32_t  cpu1_gdma_int_st               : 1;  //0x2a[11], ,0,R
           volatile uint32_t  cpu1_la_int_st                 : 1;  //0x2a[12], ,0,R
           volatile uint32_t  cpu1_timer1_int_st             : 1;  //0x2a[13], ,0,R
           volatile uint32_t  cpu1_i2c1_int_st               : 1;  //0x2a[14], ,0,R
           volatile uint32_t  cpu1_uart1_int_st              : 1;  //0x2a[15], ,0,R
           volatile uint32_t  cpu1_uart2_int_st              : 1;  //0x2a[16], ,0,R
           volatile uint32_t  cpu1_spi1_int_st               : 1;  //0x2a[17], ,0,R
           volatile uint32_t  cpu1_can_int_st                : 1;  //0x2a[18], ,0,R
           volatile uint32_t  cpu1_usb_int_st                : 1;  //0x2a[19], ,0,R
           volatile uint32_t  cpu1_qspi_int_st               : 1;  //0x2a[20], ,0,R
           volatile uint32_t  cpu1_fft_int_st                : 1;  //0x2a[21], ,0,R
           volatile uint32_t  cpu1_sbc_int_st                : 1;  //0x2a[22], ,0,R
           volatile uint32_t  cpu1_aud_int_st                : 1;  //0x2a[23], ,0,R
           volatile uint32_t  cpu1_i2s_int_st                : 1;  //0x2a[24], ,0,R
           volatile uint32_t  cpu1_jpegenc_int_st            : 1;  //0x2a[25], ,0,R
           volatile uint32_t  cpu1_jpegdec_int_st            : 1;  //0x2a[26], ,0,R
           volatile uint32_t  cpu1_lcd_int_st                : 1;  //0x2a[27], ,0,R
           volatile uint32_t  reserved                       : 1;  //0x2a[28], ,0,R
           volatile uint32_t  cpu1_wifi_int_phy_mpb_st       : 1;  //0x2a[29], ,0,R
           volatile uint32_t  cpu1_wifi_int_phy_riu_st       : 1;  //0x2a[30], ,0,R
           volatile uint32_t  cpu1_wifi_mac_int_tx_rx_timer_st : 1;  //0x2a[31], ,0,R
        }; 
        uint32_t v; 
    }cpu1_int_0_31_status; 

    /* REG_0x2b */
    union 
    { 
        struct 
        { 
           volatile uint32_t  cpu1_wifi_mac_int_tx_rx_misc_st : 1;  //0x2b[0], ,0,R
           volatile uint32_t  cpu1_wifi_mac_int_rx_trigger_st : 1;  //0x2b[1], ,0,R
           volatile uint32_t  cpu1_wifi_mac_int_tx_trigger_st : 1;  //0x2b[2], ,0,R
           volatile uint32_t  cpu1_wifi_mac_int_prot_trigger_st : 1;  //0x2b[3], ,0,R
           volatile uint32_t  cpu1_wifi_mac_int_gen_st       : 1;  //0x2b[4], ,0,R
           volatile uint32_t  cpu1_wifi_hsu_irq_st           : 1;  //0x2b[5], ,0,R
           volatile uint32_t  cpu1_wifi_int_mac_wakeup_st    : 1;  //0x2b[6], ,0,R
           volatile uint32_t  cpu1_dm_irq_st                 : 1;  //0x2b[7], ,0,R
           volatile uint32_t  cpu1_ble_irq_st                : 1;  //0x2b[8], ,0,R
           volatile uint32_t  cpu1_bt_irq_st                 : 1;  //0x2b[9], ,0,R
           volatile uint32_t  reserved2                      : 6;  //0x2b[15:10], ,0,R
           volatile uint32_t  cpu1_mbox0_int_st              : 1;  //0x2b[16], ,0,R
           volatile uint32_t  cpu1_mbox1_int_st              : 1;  //0x2b[17], ,0,R
           volatile uint32_t  cpu1_bmc64_int_st              : 1;  //0x2b[18], ,0,R
           volatile uint32_t  reserved3                      : 1;  //0x2b[19], ,0,R
           volatile uint32_t  cpu1_touched_int_st            : 1;  //0x2b[20], ,0,R
           volatile uint32_t  cpu1_usbplug_int_st            : 1;  //0x2b[21], ,0,R
           volatile uint32_t  cpu1_rtc_int_st                : 1;  //0x2b[22], ,0,R
           volatile uint32_t  cpu1_gpio_int_st               : 1;  //0x2b[23], ,0,R
           volatile uint32_t  reserved4                      : 8;  //0x2b[31:24], ,0,R
        }; 
        uint32_t v; 
    }cpu1_int_32_63_status; 

    /* REG_0x2c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x2c_reserved; 

    /* REG_0x2d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x2d_reserved; 

    /* REG_0x2e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x2e_reserved; 

    /* REG_0x2f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x2f_reserved; 

    /* REG_0x30 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sys_gpio0                      : 4;  //0x30[3:0],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio1                      : 4;  //0x30[7:4],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio2                      : 4;  //0x30[11:8],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio3                      : 4;  //0x30[15:12],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio4                      : 4;  //0x30[19:16],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio5                      : 4;  //0x30[23:20],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio6                      : 4;  //0x30[27:24],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio7                      : 4;  //0x30[31:28],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
        }; 
        uint32_t v; 
    }gpio_config0; 

    /* REG_0x31 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sys_gpio8                      : 4;  //0x31[3:0],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio9                      : 4;  //0x31[7:4],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio10                     : 4;  //0x31[11:8],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio11                     : 4;  //0x31[15:12],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio12                     : 4;  //0x31[19:16],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio13                     : 4;  //0x31[23:20],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio14                     : 4;  //0x31[27:24],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio15                     : 4;  //0x31[31:28],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
        }; 
        uint32_t v; 
    }gpio_config1; 

    /* REG_0x32 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sys_gpio16                     : 4;  //0x32[3:0],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio17                     : 4;  //0x32[7:4],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio18                     : 4;  //0x32[11:8],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio19                     : 4;  //0x32[15:12],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio20                     : 4;  //0x32[19:16],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio21                     : 4;  //0x32[23:20],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio22                     : 4;  //0x32[27:24],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio23                     : 4;  //0x32[31:28],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
        }; 
        uint32_t v; 
    }gpio_config2; 

    /* REG_0x33 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sys_gpio24                     : 4;  //0x33[3:0],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio25                     : 4;  //0x33[7:4],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio26                     : 4;  //0x33[11:8],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio27                     : 4;  //0x33[15:12],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio28                     : 4;  //0x33[19:16],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio29                     : 4;  //0x33[23:20],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio30                     : 4;  //0x33[27:24],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio31                     : 4;  //0x33[31:28],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
        }; 
        uint32_t v; 
    }gpio_config3; 

    /* REG_0x34 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sys_gpio32                     : 4;  //0x34[3:0],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio33                     : 4;  //0x34[7:4],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio34                     : 4;  //0x34[11:8],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio35                     : 4;  //0x34[15:12],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio36                     : 4;  //0x34[19:16],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio37                     : 4;  //0x34[23:20],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio38                     : 4;  //0x34[27:24],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio39                     : 4;  //0x34[31:28],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
        }; 
        uint32_t v; 
    }gpio_config4; 

    /* REG_0x35 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sys_gpio40                     : 4;  //0x35[3:0],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio41                     : 4;  //0x35[7:4],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio42                     : 4;  //0x35[11:8],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio43                     : 4;  //0x35[15:12],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio44                     : 4;  //0x35[19:16],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio45                     : 4;  //0x35[23:20],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio46                     : 4;  //0x35[27:24],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
           volatile uint32_t  sys_gpio47                     : 4;  //0x35[31:28],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W
        }; 
        uint32_t v; 
    }gpio_config5; 

    /* REG_0x36 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x36_reserved; 

    /* REG_0x37 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x37_reserved; 

    /* REG_0x38 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  dbug_config0                   :32;  //0x38[31:0], ,0,R/W
        }; 
        uint32_t v; 
    }sys_debug_config0; 

    /* REG_0x39 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  dbug_config1                   :32;  //0x39[31:0],0: btsp_debug[0:32]        1: btsp_debug[32+:32]           2: btsp_debug[64+:32]  4:btsp_debug[96+:6]       5:wifip_mac_dbg[31:0]           6: wifip_phy_dbg[31:0]                            default:  dbug_config0                   ,0,R/W
        }; 
        uint32_t v; 
    }sys_debug_config1; 

    /* REG_0x3a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x3a_reserved; 

    /* REG_0x3b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x3b_reserved; 

    /* REG_0x3c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x3c_reserved; 

    /* REG_0x3d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x3d_reserved; 

    /* REG_0x3e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x3e_reserved; 

    /* REG_0x3f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x3f_reserved; 

    /* REG_0x40 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ck2652sel                      : 1;  //0x40[0],1:26MHz/0:52MHz,0,R/W
           volatile uint32_t  cp                             : 3;  //0x40[3:1],cp curent control 0to 350uA 50uA step,2,R/W
           volatile uint32_t  spideten                       : 1;  //0x40[4],unlock detect enable fron spi 1:enable,1,R/W
           volatile uint32_t  hvref                          : 2;  //0x40[6:5],high vth control for unlock detect 00:0.85V;01:0.9V;10:0.95V;11:1.05V,2,R/W
           volatile uint32_t  lvref                          : 2;  //0x40[8:7],low vth control for unlock detect 00:0.2V;01:0.3V;10:0.35V;11:0.4V,2,R/W
           volatile uint32_t  rzctrl26m                      : 1;  //0x40[9],Rz ctrl in 26M mode:1:normal;0:add 14K,0,R/W
           volatile uint32_t  looprzctrl                     : 4;  //0x40[13:10],Rz ctrl:2K to 17K,1K step,9,R/W
           volatile uint32_t  rpc                            : 2;  //0x40[15:14],second pole Rp ctrl:00:30K;01:10K;10:22K;11:2K,2,R/W
           volatile uint32_t  nsyn                           : 1;  //0x40[16],N divider rst,1,R/W
           volatile uint32_t  cksel                          : 2;  //0x40[18:17],0:26M;1:40M;2:24M;3:19.2M,0,R/W
           volatile uint32_t  spitrig                        : 1;  //0x40[19],SPI band selection trigger signal,0,R/W
           volatile uint32_t  band                           : 5;  //0x40[24:20],band manual value/band[0] ,0,R/W
           volatile uint32_t  bandmanual                     : 1;  //0x40[25],1:band manual;0:band auto,0,R/W
           volatile uint32_t  dsptrig                        : 1;  //0x40[26],band selection trigger signal,0,R/W
           volatile uint32_t  lpen_dpll                      : 1;  //0x40[27],dpll low power mode enable,0,R/W
           volatile uint32_t  xamp                           : 4;  //0x40[31:28],xtal OSC amp control/xamp<0> shared with pll_cktst_en,0xF,R/W
        }; 
        uint32_t v; 
    }ana_reg0; 

    /* REG_0x41 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  nc                             : 1;  //0x41[0],NC,0,R/W
           volatile uint32_t  dpll_vrefsel                   : 1;  //0x41[1],dpll ldo reference voltage selection  0:vbg_aon/1:vbg_cal,0,R/W
           volatile uint32_t  msw                            : 9;  //0x41[10:2],set the frequency of DCO manual,70,R/W
           volatile uint32_t  ictrl                          : 3;  //0x41[13:11],controlling the bias cuttent of DCO core,1,R/W
           volatile uint32_t  osc_trig                       : 1;  //0x41[14],reset the DCO core by spi to make it oscillate again,0,R/W
           volatile uint32_t  osccal_trig                    : 1;  //0x41[15],trigger the action of callibration in the DCO,0,R/W
           volatile uint32_t  cnti                           : 9;  //0x41[24:16],set the controlling work of calibration in the DCO block to get the different frequency,C0,R/W
           volatile uint32_t  spi_rst                        : 1;  //0x41[25],reset the calibration block of DCO by spi,0,R/W
           volatile uint32_t  amsel                          : 1;  //0x41[26],disable the calibration function of the DCO,set the frequency of DCO manual,1,R/W
           volatile uint32_t  divctrl                        : 3;  //0x41[29:27],controlling the value of divider in the DCO to get the different frequency,7,R/W
           volatile uint32_t  dco_tsten                      : 1;  //0x41[30],dco test enable,0,R/W
           volatile uint32_t  rosc_tsten                     : 1;  //0x41[31],rosc test enable,0,R/W
        }; 
        uint32_t v; 
    }ana_reg1; 

    /* REG_0x42 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  pwmscmen                       : 1;  //0x42[0],buck nmos disable,0,R/W
           volatile uint32_t  buck_fasten                    : 1;  //0x42[1],buck EA fast transient enable(=1),1,R/W
           volatile uint32_t  cls                            : 3;  //0x42[4:2],buck current limit setting,7,R/W
           volatile uint32_t  pfms                           : 5;  //0x42[9:5],buck freewheeling damping enable(=1) ,13,R/W
           volatile uint32_t  ripc                           : 3;  //0x42[12:10],buck pfm mode voltage ripple control setting,6,R/W
           volatile uint32_t  rampc                          : 4;  //0x42[16:13],buck ramping compensation setting,7,R/W
           volatile uint32_t  rampcen                        : 1;  //0x42[17],buck ramping compensation enable(=1),1,R/W
           volatile uint32_t  dpfmen                         : 1;  //0x42[18],buck pfm mode current reduce enable(=1),1,R/W
           volatile uint32_t  pfmen                          : 1;  //0x42[19],buck pfm mode enable(=1),1,R/W
           volatile uint32_t  forcepfm                       : 1;  //0x42[20],buck force pfm mode(=1),0,R/W
           volatile uint32_t  swrsten                        : 1;  //0x42[21],buck freewheeling damping enable(=1) ,1,R/W
           volatile uint32_t  tmposel                        : 2;  //0x42[23:22],buck mpo pulse width control 0--shortest   3---longest,2,R/W
           volatile uint32_t  mpoen                          : 1;  //0x42[24],buck mpo mode enable( =1),1,R/W
           volatile uint32_t  spi_latchb                     : 1;  //0x42[25],spi latch disable 0:latch;1:no latch,0,R/W
           volatile uint32_t  ldosel                         : 1;  //0x42[26],ldo/buck select, 0:buck;1:LDO,1,R/W
           volatile uint32_t  iovoc                          : 3;  //0x42[29:27],ioldo output voltage select 0:2.9V,….7:3.6V,4,R/W
           volatile uint32_t  vbpbuf_hp                      : 1;  //0x42[30],vbspbuffer high power enable,1,R/W
           volatile uint32_t  bypassen                       : 1;  //0x42[31],ioldo bypass enable,0,R/W
        }; 
        uint32_t v; 
    }ana_reg2; 

    /* REG_0x43 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  zcdta                          : 5;  //0x43[4:0],buck zcd delay tune setting,1F,R/W
           volatile uint32_t  zcdcala                        : 6;  //0x43[10:5],buck zcd offset cali setting,E,R/W
           volatile uint32_t  zcdmen                         : 1;  //0x43[11],buck zcd manual cali enable(=1),0,R/W
           volatile uint32_t  zcdcalen                       : 1;  //0x43[12],buck zcd calibration enable(=1),0,R/W
           volatile uint32_t  zcdcal_tri                     : 1;  //0x43[13],buck zcd auto cali triggle(0-->1),0,R/W
           volatile uint32_t  mroscsel                       : 1;  //0x43[14],buck oscillator manual cali. enable(=1),0,R/W
           volatile uint32_t  mfsel                          : 3;  //0x43[17:15],buck oscillator manual fsel  ,1,R/W
           volatile uint32_t  mroscbcal                      : 4;  //0x43[21:18],buck oscillator manual cap_cal  0xA---500k 0xB--1M 0x9---2M,6,R/W
           volatile uint32_t  osccaltrig                     : 1;  //0x43[22],buck oscillator manual cali. enable(=1),0,R/W
           volatile uint32_t  ckintsel                       : 1;  //0x43[23],buck clock source select  1-- ring oscillator   0--divider,1,R/W
           volatile uint32_t  ckfs                           : 2;  //0x43[25:24],buck output clock freq. select   0--500k 1---1M  2--2M  3--4M,1,R/W
           volatile uint32_t  vlsel_ldodig                   : 3;  //0x43[28:26],digldo output voltage select(low power)  0:0.6V,…..7:1.4V,4,R/W
           volatile uint32_t  vhsel_ldodig                   : 3;  //0x43[31:29],digldo output voltage select(high power)  0:0.6V,…..7:1.4V,4,R/W
        }; 
        uint32_t v; 
    }ana_reg3; 

    /* REG_0x44 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  nc                             : 5;  //0x44[4:0],Reserved,3,R/W
           volatile uint32_t  cb_manu_val                    : 5;  //0x44[9:5],CB Calibration Manual Value,10,R/W
           volatile uint32_t  cb_cal_trig                    : 1;  //0x44[10],CB Calibration Trigger,0,R/W
           volatile uint32_t  cb_cal_manu                    : 1;  //0x44[11],CB Calibration Manual Mode ,1,R/W
           volatile uint32_t  rosc_cal_intval                : 3;  //0x44[14:12],Rosc Calibration Interlval 0.25s~2s,4,R/W
           volatile uint32_t  manu_cin                       : 7;  //0x44[21:15],Rosc Calibration Manual Cin,40,R/W
           volatile uint32_t  manu_fin                       : 5;  //0x44[26:22],Rosc Calibration Manual Fin,10,R/W
           volatile uint32_t  rosc_cal_mode                  : 1;  //0x44[27],Rosc Calibration Mode:; 0x1: 32K; 0x0: 31.25K,0,R/W
           volatile uint32_t  rosc_cal_trig                  : 1;  //0x44[28],Rosc Calibration Trigger,0,R/W
           volatile uint32_t  rosc_cal_en                    : 1;  //0x44[29],Rosc Calibration Enable,1,R/W
           volatile uint32_t  rosc_manu_en                   : 1;  //0x44[30],Rosc Calibration Manual Mode ,1,R/W
           volatile uint32_t  rosc_tsten                     : 1;  //0x44[31],Rosc test enable,0,R/W
        }; 
        uint32_t v; 
    }ana_reg4; 

    /* REG_0x45 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  vref_scale                     : 1;  //0x45[0],gadc reference voltage scale enable,0,R/W
           volatile uint32_t  dccal_en                       : 1;  //0x45[1],gadc DC calibration enable,0,R/W
           volatile uint32_t  xtalh_ctune                    : 7;  //0x45[8:2],xtalh load cap tuning,0,R/W
           volatile uint32_t  cktst_sel                      : 2;  //0x45[10:9],clock test signal selection rosc/xtall/dco/dpll,0,R/W
           volatile uint32_t  ck_tst_enbale                  : 1;  //0x45[11],system clock test enable,0,R/W
           volatile uint32_t  trxt_tst_enable                : 1;  //0x45[12],wifi trx test enable,0,R/W
           volatile uint32_t  encb                           : 1;  //0x45[13],global central bias enable,1,R/W
           volatile uint32_t  vctrl_dpllldo                  : 2;  //0x45[15:14],dpll ldo output selection,1,R/W
           volatile uint32_t  vctrl_sysldo                   : 2;  //0x45[17:16],sys ldo output selection,3,R/W
           volatile uint32_t  temptst_en                     : 1;  //0x45[18],tempdet test enable,0,R/W
           volatile uint32_t  gadc_tsel                      : 3;  //0x45[21:19],gadc test signal selection,0,R/W
           volatile uint32_t  xtalh_ictrl                    : 1;  //0x45[22],xtalh current control,0,R/W
           volatile uint32_t  bgcalm                         : 6;  //0x45[28:23],bandgap calibration manual setting,20,R/W
           volatile uint32_t  bgcal_trig                     : 1;  //0x45[29],bandgap calibrarion trig,0,R/W
           volatile uint32_t  bgcal_manu                     : 1;  //0x45[30],bandgap calibration manual mode enable,1,R/W
           volatile uint32_t  bgcal_en                       : 1;  //0x45[31],bandgap calibration enable,0,R/W
        }; 
        uint32_t v; 
    }ana_reg5; 

    /* REG_0x46 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  itune_xtall                    : 4;  //0x46[3:0],xtall core current control,7,R/W
           volatile uint32_t  xtall_ten                      : 1;  //0x46[4],xtall test enable,0,R/W
           volatile uint32_t  psldo_vsel                     : 1;  //0x46[5],ps ldo output voltage selection,0:VIO /1:1.8V,0,R/W
           volatile uint32_t  en_usb                         : 1;  //0x46[6],usb phy enable,0,R/W
           volatile uint32_t  en_xtall                       : 1;  //0x46[7],xtall oscillator enable,0,R/W
           volatile uint32_t  en_dco                         : 1;  //0x46[8],dco enable,0,R/W
           volatile uint32_t  en_psram_ldo                   : 1;  //0x46[9],psram ldo enable,0,R/W
           volatile uint32_t  en_tempdet                     : 1;  //0x46[10],tempreture det enable,0,R/W
           volatile uint32_t  en_audpll                      : 1;  //0x46[11],audio pll enable,0,R/W
           volatile uint32_t  en_dpll                        : 1;  //0x46[12],dpll enable,0,R/W
           volatile uint32_t  en_sysldo                      : 1;  //0x46[13],sysldo enable,1,R/W
           volatile uint32_t  nc                             : 3;  //0x46[16:14],reserved,4,R/W
           volatile uint32_t  pwd_gadc_buf                   : 1;  //0x46[17],gadc input buffer pwd,1,R/W
           volatile uint32_t  xtal_hpsrr_en                  : 1;  //0x46[18],xtal high psrr buffer enable,1,R/W
           volatile uint32_t  en_xtal2rf                     : 1;  //0x46[19],xtal clock to rfpll gate enable ,0,R/W
           volatile uint32_t  en_sleep                       : 1;  //0x46[20],xtal sleep enable,0,R/W
           volatile uint32_t  clkbuf_hd                      : 1;  //0x46[21],xtal lpsrr clock buffer high power mode ,1,R/W
           volatile uint32_t  clkbuf_dsel_manu               : 1;  //0x46[22],xtal lpsrr clock buffer power mode selection 0: auto /1:manu ,1,R/W
           volatile uint32_t  xtal_lpmode_ctrl               : 1;  //0x46[23],xtal core low power mode enable,1,R/W
           volatile uint32_t  rxtal_lp                       : 4;  //0x46[27:24],xtal bias current setting at low power mode ,F,R/W
           volatile uint32_t  rxtal_hp                       : 4;  //0x46[31:28],xtal26m bias current setting at high power mode ,F,R/W
        }; 
        uint32_t v; 
    }ana_reg6; 

    /* REG_0x47 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  rng_tstck_sel                  : 1;  //0x47[0],trng setting,0,R/W
           volatile uint32_t  rng_tsten                      : 1;  //0x47[1],trng setting,0,R/W
           volatile uint32_t  itune_ref                      : 3;  //0x47[4:2],trng setting,4,R/W
           volatile uint32_t  itune_opa                      : 3;  //0x47[7:5],trng setting,7,R/W
           volatile uint32_t  itune_cmp                      : 3;  //0x47[10:8],trng setting,7,R/W
           volatile uint32_t  rnooise_sel                    : 1;  //0x47[11],trng setting,0,R/W
           volatile uint32_t  fslow_sel                      : 3;  //0x47[14:12],trng setting,2,R/W
           volatile uint32_t  ffast_sel                      : 4;  //0x47[18:15],trng setting,8,R/W
           volatile uint32_t  gadc_cal_sel                   : 2;  //0x47[20:19],gadc calibration mode selection,0,R/W
           volatile uint32_t  gadc_ten                       : 1;  //0x47[21],gadc test enable,0,R/W
           volatile uint32_t  gadc_cmp_ictrl                 : 4;  //0x47[25:22],gadc comparaor current select ,8,R/W
           volatile uint32_t  gadc_buf_ictrl                 : 4;  //0x47[29:26],gadc buffer current select ,8,R/W
           volatile uint32_t  vref_sel                       : 1;  //0x47[30],gadc input reference select, 0：bandgap signal 1:GPIO voltage divided,0,R/W
           volatile uint32_t  scal_en                        : 1;  //0x47[31],gadc reference scale enable, 0:normal mode,1: scale mode ,1,R/W
        }; 
        uint32_t v; 
    }ana_reg7; 

    /* REG_0x48 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  cap_calspi                     : 9;  //0x48[8:0],manul mode ,input cap calibretion value,0,R/W
           volatile uint32_t  gain_s                         : 2;  //0x48[10:9],Sensitivity level selection,1,R/W
           volatile uint32_t  pwd_td                         : 1;  //0x48[11],power down touch module,1,R/W
           volatile uint32_t  en_fsr                         : 1;  //0x48[12],low power mode ,enable fast response,0,R/W
           volatile uint32_t  en_scm                         : 1;  //0x48[13],scan mode enable,0,R/W
           volatile uint32_t  en_adcmode                     : 1;  //0x48[14],adc mode enable,0,R/W
           volatile uint32_t  en_lpmode                      : 1;  //0x48[15],low power mode enable,0,R/W
           volatile uint32_t  chs_scan                       :16;  //0x48[31:16],scan mode chan selection,0,R/W
        }; 
        uint32_t v; 
    }ana_reg8; 

    /* REG_0x49 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  en_otp_spi                     : 1;  //0x49[0],otp ldo spi enable,0,R/W
           volatile uint32_t  nc3                            :11;  //0x49[11:1],reserved,3F8,R/W
           volatile uint32_t  nc2                            : 1;  //0x49[12],reserved,0,R/W
           volatile uint32_t  digovr_en                      : 1;  //0x49[13],digldo over voltage reset enable,0,R/W
           volatile uint32_t  usbpen                         : 4;  //0x49[17:14],usb dp driver capability control,8,R/W
           volatile uint32_t  usbnen                         : 4;  //0x49[21:18],usb dn driver capability control,8,R/W
           volatile uint32_t  usb_speed                      : 1;  //0x49[22],usb speed selection,0,R/W
           volatile uint32_t  usb_deepsleep                  : 1;  //0x49[23],usb deepsleep mode enable by spi,0,R/W
           volatile uint32_t  man_mode                       : 1;  //0x49[24],manul mode enable,0,R/W
           volatile uint32_t  crg                            : 2;  //0x49[26:25],detect range selection :8pF/12pF/19pF/27pF,2,R/W
           volatile uint32_t  vrefs                          : 3;  //0x49[29:27],detect threshold selection ,6,R/W
           volatile uint32_t  nc1                            : 1;  //0x49[30],reserved,0,R/W
           volatile uint32_t  en_cal                         : 1;  //0x49[31],calibretion enable,0,R/W
        }; 
        uint32_t v; 
    }ana_reg9; 

    /* REG_0x4a */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sdm_val                        :30;  //0x4a[29:0],audio pll sdm value,0F1FAA45,R/W
           volatile uint32_t  vco_hfreq_enb                  : 1;  //0x4a[30],audio pll vco high frequency enb,0,R/W
           volatile uint32_t  cal_refen                      : 1;  //0x4a[31],cal_ref enable of audio pll,1,R/W
        }; 
        uint32_t v; 
    }ana_reg10; 

    /* REG_0x4b */
    union 
    { 
        struct 
        { 
           volatile uint32_t  int_mod                        : 1;  //0x4b[0],DPLL integer mode enable; 0: fractional mode; 1: integer mode,0,R/W
           volatile uint32_t  nsyn                           : 1;  //0x4b[1],DPLL Ncoutner reset ,0,R/W
           volatile uint32_t  open_enb                       : 1;  //0x4b[2], ,0,R/W
           volatile uint32_t  reset                          : 1;  //0x4b[3],DPLL reset,0,R/W
           volatile uint32_t  ioffset                        : 3;  //0x4b[6:4],DPLL  charge pump offset current control,5,R/W
           volatile uint32_t  lpfrz                          : 4;  //0x4b[10:7],DPLL Rz control of LPF,6,R/W
           volatile uint32_t  vsel                           : 3;  //0x4b[13:11],DPLL vtrl selection during VCO band calibration,2,R/W
           volatile uint32_t  vsel_cal                       : 1;  //0x4b[14], selection during VCO band calibration,0,R/W
           volatile uint32_t  pwd_lockdet                    : 1;  //0x4b[15], ,1,R/W
           volatile uint32_t  lockdet_bypass                 : 1;  //0x4b[16], ,0,R/W
           volatile uint32_t  ckref_loop_sel                 : 1;  //0x4b[17],polarity selection of referenc clock  to SDM,0,R/W
           volatile uint32_t  spi_trigger                    : 1;  //0x4b[18],DPLL band calibration spi trigger,0,R/W
           volatile uint32_t  manual                         : 1;  //0x4b[19],DPLL VCO band manual enable; 0: auto mode; 1: manual mode,0,R/W
           volatile uint32_t  test_en                        : 1;  //0x4b[20],test enable,1,R/W
           volatile uint32_t  nc                             : 1;  //0x4b[21],NC; ,1,R/W
           volatile uint32_t  icp                            : 2;  //0x4b[23:22],DPLL charge pump current control; ,3,R/W
           volatile uint32_t  ck26men                        : 1;  //0x4b[24],xtal26M clock for audio enable,0,R/W
           volatile uint32_t  ckaudio_outen                  : 1;  //0x4b[25],DPLL clock output to PAD enable,0,R/W
           volatile uint32_t  divctrl                        : 3;  //0x4b[28:26],DPLL divider control; 000: div1; 001: div2; 010: div4; 011: div8; 1xx: div16,0,R/W
           volatile uint32_t  cksel                          : 1;  //0x4b[29],DPLL divider control; 0: div3; 1: div4,1,R/W
           volatile uint32_t  ck2mcu                         : 1;  //0x4b[30],DPLL clock for mcu enable,0,R/W
           volatile uint32_t  audioen                        : 1;  //0x4b[31],DPLL clock for audio enable,0,R/W
        }; 
        uint32_t v; 
    }ana_reg11; 

    /* REG_0x4c */
    union 
    { 
        struct 
        { 
           volatile uint32_t  nc                             : 2;  //0x4c[1:0],NC,0,R/W
           volatile uint32_t  digmic_ckinv                   : 1;  //0x4c[2],digmic clock inversion enable,0,R/W
           volatile uint32_t  enmicdig                       : 1;  //0x4c[3],digmic enable,0,R/W
           volatile uint32_t  audck_rlcen                    : 1;  //0x4c[4],audio clock re-latch enable,0,R/W
           volatile uint32_t  lchckinven                     : 1;  //0x4c[5],audio clock re-latch clock inversion enable,0,R/W
           volatile uint32_t  ldo1v_vsel1v                   : 3;  //0x4c[8:6],audio 1.0V LDO selection, 000=0.8, 1X1=1.0,0,R/W
           volatile uint32_t  ldo1v_adj                      : 5;  //0x4c[13:9],audio 1.0V LDO output trimming, 00000=min, 11111=max,0,R/W
           volatile uint32_t  audvdd_trm1v                   : 2;  //0x4c[15:14],audio 1.5V LDO selection, 00=min, 11=max,0,R/W
           volatile uint32_t  audvdd_voc1v                   : 5;  //0x4c[20:16],audio 1.5V LDO output trimming, 00000=min, 11111=max,0,R/W
           volatile uint32_t  enaudvdd1v                     : 1;  //0x4c[21],audio 1.0V LDO enable,0,R/W
           volatile uint32_t  loadhp                         : 1;  //0x4c[22],audio 1.5V LDO, 1=good stability with small loading,0,R/W
           volatile uint32_t  enaudvdd1v5                    : 1;  //0x4c[23],audio 1.5V LDO enable,0,R/W
           volatile uint32_t  enmicbias1v                    : 1;  //0x4c[24],micbias enable,0,R/W
           volatile uint32_t  micbias_trim                   : 2;  //0x4c[26:25],micbias output selection, 00=min, 11=max,0,R/W
           volatile uint32_t  micbias_voc1v                  : 5;  //0x4c[31:27],micbias output trimming, 00000=min, 11111=max,0,R/W
        }; 
        uint32_t v; 
    }ana_reg12; 

    /* REG_0x4d */
    union 
    { 
        struct 
        { 
           volatile uint32_t  nc5                            : 8;  //0x4d[7:0],NC,0,R/W
           volatile uint32_t  byp_dwaadc                     : 1;  //0x4d[8],adc dwa pass enable,0,R/W
           volatile uint32_t  rst                            : 1;  //0x4d[9],rst,0,R/W
           volatile uint32_t  adcdwa_mode                    : 1;  //0x4d[10],adc dwa model sel,0,R/W
           volatile uint32_t  vodadjspi                      : 5;  //0x4d[15:11],adc reference manual spi control,10,R/W
           volatile uint32_t  nc4                            : 5;  //0x4d[20:16],NC,0,R/W
           volatile uint32_t  refvsel                        : 1;  //0x4d[21],0= high reference; 1=small reference,0,R/W
           volatile uint32_t  nc3                            : 1;  //0x4d[22],NC,0,R/W
           volatile uint32_t  capsw1v                        : 5;  //0x4d[27:23],munual value for cap trimming,0,R/W
           volatile uint32_t  nc2                            : 2;  //0x4d[29:28],NC,0,R/W
           volatile uint32_t  adcckinven                     : 1;  //0x4d[30],audio adc clock inversion enable,0,R/W
           volatile uint32_t  nc1                            : 1;  //0x4d[31],NC,0,R/W
        }; 
        uint32_t v; 
    }ana_reg13; 

    /* REG_0x4e */
    union 
    { 
        struct 
        { 
           volatile uint32_t  isel                           : 2;  //0x4e[1:0],adc bias trimming,0,R/W
           volatile uint32_t  micdcocdin                     : 8;  //0x4e[9:2],adc micmode dcoc din,0,R/W
           volatile uint32_t  micdcocvc                      : 2;  //0x4e[11:10],adc micmode dcoc control,0,R/W
           volatile uint32_t  micdcocen_n                    : 1;  //0x4e[12],adc micmode dcoc enable,0,R/W
           volatile uint32_t  micdcocen_p                    : 1;  //0x4e[13],adc micmode dcoc enable,0,R/W
           volatile uint32_t  micsingleen                    : 1;  //0x4e[14],adc micmode, single_end enable,0,R/W
           volatile uint32_t  micgain                        : 4;  //0x4e[18:15],adc micmode gain, 0=0dB(17.9K), F=24dB(267.6K), 2dB/step,0,R/W
           volatile uint32_t  micdacen                       : 1;  //0x4e[19],adc micmode micdac enable,0,R/W
           volatile uint32_t  micdacih                       : 8;  //0x4e[27:20],adc micmode, micdac input ,0,R/W
           volatile uint32_t  micdacit                       : 2;  //0x4e[29:28],adc micmode, mic_dac Iout Full-range, 00=280uA, 01=320uA, 1X=447uA,0,R/W
           volatile uint32_t  hcen                           : 1;  //0x4e[30],adc 1stg op current trimming,0,R/W
           volatile uint32_t  micen                          : 1;  //0x4e[31],mic1 mode enable,0,R/W
        }; 
        uint32_t v; 
    }ana_reg14; 

    /* REG_0x4f */
    union 
    { 
        struct 
        { 
           volatile uint32_t  isel                           : 2;  //0x4f[1:0],adc bias trimming,0,R/W
           volatile uint32_t  micdcocdin                     : 8;  //0x4f[9:2],adc micmode dcoc din,0,R/W
           volatile uint32_t  micdcocvc                      : 2;  //0x4f[11:10],adc micmode dcoc control,0,R/W
           volatile uint32_t  micdcocen_n                    : 1;  //0x4f[12],adc micmode dcoc enable,0,R/W
           volatile uint32_t  micdcocen_p                    : 1;  //0x4f[13],adc micmode dcoc enable,0,R/W
           volatile uint32_t  micsingleen                    : 1;  //0x4f[14],adc micmode, single_end enable,0,R/W
           volatile uint32_t  micgain                        : 4;  //0x4f[18:15],adc micmode gain, 0=0dB(17.9K), F=24dB(267.6K), 2dB/step,0,R/W
           volatile uint32_t  micdacen                       : 1;  //0x4f[19],adc micmode micdac enable,0,R/W
           volatile uint32_t  micdacih                       : 8;  //0x4f[27:20],adc micmode, micdac input ,0,R/W
           volatile uint32_t  micdacit                       : 2;  //0x4f[29:28],adc micmode, mic_dac Iout Full-range, 00=280uA, 01=320uA, 1X=447uA,0,R/W
           volatile uint32_t  hcen                           : 1;  //0x4f[30],adc 1stg op current trimming,0,R/W
           volatile uint32_t  micen                          : 1;  //0x4f[31],mic2 mode enable,0,R/W
        }; 
        uint32_t v; 
    }ana_reg15; 

    /* REG_0x50 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  hpdac                          : 1;  //0x50[0],class ab driver high current mode. "1" high current. ,0,R/W
           volatile uint32_t  vcmsdac                        : 1;  //0x50[1],1stg OP input common model voltage selection. "1" low common mode voltage,0,R/W
           volatile uint32_t  oscdac                         : 2;  //0x50[3:2],threshold current setting for over current protection . "3" maximum current. "0" minimum current,0,R/W
           volatile uint32_t  ocendac                        : 1;  //0x50[4],over current protection enable. "1" enable.,0,R/W
           volatile uint32_t  isel_idac                      : 1;  //0x50[5],idac current sel,0,R/W
           volatile uint32_t  adjdacref                      : 5;  //0x50[10:6],audio dac reference voltage adjust.,0,R/W
           volatile uint32_t  nc2                            : 1;  //0x50[11],nc,0,R/W
           volatile uint32_t  dcochg                         : 1;  //0x50[12],dcoc high gain selection. "1" high gain,0,R/W
           volatile uint32_t  diffen                         : 1;  //0x50[13],enable differential mode. "1" enable,0,R/W
           volatile uint32_t  endaccal                       : 1;  //0x50[14],enable offset calibration process. "1" enable.,0,R/W
           volatile uint32_t  rendcoc                        : 1;  //0x50[15],R-channel dcoc dac enablel. "1" enable,0,R/W
           volatile uint32_t  lendcoc                        : 1;  //0x50[16],L-channel Dcoc dac enable. "1" enable,0,R/W
           volatile uint32_t  renvcmd                        : 1;  //0x50[17],R-channel common mode output buffer enable."1" enable,0,R/W
           volatile uint32_t  lenvcmd                        : 1;  //0x50[18],L-channel common mode output buffer enable. "1" enable,0,R/W
           volatile uint32_t  dacdrven                       : 1;  //0x50[19],dac output driver enable."1" enable,0,R/W
           volatile uint32_t  dacren                         : 1;  //0x50[20],dac R-channel enable. "1"  enable,0,R/W
           volatile uint32_t  daclen                         : 1;  //0x50[21],dac L-channel enable. "1" enable,0,R/W
           volatile uint32_t  dacg                           : 3;  //0x50[24:22],dac gain setting: 000=0dB, 111=8dB,0,R/W
           volatile uint32_t  ck4xsel                        : 1;  //0x50[25],dac clock sel ,0,R/W
           volatile uint32_t  dacmute                        : 1;  //0x50[26],dac mute enable. "1" mute enable,0,R/W
           volatile uint32_t  dwamode                        : 1;  //0x50[27],dac dwa mode sel,0,R/W
           volatile uint32_t  ckposel                        : 1;  //0x50[28],dac sample clock edge selection,0,R/W
           volatile uint32_t  nc1                            : 2;  //0x50[30:29],nc,0,R/W
           volatile uint32_t  byldo                          : 1;  //0x50[31],bypass 1v8 LDO,0,R/W
        }; 
        uint32_t v; 
    }ana_reg16; 

    /* REG_0x51 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  lmdcin                         : 8;  //0x51[7:0],l-cnannel offset cancel dac maumual input.,0,R/W
           volatile uint32_t  rmdcin                         : 8;  //0x51[15:8],r-channel offset cancel dac manmual input ,0,R/W
           volatile uint32_t  spirst_ovc                     : 1;  //0x51[16],ovc rst,0,R/W
           volatile uint32_t  nc                             : 3;  //0x51[19:17],NC,0,R/W
           volatile uint32_t  hc2s0v9                        : 1;  //0x51[20],0=current is half,0,R/W
           volatile uint32_t  lvcmsel                        : 1;  //0x51[21],low vcm sel,0,R/W
           volatile uint32_t  loop2sel                       : 1;  //0x51[22],2rd loop sel,0,R/W
           volatile uint32_t  enbias                         : 1;  //0x51[23],dac bias enable,0,R/W
           volatile uint32_t  calck_sel0v9                   : 1;  //0x51[24],offset calibration clock selection. "1" high clock.,0,R/W
           volatile uint32_t  bpdwa0v9                       : 1;  //0x51[25],bypss audio dac dwa. "1" bypass.,0,R/W
           volatile uint32_t  looprst0v9                     : 1;  //0x51[26],audio dac integrator capacitor reset. "1" reset.,0,R/W
           volatile uint32_t  oct0v9                         : 2;  //0x51[28:27],over current delay time setting."11" maximum time. "00" minimum current.,0,R/W
           volatile uint32_t  sout0v9                        : 1;  //0x51[29],short output with 600ohm resistor. "1" short output.,0,R/W
           volatile uint32_t  hc0v9                          : 2;  //0x51[31:30],dac current trimming, 00=minimum current, 11=maximum current,0,R/W
        }; 
        uint32_t v; 
    }ana_reg17; 

    /* REG_0x52 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ictrl_dsppll                   : 4;  //0x52[3:0],26M PLL setting,7,W
           volatile uint32_t  fbdivn                         :10;  //0x52[13:4],26M PLL setting,0B6,W
           volatile uint32_t  n_mcudsp                       : 5;  //0x52[18:14],26M PLL setting,0E,W
           volatile uint32_t  mode                           : 1;  //0x52[19],26M PLL setting,1,W
           volatile uint32_t  iamsel                         : 1;  //0x52[20],26M PLL setting,0,W
           volatile uint32_t  hvref                          : 2;  //0x52[22:21],26M PLL setting,0,W
           volatile uint32_t  lvref                          : 2;  //0x52[24:23],26M PLL setting,0,W
           volatile uint32_t  nc                             : 7;  //0x52[31:25],26M PLL setting,0,W
        }; 
        uint32_t v; 
    }ana_reg18; 

    /* REG_0x53 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  amsel                          : 1;  //0x53[0],26M PLL setting,0,W
           volatile uint32_t  msw                            : 9;  //0x53[9:1],26M PLL setting,101,W
           volatile uint32_t  tstcken_dpll                   : 1;  //0x53[10],26M PLL setting,0,W
           volatile uint32_t  osccal_trig                    : 1;  //0x53[11],26M PLL setting,1,W
           volatile uint32_t  cnti                           : 9;  //0x53[20:12],26M PLL setting,100,W
           volatile uint32_t  nc                             : 1;  //0x53[21],26M PLL setting,0,W
           volatile uint32_t  spi_rst                        : 1;  //0x53[22],26M PLL setting,0,W
           volatile uint32_t  closeloop_en                   : 1;  //0x53[23],26M PLL setting,1,W
           volatile uint32_t  caltime                        : 1;  //0x53[24],26M PLL setting,1,W
           volatile uint32_t  lpfrz                          : 2;  //0x53[26:25],26M PLL setting,2,W
           volatile uint32_t  icp                            : 4;  //0x53[30:27],26M PLL setting,8,W
           volatile uint32_t  cp2ctrl                        : 1;  //0x53[31],26M PLL setting,0,W
        }; 
        uint32_t v; 
    }ana_reg19; 

} sys_hw_t;

#ifdef __cplusplus 
}                  
#endif             
