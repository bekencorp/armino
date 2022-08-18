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


typedef enum {
	PSRAM_START = 0,
	PSRAM_STOP,
}cmd_type_t;

typedef struct {
	uint8_t type;
	uint32_t data;
}psram_msg_t;


extern u64 riscv_get_mtimer(void);

#define ITERATIONS             1024

#define PSRAM_QITEM_COUNT      (30)


#define write_data(addr,val)                 *((volatile uint32_t *)(addr)) = val
#define read_data(addr,val)                  val = *((volatile uint32_t *)(addr))
#define get_addr_data(addr)                  *((volatile uint32_t *)(addr))

beken_thread_t  psram_thread_hdl = NULL;
beken_queue_t psram_msg_que = NULL;

uint32_t *test_data = NULL;

static void cli_psram_help(void)
{
	CLI_LOGI("psram_test {start|stop}\n");
}

bk_err_t psram_send_msg(uint8_t type, uint32_t data)
{
	bk_err_t ret;
	psram_msg_t msg;

	if (psram_msg_que) {
		msg.type = type;
		msg.data = data;

		ret = rtos_push_to_queue(&psram_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			CLI_LOGE("send_msg failed\r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

static void psram_test(void)
{
	uint32_t i = 0;
	uint32_t error_num = 0;
	uint64_t timer0, timer1;
	uint32_t total_time = 0;
	uint32_t test_len = ITERATIONS * 1024 * 2;

	uint32_t value = 0;
	CLI_LOGI("begin write 0x60000000, len:8Mbyte\r\n");
	timer0 = riscv_get_mtimer();
	for (i = 0; i < test_len; i++)
		write_data((0x60000000 + i * 0x4), 0x11111111 + i);

	timer1 = riscv_get_mtimer();
	total_time = (uint32_t) (timer1 - timer0)/26000;
	CLI_LOGI("finish write, use time: %ld ms, write_rate:%ld Kbyte/ms\r\n", total_time, test_len * 4 / total_time);

	CLI_LOGI("begin read 0x60000000, len:8Mbyte\r\n");
	timer0 = riscv_get_mtimer();
	for (i = 0; i < test_len; i++)
		read_data((0x60000000 + i * 0x4), value);

	timer1 = riscv_get_mtimer();
	total_time = (uint32_t) (timer1 - timer0)/26000;
	CLI_LOGI("finish read, use time: %ld ms, read_rate:%ld Kbyte/ms\r\n", total_time, test_len * 4 / total_time);

	CLI_LOGI("begin compare 0x60000000, len:8Mbyte\r\n");

	for (i = 0; i < test_len; i++)
	{
		value = get_addr_data(0x60000000 + i * 0x4);
		if (value != (0x11111111 + i))
			error_num++;
	}

	CLI_LOGI("finish cpmpare, error_num: %ld, corr_rate: %ld.\r\n", error_num, ((test_len - error_num) * 100 / test_len));

	rtos_delay_milliseconds(1000);
	psram_send_msg(PSRAM_START, 0);
}

static void psram_test_main(void)
{
	bk_err_t ret = kNoErr;

	if (test_data == NULL)
	{
		test_data = (uint32_t *)os_malloc(1024 * 4);
		if (test_data == NULL)
			goto exit;
	}

	psram_send_msg(PSRAM_START, 0);

	while (1)
	{
		psram_msg_t msg;
		ret = rtos_pop_from_queue(&psram_msg_que, &msg, BEKEN_WAIT_FOREVER);
	
		if (kNoErr == ret)
		{
			switch(msg.type)
			{
				case PSRAM_START:
					psram_test();
					break;
				case PSRAM_STOP:
					goto exit;
					break;

				default:
					break;
			}

		}
	}

exit:

	if (test_data)
	{
		os_free(test_data);
		test_data = NULL;
	}

	rtos_deinit_queue(&psram_msg_que);
	psram_msg_que = NULL;

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

	if ((!psram_thread_hdl) && (!psram_msg_que))
	{
		ret = rtos_init_queue(&psram_msg_que,
							  "psram_queue",
							  sizeof(psram_msg_t),
							  PSRAM_QITEM_COUNT);
		if (kNoErr != ret)
		{
			CLI_LOGE("ceate queue failed\r\n");
			return kGeneralErr;
		}

		ret = rtos_create_thread(&psram_thread_hdl,
								 4,
								 "video_intf",
								 (beken_thread_function_t)psram_test_main,
								 4 * 1024,
								 (beken_thread_arg_t)NULL);
		if (ret != kNoErr)
		{
			rtos_deinit_queue(&psram_msg_que);
			psram_msg_que = NULL;
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
	bk_err_t ret = kNoErr;

	if (os_strcmp(argv[1], "start") == 0)
	{
		ret = psram_task_init();
		if (ret != kNoErr)
		{
			CLI_LOGE("psram test failed!\r\n");
		}

		CLI_LOGI("psram test start success!\r\n");
	}
	else if (os_strcmp(argv[1], "stop") == 0)
	{
		if (psram_thread_hdl)
		{
			psram_send_msg(PSRAM_STOP, 0);
		}
		else
		{
			CLI_LOGI("psram test have been stopped or not started!\r\n");
			return;
		}

		while (psram_thread_hdl)
		{
			rtos_delay_milliseconds(10);
		}

		CLI_LOGI("psram test stop success!\r\n");
	}
	else
	{
		cli_psram_help();
		return;
	}
}


#define PSRAM_CNT (sizeof(s_psram_commands) / sizeof(struct cli_command))
static const struct cli_command s_psram_commands[] = {
	{"psram_test", "start|stop", cli_psram_cmd_handle},

};

int cli_psram_init(void)
{
	return cli_register_commands(s_psram_commands, PSRAM_CNT);
}


