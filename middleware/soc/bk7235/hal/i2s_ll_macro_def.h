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
* CHIP ID:BK7256,GENARATE TIME:2022-02-09 17:28:47                                                 
************************************************************************************************************************************/

#pragma once                 
                            
#ifdef __cplusplus          
extern "C" {              
#endif                      

#include <soc/soc.h>          

#define I2S_LL_REG_BASE      (SOC_I2S_REG_BASE) //REG_BASE:0x47810000

/* REG_0x00 */
#define I2S_REG0X0_ADDR  (I2S_LL_REG_BASE  + 0x0*4) //REG ADDR :0x47810000
#define I2S_REG0X0_BITRATIO_POS (0) 
#define I2S_REG0X0_BITRATIO_MASK (0xFF) 

#define I2S_REG0X0_SMPRATIO_POS (8) 
#define I2S_REG0X0_SMPRATIO_MASK (0x1F) 

#define I2S_REG0X0_PCM_DLEN_POS (13) 
#define I2S_REG0X0_PCM_DLEN_MASK (0x7) 

#define I2S_REG0X0_DATALEN_POS (16) 
#define I2S_REG0X0_DATALEN_MASK (0x1F) 

#define I2S_REG0X0_SYNCLEN_POS (21) 
#define I2S_REG0X0_SYNCLEN_MASK (0x7) 

#define I2S_REG0X0_LSBFIRST_POS (24) 
#define I2S_REG0X0_LSBFIRST_MASK (0x1) 

#define I2S_REG0X0_SCLKINV_POS (25) 
#define I2S_REG0X0_SCLKINV_MASK (0x1) 

#define I2S_REG0X0_LRCKRP_POS (26) 
#define I2S_REG0X0_LRCKRP_MASK (0x1) 

#define I2S_REG0X0_MODESEL_POS (27) 
#define I2S_REG0X0_MODESEL_MASK (0x7) 

#define I2S_REG0X0_MSTEN_POS (30) 
#define I2S_REG0X0_MSTEN_MASK (0x1) 

#define I2S_REG0X0_I2SPCMEN_POS (31) 
#define I2S_REG0X0_I2SPCMEN_MASK (0x1) 

static inline uint32_t i2s_ll_get_reg0x0_value(void)
{
    return REG_READ(I2S_REG0X0_ADDR);
}

static inline void i2s_ll_set_reg0x0_value(uint32_t value)
{
    REG_WRITE(I2S_REG0X0_ADDR,value);
}

/* REG_0x00:reg0x0->BITRATIO:0x0[7:0],串行时钟SCK的比特率，即相对于系统时钟SYS_CLK的分频比（仅Maseter有效）; Fsck = Fsys_clk / 2*(BITRATIO)。; 【注意】配置0对应不分频,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x0_bitratio(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value = ((reg_value >> I2S_REG0X0_BITRATIO_POS) & I2S_REG0X0_BITRATIO_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x0_bitratio(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value &= ~(I2S_REG0X0_BITRATIO_MASK << I2S_REG0X0_BITRATIO_POS);
    reg_value |= ((value & I2S_REG0X0_BITRATIO_MASK) << I2S_REG0X0_BITRATIO_POS);
    REG_WRITE(I2S_REG0X0_ADDR,reg_value);
}

/* REG_0x00:reg0x0->SMPRATIO:0x0[12:8],采样速率指示。（单位是SCK）; Fs = Fsck / 2*(SMPRATIO+1); 【注意】该配置值表示单个声道的SCK个数。若2B+D模式下，同步信号之间的间隔等于2*(SMPRATIO+1)，其它模式下同步(左右声道指示)信号的变化间隔等于(SMPRATIO+1)。,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x0_smpratio(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value = ((reg_value >> I2S_REG0X0_SMPRATIO_POS) & I2S_REG0X0_SMPRATIO_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x0_smpratio(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value &= ~(I2S_REG0X0_SMPRATIO_MASK << I2S_REG0X0_SMPRATIO_POS);
    reg_value |= ((value & I2S_REG0X0_SMPRATIO_MASK) << I2S_REG0X0_SMPRATIO_POS);
    REG_WRITE(I2S_REG0X0_ADDR,reg_value);
}

/* REG_0x00:reg0x0->PCM_DLEN:0x0[15:13],2B+D的PCM模式中D的长度（单位是SCK）。,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x0_pcm_dlen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value = ((reg_value >> I2S_REG0X0_PCM_DLEN_POS) & I2S_REG0X0_PCM_DLEN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x0_pcm_dlen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value &= ~(I2S_REG0X0_PCM_DLEN_MASK << I2S_REG0X0_PCM_DLEN_POS);
    reg_value |= ((value & I2S_REG0X0_PCM_DLEN_MASK) << I2S_REG0X0_PCM_DLEN_POS);
    REG_WRITE(I2S_REG0X0_ADDR,reg_value);
}

/* REG_0x00:reg0x0->DATALEN:0x0[20:16],传输数据长度; 0~31分别表示1~32bit。,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x0_datalen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value = ((reg_value >> I2S_REG0X0_DATALEN_POS) & I2S_REG0X0_DATALEN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x0_datalen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value &= ~(I2S_REG0X0_DATALEN_MASK << I2S_REG0X0_DATALEN_POS);
    reg_value |= ((value & I2S_REG0X0_DATALEN_MASK) << I2S_REG0X0_DATALEN_POS);
    REG_WRITE(I2S_REG0X0_ADDR,reg_value);
}

/* REG_0x00:reg0x0->SYNCLEN:0x0[23:21],Sync长度（仅Long Frame Sync模式下有效）。; 1~7分别对应2~8个Bit Clock Cycle,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x0_synclen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value = ((reg_value >> I2S_REG0X0_SYNCLEN_POS) & I2S_REG0X0_SYNCLEN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x0_synclen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value &= ~(I2S_REG0X0_SYNCLEN_MASK << I2S_REG0X0_SYNCLEN_POS);
    reg_value |= ((value & I2S_REG0X0_SYNCLEN_MASK) << I2S_REG0X0_SYNCLEN_POS);
    REG_WRITE(I2S_REG0X0_ADDR,reg_value);
}

/* REG_0x00:reg0x0->LSBFIRST:0x0[24],LSB先发送/接收使能信号; 0：MSB先发送/接收; 1：LSB先发送/接收,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x0_lsbfirst(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value = ((reg_value >> I2S_REG0X0_LSBFIRST_POS) & I2S_REG0X0_LSBFIRST_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x0_lsbfirst(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value &= ~(I2S_REG0X0_LSBFIRST_MASK << I2S_REG0X0_LSBFIRST_POS);
    reg_value |= ((value & I2S_REG0X0_LSBFIRST_MASK) << I2S_REG0X0_LSBFIRST_POS);
    REG_WRITE(I2S_REG0X0_ADDR,reg_value);
}

/* REG_0x00:reg0x0->SCLKINV:0x0[25],SCK信号反转指示; 0：不反转; 1：反转; 【注意】本模块的输入数据在SCLK的上升沿采样，输出数据在SCLK的下降沿变化。若对端通信的器件的时序要求与本模块不同，则需将该比特配置为’1’。,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x0_sclkinv(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value = ((reg_value >> I2S_REG0X0_SCLKINV_POS) & I2S_REG0X0_SCLKINV_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x0_sclkinv(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value &= ~(I2S_REG0X0_SCLKINV_MASK << I2S_REG0X0_SCLKINV_POS);
    reg_value |= ((value & I2S_REG0X0_SCLKINV_MASK) << I2S_REG0X0_SCLKINV_POS);
    REG_WRITE(I2S_REG0X0_ADDR,reg_value);
}

/* REG_0x00:reg0x0->LRCKRP:0x0[26],LRCK信号反转指示（仅MODESEL的最高位配置’0’时有效）; 0：不反转; 1：反转; 【注意】本模块中LRCK的低电平表示右声道，高电平表示左声道。而有些应用的时序刚好相反（即低表示左声道，高表示右声道），此时需将该比特配置为’1’。,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x0_lrckrp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value = ((reg_value >> I2S_REG0X0_LRCKRP_POS) & I2S_REG0X0_LRCKRP_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x0_lrckrp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value &= ~(I2S_REG0X0_LRCKRP_MASK << I2S_REG0X0_LRCKRP_POS);
    reg_value |= ((value & I2S_REG0X0_LRCKRP_MASK) << I2S_REG0X0_LRCKRP_POS);
    REG_WRITE(I2S_REG0X0_ADDR,reg_value);
}

/* REG_0x00:reg0x0->MODESEL:0x0[29:27],模块工作模式选择信号; 000：I2S (Philips); 001：Left Justified; 010：Right Justified; 011：保留; 100：Short Frame Sync; 101：Long Frame Sync; 110：Normal 2B+D（MSB位于LRCK的第一个上升沿位置）; 111：Delay 2B+D （MSB位于LRCK的第二个上升沿位置）; 【注意】2B+D模式下，若D等于0即GCI时序，若D不等于0即IDL时序,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x0_modesel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value = ((reg_value >> I2S_REG0X0_MODESEL_POS) & I2S_REG0X0_MODESEL_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x0_modesel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value &= ~(I2S_REG0X0_MODESEL_MASK << I2S_REG0X0_MODESEL_POS);
    reg_value |= ((value & I2S_REG0X0_MODESEL_MASK) << I2S_REG0X0_MODESEL_POS);
    REG_WRITE(I2S_REG0X0_ADDR,reg_value);
}

/* REG_0x00:reg0x0->MSTEN:0x0[30],主模式使能信号; 0：工作于从模式; 1：工作于主模式,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x0_msten(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value = ((reg_value >> I2S_REG0X0_MSTEN_POS) & I2S_REG0X0_MSTEN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x0_msten(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value &= ~(I2S_REG0X0_MSTEN_MASK << I2S_REG0X0_MSTEN_POS);
    reg_value |= ((value & I2S_REG0X0_MSTEN_MASK) << I2S_REG0X0_MSTEN_POS);
    REG_WRITE(I2S_REG0X0_ADDR,reg_value);
}

/* REG_0x00:reg0x0->I2SPCMEN:0x0[31],I2S使能信号; 0：禁止模块工作; 1：允许模块工作,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x0_i2spcmen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value = ((reg_value >> I2S_REG0X0_I2SPCMEN_POS) & I2S_REG0X0_I2SPCMEN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x0_i2spcmen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X0_ADDR);
    reg_value &= ~(I2S_REG0X0_I2SPCMEN_MASK << I2S_REG0X0_I2SPCMEN_POS);
    reg_value |= ((value & I2S_REG0X0_I2SPCMEN_MASK) << I2S_REG0X0_I2SPCMEN_POS);
    REG_WRITE(I2S_REG0X0_ADDR,reg_value);
}

/* REG_0x01 */
#define I2S_REG0X1_ADDR  (I2S_LL_REG_BASE  + 0x1*4) //REG ADDR :0x47810004
#define I2S_REG0X1_RXINT_EN_POS (0) 
#define I2S_REG0X1_RXINT_EN_MASK (0x1) 

#define I2S_REG0X1_TXINT_EN_POS (1) 
#define I2S_REG0X1_TXINT_EN_MASK (0x1) 

#define I2S_REG0X1_RXOVF_EN_POS (2) 
#define I2S_REG0X1_RXOVF_EN_MASK (0x1) 

#define I2S_REG0X1_TXUDF_EN_POS (3) 
#define I2S_REG0X1_TXUDF_EN_MASK (0x1) 

#define I2S_REG0X1_RXINT_LEVEL_POS (4) 
#define I2S_REG0X1_RXINT_LEVEL_MASK (0x3) 

#define I2S_REG0X1_TXINT_LEVEL_POS (6) 
#define I2S_REG0X1_TXINT_LEVEL_MASK (0x3) 

#define I2S_REG0X1_TXFIFO_CLR_POS (8) 
#define I2S_REG0X1_TXFIFO_CLR_MASK (0x1) 

#define I2S_REG0X1_RXFIFO_CLR_POS (9) 
#define I2S_REG0X1_RXFIFO_CLR_MASK (0x1) 

#define I2S_REG0X1_SMPRATIO_H2B_POS (10) 
#define I2S_REG0X1_SMPRATIO_H2B_MASK (0x3) 

#define I2S_REG0X1_BITRATIO_H4B_POS (12) 
#define I2S_REG0X1_BITRATIO_H4B_MASK (0xF) 

#define I2S_REG0X1_LRCOM_STORE_POS (16) 
#define I2S_REG0X1_LRCOM_STORE_MASK (0x1) 

#define I2S_REG0X1_PARALLEL_EN_POS (17) 
#define I2S_REG0X1_PARALLEL_EN_MASK (0x1) 

#define I2S_REG0X1_RESERVED_POS (18) 
#define I2S_REG0X1_RESERVED_MASK (0x3FFF) 

static inline uint32_t i2s_ll_get_reg0x1_value(void)
{
    return REG_READ(I2S_REG0X1_ADDR);
}

static inline void i2s_ll_set_reg0x1_value(uint32_t value)
{
    REG_WRITE(I2S_REG0X1_ADDR,value);
}

/* REG_0x01:reg0x1->RXINT_EN:0x1[0],接收中断使能,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x1_rxint_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value = ((reg_value >> I2S_REG0X1_RXINT_EN_POS) & I2S_REG0X1_RXINT_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x1_rxint_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value &= ~(I2S_REG0X1_RXINT_EN_MASK << I2S_REG0X1_RXINT_EN_POS);
    reg_value |= ((value & I2S_REG0X1_RXINT_EN_MASK) << I2S_REG0X1_RXINT_EN_POS);
    REG_WRITE(I2S_REG0X1_ADDR,reg_value);
}

/* REG_0x01:reg0x1->TXINT_EN:0x1[1],发送中断使能,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x1_txint_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value = ((reg_value >> I2S_REG0X1_TXINT_EN_POS) & I2S_REG0X1_TXINT_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x1_txint_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value &= ~(I2S_REG0X1_TXINT_EN_MASK << I2S_REG0X1_TXINT_EN_POS);
    reg_value |= ((value & I2S_REG0X1_TXINT_EN_MASK) << I2S_REG0X1_TXINT_EN_POS);
    REG_WRITE(I2S_REG0X1_ADDR,reg_value);
}

/* REG_0x01:reg0x1->RXOVF_EN:0x1[2],接收上溢(RXOVF)中断告警使能,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x1_rxovf_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value = ((reg_value >> I2S_REG0X1_RXOVF_EN_POS) & I2S_REG0X1_RXOVF_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x1_rxovf_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value &= ~(I2S_REG0X1_RXOVF_EN_MASK << I2S_REG0X1_RXOVF_EN_POS);
    reg_value |= ((value & I2S_REG0X1_RXOVF_EN_MASK) << I2S_REG0X1_RXOVF_EN_POS);
    REG_WRITE(I2S_REG0X1_ADDR,reg_value);
}

/* REG_0x01:reg0x1->TXUDF_EN:0x1[3],发送下溢(TXUDF)中断告警使能,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x1_txudf_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value = ((reg_value >> I2S_REG0X1_TXUDF_EN_POS) & I2S_REG0X1_TXUDF_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x1_txudf_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value &= ~(I2S_REG0X1_TXUDF_EN_MASK << I2S_REG0X1_TXUDF_EN_POS);
    reg_value |= ((value & I2S_REG0X1_TXUDF_EN_MASK) << I2S_REG0X1_TXUDF_EN_POS);
    REG_WRITE(I2S_REG0X1_ADDR,reg_value);
}

/* REG_0x01:reg0x1->RXINT_LEVEL:0x1[5:4],接收中断产生条件; 0：接收FIFO中有效数据超过1个就产生中断; 1：接收FIFO中有效数据超过8个才产生中断; 2：接收FIFO中有效数据超过16个才产生中断; 3：接收FIFO中有效数据超过24个才产生中断,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x1_rxint_level(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value = ((reg_value >> I2S_REG0X1_RXINT_LEVEL_POS) & I2S_REG0X1_RXINT_LEVEL_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x1_rxint_level(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value &= ~(I2S_REG0X1_RXINT_LEVEL_MASK << I2S_REG0X1_RXINT_LEVEL_POS);
    reg_value |= ((value & I2S_REG0X1_RXINT_LEVEL_MASK) << I2S_REG0X1_RXINT_LEVEL_POS);
    REG_WRITE(I2S_REG0X1_ADDR,reg_value);
}

/* REG_0x01:reg0x1->TXINT_LEVEL:0x1[7:6],发送中断产生条件; 0：发送FIFO中有效数据少于1个就产生中断; 1：发送FIFO中有效数据少于8个就产生中断; 2：发送FIFO中有效数据少于16个就产生中断; 3：发送FIFO中有效数据少于24个就产生中断,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x1_txint_level(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value = ((reg_value >> I2S_REG0X1_TXINT_LEVEL_POS) & I2S_REG0X1_TXINT_LEVEL_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x1_txint_level(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value &= ~(I2S_REG0X1_TXINT_LEVEL_MASK << I2S_REG0X1_TXINT_LEVEL_POS);
    reg_value |= ((value & I2S_REG0X1_TXINT_LEVEL_MASK) << I2S_REG0X1_TXINT_LEVEL_POS);
    REG_WRITE(I2S_REG0X1_ADDR,reg_value);
}

/* REG_0x01:reg0x1->TXFIFO_CLR:0x1[8],发送FIFO清除。读回值永远是0,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x1_txfifo_clr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value = ((reg_value >> I2S_REG0X1_TXFIFO_CLR_POS) & I2S_REG0X1_TXFIFO_CLR_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x1_txfifo_clr(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value &= ~(I2S_REG0X1_TXFIFO_CLR_MASK << I2S_REG0X1_TXFIFO_CLR_POS);
    reg_value |= ((value & I2S_REG0X1_TXFIFO_CLR_MASK) << I2S_REG0X1_TXFIFO_CLR_POS);
    REG_WRITE(I2S_REG0X1_ADDR,reg_value);
}

/* REG_0x01:reg0x1->RXFIFO_CLR:0x1[9],接收FIFO清除。读回值永远是0,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x1_rxfifo_clr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value = ((reg_value >> I2S_REG0X1_RXFIFO_CLR_POS) & I2S_REG0X1_RXFIFO_CLR_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x1_rxfifo_clr(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value &= ~(I2S_REG0X1_RXFIFO_CLR_MASK << I2S_REG0X1_RXFIFO_CLR_POS);
    reg_value |= ((value & I2S_REG0X1_RXFIFO_CLR_MASK) << I2S_REG0X1_RXFIFO_CLR_POS);
    REG_WRITE(I2S_REG0X1_ADDR,reg_value);
}

/* REG_0x01:reg0x1->SMPRATIO_H2B:0x1[11:10],采样速率指示值的高2bit,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x1_smpratio_h2b(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value = ((reg_value >> I2S_REG0X1_SMPRATIO_H2B_POS) & I2S_REG0X1_SMPRATIO_H2B_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x1_smpratio_h2b(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value &= ~(I2S_REG0X1_SMPRATIO_H2B_MASK << I2S_REG0X1_SMPRATIO_H2B_POS);
    reg_value |= ((value & I2S_REG0X1_SMPRATIO_H2B_MASK) << I2S_REG0X1_SMPRATIO_H2B_POS);
    REG_WRITE(I2S_REG0X1_ADDR,reg_value);
}

/* REG_0x01:reg0x1->BITRATIO_H4B:0x1[15:12],比特速率指示值的高4bit,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x1_bitratio_h4b(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value = ((reg_value >> I2S_REG0X1_BITRATIO_H4B_POS) & I2S_REG0X1_BITRATIO_H4B_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x1_bitratio_h4b(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value &= ~(I2S_REG0X1_BITRATIO_H4B_MASK << I2S_REG0X1_BITRATIO_H4B_POS);
    reg_value |= ((value & I2S_REG0X1_BITRATIO_H4B_MASK) << I2S_REG0X1_BITRATIO_H4B_POS);
    REG_WRITE(I2S_REG0X1_ADDR,reg_value);
}

/* REG_0x01:reg0x1->LRCOM_STORE:0x1[16],0x1：左右声道数据拼成一个32bit同时写入PCM_DAT中，低16bit对应左声道，高16bit对应右声道。即{R,L}->{R,L}->……; 0x0：左右声道数据按时间顺序交替写入PCM_DAT中，即L->R->L->R->…… ; 【注意】仅当数据长度小于16时才能配置该位为1,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x1_lrcom_store(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value = ((reg_value >> I2S_REG0X1_LRCOM_STORE_POS) & I2S_REG0X1_LRCOM_STORE_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x1_lrcom_store(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value &= ~(I2S_REG0X1_LRCOM_STORE_MASK << I2S_REG0X1_LRCOM_STORE_POS);
    reg_value |= ((value & I2S_REG0X1_LRCOM_STORE_MASK) << I2S_REG0X1_LRCOM_STORE_POS);
    REG_WRITE(I2S_REG0X1_ADDR,reg_value);
}

/* REG_0x01:reg0x1->PARALLEL_EN:0x1[17],0x0: N(1~4)通道的数据通过sda_i[0]/sda_o[0]串行输入/输出; 0x1: N(1~4)通道的数据通过N个信号线sda_i[N-1:0]/sda_o[N-1:0]输入/输出,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x1_parallel_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value = ((reg_value >> I2S_REG0X1_PARALLEL_EN_POS) & I2S_REG0X1_PARALLEL_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x1_parallel_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X1_ADDR);
    reg_value &= ~(I2S_REG0X1_PARALLEL_EN_MASK << I2S_REG0X1_PARALLEL_EN_POS);
    reg_value |= ((value & I2S_REG0X1_PARALLEL_EN_MASK) << I2S_REG0X1_PARALLEL_EN_POS);
    REG_WRITE(I2S_REG0X1_ADDR,reg_value);
}

/* REG_0x02 */
#define I2S_REG0X2_ADDR  (I2S_LL_REG_BASE  + 0x2*4) //REG ADDR :0x47810008
#define I2S_REG0X2_RXINT_POS (0) 
#define I2S_REG0X2_RXINT_MASK (0x1) 

#define I2S_REG0X2_TXINT_POS (1) 
#define I2S_REG0X2_TXINT_MASK (0x1) 

#define I2S_REG0X2_RXOVF_POS (2) 
#define I2S_REG0X2_RXOVF_MASK (0x1) 

#define I2S_REG0X2_TXUDF_POS (3) 
#define I2S_REG0X2_TXUDF_MASK (0x1) 

#define I2S_REG0X2_RXFIFO_RD_READY_POS (4) 
#define I2S_REG0X2_RXFIFO_RD_READY_MASK (0x1) 

#define I2S_REG0X2_TXFIFO_WR_READY_POS (5) 
#define I2S_REG0X2_TXFIFO_WR_READY_MASK (0x1) 

#define I2S_REG0X2_RESERVED_POS (6) 
#define I2S_REG0X2_RESERVED_MASK (0x3FFFFFF) 

static inline uint32_t i2s_ll_get_reg0x2_value(void)
{
    return REG_READ(I2S_REG0X2_ADDR);
}

/* REG_0x02:reg0x2->RXINT:0x2[0],I2S接收中断标志。软件从接收FIFO中读取足够数据后该中断会自动清除,0x0,R*/
static inline uint32_t i2s_ll_get_reg0x2_rxint(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X2_ADDR);
    reg_value = ((reg_value >> I2S_REG0X2_RXINT_POS)&I2S_REG0X2_RXINT_MASK);
    return reg_value;
}

/* REG_0x02:reg0x2->TXINT:0x2[1],I2S发送中断标志。软件往发送FIFO中写入足够数据后该中断会自动清除,0x0,R*/
static inline uint32_t i2s_ll_get_reg0x2_txint(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X2_ADDR);
    reg_value = ((reg_value >> I2S_REG0X2_TXINT_POS)&I2S_REG0X2_TXINT_MASK);
    return reg_value;
}

/* REG_0x02:reg0x2->RXOVF:0x2[2],接收上溢标志。即接收左声道FIFO发生满写错误，会产生一个I2S中断，该位不能被硬件自动清0，必须由软件写1清0。.,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x2_rxovf(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X2_ADDR);
    reg_value = ((reg_value >> I2S_REG0X2_RXOVF_POS) & I2S_REG0X2_RXOVF_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x2_rxovf(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X2_ADDR);
    reg_value &= ~(I2S_REG0X2_RXOVF_MASK << I2S_REG0X2_RXOVF_POS);
    reg_value |= ((value & I2S_REG0X2_RXOVF_MASK) << I2S_REG0X2_RXOVF_POS);
    REG_WRITE(I2S_REG0X2_ADDR,reg_value);
}

/* REG_0x02:reg0x2->TXUDF:0x2[3],发送下溢标志。即发送左声道FIFO发生空读错误，会产生一个I2S中断，该位不能被硬件自动清0，必须由软件写1清0。.,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x2_txudf(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X2_ADDR);
    reg_value = ((reg_value >> I2S_REG0X2_TXUDF_POS) & I2S_REG0X2_TXUDF_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x2_txudf(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X2_ADDR);
    reg_value &= ~(I2S_REG0X2_TXUDF_MASK << I2S_REG0X2_TXUDF_POS);
    reg_value |= ((value & I2S_REG0X2_TXUDF_MASK) << I2S_REG0X2_TXUDF_POS);
    REG_WRITE(I2S_REG0X2_ADDR,reg_value);
}

/* REG_0x02:reg0x2->RXFIFO_RD_READY:0x2[4],RXFIFO读允许标志信号,0x0,R*/
static inline uint32_t i2s_ll_get_reg0x2_rxfifo_rd_ready(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X2_ADDR);
    reg_value = ((reg_value >> I2S_REG0X2_RXFIFO_RD_READY_POS)&I2S_REG0X2_RXFIFO_RD_READY_MASK);
    return reg_value;
}

/* REG_0x02:reg0x2->TXFIFO_WR_READY:0x2[5],TXFIFO写允许标志信号,0x0,R*/
static inline uint32_t i2s_ll_get_reg0x2_txfifo_wr_ready(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X2_ADDR);
    reg_value = ((reg_value >> I2S_REG0X2_TXFIFO_WR_READY_POS)&I2S_REG0X2_TXFIFO_WR_READY_MASK);
    return reg_value;
}

/* REG_0x03 */
#define I2S_REG0X3_ADDR  (I2S_LL_REG_BASE  + 0x3*4) //REG ADDR :0x4781000c
#define I2S_REG0X3_I2S_DAT_POS (0) 
#define I2S_REG0X3_I2S_DAT_MASK (0xFFFFFFFF) 

static inline uint32_t i2s_ll_get_reg0x3_value(void)
{
    return REG_READ(I2S_REG0X3_ADDR);
}

static inline void i2s_ll_set_reg0x3_value(uint32_t value)
{
    REG_WRITE(I2S_REG0X3_ADDR,value);
}

/* REG_0x03:reg0x3->I2S_DAT:0x3[31:0],I2S/PCM数据寄存器。（对该寄存器的写操作对应写发送FIFO，读操作对应读接收FIFO）; 【注意】当LRCOM_STORE配置为0时，左右声道数据需要交替读写。当LRCOM_STORE配置为1时，低16bit对应左声道，高16bit对应右声道,None,R/W*/
static inline uint32_t i2s_ll_get_reg0x3_i2s_dat(void)
{
    return REG_READ(I2S_REG0X3_ADDR);
}

static inline void i2s_ll_set_reg0x3_i2s_dat(uint32_t value)
{
    REG_WRITE(I2S_REG0X3_ADDR,value);
}

/* REG_0x04 */
#define I2S_REG0X4_ADDR  (I2S_LL_REG_BASE  + 0x4*4) //REG ADDR :0x47810010
#define I2S_REG0X4_RX2INT_EN_POS (0) 
#define I2S_REG0X4_RX2INT_EN_MASK (0x1) 

#define I2S_REG0X4_TX2INT_EN_POS (1) 
#define I2S_REG0X4_TX2INT_EN_MASK (0x1) 

#define I2S_REG0X4_RX2OVF_EN_POS (2) 
#define I2S_REG0X4_RX2OVF_EN_MASK (0x1) 

#define I2S_REG0X4_TX2UDF_EN_POS (3) 
#define I2S_REG0X4_TX2UDF_EN_MASK (0x1) 

#define I2S_REG0X4_RX3INT_EN_POS (4) 
#define I2S_REG0X4_RX3INT_EN_MASK (0x1) 

#define I2S_REG0X4_TX3INT_EN_POS (5) 
#define I2S_REG0X4_TX3INT_EN_MASK (0x1) 

#define I2S_REG0X4_RX3OVF_EN_POS (6) 
#define I2S_REG0X4_RX3OVF_EN_MASK (0x1) 

#define I2S_REG0X4_TX3UDF_EN_POS (7) 
#define I2S_REG0X4_TX3UDF_EN_MASK (0x1) 

#define I2S_REG0X4_RX4INT_EN_POS (8) 
#define I2S_REG0X4_RX4INT_EN_MASK (0x1) 

#define I2S_REG0X4_TX4INT_EN_POS (9) 
#define I2S_REG0X4_TX4INT_EN_MASK (0x1) 

#define I2S_REG0X4_RX4OVF_EN_POS (10) 
#define I2S_REG0X4_RX4OVF_EN_MASK (0x1) 

#define I2S_REG0X4_TX4UDF_EN_POS (11) 
#define I2S_REG0X4_TX4UDF_EN_MASK (0x1) 

#define I2S_REG0X4_RESERVED_POS (12) 
#define I2S_REG0X4_RESERVED_MASK (0xFFFFF) 

static inline uint32_t i2s_ll_get_reg0x4_value(void)
{
    return REG_READ(I2S_REG0X4_ADDR);
}

static inline void i2s_ll_set_reg0x4_value(uint32_t value)
{
    REG_WRITE(I2S_REG0X4_ADDR,value);
}

/* REG_0x04:reg0x4->RX2INT_EN:0x4[0],通道2的接收中断使能,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x4_rx2int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value = ((reg_value >> I2S_REG0X4_RX2INT_EN_POS) & I2S_REG0X4_RX2INT_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x4_rx2int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value &= ~(I2S_REG0X4_RX2INT_EN_MASK << I2S_REG0X4_RX2INT_EN_POS);
    reg_value |= ((value & I2S_REG0X4_RX2INT_EN_MASK) << I2S_REG0X4_RX2INT_EN_POS);
    REG_WRITE(I2S_REG0X4_ADDR,reg_value);
}

/* REG_0x04:reg0x4->TX2INT_EN:0x4[1],通道2的发送中断使能,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x4_tx2int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value = ((reg_value >> I2S_REG0X4_TX2INT_EN_POS) & I2S_REG0X4_TX2INT_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x4_tx2int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value &= ~(I2S_REG0X4_TX2INT_EN_MASK << I2S_REG0X4_TX2INT_EN_POS);
    reg_value |= ((value & I2S_REG0X4_TX2INT_EN_MASK) << I2S_REG0X4_TX2INT_EN_POS);
    REG_WRITE(I2S_REG0X4_ADDR,reg_value);
}

/* REG_0x04:reg0x4->RX2OVF_EN:0x4[2],通道2的接收上溢(RXOVF)中断告警使能,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x4_rx2ovf_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value = ((reg_value >> I2S_REG0X4_RX2OVF_EN_POS) & I2S_REG0X4_RX2OVF_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x4_rx2ovf_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value &= ~(I2S_REG0X4_RX2OVF_EN_MASK << I2S_REG0X4_RX2OVF_EN_POS);
    reg_value |= ((value & I2S_REG0X4_RX2OVF_EN_MASK) << I2S_REG0X4_RX2OVF_EN_POS);
    REG_WRITE(I2S_REG0X4_ADDR,reg_value);
}

/* REG_0x04:reg0x4->TX2UDF_EN:0x4[3],通道2的发送下溢(TXUDF)中断告警使能,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x4_tx2udf_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value = ((reg_value >> I2S_REG0X4_TX2UDF_EN_POS) & I2S_REG0X4_TX2UDF_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x4_tx2udf_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value &= ~(I2S_REG0X4_TX2UDF_EN_MASK << I2S_REG0X4_TX2UDF_EN_POS);
    reg_value |= ((value & I2S_REG0X4_TX2UDF_EN_MASK) << I2S_REG0X4_TX2UDF_EN_POS);
    REG_WRITE(I2S_REG0X4_ADDR,reg_value);
}

/* REG_0x04:reg0x4->RX3INT_EN:0x4[4],通道3的接收中断使能,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x4_rx3int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value = ((reg_value >> I2S_REG0X4_RX3INT_EN_POS) & I2S_REG0X4_RX3INT_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x4_rx3int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value &= ~(I2S_REG0X4_RX3INT_EN_MASK << I2S_REG0X4_RX3INT_EN_POS);
    reg_value |= ((value & I2S_REG0X4_RX3INT_EN_MASK) << I2S_REG0X4_RX3INT_EN_POS);
    REG_WRITE(I2S_REG0X4_ADDR,reg_value);
}

/* REG_0x04:reg0x4->TX3INT_EN:0x4[5],通道3的发送中断使能,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x4_tx3int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value = ((reg_value >> I2S_REG0X4_TX3INT_EN_POS) & I2S_REG0X4_TX3INT_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x4_tx3int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value &= ~(I2S_REG0X4_TX3INT_EN_MASK << I2S_REG0X4_TX3INT_EN_POS);
    reg_value |= ((value & I2S_REG0X4_TX3INT_EN_MASK) << I2S_REG0X4_TX3INT_EN_POS);
    REG_WRITE(I2S_REG0X4_ADDR,reg_value);
}

/* REG_0x04:reg0x4->RX3OVF_EN:0x4[6],通道3的接收上溢(RXOVF)中断告警使能,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x4_rx3ovf_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value = ((reg_value >> I2S_REG0X4_RX3OVF_EN_POS) & I2S_REG0X4_RX3OVF_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x4_rx3ovf_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value &= ~(I2S_REG0X4_RX3OVF_EN_MASK << I2S_REG0X4_RX3OVF_EN_POS);
    reg_value |= ((value & I2S_REG0X4_RX3OVF_EN_MASK) << I2S_REG0X4_RX3OVF_EN_POS);
    REG_WRITE(I2S_REG0X4_ADDR,reg_value);
}

/* REG_0x04:reg0x4->TX3UDF_EN:0x4[7],通道3的发送下溢(TXUDF)中断告警使能,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x4_tx3udf_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value = ((reg_value >> I2S_REG0X4_TX3UDF_EN_POS) & I2S_REG0X4_TX3UDF_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x4_tx3udf_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value &= ~(I2S_REG0X4_TX3UDF_EN_MASK << I2S_REG0X4_TX3UDF_EN_POS);
    reg_value |= ((value & I2S_REG0X4_TX3UDF_EN_MASK) << I2S_REG0X4_TX3UDF_EN_POS);
    REG_WRITE(I2S_REG0X4_ADDR,reg_value);
}

/* REG_0x04:reg0x4->RX4INT_EN:0x4[8],通道4的接收中断使能,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x4_rx4int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value = ((reg_value >> I2S_REG0X4_RX4INT_EN_POS) & I2S_REG0X4_RX4INT_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x4_rx4int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value &= ~(I2S_REG0X4_RX4INT_EN_MASK << I2S_REG0X4_RX4INT_EN_POS);
    reg_value |= ((value & I2S_REG0X4_RX4INT_EN_MASK) << I2S_REG0X4_RX4INT_EN_POS);
    REG_WRITE(I2S_REG0X4_ADDR,reg_value);
}

/* REG_0x04:reg0x4->TX4INT_EN:0x4[9],通道4的发送中断使能,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x4_tx4int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value = ((reg_value >> I2S_REG0X4_TX4INT_EN_POS) & I2S_REG0X4_TX4INT_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x4_tx4int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value &= ~(I2S_REG0X4_TX4INT_EN_MASK << I2S_REG0X4_TX4INT_EN_POS);
    reg_value |= ((value & I2S_REG0X4_TX4INT_EN_MASK) << I2S_REG0X4_TX4INT_EN_POS);
    REG_WRITE(I2S_REG0X4_ADDR,reg_value);
}

/* REG_0x04:reg0x4->RX4OVF_EN:0x4[10],通道4的接收上溢(RXOVF)中断告警使能,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x4_rx4ovf_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value = ((reg_value >> I2S_REG0X4_RX4OVF_EN_POS) & I2S_REG0X4_RX4OVF_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x4_rx4ovf_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value &= ~(I2S_REG0X4_RX4OVF_EN_MASK << I2S_REG0X4_RX4OVF_EN_POS);
    reg_value |= ((value & I2S_REG0X4_RX4OVF_EN_MASK) << I2S_REG0X4_RX4OVF_EN_POS);
    REG_WRITE(I2S_REG0X4_ADDR,reg_value);
}

/* REG_0x04:reg0x4->TX4UDF_EN:0x4[11],通道4的发送下溢(TXUDF)中断告警使能,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x4_tx4udf_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value = ((reg_value >> I2S_REG0X4_TX4UDF_EN_POS) & I2S_REG0X4_TX4UDF_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x4_tx4udf_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X4_ADDR);
    reg_value &= ~(I2S_REG0X4_TX4UDF_EN_MASK << I2S_REG0X4_TX4UDF_EN_POS);
    reg_value |= ((value & I2S_REG0X4_TX4UDF_EN_MASK) << I2S_REG0X4_TX4UDF_EN_POS);
    REG_WRITE(I2S_REG0X4_ADDR,reg_value);
}

/* REG_0x05 */
#define I2S_REG0X5_ADDR  (I2S_LL_REG_BASE  + 0x5*4) //REG ADDR :0x47810014
#define I2S_REG0X5_RX2INT_POS (0) 
#define I2S_REG0X5_RX2INT_MASK (0x1) 

#define I2S_REG0X5_TX2INT_POS (1) 
#define I2S_REG0X5_TX2INT_MASK (0x1) 

#define I2S_REG0X5_RX2OVF_POS (2) 
#define I2S_REG0X5_RX2OVF_MASK (0x1) 

#define I2S_REG0X5_TX2UDF_POS (3) 
#define I2S_REG0X5_TX2UDF_MASK (0x1) 

#define I2S_REG0X5_RX3INT_POS (4) 
#define I2S_REG0X5_RX3INT_MASK (0x1) 

#define I2S_REG0X5_TX3INT_POS (5) 
#define I2S_REG0X5_TX3INT_MASK (0x1) 

#define I2S_REG0X5_RX3OVF_POS (6) 
#define I2S_REG0X5_RX3OVF_MASK (0x1) 

#define I2S_REG0X5_TX3UDF_POS (3) 
#define I2S_REG0X5_TX3UDF_MASK (0x1) 

#define I2S_REG0X5_RX4INT_POS (8) 
#define I2S_REG0X5_RX4INT_MASK (0x1) 

#define I2S_REG0X5_TX4INT_POS (9) 
#define I2S_REG0X5_TX4INT_MASK (0x1) 

#define I2S_REG0X5_RX4OVF_POS (10) 
#define I2S_REG0X5_RX4OVF_MASK (0x1) 

#define I2S_REG0X5_TX4UDF_POS (11) 
#define I2S_REG0X5_TX4UDF_MASK (0x1) 

#define I2S_REG0X5_RESERVED_POS (6) 
#define I2S_REG0X5_RESERVED_MASK (0x3FFFFFF) 

static inline uint32_t i2s_ll_get_reg0x5_value(void)
{
    return REG_READ(I2S_REG0X5_ADDR);
}

/* REG_0x05:reg0x5->RX2INT:0x5[0],通道2的I2S接收中断标志。软件从接收FIFO中读取足够数据后该中断会自动清除,0x0,R*/
static inline uint32_t i2s_ll_get_reg0x5_rx2int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X5_ADDR);
    reg_value = ((reg_value >> I2S_REG0X5_RX2INT_POS)&I2S_REG0X5_RX2INT_MASK);
    return reg_value;
}

/* REG_0x05:reg0x5->TX2INT:0x5[1],通道2的I2S发送中断标志。软件往发送FIFO中写入足够数据后该中断会自动清除,0x0,R*/
static inline uint32_t i2s_ll_get_reg0x5_tx2int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X5_ADDR);
    reg_value = ((reg_value >> I2S_REG0X5_TX2INT_POS)&I2S_REG0X5_TX2INT_MASK);
    return reg_value;
}

/* REG_0x05:reg0x5->RX2OVF:0x5[2],通道2的接收上溢标志。即接收左声道FIFO发生满写错误，会产生一个I2S中断，该位不能被硬件自动清0，必须由软件写1清0。.,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x5_rx2ovf(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X5_ADDR);
    reg_value = ((reg_value >> I2S_REG0X5_RX2OVF_POS) & I2S_REG0X5_RX2OVF_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x5_rx2ovf(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X5_ADDR);
    reg_value &= ~(I2S_REG0X5_RX2OVF_MASK << I2S_REG0X5_RX2OVF_POS);
    reg_value |= ((value & I2S_REG0X5_RX2OVF_MASK) << I2S_REG0X5_RX2OVF_POS);
    REG_WRITE(I2S_REG0X5_ADDR,reg_value);
}

/* REG_0x05:reg0x5->TX2UDF:0x5[3],通道2的发送下溢标志。即发送左声道FIFO发生空读错误，会产生一个I2S中断，该位不能被硬件自动清0，必须由软件写1清0。.,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x5_tx2udf(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X5_ADDR);
    reg_value = ((reg_value >> I2S_REG0X5_TX2UDF_POS) & I2S_REG0X5_TX2UDF_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x5_tx2udf(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X5_ADDR);
    reg_value &= ~(I2S_REG0X5_TX2UDF_MASK << I2S_REG0X5_TX2UDF_POS);
    reg_value |= ((value & I2S_REG0X5_TX2UDF_MASK) << I2S_REG0X5_TX2UDF_POS);
    REG_WRITE(I2S_REG0X5_ADDR,reg_value);
}

/* REG_0x05:reg0x5->RX3INT:0x5[4],通道3的I2S接收中断标志。软件从接收FIFO中读取足够数据后该中断会自动清除,0x0,R*/
static inline uint32_t i2s_ll_get_reg0x5_rx3int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X5_ADDR);
    reg_value = ((reg_value >> I2S_REG0X5_RX3INT_POS)&I2S_REG0X5_RX3INT_MASK);
    return reg_value;
}

/* REG_0x05:reg0x5->TX3INT:0x5[5],通道3的I2S发送中断标志。软件往发送FIFO中写入足够数据后该中断会自动清除,0x0,R*/
static inline uint32_t i2s_ll_get_reg0x5_tx3int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X5_ADDR);
    reg_value = ((reg_value >> I2S_REG0X5_TX3INT_POS)&I2S_REG0X5_TX3INT_MASK);
    return reg_value;
}

/* REG_0x05:reg0x5->RX3OVF:0x5[6],通道3的接收上溢标志。即接收左声道FIFO发生满写错误，会产生一个I2S中断，该位不能被硬件自动清0，必须由软件写1清0。.,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x5_rx3ovf(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X5_ADDR);
    reg_value = ((reg_value >> I2S_REG0X5_RX3OVF_POS) & I2S_REG0X5_RX3OVF_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x5_rx3ovf(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X5_ADDR);
    reg_value &= ~(I2S_REG0X5_RX3OVF_MASK << I2S_REG0X5_RX3OVF_POS);
    reg_value |= ((value & I2S_REG0X5_RX3OVF_MASK) << I2S_REG0X5_RX3OVF_POS);
    REG_WRITE(I2S_REG0X5_ADDR,reg_value);
}

/* REG_0x05:reg0x5->TX3UDF:0x5[3],通道3的发送下溢标志。即发送左声道FIFO发生空读错误，会产生一个I2S中断，该位不能被硬件自动清0，必须由软件写1清0。.,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x5_tx3udf(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X5_ADDR);
    reg_value = ((reg_value >> I2S_REG0X5_TX3UDF_POS) & I2S_REG0X5_TX3UDF_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x5_tx3udf(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X5_ADDR);
    reg_value &= ~(I2S_REG0X5_TX3UDF_MASK << I2S_REG0X5_TX3UDF_POS);
    reg_value |= ((value & I2S_REG0X5_TX3UDF_MASK) << I2S_REG0X5_TX3UDF_POS);
    REG_WRITE(I2S_REG0X5_ADDR,reg_value);
}

/* REG_0x05:reg0x5->RX4INT:0x5[8],通道4的I2S接收中断标志。软件从接收FIFO中读取足够数据后该中断会自动清除,0x0,R*/
static inline uint32_t i2s_ll_get_reg0x5_rx4int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X5_ADDR);
    reg_value = ((reg_value >> I2S_REG0X5_RX4INT_POS)&I2S_REG0X5_RX4INT_MASK);
    return reg_value;
}

/* REG_0x05:reg0x5->TX4INT:0x5[9],通道4的I2S发送中断标志。软件往发送FIFO中写入足够数据后该中断会自动清除,0x0,R*/
static inline uint32_t i2s_ll_get_reg0x5_tx4int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X5_ADDR);
    reg_value = ((reg_value >> I2S_REG0X5_TX4INT_POS)&I2S_REG0X5_TX4INT_MASK);
    return reg_value;
}

/* REG_0x05:reg0x5->RX4OVF:0x5[10],通道4的接收上溢标志。即接收左声道FIFO发生满写错误，会产生一个I2S中断，该位不能被硬件自动清0，必须由软件写1清0。.,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x5_rx4ovf(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X5_ADDR);
    reg_value = ((reg_value >> I2S_REG0X5_RX4OVF_POS) & I2S_REG0X5_RX4OVF_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x5_rx4ovf(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X5_ADDR);
    reg_value &= ~(I2S_REG0X5_RX4OVF_MASK << I2S_REG0X5_RX4OVF_POS);
    reg_value |= ((value & I2S_REG0X5_RX4OVF_MASK) << I2S_REG0X5_RX4OVF_POS);
    REG_WRITE(I2S_REG0X5_ADDR,reg_value);
}

/* REG_0x05:reg0x5->TX4UDF:0x5[11],通道4的发送下溢标志。即发送左声道FIFO发生空读错误，会产生一个I2S中断，该位不能被硬件自动清0，必须由软件写1清0。.,0x0,R/W*/
static inline uint32_t i2s_ll_get_reg0x5_tx4udf(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X5_ADDR);
    reg_value = ((reg_value >> I2S_REG0X5_TX4UDF_POS) & I2S_REG0X5_TX4UDF_MASK);
    return reg_value;
}

static inline void i2s_ll_set_reg0x5_tx4udf(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_REG0X5_ADDR);
    reg_value &= ~(I2S_REG0X5_TX4UDF_MASK << I2S_REG0X5_TX4UDF_POS);
    reg_value |= ((value & I2S_REG0X5_TX4UDF_MASK) << I2S_REG0X5_TX4UDF_POS);
    REG_WRITE(I2S_REG0X5_ADDR,reg_value);
}

/* REG_0x06 */
#define I2S_REG0X6_ADDR  (I2S_LL_REG_BASE  + 0x6*4) //REG ADDR :0x47810018
#define I2S_REG0X6_I2S_DAT2_POS (0) 
#define I2S_REG0X6_I2S_DAT2_MASK (0xFFFFFFFF) 

/*write only reg:reg0x6:default value:0x0*/
static inline void i2s_ll_set_reg0x6_value(uint32_t value)
{
    REG_WRITE(I2S_REG0X6_ADDR,value);
}

/* REG_0x07 */
#define I2S_REG0X7_ADDR  (I2S_LL_REG_BASE  + 0x7*4) //REG ADDR :0x4781001c
#define I2S_REG0X7_I2S_DAT3_POS (0) 
#define I2S_REG0X7_I2S_DAT3_MASK (0xFFFFFFFF) 

/*write only reg:reg0x7:default value:0x0*/
static inline void i2s_ll_set_reg0x7_value(uint32_t value)
{
    REG_WRITE(I2S_REG0X7_ADDR,value);
}

/* REG_0x08 */
#define I2S_REG0X8_ADDR  (I2S_LL_REG_BASE  + 0x8*4) //REG ADDR :0x47810020
#define I2S_REG0X8_I2S_DAT4_POS (0) 
#define I2S_REG0X8_I2S_DAT4_MASK (0xFFFFFFFF) 

/*write only reg:reg0x8:default value:0x0*/
static inline void i2s_ll_set_reg0x8_value(uint32_t value)
{
    REG_WRITE(I2S_REG0X8_ADDR,value);
}

#ifdef __cplusplus 
}                  
#endif             
