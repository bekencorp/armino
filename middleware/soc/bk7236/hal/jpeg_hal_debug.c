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

#include "hal_config.h"
#include "jpeg_hal.h"
#include "jpeg_hw.h"
#include "jpeg_ll.h"

#if CFG_HAL_DEBUG_JPEG

void jpeg_struct_dump(void)
{
	SOC_LOGI("system_reg8:%x\r\n", REG_READ(SOC_SYSTEM_REG_BASE + 0x8 * 4));
	SOC_LOGI("system_reg9:%x\r\n", REG_READ(SOC_SYSTEM_REG_BASE + 0x9* 4));
	SOC_LOGI("system_rega:%x\r\n", REG_READ(SOC_SYSTEM_REG_BASE + 0xa* 4));
	SOC_LOGI("system_clock_en_c:%x\r\n", REG_READ(SOC_SYSTEM_REG_BASE + 0xc * 4));
	SOC_LOGI("system_clock_en_d:%x\r\n", REG_READ(SOC_SYSTEM_REG_BASE + 0xd * 4));
	SOC_LOGI("cpu0_int_en_low:%x\r\n", REG_READ(SOC_SYSTEM_REG_BASE + 0x20 * 4));
	SOC_LOGI("cpu0_int_en_high:%x\r\n", REG_READ(SOC_SYSTEM_REG_BASE + 0x21 * 4));

	jpeg_hw_t *hw = (jpeg_hw_t *)JPEG_LL_REG_BASE(0);
	SOC_LOGI("base=%x\r\n", (uint32_t)hw);

	SOC_LOGI("  device_id=0x%x value=0x%x\r\n", &hw->dev_id, hw->dev_id);
	SOC_LOGI("  dev_version=0x%x value=0x%x\r\n", &hw->dev_version, hw->dev_version);

	SOC_LOGI("  global_ctrl=0x%x value=0x%x\r\n", &hw->global_ctrl, hw->global_ctrl.v);
	SOC_LOGI("    soft_reset:      %x\r\n", hw->global_ctrl.soft_reset);
	SOC_LOGI("    clk_gate_bypass: %x\r\n", hw->global_ctrl.clk_gate_bypass);
	SOC_LOGI("    reserved:        %x\r\n", hw->global_ctrl.reserved);
	SOC_LOGI("\r\n");

	SOC_LOGI("  dev_status=0x%x value=0x%x\r\n", &hw->dev_status, hw->dev_status);
	SOC_LOGI("\r\n");

	SOC_LOGI("  em_base_addr=0x%x value=0x%x\r\n", &hw->em_base_addr, hw->em_base_addr.v);
	SOC_LOGI("    em_base_addr: %x\r\n", hw->em_base_addr.em_base_addr);
	SOC_LOGI("    eof_offset:   %x\r\n", hw->em_base_addr.eof_offset);
	SOC_LOGI("\r\n");

	/* REG_0x5 */
	SOC_LOGI("  rx_fifo_data=0x%x value=0x%x\r\n", &hw->rx_fifo_data, hw->rx_fifo_data);

	/* REG_0x6 */
	SOC_LOGI("  int_status=0x%x value=0x%x\r\n", &hw->int_status, hw->int_status.v);
	SOC_LOGI("    int_status:     %x\r\n", hw->int_status.int_status);
	SOC_LOGI("    fifo_rd_finish: %x\r\n", hw->int_status.fifo_rd_finish);
	SOC_LOGI("    reserved:       %x\r\n", hw->int_status.reserved);
	SOC_LOGI("\r\n");

	/* REG_0x7 */
	SOC_LOGI("  byte_count_pfrm=0x%x value=0x%x\r\n", &hw->byte_count_pfrm, hw->byte_count_pfrm);
	SOC_LOGI("\r\n");

	/* REG_0x8 */
	SOC_LOGI("  x_partial=0x%x value=0x%x\r\n", &hw->x_partial, hw->x_partial.v);
	SOC_LOGI("    x_partial_offset_l: %x\r\n", hw->x_partial.x_partial_offset_l);
	SOC_LOGI("    reserved0:          %x\r\n", hw->x_partial.reserved0);
	SOC_LOGI("    x_partial_offset_r: %x\r\n", hw->x_partial.x_partial_offset_r);
	SOC_LOGI("    reserved1:          %x\r\n", hw->x_partial.reserved1);
	SOC_LOGI("    partial_display_en: %x\r\n", hw->x_partial.partial_display_en);
	SOC_LOGI("    sync_edge_dect_en:  %x\r\n", hw->x_partial.sync_edge_dect_en);
	SOC_LOGI("    stream_fifo_empty:  %x\r\n", hw->x_partial.stream_fifo_empty);
	SOC_LOGI("    stream_fifo_full:   %x\r\n", hw->x_partial.stream_fifo_full);
	SOC_LOGI("    is_data_set:        %x\r\n", hw->x_partial.is_data_set);
	SOC_LOGI("    reserved2:          %x\r\n", hw->x_partial.reserved2);
	SOC_LOGI("\r\n");

	/* REG_0x9 */
	SOC_LOGI("  y_partial=0x%x value=0x%x\r\n", &hw->y_partial, hw->y_partial.v);
	SOC_LOGI("    y_partial_offset_l: %x\r\n", hw->y_partial.y_partial_offset_l);
	SOC_LOGI("    reserved0:          %x\r\n", hw->y_partial.reserved0);
	SOC_LOGI("    y_partial_offset_r: %x\r\n", hw->y_partial.y_partial_offset_r);
	SOC_LOGI("    reserved1:          %x\r\n", hw->y_partial.reserved1);
	SOC_LOGI("    y_count:            %x\r\n", hw->y_partial.y_count);
	SOC_LOGI("\r\n");

	/* REG_0xa */
	SOC_LOGI("  byte_count_every_line=0x%x value=0x%x\r\n", &hw->byte_count_every_line, hw->byte_count_every_line);
	SOC_LOGI("\r\n");

	/* REG_0xc */
	SOC_LOGI("  int_en=0x%x value=0x%x\r\n", &hw->int_en, hw->int_en.v);
	SOC_LOGI("    eoy_int_en:   %x\r\n", hw->int_en.eoy_int_en);
	SOC_LOGI("    head_int_en:  %x\r\n", hw->int_en.head_int_en);
	SOC_LOGI("    sof_int_en:   %x\r\n", hw->int_en.sof_int_en);
	SOC_LOGI("    eof_int_en:   %x\r\n", hw->int_en.eof_int_en);
	SOC_LOGI("    mclk_div:     %x\r\n", hw->int_en.mclk_div);
	SOC_LOGI("    vsync_int_en: %x\r\n", hw->int_en.vsync_int_en);
	SOC_LOGI("    reserved:     %x\r\n", hw->int_en.reserved);
	SOC_LOGI("\r\n");

	/* REG_0xd */
	SOC_LOGI("  cfg=0x%x value=0x%x\r\n", &hw->cfg, hw->int_en.v);
	SOC_LOGI("    vck_edge:           %x\r\n", hw->cfg.vck_edge);
	SOC_LOGI("    video_byte_reverse: %x\r\n", hw->cfg.video_byte_reverse);
	SOC_LOGI("    yuv_fmt_sel:        %x\r\n", hw->cfg.yuv_fmt_sel);
	SOC_LOGI("    jpeg_enc_en:        %x\r\n", hw->cfg.jpeg_enc_en);
	SOC_LOGI("    jpeg_word_reverse:  %x\r\n", hw->cfg.jpeg_word_reverse);
	SOC_LOGI("    only_y:             %x\r\n", hw->cfg.only_y);
	SOC_LOGI("    yuvbuf_mode:        %x\r\n", hw->cfg.yuvbuf_mode);
	SOC_LOGI("    x_pixel:            %x\r\n", hw->cfg.x_pixel);
	SOC_LOGI("    jpeg_enc_size:      %x\r\n", hw->cfg.jpeg_enc_size);
	SOC_LOGI("    bitrate_ctrl:       %x\r\n", hw->cfg.bitrate_ctrl);
	SOC_LOGI("    bitrate_step:       %x\r\n", hw->cfg.bitrate_step);
	SOC_LOGI("    auto_step:          %x\r\n", hw->cfg.auto_step);
	SOC_LOGI("    hsync_rev:          %x\r\n", hw->cfg.hsync_rev);
	SOC_LOGI("    vsync_rev:          %x\r\n", hw->cfg.vsync_rev);
	SOC_LOGI("    bitrate_mode:       %x\r\n", hw->cfg.bitrate_mode);
	SOC_LOGI("    y_pixel:            %x\r\n", hw->cfg.y_pixel);
	SOC_LOGI("\r\n");

	SOC_LOGI("  target_byte_h=0x%x value=0x%x\r\n", &hw->target_byte_h, hw->target_byte_h);
	SOC_LOGI("  target_byte_l=0x%x value=0x%x\r\n", &hw->target_byte_l, hw->target_byte_l);
}

#endif

