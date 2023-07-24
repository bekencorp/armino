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
#include "aud_cap.h"       

typedef volatile struct {

    /* REG_0x00 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  samp_rate_adc                  : 2;  //0x0[1:0],ADC采样率配置 0：8K  1：16K  2：44.1K  3：48K,0,RW
           volatile uint32_t  dac_enable                     : 1;  //0x0[2],DAC使能  1：使能,0,RW
           volatile uint32_t  adc_enable                     : 1;  //0x0[3],ADC使能  1：使能,0,RW
           volatile uint32_t  dtmf_enable                    : 1;  //0x0[4],DTMF使能 1: 使能,0,RW
           volatile uint32_t  line_enable                    : 1;  //0x0[5],LINE IN使能  1：使能,0,RW
           volatile uint32_t  samp_rate_dac                  : 2;  //0x0[7:6],DAC采样率配置 0：8K  1：16K  2：44.1K  3：48K,0,RW
           volatile uint32_t  reserved                       :24;  //0x0[31:8],reserved,0,R
        }; 
        uint32_t v; 
    }audio_config; 

    /* REG_0x01 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  tone_pattern                   : 1;  //0x1[0],1：Tone1(Active_Time) + Tone2(Pause_Time)              0: DTMF(Active_Time) + Zeros(Pause_Time),0,RW
           volatile uint32_t  tone_mode                      : 1;  //0x1[1],1：连续模式， 0：单次模式,0,RW
           volatile uint32_t  tone_pause_time                : 4;  //0x1[5:2],DTMF的间歇时间  N*10 ms,0,RW
           volatile uint32_t  tone_active_time               : 4;  //0x1[9:6],DTMF的工作时间  N*10 ms,0,RW
           volatile uint32_t  reserved                       :22;  //0x1[31:10],reserved,0,R
        }; 
        uint32_t v; 
    }dtmf_config0; 

    /* REG_0x02 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  tone1_step                     :16;  //0x2[15:0],Tone1的频率设置  Step = F(KHz) * 8191 ,0,RW
           volatile uint32_t  tone1_attu                     : 4;  //0x2[19:16],Tone1的衰减设置  0 ：-1 ：-15 dB ,0,RW
           volatile uint32_t  tone1_enable                   : 1;  //0x2[20],Tone1的使能设置,0,RW
           volatile uint32_t  reserved                       :11;  //0x2[31:21],reserved,0,R
        }; 
        uint32_t v; 
    }dtmf_config1; 

    /* REG_0x03 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  tone2_step                     :16;  //0x3[15:0],Tone2的频率设置  Step = F(KHz) * 8191 ,0,RW
           volatile uint32_t  tone2_attu                     : 4;  //0x3[19:16],Tone2的衰减设置  0 ：-1 ：-15 dB ,0,RW
           volatile uint32_t  tone2_enable                   : 1;  //0x3[20],Tone2的使能设置,0,RW
           volatile uint32_t  reserved                       :11;  //0x3[31:21],reserved,0,R
        }; 
        uint32_t v; 
    }dtmf_config2; 

    /* REG_0x04 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  adc_hpf2_coef_b2               :16;  //0x4[15:0],ADC HPF2的系数 B2 ,0,RW
           volatile uint32_t  adc_hpf2_bypass                : 1;  //0x4[16],ADC HPF2禁能位，1：绕开HPF2,0,RW
           volatile uint32_t  adc_hpf1_bypass                : 1;  //0x4[17],ADC HPF1禁能位，1：绕开HPF1,0,RW
           volatile uint32_t  adc_set_gain                   : 6;  //0x4[23:18],ADC 设置的增益,0,RW
           volatile uint32_t  adc_sampe_edge                 : 1;  //0x4[24],ADC数据采样的时钟边沿选择 1：下降沿，0：上升沿,0,RW
           volatile uint32_t  reserved                       : 7;  //0x4[31:25],reserved,0,R
        }; 
        uint32_t v; 
    }adc_config0; 

    /* REG_0x05 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  adc_hpf2_coef_b0               :16;  //0x5[15: 0],ADC HPF2的系数 B2 ,0,RW
           volatile uint32_t  adc_hpf2_coef_b1               :16;  //0x5[31:16],ADC HPF2的系数 B1 ,0,RW
        }; 
        uint32_t v; 
    }adc_config1; 

    /* REG_0x06 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  adc_hpf2_coef_a0               :16;  //0x6[15: 0],ADC HPF2的系数 A0 ,0,RW
           volatile uint32_t  adc_hpf2_coef_a1               :16;  //0x6[31:16],ADC HPF2的系数 A1 ,0,RW
        }; 
        uint32_t v; 
    }adc_config2; 

    /* REG_0x07 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  dac_hpf2_coef_b2               :16;  //0x7[15:0],DAC HPF2的系数 B2 ,0,RW
           volatile uint32_t  dac_hpf2_bypass                : 1;  //0x7[16],DAC HPF2禁能位，1：绕开HPF2,0,RW
           volatile uint32_t  dac_hpf1_bypass                : 1;  //0x7[17],DAC HPF1禁能位，1：绕开HPF1 ,0,RW
           volatile uint32_t  dac_set_gain                   : 6;  //0x7[23:18],DAC 设置的增益,0,RW
           volatile uint32_t  dac_clk_invert                 : 1;  //0x7[24],DAC 输出时钟边沿选择 0：下降沿，1：上升沿,0,RW
           volatile uint32_t  reserved                       : 7;  //0x7[31:25],reserved,0,R
        }; 
        uint32_t v; 
    }dac_config0; 

    /* REG_0x08 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  dac_hpf2_coef_b0               :16;  //0x8[15: 0],DAC HPF2的系数 B0 ,0,RW
           volatile uint32_t  dac_hpf2_coef_b1               :16;  //0x8[31:16],DAC HPF2的系数 B1 ,0,RW
        }; 
        uint32_t v; 
    }dac_config1; 

    /* REG_0x09 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  dac_hpf2_coef_a1               :16;  //0x9[15: 0],DAC HPF2的系数 A1 ,0,RW
           volatile uint32_t  dac_hpf2_coef_a2               :16;  //0x9[31:16],DAC HPF2的系数 A2 ,0,RW
        }; 
        uint32_t v; 
    }dac_config2; 

    /* REG_0x0a */
    union 
    { 
        struct 
        { 
           volatile uint32_t  dacr_rd_threshold              : 5;  //0xa[4:0],DAC右声道读FIFO时，数目低于该门限，发出中断请求,0,RW
           volatile uint32_t  dacl_rd_threshold              : 5;  //0xa[9:5],DAC左声道读FIFO时，数目低于该门限，发出中断请求,0,RW
           volatile uint32_t  dtmf_wr_threshold              : 5;  //0xa[14:10],DTMF自动写FIFO时，数目高于该门限，发出中断请求,0,RW
           volatile uint32_t  adcl_wr_threshold              : 5;  //0xa[19:15],ADC自动写FIFO时，数目高于该门限，发出中断请求,0,RW
           volatile uint32_t  dacr_int_en                    : 1;  //0xa[20],DAC右声道中断请求使能位,0,RW
           volatile uint32_t  dacl_int_en                    : 1;  //0xa[21],DAC左声道中断请求使能位,0,RW
           volatile uint32_t  dtmf_int_en                    : 1;  //0xa[22],DTMF中断请求使能位,0,RW
           volatile uint32_t  adcl_int_en                    : 1;  //0xa[23],ADC中断请求使能位,0,RW
           volatile uint32_t  loop_ton2dac                   : 1;  //0xa[24],DTMF到DAC环回测试使能位,0,RW
           volatile uint32_t  loop_adc2dac                   : 1;  //0xa[25],ADC到DAC环回测试使能位,0,RW
           volatile uint32_t  reserved                       : 6;  //0xa[31:26],reserved,0,R
        }; 
        uint32_t v; 
    }fifo_config; 

    /* REG_0x0b */
    union 
    { 
        struct 
        { 
           volatile uint32_t  agc_noise_thrd                 :10;  //0xb[ 9: 0],noise gating 执行时刻的门限，noise gating要在audio值比较小的时候执行,0,RW
           volatile uint32_t  agc_noise_high                 :10;  //0xb[19:10],对应MIC_RSSI低10 bit,0,RW
           volatile uint32_t  agc_noise_low                  :10;  //0xb[29:20],对应MIC_RSSI低10 bit，比如MIC_RSSI[15:10]任何一位为1，都说明信号电平超过NOISE_LOW,0,RW
           volatile uint32_t  agc_step                       : 2;  //0xb[31:30],没有用,0,RW
        }; 
        uint32_t v; 
    }agc_config0; 

    /* REG_0x0c */
    union 
    { 
        struct 
        { 
           volatile uint32_t  agc_noise_min                  : 7;  //0xc[6:0],{GAIN2[2:0] GAIN1[3:0]} when signal level below NOISE_LOW, when noise gating is enabled,0,RW
           volatile uint32_t  agc_noise_tout                 : 3;  //0xc[9:7],1: 4 (~0.5 ms), 2: 8, 3: 16, 4: 32, 5: 64, 6: 128, 7: 256 (~32 ms),0,RW
           volatile uint32_t  agc_high_dur                   : 3;  //0xc[12:10],0: 0 (0 ms，一旦发生MIC_RSSI超过NOISE_HIGH，立刻触发Noise gating，离开noise状态); 1: 8 (~1 ms), 2: 16, 3: 32, 4: 64, 5: 128, 6: 256, 7: 256 (~32 ms),0,RW
           volatile uint32_t  agc_low_dur                    : 3;  //0xc[15:13],0: 0 (0 ms，一旦发生MIC_RSSI小于NOISE_LOW，立刻触发noise gating); 1: 8 (~1 ms), 2: 16, 3: 32, 4: 64, 5: 128, 6: 256, 7: 256 (~32 ms),0,RW
           volatile uint32_t  agc_min                        : 7;  //0xc[22:16],Minimum value of {GAIN2[2:0] GAIN1[3:0]},0,RW
           volatile uint32_t  agc_max                        : 7;  //0xc[29:23],Maximum value of {GAIN2[2:0] GAIN1[3:0]}; Also the  default gain setting when AGC is disabled,0,RW
           volatile uint32_t  agc_ng_method                  : 1;  //0xc[30],0: 直接mute或者demute (default); 1: 逐渐降低增益或者增加增益,0,RW
           volatile uint32_t  agc_ng_enable                  : 1;  //0xc[31],使能 noise gating,0,RW
        }; 
        uint32_t v; 
    }agc_config1; 

    /* REG_0x0d */
    union 
    { 
        struct 
        { 
           volatile uint32_t  agc_decay_time                 : 3;  //0xd[2:0],0: 128 (~2.7 ms), 1: 256, 2: 512, 3: 1024, 4:2048, 5: 4096, 6: 8192, 7: 16384 (~340 ms),0,RW
           volatile uint32_t  agc_attack_time                : 3;  //0xd[5:3],0: 8 (~0.1667ms), 1: 16, 2: 32, 3: 64, 4: 128, 5: 256, 6: 512, 7: 1024 (~21 ms),0,RW
           volatile uint32_t  agc_high_thrd                  : 5;  //0xd[10:6],对应到MIC_RSSI的最高5位,0,RW
           volatile uint32_t  agc_low_thrd                   : 5;  //0xd[15:11],对应到MIC_RSSI的最高5位,0,RW
           volatile uint32_t  agc_iir_coef                   : 3;  //0xd[18:16],IIR系数选择 0:1/32 1:1/64 2:1/128 3:1/256 4:1/512 5:1/1024 6:1/2048 7:1/4096,0,RW
           volatile uint32_t  agc_enable                     : 1;  //0xd[19],AGC使能位,0,RW
           volatile uint32_t  manual_pga_value               : 7;  //0xd[26:20],手动PGA的值,0,RW
           volatile uint32_t  manual_pga                     : 1;  //0xd[27],手动PGA模式,0,RW
           volatile uint32_t  reserved                       : 4;  //0xd[31:28],reserved,0,R
        }; 
        uint32_t v; 
    }agc_config2; 

    /* REG_0x0e */
    union 
    { 
        struct 
        { 
           volatile uint32_t  dacr_near_full                 : 1;  //0xe[0],DAC右声道FIFO快要满  （默认大于3/4的容量）,0,R
           volatile uint32_t  dacl_near_full                 : 1;  //0xe[1],DAC左声道FIFO快要满,0,R
           volatile uint32_t  adcl_near_full                 : 1;  //0xe[2],ADC左声道FIFO快要满,0,R
           volatile uint32_t  dtmf_near_full                 : 1;  //0xe[3],DTMF的FIFO快要满,0,R
           volatile uint32_t  dacr_near_empty                : 1;  //0xe[4],DAC右声道FIFO快要空  （默认小于1/4的容量）,0,R
           volatile uint32_t  dacl_near_empty                : 1;  //0xe[5],DAC左声道FIFO快要空,0,R
           volatile uint32_t  adcl_near_empty                : 1;  //0xe[6],ADC左声道FIFO快要空,0,R
           volatile uint32_t  dtmf_near_empty                : 1;  //0xe[7],DTMF的FIFO快要空,0,R
           volatile uint32_t  dacr_fifo_full                 : 1;  //0xe[8],DAC右声道FIFO已满    (达到FIFO容量的最大值）,0,R
           volatile uint32_t  dacl_fifo_full                 : 1;  //0xe[9],DAC左声道FIFO已满,0,R
           volatile uint32_t  adcl_fifo_full                 : 1;  //0xe[10],ADC左声道FIFO已满,0,R
           volatile uint32_t  dtmf_fifo_full                 : 1;  //0xe[11],DTMF的FIFO已满,0,R
           volatile uint32_t  dacr_fifo_empty                : 1;  //0xe[12],DAC右声道FIFO已空   (达到FIFO容量的最小值）,0,R
           volatile uint32_t  dacl_fifo_empty                : 1;  //0xe[13],DAC左声道FIFO已空,0,R
           volatile uint32_t  adcl_fifo_empty                : 1;  //0xe[14],ADC左声道FIFO已空,0,R
           volatile uint32_t  dtmf_fifo_empty                : 1;  //0xe[15],DTMF的FIFO已空,0,R
           volatile uint32_t  dacr_int_flag                  : 1;  //0xe[16],DAC右声道中断标志,0,R
           volatile uint32_t  dacl_int_flag                  : 1;  //0xe[17],DAC左声道中断标志,0,R
           volatile uint32_t  adcl_int_flag                  : 1;  //0xe[18],ADC左声道中断标志,0,R
           volatile uint32_t  dtmf_int_flag                  : 1;  //0xe[19],DTMF的中断标志,0,R
           volatile uint32_t  reserved                       :12;  //0xe[31:20],reserved,0,R
        }; 
        uint32_t v; 
    }fifo_status; 

    /* REG_0x0f */
    union 
    { 
        struct 
        { 
           volatile uint32_t  rssi                           : 8;  //0xf[7:0],Absolutely microphone signal power in dB unit; Read only,0,R
           volatile uint32_t  mic_pga                        : 8;  //0xf[15:8],{1'b0, GAIN2[2:0] GAIN1[3:0]} (Read only),0,R
           volatile uint32_t  mic_rssi                       :16;  //0xf[31:16],Microphone level; MIC_RSSI[15:0]; Read only,0,R
        }; 
        uint32_t v; 
    }agc_status; 

    /* REG_0x10 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  dtmf_port                      :16;  //0x10[15:0],DTMF的FIFO读出地址,0,R
           volatile uint32_t  reserved                       :16;  //0x10[31:16],reserved,0,R
        }; 
        uint32_t v; 
    }dtmf_fport; 

    /* REG_0x11 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  adc_port                       :16;  //0x11[15:0],ADC的FIFO读出地址,0,R
           volatile uint32_t  reserved                       :16;  //0x11[31:16],reserved,0,R
        }; 
        uint32_t v; 
    }adc_fport; 

    /* REG_0x12 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  dacl_port                      :16;  //0x12[15:0],DACL的FIFO写入地址,0,W
           volatile uint32_t  dacr_port                      :16;  //0x12[31:16],DACR的FIFO写入地址,0,W
        }; 
        uint32_t v; 
    }dacl_fport; 

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
           volatile uint32_t  dac_fracmod_manual             : 1;  //0x18[0],DAC小数分频手动设置使能,0,RW
           volatile uint32_t  adc_fracmod_manual             : 1;  //0x18[1],ADC小数分频手动设置使能,0,RW
           volatile uint32_t  filt_enable                    : 1;  //0x18[2],DAC的频响滤波器使能,0,RW
           volatile uint32_t  reserved                       :29;  //0x18[31:3],reserved,0,R
        }; 
        uint32_t v; 
    }extend_cfg; 

    /* REG_0x19 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  dac_fraccoef                   :32;  //0x19[31:0],DAC小数分频设置值 N * 2^24,0,RW
        }; 
        uint32_t v; 
    }dac_fracmod; 

    /* REG_0x1a */
    union 
    { 
        struct 
        { 
           volatile uint32_t  adc_fraccoef                   :32;  //0x1a[31:0],ADC小数分频设置值 N * 2^24,0,RW
        }; 
        uint32_t v; 
    }adc_fracmod; 

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
           volatile uint32_t  hpf2_a1_l6bit                  : 6;  //0x1f[5:0],DAC HPF2的系数A1低6比特,0,RW
           volatile uint32_t  hpf2_a2_l6bit                  : 6;  //0x1f[11:6],DAC HPF2的系数A2低6比特,0,RW
           volatile uint32_t  hpf2_b0_l6bit                  : 6;  //0x1f[17:12],DAC HPF2的系数B0低6比特,0,RW
           volatile uint32_t  hpf2_b1_l6bit                  : 6;  //0x1f[23:18],DAC HPF2的系数B1低6比特,0,RW
           volatile uint32_t  hpf2_b2_l6bit                  : 6;  //0x1f[29:24],DAC HPF2的系数B2低6比特,0,RW
           volatile uint32_t  reserved                       : 2;  //0x1f[31:30],reserved,0,R
        }; 
        uint32_t v; 
    }hpf2_ext_coef; 

    /* REG_0x20 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  flt0_a1                        :16;  //0x20[15: 0],Filter 0 的IIR系数A1,0,RW
           volatile uint32_t  flt0_a2                        :16;  //0x20[31:16],Filter 0 的IIR系数A2,0,RW
        }; 
        uint32_t v; 
    }flt0_coef_a1a2; 

    /* REG_0x21 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  flt0_b0                        :16;  //0x21[15: 0],Filter 0 的IIR系数B0,0,RW
           volatile uint32_t  flt0_b1                        :16;  //0x21[31:16],Filter 0 的IIR系数B1,0,RW
        }; 
        uint32_t v; 
    }flt0_coef_b0b1; 

    /* REG_0x22 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  flt0_b2                        :16;  //0x22[15: 0],Filter 0 的IIR系数B2,0,RW
           volatile uint32_t  reserved                       :16;  //0x22[31:16],reserved,0,R
        }; 
        uint32_t v; 
    }flt0_coef_b2; 

    /* REG_0x23 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  flt1_a1                        :16;  //0x23[15: 0],Filter 1 的IIR系数A1,0,RW
           volatile uint32_t  flt1_a2                        :16;  //0x23[31:16],Filter 1 的IIR系数A2,0,RW
        }; 
        uint32_t v; 
    }flt1_coef_a1a2; 

    /* REG_0x24 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  flt1_b0                        :16;  //0x24[15: 0],Filter 1 的IIR系数B0,0,RW
           volatile uint32_t  flt1_b1                        :16;  //0x24[31:16],Filter 1 的IIR系数B1,0,RW
        }; 
        uint32_t v; 
    }flt1_coef_b0b1; 

    /* REG_0x25 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  flt1_b2                        :16;  //0x25[15: 0],Filter 1 的IIR系数B2,0,RW
           volatile uint32_t  reserved                       :16;  //0x25[31:16],reserved,0,R
        }; 
        uint32_t v; 
    }flt1_coef_b2; 

    /* REG_0x26 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  flt2_a1                        :16;  //0x26[15: 0],Filter 2 的IIR系数A1,0,RW
           volatile uint32_t  flt2_a2                        :16;  //0x26[31:16],Filter 2 的IIR系数A2,0,RW
        }; 
        uint32_t v; 
    }flt2_coef_a1a2; 

    /* REG_0x27 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  flt2_b0                        :16;  //0x27[15: 0],Filter 2 的IIR系数B0,0,RW
           volatile uint32_t  flt2_b1                        :16;  //0x27[31:16],Filter 2 的IIR系数B1,0,RW
        }; 
        uint32_t v; 
    }flt2_coef_b0b1; 

    /* REG_0x28 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  flt2_b2                        :16;  //0x28[15: 0],Filter 2 的IIR系数B2,0,RW
           volatile uint32_t  reserved                       :16;  //0x28[31:16],reserved,0,R
        }; 
        uint32_t v; 
    }flt2_coef_b2; 

    /* REG_0x29 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  flt3_a1                        :16;  //0x29[15: 0],Filter 3 的IIR系数A1,0,RW
           volatile uint32_t  flt3_a2                        :16;  //0x29[31:16],Filter 3 的IIR系数A2,0,RW
        }; 
        uint32_t v; 
    }flt3_coef_a1a2; 

    /* REG_0x2a */
    union 
    { 
        struct 
        { 
           volatile uint32_t  flt3_b0                        :16;  //0x2a[15: 0],Filter 3 的IIR系数B0,0,RW
           volatile uint32_t  flt3_b1                        :16;  //0x2a[31:16],Filter 3 的IIR系数B1,0,RW
        }; 
        uint32_t v; 
    }flt3_coef_b0b1; 

    /* REG_0x2b */
    union 
    { 
        struct 
        { 
           volatile uint32_t  flt3_b2                        :16;  //0x2b[15: 0],Filter 3 的IIR系数B2,0,RW
           volatile uint32_t  reserved                       :16;  //0x2b[31:16],reserved,0,R
        }; 
        uint32_t v; 
    }flt3_coef_b2; 

    /* REG_0x2c */
    union 
    { 
        struct 
        { 
           volatile uint32_t  flt0_a1_l6bit                  : 6;  //0x2c[5:0],Filter 0 的IIR系数A1低6比特,0,RW
           volatile uint32_t  flt0_a2_l6bit                  : 6;  //0x2c[11:6],Filter 0 的IIR系数A2低6比特,0,RW
           volatile uint32_t  flt0_b0_l6bit                  : 6;  //0x2c[17:12],Filter 0 的IIR系数B0低6比特,0,RW
           volatile uint32_t  flt0_b1_l6bit                  : 6;  //0x2c[23:18],Filter 0 的IIR系数B1低6比特,0,RW
           volatile uint32_t  flt0_b2_l6bit                  : 6;  //0x2c[29:24],Filter 0 的IIR系数B2低6比特,0,RW
           volatile uint32_t  reserved                       : 2;  //0x2c[31:30],reserved,0,R
        }; 
        uint32_t v; 
    }flt0_ext_coef; 

    /* REG_0x2d */
    union 
    { 
        struct 
        { 
           volatile uint32_t  flt1_a1_l6bit                  : 6;  //0x2d[5:0],Filter 1 的IIR系数A1低6比特,0,RW
           volatile uint32_t  flt1_a2_l6bit                  : 6;  //0x2d[11:6],Filter 1 的IIR系数A2低6比特,0,RW
           volatile uint32_t  flt1_b0_l6bit                  : 6;  //0x2d[17:12],Filter 1 的IIR系数B0低6比特,0,RW
           volatile uint32_t  flt1_b1_l6bit                  : 6;  //0x2d[23:18],Filter 1 的IIR系数B1低6比特,0,RW
           volatile uint32_t  flt1_b2_l6bit                  : 6;  //0x2d[29:24],Filter 1 的IIR系数B2低6比特,0,RW
           volatile uint32_t  reserved                       : 2;  //0x2d[31:30],reserved,0,R
        }; 
        uint32_t v; 
    }flt1_ext_coef; 

    /* REG_0x2e */
    union 
    { 
        struct 
        { 
           volatile uint32_t  flt2_a1_l6bit                  : 6;  //0x2e[5:0],Filter 2 的IIR系数A1低6比特,0,RW
           volatile uint32_t  flt2_a2_l6bit                  : 6;  //0x2e[11:6],Filter 2 的IIR系数A2低6比特,0,RW
           volatile uint32_t  flt2_b0_l6bit                  : 6;  //0x2e[17:12],Filter 2 的IIR系数B0低6比特,0,RW
           volatile uint32_t  flt2_b1_l6bit                  : 6;  //0x2e[23:18],Filter 2 的IIR系数B1低6比特,0,RW
           volatile uint32_t  flt2_b2_l6bit                  : 6;  //0x2e[29:24],Filter 2 的IIR系数B2低6比特,0,RW
           volatile uint32_t  reserved                       : 2;  //0x2e[31:30],reserved,0,R
        }; 
        uint32_t v; 
    }flt2_ext_coef; 

    /* REG_0x2f */
    union 
    { 
        struct 
        { 
           volatile uint32_t  flt3_a1_l6bit                  : 6;  //0x2f[5:0],Filter 3 的IIR系数A1低6比特,0,RW
           volatile uint32_t  flt3_a2_l6bit                  : 6;  //0x2f[11:6],Filter 3 的IIR系数A2低6比特,0,RW
           volatile uint32_t  flt3_b0_l6bit                  : 6;  //0x2f[17:12],Filter 3 的IIR系数B0低6比特,0,RW
           volatile uint32_t  flt3_b1_l6bit                  : 6;  //0x2f[23:18],Filter 3 的IIR系数B1低6比特,0,RW
           volatile uint32_t  flt3_b2_l6bit                  : 6;  //0x2f[29:24],Filter 3 的IIR系数B2低6比特,0,RW
           volatile uint32_t  reserved                       : 2;  //0x2f[31:30],reserved,0,R
        }; 
        uint32_t v; 
    }flt3_ext_coef; 

} aud_hw_t;

#ifdef __cplusplus 
}                  
#endif             
