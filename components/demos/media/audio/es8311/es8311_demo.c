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
#include "driver/es8311.h"
#include "driver/es8311_types.h"
#include <driver/i2s.h>
#include <driver/i2s_types.h>
#include "ff.h"
#include "diskio.h"

static beken_thread_t  es8311_master_thread_hdl = NULL;
static beken_queue_t es8311_master_msg_que = NULL;
static RingBufferContext *ch1_rx_rb;
static uint8_t *es8311_rx_temp = NULL;
static FIL es8311_file_mic;
static RingBufferContext mic_data_rb;
static uint8_t *mic_data_buffer = NULL;
static i2s_config_t i2s_config = DEFAULT_I2S_CONFIG( );
static es8311_codec_config_t es8311_cfg = DEFAULT_ES8311_SETUP_CONFIG();
static bool es8311_mic_save_flag = true;
static uint8_t *file_temp_buff = NULL;

#define DMIC_FRAME_SIZE   320
#define DMIC_FRAME_NUM    20
#define DMIC_BUFF_HALF_SIZE    3200


#define MONO_MIC_DATA  1


typedef enum {
	ES8311_MIC = 0,
	ES8311_EXIT,
} es8311_op_t;

typedef struct {
	es8311_op_t op;
} es8311_msg_t;

static void cli_es8311_help(void)
{
	os_printf("es8311_codec_test {start|stop 16K xxx.pcm} \r\n");
}


static bk_err_t es8311_send_msg(es8311_op_t op)
{
	bk_err_t ret;
	es8311_msg_t msg;

	msg.op = op;

	if (es8311_master_msg_que) {
		ret = rtos_push_to_queue(&es8311_master_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			os_printf("es8311_send_int_msg fail \r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

static int es8311_ch1_rx_data_handle_cb(uint32_t size)
{
	uint32 uiTemp = 0;

	//os_printf("%s, size: %d \n", __func__, size);
	uiTemp = ring_buffer_read(ch1_rx_rb, es8311_rx_temp, size);

	if (ring_buffer_get_free_size(&mic_data_rb) >= uiTemp) {
		ring_buffer_write(&mic_data_rb, es8311_rx_temp, uiTemp);
	}

	if (ring_buffer_get_fill_size(&mic_data_rb) >= DMIC_BUFF_HALF_SIZE && es8311_mic_save_flag) {
		es8311_send_msg(ES8311_MIC);
		es8311_mic_save_flag = false;
	}

	return size;
}

static void es8311_master_main(void)
{
	bk_err_t ret = BK_OK;
	uint32 uiTemp = 0;
	FRESULT fr;
	uint32_t write_size = 0;
	GLOBAL_INT_DECLARATION();

	file_temp_buff = os_malloc(DMIC_BUFF_HALF_SIZE);

#if MONO_MIC_DATA
	int16_t *mic_mono_data = NULL;
	mic_mono_data = os_malloc(DMIC_BUFF_HALF_SIZE/4);
#endif

	while(1) {
		es8311_msg_t msg;
		ret = rtos_pop_from_queue(&es8311_master_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			switch (msg.op) {
				case ES8311_MIC:
					GLOBAL_INT_DISABLE();
					write_size = ring_buffer_read(&mic_data_rb, file_temp_buff, DMIC_BUFF_HALF_SIZE);
					GLOBAL_INT_RESTORE();
					es8311_mic_save_flag = true;
#if MONO_MIC_DATA
					int16_t *ptr = (int16_t *)file_temp_buff;
					for (uint32_t i = 0; i < write_size/8; i++) {
						mic_mono_data[i] = ptr[i*4 + 1];
					}
					fr = f_write(&es8311_file_mic, (void *)mic_mono_data, write_size/4, &uiTemp);
#else
					fr = f_write(&es8311_file_mic, (void *)file_temp_buff, write_size, &uiTemp);
#endif
					if (fr != FR_OK) {
						os_printf("write es8311 mic data fail.\r\n");
					}
					break;

				case ES8311_EXIT:
					goto es8311_exit;
					break;

				default:
					break;
			}
		}
	}

es8311_exit:
	os_printf("goto es8311_exit \n");
	os_free(file_temp_buff);
	file_temp_buff = NULL;
#if MONO_MIC_DATA
	os_free(mic_mono_data);
	mic_mono_data = NULL;
#endif

	/* delete msg queue */
	ret = rtos_deinit_queue(&es8311_master_msg_que);
	if (ret != kNoErr) {
		os_printf("delete message queue fail \r\n");
	}
	es8311_master_msg_que = NULL;
	os_printf("delete message queue complete \r\n");

	/* delete task */
	es8311_master_thread_hdl = NULL;
	rtos_delete_thread(NULL);
}

void cli_es8311_codec_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;
	char mic_file_name[50];
	FRESULT fr;
	GLOBAL_INT_DECLARATION();

	if (argc < 4) {
		cli_es8311_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		sprintf(mic_file_name, "1:/%s", argv[3]);
		fr = f_open(&es8311_file_mic, mic_file_name, FA_CREATE_ALWAYS | FA_WRITE);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", mic_file_name);
			return;
		}

		es8311_rx_temp = os_malloc(DMIC_FRAME_SIZE);
		os_memset(es8311_rx_temp, 0x00, DMIC_FRAME_SIZE);

		//init i2s driver
		bk_i2s_driver_init();

		//init i2s configure
		i2s_config.role = I2S_ROLE_SLAVE;
		i2s_config.samp_rate = I2S_SAMP_RATE_16000;
		i2s_config.data_length = 32;
		bk_i2s_init(I2S_GPIO_GROUP_1, &i2s_config);
		os_printf("init i2s driver and config successful \r\n");

		ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX, 640, es8311_ch1_rx_data_handle_cb, &ch1_rx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		mic_data_buffer = os_malloc(DMIC_FRAME_SIZE * DMIC_FRAME_NUM);
		if (mic_data_buffer == NULL) {
			os_printf("malloc mic_data_buffer fail \n");
			return;
		}
		ring_buffer_init(&mic_data_rb, mic_data_buffer, DMIC_FRAME_SIZE * DMIC_FRAME_NUM, DMA_ID_MAX, RB_DMA_TYPE_NULL);


		ret = rtos_init_queue(&es8311_master_msg_que,
							  "es8311_int_que",
							  sizeof(es8311_msg_t),
							  80);
		if (ret != kNoErr) {
			os_printf("ceate es8311 message queue fail \r\n");
			return;
		}
		os_printf("ceate es8311 message queue complete \r\n");


		/* create task to asr */
		ret = rtos_create_thread(&es8311_master_thread_hdl,
							 3,
							 "es8311",
							 (beken_thread_function_t)es8311_master_main,
							 4096*2,
							 NULL);
		if (ret != kNoErr) {
			os_printf("create audio transfer driver task fail \r\n");
			rtos_deinit_queue(&es8311_master_msg_que);
			es8311_master_msg_que= NULL;
			es8311_master_thread_hdl = NULL;
		}
		os_printf("create es8311 codec task complete \r\n");

		GLOBAL_INT_DISABLE();
		es8311_codec_init();
		es8311_codec_start(&es8311_cfg);
		es8311_read_all();
		GLOBAL_INT_RESTORE();

		bk_i2s_start();

		os_printf("es8311 test start \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		es8311_send_msg(ES8311_EXIT);

		bk_i2s_stop();
		bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX);
		bk_i2s_deinit();
		bk_i2s_driver_deinit();

		es8311_stop();
		es8311_codec_deinit();

		rtos_delay_milliseconds(100);

		fr = f_close(&es8311_file_mic);
		if (fr != FR_OK) {
			os_printf("close mic file %s fail!\r\n", mic_file_name);
			return;
		}

		os_free(es8311_rx_temp);
		es8311_rx_temp = NULL;

		os_free(mic_data_buffer);
		mic_data_buffer = NULL;

		os_printf("es8311 test stop \r\n");
	} else if (os_strcmp(argv[1], "read") == 0) {
		/* read all es8311 register */
		es8311_read_all();

		os_printf("i2s master test stop successful \r\n");
	} else if (os_strcmp(argv[1], "adc_init") == 0) {
		/* init es8311 codec */
		es8311_codec_init();
		es8311_codec_start(&es8311_cfg);
		es8311_read_all();

		os_printf("es8311 test init \r\n");
	} else {
		cli_es8311_help();
		return;
	}
}

