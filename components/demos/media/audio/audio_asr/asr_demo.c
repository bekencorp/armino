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

#include <os/mem.h>
#include <os/os.h>
#include <os/str.h>
#include <stdio.h>
#include <stdlib.h>
#include "asr.h"
#include "ff.h"
#include "diskio.h"
#include <components/aud_intf.h>
#include <components/aud_intf_types.h>
#include <modules/audio_ring_buff.h>
#include <modules/pm.h>
#include <driver/media_types.h>
#include "media_app.h"
#include "lcd_act.h"
//#include "BK7256_RegList.h"


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

static void cli_audio_asr_help(void)
{
	os_printf("asr_file_test {xxx.pcm} \r\n");
	os_printf("aud_intf_asr_test {start|stop} \r\n");
}

/* mic file format: signal channel, 16K sample rate, 16bit width */
void cli_asr_file_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char mic_file_name[50];
	FIL file_mic;
	FRESULT fr;
	uint32 uiTemp = 0;
	//uint32_t encoder_size = 0;
	uint8_t ucInBuff[960] = {0};
	bool empty_flag = false;

	if (argc != 2) {
		cli_audio_asr_help();
		return;
	}

	sprintf(mic_file_name, "1:/%s", argv[1]);
	fr = f_open(&file_mic, mic_file_name, FA_READ);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", mic_file_name);
		return;
	}

	if (Wanson_ASR_Init() < 0)
	{
		os_printf("Wanson_ASR_Init Failed!\n");
		return;
	}
	Wanson_ASR_Reset();
	os_printf("Wanson_ASR_Init OK!\n");

	while (!empty_flag)
	{
		fr = f_read(&file_mic, (uint8_t *)ucInBuff, 960, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read ref file fail.\r\n");
			break;
		}

		if (uiTemp < 960)
			empty_flag = true;

		if (uiTemp == 960) {
			rs = Wanson_ASR_Recog((short*)ucInBuff, 480, &text, &score);
			if (rs == 1) {
				os_printf(" ASR Result: %s\n", text);    //识别结果打印
				if (os_strcmp(text, result0) == 0) {    //识别出唤醒词 小蜂管家
					os_printf("%s \n", "xiao feng guan jia ");
				} else if (os_strcmp(text, result1) == 0) {    //识别出唤醒词 阿尔米诺
					os_printf("%s \n", "a er mi nuo ");
				} else if (os_strcmp(text, result2) == 0) {    //识别出 会客模式
					os_printf("%s \n", "hui ke mo shi ");
				} else if (os_strcmp(text, result3) == 0) {	 //识别出 用餐模式
					os_printf("%s \n", "yong can mo shi ");
				} else if (os_strcmp(text, resulta) == 0) {  //识别出 离开模式
					os_printf("%s \n", "li kai mo shi ");
				} else if (os_strcmp(text, resultc) == 0) {  //识别出 回家模式
					os_printf("%s \n", "hui jia mo shi ");
				} else {
					//os_printf(" \n");
				}
			}
		}
	}

	empty_flag = false;

	Wanson_ASR_Release();

	fr = f_close(&file_mic);
	if (fr != FR_OK) {
		os_printf("close out file %s fail!\r\n", mic_file_name);
		return;
	}
	os_printf("encoder test complete \r\n");

	os_printf("test finish \r\n");
}

#if 0
static bk_err_t video_display_open(void)
{
	media_app_camera_open(APP_CAMERA_DVP_JPEG, PPI_800X480);
	lcd_open_t lcd_open;
	lcd_open.device_ppi = PPI_480X272;
	lcd_open.device_name = "st7282";
	media_app_lcd_open(&lcd_open);

	return BK_OK;
}

static bk_err_t video_display_close(void)
{
	media_app_save_stop();
	media_app_camera_close(APP_CAMERA_DVP_JPEG);
	media_app_lcd_close();

	return BK_OK;
}
#endif

static bk_err_t aud_asr_send_msg(void)
{
	bk_err_t ret;

	if (aud_asr_msg_que) {
		ret = rtos_push_to_queue(&aud_asr_msg_que, NULL, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			os_printf("aud_asr_send_int_msg fail \r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

static int aud_asr_handle(uint8_t *data, unsigned int len)
{
	uint32 uiTemp = 0;
//	addAON_GPIO_Reg0x2 = 2;
//	addAON_GPIO_Reg0x2 = 0;
	//os_printf("%s, write data fail, len: %d \n", __func__, len);

	/* write data to file */
	if (ring_buffer_get_free_size(&asr_rb) >= len) {
	uiTemp = ring_buffer_write(&asr_rb, data, len);
		if (uiTemp != len) {
			os_printf("%s, write data fail, uiTemp: %d \n", __func__, uiTemp);
		}
	}
	if (ring_buffer_get_fill_size(&asr_rb) >= 960) {
		uiTemp = ring_buffer_read(&asr_rb, (uint8_t *)asr_buff, 960);
		aud_asr_send_msg();
//		addAON_GPIO_Reg0x3 = 2;
//		addAON_GPIO_Reg0x3 = 0;
	}

	return len;
}


static void aud_asr_main(void)
{
	bk_err_t ret = BK_OK;
	//uint32 uiTemp = 0;

	while(1) {
		ret = rtos_pop_from_queue(&aud_asr_msg_que, NULL, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			//uiTemp = ring_buffer_read(&asr_rb, (uint8_t *)asr_buff, 960);
			//if (uiTemp == 960) {
//				addAON_GPIO_Reg0x4 = 2;
				rs = Wanson_ASR_Recog((short*)asr_buff, 480, &text, &score);
//				addAON_GPIO_Reg0x4 = 0;
				if (rs == 1) {
					os_printf(" ASR Result: %s\n", text);	 //识别结果打印
					if (os_strcmp(text, result0) == 0) {	//识别出唤醒词 小蜂管家
						os_printf("%s \n", "xiao feng guan jia ");
					} else if (os_strcmp(text, result1) == 0) {    //识别出唤醒词 阿尔米诺
						os_printf("%s \n", "a er mi nuo ");
					} else if (os_strcmp(text, result2) == 0) {    //识别出 会客模式
						os_printf("%s \n", "hui ke mo shi ");
					} else if (os_strcmp(text, result3) == 0) {  //识别出 用餐模式
						os_printf("%s \n", "yong can mo shi ");
					} else if (os_strcmp(text, resulta) == 0) {  //识别出 离开模式
						os_printf("%s \n", "li kai mo shi ");
//						video_display_close();
					} else if (os_strcmp(text, resultc) == 0) {  //识别出 回家模式
						os_printf("%s \n", "hui jia mo shi ");
//						video_display_open();
					} else {
						//os_printf(" \n");
					}
				}
		}
	}
}

void cli_aud_intf_asr_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;

	if (argc != 2) {
		cli_audio_asr_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("init record test \r\n");

		/* set cpu frequency to 320MHz */
		bk_pm_module_vote_cpu_freq(PM_DEV_ID_PSRAM, PM_CPU_FRQ_320M);

		asr_ring_buff = os_malloc(ASR_BUFF_SIZE);
		if (asr_ring_buff ==  NULL) {
			os_printf("os_malloc asr_ring_buff fail \n");
			return;
		}

		ring_buffer_init(&asr_rb, asr_ring_buff, ASR_BUFF_SIZE, DMA_ID_MAX, RB_DMA_TYPE_NULL);

		asr_buff = os_malloc(960);
		if (asr_buff ==  NULL) {
			os_printf("os_malloc asr_buff fail \n");
			return;
		}

//		addAON_GPIO_Reg0x2 = 0;
//		addAON_GPIO_Reg0x3 = 0;
//		addAON_GPIO_Reg0x4 = 0;

		if (Wanson_ASR_Init() < 0)
		{
			os_printf("Wanson_ASR_Init Failed!\n");
			return;
		}
		Wanson_ASR_Reset();
		os_printf("Wanson_ASR_Init OK!\n");

		//aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
		//aud_intf_drv_setup.task_config.priority = 3;
		//aud_intf_drv_setup.aud_intf_rx_spk_data = NULL;
		aud_intf_drv_setup.aud_intf_tx_mic_data = aud_asr_handle;
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

		//aud_intf_mic_setup.mic_chl = AUD_INTF_MIC_CHL_MIC1;
		aud_intf_mic_setup.samp_rate = AUD_ADC_SAMP_RATE_16K;
		//aud_intf_mic_setup.mic_type = AUD_INTF_MIC_TYPE_UAC;
		//aud_intf_mic_setup.frame_size = 320;
		//aud_intf_mic_setup.mic_gain = 0x2d;
		ret = bk_aud_intf_mic_init(&aud_intf_mic_setup);
		if (ret != BK_ERR_AUD_INTF_OK) {
			os_printf("bk_aud_intf_mic_init fail, ret:%d \r\n", ret);
		} else {
			os_printf("bk_aud_intf_mic_init complete \r\n");
		}
		os_printf("init mic complete \r\n");

		ret = rtos_init_queue(&aud_asr_msg_que,
							  "aud_tras_int_que",
							  0,
							  80);
		if (ret != kNoErr) {
			os_printf("ceate audio asr message queue fail \r\n");
			return;
		}
		os_printf("ceate audio asr message queue complete \r\n");


		/* create task to asr */
		ret = rtos_create_thread(&aud_asr_thread_hdl,
							 3,
							 "audio_intf",
							 (beken_thread_function_t)aud_asr_main,
							 4096,
							 NULL);
		if (ret != kNoErr) {
			os_printf("create audio transfer driver task fail \r\n");
			rtos_deinit_queue(&aud_asr_msg_que);
			aud_asr_msg_que = NULL;
			aud_asr_thread_hdl = NULL;
		}
		os_printf("create audio transfer driver task complete \r\n");


		ret = bk_aud_intf_mic_start();
		if (ret != BK_ERR_AUD_INTF_OK) {
			os_printf("bk_aud_intf_mic_start fail, ret:%d \r\n", ret);
		} else {
			os_printf("bk_aud_intf_mic_start complete \r\n");
		}
		os_printf("start asr test \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		ret = bk_aud_intf_mic_stop();
		if (ret != BK_ERR_AUD_INTF_OK) {
			os_printf("bk_aud_intf_mic_stop fail, ret:%d \r\n", ret);
		} else {
			os_printf("bk_aud_intf_mic_stop complete \r\n");
		}
		os_printf("stop mic \r\n");

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

		Wanson_ASR_Release();

		/* free buffer  */
		ring_buffer_clear(&asr_rb);
		if (asr_ring_buff) {
			os_free(asr_ring_buff);
			asr_ring_buff = NULL;
		}
		if (asr_buff) {
			os_free(asr_buff);
			asr_buff = NULL;
		}

		os_printf("stop asr test \r\n");
	} else {
		cli_audio_asr_help();
		return;
	}

}

