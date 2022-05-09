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
                            
#include <soc/soc.h>          
#include "aud_hw.h"        
#include "aud_ll_macro_def.h"         

#ifdef __cplusplus          
extern "C" {              
#endif                      

#if 0

/* REG_0x00 */

static inline uint32_t aud_ll_get_audio_config_value(aud_hw_t *hw)
{
    return hw->audio_config.v;
}

static inline void aud_ll_set_audio_config_value(aud_hw_t *hw, uint32_t value)
{
    hw->audio_config.v = value;
}

/* REG_0x00:audio_config->samp_rate_ADC:0x0[1:0],ADC采样率配置 0：8K  1：16K  2：44.1K  3：48K,0,RW*/
static inline uint32_t aud_ll_get_audio_config_samp_rate_adc(aud_hw_t *hw)
{
    return hw->audio_config.samp_rate_adc;
}

static inline void aud_ll_set_audio_config_samp_rate_adc(aud_hw_t *hw, uint32_t value)
{
    hw->audio_config.samp_rate_adc = value;
}

/* REG_0x00:audio_config->dac_enable:0x0[2],DAC使能  1：使能,0,RW*/
static inline uint32_t aud_ll_get_audio_config_dac_enable(aud_hw_t *hw)
{
    return hw->audio_config.dac_enable;
}

static inline void aud_ll_set_audio_config_dac_enable(aud_hw_t *hw, uint32_t value)
{
    hw->audio_config.dac_enable = value;
}

/* REG_0x00:audio_config->adc_enable:0x0[3],ADC使能  1：使能,0,RW*/
static inline uint32_t aud_ll_get_audio_config_adc_enable(aud_hw_t *hw)
{
    return hw->audio_config.adc_enable;
}

static inline void aud_ll_set_audio_config_adc_enable(aud_hw_t *hw, uint32_t value)
{
    hw->audio_config.adc_enable = value;
}

/* REG_0x00:audio_config->dtmf_enable:0x0[4],DTMF使能 1: 使能,0,RW*/
static inline uint32_t aud_ll_get_audio_config_dtmf_enable(aud_hw_t *hw)
{
    return hw->audio_config.dtmf_enable;
}

static inline void aud_ll_set_audio_config_dtmf_enable(aud_hw_t *hw, uint32_t value)
{
    hw->audio_config.dtmf_enable = value;
}

/* REG_0x00:audio_config->line_enable:0x0[5],LINE IN使能  1：使能,0,RW*/
static inline uint32_t aud_ll_get_audio_config_line_enable(aud_hw_t *hw)
{
    return hw->audio_config.line_enable;
}

static inline void aud_ll_set_audio_config_line_enable(aud_hw_t *hw, uint32_t value)
{
    hw->audio_config.line_enable = value;
}

/* REG_0x00:audio_config->samp_rate_DAC:0x0[7:6],DAC采样率配置 0：8K  1：16K  2：44.1K  3：48K,0,RW*/
static inline uint32_t aud_ll_get_audio_config_samp_rate_dac(aud_hw_t *hw)
{
    return hw->audio_config.samp_rate_dac;
}

static inline void aud_ll_set_audio_config_samp_rate_dac(aud_hw_t *hw, uint32_t value)
{
    hw->audio_config.samp_rate_dac = value;
}

/* REG_0x01 */

static inline uint32_t aud_ll_get_dtmf_config0_value(aud_hw_t *hw)
{
    return hw->dtmf_config0.v;
}

static inline void aud_ll_set_dtmf_config0_value(aud_hw_t *hw, uint32_t value)
{
    hw->dtmf_config0.v = value;
}

/* REG_0x01:dtmf_config0->tone_pattern:0x1[0],1：Tone1(Active_Time) + Tone2(Pause_Time)              0: DTMF(Active_Time) + Zeros(Pause_Time),0,RW*/
static inline uint32_t aud_ll_get_dtmf_config0_tone_pattern(aud_hw_t *hw)
{
    return hw->dtmf_config0.tone_pattern;
}

static inline void aud_ll_set_dtmf_config0_tone_pattern(aud_hw_t *hw, uint32_t value)
{
    hw->dtmf_config0.tone_pattern = value;
}

/* REG_0x01:dtmf_config0->tone_mode:0x1[1],1：连续模式， 0：单次模式,0,RW*/
static inline uint32_t aud_ll_get_dtmf_config0_tone_mode(aud_hw_t *hw)
{
    return hw->dtmf_config0.tone_mode;
}

static inline void aud_ll_set_dtmf_config0_tone_mode(aud_hw_t *hw, uint32_t value)
{
    hw->dtmf_config0.tone_mode = value;
}

/* REG_0x01:dtmf_config0->tone_pause_time:0x1[5:2],DTMF的间歇时间  N*10 ms,0,RW*/
static inline uint32_t aud_ll_get_dtmf_config0_tone_pause_time(aud_hw_t *hw)
{
    return hw->dtmf_config0.tone_pause_time;
}

static inline void aud_ll_set_dtmf_config0_tone_pause_time(aud_hw_t *hw, uint32_t value)
{
    hw->dtmf_config0.tone_pause_time = value;
}

/* REG_0x01:dtmf_config0->tone_active_time:0x1[9:6],DTMF的工作时间  N*10 ms,0,RW*/
static inline uint32_t aud_ll_get_dtmf_config0_tone_active_time(aud_hw_t *hw)
{
    return hw->dtmf_config0.tone_active_time;
}

static inline void aud_ll_set_dtmf_config0_tone_active_time(aud_hw_t *hw, uint32_t value)
{
    hw->dtmf_config0.tone_active_time = value;
}

/* REG_0x02 */

static inline uint32_t aud_ll_get_dtmf_config1_value(aud_hw_t *hw)
{
    return hw->dtmf_config1.v;
}

static inline void aud_ll_set_dtmf_config1_value(aud_hw_t *hw, uint32_t value)
{
    hw->dtmf_config1.v = value;
}

/* REG_0x02:dtmf_config1->tone1_step:0x2[15:0],Tone1的频率设置  Step = F(KHz) * 8191 ,0,RW*/
static inline uint32_t aud_ll_get_dtmf_config1_tone1_step(aud_hw_t *hw)
{
    return hw->dtmf_config1.tone1_step;
}

static inline void aud_ll_set_dtmf_config1_tone1_step(aud_hw_t *hw, uint32_t value)
{
    hw->dtmf_config1.tone1_step = value;
}

/* REG_0x02:dtmf_config1->tone1_attu:0x2[19:16],Tone1的衰减设置  0 ：-1 ：-15 dB ,0,RW*/
static inline uint32_t aud_ll_get_dtmf_config1_tone1_attu(aud_hw_t *hw)
{
    return hw->dtmf_config1.tone1_attu;
}

static inline void aud_ll_set_dtmf_config1_tone1_attu(aud_hw_t *hw, uint32_t value)
{
    hw->dtmf_config1.tone1_attu = value;
}

/* REG_0x02:dtmf_config1->tone1_enable:0x2[20],Tone1的使能设置,0,RW*/
static inline uint32_t aud_ll_get_dtmf_config1_tone1_enable(aud_hw_t *hw)
{
    return hw->dtmf_config1.tone1_enable;
}

static inline void aud_ll_set_dtmf_config1_tone1_enable(aud_hw_t *hw, uint32_t value)
{
    hw->dtmf_config1.tone1_enable = value;
}

/* REG_0x03 */

static inline uint32_t aud_ll_get_dtmf_config2_value(aud_hw_t *hw)
{
    return hw->dtmf_config2.v;
}

static inline void aud_ll_set_dtmf_config2_value(aud_hw_t *hw, uint32_t value)
{
    hw->dtmf_config2.v = value;
}

/* REG_0x03:dtmf_config2->tone2_step:0x3[15:0],Tone2的频率设置  Step = F(KHz) * 8191 ,0,RW*/
static inline uint32_t aud_ll_get_dtmf_config2_tone2_step(aud_hw_t *hw)
{
    return hw->dtmf_config2.tone2_step;
}

static inline void aud_ll_set_dtmf_config2_tone2_step(aud_hw_t *hw, uint32_t value)
{
    hw->dtmf_config2.tone2_step = value;
}

/* REG_0x03:dtmf_config2->tone2_attu:0x3[19:16],Tone2的衰减设置  0 ：-1 ：-15 dB ,0,RW*/
static inline uint32_t aud_ll_get_dtmf_config2_tone2_attu(aud_hw_t *hw)
{
    return hw->dtmf_config2.tone2_attu;
}

static inline void aud_ll_set_dtmf_config2_tone2_attu(aud_hw_t *hw, uint32_t value)
{
    hw->dtmf_config2.tone2_attu = value;
}

/* REG_0x03:dtmf_config2->tone2_enable:0x3[20],Tone2的使能设置,0,RW*/
static inline uint32_t aud_ll_get_dtmf_config2_tone2_enable(aud_hw_t *hw)
{
    return hw->dtmf_config2.tone2_enable;
}

static inline void aud_ll_set_dtmf_config2_tone2_enable(aud_hw_t *hw, uint32_t value)
{
    hw->dtmf_config2.tone2_enable = value;
}

/* REG_0x04 */

static inline uint32_t aud_ll_get_adc_config0_value(aud_hw_t *hw)
{
    return hw->adc_config0.v;
}

static inline void aud_ll_set_adc_config0_value(aud_hw_t *hw, uint32_t value)
{
    hw->adc_config0.v = value;
}

/* REG_0x04:adc_config0->adc_hpf2_coef_B2:0x4[15:0],ADC HPF2的系数 B2 ,0,RW*/
static inline uint32_t aud_ll_get_adc_config0_adc_hpf2_coef_b2(aud_hw_t *hw)
{
    return hw->adc_config0.adc_hpf2_coef_b2;
}

static inline void aud_ll_set_adc_config0_adc_hpf2_coef_b2(aud_hw_t *hw, uint32_t value)
{
    hw->adc_config0.adc_hpf2_coef_b2 = value;
}

/* REG_0x04:adc_config0->adc_hpf2_bypass:0x4[16],ADC HPF2禁能位，1：绕开HPF2,0,RW*/
static inline uint32_t aud_ll_get_adc_config0_adc_hpf2_bypass(aud_hw_t *hw)
{
    return hw->adc_config0.adc_hpf2_bypass;
}

static inline void aud_ll_set_adc_config0_adc_hpf2_bypass(aud_hw_t *hw, uint32_t value)
{
    hw->adc_config0.adc_hpf2_bypass = value;
}

/* REG_0x04:adc_config0->adc_hpf1_bypass:0x4[17],ADC HPF1禁能位，1：绕开HPF1,0,RW*/
static inline uint32_t aud_ll_get_adc_config0_adc_hpf1_bypass(aud_hw_t *hw)
{
    return hw->adc_config0.adc_hpf1_bypass;
}

static inline void aud_ll_set_adc_config0_adc_hpf1_bypass(aud_hw_t *hw, uint32_t value)
{
    hw->adc_config0.adc_hpf1_bypass = value;
}

/* REG_0x04:adc_config0->adc_set_gain:0x4[23:18],ADC 设置的增益,0,RW*/
static inline uint32_t aud_ll_get_adc_config0_adc_set_gain(aud_hw_t *hw)
{
    return hw->adc_config0.adc_set_gain;
}

static inline void aud_ll_set_adc_config0_adc_set_gain(aud_hw_t *hw, uint32_t value)
{
    hw->adc_config0.adc_set_gain = value;
}

/* REG_0x04:adc_config0->adc_sampe_edge:0x4[24],ADC数据采样的时钟边沿选择 1：下降沿，0：上升沿,0,RW*/
static inline uint32_t aud_ll_get_adc_config0_adc_sampe_edge(aud_hw_t *hw)
{
    return hw->adc_config0.adc_sampe_edge;
}

static inline void aud_ll_set_adc_config0_adc_sampe_edge(aud_hw_t *hw, uint32_t value)
{
    hw->adc_config0.adc_sampe_edge = value;
}

/* REG_0x05 */

static inline uint32_t aud_ll_get_adc_config1_value(aud_hw_t *hw)
{
    return hw->adc_config1.v;
}

static inline void aud_ll_set_adc_config1_value(aud_hw_t *hw, uint32_t value)
{
    hw->adc_config1.v = value;
}

/* REG_0x05:adc_config1->adc_hpf2_coef_B0:0x5[15: 0],ADC HPF2的系数 B2 ,0,RW*/
static inline uint32_t aud_ll_get_adc_config1_adc_hpf2_coef_b0(aud_hw_t *hw)
{
    return hw->adc_config1.adc_hpf2_coef_b0;
}

static inline void aud_ll_set_adc_config1_adc_hpf2_coef_b0(aud_hw_t *hw, uint32_t value)
{
    hw->adc_config1.adc_hpf2_coef_b0 = value;
}

/* REG_0x05:adc_config1->adc_hpf2_coef_B1:0x5[31:16],ADC HPF2的系数 B1 ,0,RW*/
static inline uint32_t aud_ll_get_adc_config1_adc_hpf2_coef_b1(aud_hw_t *hw)
{
    return hw->adc_config1.adc_hpf2_coef_b1;
}

static inline void aud_ll_set_adc_config1_adc_hpf2_coef_b1(aud_hw_t *hw, uint32_t value)
{
    hw->adc_config1.adc_hpf2_coef_b1 = value;
}

/* REG_0x06 */

static inline uint32_t aud_ll_get_adc_config2_value(aud_hw_t *hw)
{
    return hw->adc_config2.v;
}

static inline void aud_ll_set_adc_config2_value(aud_hw_t *hw, uint32_t value)
{
    hw->adc_config2.v = value;
}

/* REG_0x06:adc_config2->adc_hpf2_coef_A0:0x6[15: 0],ADC HPF2的系数 A0 ,0,RW*/
static inline uint32_t aud_ll_get_adc_config2_adc_hpf2_coef_a0(aud_hw_t *hw)
{
    return hw->adc_config2.adc_hpf2_coef_a0;
}

static inline void aud_ll_set_adc_config2_adc_hpf2_coef_a0(aud_hw_t *hw, uint32_t value)
{
    hw->adc_config2.adc_hpf2_coef_a0 = value;
}

/* REG_0x06:adc_config2->adc_hpf2_coef_A1:0x6[31:16],ADC HPF2的系数 A1 ,0,RW*/
static inline uint32_t aud_ll_get_adc_config2_adc_hpf2_coef_a1(aud_hw_t *hw)
{
    return hw->adc_config2.adc_hpf2_coef_a1;
}

static inline void aud_ll_set_adc_config2_adc_hpf2_coef_a1(aud_hw_t *hw, uint32_t value)
{
    hw->adc_config2.adc_hpf2_coef_a1 = value;
}

/* REG_0x07 */

static inline uint32_t aud_ll_get_dac_config0_value(aud_hw_t *hw)
{
    return hw->dac_config0.v;
}

static inline void aud_ll_set_dac_config0_value(aud_hw_t *hw, uint32_t value)
{
    hw->dac_config0.v = value;
}

/* REG_0x07:dac_config0->dac_hpf2_coef_B2:0x7[15:0],DAC HPF2的系数 B2 ,0,RW*/
static inline uint32_t aud_ll_get_dac_config0_dac_hpf2_coef_b2(aud_hw_t *hw)
{
    return hw->dac_config0.dac_hpf2_coef_b2;
}

static inline void aud_ll_set_dac_config0_dac_hpf2_coef_b2(aud_hw_t *hw, uint32_t value)
{
    hw->dac_config0.dac_hpf2_coef_b2 = value;
}

/* REG_0x07:dac_config0->dac_hpf2_bypass:0x7[16],DAC HPF2禁能位，1：绕开HPF2,0,RW*/
static inline uint32_t aud_ll_get_dac_config0_dac_hpf2_bypass(aud_hw_t *hw)
{
    return hw->dac_config0.dac_hpf2_bypass;
}

static inline void aud_ll_set_dac_config0_dac_hpf2_bypass(aud_hw_t *hw, uint32_t value)
{
    hw->dac_config0.dac_hpf2_bypass = value;
}

/* REG_0x07:dac_config0->dac_hpf1_bypass:0x7[17],DAC HPF1禁能位，1：绕开HPF1 ,0,RW*/
static inline uint32_t aud_ll_get_dac_config0_dac_hpf1_bypass(aud_hw_t *hw)
{
    return hw->dac_config0.dac_hpf1_bypass;
}

static inline void aud_ll_set_dac_config0_dac_hpf1_bypass(aud_hw_t *hw, uint32_t value)
{
    hw->dac_config0.dac_hpf1_bypass = value;
}

/* REG_0x07:dac_config0->dac_set_gain:0x7[23:18],DAC 设置的增益,0,RW*/
static inline uint32_t aud_ll_get_dac_config0_dac_set_gain(aud_hw_t *hw)
{
    return hw->dac_config0.dac_set_gain;
}

static inline void aud_ll_set_dac_config0_dac_set_gain(aud_hw_t *hw, uint32_t value)
{
    hw->dac_config0.dac_set_gain = value;
}

/* REG_0x07:dac_config0->dac_clk_invert:0x7[24],DAC 输出时钟边沿选择 0：下降沿，1：上升沿,0,RW*/
static inline uint32_t aud_ll_get_dac_config0_dac_clk_invert(aud_hw_t *hw)
{
    return hw->dac_config0.dac_clk_invert;
}

static inline void aud_ll_set_dac_config0_dac_clk_invert(aud_hw_t *hw, uint32_t value)
{
    hw->dac_config0.dac_clk_invert = value;
}

/* REG_0x08 */

static inline uint32_t aud_ll_get_dac_config1_value(aud_hw_t *hw)
{
    return hw->dac_config1.v;
}

static inline void aud_ll_set_dac_config1_value(aud_hw_t *hw, uint32_t value)
{
    hw->dac_config1.v = value;
}

/* REG_0x08:dac_config1->dac_hpf2_coef_B0:0x8[15: 0],DAC HPF2的系数 B0 ,0,RW*/
static inline uint32_t aud_ll_get_dac_config1_dac_hpf2_coef_b0(aud_hw_t *hw)
{
    return hw->dac_config1.dac_hpf2_coef_b0;
}

static inline void aud_ll_set_dac_config1_dac_hpf2_coef_b0(aud_hw_t *hw, uint32_t value)
{
    hw->dac_config1.dac_hpf2_coef_b0 = value;
}

/* REG_0x08:dac_config1->dac_hpf2_coef_B1:0x8[31:16],DAC HPF2的系数 B1 ,0,RW*/
static inline uint32_t aud_ll_get_dac_config1_dac_hpf2_coef_b1(aud_hw_t *hw)
{
    return hw->dac_config1.dac_hpf2_coef_b1;
}

static inline void aud_ll_set_dac_config1_dac_hpf2_coef_b1(aud_hw_t *hw, uint32_t value)
{
    hw->dac_config1.dac_hpf2_coef_b1 = value;
}

/* REG_0x09 */

static inline uint32_t aud_ll_get_dac_config2_value(aud_hw_t *hw)
{
    return hw->dac_config2.v;
}

static inline void aud_ll_set_dac_config2_value(aud_hw_t *hw, uint32_t value)
{
    hw->dac_config2.v = value;
}

/* REG_0x09:dac_config2->dac_hpf2_coef_A1:0x9[15: 0],DAC HPF2的系数 A1 ,0,RW*/
static inline uint32_t aud_ll_get_dac_config2_dac_hpf2_coef_a1(aud_hw_t *hw)
{
    return hw->dac_config2.dac_hpf2_coef_a1;
}

static inline void aud_ll_set_dac_config2_dac_hpf2_coef_a1(aud_hw_t *hw, uint32_t value)
{
    hw->dac_config2.dac_hpf2_coef_a1 = value;
}

/* REG_0x09:dac_config2->dac_hpf2_coef_A2:0x9[31:16],DAC HPF2的系数 A2 ,0,RW*/
static inline uint32_t aud_ll_get_dac_config2_dac_hpf2_coef_a2(aud_hw_t *hw)
{
    return hw->dac_config2.dac_hpf2_coef_a2;
}

static inline void aud_ll_set_dac_config2_dac_hpf2_coef_a2(aud_hw_t *hw, uint32_t value)
{
    hw->dac_config2.dac_hpf2_coef_a2 = value;
}

/* REG_0x0a */

static inline uint32_t aud_ll_get_fifo_config_value(aud_hw_t *hw)
{
    return hw->fifo_config.v;
}

static inline void aud_ll_set_fifo_config_value(aud_hw_t *hw, uint32_t value)
{
    hw->fifo_config.v = value;
}

/* REG_0x0a:fifo_config->dacr_rd_threshold:0xa[4:0],DAC右声道读FIFO时，数目低于该门限，发出中断请求,0,RW*/
static inline uint32_t aud_ll_get_fifo_config_dacr_rd_threshold(aud_hw_t *hw)
{
    return hw->fifo_config.dacr_rd_threshold;
}

static inline void aud_ll_set_fifo_config_dacr_rd_threshold(aud_hw_t *hw, uint32_t value)
{
    hw->fifo_config.dacr_rd_threshold = value;
}

/* REG_0x0a:fifo_config->dacl_rd_threshold:0xa[9:5],DAC左声道读FIFO时，数目低于该门限，发出中断请求,0,RW*/
static inline uint32_t aud_ll_get_fifo_config_dacl_rd_threshold(aud_hw_t *hw)
{
    return hw->fifo_config.dacl_rd_threshold;
}

static inline void aud_ll_set_fifo_config_dacl_rd_threshold(aud_hw_t *hw, uint32_t value)
{
    hw->fifo_config.dacl_rd_threshold = value;
}

/* REG_0x0a:fifo_config->dtmf_wr_threshold:0xa[14:10],DTMF自动写FIFO时，数目高于该门限，发出中断请求,0,RW*/
static inline uint32_t aud_ll_get_fifo_config_dtmf_wr_threshold(aud_hw_t *hw)
{
    return hw->fifo_config.dtmf_wr_threshold;
}

static inline void aud_ll_set_fifo_config_dtmf_wr_threshold(aud_hw_t *hw, uint32_t value)
{
    hw->fifo_config.dtmf_wr_threshold = value;
}

/* REG_0x0a:fifo_config->adcl_wr_threshold:0xa[19:15],ADC自动写FIFO时，数目高于该门限，发出中断请求,0,RW*/
static inline uint32_t aud_ll_get_fifo_config_adcl_wr_threshold(aud_hw_t *hw)
{
    return hw->fifo_config.adcl_wr_threshold;
}

static inline void aud_ll_set_fifo_config_adcl_wr_threshold(aud_hw_t *hw, uint32_t value)
{
    hw->fifo_config.adcl_wr_threshold = value;
}

/* REG_0x0a:fifo_config->dacr_int_en:0xa[20],DAC右声道中断请求使能位,0,RW*/
static inline uint32_t aud_ll_get_fifo_config_dacr_int_en(aud_hw_t *hw)
{
    return hw->fifo_config.dacr_int_en;
}

static inline void aud_ll_set_fifo_config_dacr_int_en(aud_hw_t *hw, uint32_t value)
{
    hw->fifo_config.dacr_int_en = value;
}

/* REG_0x0a:fifo_config->dacl_int_en:0xa[21],DAC左声道中断请求使能位,0,RW*/
static inline uint32_t aud_ll_get_fifo_config_dacl_int_en(aud_hw_t *hw)
{
    return hw->fifo_config.dacl_int_en;
}

static inline void aud_ll_set_fifo_config_dacl_int_en(aud_hw_t *hw, uint32_t value)
{
    hw->fifo_config.dacl_int_en = value;
}

/* REG_0x0a:fifo_config->dtmf_int_en:0xa[22],DTMF中断请求使能位,0,RW*/
static inline uint32_t aud_ll_get_fifo_config_dtmf_int_en(aud_hw_t *hw)
{
    return hw->fifo_config.dtmf_int_en;
}

static inline void aud_ll_set_fifo_config_dtmf_int_en(aud_hw_t *hw, uint32_t value)
{
    hw->fifo_config.dtmf_int_en = value;
}

/* REG_0x0a:fifo_config->adcl_int_en:0xa[23],ADC中断请求使能位,0,RW*/
static inline uint32_t aud_ll_get_fifo_config_adcl_int_en(aud_hw_t *hw)
{
    return hw->fifo_config.adcl_int_en;
}

static inline void aud_ll_set_fifo_config_adcl_int_en(aud_hw_t *hw, uint32_t value)
{
    hw->fifo_config.adcl_int_en = value;
}

/* REG_0x0a:fifo_config->loop_ton2dac:0xa[24],DTMF到DAC环回测试使能位,0,RW*/
static inline uint32_t aud_ll_get_fifo_config_loop_ton2dac(aud_hw_t *hw)
{
    return hw->fifo_config.loop_ton2dac;
}

static inline void aud_ll_set_fifo_config_loop_ton2dac(aud_hw_t *hw, uint32_t value)
{
    hw->fifo_config.loop_ton2dac = value;
}

/* REG_0x0a:fifo_config->loop_adc2dac:0xa[25],ADC到DAC环回测试使能位,0,RW*/
static inline uint32_t aud_ll_get_fifo_config_loop_adc2dac(aud_hw_t *hw)
{
    return hw->fifo_config.loop_adc2dac;
}

static inline void aud_ll_set_fifo_config_loop_adc2dac(aud_hw_t *hw, uint32_t value)
{
    hw->fifo_config.loop_adc2dac = value;
}

/* REG_0x0b */

static inline uint32_t aud_ll_get_agc_config0_value(aud_hw_t *hw)
{
    return hw->agc_config0.v;
}

static inline void aud_ll_set_agc_config0_value(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config0.v = value;
}

/* REG_0x0b:agc_config0->agc_noise_thrd:0xb[ 9: 0],noise gating 执行时刻的门限，noise gating要在audio值比较小的时候执行,0,RW*/
static inline uint32_t aud_ll_get_agc_config0_agc_noise_thrd(aud_hw_t *hw)
{
    return hw->agc_config0.agc_noise_thrd;
}

static inline void aud_ll_set_agc_config0_agc_noise_thrd(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config0.agc_noise_thrd = value;
}

/* REG_0x0b:agc_config0->agc_noise_high:0xb[19:10],对应MIC_RSSI低10 bit,0,RW*/
static inline uint32_t aud_ll_get_agc_config0_agc_noise_high(aud_hw_t *hw)
{
    return hw->agc_config0.agc_noise_high;
}

static inline void aud_ll_set_agc_config0_agc_noise_high(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config0.agc_noise_high = value;
}

/* REG_0x0b:agc_config0->agc_noise_low:0xb[29:20],对应MIC_RSSI低10 bit，比如MIC_RSSI[15:10]任何一位为1，都说明信号电平超过NOISE_LOW,0,RW*/
static inline uint32_t aud_ll_get_agc_config0_agc_noise_low(aud_hw_t *hw)
{
    return hw->agc_config0.agc_noise_low;
}

static inline void aud_ll_set_agc_config0_agc_noise_low(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config0.agc_noise_low = value;
}

/* REG_0x0b:agc_config0->agc_step:0xb[31:30],没有用,0,RW*/
static inline uint32_t aud_ll_get_agc_config0_agc_step(aud_hw_t *hw)
{
    return hw->agc_config0.agc_step;
}

static inline void aud_ll_set_agc_config0_agc_step(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config0.agc_step = value;
}

/* REG_0x0c */

static inline uint32_t aud_ll_get_agc_config1_value(aud_hw_t *hw)
{
    return hw->agc_config1.v;
}

static inline void aud_ll_set_agc_config1_value(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config1.v = value;
}

/* REG_0x0c:agc_config1->agc_noise_min:0xc[6:0],{GAIN2[2:0] GAIN1[3:0]} when signal level below NOISE_LOW, when noise gating is enabled,0,RW*/
static inline uint32_t aud_ll_get_agc_config1_agc_noise_min(aud_hw_t *hw)
{
    return hw->agc_config1.agc_noise_min;
}

static inline void aud_ll_set_agc_config1_agc_noise_min(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config1.agc_noise_min = value;
}

/* REG_0x0c:agc_config1->agc_noise_tout:0xc[9:7],1: 4 (~0.5 ms), 2: 8, 3: 16, 4: 32, 5: 64, 6: 128, 7: 256 (~32 ms),0,RW*/
static inline uint32_t aud_ll_get_agc_config1_agc_noise_tout(aud_hw_t *hw)
{
    return hw->agc_config1.agc_noise_tout;
}

static inline void aud_ll_set_agc_config1_agc_noise_tout(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config1.agc_noise_tout = value;
}

/* REG_0x0c:agc_config1->agc_high_dur:0xc[12:10],0: 0 (0 ms，一旦发生MIC_RSSI超过NOISE_HIGH，立刻触发Noise gating，离开noise状态); 1: 8 (~1 ms), 2: 16, 3: 32, 4: 64, 5: 128, 6: 256, 7: 256 (~32 ms),0,RW*/
static inline uint32_t aud_ll_get_agc_config1_agc_high_dur(aud_hw_t *hw)
{
    return hw->agc_config1.agc_high_dur;
}

static inline void aud_ll_set_agc_config1_agc_high_dur(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config1.agc_high_dur = value;
}

/* REG_0x0c:agc_config1->agc_low_dur:0xc[15:13],0: 0 (0 ms，一旦发生MIC_RSSI小于NOISE_LOW，立刻触发noise gating); 1: 8 (~1 ms), 2: 16, 3: 32, 4: 64, 5: 128, 6: 256, 7: 256 (~32 ms),0,RW*/
static inline uint32_t aud_ll_get_agc_config1_agc_low_dur(aud_hw_t *hw)
{
    return hw->agc_config1.agc_low_dur;
}

static inline void aud_ll_set_agc_config1_agc_low_dur(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config1.agc_low_dur = value;
}

/* REG_0x0c:agc_config1->agc_min:0xc[22:16],Minimum value of {GAIN2[2:0] GAIN1[3:0]},0,RW*/
static inline uint32_t aud_ll_get_agc_config1_agc_min(aud_hw_t *hw)
{
    return hw->agc_config1.agc_min;
}

static inline void aud_ll_set_agc_config1_agc_min(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config1.agc_min = value;
}

/* REG_0x0c:agc_config1->agc_max:0xc[29:23],Maximum value of {GAIN2[2:0] GAIN1[3:0]}; Also the  default gain setting when AGC is disabled,0,RW*/
static inline uint32_t aud_ll_get_agc_config1_agc_max(aud_hw_t *hw)
{
    return hw->agc_config1.agc_max;
}

static inline void aud_ll_set_agc_config1_agc_max(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config1.agc_max = value;
}

/* REG_0x0c:agc_config1->agc_ng_method:0xc[30],0: 直接mute或者demute (default); 1: 逐渐降低增益或者增加增益,0,RW*/
static inline uint32_t aud_ll_get_agc_config1_agc_ng_method(aud_hw_t *hw)
{
    return hw->agc_config1.agc_ng_method;
}

static inline void aud_ll_set_agc_config1_agc_ng_method(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config1.agc_ng_method = value;
}

/* REG_0x0c:agc_config1->agc_ng_enable:0xc[31],使能 noise gating,0,RW*/
static inline uint32_t aud_ll_get_agc_config1_agc_ng_enable(aud_hw_t *hw)
{
    return hw->agc_config1.agc_ng_enable;
}

static inline void aud_ll_set_agc_config1_agc_ng_enable(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config1.agc_ng_enable = value;
}

/* REG_0x0d */

static inline uint32_t aud_ll_get_agc_config2_value(aud_hw_t *hw)
{
    return hw->agc_config2.v;
}

static inline void aud_ll_set_agc_config2_value(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config2.v = value;
}

/* REG_0x0d:agc_config2->agc_decay_time:0xd[2:0],0: 128 (~2.7 ms), 1: 256, 2: 512, 3: 1024, 4:2048, 5: 4096, 6: 8192, 7: 16384 (~340 ms),0,RW*/
static inline uint32_t aud_ll_get_agc_config2_agc_decay_time(aud_hw_t *hw)
{
    return hw->agc_config2.agc_decay_time;
}

static inline void aud_ll_set_agc_config2_agc_decay_time(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config2.agc_decay_time = value;
}

/* REG_0x0d:agc_config2->agc_attack_time:0xd[5:3],0: 8 (~0.1667ms), 1: 16, 2: 32, 3: 64, 4: 128, 5: 256, 6: 512, 7: 1024 (~21 ms),0,RW*/
static inline uint32_t aud_ll_get_agc_config2_agc_attack_time(aud_hw_t *hw)
{
    return hw->agc_config2.agc_attack_time;
}

static inline void aud_ll_set_agc_config2_agc_attack_time(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config2.agc_attack_time = value;
}

/* REG_0x0d:agc_config2->agc_high_thrd:0xd[10:6],对应到MIC_RSSI的最高5位,0,RW*/
static inline uint32_t aud_ll_get_agc_config2_agc_high_thrd(aud_hw_t *hw)
{
    return hw->agc_config2.agc_high_thrd;
}

static inline void aud_ll_set_agc_config2_agc_high_thrd(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config2.agc_high_thrd = value;
}

/* REG_0x0d:agc_config2->agc_low_thrd:0xd[15:11],对应到MIC_RSSI的最高5位,0,RW*/
static inline uint32_t aud_ll_get_agc_config2_agc_low_thrd(aud_hw_t *hw)
{
    return hw->agc_config2.agc_low_thrd;
}

static inline void aud_ll_set_agc_config2_agc_low_thrd(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config2.agc_low_thrd = value;
}

/* REG_0x0d:agc_config2->agc_iir_coef:0xd[18:16],IIR系数选择 0:1/32 1:1/64 2:1/128 3:1/256 4:1/512 5:1/1024 6:1/2048 7:1/4096,0,RW*/
static inline uint32_t aud_ll_get_agc_config2_agc_iir_coef(aud_hw_t *hw)
{
    return hw->agc_config2.agc_iir_coef;
}

static inline void aud_ll_set_agc_config2_agc_iir_coef(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config2.agc_iir_coef = value;
}

/* REG_0x0d:agc_config2->agc_enable:0xd[19],AGC使能位,0,RW*/
static inline uint32_t aud_ll_get_agc_config2_agc_enable(aud_hw_t *hw)
{
    return hw->agc_config2.agc_enable;
}

static inline void aud_ll_set_agc_config2_agc_enable(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config2.agc_enable = value;
}

/* REG_0x0d:agc_config2->manual_pga_value:0xd[26:20],手动PGA的值,0,RW*/
static inline uint32_t aud_ll_get_agc_config2_manual_pga_value(aud_hw_t *hw)
{
    return hw->agc_config2.manual_pga_value;
}

static inline void aud_ll_set_agc_config2_manual_pga_value(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config2.manual_pga_value = value;
}

/* REG_0x0d:agc_config2->manual_pga:0xd[27],手动PGA模式,0,RW*/
static inline uint32_t aud_ll_get_agc_config2_manual_pga(aud_hw_t *hw)
{
    return hw->agc_config2.manual_pga;
}

static inline void aud_ll_set_agc_config2_manual_pga(aud_hw_t *hw, uint32_t value)
{
    hw->agc_config2.manual_pga = value;
}

/* REG_0x0e */

static inline uint32_t aud_ll_get_fifo_status_value(aud_hw_t *hw)
{
    return hw->fifo_status.v;
}

/* REG_0x0e:fifo_status->dacr_near_full:0xe[0],DAC右声道FIFO快要满  （默认大于3/4的容量）,0,R*/
static inline uint32_t aud_ll_get_fifo_status_dacr_near_full(aud_hw_t *hw)
{
    return hw->fifo_status.dacr_near_full;
}

/* REG_0x0e:fifo_status->dacl_near_full:0xe[1],DAC左声道FIFO快要满,0,R*/
static inline uint32_t aud_ll_get_fifo_status_dacl_near_full(aud_hw_t *hw)
{
    return hw->fifo_status.dacl_near_full;
}

/* REG_0x0e:fifo_status->adcl_near_full:0xe[2],ADC左声道FIFO快要满,0,R*/
static inline uint32_t aud_ll_get_fifo_status_adcl_near_full(aud_hw_t *hw)
{
    return hw->fifo_status.adcl_near_full;
}

/* REG_0x0e:fifo_status->dtmf_near_full:0xe[3],DTMF的FIFO快要满,0,R*/
static inline uint32_t aud_ll_get_fifo_status_dtmf_near_full(aud_hw_t *hw)
{
    return hw->fifo_status.dtmf_near_full;
}

/* REG_0x0e:fifo_status->dacr_near_empty:0xe[4],DAC右声道FIFO快要空  （默认小于1/4的容量）,0,R*/
static inline uint32_t aud_ll_get_fifo_status_dacr_near_empty(aud_hw_t *hw)
{
    return hw->fifo_status.dacr_near_empty;
}

/* REG_0x0e:fifo_status->dacl_near_empty:0xe[5],DAC左声道FIFO快要空,0,R*/
static inline uint32_t aud_ll_get_fifo_status_dacl_near_empty(aud_hw_t *hw)
{
    return hw->fifo_status.dacl_near_empty;
}

/* REG_0x0e:fifo_status->adcl_near_empty:0xe[6],ADC左声道FIFO快要空,0,R*/
static inline uint32_t aud_ll_get_fifo_status_adcl_near_empty(aud_hw_t *hw)
{
    return hw->fifo_status.adcl_near_empty;
}

/* REG_0x0e:fifo_status->dtmf_near_empty:0xe[7],DTMF的FIFO快要空,0,R*/
static inline uint32_t aud_ll_get_fifo_status_dtmf_near_empty(aud_hw_t *hw)
{
    return hw->fifo_status.dtmf_near_empty;
}

/* REG_0x0e:fifo_status->dacr_fifo_full:0xe[8],DAC右声道FIFO已满    (达到FIFO容量的最大值）,0,R*/
static inline uint32_t aud_ll_get_fifo_status_dacr_fifo_full(aud_hw_t *hw)
{
    return hw->fifo_status.dacr_fifo_full;
}

/* REG_0x0e:fifo_status->dacl_fifo_full:0xe[9],DAC左声道FIFO已满,0,R*/
static inline uint32_t aud_ll_get_fifo_status_dacl_fifo_full(aud_hw_t *hw)
{
    return hw->fifo_status.dacl_fifo_full;
}

/* REG_0x0e:fifo_status->adcl_fifo_full:0xe[10],ADC左声道FIFO已满,0,R*/
static inline uint32_t aud_ll_get_fifo_status_adcl_fifo_full(aud_hw_t *hw)
{
    return hw->fifo_status.adcl_fifo_full;
}

/* REG_0x0e:fifo_status->dtmf_fifo_full:0xe[11],DTMF的FIFO已满,0,R*/
static inline uint32_t aud_ll_get_fifo_status_dtmf_fifo_full(aud_hw_t *hw)
{
    return hw->fifo_status.dtmf_fifo_full;
}

/* REG_0x0e:fifo_status->dacr_fifo_empty:0xe[12],DAC右声道FIFO已空   (达到FIFO容量的最小值）,0,R*/
static inline uint32_t aud_ll_get_fifo_status_dacr_fifo_empty(aud_hw_t *hw)
{
    return hw->fifo_status.dacr_fifo_empty;
}

/* REG_0x0e:fifo_status->dacl_fifo_empty:0xe[13],DAC左声道FIFO已空,0,R*/
static inline uint32_t aud_ll_get_fifo_status_dacl_fifo_empty(aud_hw_t *hw)
{
    return hw->fifo_status.dacl_fifo_empty;
}

/* REG_0x0e:fifo_status->adcl_fifo_empty:0xe[14],ADC左声道FIFO已空,0,R*/
static inline uint32_t aud_ll_get_fifo_status_adcl_fifo_empty(aud_hw_t *hw)
{
    return hw->fifo_status.adcl_fifo_empty;
}

/* REG_0x0e:fifo_status->dtmf_fifo_empty:0xe[15],DTMF的FIFO已空,0,R*/
static inline uint32_t aud_ll_get_fifo_status_dtmf_fifo_empty(aud_hw_t *hw)
{
    return hw->fifo_status.dtmf_fifo_empty;
}

/* REG_0x0e:fifo_status->dacr_int_flag:0xe[16],DAC右声道中断标志,0,R*/
static inline uint32_t aud_ll_get_fifo_status_dacr_int_flag(aud_hw_t *hw)
{
    return hw->fifo_status.dacr_int_flag;
}

/* REG_0x0e:fifo_status->dacl_int_flag:0xe[17],DAC左声道中断标志,0,R*/
static inline uint32_t aud_ll_get_fifo_status_dacl_int_flag(aud_hw_t *hw)
{
    return hw->fifo_status.dacl_int_flag;
}

/* REG_0x0e:fifo_status->adcl_int_flag:0xe[18],ADC左声道中断标志,0,R*/
static inline uint32_t aud_ll_get_fifo_status_adcl_int_flag(aud_hw_t *hw)
{
    return hw->fifo_status.adcl_int_flag;
}

/* REG_0x0e:fifo_status->dtmf_int_flag:0xe[19],DTMF的中断标志,0,R*/
static inline uint32_t aud_ll_get_fifo_status_dtmf_int_flag(aud_hw_t *hw)
{
    return hw->fifo_status.dtmf_int_flag;
}

/* REG_0x0f */

static inline uint32_t aud_ll_get_agc_status_value(aud_hw_t *hw)
{
    return hw->agc_status.v;
}

/* REG_0x0f:agc_status->rssi:0xf[7:0],Absolutely microphone signal power in dB unit; Read only,0,R*/
static inline uint32_t aud_ll_get_agc_status_rssi(aud_hw_t *hw)
{
    return hw->agc_status.rssi;
}

/* REG_0x0f:agc_status->mic_pga:0xf[15:8],{1'b0, GAIN2[2:0] GAIN1[3:0]} (Read only),0,R*/
static inline uint32_t aud_ll_get_agc_status_mic_pga(aud_hw_t *hw)
{
    return hw->agc_status.mic_pga;
}

/* REG_0x0f:agc_status->mic_rssi:0xf[31:16],Microphone level; MIC_RSSI[15:0]; Read only,0,R*/
static inline uint32_t aud_ll_get_agc_status_mic_rssi(aud_hw_t *hw)
{
    return hw->agc_status.mic_rssi;
}

/* REG_0x10 */

static inline uint32_t aud_ll_get_dtmf_fport_value(aud_hw_t *hw)
{
    return hw->dtmf_fport.v;
}

/* REG_0x10:dtmf_fport->dtmf_port:0x10[15:0],DTMF的FIFO读出地址,0,R*/
static inline uint32_t aud_ll_get_dtmf_fport_dtmf_port(aud_hw_t *hw)
{
    return hw->dtmf_fport.dtmf_port;
}

/* REG_0x11 */

static inline uint32_t aud_ll_get_adc_fport_value(aud_hw_t *hw)
{
    return hw->adc_fport.v;
}

/* REG_0x11:adc_fport->adc_port:0x11[15:0],ADC的FIFO读出地址,0,R*/
static inline uint32_t aud_ll_get_adc_fport_adc_port(aud_hw_t *hw)
{
    return hw->adc_fport.adc_port;
}

/* REG_0x12 */

static inline void aud_ll_set_dacl_fport_value(aud_hw_t *hw, uint32_t value)
{
    hw->dacl_fport.v = value;
}

/* REG_0x12:dacl_fport->dacl_port:0x12[15:0],DACL的FIFO写入地址,0,W*/
static inline void aud_ll_set_dacl_fport_dacl_port(aud_hw_t *hw, uint32_t value)
{
    hw->dacl_fport.dacl_port = value;
}

/* REG_0x12:dacl_fport->dacr_port:0x12[31:16],DACR的FIFO写入地址,0,W*/
static inline void aud_ll_set_dacl_fport_dacr_port(aud_hw_t *hw, uint32_t value)
{
    hw->dacl_fport.dacr_port = value;
}

/* REG_0x18 */

static inline uint32_t aud_ll_get_extend_cfg_value(aud_hw_t *hw)
{
    return hw->extend_cfg.v;
}

static inline void aud_ll_set_extend_cfg_value(aud_hw_t *hw, uint32_t value)
{
    hw->extend_cfg.v = value;
}

/* REG_0x18:extend_cfg->dac_fracmod_manual:0x18[0],DAC小数分频手动设置使能,0,RW*/
static inline uint32_t aud_ll_get_extend_cfg_dac_fracmod_manual(aud_hw_t *hw)
{
    return hw->extend_cfg.dac_fracmod_manual;
}

static inline void aud_ll_set_extend_cfg_dac_fracmod_manual(aud_hw_t *hw, uint32_t value)
{
    hw->extend_cfg.dac_fracmod_manual = value;
}

/* REG_0x18:extend_cfg->adc_fracmod_manual:0x18[1],ADC小数分频手动设置使能,0,RW*/
static inline uint32_t aud_ll_get_extend_cfg_adc_fracmod_manual(aud_hw_t *hw)
{
    return hw->extend_cfg.adc_fracmod_manual;
}

static inline void aud_ll_set_extend_cfg_adc_fracmod_manual(aud_hw_t *hw, uint32_t value)
{
    hw->extend_cfg.adc_fracmod_manual = value;
}

/* REG_0x18:extend_cfg->filt_enable:0x18[2],DAC的频响滤波器使能,0,RW*/
static inline uint32_t aud_ll_get_extend_cfg_filt_enable(aud_hw_t *hw)
{
    return hw->extend_cfg.filt_enable;
}

static inline void aud_ll_set_extend_cfg_filt_enable(aud_hw_t *hw, uint32_t value)
{
    hw->extend_cfg.filt_enable = value;
}

/* REG_0x19 */

static inline uint32_t aud_ll_get_dac_fracmod_value(aud_hw_t *hw)
{
    return hw->dac_fracmod.v;
}

static inline void aud_ll_set_dac_fracmod_value(aud_hw_t *hw, uint32_t value)
{
    hw->dac_fracmod.v = value;
}

/* REG_0x19:dac_fracmod->dac_fracCoef:0x19[31:0],DAC小数分频设置值 N * 2^24,0,RW*/
static inline uint32_t aud_ll_get_dac_fracmod_dac_fraccoef(aud_hw_t *hw)
{
    return hw->dac_fracmod.v;
}

static inline void aud_ll_set_dac_fracmod_dac_fraccoef(aud_hw_t *hw, uint32_t value)
{
    hw->dac_fracmod.v = value;
}

/* REG_0x1a */

static inline uint32_t aud_ll_get_adc_fracmod_value(aud_hw_t *hw)
{
    return hw->adc_fracmod.v;
}

static inline void aud_ll_set_adc_fracmod_value(aud_hw_t *hw, uint32_t value)
{
    hw->adc_fracmod.v = value;
}

/* REG_0x1a:adc_fracmod->adc_fracCoef:0x1a[31:0],ADC小数分频设置值 N * 2^24,0,RW*/
static inline uint32_t aud_ll_get_adc_fracmod_adc_fraccoef(aud_hw_t *hw)
{
    return hw->adc_fracmod.v;
}

static inline void aud_ll_set_adc_fracmod_adc_fraccoef(aud_hw_t *hw, uint32_t value)
{
    hw->adc_fracmod.v = value;
}

/* REG_0x1f */

static inline uint32_t aud_ll_get_hpf2_ext_coef_value(aud_hw_t *hw)
{
    return hw->hpf2_ext_coef.v;
}

static inline void aud_ll_set_hpf2_ext_coef_value(aud_hw_t *hw, uint32_t value)
{
    hw->hpf2_ext_coef.v = value;
}

/* REG_0x1f:hpf2_ext_coef->hpf2_A1_L6BIT:0x1f[5:0],DAC HPF2的系数A1低6比特,0,RW*/
static inline uint32_t aud_ll_get_hpf2_ext_coef_hpf2_a1_l6bit(aud_hw_t *hw)
{
    return hw->hpf2_ext_coef.hpf2_a1_l6bit;
}

static inline void aud_ll_set_hpf2_ext_coef_hpf2_a1_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->hpf2_ext_coef.hpf2_a1_l6bit = value;
}

/* REG_0x1f:hpf2_ext_coef->hpf2_A2_L6BIT:0x1f[11:6],DAC HPF2的系数A2低6比特,0,RW*/
static inline uint32_t aud_ll_get_hpf2_ext_coef_hpf2_a2_l6bit(aud_hw_t *hw)
{
    return hw->hpf2_ext_coef.hpf2_a2_l6bit;
}

static inline void aud_ll_set_hpf2_ext_coef_hpf2_a2_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->hpf2_ext_coef.hpf2_a2_l6bit = value;
}

/* REG_0x1f:hpf2_ext_coef->hpf2_B0_L6BIT:0x1f[17:12],DAC HPF2的系数B0低6比特,0,RW*/
static inline uint32_t aud_ll_get_hpf2_ext_coef_hpf2_b0_l6bit(aud_hw_t *hw)
{
    return hw->hpf2_ext_coef.hpf2_b0_l6bit;
}

static inline void aud_ll_set_hpf2_ext_coef_hpf2_b0_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->hpf2_ext_coef.hpf2_b0_l6bit = value;
}

/* REG_0x1f:hpf2_ext_coef->hpf2_B1_L6BIT:0x1f[23:18],DAC HPF2的系数B1低6比特,0,RW*/
static inline uint32_t aud_ll_get_hpf2_ext_coef_hpf2_b1_l6bit(aud_hw_t *hw)
{
    return hw->hpf2_ext_coef.hpf2_b1_l6bit;
}

static inline void aud_ll_set_hpf2_ext_coef_hpf2_b1_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->hpf2_ext_coef.hpf2_b1_l6bit = value;
}

/* REG_0x1f:hpf2_ext_coef->hpf2_B2_L6BIT:0x1f[29:24],DAC HPF2的系数B2低6比特,0,RW*/
static inline uint32_t aud_ll_get_hpf2_ext_coef_hpf2_b2_l6bit(aud_hw_t *hw)
{
    return hw->hpf2_ext_coef.hpf2_b2_l6bit;
}

static inline void aud_ll_set_hpf2_ext_coef_hpf2_b2_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->hpf2_ext_coef.hpf2_b2_l6bit = value;
}

/* REG_0x20 */

static inline uint32_t aud_ll_get_flt0_coef_a1a2_value(aud_hw_t *hw)
{
    return hw->flt0_coef_a1a2.v;
}

static inline void aud_ll_set_flt0_coef_a1a2_value(aud_hw_t *hw, uint32_t value)
{
    hw->flt0_coef_a1a2.v = value;
}

/* REG_0x20:flt0_coef_a1a2->flt0_A1:0x20[15: 0],Filter 0 的IIR系数A1,0,RW*/
static inline uint32_t aud_ll_get_flt0_coef_a1a2_flt0_a1(aud_hw_t *hw)
{
    return hw->flt0_coef_a1a2.flt0_a1;
}

static inline void aud_ll_set_flt0_coef_a1a2_flt0_a1(aud_hw_t *hw, uint32_t value)
{
    hw->flt0_coef_a1a2.flt0_a1 = value;
}

/* REG_0x20:flt0_coef_a1a2->flt0_A2:0x20[31:16],Filter 0 的IIR系数A2,0,RW*/
static inline uint32_t aud_ll_get_flt0_coef_a1a2_flt0_a2(aud_hw_t *hw)
{
    return hw->flt0_coef_a1a2.flt0_a2;
}

static inline void aud_ll_set_flt0_coef_a1a2_flt0_a2(aud_hw_t *hw, uint32_t value)
{
    hw->flt0_coef_a1a2.flt0_a2 = value;
}

/* REG_0x21 */

static inline uint32_t aud_ll_get_flt0_coef_b0b1_value(aud_hw_t *hw)
{
    return hw->flt0_coef_b0b1.v;
}

static inline void aud_ll_set_flt0_coef_b0b1_value(aud_hw_t *hw, uint32_t value)
{
    hw->flt0_coef_b0b1.v = value;
}

/* REG_0x21:flt0_coef_b0b1->flt0_B0:0x21[15: 0],Filter 0 的IIR系数B0,0,RW*/
static inline uint32_t aud_ll_get_flt0_coef_b0b1_flt0_b0(aud_hw_t *hw)
{
    return hw->flt0_coef_b0b1.flt0_b0;
}

static inline void aud_ll_set_flt0_coef_b0b1_flt0_b0(aud_hw_t *hw, uint32_t value)
{
    hw->flt0_coef_b0b1.flt0_b0 = value;
}

/* REG_0x21:flt0_coef_b0b1->flt0_B1:0x21[31:16],Filter 0 的IIR系数B1,0,RW*/
static inline uint32_t aud_ll_get_flt0_coef_b0b1_flt0_b1(aud_hw_t *hw)
{
    return hw->flt0_coef_b0b1.flt0_b1;
}

static inline void aud_ll_set_flt0_coef_b0b1_flt0_b1(aud_hw_t *hw, uint32_t value)
{
    hw->flt0_coef_b0b1.flt0_b1 = value;
}

/* REG_0x22 */

static inline uint32_t aud_ll_get_flt0_coef_b2_value(aud_hw_t *hw)
{
    return hw->flt0_coef_b2.v;
}

static inline void aud_ll_set_flt0_coef_b2_value(aud_hw_t *hw, uint32_t value)
{
    hw->flt0_coef_b2.v = value;
}

/* REG_0x22:flt0_coef_b2->flt0_B2:0x22[15: 0],Filter 0 的IIR系数B2,0,RW*/
static inline uint32_t aud_ll_get_flt0_coef_b2_flt0_b2(aud_hw_t *hw)
{
    return hw->flt0_coef_b2.flt0_b2;
}

static inline void aud_ll_set_flt0_coef_b2_flt0_b2(aud_hw_t *hw, uint32_t value)
{
    hw->flt0_coef_b2.flt0_b2 = value;
}

/* REG_0x23 */

static inline uint32_t aud_ll_get_flt1_coef_a1a2_value(aud_hw_t *hw)
{
    return hw->flt1_coef_a1a2.v;
}

static inline void aud_ll_set_flt1_coef_a1a2_value(aud_hw_t *hw, uint32_t value)
{
    hw->flt1_coef_a1a2.v = value;
}

/* REG_0x23:flt1_coef_a1a2->flt1_A1:0x23[15: 0],Filter 1 的IIR系数A1,0,RW*/
static inline uint32_t aud_ll_get_flt1_coef_a1a2_flt1_a1(aud_hw_t *hw)
{
    return hw->flt1_coef_a1a2.flt1_a1;
}

static inline void aud_ll_set_flt1_coef_a1a2_flt1_a1(aud_hw_t *hw, uint32_t value)
{
    hw->flt1_coef_a1a2.flt1_a1 = value;
}

/* REG_0x23:flt1_coef_a1a2->flt1_A2:0x23[31:16],Filter 1 的IIR系数A2,0,RW*/
static inline uint32_t aud_ll_get_flt1_coef_a1a2_flt1_a2(aud_hw_t *hw)
{
    return hw->flt1_coef_a1a2.flt1_a2;
}

static inline void aud_ll_set_flt1_coef_a1a2_flt1_a2(aud_hw_t *hw, uint32_t value)
{
    hw->flt1_coef_a1a2.flt1_a2 = value;
}

/* REG_0x24 */

static inline uint32_t aud_ll_get_flt1_coef_b0b1_value(aud_hw_t *hw)
{
    return hw->flt1_coef_b0b1.v;
}

static inline void aud_ll_set_flt1_coef_b0b1_value(aud_hw_t *hw, uint32_t value)
{
    hw->flt1_coef_b0b1.v = value;
}

/* REG_0x24:flt1_coef_b0b1->flt1_B0:0x24[15: 0],Filter 1 的IIR系数B0,0,RW*/
static inline uint32_t aud_ll_get_flt1_coef_b0b1_flt1_b0(aud_hw_t *hw)
{
    return hw->flt1_coef_b0b1.flt1_b0;
}

static inline void aud_ll_set_flt1_coef_b0b1_flt1_b0(aud_hw_t *hw, uint32_t value)
{
    hw->flt1_coef_b0b1.flt1_b0 = value;
}

/* REG_0x24:flt1_coef_b0b1->flt1_B1:0x24[31:16],Filter 1 的IIR系数B1,0,RW*/
static inline uint32_t aud_ll_get_flt1_coef_b0b1_flt1_b1(aud_hw_t *hw)
{
    return hw->flt1_coef_b0b1.flt1_b1;
}

static inline void aud_ll_set_flt1_coef_b0b1_flt1_b1(aud_hw_t *hw, uint32_t value)
{
    hw->flt1_coef_b0b1.flt1_b1 = value;
}

/* REG_0x25 */

static inline uint32_t aud_ll_get_flt1_coef_b2_value(aud_hw_t *hw)
{
    return hw->flt1_coef_b2.v;
}

static inline void aud_ll_set_flt1_coef_b2_value(aud_hw_t *hw, uint32_t value)
{
    hw->flt1_coef_b2.v = value;
}

/* REG_0x25:flt1_coef_b2->flt1_B2:0x25[15: 0],Filter 1 的IIR系数B2,0,RW*/
static inline uint32_t aud_ll_get_flt1_coef_b2_flt1_b2(aud_hw_t *hw)
{
    return hw->flt1_coef_b2.flt1_b2;
}

static inline void aud_ll_set_flt1_coef_b2_flt1_b2(aud_hw_t *hw, uint32_t value)
{
    hw->flt1_coef_b2.flt1_b2 = value;
}

/* REG_0x26 */

static inline uint32_t aud_ll_get_flt2_coef_a1a2_value(aud_hw_t *hw)
{
    return hw->flt2_coef_a1a2.v;
}

static inline void aud_ll_set_flt2_coef_a1a2_value(aud_hw_t *hw, uint32_t value)
{
    hw->flt2_coef_a1a2.v = value;
}

/* REG_0x26:flt2_coef_a1a2->flt2_A1:0x26[15: 0],Filter 2 的IIR系数A1,0,RW*/
static inline uint32_t aud_ll_get_flt2_coef_a1a2_flt2_a1(aud_hw_t *hw)
{
    return hw->flt2_coef_a1a2.flt2_a1;
}

static inline void aud_ll_set_flt2_coef_a1a2_flt2_a1(aud_hw_t *hw, uint32_t value)
{
    hw->flt2_coef_a1a2.flt2_a1 = value;
}

/* REG_0x26:flt2_coef_a1a2->flt2_A2:0x26[31:16],Filter 2 的IIR系数A2,0,RW*/
static inline uint32_t aud_ll_get_flt2_coef_a1a2_flt2_a2(aud_hw_t *hw)
{
    return hw->flt2_coef_a1a2.flt2_a2;
}

static inline void aud_ll_set_flt2_coef_a1a2_flt2_a2(aud_hw_t *hw, uint32_t value)
{
    hw->flt2_coef_a1a2.flt2_a2 = value;
}

/* REG_0x27 */

static inline uint32_t aud_ll_get_flt2_coef_b0b1_value(aud_hw_t *hw)
{
    return hw->flt2_coef_b0b1.v;
}

static inline void aud_ll_set_flt2_coef_b0b1_value(aud_hw_t *hw, uint32_t value)
{
    hw->flt2_coef_b0b1.v = value;
}

/* REG_0x27:flt2_coef_b0b1->flt2_B0:0x27[15: 0],Filter 2 的IIR系数B0,0,RW*/
static inline uint32_t aud_ll_get_flt2_coef_b0b1_flt2_b0(aud_hw_t *hw)
{
    return hw->flt2_coef_b0b1.flt2_b0;
}

static inline void aud_ll_set_flt2_coef_b0b1_flt2_b0(aud_hw_t *hw, uint32_t value)
{
    hw->flt2_coef_b0b1.flt2_b0 = value;
}

/* REG_0x27:flt2_coef_b0b1->flt2_B1:0x27[31:16],Filter 2 的IIR系数B1,0,RW*/
static inline uint32_t aud_ll_get_flt2_coef_b0b1_flt2_b1(aud_hw_t *hw)
{
    return hw->flt2_coef_b0b1.flt2_b1;
}

static inline void aud_ll_set_flt2_coef_b0b1_flt2_b1(aud_hw_t *hw, uint32_t value)
{
    hw->flt2_coef_b0b1.flt2_b1 = value;
}

/* REG_0x28 */

static inline uint32_t aud_ll_get_flt2_coef_b2_value(aud_hw_t *hw)
{
    return hw->flt2_coef_b2.v;
}

static inline void aud_ll_set_flt2_coef_b2_value(aud_hw_t *hw, uint32_t value)
{
    hw->flt2_coef_b2.v = value;
}

/* REG_0x28:flt2_coef_b2->flt2_B2:0x28[15: 0],Filter 2 的IIR系数B2,0,RW*/
static inline uint32_t aud_ll_get_flt2_coef_b2_flt2_b2(aud_hw_t *hw)
{
    return hw->flt2_coef_b2.flt2_b2;
}

static inline void aud_ll_set_flt2_coef_b2_flt2_b2(aud_hw_t *hw, uint32_t value)
{
    hw->flt2_coef_b2.flt2_b2 = value;
}

/* REG_0x29 */

static inline uint32_t aud_ll_get_flt3_coef_a1a2_value(aud_hw_t *hw)
{
    return hw->flt3_coef_a1a2.v;
}

static inline void aud_ll_set_flt3_coef_a1a2_value(aud_hw_t *hw, uint32_t value)
{
    hw->flt3_coef_a1a2.v = value;
}

/* REG_0x29:flt3_coef_a1a2->flt3_A1:0x29[15: 0],Filter 3 的IIR系数A1,0,RW*/
static inline uint32_t aud_ll_get_flt3_coef_a1a2_flt3_a1(aud_hw_t *hw)
{
    return hw->flt3_coef_a1a2.flt3_a1;
}

static inline void aud_ll_set_flt3_coef_a1a2_flt3_a1(aud_hw_t *hw, uint32_t value)
{
    hw->flt3_coef_a1a2.flt3_a1 = value;
}

/* REG_0x29:flt3_coef_a1a2->flt3_A2:0x29[31:16],Filter 3 的IIR系数A2,0,RW*/
static inline uint32_t aud_ll_get_flt3_coef_a1a2_flt3_a2(aud_hw_t *hw)
{
    return hw->flt3_coef_a1a2.flt3_a2;
}

static inline void aud_ll_set_flt3_coef_a1a2_flt3_a2(aud_hw_t *hw, uint32_t value)
{
    hw->flt3_coef_a1a2.flt3_a2 = value;
}

/* REG_0x2a */

static inline uint32_t aud_ll_get_flt3_coef_b0b1_value(aud_hw_t *hw)
{
    return hw->flt3_coef_b0b1.v;
}

static inline void aud_ll_set_flt3_coef_b0b1_value(aud_hw_t *hw, uint32_t value)
{
    hw->flt3_coef_b0b1.v = value;
}

/* REG_0x2a:flt3_coef_b0b1->flt3_B0:0x2a[15: 0],Filter 3 的IIR系数B0,0,RW*/
static inline uint32_t aud_ll_get_flt3_coef_b0b1_flt3_b0(aud_hw_t *hw)
{
    return hw->flt3_coef_b0b1.flt3_b0;
}

static inline void aud_ll_set_flt3_coef_b0b1_flt3_b0(aud_hw_t *hw, uint32_t value)
{
    hw->flt3_coef_b0b1.flt3_b0 = value;
}

/* REG_0x2a:flt3_coef_b0b1->flt3_B1:0x2a[31:16],Filter 3 的IIR系数B1,0,RW*/
static inline uint32_t aud_ll_get_flt3_coef_b0b1_flt3_b1(aud_hw_t *hw)
{
    return hw->flt3_coef_b0b1.flt3_b1;
}

static inline void aud_ll_set_flt3_coef_b0b1_flt3_b1(aud_hw_t *hw, uint32_t value)
{
    hw->flt3_coef_b0b1.flt3_b1 = value;
}

/* REG_0x2b */

static inline uint32_t aud_ll_get_flt3_coef_b2_value(aud_hw_t *hw)
{
    return hw->flt3_coef_b2.v;
}

static inline void aud_ll_set_flt3_coef_b2_value(aud_hw_t *hw, uint32_t value)
{
    hw->flt3_coef_b2.v = value;
}

/* REG_0x2b:flt3_coef_b2->flt3_B2:0x2b[15: 0],Filter 3 的IIR系数B2,0,RW*/
static inline uint32_t aud_ll_get_flt3_coef_b2_flt3_b2(aud_hw_t *hw)
{
    return hw->flt3_coef_b2.flt3_b2;
}

static inline void aud_ll_set_flt3_coef_b2_flt3_b2(aud_hw_t *hw, uint32_t value)
{
    hw->flt3_coef_b2.flt3_b2 = value;
}

/* REG_0x2c */

static inline uint32_t aud_ll_get_flt0_ext_coef_value(aud_hw_t *hw)
{
    return hw->flt0_ext_coef.v;
}

static inline void aud_ll_set_flt0_ext_coef_value(aud_hw_t *hw, uint32_t value)
{
    hw->flt0_ext_coef.v = value;
}

/* REG_0x2c:flt0_ext_coef->flt0_A1_L6BIT:0x2c[5:0],Filter 0 的IIR系数A1低6比特,0,RW*/
static inline uint32_t aud_ll_get_flt0_ext_coef_flt0_a1_l6bit(aud_hw_t *hw)
{
    return hw->flt0_ext_coef.flt0_a1_l6bit;
}

static inline void aud_ll_set_flt0_ext_coef_flt0_a1_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->flt0_ext_coef.flt0_a1_l6bit = value;
}

/* REG_0x2c:flt0_ext_coef->flt0_A2_L6BIT:0x2c[11:6],Filter 0 的IIR系数A2低6比特,0,RW*/
static inline uint32_t aud_ll_get_flt0_ext_coef_flt0_a2_l6bit(aud_hw_t *hw)
{
    return hw->flt0_ext_coef.flt0_a2_l6bit;
}

static inline void aud_ll_set_flt0_ext_coef_flt0_a2_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->flt0_ext_coef.flt0_a2_l6bit = value;
}

/* REG_0x2c:flt0_ext_coef->flt0_B0_L6BIT:0x2c[17:12],Filter 0 的IIR系数B0低6比特,0,RW*/
static inline uint32_t aud_ll_get_flt0_ext_coef_flt0_b0_l6bit(aud_hw_t *hw)
{
    return hw->flt0_ext_coef.flt0_b0_l6bit;
}

static inline void aud_ll_set_flt0_ext_coef_flt0_b0_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->flt0_ext_coef.flt0_b0_l6bit = value;
}

/* REG_0x2c:flt0_ext_coef->flt0_B1_L6BIT:0x2c[23:18],Filter 0 的IIR系数B1低6比特,0,RW*/
static inline uint32_t aud_ll_get_flt0_ext_coef_flt0_b1_l6bit(aud_hw_t *hw)
{
    return hw->flt0_ext_coef.flt0_b1_l6bit;
}

static inline void aud_ll_set_flt0_ext_coef_flt0_b1_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->flt0_ext_coef.flt0_b1_l6bit = value;
}

/* REG_0x2c:flt0_ext_coef->flt0_B2_L6BIT:0x2c[29:24],Filter 0 的IIR系数B2低6比特,0,RW*/
static inline uint32_t aud_ll_get_flt0_ext_coef_flt0_b2_l6bit(aud_hw_t *hw)
{
    return hw->flt0_ext_coef.flt0_b2_l6bit;
}

static inline void aud_ll_set_flt0_ext_coef_flt0_b2_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->flt0_ext_coef.flt0_b2_l6bit = value;
}

/* REG_0x2d */

static inline uint32_t aud_ll_get_flt1_ext_coef_value(aud_hw_t *hw)
{
    return hw->flt1_ext_coef.v;
}

static inline void aud_ll_set_flt1_ext_coef_value(aud_hw_t *hw, uint32_t value)
{
    hw->flt1_ext_coef.v = value;
}

/* REG_0x2d:flt1_ext_coef->flt1_A1_L6BIT:0x2d[5:0],Filter 1 的IIR系数A1低6比特,0,RW*/
static inline uint32_t aud_ll_get_flt1_ext_coef_flt1_a1_l6bit(aud_hw_t *hw)
{
    return hw->flt1_ext_coef.flt1_a1_l6bit;
}

static inline void aud_ll_set_flt1_ext_coef_flt1_a1_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->flt1_ext_coef.flt1_a1_l6bit = value;
}

/* REG_0x2d:flt1_ext_coef->flt1_A2_L6BIT:0x2d[11:6],Filter 1 的IIR系数A2低6比特,0,RW*/
static inline uint32_t aud_ll_get_flt1_ext_coef_flt1_a2_l6bit(aud_hw_t *hw)
{
    return hw->flt1_ext_coef.flt1_a2_l6bit;
}

static inline void aud_ll_set_flt1_ext_coef_flt1_a2_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->flt1_ext_coef.flt1_a2_l6bit = value;
}

/* REG_0x2d:flt1_ext_coef->flt1_B0_L6BIT:0x2d[17:12],Filter 1 的IIR系数B0低6比特,0,RW*/
static inline uint32_t aud_ll_get_flt1_ext_coef_flt1_b0_l6bit(aud_hw_t *hw)
{
    return hw->flt1_ext_coef.flt1_b0_l6bit;
}

static inline void aud_ll_set_flt1_ext_coef_flt1_b0_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->flt1_ext_coef.flt1_b0_l6bit = value;
}

/* REG_0x2d:flt1_ext_coef->flt1_B1_L6BIT:0x2d[23:18],Filter 1 的IIR系数B1低6比特,0,RW*/
static inline uint32_t aud_ll_get_flt1_ext_coef_flt1_b1_l6bit(aud_hw_t *hw)
{
    return hw->flt1_ext_coef.flt1_b1_l6bit;
}

static inline void aud_ll_set_flt1_ext_coef_flt1_b1_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->flt1_ext_coef.flt1_b1_l6bit = value;
}

/* REG_0x2d:flt1_ext_coef->flt1_B2_L6BIT:0x2d[29:24],Filter 1 的IIR系数B2低6比特,0,RW*/
static inline uint32_t aud_ll_get_flt1_ext_coef_flt1_b2_l6bit(aud_hw_t *hw)
{
    return hw->flt1_ext_coef.flt1_b2_l6bit;
}

static inline void aud_ll_set_flt1_ext_coef_flt1_b2_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->flt1_ext_coef.flt1_b2_l6bit = value;
}

/* REG_0x2e */

static inline uint32_t aud_ll_get_flt2_ext_coef_value(aud_hw_t *hw)
{
    return hw->flt2_ext_coef.v;
}

static inline void aud_ll_set_flt2_ext_coef_value(aud_hw_t *hw, uint32_t value)
{
    hw->flt2_ext_coef.v = value;
}

/* REG_0x2e:flt2_ext_coef->flt2_A1_L6BIT:0x2e[5:0],Filter 2 的IIR系数A1低6比特,0,RW*/
static inline uint32_t aud_ll_get_flt2_ext_coef_flt2_a1_l6bit(aud_hw_t *hw)
{
    return hw->flt2_ext_coef.flt2_a1_l6bit;
}

static inline void aud_ll_set_flt2_ext_coef_flt2_a1_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->flt2_ext_coef.flt2_a1_l6bit = value;
}

/* REG_0x2e:flt2_ext_coef->flt2_A2_L6BIT:0x2e[11:6],Filter 2 的IIR系数A2低6比特,0,RW*/
static inline uint32_t aud_ll_get_flt2_ext_coef_flt2_a2_l6bit(aud_hw_t *hw)
{
    return hw->flt2_ext_coef.flt2_a2_l6bit;
}

static inline void aud_ll_set_flt2_ext_coef_flt2_a2_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->flt2_ext_coef.flt2_a2_l6bit = value;
}

/* REG_0x2e:flt2_ext_coef->flt2_B0_L6BIT:0x2e[17:12],Filter 2 的IIR系数B0低6比特,0,RW*/
static inline uint32_t aud_ll_get_flt2_ext_coef_flt2_b0_l6bit(aud_hw_t *hw)
{
    return hw->flt2_ext_coef.flt2_b0_l6bit;
}

static inline void aud_ll_set_flt2_ext_coef_flt2_b0_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->flt2_ext_coef.flt2_b0_l6bit = value;
}

/* REG_0x2e:flt2_ext_coef->flt2_B1_L6BIT:0x2e[23:18],Filter 2 的IIR系数B1低6比特,0,RW*/
static inline uint32_t aud_ll_get_flt2_ext_coef_flt2_b1_l6bit(aud_hw_t *hw)
{
    return hw->flt2_ext_coef.flt2_b1_l6bit;
}

static inline void aud_ll_set_flt2_ext_coef_flt2_b1_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->flt2_ext_coef.flt2_b1_l6bit = value;
}

/* REG_0x2e:flt2_ext_coef->flt2_B2_L6BIT:0x2e[29:24],Filter 2 的IIR系数B2低6比特,0,RW*/
static inline uint32_t aud_ll_get_flt2_ext_coef_flt2_b2_l6bit(aud_hw_t *hw)
{
    return hw->flt2_ext_coef.flt2_b2_l6bit;
}

static inline void aud_ll_set_flt2_ext_coef_flt2_b2_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->flt2_ext_coef.flt2_b2_l6bit = value;
}

/* REG_0x2f */

static inline uint32_t aud_ll_get_flt3_ext_coef_value(aud_hw_t *hw)
{
    return hw->flt3_ext_coef.v;
}

static inline void aud_ll_set_flt3_ext_coef_value(aud_hw_t *hw, uint32_t value)
{
    hw->flt3_ext_coef.v = value;
}

/* REG_0x2f:flt3_ext_coef->flt3_A1_L6BIT:0x2f[5:0],Filter 3 的IIR系数A1低6比特,0,RW*/
static inline uint32_t aud_ll_get_flt3_ext_coef_flt3_a1_l6bit(aud_hw_t *hw)
{
    return hw->flt3_ext_coef.flt3_a1_l6bit;
}

static inline void aud_ll_set_flt3_ext_coef_flt3_a1_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->flt3_ext_coef.flt3_a1_l6bit = value;
}

/* REG_0x2f:flt3_ext_coef->flt3_A2_L6BIT:0x2f[11:6],Filter 3 的IIR系数A2低6比特,0,RW*/
static inline uint32_t aud_ll_get_flt3_ext_coef_flt3_a2_l6bit(aud_hw_t *hw)
{
    return hw->flt3_ext_coef.flt3_a2_l6bit;
}

static inline void aud_ll_set_flt3_ext_coef_flt3_a2_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->flt3_ext_coef.flt3_a2_l6bit = value;
}

/* REG_0x2f:flt3_ext_coef->flt3_B0_L6BIT:0x2f[17:12],Filter 3 的IIR系数B0低6比特,0,RW*/
static inline uint32_t aud_ll_get_flt3_ext_coef_flt3_b0_l6bit(aud_hw_t *hw)
{
    return hw->flt3_ext_coef.flt3_b0_l6bit;
}

static inline void aud_ll_set_flt3_ext_coef_flt3_b0_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->flt3_ext_coef.flt3_b0_l6bit = value;
}

/* REG_0x2f:flt3_ext_coef->flt3_B1_L6BIT:0x2f[23:18],Filter 3 的IIR系数B1低6比特,0,RW*/
static inline uint32_t aud_ll_get_flt3_ext_coef_flt3_b1_l6bit(aud_hw_t *hw)
{
    return hw->flt3_ext_coef.flt3_b1_l6bit;
}

static inline void aud_ll_set_flt3_ext_coef_flt3_b1_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->flt3_ext_coef.flt3_b1_l6bit = value;
}

/* REG_0x2f:flt3_ext_coef->flt3_B2_L6BIT:0x2f[29:24],Filter 3 的IIR系数B2低6比特,0,RW*/
static inline uint32_t aud_ll_get_flt3_ext_coef_flt3_b2_l6bit(aud_hw_t *hw)
{
    return hw->flt3_ext_coef.flt3_b2_l6bit;
}

static inline void aud_ll_set_flt3_ext_coef_flt3_b2_l6bit(aud_hw_t *hw, uint32_t value)
{
    hw->flt3_ext_coef.flt3_b2_l6bit = value;
}

#endif 

#ifdef __cplusplus 
}                  
#endif             
