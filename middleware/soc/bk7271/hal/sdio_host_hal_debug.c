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
#include "sdio_hw.h"
#include "sdio_host_hal.h"
#include "sdio_ll.h"

#if CFG_HAL_DEBUG_SDIO_HOST

void sdio_host_struct_dump(void)
{
	sdio_hw_t *hw = (sdio_hw_t *)SDIO_LL_REG_BASE(0);
	SOC_LOGI("base=%x\r\n", (uint32_t)hw);

	/* REG_0x0 */
	SOC_LOGI("  sd_cmd_ctrl=0x%x value=0x%x\n", &hw->sd_cmd_ctrl, hw->sd_cmd_ctrl.v);
	SOC_LOGI("    start:        %x\n", hw->sd_cmd_ctrl.start);
	SOC_LOGI("    rsp_en:       %x\n", hw->sd_cmd_ctrl.rsp_en);
	SOC_LOGI("    long_rsp_en:  %x\n", hw->sd_cmd_ctrl.long_rsp_en);
	SOC_LOGI("    crc_check_en: %x\n", hw->sd_cmd_ctrl.crc_check_en);
	SOC_LOGI("    cmd_index:    %d\n", hw->sd_cmd_ctrl.cmd_index);
	SOC_LOGI("    reserved:     %x\n", hw->sd_cmd_ctrl.reserved);
	SOC_LOGI("\r\n");

	/* REG_0x1 */
	SOC_LOGI("  cmd_argument addr=0x%x value=0x%x\r\n", &hw->cmd_argument, hw->cmd_argument);
	/* REG_0x2 */
	SOC_LOGI("  sd_cmd_timer addr=0x%x value=0x%x\r\n", &hw->sd_cmd_timer, hw->sd_cmd_timer);
	SOC_LOGI("\r\n");

	/* REG_0x3 */
	SOC_LOGI("  sd_data_ctrl=0x%x value=0x%x\n", &hw->sd_data_ctrl, hw->sd_data_ctrl.v);
	SOC_LOGI("    sd_data_en:      %x\n", hw->sd_data_ctrl.sd_data_en);
	SOC_LOGI("    sd_data_stop_en: %x\n", hw->sd_data_ctrl.sd_data_stop_en);
	SOC_LOGI("    sd_data_bus:     %x\n", hw->sd_data_ctrl.sd_data_bus);
	SOC_LOGI("    sd_data_mul_blk: %x\n", hw->sd_data_ctrl.sd_data_mul_blk);
	SOC_LOGI("    sd_data_blk:     %x\n", hw->sd_data_ctrl.sd_data_blk);
	SOC_LOGI("    sd_start_wr_en:  %x\n", hw->sd_data_ctrl.sd_start_wr_en);
	SOC_LOGI("    sd_byte_sel:     %x\n", hw->sd_data_ctrl.sd_byte_sel);
	SOC_LOGI("    reserved:        %x\n", hw->sd_data_ctrl.reserved);
	SOC_LOGI("\r\n");

	/* REG_0x4 */
	SOC_LOGI("  sd_data_timer addr=0x%x value=0x%x\r\n", &hw->sd_data_timer, hw->sd_data_timer);
	/* REG_0x5 */
	SOC_LOGI("  sd_rsp_agument_0 addr=0x%x value=0x%x\r\n", &hw->sd_rsp_agument_0, hw->sd_rsp_agument_0);
	/* REG_0x6 */
	SOC_LOGI("  sd_rsp_agument_1 addr=0x%x value=0x%x\r\n", &hw->sd_rsp_agument_1, hw->sd_rsp_agument_1);
	/* REG_0x7 */
	SOC_LOGI("  sd_rsp_agument_2 addr=0x%x value=0x%x\r\n", &hw->sd_rsp_agument_2, hw->sd_rsp_agument_2);
	/* REG_0x8 */
	SOC_LOGI("  sd_rsp_agument_3 addr=0x%x value=0x%x\r\n", &hw->sd_rsp_agument_3, hw->sd_rsp_agument_3);
	SOC_LOGI("\r\n");

	/* REG_0x9 */
	SOC_LOGI("  sd_cmd_rsp_int_sel=0x%x value=0x%x\n", &hw->sd_cmd_rsp_int_sel, hw->sd_cmd_rsp_int_sel.v);
	SOC_LOGI("    sd_cmd_send_no_rsp_end_int:  %x\n", hw->sd_cmd_rsp_int_sel.sd_cmd_send_no_rsp_end_int);
	SOC_LOGI("    sd_cmd_send_rsp_end_int:     %x\n", hw->sd_cmd_rsp_int_sel.sd_cmd_send_rsp_end_int);
	SOC_LOGI("    sd_cmd_send_rsp_timeout_int: %x\n", hw->sd_cmd_rsp_int_sel.sd_cmd_send_rsp_timeout_int);
	SOC_LOGI("    sd_data_rec_end_int:         %x\n", hw->sd_cmd_rsp_int_sel.sd_data_rec_end_int);
	SOC_LOGI("    sd_data_wr_end_int:          %x\n", hw->sd_cmd_rsp_int_sel.sd_data_wr_end_int);
	SOC_LOGI("    sd_data_time_out_int:        %x\n", hw->sd_cmd_rsp_int_sel.sd_data_time_out_int);
	SOC_LOGI("    rx_fifo_need_read:           %x\n", hw->sd_cmd_rsp_int_sel.rx_fifo_need_read);
	SOC_LOGI("    tx_fifo_need_write:          %x\n", hw->sd_cmd_rsp_int_sel.tx_fifo_need_write);
	SOC_LOGI("    rx_overflow_int:             %x\n", hw->sd_cmd_rsp_int_sel.rx_overflow_int);
	SOC_LOGI("    tx_fifo_empty_int:           %x\n", hw->sd_cmd_rsp_int_sel.tx_fifo_empty_int);
	SOC_LOGI("    sd_rsp_cmd_crc_ok:           %x\n", hw->sd_cmd_rsp_int_sel.sd_rsp_cmd_crc_ok);
	SOC_LOGI("    sd_rsp_cmd_crc_fail:         %x\n", hw->sd_cmd_rsp_int_sel.sd_rsp_cmd_crc_fail);
	SOC_LOGI("    sd_data_crc_ok:              %x\n", hw->sd_cmd_rsp_int_sel.sd_data_crc_ok);
	SOC_LOGI("    sd_data_crc_fail:            %x\n", hw->sd_cmd_rsp_int_sel.sd_data_crc_fail);
	SOC_LOGI("    sd_rsp_index:                %x\n", hw->sd_cmd_rsp_int_sel.sd_rsp_index);
	SOC_LOGI("    wr_status:                   %x\n", hw->sd_cmd_rsp_int_sel.wr_status);
	SOC_LOGI("    data_busy:                   %x\n", hw->sd_cmd_rsp_int_sel.data_busy);
	SOC_LOGI("    reserved:                    %x\n", hw->sd_cmd_rsp_int_sel.reserved);
	SOC_LOGI("\r\n");

	/* REG_0xa */
	SOC_LOGI("  sd_cmd_rsp_int_mask=0x%x value=0x%x\n", &hw->sd_cmd_rsp_int_mask, hw->sd_cmd_rsp_int_mask.v);
	SOC_LOGI("    sd_cmd_send_no_rsp_end_mask:  %x\n", hw->sd_cmd_rsp_int_mask.sd_cmd_send_no_rsp_end_mask);
	SOC_LOGI("    sd_cmd_send_rsp_end_mask:     %x\n", hw->sd_cmd_rsp_int_mask.sd_cmd_send_rsp_end_mask);
	SOC_LOGI("    sd_cmd_send_rsp_timeout_mask: %x\n", hw->sd_cmd_rsp_int_mask.sd_cmd_send_rsp_timeout_mask);
	SOC_LOGI("    sd_data_rec_end_mask:         %x\n", hw->sd_cmd_rsp_int_mask.sd_data_rec_end_mask);
	SOC_LOGI("    sd_data_wr_end_mask:          %x\n", hw->sd_cmd_rsp_int_mask.sd_data_wr_end_mask);
	SOC_LOGI("    sd_data_time_out_mask:        %x\n", hw->sd_cmd_rsp_int_mask.sd_data_time_out_mask);
	SOC_LOGI("    rx_fifo_need_read_mask:       %x\n", hw->sd_cmd_rsp_int_mask.rx_fifo_need_read_mask);
	SOC_LOGI("    tx_fifo_need_write_mask:      %x\n", hw->sd_cmd_rsp_int_mask.tx_fifo_need_write_mask);
	SOC_LOGI("    rx_overflow_mask:             %x\n", hw->sd_cmd_rsp_int_mask.rx_overflow_mask);
	SOC_LOGI("    tx_fifo_empty_mask:           %x\n", hw->sd_cmd_rsp_int_mask.tx_fifo_empty_mask);
	SOC_LOGI("    reserved:                     %x\n", hw->sd_cmd_rsp_int_mask.reserved);
	SOC_LOGI("\r\n");

	/* REG_0xb */
	SOC_LOGI("  tx_fifo_din addr=0x%x value=0x%x\r\n", &hw->tx_fifo_din, hw->tx_fifo_din);

	/* REG_0xc */
	SOC_LOGI("  rx_fifo_dout addr=0x%x value=0x%x\r\n", &hw->rx_fifo_dout, hw->rx_fifo_dout);

	/* REG_0xd */
	SOC_LOGI("  sd_fifo_threshold=0x%x value=0x%x\n", &hw->sd_fifo_threshold, hw->sd_fifo_threshold.v);
	SOC_LOGI("    rx_fifo_threshold: %x\n", hw->sd_fifo_threshold.rx_fifo_threshold);
	SOC_LOGI("    tx_fifo_threshold: %x\n", hw->sd_fifo_threshold.tx_fifo_threshold);
	SOC_LOGI("    rx_fifo_reset:     %x\n", hw->sd_fifo_threshold.rx_fifo_reset);
	SOC_LOGI("    tx_fifo_reset:     %x\n", hw->sd_fifo_threshold.tx_fifo_reset);
	SOC_LOGI("    rx_fifo_rd_ready:  %x\n", hw->sd_fifo_threshold.rx_fifo_rd_ready);
	SOC_LOGI("    tx_fifo_wr_ready:  %x\n", hw->sd_fifo_threshold.tx_fifo_wr_ready);
	SOC_LOGI("    sd_start_reset:    %x\n", hw->sd_fifo_threshold.sd_start_reset);
	SOC_LOGI("    sd_rate_mux:       %x\n", hw->sd_fifo_threshold.sd_rate_mux);
	SOC_LOGI("    reserved:          %x\n", hw->sd_fifo_threshold.reserved);
}

#endif

