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
#include <os/mem.h>
#include <os/str.h>
#include <stdio.h>
#include <stdlib.h>
//#include <driver/aud.h>
//#include <driver/aud_types.h>
//#include <driver/dma.h>
//#include "aud_hal.h"
#include "sys_driver.h"
#include "aud_driver.h"
//#include "audio_play_types.h"
//#include <modules/audio_ring_buff.h>
#include <components/aud_intf.h>
#include <components/aud_intf_types.h>
#include "ff.h"
#include "diskio.h"


static FIL spk_file;
static FIL mic_file;
static FIL voc_red_file;
static FIL voc_ply_file;
static char mic_file_name[50];
static char spk_file_name[50];
static char voc_record_file_name[50];
static char voc_play_file_name[50];
static aud_intf_drv_setup_t aud_intf_drv_setup;
static aud_intf_mic_setup_t aud_intf_mic_setup;
static aud_intf_spk_setup_t aud_intf_spk_setup;
static aud_intf_voc_setup_t aud_intf_voc_setup;
static aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;
static int32_t *temp_spk_addr = NULL;     //存放从SDcard中取的pcm信号
static bool spk_file_empty = false;

extern void delay(int num);

static void cli_aud_intf_help(void)
{
	os_printf("aud_intf_record_test {start|pause|stop xxx.pcm} \r\n");
	os_printf("aud_intf_play_test {start|pause|stop xxx.pcm} \r\n");
	os_printf("aud_intf_sd_voc_test {start|stop xx.pcm, xx.pcm} \r\n");
}

static int send_mic_data_to_sd(uint8_t *data, unsigned int len)
{
	//bk_err_t ret = BK_OK;
	FRESULT fr;
	uint32 uiTemp = 0;

	//os_printf("[send_mic_data_to_sd] \r\n");

	/* write data to file */
	fr = f_write(&mic_file, (void *)data, len, &uiTemp);
	if (fr != FR_OK) {
		os_printf("write %s fail.\r\n", mic_file_name);
	}

	return uiTemp;
}

void cli_aud_intf_record_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
//	bk_err_t ret = BK_OK;
	FRESULT fr;

	if (argc != 3) {
		cli_aud_intf_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("start record test \r\n");

		/*open file to save pcm data */
		sprintf(mic_file_name, "1:/%s", argv[2]);
		fr = f_open(&mic_file, mic_file_name, FA_CREATE_ALWAYS | FA_WRITE);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", mic_file_name);
			return;
		}

		aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
		aud_intf_drv_setup.task_config.priority = 3;
		aud_intf_drv_setup.aud_intf_rx_spk_data = NULL;
		aud_intf_drv_setup.aud_intf_tx_mic_data = send_mic_data_to_sd;
		bk_aud_intf_drv_init(&aud_intf_drv_setup);
		aud_work_mode = AUD_INTF_WORK_MODE_GENERAL;
		bk_aud_intf_set_mode(aud_work_mode);
		aud_intf_mic_setup.mic_chl = AUD_INTF_MIC_CHL_MIC1;
		aud_intf_mic_setup.samp_rate = AUD_ADC_SAMP_RATE_8K;
		aud_intf_mic_setup.frame_size = 320;
		aud_intf_mic_setup.mic_gain = 0x2d;
		bk_aud_intf_mic_init(&aud_intf_mic_setup);
		bk_aud_intf_mic_start();

		os_printf("start mic record \r\n");
	} else if (os_strcmp(argv[1], "pause") == 0) {
		bk_aud_intf_mic_pause();
		os_printf("pause mic record \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		bk_aud_intf_mic_stop();

		bk_aud_intf_mic_deinit();

		bk_aud_intf_drv_deinit();

		/* close mic file */
		fr = f_close(&mic_file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", mic_file_name);
			return;
		}
		os_printf("stop mic record \r\n");

		os_printf("stop record test \r\n");
	} else {
		cli_aud_intf_help();
		return;
	}

}

static bk_err_t read_spk_data_from_sd(unsigned int size)
{
	bk_err_t ret = BK_OK;
	FRESULT fr;
	uint32 uiTemp = 0;

	if (spk_file_empty)
		return BK_FAIL;

	temp_spk_addr = os_malloc(size);

	/* read data from file */
	fr = f_read(&spk_file, (void *)temp_spk_addr, size, &uiTemp);
	if (fr != FR_OK) {
		os_printf("write %s fail.\r\n", spk_file_name);
	}

	if (uiTemp != size) {
		spk_file_empty = true;
		os_printf("the %s is empty \r\n", spk_file_name);
	}

	/* write a fram speaker data to speaker_ring_buff */
	ret = bk_aud_intf_write_spk_data((uint8_t*)temp_spk_addr, size);
	if (ret != BK_OK) {
		os_printf("write spk data fail \r\n");
		return ret;
	}

	os_free(temp_spk_addr);

	return ret;
}

void cli_aud_intf_play_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	FRESULT fr;

	if (argc != 3) {
		cli_aud_intf_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("start play test \r\n");

		/*open file to read pcm data */
		sprintf(spk_file_name, "1:/%s", argv[2]);
		fr = f_open(&spk_file, spk_file_name, FA_OPEN_EXISTING | FA_READ);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", spk_file_name);
			return;
		}

		aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
		aud_intf_drv_setup.task_config.priority = 3;
		aud_intf_drv_setup.aud_intf_rx_spk_data = read_spk_data_from_sd;
		aud_intf_drv_setup.aud_intf_tx_mic_data = NULL;
		bk_aud_intf_drv_init(&aud_intf_drv_setup);
		//delay(100);
		aud_work_mode = AUD_INTF_WORK_MODE_GENERAL;
		bk_aud_intf_set_mode(aud_work_mode);
		aud_intf_spk_setup.spk_chl = AUD_INTF_SPK_CHL_SPK1;
		aud_intf_spk_setup.samp_rate = AUD_DAC_SAMP_RATE_8K;
		aud_intf_spk_setup.frame_size = 640;
		aud_intf_spk_setup.spk_gain = 0x2d;
		bk_aud_intf_spk_init(&aud_intf_spk_setup);
		bk_aud_intf_spk_start();

		os_printf("start play \r\n");
	} else if (os_strcmp(argv[1], "pause") == 0) {
		bk_aud_intf_spk_pause();
		os_printf("pause play \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		bk_aud_intf_spk_stop();

		bk_aud_intf_spk_deinit();

		bk_aud_intf_drv_deinit();

		/* close spk file */
		fr = f_close(&spk_file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", spk_file_name);
			return;
		}

		os_printf("stop play \r\n");
		os_printf("stop play test \r\n");
	} else {
		cli_aud_intf_help();
		return;
	}

}


static bk_err_t aud_write_sd_data_to_spk(unsigned int size)
{
	bk_err_t ret = BK_OK;
	FRESULT fr;
	uint32 uiTemp = 0;

	//os_printf("enter %s \r\n", __func__);

	if (spk_file_empty)
		return BK_FAIL;

	temp_spk_addr = os_malloc(size);

	/* read data from file */
	fr = f_read(&voc_ply_file, (void *)temp_spk_addr, size, &uiTemp);
	if (fr != FR_OK) {
		os_printf("read %s fail.\r\n", voc_play_file_name);
	}

	if (uiTemp != size) {
		spk_file_empty = true;
		os_printf("the %s is empty \r\n", voc_play_file_name);
		//TODO
	}

	/* write a fram speaker data to speaker_ring_buff */
	ret = bk_aud_intf_write_spk_data((uint8_t*)temp_spk_addr, size);
	if (ret != BK_OK) {
		os_printf("write voc spk data fail \r\n");
		return ret;
	}

	os_free(temp_spk_addr);

	return ret;
}

static int aud_voc_write_mic_to_sd(UINT8 *data, UINT32 len)
{
	FRESULT fr;
	uint32 uiTemp = 0;

	//os_printf("[send_mic_data_to_sd] \r\n");

	/* write data to file */
	fr = f_write(&voc_red_file, (void *)data, len, &uiTemp);
	if (fr != FR_OK) {
		os_printf("write %s fail.\r\n", voc_record_file_name);
	}

	return uiTemp;
}

/* audio voice transfer pcm data by sdcard */
void cli_aud_intf_sd_voc_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	FRESULT fr;
	bk_err_t ret = BK_OK;

	if (argc != 4) {
		cli_aud_intf_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("start voice transfer test \r\n");

		/*open file to save voice mic data */
		sprintf(voc_record_file_name, "1:/%s", argv[2]);
		fr = f_open(&voc_red_file, voc_record_file_name, FA_CREATE_ALWAYS | FA_WRITE);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", voc_record_file_name);
			return;
		}

		/*open file to read and write voice spk data */
		sprintf(voc_play_file_name, "1:/%s", argv[3]);
		fr = f_open(&voc_ply_file, voc_play_file_name, FA_OPEN_EXISTING | FA_READ);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", voc_play_file_name);
			return;
		}

		aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
		aud_intf_drv_setup.task_config.priority = 3;
		aud_intf_drv_setup.aud_intf_rx_spk_data = aud_write_sd_data_to_spk;
		aud_intf_drv_setup.aud_intf_tx_mic_data = aud_voc_write_mic_to_sd;
		bk_aud_intf_drv_init(&aud_intf_drv_setup);
		aud_work_mode = AUD_INTF_WORK_MODE_VOICE;
		bk_aud_intf_set_mode(aud_work_mode);
		aud_intf_voc_setup.aec_enable = true;
		aud_intf_voc_setup.samp_rate = AUD_INTF_VOC_SAMP_RATE_8K;
		//aud_intf_voc_setup.data_type = AUD_INTF_VOC_DATA_TYPE_G711A;
		aud_intf_voc_setup.data_type = AUD_INTF_VOC_DATA_TYPE_PCM;
		bk_aud_intf_voc_init(aud_intf_voc_setup);

		temp_spk_addr = os_malloc(640);
		os_memset(temp_spk_addr, 0, 640);
		rtos_delay_milliseconds(2000);
		os_printf("start write spk data \r\n");
		for (int i = 0; i < 10; i++) {
			/* write a fram speaker data to speaker_ring_buff */
			ret = bk_aud_intf_write_spk_data((uint8_t*)temp_spk_addr, 640);
			if (ret != BK_OK) {
				os_printf("write voc spk data fail \r\n");
				return;
			}
		}
		os_free(temp_spk_addr);

		os_printf("start voice \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		bk_aud_intf_voc_deinit();
		aud_work_mode = AUD_INTF_WORK_MODE_NULL;
		bk_aud_intf_set_mode(aud_work_mode);
		bk_aud_intf_drv_deinit();

		/* close voice record file */
		fr = f_close(&voc_red_file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", voc_record_file_name);
			return;
		}

		/* close voice play file */
		fr = f_close(&voc_ply_file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", voc_play_file_name);
			return;
		}

		os_printf("stop voice \r\n");
		os_printf("stop voice test \r\n");
	} else {
		cli_aud_intf_help();
		return;
	}

}

void cli_aud_intf_set_voc_param_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t value = 0;

	if (argc != 3) {
		cli_aud_intf_help();
		return;
	}

	if (os_strcmp(argv[1], "mic_gain") == 0) {
		value = strtoul(argv[2], NULL, 0);
		bk_aud_intf_set_mic_gain(value);
		os_printf("set voc mic gain:%d \r\n", value);
	} else if (os_strcmp(argv[1], "spk_gain") == 0) {
		value = strtoul(argv[2], NULL, 0);
		bk_aud_intf_set_spk_gain(value);
		os_printf("set voc spk gain:%d \r\n", value);
	} else if (os_strcmp(argv[1], "gain") == 0) {


		os_printf("stop play \r\n");
	} else {
		cli_aud_intf_help();
		return;
	}

}


