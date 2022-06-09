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
#include <driver/aud.h>
#include <driver/aud_types.h>
#include <driver/dma.h>
#include "aud_hal.h"
#include "sys_driver.h"
#include "aud_driver.h"
#include "audio_mailbox.h"
#include "audio_types.h"
#include <driver/psram.h>
#include <modules/aec.h>
#include <modules/audio_ring_buff.h>
#include <modules/g711.h>

#define  AUD_TRANS_DEBUG_DAC  0
#define  AUD_TRANS_DEBUG_ADC  0
#define  AUD_DEBUG  0


typedef enum {
	ADC_TEST_MODE_NULL = 0,
	ADC_TEST_MODE_MCP,
	ADC_TEST_MODE_DMA,
	ADC_TEST_MODE_MAX
} aud_test_mode_t;


#define TU_QITEM_COUNT      (60)
static beken_thread_t  audio_thread_hdl = NULL;
static beken_queue_t aud_int_msg_que = NULL;

AECContext* aec = NULL;

uint32_t val = 0;
int16_t samplerate = 8000;
uint32_t aec_context_size = 0;
static uint32_t frame_sample = 0;        //一帧AEC处理数据的点数
int16_t* ref_addr = NULL;
int16_t* mic_addr = NULL;
int16_t* out_addr = NULL;
int16_t* temp_mic_ref_addr = NULL;
int32_t *mic_ref_ring_buff = NULL;     //存放audio adc采集到的mic信号的ref参考信号
int16_t *aec_ring_buff = NULL;         //存放经过aec算法处理后的mic信号
int32_t *speaker_ring_buff = NULL;     //存放经过decoder解码后的dac信号

dma_id_t adc_dma_id = DMA_ID_MAX;
dma_id_t dac_dma_id = DMA_ID_MAX;
static RingBufferContext mic_ref_rb;
static RingBufferContext aec_rb;
static RingBufferContext encoder_rb;
static RingBufferContext decoder_rb;
static RingBufferContext speaker_rb;

static uint32_t* read_buffer = NULL;    //save data read from CPU1
static uint32_t read_buffer_size = 0;
static uint32_t* write_buffer = NULL;    //save data write to CPU1
static uint32_t write_buffer_size = 0;


extern void delay(int num);


static bk_err_t audio_send_msg(audio_cp1_msg_t msg)
{
	bk_err_t ret;

	if (aud_int_msg_que) {
		ret = rtos_push_to_queue(&aud_int_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			os_printf("cp1: audio_send_msg failed\r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

static bk_err_t audio_send_stop_msg(audio_cp1_msg_t msg)
{
	bk_err_t ret;

	if (aud_int_msg_que) {
		ret = rtos_push_to_queue_front(&aud_int_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			os_printf("cp1: audio_send_stop_msg failed\r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

static void audio_cp1_mailbox_rx_isr(aud_mb_t *aud_mb, mb_chnl_cmd_t *cmd_buf)
{
	//os_printf("[cp1_mailbox_rx_isr] \r\n");
	bk_err_t ret = BK_OK;
	audio_cp1_msg_t msg;
	uint32_t size = 0;
	audio_mailbox_msg_t mb_msg;

	msg.op = AUDIO_CP1_IDLE;

	/* check mailbox msg and send msg to task */
	switch(cmd_buf->hdr.cmd) {
		case AUD_MB_READ_ADC_DATA_REQ_CMD:
			read_buffer = (uint32_t *)cmd_buf->param1;
			read_buffer_size = cmd_buf->param2;
			//os_printf("AUD_MB_READ_ADC_DATA_REQ_CMD: read_buffer=0x%p, read_buffer_size=%d \r\n", read_buffer, read_buffer_size);
			msg.op = AUDIO_CP1_READ_REQ;
			break;

		case AUD_MB_WRITE_DAC_DATA_REQ_CMD:
			write_buffer = (uint32_t *)cmd_buf->param1;
			write_buffer_size = cmd_buf->param2;
			//os_printf("AUD_MB_WRITE_DAC_DATA_REQ_CMD: write_buffer=0x%p, write_buffer_size=%d \r\n", write_buffer, write_buffer_size);
			msg.op = AUDIO_CP1_WRITE_REQ;
			break;

		case AUD_MB_GET_ENCODER_USED_SIZE_REQ_CMD:
			size = ring_buffer_get_fill_size(&encoder_rb);
			/* send encoder used size to cpu0 by mailbox msg */
			mb_msg.mb_cmd = AUD_MB_GET_ENCODER_USED_SIZE_DONE_CMD;
			mb_msg.param1 = size;
			mb_msg.param2 = 0;
			mb_msg.param3 = 0;
			aud_mailbox_send_msg(&mb_msg);
			return;

		case AUD_MB_GET_DECODER_REMAIN_SIZE_REQ_CMD:
			size = ring_buffer_get_free_size(&decoder_rb);
			//os_printf("cp1: remain size: %d \r\n", size);
			/* send decoder remain size to cpu0 by mailbox msg */
			mb_msg.mb_cmd = AUD_MB_GET_DECODER_REMAIN_SIZE_DONE_CMD;
			mb_msg.param1 = size;
			mb_msg.param2 = 0;
			mb_msg.param3 = 0;
			aud_mailbox_send_msg(&mb_msg);
			return;

		case AUD_MB_START_TRANSFER_CMD:
			msg.op = AUDIO_CP1_START;
			break;

		case AUD_MB_STOP_TRANSFER_CMD:
			msg.op = AUDIO_CP1_EXIT;
			ret = audio_send_stop_msg(msg);
			if (ret != kNoErr) {
				os_printf("cp1: send stop msg: %d fail \r\n", msg.op);
			}
			os_printf("cp1: send stop msg: %d complete \r\n", msg.op);
			return;

		default:
			break;
	}

	ret = audio_send_msg(msg);
	if (ret != kNoErr) {
		os_printf("cp1: send msg: %d fail \r\n", msg.op);
	}

}

static void audio_cp1_mailbox_tx_isr(aud_mb_t *aud_mb)
{
	//os_printf("enter cp1_mailbox_tx_isr \r\n");
}

static void audio_cp1_mailbox_tx_cmpl_isr(aud_mb_t *aud_mb, mb_chnl_ack_t *cmd_buf)
{
	//os_printf("enter cp1_mailbox_tx_cmpl_isr \r\n");
}


static bk_err_t audio_adc_config(audio_sample_rate_t samp_rate)
{
	bk_err_t ret = BK_OK;
	aud_adc_config_t adc_config;

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
		os_printf("cp1: init audio driver fail \r\n");
		goto aud_adc_exit;
	}

	ret = bk_aud_adc_init(AUD_ADC_WORK_MODE_ADC, &adc_config, NULL);
	if (ret != BK_OK) {
		os_printf("cp1: init audio adc fail \r\n");
		goto aud_adc_exit;
	}

	return BK_OK;

aud_adc_exit:
	os_printf("cp1: audio adc config fail \r\n");
	bk_aud_driver_deinit();
	return BK_FAIL;
}

static bk_err_t audio_dac_config(audio_sample_rate_t samp_rate)
{
	bk_err_t ret = BK_OK;
	aud_dac_config_t dac_config;

	dac_config.dac_enable = AUD_DAC_DISABLE;
	//dac_config.samp_rate = samp_rate;
	dac_config.samp_rate = AUDIO_SAMP_RATE_8K;
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
		os_printf("cp1: init audio driver fail \r\n");
		goto aud_dac_exit;
	}

	ret = bk_aud_dac_init(&dac_config);
	if (ret != BK_OK) {
		os_printf("cp1: init audio dac fail \r\n");
		goto aud_dac_exit;
	}

	return BK_OK;

aud_dac_exit:
	os_printf("cp1: audio dac config fail \r\n");
	bk_aud_driver_deinit();
	return BK_FAIL;
}

#if 1
static void audio_aec_config(audio_sample_rate_t samp_rate)
{
	/* init aec context */
	aec_context_size = aec_size();
	aec = (AECContext*)os_malloc(aec_context_size);

	/* config sample rate, default is 8K */
	if (samp_rate == AUDIO_SAMP_RATE_16K)
		aec_init(aec, 16000);
	else
		aec_init(aec, 8000);

	/* 获取处理帧长，16000采样率320点(640字节)，8000采样率160点(320字节)  (对应20毫秒数据) */
	aec_ctrl(aec, AEC_CTRL_CMD_GET_FRAME_SAMPLE, (uint32_t)(&frame_sample));

	/* 获取结构体内部可以复用的ram作为每帧tx,rx,out数据的临时buffer; ram很宽裕的话也可以在外部单独申请获取 */
	aec_ctrl(aec, AEC_CTRL_CMD_GET_TX_BUF, (uint32_t)(&val)); mic_addr = (int16_t*)val;
	aec_ctrl(aec, AEC_CTRL_CMD_GET_RX_BUF, (uint32_t)(&val)); ref_addr = (int16_t*)val;
	aec_ctrl(aec, AEC_CTRL_CMD_GET_OUT_BUF,(uint32_t)(&val)); out_addr = (int16_t*)val;

	/* 以下是参数调节示例,aec_init中都已经有默认值,可以直接先用默认值 */
	aec_ctrl(aec, AEC_CTRL_CMD_SET_FLAGS, 0x1f);							  //库内各模块开关; aec_init内默认赋值0x1f;

	/* 回声消除相关 */
	aec_ctrl(aec, AEC_CTRL_CMD_SET_MIC_DELAY, 10);							  //设置参考信号延迟(采样点数，需要dump数据观察)
	aec_ctrl(aec, AEC_CTRL_CMD_SET_EC_DEPTH, 0);							  //建议取值范围1~50; 后面几个参数建议先用aec_init内的默认值，具体需要根据实际情况调试; 总得来说回声越大需要调的越大
	aec_ctrl(aec, AEC_CTRL_CMD_SET_TxRxThr, 13);							  //建议取值范围10~64
	aec_ctrl(aec, AEC_CTRL_CMD_SET_TxRxFlr, 1); 							  //建议取值范围1~10
	aec_ctrl(aec, AEC_CTRL_CMD_SET_REF_SCALE, 0);							  //取值0,1,2；rx数据如果幅值太小的话适当放大
	aec_ctrl(aec, AEC_CTRL_CMD_SET_VOL, 14);								  //通话过程中如果需要经常调节喇叭音量就设置下当前音量等级
	/* 降噪相关 */
	aec_ctrl(aec, AEC_CTRL_CMD_SET_NS_LEVEL, 2);							  //建议取值范围1~8；值越小底噪越小
	aec_ctrl(aec, AEC_CTRL_CMD_SET_NS_PARA,  1);							  //只能取值0,1,2; 降噪由弱到强，建议默认值
	/* drc(输出音量相关) */
	aec_ctrl(aec, AEC_CTRL_CMD_SET_DRC, 0x15);								  //建议取值范围0x10~0x1f;   越大输出声音越大

}
#endif

/* 搬运audio adc 采集到的一帧mic和ref信号后，触发中断通知AEC处理数据 */
static void audio_dma_adc_finish_isr(void)
{
	//os_printf("cp1: enter the audio_dma_adc_finish_isr \r\n");
	bk_err_t ret = BK_OK;
	audio_cp1_msg_t msg;

	/* send msg to AEC to process mic and ref data */
	msg.op = AUDIO_CP1_AEC;
	ret = audio_send_msg(msg);
	if (ret != kNoErr) {
		os_printf("cp1: send msg: %d fails \r\n", msg.op);
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
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;

	/* get adc fifo address */
	if (bk_aud_get_adc_fifo_addr(&adc_port_addr) != BK_OK) {
		os_printf("get adc fifo address failed\r\n");
		return BK_FAIL;
	} else {
		dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
		dma_config.src.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
		dma_config.src.start_addr = adc_port_addr;
		dma_config.src.end_addr = adc_port_addr + 4;
	}

	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
	dma_config.dst.start_addr = (uint32_t)ring_buff_addr;
	dma_config.dst.end_addr = (uint32_t)ring_buff_addr + ring_buff_size;

	/* init dma channel */
	ret = bk_dma_init(dma_id, &dma_config);
	if (ret != BK_OK) {
		os_printf("cp1: audio adc dma channel init fail \r\n");
		return BK_FAIL;
	}

	/* set dma transfer length */
	bk_dma_set_transfer_len(dma_id, transfer_len);

	//register isr
	bk_dma_register_isr(dma_id, NULL, (void *)audio_dma_adc_finish_isr);
	bk_dma_enable_finish_interrupt(dma_id);

	//bk_dma_set_transfer_len(dma_id, 0x20);
	//bk_dma_start(dma_id);

	return BK_OK;
}

/* 搬运audio dac 一帧dac信号后，触发中断通知decoder处理数据 */
static void audio_dma_dac_finish_isr(void)
{
	//os_printf("cp1: enter the audio_dma_dac_finish_isr \r\n");
	bk_err_t ret = BK_OK;
	audio_cp1_msg_t msg;

	/* send msg to decoder to decoding recevied data */
	msg.op = AUDIO_CP1_DECODER;
	ret = audio_send_msg(msg);
	if (ret != kNoErr) {
		os_printf("cp1: send msg: %d fails \r\n", msg.op);
	}

}

/* 请求搬数据播放 */
#if 0
static void audio_cp1_req_dac_data(void)
{
	os_printf("cp1: enter the audio_cp1_req_dac_data \r\n");
	bk_err_t ret = BK_OK;
	audio_cp1_msg_t msg;

	/* send msg to decoder to decoding recevied data */
	msg.op = AUDIO_CP1_DECODER;
	ret = audio_send_msg(msg);
	if (ret != kNoErr) {
		os_printf("cp1: send msg: %d fails \r\n", msg.op);
	}

}
#endif

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
		os_printf("cp1: audio dac dma channel init fail \r\n");
		return BK_FAIL;
	}

	/* set dma transfer length */
	bk_dma_set_transfer_len(dma_id, transfer_len);

	//register isr
	bk_dma_register_isr(dma_id, NULL, (void *)audio_dma_dac_finish_isr);
	bk_dma_enable_finish_interrupt(dma_id);

	//bk_dma_set_transfer_len(dma_id, 0x20);
	//bk_dma_start(dma_id);

	return BK_OK;
}

static bk_err_t audio_aec_process(void)
{
	bk_err_t ret = BK_OK;
	uint32_t size = 0;
	uint32_t i = 0;
	audio_cp1_msg_t msg;
#if AUD_DEBUG
	os_printf("[audio_aec_process] \r\n");
#endif

#if AUD_TRANS_DEBUG_ADC
	size = ring_buffer_get_fill_size(&mic_ref_rb);
	os_printf("mic_ref_rb: fill_size=%d \r\n", size);

	size = ring_buffer_get_free_size(&mic_ref_rb);
	os_printf("mic_ref_rb: free_size=%d \r\n", size);
#endif

	/* get a fram mic and ref data from mic_ref_ring_buff */
	//size = ring_buffer_read(&mic_ref_rb, (uint8_t*)out_addr, frame_sample*2*2);
	size = ring_buffer_read(&mic_ref_rb, (uint8_t*)temp_mic_ref_addr, frame_sample*2*2);
	if (size != frame_sample*2*2) {
		os_printf("cp1: the mic and ref data readed from mic_ref_ring_buff is not a frame \r\n");
		return BK_FAIL;
	}

	/* divide the data into mic data and ref data */
	for (i = 0; i<frame_sample; i++) {
		mic_addr[i] = temp_mic_ref_addr[2*i];
		ref_addr[i] = temp_mic_ref_addr[2*i+1];
	}

	/* aec process data */
	aec_proc(aec, ref_addr, mic_addr, out_addr);

	/* save mic data after aec processed to aec_ring_buffer */
	//size = ring_buffer_write(&aec_rb, (uint8_t*)out_addr, frame_sample*2);
	size = ring_buffer_write(&aec_rb, (uint8_t*)out_addr, frame_sample*2);
	if (size != frame_sample*2) {
		os_printf("cp1: the data writeten to aec_ring_buff is not a frame \r\n");
		return BK_FAIL;
	}

#if AUD_TRANS_DEBUG_ADC
		size = ring_buffer_get_fill_size(&aec_rb);
		os_printf("aec_rb: fill_size=%d \r\n", size);

		size = ring_buffer_get_free_size(&aec_rb);
		os_printf("aec_rb: free_size=%d \r\n", size);
#endif

	/* send msg to encoder to encoding data */
	msg.op = AUDIO_CP1_ENCODER;
	ret = audio_send_msg(msg);
	if (ret != kNoErr) {
		os_printf("cp1: send msg: %d fails \r\n", msg.op);
		return BK_FAIL;
	}
	return ret;
}

static bk_err_t audio_encoder_process(void)
{
	//bk_err_t ret = BK_OK;
	uint32_t size = 0;
	uint32_t i = 0;
	int16_t pcm_data[160] = {0};
	unsigned char law_data[160] = {0};
	audio_mailbox_msg_t msg;

#if AUD_DEBUG
	os_printf("[audio_encoder_process] \r\n");
#endif

	/* get data from aec_ring_buff */
	size = ring_buffer_read(&aec_rb, (uint8_t *)pcm_data, frame_sample*2);
	if (size != frame_sample*2) {
		os_printf("cp1: the data readed from aec_ring_buff is not a frame \r\n");
		return BK_FAIL;
	}

#if AUD_TRANS_DEBUG_ADC
		size = ring_buffer_get_fill_size(&aec_rb);
		os_printf("aec_rb: fill_size=%d \r\n", size);

		size = ring_buffer_get_free_size(&aec_rb);
		os_printf("aec_rb: free_size=%d \r\n", size);
#endif

	/* G711A encoding pcm data to a-law data*/
	for (i=0; i<frame_sample; i++) {
		law_data[i] = linear2alaw(pcm_data[i]);
	}

	/* save the data after G711A processed to encoder_ring_buffer */
	//size = ring_buffer_write(&encoder_rb, law_data, frame_sample);
	size = ring_buffer_write(&encoder_rb, (uint8_t*)law_data, frame_sample);
	if (size != frame_sample) {
		os_printf("cp1: the data writeten to encoder_ring_buff is not a frame \r\n");
		return BK_FAIL;
	}

#if AUD_TRANS_DEBUG_ADC
		size = ring_buffer_get_fill_size(&encoder_rb);
		os_printf("encoder_rb: fill_size=%d \r\n", size);

		size = ring_buffer_get_free_size(&encoder_rb);
		os_printf("encoder_rb: free_size=%d \r\n", size);
#endif

	/* notify cpu0 to read encoder data by mailbox msg, and ignore the ack */
	msg.mb_cmd = AUD_MB_READ_ADC_DATA_REQ_CMD;
	msg.param1 = 0;
	msg.param2 = 0;
	msg.param3 = 0;
	return aud_mailbox_send_msg(&msg);
}

static bk_err_t audio_decoder_process(void)
{
	//bk_err_t ret = BK_OK;
	uint32_t size = 0;
	uint32_t fill_size = 0;
	uint32_t i = 0;
	audio_mailbox_msg_t mb_msg;
	//uint16_t pcm_data[AUD_DECD_FRAME_SAMP_SIZE] = {0};
	int16_t pcm_data[AUD_DECD_FRAME_SAMP_SIZE*2] = {0};
	unsigned char law_data[AUD_DECD_FRAME_SAMP_SIZE] = {0};

#if AUD_DEBUG
	os_printf("[audio_decoder_process] \r\n");
#endif

	fill_size = ring_buffer_get_fill_size(&decoder_rb);
	if (fill_size >= AUD_DECD_FRAME_SAMP_SIZE) {
		/* get data from decoder_ring_buff */
		size = ring_buffer_read(&decoder_rb, (uint8_t*)law_data, AUD_DECD_FRAME_SAMP_SIZE);
		if (size != AUD_DECD_FRAME_SAMP_SIZE) {
			os_printf("cp1: the data readed from decoder_ring_buff is not a frame \r\n");
			return BK_FAIL;
		}
#if AUD_TRANS_DEBUG_DAC
		size = ring_buffer_get_fill_size(&speaker_rb);
		os_printf("speaker_rb: fill_size=%d \r\n", size);

		size = ring_buffer_get_free_size(&speaker_rb);
		os_printf("speaker_rb: free_size=%d \r\n", size);
#endif

		/* G711A decoding a-law data to pcm data*/
		for (i=0; i<AUD_DECD_FRAME_SAMP_SIZE; i++) {
			//pcm_data[i] = linear2alaw(law_data[i]);
			pcm_data[2*i] = alaw2linear(law_data[i]);
			pcm_data[2*i + 1] = pcm_data[2*i];
		}

		/* save the data after G711A processed to encoder_ring_buffer */
		//size = ring_buffer_write(&speaker_rb, (uint8_t *)pcm_data, AUD_DECD_FRAME_SAMP_SIZE*2);
		//if (size != AUD_DECD_FRAME_SAMP_SIZE*2) {
		size = ring_buffer_write(&speaker_rb, (uint8_t *)pcm_data, AUD_DECD_FRAME_SAMP_SIZE*2*2);
		if (size != AUD_DECD_FRAME_SAMP_SIZE*2*2) {
			os_printf("cp1: the data writeten to speaker_ring_buff is not a frame, size=%d \r\n", size);
			return BK_FAIL;
		}
#if AUD_TRANS_DEBUG_DAC
		i =0;
		i = ring_buffer_get_fill_size(&speaker_rb);
		os_printf("cp1: speaker_rb: fill_size=%d \r\n", i);
		i = ring_buffer_get_free_size(&speaker_rb);
		os_printf("cp1: speaker_rb: free_size=%d \r\n", i);
#endif
	}else {
		/* send mailbox msg to cpu0, request cpu0 write dac data*/
		mb_msg.mb_cmd = AUD_MB_WRITE_DAC_DATA_REQ_CMD;
		mb_msg.param1 = 0;
		mb_msg.param2 = 0;
		mb_msg.param3 = 0;
		aud_mailbox_send_msg(&mb_msg);
	}

	return BK_OK;
}

static bk_err_t audio_read_req_process(void)
{
	uint32_t size = 0;
	audio_mailbox_msg_t msg;

#if AUD_DEBUG
	os_printf("[audio_read_req_process] \r\n");
#endif

	/* get data from encoder_ring_buff */
	size = ring_buffer_read(&encoder_rb, (uint8_t*)read_buffer, read_buffer_size);
	if (size != read_buffer_size) {
		os_printf("cp1: the data size readed from encoder_ring_buff is not read_buffer_size:%x \r\n", read_buffer_size);
		return BK_FAIL;
	}

#if AUD_TRANS_DEBUG_ADC
		size = ring_buffer_get_fill_size(&encoder_rb);
		os_printf("encoder_rb: fill_size=%d \r\n", size);

		size = ring_buffer_get_free_size(&encoder_rb);
		os_printf("encoder_rb: free_size=%d \r\n", size);
#endif

	/* read data complete, and send mailbox msg to cpu0 */
	msg.mb_cmd = AUD_MB_READ_ADC_DATA_DONE_CMD;
	msg.param1 = (uint32_t)read_buffer;
	msg.param2 = read_buffer_size;
	msg.param3 = 0;
	return aud_mailbox_send_msg(&msg);
}

static bk_err_t audio_write_req_process(void)
{
	bk_err_t ret = BK_OK;
	uint32_t size = 0;
	audio_mailbox_msg_t mb_msg;
	audio_cp1_msg_t msg;

#if AUD_DEBUG
	os_printf("[audio_write_req_process] \r\n");
#endif

	/* write data from cpu0 to decoder_ring_buff */
	size = ring_buffer_write(&decoder_rb, (uint8_t*)write_buffer, write_buffer_size);
	if (size != write_buffer_size) {
		os_printf("cp1: the data size written to encoder_ring_buff is not write_buffer_size:%x \r\n", write_buffer_size);
		return BK_FAIL;
	}
#if AUD_TRANS_DEBUG_DAC

	size = ring_buffer_get_fill_size(&decoder_rb);
	os_printf("audio_write_req_process: fill_size=%d \r\n", size);

	size = ring_buffer_get_free_size(&decoder_rb);
	os_printf("audio_write_req_process: free_size=%d \r\n", size);
#endif

	/* write data to decoder_ring_buff complete, and send mailbox msg to cpu0 */
	mb_msg.mb_cmd = AUD_MB_WRITE_DAC_DATA_DONE_CMD;
	mb_msg.param1 = (uint32_t)write_buffer;
	mb_msg.param2 = write_buffer_size;
	mb_msg.param3 = 0;
	ret = aud_mailbox_send_msg(&mb_msg);
	if (ret != BK_OK) {
		os_printf("cp1: send mailbox msg: AUD_MB_WRITE_DAC_DATA_DONE_CMD fail \r\n");
		return BK_FAIL;
	}

	/* send msg to decoder data */
	msg.op = AUDIO_CP1_DECODER;
	ret = audio_send_msg(msg);
	if (ret != kNoErr) {
		os_printf("cp1: send msg: %d fails \r\n", msg.op);
		return BK_FAIL;
	}

	return BK_OK;
}

static void audio_start_transfer_process(void)
{
	bk_err_t ret = BK_OK;

	/*  -------------------------step9: enable audio and dma to start audio trabsfer -------------------------------- */
	/* start DMA */
	ret = bk_dma_start(adc_dma_id);
	if (ret != BK_OK) {
		os_printf("cp1: start adc dma fail \r\n");
		return;
	}
	ret = bk_dma_start(dac_dma_id);
	if (ret != BK_OK) {
		os_printf("cp1: start dac dma fail \r\n");
		return;
	}

	/* enable adc */
	bk_aud_start_adc();

	/* enable dac */
	bk_aud_start_dac();
	os_printf("enable audio and dma to start audio transfer complete \r\n");

}

static bk_err_t audio_stop_transfer(void)
{
	audio_mailbox_msg_t msg;

	msg.mb_cmd = AUD_MB_STOP_TRANSFER_CMD;
	msg.param1 = 0;
	msg.param2 = 0;
	msg.param3 = 0;
	return aud_mailbox_send_msg(&msg);
}


static void audio_cp1_transfer_main(beken_thread_arg_t param_data)
{
	uint32_t psram_mode = 0x00054043;
	bk_err_t ret = BK_OK;
	audio_setup_t *audio_setup = NULL;

	/*  -------------------------step1: config mailbox -------------------------------- */
	/* init maibox */
	aud_mailbox_init(audio_cp1_mailbox_rx_isr, audio_cp1_mailbox_tx_isr, audio_cp1_mailbox_tx_cmpl_isr);
	os_printf("cp1: step1: config mailbox complete \r\n");

	/*  -------------------------step2: init audio and config ADC and DAC -------------------------------- */
	audio_setup = (audio_setup_t *)(int)param_data;
	ret = audio_adc_config(audio_setup->samp_rate);
	if (ret != BK_OK) {
		os_printf("cp1: audio adc init fail \r\n");
		return;
	}

	ret = audio_dac_config(audio_setup->samp_rate);
	if (ret != BK_OK) {
		os_printf("cp1: audio dac init fail \r\n");
		return;
	}
	os_printf("cp1: step2: init audio and config ADC and DAC complete \r\n");

	/*  -------------------------step3: init AEC and malloc two ring buffers -------------------------------- */
	/* init aec and config aec */
	audio_aec_config(audio_setup->samp_rate);
	os_printf("frame_sample: %d \r\n", frame_sample);
	frame_sample = 160;

	/* malloc mic and ref ring buffer (AEC_Frame_Size=frame_sample*2) */
	os_printf("mic_ref_ring_buff: %d \r\n", frame_sample*2*2*2);
	mic_ref_ring_buff = os_malloc(frame_sample*2*2*2);
	if (mic_ref_ring_buff == NULL) {
		os_printf("cp1: malloc mic and ref ring buffer fail \r\n");
		goto audio_transfer_exit;
	}

	/* malloc aec ring buffer to save mic data has been aec processed */
	aec_ring_buff = os_malloc(frame_sample*2*2);
	if (aec_ring_buff == NULL) {
		os_printf("cp1: malloc aec ring buffer fail \r\n");
		goto audio_transfer_exit;
	}
	os_printf("cp1: step3: init AEC and malloc two ring buffers complete \r\n");

	/*  -------------------step4: init and config DMA to carry mic and ref data ----------------------------- */
	/* init dma driver */
	ret = bk_dma_driver_init();
	if (ret != BK_OK) {
		os_printf("cp1: dma driver init failed\r\n");
		return;
	}

	/* allocate free DMA channel */
	adc_dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
	if ((adc_dma_id < DMA_ID_0) || (adc_dma_id >= DMA_ID_MAX)) {
		os_printf("cp1: malloc dma fail \r\n");
		return;
	}
	os_printf("adc_dma_id: %d \r\n", adc_dma_id);

	/* config audio adc dma to carry mic and ref data to "mic_ref_ring_buff" */
	ret = audio_adc_dma_config(adc_dma_id, mic_ref_ring_buff, frame_sample*2*2*2, frame_sample*2*2);
	if (ret != BK_OK) {
		os_printf("cp1: config audio adc dma fail \r\n");
		return;
	}
	os_printf("cp1: step4: init and config DMA to carry mic and ref data complete \r\n");

	/*  -------------------------step5: init and config G711 encoder, malloc psram memory -------------------------------- */
	/* init and config G711 encoder */

	/* init psram */
	ret = bk_psram_init(psram_mode);
	if (ret != BK_OK) {
		os_printf("cp1: psram init error \r\n");
		return;
	}

	/* malloc encoder ring buffer (size: 2*AEC_Frame_Size) */
	/* 16K sample rate: PSRAM_AUD_16K_ENCD_RING_BUFF_SIZE
	   8K sample rate: PSRAM_AUD_8K_ENCD_RING_BUFF_SIZE
	*/
	os_printf("cp1: step5: init and config G711 encoder, malloc psram memory complete \r\n");

	/*  -------------------------step6: init and config G711 decoder, malloc psram memory and speaker ring buffer -------------------------------- */
	/* init and config G711 decoder */

	/* malloc decoder ring buffer (size: 2*PSRAM_AUD_DECD_RING_BUFF_SIZE) */

	/* malloc speaker ring buffer (size: 2*PSRAM_AUD_DECD_RING_BUFF_SIZE) */
	//speaker_ring_buff = os_malloc(PSRAM_AUD_DECD_RING_BUFF_SIZE*2);
	speaker_ring_buff = os_malloc(PSRAM_AUD_DECD_RING_BUFF_SIZE*2*2);    //单声道扩充为双声道播放
	if (speaker_ring_buff == NULL) {
		os_printf("cp1: malloc speaker ring buffer fail \r\n");
	}
	os_printf("cp1: step6: init and config G711 decoder, malloc psram memory and speaker ring buffer:%p complete \r\n", speaker_ring_buff);

	/*  -------------------step7: init and config DMA to carry dac data ----------------------------- */
	/* init dma driver */
	ret = bk_dma_driver_init();
	if (ret != BK_OK) {
		os_printf("cp1: dma driver init failed\r\n");
		return;
	}

	/* allocate free DMA channel */
	dac_dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
	if ((dac_dma_id < DMA_ID_0) || (dac_dma_id >= DMA_ID_MAX)) {
		os_printf("cp1: malloc dma fail \r\n");
		return;
	}
	os_printf("dac_dma_id: %d \r\n", dac_dma_id);

	/* config audio dac dma to carry dac data to "speaker_ring_buff" */
	//ret = audio_dac_dma_config(dac_dma_id, speaker_ring_buff, PSRAM_AUD_DECD_RING_BUFF_SIZE*2, PSRAM_AUD_DECD_RING_BUFF_SIZE);
	ret = audio_dac_dma_config(dac_dma_id, speaker_ring_buff, PSRAM_AUD_DECD_RING_BUFF_SIZE*2*2, PSRAM_AUD_DECD_RING_BUFF_SIZE*2);
	if (ret != BK_OK) {
		os_printf("cp1: config audio dac dma fail \r\n");
		return;
	}
	os_printf("cp1: step7: init and config DMA to carry dac data complete \r\n");

	/*  -------------------------step8: init all audio ring buffers -------------------------------- */
	/* init mic_ref_ring_buff */
	ring_buffer_init(&mic_ref_rb, (uint8_t*)mic_ref_ring_buff, frame_sample*2*2*2, adc_dma_id, RB_DMA_TYPE_WRITE);

	/* init aec_ring_buff */
	ring_buffer_init(&aec_rb, (uint8_t*)aec_ring_buff, frame_sample*2*2, DMA_ID_MAX, RB_DMA_TYPE_NULL);

	/* init encoder_ring_buff */
	ring_buffer_init(&encoder_rb, (uint8_t*)PSRAM_AUD_ENCD_RING_BUFF_BASE, frame_sample*2, DMA_ID_MAX, RB_DMA_TYPE_NULL);

	/* init decoder_ring_buff */
	ring_buffer_init(&decoder_rb, (uint8_t*)PSRAM_AUD_DECD_RING_BUFF_BASE, PSRAM_AUD_DECD_RING_BUFF_SIZE, DMA_ID_MAX, RB_DMA_TYPE_NULL);

	/* init speaker_ref_ring_buff */
	//ring_buffer_init(&speaker_rb, (uint8_t*)speaker_ring_buff, PSRAM_AUD_DECD_RING_BUFF_SIZE*2, dac_dma_id, RB_DMA_TYPE_READ);
	ring_buffer_init(&speaker_rb, (uint8_t*)speaker_ring_buff, PSRAM_AUD_DECD_RING_BUFF_SIZE*2*2, dac_dma_id, RB_DMA_TYPE_READ);
	os_printf("cp1: step8: init all audio ring buffers complete \r\n");

	temp_mic_ref_addr = (int16_t *)os_malloc(AUD_AEC_8K_FRAME_SAMP_SIZE*2);
	if (temp_mic_ref_addr == NULL) {
		os_printf("cp1: malloc temp_mic_ref_addr fail \r\n");
	}
	os_printf("cp1: step9: malloc temp_mic_ref_addr used in aec complete \r\n");


	while(1) {
		audio_cp1_msg_t msg;
		ret = rtos_pop_from_queue(&aud_int_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			switch (msg.op) {
				case AUDIO_CP1_IDLE:
					break;

				case AUDIO_CP1_START:
					audio_start_transfer_process();
					break;

				case AUDIO_CP1_AEC:
					audio_aec_process();
					break;

				case AUDIO_CP1_ENCODER:
					audio_encoder_process();
					break;

				case AUDIO_CP1_DECODER:
					audio_decoder_process();
					break;

				case AUDIO_CP1_READ_REQ:
					audio_read_req_process();
					break;

				case AUDIO_CP1_WRITE_REQ:
					audio_write_req_process();
					break;


				case AUDIO_CP1_MAILBOX:
					//audio_mailbox_msg_handle();
					break;

				case AUDIO_CP1_EXIT:
					audio_stop_transfer();
					os_printf("cp1: send stop msg \r\n");
					goto audio_transfer_exit;
					break;

				default:
					break;
			}
		}
	}

audio_transfer_exit:
	/* disable audio adc and dac */
	bk_aud_stop_adc();
	bk_aud_adc_deinit();
	bk_aud_stop_dac();
	bk_aud_dac_deinit();
	bk_aud_driver_deinit();

	/* stop dma */
	bk_dma_stop(adc_dma_id);
	bk_dma_stop(dac_dma_id);
	bk_dma_deinit(dac_dma_id);
	bk_dma_deinit(adc_dma_id);
	bk_dma_free(DMA_DEV_AUDIO, adc_dma_id);
	bk_dma_free(DMA_DEV_AUDIO, dac_dma_id);

	/* disable AEC */
	os_free(aec);

	/* free audio ring buffer */
	ring_buffer_clear(&mic_ref_rb);
	ring_buffer_clear(&aec_rb);
	ring_buffer_clear(&encoder_rb);
	ring_buffer_clear(&decoder_rb);
	ring_buffer_clear(&speaker_rb);
	os_free(mic_ref_ring_buff);
	os_free(aec_ring_buff);
	os_free(speaker_ring_buff);
	os_free(temp_mic_ref_addr);

	/* disable mailbox */
	aud_mailbox_deinit();

	/* delate msg queue */
	ret = rtos_deinit_queue(&aud_int_msg_que);
	if (ret != kNoErr) {
		os_printf("cp1: delate message queue fail \r\n");
		//return BK_FAIL;
	}
	aud_int_msg_que = NULL;
	os_printf("cp1: delate message queue complete \r\n");

	/* delate task */
	rtos_delete_thread(NULL);
	audio_thread_hdl = NULL;
	os_printf("cp1: delate task \r\n");

}

audio_setup_t audio_transfer_setup_bak = {0};

bk_err_t bk_audio_cp1_transfer_init(audio_setup_t *setup_cfg)
{
	bk_err_t ret = BK_OK;
	if ((!audio_thread_hdl) && (!aud_int_msg_que)) {
		os_printf("cp1: start audio_transfer test \r\n");
		os_memcpy(&audio_transfer_setup_bak, setup_cfg, sizeof(audio_setup_t));

		ret = rtos_init_queue(&aud_int_msg_que,
							  "audio_internal_queue",
							  sizeof(audio_cp1_msg_t),
							  TU_QITEM_COUNT);
		if (ret != kNoErr) {
			os_printf("cp1: ceate audio internal message queue in cpu1 failed \r\n");
			return BK_FAIL;
		}

		os_printf("cp1: ceate audio internal message queue in cpu1 complete \r\n");
		delay(100);

		//creat audio transfer task
		ret = rtos_create_thread(&audio_thread_hdl,
							 BEKEN_DEFAULT_WORKER_PRIORITY,
							 "audio_intf",
							 (beken_thread_function_t)audio_cp1_transfer_main,
							 4096,
							 (beken_thread_arg_t)&audio_transfer_setup_bak);
		if (ret != kNoErr) {
			os_printf("cp1: create audio transfer task fail \r\n");
			rtos_deinit_queue(&aud_int_msg_que);
			aud_int_msg_que = NULL;
			audio_thread_hdl = NULL;
		}
		os_printf("cp1: create audio transfer task complete \r\n");

		return kNoErr;
	} else
		return kInProgressErr;
}

