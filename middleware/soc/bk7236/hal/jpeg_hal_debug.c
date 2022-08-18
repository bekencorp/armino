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
	SOC_LOGI("  reserved0:  %x\n", hw->int_en.reserved0);
	SOC_LOGI("  int_en:  %x\n", hw->int_en.int_en);
	SOC_LOGI("  mclk_div:     %x\n", hw->int_en.mclk_div);
	SOC_LOGI("  reserved1: %x\n", hw->int_en.reserved1);
	SOC_LOGI("\n");

	SOC_LOGI("  cfg=0x%x value=0x%x\n", &hw->cfg, hw->cfg.v);
	SOC_LOGI("  reserved0:          %x\n", hw->cfg.reserved0);
	SOC_LOGI("  video_byte_reverse: %x\n", hw->cfg.video_byte_reverse);
	SOC_LOGI("  yuv_fmt_sel:        %x\n", hw->cfg.yuv_fmt_sel);
	SOC_LOGI("  jpeg_enc_en:        %x\n", hw->cfg.jpeg_enc_en);
	SOC_LOGI("  reserved1:          %x\n", hw->cfg.reserved1);
	SOC_LOGI("  x_pixel:            %x\n", hw->cfg.x_pixel);
	SOC_LOGI("  jpeg_enc_size:      %x\n", hw->cfg.jpeg_enc_size);
	SOC_LOGI("  bitrate_ctrl:       %x\n", hw->cfg.bitrate_ctrl);
	SOC_LOGI("  bitrate_step:       %x\n", hw->cfg.bitrate_step);
	SOC_LOGI("  hsync_rev:          %x\n", hw->cfg.hsync_rev);
	SOC_LOGI("  vsync_rev:          %x\n", hw->cfg.vsync_rev);
	SOC_LOGI("  reserved2:          %x\n", hw->cfg.reserved2);
	SOC_LOGI("  y_pixel:            %x\n", hw->cfg.y_pixel);
	SOC_LOGI("\n");

	SOC_LOGI("  target_byte_h=0x%x value=0x%x\n", &hw->target_byte_h, hw->target_byte_h);
	SOC_LOGI("  target_byte_l=0x%x value=0x%x\n", &hw->target_byte_l, hw->target_byte_l);
	SOC_LOGI("  reserved=0x%x value=0x%x\n", &hw->reserved, hw->reserved);
	SOC_LOGI("  rx_fifo_data=0x%x value=0x%x\n", &hw->rx_fifo_data, hw->rx_fifo_data);

	SOC_LOGI("\n");
	SOC_LOGI("  int_status=0x%x value=0x%x\n", &hw->int_status, hw->int_status.v);
	SOC_LOGI("  int_status:  %x\n", hw->int_status.int_status);
	SOC_LOGI("  reserved:  %x\n", hw->int_status.reserved);
	SOC_LOGI("\n");

	SOC_LOGI("  byte_count_pfrm=0x%x value=0x%x\n", &hw->byte_count_pfrm, hw->byte_count_pfrm);
}

#endif

