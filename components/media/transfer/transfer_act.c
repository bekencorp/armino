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
#include <components/log.h>
#include <components/video_transfer.h>

#include "media_core.h"
#include <driver/int.h>
#include <os/mem.h>
#include <driver/gpio.h>
#include <driver/gpio_types.h>

#include <driver/dma.h>

#include <os/mem.h>
#include <driver/gpio.h>
#include <driver/gpio_types.h>

#include <driver/dma.h>
#include <driver/i2c.h>
#include <driver/jpeg_enc.h>
#include <driver/jpeg_enc_types.h>


#include <driver/dvp_camera.h>
#include <driver/dvp_camera_types.h>
#include <driver/media_types.h>

#include <soc/mapping.h>

#include <driver/timer.h>


#include "bk_general_dma.h"

#include "transfer_act.h"
#include "media_evt.h"

#include "frame_buffer.h"

#include "wlan_ui_pub.h"

#define TAG "transfer"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

typedef struct
{
	uint8_t type;
	uint32_t data;
} trs_task_msg_t;

typedef enum
{
	TRS_TRANSFER_DATA,
	TRS_TRANSFER_EXIT,
} trs_task_msg_type_t;

typedef struct
{
	uint8_t id;
	uint8_t eof;
	uint8_t cnt;
	uint8_t size;
	uint8_t data[];
} transfer_data_t;



transfer_info_t transfer_info;


#define MAX_TX_SIZE (1472)
#define MAX_COPY_SIZE (1472 - sizeof(transfer_data_t))
#define MAX_RETRY (10000)
#define RETRANSMITS_TIME (5)

static beken_queue_t trs_task_queue = NULL;
static beken_thread_t trs_task_thread = NULL;



transfer_data_t *wifi_tranfer_data = NULL;

uint8_t frame_id = 0;
bool runing = false;


frame_buffer_t *wifi_tranfer_frame = NULL;


video_setup_t vido_transfer_info = {0};

uint32_t lost_size = 0;
uint32_t complete_size = 0;

extern void rwnxl_set_video_transfer_flag(uint32_t video_transfer_flag);

int dvp_frame_send(uint8_t *data, uint32_t size, uint32_t retry_max, uint32_t ms_time)
{
	int ret = kGeneralErr;

	if (!vido_transfer_info.send_func)
	{
		return ret;
	}

	do
	{
		ret = vido_transfer_info.send_func(data, size);

		if (ret == size)
		{
			//LOGI("size: %d\n", size);
			complete_size += size;
			rtos_delay_milliseconds(3);
			break;
		}

		//LOGI("retry\n");
		lost_size += size;
		rtos_delay_milliseconds(ms_time);
	}
	while (retry_max-- && runing);


	return ret == size ? kNoErr : kGeneralErr;
}

static void dvp_frame_handle(frame_buffer_t *buffer)
{
	uint32_t i;
	uint32_t count = buffer->length / MAX_COPY_SIZE;
	uint32_t tail = buffer->length % MAX_COPY_SIZE;
	uint8_t id = frame_id++;
	int ret;

	LOGD("id: %u, seq: %u, length: %u, size: %u\n", buffer->id, buffer->sequence, buffer->length, buffer->size);
	wifi_tranfer_data->id = id;
	wifi_tranfer_data->size = 0;
	wifi_tranfer_data->eof = 0;
	wifi_tranfer_data->cnt = 0;

	for (i = 0; i < count && runing; i++)
	{
		if ((tail == 0) && (i == count - 1))
		{
			wifi_tranfer_data->eof = 1;
			wifi_tranfer_data->cnt = count;
		}

		dma_memcpy(wifi_tranfer_data->data, buffer->frame + (MAX_COPY_SIZE * i), MAX_COPY_SIZE);

		ret = dvp_frame_send((uint8_t *)wifi_tranfer_data, MAX_TX_SIZE, MAX_RETRY, RETRANSMITS_TIME);

		if (ret != kNoErr)
		{
			LOGE("send failed\n");
		}
	}

	if (tail)
	{
		wifi_tranfer_data->eof = 1;
		wifi_tranfer_data->cnt = count + 1;

		/* fix for psram 4bytes alignment */
		dma_memcpy(wifi_tranfer_data->data, buffer->frame + (MAX_COPY_SIZE * i), (tail % 4) ? ((tail / 4 + 1) * 4) : tail);

		ret = dvp_frame_send((uint8_t *)wifi_tranfer_data, tail + sizeof(transfer_data_t), MAX_RETRY, RETRANSMITS_TIME);

		if (ret != kNoErr)
		{
			LOGE("send failed\n");
		}
	}

	LOGD("seq: %u, length: %u, tail: %u, count: %u\n", id, buffer->length, tail, count);

	if (wifi_tranfer_frame)
	{
		media_msg_t msg;

		msg.event = EVENT_COM_FRAME_WIFI_FREE_IND;
		msg.param = (uint32_t)buffer;
		media_send_msg(&msg);
		wifi_tranfer_frame = NULL;
	}
}

void transfer_dump(uint32_t ms)
{
	uint32_t lost = lost_size, complete = complete_size, speed;
	lost_size = 0;
	complete_size = 0;

	if (transfer_info.state == TRS_STATE_DISABLED)
	{
		return;
	}

	lost = lost / 1024 / (ms / 1000);
	complete = complete / 1024 / (ms / 1000);
	speed = (complete * 8) / 1024;

	LOGI("Lost: %uKB/s, Complete: %uKB/s, Speed: %uMb/s\n", lost, complete, speed);
}


static void trs_task_entry(beken_thread_arg_t data)
{
	bk_err_t ret = BK_OK;
	trs_task_msg_t msg;

	while (1)
	{
		ret = rtos_pop_from_queue(&trs_task_queue, &msg, BEKEN_WAIT_FOREVER);

		if (kNoErr == ret)
		{
			switch (msg.type)
			{
				case TRS_TRANSFER_DATA:
					if (false == frame_buffer_get_state() && runing)
					{
						break;
					}
					dvp_frame_handle((frame_buffer_t *)msg.data);
					break;

				case TRS_TRANSFER_EXIT:
					goto exit;
				default:
					break;
			}
		}
	}

exit:

	if (wifi_tranfer_data != NULL)
	{
		os_free(wifi_tranfer_data);
		wifi_tranfer_data = NULL;
	}

	frame_id = 0;

	rtos_deinit_queue(&trs_task_queue);
	trs_task_queue = NULL;

	trs_task_thread = NULL;
	rtos_delete_thread(NULL);
}

int trs_task_start(video_setup_t *setup_cfg)
{
	int ret;

	os_memcpy(&vido_transfer_info, setup_cfg, sizeof(video_setup_t));

	runing = true;

	if (wifi_tranfer_data == NULL)
	{
		wifi_tranfer_data = (transfer_data_t *) os_malloc(MAX_TX_SIZE);
	}

	frame_id = 0;

	if (trs_task_queue == NULL)
	{
		ret = rtos_init_queue(&trs_task_queue, "trs_task_queue", sizeof(trs_task_msg_t), 60);

		if (kNoErr != ret)
		{
			LOGE("%s trs_task_queue init failed\n");
			goto error;
		}
	}

	if (trs_task_thread == NULL)
	{
		ret = rtos_create_thread(&trs_task_thread,
		                         4,
		                         "trs_task_thread",
		                         (beken_thread_function_t)trs_task_entry,
		                         4 * 1024,
		                         NULL);

		if (kNoErr != ret)
		{
			LOGE("%s trs_task_thread init failed\n");
			goto error;
		}
	}

	return kNoErr;

error:

	if (wifi_tranfer_frame)
	{
		media_msg_t msg;

		msg.event = EVENT_COM_FRAME_WIFI_FREE_IND;
		msg.param = (uint32_t)wifi_tranfer_frame;
		media_send_msg(&msg);
		wifi_tranfer_frame = NULL;
	}

	if (trs_task_queue)
	{
		rtos_deinit_queue(&trs_task_queue);
		trs_task_queue = NULL;
	}

	if (trs_task_thread)
	{
		trs_task_queue = NULL;
		rtos_delete_thread(NULL);
	}

	return kGeneralErr;
}

bk_err_t trs_task_send_msg(uint8_t msg_type, uint32_t data)
{
	bk_err_t ret;
	trs_task_msg_t msg;

	if (trs_task_queue)
	{
		msg.type = msg_type;
		msg.data = data;

		ret = rtos_push_to_queue(&trs_task_queue, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret)
		{
			LOGE("video_transfer_cpu1_send_msg failed\r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}


void trs_task_stop(void)
{
	runing = false;
	trs_task_send_msg(TRS_TRANSFER_EXIT, 0);
	while (trs_task_thread)
	{
		rtos_delay_milliseconds(10);
	}
}

frame_buffer_t *get_wifi_transfer_frame(void)
{
	return wifi_tranfer_frame;
}


void wifi_frame_complete_callback(frame_buffer_t *buffer)
{
	LOGD("%s\n", __func__);
	wifi_tranfer_frame = buffer;
	trs_task_send_msg(TRS_TRANSFER_DATA, (uint32_t)buffer);
}


void video_transfer_open_handle(param_pak_t *param)
{
	video_setup_t *setup_cfg = (video_setup_t *)param->param;

	LOGI("%s ++\n", __func__);

	frame_buffer_frame_register(MODULE_WIFI, wifi_frame_complete_callback);

	trs_task_start(setup_cfg);

	set_trs_video_transfer_state(TRS_STATE_ENABLED);

	MEDIA_EVT_RETURN(param, kNoErr);

	LOGI("%s --\n", __func__);
}

void video_transfer_close_handle(param_pak_t *param)
{
	LOGI("%s\n", __func__);

	wifi_tranfer_frame = NULL;

	trs_task_stop();
	set_trs_video_transfer_state(TRS_STATE_DISABLED);

	frame_buffer_frame_deregister(MODULE_WIFI);

	rwnxl_set_video_transfer_flag(false);

	bk_wlan_ps_enable();

	MEDIA_EVT_RETURN(param, kNoErr);
}


void wifi_transfer_event_handle(uint32_t event, uint32_t param)
{
	switch (event)
	{
		case EVENT_TRS_VIDEO_TRANSFER_OPEN_IND:
			video_transfer_open_handle((param_pak_t *)param);
			break;
		case EVENT_TRS_VIDEO_TRANSFER_CLOSE_IND:
			video_transfer_close_handle((param_pak_t *)param);
			break;
	}
}

trs_state_t get_trs_video_transfer_state(void)
{
	return transfer_info.state;
}

void set_trs_video_transfer_state(trs_state_t state)
{
	transfer_info.state = state;
}

void trs_video_transfer_init(void)
{
	transfer_info.state = TRS_STATE_DISABLED;
	transfer_info.debug = false;
}
