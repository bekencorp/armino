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
#include "spi_hw.h"
#include "spi_hal.h"
#include "spi_ll.h"

#if CFG_HAL_DEBUG_SPI

void spi_struct_dump(spi_id_t id)
{
	spi_hw_t *hw = (spi_hw_t *)SPI_LL_REG_BASE(0);
	SOC_LOGI("base=%x\r\n", (uint32_t)hw);

	SOC_LOGI("  ctrl=0x%x value=0x%x\n", &hw->ctrl, hw->ctrl.v);
	SOC_LOGI("    tx_fifo_int_level: %x\n", hw->ctrl.tx_fifo_int_level);
	SOC_LOGI("    rx_fifo_int_level: %x\n", hw->ctrl.rx_fifo_int_level);
	SOC_LOGI("    tx_udf_int_en:     %x\n", hw->ctrl.tx_udf_int_en);
	SOC_LOGI("    rx_ovf_int_en:     %x\n", hw->ctrl.rx_ovf_int_en);
	SOC_LOGI("    tx_fifo_int_en:    %x\n", hw->ctrl.tx_fifo_int_en);
	SOC_LOGI("    rx_fifo_int_en:    %x\n", hw->ctrl.rx_fifo_int_en);
	SOC_LOGI("    clk_rate:          %x\n", hw->ctrl.clk_rate);
	SOC_LOGI("    nssmd:             %x\n", hw->ctrl.nssmd);
	SOC_LOGI("    bit_width:         %x\n", hw->ctrl.bit_width);
	SOC_LOGI("    cpol:              %x\n", hw->ctrl.cpol);
	SOC_LOGI("    cpha:              %x\n", hw->ctrl.cpha);
	SOC_LOGI("    master_en:         %x\n", hw->ctrl.master_en);
	SOC_LOGI("    enable:            %x\n", hw->ctrl.enable);

	SOC_LOGI("\n");
	SOC_LOGI("  status=0x%x value=0x%x\n", &hw->status, hw->status.v);
	SOC_LOGI("    tx_fifo_empty:    %x\n", hw->status.tx_fifo_empty);
	SOC_LOGI("    tx_fifo_full:     %x\n", hw->status.tx_fifo_full);
	SOC_LOGI("    rx_fifo_empty:    %x\n", hw->status.rx_fifo_empty);
	SOC_LOGI("    rx_fifo_full:     %x\n", hw->status.rx_fifo_full);
	SOC_LOGI("    tx_fifo_int:      %x\n", hw->status.tx_fifo_int);
	SOC_LOGI("    rx_fifo_int:      %x\n", hw->status.rx_fifo_int);
	SOC_LOGI("    mode_fault:       %x\n", hw->status.mode_fault);
	SOC_LOGI("    tx_underflow_int: %x\n", hw->status.tx_underflow_int);
	SOC_LOGI("    rx_overflow_int:  %x\n", hw->status.rx_overflow_int);
	SOC_LOGI("    slave_select:     %x\n", hw->status.slave_select);
	SOC_LOGI("    spi_busy:         %x\n", hw->status.spi_busy);

	SOC_LOGI("\n");
	SOC_LOGI("  data=0x%x value=0x%x\n", &hw->data, hw->data.v);
	SOC_LOGI("    fifo_data:  %x\n", hw->data.fifo_data);

	SOC_LOGI("\n");
	SOC_LOGI("  slave_ctrl=0x%x value=0x%x\n", &hw->slave_ctrl, hw->slave_ctrl.v);
	SOC_LOGI("    slave_release_int_en: %x\n", hw->slave_ctrl.slave_release_int_en);
	SOC_LOGI("    slave_release_int:    %x\n", hw->slave_ctrl.slave_release_int);
}

#endif

