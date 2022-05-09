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
#include <driver/dma.h>
#include "dma_hal.h"


sbcdecodercontext_t sbc_decoder;


static const unsigned char sbc_data[165] = {
	0x9C, 0xFF, 0x4C, 0xC3, 0xFE, 0xEA, 0x97, 0x77, 0x67, 0x00, 0x00, 0x00, 0x07, 0x80, 0x2B, 0x03,
	0xA0, 0x4F, 0xDF, 0x3F, 0x7D, 0xFB, 0xF8, 0x10, 0x8F, 0x82, 0x08, 0xF1, 0xFC, 0x07, 0x9F, 0xBF,
	0x85, 0x4F, 0x11, 0xE2, 0xF1, 0x9D, 0xBA, 0x7A, 0x14, 0x28, 0xD2, 0x52, 0xB6, 0x2C, 0x72, 0x84,
	0x54, 0x23, 0xC7, 0xB3, 0x0F, 0x57, 0x37, 0x7D, 0x4D, 0x0D, 0x5F, 0x06, 0x0F, 0xAD, 0x05, 0xEA,
	0x71, 0x98, 0xEB, 0xCD, 0x99, 0x32, 0xB7, 0x6F, 0x1B, 0xA2, 0x8F, 0xDD, 0xBC, 0x7E, 0x0C, 0x13,
	0x8A, 0xEF, 0x6E, 0x11, 0x05, 0xDB, 0xF8, 0x5F, 0x3E, 0x00, 0x62, 0xFC, 0xE0, 0x50, 0x1F, 0x3E,
	0x7D, 0xFC, 0x04, 0x85, 0x50, 0x0D, 0xFF, 0x01, 0xFB, 0xF7, 0xDF, 0xBF, 0xC8, 0x03, 0x00, 0x1F,
	0xD0, 0x20, 0x3E, 0x7D, 0xFB, 0xFF, 0xFA, 0xCF, 0xF1, 0xFD, 0x01, 0xFB, 0xF7, 0xDF, 0xBF, 0xB7,
	0xA8, 0xFE, 0xDF, 0xEF, 0x9F, 0xBF, 0x7D, 0xFB, 0xF3, 0x7F, 0xAF, 0xFA, 0x00, 0xF9, 0xF4, 0x07,
	0xDF, 0xBF, 0x00, 0x51, 0x00, 0xA0, 0x0F, 0x9F, 0xBF, 0x7D, 0xFB, 0xF4, 0x85, 0x50, 0x0D, 0xFF,
	0x01, 0xFB, 0xF7, 0xDF, 0xBF,
};

static const unsigned char msbc_data[] = {
	0xAD, 0x31, 0x0C, 0x64, 0xEA, 0x87, 0x66, 0x76, 0x7E, 0xD7, 0xED, 0x82, 0xD8, 0x69, 0xA5, 0x9C,
	0x28, 0x86, 0xD3, 0xEC, 0x2E, 0xD6, 0xAD,
};


static void cli_sbc_decoder_help(void)
{
	SBC_LOGI("sbc_decoder_test {start|stop} \r\n");
	SBC_LOGI("msbc_decoder_test {start|stop} \r\n");
}

static void cli_sbc_decoder_isr(void *param)
{
	bk_dma_start(DMA_ID_0);
	bk_aud_start_dac();
}

void cli_sbc_decoder_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;
	aud_dac_config_t dac_config;
	dma_config_t dma_config;
	uint32_t dac_fifo_addr;
	uint8 *fb;
	uint32 fp = 0;

	fb = (uint8_t*)sbc_data;

	if (argc != 2) {
		cli_sbc_decoder_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		SBC_LOGI("sbc_decoder_test start\r\n");
		bk_sbc_decoder_init(&sbc_decoder);
		bk_sbc_decoder_register_sbc_isr(cli_sbc_decoder_isr, NULL);
		bk_sbc_decoder_interrupt_enable(1);

		sbc_decoder.sample_rate = (fb[1] >> 6) & 0x03;
		sbc_decoder.pcm_length = ((((fb[1] >> 4) & 0x03) + 1) << 2) * (((fb[1] & 0x01) + 1) << 2);

		dac_config.dac_enable = AUD_DAC_DISABLE;
		dac_config.samp_rate = sbc_decoder.sample_rate;
		dac_config.dac_hpf2_coef_B2 = 0x3A22;
		dac_config.dac_hpf2_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
		dac_config.dac_hpf1_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
		dac_config.dac_set_gain = 0x20;
		dac_config.dac_clk_invert = AUD_DAC_CLK_INVERT_RISING;

		dac_config.dac_hpf2_coef_B0 = 0x3A22;
		dac_config.dac_hpf2_coef_B1 = 0x8BBF;

		dac_config.dac_hpf2_coef_A1 = 0x751C;
		dac_config.dac_hpf2_coef_A2 = 0xC9E6;

		dac_config.dacr_rd_threshold = 0x4;
		dac_config.dacl_rd_threshold = 0x4;
		dac_config.dacr_int_enable = 0x0;
		dac_config.dacl_int_enable = 0x0;

		dac_config.dac_filt_enable = AUD_DAC_FILT_DISABLE;
		dac_config.dac_fracmod_manual_enable = AUD_DAC_FRACMOD_MANUAL_DISABLE;
		dac_config.dac_fracmode_value = 0x0;

		bk_aud_driver_init();
		bk_aud_dac_init(&dac_config);
		bk_aud_start_dac();

		//init dma driver
		bk_dma_driver_init();

		dma_config.mode = DMA_WORK_MODE_REPEAT;
		dma_config.chan_prio = 1;
		dma_config.src.dev = DMA_DEV_DTCM;
		dma_config.src.width = DMA_DATA_WIDTH_32BITS;
		dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
		dma_config.src.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
		dma_config.dst.dev = DMA_DEV_AUDIO;
		dma_config.dst.width = DMA_DATA_WIDTH_32BITS;

		//get dac fifo address
		if (bk_aud_get_dac_fifo_addr(&dac_fifo_addr) != BK_OK) {
			SBC_LOGE("get dac fifo address failed\r\n");
			return;
		} else {
			dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
			dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
			dma_config.dst.start_addr = dac_fifo_addr;
			dma_config.dst.end_addr = dac_fifo_addr + 4;
		}

		dma_config.src.start_addr =(uint32_t)&sbc_decoder.pcm_sample;
		dma_config.src.end_addr = (uint32_t)&sbc_decoder.pcm_sample + sbc_decoder.pcm_length;

		//init dma channel
		ret = bk_dma_init(DMA_ID_0, &dma_config);
		if (ret != BK_OK) {
			SBC_LOGE("dma init failed\r\n");
			return;
		}
		bk_dma_set_transfer_len(DMA_ID_0, sbc_decoder.pcm_length * 4);

		while(fp < sizeof(sbc_data))
		{
			bk_dma_start(DMA_ID_0);
			int32_t res = bk_sbc_decoder_frame_decode(&sbc_decoder, fb, 512);
			if (res >= 0) {
				fb += res;
				fp += res;
			}
		}
	}else if (os_strcmp(argv[1], "stop") == 0) {
		SBC_LOGI("sbc decoder test stop!\r\n");
		bk_sbc_decoder_deinit();
		bk_aud_stop_dac();
		bk_aud_driver_deinit();
		bk_dma_stop(DMA_ID_0);
		bk_dma_deinit(DMA_ID_0);
	} else {
		cli_sbc_decoder_help();
		return;
	}
}

void cli_msbc_decoder_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;
	aud_dac_config_t dac_config;
	dma_config_t dma_config;
	uint32_t dac_fifo_addr;
	uint8 *fb;
	uint32 fp = 0;

	fb = (uint8_t*)msbc_data;

	if (argc != 2) {
		cli_sbc_decoder_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		SBC_LOGI("msbc_decoder_test start\r\n");
		bk_sbc_decoder_init(&sbc_decoder);
		bk_sbc_decoder_register_sbc_isr(cli_sbc_decoder_isr, NULL);
		bk_sbc_decoder_interrupt_enable(1);

		sbc_decoder.sample_rate = (fb[1] >> 6) & 0x03;
		sbc_decoder.pcm_length = 15 * (((fb[1] & 0x01) + 1) << 2);

		dac_config.dac_enable = AUD_DAC_DISABLE;
		dac_config.samp_rate = sbc_decoder.sample_rate;
		dac_config.dac_hpf2_coef_B2 = 0x3A22;
		dac_config.dac_hpf2_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
		dac_config.dac_hpf1_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
		dac_config.dac_set_gain = 0x20;
		dac_config.dac_clk_invert = AUD_DAC_CLK_INVERT_RISING;

		dac_config.dac_hpf2_coef_B0 = 0x3A22;
		dac_config.dac_hpf2_coef_B1 = 0x8BBF;

		dac_config.dac_hpf2_coef_A1 = 0x751C;
		dac_config.dac_hpf2_coef_A2 = 0xC9E6;

		dac_config.dacr_rd_threshold = 0x4;
		dac_config.dacl_rd_threshold = 0x4;
		dac_config.dacr_int_enable = 0x0;
		dac_config.dacl_int_enable = 0x0;

		dac_config.dac_filt_enable = AUD_DAC_FILT_DISABLE;
		dac_config.dac_fracmod_manual_enable = AUD_DAC_FRACMOD_MANUAL_DISABLE;
		dac_config.dac_fracmode_value = 0x0;

		bk_aud_driver_init();
		bk_aud_dac_init(&dac_config);
		bk_aud_start_dac();

		//init dma driver
		bk_dma_driver_init();

		dma_config.mode = DMA_WORK_MODE_REPEAT;
		dma_config.chan_prio = 1;
		dma_config.src.dev = DMA_DEV_DTCM;
		dma_config.src.width = DMA_DATA_WIDTH_32BITS;
		dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
		dma_config.src.addr_loop_en = DMA_ADDR_LOOP_ENABLE;

		//get dac fifo address
		if (bk_aud_get_dac_fifo_addr(&dac_fifo_addr) != BK_OK) {
			SBC_LOGE("get dac fifo address failed\r\n");
			return;
		} else {
			dma_config.dst.start_addr = dac_fifo_addr;
			dma_config.dst.end_addr = dac_fifo_addr + 4;
			dma_config.dst.dev = DMA_DEV_AUDIO;
			dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
		}

		dma_config.src.start_addr =(uint32_t)&sbc_decoder.pcm_sample;
		dma_config.src.end_addr = (uint32_t)&sbc_decoder.pcm_sample + sbc_decoder.pcm_length;

		//init dma channel
		ret = bk_dma_init(DMA_ID_0, &dma_config);
		if (ret != BK_OK) {
			SBC_LOGE("dma init failed\r\n");
			return;
		}
		bk_dma_set_transfer_len(DMA_ID_0, sbc_decoder.pcm_length * 4);

		while(fp < sizeof(msbc_data))
		{
			bk_dma_start(DMA_ID_0);
			int32_t res = bk_sbc_decoder_frame_decode(&sbc_decoder, fb, 512);
			if (res >= 0) {
				fb += res;
				fp += res;
			}
		}
	}else if (os_strcmp(argv[1], "stop") == 0) {
		SBC_LOGI("msbc decoder test stop!\r\n");
		bk_sbc_decoder_support_msbc(0);
		bk_sbc_decoder_deinit();
		bk_aud_stop_dac();
		bk_aud_driver_deinit();
		bk_dma_stop(DMA_ID_0);
		bk_dma_deinit(DMA_ID_0);
	} else {
		cli_sbc_decoder_help();
		return;
	}
}


