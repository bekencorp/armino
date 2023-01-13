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
#include <os/str.h>
#include <os/os.h>
#include "sys_driver.h"
#include "sbc_driver.h"
#include <driver/sbc.h>
#include <driver/sbc_types.h>
#include <driver/aud.h>
#include <driver/aud_types.h>
#include <components/aud_intf.h>
#include <components/aud_intf_types.h>
#include "ff.h"
#include "diskio.h"


sbcdecodercontext_t g_sbc_decoder;
static char sbc_file_name[50];
static FIL sbc_file;
static uint8_t *sbc_data_buffer = NULL;
static bool sbc_file_is_empty = false;
static uint32_t g_frame_length = 0;

static void cli_sbc_decoder_help(void)
{
	SBC_LOGI("sbc_decoder_test {start|stop} {xxx.sbc} \r\n");
}

static bk_err_t read_sbc_data_from_sdcard(unsigned int size)
{
	bk_err_t ret = BK_OK;
	FRESULT fr;
	uint32 uiTemp = 0;

	if (sbc_file_is_empty) {
		fr = f_close(&sbc_file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", sbc_file_name);
			return fr;
		}
		return BK_FAIL;
	}

	/* read data from file */
	fr = f_read(&sbc_file, (void *)sbc_data_buffer, g_frame_length, &uiTemp);
	if (fr != FR_OK) {
		os_printf("write %s fail.\r\n", sbc_file_name);
	} else {
		bk_sbc_decoder_frame_decode(&g_sbc_decoder, sbc_data_buffer, g_frame_length);
	}

	if (uiTemp == 0) {
		sbc_file_is_empty = true;
		os_printf("the %s is empty \r\n", sbc_file_name);
	}

	/* write a frame speaker data to speaker_ring_buff */
	ret = bk_aud_intf_write_spk_data((uint8_t*)g_sbc_decoder.pcm_sample, size);
	if (ret != BK_OK) {
		os_printf("write spk data fail \r\n");
		return ret;
	}

	return ret;
}

void cli_sbc_decoder_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_ERR_AUD_INTF_OK;

	FRESULT fr;
	uint32 uiTemp = 0;
	uint8_t sbc_info[4];
	uint8_t sbc_pcm_length = 0;
	uint8_t sbc_sample_rate_index = 0;
	uint8_t sbc_blocks, sbc_subbands, sbc_bitpool, sbc_channel_mode, sbc_channel_number;

	aud_intf_drv_setup_t aud_intf_drv_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
	aud_intf_spk_setup_t aud_intf_spk_setup = DEFAULT_AUD_INTF_SPK_SETUP_CONFIG();
	aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;

	if (argc != 3) {
		cli_sbc_decoder_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		SBC_LOGI("sbc_decoder_test start\r\n");
		bk_sbc_decoder_init(&g_sbc_decoder);

		os_memset(sbc_file_name, 0, sizeof(sbc_file_name)/sizeof(sbc_file_name[0]));
		sprintf(sbc_file_name, "1:/%s", argv[2]);
		fr = f_open(&sbc_file, sbc_file_name, FA_OPEN_EXISTING | FA_READ);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", sbc_file_name);
			return;
		}

		fr = f_read(&sbc_file, (void *)sbc_info, 4, &uiTemp);
		if (fr == FR_OK) {
			if (sbc_info[0] == SBC_SYNCWORD) {
				sbc_blocks = (((sbc_info[1] >> 4) & 0x03) + 1) << 2;
				sbc_subbands = ((sbc_info[1] & 0x01) + 1) << 2;
				sbc_bitpool = sbc_info[2];
				sbc_channel_mode = (sbc_info[1] >> 2) & 0x03;
				sbc_sample_rate_index = (sbc_info[1] >> 6) & 0x03;
			} else if (sbc_info[0] == MSBC_SYNCWORD) {
				sbc_blocks = 15;
				if (sbc_info[1] || sbc_info[2]) {
					sbc_sample_rate_index = (sbc_info[1] >> 6) & 0x03;
					sbc_channel_mode = (sbc_info[1] >> 2) & 0x03;
					sbc_subbands = ((sbc_info[1] & 0x01) + 1) << 2;
					sbc_bitpool = sbc_info[2];
				} else {
					sbc_sample_rate_index = 0;
					sbc_channel_mode = 0;
					sbc_subbands = 8;
					sbc_bitpool = 26;
				}
			} else {
				SBC_LOGI("Not find syncword, valid sbc file!\r\n");
				return;
			}

			sbc_channel_number = (sbc_channel_mode == SBC_CHANNEL_MODE_MONO) ? 1 : 2;
			sbc_pcm_length = sbc_blocks * sbc_subbands;

			g_frame_length = 4 + ((4 * sbc_subbands * sbc_channel_number) >> 3);
			if (sbc_channel_mode < 2) {
				g_frame_length += ((uint32_t)sbc_blocks * (uint32_t)sbc_channel_number * (uint32_t)sbc_bitpool + 7) >> 3;
			} else {
				g_frame_length += ((sbc_channel_mode == SBC_CHANNEL_MODE_JOINT_STEREO) * sbc_subbands + sbc_blocks * ((uint32_t)sbc_bitpool) + 7) >> 3;
			}
			f_lseek(&sbc_file, 0);
		} else {
			os_printf("read sbc info failed!\r\n");
			return;
		}

		sbc_data_buffer = os_malloc(g_frame_length);
		if (sbc_data_buffer == NULL) {
			os_printf("sbc data buffer malloc failed!\r\n");
			return;
		}

		//aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
		//aud_intf_drv_setup.task_config.priority = 3;
		aud_intf_drv_setup.aud_intf_rx_spk_data = read_sbc_data_from_sdcard;
		//aud_intf_drv_setup.aud_intf_tx_mic_data = NULL;
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

		aud_intf_spk_setup.spk_chl = AUD_INTF_SPK_CHL_DUAL;

		if (sbc_sample_rate_index == 0) {
			aud_intf_spk_setup.samp_rate = AUD_DAC_SAMP_RATE_16K;
		} else if (sbc_sample_rate_index == 1) {
			aud_intf_spk_setup.samp_rate = AUD_DAC_SAMP_RATE_32K;
		} else if (sbc_sample_rate_index == 2){
			aud_intf_spk_setup.samp_rate = AUD_DAC_SAMP_RATE_44_1K;
		} else {
			aud_intf_spk_setup.samp_rate = AUD_DAC_SAMP_RATE_48K;
		}
		aud_intf_spk_setup.frame_size = sbc_pcm_length * 4;
		//aud_intf_spk_setup.spk_gain = 0x2d;
		//aud_intf_spk_setup.work_mode = AUD_DAC_WORK_MODE_DIFFEN;
		//aud_intf_spk_setup.spk_type = AUD_INTF_SPK_TYPE_BOARD;
		ret = bk_aud_intf_spk_init(&aud_intf_spk_setup);
		if (ret != BK_ERR_AUD_INTF_OK) {
			os_printf("bk_aud_intf_spk_init fail, ret:%d \r\n", ret);
		} else {
			os_printf("bk_aud_intf_spk_init complete \r\n");
		}

		ret = bk_aud_intf_spk_start();
		if (ret != BK_ERR_AUD_INTF_OK) {
		    os_printf("bk_aud_intf_spk_start fail, ret:%d \r\n", ret);
		} else {
		    os_printf("bk_aud_intf_spk_start complete \r\n");
		}
	}else if (os_strcmp(argv[1], "stop") == 0) {
		SBC_LOGI("sbc decoder test stop!\r\n");

		ret = bk_aud_intf_spk_stop();
		if (ret != BK_ERR_AUD_INTF_OK) {
			os_printf("bk_aud_intf_spk_stop fail, ret:%d \r\n", ret);
		} else {
			os_printf("bk_aud_intf_spk_stop complete \r\n");
		}
		
		ret = bk_aud_intf_spk_deinit();
		if (ret != BK_ERR_AUD_INTF_OK) {
			os_printf("bk_aud_intf_spk_deinit fail, ret:%d \r\n", ret);
		} else {
			os_printf("bk_aud_intf_spk_deinit complete \r\n");
		}

		ret = bk_aud_intf_drv_deinit();
		if (ret != BK_ERR_AUD_INTF_OK) {
			os_printf("bk_aud_intf_drv_deinit fail, ret:%d \r\n", ret);
		} else {
			os_printf("bk_aud_intf_drv_deinit complete \r\n");
		}

		sbc_file_is_empty = false;

		fr = f_close(&sbc_file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", sbc_file_name);
			return;
		}
	} else {
		cli_sbc_decoder_help();
		return;
	}
}


