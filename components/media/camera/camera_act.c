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
#include "media_evt.h"
#include "storage_act.h"

#include <driver/int.h>
#include <os/mem.h>
#include <driver/gpio.h>
#include <driver/gpio_types.h>

#include <driver/dma.h>
#include <driver/i2c.h>
#include <driver/jpeg_enc.h>
#include <driver/jpeg_enc_types.h>

#include "camera.h"
#include "frame_buffer.h"


#include <driver/dvp_camera.h>
#include <driver/dvp_camera_types.h>

#include <driver/timer.h>

#define TAG "dvp_act"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define DEBUG_INTERVAL (1000 * 2)

extern bk_err_t pm_core_bus_clock_ctrl(uint32_t cksel_core, uint32_t ckdiv_core, uint32_t ckdiv_bus, uint32_t ckdiv_cpu0, uint32_t ckdiv_cpu1);

extern void transfer_dump(uint32_t ms);
#if CONFIG_LCD
extern uint32_t media_jpg_isr_count;
extern uint32_t media_decoder_isr_count;
extern uint32_t media_lcd_isr_count;
#endif


dvp_info_t dvp_info;

static void dvp_debug_dump(timer_id_t timer_id)
{
	transfer_dump(DEBUG_INTERVAL);
#if CONFIG_LCD
	LOGI("ISR, jpg: %d, decoder: %d, lcd: %d\n", media_jpg_isr_count, media_decoder_isr_count, media_lcd_isr_count);
#endif
}

void dvp_open_handle(param_pak_t *param, dvp_mode_t mode)
{
	int ret = 0;

	LOGI("%s\n", __func__);

	if (DVP_STATE_DISABLED != get_dvp_camera_state())
	{
		LOGI("%s already opened\n", __func__);
		ret = kNoErr;
		goto out;
	}

	//pm_core_bus_clock_ctrl(2, 0, 1, 0, 0);

	frame_buffer_enable(true);

	ret = bk_dvp_camera_open(param->param, mode);


	if (ret != kNoErr)
	{
		LOGE("%s open failed\n", __func__);
		goto out;
	}

	set_dvp_camera_state(DVP_STATE_ENABLED);

	if (dvp_info.debug)
	{
		bk_timer_start(TIMER_ID1, DEBUG_INTERVAL, dvp_debug_dump);
	}

out:
	MEDIA_EVT_RETURN(param, ret);
}

void dvp_close_handle(param_pak_t *param)
{
	int ret = 0;

	LOGI("%s\n", __func__);

	if (DVP_STATE_DISABLED == get_dvp_camera_state())
	{
		LOGI("%s already close\n", __func__);
		ret = kNoErr;
		goto out;
	}

	bk_dvp_camera_close();

	set_dvp_camera_state(DVP_STATE_DISABLED);

	if (dvp_info.debug)
	{
		bk_timer_stop(TIMER_ID1);
	}


	frame_buffer_enable(false);

	//pm_core_bus_clock_ctrl(3, 1, 1, 0, 0);

out:
	MEDIA_EVT_RETURN(param, ret);
}

void dvp_camera_event_handle(uint32_t event, uint32_t param)
{
	switch (event)
	{
		case EVENT_DVP_JPEG_OPEN_IND:
			dvp_open_handle((param_pak_t *)param, DVP_MODE_JPG);
			break;
		case EVENT_DVP_YUV_OPEN_IND:
			dvp_open_handle((param_pak_t *)param, DVP_MODE_YUV);
			break;
		case EVENT_DVP_CLOSE_IND:
			dvp_close_handle((param_pak_t *)param);
			break;
	}
}

dvp_state_t get_dvp_camera_state(void)
{
	return dvp_info.state;
}

void set_dvp_camera_state(dvp_state_t state)
{
	dvp_info.state = state;
}

void dvp_camera_init(void)
{
	dvp_info.state = DVP_STATE_DISABLED;
	dvp_info.debug = true;
}

