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
#include "i2s_ll_macro_def.h"

#if CFG_HAL_DEBUG_I2S

void i2s_struct_dump(void)
{
	SOC_LOGI("\r\nbase=%x\r\n", (uint32_t)I2S_LL_REG_BASE);

	/* REG_0x00 */
	SOC_LOGI("  PCM_CTRL=0x%x value=0x%x\n", I2S_REG0X0_ADDR, i2s_ll_get_reg0x0_value());
	SOC_LOGI("    I2SPCMEN:  %x\n", i2s_ll_get_reg0x0_i2spcmen());
	SOC_LOGI("    MSTEN:     %x\n", i2s_ll_get_reg0x0_msten());
	SOC_LOGI("    MODESEL:   %x\n", i2s_ll_get_reg0x0_modesel());
	SOC_LOGI("    LRCKRP:    %x\n", i2s_ll_get_reg0x0_lrckrp());
	SOC_LOGI("    SCLKINV:   %x\n", i2s_ll_get_reg0x0_sclkinv());
	SOC_LOGI("    LSBFIRST:  %x\n", i2s_ll_get_reg0x0_lsbfirst());
	SOC_LOGI("    SYNCLEN:   %x\n", i2s_ll_get_reg0x0_synclen());
	SOC_LOGI("    DATALEN:   %x\n", i2s_ll_get_reg0x0_datalen());
	SOC_LOGI("    PCM_DLEN:  %x\n", i2s_ll_get_reg0x0_pcm_dlen());
	SOC_LOGI("    SMPRATIO:  %x\n", i2s_ll_get_reg0x0_smpratio());
	SOC_LOGI("    BITRATIO:  %x\n", i2s_ll_get_reg0x0_bitratio());

	/* REG_0x01 */
	SOC_LOGI("\n");
	SOC_LOGI("  PCM_CN=0x%x value=0x%x\n", I2S_REG0X1_ADDR, i2s_ll_get_reg0x1_value());
	SOC_LOGI("    PARALLEL_EN:   %x\n", i2s_ll_get_reg0x1_parallel_en());
	SOC_LOGI("    LRCOM_STORE:   %x\n", i2s_ll_get_reg0x1_lrcom_store());
	SOC_LOGI("    BITRATIO_H4B:  %x\n", i2s_ll_get_reg0x1_bitratio_h4b());
	SOC_LOGI("    SMPRATIO_H2B:  %x\n", i2s_ll_get_reg0x1_smpratio_h2b());
	SOC_LOGI("    RXFIFO_CLR:    %x\n", i2s_ll_get_reg0x1_rxfifo_clr());
	SOC_LOGI("    TXFIFO_CLR:    %x\n", i2s_ll_get_reg0x1_txfifo_clr());
	SOC_LOGI("    TXINT_LEVEL:   %x\n", i2s_ll_get_reg0x1_txint_level());
	SOC_LOGI("    RXINT_LEVEL:   %x\n", i2s_ll_get_reg0x1_rxint_level());
	SOC_LOGI("    TXUDF_EN:      %x\n", i2s_ll_get_reg0x1_txudf_en());
	SOC_LOGI("    RXOVF_EN:      %x\n", i2s_ll_get_reg0x1_rxovf_en());
	SOC_LOGI("    TXINT_EN:      %x\n", i2s_ll_get_reg0x1_txint_en());
	SOC_LOGI("    RXINT_EN:      %x\n", i2s_ll_get_reg0x1_rxint_en());

	/* REG_0x02 */
	SOC_LOGI("\n");
	SOC_LOGI("  PCM_STAT=0x%x value=0x%x\n", I2S_REG0X2_ADDR, i2s_ll_get_reg0x2_value());
	SOC_LOGI("    TXFIFO_WR_READY:   %x\n", i2s_ll_get_reg0x2_txfifo_wr_ready());
	SOC_LOGI("    RXFIFO_RD_READY:   %x\n", i2s_ll_get_reg0x2_rxfifo_rd_ready());
	SOC_LOGI("    TXUDF: %x\n", i2s_ll_get_reg0x2_txudf());
	SOC_LOGI("    RXOVF: %x\n", i2s_ll_get_reg0x2_rxovf());
	SOC_LOGI("    TXINT: %x\n", i2s_ll_get_reg0x2_txint());
	SOC_LOGI("    RXINT: %x\n", i2s_ll_get_reg0x2_rxint());

	/* REG_0x03 */
	SOC_LOGI("\n");
	SOC_LOGI("  PCM_DAT=0x%x value=0x%x\n", I2S_REG0X3_ADDR, i2s_ll_get_reg0x3_value());

	/* REG_0x04 */
	SOC_LOGI("\n");
	SOC_LOGI("  PCM_CN_LT2=0x%x value=0x%x\n", I2S_REG0X4_ADDR, i2s_ll_get_reg0x4_value());
	SOC_LOGI("    TX4UDF_EN:   %x\n", i2s_ll_get_reg0x4_tx4udf_en());
	SOC_LOGI("    RX4OVF_EN:   %x\n", i2s_ll_get_reg0x4_rx4ovf_en());
	SOC_LOGI("    TX4NT_EN:  %x\n", i2s_ll_get_reg0x4_tx4int_en());
	SOC_LOGI("    RX4INT_EN:  %x\n", i2s_ll_get_reg0x4_rx4int_en());
	SOC_LOGI("    TX3UDF_EN:    %x\n", i2s_ll_get_reg0x4_tx3udf_en());
	SOC_LOGI("    RX3OVF_EN:    %x\n", i2s_ll_get_reg0x4_rx3ovf_en());
	SOC_LOGI("    TX3NT_EN:   %x\n", i2s_ll_get_reg0x4_tx3int_en());
	SOC_LOGI("    RX3INT_EN:   %x\n", i2s_ll_get_reg0x4_rx3int_en());
	SOC_LOGI("    TX2UDF_EN:      %x\n", i2s_ll_get_reg0x4_tx2udf_en());
	SOC_LOGI("    RX2OVF_EN:      %x\n", i2s_ll_get_reg0x4_rx2ovf_en());
	SOC_LOGI("    TX2INT_EN:      %x\n", i2s_ll_get_reg0x4_tx2int_en());
	SOC_LOGI("    RX2INT_EN:      %x\n", i2s_ll_get_reg0x4_rx2int_en());

	/* REG_0x05 */
	SOC_LOGI("\n");
	SOC_LOGI("  PCM_STAT_LT2=0x%x value=0x%x\n", I2S_REG0X5_ADDR, i2s_ll_get_reg0x5_value());
	SOC_LOGI("    TX4UDF:   %x\n", i2s_ll_get_reg0x5_tx4udf());
	SOC_LOGI("    RX4OVF:   %x\n", i2s_ll_get_reg0x5_rx4ovf());
	SOC_LOGI("    TX4INT:  %x\n", i2s_ll_get_reg0x5_tx4int());
	SOC_LOGI("    RX4INT:  %x\n", i2s_ll_get_reg0x5_rx4int());
	SOC_LOGI("    TX3UDF:    %x\n", i2s_ll_get_reg0x5_tx3udf());
	SOC_LOGI("    RX3OVF:    %x\n", i2s_ll_get_reg0x5_rx3ovf());
	SOC_LOGI("    TX3INT:   %x\n", i2s_ll_get_reg0x5_tx3int());
	SOC_LOGI("    RX3INT:   %x\n", i2s_ll_get_reg0x5_rx3int());
	SOC_LOGI("    TX2UDF:      %x\n", i2s_ll_get_reg0x5_tx2udf());
	SOC_LOGI("    RX2OVF:      %x\n", i2s_ll_get_reg0x5_rx2ovf());
	SOC_LOGI("    TX2INT:      %x\n", i2s_ll_get_reg0x5_tx2int());
	SOC_LOGI("    RX2INT:      %x\n", i2s_ll_get_reg0x5_rx2int());

}

#endif

