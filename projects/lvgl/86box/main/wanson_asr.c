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

#include <os/mem.h>
#include <os/os.h>
#include <os/str.h>
#include <stdio.h>
#include <stdlib.h>
#include "asr.h"
#include <components/aud_intf.h>
#include <components/aud_intf_types.h>
#include <modules/audio_ring_buff.h>
#include <modules/pm.h>
//#include "BK7256_RegList.h"
#include "wanson_asr.h"
#include "ui.h"

#define ASR_BUFF_SIZE 8000  //>960*2

static aud_intf_drv_setup_t aud_intf_drv_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
static aud_intf_mic_setup_t aud_intf_mic_setup = DEFAULT_AUD_INTF_MIC_SETUP_CONFIG();
static aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;
static beken_thread_t  aud_asr_thread_hdl = NULL;
static beken_queue_t aud_asr_msg_que = NULL;

static uint8_t *asr_ring_buff = NULL;
static RingBufferContext asr_rb;
int8_t *asr_buff = NULL;
const static char *text;
static float score;
static int rs;

static char result0[13] = {0xE5,0xB0,0x8F,0xE8,0x9C,0x82,0xE7,0xAE,0xA1,0xE5,0xAE,0xB6,0x00};//小蜂管家
static char result1[13] = {0xE9,0x98,0xBF,0xE5,0xB0,0x94,0xE7,0xB1,0xB3,0xE8,0xAF,0xBA,0x00};//阿尔米诺
static char result2[13] = {0xE4,0xBC,0x9A,0xE5,0xAE,0xA2,0xE6,0xA8,0xA1,0xE5,0xBC,0x8F,0x00};//会客模式
static char result3[13] = {0xE7,0x94,0xA8,0xE9,0xA4,0x90,0xE6,0xA8,0xA1,0xE5,0xBC,0x8F,0x00};//用餐模式
static char resulta[13] = {0xE7,0xA6,0xBB,0xE5,0xBC,0x80,0xE6,0xA8,0xA1,0xE5,0xBC,0x8F,0x00};//离开模式
static char resultc[13] = {0xE5,0x9B,0x9E,0xE5,0xAE,0xB6,0xE6,0xA8,0xA1,0xE5,0xBC,0x8F,0x00};//回家模式

static bk_err_t aud_asr_send_msg(void)
{
	bk_err_t ret;

	if (aud_asr_msg_que) {
		ret = rtos_push_to_queue(&aud_asr_msg_que, NULL, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			LOGE("aud_asr_send_int_msg fail \r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

static int aud_asr_handle(uint8_t *data, unsigned int len)
{
	uint32 uiTemp = 0;
	//os_printf("%s, write data fail, len: %d \n", __func__, len);

	/* write data to file */
	if (ring_buffer_get_free_size(&asr_rb) >= len) {
	uiTemp = ring_buffer_write(&asr_rb, data, len);
		if (uiTemp != len) {
			LOGE("%s, write data fail, uiTemp: %d \n", __func__, uiTemp);
		}
	}
	if (ring_buffer_get_fill_size(&asr_rb) >= 960) {
		uiTemp = ring_buffer_read(&asr_rb, (uint8_t *)asr_buff, 960);
		aud_asr_send_msg();
	}

	return len;
}


static void aud_asr_main(void)
{
	bk_err_t ret = BK_OK;

	while(1) {
		ret = rtos_pop_from_queue(&aud_asr_msg_que, NULL, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
				rs = Wanson_ASR_Recog((short*)asr_buff, 480, &text, &score);
				if (rs == 1) {
					LOGI(" ASR Result: %s\n", text);	 //识别结果打印
					if (os_strcmp(text, result0) == 0) {	//识别出唤醒词 小蜂管家
						LOGI("%s \n", "xiao feng guan jia ");
					} else if (os_strcmp(text, result1) == 0) {    //识别出唤醒词 阿尔米诺
						LOGI("%s \n", "a er mi nuo ");
						ui_tabview_set(1);
					} else if (os_strcmp(text, result2) == 0) {    //识别出 会客模式
						LOGI("%s \n", "hui ke mo shi ");
						lv_event_send(ui_Ui2Panel2, LV_EVENT_CLICKED, NULL);

						if(lv_obj_has_state(ui_Ui2Panel2, LV_STATE_CHECKED)) {
							lv_obj_clear_state(ui_Ui2Panel2, LV_STATE_CHECKED);
						} else {
							lv_obj_add_state(ui_Ui2Panel2, LV_STATE_CHECKED);
						}
						lv_event_send(ui_Ui2Panel2, LV_EVENT_VALUE_CHANGED, NULL);
					} else if (os_strcmp(text, result3) == 0) {  //识别出 用餐模式
						LOGI("%s \n", "yong can mo shi ");
						lv_event_send(ui_Ui2Panel4, LV_EVENT_CLICKED, NULL);

						if(lv_obj_has_state(ui_Ui2Panel4, LV_STATE_CHECKED)) {
							lv_obj_clear_state(ui_Ui2Panel4, LV_STATE_CHECKED);
						} else {
							lv_obj_add_state(ui_Ui2Panel4, LV_STATE_CHECKED);
						}
						lv_event_send(ui_Ui2Panel4, LV_EVENT_VALUE_CHANGED, NULL);
					} else if (os_strcmp(text, resulta) == 0) {  //识别出 离开模式
						LOGI("%s \n", "li kai mo shi ");
						lv_event_send(ui_Ui2Panel3, LV_EVENT_CLICKED, NULL);
					} else if (os_strcmp(text, resultc) == 0) {  //识别出 回家模式
						LOGI("%s \n", "hui jia mo shi ");
						lv_event_send(ui_Ui2Panel1, LV_EVENT_CLICKED, NULL);
					} else {
						//LOGI(" \n");
					}
				}
		}
	}
}

bk_err_t wanson_asr_open(aud_intf_mic_type_t mic_type)
{
	bk_err_t ret = BK_OK;

	/* set cpu frequency to 320MHz */
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_PSRAM, PM_CPU_FRQ_320M);

	asr_ring_buff = os_malloc(ASR_BUFF_SIZE);
	if (asr_ring_buff ==  NULL) {
		LOGE("os_malloc asr_ring_buff fail \n");
		return BK_FAIL;
	}

	ring_buffer_init(&asr_rb, asr_ring_buff, ASR_BUFF_SIZE, DMA_ID_MAX, RB_DMA_TYPE_NULL);

	asr_buff = os_malloc(960);
	if (asr_buff ==  NULL) {
		LOGE("os_malloc asr_buff fail \n");
		return BK_FAIL;
	}

	if (Wanson_ASR_Init() < 0)
	{
		LOGE("Wanson_ASR_Init Failed!\n");
		return BK_FAIL;
	}
	Wanson_ASR_Reset();
	LOGI("Wanson_ASR_Init OK!\n");

	aud_intf_drv_setup.aud_intf_tx_mic_data = aud_asr_handle;
	ret = bk_aud_intf_drv_init(&aud_intf_drv_setup);
	if (ret != BK_ERR_AUD_INTF_OK) {
		LOGE("bk_aud_intf_drv_init fail, ret:%d \r\n", ret);
	} else {
		LOGI("bk_aud_intf_drv_init complete \r\n");
	}

	aud_work_mode = AUD_INTF_WORK_MODE_GENERAL;
	ret = bk_aud_intf_set_mode(aud_work_mode);
	if (ret != BK_ERR_AUD_INTF_OK) {
		LOGE("bk_aud_intf_set_mode fail, ret:%d \r\n", ret);
	} else {
		LOGI("bk_aud_intf_set_mode complete \r\n");
	}

	aud_intf_mic_setup.samp_rate = AUD_ADC_SAMP_RATE_16K;
	aud_intf_mic_setup.mic_type = mic_type;
	ret = bk_aud_intf_mic_init(&aud_intf_mic_setup);
	if (ret != BK_ERR_AUD_INTF_OK) {
		LOGE("bk_aud_intf_mic_init fail, ret:%d \r\n", ret);
	} else {
		LOGI("bk_aud_intf_mic_init complete \r\n");
	}
	LOGI("init mic complete \r\n");

	ret = rtos_init_queue(&aud_asr_msg_que,
						  "aud_tras_int_que",
						  0,
						  80);
	if (ret != kNoErr) {
		LOGE("ceate audio asr message queue fail \r\n");
		return ret;
	}
	LOGI("ceate audio asr message queue complete \r\n");


	/* create task to asr */
	ret = rtos_create_thread(&aud_asr_thread_hdl,
						 3,
						 "audio_intf",
						 (beken_thread_function_t)aud_asr_main,
						 4096,
						 NULL);
	if (ret != kNoErr) {
		LOGE("create audio transfer driver task fail \r\n");
		rtos_deinit_queue(&aud_asr_msg_que);
		aud_asr_msg_que = NULL;
		aud_asr_thread_hdl = NULL;
	}
	LOGI("create audio transfer driver task complete \r\n");


	ret = bk_aud_intf_mic_start();
	if (ret != BK_ERR_AUD_INTF_OK) {
		LOGE("bk_aud_intf_mic_start fail, ret:%d \r\n", ret);
	} else {
		LOGI("bk_aud_intf_mic_start complete \r\n");
	}
	LOGI("start asr \r\n");

	return ret;
}

bk_err_t wanson_asr_close(void)
{
	bk_err_t ret = BK_OK;

	ret = bk_aud_intf_mic_stop();
	if (ret != BK_ERR_AUD_INTF_OK) {
		LOGE("bk_aud_intf_mic_stop fail, ret:%d \r\n", ret);
	} else {
		LOGI("bk_aud_intf_mic_stop complete \r\n");
	}
	LOGI("stop mic \r\n");

	ret = bk_aud_intf_mic_deinit();
	if (ret != BK_ERR_AUD_INTF_OK) {
		LOGE("bk_aud_intf_mic_deinit fail, ret:%d \r\n", ret);
	} else {
		LOGI("bk_aud_intf_mic_deinit complete \r\n");
	}

	ret = bk_aud_intf_drv_deinit();
	if (ret != BK_ERR_AUD_INTF_OK) {
		LOGE("bk_aud_intf_drv_deinit fail, ret:%d \r\n", ret);
	} else {
		LOGI("bk_aud_intf_drv_deinit complete \r\n");
	}

	Wanson_ASR_Release();

	/* free buffer	*/
	ring_buffer_clear(&asr_rb);
	if (asr_ring_buff) {
		os_free(asr_ring_buff);
		asr_ring_buff = NULL;
	}
	if (asr_buff) {
		os_free(asr_buff);
		asr_buff = NULL;
	}
	
	LOGI("stop asr \r\n");

	return ret;
}


