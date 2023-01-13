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


#include <common/bk_include.h>
#include "i2s_hal.h"
#include "i2s_ll_macro_def.h"
#include <driver/i2s_types.h>

bk_err_t i2s_hal_config(const i2s_cfg_t *config)
{
	i2s_ll_set_reg0x0_i2spcmen(config->i2s_en);
	i2s_ll_set_reg0x0_msten(config->role);
	i2s_ll_set_reg0x0_modesel(config->work_mode);
	i2s_ll_set_reg0x0_lrckrp(config->lrck_invert);
	i2s_ll_set_reg0x0_sclkinv(config->sck_invert);
	i2s_ll_set_reg0x0_lsbfirst(config->lsb_first_en);
	i2s_ll_set_reg0x0_synclen(config->sync_length);
	i2s_ll_set_reg0x0_datalen(config->data_length - 1);
	i2s_ll_set_reg0x0_pcm_dlen(config->pcm_dlength);
	i2s_ll_set_reg0x0_smpratio(config->sample_ratio);
	i2s_ll_set_reg0x0_bitratio(config->sck_ratio);

	i2s_ll_set_reg0x1_parallel_en(config->parallel_en);
	i2s_ll_set_reg0x1_lrcom_store(config->store_mode);
	i2s_ll_set_reg0x1_bitratio_h4b(config->sck_ratio_h4b);
	i2s_ll_set_reg0x1_smpratio_h2b(config->sample_ratio_h2b);
	i2s_ll_set_reg0x1_txint_level(config->txint_level);
	i2s_ll_set_reg0x1_rxint_level(config->rxint_level);

	return BK_OK;
}

bk_err_t i2s_hal_deconfig(void)
{
	i2s_ll_set_reg0x0_value(0);
	i2s_ll_set_reg0x1_value(0);
	i2s_ll_set_reg0x3_value(0);
	i2s_ll_set_reg0x4_value(0);
	i2s_ll_set_reg0x5_rx2ovf(1);
	i2s_ll_set_reg0x5_tx2udf(1);
	i2s_ll_set_reg0x6_value(0);
	i2s_ll_set_reg0x7_value(0);
	i2s_ll_set_reg0x8_value(0);

	return BK_OK;
}

bk_err_t i2s_hal_int_status_get(i2s_int_status_t *int_status)
{
	switch(int_status->channel_id)
	{
		case I2S_CHANNEL_1:
			int_status->tx_udf = (bool)i2s_ll_get_reg0x2_txudf();
			int_status->rx_ovf = (bool)i2s_ll_get_reg0x2_rxovf();
			int_status->tx_int = (bool)i2s_ll_get_reg0x2_txint();
			int_status->rx_int = (bool)i2s_ll_get_reg0x2_rxint();
			break;
		case I2S_CHANNEL_2:
			int_status->tx_udf = (bool)i2s_ll_get_reg0x5_tx2udf();
			int_status->rx_ovf = (bool)i2s_ll_get_reg0x5_rx2ovf();
			int_status->tx_int = (bool)i2s_ll_get_reg0x5_tx2int();
			int_status->rx_int = (bool)i2s_ll_get_reg0x5_rx2int();
			break;
		case I2S_CHANNEL_3:
			int_status->tx_udf = (bool)i2s_ll_get_reg0x5_tx3udf();
			int_status->rx_ovf = (bool)i2s_ll_get_reg0x5_rx3ovf();
			int_status->tx_int = (bool)i2s_ll_get_reg0x5_tx3int();
			int_status->rx_int = (bool)i2s_ll_get_reg0x5_rx3int();
			break;
/*
		case I2S_CHANNEL_4:
			int_status->tx_udf = (bool)i2s_ll_get_reg0x5_tx4udf();
			int_status->rx_ovf = (bool)i2s_ll_get_reg0x5_rx4ovf();
			int_status->tx_int = (bool)i2s_ll_get_reg0x5_tx4int();
			int_status->rx_int = (bool)i2s_ll_get_reg0x5_rx4int();
			break;
*/
		default:
			break;
	}

	return BK_OK;
}

bk_err_t i2s_hal_read_ready_get(uint32_t *read_flag)
{
	*read_flag = i2s_ll_get_reg0x2_rxfifo_rd_ready();
	return BK_OK;
}

bk_err_t i2s_hal_write_ready_get(uint32_t *write_flag)
{
	*write_flag = i2s_ll_get_reg0x2_txfifo_wr_ready();
	return BK_OK;
}

bk_err_t i2s_hal_en_set(uint32_t value)
{
	i2s_ll_set_reg0x0_i2spcmen(value);
	return BK_OK;
}

bk_err_t i2s_hal_int_set(i2s_isr_id_t int_id, uint32_t value)
{
	switch(int_id)
	{
		case I2S_ISR_CHL1_TXUDF:
			i2s_ll_set_reg0x1_txudf_en(value);
			break;
		case I2S_ISR_CHL1_RXOVF:
			i2s_ll_set_reg0x1_rxovf_en(value);
			break;

		case I2S_ISR_CHL1_TXINT:
			i2s_ll_set_reg0x1_txint_en(value);
			break;
		case I2S_ISR_CHL1_RXINT:
			i2s_ll_set_reg0x1_rxint_en(value);
			break;

		case I2S_ISR_CHL2_TXUDF:
			i2s_ll_set_reg0x4_tx2udf_en(value);
			break;
		case I2S_ISR_CHL2_RXOVF:
			i2s_ll_set_reg0x4_rx2ovf_en(value);
			break;
		case I2S_ISR_CHL2_TXINT:
			i2s_ll_set_reg0x4_tx2int_en(value);
			break;
		case I2S_ISR_CHL2_RXINT:
			i2s_ll_set_reg0x4_rx2int_en(value);
			break;

		case I2S_ISR_CHL3_TXUDF:
			i2s_ll_set_reg0x4_tx3udf_en(value);
			break;
		case I2S_ISR_CHL3_RXOVF:
			i2s_ll_set_reg0x4_rx3ovf_en(value);
			break;
		case I2S_ISR_CHL3_TXINT:
			i2s_ll_set_reg0x4_tx3int_en(value);
			break;
		case I2S_ISR_CHL3_RXINT:
			i2s_ll_set_reg0x4_rx3int_en(value);
			break;

/*
		case I2S_ISR_CHL4_TXUDF:
			i2s_ll_set_reg0x4_tx4udf_en(value);
			break;
		case I2S_ISR_CHL4_RXOVF:
			i2s_ll_set_reg0x4_rx4ovf_en(value);
			break;
		case I2S_ISR_CHL4_TXINT:
			i2s_ll_set_reg0x4_tx4int_en(value);
			break;
		case I2S_ISR_CHL4_RXINT:
			i2s_ll_set_reg0x4_rx4int_en(value);
			break;
*/
		default:
			break;
	}

	return BK_OK;
}

bk_err_t i2s_hal_role_set(uint32_t value)
{
	i2s_ll_set_reg0x0_msten(value);
	return BK_OK;
}

bk_err_t i2s_hal_work_mode_set(uint32_t value)
{
	i2s_ll_set_reg0x0_modesel(value);
	return BK_OK;
}

bk_err_t i2s_hal_lrck_invert_set(uint32_t value)
{
	i2s_ll_set_reg0x0_lrckrp(value);
	return BK_OK;
}

bk_err_t i2s_hal_sck_invert_set(uint32_t value)
{
	i2s_ll_set_reg0x0_sclkinv(value);
	return BK_OK;
}

bk_err_t i2s_hal_lsb_first_set(uint32_t value)
{
	i2s_ll_set_reg0x0_lsbfirst(value);
	return BK_OK;
}

bk_err_t i2s_hal_sync_len_set(uint32_t value)
{
	i2s_ll_set_reg0x0_synclen(value);
	return BK_OK;
}

bk_err_t i2s_hal_data_len_set(uint32_t value)
{
	i2s_ll_set_reg0x0_datalen(value);
	return BK_OK;
}

bk_err_t i2s_hal_pcm_dlen_set(uint32_t value)
{
	i2s_ll_set_reg0x0_pcm_dlen(value);
	return BK_OK;
}

bk_err_t i2s_hal_store_mode_set(uint32_t value)
{
	i2s_ll_set_reg0x1_lrcom_store(value);
	return BK_OK;
}

bk_err_t i2s_hal_rxfifo_clear(void)
{
	i2s_ll_set_reg0x1_rxfifo_clr(1);
	return BK_OK;
}

bk_err_t i2s_hal_txfifo_clear(void)
{
	i2s_ll_set_reg0x1_txfifo_clr(1);
	return BK_OK;
}

bk_err_t i2s_hal_txudf_int_clear(i2s_channel_id_t channel_id)
{
	switch (channel_id)
	{
		case I2S_CHANNEL_1:
			i2s_ll_set_reg0x2_txudf(1);
			break;
		case I2S_CHANNEL_2:
			i2s_ll_set_reg0x5_tx2udf(1);
			break;
		case I2S_CHANNEL_3:
			i2s_ll_set_reg0x5_tx3udf(1);
			break;
/*
		case I2S_CHANNEL_4:
			i2s_ll_set_reg0x5_tx4udf(1);
			break;
*/
		default:
			break;
	}

	return BK_OK;
}

bk_err_t i2s_hal_rxovf_int_clear(i2s_channel_id_t channel_id)
{
	switch (channel_id)
	{
		case I2S_CHANNEL_1:
			i2s_ll_set_reg0x2_rxovf(1);
			break;
		case I2S_CHANNEL_2:
			i2s_ll_set_reg0x5_rx2ovf(1);
			break;
		case I2S_CHANNEL_3:
			i2s_ll_set_reg0x5_rx3ovf(1);
			break;
/*
		case I2S_CHANNEL_4:
			i2s_ll_set_reg0x5_rx4ovf(1);
			break;
*/
		default:
			break;
	}

	return BK_OK;
}

bk_err_t i2s_hal_txint_level_set(uint32_t value)
{
	i2s_ll_set_reg0x1_txint_level(value);
	return BK_OK;
}

bk_err_t i2s_hal_rxint_level_set(uint32_t value)
{
	i2s_ll_set_reg0x1_rxint_level(value);
	return BK_OK;
}

bk_err_t i2s_hal_data_write(uint32_t channel_id, uint32_t value)
{
	switch(channel_id)
	{
		case 1:
			i2s_ll_set_reg0x3_i2s_dat(value);
			break;
		case 2:
			i2s_ll_set_reg0x6_value(value);
			break;
		case 3:
			i2s_ll_set_reg0x7_value(value);
			break;
/*
		case 4:
			i2s_ll_set_reg0x8_value(value);
			break;
*/
		default:
			break;
	}

	return BK_OK;
}

bk_err_t i2s_hal_data_read(uint32_t *value)
{
	*value = i2s_ll_get_reg0x3_i2s_dat();
	return BK_OK;
}

bk_err_t i2s_hal_sample_ratio_set(uint32_t value)
{
	i2s_ll_set_reg0x0_smpratio(value);
	return BK_OK;
}

bk_err_t i2s_hal_sck_ratio_set(uint32_t value)
{
	i2s_ll_set_reg0x0_bitratio(value);
	return BK_OK;
}

bk_err_t i2s_hal_sample_ratio_h2b_set(uint32_t value)
{
	i2s_ll_set_reg0x1_smpratio_h2b(value);
	return BK_OK;
}

bk_err_t i2s_hal_sck_ratio_h4b_set(uint32_t value)
{
	i2s_ll_set_reg0x1_bitratio_h4b(value);
	return BK_OK;
}

