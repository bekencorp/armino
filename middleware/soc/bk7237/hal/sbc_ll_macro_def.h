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
* CHIP ID:BK7256,GENARATE TIME:2022-02-11 14:08:08                                                 
************************************************************************************************************************************/

#pragma once                 
                            
#ifdef __cplusplus          
extern "C" {              
#endif                      

#include <soc/soc.h>          

#define SBC_LL_REG_BASE      (SOC_SBC_REG_BASE) //REG_BASE:0x47010000

/* REG_0x00 */
#define SBC_SBC_CTRL_ADDR  (SBC_LL_REG_BASE  + 0x0*4) //REG ADDR :0x47010000
#define SBC_SBC_CTRL_SBC_EN_POS (0) 
#define SBC_SBC_CTRL_SBC_EN_MASK (0x1) 

#define SBC_SBC_CTRL_SBC_CHANNEL_POS (1) 
#define SBC_SBC_CTRL_SBC_CHANNEL_MASK (0x1) 

#define SBC_SBC_CTRL_SBC_SUBBAND_POS (2) 
#define SBC_SBC_CTRL_SBC_SUBBAND_MASK (0x1) 

#define SBC_SBC_CTRL_SBC_CHN_COMB_POS (3) 
#define SBC_SBC_CTRL_SBC_CHN_COMB_MASK (0x1) 

#define SBC_SBC_CTRL_SBC_BLOCKS_POS (4) 
#define SBC_SBC_CTRL_SBC_BLOCKS_MASK (0x3) 

#define SBC_SBC_CTRL_MSBC_SUPPORT_POS (6) 
#define SBC_SBC_CTRL_MSBC_SUPPORT_MASK (0x1) 

#define SBC_SBC_CTRL_RESERVED_POS (7) 
#define SBC_SBC_CTRL_RESERVED_MASK (0x1FFFFFF) 

static inline uint32_t sbc_ll_get_sbc_ctrl_value(void)
{
    return REG_READ(SBC_SBC_CTRL_ADDR);
}

static inline void sbc_ll_set_sbc_ctrl_value(uint32_t value)
{
    REG_WRITE(SBC_SBC_CTRL_ADDR,value);
}

/* REG_0x00:sbc_ctrl->sbc_en:0x0[0],SBC enable,0,RW*/
static inline uint32_t sbc_ll_get_sbc_ctrl_sbc_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_SBC_CTRL_ADDR);
    reg_value = ((reg_value >> SBC_SBC_CTRL_SBC_EN_POS) & SBC_SBC_CTRL_SBC_EN_MASK);
    return reg_value;
}

static inline void sbc_ll_set_sbc_ctrl_sbc_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_SBC_CTRL_ADDR);
    reg_value &= ~(SBC_SBC_CTRL_SBC_EN_MASK << SBC_SBC_CTRL_SBC_EN_POS);
    reg_value |= ((value & SBC_SBC_CTRL_SBC_EN_MASK) << SBC_SBC_CTRL_SBC_EN_POS);
    REG_WRITE(SBC_SBC_CTRL_ADDR,reg_value);
}

/* REG_0x00:sbc_ctrl->sbc_channel:0x0[1],sbc channel：; 0: 1 channel; 1: 2 channel,0,RW*/
static inline uint32_t sbc_ll_get_sbc_ctrl_sbc_channel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_SBC_CTRL_ADDR);
    reg_value = ((reg_value >> SBC_SBC_CTRL_SBC_CHANNEL_POS) & SBC_SBC_CTRL_SBC_CHANNEL_MASK);
    return reg_value;
}

static inline void sbc_ll_set_sbc_ctrl_sbc_channel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_SBC_CTRL_ADDR);
    reg_value &= ~(SBC_SBC_CTRL_SBC_CHANNEL_MASK << SBC_SBC_CTRL_SBC_CHANNEL_POS);
    reg_value |= ((value & SBC_SBC_CTRL_SBC_CHANNEL_MASK) << SBC_SBC_CTRL_SBC_CHANNEL_POS);
    REG_WRITE(SBC_SBC_CTRL_ADDR,reg_value);
}

/* REG_0x00:sbc_ctrl->sbc_subband:0x0[2],sbc subband number：; 0：4 subband  ; 1: 8 subband,0,RW*/
static inline uint32_t sbc_ll_get_sbc_ctrl_sbc_subband(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_SBC_CTRL_ADDR);
    reg_value = ((reg_value >> SBC_SBC_CTRL_SBC_SUBBAND_POS) & SBC_SBC_CTRL_SBC_SUBBAND_MASK);
    return reg_value;
}

static inline void sbc_ll_set_sbc_ctrl_sbc_subband(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_SBC_CTRL_ADDR);
    reg_value &= ~(SBC_SBC_CTRL_SBC_SUBBAND_MASK << SBC_SBC_CTRL_SBC_SUBBAND_POS);
    reg_value |= ((value & SBC_SBC_CTRL_SBC_SUBBAND_MASK) << SBC_SBC_CTRL_SBC_SUBBAND_POS);
    REG_WRITE(SBC_SBC_CTRL_ADDR,reg_value);
}

/* REG_0x00:sbc_ctrl->sbc_chn_comb:0x0[3],sbc decode output structure： ; 0：｛16'd0, pcm}; 1: ｛pcm_chn2, pcm_chn1},0,RW*/
static inline uint32_t sbc_ll_get_sbc_ctrl_sbc_chn_comb(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_SBC_CTRL_ADDR);
    reg_value = ((reg_value >> SBC_SBC_CTRL_SBC_CHN_COMB_POS) & SBC_SBC_CTRL_SBC_CHN_COMB_MASK);
    return reg_value;
}

static inline void sbc_ll_set_sbc_ctrl_sbc_chn_comb(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_SBC_CTRL_ADDR);
    reg_value &= ~(SBC_SBC_CTRL_SBC_CHN_COMB_MASK << SBC_SBC_CTRL_SBC_CHN_COMB_POS);
    reg_value |= ((value & SBC_SBC_CTRL_SBC_CHN_COMB_MASK) << SBC_SBC_CTRL_SBC_CHN_COMB_POS);
    REG_WRITE(SBC_SBC_CTRL_ADDR,reg_value);
}

/* REG_0x00:sbc_ctrl->sbc_blocks:0x0[5:4],sbc block number：; 00： 4; 01： 8; 10： 12; 11： 16,0,RW*/
static inline uint32_t sbc_ll_get_sbc_ctrl_sbc_blocks(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_SBC_CTRL_ADDR);
    reg_value = ((reg_value >> SBC_SBC_CTRL_SBC_BLOCKS_POS) & SBC_SBC_CTRL_SBC_BLOCKS_MASK);
    return reg_value;
}

static inline void sbc_ll_set_sbc_ctrl_sbc_blocks(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_SBC_CTRL_ADDR);
    reg_value &= ~(SBC_SBC_CTRL_SBC_BLOCKS_MASK << SBC_SBC_CTRL_SBC_BLOCKS_POS);
    reg_value |= ((value & SBC_SBC_CTRL_SBC_BLOCKS_MASK) << SBC_SBC_CTRL_SBC_BLOCKS_POS);
    REG_WRITE(SBC_SBC_CTRL_ADDR,reg_value);
}

/* REG_0x00:sbc_ctrl->msbc_support:0x0[6],support msbc encode,0,RW*/
static inline uint32_t sbc_ll_get_sbc_ctrl_msbc_support(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_SBC_CTRL_ADDR);
    reg_value = ((reg_value >> SBC_SBC_CTRL_MSBC_SUPPORT_POS) & SBC_SBC_CTRL_MSBC_SUPPORT_MASK);
    return reg_value;
}

static inline void sbc_ll_set_sbc_ctrl_msbc_support(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_SBC_CTRL_ADDR);
    reg_value &= ~(SBC_SBC_CTRL_MSBC_SUPPORT_MASK << SBC_SBC_CTRL_MSBC_SUPPORT_POS);
    reg_value |= ((value & SBC_SBC_CTRL_MSBC_SUPPORT_MASK) << SBC_SBC_CTRL_MSBC_SUPPORT_POS);
    REG_WRITE(SBC_SBC_CTRL_ADDR,reg_value);
}

/* REG_0x01 */
#define SBC_SBC_STATE_ADDR  (SBC_LL_REG_BASE  + 0x1*4) //REG ADDR :0x47010004
#define SBC_SBC_STATE_SBC_INT_POS (0) 
#define SBC_SBC_STATE_SBC_INT_MASK (0x1) 

#define SBC_SBC_STATE_SBC_IDLE_POS (1) 
#define SBC_SBC_STATE_SBC_IDLE_MASK (0x1) 

#define SBC_SBC_STATE_RESERVED_POS (2) 
#define SBC_SBC_STATE_RESERVED_MASK (0x3FFFFFFF) 

static inline uint32_t sbc_ll_get_sbc_state_value(void)
{
    return REG_READ(SBC_SBC_STATE_ADDR);
}

static inline void sbc_ll_set_sbc_state_value(uint32_t value)
{
    REG_WRITE(SBC_SBC_STATE_ADDR,value);
}

/* REG_0x01:sbc_state->sbc_int:0x1[0],sbc interrupt status,0,RW*/
static inline uint32_t sbc_ll_get_sbc_state_sbc_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_SBC_STATE_ADDR);
    reg_value = ((reg_value >> SBC_SBC_STATE_SBC_INT_POS)&SBC_SBC_STATE_SBC_INT_MASK);
    return reg_value;
}

static inline void sbc_ll_set_sbc_state_sbc_int(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_SBC_STATE_ADDR);
    reg_value &= ~(SBC_SBC_STATE_SBC_INT_MASK << SBC_SBC_STATE_SBC_INT_POS);
    reg_value |= ((value & SBC_SBC_STATE_SBC_INT_MASK) << SBC_SBC_STATE_SBC_INT_POS);
    REG_WRITE(SBC_SBC_STATE_ADDR,reg_value);
}


/* REG_0x01:sbc_state->sbc_idle:0x1[1],sbc idle status,0,R*/
static inline uint32_t sbc_ll_get_sbc_state_sbc_idle(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_SBC_STATE_ADDR);
    reg_value = ((reg_value >> SBC_SBC_STATE_SBC_IDLE_POS)&SBC_SBC_STATE_SBC_IDLE_MASK);
    return reg_value;
}

/* REG_0x02 */
#define SBC_RES_BYTEL_ADDR  (SBC_LL_REG_BASE  + 0x2*4) //REG ADDR :0x47010008
#define SBC_RES_BYTEL_BYTE_RESO_POS (0) 
#define SBC_RES_BYTEL_BYTE_RESO_MASK (0xFF) 

#define SBC_RES_BYTEL_RESERVED_POS (8) 
#define SBC_RES_BYTEL_RESERVED_MASK (0xFFFFFF) 

static inline uint32_t sbc_ll_get_res_bytel_value(void)
{
    return REG_READ(SBC_RES_BYTEL_ADDR);
}

static inline void sbc_ll_set_res_bytel_value(uint32_t value)
{
    REG_WRITE(SBC_RES_BYTEL_ADDR,value);
}

/* REG_0x02:res_bytel->byte_reso:0x2[7:0],sbc need to resolve low bit byte,0,RW*/
static inline uint32_t sbc_ll_get_res_bytel_byte_reso(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_RES_BYTEL_ADDR);
    reg_value = ((reg_value >> SBC_RES_BYTEL_BYTE_RESO_POS) & SBC_RES_BYTEL_BYTE_RESO_MASK);
    return reg_value;
}

static inline void sbc_ll_set_res_bytel_byte_reso(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_RES_BYTEL_ADDR);
    reg_value &= ~(SBC_RES_BYTEL_BYTE_RESO_MASK << SBC_RES_BYTEL_BYTE_RESO_POS);
    reg_value |= ((value & SBC_RES_BYTEL_BYTE_RESO_MASK) << SBC_RES_BYTEL_BYTE_RESO_POS);
    REG_WRITE(SBC_RES_BYTEL_ADDR,reg_value);
}

/* REG_0x03 */
#define SBC_RES_BYTEM_ADDR  (SBC_LL_REG_BASE  + 0x3*4) //REG ADDR :0x4701000c
#define SBC_RES_BYTEM_BYTE_RESO_POS (0) 
#define SBC_RES_BYTEM_BYTE_RESO_MASK (0xFF) 

#define SBC_RES_BYTEM_RESERVED_POS (8) 
#define SBC_RES_BYTEM_RESERVED_MASK (0xFFFFFF) 

static inline uint32_t sbc_ll_get_res_bytem_value(void)
{
    return REG_READ(SBC_RES_BYTEM_ADDR);
}

static inline void sbc_ll_set_res_bytem_value(uint32_t value)
{
    REG_WRITE(SBC_RES_BYTEM_ADDR,value);
}

/* REG_0x03:res_bytem->byte_reso:0x3[7:0],sbc need to resolve medium bit byte,0,RW*/
static inline uint32_t sbc_ll_get_res_bytem_byte_reso(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_RES_BYTEM_ADDR);
    reg_value = ((reg_value >> SBC_RES_BYTEM_BYTE_RESO_POS) & SBC_RES_BYTEM_BYTE_RESO_MASK);
    return reg_value;
}

static inline void sbc_ll_set_res_bytem_byte_reso(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_RES_BYTEM_ADDR);
    reg_value &= ~(SBC_RES_BYTEM_BYTE_RESO_MASK << SBC_RES_BYTEM_BYTE_RESO_POS);
    reg_value |= ((value & SBC_RES_BYTEM_BYTE_RESO_MASK) << SBC_RES_BYTEM_BYTE_RESO_POS);
    REG_WRITE(SBC_RES_BYTEM_ADDR,reg_value);
}

/* REG_0x04 */
#define SBC_RES_BYTEH_ADDR  (SBC_LL_REG_BASE  + 0x4*4) //REG ADDR :0x47010010
#define SBC_RES_BYTEH_BYTE_RESO_POS (0) 
#define SBC_RES_BYTEH_BYTE_RESO_MASK (0xFF) 

#define SBC_RES_BYTEH_RESERVED_POS (8) 
#define SBC_RES_BYTEH_RESERVED_MASK (0xFFFFFF) 

static inline uint32_t sbc_ll_get_res_byteh_value(void)
{
    return REG_READ(SBC_RES_BYTEH_ADDR);
}

static inline void sbc_ll_set_res_byteh_value(uint32_t value)
{
    REG_WRITE(SBC_RES_BYTEH_ADDR,value);
}

/* REG_0x04:res_byteh->byte_reso:0x4[7:0],sbc need to resolve high-bit byte,0,RW*/
static inline uint32_t sbc_ll_get_res_byteh_byte_reso(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_RES_BYTEH_ADDR);
    reg_value = ((reg_value >> SBC_RES_BYTEH_BYTE_RESO_POS) & SBC_RES_BYTEH_BYTE_RESO_MASK);
    return reg_value;
}

static inline void sbc_ll_set_res_byteh_byte_reso(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_RES_BYTEH_ADDR);
    reg_value &= ~(SBC_RES_BYTEH_BYTE_RESO_MASK << SBC_RES_BYTEH_BYTE_RESO_POS);
    reg_value |= ((value & SBC_RES_BYTEH_BYTE_RESO_MASK) << SBC_RES_BYTEH_BYTE_RESO_POS);
    REG_WRITE(SBC_RES_BYTEH_ADDR,reg_value);
}

/* REG_0x05 */
#define SBC_BIT_NUM_ADDR  (SBC_LL_REG_BASE  + 0x5*4) //REG ADDR :0x47010014
#define SBC_BIT_NUM_SBC_BITNUM_POS (0) 
#define SBC_BIT_NUM_SBC_BITNUM_MASK (0xFF) 

#define SBC_BIT_NUM_RESERVED_POS (8) 
#define SBC_BIT_NUM_RESERVED_MASK (0xFFFFFF) 

static inline uint32_t sbc_ll_get_bit_num_value(void)
{
    return REG_READ(SBC_BIT_NUM_ADDR);
}

static inline void sbc_ll_set_bit_num_value(uint32_t value)
{
    REG_WRITE(SBC_BIT_NUM_ADDR,value);
}

/* REG_0x05:bit_num->sbc_bitnum:0x5[7:0],sbc bit_num,0,RW*/
static inline uint32_t sbc_ll_get_bit_num_sbc_bitnum(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_BIT_NUM_ADDR);
    reg_value = ((reg_value >> SBC_BIT_NUM_SBC_BITNUM_POS) & SBC_BIT_NUM_SBC_BITNUM_MASK);
    return reg_value;
}

static inline void sbc_ll_set_bit_num_sbc_bitnum(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_BIT_NUM_ADDR);
    reg_value &= ~(SBC_BIT_NUM_SBC_BITNUM_MASK << SBC_BIT_NUM_SBC_BITNUM_POS);
    reg_value |= ((value & SBC_BIT_NUM_SBC_BITNUM_MASK) << SBC_BIT_NUM_SBC_BITNUM_POS);
    REG_WRITE(SBC_BIT_NUM_ADDR,reg_value);
}

/* REG_0x06 */
#define SBC_SF_ADDR  (SBC_LL_REG_BASE  + 0x6*4) //REG ADDR :0x47010018
#define SBC_SF_SBC_SCALE_FACTOR_POS (0) 
#define SBC_SF_SBC_SCALE_FACTOR_MASK (0xFF) 

#define SBC_SF_RESERVED_POS (8) 
#define SBC_SF_RESERVED_MASK (0xFFFFFF) 

static inline uint32_t sbc_ll_get_sf_value(void)
{
    return REG_READ(SBC_SF_ADDR);
}

static inline void sbc_ll_set_sf_value(uint32_t value)
{
    REG_WRITE(SBC_SF_ADDR,value);
}

/* REG_0x06:sf->sbc_scale_factor:0x6[7:0],sbc scale factor,0,RW*/
static inline uint32_t sbc_ll_get_sf_sbc_scale_factor(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_SF_ADDR);
    reg_value = ((reg_value >> SBC_SF_SBC_SCALE_FACTOR_POS) & SBC_SF_SBC_SCALE_FACTOR_MASK);
    return reg_value;
}

static inline void sbc_ll_set_sf_sbc_scale_factor(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_SF_ADDR);
    reg_value &= ~(SBC_SF_SBC_SCALE_FACTOR_MASK << SBC_SF_SBC_SCALE_FACTOR_POS);
    reg_value |= ((value & SBC_SF_SBC_SCALE_FACTOR_MASK) << SBC_SF_SBC_SCALE_FACTOR_POS);
    REG_WRITE(SBC_SF_ADDR,reg_value);
}

/* REG_0x07 */
#define SBC_LEVEL_ADDR  (SBC_LL_REG_BASE  + 0x7*4) //REG ADDR :0x4701001c
#define SBC_LEVEL_SBC_LEVEL_POS (0) 
#define SBC_LEVEL_SBC_LEVEL_MASK (0xFFFFFFFF) 

static inline uint32_t sbc_ll_get_level_value(void)
{
    return REG_READ(SBC_LEVEL_ADDR);
}

static inline void sbc_ll_set_level_value(uint32_t value)
{
    REG_WRITE(SBC_LEVEL_ADDR,value);
}

/* REG_0x07:level->sbc_level:0x7[31: 0],sbc level,0,RW*/
static inline uint32_t sbc_ll_get_level_sbc_level(void)
{
    return REG_READ(SBC_LEVEL_ADDR);
}

static inline void sbc_ll_set_level_sbc_level(uint32_t value)
{
    REG_WRITE(SBC_LEVEL_ADDR,value);
}

/* REG_0x08 */
#define SBC_RES_BIT_ADDR  (SBC_LL_REG_BASE  + 0x8*4) //REG ADDR :0x47010020
#define SBC_RES_BIT_SBC_RES_BIT_POS (0) 
#define SBC_RES_BIT_SBC_RES_BIT_MASK (0x7) 

#define SBC_RES_BIT_RESERVED_POS (3) 
#define SBC_RES_BIT_RESERVED_MASK (0x1FFFFFFF) 

static inline uint32_t sbc_ll_get_res_bit_value(void)
{
    return REG_READ(SBC_RES_BIT_ADDR);
}

static inline void sbc_ll_set_res_bit_value(uint32_t value)
{
    REG_WRITE(SBC_RES_BIT_ADDR,value);
}

/* REG_0x08:res_bit->sbc_res_bit:0x8[2: 0],sbc res bit,0,RW*/
static inline uint32_t sbc_ll_get_res_bit_sbc_res_bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_RES_BIT_ADDR);
    reg_value = ((reg_value >> SBC_RES_BIT_SBC_RES_BIT_POS) & SBC_RES_BIT_SBC_RES_BIT_MASK);
    return reg_value;
}

static inline void sbc_ll_set_res_bit_sbc_res_bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_RES_BIT_ADDR);
    reg_value &= ~(SBC_RES_BIT_SBC_RES_BIT_MASK << SBC_RES_BIT_SBC_RES_BIT_POS);
    reg_value |= ((value & SBC_RES_BIT_SBC_RES_BIT_MASK) << SBC_RES_BIT_SBC_RES_BIT_POS);
    REG_WRITE(SBC_RES_BIT_ADDR,reg_value);
}

/* REG_0x09 */
#define SBC_DEC_EN_ADDR  (SBC_LL_REG_BASE  + 0x9*4) //REG ADDR :0x47010024
#define SBC_DEC_EN_DEC_EN_POS (0) 
#define SBC_DEC_EN_DEC_EN_MASK (0x1) 

#define SBC_DEC_EN_RESERVED_POS (1) 
#define SBC_DEC_EN_RESERVED_MASK (0x7FFFFFFF) 

static inline uint32_t sbc_ll_get_dec_en_value(void)
{
    return REG_READ(SBC_DEC_EN_ADDR);
}

static inline void sbc_ll_set_dec_en_value(uint32_t value)
{
    REG_WRITE(SBC_DEC_EN_ADDR,value);
}

/* REG_0x09:dec_en->dec_en:0x9[0],DEC enable, stop automatically,0,RW*/
static inline uint32_t sbc_ll_get_dec_en_dec_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_DEC_EN_ADDR);
    reg_value = ((reg_value >> SBC_DEC_EN_DEC_EN_POS) & SBC_DEC_EN_DEC_EN_MASK);
    return reg_value;
}

static inline void sbc_ll_set_dec_en_dec_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SBC_DEC_EN_ADDR);
    reg_value &= ~(SBC_DEC_EN_DEC_EN_MASK << SBC_DEC_EN_DEC_EN_POS);
    reg_value |= ((value & SBC_DEC_EN_DEC_EN_MASK) << SBC_DEC_EN_DEC_EN_POS);
    REG_WRITE(SBC_DEC_EN_ADDR,reg_value);
}

/* REG_0x0A */
#define SBC_PCM_DATA_ADDR  (SBC_LL_REG_BASE  + 0xA*4) //REG ADDR :0x47010028
#define SBC_PCM_DATA_PCM_DATA_POS (0) 
#define SBC_PCM_DATA_PCM_DATA_MASK (0xFFFFFFFF) 

static inline uint32_t sbc_ll_get_pcm_data_value(void)
{
    return REG_READ(SBC_PCM_DATA_ADDR);
}

/* REG_0x0a:pcm_data->pcm_data:0xa[31:0],sbc decoded pcm data,None,R*/
static inline uint32_t sbc_ll_get_pcm_data_pcm_data(void)
{
    return REG_READ(SBC_PCM_DATA_ADDR);
}

/* REG_0x100 */
#define SBC_SBC_MEM0_ADDR  (SBC_LL_REG_BASE  + 0x100*4) //REG ADDR :0x47010400
#define SBC_SBC_MEM0_SBC_MEM0_POS (0) 
#define SBC_SBC_MEM0_SBC_MEM0_MASK (0xFFFFFFFF) 

///*write only reg:sbc_mem0:default value:0x0*/
//static inline void sbc_ll_set_sbc_mem0_value(uint32_t value)
//{
//    REG_WRITE(SBC_SBC_MEM0_ADDR,value);
//}

/* REG_0x200 */
#define SBC_SBC_MEM1_ADDR  (SBC_LL_REG_BASE  + 0x200*4) //REG ADDR :0x47010800
#define SBC_SBC_MEM1_SBC_MEM1_POS (0) 
#define SBC_SBC_MEM1_SBC_MEM1_MASK (0xFFFFFFFF) 

///*write only reg:sbc_mem1:default value:0x0*/
//static inline void sbc_ll_set_sbc_mem1_value(uint32_t value)
//{
//    REG_WRITE(SBC_SBC_MEM1_ADDR,value);
//}

#ifdef __cplusplus 
}                  
#endif             
