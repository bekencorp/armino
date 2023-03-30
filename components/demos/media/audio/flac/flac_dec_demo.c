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
#include "ff.h"
#include "diskio.h"
#include <modules/flac_dec.h>
#include <modules/flac_dec_types.h>
#include "bk_misc.h"

#define FLAC_DEMO_TAG "flac_demo"

#define LOGI(...) BK_LOGI(FLAC_DEMO_TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(FLAC_DEMO_TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(FLAC_DEMO_TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(FLAC_DEMO_TAG, ##__VA_ARGS__)


typedef struct {
	FIL *in;
	FIL *out;
} dec_usr_data_t;

static dec_usr_data_t *dec_usr_data;
static flac_dec_data_t *dec_data;
flac_dec_setup_t *dec_setup;
static bool file_empty_flag = false;
static char in_file_name[50];
static char out_file_name[50];

static void cli_aud_flac_help(void)
{
	os_printf("aud_flac_decoder_test {xxx.flac xxx.wav} \r\n");
}

static unsigned int dec_read_cb(unsigned char *buffer, long unsigned int bytes, void *data)
{
	uint32 uiTemp = 0;
	dec_usr_data_t *temp_dec_data = (dec_usr_data_t *)(((flac_dec_data_t *)data)->usr_param);
	FIL *file_in = temp_dec_data->in;

	if(bytes > 0) {
		f_read(file_in, buffer, bytes, &uiTemp);
		//if (uiTemp == 0 || uiTemp<bytes) {
		if (uiTemp == 0) {
			file_empty_flag = true;
			os_printf(" file_empty_flag = true \r\n");
		}
		return uiTemp;
	}
	else
		return 0;
}

static unsigned int dec_write_cb(unsigned char *buffer, long unsigned int bytes, void *data)
{
	uint32 uiTemp = 0;
	dec_usr_data_t *temp_dec_data = (dec_usr_data_t *)(((flac_dec_data_t *)data)->usr_param);
	FIL *file_out = temp_dec_data->out;
	f_write(file_out, buffer, bytes, &uiTemp);

	return uiTemp;
}

static void dec_metadata_cb(void *data)
{
	flac_dec_data_t *dec_data = (flac_dec_data_t *)data;

	LOGI("sample rate    : %u Hz\n", dec_data->meta_info->sample_rate);
	LOGI("channels       : %u\n", dec_data->meta_info->channels);
	LOGI("bits per sample: %u\n", dec_data->meta_info->bps);
	LOGI("total samples  : %ds\n", dec_data->meta_info->total_samples);
}

static void dec_error_cb(void *data)
{
	//(void *)data;
}

void cli_aud_flac_decoder_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	FRESULT fr;
	uint32_t ns_size = 0;
	bk_err_t ret = BK_OK;

	if (argc != 3) {
		cli_aud_flac_help();
		return;
	}

	dec_data = os_malloc(sizeof(flac_dec_data_t));
	if (dec_data == NULL) {
		os_printf("malloc dec_data fail \r\n");
		return;
	}
	dec_usr_data = os_malloc(sizeof(dec_usr_data_t));
	if (dec_usr_data == NULL) {
		os_printf("malloc dec_usr_data fail \r\n");
		return;
	}

	dec_usr_data->in = os_malloc(sizeof(FIL));
	dec_usr_data->out = os_malloc(sizeof(FIL));
	if (dec_usr_data->in == NULL || dec_usr_data->out == NULL) {
		os_printf("malloc file in or out fail \r\n");
		return;
	}

	dec_data->meta_info = os_malloc(sizeof(flac_dec_meta_info_t));
	if (dec_data->meta_info == NULL) {
		os_printf("malloc dec_data->meta_info fail \r\n");
		return;
	}

	dec_setup = os_malloc(sizeof(flac_dec_setup_t));
	if (dec_setup == NULL) {
		os_printf("malloc dec_setup fail \r\n");
		return;
	}

	sprintf(in_file_name, "1:/%s", argv[1]);
	fr = f_open(dec_usr_data->in, in_file_name, FA_OPEN_EXISTING | FA_READ);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", in_file_name);
		return;
	}
	sprintf(out_file_name, "1:/%s", argv[2]);
	fr = f_open(dec_usr_data->out, out_file_name, FA_CREATE_ALWAYS | FA_WRITE);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", out_file_name);
		return;
	}

	dec_data->usr_param = dec_usr_data;
	dec_setup->read_callback = dec_read_cb;
	dec_setup->write_callback = dec_write_cb;
	dec_setup->metadata_callback = dec_metadata_cb;
	dec_setup->error_callback = dec_error_cb;
	dec_setup->param = dec_data;

	ret = bk_aud_flac_dec_init(dec_setup);
	if (ret != BK_OK) {
		os_printf("init flac decoder fail \r\n");
		goto flac_dec_fail;
	}

	ns_size = f_size(dec_usr_data->in);
	os_printf("rsp_size = %d \r\n", ns_size);
	if (ns_size <= 0)
		return;
	while (!file_empty_flag) {
		//addAON_GPIO_Reg0x8 = 2;
		ret = bk_aud_flac_dec_process();
		//addAON_GPIO_Reg0x8 = 0;
		if (ret != BK_OK) {
			os_printf("signal fail \r\n");
			break;
		} else {
			os_printf("decoder ok \r\n");
		}
	}

	os_printf("break while \r\n");
	fr = f_close(dec_usr_data->out);
	if (fr != FR_OK) {
		os_printf("close out file %s fail!\r\n", out_file_name);
		return;
	}

	fr = f_close(dec_usr_data->in);
	if (fr != FR_OK) {
		os_printf("close out file %s fail!\r\n", in_file_name);
		return;
	}

	bk_aud_flac_dec_deinit();
	os_free(dec_usr_data);
	dec_usr_data = NULL;
	os_free(dec_data->meta_info);
	dec_data->meta_info = NULL;
	os_free(dec_data);
	dec_data = NULL;
	os_free(dec_setup);
	dec_setup = NULL;
	file_empty_flag = false;

	os_printf("test finish \r\n");
	return;

flac_dec_fail:
	if (dec_usr_data)
		os_free(dec_usr_data);
	dec_usr_data = NULL;
	if (dec_data->meta_info)
		os_free(dec_data->meta_info);
	dec_data->meta_info = NULL;
	if (dec_data)
		os_free(dec_data);
	dec_data = NULL;
	if (dec_setup)
		os_free(dec_setup);
	dec_setup = NULL;
	os_printf("goto flac_dec_fail, test fail \r\n");
	return;
}

