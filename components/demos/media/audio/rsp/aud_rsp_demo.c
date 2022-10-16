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
#include <modules/audio_rsp_types.h>
#include <modules/audio_rsp.h>
#include "ff.h"
#include "diskio.h"
//#include "BK7256_RegList.h"


#define FARME_LEN 1024*2

static uint32_t in_len = 0;
static uint32_t out_len = 0;

static void cli_aud_rsp_help(void)
{
	os_printf("cli_aud_rsp_test_cmd {8000|16000|32000|48000 0|1 16 xxx.pcm 8000|16000|32000|48000 0|1 16 xxx.pcm 0~10} \r\n");
}

void cli_aud_rsp_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char in_file_name[50];
	char out_file_name[50];
	FIL file_in;
	FIL file_out;
	int16_t *in_addr = NULL;
	int16_t *out_addr = NULL;
	FRESULT fr;
	uint32 uiTemp = 0;
	uint32_t rsp_size = 0;
	bool file_empty_flag = false;

	aud_rsp_cfg_t rsp_cfg = {0};
	bk_err_t ret = BK_OK;

	if (argc != 10) {
		cli_aud_rsp_help();
		return;
	}

	rsp_cfg.src_rate = os_strtoul(argv[1], NULL, 10);
	rsp_cfg.src_ch= os_strtoul(argv[2], NULL, 10);
	rsp_cfg.src_bits = os_strtoul(argv[3], NULL, 10);
	sprintf(in_file_name, "1:/%s", argv[4]);
	fr = f_open(&file_in, in_file_name, FA_OPEN_EXISTING | FA_READ);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", in_file_name);
		return;
	}

	rsp_cfg.dest_rate = os_strtoul(argv[5], NULL, 10);
	rsp_cfg.dest_ch= os_strtoul(argv[6], NULL, 10);
	rsp_cfg.dest_bits = os_strtoul(argv[7], NULL, 10);

	sprintf(out_file_name, "1:/%s", argv[8]);
	fr = f_open(&file_out, out_file_name, FA_CREATE_ALWAYS | FA_WRITE);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", out_file_name);
		return;
	}
	rsp_cfg.complexity = os_strtoul(argv[9], NULL, 10);
	rsp_cfg.down_ch_idx = 0;

    /* init audio resample */
	ret = bk_aud_rsp_init(rsp_cfg);
	if (ret != BK_OK) {
		os_printf("init rsp fail \r\n");
		return;
	}

	in_addr = (int16_t *)os_malloc(FARME_LEN);
	out_addr = (int16_t *)os_malloc(FARME_LEN);
	if (in_addr != NULL && out_addr != NULL)
		os_printf("malloc ok \r\n");
	else {
		os_printf("malloc fail \r\n");
		return;
	}
	os_memset(in_addr, 0, FARME_LEN);
	os_memset(out_addr, 0, FARME_LEN);

	rsp_size = f_size(&file_in);
	os_printf("rsp_size = %d \r\n", rsp_size);
	while (!file_empty_flag) {
		fr = f_read(&file_in, in_addr, FARME_LEN, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read in data fail.\r\n");
			break;
		}

		if (uiTemp == 0 || uiTemp < FARME_LEN)
			file_empty_flag = true;

		in_len = uiTemp/2;
		out_len = FARME_LEN;

//		addAON_GPIO_Reg0x8 = 2;
        ret = bk_aud_rsp_process(in_addr, &in_len, out_addr, &out_len);
//		addAON_GPIO_Reg0x8 = 0;
		if (ret != BK_OK) {
			os_printf("bk_aud_rsp_process fail \r\n");
			break;
		}
		//os_printf("err:%d, in_len:%d, out_len:%d \r\n", err, in_len, out_len);

		fr = f_write(&file_out, (void *)out_addr, out_len*2, &uiTemp);
		if (fr != FR_OK) {
			os_printf("write output data %s fail.\r\n", out_file_name);
			break;
		}
	}

	os_printf("break while \r\n");
	fr = f_close(&file_out);
	if (fr != FR_OK) {
		os_printf("close out file %s fail!\r\n", out_file_name);
		return;
	}

	fr = f_close(&file_in);
	if (fr != FR_OK) {
		os_printf("close out file %s fail!\r\n", in_file_name);
		return;
	}

    bk_aud_rsp_deinit();

	os_printf("resample test complete \r\n");

	os_printf("test finish \r\n");
}

