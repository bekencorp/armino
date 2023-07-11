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

#include "hal_config.h"
#include "jpeg_dec_hal.h"
#include "jpeg_dec_hw.h"

#if CFG_HAL_DEBUG_JPEG_DEC

void jpeg_dec_struct_dump(void)
{
	jpeg_dec_hw_t *hw = (jpeg_dec_hw_t *)JPEG_DEC_R_BASE;
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

	/* REG_0x04 */
	SOC_LOGI("  jd_mask_cur=0x%x value=0x%x\r\n", &hw->jd_mask_cur, hw->jd_mask_cur);

	/* REG_0x05 */
	SOC_LOGI("  mcu_x=0x%x value=0x%x\r\n", &hw->mcu_x, hw->mcu_x.v);
	SOC_LOGI("    mcu_x:    %x\r\n", hw->mcu_x.mcu_x);
	SOC_LOGI("    reserved: %x\r\n", hw->mcu_x.reserved);
	SOC_LOGI("\r\n");

	/* REG_0x06 */
	SOC_LOGI("  mcu_y=0x%x value=0x%x\r\n", &hw->mcu_y, hw->mcu_y.v);
	SOC_LOGI("    mcu_y:    %x\r\n", hw->mcu_y.mcu_y);
	SOC_LOGI("    reserved: %x\r\n", hw->mcu_y.reserved);
	SOC_LOGI("\r\n");

	/* REG_0x07 */
	SOC_LOGI("  v_vld_num=0x%x value=0x%x\r\n", &hw->v_vld_num, hw->v_vld_num.v);
	SOC_LOGI("    v_vld_num: %x\r\n", hw->v_vld_num.v_vld_num);
	SOC_LOGI("    reserved:  %x\r\n", hw->v_vld_num.reserved);
	SOC_LOGI("\r\n");

	/* REG_0x08 */
	SOC_LOGI("  dec_cmd=0x%x value=0x%x\r\n", &hw->dec_cmd, hw->dec_cmd.v);
	SOC_LOGI("    dec_cmd:  %x\r\n", hw->dec_cmd.dec_cmd);
	SOC_LOGI("    reserved: %x\r\n", hw->dec_cmd.reserved);
	SOC_LOGI("\r\n");

	/* REG_0x9 */
	SOC_LOGI("  cur_run=0x%x value=0x%x\r\n", &hw->cur_run, hw->cur_run);

	/* REG_0xa */
	SOC_LOGI("  x_pixel=0x%x value=0x%x\r\n", &hw->x_pixel, hw->x_pixel.v);
	SOC_LOGI("    x_pixel:  %x\r\n", hw->x_pixel.x_pixel);
	SOC_LOGI("    reserved: %x\r\n", hw->x_pixel.reserved);
	SOC_LOGI("\r\n");

	/* REG_0xb */
	SOC_LOGI("  huf_data=0x%x value=0x%x\r\n", &hw->huf_data, hw->huf_data);

	/* REG_0xc */
	SOC_LOGI("  cur_nbit=0x%x value=0x%x\r\n", &hw->cur_nbit, hw->cur_nbit);

	/* REG_0xd */
	SOC_LOGI("  state=0x%x value=0x%x\r\n", &hw->state, hw->state.v);
	SOC_LOGI("    state_dec_busy:    %x\r\n", hw->state.state_dec_busy);
	SOC_LOGI("    state_rrload:      %x\r\n", hw->state.state_rrload);
	SOC_LOGI("    state_rload:       %x\r\n", hw->state.state_rload);
	SOC_LOGI("    state_search:      %x\r\n", hw->state.state_search);
	SOC_LOGI("    state_ext_rload:   %x\r\n", hw->state.state_ext_rload);
	SOC_LOGI("    state_ext_bits:    %x\r\n", hw->state.state_ext_bits);
	SOC_LOGI("    state_idct:        %x\r\n", hw->state.state_idct);
	SOC_LOGI("    state_tmp_wr0:     %x\r\n", hw->state.state_tmp_wr0);
	SOC_LOGI("    state_dec_busy2:   %x\r\n", hw->state.state_dec_busy2);
	SOC_LOGI("    state_ahbmst_idle: %x\r\n", hw->state.state_ahbmst_idle);
	SOC_LOGI("    reserved:          %x\r\n", hw->state.reserved);
	SOC_LOGI("\r\n");

	/* REG_0xe */
	SOC_LOGI("  ext_data=0x%x value=0x%x\r\n", &hw->ext_data, hw->ext_data);

	/* REG_0xf */
	SOC_LOGI("  mcu_blk=0x%x value=0x%x\r\n", &hw->mcu_blk, hw->mcu_blk.v);
	SOC_LOGI("    mcu_blk:  %x\r\n", hw->mcu_blk.mcu_blk);
	SOC_LOGI("    reserved: %x\r\n", hw->mcu_blk.reserved);
	SOC_LOGI("\r\n");

	/* REG_0x10 - REG_0x1f */
	for (uint32_t i = 0; i < 0xf; i++) {
		SOC_LOGI("  hufdc_len_y%x=0x%x value=0x%x\r\n", i, &hw->hufdc_len_y[i], hw->hufdc_len_y[i]);
	}
	SOC_LOGI("\r\n");

	/* REG_0x20 - REG_0x2f */
	for (uint32_t i = 0; i < 0xf; i++) {
		SOC_LOGI("  hufdc_len_uv%x=0x%x value=0x%x\r\n", i, &hw->hufdc_len_uv[i], hw->hufdc_len_uv[i]);
	}
	SOC_LOGI("\r\n");

	/* REG_0x30 - REG_0x3f */
	for (uint32_t i = 0; i < 0xf; i++) {
		SOC_LOGI("  hufac_len_y%x=0x%x value=0x%x\r\n", i, &hw->hufac_len_y[i], hw->hufac_len_y[i]);
	}
	SOC_LOGI("\r\n");

	/* REG_0x40 - REG_0x4f */
	for (uint32_t i = 0; i < 0xf; i++) {
		SOC_LOGI("  hufac_len_uv%x=0x%x value=0x%x\r\n", i, &hw->hufac_len_uv[i], hw->hufac_len_uv[i]);
	}
	SOC_LOGI("\r\n");

	/* REG_0x50 */
	SOC_LOGI("  jpeg_dec_en=0x%x value=0x%x\r\n", &hw->jpeg_dec_en, hw->jpeg_dec_en.v);
	SOC_LOGI("    jpeg_dec_en:  %x\r\n", hw->jpeg_dec_en.jpeg_dec_en);
	SOC_LOGI("    reserved:     %x\r\n", hw->jpeg_dec_en.reserved);
	SOC_LOGI("    dri_bps:      %x\r\n", hw->jpeg_dec_en.dri_bps);
	SOC_LOGI("    yy_vld_bps:   %x\r\n", hw->jpeg_dec_en.yy_vld_bps);
	SOC_LOGI("    sharemem_bps: %x\r\n", hw->jpeg_dec_en.sharemem_bps);
	SOC_LOGI("\r\n");

	/* REG_0x51 */
	SOC_LOGI("  mcu_index=0x%x value=0x%x\r\n", &hw->mcu_index, hw->mcu_index.v);
	SOC_LOGI("    mcu_index: %x\r\n", hw->mcu_index.mcu_index);
	SOC_LOGI("    mcu_y_num: %x\r\n", hw->mcu_index.mcu_y_num);
	SOC_LOGI("    reserved:  %x\r\n", hw->mcu_index.reserved);
	SOC_LOGI("\r\n");

	/* REG_0x52 */
	SOC_LOGI("  jpeg_dec_auto=0x%x value=0x%x\r\n", &hw->jpeg_dec_auto, hw->jpeg_dec_auto.v);
	SOC_LOGI("    jpeg_dec_auto:  %x\r\n", hw->jpeg_dec_auto.jpeg_dec_auto);
	SOC_LOGI("    jpeg_dec_linen: %x\r\n", hw->jpeg_dec_auto.jpeg_dec_linen);
	SOC_LOGI("    reserved0:      %x\r\n", hw->jpeg_dec_auto.reserved0);
	SOC_LOGI("    jpeg_line_num:  %x\r\n", hw->jpeg_dec_auto.jpeg_line_num);
	SOC_LOGI("    reserved1:      %x\r\n", hw->jpeg_dec_auto.reserved1);
	SOC_LOGI("\r\n");

	/* REG_0x53 */
	SOC_LOGI("  uv_vld=0x%x value=0x%x\r\n", &hw->uv_vld, hw->uv_vld.v);
	SOC_LOGI("    uv_vld:   %x\r\n", hw->uv_vld.uv_vld);
	SOC_LOGI("    reserved: %x\r\n", hw->uv_vld.reserved);
	SOC_LOGI("\r\n");

	/* REG_0x57 */
	SOC_LOGI("  master_wr_cnt=0x%x value=0x%x\r\n", &hw->master_wr_cnt, hw->master_wr_cnt);

	/* REG_0x58 */
	SOC_LOGI("  base_raddr=0x%x value=0x%x\r\n", &hw->base_raddr, hw->base_raddr);

	/* REG_0x59 */
	SOC_LOGI("  base_waddr=0x%x value=0x%x\r\n", &hw->base_waddr, hw->base_waddr);

	/* REG_0x5a */
	SOC_LOGI("  base_rd_len=0x%x value=0x%x\r\n", &hw->base_rd_len, hw->base_rd_len);

	/* REG_0x5b */
	SOC_LOGI("  base_wr_len=0x%x value=0x%x\r\n", &hw->base_wr_len, hw->base_wr_len);

	/* REG_0x5c */
	SOC_LOGI("  base_ffda=0x%x value=0x%x\r\n", &hw->base_ffda, hw->base_ffda);

	/* REG_0x5d */
	SOC_LOGI("  master_rd_cnt=0x%x value=0x%x\r\n", &hw->master_rd_cnt, hw->master_rd_cnt);

	/* REG_0x5e */
	SOC_LOGI("  jpeg_dec_auto=0x%x value=0x%x\r\n", &hw->int_en, hw->int_en.v);
	SOC_LOGI("    dec_busy_int:       %x\r\n", hw->int_en.dec_busy_int);
	SOC_LOGI("    dec_rrload_int:     %x\r\n", hw->int_en.dec_rrload_int);
	SOC_LOGI("    dec_rload_int:      %x\r\n", hw->int_en.dec_rload_int);
	SOC_LOGI("    dec_search_int:     %x\r\n", hw->int_en.dec_search_int);
	SOC_LOGI("    dec_ext_reload_int: %x\r\n", hw->int_en.dec_ext_reload_int);
	SOC_LOGI("    dec_ext_bit_int:    %x\r\n", hw->int_en.dec_ext_bit_int);
	SOC_LOGI("    dec_total_busy_int: %x\r\n", hw->int_en.dec_total_busy_int);
	SOC_LOGI("    dec_mcu_finish_int: %x\r\n", hw->int_en.dec_mcu_finish_int);
	SOC_LOGI("    dec_frame_int:      %x\r\n", hw->int_en.dec_frame_int);
	SOC_LOGI("    reserved:           %x\r\n", hw->int_en.reserved);
	SOC_LOGI("\r\n");
}

#endif

