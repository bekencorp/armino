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
#include <stdio.h>
#include <stdlib.h>
#include <modules/opus.h>
#include "ff.h"
#include "diskio.h"
#include <modules/audio_ring_buff.h>
//#include "BK7256_RegList.h"

#define FRAME_16K_20MS_SIZE    640
#define PACKET_16k_20MS_SIZE   40
static char pcm_file_name[50] = "";
static char opus_file_name[50] = "";

static void cli_audio_opus_help(void)
{
	os_printf("opus_encoder_test {xxx.pcm xxx.opus} \r\n");
	os_printf("opus_decoder_test {xxx.opus xxx.pcm} \r\n");
}

static beken_thread_t opus_enc_thread_handle = NULL;

void opus_encoder_main(void)
{
	FIL file_pcm;
	FIL file_opus;
	FRESULT fr;
	uint32 uiTemp = 0;
	FSIZE_t pcm_data_size = 0;

	uint8_t *pcm_addr = NULL;
	uint8_t *opus_out = NULL;
	int32_t encoder_len = 0;
	int error;
	OpusEncoder *enc = NULL;


	fr = f_open(&file_pcm, pcm_file_name, FA_READ);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", pcm_file_name);
		return;
	}
	fr = f_open(&file_opus, opus_file_name, FA_CREATE_ALWAYS | FA_WRITE);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", opus_file_name);
		return;
	}

	pcm_addr = (uint8_t *)os_malloc(FRAME_16K_20MS_SIZE);
	if (pcm_addr == NULL) {
		os_printf("malloc pcm_addr fail \r\n");
		return;
	}
	os_memset(pcm_addr, 0, FRAME_16K_20MS_SIZE);

	opus_out = (uint8_t *)os_malloc(4000);
	if (opus_out == NULL) {
		os_printf("malloc opus_out fail \r\n");
		return;
	}
	os_memset(opus_out, 0, 4000);

	enc = opus_encoder_create(16000, 1, OPUS_APPLICATION_AUDIO, &error);
	if (enc == NULL) {
		os_printf("creat fail \r\n");
		return;
	}

	opus_encoder_ctl(enc, OPUS_SET_BITRATE(16000));
	opus_encoder_ctl(enc, OPUS_SET_FORCE_CHANNELS(1));
	opus_encoder_ctl(enc, OPUS_SET_VBR(0));
	opus_encoder_ctl(enc, OPUS_SET_VBR_CONSTRAINT(0));
	opus_encoder_ctl(enc, OPUS_SET_COMPLEXITY(10));
	opus_encoder_ctl(enc, OPUS_SET_MAX_BANDWIDTH(OPUS_BANDWIDTH_MEDIUMBAND));
	opus_encoder_ctl(enc, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
	opus_encoder_ctl(enc, OPUS_SET_INBAND_FEC(0));
	opus_encoder_ctl(enc, OPUS_SET_PACKET_LOSS_PERC(0));
	opus_encoder_ctl(enc, OPUS_SET_LSB_DEPTH(16));
	opus_encoder_ctl(enc, OPUS_SET_PREDICTION_DISABLED(1));
	opus_encoder_ctl(enc, OPUS_SET_DTX(0));
	opus_encoder_ctl(enc, OPUS_SET_EXPERT_FRAME_DURATION(OPUS_FRAMESIZE_20_MS));

/*
	if(opus_encoder_ctl(enc, OPUS_SET_BITRATE(bitrate)) != OPUS_OK) test_failed();
	if(opus_encoder_ctl(enc, OPUS_SET_FORCE_CHANNELS(force_channel)) != OPUS_OK) test_failed();
	if(opus_encoder_ctl(enc, OPUS_SET_VBR(vbr)) != OPUS_OK) test_failed();
	if(opus_encoder_ctl(enc, OPUS_SET_VBR_CONSTRAINT(vbr_constraint)) != OPUS_OK) test_failed();
	if(opus_encoder_ctl(enc, OPUS_SET_COMPLEXITY(complexity)) != OPUS_OK) test_failed();
	if(opus_encoder_ctl(enc, OPUS_SET_MAX_BANDWIDTH(max_bw)) != OPUS_OK) test_failed();
	if(opus_encoder_ctl(enc, OPUS_SET_SIGNAL(sig)) != OPUS_OK) test_failed();
	if(opus_encoder_ctl(enc, OPUS_SET_INBAND_FEC(inband_fec)) != OPUS_OK) test_failed();
	if(opus_encoder_ctl(enc, OPUS_SET_PACKET_LOSS_PERC(pkt_loss)) != OPUS_OK) test_failed();
	if(opus_encoder_ctl(enc, OPUS_SET_LSB_DEPTH(lsb_depth)) != OPUS_OK) test_failed();
	if(opus_encoder_ctl(enc, OPUS_SET_PREDICTION_DISABLED(pred_disabled)) != OPUS_OK) test_failed();
	if(opus_encoder_ctl(enc, OPUS_SET_DTX(dtx)) != OPUS_OK) test_failed();
	if(opus_encoder_ctl(enc, OPUS_SET_EXPERT_FRAME_DURATION(frame_size_enum)) != OPUS_OK) test_failed();
*/

	pcm_data_size = f_size(&file_pcm);
	os_printf("pcm_data_size: %d \r\n", (uint32_t)pcm_data_size);
	while (pcm_data_size >= FRAME_16K_20MS_SIZE)
	{
		fr = f_read(&file_pcm, pcm_addr, FRAME_16K_20MS_SIZE, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read pcm file fail.\r\n");
			break;
		}

		//addAON_GPIO_Reg0x8 = 2;
		encoder_len = opus_encode(enc, (int16_t *)pcm_addr, 320, opus_out, 4000);
		//addAON_GPIO_Reg0x8 = 0;
		//os_printf("encoder_len: %d \r\n", encoder_len);
		//rtos_delay_milliseconds(5);
		fr = f_write(&file_opus, (void *)opus_out, encoder_len, &uiTemp);
		if (fr != FR_OK) {
			os_printf("write output data %s fail, uiTemp: %d.\r\n", opus_file_name, uiTemp);
			break;
		}

		pcm_data_size -= FRAME_16K_20MS_SIZE;
	}

	opus_encoder_destroy(enc);

	fr = f_close(&file_pcm);
	if (fr != FR_OK) {
		os_printf("close mic file %s fail!\r\n", pcm_file_name);
		return;
	}

	fr = f_close(&file_opus);
	if (fr != FR_OK) {
		os_printf("close ref file %s fail!\r\n", opus_file_name);
		return;
	}

	os_free(pcm_addr);
	os_free(opus_out);
	os_printf("encoder test complete \r\n");

	/* delete task */
	opus_enc_thread_handle = NULL;
	rtos_delete_thread(NULL);
}

void cli_opus_encoder_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;

	if (argc != 3) {
		cli_audio_opus_help();
		return;
	}

	sprintf(pcm_file_name, "1:/%s", argv[1]);
	sprintf(opus_file_name, "1:/%s", argv[2]);

	ret = rtos_create_thread(&opus_enc_thread_handle,
							 BEKEN_DEFAULT_WORKER_PRIORITY,
							 "opus_enc",
							 (beken_thread_function_t)opus_encoder_main,
							 1024*22,
							 0);
	if (ret != kNoErr) {
		os_printf("Error: Failed to create opus encoder thread: %d\r\n",
				  ret);
	}

	os_printf("test finish \r\n");
}

static beken_thread_t opus_dec_thread_handle = NULL;

void opus_decoder_main(void)
{
	FIL file_pcm;
	FIL file_opus;
	FRESULT fr;
	uint32 uiTemp = 0;
	FSIZE_t opus_data_size = 0;

	uint8_t *opus_addr = NULL;
	uint8_t *pcm_out = NULL;
	int32_t decoder_len = 0;
	int error;
	OpusDecoder *dec = NULL;

	fr = f_open(&file_pcm, pcm_file_name, FA_CREATE_ALWAYS | FA_WRITE);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", pcm_file_name);
		return;
	}
	fr = f_open(&file_opus, opus_file_name, FA_READ);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", opus_file_name);
		return;
	}

	opus_addr = (uint8_t *)os_malloc(PACKET_16k_20MS_SIZE);
	if (opus_addr == NULL) {
		os_printf("malloc pcm_addr fail \r\n");
		return;
	}
	os_memset(opus_addr, 0, PACKET_16k_20MS_SIZE);

	pcm_out = (uint8_t *)os_malloc(4000);
	if (pcm_out == NULL) {
		os_printf("malloc opus_out fail \r\n");
		return;
	}
	os_memset(pcm_out, 0, 4000);

	dec = opus_decoder_create(16000, 1, &error);
	if (dec == NULL) {
		os_printf("creat fail \r\n");
		return;
	}
/*
	opus_encoder_ctl(enc, OPUS_SET_BITRATE(OPUS_BANDWIDTH_NARROWBAND));
	opus_encoder_ctl(enc, OPUS_SET_COMPLEXITY(2));
	opus_encoder_ctl(enc, OPUS_SET_SIGNAL(OPUS_APPLICATION_VOIP));
	opus_decoder_ctl(enc, OPUS_SET_EXPERT_FRAME_DURATION(OPUS_FRAMESIZE_20_MS));
*/
	opus_data_size = f_size(&file_opus);
	os_printf("opus_data_size: %d \r\n", (uint32_t)opus_data_size);
	while (opus_data_size >= PACKET_16k_20MS_SIZE)
	{
		fr = f_read(&file_opus, opus_addr, PACKET_16k_20MS_SIZE, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read pcm file fail.\r\n");
			break;
		}

		decoder_len = opus_decode(dec, opus_addr, PACKET_16k_20MS_SIZE, (int16_t *)pcm_out, 4000, 0);
		//os_printf("decoder_len: %d \r\n", decoder_len*2);
		//rtos_delay_milliseconds(5);
		if (decoder_len > 0) {
			fr = f_write(&file_pcm, (void *)pcm_out, decoder_len*2, &uiTemp);
			if (fr != FR_OK) {
				os_printf("write output data %s fail, uiTemp: %d.\r\n", pcm_file_name, uiTemp);
				break;
			}
		}

		opus_data_size -= PACKET_16k_20MS_SIZE;
	}

	opus_decoder_destroy(dec);

	fr = f_close(&file_pcm);
	if (fr != FR_OK) {
		os_printf("close mic file %s fail!\r\n", pcm_file_name);
		return;
	}

	fr = f_close(&file_opus);
	if (fr != FR_OK) {
		os_printf("close ref file %s fail!\r\n", opus_file_name);
		return;
	}

	os_free(opus_addr);
	os_free(pcm_out);
	os_printf("decoder test complete \r\n");

	/* delete task */
	opus_dec_thread_handle = NULL;
	rtos_delete_thread(NULL);
}

void cli_opus_decoder_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;

	if (argc != 3) {
		cli_audio_opus_help();
		return;
	}

	sprintf(pcm_file_name, "1:/%s", argv[2]);
	sprintf(opus_file_name, "1:/%s", argv[1]);

	ret = rtos_create_thread(&opus_dec_thread_handle,
							 BEKEN_DEFAULT_WORKER_PRIORITY,
							 "opus_enc",
							 (beken_thread_function_t)opus_decoder_main,
							 1024*8,
							 0);
	if (ret != kNoErr) {
		os_printf("Error: Failed to create opus encoder thread: %d\r\n",
				  ret);
	}

	os_printf("test finish \r\n");
}

