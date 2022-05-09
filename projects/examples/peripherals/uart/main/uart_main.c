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

#include <common/sys_config.h>
#include <components/log.h>
#include <os/mem.h>
#include <driver/uart.h>

#define TAG "uart_example"
#define UART_EXAMPLE_BUF_LEN       128
#define UART_EXAMPLE_RX_TIMEOUT    BEKEN_WAIT_FOREVER

static const uart_config_t s_config = {
	.baud_rate = CONFIG_UART_EXAMPLE_BAUD_RATE,
	.data_bits = CONFIG_UART_EXAMPLE_DATA_BITS,
	.parity = CONFIG_UART_EXAMPLE_PARITY,
	.stop_bits = CONFIG_UART_EXAMPLE_STOP_BITS,
	.flow_ctrl = CONFIG_UART_EXAMPLE_FLOW_CTRL,
	.src_clk = CONFIG_UART_EXAMPLE_SRC_CLK,
};

static void uart_example_rx_isr(uart_id_t id, void *param)
{
	BK_LOGI(TAG, "uart(%d) enter uart_example_rx_isr\r\n", id);
}

static bk_err_t uart_example_recv_data(void)
{
	BK_LOG_ON_ERR(bk_uart_init(CONFIG_UART_EXAMPLE_UART_ID, &s_config));
	BK_LOG_ON_ERR(bk_uart_enable_rx_interrupt(CONFIG_UART_EXAMPLE_UART_ID));
	BK_LOG_ON_ERR(bk_uart_register_rx_isr(CONFIG_UART_EXAMPLE_UART_ID, uart_example_rx_isr, NULL));

	uint8_t *data = (uint8_t *)os_malloc(UART_EXAMPLE_BUF_LEN);
	BK_RETURN_ON_NULL(data);
	os_memset(data, 0, UART_EXAMPLE_BUF_LEN);

	int len = bk_uart_read_bytes(CONFIG_UART_EXAMPLE_UART_ID, data, UART_EXAMPLE_BUF_LEN, UART_EXAMPLE_RX_TIMEOUT);
	if (len < 0) {
		BK_LOGE(TAG, "uart read failed, ret:-0x%x\r\n", -len);
		goto exit;
	}

	BK_LOGI(TAG, "uart recv data:\r\n");
	for (int i = 0; i < len; i++) {
		BK_LOGI(TAG, "data[%d]=0x%x\r\n", i, data[i]);
	}

exit:
	if (!data) {
		os_free(data);
	}
	data = NULL;

	return BK_OK;
}

static bk_err_t uart_example_send_data(void)
{
	BK_LOG_ON_ERR(bk_uart_init(CONFIG_UART_EXAMPLE_UART_ID, &s_config));

	uint8_t *data = (uint8_t *)os_malloc(UART_EXAMPLE_BUF_LEN);
	BK_RETURN_ON_NULL(data);
	os_memset(data, 0, UART_EXAMPLE_BUF_LEN);

	BK_LOGI(TAG, "uart send data:\r\n");
	for (int i = 0; i < UART_EXAMPLE_BUF_LEN; i++) {
		data[i] = i & 0xff;
		BK_LOGI(TAG, "data[%d]=0x%x\r\n", i, data[i]);
	}
	BK_LOG_ON_ERR(bk_uart_write_bytes(CONFIG_UART_EXAMPLE_UART_ID, data, UART_EXAMPLE_BUF_LEN));
	if (!data) {
		os_free(data);
	}

	return BK_OK;
}

int main(void)
{
	BK_LOG_ON_ERR(bk_uart_driver_init());

	BK_LOG_ON_ERR(uart_example_send_data());
	BK_LOG_ON_ERR(uart_example_recv_data());

	return 0;
}
