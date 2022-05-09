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
#include "qspi_hal.h"
#include "qspi_hw.h"
#include "qspi_ll.h"

#if CFG_HAL_DEBUG_QSPI

void qspi_struct_dump(void)
{
	qspi_hw_t *hw = (qspi_hw_t *)QSPI_LL_REG_BASE(0);
	SOC_LOGI("base=%x\r\n", (uint32_t)hw);

	SOC_LOGI("  sw_cmd=0x%x value=0x%x\n", &hw->sw_cmd, hw->sw_cmd.v);
	SOC_LOGI("    sw_cmd_en:  %x\n", hw->sw_cmd.sw_cmd_en);
	SOC_LOGI("    sw_cmd_bw:  %x\n", hw->sw_cmd.sw_cmd_bw);
	SOC_LOGI("    sw_cmd:     %x\n", hw->sw_cmd.sw_cmd);
	SOC_LOGI("    sw_cmd_cnt: %x\n", hw->sw_cmd.sw_cmd_cnt);
	SOC_LOGI("\n");

	SOC_LOGI("  sw_addr=0x%x value=0x%x\n", &hw->sw_addr, hw->sw_addr.v);
	SOC_LOGI("    sw_addr_en:  %x\n", hw->sw_addr.sw_addr_en);
	SOC_LOGI("    sw_addr_bw:  %x\n", hw->sw_addr.sw_addr_bw);
	SOC_LOGI("    sw_addr:     %x\n", hw->sw_addr.sw_addr);
	SOC_LOGI("    sw_addr_cnt: %x\n", hw->sw_addr.sw_addr_cnt);
	SOC_LOGI("\n");

	SOC_LOGI("  sw_dum=0x%x value=0x%x\n", &hw->sw_dum, hw->sw_dum.v);
	SOC_LOGI("    sw_dum_en:  %x\n", hw->sw_dum.sw_dum_en);
	SOC_LOGI("    sw_dum_bw:  %x\n", hw->sw_dum.sw_dum_bw);
	SOC_LOGI("    sw_dum_cnt: %x\n", hw->sw_dum.sw_dum_cnt);
	SOC_LOGI("\n");

	SOC_LOGI("  sw_dat=0x%x value=0x%x\n", &hw->sw_dat, hw->sw_dat.v);
	SOC_LOGI("    sw_dat_en:  %x\n", hw->sw_dat.sw_dat_en);
	SOC_LOGI("    sw_dat_bw:  %x\n", hw->sw_dat.sw_dat_bw);
	SOC_LOGI("    sw_dat_cnt: %x\n", hw->sw_dat.sw_dat_cnt);
	SOC_LOGI("    sw_dat_dir: %x\n", hw->sw_dat.sw_dat_dir);
	SOC_LOGI("\n");

	SOC_LOGI("  addr_vid_ini=0x%x value=0x%x\n", &hw->addr_vid_ini, hw->addr_vid_ini.v);
	SOC_LOGI("    addr_vid_ini: %x\n", hw->addr_vid_ini.addr_vid_ini);
	SOC_LOGI("\n");

	SOC_LOGI("  addr_aud_ini=0x%x value=0x%x\n", &hw->addr_aud_ini, hw->addr_aud_ini.v);
	SOC_LOGI("    addr_aud_ini: %x\n", hw->addr_aud_ini.addr_aud_ini);
	SOC_LOGI("\n");

	SOC_LOGI("  addr_vid_bon=0x%x value=0x%x\n", &hw->addr_vid_bon, hw->addr_vid_bon.v);
	SOC_LOGI("    addr_vid_bon: %x\n", hw->addr_vid_bon.addr_vid_bon);
	SOC_LOGI("\n");


	SOC_LOGI("  addr_aud_bon=0x%x value=0x%x\n", &hw->addr_aud_bon, hw->addr_aud_bon.v);
	SOC_LOGI("    addr_aud_bon: %x\n", hw->addr_aud_bon.addr_aud_bon);
	SOC_LOGI("\n");

	SOC_LOGI("  sw_op=0x%x value=0x%x\n", &hw->sw_op, hw->sw_op.v);
	SOC_LOGI("    sw_op_type:    %x\n", hw->sw_op.sw_op_type);
	SOC_LOGI("    sw_start:      %x\n", hw->sw_op.sw_start);
	SOC_LOGI("    addr_ini_set:  %x\n", hw->sw_op.addr_ini_set);
	SOC_LOGI("    int_sw_op_clr: %x\n", hw->sw_op.int_sw_op_clr);
	SOC_LOGI("    int_mask:      %x\n", hw->sw_op.int_mask);
	SOC_LOGI("    bp_empty_aud:  %x\n", hw->sw_op.bp_empty_aud);
	SOC_LOGI("    bp_full_aud:   %x\n", hw->sw_op.bp_full_aud);
	SOC_LOGI("    bp_empty_vid:  %x\n", hw->sw_op.bp_empty_vid);
	SOC_LOGI("    bp_full_vid:   %x\n", hw->sw_op.bp_full_vid);
	SOC_LOGI("\n");

	SOC_LOGI("  vid_trxfifo_data=0x%x, value=0x%x\n", &hw->vid_trxfifo_data, hw->vid_trxfifo_data);
	SOC_LOGI("  aud_trxfifo_data=0x%x, value=0x%x\n", &hw->aud_trxfifo_data, hw->aud_trxfifo_data);
	SOC_LOGI("  ge0_trxfifo_data=0x%x, value=0x%x\n", &hw->ge0_trxfifo_data, hw->ge0_trxfifo_data);
	SOC_LOGI("  ge1_trxfifo_data=0x%x, value=0x%x\n", &hw->ge1_trxfifo_data, hw->ge1_trxfifo_data);
	SOC_LOGI("  fls_trxfifo_data=0x%x, value=0x%x\n", &hw->fls_trxfifo_data, hw->fls_trxfifo_data);

	SOC_LOGI("  vid_th=0x%x value=0x%x\n", &hw->vid_th, hw->vid_th.v);
	SOC_LOGI("    vidwr_th: %x\n", hw->vid_th.vidwr_th);
	SOC_LOGI("    vidrd_th: %x\n", hw->vid_th.vidrd_th);
	SOC_LOGI("\n");

	SOC_LOGI("  aud_th=0x%x value=0x%x\n", &hw->aud_th, hw->aud_th.v);
	SOC_LOGI("    audwr_th: %x\n", hw->aud_th.audwr_th);
	SOC_LOGI("    audrd_th: %x\n", hw->aud_th.audrd_th);
	SOC_LOGI("\n");

	SOC_LOGI("  ge0_th=0x%x value=0x%x\n", &hw->ge0_th, hw->ge0_th.v);
	SOC_LOGI("    ge0wr_th: %x\n", hw->ge0_th.ge0wr_th);
	SOC_LOGI("    ge0rd_th: %x\n", hw->ge0_th.ge0rd_th);
	SOC_LOGI("\n");

	SOC_LOGI("  ge1_th=0x%x value=0x%x\n", &hw->ge1_th, hw->ge1_th.v);
	SOC_LOGI("    ge1wr_th: %x\n", hw->ge1_th.ge1wr_th);
	SOC_LOGI("    ge1rd_th: %x\n", hw->ge1_th.ge1rd_th);
	SOC_LOGI("\n");

	SOC_LOGI("  vid_dep=0x%x value=0x%x\n", &hw->vid_dep, hw->vid_dep.v);
	SOC_LOGI("    vidwr_dep: %x\n", hw->vid_dep.vidwr_dep);
	SOC_LOGI("    vidrd_dep: %x\n", hw->vid_dep.vidrd_dep);
	SOC_LOGI("\n");

	SOC_LOGI("  aud_dep=0x%x value=0x%x\n", &hw->aud_dep, hw->aud_dep.v);
	SOC_LOGI("    audwr_dep: %x\n", hw->aud_dep.audwr_dep);
	SOC_LOGI("    audrd_dep: %x\n", hw->aud_dep.audrd_dep);
	SOC_LOGI("\n");

	SOC_LOGI("  ge0_dep=0x%x value=0x%x\n", &hw->ge0_dep, hw->ge0_dep.v);
	SOC_LOGI("    ge0wr_dep: %x\n", hw->ge0_dep.ge0wr_dep);
	SOC_LOGI("    ge0rd_dep: %x\n", hw->ge0_dep.ge0rd_dep);
	SOC_LOGI("\n");

	SOC_LOGI("  ge1_dep=0x%x value=0x%x\n", &hw->ge1_dep, hw->ge1_dep.v);
	SOC_LOGI("    ge1wr_dep: %x\n", hw->ge1_dep.ge1wr_dep);
	SOC_LOGI("    ge1rd_dep: %x\n", hw->ge1_dep.ge1rd_dep);
	SOC_LOGI("\n");

	SOC_LOGI("  fls_dep=0x%x value=0x%x\n", &hw->fls_dep, hw->fls_dep.v);
	SOC_LOGI("    flswr_dep: %x\n", hw->fls_dep.flswr_dep);
	SOC_LOGI("    flsrd_dep: %x\n", hw->fls_dep.flsrd_dep);
	SOC_LOGI("\n");

	SOC_LOGI("  enable=0x%x value=0x%x\n", &hw->enable, hw->enable.v);
	SOC_LOGI("    fls_rx_enable: %x\n", hw->enable.fls_rx_enable);
	SOC_LOGI("    fls_tx_enable: %x\n", hw->enable.fls_tx_enable);
	SOC_LOGI("    ge1_rx_enable: %x\n", hw->enable.ge1_rx_enable);
	SOC_LOGI("    ge1_tx_enable: %x\n", hw->enable.ge1_tx_enable);
	SOC_LOGI("    ge0_rx_enable: %x\n", hw->enable.ge0_rx_enable);
	SOC_LOGI("    ge0_tx_enable: %x\n", hw->enable.ge0_tx_enable);
	SOC_LOGI("    aud_rx_enable: %x\n", hw->enable.aud_rx_enable);
	SOC_LOGI("    aud_tx_enable: %x\n", hw->enable.aud_tx_enable);
	SOC_LOGI("    vid_rx_enable: %x\n", hw->enable.vid_rx_enable);
	SOC_LOGI("    vid_tx_enable: %x\n", hw->enable.vid_tx_enable);
	SOC_LOGI("\n");

	SOC_LOGI("  en_dly_cnt=0x%x value=0x%x\n", &hw->en_dly_cnt, hw->en_dly_cnt.v);
	SOC_LOGI("    en_dly_cnt: %x\n", hw->en_dly_cnt.en_dly_cnt);
	SOC_LOGI("\n");

	SOC_LOGI("  ctrl=0x%x value=0x%x\n", &hw->ctrl, hw->ctrl.v);
	SOC_LOGI("    m_value:      %x\n", hw->ctrl.m_value);
	SOC_LOGI("    clk_div:      %x\n", hw->ctrl.clk_div);
	SOC_LOGI("    flsrd_sel:    %x\n", hw->ctrl.flsrd_sel);
	SOC_LOGI("    sclk_fls_com: %x\n", hw->ctrl.sclk_fls_com);
	SOC_LOGI("\n");

	SOC_LOGI("  vid_cnt=0x%x value=0x%x\n", &hw->vid_cnt, hw->vid_cnt.v);
	SOC_LOGI("    vidwr_cnt: %x\n", hw->vid_cnt.vidwr_cnt);
	SOC_LOGI("    vidrd_cnt: %x\n", hw->vid_cnt.vidrd_cnt);
	SOC_LOGI("\n");

	SOC_LOGI("  aud_cnt=0x%x value=0x%x\n", &hw->aud_cnt, hw->aud_cnt.v);
	SOC_LOGI("    audwr_cnt: %x\n", hw->aud_cnt.audwr_cnt);
	SOC_LOGI("    audrd_cnt: %x\n", hw->aud_cnt.audrd_cnt);
	SOC_LOGI("\n");

	SOC_LOGI("  vid_psram_full_th=0x%x value=0x%x\n", &hw->vid_psram_full_th, hw->vid_psram_full_th.v);
	SOC_LOGI("    vid_psram_full_th: %x\n", hw->vid_psram_full_th.vid_psram_full_th);
	SOC_LOGI("\n");

	SOC_LOGI("  vid_psram_empty_th=0x%x value=0x%x\n", &hw->vid_psram_empty_th, hw->vid_psram_empty_th.v);
	SOC_LOGI("    vid_psram_empty_th: %x\n", hw->vid_psram_empty_th.vid_psram_empty_th);
	SOC_LOGI("\n");

	SOC_LOGI("  aud_psram_full_th=0x%x value=0x%x\n", &hw->aud_psram_full_th, hw->aud_psram_full_th.v);
	SOC_LOGI("    aud_psram_full_th: %x\n", hw->aud_psram_full_th.aud_psram_full_th);
	SOC_LOGI("\n");

	SOC_LOGI("  aud_psram_empty_th=0x%x value=0x%x\n", &hw->aud_psram_empty_th, hw->aud_psram_empty_th.v);
	SOC_LOGI("    aud_psram_empty_th: %x\n", hw->aud_psram_empty_th.aud_psram_empty_th);
	SOC_LOGI("\n");

	SOC_LOGI("  dcard_cmd=0x%x value=0x%x\n", &hw->dcard_cmd, hw->dcard_cmd.v);
	SOC_LOGI("    dcard_cmd_en:  %x\n", hw->dcard_cmd.dcard_cmd_en);
	SOC_LOGI("    dcard_cmd_bw:  %x\n", hw->dcard_cmd.dcard_cmd_bw);
	SOC_LOGI("    dcard_cmd:     %x\n", hw->dcard_cmd.dcard_cmd);
	SOC_LOGI("    dcard_cmd_cnt: %x\n", hw->dcard_cmd.dcard_cmd_cnt);
	SOC_LOGI("\n");

	SOC_LOGI("  dcard_addr=0x%x value=0x%x\n", &hw->dcard_addr, hw->dcard_addr.v);
	SOC_LOGI("    dcard_addr_en:  %x\n", hw->dcard_addr.dcard_addr_en);
	SOC_LOGI("    dcard_addr_bw:  %x\n", hw->dcard_addr.dcard_addr_bw);
	SOC_LOGI("    dcard_addr_cnt: %x\n", hw->dcard_addr.dcard_addr_cnt);
	SOC_LOGI("\n");

	SOC_LOGI("  dcard_dum=0x%x value=0x%x\n", &hw->dcard_dum, hw->dcard_dum.v);
	SOC_LOGI("    dcard_dum_en:  %x\n", hw->dcard_dum.dcard_dum_en);
	SOC_LOGI("    dcard_dum_bw:  %x\n", hw->dcard_dum.dcard_dum_bw);
	SOC_LOGI("    dcard_dum_cnt: %x\n", hw->dcard_dum.dcard_dum_cnt);
	SOC_LOGI("\n");

	SOC_LOGI("  dcard_dat=0x%x value=0x%x\n", &hw->dcard_dat, hw->dcard_dat.v);
	SOC_LOGI("    dcard_dat_en:  %x\n", hw->dcard_dat.dcard_dat_en);
	SOC_LOGI("    dcard_dat_bw:  %x\n", hw->dcard_dat.dcard_dat_bw);
	SOC_LOGI("    dcard_dat_cnt: %x\n", hw->dcard_dat.dcard_dat_cnt);
	SOC_LOGI("    dcard_dat_dir: %x\n", hw->dcard_dat.dcard_dat_dir);
	SOC_LOGI("\n");

	SOC_LOGI("  dcawr_cmd=0x%x value=0x%x\n", &hw->dcawr_cmd, hw->dcawr_cmd.v);
	SOC_LOGI("    dcawr_cmd_en:  %x\n", hw->dcawr_cmd.dcawr_cmd_en);
	SOC_LOGI("    dcawr_cmd_bw:  %x\n", hw->dcawr_cmd.dcawr_cmd_bw);
	SOC_LOGI("    dcawr_cmd:     %x\n", hw->dcawr_cmd.dcawr_cmd);
	SOC_LOGI("    dcawr_cmd_cnt: %x\n", hw->dcawr_cmd.dcawr_cmd_cnt);
	SOC_LOGI("\n");

	SOC_LOGI("  dcawr_addr=0x%x value=0x%x\n", &hw->dcawr_addr, hw->dcawr_addr.v);
	SOC_LOGI("    dcawr_addr_en:  %x\n", hw->dcawr_addr.dcawr_addr_en);
	SOC_LOGI("    dcawr_addr_bw:  %x\n", hw->dcawr_addr.dcawr_addr_bw);
	SOC_LOGI("    dcawr_addr_cnt: %x\n", hw->dcawr_addr.dcawr_addr_cnt);
	SOC_LOGI("\n");

	SOC_LOGI("  dcawr_dum=0x%x value=0x%x\n", &hw->dcawr_dum, hw->dcawr_dum.v);
	SOC_LOGI("    dcawr_dum_en:  %x\n", hw->dcawr_dum.dcawr_dum_en);
	SOC_LOGI("    dcawr_dum_bw:  %x\n", hw->dcawr_dum.dcawr_dum_bw);
	SOC_LOGI("    dcawr_dum_cnt: %x\n", hw->dcawr_dum.dcawr_dum_cnt);
	SOC_LOGI("\n");

	SOC_LOGI("  dcawr_dat=0x%x value=0x%x\n", &hw->dcawr_dat, hw->dcawr_dat.v);
	SOC_LOGI("    dcawr_dat_en:  %x\n", hw->dcawr_dat.dcawr_dat_en);
	SOC_LOGI("    dcawr_dat_bw:  %x\n", hw->dcawr_dat.dcawr_dat_bw);
	SOC_LOGI("    dcawr_dat_cnt: %x\n", hw->dcawr_dat.dcawr_dat_cnt);
	SOC_LOGI("    dcawr_dat_dir: %x\n", hw->dcawr_dat.dcawr_dat_dir);
	SOC_LOGI("\n");

	SOC_LOGI("  rw_req_msk=0x%x value=0x%x\n", &hw->rw_req_msk, hw->rw_req_msk.v);
	SOC_LOGI("    rw_req_msk: %x\n", hw->rw_req_msk.rw_req_msk);
	SOC_LOGI("\n");

	SOC_LOGI("  fifo_status=0x%x value=0x%x\n", &hw->fifo_status, hw->fifo_status);

	SOC_LOGI("  int_status=0x%x value=0x%x\n", &hw->int_status, hw->int_status.v);
	SOC_LOGI("    int_status:     %x\n", hw->int_status.int_status);
	SOC_LOGI("    int_sw_op:      %x\n", hw->int_status.int_sw_op);
	SOC_LOGI("    near_empty_aud: %x\n", hw->int_status.near_empty_aud);
	SOC_LOGI("    near_full_aud:  %x\n", hw->int_status.near_full_aud);
	SOC_LOGI("    near_empty_vid: %x\n", hw->int_status.near_empty_vid);
	SOC_LOGI("    near_full_vid:  %x\n", hw->int_status.near_full_vid);
	SOC_LOGI("\n");

	SOC_LOGI("  addr_vid_rd_reg=0x%x value=0x%x\n", &hw->addr_vid_rd_reg, hw->addr_vid_rd_reg.v);
	SOC_LOGI("    addr_vid_rd_reg: %x\n", hw->addr_vid_rd_reg.addr_vid_rd_reg);
	SOC_LOGI("\n");

	SOC_LOGI("  addr_vid_wr_reg=0x%x value=0x%x\n", &hw->addr_vid_wr_reg, hw->addr_vid_wr_reg.v);
	SOC_LOGI("    addr_vid_wr_reg: %x\n", hw->addr_vid_wr_reg.addr_vid_wr_reg);
	SOC_LOGI("\n");

	SOC_LOGI("  addr_aud_rd_reg=0x%x value=0x%x\n", &hw->addr_aud_rd_reg, hw->addr_aud_rd_reg.v);
	SOC_LOGI("    addr_aud_rd_reg: %x\n", hw->addr_aud_rd_reg.addr_aud_rd_reg);
	SOC_LOGI("\n");

	SOC_LOGI("  addr_aud_wr_reg=0x%x value=0x%x\n", &hw->addr_aud_wr_reg, hw->addr_aud_wr_reg.v);
	SOC_LOGI("    addr_aud_wr_reg: %x\n", hw->addr_aud_wr_reg.addr_aud_wr_reg);
	SOC_LOGI("\n");
}

#endif

