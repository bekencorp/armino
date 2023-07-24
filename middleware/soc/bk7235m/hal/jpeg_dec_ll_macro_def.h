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
* This file is generated from BK7256_ADDR_Mapping_MP2.xlsm automatically                                
* Modify it manually is not recommended                                       
* CHIP ID:BK7256,GENARATE TIME:2022-07-16 15:30:25                                                 
************************************************************************************************************************************/

#pragma once                 
                            
#ifdef __cplusplus          
extern "C" {              
#endif                      

#include <soc/soc.h>                 

#define JPEG_DEC_LL_REG_BASE      (SOC_JPEG_DEC_REG_BASE) //REG_BASE:0x48040000

/* REG_0x00 */
#define JPEG_DEC_REG0X0_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x0*4) //REG ADDR :0x48040000
#define JPEG_DEC_REG0X0_JPEG_DEC_EN_POS (0) 
#define JPEG_DEC_REG0X0_JPEG_DEC_EN_MASK (0x3) 

#define JPEG_DEC_REG0X0_RESERVED_POS (2) 
#define JPEG_DEC_REG0X0_RESERVED_MASK (0x7FFFFFF) 

#define JPEG_DEC_REG0X0_DRI_BPS_POS (29) 
#define JPEG_DEC_REG0X0_DRI_BPS_MASK (0x1) 

#define JPEG_DEC_REG0X0_YY_VLD_BPS_POS (30) 
#define JPEG_DEC_REG0X0_YY_VLD_BPS_MASK (0x1) 

#define JPEG_DEC_REG0X0_SHARMEM_BPS_POS (31) 
#define JPEG_DEC_REG0X0_SHARMEM_BPS_MASK (0x1) 

static inline uint32_t jpeg_dec_ll_get_reg0x0_value(void)
{
    return REG_READ(JPEG_DEC_REG0X0_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x0_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X0_ADDR,value);
}

/* REG_0x00:reg0x0->jpeg_dec_en:0x0[1:0],jpeg dec enable,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x0_jpeg_dec_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X0_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X0_JPEG_DEC_EN_POS) & JPEG_DEC_REG0X0_JPEG_DEC_EN_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x0_jpeg_dec_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X0_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X0_JPEG_DEC_EN_MASK << JPEG_DEC_REG0X0_JPEG_DEC_EN_POS);
    reg_value |= ((value & JPEG_DEC_REG0X0_JPEG_DEC_EN_MASK) << JPEG_DEC_REG0X0_JPEG_DEC_EN_POS);
    REG_WRITE(JPEG_DEC_REG0X0_ADDR,reg_value);
}

/* REG_0x00:reg0x0->dri_bps:0x0[29],set 0,bypass dri support,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x0_dri_bps(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X0_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X0_DRI_BPS_POS) & JPEG_DEC_REG0X0_DRI_BPS_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x0_dri_bps(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X0_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X0_DRI_BPS_MASK << JPEG_DEC_REG0X0_DRI_BPS_POS);
    reg_value |= ((value & JPEG_DEC_REG0X0_DRI_BPS_MASK) << JPEG_DEC_REG0X0_DRI_BPS_POS);
    REG_WRITE(JPEG_DEC_REG0X0_ADDR,reg_value);
}

/* REG_0x00:reg0x0->yy_vld_bps:0x0[30],set 0,used for debug,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x0_yy_vld_bps(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X0_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X0_YY_VLD_BPS_POS) & JPEG_DEC_REG0X0_YY_VLD_BPS_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x0_yy_vld_bps(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X0_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X0_YY_VLD_BPS_MASK << JPEG_DEC_REG0X0_YY_VLD_BPS_POS);
    reg_value |= ((value & JPEG_DEC_REG0X0_YY_VLD_BPS_MASK) << JPEG_DEC_REG0X0_YY_VLD_BPS_POS);
    REG_WRITE(JPEG_DEC_REG0X0_ADDR,reg_value);
}

/* REG_0x00:reg0x0->sharmem_bps:0x0[31],set 0,used for debug,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x0_sharmem_bps(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X0_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X0_SHARMEM_BPS_POS) & JPEG_DEC_REG0X0_SHARMEM_BPS_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x0_sharmem_bps(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X0_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X0_SHARMEM_BPS_MASK << JPEG_DEC_REG0X0_SHARMEM_BPS_POS);
    reg_value |= ((value & JPEG_DEC_REG0X0_SHARMEM_BPS_MASK) << JPEG_DEC_REG0X0_SHARMEM_BPS_POS);
    REG_WRITE(JPEG_DEC_REG0X0_ADDR,reg_value);
}

/* REG_0x01 */
#define JPEG_DEC_REG0X1_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x1*4) //REG ADDR :0x48040004
#define JPEG_DEC_REG0X1_MCU_INDEX_POS (0) 
#define JPEG_DEC_REG0X1_MCU_INDEX_MASK (0xFFFF) 

#define JPEG_DEC_REG0X1_MCU_Y_NUM_POS (16) 
#define JPEG_DEC_REG0X1_MCU_Y_NUM_MASK (0x3FF) 

#define JPEG_DEC_REG0X1_RESERVED_POS (26) 
#define JPEG_DEC_REG0X1_RESERVED_MASK (0x3F) 

static inline uint32_t jpeg_dec_ll_get_reg0x1_value(void)
{
    return REG_READ(JPEG_DEC_REG0X1_ADDR);
}



/* REG_0x01:reg0x1->mcu_index:0x1[15:0],mcu index,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x1_mcu_index(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X1_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X1_MCU_INDEX_POS) & JPEG_DEC_REG0X1_MCU_INDEX_MASK);
    return reg_value;
}


/* REG_0x01:reg0x1->mcu_y_num:0x1[25:16],line number,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x1_mcu_y_num(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X1_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X1_MCU_Y_NUM_POS) & JPEG_DEC_REG0X1_MCU_Y_NUM_MASK);
    return reg_value;
}


/* REG_0x02 */
#define JPEG_DEC_REG0X2_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x2*4) //REG ADDR :0x48040008
#define JPEG_DEC_REG0X2_JPEG_DEC_AUTO_POS (0) 
#define JPEG_DEC_REG0X2_JPEG_DEC_AUTO_MASK (0x1) 

#define JPEG_DEC_REG0X2_JPEG_DEC_LINEN_POS (1) 
#define JPEG_DEC_REG0X2_JPEG_DEC_LINEN_MASK (0x1) 

#define JPEG_DEC_REG0X2_RESERVED2_POS (2) 
#define JPEG_DEC_REG0X2_RESERVED2_MASK (0x3F) 

#define JPEG_DEC_REG0X2_JPEG_LINE_NUM_POS (8) 
#define JPEG_DEC_REG0X2_JPEG_LINE_NUM_MASK (0x1FFF) 

#define JPEG_DEC_REG0X2_RESERVED_POS (21) 
#define JPEG_DEC_REG0X2_RESERVED_MASK (0x7FF) 

static inline uint32_t jpeg_dec_ll_get_reg0x2_value(void)
{
    return REG_READ(JPEG_DEC_REG0X2_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x2_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X2_ADDR,value);
}

/* REG_0x02:reg0x2->jpeg_dec_auto:0x2[0],auto jpeg dec,total decode,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x2_jpeg_dec_auto(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X2_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X2_JPEG_DEC_AUTO_POS) & JPEG_DEC_REG0X2_JPEG_DEC_AUTO_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x2_jpeg_dec_auto(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X2_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X2_JPEG_DEC_AUTO_MASK << JPEG_DEC_REG0X2_JPEG_DEC_AUTO_POS);
    reg_value |= ((value & JPEG_DEC_REG0X2_JPEG_DEC_AUTO_MASK) << JPEG_DEC_REG0X2_JPEG_DEC_AUTO_POS);
    REG_WRITE(JPEG_DEC_REG0X2_ADDR,reg_value);
}

/* REG_0x02:reg0x2->jpeg_dec_linen:0x2[1],auto jpeg dec,line decode,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x2_jpeg_dec_linen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X2_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X2_JPEG_DEC_LINEN_POS) & JPEG_DEC_REG0X2_JPEG_DEC_LINEN_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x2_jpeg_dec_linen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X2_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X2_JPEG_DEC_LINEN_MASK << JPEG_DEC_REG0X2_JPEG_DEC_LINEN_POS);
    reg_value |= ((value & JPEG_DEC_REG0X2_JPEG_DEC_LINEN_MASK) << JPEG_DEC_REG0X2_JPEG_DEC_LINEN_POS);
    REG_WRITE(JPEG_DEC_REG0X2_ADDR,reg_value);
}

/* REG_0x02:reg0x2->jpeg_line_num:0x2[20:8],when line decode, when(line==jpeg_line_num),generate interrupt,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x2_jpeg_line_num(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X2_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X2_JPEG_LINE_NUM_POS) & JPEG_DEC_REG0X2_JPEG_LINE_NUM_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x2_jpeg_line_num(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X2_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X2_JPEG_LINE_NUM_MASK << JPEG_DEC_REG0X2_JPEG_LINE_NUM_POS);
    reg_value |= ((value & JPEG_DEC_REG0X2_JPEG_LINE_NUM_MASK) << JPEG_DEC_REG0X2_JPEG_LINE_NUM_POS);
    REG_WRITE(JPEG_DEC_REG0X2_ADDR,reg_value);
}

/* REG_0x03 */
#define JPEG_DEC_REG0X3_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x3*4) //REG ADDR :0x4804000c
#define JPEG_DEC_REG0X3_UV_VLD_POS (0) 
#define JPEG_DEC_REG0X3_UV_VLD_MASK (0x1) 

#define JPEG_DEC_REG0X3_RESERVED_POS (1) 
#define JPEG_DEC_REG0X3_RESERVED_MASK (0x7FFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x3_value(void)
{
    return REG_READ(JPEG_DEC_REG0X3_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x3_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X3_ADDR,value);
}

/* REG_0x03:reg0x3->uv_vld:0x3[0],uv  valid,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x3_uv_vld(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X3_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X3_UV_VLD_POS) & JPEG_DEC_REG0X3_UV_VLD_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x3_uv_vld(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X3_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X3_UV_VLD_MASK << JPEG_DEC_REG0X3_UV_VLD_POS);
    reg_value |= ((value & JPEG_DEC_REG0X3_UV_VLD_MASK) << JPEG_DEC_REG0X3_UV_VLD_POS);
    REG_WRITE(JPEG_DEC_REG0X3_ADDR,reg_value);
}

/* REG_0x04 */
#define JPEG_DEC_REG0X4_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x4*4) //REG ADDR :0x48040010
#define JPEG_DEC_REG0X4_JD_MASK_CUR_POS (0) 
#define JPEG_DEC_REG0X4_JD_MASK_CUR_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x4_value(void)
{
    return REG_READ(JPEG_DEC_REG0X4_ADDR);
}

/* REG_0x04:reg0x4->jd_mask_cur:0x4[31:0],current mask of one byte,0x0,R*/
static inline uint32_t jpeg_dec_ll_get_reg0x4_jd_mask_cur(void)
{
    return REG_READ(JPEG_DEC_REG0X4_ADDR);
}

/* REG_0x05 */
#define JPEG_DEC_REG0X5_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x5*4) //REG ADDR :0x48040014
#define JPEG_DEC_REG0X5_MCU_X_POS (0) 
#define JPEG_DEC_REG0X5_MCU_X_MASK (0xFFFF) 

#define JPEG_DEC_REG0X5_RESERVED_POS (16) 
#define JPEG_DEC_REG0X5_RESERVED_MASK (0xFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x5_value(void)
{
    return REG_READ(JPEG_DEC_REG0X5_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x5_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X5_ADDR,value);
}

/* REG_0x05:reg0x5->mcu_x:0x5[15:0],mcu_x  index,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5_mcu_x(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X5_MCU_X_POS) & JPEG_DEC_REG0X5_MCU_X_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x5_mcu_x(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X5_MCU_X_MASK << JPEG_DEC_REG0X5_MCU_X_POS);
    reg_value |= ((value & JPEG_DEC_REG0X5_MCU_X_MASK) << JPEG_DEC_REG0X5_MCU_X_POS);
    REG_WRITE(JPEG_DEC_REG0X5_ADDR,reg_value);
}

/* REG_0x06 */
#define JPEG_DEC_REG0X6_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x6*4) //REG ADDR :0x48040018
#define JPEG_DEC_REG0X6_MCU_Y_POS (0) 
#define JPEG_DEC_REG0X6_MCU_Y_MASK (0xFFFF) 

#define JPEG_DEC_REG0X6_RESERVED_POS (16) 
#define JPEG_DEC_REG0X6_RESERVED_MASK (0xFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x6_value(void)
{
    return REG_READ(JPEG_DEC_REG0X6_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x6_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X6_ADDR,value);
}

/* REG_0x06:reg0x6->mcu_y:0x6[15:0],mcu_y  index,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x6_mcu_y(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X6_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X6_MCU_Y_POS) & JPEG_DEC_REG0X6_MCU_Y_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x6_mcu_y(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X6_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X6_MCU_Y_MASK << JPEG_DEC_REG0X6_MCU_Y_POS);
    reg_value |= ((value & JPEG_DEC_REG0X6_MCU_Y_MASK) << JPEG_DEC_REG0X6_MCU_Y_POS);
    REG_WRITE(JPEG_DEC_REG0X6_ADDR,reg_value);
}

/* REG_0x07 */
#define JPEG_DEC_REG0X7_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x7*4) //REG ADDR :0x4804001c
#define JPEG_DEC_REG0X7_V_VLD_NUM_POS (0) 
#define JPEG_DEC_REG0X7_V_VLD_NUM_MASK (0xF) 

#define JPEG_DEC_REG0X7_RESERVED_POS (4) 
#define JPEG_DEC_REG0X7_RESERVED_MASK (0xFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x7_value(void)
{
    return REG_READ(JPEG_DEC_REG0X7_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x7_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X7_ADDR,value);
}

/* REG_0x07:reg0x7->v_vld_num:0x7[3:0],v valid number, default: 1  U:V ,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x7_v_vld_num(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X7_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X7_V_VLD_NUM_POS) & JPEG_DEC_REG0X7_V_VLD_NUM_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x7_v_vld_num(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X7_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X7_V_VLD_NUM_MASK << JPEG_DEC_REG0X7_V_VLD_NUM_POS);
    reg_value |= ((value & JPEG_DEC_REG0X7_V_VLD_NUM_MASK) << JPEG_DEC_REG0X7_V_VLD_NUM_POS);
    REG_WRITE(JPEG_DEC_REG0X7_ADDR,reg_value);
}

/* REG_0x08 */
#define JPEG_DEC_REG0X8_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x8*4) //REG ADDR :0x48040020
#define JPEG_DEC_REG0X8_DEC_CMD_POS (0) 
#define JPEG_DEC_REG0X8_DEC_CMD_MASK (0xF) 

#define JPEG_DEC_REG0X8_RESERVED_POS (4) 
#define JPEG_DEC_REG0X8_RESERVED_MASK (0xFFFFFFF) 

static inline void jpeg_dec_ll_set_reg0x8_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X8_ADDR,value);
}

/* REG_0x08:reg0x8->DEC_CMD:0x8[3:0],4'b0001:  dec_start; 4'b0100:  dc value clr,0x0,W*/
static inline void jpeg_dec_ll_set_reg0x8_dec_cmd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X8_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X8_DEC_CMD_MASK << JPEG_DEC_REG0X8_DEC_CMD_POS);
    reg_value |= ((value & JPEG_DEC_REG0X8_DEC_CMD_MASK) << JPEG_DEC_REG0X8_DEC_CMD_POS);
    REG_WRITE(JPEG_DEC_REG0X8_ADDR,reg_value);
}

/* REG_0x09 */
#define JPEG_DEC_REG0X9_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x9*4) //REG ADDR :0x48040024
#define JPEG_DEC_REG0X9_CUR_RUN_POS (0) 
#define JPEG_DEC_REG0X9_CUR_RUN_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x9_value(void)
{
    return REG_READ(JPEG_DEC_REG0X9_ADDR);
}

/* REG_0x09:reg0x9->cur_run:0x9[31:0],current run,0x0,R*/
static inline uint32_t jpeg_dec_ll_get_reg0x9_cur_run(void)
{
    return REG_READ(JPEG_DEC_REG0X9_ADDR);
}

/* REG_0x0A */
#define JPEG_DEC_REG0XA_ADDR  (JPEG_DEC_LL_REG_BASE  + 0xA*4) //REG ADDR :0x48040028
#define JPEG_DEC_REG0XA_X_PIXEL_POS (0) 
#define JPEG_DEC_REG0XA_X_PIXEL_MASK (0xFFFF) 

#define JPEG_DEC_REG0XA_RESERVED_POS (16) 
#define JPEG_DEC_REG0XA_RESERVED_MASK (0xFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0xa_value(void)
{
    return REG_READ(JPEG_DEC_REG0XA_ADDR);
}

static inline void jpeg_dec_ll_set_reg0xa_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0XA_ADDR,value);
}

/* REG_0x0a:reg0xa->x_pixel:0xa[15:0],pixel number X,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0xa_x_pixel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0XA_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0XA_X_PIXEL_POS) & JPEG_DEC_REG0XA_X_PIXEL_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0xa_x_pixel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0XA_ADDR);
    reg_value &= ~(JPEG_DEC_REG0XA_X_PIXEL_MASK << JPEG_DEC_REG0XA_X_PIXEL_POS);
    reg_value |= ((value & JPEG_DEC_REG0XA_X_PIXEL_MASK) << JPEG_DEC_REG0XA_X_PIXEL_POS);
    REG_WRITE(JPEG_DEC_REG0XA_ADDR,reg_value);
}

/* REG_0x0B */
#define JPEG_DEC_REG0XB_ADDR  (JPEG_DEC_LL_REG_BASE  + 0xB*4) //REG ADDR :0x4804002c
#define JPEG_DEC_REG0XB_HUF_DATA_POS (0) 
#define JPEG_DEC_REG0XB_HUF_DATA_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0xb_value(void)
{
    return REG_READ(JPEG_DEC_REG0XB_ADDR);
}

/* REG_0x0b:reg0xb->huf_data:0xb[31:0],huffman data,0x0,R*/
static inline uint32_t jpeg_dec_ll_get_reg0xb_huf_data(void)
{
    return REG_READ(JPEG_DEC_REG0XB_ADDR);
}

/* REG_0x0C */
#define JPEG_DEC_REG0XC_ADDR  (JPEG_DEC_LL_REG_BASE  + 0xC*4) //REG ADDR :0x48040030
#define JPEG_DEC_REG0XC_CUR_NBIT_POS (0) 
#define JPEG_DEC_REG0XC_CUR_NBIT_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0xc_value(void)
{
    return REG_READ(JPEG_DEC_REG0XC_ADDR);
}

/* REG_0x0c:reg0xc->cur_nbit:0xc[31:0],current nbit,0x0,R*/
static inline uint32_t jpeg_dec_ll_get_reg0xc_cur_nbit(void)
{
    return REG_READ(JPEG_DEC_REG0XC_ADDR);
}

/* REG_0x0D */
#define JPEG_DEC_REG0XD_ADDR  (JPEG_DEC_LL_REG_BASE  + 0xD*4) //REG ADDR :0x48040034
#define JPEG_DEC_REG0XD_STATE_DEC_BUSY_POS (0) 
#define JPEG_DEC_REG0XD_STATE_DEC_BUSY_MASK (0x1) 

#define JPEG_DEC_REG0XD_STATE_RRLOAD_POS (1) 
#define JPEG_DEC_REG0XD_STATE_RRLOAD_MASK (0x1) 

#define JPEG_DEC_REG0XD_STATE_RLOAD_POS (2) 
#define JPEG_DEC_REG0XD_STATE_RLOAD_MASK (0x1) 

#define JPEG_DEC_REG0XD_STATE_SEARCH_POS (3) 
#define JPEG_DEC_REG0XD_STATE_SEARCH_MASK (0x1) 

#define JPEG_DEC_REG0XD_STATE_EXT_RLOAD_POS (4) 
#define JPEG_DEC_REG0XD_STATE_EXT_RLOAD_MASK (0x1) 

#define JPEG_DEC_REG0XD_STATE_EXT_BITS_POS (5) 
#define JPEG_DEC_REG0XD_STATE_EXT_BITS_MASK (0x1) 

#define JPEG_DEC_REG0XD_STATE_IDCT_POS (6) 
#define JPEG_DEC_REG0XD_STATE_IDCT_MASK (0x1) 

#define JPEG_DEC_REG0XD_TMP_WR0_POS (7) 
#define JPEG_DEC_REG0XD_TMP_WR0_MASK (0x1) 

#define JPEG_DEC_REG0XD_STATE_DEC_BUSY2_POS (8) 
#define JPEG_DEC_REG0XD_STATE_DEC_BUSY2_MASK (0x1) 

#define JPEG_DEC_REG0XD_STATE_AHBMST_IDLE_POS (9) 
#define JPEG_DEC_REG0XD_STATE_AHBMST_IDLE_MASK (0x1) 

#define JPEG_DEC_REG0XD_RESERVED_POS (10) 
#define JPEG_DEC_REG0XD_RESERVED_MASK (0x3FFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0xd_value(void)
{
    return REG_READ(JPEG_DEC_REG0XD_ADDR);
}

/* REG_0x0d:reg0xd->state_dec_busy:0xd[0],state_dec_busy,None,R*/
static inline uint32_t jpeg_dec_ll_get_reg0xd_state_dec_busy(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0XD_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0XD_STATE_DEC_BUSY_POS)&JPEG_DEC_REG0XD_STATE_DEC_BUSY_MASK);
    return reg_value;
}

/* REG_0x0d:reg0xd->state_rrload:0xd[1],state_rrload,None,R*/
static inline uint32_t jpeg_dec_ll_get_reg0xd_state_rrload(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0XD_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0XD_STATE_RRLOAD_POS)&JPEG_DEC_REG0XD_STATE_RRLOAD_MASK);
    return reg_value;
}

/* REG_0x0d:reg0xd->state_rload:0xd[2],state_rload,None,R*/
static inline uint32_t jpeg_dec_ll_get_reg0xd_state_rload(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0XD_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0XD_STATE_RLOAD_POS)&JPEG_DEC_REG0XD_STATE_RLOAD_MASK);
    return reg_value;
}

/* REG_0x0d:reg0xd->state_search:0xd[3],state_search,None,R*/
static inline uint32_t jpeg_dec_ll_get_reg0xd_state_search(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0XD_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0XD_STATE_SEARCH_POS)&JPEG_DEC_REG0XD_STATE_SEARCH_MASK);
    return reg_value;
}

/* REG_0x0d:reg0xd->state_ext_rload:0xd[4],state_ext_rload,None,R*/
static inline uint32_t jpeg_dec_ll_get_reg0xd_state_ext_rload(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0XD_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0XD_STATE_EXT_RLOAD_POS)&JPEG_DEC_REG0XD_STATE_EXT_RLOAD_MASK);
    return reg_value;
}

/* REG_0x0d:reg0xd->state_ext_bits:0xd[5],state_ext_bits,None,R*/
static inline uint32_t jpeg_dec_ll_get_reg0xd_state_ext_bits(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0XD_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0XD_STATE_EXT_BITS_POS)&JPEG_DEC_REG0XD_STATE_EXT_BITS_MASK);
    return reg_value;
}

/* REG_0x0d:reg0xd->state_idct:0xd[6],state_idct,None,R*/
static inline uint32_t jpeg_dec_ll_get_reg0xd_state_idct(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0XD_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0XD_STATE_IDCT_POS)&JPEG_DEC_REG0XD_STATE_IDCT_MASK);
    return reg_value;
}

/* REG_0x0d:reg0xd->tmp_wr0:0xd[7],tmp_wr0,None,R*/
static inline uint32_t jpeg_dec_ll_get_reg0xd_tmp_wr0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0XD_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0XD_TMP_WR0_POS)&JPEG_DEC_REG0XD_TMP_WR0_MASK);
    return reg_value;
}

/* REG_0x0d:reg0xd->state_dec_busy2:0xd[8],state_dec_busy2,None,R*/
static inline uint32_t jpeg_dec_ll_get_reg0xd_state_dec_busy2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0XD_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0XD_STATE_DEC_BUSY2_POS)&JPEG_DEC_REG0XD_STATE_DEC_BUSY2_MASK);
    return reg_value;
}

/* REG_0x0d:reg0xd->state_ahbmst_idle:0xd[9],state_ahbmst_idle,None,R*/
static inline uint32_t jpeg_dec_ll_get_reg0xd_state_ahbmst_idle(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0XD_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0XD_STATE_AHBMST_IDLE_POS)&JPEG_DEC_REG0XD_STATE_AHBMST_IDLE_MASK);
    return reg_value;
}

/* REG_0x0E */
#define JPEG_DEC_REG0XE_ADDR  (JPEG_DEC_LL_REG_BASE  + 0xE*4) //REG ADDR :0x48040038
#define JPEG_DEC_REG0XE_EXT_DATA_POS (0) 
#define JPEG_DEC_REG0XE_EXT_DATA_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0xe_value(void)
{
    return REG_READ(JPEG_DEC_REG0XE_ADDR);
}

/* REG_0x0e:reg0xe->ext_data:0xe[31:0],ext data,0x0,R*/
static inline uint32_t jpeg_dec_ll_get_reg0xe_ext_data(void)
{
    return REG_READ(JPEG_DEC_REG0XE_ADDR);
}

/* REG_0x0F */
#define JPEG_DEC_REG0XF_ADDR  (JPEG_DEC_LL_REG_BASE  + 0xF*4) //REG ADDR :0x4804003c
#define JPEG_DEC_REG0XF_MCU_BLK_POS (0) 
#define JPEG_DEC_REG0XF_MCU_BLK_MASK (0xFF) 

#define JPEG_DEC_REG0XF_RESERVED_POS (8) 
#define JPEG_DEC_REG0XF_RESERVED_MASK (0xFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0xf_value(void)
{
    return REG_READ(JPEG_DEC_REG0XF_ADDR);
}

static inline void jpeg_dec_ll_set_reg0xf_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0XF_ADDR,value);
}

/* REG_0x0f:reg0xf->mcu_blk:0xf[7:0],block number of one mcu,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0xf_mcu_blk(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0XF_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0XF_MCU_BLK_POS) & JPEG_DEC_REG0XF_MCU_BLK_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0xf_mcu_blk(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0XF_ADDR);
    reg_value &= ~(JPEG_DEC_REG0XF_MCU_BLK_MASK << JPEG_DEC_REG0XF_MCU_BLK_POS);
    reg_value |= ((value & JPEG_DEC_REG0XF_MCU_BLK_MASK) << JPEG_DEC_REG0XF_MCU_BLK_POS);
    REG_WRITE(JPEG_DEC_REG0XF_ADDR,reg_value);
}

/* REG_0x10 */
#define JPEG_DEC_REG0X10_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x10*4) //REG ADDR :0x48040040
#define JPEG_DEC_REG0X10_HUFDC_LEN_Y0_POS (0) 
#define JPEG_DEC_REG0X10_HUFDC_LEN_Y0_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x10_value(void)
{
    return REG_READ(JPEG_DEC_REG0X10_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x10_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X10_ADDR,value);
}

/* REG_0x10:reg0x10->hufdc_len_y0:0x10[31:0],y huffman table dc0,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x10_hufdc_len_y0(void)
{
    return REG_READ(JPEG_DEC_REG0X10_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x10_hufdc_len_y0(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X10_ADDR,value);
}

/* REG_0x11 */
#define JPEG_DEC_REG0X11_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x11*4) //REG ADDR :0x48040044
#define JPEG_DEC_REG0X11_HUFDC_LEN_Y1_POS (0) 
#define JPEG_DEC_REG0X11_HUFDC_LEN_Y1_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x11_value(void)
{
    return REG_READ(JPEG_DEC_REG0X11_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x11_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X11_ADDR,value);
}

/* REG_0x11:reg0x11->hufdc_len_y1:0x11,y huffman table dc1,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x11_hufdc_len_y1(void)
{
    return REG_READ(JPEG_DEC_REG0X11_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x11_hufdc_len_y1(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X11_ADDR,value);
}

/* REG_0x12 */
#define JPEG_DEC_REG0X12_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x12*4) //REG ADDR :0x48040048
#define JPEG_DEC_REG0X12_HUFDC_LEN_Y2_POS (0) 
#define JPEG_DEC_REG0X12_HUFDC_LEN_Y2_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x12_value(void)
{
    return REG_READ(JPEG_DEC_REG0X12_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x12_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X12_ADDR,value);
}

/* REG_0x12:reg0x12->hufdc_len_y2:0x12,y huffman table dc2,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x12_hufdc_len_y2(void)
{
    return REG_READ(JPEG_DEC_REG0X12_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x12_hufdc_len_y2(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X12_ADDR,value);
}

/* REG_0x13 */
#define JPEG_DEC_REG0X13_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x13*4) //REG ADDR :0x4804004c
#define JPEG_DEC_REG0X13_HUFDC_LEN_Y3_POS (0) 
#define JPEG_DEC_REG0X13_HUFDC_LEN_Y3_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x13_value(void)
{
    return REG_READ(JPEG_DEC_REG0X13_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x13_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X13_ADDR,value);
}

/* REG_0x13:reg0x13->hufdc_len_y3:0x13,y huffman table dc3,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x13_hufdc_len_y3(void)
{
    return REG_READ(JPEG_DEC_REG0X13_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x13_hufdc_len_y3(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X13_ADDR,value);
}

/* REG_0x14 */
#define JPEG_DEC_REG0X14_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x14*4) //REG ADDR :0x48040050
#define JPEG_DEC_REG0X14_HUFDC_LEN_Y4_POS (0) 
#define JPEG_DEC_REG0X14_HUFDC_LEN_Y4_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x14_value(void)
{
    return REG_READ(JPEG_DEC_REG0X14_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x14_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X14_ADDR,value);
}

/* REG_0x14:reg0x14->hufdc_len_y4:0x14,y huffman table dc4,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x14_hufdc_len_y4(void)
{
    return REG_READ(JPEG_DEC_REG0X14_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x14_hufdc_len_y4(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X14_ADDR,value);
}

/* REG_0x15 */
#define JPEG_DEC_REG0X15_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x15*4) //REG ADDR :0x48040054
#define JPEG_DEC_REG0X15_HUFDC_LEN_Y5_POS (0) 
#define JPEG_DEC_REG0X15_HUFDC_LEN_Y5_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x15_value(void)
{
    return REG_READ(JPEG_DEC_REG0X15_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x15_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X15_ADDR,value);
}

/* REG_0x15:reg0x15->hufdc_len_y5:0x15,y huffman table dc5,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x15_hufdc_len_y5(void)
{
    return REG_READ(JPEG_DEC_REG0X15_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x15_hufdc_len_y5(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X15_ADDR,value);
}

/* REG_0x16 */
#define JPEG_DEC_REG0X16_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x16*4) //REG ADDR :0x48040058
#define JPEG_DEC_REG0X16_HUFDC_LEN_Y6_POS (0) 
#define JPEG_DEC_REG0X16_HUFDC_LEN_Y6_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x16_value(void)
{
    return REG_READ(JPEG_DEC_REG0X16_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x16_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X16_ADDR,value);
}

/* REG_0x16:reg0x16->hufdc_len_y6:0x16,y huffman table dc6,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x16_hufdc_len_y6(void)
{
    return REG_READ(JPEG_DEC_REG0X16_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x16_hufdc_len_y6(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X16_ADDR,value);
}

/* REG_0x17 */
#define JPEG_DEC_REG0X17_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x17*4) //REG ADDR :0x4804005c
#define JPEG_DEC_REG0X17_HUFDC_LEN_Y7_POS (0) 
#define JPEG_DEC_REG0X17_HUFDC_LEN_Y7_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x17_value(void)
{
    return REG_READ(JPEG_DEC_REG0X17_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x17_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X17_ADDR,value);
}

/* REG_0x17:reg0x17->hufdc_len_y7:0x17,y huffman table dc7,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x17_hufdc_len_y7(void)
{
    return REG_READ(JPEG_DEC_REG0X17_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x17_hufdc_len_y7(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X17_ADDR,value);
}

/* REG_0x18 */
#define JPEG_DEC_REG0X18_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x18*4) //REG ADDR :0x48040060
#define JPEG_DEC_REG0X18_HUFDC_LEN_Y8_POS (0) 
#define JPEG_DEC_REG0X18_HUFDC_LEN_Y8_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x18_value(void)
{
    return REG_READ(JPEG_DEC_REG0X18_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x18_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X18_ADDR,value);
}

/* REG_0x18:reg0x18->hufdc_len_y8:0x18,y huffman table dc8,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x18_hufdc_len_y8(void)
{
    return REG_READ(JPEG_DEC_REG0X18_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x18_hufdc_len_y8(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X18_ADDR,value);
}

/* REG_0x19 */
#define JPEG_DEC_REG0X19_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x19*4) //REG ADDR :0x48040064
#define JPEG_DEC_REG0X19_HUFDC_LEN_Y9_POS (0) 
#define JPEG_DEC_REG0X19_HUFDC_LEN_Y9_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x19_value(void)
{
    return REG_READ(JPEG_DEC_REG0X19_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x19_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X19_ADDR,value);
}

/* REG_0x19:reg0x19->hufdc_len_y9:0x19,y huffman table dc9,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x19_hufdc_len_y9(void)
{
    return REG_READ(JPEG_DEC_REG0X19_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x19_hufdc_len_y9(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X19_ADDR,value);
}

/* REG_0x1A */
#define JPEG_DEC_REG0X1A_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x1A*4) //REG ADDR :0x48040068
#define JPEG_DEC_REG0X1A_HUFDC_LEN_YA_POS (0) 
#define JPEG_DEC_REG0X1A_HUFDC_LEN_YA_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x1a_value(void)
{
    return REG_READ(JPEG_DEC_REG0X1A_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x1a_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X1A_ADDR,value);
}

/* REG_0x1a:reg0x1a->hufdc_len_ya:0x1a,y huffman table dca,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x1a_hufdc_len_ya(void)
{
    return REG_READ(JPEG_DEC_REG0X1A_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x1a_hufdc_len_ya(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X1A_ADDR,value);
}

/* REG_0x1B */
#define JPEG_DEC_REG0X1B_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x1B*4) //REG ADDR :0x4804006c
#define JPEG_DEC_REG0X1B_HUFDC_LEN_YB_POS (0) 
#define JPEG_DEC_REG0X1B_HUFDC_LEN_YB_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x1b_value(void)
{
    return REG_READ(JPEG_DEC_REG0X1B_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x1b_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X1B_ADDR,value);
}

/* REG_0x1b:reg0x1b->hufdc_len_yb:0x1b,y huffman table dcb,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x1b_hufdc_len_yb(void)
{
    return REG_READ(JPEG_DEC_REG0X1B_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x1b_hufdc_len_yb(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X1B_ADDR,value);
}

/* REG_0x1C */
#define JPEG_DEC_REG0X1C_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x1C*4) //REG ADDR :0x48040070
#define JPEG_DEC_REG0X1C_HUFDC_LEN_YC_POS (0) 
#define JPEG_DEC_REG0X1C_HUFDC_LEN_YC_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x1c_value(void)
{
    return REG_READ(JPEG_DEC_REG0X1C_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x1c_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X1C_ADDR,value);
}

/* REG_0x1c:reg0x1c->hufdc_len_yc:0x1c,y huffman table dcc,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x1c_hufdc_len_yc(void)
{
    return REG_READ(JPEG_DEC_REG0X1C_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x1c_hufdc_len_yc(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X1C_ADDR,value);
}

/* REG_0x1D */
#define JPEG_DEC_REG0X1D_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x1D*4) //REG ADDR :0x48040074
#define JPEG_DEC_REG0X1D_HUFDC_LEN_YD_POS (0) 
#define JPEG_DEC_REG0X1D_HUFDC_LEN_YD_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x1d_value(void)
{
    return REG_READ(JPEG_DEC_REG0X1D_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x1d_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X1D_ADDR,value);
}

/* REG_0x1d:reg0x1d->hufdc_len_yd:0x1d,y huffman table dcd,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x1d_hufdc_len_yd(void)
{
    return REG_READ(JPEG_DEC_REG0X1D_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x1d_hufdc_len_yd(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X1D_ADDR,value);
}

/* REG_0x1E */
#define JPEG_DEC_REG0X1E_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x1E*4) //REG ADDR :0x48040078
#define JPEG_DEC_REG0X1E_HUFDC_LEN_YE_POS (0) 
#define JPEG_DEC_REG0X1E_HUFDC_LEN_YE_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x1e_value(void)
{
    return REG_READ(JPEG_DEC_REG0X1E_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x1e_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X1E_ADDR,value);
}

/* REG_0x1e:reg0x1e->hufdc_len_ye:0x1e,y huffman table dce,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x1e_hufdc_len_ye(void)
{
    return REG_READ(JPEG_DEC_REG0X1E_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x1e_hufdc_len_ye(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X1E_ADDR,value);
}

/* REG_0x1F */
#define JPEG_DEC_REG0X1F_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x1F*4) //REG ADDR :0x4804007c
#define JPEG_DEC_REG0X1F_HUFDC_LEN_YF_POS (0) 
#define JPEG_DEC_REG0X1F_HUFDC_LEN_YF_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x1f_value(void)
{
    return REG_READ(JPEG_DEC_REG0X1F_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x1f_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X1F_ADDR,value);
}

/* REG_0x1f:reg0x1f->hufdc_len_yf:0x1f,y huffman table dcf,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x1f_hufdc_len_yf(void)
{
    return REG_READ(JPEG_DEC_REG0X1F_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x1f_hufdc_len_yf(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X1F_ADDR,value);
}

/* REG_0x20 */
#define JPEG_DEC_REG0X20_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x20*4) //REG ADDR :0x48040080
#define JPEG_DEC_REG0X20_HUFDC_LEN_UV0_POS (0) 
#define JPEG_DEC_REG0X20_HUFDC_LEN_UV0_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x20_value(void)
{
    return REG_READ(JPEG_DEC_REG0X20_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x20_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X20_ADDR,value);
}

/* REG_0x20:reg0x20->hufdc_len_uv0:0x20,uv huffman table dc0,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x20_hufdc_len_uv0(void)
{
    return REG_READ(JPEG_DEC_REG0X20_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x20_hufdc_len_uv0(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X20_ADDR,value);
}

/* REG_0x21 */
#define JPEG_DEC_REG0X21_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x21*4) //REG ADDR :0x48040084
#define JPEG_DEC_REG0X21_HUFDC_LEN_UV1_POS (0) 
#define JPEG_DEC_REG0X21_HUFDC_LEN_UV1_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x21_value(void)
{
    return REG_READ(JPEG_DEC_REG0X21_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x21_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X21_ADDR,value);
}

/* REG_0x21:reg0x21->hufdc_len_uv1:0x21,uv huffman table dc1,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x21_hufdc_len_uv1(void)
{
    return REG_READ(JPEG_DEC_REG0X21_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x21_hufdc_len_uv1(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X21_ADDR,value);
}

/* REG_0x22 */
#define JPEG_DEC_REG0X22_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x22*4) //REG ADDR :0x48040088
#define JPEG_DEC_REG0X22_HUFDC_LEN_UV2_POS (0) 
#define JPEG_DEC_REG0X22_HUFDC_LEN_UV2_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x22_value(void)
{
    return REG_READ(JPEG_DEC_REG0X22_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x22_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X22_ADDR,value);
}

/* REG_0x22:reg0x22->hufdc_len_uv2:0x22,uv huffman table dc2,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x22_hufdc_len_uv2(void)
{
    return REG_READ(JPEG_DEC_REG0X22_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x22_hufdc_len_uv2(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X22_ADDR,value);
}

/* REG_0x23 */
#define JPEG_DEC_REG0X23_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x23*4) //REG ADDR :0x4804008c
#define JPEG_DEC_REG0X23_HUFDC_LEN_UV3_POS (0) 
#define JPEG_DEC_REG0X23_HUFDC_LEN_UV3_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x23_value(void)
{
    return REG_READ(JPEG_DEC_REG0X23_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x23_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X23_ADDR,value);
}

/* REG_0x23:reg0x23->hufdc_len_uv3:0x23,uv huffman table dc3,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x23_hufdc_len_uv3(void)
{
    return REG_READ(JPEG_DEC_REG0X23_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x23_hufdc_len_uv3(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X23_ADDR,value);
}

/* REG_0x24 */
#define JPEG_DEC_REG0X24_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x24*4) //REG ADDR :0x48040090
#define JPEG_DEC_REG0X24_HUFDC_LEN_UV4_POS (0) 
#define JPEG_DEC_REG0X24_HUFDC_LEN_UV4_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x24_value(void)
{
    return REG_READ(JPEG_DEC_REG0X24_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x24_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X24_ADDR,value);
}

/* REG_0x24:reg0x24->hufdc_len_uv4:0x24,uv huffman table dc4,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x24_hufdc_len_uv4(void)
{
    return REG_READ(JPEG_DEC_REG0X24_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x24_hufdc_len_uv4(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X24_ADDR,value);
}

/* REG_0x25 */
#define JPEG_DEC_REG0X25_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x25*4) //REG ADDR :0x48040094
#define JPEG_DEC_REG0X25_HUFDC_LEN_UV5_POS (0) 
#define JPEG_DEC_REG0X25_HUFDC_LEN_UV5_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x25_value(void)
{
    return REG_READ(JPEG_DEC_REG0X25_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x25_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X25_ADDR,value);
}

/* REG_0x25:reg0x25->hufdc_len_uv5:0x25,uv huffman table dc5,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x25_hufdc_len_uv5(void)
{
    return REG_READ(JPEG_DEC_REG0X25_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x25_hufdc_len_uv5(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X25_ADDR,value);
}

/* REG_0x26 */
#define JPEG_DEC_REG0X26_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x26*4) //REG ADDR :0x48040098
#define JPEG_DEC_REG0X26_HUFDC_LEN_UV6_POS (0) 
#define JPEG_DEC_REG0X26_HUFDC_LEN_UV6_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x26_value(void)
{
    return REG_READ(JPEG_DEC_REG0X26_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x26_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X26_ADDR,value);
}

/* REG_0x26:reg0x26->hufdc_len_uv6:0x26,uv huffman table dc6,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x26_hufdc_len_uv6(void)
{
    return REG_READ(JPEG_DEC_REG0X26_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x26_hufdc_len_uv6(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X26_ADDR,value);
}

/* REG_0x27 */
#define JPEG_DEC_REG0X27_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x27*4) //REG ADDR :0x4804009c
#define JPEG_DEC_REG0X27_HUFDC_LEN_UV7_POS (0) 
#define JPEG_DEC_REG0X27_HUFDC_LEN_UV7_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x27_value(void)
{
    return REG_READ(JPEG_DEC_REG0X27_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x27_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X27_ADDR,value);
}

/* REG_0x27:reg0x27->hufdc_len_uv7:0x27,uv huffman table dc7,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x27_hufdc_len_uv7(void)
{
    return REG_READ(JPEG_DEC_REG0X27_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x27_hufdc_len_uv7(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X27_ADDR,value);
}

/* REG_0x28 */
#define JPEG_DEC_REG0X28_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x28*4) //REG ADDR :0x480400a0
#define JPEG_DEC_REG0X28_HUFDC_LEN_UV8_POS (0) 
#define JPEG_DEC_REG0X28_HUFDC_LEN_UV8_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x28_value(void)
{
    return REG_READ(JPEG_DEC_REG0X28_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x28_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X28_ADDR,value);
}

/* REG_0x28:reg0x28->hufdc_len_uv8:0x28,uv huffman table dc8,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x28_hufdc_len_uv8(void)
{
    return REG_READ(JPEG_DEC_REG0X28_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x28_hufdc_len_uv8(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X28_ADDR,value);
}

/* REG_0x29 */
#define JPEG_DEC_REG0X29_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x29*4) //REG ADDR :0x480400a4
#define JPEG_DEC_REG0X29_HUFDC_LEN_UV9_POS (0) 
#define JPEG_DEC_REG0X29_HUFDC_LEN_UV9_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x29_value(void)
{
    return REG_READ(JPEG_DEC_REG0X29_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x29_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X29_ADDR,value);
}

/* REG_0x29:reg0x29->hufdc_len_uv9:0x29,uv huffman table dc9,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x29_hufdc_len_uv9(void)
{
    return REG_READ(JPEG_DEC_REG0X29_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x29_hufdc_len_uv9(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X29_ADDR,value);
}

/* REG_0x2A */
#define JPEG_DEC_REG0X2A_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x2A*4) //REG ADDR :0x480400a8
#define JPEG_DEC_REG0X2A_HUFDC_LEN_UVA_POS (0) 
#define JPEG_DEC_REG0X2A_HUFDC_LEN_UVA_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x2a_value(void)
{
    return REG_READ(JPEG_DEC_REG0X2A_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x2a_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X2A_ADDR,value);
}

/* REG_0x2a:reg0x2a->hufdc_len_uva:0x2a,uv huffman table dca,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x2a_hufdc_len_uva(void)
{
    return REG_READ(JPEG_DEC_REG0X2A_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x2a_hufdc_len_uva(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X2A_ADDR,value);
}

/* REG_0x2B */
#define JPEG_DEC_REG0X2B_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x2B*4) //REG ADDR :0x480400ac
#define JPEG_DEC_REG0X2B_HUFDC_LEN_UVB_POS (0) 
#define JPEG_DEC_REG0X2B_HUFDC_LEN_UVB_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x2b_value(void)
{
    return REG_READ(JPEG_DEC_REG0X2B_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x2b_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X2B_ADDR,value);
}

/* REG_0x2b:reg0x2b->hufdc_len_uvb:0x2b,uv huffman table dcb,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x2b_hufdc_len_uvb(void)
{
    return REG_READ(JPEG_DEC_REG0X2B_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x2b_hufdc_len_uvb(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X2B_ADDR,value);
}

/* REG_0x2C */
#define JPEG_DEC_REG0X2C_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x2C*4) //REG ADDR :0x480400b0
#define JPEG_DEC_REG0X2C_HUFDC_LEN_UVC_POS (0) 
#define JPEG_DEC_REG0X2C_HUFDC_LEN_UVC_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x2c_value(void)
{
    return REG_READ(JPEG_DEC_REG0X2C_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x2c_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X2C_ADDR,value);
}

/* REG_0x2c:reg0x2c->hufdc_len_uvc:0x2c,uv huffman table dcc,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x2c_hufdc_len_uvc(void)
{
    return REG_READ(JPEG_DEC_REG0X2C_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x2c_hufdc_len_uvc(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X2C_ADDR,value);
}

/* REG_0x2D */
#define JPEG_DEC_REG0X2D_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x2D*4) //REG ADDR :0x480400b4
#define JPEG_DEC_REG0X2D_HUFDC_LEN_UVD_POS (0) 
#define JPEG_DEC_REG0X2D_HUFDC_LEN_UVD_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x2d_value(void)
{
    return REG_READ(JPEG_DEC_REG0X2D_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x2d_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X2D_ADDR,value);
}

/* REG_0x2d:reg0x2d->hufdc_len_uvd:0x2d,uv huffman table dcd,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x2d_hufdc_len_uvd(void)
{
    return REG_READ(JPEG_DEC_REG0X2D_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x2d_hufdc_len_uvd(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X2D_ADDR,value);
}

/* REG_0x2E */
#define JPEG_DEC_REG0X2E_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x2E*4) //REG ADDR :0x480400b8
#define JPEG_DEC_REG0X2E_HUFDC_LEN_UVE_POS (0) 
#define JPEG_DEC_REG0X2E_HUFDC_LEN_UVE_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x2e_value(void)
{
    return REG_READ(JPEG_DEC_REG0X2E_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x2e_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X2E_ADDR,value);
}

/* REG_0x2e:reg0x2e->hufdc_len_uve:0x2e,uv huffman table dce,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x2e_hufdc_len_uve(void)
{
    return REG_READ(JPEG_DEC_REG0X2E_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x2e_hufdc_len_uve(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X2E_ADDR,value);
}

/* REG_0x2F */
#define JPEG_DEC_REG0X2F_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x2F*4) //REG ADDR :0x480400bc
#define JPEG_DEC_REG0X2F_HUFDC_LEN_UVF_POS (0) 
#define JPEG_DEC_REG0X2F_HUFDC_LEN_UVF_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x2f_value(void)
{
    return REG_READ(JPEG_DEC_REG0X2F_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x2f_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X2F_ADDR,value);
}

/* REG_0x2f:reg0x2f->hufdc_len_uvf:0x2f,uv huffman table dcf,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x2f_hufdc_len_uvf(void)
{
    return REG_READ(JPEG_DEC_REG0X2F_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x2f_hufdc_len_uvf(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X2F_ADDR,value);
}

/* REG_0x30 */
#define JPEG_DEC_REG0X30_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x30*4) //REG ADDR :0x480400c0
#define JPEG_DEC_REG0X30_HUFAC_LEN_Y0_POS (0) 
#define JPEG_DEC_REG0X30_HUFAC_LEN_Y0_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x30_value(void)
{
    return REG_READ(JPEG_DEC_REG0X30_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x30_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X30_ADDR,value);
}

/* REG_0x30:reg0x30->hufac_len_y0:0x30,y huffman table ac0,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x30_hufac_len_y0(void)
{
    return REG_READ(JPEG_DEC_REG0X30_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x30_hufac_len_y0(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X30_ADDR,value);
}

/* REG_0x31 */
#define JPEG_DEC_REG0X31_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x31*4) //REG ADDR :0x480400c4
#define JPEG_DEC_REG0X31_HUFAC_LEN_Y1_POS (0) 
#define JPEG_DEC_REG0X31_HUFAC_LEN_Y1_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x31_value(void)
{
    return REG_READ(JPEG_DEC_REG0X31_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x31_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X31_ADDR,value);
}

/* REG_0x31:reg0x31->hufac_len_y1:0x31,y huffman table ac1,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x31_hufac_len_y1(void)
{
    return REG_READ(JPEG_DEC_REG0X31_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x31_hufac_len_y1(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X31_ADDR,value);
}

/* REG_0x32 */
#define JPEG_DEC_REG0X32_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x32*4) //REG ADDR :0x480400c8
#define JPEG_DEC_REG0X32_HUFAC_LEN_Y2_POS (0) 
#define JPEG_DEC_REG0X32_HUFAC_LEN_Y2_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x32_value(void)
{
    return REG_READ(JPEG_DEC_REG0X32_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x32_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X32_ADDR,value);
}

/* REG_0x32:reg0x32->hufac_len_y2:0x32,y huffman table ac2,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x32_hufac_len_y2(void)
{
    return REG_READ(JPEG_DEC_REG0X32_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x32_hufac_len_y2(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X32_ADDR,value);
}

/* REG_0x33 */
#define JPEG_DEC_REG0X33_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x33*4) //REG ADDR :0x480400cc
#define JPEG_DEC_REG0X33_HUFAC_LEN_Y3_POS (0) 
#define JPEG_DEC_REG0X33_HUFAC_LEN_Y3_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x33_value(void)
{
    return REG_READ(JPEG_DEC_REG0X33_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x33_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X33_ADDR,value);
}

/* REG_0x33:reg0x33->hufac_len_y3:0x33,y huffman table ac3,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x33_hufac_len_y3(void)
{
    return REG_READ(JPEG_DEC_REG0X33_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x33_hufac_len_y3(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X33_ADDR,value);
}

/* REG_0x34 */
#define JPEG_DEC_REG0X34_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x34*4) //REG ADDR :0x480400d0
#define JPEG_DEC_REG0X34_HUFAC_LEN_Y4_POS (0) 
#define JPEG_DEC_REG0X34_HUFAC_LEN_Y4_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x34_value(void)
{
    return REG_READ(JPEG_DEC_REG0X34_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x34_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X34_ADDR,value);
}

/* REG_0x34:reg0x34->hufac_len_y4:0x34,y huffman table ac4,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x34_hufac_len_y4(void)
{
    return REG_READ(JPEG_DEC_REG0X34_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x34_hufac_len_y4(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X34_ADDR,value);
}

/* REG_0x35 */
#define JPEG_DEC_REG0X35_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x35*4) //REG ADDR :0x480400d4
#define JPEG_DEC_REG0X35_HUFAC_LEN_Y5_POS (0) 
#define JPEG_DEC_REG0X35_HUFAC_LEN_Y5_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x35_value(void)
{
    return REG_READ(JPEG_DEC_REG0X35_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x35_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X35_ADDR,value);
}

/* REG_0x35:reg0x35->hufac_len_y5:0x35,y huffman table ac5,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x35_hufac_len_y5(void)
{
    return REG_READ(JPEG_DEC_REG0X35_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x35_hufac_len_y5(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X35_ADDR,value);
}

/* REG_0x36 */
#define JPEG_DEC_REG0X36_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x36*4) //REG ADDR :0x480400d8
#define JPEG_DEC_REG0X36_HUFAC_LEN_Y6_POS (0) 
#define JPEG_DEC_REG0X36_HUFAC_LEN_Y6_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x36_value(void)
{
    return REG_READ(JPEG_DEC_REG0X36_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x36_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X36_ADDR,value);
}

/* REG_0x36:reg0x36->hufac_len_y6:0x36,y huffman table ac6,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x36_hufac_len_y6(void)
{
    return REG_READ(JPEG_DEC_REG0X36_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x36_hufac_len_y6(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X36_ADDR,value);
}

/* REG_0x37 */
#define JPEG_DEC_REG0X37_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x37*4) //REG ADDR :0x480400dc
#define JPEG_DEC_REG0X37_HUFAC_LEN_Y7_POS (0) 
#define JPEG_DEC_REG0X37_HUFAC_LEN_Y7_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x37_value(void)
{
    return REG_READ(JPEG_DEC_REG0X37_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x37_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X37_ADDR,value);
}

/* REG_0x37:reg0x37->hufac_len_y7:0x37,y huffman table ac7,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x37_hufac_len_y7(void)
{
    return REG_READ(JPEG_DEC_REG0X37_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x37_hufac_len_y7(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X37_ADDR,value);
}

/* REG_0x38 */
#define JPEG_DEC_REG0X38_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x38*4) //REG ADDR :0x480400e0
#define JPEG_DEC_REG0X38_HUFAC_LEN_Y8_POS (0) 
#define JPEG_DEC_REG0X38_HUFAC_LEN_Y8_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x38_value(void)
{
    return REG_READ(JPEG_DEC_REG0X38_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x38_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X38_ADDR,value);
}

/* REG_0x38:reg0x38->hufac_len_y8:0x38,y huffman table ac8,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x38_hufac_len_y8(void)
{
    return REG_READ(JPEG_DEC_REG0X38_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x38_hufac_len_y8(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X38_ADDR,value);
}

/* REG_0x39 */
#define JPEG_DEC_REG0X39_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x39*4) //REG ADDR :0x480400e4
#define JPEG_DEC_REG0X39_HUFAC_LEN_Y9_POS (0) 
#define JPEG_DEC_REG0X39_HUFAC_LEN_Y9_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x39_value(void)
{
    return REG_READ(JPEG_DEC_REG0X39_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x39_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X39_ADDR,value);
}

/* REG_0x39:reg0x39->hufac_len_y9:0x39,y huffman table ac9,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x39_hufac_len_y9(void)
{
    return REG_READ(JPEG_DEC_REG0X39_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x39_hufac_len_y9(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X39_ADDR,value);
}

/* REG_0x3A */
#define JPEG_DEC_REG0X3A_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x3A*4) //REG ADDR :0x480400e8
#define JPEG_DEC_REG0X3A_HUFAC_LEN_YA_POS (0) 
#define JPEG_DEC_REG0X3A_HUFAC_LEN_YA_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x3a_value(void)
{
    return REG_READ(JPEG_DEC_REG0X3A_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x3a_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X3A_ADDR,value);
}

/* REG_0x3a:reg0x3a->hufac_len_ya:0x3a,y huffman table aca,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x3a_hufac_len_ya(void)
{
    return REG_READ(JPEG_DEC_REG0X3A_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x3a_hufac_len_ya(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X3A_ADDR,value);
}

/* REG_0x3B */
#define JPEG_DEC_REG0X3B_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x3B*4) //REG ADDR :0x480400ec
#define JPEG_DEC_REG0X3B_HUFAC_LEN_YB_POS (0) 
#define JPEG_DEC_REG0X3B_HUFAC_LEN_YB_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x3b_value(void)
{
    return REG_READ(JPEG_DEC_REG0X3B_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x3b_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X3B_ADDR,value);
}

/* REG_0x3b:reg0x3b->hufac_len_yb:0x3b,y huffman table acb,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x3b_hufac_len_yb(void)
{
    return REG_READ(JPEG_DEC_REG0X3B_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x3b_hufac_len_yb(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X3B_ADDR,value);
}

/* REG_0x3C */
#define JPEG_DEC_REG0X3C_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x3C*4) //REG ADDR :0x480400f0
#define JPEG_DEC_REG0X3C_HUFAC_LEN_YC_POS (0) 
#define JPEG_DEC_REG0X3C_HUFAC_LEN_YC_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x3c_value(void)
{
    return REG_READ(JPEG_DEC_REG0X3C_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x3c_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X3C_ADDR,value);
}

/* REG_0x3c:reg0x3c->hufac_len_yc:0x3c,y huffman table acc,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x3c_hufac_len_yc(void)
{
    return REG_READ(JPEG_DEC_REG0X3C_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x3c_hufac_len_yc(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X3C_ADDR,value);
}

/* REG_0x3D */
#define JPEG_DEC_REG0X3D_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x3D*4) //REG ADDR :0x480400f4
#define JPEG_DEC_REG0X3D_HUFAC_LEN_YD_POS (0) 
#define JPEG_DEC_REG0X3D_HUFAC_LEN_YD_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x3d_value(void)
{
    return REG_READ(JPEG_DEC_REG0X3D_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x3d_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X3D_ADDR,value);
}

/* REG_0x3d:reg0x3d->hufac_len_yd:0x3d,y huffman table acd,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x3d_hufac_len_yd(void)
{
    return REG_READ(JPEG_DEC_REG0X3D_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x3d_hufac_len_yd(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X3D_ADDR,value);
}

/* REG_0x3E */
#define JPEG_DEC_REG0X3E_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x3E*4) //REG ADDR :0x480400f8
#define JPEG_DEC_REG0X3E_HUFAC_LEN_YE_POS (0) 
#define JPEG_DEC_REG0X3E_HUFAC_LEN_YE_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x3e_value(void)
{
    return REG_READ(JPEG_DEC_REG0X3E_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x3e_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X3E_ADDR,value);
}

/* REG_0x3e:reg0x3e->hufac_len_ye:0x3e,y huffman table ace,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x3e_hufac_len_ye(void)
{
    return REG_READ(JPEG_DEC_REG0X3E_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x3e_hufac_len_ye(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X3E_ADDR,value);
}

/* REG_0x3F */
#define JPEG_DEC_REG0X3F_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x3F*4) //REG ADDR :0x480400fc
#define JPEG_DEC_REG0X3F_HUFAC_LEN_YF_POS (0) 
#define JPEG_DEC_REG0X3F_HUFAC_LEN_YF_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x3f_value(void)
{
    return REG_READ(JPEG_DEC_REG0X3F_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x3f_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X3F_ADDR,value);
}

/* REG_0x3f:reg0x3f->hufac_len_yf:0x3f,y huffman table acf,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x3f_hufac_len_yf(void)
{
    return REG_READ(JPEG_DEC_REG0X3F_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x3f_hufac_len_yf(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X3F_ADDR,value);
}

/* REG_0x40 */
#define JPEG_DEC_REG0X40_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x40*4) //REG ADDR :0x48040100
#define JPEG_DEC_REG0X40_HUFAC_LEN_UV0_POS (0) 
#define JPEG_DEC_REG0X40_HUFAC_LEN_UV0_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x40_value(void)
{
    return REG_READ(JPEG_DEC_REG0X40_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x40_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X40_ADDR,value);
}

/* REG_0x40:reg0x40->hufac_len_uv0:0x40,uv huffman table ac0,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x40_hufac_len_uv0(void)
{
    return REG_READ(JPEG_DEC_REG0X40_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x40_hufac_len_uv0(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X40_ADDR,value);
}

/* REG_0x41 */
#define JPEG_DEC_REG0X41_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x41*4) //REG ADDR :0x48040104
#define JPEG_DEC_REG0X41_HUFAC_LEN_UV1_POS (0) 
#define JPEG_DEC_REG0X41_HUFAC_LEN_UV1_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x41_value(void)
{
    return REG_READ(JPEG_DEC_REG0X41_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x41_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X41_ADDR,value);
}

/* REG_0x41:reg0x41->hufac_len_uv1:0x41,uv huffman table ac1,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x41_hufac_len_uv1(void)
{
    return REG_READ(JPEG_DEC_REG0X41_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x41_hufac_len_uv1(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X41_ADDR,value);
}

/* REG_0x42 */
#define JPEG_DEC_REG0X42_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x42*4) //REG ADDR :0x48040108
#define JPEG_DEC_REG0X42_HUFAC_LEN_UV2_POS (0) 
#define JPEG_DEC_REG0X42_HUFAC_LEN_UV2_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x42_value(void)
{
    return REG_READ(JPEG_DEC_REG0X42_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x42_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X42_ADDR,value);
}

/* REG_0x42:reg0x42->hufac_len_uv2:0x42,uv huffman table ac2,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x42_hufac_len_uv2(void)
{
    return REG_READ(JPEG_DEC_REG0X42_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x42_hufac_len_uv2(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X42_ADDR,value);
}

/* REG_0x43 */
#define JPEG_DEC_REG0X43_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x43*4) //REG ADDR :0x4804010c
#define JPEG_DEC_REG0X43_HUFAC_LEN_UV3_POS (0) 
#define JPEG_DEC_REG0X43_HUFAC_LEN_UV3_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x43_value(void)
{
    return REG_READ(JPEG_DEC_REG0X43_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x43_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X43_ADDR,value);
}

/* REG_0x43:reg0x43->hufac_len_uv3:0x43,uv huffman table ac3,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x43_hufac_len_uv3(void)
{
    return REG_READ(JPEG_DEC_REG0X43_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x43_hufac_len_uv3(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X43_ADDR,value);
}

/* REG_0x44 */
#define JPEG_DEC_REG0X44_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x44*4) //REG ADDR :0x48040110
#define JPEG_DEC_REG0X44_HUFAC_LEN_UV4_POS (0) 
#define JPEG_DEC_REG0X44_HUFAC_LEN_UV4_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x44_value(void)
{
    return REG_READ(JPEG_DEC_REG0X44_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x44_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X44_ADDR,value);
}

/* REG_0x44:reg0x44->hufac_len_uv4:0x44,uv huffman table ac4,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x44_hufac_len_uv4(void)
{
    return REG_READ(JPEG_DEC_REG0X44_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x44_hufac_len_uv4(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X44_ADDR,value);
}

/* REG_0x45 */
#define JPEG_DEC_REG0X45_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x45*4) //REG ADDR :0x48040114
#define JPEG_DEC_REG0X45_HUFAC_LEN_UV5_POS (0) 
#define JPEG_DEC_REG0X45_HUFAC_LEN_UV5_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x45_value(void)
{
    return REG_READ(JPEG_DEC_REG0X45_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x45_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X45_ADDR,value);
}

/* REG_0x45:reg0x45->hufac_len_uv5:0x45,uv huffman table ac5,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x45_hufac_len_uv5(void)
{
    return REG_READ(JPEG_DEC_REG0X45_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x45_hufac_len_uv5(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X45_ADDR,value);
}

/* REG_0x46 */
#define JPEG_DEC_REG0X46_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x46*4) //REG ADDR :0x48040118
#define JPEG_DEC_REG0X46_HUFAC_LEN_UV6_POS (0) 
#define JPEG_DEC_REG0X46_HUFAC_LEN_UV6_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x46_value(void)
{
    return REG_READ(JPEG_DEC_REG0X46_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x46_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X46_ADDR,value);
}

/* REG_0x46:reg0x46->hufac_len_uv6:0x46,uv huffman table ac6,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x46_hufac_len_uv6(void)
{
    return REG_READ(JPEG_DEC_REG0X46_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x46_hufac_len_uv6(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X46_ADDR,value);
}

/* REG_0x47 */
#define JPEG_DEC_REG0X47_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x47*4) //REG ADDR :0x4804011c
#define JPEG_DEC_REG0X47_HUFAC_LEN_UV7_POS (0) 
#define JPEG_DEC_REG0X47_HUFAC_LEN_UV7_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x47_value(void)
{
    return REG_READ(JPEG_DEC_REG0X47_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x47_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X47_ADDR,value);
}

/* REG_0x47:reg0x47->hufac_len_uv7:0x47,uv huffman table ac7,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x47_hufac_len_uv7(void)
{
    return REG_READ(JPEG_DEC_REG0X47_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x47_hufac_len_uv7(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X47_ADDR,value);
}

/* REG_0x48 */
#define JPEG_DEC_REG0X48_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x48*4) //REG ADDR :0x48040120
#define JPEG_DEC_REG0X48_HUFAC_LEN_UV8_POS (0) 
#define JPEG_DEC_REG0X48_HUFAC_LEN_UV8_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x48_value(void)
{
    return REG_READ(JPEG_DEC_REG0X48_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x48_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X48_ADDR,value);
}

/* REG_0x48:reg0x48->hufac_len_uv8:0x48,uv huffman table ac8,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x48_hufac_len_uv8(void)
{
    return REG_READ(JPEG_DEC_REG0X48_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x48_hufac_len_uv8(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X48_ADDR,value);
}

/* REG_0x49 */
#define JPEG_DEC_REG0X49_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x49*4) //REG ADDR :0x48040124
#define JPEG_DEC_REG0X49_HUFAC_LEN_UV9_POS (0) 
#define JPEG_DEC_REG0X49_HUFAC_LEN_UV9_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x49_value(void)
{
    return REG_READ(JPEG_DEC_REG0X49_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x49_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X49_ADDR,value);
}

/* REG_0x49:reg0x49->hufac_len_uv9:0x49,uv huffman table ac9,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x49_hufac_len_uv9(void)
{
    return REG_READ(JPEG_DEC_REG0X49_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x49_hufac_len_uv9(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X49_ADDR,value);
}

/* REG_0x4A */
#define JPEG_DEC_REG0X4A_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x4A*4) //REG ADDR :0x48040128
#define JPEG_DEC_REG0X4A_HUFAC_LEN_UVA_POS (0) 
#define JPEG_DEC_REG0X4A_HUFAC_LEN_UVA_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x4a_value(void)
{
    return REG_READ(JPEG_DEC_REG0X4A_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x4a_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X4A_ADDR,value);
}

/* REG_0x4a:reg0x4a->hufac_len_uva:0x4a,uv huffman table aca,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x4a_hufac_len_uva(void)
{
    return REG_READ(JPEG_DEC_REG0X4A_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x4a_hufac_len_uva(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X4A_ADDR,value);
}

/* REG_0x4B */
#define JPEG_DEC_REG0X4B_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x4B*4) //REG ADDR :0x4804012c
#define JPEG_DEC_REG0X4B_HUFAC_LEN_UVB_POS (0) 
#define JPEG_DEC_REG0X4B_HUFAC_LEN_UVB_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x4b_value(void)
{
    return REG_READ(JPEG_DEC_REG0X4B_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x4b_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X4B_ADDR,value);
}

/* REG_0x4b:reg0x4b->hufac_len_uvb:0x4b,uv huffman table acb,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x4b_hufac_len_uvb(void)
{
    return REG_READ(JPEG_DEC_REG0X4B_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x4b_hufac_len_uvb(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X4B_ADDR,value);
}

/* REG_0x4C */
#define JPEG_DEC_REG0X4C_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x4C*4) //REG ADDR :0x48040130
#define JPEG_DEC_REG0X4C_HUFAC_LEN_UVC_POS (0) 
#define JPEG_DEC_REG0X4C_HUFAC_LEN_UVC_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x4c_value(void)
{
    return REG_READ(JPEG_DEC_REG0X4C_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x4c_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X4C_ADDR,value);
}

/* REG_0x4c:reg0x4c->hufac_len_uvc:0x4c,uv huffman table acc,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x4c_hufac_len_uvc(void)
{
    return REG_READ(JPEG_DEC_REG0X4C_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x4c_hufac_len_uvc(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X4C_ADDR,value);
}

/* REG_0x4D */
#define JPEG_DEC_REG0X4D_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x4D*4) //REG ADDR :0x48040134
#define JPEG_DEC_REG0X4D_HUFAC_LEN_UVD_POS (0) 
#define JPEG_DEC_REG0X4D_HUFAC_LEN_UVD_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x4d_value(void)
{
    return REG_READ(JPEG_DEC_REG0X4D_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x4d_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X4D_ADDR,value);
}

/* REG_0x4d:reg0x4d->hufac_len_uvd:0x4d,uv huffman table acd,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x4d_hufac_len_uvd(void)
{
    return REG_READ(JPEG_DEC_REG0X4D_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x4d_hufac_len_uvd(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X4D_ADDR,value);
}

/* REG_0x4E */
#define JPEG_DEC_REG0X4E_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x4E*4) //REG ADDR :0x48040138
#define JPEG_DEC_REG0X4E_HUFAC_LEN_UVE_POS (0) 
#define JPEG_DEC_REG0X4E_HUFAC_LEN_UVE_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x4e_value(void)
{
    return REG_READ(JPEG_DEC_REG0X4E_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x4e_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X4E_ADDR,value);
}

/* REG_0x4e:reg0x4e->hufac_len_uve:0x4e,uv huffman table ace,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x4e_hufac_len_uve(void)
{
    return REG_READ(JPEG_DEC_REG0X4E_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x4e_hufac_len_uve(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X4E_ADDR,value);
}

/* REG_0x4F */
#define JPEG_DEC_REG0X4F_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x4F*4) //REG ADDR :0x4804013c
#define JPEG_DEC_REG0X4F_HUFAC_LEN_UVF_POS (0) 
#define JPEG_DEC_REG0X4F_HUFAC_LEN_UVF_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x4f_value(void)
{
    return REG_READ(JPEG_DEC_REG0X4F_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x4f_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X4F_ADDR,value);
}

/* REG_0x4f:reg0x4f->hufac_len_uvf:0x4f,uv huffman table acf,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x4f_hufac_len_uvf(void)
{
    return REG_READ(JPEG_DEC_REG0X4F_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x4f_hufac_len_uvf(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X4F_ADDR,value);
}

/* REG_0x57 */
#define JPEG_DEC_REG0X57_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x57*4) //REG ADDR :0x4804015c
#define JPEG_DEC_REG0X57_MASTER_WR_CNT_POS (0) 
#define JPEG_DEC_REG0X57_MASTER_WR_CNT_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x57_value(void)
{
    return REG_READ(JPEG_DEC_REG0X57_ADDR);
}

/* REG_0x57:reg0x57->master_wr_cnt:0x57,master write count,None,R*/
static inline uint32_t jpeg_dec_ll_get_reg0x57_master_wr_cnt(void)
{
    return REG_READ(JPEG_DEC_REG0X57_ADDR);
}

/* REG_0x58 */
#define JPEG_DEC_REG0X58_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x58*4) //REG ADDR :0x48040160
#define JPEG_DEC_REG0X58_BASE_RADDR_POS (0) 
#define JPEG_DEC_REG0X58_BASE_RADDR_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x58_value(void)
{
    return REG_READ(JPEG_DEC_REG0X58_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x58_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X58_ADDR,value);
}

/* REG_0x58:reg0x58->BASE_RADDR:0x58,ahb memory read baseaddr,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x58_base_raddr(void)
{
    return REG_READ(JPEG_DEC_REG0X58_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x58_base_raddr(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X58_ADDR,value);
}

/* REG_0x59 */
#define JPEG_DEC_REG0X59_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x59*4) //REG ADDR :0x48040164
#define JPEG_DEC_REG0X59_BASE_WADDR_POS (0) 
#define JPEG_DEC_REG0X59_BASE_WADDR_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x59_value(void)
{
    return REG_READ(JPEG_DEC_REG0X59_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x59_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X59_ADDR,value);
}

/* REG_0x59:reg0x59->BASE_WADDR:0x59,ahb memory write baseaddr,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x59_base_waddr(void)
{
    return REG_READ(JPEG_DEC_REG0X59_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x59_base_waddr(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X59_ADDR,value);
}

/* REG_0x5A */
#define JPEG_DEC_REG0X5A_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x5A*4) //REG ADDR :0x48040168
#define JPEG_DEC_REG0X5A_BASE_RD_LEN_POS (0) 
#define JPEG_DEC_REG0X5A_BASE_RD_LEN_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x5a_value(void)
{
    return REG_READ(JPEG_DEC_REG0X5A_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x5a_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X5A_ADDR,value);
}

/* REG_0x5a:reg0x5a->BASE_RD_LEN:0x5a,ahb memory read len,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5a_base_rd_len(void)
{
    return REG_READ(JPEG_DEC_REG0X5A_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x5a_base_rd_len(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X5A_ADDR,value);
}

/* REG_0x5B */
#define JPEG_DEC_REG0X5B_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x5B*4) //REG ADDR :0x4804016c
#define JPEG_DEC_REG0X5B_BASE_WR_LEN_POS (0) 
#define JPEG_DEC_REG0X5B_BASE_WR_LEN_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x5b_value(void)
{
    return REG_READ(JPEG_DEC_REG0X5B_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x5b_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X5B_ADDR,value);
}

/* REG_0x5b:reg0x5b->BASE_WR_LEN:0x5b,ahb memory read len,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5b_base_wr_len(void)
{
    return REG_READ(JPEG_DEC_REG0X5B_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x5b_base_wr_len(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X5B_ADDR,value);
}

/* REG_0x5C */
#define JPEG_DEC_REG0X5C_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x5C*4) //REG ADDR :0x48040170
#define JPEG_DEC_REG0X5C_BASE_FFDA_POS (0) 
#define JPEG_DEC_REG0X5C_BASE_FFDA_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x5c_value(void)
{
    return REG_READ(JPEG_DEC_REG0X5C_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x5c_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X5C_ADDR,value);
}

/* REG_0x5c:reg0x5c->BASE_FFDA:0x5c,ahb memory ffda addr from BASE_RADDR,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5c_base_ffda(void)
{
    return REG_READ(JPEG_DEC_REG0X5C_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x5c_base_ffda(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X5C_ADDR,value);
}

/* REG_0x5D */
#define JPEG_DEC_REG0X5D_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x5D*4) //REG ADDR :0x48040174
#define JPEG_DEC_REG0X5D_MASTER_RD_CNT_POS (0) 
#define JPEG_DEC_REG0X5D_MASTER_RD_CNT_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x5d_value(void)
{
    return REG_READ(JPEG_DEC_REG0X5D_ADDR);
}

/* REG_0x5d:reg0x5d->master_rd_cnt:0x5d,master read count,0x0,R*/
static inline uint32_t jpeg_dec_ll_get_reg0x5d_master_rd_cnt(void)
{
    return REG_READ(JPEG_DEC_REG0X5D_ADDR);
}

/* REG_0x5E */
#define JPEG_DEC_REG0X5E_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x5E*4) //REG ADDR :0x48040178
#define JPEG_DEC_REG0X5E_DEC_BUSY_INT_POS (0) 
#define JPEG_DEC_REG0X5E_DEC_BUSY_INT_MASK (0x1) 

#define JPEG_DEC_REG0X5E_DEC_RRLOAD_INT_POS (1) 
#define JPEG_DEC_REG0X5E_DEC_RRLOAD_INT_MASK (0x1) 

#define JPEG_DEC_REG0X5E_DEC_RLOAD_INT_POS (2) 
#define JPEG_DEC_REG0X5E_DEC_RLOAD_INT_MASK (0x1) 

#define JPEG_DEC_REG0X5E_DEC_SEARCH_INT_POS (3) 
#define JPEG_DEC_REG0X5E_DEC_SEARCH_INT_MASK (0x1) 

#define JPEG_DEC_REG0X5E_DEC_EXT_RELOAD_INT_POS (4) 
#define JPEG_DEC_REG0X5E_DEC_EXT_RELOAD_INT_MASK (0x1) 

#define JPEG_DEC_REG0X5E_DEC_EXT_BIT_INT_POS (5) 
#define JPEG_DEC_REG0X5E_DEC_EXT_BIT_INT_MASK (0x1) 

#define JPEG_DEC_REG0X5E_DEC_TOTALBUSY_INT_POS (6) 
#define JPEG_DEC_REG0X5E_DEC_TOTALBUSY_INT_MASK (0x1) 

#define JPEG_DEC_REG0X5E_DEC_MCUFINISH_INT_POS (7) 
#define JPEG_DEC_REG0X5E_DEC_MCUFINISH_INT_MASK (0x1) 

#define JPEG_DEC_REG0X5E_DEC_FRAME_INT_POS (8) 
#define JPEG_DEC_REG0X5E_DEC_FRAME_INT_MASK (0x1) 

#define JPEG_DEC_REG0X5E_RESERVED_POS (9) 
#define JPEG_DEC_REG0X5E_RESERVED_MASK (0x7FFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x5e_value(void)
{
    return REG_READ(JPEG_DEC_REG0X5E_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x5e_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X5E_ADDR,value);
}

/* REG_0x5e:reg0x5e->dec_busy_int:0x5e[0],int enable; int_en[0] : dec_busy int; ,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5e_dec_busy_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5E_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X5E_DEC_BUSY_INT_POS) & JPEG_DEC_REG0X5E_DEC_BUSY_INT_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x5e_dec_busy_int(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5E_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X5E_DEC_BUSY_INT_MASK << JPEG_DEC_REG0X5E_DEC_BUSY_INT_POS);
    reg_value |= ((value & JPEG_DEC_REG0X5E_DEC_BUSY_INT_MASK) << JPEG_DEC_REG0X5E_DEC_BUSY_INT_POS);
    REG_WRITE(JPEG_DEC_REG0X5E_ADDR,reg_value);
}

/* REG_0x5e:reg0x5e->dec_rrload_int:0x5e[1],int_en[1] : dec_rrload int; ,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5e_dec_rrload_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5E_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X5E_DEC_RRLOAD_INT_POS) & JPEG_DEC_REG0X5E_DEC_RRLOAD_INT_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x5e_dec_rrload_int(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5E_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X5E_DEC_RRLOAD_INT_MASK << JPEG_DEC_REG0X5E_DEC_RRLOAD_INT_POS);
    reg_value |= ((value & JPEG_DEC_REG0X5E_DEC_RRLOAD_INT_MASK) << JPEG_DEC_REG0X5E_DEC_RRLOAD_INT_POS);
    REG_WRITE(JPEG_DEC_REG0X5E_ADDR,reg_value);
}

/* REG_0x5e:reg0x5e->dec_rload_int:0x5e[2],int_en[2] : dec_rload int; ,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5e_dec_rload_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5E_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X5E_DEC_RLOAD_INT_POS) & JPEG_DEC_REG0X5E_DEC_RLOAD_INT_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x5e_dec_rload_int(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5E_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X5E_DEC_RLOAD_INT_MASK << JPEG_DEC_REG0X5E_DEC_RLOAD_INT_POS);
    reg_value |= ((value & JPEG_DEC_REG0X5E_DEC_RLOAD_INT_MASK) << JPEG_DEC_REG0X5E_DEC_RLOAD_INT_POS);
    REG_WRITE(JPEG_DEC_REG0X5E_ADDR,reg_value);
}

/* REG_0x5e:reg0x5e->dec_search_int:0x5e[3],int_en[3] : dec_search int; ,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5e_dec_search_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5E_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X5E_DEC_SEARCH_INT_POS) & JPEG_DEC_REG0X5E_DEC_SEARCH_INT_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x5e_dec_search_int(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5E_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X5E_DEC_SEARCH_INT_MASK << JPEG_DEC_REG0X5E_DEC_SEARCH_INT_POS);
    reg_value |= ((value & JPEG_DEC_REG0X5E_DEC_SEARCH_INT_MASK) << JPEG_DEC_REG0X5E_DEC_SEARCH_INT_POS);
    REG_WRITE(JPEG_DEC_REG0X5E_ADDR,reg_value);
}

/* REG_0x5e:reg0x5e->dec_ext_reload_int:0x5e[4],int_en[4] : dec_ext_reload int; ,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5e_dec_ext_reload_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5E_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X5E_DEC_EXT_RELOAD_INT_POS) & JPEG_DEC_REG0X5E_DEC_EXT_RELOAD_INT_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x5e_dec_ext_reload_int(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5E_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X5E_DEC_EXT_RELOAD_INT_MASK << JPEG_DEC_REG0X5E_DEC_EXT_RELOAD_INT_POS);
    reg_value |= ((value & JPEG_DEC_REG0X5E_DEC_EXT_RELOAD_INT_MASK) << JPEG_DEC_REG0X5E_DEC_EXT_RELOAD_INT_POS);
    REG_WRITE(JPEG_DEC_REG0X5E_ADDR,reg_value);
}

/* REG_0x5e:reg0x5e->dec_ext_bit_int:0x5e[5],int_en[5] : dec_ext_bit int; ,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5e_dec_ext_bit_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5E_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X5E_DEC_EXT_BIT_INT_POS) & JPEG_DEC_REG0X5E_DEC_EXT_BIT_INT_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x5e_dec_ext_bit_int(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5E_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X5E_DEC_EXT_BIT_INT_MASK << JPEG_DEC_REG0X5E_DEC_EXT_BIT_INT_POS);
    reg_value |= ((value & JPEG_DEC_REG0X5E_DEC_EXT_BIT_INT_MASK) << JPEG_DEC_REG0X5E_DEC_EXT_BIT_INT_POS);
    REG_WRITE(JPEG_DEC_REG0X5E_ADDR,reg_value);
}

/* REG_0x5e:reg0x5e->dec_totalbusy_int:0x5e[6],int_en[6] : dec_totalbusy int //line en dec; ,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5e_dec_totalbusy_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5E_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X5E_DEC_TOTALBUSY_INT_POS) & JPEG_DEC_REG0X5E_DEC_TOTALBUSY_INT_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x5e_dec_totalbusy_int(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5E_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X5E_DEC_TOTALBUSY_INT_MASK << JPEG_DEC_REG0X5E_DEC_TOTALBUSY_INT_POS);
    reg_value |= ((value & JPEG_DEC_REG0X5E_DEC_TOTALBUSY_INT_MASK) << JPEG_DEC_REG0X5E_DEC_TOTALBUSY_INT_POS);
    REG_WRITE(JPEG_DEC_REG0X5E_ADDR,reg_value);
}

/* REG_0x5e:reg0x5e->dec_mcufinish_int:0x5e[7],int_en[7] : dec_mcufinish int; ,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5e_dec_mcufinish_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5E_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X5E_DEC_MCUFINISH_INT_POS) & JPEG_DEC_REG0X5E_DEC_MCUFINISH_INT_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x5e_dec_mcufinish_int(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5E_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X5E_DEC_MCUFINISH_INT_MASK << JPEG_DEC_REG0X5E_DEC_MCUFINISH_INT_POS);
    reg_value |= ((value & JPEG_DEC_REG0X5E_DEC_MCUFINISH_INT_MASK) << JPEG_DEC_REG0X5E_DEC_MCUFINISH_INT_POS);
    REG_WRITE(JPEG_DEC_REG0X5E_ADDR,reg_value);
}

/* REG_0x5e:reg0x5e->dec_frame_int:0x5e[8],int_en[8] : dec_frame int //auto dec,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5e_dec_frame_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5E_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X5E_DEC_FRAME_INT_POS) & JPEG_DEC_REG0X5E_DEC_FRAME_INT_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x5e_dec_frame_int(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5E_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X5E_DEC_FRAME_INT_MASK << JPEG_DEC_REG0X5E_DEC_FRAME_INT_POS);
    reg_value |= ((value & JPEG_DEC_REG0X5E_DEC_FRAME_INT_MASK) << JPEG_DEC_REG0X5E_DEC_FRAME_INT_POS);
    REG_WRITE(JPEG_DEC_REG0X5E_ADDR,reg_value);
}

/* REG_0x5F */
#define JPEG_DEC_REG0X5F_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x5F*4) //REG ADDR :0x4804017c
#define JPEG_DEC_REG0X5F_DEC_BUSY_INT_CLR_POS (0) 
#define JPEG_DEC_REG0X5F_DEC_BUSY_INT_CLR_MASK (0x1) 

#define JPEG_DEC_REG0X5F_DEC_RRLOAD_INT_CLR_POS (1) 
#define JPEG_DEC_REG0X5F_DEC_RRLOAD_INT_CLR_MASK (0x1) 

#define JPEG_DEC_REG0X5F_DEC_RLOAD_INT_CLR_POS (2) 
#define JPEG_DEC_REG0X5F_DEC_RLOAD_INT_CLR_MASK (0x1) 

#define JPEG_DEC_REG0X5F_DEC_SEARCH_INT_CLR_POS (3) 
#define JPEG_DEC_REG0X5F_DEC_SEARCH_INT_CLR_MASK (0x1) 

#define JPEG_DEC_REG0X5F_DEC_EXT_RELOAD_INT_CLR_POS (4) 
#define JPEG_DEC_REG0X5F_DEC_EXT_RELOAD_INT_CLR_MASK (0x1) 

#define JPEG_DEC_REG0X5F_DEC_EXT_BIT_INT_CLR_POS (5) 
#define JPEG_DEC_REG0X5F_DEC_EXT_BIT_INT_CLR_MASK (0x1) 

#define JPEG_DEC_REG0X5F_DEC_TOTALBUSY_INT_CLR_POS (6) 
#define JPEG_DEC_REG0X5F_DEC_TOTALBUSY_INT_CLR_MASK (0x1) 

#define JPEG_DEC_REG0X5F_DEC_MCUFINISH_INT_CLR_POS (7) 
#define JPEG_DEC_REG0X5F_DEC_MCUFINISH_INT_CLR_MASK (0x1) 

#define JPEG_DEC_REG0X5F_DEC_FRAME_INT_CLR_POS (8) 
#define JPEG_DEC_REG0X5F_DEC_FRAME_INT_CLR_MASK (0x1) 

#define JPEG_DEC_REG0X5F_RESERVED0_POS (9) 
#define JPEG_DEC_REG0X5F_RESERVED0_MASK (0x7FFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x5f_value(void)
{
    return REG_READ(JPEG_DEC_REG0X5F_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x5f_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X5F_ADDR,value);
}

/* REG_0x5f:reg0x5f->dec_busy_int_clr:0x5f[0],int enable; int_en[0] : dec_busy int; ,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5f_dec_busy_int_clr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5F_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X5F_DEC_BUSY_INT_CLR_POS) & JPEG_DEC_REG0X5F_DEC_BUSY_INT_CLR_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x5f_dec_busy_int_clr(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5F_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X5F_DEC_BUSY_INT_CLR_MASK << JPEG_DEC_REG0X5F_DEC_BUSY_INT_CLR_POS);
    reg_value |= ((value & JPEG_DEC_REG0X5F_DEC_BUSY_INT_CLR_MASK) << JPEG_DEC_REG0X5F_DEC_BUSY_INT_CLR_POS);
    REG_WRITE(JPEG_DEC_REG0X5F_ADDR,reg_value);
}

/* REG_0x5f:reg0x5f->dec_rrload_int_clr:0x5f[1],int_en[1] : dec_rrload int; ,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5f_dec_rrload_int_clr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5F_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X5F_DEC_RRLOAD_INT_CLR_POS) & JPEG_DEC_REG0X5F_DEC_RRLOAD_INT_CLR_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x5f_dec_rrload_int_clr(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5F_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X5F_DEC_RRLOAD_INT_CLR_MASK << JPEG_DEC_REG0X5F_DEC_RRLOAD_INT_CLR_POS);
    reg_value |= ((value & JPEG_DEC_REG0X5F_DEC_RRLOAD_INT_CLR_MASK) << JPEG_DEC_REG0X5F_DEC_RRLOAD_INT_CLR_POS);
    REG_WRITE(JPEG_DEC_REG0X5F_ADDR,reg_value);
}

/* REG_0x5f:reg0x5f->dec_rload_int_clr:0x5f[2],int_en[2] : dec_rload int; ,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5f_dec_rload_int_clr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5F_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X5F_DEC_RLOAD_INT_CLR_POS) & JPEG_DEC_REG0X5F_DEC_RLOAD_INT_CLR_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x5f_dec_rload_int_clr(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5F_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X5F_DEC_RLOAD_INT_CLR_MASK << JPEG_DEC_REG0X5F_DEC_RLOAD_INT_CLR_POS);
    reg_value |= ((value & JPEG_DEC_REG0X5F_DEC_RLOAD_INT_CLR_MASK) << JPEG_DEC_REG0X5F_DEC_RLOAD_INT_CLR_POS);
    REG_WRITE(JPEG_DEC_REG0X5F_ADDR,reg_value);
}

/* REG_0x5f:reg0x5f->dec_search_int_clr:0x5f[3],int_en[3] : dec_search int; ,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5f_dec_search_int_clr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5F_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X5F_DEC_SEARCH_INT_CLR_POS) & JPEG_DEC_REG0X5F_DEC_SEARCH_INT_CLR_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x5f_dec_search_int_clr(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5F_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X5F_DEC_SEARCH_INT_CLR_MASK << JPEG_DEC_REG0X5F_DEC_SEARCH_INT_CLR_POS);
    reg_value |= ((value & JPEG_DEC_REG0X5F_DEC_SEARCH_INT_CLR_MASK) << JPEG_DEC_REG0X5F_DEC_SEARCH_INT_CLR_POS);
    REG_WRITE(JPEG_DEC_REG0X5F_ADDR,reg_value);
}

/* REG_0x5f:reg0x5f->dec_ext_reload_int_clr:0x5f[4],int_en[4] : dec_ext_reload int; ,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5f_dec_ext_reload_int_clr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5F_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X5F_DEC_EXT_RELOAD_INT_CLR_POS) & JPEG_DEC_REG0X5F_DEC_EXT_RELOAD_INT_CLR_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x5f_dec_ext_reload_int_clr(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5F_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X5F_DEC_EXT_RELOAD_INT_CLR_MASK << JPEG_DEC_REG0X5F_DEC_EXT_RELOAD_INT_CLR_POS);
    reg_value |= ((value & JPEG_DEC_REG0X5F_DEC_EXT_RELOAD_INT_CLR_MASK) << JPEG_DEC_REG0X5F_DEC_EXT_RELOAD_INT_CLR_POS);
    REG_WRITE(JPEG_DEC_REG0X5F_ADDR,reg_value);
}

/* REG_0x5f:reg0x5f->dec_ext_bit_int_clr:0x5f[5],int_en[5] : dec_ext_bit int; ,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5f_dec_ext_bit_int_clr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5F_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X5F_DEC_EXT_BIT_INT_CLR_POS) & JPEG_DEC_REG0X5F_DEC_EXT_BIT_INT_CLR_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x5f_dec_ext_bit_int_clr(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5F_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X5F_DEC_EXT_BIT_INT_CLR_MASK << JPEG_DEC_REG0X5F_DEC_EXT_BIT_INT_CLR_POS);
    reg_value |= ((value & JPEG_DEC_REG0X5F_DEC_EXT_BIT_INT_CLR_MASK) << JPEG_DEC_REG0X5F_DEC_EXT_BIT_INT_CLR_POS);
    REG_WRITE(JPEG_DEC_REG0X5F_ADDR,reg_value);
}

/* REG_0x5f:reg0x5f->dec_totalbusy_int_clr:0x5f[6],int_en[6] : dec_totalbusy int //line en dec; ,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5f_dec_totalbusy_int_clr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5F_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X5F_DEC_TOTALBUSY_INT_CLR_POS) & JPEG_DEC_REG0X5F_DEC_TOTALBUSY_INT_CLR_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x5f_dec_totalbusy_int_clr(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5F_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X5F_DEC_TOTALBUSY_INT_CLR_MASK << JPEG_DEC_REG0X5F_DEC_TOTALBUSY_INT_CLR_POS);
    reg_value |= ((value & JPEG_DEC_REG0X5F_DEC_TOTALBUSY_INT_CLR_MASK) << JPEG_DEC_REG0X5F_DEC_TOTALBUSY_INT_CLR_POS);
    REG_WRITE(JPEG_DEC_REG0X5F_ADDR,reg_value);
}

/* REG_0x5f:reg0x5f->dec_mcufinish_int_clr:0x5f[7],int_en[7] : dec_mcufinish int; ,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5f_dec_mcufinish_int_clr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5F_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X5F_DEC_MCUFINISH_INT_CLR_POS) & JPEG_DEC_REG0X5F_DEC_MCUFINISH_INT_CLR_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x5f_dec_mcufinish_int_clr(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5F_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X5F_DEC_MCUFINISH_INT_CLR_MASK << JPEG_DEC_REG0X5F_DEC_MCUFINISH_INT_CLR_POS);
    reg_value |= ((value & JPEG_DEC_REG0X5F_DEC_MCUFINISH_INT_CLR_MASK) << JPEG_DEC_REG0X5F_DEC_MCUFINISH_INT_CLR_POS);
    REG_WRITE(JPEG_DEC_REG0X5F_ADDR,reg_value);
}

/* REG_0x5f:reg0x5f->dec_frame_int_clr:0x5f[8],int_en[8] : dec_frame int //auto dec,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x5f_dec_frame_int_clr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5F_ADDR);
    reg_value = ((reg_value >> JPEG_DEC_REG0X5F_DEC_FRAME_INT_CLR_POS) & JPEG_DEC_REG0X5F_DEC_FRAME_INT_CLR_MASK);
    return reg_value;
}

static inline void jpeg_dec_ll_set_reg0x5f_dec_frame_int_clr(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_DEC_REG0X5F_ADDR);
    reg_value &= ~(JPEG_DEC_REG0X5F_DEC_FRAME_INT_CLR_MASK << JPEG_DEC_REG0X5F_DEC_FRAME_INT_CLR_POS);
    reg_value |= ((value & JPEG_DEC_REG0X5F_DEC_FRAME_INT_CLR_MASK) << JPEG_DEC_REG0X5F_DEC_FRAME_INT_CLR_POS);
    REG_WRITE(JPEG_DEC_REG0X5F_ADDR,reg_value);
}

/* REG_0x80 */
#define JPEG_DEC_REG0X80_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x80*4) //REG ADDR :0x48040200
#define JPEG_DEC_REG0X80_HUFDC_Y_POS (0) 
#define JPEG_DEC_REG0X80_HUFDC_Y_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x80_value(void)
{
    return REG_READ(JPEG_DEC_REG0X80_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x80_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X80_ADDR,value);
}

/* REG_0x80:reg0x80->hufdc_y:0x80,huffman table dc of y,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x80_hufdc_y(void)
{
    return REG_READ(JPEG_DEC_REG0X80_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x80_hufdc_y(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X80_ADDR,value);
}

/* REG_0xC0 */
#define JPEG_DEC_REG0XC0_ADDR  (JPEG_DEC_LL_REG_BASE  + 0xC0*4) //REG ADDR :0x48040300
#define JPEG_DEC_REG0XC0_HUFDC_UV_POS (0) 
#define JPEG_DEC_REG0XC0_HUFDC_UV_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0xc0_value(void)
{
    return REG_READ(JPEG_DEC_REG0XC0_ADDR);
}

static inline void jpeg_dec_ll_set_reg0xc0_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0XC0_ADDR,value);
}

/* REG_0xc0:reg0xc0->hufdc_uv:0xc0,huffman table dc of uv,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0xc0_hufdc_uv(void)
{
    return REG_READ(JPEG_DEC_REG0XC0_ADDR);
}

static inline void jpeg_dec_ll_set_reg0xc0_hufdc_uv(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0XC0_ADDR,value);
}

/* REG_0x100 */
#define JPEG_DEC_REG0X100_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x100*4) //REG ADDR :0x48040400
#define JPEG_DEC_REG0X100_HUFAC_Y_POS (0) 
#define JPEG_DEC_REG0X100_HUFAC_Y_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x100_value(void)
{
    return REG_READ(JPEG_DEC_REG0X100_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x100_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X100_ADDR,value);
}

/* REG_0x100:reg0x100->hufac_y:0x100,huffman table ac of y,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x100_hufac_y(void)
{
    return REG_READ(JPEG_DEC_REG0X100_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x100_hufac_y(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X100_ADDR,value);
}

/* REG_0x200 */
#define JPEG_DEC_REG0X200_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x200*4) //REG ADDR :0x48040800
#define JPEG_DEC_REG0X200_HUFAC_UV_POS (0) 
#define JPEG_DEC_REG0X200_HUFAC_UV_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x200_value(void)
{
    return REG_READ(JPEG_DEC_REG0X200_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x200_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X200_ADDR,value);
}

/* REG_0x200:reg0x200->hufac_uv:0x200,huffman table ac of uv,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x200_hufac_uv(void)
{
    return REG_READ(JPEG_DEC_REG0X200_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x200_hufac_uv(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X200_ADDR,value);
}

/* REG_0x300 */
#define JPEG_DEC_REG0X300_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x300*4) //REG ADDR :0x48040c00
#define JPEG_DEC_REG0X300_ZIGZAG_BUF_POS (0) 
#define JPEG_DEC_REG0X300_ZIGZAG_BUF_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x300_value(void)
{
    return REG_READ(JPEG_DEC_REG0X300_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x300_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X300_ADDR,value);
}

/* REG_0x300:reg0x300->zigzag_buf:0x300,zigzag buf,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x300_zigzag_buf(void)
{
    return REG_READ(JPEG_DEC_REG0X300_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x300_zigzag_buf(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X300_ADDR,value);
}

/* REG_0x340 */
#define JPEG_DEC_REG0X340_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x340*4) //REG ADDR :0x48040d00
#define JPEG_DEC_REG0X340_DCT_TEMP_BUF_POS (0) 
#define JPEG_DEC_REG0X340_DCT_TEMP_BUF_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x340_value(void)
{
    return REG_READ(JPEG_DEC_REG0X340_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x340_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X340_ADDR,value);
}

/* REG_0x340:reg0x340->dct_temp_buf:0x340,dct_temp_buf,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x340_dct_temp_buf(void)
{
    return REG_READ(JPEG_DEC_REG0X340_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x340_dct_temp_buf(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X340_ADDR,value);
}

/* REG_0x380 */
#define JPEG_DEC_REG0X380_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x380*4) //REG ADDR :0x48040e00
#define JPEG_DEC_REG0X380_DQT_TABLE_DC_POS (0) 
#define JPEG_DEC_REG0X380_DQT_TABLE_DC_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x380_value(void)
{
    return REG_READ(JPEG_DEC_REG0X380_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x380_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X380_ADDR,value);
}

/* REG_0x380:reg0x380->DQT_table_dc:0x380,DQT_table_dc,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x380_dqt_table_dc(void)
{
    return REG_READ(JPEG_DEC_REG0X380_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x380_dqt_table_dc(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X380_ADDR,value);
}

/* REG_0x3C0 */
#define JPEG_DEC_REG0X3C0_ADDR  (JPEG_DEC_LL_REG_BASE  + 0x3C0*4) //REG ADDR :0x48040f00
#define JPEG_DEC_REG0X3C0_DQT_TABLE_AC_POS (0) 
#define JPEG_DEC_REG0X3C0_DQT_TABLE_AC_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_dec_ll_get_reg0x3c0_value(void)
{
    return REG_READ(JPEG_DEC_REG0X3C0_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x3c0_value(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X3C0_ADDR,value);
}

/* REG_0x3c0:reg0x3c0->DQT_table_ac:0x3c0,DQT_table_ac,0x0,R/W*/
static inline uint32_t jpeg_dec_ll_get_reg0x3c0_dqt_table_ac(void)
{
    return REG_READ(JPEG_DEC_REG0X3C0_ADDR);
}

static inline void jpeg_dec_ll_set_reg0x3c0_dqt_table_ac(uint32_t value)
{
    REG_WRITE(JPEG_DEC_REG0X3C0_ADDR,value);
}

#ifdef __cplusplus 
}                  
#endif             
