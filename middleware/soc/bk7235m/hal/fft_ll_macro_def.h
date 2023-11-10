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
* CHIP ID:BK7256,GENARATE TIME:2022-01-20 10:15:30                                                 
************************************************************************************************************************************/

#pragma once                 
                            
#ifdef __cplusplus          
extern "C" {              
#endif                      

#include <soc/soc.h>          

#define FFT_LL_REG_BASE      (SOC_FFT_REG_BASE) //REG_BASE:0x47000000

/* REG_0x00 */
#define FFT_FFT_CONFIG_ADDR  (FFT_LL_REG_BASE  + 0x0*4) //REG ADDR :0x47000000
#define FFT_FFT_CONFIG_FFT_MODE_POS (0) 
#define FFT_FFT_CONFIG_FFT_MODE_MASK (0x1) 

#define FFT_FFT_CONFIG_IFFT_POS (1) 
#define FFT_FFT_CONFIG_IFFT_MASK (0x1) 

#define FFT_FFT_CONFIG_FFT_INT_EN_POS (2) 
#define FFT_FFT_CONFIG_FFT_INT_EN_MASK (0x1) 

#define FFT_FFT_CONFIG_FFT_ENABLE_POS (3) 
#define FFT_FFT_CONFIG_FFT_ENABLE_MASK (0x1) 

#define FFT_FFT_CONFIG_FFT_GAT_OFF_POS (4) 
#define FFT_FFT_CONFIG_FFT_GAT_OFF_MASK (0x1) 

#define FFT_FFT_CONFIG_RESERVED_POS (5) 
#define FFT_FFT_CONFIG_RESERVED_MASK (0x7FFFFFF) 

static inline uint32_t fft_ll_get_fft_config_value(void)
{
    return REG_READ(FFT_FFT_CONFIG_ADDR);
}

static inline void fft_ll_set_fft_config_value(uint32_t value)
{
    REG_WRITE(FFT_FFT_CONFIG_ADDR,value);
}

/* REG_0x00:fft_config->fft_mode:0x0[0],IFFT模式。 1：BK5130模式　0: 正常模式,0,RW*/
static inline uint32_t fft_ll_get_fft_config_fft_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FFT_CONFIG_ADDR);
    reg_value = ((reg_value >> FFT_FFT_CONFIG_FFT_MODE_POS) & FFT_FFT_CONFIG_FFT_MODE_MASK);
    return reg_value;
}

static inline void fft_ll_set_fft_config_fft_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FFT_CONFIG_ADDR);
    reg_value &= ~(FFT_FFT_CONFIG_FFT_MODE_MASK << FFT_FFT_CONFIG_FFT_MODE_POS);
    reg_value |= ((value & FFT_FFT_CONFIG_FFT_MODE_MASK) << FFT_FFT_CONFIG_FFT_MODE_POS);
    REG_WRITE(FFT_FFT_CONFIG_ADDR,reg_value);
}

/* REG_0x00:fft_config->ifft:0x0[1],IFFT使能。 1：IFFT运算。 0：FFT运算 ,0,RW*/
static inline uint32_t fft_ll_get_fft_config_ifft(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FFT_CONFIG_ADDR);
    reg_value = ((reg_value >> FFT_FFT_CONFIG_IFFT_POS) & FFT_FFT_CONFIG_IFFT_MASK);
    return reg_value;
}

static inline void fft_ll_set_fft_config_ifft(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FFT_CONFIG_ADDR);
    reg_value &= ~(FFT_FFT_CONFIG_IFFT_MASK << FFT_FFT_CONFIG_IFFT_POS);
    reg_value |= ((value & FFT_FFT_CONFIG_IFFT_MASK) << FFT_FFT_CONFIG_IFFT_POS);
    REG_WRITE(FFT_FFT_CONFIG_ADDR,reg_value);
}

/* REG_0x00:fft_config->fft_int_en:0x0[2],FFT中断使能信号,0,RW*/
static inline uint32_t fft_ll_get_fft_config_fft_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FFT_CONFIG_ADDR);
    reg_value = ((reg_value >> FFT_FFT_CONFIG_FFT_INT_EN_POS) & FFT_FFT_CONFIG_FFT_INT_EN_MASK);
    return reg_value;
}

static inline void fft_ll_set_fft_config_fft_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FFT_CONFIG_ADDR);
    reg_value &= ~(FFT_FFT_CONFIG_FFT_INT_EN_MASK << FFT_FFT_CONFIG_FFT_INT_EN_POS);
    reg_value |= ((value & FFT_FFT_CONFIG_FFT_INT_EN_MASK) << FFT_FFT_CONFIG_FFT_INT_EN_POS);
    REG_WRITE(FFT_FFT_CONFIG_ADDR,reg_value);
}

/* REG_0x00:fft_config->fft_enable:0x0[3],FFT使能信号，每次FFT或者IFFT结束后，此信号拉低,0,RW*/
static inline uint32_t fft_ll_get_fft_config_fft_enable(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FFT_CONFIG_ADDR);
    reg_value = ((reg_value >> FFT_FFT_CONFIG_FFT_ENABLE_POS) & FFT_FFT_CONFIG_FFT_ENABLE_MASK);
    return reg_value;
}

static inline void fft_ll_set_fft_config_fft_enable(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FFT_CONFIG_ADDR);
    reg_value &= ~(FFT_FFT_CONFIG_FFT_ENABLE_MASK << FFT_FFT_CONFIG_FFT_ENABLE_POS);
    reg_value |= ((value & FFT_FFT_CONFIG_FFT_ENABLE_MASK) << FFT_FFT_CONFIG_FFT_ENABLE_POS);
    REG_WRITE(FFT_FFT_CONFIG_ADDR,reg_value);
}

/* REG_0x00:fft_config->fft_gat_off:0x0[4],FFT模块clock gating信号，1 ：gating off 0:gating on ,0,RW*/
static inline uint32_t fft_ll_get_fft_config_fft_gat_off(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FFT_CONFIG_ADDR);
    reg_value = ((reg_value >> FFT_FFT_CONFIG_FFT_GAT_OFF_POS) & FFT_FFT_CONFIG_FFT_GAT_OFF_MASK);
    return reg_value;
}

static inline void fft_ll_set_fft_config_fft_gat_off(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FFT_CONFIG_ADDR);
    reg_value &= ~(FFT_FFT_CONFIG_FFT_GAT_OFF_MASK << FFT_FFT_CONFIG_FFT_GAT_OFF_POS);
    reg_value |= ((value & FFT_FFT_CONFIG_FFT_GAT_OFF_MASK) << FFT_FFT_CONFIG_FFT_GAT_OFF_POS);
    REG_WRITE(FFT_FFT_CONFIG_ADDR,reg_value);
}

/* REG_0x01 */
#define FFT_FIR_CONFIG_ADDR  (FFT_LL_REG_BASE  + 0x1*4) //REG ADDR :0x47000004
#define FFT_FIR_CONFIG_FIR_LENGTH_POS (0) 
#define FFT_FIR_CONFIG_FIR_LENGTH_MASK (0xFF) 

#define FFT_FIR_CONFIG_FIR_CWIDTH_POS (8) 
#define FFT_FIR_CONFIG_FIR_CWIDTH_MASK (0x7) 

#define FFT_FIR_CONFIG_FIR_DWIDTH_POS (11) 
#define FFT_FIR_CONFIG_FIR_DWIDTH_MASK (0x7) 

#define FFT_FIR_CONFIG_FIR_MODE_POS (14) 
#define FFT_FIR_CONFIG_FIR_MODE_MASK (0x1) 

#define FFT_FIR_CONFIG_FIR_INT_EN_POS (15) 
#define FFT_FIR_CONFIG_FIR_INT_EN_MASK (0x1) 

#define FFT_FIR_CONFIG_FIR_ENABLE_POS (16) 
#define FFT_FIR_CONFIG_FIR_ENABLE_MASK (0x1) 

#define FFT_FIR_CONFIG_RESERVED_POS (17) 
#define FFT_FIR_CONFIG_RESERVED_MASK (0x7FFF) 

static inline uint32_t fft_ll_get_fir_config_value(void)
{
    return REG_READ(FFT_FIR_CONFIG_ADDR);
}

static inline void fft_ll_set_fir_config_value(uint32_t value)
{
    REG_WRITE(FFT_FIR_CONFIG_ADDR,value);
}

/* REG_0x01:fir_config->fir_length:0x1[7:0],FIR的计算点数 fir_length + 1 ,0,RW*/
static inline uint32_t fft_ll_get_fir_config_fir_length(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FIR_CONFIG_ADDR);
    reg_value = ((reg_value >> FFT_FIR_CONFIG_FIR_LENGTH_POS) & FFT_FIR_CONFIG_FIR_LENGTH_MASK);
    return reg_value;
}

static inline void fft_ll_set_fir_config_fir_length(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FIR_CONFIG_ADDR);
    reg_value &= ~(FFT_FIR_CONFIG_FIR_LENGTH_MASK << FFT_FIR_CONFIG_FIR_LENGTH_POS);
    reg_value |= ((value & FFT_FIR_CONFIG_FIR_LENGTH_MASK) << FFT_FIR_CONFIG_FIR_LENGTH_POS);
    REG_WRITE(FFT_FIR_CONFIG_ADDR,reg_value);
}

/* REG_0x01:fir_config->fir_cwidth:0x1[10:8],FIR的系数位宽 16 - fir_cwidth,0,RW*/
static inline uint32_t fft_ll_get_fir_config_fir_cwidth(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FIR_CONFIG_ADDR);
    reg_value = ((reg_value >> FFT_FIR_CONFIG_FIR_CWIDTH_POS) & FFT_FIR_CONFIG_FIR_CWIDTH_MASK);
    return reg_value;
}

static inline void fft_ll_set_fir_config_fir_cwidth(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FIR_CONFIG_ADDR);
    reg_value &= ~(FFT_FIR_CONFIG_FIR_CWIDTH_MASK << FFT_FIR_CONFIG_FIR_CWIDTH_POS);
    reg_value |= ((value & FFT_FIR_CONFIG_FIR_CWIDTH_MASK) << FFT_FIR_CONFIG_FIR_CWIDTH_POS);
    REG_WRITE(FFT_FIR_CONFIG_ADDR,reg_value);
}

/* REG_0x01:fir_config->fir_dwidth:0x1[13:11],FIR的数据位宽 21 - fir_dwidth,0,RW*/
static inline uint32_t fft_ll_get_fir_config_fir_dwidth(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FIR_CONFIG_ADDR);
    reg_value = ((reg_value >> FFT_FIR_CONFIG_FIR_DWIDTH_POS) & FFT_FIR_CONFIG_FIR_DWIDTH_MASK);
    return reg_value;
}

static inline void fft_ll_set_fir_config_fir_dwidth(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FIR_CONFIG_ADDR);
    reg_value &= ~(FFT_FIR_CONFIG_FIR_DWIDTH_MASK << FFT_FIR_CONFIG_FIR_DWIDTH_POS);
    reg_value |= ((value & FFT_FIR_CONFIG_FIR_DWIDTH_MASK) << FFT_FIR_CONFIG_FIR_DWIDTH_POS);
    REG_WRITE(FFT_FIR_CONFIG_ADDR,reg_value);
}

/* REG_0x01:fir_config->fir_mode:0x1[14],FIR的工作模式，1 ：双FIR模式，0：单FIR模式,0,RW*/
static inline uint32_t fft_ll_get_fir_config_fir_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FIR_CONFIG_ADDR);
    reg_value = ((reg_value >> FFT_FIR_CONFIG_FIR_MODE_POS) & FFT_FIR_CONFIG_FIR_MODE_MASK);
    return reg_value;
}

static inline void fft_ll_set_fir_config_fir_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FIR_CONFIG_ADDR);
    reg_value &= ~(FFT_FIR_CONFIG_FIR_MODE_MASK << FFT_FIR_CONFIG_FIR_MODE_POS);
    reg_value |= ((value & FFT_FIR_CONFIG_FIR_MODE_MASK) << FFT_FIR_CONFIG_FIR_MODE_POS);
    REG_WRITE(FFT_FIR_CONFIG_ADDR,reg_value);
}

/* REG_0x01:fir_config->fir_int_en:0x1[15],FIR的中断使能,0,RW*/
static inline uint32_t fft_ll_get_fir_config_fir_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FIR_CONFIG_ADDR);
    reg_value = ((reg_value >> FFT_FIR_CONFIG_FIR_INT_EN_POS) & FFT_FIR_CONFIG_FIR_INT_EN_MASK);
    return reg_value;
}

static inline void fft_ll_set_fir_config_fir_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FIR_CONFIG_ADDR);
    reg_value &= ~(FFT_FIR_CONFIG_FIR_INT_EN_MASK << FFT_FIR_CONFIG_FIR_INT_EN_POS);
    reg_value |= ((value & FFT_FIR_CONFIG_FIR_INT_EN_MASK) << FFT_FIR_CONFIG_FIR_INT_EN_POS);
    REG_WRITE(FFT_FIR_CONFIG_ADDR,reg_value);
}

/* REG_0x01:fir_config->fir_enable:0x1[16],FIR的工作使能  1 ：使能,0,RW*/
static inline uint32_t fft_ll_get_fir_config_fir_enable(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FIR_CONFIG_ADDR);
    reg_value = ((reg_value >> FFT_FIR_CONFIG_FIR_ENABLE_POS) & FFT_FIR_CONFIG_FIR_ENABLE_MASK);
    return reg_value;
}

static inline void fft_ll_set_fir_config_fir_enable(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_FIR_CONFIG_ADDR);
    reg_value &= ~(FFT_FIR_CONFIG_FIR_ENABLE_MASK << FFT_FIR_CONFIG_FIR_ENABLE_POS);
    reg_value |= ((value & FFT_FIR_CONFIG_FIR_ENABLE_MASK) << FFT_FIR_CONFIG_FIR_ENABLE_POS);
    REG_WRITE(FFT_FIR_CONFIG_ADDR,reg_value);
}

/* REG_0x02 */
#define FFT_DATA_PORTS_ADDR  (FFT_LL_REG_BASE  + 0x2*4) //REG ADDR :0x47000008
#define FFT_DATA_PORTS_DATA_PORT_POS (0) 
#define FFT_DATA_PORTS_DATA_PORT_MASK (0xFFFFFFFF) 

static inline int32 fft_ll_get_data_ports_value(void)
{
    return REG_READ(FFT_DATA_PORTS_ADDR);
}

static inline void fft_ll_set_data_ports_value(int32 value)
{
    REG_WRITE(FFT_DATA_PORTS_ADDR,value);
}

/* REG_0x02:data_ports->data_port:0x2[31:0],FIR和FFT的入口地址，写是输入口，读是输出口     {FFT_I,FFT_Q} 31:0  ;  {FIR_D1,FIR_D0} 31:0,0,R/W*/
static inline int32 fft_ll_get_data_ports_data_port(void)
{
    return REG_READ(FFT_DATA_PORTS_ADDR);
}

static inline void fft_ll_set_data_ports_data_port(int32 value)
{
    REG_WRITE(FFT_DATA_PORTS_ADDR,value);
}

/* REG_0x03 */
#define FFT_COEF_PORTS_ADDR  (FFT_LL_REG_BASE  + 0x3*4) //REG ADDR :0x4700000c
#define FFT_COEF_PORTS_COEF_PORT_POS (0) 
#define FFT_COEF_PORTS_COEF_PORT_MASK (0xFFFFFFFF) 

static inline void fft_ll_set_coef_ports_value(int32 value)
{
    REG_WRITE(FFT_COEF_PORTS_ADDR,value);
}

/* REG_0x03:coef_ports->coef_port:0x3[31:0],FIR 系数的入口地址 {FIR_C1, FIR_C0} 31:0,0,WO*/
static inline void fft_ll_set_coef_ports_coef_port(int32 value)
{
    REG_WRITE(FFT_COEF_PORTS_ADDR,value);
}

/* REG_0x04 */
#define FFT_MAC_REULT1_ADDR  (FFT_LL_REG_BASE  + 0x4*4) //REG ADDR :0x47000010
#define FFT_MAC_REULT1_MAC_RESULT_POS (0) 
#define FFT_MAC_REULT1_MAC_RESULT_MASK (0xFFFFFFFF) 

static inline uint32_t fft_ll_get_mac_reult1_value(void)
{
    return REG_READ(FFT_MAC_REULT1_ADDR);
}

/* REG_0x04:mac_reult1->mac_result:0x4[31:0],MAC 结果输出 ,0,RO*/
static inline uint32_t fft_ll_get_mac_reult1_mac_result(void)
{
    return REG_READ(FFT_MAC_REULT1_ADDR);
}

/* REG_0x05 */
#define FFT_MAC_REULT2_ADDR  (FFT_LL_REG_BASE  + 0x5*4) //REG ADDR :0x47000014
#define FFT_MAC_REULT2_MAC_RESULT_POS (0) 
#define FFT_MAC_REULT2_MAC_RESULT_MASK (0xFFF) 

#define FFT_MAC_REULT2_RESERVED_POS (12) 
#define FFT_MAC_REULT2_RESERVED_MASK (0xFFFFF) 

static inline uint32_t fft_ll_get_mac_reult2_value(void)
{
    return REG_READ(FFT_MAC_REULT2_ADDR);
}

/* REG_0x05:mac_reult2->mac_result:0x5[11:0],,MAC 结果输出 ,0,RO*/
static inline uint32_t fft_ll_get_mac_reult2_mac_result(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_MAC_REULT2_ADDR);
    reg_value = ((reg_value >> FFT_MAC_REULT2_MAC_RESULT_POS)&FFT_MAC_REULT2_MAC_RESULT_MASK);
    return reg_value;
}

/* REG_0x06 */
#define FFT_STATUS_ADDR  (FFT_LL_REG_BASE  + 0x6*4) //REG ADDR :0x47000018
#define FFT_STATUS_FFT_DONE_POS (0) 
#define FFT_STATUS_FFT_DONE_MASK (0x1) 

#define FFT_STATUS_FIR_DONE_POS (1) 
#define FFT_STATUS_FIR_DONE_MASK (0x1) 

#define FFT_STATUS_SELF_PROC_DONE_POS (2) 
#define FFT_STATUS_SELF_PROC_DONE_MASK (0x1) 

#define FFT_STATUS_RESERVED_POS (3) 
#define FFT_STATUS_RESERVED_MASK (0xF) 

#define FFT_STATUS_BIT_EXT_POS (7) 
#define FFT_STATUS_BIT_EXT_MASK (0x3F) 

#define FFT_STATUS_RESERVED1_POS (13) 
#define FFT_STATUS_RESERVED1_MASK (0x7FFFF) 

static inline uint32_t fft_ll_get_status_value(void)
{
    return REG_READ(FFT_STATUS_ADDR);
}

static inline void fft_ll_set_status_value(uint32_t value)
{
    REG_WRITE(FFT_STATUS_ADDR,value);
}

/* REG_0x06:status->fft_done:0x6[0],FFT 结束指示标志, 一旦启动读结果，自动清零,0,RW*/
static inline uint32_t fft_ll_get_status_fft_done(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_STATUS_ADDR);
    reg_value = ((reg_value >> FFT_STATUS_FFT_DONE_POS) & FFT_STATUS_FFT_DONE_MASK);
    return reg_value;
}

static inline void fft_ll_set_status_fft_done(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_STATUS_ADDR);
    reg_value &= ~(FFT_STATUS_FFT_DONE_MASK << FFT_STATUS_FFT_DONE_POS);
    reg_value |= ((value & FFT_STATUS_FFT_DONE_MASK) << FFT_STATUS_FFT_DONE_POS);
    REG_WRITE(FFT_STATUS_ADDR,reg_value);
}

/* REG_0x06:status->fir_done:0x6[1],FIR 结束指示标志, 一旦启动读结果，自动清零,0,RW*/
static inline uint32_t fft_ll_get_status_fir_done(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_STATUS_ADDR);
    reg_value = ((reg_value >> FFT_STATUS_FIR_DONE_POS) & FFT_STATUS_FIR_DONE_MASK);
    return reg_value;
}

static inline void fft_ll_set_status_fir_done(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_STATUS_ADDR);
    reg_value &= ~(FFT_STATUS_FIR_DONE_MASK << FFT_STATUS_FIR_DONE_POS);
    reg_value |= ((value & FFT_STATUS_FIR_DONE_MASK) << FFT_STATUS_FIR_DONE_POS);
    REG_WRITE(FFT_STATUS_ADDR,reg_value);
}

/* REG_0x06:status->self_proc_done:0x6[2],自相关处理结束指示标志, 一旦启动读结果，自动清零,0,RW*/
static inline uint32_t fft_ll_get_status_self_proc_done(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_STATUS_ADDR);
    reg_value = ((reg_value >> FFT_STATUS_SELF_PROC_DONE_POS) & FFT_STATUS_SELF_PROC_DONE_MASK);
    return reg_value;
}

static inline void fft_ll_set_status_self_proc_done(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_STATUS_ADDR);
    reg_value &= ~(FFT_STATUS_SELF_PROC_DONE_MASK << FFT_STATUS_SELF_PROC_DONE_POS);
    reg_value |= ((value & FFT_STATUS_SELF_PROC_DONE_MASK) << FFT_STATUS_SELF_PROC_DONE_POS);
    REG_WRITE(FFT_STATUS_ADDR,reg_value);
}

/* REG_0x06:status->bit_ext:0x6[12:7],FFT输出扩展标志，2的补码格式，如果为正数。需要在输出缩小2^bit_ext倍数。,0,RW*/
static inline uint32_t fft_ll_get_status_bit_ext(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_STATUS_ADDR);
    reg_value = ((reg_value >> FFT_STATUS_BIT_EXT_POS) & FFT_STATUS_BIT_EXT_MASK);
    return reg_value;
}

static inline void fft_ll_set_status_bit_ext(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_STATUS_ADDR);
    reg_value &= ~(FFT_STATUS_BIT_EXT_MASK << FFT_STATUS_BIT_EXT_POS);
    reg_value |= ((value & FFT_STATUS_BIT_EXT_MASK) << FFT_STATUS_BIT_EXT_POS);
    REG_WRITE(FFT_STATUS_ADDR,reg_value);
}

/* REG_0x07 */
#define FFT_START_TRIGGER_ADDR  (FFT_LL_REG_BASE  + 0x7*4) //REG ADDR :0x4700001c
#define FFT_START_TRIGGER_START_TRIGGER_POS (0) 
#define FFT_START_TRIGGER_START_TRIGGER_MASK (0x1) 

#define FFT_START_TRIGGER_RESERVED_POS (1) 
#define FFT_START_TRIGGER_RESERVED_MASK (0x7FFFFFFF) 

static inline uint32_t fft_ll_get_start_trigger_value(void)
{
    return REG_READ(FFT_START_TRIGGER_ADDR);
}

static inline void fft_ll_set_start_trigger_value(uint32_t value)
{
    REG_WRITE(FFT_START_TRIGGER_ADDR,value);
}

/* REG_0x07:start_trigger->start_trigger:0x7[0],写1有效，触发内核工作，并自动清零。,0,RW*/
static inline uint32_t fft_ll_get_start_trigger_start_trigger(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_START_TRIGGER_ADDR);
    reg_value = ((reg_value >> FFT_START_TRIGGER_START_TRIGGER_POS) & FFT_START_TRIGGER_START_TRIGGER_MASK);
    return reg_value;
}

static inline void fft_ll_set_start_trigger_start_trigger(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_START_TRIGGER_ADDR);
    reg_value &= ~(FFT_START_TRIGGER_START_TRIGGER_MASK << FFT_START_TRIGGER_START_TRIGGER_POS);
    reg_value |= ((value & FFT_START_TRIGGER_START_TRIGGER_MASK) << FFT_START_TRIGGER_START_TRIGGER_POS);
    REG_WRITE(FFT_START_TRIGGER_ADDR,reg_value);
}

/* REG_0x08 */
#define FFT_SELF_CONFIG_ADDR  (FFT_LL_REG_BASE  + 0x8*4) //REG ADDR :0x47000020
#define FFT_SELF_CONFIG_ANA_LEN_POS (0) 
#define FFT_SELF_CONFIG_ANA_LEN_MASK (0xFF) 

#define FFT_SELF_CONFIG_LPC_LEN_POS (8) 
#define FFT_SELF_CONFIG_LPC_LEN_MASK (0xFF) 

#define FFT_SELF_CONFIG_ROUND_BIT_IDX_POS (16) 
#define FFT_SELF_CONFIG_ROUND_BIT_IDX_MASK (0x1F) 

#define FFT_SELF_CONFIG_RESERVED_POS (21) 
#define FFT_SELF_CONFIG_RESERVED_MASK (0x7) 

#define FFT_SELF_CONFIG_SELF_PROC_MODE_POS (24) 
#define FFT_SELF_CONFIG_SELF_PROC_MODE_MASK (0x1) 

#define FFT_SELF_CONFIG_RESERVED1_POS (25) 
#define FFT_SELF_CONFIG_RESERVED1_MASK (0x7) 

#define FFT_SELF_CONFIG_SELF_ENABLE_POS (28) 
#define FFT_SELF_CONFIG_SELF_ENABLE_MASK (0x1) 

#define FFT_SELF_CONFIG_RESERVED2_POS (29) 
#define FFT_SELF_CONFIG_RESERVED2_MASK (0x7) 

static inline uint32_t fft_ll_get_self_config_value(void)
{
    return REG_READ(FFT_SELF_CONFIG_ADDR);
}

static inline void fft_ll_set_self_config_value(uint32_t value)
{
    REG_WRITE(FFT_SELF_CONFIG_ADDR,value);
}

/* REG_0x08:self_config->ana_len:0x8[7:0],输入数据长度,0,RW*/
static inline uint32_t fft_ll_get_self_config_ana_len(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_SELF_CONFIG_ADDR);
    reg_value = ((reg_value >> FFT_SELF_CONFIG_ANA_LEN_POS) & FFT_SELF_CONFIG_ANA_LEN_MASK);
    return reg_value;
}

static inline void fft_ll_set_self_config_ana_len(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_SELF_CONFIG_ADDR);
    reg_value &= ~(FFT_SELF_CONFIG_ANA_LEN_MASK << FFT_SELF_CONFIG_ANA_LEN_POS);
    reg_value |= ((value & FFT_SELF_CONFIG_ANA_LEN_MASK) << FFT_SELF_CONFIG_ANA_LEN_POS);
    REG_WRITE(FFT_SELF_CONFIG_ADDR,reg_value);
}

/* REG_0x08:self_config->lpc_len:0x8[15:8],LPC_LEN长度,0,RW*/
static inline uint32_t fft_ll_get_self_config_lpc_len(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_SELF_CONFIG_ADDR);
    reg_value = ((reg_value >> FFT_SELF_CONFIG_LPC_LEN_POS) & FFT_SELF_CONFIG_LPC_LEN_MASK);
    return reg_value;
}

static inline void fft_ll_set_self_config_lpc_len(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_SELF_CONFIG_ADDR);
    reg_value &= ~(FFT_SELF_CONFIG_LPC_LEN_MASK << FFT_SELF_CONFIG_LPC_LEN_POS);
    reg_value |= ((value & FFT_SELF_CONFIG_LPC_LEN_MASK) << FFT_SELF_CONFIG_LPC_LEN_POS);
    REG_WRITE(FFT_SELF_CONFIG_ADDR,reg_value);
}

/* REG_0x08:self_config->round_bit_idx:0x8[20:16],自相关累加结果需要round的比特数,0,RW*/
static inline uint32_t fft_ll_get_self_config_round_bit_idx(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_SELF_CONFIG_ADDR);
    reg_value = ((reg_value >> FFT_SELF_CONFIG_ROUND_BIT_IDX_POS) & FFT_SELF_CONFIG_ROUND_BIT_IDX_MASK);
    return reg_value;
}

static inline void fft_ll_set_self_config_round_bit_idx(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_SELF_CONFIG_ADDR);
    reg_value &= ~(FFT_SELF_CONFIG_ROUND_BIT_IDX_MASK << FFT_SELF_CONFIG_ROUND_BIT_IDX_POS);
    reg_value |= ((value & FFT_SELF_CONFIG_ROUND_BIT_IDX_MASK) << FFT_SELF_CONFIG_ROUND_BIT_IDX_POS);
    REG_WRITE(FFT_SELF_CONFIG_ADDR,reg_value);
}

/* REG_0x08:self_config->self_proc_mode:0x8[24],自相关处理模式选择,0,RW*/
static inline uint32_t fft_ll_get_self_config_self_proc_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_SELF_CONFIG_ADDR);
    reg_value = ((reg_value >> FFT_SELF_CONFIG_SELF_PROC_MODE_POS) & FFT_SELF_CONFIG_SELF_PROC_MODE_MASK);
    return reg_value;
}

static inline void fft_ll_set_self_config_self_proc_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_SELF_CONFIG_ADDR);
    reg_value &= ~(FFT_SELF_CONFIG_SELF_PROC_MODE_MASK << FFT_SELF_CONFIG_SELF_PROC_MODE_POS);
    reg_value |= ((value & FFT_SELF_CONFIG_SELF_PROC_MODE_MASK) << FFT_SELF_CONFIG_SELF_PROC_MODE_POS);
    REG_WRITE(FFT_SELF_CONFIG_ADDR,reg_value);
}

/* REG_0x08:self_config->self_enable:0x8[28],自相关工作使能. 0:不使能;1:使能,0,RW*/
static inline uint32_t fft_ll_get_self_config_self_enable(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_SELF_CONFIG_ADDR);
    reg_value = ((reg_value >> FFT_SELF_CONFIG_SELF_ENABLE_POS) & FFT_SELF_CONFIG_SELF_ENABLE_MASK);
    return reg_value;
}

static inline void fft_ll_set_self_config_self_enable(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_SELF_CONFIG_ADDR);
    reg_value &= ~(FFT_SELF_CONFIG_SELF_ENABLE_MASK << FFT_SELF_CONFIG_SELF_ENABLE_POS);
    reg_value |= ((value & FFT_SELF_CONFIG_SELF_ENABLE_MASK) << FFT_SELF_CONFIG_SELF_ENABLE_POS);
    REG_WRITE(FFT_SELF_CONFIG_ADDR,reg_value);
}

/* REG_0x09 */
#define FFT_MEMORY_CONFIG_ADDR  (FFT_LL_REG_BASE  + 0x9*4) //REG ADDR :0x47000024
#define FFT_MEMORY_CONFIG_MEMORY_CONFIG_MODE_POS (0) 
#define FFT_MEMORY_CONFIG_MEMORY_CONFIG_MODE_MASK (0x1) 

#define FFT_MEMORY_CONFIG_RESERVED_POS (1) 
#define FFT_MEMORY_CONFIG_RESERVED_MASK (0x7FFFFFFF) 

static inline uint32_t fft_ll_get_memory_config_value(void)
{
    return REG_READ(FFT_MEMORY_CONFIG_ADDR);
}

static inline void fft_ll_set_memory_config_value(uint32_t value)
{
    REG_WRITE(FFT_MEMORY_CONFIG_ADDR,value);
}

/* REG_0x09:memory_config->memory_config_mode:0x9[0],Memory0和Memory1软件配置方式.; 0:Memory0和Memory1软件分别配置;; 1:在fft_enable/fir_enable/self_enable都不使能的情况下,Memory0和Memory1软件同时配置.,0,RW*/
static inline uint32_t fft_ll_get_memory_config_memory_config_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_MEMORY_CONFIG_ADDR);
    reg_value = ((reg_value >> FFT_MEMORY_CONFIG_MEMORY_CONFIG_MODE_POS) & FFT_MEMORY_CONFIG_MEMORY_CONFIG_MODE_MASK);
    return reg_value;
}

static inline void fft_ll_set_memory_config_memory_config_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(FFT_MEMORY_CONFIG_ADDR);
    reg_value &= ~(FFT_MEMORY_CONFIG_MEMORY_CONFIG_MODE_MASK << FFT_MEMORY_CONFIG_MEMORY_CONFIG_MODE_POS);
    reg_value |= ((value & FFT_MEMORY_CONFIG_MEMORY_CONFIG_MODE_MASK) << FFT_MEMORY_CONFIG_MEMORY_CONFIG_MODE_POS);
    REG_WRITE(FFT_MEMORY_CONFIG_ADDR,reg_value);
}

#ifdef __cplusplus 
}                  
#endif             
