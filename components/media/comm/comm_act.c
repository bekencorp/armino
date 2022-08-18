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
#include "frame_buffer.h"

#include <driver/int.h>
#include <os/mem.h>
#include <driver/gpio.h>
#include <driver/gpio_types.h>

#include <driver/dma.h>


#define TAG "comm"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


void comm_event_handle(uint32_t event, uint32_t param)
{
	switch (event)
	{
		case EVENT_COM_FRAME_WIFI_FREE_IND:
			frame_buffer_free_request((frame_buffer_t *)param, MODULE_WIFI);
			break;
		case EVENT_COM_FRAME_DECODER_FREE_IND:
			frame_buffer_free_request((frame_buffer_t *)param, MODULE_DECODER);
			break;
		case EVENT_COM_FRAME_CAPTURE_FREE_IND:
			frame_buffer_free_request((frame_buffer_t *)param, MODULE_CAPTURE);
			break;
	}
}


