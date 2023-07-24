#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <components/system.h>
#include <os/os.h>
#include <os/mem.h>

#include <modules/audio_ring_buff.h>
#include "audio_transfer.h"

#include "BK7256_RegList.h"


#define TAG "agora_audio_tras"
#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

typedef enum {
	AUD_TRAS_TX_DATA = 0,

	AUD_TRAS_EXIT,
} aud_tras_op_t;

typedef struct {
	aud_tras_op_t op;
} aud_tras_msg_t;

static beken_thread_t  agora_aud_thread_hdl = NULL;
static beken_queue_t agora_aud_msg_que = NULL;
static RingBufferContext mic_data_rb;
static uint8_t *mic_data_buffer = NULL;
#define MIC_FRAME_SIZE 160
#define MIC_FRAME_NUM 4


extern int send_agora_audio_frame(uint8_t *data, uint32_t len);

static bk_err_t agora_aud_send_msg(void)
{
	bk_err_t ret;
	aud_tras_msg_t msg;

	msg.op = AUD_TRAS_TX_DATA;

	if (agora_aud_msg_que) {
		ret = rtos_push_to_queue(&agora_aud_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			LOGE("audio send msg: AUD_TRAS_TX_DATA fail \r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}


int send_audio_data_to_agora(uint8_t *data, unsigned int len)
{
	if (ring_buffer_get_free_size(&mic_data_rb) >= len) {
//GLOBAL_INT_DECLARATION();
		ring_buffer_write(&mic_data_rb, data, len);
//GLOBAL_INT_RESTORE();
	} else {
		return 0;
	}

	if (ring_buffer_get_fill_size(&mic_data_rb) >= MIC_FRAME_SIZE) {
//		addAON_GPIO_Reg0x3 = 2;
		agora_aud_send_msg();
//		addAON_GPIO_Reg0x3 = 0;
	}
	return len;
}

static void agora_aud_tras_main(void)
{
	bk_err_t ret = BK_OK;
	GLOBAL_INT_DECLARATION();
	uint32_t size = 0;
	uint32_t count = 0;

	uint8_t *mic_temp_buff = NULL;

	while(1) {
		aud_tras_msg_t msg;

		ret = rtos_pop_from_queue(&agora_aud_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			switch (msg.op) {
				case AUD_TRAS_TX_DATA:
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
					break;

				case AUD_TRAS_EXIT:
					LOGD("goto: AUD_TRAS_EXIT \r\n");
					goto aud_tras_exit;
					break;

				default:
					break;
			}
		}
	}

aud_tras_exit:

	ring_buffer_clear(&mic_data_rb);

	if (mic_data_buffer) {
		os_free(mic_data_buffer);
		mic_data_buffer = NULL;
	}

	/* delete msg queue */
	ret = rtos_deinit_queue(&agora_aud_msg_que);
	if (ret != kNoErr) {
		LOGE("delete message queue fail \r\n");
	}
	agora_aud_msg_que = NULL;
	LOGI("delete message queue complete \r\n");

	/* delete task */
	agora_aud_thread_hdl = NULL;
	rtos_delete_thread(NULL);

}

bk_err_t audio_tras_init(void)
{
	bk_err_t ret = BK_OK;

	mic_data_buffer = os_malloc(MIC_FRAME_SIZE * MIC_FRAME_NUM);
	if (mic_data_buffer == NULL) {
		LOGE("malloc mic_data_buffer fail \n");
		return BK_FAIL;
	}
	ring_buffer_init(&mic_data_rb, mic_data_buffer, MIC_FRAME_SIZE*MIC_FRAME_NUM, DMA_ID_MAX, RB_DMA_TYPE_NULL);

	ret = rtos_init_queue(&agora_aud_msg_que,
						  "aud_int_que",
						  sizeof(aud_tras_msg_t),
						  60);
	if (ret != kNoErr) {
		LOGE("ceate agoar audio tras message queue fail \r\n");
		goto audio_tras_init_fail;
	}
	LOGI("ceate audio asr message queue complete \r\n");

	/* create task to asr */
	ret = rtos_create_thread(&agora_aud_thread_hdl,
						 6,
						 "agora_audio_tras",
						 (beken_thread_function_t)agora_aud_tras_main,
						 2048,
						 NULL);
	if (ret != kNoErr) {
		LOGE("create audio transfer driver task fail \r\n");
		rtos_deinit_queue(&agora_aud_msg_que);
		agora_aud_msg_que= NULL;
		agora_aud_thread_hdl = NULL;
		goto audio_tras_init_fail;
	}
	LOGI("create audio transfer driver task complete \r\n");

	return BK_OK;

audio_tras_init_fail:
	if (mic_data_buffer) {
		ring_buffer_clear(&mic_data_rb);
		os_free(mic_data_buffer);
		mic_data_buffer = NULL;
	}

	return BK_FAIL;
}

bk_err_t audio_tras_deinit(void)
{
	bk_err_t ret;
	aud_tras_msg_t msg;

	msg.op = AUD_TRAS_EXIT;
	if (agora_aud_msg_que) {
		ret = rtos_push_to_queue_front(&agora_aud_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			LOGE("audio send msg: AUD_TRAS_EXIT fail \r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}


