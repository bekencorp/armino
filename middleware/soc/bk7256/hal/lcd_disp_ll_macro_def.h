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
* CHIP ID:BK7256,GENARATE TIME:2022-01-26 14:14:13                                                 
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

#define LCD_DISP_DISPLAY_INT_WR_THRD_POS (8) 
#define LCD_DISP_DISPLAY_INT_WR_THRD_MASK (0x3FF) 

#define LCD_DISP_DISPLAY_INT_RD_THRD_POS (16) 
#define LCD_DISP_DISPLAY_INT_RD_THRD_MASK (0xFFF) 

#define LCD_DISP_DISPLAY_INT_DISCONTI_MODE_POS (28) 
#define LCD_DISP_DISPLAY_INT_DISCONTI_MODE_MASK (0x1) 

#define LCD_DISP_DISPLAY_INT_RESERVED_POS (29) 
#define LCD_DISP_DISPLAY_INT_RESERVED_MASK (0x7) 

static inline uint32_t lcd_disp_ll_get_display_int_value(void)
{
    return REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
}

static inline void lcd_disp_ll_set_display_int_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_DISPLAY_INT_ADDR,value);
}

/* REG_0x00:display_int->rgb_int_en:0x0[  1:0],rgb display interrupt enable,0x0,r/w*/
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

/* REG_0x00:display_int->i8080_int_en:0x0[  3:2],i8080 display interrupt enable,0x0,r/w*/
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

/* REG_0x00:display_int->rgb_sof:0x0[    4],rgb display output start of frame,0x0,r/w*/
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

/* REG_0x00:display_int->rgb_eof:0x0[    5],rgb display output end of frame,0x0,r/w*/
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

/* REG_0x00:display_int->i8080_sof:0x0[    6],8080 display output start of frame,0x0,r/w*/
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

/* REG_0x00:display_int->i8080_eof:0x0[    7],8080 display output end of frame,0x0,r/w*/
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

/* REG_0x00:display_int->wr_thrd:0x0[17: 8],rgb fifo wr thrd,None,r/w*/
static inline uint32_t lcd_disp_ll_get_display_int_wr_thrd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISPLAY_INT_WR_THRD_POS) & LCD_DISP_DISPLAY_INT_WR_THRD_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_display_int_wr_thrd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value &= ~(LCD_DISP_DISPLAY_INT_WR_THRD_MASK << LCD_DISP_DISPLAY_INT_WR_THRD_POS);
    reg_value |= ((value & LCD_DISP_DISPLAY_INT_WR_THRD_MASK) << LCD_DISP_DISPLAY_INT_WR_THRD_POS);
    REG_WRITE(LCD_DISP_DISPLAY_INT_ADDR,reg_value);
}

/* REG_0x00:display_int->rd_thrd:0x0[27:16],rgb fifo rd thrd,None,r/w*/
static inline uint32_t lcd_disp_ll_get_display_int_rd_thrd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISPLAY_INT_RD_THRD_POS) & LCD_DISP_DISPLAY_INT_RD_THRD_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_display_int_rd_thrd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value &= ~(LCD_DISP_DISPLAY_INT_RD_THRD_MASK << LCD_DISP_DISPLAY_INT_RD_THRD_POS);
    reg_value |= ((value & LCD_DISP_DISPLAY_INT_RD_THRD_MASK) << LCD_DISP_DISPLAY_INT_RD_THRD_POS);
    REG_WRITE(LCD_DISP_DISPLAY_INT_ADDR,reg_value);
}

/* REG_0x00:display_int->disconti_mode:0x0[28],0：close，1：open,0x0,r/w*/
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
#define LCD_DISP_STATUS_ADDR  (LCD_DISP_LL_REG_BASE  + 0x1*4) //REG ADDR :0x48060004
#define LCD_DISP_STATUS_X_PIXEL_POS (0) 
#define LCD_DISP_STATUS_X_PIXEL_MASK (0x7FF) 

#define LCD_DISP_STATUS_DCLK_REV_POS (11) 
#define LCD_DISP_STATUS_DCLK_REV_MASK (0x1) 

#define LCD_DISP_STATUS_Y_PIXEL_POS (12) 
#define LCD_DISP_STATUS_Y_PIXEL_MASK (0x7FF) 

#define LCD_DISP_STATUS_STR_FIFO_CLR_POS (23) 
#define LCD_DISP_STATUS_STR_FIFO_CLR_MASK (0x1) 

#define LCD_DISP_STATUS_RGB_DISP_ON_POS (24) 
#define LCD_DISP_STATUS_RGB_DISP_ON_MASK (0x1) 

#define LCD_DISP_STATUS_RGB_ON_POS (25) 
#define LCD_DISP_STATUS_RGB_ON_MASK (0x1) 

#define LCD_DISP_STATUS_LCD_DISPLAY_ON_POS (26) 
#define LCD_DISP_STATUS_LCD_DISPLAY_ON_MASK (0x1) 

#define LCD_DISP_STATUS_RGB_CLK_DIV_POS (27) 
#define LCD_DISP_STATUS_RGB_CLK_DIV_MASK (0x1f) 

static inline uint32_t lcd_disp_ll_get_status_value(void)
{
    return REG_READ(LCD_DISP_STATUS_ADDR);
}

static inline void lcd_disp_ll_set_status_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_STATUS_ADDR,value);
}

/* REG_0x01:status->x_pixel:0x1[10: 0], ,0x1E0,r/w*/
static inline uint32_t lcd_disp_ll_get_status_x_pixel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_STATUS_ADDR);
    reg_value = ((reg_value >> LCD_DISP_STATUS_X_PIXEL_POS) & LCD_DISP_STATUS_X_PIXEL_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_status_x_pixel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_STATUS_ADDR);
    reg_value &= ~(LCD_DISP_STATUS_X_PIXEL_MASK << LCD_DISP_STATUS_X_PIXEL_POS);
    reg_value |= ((value & LCD_DISP_STATUS_X_PIXEL_MASK) << LCD_DISP_STATUS_X_PIXEL_POS);
    REG_WRITE(LCD_DISP_STATUS_ADDR,reg_value);
}

/* REG_0x01:status->dclk_rev:0x1[   11],rgb output edge sel：posedge；1：negedge。,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_status_dclk_rev(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_STATUS_ADDR);
    reg_value = ((reg_value >> LCD_DISP_STATUS_DCLK_REV_POS) & LCD_DISP_STATUS_DCLK_REV_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_status_dclk_rev(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_STATUS_ADDR);
    reg_value &= ~(LCD_DISP_STATUS_DCLK_REV_MASK << LCD_DISP_STATUS_DCLK_REV_POS);
    reg_value |= ((value & LCD_DISP_STATUS_DCLK_REV_MASK) << LCD_DISP_STATUS_DCLK_REV_POS);
    REG_WRITE(LCD_DISP_STATUS_ADDR,reg_value);
}

/* REG_0x01:status->y_pixel:0x1[22:12], ,0x110,r/w*/
static inline uint32_t lcd_disp_ll_get_status_y_pixel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_STATUS_ADDR);
    reg_value = ((reg_value >> LCD_DISP_STATUS_Y_PIXEL_POS) & LCD_DISP_STATUS_Y_PIXEL_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_status_y_pixel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_STATUS_ADDR);
    reg_value &= ~(LCD_DISP_STATUS_Y_PIXEL_MASK << LCD_DISP_STATUS_Y_PIXEL_POS);
    reg_value |= ((value & LCD_DISP_STATUS_Y_PIXEL_MASK) << LCD_DISP_STATUS_Y_PIXEL_POS);
    REG_WRITE(LCD_DISP_STATUS_ADDR,reg_value);
}

/* REG_0x01:status->str_fifo_clr:0x1[   23],rgb stream_fifo rst,not used,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_status_str_fifo_clr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_STATUS_ADDR);
    reg_value = ((reg_value >> LCD_DISP_STATUS_STR_FIFO_CLR_POS) & LCD_DISP_STATUS_STR_FIFO_CLR_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_status_str_fifo_clr(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_STATUS_ADDR);
    reg_value &= ~(LCD_DISP_STATUS_STR_FIFO_CLR_MASK << LCD_DISP_STATUS_STR_FIFO_CLR_POS);
    reg_value |= ((value & LCD_DISP_STATUS_STR_FIFO_CLR_MASK) << LCD_DISP_STATUS_STR_FIFO_CLR_POS);
    REG_WRITE(LCD_DISP_STATUS_ADDR,reg_value);
}

/* REG_0x01:status->rgb_disp_on:0x1[   24],rgb display enable,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_status_rgb_disp_on(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_STATUS_ADDR);
    reg_value = ((reg_value >> LCD_DISP_STATUS_RGB_DISP_ON_POS) & LCD_DISP_STATUS_RGB_DISP_ON_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_status_rgb_disp_on(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_STATUS_ADDR);
    reg_value &= ~(LCD_DISP_STATUS_RGB_DISP_ON_MASK << LCD_DISP_STATUS_RGB_DISP_ON_POS);
    reg_value |= ((value & LCD_DISP_STATUS_RGB_DISP_ON_MASK) << LCD_DISP_STATUS_RGB_DISP_ON_POS);
    REG_WRITE(LCD_DISP_STATUS_ADDR,reg_value);
}

/* REG_0x01:status->rgb_on:0x1[   25],Rgb module IO 1：rgb output。0：not rgb output,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_status_rgb_on(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_STATUS_ADDR);
    reg_value = ((reg_value >> LCD_DISP_STATUS_RGB_ON_POS) & LCD_DISP_STATUS_RGB_ON_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_status_rgb_on(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_STATUS_ADDR);
    reg_value &= ~(LCD_DISP_STATUS_RGB_ON_MASK << LCD_DISP_STATUS_RGB_ON_POS);
    reg_value |= ((value & LCD_DISP_STATUS_RGB_ON_MASK) << LCD_DISP_STATUS_RGB_ON_POS);
    REG_WRITE(LCD_DISP_STATUS_ADDR,reg_value);
}

/* REG_0x01:status->lcd_display_on:0x1[   26],lcd,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_status_lcd_display_on(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_STATUS_ADDR);
    reg_value = ((reg_value >> LCD_DISP_STATUS_LCD_DISPLAY_ON_POS) & LCD_DISP_STATUS_LCD_DISPLAY_ON_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_status_lcd_display_on(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_STATUS_ADDR);
    reg_value &= ~(LCD_DISP_STATUS_LCD_DISPLAY_ON_MASK << LCD_DISP_STATUS_LCD_DISPLAY_ON_POS);
    reg_value |= ((value & LCD_DISP_STATUS_LCD_DISPLAY_ON_MASK) << LCD_DISP_STATUS_LCD_DISPLAY_ON_POS);
    REG_WRITE(LCD_DISP_STATUS_ADDR,reg_value);
}

/* REG_0x01:status->rgb_clk_div:0x1[29:27],00：9mhz， 01：5mhz，10：12mhz。11：nc,0x0,r/w */
static inline uint32_t lcd_disp_ll_get_status_rgb_clk_div(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_STATUS_ADDR);
    reg_value = ((reg_value >> LCD_DISP_STATUS_RGB_CLK_DIV_POS) & LCD_DISP_STATUS_RGB_CLK_DIV_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_status_rgb_clk_div(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_STATUS_ADDR);
    reg_value &= ~(LCD_DISP_STATUS_RGB_CLK_DIV_MASK << LCD_DISP_STATUS_RGB_CLK_DIV_POS);
    reg_value |= ((value & LCD_DISP_STATUS_RGB_CLK_DIV_MASK) << LCD_DISP_STATUS_RGB_CLK_DIV_POS);
    REG_WRITE(LCD_DISP_STATUS_ADDR,reg_value);
}


/* REG_0x02 */
#define LCD_DISP_RGB_FIFO_ADDR  (LCD_DISP_LL_REG_BASE  + 0x2*4) //REG ADDR :0x48060008
#define LCD_DISP_RGB_FIFO_RGB_DAT_POS (0) 
#define LCD_DISP_RGB_FIFO_RGB_DAT_MASK (0xFFFF) 

#define LCD_DISP_RGB_FIFO_RESERVED_POS (16) 
#define LCD_DISP_RGB_FIFO_RESERVED_MASK (0xFFFF) 

static inline void lcd_disp_ll_set_rgb_fifo_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_RGB_FIFO_ADDR,value);
}

/* REG_0x02:rgb_fifo->rgb_dat:0x2[15: 0], rgb data move to this reg by dma,0x0,w*/
static inline void lcd_disp_ll_set_rgb_fifo_rgb_dat(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_RGB_FIFO_ADDR);
    reg_value &= ~(LCD_DISP_RGB_FIFO_RGB_DAT_MASK << LCD_DISP_RGB_FIFO_RGB_DAT_POS);
    reg_value |= ((value & LCD_DISP_RGB_FIFO_RGB_DAT_MASK) << LCD_DISP_RGB_FIFO_RGB_DAT_POS);
    REG_WRITE(LCD_DISP_RGB_FIFO_ADDR,reg_value);
}

/* REG_0x03 */
#define LCD_DISP_HSYNC_VSYNC_CFG_ADDR  (LCD_DISP_LL_REG_BASE  + 0x3*4) //REG ADDR :0x4806000c
#define LCD_DISP_HSYNC_VSYNC_CFG_HSYNC_BACK_PORCH_POS (0) 
#define LCD_DISP_HSYNC_VSYNC_CFG_HSYNC_BACK_PORCH_MASK (0xFF) 

#define LCD_DISP_HSYNC_VSYNC_CFG_HSYNC_FRONT_PORCH_POS (8) 
#define LCD_DISP_HSYNC_VSYNC_CFG_HSYNC_FRONT_PORCH_MASK (0x7F) 

#define LCD_DISP_HSYNC_VSYNC_CFG_VSYNC_BACK_PORCH_POS (15) 
#define LCD_DISP_HSYNC_VSYNC_CFG_VSYNC_BACK_PORCH_MASK (0x1F) 

#define LCD_DISP_HSYNC_VSYNC_CFG_VSYNC_FRONT_PORCH_POS (20) 
#define LCD_DISP_HSYNC_VSYNC_CFG_VSYNC_FRONT_PORCH_MASK (0x7F) 

#define LCD_DISP_HSYNC_VSYNC_CFG_YUV_SEL_POS (28) 
#define LCD_DISP_HSYNC_VSYNC_CFG_YUV_SEL_MASK (0x7) 

#define LCD_DISP_HSYNC_VSYNC_CFG_RESERVED_POS (31) 
#define LCD_DISP_HSYNC_VSYNC_CFG_RESERVED_MASK (0x1) 

static inline uint32_t lcd_disp_ll_get_hsync_vsync_cfg_value(void)
{
    return REG_READ(LCD_DISP_HSYNC_VSYNC_CFG_ADDR);
}

static inline void lcd_disp_ll_set_hsync_vsync_cfg_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_HSYNC_VSYNC_CFG_ADDR,value);
}

/* REG_0x03:hsync_vsync_cfg->hsync_back_porch:0x3[ 7: 0], ,40,r/w*/
static inline uint32_t lcd_disp_ll_get_hsync_vsync_cfg_hsync_back_porch(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_HSYNC_VSYNC_CFG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_HSYNC_VSYNC_CFG_HSYNC_BACK_PORCH_POS) & LCD_DISP_HSYNC_VSYNC_CFG_HSYNC_BACK_PORCH_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_hsync_vsync_cfg_hsync_back_porch(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_HSYNC_VSYNC_CFG_ADDR);
    reg_value &= ~(LCD_DISP_HSYNC_VSYNC_CFG_HSYNC_BACK_PORCH_MASK << LCD_DISP_HSYNC_VSYNC_CFG_HSYNC_BACK_PORCH_POS);
    reg_value |= ((value & LCD_DISP_HSYNC_VSYNC_CFG_HSYNC_BACK_PORCH_MASK) << LCD_DISP_HSYNC_VSYNC_CFG_HSYNC_BACK_PORCH_POS);
    REG_WRITE(LCD_DISP_HSYNC_VSYNC_CFG_ADDR,reg_value);
}

/* REG_0x03:hsync_vsync_cfg->hsync_front_porch:0x3[14: 8], ,5,r/w*/
static inline uint32_t lcd_disp_ll_get_hsync_vsync_cfg_hsync_front_porch(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_HSYNC_VSYNC_CFG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_HSYNC_VSYNC_CFG_HSYNC_FRONT_PORCH_POS) & LCD_DISP_HSYNC_VSYNC_CFG_HSYNC_FRONT_PORCH_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_hsync_vsync_cfg_hsync_front_porch(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_HSYNC_VSYNC_CFG_ADDR);
    reg_value &= ~(LCD_DISP_HSYNC_VSYNC_CFG_HSYNC_FRONT_PORCH_MASK << LCD_DISP_HSYNC_VSYNC_CFG_HSYNC_FRONT_PORCH_POS);
    reg_value |= ((value & LCD_DISP_HSYNC_VSYNC_CFG_HSYNC_FRONT_PORCH_MASK) << LCD_DISP_HSYNC_VSYNC_CFG_HSYNC_FRONT_PORCH_POS);
    REG_WRITE(LCD_DISP_HSYNC_VSYNC_CFG_ADDR,reg_value);
}

/* REG_0x03:hsync_vsync_cfg->vsync_back_porch:0x3[19:15], ,8,r/w*/
static inline uint32_t lcd_disp_ll_get_hsync_vsync_cfg_vsync_back_porch(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_HSYNC_VSYNC_CFG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_HSYNC_VSYNC_CFG_VSYNC_BACK_PORCH_POS) & LCD_DISP_HSYNC_VSYNC_CFG_VSYNC_BACK_PORCH_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_hsync_vsync_cfg_vsync_back_porch(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_HSYNC_VSYNC_CFG_ADDR);
    reg_value &= ~(LCD_DISP_HSYNC_VSYNC_CFG_VSYNC_BACK_PORCH_MASK << LCD_DISP_HSYNC_VSYNC_CFG_VSYNC_BACK_PORCH_POS);
    reg_value |= ((value & LCD_DISP_HSYNC_VSYNC_CFG_VSYNC_BACK_PORCH_MASK) << LCD_DISP_HSYNC_VSYNC_CFG_VSYNC_BACK_PORCH_POS);
    REG_WRITE(LCD_DISP_HSYNC_VSYNC_CFG_ADDR,reg_value);
}

/* REG_0x03:hsync_vsync_cfg->vsync_front_porch:0x3[26:20], ,8,r/w*/
static inline uint32_t lcd_disp_ll_get_hsync_vsync_cfg_vsync_front_porch(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_HSYNC_VSYNC_CFG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_HSYNC_VSYNC_CFG_VSYNC_FRONT_PORCH_POS) & LCD_DISP_HSYNC_VSYNC_CFG_VSYNC_FRONT_PORCH_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_hsync_vsync_cfg_vsync_front_porch(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_HSYNC_VSYNC_CFG_ADDR);
    reg_value &= ~(LCD_DISP_HSYNC_VSYNC_CFG_VSYNC_FRONT_PORCH_MASK << LCD_DISP_HSYNC_VSYNC_CFG_VSYNC_FRONT_PORCH_POS);
    reg_value |= ((value & LCD_DISP_HSYNC_VSYNC_CFG_VSYNC_FRONT_PORCH_MASK) << LCD_DISP_HSYNC_VSYNC_CFG_VSYNC_FRONT_PORCH_POS);
    REG_WRITE(LCD_DISP_HSYNC_VSYNC_CFG_ADDR,reg_value);
}

/* REG_0x03:hsync_vsync_cfg->yuv_sel:0x3[30:28], ,0,r/w*/
static inline uint32_t lcd_disp_ll_get_hsync_vsync_cfg_yuv_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_HSYNC_VSYNC_CFG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_HSYNC_VSYNC_CFG_YUV_SEL_POS) & LCD_DISP_HSYNC_VSYNC_CFG_YUV_SEL_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_hsync_vsync_cfg_yuv_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_HSYNC_VSYNC_CFG_ADDR);
    reg_value &= ~(LCD_DISP_HSYNC_VSYNC_CFG_YUV_SEL_MASK << LCD_DISP_HSYNC_VSYNC_CFG_YUV_SEL_POS);
    reg_value |= ((value & LCD_DISP_HSYNC_VSYNC_CFG_YUV_SEL_MASK) << LCD_DISP_HSYNC_VSYNC_CFG_YUV_SEL_POS);
    REG_WRITE(LCD_DISP_HSYNC_VSYNC_CFG_ADDR,reg_value);
}

/* REG_0x04 */
#define LCD_DISP_I8080_CONFIG_ADDR  (LCD_DISP_LL_REG_BASE  + 0x4*4) //REG ADDR :0x48060010
#define LCD_DISP_I8080_CONFIG_I8080_DISP_EN_POS (0) 
#define LCD_DISP_I8080_CONFIG_I8080_DISP_EN_MASK (0x1) 

#define LCD_DISP_I8080_CONFIG_I8080_DAT_ON_POS (1) 
#define LCD_DISP_I8080_CONFIG_I8080_DAT_ON_MASK (0x1) 

#define LCD_DISP_I8080_CONFIG_I8080_FIFO_MODE_POS (2) 
#define LCD_DISP_I8080_CONFIG_I8080_FIFO_MODE_MASK (0x1) 

#define LCD_DISP_I8080_CONFIG_I8080_FIFO_CLR_POS (3) 
#define LCD_DISP_I8080_CONFIG_I8080_FIFO_CLR_MASK (0x1) 

#define LCD_DISP_I8080_CONFIG_I8080_CMDFIFO_CLR_POS (4) 
#define LCD_DISP_I8080_CONFIG_I8080_CMDFIFO_CLR_MASK (0x1) 

#define LCD_DISP_I8080_CONFIG_RESET_SLEEP_IN_POS (5) 
#define LCD_DISP_I8080_CONFIG_RESET_SLEEP_IN_MASK (0x1) 

#define LCD_DISP_I8080_CONFIG_RESERVED1_POS (6) 
#define LCD_DISP_I8080_CONFIG_RESERVED1_MASK (0x3) 

#define LCD_DISP_I8080_CONFIG_TIK_CNT_POS (8) 
#define LCD_DISP_I8080_CONFIG_TIK_CNT_MASK (0x3) 

#define LCD_DISP_I8080_CONFIG_RESERVED2_POS (10) 
#define LCD_DISP_I8080_CONFIG_RESERVED2_MASK (0x3) 

#define LCD_DISP_I8080_CONFIG_I8080_1MS_COUNT_POS (12) 
#define LCD_DISP_I8080_CONFIG_I8080_1MS_COUNT_MASK (0x1FF) 

#define LCD_DISP_I8080_CONFIG_RESERVED3_POS (21) 
#define LCD_DISP_I8080_CONFIG_RESERVED3_MASK (0x7FF) 

static inline uint32_t lcd_disp_ll_get_i8080_config_value(void)
{
    return REG_READ(LCD_DISP_I8080_CONFIG_ADDR);
}

static inline void lcd_disp_ll_set_i8080_config_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_I8080_CONFIG_ADDR,value);
}

/* REG_0x04:i8080_config->i8080_disp_en:0x4[    0],i8080 function enable,0x0,r/w*/
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

/* REG_0x04:i8080_config->i8080_dat_on:0x4[    1],i8080 data start transfer,0x0,r/w*/
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

/* REG_0x04:i8080_config->i8080_fifo_mode:0x4[    2], ,0x0,r/w*/
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

/* REG_0x04:i8080_config->i8080_fifo_clr:0x4[    3],i8080 data_fifo rst,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_i8080_config_i8080_fifo_clr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CONFIG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_I8080_CONFIG_I8080_FIFO_CLR_POS) & LCD_DISP_I8080_CONFIG_I8080_FIFO_CLR_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_i8080_config_i8080_fifo_clr(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CONFIG_ADDR);
    reg_value &= ~(LCD_DISP_I8080_CONFIG_I8080_FIFO_CLR_MASK << LCD_DISP_I8080_CONFIG_I8080_FIFO_CLR_POS);
    reg_value |= ((value & LCD_DISP_I8080_CONFIG_I8080_FIFO_CLR_MASK) << LCD_DISP_I8080_CONFIG_I8080_FIFO_CLR_POS);
    REG_WRITE(LCD_DISP_I8080_CONFIG_ADDR,reg_value);
}

/* REG_0x04:i8080_config->i8080_cmdfifo_clr:0x4[    4],i8080 cmd_fifo rst,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_i8080_config_i8080_cmdfifo_clr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CONFIG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_I8080_CONFIG_I8080_CMDFIFO_CLR_POS) & LCD_DISP_I8080_CONFIG_I8080_CMDFIFO_CLR_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_i8080_config_i8080_cmdfifo_clr(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CONFIG_ADDR);
    reg_value &= ~(LCD_DISP_I8080_CONFIG_I8080_CMDFIFO_CLR_MASK << LCD_DISP_I8080_CONFIG_I8080_CMDFIFO_CLR_POS);
    reg_value |= ((value & LCD_DISP_I8080_CONFIG_I8080_CMDFIFO_CLR_MASK) << LCD_DISP_I8080_CONFIG_I8080_CMDFIFO_CLR_POS);
    REG_WRITE(LCD_DISP_I8080_CONFIG_ADDR,reg_value);
}

/* REG_0x04:i8080_config->reset_sleep_in:0x4[    5], ,0x0,r/w*/
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

/* REG_0x04:i8080_config->tik_cnt:0x4[    9:8],。0：8clk；1：6clk；2：4clk；3：2clk。,0x0,r/w*/
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

/* REG_0x04:i8080_config->i8080_1ms_count:0x4[    20:12], ,0x0,r/w*/
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

/* REG_0x05:i8080_cmd_fifo->i8080_cmd_fifo:0x5[15: 0],i8080 command fifo,0x0,w*/
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
#define LCD_DISP_I8080_DAT_FIFO_I8080_DAT_FIFO_MASK (0xFFFF) 

#define LCD_DISP_I8080_DAT_FIFO_RESERVED_POS (16) 
#define LCD_DISP_I8080_DAT_FIFO_RESERVED_MASK (0xFFFF) 

static inline void lcd_disp_ll_set_i8080_dat_fifo_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_I8080_DAT_FIFO_ADDR,value);
}

/* REG_0x06:i8080_dat_fifo->i8080_dat_fifo:0x6[15 :0],i8080 data fifo,0x0,w*/
static inline void lcd_disp_ll_set_i8080_dat_fifo_i8080_dat_fifo(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_DAT_FIFO_ADDR);
    reg_value &= ~(LCD_DISP_I8080_DAT_FIFO_I8080_DAT_FIFO_MASK << LCD_DISP_I8080_DAT_FIFO_I8080_DAT_FIFO_POS);
    reg_value |= ((value & LCD_DISP_I8080_DAT_FIFO_I8080_DAT_FIFO_MASK) << LCD_DISP_I8080_DAT_FIFO_I8080_DAT_FIFO_POS);
    REG_WRITE(LCD_DISP_I8080_DAT_FIFO_ADDR,reg_value);
}

/* REG_0x07 */
#define LCD_DISP_I8080THRD_ADDR  (LCD_DISP_LL_REG_BASE  + 0x7*4) //REG ADDR :0x4806001c
#define LCD_DISP_I8080THRD_DAT_WR_THRD_POS (0) 
#define LCD_DISP_I8080THRD_DAT_WR_THRD_MASK (0xFF) 

#define LCD_DISP_I8080THRD_CMD_WR_THRD_POS (8) 
#define LCD_DISP_I8080THRD_CMD_WR_THRD_MASK (0xFF) 

#define LCD_DISP_I8080THRD_DAT_RD_THRD_POS (16) 
#define LCD_DISP_I8080THRD_DAT_RD_THRD_MASK (0xFF) 

#define LCD_DISP_I8080THRD_CMD_RD_THRD_POS (24) 
#define LCD_DISP_I8080THRD_CMD_RD_THRD_MASK (0xFF) 

static inline uint32_t lcd_disp_ll_get_i8080thrd_value(void)
{
    return REG_READ(LCD_DISP_I8080THRD_ADDR);
}

static inline void lcd_disp_ll_set_i8080thrd_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_I8080THRD_ADDR,value);
}

/* REG_0x07:i8080thrd->dat_wr_thrd:0x7[ 7 :0],i8080 rgb fifo wr thrd,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_i8080thrd_dat_wr_thrd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080THRD_ADDR);
    reg_value = ((reg_value >> LCD_DISP_I8080THRD_DAT_WR_THRD_POS) & LCD_DISP_I8080THRD_DAT_WR_THRD_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_i8080thrd_dat_wr_thrd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080THRD_ADDR);
    reg_value &= ~(LCD_DISP_I8080THRD_DAT_WR_THRD_MASK << LCD_DISP_I8080THRD_DAT_WR_THRD_POS);
    reg_value |= ((value & LCD_DISP_I8080THRD_DAT_WR_THRD_MASK) << LCD_DISP_I8080THRD_DAT_WR_THRD_POS);
    REG_WRITE(LCD_DISP_I8080THRD_ADDR,reg_value);
}

/* REG_0x07:i8080thrd->cmd_wr_thrd:0x7[15: 8],i8080 cmd fifo wr thrd,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_i8080thrd_cmd_wr_thrd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080THRD_ADDR);
    reg_value = ((reg_value >> LCD_DISP_I8080THRD_CMD_WR_THRD_POS) & LCD_DISP_I8080THRD_CMD_WR_THRD_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_i8080thrd_cmd_wr_thrd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080THRD_ADDR);
    reg_value &= ~(LCD_DISP_I8080THRD_CMD_WR_THRD_MASK << LCD_DISP_I8080THRD_CMD_WR_THRD_POS);
    reg_value |= ((value & LCD_DISP_I8080THRD_CMD_WR_THRD_MASK) << LCD_DISP_I8080THRD_CMD_WR_THRD_POS);
    REG_WRITE(LCD_DISP_I8080THRD_ADDR,reg_value);
}

/* REG_0x07:i8080thrd->dat_rd_thrd:0x7[23:16],i8080 dat fifo rd thrd,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_i8080thrd_dat_rd_thrd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080THRD_ADDR);
    reg_value = ((reg_value >> LCD_DISP_I8080THRD_DAT_RD_THRD_POS) & LCD_DISP_I8080THRD_DAT_RD_THRD_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_i8080thrd_dat_rd_thrd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080THRD_ADDR);
    reg_value &= ~(LCD_DISP_I8080THRD_DAT_RD_THRD_MASK << LCD_DISP_I8080THRD_DAT_RD_THRD_POS);
    reg_value |= ((value & LCD_DISP_I8080THRD_DAT_RD_THRD_MASK) << LCD_DISP_I8080THRD_DAT_RD_THRD_POS);
    REG_WRITE(LCD_DISP_I8080THRD_ADDR,reg_value);
}

/* REG_0x07:i8080thrd->cmd_rd_thrd:0x7[31:24],i8080 cmd fifo rd thrd,0x0,r/w*/
static inline uint32_t lcd_disp_ll_get_i8080thrd_cmd_rd_thrd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080THRD_ADDR);
    reg_value = ((reg_value >> LCD_DISP_I8080THRD_CMD_RD_THRD_POS) & LCD_DISP_I8080THRD_CMD_RD_THRD_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_i8080thrd_cmd_rd_thrd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080THRD_ADDR);
    reg_value &= ~(LCD_DISP_I8080THRD_CMD_RD_THRD_MASK << LCD_DISP_I8080THRD_CMD_RD_THRD_POS);
    reg_value |= ((value & LCD_DISP_I8080THRD_CMD_RD_THRD_MASK) << LCD_DISP_I8080THRD_CMD_RD_THRD_POS);
    REG_WRITE(LCD_DISP_I8080THRD_ADDR,reg_value);
}

#ifdef __cplusplus 
}                  
#endif             
