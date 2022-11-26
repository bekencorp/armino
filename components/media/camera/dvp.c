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

#include <driver/int.h>
#include <os/mem.h>
#include <driver/gpio.h>
#include <driver/gpio_types.h>

#include <driver/dma.h>
#include <driver/i2c.h>
#include <driver/jpeg_enc.h>
#include <driver/jpeg_enc_types.h>


#include <driver/dvp_camera.h>
#include <driver/dvp_camera_types.h>

#include "frame_buffer.h"

#define TAG "dvp"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

bk_err_t bk_dvp_camera_open(media_ppi_t ppi, dvp_mode_t mode)
{
	dvp_camera_config_t config;

	config.fb_jpeg_init = frame_buffer_fb_jpeg_init;
	config.fb_jpeg_deinit = frame_buffer_fb_jpeg_deinit;
	config.fb_display_init = frame_buffer_fb_display_init;
	config.fb_display_deinit = frame_buffer_fb_display_deinit;
	config.fb_jpeg_complete = frame_buffer_fb_push;
	config.fb_display_complete = frame_buffer_fb_display_push;
	config.fb_jpeg_malloc = frame_buffer_fb_jpeg_malloc;
	config.fb_display_malloc = frame_buffer_fb_display_malloc;
	config.fb_jpeg_free = frame_buffer_fb_jpeg_free;
	config.fb_display_free = frame_buffer_fb_display_free;

	config.ppi = ppi;
	config.mode = mode;

#if (!CONFIG_YUV_BUF)
	if (mode == DVP_MODE_JPG)
	{
		config.mode = DVP_MODE_MIX;
	}
#endif

	return bk_dvp_camera_driver_init(&config);
}

bk_err_t bk_dvp_camera_close(void)
{
	bk_dvp_camera_driver_deinit();
	return 0;
}
