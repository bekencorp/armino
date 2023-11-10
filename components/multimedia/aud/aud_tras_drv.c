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
#include <bk_general_dma.h>
#include "aud_hal.h"
#include "sys_driver.h"
#include "aud_driver.h"
#include "aud_intf_private.h"
#include "aud_tras_drv.h"
#include <driver/psram.h>
#include <modules/aec.h>
#include <modules/audio_ring_buff.h>
#include <modules/g711.h>
#include "gpio_driver.h"
#include <driver/gpio.h>
#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
#include <driver/timer.h>
#endif

#include "bk_misc.h"
#include <soc/mapping.h>

#include "media_evt.h"
#include "media_mailbox_list_util.h"
#include <bk_general_dma.h>

#define AUD_TRAS_DRV_TAG "aud_tras_drv"

#define LOGI(...) BK_LOGI(AUD_TRAS_DRV_TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(AUD_TRAS_DRV_TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(AUD_TRAS_DRV_TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(AUD_TRAS_DRV_TAG, ##__VA_ARGS__)

#define CONFIG_AUD_TRAS_AEC_MIC_DELAY_POINTS   53
//#define CONFIG_AUD_RING_BUFF_SAFE_INTERVAL    20

#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
#define AUD_LOST_DEBUG_INTERVAL (1000 * 2)
#endif

#define TU_QITEM_COUNT      (120)
static beken_thread_t  aud_trs_drv_thread_hdl = NULL;
static beken_queue_t aud_trs_drv_int_msg_que = NULL;

aud_tras_drv_info_t aud_tras_drv_info = DEFAULT_AUD_TRAS_DRV_INFO();
static bool uac_mic_read_flag = false;

media_mailbox_msg_t *aud_to_media_app_msg = NULL;

media_mailbox_msg_t mic_to_media_app_msg = {0};
beken_semaphore_t mailbox_media_aud_mic_sem = NULL;

aud_tras_drv_mic_notify_t mic_nofity = {0, 0};


#if CONFIG_AUD_TRAS_AEC_MIC_DELAY_DEBUG
static uint8_t mic_delay_num = 0;
#endif

#define AUD_MIC_DEBUG       0


#if AUD_MIC_DEBUG
static const unsigned int PCM_8000[] = {
	0x0001, 0x5A82, 0x7FFF, 0x5A82, 0x0000, 0xA57F, 0x8001, 0xA57E,
	0x0001, 0x5A82, 0x7FFF, 0x5A82, 0x0000, 0xA57F, 0x8001, 0xA57E,
	0x0001, 0x5A82, 0x7FFF, 0x5A82, 0x0000, 0xA57F, 0x8001, 0xA57E,
	0x0001, 0x5A82, 0x7FFF, 0x5A82, 0x0000, 0xA57F, 0x8001, 0xA57E,
	0x0001, 0x5A82, 0x7FFF, 0x5A82, 0x0000, 0xA57F, 0x8001, 0xA57E,
	0x0001, 0x5A82, 0x7FFF, 0x5A82, 0x0000, 0xA57F, 0x8001, 0xA57E,
	0x0001, 0x5A82, 0x7FFF, 0x5A82, 0x0000, 0xA57F, 0x8001, 0xA57E,
	0x0001, 0x5A82, 0x7FFF, 0x5A82, 0x0000, 0xA57F, 0x8001, 0xA57E,
	0x0001, 0x5A82, 0x7FFF, 0x5A82, 0x0000, 0xA57F, 0x8001, 0xA57E,
	0x0001, 0x5A82, 0x7FFF, 0x5A82, 0x0000, 0xA57F, 0x8001, 0xA57E,
	0x0001, 0x5A82, 0x7FFF, 0x5A82, 0x0000, 0xA57F, 0x8001, 0xA57E,
	0x0001, 0x5A82, 0x7FFF, 0x5A82, 0x0000, 0xA57F, 0x8001, 0xA57E,
	0x0001, 0x5A82, 0x7FFF, 0x5A82, 0x0000, 0xA57F, 0x8001, 0xA57E,
	0x0001, 0x5A82, 0x7FFF, 0x5A82, 0x0000, 0xA57F, 0x8001, 0xA57E,
	0x0001, 0x5A82, 0x7FFF, 0x5A82, 0x0000, 0xA57F, 0x8001, 0xA57E,
	0x0001, 0x5A82, 0x7FFF, 0x5A82, 0x0000, 0xA57F, 0x8001, 0xA57E,
	0x0001, 0x5A82, 0x7FFF, 0x5A82, 0x0000, 0xA57F, 0x8001, 0xA57E,
	0x0001, 0x5A82, 0x7FFF, 0x5A82, 0x0000, 0xA57F, 0x8001, 0xA57E,
	0x0001, 0x5A82, 0x7FFF, 0x5A82, 0x0000, 0xA57F, 0x8001, 0xA57E,
	0x0001, 0x5A82, 0x7FFF, 0x5A82, 0x0000, 0xA57F, 0x8001, 0xA57E,
};
#endif

/* extern api */
bk_err_t aud_tras_drv_deinit(void);
static void aud_tras_drv_mic_uac_mic_cb(uint8_t *buff, uint32_t count);
static void aud_tras_drv_spk_uac_spk_cb(void);
static void aud_tras_drv_voc_uac_mic_cb(uint8_t *buff, uint32_t count);
static void aud_tras_drv_voc_uac_spk_cb(void);
static bk_err_t uac_set_mic_samp_rate(aud_adc_samp_rate_t samp_rate);
static bk_err_t uac_set_spk_samp_rate(aud_dac_samp_rate_t samp_rate);
static bk_err_t aud_tras_drv_voc_start(void);
static bk_err_t aud_tras_drv_set_spk_gain(uint16_t value);


static void aud_tras_dac_pa_ctrl(bool en)
{
	if (en) {
#if CONFIG_AUD_TRAS_DAC_PA_CTRL
	/* delay 2ms to avoid po audio data, and then open pa */
	delay_ms(2);
	/* open pa according to congfig */
	gpio_dev_unmap(AUD_DAC_PA_CTRL_GPIO);
	bk_gpio_enable_output(AUD_DAC_PA_CTRL_GPIO);
#if AUD_DAC_PA_ENABLE_LEVEL
	bk_gpio_set_output_high(AUD_DAC_PA_CTRL_GPIO);
#else
	bk_gpio_set_output_low(AUD_DAC_PA_CTRL_GPIO);
#endif
#endif
	} else {
#if CONFIG_AUD_TRAS_DAC_PA_CTRL
		/* open pa according to congfig */
		//gpio_dev_unmap(AUD_DAC_PA_CTRL_GPIO);
		//bk_gpio_enable_output(AUD_DAC_PA_CTRL_GPIO);
#if AUD_DAC_PA_ENABLE_LEVEL
		bk_gpio_set_output_low(AUD_DAC_PA_CTRL_GPIO);
#else
		bk_gpio_set_output_high(AUD_DAC_PA_CTRL_GPIO);
#endif
#endif
	}
}

bk_err_t aud_tras_drv_send_msg(aud_tras_drv_op_t op, void *param)
{
	bk_err_t ret;
	aud_tras_drv_msg_t msg;

	msg.op = op;
	msg.param = param;
	if (aud_trs_drv_int_msg_que) {
		ret = rtos_push_to_queue(&aud_trs_drv_int_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			LOGE("aud_tras_send_int_msg fail \r\n");
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
		LOGE("init audio driver fail \r\n");
		goto aud_adc_exit;
	}

	ret = bk_aud_adc_init(AUD_ADC_WORK_MODE_ADC, adc_config, NULL);
	if (ret != BK_OK) {
		LOGE("init audio adc fail \r\n");
		goto aud_adc_exit;
	}

	return BK_OK;

aud_adc_exit:
	LOGE("audio adc config fail \r\n");
	bk_aud_driver_deinit();
	return BK_FAIL;
}

static bk_err_t aud_tras_dac_config(aud_dac_config_t *dac_config)
{
	bk_err_t ret = BK_OK;

	/* init audio driver and config dac */
	ret = bk_aud_driver_init();
	if (ret != BK_OK) {
		LOGE("init audio driver fail \r\n");
		goto aud_dac_exit;
	}

	ret = bk_aud_dac_init(dac_config);
	if (ret != BK_OK) {
		LOGE("init audio dac fail \r\n");
		goto aud_dac_exit;
	}

	return BK_OK;

aud_dac_exit:
	LOGE("audio dac config fail \r\n");
	bk_aud_driver_deinit();
	return BK_FAIL;
}

static bk_err_t aud_tras_drv_aec_cfg(void)
{
	uint32_t aec_context_size = 0;
	uint32_t val = 0;
	aec_info_t *temp_aec_info = aud_tras_drv_info.voc_info.aec_info;
	/* init aec context */
	aec_context_size = aec_size(1000);
	LOGI("malloc aec size: %d \r\n", aec_context_size);
#if CONFIG_AUD_TRAS_AEC_FIXED_SRAM
	temp_aec_info->aec = (AECContext*)AUD_AEC_SRAM_ADDRESS;
	LOGI("aec use fixed sram: %p \n", temp_aec_info->aec);
#elif CONFIG_AUD_TRAS_AEC_MALLOC_PSRAM
	temp_aec_info->aec = (AECContext*)psram_malloc(aec_context_size);
	LOGI("aec use malloc psram: %p \n", temp_aec_info->aec);
#else
	temp_aec_info->aec = (AECContext*)os_malloc(aec_context_size);
	LOGI("aec use malloc sram: %p \n", temp_aec_info->aec);
#endif
	if (temp_aec_info->aec == NULL) {
		LOGE("malloc aec fail, aec_context_size: %d \r\n", aec_context_size);
		return BK_FAIL;
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
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_SET_FLAGS, temp_aec_info->aec_config->init_flags);							//库内各模块开关; aec_init内默认赋值0x1f;

	/* 回声消除相关 */
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_SET_MIC_DELAY, temp_aec_info->aec_config->mic_delay);						//设置参考信号延迟(采样点数，需要dump数据观察)
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_SET_EC_DEPTH, temp_aec_info->aec_config->ec_depth);							//建议取值范围1~50; 后面几个参数建议先用aec_init内的默认值，具体需要根据实际情况调试; 总得来说回声越大需要调的越大
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_SET_TxRxThr, temp_aec_info->aec_config->TxRxThr);							//建议取值范围10~64
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_SET_TxRxFlr, temp_aec_info->aec_config->TxRxFlr);							//建议取值范围1~10

	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_SET_REF_SCALE, temp_aec_info->aec_config->ref_scale);						//取值0,1,2；rx数据如果幅值太小的话适当放大
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_SET_VOL, temp_aec_info->aec_config->voice_vol);								//通话过程中如果需要经常调节喇叭音量就设置下当前音量等级
	/* 降噪相关 */
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_SET_NS_LEVEL, temp_aec_info->aec_config->ns_level);							//建议取值范围1~8；值越小底噪越小
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_SET_NS_PARA, temp_aec_info->aec_config->ns_para);							//只能取值0,1,2; 降噪由弱到强，建议默认值
	/* drc(输出音量相关) */
	aec_ctrl(temp_aec_info->aec, AEC_CTRL_CMD_SET_DRC, temp_aec_info->aec_config->drc);									//建议取值范围0x10~0x1f;   越大输出声音越大

	return BK_OK;
}

static bk_err_t aud_tras_drv_aec_buff_cfg(aec_info_t *aec_info)
{
	uint16_t samp_rate_points = aec_info->samp_rate_points;

	/* malloc aec ref ring buffer to save ref data */
	LOGI("ref_ring_buff size: %d \r\n", samp_rate_points*2*2);
	aec_info->aec_ref_ring_buff = (int16_t *)os_malloc(samp_rate_points*2*2 + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL);
	if (aec_info->aec_ref_ring_buff == NULL) {
		LOGE("malloc ref ring buffer fail \r\n");
		goto aud_tras_drv_aec_buff_cfg_exit;
	}

	/* malloc aec out ring buffer to save mic data has been aec processed */
	aec_info->aec_out_ring_buff = (int16_t *)os_malloc(samp_rate_points*2*2 + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL);
	if (aec_info->aec_out_ring_buff == NULL) {
		LOGE("malloc aec out ring buffer fail \r\n");
		goto aud_tras_drv_aec_buff_cfg_exit;
	}

	/* init ref_ring_buff */
	ring_buffer_init(&(aec_info->ref_rb), (uint8_t*)aec_info->aec_ref_ring_buff, samp_rate_points*2*2 + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL, DMA_ID_MAX, RB_DMA_TYPE_NULL);

	/* init aec_ring_buff */
	ring_buffer_init(&(aec_info->aec_rb), (uint8_t*)aec_info->aec_out_ring_buff, samp_rate_points*2*2 + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL, DMA_ID_MAX, RB_DMA_TYPE_NULL);

	return BK_OK;

aud_tras_drv_aec_buff_cfg_exit:
	if (aec_info->aec_ref_ring_buff != NULL) {
		os_free(aec_info->aec_ref_ring_buff);
		aec_info->aec_ref_ring_buff = NULL;
	}
	if (aec_info->aec_out_ring_buff != NULL) {
		os_free(aec_info->aec_out_ring_buff);
		aec_info->aec_out_ring_buff = NULL;
	}
	return BK_FAIL;
}

static void aud_tras_drv_aec_decfg(void)
{
	if (aud_tras_drv_info.voc_info.aec_enable) {
		if (aud_tras_drv_info.voc_info.aec_info->aec) {
#if CONFIG_AUD_TRAS_AEC_MALLOC_SRAM
			os_free(aud_tras_drv_info.voc_info.aec_info->aec);
#else
			//not need free
#endif
			aud_tras_drv_info.voc_info.aec_info->aec = NULL;
		}

		if (aud_tras_drv_info.voc_info.aec_info->aec_config) {
			os_free(aud_tras_drv_info.voc_info.aec_info->aec_config);
			aud_tras_drv_info.voc_info.aec_info->aec_config = NULL;
		}

		ring_buffer_clear(&(aud_tras_drv_info.voc_info.aec_info->ref_rb));
		ring_buffer_clear(&(aud_tras_drv_info.voc_info.aec_info->aec_rb));

		if (aud_tras_drv_info.voc_info.aec_info->aec_ref_ring_buff) {
			os_free(aud_tras_drv_info.voc_info.aec_info->aec_ref_ring_buff);
			aud_tras_drv_info.voc_info.aec_info->aec_ref_ring_buff = NULL;
		}

		if (aud_tras_drv_info.voc_info.aec_info->aec_out_ring_buff) {
			os_free(aud_tras_drv_info.voc_info.aec_info->aec_out_ring_buff);
			aud_tras_drv_info.voc_info.aec_info->aec_out_ring_buff = NULL;
		}

		if (aud_tras_drv_info.voc_info.aec_info) {
			os_free(aud_tras_drv_info.voc_info.aec_info);
			aud_tras_drv_info.voc_info.aec_info = NULL;
		}
	} else {
		aud_tras_drv_info.voc_info.aec_info = NULL;
	}

	aud_tras_drv_info.voc_info.aec_enable = false;
}

/* 搬运audio adc 采集到的一帧mic和ref信号后，触发中断通知AEC处理数据 */
static void aud_tras_adc_dma_finish_isr(void)
{
	bk_err_t ret = BK_OK;
	GPIO_UP(3);

	if (aud_tras_drv_info.work_mode == AUD_INTF_WORK_MODE_GENERAL) {
		ret = aud_tras_drv_send_msg(AUD_TRAS_DRV_MIC_TX_DATA, NULL);
	} else {
		/* send msg to AEC or ENCODER to process mic data */
		if (aud_tras_drv_info.voc_info.aec_enable)
			ret = aud_tras_drv_send_msg(AUD_TRAS_DRV_AEC, NULL);
		else
			ret = aud_tras_drv_send_msg(AUD_TRAS_DRV_ENCODER, NULL);
	}
	if (ret != kNoErr) {
		LOGE("send msg: AUD_TRAS_DRV_AEC fail \r\n");
	}
	GPIO_DOWN(3);
}

static bk_err_t aud_tras_adc_dma_config(dma_id_t dma_id, int32_t *ring_buff_addr, uint32_t ring_buff_size, uint32_t transfer_len, aud_intf_mic_chl_t mic_chl)
{
	bk_err_t ret = BK_OK;
	dma_config_t dma_config;
	uint32_t adc_port_addr;

	dma_config.mode = DMA_WORK_MODE_REPEAT;
	dma_config.chan_prio = 1;
	dma_config.src.dev = DMA_DEV_AUDIO_RX;
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
		LOGE("get adc fifo address failed\r\n");
		return BK_ERR_AUD_INTF_ADC;
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
		LOGE("audio adc dma channel init fail \r\n");
		return BK_ERR_AUD_INTF_DMA;
	}

	/* set dma transfer length */
	bk_dma_set_transfer_len(dma_id, transfer_len);

	//register isr
	bk_dma_register_isr(dma_id, NULL, (void *)aud_tras_adc_dma_finish_isr);
	bk_dma_enable_finish_interrupt(dma_id);
#if (CONFIG_SPE)
	BK_LOG_ON_ERR(bk_dma_set_dest_sec_attr(dma_id, DMA_ATTR_SEC));
	BK_LOG_ON_ERR(bk_dma_set_src_sec_attr(dma_id, DMA_ATTR_SEC));
#endif

	return BK_ERR_AUD_INTF_OK;
}

/* 搬运audio dac 一帧dac信号后，触发中断通知decoder处理数据 */
static void aud_tras_dac_dma_finish_isr(void)
{
	bk_err_t ret = BK_OK;
	GPIO_UP(2);

	if (aud_tras_drv_info.work_mode == AUD_INTF_WORK_MODE_GENERAL)
		/* send msg to notify app to write speaker data */
		ret = aud_tras_drv_send_msg(AUD_TRAS_DRV_SPK_REQ_DATA, NULL);
	else
		/* send msg to decoder to decoding recevied data */
		ret = aud_tras_drv_send_msg(AUD_TRAS_DRV_DECODER, NULL);
	if (ret != kNoErr) {
		LOGE("send msg: AUD_TRAS_DRV_DECODER fails \r\n");
	}
	GPIO_DOWN(2);
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
		case AUD_INTF_SPK_CHL_LEFT:
			dma_config.dst.width = DMA_DATA_WIDTH_16BITS;
			break;

		case AUD_INTF_SPK_CHL_DUAL:
			dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
			break;

		default:
			break;
	}

	/* get dac fifo address */
	if (bk_aud_get_dac_fifo_addr(&dac_port_addr) != BK_OK) {
		LOGE("get dac fifo address failed\r\n");
		return BK_FAIL;
	} else {
		dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
		dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
		dma_config.dst.start_addr = dac_port_addr;
		dma_config.dst.end_addr = dac_port_addr + 4;
	}
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.src.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
	dma_config.src.start_addr = (uint32_t)ring_buff_addr;
	dma_config.src.end_addr = (uint32_t)(ring_buff_addr) + ring_buff_size;

	/* init dma channel */
	ret = bk_dma_init(dma_id, &dma_config);
	if (ret != BK_OK) {
		LOGE("audio dac dma channel init fail \r\n");
		return BK_FAIL;
	}

	/* set dma transfer length */
	bk_dma_set_transfer_len(dma_id, transfer_len);

	//register isr
	bk_dma_register_isr(dma_id, NULL, (void *)aud_tras_dac_dma_finish_isr);
	bk_dma_enable_finish_interrupt(dma_id);
#if (CONFIG_SPE)
	BK_LOG_ON_ERR(bk_dma_set_dest_sec_attr(dma_id, DMA_ATTR_SEC));
	BK_LOG_ON_ERR(bk_dma_set_src_sec_attr(dma_id, DMA_ATTR_SEC));
#endif

	return BK_OK;
}

static bk_err_t aud_tras_aec(void)
{
	bk_err_t ret = BK_OK;
	uint32_t size = 0;

	if (aud_tras_drv_info.voc_info.status == AUD_TRAS_DRV_VOC_STA_NULL)
		return BK_OK;

	aec_info_t *aec_info_pr = aud_tras_drv_info.voc_info.aec_info;

	/* get a fram mic data from mic_ring_buff */
	if (ring_buffer_get_fill_size(&(aud_tras_drv_info.voc_info.mic_rb)) >= aec_info_pr->samp_rate_points*2) {
		size = ring_buffer_read(&(aud_tras_drv_info.voc_info.mic_rb), (uint8_t*)aec_info_pr->mic_addr, aec_info_pr->samp_rate_points*2);
		if (size != aec_info_pr->samp_rate_points*2) {
			LOGE("read mic_ring_buff fail, size:%d \r\n", size);
			//return BK_FAIL;
		}
	} else {
		LOGD("%s: do not have mic data need to aec \r\n", __func__);
		return BK_OK;
	}

#if CONFIG_VIDEO_AVI
	aud_tras_drv_save_data_pool((uint8_t*)aec_info_pr->mic_addr,size);
#endif

	if (aud_tras_drv_info.voc_info.spk_en == AUD_INTF_VOC_SPK_CLOSE || aud_tras_drv_info.voc_info.aec_enable == false) {
		/* save mic data after aec processed to aec_ring_buffer */
		if (ring_buffer_get_free_size(&(aec_info_pr->aec_rb)) >= aec_info_pr->samp_rate_points*2) {
			size = ring_buffer_write(&(aec_info_pr->aec_rb), (uint8_t*)aec_info_pr->mic_addr, aec_info_pr->samp_rate_points*2);
			if (size != aec_info_pr->samp_rate_points*2) {
				LOGE("the data writeten to aec_ring_buff is not a frame \r\n");
				//return BK_FAIL;
			}
		}

		/* send msg to encoder to encoding data */
		ret = aud_tras_drv_send_msg(AUD_TRAS_DRV_ENCODER, NULL);
		if (ret != kNoErr) {
			LOGE("send msg: AUD_TRAS_DRV_ENCODER fail \r\n");
			return BK_FAIL;
		}

		return BK_OK;
	}

	/* read ref data from ref_ring_buff */
	if (ring_buffer_get_fill_size(&(aec_info_pr->ref_rb)) >= aec_info_pr->samp_rate_points*2) {
		size = ring_buffer_read(&(aec_info_pr->ref_rb), (uint8_t*)aec_info_pr->ref_addr, aec_info_pr->samp_rate_points*2);
		if (size != aec_info_pr->samp_rate_points*2) {
			LOGE("the ref data readed from ref_ring_buff is not a frame \r\n");
			//return BK_FAIL;
			//os_memcpy(ref_addr, 0, frame_sample*2);
		}
	}
	//os_printf("ref_addr: ref_addr[0]= %02x, ref_addr[0]= %02x \r\n", ref_addr[0], ref_addr[1]);

	if (aud_tras_drv_info.voc_info.aud_tras_dump_aec_cb) {
		aud_tras_drv_info.voc_info.aud_tras_dump_aec_cb((uint8_t *)aec_info_pr->mic_addr, aec_info_pr->samp_rate_points*2);
		aud_tras_drv_info.voc_info.aud_tras_dump_aec_cb((uint8_t *)aec_info_pr->ref_addr, aec_info_pr->samp_rate_points*2);
	}

#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
#if CONFIG_AUD_TRAS_AEC_DUMP_MODE_TF
	os_memcpy((void *)aud_tras_drv_info.voc_info.aec_dump.mic_dump_addr, aec_info_pr->mic_addr, aec_info_pr->samp_rate_points*2);
	os_memcpy((void *)aud_tras_drv_info.voc_info.aec_dump.ref_dump_addr, aec_info_pr->ref_addr, aec_info_pr->samp_rate_points*2);
	//os_printf("memcopy complete \r\n");
#endif //CONFIG_AUD_TRAS_AEC_DUMP_MODE_TF

#if CONFIG_AUD_TRAS_AEC_DUMP_MODE_UART
	bk_uart_write_bytes(CONFIG_AUD_TRAS_AEC_DUMP_UART_ID, aec_info_pr->mic_addr, aec_info_pr->samp_rate_points*2);
	bk_uart_write_bytes(CONFIG_AUD_TRAS_AEC_DUMP_UART_ID, aec_info_pr->ref_addr, aec_info_pr->samp_rate_points*2);
#endif

#if CONFIG_AUD_TRAS_AEC_DUMP_MODE_UDP
	size = bk_aud_debug_voc_udp_send_packet((unsigned char *)aec_info_pr->mic_addr, aec_info_pr->samp_rate_points*2);
	if (size != aec_info_pr->samp_rate_points*2)
		os_printf("udp dump mic fail, all:%d, complete:%d \r\n", aec_info_pr->samp_rate_points*2, size);
	size = bk_aud_debug_voc_udp_send_packet((unsigned char *)aec_info_pr->ref_addr, aec_info_pr->samp_rate_points*2);
	if (size != aec_info_pr->samp_rate_points*2)
		os_printf("udp dump ref fail, all:%d, complete:%d \r\n", aec_info_pr->samp_rate_points*2, size);
#endif
#if CONFIG_AUD_TRAS_AEC_DUMP_MODE_TCP
	size = bk_aud_debug_voc_tcp_send_packet((unsigned char *)aec_info_pr->mic_addr, aec_info_pr->samp_rate_points*2);
	if (size != aec_info_pr->samp_rate_points*2)
		os_printf("tcp dump mic fail, all:%d, complete:%d \r\n", aec_info_pr->samp_rate_points*2, size);
	size = bk_aud_debug_voc_tcp_send_packet((unsigned char *)aec_info_pr->ref_addr, aec_info_pr->samp_rate_points*2);
	if (size != aec_info_pr->samp_rate_points*2)
		os_printf("tcp dump ref fail, all:%d, complete:%d \r\n", aec_info_pr->samp_rate_points*2, size);
#endif

#endif //CONFIG_AUD_TRAS_AEC_DUMP_DEBUG

	/* aec process data */
	//os_printf("ref_addr:%p, mic_addr:%p, out_addr:%p \r\n", aec_context_pr->ref_addr, aec_context_pr->mic_addr, aec_context_pr->out_addr);
	aec_proc(aec_info_pr->aec, aec_info_pr->ref_addr, aec_info_pr->mic_addr, aec_info_pr->out_addr);

	if (aud_tras_drv_info.voc_info.aud_tras_dump_aec_cb) {
		aud_tras_drv_info.voc_info.aud_tras_dump_aec_cb((uint8_t *)aec_info_pr->out_addr, aec_info_pr->samp_rate_points*2);
	}

#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
#if CONFIG_AUD_TRAS_AEC_DUMP_MODE_TF
	os_memcpy((void *)aud_tras_drv_info.voc_info.aec_dump.out_dump_addr, aec_info_pr->out_addr, aec_info_pr->samp_rate_points*2);
#endif //CONFIG_AUD_TRAS_AEC_DUMP_MODE_TF

#if CONFIG_AUD_TRAS_AEC_DUMP_MODE_UART
	bk_uart_write_bytes(CONFIG_AUD_TRAS_AEC_DUMP_UART_ID, aec_info_pr->out_addr, aec_info_pr->samp_rate_points*2);
#endif

#if CONFIG_AUD_TRAS_AEC_DUMP_MODE_UDP
	size = bk_aud_debug_voc_udp_send_packet((unsigned char *)aec_info_pr->out_addr, aec_info_pr->samp_rate_points*2);
	if (size != aec_info_pr->samp_rate_points*2)
		os_printf("udp dump aec out fail, all:%d, complete:%d \r\n", aec_info_pr->samp_rate_points*2, size);
#endif
#if CONFIG_AUD_TRAS_AEC_DUMP_MODE_TCP
	size = bk_aud_debug_voc_tcp_send_packet((unsigned char *)aec_info_pr->out_addr, aec_info_pr->samp_rate_points*2);
	if (size != aec_info_pr->samp_rate_points*2)
		os_printf("tcp dump aec out fail, all:%d, complete:%d \r\n", aec_info_pr->samp_rate_points*2, size);
#endif
#endif //CONFIG_AUD_TRAS_AEC_DUMP_DEBUG

	/* save mic data after aec processed to aec_ring_buffer */
	if (ring_buffer_get_free_size(&(aec_info_pr->aec_rb)) >= aec_info_pr->samp_rate_points*2) {
		size = ring_buffer_write(&(aec_info_pr->aec_rb), (uint8_t*)aec_info_pr->out_addr, aec_info_pr->samp_rate_points*2);
		if (size != aec_info_pr->samp_rate_points*2) {
			LOGE("the data writeten to aec_ring_buff is not a frame \r\n");
			//return BK_FAIL;
		}
	}

	/* send msg to encoder to encoding data */
	ret = aud_tras_drv_send_msg(AUD_TRAS_DRV_ENCODER, NULL);
	if (ret != kNoErr) {
		LOGE("send msg: AUD_TRAS_DRV_ENCODER fail \r\n");
		return BK_FAIL;
	}

	return ret;
}

/* Automatic recover uac connect */
static bk_err_t uac_recover_connect(void)
{
	LOGI("%s\n", __func__);
	bk_err_t ret = BK_OK;
	/* mount uac */
	ret = bk_aud_uac_driver_init();
	if (ret != BK_OK) {
		aud_tras_drv_info.uac_status = AUD_INTF_UAC_CANNOT_RECOVER_CONNECT;
		if (aud_tras_drv_info.aud_tras_drv_uac_connect_state_cb) {
			aud_tras_drv_info.aud_tras_drv_uac_connect_state_cb(AUD_INTF_UAC_CANNOT_RECOVER_CONNECT);
		}
	}

	//aud_tras_drv_info.uac_status = AUD_INTF_UAC_CONNECTED;

	/* uac mic: register mic buffer and callback */
	if ((aud_tras_drv_info.mic_info.status != AUD_TRAS_DRV_MIC_STA_NULL) && (aud_tras_drv_info.mic_info.mic_type == AUD_INTF_MIC_TYPE_UAC)) {
		LOGI("[%s] recover uac mic \n", __func__);
		ret = bk_aud_uac_register_mic_callback((void *)aud_tras_drv_mic_uac_mic_cb);
		if (ret != BK_OK) {
			LOGI("[%s] register_mic_callback fail, ret:%d \n", ret);
			return ret;
		}
		ret = uac_set_mic_samp_rate(aud_tras_drv_info.mic_info.uac_config->mic_samp_rate);
		if (ret != BK_OK) {
			LOGI("[%s] uac_set_mic_samp_rate fail, ret:%d \n", ret);
			return ret;
		}
		/* recover mic status */
		if (aud_tras_drv_info.mic_info.status == AUD_TRAS_DRV_MIC_STA_START) {
			ret = bk_aud_uac_start_mic();
			if (ret != BK_OK) {
				LOGI("[%s] uac_start_mic fail, ret:%d \n", ret);
				return ret;
			}
		}
	}

	/* uac spk: register speaker buffer and callback */
	if ((aud_tras_drv_info.spk_info.status != AUD_TRAS_DRV_SPK_STA_NULL) && (aud_tras_drv_info.spk_info.spk_type == AUD_INTF_SPK_TYPE_UAC)) {
		LOGI("[%s] recover uac spk \n", __func__);
		/* register buffer to save uac speaker data */
		ret = bk_aud_uac_register_spk_buff_ptr(aud_tras_drv_info.spk_info.uac_spk_buff, aud_tras_drv_info.spk_info.uac_spk_buff_size);
		if (ret != BK_OK) {
			LOGI("[%s] register_spk_buff_ptr fail, ret:%d \n", ret);
			return ret;
		}
		LOGI("[%s] register_spk_buff_ptr complete \n", __func__);

		ret = bk_aud_uac_register_spk_callback((void *)aud_tras_drv_spk_uac_spk_cb);
		if (ret != BK_OK) {
			LOGI("[%s] register_spk_callback fail, ret:%d \n", ret);
			return ret;
		}
		LOGI("[%s] register_spk_callback complete \n", __func__);

		ret = uac_set_spk_samp_rate(aud_tras_drv_info.spk_info.uac_config->spk_samp_rate);
		if (ret != BK_OK) {
			LOGI("[%s] uac_set_spk_samp_rate fail, ret:%d \n", ret);
			return ret;
		}
		LOGI("[%s] uac_set_spk_samp_rate complete \n", __func__);

		/* recover spk status */
		if (aud_tras_drv_info.spk_info.status == AUD_TRAS_DRV_SPK_STA_START) {
			ret = bk_aud_uac_start_spk();
			if (ret != BK_OK) {
				LOGI("[%s] uac_start_spk fail, ret:%d \n", ret);
				return ret;
			}
		}
	}

	/* uac voc: register mic and speaker buffer and callback */
	if (aud_tras_drv_info.voc_info.status != AUD_TRAS_DRV_VOC_STA_NULL) {
		LOGI("[%s] recover uac voc \n", __func__);
		/* recover uac mic */
		if (aud_tras_drv_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_UAC) {
			ret = bk_aud_uac_register_mic_callback((void *)aud_tras_drv_voc_uac_mic_cb);
			if (ret != BK_OK) {
				LOGI("[%s] register_mic_callback fail, ret:%d \n", ret);
				return ret;
			}
			/* clear uac mic data */
			ring_buffer_clear(&(aud_tras_drv_info.voc_info.mic_rb));
		}

		/* recover uac spk */
		if (aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_UAC) {
			ret = bk_aud_uac_register_spk_buff_ptr(aud_tras_drv_info.voc_info.uac_spk_buff, aud_tras_drv_info.voc_info.uac_spk_buff_size);
			if (ret != BK_OK) {
				LOGI("[%s] register_spk_buff_ptr fail, ret:%d \n", ret);
				return ret;
			}
			ret = bk_aud_uac_register_spk_callback(aud_tras_drv_voc_uac_spk_cb);
			if (ret != BK_OK) {
				LOGI("[%s] register_spk_callback fail, ret:%d \n", ret);
				return ret;
			}
			ring_buffer_clear(&(aud_tras_drv_info.voc_info.speaker_rb));
		}

		/* recover mic and spk status */
		if (aud_tras_drv_info.voc_info.status == AUD_TRAS_DRV_VOC_STA_START) {
			ret = aud_tras_drv_voc_start();
			if (ret != BK_OK) {
				LOGI("[%s] voc_start fail, ret:%d \n", ret);
				return ret;
			}
		}
	}

	return BK_OK;
}

static bk_err_t aud_tras_uac_auto_connect_ctrl(bool en)
{
	aud_tras_drv_info.uac_auto_connect = en;

	if (aud_tras_drv_info.aud_tras_drv_com_event_cb)
		aud_tras_drv_info.aud_tras_drv_com_event_cb(EVENT_AUD_TRAS_COM_UAC_AUTO_CONT_CTRL, BK_ERR_AUD_INTF_OK);

	return BK_OK;
}

static bk_err_t aud_tras_uac_disconnect_handle(void)
{
	/* notify app that uac disconnecting */
	if (aud_tras_drv_info.aud_tras_drv_uac_connect_state_cb) {
		aud_tras_drv_info.aud_tras_drv_uac_connect_state_cb(AUD_INTF_UAC_ABNORMAL_DISCONNECTED);
	}

	return BK_OK;
}

static void aud_tras_uac_disconnect_cb(void)
{
	uint8_t count = 6;
	bk_err_t ret = BK_OK;

	if (aud_tras_drv_info.uac_status != AUD_INTF_UAC_NORMAL_DISCONNECTED) {
		aud_tras_drv_info.uac_status = AUD_INTF_UAC_ABNORMAL_DISCONNECTED;
		do {
			if (count == 0)
				break;
			count--;
			ret = aud_tras_drv_send_msg(AUD_TRAS_DRV_UAC_DISCONT, NULL);
			if (ret != BK_OK)
				LOGE("send msg: AUD_TRAS_DRV_UAC_DISCONT fail: %d \n", count);
			rtos_delay_milliseconds(20);
		} while (ret != BK_OK);
	}
}

static bk_err_t aud_tras_uac_connect_handle(void)
{
	bk_err_t ret = BK_OK;

	/* notify app that uac disconnecting */
	if (aud_tras_drv_info.aud_tras_drv_uac_connect_state_cb) {
		aud_tras_drv_info.aud_tras_drv_uac_connect_state_cb(AUD_INTF_UAC_CONNECTED);
	}

	/* uac automatically connect */
	if (aud_tras_drv_info.uac_auto_connect) {
		ret = uac_recover_connect();
		if (ret != BK_OK) {
			LOGI("[%s] uac_recover_connect fail, ret: %d \n", ret);
			//uac_sta = AUD_INTF_UAC_CANNOT_RECOVER_CONNECT;
		}
	}
	return ret;
}


static void aud_tras_uac_connect_cb(void)
{
	uint8_t count = 6;
	bk_err_t ret = BK_OK;

	if (aud_tras_drv_info.uac_status == AUD_INTF_UAC_ABNORMAL_DISCONNECTED) {
		aud_tras_drv_info.uac_status = AUD_INTF_UAC_CONNECTED;
		do {
			if (count == 0)
				break;
			count--;
			ret = aud_tras_drv_send_msg(AUD_TRAS_DRV_UAC_CONT, NULL);
			if (ret != BK_OK)
				LOGE("send msg: AUD_TRAS_DRV_UAC_CONT fail: %d \n", count);
			rtos_delay_milliseconds(20);
		} while (ret != BK_OK);
	}
}

static bk_err_t aud_tras_enc(void)
{
	bk_err_t ret = BK_OK;
	uint32_t size = 0;
	uint32_t i = 0;
	uint16_t temp_mic_samp_rate_points = aud_tras_drv_info.voc_info.mic_samp_rate_points;
	tx_info_t temp_tx_info;
//	uint32_t fill_size = 0;

	if (aud_tras_drv_info.voc_info.status == AUD_TRAS_DRV_VOC_STA_NULL)
		return BK_OK;

	if (aud_tras_drv_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_BOARD) {
		if (aud_tras_drv_info.voc_info.aec_enable) {
			/* get data from aec_ring_buff */
			size = ring_buffer_read(&(aud_tras_drv_info.voc_info.aec_info->aec_rb), (uint8_t *)aud_tras_drv_info.voc_info.encoder_temp.pcm_data, temp_mic_samp_rate_points*2);
			if (size != temp_mic_samp_rate_points*2) {
				//LOGE("read aec_rb :%d \r\n", size);
				//os_memset(aud_tras_drv_info.voc_info.encoder_temp.pcm_data, 0, temp_mic_samp_rate_points*2);
				goto encoder_exit;
			}
		} else {
			/* get data from mic_ring_buff */
//			fill_size = ring_buffer_get_fill_size(&(aud_tras_drv_info.voc_info.mic_rb));
			size = ring_buffer_read(&(aud_tras_drv_info.voc_info.mic_rb), (uint8_t *)aud_tras_drv_info.voc_info.encoder_temp.pcm_data, temp_mic_samp_rate_points*2);
			if (size != temp_mic_samp_rate_points*2) {
				//LOGE("mic fill:%d, need:%d, read:%d,\r\n", fill_size,  temp_mic_samp_rate_points*2, size);
				//goto encoder_exit;
			}
#if AUD_MIC_DEBUG
			os_memcpy(aud_tras_drv_info.voc_info.encoder_temp.pcm_data, PCM_8000, 320);
#endif
		}
	} else {
		if (aud_tras_drv_info.voc_info.aec_enable) {
			/* get data from aec_ring_buff */
			size = ring_buffer_read(&(aud_tras_drv_info.voc_info.aec_info->aec_rb), (uint8_t *)aud_tras_drv_info.voc_info.encoder_temp.pcm_data, temp_mic_samp_rate_points*2);
			if (size != temp_mic_samp_rate_points*2) {
				LOGE("read aec_rb :%d \r\n", size);
				goto encoder_exit;
			}
		} else {
			/* get data from mic_ring_buff */
			size = ring_buffer_read(&(aud_tras_drv_info.voc_info.mic_rb), (uint8_t *)aud_tras_drv_info.voc_info.encoder_temp.pcm_data, temp_mic_samp_rate_points*2);
			if (size != temp_mic_samp_rate_points*2) {
				LOGE("the data readed from mic_ring_buff is not a frame, size:%d \r\n", size);
				goto encoder_exit;
			}
		}
	}

	switch (aud_tras_drv_info.voc_info.data_type) {
		case AUD_INTF_VOC_DATA_TYPE_G711A:
			/* G711A encoding pcm data to a-law data*/
			for (i=0; i<temp_mic_samp_rate_points; i++) {
				aud_tras_drv_info.voc_info.encoder_temp.law_data[i] = linear2alaw(aud_tras_drv_info.voc_info.encoder_temp.pcm_data[i]);
			}
			break;

		case AUD_INTF_VOC_DATA_TYPE_G711U:
			/* G711U encoding pcm data to u-law data*/
			for (i=0; i<temp_mic_samp_rate_points; i++) {
				aud_tras_drv_info.voc_info.encoder_temp.law_data[i] = linear2ulaw(aud_tras_drv_info.voc_info.encoder_temp.pcm_data[i]);
			}
			break;

		case AUD_INTF_VOC_DATA_TYPE_PCM:
			break;

		default:
			break;
	}

	temp_tx_info = aud_tras_drv_info.voc_info.tx_info;
	switch (aud_tras_drv_info.voc_info.data_type) {
		case AUD_INTF_VOC_DATA_TYPE_G711A:
		case AUD_INTF_VOC_DATA_TYPE_G711U:
			os_memcpy(temp_tx_info.ping.buff_addr, aud_tras_drv_info.voc_info.encoder_temp.law_data, temp_mic_samp_rate_points);
			break;

		case AUD_INTF_VOC_DATA_TYPE_PCM:
			os_memcpy(temp_tx_info.ping.buff_addr, aud_tras_drv_info.voc_info.encoder_temp.pcm_data, temp_mic_samp_rate_points * 2);
			break;

		default:
			break;
	}

	/* dump tx data */
	if (aud_tras_drv_info.voc_info.aud_tras_dump_tx_cb) {
		aud_tras_drv_info.voc_info.aud_tras_dump_tx_cb((uint8_t *)temp_tx_info.ping.buff_addr, (uint32_t)temp_tx_info.buff_length);
	}

	/* send mic notify mailbox msg to media app */
	if (aud_tras_drv_info.aud_tras_tx_mic_data != NULL) {
		uint32_t result = BK_OK;
		mic_nofity.ptr_data = (uint32_t)temp_tx_info.ping.buff_addr;
		mic_nofity.length = (uint32_t)temp_tx_info.buff_length;
		mic_to_media_app_msg.event = EVENT_AUD_MIC_DATA_NOTIFY;
		mic_to_media_app_msg.param = (uint32_t)&mic_nofity;
		mic_to_media_app_msg.sem = mailbox_media_aud_mic_sem;
		mic_to_media_app_msg.result = (uint32_t)&result;
		//msg_send_to_app_mailbox(mic_to_media_app_msg, );
		msg_send_to_media_major_mailbox(&mic_to_media_app_msg, (uint32_t)&result, APP_MODULE);

		ret = rtos_get_semaphore(&mailbox_media_aud_mic_sem, BEKEN_WAIT_FOREVER);
		if (ret != BK_OK)
		{
			LOGE("%s, rtos_get_semaphore\n", __func__);
			ret = BK_FAIL;
		}
		else
		{
			ret = result;
		}
	}

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

	LOGI("[AUD Tx] Lost: %uB/s, Complete: %uKB/s \r\n", lost, complete);
}
#endif

#if CONFIG_AUD_TRAS_DAC_DEBUG
static bk_err_t aud_tras_voc_dac_debug(bool enable)
{
	os_printf("%s, enable:%d \r\n", __func__, enable);
	if (enable == aud_voc_dac_debug_flag)
		return BK_FAIL;

	//open dac debug
	FRESULT fr;
	if (enable) {
		/*open file to save data write to speaker */
		fr = f_open(&dac_debug_file, dac_debug_file_name, FA_CREATE_ALWAYS | FA_WRITE);
		if (fr != FR_OK) {
			LOGE("open %s fail.\r\n", dac_debug_file_name);
			return BK_FAIL;
		}
		aud_voc_dac_debug_flag = true;
		os_printf("start dac debug \r\n");
	} else {
		/*open file to save data write to speaker */
		fr = f_close(&dac_debug_file);
		if (fr != FR_OK) {
			LOGE("open %s fail.\r\n", dac_debug_file_name);
			return BK_FAIL;
		}
		aud_voc_dac_debug_flag = false;
		os_printf("stop dac debug \r\n");
	}
	return BK_OK;
}
#endif

static bk_err_t aud_tras_dec(void)
{
	uint32_t size = 0;
	uint32_t i = 0;

	if (aud_tras_drv_info.voc_info.status == AUD_TRAS_DRV_VOC_STA_NULL)
		return BK_OK;

	switch (aud_tras_drv_info.voc_info.data_type) {
		case AUD_INTF_VOC_DATA_TYPE_G711A:
		case AUD_INTF_VOC_DATA_TYPE_G711U:
			/* check the frame number in decoder_ring_buffer */
			if (aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_BOARD) {
				if (ring_buffer_get_fill_size(aud_tras_drv_info.voc_info.rx_info.decoder_rb) >= aud_tras_drv_info.voc_info.speaker_samp_rate_points) {
					//os_printf("decoder process \r\n", size);
					/* get G711A data from decoder_ring_buff */
					size = ring_buffer_read(aud_tras_drv_info.voc_info.rx_info.decoder_rb, (uint8_t*)aud_tras_drv_info.voc_info.decoder_temp.law_data, aud_tras_drv_info.voc_info.speaker_samp_rate_points);
					if (size != aud_tras_drv_info.voc_info.speaker_samp_rate_points) {
						LOGE("read decoder_ring_buff G711A data fail \r\n");
						if (aud_tras_drv_info.voc_info.data_type == AUD_INTF_VOC_DATA_TYPE_G711U)
							os_memset(aud_tras_drv_info.voc_info.decoder_temp.law_data, 0xFF, aud_tras_drv_info.voc_info.speaker_samp_rate_points);
						else
							os_memset(aud_tras_drv_info.voc_info.decoder_temp.law_data, 0xD5, aud_tras_drv_info.voc_info.speaker_samp_rate_points);
					}
				} else {
					if (aud_tras_drv_info.voc_info.data_type == AUD_INTF_VOC_DATA_TYPE_G711U)
						os_memset(aud_tras_drv_info.voc_info.decoder_temp.law_data, 0xFF, aud_tras_drv_info.voc_info.speaker_samp_rate_points);
					else
						os_memset(aud_tras_drv_info.voc_info.decoder_temp.law_data, 0xD5, aud_tras_drv_info.voc_info.speaker_samp_rate_points);
				}

				/* dump rx data */
				if (aud_tras_drv_info.voc_info.aud_tras_dump_rx_cb) {
					aud_tras_drv_info.voc_info.aud_tras_dump_rx_cb(aud_tras_drv_info.voc_info.decoder_temp.law_data, aud_tras_drv_info.voc_info.speaker_samp_rate_points);
				}

				if (aud_tras_drv_info.voc_info.data_type == AUD_INTF_VOC_DATA_TYPE_G711U) {
					/* G711U decoding u-law data to pcm data*/
					for (i=0; i<aud_tras_drv_info.voc_info.speaker_samp_rate_points; i++) {
						aud_tras_drv_info.voc_info.decoder_temp.pcm_data[i] = ulaw2linear(aud_tras_drv_info.voc_info.decoder_temp.law_data[i]);
					}
				} else {
					/* G711A decoding a-law data to pcm data*/
					for (i=0; i<aud_tras_drv_info.voc_info.speaker_samp_rate_points; i++) {
						aud_tras_drv_info.voc_info.decoder_temp.pcm_data[i] = alaw2linear(aud_tras_drv_info.voc_info.decoder_temp.law_data[i]);
					}
				}
			} else {
				if (ring_buffer_get_free_size(&aud_tras_drv_info.voc_info.speaker_rb) > aud_tras_drv_info.voc_info.speaker_samp_rate_points * 2) {
					/* check the frame number in decoder_ring_buffer */
					if (ring_buffer_get_fill_size(aud_tras_drv_info.voc_info.rx_info.decoder_rb) >= aud_tras_drv_info.voc_info.speaker_samp_rate_points) {
						//os_printf("decoder process \r\n", size);
						/* get G711A data from decoder_ring_buff */
						//addAON_GPIO_Reg0x9 = 2;
						size = ring_buffer_read(aud_tras_drv_info.voc_info.rx_info.decoder_rb, (uint8_t*)aud_tras_drv_info.voc_info.decoder_temp.law_data, aud_tras_drv_info.voc_info.speaker_samp_rate_points);
						if (size != aud_tras_drv_info.voc_info.speaker_samp_rate_points) {
							LOGE("read decoder_ring_buff G711A data fail \r\n");
							if (aud_tras_drv_info.voc_info.data_type == AUD_INTF_VOC_DATA_TYPE_G711U)
								os_memset(aud_tras_drv_info.voc_info.decoder_temp.law_data, 0xFF, aud_tras_drv_info.voc_info.speaker_samp_rate_points);
							else
								os_memset(aud_tras_drv_info.voc_info.decoder_temp.law_data, 0xD5, aud_tras_drv_info.voc_info.speaker_samp_rate_points);
						}
						//addAON_GPIO_Reg0x9 = 0;
					} else {
						if (aud_tras_drv_info.voc_info.data_type == AUD_INTF_VOC_DATA_TYPE_G711U)
							os_memset(aud_tras_drv_info.voc_info.decoder_temp.law_data, 0xFF, aud_tras_drv_info.voc_info.speaker_samp_rate_points);
						else
							os_memset(aud_tras_drv_info.voc_info.decoder_temp.law_data, 0xD5, aud_tras_drv_info.voc_info.speaker_samp_rate_points);
					}

					/* dump rx data */
					if (aud_tras_drv_info.voc_info.aud_tras_dump_rx_cb) {
						aud_tras_drv_info.voc_info.aud_tras_dump_rx_cb(aud_tras_drv_info.voc_info.decoder_temp.law_data, aud_tras_drv_info.voc_info.speaker_samp_rate_points);
					}

					if (aud_tras_drv_info.voc_info.data_type == AUD_INTF_VOC_DATA_TYPE_G711U) {
						/* G711U decoding u-law data to pcm data*/
						for (i=0; i<aud_tras_drv_info.voc_info.speaker_samp_rate_points; i++) {
							aud_tras_drv_info.voc_info.decoder_temp.pcm_data[i] = ulaw2linear(aud_tras_drv_info.voc_info.decoder_temp.law_data[i]);
						}
					} else {
						/* G711A decoding a-law data to pcm data*/
						for (i=0; i<aud_tras_drv_info.voc_info.speaker_samp_rate_points; i++) {
							aud_tras_drv_info.voc_info.decoder_temp.pcm_data[i] = alaw2linear(aud_tras_drv_info.voc_info.decoder_temp.law_data[i]);
						}
					}
				}
			}
			break;

		case AUD_INTF_VOC_DATA_TYPE_PCM:
			if (aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_BOARD) {
				if (ring_buffer_get_fill_size(aud_tras_drv_info.voc_info.rx_info.decoder_rb) >= aud_tras_drv_info.voc_info.speaker_samp_rate_points * 2) {
					//os_printf("decoder process \r\n", size);
					/* get pcm data from decoder_ring_buff */
					size = ring_buffer_read(aud_tras_drv_info.voc_info.rx_info.decoder_rb, (uint8_t*)aud_tras_drv_info.voc_info.decoder_temp.pcm_data, aud_tras_drv_info.voc_info.speaker_samp_rate_points * 2);
					if (size != aud_tras_drv_info.voc_info.speaker_samp_rate_points * 2) {
						LOGE("read decoder_ring_buff pcm data fail \r\n");
						os_memset(aud_tras_drv_info.voc_info.decoder_temp.pcm_data, 0x00, aud_tras_drv_info.voc_info.speaker_samp_rate_points * 2);
					}
				} else {
					os_memset(aud_tras_drv_info.voc_info.decoder_temp.pcm_data, 0x00, aud_tras_drv_info.voc_info.speaker_samp_rate_points * 2);
				}
				/* dump rx data */
				if (aud_tras_drv_info.voc_info.aud_tras_dump_rx_cb) {
					aud_tras_drv_info.voc_info.aud_tras_dump_rx_cb((uint8_t *)aud_tras_drv_info.voc_info.decoder_temp.pcm_data, aud_tras_drv_info.voc_info.speaker_samp_rate_points * 2);
				}
			} else {
				if (ring_buffer_get_free_size(&aud_tras_drv_info.voc_info.speaker_rb) > aud_tras_drv_info.voc_info.speaker_samp_rate_points * 2) {
					/* check the frame number in decoder_ring_buffer */
					if (ring_buffer_get_fill_size(aud_tras_drv_info.voc_info.rx_info.decoder_rb) >= aud_tras_drv_info.voc_info.speaker_samp_rate_points * 2) {
						//os_printf("decoder process \r\n", size);
						/* get pcm data from decoder_ring_buff */
						size = ring_buffer_read(aud_tras_drv_info.voc_info.rx_info.decoder_rb, (uint8_t*)aud_tras_drv_info.voc_info.decoder_temp.pcm_data, aud_tras_drv_info.voc_info.speaker_samp_rate_points * 2);
						if (size != aud_tras_drv_info.voc_info.speaker_samp_rate_points * 2) {
							LOGE("read decoder_ring_buff pcm data fail \r\n");
							os_memset(aud_tras_drv_info.voc_info.decoder_temp.pcm_data, 0x00, aud_tras_drv_info.voc_info.speaker_samp_rate_points * 2);
						}
					} else {
						os_memset(aud_tras_drv_info.voc_info.decoder_temp.pcm_data, 0x00, aud_tras_drv_info.voc_info.speaker_samp_rate_points * 2);
					}

					/* dump rx data */
					if (aud_tras_drv_info.voc_info.aud_tras_dump_rx_cb) {
						aud_tras_drv_info.voc_info.aud_tras_dump_rx_cb((uint8_t *)aud_tras_drv_info.voc_info.decoder_temp.pcm_data, aud_tras_drv_info.voc_info.speaker_samp_rate_points * 2);
					}
				}
			}
			break;

		default:
			break;
	}

#if CONFIG_AUD_TRAS_AEC_MIC_DELAY_DEBUG
	mic_delay_num++;
	os_memset(aud_tras_drv_info.voc_info.decoder_temp.pcm_data, 0, aud_tras_drv_info.voc_info.speaker_samp_rate_points*2);
	if (mic_delay_num == 50) {
		aud_tras_drv_info.voc_info.decoder_temp.pcm_data[0] = 0x2FFF;
		mic_delay_num = 0;
		LOGI("mic_delay_num");
	}
#endif

	if (aud_tras_drv_info.voc_info.aec_enable) {
		/* read mic fill data size */
		uint32_t mic_fill_size = ring_buffer_get_fill_size(&(aud_tras_drv_info.voc_info.mic_rb));
		//os_printf("mic_rb: fill_size=%d \r\n", mic_fill_size);
		uint32_t speaker_fill_size = ring_buffer_get_fill_size(&(aud_tras_drv_info.voc_info.speaker_rb));
		//os_printf("speaker_rb: fill_size=%d \r\n", speaker_fill_size);
		uint32_t ref_fill_size = ring_buffer_get_fill_size(&(aud_tras_drv_info.voc_info.aec_info->ref_rb));
		//os_printf("ref_rb: fill_size=%d \r\n", ref_fill_size);
		/* 设置参考信号延迟(采样点数，需要dump数据观察) */
#if CONFIG_AUD_TRAS_AEC_MIC_DELAY_DEBUG
		os_printf("MIC_DELAY: %d \r\n", (mic_fill_size + speaker_fill_size - ref_fill_size)/2);
#endif
		if ((mic_fill_size + speaker_fill_size - ref_fill_size)/2 < 0) {
			LOGE("MIC_DELAY is error \r\n", ref_fill_size);
			aec_ctrl(aud_tras_drv_info.voc_info.aec_info->aec, AEC_CTRL_CMD_SET_MIC_DELAY, 0);
		} else {
			aec_ctrl(aud_tras_drv_info.voc_info.aec_info->aec, AEC_CTRL_CMD_SET_MIC_DELAY, (mic_fill_size + speaker_fill_size - ref_fill_size)/2 + CONFIG_AUD_TRAS_AEC_MIC_DELAY_POINTS);
		}

		if (ring_buffer_get_free_size(&(aud_tras_drv_info.voc_info.aec_info->ref_rb)) > aud_tras_drv_info.voc_info.aec_info->samp_rate_points*2) {
			size = ring_buffer_write(&(aud_tras_drv_info.voc_info.aec_info->ref_rb), (uint8_t *)aud_tras_drv_info.voc_info.decoder_temp.pcm_data, aud_tras_drv_info.voc_info.aec_info->samp_rate_points*2);
			if (size != aud_tras_drv_info.voc_info.aec_info->samp_rate_points*2) {
				LOGE("write data to ref_ring_buff fail, size=%d \r\n", size);
				goto decoder_exit;
			}
		}


	}

#if CONFIG_AUD_TRAS_DAC_DEBUG
	if (aud_voc_dac_debug_flag) {
		//dump the data write to speaker
		FRESULT fr;
		uint32 uiTemp = 0;
		uint32_t i = 0, j = 0;
		/* write data to file */
		fr = f_write(&dac_debug_file, (uint32_t *)aud_tras_drv_info.voc_info.decoder_temp.pcm_data, aud_tras_drv_info.voc_info.speaker_samp_rate_points*2, &uiTemp);
		if (fr != FR_OK) {
			LOGE("write %s fail.\r\n", dac_debug_file_name);
			return BK_FAIL;
		}

		//write 8K sin data
		for (i = 0; i < aud_tras_drv_info.voc_info.speaker_samp_rate_points*2; i++) {
			for (j = 0; j < 8; j++) {
				*(uint32_t *)0x47800048 = PCM_8000[j];
			}
			i += 8;
		}
	} else 
#endif
	{
	/* save the data after G711A processed to encoder_ring_buffer */
		if (ring_buffer_get_free_size(&(aud_tras_drv_info.voc_info.speaker_rb)) > aud_tras_drv_info.voc_info.speaker_samp_rate_points*2) {
#if AUD_MIC_DEBUG
			size = ring_buffer_write(&(aud_tras_drv_info.voc_info.speaker_rb), (uint8_t *)PCM_8000, aud_tras_drv_info.voc_info.speaker_samp_rate_points*2);
			if (size != aud_tras_drv_info.voc_info.speaker_samp_rate_points*2) {
				LOGE("the data writeten to speaker_ring_buff is not a frame, size=%d \r\n", size);
				goto decoder_exit;
			}
#else
			size = ring_buffer_write(&(aud_tras_drv_info.voc_info.speaker_rb), (uint8_t *)aud_tras_drv_info.voc_info.decoder_temp.pcm_data, aud_tras_drv_info.voc_info.speaker_samp_rate_points*2);
			if (size != aud_tras_drv_info.voc_info.speaker_samp_rate_points*2) {
				LOGE("the data writeten to speaker_ring_buff is not a frame, size=%d \r\n", size);
				goto decoder_exit;
			}

#endif
			aud_tras_drv_info.voc_info.rx_info.aud_trs_read_seq++;
		}
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
			LOGE("read mic_ring_buff fail, size:%d \r\n", size);
		}
	}

	/* call sendto api of lwip UDP */
	if (aud_tras_drv_info.aud_tras_tx_mic_data != NULL) {
		size = aud_tras_drv_info.aud_tras_tx_mic_data((uint8_t *)aud_tras_drv_info.mic_info.temp_mic_addr, (uint32_t)aud_tras_drv_info.mic_info.frame_size);
		if (size != aud_tras_drv_info.mic_info.frame_size) {
			LOGE("audio mic Tx lost \r\n");
		}
	}

	return ret;
}

static bk_err_t aud_tras_drv_spk_req_data(void)
{
	bk_err_t ret = BK_OK;
	uint32_t size = 0;

	if (aud_tras_drv_info.spk_info.spk_type == AUD_INTF_SPK_TYPE_BOARD) {
		/* get speaker data from spk_rx_ring_buff */
		if (ring_buffer_get_fill_size(aud_tras_drv_info.spk_info.spk_rx_rb) >= aud_tras_drv_info.spk_info.frame_size) {
			size = ring_buffer_read(aud_tras_drv_info.spk_info.spk_rx_rb, (uint8_t*)aud_tras_drv_info.spk_info.temp_spk_addr, aud_tras_drv_info.spk_info.frame_size);
			if (size != aud_tras_drv_info.spk_info.frame_size) {
				LOGE("read spk_rx_ring_buff fail, size:%d \r\n", size);
				os_memset(aud_tras_drv_info.spk_info.temp_spk_addr, 0, aud_tras_drv_info.spk_info.frame_size);
			}
		} else {
			LOGW("spk_rx_rb is empty \r\n");
			os_memset(aud_tras_drv_info.spk_info.temp_spk_addr, 0, aud_tras_drv_info.spk_info.frame_size);
		}

		/* write spk_rx_data to audio dac */
		size = ring_buffer_write(&(aud_tras_drv_info.spk_info.spk_rb), (uint8_t*)aud_tras_drv_info.spk_info.temp_spk_addr, aud_tras_drv_info.spk_info.frame_size);
		if (size != aud_tras_drv_info.spk_info.frame_size) {
			LOGE("write spk_data to audio dac fail, size:%d \r\n", size);
			//TODO
			//add handle code
		}

		/* call callback to notify app */
		if (aud_tras_drv_info.aud_tras_rx_spk_data)
			ret = aud_tras_drv_info.aud_tras_rx_spk_data((unsigned int)aud_tras_drv_info.spk_info.frame_size);
	} else {
		if (ring_buffer_get_fill_size(&aud_tras_drv_info.spk_info.spk_rb) >= aud_tras_drv_info.spk_info.uac_spk_buff_size) {
			/* get pcm data from spk_rb */
			size = ring_buffer_read(&aud_tras_drv_info.spk_info.spk_rb, aud_tras_drv_info.spk_info.uac_spk_buff, aud_tras_drv_info.spk_info.uac_spk_buff_size);
			if (size != aud_tras_drv_info.spk_info.uac_spk_buff_size) {
				LOGE("read spk_rb uac spk data fail \r\n");
				os_memset(aud_tras_drv_info.spk_info.uac_spk_buff, 0x00, aud_tras_drv_info.spk_info.uac_spk_buff_size);
			}
		} else {
			os_memset(aud_tras_drv_info.spk_info.uac_spk_buff, 0x00, aud_tras_drv_info.spk_info.uac_spk_buff_size);
		}

		/* save the data after G711A processed to encoder_ring_buffer */
		if (ring_buffer_get_free_size(&(aud_tras_drv_info.spk_info.spk_rb)) > aud_tras_drv_info.spk_info.frame_size) {
			/* get speaker data from spk_rx_ring_buff */
			if (ring_buffer_get_fill_size(aud_tras_drv_info.spk_info.spk_rx_rb) >= aud_tras_drv_info.spk_info.frame_size) {
				size = ring_buffer_read(aud_tras_drv_info.spk_info.spk_rx_rb, (uint8_t*)aud_tras_drv_info.spk_info.temp_spk_addr, aud_tras_drv_info.spk_info.frame_size);
				if (size != aud_tras_drv_info.spk_info.frame_size) {
					LOGE("read spk_rx_ring_buff fail, size:%d \r\n", size);
					os_memset(aud_tras_drv_info.spk_info.temp_spk_addr, 0, aud_tras_drv_info.spk_info.frame_size);
				}
			} else {
				LOGW("spk_rx_rb is empty \r\n");
				os_memset(aud_tras_drv_info.spk_info.temp_spk_addr, 0, aud_tras_drv_info.spk_info.frame_size);
			}

			/* write spk_rx_data to audio dac */
			size = ring_buffer_write(&(aud_tras_drv_info.spk_info.spk_rb), (uint8_t*)aud_tras_drv_info.spk_info.temp_spk_addr, aud_tras_drv_info.spk_info.frame_size);
			if (size != aud_tras_drv_info.spk_info.frame_size) {
				LOGE("write spk_data to audio dac fail, size:%d \r\n", size);
				//TODO
				//add handle code
			}

			/* call callback to notify app */
			if (aud_tras_drv_info.aud_tras_rx_spk_data)
				ret = aud_tras_drv_info.aud_tras_rx_spk_data((unsigned int)aud_tras_drv_info.spk_info.frame_size);
		}
	}

	return ret;
}

static void aud_tras_drv_spk_uac_spk_cb(void)
{
	bk_err_t ret = BK_OK;

	/* send msg to notify app to write speaker data */
	ret = aud_tras_drv_send_msg(AUD_TRAS_DRV_SPK_REQ_DATA, NULL);
	if (ret != kNoErr) {
		LOGE("send msg: AUD_TRAS_DRV_SPK_REQ_DATA fails \r\n");
	}
}

#if 0
static uint32_t transfer_spk_samp_rate_to_value(aud_dac_samp_rate_t samp_rate)
{
	uint32_t samp_rate_value = 0;

	switch (samp_rate) {
		case AUD_DAC_SAMP_RATE_8K:
			samp_rate_value = 8000;
			break;

		case AUD_DAC_SAMP_RATE_12K:
			samp_rate_value = 12000;
			break;

		case AUD_DAC_SAMP_RATE_16K:
			samp_rate_value = 16000;
			break;

		case AUD_DAC_SAMP_RATE_24K:
			samp_rate_value = 24000;
			break;

		case AUD_DAC_SAMP_RATE_32K:
			samp_rate_value = 32000;
			break;

		case AUD_DAC_SAMP_RATE_48K:
			samp_rate_value = 48000;
			break;

		default:
			samp_rate_value = 16000;
			break;
	}
	return samp_rate_value;
}
#endif

static bk_err_t uac_set_spk_samp_rate(aud_dac_samp_rate_t samp_rate)
{
	uint32_t samp_rate_value = 0;

	switch (samp_rate) {
		case AUD_DAC_SAMP_RATE_8K:
			samp_rate_value = 8000;
			break;

		case AUD_DAC_SAMP_RATE_12K:
			samp_rate_value = 12000;
			break;

		case AUD_DAC_SAMP_RATE_16K:
			samp_rate_value = 16000;
			break;

		case AUD_DAC_SAMP_RATE_24K:
			samp_rate_value = 24000;
			break;

		case AUD_DAC_SAMP_RATE_32K:
			samp_rate_value = 32000;
			break;

		case AUD_DAC_SAMP_RATE_48K:
			samp_rate_value = 48000;
			break;

		default:
			samp_rate_value = 16000;
			break;
	}
	return bk_aud_uac_set_spk_samp_rate(samp_rate_value);
}

static bk_err_t aud_tras_drv_spk_init(aud_intf_spk_config_t *spk_cfg)
{
	bk_err_t ret = BK_OK;
	bk_err_t err = BK_ERR_AUD_INTF_FAIL;

	/* get callback */
//	aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb = spk_cfg->aud_tras_drv_spk_event_cb;

	aud_tras_drv_info.spk_info.spk_type = spk_cfg->spk_type;
	if (aud_tras_drv_info.spk_info.spk_type == AUD_INTF_SPK_TYPE_BOARD) {
		/* get audio dac config */
		aud_tras_drv_info.spk_info.dac_config = os_malloc(sizeof(aud_dac_config_t));
		if (aud_tras_drv_info.spk_info.dac_config == NULL) {
			LOGE("adc_config os_malloc fail \r\n");
			err = BK_ERR_AUD_INTF_MEMY;
			goto aud_tras_drv_spk_init_exit;
		} else {
			aud_tras_drv_info.spk_info.dac_config->dac_enable = AUD_DAC_DISABLE;
			aud_tras_drv_info.spk_info.dac_config->samp_rate = spk_cfg->samp_rate;
			if (spk_cfg->spk_chl == AUD_INTF_SPK_CHL_LEFT)
				aud_tras_drv_info.spk_info.dac_config->dac_chl = AUD_DAC_CHL_L_ENABLE;
			else
				aud_tras_drv_info.spk_info.dac_config->dac_chl = AUD_DAC_CHL_LR_ENABLE;
			aud_tras_drv_info.spk_info.dac_config->work_mode = spk_cfg->work_mode;
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
		}
		aud_tras_drv_info.spk_info.spk_en = true;
	} else {
		/* set audio uac config */
		if (aud_tras_drv_info.spk_info.uac_config == NULL) {
			aud_tras_drv_info.spk_info.uac_config = os_malloc(sizeof(aud_uac_config_t));
			if (aud_tras_drv_info.spk_info.uac_config == NULL) {
				LOGE("uac_config os_malloc fail \r\n");
				err = BK_ERR_AUD_INTF_MEMY;
				goto aud_tras_drv_spk_init_exit;
			} else {
				aud_tras_drv_info.spk_info.uac_config->mic_samp_rate = AUD_ADC_SAMP_RATE_MAX;
				aud_tras_drv_info.spk_info.uac_config->spk_samp_rate = AUD_DAC_SAMP_RATE_MAX;
				aud_tras_drv_info.spk_info.uac_config->spk_gain = 0;
			}
			aud_tras_drv_info.spk_info.uac_config->spk_samp_rate = spk_cfg->samp_rate;
			aud_tras_drv_info.spk_info.uac_config->spk_gain = spk_cfg->spk_gain;
		}
	}

	aud_tras_drv_info.spk_info.spk_chl = spk_cfg->spk_chl;
	aud_tras_drv_info.spk_info.frame_size = spk_cfg->frame_size;
	aud_tras_drv_info.spk_info.fifo_frame_num = spk_cfg->fifo_frame_num;
	aud_tras_drv_info.spk_info.spk_rx_rb = spk_cfg->spk_rx_rb;
	aud_tras_drv_info.spk_info.spk_rx_ring_buff = spk_cfg->spk_rx_ring_buff;

	/* malloc spk_ring_buff to save audio data */
	aud_tras_drv_info.spk_info.spk_ring_buff = (int32_t *)os_malloc(aud_tras_drv_info.spk_info.frame_size * 2 + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL);
	if (aud_tras_drv_info.spk_info.spk_ring_buff == NULL) {
		LOGE("malloc speakerk ring buffer fail \r\n");
		err = BK_ERR_AUD_INTF_MEMY;
		goto aud_tras_drv_spk_init_exit;
	}

	if (aud_tras_drv_info.spk_info.spk_type == AUD_INTF_SPK_TYPE_BOARD) {
		/* audio dac config */
		ret = aud_tras_dac_config(aud_tras_drv_info.spk_info.dac_config);
		if (ret != BK_OK) {
			LOGE("audio dac init fail \r\n");
			err = BK_ERR_AUD_INTF_DAC;
			goto aud_tras_drv_spk_init_exit;
		}
		LOGI("step1: init audio and config DAC complete \r\n");

		/* init dma driver */
		ret = bk_dma_driver_init();
		if (ret != BK_OK) {
			LOGE("dma driver init failed\r\n");
			err = BK_ERR_AUD_INTF_DMA;
			goto aud_tras_drv_spk_init_exit;
		}

		/* allocate free DMA channel */
		aud_tras_drv_info.spk_info.spk_dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
		if ((aud_tras_drv_info.spk_info.spk_dma_id < DMA_ID_0) || (aud_tras_drv_info.spk_info.spk_dma_id >= DMA_ID_MAX)) {
			LOGE("malloc dac dma fail \r\n");
			err = BK_ERR_AUD_INTF_DMA;
			goto aud_tras_drv_spk_init_exit;
		}

		/* config audio dac dma to carry speaker data to "spk_ring_buff" */
		ret = aud_tras_dac_dma_config(aud_tras_drv_info.spk_info.spk_dma_id, aud_tras_drv_info.spk_info.spk_ring_buff, aud_tras_drv_info.spk_info.frame_size * 2, aud_tras_drv_info.spk_info.frame_size, aud_tras_drv_info.spk_info.spk_chl);
		if (ret != BK_ERR_AUD_INTF_OK) {
			LOGE("config audio dac dma fail \r\n");
			ret = ret;
			goto aud_tras_drv_spk_init_exit;
		}

		/* init spk_ring_buff */
		ring_buffer_init(&(aud_tras_drv_info.spk_info.spk_rb), (uint8_t*)aud_tras_drv_info.spk_info.spk_ring_buff, aud_tras_drv_info.spk_info.frame_size * 2 + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL, aud_tras_drv_info.spk_info.spk_dma_id, RB_DMA_TYPE_READ);
		LOGI("step2: init dma:%d, and spk ring buff:%p, size:%d complete \r\n", aud_tras_drv_info.spk_info.spk_dma_id, aud_tras_drv_info.spk_info.spk_ring_buff, aud_tras_drv_info.spk_info.frame_size * 2);
	} else if (aud_tras_drv_info.spk_info.spk_type == AUD_INTF_SPK_TYPE_UAC) {
		/* init spk_ring_buff */
		ring_buffer_init(&(aud_tras_drv_info.spk_info.spk_rb), (uint8_t*)aud_tras_drv_info.spk_info.spk_ring_buff, aud_tras_drv_info.spk_info.frame_size*2 + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL, DMA_ID_MAX, RB_DMA_TYPE_NULL);

		/* close spk */
		aud_tras_drv_info.spk_info.spk_en = false;

		switch (spk_cfg->samp_rate) {
			case AUD_DAC_SAMP_RATE_8K:
				aud_tras_drv_info.spk_info.uac_spk_buff_size = 16;
				break;

			case AUD_DAC_SAMP_RATE_12K:
				aud_tras_drv_info.spk_info.uac_spk_buff_size = 24;
				break;

			case AUD_DAC_SAMP_RATE_16K:
				aud_tras_drv_info.spk_info.uac_spk_buff_size = 32;
				break;

			case AUD_DAC_SAMP_RATE_24K:
				aud_tras_drv_info.spk_info.uac_spk_buff_size = 48;
				break;

			case AUD_DAC_SAMP_RATE_32K:
				aud_tras_drv_info.spk_info.uac_spk_buff_size = 64;
				break;

			case AUD_DAC_SAMP_RATE_48K:
				aud_tras_drv_info.spk_info.uac_spk_buff_size = 96;
				break;

			default:
				aud_tras_drv_info.spk_info.uac_spk_buff_size = 32;
				break;
		}
		aud_tras_drv_info.spk_info.uac_spk_buff = (uint8_t *)os_malloc(aud_tras_drv_info.spk_info.uac_spk_buff_size);

		bk_aud_uac_register_spk_buff_ptr(aud_tras_drv_info.spk_info.uac_spk_buff, aud_tras_drv_info.spk_info.uac_spk_buff_size);

		bk_aud_uac_register_spk_callback((void *)aud_tras_drv_spk_uac_spk_cb);

		/* register uac connect and disconnect callback */
		bk_aud_uac_register_disconnect_cb(aud_tras_uac_disconnect_cb);
		bk_aud_uac_register_connect_cb(aud_tras_uac_connect_cb);

		LOGI("init uac driver \r\n");
		ret = bk_aud_uac_driver_init();
		if (ret != BK_OK) {
			LOGE("init uac driver fail \r\n");
			err = BK_ERR_AUD_INTF_UAC_DRV;
			goto aud_tras_drv_spk_init_exit;
		}
		aud_tras_drv_info.uac_status = AUD_INTF_UAC_CONNECTED;

		ret = uac_set_spk_samp_rate(aud_tras_drv_info.spk_info.uac_config->spk_samp_rate);
		if (ret != BK_OK) {
			LOGE("init uac spk samp rate fail \r\n");
			err = BK_ERR_AUD_INTF_PARAM;
			goto aud_tras_drv_spk_init_exit;
		}

		/* set uac speaker volume */
		//bk_aud_uac_set_spk_gain(aud_tras_drv_info.spk_info.uac_config->spk_gain);
		aud_tras_drv_set_spk_gain(aud_tras_drv_info.spk_info.uac_config->spk_gain);

		LOGI("step2: init spk ring buff:%p, size:%d complete \r\n", aud_tras_drv_info.spk_info.spk_ring_buff, aud_tras_drv_info.spk_info.frame_size*2 + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL);
	} else {
		err = BK_ERR_AUD_INTF_PARAM;
		goto aud_tras_drv_spk_init_exit;
	}

	/* init spk temp buffer */
	aud_tras_drv_info.spk_info.temp_spk_addr = (int32_t *)os_malloc(aud_tras_drv_info.spk_info.frame_size);
	if (aud_tras_drv_info.spk_info.temp_spk_addr == NULL) {
		LOGE("malloc temp spk ring buffer fail \r\n");
		err = BK_ERR_AUD_INTF_MEMY;
		goto aud_tras_drv_spk_init_exit;
	}
	LOGI("step3: init temp spk ring buff complete \r\n");

	/* change status: AUD_TRAS_DRV_SPK_NULL --> AUD_TRAS_DRV_SPK_IDLE */
	aud_tras_drv_info.spk_info.status = AUD_TRAS_DRV_SPK_STA_IDLE;
/*
	if (aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb)
		aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb(EVENT_AUD_TRAS_SPK_INIT, BK_ERR_AUD_INTF_OK);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);

	LOGI("step4: init spk complete \r\n");

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

	if (aud_tras_drv_info.spk_info.uac_config)
		os_free(aud_tras_drv_info.spk_info.uac_config);
	aud_tras_drv_info.spk_info.uac_config = NULL;

	LOGE("init spk fail \r\n");
/*
	if (aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb)
		aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb(EVENT_AUD_TRAS_SPK_INIT, err);
	aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb = NULL;
*/
//	msg_send_to_app_mailbox(aud_to_media_app_msg, err);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, err, APP_MODULE);

	return err;
}

static bk_err_t aud_tras_drv_spk_deinit(void)
{
	if (aud_tras_drv_info.spk_info.spk_type == AUD_INTF_SPK_TYPE_BOARD) {
		/* disable audio dac */
		bk_aud_stop_dac();
		aud_tras_dac_pa_ctrl(false);
		bk_dma_stop(aud_tras_drv_info.spk_info.spk_dma_id);
		bk_dma_deinit(aud_tras_drv_info.spk_info.spk_dma_id);
		bk_dma_free(DMA_DEV_AUDIO, aud_tras_drv_info.spk_info.spk_dma_id);
		bk_aud_dac_deinit();
		if (aud_tras_drv_info.mic_info.status == AUD_TRAS_DRV_MIC_STA_NULL)
			bk_aud_driver_deinit();
		os_free(aud_tras_drv_info.spk_info.dac_config);
		aud_tras_drv_info.spk_info.dac_config = NULL;
	} else {
		aud_tras_drv_info.uac_status = AUD_INTF_UAC_NORMAL_DISCONNECTED;
		bk_aud_uac_stop_spk();
		bk_aud_uac_unregister_spk_callback();
		bk_aud_uac_driver_deinit();

		os_free(aud_tras_drv_info.spk_info.uac_spk_buff);
		aud_tras_drv_info.spk_info.uac_spk_buff = NULL;
		aud_tras_drv_info.spk_info.uac_spk_buff_size = 0;
	}

	aud_tras_drv_info.spk_info.spk_en = false;
	aud_tras_drv_info.spk_info.spk_chl = AUD_INTF_MIC_CHL_MIC1;
	aud_tras_drv_info.spk_info.frame_size = 0;
	aud_tras_drv_info.spk_info.spk_dma_id = DMA_ID_MAX;

	if (aud_tras_drv_info.spk_info.uac_config)
		os_free(aud_tras_drv_info.spk_info.uac_config);
	aud_tras_drv_info.spk_info.uac_config = NULL;

	ring_buffer_clear(&(aud_tras_drv_info.spk_info.spk_rb));
	os_free(aud_tras_drv_info.spk_info.spk_ring_buff);
	aud_tras_drv_info.spk_info.spk_ring_buff = NULL;

	os_free(aud_tras_drv_info.spk_info.temp_spk_addr);
	aud_tras_drv_info.spk_info.temp_spk_addr = NULL;

	aud_tras_drv_info.spk_info.status = AUD_TRAS_DRV_SPK_STA_NULL;

/*
	if (aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb)
		aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb(EVENT_AUD_TRAS_SPK_DEINIT, BK_ERR_AUD_INTF_OK);
	aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb = NULL;
*/
	return BK_ERR_AUD_INTF_OK;
}

static bk_err_t aud_tras_drv_spk_start(void)
{
	bk_err_t ret = BK_OK;
	bk_err_t err = BK_ERR_AUD_INTF_FAIL;
	uint32_t size = 0;

	if ((aud_tras_drv_info.spk_info.status != AUD_TRAS_DRV_SPK_STA_IDLE) && (aud_tras_drv_info.spk_info.status != AUD_TRAS_DRV_SPK_STA_PAUSE)) {
		err = BK_ERR_AUD_INTF_STA;
		goto spk_start_err;
	}

	if (aud_tras_drv_info.spk_info.spk_type == AUD_INTF_SPK_TYPE_BOARD) {
		/* enable dac */
		bk_aud_start_dac();
		bk_aud_start_dac();

		aud_tras_dac_pa_ctrl(true);

		ret = bk_dma_start(aud_tras_drv_info.spk_info.spk_dma_id);
		if (ret != BK_OK) {
			LOGE("start speaker dma fail \r\n");
			err = BK_ERR_AUD_INTF_DMA;
			goto spk_start_err;
		}
	} else {
		LOGI("start uac spk \r\n");
		bk_aud_uac_start_spk();
	}

	if (aud_tras_drv_info.spk_info.status == AUD_TRAS_DRV_SPK_STA_IDLE) {
		/* get speaker data from spk_rx_ring_buff */
		if (ring_buffer_get_fill_size(aud_tras_drv_info.spk_info.spk_rx_rb) >= aud_tras_drv_info.spk_info.frame_size) {
			size = ring_buffer_read(aud_tras_drv_info.spk_info.spk_rx_rb, (uint8_t*)aud_tras_drv_info.spk_info.temp_spk_addr, aud_tras_drv_info.spk_info.frame_size);
			if (size != aud_tras_drv_info.spk_info.frame_size) {
				LOGE("read spk_rx_ring_buff fail, size:%d \r\n", size);
			}
		} else {
			LOGW("spk_rx_rb is empty \r\n");
			os_memset(aud_tras_drv_info.spk_info.temp_spk_addr, 0, aud_tras_drv_info.spk_info.frame_size);
		}

		/* write spk_rx_data to audio dac */
		size = ring_buffer_write(&(aud_tras_drv_info.spk_info.spk_rb), (uint8_t*)aud_tras_drv_info.spk_info.temp_spk_addr, aud_tras_drv_info.spk_info.frame_size);
		if (size != aud_tras_drv_info.spk_info.frame_size) {
			LOGE("write spk_data to audio dac fail, size:%d \r\n", size);
			//return BK_FAIL;
		}

		if (ring_buffer_get_fill_size(aud_tras_drv_info.spk_info.spk_rx_rb) >= aud_tras_drv_info.spk_info.frame_size) {
			size = ring_buffer_read(aud_tras_drv_info.spk_info.spk_rx_rb, (uint8_t*)aud_tras_drv_info.spk_info.temp_spk_addr, aud_tras_drv_info.spk_info.frame_size);
			if (size != aud_tras_drv_info.spk_info.frame_size) {
				LOGE("read spk_rx_ring_buff fail, size:%d \r\n", size);
			}
		} else {
			LOGW("spk_rx_rb is empty \r\n");
			os_memset(aud_tras_drv_info.spk_info.temp_spk_addr, 0, aud_tras_drv_info.spk_info.frame_size);
		}

		/* write spk_rx_data to audio dac */
		size = ring_buffer_write(&(aud_tras_drv_info.spk_info.spk_rb), (uint8_t*)aud_tras_drv_info.spk_info.temp_spk_addr, aud_tras_drv_info.spk_info.frame_size);
		if (size != aud_tras_drv_info.spk_info.frame_size) {
			LOGE("write spk_data to audio dac fail, size:%d \r\n", size);
			//return BK_FAIL;
		}
	}

	aud_tras_drv_info.spk_info.status = AUD_TRAS_DRV_SPK_STA_START;
/*
	if (aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb)
		aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb(EVENT_AUD_TRAS_SPK_START, BK_ERR_AUD_INTF_OK);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);

	return BK_ERR_AUD_INTF_OK;
spk_start_err:
	if (aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb)
		aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb(EVENT_AUD_TRAS_SPK_START, err);
	//msg_send_to_app_mailbox(aud_to_media_app_msg, err);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, err, APP_MODULE);

	return err;
}

/* not stop dma, only disable adc */
static bk_err_t aud_tras_drv_spk_pause(void)
{
	bk_err_t err = BK_ERR_AUD_INTF_FAIL;
	if (aud_tras_drv_info.spk_info.status != AUD_TRAS_DRV_SPK_STA_START) {
		err = BK_ERR_AUD_INTF_STA;
		goto spk_pause_err;
	}

	if (aud_tras_drv_info.spk_info.spk_type == AUD_INTF_SPK_TYPE_BOARD) {
		/* disable adc */
		bk_aud_stop_dac();
		aud_tras_dac_pa_ctrl(false);
		bk_dma_stop(aud_tras_drv_info.spk_info.spk_dma_id);
	} else {
		bk_aud_uac_stop_spk();
	}

	aud_tras_drv_info.spk_info.status = AUD_TRAS_DRV_SPK_STA_PAUSE;
/*
	if (aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb)
		aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb(EVENT_AUD_TRAS_SPK_PAUSE, BK_ERR_AUD_INTF_OK);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);
	return BK_ERR_AUD_INTF_OK;

spk_pause_err:
/*
	if (aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb)
		aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb(EVENT_AUD_TRAS_SPK_STOP, err);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, err);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, err, APP_MODULE);

	return err;
}

static bk_err_t aud_tras_drv_spk_stop(void)
{
	bk_err_t err = BK_ERR_AUD_INTF_FAIL;
	if ((aud_tras_drv_info.spk_info.status != AUD_TRAS_DRV_SPK_STA_START) && (aud_tras_drv_info.spk_info.status != AUD_TRAS_DRV_SPK_STA_PAUSE)) {
		err = BK_ERR_AUD_INTF_STA;
		goto spk_stop_err;
	}

	if (aud_tras_drv_info.spk_info.spk_type == AUD_INTF_SPK_TYPE_BOARD) {
		/* disable adc */
		bk_aud_stop_dac();
		aud_tras_dac_pa_ctrl(false);
		bk_dma_stop(aud_tras_drv_info.spk_info.spk_dma_id);
	} else {
		bk_aud_uac_stop_spk();
	}

	ring_buffer_clear(&(aud_tras_drv_info.spk_info.spk_rb));
	aud_tras_drv_info.spk_info.status = AUD_TRAS_DRV_SPK_STA_IDLE;
/*
	if (aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb)
		aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb(EVENT_AUD_TRAS_SPK_STOP, BK_ERR_AUD_INTF_OK);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);
	return BK_ERR_AUD_INTF_OK;

spk_stop_err:
/*
	if (aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb)
		aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb(EVENT_AUD_TRAS_SPK_STOP, err);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, err);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, err, APP_MODULE);

	return err;
}

static bk_err_t aud_tras_drv_spk_set_chl(aud_intf_spk_chl_t spk_chl)
{
	bk_err_t ret = BK_ERR_AUD_INTF_OK;
	switch (spk_chl) {
		case AUD_INTF_SPK_CHL_LEFT:
			ret = bk_aud_set_dac_chl(AUD_DAC_CHL_L_ENABLE);
			if (ret == BK_OK)
				ret = bk_dma_set_dest_data_width(aud_tras_drv_info.mic_info.mic_dma_id, DMA_DATA_WIDTH_16BITS);
			break;

		case AUD_INTF_SPK_CHL_DUAL:
			ret = bk_aud_set_dac_chl(AUD_DAC_CHL_LR_ENABLE);
			if (ret == BK_OK)
				ret = bk_dma_set_dest_data_width(aud_tras_drv_info.mic_info.mic_dma_id, DMA_DATA_WIDTH_32BITS);
			break;

		default:
			break;
	}
/*
	if (aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb)
		aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb(EVENT_AUD_TRAS_SPK_SET_CHL, ret);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, ret);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, ret, APP_MODULE);

	return ret;
}

static void aud_tras_drv_mic_uac_mic_cb(uint8_t *buff, uint32_t count)
{
	bk_err_t ret = BK_OK;
	uint32_t write_size = 0;
	uint32_t data = 0;

	if (aud_tras_drv_info.mic_info.status == AUD_TRAS_DRV_MIC_STA_START && count > 0) {
		if (ring_buffer_get_free_size(&aud_tras_drv_info.mic_info.mic_rb) >= count) {
			for (write_size = 0; write_size < count/4; write_size++) {
				data = *((uint32_t *)buff);
				ring_buffer_write(&aud_tras_drv_info.mic_info.mic_rb, (uint8_t *)&data, 4);
			}
		}
	}

	if (uac_mic_read_flag && (ring_buffer_get_fill_size(&aud_tras_drv_info.mic_info.mic_rb) < aud_tras_drv_info.mic_info.frame_size))
		uac_mic_read_flag = false;

	if ((ring_buffer_get_fill_size(&aud_tras_drv_info.mic_info.mic_rb) > aud_tras_drv_info.mic_info.frame_size) && (uac_mic_read_flag == false)) {
		uac_mic_read_flag = true;
		/* send msg to TX_DATA to process mic data */
		ret = aud_tras_drv_send_msg(AUD_TRAS_DRV_MIC_TX_DATA, NULL);
		if (ret != kNoErr) {
			LOGE("send msg: AUD_TRAS_DRV_MIC_TX_DATA fail \r\n");
			uac_mic_read_flag = false;
		}
	}
}

static bk_err_t uac_set_mic_samp_rate(aud_adc_samp_rate_t samp_rate)
{
	uint32_t samp_rate_value = 0;

	switch (samp_rate) {
		case AUD_ADC_SAMP_RATE_8K:
			samp_rate_value = 8000;
			break;

		case AUD_ADC_SAMP_RATE_16K:
			samp_rate_value = 16000;
			break;

		case AUD_ADC_SAMP_RATE_44_1K:
			samp_rate_value = 44100;
			break;

		case AUD_ADC_SAMP_RATE_48K:
			samp_rate_value = 48000;
			break;

		case AUD_ADC_SAMP_RATE_11_025K:
			samp_rate_value = 11025;
			break;

		case AUD_ADC_SAMP_RATE_22_05K:
			samp_rate_value = 22050;
			break;

		case AUD_ADC_SAMP_RATE_24K:
			samp_rate_value = 24000;
			break;

		case AUD_ADC_SAMP_RATE_32K:
			samp_rate_value = 32000;
			break;

		default:
			samp_rate_value = 16000;
			break;
	}

	return bk_aud_uac_set_mic_samp_rate(samp_rate_value);
}

static bk_err_t aud_tras_drv_mic_init(aud_intf_mic_config_t *mic_cfg)
{
	bk_err_t ret = BK_OK;
	bk_err_t err = BK_ERR_AUD_INTF_FAIL;

	LOGD("mic_cfg->frame_size: %d \r\n", mic_cfg->frame_size);

//	aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb = mic_cfg->aud_tras_drv_mic_event_cb;
	aud_tras_drv_info.mic_info.mic_type = mic_cfg->mic_type;
	if (aud_tras_drv_info.mic_info.mic_type == AUD_INTF_MIC_TYPE_BOARD) {
		/* get audio adc config */
		aud_tras_drv_info.mic_info.adc_config = os_malloc(sizeof(aud_adc_config_t));
		if (aud_tras_drv_info.mic_info.adc_config == NULL) {
			LOGE("adc_config os_malloc fail \r\n");
			err = BK_ERR_AUD_INTF_MEMY;
			goto aud_tras_drv_mic_init_exit;
		} else {
			if (mic_cfg->mic_chl == AUD_INTF_MIC_CHL_MIC1) {
				aud_tras_drv_info.mic_info.adc_config->mic_config = AUD_MIC_MIC1_ENABLE;
			} else if (mic_cfg->mic_chl == AUD_INTF_MIC_CHL_DUAL) {
				aud_tras_drv_info.mic_info.adc_config->mic_config = AUD_MIC_ALL_ENABLE;
			} else {
				LOGW("mic chl is error, set default: AUD_MIC_MIC1_ENABLE \r\n");
				aud_tras_drv_info.mic_info.adc_config->mic_config = AUD_MIC_MIC1_ENABLE;
			}
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
		}
		aud_tras_drv_info.mic_info.mic_en = true;
	} else {
		/* set audio uac config */
		if (aud_tras_drv_info.mic_info.uac_config == NULL) {
			aud_tras_drv_info.mic_info.uac_config = os_malloc(sizeof(aud_adc_config_t));
			if (aud_tras_drv_info.mic_info.uac_config == NULL) {
				LOGE("uac_config os_malloc fail \r\n");
				err = BK_ERR_AUD_INTF_MEMY;
				goto aud_tras_drv_mic_init_exit;
			} else {
				aud_tras_drv_info.mic_info.uac_config->mic_samp_rate = AUD_ADC_SAMP_RATE_MAX;
				aud_tras_drv_info.mic_info.uac_config->spk_samp_rate = AUD_DAC_SAMP_RATE_MAX;
				aud_tras_drv_info.mic_info.uac_config->spk_gain = 0;
			}
			aud_tras_drv_info.mic_info.uac_config->mic_samp_rate = mic_cfg->samp_rate;
		}
	}

	aud_tras_drv_info.mic_info.mic_chl = mic_cfg->mic_chl;
	aud_tras_drv_info.mic_info.frame_size = mic_cfg->frame_size;

	/* config audio adc or uac and dma */
	LOGD("mic_ring_buff size: %d \r\n", aud_tras_drv_info.mic_info.frame_size * 2 + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL);
	aud_tras_drv_info.mic_info.mic_ring_buff = (int32_t *)os_malloc(aud_tras_drv_info.mic_info.frame_size * 2 + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL);
	if (aud_tras_drv_info.mic_info.mic_ring_buff == NULL) {
		LOGE("malloc mic ring buffer fail \r\n");
		err = BK_ERR_AUD_INTF_MEMY;
		goto aud_tras_drv_mic_init_exit;
	}

	if (aud_tras_drv_info.mic_info.mic_type == AUD_INTF_MIC_TYPE_BOARD) {
		/* audio adc config */
		ret = aud_tras_adc_config(aud_tras_drv_info.mic_info.adc_config);
		if (ret != BK_OK) {
			LOGE("audio adc init fail \r\n");
			err = BK_ERR_AUD_INTF_ADC;
			goto aud_tras_drv_mic_init_exit;
		}
		LOGI("step1: init audio and config ADC complete \r\n");

		/* init dma driver */
		ret = bk_dma_driver_init();
		if (ret != BK_OK) {
			LOGE("dma driver init failed\r\n");
			err = BK_ERR_AUD_INTF_DMA;
			goto aud_tras_drv_mic_init_exit;
		}

		/* allocate free DMA channel */
		aud_tras_drv_info.mic_info.mic_dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
		if ((aud_tras_drv_info.mic_info.mic_dma_id < DMA_ID_0) || (aud_tras_drv_info.mic_info.mic_dma_id >= DMA_ID_MAX)) {
			LOGE("malloc adc dma fail \r\n");
			err = BK_ERR_AUD_INTF_DMA;
			goto aud_tras_drv_mic_init_exit;
		}

		/* config audio adc dma to carry mic data to "mic_ring_buff" */
		ret = aud_tras_adc_dma_config(aud_tras_drv_info.mic_info.mic_dma_id, aud_tras_drv_info.mic_info.mic_ring_buff, aud_tras_drv_info.mic_info.frame_size * 2, aud_tras_drv_info.mic_info.frame_size, aud_tras_drv_info.mic_info.mic_chl);
		if (ret != BK_OK) {
			LOGE("config audio adc dma fail \r\n");
			err = ret;
			goto aud_tras_drv_mic_init_exit;
		}

		/* init mic_ring_buff */
		ring_buffer_init(&(aud_tras_drv_info.mic_info.mic_rb), (uint8_t*)aud_tras_drv_info.mic_info.mic_ring_buff, aud_tras_drv_info.mic_info.frame_size * 2 + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL, aud_tras_drv_info.mic_info.mic_dma_id, RB_DMA_TYPE_WRITE);
		LOGI("step2: init dma:%d, and mic ring buff:%p, size:%d complete \r\n", aud_tras_drv_info.mic_info.mic_dma_id, aud_tras_drv_info.mic_info.mic_ring_buff, aud_tras_drv_info.mic_info.frame_size * 2);
	} else if (aud_tras_drv_info.mic_info.mic_type == AUD_INTF_MIC_TYPE_UAC) {
		/* init mic_ring_buff */
		ring_buffer_init(&(aud_tras_drv_info.mic_info.mic_rb), (uint8_t*)aud_tras_drv_info.mic_info.mic_ring_buff, aud_tras_drv_info.mic_info.frame_size*2 + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL, DMA_ID_MAX, RB_DMA_TYPE_NULL);

		/* register uac connect and disconnect callback */
		bk_aud_uac_register_disconnect_cb(aud_tras_uac_disconnect_cb);
		bk_aud_uac_register_connect_cb(aud_tras_uac_connect_cb);

		/* close mic */
		aud_tras_drv_info.mic_info.mic_en = false;
		bk_aud_uac_register_mic_callback((void *)aud_tras_drv_mic_uac_mic_cb);

		LOGI("init uac driver \r\n");

		ret = bk_aud_uac_driver_init();
		if (ret != BK_OK) {
			LOGE("init uac driver fail \r\n");
			err = BK_ERR_AUD_INTF_UAC_DRV;
			goto aud_tras_drv_mic_init_exit;
		}
		aud_tras_drv_info.uac_status = AUD_INTF_UAC_CONNECTED;

		ret = uac_set_mic_samp_rate(mic_cfg->samp_rate);
		if (ret != BK_OK) {
			err = BK_ERR_AUD_INTF_UAC_MIC;
			goto aud_tras_drv_mic_init_exit;
		}
		LOGI("step2: init mic ring buff:%p, size:%d complete \r\n", aud_tras_drv_info.mic_info.mic_ring_buff, aud_tras_drv_info.voc_info.mic_samp_rate_points*2 + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL);
	} else {
		err = BK_ERR_AUD_INTF_UAC_MIC;
		goto aud_tras_drv_mic_init_exit;
	}

	/* init mic temp buffer */
	aud_tras_drv_info.mic_info.temp_mic_addr = (int32_t *)os_malloc(aud_tras_drv_info.mic_info.frame_size);
	if (aud_tras_drv_info.mic_info.temp_mic_addr == NULL) {
		LOGE("malloc temp mic ring buffer fail \r\n");
		err = BK_ERR_AUD_INTF_MEMY;
		goto aud_tras_drv_mic_init_exit;
	}
	LOGI("step3: init temp mic ring buff complete \r\n");

	/* change status: AUD_TRAS_DRV_MIC_NULL --> AUD_TRAS_DRV_MIC_IDLE */
	aud_tras_drv_info.mic_info.status = AUD_TRAS_DRV_MIC_STA_IDLE;

	LOGI("step4: init mic complete \r\n");
/*
	if (aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb)
		aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb(EVENT_AUD_TRAS_MIC_INIT, BK_ERR_AUD_INTF_OK);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);

	return err = BK_ERR_AUD_INTF_OK;

aud_tras_drv_mic_init_exit:
	aud_tras_drv_info.mic_info.mic_en = false;
	aud_tras_drv_info.mic_info.status = AUD_TRAS_DRV_MIC_STA_NULL;
	aud_tras_drv_info.mic_info.mic_chl = AUD_INTF_MIC_CHL_MIC1;

	if (aud_tras_drv_info.mic_info.adc_config)
		os_free(aud_tras_drv_info.mic_info.adc_config);
	aud_tras_drv_info.mic_info.adc_config = NULL;

	if (aud_tras_drv_info.mic_info.uac_config)
		os_free(aud_tras_drv_info.mic_info.uac_config);
	aud_tras_drv_info.mic_info.uac_config = NULL;

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

	aud_tras_drv_info.mic_info.mic_type = AUD_INTF_MIC_TYPE_MAX;
/*
	if (aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb)
		aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb(EVENT_AUD_TRAS_MIC_INIT, err);
	aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb = NULL;
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, err);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, err, APP_MODULE);

	return err;
}

static bk_err_t aud_tras_drv_mic_deinit(void)
{
	if (aud_tras_drv_info.mic_info.mic_type == AUD_INTF_MIC_TYPE_BOARD) {
		/* disable audio adc */
		bk_aud_stop_adc();
		bk_dma_stop(aud_tras_drv_info.mic_info.mic_dma_id);
		bk_dma_deinit(aud_tras_drv_info.mic_info.mic_dma_id);
		bk_dma_free(DMA_DEV_AUDIO, aud_tras_drv_info.mic_info.mic_dma_id);
		bk_aud_adc_deinit();
		if (aud_tras_drv_info.spk_info.status == AUD_TRAS_DRV_SPK_STA_NULL)
			bk_aud_driver_deinit();
		os_free(aud_tras_drv_info.mic_info.adc_config);
		aud_tras_drv_info.mic_info.adc_config = NULL;
	} else {
		aud_tras_drv_info.uac_status = AUD_INTF_UAC_NORMAL_DISCONNECTED;
		bk_aud_uac_stop_mic();
		bk_aud_uac_register_mic_callback(NULL);
		bk_aud_uac_driver_deinit();
	}

	aud_tras_drv_info.mic_info.mic_en = false;
	aud_tras_drv_info.mic_info.mic_chl = AUD_INTF_MIC_CHL_MIC1;
	aud_tras_drv_info.mic_info.frame_size = 0;
	aud_tras_drv_info.mic_info.mic_dma_id = DMA_ID_MAX;

	if (aud_tras_drv_info.mic_info.uac_config)
		os_free(aud_tras_drv_info.mic_info.uac_config);
	aud_tras_drv_info.mic_info.uac_config = NULL;

	ring_buffer_clear(&(aud_tras_drv_info.mic_info.mic_rb));
	os_free(aud_tras_drv_info.mic_info.mic_ring_buff);
	aud_tras_drv_info.mic_info.mic_ring_buff = NULL;

	os_free(aud_tras_drv_info.mic_info.temp_mic_addr);
	aud_tras_drv_info.mic_info.temp_mic_addr = NULL;

	aud_tras_drv_info.mic_info.status = AUD_TRAS_DRV_MIC_STA_NULL;

	return BK_ERR_AUD_INTF_OK;
}

static bk_err_t aud_tras_drv_mic_start(void)
{
	bk_err_t ret = BK_OK;
	bk_err_t err = BK_ERR_AUD_INTF_FAIL;
	if ((aud_tras_drv_info.mic_info.status != AUD_TRAS_DRV_MIC_STA_IDLE) && (aud_tras_drv_info.mic_info.status != AUD_TRAS_DRV_MIC_STA_PAUSE)) {
		err = BK_ERR_AUD_INTF_STA;
		goto mic_start_err;
	}

	if (aud_tras_drv_info.mic_info.mic_type == AUD_INTF_MIC_TYPE_BOARD) {
		ret = bk_dma_start(aud_tras_drv_info.mic_info.mic_dma_id);
		if (ret != BK_OK) {
			LOGE("start mic dma fail \r\n");
			err = BK_ERR_AUD_INTF_DMA;
			goto mic_start_err;
		}

		/* enable adc */
		if (aud_tras_drv_info.mic_info.status == AUD_TRAS_DRV_MIC_STA_IDLE)
			bk_aud_start_adc();
	} else {
		LOGI("start uac mic \r\n");
		ret = bk_aud_uac_start_mic();
		if (ret != BK_OK) {
			LOGE("start uac mic fail \r\n");
			err = BK_ERR_AUD_INTF_UAC_MIC;
			goto mic_start_err;
		}
	}

	aud_tras_drv_info.mic_info.status = AUD_TRAS_DRV_MIC_STA_START;
/*
	if (aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb)
		aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb(EVENT_AUD_TRAS_MIC_START, BK_ERR_AUD_INTF_OK);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);
	return BK_ERR_AUD_INTF_OK;

mic_start_err:
/*
	if (aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb)
		aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb(EVENT_AUD_TRAS_MIC_START, err);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, err);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, err, APP_MODULE);

	return err;
}

/* not stop dma, only disable adc */
static bk_err_t aud_tras_drv_mic_pause(void)
{
	bk_err_t ret = BK_OK;
	bk_err_t err = BK_ERR_AUD_INTF_FAIL;
	if (aud_tras_drv_info.mic_info.status != AUD_TRAS_DRV_MIC_STA_START) {
		err = BK_ERR_AUD_INTF_STA;
		goto mic_pause_err;
	}

	if (aud_tras_drv_info.mic_info.mic_type == AUD_INTF_MIC_TYPE_BOARD) {
		/* stop adc dma */
		//bk_aud_stop_adc();
		bk_dma_stop(aud_tras_drv_info.mic_info.mic_dma_id);
	} else {
		LOGI("stop uac mic \r\n");
		ret = bk_aud_uac_stop_mic();
		if (ret != BK_OK) {
			LOGE("stop uac mic fail \r\n");
			err = BK_ERR_AUD_INTF_UAC_MIC;
			goto mic_pause_err;
		}
	}

	aud_tras_drv_info.mic_info.status = AUD_TRAS_DRV_MIC_STA_PAUSE;
/*
	if (aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb)
		aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb(EVENT_AUD_TRAS_MIC_PAUSE, BK_ERR_AUD_INTF_OK);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);
	return BK_ERR_AUD_INTF_OK;

mic_pause_err:
/*
	if (aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb)
		aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb(EVENT_AUD_TRAS_MIC_PAUSE, err);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, err);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, err, APP_MODULE);

	return err;
}

static bk_err_t aud_tras_drv_mic_stop(void)
{
	bk_err_t ret = BK_OK;
	bk_err_t err = BK_ERR_AUD_INTF_FAIL;
	if ((aud_tras_drv_info.mic_info.status != AUD_TRAS_DRV_MIC_STA_START) && (aud_tras_drv_info.mic_info.status != AUD_TRAS_DRV_MIC_STA_PAUSE)) {
		err = BK_ERR_AUD_INTF_STA;
		goto mic_stop_err;
	}

	if (aud_tras_drv_info.mic_info.mic_type == AUD_INTF_MIC_TYPE_BOARD) {
		/* disable adc */
		bk_aud_stop_adc();
		bk_dma_stop(aud_tras_drv_info.mic_info.mic_dma_id);
		ring_buffer_clear(&(aud_tras_drv_info.mic_info.mic_rb));
	} else {
		LOGI("stop uac mic \r\n");
		ret = bk_aud_uac_stop_mic();
		if (ret != BK_OK) {
			LOGE("stop uac mic fail \r\n");
			err = BK_ERR_AUD_INTF_UAC_MIC;
			goto mic_stop_err;
		}
	}

	aud_tras_drv_info.mic_info.status = AUD_TRAS_DRV_MIC_STA_IDLE;
/*
	if (aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb)
		aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb(EVENT_AUD_TRAS_MIC_STOP, BK_ERR_AUD_INTF_OK);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);
	return BK_ERR_AUD_INTF_OK;

mic_stop_err:
/*
	if (aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb)
		aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb(EVENT_AUD_TRAS_MIC_STOP, err);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, err);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, err, APP_MODULE);

	return err;
}

/*
static bk_err_t aud_tras_drv_mic_get_gain(uint8_t *value)
{
	*value = aud_tras_drv_info.mic_info.adc_config->adc_set_gain;

	return BK_OK;
}
*/

static bk_err_t aud_tras_drv_mic_set_chl(aud_intf_mic_chl_t mic_chl)
{
	bk_err_t ret = BK_ERR_AUD_INTF_OK;
	switch (mic_chl) {
		case AUD_INTF_MIC_CHL_MIC1:
			ret = bk_aud_set_mic_chl(AUD_MIC_MIC1_ENABLE);
			if (ret == BK_OK)
				ret = bk_dma_set_src_data_width(aud_tras_drv_info.mic_info.mic_dma_id, DMA_DATA_WIDTH_16BITS);
			break;

		case AUD_INTF_MIC_CHL_DUAL:
			ret = bk_aud_set_mic_chl(AUD_MIC_ALL_ENABLE);
			if (ret == BK_OK)
				ret = bk_dma_set_src_data_width(aud_tras_drv_info.mic_info.mic_dma_id, DMA_DATA_WIDTH_32BITS);
			break;

		default:
			break;
	}

/*
	if (aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb)
		aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb(EVENT_AUD_TRAS_MIC_SET_CHL, ret);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, ret);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, ret, APP_MODULE);

	return ret;
}

static bk_err_t aud_tras_drv_mic_set_samp_rate(aud_adc_samp_rate_t samp_rate)
{
	bk_err_t ret = BK_ERR_AUD_INTF_OK;
	ret = bk_aud_set_adc_samp_rate(samp_rate);
/*
	if (aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb)
		aud_tras_drv_info.mic_info.aud_tras_drv_mic_event_cb(EVENT_AUD_TRAS_MIC_SET_SAMP_RATE, ret);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, ret);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, ret, APP_MODULE);

	return ret;
}


static bk_err_t aud_tras_drv_voc_deinit(void)
{
//	if (aud_tras_drv_info.voc_info.status == AUD_TRAS_DRV_VOC_STA_NULL)
//		return BK_ERR_AUD_INTF_OK;

#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
		bk_timer_stop(TIMER_ID4);
#endif

	/* disable mic */
	if (aud_tras_drv_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_BOARD) {
		bk_aud_stop_adc();
		bk_aud_adc_deinit();
		bk_dma_stop(aud_tras_drv_info.voc_info.adc_dma_id);
		bk_dma_deinit(aud_tras_drv_info.voc_info.adc_dma_id);
		bk_dma_free(DMA_DEV_AUDIO, aud_tras_drv_info.voc_info.adc_dma_id);
		if (aud_tras_drv_info.voc_info.adc_config) {
			os_free(aud_tras_drv_info.voc_info.adc_config);
			aud_tras_drv_info.voc_info.adc_config = NULL;
		}
	} else {
		bk_aud_uac_stop_mic();
		bk_aud_uac_unregister_mic_callback();
	}

	/* disable spk */
	if (aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_BOARD) {
		bk_aud_stop_dac();
		aud_tras_dac_pa_ctrl(false);
		bk_aud_dac_deinit();
		if (aud_tras_drv_info.voc_info.dac_config) {
			os_free(aud_tras_drv_info.voc_info.dac_config);
			aud_tras_drv_info.voc_info.dac_config = NULL;
		}
		/* stop dma */
		bk_dma_stop(aud_tras_drv_info.voc_info.dac_dma_id);
		bk_dma_deinit(aud_tras_drv_info.voc_info.dac_dma_id);
		bk_dma_free(DMA_DEV_AUDIO, aud_tras_drv_info.voc_info.dac_dma_id);
	} else {
		bk_aud_uac_stop_spk();
		bk_aud_uac_unregister_spk_callback();
		bk_aud_uac_register_spk_buff_ptr(NULL, 0);
	}

	if (aud_tras_drv_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_BOARD || aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_BOARD)
		bk_aud_driver_deinit();

	if (aud_tras_drv_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_UAC || aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_UAC)
		bk_aud_uac_driver_deinit();

	/* disable AEC */
	aud_tras_drv_aec_decfg();

	if (aud_tras_drv_info.voc_info.uac_config) {
		os_free(aud_tras_drv_info.voc_info.uac_config);
		aud_tras_drv_info.voc_info.uac_config = NULL;
	}

	/* free audio ring buffer */
	//mic deconfig
	ring_buffer_clear(&(aud_tras_drv_info.voc_info.mic_rb));
	if (aud_tras_drv_info.voc_info.mic_ring_buff) {
		os_free(aud_tras_drv_info.voc_info.mic_ring_buff);
		aud_tras_drv_info.voc_info.mic_ring_buff = NULL;
	}
	aud_tras_drv_info.voc_info.mic_samp_rate_points = 0;
	aud_tras_drv_info.voc_info.mic_frame_number = 0;
	aud_tras_drv_info.voc_info.adc_dma_id = DMA_ID_MAX;

	//speaker deconfig
	ring_buffer_clear(&(aud_tras_drv_info.voc_info.speaker_rb));
	if (aud_tras_drv_info.voc_info.speaker_ring_buff) {
		os_free(aud_tras_drv_info.voc_info.speaker_ring_buff);
		aud_tras_drv_info.voc_info.speaker_ring_buff = NULL;
	}
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

	/* uac spk buffer */
	if (aud_tras_drv_info.voc_info.uac_spk_buff) {
		os_free(aud_tras_drv_info.voc_info.uac_spk_buff);
		aud_tras_drv_info.voc_info.uac_spk_buff = NULL;
		aud_tras_drv_info.voc_info.uac_spk_buff_size = 0;
	}

	/* encoder_temp and decoder_temp deconfig*/
	if (aud_tras_drv_info.voc_info.encoder_temp.law_data) {
		os_free(aud_tras_drv_info.voc_info.encoder_temp.law_data);
		aud_tras_drv_info.voc_info.encoder_temp.law_data = NULL;
	}
	if (aud_tras_drv_info.voc_info.decoder_temp.law_data) {
		os_free(aud_tras_drv_info.voc_info.decoder_temp.law_data);
		aud_tras_drv_info.voc_info.decoder_temp.law_data = NULL;
	}
	if (aud_tras_drv_info.voc_info.encoder_temp.pcm_data) {
		os_free(aud_tras_drv_info.voc_info.encoder_temp.pcm_data);
		aud_tras_drv_info.voc_info.encoder_temp.pcm_data = NULL;
	}
	if (aud_tras_drv_info.voc_info.decoder_temp.pcm_data) {
		os_free(aud_tras_drv_info.voc_info.decoder_temp.pcm_data);
		aud_tras_drv_info.voc_info.decoder_temp.pcm_data = NULL;
	}
	aud_tras_drv_info.voc_info.data_type = AUD_INTF_VOC_DATA_TYPE_PCM;

	/* change status:
				AUD_TRAS_DRV_VOC_IDLE --> AUD_TRAS_DRV_VOC_NULL
				AUD_TRAS_DRV_VOC_START --> AUD_TRAS_DRV_VOC_NULL
				AUD_TRAS_DRV_VOC_STOP --> AUD_TRAS_DRV_VOC_NULL
	*/
	aud_tras_drv_info.voc_info.status = AUD_TRAS_DRV_VOC_STA_NULL;

#if CONFIG_AUD_TRAS_AEC_DUMP_MODE_UART
	bk_uart_deinit(CONFIG_AUD_TRAS_AEC_DUMP_UART_ID);
#endif

	LOGI("voc deinit complete \r\n");
	return BK_ERR_AUD_INTF_OK;
}

static void aud_tras_drv_voc_uac_mic_cb(uint8_t *buff, uint32_t count)
{
	bk_err_t ret = BK_OK;
	uint32_t write_size = 0;
	uint32_t data = 0;

	if (aud_tras_drv_info.voc_info.status == AUD_TRAS_DRV_VOC_STA_START && count > 0) {
		if (ring_buffer_get_free_size(&aud_tras_drv_info.voc_info.mic_rb) >= count) {
			for (write_size = 0; write_size < count/4; write_size++) {
				data = *((uint32_t *)buff);
				ring_buffer_write(&aud_tras_drv_info.voc_info.mic_rb, (uint8_t *)&data, 4);
			}
		}
	}

	if (uac_mic_read_flag && (ring_buffer_get_fill_size(&aud_tras_drv_info.voc_info.mic_rb) < aud_tras_drv_info.voc_info.mic_samp_rate_points * 2))
		uac_mic_read_flag = false;

	if ((ring_buffer_get_fill_size(&aud_tras_drv_info.voc_info.mic_rb) > aud_tras_drv_info.voc_info.mic_samp_rate_points * 2) && (uac_mic_read_flag == false)) {
		uac_mic_read_flag = true;
		/* send msg to AEC or ENCODER to process mic data */
		if (aud_tras_drv_info.voc_info.aec_enable)
			ret = aud_tras_drv_send_msg(AUD_TRAS_DRV_AEC, NULL);
		else
			ret = aud_tras_drv_send_msg(AUD_TRAS_DRV_ENCODER, NULL);
		if (ret != kNoErr) {
			LOGE("send msg: AUD_TRAS_DRV_ENCODER fail \r\n");
			uac_mic_read_flag = false;
		}
	}
}

static void aud_tras_drv_voc_uac_spk_cb(void)
{
	bk_err_t ret = BK_OK;
	uint32_t size = 0;

	if (aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_UAC) {
		/* get G711A data from decoder_ring_buff */
		if (ring_buffer_get_fill_size(&aud_tras_drv_info.voc_info.speaker_rb) >= aud_tras_drv_info.voc_info.uac_spk_buff_size) {
			size = ring_buffer_read(&aud_tras_drv_info.voc_info.speaker_rb, aud_tras_drv_info.voc_info.uac_spk_buff, aud_tras_drv_info.voc_info.uac_spk_buff_size);
			if (size != aud_tras_drv_info.voc_info.uac_spk_buff_size) {
				LOGE("read speaker_rb uac spk data fail \r\n");
				os_memset(aud_tras_drv_info.voc_info.uac_spk_buff, 0x00, aud_tras_drv_info.voc_info.uac_spk_buff_size);
			}
		} else {
			LOGW("write 0x00 uac spk data fail \r\n");
			os_memset(aud_tras_drv_info.voc_info.uac_spk_buff, 0x00, aud_tras_drv_info.voc_info.uac_spk_buff_size);
		}
	}

	/* send msg to decoder to decoding recevied data */
	ret = aud_tras_drv_send_msg(AUD_TRAS_DRV_DECODER, NULL);
	if (ret != kNoErr) {
		LOGE("send msg: AUD_TRAS_DRV_DECODER fails \r\n");
	}
}


/* audio transfer driver voice mode init */
static bk_err_t aud_tras_drv_voc_init(aud_intf_voc_config_t* voc_cfg)
{
	bk_err_t ret = BK_OK;
	bk_err_t err = BK_ERR_AUD_INTF_FAIL;

	/* callback config */
//	aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb = voc_cfg->aud_tras_drv_voc_event_cb;

	/* get aec config */
	aud_tras_drv_info.voc_info.aec_enable = voc_cfg->aec_enable;
	if (aud_tras_drv_info.voc_info.aec_enable) {
		aud_tras_drv_info.voc_info.aec_info = os_malloc(sizeof(aec_info_t));
		if (aud_tras_drv_info.voc_info.aec_info == NULL) {
			LOGE("aec_info os_malloc fail \r\n");
			err = BK_ERR_AUD_INTF_MEMY;
			goto aud_tras_drv_voc_init_exit;
		} else {
			aud_tras_drv_info.voc_info.aec_info->samp_rate = voc_cfg->samp_rate;
			aud_tras_drv_info.voc_info.aec_info->aec_config = os_malloc(sizeof(aec_config_t));
			if (aud_tras_drv_info.voc_info.aec_info->aec_config == NULL) {
				LOGE("aec_config_t os_malloc fail \r\n");
				err = BK_ERR_AUD_INTF_MEMY;
				goto aud_tras_drv_voc_init_exit;
			} else {
				aud_tras_drv_info.voc_info.aec_info->aec_config->init_flags = voc_cfg->aec_setup->init_flags;
				aud_tras_drv_info.voc_info.aec_info->aec_config->mic_delay = voc_cfg->aec_setup->mic_delay;
				aud_tras_drv_info.voc_info.aec_info->aec_config->ec_depth = voc_cfg->aec_setup->ec_depth;
				aud_tras_drv_info.voc_info.aec_info->aec_config->ref_scale = voc_cfg->aec_setup->ref_scale;
				aud_tras_drv_info.voc_info.aec_info->aec_config->voice_vol = voc_cfg->aec_setup->voice_vol;
				aud_tras_drv_info.voc_info.aec_info->aec_config->TxRxThr = voc_cfg->aec_setup->TxRxThr;
				aud_tras_drv_info.voc_info.aec_info->aec_config->TxRxFlr = voc_cfg->aec_setup->TxRxFlr;
				aud_tras_drv_info.voc_info.aec_info->aec_config->ns_level = voc_cfg->aec_setup->ns_level;
				aud_tras_drv_info.voc_info.aec_info->aec_config->ns_para = voc_cfg->aec_setup->ns_para;
				aud_tras_drv_info.voc_info.aec_info->aec_config->drc = voc_cfg->aec_setup->drc;
			}
		}
	} else {
		aud_tras_drv_info.voc_info.aec_info = NULL;
	}

	aud_tras_drv_info.voc_info.data_type = voc_cfg->data_type;
	LOGD("aud_tras_drv_info.voc_info.data_type:%d \r\n", aud_tras_drv_info.voc_info.data_type);
	aud_tras_drv_info.voc_info.mic_en = voc_cfg->mic_en;
	aud_tras_drv_info.voc_info.spk_en = voc_cfg->spk_en;
	aud_tras_drv_info.voc_info.mic_type = voc_cfg->mic_type;
	aud_tras_drv_info.voc_info.spk_type = voc_cfg->spk_type;

	if (aud_tras_drv_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_BOARD) {
		/* get audio adc config */
		aud_tras_drv_info.voc_info.adc_config = os_malloc(sizeof(aud_adc_config_t));
		if (aud_tras_drv_info.voc_info.adc_config == NULL) {
			LOGE("adc_config os_malloc fail \r\n");
			err = BK_ERR_AUD_INTF_MEMY;
			goto aud_tras_drv_voc_init_exit;
		} else {
			aud_tras_drv_info.voc_info.adc_config->mic_config = AUD_MIC_MIC1_ENABLE;
			if (voc_cfg->samp_rate == AUD_INTF_VOC_SAMP_RATE_16K)
				aud_tras_drv_info.voc_info.adc_config->samp_rate = AUD_ADC_SAMP_RATE_16K;
			else
				aud_tras_drv_info.voc_info.adc_config->samp_rate = AUD_ADC_SAMP_RATE_8K;
			//aud_tras_drv_info.voc_info.adc_config->samp_rate = voc_cfg->samp_rate;
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
		}
	} else {
		/* set audio uac config */
		if (aud_tras_drv_info.voc_info.uac_config == NULL) {
			aud_tras_drv_info.voc_info.uac_config = os_malloc(sizeof(aud_uac_config_t));
			if (aud_tras_drv_info.voc_info.uac_config == NULL) {
				LOGE("uac_config os_malloc fail \r\n");
				err = BK_ERR_AUD_INTF_MEMY;
				goto aud_tras_drv_voc_init_exit;
			} else {
				aud_tras_drv_info.voc_info.uac_config->mic_samp_rate = AUD_ADC_SAMP_RATE_MAX;
				aud_tras_drv_info.voc_info.uac_config->spk_samp_rate = AUD_DAC_SAMP_RATE_MAX;
				aud_tras_drv_info.voc_info.uac_config->spk_gain = 0;
			}
			aud_tras_drv_info.voc_info.uac_config->spk_samp_rate = voc_cfg->samp_rate;
			aud_tras_drv_info.voc_info.uac_config->spk_gain = voc_cfg->aud_setup.dac_gain;
		}
	}

	if (aud_tras_drv_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_BOARD) {
		/* get audio dac config */
		aud_tras_drv_info.voc_info.dac_config = os_malloc(sizeof(aud_dac_config_t));
		if (aud_tras_drv_info.voc_info.adc_config == NULL) {
			LOGE("dac_config os_malloc fail \r\n");
			err = BK_ERR_AUD_INTF_MEMY;
			goto aud_tras_drv_voc_init_exit;
		} else {
			aud_tras_drv_info.voc_info.dac_config->dac_enable = AUD_DAC_DISABLE;
			if (voc_cfg->samp_rate == AUD_INTF_VOC_SAMP_RATE_16K)
				aud_tras_drv_info.voc_info.dac_config->samp_rate = AUD_DAC_SAMP_RATE_16K;
			else
				aud_tras_drv_info.voc_info.dac_config->samp_rate = AUD_DAC_SAMP_RATE_8K;
			//aud_tras_drv_info.voc_info.dac_config->samp_rate = voc_cfg->samp_rate;
			aud_tras_drv_info.voc_info.dac_config->dac_chl = AUD_DAC_CHL_L_ENABLE;
			aud_tras_drv_info.voc_info.dac_config->work_mode = voc_cfg->aud_setup.spk_mode;
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
		}
	}

	/* get ring buffer config */
	//aud_tras_drv_info.voc_info.mode = setup->aud_trs_mode;
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

	aud_tras_drv_info.voc_info.mic_frame_number = voc_cfg->aud_setup.mic_frame_number;
	aud_tras_drv_info.voc_info.speaker_frame_number = voc_cfg->aud_setup.speaker_frame_number;

	/* get tx and rx context config */
	aud_tras_drv_info.voc_info.tx_info = voc_cfg->tx_info;
	aud_tras_drv_info.voc_info.rx_info = voc_cfg->rx_info;

#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
	aud_tras_drv_info.voc_info.lost_count.complete_size = 0;
	aud_tras_drv_info.voc_info.lost_count.lost_size = 0;
#endif

	/*  -------------------------step0: init audio and config ADC and DAC -------------------------------- */
	/* config mailbox according audio transfer work mode */

	/*  -------------------------step2: init AEC and malloc two ring buffers -------------------------------- */
	/* init aec and config aec according AEC_enable*/
	if (aud_tras_drv_info.voc_info.aec_enable) {
		ret = aud_tras_drv_aec_cfg();
		if (ret != BK_OK) {
			err = BK_ERR_AUD_INTF_AEC;
			goto aud_tras_drv_voc_init_exit;
		}
		LOGI("aec samp_rate_points: %d \r\n", aud_tras_drv_info.voc_info.aec_info->samp_rate_points);
		ret = aud_tras_drv_aec_buff_cfg(aud_tras_drv_info.voc_info.aec_info);
		if (ret != BK_OK) {
			err = ret;
			goto aud_tras_drv_voc_init_exit;
		}
		LOGI("step2: init AEC and malloc two ring buffers complete \r\n");
	}

	/* -------------------step3: init and config DMA to carry mic and ref data ----------------------------- */
	aud_tras_drv_info.voc_info.mic_ring_buff = (int32_t *)os_malloc(aud_tras_drv_info.voc_info.mic_samp_rate_points*2*aud_tras_drv_info.voc_info.mic_frame_number + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL);
	if (aud_tras_drv_info.voc_info.mic_ring_buff == NULL) {
		LOGE("malloc mic ring buffer fail \r\n");
		err = BK_ERR_AUD_INTF_MEMY;
		goto aud_tras_drv_voc_init_exit;
	}

	if (aud_tras_drv_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_BOARD) {
		/* init dma driver */
		ret = bk_dma_driver_init();
		if (ret != BK_OK) {
			LOGE("dma driver init failed\r\n");
			err = BK_ERR_AUD_INTF_DMA;
			goto aud_tras_drv_voc_init_exit;
		}

		/* allocate free DMA channel */
		aud_tras_drv_info.voc_info.adc_dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
		if ((aud_tras_drv_info.voc_info.adc_dma_id < DMA_ID_0) || (aud_tras_drv_info.voc_info.adc_dma_id >= DMA_ID_MAX)) {
			LOGE("malloc adc dma fail \r\n");
			err = BK_ERR_AUD_INTF_DMA;
			goto aud_tras_drv_voc_init_exit;
		}

		/* config audio adc dma to carry mic data to "mic_ring_buff" */
		ret = aud_tras_adc_dma_config(aud_tras_drv_info.voc_info.adc_dma_id, aud_tras_drv_info.voc_info.mic_ring_buff, (aud_tras_drv_info.voc_info.mic_samp_rate_points*2)*aud_tras_drv_info.voc_info.mic_frame_number + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL, aud_tras_drv_info.voc_info.mic_samp_rate_points*2, AUD_INTF_MIC_CHL_MIC1);
		if (ret != BK_OK) {
			LOGE("config audio adc dma fail \r\n");
			err = ret;
			goto aud_tras_drv_voc_init_exit;
		}

		ring_buffer_init(&(aud_tras_drv_info.voc_info.mic_rb), (uint8_t*)aud_tras_drv_info.voc_info.mic_ring_buff, aud_tras_drv_info.voc_info.mic_samp_rate_points*2*aud_tras_drv_info.voc_info.mic_frame_number + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL, aud_tras_drv_info.voc_info.adc_dma_id, RB_DMA_TYPE_WRITE);

		LOGI("step3: init and config mic DMA complete, adc_dma_id:%d, mic_ring_buff:%p, size:%d, carry_length:%d \r\n", aud_tras_drv_info.voc_info.adc_dma_id, aud_tras_drv_info.voc_info.mic_ring_buff, (aud_tras_drv_info.voc_info.mic_samp_rate_points*2)*aud_tras_drv_info.voc_info.mic_frame_number, aud_tras_drv_info.voc_info.mic_samp_rate_points*2);
	} else if (aud_tras_drv_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_UAC) {
		/* init mic_ring_buff */
		ring_buffer_init(&(aud_tras_drv_info.voc_info.mic_rb), (uint8_t*)aud_tras_drv_info.voc_info.mic_ring_buff, aud_tras_drv_info.voc_info.mic_samp_rate_points*2*aud_tras_drv_info.voc_info.mic_frame_number + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL, DMA_ID_MAX, RB_DMA_TYPE_NULL);

		/* close mic */
		aud_tras_drv_info.voc_info.mic_en = false;
		bk_aud_uac_register_mic_callback((void *)aud_tras_drv_voc_uac_mic_cb);

		/* register uac connect and disconnect callback */
		bk_aud_uac_register_disconnect_cb(aud_tras_uac_disconnect_cb);
		bk_aud_uac_register_connect_cb(aud_tras_uac_connect_cb);

		LOGI("step3: init voc mic ring buff:%p, size:%d complete \r\n", aud_tras_drv_info.voc_info.mic_ring_buff, aud_tras_drv_info.voc_info.mic_samp_rate_points*2*aud_tras_drv_info.voc_info.mic_frame_number + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL);
	} else {
		err = BK_ERR_AUD_INTF_UAC_MIC;
		goto aud_tras_drv_voc_init_exit;
	}

	/*  -------------------step4: init and config DMA to carry dac data ----------------------------- */
	aud_tras_drv_info.voc_info.speaker_ring_buff = (int32_t *)os_malloc(aud_tras_drv_info.voc_info.speaker_samp_rate_points*2*aud_tras_drv_info.voc_info.speaker_frame_number + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL);
	if (aud_tras_drv_info.voc_info.speaker_ring_buff == NULL) {
		LOGE("malloc speaker ring buffer fail \r\n");
		err = BK_ERR_AUD_INTF_MEMY;
		goto aud_tras_drv_voc_init_exit;
	}

	if (aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_BOARD) {
		/* init dma driver */
		ret = bk_dma_driver_init();
		if (ret != BK_OK) {
			LOGE("dma driver init failed\r\n");
			err = BK_ERR_AUD_INTF_DMA;
			goto aud_tras_drv_voc_init_exit;
		}

		/* allocate free DMA channel */
		aud_tras_drv_info.voc_info.dac_dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
		if ((aud_tras_drv_info.voc_info.dac_dma_id < DMA_ID_0) || (aud_tras_drv_info.voc_info.dac_dma_id >= DMA_ID_MAX)) {
			LOGE("malloc dac dma fail \r\n");
			err = BK_ERR_AUD_INTF_DMA;
			goto aud_tras_drv_voc_init_exit;
		}

		/* config audio dac dma to carry dac data to "speaker_ring_buff" */
		ret = aud_tras_dac_dma_config(aud_tras_drv_info.voc_info.dac_dma_id, aud_tras_drv_info.voc_info.speaker_ring_buff, (aud_tras_drv_info.voc_info.speaker_samp_rate_points*2)*aud_tras_drv_info.voc_info.speaker_frame_number + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL, aud_tras_drv_info.voc_info.speaker_samp_rate_points*2, AUD_INTF_SPK_CHL_LEFT);
		if (ret != BK_OK) {
			LOGE("config audio adc dma fail \r\n");
			err = ret;
			goto aud_tras_drv_voc_init_exit;
		}

		ring_buffer_init(&(aud_tras_drv_info.voc_info.speaker_rb), (uint8_t*)aud_tras_drv_info.voc_info.speaker_ring_buff, aud_tras_drv_info.voc_info.speaker_samp_rate_points*2*aud_tras_drv_info.voc_info.speaker_frame_number + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL, aud_tras_drv_info.voc_info.dac_dma_id, RB_DMA_TYPE_READ);

//		LOGI("step4: init and config speaker DMA complete, dac_dma_id:%d, speaker_ring_buff:%p, size:%d, carry_length:%d \r\n", aud_tras_drv_info.voc_info.dac_dma_id, aud_tras_drv_info.voc_info.speaker_ring_buff, (aud_tras_drv_info.voc_info.speaker_samp_rate_points*2)*aud_tras_drv_info.voc_info.speaker_frame_number + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL, aud_tras_drv_info.voc_info.speaker_samp_rate_points*2);
	} else if (aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_UAC) {
		aud_tras_drv_info.voc_info.spk_en = false;
		/* get uac info, and set uac_spk_buff_size */
		switch (voc_cfg->samp_rate) {
			case AUD_INTF_VOC_SAMP_RATE_8K:
				aud_tras_drv_info.voc_info.uac_spk_buff_size = 16;
				break;

			case AUD_INTF_VOC_SAMP_RATE_16K:
				aud_tras_drv_info.voc_info.uac_spk_buff_size = 32;
				break;

			default:
				aud_tras_drv_info.voc_info.uac_spk_buff_size = 32;
				break;
		}
		//aud_tras_drv_info.voc_info.uac_spk_buff_size = 32;
		aud_tras_drv_info.voc_info.uac_spk_buff = (uint8_t *)os_malloc(aud_tras_drv_info.voc_info.uac_spk_buff_size);

		bk_aud_uac_register_spk_buff_ptr(aud_tras_drv_info.voc_info.uac_spk_buff, aud_tras_drv_info.voc_info.uac_spk_buff_size);

		bk_aud_uac_register_spk_callback(aud_tras_drv_voc_uac_spk_cb);

		/* register uac connect and disconnect callback */
		bk_aud_uac_register_disconnect_cb(aud_tras_uac_disconnect_cb);
		bk_aud_uac_register_connect_cb(aud_tras_uac_connect_cb);

		ring_buffer_init(&(aud_tras_drv_info.voc_info.speaker_rb), (uint8_t*)aud_tras_drv_info.voc_info.speaker_ring_buff, aud_tras_drv_info.voc_info.speaker_samp_rate_points*2*aud_tras_drv_info.voc_info.speaker_frame_number + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL, DMA_ID_MAX, RB_DMA_TYPE_NULL);

//		LOGI("step4: init uac speaker_ring_buff:%p, spk_ring_buff_size:%d, uac_spk_buff:%p, uac_spk_buff_size:%d\r\n", aud_tras_drv_info.voc_info.speaker_ring_buff, (aud_tras_drv_info.voc_info.speaker_samp_rate_points*2)*aud_tras_drv_info.voc_info.speaker_frame_number + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL, aud_tras_drv_info.voc_info.uac_spk_buff, aud_tras_drv_info.voc_info.uac_spk_buff_size);
	} else {
		//TODO
	}

	/*  -------------------------step6: init all audio ring buffers -------------------------------- */
	/* init encoder and decoder temp buffer */
	aud_tras_drv_info.voc_info.encoder_temp.pcm_data = (int16_t *)os_malloc(aud_tras_drv_info.voc_info.mic_samp_rate_points * 2);
	if (aud_tras_drv_info.voc_info.encoder_temp.pcm_data == NULL) {
		LOGE("malloc pcm_data of encoder used fail \r\n");
		err = BK_ERR_AUD_INTF_MEMY;
		goto aud_tras_drv_voc_init_exit;
	}

	aud_tras_drv_info.voc_info.decoder_temp.pcm_data = (int16_t *)os_malloc(aud_tras_drv_info.voc_info.speaker_samp_rate_points*2);
	if (aud_tras_drv_info.voc_info.decoder_temp.pcm_data == NULL) {
		LOGE("malloc pcm_data of decoder used fail \r\n");
		err = BK_ERR_AUD_INTF_MEMY;
		goto aud_tras_drv_voc_init_exit;
	}

	switch (aud_tras_drv_info.voc_info.data_type) {
		case AUD_INTF_VOC_DATA_TYPE_G711A:
		case AUD_INTF_VOC_DATA_TYPE_G711U:
			LOGI("malloc law_data temp buffer \r\n");
			aud_tras_drv_info.voc_info.encoder_temp.law_data = (uint8_t *)os_malloc(aud_tras_drv_info.voc_info.mic_samp_rate_points);
			if (aud_tras_drv_info.voc_info.encoder_temp.law_data == NULL) {
				LOGE("malloc law_data of encoder used fail \r\n");
				err = BK_ERR_AUD_INTF_MEMY;
				goto aud_tras_drv_voc_init_exit;
			}

			aud_tras_drv_info.voc_info.decoder_temp.law_data = (unsigned char *)os_malloc(aud_tras_drv_info.voc_info.speaker_samp_rate_points);
			if (aud_tras_drv_info.voc_info.decoder_temp.law_data == NULL) {
				LOGE("malloc law_data of decoder used fail \r\n");
				err = BK_ERR_AUD_INTF_MEMY;
				goto aud_tras_drv_voc_init_exit;
			}
			break;

		case AUD_INTF_VOC_DATA_TYPE_PCM:
			//os_printf("not need to malloc law_data temp buffer \r\n");
			break;

		default:
			break;
	}

	/* audio debug */
	aud_tras_drv_info.voc_info.aud_tras_dump_tx_cb = NULL;
	aud_tras_drv_info.voc_info.aud_tras_dump_rx_cb = NULL;
	aud_tras_drv_info.voc_info.aud_tras_dump_aec_cb = NULL;

	/* change status: AUD_TRAS_DRV_VOC_NULL --> AUD_TRAS_DRV_VOC_IDLE */
	aud_tras_drv_info.voc_info.status = AUD_TRAS_DRV_VOC_STA_IDLE;
//	LOGI("step6: init aud ring buff complete \r\n");

	LOGI("init voc complete \r\n");

	/* audio transfer init callback */
/*
	if (aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb)
		aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb(EVENT_AUD_TRAS_VOC_INIT, BK_ERR_AUD_INTF_OK);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);

	return BK_ERR_AUD_INTF_OK;

aud_tras_drv_voc_init_exit:
	/* audio transfer driver deconfig */
	aud_tras_drv_voc_deinit();
	//CALLBACK_VOC_CB_AND_DEINIT(EVENT_AUD_TRAS_VOC_INIT, err);
	//msg_send_to_app_mailbox(aud_to_media_app_msg, err);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, err, APP_MODULE);

	return err;
}

static bk_err_t aud_tras_drv_voc_start(void)
{
	bk_err_t ret = BK_OK;
	bk_err_t err = BK_ERR_AUD_INTF_FAIL;
	uint32_t size = 0;
	uint8_t *pcm_data = NULL;

	LOGI("%s \r\n", __func__);

	if (aud_tras_drv_info.voc_info.mic_en == AUD_INTF_VOC_MIC_OPEN) {
		if (aud_tras_drv_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_BOARD) {
			/* init audio and config ADC and DAC */
			ret = aud_tras_adc_config(aud_tras_drv_info.voc_info.adc_config);
			if (ret != BK_OK) {
				LOGE("audio adc init fail \r\n");
				err = BK_ERR_AUD_INTF_ADC;
				goto audio_start_transfer_exit;
			}

			/* start DMA */
			//ret = bk_dma_start(aud_tras_drv_info.voc_info.adc_dma_id);
			if (ret != BK_OK) {
				LOGE("start adc dma fail \r\n");
				err = BK_ERR_AUD_INTF_DMA;
				goto audio_start_transfer_exit;
			}

			/* enable adc */
			/* wait receive data and then open adc */
			bk_aud_start_adc();
		} else {
			LOGI("init uac \r\n");
#if 0
			ret = bk_aud_uac_register_mic_callback((void *)aud_tras_drv_voc_uac_mic_cb);
			if (ret != BK_OK) {
				LOGE("malloc mic ring buffer fail \r\n");
				err = BK_ERR_AUD_INTF_UAC_MIC;
				goto audio_start_transfer_exit;
			}
#endif

			ret = bk_aud_uac_driver_init();
			if (ret != BK_OK) {
				LOGE("init uac driver fail \r\n");
				err = BK_ERR_AUD_INTF_UAC_DRV;
				goto audio_start_transfer_exit;
			}
			aud_tras_drv_info.uac_status = AUD_INTF_UAC_CONNECTED;

			if (aud_tras_drv_info.voc_info.mic_samp_rate_points == 160) {
				/* 8K sample rate */
				ret = bk_aud_uac_set_mic_samp_rate(8000);
			} else {
				/* 16K sample rate */
				ret = bk_aud_uac_set_mic_samp_rate(16000);
			}
			if (ret != BK_OK) {
				LOGE("set uac mic samp rate fail \r\n");
				err = BK_ERR_AUD_INTF_UAC_MIC;
				goto audio_start_transfer_exit;
			}

			LOGI("start uac mic \r\n");
			ret = bk_aud_uac_start_mic();
			if (ret != BK_OK) {
				LOGE("start uac mic fail, ret:%d \r\n", ret);
				err = BK_ERR_AUD_INTF_UAC_MIC;
				goto audio_start_transfer_exit;
			}
		}
	}

	if (aud_tras_drv_info.voc_info.spk_en == AUD_INTF_VOC_SPK_OPEN) {
		pcm_data = (uint8_t *)os_malloc(aud_tras_drv_info.voc_info.speaker_samp_rate_points*2);
		if (pcm_data == NULL) {
			LOGE("malloc temp pcm_data fial \r\n");
			err = BK_ERR_AUD_INTF_MEMY;
			goto audio_start_transfer_exit;
		} else {
			os_memset(pcm_data, 0x00, aud_tras_drv_info.voc_info.speaker_samp_rate_points*2);
		}

		if (aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_BOARD) {
			ret = aud_tras_dac_config(aud_tras_drv_info.voc_info.dac_config);
			if (ret != BK_OK) {
				LOGE("audio dac init fail \r\n");
				err = BK_ERR_AUD_INTF_DAC;
				goto audio_start_transfer_exit;
			}

			/* enable dac */
			bk_aud_start_dac();
			aud_tras_dac_pa_ctrl(true);

			ret = bk_dma_start(aud_tras_drv_info.voc_info.dac_dma_id);
			if (ret != BK_OK) {
				LOGE("start dac dma fail \r\n");
				err = BK_ERR_AUD_INTF_DMA;
				goto audio_start_transfer_exit;
			}
		} else {
			LOGI("start uac spk \r\n");
			ret = bk_aud_uac_driver_init();
			if (ret != BK_OK) {
				LOGE("init uac driver fail \r\n");
				err = BK_ERR_AUD_INTF_UAC_DRV;
				goto audio_start_transfer_exit;
			}
			aud_tras_drv_info.uac_status = AUD_INTF_UAC_CONNECTED;

			if (aud_tras_drv_info.voc_info.uac_spk_buff_size == 16) {
				/* 8K sample rate */
				ret = bk_aud_uac_set_spk_samp_rate(8000);
			} else {
				/* 16K sample rate */
				ret = bk_aud_uac_set_spk_samp_rate(16000);
			}
			if (ret != BK_OK) {
				LOGE("set uac spk samp rate fail \r\n");
				err = BK_ERR_AUD_INTF_UAC_SPK;
				goto audio_start_transfer_exit;
			}

			/* set uac speaker volume */
			//bk_aud_uac_set_spk_gain(aud_tras_drv_info.spk_info.uac_config->spk_gain);
			aud_tras_drv_set_spk_gain(aud_tras_drv_info.voc_info.uac_config->spk_gain);

#if CONFIG_AUD_TRAS_UAC_SPK_VOL_CTRL_MODE_STOP_UAC_TRAS
			/* reopen uac mic */
			if ((aud_tras_drv_info.voc_info.mic_en == AUD_INTF_VOC_MIC_OPEN) && (aud_tras_drv_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_UAC)) {
				bk_aud_uac_start_mic();
			}
#endif

			ret = bk_aud_uac_start_spk();
			if (ret != BK_OK) {
				LOGE("start uac spk fail, ret:%d \r\n", ret);
				err = BK_ERR_AUD_INTF_UAC_SPK;
				goto audio_start_transfer_exit;
			}
		}

		/* write two frame data to speaker and ref ring buffer */
		size = ring_buffer_write(&(aud_tras_drv_info.voc_info.speaker_rb), (uint8_t *)pcm_data, aud_tras_drv_info.voc_info.speaker_samp_rate_points*2);
		if (size != aud_tras_drv_info.voc_info.speaker_samp_rate_points*2) {
			LOGE("the data writeten to speaker_ring_buff error, size=%d \r\n", size);
			err = BK_ERR_AUD_INTF_RING_BUFF;
			goto audio_start_transfer_exit;
		}
		size = ring_buffer_write(&(aud_tras_drv_info.voc_info.speaker_rb), (uint8_t *)pcm_data, aud_tras_drv_info.voc_info.speaker_samp_rate_points*2);
		if (size != aud_tras_drv_info.voc_info.speaker_samp_rate_points*2) {
			LOGE("the data writeten to speaker_ring_buff error, size=%d \r\n", size);
			err = BK_ERR_AUD_INTF_RING_BUFF;
			goto audio_start_transfer_exit;
		}

		os_free(pcm_data);
		pcm_data = NULL;
	}
	LOGI("enable audio to start audio transfer complete \r\n");

	/* change status:
				AUD_TRAS_DRV_VOC_STA_IDLE --> AUD_TRAS_DRV_VOC_STA_START
				AUD_TRAS_DRV_VOC_STA_STOP --> AUD_TRAS_DRV_VOC_STA_START
	*/
	aud_tras_drv_info.voc_info.status = AUD_TRAS_DRV_VOC_STA_START;

#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
		//bk_timer_driver_init();
		ret = bk_timer_start(TIMER_ID4, AUD_LOST_DEBUG_INTERVAL, aud_tras_drv_tx_lost_count_dump);
		if (ret != BK_OK)
			LOGE("start audio lost count timer fail \r\n");
		LOGI("start audio lost count timer complete \r\n");
#endif

	return BK_ERR_AUD_INTF_OK;

audio_start_transfer_exit:
	//deinit audio transfer
	if (pcm_data)
		os_free(pcm_data);

	return err;
}

static bk_err_t aud_tras_drv_voc_stop(void)
{
	bk_err_t ret = BK_OK;

	LOGI("%s \r\n", __func__);

#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
			bk_timer_stop(TIMER_ID4);
#endif

	aud_tras_drv_info.uac_status = AUD_INTF_UAC_NORMAL_DISCONNECTED;

	/* stop adc and dac dma */
	if (aud_tras_drv_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_BOARD) {
		ret = bk_dma_stop(aud_tras_drv_info.voc_info.adc_dma_id);
		if (ret != BK_OK) {
			//err = BK_ERR_AUD_INTF_DMA;
			LOGE("start adc dma fail \r\n");
		}
	} else {
		LOGI("stop uac mic \r\n");
		bk_aud_uac_stop_mic();
	}

	if (aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_BOARD) {
		ret = bk_dma_stop(aud_tras_drv_info.voc_info.dac_dma_id);
		if (ret != BK_OK) {
			//err = BK_ERR_AUD_INTF_DMA;
			LOGE("stop dac dma fail \r\n");
		}
	} else {
		LOGI("stop uac spk \r\n");
		ret = bk_aud_uac_stop_spk();
		if (ret != BK_OK) {
			//err = BK_ERR_AUD_INTF_DMA;
			LOGE("stop uac spk fail \r\n");
		}
	}

	if (aud_tras_drv_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_BOARD) {
		/* disable adc */
		bk_aud_stop_adc();
		bk_aud_adc_deinit();
	}

	if (aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_BOARD) {
		/* disable dac */
		bk_aud_stop_dac();
		aud_tras_dac_pa_ctrl(false);
		bk_aud_dac_deinit();
	}

	if (aud_tras_drv_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_BOARD || aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_BOARD)
	/* deinit audio driver */
		bk_aud_driver_deinit();

	if (aud_tras_drv_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_UAC || aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_UAC)
		bk_aud_uac_driver_deinit();

	/* clear adc and dac ring buffer */
	ring_buffer_clear(&(aud_tras_drv_info.voc_info.speaker_rb));
	ring_buffer_clear(&(aud_tras_drv_info.voc_info.mic_rb));

	uac_mic_read_flag = false;

	/* notify cpu0 that audio transfer start */
/*
	if (aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb) {
		aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb(EVENT_AUD_TRAS_VOC_STOP, BK_ERR_AUD_INTF_OK);
	}
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);

	/* change status:
				AUD_TRAS_DRV_VOC_STA_IDLE --> AUD_TRAS_DRV_VOC_STA_STOP
				AUD_TRAS_DRV_VOC_STA_STOP --> AUD_TRAS_DRV_VOC_STA_STOP
	*/
	aud_tras_drv_info.voc_info.status = AUD_TRAS_DRV_VOC_STA_STOP;

	LOGI("stop voice transfer complete \r\n");

	return ret;
}

static bk_err_t aud_tras_drv_voc_ctrl_mic(aud_intf_voc_mic_ctrl_t mic_en)
{
	bk_err_t ret = BK_OK;
	bk_err_t err = BK_ERR_AUD_INTF_FAIL;

	GLOBAL_INT_DECLARATION();

	if (mic_en == AUD_INTF_VOC_MIC_OPEN) {
		if (aud_tras_drv_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_BOARD) {
			LOGI("open mic \r\n");
			/* enable adc */
			bk_aud_start_adc();

			ret = bk_dma_start(aud_tras_drv_info.voc_info.adc_dma_id);
			if (ret != BK_OK) {
				LOGE("start adc dma fail \r\n");
				err = BK_ERR_AUD_INTF_DMA;
				goto voc_ctrl_mic_fail;
			}
		} else {
			LOGI("open uac mic \r\n");
			ret = bk_aud_uac_start_mic();
			if (ret != BK_OK) {
				LOGE("start uac mic fail \r\n");
				err = BK_ERR_AUD_INTF_UAC_MIC;
				goto voc_ctrl_mic_fail;
			}
		}
	} else if (mic_en == AUD_INTF_VOC_MIC_CLOSE) {
		if (aud_tras_drv_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_BOARD) {
			LOGI("close mic \r\n");
			bk_aud_stop_adc();
			bk_dma_stop(aud_tras_drv_info.voc_info.adc_dma_id);
			ring_buffer_clear(&(aud_tras_drv_info.voc_info.mic_rb));
		} else {
			LOGI("close uac mic \r\n");
			ret = bk_aud_uac_stop_mic();
			if (ret != BK_OK) {
				LOGE("stop uac mic fail \r\n");
				err = BK_ERR_AUD_INTF_UAC_MIC;
				goto voc_ctrl_mic_fail;
			}
			uac_mic_read_flag = false;
		}
	} else {
		err = BK_ERR_AUD_INTF_PARAM;
		goto voc_ctrl_mic_fail;
	}

	GLOBAL_INT_DISABLE();
	if (aud_tras_drv_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_BOARD)
		aud_tras_drv_info.voc_info.mic_en = mic_en;
	GLOBAL_INT_RESTORE();
/*
	if (aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb)
		aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb(EVENT_AUD_TRAS_VOC_CTRL_MIC, BK_ERR_AUD_INTF_OK);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);
	return BK_OK;

voc_ctrl_mic_fail:
	if (aud_tras_drv_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_BOARD) {
		bk_aud_stop_adc();
		bk_dma_stop(aud_tras_drv_info.voc_info.adc_dma_id);
	}
/*
	if (aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb)
		aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb(EVENT_AUD_TRAS_VOC_CTRL_MIC, err);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, err);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, err, APP_MODULE);

	return err;
}

static bk_err_t aud_tras_drv_voc_ctrl_spk(aud_intf_voc_spk_ctrl_t spk_en)
{
	bk_err_t ret = BK_OK;
	bk_err_t err = BK_ERR_AUD_INTF_FAIL;
	uint32_t size = 0;
	uint8_t *pcm_data = NULL;

	GLOBAL_INT_DECLARATION();

	if (spk_en == AUD_INTF_VOC_SPK_OPEN) {
		pcm_data = (uint8_t *)os_malloc(aud_tras_drv_info.voc_info.mic_samp_rate_points*2);
		if (pcm_data == NULL) {
			LOGE("malloc temp pcm_data fial \r\n");
			err = BK_ERR_AUD_INTF_MEMY;
			goto voc_ctrl_spk_fail;
		} else {
			os_memset(pcm_data, 0x00, aud_tras_drv_info.voc_info.mic_samp_rate_points*2);
		}

		if (aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_BOARD) {
			LOGI("open spk \r\n");
			/* enable dac */
			bk_aud_start_dac();
			aud_tras_dac_pa_ctrl(true);

			ret = bk_dma_start(aud_tras_drv_info.voc_info.dac_dma_id);
			if (ret != BK_OK) {
				LOGE("start dac dma fail \r\n");
				err = BK_ERR_AUD_INTF_DMA;
				goto voc_ctrl_spk_fail;
			}
		} else {
			LOGI("open uac spk \r\n");
			ret = bk_aud_uac_start_spk();
			if (ret != BK_OK) {
				LOGE("open uac spk fail \r\n");
				err = BK_ERR_AUD_INTF_UAC_SPK;
				goto voc_ctrl_spk_fail;
			}
		}

		/* write two frame data to speaker and ref ring buffer */
		size = ring_buffer_write(&(aud_tras_drv_info.voc_info.speaker_rb), (uint8_t *)pcm_data, aud_tras_drv_info.voc_info.mic_samp_rate_points*2);
		if (size != aud_tras_drv_info.voc_info.mic_samp_rate_points*2) {
			LOGE("the data writeten to speaker_ring_buff error, size=%d \r\n", size);
			err = BK_ERR_AUD_INTF_RING_BUFF;
			goto voc_ctrl_spk_fail;
		}

		size = ring_buffer_write(&(aud_tras_drv_info.voc_info.speaker_rb), (uint8_t *)pcm_data, aud_tras_drv_info.voc_info.mic_samp_rate_points*2);
		if (size != aud_tras_drv_info.voc_info.mic_samp_rate_points*2) {
			LOGE("the data writeten to speaker_ring_buff error, size=%d \r\n", size);
			err = BK_ERR_AUD_INTF_RING_BUFF;
			goto voc_ctrl_spk_fail;
		}

		os_free(pcm_data);
		pcm_data = NULL;
	} else if (spk_en == AUD_INTF_VOC_SPK_CLOSE) {
		if (aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_BOARD) {
			LOGI("open spk \r\n");
			bk_aud_stop_dac();
			aud_tras_dac_pa_ctrl(false);
			bk_dma_stop(aud_tras_drv_info.voc_info.dac_dma_id);
		} else {
			LOGI("close uac spk \r\n");
			ret = bk_aud_uac_stop_spk();
			if (ret != BK_OK) {
				LOGE("close uac spk fail \r\n");
				err = BK_ERR_AUD_INTF_UAC_SPK;
				goto voc_ctrl_spk_fail;
			}
		}
		ring_buffer_clear(&(aud_tras_drv_info.voc_info.speaker_rb));
	} else {
		err = BK_ERR_AUD_INTF_PARAM;
		goto voc_ctrl_spk_fail;
	}

	GLOBAL_INT_DISABLE();
	if (aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_BOARD)
		aud_tras_drv_info.voc_info.spk_en = spk_en;
	GLOBAL_INT_RESTORE();
/*
	if (aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb)
		aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb(EVENT_AUD_TRAS_VOC_CTRL_SPK, BK_ERR_AUD_INTF_OK);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);

	return BK_OK;

voc_ctrl_spk_fail:
	if (pcm_data)
		os_free(pcm_data);

	if (spk_en == AUD_INTF_VOC_SPK_OPEN) {
		if (aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_BOARD) {
			bk_aud_stop_dac();
			aud_tras_dac_pa_ctrl(false);
			bk_dma_stop(aud_tras_drv_info.voc_info.dac_dma_id);
		} else {
			bk_aud_uac_stop_spk();
		}
	}
/*
	if (aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb)
		aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb(EVENT_AUD_TRAS_VOC_CTRL_SPK, err);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, err);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, err, APP_MODULE);

	return err;
}

static bk_err_t aud_tras_drv_voc_ctrl_aec(bool aec_en)
{
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	aud_tras_drv_info.voc_info.aec_enable = aec_en;
	GLOBAL_INT_RESTORE();
/*
	if (aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb)
		aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb(EVENT_AUD_TRAS_VOC_CTRL_AEC, BK_ERR_AUD_INTF_OK);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);

	return BK_OK;
}

static bk_err_t aud_tras_drv_set_aec_para(aud_intf_voc_aec_ctl_t *aec_ctl)
{
	switch (aec_ctl->op) {
		case AUD_INTF_VOC_AEC_MIC_DELAY:
			aud_tras_drv_info.voc_info.aec_info->aec_config->mic_delay = aec_ctl->value;
			aec_ctrl(aud_tras_drv_info.voc_info.aec_info->aec, AEC_CTRL_CMD_SET_MIC_DELAY, aud_tras_drv_info.voc_info.aec_info->aec_config->mic_delay);
			break;

		case AUD_INTF_VOC_AEC_EC_DEPTH:
			aud_tras_drv_info.voc_info.aec_info->aec_config->ec_depth = aec_ctl->value;
			aec_ctrl(aud_tras_drv_info.voc_info.aec_info->aec, AEC_CTRL_CMD_SET_EC_DEPTH, aud_tras_drv_info.voc_info.aec_info->aec_config->ec_depth);
			break;

		case AUD_INTF_VOC_AEC_REF_SCALE:
			aud_tras_drv_info.voc_info.aec_info->aec_config->ref_scale = (uint8_t)aec_ctl->value;
			aec_ctrl(aud_tras_drv_info.voc_info.aec_info->aec, AEC_CTRL_CMD_SET_REF_SCALE, aud_tras_drv_info.voc_info.aec_info->aec_config->ref_scale);
			break;

		case AUD_INTF_VOC_AEC_VOICE_VOL:
			aud_tras_drv_info.voc_info.aec_info->aec_config->voice_vol = (uint8_t)aec_ctl->value;
			aec_ctrl(aud_tras_drv_info.voc_info.aec_info->aec, AEC_CTRL_CMD_SET_VOL, aud_tras_drv_info.voc_info.aec_info->aec_config->voice_vol);
			break;

		case AUD_INTF_VOC_AEC_TXRX_THR:
			aud_tras_drv_info.voc_info.aec_info->aec_config->TxRxThr = aec_ctl->value;
			aec_ctrl(aud_tras_drv_info.voc_info.aec_info->aec, AEC_CTRL_CMD_SET_TxRxThr, aud_tras_drv_info.voc_info.aec_info->aec_config->TxRxThr);
			break;

		case AUD_INTF_VOC_AEC_TXRX_FLR:
			aud_tras_drv_info.voc_info.aec_info->aec_config->TxRxFlr = aec_ctl->value;
			aec_ctrl(aud_tras_drv_info.voc_info.aec_info->aec, AEC_CTRL_CMD_SET_TxRxFlr, aud_tras_drv_info.voc_info.aec_info->aec_config->TxRxFlr);
			break;

		case AUD_INTF_VOC_AEC_NS_LEVEL:
			aud_tras_drv_info.voc_info.aec_info->aec_config->ns_level = (uint8_t)aec_ctl->value;
			aec_ctrl(aud_tras_drv_info.voc_info.aec_info->aec, AEC_CTRL_CMD_SET_NS_LEVEL, aud_tras_drv_info.voc_info.aec_info->aec_config->ns_level);
			break;

		case AUD_INTF_VOC_AEC_NS_PARA:
			aud_tras_drv_info.voc_info.aec_info->aec_config->ns_para = (uint8_t)aec_ctl->value;
			aec_ctrl(aud_tras_drv_info.voc_info.aec_info->aec, AEC_CTRL_CMD_SET_NS_PARA, aud_tras_drv_info.voc_info.aec_info->aec_config->ns_para);
			break;

		case AUD_INTF_VOC_AEC_DRC:
			aud_tras_drv_info.voc_info.aec_info->aec_config->drc = (uint8_t)aec_ctl->value;
			aec_ctrl(aud_tras_drv_info.voc_info.aec_info->aec, AEC_CTRL_CMD_SET_DRC, aud_tras_drv_info.voc_info.aec_info->aec_config->drc);
			break;

		case AUD_INTF_VOC_AEC_INIT_FLAG:
			aud_tras_drv_info.voc_info.aec_info->aec_config->init_flags = (uint16_t)aec_ctl->value;
			aec_ctrl(aud_tras_drv_info.voc_info.aec_info->aec, AEC_CTRL_CMD_SET_FLAGS, aud_tras_drv_info.voc_info.aec_info->aec_config->init_flags);
			break;

		default:
			break;
	}
	os_free(aec_ctl);

/*
	if (aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb)
		aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb(EVENT_AUD_TRAS_VOC_SET_AEC_PARA, BK_ERR_AUD_INTF_OK);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);

	return BK_OK;
}

static bk_err_t aud_tras_drv_get_aec_para(void)
{
	LOGI("aud_intf aec params: \r\n");
	LOGI("init_flags: %d \r\n", aud_tras_drv_info.voc_info.aec_info->aec_config->init_flags);
	LOGI("ec_depth: %d \r\n", aud_tras_drv_info.voc_info.aec_info->aec_config->ec_depth);
	LOGI("ref_scale: %d \r\n", aud_tras_drv_info.voc_info.aec_info->aec_config->ref_scale);
	LOGI("voice_vol: %d \r\n", aud_tras_drv_info.voc_info.aec_info->aec_config->voice_vol);
	LOGI("TxRxThr: %d \r\n", aud_tras_drv_info.voc_info.aec_info->aec_config->TxRxThr);
	LOGI("TxRxFlr: %d \r\n", aud_tras_drv_info.voc_info.aec_info->aec_config->TxRxFlr);
	LOGI("ns_level: %d \r\n", aud_tras_drv_info.voc_info.aec_info->aec_config->ns_level);
	LOGI("ns_para: %d \r\n", aud_tras_drv_info.voc_info.aec_info->aec_config->ns_para);
	LOGI("drc: %d \r\n", aud_tras_drv_info.voc_info.aec_info->aec_config->drc);
	LOGI("end \r\n");
/*
	if (aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb)
		aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb(EVENT_AUD_TRAS_VOC_GET_AEC_PARA, BK_ERR_AUD_INTF_OK);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);

	return BK_OK;
}

static bk_err_t aud_tras_drv_voc_tx_debug(aud_intf_dump_data_callback callback)
{
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	aud_tras_drv_info.voc_info.aud_tras_dump_tx_cb = callback;
	GLOBAL_INT_RESTORE();
/*
	if (aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb)
		aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb(EVENT_AUD_TRAS_VOC_TX_DEBUG, BK_ERR_AUD_INTF_OK);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);

	return BK_OK;
}

static bk_err_t aud_tras_drv_voc_rx_debug(aud_intf_dump_data_callback callback)
{
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	aud_tras_drv_info.voc_info.aud_tras_dump_rx_cb = callback;
	GLOBAL_INT_RESTORE();
/*
	if (aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb)
		aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb(EVENT_AUD_TRAS_VOC_RX_DEBUG, BK_ERR_AUD_INTF_OK);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);

	return BK_OK;
}

static bk_err_t aud_tras_drv_voc_aec_debug(aud_intf_dump_data_callback callback)
{
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	aud_tras_drv_info.voc_info.aud_tras_dump_aec_cb = callback;
	GLOBAL_INT_RESTORE();
/*
	if (aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb)
		aud_tras_drv_info.voc_info.aud_tras_drv_voc_event_cb(EVENT_AUD_TRAS_VOC_AEC_DEBUG, BK_ERR_AUD_INTF_OK);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);

	return BK_OK;
}

static bk_err_t aud_tras_drv_uac_register_connect_state_cb(void * cb)
{
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	aud_tras_drv_info.aud_tras_drv_uac_connect_state_cb = cb;
	GLOBAL_INT_RESTORE();
/*
	if (aud_tras_drv_info.aud_tras_drv_com_event_cb)
		aud_tras_drv_info.aud_tras_drv_com_event_cb(EVENT_AUD_TRAS_COM_UAC_REGIS_CONT_STATE_CB, BK_ERR_AUD_INTF_OK);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);

	return BK_OK;
}

static bk_err_t aud_tras_drv_spk_set_samp_rate(aud_dac_samp_rate_t samp_rate)
{
	bk_err_t ret = BK_ERR_AUD_INTF_OK;

	if (samp_rate != AUD_DAC_SAMP_RATE_MAX) {
	
	bk_aud_stop_dac();
		aud_tras_dac_pa_ctrl(false);
		bk_aud_set_dac_samp_rate(samp_rate);
		bk_aud_start_dac();
		bk_aud_start_dac();
		aud_tras_dac_pa_ctrl(true);
		ret = BK_ERR_AUD_INTF_OK;
	} else {
		ret = BK_ERR_AUD_INTF_PARAM;
	}
/*
	if (aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb)
		aud_tras_drv_info.spk_info.aud_tras_drv_spk_event_cb(EVENT_AUD_TRAS_SPK_SET_SAMP_RATE, ret);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, ret);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, ret, APP_MODULE);

	return ret;
}



bk_err_t aud_tras_drv_set_work_mode(aud_intf_work_mode_t mode)
{
	bk_err_t ret = BK_OK;
//	bk_err_t err = BK_ERR_AUD_INTF_FAIL;

	LOGI("set mode: %d \r\n", mode);

	switch (mode) {
		case AUD_INTF_WORK_MODE_GENERAL:
			if ((aud_tras_drv_info.work_mode == AUD_INTF_WORK_MODE_VOICE) && (aud_tras_drv_info.voc_info.status != AUD_TRAS_DRV_VOC_STA_NULL)) {
				aud_tras_drv_voc_deinit();
			}
			aud_tras_drv_info.work_mode = AUD_INTF_WORK_MODE_GENERAL;
			break;

		case AUD_INTF_WORK_MODE_VOICE:
			if (aud_tras_drv_info.work_mode == AUD_INTF_WORK_MODE_GENERAL) {
				if (aud_tras_drv_info.mic_info.status != AUD_TRAS_DRV_MIC_STA_NULL) {
					ret = aud_tras_drv_mic_deinit();
					if (ret != BK_OK) {
						LOGE("mic deinit fail \r\n");
						//return BK_FAIL;
					}
				}
				if (aud_tras_drv_info.spk_info.status != AUD_TRAS_DRV_SPK_STA_NULL) {
					ret = aud_tras_drv_spk_deinit();
					if (ret != BK_OK) {
						LOGE("spk deinit fail \r\n");
						//return BK_FAIL;
					}
				}
			}
			aud_tras_drv_info.work_mode = AUD_INTF_WORK_MODE_VOICE;
			break;

		case AUD_INTF_WORK_MODE_NULL:
			if ((aud_tras_drv_info.work_mode == AUD_INTF_WORK_MODE_VOICE) && (aud_tras_drv_info.voc_info.status != AUD_TRAS_DRV_VOC_STA_NULL)) {
				aud_tras_drv_voc_deinit();
			}
			if (aud_tras_drv_info.work_mode == AUD_INTF_WORK_MODE_GENERAL) {
				if (aud_tras_drv_info.mic_info.status != AUD_TRAS_DRV_MIC_STA_NULL) {
					ret = aud_tras_drv_mic_deinit();
					if (ret != BK_OK) {
						LOGE("mic deinit fail \r\n");
						//return BK_FAIL;
					}
				}
				if (aud_tras_drv_info.spk_info.status != AUD_TRAS_DRV_SPK_STA_NULL) {
					ret = aud_tras_drv_spk_deinit();
					if (ret != BK_OK) {
						LOGE("spk deinit fail \r\n");
						//return BK_FAIL;
					}
				}
			}
			aud_tras_drv_info.work_mode = AUD_INTF_WORK_MODE_NULL;
			break;

		default:
			ret = BK_ERR_AUD_INTF_FAIL;
			break;
	}

/*
	if (aud_tras_drv_info.aud_tras_drv_com_event_cb)
		aud_tras_drv_info.aud_tras_drv_com_event_cb(EVENT_AUD_TRAS_COM_SET_MODE, BK_ERR_AUD_INTF_OK);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, ret);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, ret, APP_MODULE);

	return BK_ERR_AUD_INTF_OK;
}

static bk_err_t aud_tras_drv_set_mic_gain(uint8_t value)
{
	if (aud_tras_drv_info.mic_info.mic_type == AUD_INTF_MIC_TYPE_BOARD || aud_tras_drv_info.mic_info.mic_type == AUD_INTF_MIC_TYPE_BOARD) {
		bk_aud_set_adc_gain((uint32_t)value);
/*
		if (aud_tras_drv_info.aud_tras_drv_com_event_cb)
			aud_tras_drv_info.aud_tras_drv_com_event_cb(EVENT_AUD_TRAS_COM_SET_MIC_GAIN, BK_ERR_AUD_INTF_OK);
*/
		//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
		msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);

		return BK_ERR_AUD_INTF_OK;
	}

	return BK_ERR_AUD_INTF_PARAM;
}

#if CONFIG_AUD_TRAS_UAC_SPK_VOL_CTRL_MODE_STOP_UAC_TRAS
static void uac_mic_spk_recover(void)
{
	LOGD("%s \n", __func__);
	if (aud_tras_drv_info.work_mode == AUD_INTF_WORK_MODE_GENERAL) {
		/* check mic status */
		if ((aud_tras_drv_info.mic_info.status == AUD_TRAS_DRV_MIC_STA_START) && (aud_tras_drv_info.mic_info.mic_type == AUD_INTF_MIC_TYPE_UAC)) {
			bk_aud_uac_start_mic();
		}

		/* check spk status */
		if ((aud_tras_drv_info.spk_info.status == AUD_TRAS_DRV_SPK_STA_START) && (aud_tras_drv_info.spk_info.spk_type == AUD_INTF_SPK_TYPE_UAC)) {
			bk_aud_uac_start_spk();
		}
	} else {
		if (aud_tras_drv_info.voc_info.status == AUD_TRAS_DRV_VOC_STA_START) {
			if ((aud_tras_drv_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_UAC) && (aud_tras_drv_info.voc_info.mic_en == AUD_INTF_VOC_MIC_OPEN)) {
				bk_aud_uac_start_mic();
			}
			if ((aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_UAC) && (aud_tras_drv_info.voc_info.spk_en == AUD_INTF_VOC_SPK_OPEN)) {
				bk_aud_uac_start_spk();
			}
		}
	}
}
#endif //CONFIG_AUD_TRAS_UAC_SPK_VOL_CTRL_MODE_STOP_UAC_TRAS

static bk_err_t aud_tras_drv_set_spk_gain(uint16_t value)
{
	bk_err_t ret = BK_OK;
	if (aud_tras_drv_info.spk_info.spk_type == AUD_INTF_SPK_TYPE_BOARD || aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_BOARD) {
		bk_aud_set_dac_gain((uint32_t)value);
//		if (aud_tras_drv_info.aud_tras_drv_com_event_cb)
//			aud_tras_drv_info.aud_tras_drv_com_event_cb(EVENT_AUD_TRAS_COM_SET_SPK_GAIN, BK_ERR_AUD_INTF_OK);
		return BK_ERR_AUD_INTF_OK;
	}

	if (aud_tras_drv_info.spk_info.spk_type == AUD_INTF_SPK_TYPE_UAC || aud_tras_drv_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_UAC) {
		/* check uac support volume configuration */
		if (bk_aud_uac_check_spk_gain_cfg() == BK_OK) {
			LOGI("%s, set uac speaker volume: %d \n", __func__, value);
#if CONFIG_AUD_TRAS_UAC_SPK_VOL_CTRL_MODE_STOP_UAC_TRAS
			/* step1: stop uac mic and speaker
			   step2: set volume
			   step3: recover uac mic and speaker status
			*/
			/* step1: stop uac mic and speaker */
			bk_aud_uac_stop_mic();
			bk_aud_uac_stop_spk();

			/* step2: set volume */
			ret = bk_aud_uac_set_spk_gain((uint32_t)value);
//			if (aud_tras_drv_info.aud_tras_drv_com_event_cb)
//				aud_tras_drv_info.aud_tras_drv_com_event_cb(EVENT_AUD_TRAS_COM_SET_SPK_GAIN, ret);

			/* step3: recover uac mic and speaker status */
			uac_mic_spk_recover();
#endif

#if CONFIG_AUD_TRAS_UAC_SPK_VOL_CTRL_MODE_DIRECT
			ret = bk_aud_uac_set_spk_gain((uint32_t)value);
#endif

#if CONFIG_AUD_TRAS_UAC_SPK_VOL_CTRL_MODE_MUTE
			bk_aud_uac_ctrl_spk_mute(1);
			ret = bk_aud_uac_set_spk_gain((uint32_t)value);
			bk_aud_uac_ctrl_spk_mute(0);
#endif

			return ret;
		} else {
			LOGW("The uac speaker not support volume configuration \n");
			return BK_ERR_AUD_INTF_PARAM;
		}
	}

	return ret;
}


static void aud_tras_drv_main(beken_thread_arg_t param_data)
{
	bk_err_t ret = BK_OK;
	aud_intf_drv_config_t *aud_trs_setup = NULL;

	aud_trs_setup = (aud_intf_drv_config_t *)param_data;

	aud_tras_drv_info.work_mode = aud_trs_setup->setup.work_mode;
	aud_tras_drv_info.aud_tras_tx_mic_data = aud_trs_setup->setup.aud_intf_tx_mic_data;
	aud_tras_drv_info.aud_tras_rx_spk_data = aud_trs_setup->setup.aud_intf_rx_spk_data;
//	aud_tras_drv_info.aud_tras_drv_com_event_cb = aud_trs_setup->aud_tras_drv_com_event_cb;

	/* set work status to IDLE */
	aud_tras_drv_info.status = AUD_TRAS_DRV_STA_IDLE;
/*
	if (aud_tras_drv_info.aud_tras_drv_com_event_cb)
		aud_tras_drv_info.aud_tras_drv_com_event_cb(EVENT_AUD_TRAS_COM_INIT, BK_ERR_AUD_INTF_OK);
*/
	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);

	while(1) {
		aud_tras_drv_msg_t msg;
		GPIO_UP(5);
		ret = rtos_pop_from_queue(&aud_trs_drv_int_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			switch (msg.op) {
				case AUD_TRAS_DRV_IDLE:
					break;

				case AUD_TRAS_DRV_EXIT:
					LOGD("goto: AUD_TRAS_DRV_EXIT \r\n");
					goto aud_tras_drv_exit;
					break;

				case AUD_TRAS_DRV_SET_MODE:
					LOGD("goto: AUD_TRAS_DRV_SET_MODE \r\n");
					aud_tras_drv_set_work_mode(*(aud_intf_work_mode_t *)msg.param);
					break;

				/* mic op */
				case AUD_TRAS_DRV_MIC_INIT:
					LOGD("goto: AUD_TRAS_DRV_MIC_INIT \r\n");
					aud_tras_drv_mic_init((aud_intf_mic_config_t *)msg.param);
					break;

				case AUD_TRAS_DRV_MIC_DEINIT:
					LOGD("goto: AUD_TRAS_DRV_MIC_DEINIT \r\n");
					ret = aud_tras_drv_mic_deinit();
					//msg_send_to_app_mailbox(aud_to_media_app_msg, ret);
					msg_send_to_media_major_mailbox(aud_to_media_app_msg, ret, APP_MODULE);
					break;

				case AUD_TRAS_DRV_MIC_START:
					LOGD("goto: AUD_TRAS_DRV_MIC_START \r\n");
					aud_tras_drv_mic_start();
					break;

				case AUD_TRAS_DRV_MIC_PAUSE:
					LOGD("goto: AUD_TRAS_DRV_MIC_PAUSE \r\n");
					aud_tras_drv_mic_pause();
					break;

				case AUD_TRAS_DRV_MIC_STOP:
					LOGD("goto: AUD_TRAS_DRV_MIC_STOP \r\n");
					aud_tras_drv_mic_stop();
					break;

				case AUD_TRAS_DRV_MIC_SET_CHL:
					LOGD("goto: AUD_TRAS_DRV_MIC_SET_MIC_CHL \r\n");
					aud_tras_drv_mic_set_chl(*((aud_intf_mic_chl_t *)msg.param));
					break;

				case AUD_TRAS_DRV_MIC_SET_SAMP_RATE:
					LOGD("goto: AUD_TRAS_DRV_MIC_SET_SAMP_RATE \r\n");
					aud_tras_drv_mic_set_samp_rate(*((aud_adc_samp_rate_t *)msg.param));
					break;

				case AUD_TRAS_DRV_MIC_SET_GAIN:
					LOGD("goto: AUD_TRAS_DRV_MIC_SET_GAIN \r\n");
					aud_tras_drv_set_mic_gain(*((uint8_t *)msg.param));
					break;

				case AUD_TRAS_DRV_MIC_TX_DATA:
					LOGD("goto: AUD_TRAS_DRV_MIC_TX_DATA \r\n");
					aud_tras_drv_mic_tx_data();
					break;

				/* spk op */
				case AUD_TRAS_DRV_SPK_INIT:
					LOGD("goto: AUD_TRAS_DRV_SPK_INIT \r\n");
					aud_tras_drv_spk_init((aud_intf_spk_config_t *)msg.param);
					break;

				case AUD_TRAS_DRV_SPK_DEINIT:
					LOGD("goto: AUD_TRAS_DRV_SPK_DEINIT \r\n");
					ret = aud_tras_drv_spk_deinit();
					//msg_send_to_app_mailbox(aud_to_media_app_msg, ret);
					msg_send_to_media_major_mailbox(aud_to_media_app_msg, ret, APP_MODULE);
					break;

				case AUD_TRAS_DRV_SPK_START:
					LOGD("goto: AUD_TRAS_DRV_SPK_START \r\n");
					aud_tras_drv_spk_start();
					break;

				case AUD_TRAS_DRV_SPK_PAUSE:
					LOGD("goto: AUD_TRAS_DRV_SPK_PAUSE \r\n");
					aud_tras_drv_spk_pause();
					break;

				case AUD_TRAS_DRV_SPK_STOP:
					LOGD("goto: AUD_TRAS_DRV_SPK_STOP \r\n");
					aud_tras_drv_spk_stop();
					break;

				case AUD_TRAS_DRV_SPK_SET_CHL:
					LOGD("goto: AUD_TRAS_DRV_MIC_SET_SPK_CHL \r\n");
					aud_tras_drv_spk_set_chl(*((aud_intf_spk_chl_t *)msg.param));
					break;


				case AUD_TRAS_DRV_SPK_SET_SAMP_RATE:
					LOGD("goto: AUD_TRAS_DRV_SPK_STOP \r\n");
					aud_tras_drv_spk_set_samp_rate(*((aud_dac_samp_rate_t *)msg.param));
					break;

				case AUD_TRAS_DRV_SPK_SET_GAIN:
					LOGD("goto: AUD_TRAS_DRV_SPK_SET_GAIN \r\n");
					ret = aud_tras_drv_set_spk_gain(*((uint16_t *)msg.param));
					msg_send_to_media_major_mailbox(aud_to_media_app_msg, ret, APP_MODULE);
					break;

				case AUD_TRAS_DRV_SPK_REQ_DATA:
					LOGD("goto: AUD_TRAS_DRV_SPK_REQ_DATA \r\n");
					aud_tras_drv_spk_req_data();
					break;

				/* voc op */
				case AUD_TRAS_DRV_VOC_INIT:
					LOGD("goto: AUD_TRAS_DRV_VOC_INIT \r\n");
					aud_tras_drv_voc_init((aud_intf_voc_config_t*)msg.param);
					break;

				case AUD_TRAS_DRV_VOC_DEINIT:
					LOGD("goto: AUD_TRAS_DRV_VOC_DEINIT \r\n");
					if (aud_tras_drv_info.voc_info.status != AUD_TRAS_DRV_VOC_STA_NULL) {
						ret = aud_tras_drv_voc_deinit();
					} else {
						ret = BK_ERR_AUD_INTF_OK;
					}
					msg_send_to_media_major_mailbox(aud_to_media_app_msg, ret, APP_MODULE);
					break;

				case AUD_TRAS_DRV_VOC_START:
					LOGD("goto: AUD_TRAS_DRV_VOC_START \r\n");
					ret = aud_tras_drv_voc_start();
					msg_send_to_media_major_mailbox(aud_to_media_app_msg, ret, APP_MODULE);
					break;

				case AUD_TRAS_DRV_VOC_STOP:
					LOGD("goto: AUD_TRAS_DRV_VOC_STOP \r\n");
					aud_tras_drv_voc_stop();
					break;

				case AUD_TRAS_DRV_VOC_CTRL_MIC:
					LOGD("goto: AUD_TRAS_DRV_VOC_CTRL_MIC \r\n");
					aud_tras_drv_voc_ctrl_mic(*((aud_intf_voc_mic_ctrl_t *)msg.param));
					break;

				case AUD_TRAS_DRV_VOC_CTRL_SPK:
					LOGD("goto: AUD_TRAS_DRV_VOC_CTRL_SPK \r\n");
					aud_tras_drv_voc_ctrl_spk(*((aud_intf_voc_spk_ctrl_t *)msg.param));
					break;

				case AUD_TRAS_DRV_VOC_CTRL_AEC:
					LOGD("goto: AUD_TRAS_DRV_VOC_CTRL_MIC \r\n");
					aud_tras_drv_voc_ctrl_aec(*((bool *)msg.param));
					break;

				case AUD_TRAS_DRV_VOC_SET_MIC_GAIN:
					LOGD("goto: AUD_TRAS_DRV_VOC_SET_MIC_GAIN \r\n");
					aud_tras_drv_set_mic_gain(*((uint8_t *)msg.param));
					break;

				case AUD_TRAS_DRV_VOC_SET_SPK_GAIN:
					LOGI("goto: AUD_TRAS_DRV_VOC_SET_SPK_GAIN \r\n");
					ret = aud_tras_drv_set_spk_gain(*((uint16_t *)msg.param));
					msg_send_to_media_major_mailbox(aud_to_media_app_msg, ret, APP_MODULE);
					break;

				case AUD_TRAS_DRV_VOC_SET_AEC_PARA:
					LOGD("goto: AUD_TRAS_DRV_VOC_SET_AEC_PARA \r\n");
					aud_tras_drv_set_aec_para(msg.param);
					break;

				case AUD_TRAS_DRV_VOC_GET_AEC_PARA:
					LOGD("goto: AUD_TRAS_DRV_VOC_GET_AEC_PARA \r\n");
					aud_tras_drv_get_aec_para();
					break;

				case AUD_TRAS_DRV_VOC_TX_DEBUG:
					LOGD("goto: AUD_TRAS_DRV_VOC_TX_DEBUG \r\n");
					aud_tras_drv_voc_tx_debug((aud_intf_dump_data_callback)msg.param);
					break;

				case AUD_TRAS_DRV_VOC_RX_DEBUG:
					LOGD("goto: AUD_TRAS_DRV_VOC_RX_DEBUG \r\n");
					aud_tras_drv_voc_rx_debug((aud_intf_dump_data_callback)msg.param);
					break;

				case AUD_TRAS_DRV_VOC_AEC_DEBUG:
					LOGD("goto: AUD_TRAS_DRV_VOC_AEC_DEBUG \r\n");
					aud_tras_drv_voc_aec_debug((aud_intf_dump_data_callback)msg.param);
					break;

				/* uac op */
				case AUD_TRAS_DRV_UAC_REGIS_CONT_STATE_CB:
					LOGD("goto: AUD_TRAS_DRV_UAC_REGIS_CONT_STATE_CB \r\n");
					aud_tras_drv_uac_register_connect_state_cb(msg.param);
					break;

				case AUD_TRAS_DRV_UAC_CONT:
					LOGD("goto: AUD_TRAS_DRV_UAC_CONT \r\n");
					aud_tras_uac_connect_handle();
					break;

				case AUD_TRAS_DRV_UAC_DISCONT:
					LOGD("goto: AUD_TRAS_DRV_UAC_CONT \r\n");
					aud_tras_uac_disconnect_handle();
					break;

				case AUD_TRAS_DRV_UAC_AUTO_CONT_CTRL:
					LOGD("goto: AUD_TRAS_DRV_UAC_AUTO_CONT_CTRL \r\n");
					aud_tras_uac_auto_connect_ctrl(*((bool *)msg.param));
					break;

				/* voc int op */
				case AUD_TRAS_DRV_AEC:
					LOGD("goto: AUD_TRAS_DRV_AEC \r\n");
					if (aud_tras_drv_info.voc_info.status == AUD_TRAS_DRV_VOC_STA_START) {
						aud_tras_aec();
					}
					break;

				case AUD_TRAS_DRV_ENCODER:
					LOGD("goto: AUD_TRAS_DRV_ENCODER \r\n");
					if (aud_tras_drv_info.voc_info.status == AUD_TRAS_DRV_VOC_STA_START) {
						aud_tras_enc();
					}
					break;

				case AUD_TRAS_DRV_DECODER:
					LOGD("goto: AUD_TRAS_DRV_DECODER \r\n");
					if (aud_tras_drv_info.voc_info.status == AUD_TRAS_DRV_VOC_STA_START) {
						aud_tras_dec();
					}
					break;

				default:
					break;
			}
			GPIO_DOWN(5);
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

	LOGI("aud_tras_drv_com_event_cb \r\n");

	aud_tras_drv_info.work_mode = AUD_INTF_WORK_MODE_NULL;
	aud_tras_drv_info.aud_tras_tx_mic_data = NULL;
	aud_tras_drv_info.aud_tras_rx_spk_data = NULL;

	/* set work status to NULL */
	aud_tras_drv_info.status = AUD_TRAS_DRV_STA_NULL;


	/* delete msg queue */
	ret = rtos_deinit_queue(&aud_trs_drv_int_msg_que);
	if (ret != kNoErr) {
		LOGE("delete message queue fail \r\n");
	}
	aud_trs_drv_int_msg_que = NULL;
	LOGI("delete message queue complete \r\n");

	//msg_send_to_app_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK);
	msg_send_to_media_major_mailbox(aud_to_media_app_msg, BK_ERR_AUD_INTF_OK, APP_MODULE);

/*
	if (aud_tras_drv_info.aud_tras_drv_com_event_cb)
		aud_tras_drv_info.aud_tras_drv_com_event_cb(EVENT_AUD_TRAS_COM_DEINIT, BK_ERR_AUD_INTF_OK);
	aud_tras_drv_info.aud_tras_drv_com_event_cb = NULL;
*/
	/* deinit semaphore used to  */
	if (mailbox_media_aud_mic_sem) {
		rtos_deinit_semaphore(&mailbox_media_aud_mic_sem);
		mailbox_media_aud_mic_sem = NULL;
	}

	/* reset uac to default */
	aud_tras_drv_info.aud_tras_drv_uac_connect_state_cb = NULL;
	aud_tras_drv_info.uac_status = AUD_INTF_UAC_NORMAL_DISCONNECTED;
	aud_tras_drv_info.uac_auto_connect = true;

	/* delete task */
	aud_trs_drv_thread_hdl = NULL;
	rtos_delete_thread(NULL);
}

aud_intf_drv_config_t aud_trs_drv_setup_bak = {0};

bk_err_t aud_tras_drv_init(aud_intf_drv_config_t *setup_cfg)
{
	bk_err_t ret = BK_OK;

	/* init semaphore used to  */
	if (!mailbox_media_aud_mic_sem) {
		ret = rtos_init_semaphore(&mailbox_media_aud_mic_sem, 1);

		if (ret != BK_OK)
		{
			LOGE("%s, create mailbox audio mic semaphore failed\n");
			return BK_FAIL;
		}
	}

	if ((!aud_trs_drv_thread_hdl) && (!aud_trs_drv_int_msg_que)) {
		LOGI("init audio transfer driver \r\n");
		os_memcpy(&aud_trs_drv_setup_bak, setup_cfg, sizeof(aud_intf_drv_config_t));

		ret = rtos_init_queue(&aud_trs_drv_int_msg_que,
							  "aud_tras_int_que",
							  sizeof(aud_tras_drv_msg_t),
							  TU_QITEM_COUNT);
		if (ret != kNoErr) {
			LOGE("ceate audio transfer driver internal message queue fail \r\n");
			return BK_FAIL;
		}
		LOGI("ceate audio transfer driver internal message queue complete \r\n");

		//create audio transfer driver task
		ret = rtos_create_thread(&aud_trs_drv_thread_hdl,
							 setup_cfg->setup.task_config.priority,
							 "audio_intf",
							 (beken_thread_function_t)aud_tras_drv_main,
							 4096,
							 (beken_thread_arg_t)&aud_trs_drv_setup_bak);
		if (ret != kNoErr) {
			LOGE("create audio transfer driver task fail \r\n");
			rtos_deinit_queue(&aud_trs_drv_int_msg_que);
			aud_trs_drv_int_msg_que = NULL;
			aud_trs_drv_thread_hdl = NULL;
		}
		LOGI("create audio transfer driver task complete \r\n");

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
			LOGE("audio send msg: AUD_TRAS_DRV_EXIT fail \r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}


bk_err_t audio_event_handle(media_mailbox_msg_t * msg)
{
	bk_err_t ret = BK_FAIL;

	/* save mailbox msg received from media app */
	aud_to_media_app_msg = msg;

	LOGD(" %s, event: %d \n", __func__, msg->event);

	switch (msg->event)
	{
		case EVENT_AUD_INIT_REQ:
			ret = aud_tras_drv_init((aud_intf_drv_config_t *)msg->param);
			if (ret != BK_OK) {
				LOGE("aud_tras_drv_init fail, ret: %d \n", ret);
				//msg_send_to_app_mailbox(aud_to_media_app_msg, ret);
				msg_send_to_media_major_mailbox(aud_to_media_app_msg, ret, APP_MODULE);
			}
			break;

		case EVENT_AUD_DEINIT_REQ:
			aud_tras_drv_deinit();
			break;

		case EVENT_AUD_SET_MODE_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_SET_MODE, (void *)msg->param);
			break;

		/* mic mode event */
		case EVENT_AUD_MIC_INIT_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_MIC_INIT, (void *)msg->param);
			break;

		case EVENT_AUD_MIC_DEINIT_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_MIC_DEINIT, NULL);
			break;

		case EVENT_AUD_MIC_START_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_MIC_START, NULL);
			break;

		case EVENT_AUD_MIC_PAUSE_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_MIC_PAUSE, NULL);
			break;

		case EVENT_AUD_MIC_STOP_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_MIC_STOP, NULL);
			break;

		case EVENT_AUD_MIC_SET_CHL_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_MIC_SET_CHL, NULL);
			break;

		case EVENT_AUD_MIC_SET_SAMP_RATE_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_MIC_SET_SAMP_RATE, NULL);
			break;

		case EVENT_AUD_MIC_SET_GAIN_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_MIC_SET_GAIN, NULL);
			break;

		/* spk event */
		case EVENT_AUD_SPK_INIT_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_SPK_INIT, (void *)msg->param);
			break;

		case EVENT_AUD_SPK_DEINIT_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_SPK_DEINIT, NULL);
			break;

		case EVENT_AUD_SPK_START_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_SPK_START, NULL);
			break;

		case EVENT_AUD_SPK_PAUSE_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_SPK_PAUSE, NULL);
			break;

		case EVENT_AUD_SPK_STOP_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_SPK_STOP, NULL);
			break;

		case EVENT_AUD_SPK_SET_CHL_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_SPK_SET_CHL, (void *)msg->param);
			break;

		case EVENT_AUD_SPK_SET_SAMP_RATE_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_SPK_SET_SAMP_RATE, (void *)msg->param);
			break;

		case EVENT_AUD_SPK_SET_GAIN_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_SPK_SET_GAIN, (void *)msg->param);
			break;

		/* voc op */
		case EVENT_AUD_VOC_INIT_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_VOC_INIT, (void *)msg->param);
			break;

		case EVENT_AUD_VOC_DEINIT_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_VOC_DEINIT, NULL);
			break;

		case EVENT_AUD_VOC_START_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_VOC_START, NULL);
			break;

		case EVENT_AUD_VOC_STOP_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_VOC_STOP, NULL);
			break;

		case EVENT_AUD_VOC_CTRL_MIC_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_VOC_CTRL_MIC, (void *)msg->param);
			break;

		case EVENT_AUD_VOC_CTRL_SPK_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_VOC_CTRL_SPK, (void *)msg->param);
			break;

		case EVENT_AUD_VOC_CTRL_AEC_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_VOC_CTRL_AEC, (void *)msg->param);
			break;

		case EVENT_AUD_VOC_SET_MIC_GAIN_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_VOC_SET_MIC_GAIN, (void *)msg->param);
			break;

		case EVENT_AUD_VOC_SET_SPK_GAIN_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_VOC_SET_SPK_GAIN, (void *)msg->param);
			break;

		case EVENT_AUD_VOC_SET_AEC_PARA_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_VOC_SET_AEC_PARA, (void *)msg->param);
			break;

		case EVENT_AUD_VOC_GET_AEC_PARA_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_VOC_GET_AEC_PARA, (void *)msg->param);
			break;

		case EVENT_AUD_VOC_TX_DEBUG_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_VOC_TX_DEBUG, (void *)msg->param);
			break;

		case EVENT_AUD_VOC_RX_DEBUG_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_VOC_RX_DEBUG, (void *)msg->param);
			break;

		case EVENT_AUD_VOC_AEC_DEBUG_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_VOC_AEC_DEBUG, (void *)msg->param);
			break;

		/* uac event */
		case EVENT_AUD_UAC_REGIS_CONT_STATE_CB_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_UAC_REGIS_CONT_STATE_CB, (void *)msg->param);
			break;

		case EVENT_AUD_UAC_CONT_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_UAC_CONT, (void *)msg->param);
			break;

		case EVENT_AUD_UAC_DISCONT_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_UAC_CONT, NULL);
			break;

		case EVENT_AUD_UAC_AUTO_CONT_CTRL_REQ:
			aud_tras_drv_send_msg(AUD_TRAS_DRV_UAC_CONT, (void *)msg->param);
			break;

		case EVENT_AUD_MIC_DATA_NOTIFY:
			GPIO_UP(6);
			ret = rtos_set_semaphore(&msg->sem);
			if (ret != BK_OK)
			{
				LOGE("%s semaphore set failed: %d\n", __func__, ret);
			}
			GPIO_DOWN(6);
			break;

		case EVENT_AUD_SPK_DATA_NOTIFY:
			//TODO set sem
			//	;
			break;

		default:
			break;
	}

	return ret;
}



