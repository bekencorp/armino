// Copyright 2022-2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//	 http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "h264_hw.h"
#include <driver/hal/hal_h264_types.h>
#include <driver/hal/hal_gpio_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define H264_LL_REG_BASE(_h264_unit_id) (H264_R_BASE)

#define H264_GPIO_PIN_NUMBER	12
#define H264_GPIO_MAP \
{\
	{GPIO_27, GPIO_DEV_JPEG_MCLK},\
	{GPIO_29, GPIO_DEV_JPEG_PCLK},\
	{GPIO_32, GPIO_DEV_JPEG_PXDATA0},\
	{GPIO_33, GPIO_DEV_JPEG_PXDATA1},\
	{GPIO_34, GPIO_DEV_JPEG_PXDATA2},\
	{GPIO_35, GPIO_DEV_JPEG_PXDATA3},\
	{GPIO_36, GPIO_DEV_JPEG_PXDATA4},\
	{GPIO_37, GPIO_DEV_JPEG_PXDATA5},\
	{GPIO_38, GPIO_DEV_JPEG_PXDATA6},\
	{GPIO_39, GPIO_DEV_JPEG_PXDATA7},\
	{GPIO_30, GPIO_DEV_JPEG_HSYNC},\
	{GPIO_31, GPIO_DEV_JPEG_VSYNC},\
}

extern void delay(int num);

static inline void h264_ll_init(h264_hw_t *hw)
{
	hw->global_ctrl.soft_reset = 1;
	delay(1);
	hw->global_ctrl.clkg_bypass = 1;
}

static inline void h264_ll_reset(h264_hw_t *hw)
{
	 hw->global_ctrl.soft_reset = 0;
}

static inline void h264_ll_encode_enable(h264_hw_t *hw)
{
	hw->enc_ctrl.enc_en = 1;
}

static inline void h264_ll_encode_disable(h264_hw_t *hw)
{
	hw->enc_ctrl.enc_en = 0;
}

static inline void h264_ll_set_img_width(h264_hw_t *hw, uint32_t img_width)
{
	hw->img_width = img_width;
}

static inline void h264_ll_set_img_height(h264_hw_t *hw, uint32_t img_height)
{
	hw->img_height = img_height;
}

static inline void h264_ll_set_qp(h264_hw_t *hw, uint32_t param)
{
	hw->qp = param;
}

static inline void h264_ll_set_cqp_offset(h264_hw_t *hw, uint32_t param)
{
	hw->cqp_offset = param;
}

static inline void h264_ll_set_qp_target(h264_hw_t *hw, uint32_t param)
{
	hw->qp_target = param;
}

static inline void h264_ll_set_mb_lines(h264_hw_t *hw, uint32_t param)
{
	hw->mb_lines = param;
}

static inline void h264_ll_set_num_slices(h264_hw_t *hw, uint32_t param)
{
	hw->num_slices = param;
}

static inline void h264_ll_set_num_pframe(h264_hw_t *hw, uint32_t param)
{
	hw->gop_header.num_pframes = param;
}

static inline void h264_ll_set_skip_frame_mode(h264_hw_t *hw, uint32_t mode)
{
	hw->gop_header.disable_skip_frame = mode;
}

static inline void h264_ll_set_iframe_I_gain(h264_hw_t *hw, uint32_t value)
{
	hw->iframe_IP_gain.iframe_I_gain = value;
}

static inline void h264_ll_set_iframe_P_gain(h264_hw_t *hw, uint32_t value)
{
	hw->iframe_IP_gain.iframe_P_gain = value;
}

static inline void h264_ll_set_iframe_D_gain(h264_hw_t *hw, uint32_t value)
{
	hw->iframe_D_gain = value;
}

static inline void h264_ll_set_iframe_min_qp(h264_hw_t *hw, uint32_t param)
{
	hw->iframe_qp_boundary.iframe_min_qp = param;
}

static inline void h264_ll_set_iframe_max_qp(h264_hw_t *hw, uint32_t param)
{
	hw->iframe_qp_boundary.iframe_max_qp = param;
}

static inline void h264_ll_set_profile_idc(h264_hw_t *hw, uint32_t param)
{
	hw->profile_idc = param;
}

static inline void h264_ll_set_level_idc(h264_hw_t *hw, uint32_t param)
{
	hw->level_idc = param;
}

static inline void h264_ll_set_filter_mode(h264_hw_t *hw, uint32_t mode)
{
	hw->filter_ctrl.filter_disable = mode;
}

static inline void h264_ll_set_alpha_off(h264_hw_t *hw, uint32_t param)
{
	hw->filter_ctrl.alpha_off = param;
}

static inline void h264_ll_set_beta_off(h264_hw_t *hw, uint32_t param)
{
	hw->filter_ctrl.beta_off = param;
}

static inline void h264_ll_set_num_imb_bits(h264_hw_t *hw, uint32_t value)
{
	hw->iframe_bit_ctrl.num_imb_bits = value;
}

static inline void h264_ll_set_nal_align_mode(h264_hw_t *hw, uint32_t mode)
{
	hw->iframe_bit_ctrl.nal_align = mode;
}

static inline void h264_ll_set_cons_ipred_mode(h264_hw_t *hw, uint32_t mode)
{
	hw->iframe_bit_ctrl.cons_ipred = mode;
}

static inline void h264_ll_set_num_pmb_bits(h264_hw_t *hw, uint32_t value)
{
	hw->num_pmb_bits = value;
}

static inline void h264_ll_set_pframe_I_gain(h264_hw_t *hw, uint32_t value)
{
	hw->pframe_IP_gain.pframe_I_gain = value;
}

static inline void h264_ll_set_pframe_P_gain(h264_hw_t *hw, uint32_t value)
{
	hw->pframe_IP_gain.pframe_P_gain = value;
}

static inline void h264_ll_set_pframe_D_gain(h264_hw_t *hw, uint32_t value)
{
	hw->pframe_D_gain = value;
}

static inline void h264_ll_set_pframe_min_qp(h264_hw_t *hw, uint32_t param)
{
	hw->pframe_qp_boudary.pframe_min_qp = param;
}

static inline void h264_ll_set_pframe_max_qp(h264_hw_t *hw, uint32_t param)
{
	hw->pframe_qp_boudary.pframe_max_qp = param;
}

static inline void h264_ll_set_Pbits_factor1(h264_hw_t *hw,uint32_t param)
{
	hw->pbits_fact_1to3.pbits_fact1=param;
}

static inline void h264_ll_set_Pbits_factor2(h264_hw_t *hw,uint32_t param)
{
	hw->pbits_fact_1to3.pbits_fact2=param;
}

static inline void h264_ll_set_Pbits_factor3(h264_hw_t *hw,uint32_t param)
{
	hw->pbits_fact_1to3.pbits_fact3=param;
}

static inline void h264_ll_set_Pbits_factor4(h264_hw_t *hw,uint32_t param)
{
	hw->pbits_fact_4to5.pbits_fact4=param;
}

static inline void h264_ll_set_Pbits_factor5(h264_hw_t *hw,uint32_t param)
{
	hw->pbits_fact_4to5.pbits_fact5=param;
}

static inline void h264_ll_set_num_ref_bits(h264_hw_t *hw, uint32_t param)
{
	hw->num_ref_bits = param;
}

static inline void h264_ll_set_reset_i_state_mode(h264_hw_t *hw, uint32_t mode)
{
	hw->reset_i_state = mode;
}

static inline void h264_ll_set_vui_time_scale_L(h264_hw_t *hw, uint32_t param)
{
	hw->vui_time_scale_L = param;
}

static inline void h264_ll_set_vui_time_scale_H(h264_hw_t *hw, uint32_t param)
{
	hw->vui_time_scale_H = param;
}

static inline void h264_ll_set_vui_num_u_tick_L(h264_hw_t *hw, uint32_t param)
{
	hw->vui_num_u_tick_L = param;
}

static inline void h264_ll_set_vui_num_u_tick_H(h264_hw_t *hw, uint32_t param)
{
	hw->vui_num_u_tick_H = param;
}

static inline void h264_ll_set_bitrate_scale(h264_hw_t *hw, uint32_t param)
{
	hw->scale.bitrate_scale = param;
}

static inline void h264_ll_set_cpb_size_scale(h264_hw_t *hw, uint32_t param)
{
	hw->scale.cpb_size_scale = param;
}

static inline void h264_ll_set_bitrate_value_less1(h264_hw_t *hw, uint32_t param)
{
	hw->bitrate_value_less1 = param;
}

static inline void h264_ll_set_cpb_size_value_less1(h264_hw_t *hw, uint32_t param)
{
	hw->cpb_size_value_less1 = param;
}

static inline void h264_ll_set_init_cpb_removal_delay(h264_hw_t *hw, uint32_t param)
{
	hw->init_cpb_removal_delay = param;
}

static inline void h264_ll_set_init_enc_buffer_level_L(h264_hw_t *hw, uint32_t param)
{
	hw->init_enc_buffer_level_L = param;
}

static inline void h264_ll_set_init_enc_buffer_level_H(h264_hw_t *hw, uint32_t param)
{
	hw->init_enc_buffer_level_H = param;
}

static inline void h264_ll_set_bits_per_frame_int_L(h264_hw_t *hw, uint32_t param)
{
	hw->bits_per_frame_int_L = param;
}

static inline void h264_ll_set_bits_per_frame_int_H(h264_hw_t *hw, uint32_t param)
{
	hw->bits_per_frame_int_H = param;
}

static inline void h264_ll_set_bits_per_frame_rem_L(h264_hw_t *hw, uint32_t param)
{
	hw->bits_per_frame_rem_L = param;
}

static inline void h264_ll_set_bits_per_frame_rem_H(h264_hw_t *hw, uint32_t param)
{
	hw->bits_per_frame_rem_H = param;
}

static inline void h264_ll_set_cpb_size_L(h264_hw_t *hw, uint32_t param)
{
	hw->cpb_size_L = param;
}

static inline void h264_ll_set_cpb_size_H(h264_hw_t *hw, uint32_t param)
{
	hw->cpb_size_H = param;
}

static inline void h264_ll_set_soft_reset_mode(h264_hw_t *hw, uint32_t mode)
{
	hw->soft_reset = mode;
}

static inline void h264_ll_set_rsvd1(h264_hw_t *hw, uint32_t value)
{
	hw->rsvd1 = value;
}

static inline void h264_ll_set_rsvd2(h264_hw_t *hw, uint32_t value)
{
	hw->rsvd2 = value;
}

static inline void h264_ll_set_int_stat(h264_hw_t *hw, uint32_t mode)
{
	REG_WRITE(H264_R_INT_STATUS, mode);
}

static inline void h264_ll_set_int_mode(h264_hw_t *hw, uint32_t mode)
{
	hw->int_en = mode;
}

static inline void h264_ll_set_r_h264_clk_en_mode(h264_hw_t *hw, uint32_t value)
{
	hw->r_h264_clk_en = value;
}

/* read reg */

static inline uint32_t h264_ll_get_devstatus(h264_hw_t *hw)
{
	return hw->devstatus;
}

static inline uint32_t h264_ll_get_int_stat(h264_hw_t *hw)
{
	return hw->int_stat.v;
}

static inline uint32_t h264_ll_devid(h264_hw_t *hw)
{
	return hw->devid;
}

static inline uint32_t h264_ll_verid(h264_hw_t *hw)
{
	return hw->verid;
}

static inline uint32_t h264_ll_get_encode_count(h264_hw_t *hw)
{
	return hw->encoder_cnt;
}

#ifdef __cplusplus
}
#endif