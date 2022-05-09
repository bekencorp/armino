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

#include <os/os.h>
#include <os/mem.h>
#include <os/str.h>
#include <driver/i2s.h>
#include <driver/i2s_types.h>
#include <driver/dma.h>
#include "i2s_hal.h"
#include "sys_driver.h"
#include "i2s_driver.h"
#include <driver/aud.h>
#include <driver/aud_types.h>
#include <driver/dma.h>
#include "aud_hal.h"


#define I2S_TEST_DATA_SIZE  256
uint32_t data_source[I2S_TEST_DATA_SIZE] = {
	0x7fff0000, 0x7c760000, 0x720c0000, 0x61540000, 0x4b3c0000, 0x30fb0000, 0x14060000, 0xf5f50000,
	0xd8720000, 0xbd1f0000, 0xa57e0000, 0x92de0000, 0x86450000, 0x80660000, 0x81940000, 0x89bf0000,
	0x98730000, 0xace00000, 0xc5e40000, 0xe21f0000, 0x00000000, 0x1de10000, 0x3a1c0000, 0x53200000,
	0x678d0000, 0x76410000, 0x7e6c0000, 0x7f9a0000, 0x79bb0000, 0x6d220000, 0x5a820000, 0x42e10000,
	0x278e0000, 0x0a0b0000, 0xebfa0000, 0xcf050000, 0xb4c40000, 0x9eac0000, 0x8df40000, 0x838a0000,
	0x80010000, 0x838a0000, 0x8df40000, 0x9eac0000, 0xb4c40000, 0xcf050000, 0xebfa0000, 0x0a0b0000,
	0x278e0000, 0x42e10000, 0x5a820000, 0x6d220000, 0x79bb0000, 0x7f9a0000, 0x7e6c0000, 0x76410000,
	0x678d0000, 0x53200000, 0x3a1c0000, 0x1de10000, 0x00000000, 0xe21f0000, 0xc5e40000, 0xace00000,
	0x98730000, 0x89bf0000, 0x81940000, 0x80660000, 0x86450000, 0x92de0000, 0xa57e0000, 0xbd1f0000,
	0xd8720000, 0xf5f50000, 0x14060000, 0x30fb0000, 0x4b3c0000, 0x61540000, 0x720c0000, 0x7c760000,
	0x7fff0000, 0x7c760000, 0x720c0000, 0x61540000, 0x4b3c0000, 0x30fb0000, 0x14060000, 0xf5f50000,
	0xd8720000, 0xbd1f0000, 0xa57e0000, 0x92de0000, 0x86450000, 0x80660000, 0x81940000, 0x89bf0000,
	0x98730000, 0xace00000, 0xc5e40000, 0xe21f0000, 0x00000000, 0x1de10000, 0x3a1c0000, 0x53200000,
	0x678d0000, 0x76410000, 0x7e6c0000, 0x7f9a0000, 0x79bb0000, 0x6d220000, 0x5a820000, 0x42e10000,
	0x278e0000, 0x0a0b0000, 0xebfa0000, 0xcf050000, 0xb4c40000, 0x9eac0000, 0x8df40000, 0x838a0000,
	0x80010000, 0x838a0000, 0x8df40000, 0x9eac0000, 0xb4c40000, 0xcf050000, 0xebfa0000, 0x0a0b0000,
	0x278e0000, 0x42e10000, 0x5a820000, 0x6d220000, 0x79bb0000, 0x7f9a0000, 0x7e6c0000, 0x76410000,
	0x678d0000, 0x53200000, 0x3a1c0000, 0x1de10000, 0x00000000, 0xe21f0000, 0xc5e40000, 0xace00000,
	0x98730000, 0x89bf0000, 0x81940000, 0x80660000, 0x86450000, 0x92de0000, 0xa57e0000, 0xbd1f0000,
	0xd8720000, 0xf5f50000, 0x14060000, 0x30fb0000, 0x4b3c0000, 0x61540000, 0x720c0000, 0x7c760000,
	0x7fff0000, 0x7c760000, 0x720c0000, 0x61540000, 0x4b3c0000, 0x30fb0000, 0x14060000, 0xf5f50000,
	0xd8720000, 0xbd1f0000, 0xa57e0000, 0x92de0000, 0x86450000, 0x80660000, 0x81940000, 0x89bf0000,
	0x98730000, 0xace00000, 0xc5e40000, 0xe21f0000, 0x00000000, 0x1de10000, 0x3a1c0000, 0x53200000,
	0x678d0000, 0x76410000, 0x7e6c0000, 0x7f9a0000, 0x79bb0000, 0x6d220000, 0x5a820000, 0x42e10000,
	0x278e0000, 0x0a0b0000, 0xebfa0000, 0xcf050000, 0xb4c40000, 0x9eac0000, 0x8df40000, 0x838a0000,
	0x80010000, 0x838a0000, 0x8df40000, 0x9eac0000, 0xb4c40000, 0xcf050000, 0xebfa0000, 0x0a0b0000,
	0x278e0000, 0x42e10000, 0x5a820000, 0x6d220000, 0x79bb0000, 0x7f9a0000, 0x7e6c0000, 0x76410000,
	0x678d0000, 0x53200000, 0x3a1c0000, 0x1de10000, 0x00000000, 0xe21f0000, 0xc5e40000, 0xace00000,
	0x98730000, 0x89bf0000, 0x81940000, 0x80660000, 0x86450000, 0x92de0000, 0xa57e0000, 0xbd1f0000,
	0xd8720000, 0xf5f50000, 0x14060000, 0x30fb0000, 0x4b3c0000, 0x61540000, 0x720c0000, 0x7c760000,
	0x7fff0000, 0x7c760000, 0x720c0000, 0x61540000, 0x4b3c0000, 0x30fb0000, 0x14060000, 0xf5f50000,
	0xd8720000, 0xbd1f0000, 0xa57e0000, 0x92de0000, 0x86450000, 0x80660000, 0x81940000, 0x89bf0000
} ;

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


typedef enum {
	I2S_IDLE = 0,
	I2S_TESTING,
	I2S_STOP
} i2s_status_t;

typedef struct {
	i2s_status_t test_status;
	i2s_work_mode_t work_mode;
	i2s_lrck_invert_en_t lrck_invert;
	i2s_sck_invert_en_t sck_invert;
	i2s_lsb_first_en_t lsb_first;
	i2s_lrcom_store_mode_t store_mode;
	i2s_role_t role;
	uint32_t test_case_index;
} i2s_test_t;

static i2s_test_t i2s_test_global = {0};
extern void delay(int num);//TODO fix me

static void cli_i2s_help(void)
{
	os_printf("i2s_master_test {start|stop} \r\n");
	os_printf("i2s_master_sin_test {start|stop} \r\n");
	os_printf("i2s_master_mic_test {start|stop} \r\n");
	os_printf("i2s_slave_test {start|stop} \r\n");
	os_printf("i2s_slave_mic_test {start|stop} \r\n");
}

static void cli_i2s_master_txudf_isr(void *param)
{
	//os_printf("enter cli_i2s_txudf_isr \r\n");
}

static void cli_i2s_master_rxovf_isr(void *param)
{
	//os_printf("enter cli_i2s_rxovf_isr \r\n");
	//change test_status: testing-->stop
	if (i2s_test_global.test_status == I2S_TESTING) {
		//os_printf("enter cli_i2s_master_rxovf_isr \r\n");
		i2s_test_global.test_status = I2S_STOP;
		//disable i2s rxovf interrupt
		bk_i2s_int_enable(I2S_ISR_CHL1_RXOVF, I2S_INT_DISABLE);
	}
}

static void cli_i2s_master_txint_isr(void *param)
{
	//os_printf("enter cli_i2s_txint_isr \r\n");
}

static void cli_i2s_master_rxint_isr(void *param)
{
	uint32_t write_flag = 0;
	uint32_t data_buf_rl = 0x12345678;

	//os_printf("enter cli_i2s_master_rxint_isr \r\n");
	bk_i2s_get_write_ready(&write_flag);
	if (write_flag)
		bk_i2s_write_data(1, &data_buf_rl, 1);
}

static void cli_i2s_slave_txudf_isr(void *param)
{
	//os_printf("enter cli_i2s_txudf_isr \r\n");
}

static void cli_i2s_slave_rxovf_isr(void *param)
{
	//os_printf("enter cli_i2s_slave_rxovf_isr \r\n");
	//change test_status: testing-->stop
	if (i2s_test_global.test_status == I2S_TESTING) {
		//os_printf("enter cli_i2s_slave_rxovf_isr \r\n");
		i2s_test_global.test_status = I2S_STOP;
		//disable i2s rxovf interrupt
		bk_i2s_int_enable(I2S_ISR_CHL1_RXOVF, I2S_INT_DISABLE);
	}
}

static void cli_i2s_slave_txint_isr(void *param)
{
	//os_printf("enter cli_i2s_txint_isr \r\n");
}

static void cli_i2s_slave_rxint_isr(void *param)
{
	uint32_t write_flag = 0;
	uint32_t data_buf_rl = 0x87654321;

	//os_printf("enter cli_i2s_slave_rxint_isr \r\n");
	bk_i2s_get_write_ready(&write_flag);
	if (write_flag)
		bk_i2s_write_data(1, &data_buf_rl, 1);
}

static void test_log_print(i2s_rate_table_node_t rate_node)
{
	switch (rate_node.samp_rate)
	{
		case I2S_SAMP_RATE_8000:
			os_printf("========= CASE sample_rate: 8K, data_lenght: %d ============\r\n", rate_node.datawidth);
			break;

		case I2S_SAMP_RATE_12000:
			os_printf("========= CASE sample_rate: 12K, data_lenght: %d ============\r\n", rate_node.datawidth);
			break;

		case I2S_SAMP_RATE_16000:
			os_printf("========= CASE sample_rate: 16K, data_lenght: %d ============\r\n", rate_node.datawidth);
			break;

		case I2S_SAMP_RATE_24000:
			os_printf("========= CASE sample_rate: 24K, data_lenght: %d ============\r\n", rate_node.datawidth);
			break;

		case I2S_SAMP_RATE_32000:
			os_printf("========= CASE sample_rate: 32K, data_lenght: %d =============\r\n", rate_node.datawidth);
			break;

		case I2S_SAMP_RATE_48000:
			os_printf("========= CASE sample_rate: 48K, data_lenght: %d =============\r\n", rate_node.datawidth);
			break;

		case I2S_SAMP_RATE_96000:
			os_printf("========= CASE sample_rate: 96K, data_lenght: %d ==============\r\n", rate_node.datawidth);
			break;

		case I2S_SAMP_RATE_8018:
			os_printf("========= CASE sample_rate: 8.0182K, data_lenght: %d ==========\r\n", rate_node.datawidth);
			break;

		case I2S_SAMP_RATE_11025:
			os_printf("========= CASE sample_rate: 11.025K, data_lenght: %d ===========\r\n", rate_node.datawidth);
			break;

		case I2S_SAMP_RATE_22050:
			os_printf("========= CASE sample_rate: 22.050K, data_lenght: %d ============\r\n", rate_node.datawidth);
			break;

		case I2S_SAMP_RATE_44100:
			os_printf("========= CASE sample_rate: 44.1K, data_lenght: %d ==============\r\n", rate_node.datawidth);
			break;

		case I2S_SAMP_RATE_88200:
			os_printf("========= CASE sample_rate: 88.2K, data_lenght: %d ===============\r\n", rate_node.datawidth);
			break;

		default:
			break;
	}
}

static void i2s_master_test(void)
{
	i2s_rate_t rate;
	uint32_t data_buf_rl = 0x12345679;
	uint32_t receive_data_check = 0x87654320;
	uint32_t data_buf = 0;
	uint32_t write_flag = 0;
	uint32_t read_flag = 0;
	uint32_t i = 0;
	bk_err_t ret = BK_OK;

	for (i = 0; i < sizeof(i2s_rate_table)/sizeof(i2s_rate_table_node_t); i++)
	{
		test_log_print(i2s_rate_table[i]);

		//enable i2s rxovf interrupt
		bk_i2s_int_enable(I2S_ISR_CHL1_RXOVF, I2S_INT_ENABLE);
		os_printf("enable i2s rxovf interrupt successful \r\n");

		//set sample and bitclk ratio
		rate.datawidth = i2s_rate_table[i].datawidth;
		rate.samp_rate = i2s_rate_table[i].samp_rate;
		ret = bk_i2s_set_ratio(&rate);
		if (ret != BK_OK) {
			os_printf("config i2s sample_rate and data_length fail \r\n");
			os_printf("=================================== CASE TEST FAIL ==================================\r\n");
			continue;
		}
		os_printf("config i2s sample_rate and data_length \r\n");

		//enable i2s
		bk_i2s_enable(I2S_ENABLE);
		os_printf("enable i2s successful \r\n");

		//i2s_struct_dump();

		//save test config
		i2s_test_global.test_status = I2S_TESTING;
//			i2s_test_global.role = i2s_config.role;
//			i2s_test_global.work_mode = i2s_config.work_mode;
//			i2s_test_global.lrck_invert = i2s_config.lrck_invert;
//			i2s_test_global.sck_invert = i2s_config.sck_invert;
//			i2s_test_global.lsb_first = i2s_config.lsb_first_en;
//			i2s_test_global.store_mode = i2s_config.store_mode;

		data_buf_rl -= 1;
		while (i2s_test_global.test_status == I2S_TESTING) {
			bk_i2s_get_write_ready(&write_flag);
			if (write_flag)
				bk_i2s_write_data(1, &data_buf_rl, 1);
		}
		os_printf("i2s master stop send i2s data to slave \r\n");
		bk_i2s_get_read_ready(&read_flag);
		if (read_flag) {
			bk_i2s_read_data(&data_buf, 1);
			os_printf("i2s master receive i2s data from slave: %08x \r\n", data_buf);
			bk_i2s_clear_rxfifo();
		}
		receive_data_check += 1;
		if (data_buf == receive_data_check) {
			os_printf("i2s master receiving and slave sending test result: PASS \r\n");
			os_printf("=================================== CASE TEST PASS ==================================\r\n");
		} else {
			os_printf("i2s master receiving and slave sending test result: FAIL \r\n");
			os_printf("=================================== CASE TEST FAIL ==================================\r\n");
		}

		//disable i2s
		bk_i2s_enable(I2S_DISABLE);
		os_printf("disable i2s \r\n");

		//clear rxfifo and txfifo
		bk_i2s_clear_rxfifo();
		bk_i2s_clear_txfifo();
		os_printf("clear rxfifo and txfifo \r\n");

		//wait slave write data to txfifo
		delay(10000);
	}
}

void cli_i2s_master_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	i2s_config_t i2s_config;

	if (argc != 2) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("i2s master test start \r\n");
		i2s_config.i2s_en = I2S_DISABLE;
		i2s_config.role = I2S_ROLE_MASTER;
		i2s_config.work_mode = I2S_WORK_MODE_I2S;
		i2s_config.lrck_invert = I2S_LRCK_INVERT_DISABLE;
		i2s_config.sck_invert = I2S_SCK_INVERT_DISABLE;
		i2s_config.lsb_first_en = I2S_LSB_FIRST_DISABLE;
		i2s_config.sync_length = 0;
		i2s_config.data_length = 15;
		i2s_config.pcm_dlength = 0;
		i2s_config.sample_ratio = 0;
		i2s_config.sck_ratio = 0;
		i2s_config.parallel_en = I2S_PARALLEL_DISABLE;
		i2s_config.store_mode = I2S_LRCOM_STORE_16R16L;
		i2s_config.sck_ratio_h4b = 0;
		i2s_config.sample_ratio_h2b = 0;
		i2s_config.txint_level = I2S_TXINT_LEVEL_1;
		i2s_config.rxint_level = I2S_RXINT_LEVEL_1;

		//init i2s driver
		bk_i2s_driver_init();

		//init i2s configure
		bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
		os_printf("init i2s driver and config successful \r\n");

		//register isr
		bk_i2s_register_i2s_isr(I2S_ISR_CHL1_TXUDF, cli_i2s_master_txudf_isr, NULL);
		bk_i2s_register_i2s_isr(I2S_ISR_CHL1_RXOVF, cli_i2s_master_rxovf_isr, NULL);
		bk_i2s_register_i2s_isr(I2S_ISR_CHL1_TXINT, cli_i2s_master_txint_isr, NULL);
		bk_i2s_register_i2s_isr(I2S_ISR_CHL1_RXINT, cli_i2s_master_rxint_isr, NULL);
		os_printf("register i2s isr successful \r\n");

		os_printf("########################### I2S MODE: Philips TEST START ##########################\r\n");
		bk_i2s_set_work_mode(I2S_WORK_MODE_I2S);
		i2s_master_test();
		os_printf("########################### I2S MODE: Philips TEST END ############################\r\n");
		os_printf("--------------------------------------------------------------------------------------------\r\n");

		os_printf("########################### I2S MODE: Left Justified TEST START ###################\r\n");
		bk_i2s_set_work_mode(I2S_WORK_MODE_LEFTJUST);
		i2s_master_test();
		os_printf("########################### I2S MODE: Left Justified TEST END #####################\r\n");
		os_printf("--------------------------------------------------------------------------------------------\r\n");

		os_printf("########################### I2S MODE: Right Justified TEST START ###################\r\n");
		bk_i2s_set_work_mode(I2S_WORK_MODE_RIGHTJUST);
		i2s_master_test();
		os_printf("########################### I2S MODE: Right Justified TEST END #####################\r\n");
		os_printf("--------------------------------------------------------------------------------------------\r\n");

		os_printf("i2s master test complete \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("i2s master test stop \r\n");
		bk_i2s_driver_deinit();
		os_printf("i2s master test stop successful \r\n");
	} else {
		cli_i2s_help();
		return;
	}
}

void cli_i2s_master_sin_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	i2s_config_t i2s_config;
	i2s_rate_t rate;
	//uint32_t data_buf_rl = 0xffff0000;
	uint32_t write_flag = 0;
	uint32_t i = 0;

	if (argc != 2) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("i2s master test start\n");
		i2s_config.i2s_en = I2S_DISABLE;
		i2s_config.role = I2S_ROLE_MASTER;
		i2s_config.work_mode = I2S_WORK_MODE_I2S;
		i2s_config.lrck_invert = I2S_LRCK_INVERT_DISABLE;
		i2s_config.sck_invert = I2S_SCK_INVERT_DISABLE;
		i2s_config.lsb_first_en = I2S_LSB_FIRST_DISABLE;
		i2s_config.sync_length = 0;
		i2s_config.data_length = 15;
		i2s_config.pcm_dlength = 0;
		i2s_config.sample_ratio = 0;
		i2s_config.sck_ratio = 0;
		i2s_config.parallel_en = I2S_PARALLEL_DISABLE;
		i2s_config.store_mode = I2S_LRCOM_STORE_16R16L;
		i2s_config.sck_ratio_h4b = 0;
		i2s_config.sample_ratio_h2b = 0;
		i2s_config.txint_level = I2S_TXINT_LEVEL_1;
		i2s_config.rxint_level = I2S_RXINT_LEVEL_24;


		//init i2s driver
		bk_i2s_driver_init();

		//init i2s configure
		bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
		os_printf("init i2s driver and config successful\n");

		//register isr
		bk_i2s_register_i2s_isr(I2S_ISR_CHL1_TXUDF, cli_i2s_master_txudf_isr, NULL);
		bk_i2s_register_i2s_isr(I2S_ISR_CHL1_RXOVF, cli_i2s_master_rxovf_isr, NULL);
		bk_i2s_register_i2s_isr(I2S_ISR_CHL1_TXINT, cli_i2s_master_txint_isr, NULL);
		bk_i2s_register_i2s_isr(I2S_ISR_CHL1_RXINT, cli_i2s_master_rxint_isr, NULL);
		os_printf("register i2s isr successful\n");

		//set sample and bitclk ratio
		rate.datawidth = I2S_DATA_WIDTH_16;
		rate.samp_rate = I2S_SAMP_RATE_48000;
		bk_i2s_set_ratio(&rate);

		//enable i2s
		bk_i2s_enable(I2S_ENABLE);
		os_printf("enable i2s successful\n");

		i2s_struct_dump();

		//for (i=0; i < 60; i++)
		while (1)
		{
			for (i = 0; i<I2S_TEST_DATA_SIZE; i++)
			{
				bk_i2s_get_write_ready(&write_flag);
				while (!write_flag)
					bk_i2s_get_write_ready(&write_flag);
				bk_i2s_write_data(1, &data_source[i], 1);
				//delay(10);
				//os_printf("write data: 0x%08x\r\n", data_source[i]);
			}
		}

		os_printf("start i2s master send test successful\r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("i2s sin test stop\n");
		bk_i2s_driver_deinit();
		os_printf("i2s sin test stop successful\n");
	} else {
		cli_i2s_help();
		return;
	}
}

#if CONFIG_AUDIO
void cli_i2s_master_mic_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;
	i2s_config_t i2s_config;
	i2s_rate_t rate;
	//uint32_t data_buf_lr = 0xffff0000;
	//uint32_t i = 0;
	aud_adc_config_t adc_config;
	uint32_t adc_fifo_addr;
	uint32_t i2s_data_addr;
	dma_config_t dma_config;


	if (argc != 2) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("i2s master test start\n");
		i2s_config.i2s_en = I2S_DISABLE;
		i2s_config.role = I2S_ROLE_MASTER;
		i2s_config.work_mode = I2S_WORK_MODE_I2S;
		i2s_config.lrck_invert = I2S_LRCK_INVERT_DISABLE;
		i2s_config.sck_invert = I2S_SCK_INVERT_DISABLE;
		i2s_config.lsb_first_en = I2S_LSB_FIRST_DISABLE;
		i2s_config.sync_length = 0;
		i2s_config.data_length = 15;
		i2s_config.pcm_dlength = 0;
		i2s_config.sample_ratio = 0;
		i2s_config.sck_ratio = 0;
		i2s_config.parallel_en = I2S_PARALLEL_DISABLE;
		i2s_config.store_mode = I2S_LRCOM_STORE_16R16L;
		i2s_config.sck_ratio_h4b = 0;
		i2s_config.sample_ratio_h2b = 0;
		i2s_config.txint_level = I2S_TXINT_LEVEL_1;
		i2s_config.rxint_level = I2S_RXINT_LEVEL_24;

		adc_config.samp_rate = AUD_ADC_SAMP_RATE_8K;
		adc_config.adc_enable = AUD_ADC_DISABLE;
		adc_config.line_enable = AUD_ADC_LINE_DISABLE;
		adc_config.dtmf_enable = AUD_DTMF_DISABLE;
		adc_config.adc_hpf2_coef_B2 = 0;
		adc_config.adc_hpf2_bypass_enable = AUD_ADC_HPF_BYPASS_ENABLE;
		adc_config.adc_hpf1_bypass_enable = AUD_ADC_HPF_BYPASS_ENABLE;
		adc_config.adc_set_gain = 0x2d;
		adc_config.adc_samp_edge = AUD_ADC_SAMP_EDGE_RISING;
		adc_config.adc_hpf2_coef_B0 = 0;
		adc_config.adc_hpf2_coef_B1 = 0;
		adc_config.adc_hpf2_coef_A0 = 0;
		adc_config.adc_hpf2_coef_A1 = 0;
		adc_config.dtmf_wr_threshold = 8;
		adc_config.adcl_wr_threshold = 8;
		adc_config.dtmf_int_enable = AUD_DTMF_INT_DISABLE;
		adc_config.adcl_int_enable = AUD_ADCL_INT_DISABLE;
		adc_config.loop_adc2dac = AUD_LOOP_ADC2DAC_DISABLE;
		adc_config.agc_noise_thrd = 101;
		adc_config.agc_noise_high = 101;
		adc_config.agc_noise_low = 160;
		adc_config.agc_noise_min = 1;
		adc_config.agc_noise_tout = 0;
		adc_config.agc_high_dur = 3;
		adc_config.agc_low_dur = 3;
		adc_config.agc_min = 1;
		adc_config.agc_max = 4;
		adc_config.agc_ng_method = AUD_AGC_NG_METHOD_MUTE;
		adc_config.agc_ng_enable = AUD_AGC_NG_DISABLE;
		adc_config.agc_decay_time = AUD_AGC_DECAY_TIME_128;
		adc_config.agc_attack_time = AUD_AGC_ATTACK_TIME_128;
		adc_config.agc_high_thrd = 18;
		adc_config.agc_low_thrd = 0;
		adc_config.agc_iir_coef = AUD_AGC_IIR_COEF_1_1024;
		adc_config.agc_enable = AUD_AGC_DISABLE;
		adc_config.manual_pga_value = 0;
		adc_config.manual_pga_enable = AUD_GAC_MANUAL_PGA_DISABLE;
		adc_config.adc_fracmod_manual = AUD_ADC_TRACMOD_MANUAL_DISABLE;
		adc_config.adc_fracmod = 0;


		//init i2s driver
		bk_i2s_driver_init();
		//init i2s configure
		bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
		os_printf("init i2s driver and config successful\n");

		//register isr
		bk_i2s_register_i2s_isr(I2S_ISR_CHL1_TXUDF, cli_i2s_master_txudf_isr, NULL);
		bk_i2s_register_i2s_isr(I2S_ISR_CHL1_RXOVF, cli_i2s_master_rxovf_isr, NULL);
		bk_i2s_register_i2s_isr(I2S_ISR_CHL1_TXINT, cli_i2s_master_txint_isr, NULL);
		bk_i2s_register_i2s_isr(I2S_ISR_CHL1_RXINT, cli_i2s_master_rxint_isr, NULL);
		os_printf("register i2s isr successful\n");

		//set sample and bitclk ratio
		rate.datawidth = I2S_DATA_WIDTH_16;
		rate.samp_rate = I2S_SAMP_RATE_48000;
		bk_i2s_set_ratio(&rate);

		//enable i2s
		bk_i2s_enable(I2S_ENABLE);
		os_printf("enable i2s successful\n");

		i2s_struct_dump();

		//init audio driver
		bk_aud_driver_init();

		bk_aud_adc_init(AUD_ADC_WORK_MODE_ADC, &adc_config, NULL);
		os_printf("init audio driver, adc successful\n");

		//init dma driver
		ret = bk_dma_driver_init();
		if (ret != BK_OK) {
			os_printf("dma driver init failed\r\n");
			return;
		}
		dma_config.mode = DMA_WORK_MODE_REPEAT;
		dma_config.chan_prio = 1;
		dma_config.src.dev = DMA_DEV_AUDIO;
		dma_config.src.width = DMA_DATA_WIDTH_32BITS;
		dma_config.dst.dev = DMA_DEV_I2S;
		dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
		//get adc fifo address
		if (bk_aud_get_adc_fifo_addr(&adc_fifo_addr) != BK_OK) {
			os_printf("get adc fifo address failed\r\n");
			return;
		} else {
			dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
			dma_config.src.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
			dma_config.src.start_addr = adc_fifo_addr;
			dma_config.src.end_addr = adc_fifo_addr + 4;
		}
		//get i2s data address
		if (bk_i2s_get_data_addr(&i2s_data_addr) != BK_OK) {
			os_printf("get i2s data address failed\r\n");
			return;
		} else {
			dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
			dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
			dma_config.dst.start_addr = i2s_data_addr;
			dma_config.dst.end_addr = i2s_data_addr + 4;
		}

		os_printf("source_addr:0x%x, dest_addr:0x%x\r\n", dma_config.src.start_addr, dma_config.dst.start_addr);

		//init dma channel
		ret = bk_dma_init(DMA_ID_2, &dma_config);
		if (ret != BK_OK) {
			os_printf("dma init failed\r\n");
			return;
		}
		bk_dma_set_transfer_len(DMA_ID_2, 4);
		ret = bk_dma_start(DMA_ID_2);

		//start adc and dac
		bk_aud_start_adc();
		os_printf("enable adc successful\n");

		//aud_struct_dump();

		os_printf("start i2s master send test successful\r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("audio adc mcp test stop\n");
		bk_i2s_driver_deinit();
		os_printf("audio adc mcp test stop successful\n");
	} else {
		cli_i2s_help();
		return;
	}
}
#endif
static void i2s_slave_test(void)
{
	i2s_rate_t rate;
	uint32_t data_buf_rl = 0x87654320;
	uint32_t receive_data_check = 0x12345679;
	uint32_t data_buf = 0;
	uint32_t write_flag = 0;
	uint32_t read_flag = 0;
	uint32_t i = 0;
	bk_err_t ret = BK_OK;

	for (i = 0; i < sizeof(i2s_rate_table)/sizeof(i2s_rate_table_node_t); i++)
	{
		test_log_print(i2s_rate_table[i]);

		//enable i2s rxovf interrupt
		bk_i2s_int_enable(I2S_ISR_CHL1_RXOVF, I2S_INT_ENABLE);
		os_printf("enable i2s rxovf interrupt successful \r\n");

		//set sample and bitclk ratio
		rate.datawidth = i2s_rate_table[i].datawidth;
		rate.samp_rate = i2s_rate_table[i].samp_rate;
		ret = bk_i2s_set_ratio(&rate);
		if (ret != BK_OK) {
			os_printf("config i2s sample_rate and data_length fail \r\n");
			os_printf("=================================== CASE TEST FAIL ==================================\r\n");
			continue;
		}
		os_printf("config i2s sample_rate and data_length \r\n");

		//enable i2s
		bk_i2s_enable(I2S_ENABLE);
		os_printf("enable i2s successful \r\n");

		//i2s_struct_dump();

		//save test config
		i2s_test_global.test_status = I2S_TESTING;
//			i2s_test_global.role = i2s_config.role;
//			i2s_test_global.work_mode = i2s_config.work_mode;
//			i2s_test_global.lrck_invert = i2s_config.lrck_invert;
//			i2s_test_global.sck_invert = i2s_config.sck_invert;
//			i2s_test_global.lsb_first = i2s_config.lsb_first_en;
//			i2s_test_global.store_mode = i2s_config.store_mode;

		data_buf_rl += 1;
		while (i2s_test_global.test_status == I2S_TESTING) {
			bk_i2s_get_write_ready(&write_flag);
			if (write_flag)
				bk_i2s_write_data(1, &data_buf_rl, 1);
		}
		os_printf("i2s master stop send i2s data to slave \r\n");
		bk_i2s_get_read_ready(&read_flag);
		if (read_flag) {
			bk_i2s_read_data(&data_buf, 1);
			os_printf("i2s master receive i2s data from slave: %08x \r\n", data_buf);
			bk_i2s_clear_rxfifo();
		}
		receive_data_check -= 1;
		if (data_buf == receive_data_check) {
			os_printf("i2s master sending and slave receiving test result: PASS \r\n");
			os_printf("=================================== CASE TEST PASS ==================================\r\n");
		} else {
			os_printf("i2s master sending and slave receiving test result: FAIL \r\n");
			os_printf("=================================== CASE TEST FAIL ==================================\r\n");
		}

		//disable i2s
		bk_i2s_enable(I2S_DISABLE);
		os_printf("disable i2s \r\n");

		//clear rxfifo and txfifo
		bk_i2s_clear_rxfifo();
		bk_i2s_clear_txfifo();
		os_printf("clear rxfifo and txfifo \r\n");
	}


}

void cli_i2s_slave_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	i2s_config_t i2s_config;

	if (argc != 2) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("i2s slave test start \r\n");
		i2s_config.i2s_en = I2S_DISABLE;
		i2s_config.role = I2S_ROLE_SLAVE;
		i2s_config.work_mode = I2S_WORK_MODE_I2S;
		i2s_config.lrck_invert = I2S_LRCK_INVERT_DISABLE;
		i2s_config.sck_invert = I2S_SCK_INVERT_DISABLE;
		i2s_config.lsb_first_en = I2S_LSB_FIRST_DISABLE;
		i2s_config.sync_length = 0;
		i2s_config.data_length = 15;
		i2s_config.pcm_dlength = 0;
		i2s_config.sample_ratio = 0;
		i2s_config.sck_ratio = 0;
		i2s_config.parallel_en = I2S_PARALLEL_DISABLE;
		i2s_config.store_mode = I2S_LRCOM_STORE_16R16L;
		i2s_config.sck_ratio_h4b = 0;
		i2s_config.sample_ratio_h2b = 0;
		i2s_config.txint_level = I2S_TXINT_LEVEL_1;
		i2s_config.rxint_level = I2S_RXINT_LEVEL_24;

		//init i2s driver
		bk_i2s_driver_init();
		//init i2s configure
		bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
		os_printf("init i2s driver and config successful \r\n");

		//register isr
		bk_i2s_register_i2s_isr(I2S_ISR_CHL1_TXUDF, cli_i2s_slave_txudf_isr, NULL);
		bk_i2s_register_i2s_isr(I2S_ISR_CHL1_RXOVF, cli_i2s_slave_rxovf_isr, NULL);
		bk_i2s_register_i2s_isr(I2S_ISR_CHL1_TXINT, cli_i2s_slave_txint_isr, NULL);
		bk_i2s_register_i2s_isr(I2S_ISR_CHL1_RXINT, cli_i2s_slave_rxint_isr, NULL);
		os_printf("register i2s isr successful \r\n");

		os_printf("########################### I2S MODE: Philips TEST START ##########################\r\n");
		bk_i2s_set_work_mode(I2S_WORK_MODE_I2S);
		i2s_slave_test();
		os_printf("########################### I2S MODE: Philips TEST END ############################\r\n");
		os_printf("--------------------------------------------------------------------------------------------\r\n");

		os_printf("########################### I2S MODE: Left Justified TEST START ###################\r\n");
		bk_i2s_set_work_mode(I2S_WORK_MODE_LEFTJUST);
		i2s_slave_test();
		os_printf("########################### I2S MODE: Left Justified TEST END #####################\r\n");
		os_printf("--------------------------------------------------------------------------------------------\r\n");

		os_printf("########################### I2S MODE: Right Justified TEST START ###################\r\n");
		bk_i2s_set_work_mode(I2S_WORK_MODE_RIGHTJUST);
		i2s_slave_test();
		os_printf("########################### I2S MODE: Right Justified TEST END #####################\r\n");
		os_printf("--------------------------------------------------------------------------------------------\r\n");

		os_printf("i2s slave test complete\r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("i2s slave test stop \r\n");
		bk_i2s_driver_deinit();
		os_printf("i2s slave test stop successful \r\n");
	} else {
		cli_i2s_help();
		return;
	}
}

#if CONFIG_AUDIO
void cli_i2s_slave_mic_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;
	i2s_config_t i2s_config;
	i2s_rate_t rate;
	uint32_t dac_fifo_addr;
	aud_dac_config_t dac_config;
	uint32_t i2s_data_addr;
	dma_config_t dma_config;
	uint32_t data_buf = 0;


	if (argc != 2) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("i2s master test start\n");
		i2s_config.i2s_en = I2S_DISABLE;
		i2s_config.role = I2S_ROLE_MASTER;
		i2s_config.work_mode = I2S_WORK_MODE_I2S;
		i2s_config.lrck_invert = I2S_LRCK_INVERT_DISABLE;
		i2s_config.sck_invert = I2S_SCK_INVERT_DISABLE;
		i2s_config.lsb_first_en = I2S_LSB_FIRST_DISABLE;
		i2s_config.sync_length = 0;
		i2s_config.data_length = 15;
		i2s_config.pcm_dlength = 0;
		i2s_config.sample_ratio = 0;
		i2s_config.sck_ratio = 0;
		i2s_config.parallel_en = I2S_PARALLEL_DISABLE;
		i2s_config.store_mode = I2S_LRCOM_STORE_16R16L;
		i2s_config.sck_ratio_h4b = 0;
		i2s_config.sample_ratio_h2b = 0;
		i2s_config.txint_level = I2S_TXINT_LEVEL_1;
		i2s_config.rxint_level = I2S_RXINT_LEVEL_24;

		dac_config.dac_enable = AUD_DAC_DISABLE;
		dac_config.samp_rate = AUD_DAC_SAMP_RATE_8K;
		dac_config.dac_hpf2_coef_B2 = 0x3A22;
		dac_config.dac_hpf2_bypass_enable = AUD_DAC_HPF_BYPASS_DISABLE;
		dac_config.dac_hpf1_bypass_enable = AUD_DAC_HPF_BYPASS_DISABLE;
		dac_config.dac_set_gain = 0x2D;
		dac_config.dac_clk_invert = AUD_DAC_CLK_INVERT_RISING;

		dac_config.dac_hpf2_coef_B0 = 0x3A22;
		dac_config.dac_hpf2_coef_B1 = 0x8BBF;

		dac_config.dac_hpf2_coef_A1 = 0x751C;
		dac_config.dac_hpf2_coef_A2 = 0xC9E6;

		dac_config.dacr_rd_threshold = 0x4;
		dac_config.dacl_rd_threshold = 0x4;
		dac_config.dacr_int_enable = 0x0;
		dac_config.dacl_int_enable = 0x0;

		dac_config.dac_filt_enable = AUD_DAC_FILT_DISABLE;
		dac_config.dac_fracmod_manual_enable = AUD_DAC_FRACMOD_MANUAL_DISABLE;
		dac_config.dac_fracmode_value = 0x0;

		//init i2s driver
		bk_i2s_driver_init();
		//init i2s configure
		bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
		os_printf("init i2s driver and config successful\n");

		//register isr
		bk_i2s_register_i2s_isr(I2S_ISR_CHL1_TXUDF, cli_i2s_slave_txudf_isr, NULL);
		bk_i2s_register_i2s_isr(I2S_ISR_CHL1_RXOVF, cli_i2s_slave_rxovf_isr, NULL);
		bk_i2s_register_i2s_isr(I2S_ISR_CHL1_TXINT, cli_i2s_slave_txint_isr, NULL);
		bk_i2s_register_i2s_isr(I2S_ISR_CHL1_RXINT, cli_i2s_slave_rxint_isr, NULL);
		os_printf("register i2s isr successful\n");

		//set sample and bitclk ratio
		rate.datawidth = I2S_DATA_WIDTH_16;
		rate.samp_rate = I2S_SAMP_RATE_48000;
		bk_i2s_set_ratio(&rate);

		//enable i2s
		bk_i2s_enable(I2S_ENABLE);
		os_printf("enable i2s successful\n");

		bk_i2s_write_data(1, &data_buf, 1);

		i2s_struct_dump();

		//init audio driver
		bk_aud_driver_init();

		bk_aud_dac_init(&dac_config);
		os_printf("init audio driver, dac successful\n");

		//init dma driver
		ret = bk_dma_driver_init();
		if (ret != BK_OK) {
			os_printf("dma driver init failed\r\n");
			return;
		}
		dma_config.mode = DMA_WORK_MODE_REPEAT;
		dma_config.chan_prio = 1;
		dma_config.src.dev = DMA_DEV_I2S;
		dma_config.src.width = DMA_DATA_WIDTH_32BITS;
		dma_config.dst.dev = DMA_DEV_AUDIO;
		dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
		//get i2s data address
		if (bk_i2s_get_data_addr(&i2s_data_addr) != BK_OK) {
			os_printf("get i2s data address failed\r\n");
			return;
		} else {
			dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
			dma_config.src.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
			dma_config.src.start_addr = i2s_data_addr;
			dma_config.src.end_addr = i2s_data_addr + 4;
		}
		//get dac fifo address
		if (bk_aud_get_dac_fifo_addr(&dac_fifo_addr) != BK_OK) {
			os_printf("get dac fifo address failed\r\n");
			return;
		} else {
			dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
			dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
			dma_config.dst.start_addr = dac_fifo_addr;
			dma_config.dst.end_addr = dac_fifo_addr + 4;
		}

		os_printf("source_addr:0x%x, dest_addr:0x%x\r\n", dma_config.src.start_addr, dma_config.dst.start_addr);

		//init dma channel
		ret = bk_dma_init(DMA_ID_2, &dma_config);
		if (ret != BK_OK) {
			os_printf("dma init failed\r\n");
			return;
		}
		bk_dma_set_transfer_len(DMA_ID_2, 4);
		ret = bk_dma_start(DMA_ID_2);

		//start adc and dac
		bk_aud_start_dac();
		os_printf("enable adc successful\n");

		//aud_struct_dump();

		os_printf("start i2s slave receive test successful\r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("audio adc mcp test stop\n");
		bk_i2s_driver_deinit();
		os_printf("audio adc mcp test stop successful\n");
	} else {
		cli_i2s_help();
		return;
	}
}
#endif

