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

#include "camera_act.h"
#include "media_evt.h"
#include "media_app.h"

#include <os/mem.h>

#include <driver/uvc_camera.h>

#include "frame_buffer.h"

#define TAG "uvc"

#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#ifdef CONFIG_USB_UVC

static bool uvc_state_flag = false;
static uvc_camera_config_t *uvc_camera_config_st = NULL;

void uvc_device_connect_state_callback(uvc_state_t state)
{
	LOGI("%s, %d, %d\n", __func__, state, uvc_state_flag);

	if (state == UVC_DISCONNECT_ABNORMAL)
	{
		if (!uvc_state_flag)
		{
			return;
		}

		uvc_state_flag = false;

		media_msg_t media_msg;

		media_msg.event = EVENT_CAM_UVC_RESET_IND;
		media_msg.param = state;

		media_send_msg(&media_msg);
	}
	else if (state == UVC_CONNECTED)
	{
		if (uvc_state_flag)
			return;

		media_msg_t media_msg;

		media_msg.event = EVENT_CAM_UVC_RESET_IND;
		media_msg.param = state;

		media_send_msg(&media_msg);
	}

}

bk_err_t bk_uvc_camera_open(media_ppi_t ppi, media_camera_type_t type)
{
	int ret = BK_FAIL;

	if (uvc_camera_config_st == NULL)
	{
		uvc_camera_config_st = (uvc_camera_config_t *)os_malloc(sizeof(uvc_camera_config_t));
		if (uvc_camera_config_st == NULL)
		{
			LOGE("uvc_camera_config_st malloc failed\r\n");
			ret = kNoMemoryErr;
			return ret;
		}

		os_memset(uvc_camera_config_st, 0, sizeof(uvc_camera_config_t));

		if (uvc_camera_config_st->uvc_config == NULL)
		{
			uvc_camera_config_st->uvc_config = (uvc_config_t *)os_malloc(sizeof(uvc_config_t));
			if (uvc_camera_config_st->uvc_config == NULL)
			{
				LOGE("uvc_config malloc failed\r\n");
				os_free(uvc_camera_config_st);
				uvc_camera_config_st = NULL;
				ret = kNoMemoryErr;
				return ret;
			}

			os_memset(uvc_camera_config_st->uvc_config, 0, sizeof(uvc_config_t));
		}
	}

	uvc_state_flag = true;

	uvc_camera_config_st->uvc_config->type = type;
	uvc_camera_config_st->uvc_config->device.fps = 20;
	uvc_camera_config_st->uvc_config->device.width = ppi >> 16;
	uvc_camera_config_st->uvc_config->device.height = ppi & 0xFFFF;
	uvc_camera_config_st->uvc_config->connect_state_change_cb = NULL;
	uvc_camera_config_st->uvc_config->uvc_packet_rx_cb = NULL;

	uvc_camera_config_st->fb_jpeg_init = frame_buffer_fb_jpeg_init;
	uvc_camera_config_st->fb_jpeg_deinit = frame_buffer_fb_jpeg_deinit;
	uvc_camera_config_st->fb_jpeg_complete = frame_buffer_fb_push;
	uvc_camera_config_st->fb_jpeg_malloc = frame_buffer_fb_jpeg_malloc;
	uvc_camera_config_st->fb_jpeg_free = frame_buffer_fb_jpeg_free;
	uvc_camera_config_st->uvc_connect_state_change_cb = uvc_device_connect_state_callback;

	ret = bk_uvc_camera_driver_init(uvc_camera_config_st);
	if (ret == BK_OK)
	{
		uvc_state_flag = true;
	}
	else
	{
		LOGE("%s failed\r\n", __func__);
		uvc_state_flag = false;
	}

	LOGI("%s, complete, %d\r\n", __func__, uvc_state_flag);

	return ret;
}

bk_err_t bk_uvc_camera_close(void)
{
	if (uvc_state_flag)
	{
		uvc_state_flag = false;
		bk_uvc_camera_driver_deinit();

		if (uvc_camera_config_st)
		{
			LOGI("%s\r\n", __func__);
			if (uvc_camera_config_st->uvc_config)
			{
				os_free(uvc_camera_config_st->uvc_config);
				uvc_camera_config_st->uvc_config = NULL;
			}

			os_free(uvc_camera_config_st);
			uvc_camera_config_st = NULL;
		}
	}

	return 0;
}

#endif
