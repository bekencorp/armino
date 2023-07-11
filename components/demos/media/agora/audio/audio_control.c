
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <components/system.h>
#include <os/os.h>
#include <os/mem.h>

#include <components/aud_intf.h>
#include <components/aud_intf_types.h>
#include <modules/audio_ring_buff.h>

#include "amf_ringbuf.h"
#include "BK7256_RegList.h"

#define TAG "agora_audio"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define RTSA_CODEC    1

static aud_intf_drv_setup_t aud_intf_drv_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
static aud_intf_voc_setup_t aud_intf_voc_setup = DEFAULT_AUD_INTF_VOC_SETUP_CONFIG();
static aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;

#if AUD_TASK
static beken_thread_t  agora_aud_thread_hdl = NULL;
static beken_queue_t agora_aud_msg_que = NULL;
static RingBufferContext mic_data_rb;
static uint8_t *mic_data_buffer = NULL;
#define MIC_FRAME_SIZE 320
#define MIC_FRAME_NUM 2
#endif

extern int send_agora_audio_frame(uint8_t *data, uint32_t len);

#if AUD_TASK
static bk_err_t agora_aud_send_msg(void)
{
	bk_err_t ret;

	if (agora_aud_msg_que) {
		ret = rtos_push_to_queue(&agora_aud_msg_que, NULL, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			os_printf("es8311_send_int_msg fail \r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}
#endif

static int send_audio_data_to_agora(uint8_t *data, unsigned int len)
{
#if AUD_TASK
	if (ring_buffer_get_free_size(&mic_data_rb) >= len) {
//GLOBAL_INT_DECLARATION();
		ring_buffer_write(&mic_data_rb, data, len);
//GLOBAL_INT_RESTORE();
	}
	

	if (ring_buffer_get_fill_size(&mic_data_rb) >= MIC_FRAME_SIZE) {
		//bk_i2s_stop();
		addAON_GPIO_Reg0x3 = 2;
		agora_aud_send_msg();
		addAON_GPIO_Reg0x3 = 0;
	}
#else
	send_agora_audio_frame(data, len);
#endif
	return len;
}

#if AUD_TASK
static void agora_aud_main(void)
{
	bk_err_t ret = BK_OK;
	GLOBAL_INT_DECLARATION();
	uint32_t size = 0;
	uint32_t count = 0;

	uint8_t *mic_temp_buff = NULL;

	while(1) {
		ret = rtos_pop_from_queue(&agora_aud_msg_que, NULL, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			//addAON_GPIO_Reg0x8 = 2;
			size = ring_buffer_get_fill_size(&mic_data_rb);
			if (size) {
				mic_temp_buff = os_malloc(size);
				if (mic_temp_buff != NULL) {
					GLOBAL_INT_DISABLE();
					count = ring_buffer_read(&mic_data_rb, mic_temp_buff, size);
					GLOBAL_INT_RESTORE();
					if (count)
						send_agora_audio_frame(mic_temp_buff, count);
					os_free(mic_temp_buff);
				}
			}
			//addAON_GPIO_Reg0x8 = 0;

		}
	}

	os_free(mic_temp_buff);
}
#endif

void audio_init(void)
{
	bk_err_t ret = BK_OK;

	aud_intf_drv_setup.aud_intf_tx_mic_data = send_audio_data_to_agora;
	ret = bk_aud_intf_drv_init(&aud_intf_drv_setup);
	if (ret != BK_ERR_AUD_INTF_OK) {
		LOGE("bk_aud_intf_drv_init fail, ret:%d \r\n", ret);
	} else {
		LOGI("bk_aud_intf_drv_init complete \r\n");
	}

	aud_work_mode = AUD_INTF_WORK_MODE_VOICE;
	ret = bk_aud_intf_set_mode(aud_work_mode);
	if (ret != BK_ERR_AUD_INTF_OK) {
		LOGE("bk_aud_intf_set_mode fail, ret:%d \r\n", ret);
	} else {
		LOGI("bk_aud_intf_set_mode complete \r\n");
	}

#if AUD_TASK
	mic_data_buffer = os_malloc(MIC_FRAME_SIZE * MIC_FRAME_NUM);
	if (mic_data_buffer == NULL) {
		os_printf("malloc mic_data_buffer fail \n");
		return;
	}
	ring_buffer_init(&mic_data_rb, mic_data_buffer, MIC_FRAME_SIZE*MIC_FRAME_NUM, DMA_ID_MAX, RB_DMA_TYPE_NULL);

	ret = rtos_init_queue(&agora_aud_msg_que,
						  "aud_int_que",
						  0,
						  80);
	if (ret != kNoErr) {
		os_printf("ceate audio asr message queue fail \r\n");
		return;
	}
	os_printf("ceate audio asr message queue complete \r\n");

	/* create task to asr */
	ret = rtos_create_thread(&agora_aud_thread_hdl,
						 3,
						 "agora_aud",
						 (beken_thread_function_t)agora_aud_main,
						 4096,
						 NULL);
	if (ret != kNoErr) {
		os_printf("create audio transfer driver task fail \r\n");
		rtos_deinit_queue(&agora_aud_msg_que);
		agora_aud_msg_que= NULL;
		agora_aud_thread_hdl = NULL;
	}
	os_printf("create audio transfer driver task complete \r\n");
#endif
}

void audio_deinit(void)
{
	bk_err_t ret = BK_OK;

	aud_work_mode = AUD_INTF_WORK_MODE_NULL;
	bk_aud_intf_set_mode(aud_work_mode);

	ret = bk_aud_intf_drv_deinit();
	if (ret != BK_ERR_AUD_INTF_OK) {
		LOGE("bk_aud_intf_drv_deinit fail, ret:%d \r\n", ret);
	} else {
		LOGI("bk_aud_intf_drv_deinit complete \r\n");
	}
}

void voice_init(uint8_t type, aud_intf_voc_samp_rate_t samp_rate, bool aec_en)
{
	bk_err_t ret = BK_OK;

#if RTSA_CODEC
	aud_intf_voc_setup.data_type  = AUD_INTF_VOC_DATA_TYPE_PCM;
#else
	aud_intf_voc_setup.data_type  = AUD_INTF_VOC_DATA_TYPE_G711U;
#endif
	aud_intf_voc_setup.spk_mode   = AUD_DAC_WORK_MODE_SIGNAL_END;
	aud_intf_voc_setup.aec_enable = aec_en;
	aud_intf_voc_setup.samp_rate = samp_rate;
	if (type == 1) {
		aud_intf_voc_setup.mic_type = AUD_INTF_MIC_TYPE_UAC;
		aud_intf_voc_setup.spk_type = AUD_INTF_MIC_TYPE_UAC;
	} else {
		aud_intf_voc_setup.mic_type = AUD_INTF_MIC_TYPE_BOARD;
		aud_intf_voc_setup.spk_type = AUD_INTF_MIC_TYPE_BOARD;
	}

	bk_aud_intf_voc_init(aud_intf_voc_setup);
	if (ret != BK_ERR_AUD_INTF_OK) {
		LOGE("bk_aud_intf_voc_init fail, ret:%d \r\n", ret);
	} else {
		LOGI("bk_aud_intf_voc_init complete \r\n");
	}
}

void voice_start(void)
{
	bk_err_t ret = BK_OK;

	ret = bk_aud_intf_voc_start();
	if (ret != BK_ERR_AUD_INTF_OK) {
		LOGE("bk_aud_intf_voc_start fail, ret:%d \r\n", ret);
	} else {
		LOGI("bk_aud_intf_voc_start complete \r\n");
	}
}

void voice_stop(void)
{
	bk_err_t ret = BK_OK;

	ret = bk_aud_intf_voc_stop();
	if (ret != BK_ERR_AUD_INTF_OK) {
		LOGE("bk_aud_intf_voc_stop fail, ret:%d \r\n", ret);
	} else {
		LOGI("bk_aud_intf_voc_stop complete \r\n");
	}
}

void voice_deinit(void)
{
	bk_err_t ret = BK_OK;

	ret = bk_aud_intf_voc_deinit();
	if (ret != BK_ERR_AUD_INTF_OK) {
		LOGE("bk_aud_intf_voc_deinit fail, ret:%d \r\n", ret);
	} else {
		LOGI("bk_aud_intf_voc_deinit complete \r\n");
	}
}

void voice_spk_ctrl(bool enable)
{
	bk_err_t ret = BK_OK;
	aud_intf_voc_spk_ctrl_t spk_en = AUD_INTF_VOC_SPK_MAX;

	if (enable) {
		spk_en = AUD_INTF_VOC_SPK_OPEN;
	} else {
		spk_en = AUD_INTF_VOC_SPK_CLOSE;
	}

	ret = bk_aud_intf_voc_spk_ctrl(spk_en);
	if (ret != BK_ERR_AUD_INTF_OK)
		LOGE("ctrl voc spk fail: %d \r\n", spk_en);
	else
		LOGI("ctrl voc spk success: %d \r\n", spk_en);
}

void voice_mic_ctrl(bool enable)
{
	bk_err_t ret = BK_OK;
	aud_intf_voc_mic_ctrl_t mic_en = AUD_INTF_VOC_MIC_MAX;

	if (enable) {
		mic_en = AUD_INTF_VOC_MIC_OPEN;
	} else {
		mic_en = AUD_INTF_VOC_MIC_CLOSE;
	}

	ret = bk_aud_intf_voc_mic_ctrl(mic_en);
	if (ret != BK_ERR_AUD_INTF_OK)
		LOGE("ctrl voc mic fail: %d \r\n", mic_en);
	else
		LOGI("ctrl voc mic success: %d \r\n", mic_en);
}

