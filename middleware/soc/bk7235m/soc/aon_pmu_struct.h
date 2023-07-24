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
* CHIP ID:BK7256,GENARATE TIME:2022-01-17 15:35:03                                                 
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
           volatile uint32_t  memchk_bps                     : 1;  //0x0[0],memcheck bypass,1,R/W
           volatile uint32_t  reserved                       :31;  //0x0[31:1],reserved,0,R
        }; 
        uint32_t v; 
    }reg0; 

    /* REG_0x01 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  touch_select                   : 4;  //0x1[3:0], ,None,R
           volatile uint32_t  touch_int_en                   :16;  //0x1[19:4], ,None,R/W
           volatile uint32_t  usbplug_int_en                 : 1;  //0x1[20], ,None,R/W
           volatile uint32_t  reserved                       :11;  //0x1[31:21],reserved,0,R
        }; 
        uint32_t v; 
    }reg1; 

    /* REG_0x02 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  wdt_rst_ana                    : 1;  //0x2[0],wdt rst of ana,None,R/W
           volatile uint32_t  wdt_rst_top                    : 1;  //0x2[1],wdt rst of top,None,R/W
           volatile uint32_t  wdt_rst_aon                    : 1;  //0x2[2],wdt rst of aon,None,R/W
           volatile uint32_t  wdt_rst_awt                    : 1;  //0x2[3],wdt rst of awt,None,R/W
           volatile uint32_t  wdt_rst_gpio                   : 1;  //0x2[4],wdt rst of gpio,None,R/W
           volatile uint32_t  wdt_rst_rtc                    : 1;  //0x2[5],wdt rst of rtc,None,R/W
           volatile uint32_t  wdt_rst_wdt                    : 1;  //0x2[6],wdt rst of wdt,None,R/W
           volatile uint32_t  wdt_rst_pmu                    : 1;  //0x2[7],wdt rst of pmu,None,R/W
           volatile uint32_t  wdt_rst_phy                    : 1;  //0x2[8],wdt rst of phy,None,R/W
           volatile uint32_t  reserved                       :23;  //0x2[31:9],reserved,0,R
        }; 
        uint32_t v; 
    }reg2; 

    /* REG_0x03 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  aon_reg3                       :32;  //0x3[31:0],aon_reg3,None,R/W
        }; 
        uint32_t v; 
    }reg3; 

    /* REG_0x04 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x4_reserved; 

    /* REG_0x05 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x5_reserved; 

    /* REG_0x06 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x6_reserved; 

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
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x8_reserved; 

    /* REG_0x09 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x9_reserved; 

    /* REG_0x0a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xa_reserved; 

    /* REG_0x0b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xb_reserved; 

    /* REG_0x0c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xc_reserved; 

    /* REG_0x0d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xd_reserved; 

    /* REG_0x0e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xe_reserved; 

    /* REG_0x0f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xf_reserved; 

    /* REG_0x10 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x10_reserved; 

    /* REG_0x11 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x11_reserved; 

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
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x20_reserved; 

    /* REG_0x21 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x21_reserved; 

    /* REG_0x22 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x22_reserved; 

    /* REG_0x23 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x23_reserved; 

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
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x28_reserved; 

    /* REG_0x29 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x29_reserved; 

    /* REG_0x2a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x2a_reserved; 

    /* REG_0x2b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x2b_reserved; 

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
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x30_reserved; 

    /* REG_0x31 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x31_reserved; 

    /* REG_0x32 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x32_reserved; 

    /* REG_0x33 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x33_reserved; 

    /* REG_0x34 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x34_reserved; 

    /* REG_0x35 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x35_reserved; 

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
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x38_reserved; 

    /* REG_0x39 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x39_reserved; 

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
           volatile uint32_t  wake1_delay                    : 4;  //0x40[3:0],wake delay of Xtal,None,R/W
           volatile uint32_t  wake2_delay                    : 4;  //0x40[7:4],wake delay of Voltage,None,R/W
           volatile uint32_t  wake3_delay                    : 4;  //0x40[11:8], ,None,R/W
           volatile uint32_t  halt1_delay                    : 4;  //0x40[15:12],halt delay of Voltage ,None,R/W
           volatile uint32_t  halt2_delay                    : 4;  //0x40[19:16],halt delay of Core and flash ,None,R/W
           volatile uint32_t  halt3_delay                    : 4;  //0x40[23:20],halt delay of Xtal,None,R/W
           volatile uint32_t  halt_volt                      : 1;  //0x40[24],1:volt   halt ,None,R/W
           volatile uint32_t  halt_xtal                      : 1;  //0x40[25],1:xtal   halt ,None,R/W
           volatile uint32_t  halt_core                      : 1;  //0x40[26],1:core   halt ,None,R/W
           volatile uint32_t  halt_flash                     : 1;  //0x40[27],1:flash  halt ,None,R/W
           volatile uint32_t  halt_rosc                      : 1;  //0x40[28],1:rosc   halt ,None,R/W
           volatile uint32_t  halt_resten                    : 1;  //0x40[29],1:resten halt ,None,R/W
           volatile uint32_t  halt_isolat                    : 1;  //0x40[30],1:isolat halt ,None,R/W
           volatile uint32_t  halt_clkena                    : 1;  //0x40[31],0:reg config clk halt ,None,R/W
        }; 
        uint32_t v; 
    }reg40; 

    /* REG_0x41 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  lpo_config                     : 2;  //0x41[1:0],lpo_src : 1x : clk_ROSC ; 1:clk_X32k ;     0:clk_DIVD,None,R/W
           volatile uint32_t  flshsck_iocap                  : 2;  //0x41[3:2],spiflash sck driver selection,None,R/W
           volatile uint32_t  wakeup_ena                     : 5;  //0x41[8:4],[8]:int_touched                                                                 [7]:int_usbplug                                                                    [6]:system_wake                                                                   [5]:int_rtc                                                                                 [4]:int_gpio,None,R/W
           volatile uint32_t  reserved                       :23;  //0x41[31:9],reserved,0,R
        }; 
        uint32_t v; 
    }reg41; 

    /* REG_0x42 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  reg42                          :32;  //0x42[31:0], ,None,R/W
        }; 
        uint32_t v; 
    }reg42; 

    /* REG_0x43 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  clr_int_touched                :16;  //0x43[15:0],0:touch reset enable,None,R/W
           volatile uint32_t  clr_int_usbplug                : 1;  //0x43[16],0:usb reset enable,None,R/W
           volatile uint32_t  reserved                       :15;  //0x43[31:17],reserved,None,R
        }; 
        uint32_t v; 
    }reg43; 

    /* REG_0x44 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x44_reserved; 

    /* REG_0x45 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x45_reserved; 

    /* REG_0x46 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x46_reserved; 

    /* REG_0x47 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x47_reserved; 

    /* REG_0x48 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x48_reserved; 

    /* REG_0x49 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x49_reserved; 

    /* REG_0x4a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x4a_reserved; 

    /* REG_0x4b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x4b_reserved; 

    /* REG_0x4c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x4c_reserved; 

    /* REG_0x4d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x4d_reserved; 

    /* REG_0x4e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x4e_reserved; 

    /* REG_0x4f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x4f_reserved; 

    /* REG_0x50 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x50_reserved; 

    /* REG_0x51 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x51_reserved; 

    /* REG_0x52 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x52_reserved; 

    /* REG_0x53 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x53_reserved; 

    /* REG_0x54 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x54_reserved; 

    /* REG_0x55 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x55_reserved; 

    /* REG_0x56 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x56_reserved; 

    /* REG_0x57 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x57_reserved; 

    /* REG_0x58 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x58_reserved; 

    /* REG_0x59 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x59_reserved; 

    /* REG_0x5a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x5a_reserved; 

    /* REG_0x5b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x5b_reserved; 

    /* REG_0x5c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x5c_reserved; 

    /* REG_0x5d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x5d_reserved; 

    /* REG_0x5e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x5e_reserved; 

    /* REG_0x5f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x5f_reserved; 

    /* REG_0x60 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x60_reserved; 

    /* REG_0x61 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x61_reserved; 

    /* REG_0x62 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x62_reserved; 

    /* REG_0x63 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x63_reserved; 

    /* REG_0x64 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x64_reserved; 

    /* REG_0x65 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x65_reserved; 

    /* REG_0x66 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x66_reserved; 

    /* REG_0x67 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x67_reserved; 

    /* REG_0x68 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x68_reserved; 

    /* REG_0x69 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x69_reserved; 

    /* REG_0x6a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x6a_reserved; 

    /* REG_0x6b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x6b_reserved; 

    /* REG_0x6c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x6c_reserved; 

    /* REG_0x6d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x6d_reserved; 

    /* REG_0x6e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x6e_reserved; 

    /* REG_0x6f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x6f_reserved; 

    /* REG_0x70 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  int_touched                    :16;  //0x70[15:0],touch int state,None,RO
           volatile uint32_t  int_usbplug                    : 1;  //0x70[16],usb int state,None,RO
           volatile uint32_t  reserved                       :15;  //0x70[31:17],reserved,None,R
        }; 
        uint32_t v; 
    }reg70; 

    /* REG_0x71 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  touch_state                    :16;  //0x71[15:0],touch_state,None,RO
           volatile uint32_t  usbplug_state                  : 1;  //0x71[16],usbplug_state,None,RO
           volatile uint32_t  reserved0                      : 3;  //0x71[19:17],reserved,None,R
           volatile uint32_t  wakeup_source                  : 5;  //0x71[24:20],wakeup_source,None,RO
           volatile uint32_t  reserved1                      : 7;  //0x71[31:25],reserved,None,R
        }; 
        uint32_t v; 
    }reg71; 

    /* REG_0x72 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x72_reserved; 

    /* REG_0x73 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x73_reserved; 

    /* REG_0x74 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x74_reserved; 

    /* REG_0x75 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x75_reserved; 

    /* REG_0x76 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x76_reserved; 

    /* REG_0x77 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x77_reserved; 

    /* REG_0x78 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x78_reserved; 

    /* REG_0x79 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x79_reserved; 

    /* REG_0x7a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x7a_reserved; 

    /* REG_0x7b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x7b_reserved; 

    /* REG_0x7c */
    union 
    { 
        struct 
        { 
           volatile uint32_t  id                             :32;  //0x7c[31:0], ,None,R
        }; 
        uint32_t v; 
    }reg7c; 

    /* REG_0x7d */
    union 
    { 
        struct 
        { 
           volatile uint32_t  lcal                           : 8;  //0x7d[7:0], ,None,R
           volatile uint32_t  rcal                           : 8;  //0x7d[15:8], ,None,R
           volatile uint32_t  adc_cal                        : 7;  //0x7d[22:16], ,None,R
           volatile uint32_t  bgcal                          :16;  //0x7d[38:23], ,None,R
           volatile uint32_t  ltemp                          : 1;  //0x7d[29], ,None,R
           volatile uint32_t  htemp                          : 1;  //0x7d[30], ,None,R
           volatile uint32_t  pll26m_unlock                  : 1;  //0x7d[31], ,None,R
        }; 
        uint32_t v; 
    }reg7d; 

    /* REG_0x7e */
    union 
    { 
        struct 
        { 
           volatile uint32_t  capcal                         : 9;  //0x7e[8:0], ,None,R/W
           volatile uint32_t  h                              :23;  //0x7e[31:9], ,None,R
        }; 
        uint32_t v; 
    }reg7e; 

    /* REG_0x7f */
    union 
    { 
        struct 
        { 
           volatile uint32_t  h                              :32;  //0x7f[31:0], ,None,R
        }; 
        uint32_t v; 
    }reg7f; 

} aon_pmu_hw_t;

#ifdef __cplusplus 
}                  
#endif             
