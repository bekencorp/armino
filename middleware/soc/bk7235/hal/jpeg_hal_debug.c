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
	jpeg_hw_t *hw = (jpeg_hw_t *)JPEG_LL_REG_BASE(0);
	SOC_LOGI("base=%x\r\n", (uint32_t)hw);

	SOC_LOGI("  int_en=0x%x value=0x%x\n", &hw->int_en, hw->int_en.v);
	SOC_LOGI("  int_en:  %x\n", hw->int_en.int_en);
	SOC_LOGI("  mclk_div:     %x\n", hw->int_en.mclk_div);
	SOC_LOGI("  vsync_int_en:%x\n", hw->int_en.vsync_int_en);
	SOC_LOGI("  reserved1: %x\n", hw->int_en.reserved1);
	SOC_LOGI("\n");

	SOC_LOGI("  cfg=0x%x value=0x%x\n", &hw->cfg, hw->cfg.v);
	SOC_LOGI("  vck_edge:           %x\n", hw->cfg.vck_edge);
	SOC_LOGI("  video_byte_reverse: %x\n", hw->cfg.video_byte_reverse);
	SOC_LOGI("  yuv_fmt_sel:        %x\n", hw->cfg.yuv_fmt_sel);
	SOC_LOGI("  jpeg_enc_en:        %x\n", hw->cfg.jpeg_enc_en);
	SOC_LOGI("  yuv_byte_reverse:   %x\n", hw->cfg.yuv_byte_reverse);
	SOC_LOGI("  only_y:             %x\n", hw->cfg.only_y);
	SOC_LOGI("  yuvbuf_mode:        %x\n", hw->cfg.yuvbuf_mode);
	SOC_LOGI("  x_pixel:            %x\n", hw->cfg.x_pixel);
	SOC_LOGI("  jpeg_enc_size:      %x\n", hw->cfg.jpeg_enc_size);
	SOC_LOGI("  bitrate_ctrl:       %x\n", hw->cfg.bitrate_ctrl);
	SOC_LOGI("  bitrate_step:       %x\n", hw->cfg.bitrate_step);
	SOC_LOGI("  auto_step:          %x\n", hw->cfg.auto_step);
	SOC_LOGI("  hsync_rev:          %x\n", hw->cfg.hsync_rev);
	SOC_LOGI("  vsync_rev:          %x\n", hw->cfg.vsync_rev);
	SOC_LOGI("  bitrate_mode:       %x\n", hw->cfg.bitrate_mode);
	SOC_LOGI("  y_pixel:            %x\n", hw->cfg.y_pixel);
	SOC_LOGI("\n");

	SOC_LOGI("  target_byte_h=0x%x value=0x%x\n", &hw->target_byte_h, hw->target_byte_h);
	SOC_LOGI("  target_byte_l=0x%x value=0x%x\n", &hw->target_byte_l, hw->target_byte_l);
	SOC_LOGI("  em_base_addr=0x%x  value=0x%x\n", &hw->em_base_addr, hw->em_base_addr.em_base_addr);
	SOC_LOGI("  rx_fifo_data=0x%x  value=0x%x\n", &hw->rx_fifo_data, hw->rx_fifo_data);

	SOC_LOGI("\n");
	SOC_LOGI("  int_status=0x%x value=0x%x\n", &hw->int_status, hw->int_status.v);
	SOC_LOGI("  int_status:       %x\n", hw->int_status.int_status);
	SOC_LOGI("  fifo_rd_finish:  %x\n", hw->int_status.fifo_rd_finish);
	SOC_LOGI("  reserved:  %x\n", hw->int_status.reserved);
	SOC_LOGI("\n");

	SOC_LOGI("  byte_count_pfrm=0x%x value=0x%x\n", &hw->byte_count_pfrm, hw->byte_count_pfrm);
	SOC_LOGI("\n");

	SOC_LOGI("  x_partial=0x%x value=0x%x\n", &hw->x_partial, hw->x_partial.v);
	SOC_LOGI("  x_partial_offset_l:  %x\n", hw->x_partial.x_partial_offset_l);
	SOC_LOGI("  reserved:            %x\n", hw->x_partial.reserved);
	SOC_LOGI("  x_partial_offset_r:  %x\n", hw->x_partial.x_partial_offset_r);
	SOC_LOGI("  reserved1:           %x\n", hw->x_partial.reserved1);
	SOC_LOGI("  partial_display_en:  %x\n", hw->x_partial.partial_display_en);
	SOC_LOGI("  sync_edge_dect_en:   %x\n", hw->x_partial.sync_edge_dect_en);
	SOC_LOGI("  stream_fifo_empty:   %x\n", hw->x_partial.stream_fifo_empty);
	SOC_LOGI("  stream_fifo_full:    %x\n", hw->x_partial.stream_fifo_full);
	SOC_LOGI("  is_data_set:         %x\n", hw->x_partial.is_data_set);
	SOC_LOGI("  reserved2:           %x\n", hw->x_partial.reserved2);
	SOC_LOGI("\n");

	SOC_LOGI("  y_partial=0x%x value=0x%x\n", &hw->y_partial, hw->y_partial.v);
	SOC_LOGI("  y_partial_offset_l:  %x\n", hw->y_partial.y_partial_offset_l);
	SOC_LOGI("  reserved:            %x\n", hw->y_partial.reserved);
	SOC_LOGI("  y_partial_offset_r:  %x\n", hw->y_partial.y_partial_offset_r);
	SOC_LOGI("  reserved1:           %x\n", hw->y_partial.reserved1);
	SOC_LOGI("  y_count:             %x\n", hw->y_partial.y_count);
	SOC_LOGI("\n");
}

#endif

