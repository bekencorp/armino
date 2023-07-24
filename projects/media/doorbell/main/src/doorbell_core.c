#include "bk_private/bk_init.h"
#include <components/system.h>
#include <os/os.h>
#include <components/shell_task.h>

#include <modules/wifi.h>
#include <components/event.h>
#include <components/netif.h>


#include "ble_boarding.h"
#include "doorbell_comm.h"
#include "doorbell_sdp.h"
#include "doorbell_cmd.h"
#include "doorbell_devices.h"
#include "doorbell_network.h"
#include "doorbell_cs2_service.h"
#include "doorbell_udp_service.h"
#include "doorbell_tcp_service.h"
#include "doorbell_boarding.h"
#include "cli.h"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define TAG "db-core"

typedef struct
{
	uint32_t enabled : 1;
	uint32_t service : 6;

	char *id;
	beken_thread_t thd;
	beken_queue_t queue;
} doorbell_info_t;

doorbell_info_t *db_info = NULL;

const doorbell_service_interface_t *doorbell_current_service = NULL;


bk_err_t doorbell_send_msg(doorbell_msg_t *msg)
{
	bk_err_t ret = BK_OK;

	if (db_info->queue)
	{
		ret = rtos_push_to_queue(&db_info->queue, msg, BEKEN_NO_WAIT);

		if (BK_OK != ret)
		{
			LOGE("%s failed\n", __func__);
			return BK_FAIL;
		}

		return ret;
	}

	return ret;
}

static void doorbell_message_handle(void)
{
	bk_err_t ret = BK_OK;
	doorbell_msg_t msg;

	while (1)
	{

		ret = rtos_pop_from_queue(&db_info->queue, &msg, BEKEN_WAIT_FOREVER);

		if (kNoErr == ret)
		{
			switch (msg.event)
			{
				case DBEVT_WIFI_STATION_CONNECT:
				{
					LOGI("DBEVT_WIFI_STATION_CONNECT\n");

					ble_boarding_info_t *boarding = (ble_boarding_info_t *) msg.param;

					doorbell_wifi_sta_connect(boarding->ssid_value, boarding->password_value);
				}
				break;

				case DBEVT_WIFI_STATION_CONNECTED:
				{
					LOGI("DBEVT_WIFI_STATION_CONNECTED\n");

					netif_ip4_config_t ip4_config;
					extern uint32_t uap_ip_is_start(void);

					os_memset(&ip4_config, 0x0, sizeof(netif_ip4_config_t));
					bk_netif_get_ip4_config(NETIF_IF_AP, &ip4_config);
					if (uap_ip_is_start())
						bk_netif_get_ip4_config(NETIF_IF_AP, &ip4_config);
					else
						bk_netif_get_ip4_config(NETIF_IF_STA, &ip4_config);

					LOGI("ip: %s\n", ip4_config.ip);

					doorbell_boarding_event_notify_with_data(BOARDING_OP_STATION_START, BK_OK, ip4_config.ip, strlen(ip4_config.ip));
				}
				break;

				case DBEVT_WIFI_STATION_DISCONNECTED:
				{
					LOGI("DBEVT_WIFI_STATION_DISCONNECTED\n");
				}
				break;

				case DBEVT_WIFI_SOFT_AP_TURNING_ON:
				{
					LOGI("DBEVT_WIFI_SOFT_AP_TURNING_ON\n");
					ble_boarding_info_t *boarding = (ble_boarding_info_t *) msg.param;
					int ret = doorbell_wifi_soft_ap_start(boarding->ssid_value, boarding->password_value);

					if (ret == BK_OK)
					{
						doorbell_boarding_event_notify(BOARDING_OP_SOFT_AP_START, EVT_STATUS_OK);
					}
					else
					{
						doorbell_boarding_event_notify(BOARDING_OP_SOFT_AP_START, EVT_STATUS_ERROR);
					}
				}
				break;


				case DBEVT_LAN_UDP_SERVICE_START_REQUEST:
				{
					LOGI("DBEVT_LAN_UDP_SERVICE_START_REQUEST\n");

					if (db_info->service != DOORBELL_SERVICE_NONE)
					{
						LOGW("DBEVT_LAN_UDP_SERVICE_START_REQUEST service: %d already start up\n", db_info->service);
						break;
					}

					db_info->service = DOORBELL_SERVICE_LAN_UDP;

					doorbell_cmd_server_init();
					doorbell_udp_service_init();

				}
				break;

				case DBEVT_LAN_UDP_SERVICE_START_RESPONSE:
				{
					LOGI("DBEVT_LAN_UDP_SERVICE_START_RESPONSE\n");

					doorbell_sdp_start("doorbell-udp", DOORBELL_CMD_PORT, DOORBELL_UDP_IMG_PORT, DOORBELL_UDP_AUD_PORT);

					doorbell_boarding_event_notify(BOARDING_OP_SERVICE_UDP_START, BK_OK);
				}
				break;

				case DBEVT_LAN_TCP_SERVICE_START_REQUEST:
				{
					LOGI("DBEVT_LAN_TCP_SERVICE_START_REQUEST\n");

					if (db_info->service != DOORBELL_SERVICE_NONE)
					{
						LOGW("DBEVT_LAN_TCP_SERVICE_START_REQUEST service: %d already start up\n", db_info->service);
						break;
					}

					db_info->service = DOORBELL_SERVICE_LAN_TCP;

					doorbell_cmd_server_init();
					doorbell_tcp_service_init();

				}
				break;

				case DBEVT_LAN_TCP_SERVICE_START_RESPONSE:
				{
					LOGI("DBEVT_LAN_TCP_SERVICE_START_RESPONSE\n");

					doorbell_sdp_start("doorbell-tcp", DOORBELL_CMD_PORT, DOORBELL_TCP_IMG_PORT, DOORBELL_TCP_AUD_PORT);

					doorbell_boarding_event_notify(BOARDING_OP_SERVICE_TCP_START, BK_OK);
				}
				break;

				case DBEVT_P2P_CS2_SERVICE_START_REQUEST:
				{
#ifdef CONFIG_INTEGRATION_DOORBELL_CS2
					LOGI("DBEVT_P2P_CS2_SERVICE_START_REQUEST\n");

					if (db_info->service != DOORBELL_SERVICE_NONE)
					{
						LOGW("DBEVT_P2P_CS2_SERVICE_START_REQUEST service: %d already start up\n", db_info->service);
						break;
					}

					db_info->service = DOORBELL_SERVICE_P2P_CS2;

					p2p_cs2_key_t *key = (p2p_cs2_key_t *)msg.param;

					doorbell_current_service = get_doorbell_cs2_service_interface();
					doorbell_current_service->init(key);
#endif
				}
				break;

				case DBEVT_P2P_CS2_SERVICE_START_RESPONSE:
				{
					doorbell_boarding_event_notify(BOARDING_OP_SRRVICE_CS2_START, BK_OK);
				}
				break;

				case DBEVT_START_BOARDING_EVENT:
				{
					uint16_t opcode = msg.param & 0xFFFF;
					int status = msg.param >> 16;
					doorbell_boarding_event_notify(opcode, status);
				}
				break;


				case DBEVT_BLE_DISABLE:
				{
					doorbell_boarding_ble_disable();
				}
				break;

				case DBEVT_REMOTE_DEVICE_CONNECTED:
				{
					if (db_info->service == DOORBELL_SERVICE_LAN_UDP)
					{
						doorbell_udp_update_remote_address((in_addr_t)msg.param);
						doorbell_sdp_stop();
					}
					else if(db_info->service == DOORBELL_SERVICE_LAN_TCP)
					{
						doorbell_sdp_stop();
					}
				}
				break;

				case DBEVT_REMOTE_DEVICE_DISCONNECTED:
				{
					doorbell_camera_turn_off();
					doorbell_audio_turn_off();

					if (db_info->service == DOORBELL_SERVICE_LAN_UDP)
					{
						doorbell_sdp_start("doorbell-udp", DOORBELL_CMD_PORT, DOORBELL_UDP_IMG_PORT, DOORBELL_UDP_AUD_PORT);
					}
					else if(db_info->service == DOORBELL_SERVICE_LAN_TCP)
					{
						doorbell_sdp_start("doorbell-tcp", DOORBELL_CMD_PORT, DOORBELL_TCP_IMG_PORT, DOORBELL_TCP_AUD_PORT);
					}


				}
				break;

				case DBEVT_EXIT:
					goto exit;
					break;

				default:
					break;
			}
		}
	}

exit:

	/* delate msg queue */
	ret = rtos_deinit_queue(&db_info->queue);

	if (ret != kNoErr)
	{
		LOGE("delate message queue fail\n");
	}

	db_info->queue = NULL;

	LOGE("delate message queue complete\n");

	/* delate task */
	rtos_delete_thread(NULL);

	db_info->thd = NULL;

	LOGE("delate task complete\n");
}



void doorbell_core_init(void)
{
	bk_err_t ret = BK_OK;

	if (db_info == NULL)
	{
		db_info = os_malloc(sizeof(doorbell_info_t));

		if (db_info == NULL)
		{
			LOGE("%s, malloc db_info failed\n", __func__);
			goto error;
		}

		os_memset(db_info, 0, sizeof(doorbell_info_t));
	}


	if (db_info->queue != NULL)
	{
		ret = BK_FAIL;
		LOGE("%s, db_info->queue allready init, exit!\n", __func__);
		goto error;
	}

	if (db_info->thd != NULL)
	{
		ret = BK_FAIL;
		LOGE("%s, db_info->thd allready init, exit!\n", __func__);
		goto error;
	}

	ret = rtos_init_queue(&db_info->queue,
	                      "db_info->queue",
	                      sizeof(doorbell_msg_t),
	                      10);

	if (ret != BK_OK)
	{
		LOGE("%s, ceate doorbell message queue failed\n");
		goto error;
	}

	ret = rtos_create_thread(&db_info->thd,
	                         BEKEN_DEFAULT_WORKER_PRIORITY,
	                         "db_info->thd",
	                         (beken_thread_function_t)doorbell_message_handle,
	                         1024 * 4,
	                         NULL);

	if (ret != BK_OK)
	{
		LOGE("create media major thread fail\n");
		goto error;
	}

	doorbell_devices_init();
	doorbell_boarding_init();


	db_info->enabled = BK_TRUE;

	LOGE("%s success\n", __func__);

	return;

error:

	LOGE("%s fail\n", __func__);
}
