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
#include "stdio.h"
#include "sys_driver.h"
#include "aud_intf_private.h"


#include "media_evt.h"
#include "media_mailbox_list_util.h"


#define AUD_INTF_TAG "aud_intf"

#define LOGI(...) BK_LOGI(AUD_INTF_TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(AUD_INTF_TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(AUD_INTF_TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(AUD_INTF_TAG, ##__VA_ARGS__)

/* check aud_intf busy status */
#define CHECK_AUD_INTF_BUSY_STA() do {\
		if (aud_intf_info.api_info.busy_status) {\
			return BK_ERR_AUD_INTF_BUSY;\
		}\
		aud_intf_info.api_info.busy_status = true;\
	} while(0)

//aud_intf_all_setup_t aud_all_setup;
aud_intf_info_t aud_intf_info = DEFAULT_AUD_INTF_CONFIG();

/* extern api */
//static void audio_tras_demo_deconfig(void);
beken_semaphore_t mailbox_media_app_aud_sem = NULL;
media_mailbox_msg_t *media_aud_mailbox_msg = NULL;

/*
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
*/

static bk_err_t aud_intf_voc_write_spk_data(uint8_t *dac_buff, uint32_t size);


bk_err_t mailbox_media_aud_send_msg(media_event_t event, void *param)
{
	bk_err_t ret = BK_OK;
	uint32_t result = BK_OK;

	if (!media_aud_mailbox_msg) {
		media_aud_mailbox_msg = os_malloc(sizeof(media_mailbox_msg_t));
		if (!media_aud_mailbox_msg) {
			LOGE("%s, malloc media_aud_mailbox_msg fail\n", __func__);
			aud_intf_info.api_info.busy_status = false;
			return BK_ERR_NO_MEM;
		}
	}

	media_aud_mailbox_msg->event = event;
	media_aud_mailbox_msg->dest = MAJOR_MODULE;
	media_aud_mailbox_msg->src = APP_MODULE;
	media_aud_mailbox_msg->param = (uint32_t)param;
	media_aud_mailbox_msg->result = result;
	media_aud_mailbox_msg->sem = mailbox_media_app_aud_sem;
	//        LOGE("====>>>>1 %s\n", __func__);
	msg_send_to_media_app_mailbox(media_aud_mailbox_msg, result);
	ret = rtos_get_semaphore(&mailbox_media_app_aud_sem, BEKEN_WAIT_FOREVER);
	if (ret != BK_OK)
	{
		LOGE("%s, rtos_get_semaphore\n", __func__);
		ret = BK_FAIL;
	}
	else
	{
		ret = result;
	}

	aud_intf_info.api_info.busy_status = false;
	return ret;
}


#if 0
/* common api */
static void aud_tras_drv_com_event_cb_handle(aud_tras_drv_com_event_t event, bk_err_t result)
{
	switch (event) {
		case EVENT_AUD_TRAS_COM_INIT:
			LOGD("init aud_intf driver complete \r\n");
			if (result == BK_ERR_AUD_INTF_OK) {
				aud_intf_info.drv_status = AUD_INTF_DRV_STA_IDLE;
			}
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_COM_DEINIT:
			LOGD("deinit aud_intf driver complete \r\n");
			if (result == BK_ERR_AUD_INTF_OK) {
				aud_intf_info.drv_status = AUD_INTF_DRV_STA_NULL;
			}
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_COM_SET_MODE:
			LOGD("set aud_intf mode complete \r\n");
			if (result == BK_ERR_AUD_INTF_OK) {
				aud_intf_info.drv_status = AUD_INTF_DRV_STA_WORK;
			}
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_COM_SET_MIC_GAIN:
			LOGD("set mic gain complete \r\n");
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_COM_SET_SPK_GAIN:
			LOGD("set spk gain complete \r\n");
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_COM_UAC_REGIS_CONT_STATE_CB:
			LOGD("register uac connect state change callback complete \r\n");
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_COM_UAC_AUTO_CONT_CTRL:
			LOGD("control uac automatically connect complete \r\n");
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		default:
			break;
	}
}
#endif


bk_err_t bk_aud_intf_set_mode(aud_intf_work_mode_t work_mode)
{
	bk_err_t ret = BK_OK;

	if (aud_intf_info.drv_info.setup.work_mode == work_mode)
		return BK_ERR_AUD_INTF_OK;

	CHECK_AUD_INTF_BUSY_STA();
	aud_intf_work_mode_t temp = aud_intf_info.drv_info.setup.work_mode;

	aud_intf_info.drv_info.setup.work_mode = work_mode;
	ret = mailbox_media_aud_send_msg(EVENT_AUD_SET_MODE_REQ, &aud_intf_info.drv_info.setup.work_mode);
	if (ret != BK_OK) {
		aud_intf_info.drv_info.setup.work_mode = temp;
	} else {
		aud_intf_info.drv_status = AUD_INTF_DRV_STA_WORK;
	}

	return ret;
}

bk_err_t bk_aud_intf_set_mic_gain(uint8_t value)
{
	bk_err_t ret = BK_ERR_AUD_INTF_STA;

	/* check value range */
	if (value > 0x3F)
		return BK_ERR_AUD_INTF_PARAM;

	CHECK_AUD_INTF_BUSY_STA();

	uint8_t temp = 0;

	/*check mic status */
	if (aud_intf_info.mic_status != AUD_INTF_MIC_STA_NULL || aud_intf_info.voc_status != AUD_INTF_VOC_STA_NULL) {
		if (aud_intf_info.mic_status != AUD_INTF_MIC_STA_NULL) {
			temp = aud_intf_info.mic_info.mic_gain;
			aud_intf_info.mic_info.mic_gain = value;
			ret = mailbox_media_aud_send_msg(EVENT_AUD_MIC_SET_GAIN_REQ, &aud_intf_info.mic_info.mic_gain);
			if (ret != BK_OK)
				aud_intf_info.mic_info.mic_gain = temp;
		}

		if (aud_intf_info.voc_status != AUD_INTF_VOC_STA_NULL) {
			temp = aud_intf_info.voc_info.aud_setup.adc_gain;
			aud_intf_info.voc_info.aud_setup.adc_gain = value;
			ret = mailbox_media_aud_send_msg(EVENT_AUD_VOC_SET_MIC_GAIN_REQ, &aud_intf_info.voc_info.aud_setup.adc_gain);
			if (ret != BK_OK)
				aud_intf_info.voc_info.aud_setup.adc_gain = temp;
		}
	}

	aud_intf_info.api_info.busy_status = false;
	return ret;
}

bk_err_t bk_aud_intf_set_mic_samp_rate(aud_adc_samp_rate_t samp_rate)
{
	bk_err_t ret = BK_ERR_AUD_INTF_STA;

	CHECK_AUD_INTF_BUSY_STA();

	/*check mic status */
	if (aud_intf_info.mic_status != AUD_INTF_MIC_STA_NULL) {
		aud_adc_samp_rate_t temp = aud_intf_info.mic_info.samp_rate;
		aud_intf_info.mic_info.samp_rate = samp_rate;
		ret = mailbox_media_aud_send_msg(EVENT_AUD_MIC_SET_SAMP_RATE_REQ, &aud_intf_info.mic_info.samp_rate);
		if (ret != BK_OK)
			aud_intf_info.voc_info.aud_setup.adc_gain = temp;
	}

	aud_intf_info.api_info.busy_status = false;
	return ret;
}

bk_err_t bk_aud_intf_set_mic_chl(aud_intf_mic_chl_t mic_chl)
{
	bk_err_t ret = BK_ERR_AUD_INTF_STA;

	CHECK_AUD_INTF_BUSY_STA();

	/*check mic status */
	if (aud_intf_info.mic_status != AUD_INTF_MIC_STA_NULL) {
		aud_intf_mic_chl_t temp = aud_intf_info.mic_info.mic_chl;
		aud_intf_info.mic_info.mic_chl = mic_chl;
		ret = mailbox_media_aud_send_msg(EVENT_AUD_MIC_SET_CHL_REQ, &aud_intf_info.mic_info.mic_chl);
		if (ret != BK_OK)
			aud_intf_info.mic_info.mic_chl = temp;
	}

	aud_intf_info.api_info.busy_status = false;
	return ret;
}

bk_err_t bk_aud_intf_set_spk_gain(uint32_t value)
{
	bk_err_t ret = BK_ERR_AUD_INTF_STA;

	CHECK_AUD_INTF_BUSY_STA();
	uint16_t temp = 0;

	/*check mic status */
	if (aud_intf_info.spk_status != AUD_INTF_SPK_STA_NULL || aud_intf_info.voc_status != AUD_INTF_VOC_STA_NULL) {
		if (aud_intf_info.spk_status != AUD_INTF_SPK_STA_NULL) {
			/* check value range */
			if (aud_intf_info.spk_info.spk_type == AUD_INTF_SPK_TYPE_BOARD) {
				if (value > 0x3F) {
					LOGE("the spk_gain out of range:0x00-0x3F \n");
					aud_intf_info.api_info.busy_status = false;
					return BK_ERR_AUD_INTF_PARAM;
				}
			} else {
				if (value > 100) {
					LOGE("the spk_gain out of range:0-100 \n");
					aud_intf_info.api_info.busy_status = false;
					return BK_ERR_AUD_INTF_PARAM;
				}
			}
			temp = aud_intf_info.spk_info.spk_gain;
			aud_intf_info.spk_info.spk_gain = value;
			ret = mailbox_media_aud_send_msg(EVENT_AUD_SPK_SET_GAIN_REQ, &aud_intf_info.spk_info.spk_gain);
			if (ret != BK_OK)
				aud_intf_info.spk_info.spk_gain = temp;
		}

		if (aud_intf_info.voc_status != AUD_INTF_VOC_STA_NULL) {
			/* check value range */
			if (aud_intf_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_BOARD) {
				if (value > 0x3F) {
					LOGE("the spk_gain out of range:0x00-0x3F \n");
					aud_intf_info.api_info.busy_status = false;
					return BK_ERR_AUD_INTF_PARAM;
				}
			} else {
					if (value > 100) {
						LOGE("the spk_gain out of range:0-100 \n");
						aud_intf_info.api_info.busy_status = false;
						return BK_ERR_AUD_INTF_PARAM;
					}
			}
			temp = aud_intf_info.voc_info.aud_setup.dac_gain;
			aud_intf_info.voc_info.aud_setup.dac_gain = value;
			LOGI("set spk_gain: %d \n", aud_intf_info.voc_info.aud_setup.dac_gain);
			//return aud_tras_drv_send_msg(AUD_TRAS_DRV_VOC_SET_SPK_GAIN, &aud_intf_info.voc_info.aud_setup.dac_gain);
			//return aud_intf_send_msg_with_sem(AUD_TRAS_DRV_VOC_SET_SPK_GAIN, &aud_intf_info.voc_info.aud_setup.dac_gain, BEKEN_WAIT_FOREVER);
			ret = mailbox_media_aud_send_msg(EVENT_AUD_VOC_SET_SPK_GAIN_REQ, &aud_intf_info.voc_info.aud_setup.dac_gain);
			if (ret != BK_OK)
				aud_intf_info.voc_info.aud_setup.dac_gain = temp;
		}
	}

	aud_intf_info.api_info.busy_status = false;
	return ret;
}

bk_err_t bk_aud_intf_set_spk_samp_rate(aud_dac_samp_rate_t samp_rate)
{
	bk_err_t ret = BK_ERR_AUD_INTF_STA;

	CHECK_AUD_INTF_BUSY_STA();

	/*check spk status */
	if (aud_intf_info.spk_status != AUD_INTF_SPK_STA_NULL) {
		aud_dac_samp_rate_t temp = aud_intf_info.spk_info.samp_rate;
		aud_intf_info.spk_info.samp_rate = samp_rate;
		//return aud_intf_send_msg_with_sem(AUD_TRAS_DRV_SPK_SET_SAMP_RATE, &aud_intf_info.spk_info.samp_rate, BEKEN_WAIT_FOREVER);
		ret = mailbox_media_aud_send_msg(EVENT_AUD_SPK_SET_SAMP_RATE_REQ, &aud_intf_info.voc_info.aud_setup.dac_gain);
		if (ret != BK_OK)
			aud_intf_info.spk_info.samp_rate = temp;
	}

	aud_intf_info.api_info.busy_status = false;
	return ret;
}

bk_err_t bk_aud_intf_set_spk_chl(aud_intf_spk_chl_t spk_chl)
{
	bk_err_t ret = BK_ERR_AUD_INTF_STA;

	CHECK_AUD_INTF_BUSY_STA();

	/*check spk status */
	if (aud_intf_info.spk_status != AUD_INTF_SPK_STA_NULL) {
		aud_intf_spk_chl_t temp = aud_intf_info.spk_info.spk_chl;
		aud_intf_info.spk_info.spk_chl = spk_chl;
		ret = mailbox_media_aud_send_msg(EVENT_AUD_SPK_SET_CHL_REQ, &aud_intf_info.spk_info.spk_chl);
		if (ret != BK_OK)
			aud_intf_info.spk_info.spk_chl = temp;
	}

	aud_intf_info.api_info.busy_status = false;
	return ret;
}

bk_err_t bk_aud_intf_register_uac_connect_state_cb(void *cb)
{
	bk_err_t ret = BK_ERR_AUD_INTF_STA;

	/* check if mic, speaker or voc init */
	if (aud_intf_info.mic_status == AUD_INTF_MIC_STA_NULL && aud_intf_info.spk_status == AUD_INTF_SPK_STA_NULL && aud_intf_info.voc_status == AUD_INTF_VOC_STA_NULL) {
		return BK_ERR_AUD_INTF_STA;
	}

	CHECK_AUD_INTF_BUSY_STA();

	/* check uac mic and speaker type */
	if (((aud_intf_info.mic_status != AUD_INTF_MIC_STA_NULL) && (aud_intf_info.mic_info.mic_type == AUD_INTF_MIC_TYPE_UAC))
			|| ((aud_intf_info.spk_status != AUD_INTF_SPK_STA_NULL) && (aud_intf_info.spk_info.spk_type == AUD_INTF_SPK_TYPE_UAC))
			|| ((aud_intf_info.voc_status != AUD_INTF_VOC_STA_NULL) && ((aud_intf_info.voc_info.mic_type == AUD_INTF_MIC_TYPE_UAC) || (aud_intf_info.voc_info.spk_type == AUD_INTF_SPK_TYPE_UAC)))) {
		ret = mailbox_media_aud_send_msg(EVENT_AUD_UAC_REGIS_CONT_STATE_CB_REQ, cb);
		if (ret == BK_OK) {
			aud_intf_info.aud_intf_uac_connect_state_cb = cb;
		}
	}

	aud_intf_info.api_info.busy_status = false;
	return ret;
}

bk_err_t bk_aud_intf_uac_auto_connect_ctrl(bool enable)
{
	bk_err_t ret = BK_ERR_AUD_INTF_STA;

	if (aud_intf_info.uac_auto_connect == enable) {
		return BK_ERR_AUD_INTF_OK;
	}

	/* check if aud_intf driver init */
	if (aud_intf_info.drv_status != AUD_INTF_DRV_STA_IDLE && aud_intf_info.drv_status != AUD_INTF_DRV_STA_WORK) {
		return BK_ERR_AUD_INTF_STA;
	}

	CHECK_AUD_INTF_BUSY_STA();
	bool temp = aud_intf_info.uac_auto_connect;
	aud_intf_info.uac_auto_connect = enable;
	ret = mailbox_media_aud_send_msg(EVENT_AUD_UAC_AUTO_CONT_CTRL_REQ, &aud_intf_info.uac_auto_connect);
	if (ret != BK_OK) {
		aud_intf_info.uac_auto_connect = temp;
	}

	return ret;
}

bk_err_t bk_aud_intf_set_aec_para(aud_intf_voc_aec_para_t aec_para, uint32_t value)
{
	bk_err_t ret = BK_OK;
//	bk_err_t err = BK_ERR_AUD_INTF_FAIL;
	aud_intf_voc_aec_ctl_t *aec_ctl = NULL;

	/*check aec status */
	if (aud_intf_info.voc_status == AUD_INTF_VOC_STA_NULL)
		return BK_ERR_AUD_INTF_STA;

	CHECK_AUD_INTF_BUSY_STA();

	aec_ctl = os_malloc(sizeof(aud_intf_voc_aec_ctl_t));
	if (aec_ctl == NULL) {
		aud_intf_info.api_info.busy_status = false;
		return BK_ERR_AUD_INTF_MEMY;
	}
	aec_ctl->op = aec_para;
	aec_ctl->value = value;

	//TODO
	//cpy value before set

	switch (aec_para) {
		case AUD_INTF_VOC_AEC_MIC_DELAY:
			aud_intf_info.voc_info.aec_setup->mic_delay = value;
			break;

		case AUD_INTF_VOC_AEC_EC_DEPTH:
			aud_intf_info.voc_info.aec_setup->ec_depth = value;
			break;

		case AUD_INTF_VOC_AEC_REF_SCALE:
			aud_intf_info.voc_info.aec_setup->ref_scale = value;
			break;

		case AUD_INTF_VOC_AEC_VOICE_VOL:
			aud_intf_info.voc_info.aec_setup->voice_vol = value;
			break;

		case AUD_INTF_VOC_AEC_TXRX_THR:
			aud_intf_info.voc_info.aec_setup->TxRxThr = value;
			break;

		case AUD_INTF_VOC_AEC_TXRX_FLR:
			aud_intf_info.voc_info.aec_setup->TxRxFlr = value;
			break;

		case AUD_INTF_VOC_AEC_NS_LEVEL:
			aud_intf_info.voc_info.aec_setup->ns_level = value;
			break;

		case AUD_INTF_VOC_AEC_NS_PARA:
			aud_intf_info.voc_info.aec_setup->ns_para = value;
			break;

		case AUD_INTF_VOC_AEC_DRC:
			aud_intf_info.voc_info.aec_setup->drc = value;
			break;

		case AUD_INTF_VOC_AEC_INIT_FLAG:
			aud_intf_info.voc_info.aec_setup->init_flags = value;
			break;

		default:
			break;
	}

	ret = mailbox_media_aud_send_msg(EVENT_AUD_VOC_SET_AEC_PARA_REQ, aec_ctl);

	os_free(aec_ctl);
	aud_intf_info.api_info.busy_status = false;
	return ret;
}

bk_err_t bk_aud_intf_get_aec_para(void)
{
	/*check aec status */
	if (aud_intf_info.voc_status == AUD_INTF_VOC_STA_NULL)
		return BK_ERR_AUD_INTF_STA;

	CHECK_AUD_INTF_BUSY_STA();
	return mailbox_media_aud_send_msg(EVENT_AUD_VOC_SET_AEC_PARA_REQ, NULL);
}

/*
static void aud_tras_drv_mic_event_cb_handle(aud_tras_drv_mic_event_t event, bk_err_t result)
{
	switch (event) {
		case EVENT_AUD_TRAS_MIC_INIT:
			LOGD("init aud mic complete \r\n");
			if (result == BK_ERR_AUD_INTF_OK) {
				aud_intf_info.mic_status = AUD_INTF_MIC_STA_IDLE;
			}
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_MIC_START:
			LOGD("start aud mic complete \r\n");
			if (result == BK_ERR_AUD_INTF_OK) {
				aud_intf_info.mic_status = AUD_INTF_MIC_STA_START;
			}
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_MIC_PAUSE:
			LOGD("pause aud mic complete \r\n");
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_MIC_STOP:
			LOGD("stop aud mic complete \r\n");
			if (result == BK_ERR_AUD_INTF_OK) {
				aud_intf_info.mic_status = AUD_INTF_MIC_STA_IDLE;
			}
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_MIC_DEINIT:
			LOGD("deinit aud mic complete \r\n");
			if (result == BK_ERR_AUD_INTF_OK) {
				aud_intf_info.mic_status = AUD_INTF_MIC_STA_NULL;
			}
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_MIC_SET_SAMP_RATE:
			LOGD("set aud mic samp rate complete \r\n");
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_MIC_SET_CHL:
			LOGD("set aud mic chl complete \r\n");
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		default:
			break;
	}
}
*/

bk_err_t bk_aud_intf_mic_init(aud_intf_mic_setup_t *setup)
{
	bk_err_t ret = BK_OK;
	CHECK_AUD_INTF_BUSY_STA();

	aud_intf_info.mic_info.mic_chl = setup->mic_chl;
	aud_intf_info.mic_info.samp_rate = setup->samp_rate;
	aud_intf_info.mic_info.frame_size = setup->frame_size;
	aud_intf_info.mic_info.mic_gain = setup->mic_gain;
	aud_intf_info.mic_info.mic_type = setup->mic_type;
//	aud_intf_info.mic_info.aud_tras_drv_mic_event_cb = aud_tras_drv_mic_event_cb_handle;

//TODO
	ret = mailbox_media_aud_send_msg(EVENT_AUD_MIC_INIT_REQ, &aud_intf_info.mic_info);
	if (ret == BK_OK)
		aud_intf_info.mic_status = AUD_INTF_MIC_STA_IDLE;
	return ret;
}

bk_err_t bk_aud_intf_mic_deinit(void)
{
	bk_err_t ret = BK_OK;
	CHECK_AUD_INTF_BUSY_STA();
	ret = mailbox_media_aud_send_msg(EVENT_AUD_MIC_DEINIT_REQ, NULL);
	if (ret == BK_OK)
		aud_intf_info.mic_status = AUD_INTF_MIC_STA_NULL;
	return ret;
}

bk_err_t bk_aud_intf_mic_start(void)
{
	bk_err_t ret = BK_OK;
	CHECK_AUD_INTF_BUSY_STA();
	ret = mailbox_media_aud_send_msg(EVENT_AUD_MIC_START_REQ, NULL);
	if (ret == BK_OK)
		aud_intf_info.mic_status = AUD_INTF_MIC_STA_START;
	return ret;
}

bk_err_t bk_aud_intf_mic_pause(void)
{
	bk_err_t ret = BK_OK;
	CHECK_AUD_INTF_BUSY_STA();
	ret = mailbox_media_aud_send_msg(EVENT_AUD_MIC_PAUSE_REQ, NULL);
	if (ret == BK_OK)
		aud_intf_info.mic_status = AUD_INTF_MIC_STA_STOP;
	return ret;
}

bk_err_t bk_aud_intf_mic_stop(void)
{
	bk_err_t ret = BK_OK;
	CHECK_AUD_INTF_BUSY_STA();
	ret = mailbox_media_aud_send_msg(EVENT_AUD_MIC_STOP_REQ, NULL);
	if (ret == BK_OK)
		aud_intf_info.mic_status = AUD_INTF_MIC_STA_STOP;
	return ret;
}

static void aud_intf_spk_deconfig(void)
{
	aud_intf_info.spk_info.spk_chl = AUD_INTF_SPK_CHL_LEFT;
	aud_intf_info.spk_info.samp_rate = AUD_DAC_SAMP_RATE_8K;
	aud_intf_info.spk_info.frame_size = 0;
	aud_intf_info.spk_info.spk_gain = 0;
	ring_buffer_clear(aud_intf_info.spk_info.spk_rx_rb);
	if (aud_intf_info.spk_info.spk_rx_ring_buff) {
		os_free(aud_intf_info.spk_info.spk_rx_ring_buff);
		aud_intf_info.spk_info.spk_rx_ring_buff = NULL;
	}

	if (aud_intf_info.spk_info.spk_rx_rb) {
		os_free(aud_intf_info.spk_info.spk_rx_rb);
		aud_intf_info.spk_info.spk_rx_rb = NULL;
	}
}

/*
static void aud_tras_drv_spk_event_cb_handle(aud_tras_drv_spk_event_t event, bk_err_t result)
{
	switch (event) {
		case EVENT_AUD_TRAS_SPK_INIT:
			LOGD("init aud spk complete \r\n");
			if (result == BK_ERR_AUD_INTF_OK) {
				aud_intf_info.spk_status = AUD_INTF_SPK_STA_IDLE;
			}
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_SPK_START:
			LOGD("start aud spk complete \r\n");
			if (result == BK_ERR_AUD_INTF_OK) {
				aud_intf_info.spk_status = AUD_INTF_SPK_STA_START;
			}
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_SPK_PAUSE:
			LOGD("pause aud spk complete \r\n");
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_SPK_STOP:
			LOGD("stop aud spk complete \r\n");
			if (result == BK_ERR_AUD_INTF_OK) {
				aud_intf_info.spk_status = AUD_INTF_SPK_STA_IDLE;
			}
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_SPK_DEINIT:
			LOGD("deinit aud spk complete \r\n");
			aud_intf_spk_deconfig();
			if (result == BK_ERR_AUD_INTF_OK) {
				aud_intf_info.spk_status = AUD_INTF_SPK_STA_NULL;
			}
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_SPK_SET_SAMP_RATE:
			LOGD("set aud spk samp rate complete \r\n");
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_SPK_SET_CHL:
			LOGD("set aud spk chl complete \r\n");
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		default:
			break;
	}
}
*/

bk_err_t bk_aud_intf_spk_init(aud_intf_spk_setup_t *setup)
{
	bk_err_t ret = BK_ERR_AUD_INTF_OK;
	bk_err_t err = BK_ERR_AUD_INTF_FAIL;
	//os_printf("bk_aud_intf_spk_init \r\n");
	CHECK_AUD_INTF_BUSY_STA();

	aud_intf_info.spk_info.spk_chl = setup->spk_chl;
	aud_intf_info.spk_info.samp_rate = setup->samp_rate;
	aud_intf_info.spk_info.frame_size = setup->frame_size;
	aud_intf_info.spk_info.spk_gain = setup->spk_gain;
	aud_intf_info.spk_info.work_mode = setup->work_mode;
	aud_intf_info.spk_info.spk_type = setup->spk_type;
	aud_intf_info.spk_info.fifo_frame_num = 4;			//default: 4
//	aud_intf_info.spk_info.aud_tras_drv_spk_event_cb = aud_tras_drv_spk_event_cb_handle;

	aud_intf_info.spk_info.spk_rx_ring_buff = os_malloc(aud_intf_info.spk_info.frame_size * aud_intf_info.spk_info.fifo_frame_num + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL);
	if (aud_intf_info.spk_info.spk_rx_ring_buff == NULL) {
		LOGE("malloc spk_rx_ring_buff fail \r\n");
		err = BK_ERR_AUD_INTF_MEMY;
		goto aud_intf_spk_init_exit;
	}

	aud_intf_info.spk_info.spk_rx_rb = os_malloc(sizeof(RingBufferContext));
	if (aud_intf_info.spk_info.spk_rx_rb == NULL) {
		LOGE("malloc spk_rx_rb fail \r\n");
		err = BK_ERR_AUD_INTF_MEMY;
		goto aud_intf_spk_init_exit;
	}

	ring_buffer_init(aud_intf_info.spk_info.spk_rx_rb, (uint8_t *)aud_intf_info.spk_info.spk_rx_ring_buff, aud_intf_info.spk_info.frame_size * aud_intf_info.spk_info.fifo_frame_num + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL, DMA_ID_MAX, RB_DMA_TYPE_NULL);
	//ret = aud_intf_send_msg_with_sem(AUD_TRAS_DRV_SPK_INIT, &aud_intf_info.spk_info, BEKEN_WAIT_FOREVER);
	ret = mailbox_media_aud_send_msg(EVENT_AUD_SPK_INIT_REQ, &aud_intf_info.spk_info);
	if (ret != BK_ERR_AUD_INTF_OK) {
		err = ret;
		goto aud_intf_spk_init_exit;
	}

	aud_intf_info.spk_status = AUD_INTF_SPK_STA_IDLE;

	return BK_ERR_AUD_INTF_OK;

aud_intf_spk_init_exit:
	if (aud_intf_info.spk_info.spk_rx_ring_buff != NULL)
		os_free(aud_intf_info.spk_info.spk_rx_ring_buff);
	if (aud_intf_info.spk_info.spk_rx_rb != NULL)
		os_free(aud_intf_info.spk_info.spk_rx_rb);
//	aud_intf_info.spk_info.aud_tras_drv_spk_event_cb = NULL;
	aud_intf_info.api_info.busy_status = false;
	return err;
}

bk_err_t bk_aud_intf_spk_deinit(void)
{
	bk_err_t ret = BK_OK;
	CHECK_AUD_INTF_BUSY_STA();
	ret = mailbox_media_aud_send_msg(EVENT_AUD_SPK_DEINIT_REQ, NULL);
	if (ret == BK_OK)
		aud_intf_info.spk_status = AUD_INTF_SPK_STA_NULL;
	return ret;
}

bk_err_t bk_aud_intf_spk_start(void)
{
	bk_err_t ret = BK_OK;
	CHECK_AUD_INTF_BUSY_STA();
	ret = mailbox_media_aud_send_msg(EVENT_AUD_SPK_START_REQ, NULL);
	if (ret == BK_OK)
		aud_intf_info.spk_status = AUD_INTF_SPK_STA_START;
	return ret;
}

bk_err_t bk_aud_intf_spk_pause(void)
{
	bk_err_t ret = BK_OK;
	CHECK_AUD_INTF_BUSY_STA();
	ret = mailbox_media_aud_send_msg(EVENT_AUD_SPK_PAUSE_REQ, NULL);
	if (ret == BK_OK)
		aud_intf_info.spk_status = AUD_INTF_SPK_STA_STOP;
	return ret;
}

bk_err_t bk_aud_intf_spk_stop(void)
{
	bk_err_t ret = BK_OK;
	CHECK_AUD_INTF_BUSY_STA();
	ret = mailbox_media_aud_send_msg(EVENT_AUD_SPK_STOP_REQ, NULL);
	if (ret == BK_OK)
		aud_intf_info.spk_status = AUD_INTF_SPK_STA_STOP;
	return ret;
}

static void aud_intf_voc_deconfig(void)
{
	/* audio deconfig */
	aud_intf_info.voc_info.samp_rate = AUD_INTF_VOC_SAMP_RATE_8K;	//default value
	aud_intf_info.voc_info.aud_setup.adc_gain = 0;
	aud_intf_info.voc_info.aud_setup.dac_gain = 0;
	aud_intf_info.voc_info.aud_setup.mic_frame_number = 0;
	aud_intf_info.voc_info.aud_setup.mic_samp_rate_points = 0;
	aud_intf_info.voc_info.aud_setup.speaker_frame_number = 0;
	aud_intf_info.voc_info.aud_setup.speaker_samp_rate_points = 0;

	/* aec deconfig */
	if (aud_intf_info.voc_info.aec_enable && aud_intf_info.voc_info.aec_setup) {
		os_free(aud_intf_info.voc_info.aec_setup);
	}
	aud_intf_info.voc_info.aec_setup = NULL;
	aud_intf_info.voc_info.aec_enable = false;

	/* tx deconfig */
	aud_intf_info.voc_info.tx_info.buff_length = 0;
	aud_intf_info.voc_info.tx_info.tx_buff_status = false;
	aud_intf_info.voc_info.tx_info.ping.busy_status = false;
	if (aud_intf_info.voc_info.tx_info.ping.buff_addr) {
		os_free(aud_intf_info.voc_info.tx_info.ping.buff_addr);
		aud_intf_info.voc_info.tx_info.ping.buff_addr = NULL;
	}

	aud_intf_info.voc_info.tx_info.pang.busy_status = false;
	if (aud_intf_info.voc_info.tx_info.pang.buff_addr) {
		os_free(aud_intf_info.voc_info.tx_info.pang.buff_addr);
		aud_intf_info.voc_info.tx_info.pang.buff_addr = NULL;
	}

	/* rx deconfig */
	aud_intf_info.voc_info.rx_info.rx_buff_status = false;
	if (aud_intf_info.voc_info.rx_info.decoder_ring_buff) {
		os_free(aud_intf_info.voc_info.rx_info.decoder_ring_buff);
		aud_intf_info.voc_info.rx_info.decoder_ring_buff = NULL;
	}

	if (aud_intf_info.voc_info.rx_info.decoder_rb) {
		os_free(aud_intf_info.voc_info.rx_info.decoder_rb);
		aud_intf_info.voc_info.rx_info.decoder_rb = NULL;
	}

	aud_intf_info.voc_info.rx_info.frame_num = 0;
	aud_intf_info.voc_info.rx_info.frame_size = 0;
	aud_intf_info.voc_info.rx_info.fifo_frame_num = 0;
	aud_intf_info.voc_info.rx_info.rx_buff_seq_tail = 0;
	aud_intf_info.voc_info.rx_info.aud_trs_read_seq = 0;

	/* callback deconfig */
//	aud_intf_info.voc_info.aud_tras_drv_voc_event_cb = NULL;
}

/*
static void aud_tras_drv_voc_event_cb_handle(aud_tras_drv_voc_event_t event, bk_err_t result)
{
	switch (event) {
		case EVENT_AUD_TRAS_VOC_INIT:
			LOGD("init voc complete \r\n");
			if (result == BK_ERR_AUD_INTF_OK) {
				aud_intf_info.voc_status = AUD_INTF_VOC_STA_IDLE;
			}
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_VOC_START:
			LOGD("start voc transfer complete \r\n");
			if (result == BK_ERR_AUD_INTF_OK) {
				aud_intf_info.voc_status = AUD_INTF_VOC_STA_START;
			}
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_VOC_STOP:
			LOGD("stop voc transfer complete \r\n");
			if (result == BK_ERR_AUD_INTF_OK) {
				aud_intf_info.voc_status = AUD_INTF_VOC_STA_STOP;
			}
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_VOC_CTRL_SPK:
			LOGD("ctrl voc spk complete \r\n");
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_VOC_CTRL_MIC:
			LOGD("ctrl voc mic complete \r\n");
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_VOC_CTRL_AEC:
			LOGD("ctrl voc aec complete \r\n");
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_VOC_DEINIT:
			LOGD("deinit voc transfer complete \r\n");
			aud_intf_voc_deconfig();
			if (result == BK_ERR_AUD_INTF_OK) {
				aud_intf_info.voc_status = AUD_INTF_VOC_STA_NULL;
			}
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_VOC_SET_AEC_PARA:
			LOGD("set voc aec param complete \r\n");
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_VOC_GET_AEC_PARA:
			LOGD("get voc aec param complete \r\n");
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_VOC_TX_DEBUG:
			LOGD("set voc tx debug complete \r\n");
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_VOC_RX_DEBUG:
			LOGD("set voc rx debug complete \r\n");
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		case EVENT_AUD_TRAS_VOC_AEC_DEBUG:
			LOGD("set voc aec debug complete \r\n");
			aud_intf_info.api_info.result = result;
			rtos_set_semaphore(&aud_intf_info.api_info.sem);
			break;

		default:
			break;
	}
}
*/

bk_err_t bk_aud_intf_voc_init(aud_intf_voc_setup_t setup)
{
	bk_err_t ret = BK_OK;
	bk_err_t err = BK_ERR_AUD_INTF_FAIL;
	CHECK_AUD_INTF_BUSY_STA();

//	aud_intf_info.api_info.busy_status = true;

	//aud_tras_drv_setup.aud_trs_mode = demo_setup.mode;
	aud_intf_info.voc_info.samp_rate = setup.samp_rate;
	aud_intf_info.voc_info.aec_enable = setup.aec_enable;
	aud_intf_info.voc_info.data_type = setup.data_type;
	/* audio config */
	aud_intf_info.voc_info.aud_setup.adc_gain = setup.mic_gain;	//default: 0x2d
	aud_intf_info.voc_info.aud_setup.dac_gain = setup.spk_gain;	//default: 0x2d
	if (aud_intf_info.voc_info.samp_rate == AUD_INTF_VOC_SAMP_RATE_16K) {
		aud_intf_info.voc_info.aud_setup.mic_samp_rate_points = 320;	//if AEC enable , the value is equal to aec_samp_rate_points, and the value not need to set
		aud_intf_info.voc_info.aud_setup.speaker_samp_rate_points = 320;	//if AEC enable , the value is equal to aec_samp_rate_points, and the value not need to set
	} else {
		aud_intf_info.voc_info.aud_setup.mic_samp_rate_points = 160;	//if AEC enable , the value is equal to aec_samp_rate_points, and the value not need to set
		aud_intf_info.voc_info.aud_setup.speaker_samp_rate_points = 160;	//if AEC enable , the value is equal to aec_samp_rate_points, and the value not need to set
	}
	aud_intf_info.voc_info.aud_setup.mic_frame_number = 2;
	aud_intf_info.voc_info.aud_setup.speaker_frame_number = 2;
	aud_intf_info.voc_info.aud_setup.spk_mode = setup.spk_mode;
	aud_intf_info.voc_info.mic_en = setup.mic_en;
	aud_intf_info.voc_info.spk_en = setup.spk_en;
	aud_intf_info.voc_info.mic_type = setup.mic_type;
	aud_intf_info.voc_info.spk_type = setup.spk_type;

	/* aec config */
	if (aud_intf_info.voc_info.aec_enable) {
		aud_intf_info.voc_info.aec_setup = os_malloc(sizeof(aec_config_t));
		if (aud_intf_info.voc_info.aec_setup == NULL) {
			LOGE("malloc aec_setup fail \r\n");
			err = BK_ERR_AUD_INTF_MEMY;
			goto aud_intf_voc_init_exit;
		}
		aud_intf_info.voc_info.aec_setup->init_flags = 0x1f;
		aud_intf_info.voc_info.aec_setup->mic_delay = 0;
		aud_intf_info.voc_info.aec_setup->ec_depth = setup.aec_cfg.ec_depth;
		aud_intf_info.voc_info.aec_setup->ref_scale = setup.aec_cfg.ref_scale;
		aud_intf_info.voc_info.aec_setup->TxRxThr = setup.aec_cfg.TxRxThr;
		aud_intf_info.voc_info.aec_setup->TxRxFlr = setup.aec_cfg.TxRxFlr;
		aud_intf_info.voc_info.aec_setup->voice_vol = 14;
		aud_intf_info.voc_info.aec_setup->ns_level = setup.aec_cfg.ns_level;
		aud_intf_info.voc_info.aec_setup->ns_para = setup.aec_cfg.ns_para;
		aud_intf_info.voc_info.aec_setup->drc = 15;
	} else {
		aud_intf_info.voc_info.aec_setup = NULL;
	}

	/* tx config */
	switch (aud_intf_info.voc_info.data_type) {
		case AUD_INTF_VOC_DATA_TYPE_G711A:
		case AUD_INTF_VOC_DATA_TYPE_G711U:
			aud_intf_info.voc_info.tx_info.buff_length = aud_intf_info.voc_info.aud_setup.mic_samp_rate_points;
			break;

		case AUD_INTF_VOC_DATA_TYPE_PCM:
			aud_intf_info.voc_info.tx_info.buff_length = aud_intf_info.voc_info.aud_setup.mic_samp_rate_points * 2;
			break;

		default:
			break;
	}
	aud_intf_info.voc_info.tx_info.ping.busy_status = false;
	aud_intf_info.voc_info.tx_info.ping.buff_addr = os_malloc(aud_intf_info.voc_info.tx_info.buff_length);
	if (aud_intf_info.voc_info.tx_info.ping.buff_addr == NULL) {
		LOGE("malloc pingpang buffer of tx fail \r\n");
		err = BK_ERR_AUD_INTF_MEMY;
		goto aud_intf_voc_init_exit;
	}
	aud_intf_info.voc_info.tx_info.pang.busy_status = false;
	aud_intf_info.voc_info.tx_info.pang.buff_addr = os_malloc(aud_intf_info.voc_info.tx_info.buff_length);
	if (aud_intf_info.voc_info.tx_info.pang.buff_addr == NULL) {
		LOGE("malloc pang buffer of tx fail \r\n");
		err = BK_ERR_AUD_INTF_MEMY;
		goto aud_intf_voc_init_exit;
	}
	aud_intf_info.voc_info.tx_info.tx_buff_status = true;

	/* rx config */
	aud_intf_info.voc_info.rx_info.aud_trs_read_seq = 0;
	switch (aud_intf_info.voc_info.data_type) {
		case AUD_INTF_VOC_DATA_TYPE_G711A:
		case AUD_INTF_VOC_DATA_TYPE_G711U:
			aud_intf_info.voc_info.rx_info.frame_size = 320;		//apk receive one frame 40ms
			//aud_intf_info.voc_info.rx_info.frame_size = aud_intf_info.voc_info.aud_setup.mic_samp_rate_points;
			break;

		case AUD_INTF_VOC_DATA_TYPE_PCM:
			aud_intf_info.voc_info.rx_info.frame_size = 320 * 2;		//apk receive one frame 40ms
			//aud_intf_info.voc_info.rx_info.frame_size = aud_intf_info.voc_info.aud_setup.mic_samp_rate_points * 2;
			break;

		default:
			break;
	}
	aud_intf_info.voc_info.rx_info.frame_num = setup.frame_num;
	aud_intf_info.voc_info.rx_info.rx_buff_seq_tail = 0;
	aud_intf_info.voc_info.rx_info.fifo_frame_num = setup.fifo_frame_num;
	aud_intf_info.voc_info.rx_info.decoder_ring_buff = os_malloc(aud_intf_info.voc_info.rx_info.frame_size * aud_intf_info.voc_info.rx_info.frame_num + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL);
	if (aud_intf_info.voc_info.rx_info.decoder_ring_buff == NULL) {
		LOGE("malloc decoder ring buffer of rx fail \r\n");
		err = BK_ERR_AUD_INTF_MEMY;
		goto aud_intf_voc_init_exit;
	}
	LOGI("malloc decoder_ring_buff:%p, size:%d \r\n", aud_intf_info.voc_info.rx_info.decoder_ring_buff, aud_intf_info.voc_info.rx_info.frame_size * aud_intf_info.voc_info.rx_info.frame_num);
	aud_intf_info.voc_info.rx_info.decoder_rb = os_malloc(sizeof(RingBufferContext));
	if (aud_intf_info.voc_info.rx_info.decoder_rb == NULL) {
		LOGE("malloc decoder_rb fail \r\n");
		err = BK_ERR_AUD_INTF_MEMY;
		goto aud_intf_voc_init_exit;
	}
	ring_buffer_init(aud_intf_info.voc_info.rx_info.decoder_rb, (uint8_t *)aud_intf_info.voc_info.rx_info.decoder_ring_buff, aud_intf_info.voc_info.rx_info.frame_size * aud_intf_info.voc_info.rx_info.frame_num + CONFIG_AUD_RING_BUFF_SAFE_INTERVAL, DMA_ID_MAX, RB_DMA_TYPE_NULL);
	aud_intf_info.voc_info.rx_info.rx_buff_status = true;

	LOGI("decoder_rb:%p \r\n",aud_intf_info.voc_info.rx_info.decoder_rb);

	/* callback config */
//	aud_intf_info.voc_info.aud_tras_drv_voc_event_cb = aud_tras_drv_voc_event_cb_handle;

//	CHECK_AUD_INTF_BUSY_STA();
	ret = mailbox_media_aud_send_msg(EVENT_AUD_VOC_INIT_REQ, &aud_intf_info.voc_info);
	if (ret != BK_OK) {
		err = BK_ERR_AUD_INTF_TX_MSG;
		goto aud_intf_voc_init_exit;
	}

	if (media_aud_mailbox_msg->result != BK_ERR_AUD_INTF_OK) {
		err = media_aud_mailbox_msg->result;
		LOGE("%s fail, err:%d \r\n", __func__, err);
		goto aud_intf_voc_init_exit;
	}

	aud_intf_info.voc_status = AUD_INTF_VOC_STA_IDLE;

	return BK_ERR_AUD_INTF_OK;

aud_intf_voc_init_exit:
	if (aud_intf_info.voc_info.aec_setup != NULL) {
		os_free(aud_intf_info.voc_info.aec_setup);
		aud_intf_info.voc_info.aec_setup = NULL;
	}

	if (aud_intf_info.voc_info.tx_info.ping.buff_addr != NULL) {
		os_free(aud_intf_info.voc_info.tx_info.ping.buff_addr);
		aud_intf_info.voc_info.tx_info.ping.buff_addr = NULL;
	}

	if (aud_intf_info.voc_info.tx_info.pang.buff_addr != NULL) {
		os_free(aud_intf_info.voc_info.tx_info.pang.buff_addr);
		aud_intf_info.voc_info.tx_info.pang.buff_addr = NULL;
	}

	if (aud_intf_info.voc_info.rx_info.decoder_ring_buff != NULL) {
		os_free(aud_intf_info.voc_info.rx_info.decoder_ring_buff);
		aud_intf_info.voc_info.rx_info.decoder_ring_buff = NULL;
	}

	if (aud_intf_info.voc_info.rx_info.decoder_rb != NULL) {
		os_free(aud_intf_info.voc_info.rx_info.decoder_rb);
		aud_intf_info.voc_info.rx_info.decoder_rb = NULL;
	}

//	aud_intf_info.voc_info.aud_tras_drv_voc_event_cb = NULL;

//	aud_intf_info.api_info.busy_status = false;

	return err;
}

bk_err_t bk_aud_intf_voc_deinit(void)
{
	bk_err_t ret = BK_OK;

	if (aud_intf_info.voc_status == AUD_INTF_VOC_STA_NULL) {
		LOGI("voice is alreay deinit \r\n");
		return BK_ERR_AUD_INTF_OK;
	}

	CHECK_AUD_INTF_BUSY_STA();
	ret = mailbox_media_aud_send_msg(EVENT_AUD_VOC_DEINIT_REQ, NULL);
	if (ret != BK_OK) {
		LOGE("%s fail, result: %d \r\n", ret);
	} else {
		aud_intf_voc_deconfig();
		aud_intf_info.voc_status = AUD_INTF_VOC_STA_NULL;
	}

	return ret;
}

bk_err_t bk_aud_intf_voc_start(void)
{
	bk_err_t ret = BK_OK;

	switch (aud_intf_info.voc_status) {
		case AUD_INTF_VOC_STA_NULL:
		case AUD_INTF_VOC_STA_START:
			return BK_ERR_AUD_INTF_OK;

		case AUD_INTF_VOC_STA_IDLE:
		case AUD_INTF_VOC_STA_STOP:
			if (ring_buffer_get_fill_size(aud_intf_info.voc_info.rx_info.decoder_rb)/aud_intf_info.voc_info.rx_info.frame_size < aud_intf_info.voc_info.rx_info.fifo_frame_num) {
				uint8_t *temp_buff = NULL;
				uint32_t temp_size = aud_intf_info.voc_info.rx_info.frame_size * aud_intf_info.voc_info.rx_info.fifo_frame_num - ring_buffer_get_fill_size(aud_intf_info.voc_info.rx_info.decoder_rb);
				temp_buff = os_malloc(temp_size);
				if (temp_buff == NULL) {
					return BK_ERR_AUD_INTF_MEMY;
				} else {
					switch (aud_intf_info.voc_info.data_type) {
						case AUD_INTF_VOC_DATA_TYPE_G711A:
							os_memset(temp_buff, 0xD5, temp_size);
							break;

						case AUD_INTF_VOC_DATA_TYPE_PCM:
							os_memset(temp_buff, 0x00, temp_size);
							break;

						case AUD_INTF_VOC_DATA_TYPE_G711U:
							os_memset(temp_buff, 0xFF, temp_size);
							break;

						default:
							break;
					}

					aud_intf_voc_write_spk_data(temp_buff, temp_size);
					os_free(temp_buff);
				}
			}
			break;

		default:
			return BK_ERR_AUD_INTF_FAIL;
	}

	CHECK_AUD_INTF_BUSY_STA();
	ret = mailbox_media_aud_send_msg(EVENT_AUD_VOC_START_REQ, NULL);
	if (ret != BK_OK) {
		LOGE("%s fail, result: %d \r\n", ret);
	} else {
		aud_intf_info.voc_status = AUD_INTF_VOC_STA_START;
	}

	return ret;
}

bk_err_t bk_aud_intf_voc_stop(void)
{
	bk_err_t ret = BK_OK;

	switch (aud_intf_info.voc_status) {
		case AUD_INTF_VOC_STA_NULL:
		case AUD_INTF_VOC_STA_IDLE:
		case AUD_INTF_VOC_STA_STOP:
			return BK_ERR_AUD_INTF_STA;
			break;

		case AUD_INTF_VOC_STA_START:
			break;

		default:
			return BK_ERR_AUD_INTF_FAIL;
	}

	CHECK_AUD_INTF_BUSY_STA();
	ret = mailbox_media_aud_send_msg(EVENT_AUD_VOC_STOP_REQ, NULL);
	if (ret != BK_OK) {
		LOGE("%s fail, result: %d \r\n", ret);
	} else {
		aud_intf_info.voc_status = AUD_INTF_VOC_STA_STOP;
	}

	return ret;
}

bk_err_t bk_aud_intf_voc_mic_ctrl(aud_intf_voc_mic_ctrl_t mic_en)
{
	bk_err_t ret = BK_OK;
	if (aud_intf_info.voc_info.mic_en == mic_en) {
		return BK_OK;
	}

	aud_intf_voc_mic_ctrl_t temp = aud_intf_info.voc_info.mic_en;
	aud_intf_info.voc_info.mic_en = mic_en;

	CHECK_AUD_INTF_BUSY_STA();
	ret = mailbox_media_aud_send_msg(EVENT_AUD_VOC_CTRL_MIC_REQ, &aud_intf_info.voc_info.mic_en);
	if (ret != BK_OK)
		aud_intf_info.voc_info.mic_en = temp;

	return ret;
}

bk_err_t bk_aud_intf_voc_spk_ctrl(aud_intf_voc_spk_ctrl_t spk_en)
{
	bk_err_t ret = BK_OK;
	if (aud_intf_info.voc_info.spk_en == spk_en) {
		return BK_OK;
	}

	aud_intf_voc_spk_ctrl_t temp = aud_intf_info.voc_info.spk_en;
	aud_intf_info.voc_info.spk_en = spk_en;

	CHECK_AUD_INTF_BUSY_STA();
	ret = mailbox_media_aud_send_msg(EVENT_AUD_VOC_CTRL_SPK_REQ, &aud_intf_info.voc_info.spk_en);
	if (ret != BK_OK)
		aud_intf_info.voc_info.spk_en = temp;

	return ret;
}

bk_err_t bk_aud_intf_voc_aec_ctrl(bool aec_en)
{
	bk_err_t ret = BK_OK;
	if (aud_intf_info.voc_info.aec_enable == aec_en) {
		return BK_OK;
	}

	bool temp = aud_intf_info.voc_info.aec_enable;
	aud_intf_info.voc_info.aec_enable = aec_en;

	CHECK_AUD_INTF_BUSY_STA();
	ret = mailbox_media_aud_send_msg(EVENT_AUD_VOC_CTRL_AEC_REQ, &aud_intf_info.voc_info.aec_enable);
	if (ret != BK_OK)
		aud_intf_info.voc_info.aec_enable = temp;

	return ret;
}

bk_err_t bk_aud_intf_drv_init(aud_intf_drv_setup_t *setup)
{
	bk_err_t ret = BK_OK;
	bk_err_t err = BK_ERR_AUD_INTF_FAIL;

	/* init semaphore used to  */
	if (!mailbox_media_app_aud_sem) {
		ret = rtos_init_semaphore(&mailbox_media_app_aud_sem, 1);

		if (ret != BK_OK)
		{
			LOGE("%s, create mailbox app test semaphore failed\n");
			goto aud_intf_drv_init_exit;
		}
	}

	if (!media_aud_mailbox_msg) {
		media_aud_mailbox_msg = os_malloc(sizeof(media_mailbox_msg_t));
		if (!media_aud_mailbox_msg) {
			LOGE("%s, malloc media_aud_mailbox_msg fail\n", __func__);
			err = BK_ERR_NO_MEM;
			goto aud_intf_drv_init_exit;
		}
	}

	if (aud_intf_info.drv_status != AUD_INTF_DRV_STA_NULL) {
		LOGI("aud_intf driver already init \r\n");
		return BK_ERR_AUD_INTF_OK;
	}

	/* save drv_info */
	aud_intf_info.drv_info.setup = *setup;
//	aud_intf_info.drv_info.aud_tras_drv_com_event_cb = aud_tras_drv_com_event_cb_handle;

	CHECK_AUD_INTF_BUSY_STA();

	/* init audio interface driver */
	LOGI("init aud_intf driver in CPU1 mode \r\n");
	ret = mailbox_media_aud_send_msg(EVENT_AUD_INIT_REQ, &aud_intf_info.drv_info);
	if (ret != BK_OK) {
		LOGE("init aud_intf driver fail \r\n");
		goto aud_intf_drv_init_exit;
	} else {
		aud_intf_info.drv_status = AUD_INTF_DRV_STA_IDLE;
	}

	return BK_ERR_AUD_INTF_OK;

aud_intf_drv_init_exit:
	if (mailbox_media_app_aud_sem) {
		rtos_deinit_semaphore(&mailbox_media_app_aud_sem);
		mailbox_media_app_aud_sem = NULL;
	}

	if (media_aud_mailbox_msg) {
		os_free(media_aud_mailbox_msg);
		media_aud_mailbox_msg = NULL;
	}

	LOGE("init aud_intf driver fail \r\n");

	return err;
}

bk_err_t bk_aud_intf_drv_deinit(void)
{
	bk_err_t ret = BK_OK;

	if (aud_intf_info.drv_status == AUD_INTF_DRV_STA_NULL) {
		LOGI("aud_intf already deinit \r\n");
		return BK_OK;
	}

	/* reset uac_auto_connect */
	aud_intf_info.uac_auto_connect = true;

	CHECK_AUD_INTF_BUSY_STA();

	ret = mailbox_media_aud_send_msg(EVENT_AUD_DEINIT_REQ, NULL);
	if (ret != BK_OK) {
		LOGE("deinit audio transfer fail \r\n");
		return BK_ERR_AUD_INTF_FAIL;
	}

	/* deinit semaphore used to  */
	if (mailbox_media_app_aud_sem) {
		rtos_deinit_semaphore(&mailbox_media_app_aud_sem);
		mailbox_media_app_aud_sem = NULL;
	}

	aud_intf_info.drv_status = AUD_INTF_DRV_STA_NULL;

	return BK_ERR_AUD_INTF_OK;
}

/* write speaker data in voice work mode */
static bk_err_t aud_intf_voc_write_spk_data(uint8_t *dac_buff, uint32_t size)
{
	uint32_t write_size = 0;

//	LOGI("enter: %s \r\n", __func__);

	/* check aud_intf status */
	if (aud_intf_info.voc_status == AUD_INTF_VOC_STA_NULL)
		return BK_ERR_AUD_INTF_STA;
#if 1
	if (ring_buffer_get_free_size(aud_intf_info.voc_info.rx_info.decoder_rb) >= size) {
		write_size = ring_buffer_write(aud_intf_info.voc_info.rx_info.decoder_rb, dac_buff, size);
		//write_size = ring_buffer_write(aud_intf_info.voc_info.rx_info.decoder_rb, (uint8_t *)PCM_8000, size);
		if (write_size != size) {
			LOGE("write decoder_ring_buff fail, size:%d \r\n", size);
			return BK_FAIL;
		}
		aud_intf_info.voc_info.rx_info.rx_buff_seq_tail += size/(aud_intf_info.voc_info.rx_info.frame_size);
	}
#endif
	return BK_OK;
}

/* write speaker data in general work mode */
static bk_err_t aud_intf_genl_write_spk_data(uint8_t *dac_buff, uint32_t size)
{
	uint32_t write_size = 0;

	//LOGI("enter: %s \r\n", __func__);

	if (aud_intf_info.spk_status == AUD_INTF_SPK_STA_START || aud_intf_info.spk_status == AUD_INTF_SPK_STA_IDLE) {
		if (ring_buffer_get_free_size(aud_intf_info.spk_info.spk_rx_rb) >= size) {
			write_size = ring_buffer_write(aud_intf_info.spk_info.spk_rx_rb, dac_buff, size);
			if (write_size != size) {
				LOGE("write spk_rx_ring_buff fail, write_size:%d \r\n", write_size);
				return BK_FAIL;
			}
		}
	}

	return BK_OK;
}

bk_err_t bk_aud_intf_write_spk_data(uint8_t *dac_buff, uint32_t size)
{
	bk_err_t ret = BK_OK;

	//LOGI("enter: %s \r\n", __func__);

	switch (aud_intf_info.drv_info.setup.work_mode) {
		case AUD_INTF_WORK_MODE_GENERAL:
			ret = aud_intf_genl_write_spk_data(dac_buff, size);
			break;

		case AUD_INTF_WORK_MODE_VOICE:
			ret = aud_intf_voc_write_spk_data(dac_buff, size);
			break;

		default:
			ret = BK_FAIL;
			break;
	}

	return ret;
}


bk_err_t bk_aud_intf_voc_tx_debug(aud_intf_dump_data_callback dump_callback)
{
	if (aud_intf_info.voc_status == AUD_INTF_VOC_STA_NULL)
		return BK_ERR_AUD_INTF_STA;

	CHECK_AUD_INTF_BUSY_STA();
	return mailbox_media_aud_send_msg(EVENT_AUD_VOC_TX_DEBUG_REQ, dump_callback);
}

bk_err_t bk_aud_intf_voc_rx_debug(aud_intf_dump_data_callback dump_callback)
{
	if (aud_intf_info.voc_status == AUD_INTF_VOC_STA_NULL)
		return BK_ERR_AUD_INTF_STA;

	CHECK_AUD_INTF_BUSY_STA();
	return mailbox_media_aud_send_msg(EVENT_AUD_VOC_RX_DEBUG_REQ, dump_callback);
}

bk_err_t bk_aud_intf_voc_aec_debug(aud_intf_dump_data_callback dump_callback)
{
	if (aud_intf_info.voc_status == AUD_INTF_VOC_STA_NULL)
		return BK_ERR_AUD_INTF_STA;

	CHECK_AUD_INTF_BUSY_STA();
	return mailbox_media_aud_send_msg(EVENT_AUD_VOC_AEC_DEBUG_REQ, dump_callback);
}

