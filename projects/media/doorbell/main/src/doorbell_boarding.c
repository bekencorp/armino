#include "bk_private/bk_init.h"
#include <components/system.h>
#include <os/os.h>
#include <components/shell_task.h>

#include "cli.h"
#include "doorbell_comm.h"
#include "doorbell_boarding.h"

#include "doorbell_cs2_service.h"
#include "ble_boarding.h"

#include "modules/ble.h"

#include <modules/ble.h>
#include <modules/dm_ble.h>

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define TAG "db-bd"

#define ADV_MAX_SIZE (251)
#define ADV_NAME_HEAD "doorbell"

#define ADV_TYPE_FLAGS                      (0x01)
#define ADV_TYPE_LOCAL_NAME                 (0x09)
#define ADV_TYPE_SERVICE_UUIDS_16BIT        (0x14)
#define ADV_TYPE_SERVICE_DATA               (0x16)
#define ADV_TYPE_MANUFACTURER_SPECIFIC      (0xFF)

#define BEKEN_COMPANY_ID                    (0x05F0)

#define BOARDING_UUID                       (0xFE01)

static ble_boarding_info_t *doorbell_boarding_info = NULL;
static p2p_cs2_key_t *p2p_cs2_key = NULL;
beken_semaphore_t sync_seam = NULL;


void doorbell_boarding_event_notify(uint16_t opcode, int status)
{
	uint8_t data[] =
	{
		opcode & 0xFF, opcode >> 8,     /* opcode           */
		                      status & 0xFF,                                                          /* status           */
		                      0, 0,                                                                   /* payload length   */
	};

	LOGI("%s: %d, %d\n", __func__, opcode, status);
	ble_boarding_notify(data, sizeof(data));
}


void doorbell_boarding_event_message(uint16_t opcode, int status)
{
	doorbell_msg_t msg;

	msg.event = DBEVT_START_BOARDING_EVENT;
	msg.param = status << 16 | opcode;
	doorbell_send_msg(&msg);
}


static void doorbell_boarding_operation_handle(uint16_t opcode, uint16_t length, uint8_t *data)
{
	LOGE("%s, opcode: %04X, length: %u\n", __func__, opcode, length);

	switch (opcode)
	{
		case BOARDING_OP_STATION_START:
		{
			doorbell_msg_t msg;

			msg.event = DBEVT_WIFI_STATION_CONNECT;
			msg.param = (uint32_t)doorbell_boarding_info;
			doorbell_send_msg(&msg);
		}
		break;

		case BOARDING_OP_SOFT_AP_START:
		{
			doorbell_msg_t msg;

			msg.event = DBEVT_WIFI_SOFT_AP_TURNING_ON;
			msg.param = (uint32_t)doorbell_boarding_info;
			doorbell_send_msg(&msg);
		}
		break;

		case BOARDING_OP_SERVICE_UDP_START:
		{
			doorbell_msg_t msg;

			msg.event = DBEVT_LAN_UDP_SERVICE_START_REQUEST;
			msg.param = 0;
			doorbell_send_msg(&msg);
		}
		break;

		case BOARDING_OP_SERVICE_TCP_START:
		{
			doorbell_msg_t msg;

			msg.event = DBEVT_LAN_TCP_SERVICE_START_REQUEST;
			msg.param = 0;
			doorbell_send_msg(&msg);
		}
		break;

		case BOARDING_OP_SET_CS2_DID:
		{
			if (p2p_cs2_key == NULL)
			{
				p2p_cs2_key = os_malloc(sizeof(p2p_cs2_key_t));

				if (p2p_cs2_key == NULL)
				{
					LOGE("malloc p2p_cs2_key\n");
					break;
				}

				os_memset(p2p_cs2_key, 0, sizeof(p2p_cs2_key_t));
			}

			if (length > sizeof(p2p_cs2_key->did))
			{
				LOGE("payload[%d] > did size[%d]\n", length, sizeof(p2p_cs2_key->did));
				break;
			}

			os_memcpy(p2p_cs2_key->did, data, length);

			LOGI("did: %s\n", p2p_cs2_key->did);

			doorbell_boarding_event_message(opcode, BK_OK);
		}
		break;

		case BOARDING_OP_SET_CS2_APILICENSE:
		{
			if (p2p_cs2_key == NULL)
			{
				p2p_cs2_key = os_malloc(sizeof(p2p_cs2_key_t));

				if (p2p_cs2_key == NULL)
				{
					LOGE("malloc p2p_cs2_key\n");
					break;
				}

				os_memset(p2p_cs2_key, 0, sizeof(p2p_cs2_key_t));
			}

			if (length > sizeof(p2p_cs2_key->apilicense))
			{
				LOGE("payload[%d] > apilicense size[%d]\n", length, sizeof(p2p_cs2_key->apilicense));
				break;
			}

			os_memcpy(p2p_cs2_key->apilicense, data, length);

			LOGI("apilicense: %s\n", p2p_cs2_key->apilicense);

			doorbell_boarding_event_message(opcode, BK_OK);
		}
		break;

		case BOARDING_OP_SET_CS2_KEY:
		{
			if (p2p_cs2_key == NULL)
			{
				p2p_cs2_key = os_malloc(sizeof(p2p_cs2_key_t));

				if (p2p_cs2_key == NULL)
				{
					LOGE("malloc p2p_cs2_key\n");
					break;
				}

				os_memset(p2p_cs2_key, 0, sizeof(p2p_cs2_key_t));
			}

			if (length > sizeof(p2p_cs2_key->key))
			{
				LOGE("payload[%d] > key size[%d]\n", length, sizeof(p2p_cs2_key->key));
				break;
			}

			os_memcpy(p2p_cs2_key->key, data, length);

			LOGI("key: %s\n", p2p_cs2_key->key);

			doorbell_boarding_event_message(opcode, BK_OK);
		}
		break;

		case BOARDING_OP_SET_CS2_INIT_STRING:
		{
			if (p2p_cs2_key == NULL)
			{
				p2p_cs2_key = os_malloc(sizeof(p2p_cs2_key_t));

				if (p2p_cs2_key == NULL)
				{
					LOGE("malloc p2p_cs2_key\n");
					break;
				}

				os_memset(p2p_cs2_key, 0, sizeof(p2p_cs2_key_t));
			}

			if (length > sizeof(p2p_cs2_key->initstring))
			{
				LOGE("payload[%d] > initstring size[%d]\n", length, sizeof(p2p_cs2_key->initstring));
				break;
			}


			os_memcpy(p2p_cs2_key->initstring, data, length);

			LOGI("initstring: %s\n", p2p_cs2_key->initstring);

			doorbell_boarding_event_message(opcode, BK_OK);
		}
		break;

		case BOARDING_OP_SRRVICE_CS2_START:
		{
			if (p2p_cs2_key == NULL)
			{
				LOGE("malloc p2p_cs2_key\n");
				break;
			}

			strcat(p2p_cs2_key->apilicense, ":");
			strcat(p2p_cs2_key->apilicense, p2p_cs2_key->key);
			strcat(p2p_cs2_key->initstring, ":");
			strcat(p2p_cs2_key->initstring, p2p_cs2_key->key);

			doorbell_msg_t msg;

			msg.event = DBEVT_P2P_CS2_SERVICE_START_REQUEST;
			msg.param = (uint32_t)p2p_cs2_key;
			doorbell_send_msg(&msg);
		}
		break;

		case BOARDING_OP_BLE_DISABLE:
		{
			doorbell_msg_t msg;

			msg.event = DBEVT_BLE_DISABLE;
			msg.param = 0;
			doorbell_send_msg(&msg);
		}
		break;

	}
}


int doorbell_boarding_init(void)
{
	uint8_t adv_data[ADV_MAX_SIZE] = {0};
	uint8_t adv_index = 0;
	uint8_t len_index = 0;
	uint8_t mac[6];
	int ret;


	LOGI("%s\n", __func__);

	/* flags */
	len_index = adv_index;
	adv_data[adv_index++] = 0x00;
	adv_data[adv_index++] = ADV_TYPE_FLAGS;
	adv_data[adv_index++] = 0x06;
	adv_data[len_index] = 2;

	/* local name */
	bk_ble_get_mac(mac);

	len_index = adv_index;
	adv_data[adv_index++] = 0x00;
	adv_data[adv_index++] = ADV_TYPE_LOCAL_NAME;

	ret = sprintf((char *)&adv_data[adv_index], "%s_%02X%02X%02X",
	              ADV_NAME_HEAD, mac[0], mac[1], mac[2]);

	adv_index += ret;
	adv_data[len_index] = ret + 1;

	/* 16bit uuid */
	len_index = adv_index;
	adv_data[adv_index++] = 0x00;
	adv_data[adv_index++] = ADV_TYPE_SERVICE_DATA;
	adv_data[adv_index++] = BOARDING_UUID & 0xFF;
	adv_data[adv_index++] = BOARDING_UUID >> 8;
	adv_data[len_index] = 3;

	/* manufacturer */
	len_index = adv_index;
	adv_data[adv_index++] = 0x00;
	adv_data[adv_index++] = ADV_TYPE_MANUFACTURER_SPECIFIC;
	adv_data[adv_index++] = BEKEN_COMPANY_ID & 0xFF;
	adv_data[adv_index++] = BEKEN_COMPANY_ID >> 8;
	adv_data[len_index] = 3;

	/*
	os_printf("adv data:\n");

	int i = 0;
	for (i = 0; i < adv_index; i++)
	{
	    os_printf("%02X ", adv_data[i]);
	}

	os_printf("\n");
	*/

	if (doorbell_boarding_info == NULL)
	{
		doorbell_boarding_info = os_malloc(sizeof(ble_boarding_info_t));

		if (doorbell_boarding_info == NULL)
		{
			LOGE("doorbell_boarding_info malloc failed\n");

			goto error;
		}

		os_memset(doorbell_boarding_info, 0, sizeof(ble_boarding_info_t));
	}

	doorbell_boarding_info->cb = doorbell_boarding_operation_handle;

	ble_boarding_init(doorbell_boarding_info);
	ble_boarding_adv_start(adv_data, adv_index);

	return BK_OK;
error:
	return BK_FAIL;
}


void doorbell_boarding_ble_notice_cb(ble_notice_t notice, void *param)
{
	switch (notice)
	{
		case BLE_5_SHUTDOWN_SUCCEED:
			LOGI("ble shutdown ok\n");
			rtos_set_semaphore(&sync_seam);
			break;
		default:
			break;
	}
}

void doorbell_boarding_event_notify_with_data(uint16_t opcode, int status, char *payload, uint16_t length)
{
	uint8_t data[1024] =
	{
		opcode & 0xFF, opcode >> 8,     /* opcode           */
		status & 0xFF,            		/* status           */
		length & 0xFF, length >> 8,     /* payload length   */
	};

	if (length > 1024 - 5)
	{
		LOGE("size %d over flow\n", length);
		return;
	}

	os_memcpy(&data[5], payload, length);

	LOGI("%s: %d, %d\n", __func__, opcode, status);
	ble_boarding_notify(data, length + 5);
}


void doorbell_boarding_ble_disable(void)
{
	LOGD("%s\n", __func__);

#if CONFIG_BTDM_5_2
	if (bk_ble_get_env_state())
	{
		if (bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
		{
			bk_ble_set_notice_cb(doorbell_boarding_ble_notice_cb);
		}
		else
		{
			bk_ble_set_event_callback(NULL);

			LOGE("%s not support !!!\n");
			return;
		}

		LOGI("bluetooth is enabled, shutdown bluetooth\n");
		rtos_init_semaphore(&sync_seam, 1);
		bk_ble_deinit();
		if (rtos_get_semaphore(&sync_seam, 1000) != BK_OK)
		{
			LOGI("%s TIMEOUT\n", __func__);
		}

		rtos_deinit_semaphore(&sync_seam);
		sync_seam = NULL;

		if (bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
		{
			bk_ble_set_notice_cb(NULL);
		}
		else
		{
			bk_ble_set_event_callback(NULL);

			LOGE("%s not support !!!\n");
			return;

		}
	}
	else
	{
		LOGI("bluetooth state: %d\n", bk_ble_get_env_state());
	}
#endif
}
