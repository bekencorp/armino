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


#pragma once

#include <soc/soc.h>
#include "dma2d_struct.h"

#ifdef __cplusplus
extern "C" {
#endif
/* REG_0x00 */

static inline uint32_t dma2d_ll_get_dma2d_control_reg_value(dma2d_hw_t *hw)
{
    return hw->dma2d_control_reg.v;
}

static inline void dma2d_ll_set_dma2d_control_reg_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_control_reg.v = value;
}

/* REG_0x00:dma2d_control_reg->tran_start:dma2d transfer start.,RW,0x0[       0]*/
static inline uint32_t dma2d_ll_get_dma2d_control_reg_tran_start(dma2d_hw_t *hw)
{
    return hw->dma2d_control_reg.tran_start;
}

static inline void dma2d_ll_set_dma2d_control_reg_tran_start(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_control_reg.tran_start = value;
}

/* REG_0x00:dma2d_control_reg->tran_suspend:dma2d transfer start.,RW,0x0[       1]*/
static inline uint32_t dma2d_ll_get_dma2d_control_reg_tran_suspend(dma2d_hw_t *hw)
{
    return hw->dma2d_control_reg.tran_suspend;
}

static inline void dma2d_ll_set_dma2d_control_reg_tran_suspend(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_control_reg.tran_suspend = value;
}

/* REG_0x00:dma2d_control_reg->tran_abort:dma2d transfer start.,RW,0x0[       2]*/
static inline uint32_t dma2d_ll_get_dma2d_control_reg_tran_abort(dma2d_hw_t *hw)
{
    return hw->dma2d_control_reg.tran_abort;
}

static inline void dma2d_ll_set_dma2d_control_reg_tran_abort(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_control_reg.tran_abort = value;
}

/* REG_0x00:dma2d_control_reg->line_offset_mode:line's offset mode sel: 0:in pixel express; 1: in bytes express.,RW,0x0[       6]*/
static inline uint32_t dma2d_ll_get_dma2d_control_reg_line_offset_mode(dma2d_hw_t *hw)
{
    return hw->dma2d_control_reg.line_offset_mode;
}

static inline void dma2d_ll_set_dma2d_control_reg_line_offset_mode(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_control_reg.line_offset_mode = value;
}

/* REG_0x00:dma2d_control_reg->error_int_en:trabsfer error int ena.,RW,0x0[       8]*/
static inline uint32_t dma2d_ll_get_dma2d_control_reg_error_int_en(dma2d_hw_t *hw)
{
    return hw->dma2d_control_reg.error_int_en;
}

static inline void dma2d_ll_set_dma2d_control_reg_error_int_en(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_control_reg.error_int_en = value;
}

/* REG_0x00:dma2d_control_reg->complete_int_en:transfer complete int ena.,RW,0x0[       9]*/
static inline uint32_t dma2d_ll_get_dma2d_control_reg_complete_int_en(dma2d_hw_t *hw)
{
    return hw->dma2d_control_reg.complete_int_en;
}

static inline void dma2d_ll_set_dma2d_control_reg_complete_int_en(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_control_reg.complete_int_en = value;
}

/* REG_0x00:dma2d_control_reg->waterm_int_en:transfer watermark int ena.,RW,0x0[     10]*/
static inline uint32_t dma2d_ll_get_dma2d_control_reg_waterm_int_en(dma2d_hw_t *hw)
{
    return hw->dma2d_control_reg.waterm_int_en;
}

static inline void dma2d_ll_set_dma2d_control_reg_waterm_int_en(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_control_reg.waterm_int_en = value;
}

/* REG_0x00:dma2d_control_reg->clut_error_int_en:clut transfer error int ena.,RW,0x0[     11]*/
static inline uint32_t dma2d_ll_get_dma2d_control_reg_clut_error_int_en(dma2d_hw_t *hw)
{
    return hw->dma2d_control_reg.clut_error_int_en;
}

static inline void dma2d_ll_set_dma2d_control_reg_clut_error_int_en(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_control_reg.clut_error_int_en = value;
}

/* REG_0x00:dma2d_control_reg->clut_cmplt_int_en:clut transfer complete int ena.,RW,0x0[     12]*/
static inline uint32_t dma2d_ll_get_dma2d_control_reg_clut_cmplt_int_en(dma2d_hw_t *hw)
{
    return hw->dma2d_control_reg.clut_cmplt_int_en;
}

static inline void dma2d_ll_set_dma2d_control_reg_clut_cmplt_int_en(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_control_reg.clut_cmplt_int_en = value;
}

/* REG_0x00:dma2d_control_reg->config_error_int_en:config error int ena,,RW,0x0[     13]*/
static inline uint32_t dma2d_ll_get_dma2d_control_reg_config_error_int_en(dma2d_hw_t *hw)
{
    return hw->dma2d_control_reg.config_error_int_en;
}

static inline void dma2d_ll_set_dma2d_control_reg_config_error_int_en(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_control_reg.config_error_int_en = value;
}

/* REG_0x00:dma2d_control_reg->mode:DMA2D mode sel: 000:m2m; 001:m2m pixel convert with fg; 010:m2m blend; 011:r2m.only with output; 100: m2m blend fix fg; 101:m2m blend fix bg;,RW,0x0[18:16]*/
static inline uint32_t dma2d_ll_get_dma2d_control_reg_mode(dma2d_hw_t *hw)
{
    return hw->dma2d_control_reg.mode;
}

static inline void dma2d_ll_set_dma2d_control_reg_mode(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_control_reg.mode = value;
}

/* REG_0x00:[19:20] max transfer length:00:256bytes; 01:192 bytes;  10:128bytes;  11:64bytes;*/
static inline uint32_t dma2d_ll_get_dma2d_control_reg_master_tran_length(dma2d_hw_t *hw)
{
    return hw->dma2d_control_reg.master_tran_length;
}

/* REG_0x00:[19:20] dma2dmax transfer length:00:256bytes; 01:192 bytes;  10:128bytes;  11:64bytes;*/
static inline void dma2d_ll_set_dma2d_control_reg_master_tran_length(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_control_reg.master_tran_length = value;
}

/* REG_0x00:[21] in output rgb888 formart, reverse data byte by byte.;*/
static inline uint32_t dma2d_ll_get_dma2d_control_reg_out_byte_revese(dma2d_hw_t *hw)
{
    return hw->dma2d_control_reg.out_byte_revese;
}

/* REG_0x00:[21] in output rgb888 formart, reverse data byte by byte.*/
static inline void dma2d_ll_set_dma2d_control_reg_out_byte_revese(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_control_reg.out_byte_revese = value;
}

/* REG_0x01 */

/* REG_0x01 */

static inline uint32_t dma2d_ll_get_dma2d_int_status_value(dma2d_hw_t *hw)
{
    return hw->dma2d_int_status.v;
}

/* REG_0x01:dma2d_int_status->error_int:transfer error int flag.,R,0x1[       0]*/
static inline uint32_t dma2d_ll_get_dma2d_int_status_error_int(dma2d_hw_t *hw)
{
    return hw->dma2d_int_status.error_int;
}

/* REG_0x01:dma2d_int_status->complete_int:transfer complete int flag.,R,0x1[       1]*/
static inline uint32_t dma2d_ll_get_dma2d_int_status_complete_int(dma2d_hw_t *hw)
{
    return hw->dma2d_int_status.complete_int;
}

/* REG_0x01:dma2d_int_status->waterm_int:transfer watermark intt flag.,R,0x1[       2]*/
static inline uint32_t dma2d_ll_get_dma2d_int_status_waterm_int(dma2d_hw_t *hw)
{
    return hw->dma2d_int_status.waterm_int;
}

/* REG_0x01:dma2d_int_status->clut_error_int:clut transfer error intt flag.,R,0x1[       3]*/
static inline uint32_t dma2d_ll_get_dma2d_int_status_clut_error_int(dma2d_hw_t *hw)
{
    return hw->dma2d_int_status.clut_error_int;
}

/* REG_0x01:dma2d_int_status->clut_cmplt_int:clut transfer complete intt flag.,R,0x1[       4]*/
static inline uint32_t dma2d_ll_get_dma2d_int_status_clut_cmplt_int(dma2d_hw_t *hw)
{
    return hw->dma2d_int_status.clut_cmplt_int;
}

/* REG_0x01:dma2d_int_status->config_error_int:config error int t flag.,R,0x1[       5]*/
static inline uint32_t dma2d_ll_get_dma2d_int_status_config_error_int(dma2d_hw_t *hw)
{
    return hw->dma2d_int_status.config_error_int;
}

/* REG_0x02 */

/* REG_0x02 */

static inline void dma2d_ll_set_dma2d_int_clear_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_int_clear.v = value;
}

/* REG_0x02:dma2d_int_clear->clr_error_int:clear transfer error int flag.,W,0x2[       0]*/
static inline void dma2d_ll_set_dma2d_int_clear_clr_error_int(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_int_clear.clr_error_int = value;
}

/* REG_0x02:dma2d_int_clear->clr_complete_int:clear transfer complete int flag.,W,0x2[       1]*/
static inline void dma2d_ll_set_dma2d_int_clear_clr_complete_int(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_int_clear.clr_complete_int = value;
}

/* REG_0x02:dma2d_int_clear->clr_waterm_int:clear transfer watermark intt flag.,W,0x2[       2]*/
static inline void dma2d_ll_set_dma2d_int_clear_clr_waterm_int(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_int_clear.clr_waterm_int = value;
}

/* REG_0x02:dma2d_int_clear->clr_clut_error_int:clear clut transfer error intt flag.,W,0x2[       3]*/
static inline void dma2d_ll_set_dma2d_int_clear_clr_clut_error_int(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_int_clear.clr_clut_error_int = value;
}

/* REG_0x02:dma2d_int_clear->clr_clut_cmplt_int:clear clut transfer complete intt flag.,W,0x2[       4]*/
static inline void dma2d_ll_set_dma2d_int_clear_clr_clut_cmplt_int(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_int_clear.clr_clut_cmplt_int = value;
}

/* REG_0x02:dma2d_int_clear->clr_config_error_int:clear config error int t flag.,W,0x2[       5]*/
static inline void dma2d_ll_set_dma2d_int_clear_clr_config_error_int(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_int_clear.clr_config_error_int = value;
}

/* REG_0x03 */

static inline uint32_t dma2d_ll_get_dma2d_fg_address_value(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_address.v;
}

static inline void dma2d_ll_set_dma2d_fg_address_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_address.v = value;
}

/* REG_0x03:dma2d_fg_address->fg_address:foreground mem address, written when transfer disable.,RW,0x3[31:  0]*/
static inline uint32_t dma2d_ll_get_dma2d_fg_address_fg_address(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_address.v;
}

static inline void dma2d_ll_set_dma2d_fg_address_fg_address(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_address.v = value;
}

/* REG_0x04 */

static inline uint32_t dma2d_ll_get_dma2d_fg_offset_value(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_offset.v;
}

static inline void dma2d_ll_set_dma2d_fg_offset_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_offset.v = value;
}

/* REG_0x04:dma2d_fg_offset->fg_line_offset:line offset for fg, offset In pixel or bytes.,RW,0x4[15:  0]*/
static inline uint32_t dma2d_ll_get_dma2d_fg_offset_fg_line_offset(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_offset.fg_line_offset;
}

static inline void dma2d_ll_set_dma2d_fg_offset_fg_line_offset(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_offset.fg_line_offset = value;
}

/* REG_0x05 */

static inline uint32_t dma2d_ll_get_dma2d_bg_address_value(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_address.v;
}

static inline void dma2d_ll_set_dma2d_bg_address_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_address.v = value;
}

/* REG_0x05:dma2d_bg_address->bg_address:background mem address, written when transfer disable.,RW,0x5[31:  0]*/
static inline uint32_t dma2d_ll_get_dma2d_bg_address_bg_address(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_address.v;
}

static inline void dma2d_ll_set_dma2d_bg_address_bg_address(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_address.v = value;
}

/* REG_0x06 */

static inline uint32_t dma2d_ll_get_dma2d_bg_offset_value(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_offset.v;
}

static inline void dma2d_ll_set_dma2d_bg_offset_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_offset.v = value;
}

/* REG_0x06:dma2d_bg_offset->bg_line_offset:line offset for bg, offset In pixel or bytes.,RW,0x6[15:  0]*/
static inline uint32_t dma2d_ll_get_dma2d_bg_offset_bg_line_offset(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_offset.bg_line_offset;
}

static inline void dma2d_ll_set_dma2d_bg_offset_bg_line_offset(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_offset.bg_line_offset = value;
}

/* REG_0x07 */

static inline uint32_t dma2d_ll_get_dma2d_fg_pfc_ctrl_value(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_pfc_ctrl.v;
}

static inline void dma2d_ll_set_dma2d_fg_pfc_ctrl_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_pfc_ctrl.v = value;
}

/* REG_0x07:dma2d_fg_pfc_ctrl->fg_color_mode:foreground image color mode.; 0000:ARGB888;0001:RGB888;0010:RGB565;0011:ARGB1555;0100:ARGB4444; 0101:L8;0110:AL44; 0111:AL88; 1000:L4; 1001:A8;1010:A4;,RW,0x7[  3:  0]*/
static inline uint32_t dma2d_ll_get_dma2d_fg_pfc_ctrl_fg_color_mode(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_pfc_ctrl.fg_color_mode;
}

static inline void dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_color_mode(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_pfc_ctrl.fg_color_mode = value;
}

/* REG_0x07:dma2d_fg_pfc_ctrl->fg_clut_color_mode:color mode for clut. 0:argb8888; 1:rgb888.,RW,0x7[       4]*/
static inline uint32_t dma2d_ll_get_dma2d_fg_pfc_ctrl_fg_clut_color_mode(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_pfc_ctrl.fg_clut_color_mode;
}

static inline void dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_clut_color_mode(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_pfc_ctrl.fg_clut_color_mode = value;
}

/* REG_0x07:dma2d_fg_pfc_ctrl->fg_start_clut:automatic load the clut. Automatic reset.,RW,0x7[       5]*/
static inline uint32_t dma2d_ll_get_dma2d_fg_pfc_ctrl_fg_start_clut(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_pfc_ctrl.fg_start_clut;
}

static inline void dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_start_clut(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_pfc_ctrl.fg_start_clut = value;
}

/* REG_0x07:dma2d_fg_pfc_ctrl->fg_clut_size:the size of clut used for foreground image. Size = fg_clut_size + 1;,RW,0x7[15:  8]*/
static inline uint32_t dma2d_ll_get_dma2d_fg_pfc_ctrl_fg_clut_size(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_pfc_ctrl.fg_clut_size;
}

static inline void dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_clut_size(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_pfc_ctrl.fg_clut_size = value;
}

/* REG_0x07:dma2d_fg_pfc_ctrl->fg_alpha_mode:alpha value use for fg image. 00: nochange; 01:replace orginal, 10: alpha[7:0] multipied with orginal value.,RW,0x7[17:16]*/
static inline uint32_t dma2d_ll_get_dma2d_fg_pfc_ctrl_fg_alpha_mode(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_pfc_ctrl.fg_alpha_mode;
}

static inline void dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_alpha_mode(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_pfc_ctrl.fg_alpha_mode = value;
}

/* REG_0x07:dma2d_fg_pfc_ctrl->alpha_invert:invert alpha value.,RW,0x7[     20]*/
static inline uint32_t dma2d_ll_get_dma2d_fg_pfc_ctrl_alpha_invert(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_pfc_ctrl.alpha_invert;
}

static inline void dma2d_ll_set_dma2d_fg_pfc_ctrl_alpha_invert(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_pfc_ctrl.alpha_invert = value;
}

/* REG_0x07:dma2d_fg_pfc_ctrl->fg_rb_swap:red blue swap to support rgb or argb. 0: regular mode.1:swap_mode.,RW,0x7[     21]*/
static inline uint32_t dma2d_ll_get_dma2d_fg_pfc_ctrl_fg_rb_swap(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_pfc_ctrl.fg_rb_swap;
}

static inline void dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_rb_swap(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_pfc_ctrl.fg_rb_swap = value;
}

/* REG_0x07:dma2d_fg_pfc_ctrl->fg_alpha:fg alpha value set. Use with fg_alpha_mode.,RW,0x7[31:24]*/
static inline uint32_t dma2d_ll_get_dma2d_fg_pfc_ctrl_fg_alpha(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_pfc_ctrl.fg_alpha;
}

static inline void dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_alpha(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_pfc_ctrl.fg_alpha = value;
}

/* REG_0x08 */

static inline uint32_t dma2d_ll_get_dma2d_fg_color_reg_value(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_color_reg.v;
}

static inline void dma2d_ll_set_dma2d_fg_color_reg_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_color_reg.v = value;
}

/* REG_0x08:dma2d_fg_color_reg->blue_value:blue_value in a4 or a8 mode of fg,,RW,0x8[  7:  0]*/
static inline uint32_t dma2d_ll_get_dma2d_fg_color_reg_blue_value(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_color_reg.blue_value;
}

static inline void dma2d_ll_set_dma2d_fg_color_reg_blue_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_color_reg.blue_value = value;
}

/* REG_0x08:dma2d_fg_color_reg->green_value:green_value in a4 or a8 mode of fg,,RW,0x8[15:  8]*/
static inline uint32_t dma2d_ll_get_dma2d_fg_color_reg_green_value(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_color_reg.green_value;
}

static inline void dma2d_ll_set_dma2d_fg_color_reg_green_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_color_reg.green_value = value;
}

/* REG_0x08:dma2d_fg_color_reg->red_value:red_value in a4 or a8 mode of fg,,RW,0x8[23:16]*/
static inline uint32_t dma2d_ll_get_dma2d_fg_color_reg_red_value(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_color_reg.red_value;
}

static inline void dma2d_ll_set_dma2d_fg_color_reg_red_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_color_reg.red_value = value;
}

/* REG_0x09 */

static inline uint32_t dma2d_ll_get_dma2d_bg_pfc_ctrl_value(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_pfc_ctrl.v;
}

static inline void dma2d_ll_set_dma2d_bg_pfc_ctrl_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_pfc_ctrl.v = value;
}

/* REG_0x09:dma2d_bg_pfc_ctrl->bg_color_mode:background image color mode.; 0000:ARGB888;0001:RGB888;0010:RGB565;0011:ARGB1555;0100:ARGB4444; 0101:L8;0110:AL44; 0111:AL88; 1000:L4; 1001:A8;1010:A4;,RW,0x9[  3:  0]*/
static inline uint32_t dma2d_ll_get_dma2d_bg_pfc_ctrl_bg_color_mode(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_pfc_ctrl.bg_color_mode;
}

static inline void dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_color_mode(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_pfc_ctrl.bg_color_mode = value;
}

/* REG_0x09:dma2d_bg_pfc_ctrl->bg_clut_color_mode:color mode for clut. 0:argb8888; 1:rgb888.,RW,0x9[       4]*/
static inline uint32_t dma2d_ll_get_dma2d_bg_pfc_ctrl_bg_clut_color_mode(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_pfc_ctrl.bg_clut_color_mode;
}

static inline void dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_clut_color_mode(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_pfc_ctrl.bg_clut_color_mode = value;
}

/* REG_0x09:dma2d_bg_pfc_ctrl->bg_start_clut:automatic load the clut. Automatic reset.,RW,0x9[       5]*/
static inline uint32_t dma2d_ll_get_dma2d_bg_pfc_ctrl_bg_start_clut(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_pfc_ctrl.bg_start_clut;
}

static inline void dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_start_clut(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_pfc_ctrl.bg_start_clut = value;
}

/* REG_0x09:dma2d_bg_pfc_ctrl->bg_clut_size:the size of clut used for background image. Size = fg_clut_size + 1;,RW,0x9[15:  8]*/
static inline uint32_t dma2d_ll_get_dma2d_bg_pfc_ctrl_bg_clut_size(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_pfc_ctrl.bg_clut_size;
}

static inline void dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_clut_size(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_pfc_ctrl.bg_clut_size = value;
}

/* REG_0x09:dma2d_bg_pfc_ctrl->bg_alpha_mode:alpha value use for fg image. 00: nochange; 01:replace orginal, 10: alpha[7:0] multipied with orginal value.,RW,0x9[17:16]*/
static inline uint32_t dma2d_ll_get_dma2d_bg_pfc_ctrl_bg_alpha_mode(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_pfc_ctrl.bg_alpha_mode;
}

static inline void dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_alpha_mode(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_pfc_ctrl.bg_alpha_mode = value;
}

/* REG_0x09:dma2d_bg_pfc_ctrl->alpha_invert:invert alpha value.,RW,0x9[     20]*/
static inline uint32_t dma2d_ll_get_dma2d_bg_pfc_ctrl_alpha_invert(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_pfc_ctrl.alpha_invert;
}

static inline void dma2d_ll_set_dma2d_bg_pfc_ctrl_alpha_invert(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_pfc_ctrl.alpha_invert = value;
}

/* REG_0x09:dma2d_bg_pfc_ctrl->bg_rb_swap:red blue swap to support rgb or argb. 0: regular mode.1:swap_mode.,RW,0x9[     21]*/
static inline uint32_t dma2d_ll_get_dma2d_bg_pfc_ctrl_bg_rb_swap(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_pfc_ctrl.bg_rb_swap;
}

static inline void dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_rb_swap(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_pfc_ctrl.bg_rb_swap = value;
}

/* REG_0x09:dma2d_bg_pfc_ctrl->bg_alpha:bg alpha value set. Use withbg_alpha_mode.,RW,0x9[31:24]*/
static inline uint32_t dma2d_ll_get_dma2d_bg_pfc_ctrl_bg_alpha(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_pfc_ctrl.bg_alpha;
}

static inline void dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_alpha(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_pfc_ctrl.bg_alpha = value;
}

/* REG_0x0a */

static inline uint32_t dma2d_ll_get_dma2d_bg_color_reg_value(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_color_reg.v;
}

static inline void dma2d_ll_set_dma2d_bg_color_reg_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_color_reg.v = value;
}

/* REG_0x0a:dma2d_bg_color_reg->blue_value:blue_value in a4 or a8 mode of bg,,RW,0xa[  7:  0]*/
static inline uint32_t dma2d_ll_get_dma2d_bg_color_reg_blue_value(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_color_reg.blue_value;
}

static inline void dma2d_ll_set_dma2d_bg_color_reg_blue_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_color_reg.blue_value = value;
}

/* REG_0x0a:dma2d_bg_color_reg->green_value:green_value in a4 or a8 mode of bg,,RW,0xa[15:  8]*/
static inline uint32_t dma2d_ll_get_dma2d_bg_color_reg_green_value(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_color_reg.green_value;
}

static inline void dma2d_ll_set_dma2d_bg_color_reg_green_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_color_reg.green_value = value;
}

/* REG_0x0a:dma2d_bg_color_reg->red_value:red_value in a4 or a8 mode of bg,,RW,0xa[23:16]*/
static inline uint32_t dma2d_ll_get_dma2d_bg_color_reg_red_value(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_color_reg.red_value;
}

static inline void dma2d_ll_set_dma2d_bg_color_reg_red_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_color_reg.red_value = value;
}

/* REG_0x0b */

static inline uint32_t dma2d_ll_get_fg_clut_mem_address_value(dma2d_hw_t *hw)
{
    return hw->fg_clut_mem_address.v;
}

static inline void dma2d_ll_set_fg_clut_mem_address_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->fg_clut_mem_address.v = value;
}

/* REG_0x0b:fg_clut_mem_address->fg_clut_address:clut data dedicated to the fg image.,RW,0xb[31: 0]*/
static inline uint32_t dma2d_ll_get_fg_clut_mem_address_fg_clut_address(dma2d_hw_t *hw)
{
    return hw->fg_clut_mem_address.v;
}

static inline void dma2d_ll_set_fg_clut_mem_address_fg_clut_address(dma2d_hw_t *hw, uint32_t value)
{
    hw->fg_clut_mem_address.v = value;
}

/* REG_0x0c */

static inline uint32_t dma2d_ll_get_bg_clut_mem_address_value(dma2d_hw_t *hw)
{
    return hw->bg_clut_mem_address.v;
}

static inline void dma2d_ll_set_bg_clut_mem_address_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->bg_clut_mem_address.v = value;
}

/* REG_0x0c:bg_clut_mem_address->bg_clut_address:clut data dedicated to the bg image.,RW,0xc[31:  0]*/
static inline uint32_t dma2d_ll_get_bg_clut_mem_address_bg_clut_address(dma2d_hw_t *hw)
{
    return hw->bg_clut_mem_address.v;
}

static inline void dma2d_ll_set_bg_clut_mem_address_bg_clut_address(dma2d_hw_t *hw, uint32_t value)
{
    hw->bg_clut_mem_address.v = value;
}

/* REG_0x0d */

static inline uint32_t dma2d_ll_get_out_pfc_contrl_value(dma2d_hw_t *hw)
{
    return hw->out_pfc_contrl.v;
}

static inline void dma2d_ll_set_out_pfc_contrl_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->out_pfc_contrl.v = value;
}

/* REG_0x0d:out_pfc_contrl->out_color_mode:format of output image.0:argb888; 1:rgb888; 010:rgb565; 011:argb1555; 100:argb444,RW,0xd[  2:  0]*/
static inline uint32_t dma2d_ll_get_out_pfc_contrl_out_color_mode(dma2d_hw_t *hw)
{
    return hw->out_pfc_contrl.out_color_mode;
}

static inline void dma2d_ll_set_out_pfc_contrl_out_color_mode(dma2d_hw_t *hw, uint32_t value)
{
    hw->out_pfc_contrl.out_color_mode = value;
}

/* REG_0x0d:out_pfc_contrl->out_swap_bytes:0:bytes in regular order. 1:bytes swaped two by two in output fifo.,RW,0xd[       9]*/
static inline uint32_t dma2d_ll_get_out_pfc_contrl_out_swap_bytes(dma2d_hw_t *hw)
{
    return hw->out_pfc_contrl.out_swap_bytes;
}

static inline void dma2d_ll_set_out_pfc_contrl_out_swap_bytes(dma2d_hw_t *hw, uint32_t value)
{
    hw->out_pfc_contrl.out_swap_bytes = value;
}

/* REG_0x0d:out_pfc_contrl->out_alpha_invert.:invert alpha value.,RW,0xd[     20]*/
static inline uint32_t dma2d_ll_get_out_pfc_contrl_out_alpha_invert(dma2d_hw_t *hw)
{
    return hw->out_pfc_contrl.out_alpha_invert;
}

static inline void dma2d_ll_set_out_pfc_contrl_out_alpha_invert(dma2d_hw_t *hw, uint32_t value)
{
    hw->out_pfc_contrl.out_alpha_invert = value;
}

/* REG_0x0d:out_pfc_contrl->out_rb_swap:ed blue swap to support rgb or argb. 0: regular mode.1:swap_mode.,RW,0xd[     21]*/
static inline uint32_t dma2d_ll_get_out_pfc_contrl_out_rb_swap(dma2d_hw_t *hw)
{
    return hw->out_pfc_contrl.out_rb_swap;
}

static inline void dma2d_ll_set_out_pfc_contrl_out_rb_swap(dma2d_hw_t *hw, uint32_t value)
{
    hw->out_pfc_contrl.out_rb_swap = value;
}

/* REG_0x0e */

static inline uint32_t dma2d_ll_get_out_color_reg_value(dma2d_hw_t *hw)
{
    return hw->out_color_reg.v;
}

static inline void dma2d_ll_set_out_color_reg_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->out_color_reg.v = value;
}

/* REG_0x0e:out_color_reg->output_clor_reg:output reg, in different color mode. Output is different.; Argb888 or rgb888: alpha[31:24];red[23:16]; green[15:8];blue[7:0].; Rgb565:red[15:11]; green[12:5]; blue[4:0];; argb1555:a[15];red[14:10]; green[4:0]; blue[4:0];; argb4444:alpha[15:12];red[11:8];green[7:4];blue[3;0].,RW,0xe[31:  0]*/
static inline uint32_t dma2d_ll_get_out_color_reg_output_clor_reg(dma2d_hw_t *hw)
{
    return hw->out_color_reg.v;
}

static inline void dma2d_ll_set_out_color_reg_output_clor_reg(dma2d_hw_t *hw, uint32_t value)
{
    hw->out_color_reg.v = value;
}

/* REG_0x0f */

static inline uint32_t dma2d_ll_get_dma2d_out_mem_address_value(dma2d_hw_t *hw)
{
    return hw->dma2d_out_mem_address.v;
}

static inline void dma2d_ll_set_dma2d_out_mem_address_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_out_mem_address.v = value;
}

/* REG_0x0f:dma2d_out_mem_address->out_mem_address:address of data output fifo.,RW,0xf[31:  0]*/
static inline uint32_t dma2d_ll_get_dma2d_out_mem_address_out_mem_address(dma2d_hw_t *hw)
{
    return hw->dma2d_out_mem_address.v;
}

static inline void dma2d_ll_set_dma2d_out_mem_address_out_mem_address(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_out_mem_address.v = value;
}

/* REG_0x10 */

static inline uint32_t dma2d_ll_get_output_offset_value(dma2d_hw_t *hw)
{
    return hw->output_offset.v;
}

static inline void dma2d_ll_set_output_offset_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->output_offset.v = value;
}

/* REG_0x10:output_offset->out_line_offset:output line offset. Offfset with bytes or pixel.in pixel[15:14] ignored.,RW,0x10[15:  0]*/
static inline uint32_t dma2d_ll_get_output_offset_out_line_offset(dma2d_hw_t *hw)
{
    return hw->output_offset.out_line_offset;
}

static inline void dma2d_ll_set_output_offset_out_line_offset(dma2d_hw_t *hw, uint32_t value)
{
    hw->output_offset.out_line_offset = value;
}

/* REG_0x11 */

static inline uint32_t dma2d_ll_get_dma2d_number_of_line_value(dma2d_hw_t *hw)
{
    return hw->dma2d_number_of_line.v;
}

static inline void dma2d_ll_set_dma2d_number_of_line_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_number_of_line.v = value;
}

/* REG_0x11:dma2d_number_of_line->number_line:X PIXEL.,RW,0x11[15:  0]*/
static inline uint32_t dma2d_ll_get_dma2d_number_of_line_number_line(dma2d_hw_t *hw)
{
    return hw->dma2d_number_of_line.number_line;
}

static inline void dma2d_ll_set_dma2d_number_of_line_number_line(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_number_of_line.number_line = value;
}

/* REG_0x11:dma2d_number_of_line->pixel_line:Y_PIXEL.,RW,0x11[29:16]*/
static inline uint32_t dma2d_ll_get_dma2d_number_of_line_pixel_line(dma2d_hw_t *hw)
{
    return hw->dma2d_number_of_line.pixel_line;
}

static inline void dma2d_ll_set_dma2d_number_of_line_pixel_line(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_number_of_line.pixel_line = value;
}

/* REG_0x12 */

static inline uint32_t dma2d_ll_get_dma2d_line_watermark_value(dma2d_hw_t *hw)
{
    return hw->dma2d_line_watermark.v;
}

static inline void dma2d_ll_set_dma2d_line_watermark_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_line_watermark.v = value;
}

/* REG_0x12:dma2d_line_watermark->line_watermark:config the line watermark int generation, transfer reach the watermark, int hold on.,RW,0x12[15:  0]*/
static inline uint32_t dma2d_ll_get_dma2d_line_watermark_line_watermark(dma2d_hw_t *hw)
{
    return hw->dma2d_line_watermark.line_watermark;
}

static inline void dma2d_ll_set_dma2d_line_watermark_line_watermark(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_line_watermark.line_watermark = value;
}

/* REG_0x13 */

static inline uint32_t dma2d_ll_get_dma2d_master_time_config_value(dma2d_hw_t *hw)
{
    return hw->dma2d_master_time_config.v;
}

static inline void dma2d_ll_set_dma2d_master_time_config_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_master_time_config.v = value;
}

/* REG_0x13:dma2d_master_time_config->master_time_ena:enable dead time function.,RW,0x18[       0]*/
static inline uint32_t dma2d_ll_get_dma2d_master_time_config_master_time_ena(dma2d_hw_t *hw)
{
    return hw->dma2d_master_time_config.master_time_en;
}

static inline void dma2d_ll_set_dma2d_master_time_config_master_time_ena(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_master_time_config.master_time_en = value;
}

/* REG_0x18:dma2d_master_time_config->dead_time:dead time value in ahb clock cycle inserted between two consecutive accesses on ahb master.,RW,0x18[15:  8]*/
static inline uint32_t dma2d_ll_get_dma2d_master_time_config_dead_time(dma2d_hw_t *hw)
{
    return hw->dma2d_master_time_config.dead_time;
}

static inline void dma2d_ll_set_dma2d_master_time_config_dead_time(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_master_time_config.dead_time = value;
}

/* REG_0x100 */

static inline uint32_t dma2d_ll_get_dma2d_fg_clut0_value(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_clut0.v;
}

static inline void dma2d_ll_set_dma2d_fg_clut0_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_clut0.v = value;
}

/* REG_0x100:dma2d_fg_clut0->blue:blue value foe index<y>for the fg.,RW,0x100[  7:  0]*/
static inline uint32_t dma2d_ll_get_dma2d_fg_clut0_blue(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_clut0.blue;
}

static inline void dma2d_ll_set_dma2d_fg_clut0_blue(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_clut0.blue = value;
}

/* REG_0x100:dma2d_fg_clut0->green:green value foe index<y>for the fg.,RW,0x100[15:  8]*/
static inline uint32_t dma2d_ll_get_dma2d_fg_clut0_green(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_clut0.green;
}

static inline void dma2d_ll_set_dma2d_fg_clut0_green(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_clut0.green = value;
}

/* REG_0x100:dma2d_fg_clut0->red:red value foe index<y>for the fg.,RW,0x100[23:16]*/
static inline uint32_t dma2d_ll_get_dma2d_fg_clut0_red(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_clut0.red;
}

static inline void dma2d_ll_set_dma2d_fg_clut0_red(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_clut0.red = value;
}

/* REG_0x100:dma2d_fg_clut0->alpha:alpha value foe index<y>for the fg.,RW,0x100[31:24]*/
static inline uint32_t dma2d_ll_get_dma2d_fg_clut0_alpha(dma2d_hw_t *hw)
{
    return hw->dma2d_fg_clut0.alpha;
}

static inline void dma2d_ll_set_dma2d_fg_clut0_alpha(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_fg_clut0.alpha = value;
}

/* REG_0x200 */
static inline uint32_t dma2d_ll_get_dma2d_bg_clut0_value(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_clut0.v;
}

static inline void dma2d_ll_set_dma2d_bg_clut0_value(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_clut0.v = value;
}

/* REG_0x200:dma2d_bg_clut0->blue:blue value foe index<y>for the bg.,RW,0x200[  7:  0]*/
static inline uint32_t dma2d_ll_get_dma2d_bg_clut0_blue(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_clut0.blue;
}

static inline void dma2d_ll_set_dma2d_bg_clut0_blue(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_clut0.blue = value;
}

/* REG_0x200:dma2d_bg_clut0->green:green value foe index<y>for the bg.,RW,0x200[15:  8]*/
static inline uint32_t dma2d_ll_get_dma2d_bg_clut0_green(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_clut0.green;
}

static inline void dma2d_ll_set_dma2d_bg_clut0_green(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_clut0.green = value;
}

/* REG_0x200:dma2d_bg_clut0->red:red value foe index<y>for the bg.,RW,0x200[23:16]*/
static inline uint32_t dma2d_ll_get_dma2d_bg_clut0_red(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_clut0.red;
}

static inline void dma2d_ll_set_dma2d_bg_clut0_red(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_clut0.red = value;
}

/* REG_0x200:dma2d_bg_clut0->alpha:alpha value foe index<y>for the bg.,RW,0x200[31:24]*/
static inline uint32_t dma2d_ll_get_dma2d_bg_clut0_alpha(dma2d_hw_t *hw)
{
    return hw->dma2d_bg_clut0.alpha;
}

static inline void dma2d_ll_set_dma2d_bg_clut0_alpha(dma2d_hw_t *hw, uint32_t value)
{
    hw->dma2d_bg_clut0.alpha = value;
}

#ifdef __cplusplus 
}
#endif
