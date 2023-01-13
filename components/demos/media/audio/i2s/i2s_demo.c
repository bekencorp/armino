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

//extern void delay(int num);//TODO fix me
static RingBufferContext *ch1_tx_rb;
static RingBufferContext *ch1_rx_rb;
static RingBufferContext *ch2_tx_rb;
static RingBufferContext *ch2_rx_rb;
static RingBufferContext *ch3_tx_rb;
static RingBufferContext *ch3_rx_rb;

static uint8_t *ch1_temp = NULL;
static uint8_t *ch2_temp = NULL;
static uint8_t *ch3_temp = NULL;


static void cli_i2s_help(void)
{
	os_printf("i2s_master_test {start|stop} \r\n");
	os_printf("i2s_slave_test {start|stop} \r\n");
	os_printf("pcm_master_test {start|stop} \r\n");
	os_printf("pcm_slave_test {start|stop} \r\n");
	os_printf("dtm_master_test {start|stop} \r\n");
	os_printf("dtm_slave_test {start|stop} \r\n");
	os_printf("2bd_master_test {start|stop} \r\n");
	os_printf("2bd_slave_test {start|stop} \r\n");
}

static int ch1_tx_data_handle_cb(uint32_t size)
{
	//os_printf("%s, size: %d \n", __func__, size);
	ring_buffer_write(ch1_tx_rb, ch1_temp, size);

	return size;
}

static int ch1_rx_data_handle_cb(uint32_t size)
{
	//os_printf("%s, size: %d \n", __func__, size);
	ring_buffer_read(ch1_rx_rb, ch1_temp, size);
	os_printf("rx ch1_temp[0]: 0x%2x \n", ch1_temp[0]);

	return size;
}

static int ch2_tx_data_handle_cb(uint32_t size)
{
	ring_buffer_write(ch2_tx_rb, ch2_temp, size);

	return size;
}

static int ch2_rx_data_handle_cb(uint32_t size)
{
	ring_buffer_read(ch2_rx_rb, ch2_temp, size);
	os_printf("rx ch2_temp[0]: 0x%2x \n", ch2_temp[0]);

	return size;
}

static int ch3_tx_data_handle_cb(uint32_t size)
{
	ring_buffer_write(ch3_tx_rb, ch3_temp, size);

	return size;
}

static int ch3_rx_data_handle_cb(uint32_t size)
{
	ring_buffer_read(ch3_rx_rb, ch3_temp, size);
	os_printf("rx ch3_temp[0]: 0x%2x \n", ch3_temp[0]);

	return size;
}

void cli_i2s_master_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	i2s_config_t i2s_config = DEFAULT_I2S_CONFIG();
	bk_err_t ret = BK_OK;
	uint32_t size = 0;

	if (argc != 2) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("i2s master test start \r\n");

		ch1_temp = os_malloc(320);
		os_memset(ch1_temp, 0xF1, 320);

		//init i2s driver
		bk_i2s_driver_init();

		//init i2s configure
		i2s_config.samp_rate = I2S_SAMP_RATE_48000;
		bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
		os_printf("init i2s driver and config successful \r\n");

		ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX, 640, ch1_tx_data_handle_cb, &ch1_tx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		uint8_t *temp_data = (uint8_t *)os_malloc(640);
		os_memset(temp_data, 0x00, 640);
		size = ring_buffer_write(ch1_tx_rb, temp_data, 640);
		os_printf("ring_buffer_write, size: %d \n", size);
		os_free(temp_data);
		bk_i2s_start();

		os_printf("i2s master test complete \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("i2s master test stop \r\n");
		bk_i2s_stop();
		bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX);
		bk_i2s_deinit();
		bk_i2s_driver_deinit();

		os_free(ch1_temp);
		ch1_temp = NULL;
		os_printf("i2s master test stop successful \r\n");
	} else {
		cli_i2s_help();
		return;
	}
}


void cli_i2s_slave_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	i2s_config_t i2s_config = DEFAULT_I2S_CONFIG();
	bk_err_t ret = BK_OK;

	if (argc != 2) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("i2s slave test start \r\n");

		ch1_temp = os_malloc(320);

		//init i2s driver
		bk_i2s_driver_init();

		//init i2s configure
		i2s_config.role = I2S_ROLE_SLAVE;
		i2s_config.samp_rate = I2S_SAMP_RATE_48000;
		bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
		os_printf("init i2s driver and config successful \r\n");

		ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX, 640, ch1_rx_data_handle_cb, &ch1_rx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		bk_i2s_start();

		os_printf("i2s slave test complete \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("i2s slave test stop \r\n");
		bk_i2s_stop();
		bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX);
		bk_i2s_deinit();
		bk_i2s_driver_deinit();

		os_free(ch1_temp);
		ch1_temp = NULL;
		os_printf("i2s slave test stop successful \r\n");
	} else {
		//bk_i2s_start_test();
		cli_i2s_help();
		return;
	}
}


void cli_pcm_master_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	i2s_config_t i2s_config = DEFAULT_PCM_CONFIG();
	bk_err_t ret = BK_OK;
	uint32_t size = 0;

	if (argc != 2) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("pcm master test start \r\n");

		ch1_temp = os_malloc(320);
		os_memset(ch1_temp, 0xF1, 320);

		//init i2s driver
		bk_i2s_driver_init();

		//init i2s configure
		i2s_config.samp_rate = I2S_SAMP_RATE_48000;
		bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
		os_printf("init i2s driver and config successful \r\n");

		ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX, 640, ch1_tx_data_handle_cb, &ch1_tx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		uint8_t *temp_data = (uint8_t *)os_malloc(640);
		os_memset(temp_data, 0xF1, 640);
		size = ring_buffer_write(ch1_tx_rb, temp_data, 640);
		os_printf("ring_buffer_write, size: %d \n", size);
		os_free(temp_data);
		bk_i2s_start();

		os_printf("pcm master test complete \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("pcm master test stop \r\n");
		bk_i2s_stop();
		bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX);
		bk_i2s_deinit();
		bk_i2s_driver_deinit();

		os_free(ch1_temp);
		ch1_temp = NULL;
		os_printf("pcm master test stop successful \r\n");
	} else {
		cli_i2s_help();
		return;
	}
}

void cli_pcm_slave_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	i2s_config_t i2s_config = DEFAULT_PCM_CONFIG();
	bk_err_t ret = BK_OK;

	if (argc != 2) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("pcm slave test start \r\n");

		ch1_temp = os_malloc(320);

		//init i2s driver
		bk_i2s_driver_init();

		//init i2s configure
		i2s_config.samp_rate = I2S_SAMP_RATE_48000;
		i2s_config.role = I2S_ROLE_SLAVE;
		bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
		os_printf("init i2s driver and config successful \r\n");

		ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX, 640, ch1_rx_data_handle_cb, &ch1_rx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}
		bk_i2s_start();
		os_printf("pcm slave test complete \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("pcm slave test stop \r\n");
		bk_i2s_stop();
		bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX);
		bk_i2s_deinit();
		bk_i2s_driver_deinit();

		os_free(ch1_temp);
		ch1_temp = NULL;
		os_printf("pcm slave test stop successful \r\n");
	} else {
		cli_i2s_help();
		return;
	}
}


void cli_dtm_master_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	i2s_config_t i2s_config = DEFAULT_TDM_CONFIG();
	bk_err_t ret = BK_OK;

	if (argc != 2) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("pcm master test start \r\n");

		ch1_temp = os_malloc(320);
		os_memset(ch1_temp, 0xF1, 320);
		ch2_temp = os_malloc(320);
		os_memset(ch2_temp, 0xF2, 320);
		ch3_temp = os_malloc(320);
		os_memset(ch3_temp, 0xF3, 320);

		//init i2s driver
		bk_i2s_driver_init();

		//init i2s configure
		//i2s_config.samp_rate = I2S_SAMP_RATE_16000;
		bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
		os_printf("init i2s driver and config successful \r\n");

		ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX, 640, ch1_tx_data_handle_cb, &ch1_tx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		ret = bk_i2s_chl_init(I2S_CHANNEL_2, I2S_TXRX_TYPE_TX, 640, ch2_tx_data_handle_cb, &ch2_tx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		ret = bk_i2s_chl_init(I2S_CHANNEL_3, I2S_TXRX_TYPE_TX, 640, ch3_tx_data_handle_cb, &ch3_tx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		uint8_t *temp_data = (uint8_t *)os_malloc(640);
		os_memset(temp_data, 0xF1, 640);
		ring_buffer_write(ch1_tx_rb, temp_data, 640);
		os_memset(temp_data, 0xF1, 640);
		ring_buffer_write(ch2_tx_rb, temp_data, 640);
		os_memset(temp_data, 0xF1, 640);
		ring_buffer_write(ch3_tx_rb, temp_data, 640);
		os_free(temp_data);
		os_printf("write data ok \r\n");
		bk_i2s_start();

		os_printf("pcm master test complete \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("pcm master test stop \r\n");
		bk_i2s_stop();
		bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX);
		bk_i2s_chl_deinit(I2S_CHANNEL_2, I2S_TXRX_TYPE_TX);
		bk_i2s_chl_deinit(I2S_CHANNEL_3, I2S_TXRX_TYPE_TX);
		bk_i2s_deinit();
		bk_i2s_driver_deinit();

		os_free(ch1_temp);
		ch1_temp = NULL;
		os_free(ch2_temp);
		ch2_temp = NULL;
		os_free(ch3_temp);
		ch3_temp = NULL;
		os_printf("pcm master test stop successful \r\n");
	} else {
		cli_i2s_help();
		return;
	}
}

void cli_dtm_slave_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	i2s_config_t i2s_config = DEFAULT_TDM_CONFIG();
	bk_err_t ret = BK_OK;

	if (argc != 2) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("dtm slave test start \r\n");

		ch1_temp = os_malloc(320);
		ch2_temp = os_malloc(320);
		ch3_temp = os_malloc(320);

		//init i2s driver
		bk_i2s_driver_init();

		//init i2s configure
		//i2s_config.samp_rate = I2S_SAMP_RATE_48000;
		i2s_config.role = I2S_ROLE_SLAVE;
		bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
		os_printf("init i2s driver and config successful \r\n");

		ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX, 640, ch1_rx_data_handle_cb, &ch1_rx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		ret = bk_i2s_chl_init(I2S_CHANNEL_2, I2S_TXRX_TYPE_RX, 640, ch2_rx_data_handle_cb, &ch2_rx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		ret = bk_i2s_chl_init(I2S_CHANNEL_3, I2S_TXRX_TYPE_RX, 640, ch3_rx_data_handle_cb, &ch3_rx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		bk_i2s_start();
		os_printf("dtm slave test complete \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("dtm slave test stop \r\n");
		bk_i2s_stop();
		bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX);
		bk_i2s_chl_deinit(I2S_CHANNEL_2, I2S_TXRX_TYPE_RX);
		bk_i2s_chl_deinit(I2S_CHANNEL_3, I2S_TXRX_TYPE_RX);
		bk_i2s_deinit();
		bk_i2s_driver_deinit();

		os_free(ch1_temp);
		ch1_temp = NULL;
		os_free(ch2_temp);
		ch2_temp = NULL;
		os_free(ch3_temp);
		ch3_temp = NULL;
		os_printf("pcm slave test stop successful \r\n");
	} else {
		cli_i2s_help();
		return;
	}
}

void cli_2bd_master_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	i2s_config_t i2s_config = DEFAULT_2BD_CONFIG();
	bk_err_t ret = BK_OK;
	uint32_t size = 0;

	if (argc != 2) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("2B+D master test start \r\n");

		ch1_temp = os_malloc(320);
		os_memset(ch1_temp, 0xF1, 320);

		//init i2s driver
		bk_i2s_driver_init();

		//init i2s configure
		i2s_config.samp_rate = I2S_SAMP_RATE_48000;
		bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
		os_printf("init i2s driver and config successful \r\n");

		ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX, 640, ch1_tx_data_handle_cb, &ch1_tx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		uint8_t *temp_data = (uint8_t *)os_malloc(640);
		os_memset(temp_data, 0xF1, 640);
		size = ring_buffer_write(ch1_tx_rb, temp_data, 640);
		os_printf("ring_buffer_write, size: %d \n", size);
		os_free(temp_data);
		bk_i2s_start();

		os_printf("2B+D master test complete \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("2B+D master test stop \r\n");
		bk_i2s_stop();
		bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX);
		bk_i2s_deinit();
		bk_i2s_driver_deinit();

		os_free(ch1_temp);
		ch1_temp = NULL;
		os_printf("2B+D master test stop successful \r\n");
	} else {
		cli_i2s_help();
		return;
	}
}

void cli_2bd_slave_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	i2s_config_t i2s_config = DEFAULT_2BD_CONFIG();
	bk_err_t ret = BK_OK;

	if (argc != 2) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("2B+D slave test start \r\n");

		ch1_temp = os_malloc(320);

		//init i2s driver
		bk_i2s_driver_init();

		//init i2s configure
		i2s_config.role = I2S_ROLE_SLAVE;
		i2s_config.samp_rate = I2S_SAMP_RATE_48000;
		bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
		os_printf("init i2s driver and config successful \r\n");

		ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX, 640, ch1_rx_data_handle_cb, &ch1_rx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		bk_i2s_start();

		os_printf("2B+D slave test complete \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("2B+D slave test stop \r\n");
		bk_i2s_stop();
		bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX);
		bk_i2s_deinit();
		bk_i2s_driver_deinit();

		os_free(ch1_temp);
		ch1_temp = NULL;
		os_printf("2B+D slave test stop successful \r\n");
	} else {
		cli_i2s_help();
		return;
	}
}

