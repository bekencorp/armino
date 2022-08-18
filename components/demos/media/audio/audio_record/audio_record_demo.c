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
#include "audio_record_types.h"
#include <modules/audio_ring_buff.h>
#include "ff.h"
#include "diskio.h"

#define  AUD_RECORD_DEBUG_ADC  0

FIL mic_file;

//static char file_name[] = "1:/signal_mic_data.pcm";    //save mic data received
static char file_name[50];


#define TU_QITEM_COUNT      (60)
static beken_thread_t  audio_record_thread_hdl = NULL;
static beken_queue_t aud_record_int_msg_que = NULL;


static int32_t *mic_ring_buff = NULL;     //存放audio adc采集到的mic信号
static int32_t *temp_mic_addr = NULL;     //存放从mic_ring_buff中取的mic信号


static dma_id_t adc_dma_id = DMA_ID_MAX;
static RingBufferContext mic_rb;

extern void delay(int num);

static bk_err_t audio_send_msg(audio_record_msg_t msg)
{
	bk_err_t ret;

	if (aud_record_int_msg_que) {
		ret = rtos_push_to_queue(&aud_record_int_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			os_printf("audio_send_msg failed\r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

static bk_err_t audio_adc_config(audio_sample_rate_t samp_rate)
{
	bk_err_t ret = BK_OK;
	aud_adc_config_t adc_config;

	adc_config.mic_config = AUD_MIC_MIC1_ENABLE;
	adc_config.samp_rate = samp_rate;
	adc_config.adc_enable = AUD_ADC_DISABLE;
	adc_config.line_enable = AUD_ADC_LINE_DISABLE;
	adc_config.dtmf_enable = AUD_DTMF_DISABLE;
	adc_config.adc_hpf2_coef_B2 = 0;
	adc_config.adc_hpf2_bypass_enable = AUD_ADC_HPF_BYPASS_ENABLE;
	adc_config.adc_hpf1_bypass_enable = AUD_ADC_HPF_BYPASS_ENABLE;
	adc_config.adc_set_gain = 0x2d;
	adc_config.adc_samp_edge = AUD_ADC_SAMP_EDGE_RISING;
	adc_config.adc_hpf2_coef_B0 = 0;
	adc_config.adc_hpf2_coef_B1 = 0;
	adc_config.adc_hpf2_coef_A0 = 0;
	adc_config.adc_hpf2_coef_A1 = 0;
	adc_config.dtmf_wr_threshold = 8;
	adc_config.adcl_wr_threshold = 8;
	adc_config.dtmf_int_enable = AUD_DTMF_INT_DISABLE;
	adc_config.adcl_int_enable = AUD_ADCL_INT_DISABLE;
	adc_config.loop_adc2dac = AUD_LOOP_ADC2DAC_DISABLE;
	adc_config.agc_noise_thrd = 101;
	adc_config.agc_noise_high = 101;
	adc_config.agc_noise_low = 160;
	adc_config.agc_noise_min = 1;
	adc_config.agc_noise_tout = 0;
	adc_config.agc_high_dur = 3;
	adc_config.agc_low_dur = 3;
	adc_config.agc_min = 1;
	adc_config.agc_max = 4;
	adc_config.agc_ng_method = AUD_AGC_NG_METHOD_MUTE;
	adc_config.agc_ng_enable = AUD_AGC_NG_DISABLE;
	adc_config.agc_decay_time = AUD_AGC_DECAY_TIME_128;
	adc_config.agc_attack_time = AUD_AGC_ATTACK_TIME_128;
	adc_config.agc_high_thrd = 18;
	adc_config.agc_low_thrd = 0;
	adc_config.agc_iir_coef = AUD_AGC_IIR_COEF_1_1024;
	adc_config.agc_enable = AUD_AGC_DISABLE;
	adc_config.manual_pga_value = 0;
	adc_config.manual_pga_enable = AUD_GAC_MANUAL_PGA_DISABLE;
	adc_config.adc_fracmod_manual = AUD_ADC_TRACMOD_MANUAL_DISABLE;
	adc_config.adc_fracmod = 0;

	/* init audio driver and config adc */
	ret = bk_aud_driver_init();
	if (ret != BK_OK) {
		os_printf("init audio driver fail \r\n");
		goto aud_adc_exit;
	}

	ret = bk_aud_adc_init(AUD_ADC_WORK_MODE_ADC, &adc_config, NULL);
	if (ret != BK_OK) {
		os_printf("init audio adc fail \r\n");
		goto aud_adc_exit;
	}

	return BK_OK;

aud_adc_exit:
	os_printf("audio adc config fail \r\n");
	bk_aud_driver_deinit();
	return BK_FAIL;
}

/* 搬运audio adc 采集到的一帧mic和ref信号后，触发中断通知AEC处理数据 */
static void audio_dma_adc_finish_isr(void)
{
	//os_printf("enter the audio_dma_adc_finish_isr \r\n");
	bk_err_t ret = BK_OK;
	audio_record_msg_t msg;

	/* send msg to process mic data */
	msg.op = AUDIO_MIC;
	ret = audio_send_msg(msg);
	if (ret != kNoErr) {
		os_printf("send msg: %d fails \r\n", msg.op);
	}
}

static bk_err_t audio_adc_dma_config(dma_id_t dma_id, int32_t *ring_buff_addr, uint32_t ring_buff_size, uint32_t transfer_len)
{
	bk_err_t ret = BK_OK;
	dma_config_t dma_config;
	uint32_t adc_port_addr;

	dma_config.mode = DMA_WORK_MODE_REPEAT;
	dma_config.chan_prio = 1;
	dma_config.src.dev = DMA_DEV_AUDIO;
	dma_config.src.width = DMA_DATA_WIDTH_16BITS;
	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;

	/* get adc fifo address */
	if (bk_aud_get_adc_fifo_addr(&adc_port_addr) != BK_OK) {
		os_printf("get adc fifo address failed\r\n");
		return BK_FAIL;
	} else {
		dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
		dma_config.src.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
		dma_config.src.start_addr = adc_port_addr + 2;
		dma_config.src.end_addr = adc_port_addr + 4;
	}

	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
	dma_config.dst.start_addr = (uint32_t)ring_buff_addr;
	dma_config.dst.end_addr = (uint32_t)ring_buff_addr + ring_buff_size;

	/* init dma channel */
	ret = bk_dma_init(dma_id, &dma_config);
	if (ret != BK_OK) {
		os_printf("audio adc dma channel init fail \r\n");
		return BK_FAIL;
	}

	/* set dma transfer length */
	bk_dma_set_transfer_len(dma_id, transfer_len);

	//register isr
	bk_dma_register_isr(dma_id, NULL, (void *)audio_dma_adc_finish_isr);
	bk_dma_enable_finish_interrupt(dma_id);

	return BK_OK;
}

static bk_err_t audio_mic_process(void)
{
	bk_err_t ret = BK_OK;
	uint32_t size = 0;
	FRESULT fr;
	uint32 uiTemp = 0;

	//os_printf("[audio_mic_process] \r\n");

#if AUD_RECORD_DEBUG_ADC
	size = ring_buffer_get_fill_size(&mic_rb);
	os_printf("mic_ref_rb: fill_size=%d \r\n", size);

	size = ring_buffer_get_free_size(&mic_rb);
	os_printf("mic_ref_rb: free_size=%d \r\n", size);
#endif

	/* get a fram mic data from mic_ref_ring_buff */
	size = ring_buffer_read(&mic_rb, (uint8_t*)temp_mic_addr, AUD_8K_FRAME_SAMP_SIZE);
	if (size != AUD_8K_FRAME_SAMP_SIZE) {
		os_printf("the mic data readed from mic_ring_buff is not a frame \r\n");
		return BK_FAIL;
	}

	/* write data to file */
	fr = f_write(&mic_file, (void *)temp_mic_addr, AUD_8K_FRAME_SAMP_SIZE, &uiTemp);
	//os_printf("write uiTemp:%d \r\n", uiTemp);
	if (fr != FR_OK) {
		os_printf("write %s fail.\r\n", file_name);
	}

#if AUD_RECORD_DEBUG_ADC
		size = ring_buffer_get_fill_size(&mic_rb);
		os_printf("aec_rb: fill_size=%d \r\n", size);

		size = ring_buffer_get_free_size(&mic_rb);
		os_printf("aec_rb: free_size=%d \r\n", size);
#endif
	return ret;
}

static void audio_record_main(beken_thread_arg_t param_data)
{
	bk_err_t ret = BK_OK;
	audio_setup_t *audio_setup = NULL;

	/*  -------------------------step2: init audio and config ADC -------------------------------- */
	audio_setup = (audio_setup_t *)(int)param_data;
	ret = audio_adc_config(audio_setup->samp_rate);
	if (ret != BK_OK) {
		os_printf("audio adc init fail \r\n");
		return;
	}

	/*  -------------------step3: init and config DMA to carry mic and ref data ----------------------------- */
	/* init dma driver */
	ret = bk_dma_driver_init();
	if (ret != BK_OK) {
		os_printf("dma driver init failed\r\n");
		return;
	}

	/* allocate free DMA channel */
	adc_dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
	if ((adc_dma_id < DMA_ID_0) || (adc_dma_id >= DMA_ID_MAX)) {
		os_printf("malloc dma fail \r\n");
		return;
	}
	os_printf("adc_dma_id: %d \r\n", adc_dma_id);

	/* malloc mic_ring_buff */
	os_printf("mic_ring_buff: %d \r\n", AUD_8K_FRAME_SAMP_SIZE*2);
	mic_ring_buff = os_malloc(AUD_8K_FRAME_SAMP_SIZE*2);
	if (mic_ring_buff == NULL) {
		os_printf("cp1: malloc mic_ring_buff ring buffer fail \r\n");
		goto audio_record_exit;
	}

	/* config audio adc dma to carry mic and ref data to "mic_ref_ring_buff" */
	ret = audio_adc_dma_config(adc_dma_id, mic_ring_buff, AUD_8K_FRAME_SAMP_SIZE*2, AUD_8K_FRAME_SAMP_SIZE);
	if (ret != BK_OK) {
		os_printf("config audio adc dma fail \r\n");
		return;
	}
	os_printf("step3: init and config DMA to carry mic data complete \r\n");

	/*  -------------------------step4: init all audio ring buffers -------------------------------- */
	/* init mic_ring_buff */
	ring_buffer_init(&mic_rb, (uint8_t*)mic_ring_buff, AUD_8K_FRAME_SAMP_SIZE*2, adc_dma_id, RB_DMA_TYPE_WRITE);

	/* malloc temp_mic_addr */
	os_printf("mic_ref_ring_buff: %d \r\n", AUD_8K_FRAME_SAMP_SIZE);
	temp_mic_addr = os_malloc(AUD_8K_FRAME_SAMP_SIZE);
	if (temp_mic_addr == NULL) {
		os_printf("cp1: malloctemp_mic_addr ring buffer fail \r\n");
		goto audio_record_exit;
	}

	ret = bk_dma_start(adc_dma_id);
	if (ret != BK_OK) {
		os_printf("cp1: start dac dma fail \r\n");
		return;
	}

	/* enable adc */
	bk_aud_start_adc();

	while(1) {
		audio_record_msg_t msg;
		ret = rtos_pop_from_queue(&aud_record_int_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			switch (msg.op) {
				case AUDIO_IDLE:
					break;

				case AUDIO_MIC:
					ret = audio_mic_process();
					if (ret != BK_OK) {
						os_printf("audio_mic_process excute fail \r\n");
					}
					break;

				case AUDIO_EXIT:
					os_printf("goto audio_record_exit \r\n");
					goto audio_record_exit;
					break;

				default:
					break;
			}
		}
	}

audio_record_exit:
	/* disable audio adc and dac */
	bk_aud_stop_adc();
	bk_aud_adc_deinit();
	bk_aud_driver_deinit();

	/* stop dma */
	bk_dma_stop(adc_dma_id);
	bk_dma_deinit(adc_dma_id);
	bk_dma_free(DMA_DEV_AUDIO, adc_dma_id);

	/* free audio ring buffer */
	ring_buffer_clear(&mic_rb);
	os_free(mic_ring_buff);

	/* delate msg queue */
	ret = rtos_deinit_queue(&aud_record_int_msg_que);
	if (ret != kNoErr) {
		os_printf("delate message queue fail \r\n");
		//return BK_FAIL;
	}
	aud_record_int_msg_que = NULL;
	os_printf("delate message queue complete \r\n");

	/* delate task */
	rtos_delete_thread(NULL);
	audio_record_thread_hdl = NULL;
	os_printf("delate task \r\n");

}

audio_setup_t audio_record_setup_bak = {0};

bk_err_t bk_audio_record_init(audio_setup_t *setup_cfg)
{
	bk_err_t ret = BK_OK;
	if ((!audio_record_thread_hdl) && (!aud_record_int_msg_que)) {
		os_printf("start audio_record test \r\n");
		os_memcpy(&audio_record_setup_bak, setup_cfg, sizeof(audio_setup_t));

		ret = rtos_init_queue(&aud_record_int_msg_que,
							  "audio_internal_queue",
							  sizeof(audio_record_msg_t),
							  TU_QITEM_COUNT);
		if (ret != kNoErr) {
			os_printf("ceate audio internal message queue failed \r\n");
			return BK_FAIL;
		}
		os_printf("ceate audio internal message queuecomplete \r\n");

		//creat audio record task
		ret = rtos_create_thread(&audio_record_thread_hdl,
							 BEKEN_DEFAULT_WORKER_PRIORITY,
							 "audio_intf",
							 (beken_thread_function_t)audio_record_main,
							 4096,
							 (beken_thread_arg_t)&audio_record_setup_bak);
		if (ret != kNoErr) {
			os_printf("create audio record task fail \r\n");
			rtos_deinit_queue(&aud_record_int_msg_que);
			aud_record_int_msg_que = NULL;
			audio_record_thread_hdl = NULL;
		}
		os_printf("create audio record task complete \r\n");

		return kNoErr;
	} else
		return kInProgressErr;
}

bk_err_t bk_audio_record_deinit(void)
{
	bk_err_t ret = BK_OK;
	audio_record_msg_t msg;

	/* send msg to encoder to encoding data */
	msg.op = AUDIO_EXIT;
	ret = audio_send_msg(msg);
	if (ret != kNoErr) {
		os_printf("send msg: %d fails \r\n", msg.op);
		return BK_FAIL;
	}
	return ret;
}

static void cli_audio_record_help(void)
{
	os_printf("aud_cp0_audio_record_test {start|stop xxx.pcm} \r\n");
}

void cli_cp0_audio_record_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;
	FRESULT fr;

	if (argc != 3) {
		cli_audio_record_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("cp0: start audio_record test \r\n");

		/*  -------------------------step1: init audio and config ADC -------------------------------- */
		/*open file to save mic data has been encoding by g711a */
		sprintf(file_name, "1:/%s", argv[2]);
		fr = f_open(&mic_file, file_name, FA_CREATE_ALWAYS | FA_WRITE);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", file_name);
			return;
		}

		audio_setup_t config;
		config.samp_rate = AUDIO_SAMP_RATE_8K;
		/* init audio cpu0 record task */
		ret = bk_audio_record_init(&config);
		if (ret != BK_OK) {
			os_printf("cp0: init audio record task fail \r\n");
			return;
		}
		os_printf("cp0: start audio_record test successful\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		ret = bk_audio_record_deinit();
		if (ret != BK_OK) {
			os_printf("deinit audio record task fail \r\n");
		}

		delay(1000);
		/* close encoder file */
		fr = f_close(&mic_file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", file_name);
			return;
		}

		os_printf("stop audio_record test successful \r\n");
	} else {
		cli_audio_record_help();
		return;
	}

}

