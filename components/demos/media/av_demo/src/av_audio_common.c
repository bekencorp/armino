#include <common/bk_include.h>
#include <components/aud_intf.h>
#include <components/aud_intf_types.h>
#include "av_audio_common.h"


#define TAG "av_audio_common"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

static aud_intf_drv_setup_t aud_intf_drv_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
static aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;
static aud_intf_voc_setup_t aud_voc_setup = DEFAULT_AUD_INTF_VOC_SETUP_CONFIG();

void aud_voc_start(av_aud_voc_setup_t setup)
{
	bk_err_t ret = BK_OK;

	//aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
	//aud_intf_drv_setup.task_config.priority = 3;
	//aud_intf_drv_setup.aud_intf_rx_spk_data = NULL;
	aud_intf_drv_setup.aud_intf_tx_mic_data = setup.av_aud_voc_send_packet;
	ret = bk_aud_intf_drv_init(&aud_intf_drv_setup);
	if (ret != BK_ERR_AUD_INTF_OK)
	{
		LOGE("bk_aud_intf_drv_init fail, ret:%d\n", ret);
	}
	aud_work_mode = AUD_INTF_WORK_MODE_VOICE;
	ret = bk_aud_intf_set_mode(aud_work_mode);
	if (ret != BK_ERR_AUD_INTF_OK)
	{
		LOGE("bk_aud_intf_set_mode fail, ret:%d\n", ret);
	}
	//aud_voc_setup.aec_enable = true;
	aud_voc_setup.samp_rate = AUD_INTF_VOC_SAMP_RATE_8K;
	aud_voc_setup.data_type = AUD_INTF_VOC_DATA_TYPE_G711A;
	aud_voc_setup.spk_mode = AUD_DAC_WORK_MODE_SIGNAL_END;
	//aud_voc_setup.mic_en = AUD_INTF_VOC_MIC_OPEN;
	//aud_voc_setup.spk_en = AUD_INTF_VOC_SPK_OPEN;
	//aud_voc_setup.aec_cfg.ref_scale = 1;
	//aud_voc_setup.mic_gain = 0x2d;
	//aud_voc_setup.spk_gain = 0x2d;
	//aud_voc_setup.aec_cfg.ec_depth = 20;
	//aud_voc_setup.aec_cfg.TxRxThr = 30;
	//aud_voc_setup.aec_cfg.TxRxFlr = 6;
	//aud_voc_setup.aec_cfg.ns_level = 2;
	//aud_voc_setup.aec_cfg.ns_para = 1;

	ret = bk_aud_intf_voc_init(aud_voc_setup);
	if (ret != BK_ERR_AUD_INTF_OK)
	{
		LOGE("bk_aud_intf_voc_init fail, ret:%d\n", ret);
	}
	ret = bk_aud_intf_voc_start();
	if (ret != BK_ERR_AUD_INTF_OK)
	{
		LOGE("bk_aud_intf_voc_start fail, ret:%d\n", ret);
	}

}

void aud_voc_stop(void)
{
	bk_aud_intf_voc_stop();

	bk_aud_intf_voc_deinit();

	bk_aud_intf_drv_deinit();
}

