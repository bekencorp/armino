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
#include <driver/dma.h>
#include <driver/uart.h>
#include "bk_general_dma.h"

#define SEND_BUF_LEN 1024

static void cli_dma_help(void)
{
    CLI_LOGI("dma_driver {init|deinit}\n");
    CLI_LOGI("dma {id} {init|deinit|start|stop}\n");
    CLI_LOGI("dma_int {id} {reg|enable|disable}\n");
    CLI_LOGI("dma_chnl alloc \n");
    CLI_LOGI("dma_chnl_free free {id} \n");
    CLI_LOGI("dma_memcopy_test {copy} {count|in_number1|in_number2|out_number1|out_number2}\r\n");
    CLI_LOGI("dma_chnl_test {start|stop} {uart1|uart2|uart3} {wait_ms}\r\n");
}

static void cli_dma_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    if (argc < 2) {
        cli_dma_help();
        return;
    }

    if (os_strcmp(argv[1], "init") == 0) {
        BK_LOG_ON_ERR(bk_dma_driver_init());
        CLI_LOGI("dma driver init\n");
    } else if (os_strcmp(argv[1], "deinit") == 0) {
        BK_LOG_ON_ERR(bk_dma_driver_deinit());
        CLI_LOGI("dma driver deinit\n");
    } else {
        cli_dma_help();
        return;
    }
}

static void cli_dma_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint32_t id;

    if (argc < 2) {
        cli_dma_help();
        return;
    }

    id = os_strtoul(argv[1], NULL, 10);

    if (os_strcmp(argv[2], "init") == 0) {
        CLI_RET_ON_INVALID_ARGC(argc, 3);
        dma_config_t config = {
            .mode = DMA_WORK_MODE_SINGLE,
            .chan_prio = 2,
            .src = {
                .dev = DMA_DEV_DTCM,
                .width = DMA_DATA_WIDTH_32BITS,
                .addr_inc_en = DMA_ADDR_INC_ENABLE,
                .addr_loop_en = DMA_ADDR_LOOP_DISABLE,
                .start_addr = 0,
                .end_addr = 0,
            },
            .dst = {
                .dev = DMA_DEV_UART1,
                .width = DMA_DATA_WIDTH_8BITS,
                .addr_inc_en = DMA_ADDR_INC_DISABLE,
                .addr_loop_en = DMA_ADDR_LOOP_DISABLE,
                .start_addr = 0x0802100 + 0x3 * 4,
                .end_addr = 0,
            },
        };
        BK_LOG_ON_ERR(bk_dma_init(id, &config));
        CLI_LOGI("dma init, id=%d\n", id);
    }else if (os_strcmp(argv[2], "start") == 0) {
        BK_LOG_ON_ERR(bk_dma_start(id));
        CLI_LOGI("dma start, id=%d\n", id);
    } else if (os_strcmp(argv[2], "stop") == 0) {
        BK_LOG_ON_ERR(bk_dma_stop(id));
        CLI_LOGI("dma stop, id=%d\n", id);
    } else if (os_strcmp(argv[2], "deinit") == 0) {
        BK_LOG_ON_ERR(bk_dma_deinit(id));
        CLI_LOGI("dma deinit, id=%d\n", id);
    } else if (os_strcmp(argv[2], "set_tran_len") == 0) {
        BK_LOG_ON_ERR(bk_dma_set_transfer_len(id, SEND_BUF_LEN));
        CLI_LOGI("dma set tran len, id=%d, len=%d\n", id, SEND_BUF_LEN);
    } else if (os_strcmp(argv[2], "get_remain_len") == 0) {
        uint32_t remain_len = bk_dma_get_remain_len(id);
        CLI_LOGI("dma get remain_len, id=%d, len=%x\n", id, remain_len);
    }else {
        cli_dma_help();
        return;
    }
}

static void cli_dma_half_finish_isr(dma_id_t dma_id)
{
    CLI_LOGI("dma half finish isr(%d)\n", dma_id);
}

static void cli_dma_finish_isr(dma_id_t dma_id)
{
    CLI_LOGI("dma finish isr(%d)\n", dma_id);
}

static void cli_dma_int_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint32_t id;

    if (argc != 3) {
        cli_dma_help();
        return;
    }

    id = os_strtoul(argv[1], NULL, 10);

    if (os_strcmp(argv[2], "reg") == 0) {
        BK_LOG_ON_ERR(bk_dma_register_isr(id, cli_dma_half_finish_isr, cli_dma_finish_isr));
        CLI_LOGI("dma id:%d register interrupt isr\n", id);
    } else if (os_strcmp(argv[2], "enable_hf_fini") == 0) {
        BK_LOG_ON_ERR(bk_dma_enable_half_finish_interrupt(id));
        CLI_LOGI("dma id%d enable half finish interrupt\n", id);
    } else if (os_strcmp(argv[2], "disable_hf_fini") == 0) {
        BK_LOG_ON_ERR(bk_dma_disable_half_finish_interrupt(id));
        CLI_LOGI("dma id%d disable half finish interrupt\n", id);
    } else if (os_strcmp(argv[2], "enable_fini") == 0) {
        BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(id));
        CLI_LOGI("dma id%d enable finish interrupt\n", id);
    } else if (os_strcmp(argv[2], "disable_fini") == 0) {
        BK_LOG_ON_ERR(bk_dma_disable_finish_interrupt(id));
        CLI_LOGI("dma id%d disable finish interrupt\n", id);
    } else {
        cli_dma_help();
        return;
    }
}

static void cli_dma_chnl_alloc(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint8_t id;

    if (argc < 2) {
        cli_dma_help();
        return;
    }

    if (os_strcmp(argv[1], "alloc") == 0) {
	 id = bk_dma_alloc(DMA_DEV_MAX);
        CLI_LOGI("dma channel id:%x\n", id);
    } else {
        cli_dma_help();
        return;
    }
}

static void cli_dma_chnl_free(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint32_t id;
    uint32_t ret;

    if (argc < 2) {
        cli_dma_help();
        return;
    }

    if (os_strcmp(argv[1], "free") == 0) {
	id = os_strtoul(argv[2], NULL, 10);
	ret = bk_dma_free(DMA_DEV_MAX, id);
	CLI_LOGI("dma channel free id:%d ret:%d\n", id, ret);
    } else {
	CLI_LOGI("cli_dma_chnl_free NOT free\n");
       cli_dma_help();
       return;
    }
}

static void cli_dma_memcpy_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{

    if (argc < 7) {
        cli_dma_help();
        return;
    }
    uint32_t *out_buffer = NULL;
    uint32_t *in_buffer = NULL;
    uint32_t in_write_buffer[2] = {0};
	uint32_t out_write_buffer[2] = {0};

    int count = 1;
    if (os_strcmp(argv[1], "copy") == 0) {
        count = os_strtoul(argv[2], NULL, 10);

        in_write_buffer[0] = os_strtoul(argv[3], NULL, 10);
        CLI_LOGD("cli_dma_memcpy_TEST IN_buffer[0]: %d\r\n", in_write_buffer[0]);
        in_write_buffer[1] = os_strtoul(argv[4], NULL, 10);
        CLI_LOGD("cli_dma_memcpy_TEST IN_buffer[1]: %d\r\n", in_write_buffer[1]);
        in_buffer = in_write_buffer;


        out_write_buffer[0] = os_strtoul(argv[5], NULL, 10);
        CLI_LOGD("cli_dma_memcpy_TEST OUT_buffer[0]: %d\r\n", out_write_buffer[0]);
        out_write_buffer[1] = os_strtoul(argv[6], NULL, 10);
        CLI_LOGD("cli_dma_memcpy_TEST OUT_buffer[1]: %d\r\n", out_write_buffer[1]);
        out_buffer = out_write_buffer;

        while(count) {
              for(int j = 0; j < 2; j++) {
                  CLI_LOGI("cli_dma_memcpy_test BEFORE IN_buffer[%d]: %d\r\n", j,in_buffer[j]);
                  CLI_LOGI("cli_dma_memcpy_test BEFORE OUT_buffer[%d]: %d\r\n", j,out_buffer[j]);
              }
              CLI_LOGI("===============================================\r\n");

              dma_memcpy(out_buffer, in_buffer, 2 * sizeof(uint32_t));

              for(int i = 0; i < 2; i++) {
                  CLI_LOGI("cli_dma_memcpy_test AFTER IN_buffer[%d]: %d\r\n", i,in_buffer[i]);
                  CLI_LOGI("cli_dma_memcpy_test AFTER OUT_buffer[%d]: %d\r\n", i,out_buffer[i]);
              }
              CLI_LOGI("#############################################\r\n");

              count--;
        }

    } else {
        cli_dma_help();
        return;
    }
}

static beken_semaphore_t dma_chnl_test_semaphore = NULL;
static beken_thread_t dma_chnl_test_handle = NULL;
static uint16_t dma_chnl_get_data_uart_id = UART_ID_MAX;
static uint16_t dma_chnl_wait_ms = 1000;

static void dma_chnl_primenumber_copy_finish_isr(void)
{
	CLI_LOGD("dma_channel test PRIMENUMBER copy finish!\r\n");
}

static uint32_t dma_chnl_primenumber_copy(void)
{
    dma_config_t dma_config;

	int length = 3;
	uint32_t *trng_inbuffer = NULL;
	uint32_t *trng_outbuffer = NULL;
	uint32_t set_inbuffer[3] = {21341, 16931, 11239};
	uint32_t set_outbuffer[3] = {9929, 14633, 22727};
	uint32_t irq_level;

	trng_inbuffer = set_inbuffer;
	trng_outbuffer = set_outbuffer;
    os_memset(&dma_config, 0, sizeof(dma_config_t));

    dma_config.mode = DMA_WORK_MODE_SINGLE;
    dma_config.chan_prio = 0;

    dma_config.src.dev = DMA_DEV_DTCM;
    dma_config.src.width = DMA_DATA_WIDTH_32BITS;
    dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
    dma_config.src.start_addr = (uint32_t)trng_inbuffer;
    dma_config.src.end_addr = (uint32_t)(trng_inbuffer + length * sizeof(uint32_t));

    dma_config.dst.dev = DMA_DEV_DTCM;
    dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
    dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
    dma_config.dst.start_addr = (uint32_t)trng_outbuffer;
    dma_config.dst.end_addr = (uint32_t)(trng_outbuffer + length * sizeof(uint32_t));

    dma_id_t cpy_chnl = bk_dma_alloc(DMA_DEV_DTCM);
    CLI_LOGI("dma_chnl_test_PRIMENUMBER dma_chnl: %d\r\n", cpy_chnl);

    if(cpy_chnl >= DMA_ID_MAX) {
        return BK_FAIL;
    }

	irq_level = rtos_disable_int();

    bk_dma_init(cpy_chnl, &dma_config);
    bk_dma_register_isr(cpy_chnl, NULL, (void *)dma_chnl_primenumber_copy_finish_isr);
    bk_dma_enable_finish_interrupt(cpy_chnl);
    bk_dma_set_transfer_len(cpy_chnl, length * sizeof(uint32_t));
    bk_dma_start(cpy_chnl);
    bk_dma_get_enable_status(cpy_chnl);
	bk_dma_free(DMA_DEV_DTCM, cpy_chnl);

    rtos_enable_int(irq_level);

	for(int i = 0; i < length; i++) {
		if(set_inbuffer[i] == set_outbuffer[i])
			CLI_LOGD("copy PRIMENUMBER right\r\n");
		else
			CLI_LOGI("copy PRIMENUMBER ERROR!!!\r\n");
	}

    return BK_OK;

}

static void dma_chnl_data_from_uart_copy_finish(void)
{
	CLI_LOGD("dma_channel test UART_DATA copy finish isr\r\n");
}

static uint32_t dma_chnl_data_from_uart_copy(void *out, const void *in, uint32_t len)
{
	dma_config_t dma_config;
	uint32_t irq_level;

	os_memset(&dma_config, 0, sizeof(dma_config_t));

	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;

	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.src.start_addr = (uint32_t)in;
	dma_config.src.end_addr = (uint32_t)(in + len);

	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.start_addr = (uint32_t)out;
	dma_config.dst.end_addr = (uint32_t)(out + len);

	dma_id_t cpy_chnl = bk_dma_alloc(DMA_DEV_DTCM);
	CLI_LOGI("UART FROM DATA copy dma_chnl: %d\r\n", cpy_chnl);

	if(cpy_chnl >= DMA_ID_MAX) {
		return BK_FAIL;
	}

	irq_level = rtos_disable_int();

	bk_dma_init(cpy_chnl, &dma_config);
	bk_dma_register_isr(cpy_chnl, NULL, (void *)dma_chnl_data_from_uart_copy_finish);
	bk_dma_enable_finish_interrupt(cpy_chnl);
	bk_dma_set_transfer_len(cpy_chnl, len);
	bk_dma_start(cpy_chnl);
	bk_dma_get_enable_status(cpy_chnl);
	bk_dma_free(DMA_DEV_DTCM, cpy_chnl);

	rtos_enable_int(irq_level);

	return BK_OK;
}

static void dma_channel_test_uart_rx_isr(uart_id_t id, void *param)
{
	int ret;

	ret = rtos_set_semaphore(&dma_chnl_test_semaphore);
	if(kNoErr !=ret)
		CLI_LOGI("dma_channel_uart_rx_isr: uart set sema failed\r\n");
	return;
}

static void cli_dma_channel_test(uint32_t data)
{

	char *msg = NULL;
	int ret = -1;
	int cnt = 0;
	uint8_t rx_data;
	char uart_get_data[INBUF_SIZE];
	char dma_out_buffer[INBUF_SIZE];

	if(NULL == dma_chnl_test_semaphore)
	{
		ret = rtos_init_semaphore(&dma_chnl_test_semaphore, 1);
		if (kNoErr != ret)
			CLI_LOGI("cli_dma_channel_test: create background sema failed\r\n");
	}

	while (1) {

		dma_chnl_primenumber_copy();

		ret = rtos_get_semaphore(&dma_chnl_test_semaphore, dma_chnl_wait_ms);
		if(kNoErr == ret) {
			while(1)  /* read all data from rx-FIFO. */
			{
				ret = uart_read_byte_ex(dma_chnl_get_data_uart_id, &rx_data);
				if (ret == -1)
					break;

				uart_get_data[cnt] = (char)rx_data;
				cnt++;

				if(cnt >= INBUF_SIZE)
					break;
			}

			if (cnt <= 0)
				break;

			BK_LOG_ON_ERR(dma_chnl_data_from_uart_copy(dma_out_buffer, uart_get_data, cnt));
			BK_LOG_ON_ERR(bk_uart_write_bytes(dma_chnl_get_data_uart_id, dma_out_buffer, cnt));
			CLI_LOGI("copy From UART Get/Send OK!\r\n");

			for(int i = 0; i < cnt; i++) {
				if(dma_out_buffer[i] == uart_get_data[i]) {
					CLI_LOGD("copy From UART right!!!\r\n");
					dma_out_buffer[i] = 0;
				} else
					CLI_LOGI("copy From UART ERROR\r\n");
			}

			BK_LOG_ON_ERR(dma_memcpy(dma_out_buffer, uart_get_data, cnt * sizeof(char)));
			CLI_LOGI("copy dma_MEMCPY OK!\r\n");

			for(int k = 0; k < cnt; k++) {
				if(dma_out_buffer[k] == uart_get_data[k]) {
					CLI_LOGD("copy dma_MEMCPY right!!!\r\n");
					dma_out_buffer[k] = 0;
				} else
					CLI_LOGI("copy dma_MEMCPY ERROR\r\n");
			}

			if (cnt > 0) {
				for (int j = 0;j < cnt; j++)
					uart_get_data[j] = 0;
				cnt = 0;
			}
		}

		msg = uart_get_data;
		if (os_strcmp(msg, EXIT_MSG) == 0)
			os_printf("cli_dma_channel_test: EXIT_MSG\r\n");
	}

}

static void cli_dma_chnl_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 3) {
		cli_dma_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {

		if (!dma_chnl_test_handle) {
			if (os_strcmp(argv[2], "uart1") == 0) {
				dma_chnl_get_data_uart_id = UART_ID_0;
				CLI_LOGI("cli_uart_test_cmd Maybe UART1 for log output!!!\n");
			} else if (os_strcmp(argv[2], "uart2")== 0) {
				dma_chnl_get_data_uart_id = UART_ID_1;
				CLI_LOGI("cli_uart_test_cmd Maybe UART2 for log output!!!\n");
			} else if (os_strcmp(argv[2], "uart3")== 0) {
				dma_chnl_get_data_uart_id = UART_ID_2;
				CLI_LOGI("cli_uart_test_cmd Maybe UART3 for log output!!!\n");
			} else {
					cli_dma_help();
					return;
			}
		} else {
			CLI_LOGI("dma_chnl_test TASK WARKING!!!\n");
			return;
		}

		uint32_t wait_ms = 0;
		wait_ms = os_strtoul(argv[3], NULL, 10);

		if (wait_ms)
			dma_chnl_wait_ms = wait_ms;


		uart_config_t config = {0};
		os_memset(&config, 0, sizeof(uart_config_t));

		config.baud_rate = UART_BAUD_RATE;
		config.data_bits = UART_DATA_8_BITS;
		config.parity = UART_PARITY_NONE;
		config.stop_bits = UART_STOP_BITS_1;
		config.flow_ctrl = UART_FLOWCTRL_DISABLE;
		config.src_clk = UART_SCLK_XTAL_26M;

		BK_LOG_ON_ERR(bk_uart_init(dma_chnl_get_data_uart_id, &config));

		BK_LOG_ON_ERR(bk_uart_disable_sw_fifo(dma_chnl_get_data_uart_id));
		BK_LOG_ON_ERR(bk_uart_register_rx_isr(dma_chnl_get_data_uart_id,
					  (uart_isr_t)dma_channel_test_uart_rx_isr, NULL));
		BK_LOG_ON_ERR(bk_uart_enable_rx_interrupt(dma_chnl_get_data_uart_id));

		if(rtos_create_thread(&dma_chnl_test_handle, 8, "dma_chnl_test",
					(beken_thread_function_t) cli_dma_channel_test, 2048, 0)) {
				CLI_LOGI("cli_dma_channel_test rtos_create_thread FAILED!\n");
				return;
		}

	}

	if (os_strcmp(argv[1], "stop") == 0) {

		if (os_strcmp(argv[2], "uart1") == 0) {
			if(dma_chnl_get_data_uart_id != UART_ID_0) {
				CLI_LOGI("PLEASE enter a correct ID\n");
				return;
			} else
					dma_chnl_get_data_uart_id = UART_ID_0;
		} else if (os_strcmp(argv[2], "uart2")== 0) {
			if(dma_chnl_get_data_uart_id != UART_ID_1) {
				CLI_LOGI("PLEASE enter a correct ID\n");
				return;
			} else
				dma_chnl_get_data_uart_id = UART_ID_1;
		} else if (os_strcmp(argv[2], "uart3")== 0) {
			if(dma_chnl_get_data_uart_id != UART_ID_2) {
				CLI_LOGI("PLEASE enter a correct ID\n");
				return;
			} else
				dma_chnl_get_data_uart_id = UART_ID_2;
		} else {
			cli_dma_help();
			return;
		}

		rtos_delete_thread(&dma_chnl_test_handle);
		dma_chnl_test_handle = NULL;
		BK_LOG_ON_ERR(bk_uart_disable_rx_interrupt(dma_chnl_get_data_uart_id));
		BK_LOG_ON_ERR(bk_uart_register_rx_isr(dma_chnl_get_data_uart_id, NULL, NULL));
		BK_LOG_ON_ERR(bk_uart_deinit(dma_chnl_get_data_uart_id));
		dma_chnl_get_data_uart_id = UART_ID_MAX;
		dma_chnl_wait_ms = 1000;

		CLI_LOGI("idle_uart_out task stop\n");
	}
}

#define DMA_CMD_CNT (sizeof(s_dma_commands) / sizeof(struct cli_command))
static const struct cli_command s_dma_commands[] = {
    {"dma_driver", "dma_driver {init|deinit}", cli_dma_driver_cmd},
    {"dma", "dma {id} {init|deinit|start|stop|set_tran_len|get_remain_len}", cli_dma_cmd},
    {"dma_int", "dma_int {id} {reg|enable_hf_fini|disable_hf_fini|enable_fini|disable_fini}", cli_dma_int_cmd},
    {"dma_chnl", "dma_chnl alloc", cli_dma_chnl_alloc},
    {"dma_chnl_free", "dma_chnl_free {id}", cli_dma_chnl_free},
    {"dma_memcopy_test", "copy {count|in_number1|in_number2|out_number1|out_number2}", cli_dma_memcpy_test},
    {"dma_chnl_test", "{start|stop} {uart1|uart2|uart3} {wait_ms}", cli_dma_chnl_test_cmd},
};

int cli_dma_init(void)
{
    BK_LOG_ON_ERR(bk_dma_driver_init());
    return cli_register_commands(s_dma_commands, DMA_CMD_CNT);
}

