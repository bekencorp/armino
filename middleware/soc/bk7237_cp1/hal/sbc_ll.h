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
* CHIP ID:BK7256,GENARATE TIME:2022-02-11 14:08:07                                                 
************************************************************************************************************************************/

#pragma once                 
                            
#include <soc/soc.h>          
#include "sbc_hw.h"        
#include "sbc_ll_macro_def.h"         

#ifdef __cplusplus          
extern "C" {              
#endif                      

#if 0

/* REG_0x00 */

static inline uint32_t sbc_ll_get_sbc_ctrl_value(sbc_hw_t *hw)
{
    return hw->sbc_ctrl.v;
}

static inline void sbc_ll_set_sbc_ctrl_value(sbc_hw_t *hw, uint32_t value)
{
    hw->sbc_ctrl.v = value;
}

/* REG_0x00:sbc_ctrl->sbc_en:0x0[0],SBC enable,0,RW*/
static inline uint32_t sbc_ll_get_sbc_ctrl_sbc_en(sbc_hw_t *hw)
{
    return hw->sbc_ctrl.sbc_en;
}

static inline void sbc_ll_set_sbc_ctrl_sbc_en(sbc_hw_t *hw, uint32_t value)
{
    hw->sbc_ctrl.sbc_en = value;
}

/* REG_0x00:sbc_ctrl->sbc_channel:0x0[1],sbc channel：; 0: 1 channel; 1: 2 channel,0,RW*/
static inline uint32_t sbc_ll_get_sbc_ctrl_sbc_channel(sbc_hw_t *hw)
{
    return hw->sbc_ctrl.sbc_channel;
}

static inline void sbc_ll_set_sbc_ctrl_sbc_channel(sbc_hw_t *hw, uint32_t value)
{
    hw->sbc_ctrl.sbc_channel = value;
}

/* REG_0x00:sbc_ctrl->sbc_subband:0x0[2],sbc subband number：; 0：4 subband  ; 1: 8 subband,0,RW*/
static inline uint32_t sbc_ll_get_sbc_ctrl_sbc_subband(sbc_hw_t *hw)
{
    return hw->sbc_ctrl.sbc_subband;
}

static inline void sbc_ll_set_sbc_ctrl_sbc_subband(sbc_hw_t *hw, uint32_t value)
{
    hw->sbc_ctrl.sbc_subband = value;
}

/* REG_0x00:sbc_ctrl->sbc_chn_comb:0x0[3],sbc decode output structure： ; 0：｛16'd0, pcm}; 1: ｛pcm_chn2, pcm_chn1},0,RW*/
static inline uint32_t sbc_ll_get_sbc_ctrl_sbc_chn_comb(sbc_hw_t *hw)
{
    return hw->sbc_ctrl.sbc_chn_comb;
}

static inline void sbc_ll_set_sbc_ctrl_sbc_chn_comb(sbc_hw_t *hw, uint32_t value)
{
    hw->sbc_ctrl.sbc_chn_comb = value;
}

/* REG_0x00:sbc_ctrl->sbc_blocks:0x0[5:4],sbc block number：; 00： 4; 01： 8; 10： 12; 11： 16,0,RW*/
static inline uint32_t sbc_ll_get_sbc_ctrl_sbc_blocks(sbc_hw_t *hw)
{
    return hw->sbc_ctrl.sbc_blocks;
}

static inline void sbc_ll_set_sbc_ctrl_sbc_blocks(sbc_hw_t *hw, uint32_t value)
{
    hw->sbc_ctrl.sbc_blocks = value;
}

/* REG_0x00:sbc_ctrl->msbc_support:0x0[6],support msbc encode,0,RW*/
static inline uint32_t sbc_ll_get_sbc_ctrl_msbc_support(sbc_hw_t *hw)
{
    return hw->sbc_ctrl.msbc_support;
}

static inline void sbc_ll_set_sbc_ctrl_msbc_support(sbc_hw_t *hw, uint32_t value)
{
    hw->sbc_ctrl.msbc_support = value;
}

/* REG_0x01 */

static inline uint32_t sbc_ll_get_sbc_state_value(sbc_hw_t *hw)
{
    return hw->sbc_state.v;
}

/* REG_0x01:sbc_state->sbc_int:0x1[0],sbc interrupt status,0,R*/
static inline uint32_t sbc_ll_get_sbc_state_sbc_int(sbc_hw_t *hw)
{
    return hw->sbc_state.sbc_int;
}

/* REG_0x01:sbc_state->sbc_idle:0x1[1],sbc idle status,0,R*/
static inline uint32_t sbc_ll_get_sbc_state_sbc_idle(sbc_hw_t *hw)
{
    return hw->sbc_state.sbc_idle;
}

/* REG_0x02 */

static inline uint32_t sbc_ll_get_res_bytel_value(sbc_hw_t *hw)
{
    return hw->res_bytel.v;
}

static inline void sbc_ll_set_res_bytel_value(sbc_hw_t *hw, uint32_t value)
{
    hw->res_bytel.v = value;
}

/* REG_0x02:res_bytel->byte_reso:0x2[7:0],sbc need to resolve low bit byte,0,RW*/
static inline uint32_t sbc_ll_get_res_bytel_byte_reso(sbc_hw_t *hw)
{
    return hw->res_bytel.byte_reso;
}

static inline void sbc_ll_set_res_bytel_byte_reso(sbc_hw_t *hw, uint32_t value)
{
    hw->res_bytel.byte_reso = value;
}

/* REG_0x03 */

static inline uint32_t sbc_ll_get_res_bytem_value(sbc_hw_t *hw)
{
    return hw->res_bytem.v;
}

static inline void sbc_ll_set_res_bytem_value(sbc_hw_t *hw, uint32_t value)
{
    hw->res_bytem.v = value;
}

/* REG_0x03:res_bytem->byte_reso:0x3[7:0],sbc need to resolve medium bit byte,0,RW*/
static inline uint32_t sbc_ll_get_res_bytem_byte_reso(sbc_hw_t *hw)
{
    return hw->res_bytem.byte_reso;
}

static inline void sbc_ll_set_res_bytem_byte_reso(sbc_hw_t *hw, uint32_t value)
{
    hw->res_bytem.byte_reso = value;
}

/* REG_0x04 */

static inline uint32_t sbc_ll_get_res_byteh_value(sbc_hw_t *hw)
{
    return hw->res_byteh.v;
}

static inline void sbc_ll_set_res_byteh_value(sbc_hw_t *hw, uint32_t value)
{
    hw->res_byteh.v = value;
}

/* REG_0x04:res_byteh->byte_reso:0x4[7:0],sbc need to resolve high-bit byte,0,RW*/
static inline uint32_t sbc_ll_get_res_byteh_byte_reso(sbc_hw_t *hw)
{
    return hw->res_byteh.byte_reso;
}

static inline void sbc_ll_set_res_byteh_byte_reso(sbc_hw_t *hw, uint32_t value)
{
    hw->res_byteh.byte_reso = value;
}

/* REG_0x05 */

static inline uint32_t sbc_ll_get_bit_num_value(sbc_hw_t *hw)
{
    return hw->bit_num.v;
}

static inline void sbc_ll_set_bit_num_value(sbc_hw_t *hw, uint32_t value)
{
    hw->bit_num.v = value;
}

/* REG_0x05:bit_num->sbc_bitnum:0x5[7:0],sbc bit_num,0,RW*/
static inline uint32_t sbc_ll_get_bit_num_sbc_bitnum(sbc_hw_t *hw)
{
    return hw->bit_num.sbc_bitnum;
}

static inline void sbc_ll_set_bit_num_sbc_bitnum(sbc_hw_t *hw, uint32_t value)
{
    hw->bit_num.sbc_bitnum = value;
}

/* REG_0x06 */

static inline uint32_t sbc_ll_get_sf_value(sbc_hw_t *hw)
{
    return hw->sf.v;
}

static inline void sbc_ll_set_sf_value(sbc_hw_t *hw, uint32_t value)
{
    hw->sf.v = value;
}

/* REG_0x06:sf->sbc_scale_factor:0x6[7:0],sbc scale factor,0,RW*/
static inline uint32_t sbc_ll_get_sf_sbc_scale_factor(sbc_hw_t *hw)
{
    return hw->sf.sbc_scale_factor;
}

static inline void sbc_ll_set_sf_sbc_scale_factor(sbc_hw_t *hw, uint32_t value)
{
    hw->sf.sbc_scale_factor = value;
}

/* REG_0x07 */

static inline uint32_t sbc_ll_get_level_value(sbc_hw_t *hw)
{
    return hw->level.v;
}

static inline void sbc_ll_set_level_value(sbc_hw_t *hw, uint32_t value)
{
    hw->level.v = value;
}

/* REG_0x07:level->sbc_level:0x7[31: 0],sbc level,0,RW*/
static inline uint32_t sbc_ll_get_level_sbc_level(sbc_hw_t *hw)
{
    return hw->level.v;
}

static inline void sbc_ll_set_level_sbc_level(sbc_hw_t *hw, uint32_t value)
{
    hw->level.v = value;
}

/* REG_0x08 */

static inline uint32_t sbc_ll_get_res_bit_value(sbc_hw_t *hw)
{
    return hw->res_bit.v;
}

static inline void sbc_ll_set_res_bit_value(sbc_hw_t *hw, uint32_t value)
{
    hw->res_bit.v = value;
}

/* REG_0x08:res_bit->sbc_res_bit:0x8[2: 0],sbc res bit,0,RW*/
static inline uint32_t sbc_ll_get_res_bit_sbc_res_bit(sbc_hw_t *hw)
{
    return hw->res_bit.sbc_res_bit;
}

static inline void sbc_ll_set_res_bit_sbc_res_bit(sbc_hw_t *hw, uint32_t value)
{
    hw->res_bit.sbc_res_bit = value;
}

/* REG_0x09 */

static inline uint32_t sbc_ll_get_dec_en_value(sbc_hw_t *hw)
{
    return hw->dec_en.v;
}

static inline void sbc_ll_set_dec_en_value(sbc_hw_t *hw, uint32_t value)
{
    hw->dec_en.v = value;
}

/* REG_0x09:dec_en->dec_en:0x9[0],DEC enable, stop automatically,0,RW*/
static inline uint32_t sbc_ll_get_dec_en_dec_en(sbc_hw_t *hw)
{
    return hw->dec_en.dec_en;
}

static inline void sbc_ll_set_dec_en_dec_en(sbc_hw_t *hw, uint32_t value)
{
    hw->dec_en.dec_en = value;
}

/* REG_0x0a */

static inline uint32_t sbc_ll_get_pcm_data_value(sbc_hw_t *hw)
{
    return hw->pcm_data.v;
}

/* REG_0x0a:pcm_data->pcm_data:0xa[31:0],sbc decoded pcm data,None,R*/
static inline uint32_t sbc_ll_get_pcm_data_pcm_data(sbc_hw_t *hw)
{
    return hw->pcm_data.v;
}

/* REG_0x100 */

static inline void sbc_ll_set_sbc_mem0_value(sbc_hw_t *hw, uint32_t value)
{
    hw->sbc_mem0.v = value;
}

/* REG_0x100:sbc_mem0->sbc_mem0:0x100,length 256,None,W*/
static inline void sbc_ll_set_sbc_mem0_sbc_mem0(sbc_hw_t *hw, uint32_t value)
{
    hw->sbc_mem0.v = value;
}

/* REG_0x200 */

static inline void sbc_ll_set_sbc_mem1_value(sbc_hw_t *hw, uint32_t value)
{
    hw->sbc_mem1.v = value;
}

/* REG_0x200:sbc_mem1->sbc_mem1:0x200,length 320,None,W*/
static inline void sbc_ll_set_sbc_mem1_sbc_mem1(sbc_hw_t *hw, uint32_t value)
{
    hw->sbc_mem1.v = value;
}

#endif 

#ifdef __cplusplus 
}                  
#endif             
