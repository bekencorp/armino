// Copyright 2021-2022 Beken
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
#define LCD_DISP_DEVICE_ID_ADDR  (LCD_DISP_LL_REG_BASE  + 0x0*4)
static inline uint32_t lcd_disp_ll_get_devide_id(void)
{
    return REG_READ(LCD_DISP_DEVICE_ID_ADDR);
}

/* REG_0x01 */
#define LCD_DISP_VERSION_ID_ADDR  (LCD_DISP_LL_REG_BASE  + 0x1*4)
static inline uint32_t lcd_disp_ll_get_version_id(void)
{
    return REG_READ(LCD_DISP_VERSION_ID_ADDR);
}

/* REG_0x02 */
#define LCD_DISP_MODULE_CONTROL_ADDR  (LCD_DISP_LL_REG_BASE  + 0x2*4)
#define LCD_DISP_MODULE_CONTROL_REG_SOFT_RESET_POS (0)
#define LCD_DISP_MODULE_CONTROL_REG_SOFT_RESET_MASK (0x1)

#define LCD_DISP_MODULE_CONTROL_CLK_GATE_POS (1)
#define LCD_DISP_MODULE_CONTROL_CLK_GATE_MASK (0x1)

static inline uint32_t lcd_disp_ll_get_module_control_soft_reset(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_MODULE_CONTROL_ADDR);
    reg_value = ((reg_value >> LCD_DISP_MODULE_CONTROL_REG_SOFT_RESET_POS) & LCD_DISP_MODULE_CONTROL_REG_SOFT_RESET_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_module_control_soft_reset(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_MODULE_CONTROL_ADDR);
    reg_value &= ~(LCD_DISP_MODULE_CONTROL_REG_SOFT_RESET_MASK << LCD_DISP_MODULE_CONTROL_REG_SOFT_RESET_POS);
    reg_value |= ((value & LCD_DISP_MODULE_CONTROL_REG_SOFT_RESET_MASK) << LCD_DISP_MODULE_CONTROL_REG_SOFT_RESET_POS);
    REG_WRITE(LCD_DISP_MODULE_CONTROL_ADDR,reg_value);
}

static inline uint32_t lcd_disp_ll_get_module_control_clk_gate(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_MODULE_CONTROL_ADDR);
    reg_value = ((reg_value >> LCD_DISP_MODULE_CONTROL_CLK_GATE_POS) & LCD_DISP_MODULE_CONTROL_CLK_GATE_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_module_control_clk_gate(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_MODULE_CONTROL_ADDR);
    reg_value &= ~(LCD_DISP_MODULE_CONTROL_CLK_GATE_MASK << LCD_DISP_MODULE_CONTROL_CLK_GATE_POS);
    reg_value |= ((value & LCD_DISP_MODULE_CONTROL_CLK_GATE_MASK) << LCD_DISP_MODULE_CONTROL_CLK_GATE_POS);
    REG_WRITE(LCD_DISP_MODULE_CONTROL_ADDR,reg_value);
}

/* REG_0x03 */
#define LCD_DISP_GLOBAL_STATUS_ADDR  (LCD_DISP_LL_REG_BASE  + 0x3*4)
static inline uint32_t lcd_disp_ll_get_global_status(void)
{
    return REG_READ(LCD_DISP_GLOBAL_STATUS_ADDR);
}

/* REG_0x04 */
#define LCD_DISP_DISPLAY_INT_ADDR  (LCD_DISP_LL_REG_BASE  + 0x4*4)
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

#define LCD_DISP_DISPLAY_INT_I8080_DE_POS (8)
#define LCD_DISP_DISPLAY_INT_I8080_DE_MASK (0x1)

#define LCD_DISP_DISPLAY_INT_RESERVED1_POS (9)
#define LCD_DISP_DISPLAY_INT_RESERVED1_MASK (0x3FFFF)

#define LCD_DISP_DISPLAY_INT_DE_INT_EN_POS (27)
#define LCD_DISP_DISPLAY_INT_DE_INT_EN_MASK (0x1)

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

static inline uint32_t lcd_disp_ll_get_display_int_i8080_de(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISPLAY_INT_I8080_DE_POS) & LCD_DISP_DISPLAY_INT_I8080_DE_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_display_int_i8080_de(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value &= ~(LCD_DISP_DISPLAY_INT_I8080_DE_MASK << LCD_DISP_DISPLAY_INT_I8080_DE_POS);
    reg_value |= ((value & LCD_DISP_DISPLAY_INT_I8080_DE_MASK) << LCD_DISP_DISPLAY_INT_I8080_DE_POS);
    REG_WRITE(LCD_DISP_DISPLAY_INT_ADDR,reg_value);
}

static inline uint32_t lcd_disp_ll_get_display_int_de_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISPLAY_INT_I8080_DE_POS) & LCD_DISP_DISPLAY_INT_I8080_DE_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_display_int_de_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISPLAY_INT_ADDR);
    reg_value &= ~(LCD_DISP_DISPLAY_INT_DE_INT_EN_MASK << LCD_DISP_DISPLAY_INT_DE_INT_EN_POS);
    reg_value |= ((value & LCD_DISP_DISPLAY_INT_DE_INT_EN_MASK) << LCD_DISP_DISPLAY_INT_DE_INT_EN_POS);
    REG_WRITE(LCD_DISP_DISPLAY_INT_ADDR,reg_value);
}

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

/* REG_0x05 */
#define LCD_DISP_RGB_CFG_ADDR  (LCD_DISP_LL_REG_BASE  + 0x5*4)
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

/* REG_0x06 */
#define LCD_DISP_RGB_FIFO_ADDR  (LCD_DISP_LL_REG_BASE  + 0x6*4)
#define LCD_DISP_RGB_FIFO_RESERVED_POS (0)
#define LCD_DISP_RGB_FIFO_RESERVED_MASK (0xFFFFFFFF)

/* REG_0x07 */
#define LCD_DISP_SYNC_CFG_ADDR  (LCD_DISP_LL_REG_BASE  + 0x7*4)
#define LCD_DISP_SYNC_CFG_HSYNC_BACK_PORCH_POS (0)
#define LCD_DISP_SYNC_CFG_HSYNC_BACK_PORCH_MASK (0xFF)

#define LCD_DISP_SYNC_CFG_HSYNC_FRONT_PORCH_POS (8)
#define LCD_DISP_SYNC_CFG_HSYNC_FRONT_PORCH_MASK (0x7F)

#define LCD_DISP_SYNC_CFG_VSYNC_BACK_PORCH_POS (15)
#define LCD_DISP_SYNC_CFG_VSYNC_BACK_PORCH_MASK (0x1F)

#define LCD_DISP_SYNC_CFG_VSYNC_FRONT_PORCH_POS (20)
#define LCD_DISP_SYNC_CFG_VSYNC_FRONT_PORCH_MASK (0x7F)

#define LCD_DISP_SYNC_CFG_RESERVED1_POS (27)
#define LCD_DISP_SYNC_CFG_RESERVED1_MASK (0x1)

#define LCD_DISP_SYNC_CFG_YUV_SEL_POS (28)
#define LCD_DISP_SYNC_CFG_YUV_SEL_MASK (0x7)

#define LCD_DISP_SYNC_CFG_RESERVED0_POS (31)
#define LCD_DISP_SYNC_CFG_RESERVED0_MASK (0x1)

static inline uint32_t lcd_disp_ll_get_sync_cfg_value(void)
{
    return REG_READ(LCD_DISP_SYNC_CFG_ADDR);
}

static inline void lcd_disp_ll_set_sync_cfg_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_SYNC_CFG_ADDR,value);
}

static inline uint32_t lcd_disp_ll_get_sync_cfg_hsync_back_porch(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_SYNC_CFG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_SYNC_CFG_HSYNC_BACK_PORCH_POS) & LCD_DISP_SYNC_CFG_HSYNC_BACK_PORCH_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_sync_cfg_hsync_back_porch(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_SYNC_CFG_ADDR);
    reg_value &= ~(LCD_DISP_SYNC_CFG_HSYNC_BACK_PORCH_MASK << LCD_DISP_SYNC_CFG_HSYNC_BACK_PORCH_POS);
    reg_value |= ((value & LCD_DISP_SYNC_CFG_HSYNC_BACK_PORCH_MASK) << LCD_DISP_SYNC_CFG_HSYNC_BACK_PORCH_POS);
    REG_WRITE(LCD_DISP_SYNC_CFG_ADDR,reg_value);
}

static inline uint32_t lcd_disp_ll_get_sync_cfg_hsync_front_porch(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_SYNC_CFG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_SYNC_CFG_HSYNC_FRONT_PORCH_POS) & LCD_DISP_SYNC_CFG_HSYNC_FRONT_PORCH_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_sync_cfg_hsync_front_porch(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_SYNC_CFG_ADDR);
    reg_value &= ~(LCD_DISP_SYNC_CFG_HSYNC_FRONT_PORCH_MASK << LCD_DISP_SYNC_CFG_HSYNC_FRONT_PORCH_POS);
    reg_value |= ((value & LCD_DISP_SYNC_CFG_HSYNC_FRONT_PORCH_MASK) << LCD_DISP_SYNC_CFG_HSYNC_FRONT_PORCH_POS);
    REG_WRITE(LCD_DISP_SYNC_CFG_ADDR,reg_value);
}

static inline uint32_t lcd_disp_ll_get_sync_cfg_vsync_back_porch(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_SYNC_CFG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_SYNC_CFG_VSYNC_BACK_PORCH_POS) & LCD_DISP_SYNC_CFG_VSYNC_BACK_PORCH_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_sync_cfg_vsync_back_porch(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_SYNC_CFG_ADDR);
    reg_value &= ~(LCD_DISP_SYNC_CFG_VSYNC_BACK_PORCH_MASK << LCD_DISP_SYNC_CFG_VSYNC_BACK_PORCH_POS);
    reg_value |= ((value & LCD_DISP_SYNC_CFG_VSYNC_BACK_PORCH_MASK) << LCD_DISP_SYNC_CFG_VSYNC_BACK_PORCH_POS);
    REG_WRITE(LCD_DISP_SYNC_CFG_ADDR,reg_value);
}

static inline uint32_t lcd_disp_ll_get_sync_cfg_vsync_front_porch(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_SYNC_CFG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_SYNC_CFG_VSYNC_FRONT_PORCH_POS) & LCD_DISP_SYNC_CFG_VSYNC_FRONT_PORCH_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_sync_cfg_vsync_front_porch(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_SYNC_CFG_ADDR);
    reg_value &= ~(LCD_DISP_SYNC_CFG_VSYNC_FRONT_PORCH_MASK << LCD_DISP_SYNC_CFG_VSYNC_FRONT_PORCH_POS);
    reg_value |= ((value & LCD_DISP_SYNC_CFG_VSYNC_FRONT_PORCH_MASK) << LCD_DISP_SYNC_CFG_VSYNC_FRONT_PORCH_POS);
    REG_WRITE(LCD_DISP_SYNC_CFG_ADDR,reg_value);
}

static inline uint32_t lcd_disp_ll_get_sync_cfg_yuv_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_SYNC_CFG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_SYNC_CFG_YUV_SEL_POS) & LCD_DISP_SYNC_CFG_YUV_SEL_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_sync_cfg_yuv_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_SYNC_CFG_ADDR);
    reg_value &= ~(LCD_DISP_SYNC_CFG_YUV_SEL_MASK << LCD_DISP_SYNC_CFG_YUV_SEL_POS);
    reg_value |= ((value & LCD_DISP_SYNC_CFG_YUV_SEL_MASK) << LCD_DISP_SYNC_CFG_YUV_SEL_POS);
    REG_WRITE(LCD_DISP_SYNC_CFG_ADDR,reg_value);
}

/* REG_0x08 */
#define LCD_DISP_I8080_CONFIG_ADDR  (LCD_DISP_LL_REG_BASE  + 0x8*4)
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

#define LCD_DISP_I8080_CONFIG_RESERVED2_POS (6)
#define LCD_DISP_I8080_CONFIG_RESERVED2_MASK (0x1)

#define LCD_DISP_I8080_CONFIG_I8080_ENA_POS (7)
#define LCD_DISP_I8080_CONFIG_I8080_ENA_MASK (0x1)

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

static inline uint32_t lcd_disp_ll_get_i8080_config_i8080_ena(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CONFIG_ADDR);
    reg_value = ((reg_value >> LCD_DISP_I8080_CONFIG_I8080_ENA_POS) & LCD_DISP_I8080_CONFIG_I8080_ENA_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_i8080_config_i8080_ena(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CONFIG_ADDR);
    reg_value &= ~(LCD_DISP_I8080_CONFIG_I8080_ENA_MASK << LCD_DISP_I8080_CONFIG_I8080_ENA_POS);
    reg_value |= ((value & LCD_DISP_I8080_CONFIG_I8080_ENA_MASK) << LCD_DISP_I8080_CONFIG_I8080_ENA_POS);
    REG_WRITE(LCD_DISP_I8080_CONFIG_ADDR,reg_value);
}

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

/* REG_0x09 */
#define LCD_DISP_I8080_CMD_FIFO_ADDR  (LCD_DISP_LL_REG_BASE  + 0x9*4)
#define LCD_DISP_I8080_CMD_FIFO_I8080_CMD_FIFO_POS (0)
#define LCD_DISP_I8080_CMD_FIFO_I8080_CMD_FIFO_MASK (0xFFFF)

#define LCD_DISP_I8080_CMD_FIFO_RESERVED_POS (16)
#define LCD_DISP_I8080_CMD_FIFO_RESERVED_MASK (0xFFFF)

static inline uint32_t lcd_disp_ll_get_i8080_cmd_fifo_value(void)
{
    return REG_READ(LCD_DISP_I8080_CMD_FIFO_ADDR);
}


static inline void lcd_disp_ll_set_i8080_cmd_fifo_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_I8080_CMD_FIFO_ADDR,value);
}

static inline uint32_t lcd_disp_ll_get_i8080_cmd_fifo_i8080_cmd_fifo(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CMD_FIFO_ADDR);
    reg_value = ((reg_value >> LCD_DISP_I8080_CMD_FIFO_I8080_CMD_FIFO_POS) & LCD_DISP_I8080_CMD_FIFO_I8080_CMD_FIFO_MASK);
    return reg_value;
}

static inline void lcd_disp_ll_set_i8080_cmd_fifo_i8080_cmd_fifo(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_I8080_CMD_FIFO_ADDR);
    reg_value &= ~(LCD_DISP_I8080_CMD_FIFO_I8080_CMD_FIFO_MASK << LCD_DISP_I8080_CMD_FIFO_I8080_CMD_FIFO_POS);
    reg_value |= ((value & LCD_DISP_I8080_CMD_FIFO_I8080_CMD_FIFO_MASK) << LCD_DISP_I8080_CMD_FIFO_I8080_CMD_FIFO_POS);
    REG_WRITE(LCD_DISP_I8080_CMD_FIFO_ADDR,reg_value);
}

/* REG_0x0a */
#define LCD_DISP_I8080_DAT_FIFO_ADDR  (LCD_DISP_LL_REG_BASE  + 0xa*4)
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

static inline uint32_t lcd_disp_ll_get_i8080_dat_fifo_i8080_dat_fifo(void)
{
    return REG_READ(LCD_DISP_I8080_DAT_FIFO_ADDR);
}

static inline void lcd_disp_ll_set_i8080_dat_fifo_i8080_dat_fifo(uint32_t value)
{
    REG_WRITE(LCD_DISP_I8080_DAT_FIFO_ADDR,value);
}

/* REG_0x0b */
#define LCD_DISP_I8080_THRD_ADDR  (LCD_DISP_LL_REG_BASE  + 0xb*4)
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

/* REG_0x0c */
#define LCD_DISP_DISP_STATUS_ADDR  (LCD_DISP_LL_REG_BASE  + 0xc*4)
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

static inline uint32_t lcd_disp_ll_get_disp_status_rgb_ver_cnt(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISP_STATUS_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISP_STATUS_RGB_VER_CNT_POS)&LCD_DISP_DISP_STATUS_RGB_VER_CNT_MASK);
    return reg_value;
}

static inline uint32_t lcd_disp_ll_get_disp_status_i8080_cmd_cfg_done(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISP_STATUS_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISP_STATUS_I8080_CMD_CFG_DONE_POS)&LCD_DISP_DISP_STATUS_I8080_CMD_CFG_DONE_MASK);
    return reg_value;
}

static inline uint32_t lcd_disp_ll_get_disp_status_i8080_ver_cnt(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISP_STATUS_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISP_STATUS_I8080_VER_CNT_POS)&LCD_DISP_DISP_STATUS_I8080_VER_CNT_MASK);
    return reg_value;
}

static inline uint32_t lcd_disp_ll_get_disp_status_disp_fifo_empty(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISP_STATUS_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISP_STATUS_DISP_FIFO_EMPTY_POS)&LCD_DISP_DISP_STATUS_DISP_FIFO_EMPTY_MASK);
    return reg_value;
}

static inline uint32_t lcd_disp_ll_get_disp_status_disp_fifo_near_full(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(LCD_DISP_DISP_STATUS_ADDR);
    reg_value = ((reg_value >> LCD_DISP_DISP_STATUS_DISP_FIFO_NEAR_FULL_POS)&LCD_DISP_DISP_STATUS_DISP_FIFO_NEAR_FULL_MASK);
    return reg_value;
}

/* REG_0x0d */
#define LCD_DISP_RGB_SYNC_LOW_ADDR  (LCD_DISP_LL_REG_BASE  + 0xd*4)
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

/* REG_0x0e */
#define LCD_DISP_RGB_CLUM_OFFSET_ADDR  (LCD_DISP_LL_REG_BASE  + 0xe*4)
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

/* REG_0xf */
#define LCD_DISP_RGB_LINE_OFFSET_ADDR  (LCD_DISP_LL_REG_BASE  + 0xf*4)
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

/* REG_0x10 */
#define LCD_DISP_DAT_FIFO_THRD_ADDR  (LCD_DISP_LL_REG_BASE  + 0x10*4)

#define LCD_DISP_DAT_FIFO_THRD_RESERVED2_POS (0)
#define LCD_DISP_DAT_FIFO_THRD_RESERVED2_MASK (0xFF)

#define LCD_DISP_DAT_FIFO_THRD_DAT_WR_THRD_POS (8)
#define LCD_DISP_DAT_FIFO_THRD_DAT_WR_THRD_MASK (0x3FF)

#define LCD_DISP_DAT_FIFO_THRD_RESERVED1_POS (18)
#define LCD_DISP_DAT_FIFO_THRD_RESERVED1_MASK (0x3)

#define LCD_DISP_DAT_FIFO_THRD_DAT_RD_THRD_POS (20)
#define LCD_DISP_DAT_FIFO_THRD_DAT_RD_THRD_MASK (0x3FF)

#define LCD_DISP_DAT_FIFO_THRD_RESERVED0_POS (30)
#define LCD_DISP_DAT_FIFO_THRD_RESERVED0_MASK (0x3)

static inline uint32_t lcd_disp_ll_get_dat_fifo_thrd_value(void)
{
    return REG_READ(LCD_DISP_DAT_FIFO_THRD_ADDR);
}

static inline void lcd_disp_ll_set_dat_fifo_thrd_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_DAT_FIFO_THRD_ADDR,value);
}

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

/* REG_0x11 */
#define LCD_DISP_MATER_RD_BASE_ADDR_ADDR  (LCD_DISP_LL_REG_BASE  + 0x11*4)
#define LCD_DISP_MATER_RD_BASE_ADDR_MASTER_RD_BASE_ADR_POS (0)
#define LCD_DISP_MATER_RD_BASE_ADDR_MASTER_RD_BASE_ADR_MASK (0xFFFFFFFF)

static inline uint32_t lcd_disp_ll_get_mater_rd_base_addr(void)
{
    return REG_READ(LCD_DISP_MATER_RD_BASE_ADDR_ADDR);
}

static inline void lcd_disp_ll_set_mater_rd_base_addr(uint32_t value)
{
    REG_WRITE(LCD_DISP_MATER_RD_BASE_ADDR_ADDR,value);
}


/* REG_0x13 */
#define LCD_DISP_CMD_COUNT_ADDR  (LCD_DISP_LL_REG_BASE  + 0x13*4)
#define LCD_DISP_CMD_COUNT_I8080_CMD_PARA_COUNT_POS (0)
#define LCD_DISP_CMD_COUNT_I8080_CMD_PARA_COUNT_MASK (0x3FFFFF)

static inline uint32_t lcd_disp_ll_get_cmd_count_value(void)
{
    return REG_READ(LCD_DISP_CMD_COUNT_ADDR);
}

static inline void lcd_disp_ll_set_cmd_count_value(uint32_t value)
{
    REG_WRITE(LCD_DISP_CMD_COUNT_ADDR,value);
}


static inline uint32_t lcd_disp_ll_get_cmd_count_i8080_cmd_para_count(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(LCD_DISP_CMD_COUNT_ADDR);
	reg_value = ((reg_value >> LCD_DISP_CMD_COUNT_I8080_CMD_PARA_COUNT_POS) & LCD_DISP_CMD_COUNT_I8080_CMD_PARA_COUNT_MASK);
	return reg_value;
}

static inline void lcd_disp_ll_set_dat_fifo_thrd_i8080_cmd_para_count(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(LCD_DISP_CMD_COUNT_ADDR);
	reg_value &= ~(LCD_DISP_CMD_COUNT_I8080_CMD_PARA_COUNT_MASK << LCD_DISP_CMD_COUNT_I8080_CMD_PARA_COUNT_POS);
	reg_value |= ((value & LCD_DISP_CMD_COUNT_I8080_CMD_PARA_COUNT_MASK) << LCD_DISP_CMD_COUNT_I8080_CMD_PARA_COUNT_POS);
	REG_WRITE(LCD_DISP_CMD_COUNT_ADDR,reg_value);
}

#ifdef __cplusplus
}
#endif

