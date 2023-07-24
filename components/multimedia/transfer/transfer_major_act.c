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
#include "gpio_map.h"
#include "gpio_driver.h"

#include <driver/media_types.h>

#include <soc/mapping.h>

#include "transfer_act.h"
#include "media_evt.h"

#include "frame_buffer.h"

#include "wlan_ui_pub.h"

#if CONFIG_ARCH_RISCV
#include "cache.h"
#endif

#if CONFIG_VIDEO_AVI
#include "app_jpeg2avi.h"
#include "diskio.h"
#endif

#define TAG "trs_major"

#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

//#define DVP_DIAG_DEBUG

#ifdef TRANSFER_DIAG_DEBUG

#define TRANSFER_DIAG_DEBUG_INIT()                  \
	do {                                            \
		gpio_dev_unmap(GPIO_0);                     \
		bk_gpio_disable_pull(GPIO_0);               \
		bk_gpio_enable_output(GPIO_0);              \
		bk_gpio_set_output_low(GPIO_0);             \
		\
		gpio_dev_unmap(GPIO_1);                     \
		bk_gpio_disable_pull(GPIO_1);               \
		bk_gpio_enable_output(GPIO_1);              \
		bk_gpio_set_output_low(GPIO_1);             \
		\
	} while (0)

#define WIFI_TRANSFER_START()                   bk_gpio_set_output_high(GPIO_0)
#define WIFI_TRANSFER_END()                     bk_gpio_set_output_low(GPIO_0)

#define WIFI_DMA_START()                    bk_gpio_set_output_high(GPIO_1)
#define WIFI_DMA_END()                      bk_gpio_set_output_low(GPIO_1)

#else

#define TRANSFER_DIAG_DEBUG_INIT()

#define WIFI_TRANSFER_START()
#define WIFI_TRANSFER_END()

#define WIFI_DMA_START()
#define WIFI_DMA_END()

#endif

static transfer_info_t transfer_info;
static media_mailbox_msg_t *transfer_major_node = NULL;
static beken_thread_t transfer_major_task = NULL;
static beken_queue_t transfer_major_msg_que = NULL;
static bool transfer_major_task_running = false;
extern media_debug_t *media_debug;

extern u64 riscv_get_mtimer(void);

static bk_err_t transfer_major_task_send_msg(uint8_t type, uint32_t data)
{
	bk_err_t ret;
	trs_task_msg_t msg;

	if (transfer_major_msg_que)
	{
		msg.type = type;
		msg.data = data;

		ret = rtos_push_to_queue(&transfer_major_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret)
		{
			LOGE("transfer_major_msg_que failed, type:%d\r\n", type);
			return kNoResourcesErr;
		}

		return ret;
	}
	return kGeneralErr;
}

static void transfer_major_task_transfer_start(uint32_t param)
{
	LOGI("%s, %d\r\n", __func__, param);

	transfer_info.param = param;
	frame_buffer_fb_register(MODULE_WIFI, transfer_info.param);
	transfer_major_task_send_msg(TRS_TRANSFER_DATA, 0);
}

static void transfer_major_task_transfer_data(uint32_t param)
{
	uint64 before, after;

	frame_buffer_t *encode_frame = NULL;

	transfer_major_task_running = true;

	while (transfer_major_task_running)
	{
		if (transfer_info.param == FB_INDEX_JPEG)
		{
			encode_frame = frame_buffer_fb_read(MODULE_WIFI);
		} else {
			encode_frame = frame_buffer_fb_h264_pop_wait();
		}

		if (encode_frame == NULL)
		{
			LOGE("read frame NULL\n");
			continue;
		}

		media_debug->wifi_read++;
		media_debug->fps_wifi++;

#if (CONFIG_ARCH_RISCV)
		before = riscv_get_mtimer();
#else
		before = 0;
#endif

		// send msg to cpu0
		transfer_major_node->event = EVENT_MEDIA_DATA_NOTIFY;
		transfer_major_node->param = (uint32_t)encode_frame;
		msg_send_req_to_media_major_mailbox_sync(transfer_major_node, APP_MODULE);

		if (transfer_info.param == FB_INDEX_JPEG)
		{
			frame_buffer_fb_free(encode_frame, MODULE_WIFI);
		} else {
			frame_buffer_fb_h264_free(encode_frame);
		}

#if (CONFIG_ARCH_RISCV)
		after = riscv_get_mtimer();
#else
		after = 0;
#endif
		LOGD("transfer time: %lu\n", (after - before) / 26000);
	}
}

static void transfer_major_task_exit_handle(uint32_t param)
{
	media_mailbox_msg_t *msg = (media_mailbox_msg_t *)param;

	if (transfer_major_node)
	{
		if(transfer_major_node->sem)
		{
			rtos_deinit_semaphore(&transfer_major_node->sem);
			transfer_major_node->sem = NULL;
		}

		os_free(transfer_major_node);
		transfer_major_node = NULL;
	}

	frame_buffer_fb_deregister(MODULE_WIFI);

	set_transfer_state(TRS_STATE_DISABLED);

	msg_send_rsp_to_media_major_mailbox(msg, kNoErr, APP_MODULE);
}

static void transfer_major_task_entry(beken_thread_arg_t data)
{
	int ret = kNoErr;

	media_debug->wifi_read = 0;
	media_debug->transfer_timer_us = 50000;

	while (1)
	{
		trs_task_msg_t msg;
		ret = rtos_pop_from_queue(&transfer_major_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret)
		{
			switch (msg.type)
			{
				case TRS_TRANSFER_START:
					transfer_major_task_transfer_start(msg.data);
					break;

				case TRS_TRANSFER_DATA:
					transfer_major_task_transfer_data(msg.data);
					break;

				case TRS_TRANSFER_EXIT:
					transfer_major_task_exit_handle(msg.data);
					goto exit;
					break;

				default:
					break;
			}
		}
	};

exit:

	LOGI("transfer_major_task task exit\n");

	rtos_deinit_queue(&transfer_major_msg_que);
	transfer_major_msg_que = NULL;

	transfer_major_task = NULL;
	rtos_delete_thread(NULL);
}

static bk_err_t transfer_major_task_init(void)
{
	bk_err_t ret = BK_FAIL;

	TRANSFER_DIAG_DEBUG_INIT();

	if (transfer_major_node == NULL)
	{
		transfer_major_node = (media_mailbox_msg_t *)os_malloc(sizeof(media_mailbox_msg_t));
		if (transfer_major_node != NULL)
		{
			ret = rtos_init_semaphore_ex(&transfer_major_node->sem, 1, 0);

			if (ret != kNoErr)
			{
				LOGE("%s init semaphore failed\n", __func__);
				goto error;
			}
		}
	}

	if ((!transfer_major_task) && (!transfer_major_msg_que))
	{
		ret = rtos_init_queue(&transfer_major_msg_que,
								"transfer_major_msg_que",
								sizeof(trs_task_msg_t),
								10);
		if (kNoErr != ret)
		{
			LOGE("transfer_major_msg_que init failed\n");
			ret = kNoMemoryErr;
			goto error;
		}

		ret = rtos_create_thread(&transfer_major_task,
								BEKEN_DEFAULT_WORKER_PRIORITY,
								"transfer_major_task",
								(beken_thread_function_t)transfer_major_task_entry,
								1024,
								NULL);

		if (BK_OK != ret)
		{
			LOGE("transfer_major_task init failed\n");
			ret = kNoMemoryErr;
			goto error;
		}
	}

	return ret;

error:

	if (transfer_major_node)
	{
		if(transfer_major_node->sem)
		{
			rtos_deinit_semaphore(&transfer_major_node->sem);
			transfer_major_node->sem = NULL;
		}

		os_free(transfer_major_node);
		transfer_major_node = NULL;
	}

	if (transfer_major_msg_que)
	{
		rtos_deinit_queue(&transfer_major_msg_que);
		transfer_major_msg_que = NULL;
	}

	if (transfer_major_task)
	{
		transfer_major_task = NULL;
	}

	return ret;
}

static bk_err_t transfer_major_task_open_handle(media_mailbox_msg_t *msg)
{
	int ret = BK_OK;

	if (get_transfer_state() == TRS_STATE_ENABLED)
	{
		LOGI("transfer_major_task have been opened!\r\n");
		goto end;
	}

	ret = transfer_major_task_init();
	if (ret == BK_OK)
	{
		transfer_major_task_send_msg(TRS_TRANSFER_START, msg->param);

		set_transfer_state(TRS_STATE_ENABLED);
	}

end:

	msg_send_rsp_to_media_major_mailbox(msg, ret, APP_MODULE);

	LOGI("%s complete\n", __func__);

	return ret;
}

static bk_err_t transfer_major_task_close_handle(media_mailbox_msg_t *msg)
{
	LOGI("%s\n", __func__);

	if (transfer_major_task_running == false || get_transfer_state() == TRS_STATE_DISABLED)
	{
		LOGI("transfer_major_task have closed!\r\n");
		msg_send_rsp_to_media_major_mailbox(msg, BK_OK, APP_MODULE);
	}

	transfer_major_task_running = false;

	transfer_major_task_send_msg(TRS_TRANSFER_EXIT, (uint32_t)msg);

	return BK_OK;
}

static bk_err_t transfer_major_task_pause_handle(media_mailbox_msg_t *msg)
{
	LOGI("%s, %d\n", __func__, msg->param);

	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();

	transfer_info.pause = msg->param;

	GLOBAL_INT_RESTORE();

	msg_send_rsp_to_media_major_mailbox(msg, kNoErr, APP_MODULE);

	return BK_OK;
}

bk_err_t transfer_major_event_handle(media_mailbox_msg_t *msg)
{
	int ret = BK_OK;

	switch (msg->event)
	{
		case EVENT_TRANSFER_OPEN_IND:
			ret = transfer_major_task_open_handle(msg);
			break;
		case EVENT_TRANSFER_CLOSE_IND:
			ret = transfer_major_task_close_handle(msg);
			break;
		case EVENT_TRANSFER_PAUSE_IND:
			ret = transfer_major_task_pause_handle(msg);
			break;

		default:
			break;
	}

	return ret;
}

media_trs_state_t get_transfer_state(void)
{
	return transfer_info.state;
}

void set_transfer_state(media_trs_state_t state)
{
	transfer_info.state = state;
}

void transfer_init(void)
{
	transfer_info.state = TRS_STATE_DISABLED;
	transfer_info.debug = false;
	transfer_info.pause = false;
	transfer_info.param = FB_INDEX_JPEG;
}
