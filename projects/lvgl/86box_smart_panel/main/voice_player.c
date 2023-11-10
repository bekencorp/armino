// Copyright 2023-2024 Beken
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
#include <stdio.h>
#include <stdlib.h>
#include "sys_driver.h"
#include <components/aud_intf.h>
#include <components/aud_intf_types.h>
#include "ff.h"
#include "diskio.h"
#include "voice_player.h"


#define VOICE_PLAY_TAG "voice_play"

#define LOGI(...) BK_LOGI(VOICE_PLAY_TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(VOICE_PLAY_TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(VOICE_PLAY_TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(VOICE_PLAY_TAG, ##__VA_ARGS__)


static FIL spk_file;
static char spk_file_name[50];
static aud_intf_drv_setup_t aud_intf_drv_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
static aud_intf_spk_setup_t aud_intf_spk_setup = DEFAULT_AUD_INTF_SPK_SETUP_CONFIG();
static aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;
static int32_t *temp_spk_addr = NULL;     //存放从SDcard中取的pcm信号
static bool spk_file_empty = false;

static beken_thread_t  voice_play_thread_hdl = NULL;
static beken_queue_t voice_play_msg_que = NULL;

static FATFS *pfs = NULL;

static bk_err_t tf_mount(void)
{
	FRESULT fr;

	if (pfs != NULL)
	{
		os_free(pfs);
	}

	pfs = os_malloc(sizeof(FATFS));
	if(NULL == pfs)
	{
		LOGI("f_mount malloc failed!\r\n");
		return BK_FAIL;
	}

	fr = f_mount(pfs, "1:", 1);
	if (fr != FR_OK)
	{
		LOGE("f_mount failed:%d\r\n", fr);
		return BK_FAIL;
	}
	else
	{
		LOGI("f_mount OK!\r\n");
	}

	return BK_OK;
}


bk_err_t voice_play_send_msg(voice_play_op_t op, void * param)
{
	bk_err_t ret;

	voice_play_msg_t msg;
	msg.op = op;
	msg.param = param;

	if (voice_play_msg_que) {
		ret = rtos_push_to_queue(&voice_play_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			LOGE("aud_asr_send_int_msg fail \r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

static bk_err_t voice_play_open_file(uint8_t *file_name)
{
	FRESULT fr;

	/*open file to read pcm data */
	sprintf(spk_file_name, "1:/%s", file_name);
	fr = f_open(&spk_file, spk_file_name, FA_OPEN_EXISTING | FA_READ);
	if (fr != FR_OK) {
		LOGE("open %s fail.\r\n", spk_file_name);
		return BK_FAIL;
	}

	return BK_OK;
}

static bk_err_t voice_play_close_file(void)
{
	FRESULT fr;

	/* close spk file */
	fr = f_close(&spk_file);
	if (fr != FR_OK) {
		LOGE("close %s fail!\r\n", spk_file_name);
		return BK_FAIL;
	}

	return BK_OK;
}


static bk_err_t vioce_play_start(uint8_t *file_name)
{
	bk_err_t ret = BK_OK;

	ret = voice_play_open_file(file_name);
	if (ret != BK_OK) {
		//return BK_FAIL;
	}

	ret = voice_play_open_file(file_name);
	if (ret != BK_OK) {
		return BK_FAIL;
	}

	aud_intf_spk_setup.frame_size = 640;
	ret = bk_aud_intf_spk_init(&aud_intf_spk_setup);
	if (ret != BK_ERR_AUD_INTF_OK) {
		LOGE("bk_aud_intf_spk_init fail, ret:%d \r\n", ret);
		goto error;
	} else {
		LOGI("bk_aud_intf_spk_init complete \r\n");
	}

	LOGI("init spk complete \r\n");

	ret = bk_aud_intf_spk_start();
	if (ret != BK_ERR_AUD_INTF_OK) {
		LOGE("bk_aud_intf_spk_start fail, ret:%d \r\n", ret);
		goto error;
	} else {
		LOGI("bk_aud_intf_spk_start complete \r\n");
	}

	LOGI("voice start \r\n");

	return ret;
error:
	voice_play_close_file();
	bk_aud_intf_spk_deinit();
	return ret;
}

static bk_err_t voice_play_stop(void)
{
	bk_err_t ret = BK_OK;

	ret = bk_aud_intf_spk_stop();
	if (ret != BK_ERR_AUD_INTF_OK) {
		LOGE("bk_aud_intf_spk_stop fail, ret:%d \r\n", ret);
	} else {
		LOGI("bk_aud_intf_spk_stop complete \r\n");
	}

	ret = bk_aud_intf_spk_deinit();
	if (ret != BK_ERR_AUD_INTF_OK) {
		LOGE("bk_aud_intf_spk_deinit fail, ret:%d \r\n", ret);
	} else {
		LOGI("bk_aud_intf_spk_deinit complete \r\n");
	}

	voice_play_close_file();

	return ret;
}

bk_err_t read_voice_data_from_sd(unsigned int size)
{
	bk_err_t ret = BK_OK;
	FRESULT fr;
	uint32 uiTemp = 0;

	if (spk_file_empty) {
		voice_play_send_msg(VOICE_PLAY_STOP, NULL);
		return BK_FAIL;
	}

	temp_spk_addr = os_malloc(size);

	/* read data from file */
	fr = f_read(&spk_file, (void *)temp_spk_addr, size, &uiTemp);
	if (fr != FR_OK) {
		LOGE("write %s fail.\r\n", spk_file_name);
	}

	if (uiTemp != size) {
		spk_file_empty = true;
		LOGW("the %s is empty \r\n", spk_file_name);
	}

	/* write a fram speaker data to speaker_ring_buff */
	ret = bk_aud_intf_write_spk_data((uint8_t*)temp_spk_addr, size);
	if (ret != BK_OK) {
		LOGE("write spk data fail \r\n");
		return ret;
	}

	os_free(temp_spk_addr);

	return ret;
}


static void voice_play_main(void)
{
	bk_err_t ret = BK_OK;

	/* mount tf card */
	ret = tf_mount();
	if (ret != BK_OK) {
		LOGE("mount tf card fail \r\n");
		goto exit;
	}

	voice_play_send_msg(VOICE_PLAY_START, "armino_home.pcm");

	voice_play_msg_t msg;
	while(1) {
		ret = rtos_pop_from_queue(&voice_play_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			switch (msg.op) {
				case VOICE_PLAY_START:
					vioce_play_start((uint8_t *)msg.param);
					break;

				case VOICE_PLAY_STOP:
					voice_play_stop();
					/* reset flag */
					spk_file_empty = false;
					break;

				case VOICE_PLAY_EXIT:
					goto exit;
					break;

				default:
					break;
			}

		}
	}

exit:

	/* delete msg queue */
	ret = rtos_deinit_queue(&voice_play_msg_que);
	if (ret != kNoErr) {
		LOGE("delete message queue fail \r\n");
	}
	voice_play_msg_que = NULL;
	LOGI("delete message queue complete \r\n");

	/* delete task */
	voice_play_thread_hdl = NULL;
	rtos_delete_thread(NULL);
}

bk_err_t voice_play_init(void)
{
	bk_err_t ret = BK_OK;

	ret = rtos_init_queue(&voice_play_msg_que,
						  "voice_play_int_que",
						  sizeof(voice_play_msg_t),
						  20);
	if (ret != kNoErr) {
		LOGE("ceate voice play message queue fail \r\n");
		return ret;
	}
	LOGI("ceate voice play message queue complete \r\n");


	/* create task to asr */
	ret = rtos_create_thread(&voice_play_thread_hdl,
						 3,
						 "voice_play",
						 (beken_thread_function_t)voice_play_main,
						 2048,
						 NULL);
	if (ret != kNoErr) {
		LOGE("create audio transfer driver task fail \r\n");
		rtos_deinit_queue(&voice_play_msg_que);
		voice_play_msg_que = NULL;
		voice_play_thread_hdl = NULL;
	}
	LOGI("create voice play task complete \r\n");

	return ret;
}

bk_err_t voice_play_deinit(void)
{
	bk_err_t ret = BK_OK;

	voice_play_send_msg(VOICE_PLAY_EXIT, NULL);

	LOGI("deinit voice play \r\n");

	return ret;
}


