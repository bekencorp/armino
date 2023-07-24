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
* This file is generated from BK7236_ADDR_Mapping_s.xlsm automatically                                
* Modify it manually is not recommended                                       
* CHIP ID:BK7236,GENARATE TIME:2023-03-20 20:21:54                                                 
************************************************************************************************************************************/

#pragma once                 
                            
#ifdef __cplusplus          
extern "C" {              
#endif                      

#include <soc/soc.h>          

#define ROTT_LL_REG_BASE      (SOC_ROTT_REG_BASE) //REG_BASE:0x480C0000

/* REG_0x00 */
#define ROTT_DEVICE_ID_ADDR  (ROTT_LL_REG_BASE  + 0x0*4) //REG ADDR :0x480c0000
#define ROTT_DEVICE_ID_DEVICE_ID_POS (0) 
#define ROTT_DEVICE_ID_DEVICE_ID_MASK (0xFFFFFFFF) 

static inline uint32_t rott_ll_get_device_id_value(void)
{
    return REG_READ(ROTT_DEVICE_ID_ADDR);
}

/* REG_0x00:device_id->device_id:0x0[31:0],Device ID:ASCII Code "ROTT",0x524f5454,R*/
static inline uint32_t rott_ll_get_device_id_device_id(void)
{
    return REG_READ(ROTT_DEVICE_ID_ADDR);
}

/* REG_0x01 */
#define ROTT_VERSION_ID_ADDR  (ROTT_LL_REG_BASE  + 0x1*4) //REG ADDR :0x480c0004
#define ROTT_VERSION_ID_VERSION_ID_POS (0) 
#define ROTT_VERSION_ID_VERSION_ID_MASK (0xFFFFFFFF) 

static inline uint32_t rott_ll_get_version_id_value(void)
{
    return REG_READ(ROTT_VERSION_ID_ADDR);
}

/* REG_0x01:version_id->version_id:0x1[31:0],Version ID:V1.1,0x00010001,R*/
static inline uint32_t rott_ll_get_version_id_version_id(void)
{
    return REG_READ(ROTT_VERSION_ID_ADDR);
}

/* REG_0x02 */
#define ROTT_MODULE_CONTOL_ADDR  (ROTT_LL_REG_BASE  + 0x2*4) //REG ADDR :0x480c0008
#define ROTT_MODULE_CONTOL_SOFT_RESET_POS (0) 
#define ROTT_MODULE_CONTOL_SOFT_RESET_MASK (0x1) 

#define ROTT_MODULE_CONTOL_CLK_GATE_POS (1) 
#define ROTT_MODULE_CONTOL_CLK_GATE_MASK (0x1) 

#define ROTT_MODULE_CONTOL_RESERVED0_POS (2) 
#define ROTT_MODULE_CONTOL_RESERVED0_MASK (0x3FFFFFFF) 

static inline uint32_t rott_ll_get_module_contol_value(void)
{
    return REG_READ(ROTT_MODULE_CONTOL_ADDR);
}

static inline void rott_ll_set_module_contol_value(uint32_t value)
{
    REG_WRITE(ROTT_MODULE_CONTOL_ADDR,value);
}

/* REG_0x02:module_contol->soft_reset:0x2[0],enabwrite 0 to clear, can't reset automaticly.,0x1,R/W */
static inline uint32_t rott_ll_get_module_contol_soft_reset(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_MODULE_CONTOL_ADDR);
    reg_value = ((reg_value >> ROTT_MODULE_CONTOL_SOFT_RESET_POS) & ROTT_MODULE_CONTOL_SOFT_RESET_MASK);
    return reg_value;
}

static inline void rott_ll_set_module_contol_soft_reset(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_MODULE_CONTOL_ADDR);
    reg_value &= ~(ROTT_MODULE_CONTOL_SOFT_RESET_MASK << ROTT_MODULE_CONTOL_SOFT_RESET_POS);
    reg_value |= ((value & ROTT_MODULE_CONTOL_SOFT_RESET_MASK) << ROTT_MODULE_CONTOL_SOFT_RESET_POS);
    REG_WRITE(ROTT_MODULE_CONTOL_ADDR,reg_value);
}

/* REG_0x02:module_contol->clk_gate:0x2[1],0x0: use ahb clock gate; 0x1: disable ahb clock gate,0x0,R/W*/
static inline uint32_t rott_ll_get_module_contol_clk_gate(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_MODULE_CONTOL_ADDR);
    reg_value = ((reg_value >> ROTT_MODULE_CONTOL_CLK_GATE_POS) & ROTT_MODULE_CONTOL_CLK_GATE_MASK);
    return reg_value;
}

static inline void rott_ll_set_module_contol_clk_gate(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_MODULE_CONTOL_ADDR);
    reg_value &= ~(ROTT_MODULE_CONTOL_CLK_GATE_MASK << ROTT_MODULE_CONTOL_CLK_GATE_POS);
    reg_value |= ((value & ROTT_MODULE_CONTOL_CLK_GATE_MASK) << ROTT_MODULE_CONTOL_CLK_GATE_POS);
    REG_WRITE(ROTT_MODULE_CONTOL_ADDR,reg_value);
}

/* REG_0x03 */
#define ROTT_GLOBAL_STATUS_ADDR  (ROTT_LL_REG_BASE  + 0x3*4) //REG ADDR :0x480c000c
#define ROTT_GLOBAL_STATUS_GLOBAL_STATUS_POS (0) 
#define ROTT_GLOBAL_STATUS_GLOBAL_STATUS_MASK (0xFFFFFFFF) 

static inline uint32_t rott_ll_get_global_status_value(void)
{
    return REG_READ(ROTT_GLOBAL_STATUS_ADDR);
}

/* REG_0x03:global_status->global_Status:0x3[31:0], ,0x0,R*/
static inline uint32_t rott_ll_get_global_status_global_status(void)
{
    return REG_READ(ROTT_GLOBAL_STATUS_ADDR);
}

/* REG_0x04 */
#define ROTT_ROTATE_CTRL_ADDR  (ROTT_LL_REG_BASE  + 0x4*4) //REG ADDR :0x480c0010
#define ROTT_ROTATE_CTRL_ROTATE_ENA_POS (0) 
#define ROTT_ROTATE_CTRL_ROTATE_ENA_MASK (0x1) 

#define ROTT_ROTATE_CTRL_ROTATE_BPS_POS (1) 
#define ROTT_ROTATE_CTRL_ROTATE_BPS_MASK (0x1) 

#define ROTT_ROTATE_CTRL_RESERVED0_POS (2) 
#define ROTT_ROTATE_CTRL_RESERVED0_MASK (0x3) 

#define ROTT_ROTATE_CTRL_ROTATE_DONE_INT_ENA_POS (4) 
#define ROTT_ROTATE_CTRL_ROTATE_DONE_INT_ENA_MASK (0x1) 

#define ROTT_ROTATE_CTRL_ROTATE_WTMK_INT_ENA_POS (5) 
#define ROTT_ROTATE_CTRL_ROTATE_WTMK_INT_ENA_MASK (0x1) 

#define ROTT_ROTATE_CTRL_ROTATE_CFG_ERR_INT_ENA_POS (6) 
#define ROTT_ROTATE_CTRL_ROTATE_CFG_ERR_INT_ENA_MASK (0x1) 

#define ROTT_ROTATE_CTRL_RESERVED_POS (7) 
#define ROTT_ROTATE_CTRL_RESERVED_MASK (0x1FFFFFF) 

static inline uint32_t rott_ll_get_rotate_ctrl_value(void)
{
    return REG_READ(ROTT_ROTATE_CTRL_ADDR);
}

static inline void rott_ll_set_rotate_ctrl_value(uint32_t value)
{
    REG_WRITE(ROTT_ROTATE_CTRL_ADDR,value);
}

/* REG_0x04:rotate_ctrl->rotate_ena:0x4[0],rotate module enable.,0x0,r/w*/
static inline uint32_t rott_ll_get_rotate_ctrl_rotate_ena(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_ROTATE_CTRL_ADDR);
    reg_value = ((reg_value >> ROTT_ROTATE_CTRL_ROTATE_ENA_POS) & ROTT_ROTATE_CTRL_ROTATE_ENA_MASK);
    return reg_value;
}

static inline void rott_ll_set_rotate_ctrl_rotate_ena(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_ROTATE_CTRL_ADDR);
    reg_value &= ~(ROTT_ROTATE_CTRL_ROTATE_ENA_MASK << ROTT_ROTATE_CTRL_ROTATE_ENA_POS);
    reg_value |= ((value & ROTT_ROTATE_CTRL_ROTATE_ENA_MASK) << ROTT_ROTATE_CTRL_ROTATE_ENA_POS);
    REG_WRITE(ROTT_ROTATE_CTRL_ADDR,reg_value);
}

/* REG_0x04:rotate_ctrl->rotate_bps:0x4[1],bypass rotating, only use yuv2rgb565 pixel formart convert function.,0x0,r/w*/
static inline uint32_t rott_ll_get_rotate_ctrl_rotate_bps(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_ROTATE_CTRL_ADDR);
    reg_value = ((reg_value >> ROTT_ROTATE_CTRL_ROTATE_BPS_POS) & ROTT_ROTATE_CTRL_ROTATE_BPS_MASK);
    return reg_value;
}

static inline void rott_ll_set_rotate_ctrl_rotate_bps(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_ROTATE_CTRL_ADDR);
    reg_value &= ~(ROTT_ROTATE_CTRL_ROTATE_BPS_MASK << ROTT_ROTATE_CTRL_ROTATE_BPS_POS);
    reg_value |= ((value & ROTT_ROTATE_CTRL_ROTATE_BPS_MASK) << ROTT_ROTATE_CTRL_ROTATE_BPS_POS);
    REG_WRITE(ROTT_ROTATE_CTRL_ADDR,reg_value);
}

/* REG_0x04:rotate_ctrl->rotate_done_int_ena:0x4[4],rotate finish int enable.,0x0,r/w*/
static inline uint32_t rott_ll_get_rotate_ctrl_rotate_done_int_ena(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_ROTATE_CTRL_ADDR);
    reg_value = ((reg_value >> ROTT_ROTATE_CTRL_ROTATE_DONE_INT_ENA_POS) & ROTT_ROTATE_CTRL_ROTATE_DONE_INT_ENA_MASK);
    return reg_value;
}

static inline void rott_ll_set_rotate_ctrl_rotate_done_int_ena(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_ROTATE_CTRL_ADDR);
    reg_value &= ~(ROTT_ROTATE_CTRL_ROTATE_DONE_INT_ENA_MASK << ROTT_ROTATE_CTRL_ROTATE_DONE_INT_ENA_POS);
    reg_value |= ((value & ROTT_ROTATE_CTRL_ROTATE_DONE_INT_ENA_MASK) << ROTT_ROTATE_CTRL_ROTATE_DONE_INT_ENA_POS);
    REG_WRITE(ROTT_ROTATE_CTRL_ADDR,reg_value);
}

/* REG_0x04:rotate_ctrl->rotate_wtmk_int_ena:0x4[5],rotate watermark int enable.,0x0,r/w*/
static inline uint32_t rott_ll_get_rotate_ctrl_rotate_wtmk_int_ena(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_ROTATE_CTRL_ADDR);
    reg_value = ((reg_value >> ROTT_ROTATE_CTRL_ROTATE_WTMK_INT_ENA_POS) & ROTT_ROTATE_CTRL_ROTATE_WTMK_INT_ENA_MASK);
    return reg_value;
}

static inline void rott_ll_set_rotate_ctrl_rotate_wtmk_int_ena(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_ROTATE_CTRL_ADDR);
    reg_value &= ~(ROTT_ROTATE_CTRL_ROTATE_WTMK_INT_ENA_MASK << ROTT_ROTATE_CTRL_ROTATE_WTMK_INT_ENA_POS);
    reg_value |= ((value & ROTT_ROTATE_CTRL_ROTATE_WTMK_INT_ENA_MASK) << ROTT_ROTATE_CTRL_ROTATE_WTMK_INT_ENA_POS);
    REG_WRITE(ROTT_ROTATE_CTRL_ADDR,reg_value);
}

/* REG_0x04:rotate_ctrl->rotate_cfg_err_int_ena:0x4[6],rotate config error int enable.,0x0,r/w*/
static inline uint32_t rott_ll_get_rotate_ctrl_rotate_cfg_err_int_ena(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_ROTATE_CTRL_ADDR);
    reg_value = ((reg_value >> ROTT_ROTATE_CTRL_ROTATE_CFG_ERR_INT_ENA_POS) & ROTT_ROTATE_CTRL_ROTATE_CFG_ERR_INT_ENA_MASK);
    return reg_value;
}

static inline void rott_ll_set_rotate_ctrl_rotate_cfg_err_int_ena(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_ROTATE_CTRL_ADDR);
    reg_value &= ~(ROTT_ROTATE_CTRL_ROTATE_CFG_ERR_INT_ENA_MASK << ROTT_ROTATE_CTRL_ROTATE_CFG_ERR_INT_ENA_POS);
    reg_value |= ((value & ROTT_ROTATE_CTRL_ROTATE_CFG_ERR_INT_ENA_MASK) << ROTT_ROTATE_CTRL_ROTATE_CFG_ERR_INT_ENA_POS);
    REG_WRITE(ROTT_ROTATE_CTRL_ADDR,reg_value);
}

/* REG_0x05 */
#define ROTT_ROTATE_FMT_ADDR  (ROTT_LL_REG_BASE  + 0x5*4) //REG ADDR :0x480c0014
#define ROTT_ROTATE_FMT_PFC_I_HWORD_REVE_POS (0) 
#define ROTT_ROTATE_FMT_PFC_I_HWORD_REVE_MASK (0x1) 

#define ROTT_ROTATE_FMT_PFC_I_BYTE_REVE_POS (1) 
#define ROTT_ROTATE_FMT_PFC_I_BYTE_REVE_MASK (0x1) 

#define ROTT_ROTATE_FMT_PFC_O_HWORD_REVE_POS (2) 
#define ROTT_ROTATE_FMT_PFC_O_HWORD_REVE_MASK (0x1) 

#define ROTT_ROTATE_FMT_ROTATE_ANTICLOCK_POS (3) 
#define ROTT_ROTATE_FMT_ROTATE_ANTICLOCK_MASK (0x1) 

#define ROTT_ROTATE_FMT_YUV_FMT_POS (4) 
#define ROTT_ROTATE_FMT_YUV_FMT_MASK (0x7) 

#define ROTT_ROTATE_FMT_RESERVED0_POS (7) 
#define ROTT_ROTATE_FMT_RESERVED0_MASK (0x1FFFFFF) 

static inline uint32_t rott_ll_get_rotate_fmt_value(void)
{
    return REG_READ(ROTT_ROTATE_FMT_ADDR);
}

static inline void rott_ll_set_rotate_fmt_value(uint32_t value)
{
    REG_WRITE(ROTT_ROTATE_FMT_ADDR,value);
}

/* REG_0x05:rotate_fmt->pfc_i_hword_reve:0x5[0],yuv pixel format convert input data revese halfword by halfword.,0x0,r/w*/
static inline uint32_t rott_ll_get_rotate_fmt_pfc_i_hword_reve(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_ROTATE_FMT_ADDR);
    reg_value = ((reg_value >> ROTT_ROTATE_FMT_PFC_I_HWORD_REVE_POS) & ROTT_ROTATE_FMT_PFC_I_HWORD_REVE_MASK);
    return reg_value;
}

static inline void rott_ll_set_rotate_fmt_pfc_i_hword_reve(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_ROTATE_FMT_ADDR);
    reg_value &= ~(ROTT_ROTATE_FMT_PFC_I_HWORD_REVE_MASK << ROTT_ROTATE_FMT_PFC_I_HWORD_REVE_POS);
    reg_value |= ((value & ROTT_ROTATE_FMT_PFC_I_HWORD_REVE_MASK) << ROTT_ROTATE_FMT_PFC_I_HWORD_REVE_POS);
    REG_WRITE(ROTT_ROTATE_FMT_ADDR,reg_value);
}

/* REG_0x05:rotate_fmt->pfc_i_byte_reve:0x5[1],yuv pixel format convert input data revese byte by byte.,0x0,r/w*/
static inline uint32_t rott_ll_get_rotate_fmt_pfc_i_byte_reve(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_ROTATE_FMT_ADDR);
    reg_value = ((reg_value >> ROTT_ROTATE_FMT_PFC_I_BYTE_REVE_POS) & ROTT_ROTATE_FMT_PFC_I_BYTE_REVE_MASK);
    return reg_value;
}

static inline void rott_ll_set_rotate_fmt_pfc_i_byte_reve(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_ROTATE_FMT_ADDR);
    reg_value &= ~(ROTT_ROTATE_FMT_PFC_I_BYTE_REVE_MASK << ROTT_ROTATE_FMT_PFC_I_BYTE_REVE_POS);
    reg_value |= ((value & ROTT_ROTATE_FMT_PFC_I_BYTE_REVE_MASK) << ROTT_ROTATE_FMT_PFC_I_BYTE_REVE_POS);
    REG_WRITE(ROTT_ROTATE_FMT_ADDR,reg_value);
}

/* REG_0x05:rotate_fmt->pfc_o_hword_reve:0x5[2],yuv pixel format convert output data reverse halfword by halfword.,0x0,r/w*/
static inline uint32_t rott_ll_get_rotate_fmt_pfc_o_hword_reve(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_ROTATE_FMT_ADDR);
    reg_value = ((reg_value >> ROTT_ROTATE_FMT_PFC_O_HWORD_REVE_POS) & ROTT_ROTATE_FMT_PFC_O_HWORD_REVE_MASK);
    return reg_value;
}

static inline void rott_ll_set_rotate_fmt_pfc_o_hword_reve(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_ROTATE_FMT_ADDR);
    reg_value &= ~(ROTT_ROTATE_FMT_PFC_O_HWORD_REVE_MASK << ROTT_ROTATE_FMT_PFC_O_HWORD_REVE_POS);
    reg_value |= ((value & ROTT_ROTATE_FMT_PFC_O_HWORD_REVE_MASK) << ROTT_ROTATE_FMT_PFC_O_HWORD_REVE_POS);
    REG_WRITE(ROTT_ROTATE_FMT_ADDR,reg_value);
}

/* REG_0x05:rotate_fmt->rotate_anticlock:0x5[3],rotate with anticlock rotating.,0x0,r/w*/
static inline uint32_t rott_ll_get_rotate_fmt_rotate_anticlock(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_ROTATE_FMT_ADDR);
    reg_value = ((reg_value >> ROTT_ROTATE_FMT_ROTATE_ANTICLOCK_POS) & ROTT_ROTATE_FMT_ROTATE_ANTICLOCK_MASK);
    return reg_value;
}

static inline void rott_ll_set_rotate_fmt_rotate_anticlock(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_ROTATE_FMT_ADDR);
    reg_value &= ~(ROTT_ROTATE_FMT_ROTATE_ANTICLOCK_MASK << ROTT_ROTATE_FMT_ROTATE_ANTICLOCK_POS);
    reg_value |= ((value & ROTT_ROTATE_FMT_ROTATE_ANTICLOCK_MASK) << ROTT_ROTATE_FMT_ROTATE_ANTICLOCK_POS);
    REG_WRITE(ROTT_ROTATE_FMT_ADDR,reg_value);
}

/* REG_0x05:rotate_fmt->yuv_fmt:0x5[6:4],yuv dat foramt: 000：default，rgb565；001：YUYV；010: UYVY；011: YYUV；100: UVYY；101: VUYY,0x0,r/w*/
static inline uint32_t rott_ll_get_rotate_fmt_yuv_fmt(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_ROTATE_FMT_ADDR);
    reg_value = ((reg_value >> ROTT_ROTATE_FMT_YUV_FMT_POS) & ROTT_ROTATE_FMT_YUV_FMT_MASK);
    return reg_value;
}

static inline void rott_ll_set_rotate_fmt_yuv_fmt(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_ROTATE_FMT_ADDR);
    reg_value &= ~(ROTT_ROTATE_FMT_YUV_FMT_MASK << ROTT_ROTATE_FMT_YUV_FMT_POS);
    reg_value |= ((value & ROTT_ROTATE_FMT_YUV_FMT_MASK) << ROTT_ROTATE_FMT_YUV_FMT_POS);
    REG_WRITE(ROTT_ROTATE_FMT_ADDR,reg_value);
}

/* REG_0x06 */
#define ROTT_BLOCK_RESOLU_ADDR  (ROTT_LL_REG_BASE  + 0x6*4) //REG ADDR :0x480c0018
#define ROTT_BLOCK_RESOLU_BLK_LINE_PIXEL_POS (0) 
#define ROTT_BLOCK_RESOLU_BLK_LINE_PIXEL_MASK (0x7FF) 

#define ROTT_BLOCK_RESOLU_RESERVED1_POS (11) 
#define ROTT_BLOCK_RESOLU_RESERVED1_MASK (0x1) 

#define ROTT_BLOCK_RESOLU_BLK_CLUM_PIXEL_POS (12) 
#define ROTT_BLOCK_RESOLU_BLK_CLUM_PIXEL_MASK (0x7FF) 

#define ROTT_BLOCK_RESOLU_RESERVED0_POS (23) 
#define ROTT_BLOCK_RESOLU_RESERVED0_MASK (0x1FF) 

static inline uint32_t rott_ll_get_block_resolu_value(void)
{
    return REG_READ(ROTT_BLOCK_RESOLU_ADDR);
}

static inline void rott_ll_set_block_resolu_value(uint32_t value)
{
    REG_WRITE(ROTT_BLOCK_RESOLU_ADDR,value);
}

/* REG_0x06:block_resolu->blk_line_pixel:0x6[10:0],rotate block line pixel count.,0x0,r/w*/
static inline uint32_t rott_ll_get_block_resolu_blk_line_pixel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_BLOCK_RESOLU_ADDR);
    reg_value = ((reg_value >> ROTT_BLOCK_RESOLU_BLK_LINE_PIXEL_POS) & ROTT_BLOCK_RESOLU_BLK_LINE_PIXEL_MASK);
    return reg_value;
}

static inline void rott_ll_set_block_resolu_blk_line_pixel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_BLOCK_RESOLU_ADDR);
    reg_value &= ~(ROTT_BLOCK_RESOLU_BLK_LINE_PIXEL_MASK << ROTT_BLOCK_RESOLU_BLK_LINE_PIXEL_POS);
    reg_value |= ((value & ROTT_BLOCK_RESOLU_BLK_LINE_PIXEL_MASK) << ROTT_BLOCK_RESOLU_BLK_LINE_PIXEL_POS);
    REG_WRITE(ROTT_BLOCK_RESOLU_ADDR,reg_value);
}

/* REG_0x06:block_resolu->blk_clum_pixel:0x6[22:12],rotate block clum pixel count. warming! Block max pixel is 4800pixel, clum_pixel * line_pixel must less than 4800.l ,0x0,r/w*/
static inline uint32_t rott_ll_get_block_resolu_blk_clum_pixel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_BLOCK_RESOLU_ADDR);
    reg_value = ((reg_value >> ROTT_BLOCK_RESOLU_BLK_CLUM_PIXEL_POS) & ROTT_BLOCK_RESOLU_BLK_CLUM_PIXEL_MASK);
    return reg_value;
}

static inline void rott_ll_set_block_resolu_blk_clum_pixel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_BLOCK_RESOLU_ADDR);
    reg_value &= ~(ROTT_BLOCK_RESOLU_BLK_CLUM_PIXEL_MASK << ROTT_BLOCK_RESOLU_BLK_CLUM_PIXEL_POS);
    reg_value |= ((value & ROTT_BLOCK_RESOLU_BLK_CLUM_PIXEL_MASK) << ROTT_BLOCK_RESOLU_BLK_CLUM_PIXEL_POS);
    REG_WRITE(ROTT_BLOCK_RESOLU_ADDR,reg_value);
}

/* REG_0x07 */
#define ROTT_RD_ADDR_ADDR  (ROTT_LL_REG_BASE  + 0x7*4) //REG ADDR :0x480c001c
#define ROTT_RD_ADDR_ROTATE_RD_BASE_ADDR_POS (0) 
#define ROTT_RD_ADDR_ROTATE_RD_BASE_ADDR_MASK (0xFFFFFFFF) 

static inline uint32_t rott_ll_get_rd_addr_value(void)
{
    return REG_READ(ROTT_RD_ADDR_ADDR);
}

static inline void rott_ll_set_rd_addr_value(uint32_t value)
{
    REG_WRITE(ROTT_RD_ADDR_ADDR,value);
}

/* REG_0x07:rd_addr->rotate_rd_base_addr:0x7[31:0],rotate rd yuv data's base address.,0x0,r/w*/
static inline uint32_t rott_ll_get_rd_addr_rotate_rd_base_addr(void)
{
    return REG_READ(ROTT_RD_ADDR_ADDR);
}

static inline void rott_ll_set_rd_addr_rotate_rd_base_addr(uint32_t value)
{
    REG_WRITE(ROTT_RD_ADDR_ADDR,value);
}

/* REG_0x08 */
#define ROTT_WR_ADDR_ADDR  (ROTT_LL_REG_BASE  + 0x8*4) //REG ADDR :0x480c0020
#define ROTT_WR_ADDR_ROTATE_WR_BASE_ADDR_POS (0) 
#define ROTT_WR_ADDR_ROTATE_WR_BASE_ADDR_MASK (0xFFFFFFFF) 

static inline uint32_t rott_ll_get_wr_addr_value(void)
{
    return REG_READ(ROTT_WR_ADDR_ADDR);
}

static inline void rott_ll_set_wr_addr_value(uint32_t value)
{
    REG_WRITE(ROTT_WR_ADDR_ADDR,value);
}

/* REG_0x08:wr_addr->rotate_wr_base_addr:0x8[31:0],rotate wr rgb data's base address.,0x0,r/w*/
static inline uint32_t rott_ll_get_wr_addr_rotate_wr_base_addr(void)
{
    return REG_READ(ROTT_WR_ADDR_ADDR);
}

static inline void rott_ll_set_wr_addr_rotate_wr_base_addr(uint32_t value)
{
    REG_WRITE(ROTT_WR_ADDR_ADDR,value);
}

/* REG_0x09 */
#define ROTT_PICTURE_RESOLU_ADDR  (ROTT_LL_REG_BASE  + 0x9*4) //REG ADDR :0x480c0024
#define ROTT_PICTURE_RESOLU_PIC_LINE_PIXEL_POS (0) 
#define ROTT_PICTURE_RESOLU_PIC_LINE_PIXEL_MASK (0x7FF) 

#define ROTT_PICTURE_RESOLU_RESERVED1_POS (11) 
#define ROTT_PICTURE_RESOLU_RESERVED1_MASK (0x1) 

#define ROTT_PICTURE_RESOLU_PIC_CLUM_PIXEL_POS (12) 
#define ROTT_PICTURE_RESOLU_PIC_CLUM_PIXEL_MASK (0x7FF) 

#define ROTT_PICTURE_RESOLU_RESERVED0_POS (23) 
#define ROTT_PICTURE_RESOLU_RESERVED0_MASK (0x1FF) 

static inline uint32_t rott_ll_get_picture_resolu_value(void)
{
    return REG_READ(ROTT_PICTURE_RESOLU_ADDR);
}

static inline void rott_ll_set_picture_resolu_value(uint32_t value)
{
    REG_WRITE(ROTT_PICTURE_RESOLU_ADDR,value);
}

/* REG_0x09:picture_resolu->pic_line_pixel:0x9[10:0],the full pitcture's line pixel count.,0x0,r/w*/
static inline uint32_t rott_ll_get_picture_resolu_pic_line_pixel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_PICTURE_RESOLU_ADDR);
    reg_value = ((reg_value >> ROTT_PICTURE_RESOLU_PIC_LINE_PIXEL_POS) & ROTT_PICTURE_RESOLU_PIC_LINE_PIXEL_MASK);
    return reg_value;
}

static inline void rott_ll_set_picture_resolu_pic_line_pixel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_PICTURE_RESOLU_ADDR);
    reg_value &= ~(ROTT_PICTURE_RESOLU_PIC_LINE_PIXEL_MASK << ROTT_PICTURE_RESOLU_PIC_LINE_PIXEL_POS);
    reg_value |= ((value & ROTT_PICTURE_RESOLU_PIC_LINE_PIXEL_MASK) << ROTT_PICTURE_RESOLU_PIC_LINE_PIXEL_POS);
    REG_WRITE(ROTT_PICTURE_RESOLU_ADDR,reg_value);
}

/* REG_0x09:picture_resolu->pic_clum_pixel:0x9[22:12],the full picture 's clum_pixel count.,0x0,r/w*/
static inline uint32_t rott_ll_get_picture_resolu_pic_clum_pixel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_PICTURE_RESOLU_ADDR);
    reg_value = ((reg_value >> ROTT_PICTURE_RESOLU_PIC_CLUM_PIXEL_POS) & ROTT_PICTURE_RESOLU_PIC_CLUM_PIXEL_MASK);
    return reg_value;
}

static inline void rott_ll_set_picture_resolu_pic_clum_pixel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_PICTURE_RESOLU_ADDR);
    reg_value &= ~(ROTT_PICTURE_RESOLU_PIC_CLUM_PIXEL_MASK << ROTT_PICTURE_RESOLU_PIC_CLUM_PIXEL_POS);
    reg_value |= ((value & ROTT_PICTURE_RESOLU_PIC_CLUM_PIXEL_MASK) << ROTT_PICTURE_RESOLU_PIC_CLUM_PIXEL_POS);
    REG_WRITE(ROTT_PICTURE_RESOLU_ADDR,reg_value);
}

/* REG_0x0A */
#define ROTT_BLOCK_COUNT_ADDR  (ROTT_LL_REG_BASE  + 0xA*4) //REG ADDR :0x480c0028
#define ROTT_BLOCK_COUNT_ROTATE_BLK_COUNT_POS (0) 
#define ROTT_BLOCK_COUNT_ROTATE_BLK_COUNT_MASK (0xFFFF) 

#define ROTT_BLOCK_COUNT_WTMK_CLUM_PIXEL_POS (16) 
#define ROTT_BLOCK_COUNT_WTMK_CLUM_PIXEL_MASK (0x7FF) 

#define ROTT_BLOCK_COUNT_RESERVED0_POS (27) 
#define ROTT_BLOCK_COUNT_RESERVED0_MASK (0x1F) 

static inline uint32_t rott_ll_get_block_count_value(void)
{
    return REG_READ(ROTT_BLOCK_COUNT_ADDR);
}

static inline void rott_ll_set_block_count_value(uint32_t value)
{
    REG_WRITE(ROTT_BLOCK_COUNT_ADDR,value);
}

/* REG_0x0a:block_count->rotate_blk_count:0xa[15:  0],the full picture devided block picture's block count. block_count = pic_line_pixel/blk_line_pixel*pic_clum_pixel/blk_clum_pixel. 0x0,r/w*/
static inline uint32_t rott_ll_get_block_count_rotate_blk_count(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_BLOCK_COUNT_ADDR);
    reg_value = ((reg_value >> ROTT_BLOCK_COUNT_ROTATE_BLK_COUNT_POS) & ROTT_BLOCK_COUNT_ROTATE_BLK_COUNT_MASK);
    return reg_value;
}

static inline void rott_ll_set_block_count_rotate_blk_count(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_BLOCK_COUNT_ADDR);
    reg_value &= ~(ROTT_BLOCK_COUNT_ROTATE_BLK_COUNT_MASK << ROTT_BLOCK_COUNT_ROTATE_BLK_COUNT_POS);
    reg_value |= ((value & ROTT_BLOCK_COUNT_ROTATE_BLK_COUNT_MASK) << ROTT_BLOCK_COUNT_ROTATE_BLK_COUNT_POS);
    REG_WRITE(ROTT_BLOCK_COUNT_ADDR,reg_value);
}

/* REG_0x0a:block_count->wtmk_clum_pixel:0xa[26:16],water_mark int with clum postion.,0x0,r/w*/
static inline uint32_t rott_ll_get_block_count_wtmk_clum_pixel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_BLOCK_COUNT_ADDR);
    reg_value = ((reg_value >> ROTT_BLOCK_COUNT_WTMK_CLUM_PIXEL_POS) & ROTT_BLOCK_COUNT_WTMK_CLUM_PIXEL_MASK);
    return reg_value;
}

static inline void rott_ll_set_block_count_wtmk_clum_pixel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_BLOCK_COUNT_ADDR);
    reg_value &= ~(ROTT_BLOCK_COUNT_WTMK_CLUM_PIXEL_MASK << ROTT_BLOCK_COUNT_WTMK_CLUM_PIXEL_POS);
    reg_value |= ((value & ROTT_BLOCK_COUNT_WTMK_CLUM_PIXEL_MASK) << ROTT_BLOCK_COUNT_WTMK_CLUM_PIXEL_POS);
    REG_WRITE(ROTT_BLOCK_COUNT_ADDR,reg_value);
}

/* REG_0x0B */
#define ROTT_INT_STATUS_ADDR  (ROTT_LL_REG_BASE  + 0xB*4) //REG ADDR :0x480c002c
#define ROTT_INT_STATUS_ROTATE_FINISH_POS (0) 
#define ROTT_INT_STATUS_ROTATE_FINISH_MASK (0x1) 

#define ROTT_INT_STATUS_ROTATE_WTMK_POS (1) 
#define ROTT_INT_STATUS_ROTATE_WTMK_MASK (0x1) 

#define ROTT_INT_STATUS_ROTATE_CFG_ERR_POS (2) 
#define ROTT_INT_STATUS_ROTATE_CFG_ERR_MASK (0x1) 

#define ROTT_INT_STATUS_RESERVED0_POS (3) 
#define ROTT_INT_STATUS_RESERVED0_MASK (0x1FFFFFFF) 

static inline uint32_t rott_ll_get_int_status_value(void)
{
    return REG_READ(ROTT_INT_STATUS_ADDR);
}

static inline void rott_ll_set_int_status_value(uint32_t value)
{
    REG_WRITE(ROTT_INT_STATUS_ADDR,value);
}

/* REG_0x0b:int_status->rotate_finish:0xb[0],the full picture rotate finish.(need write 1 to clear.),0x0,r/w*/
static inline uint32_t rott_ll_get_int_status_rotate_finish(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_INT_STATUS_ADDR);
    reg_value = ((reg_value >> ROTT_INT_STATUS_ROTATE_FINISH_POS) & ROTT_INT_STATUS_ROTATE_FINISH_MASK);
    return reg_value;
}

static inline void rott_ll_set_int_status_rotate_finish(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_INT_STATUS_ADDR);
    reg_value &= ~(ROTT_INT_STATUS_ROTATE_FINISH_MASK << ROTT_INT_STATUS_ROTATE_FINISH_POS);
    reg_value |= ((value & ROTT_INT_STATUS_ROTATE_FINISH_MASK) << ROTT_INT_STATUS_ROTATE_FINISH_POS);
    REG_WRITE(ROTT_INT_STATUS_ADDR,reg_value);
}

/* REG_0x0b:int_status->rotate_wtmk:0xb[1],the watermark in line postion is arive.(need write 1 to clear.),0x0,r/w*/
static inline uint32_t rott_ll_get_int_status_rotate_wtmk(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_INT_STATUS_ADDR);
    reg_value = ((reg_value >> ROTT_INT_STATUS_ROTATE_WTMK_POS) & ROTT_INT_STATUS_ROTATE_WTMK_MASK);
    return reg_value;
}

static inline void rott_ll_set_int_status_rotate_wtmk(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_INT_STATUS_ADDR);
    reg_value &= ~(ROTT_INT_STATUS_ROTATE_WTMK_MASK << ROTT_INT_STATUS_ROTATE_WTMK_POS);
    reg_value |= ((value & ROTT_INT_STATUS_ROTATE_WTMK_MASK) << ROTT_INT_STATUS_ROTATE_WTMK_POS);
    REG_WRITE(ROTT_INT_STATUS_ADDR,reg_value);
}

/* REG_0x0b:int_status->rotate_cfg_err:0xb[2],the rotate config error while rotate enable.(need write 1 to clear.),0x0,r/w*/
static inline uint32_t rott_ll_get_int_status_rotate_cfg_err(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_INT_STATUS_ADDR);
    reg_value = ((reg_value >> ROTT_INT_STATUS_ROTATE_CFG_ERR_POS) & ROTT_INT_STATUS_ROTATE_CFG_ERR_MASK);
    return reg_value;
}

static inline void rott_ll_set_int_status_rotate_cfg_err(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(ROTT_INT_STATUS_ADDR);
    reg_value &= ~(ROTT_INT_STATUS_ROTATE_CFG_ERR_MASK << ROTT_INT_STATUS_ROTATE_CFG_ERR_POS);
    reg_value |= ((value & ROTT_INT_STATUS_ROTATE_CFG_ERR_MASK) << ROTT_INT_STATUS_ROTATE_CFG_ERR_POS);
    REG_WRITE(ROTT_INT_STATUS_ADDR,reg_value);
}

#ifdef __cplusplus 
}                  
#endif             
