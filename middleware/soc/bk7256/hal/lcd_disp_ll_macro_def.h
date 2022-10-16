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
* CHIP ID:BK7256,GENARATE TIME:2022-07-08 17:30:58                                                 
************************************************************************************************************************************/

#pragma once                 
                            
#ifdef __cplusplus          
extern "C" {              
#endif                      

#include <soc/soc.h>          

#define LCD_DISP_LL_REG_BASE      (SOC_LCD_DISP_REG_BASE) //REG_BASE:0x48060000

/* REG_0x00 */
#define LCD_DISP_DISPLAY_INT_ADDR  (LCD_DISP_LL_REG_BASE  + 0x0*4) //REG ADDR :0x48060000
#define LCD_DISP_DISPLAY_INT_RGB_INT_EN_POS (0) 
#define LCD_DISP_DISPLAY_INT_RGB_INT_EN_MASK (0x3) 

#define LCD_DISP_DISPLAY_INT_I8080_INT_EN_POS (2) 
#define LCD_DISP_DISPLAY_INT_I8080_INT_EN_MASK (0x3) 

#define LCD_DISP_DISPLAY_INT_RGB_SOF_POS (4) 
#define LCD_DISP_DISPLAY_INT_RGB_SOF_MASK (0x1) 

#define LCD_DISP_DISPLAY_INT_RGB_EOF_POS (5) 
#define LCD_DISP_DISPLAY_INT_RGB_EOF_MASK (0x1) 

#define LCD_DISP_DISPLAY_INT_I8080_SOF_POS (6) 
#define LCD_DISP_DISPLAY_INT_I8080_SOF_MASK (0x1) 

#define LCD_DISP_DISPLAY_INT_I8080_EOF_POS (7) 
#define LCD_DISP_DISPLAY_INT_I8080_EOF_MASK (0x1) 

#define LCD_DISP_DISPLAY_INT_RESERVED2_POS (8) 
#define LCD_DISP_DISPLAY_INT_RESERVED2_MASK (0xF) 

#define LCD_DISP_DISPLAY_INT_SOFT_RSET_POS (12) 
#define LCD_DISP_DISPLAY_INT_SOFT_RSET_MASK (0x1) 

#define LCD_DISP_DISPLAY_INT_RESERVED1_POS (13) 
#define LCD_DISP_DISPLAY_INT_RESERVED1_MASK (0x7FFF) 

#define LCD_DISP_DISPLAY_INT_DISCONTI_MODE_POS (28) 
#define LCD_DISP_DISPLAY_INT_DISCONTI_MODE_MASK (0x1) 

#define LCD_DISP_DISPLAY_INT_RESERVED0_POS (29) 
#define LCD_DISP_DISPLAY_INT_RESERVED0_MASK (0x7) 

static inline uint32_t lcd_disp_ll_get_display_int_value(void)
{
    return REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
}

static inline void lcd_disp_ll_set_display_int_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_DISPLAY_INT_ADDR,value);
}

/* REG_0x00:display_int->rgb_int_en:0x0[  1:  0],rgb display interrupt enable,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_display_int_rgb_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISPLAY_INT_RGB_INT_EN_POS) & LCD_DISP_DISPLAY_INT_RGB_INT_EN_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_display_int_rgb_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value &= ~(LCD_DISP_DISPLAY_INT_RGB_INT_EN_MASK << LCD_DISP_DISPLAY_INT_RGB_INT_EN_POS);
    reg_value |= ((value & LCD_DISP_DISPLAY_INT_RGB_INT_EN_MASK) << LCD_DISP_DISPLAY_INT_RGB_INT_EN_POS);
    REG_WRITE(LCD_DISP_DISPLAY_INT_ADDR,reg_value);
}

/* REG_0x00:display_int->i8080_int_en:0x0[  3:  2],i8080 display interrupt enable,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_display_int_i8080_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISPLAY_INT_I8080_INT_EN_POS) & LCD_DISP_DISPLAY_INT_I8080_INT_EN_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_display_int_i8080_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value &= ~(LCD_DISP_DISPLAY_INT_I8080_INT_EN_MASK << LCD_DISP_DISPLAY_INT_I8080_INT_EN_POS);
    reg_value |= ((value & LCD_DISP_DISPLAY_INT_I8080_INT_EN_MASK) << LCD_DISP_DISPLAY_INT_I8080_INT_EN_POS);
    REG_WRITE(LCD_DISP_DISPLAY_INT_ADDR,reg_value);
}

/* REG_0x00:display_int->rgb_sof:0x0[       4],rgb display output start of frame,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_display_int_rgb_sof(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISPLAY_INT_RGB_SOF_POS) & LCD_DISP_DISPLAY_INT_RGB_SOF_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_display_int_rgb_sof(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value &= ~(LCD_DISP_DISPLAY_INT_RGB_SOF_MASK << LCD_DISP_DISPLAY_INT_RGB_SOF_POS);
    reg_value |= ((value & LCD_DISP_DISPLAY_INT_RGB_SOF_MASK) << LCD_DISP_DISPLAY_INT_RGB_SOF_POS);
    REG_WRITE(LCD_DISP_DISPLAY_INT_ADDR,reg_value);
}

/* REG_0x00:display_int->rgb_eof:0x0[       5],rgb display output end of frame,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_display_int_rgb_eof(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISPLAY_INT_RGB_EOF_POS) & LCD_DISP_DISPLAY_INT_RGB_EOF_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_display_int_rgb_eof(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value &= ~(LCD_DISP_DISPLAY_INT_RGB_EOF_MASK << LCD_DISP_DISPLAY_INT_RGB_EOF_POS);
    reg_value |= ((value & LCD_DISP_DISPLAY_INT_RGB_EOF_MASK) << LCD_DISP_DISPLAY_INT_RGB_EOF_POS);
    REG_WRITE(LCD_DISP_DISPLAY_INT_ADDR,reg_value);
}

/* REG_0x00:display_int->i8080_sof:0x0[       6],8080 display output start of frame,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_display_int_i8080_sof(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISPLAY_INT_I8080_SOF_POS) & LCD_DISP_DISPLAY_INT_I8080_SOF_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_display_int_i8080_sof(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value &= ~(LCD_DISP_DISPLAY_INT_I8080_SOF_MASK << LCD_DISP_DISPLAY_INT_I8080_SOF_POS);
    reg_value |= ((value & LCD_DISP_DISPLAY_INT_I8080_SOF_MASK) << LCD_DISP_DISPLAY_INT_I8080_SOF_POS);
    REG_WRITE(LCD_DISP_DISPLAY_INT_ADDR,reg_value);
}

/* REG_0x00:display_int->i8080_eof:0x0[       7],8080 display output end of frame,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_display_int_i8080_eof(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISPLAY_INT_I8080_EOF_POS) & LCD_DISP_DISPLAY_INT_I8080_EOF_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_display_int_i8080_eof(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value &= ~(LCD_DISP_DISPLAY_INT_I8080_EOF_MASK << LCD_DISP_DISPLAY_INT_I8080_EOF_POS);
    reg_value |= ((value & LCD_DISP_DISPLAY_INT_I8080_EOF_MASK) << LCD_DISP_DISPLAY_INT_I8080_EOF_POS);
    REG_WRITE(LCD_DISP_DISPLAY_INT_ADDR,reg_value);
}

/* REG_0x00:display_int->soft_rset:0x0[     12],display模块的软复位操作，置1后模块复位，复位前关掉disp的使能位。,0x0,w*/
static inline void lcd_disp_ll_set_display_int_soft_rset(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value &= ~(LCD_DISP_DISPLAY_INT_SOFT_RSET_MASK << LCD_DISP_DISPLAY_INT_SOFT_RSET_POS);
    reg_value |= ((value & LCD_DISP_DISPLAY_INT_SOFT_RSET_MASK) << LCD_DISP_DISPLAY_INT_SOFT_RSET_POS);
    REG_WRITE(LCD_DISP_DISPLAY_INT_ADDR,reg_value);
}

/* REG_0x00:display_int->disconti_mode:0x0[     28],数据不连续模式，0：close，1：open。 Rgb模式：rgb clk到25mhz时，disconti需要打开,不然速度跟不上;I8080模式：请使用disconti_mode.,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_display_int_disconti_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISPLAY_INT_DISCONTI_MODE_POS) & LCD_DISP_DISPLAY_INT_DISCONTI_MODE_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_display_int_disconti_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value &= ~(LCD_DISP_DISPLAY_INT_DISCONTI_MODE_MASK << LCD_DISP_DISPLAY_INT_DISCONTI_MODE_POS);
    reg_value |= ((value & LCD_DISP_DISPLAY_INT_DISCONTI_MODE_MASK) << LCD_DISP_DISPLAY_INT_DISCONTI_MODE_POS);
    REG_WRITE(LCD_DISP_DISPLAY_INT_ADDR,reg_value);
}

/* REG_0x01 */
#define LCD_DISP_RGB_CFG_ADDR  (LCD_DISP_LL_REG_BASE  + 0x1*4) //REG ADDR :0x48060004
#define LCD_DISP_RGB_CFG_X_PIXEL_POS (0) 
#define LCD_DISP_RGB_CFG_X_PIXEL_MASK (0x7FF) 

#define LCD_DISP_RGB_CFG_DCLK_REV_POS (11) 
#define LCD_DISP_RGB_CFG_DCLK_REV_MASK (0x1) 

#define LCD_DISP_RGB_CFG_Y_PIXEL_POS (12) 
#define LCD_DISP_RGB_CFG_Y_PIXEL_MASK (0x7FF) 

#define LCD_DISP_RGB_CFG_RESERVED1_POS (23) 
#define LCD_DISP_RGB_CFG_RESERVED1_MASK (0x1) 

#define LCD_DISP_RGB_CFG_RGB_DISP_ON_POS (24) 
#define LCD_DISP_RGB_CFG_RGB_DISP_ON_MASK (0x1) 

#define LCD_DISP_RGB_CFG_RGB_ON_POS (25) 
#define LCD_DISP_RGB_CFG_RGB_ON_MASK (0x1) 

#define LCD_DISP_RGB_CFG_LCD_DISPLAY_ON_POS (26) 
#define LCD_DISP_RGB_CFG_LCD_DISPLAY_ON_MASK (0x1) 

#define LCD_DISP_RGB_CFG_RESERVED0_POS (27) 
#define LCD_DISP_RGB_CFG_RESERVED0_MASK (0x1F) 

static inline uint32_t lcd_disp_ll_get_rgb_cfg_value(void)
{
    return REG_READ(LCD_DISP_RGB_CFG_ADDR);
}

static inline void lcd_disp_ll_set_rgb_cfg_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_RGB_CFG_ADDR,value);
}

/* REG_0x01:rgb_cfg->x_pixel:0x1[10:  0],行像素长度,0x1e0,r/w*/
static inline uint32_t lcd_disp_ll_get_rgb_cfg_x_pixel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_CFG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_RGB_CFG_X_PIXEL_POS) & LCD_DISP_RGB_CFG_X_PIXEL_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_rgb_cfg_x_pixel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_CFG_ADDR);
    reg_value &= ~(LCD_DISP_RGB_CFG_X_PIXEL_MASK << LCD_DISP_RGB_CFG_X_PIXEL_POS);
    reg_value |= ((value & LCD_DISP_RGB_CFG_X_PIXEL_MASK) << LCD_DISP_RGB_CFG_X_PIXEL_POS);
    REG_WRITE(LCD_DISP_RGB_CFG_ADDR,reg_value);
}

/* REG_0x01:rgb_cfg->dclk_rev:0x1[     11],输出数据是随着时钟上升沿输出还是下降沿输出。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_rgb_cfg_dclk_rev(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_CFG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_RGB_CFG_DCLK_REV_POS) & LCD_DISP_RGB_CFG_DCLK_REV_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_rgb_cfg_dclk_rev(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_CFG_ADDR);
    reg_value &= ~(LCD_DISP_RGB_CFG_DCLK_REV_MASK << LCD_DISP_RGB_CFG_DCLK_REV_POS);
    reg_value |= ((value & LCD_DISP_RGB_CFG_DCLK_REV_MASK) << LCD_DISP_RGB_CFG_DCLK_REV_POS);
    REG_WRITE(LCD_DISP_RGB_CFG_ADDR,reg_value);
}

/* REG_0x01:rgb_cfg->y_pixel:0x1[22:12],列像素长度,0x110,r/w*/
static inline uint32_t lcd_disp_ll_get_rgb_cfg_y_pixel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_CFG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_RGB_CFG_Y_PIXEL_POS) & LCD_DISP_RGB_CFG_Y_PIXEL_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_rgb_cfg_y_pixel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_CFG_ADDR);
    reg_value &= ~(LCD_DISP_RGB_CFG_Y_PIXEL_MASK << LCD_DISP_RGB_CFG_Y_PIXEL_POS);
    reg_value |= ((value & LCD_DISP_RGB_CFG_Y_PIXEL_MASK) << LCD_DISP_RGB_CFG_Y_PIXEL_POS);
    REG_WRITE(LCD_DISP_RGB_CFG_ADDR,reg_value);
}

/* REG_0x01:rgb_cfg->rgb_disp_on:0x1[     24],rgb display enable,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_rgb_cfg_rgb_disp_on(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_CFG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_RGB_CFG_RGB_DISP_ON_POS) & LCD_DISP_RGB_CFG_RGB_DISP_ON_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_rgb_cfg_rgb_disp_on(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_CFG_ADDR);
    reg_value &= ~(LCD_DISP_RGB_CFG_RGB_DISP_ON_MASK << LCD_DISP_RGB_CFG_RGB_DISP_ON_POS);
    reg_value |= ((value & LCD_DISP_RGB_CFG_RGB_DISP_ON_MASK) << LCD_DISP_RGB_CFG_RGB_DISP_ON_POS);
    REG_WRITE(LCD_DISP_RGB_CFG_ADDR,reg_value);
}

/* REG_0x01:rgb_cfg->rgb_on:0x1[     25],rgb和i8080的功能选择，1：rgb模块，0：i8080模块。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_rgb_cfg_rgb_on(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_CFG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_RGB_CFG_RGB_ON_POS) & LCD_DISP_RGB_CFG_RGB_ON_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_rgb_cfg_rgb_on(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_CFG_ADDR);
    reg_value &= ~(LCD_DISP_RGB_CFG_RGB_ON_MASK << LCD_DISP_RGB_CFG_RGB_ON_POS);
    reg_value |= ((value & LCD_DISP_RGB_CFG_RGB_ON_MASK) << LCD_DISP_RGB_CFG_RGB_ON_POS);
    REG_WRITE(LCD_DISP_RGB_CFG_ADDR,reg_value);
}

/* REG_0x01:rgb_cfg->lcd_display_on:0x1[     26],lcd屏幕打开引脚，是disp_on引脚的直接输出,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_rgb_cfg_lcd_display_on(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_CFG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_RGB_CFG_LCD_DISPLAY_ON_POS) & LCD_DISP_RGB_CFG_LCD_DISPLAY_ON_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_rgb_cfg_lcd_display_on(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_CFG_ADDR);
    reg_value &= ~(LCD_DISP_RGB_CFG_LCD_DISPLAY_ON_MASK << LCD_DISP_RGB_CFG_LCD_DISPLAY_ON_POS);
    reg_value |= ((value & LCD_DISP_RGB_CFG_LCD_DISPLAY_ON_MASK) << LCD_DISP_RGB_CFG_LCD_DISPLAY_ON_POS);
    REG_WRITE(LCD_DISP_RGB_CFG_ADDR,reg_value);
}

/* REG_0x02 */
#define LCD_DISP_RGB_FIFO_ADDR  (LCD_DISP_LL_REG_BASE  + 0x2*4) //REG ADDR :0x48060008
#define LCD_DISP_RGB_FIFO_RESERVED_POS (0) 
#define LCD_DISP_RGB_FIFO_RESERVED_MASK (0xFFFFFFFF) 

/* REG_0x03 */
#define LCD_DISP_HSYNC_VSYNC_ADDR  (LCD_DISP_LL_REG_BASE  + 0x3*4) //REG ADDR :0x4806000c
#define LCD_DISP_HSYNC_VSYNC_HSYNC_BACK_PORCH_POS (0) 
#define LCD_DISP_HSYNC_VSYNC_HSYNC_BACK_PORCH_MASK (0xFF) 

#define LCD_DISP_HSYNC_VSYNC_HSYNC_FRONT_PORCH_POS (8) 
#define LCD_DISP_HSYNC_VSYNC_HSYNC_FRONT_PORCH_MASK (0x7F) 

#define LCD_DISP_HSYNC_VSYNC_VSYNC_BACK_PORCH_POS (15) 
#define LCD_DISP_HSYNC_VSYNC_VSYNC_BACK_PORCH_MASK (0x1F) 

#define LCD_DISP_HSYNC_VSYNC_VSYNC_FRONT_PORCH_POS (20) 
#define LCD_DISP_HSYNC_VSYNC_VSYNC_FRONT_PORCH_MASK (0x7F) 

#define LCD_DISP_HSYNC_VSYNC_RESERVED1_POS (27) 
#define LCD_DISP_HSYNC_VSYNC_RESERVED1_MASK (0x1) 

#define LCD_DISP_HSYNC_VSYNC_YUV_SEL_POS (28) 
#define LCD_DISP_HSYNC_VSYNC_YUV_SEL_MASK (0x7) 

#define LCD_DISP_HSYNC_VSYNC_RESERVED0_POS (31) 
#define LCD_DISP_HSYNC_VSYNC_RESERVED0_MASK (0x1) 

static inline uint32_t lcd_disp_ll_get_hsync_vsync_value(void)
{
    return REG_READ(LCD_DISP_HSYNC_VSYNC_ADDR);
}

static inline void lcd_disp_ll_set_hsync_vsync_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_HSYNC_VSYNC_ADDR,value);
}

/* REG_0x03:hsync_vsync->hsync_back_porch:0x3[  7:  0],hsync拉高至load图像数据的mclk symbol数。,0x28,r/w*/
static inline uint32_t lcd_disp_ll_get_hsync_vsync_hsync_back_porch(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_HSYNC_VSYNC_ADDR);
    reg_value = ((reg_value >> LCD_DISP_HSYNC_VSYNC_HSYNC_BACK_PORCH_POS) & LCD_DISP_HSYNC_VSYNC_HSYNC_BACK_PORCH_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_hsync_vsync_hsync_back_porch(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_HSYNC_VSYNC_ADDR);
    reg_value &= ~(LCD_DISP_HSYNC_VSYNC_HSYNC_BACK_PORCH_MASK << LCD_DISP_HSYNC_VSYNC_HSYNC_BACK_PORCH_POS);
    reg_value |= ((value & LCD_DISP_HSYNC_VSYNC_HSYNC_BACK_PORCH_MASK) << LCD_DISP_HSYNC_VSYNC_HSYNC_BACK_PORCH_POS);
    REG_WRITE(LCD_DISP_HSYNC_VSYNC_ADDR,reg_value);
}

/* REG_0x03:hsync_vsync->hsync_front_porch:0x3[14:  8],hsync从数据结束到hsync信号拉低的mclk symbol数。,0x5,r/w*/
static inline uint32_t lcd_disp_ll_get_hsync_vsync_hsync_front_porch(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_HSYNC_VSYNC_ADDR);
    reg_value = ((reg_value >> LCD_DISP_HSYNC_VSYNC_HSYNC_FRONT_PORCH_POS) & LCD_DISP_HSYNC_VSYNC_HSYNC_FRONT_PORCH_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_hsync_vsync_hsync_front_porch(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_HSYNC_VSYNC_ADDR);
    reg_value &= ~(LCD_DISP_HSYNC_VSYNC_HSYNC_FRONT_PORCH_MASK << LCD_DISP_HSYNC_VSYNC_HSYNC_FRONT_PORCH_POS);
    reg_value |= ((value & LCD_DISP_HSYNC_VSYNC_HSYNC_FRONT_PORCH_MASK) << LCD_DISP_HSYNC_VSYNC_HSYNC_FRONT_PORCH_POS);
    REG_WRITE(LCD_DISP_HSYNC_VSYNC_ADDR,reg_value);
}

/* REG_0x03:hsync_vsync->vsync_back_porch:0x3[19:15],vsync拉高至load hsync图像数据的hsync symbol数。,0x8,r/w*/
static inline uint32_t lcd_disp_ll_get_hsync_vsync_vsync_back_porch(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_HSYNC_VSYNC_ADDR);
    reg_value = ((reg_value >> LCD_DISP_HSYNC_VSYNC_VSYNC_BACK_PORCH_POS) & LCD_DISP_HSYNC_VSYNC_VSYNC_BACK_PORCH_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_hsync_vsync_vsync_back_porch(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_HSYNC_VSYNC_ADDR);
    reg_value &= ~(LCD_DISP_HSYNC_VSYNC_VSYNC_BACK_PORCH_MASK << LCD_DISP_HSYNC_VSYNC_VSYNC_BACK_PORCH_POS);
    reg_value |= ((value & LCD_DISP_HSYNC_VSYNC_VSYNC_BACK_PORCH_MASK) << LCD_DISP_HSYNC_VSYNC_VSYNC_BACK_PORCH_POS);
    REG_WRITE(LCD_DISP_HSYNC_VSYNC_ADDR,reg_value);
}

/* REG_0x03:hsync_vsync->vsync_front_porch:0x3[26:20],vsync从行数据结束到vsync信号拉低的hsync sym数。,0x8,r/w*/
static inline uint32_t lcd_disp_ll_get_hsync_vsync_vsync_front_porch(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_HSYNC_VSYNC_ADDR);
    reg_value = ((reg_value >> LCD_DISP_HSYNC_VSYNC_VSYNC_FRONT_PORCH_POS) & LCD_DISP_HSYNC_VSYNC_VSYNC_FRONT_PORCH_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_hsync_vsync_vsync_front_porch(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_HSYNC_VSYNC_ADDR);
    reg_value &= ~(LCD_DISP_HSYNC_VSYNC_VSYNC_FRONT_PORCH_MASK << LCD_DISP_HSYNC_VSYNC_VSYNC_FRONT_PORCH_POS);
    reg_value |= ((value & LCD_DISP_HSYNC_VSYNC_VSYNC_FRONT_PORCH_MASK) << LCD_DISP_HSYNC_VSYNC_VSYNC_FRONT_PORCH_POS);
    REG_WRITE(LCD_DISP_HSYNC_VSYNC_ADDR,reg_value);
}

/* REG_0x03:hsync_vsync->yuv_sel:0x3[30:28],rgb屏幕的输入格式选择，000：default，rgb565；001：YUYV；010: UYVY；011: YYUV；100: UVYY；101: VUYY。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_hsync_vsync_yuv_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_HSYNC_VSYNC_ADDR);
    reg_value = ((reg_value >> LCD_DISP_HSYNC_VSYNC_YUV_SEL_POS) & LCD_DISP_HSYNC_VSYNC_YUV_SEL_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_hsync_vsync_yuv_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_HSYNC_VSYNC_ADDR);
    reg_value &= ~(LCD_DISP_HSYNC_VSYNC_YUV_SEL_MASK << LCD_DISP_HSYNC_VSYNC_YUV_SEL_POS);
    reg_value |= ((value & LCD_DISP_HSYNC_VSYNC_YUV_SEL_MASK) << LCD_DISP_HSYNC_VSYNC_YUV_SEL_POS);
    REG_WRITE(LCD_DISP_HSYNC_VSYNC_ADDR,reg_value);
}

/* REG_0x04 */
#define LCD_DISP_I8080_CONFIG_ADDR  (LCD_DISP_LL_REG_BASE  + 0x4*4) //REG ADDR :0x48060010
#define LCD_DISP_I8080_CONFIG_I8080_DISP_EN_POS (0) 
#define LCD_DISP_I8080_CONFIG_I8080_DISP_EN_MASK (0x1) 

#define LCD_DISP_I8080_CONFIG_I8080_DAT_ON_POS (1) 
#define LCD_DISP_I8080_CONFIG_I8080_DAT_ON_MASK (0x1) 

#define LCD_DISP_I8080_CONFIG_I8080_FIFO_MODE_POS (2) 
#define LCD_DISP_I8080_CONFIG_I8080_FIFO_MODE_MASK (0x1) 

#define LCD_DISP_I8080_CONFIG_RESERVED3_POS (3) 
#define LCD_DISP_I8080_CONFIG_RESERVED3_MASK (0x3) 

#define LCD_DISP_I8080_CONFIG_RESET_SLEEP_IN_POS (5) 
#define LCD_DISP_I8080_CONFIG_RESET_SLEEP_IN_MASK (0x1) 

#define LCD_DISP_I8080_CONFIG_RESERVED2_POS (6) 
#define LCD_DISP_I8080_CONFIG_RESERVED2_MASK (0x3) 

#define LCD_DISP_I8080_CONFIG_TIK_CNT_POS (8) 
#define LCD_DISP_I8080_CONFIG_TIK_CNT_MASK (0x3) 

#define LCD_DISP_I8080_CONFIG_RESERVED1_POS (10) 
#define LCD_DISP_I8080_CONFIG_RESERVED1_MASK (0x3) 

#define LCD_DISP_I8080_CONFIG_I8080_1MS_COUNT_POS (12) 
#define LCD_DISP_I8080_CONFIG_I8080_1MS_COUNT_MASK (0x1FF) 

#define LCD_DISP_I8080_CONFIG_RESERVED0_POS (21) 
#define LCD_DISP_I8080_CONFIG_RESERVED0_MASK (0x7FF) 

static inline uint32_t lcd_disp_ll_get_i8080_config_value(void)
{
    return REG_READ(LCD_DISP_I8080_CONFIG_ADDR);
}

static inline void lcd_disp_ll_set_i8080_config_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_I8080_CONFIG_ADDR,value);
}

/* REG_0x04:i8080_config->i8080_disp_en:0x4[       0],i8080 function enable,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_i8080_config_i8080_disp_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CONFIG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_I8080_CONFIG_I8080_DISP_EN_POS) & LCD_DISP_I8080_CONFIG_I8080_DISP_EN_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_i8080_config_i8080_disp_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CONFIG_ADDR);
    reg_value &= ~(LCD_DISP_I8080_CONFIG_I8080_DISP_EN_MASK << LCD_DISP_I8080_CONFIG_I8080_DISP_EN_POS);
    reg_value |= ((value & LCD_DISP_I8080_CONFIG_I8080_DISP_EN_MASK) << LCD_DISP_I8080_CONFIG_I8080_DISP_EN_POS);
    REG_WRITE(LCD_DISP_I8080_CONFIG_ADDR,reg_value);
}

/* REG_0x04:i8080_config->i8080_dat_on:0x4[       1],i8080 data start transfer，初始化完需要传输数据时置位。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_i8080_config_i8080_dat_on(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CONFIG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_I8080_CONFIG_I8080_DAT_ON_POS) & LCD_DISP_I8080_CONFIG_I8080_DAT_ON_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_i8080_config_i8080_dat_on(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CONFIG_ADDR);
    reg_value &= ~(LCD_DISP_I8080_CONFIG_I8080_DAT_ON_MASK << LCD_DISP_I8080_CONFIG_I8080_DAT_ON_POS);
    reg_value |= ((value & LCD_DISP_I8080_CONFIG_I8080_DAT_ON_MASK) << LCD_DISP_I8080_CONFIG_I8080_DAT_ON_POS);
    REG_WRITE(LCD_DISP_I8080_CONFIG_ADDR,reg_value);
}

/* REG_0x04:i8080_config->i8080_fifo_mode:0x4[       2],I8080的fifo写使能。0：数据只有在i8080_disp_en为1时可以输出。 1：任何时候。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_i8080_config_i8080_fifo_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CONFIG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_I8080_CONFIG_I8080_FIFO_MODE_POS) & LCD_DISP_I8080_CONFIG_I8080_FIFO_MODE_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_i8080_config_i8080_fifo_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CONFIG_ADDR);
    reg_value &= ~(LCD_DISP_I8080_CONFIG_I8080_FIFO_MODE_MASK << LCD_DISP_I8080_CONFIG_I8080_FIFO_MODE_POS);
    reg_value |= ((value & LCD_DISP_I8080_CONFIG_I8080_FIFO_MODE_MASK) << LCD_DISP_I8080_CONFIG_I8080_FIFO_MODE_POS);
    REG_WRITE(LCD_DISP_I8080_CONFIG_ADDR,reg_value);
}

/* REG_0x04:i8080_config->reset_sleep_in:0x4[       5],屏幕初始化阶段的复位，1ms后reset拉低10ms，之后稳定等待120ms稳定。共131ms。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_i8080_config_reset_sleep_in(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CONFIG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_I8080_CONFIG_RESET_SLEEP_IN_POS) & LCD_DISP_I8080_CONFIG_RESET_SLEEP_IN_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_i8080_config_reset_sleep_in(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CONFIG_ADDR);
    reg_value &= ~(LCD_DISP_I8080_CONFIG_RESET_SLEEP_IN_MASK << LCD_DISP_I8080_CONFIG_RESET_SLEEP_IN_POS);
    reg_value |= ((value & LCD_DISP_I8080_CONFIG_RESET_SLEEP_IN_MASK) << LCD_DISP_I8080_CONFIG_RESET_SLEEP_IN_POS);
    REG_WRITE(LCD_DISP_I8080_CONFIG_ADDR,reg_value);
}

/* REG_0x04:i8080_config->tik_cnt:0x4[  9:  8],db输出的持续时间。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_i8080_config_tik_cnt(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CONFIG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_I8080_CONFIG_TIK_CNT_POS) & LCD_DISP_I8080_CONFIG_TIK_CNT_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_i8080_config_tik_cnt(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CONFIG_ADDR);
    reg_value &= ~(LCD_DISP_I8080_CONFIG_TIK_CNT_MASK << LCD_DISP_I8080_CONFIG_TIK_CNT_POS);
    reg_value |= ((value & LCD_DISP_I8080_CONFIG_TIK_CNT_MASK) << LCD_DISP_I8080_CONFIG_TIK_CNT_POS);
    REG_WRITE(LCD_DISP_I8080_CONFIG_ADDR,reg_value);
}

/* REG_0x04:i8080_config->i8080_1ms_count:0x4[20:12],i8080需要初始化前复位，根据时钟速率设置计数器的大小，为count*1000。96mhz：96；160mhz：160；,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_i8080_config_i8080_1ms_count(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CONFIG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_I8080_CONFIG_I8080_1MS_COUNT_POS) & LCD_DISP_I8080_CONFIG_I8080_1MS_COUNT_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_i8080_config_i8080_1ms_count(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CONFIG_ADDR);
    reg_value &= ~(LCD_DISP_I8080_CONFIG_I8080_1MS_COUNT_MASK << LCD_DISP_I8080_CONFIG_I8080_1MS_COUNT_POS);
    reg_value |= ((value & LCD_DISP_I8080_CONFIG_I8080_1MS_COUNT_MASK) << LCD_DISP_I8080_CONFIG_I8080_1MS_COUNT_POS);
    REG_WRITE(LCD_DISP_I8080_CONFIG_ADDR,reg_value);
}

/* REG_0x05 */
#define LCD_DISP_I8080_CMD_FIFO_ADDR  (LCD_DISP_LL_REG_BASE  + 0x5*4) //REG ADDR :0x48060014
#define LCD_DISP_I8080_CMD_FIFO_I8080_CMD_FIFO_POS (0) 
#define LCD_DISP_I8080_CMD_FIFO_I8080_CMD_FIFO_MASK (0xFFFF) 

#define LCD_DISP_I8080_CMD_FIFO_RESERVED_POS (16) 
#define LCD_DISP_I8080_CMD_FIFO_RESERVED_MASK (0xFFFF) 

static inline void lcd_disp_ll_set_i8080_cmd_fifo_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_I8080_CMD_FIFO_ADDR,value);
}

/* REG_0x05:i8080_cmd_fifo->i8080_cmd_fifo:0x5[15:  0],i8080 command fifo，初始化和持续读写的cmd写入此fifo。,0x0,w*/
static inline void lcd_disp_ll_set_i8080_cmd_fifo_i8080_cmd_fifo(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CMD_FIFO_ADDR);
    reg_value &= ~(LCD_DISP_I8080_CMD_FIFO_I8080_CMD_FIFO_MASK << LCD_DISP_I8080_CMD_FIFO_I8080_CMD_FIFO_POS);
    reg_value |= ((value & LCD_DISP_I8080_CMD_FIFO_I8080_CMD_FIFO_MASK) << LCD_DISP_I8080_CMD_FIFO_I8080_CMD_FIFO_POS);
    REG_WRITE(LCD_DISP_I8080_CMD_FIFO_ADDR,reg_value);
}

/* REG_0x06 */
#define LCD_DISP_I8080_DAT_FIFO_ADDR  (LCD_DISP_LL_REG_BASE  + 0x6*4) //REG ADDR :0x48060018
#define LCD_DISP_I8080_DAT_FIFO_I8080_DAT_FIFO_POS (0) 
#define LCD_DISP_I8080_DAT_FIFO_I8080_DAT_FIFO_MASK (0xFFFFFFFF) 

static inline uint32_t lcd_disp_ll_get_i8080_dat_fifo_value(void)
{
    return REG_READ(LCD_DISP_I8080_DAT_FIFO_ADDR);
}

static inline void lcd_disp_ll_set_i8080_dat_fifo_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_I8080_DAT_FIFO_ADDR,value);
}

/* REG_0x06:i8080_dat_fifo->i8080_dat_fifo:0x6[31:  0],i8080 data fifo，初始化的dat和像素数据写入此fifo。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_i8080_dat_fifo_i8080_dat_fifo(void)
{
    return REG_READ(LCD_DISP_I8080_DAT_FIFO_ADDR);
}

static inline void lcd_disp_ll_set_i8080_dat_fifo_i8080_dat_fifo(uint32_t value)
{
    REG_WRITE(LCD_DISP_I8080_DAT_FIFO_ADDR,value);
}

/* REG_0x07 */
#define LCD_DISP_I8080_THRD_ADDR  (LCD_DISP_LL_REG_BASE  + 0x7*4) //REG ADDR :0x4806001c
#define LCD_DISP_I8080_THRD_RESERVED1_POS (0) 
#define LCD_DISP_I8080_THRD_RESERVED1_MASK (0xFF) 

#define LCD_DISP_I8080_THRD_CMD_WR_THRD_POS (8) 
#define LCD_DISP_I8080_THRD_CMD_WR_THRD_MASK (0xFF) 

#define LCD_DISP_I8080_THRD_RESERVED0_POS (16) 
#define LCD_DISP_I8080_THRD_RESERVED0_MASK (0xFF) 

#define LCD_DISP_I8080_THRD_CMD_RD_THRD_POS (24) 
#define LCD_DISP_I8080_THRD_CMD_RD_THRD_MASK (0xFF) 

static inline uint32_t lcd_disp_ll_get_i8080_thrd_value(void)
{
    return REG_READ(LCD_DISP_I8080_THRD_ADDR);
}

static inline void lcd_disp_ll_set_i8080_thrd_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_I8080_THRD_ADDR,value);
}

/* REG_0x07:i8080_thrd->cmd_wr_thrd:0x7[15:  8],i8080 cmd fifo wr thrd,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_i8080_thrd_cmd_wr_thrd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_THRD_ADDR);
    reg_value = ((reg_value >> LCD_DISP_I8080_THRD_CMD_WR_THRD_POS) & LCD_DISP_I8080_THRD_CMD_WR_THRD_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_i8080_thrd_cmd_wr_thrd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_THRD_ADDR);
    reg_value &= ~(LCD_DISP_I8080_THRD_CMD_WR_THRD_MASK << LCD_DISP_I8080_THRD_CMD_WR_THRD_POS);
    reg_value |= ((value & LCD_DISP_I8080_THRD_CMD_WR_THRD_MASK) << LCD_DISP_I8080_THRD_CMD_WR_THRD_POS);
    REG_WRITE(LCD_DISP_I8080_THRD_ADDR,reg_value);
}

/* REG_0x07:i8080_thrd->cmd_rd_thrd:0x7[31:24],i8080 cmd fifo rd thrd,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_i8080_thrd_cmd_rd_thrd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_THRD_ADDR);
    reg_value = ((reg_value >> LCD_DISP_I8080_THRD_CMD_RD_THRD_POS) & LCD_DISP_I8080_THRD_CMD_RD_THRD_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_i8080_thrd_cmd_rd_thrd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_THRD_ADDR);
    reg_value &= ~(LCD_DISP_I8080_THRD_CMD_RD_THRD_MASK << LCD_DISP_I8080_THRD_CMD_RD_THRD_POS);
    reg_value |= ((value & LCD_DISP_I8080_THRD_CMD_RD_THRD_MASK) << LCD_DISP_I8080_THRD_CMD_RD_THRD_POS);
    REG_WRITE(LCD_DISP_I8080_THRD_ADDR,reg_value);
}

/* REG_0x08 */
#define LCD_DISP_DISP_STATUS_ADDR  (LCD_DISP_LL_REG_BASE  + 0x8*4) //REG ADDR :0x48060020
#define LCD_DISP_DISP_STATUS_RGB_VER_CNT_POS (0) 
#define LCD_DISP_DISP_STATUS_RGB_VER_CNT_MASK (0x7FF) 

#define LCD_DISP_DISP_STATUS_I8080_CMD_CFG_DONE_POS (11) 
#define LCD_DISP_DISP_STATUS_I8080_CMD_CFG_DONE_MASK (0x1) 

#define LCD_DISP_DISP_STATUS_I8080_VER_CNT_POS (12) 
#define LCD_DISP_DISP_STATUS_I8080_VER_CNT_MASK (0x7FF) 

#define LCD_DISP_DISP_STATUS_DISP_FIFO_EMPTY_POS (23) 
#define LCD_DISP_DISP_STATUS_DISP_FIFO_EMPTY_MASK (0x1) 

#define LCD_DISP_DISP_STATUS_DISP_FIFO_NEAR_FULL_POS (24) 
#define LCD_DISP_DISP_STATUS_DISP_FIFO_NEAR_FULL_MASK (0x1) 

#define LCD_DISP_DISP_STATUS_RESERVED_POS (25) 
#define LCD_DISP_DISP_STATUS_RESERVED_MASK (0x7F) 

static inline uint32_t lcd_disp_ll_get_disp_status_value(void)
{
    return REG_READ(LCD_DISP_DISP_STATUS_ADDR);
}

/* REG_0x08:disp_status->rgb_ver_cnt:0x8[10:  0],rgb模块输出列状态查询。,0x0,r*/
static inline uint32_t lcd_disp_ll_get_disp_status_rgb_ver_cnt(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISP_STATUS_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISP_STATUS_RGB_VER_CNT_POS)&LCD_DISP_DISP_STATUS_RGB_VER_CNT_MASK);
    return reg_value;
}

/* REG_0x08:disp_status->i8080_cmd_cfg_done:0x8[     11],i8080模块cmd命令的parameter写完成查询。,0x0,r*/
static inline uint32_t lcd_disp_ll_get_disp_status_i8080_cmd_cfg_done(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISP_STATUS_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISP_STATUS_I8080_CMD_CFG_DONE_POS)&LCD_DISP_DISP_STATUS_I8080_CMD_CFG_DONE_MASK);
    return reg_value;
}

/* REG_0x08:disp_status->i8080_ver_cnt:0x8[22:12],i8080模块输出列状态查询。,0x0,r*/
static inline uint32_t lcd_disp_ll_get_disp_status_i8080_ver_cnt(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISP_STATUS_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISP_STATUS_I8080_VER_CNT_POS)&LCD_DISP_DISP_STATUS_I8080_VER_CNT_MASK);
    return reg_value;
}

/* REG_0x08:disp_status->disp_fifo_empty:0x8[     23],disp的data fifo空状态位。,0x0,r*/
static inline uint32_t lcd_disp_ll_get_disp_status_disp_fifo_empty(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISP_STATUS_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISP_STATUS_DISP_FIFO_EMPTY_POS)&LCD_DISP_DISP_STATUS_DISP_FIFO_EMPTY_MASK);
    return reg_value;
}

/* REG_0x08:disp_status->disp_fifo_near_full:0x8[     24],disp的data fifo仅满状态位。,0x0,r*/
static inline uint32_t lcd_disp_ll_get_disp_status_disp_fifo_near_full(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISP_STATUS_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISP_STATUS_DISP_FIFO_NEAR_FULL_POS)&LCD_DISP_DISP_STATUS_DISP_FIFO_NEAR_FULL_MASK);
    return reg_value;
}

/* REG_0x09 */
#define LCD_DISP_RGB_SYNC_LOW_ADDR  (LCD_DISP_LL_REG_BASE  + 0x9*4) //REG ADDR :0x48060024
#define LCD_DISP_RGB_SYNC_LOW_HSYNC_BACK_LOW_POS (0) 
#define LCD_DISP_RGB_SYNC_LOW_HSYNC_BACK_LOW_MASK (0x3F) 

#define LCD_DISP_RGB_SYNC_LOW_RESERVED2_POS (6) 
#define LCD_DISP_RGB_SYNC_LOW_RESERVED2_MASK (0x3) 

#define LCD_DISP_RGB_SYNC_LOW_VSYNC_BACK_LOW_POS (8) 
#define LCD_DISP_RGB_SYNC_LOW_VSYNC_BACK_LOW_MASK (0x3F) 

#define LCD_DISP_RGB_SYNC_LOW_RESERVED1_POS (14) 
#define LCD_DISP_RGB_SYNC_LOW_RESERVED1_MASK (0x3) 

#define LCD_DISP_RGB_SYNC_LOW_PFC_PIXEL_REVE_POS (16) 
#define LCD_DISP_RGB_SYNC_LOW_PFC_PIXEL_REVE_MASK (0x1) 

#define LCD_DISP_RGB_SYNC_LOW_RESERVED0_POS (17) 
#define LCD_DISP_RGB_SYNC_LOW_RESERVED0_MASK (0x7FFF) 

static inline uint32_t lcd_disp_ll_get_rgb_sync_low_value(void)
{
    return REG_READ(LCD_DISP_RGB_SYNC_LOW_ADDR);
}

static inline void lcd_disp_ll_set_rgb_sync_low_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_RGB_SYNC_LOW_ADDR,value);
}

/* REG_0x09:rgb_sync_low->hsync_back_low:0x9[  5:  0],hsync开始时为低的cycle数，默认为2。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_rgb_sync_low_hsync_back_low(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_SYNC_LOW_ADDR);
    reg_value = ((reg_value >> LCD_DISP_RGB_SYNC_LOW_HSYNC_BACK_LOW_POS) & LCD_DISP_RGB_SYNC_LOW_HSYNC_BACK_LOW_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_rgb_sync_low_hsync_back_low(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_SYNC_LOW_ADDR);
    reg_value &= ~(LCD_DISP_RGB_SYNC_LOW_HSYNC_BACK_LOW_MASK << LCD_DISP_RGB_SYNC_LOW_HSYNC_BACK_LOW_POS);
    reg_value |= ((value & LCD_DISP_RGB_SYNC_LOW_HSYNC_BACK_LOW_MASK) << LCD_DISP_RGB_SYNC_LOW_HSYNC_BACK_LOW_POS);
    REG_WRITE(LCD_DISP_RGB_SYNC_LOW_ADDR,reg_value);
}

/* REG_0x09:rgb_sync_low->vsync_back_low:0x9[ 13: 8],vsync开始时为低的cycle数，默认为2。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_rgb_sync_low_vsync_back_low(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_SYNC_LOW_ADDR);
    reg_value = ((reg_value >> LCD_DISP_RGB_SYNC_LOW_VSYNC_BACK_LOW_POS) & LCD_DISP_RGB_SYNC_LOW_VSYNC_BACK_LOW_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_rgb_sync_low_vsync_back_low(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_SYNC_LOW_ADDR);
    reg_value &= ~(LCD_DISP_RGB_SYNC_LOW_VSYNC_BACK_LOW_MASK << LCD_DISP_RGB_SYNC_LOW_VSYNC_BACK_LOW_POS);
    reg_value |= ((value & LCD_DISP_RGB_SYNC_LOW_VSYNC_BACK_LOW_MASK) << LCD_DISP_RGB_SYNC_LOW_VSYNC_BACK_LOW_POS);
    REG_WRITE(LCD_DISP_RGB_SYNC_LOW_ADDR,reg_value);
}

/* REG_0x09:rgb_sync_low->pfc_pixel_reve:0x9[     16],输入display模块的data 按2byte翻转顺序，默认为0不翻转。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_rgb_sync_low_pfc_pixel_reve(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_SYNC_LOW_ADDR);
    reg_value = ((reg_value >> LCD_DISP_RGB_SYNC_LOW_PFC_PIXEL_REVE_POS) & LCD_DISP_RGB_SYNC_LOW_PFC_PIXEL_REVE_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_rgb_sync_low_pfc_pixel_reve(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_SYNC_LOW_ADDR);
    reg_value &= ~(LCD_DISP_RGB_SYNC_LOW_PFC_PIXEL_REVE_MASK << LCD_DISP_RGB_SYNC_LOW_PFC_PIXEL_REVE_POS);
    reg_value |= ((value & LCD_DISP_RGB_SYNC_LOW_PFC_PIXEL_REVE_MASK) << LCD_DISP_RGB_SYNC_LOW_PFC_PIXEL_REVE_POS);
    REG_WRITE(LCD_DISP_RGB_SYNC_LOW_ADDR,reg_value);
}

/* REG_0x0A */
#define LCD_DISP_RGB_CLUM_OFFSET_ADDR  (LCD_DISP_LL_REG_BASE  + 0xA*4) //REG ADDR :0x48060028
#define LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_OFFSET_CLUM_L_POS (0) 
#define LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_OFFSET_CLUM_L_MASK (0x7FF) 

#define LCD_DISP_RGB_CLUM_OFFSET_RESERVED2_POS (11) 
#define LCD_DISP_RGB_CLUM_OFFSET_RESERVED2_MASK (0x1F) 

#define LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_OFFSET_CLUM_R_POS (16) 
#define LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_OFFSET_CLUM_R_MASK (0x7FF) 

#define LCD_DISP_RGB_CLUM_OFFSET_RESERVED1_POS (27) 
#define LCD_DISP_RGB_CLUM_OFFSET_RESERVED1_MASK (0x1) 

#define LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_AREA_ENA_POS (28) 
#define LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_AREA_ENA_MASK (0x1) 

#define LCD_DISP_RGB_CLUM_OFFSET_RESERVED0_POS (29) 
#define LCD_DISP_RGB_CLUM_OFFSET_RESERVED0_MASK (0x7) 

static inline uint32_t lcd_disp_ll_get_rgb_clum_offset_value(void)
{
    return REG_READ(LCD_DISP_RGB_CLUM_OFFSET_ADDR);
}

static inline void lcd_disp_ll_set_rgb_clum_offset_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_RGB_CLUM_OFFSET_ADDR,value);
}

/* REG_0x0a:rgb_clum_offset->partial_offset_clum_l:0xa[10:  0],局部显示行左侧偏移。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_rgb_clum_offset_partial_offset_clum_l(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_CLUM_OFFSET_ADDR);
    reg_value = ((reg_value >> LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_OFFSET_CLUM_L_POS) & LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_OFFSET_CLUM_L_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_rgb_clum_offset_partial_offset_clum_l(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_CLUM_OFFSET_ADDR);
    reg_value &= ~(LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_OFFSET_CLUM_L_MASK << LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_OFFSET_CLUM_L_POS);
    reg_value |= ((value & LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_OFFSET_CLUM_L_MASK) << LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_OFFSET_CLUM_L_POS);
    REG_WRITE(LCD_DISP_RGB_CLUM_OFFSET_ADDR,reg_value);
}

/* REG_0x0a:rgb_clum_offset->partial_offset_clum_r:0xa[26:16],局部显示行右侧偏移。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_rgb_clum_offset_partial_offset_clum_r(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_CLUM_OFFSET_ADDR);
    reg_value = ((reg_value >> LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_OFFSET_CLUM_R_POS) & LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_OFFSET_CLUM_R_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_rgb_clum_offset_partial_offset_clum_r(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_CLUM_OFFSET_ADDR);
    reg_value &= ~(LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_OFFSET_CLUM_R_MASK << LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_OFFSET_CLUM_R_POS);
    reg_value |= ((value & LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_OFFSET_CLUM_R_MASK) << LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_OFFSET_CLUM_R_POS);
    REG_WRITE(LCD_DISP_RGB_CLUM_OFFSET_ADDR,reg_value);
}

/* REG_0x0a:rgb_clum_offset->partial_area_ena:0xa[     28],局部显示使能位。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_rgb_clum_offset_partial_area_ena(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_CLUM_OFFSET_ADDR);
    reg_value = ((reg_value >> LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_AREA_ENA_POS) & LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_AREA_ENA_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_rgb_clum_offset_partial_area_ena(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_CLUM_OFFSET_ADDR);
    reg_value &= ~(LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_AREA_ENA_MASK << LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_AREA_ENA_POS);
    reg_value |= ((value & LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_AREA_ENA_MASK) << LCD_DISP_RGB_CLUM_OFFSET_PARTIAL_AREA_ENA_POS);
    REG_WRITE(LCD_DISP_RGB_CLUM_OFFSET_ADDR,reg_value);
}

/* REG_0x0B */
#define LCD_DISP_RGB_LINE_OFFSET_ADDR  (LCD_DISP_LL_REG_BASE  + 0xB*4) //REG ADDR :0x4806002c
#define LCD_DISP_RGB_LINE_OFFSET_PARTIAL_OFFSET_CLUM_L_POS (0) 
#define LCD_DISP_RGB_LINE_OFFSET_PARTIAL_OFFSET_CLUM_L_MASK (0x7FF) 

#define LCD_DISP_RGB_LINE_OFFSET_RESERVED1_POS (11) 
#define LCD_DISP_RGB_LINE_OFFSET_RESERVED1_MASK (0x1F) 

#define LCD_DISP_RGB_LINE_OFFSET_PARTIAL_OFFSET_CLUM_R_POS (16) 
#define LCD_DISP_RGB_LINE_OFFSET_PARTIAL_OFFSET_CLUM_R_MASK (0x7FF) 

#define LCD_DISP_RGB_LINE_OFFSET_RESERVED0_POS (27) 
#define LCD_DISP_RGB_LINE_OFFSET_RESERVED0_MASK (0x1F) 

static inline uint32_t lcd_disp_ll_get_rgb_line_offset_value(void)
{
    return REG_READ(LCD_DISP_RGB_LINE_OFFSET_ADDR);
}

static inline void lcd_disp_ll_set_rgb_line_offset_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_RGB_LINE_OFFSET_ADDR,value);
}

/* REG_0x0b:rgb_line_offset->partial_offset_clum_l:0xb[10:  0],局部显示列左侧偏移。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_rgb_line_offset_partial_offset_clum_l(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_LINE_OFFSET_ADDR);
    reg_value = ((reg_value >> LCD_DISP_RGB_LINE_OFFSET_PARTIAL_OFFSET_CLUM_L_POS) & LCD_DISP_RGB_LINE_OFFSET_PARTIAL_OFFSET_CLUM_L_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_rgb_line_offset_partial_offset_clum_l(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_LINE_OFFSET_ADDR);
    reg_value &= ~(LCD_DISP_RGB_LINE_OFFSET_PARTIAL_OFFSET_CLUM_L_MASK << LCD_DISP_RGB_LINE_OFFSET_PARTIAL_OFFSET_CLUM_L_POS);
    reg_value |= ((value & LCD_DISP_RGB_LINE_OFFSET_PARTIAL_OFFSET_CLUM_L_MASK) << LCD_DISP_RGB_LINE_OFFSET_PARTIAL_OFFSET_CLUM_L_POS);
    REG_WRITE(LCD_DISP_RGB_LINE_OFFSET_ADDR,reg_value);
}

/* REG_0x0b:rgb_line_offset->partial_offset_clum_r:0xb[26:16],局部显示列右侧偏移。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_rgb_line_offset_partial_offset_clum_r(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_LINE_OFFSET_ADDR);
    reg_value = ((reg_value >> LCD_DISP_RGB_LINE_OFFSET_PARTIAL_OFFSET_CLUM_R_POS) & LCD_DISP_RGB_LINE_OFFSET_PARTIAL_OFFSET_CLUM_R_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_rgb_line_offset_partial_offset_clum_r(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_LINE_OFFSET_ADDR);
    reg_value &= ~(LCD_DISP_RGB_LINE_OFFSET_PARTIAL_OFFSET_CLUM_R_MASK << LCD_DISP_RGB_LINE_OFFSET_PARTIAL_OFFSET_CLUM_R_POS);
    reg_value |= ((value & LCD_DISP_RGB_LINE_OFFSET_PARTIAL_OFFSET_CLUM_R_MASK) << LCD_DISP_RGB_LINE_OFFSET_PARTIAL_OFFSET_CLUM_R_POS);
    REG_WRITE(LCD_DISP_RGB_LINE_OFFSET_ADDR,reg_value);
}

/* REG_0x0C */
#define LCD_DISP_DAT_FIFO_THRD_ADDR  (LCD_DISP_LL_REG_BASE  + 0xC*4) //REG ADDR :0x48060030
#define LCD_DISP_DAT_FIFO_THRD_I8080_CMD_PARA_COUNT_POS (0) 
#define LCD_DISP_DAT_FIFO_THRD_I8080_CMD_PARA_COUNT_MASK (0x1F) 

#define LCD_DISP_DAT_FIFO_THRD_RESERVED2_POS (5) 
#define LCD_DISP_DAT_FIFO_THRD_RESERVED2_MASK (0x7) 

#define LCD_DISP_DAT_FIFO_THRD_DAT_WR_THRD_POS (8) 
#define LCD_DISP_DAT_FIFO_THRD_DAT_WR_THRD_MASK (0x1FF) 

#define LCD_DISP_DAT_FIFO_THRD_RESERVED1_POS (17) 
#define LCD_DISP_DAT_FIFO_THRD_RESERVED1_MASK (0x7) 

#define LCD_DISP_DAT_FIFO_THRD_DAT_RD_THRD_POS (20) 
#define LCD_DISP_DAT_FIFO_THRD_DAT_RD_THRD_MASK (0x1FF) 

#define LCD_DISP_DAT_FIFO_THRD_RESERVED0_POS (29) 
#define LCD_DISP_DAT_FIFO_THRD_RESERVED0_MASK (0x7) 

static inline uint32_t lcd_disp_ll_get_dat_fifo_thrd_value(void)
{
    return REG_READ(LCD_DISP_DAT_FIFO_THRD_ADDR);
}

static inline void lcd_disp_ll_set_dat_fifo_thrd_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_DAT_FIFO_THRD_ADDR,value);
}

/* REG_0x0c:dat_fifo_thrd->i8080_cmd_para_count:0xc[ 4:0],i8080模块初始化期间，cmd指令后param的个数。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_dat_fifo_thrd_i8080_cmd_para_count(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DAT_FIFO_THRD_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DAT_FIFO_THRD_I8080_CMD_PARA_COUNT_POS) & LCD_DISP_DAT_FIFO_THRD_I8080_CMD_PARA_COUNT_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_dat_fifo_thrd_i8080_cmd_para_count(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DAT_FIFO_THRD_ADDR);
    reg_value &= ~(LCD_DISP_DAT_FIFO_THRD_I8080_CMD_PARA_COUNT_MASK << LCD_DISP_DAT_FIFO_THRD_I8080_CMD_PARA_COUNT_POS);
    reg_value |= ((value & LCD_DISP_DAT_FIFO_THRD_I8080_CMD_PARA_COUNT_MASK) << LCD_DISP_DAT_FIFO_THRD_I8080_CMD_PARA_COUNT_POS);
    REG_WRITE(LCD_DISP_DAT_FIFO_THRD_ADDR,reg_value);
}

/* REG_0x0c:dat_fifo_thrd->dat_wr_thrd:0xc[16:  8],display fifo 写门限。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_dat_fifo_thrd_dat_wr_thrd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DAT_FIFO_THRD_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DAT_FIFO_THRD_DAT_WR_THRD_POS) & LCD_DISP_DAT_FIFO_THRD_DAT_WR_THRD_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_dat_fifo_thrd_dat_wr_thrd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DAT_FIFO_THRD_ADDR);
    reg_value &= ~(LCD_DISP_DAT_FIFO_THRD_DAT_WR_THRD_MASK << LCD_DISP_DAT_FIFO_THRD_DAT_WR_THRD_POS);
    reg_value |= ((value & LCD_DISP_DAT_FIFO_THRD_DAT_WR_THRD_MASK) << LCD_DISP_DAT_FIFO_THRD_DAT_WR_THRD_POS);
    REG_WRITE(LCD_DISP_DAT_FIFO_THRD_ADDR,reg_value);
}

/* REG_0x0c:dat_fifo_thrd->dat_rd_thrd:0xc[28:20],display fifo读门限。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_dat_fifo_thrd_dat_rd_thrd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DAT_FIFO_THRD_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DAT_FIFO_THRD_DAT_RD_THRD_POS) & LCD_DISP_DAT_FIFO_THRD_DAT_RD_THRD_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_dat_fifo_thrd_dat_rd_thrd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DAT_FIFO_THRD_ADDR);
    reg_value &= ~(LCD_DISP_DAT_FIFO_THRD_DAT_RD_THRD_MASK << LCD_DISP_DAT_FIFO_THRD_DAT_RD_THRD_POS);
    reg_value |= ((value & LCD_DISP_DAT_FIFO_THRD_DAT_RD_THRD_MASK) << LCD_DISP_DAT_FIFO_THRD_DAT_RD_THRD_POS);
    REG_WRITE(LCD_DISP_DAT_FIFO_THRD_ADDR,reg_value);
}

/* REG_0x0D */
#define LCD_DISP_MATER_RD_BASE_ADDR_ADDR  (LCD_DISP_LL_REG_BASE  + 0xD*4) //REG ADDR :0x48060034
#define LCD_DISP_MATER_RD_BASE_ADDR_MASTER_RD_BASE_ADR_POS (0) 
#define LCD_DISP_MATER_RD_BASE_ADDR_MASTER_RD_BASE_ADR_MASK (0xFFFFFFFF) 


/* REG_0x0d:mater_rd_base_addr->master_rd_base_adr:0xd[31:  0],display master读数据的base address，图像数据的首地址。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_mater_rd_base_addr(void)
{
    return REG_READ(LCD_DISP_MATER_RD_BASE_ADDR_ADDR);
}

static inline void lcd_disp_ll_set_mater_rd_base_addr(uint32_t value)
{
    REG_WRITE(LCD_DISP_MATER_RD_BASE_ADDR_ADDR,value);
}
#ifdef __cplusplus 
}                  
#endif             
