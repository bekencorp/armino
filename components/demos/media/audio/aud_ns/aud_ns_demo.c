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
#include <modules/audio_ns.h>
#include "ff.h"
#include "diskio.h"
//#include "BK7256_RegList.h"


#define FARME_SIZE 320

static void cli_aud_ns_help(void)
{
	os_printf("aud_ns_test {8000|16000|32000|48000 xxx.pcm xxx.pcm noise_value} \r\n");
}

void cli_aud_ns_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char in_file_name[50];
	char out_file_name[50];
	FIL file_in;
	FIL file_out;
	int16_t *in_addr = NULL;
	FRESULT fr;
	uint32 uiTemp = 0;
	uint32_t ns_size = 0;
	bool file_empty_flag = false;
	int samp_rate = 0;
	uint32_t in_len = 0;
	int nosie_value = -25;
	bk_err_t ret = BK_OK;

	if (argc != 5) {
		cli_aud_ns_help();
		return;
	}

	samp_rate = os_strtoul(argv[1], NULL, 10);
	sprintf(in_file_name, "1:/%s", argv[2]);
	fr = f_open(&file_in, in_file_name, FA_OPEN_EXISTING | FA_READ);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", in_file_name);
		return;
	}
	sprintf(out_file_name, "1:/%s", argv[3]);
	fr = f_open(&file_out, out_file_name, FA_CREATE_ALWAYS | FA_WRITE);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", out_file_name);
		return;
	}
	nosie_value = os_strtoul(argv[4], NULL, 10);

	/* init audio ns */
	ret = bk_aud_ns_init(FARME_SIZE/2, samp_rate);
	if (ret != BK_OK) {
		os_printf("init ns fail \r\n");
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

	os_printf("nosie_value:%d \r\n", nosie_value);
	bk_aud_ns_set_suprs(nosie_value);

	ns_size = f_size(&file_in);
	os_printf("rsp_size = %d \r\n", ns_size);
	while (!file_empty_flag) {
		fr = f_read(&file_in, in_addr, FARME_SIZE, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read in data fail.\r\n");
			break;
		}

		if (uiTemp == 0 || uiTemp < FARME_SIZE)
			file_empty_flag = true;
		in_len = uiTemp;

//		addAON_GPIO_Reg0x8 = 2;
		bk_aud_ns_process(in_addr);
//		addAON_GPIO_Reg0x8 = 0;

		fr = f_write(&file_out, (void *)in_addr, in_len, &uiTemp);
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

    bk_aud_ns_deinit();

	os_printf("ns test complete \r\n");

	os_printf("test finish \r\n");
}



