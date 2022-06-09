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
#include "cli.h"
#include <driver/spi.h>
#include "spi_pub.h"

static void cli_spi_help(void)
{
	CLI_LOGI("spi_driver {init|deinit}\r\n");
	CLI_LOGI("spi {id} {init} [mode] [bit_width] [bit_width] [cpol] [cpha] [wire_mode] [baud_rate] [bit_order]\r\n");
	CLI_LOGI("spi {id} {deinit} \r\n");
	CLI_LOGI("spi {id} {write} [buf_len]\r\n");
}

static void cli_spi_rx_isr(spi_id_t id, void *param)
{
	CLI_LOGI("spi_rx_isr(%d)\n", id);
}

static void cli_spi_tx_isr(spi_id_t id, void *param)
{
	CLI_LOGI("spi_tx_isr(%d)\n", id);
}

static void cli_spi_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_spi_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_spi_driver_init());
		CLI_LOGI("spi driver init\n");
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_spi_driver_deinit());
		CLI_LOGI("spi driver deinit\n");
	} else {
		cli_spi_help();
		return;
	}
}

static void cli_spi_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_spi_help();
		return;
	}
	uint32_t spi_id = os_strtoul(argv[1], NULL, 10);

	if (os_strcmp(argv[2], "init") == 0) {
		spi_config_t config = {0};
		if (os_strcmp(argv[3], "master") == 0) {
			config.role = SPI_ROLE_MASTER;
		} else {
			config.role = SPI_ROLE_SLAVE;
		}
		if (os_strtoul(argv[4], NULL, 10) == 8) {
			config.bit_width = SPI_BIT_WIDTH_8BITS;
		} else {
			config.bit_width = SPI_BIT_WIDTH_16BITS;
		}
		config.polarity = os_strtoul(argv[5], NULL, 10);
		config.phase = os_strtoul(argv[6], NULL, 10);
		if (os_strtoul(argv[7], NULL, 10) == 3) {
			config.wire_mode = SPI_3WIRE_MODE;
		} else {
			config.wire_mode = SPI_4WIRE_MODE;
		}
		config.baud_rate = os_strtoul(argv[8], NULL, 10);
		if (os_strcmp(argv[9], "LSB") == 0) {
			config.bit_order = SPI_LSB_FIRST;
		} else {
			config.bit_order = SPI_MSB_FIRST;
		}
#if CONFIG_SPI_DMA
		config.dma_mode = os_strtoul(argv[10], NULL, 10);
		config.spi_tx_dma_chan = os_strtoul(argv[11], NULL, 10);
		config.spi_rx_dma_chan = os_strtoul(argv[12], NULL, 10);
#endif
		BK_LOG_ON_ERR(bk_spi_init(spi_id, &config));
		CLI_LOGI("spi init, spi_id=%d\n", spi_id);
	} else if (os_strcmp(argv[2], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_spi_deinit(spi_id));
		CLI_LOGI("spi deinit, spi_id=%d\n", spi_id);
	} else if (os_strcmp(argv[2], "write") == 0) {
		uint32_t buf_len = os_strtoul(argv[3], NULL, 10);
		uint8_t *send_data = (uint8_t *)os_zalloc(buf_len);
		if (send_data == NULL) {
			CLI_LOGE("send buffer malloc failed\r\n");
			return;
		}
		for (int i = 0; i < buf_len; i++) {
			send_data[i] = i & 0xff;
		}
		BK_LOG_ON_ERR(bk_spi_write_bytes(spi_id, send_data, buf_len));
		if (send_data) {
			os_free(send_data);
		}
		send_data = NULL;
		CLI_LOGI("spi write bytes, spi_id=%d, data_len=%d\n", spi_id, buf_len);
	} else if (os_strcmp(argv[2], "read") == 0) {
		uint32_t buf_len = os_strtoul(argv[3], NULL, 10);
		uint8_t *recv_data = (uint8_t *)os_malloc(buf_len);
		if (recv_data == NULL) {
			CLI_LOGE("recv buffer malloc failed\r\n");
			return;
		}
		os_memset(recv_data, 0xff, buf_len);
		BK_LOG_ON_ERR(bk_spi_read_bytes(spi_id, recv_data, buf_len));
		CLI_LOGI("spi read, spi_id=%d, size:%d\n", spi_id, buf_len);
		for (int i = 0; i < buf_len; i++) {
			CLI_LOGI("recv_buffer[%d]=0x%x\n", i, recv_data[i]);
		}
		if (recv_data) {
			os_free(recv_data);
		}
		recv_data = NULL;
	} else if (os_strcmp(argv[2], "transmit") == 0) {
		int send_len = os_strtoul(argv[3], NULL, 10);
		int recv_len = os_strtoul(argv[4], NULL, 10);

		uint8_t *send_data = (uint8_t *)os_zalloc(send_len);
		if (send_data == NULL) {
			CLI_LOGE("send buffer malloc failed\r\n");
			return;
		}
		for (int i = 0; i < send_len; i++) {
			send_data[i] = i & 0xff;
		}
		uint8_t *recv_data = (uint8_t *)os_malloc(recv_len);
		if (recv_data == NULL) {
			CLI_LOGE("recv buffer malloc failed\r\n");
			return;
		}
		os_memset(recv_data, 0xff, recv_len);
		int ret = bk_spi_transmit(spi_id, send_data, send_len, recv_data, recv_len);
		if (ret < 0) {
			CLI_LOGE("spi transmit failed, ret:-0x%x\r\n", -ret);
			goto transmit_exit;
		}
		for (int i = 0; i < recv_len; i++) {
			CLI_LOGI("recv_buffer[%d]=0x%x\r\n", i, recv_data[i]);
		}
transmit_exit:
		if (send_data) {
			os_free(send_data);
		}
		send_data = NULL;

		if (recv_data) {
			os_free(recv_data);
		}
		recv_data = NULL;
	}
#if CONFIG_SPI_DMA
	else if (os_strcmp(argv[2], "dma_write") == 0) {
		uint32_t buf_len = os_strtoul(argv[3], NULL, 10);
		uint8_t *send_data = (uint8_t *)os_zalloc(buf_len);
		if (send_data == NULL) {
			CLI_LOGE("send buffer malloc failed\r\n");
			return;
		}
		for (int i = 0; i < buf_len; i++) {
			send_data[i] = i & 0xff;
		}
		BK_LOG_ON_ERR(bk_spi_dma_write_bytes(spi_id, send_data, buf_len));
		if (send_data) {
			os_free(send_data);
		}
		send_data = NULL;
		CLI_LOGI("spi dma send, spi_id=%d, data_len=%d\n", spi_id, buf_len);
	} else if (os_strcmp(argv[2], "dma_read") == 0) {
		uint32_t buf_len = os_strtoul(argv[3], NULL, 10);
		uint8_t *recv_data = (uint8_t *)os_malloc(buf_len);
		if (recv_data == NULL) {
			CLI_LOGE("recv buffer malloc failed\r\n");
			return;
		}
		os_memset(recv_data, 0xff, buf_len);
		BK_LOG_ON_ERR(bk_spi_dma_read_bytes(spi_id, recv_data, buf_len));
		CLI_LOGI("spi dma recv, spi_id=%d, data_len=%d\n", spi_id, buf_len);
		for (int i = 0; i < buf_len; i++) {
			CLI_LOGI("recv_buffer[%d]=0x%x\n", i, recv_data[i]);
		}
		if (recv_data) {
			os_free(recv_data);
		}
		recv_data = NULL;
	}
#endif
	else {
		cli_spi_help();
		return;
	}
}

static void cli_spi_config_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t spi_id;

	if (argc < 4) {
		cli_spi_help();
		return;
	}

	spi_id = os_strtoul(argv[1], NULL, 10);

	if (os_strcmp(argv[2], "baud_rate") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		uint32_t baud_rate = os_strtoul(argv[3], NULL, 10);
		BK_LOG_ON_ERR(bk_spi_set_baud_rate(spi_id, baud_rate));
		CLI_LOGI("spi(%d) config baud_rate:%d\n", spi_id, baud_rate);
	} else if (os_strcmp(argv[2], "mode") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		uint32_t mode = os_strtoul(argv[3], NULL, 10);
		BK_LOG_ON_ERR(bk_spi_set_mode(spi_id, mode));
		CLI_LOGI("spi(%d) config mode:%d\n", spi_id, mode);
	} else if (os_strcmp(argv[2], "bit_width") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		uint32_t bit_width = os_strtoul(argv[3], NULL, 10);
		if (bit_width == 16) {
			BK_LOG_ON_ERR(bk_spi_set_bit_width(spi_id, SPI_BIT_WIDTH_16BITS));
		} else {
			bit_width = 8;
			BK_LOG_ON_ERR(bk_spi_set_bit_width(spi_id, SPI_BIT_WIDTH_8BITS));
		}
		CLI_LOGI("spi(%d) config bit_width:%d\n", spi_id, bit_width);
	} else if (os_strcmp(argv[2], "wire_mode") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		uint32_t wire_mode = os_strtoul(argv[3], NULL, 10);
		if (wire_mode == 3) {
			BK_LOG_ON_ERR(bk_spi_set_wire_mode(spi_id, SPI_3WIRE_MODE));
		} else {
			BK_LOG_ON_ERR(bk_spi_set_wire_mode(spi_id, SPI_4WIRE_MODE));
		}
		CLI_LOGI("spi(%d) config wire_mode:%d\n", spi_id, wire_mode);
	} else if (os_strcmp(argv[2], "bit_order") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		if (os_strcmp(argv[3], "LSB") == 0) {
			BK_LOG_ON_ERR(bk_spi_set_bit_order(spi_id, SPI_LSB_FIRST));
		} else {
			BK_LOG_ON_ERR(bk_spi_set_bit_order(spi_id, SPI_MSB_FIRST));
		}
		CLI_LOGI("spi(%d) config bit_order:%s\n", spi_id, argv[3]);
	} else {
		cli_spi_help();
		return;
	}
}

static void cli_spi_int_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t spi_id;

	if (argc != 4) {
		cli_spi_help();
		return;
	}

	spi_id = os_strtoul(argv[1], NULL, 10);
	if (os_strcmp(argv[2], "reg") == 0) {
		if (os_strcmp(argv[3], "tx") == 0) {
			BK_LOG_ON_ERR(bk_spi_register_tx_finish_isr(spi_id, cli_spi_tx_isr, NULL));
			CLI_LOGI("spi id:%d register tx finish interrupt isr\n", spi_id);
		} else {
			BK_LOG_ON_ERR(bk_spi_register_rx_isr(spi_id, cli_spi_rx_isr, NULL));
			CLI_LOGI("spi id:%d register rx interrupt isr\n", spi_id);
		}
	} else {
		cli_spi_help();
		return;
	}
}

#define SPI_CMD_CNT (sizeof(s_spi_commands) / sizeof(struct cli_command))
static const struct cli_command s_spi_commands[] = {
	{"spi_driver", "spi_driver {init|deinit}", cli_spi_driver_cmd},
	{"spi", "spi {init|write|read}", cli_spi_cmd},
	{"spi_config", "spi_config {id} {mode|baud_rate} [...]", cli_spi_config_cmd},
	{"spi_int", "spi_int {id} {reg} {tx|rx}", cli_spi_int_cmd},
};

int cli_spi_init(void)
{
	BK_LOG_ON_ERR(bk_spi_driver_init());
	return cli_register_commands(s_spi_commands, SPI_CMD_CNT);
}


