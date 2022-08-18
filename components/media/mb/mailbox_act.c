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
#include "media_evt.h"
#include "mailbox_act.h"
#include "lcd_act.h"

#include <driver/int.h>
#include <os/mem.h>
#include <driver/gpio.h>
#include <driver/gpio_types.h>

#include <driver/dma.h>

#ifdef CONFIG_MASTER_CORE
#define TAG "mb_major"
#endif

#ifdef CONFIG_SLAVE_CORE
#define TAG "mb_minor"
#endif

#include <driver/media_types.h>

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)











#if CONFIG_SLAVE_CORE
void mailbox_cmd_handle(uint32_t event, uint32_t param)
{
	switch (event)
	{
		case EVENT_LCD_DEFAULT_CMD:
			LOGI("EVENT_LCD_DEFAULT_CMD \n");
			media_msg_t msg;
			msg.event = EVENT_LCD_DEFAULT_EVT;
			msg.param = param;
			media_send_msg(&msg);

		case EVENT_LCD_ROTATE_RIGHT_CMD:
			//LOGI("EVENT_LCD_ROTATE_RIGHT_CMD \n");
			lcd_act_rotate_degree90(param);
			break;
	}
}
#endif

#if CONFIG_MASTER_CORE
void mailbox_evt_handle(uint32_t event, uint32_t param)
{
	switch (event)
	{
		case EVENT_LCD_DEFAULT_EVT:
			LOGI("EVENT_LCD_DEFAULT_EVT \n");

			param_pak_t *param_pak = (param_pak_t *)param;


			LOGI("param: %08X\n", param_pak->param);

			MEDIA_EVT_RETURN(param_pak, 0);

			break;

#ifdef CONFIG_LCD
		case EVENT_LCD_ROTATE_RIGHT_COMP_EVT:
			//LOGI("EVENT_LCD_ROTATE_RIGHT_COMP_EVT\n");
			lcd_act_rotate_complete((frame_buffer_t *)param);
			break;
#endif

	}
}
#endif
