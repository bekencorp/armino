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
#include "aud_hal.h"
#include "sys_driver.h"
#include "aud_driver.h"
#include "audio_play_types.h"
#include <modules/audio_ring_buff.h>
#include "ff.h"
#include "diskio.h"

#define  AUD_PLAY_DEBUG_DAC  0

FIL speaker_file;

//static char file_name[] = "1:/signal_mic_data.pcm";    //save mic data received
static char file_name[50];


#define TU_QITEM_COUNT      (60)
static beken_thread_t  audio_play_thread_hdl = NULL;
static beken_queue_t aud_play_int_msg_que = NULL;


static int32_t *speaker_ring_buff = NULL;     //存放audio dac要播放的pcm信号
static int32_t *temp_speaker_addr = NULL;     //存放从speaker_ring_buff中取的pcm信号


static dma_id_t dac_dma_id = DMA_ID_MAX;
static RingBufferContext speaker_rb;

extern void delay(int num);

static bk_err_t audio_send_msg(audio_play_msg_t msg)
{
	bk_err_t ret;

	if (aud_play_int_msg_que) {
		ret = rtos_push_to_queue(&aud_play_int_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			os_printf("audio_send_msg failed\r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

static bk_err_t audio_dac_config(void)
{
	bk_err_t ret = BK_OK;
	aud_dac_config_t dac_config;

	dac_config.dac_enable = AUD_DAC_DISABLE;
	dac_config.samp_rate = AUD_DAC_SAMP_RATE_SOURCE_8K;
	dac_config.dac_hpf2_coef_B2 = 0x3A22;
	dac_config.dac_hpf2_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
	dac_config.dac_hpf1_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
	dac_config.dac_set_gain = 0x15;    //default 2D  3F
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


/* 搬运audio dac 一帧dac信号后，触发中断通知decoder处理数据 */
static void audio_dma_dac_finish_isr(void)
{
	//os_printf("enter the audio_dma_dac_finish_isr \r\n");
	bk_err_t ret = BK_OK;
	audio_play_msg_t msg;

	/* send msg to read pcm data */
	msg.op = AUDIO_SPEAKER;
	ret = audio_send_msg(msg);
	if (ret != kNoErr) {
		os_printf("cp1: send msg: %d fails \r\n", msg.op);
	}
}

static bk_err_t audio_dac_dma_config(dma_id_t dma_id, int32_t *ring_buff_addr, uint32_t ring_buff_size, uint32_t transfer_len)
{
	bk_err_t ret = BK_OK;
	dma_config_t dma_config;
	uint32_t dac_port_addr;

	dma_config.mode = DMA_WORK_MODE_REPEAT;
	dma_config.chan_prio = 1;
	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.dev = DMA_DEV_AUDIO;
	dma_config.dst.width = DMA_DATA_WIDTH_16BITS;

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
	bk_dma_register_isr(dma_id, NULL, (void *)audio_dma_dac_finish_isr);
	bk_dma_enable_finish_interrupt(dma_id);

	//bk_dma_start(dma_id);

	return BK_OK;
}

static bk_err_t audio_speaker_process(void)
{
	bk_err_t ret = BK_OK;
	uint32_t size = 0;
	FRESULT fr;
	uint32 uiTemp = 0;

	//os_printf("[audio_speaker_process] \r\n");

#if AUD_PLAY_DEBUG_DAC
	size = ring_buffer_get_fill_size(&speaker_rb);
	os_printf("speaker_rb: fill_size=%d \r\n", size);

	size = ring_buffer_get_free_size(&speaker_rb);
	os_printf("speaker_rb: free_size=%d \r\n", size);
#endif

	/* read data from file */
	fr = f_read(&speaker_file, (void *)temp_speaker_addr, AUD_8K_SPEAKER_SAMP_SIZE, &uiTemp);
	if (fr != FR_OK) {
		os_printf("write %s fail.\r\n", file_name);
	}

	/* write a fram speaker data to speaker_ring_buff */
	size = ring_buffer_write(&speaker_rb, (uint8_t*)temp_speaker_addr, AUD_8K_SPEAKER_SAMP_SIZE);
	if (size != AUD_8K_SPEAKER_SAMP_SIZE) {
		os_printf("the mic data readed from mic_ring_buff is not a frame \r\n");
		return BK_FAIL;
	}

#if AUD_PLAY_DEBUG_DAC
		size = ring_buffer_get_fill_size(&speaker_rb);
		os_printf("speaker_rb: fill_size=%d \r\n", size);

		size = ring_buffer_get_free_size(&speaker_rb);
		os_printf("speaker_rb: free_size=%d \r\n", size);
#endif
	return ret;
}

static bk_err_t audio_play_ready(void)
{
	FRESULT fr;
	uint32 uiTemp = 0;
	uint32_t size = 0;

	/* read data from file */
	fr = f_read(&speaker_file, (void *)temp_speaker_addr, AUD_8K_SPEAKER_SAMP_SIZE, &uiTemp);
	if (fr != FR_OK) {
		os_printf("write %s fail.\r\n", file_name);
	}

	/* write a fram speaker data to speaker_ring_buff */
	size = ring_buffer_write(&speaker_rb, (uint8_t*)temp_speaker_addr, AUD_8K_SPEAKER_SAMP_SIZE);
	if (size != AUD_8K_SPEAKER_SAMP_SIZE) {
		os_printf("the mic data readed from mic_ring_buff is not a frame \r\n");
		return BK_FAIL;
	}

	return BK_OK;
}

static void audio_play_main(void)
{
	bk_err_t ret = BK_OK;

	/*  -------------------------step1: init audio and config ADC -------------------------------- */
	ret = audio_dac_config();
	if (ret != BK_OK) {
		os_printf("audio adc init fail \r\n");
		return;
	}

	/*  -------------------step2: init and config DMA to carry mic and ref data ----------------------------- */
	/* init dma driver */
	ret = bk_dma_driver_init();
	if (ret != BK_OK) {
		os_printf("dma driver init failed\r\n");
		return;
	}

	/* allocate free DMA channel */
	dac_dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
	if ((dac_dma_id < DMA_ID_0) || (dac_dma_id >= DMA_ID_MAX)) {
		os_printf("malloc dma fail \r\n");
		return;
	}
	os_printf("dac_dma_id: %d \r\n", dac_dma_id);

	/* malloc speaker_ring_buff */
	os_printf("speaker_ring_buff: %d \r\n", AUD_8K_SPEAKER_SAMP_SIZE*2);
	speaker_ring_buff = os_malloc(AUD_8K_SPEAKER_SAMP_SIZE*2);
	if (speaker_ring_buff == NULL) {
		os_printf("malloc mic_ring_buff ring buffer fail \r\n");
		goto audio_play_exit;
	}

	/* config audio dac dma to carry pcm data to "speaker_ref_ring_buff" */
	ret = audio_dac_dma_config(dac_dma_id, speaker_ring_buff, AUD_8K_SPEAKER_SAMP_SIZE*2, AUD_8K_SPEAKER_SAMP_SIZE);
	if (ret != BK_OK) {
		os_printf("config audio adc dma fail \r\n");
		return;
	}
	os_printf("step2: init and config DMA to carry pcm data complete \r\n");

	/*  -------------------------step3: init all audio ring buffers -------------------------------- */
	/* init speaker_ring_buff */
	ring_buffer_init(&speaker_rb, (uint8_t*)speaker_ring_buff, AUD_8K_SPEAKER_SAMP_SIZE*2, dac_dma_id, RB_DMA_TYPE_READ);

	/* malloc temp_speaker_addr */
	os_printf("speaker_ref_ring_buff: %d \r\n", AUD_8K_SPEAKER_SAMP_SIZE);
	temp_speaker_addr = os_malloc(AUD_8K_SPEAKER_SAMP_SIZE);
	if (temp_speaker_addr == NULL) {
		os_printf("cp1: malloc temp_speaker_addr buffer fail \r\n");
		goto audio_play_exit;
	}

	ret = bk_dma_start(dac_dma_id);
	if (ret != BK_OK) {
		os_printf("start dac dma fail \r\n");
		return;
	}

	/* enable dac */
	bk_aud_start_dac();

	/* read a fram pcm data before start test */
	audio_play_ready();
	//audio_dma_dac_finish_isr();
	delay(100);
	bk_aud_start_dac();

	while(1) {
		audio_play_msg_t msg;
		ret = rtos_pop_from_queue(&aud_play_int_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			switch (msg.op) {
				case AUDIO_IDLE:
					break;

				case AUDIO_SPEAKER:
					ret = audio_speaker_process();
					if (ret != BK_OK) {
						os_printf("audio_mic_process excute fail \r\n");
					}
					break;

				case AUDIO_EXIT:
					os_printf("goto audio_play_exit \r\n");
					goto audio_play_exit;
					break;

				default:
					break;
			}
		}
	}

audio_play_exit:
	/* disable audio adc and dac */
	bk_aud_stop_dac();
	bk_aud_dac_deinit();
	bk_aud_driver_deinit();

	/* stop dma */
	bk_dma_stop(dac_dma_id);
	bk_dma_deinit(dac_dma_id);
	bk_dma_free(DMA_DEV_AUDIO, dac_dma_id);

	/* free audio ring buffer */
	ring_buffer_clear(&speaker_rb);
	os_free(speaker_ring_buff);

	/* delate msg queue */
	ret = rtos_deinit_queue(&aud_play_int_msg_que);
	if (ret != kNoErr) {
		os_printf("delate message queue fail \r\n");
		//return BK_FAIL;
	}
	aud_play_int_msg_que = NULL;
	os_printf("delate message queue complete \r\n");

	/* delate task */
	rtos_delete_thread(NULL);
	audio_play_thread_hdl = NULL;
	os_printf("delate task \r\n");

}

bk_err_t bk_audio_play_init(void)
{
	bk_err_t ret = BK_OK;
	if ((!audio_play_thread_hdl) && (!aud_play_int_msg_que)) {
		os_printf("start audio_play test \r\n");

		ret = rtos_init_queue(&aud_play_int_msg_que,
							  "audio_internal_queue",
							  sizeof(audio_play_msg_t),
							  TU_QITEM_COUNT);
		if (ret != kNoErr) {
			os_printf("ceate audio internal message queue failed \r\n");
			return BK_FAIL;
		}
		os_printf("ceate audio internal message queuecomplete \r\n");

		//creat audio play task
		ret = rtos_create_thread(&audio_play_thread_hdl,
							 BEKEN_DEFAULT_WORKER_PRIORITY,
							 "audio_intf",
							 (beken_thread_function_t)audio_play_main,
							 4096,
							 NULL);
		if (ret != kNoErr) {
			os_printf("create audio play task fail \r\n");
			rtos_deinit_queue(&aud_play_int_msg_que);
			aud_play_int_msg_que = NULL;
			audio_play_thread_hdl = NULL;
		}
		os_printf("create audio play task complete \r\n");

		return kNoErr;
	} else
		return kInProgressErr;
}

bk_err_t bk_audio_play_deinit(void)
{
	bk_err_t ret = BK_OK;
	audio_play_msg_t msg;

	msg.op = AUDIO_EXIT;
	ret = audio_send_msg(msg);
	if (ret != kNoErr) {
		os_printf("send msg: %d fails \r\n", msg.op);
		return BK_FAIL;
	}
	return ret;
}

static void cli_audio_play_help(void)
{
	os_printf("aud_cp0_audio_play_test {start|stop xxx.pcm} \r\n");
}

void cli_cp0_audio_play_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;
	FRESULT fr;

	if (argc != 3) {
		cli_audio_play_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("start audio_play test \r\n");

		/*open file to read pcm data */
		sprintf(file_name, "1:/%s", argv[2]);
		fr = f_open(&speaker_file, file_name, FA_OPEN_EXISTING | FA_READ);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", file_name);
			return;
		}

		/* init audio play task */
		ret = bk_audio_play_init();
		if (ret != BK_OK) {
			os_printf("init audio play task fail \r\n");
			return;
		}
		os_printf("cp0: start audio_play test successful\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		ret = bk_audio_play_deinit();
		if (ret != BK_OK) {
			os_printf("deinit audio play task fail \r\n");
		}

		delay(1000);
		/* close encoder file */
		fr = f_close(&speaker_file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", file_name);
			return;
		}

		os_printf("stop audio_play test successful \r\n");
	} else {
		cli_audio_play_help();
		return;
	}

}

