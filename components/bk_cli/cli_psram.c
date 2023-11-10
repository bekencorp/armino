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

#include "cli.h"

#include <os/os.h>
#include <driver/psram.h>
#include <driver/trng.h>
#include "bk_general_dma.h"

#if (CONFIG_ARCH_RISCV)
extern u64 riscv_get_mtimer(void);

#define PSRAM_QITEM_COUNT      (30)

#define PSRAM_TEST_LEN               (1024 * 4)

#define write_data(addr,val)                 *((volatile uint32_t *)(addr)) = val
#define read_data(addr,val)                  val = *((volatile uint32_t *)(addr))
#define get_addr_data(addr)                  *((volatile uint32_t *)(addr))

beken_thread_t  psram_thread_hdl = NULL;

typedef struct {
	uint8_t test_running;
	uint8_t test_mode;
	uint8_t cacheable;
	uint32_t *data;
	uint32_t delay_time;
} psram_debug_t;

psram_debug_t * psram_debug = NULL;

static void cli_psram_help(void)
{
	CLI_LOGI("psram_test {start|stop}\n");
}


static void psram_test_dtcm(void)
{
	uint32_t i = 0;
	uint32_t error_num = 0;
	uint64_t timer0, timer1;
	uint32_t total_time = 0;
	uint32_t test_len = 1024 * 1024 * 8;
	uint32_t value = 0;
	uint32_t base_addr = 0x60000000;

	if (psram_debug->cacheable)
	{
		base_addr = 0x64000000;
	}

	CLI_LOGI("begin write %08x, len:8Mbyte\r\n", base_addr);
	timer0 = riscv_get_mtimer();
	for (i = 0; i < test_len / 4; i++)
		write_data((base_addr + i * 0x4), 0x11111111 + i);

	timer1 = riscv_get_mtimer();
	total_time = (uint32_t) (timer1 - timer0)/26000;
	CLI_LOGI("finish write, use time: %ld ms, write_rate:%ld byte/ms\r\n", total_time, test_len / total_time);

	CLI_LOGI("begin read %08x, len:8Mbyte\r\n", base_addr);
	timer0 = riscv_get_mtimer();
	for (i = 0; i < test_len / 4; i++)
		read_data((base_addr + i * 0x4), value);

	timer1 = riscv_get_mtimer();
	total_time = (uint32_t) (timer1 - timer0)/26000;
	CLI_LOGI("finish read, use time: %ld ms, read_rate:%ld byte/ms\r\n", total_time, test_len / total_time);

	CLI_LOGI("begin compare %08x, len:8Mbyte\r\n", base_addr);

	for (i = 0; i < test_len / 4; i++)
	{
		value = get_addr_data(base_addr + i * 0x4);
		if (value != (0x11111111 + i))
			error_num++;
	}

	CLI_LOGI("finish cpmpare, error_num: %ld, corr_rate: %ld.\r\n", error_num, ((test_len - error_num) * 100 / test_len));

	rtos_delay_milliseconds(psram_debug->delay_time);

}

static void psram_write_continue_test(void)
{
	uint32_t i = 0;
	uint32_t error_num = 0;
	uint64_t timer0, timer1;
	uint32_t total_time = 0;
	uint32_t test_len = 1024 * 1024 * 8; // 8Mbyte
	uint32_t value = 0;
	uint32_t base_addr = 0x60000000;

	if (psram_debug->cacheable)
	{
		base_addr = 0x64000000;
	}

	for (i = 0; i < 1024; i++)
	{
		psram_debug->data[i] = 0x11223344 + i;
	}

	CLI_LOGI("begin write %08x, len:8Mbyte\r\n", base_addr);
	timer0 = riscv_get_mtimer();

	for (i = 0; i < test_len; i += 4096)
	{
		bk_psram_memcpy(base_addr + i, &psram_debug->data[0], 4096);
	}

	timer1 = riscv_get_mtimer();

	total_time = (uint32_t) (timer1 - timer0)/26000;
	CLI_LOGI("finish write, use time: %ld ms, write_rate:%ld byte/ms\r\n", total_time, test_len / total_time);

	CLI_LOGI("begin read %08x, len:8Mbyte\r\n", base_addr);
	timer0 = riscv_get_mtimer();
	for (i = 0; i < test_len / 4; i++)
		read_data((base_addr + i * 0x4), value);

	timer1 = riscv_get_mtimer();

	total_time = (uint32_t) (timer1 - timer0)/26000;
	CLI_LOGI("finish read, use time: %ld ms, read_rate:%ld byte/ms\r\n", total_time, test_len / total_time);


	for (i = 0; i < test_len / 4096; i++)
	{
		for (uint32_t k = 0; k < 1024; k++)
		{
			value = get_addr_data(base_addr + i * 4096 + k * 0x4);

			if (value != psram_debug->data[k])
			{
				error_num++;
			}
		}
	}

	CLI_LOGI("finish cpmpare, error_num: %ld, corr_rate: %ld.\r\n", error_num, (test_len - error_num) * 100 / test_len);

	rtos_delay_milliseconds(psram_debug->delay_time);

}


#include "soc/mapping.h"
static void psram_test_unit(void)
{
#if (CONFIG_BK7256_SOC)

	uint32_t i = 0;
	uint32_t error_num = 0;
	uint64_t timer0, timer1;
	uint32_t total_time = 0;
	uint32_t start_address = (uint32_t)&psram_map->reserved;
	uint32_t end_address = (8 * 1024 * 1024 + 0x60000000);
	uint32_t test_len = end_address - start_address;

	uint32_t value = 0;
	CLI_LOGI("begin write %08X-%08X, count: %d\n", start_address, end_address, test_len / 4);
	timer0 = riscv_get_mtimer();
	for (i = 0; i < test_len / 4; i++)
		write_data((start_address + i * 0x4), 0x11111111 + i);

	timer1 = riscv_get_mtimer();
	total_time = (uint32_t) (timer1 - timer0)/26000;
	CLI_LOGI("finish write, use time: %ld ms, write_rate:%ld byte/ms\n", total_time, test_len / total_time);

	CLI_LOGI("begin write %08X-%08X, count: %d\n", start_address, end_address, test_len);
	timer0 = riscv_get_mtimer();
	for (i = 0; i < test_len / 4; i++)
		read_data((start_address + i * 0x4), value);

	timer1 = riscv_get_mtimer();
	total_time = (uint32_t) (timer1 - timer0)/26000;
	CLI_LOGI("finish read, use time: %ld ms, read_rate:%ld byte/ms\n", total_time, test_len / total_time);

	CLI_LOGI("begin write %08X-%08X, count: %d\n", start_address, end_address, test_len);

	for (i = 0; i < test_len / 4; i++)
	{
		value = get_addr_data(start_address + i * 0x4);
		if (value != (0x11111111 + i))
			error_num++;
	}

	CLI_LOGI("finish cpmpare, error_num: %ld, corr_rate: %ld\n", error_num, ((test_len - error_num) * 100 / test_len));

	rtos_delay_milliseconds(psram_debug->delay_time);
#endif
}

static void psram_test_main(void)
{
	while (psram_debug->test_running)
	{
		if (psram_debug->test_mode == 0)
		{
			psram_test_dtcm();
		} else if (psram_debug->test_mode == 1) {
			psram_test_unit();
		} else if (psram_debug->test_mode == 2) {
			psram_write_continue_test();
		}
	}

	CLI_LOGI("psram_test task exit\n");

	if (psram_debug)
	{
		if (psram_debug->data)
		{
			os_free(psram_debug->data);
			psram_debug->data = NULL;
		}

		os_free(psram_debug);
		psram_debug = NULL;
	}

	psram_thread_hdl = NULL;
	rtos_delete_thread(NULL);
}

static bk_err_t psram_task_init(void)
{
	bk_err_t ret = kNoErr;

	ret = bk_psram_init();
	if (ret != kNoErr)
	{
		CLI_LOGE("psram init failed!\r\n");
		return ret;
	}

	if (!psram_thread_hdl)
	{
		ret = rtos_create_thread(&psram_thread_hdl,
								 4,
								 "psram_debug",
								 (beken_thread_function_t)psram_test_main,
								 4 * 1024,
								 (beken_thread_arg_t)NULL);
		if (ret != kNoErr)
		{
			psram_thread_hdl = NULL;
			CLI_LOGE("Error: Failed to create psram test task: %d\r\n", ret);
			return kGeneralErr;
		}

		return kNoErr;
	}
	else
		return kInProgressErr;
}

static void cli_psram_cmd_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret = BK_OK;
	char *msg = NULL;

	if (os_strcmp(argv[1], "start") == 0)
	{

		psram_debug = (psram_debug_t *)os_malloc(sizeof(psram_debug_t));

		if (psram_debug == NULL)
		{
			CLI_LOGE("psram test malloc failed!\r\n");
			msg = CLI_CMD_RSP_ERROR;
			os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
			return;
		}

		os_memset(psram_debug, 0, sizeof(psram_debug_t));

		psram_debug->test_running = 1;

		if (os_strcmp(argv[2], "dtcm") == 0)
		{
			psram_debug->test_mode = 0;
		}
		else if (os_strcmp(argv[2], "conexist") == 0)
		{
			psram_debug->test_mode = 1;
		}
		else if (os_strcmp(argv[2], "continue_write") == 0)
		{
			psram_debug->test_mode = 2;
			if (psram_debug->data == NULL)
			{
				psram_debug->data = (uint32_t *)os_malloc(1024 * 4);
				if (psram_debug->data == NULL)
				{
					CLI_LOGE("malloc error!\r\n");
					os_free(psram_debug);
					psram_debug = NULL;
				}
			}
		}

		if (os_strcmp(argv[3], "1") == 0)
		{
			psram_debug->cacheable = 1;
		}
		else
		{
			psram_debug->cacheable = 0;
		}

		psram_debug->delay_time = 500;

		if (argc >= 5)
		{
			psram_debug->delay_time = os_strtoul(argv[4], NULL, 10);

			if (psram_debug->delay_time == 0)
				psram_debug->delay_time = 500;
		}

		if (psram_task_init() != kNoErr)
		{
			CLI_LOGE("psram test failed!\r\n");
			msg = CLI_CMD_RSP_ERROR;
			os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
			return;
		}

		CLI_LOGI("psram test start success!\r\n");
		msg = CLI_CMD_RSP_SUCCEED;
	}
	else if (os_strcmp(argv[1], "stop") == 0)
	{
		if (psram_thread_hdl)
		{
			psram_debug->test_running = 0;
		}

		while (psram_thread_hdl)
		{
			rtos_delay_milliseconds(10);
		}

		CLI_LOGI("psram test stop success!\r\n");
		msg = CLI_CMD_RSP_SUCCEED;
	}
	else if (os_strcmp(argv[1], "clk") == 0)
	{
		uint16_t clk = os_strtoul(argv[2], NULL, 10);

		switch (clk)
		{
			case 60:
				bk_psram_set_clk(PSRAM_80M);
				break;

			case 120:
				bk_psram_set_clk(PSRAM_120M);
				break;

			case 160:
				bk_psram_set_clk(PSRAM_160M);
				break;

			case 240:
				bk_psram_set_clk(PSRAM_240M);
				break;

			default:
				CLI_LOGE("can not support this clk!\r\n");
				goto error;
				break;
		}

		CLI_LOGI("set psram clk ok!\r\n");
		msg = CLI_CMD_RSP_SUCCEED;

		error:
			msg = CLI_CMD_RSP_ERROR;
	}
	else if (os_strcmp(argv[1], "init") == 0)
	{
		ret = bk_psram_init();
		if (ret != BK_OK)
		{
			CLI_LOGE("psram init failed!\r\n");
			msg = CLI_CMD_RSP_ERROR;
		}
		else
		{
			CLI_LOGI("psram init ok!\r\n");
			msg = CLI_CMD_RSP_SUCCEED;
		}
	}
	else if (os_strcmp(argv[1], "deinit") == 0)
	{
		ret = bk_psram_deinit();
		if (ret != BK_OK)
		{
			CLI_LOGE("psram init failed!\r\n");
			msg = CLI_CMD_RSP_ERROR;
		}
		else
		{
			CLI_LOGI("psram init ok!\r\n");
			msg = CLI_CMD_RSP_SUCCEED;
		}
	}
	else if (os_strcmp(argv[1], "strcat") == 0)
	{
		bk_psram_init();
		uint8_t *data = psram_malloc(20);
		if (data == NULL)
		{
			CLI_LOGI("psram malloc error!\r\n");
			msg = CLI_CMD_RSP_ERROR;
			os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
			return;
		}
		os_memset(data, 0, 20);
		bk_psram_strcat((char *)data, (char *)argv[2]);
		bk_psram_strcat((char *)data, (char *)argv[2]);
		bk_psram_strcat((char *)data, (char *)argv[2]);
		psram_free(data);
		msg = CLI_CMD_RSP_SUCCEED;
	}
	else
	{
		cli_psram_help();
		msg = CLI_CMD_RSP_ERROR;
	}

	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}
#endif

#if (CONFIG_MPC)
#include <driver/mpc.h>

#define BUFFER_SIZE         (34)
#define TEST_VALUE_START    0x41

uint8_t psram_tx_buffer[BUFFER_SIZE] = {0};
uint8_t psram_rx_buffer[BUFFER_SIZE] = {0};

static void fill_buffer(uint8_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
	uint32_t tmpIndex = 0;

	/* Put in global buffer different values */
	for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ ) {
		pBuffer[tmpIndex] = tmpIndex + uwOffset;
	}
}

static void cli_psram_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int i;
	uint32_t test_addr_sec;
	char *msg = NULL;

	fill_buffer(psram_tx_buffer, BUFFER_SIZE, TEST_VALUE_START);

	/*set first block non-sec and second block sec*/
	bk_mpc_driver_init();
	bk_mpc_set_secure_attribute(MPC_DEV_PSRAM, 0, 1, MPC_BLOCK_NON_SECURE);
	bk_mpc_set_secure_attribute(MPC_DEV_PSRAM, bk_mpc_get_block_size(MPC_DEV_PSRAM), 1, MPC_BLOCK_SECURE);

	/*init psram*/
	bk_psram_init();

	test_addr_sec = SOC_PSRAM_DATA_ADDR_SEC + bk_mpc_get_block_size(MPC_DEV_PSRAM);
	bk_psram_memcpy(test_addr_sec, (uint32_t *)psram_tx_buffer, BUFFER_SIZE);
	bk_psram_memread(test_addr_sec, (uint32_t *)psram_rx_buffer, BUFFER_SIZE);

	for (i = 0; i < BUFFER_SIZE; i++) {
		bk_printf("%02x ", psram_rx_buffer[i]);
	}
	bk_printf("\r\n");
	msg = CLI_CMD_RSP_SUCCEED;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}
#endif


static void cli_psram_cmd_handle_ext(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t addr = 0x60000000;
	uint32_t i = 0;
	uint32_t length = 512;
	char *msg = NULL;

	if (argc < 2)
	{
		msg = CLI_CMD_RSP_ERROR;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}

	if (os_strcmp(argv[1], "init") == 0)
	{
		/*init psram*/
		bk_psram_init();
		msg = CLI_CMD_RSP_SUCCEED;
	}
	else if (os_strcmp(argv[1], "byte") == 0)
	{
		uint8_t  value = 0;
		for (i = 0; i < length; i++)
		{
			*((volatile uint8_t *)addr + i) = i;
		}

		for (i = 0; i < length; i++)
		{
			value = *((volatile uint8_t *)addr + i);
			if ((i % 32) == 0)
			{
				os_printf("\r\n");
			}
			os_printf("%d ", value);

			if (i > 255)
			{
				if (value != (i - 256))
				{
					break;
				}
			}
			else
			{
				if (value != i)
				{
					break;
				}
			}
		}
		CLI_LOGI("\r\n%d\r\n", i);

		if (i < length)
		{
			msg = CLI_CMD_RSP_ERROR;
		}
		else
		{
			msg = CLI_CMD_RSP_SUCCEED;
		}
	}
	else if (os_strcmp(argv[1], "rewrite") == 0)
	{
		if (argc < 4)
		{
			msg = CLI_CMD_RSP_ERROR;
			os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
			return;
		}

		length = 20;
		if (os_strcmp(argv[2], "1") == 0)
		{
			for (i = 0; i < length; i++)
			{
				*((volatile uint32_t *)(addr + i * 4)) = i + 0x12345600;
			}
		}

		for (i = 0; i < length * 100; i++)
		{
			*((volatile uint32_t *)(addr + 5 * 4)) = i;
		}

		if (os_strcmp(argv[3], "1") == 0)
		{
			uint32_t value = 0;
			for (i = 0; i < length; i ++)
			{
				value = *((volatile uint32_t *)(addr + i * 4));
				if ((i % 10) == 0)
				{
					os_printf("\r\n");
				}
				os_printf("%08x ", value);
			}
		}
		msg = CLI_CMD_RSP_SUCCEED;
	}
	else if (os_strcmp(argv[1], "deinit") == 0)
	{
		/*init psram*/
		bk_psram_deinit();
		msg = CLI_CMD_RSP_SUCCEED;
	}

	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}

#define PSRAM_CNT (sizeof(s_psram_commands) / sizeof(struct cli_command))
static const struct cli_command s_psram_commands[] = {
	{"psram_test_ext", "init|byte|word|rewirte|deinit", cli_psram_cmd_handle_ext},
#if (CONFIG_ARCH_RISCV)
	{"psram_test", "start|stop", cli_psram_cmd_handle},
#endif
#if (CONFIG_MPC)
	{"psram_test", "", cli_psram_test},
#endif
};

int cli_psram_init(void)
{
	return cli_register_commands(s_psram_commands, PSRAM_CNT);
}


