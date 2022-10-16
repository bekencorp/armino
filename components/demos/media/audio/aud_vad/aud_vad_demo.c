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
#include <os/str.h>
#include <os/mem.h>
#include <stdio.h>
#include <stdlib.h>
#include <modules/audio_vad.h>
#include <components/aud_intf.h>
#include <components/aud_intf_types.h>
#include "ff.h"
#include "diskio.h"
#include "BK7256_RegList.h"


#define FARME_SIZE 320

static beken_thread_t vad_thread_handle = NULL;
static beken_queue_t vad_int_msg_que = NULL;
static aud_intf_drv_setup_t aud_intf_drv_setup;
static aud_intf_mic_setup_t aud_intf_mic_setup;
static aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;


typedef enum {
	VAD_CTRL_OP_SET_START = 0,
	VAD_CTRL_OP_SET_CONTINUE,
	VAD_CTRL_OP_EXIT,
	VAD_CTRL_OP_MAX,
} vad_ctrl_op_t;

typedef struct {
	vad_ctrl_op_t op;
	int value;
} vad_ctrl_msg_t;

static void cli_aud_vad_help(void)
{
	os_printf("aud_vad_test {8000|16000|32000|48000 xxx.pcm} \r\n");
	os_printf("aud_vad_all_test {start|stop|set_start|set_continue xx} \r\n");
}

bk_err_t vad_send_msg(vad_ctrl_op_t op, int param)
{
	bk_err_t ret;
	vad_ctrl_msg_t msg;

	msg.op = op;
	msg.value = param;
	if (vad_int_msg_que) {
		ret = rtos_push_to_queue(&vad_int_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			os_printf("vad_send_msg fail \r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

static int check_mic_data(uint8_t *data, unsigned int len)
{
	int result;
	result = bk_aud_vad_process((int16_t *)data);
	if (result == 1) {
		os_printf("speech \r\n");
	} else if (result == 0) {
		os_printf("noise/silence \r\n");
	}

	return len;
}

static void vad_test_task_main(beken_thread_arg_t param_data)
{
	bk_err_t ret = BK_OK;
	vad_ctrl_msg_t msg;

	aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
	aud_intf_drv_setup.task_config.priority = 3;
	aud_intf_drv_setup.aud_intf_rx_spk_data = NULL;
	aud_intf_drv_setup.aud_intf_tx_mic_data = check_mic_data;
	ret = bk_aud_intf_drv_init(&aud_intf_drv_setup);
	if (ret != BK_ERR_AUD_INTF_OK) {
		os_printf("bk_aud_intf_drv_init fail, ret:%d \r\n", ret);
	} else {
		os_printf("bk_aud_intf_drv_init complete \r\n");
	}

	aud_work_mode = AUD_INTF_WORK_MODE_GENERAL;
	ret = bk_aud_intf_set_mode(aud_work_mode);
	if (ret != BK_ERR_AUD_INTF_OK) {
		os_printf("bk_aud_intf_set_mode fail, ret:%d \r\n", ret);
	} else {
		os_printf("bk_aud_intf_set_mode complete \r\n");
	}

	aud_intf_mic_setup.mic_chl = AUD_INTF_MIC_CHL_MIC1;
	aud_intf_mic_setup.samp_rate = AUD_ADC_SAMP_RATE_8K;
	aud_intf_mic_setup.frame_size = 320;
	aud_intf_mic_setup.mic_gain = 0x2d;
	ret = bk_aud_intf_mic_init(&aud_intf_mic_setup);
	if (ret != BK_ERR_AUD_INTF_OK) {
		os_printf("bk_aud_intf_mic_init fail, ret:%d \r\n", ret);
	} else {
		os_printf("bk_aud_intf_mic_init complete \r\n");
	}

    /* init audio vad */
	ret = bk_aud_vad_init(FARME_SIZE/2, 8000);
	if (ret != BK_OK) {
		os_printf("init vad fail \r\n");
		return;
	}

	ret = bk_aud_intf_mic_start();
	if (ret != BK_ERR_AUD_INTF_OK) {
		os_printf("bk_aud_intf_mic_start fail, ret:%d \r\n", ret);
	} else {
		os_printf("bk_aud_intf_mic_start complete \r\n");
	}

	while (1) {
		ret = rtos_pop_from_queue(&vad_int_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			switch (msg.op) {
				case VAD_CTRL_OP_SET_START:
					os_printf("goto: VAD_CTRL_OP_SET_START \r\n");
					bk_aud_intf_mic_pause();
					bk_aud_vad_set_start(msg.value);
					bk_aud_intf_mic_start();
					break;

				case VAD_CTRL_OP_SET_CONTINUE:
					os_printf("goto: VAD_CTRL_OP_SET_CONTINUE \r\n");
					bk_aud_intf_mic_pause();
					bk_aud_vad_set_continue(msg.value);
					bk_aud_intf_mic_start();
					break;

				case VAD_CTRL_OP_EXIT:
					os_printf("goto: VAD_CTRL_OP_EXIT \r\n");
					goto vad_exit;
					break;

				default:
					break;
			}
		}
	}

vad_exit:
	ret = bk_aud_intf_mic_deinit();
	if (ret != BK_ERR_AUD_INTF_OK) {
		os_printf("bk_aud_intf_mic_deinit fail, ret:%d \r\n", ret);
	} else {
		os_printf("bk_aud_intf_mic_deinit complete \r\n");
	}

	ret = bk_aud_intf_drv_deinit();
	if (ret != BK_ERR_AUD_INTF_OK) {
		os_printf("bk_aud_intf_drv_deinit fail, ret:%d \r\n", ret);
	} else {
		os_printf("bk_aud_intf_drv_deinit complete \r\n");
	}

	bk_aud_vad_deinit();

		/* delete msg queue */
	ret = rtos_deinit_queue(&vad_int_msg_que);
	if (ret != kNoErr) {
		os_printf("delete message queue fail \r\n");
	}
	vad_int_msg_que = NULL;
	os_printf("delete message queue complete \r\n");

	/* delete task */
	vad_thread_handle = NULL;
	rtos_delete_thread(NULL);
}

void cli_aud_vad_all_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;
	int param = 0;

	if (argc != 2 && argc != 3) {
		cli_aud_vad_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		ret = rtos_init_queue(&vad_int_msg_que,
							  "vad_int_que",
							  sizeof(vad_ctrl_msg_t),
							  60);
		if (ret != kNoErr) {
			os_printf("ceate vad internal message queue fail \r\n");
			return;
		}

		ret = rtos_create_thread(&vad_thread_handle,
							 BEKEN_DEFAULT_WORKER_PRIORITY,
							 "vad_test",
							 (beken_thread_function_t)vad_test_task_main,
							 4096,
							 NULL);
		if (ret != BK_OK)
			os_printf("create cli vad task fail \r\n");

		os_printf("start vad test \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		vad_send_msg(VAD_CTRL_OP_EXIT, 0);
		os_printf("stop vad test\n");
	} else if (os_strcmp(argv[1], "set_start") == 0) {
		param = os_strtoul(argv[2], NULL, 10);
		vad_send_msg(VAD_CTRL_OP_SET_START, param);
		os_printf("set start \r\n");
	} else if (os_strcmp(argv[1], "set_continue") == 0) {
		param = os_strtoul(argv[2], NULL, 10);
		vad_send_msg(VAD_CTRL_OP_SET_CONTINUE, param);
		os_printf("set continue \r\n");
	} else {
		cli_aud_vad_help();
		return;
	}
}

void cli_aud_vad_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char in_file_name[50];
	FIL file_in;
	int16_t *in_addr = NULL;
	FRESULT fr;
	uint32 uiTemp = 0;
	uint32_t rsp_size = 0;

	bk_err_t ret = BK_OK;
	int result = 0;
	int samp_rate = 0;

	if (argc != 3) {
		cli_aud_vad_help();
		return;
	}

	samp_rate = os_strtoul(argv[1], NULL, 10);
	sprintf(in_file_name, "1:/%s", argv[2]);
	fr = f_open(&file_in, in_file_name, FA_OPEN_EXISTING | FA_READ);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", in_file_name);
		return;
	}

    /* init audio vad */
	ret = bk_aud_vad_init(FARME_SIZE/2, samp_rate);
	if (ret != BK_OK) {
		os_printf("init vad fail \r\n");
		return;
	}

	in_addr = (int16_t *)os_malloc(FARME_SIZE);
	if (in_addr != NULL)
		os_printf("malloc ok \r\n");
	else {
		os_printf("malloc fail \r\n");
		return;
	}
	os_memset(in_addr, 0, FARME_SIZE);

	rsp_size = f_size(&file_in);
	os_printf("rsp_size = %d \r\n", rsp_size);
	while (1) {
		fr = f_read(&file_in, in_addr, FARME_SIZE, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read in data fail.\r\n");
			break;
		}

		if (uiTemp == 0 || uiTemp < FARME_SIZE)
			break;
		//os_memset(in_addr, 0, FARME_SIZE);

//		addAON_GPIO_Reg0x8 = 2;
        result = bk_aud_vad_process(in_addr);
//		addAON_GPIO_Reg0x8 = 0;
		if (result == 1) {
			os_printf("speech \r\n");
		} else if (result == 0) {
			os_printf("noise/silence \r\n");
		}
	}

	os_printf("break while \r\n");

	fr = f_close(&file_in);
	if (fr != FR_OK) {
		os_printf("close out file %s fail!\r\n", in_file_name);
		return;
	}

    bk_aud_vad_deinit();

	os_printf("vad test complete \r\n");

	os_printf("test finish \r\n");
}

