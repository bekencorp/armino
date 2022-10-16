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

#include <soc/soc.h>
#include <soc/soc_port.h>
#include "hal_config.h"
#include "dma2d_struct.h"
#include "dma2d_ll_macro_def.h"

#if CFG_HAL_DEBUG_DMA2D

void dma2d_struct_dump()
{
	dma2d_hw_t *hw = (dma2d_hw_t *)DMA2D_LL_REG_BASE;
	SOC_LOGI("    dma2d_reg_base=0x%x\r\n", (uint32_t)hw);

	SOC_LOGI("\n");
	SOC_LOGI("    device_id addr=0x%x, value=0x%x\n", &hw->device_id, hw->device_id);

	SOC_LOGI("\n");
	SOC_LOGI("    version_id addr=0x%x, value=0x%x\n", &hw->version_id, hw->version_id);

	SOC_LOGI("\n");
	SOC_LOGI("    module_control addr=0x%x, value=0x%x\n", &hw->module_control, hw->module_control.v);
	SOC_LOGI("    soft_reset:    %x\n", hw->module_control.soft_reset);
	SOC_LOGI("    clk_gate:    %x\n", hw->module_control.clk_gate);

	SOC_LOGI("\n");
	SOC_LOGI("    global_status addr=0x%x, value=0x%x\n", &hw->global_status, hw->global_status);

	SOC_LOGI("\n");
	SOC_LOGI("    dma2d_control_reg addr=0x%x, value=0x%x\n", &hw->dma2d_control_reg, hw->dma2d_control_reg.v);
	SOC_LOGI("    tran_start:    %x\n", hw->dma2d_control_reg.tran_start);
	SOC_LOGI("    tran_suspend:    %x\n", hw->dma2d_control_reg.tran_suspend);
	SOC_LOGI("    tran_abort:    %x\n", hw->dma2d_control_reg.tran_abort);
	SOC_LOGI("    line_offset_mode:    %x\n", hw->dma2d_control_reg.line_offset_mode);
	SOC_LOGI("    error_int_en:    %x\n", hw->dma2d_control_reg.error_int_en);
	SOC_LOGI("    complete_int_en:    %x\n", hw->dma2d_control_reg.complete_int_en);
	SOC_LOGI("    waterm_int_en:    %x\n", hw->dma2d_control_reg.waterm_int_en);
	SOC_LOGI("    clut_error_int_en:    %x\n", hw->dma2d_control_reg.clut_error_int_en);
	SOC_LOGI("    clut_cmplt_int_en:    %x\n", hw->dma2d_control_reg.clut_cmplt_int_en);
	SOC_LOGI("    config_error_int_en:    %x\n", hw->dma2d_control_reg.config_error_int_en);
	SOC_LOGI("    mode:    %x\n", hw->dma2d_control_reg.mode);
	SOC_LOGI("    master_tran_length:    %x\n", hw->dma2d_control_reg.master_tran_length);
	SOC_LOGI("    out_byte_revese:    %x\n", hw->dma2d_control_reg.out_byte_revese);

	SOC_LOGI("\n");
	SOC_LOGI("    dma2d_int_status addr=0x%x, value=0x%x\n", &hw->dma2d_int_status, hw->dma2d_int_status.v);
	SOC_LOGI("    error_int:    %x\n", hw->dma2d_int_status.error_int);
	SOC_LOGI("    complete_int:    %x\n", hw->dma2d_int_status.complete_int);
	SOC_LOGI("    waterm_int:    %x\n", hw->dma2d_int_status.waterm_int);
	SOC_LOGI("    clut_error_int:    %x\n", hw->dma2d_int_status.clut_error_int);
	SOC_LOGI("    clut_cmplt_int:    %x\n", hw->dma2d_int_status.clut_cmplt_int);
	SOC_LOGI("    config_error_int:    %x\n", hw->dma2d_int_status.config_error_int);

	SOC_LOGI("\n");
	SOC_LOGI("    dma2d_int_clear addr=0x%x, value=0x%x\n", &hw->dma2d_int_clear, hw->dma2d_int_clear.v);
	SOC_LOGI("    clr_error_int:    %x\n", hw->dma2d_int_clear.clr_error_int);
	SOC_LOGI("    clr_complete_int:    %x\n", hw->dma2d_int_clear.clr_complete_int);
	SOC_LOGI("    clr_waterm_int:    %x\n", hw->dma2d_int_clear.clr_waterm_int);
	SOC_LOGI("    clr_clut_error_int:    %x\n", hw->dma2d_int_clear.clr_clut_error_int);
	SOC_LOGI("    clr_clut_cmplt_int:    %x\n", hw->dma2d_int_clear.clr_clut_cmplt_int);
	SOC_LOGI("    clr_config_error_int:    %x\n", hw->dma2d_int_clear.clr_config_error_int);

	SOC_LOGI("\n");
	SOC_LOGI("    dma2d_fg_address addr=0x%x, value=0x%x\n", &hw->dma2d_fg_address, hw->dma2d_fg_address.v);

	SOC_LOGI("\n");
	SOC_LOGI("    dma2d_fg_offset addr=0x%x, value=0x%x\n", &hw->dma2d_fg_offset, hw->dma2d_fg_offset.v);
	SOC_LOGI("    fg_line_offset:	%x\n", hw->dma2d_fg_offset.fg_line_offset);

	SOC_LOGI("\n");
	SOC_LOGI("    dma2d_bg_address addr=0x%x, value=0x%x\n", &hw->dma2d_bg_address, hw->dma2d_bg_address.v);

	SOC_LOGI("\n");
	SOC_LOGI("    dma2d_bg_offset addr=0x%x, value=0x%x\n", &hw->dma2d_bg_offset, hw->dma2d_bg_offset.v);
	SOC_LOGI("    bg_line_offset:	%x\n", hw->dma2d_bg_offset.bg_line_offset);

	SOC_LOGI("\n");
	SOC_LOGI("    dma2d_fg_pfc_ctrl addr=0x%x, value=0x%x\n", &hw->dma2d_fg_pfc_ctrl, hw->dma2d_fg_pfc_ctrl.v);
	SOC_LOGI("    fg_color_mode:	%x\n", hw->dma2d_fg_pfc_ctrl.fg_color_mode);
	SOC_LOGI("    fg_clut_color_mode:	%x\n", hw->dma2d_fg_pfc_ctrl.fg_clut_color_mode);
	SOC_LOGI("    fg_start_clut:	%x\n", hw->dma2d_fg_pfc_ctrl.fg_start_clut);
	SOC_LOGI("    fg_clut_size:	%x\n", hw->dma2d_fg_pfc_ctrl.fg_clut_size);
	SOC_LOGI("    fg_alpha_mode:	%x\n", hw->dma2d_fg_pfc_ctrl.fg_alpha_mode);
	SOC_LOGI("    alpha_invert:	%x\n", hw->dma2d_fg_pfc_ctrl.alpha_invert);
	SOC_LOGI("    fg_rb_swap:	%x\n", hw->dma2d_fg_pfc_ctrl.fg_rb_swap);
	SOC_LOGI("    fg_alpha:	%x\n", hw->dma2d_fg_pfc_ctrl.fg_alpha);

	SOC_LOGI("\n");
	SOC_LOGI("    dma2d_fg_color_reg addr=0x%x, value=0x%x\n", &hw->dma2d_fg_color_reg, hw->dma2d_fg_color_reg.v);
	SOC_LOGI("    blue_value:	%x\n", hw->dma2d_fg_color_reg.blue_value);
	SOC_LOGI("    green_value:	%x\n", hw->dma2d_fg_color_reg.green_value);
	SOC_LOGI("    red_value:	%x\n", hw->dma2d_fg_color_reg.red_value);

	SOC_LOGI("\n");
	SOC_LOGI("    dma2d_bg_pfc_ctrl addr=0x%x, value=0x%x\n", &hw->dma2d_bg_pfc_ctrl, hw->dma2d_bg_pfc_ctrl.v);
	SOC_LOGI("    bg_color_mode:	%x\n", hw->dma2d_bg_pfc_ctrl.bg_color_mode);
	SOC_LOGI("    bg_clut_color_mode:	%x\n", hw->dma2d_bg_pfc_ctrl.bg_clut_color_mode);
	SOC_LOGI("    bg_start_clut:	%x\n", hw->dma2d_bg_pfc_ctrl.bg_start_clut);
	SOC_LOGI("    bg_clut_size:	%x\n", hw->dma2d_bg_pfc_ctrl.bg_clut_size);
	SOC_LOGI("    bg_alpha_mode:	%x\n", hw->dma2d_bg_pfc_ctrl.bg_alpha_mode);
	SOC_LOGI("    alpha_invert:	%x\n", hw->dma2d_bg_pfc_ctrl.alpha_invert);
	SOC_LOGI("    bg_rb_swap:	%x\n", hw->dma2d_bg_pfc_ctrl.bg_rb_swap);
	SOC_LOGI("    bg_alpha:	%x\n", hw->dma2d_bg_pfc_ctrl.bg_alpha);

	SOC_LOGI("\n");
	SOC_LOGI("    dma2d_bg_color_reg addr=0x%x, value=0x%x\n", &hw->dma2d_bg_color_reg, hw->dma2d_bg_color_reg.v);
	SOC_LOGI("    blue_value:	%x\n", hw->dma2d_bg_color_reg.blue_value);
	SOC_LOGI("    green_value:	%x\n", hw->dma2d_bg_color_reg.green_value);
	SOC_LOGI("    red_value:	%x\n", hw->dma2d_bg_color_reg.red_value);

	SOC_LOGI("\n");
	SOC_LOGI("    fg_clut_mem_address addr=0x%x, value=0x%x\n", &hw->fg_clut_mem_address, hw->fg_clut_mem_address.v);

	SOC_LOGI("\n");
	SOC_LOGI("    bg_clut_mem_address addr=0x%x, value=0x%x\n", &hw->bg_clut_mem_address, hw->bg_clut_mem_address.v);

	SOC_LOGI("\n");
	SOC_LOGI("    out_pfc_contrl addr=0x%x, value=0x%x\n", &hw->out_pfc_contrl, hw->out_pfc_contrl.v);
	SOC_LOGI("    out_color_mode:	%x\n", hw->out_pfc_contrl.out_color_mode);
	SOC_LOGI("    out_alpha_invert:	%x\n", hw->out_pfc_contrl.out_alpha_invert);
	SOC_LOGI("    out_rb_swap:	%x\n", hw->out_pfc_contrl.out_rb_swap);

	SOC_LOGI("\n");
	SOC_LOGI("    out_color_reg addr=0x%x, value=0x%x\n", &hw->out_color_reg, hw->out_color_reg.v);

	SOC_LOGI("\n");
	SOC_LOGI("    dma2d_out_mem_address addr=0x%x, value=0x%x\n", &hw->dma2d_out_mem_address, hw->dma2d_out_mem_address.v);

	SOC_LOGI("\n");
	SOC_LOGI("    output_offset addr=0x%x, value=0x%x\n", &hw->output_offset, hw->output_offset.v);
	SOC_LOGI("    out_line_offset:	%x\n", hw->output_offset.out_line_offset);

	SOC_LOGI("\n");
	SOC_LOGI("    dma2d_number_of_line addr=0x%x, value=0x%x\n", &hw->dma2d_number_of_line, hw->dma2d_number_of_line.v);
	SOC_LOGI("    number_line:	%x\n", hw->dma2d_number_of_line.number_line);
	SOC_LOGI("    pixel_line:	%x\n", hw->dma2d_number_of_line.pixel_line);

	SOC_LOGI("\n");
	SOC_LOGI("    dma2d_line_watermark addr=0x%x, value=0x%x\n", &hw->dma2d_line_watermark, hw->dma2d_line_watermark.v);
	SOC_LOGI("    line_watermark:	%x\n", hw->dma2d_line_watermark.line_watermark);

}

#endif

