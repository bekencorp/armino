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

const h264_config_t h264_vga_480p =
{
	.num_pframe = 5,
	.skip_mode = 1,
	.qp = 0x19,
	.cqp_off = 0,
	.tqp = 0x19,

	.iframe_igain = 0x30,
	.iframe_pgain = 0x50,
	.iframe_dgain = 0xA,
	.iframe_min_qp = 0x33,
	.iframe_max_qp = 0x0,

	
	.pframe_igain = 0x1E,
	.pframe_pgain = 0x1E,
	.pframe_dgain = 0xA,
	.pframe_min_qp = 0x33,
	.pframe_max_qp = 0x0,
	.profile_idc = 0x0,
	.level_idc = 0x29,
	.filter_mode = 0x1,
	.imb_bits = 0x50,
	.pmb_bits = 0x23,
	.nal_align_mode = 0x0,
	.cons_ipred_mode = 0x0,
	.mb_cfs_bits = 0x1A4,
	.reset_i_state_mode = 0x1,

	.vui_time_scale_L = 0xEA60,
	.vui_time_scale_H = 0x0,
	.vui_num_u_tick_L = 0x3E8,
	.vui_num_u_tick_H = 0x0,
	.bitrate_scale = 0x4,
	.cpb_size_scale = 0x6,
	.bitrate_value_less1 = 0X4C4B,
	.cpb_size_value_less1 = 0XBFB,

	.init_cpb_removal_delay = 0X34B6,
	.init_enc_buffer_level_L = 0X2E02,
	.init_enc_buffer_level_H = 0X2,
	.bits_per_frame_int_L = 0X2C44,
	.bits_per_frame_int_H = 0XA,
	.bits_per_frame_rem_L = 0X3E80,
	.bits_per_frame_rem_H = 0X0,
	.cpb_size_L = 0XF000,
	.cpb_size_H = 0X2F,
};
