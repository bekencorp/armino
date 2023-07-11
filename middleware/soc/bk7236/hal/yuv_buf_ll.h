// Copyright 2022-2023 Beken
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
#include "hal_port.h"
#include "yuv_buf_hw.h"
#include <driver/hal/hal_yuv_buf_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define YUV_BUF_LL_REG_BASE    (SOC_YUV_BUF_REG_BASE)

static inline void yuv_buf_ll_soft_reset(yuv_buf_hw_t *hw)
{
	hw->global_ctrl.soft_reset = 0;
	hw->global_ctrl.soft_reset = 1;
}

static inline void yuv_buf_ll_init(yuv_buf_hw_t *hw)
{
	hw->global_ctrl.soft_reset = 1;
	hw->global_ctrl.clk_gate_bypass = 1;
}

static inline void yuv_buf_ll_set_mclk_div(yuv_buf_hw_t *hw, yuv_buf_mclk_div_t mclk_div)
{
	hw->ctrl.mclk_div = mclk_div & YUV_BUF_F_MCLK_DIV_M;
}

static inline void yuv_buf_ll_enable_yuv_buf_mode(yuv_buf_hw_t *hw)
{
	hw->ctrl.yuv_mode = 1;
}

static inline void yuv_buf_ll_disable_yuv_buf_mode(yuv_buf_hw_t *hw)
{
	hw->ctrl.yuv_mode = 0;
}

static inline void yuv_buf_ll_enable_h264_encode_mode(yuv_buf_hw_t *hw)
{
	hw->ctrl.h264_mode = 1;
}

static inline void yuv_buf_ll_disable_h264_encode_mode(yuv_buf_hw_t *hw)
{
	hw->ctrl.h264_mode = 0;
}

static inline void yuv_buf_ll_enable_mclk_hold_on(yuv_buf_hw_t *hw)
{
	hw->ctrl.mclk_hold_en = 1;
}

static inline void yuv_buf_ll_set_em_base_addr(yuv_buf_hw_t *hw, uint32_t em_base_addr)
{
	hw->em_base_addr = em_base_addr;
}

static inline void yuv_buf_ll_set_encode_begin_hsync_posedge(yuv_buf_hw_t *hw)
{
	hw->ctrl.soi_hsync = 1;
}

static inline void yuv_buf_ll_set_encode_begin_hsync_negedge(yuv_buf_hw_t *hw)
{
	hw->ctrl.soi_hsync = 0;
}

static inline void yuv_buf_ll_enable_reverse_yuv_data(yuv_buf_hw_t *hw)
{
	hw->ctrl.bus_dat_byte_reve = 1;
}

static inline void yuv_buf_ll_disable_reverse_yuv_data(yuv_buf_hw_t *hw)
{
	hw->ctrl.bus_dat_byte_reve = 0;
}

static inline void yuv_buf_ll_enable_reverse_jpeg_word(yuv_buf_hw_t *hw)
{
	hw->ctrl.jpeg_word_reverse = 1;
}

static inline void yuv_buf_ll_enable_bps_cis(yuv_buf_hw_t *hw)
{
	hw->ctrl.bps_cis = 1;
}

static inline void yuv_buf_ll_disable_bps_cis(yuv_buf_hw_t *hw)
{
	hw->ctrl.bps_cis = 0;
}

static inline void yuv_buf_ll_enable_memrev(yuv_buf_hw_t *hw)
{
	hw->ctrl.memrev = 1;
}

static inline void yuv_buf_ll_disable_memrev(yuv_buf_hw_t *hw)
{
	hw->ctrl.memrev = 0;
}

static inline void yuv_buf_ll_set_yuv_format(yuv_buf_hw_t *hw, yuv_format_t yuv_format)
{
	hw->ctrl.yuv_fmt_sel = yuv_format;
}

static inline void yuv_buf_ll_enable_partial_display(yuv_buf_hw_t *hw)
{
	hw->ctrl.partial_display_en = 1;
}

static inline void yuv_buf_ll_disable_partial_display(yuv_buf_hw_t *hw)
{
	hw->ctrl.partial_display_en = 0;
}

static inline void yuv_buf_ll_enable_sync_edge_dect(yuv_buf_hw_t *hw)
{
	hw->ctrl.sync_edge_dect_en = 1;
}

static inline void yuv_buf_ll_disable_sync_edge_dect(yuv_buf_hw_t *hw)
{
	hw->ctrl.sync_edge_dect_en = 0;
}

static inline void yuv_buf_ll_set_x_pixel(yuv_buf_hw_t *hw, uint32_t x_pixel)
{
	hw->pixel.x_pixel = x_pixel;
}

static inline void yuv_buf_ll_set_y_pixel(yuv_buf_hw_t *hw, uint32_t y_pixel)
{
	hw->pixel.y_pixel = y_pixel;
}

static inline void yuv_buf_ll_set_frame_blk(yuv_buf_hw_t *hw, uint32_t frame_blk)
{
	hw->pixel.frame_blk = frame_blk;
}

static inline void yuv_buf_ll_set_x_partial_offset_l(yuv_buf_hw_t *hw, uint32_t offset)
{
	hw->x_partial_offset.x_partial_offset_l = offset;
}

static inline void yuv_buf_ll_set_x_partial_offset_r(yuv_buf_hw_t *hw, uint32_t offset)
{
	hw->x_partial_offset.x_partial_offset_r = offset;
}

static inline void yuv_buf_ll_set_y_partial_offset_l(yuv_buf_hw_t *hw, uint32_t offset)
{
	hw->y_partial_offset.y_partial_offset_l = offset;
}

static inline void yuv_buf_ll_set_y_partial_offset_r(yuv_buf_hw_t *hw, uint32_t offset)
{
	hw->y_partial_offset.y_partial_offset_r = offset;
}

static inline void yuv_buf_ll_enable_int(yuv_buf_hw_t *hw)
{
	hw->int_en.v = 0x1FF;
}

static inline void yuv_buf_ll_disable_int(yuv_buf_hw_t *hw)
{
	hw->int_en.v = 0x0;
}

static inline void yuv_buf_ll_enable_vsync_negedge_int(yuv_buf_hw_t *hw)
{
	hw->int_en.vsync_nege_int_en = 1;
}

static inline void yuv_buf_ll_enable_yuv_arv_int(yuv_buf_hw_t *hw)
{
	hw->int_en.yuv_arv_int_en = 1;
}

static inline void yuv_buf_ll_send_data_to_h264(yuv_buf_hw_t *hw)
{
	hw->renc_start.renc_start = 1;
}

static inline uint32_t yuv_buf_ll_get_interrupt_status(yuv_buf_hw_t *hw)
{
	return hw->int_status.v;
}

static inline void yuv_buf_ll_clear_interrupt_status(yuv_buf_hw_t *hw, uint32_t int_status)
{
	REG_WRITE(YUV_BUF_R_INT_STATUS, int_status);
}

static inline bool yuv_buf_ll_is_vsync_negedge_int_triggered(yuv_buf_hw_t *hw, uint32_t int_status)
{
	return (int_status & BIT(0));
}

static inline bool yuv_buf_ll_is_yuv_arv_int_triggered(yuv_buf_hw_t *hw, uint32_t int_status)
{
	return (int_status & BIT(1));
}

static inline bool yuv_buf_ll_is_sm0_wr_int_triggered(yuv_buf_hw_t *hw, uint32_t int_status)
{
	return (int_status & BIT(2));
}

static inline bool yuv_buf_ll_is_sm1_wr_int_triggered(yuv_buf_hw_t *hw, uint32_t int_status)
{
	return (int_status & BIT(3));
}

static inline bool yuv_buf_ll_is_fifo_full_int_triggered(yuv_buf_hw_t *hw, uint32_t int_status)
{
	return (int_status & BIT(4));
}

static inline bool yuv_buf_ll_is_enc_line_done_int_triggered(yuv_buf_hw_t *hw, uint32_t int_status)
{
	return (int_status & BIT(5));
}

static inline bool yuv_buf_ll_is_sensor_resolution_err_int_triggered(yuv_buf_hw_t *hw, uint32_t int_status)
{
	return (int_status & BIT(6));
}

static inline bool yuv_buf_ll_is_h264_err_int_triggered(yuv_buf_hw_t *hw, uint32_t int_status)
{
	return (int_status & BIT(7));
}

static inline bool yuv_buf_ll_is_enc_slow_int_triggered(yuv_buf_hw_t *hw, uint32_t int_status)
{
	return (int_status & BIT(8));
}

static inline void yuv_buf_ll_reset_config_to_default(yuv_buf_hw_t *hw)
{
	hw->global_ctrl.soft_reset = 1;
}

#ifdef __cplusplus
}
#endif

