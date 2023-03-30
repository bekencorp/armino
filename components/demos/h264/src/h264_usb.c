// Copyright 2022-2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//	 http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <os/os.h>
#include <os/mem.h>
#include "frame_buffer.h"
#include "h264_transfer.h"

#define TAG "usb_transfer"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define MAX_TX_SIZE (1472)
#define MAX_COPY_SIZE (1472 - sizeof(transfer_data_t))
#define MAX_RETRY (10000)
#define RETRANSMITS_TIME (5)

typedef struct {
	uint8_t id;
	uint8_t eof;
	uint8_t cnt;
	uint8_t size;
	uint8_t data[];
} transfer_data_t;

static beken_semaphore_t usb_transfer_sem;
static beken_thread_t usb_transfer_task = NULL;
static transfer_data_t *transfer_data = NULL;
static bool transfer_task_running = false;

extern void usbd_video_open(uint8_t intf);
extern void usbd_video_send_frame(uint8_t *data, uint32_t size);
extern void transfer_memcpy_word_htonl(uint32_t *dst, uint32_t *src, uint32_t size);

static void h264_frame_handle(frame_buffer_t *buffer)
{
	LOGI("seq: %u, length: %u, size: %u\n", buffer->sequence, buffer->length, buffer->size);
	transfer_memcpy_word_htonl((uint32_t *)buffer->frame, (uint32_t *)buffer->frame, buffer->length);
	usbd_video_send_frame(buffer->frame, buffer->length);
	/* send h264 I frame again, otherwise pc decode failed */
	if (buffer->sequence == 1) {
		usbd_video_send_frame(buffer->frame, buffer->length);
	}
}

static void h264_usb_task_entry(beken_thread_arg_t data)
{
	frame_buffer_t *h264_frame = NULL;

	rtos_set_semaphore(&usb_transfer_sem);
	usbd_video_open(0);

	while (1) {
		h264_frame = frame_buffer_fb_h264_pop_wait();
		if (h264_frame == NULL) {
			LOGE("h264_usb_task read h264 frame NULL\n");
			continue;
		}
		h264_frame_handle(h264_frame);
		frame_buffer_fb_h264_free(h264_frame);
	}

	LOGI("usb transfer task exit\n");

	usb_transfer_task = NULL;
	rtos_set_semaphore(&usb_transfer_sem);
	rtos_delete_thread(NULL);
}

bk_err_t demo_h264_usb_init(void)
{
	bk_err_t ret = 0;

	if (transfer_data == NULL) {
		transfer_data = (transfer_data_t *)os_malloc(sizeof(transfer_data_t) + MAX_COPY_SIZE);
		if (transfer_data == NULL) {
			LOGE("%s transfer_data malloc failed\n", __func__);
			return BK_ERR_NO_MEM;
		}
	}

	ret = rtos_init_semaphore_ex(&usb_transfer_sem, 1, 0);
	if (BK_OK != ret) {
		LOGE("%s semaphore init failed\n", __func__);
		return ret;
	}

	transfer_task_running = true;

	ret = rtos_create_thread(&usb_transfer_task,
							4,
							"usb_transfer_task",
							(beken_thread_function_t)h264_usb_task_entry,
							4 * 1024,
							NULL);

	if (BK_OK != ret) {
		LOGE("%s usb_transfer_task init failed\n");
		return ret;
	}

	ret = rtos_get_semaphore(&usb_transfer_sem, BEKEN_NEVER_TIMEOUT);

	if (BK_OK != ret) {
		LOGE("%s usb_transfer_sem get failed\n", __func__);
	}

	return BK_OK;
}
