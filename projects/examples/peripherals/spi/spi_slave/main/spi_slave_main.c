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

#include <components/log.h>
#include <driver/spi.h>
#include <os/mem.h>
#include <os/os.h>
#include "sys_rtos.h"
#include <common/sys_config.h>

#define TAG "spi_exam"
#define CONFIG_SPI_EXAM_SPI_ID     SPI_ID_0
#define CONFIG_SPI_EXAM_BAUD_RATE  10000000
#define CONFIG_SPI_EXAM_BUF_LEN    1024
#define CONFIG_SPI_EXAM_TX_TIMEOUT BEKEN_WAIT_FOREVER
#define CONFIG_SPI_EXAM_RX_TIMEOUT BEKEN_WAIT_FOREVER

static bk_err_t spi_example_send_data(void)
{
	spi_config_t config = {
		.role = SPI_ROLE_SLAVE,
		.bit_width = SPI_BIT_WIDTH_8BITS,
		.polarity = SPI_POLARITY_HIGH,
		.phase = SPI_PHASE_2ND_EDGE,
		.wire_mode = SPI_4WIRE_MODE,
		.baud_rate = CONFIG_SPI_EXAM_BAUD_RATE,
		.bit_order = SPI_MSB_FIRST,
		.dma_mode = SPI_DMA_MODE_DISABLE,
	};
	uint8_t send_exam_buf[CONFIG_SPI_EXAM_BUF_LEN] = {0};
	for (int i = 0; i < CONFIG_SPI_EXAM_BUF_LEN; i++) {
		send_exam_buf = i & 0xff;
	}
	BK_LOG_ON_ERR(bk_spi_init(CONFIG_SPI_EXAM_SPI_ID, &config));
	BK_LOG_ON_ERR(bk_spi_write_bytes(CONFIG_SPI_EXAM_SPI_ID,
									send_exam_buf,
									CONFIG_SPI_EXAM_BUF_LEN,
									CONFIG_SPI_EXAM_TX_TIMEOUT));
	return BK_OK;
}

static bk_err_t spi_example_dma_send_data(void)
{
	spi_config_t config = {
		.role = SPI_ROLE_SLAVE,
		.bit_width = SPI_BIT_WIDTH_8BITS,
		.polarity = SPI_POLARITY_HIGH,
		.phase = SPI_PHASE_2ND_EDGE,
		.wire_mode = SPI_4WIRE_MODE,
		.baud_rate = CONFIG_SPI_EXAM_BAUD_RATE,
		.bit_order = SPI_MSB_FIRST,
		.dma_mode = SPI_DMA_MODE_ENABLE,
	};
	uint8_t send_exam_buf[CONFIG_SPI_EXAM_BUF_LEN] = {0};
	for (int i = 0; i < CONFIG_SPI_EXAM_BUF_LEN; i++) {
		send_exam_buf = i & 0xff;
	}

	BK_LOG_ON_ERR(bk_spi_init(CONFIG_UART_EXAMPLE_UART_ID, &s_config));
	BK_LOG_ON_ERR(bk_spi_dma_write_bytes(CONFIG_SPI_EXAM_SPI_ID,
										send_exam_buf,
										CONFIG_SPI_EXAM_BUF_LEN,
										CONFIG_SPI_EXAM_TX_TIMEOUT));
	return BK_OK;
}

static bk_err_t spi_example_recv_data(void)
{
	spi_config_t config = {
		.role = SPI_ROLE_SLAVE,
		.bit_width = SPI_BIT_WIDTH_8BITS,
		.polarity = SPI_POLARITY_HIGH,
		.phase = SPI_PHASE_2ND_EDGE,
		.wire_mode = SPI_4WIRE_MODE,
		.baud_rate = CONFIG_SPI_EXAM_BAUD_RATE,
		.bit_order = SPI_MSB_FIRST,
		.dma_mode = SPI_DMA_MODE_DISABLE,
	};
	uint8_t recv_exam_buf[CONFIG_SPI_EXAM_BUF_LEN] = {0};
	BK_LOG_ON_ERR(bk_spi_init(CONFIG_SPI_EXAM_SPI_ID, &config));
	BK_LOG_ON_ERR(bk_spi_read_bytes(CONFIG_SPI_EXAM_SPI_ID,
									recv_exam_buf,
									CONFIG_SPI_EXAM_BUF_LEN,
									CONFIG_SPI_EXAM_RX_TIMEOUT));
	for (int i = 0; i < CONFIG_SPI_EXAM_BUF_LEN; i++) {
		BK_LOGI(TAG, "recv_data[%d]=%x\r\n", i, recv_exam_buf[i]);
	}
	return BK_OK;
}

static bk_err_t spi_example_dma_recv_data(void)
{
	spi_config_t config = {
		.role = SPI_ROLE_SLAVE,
		.bit_width = SPI_BIT_WIDTH_8BITS,
		.polarity = SPI_POLARITY_HIGH,
		.phase = SPI_PHASE_2ND_EDGE,
		.wire_mode = SPI_4WIRE_MODE,
		.baud_rate = CONFIG_SPI_EXAM_BAUD_RATE,
		.bit_order = SPI_MSB_FIRST,
		.dma_mode = SPI_DMA_MODE_ENABLE,
	};
	uint8_t recv_exam_buf[CONFIG_SPI_EXAM_BUF_LEN] = {0};
	BK_LOG_ON_ERR(bk_spi_init(CONFIG_SPI_EXAM_SPI_ID, &config));
	BK_LOG_ON_ERR(bk_spi_dma_read_bytes(CONFIG_SPI_EXAM_SPI_ID,
										recv_exam_buf,
										CONFIG_SPI_EXAM_BUF_LEN,
										CONFIG_SPI_EXAM_RX_TIMEOUT));
	for (int i = 0; i < CONFIG_SPI_EXAM_BUF_LEN; i++) {
		BK_LOGI(TAG, "dma_recv_data[%d]=%x\r\n", i, recv_exam_buf[i]);
	}
	return BK_OK;
}

int main(void)
{
	BK_LOG_ON_ERR(bk_spi_driver_init());
	BK_LOG_ON_ERR(spi_example_send_data());
	BK_LOG_ON_ERR(spi_example_dma_send_data());
	BK_LOG_ON_ERR(spi_example_recv_data());
	BK_LOG_ON_ERR(spi_example_dma_recv_data());

	return 0;
}

