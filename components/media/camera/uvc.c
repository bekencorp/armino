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

#include "media_core.h"
#include "camera_act.h"
#include "lcd_act.h"
#include "storage_act.h"
#include "media_evt.h"

#include <driver/int.h>
#include <os/mem.h>

#include <driver/dma.h>

#include <driver/uvc_camera.h>

#include "frame_buffer.h"

#define TAG "uvc"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

uvc_camera_device_t uvc_camera_device =
{
	.width = 640,
	.height = 480,
	.fps = 25,
};

void frame_buffer_uvc_jpg_complete(frame_buffer_t *buffer)
{
	frame_buffer_generate_complete(buffer, FRAME_JPEG);
}

frame_buffer_t *frame_buffer_uvc_jpg_alloc(void)
{
	return frame_buffer_alloc(FRAME_JPEG);
}


const uvc_camera_config_t uvc_camera_config =
{
	.device = &uvc_camera_device,
	.frame_set_ppi = frame_buffer_set_ppi,
	.frame_complete = frame_buffer_uvc_jpg_complete,
	.frame_alloc = frame_buffer_uvc_jpg_alloc,
};


bk_err_t bk_uvc_camera_open(media_ppi_t ppi)
{
	if (ppi != 0)
	{
		uvc_camera_device.width = ppi >> 16;
		uvc_camera_device.height = ppi & 0xFFFF;
	}

	return bk_uvc_camera_driver_init(&uvc_camera_config);
}

bk_err_t bk_uvc_camera_start(void)
{
	return bk_uvc_camera_driver_start();
}

bk_err_t bk_uvc_camera_stop(void)
{
	return bk_uvc_camera_driver_stop();
}

bk_err_t bk_uvc_camera_close(void)
{
	bk_uvc_camera_driver_deinit();
	return 0;
}

bk_err_t bk_uvc_camera_param_set(param_pak_t *param)
{
	return bk_uvc_camera_set_config((uvc_camera_device_t *)param->param);
}
