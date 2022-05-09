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
* CHIP ID:BK7256,GENARATE TIME:2022-01-18 17:28:24                                                 
************************************************************************************************************************************/

#pragma once                 
                            
#ifdef __cplusplus          
extern "C" {              
#endif                      

#include <soc/soc.h>          

#define JPEG_LL_REG_BASE      (SOC_JPEG_REG_BASE) //REG_BASE:0x48000000

/* REG_0x00 */
#define JPEG_REG0_ADDR  (JPEG_LL_REG_BASE  + 0x0*4) //REG ADDR :0x48000000
#define JPEG_REG0_RESERVED_POS (0) 
#define JPEG_REG0_RESERVED_MASK (0x3) 

#define JPEG_REG0_INT_EN_POS (2) 
#define JPEG_REG0_INT_EN_MASK (0x3) 

#define JPEG_REG0_MCLK_DIV_POS (4) 
#define JPEG_REG0_MCLK_DIV_MASK (0x3) 

#define JPEG_REG0_RESERVED_POS (6) 
#define JPEG_REG0_RESERVED_MASK (0x3FFFFFF) 

static inline uint32_t jpeg_ll_get_reg0_value(void)
{
    return REG_READ(JPEG_REG0_ADDR);
}

static inline void jpeg_ll_set_reg0_value(uint32_t value)
{
    REG_WRITE(JPEG_REG0_ADDR,value);
}

/* REG_0x00:reg0->int_en:0x0[3:2],int_en[0]  start of frame int enable; int_en[1]   end of frame int enable                      ; ,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg0_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG0_ADDR);
    reg_value = ((reg_value >> JPEG_REG0_INT_EN_POS) & JPEG_REG0_INT_EN_MASK);
    return reg_value;
}

static inline void jpeg_ll_set_reg0_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG0_ADDR);
    reg_value &= ~(JPEG_REG0_INT_EN_MASK << JPEG_REG0_INT_EN_POS);
    reg_value |= ((value & JPEG_REG0_INT_EN_MASK) << JPEG_REG0_INT_EN_POS);
    REG_WRITE(JPEG_REG0_ADDR,reg_value);
}

/* REG_0x00:reg0->mclk_div:0x0[5:4],MCLK div  00/11:  24M;                   01    :   16M;                   10    :    12M,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg0_mclk_div(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG0_ADDR);
    reg_value = ((reg_value >> JPEG_REG0_MCLK_DIV_POS) & JPEG_REG0_MCLK_DIV_MASK);
    return reg_value;
}

static inline void jpeg_ll_set_reg0_mclk_div(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG0_ADDR);
    reg_value &= ~(JPEG_REG0_MCLK_DIV_MASK << JPEG_REG0_MCLK_DIV_POS);
    reg_value |= ((value & JPEG_REG0_MCLK_DIV_MASK) << JPEG_REG0_MCLK_DIV_POS);
    REG_WRITE(JPEG_REG0_ADDR,reg_value);
}

/* REG_0x01 */
#define JPEG_REG1_ADDR  (JPEG_LL_REG_BASE  + 0x1*4) //REG ADDR :0x48000004
#define JPEG_REG1_VCK_EDGE_POS (0) 
#define JPEG_REG1_VCK_EDGE_MASK (0x1) 

#define JPEG_REG1_VIDEO_BYTE_REV_POS (1) 
#define JPEG_REG1_VIDEO_BYTE_REV_MASK (0x1) 

#define JPEG_REG1_YUV_FMT_SEL_POS (2) 
#define JPEG_REG1_YUV_FMT_SEL_MASK (0x3) 

#define JPEG_REG1_JPEG_ENC_EN_POS (4) 
#define JPEG_REG1_JPEG_ENC_EN_MASK (0x1) 

#define JPEG_REG1_RESERVED_POS (5) 
#define JPEG_REG1_RESERVED_MASK (0x1) 

#define JPEG_REG1_ONLY_Y_POS (6) 
#define JPEG_REG1_ONLY_Y_MASK (0x1) 

#define JPEG_REG1_YUVBUF_MODE_POS (7) 
#define JPEG_REG1_YUVBUF_MODE_MASK (0x1) 

#define JPEG_REG1_X_PIXEL_POS (8) 
#define JPEG_REG1_X_PIXEL_MASK (0xFF) 

#define JPEG_REG1_JPEG_ENC_SIZE_POS (16) 
#define JPEG_REG1_JPEG_ENC_SIZE_MASK (0x1) 

#define JPEG_REG1_BITRATE_CTRL_POS (17) 
#define JPEG_REG1_BITRATE_CTRL_MASK (0x1) 

#define JPEG_REG1_BITRATE_STEP_POS (18) 
#define JPEG_REG1_BITRATE_STEP_MASK (0x3) 

#define JPEG_REG1_AUTO_STEP_POS (20) 
#define JPEG_REG1_AUTO_STEP_MASK (0x1) 

#define JPEG_REG1_HSYNC_REV_POS (21) 
#define JPEG_REG1_HSYNC_REV_MASK (0x1) 

#define JPEG_REG1_VSYNC_REV_POS (22) 
#define JPEG_REG1_VSYNC_REV_MASK (0x1) 

#define JPEG_REG1_BITRATE_MODE_POS (23) 
#define JPEG_REG1_BITRATE_MODE_MASK (0x1) 

#define JPEG_REG1_Y_PIXEL_POS (24) 
#define JPEG_REG1_Y_PIXEL_MASK (0xFF) 

static inline uint32_t jpeg_ll_get_reg1_value(void)
{
    return REG_READ(JPEG_REG1_ADDR);
}

static inline void jpeg_ll_set_reg1_value(uint32_t value)
{
    REG_WRITE(JPEG_REG1_ADDR,value);
}

/* REG_0x01:reg1->vck_edge:0x1[0],vclk sample data edge  1:negedge  0： posedge,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg1_vck_edge(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value = ((reg_value >> JPEG_REG1_VCK_EDGE_POS) & JPEG_REG1_VCK_EDGE_MASK);
    return reg_value;
}

static inline void jpeg_ll_set_reg1_vck_edge(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value &= ~(JPEG_REG1_VCK_EDGE_MASK << JPEG_REG1_VCK_EDGE_POS);
    reg_value |= ((value & JPEG_REG1_VCK_EDGE_MASK) << JPEG_REG1_VCK_EDGE_POS);
    REG_WRITE(JPEG_REG1_ADDR,reg_value);
}

/* REG_0x01:reg1->video_byte_rev:0x1[1],reverse the  4byte word of fifo ,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg1_video_byte_rev(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value = ((reg_value >> JPEG_REG1_VIDEO_BYTE_REV_POS) & JPEG_REG1_VIDEO_BYTE_REV_MASK);
    return reg_value;
}

static inline void jpeg_ll_set_reg1_video_byte_rev(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value &= ~(JPEG_REG1_VIDEO_BYTE_REV_MASK << JPEG_REG1_VIDEO_BYTE_REV_POS);
    reg_value |= ((value & JPEG_REG1_VIDEO_BYTE_REV_MASK) << JPEG_REG1_VIDEO_BYTE_REV_POS);
    REG_WRITE(JPEG_REG1_ADDR,reg_value);
}

/* REG_0x01:reg1->yuv_fmt_sel:0x1[3:2],yuv format 00: YUYV    01: UYVY   10: YYUV   11: UVYY,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg1_yuv_fmt_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value = ((reg_value >> JPEG_REG1_YUV_FMT_SEL_POS) & JPEG_REG1_YUV_FMT_SEL_MASK);
    return reg_value;
}

static inline void jpeg_ll_set_reg1_yuv_fmt_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value &= ~(JPEG_REG1_YUV_FMT_SEL_MASK << JPEG_REG1_YUV_FMT_SEL_POS);
    reg_value |= ((value & JPEG_REG1_YUV_FMT_SEL_MASK) << JPEG_REG1_YUV_FMT_SEL_POS);
    REG_WRITE(JPEG_REG1_ADDR,reg_value);
}

/* REG_0x01:reg1->jpeg_enc_en:0x1[4],jpeg enc enable,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg1_jpeg_enc_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value = ((reg_value >> JPEG_REG1_JPEG_ENC_EN_POS) & JPEG_REG1_JPEG_ENC_EN_MASK);
    return reg_value;
}

static inline void jpeg_ll_set_reg1_jpeg_enc_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value &= ~(JPEG_REG1_JPEG_ENC_EN_MASK << JPEG_REG1_JPEG_ENC_EN_POS);
    reg_value |= ((value & JPEG_REG1_JPEG_ENC_EN_MASK) << JPEG_REG1_JPEG_ENC_EN_POS);
    REG_WRITE(JPEG_REG1_ADDR,reg_value);
}

/* REG_0x01:reg1->only_y:0x1[6],yuv buf mode下, output y   data,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg1_only_y(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value = ((reg_value >> JPEG_REG1_ONLY_Y_POS) & JPEG_REG1_ONLY_Y_MASK);
    return reg_value;
}

static inline void jpeg_ll_set_reg1_only_y(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value &= ~(JPEG_REG1_ONLY_Y_MASK << JPEG_REG1_ONLY_Y_POS);
    reg_value |= ((value & JPEG_REG1_ONLY_Y_MASK) << JPEG_REG1_ONLY_Y_POS);
    REG_WRITE(JPEG_REG1_ADDR,reg_value);
}

/* REG_0x01:reg1->yuvbuf_mode:0x1[7],yuv buf mode, output yuv data,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg1_yuvbuf_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value = ((reg_value >> JPEG_REG1_YUVBUF_MODE_POS) & JPEG_REG1_YUVBUF_MODE_MASK);
    return reg_value;
}

static inline void jpeg_ll_set_reg1_yuvbuf_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value &= ~(JPEG_REG1_YUVBUF_MODE_MASK << JPEG_REG1_YUVBUF_MODE_POS);
    reg_value |= ((value & JPEG_REG1_YUVBUF_MODE_MASK) << JPEG_REG1_YUVBUF_MODE_POS);
    REG_WRITE(JPEG_REG1_ADDR,reg_value);
}

/* REG_0x01:reg1->x_pixel:0x1[15:8],行像素长度=y_pixel*8, for VGA y_pixel = 80,0x50,R/W*/
static inline uint32_t jpeg_ll_get_reg1_x_pixel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value = ((reg_value >> JPEG_REG1_X_PIXEL_POS) & JPEG_REG1_X_PIXEL_MASK);
    return reg_value;
}

static inline void jpeg_ll_set_reg1_x_pixel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value &= ~(JPEG_REG1_X_PIXEL_MASK << JPEG_REG1_X_PIXEL_POS);
    reg_value |= ((value & JPEG_REG1_X_PIXEL_MASK) << JPEG_REG1_X_PIXEL_POS);
    REG_WRITE(JPEG_REG1_ADDR,reg_value);
}

/* REG_0x01:reg1->jpeg_enc_size:0x1[16],output  size after end image,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg1_jpeg_enc_size(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value = ((reg_value >> JPEG_REG1_JPEG_ENC_SIZE_POS) & JPEG_REG1_JPEG_ENC_SIZE_MASK);
    return reg_value;
}

static inline void jpeg_ll_set_reg1_jpeg_enc_size(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value &= ~(JPEG_REG1_JPEG_ENC_SIZE_MASK << JPEG_REG1_JPEG_ENC_SIZE_POS);
    reg_value |= ((value & JPEG_REG1_JPEG_ENC_SIZE_MASK) << JPEG_REG1_JPEG_ENC_SIZE_POS);
    REG_WRITE(JPEG_REG1_ADDR,reg_value);
}

/* REG_0x01:reg1->bitrate_ctrl:0x1[17],bit_rate control enable,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg1_bitrate_ctrl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value = ((reg_value >> JPEG_REG1_BITRATE_CTRL_POS) & JPEG_REG1_BITRATE_CTRL_MASK);
    return reg_value;
}

static inline void jpeg_ll_set_reg1_bitrate_ctrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value &= ~(JPEG_REG1_BITRATE_CTRL_MASK << JPEG_REG1_BITRATE_CTRL_POS);
    reg_value |= ((value & JPEG_REG1_BITRATE_CTRL_MASK) << JPEG_REG1_BITRATE_CTRL_POS);
    REG_WRITE(JPEG_REG1_ADDR,reg_value);
}

/* REG_0x01:reg1->bitrate_step:0x1[19:18],quat table scator adujest step; 00： 1/4倍; 01： 1/2倍; 10： 3/4倍; 11： 1倍,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg1_bitrate_step(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value = ((reg_value >> JPEG_REG1_BITRATE_STEP_POS) & JPEG_REG1_BITRATE_STEP_MASK);
    return reg_value;
}

static inline void jpeg_ll_set_reg1_bitrate_step(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value &= ~(JPEG_REG1_BITRATE_STEP_MASK << JPEG_REG1_BITRATE_STEP_POS);
    reg_value |= ((value & JPEG_REG1_BITRATE_STEP_MASK) << JPEG_REG1_BITRATE_STEP_POS);
    REG_WRITE(JPEG_REG1_ADDR,reg_value);
}

/* REG_0x01:reg1->auto_step:0x1[20],码率自适应模式步长  0： 4倍   1：:2倍,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg1_auto_step(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value = ((reg_value >> JPEG_REG1_AUTO_STEP_POS) & JPEG_REG1_AUTO_STEP_MASK);
    return reg_value;
}

static inline void jpeg_ll_set_reg1_auto_step(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value &= ~(JPEG_REG1_AUTO_STEP_MASK << JPEG_REG1_AUTO_STEP_POS);
    reg_value |= ((value & JPEG_REG1_AUTO_STEP_MASK) << JPEG_REG1_AUTO_STEP_POS);
    REG_WRITE(JPEG_REG1_ADDR,reg_value);
}

/* REG_0x01:reg1->hsync_rev:0x1[21],hsync reverse,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg1_hsync_rev(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value = ((reg_value >> JPEG_REG1_HSYNC_REV_POS) & JPEG_REG1_HSYNC_REV_MASK);
    return reg_value;
}

static inline void jpeg_ll_set_reg1_hsync_rev(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value &= ~(JPEG_REG1_HSYNC_REV_MASK << JPEG_REG1_HSYNC_REV_POS);
    reg_value |= ((value & JPEG_REG1_HSYNC_REV_MASK) << JPEG_REG1_HSYNC_REV_POS);
    REG_WRITE(JPEG_REG1_ADDR,reg_value);
}

/* REG_0x01:reg1->vsync_rev:0x1[22],vsync reverse,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg1_vsync_rev(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value = ((reg_value >> JPEG_REG1_VSYNC_REV_POS) & JPEG_REG1_VSYNC_REV_MASK);
    return reg_value;
}

static inline void jpeg_ll_set_reg1_vsync_rev(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value &= ~(JPEG_REG1_VSYNC_REV_MASK << JPEG_REG1_VSYNC_REV_POS);
    reg_value |= ((value & JPEG_REG1_VSYNC_REV_MASK) << JPEG_REG1_VSYNC_REV_POS);
    REG_WRITE(JPEG_REG1_ADDR,reg_value);
}

/* REG_0x01:reg1->bitrate_mode:0x1[23],码率自适应模式，如果码率比目标码率大2倍，则量化步长为2倍或4倍，由auto_step决定,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg1_bitrate_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value = ((reg_value >> JPEG_REG1_BITRATE_MODE_POS) & JPEG_REG1_BITRATE_MODE_MASK);
    return reg_value;
}

static inline void jpeg_ll_set_reg1_bitrate_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value &= ~(JPEG_REG1_BITRATE_MODE_MASK << JPEG_REG1_BITRATE_MODE_POS);
    reg_value |= ((value & JPEG_REG1_BITRATE_MODE_MASK) << JPEG_REG1_BITRATE_MODE_POS);
    REG_WRITE(JPEG_REG1_ADDR,reg_value);
}

/* REG_0x01:reg1->y_pixel:0x1[31:24],列像素长度=y_pixel*8, for VGA y_pixel = 60,0x3c,R/W*/
static inline uint32_t jpeg_ll_get_reg1_y_pixel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value = ((reg_value >> JPEG_REG1_Y_PIXEL_POS) & JPEG_REG1_Y_PIXEL_MASK);
    return reg_value;
}

static inline void jpeg_ll_set_reg1_y_pixel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG1_ADDR);
    reg_value &= ~(JPEG_REG1_Y_PIXEL_MASK << JPEG_REG1_Y_PIXEL_POS);
    reg_value |= ((value & JPEG_REG1_Y_PIXEL_MASK) << JPEG_REG1_Y_PIXEL_POS);
    REG_WRITE(JPEG_REG1_ADDR,reg_value);
}

/* REG_0x02 */
#define JPEG_REG2_ADDR  (JPEG_LL_REG_BASE  + 0x2*4) //REG ADDR :0x48000008
#define JPEG_REG2_TARGET_BYTE_H_POS (0) 
#define JPEG_REG2_TARGET_BYTE_H_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg2_value(void)
{
    return REG_READ(JPEG_REG2_ADDR);
}

static inline void jpeg_ll_set_reg2_value(uint32_t value)
{
    REG_WRITE(JPEG_REG2_ADDR,value);
}

/* REG_0x02:reg2->target_byte_h:0x2,high limit of  size after jpeg compression,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg2_target_byte_h(void)
{
    return REG_READ(JPEG_REG2_ADDR);
}

static inline void jpeg_ll_set_reg2_target_byte_h(uint32_t value)
{
    REG_WRITE(JPEG_REG2_ADDR,value);
}

/* REG_0x03 */
#define JPEG_REG3_ADDR  (JPEG_LL_REG_BASE  + 0x3*4) //REG ADDR :0x4800000c
#define JPEG_REG3_TARGET_BYTE_L_POS (0) 
#define JPEG_REG3_TARGET_BYTE_L_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg3_value(void)
{
    return REG_READ(JPEG_REG3_ADDR);
}

static inline void jpeg_ll_set_reg3_value(uint32_t value)
{
    REG_WRITE(JPEG_REG3_ADDR,value);
}

/* REG_0x03:reg3->target_byte_l:0x3,low  limit of  size after jpeg compression,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg3_target_byte_l(void)
{
    return REG_READ(JPEG_REG3_ADDR);
}

static inline void jpeg_ll_set_reg3_target_byte_l(uint32_t value)
{
    REG_WRITE(JPEG_REG3_ADDR,value);
}

/* REG_0x04 */
#define JPEG_REG4_ADDR  (JPEG_LL_REG_BASE  + 0x4*4) //REG ADDR :0x48000010
#define JPEG_REG4_EM_BASE_ADDR_POS (0) 
#define JPEG_REG4_EM_BASE_ADDR_MASK (0xFFFF) 

static inline uint32_t jpeg_ll_get_reg4_value(void)
{
    return REG_READ(JPEG_REG4_ADDR);
}

/* REG_0x04:reg4->em_base_addr:0x4[15:0],sensor数据存放的高16位地址，请直接指向sharemem的地址。,0x0,R*/
static inline uint32_t jpeg_ll_get_reg4_em_base_addr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG4_ADDR);
    reg_value = ((reg_value >> JPEG_REG4_EM_BASE_ADDR_POS)&JPEG_REG4_EM_BASE_ADDR_MASK);
    return reg_value;
}

/* REG_0x05 */
#define JPEG_REG5_ADDR  (JPEG_LL_REG_BASE  + 0x5*4) //REG ADDR :0x48000014
#define JPEG_REG5_RXFIFO_DATA_POS (0) 
#define JPEG_REG5_RXFIFO_DATA_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg5_value(void)
{
    return REG_READ(JPEG_REG5_ADDR);
}

/* REG_0x05:reg5->rxfifo_data:0x5[31:0],Rx FIFO Data,0x0,R*/
static inline uint32_t jpeg_ll_get_reg5_rxfifo_data(void)
{
    return REG_READ(JPEG_REG5_ADDR);
}

/* REG_0x06 */
#define JPEG_REG6_ADDR  (JPEG_LL_REG_BASE  + 0x6*4) //REG ADDR :0x48000018
#define JPEG_REG6_INT_STATUS_POS (0) 
#define JPEG_REG6_INT_STATUS_MASK (0x3) 

#define JPEG_REG6_FIFO_RD_FIFNISH_POS (2) 
#define JPEG_REG6_FIFO_RD_FIFNISH_MASK (0x1) 

#define JPEG_REG6_RESERVED_POS (3) 
#define JPEG_REG6_RESERVED_MASK (0x1FFFFFFF) 

static inline uint32_t jpeg_ll_get_reg6_value(void)
{
    return REG_READ(JPEG_REG6_ADDR);
}

static inline void jpeg_ll_set_reg6_value(uint32_t value)
{
    REG_WRITE(JPEG_REG6_ADDR,value);
}

/* REG_0x06:reg6->int_status:0x6[1:0],Interrupt status. Need software write '1' to clear it; [0]: frame start int; [1]: frame end int,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg6_int_status(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG6_ADDR);
    reg_value = ((reg_value >> JPEG_REG6_INT_STATUS_POS) & JPEG_REG6_INT_STATUS_MASK);
    return reg_value;
}

static inline void jpeg_ll_set_reg6_int_status(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG6_ADDR);
    reg_value &= ~(JPEG_REG6_INT_STATUS_MASK << JPEG_REG6_INT_STATUS_POS);
    reg_value |= ((value & JPEG_REG6_INT_STATUS_MASK) << JPEG_REG6_INT_STATUS_POS);
    REG_WRITE(JPEG_REG6_ADDR,reg_value);
}

/* REG_0x06:reg6->fifo_rd_fifnish:0x6[2],读fifo已经完成,0x0,R*/
static inline uint32_t jpeg_ll_get_reg6_fifo_rd_fifnish(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG6_ADDR);
    reg_value = ((reg_value >> JPEG_REG6_FIFO_RD_FIFNISH_POS)&JPEG_REG6_FIFO_RD_FIFNISH_MASK);
    return reg_value;
}

/* REG_0x07 */
#define JPEG_REG7_ADDR  (JPEG_LL_REG_BASE  + 0x7*4) //REG ADDR :0x4800001c
#define JPEG_REG7_BYTE_COUNT_PFRM_POS (0) 
#define JPEG_REG7_BYTE_COUNT_PFRM_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg7_value(void)
{
    return REG_READ(JPEG_REG7_ADDR);
}

/* REG_0x07:reg7->byte_count_pfrm:0x7,the byte number of every frame,0x0,R*/
static inline uint32_t jpeg_ll_get_reg7_byte_count_pfrm(void)
{
    return REG_READ(JPEG_REG7_ADDR);
}

/* REG_0x08 */
#define JPEG_REG8_ADDR  (JPEG_LL_REG_BASE  + 0x8*4) //REG ADDR :0x48000020
#define JPEG_REG8_FIFO_EMPTY_POS (0) 
#define JPEG_REG8_FIFO_EMPTY_MASK (0x1) 

#define JPEG_REG8_FIFO_FULL_POS (1) 
#define JPEG_REG8_FIFO_FULL_MASK (0x1) 

#define JPEG_REG8_RESERVED_POS (2) 
#define JPEG_REG8_RESERVED_MASK (0x3FFFFFFF) 

static inline uint32_t jpeg_ll_get_reg8_value(void)
{
    return REG_READ(JPEG_REG8_ADDR);
}

/* REG_0x08:reg8->fifo_empty:0x8[0],stream out fifo empty,0x0,R*/
static inline uint32_t jpeg_ll_get_reg8_fifo_empty(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG8_ADDR);
    reg_value = ((reg_value >> JPEG_REG8_FIFO_EMPTY_POS)&JPEG_REG8_FIFO_EMPTY_MASK);
    return reg_value;
}

/* REG_0x08:reg8->fifo_full:0x8[1],stream out fifo full,0x0,R*/
static inline uint32_t jpeg_ll_get_reg8_fifo_full(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG8_ADDR);
    reg_value = ((reg_value >> JPEG_REG8_FIFO_FULL_POS)&JPEG_REG8_FIFO_FULL_MASK);
    return reg_value;
}

/* REG_0x09 */
#define JPEG_REG9_ADDR  (JPEG_LL_REG_BASE  + 0x9*4) //REG ADDR :0x48000024
#define JPEG_REG9_Y_COUNT_POS (0) 
#define JPEG_REG9_Y_COUNT_MASK (0xFF) 

#define JPEG_REG9_IS_DATA_ST_POS (8) 
#define JPEG_REG9_IS_DATA_ST_MASK (0x1) 

#define JPEG_REG9_RESERVED_POS (2) 
#define JPEG_REG9_RESERVED_MASK (0x3FFFFFFF) 

static inline uint32_t jpeg_ll_get_reg9_value(void)
{
    return REG_READ(JPEG_REG9_ADDR);
}

/* REG_0x09:reg9->y_count:0x9[7:0],y count number,0x0,R*/
static inline uint32_t jpeg_ll_get_reg9_y_count(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG9_ADDR);
    reg_value = ((reg_value >> JPEG_REG9_Y_COUNT_POS)&JPEG_REG9_Y_COUNT_MASK);
    return reg_value;
}

/* REG_0x09:reg9->is_data_st:0x9[8],jpeg data state,0x0,R*/
static inline uint32_t jpeg_ll_get_reg9_is_data_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(JPEG_REG9_ADDR);
    reg_value = ((reg_value >> JPEG_REG9_IS_DATA_ST_POS)&JPEG_REG9_IS_DATA_ST_MASK);
    return reg_value;
}

/* REG_0x0A */
#define JPEG_REGA_ADDR  (JPEG_LL_REG_BASE  + 0xA*4) //REG ADDR :0x48000028
#define JPEG_REGA_BYTE_COUNT_LINE_POS (0) 
#define JPEG_REGA_BYTE_COUNT_LINE_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_rega_value(void)
{
    return REG_READ(JPEG_REGA_ADDR);
}

/* REG_0x0a:rega->byte_count_line:0xa[31:0],byte count every line,0x0,R*/
static inline uint32_t jpeg_ll_get_rega_byte_count_line(void)
{
    return REG_READ(JPEG_REGA_ADDR);
}

/* REG_0x20 */
#define JPEG_REG20_ADDR  (JPEG_LL_REG_BASE  + 0x20*4) //REG ADDR :0x48000080
#define JPEG_REG20_QUTABLE_0_POS (0) 
#define JPEG_REG20_QUTABLE_0_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg20_value(void)
{
    return REG_READ(JPEG_REG20_ADDR);
}

static inline void jpeg_ll_set_reg20_value(uint32_t value)
{
    REG_WRITE(JPEG_REG20_ADDR,value);
}

/* REG_0x20:reg20->qutable_0:0x20[31:0],quat table0,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg20_qutable_0(void)
{
    return REG_READ(JPEG_REG20_ADDR);
}

static inline void jpeg_ll_set_reg20_qutable_0(uint32_t value)
{
    REG_WRITE(JPEG_REG20_ADDR,value);
}

/* REG_0x21 */
#define JPEG_REG21_ADDR  (JPEG_LL_REG_BASE  + 0x21*4) //REG ADDR :0x48000084
#define JPEG_REG21_QUTABLE_1_POS (0) 
#define JPEG_REG21_QUTABLE_1_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg21_value(void)
{
    return REG_READ(JPEG_REG21_ADDR);
}

static inline void jpeg_ll_set_reg21_value(uint32_t value)
{
    REG_WRITE(JPEG_REG21_ADDR,value);
}

/* REG_0x21:reg21->qutable_1:0x21[31:0],quat table1,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg21_qutable_1(void)
{
    return REG_READ(JPEG_REG21_ADDR);
}

static inline void jpeg_ll_set_reg21_qutable_1(uint32_t value)
{
    REG_WRITE(JPEG_REG21_ADDR,value);
}

/* REG_0x22 */
#define JPEG_REG22_ADDR  (JPEG_LL_REG_BASE  + 0x22*4) //REG ADDR :0x48000088
#define JPEG_REG22_QUTABLE_2_POS (0) 
#define JPEG_REG22_QUTABLE_2_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg22_value(void)
{
    return REG_READ(JPEG_REG22_ADDR);
}

static inline void jpeg_ll_set_reg22_value(uint32_t value)
{
    REG_WRITE(JPEG_REG22_ADDR,value);
}

/* REG_0x22:reg22->qutable_2:0x22[31:0],quat table2,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg22_qutable_2(void)
{
    return REG_READ(JPEG_REG22_ADDR);
}

static inline void jpeg_ll_set_reg22_qutable_2(uint32_t value)
{
    REG_WRITE(JPEG_REG22_ADDR,value);
}

/* REG_0x23 */
#define JPEG_REG23_ADDR  (JPEG_LL_REG_BASE  + 0x23*4) //REG ADDR :0x4800008c
#define JPEG_REG23_QUTABLE_3_POS (0) 
#define JPEG_REG23_QUTABLE_3_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg23_value(void)
{
    return REG_READ(JPEG_REG23_ADDR);
}

static inline void jpeg_ll_set_reg23_value(uint32_t value)
{
    REG_WRITE(JPEG_REG23_ADDR,value);
}

/* REG_0x23:reg23->qutable_3:0x23[31:0],quat table3,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg23_qutable_3(void)
{
    return REG_READ(JPEG_REG23_ADDR);
}

static inline void jpeg_ll_set_reg23_qutable_3(uint32_t value)
{
    REG_WRITE(JPEG_REG23_ADDR,value);
}

/* REG_0x24 */
#define JPEG_REG24_ADDR  (JPEG_LL_REG_BASE  + 0x24*4) //REG ADDR :0x48000090
#define JPEG_REG24_QUTABLE_4_POS (0) 
#define JPEG_REG24_QUTABLE_4_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg24_value(void)
{
    return REG_READ(JPEG_REG24_ADDR);
}

static inline void jpeg_ll_set_reg24_value(uint32_t value)
{
    REG_WRITE(JPEG_REG24_ADDR,value);
}

/* REG_0x24:reg24->qutable_4:0x24[31:0],quat table4,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg24_qutable_4(void)
{
    return REG_READ(JPEG_REG24_ADDR);
}

static inline void jpeg_ll_set_reg24_qutable_4(uint32_t value)
{
    REG_WRITE(JPEG_REG24_ADDR,value);
}

/* REG_0x25 */
#define JPEG_REG25_ADDR  (JPEG_LL_REG_BASE  + 0x25*4) //REG ADDR :0x48000094
#define JPEG_REG25_QUTABLE_5_POS (0) 
#define JPEG_REG25_QUTABLE_5_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg25_value(void)
{
    return REG_READ(JPEG_REG25_ADDR);
}

static inline void jpeg_ll_set_reg25_value(uint32_t value)
{
    REG_WRITE(JPEG_REG25_ADDR,value);
}

/* REG_0x25:reg25->qutable_5:0x25[31:0],quat table5,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg25_qutable_5(void)
{
    return REG_READ(JPEG_REG25_ADDR);
}

static inline void jpeg_ll_set_reg25_qutable_5(uint32_t value)
{
    REG_WRITE(JPEG_REG25_ADDR,value);
}

/* REG_0x26 */
#define JPEG_REG26_ADDR  (JPEG_LL_REG_BASE  + 0x26*4) //REG ADDR :0x48000098
#define JPEG_REG26_QUTABLE_6_POS (0) 
#define JPEG_REG26_QUTABLE_6_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg26_value(void)
{
    return REG_READ(JPEG_REG26_ADDR);
}

static inline void jpeg_ll_set_reg26_value(uint32_t value)
{
    REG_WRITE(JPEG_REG26_ADDR,value);
}

/* REG_0x26:reg26->qutable_6:0x26[31:0],quat table6,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg26_qutable_6(void)
{
    return REG_READ(JPEG_REG26_ADDR);
}

static inline void jpeg_ll_set_reg26_qutable_6(uint32_t value)
{
    REG_WRITE(JPEG_REG26_ADDR,value);
}

/* REG_0x27 */
#define JPEG_REG27_ADDR  (JPEG_LL_REG_BASE  + 0x27*4) //REG ADDR :0x4800009c
#define JPEG_REG27_QUTABLE_7_POS (0) 
#define JPEG_REG27_QUTABLE_7_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg27_value(void)
{
    return REG_READ(JPEG_REG27_ADDR);
}

static inline void jpeg_ll_set_reg27_value(uint32_t value)
{
    REG_WRITE(JPEG_REG27_ADDR,value);
}

/* REG_0x27:reg27->qutable_7:0x27[31:0],quat table7,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg27_qutable_7(void)
{
    return REG_READ(JPEG_REG27_ADDR);
}

static inline void jpeg_ll_set_reg27_qutable_7(uint32_t value)
{
    REG_WRITE(JPEG_REG27_ADDR,value);
}

/* REG_0x28 */
#define JPEG_REG28_ADDR  (JPEG_LL_REG_BASE  + 0x28*4) //REG ADDR :0x480000a0
#define JPEG_REG28_QUTABLE_8_POS (0) 
#define JPEG_REG28_QUTABLE_8_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg28_value(void)
{
    return REG_READ(JPEG_REG28_ADDR);
}

static inline void jpeg_ll_set_reg28_value(uint32_t value)
{
    REG_WRITE(JPEG_REG28_ADDR,value);
}

/* REG_0x28:reg28->qutable_8:0x28[31:0],quat table8,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg28_qutable_8(void)
{
    return REG_READ(JPEG_REG28_ADDR);
}

static inline void jpeg_ll_set_reg28_qutable_8(uint32_t value)
{
    REG_WRITE(JPEG_REG28_ADDR,value);
}

/* REG_0x29 */
#define JPEG_REG29_ADDR  (JPEG_LL_REG_BASE  + 0x29*4) //REG ADDR :0x480000a4
#define JPEG_REG29_QUTABLE_9_POS (0) 
#define JPEG_REG29_QUTABLE_9_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg29_value(void)
{
    return REG_READ(JPEG_REG29_ADDR);
}

static inline void jpeg_ll_set_reg29_value(uint32_t value)
{
    REG_WRITE(JPEG_REG29_ADDR,value);
}

/* REG_0x29:reg29->qutable_9:0x29[31:0],quat table9,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg29_qutable_9(void)
{
    return REG_READ(JPEG_REG29_ADDR);
}

static inline void jpeg_ll_set_reg29_qutable_9(uint32_t value)
{
    REG_WRITE(JPEG_REG29_ADDR,value);
}

/* REG_0x2A */
#define JPEG_REG2A_ADDR  (JPEG_LL_REG_BASE  + 0x2A*4) //REG ADDR :0x480000a8
#define JPEG_REG2A_QUTABLE_A_POS (0) 
#define JPEG_REG2A_QUTABLE_A_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg2a_value(void)
{
    return REG_READ(JPEG_REG2A_ADDR);
}

static inline void jpeg_ll_set_reg2a_value(uint32_t value)
{
    REG_WRITE(JPEG_REG2A_ADDR,value);
}

/* REG_0x2a:reg2a->qutable_a:0x2a[31:0],quat tablea,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg2a_qutable_a(void)
{
    return REG_READ(JPEG_REG2A_ADDR);
}

static inline void jpeg_ll_set_reg2a_qutable_a(uint32_t value)
{
    REG_WRITE(JPEG_REG2A_ADDR,value);
}

/* REG_0x2B */
#define JPEG_REG2B_ADDR  (JPEG_LL_REG_BASE  + 0x2B*4) //REG ADDR :0x480000ac
#define JPEG_REG2B_QUTABLE_B_POS (0) 
#define JPEG_REG2B_QUTABLE_B_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg2b_value(void)
{
    return REG_READ(JPEG_REG2B_ADDR);
}

static inline void jpeg_ll_set_reg2b_value(uint32_t value)
{
    REG_WRITE(JPEG_REG2B_ADDR,value);
}

/* REG_0x2b:reg2b->qutable_b:0x2b[31:0],quat tableb,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg2b_qutable_b(void)
{
    return REG_READ(JPEG_REG2B_ADDR);
}

static inline void jpeg_ll_set_reg2b_qutable_b(uint32_t value)
{
    REG_WRITE(JPEG_REG2B_ADDR,value);
}

/* REG_0x2C */
#define JPEG_REG2C_ADDR  (JPEG_LL_REG_BASE  + 0x2C*4) //REG ADDR :0x480000b0
#define JPEG_REG2C_QUTABLE_C_POS (0) 
#define JPEG_REG2C_QUTABLE_C_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg2c_value(void)
{
    return REG_READ(JPEG_REG2C_ADDR);
}

static inline void jpeg_ll_set_reg2c_value(uint32_t value)
{
    REG_WRITE(JPEG_REG2C_ADDR,value);
}

/* REG_0x2c:reg2c->qutable_c:0x2c[31:0],quat tablec,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg2c_qutable_c(void)
{
    return REG_READ(JPEG_REG2C_ADDR);
}

static inline void jpeg_ll_set_reg2c_qutable_c(uint32_t value)
{
    REG_WRITE(JPEG_REG2C_ADDR,value);
}

/* REG_0x2D */
#define JPEG_REG2D_ADDR  (JPEG_LL_REG_BASE  + 0x2D*4) //REG ADDR :0x480000b4
#define JPEG_REG2D_QUTABLE_D_POS (0) 
#define JPEG_REG2D_QUTABLE_D_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg2d_value(void)
{
    return REG_READ(JPEG_REG2D_ADDR);
}

static inline void jpeg_ll_set_reg2d_value(uint32_t value)
{
    REG_WRITE(JPEG_REG2D_ADDR,value);
}

/* REG_0x2d:reg2d->qutable_d:0x2d[31:0],quat tabled,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg2d_qutable_d(void)
{
    return REG_READ(JPEG_REG2D_ADDR);
}

static inline void jpeg_ll_set_reg2d_qutable_d(uint32_t value)
{
    REG_WRITE(JPEG_REG2D_ADDR,value);
}

/* REG_0x2E */
#define JPEG_REG2E_ADDR  (JPEG_LL_REG_BASE  + 0x2E*4) //REG ADDR :0x480000b8
#define JPEG_REG2E_QUTABLE_E_POS (0) 
#define JPEG_REG2E_QUTABLE_E_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg2e_value(void)
{
    return REG_READ(JPEG_REG2E_ADDR);
}

static inline void jpeg_ll_set_reg2e_value(uint32_t value)
{
    REG_WRITE(JPEG_REG2E_ADDR,value);
}

/* REG_0x2e:reg2e->qutable_e:0x2e[31:0],quat tablee,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg2e_qutable_e(void)
{
    return REG_READ(JPEG_REG2E_ADDR);
}

static inline void jpeg_ll_set_reg2e_qutable_e(uint32_t value)
{
    REG_WRITE(JPEG_REG2E_ADDR,value);
}

/* REG_0x2F */
#define JPEG_REG2F_ADDR  (JPEG_LL_REG_BASE  + 0x2F*4) //REG ADDR :0x480000bc
#define JPEG_REG2F_QUTABLE_F_POS (0) 
#define JPEG_REG2F_QUTABLE_F_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg2f_value(void)
{
    return REG_READ(JPEG_REG2F_ADDR);
}

static inline void jpeg_ll_set_reg2f_value(uint32_t value)
{
    REG_WRITE(JPEG_REG2F_ADDR,value);
}

/* REG_0x2f:reg2f->qutable_f:0x2f[31:0],quat tablef,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg2f_qutable_f(void)
{
    return REG_READ(JPEG_REG2F_ADDR);
}

static inline void jpeg_ll_set_reg2f_qutable_f(uint32_t value)
{
    REG_WRITE(JPEG_REG2F_ADDR,value);
}

/* REG_0x30 */
#define JPEG_REG30_ADDR  (JPEG_LL_REG_BASE  + 0x30*4) //REG ADDR :0x480000c0
#define JPEG_REG30_QUTABLE_10_POS (0) 
#define JPEG_REG30_QUTABLE_10_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg30_value(void)
{
    return REG_READ(JPEG_REG30_ADDR);
}

static inline void jpeg_ll_set_reg30_value(uint32_t value)
{
    REG_WRITE(JPEG_REG30_ADDR,value);
}

/* REG_0x30:reg30->qutable_10:0x30[31:0],quat table10,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg30_qutable_10(void)
{
    return REG_READ(JPEG_REG30_ADDR);
}

static inline void jpeg_ll_set_reg30_qutable_10(uint32_t value)
{
    REG_WRITE(JPEG_REG30_ADDR,value);
}

/* REG_0x31 */
#define JPEG_REG31_ADDR  (JPEG_LL_REG_BASE  + 0x31*4) //REG ADDR :0x480000c4
#define JPEG_REG31_QUTABLE_11_POS (0) 
#define JPEG_REG31_QUTABLE_11_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg31_value(void)
{
    return REG_READ(JPEG_REG31_ADDR);
}

static inline void jpeg_ll_set_reg31_value(uint32_t value)
{
    REG_WRITE(JPEG_REG31_ADDR,value);
}

/* REG_0x31:reg31->qutable_11:0x31[31:0],quat table11,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg31_qutable_11(void)
{
    return REG_READ(JPEG_REG31_ADDR);
}

static inline void jpeg_ll_set_reg31_qutable_11(uint32_t value)
{
    REG_WRITE(JPEG_REG31_ADDR,value);
}

/* REG_0x32 */
#define JPEG_REG32_ADDR  (JPEG_LL_REG_BASE  + 0x32*4) //REG ADDR :0x480000c8
#define JPEG_REG32_QUTABLE_12_POS (0) 
#define JPEG_REG32_QUTABLE_12_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg32_value(void)
{
    return REG_READ(JPEG_REG32_ADDR);
}

static inline void jpeg_ll_set_reg32_value(uint32_t value)
{
    REG_WRITE(JPEG_REG32_ADDR,value);
}

/* REG_0x32:reg32->qutable_12:0x32[31:0],quat table12,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg32_qutable_12(void)
{
    return REG_READ(JPEG_REG32_ADDR);
}

static inline void jpeg_ll_set_reg32_qutable_12(uint32_t value)
{
    REG_WRITE(JPEG_REG32_ADDR,value);
}

/* REG_0x33 */
#define JPEG_REG33_ADDR  (JPEG_LL_REG_BASE  + 0x33*4) //REG ADDR :0x480000cc
#define JPEG_REG33_QUTABLE_13_POS (0) 
#define JPEG_REG33_QUTABLE_13_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg33_value(void)
{
    return REG_READ(JPEG_REG33_ADDR);
}

static inline void jpeg_ll_set_reg33_value(uint32_t value)
{
    REG_WRITE(JPEG_REG33_ADDR,value);
}

/* REG_0x33:reg33->qutable_13:0x33[31:0],quat table13,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg33_qutable_13(void)
{
    return REG_READ(JPEG_REG33_ADDR);
}

static inline void jpeg_ll_set_reg33_qutable_13(uint32_t value)
{
    REG_WRITE(JPEG_REG33_ADDR,value);
}

/* REG_0x34 */
#define JPEG_REG34_ADDR  (JPEG_LL_REG_BASE  + 0x34*4) //REG ADDR :0x480000d0
#define JPEG_REG34_QUTABLE_14_POS (0) 
#define JPEG_REG34_QUTABLE_14_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg34_value(void)
{
    return REG_READ(JPEG_REG34_ADDR);
}

static inline void jpeg_ll_set_reg34_value(uint32_t value)
{
    REG_WRITE(JPEG_REG34_ADDR,value);
}

/* REG_0x34:reg34->qutable_14:0x34[31:0],quat table14,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg34_qutable_14(void)
{
    return REG_READ(JPEG_REG34_ADDR);
}

static inline void jpeg_ll_set_reg34_qutable_14(uint32_t value)
{
    REG_WRITE(JPEG_REG34_ADDR,value);
}

/* REG_0x35 */
#define JPEG_REG35_ADDR  (JPEG_LL_REG_BASE  + 0x35*4) //REG ADDR :0x480000d4
#define JPEG_REG35_QUTABLE_15_POS (0) 
#define JPEG_REG35_QUTABLE_15_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg35_value(void)
{
    return REG_READ(JPEG_REG35_ADDR);
}

static inline void jpeg_ll_set_reg35_value(uint32_t value)
{
    REG_WRITE(JPEG_REG35_ADDR,value);
}

/* REG_0x35:reg35->qutable_15:0x35[31:0],quat table15,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg35_qutable_15(void)
{
    return REG_READ(JPEG_REG35_ADDR);
}

static inline void jpeg_ll_set_reg35_qutable_15(uint32_t value)
{
    REG_WRITE(JPEG_REG35_ADDR,value);
}

/* REG_0x36 */
#define JPEG_REG36_ADDR  (JPEG_LL_REG_BASE  + 0x36*4) //REG ADDR :0x480000d8
#define JPEG_REG36_QUTABLE_16_POS (0) 
#define JPEG_REG36_QUTABLE_16_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg36_value(void)
{
    return REG_READ(JPEG_REG36_ADDR);
}

static inline void jpeg_ll_set_reg36_value(uint32_t value)
{
    REG_WRITE(JPEG_REG36_ADDR,value);
}

/* REG_0x36:reg36->qutable_16:0x36[31:0],quat table16,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg36_qutable_16(void)
{
    return REG_READ(JPEG_REG36_ADDR);
}

static inline void jpeg_ll_set_reg36_qutable_16(uint32_t value)
{
    REG_WRITE(JPEG_REG36_ADDR,value);
}

/* REG_0x37 */
#define JPEG_REG37_ADDR  (JPEG_LL_REG_BASE  + 0x37*4) //REG ADDR :0x480000dc
#define JPEG_REG37_QUTABLE_17_POS (0) 
#define JPEG_REG37_QUTABLE_17_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg37_value(void)
{
    return REG_READ(JPEG_REG37_ADDR);
}

static inline void jpeg_ll_set_reg37_value(uint32_t value)
{
    REG_WRITE(JPEG_REG37_ADDR,value);
}

/* REG_0x37:reg37->qutable_17:0x37[31:0],quat table17,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg37_qutable_17(void)
{
    return REG_READ(JPEG_REG37_ADDR);
}

static inline void jpeg_ll_set_reg37_qutable_17(uint32_t value)
{
    REG_WRITE(JPEG_REG37_ADDR,value);
}

/* REG_0x38 */
#define JPEG_REG38_ADDR  (JPEG_LL_REG_BASE  + 0x38*4) //REG ADDR :0x480000e0
#define JPEG_REG38_QUTABLE_18_POS (0) 
#define JPEG_REG38_QUTABLE_18_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg38_value(void)
{
    return REG_READ(JPEG_REG38_ADDR);
}

static inline void jpeg_ll_set_reg38_value(uint32_t value)
{
    REG_WRITE(JPEG_REG38_ADDR,value);
}

/* REG_0x38:reg38->qutable_18:0x38[31:0],quat table18,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg38_qutable_18(void)
{
    return REG_READ(JPEG_REG38_ADDR);
}

static inline void jpeg_ll_set_reg38_qutable_18(uint32_t value)
{
    REG_WRITE(JPEG_REG38_ADDR,value);
}

/* REG_0x39 */
#define JPEG_REG39_ADDR  (JPEG_LL_REG_BASE  + 0x39*4) //REG ADDR :0x480000e4
#define JPEG_REG39_QUTABLE_19_POS (0) 
#define JPEG_REG39_QUTABLE_19_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg39_value(void)
{
    return REG_READ(JPEG_REG39_ADDR);
}

static inline void jpeg_ll_set_reg39_value(uint32_t value)
{
    REG_WRITE(JPEG_REG39_ADDR,value);
}

/* REG_0x39:reg39->qutable_19:0x39[31:0],quat table19,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg39_qutable_19(void)
{
    return REG_READ(JPEG_REG39_ADDR);
}

static inline void jpeg_ll_set_reg39_qutable_19(uint32_t value)
{
    REG_WRITE(JPEG_REG39_ADDR,value);
}

/* REG_0x3A */
#define JPEG_REG3A_ADDR  (JPEG_LL_REG_BASE  + 0x3A*4) //REG ADDR :0x480000e8
#define JPEG_REG3A_QUTABLE_1A_POS (0) 
#define JPEG_REG3A_QUTABLE_1A_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg3a_value(void)
{
    return REG_READ(JPEG_REG3A_ADDR);
}

static inline void jpeg_ll_set_reg3a_value(uint32_t value)
{
    REG_WRITE(JPEG_REG3A_ADDR,value);
}

/* REG_0x3a:reg3a->qutable_1a:0x3a[31:0],quat table1a,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg3a_qutable_1a(void)
{
    return REG_READ(JPEG_REG3A_ADDR);
}

static inline void jpeg_ll_set_reg3a_qutable_1a(uint32_t value)
{
    REG_WRITE(JPEG_REG3A_ADDR,value);
}

/* REG_0x3B */
#define JPEG_REG3B_ADDR  (JPEG_LL_REG_BASE  + 0x3B*4) //REG ADDR :0x480000ec
#define JPEG_REG3B_QUTABLE_1B_POS (0) 
#define JPEG_REG3B_QUTABLE_1B_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg3b_value(void)
{
    return REG_READ(JPEG_REG3B_ADDR);
}

static inline void jpeg_ll_set_reg3b_value(uint32_t value)
{
    REG_WRITE(JPEG_REG3B_ADDR,value);
}

/* REG_0x3b:reg3b->qutable_1b:0x3b[31:0],quat table1b,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg3b_qutable_1b(void)
{
    return REG_READ(JPEG_REG3B_ADDR);
}

static inline void jpeg_ll_set_reg3b_qutable_1b(uint32_t value)
{
    REG_WRITE(JPEG_REG3B_ADDR,value);
}

/* REG_0x3C */
#define JPEG_REG3C_ADDR  (JPEG_LL_REG_BASE  + 0x3C*4) //REG ADDR :0x480000f0
#define JPEG_REG3C_QUTABLE_1C_POS (0) 
#define JPEG_REG3C_QUTABLE_1C_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg3c_value(void)
{
    return REG_READ(JPEG_REG3C_ADDR);
}

static inline void jpeg_ll_set_reg3c_value(uint32_t value)
{
    REG_WRITE(JPEG_REG3C_ADDR,value);
}

/* REG_0x3c:reg3c->qutable_1c:0x3c[31:0],quat table1c,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg3c_qutable_1c(void)
{
    return REG_READ(JPEG_REG3C_ADDR);
}

static inline void jpeg_ll_set_reg3c_qutable_1c(uint32_t value)
{
    REG_WRITE(JPEG_REG3C_ADDR,value);
}

/* REG_0x3D */
#define JPEG_REG3D_ADDR  (JPEG_LL_REG_BASE  + 0x3D*4) //REG ADDR :0x480000f4
#define JPEG_REG3D_QUTABLE_1D_POS (0) 
#define JPEG_REG3D_QUTABLE_1D_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg3d_value(void)
{
    return REG_READ(JPEG_REG3D_ADDR);
}

static inline void jpeg_ll_set_reg3d_value(uint32_t value)
{
    REG_WRITE(JPEG_REG3D_ADDR,value);
}

/* REG_0x3d:reg3d->qutable_1d:0x3d[31:0],quat table1d,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg3d_qutable_1d(void)
{
    return REG_READ(JPEG_REG3D_ADDR);
}

static inline void jpeg_ll_set_reg3d_qutable_1d(uint32_t value)
{
    REG_WRITE(JPEG_REG3D_ADDR,value);
}

/* REG_0x3E */
#define JPEG_REG3E_ADDR  (JPEG_LL_REG_BASE  + 0x3E*4) //REG ADDR :0x480000f8
#define JPEG_REG3E_QUTABLE_1E_POS (0) 
#define JPEG_REG3E_QUTABLE_1E_MASK (0xFFFFFFFF) 

static inline uint32_t jpeg_ll_get_reg3e_value(void)
{
    return REG_READ(JPEG_REG3E_ADDR);
}

static inline void jpeg_ll_set_reg3e_value(uint32_t value)
{
    REG_WRITE(JPEG_REG3E_ADDR,value);
}

/* REG_0x3e:reg3e->qutable_1e:0x3e[31:0],quat table1e,0x0,R/W*/
static inline uint32_t jpeg_ll_get_reg3e_qutable_1e(void)
{
    return REG_READ(JPEG_REG3E_ADDR);
}

static inline void jpeg_ll_set_reg3e_qutable_1e(uint32_t value)
{
    REG_WRITE(JPEG_REG3E_ADDR,value);
}

#ifdef __cplusplus 
}                  
#endif             
