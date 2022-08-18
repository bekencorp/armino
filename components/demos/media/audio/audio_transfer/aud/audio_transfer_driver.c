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
#if CONFIG_AUD_TRAS_MODE_CPU1
#include "audio_mailbox.h"
#include "media_common.h"
#endif
#include <driver/psram.h>
#include <modules/aec.h>
#include <modules/audio_ring_buff.h>
#include <modules/g711.h>
#include "audio_transfer_driver.h"
#include "audio_transfer_private.h"

#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
#include <driver/timer.h>
#endif
#if CONFIG_DOORBELL_DEMO1
#include <driver/gpio.h>
#endif

#define  AUD_DEBUG  0

#define AUD_TRAS_DRV_TAG "aud_trs_drv"

#define LOGI(...) BK_LOGI(AUD_TRAS_DRV_TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(AUD_TRAS_DRV_TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(AUD_TRAS_DRV_TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(AUD_TRAS_DRV_TAG, ##__VA_ARGS__)

/*
#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
#define PSRAM_AUDIO_AEC_SIN_DUMP_DEBUG_BASE      PSRAM_AUD_DECD_RING_BUFF_BASE + 1500
#define PSRAM_AUDIO_AEC_REF_DUMP_DEBUG_BASE      PSRAM_AUDIO_AEC_SIN_DUMP_DEBUG_BASE + 600
#endif
*/

#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
#define AUD_LOST_DEBUG_INTERVAL (1000 * 2)
#endif

#define TU_QITEM_COUNT      (60)
static beken_thread_t  aud_trs_drv_thread_hdl = NULL;
static beken_queue_t aud_trs_drv_int_msg_que = NULL;
audio_transfer_context_t audio_transfer_context;

#if CONFIG_DOORBELL_DEMO1
static bool aud_pa_en = false;
extern int delay_ms(INT32 ms_count);
#endif

#if CONFIG_AUD_TRAS_MODE_CPU1
#define AUD_TRS_MAILBOX_CHECK_CONTROL    0
#if AUD_TRS_MAILBOX_CHECK_CONTROL
static bool aud_trs_drv_mb_busy_status = false;
#endif
#endif //CONFIG_AUD_TRAS_MODE_CPU1

/* extern api */
bk_err_t bk_aud_tras_drv_deinit(void);

bk_err_t bk_aud_tras_drv_start_work(void)
{
	bk_err_t ret;
	audio_tras_drv_msg_t msg;
	os_printf("audio send msg: AUD_TRAS_DRV_START \r\n");

	msg.op = AUD_TRAS_DRV_START;
	if (aud_trs_drv_int_msg_que) {
		ret = rtos_push_to_queue_front(&aud_trs_drv_int_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			os_printf("audio send msg: AUD_TRAS_DRV_START fail \r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

#if CONFIG_AUD_TRAS_MODE_CPU1
static void audio_transfer_tx_done_process(uint32_t *buff_addr, uint32_t leng)
{
	tx_context_t temp_tx_context = audio_transfer_context.tx_context;

	if ((temp_tx_context.ping.buff_addr == buff_addr) && (temp_tx_context.buff_length == leng))
		temp_tx_context.ping.busy_status = false;
	else if ((temp_tx_context.pang.buff_addr == buff_addr) && (temp_tx_context.buff_length == leng))
		temp_tx_context.pang.busy_status = false;
	else
		os_printf("tx buff addr error \r\n");
}

static bk_err_t audio_send_aud_mailbox_msg(audio_tras_mb_cmd_t mb_cmd, uint32_t param1, uint32_t param2, uint32_t param3)
{
	bk_err_t ret = BK_OK;
	audio_tras_mb_msg_t mb_msg;

	mb_msg.mb_cmd = mb_cmd;
	mb_msg.param1 = param1;
	mb_msg.param2 = param2;
	mb_msg.param3 = param3;

#if AUD_TRS_MAILBOX_CHECK_CONTROL
	/* wait send audio_mailbox_msg complete */
	while(aud_trs_drv_mb_busy_status)
		;

	aud_trs_drv_mb_busy_status = true;

	ret = aud_tras_mb_send_msg(&mb_msg);
	if (ret != BK_OK)
		aud_trs_drv_mb_busy_status = false;
#else
	ret = aud_tras_mb_send_msg(&mb_msg);
#endif

	return ret;
}

static void audio_mailbox_rx_isr(aud_mb_t *aud_mb, mb_chnl_cmd_t *cmd_buf)
{
	/* check mailbox msg and send msg to task */
	switch(cmd_buf->hdr.cmd) {
		case AUD_TRAS_MB_CMD_START_TRANSFER:
			bk_aud_tras_drv_start_work();
			break;

		case AUD_TRAS_MB_CMD_TX_DONE:
			audio_transfer_tx_done_process((uint32_t *)cmd_buf->param1, cmd_buf->param2);
			break;

		case AUD_TRAS_MB_CMD_STOP_TRANSFER:
			bk_aud_tras_drv_deinit();
			break;

		default:
			break;
	}
}

static void audio_mailbox_tx_isr(aud_mb_t *aud_mb)
{
	//os_printf("enter cp1_mailbox_tx_isr \r\n");
}

static void audio_mailbox_tx_cmpl_isr(aud_mb_t *aud_mb, mb_chnl_ack_t *cmd_buf)
{
	//os_printf("enter cp1_mailbox_tx_cmpl_isr \r\n");
	/* clear audio mailbox busy status */
#if AUD_TRS_MAILBOX_CHECK_CONTROL
	aud_trs_drv_mb_busy_status = false;
#endif
}

#if 0
static void audio_transfer_mailbox_init(void)
{
	switch (audio_transfer_context.mode) {
		case AUD_TRAS_DRV_MODE_CPU0:
			break;

		case AUD_TRAS_DRV_MODE_CPU1:
			/* init audio maibox channel */
			aud_tras_mb_init(audio_mailbox_rx_isr, audio_mailbox_tx_isr, audio_mailbox_tx_cmpl_isr);
			os_printf("step1: config audio mailbox channel complete \r\n");
			break;

		default:
			break;
	}
}

static void audio_transfer_mailbox_deinit(void)
{
	switch (audio_transfer_context.mode) {
		case AUD_TRAS_DRV_MODE_CPU0:
			break;

		case AUD_TRAS_DRV_MODE_CPU1:
			/* deinit audio maibox channel */
			aud_tras_mb_deinit();
			os_printf("close audio mailbox channel complete \r\n");
			break;

		default:
			break;
	}
}
#endif

#endif	//CONFIG_AUD_TRAS_MODE_CPU1

bk_err_t audio_transfer_send_msg(audio_tras_drv_op_t op, audio_tras_ctrl_t * param)
{
	bk_err_t ret;
	audio_tras_drv_msg_t msg;

	msg.op = op;
	if (aud_trs_drv_int_msg_que) {
		ret = rtos_push_to_queue(&aud_trs_drv_int_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			os_printf("audio_transfer_send_msg failed \r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

static bk_err_t audio_transfer_adc_config(void)
{
	bk_err_t ret = BK_OK;
	aud_adc_config_t *adc_config = audio_transfer_context.adc_config;

	/* init audio driver and config adc */
	ret = bk_aud_driver_init();
	if (ret != BK_OK) {
		os_printf("init audio driver fail \r\n");
		goto aud_adc_exit;
	}

	ret = bk_aud_adc_init(AUD_ADC_WORK_MODE_ADC, adc_config, NULL);
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

static bk_err_t audio_transfer_dac_config(void)
{
	bk_err_t ret = BK_OK;
	aud_dac_config_t *dac_config = audio_transfer_context.dac_config;

	/* init audio driver and config dac */
	ret = bk_aud_driver_init();
	if (ret != BK_OK) {
		os_printf("init audio driver fail \r\n");
		goto aud_dac_exit;
	}

	ret = bk_aud_dac_init(dac_config);
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

static void audio_transfer_aec_config(void)
{
	uint32_t aec_context_size = 0;
	uint32_t val = 0;
	aec_context_t *temp_aec_context = audio_transfer_context.aec_context;

	/* init aec context */
	aec_context_size = aec_size();
	temp_aec_context->aec = (AECContext*)os_malloc(aec_context_size);
	if (temp_aec_context->aec == NULL) {
		os_printf("malloc aec fail, aec_context_size: %d \r\n", aec_context_size);
	}

	/* config sample rate, default is 8K */
	switch (temp_aec_context->samp_rate) {
		case AEC_SAMP_RATE_8K:
			aec_init(temp_aec_context->aec, 8000);
			break;

		case AEC_SAMP_RATE_16K:
			aec_init(temp_aec_context->aec, 16000);
			break;

		default:
			break;
	}

	/* 获取处理帧长，16000采样率320点(640字节)，8000采样率160点(320字节)  (对应20毫秒数据) */
	aec_ctrl(temp_aec_context->aec, AEC_CTRL_CMD_GET_FRAME_SAMPLE, (uint32_t)(&(temp_aec_context->samp_rate_points)));

	/* 获取结构体内部可以复用的ram作为每帧tx,rx,out数据的临时buffer; ram很宽裕的话也可以在外部单独申请获取 */
	aec_ctrl(temp_aec_context->aec, AEC_CTRL_CMD_GET_TX_BUF, (uint32_t)(&val)); temp_aec_context->mic_addr = (int16_t*)val;
	aec_ctrl(temp_aec_context->aec, AEC_CTRL_CMD_GET_RX_BUF, (uint32_t)(&val)); temp_aec_context->ref_addr = (int16_t*)val;
	aec_ctrl(temp_aec_context->aec, AEC_CTRL_CMD_GET_OUT_BUF,(uint32_t)(&val)); temp_aec_context->out_addr = (int16_t*)val;

	/* 以下是参数调节示例,aec_init中都已经有默认值,可以直接先用默认值 */
	//aec_ctrl(temp_aec_context->aec, AEC_CTRL_CMD_SET_FLAGS, 0x1f);															//库内各模块开关; aec_init内默认赋值0x1f;
	aec_ctrl(temp_aec_context->aec, AEC_CTRL_CMD_SET_FLAGS, temp_aec_context->aec_config->init_flags);							//库内各模块开关; aec_init内默认赋值0x1f;

	/* 回声消除相关 */
	aec_ctrl(temp_aec_context->aec, AEC_CTRL_CMD_SET_MIC_DELAY, temp_aec_context->aec_config->mic_delay);						//设置参考信号延迟(采样点数，需要dump数据观察)
	aec_ctrl(temp_aec_context->aec, AEC_CTRL_CMD_SET_EC_DEPTH, temp_aec_context->aec_config->ec_depth);							//建议取值范围1~50; 后面几个参数建议先用aec_init内的默认值，具体需要根据实际情况调试; 总得来说回声越大需要调的越大
	aec_ctrl(temp_aec_context->aec, AEC_CTRL_CMD_SET_TxRxThr, temp_aec_context->aec_config->TxRxThr);							//建议取值范围10~64
	aec_ctrl(temp_aec_context->aec, AEC_CTRL_CMD_SET_TxRxFlr, temp_aec_context->aec_config->TxRxFlr);							//建议取值范围1~10
//	aec_ctrl(temp_aec_context->aec, AEC_CTRL_CMD_SET_TxRxThr, 30);																//建议取值范围10~64
//	aec_ctrl(temp_aec_context->aec, AEC_CTRL_CMD_SET_TxRxFlr, 6);																//建议取值范围1~10

	aec_ctrl(temp_aec_context->aec, AEC_CTRL_CMD_SET_REF_SCALE, temp_aec_context->aec_config->ref_scale);						//取值0,1,2；rx数据如果幅值太小的话适当放大
	aec_ctrl(temp_aec_context->aec, AEC_CTRL_CMD_SET_VOL, temp_aec_context->aec_config->voice_vol);								//通话过程中如果需要经常调节喇叭音量就设置下当前音量等级
	/* 降噪相关 */
	aec_ctrl(temp_aec_context->aec, AEC_CTRL_CMD_SET_NS_LEVEL, temp_aec_context->aec_config->ns_level);							//建议取值范围1~8；值越小底噪越小
	aec_ctrl(temp_aec_context->aec, AEC_CTRL_CMD_SET_NS_PARA, temp_aec_context->aec_config->ns_para);							//只能取值0,1,2; 降噪由弱到强，建议默认值
	/* drc(输出音量相关) */
	aec_ctrl(temp_aec_context->aec, AEC_CTRL_CMD_SET_DRC, temp_aec_context->aec_config->drc);									//建议取值范围0x10~0x1f;   越大输出声音越大

}

static bk_err_t audio_transfer_aec_buff_config(aec_context_t *aec_context)
{
	bk_err_t ret = BK_OK;
	uint16_t samp_rate_points = aec_context->samp_rate_points;

	/* malloc aec ref ring buffer to save ref data */
	os_printf("ref_ring_buff size: %d \r\n", samp_rate_points*2*2);
	aec_context->aec_ref_ring_buff = (int16_t *)os_malloc(samp_rate_points*2*2);
	if (aec_context->aec_ref_ring_buff == NULL) {
		os_printf("malloc ref ring buffer fail \r\n");
		return BK_FAIL;
	}

	/* malloc aec out ring buffer to save mic data has been aec processed */
	aec_context->aec_out_ring_buff = (int16_t *)os_malloc(samp_rate_points*2*2);
	if (aec_context->aec_out_ring_buff == NULL) {
		os_printf("malloc aec out ring buffer fail \r\n");
		return BK_FAIL;
	}

	/* init ref_ring_buff */
	ring_buffer_init(&(aec_context->ref_rb), (uint8_t*)aec_context->aec_ref_ring_buff, samp_rate_points*2*2, DMA_ID_MAX, RB_DMA_TYPE_NULL);

	/* init aec_ring_buff */
	ring_buffer_init(&(aec_context->aec_rb), (uint8_t*)aec_context->aec_out_ring_buff, samp_rate_points*2*2, DMA_ID_MAX, RB_DMA_TYPE_NULL);

	return ret;
}

static void audio_transfer_aec_deconfig(void)
{
	if (audio_transfer_context.aec_enable) {
		os_free(audio_transfer_context.aec_context->aec);
		audio_transfer_context.aec_context->aec = NULL;
		os_free(audio_transfer_context.aec_context->aec_config);
		audio_transfer_context.aec_context->aec_config = NULL;
		ring_buffer_clear(&(audio_transfer_context.aec_context->ref_rb));
		ring_buffer_clear(&(audio_transfer_context.aec_context->aec_rb));
		os_free(audio_transfer_context.aec_context->aec_ref_ring_buff);
		audio_transfer_context.aec_context->aec_ref_ring_buff = NULL;
		os_free(audio_transfer_context.aec_context->aec_out_ring_buff);
		audio_transfer_context.aec_context->aec_out_ring_buff = NULL;
		os_free(audio_transfer_context.aec_context);
		audio_transfer_context.aec_context = NULL;
	} else {
		audio_transfer_context.aec_context = NULL;
	}

	audio_transfer_context.aec_enable = false;
}

/* 搬运audio adc 采集到的一帧mic和ref信号后，触发中断通知AEC处理数据 */
static void audio_transfer_adc_dma_finish_isr(void)
{
	bk_err_t ret = BK_OK;

	/* send msg to AEC to process mic and ref data */
	ret = audio_transfer_send_msg(AUD_TRAS_DRV_AEC, NULL);
	if (ret != kNoErr) {
		os_printf("send msg: AUD_TRAS_DRV_AEC fail \r\n");
	}
}

static bk_err_t audio_transfer_adc_dma_config(dma_id_t dma_id, int32_t *ring_buff_addr, uint32_t ring_buff_size, uint32_t transfer_len)
{
	bk_err_t ret = BK_OK;
	dma_config_t dma_config;
	uint32_t adc_port_addr;

	dma_config.mode = DMA_WORK_MODE_REPEAT;
	dma_config.chan_prio = 1;
	dma_config.src.dev = DMA_DEV_AUDIO;
	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_16BITS;
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
		os_printf("audio adc dma channel init fail \r\n");
		return BK_FAIL;
	}

	/* set dma transfer length */
	bk_dma_set_transfer_len(dma_id, transfer_len);

	//register isr
	bk_dma_register_isr(dma_id, NULL, (void *)audio_transfer_adc_dma_finish_isr);
	bk_dma_enable_finish_interrupt(dma_id);

	return BK_OK;
}

/* 搬运audio dac 一帧dac信号后，触发中断通知decoder处理数据 */
static void audio_transfer_dac_dma_finish_isr(void)
{
	//os_printf("enter the audio_dma_dac_finish_isr \r\n");
	bk_err_t ret = BK_OK;

	/* send msg to decoder to decoding recevied data */
	ret = audio_transfer_send_msg(AUD_TRAS_DRV_DECODER, NULL);
	if (ret != kNoErr) {
		os_printf("send msg: AUD_TRAS_DRV_DECODER fails \r\n");
	}
}

static bk_err_t audio_transfer_dac_dma_config(dma_id_t dma_id, int32_t *ring_buff_addr, uint32_t ring_buff_size, uint32_t transfer_len)
{
	bk_err_t ret = BK_OK;
	dma_config_t dma_config;
	uint32_t dac_port_addr;

	dma_config.mode = DMA_WORK_MODE_REPEAT;
	dma_config.chan_prio = 1;
	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.dst.dev = DMA_DEV_AUDIO;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
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
	bk_dma_register_isr(dma_id, NULL, (void *)audio_transfer_dac_dma_finish_isr);
	bk_dma_enable_finish_interrupt(dma_id);

	return BK_OK;
}

static bk_err_t audio_transfer_aec_process(void)
{
	bk_err_t ret = BK_OK;
	uint32_t size = 0;
	aec_context_t *aec_context_pr = audio_transfer_context.aec_context;

	/* get a fram mic data from mic_ring_buff */
	if (ring_buffer_get_fill_size(&(audio_transfer_context.mic_rb)) >= aec_context_pr->samp_rate_points*2) {
		size = ring_buffer_read(&(audio_transfer_context.mic_rb), (uint8_t*)aec_context_pr->mic_addr, aec_context_pr->samp_rate_points*2);
		if (size != aec_context_pr->samp_rate_points*2) {
			os_printf("read mic_ring_buff fail, size:%d \r\n", size);
			//return BK_FAIL;
		}
	}

	/* read ref data from ref_ring_buff */
	if (ring_buffer_get_fill_size(&(aec_context_pr->ref_rb)) >= aec_context_pr->samp_rate_points*2) {
		size = ring_buffer_read(&(aec_context_pr->ref_rb), (uint8_t*)aec_context_pr->ref_addr, aec_context_pr->samp_rate_points*2);
		if (size != aec_context_pr->samp_rate_points*2) {
			os_printf("the ref data readed from ref_ring_buff is not a frame \r\n");
			//return BK_FAIL;
			//os_memcpy(ref_addr, 0, frame_sample*2);
		}
	}
	//os_printf("ref_addr: ref_addr[0]= %02x, ref_addr[0]= %02x \r\n", ref_addr[0], ref_addr[1]);

#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
	os_memcpy((void *)audio_transfer_context.aec_dump.mic_dump_addr, aec_context_pr->mic_addr, aec_context_pr->samp_rate_points*2);
	os_memcpy((void *)audio_transfer_context.aec_dump.ref_dump_addr, aec_context_pr->ref_addr, aec_context_pr->samp_rate_points*2);
	//os_printf("memcopy complete \r\n");
#endif

	/* aec process data */
	//os_printf("ref_addr:%p, mic_addr:%p, out_addr:%p \r\n", aec_context_pr->ref_addr, aec_context_pr->mic_addr, aec_context_pr->out_addr);
	aec_proc(aec_context_pr->aec, aec_context_pr->ref_addr, aec_context_pr->mic_addr, aec_context_pr->out_addr);

#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
		os_memcpy((void *)audio_transfer_context.aec_dump.out_dump_addr, aec_context_pr->out_addr, aec_context_pr->samp_rate_points*2);
#endif

	/* save mic data after aec processed to aec_ring_buffer */
	if (ring_buffer_get_free_size(&(aec_context_pr->aec_rb)) >= aec_context_pr->samp_rate_points*2) {
		size = ring_buffer_write(&(aec_context_pr->aec_rb), (uint8_t*)aec_context_pr->out_addr, aec_context_pr->samp_rate_points*2);
		if (size != aec_context_pr->samp_rate_points*2) {
			os_printf("the data writeten to aec_ring_buff is not a frame \r\n");
			//return BK_FAIL;
		}
	}

	/* send msg to encoder to encoding data */
	ret = audio_transfer_send_msg(AUD_TRAS_DRV_ENCODER, NULL);
	if (ret != kNoErr) {
		os_printf("send msg: AUD_TRAS_DRV_ENCODER fail \r\n");
		return BK_FAIL;
	}

#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
#if CONFIG_AUD_TRAS_MODE_CPU0
	if (audio_transfer_context.aud_cb.audio_transfer_event_cb)
		audio_transfer_context.aud_cb.audio_transfer_event_cb(EVENT_AUD_TRAS_AEC_DUMP);
#endif
#endif

	return ret;
}

static bk_err_t audio_transfer_encoder_process(void)
{
	bk_err_t ret = BK_OK;
	uint32_t size = 0;
	uint32_t i = 0;
	uint16_t temp_mic_samp_rate_points = audio_transfer_context.mic_samp_rate_points;
	tx_context_t temp_tx_context;

	if (audio_transfer_context.aec_enable) {
		/* get data from aec_ring_buff */
		size = ring_buffer_read(&(audio_transfer_context.aec_context->aec_rb), (uint8_t *)audio_transfer_context.encoder_temp.pcm_data, temp_mic_samp_rate_points*2);
		if (size != temp_mic_samp_rate_points*2) {
			os_printf("the data readed from aec_ring_buff is not a frame \r\n");
			goto encoder_exit;
		}
	} else {
		/* get data from mic_ring_buff */
		size = ring_buffer_read(&(audio_transfer_context.mic_rb), (uint8_t *)audio_transfer_context.encoder_temp.pcm_data, temp_mic_samp_rate_points*2);
		if (size != temp_mic_samp_rate_points*2) {
			os_printf("the data readed from mic_ring_buff is not a frame \r\n");
			goto encoder_exit;
		}
	}

	/* G711A encoding pcm data to a-law data*/
	for (i=0; i<temp_mic_samp_rate_points; i++) {
		audio_transfer_context.encoder_temp.law_data[i] = linear2alaw(audio_transfer_context.encoder_temp.pcm_data[i]);
	}

	temp_tx_context = audio_transfer_context.tx_context;
#if CONFIG_AUD_TRAS_MODE_CPU0
	os_memcpy(temp_tx_context.ping.buff_addr, audio_transfer_context.encoder_temp.law_data, temp_mic_samp_rate_points);
	/* call sendto api of lwip UDP */
	if (audio_transfer_context.aud_cb.audio_send_mic_data != NULL) {
		size = audio_transfer_context.aud_cb.audio_send_mic_data((uint8_t *)temp_tx_context.ping.buff_addr, temp_tx_context.buff_length);
		if (size != temp_tx_context.buff_length) {
#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
			audio_transfer_context.lost_count.lost_size += temp_tx_context.buff_length;
#endif
		} else {
#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
			audio_transfer_context.lost_count.complete_size += temp_tx_context.buff_length;
#endif
		}
	}
#endif

#if CONFIG_AUD_TRAS_MODE_CPU1
	/* save the data after G711A processed to ping pang buffer of tx */
	uint32_t *temp_tx_buffer = NULL;
	//temp_tx_context = audio_transfer_context.tx_context;
	if (temp_tx_context.ping.busy_status != true) {
		os_memcpy(temp_tx_context.ping.buff_addr, audio_transfer_context.encoder_temp.law_data, temp_mic_samp_rate_points);
		temp_tx_context.ping.busy_status = true;
		if (temp_tx_context.pang.busy_status == true) {
			temp_tx_buffer = temp_tx_context.pang.buff_addr;
		} else {
			temp_tx_buffer = temp_tx_context.ping.buff_addr;
		}
	} else {
		os_memcpy(temp_tx_context.pang.buff_addr, audio_transfer_context.encoder_temp.law_data, temp_mic_samp_rate_points);
		temp_tx_context.pang.busy_status = true;
		temp_tx_buffer = temp_tx_context.ping.buff_addr;
	}

	/* send mailbox to notify cpu0 to send data by lwip of wifi */
	ret = audio_send_aud_mailbox_msg(AUD_TRAS_MB_CMD_TX_REQ, (uint32_t)temp_tx_buffer, (uint32_t)temp_tx_context.buff_length, 0);
	if (ret != BK_OK) {
		os_printf("send mailbox msg: AUD_TRAS_MB_CMD_TX_REQ fail \r\n");
	}
#endif

	return ret;

encoder_exit:

	return BK_FAIL;
}

#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
static void aud_tras_lost_count_dump(timer_id_t timer_id)
{
	uint32_t lost = audio_transfer_context.lost_count.lost_size, complete = audio_transfer_context.lost_count.complete_size;
	audio_transfer_context.lost_count.lost_size = 0;
	audio_transfer_context.lost_count.complete_size = 0;

	lost = lost / (AUD_LOST_DEBUG_INTERVAL / 1000);
	complete = complete / 1024 / (AUD_LOST_DEBUG_INTERVAL / 1000);

	os_printf("[AUD Tx] Lost: %uB/s, Complete: %uKB/s \r\n", lost, complete);
}
#endif

static bk_err_t audio_transfer_decoder_process(void)
{
	uint32_t size = 0;
	uint32_t i = 0;

#if CONFIG_DOORBELL_DEMO1
	if (aud_pa_en == false) {
		bk_gpio_disable_input(39);
		bk_gpio_enable_output(39);
		// pull up gpio39, enable audio pa vol
		bk_gpio_set_output_low(39);
		delay_ms(15);
		bk_gpio_set_output_high(39);
		aud_pa_en = true;
		os_printf("[yikang] open audio dac pa \r\n");
	}
#endif

	/* check the frame number in decoder_ring_buffer */
	if (ring_buffer_get_fill_size(audio_transfer_context.rx_context.decoder_rb) >= audio_transfer_context.speaker_samp_rate_points) {
		//os_printf("decoder process \r\n", size);
		/* get data from decoder_ring_buff */
		size = ring_buffer_read(audio_transfer_context.rx_context.decoder_rb, (uint8_t*)audio_transfer_context.decoder_temp.law_data, audio_transfer_context.speaker_samp_rate_points);
		if (size != audio_transfer_context.speaker_samp_rate_points) {
			os_printf("read decoder_ring_buff data fail \r\n");
			//return BK_FAIL;
		}
	} else {
		os_memset(audio_transfer_context.decoder_temp.law_data, 0xD5, audio_transfer_context.speaker_samp_rate_points);
	}

	/* G711A decoding a-law data to pcm data*/
	for (i=0; i<audio_transfer_context.speaker_samp_rate_points; i++) {
		audio_transfer_context.decoder_temp.pcm_data[i] = alaw2linear(audio_transfer_context.decoder_temp.law_data[i]);
	}

	if (audio_transfer_context.aec_enable) {
		/* read mic fill data size */
		uint32_t mic_fill_size = ring_buffer_get_fill_size(&(audio_transfer_context.mic_rb));
		//os_printf("mic_rb: fill_size=%d \r\n", mic_fill_size);
		uint32_t speaker_fill_size = ring_buffer_get_fill_size(&(audio_transfer_context.speaker_rb));
		//os_printf("speaker_rb: fill_size=%d \r\n", speaker_fill_size);
		uint32_t ref_fill_size = ring_buffer_get_fill_size(&(audio_transfer_context.aec_context->ref_rb));
		//os_printf("ref_rb: fill_size=%d \r\n", ref_fill_size);
		/* 设置参考信号延迟(采样点数，需要dump数据观察) */
		//os_printf("MIC_DELAY: %d \r\n", (mic_fill_size + speaker_fill_size - ref_fill_size)/2);
		if ((mic_fill_size + speaker_fill_size - ref_fill_size)/2 < 0) {
			os_printf("MIC_DELAY is error \r\n", ref_fill_size);
			aec_ctrl(audio_transfer_context.aec_context->aec, AEC_CTRL_CMD_SET_MIC_DELAY, 0);
		} else {
			aec_ctrl(audio_transfer_context.aec_context->aec, AEC_CTRL_CMD_SET_MIC_DELAY, (mic_fill_size + speaker_fill_size - ref_fill_size)/2);
		}

		if (ring_buffer_get_free_size(&(audio_transfer_context.aec_context->ref_rb)) > audio_transfer_context.aec_context->samp_rate_points*2) {
			size = ring_buffer_write(&(audio_transfer_context.aec_context->ref_rb), (uint8_t *)audio_transfer_context.decoder_temp.pcm_data, audio_transfer_context.aec_context->samp_rate_points*2);
			if (size != audio_transfer_context.aec_context->samp_rate_points*2) {
				os_printf("write data to ref_ring_buff fail, size=%d \r\n", size);
				goto decoder_exit;
			}
		}
		}

	/* save the data after G711A processed to encoder_ring_buffer */
	if (ring_buffer_get_free_size(&(audio_transfer_context.speaker_rb)) > audio_transfer_context.speaker_samp_rate_points*2) {
		size = ring_buffer_write(&(audio_transfer_context.speaker_rb), (uint8_t *)audio_transfer_context.decoder_temp.pcm_data, audio_transfer_context.speaker_samp_rate_points*2);
		if (size != audio_transfer_context.speaker_samp_rate_points*2) {
			os_printf("the data writeten to speaker_ring_buff is not a frame, size=%d \r\n", size);
			goto decoder_exit;
		}
		audio_transfer_context.rx_context.aud_trs_read_seq++;
	}

	return BK_OK;

decoder_exit:

	return BK_FAIL;
}

static void audio_start_transfer_process(void)
{
	bk_err_t ret = BK_OK;
	uint32_t size = 0;
	//uint8_t pcm_data[audio_transfer_context.mic_samp_rate_points*2]= {0};
	uint8_t *pcm_data = NULL;

	os_printf("%s \r\n", __func__);

	pcm_data = (uint8_t *)os_malloc(audio_transfer_context.mic_samp_rate_points*2);
	if (pcm_data == NULL) {
		os_printf("malloc temp pcm_data fial \r\n");
		goto audio_start_transfer_exit;
	} else {
		os_memset(pcm_data, 0x00, audio_transfer_context.mic_samp_rate_points*2);
	}

	/* start DMA */
	ret = bk_dma_start(audio_transfer_context.adc_dma_id);
	if (ret != BK_OK) {
		os_printf("start adc dma fail \r\n");
		goto audio_start_transfer_exit;
	}

	/* enable adc */
	/* wait receive data and then open adc */
	bk_aud_start_adc();

	/* enable dac */
	bk_aud_start_dac();

	ret = bk_dma_start(audio_transfer_context.dac_dma_id);
	if (ret != BK_OK) {
		os_printf("start dac dma fail \r\n");
		goto audio_start_transfer_exit;
	}
	/* write two frame data to speaker and ref ring buffer */
	size = ring_buffer_write(&(audio_transfer_context.speaker_rb), (uint8_t *)pcm_data, audio_transfer_context.mic_samp_rate_points*2);
	if (size != audio_transfer_context.mic_samp_rate_points*2) {
		os_printf("the data writeten to speaker_ring_buff error, size=%d \r\n", size);
		goto audio_start_transfer_exit;
	}

	size = ring_buffer_write(&(audio_transfer_context.speaker_rb), (uint8_t *)pcm_data, audio_transfer_context.mic_samp_rate_points*2);
	if (size != audio_transfer_context.mic_samp_rate_points*2) {
		os_printf("the data writeten to speaker_ring_buff error, size=%d \r\n", size);
		goto audio_start_transfer_exit;
	}

	/* notify cpu0 that audio transfer start */
#if CONFIG_AUD_TRAS_MODE_CPU0
//	switch (audio_transfer_context.mode) {
//		case AUD_TRAS_DRV_MODE_CPU0:
			if (audio_transfer_context.aud_cb.audio_transfer_event_cb)
				audio_transfer_context.aud_cb.audio_transfer_event_cb(EVENT_AUD_TRAS_START_CMP);
//			break;
#endif

#if CONFIG_AUD_TRAS_MODE_CPU1
//		case AUD_TRAS_DRV_MODE_CPU1:
			ret = audio_send_aud_mailbox_msg(AUD_TRAS_MB_CMD_START_TRANSFER_DONE, 0, 0, 0);
			if (ret != BK_OK) {
				os_printf("send mailbox msg: AUD_TRAS_MB_CMD_START_TRANSFER_DONE fail \r\n");
			}
//			break;
#endif

	os_free(pcm_data);
	pcm_data = NULL;
	os_printf("enable audio and dma to start audio transfer complete \r\n");

#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
	//bk_timer_driver_init();
	ret = bk_timer_start(TIMER_ID5, AUD_LOST_DEBUG_INTERVAL, aud_tras_lost_count_dump);
	if (ret != BK_OK)
		os_printf("start audio lost count timer fail \r\n");
	os_printf("start audio lost count timer complete \r\n");
#endif

	return;

audio_start_transfer_exit:
	//deinit audio transfer
	if (pcm_data)
		os_free(pcm_data);
}

static void audio_transfer_get_config(aud_tras_drv_setup_t *setup)
{
	/* get aec config */
	audio_transfer_context.aec_enable = setup->aec_enable;
	if (audio_transfer_context.aec_enable) {
		audio_transfer_context.aec_context = os_malloc(sizeof(aec_context_t));
		if (audio_transfer_context.aec_context == NULL) {
			os_printf("aec_context_t os_malloc fail \r\n");
		} else {
			audio_transfer_context.aec_context->samp_rate = setup->samp_rate;
			//audio_transfer_context.aec_context->samp_rate_points
			audio_transfer_context.aec_context->aec_config = os_malloc(sizeof(aec_config_t));
			if (audio_transfer_context.aec_context->aec_config == NULL) {
				os_printf("aec_config_t os_malloc fail \r\n");
			} else {
				audio_transfer_context.aec_context->aec_config->mic_delay = setup->aec_setup->init_flags;
				audio_transfer_context.aec_context->aec_config->mic_delay = setup->aec_setup->mic_delay;
				audio_transfer_context.aec_context->aec_config->ec_depth = setup->aec_setup->ec_depth;
				audio_transfer_context.aec_context->aec_config->ref_scale = setup->aec_setup->ref_scale;
				audio_transfer_context.aec_context->aec_config->voice_vol = setup->aec_setup->voice_vol;
				audio_transfer_context.aec_context->aec_config->mic_delay = setup->aec_setup->TxRxThr;
				audio_transfer_context.aec_context->aec_config->mic_delay = setup->aec_setup->TxRxFlr;
				audio_transfer_context.aec_context->aec_config->ns_level = setup->aec_setup->ns_level;
				audio_transfer_context.aec_context->aec_config->ns_para = setup->aec_setup->ns_para;
				audio_transfer_context.aec_context->aec_config->drc = setup->aec_setup->drc;
			}
		}
	}
	else {
		audio_transfer_context.aec_context = NULL;
	}

	/* get audio adc config */
	audio_transfer_context.adc_config = os_malloc(sizeof(aud_adc_config_t));
	if (audio_transfer_context.adc_config == NULL) {
		os_printf("adc_config os_malloc fail \r\n");
	} else {
		audio_transfer_context.adc_config->mic_config = AUD_MIC_MIC1_ENABLE;
		audio_transfer_context.adc_config->samp_rate = setup->samp_rate;
		audio_transfer_context.adc_config->adc_enable = AUD_ADC_DISABLE;
		audio_transfer_context.adc_config->line_enable = AUD_ADC_LINE_DISABLE;
		audio_transfer_context.adc_config->dtmf_enable = AUD_DTMF_DISABLE;
		audio_transfer_context.adc_config->adc_hpf2_coef_B2 = 0;
		audio_transfer_context.adc_config->adc_hpf2_bypass_enable = AUD_ADC_HPF_BYPASS_ENABLE;
		audio_transfer_context.adc_config->adc_hpf1_bypass_enable = AUD_ADC_HPF_BYPASS_ENABLE;
		audio_transfer_context.adc_config->adc_set_gain = setup->aud_setup.adc_gain;    //default: 0x2d
		audio_transfer_context.adc_config->adc_samp_edge = AUD_ADC_SAMP_EDGE_RISING;
		audio_transfer_context.adc_config->adc_hpf2_coef_B0 = 0;
		audio_transfer_context.adc_config->adc_hpf2_coef_B1 = 0;
		audio_transfer_context.adc_config->adc_hpf2_coef_A0 = 0;
		audio_transfer_context.adc_config->adc_hpf2_coef_A1 = 0;
		audio_transfer_context.adc_config->dtmf_wr_threshold = 8;
		audio_transfer_context.adc_config->adcl_wr_threshold = 8;
		audio_transfer_context.adc_config->dtmf_int_enable = AUD_DTMF_INT_DISABLE;
		audio_transfer_context.adc_config->adcl_int_enable = AUD_ADCL_INT_DISABLE;
		audio_transfer_context.adc_config->loop_adc2dac = AUD_LOOP_ADC2DAC_DISABLE;
		audio_transfer_context.adc_config->agc_noise_thrd = 101;
		audio_transfer_context.adc_config->agc_noise_high = 101;
		audio_transfer_context.adc_config->agc_noise_low = 160;
		audio_transfer_context.adc_config->agc_noise_min = 1;
		audio_transfer_context.adc_config->agc_noise_tout = 0;
		audio_transfer_context.adc_config->agc_high_dur = 3;
		audio_transfer_context.adc_config->agc_low_dur = 3;
		audio_transfer_context.adc_config->agc_min = 1;
		audio_transfer_context.adc_config->agc_max = 4;
		audio_transfer_context.adc_config->agc_ng_method = AUD_AGC_NG_METHOD_MUTE;
		audio_transfer_context.adc_config->agc_ng_enable = AUD_AGC_NG_DISABLE;
		audio_transfer_context.adc_config->agc_decay_time = AUD_AGC_DECAY_TIME_128;
		audio_transfer_context.adc_config->agc_attack_time = AUD_AGC_ATTACK_TIME_128;
		audio_transfer_context.adc_config->agc_high_thrd = 18;
		audio_transfer_context.adc_config->agc_low_thrd = 0;
		audio_transfer_context.adc_config->agc_iir_coef = AUD_AGC_IIR_COEF_1_1024;
		audio_transfer_context.adc_config->agc_enable = AUD_AGC_DISABLE;
		audio_transfer_context.adc_config->manual_pga_value = 0;
		audio_transfer_context.adc_config->manual_pga_enable = AUD_GAC_MANUAL_PGA_DISABLE;
		audio_transfer_context.adc_config->adc_fracmod_manual = AUD_ADC_TRACMOD_MANUAL_DISABLE;
		audio_transfer_context.adc_config->adc_fracmod = 0;
	}

	/* get audio dac config */
	audio_transfer_context.dac_config = os_malloc(sizeof(aud_dac_config_t));
	if (audio_transfer_context.adc_config == NULL) {
		os_printf("dac_config os_malloc fail \r\n");
	} else {
		audio_transfer_context.dac_config->dac_enable = AUD_DAC_DISABLE;
		audio_transfer_context.dac_config->samp_rate = setup->samp_rate;
		audio_transfer_context.dac_config->dac_hpf2_coef_B2 = 0x3A22;
		audio_transfer_context.dac_config->dac_hpf2_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
		audio_transfer_context.dac_config->dac_hpf1_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
		audio_transfer_context.dac_config->dac_set_gain = setup->aud_setup.dac_gain;    //default 2D  3F  15
		audio_transfer_context.dac_config->dac_clk_invert = AUD_DAC_CLK_INVERT_RISING;
		audio_transfer_context.dac_config->dac_hpf2_coef_B0 = 0x3A22;
		audio_transfer_context.dac_config->dac_hpf2_coef_B1 = 0x8BBF;
		audio_transfer_context.dac_config->dac_hpf2_coef_A1 = 0x751C;
		audio_transfer_context.dac_config->dac_hpf2_coef_A2 = 0xC9E6;
		audio_transfer_context.dac_config->dacr_rd_threshold = 0x4;
		audio_transfer_context.dac_config->dacl_rd_threshold = 0x4;
		audio_transfer_context.dac_config->dacr_int_enable = 0x0;
		audio_transfer_context.dac_config->dacl_int_enable = 0x0;
		audio_transfer_context.dac_config->dac_filt_enable = AUD_DAC_FILT_DISABLE;
		audio_transfer_context.dac_config->dac_fracmod_manual_enable = AUD_DAC_FRACMOD_MANUAL_DISABLE;
		audio_transfer_context.dac_config->dac_fracmode_value = 0x0;
	}

	/* get ring buffer config */
	//audio_transfer_context.mode = setup->aud_trs_mode;
	if (audio_transfer_context.aec_enable) {
		switch (setup->samp_rate) {
			case AUD_TRAS_SAMP_RATE_8K:
				audio_transfer_context.mic_samp_rate_points = 160;
				audio_transfer_context.speaker_samp_rate_points = 160;
				break;

			case AUD_TRAS_SAMP_RATE_16K:
				audio_transfer_context.mic_samp_rate_points = 320;
				audio_transfer_context.speaker_samp_rate_points = 320;
				break;

			default:
				break;
		}
	} else {
		audio_transfer_context.mic_samp_rate_points = setup->aud_setup.mic_samp_rate_points;
		audio_transfer_context.speaker_samp_rate_points = setup->aud_setup.speaker_samp_rate_points;
	}
	audio_transfer_context.mic_frame_number = setup->aud_setup.mic_frame_number;
	audio_transfer_context.speaker_frame_number = setup->aud_setup.speaker_frame_number;

	/* get tx and rx context config */
	audio_transfer_context.tx_context = setup->tx_context;
	audio_transfer_context.rx_context = setup->rx_context;

	/* callback config */
	audio_transfer_context.aud_cb.audio_transfer_event_cb = setup->aud_cb.audio_transfer_event_cb;
	audio_transfer_context.aud_cb.audio_send_mic_data = setup->aud_cb.audio_send_mic_data;

#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
	/* get dump config */
	audio_transfer_context.aec_dump.len = setup->aec_dump.len;
	audio_transfer_context.aec_dump.mic_dump_addr = setup->aec_dump.mic_dump_addr;
	audio_transfer_context.aec_dump.ref_dump_addr = setup->aec_dump.ref_dump_addr;
	audio_transfer_context.aec_dump.out_dump_addr = setup->aec_dump.out_dump_addr;
#endif

#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
	audio_transfer_context.lost_count.complete_size = 0;
	audio_transfer_context.lost_count.lost_size = 0;
#endif

}

static void audio_transfer_deinit_config(void)
{
#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
	bk_timer_stop(TIMER_ID5);
#endif

	/* disable audio adc and dac */
	bk_aud_stop_adc();
	bk_aud_adc_deinit();
	bk_aud_stop_dac();
	bk_aud_dac_deinit();
	bk_aud_driver_deinit();
	os_free(audio_transfer_context.adc_config);
	audio_transfer_context.adc_config = NULL;
	os_free(audio_transfer_context.dac_config);
	audio_transfer_context.dac_config = NULL;

#if CONFIG_DOORBELL_DEMO1
	aud_pa_en = false;
#endif

	/* stop dma */
	bk_dma_stop(audio_transfer_context.adc_dma_id);
	bk_dma_stop(audio_transfer_context.dac_dma_id);
	bk_dma_deinit(audio_transfer_context.adc_dma_id);
	bk_dma_deinit(audio_transfer_context.dac_dma_id);
	bk_dma_free(DMA_DEV_AUDIO, audio_transfer_context.adc_dma_id);
	bk_dma_free(DMA_DEV_AUDIO, audio_transfer_context.dac_dma_id);

	/* disable AEC */
	audio_transfer_aec_deconfig();

	/* free audio ring buffer */
	//mic deconfig
	ring_buffer_clear(&(audio_transfer_context.mic_rb));
	os_free(audio_transfer_context.mic_ring_buff);
	audio_transfer_context.mic_ring_buff = NULL;
	audio_transfer_context.mic_samp_rate_points = 0;
	audio_transfer_context.mic_frame_number = 0;
	audio_transfer_context.adc_dma_id = DMA_ID_MAX;
	//speaker deconfig
	ring_buffer_clear(&(audio_transfer_context.speaker_rb));
	os_free(audio_transfer_context.speaker_ring_buff);
	audio_transfer_context.speaker_ring_buff = NULL;
	audio_transfer_context.speaker_samp_rate_points = 0;
	audio_transfer_context.speaker_frame_number = 0;
	audio_transfer_context.dac_dma_id = DMA_ID_MAX;

	/* tx and rx deconfig */
	//tx deconfig
	audio_transfer_context.tx_context.tx_buff_status = false;
	audio_transfer_context.tx_context.buff_length = 0;
	audio_transfer_context.tx_context.ping.busy_status = false;
	audio_transfer_context.tx_context.ping.buff_addr = NULL;
	//rx deconfig
	audio_transfer_context.rx_context.rx_buff_status = false;
	audio_transfer_context.rx_context.decoder_ring_buff = NULL;
	audio_transfer_context.rx_context.decoder_rb = NULL;
	audio_transfer_context.rx_context.frame_size = 0;
	audio_transfer_context.rx_context.frame_num = 0;
	audio_transfer_context.rx_context.rx_buff_seq_tail = 0;
	audio_transfer_context.rx_context.aud_trs_read_seq = 0;
	audio_transfer_context.rx_context.fifo_frame_num = 0;

	/* notify audio transfer task to stop test */
#if CONFIG_AUD_TRAS_MODE_CPU0
	if (audio_transfer_context.aud_cb.audio_transfer_event_cb)
		audio_transfer_context.aud_cb.audio_transfer_event_cb(EVENT_AUD_TRAS_STOP_CMP);
#endif

#if CONFIG_AUD_TRAS_MODE_CPU1
	/* notify cpu0 by mailbox */
	if (audio_send_aud_mailbox_msg(AUD_TRAS_MB_CMD_STOP_TRANSFER_DONE, 0, 0, 0) != BK_OK) {
		os_printf("send mailbox msg: AUD_TRAS_MB_CMD_STOP_TRANSFER_DONE fail \r\n");
	}

	/* deinit audio maibox channel */
	aud_tras_mb_deinit();
	os_printf("close audio mailbox channel complete \r\n");
#endif

	/* callback deconfig */
	audio_transfer_context.aud_cb.audio_transfer_event_cb = NULL;
	audio_transfer_context.aud_cb.audio_send_mic_data = NULL;

	/* encoder_temp and decoder_temp deconfig*/
	os_free(audio_transfer_context.encoder_temp.law_data);
	audio_transfer_context.encoder_temp.law_data = NULL;
	os_free(audio_transfer_context.encoder_temp.pcm_data);
	audio_transfer_context.encoder_temp.pcm_data = NULL;
	os_free(audio_transfer_context.decoder_temp.law_data);
	audio_transfer_context.decoder_temp.law_data = NULL;
	os_free(audio_transfer_context.decoder_temp.pcm_data);
	audio_transfer_context.decoder_temp.pcm_data = NULL;
}

static void audio_transfer_driver_main(beken_thread_arg_t param_data)
{
	bk_err_t ret = BK_OK;
	aud_tras_drv_setup_t *aud_trs_setup = NULL;

	aud_trs_setup = (aud_tras_drv_setup_t *)(int)param_data;
	audio_transfer_get_config(aud_trs_setup);

	/*  -------------------------step1: init audio and config ADC and DAC -------------------------------- */
	/* config mailbox according audio transfer work mode */
#if CONFIG_AUD_TRAS_MODE_CPU1
	//audio_transfer_mailbox_init();
	/* init audio maibox channel */
	aud_tras_mb_init(audio_mailbox_rx_isr, audio_mailbox_tx_isr, audio_mailbox_tx_cmpl_isr);
	os_printf("step1: config audio mailbox channel complete \r\n");
#endif

	/*  -------------------------step2: init audio and config ADC and DAC -------------------------------- */
	ret = audio_transfer_adc_config();
	if (ret != BK_OK) {
		os_printf("audio adc init fail \r\n");
		goto audio_tras_drv_exit;
	}

	ret = audio_transfer_dac_config();
	if (ret != BK_OK) {
		os_printf("audio dac init fail \r\n");
		goto audio_tras_drv_exit;
	}
	os_printf("step2: init audio and config ADC and DAC complete \r\n");

	/*  -------------------------step3: init AEC and malloc two ring buffers -------------------------------- */
	/* init aec and config aec according AEC_enable*/
	if (audio_transfer_context.aec_enable) {
		audio_transfer_aec_config();
		os_printf("aec samp_rate_points: %d \r\n", audio_transfer_context.aec_context->samp_rate_points);
		ret = audio_transfer_aec_buff_config(audio_transfer_context.aec_context);
		if (ret != BK_OK)
			goto audio_tras_drv_exit;
		os_printf("step3: init AEC and malloc two ring buffers complete \r\n");
	}

	/*  -------------------step4: init and config DMA to carry mic and ref data ----------------------------- */
	/* init dma driver */
	ret = bk_dma_driver_init();
	if (ret != BK_OK) {
		os_printf("dma driver init failed\r\n");
		goto audio_tras_drv_exit;
	}

	/* allocate free DMA channel */
	audio_transfer_context.adc_dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
	if ((audio_transfer_context.adc_dma_id < DMA_ID_0) || (audio_transfer_context.adc_dma_id >= DMA_ID_MAX)) {
		os_printf("malloc adc dma fail \r\n");
		goto audio_tras_drv_exit;
	}

	audio_transfer_context.mic_ring_buff = (int32_t *)os_malloc(audio_transfer_context.mic_samp_rate_points*2*audio_transfer_context.mic_frame_number);
	if (audio_transfer_context.mic_ring_buff == NULL) {
		os_printf("malloc mic ring buffer fail \r\n");
		goto audio_tras_drv_exit;
	}

	/* config audio adc dma to carry mic data to "mic_ring_buff" */
	ret = audio_transfer_adc_dma_config(audio_transfer_context.adc_dma_id, audio_transfer_context.mic_ring_buff, (audio_transfer_context.mic_samp_rate_points*2)*audio_transfer_context.mic_frame_number, audio_transfer_context.mic_samp_rate_points*2);
	if (ret != BK_OK) {
		os_printf("config audio adc dma fail \r\n");
		goto audio_tras_drv_exit;
	}
	os_printf("step4: init and config mic DMA complete, adc_dma_id:%d, mic_ring_buff:%p, size:%d, carry_length:%d \r\n", audio_transfer_context.adc_dma_id, audio_transfer_context.mic_ring_buff, (audio_transfer_context.mic_samp_rate_points*2)*audio_transfer_context.mic_frame_number, audio_transfer_context.mic_samp_rate_points*2);

	/*  -------------------step5: init and config DMA to carry dac data ----------------------------- */
	/* init dma driver */
	ret = bk_dma_driver_init();
	if (ret != BK_OK) {
		os_printf("dma driver init failed\r\n");
		goto audio_tras_drv_exit;
	}

	/* allocate free DMA channel */
	audio_transfer_context.dac_dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
	if ((audio_transfer_context.dac_dma_id < DMA_ID_0) || (audio_transfer_context.dac_dma_id >= DMA_ID_MAX)) {
		os_printf("malloc dac dma fail \r\n");
		goto audio_tras_drv_exit;
	}

	audio_transfer_context.speaker_ring_buff = (int32_t *)os_malloc(audio_transfer_context.speaker_samp_rate_points*2*audio_transfer_context.speaker_frame_number);
	if (audio_transfer_context.speaker_ring_buff == NULL) {
		os_printf("malloc speaker ring buffer fail \r\n");
		goto audio_tras_drv_exit;
	}

	/* config audio dac dma to carry dac data to "speaker_ring_buff" */
	ret = audio_transfer_dac_dma_config(audio_transfer_context.dac_dma_id, audio_transfer_context.speaker_ring_buff, (audio_transfer_context.speaker_samp_rate_points*2)*audio_transfer_context.speaker_frame_number, audio_transfer_context.speaker_samp_rate_points*2);
	if (ret != BK_OK) {
		os_printf("config audio adc dma fail \r\n");
		goto audio_tras_drv_exit;
	}
	os_printf("step5: init and config speaker DMA complete, dac_dma_id:%d, speaker_ring_buff:%p, size:%d, carry_length:%d \r\n", audio_transfer_context.dac_dma_id, audio_transfer_context.speaker_ring_buff, (audio_transfer_context.speaker_samp_rate_points*2)*audio_transfer_context.speaker_frame_number, audio_transfer_context.speaker_samp_rate_points*2);

	/*  -------------------------step6: init all audio ring buffers -------------------------------- */
	/* init mic_ring_buff */
	//ring_buffer_init(&mic_rb, (uint8_t*)mic_ring_buff, frame_sample*2*2, adc_dma_id, RB_DMA_TYPE_WRITE);
	ring_buffer_init(&(audio_transfer_context.mic_rb), (uint8_t*)audio_transfer_context.mic_ring_buff, audio_transfer_context.mic_samp_rate_points*2*audio_transfer_context.mic_frame_number, audio_transfer_context.adc_dma_id, RB_DMA_TYPE_WRITE);

	/* init speaker_ref_ring_buff */
	//ring_buffer_init(&speaker_rb, (uint8_t*)speaker_ring_buff, PSRAM_AUD_DECD_RING_BUFF_SIZE*2, dac_dma_id, RB_DMA_TYPE_READ);
	ring_buffer_init(&(audio_transfer_context.speaker_rb), (uint8_t*)audio_transfer_context.speaker_ring_buff, audio_transfer_context.speaker_samp_rate_points*2*audio_transfer_context.speaker_frame_number, audio_transfer_context.dac_dma_id, RB_DMA_TYPE_READ);
	os_printf("step6: init all audio ring buffers complete \r\n");

	/*  -------------------------step6: init all audio ring buffers -------------------------------- */
	/* init encoder and decoder temp buffer */
	audio_transfer_context.encoder_temp.law_data = (uint8_t *)os_malloc(audio_transfer_context.mic_samp_rate_points);
	if (audio_transfer_context.encoder_temp.law_data == NULL) {
		os_printf("malloc law_data of encoder used fail \r\n");
		goto audio_tras_drv_exit;
	}

	audio_transfer_context.encoder_temp.pcm_data = (int16_t *)os_malloc(audio_transfer_context.mic_samp_rate_points*2);
	if (audio_transfer_context.encoder_temp.law_data == NULL) {
		os_printf("malloc pcm_data of encoder used fail \r\n");
		goto audio_tras_drv_exit;
	}

	audio_transfer_context.decoder_temp.law_data = (unsigned char *)os_malloc(audio_transfer_context.speaker_samp_rate_points);
	if (audio_transfer_context.decoder_temp.law_data == NULL) {
		os_printf("malloc law_data of decoder used fail \r\n");
		goto audio_tras_drv_exit;
	}

	audio_transfer_context.decoder_temp.pcm_data = (int16_t *)os_malloc(audio_transfer_context.speaker_samp_rate_points*2);
	if (audio_transfer_context.decoder_temp.pcm_data == NULL) {
		os_printf("malloc pcm_data of decoder used fail \r\n");
		goto audio_tras_drv_exit;
	}

	os_printf("init complete \r\n");
#if CONFIG_AUD_TRAS_MODE_CPU0
	/* audio transfer init callback */
	if (audio_transfer_context.aud_cb.audio_transfer_event_cb)
		audio_transfer_context.aud_cb.audio_transfer_event_cb(EVENT_AUD_TRAS_INIT_CMP);
#endif

#if CONFIG_AUD_TRAS_MODE_CPU1
	ret = media_send_com_mb_msg(CMD_COM_MB_AUD_TRAS_INIT_CMPL, 0, 0, 0);
	if (ret != BK_OK) {
		os_printf("send media common mailbox:CMD_COM_MB_AUD_TRAS_INIT_CMPL fail \r\n");
		goto audio_tras_drv_exit;
	}
#endif

	while(1) {
		audio_tras_drv_msg_t msg;
		ret = rtos_pop_from_queue(&aud_trs_drv_int_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			switch (msg.op) {
				case AUD_TRAS_DRV_IDLE:
					break;

				case AUD_TRAS_DRV_START:
					audio_start_transfer_process();
					break;

				case AUD_TRAS_DRV_AEC:
					audio_transfer_aec_process();
					break;

				case AUD_TRAS_DRV_ENCODER:
					audio_transfer_encoder_process();
					break;

				case AUD_TRAS_DRV_DECODER:
					audio_transfer_decoder_process();
					break;

#if 0
				case AUD_TRAS_DRV_TX_DONE:
					audio_transfer_decoder_process();
					break;
#endif
				case AUD_TRAS_DRV_EXIT:
					os_printf("goto: AUD_TRAS_DRV_EXIT \r\n");
					goto audio_tras_drv_exit;
					break;

				default:
					break;
			}
		}
	}

audio_tras_drv_exit:
	/* audio transfer driver deconfig */
	audio_transfer_deinit_config();

	/* delate msg queue */
	ret = rtos_deinit_queue(&aud_trs_drv_int_msg_que);
	if (ret != kNoErr) {
		os_printf("delate message queue fail \r\n");
	}
	aud_trs_drv_int_msg_que = NULL;
	os_printf("delate message queue complete \r\n");

	/* delate task */
	aud_trs_drv_thread_hdl = NULL;
	rtos_delete_thread(NULL);
}

aud_tras_drv_setup_t aud_trs_drv_setup_bak = {0};

bk_err_t bk_aud_tras_drv_init(aud_tras_drv_setup_t *setup_cfg)
{
	bk_err_t ret = BK_OK;
	if ((!aud_trs_drv_thread_hdl) && (!aud_trs_drv_int_msg_que)) {
		os_printf("start audio_transfer test \r\n");
		os_memcpy(&aud_trs_drv_setup_bak, setup_cfg, sizeof(aud_tras_drv_setup_t));

		ret = rtos_init_queue(&aud_trs_drv_int_msg_que,
							  "audio_internal_queue",
							  sizeof(audio_tras_drv_msg_t),
							  TU_QITEM_COUNT);
		if (ret != kNoErr) {
			os_printf("ceate audio transfer driver internal message queue failed \r\n");
			return BK_FAIL;
		}

		os_printf("ceate audio transfer driver internal message queue complete \r\n");
		//delay(100);

		//creat audio transfer driver task
		ret = rtos_create_thread(&aud_trs_drv_thread_hdl,
							 4,
							 "audio_intf",
							 (beken_thread_function_t)audio_transfer_driver_main,
							 4096,
							 (beken_thread_arg_t)&aud_trs_drv_setup_bak);
		if (ret != kNoErr) {
			os_printf("create audio transfer driver task fail \r\n");
			rtos_deinit_queue(&aud_trs_drv_int_msg_que);
			aud_trs_drv_int_msg_que = NULL;
			aud_trs_drv_thread_hdl = NULL;
		}
		os_printf("create audio transfer driver task complete \r\n");

		return kNoErr;
	} else
		return kInProgressErr;
}

bk_err_t bk_aud_tras_drv_deinit(void)
{
	bk_err_t ret;
	audio_tras_drv_msg_t msg;

	msg.op = AUD_TRAS_DRV_EXIT;
	if (aud_trs_drv_int_msg_que) {
		ret = rtos_push_to_queue_front(&aud_trs_drv_int_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			os_printf("audio send msg: AUD_TRAS_DRV_EXIT fail \r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

