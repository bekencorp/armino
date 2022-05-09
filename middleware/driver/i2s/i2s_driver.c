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
#include "i2s_driver.h"
#include "sys_driver.h"
#include "clock_driver.h"
#include <soc/soc.h>
#include <driver/i2s_types.h>
#include <os/os.h>
#include <os/mem.h>
#include <driver/int.h>
#include "gpio_driver.h"
#include <driver/gpio.h>
#include <modules/pm.h>

typedef struct {
	i2s_isr_t callback;
	void *param;
} i2s_callback_t;


#define I2S_RETURN_ON_NOT_INIT() do {\
		if (!s_i2s_driver_is_init) {\
			return BK_ERR_I2S_NOT_INIT;\
		}\
	} while(0)

#define I2S_RETURN_ON_INVALID_ISR_ID(isr_id) do {\
		if ((isr_id) >= SOC_I2S_ISR_NUM) {\
			return BK_ERR_I2S_ISR_ID;\
		}\
	} while(0)


static bool s_i2s_driver_is_init = false;
static i2s_callback_t s_i2s_isr[SOC_I2S_ISR_NUM] = {NULL};
//static i2s_driver_t driver_i2s;
static i2s_role_t i2s_role = I2S_ROLE_MAX;

static void i2s_isr(void);
extern void delay(int num);//TODO fix me


static void i2s_init_gpio(i2s_gpio_group_id_t id)
{
	switch(id)
	{
		case I2S_GPIO_GROUP_0:
			gpio_dev_unmap(GPIO_6);
			gpio_dev_map(GPIO_6, GPIO_DEV_I2S1_CLK);
			gpio_dev_unmap(GPIO_7);
			gpio_dev_map(GPIO_7, GPIO_DEV_I2S1_SYNC);
			gpio_dev_unmap(GPIO_8);
			gpio_dev_map(GPIO_8, GPIO_DEV_I2S1_DIN);
			gpio_dev_unmap(GPIO_9);
			gpio_dev_map(GPIO_9, GPIO_DEV_I2S1_DOUT);
			gpio_dev_unmap(GPIO_28);
			gpio_dev_map(GPIO_28, GPIO_DEV_I2S1_MCLK);
			bk_gpio_disable_output(GPIO_28);
			break;

		case I2S_GPIO_GROUP_1:
			gpio_dev_unmap(GPIO_40);
			gpio_dev_map(GPIO_40, GPIO_DEV_I2S1_CLK);
			bk_gpio_disable_output(GPIO_40);
			gpio_dev_unmap(GPIO_41);
			gpio_dev_map(GPIO_41, GPIO_DEV_I2S1_SYNC);
			bk_gpio_disable_output(GPIO_41);
			gpio_dev_unmap(GPIO_42);
			gpio_dev_map(GPIO_42, GPIO_DEV_I2S1_DIN);
			bk_gpio_disable_output(GPIO_42);
			gpio_dev_unmap(GPIO_43);
			gpio_dev_map(GPIO_43, GPIO_DEV_I2S1_DOUT);
			bk_gpio_disable_output(GPIO_43);
			gpio_dev_unmap(GPIO_28);
			gpio_dev_map(GPIO_28, GPIO_DEV_I2S1_MCLK);
			bk_gpio_disable_output(GPIO_28);
			break;

		default:
			break;
	}
}

bk_err_t bk_i2s_driver_init(void)
{
	if (s_i2s_driver_is_init)
		return BK_OK;

	//power on
	pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_AUDP, PM_POWER_MODULE_STATE_ON);
	//sys_drv_aud_power_en(0);    //temp used

	//select 26M XTAL clock and enable i2s clock
	sys_drv_i2s_select_clock(1);
	sys_drv_i2s_clock_en(1);

	//i2s_disckg always on
	sys_drv_i2s_disckg_set(1);

	//set apll clock config
	sys_drv_apll_en(1);
	sys_drv_cb_manu_val_set(0x14);
	sys_drv_ana_reg11_vsel_set(7);

	//enable i2s interrupt
	sys_drv_i2s_int_en(1);

	os_memset(&s_i2s_isr, 0, sizeof(s_i2s_isr));

	//register fft isr
	i2s_int_config_t int_config_table = {INT_SRC_I2S, i2s_isr};
	bk_int_isr_register(int_config_table.int_src, int_config_table.isr, NULL);
	s_i2s_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_i2s_driver_deinit(void)
{
	//power down
	//pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_AUDP, PM_POWER_MODULE_STATE_OFF);
	sys_drv_i2s_clock_en(0);

	//i2s_disckg not always on
	sys_drv_i2s_disckg_set(0);
	//disable i2s interrupt
	sys_drv_i2s_int_en(0);

	//set apll clock config
	sys_drv_apll_en(0);
	sys_drv_cb_manu_val_set(0x10);
	sys_drv_ana_reg11_vsel_set(5);

	i2s_int_config_t int_config_table = {INT_SRC_I2S, i2s_isr};
	bk_int_isr_unregister(int_config_table.int_src);
	s_i2s_driver_is_init = false;

	return BK_OK;
}

bk_err_t bk_i2s_init(i2s_gpio_group_id_t id, const i2s_config_t *config)
{
	I2S_RETURN_ON_NOT_INIT();
	if (!config)
		return BK_ERR_I2S_PARAM;

	i2s_init_gpio(id);
	i2s_hal_config(config);
	i2s_role = config->role;

	return BK_OK;
}

bk_err_t bk_i2s_deinit(void)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_deconfig();
	i2s_role = I2S_ROLE_MAX;

	return BK_OK;
}

bk_err_t bk_i2s_get_read_ready(uint32_t *read_flag)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_read_ready_get(read_flag);
	return BK_OK;
}

bk_err_t bk_i2s_get_write_ready(uint32_t *write_flag)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_write_ready_get(write_flag);
	return BK_OK;
}

bk_err_t bk_i2s_enable(i2s_en_t en_value)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_en_set(en_value);
	return BK_OK;
}

bk_err_t bk_i2s_int_enable(i2s_isr_id_t int_id, uint32_t value)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_int_set(int_id, value);
	return BK_OK;
}

bk_err_t bk_i2s_set_role(i2s_role_t role)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_role_set(role);
	return BK_OK;
}

bk_err_t bk_i2s_set_work_mode(i2s_work_mode_t work_mode)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_work_mode_set(work_mode);
	return BK_OK;
}

bk_err_t bk_i2s_set_lrck_invert(i2s_lrck_invert_en_t lrckrp)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_lrck_invert_set(lrckrp);
	return BK_OK;
}

bk_err_t bk_i2s_set_sck_invert(i2s_sck_invert_en_t sck_invert)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_sck_invert_set(sck_invert);
	return BK_OK;
}

bk_err_t bk_i2s_set_lsb_first(i2s_lsb_first_en_t lsb_first)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_lsb_first_set(lsb_first);
	return BK_OK;
}

bk_err_t bk_i2s_set_sync_len(uint32_t sync_len)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_sync_len_set(sync_len);
	return BK_OK;
}

bk_err_t bk_i2s_set_data_len(uint32_t data_len)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_data_len_set(data_len);
	return BK_OK;
}

bk_err_t bk_i2s_set_pcm_dlen(uint32_t pcm_dlen)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_pcm_dlen_set(pcm_dlen);
	return BK_OK;
}

bk_err_t bk_i2s_set_store_mode(i2s_lrcom_store_mode_t store_mode)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_store_mode_set(store_mode);
	return BK_OK;
}

bk_err_t bk_i2s_clear_rxfifo(void)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_rxfifo_clear();
	return BK_OK;
}

bk_err_t bk_i2s_clear_txfifo(void)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_txfifo_clear();
	return BK_OK;
}

bk_err_t bk_i2s_clear_txudf_int(i2s_channel_id_t channel_id)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_txudf_int_clear(channel_id);
	return BK_OK;
}

bk_err_t bk_i2s_clear_rxovf_int(i2s_channel_id_t channel_id)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_rxovf_int_clear(channel_id);
	return BK_OK;
}

bk_err_t bk_i2s_set_txint_level(i2s_txint_level_t txint_level)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_txint_level_set(txint_level);
	return BK_OK;
}

bk_err_t bk_i2s_set_rxint_level(i2s_rxint_level_t rxint_level)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_rxint_level_set(rxint_level);
	return BK_OK;
}

bk_err_t bk_i2s_write_data(uint32_t channel_id, uint32_t *data_buf, uint32_t data_len)
{
	uint32_t i = 0;
	I2S_RETURN_ON_NOT_INIT();

	for (i=0; i<data_len; i++)
		i2s_hal_data_write(channel_id, data_buf[i]);
	return BK_OK;
}

bk_err_t bk_i2s_read_data(uint32_t *data_buf, uint32_t data_len)
{
	uint32_t i = 0;
	I2S_RETURN_ON_NOT_INIT();

	for (i=0; i<data_len; i++)
		i2s_hal_data_read(&data_buf[i]);
	return BK_OK;
}

bk_err_t bk_i2s_get_data_addr(uint32_t *i2s_data_addr)
{
	I2S_RETURN_ON_NOT_INIT();

	*i2s_data_addr = I2S_REG0X3_ADDR;
	return BK_OK;
}

bk_err_t bk_i2s_set_ratio(i2s_rate_t *rate)
{
	//SOC_I2S_APLL_RATE
	uint32_t i = 0;
	uint32_t smp_ratio = 0;
	uint32_t bit_ratio = 0;
	uint32_t apll_clk = 0;

	const i2s_rate_table_node_t i2s_rate_table [] = {
		//8000
		{I2S_SAMP_RATE_8000, I2S_DATA_WIDTH_16, 98304000, 15, 192},
		{I2S_SAMP_RATE_8000, I2S_DATA_WIDTH_24, 98304000, 23, 128},
		{I2S_SAMP_RATE_8000, I2S_DATA_WIDTH_32, 98304000, 31, 96},

		//12000
		{I2S_SAMP_RATE_12000, I2S_DATA_WIDTH_16, 98304000, 15, 128},
		{I2S_SAMP_RATE_12000, I2S_DATA_WIDTH_32, 98304000, 31, 64},

		//16000
		{I2S_SAMP_RATE_16000, I2S_DATA_WIDTH_16, 98304000, 15, 96},
		{I2S_SAMP_RATE_16000, I2S_DATA_WIDTH_24, 98304000, 23, 64},
		{I2S_SAMP_RATE_16000, I2S_DATA_WIDTH_32, 98304000, 31, 48},

		//24000
		{I2S_SAMP_RATE_24000, I2S_DATA_WIDTH_16, 98304000, 15, 64},
		{I2S_SAMP_RATE_24000, I2S_DATA_WIDTH_32, 98304000, 31, 32},

		//32000
		{I2S_SAMP_RATE_32000, I2S_DATA_WIDTH_16, 98304000, 15, 48},
		{I2S_SAMP_RATE_32000, I2S_DATA_WIDTH_24, 98304000, 23, 32},
		{I2S_SAMP_RATE_32000, I2S_DATA_WIDTH_32, 98304000, 31, 24},

		//48000
		{I2S_SAMP_RATE_48000, I2S_DATA_WIDTH_16, 98304000, 15, 32},
		{I2S_SAMP_RATE_48000, I2S_DATA_WIDTH_32, 98304000, 31, 16},

		//96000
		{I2S_SAMP_RATE_96000, I2S_DATA_WIDTH_16, 98304000, 15, 16},
		{I2S_SAMP_RATE_96000, I2S_DATA_WIDTH_32, 98304000, 31, 8},

		//8018.2
		{I2S_SAMP_RATE_8018, I2S_DATA_WIDTH_16, 90316800, 15, 176},
		{I2S_SAMP_RATE_8018, I2S_DATA_WIDTH_32, 90316800, 31, 88},

		//11025
		{I2S_SAMP_RATE_11025, I2S_DATA_WIDTH_16, 90316800, 15, 128},
		{I2S_SAMP_RATE_11025, I2S_DATA_WIDTH_32, 90316800, 31, 64},

		//22050
		{I2S_SAMP_RATE_22050, I2S_DATA_WIDTH_16, 90316800, 15, 64},
		{I2S_SAMP_RATE_22050, I2S_DATA_WIDTH_32, 90316800, 31, 32},

		//44100
		{I2S_SAMP_RATE_44100, I2S_DATA_WIDTH_16, 90316800, 15, 32},
		{I2S_SAMP_RATE_44100, I2S_DATA_WIDTH_32, 90316800, 31, 16},

		//88200
		{I2S_SAMP_RATE_88200, I2S_DATA_WIDTH_16, 90316800, 15, 16},
		{I2S_SAMP_RATE_88200, I2S_DATA_WIDTH_32, 90316800, 31, 8},
	};

	I2S_RETURN_ON_NOT_INIT();

	for (i = 0; i < sizeof(i2s_rate_table)/sizeof(i2s_rate_table_node_t); i++)
	{
		if ((rate->samp_rate == i2s_rate_table[i].samp_rate) && (rate->datawidth == i2s_rate_table[i].datawidth)) {
			smp_ratio = i2s_rate_table[i].smp_ratio;
			bit_ratio = i2s_rate_table[i].bit_ratio;
			apll_clk = i2s_rate_table[i].sys_clk;
			break;
		}
	}

	if (smp_ratio == 0) {
		I2S_LOGI("cannot find ratio value to use, please check rate parameters \r\n");
		return BK_ERR_I2S_PARAM;
	}

	if (apll_clk == 98304000)
		sys_drv_ana_reg10_sdm_val_set(0xF1FAA45);
	else if (apll_clk == 90316800)
		sys_drv_ana_reg10_sdm_val_set(0xDE517A9);
	else
		sys_drv_ana_reg10_sdm_val_set(0xF1FAA45);
	sys_drv_ana_reg11_spi_trigger_set(1);
	delay(10);
	sys_drv_ana_reg11_spi_trigger_set(0);

	if (smp_ratio > 0x1F) {
		i2s_hal_sample_ratio_set(smp_ratio & 0x1F);
		i2s_hal_sample_ratio_h2b_set((smp_ratio >> 5) & 0x3);
	} else {
		i2s_hal_sample_ratio_set(smp_ratio);
	}
	if (bit_ratio > 0xFF) {
		i2s_hal_sck_ratio_set(bit_ratio & 0xFF);
		i2s_hal_sck_ratio_h4b_set((bit_ratio >> 8) & 0xF);
	} else {
		i2s_hal_sck_ratio_set(bit_ratio);
	}

	return BK_OK;
}

/* register i2s interrupt */
bk_err_t bk_i2s_register_i2s_isr(i2s_isr_id_t isr_id, i2s_isr_t isr, void *param)
{
	I2S_RETURN_ON_NOT_INIT();
	I2S_RETURN_ON_INVALID_ISR_ID(isr_id);
	uint32_t int_level = rtos_disable_int();
	s_i2s_isr[isr_id].callback = isr;
	s_i2s_isr[isr_id].param = param;
	rtos_enable_int(int_level);

	return BK_OK;
}

void i2s_isr(void)
{
	i2s_int_status_t i2s_status = {0};
	i2s_status.channel_id = I2S_CHANNEL_1;
	i2s_hal_int_status_get(&i2s_status);

	if (i2s_status.tx_udf) {
		i2s_hal_txudf_int_clear(I2S_CHANNEL_1);
		if (s_i2s_isr[I2S_ISR_CHL1_TXUDF].callback) {
			s_i2s_isr[I2S_ISR_CHL1_TXUDF].callback(s_i2s_isr[I2S_ISR_CHL1_TXUDF].param);
		}
	}

	if (i2s_status.rx_ovf) {
		i2s_hal_rxovf_int_clear(I2S_CHANNEL_1);
		if (s_i2s_isr[I2S_ISR_CHL1_RXOVF].callback) {
			s_i2s_isr[I2S_ISR_CHL1_RXOVF].callback(s_i2s_isr[I2S_ISR_CHL1_RXOVF].param);
		}
	}

	if (i2s_status.tx_int) {
		if (s_i2s_isr[I2S_ISR_CHL1_TXINT].callback) {
			s_i2s_isr[I2S_ISR_CHL1_TXINT].callback(s_i2s_isr[I2S_ISR_CHL1_TXINT].param);
		}
	}

	if (i2s_status.rx_int) {
		if (s_i2s_isr[I2S_ISR_CHL1_RXINT].callback) {
			s_i2s_isr[I2S_ISR_CHL1_RXINT].callback(s_i2s_isr[I2S_ISR_CHL1_RXINT].param);
		}
	}

	os_memset(&i2s_status, 0, sizeof(i2s_status));
	i2s_status.channel_id = I2S_CHANNEL_2;
	i2s_hal_int_status_get(&i2s_status);
	if (i2s_status.tx_udf) {
		i2s_hal_txudf_int_clear(I2S_CHANNEL_2);
		if (s_i2s_isr[I2S_ISR_CHL2_TXUDF].callback) {
			s_i2s_isr[I2S_ISR_CHL2_TXUDF].callback(s_i2s_isr[I2S_ISR_CHL2_TXUDF].param);
		}
	}

	if (i2s_status.rx_ovf) {
		i2s_hal_rxovf_int_clear(I2S_CHANNEL_2);
		if (s_i2s_isr[I2S_ISR_CHL2_RXOVF].callback) {
			s_i2s_isr[I2S_ISR_CHL2_RXOVF].callback(s_i2s_isr[I2S_ISR_CHL2_RXOVF].param);
		}
	}

	if (i2s_status.tx_int) {
		if (s_i2s_isr[I2S_ISR_CHL2_TXINT].callback) {
			s_i2s_isr[I2S_ISR_CHL2_TXINT].callback(s_i2s_isr[I2S_ISR_CHL2_TXINT].param);
		}
	}

	if (i2s_status.rx_int) {
		if (s_i2s_isr[I2S_ISR_CHL2_RXINT].callback) {
			s_i2s_isr[I2S_ISR_CHL2_RXINT].callback(s_i2s_isr[I2S_ISR_CHL2_RXINT].param);
		}
	}

	os_memset(&i2s_status, 0, sizeof(i2s_status));
	i2s_status.channel_id = I2S_CHANNEL_3;
	i2s_hal_int_status_get(&i2s_status);
	if (i2s_status.tx_udf) {
		i2s_hal_txudf_int_clear(I2S_CHANNEL_3);
		if (s_i2s_isr[I2S_ISR_CHL3_TXUDF].callback) {
			s_i2s_isr[I2S_ISR_CHL3_TXUDF].callback(s_i2s_isr[I2S_ISR_CHL3_TXUDF].param);
		}
	}

	if (i2s_status.rx_ovf) {
		i2s_hal_rxovf_int_clear(I2S_CHANNEL_3);
		if (s_i2s_isr[I2S_ISR_CHL3_RXOVF].callback) {
			s_i2s_isr[I2S_ISR_CHL3_RXOVF].callback(s_i2s_isr[I2S_ISR_CHL3_RXOVF].param);
		}
	}

	if (i2s_status.tx_int) {
		if (s_i2s_isr[I2S_ISR_CHL3_TXINT].callback) {
			s_i2s_isr[I2S_ISR_CHL3_TXINT].callback(s_i2s_isr[I2S_ISR_CHL3_TXINT].param);
		}
	}

	if (i2s_status.rx_int) {
		if (s_i2s_isr[I2S_ISR_CHL3_RXINT].callback) {
			s_i2s_isr[I2S_ISR_CHL3_RXINT].callback(s_i2s_isr[I2S_ISR_CHL3_RXINT].param);
		}
	}

	os_memset(&i2s_status, 0, sizeof(i2s_status));
	i2s_status.channel_id = I2S_CHANNEL_4;
	i2s_hal_int_status_get(&i2s_status);
	if (i2s_status.tx_udf) {
		i2s_hal_txudf_int_clear(I2S_CHANNEL_4);
		if (s_i2s_isr[I2S_ISR_CHL4_TXUDF].callback) {
			s_i2s_isr[I2S_ISR_CHL4_TXUDF].callback(s_i2s_isr[I2S_ISR_CHL4_TXUDF].param);
		}
	}

	if (i2s_status.rx_ovf) {
		i2s_hal_rxovf_int_clear(I2S_CHANNEL_4);
		if (s_i2s_isr[I2S_ISR_CHL4_RXOVF].callback) {
			s_i2s_isr[I2S_ISR_CHL4_RXOVF].callback(s_i2s_isr[I2S_ISR_CHL4_RXOVF].param);
		}
	}

	if (i2s_status.tx_int) {
		if (s_i2s_isr[I2S_ISR_CHL4_TXINT].callback) {
			s_i2s_isr[I2S_ISR_CHL4_TXINT].callback(s_i2s_isr[I2S_ISR_CHL4_TXINT].param);
		}
	}

	if (i2s_status.rx_int) {
		if (s_i2s_isr[I2S_ISR_CHL4_RXINT].callback) {
			s_i2s_isr[I2S_ISR_CHL4_RXINT].callback(s_i2s_isr[I2S_ISR_CHL4_RXINT].param);
		}
	}

}

