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
#include <driver/aud.h>
#include <driver/aud_types.h>
#include <driver/dma.h>
#include "sys_driver.h"
#include "aud_driver.h"
#include "mp3_demo_types.h"
#include <modules/audio_ring_buff.h>
#include <modules/mp3dec.h>
#include "ff.h"
#include "diskio.h"
#include <components/aud_intf.h>
#include <components/aud_intf_types.h>


#define TU_QITEM_COUNT      (60)
#define PCM_SIZE_MAX		(MAX_NSAMP * MAX_NCHAN * MAX_NGRAN)

static beken_thread_t audio_mp3_play_thread_hdl = NULL;
static beken_queue_t audio_mp3_play_msg_que = NULL;

HMP3Decoder hMP3Decoder;
MP3FrameInfo mp3FrameInfo;
unsigned char *readBuf;
short *pcmBuf;
int bytesLeft = 0;
int g_mp3_decode_status = 0;
int g_mp3_decode_complete_status = 0;
int g_mp3_play_is_running = 0;
short *output;

FIL mp3file;
FIL pcmfile;
uint32 uiTemp = 0;
uint32 br = 0;
static char *g_mp3_name;
static char mp3_file_name[50];
int offset = 0;
unsigned char *g_readptr;

static dma_id_t mp3_dac_dma_id = DMA_ID_MAX;
static int32_t *mp3_play_ring_buff = NULL;
static RingBufferContext mp3_play_rb;
static bool mp3_file_is_empty = false;

static bk_err_t audio_send_msg(audio_mp3_play_msg_t msg)
{
	bk_err_t ret = BK_OK;

	if (audio_mp3_play_msg_que) {
		ret = rtos_push_to_queue(&audio_mp3_play_msg_que, &msg, BEKEN_NO_WAIT);
		if (ret != kNoErr) {
			os_printf("audio_send_msg failed\r\n");
			return kGeneralErr;
		}
		return ret;
	}
	return kNoResourcesErr;
}

static bk_err_t bk_audio_mp3_play_dac_config(void)
{
	bk_err_t ret = BK_OK;
	aud_dac_config_t dac_config;

	dac_config.dac_enable = AUD_DAC_DISABLE;
	dac_config.samp_rate = AUD_DAC_SAMP_RATE_8K;
	dac_config.dac_chl = AUD_DAC_CHL_LR_ENABLE;
	dac_config.work_mode = AUD_DAC_WORK_MODE_DIFFEN;
	dac_config.dac_hpf2_coef_B2 = 0x3A22;
	dac_config.dac_hpf2_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
	dac_config.dac_hpf1_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
	dac_config.dac_set_gain = 0x2d;
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

	/* init audio driver and config dac */
	ret = bk_aud_driver_init();
	if (ret != BK_OK) {
		os_printf("init audio driver fail \r\n");
		goto aud_dac_exit;
	}

	ret = bk_aud_dac_init(&dac_config);
	if (ret != BK_OK) {
		os_printf("init audio dac fail \r\n");
		goto aud_dac_exit;
	}

	return BK_OK;

aud_dac_exit:
	os_printf("audio dac config fail \r\n");
	bk_aud_driver_deinit();
	return BK_FAIL;

}

static void bk_audio_mp3_play_dac_sample_rate_set(void)
{
	switch(mp3FrameInfo.samprate) {
		case 8000:
			bk_aud_set_dac_samp_rate(AUD_DAC_SAMP_RATE_8K);
			break;

		case 11025:
			bk_aud_set_dac_samp_rate(AUD_DAC_SAMP_RATE_11_025K);
			break;

		case 12000:
			bk_aud_set_dac_samp_rate(AUD_DAC_SAMP_RATE_12K);
			break;

		case 16000:
			bk_aud_set_dac_samp_rate(AUD_DAC_SAMP_RATE_16K);
			break;

		case 22050:
			bk_aud_set_dac_samp_rate(AUD_DAC_SAMP_RATE_22_05K);
			break;

		case 24000:
			bk_aud_set_dac_samp_rate(AUD_DAC_SAMP_RATE_24K);
			break;

		case 32000:
			bk_aud_set_dac_samp_rate(AUD_DAC_SAMP_RATE_32K);
			break;

		case 44100:
			bk_aud_set_dac_samp_rate(AUD_DAC_SAMP_RATE_44_1K);
			break;

		case 48000:
			bk_aud_set_dac_samp_rate(AUD_DAC_SAMP_RATE_48K);
			break;

		default:
			break;
	}

}

static void audio_mp3_play_dma_finish_isr(void)
{
	bk_err_t ret = BK_OK;
	audio_mp3_play_msg_t msg;
	
	msg.op = AUDIO_MP3_PLAY_START;
	ret = audio_send_msg(msg);
	if (ret != kNoErr) {
		os_printf("mp3 play send msg: %d fails \r\n", msg.op);
	}

}

static bk_err_t bk_audio_mp3_play_dma_config(dma_id_t dma_id, int32_t *ring_buff_addr, uint32_t ring_buff_size, uint32_t transfer_len)
{
	bk_err_t ret = BK_OK;
	dma_config_t dma_config;
	uint32_t dac_port_addr;

	dma_config.mode = DMA_WORK_MODE_REPEAT;
	dma_config.chan_prio = 1;
	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.dev = DMA_DEV_AUDIO;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;

	/* get dac fifo address */
	if (bk_aud_get_dac_fifo_addr(&dac_port_addr) != BK_OK) {
		os_printf("get dac fifo address failed\r\n");
		return BK_FAIL;
	} else {
		dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
		dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
		dma_config.dst.start_addr = dac_port_addr;
		dma_config.dst.end_addr = dac_port_addr + 4;
	}
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
	//dma_config.src.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
	dma_config.src.start_addr = (uint32_t)ring_buff_addr;
	dma_config.src.end_addr = (uint32_t)(ring_buff_addr) + ring_buff_size;

	/* init dma channel */
	ret = bk_dma_init(dma_id, &dma_config);
	if (ret != BK_OK) {
		os_printf("audio dac dma channel init fail \r\n");
		return BK_FAIL;
	}

	/* set dma transfer length */
	bk_dma_set_transfer_len(dma_id, transfer_len);

	//register isr
	bk_dma_register_isr(dma_id, NULL, (void *)audio_mp3_play_dma_finish_isr);
	bk_dma_enable_finish_interrupt(dma_id);

	return BK_OK;

}

void bk_audio_mp3_play_decode_init(void)
{
	readBuf = os_malloc(MAINBUF_SIZE);
	if (readBuf == NULL) {
		os_printf("readBuf malloc failed!\r\n");
		return;
	}

	pcmBuf = os_malloc(PCM_SIZE_MAX * 2);
	if (pcmBuf == NULL) {
		os_printf("pcmBuf malloc failed!\r\n");
		return;
	}

	hMP3Decoder = MP3InitDecoder();
	if (hMP3Decoder == 0) {
		os_free(readBuf);
		os_free(pcmBuf);
		os_printf("MP3Decoder init failed!\r\n");
		return;
	}

}

static void bk_audio_mp3_play_decode_start(DISK_NUMBER disk_id, char *mp3_name, unsigned char **mp3_readptr)
{
	bk_err_t ret = BK_OK;
	int offset = 0;
	char tag_header[10];
	int tag_size = 0;
	unsigned char *readptr = *mp3_readptr;
	audio_mp3_play_msg_t msg;

	FRESULT fr;

	if (!g_mp3_decode_status) {
		/*open file to read mp3 data */
		os_memset(mp3_file_name, 0, sizeof(mp3_file_name)/sizeof(mp3_file_name[0]));
		sprintf(mp3_file_name, "%d:/%s", disk_id, mp3_name);
		fr = f_open(&mp3file, mp3_file_name, FA_OPEN_EXISTING | FA_READ);
		if (fr != FR_OK) {
			os_printf("open %s failed!\r\n", mp3_file_name);
			return;
		}

		fr = f_read(&mp3file, (void *)tag_header, 10, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read %s failed!\r\n", mp3_file_name);
			return;
		}

		if (os_memcmp(tag_header, "ID3", 3) == 0) {
			tag_size = ((tag_header[6] & 0x7F) << 21) | ((tag_header[7] & 0x7F) << 14) | ((tag_header[8] & 0x7F) << 7) | (tag_header[9] & 0x7F);
//			os_printf("tag_size = %d\r\n", tag_size);
			f_lseek(&mp3file, tag_size + 10);
		} else {
			os_printf("tag_header not found!\r\n");
			f_lseek(&mp3file, 0);
		}

		g_mp3_decode_status = 1;
	}

	/* start mp3 decode */
	if (bytesLeft < MAINBUF_SIZE) {
		os_memmove(readBuf, readptr, bytesLeft);
//		os_printf("mp3file bytesLeft = %d!\r\n", bytesLeft);
		fr = f_read(&mp3file, (void *)(readBuf + bytesLeft), MAINBUF_SIZE - bytesLeft, &br);
		if (fr != FR_OK) {
			os_printf("read %s failed!\r\n", mp3_file_name);
			return;
		}

		if ((br == 0) && (bytesLeft == 0)) {
			os_printf("uiTemp = 0 and bytesLeft = 0!\r\n");
			msg.op = AUDIO_MP3_PLAY_EXIT;
			ret = audio_send_msg(msg);
			if (ret != kNoErr) {
				os_printf("mp3 play send msg: %d fails \r\n", msg.op);
			}
			return;
		}

		bytesLeft = bytesLeft + br;
		readptr = readBuf;
	}

	offset = MP3FindSyncWord(readptr, bytesLeft);
//	os_printf("MP3FindSyncWord offset = %d!\r\n", offset);

	if (offset < 0) {
		os_printf("MP3FindSyncWord not find!\r\n");
		bytesLeft = 0;
		msg.op = AUDIO_MP3_PLAY_EXIT;
		ret = audio_send_msg(msg);
		if (ret != kNoErr) {
			os_printf("mp3 play send msg: %d fails \r\n", msg.op);
		}
	} else {
		readptr += offset;
		bytesLeft -= offset;
		
		ret = MP3Decode(hMP3Decoder, &readptr, &bytesLeft, pcmBuf, 0);
		if (ret != ERR_MP3_NONE) {
			os_printf("MP3Decode failed, err is %d", ret);
			return;
		}

		MP3GetLastFrameInfo(hMP3Decoder, &mp3FrameInfo);
//		os_printf("Bitrate: %d kb/s, Samprate: %d\r\n", (mp3FrameInfo.bitrate) / 1000, mp3FrameInfo.samprate);
//		os_printf("Channel: %d, Version: %d, Layer: %d\r\n", mp3FrameInfo.nChans, mp3FrameInfo.version, mp3FrameInfo.layer);
//		os_printf("OutputSamps: %d\r\n", mp3FrameInfo.outputSamps);
		*mp3_readptr = readptr;
	}

}

static void audio_mp3_play_main(void)
{
	bk_err_t ret = BK_OK;
	unsigned char *readptr;
	uint32_t size = 0;

	ret = bk_audio_mp3_play_dac_config();
	if (ret != BK_OK) {
		os_printf("audio dac init failed!\r\n");
		return;
	}

	ret = bk_dma_driver_init();
	if (ret != BK_OK) {
		os_printf("dma driver init failed!\r\n");
		return;
	}

	/* allocate free DMA channel */
	mp3_dac_dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
	if ((mp3_dac_dma_id < DMA_ID_0) || (mp3_dac_dma_id >= DMA_ID_MAX)) {
		os_printf("mp3 dma malloc failed!\r\n");
		return;
	}
//	os_printf("mp3_dac_dma_id: %d \r\n", mp3_dac_dma_id);

	/* start mp3 init and decode */
	bk_audio_mp3_play_decode_init();
	readptr = readBuf;
	bk_audio_mp3_play_decode_start(DISK_NUMBER_SDIO_SD, g_mp3_name, &readptr);


	mp3_play_ring_buff = os_malloc(PCM_SIZE_MAX * 5);
	if (mp3_play_ring_buff == NULL) {
		os_printf("mp3 play ring buffer malloc failed!\r\n");
		return;
	}

	ret = bk_audio_mp3_play_dma_config(mp3_dac_dma_id, mp3_play_ring_buff, PCM_SIZE_MAX * 5, mp3FrameInfo.outputSamps * 2);
	if (ret != BK_OK) {
		os_printf("audio mp3 play dma config failed!\r\n");
		return;
	}

	ring_buffer_init(&mp3_play_rb, (uint8_t*)mp3_play_ring_buff, PCM_SIZE_MAX * 5, mp3_dac_dma_id, RB_DMA_TYPE_READ);

	bk_audio_mp3_play_dac_sample_rate_set();

	size = ring_buffer_write(&mp3_play_rb, (uint8_t *)pcmBuf, mp3FrameInfo.outputSamps * 2);
//	os_printf("ring buffer write size = %d!\r\n", size);

	bk_audio_mp3_play_decode_start(DISK_NUMBER_SDIO_SD, g_mp3_name, &readptr);
	size = ring_buffer_write(&mp3_play_rb, (uint8_t *)pcmBuf, mp3FrameInfo.outputSamps * 2);
//	os_printf("ring buffer write size = %d!\r\n", size);

	bk_aud_start_dac();
	bk_dma_start(mp3_dac_dma_id);
	bk_aud_start_dac();

	while(1) {
		audio_mp3_play_msg_t msg;
		ret = rtos_pop_from_queue(&audio_mp3_play_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			switch (msg.op) {
				case AUDIO_MP3_PLAY_PREV:
					break;

				case AUDIO_MP3_PLAY_NEXT:
					break;

				case AUDIO_MP3_PLAY_START:
					size = ring_buffer_get_free_size(&mp3_play_rb);
//					os_printf("speaker_rb: free_size=%d \r\n", size);
					if (g_mp3_decode_complete_status == 1) {
						if (size > PCM_SIZE_MAX * 5 - mp3FrameInfo.outputSamps * 2) {
							goto audio_mp3_play_exit;
						} else {
							break;
						}
					}
					
					if (size > PCM_SIZE_MAX) {
						bk_audio_mp3_play_decode_start(DISK_NUMBER_SDIO_SD, g_mp3_name, &readptr);
						size = ring_buffer_write(&mp3_play_rb, (uint8_t *)pcmBuf, mp3FrameInfo.outputSamps * 2);
//						os_printf("ring buffer write size = %d!\r\n", size);
					}
					break;

				case AUDIO_MP3_PLAY_STOP:
					break;

				case AUDIO_MP3_PLAY_EXIT:
					g_mp3_decode_complete_status = 1;
					break;

				default:
					break;
			}
		}
//		rtos_delay_milliseconds(10);
	}

audio_mp3_play_exit:
	bytesLeft = 0;
	f_close(&mp3file);
	g_mp3_play_is_running = 0;
	g_mp3_decode_status = 0;
	g_mp3_decode_complete_status = 0;

	MP3FreeDecoder(hMP3Decoder);
    os_free(readBuf);
    os_free(pcmBuf);

	bk_aud_stop_dac();
	bk_aud_dac_deinit();
	bk_aud_driver_deinit();

	bk_dma_stop(mp3_dac_dma_id);
	bk_dma_deinit(mp3_dac_dma_id);
	bk_dma_free(DMA_DEV_AUDIO, mp3_dac_dma_id);

	/* free audio ring buffer */
	ring_buffer_clear(&mp3_play_rb);
	os_free(mp3_play_ring_buff);

	/* delete msg queue */
	ret = rtos_deinit_queue(&audio_mp3_play_msg_que);
	if (ret != kNoErr) {
		os_printf("delete message queue failed!\r\n");
	}
	audio_mp3_play_msg_que = NULL;
	os_printf("delete message queue completed!\r\n");

	audio_mp3_play_thread_hdl = NULL;
	ret = rtos_delete_thread(NULL);
	if (ret != kNoErr) {
		os_printf("delete task failed!\r\n");
	}

}

bk_err_t bk_audio_mp3_play_init(void)
{
	bk_err_t ret = BK_OK;

	g_mp3_play_is_running = 1;

	ret = rtos_init_queue(&audio_mp3_play_msg_que,
							"audio_mp3_play_queue",
							sizeof(audio_mp3_play_msg_t),
							TU_QITEM_COUNT);
	if (ret != kNoErr) {
		os_printf("create audio mp3 play message queue failed!\r\n");
		return BK_FAIL;
	}

	ret = rtos_create_thread(&audio_mp3_play_thread_hdl,
							 BEKEN_DEFAULT_WORKER_PRIORITY,
							 "audio_mp3_play",
							 (beken_thread_function_t)audio_mp3_play_main,
							 4096,
							 NULL);
	if (ret != kNoErr) {
		os_printf("create audio mp3 play task fail!\r\n");
		rtos_deinit_queue(&audio_mp3_play_msg_que);
		audio_mp3_play_msg_que = NULL;
		audio_mp3_play_thread_hdl = NULL;
	}

	return ret;
}

bk_err_t bk_audio_mp3_play_deinit(void)
{
	bk_err_t ret = BK_OK;

	audio_mp3_play_msg_t msg;

	msg.op = AUDIO_MP3_PLAY_EXIT;
	ret = audio_send_msg(msg);
	if (ret != kNoErr) {
		os_printf("send msg: %d fails \r\n", msg.op);
		return BK_FAIL;
	}

	return ret;
}

void cli_mp3_play_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;

	if (argc != 3) {
		os_printf("aud_cp0_audio_mp3_play_test {start|stop} xx.mp3 \r\n");
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("start audio mp3 play test!\r\n");

		g_mp3_name = argv[2];

		if (!g_mp3_play_is_running) {
			/* init audio mp3 play task */
			ret = bk_audio_mp3_play_init();
			if (ret != BK_OK) {
				os_printf("init audio mp3 play task fail!\r\n");
				return;
			}
		} else {
			os_printf("mp3 play task is running, please input stop command firstly!\r\n");
			return;
		}

	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("stop audio mp3 play test!\r\n");

		ret = bk_audio_mp3_play_deinit();
		if (ret != BK_OK) {
			os_printf("init audio mp3 play task fail!\r\n");
			return;
		}
	}
}

static void bk_audio_mp3_decode(void)
{
	bk_err_t ret = BK_OK;

	FRESULT fr;
	uint32 uiTemp = 0;
	uint32 uiTemp2 = 0;
	int offset = 0;
	unsigned char *readptr = readBuf;

	while(1) {
		if (bytesLeft < MAINBUF_SIZE) {
			os_memmove(readBuf, readptr, bytesLeft);
			os_printf("mp3file bytesLeft = %d!\r\n", bytesLeft);
			fr = f_read(&mp3file, (void *)(readBuf + bytesLeft), MAINBUF_SIZE - bytesLeft, &uiTemp);
			if (fr != FR_OK) {
				os_printf("read %s failed!\r\n", mp3_file_name);
				return;
			}

			if ((uiTemp == 0) && (bytesLeft == 0)) {
				os_printf("uiTemp = 0 and bytesLeft = 0\r\n");
				return;
			}

			bytesLeft = bytesLeft + uiTemp;
			readptr = readBuf;
		}

		offset = MP3FindSyncWord(readptr, bytesLeft);
		os_printf("MP3FindSyncWord offset = %d!\r\n", offset);

		if (offset < 0) {
			os_printf("MP3FindSyncWord not find!\r\n");
			bytesLeft = 0;
		} else {
			readptr += offset;
			bytesLeft -= offset;
			
			ret = MP3Decode(hMP3Decoder, &readptr, &bytesLeft, output, 0);
			if (ret != ERR_MP3_NONE) {
				os_printf("MP3Decode failed, code is %d", ret);
				return;
			} 

			fr = f_write(&pcmfile, (void *)output, mp3FrameInfo.outputSamps * 2, &uiTemp2);
			if (fr != FR_OK) {
				os_printf("write pcm file failed.\r\n");
				return;
			}

			MP3GetLastFrameInfo(hMP3Decoder, &mp3FrameInfo);
			os_printf("Bitrate: %d kb/s, Samprate: %d\r\n", (mp3FrameInfo.bitrate) / 1000, mp3FrameInfo.samprate);
			os_printf("Channel: %d, Version: %d, Layer: %d\r\n", mp3FrameInfo.nChans, mp3FrameInfo.version, mp3FrameInfo.layer);
			os_printf("OutputSamps: %d\r\n", mp3FrameInfo.outputSamps);
		}
	}
}

void cli_mp3_decode_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	FRESULT fr;
	static char pcm_file_name[50];
	uint32 uiTemp = 0;
	char tag_header[10];
	int tag_size = 0;

	readBuf = os_malloc(MAINBUF_SIZE);
	if (readBuf == NULL) {
		os_printf("readBuf malloc failed\r\n");
		return;
	}

	output = os_malloc(PCM_SIZE_MAX * 2);
	if (output == NULL) {
		os_free(readBuf);
		os_printf("outBuf malloc failed");
		return;
    }

	if (argc != 4) {
		os_printf("mp3_decode_test {start|stop} xx.mp3 xx.pcm \r\n");
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("start audio mp3 decode test!\r\n");

		hMP3Decoder = MP3InitDecoder();
		if (hMP3Decoder == 0) {
			os_free(readBuf);
			os_free(output);
			os_printf("hMP3Decoder malloc failed!\r\n");
			return;
		}

		os_printf("MP3InitDecoder init successful!\r\n");

		/*open file to read mp3 data */
		sprintf(mp3_file_name, "%d:/%s", DISK_NUMBER_SDIO_SD, argv[2]);
		fr = f_open(&mp3file, mp3_file_name, FA_OPEN_EXISTING | FA_READ);
		if (fr != FR_OK) {
			MP3FreeDecoder(hMP3Decoder);
			os_free(readBuf);
			os_free(output);
			os_printf("open %s failed!\r\n", mp3_file_name);
			return;
		}
		os_printf("mp3 file open successfully!\r\n");

		sprintf(pcm_file_name, "%d:/%s", DISK_NUMBER_SDIO_SD, argv[3]);
		fr = f_open(&pcmfile, pcm_file_name, FA_CREATE_ALWAYS | FA_WRITE);
		if (fr != FR_OK) {
			os_printf("open %s failed!\r\n", pcm_file_name);
			return;
		}
		os_printf("pcm file open successfully!\r\n");

		fr = f_read(&mp3file, (void *)tag_header, 10, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read %s failed!\r\n", mp3_file_name);
			return;
		}
		os_printf("mp3file read successfully!\r\n");

		if (os_memcmp(tag_header, "ID3", 3) == 0) {
			tag_size = ((tag_header[6] & 0x7F) << 21) | ((tag_header[7] & 0x7F) << 14) | ((tag_header[8] & 0x7F) << 7) | (tag_header[9] & 0x7F);
			os_printf("tag_size = %d\r\n", tag_size);
			f_lseek(&mp3file, tag_size + 10);
			os_printf("tag_header has found!\r\n");
		} else {
			os_printf("tag_header not found!\r\n");
			f_lseek(&mp3file, 0);
		}

		bk_audio_mp3_decode();

		fr = f_close(&pcmfile);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", pcm_file_name);
			return;
		}

		os_printf("start audio mp3 play test successful!\r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		MP3FreeDecoder(hMP3Decoder);
	    os_free(readBuf);
	    os_free(output);

		/* close mp3 file */
		fr = f_close(&mp3file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", mp3_file_name);
			return;
		}

		os_printf("stop audio mp3 play test successful!\r\n");
	} else {
		os_printf("mp3_decode_test {start|stop} xx.mp3 xx.pcm \r\n");
		return;
	}

}

static bk_err_t mp3_decode_handler(unsigned int size)
{
	bk_err_t ret = BK_OK;

	FRESULT fr;
	uint32 uiTemp = 0;

	if (mp3_file_is_empty) {
		os_printf("==========================================================\r\n");
		os_printf("%s playback is over, please input the stop command!\r\n", mp3_file_name);
		os_printf("==========================================================\r\n");
		return BK_FAIL;
	}

	if (bytesLeft < MAINBUF_SIZE) {
		os_memmove(readBuf, g_readptr, bytesLeft);
		fr = f_read(&mp3file, (void *)(readBuf + bytesLeft), MAINBUF_SIZE - bytesLeft, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read %s failed!\r\n", mp3_file_name);
			return fr;
		}

		if ((uiTemp == 0) && (bytesLeft == 0)) {
			os_printf("uiTemp = 0 and bytesLeft = 0\r\n");
			mp3_file_is_empty = true;
			os_printf("the %s is empty \r\n", mp3_file_name);
			return ret;
		}

		bytesLeft = bytesLeft + uiTemp;
		g_readptr = readBuf;
	}

	offset = MP3FindSyncWord(g_readptr, bytesLeft);

	if (offset < 0) {
		os_printf("MP3FindSyncWord not find!\r\n");
		bytesLeft = 0;
	} else {
		g_readptr += offset;
		bytesLeft -= offset;
		
		ret = MP3Decode(hMP3Decoder, &g_readptr, &bytesLeft, pcmBuf, 0);
		if (ret != ERR_MP3_NONE) {
			os_printf("MP3Decode failed, code is %d", ret);
			return ret;
		}

		MP3GetLastFrameInfo(hMP3Decoder, &mp3FrameInfo);
//		os_printf("Bitrate: %d kb/s, Samprate: %d\r\n", (mp3FrameInfo.bitrate) / 1000, mp3FrameInfo.samprate);
//		os_printf("Channel: %d, Version: %d, Layer: %d\r\n", mp3FrameInfo.nChans, mp3FrameInfo.version, mp3FrameInfo.layer);
//		os_printf("OutputSamps: %d\r\n", mp3FrameInfo.outputSamps);

		/* write a frame speaker data to speaker_ring_buff */
		ret = bk_aud_intf_write_spk_data((uint8_t*)pcmBuf, mp3FrameInfo.outputSamps * 2);
		if (ret != BK_OK) {
			os_printf("write spk data fail \r\n");
			return ret;
		}
	}

	return ret;

}

void cli_aud_intf_mp3_play_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_ERR_AUD_INTF_OK;
	FRESULT fr;

	aud_intf_drv_setup_t aud_intf_drv_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
	aud_intf_spk_setup_t aud_intf_spk_setup = DEFAULT_AUD_INTF_SPK_SETUP_CONFIG();
	aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;

	if (argc != 3) {
		os_printf("aud_intf_mp3_play_test {start|stop} xx.mp3\r\n");
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		bk_audio_mp3_play_decode_init();
		os_printf("audio mp3 play decode init completely!\r\n");

		/*open file to read mp3 data */
		os_memset(mp3_file_name, 0, sizeof(mp3_file_name)/sizeof(mp3_file_name[0]));
		sprintf(mp3_file_name, "%d:/%s", DISK_NUMBER_SDIO_SD, argv[2]);
		fr = f_open(&mp3file, mp3_file_name, FA_OPEN_EXISTING | FA_READ);
		if (fr != FR_OK) {
			MP3FreeDecoder(hMP3Decoder);
			os_free(readBuf);
			os_free(output);
			os_printf("open %s failed!\r\n", mp3_file_name);
			return;
		}
		os_printf("mp3 file open successfully!\r\n");	

		//aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
		//aud_intf_drv_setup.task_config.priority = 3;
		aud_intf_drv_setup.aud_intf_rx_spk_data = mp3_decode_handler;
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

		g_readptr = readBuf;
		mp3_decode_handler(mp3FrameInfo.outputSamps * 2);

		aud_intf_spk_setup.spk_chl = AUD_INTF_SPK_CHL_DUAL;

		switch(mp3FrameInfo.samprate) {
			case 8000:
				aud_intf_spk_setup.samp_rate = AUD_DAC_SAMP_RATE_8K;
				break;
	
			case 11025:
				aud_intf_spk_setup.samp_rate = AUD_DAC_SAMP_RATE_11_025K;
				break;
	
			case 12000:
				aud_intf_spk_setup.samp_rate = AUD_DAC_SAMP_RATE_12K;
				break;
	
			case 16000:
				aud_intf_spk_setup.samp_rate = AUD_DAC_SAMP_RATE_16K;
				break;
	
			case 22050:
				aud_intf_spk_setup.samp_rate = AUD_DAC_SAMP_RATE_22_05K;
				break;
	
			case 24000:
				aud_intf_spk_setup.samp_rate = AUD_DAC_SAMP_RATE_24K;
				break;
	
			case 32000:
				aud_intf_spk_setup.samp_rate = AUD_DAC_SAMP_RATE_32K;
				break;
	
			case 44100:
				aud_intf_spk_setup.samp_rate = AUD_DAC_SAMP_RATE_44_1K;
				break;
	
			case 48000:
				aud_intf_spk_setup.samp_rate = AUD_DAC_SAMP_RATE_48K;
				break;
	
			default:
				break;
		}
		
		aud_intf_spk_setup.frame_size = mp3FrameInfo.outputSamps * 2;
		//aud_intf_spk_setup.spk_gain = 0x2d;
		aud_intf_spk_setup.work_mode = AUD_DAC_WORK_MODE_DIFFEN;
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

		bytesLeft = 0;
		mp3_file_is_empty = false;

		fr = f_close(&mp3file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", mp3_file_name);
			return;
		}

		MP3FreeDecoder(hMP3Decoder);
	    os_free(readBuf);
	    os_free(pcmBuf);
	}
}


#if (CONFIG_WEBCLIENT)
#define TAG   "http_mp3_play"

#include "components/webclient.h"

#define GET_HEADER_BUFSZ               1024
#define GET_RESP_BUFSZ                 1024 * 10

static char *uri = NULL;
static uint8_t aud_spk_init_flag = 0;
static uint32_t psram_buffer = 0x60000000;
static unsigned char *psram_ptr = NULL;
static unsigned char *mp3_end_addr = NULL;
static beken_thread_t http_get_thread_hdl = NULL;
static beken_thread_t http_mp3_play_thread_hdl = NULL;
static beken_queue_t http_mp3_play_msg_que = NULL;
static aud_intf_spk_setup_t aud_intf_spk_setup = DEFAULT_AUD_INTF_SPK_SETUP_CONFIG();

static const uint16_t MP3_SAMPLE_RATES[] = {8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000};

static const char *song_list[] = {
	"http://dl.bekencorp.com/d/test/2023/music/qingtian.mp3?sign=YzsQI3YhI1QU6KQ9trGAzMo6OHJxAo0aMVNQtr2DLsg=:0",
	"http://dl.bekencorp.com/d/test/2023/music/chengdu.mp3?sign=WSKOehaDFGv8csOmTEGjHXHLaygTurgT5NEgjgw4jCY=:0",
	"http://dl.bekencorp.com/d/test/2023/music/myheart.mp3?sign=R9SLp-MvEEkiauuuKDmPsyV_74wePmKsRdJbFWw4mBo=:0",
};

static bk_err_t http_mp3_send_msg(audio_mp3_play_msg_t msg)
{
	bk_err_t ret = BK_OK;

	if (http_mp3_play_msg_que) {
		ret = rtos_push_to_queue(&http_mp3_play_msg_que, &msg, BEKEN_NO_WAIT);
		if (ret != kNoErr) {
			os_printf("http_mp3_send_msg failed\r\n");
			return kGeneralErr;
		}
		return ret;
	}
	return kNoResourcesErr;
}

static bk_err_t http_mp3_decode_handler(unsigned int size)
{
	bk_err_t ret = BK_OK;
	uint8_t idx = 0;

	if (psram_ptr >= mp3_end_addr) {
		audio_mp3_play_msg_t msg;
		msg.op = AUDIO_MP3_PLAY_STOP;
		ret = http_mp3_send_msg(msg);
		if (ret != kNoErr) {
			BK_LOGE(TAG, "http send msg: %d fails \r\n", msg.op);
			return BK_FAIL;
		}
		return BK_OK;
	}

	if (bytesLeft < MAINBUF_SIZE) {
		os_memmove(readBuf, g_readptr, bytesLeft);
		os_memcpy((void *)(readBuf + bytesLeft), (const void *)psram_ptr, MAINBUF_SIZE - bytesLeft);

		psram_ptr += MAINBUF_SIZE - bytesLeft;
		bytesLeft = MAINBUF_SIZE;
		g_readptr = readBuf;
	}

	offset = MP3FindSyncWord(g_readptr, bytesLeft);
	if (offset < 0) {
		BK_LOGI(TAG, "MP3FindSyncWord not find!\r\n");
		bytesLeft = 0;
	} else {
		g_readptr += offset;
		bytesLeft -= offset;

		ret = MP3Decode(hMP3Decoder, &g_readptr, &bytesLeft, pcmBuf, 0);
		if (ret != ERR_MP3_NONE) {
			BK_LOGE(TAG, "MP3Decode failed, code is %d", ret);
			return ret;
		}

		if (!aud_spk_init_flag) {
			MP3GetLastFrameInfo(hMP3Decoder, &mp3FrameInfo);

			for (idx = 0; idx < sizeof(MP3_SAMPLE_RATES) / sizeof(MP3_SAMPLE_RATES[0]); idx++) {
				if (MP3_SAMPLE_RATES[idx] == mp3FrameInfo.samprate) {
					aud_intf_spk_setup.samp_rate = idx;
					break;
				}
			}
			aud_intf_spk_setup.spk_chl = AUD_INTF_SPK_CHL_DUAL;
			aud_intf_spk_setup.frame_size = mp3FrameInfo.outputSamps * 2;
			aud_intf_spk_setup.spk_gain = 0x20;
			aud_intf_spk_setup.work_mode = AUD_DAC_WORK_MODE_DIFFEN;
			ret = bk_aud_intf_spk_init(&aud_intf_spk_setup);
			if (ret != BK_ERR_AUD_INTF_OK) {
				BK_LOGE(TAG, "bk_aud_intf_spk_init fail, ret:%d \r\n", ret);
			} else {
				BK_LOGI(TAG, "bk_aud_intf_spk_init complete \r\n");
			}

			ret = bk_aud_intf_spk_start();
			if (ret != BK_ERR_AUD_INTF_OK) {
				BK_LOGE(TAG, "bk_aud_intf_spk_start fail, ret:%d \r\n", ret);
			} else {
				BK_LOGI(TAG, "bk_aud_intf_spk_start complete \r\n");
			}

			aud_spk_init_flag = 1;
		}

		/* write a frame speaker data to speaker_ring_buff */
		ret = bk_aud_intf_write_spk_data((uint8_t*)pcmBuf, mp3FrameInfo.outputSamps * 2);
		if (ret != BK_OK) {
			BK_LOGE(TAG, "write spk data fail \r\n");
			return ret;
		}
	}

	return ret;

}

static void http_mp3_find_id3(void)
{
	char tag_header[10];
	int tag_size = 0;

	os_memcpy((void *)tag_header, (const void *)psram_buffer, 10);

	if (os_memcmp(tag_header, "ID3", 3) == 0) {
		tag_size = ((tag_header[6] & 0x7F) << 21) | ((tag_header[7] & 0x7F) << 14) | ((tag_header[8] & 0x7F) << 7) | (tag_header[9] & 0x7F);
		BK_LOGI(TAG, "tag_size = %d\r\n", tag_size);
		psram_ptr += tag_size + 10;
	} else {
		BK_LOGI(TAG, "tag_header not found!\r\n");
	}
}

static void http_mp3_play_main(void)
{
	bk_err_t ret = BK_OK;

	aud_intf_drv_setup_t aud_intf_drv_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
	aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;

	aud_intf_drv_setup.aud_intf_rx_spk_data = http_mp3_decode_handler;
	ret = bk_aud_intf_drv_init(&aud_intf_drv_setup);
	if (ret != BK_ERR_AUD_INTF_OK) {
		BK_LOGE(TAG, "bk_aud_intf_drv_init fail, ret:%d \r\n", ret);
	} else {
		BK_LOGI(TAG, "bk_aud_intf_drv_init complete \r\n");
	}

	aud_work_mode = AUD_INTF_WORK_MODE_GENERAL;
	ret = bk_aud_intf_set_mode(aud_work_mode);
	if (ret != BK_ERR_AUD_INTF_OK) {
		BK_LOGE(TAG, "bk_aud_intf_set_mode fail, ret:%d \r\n", ret);
	} else {
		BK_LOGI(TAG, "bk_aud_intf_set_mode complete \r\n");
	}

	while(1) {
		audio_mp3_play_msg_t msg;
		ret = rtos_pop_from_queue(&http_mp3_play_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			switch (msg.op) {
				case AUDIO_MP3_PLAY_START:
					bk_audio_mp3_play_decode_init();
					g_readptr = readBuf;
					http_mp3_find_id3();
					ret = http_mp3_decode_handler(0);
					if (ret != BK_OK) {
						BK_LOGE(TAG, "http_mp3_decode_handler error!\r\n");
					}
					break;

				case AUDIO_MP3_PLAY_STOP:
					aud_spk_init_flag = 0;

					ret = bk_aud_intf_spk_stop();
					if (ret != BK_ERR_AUD_INTF_OK) {
						BK_LOGE(TAG, "bk_aud_intf_spk_stop fail, ret:%d \r\n", ret);
					} else {
						BK_LOGI(TAG, "bk_aud_intf_spk_stop complete \r\n");
					}

					ret = bk_aud_intf_spk_deinit();
					if (ret != BK_ERR_AUD_INTF_OK) {
						BK_LOGE(TAG, "bk_aud_intf_spk_deinit fail, ret:%d \r\n", ret);
					} else {
						BK_LOGI(TAG, "bk_aud_intf_spk_deinit complete \r\n");
					}

					ret = bk_aud_intf_drv_deinit();
					if (ret != BK_ERR_AUD_INTF_OK) {
						BK_LOGE(TAG, "bk_aud_intf_drv_deinit fail, ret:%d \r\n", ret);
					} else {
						BK_LOGI(TAG, "bk_aud_intf_drv_deinit complete \r\n");
					}

					MP3FreeDecoder(hMP3Decoder);
					os_free(readBuf);
					os_free(pcmBuf);
					goto http_mp3_play_exit;
					break;

				case AUDIO_MP3_PLAY_EXIT:
					goto http_mp3_play_exit;
					break;

				default:
					break;
			}
		}
//		rtos_delay_milliseconds(2);
	}

http_mp3_play_exit:
	/* delete msg queue */
	rtos_deinit_queue(&http_mp3_play_msg_que);
	http_mp3_play_msg_que = NULL;
	BK_LOGI(TAG, "http_mp3_play queue deinit complete \r\n");

	http_mp3_play_thread_hdl = NULL;
	rtos_delete_thread(NULL);
}

static void http_get_main(void)
{
	struct webclient_session* session = NULL;
	int ret = 0;
	int bytes_read, resp_status;
	int content_length = -1;
	unsigned char *http_buffer = NULL;
	uint32_t *paddr  = NULL;
	paddr = (uint32_t*)psram_buffer;
	audio_mp3_play_msg_t msg;

	http_buffer = (unsigned char *) web_malloc(GET_RESP_BUFSZ);
	if (http_buffer == NULL) {
		BK_LOGE(TAG, "no memory for receive buffer.\n");
		goto __exit;
	}

	/* create webclient session and set header response size */
	session = webclient_session_create(GET_HEADER_BUFSZ);
	if (session == NULL) {
		goto __exit;
	}

	/* send GET request by default header */
	if ((resp_status = webclient_get(session, uri)) != 200) {
		BK_LOGE(TAG, "webclient GET request failed, response(%d) error.\n", resp_status);
		goto __exit;
	}

	BK_LOGI(TAG, "webclient get response data: \n");

	content_length = webclient_content_length_get(session);
	mp3_end_addr =(unsigned char *)psram_buffer + content_length;

	if (content_length < 0) {
		BK_LOGI(TAG, "webclient GET request type is chunked.\n");
		do
		{
			bytes_read = webclient_read(session, (void *)http_buffer, GET_RESP_BUFSZ);
			if (bytes_read <= 0) {
				break;
			}

		} while (1);
	} else {
		int content_pos = 0;

		do
		{
			bytes_read = webclient_read(session, (void *)http_buffer,
					content_length - content_pos > GET_RESP_BUFSZ ?
							GET_RESP_BUFSZ : content_length - content_pos);

			if (bytes_read <= 0) {
				break;
			}

			if(bytes_read != GET_RESP_BUFSZ) {
				if (bytes_read % 4) {
					bytes_read = (bytes_read / 4 + 1) * 4;
				}
				os_memcpy_word((uint32_t *)paddr, (uint32_t *)http_buffer, bytes_read);
			} else {
				os_memcpy_word((uint32_t *)paddr, (uint32_t *)http_buffer, GET_RESP_BUFSZ);
				paddr += (GET_RESP_BUFSZ / 4);
			}

			if (content_pos == GET_RESP_BUFSZ * 2) {
				msg.op = AUDIO_MP3_PLAY_START;
				ret = http_mp3_send_msg(msg);
				if (ret != kNoErr) {
					BK_LOGE(TAG, "http send msg: %d fails \r\n", msg.op);
					return;
				}
			}

			content_pos += bytes_read;
		} while (content_pos < content_length);
	}

	BK_LOGI(TAG, "webclient get response data complete\n");

__exit:
	if (session) {
		webclient_close(session);
	}

	if (http_buffer) {
		web_free(http_buffer);
	}

	if (uri) {
		web_free(uri);
	}

	http_get_thread_hdl = NULL;
	rtos_delete_thread(NULL);
}

void cli_http_mp3_play_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret = 0;
	uint32_t song_index = 0;
	psram_ptr = (unsigned char *)psram_buffer;

	if (os_strcmp(argv[1], "init") == 0) {
		ret = rtos_init_queue(&http_mp3_play_msg_que,
								"http_mp3_play_queue",
								sizeof(audio_mp3_play_msg_t),
								TU_QITEM_COUNT);
		if (ret != kNoErr) {
			BK_LOGE(TAG, "create http mp3 play message queue failed!\r\n");
			return;
		}

		ret = rtos_create_thread(&http_mp3_play_thread_hdl,
								 BEKEN_DEFAULT_WORKER_PRIORITY,
								 "http_mp3_play",
								 (beken_thread_function_t)http_mp3_play_main,
								 4096,
								 NULL);
		if (ret != kNoErr) {
			BK_LOGE(TAG, "http mp3 play task create fail!\r\n");
			rtos_deinit_queue(&http_mp3_play_msg_que);
			http_mp3_play_msg_que = NULL;
			http_mp3_play_thread_hdl = NULL;
		}

	} else if (os_strcmp(argv[1], "start") == 0) {
		song_index = os_strtoul(argv[2], NULL, 10);
		if (song_list[song_index] != NULL) {
			uri = web_strdup(song_list[song_index]);
			if(uri == NULL) {
				BK_LOGE(TAG, "no memory for create get request uri buffer.\n");
				return;
			}

			ret = rtos_create_thread(&http_get_thread_hdl,
									 BEKEN_DEFAULT_WORKER_PRIORITY,
									 "http_get",
									 (beken_thread_function_t)http_get_main,
									 2048,
									 NULL);
			if (ret != kNoErr) {
				BK_LOGE(TAG, "http get task create fail\r\n");
				http_get_thread_hdl = NULL;
			}
		}
	}else if (os_strcmp(argv[1], "pause") == 0) {
		ret = bk_aud_intf_spk_pause();
		if (ret != BK_ERR_AUD_INTF_OK) {
			BK_LOGE(TAG, "bk_aud_intf_spk_pause fail, ret:%d \r\n", ret);
		} else {
			BK_LOGI(TAG, "bk_aud_intf_spk_pause complete \r\n");
		}

	} else if (os_strcmp(argv[1], "play") == 0) {
		ret = bk_aud_intf_spk_start();
		if (ret != BK_ERR_AUD_INTF_OK) {
			BK_LOGE(TAG, "bk_aud_intf_spk_start fail, ret:%d \r\n", ret);
		} else {
			BK_LOGI(TAG, "bk_aud_intf_spk_start complete \r\n");
		}

	} else if (os_strcmp(argv[1], "set_volume") == 0) {
		uint32_t value = os_strtoul(argv[2], NULL, 0); // The range of value is 0x0 ~ 0x3F.
		bk_aud_intf_set_spk_gain(value);
	}
}

#endif


