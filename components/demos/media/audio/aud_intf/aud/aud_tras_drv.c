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

//#include <common/bk_include.h>
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
#include "aud_intf_private.h"
#include "aud_tras_drv.h"
#include <driver/psram.h>
#include <modules/aec.h>
#include <modules/audio_ring_buff.h>
#include <modules/g711.h>
#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
#include <driver/timer.h>
#endif

#define  AUD_DEBUG  0

#define AUD_TRAS_DRV_TAG "aud_trs_drv"

#define LOGI(...) BK_LOGI(AUD_TRAS_DRV_TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(AUD_TRAS_DRV_TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(AUD_TRAS_DRV_TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(AUD_TRAS_DRV_TAG, ##__VA_ARGS__)

#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
#define AUD_LOST_DEBUG_INTERVAL (1000 * 2)
#endif

#define TU_QITEM_COUNT      (60)
static beken_thread_t  aud_trs_drv_thread_hdl = NULL;
static beken_queue_t aud_trs_drv_int_msg_que = NULL;
//audio_transfer_context_t audio_transfer_context;

aud_tras_drv_info_t aud_tras_drv_info;


#if CONFIG_AUD_TRAS_MODE_CPU1
#define AUD_TRS_MAILBOX_CHECK_CONTROL    0
#if AUD_TRS_MAILBOX_CHECK_CONTROL
static bool aud_trs_drv_mb_busy_status = false;
#endif
#endif //CONFIG_AUD_TRAS_MODE_CPU1


#define CHECK_AUD_TRAS_DRV_MIC_STA() do {\
		if (aud_tras_drv_info.mic_info.status == AUD_TRAS_DRV_MIC_STA_NULL) {\
			return BK_FAIL;\
		}\
	} while(0)

/* extern api */
bk_err_t aud_tras_drv_deinit(void);

/*
bk_err_t aud_tras_drv_start_work(void)
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
*/

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
			//aud_tras_drv_start_work();
			break;

		case AUD_TRAS_MB_CMD_TX_DONE:
			audio_transfer_tx_done_process((uint32_t *)cmd_buf->param1, cmd_buf->param2);
			break;

		case AUD_TRAS_MB_CMD_STOP_TRANSFER:
			aud_tras_drv_deinit();
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

bk_err_t aud_tras_drv_send_msg(aud_tras_drv_op_t op, void *param)
{
	bk_err_t ret;
	aud_tras_drv_msg_t msg;

	msg.op = op;
	msg.param = param;
	if (aud_trs_drv_int_msg_que) {
		ret = rtos_push_to_queue(&aud_trs_drv_int_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			LOGI("aud_tras_send_int_msg fail \r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

static bk_err_t aud_tras_adc_config(aud_adc_config_t *adc_config)
{
	bk_err_t ret = BK_OK;

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

static bk_err_t aud_tras_dac_config(aud_dac_config_t *dac_config)
{
	bk_err_t ret = BK_OK;

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
	aec_info_t *temp_aec_info = aud_tras_drv_info.voc_info.aec_info;

	/* init aec context */
	aec_context_size = aec_size();
	temp_aec_info->aec = (AECContext*)os_malloc(aec_context_size);
	if (temp_aec_info->aec == NULL) {
		os_printf("malloc aec fail, aec_context_size: %d \r\n", aec_context_size);
	}

	/* config sample rate, default is 8K */
	switch (temp_aec_info->samp_rate) {
		case AEC_SAMP_RATE_8K:
			aec_init(temp_aec_info->aec, 8000);
			break;

		case AEC_SAMP_RATE_16K:
			aec_init(temp_aec_info->aec, 16000);
			break;

		default:
			break;
	}

	/* 获取处理帧长，16000采样率320点(640字节)，8000采样率160点(320字节)  (对应20毫秒数据) */
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_GET_FRAME_SAMPLE, (uint32_t)(&(temp_aec_info->samp_rate_points)));

	/* 获取结构体内部可以复用的ram作为每帧tx,rx,out数据的临时buffer; ram很宽裕的话也可以在外部单独申请获取 */
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_GET_TX_BUF, (uint32_t)(&val)); temp_aec_info->mic_addr = (int16_t*)val;
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_GET_RX_BUF, (uint32_t)(&val)); temp_aec_info->ref_addr = (int16_t*)val;
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_GET_OUT_BUF,(uint32_t)(&val)); temp_aec_info->out_addr = (int16_t*)val;

	/* 以下是参数调节示例,aec_init中都已经有默认值,可以直接先用默认值 */
	//aec_ctrl(temp_aec_context->aec, AEC_CTRL_CMD_SET_FLAGS, 0x1f);															//库内各模块开关; aec_init内默认赋值0x1f;
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_SET_FLAGS, temp_aec_info->aec_config->init_flags);							//库内各模块开关; aec_init内默认赋值0x1f;

	/* 回声消除相关 */
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_SET_MIC_DELAY, temp_aec_info->aec_config->mic_delay);						//设置参考信号延迟(采样点数，需要dump数据观察)
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_SET_EC_DEPTH, temp_aec_info->aec_config->ec_depth);							//建议取值范围1~50; 后面几个参数建议先用aec_init内的默认值，具体需要根据实际情况调试; 总得来说回声越大需要调的越大
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_SET_TxRxThr, temp_aec_info->aec_config->TxRxThr);							//建议取值范围10~64
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_SET_TxRxFlr, temp_aec_info->aec_config->TxRxFlr);							//建议取值范围1~10
//	aec_ctrl(temp_aec_context->aec, AEC_CTRL_CMD_SET_TxRxThr, 30);																//建议取值范围10~64
//	aec_ctrl(temp_aec_context->aec, AEC_CTRL_CMD_SET_TxRxFlr, 6);																//建议取值范围1~10

	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_SET_REF_SCALE, temp_aec_info->aec_config->ref_scale);						//取值0,1,2；rx数据如果幅值太小的话适当放大
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_SET_VOL, temp_aec_info->aec_config->voice_vol);								//通话过程中如果需要经常调节喇叭音量就设置下当前音量等级
	/* 降噪相关 */
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_SET_NS_LEVEL, temp_aec_info->aec_config->ns_level);							//建议取值范围1~8；值越小底噪越小
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_SET_NS_PARA, temp_aec_info->aec_config->ns_para);							//只能取值0,1,2; 降噪由弱到强，建议默认值
	/* drc(输出音量相关) */
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_SET_DRC, temp_aec_info->aec_config->drc);									//建议取值范围0x10~0x1f;   越大输出声音越大

}

static bk_err_t audio_transfer_aec_buff_config(aec_info_t *aec_info)
{
	bk_err_t ret = BK_OK;
	uint16_t samp_rate_points = aec_info->samp_rate_points;

	/* malloc aec ref ring buffer to save ref data */
	os_printf("ref_ring_buff size: %d \r\n", samp_rate_points*2*2);
	aec_info->aec_ref_ring_buff = (int16_t *)os_malloc(samp_rate_points*2*2);
	if (aec_info->aec_ref_ring_buff == NULL) {
		os_printf("malloc ref ring buffer fail \r\n");
		return BK_FAIL;
	}

	/* malloc aec out ring buffer to save mic data has been aec processed */
	aec_info->aec_out_ring_buff = (int16_t *)os_malloc(samp_rate_points*2*2);
	if (aec_info->aec_out_ring_buff == NULL) {
		os_printf("malloc aec out ring buffer fail \r\n");
		return BK_FAIL;
	}

	/* init ref_ring_buff */
	ring_buffer_init(&(aec_info->ref_rb), (uint8_t*)aec_info->aec_ref_ring_buff, samp_rate_points*2*2, DMA_ID_MAX, RB_DMA_TYPE_NULL);

	/* init aec_ring_buff */
	ring_buffer_init(&(aec_info->aec_rb), (uint8_t*)aec_info->aec_out_ring_buff, samp_rate_points*2*2, DMA_ID_MAX, RB_DMA_TYPE_NULL);

	return ret;
}

static void audio_transfer_aec_deconfig(void)
{
	if (aud_tras_drv_info.voc_info.aec_enable) {
		os_free(aud_tras_drv_info.voc_info.aec_info->aec);
		aud_tras_drv_info.voc_info.aec_info->aec = NULL;
		os_free(aud_tras_drv_info.voc_info.aec_info->aec_config);
		aud_tras_drv_info.voc_info.aec_info->aec_config = NULL;
		ring_buffer_clear(&(aud_tras_drv_info.voc_info.aec_info->ref_rb));
		ring_buffer_clear(&(aud_tras_drv_info.voc_info.aec_info->aec_rb));
		os_free(aud_tras_drv_info.voc_info.aec_info->aec_ref_ring_buff);
		aud_tras_drv_info.voc_info.aec_info->aec_ref_ring_buff = NULL;
		os_free(aud_tras_drv_info.voc_info.aec_info->aec_out_ring_buff);
		aud_tras_drv_info.voc_info.aec_info->aec_out_ring_buff = NULL;
		os_free(aud_tras_drv_info.voc_info.aec_info);
		aud_tras_drv_info.voc_info.aec_info = NULL;
	} else {
		aud_tras_drv_info.voc_info.aec_info = NULL;
	}

	aud_tras_drv_info.voc_info.aec_enable = false;
}

/* 搬运audio adc 采集到的一帧mic和ref信号后，触发中断通知AEC处理数据 */
static void aud_tras_adc_dma_finish_isr(void)
{
	bk_err_t ret = BK_OK;

	if (aud_tras_drv_info.work_mode == AUD_INTF_WORK_MODE_GENERAL)
		ret = aud_tras_drv_send_msg(AUD_TRAS_DRV_MIC_TX_DATA, NULL);
	else
		/* send msg to AEC to process mic and ref data */
		ret = aud_tras_drv_send_msg(AUD_TRAS_DRV_AEC, NULL);
	if (ret != kNoErr) {
		os_printf("send msg: AUD_TRAS_DRV_AEC fail \r\n");
	}
}

static bk_err_t aud_tras_adc_dma_config(dma_id_t dma_id, int32_t *ring_buff_addr, uint32_t ring_buff_size, uint32_t transfer_len, aud_intf_mic_chl_t mic_chl)
{
	bk_err_t ret = BK_OK;
	dma_config_t dma_config;
	uint32_t adc_port_addr;

	dma_config.mode = DMA_WORK_MODE_REPEAT;
	dma_config.chan_prio = 1;
	dma_config.src.dev = DMA_DEV_AUDIO;
	dma_config.dst.dev = DMA_DEV_DTCM;
	switch (mic_chl) {
		case AUD_INTF_MIC_CHL_MIC1:
			dma_config.src.width = DMA_DATA_WIDTH_16BITS;
			break;

		case AUD_INTF_MIC_CHL_DUAL:
			dma_config.src.width = DMA_DATA_WIDTH_32BITS;
			break;

		default:
			break;
	}
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
	bk_dma_register_isr(dma_id, NULL, (void *)aud_tras_adc_dma_finish_isr);
	bk_dma_enable_finish_interrupt(dma_id);

	return BK_OK;
}

/* 搬运audio dac 一帧dac信号后，触发中断通知decoder处理数据 */
static void aud_tras_dac_dma_finish_isr(void)
{
	bk_err_t ret = BK_OK;

	if (aud_tras_drv_info.work_mode == AUD_INTF_WORK_MODE_GENERAL)
		/* send msg to notify app to write speaker data */
		ret = aud_tras_drv_send_msg(AUD_TRAS_DRV_SPK_REQ_DATA, NULL);
	else
		/* send msg to decoder to decoding recevied data */
		ret = aud_tras_drv_send_msg(AUD_TRAS_DRV_DECODER, NULL);
	if (ret != kNoErr) {
		os_printf("send msg: AUD_TRAS_DRV_DECODER fails \r\n");
	}
}

static bk_err_t aud_tras_dac_dma_config(dma_id_t dma_id, int32_t *ring_buff_addr, uint32_t ring_buff_size, uint32_t transfer_len, aud_intf_spk_chl_t spk_chl)
{
	bk_err_t ret = BK_OK;
	dma_config_t dma_config;
	uint32_t dac_port_addr;

	dma_config.mode = DMA_WORK_MODE_REPEAT;
	dma_config.chan_prio = 1;
	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.dst.dev = DMA_DEV_AUDIO;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	switch (spk_chl) {
		case AUD_INTF_MIC_CHL_MIC1:
			dma_config.dst.width = DMA_DATA_WIDTH_16BITS;
			break;

		case AUD_INTF_MIC_CHL_DUAL:
			dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
			break;

		default:
			break;
	}

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
	bk_dma_register_isr(dma_id, NULL, (void *)aud_tras_dac_dma_finish_isr);
	bk_dma_enable_finish_interrupt(dma_id);

	return BK_OK;
}

static bk_err_t aud_tras_aec(void)
{
	bk_err_t ret = BK_OK;
	uint32_t size = 0;
	aec_info_t *aec_info_pr = aud_tras_drv_info.voc_info.aec_info;

	/* get a fram mic data from mic_ring_buff */
	if (ring_buffer_get_fill_size(&(aud_tras_drv_info.voc_info.mic_rb)) >= aec_info_pr->samp_rate_points*2) {
		size = ring_buffer_read(&(aud_tras_drv_info.voc_info.mic_rb), (uint8_t*)aec_info_pr->mic_addr, aec_info_pr->samp_rate_points*2);
		if (size != aec_info_pr->samp_rate_points*2) {
			os_printf("read mic_ring_buff fail, size:%d \r\n", size);
			//return BK_FAIL;
		}
	}

	/* read ref data from ref_ring_buff */
	if (ring_buffer_get_fill_size(&(aec_info_pr->ref_rb)) >= aec_info_pr->samp_rate_points*2) {
		size = ring_buffer_read(&(aec_info_pr->ref_rb), (uint8_t*)aec_info_pr->ref_addr, aec_info_pr->samp_rate_points*2);
		if (size != aec_info_pr->samp_rate_points*2) {
			os_printf("the ref data readed from ref_ring_buff is not a frame \r\n");
			//return BK_FAIL;
			//os_memcpy(ref_addr, 0, frame_sample*2);
		}
	}
	//os_printf("ref_addr: ref_addr[0]= %02x, ref_addr[0]= %02x \r\n", ref_addr[0], ref_addr[1]);

#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
	os_memcpy((void *)aud_tras_drv_info.voc_info.aec_dump.mic_dump_addr, aec_info_pr->mic_addr, aec_info_pr->samp_rate_points*2);
	os_memcpy((void *)aud_tras_drv_info.voc_info.aec_dump.ref_dump_addr, aec_info_pr->ref_addr, aec_info_pr->samp_rate_points*2);
	//os_printf("memcopy complete \r\n");
#endif

	/* aec process data */
	//os_printf("ref_addr:%p, mic_addr:%p, out_addr:%p \r\n", aec_context_pr->ref_addr, aec_context_pr->mic_addr, aec_context_pr->out_addr);
	aec_proc(aec_info_pr->aec, aec_info_pr->ref_addr, aec_info_pr->mic_addr, aec_info_pr->out_addr);

#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
		os_memcpy((void *)aud_tras_drv_info.voc_info.aec_dump.out_dump_addr, aec_info_pr->out_addr, aec_info_pr->samp_rate_points*2);
#endif

	/* save mic data after aec processed to aec_ring_buffer */
	if (ring_buffer_get_free_size(&(aec_info_pr->aec_rb)) >= aec_info_pr->samp_rate_points*2) {
		size = ring_buffer_write(&(aec_info_pr->aec_rb), (uint8_t*)aec_info_pr->out_addr, aec_info_pr->samp_rate_points*2);
		if (size != aec_info_pr->samp_rate_points*2) {
			os_printf("the data writeten to aec_ring_buff is not a frame \r\n");
			//return BK_FAIL;
		}
	}

	/* send msg to encoder to encoding data */
	ret = aud_tras_drv_send_msg(AUD_TRAS_DRV_ENCODER, NULL);
	if (ret != kNoErr) {
		os_printf("send msg: AUD_TRAS_DRV_ENCODER fail \r\n");
		return BK_FAIL;
	}

#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
#if CONFIG_AUD_TRAS_MODE_CPU0
	if (aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb)
		aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb(EVENT_AUD_TRAS_VOC_AEC_DUMP, &ret);
#endif
#endif

	return ret;
}

static bk_err_t aud_tras_enc(void)
{
	bk_err_t ret = BK_OK;
	uint32_t size = 0;
	uint32_t i = 0;
	uint16_t temp_mic_samp_rate_points = aud_tras_drv_info.voc_info.mic_samp_rate_points;
	tx_info_t temp_tx_info;

	if (aud_tras_drv_info.voc_info.aec_enable) {
		/* get data from aec_ring_buff */
		size = ring_buffer_read(&(aud_tras_drv_info.voc_info.aec_info->aec_rb), (uint8_t *)aud_tras_drv_info.voc_info.encoder_temp.pcm_data, temp_mic_samp_rate_points*2);
		if (size != temp_mic_samp_rate_points*2) {
			os_printf("the data readed from aec_ring_buff is not a frame, size:%d \r\n", size);
			goto encoder_exit;
		}
	} else {
		/* get data from mic_ring_buff */
		size = ring_buffer_read(&(aud_tras_drv_info.voc_info.mic_rb), (uint8_t *)aud_tras_drv_info.voc_info.encoder_temp.pcm_data, temp_mic_samp_rate_points*2);
		if (size != temp_mic_samp_rate_points*2) {
			os_printf("the data readed from mic_ring_buff is not a frame, size:%d \r\n", size);
			goto encoder_exit;
		}
	}

	switch (aud_tras_drv_info.voc_info.data_type) {
		case AUD_INTF_VOC_DATA_TYPE_G711A:
			/* G711A encoding pcm data to a-law data*/
			for (i=0; i<temp_mic_samp_rate_points; i++) {
				aud_tras_drv_info.voc_info.encoder_temp.law_data[i] = linear2alaw(aud_tras_drv_info.voc_info.encoder_temp.pcm_data[i]);
			}
			break;

		case AUD_INTF_VOC_DATA_TYPE_PCM:
			break;

		default:
			break;
	}

	temp_tx_info = aud_tras_drv_info.voc_info.tx_info;
#if CONFIG_AUD_TRAS_MODE_CPU0
	switch (aud_tras_drv_info.voc_info.data_type) {
		case AUD_INTF_VOC_DATA_TYPE_G711A:
			os_memcpy(temp_tx_info.ping.buff_addr, aud_tras_drv_info.voc_info.encoder_temp.law_data, temp_mic_samp_rate_points);
			break;

		case AUD_INTF_VOC_DATA_TYPE_PCM:
			os_memcpy(temp_tx_info.ping.buff_addr, aud_tras_drv_info.voc_info.encoder_temp.pcm_data, temp_mic_samp_rate_points * 2);
			break;

		default:
			break;
	}

	/* call sendto api of lwip UDP */
	if (aud_tras_drv_info.aud_tras_tx_mic_data != NULL) {
		size = aud_tras_drv_info.aud_tras_tx_mic_data((uint8_t *)temp_tx_info.ping.buff_addr, (uint32_t)temp_tx_info.buff_length);
		if (size != temp_tx_info.buff_length) {
#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
			aud_tras_drv_info.voc_info.lost_count.lost_size += temp_tx_info.buff_length;
#endif
		} else {
#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
			aud_tras_drv_info.voc_info.lost_count.complete_size += temp_tx_info.buff_length;
#endif
		}
	}
#endif

#if CONFIG_AUD_TRAS_MODE_CPU1
	/* save the data after G711A processed to ping pang buffer of tx */
	uint32_t *temp_tx_buffer = NULL;
	//temp_tx_context = audio_transfer_context.tx_context;
	if (temp_tx_info.ping.busy_status != true) {
		os_memcpy(temp_tx_info.ping.buff_addr, aud_tras_drv_info.voc_info.encoder_temp.law_data, temp_mic_samp_rate_points);
		temp_tx_info.ping.busy_status = true;
		if (temp_tx_info.pang.busy_status == true) {
			temp_tx_buffer = temp_tx_info.pang.buff_addr;
		} else {
			temp_tx_buffer = temp_tx_info.ping.buff_addr;
		}
	} else {
		os_memcpy(temp_tx_info.pang.buff_addr, aud_tras_drv_info.voc_info.encoder_temp.law_data, temp_mic_samp_rate_points);
		temp_tx_info.pang.busy_status = true;
		temp_tx_buffer = temp_tx_info.ping.buff_addr;
	}

	/* send mailbox to notify cpu0 to send data by lwip of wifi */
	ret = audio_send_aud_mailbox_msg(AUD_TRAS_MB_CMD_TX_REQ, (uint32_t)temp_tx_buffer, (uint32_t)temp_tx_info.buff_length, 0);
	if (ret != BK_OK) {
		os_printf("send mailbox msg: AUD_TRAS_MB_CMD_TX_REQ fail \r\n");
	}
#endif

	return ret;

encoder_exit:

	return BK_FAIL;
}

#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
static void aud_tras_drv_tx_lost_count_dump(timer_id_t timer_id)
{
	uint32_t lost = aud_tras_drv_info.voc_info.lost_count.lost_size, complete = aud_tras_drv_info.voc_info.lost_count.complete_size;
	aud_tras_drv_info.voc_info.lost_count.lost_size = 0;
	aud_tras_drv_info.voc_info.lost_count.complete_size = 0;

	lost = lost / (AUD_LOST_DEBUG_INTERVAL / 1000);
	complete = complete / 1024 / (AUD_LOST_DEBUG_INTERVAL / 1000);

	os_printf("[AUD Tx] Lost: %uB/s, Complete: %uKB/s \r\n", lost, complete);
}
#endif

static bk_err_t aud_tras_dec(void)
{
	uint32_t size = 0;
	uint32_t i = 0;

	switch (aud_tras_drv_info.voc_info.data_type) {
		case AUD_INTF_VOC_DATA_TYPE_G711A:
			/* check the frame number in decoder_ring_buffer */
			if (ring_buffer_get_fill_size(aud_tras_drv_info.voc_info.rx_info.decoder_rb) >= aud_tras_drv_info.voc_info.speaker_samp_rate_points) {
				//os_printf("decoder process \r\n", size);
				/* get G711A data from decoder_ring_buff */
				size = ring_buffer_read(aud_tras_drv_info.voc_info.rx_info.decoder_rb, (uint8_t*)aud_tras_drv_info.voc_info.decoder_temp.law_data, aud_tras_drv_info.voc_info.speaker_samp_rate_points);
				if (size != aud_tras_drv_info.voc_info.speaker_samp_rate_points) {
					os_printf("read decoder_ring_buff G711A data fail \r\n");
					//return BK_FAIL;
				}
			} else {
				os_memset(aud_tras_drv_info.voc_info.decoder_temp.law_data, 0xD5, aud_tras_drv_info.voc_info.speaker_samp_rate_points);
			}

			/* G711A decoding a-law data to pcm data*/
			for (i=0; i<aud_tras_drv_info.voc_info.speaker_samp_rate_points; i++) {
				aud_tras_drv_info.voc_info.decoder_temp.pcm_data[i] = alaw2linear(aud_tras_drv_info.voc_info.decoder_temp.law_data[i]);
			}
			break;

		case AUD_INTF_VOC_DATA_TYPE_PCM:
			if (ring_buffer_get_fill_size(aud_tras_drv_info.voc_info.rx_info.decoder_rb) >= aud_tras_drv_info.voc_info.speaker_samp_rate_points * 2) {
				//os_printf("decoder process \r\n", size);
				/* get pcm data from decoder_ring_buff */
				size = ring_buffer_read(aud_tras_drv_info.voc_info.rx_info.decoder_rb, (uint8_t*)aud_tras_drv_info.voc_info.decoder_temp.pcm_data, aud_tras_drv_info.voc_info.speaker_samp_rate_points * 2);
				if (size != aud_tras_drv_info.voc_info.speaker_samp_rate_points * 2) {
					os_printf("read decoder_ring_buff pcm data fail \r\n");
					//return BK_FAIL;
				}
			} else {
				os_memset(aud_tras_drv_info.voc_info.decoder_temp.pcm_data, 0x00, aud_tras_drv_info.voc_info.speaker_samp_rate_points * 2);
			}
			break;

		default:
			break;
	}

	if (aud_tras_drv_info.voc_info.aec_enable) {
		/* read mic fill data size */
		uint32_t mic_fill_size = ring_buffer_get_fill_size(&(aud_tras_drv_info.voc_info.mic_rb));
		//os_printf("mic_rb: fill_size=%d \r\n", mic_fill_size);
		uint32_t speaker_fill_size = ring_buffer_get_fill_size(&(aud_tras_drv_info.voc_info.speaker_rb));
		//os_printf("speaker_rb: fill_size=%d \r\n", speaker_fill_size);
		uint32_t ref_fill_size = ring_buffer_get_fill_size(&(aud_tras_drv_info.voc_info.aec_info->ref_rb));
		//os_printf("ref_rb: fill_size=%d \r\n", ref_fill_size);
		/* 设置参考信号延迟(采样点数，需要dump数据观察) */
		//os_printf("MIC_DELAY: %d \r\n", (mic_fill_size + speaker_fill_size - ref_fill_size)/2);
		if ((mic_fill_size + speaker_fill_size - ref_fill_size)/2 < 0) {
			os_printf("MIC_DELAY is error \r\n", ref_fill_size);
			aec_ctrl(aud_tras_drv_info.voc_info.aec_info->aec, AEC_CTRL_CMD_SET_MIC_DELAY, 0);
		} else {
			aec_ctrl(aud_tras_drv_info.voc_info.aec_info->aec, AEC_CTRL_CMD_SET_MIC_DELAY, (mic_fill_size + speaker_fill_size - ref_fill_size)/2);
		}

		if (ring_buffer_get_free_size(&(aud_tras_drv_info.voc_info.aec_info->ref_rb)) > aud_tras_drv_info.voc_info.aec_info->samp_rate_points*2) {
			size = ring_buffer_write(&(aud_tras_drv_info.voc_info.aec_info->ref_rb), (uint8_t *)aud_tras_drv_info.voc_info.decoder_temp.pcm_data, aud_tras_drv_info.voc_info.aec_info->samp_rate_points*2);
			if (size != aud_tras_drv_info.voc_info.aec_info->samp_rate_points*2) {
				os_printf("write data to ref_ring_buff fail, size=%d \r\n", size);
				goto decoder_exit;
			}
		}
		}

	/* save the data after G711A processed to encoder_ring_buffer */
	if (ring_buffer_get_free_size(&(aud_tras_drv_info.voc_info.speaker_rb)) > aud_tras_drv_info.voc_info.speaker_samp_rate_points*2) {
		size = ring_buffer_write(&(aud_tras_drv_info.voc_info.speaker_rb), (uint8_t *)aud_tras_drv_info.voc_info.decoder_temp.pcm_data, aud_tras_drv_info.voc_info.speaker_samp_rate_points*2);
		if (size != aud_tras_drv_info.voc_info.speaker_samp_rate_points*2) {
			os_printf("the data writeten to speaker_ring_buff is not a frame, size=%d \r\n", size);
			goto decoder_exit;
		}
		aud_tras_drv_info.voc_info.rx_info.aud_trs_read_seq++;
	}

	/* call callback to notify app */
	if (aud_tras_drv_info.aud_tras_rx_spk_data)
		aud_tras_drv_info.aud_tras_rx_spk_data((unsigned int)aud_tras_drv_info.voc_info.speaker_samp_rate_points*2);

	return BK_OK;

decoder_exit:

	return BK_FAIL;
}


static bk_err_t aud_tras_drv_mic_tx_data(void)
{
	bk_err_t ret = BK_OK;
	uint32_t size = 0;

	/* get a fram mic data from mic_ring_buff */
	if (ring_buffer_get_fill_size(&(aud_tras_drv_info.mic_info.mic_rb)) >= aud_tras_drv_info.mic_info.frame_size) {
		size = ring_buffer_read(&(aud_tras_drv_info.mic_info.mic_rb), (uint8_t*)aud_tras_drv_info.mic_info.temp_mic_addr, aud_tras_drv_info.mic_info.frame_size);
		if (size != aud_tras_drv_info.mic_info.frame_size) {
			os_printf("read mic_ring_buff fail, size:%d \r\n", size);
			//return BK_FAIL;
		}
	}

	/* call sendto api of lwip UDP */
	if (aud_tras_drv_info.aud_tras_tx_mic_data != NULL) {
		size = aud_tras_drv_info.aud_tras_tx_mic_data((uint8_t *)aud_tras_drv_info.mic_info.temp_mic_addr, (uint32_t)aud_tras_drv_info.mic_info.frame_size);
		if (size != aud_tras_drv_info.mic_info.frame_size) {
			//os_printf("send audio data by wifi fail \r\n");
			os_printf("audio lost \r\n");
		}
	}

	return ret;
}

static bk_err_t aud_tras_drv_spk_req_data(void)
{
	bk_err_t ret = BK_OK;
	uint32_t size = 0;

	/* get speaker data from spk_rx_ring_buff */
	if (ring_buffer_get_fill_size(aud_tras_drv_info.spk_info.spk_rx_rb) >= aud_tras_drv_info.spk_info.frame_size) {
		size = ring_buffer_read(aud_tras_drv_info.spk_info.spk_rx_rb, (uint8_t*)aud_tras_drv_info.spk_info.temp_spk_addr, aud_tras_drv_info.spk_info.frame_size);
		if (size != aud_tras_drv_info.spk_info.frame_size) {
			os_printf("read spk_rx_ring_buff fail, size:%d \r\n", size);
			//return BK_FAIL;
		}
	} else {
		os_printf("spk_rx_rb is empty \r\n");
		os_memset(aud_tras_drv_info.spk_info.temp_spk_addr, 0, aud_tras_drv_info.spk_info.frame_size);
	}

	/* write spk_rx_data to audio dac */
	size = ring_buffer_write(&(aud_tras_drv_info.spk_info.spk_rb), (uint8_t*)aud_tras_drv_info.spk_info.temp_spk_addr, aud_tras_drv_info.spk_info.frame_size);
	if (size != aud_tras_drv_info.spk_info.frame_size) {
		os_printf("write spk_data to audio dac fail, size:%d \r\n", size);
		return BK_FAIL;
	}

	/* call callback to notify app */
	if (aud_tras_drv_info.aud_tras_rx_spk_data)
		ret = aud_tras_drv_info.aud_tras_rx_spk_data((unsigned int)aud_tras_drv_info.spk_info.frame_size);

	return ret;
}

static bk_err_t aud_tras_drv_spk_init(aud_intf_spk_config_t *spk_cfg)
{
	bk_err_t ret = BK_OK;

	/* get audio dac config */
	aud_tras_drv_info.spk_info.dac_config = os_malloc(sizeof(aud_dac_config_t));
	if (aud_tras_drv_info.spk_info.dac_config == NULL) {
		os_printf("adc_config os_malloc fail \r\n");
	} else {
		aud_tras_drv_info.spk_info.dac_config->dac_enable = AUD_DAC_DISABLE;
		aud_tras_drv_info.spk_info.dac_config->samp_rate = spk_cfg->samp_rate;
		aud_tras_drv_info.spk_info.dac_config->dac_hpf2_coef_B2 = 0x3A22;
		aud_tras_drv_info.spk_info.dac_config->dac_hpf2_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
		aud_tras_drv_info.spk_info.dac_config->dac_hpf1_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
		aud_tras_drv_info.spk_info.dac_config->dac_set_gain = spk_cfg->spk_gain;	//default 2D  3F  15
		aud_tras_drv_info.spk_info.dac_config->dac_clk_invert = AUD_DAC_CLK_INVERT_RISING;
		aud_tras_drv_info.spk_info.dac_config->dac_hpf2_coef_B0 = 0x3A22;
		aud_tras_drv_info.spk_info.dac_config->dac_hpf2_coef_B1 = 0x8BBF;
		aud_tras_drv_info.spk_info.dac_config->dac_hpf2_coef_A1 = 0x751C;
		aud_tras_drv_info.spk_info.dac_config->dac_hpf2_coef_A2 = 0xC9E6;
		aud_tras_drv_info.spk_info.dac_config->dacr_rd_threshold = 0x4;
		aud_tras_drv_info.spk_info.dac_config->dacl_rd_threshold = 0x4;
		aud_tras_drv_info.spk_info.dac_config->dacr_int_enable = 0x0;
		aud_tras_drv_info.spk_info.dac_config->dacl_int_enable = 0x0;
		aud_tras_drv_info.spk_info.dac_config->dac_filt_enable = AUD_DAC_FILT_DISABLE;
		aud_tras_drv_info.spk_info.dac_config->dac_fracmod_manual_enable = AUD_DAC_FRACMOD_MANUAL_DISABLE;
		aud_tras_drv_info.spk_info.dac_config->dac_fracmode_value = 0x0;

	}
	aud_tras_drv_info.spk_info.spk_en = true;
	aud_tras_drv_info.spk_info.spk_chl = spk_cfg->spk_chl;
	aud_tras_drv_info.spk_info.frame_size = spk_cfg->frame_size;
	aud_tras_drv_info.spk_info.fifo_frame_num = spk_cfg->fifo_frame_num;
	aud_tras_drv_info.spk_info.spk_rx_rb = spk_cfg->spk_rx_rb;
	aud_tras_drv_info.spk_info.spk_rx_ring_buff = spk_cfg->spk_rx_ring_buff;
	aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb = spk_cfg->aud_tras_drv_spk_event_cb;

	/* audio dac config */
	ret = aud_tras_dac_config(aud_tras_drv_info.spk_info.dac_config);
	if (ret != BK_OK) {
		os_printf("audio dac init fail \r\n");
		goto aud_tras_drv_spk_init_exit;
	}
	os_printf("step1: init audio and config ADC and DAC complete \r\n");

	/* init dma driver */
	ret = bk_dma_driver_init();
	if (ret != BK_OK) {
		os_printf("dma driver init failed\r\n");
		goto aud_tras_drv_spk_init_exit;
	}

	/* allocate free DMA channel */
	aud_tras_drv_info.spk_info.spk_dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
	if ((aud_tras_drv_info.spk_info.spk_dma_id < DMA_ID_0) || (aud_tras_drv_info.spk_info.spk_dma_id >= DMA_ID_MAX)) {
		os_printf("malloc dac dma fail \r\n");
		goto aud_tras_drv_spk_init_exit;
	}

	aud_tras_drv_info.spk_info.spk_ring_buff = (int32_t *)os_malloc(aud_tras_drv_info.spk_info.frame_size * 2);
	if (aud_tras_drv_info.spk_info.spk_ring_buff == NULL) {
		os_printf("malloc speakerk ring buffer fail \r\n");
		goto aud_tras_drv_spk_init_exit;
	}

	/* config audio dac dma to carry speaker data to "spk_ring_buff" */
	ret = aud_tras_dac_dma_config(aud_tras_drv_info.spk_info.spk_dma_id, aud_tras_drv_info.spk_info.spk_ring_buff, aud_tras_drv_info.spk_info.frame_size * 2, aud_tras_drv_info.spk_info.frame_size, aud_tras_drv_info.spk_info.spk_chl);
	if (ret != BK_OK) {
		os_printf("config audio dac dma fail \r\n");
		goto aud_tras_drv_spk_init_exit;
	}

	/* init spk_ring_buff */
	ring_buffer_init(&(aud_tras_drv_info.spk_info.spk_rb), (uint8_t*)aud_tras_drv_info.spk_info.spk_ring_buff, aud_tras_drv_info.spk_info.frame_size * 2, aud_tras_drv_info.spk_info.spk_dma_id, RB_DMA_TYPE_READ);

	os_printf("step2: init dma:%d, and spk ring buff:%p, size:%d complete \r\n", aud_tras_drv_info.spk_info.spk_dma_id, aud_tras_drv_info.spk_info.spk_ring_buff, aud_tras_drv_info.spk_info.frame_size * 2);

	/* init mic temp buffer */
	aud_tras_drv_info.spk_info.temp_spk_addr = (int32_t *)os_malloc(aud_tras_drv_info.spk_info.frame_size);
	if (aud_tras_drv_info.spk_info.temp_spk_addr == NULL) {
		os_printf("malloc temp spk ring buffer fail \r\n");
		goto aud_tras_drv_spk_init_exit;
	}

	os_printf("step3: init temp spk ring buff \r\n");

	/* change status: AUD_TRAS_DRV_SPK_NULL --> AUD_TRAS_DRV_SPK_IDLE */
	aud_tras_drv_info.spk_info.status = AUD_TRAS_DRV_SPK_STA_IDLE;

	if (aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb)
		aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb(EVENT_AUD_TRAS_SPK_INIT, NULL);

	os_printf("step4: init spk complete \r\n");

	return BK_OK;

aud_tras_drv_spk_init_exit:
	aud_tras_drv_info.spk_info.spk_en = false;
	aud_tras_drv_info.spk_info.status = AUD_TRAS_DRV_MIC_STA_NULL;
	aud_tras_drv_info.spk_info.spk_chl = AUD_INTF_MIC_CHL_MIC1;

	if (aud_tras_drv_info.spk_info.dac_config)
		os_free(aud_tras_drv_info.spk_info.dac_config);
	aud_tras_drv_info.spk_info.dac_config = NULL;

	aud_tras_drv_info.spk_info.frame_size = 0;
	if (aud_tras_drv_info.spk_info.spk_dma_id != DMA_ID_MAX) {
//		bk_dma_stop(aud_tras_drv_info.spk_info.mic_dma_id);
		bk_dma_deinit(aud_tras_drv_info.spk_info.spk_dma_id);
		bk_dma_free(DMA_DEV_AUDIO, aud_tras_drv_info.spk_info.spk_dma_id);
	}
	aud_tras_drv_info.spk_info.spk_dma_id = DMA_ID_MAX;

	if (aud_tras_drv_info.spk_info.spk_ring_buff != NULL) {
		ring_buffer_clear(&(aud_tras_drv_info.spk_info.spk_rb));
		os_free(aud_tras_drv_info.spk_info.spk_ring_buff);
	}
	aud_tras_drv_info.spk_info.spk_ring_buff = NULL;

	if (aud_tras_drv_info.spk_info.temp_spk_addr != NULL)
		os_free(aud_tras_drv_info.spk_info.temp_spk_addr);
	aud_tras_drv_info.spk_info.temp_spk_addr = NULL;

	return BK_FAIL;
}

static bk_err_t aud_tras_drv_spk_deinit(void)
{
	/* disable audio dac */
	bk_aud_stop_dac();
	bk_dma_stop(aud_tras_drv_info.spk_info.spk_dma_id);
	bk_dma_deinit(aud_tras_drv_info.spk_info.spk_dma_id);
	bk_dma_free(DMA_DEV_AUDIO, aud_tras_drv_info.spk_info.spk_dma_id);
	bk_aud_adc_deinit();
	os_free(aud_tras_drv_info.spk_info.dac_config);
	aud_tras_drv_info.spk_info.dac_config = NULL;

	aud_tras_drv_info.spk_info.spk_en = false;
	aud_tras_drv_info.spk_info.spk_chl = AUD_INTF_MIC_CHL_MIC1;
	aud_tras_drv_info.spk_info.frame_size = 0;
	aud_tras_drv_info.spk_info.spk_dma_id = DMA_ID_MAX;

	ring_buffer_clear(&(aud_tras_drv_info.spk_info.spk_rb));
	os_free(aud_tras_drv_info.spk_info.spk_ring_buff);
	aud_tras_drv_info.spk_info.spk_ring_buff = NULL;

	os_free(aud_tras_drv_info.spk_info.temp_spk_addr);
	aud_tras_drv_info.spk_info.temp_spk_addr = NULL;

	aud_tras_drv_info.spk_info.status = AUD_TRAS_DRV_SPK_STA_NULL;

	if (aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb)
		aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb(EVENT_AUD_TRAS_SPK_DEINIT, NULL);

	return BK_OK;
}

static bk_err_t aud_tras_drv_spk_start(void)
{
	bk_err_t ret = BK_OK;
	uint32_t size = 0;

	if ((aud_tras_drv_info.spk_info.status != AUD_TRAS_DRV_SPK_STA_IDLE) && (aud_tras_drv_info.spk_info.status != AUD_TRAS_DRV_SPK_STA_PAUSE))
		return BK_FAIL;

	ret = bk_dma_start(aud_tras_drv_info.spk_info.spk_dma_id);
	if (ret != BK_OK) {
		os_printf("start speaker dma fail \r\n");
		return BK_FAIL;
	}

	/* get speaker data from spk_rx_ring_buff */
	if (ring_buffer_get_fill_size(aud_tras_drv_info.spk_info.spk_rx_rb) >= aud_tras_drv_info.spk_info.frame_size) {
		size = ring_buffer_read(aud_tras_drv_info.spk_info.spk_rx_rb, (uint8_t*)aud_tras_drv_info.spk_info.temp_spk_addr, aud_tras_drv_info.spk_info.frame_size);
		if (size != aud_tras_drv_info.spk_info.frame_size) {
			os_printf("read spk_rx_ring_buff fail, size:%d \r\n", size);
			//return BK_FAIL;
		}
	} else {
		os_printf("spk_rx_rb is empty \r\n");
		os_memset(aud_tras_drv_info.spk_info.temp_spk_addr, 0, aud_tras_drv_info.spk_info.frame_size);
	}

	/* write spk_rx_data to audio dac */
	size = ring_buffer_write(&(aud_tras_drv_info.spk_info.spk_rb), (uint8_t*)aud_tras_drv_info.spk_info.temp_spk_addr, aud_tras_drv_info.spk_info.frame_size);
	if (size != aud_tras_drv_info.spk_info.frame_size) {
		os_printf("write spk_data to audio dac fail, size:%d \r\n", size);
		return BK_FAIL;
	}

	if (ring_buffer_get_fill_size(aud_tras_drv_info.spk_info.spk_rx_rb) >= aud_tras_drv_info.spk_info.frame_size) {
		size = ring_buffer_read(aud_tras_drv_info.spk_info.spk_rx_rb, (uint8_t*)aud_tras_drv_info.spk_info.temp_spk_addr, aud_tras_drv_info.spk_info.frame_size);
		if (size != aud_tras_drv_info.spk_info.frame_size) {
			os_printf("read spk_rx_ring_buff fail, size:%d \r\n", size);
			//return BK_FAIL;
		}
	} else {
		os_printf("spk_rx_rb is empty \r\n");
		os_memset(aud_tras_drv_info.spk_info.temp_spk_addr, 0, aud_tras_drv_info.spk_info.frame_size);
	}

	/* write spk_rx_data to audio dac */
	size = ring_buffer_write(&(aud_tras_drv_info.spk_info.spk_rb), (uint8_t*)aud_tras_drv_info.spk_info.temp_spk_addr, aud_tras_drv_info.spk_info.frame_size);
	if (size != aud_tras_drv_info.spk_info.frame_size) {
		os_printf("write spk_data to audio dac fail, size:%d \r\n", size);
		return BK_FAIL;
	}

	/* enable dac */
	bk_aud_start_dac();
	bk_aud_start_dac();
	aud_tras_drv_info.spk_info.status = AUD_TRAS_DRV_SPK_STA_START;

	if (aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb)
		aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb(EVENT_AUD_TRAS_SPK_START, NULL);

	return BK_OK;
}

/* not stop dma, only disable adc */
static bk_err_t aud_tras_drv_spk_pause(void)
{
	//bk_err_t ret = BK_OK;
	if (aud_tras_drv_info.mic_info.status != AUD_TRAS_DRV_MIC_STA_START)
		return BK_FAIL;

	/* disable adc */
	bk_aud_stop_dac();
	aud_tras_drv_info.spk_info.status = AUD_TRAS_DRV_SPK_STA_PAUSE;

	if (aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb)
		aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb(EVENT_AUD_TRAS_SPK_PAUSE, NULL);

	return BK_OK;
}

static bk_err_t aud_tras_drv_spk_stop(void)
{
	//bk_err_t ret = BK_OK;
	if ((aud_tras_drv_info.spk_info.status != AUD_TRAS_DRV_SPK_STA_START) && (aud_tras_drv_info.spk_info.status != AUD_TRAS_DRV_SPK_STA_PAUSE))
		return BK_FAIL;

	/* disable adc */
	bk_aud_stop_dac();
	bk_dma_stop(aud_tras_drv_info.spk_info.spk_dma_id);
	ring_buffer_clear(&(aud_tras_drv_info.spk_info.spk_rb));
	aud_tras_drv_info.spk_info.status = AUD_TRAS_DRV_SPK_STA_IDLE;

	if (aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb)
		aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb(EVENT_AUD_TRAS_SPK_STOP, NULL);

	return BK_OK;
}


static bk_err_t aud_tras_drv_mic_init(aud_intf_mic_config_t *mic_cfg)
{
	bk_err_t ret = BK_OK;

	os_printf("mic_cfg->frame_size: %d \r\n", mic_cfg->frame_size);

	/* get audio adc config */
	aud_tras_drv_info.mic_info.adc_config = os_malloc(sizeof(aud_adc_config_t));
	if (aud_tras_drv_info.mic_info.adc_config == NULL) {
		os_printf("adc_config os_malloc fail \r\n");
	} else {
		aud_tras_drv_info.mic_info.adc_config->mic_config = AUD_MIC_MIC1_ENABLE;
		aud_tras_drv_info.mic_info.adc_config->samp_rate = mic_cfg->samp_rate;
		aud_tras_drv_info.mic_info.adc_config->adc_enable = AUD_ADC_DISABLE;
		aud_tras_drv_info.mic_info.adc_config->line_enable = AUD_ADC_LINE_DISABLE;
		aud_tras_drv_info.mic_info.adc_config->dtmf_enable = AUD_DTMF_DISABLE;
		aud_tras_drv_info.mic_info.adc_config->adc_hpf2_coef_B2 = 0;
		aud_tras_drv_info.mic_info.adc_config->adc_hpf2_bypass_enable = AUD_ADC_HPF_BYPASS_ENABLE;
		aud_tras_drv_info.mic_info.adc_config->adc_hpf1_bypass_enable = AUD_ADC_HPF_BYPASS_ENABLE;
		aud_tras_drv_info.mic_info.adc_config->adc_set_gain = mic_cfg->mic_gain;	//default: 0x2d
		aud_tras_drv_info.mic_info.adc_config->adc_samp_edge = AUD_ADC_SAMP_EDGE_RISING;
		aud_tras_drv_info.mic_info.adc_config->adc_hpf2_coef_B0 = 0;
		aud_tras_drv_info.mic_info.adc_config->adc_hpf2_coef_B1 = 0;
		aud_tras_drv_info.mic_info.adc_config->adc_hpf2_coef_A0 = 0;
		aud_tras_drv_info.mic_info.adc_config->adc_hpf2_coef_A1 = 0;
		aud_tras_drv_info.mic_info.adc_config->dtmf_wr_threshold = 8;
		aud_tras_drv_info.mic_info.adc_config->adcl_wr_threshold = 8;
		aud_tras_drv_info.mic_info.adc_config->dtmf_int_enable = AUD_DTMF_INT_DISABLE;
		aud_tras_drv_info.mic_info.adc_config->adcl_int_enable = AUD_ADCL_INT_DISABLE;
		aud_tras_drv_info.mic_info.adc_config->loop_adc2dac = AUD_LOOP_ADC2DAC_DISABLE;
		aud_tras_drv_info.mic_info.adc_config->agc_noise_thrd = 101;
		aud_tras_drv_info.mic_info.adc_config->agc_noise_high = 101;
		aud_tras_drv_info.mic_info.adc_config->agc_noise_low = 160;
		aud_tras_drv_info.mic_info.adc_config->agc_noise_min = 1;
		aud_tras_drv_info.mic_info.adc_config->agc_noise_tout = 0;
		aud_tras_drv_info.mic_info.adc_config->agc_high_dur = 3;
		aud_tras_drv_info.mic_info.adc_config->agc_low_dur = 3;
		aud_tras_drv_info.mic_info.adc_config->agc_min = 1;
		aud_tras_drv_info.mic_info.adc_config->agc_max = 4;
		aud_tras_drv_info.mic_info.adc_config->agc_ng_method = AUD_AGC_NG_METHOD_MUTE;
		aud_tras_drv_info.mic_info.adc_config->agc_ng_enable = AUD_AGC_NG_DISABLE;
		aud_tras_drv_info.mic_info.adc_config->agc_decay_time = AUD_AGC_DECAY_TIME_128;
		aud_tras_drv_info.mic_info.adc_config->agc_attack_time = AUD_AGC_ATTACK_TIME_128;
		aud_tras_drv_info.mic_info.adc_config->agc_high_thrd = 18;
		aud_tras_drv_info.mic_info.adc_config->agc_low_thrd = 0;
		aud_tras_drv_info.mic_info.adc_config->agc_iir_coef = AUD_AGC_IIR_COEF_1_1024;
		aud_tras_drv_info.mic_info.adc_config->agc_enable = AUD_AGC_DISABLE;
		aud_tras_drv_info.mic_info.adc_config->manual_pga_value = 0;
		aud_tras_drv_info.mic_info.adc_config->manual_pga_enable = AUD_GAC_MANUAL_PGA_DISABLE;
		aud_tras_drv_info.mic_info.adc_config->adc_fracmod_manual = AUD_ADC_TRACMOD_MANUAL_DISABLE;
		aud_tras_drv_info.mic_info.adc_config->adc_fracmod = 0;
	}
	aud_tras_drv_info.mic_info.mic_en = true;
	aud_tras_drv_info.mic_info.mic_chl = mic_cfg->mic_chl;
	aud_tras_drv_info.mic_info.frame_size = mic_cfg->frame_size;

	/* audio adc config */
	ret = aud_tras_adc_config(aud_tras_drv_info.mic_info.adc_config);
	if (ret != BK_OK) {
		os_printf("audio adc init fail \r\n");
		goto aud_tras_drv_mic_init_exit;
	}

	/* init dma driver */
	ret = bk_dma_driver_init();
	if (ret != BK_OK) {
		os_printf("dma driver init failed\r\n");
		goto aud_tras_drv_mic_init_exit;
	}

	/* allocate free DMA channel */
	aud_tras_drv_info.mic_info.mic_dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
	if ((aud_tras_drv_info.mic_info.mic_dma_id < DMA_ID_0) || (aud_tras_drv_info.mic_info.mic_dma_id >= DMA_ID_MAX)) {
		os_printf("malloc adc dma fail \r\n");
		goto aud_tras_drv_mic_init_exit;
	}

	os_printf("mic_ring_buff size: %d \r\n", aud_tras_drv_info.mic_info.frame_size * 2);
	aud_tras_drv_info.mic_info.mic_ring_buff = (int32_t *)os_malloc(aud_tras_drv_info.mic_info.frame_size * 2);
	if (aud_tras_drv_info.mic_info.mic_ring_buff == NULL) {
		os_printf("malloc mic ring buffer fail \r\n");
		goto aud_tras_drv_mic_init_exit;
	}

	/* config audio adc dma to carry mic data to "mic_ring_buff" */
	ret = aud_tras_adc_dma_config(aud_tras_drv_info.mic_info.mic_dma_id, aud_tras_drv_info.mic_info.mic_ring_buff, aud_tras_drv_info.mic_info.frame_size * 2, aud_tras_drv_info.mic_info.frame_size, aud_tras_drv_info.mic_info.mic_chl);
	if (ret != BK_OK) {
		os_printf("config audio adc dma fail \r\n");
		goto aud_tras_drv_mic_init_exit;
	}

	/* init mic_ring_buff */
	ring_buffer_init(&(aud_tras_drv_info.mic_info.mic_rb), (uint8_t*)aud_tras_drv_info.mic_info.mic_ring_buff, aud_tras_drv_info.mic_info.frame_size * 2, aud_tras_drv_info.mic_info.mic_dma_id, RB_DMA_TYPE_WRITE);

	/* init mic temp buffer */
	aud_tras_drv_info.mic_info.temp_mic_addr = (int32_t *)os_malloc(aud_tras_drv_info.mic_info.frame_size);
	if (aud_tras_drv_info.mic_info.temp_mic_addr == NULL) {
		os_printf("malloc temp mic ring buffer fail \r\n");
		goto aud_tras_drv_mic_init_exit;
	}

	/* change status: AUD_TRAS_DRV_MIC_NULL --> AUD_TRAS_DRV_MIC_IDLE */
	aud_tras_drv_info.mic_info.status = AUD_TRAS_DRV_MIC_STA_IDLE;

	os_printf("init mic of complete, mic_dma_id:%d, mic_ring_buff:%p, size:%d, carry_length:%d \r\n", aud_tras_drv_info.mic_info.mic_dma_id, aud_tras_drv_info.mic_info.mic_ring_buff, aud_tras_drv_info.mic_info.frame_size * 2, aud_tras_drv_info.mic_info.frame_size);
	return BK_OK;

aud_tras_drv_mic_init_exit:
	aud_tras_drv_info.mic_info.mic_en = false;
	aud_tras_drv_info.mic_info.status = AUD_TRAS_DRV_MIC_STA_NULL;
	aud_tras_drv_info.mic_info.mic_chl = AUD_INTF_MIC_CHL_MIC1;

	if (aud_tras_drv_info.mic_info.adc_config)
		os_free(aud_tras_drv_info.mic_info.adc_config);
	aud_tras_drv_info.mic_info.adc_config = NULL;

	aud_tras_drv_info.mic_info.frame_size = 0;
	if (aud_tras_drv_info.mic_info.mic_dma_id != DMA_ID_MAX) {
		bk_dma_stop(aud_tras_drv_info.mic_info.mic_dma_id);
		bk_dma_deinit(aud_tras_drv_info.mic_info.mic_dma_id);
		bk_dma_free(DMA_DEV_AUDIO, aud_tras_drv_info.mic_info.mic_dma_id);
	}
	aud_tras_drv_info.mic_info.mic_dma_id = DMA_ID_MAX;

	if (aud_tras_drv_info.mic_info.mic_ring_buff != NULL) {
		ring_buffer_clear(&(aud_tras_drv_info.mic_info.mic_rb));
		os_free(aud_tras_drv_info.mic_info.mic_ring_buff);
	}
	aud_tras_drv_info.mic_info.mic_ring_buff = NULL;

	if (aud_tras_drv_info.mic_info.temp_mic_addr != NULL)
		os_free(aud_tras_drv_info.mic_info.temp_mic_addr);
	aud_tras_drv_info.mic_info.temp_mic_addr = NULL;

	return BK_FAIL;
}

static bk_err_t aud_tras_drv_mic_deinit(void)
{
	/* disable audio adc */
	bk_aud_stop_adc();
	bk_dma_stop(aud_tras_drv_info.mic_info.mic_dma_id);
	bk_dma_deinit(aud_tras_drv_info.mic_info.mic_dma_id);
	bk_dma_free(DMA_DEV_AUDIO, aud_tras_drv_info.mic_info.mic_dma_id);
	bk_aud_adc_deinit();
	os_free(aud_tras_drv_info.mic_info.adc_config);
	aud_tras_drv_info.mic_info.adc_config = NULL;

	aud_tras_drv_info.mic_info.mic_en = false;
	aud_tras_drv_info.mic_info.mic_chl = AUD_INTF_MIC_CHL_MIC1;
	aud_tras_drv_info.mic_info.frame_size = 0;
	aud_tras_drv_info.mic_info.mic_dma_id = DMA_ID_MAX;

	ring_buffer_clear(&(aud_tras_drv_info.mic_info.mic_rb));
	os_free(aud_tras_drv_info.mic_info.mic_ring_buff);
	aud_tras_drv_info.mic_info.mic_ring_buff = NULL;

	os_free(aud_tras_drv_info.mic_info.temp_mic_addr);
	aud_tras_drv_info.mic_info.temp_mic_addr = NULL;

	aud_tras_drv_info.mic_info.status = AUD_TRAS_DRV_MIC_STA_NULL;

	return BK_OK;
}

static bk_err_t aud_tras_drv_mic_start(void)
{
	bk_err_t ret = BK_OK;
	if ((aud_tras_drv_info.mic_info.status != AUD_TRAS_DRV_MIC_STA_IDLE) && (aud_tras_drv_info.mic_info.status != AUD_TRAS_DRV_MIC_STA_PAUSE))
		return BK_FAIL;

	ret = bk_dma_start(aud_tras_drv_info.mic_info.mic_dma_id);
	if (ret != BK_OK) {
		os_printf("start mic dma fail \r\n");
		return BK_FAIL;
	}

	/* enable adc */
	bk_aud_start_adc();
	aud_tras_drv_info.mic_info.status = AUD_TRAS_DRV_MIC_STA_START;

	return BK_OK;
}

/* not stop dma, only disable adc */
static bk_err_t aud_tras_drv_mic_pause(void)
{
	//bk_err_t ret = BK_OK;
	if (aud_tras_drv_info.mic_info.status != AUD_TRAS_DRV_MIC_STA_START)
		return BK_FAIL;

	/* disable adc */
	bk_aud_stop_adc();
	aud_tras_drv_info.mic_info.status = AUD_TRAS_DRV_MIC_STA_PAUSE;

	return BK_OK;
}

static bk_err_t aud_tras_drv_mic_stop(void)
{
	//bk_err_t ret = BK_OK;
	if ((aud_tras_drv_info.mic_info.status != AUD_TRAS_DRV_MIC_STA_START) && (aud_tras_drv_info.mic_info.status != AUD_TRAS_DRV_MIC_STA_PAUSE))
		return BK_FAIL;

	/* disable adc */
	bk_aud_stop_adc();
	bk_dma_stop(aud_tras_drv_info.mic_info.mic_dma_id);
	ring_buffer_clear(&(aud_tras_drv_info.mic_info.mic_rb));
	aud_tras_drv_info.mic_info.status = AUD_TRAS_DRV_MIC_STA_IDLE;

	return BK_OK;
}

/*
static bk_err_t aud_tras_drv_mic_get_gain(uint8_t *value)
{
	CHECK_AUD_TRAS_DRV_MIC_STA();
	*value = aud_tras_drv_info.mic_info.adc_config->adc_set_gain;

	return BK_OK;
}
*/

static bk_err_t aud_tras_drv_set_mic_gain(uint8_t value)
{
	//CHECK_AUD_TRAS_DRV_MIC_STA();
	bk_aud_set_adc_gain((uint32_t)value);

	return BK_OK;
}

static bk_err_t aud_tras_drv_set_spk_gain(uint8_t value)
{
	//CHECK_AUD_TRAS_DRV_MIC_STA();
	bk_aud_set_dac_gain((uint32_t)value);

	return BK_OK;
}


/*
static bk_err_t aud_tras_drv_mic_get_samp_rate(aud_adc_samp_rate_t *samp_rate)
{
	CHECK_AUD_TRAS_DRV_MIC_STA();
	*samp_rate = aud_tras_drv_info.mic_info.adc_config->samp_rate;

	return BK_OK;
}
*/

static bk_err_t aud_tras_drv_mic_set_samp_rate(aud_adc_samp_rate_t samp_rate)
{
	CHECK_AUD_TRAS_DRV_MIC_STA();
	bk_aud_set_adc_samp_rate(samp_rate);

	return BK_OK;
}

static bk_err_t aud_tras_drv_mic_set_chl(aud_intf_mic_chl_t mic_chl)
{
	bk_err_t ret = BK_OK;

	CHECK_AUD_TRAS_DRV_MIC_STA();
	switch (aud_tras_drv_info.mic_info.status) {
		case AUD_TRAS_DRV_MIC_STA_IDLE:
			if (aud_tras_drv_info.mic_info.mic_chl != mic_chl) {
				//aud_intf_mic_chl_t mic_chl_bk = aud_tras_drv_info.mic_info.mic_chl;
				/* reconfig mic dma */
				ret = aud_tras_adc_dma_config(aud_tras_drv_info.mic_info.mic_dma_id, aud_tras_drv_info.mic_info.mic_ring_buff, aud_tras_drv_info.mic_info.frame_size * 2, aud_tras_drv_info.mic_info.frame_size, mic_chl);
				if (ret != BK_OK) {
					os_printf("set mic chl fail \r\n");
					return BK_FAIL;
				}
			}
			break;

		case AUD_TRAS_DRV_MIC_STA_START:
			break;

		case AUD_TRAS_DRV_MIC_STA_PAUSE:
			break;

		default:
			break;
	}

	//bk_aud_set_adc_samp_rate(samp_rate);

	return BK_OK;
}


/* audio transfer driver voice mode init */
static bk_err_t aud_tras_drv_voc_init(aud_intf_voc_config_t* voc_cfg)
{
	bk_err_t ret = BK_OK;

	/* get aec config */
	aud_tras_drv_info.voc_info.aec_enable = voc_cfg->aec_enable;
	if (aud_tras_drv_info.voc_info.aec_enable) {
		aud_tras_drv_info.voc_info.aec_info = os_malloc(sizeof(aec_info_t));
		if (aud_tras_drv_info.voc_info.aec_info == NULL) {
			os_printf("aec_context_t os_malloc fail \r\n");
		} else {
			aud_tras_drv_info.voc_info.aec_info->samp_rate = voc_cfg->samp_rate;
			//audio_transfer_context.aec_context->samp_rate_points
			aud_tras_drv_info.voc_info.aec_info->aec_config = os_malloc(sizeof(aec_config_t));
			if (aud_tras_drv_info.voc_info.aec_info->aec_config == NULL) {
				os_printf("aec_config_t os_malloc fail \r\n");
			} else {
				aud_tras_drv_info.voc_info.aec_info->aec_config->mic_delay = voc_cfg->aec_setup->init_flags;
				aud_tras_drv_info.voc_info.aec_info->aec_config->mic_delay = voc_cfg->aec_setup->mic_delay;
				aud_tras_drv_info.voc_info.aec_info->aec_config->ec_depth = voc_cfg->aec_setup->ec_depth;
				aud_tras_drv_info.voc_info.aec_info->aec_config->ref_scale = voc_cfg->aec_setup->ref_scale;
				aud_tras_drv_info.voc_info.aec_info->aec_config->voice_vol = voc_cfg->aec_setup->voice_vol;
				aud_tras_drv_info.voc_info.aec_info->aec_config->mic_delay = voc_cfg->aec_setup->TxRxThr;
				aud_tras_drv_info.voc_info.aec_info->aec_config->mic_delay = voc_cfg->aec_setup->TxRxFlr;
				aud_tras_drv_info.voc_info.aec_info->aec_config->ns_level = voc_cfg->aec_setup->ns_level;
				aud_tras_drv_info.voc_info.aec_info->aec_config->ns_para = voc_cfg->aec_setup->ns_para;
				aud_tras_drv_info.voc_info.aec_info->aec_config->drc = voc_cfg->aec_setup->drc;
			}
		}
	}
	else {
		aud_tras_drv_info.voc_info.aec_info = NULL;
	}

	aud_tras_drv_info.voc_info.data_type = voc_cfg->data_type;
	os_printf("aud_tras_drv_info.voc_info.data_type:%d \r\n", aud_tras_drv_info.voc_info.data_type);

	/* get audio adc config */
	aud_tras_drv_info.voc_info.adc_config = os_malloc(sizeof(aud_adc_config_t));
	if (aud_tras_drv_info.voc_info.adc_config == NULL) {
		os_printf("adc_config os_malloc fail \r\n");
	} else {
		aud_tras_drv_info.voc_info.adc_config->mic_config = AUD_MIC_MIC1_ENABLE;
		aud_tras_drv_info.voc_info.adc_config->samp_rate = voc_cfg->samp_rate;
		aud_tras_drv_info.voc_info.adc_config->adc_enable = AUD_ADC_DISABLE;
		aud_tras_drv_info.voc_info.adc_config->line_enable = AUD_ADC_LINE_DISABLE;
		aud_tras_drv_info.voc_info.adc_config->dtmf_enable = AUD_DTMF_DISABLE;
		aud_tras_drv_info.voc_info.adc_config->adc_hpf2_coef_B2 = 0;
		aud_tras_drv_info.voc_info.adc_config->adc_hpf2_bypass_enable = AUD_ADC_HPF_BYPASS_ENABLE;
		aud_tras_drv_info.voc_info.adc_config->adc_hpf1_bypass_enable = AUD_ADC_HPF_BYPASS_ENABLE;
		aud_tras_drv_info.voc_info.adc_config->adc_set_gain = voc_cfg->aud_setup.adc_gain;	//default: 0x2d
		aud_tras_drv_info.voc_info.adc_config->adc_samp_edge = AUD_ADC_SAMP_EDGE_RISING;
		aud_tras_drv_info.voc_info.adc_config->adc_hpf2_coef_B0 = 0;
		aud_tras_drv_info.voc_info.adc_config->adc_hpf2_coef_B1 = 0;
		aud_tras_drv_info.voc_info.adc_config->adc_hpf2_coef_A0 = 0;
		aud_tras_drv_info.voc_info.adc_config->adc_hpf2_coef_A1 = 0;
		aud_tras_drv_info.voc_info.adc_config->dtmf_wr_threshold = 8;
		aud_tras_drv_info.voc_info.adc_config->adcl_wr_threshold = 8;
		aud_tras_drv_info.voc_info.adc_config->dtmf_int_enable = AUD_DTMF_INT_DISABLE;
		aud_tras_drv_info.voc_info.adc_config->adcl_int_enable = AUD_ADCL_INT_DISABLE;
		aud_tras_drv_info.voc_info.adc_config->loop_adc2dac = AUD_LOOP_ADC2DAC_DISABLE;
		aud_tras_drv_info.voc_info.adc_config->agc_noise_thrd = 101;
		aud_tras_drv_info.voc_info.adc_config->agc_noise_high = 101;
		aud_tras_drv_info.voc_info.adc_config->agc_noise_low = 160;
		aud_tras_drv_info.voc_info.adc_config->agc_noise_min = 1;
		aud_tras_drv_info.voc_info.adc_config->agc_noise_tout = 0;
		aud_tras_drv_info.voc_info.adc_config->agc_high_dur = 3;
		aud_tras_drv_info.voc_info.adc_config->agc_low_dur = 3;
		aud_tras_drv_info.voc_info.adc_config->agc_min = 1;
		aud_tras_drv_info.voc_info.adc_config->agc_max = 4;
		aud_tras_drv_info.voc_info.adc_config->agc_ng_method = AUD_AGC_NG_METHOD_MUTE;
		aud_tras_drv_info.voc_info.adc_config->agc_ng_enable = AUD_AGC_NG_DISABLE;
		aud_tras_drv_info.voc_info.adc_config->agc_decay_time = AUD_AGC_DECAY_TIME_128;
		aud_tras_drv_info.voc_info.adc_config->agc_attack_time = AUD_AGC_ATTACK_TIME_128;
		aud_tras_drv_info.voc_info.adc_config->agc_high_thrd = 18;
		aud_tras_drv_info.voc_info.adc_config->agc_low_thrd = 0;
		aud_tras_drv_info.voc_info.adc_config->agc_iir_coef = AUD_AGC_IIR_COEF_1_1024;
		aud_tras_drv_info.voc_info.adc_config->agc_enable = AUD_AGC_DISABLE;
		aud_tras_drv_info.voc_info.adc_config->manual_pga_value = 0;
		aud_tras_drv_info.voc_info.adc_config->manual_pga_enable = AUD_GAC_MANUAL_PGA_DISABLE;
		aud_tras_drv_info.voc_info.adc_config->adc_fracmod_manual = AUD_ADC_TRACMOD_MANUAL_DISABLE;
		aud_tras_drv_info.voc_info.adc_config->adc_fracmod = 0;
	}

	/* get audio dac config */
	aud_tras_drv_info.voc_info.dac_config = os_malloc(sizeof(aud_dac_config_t));
	if (aud_tras_drv_info.voc_info.adc_config == NULL) {
		os_printf("dac_config os_malloc fail \r\n");
	} else {
		aud_tras_drv_info.voc_info.dac_config->dac_enable = AUD_DAC_DISABLE;
		aud_tras_drv_info.voc_info.dac_config->samp_rate = voc_cfg->samp_rate;
		aud_tras_drv_info.voc_info.dac_config->dac_hpf2_coef_B2 = 0x3A22;
		aud_tras_drv_info.voc_info.dac_config->dac_hpf2_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
		aud_tras_drv_info.voc_info.dac_config->dac_hpf1_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
		aud_tras_drv_info.voc_info.dac_config->dac_set_gain = voc_cfg->aud_setup.dac_gain;	//default 2D  3F  15
		aud_tras_drv_info.voc_info.dac_config->dac_clk_invert = AUD_DAC_CLK_INVERT_RISING;
		aud_tras_drv_info.voc_info.dac_config->dac_hpf2_coef_B0 = 0x3A22;
		aud_tras_drv_info.voc_info.dac_config->dac_hpf2_coef_B1 = 0x8BBF;
		aud_tras_drv_info.voc_info.dac_config->dac_hpf2_coef_A1 = 0x751C;
		aud_tras_drv_info.voc_info.dac_config->dac_hpf2_coef_A2 = 0xC9E6;
		aud_tras_drv_info.voc_info.dac_config->dacr_rd_threshold = 0x4;
		aud_tras_drv_info.voc_info.dac_config->dacl_rd_threshold = 0x4;
		aud_tras_drv_info.voc_info.dac_config->dacr_int_enable = 0x0;
		aud_tras_drv_info.voc_info.dac_config->dacl_int_enable = 0x0;
		aud_tras_drv_info.voc_info.dac_config->dac_filt_enable = AUD_DAC_FILT_DISABLE;
		aud_tras_drv_info.voc_info.dac_config->dac_fracmod_manual_enable = AUD_DAC_FRACMOD_MANUAL_DISABLE;
		aud_tras_drv_info.voc_info.dac_config->dac_fracmode_value = 0x0;
	}

	/* get ring buffer config */
	//aud_tras_drv_info.voc_info.mode = setup->aud_trs_mode;
	if (aud_tras_drv_info.voc_info.aec_enable) {
		switch (voc_cfg->samp_rate) {
			case AUD_INTF_VOC_SAMP_RATE_8K:
				aud_tras_drv_info.voc_info.mic_samp_rate_points = 160;
				aud_tras_drv_info.voc_info.speaker_samp_rate_points = 160;
				break;

			case AUD_INTF_VOC_SAMP_RATE_16K:
				aud_tras_drv_info.voc_info.mic_samp_rate_points = 320;
				aud_tras_drv_info.voc_info.speaker_samp_rate_points = 320;
				break;

			default:
				break;
		}
	} else {
		aud_tras_drv_info.voc_info.mic_samp_rate_points = voc_cfg->aud_setup.mic_samp_rate_points;
		aud_tras_drv_info.voc_info.speaker_samp_rate_points = voc_cfg->aud_setup.speaker_samp_rate_points;
	}
	aud_tras_drv_info.voc_info.mic_frame_number = voc_cfg->aud_setup.mic_frame_number;
	aud_tras_drv_info.voc_info.speaker_frame_number = voc_cfg->aud_setup.speaker_frame_number;

	/* get tx and rx context config */
	aud_tras_drv_info.voc_info.tx_info = voc_cfg->tx_info;
	aud_tras_drv_info.voc_info.rx_info = voc_cfg->rx_info;

	/* callback config */
	aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb = voc_cfg->aud_tras_drv_voc_event_cb;
//	aud_tras_drv_info.voc_info.aud_cb.audio_send_mic_data = voc_cfg->aud_cb.audio_send_mic_data;

#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
	/* get dump config */
	aud_tras_drv_info.voc_info.aec_dump.len = voc_cfg->aec_dump.len;
	aud_tras_drv_info.voc_info.aec_dump.mic_dump_addr = voc_cfg->aec_dump.mic_dump_addr;
	aud_tras_drv_info.voc_info.aec_dump.ref_dump_addr = voc_cfg->aec_dump.ref_dump_addr;
	aud_tras_drv_info.voc_info.aec_dump.out_dump_addr = voc_cfg->aec_dump.out_dump_addr;
#endif

#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
	aud_tras_drv_info.voc_info.lost_count.complete_size = 0;
	aud_tras_drv_info.voc_info.lost_count.lost_size = 0;
#endif

	/*  -------------------------step1: init audio and config ADC and DAC -------------------------------- */
	/* config mailbox according audio transfer work mode */
#if CONFIG_AUD_TRAS_MODE_CPU1
	//audio_transfer_mailbox_init();
	/* init audio maibox channel */
	aud_tras_mb_init(audio_mailbox_rx_isr, audio_mailbox_tx_isr, audio_mailbox_tx_cmpl_isr);
	os_printf("step1: config audio mailbox channel complete \r\n");
#endif

	/*  -------------------------step2: init audio and config ADC and DAC -------------------------------- */
	ret = aud_tras_adc_config(aud_tras_drv_info.voc_info.adc_config);
	if (ret != BK_OK) {
		os_printf("audio adc init fail \r\n");
		goto aud_tras_drv_voc_init_exit;
	}

	ret = aud_tras_dac_config(aud_tras_drv_info.voc_info.dac_config);
	if (ret != BK_OK) {
		os_printf("audio dac init fail \r\n");
		goto aud_tras_drv_voc_init_exit;
	}
	os_printf("step2: init audio and config ADC and DAC complete \r\n");

	/*  -------------------------step3: init AEC and malloc two ring buffers -------------------------------- */
	/* init aec and config aec according AEC_enable*/
	if (aud_tras_drv_info.voc_info.aec_enable) {
		audio_transfer_aec_config();
		os_printf("aec samp_rate_points: %d \r\n", aud_tras_drv_info.voc_info.aec_info->samp_rate_points);
		ret = audio_transfer_aec_buff_config(aud_tras_drv_info.voc_info.aec_info);
		if (ret != BK_OK)
			goto aud_tras_drv_voc_init_exit;
		os_printf("step3: init AEC and malloc two ring buffers complete \r\n");
	}

	/*  -------------------step4: init and config DMA to carry mic and ref data ----------------------------- */
	/* init dma driver */
	ret = bk_dma_driver_init();
	if (ret != BK_OK) {
		os_printf("dma driver init failed\r\n");
		goto aud_tras_drv_voc_init_exit;
	}

	/* allocate free DMA channel */
	aud_tras_drv_info.voc_info.adc_dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
	if ((aud_tras_drv_info.voc_info.adc_dma_id < DMA_ID_0) || (aud_tras_drv_info.voc_info.adc_dma_id >= DMA_ID_MAX)) {
		os_printf("malloc adc dma fail \r\n");
		goto aud_tras_drv_voc_init_exit;
	}

	aud_tras_drv_info.voc_info.mic_ring_buff = (int32_t *)os_malloc(aud_tras_drv_info.voc_info.mic_samp_rate_points*2*aud_tras_drv_info.voc_info.mic_frame_number);
	if (aud_tras_drv_info.voc_info.mic_ring_buff == NULL) {
		os_printf("malloc mic ring buffer fail \r\n");
		goto aud_tras_drv_voc_init_exit;
	}

	/* config audio adc dma to carry mic data to "mic_ring_buff" */
	ret = aud_tras_adc_dma_config(aud_tras_drv_info.voc_info.adc_dma_id, aud_tras_drv_info.voc_info.mic_ring_buff, (aud_tras_drv_info.voc_info.mic_samp_rate_points*2)*aud_tras_drv_info.voc_info.mic_frame_number, aud_tras_drv_info.voc_info.mic_samp_rate_points*2, AUD_INTF_MIC_CHL_MIC1);
	if (ret != BK_OK) {
		os_printf("config audio adc dma fail \r\n");
		goto aud_tras_drv_voc_init_exit;
	}
	os_printf("step4: init and config mic DMA complete, adc_dma_id:%d, mic_ring_buff:%p, size:%d, carry_length:%d \r\n", aud_tras_drv_info.voc_info.adc_dma_id, aud_tras_drv_info.voc_info.mic_ring_buff, (aud_tras_drv_info.voc_info.mic_samp_rate_points*2)*aud_tras_drv_info.voc_info.mic_frame_number, aud_tras_drv_info.voc_info.mic_samp_rate_points*2);

	/*  -------------------step5: init and config DMA to carry dac data ----------------------------- */
	/* init dma driver */
	ret = bk_dma_driver_init();
	if (ret != BK_OK) {
		os_printf("dma driver init failed\r\n");
		goto aud_tras_drv_voc_init_exit;
	}

	/* allocate free DMA channel */
	aud_tras_drv_info.voc_info.dac_dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
	if ((aud_tras_drv_info.voc_info.dac_dma_id < DMA_ID_0) || (aud_tras_drv_info.voc_info.dac_dma_id >= DMA_ID_MAX)) {
		os_printf("malloc dac dma fail \r\n");
		goto aud_tras_drv_voc_init_exit;
	}

	aud_tras_drv_info.voc_info.speaker_ring_buff = (int32_t *)os_malloc(aud_tras_drv_info.voc_info.speaker_samp_rate_points*2*aud_tras_drv_info.voc_info.speaker_frame_number);
	if (aud_tras_drv_info.voc_info.speaker_ring_buff == NULL) {
		os_printf("malloc speaker ring buffer fail \r\n");
		goto aud_tras_drv_voc_init_exit;
	}

	/* config audio dac dma to carry dac data to "speaker_ring_buff" */
	ret = aud_tras_dac_dma_config(aud_tras_drv_info.voc_info.dac_dma_id, aud_tras_drv_info.voc_info.speaker_ring_buff, (aud_tras_drv_info.voc_info.speaker_samp_rate_points*2)*aud_tras_drv_info.voc_info.speaker_frame_number, aud_tras_drv_info.voc_info.speaker_samp_rate_points*2, AUD_INTF_SPK_CHL_SPK1);
	if (ret != BK_OK) {
		os_printf("config audio adc dma fail \r\n");
		goto aud_tras_drv_voc_init_exit;
	}
	os_printf("step5: init and config speaker DMA complete, dac_dma_id:%d, speaker_ring_buff:%p, size:%d, carry_length:%d \r\n", aud_tras_drv_info.voc_info.dac_dma_id, aud_tras_drv_info.voc_info.speaker_ring_buff, (aud_tras_drv_info.voc_info.speaker_samp_rate_points*2)*aud_tras_drv_info.voc_info.speaker_frame_number, aud_tras_drv_info.voc_info.speaker_samp_rate_points*2);

	/*  -------------------------step6: init all audio ring buffers -------------------------------- */
	/* init mic_ring_buff */
	//ring_buffer_init(&mic_rb, (uint8_t*)mic_ring_buff, frame_sample*2*2, adc_dma_id, RB_DMA_TYPE_WRITE);
	ring_buffer_init(&(aud_tras_drv_info.voc_info.mic_rb), (uint8_t*)aud_tras_drv_info.voc_info.mic_ring_buff, aud_tras_drv_info.voc_info.mic_samp_rate_points*2*aud_tras_drv_info.voc_info.mic_frame_number, aud_tras_drv_info.voc_info.adc_dma_id, RB_DMA_TYPE_WRITE);

	/* init speaker_ref_ring_buff */
	//ring_buffer_init(&speaker_rb, (uint8_t*)speaker_ring_buff, PSRAM_AUD_DECD_RING_BUFF_SIZE*2, dac_dma_id, RB_DMA_TYPE_READ);
	ring_buffer_init(&(aud_tras_drv_info.voc_info.speaker_rb), (uint8_t*)aud_tras_drv_info.voc_info.speaker_ring_buff, aud_tras_drv_info.voc_info.speaker_samp_rate_points*2*aud_tras_drv_info.voc_info.speaker_frame_number, aud_tras_drv_info.voc_info.dac_dma_id, RB_DMA_TYPE_READ);
	os_printf("step6: init all audio ring buffers complete \r\n");

	/*  -------------------------step6: init all audio ring buffers -------------------------------- */
	/* init encoder and decoder temp buffer */
	aud_tras_drv_info.voc_info.encoder_temp.pcm_data = (int16_t *)os_malloc(aud_tras_drv_info.voc_info.mic_samp_rate_points * 2);
	if (aud_tras_drv_info.voc_info.encoder_temp.pcm_data == NULL) {
		os_printf("malloc pcm_data of encoder used fail \r\n");
		goto aud_tras_drv_voc_init_exit;
	}

	aud_tras_drv_info.voc_info.decoder_temp.pcm_data = (int16_t *)os_malloc(aud_tras_drv_info.voc_info.speaker_samp_rate_points*2);
	if (aud_tras_drv_info.voc_info.decoder_temp.pcm_data == NULL) {
		os_printf("malloc pcm_data of decoder used fail \r\n");
		goto aud_tras_drv_voc_init_exit;
	}

	switch (aud_tras_drv_info.voc_info.data_type) {
		case AUD_INTF_VOC_DATA_TYPE_G711A:
			os_printf("need to malloc law_data temp buffer \r\n");
			aud_tras_drv_info.voc_info.encoder_temp.law_data = (uint8_t *)os_malloc(aud_tras_drv_info.voc_info.mic_samp_rate_points);
			if (aud_tras_drv_info.voc_info.encoder_temp.law_data == NULL) {
				os_printf("malloc law_data of encoder used fail \r\n");
				goto aud_tras_drv_voc_init_exit;
			}

			aud_tras_drv_info.voc_info.decoder_temp.law_data = (unsigned char *)os_malloc(aud_tras_drv_info.voc_info.speaker_samp_rate_points);
			if (aud_tras_drv_info.voc_info.decoder_temp.law_data == NULL) {
				os_printf("malloc law_data of decoder used fail \r\n");
				goto aud_tras_drv_voc_init_exit;
			}
			break;

		case AUD_INTF_VOC_DATA_TYPE_PCM:
			os_printf("not need to malloc law_data temp buffer \r\n");
			break;

		default:
			break;
	}


	/* change status: AUD_TRAS_DRV_VOC_NULL --> AUD_TRAS_DRV_VOC_IDLE */
	aud_tras_drv_info.voc_info.status = AUD_TRAS_DRV_VOC_STA_IDLE;

	os_printf("init complete \r\n");

#if CONFIG_AUD_TRAS_MODE_CPU0
	/* audio transfer init callback */
	ret = BK_OK;
	if (aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb)
		aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb(EVENT_AUD_TRAS_VOC_INIT, (void *)&ret);
#endif

#if CONFIG_AUD_TRAS_MODE_CPU1
	ret = media_send_com_mb_msg(CMD_COM_MB_AUD_TRAS_INIT_CMPL, 0, 0, 0);
	if (ret != BK_OK) {
		os_printf("send media common mailbox:CMD_COM_MB_AUD_TRAS_INIT_CMPL fail \r\n");
		goto aud_tras_drv_voc_init_exit;
	}
#endif

	return BK_OK;

aud_tras_drv_voc_init_exit:
	/* audio transfer driver deconfig */
	//aud_tras_drv_voc_deinit();
	//TODO
	return BK_FAIL;
}

static void aud_tras_drv_voc_deinit(void)
{
#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
		bk_timer_stop(TIMER_ID4);
#endif

	/* disable audio adc and dac */
	bk_aud_stop_adc();
	bk_aud_adc_deinit();
	bk_aud_stop_dac();
	bk_aud_dac_deinit();
	bk_aud_driver_deinit();
	os_free(aud_tras_drv_info.voc_info.adc_config);
	aud_tras_drv_info.voc_info.adc_config = NULL;
	os_free(aud_tras_drv_info.voc_info.dac_config);
	aud_tras_drv_info.voc_info.dac_config = NULL;

	/* stop dma */
	bk_dma_stop(aud_tras_drv_info.voc_info.adc_dma_id);
	bk_dma_stop(aud_tras_drv_info.voc_info.dac_dma_id);
	bk_dma_deinit(aud_tras_drv_info.voc_info.adc_dma_id);
	bk_dma_deinit(aud_tras_drv_info.voc_info.dac_dma_id);
	bk_dma_free(DMA_DEV_AUDIO, aud_tras_drv_info.voc_info.adc_dma_id);
	bk_dma_free(DMA_DEV_AUDIO, aud_tras_drv_info.voc_info.dac_dma_id);

	/* disable AEC */
	audio_transfer_aec_deconfig();

	/* free audio ring buffer */
	//mic deconfig
	ring_buffer_clear(&(aud_tras_drv_info.voc_info.mic_rb));
	os_free(aud_tras_drv_info.voc_info.mic_ring_buff);
	aud_tras_drv_info.voc_info.mic_ring_buff = NULL;
	aud_tras_drv_info.voc_info.mic_samp_rate_points = 0;
	aud_tras_drv_info.voc_info.mic_frame_number = 0;
	aud_tras_drv_info.voc_info.adc_dma_id = DMA_ID_MAX;
	//speaker deconfig
	ring_buffer_clear(&(aud_tras_drv_info.voc_info.speaker_rb));
	os_free(aud_tras_drv_info.voc_info.speaker_ring_buff);
	aud_tras_drv_info.voc_info.speaker_ring_buff = NULL;
	aud_tras_drv_info.voc_info.speaker_samp_rate_points = 0;
	aud_tras_drv_info.voc_info.speaker_frame_number = 0;
	aud_tras_drv_info.voc_info.dac_dma_id = DMA_ID_MAX;

	/* tx and rx deconfig */
	//tx deconfig
	aud_tras_drv_info.voc_info.tx_info.tx_buff_status = false;
	aud_tras_drv_info.voc_info.tx_info.buff_length = 0;
	aud_tras_drv_info.voc_info.tx_info.ping.busy_status = false;
	aud_tras_drv_info.voc_info.tx_info.ping.buff_addr = NULL;
	//rx deconfig
	aud_tras_drv_info.voc_info.rx_info.rx_buff_status = false;
	aud_tras_drv_info.voc_info.rx_info.decoder_ring_buff = NULL;
	aud_tras_drv_info.voc_info.rx_info.decoder_rb = NULL;
	aud_tras_drv_info.voc_info.rx_info.frame_size = 0;
	aud_tras_drv_info.voc_info.rx_info.frame_num = 0;
	aud_tras_drv_info.voc_info.rx_info.rx_buff_seq_tail = 0;
	aud_tras_drv_info.voc_info.rx_info.aud_trs_read_seq = 0;
	aud_tras_drv_info.voc_info.rx_info.fifo_frame_num = 0;

	/* encoder_temp and decoder_temp deconfig*/
	switch (aud_tras_drv_info.voc_info.data_type) {
		case AUD_INTF_VOC_DATA_TYPE_G711A:
			os_free(aud_tras_drv_info.voc_info.encoder_temp.law_data);
			aud_tras_drv_info.voc_info.encoder_temp.law_data = NULL;
			break;

		case AUD_INTF_VOC_DATA_TYPE_PCM:
			os_free(aud_tras_drv_info.voc_info.decoder_temp.law_data);
			aud_tras_drv_info.voc_info.decoder_temp.law_data = NULL;
			break;

		default:
			break;
	}
	os_free(aud_tras_drv_info.voc_info.encoder_temp.pcm_data);
	aud_tras_drv_info.voc_info.encoder_temp.pcm_data = NULL;
	os_free(aud_tras_drv_info.voc_info.decoder_temp.pcm_data);
	aud_tras_drv_info.voc_info.decoder_temp.pcm_data = NULL;

	aud_tras_drv_info.voc_info.data_type = AUD_INTF_VOC_DATA_TYPE_PCM;

	/* notify audio transfer task to stop test */
#if CONFIG_AUD_TRAS_MODE_CPU0
	if (aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb)
		aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb(EVENT_AUD_TRAS_VOC_DEINIT, NULL);
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
	aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb = NULL;
//	aud_tras_drv_info.voc_info.aud_cb.audio_send_mic_data = NULL;

	/* change status: 
				AUD_TRAS_DRV_VOC_IDLE --> AUD_TRAS_DRV_VOC_NULL
				AUD_TRAS_DRV_VOC_START --> AUD_TRAS_DRV_VOC_NULL
				AUD_TRAS_DRV_VOC_STOP --> AUD_TRAS_DRV_VOC_NULL
	*/
	aud_tras_drv_info.voc_info.status = AUD_TRAS_DRV_VOC_STA_NULL;

}

static void aud_tras_drv_voc_start(void)
{
	bk_err_t ret = BK_OK;
	uint32_t size = 0;
	//uint8_t pcm_data[audio_transfer_context.mic_samp_rate_points*2]= {0};
	uint8_t *pcm_data = NULL;

	os_printf("%s \r\n", __func__);

	pcm_data = (uint8_t *)os_malloc(aud_tras_drv_info.voc_info.mic_samp_rate_points*2);
	if (pcm_data == NULL) {
		os_printf("malloc temp pcm_data fial \r\n");
		goto audio_start_transfer_exit;
	} else {
		os_memset(pcm_data, 0x00, aud_tras_drv_info.voc_info.mic_samp_rate_points*2);
	}

	/* start DMA */
	ret = bk_dma_start(aud_tras_drv_info.voc_info.adc_dma_id);
	if (ret != BK_OK) {
		os_printf("start adc dma fail \r\n");
		goto audio_start_transfer_exit;
	}

	/* enable adc */
	/* wait receive data and then open adc */
	bk_aud_start_adc();

	/* enable dac */
	bk_aud_start_dac();

	ret = bk_dma_start(aud_tras_drv_info.voc_info.dac_dma_id);
	if (ret != BK_OK) {
		os_printf("start dac dma fail \r\n");
		goto audio_start_transfer_exit;
	}
	/* write two frame data to speaker and ref ring buffer */
	size = ring_buffer_write(&(aud_tras_drv_info.voc_info.speaker_rb), (uint8_t *)pcm_data, aud_tras_drv_info.voc_info.mic_samp_rate_points*2);
	if (size != aud_tras_drv_info.voc_info.mic_samp_rate_points*2) {
		os_printf("the data writeten to speaker_ring_buff error, size=%d \r\n", size);
		goto audio_start_transfer_exit;
	}

	size = ring_buffer_write(&(aud_tras_drv_info.voc_info.speaker_rb), (uint8_t *)pcm_data, aud_tras_drv_info.voc_info.mic_samp_rate_points*2);
	if (size != aud_tras_drv_info.voc_info.mic_samp_rate_points*2) {
		os_printf("the data writeten to speaker_ring_buff error, size=%d \r\n", size);
		goto audio_start_transfer_exit;
	}

	/* notify cpu0 that audio transfer start */
#if CONFIG_AUD_TRAS_MODE_CPU0
//	switch (audio_transfer_context.mode) {
//		case AUD_TRAS_DRV_MODE_CPU0:
			if (aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb)
				aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb(EVENT_AUD_TRAS_VOC_START, NULL);
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

	/* change status: 
				AUD_TRAS_DRV_VOC_IDLE --> AUD_TRAS_DRV_VOC_START
				AUD_TRAS_DRV_VOC_STOP --> AUD_TRAS_DRV_VOC_START
	*/
	aud_tras_drv_info.voc_info.status = AUD_TRAS_DRV_VOC_START;

#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
		//bk_timer_driver_init();
		ret = bk_timer_start(TIMER_ID4, AUD_LOST_DEBUG_INTERVAL, aud_tras_drv_tx_lost_count_dump);
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

static bk_err_t aud_tras_drv_voc_stop(void)
{
	bk_err_t ret = BK_OK;

	os_printf("%s \r\n", __func__);

	/* stop adc and dac dma */
	ret = bk_dma_stop(aud_tras_drv_info.voc_info.adc_dma_id);
	if (ret != BK_OK) {
		os_printf("start adc dma fail \r\n");
	}
	ret = bk_dma_stop(aud_tras_drv_info.voc_info.dac_dma_id);
	if (ret != BK_OK) {
		os_printf("stop dac dma fail \r\n");
	}

	/* disable adc */
	bk_aud_stop_adc();

	/* disable dac */
	bk_aud_stop_dac();

	/* clear adc and dac ring buffer */
	ring_buffer_clear(&(aud_tras_drv_info.voc_info.speaker_rb));
	ring_buffer_clear(&(aud_tras_drv_info.voc_info.mic_rb));

	/* notify cpu0 that audio transfer start */
#if CONFIG_AUD_TRAS_MODE_CPU0
	if (aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb)
		aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb(EVENT_AUD_TRAS_VOC_STOP, NULL);
#endif

#if CONFIG_AUD_TRAS_MODE_CPU1
	ret = audio_send_aud_mailbox_msg(AUD_TRAS_MB_CMD_START_TRANSFER_DONE, 0, 0, 0);
	if (ret != BK_OK) {
		os_printf("send mailbox msg: AUD_TRAS_MB_CMD_START_TRANSFER_DONE fail \r\n");
	}
#endif

	/* change status: 
				AUD_TRAS_DRV_VOC_IDLE --> AUD_TRAS_DRV_VOC_STOP
				AUD_TRAS_DRV_VOC_STOP --> AUD_TRAS_DRV_VOC_STOP
	*/
	aud_tras_drv_info.voc_info.status = AUD_TRAS_DRV_VOC_STOP;

	os_printf("stop voice transfer complete \r\n");

	return ret;
}

bk_err_t aud_tras_drv_set_work_mode(aud_intf_work_mode_t mode)
{
	bk_err_t ret = BK_OK;

	os_printf("set mode: %d \r\n", mode);

	if (aud_tras_drv_info.work_mode == mode)
		return BK_OK;

	switch (mode) {
		case AUD_INTF_WORK_MODE_GENERAL:
			if (aud_tras_drv_info.work_mode == AUD_INTF_WORK_MODE_VOICE) {
				aud_tras_drv_voc_deinit();
			}
			aud_tras_drv_info.work_mode = AUD_INTF_WORK_MODE_GENERAL;
			break;

		case AUD_INTF_WORK_MODE_VOICE:
			if (aud_tras_drv_info.work_mode == AUD_INTF_WORK_MODE_GENERAL) {
				if (aud_tras_drv_info.mic_info.status != AUD_TRAS_DRV_MIC_STA_NULL) {
					ret = aud_tras_drv_mic_deinit();
					if (ret != BK_OK) {
						os_printf("mic deinit fail \r\n");
						return BK_FAIL;
					}
				}
				if (aud_tras_drv_info.spk_info.status != AUD_TRAS_DRV_SPK_STA_NULL) {
					ret = aud_tras_drv_spk_deinit();
					if (ret != BK_OK) {
						os_printf("spk deinit fail \r\n");
						return BK_FAIL;
					}
				}
			}
			aud_tras_drv_info.work_mode = AUD_INTF_WORK_MODE_VOICE;
			break;

		case AUD_INTF_WORK_MODE_NULL:
			if (aud_tras_drv_info.work_mode == AUD_INTF_WORK_MODE_VOICE) {
				aud_tras_drv_voc_deinit();
			}
			if (aud_tras_drv_info.work_mode == AUD_INTF_WORK_MODE_GENERAL) {
				if (aud_tras_drv_info.mic_info.status != AUD_TRAS_DRV_MIC_STA_NULL) {
					ret = aud_tras_drv_mic_deinit();
					if (ret != BK_OK) {
						os_printf("mic deinit fail \r\n");
						return BK_FAIL;
					}
				}
				if (aud_tras_drv_info.spk_info.status != AUD_TRAS_DRV_SPK_STA_NULL) {
					ret = aud_tras_drv_spk_deinit();
					if (ret != BK_OK) {
						os_printf("spk deinit fail \r\n");
						return BK_FAIL;
					}
				}
			}
			aud_tras_drv_info.work_mode = AUD_INTF_WORK_MODE_NULL;
			break;

		default:
			break;
	}

	return BK_OK;
}

static void aud_tras_drv_main(beken_thread_arg_t param_data)
{
	bk_err_t ret = BK_OK;
	aud_intf_drv_setup_t *aud_trs_setup = NULL;

	aud_trs_setup = (aud_intf_drv_setup_t *)param_data;

	aud_tras_drv_info.work_mode = aud_trs_setup->work_mode;
	aud_tras_drv_info.aud_tras_tx_mic_data = aud_trs_setup->aud_intf_tx_mic_data;
	aud_tras_drv_info.aud_tras_rx_spk_data = aud_trs_setup->aud_intf_rx_spk_data;

	/* set work status to IDLE */
	aud_tras_drv_info.status = AUD_TRAS_DRV_STA_IDLE;

	while(1) {
		aud_tras_drv_msg_t msg;
		ret = rtos_pop_from_queue(&aud_trs_drv_int_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			switch (msg.op) {
				case AUD_TRAS_DRV_IDLE:
					break;

				case AUD_TRAS_DRV_EXIT:
					os_printf("goto: AUD_TRAS_DRV_EXIT \r\n");
					goto aud_tras_drv_exit;
					break;

				case AUD_TRAS_DRV_SET_MODE:
					os_printf("goto: AUD_TRAS_DRV_SET_MODE \r\n");
					aud_tras_drv_set_work_mode(*(aud_intf_work_mode_t *)msg.param);
					break;

				/* mic op */
				case AUD_TRAS_DRV_MIC_INIT:
					os_printf("goto: AUD_TRAS_DRV_MIC_INIT \r\n");
					aud_tras_drv_mic_init((aud_intf_mic_config_t *)msg.param);
					break;

				case AUD_TRAS_DRV_MIC_DEINIT:
					os_printf("goto: AUD_TRAS_DRV_MIC_DEINIT \r\n");
					aud_tras_drv_mic_deinit();
					break;

				case AUD_TRAS_DRV_MIC_START:
					os_printf("goto: AUD_TRAS_DRV_MIC_START \r\n");
					aud_tras_drv_mic_start();
					break;

				case AUD_TRAS_DRV_MIC_PAUSE:
					aud_tras_drv_mic_pause();
					break;

				case AUD_TRAS_DRV_MIC_STOP:
					os_printf("goto: AUD_TRAS_DRV_MIC_STOP \r\n");
					aud_tras_drv_mic_stop();
					break;

				case AUD_TRAS_DRV_MIC_SET_CHL:
					aud_tras_drv_mic_set_chl(*((aud_intf_mic_chl_t *)msg.param));
					break;

				case AUD_TRAS_DRV_MIC_SET_SAMP_RATE:
					aud_tras_drv_mic_set_samp_rate(*((aud_adc_samp_rate_t *)msg.param));
					break;

				case AUD_TRAS_DRV_MIC_SET_GAIN:
					os_printf("goto: AUD_TRAS_DRV_MIC_SET_GAIN \r\n");
					aud_tras_drv_set_mic_gain(*((uint8_t *)msg.param));
					break;

				case AUD_TRAS_DRV_MIC_TX_DATA:
					//os_printf("goto: AUD_TRAS_DRV_MIC_TX_DATA \r\n");
					aud_tras_drv_mic_tx_data();
					break;

				/* spk op */
				case AUD_TRAS_DRV_SPK_INIT:
					os_printf("goto: AUD_TRAS_DRV_SPK_INIT \r\n");
					aud_tras_drv_spk_init((aud_intf_spk_config_t *)msg.param);
					break;

				case AUD_TRAS_DRV_SPK_DEINIT:
					aud_tras_drv_spk_deinit();
					break;

				case AUD_TRAS_DRV_SPK_START:
					os_printf("goto: AUD_TRAS_DRV_SPK_START \r\n");
					aud_tras_drv_spk_start();
					break;

				case AUD_TRAS_DRV_SPK_PAUSE:
					aud_tras_drv_spk_pause();
					break;

				case AUD_TRAS_DRV_SPK_STOP:
					aud_tras_drv_spk_stop();
					break;

				case AUD_TRAS_DRV_SPK_SET_CHL:
					//aud_tras_drv_spk_set_chl();
					break;

				case AUD_TRAS_DRV_SPK_SET_SAMP_RATE:
					break;

				case AUD_TRAS_DRV_SPK_SET_GAIN:
					os_printf("goto: AUD_TRAS_DRV_SPK_SET_GAIN \r\n");
					aud_tras_drv_set_spk_gain(*((uint8_t *)msg.param));
					break;

				case AUD_TRAS_DRV_SPK_REQ_DATA:
					//os_printf("goto: AUD_TRAS_DRV_SPK_REQ_DATA \r\n");
					aud_tras_drv_spk_req_data();
					break;

				/* voc op */
				case AUD_TRAS_DRV_VOC_INIT:
					os_printf("goto: AUD_TRAS_DRV_VOC_INIT \r\n");
					aud_tras_drv_voc_init((aud_intf_voc_config_t*)msg.param);
					break;

				case AUD_TRAS_DRV_VOC_DEINIT:
					os_printf("goto: AUD_TRAS_DRV_VOC_DEINIT \r\n");
					aud_tras_drv_voc_deinit();
					break;

				case AUD_TRAS_DRV_VOC_START:
					os_printf("goto: AUD_TRAS_DRV_VOC_START \r\n");
					aud_tras_drv_voc_start();
					break;

				case AUD_TRAS_DRV_VOC_STOP:
					os_printf("goto: AUD_TRAS_DRV_VOC_STOP \r\n");
					aud_tras_drv_voc_stop();
					break;

				case AUD_TRAS_DRV_VOC_SET_MIC_GAIN:
					os_printf("goto: AUD_TRAS_DRV_VOC_SET_MIC_GAIN \r\n");
					aud_tras_drv_set_mic_gain(*((uint8_t *)msg.param));
					break;

				case AUD_TRAS_DRV_VOC_SET_SPK_GAIN:
					os_printf("goto: AUD_TRAS_DRV_VOC_SET_SPK_GAIN \r\n");
					aud_tras_drv_set_spk_gain(*((uint8_t *)msg.param));
					break;

				/* voc int op */
				case AUD_TRAS_DRV_AEC:
					//os_printf("goto: AUD_TRAS_DRV_AEC \r\n");
					aud_tras_aec();
					break;

				case AUD_TRAS_DRV_ENCODER:
					//os_printf("goto: AUD_TRAS_DRV_ENCODER \r\n");
					aud_tras_enc();
					break;

				case AUD_TRAS_DRV_DECODER:
					//os_printf("goto: AUD_TRAS_DRV_DECODER \r\n");
					aud_tras_dec();
					break;

				default:
					break;
			}
		}
	}

aud_tras_drv_exit:
	/* deinit mic, speaker and voice */
	/* check audio transfer driver work status */
	switch (aud_tras_drv_info.work_mode) {
		case AUD_INTF_WORK_MODE_GENERAL:
			/* check mic work status */
			if (aud_tras_drv_info.mic_info.status != AUD_TRAS_DRV_MIC_STA_NULL) {
				/* stop mic and deinit */
				aud_tras_drv_mic_stop();
				aud_tras_drv_mic_deinit();
			}
			/* check speaker work status */
			if (aud_tras_drv_info.spk_info.status != AUD_TRAS_DRV_SPK_STA_NULL) {
				/* stop speaker and deinit */
				aud_tras_drv_spk_stop();
				aud_tras_drv_spk_deinit();
			}
			break;

		case AUD_INTF_WORK_MODE_VOICE:
			/* check voice work status */
			if (aud_tras_drv_info.voc_info.status != AUD_TRAS_DRV_VOC_STA_NULL) {
				/* stop voice transfer and deinit */
				aud_tras_drv_voc_stop();
				aud_tras_drv_voc_deinit();
			}
			break;

		default:
			break;
	}

	aud_tras_drv_info.work_mode = AUD_INTF_WORK_MODE_NULL;
	aud_tras_drv_info.aud_tras_tx_mic_data = NULL;
	aud_tras_drv_info.aud_tras_rx_spk_data = NULL;

	/* set work status to NULL */
	aud_tras_drv_info.status = AUD_TRAS_DRV_STA_NULL;


	/* delete msg queue */
	ret = rtos_deinit_queue(&aud_trs_drv_int_msg_que);
	if (ret != kNoErr) {
		os_printf("delete message queue fail \r\n");
	}
	aud_trs_drv_int_msg_que = NULL;
	os_printf("delete message queue complete \r\n");

	/* delete task */
	aud_trs_drv_thread_hdl = NULL;
	rtos_delete_thread(NULL);
}

aud_intf_drv_setup_t aud_trs_drv_setup_bak = {0};

bk_err_t aud_tras_drv_init(aud_intf_drv_setup_t *setup_cfg)
{
	bk_err_t ret = BK_OK;
	if ((!aud_trs_drv_thread_hdl) && (!aud_trs_drv_int_msg_que)) {
		os_printf("init audio transfer driver \r\n");
		os_memcpy(&aud_trs_drv_setup_bak, setup_cfg, sizeof(aud_intf_drv_setup_t));

		ret = rtos_init_queue(&aud_trs_drv_int_msg_que,
							  "aud_tras_int_que",
							  sizeof(aud_tras_drv_msg_t),
							  TU_QITEM_COUNT);
		if (ret != kNoErr) {
			os_printf("ceate audio transfer driver internal message queue fail \r\n");
			return BK_FAIL;
		}
		os_printf("ceate audio transfer driver internal message queue complete \r\n");

		//create audio transfer driver task
		ret = rtos_create_thread(&aud_trs_drv_thread_hdl,
							 setup_cfg->task_config.priority,
							 "audio_intf",
							 (beken_thread_function_t)aud_tras_drv_main,
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

bk_err_t aud_tras_drv_deinit(void)
{
	bk_err_t ret;
	aud_tras_drv_msg_t msg;

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

