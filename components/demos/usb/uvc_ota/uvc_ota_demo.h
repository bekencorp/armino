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

#pragma once

#include <components/log.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UVC_OTA_TAG "UVC_OTA"
#define UVC_OTA_LOGI(...) BK_LOGI(UVC_OTA_TAG, ##__VA_ARGS__)
#define UVC_OTA_LOGW(...) BK_LOGW(UVC_OTA_TAG, ##__VA_ARGS__)
#define UVC_OTA_LOGE(...) BK_LOGE(UVC_OTA_TAG, ##__VA_ARGS__)
#define UVC_OTA_LOGD(...) BK_LOGD(UVC_OTA_TAG, ##__VA_ARGS__)

#define CW_SendInfo os_printf
typedef unsigned char	UINT8;
typedef unsigned short	 UINT16;
typedef unsigned int   UINT;
typedef enum {
	IPC_STATUS_OK = 0,
	IPC_STATUS_FAIL,
	IPC_STATUS_ARGV,
	IPC_STATUS_BUSY,
	IPC_STATUS_NOT_FIND,
	IPC_STATUS_APP_INVALID,
	IPC_STATUS_EXIST,
	IPC_STATUS_EXCEED,
	IPC_STATUS_MEM_MALLOC_FAIL,
	IPC_STATUS_CONFLICT,
	IPC_STATUS_ENTRY_FULL,
	IPC_STATUS_NOT_CHANGE,

	IPC_STATUS_END
}ipc_status_t;

#define PKT_RESEND_NUMBER 5
#define PKT_MAX_LEN 1000
#define PKT_HEAD 0xFE
#define PKT_HEAD_FLAG_LEN 5
#define PKT_CRC_CHECK_LEN 2
#define PKT_CRC_TABLE_MAX_LEN 256
#define PKT_INDENTIFICATION_DETECTION_LEN 7

#define UVC_TRAMSMIT_DATA_MAX_LEN		64

/* A.7.1 USB Request Privatie Protocol Standard */
#define UVC_CUSTOM_GET_VERSION			0x00
#define UVC_CUSTOM_SET_UPGRADE_FILE_LEN		0x01
#define UVC_CUSTOM_GET_UPGRADE_FILE_LEN		0x02
#define UVC_CUSTOM_SET_UPGRADE_DATA			0x03
#define UVC_CUSTOM_GET_UPGRADE_DATA_NUM		0x04
#define UVC_CUSTOM_GET_UPGRADE_FILE_RESULT	0x05
#define UVC_CUSTOM_GET_UPGRADE_STATUS		0x06

typedef struct {
	uint32_t cmd;
	uint32_t len;
	void *buffer;
} uvc_ota_demo_drv_msg_t;

typedef struct {
	uint32_t recived;
	uint32_t total;
} uvc_ota_demo_http_recived_data_t;

typedef struct uvc_ota_demo_link_list
{
	void *pItem;
	struct uvc_ota_demo_link_list *pNext;
	unsigned long length;
} uvc_ota_demo_list;

#ifdef __cplusplus
}
#endif
