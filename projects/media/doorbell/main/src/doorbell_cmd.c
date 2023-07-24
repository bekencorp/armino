#include <common/bk_include.h>
#include "cli.h"
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include <driver/int.h>
#include <common/bk_err.h>
#include <getopt.h>

#include "lwip/sockets.h"
#include "lwip/udp.h"
#include "net.h"
#include "string.h"
#include <components/netif.h>

#include <common/bk_generic.h>

#include "doorbell_comm.h"
#include "doorbell_transmission.h"
#include "doorbell_devices.h"
#include "doorbell_cmd.h"

#include <driver/dvp_camera_types.h>

#include "cli.h"

#define TAG "db-cmd"

#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define DOORBELL_CMD_BUFFER (1460)
#define DOORBELL_NOTIFY_FLAGS (0xFF << 24)

typedef enum
{
	DBCMD_SET_SERVICE_TYPE = 1,
	DBCMD_SET_KEEP_ALIVE = 2,
	DBCMD_GET_SUPPORTED_CAMERA__DEVICES = 3,
	DBCMD_GET_SUPPORTED_LCD_DEVICES = 4,
	DBCMD_GET_SUPPORTED_MIC_DEVICES = 5,
	DBCMD_GET_SUPPORTED_SPEAKER_DEVICES = 6,

	DBCMD_SET_CAMERA_TURN_ON = 7,
	DBCMD_SET_CAMERA_TURN_OFF = 8,
	DBCMD_GET_CAMERA_STATUS = 9,

	DBCMD_SET_AUDIO_TURN_ON = 10,
	DBCMD_SET_AUDIO_TURN_OFF = 11,
	DBCMD_GET_AUDIO_STATUS = 12,

	DBCMD_SET_LCD_TURN_ON = 13,
	DBCMD_SET_LCD_TURN_OFF = 14,
	DBCMD_GET_LCD_STATUS = 15,
} dbcmd_t;


typedef enum
{
	DBNOTIFY_HEARTBEAT = 1 | (DOORBELL_NOTIFY_FLAGS),
} dbnotify_t;



typedef struct
{
	uint32_t server_state : 1;
	struct sockaddr_in socket;
	beken_thread_t thread;
	int server_fd;
	int client_fd;
	db_channel_t *db_channel;
	beken_mutex_t tx_lock;
	beken_timer_t timer;
	uint32_t intval_ms;
} db_cmd_info_t;


db_cmd_info_t *db_cmd_info = NULL;




int doorbell_transmission_send(uint8_t *data, uint16_t length)
{
	int ret = 0;

	if (db_cmd_info->server_state == BK_FALSE)
	{
		LOGE("%s server not ready\n", __func__);
		return -1;
	}

	if (db_cmd_info->client_fd < 0)
	{
		LOGE("%s client not ready\n", __func__);
		return -1;
	}

	rtos_lock_mutex(&db_cmd_info->tx_lock);
	ret = write(db_cmd_info->client_fd, data, length);
	rtos_unlock_mutex(&db_cmd_info->tx_lock);

	return ret;
}


static const db_channel_cb_t db_channel_callback =
{
	.tsend = doorbell_transmission_send,
};

void doorbell_transmission_event_report(db_channel_t *channel, uint32_t opcode, uint8_t status, uint16_t flags)
{
	db_evt_head_t evt;
	evt.opcode = CHECK_ENDIAN_UINT16(opcode);
	evt.status = status;
	evt.length = CHECK_ENDIAN_UINT16(0);
	evt.flags = flags;

	doorbell_transmission_pack_send(channel, (uint8_t *)&evt, sizeof(db_evt_head_t), doorbell_transmission_send);
}


static void doorbell_keep_alive_timer_handler(void *data)
{
	if (db_cmd_info == NULL)
	{
		LOGE("db_cmd_info NULL return");
		return;
	}

	LOGI("doorbell_keep_alive_timer_handler\n");

	db_evt_head_t evt;
	evt.opcode = CHECK_ENDIAN_UINT16(DBNOTIFY_HEARTBEAT);
	evt.status = EVT_STATUS_OK;
	evt.length = CHECK_ENDIAN_UINT16(0);
	evt.flags = EVT_FLAGS_COMPLETE;

	doorbell_transmission_pack_send(db_cmd_info->db_channel, (uint8_t *)&evt, sizeof(db_evt_head_t), doorbell_transmission_send);
}

int doorbell_keep_alive_start_timer(UINT32 time_ms)
{
	if (db_cmd_info)
	{
		int err;
		UINT32 org_ms = db_cmd_info->intval_ms;

		if (org_ms != 0)
		{
			if ((org_ms != time_ms))
			{
				if (db_cmd_info->timer.handle != NULL)
				{
					err = rtos_deinit_timer(&db_cmd_info->timer);
					if (BK_OK != err)
					{
						LOGE("deinit time fail\r\n");
						return BK_FAIL;
					}
					db_cmd_info->timer.handle = NULL;
				}
			}
			else
			{
				LOGE("timer aready start\r\n");
				return BK_OK;
			}
		}

		err = rtos_init_timer(&db_cmd_info->timer,
		                      time_ms,
		                      doorbell_keep_alive_timer_handler,
		                      NULL);
		if (BK_OK != err)
		{
			LOGE("init timer fail\r\n");
			return BK_FAIL;
		}
		db_cmd_info->intval_ms = time_ms;

		err = rtos_start_timer(&db_cmd_info->timer);
		if (BK_OK != err)
		{
			LOGE("start timer fail\r\n");
			return BK_FAIL;
		}
		LOGI("doorbell_keep_alive_start_timer\r\n");

		return BK_OK;
	}
	return BK_FAIL;
}

int doorbell_keep_alive_stop_timer(void)
{
	if (db_cmd_info)
	{
		int err;

		err = rtos_stop_timer(&db_cmd_info->timer);
		if (BK_OK != err)
		{
			LOGE("stop time fail\r\n");
			return BK_FAIL;
		}

		return BK_OK;
	}
	return BK_FAIL;
}


void doorbell_transmission_cmd_recive_callback(db_channel_t *channel, uint16_t sequence, uint16_t flags, uint32_t timestamp, uint8_t sequences, uint8_t *data, uint16_t length)
{
	db_cmd_head_t cmd, *ptr = (db_cmd_head_t *)data;
	uint8_t *p = ptr->payload;

	if (length < sizeof(db_cmd_head_t))
	{
		LOGE("cmd not enough\n");
		return;
	}

	cmd.opcode = CHECK_ENDIAN_UINT32(ptr->opcode);
	cmd.param = CHECK_ENDIAN_UINT32(ptr->param);
	cmd.length = CHECK_ENDIAN_UINT32(ptr->length);

	LOGI("opcode: %u, param: %u, length: %u, time: %u, sequence: %u\n", cmd.opcode, cmd.param, cmd.length, timestamp, sequences);

	switch (cmd.opcode)
	{
		case DBCMD_SET_SERVICE_TYPE:
		{
			LOGI("DBCMD_SET_SOLUTION: %d, %s(%d)\n", cmd.opcode, ptr->payload, strlen((char *)ptr->payload));
			db_evt_head_t evt;
			doorbell_msg_t msg;

			evt.opcode = CHECK_ENDIAN_UINT16(cmd.opcode);
			evt.status = EVT_STATUS_OK;
			evt.length = CHECK_ENDIAN_UINT16(0);
			evt.flags = EVT_FLAGS_COMPLETE;

			if (!os_strncmp((const char *)ptr->payload, "doorbell-udp", strlen("doorbell-udp")))
			{
				msg.event = DBEVT_LAN_UDP_SERVICE_START_REQUEST;
				doorbell_send_msg(&msg);
			}
			else if (!os_strncmp((const char *)ptr->payload, "doorbell-tcp", strlen("doorbell-tcp")))
			{
				msg.event = DBEVT_LAN_TCP_SERVICE_START_REQUEST;
				doorbell_send_msg(&msg);
			}
			else
			{
				evt.status = EVT_STATUS_ERROR;

				LOGE("DBCMD_SET_SERVICE_TYPE error\n");
			}

			doorbell_transmission_pack_send(channel, (uint8_t *)&evt, sizeof(db_evt_head_t), doorbell_transmission_send);
		}
		break;

		case DBCMD_SET_KEEP_ALIVE:
		{
			LOGI("DBCMD_SET_KEEP_ALIVE: %u\n", cmd.param);

			if (cmd.param)
			{
				doorbell_keep_alive_start_timer(cmd.param);
			}
			else
			{
				doorbell_keep_alive_stop_timer();
			}
		}
		break;

		case DBCMD_GET_SUPPORTED_CAMERA__DEVICES:
		{
			doorbell_get_supported_camera_devices(cmd.opcode, channel, doorbell_transmission_send);
		}
		break;

		case DBCMD_GET_SUPPORTED_LCD_DEVICES:
		{
			doorbell_get_supported_lcd_devices(cmd.opcode, channel, doorbell_transmission_send);
		}
		break;

		case DBCMD_SET_CAMERA_TURN_ON:
		{
			if (length != 4)
			{
				LOGD("error\n");
			}

			uint16_t width, height;
			STREAM_TO_UINT16(width, p);
			STREAM_TO_UINT16(height, p);

			int ret = doorbell_camera_turn_on(cmd.param & 0xFFFF, width, height);

			doorbell_transmission_event_report(channel, cmd.opcode, ret & 0xFF, EVT_FLAGS_COMPLETE);
		}
		break;

		case DBCMD_SET_CAMERA_TURN_OFF:
		{
			int ret = doorbell_camera_turn_off();

			doorbell_transmission_event_report(channel, cmd.opcode, ret & 0xFF, EVT_FLAGS_COMPLETE);
		}
		break;

		case DBCMD_GET_CAMERA_STATUS:
		{

		}
		break;

		case DBCMD_SET_AUDIO_TURN_ON:
		{
			uint8_t aec = 0, uac = 0;
			uint32_t sample_rate = 0;

			STREAM_TO_UINT8(aec, p);
			STREAM_TO_UINT8(uac, p);
			STREAM_TO_UINT32(sample_rate, p);

			int ret = doorbell_audio_turn_on(aec, uac, sample_rate);

			doorbell_transmission_event_report(channel, cmd.opcode, ret & 0xFF, EVT_FLAGS_COMPLETE);
		}
		break;

		case DBCMD_SET_AUDIO_TURN_OFF:
		{
			LOGI("DBCMD_SET_AUDIO_TURN_OFF\n");

			int ret = doorbell_audio_turn_off();

			doorbell_transmission_event_report(channel, cmd.opcode, ret & 0xFF, EVT_FLAGS_COMPLETE);
		}
		break;

		case DBCMD_GET_AUDIO_STATUS:
		{

		}
		break;

		case DBCMD_SET_LCD_TURN_ON:
		{
			uint16_t id, rotate;
			STREAM_TO_UINT16(id, p);
			STREAM_TO_UINT8(rotate, p);

			int ret = doorbell_display_turn_on(id, rotate);

			doorbell_transmission_event_report(channel, cmd.opcode, ret & 0xFF, EVT_FLAGS_COMPLETE);
		}
		break;

		case DBCMD_SET_LCD_TURN_OFF:
		{
			int ret = doorbell_display_turn_off();

			doorbell_transmission_event_report(channel, cmd.opcode, ret & 0xFF, EVT_FLAGS_COMPLETE);
		}
		break;

		case DBCMD_GET_LCD_STATUS:
		{

		}
		break;



	}
}

static void doorbell_cmd_set_keepalive(int fd)
{
	int opt = 1, ret;
	// open tcp keepalive
	ret = setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(int));

	opt = 30;  // 5 second
	ret = setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &opt, sizeof(int));

	opt = 1;  // 1s second for intval
	ret = setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &opt, sizeof(int));

	opt = 3;  // 3 times
	ret = setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &opt, sizeof(int));
	ret = ret;
}


static void doorbell_cmd_server_thread(beken_thread_arg_t data)
{
	int rcv_len = 0;
	//  struct sockaddr_in server;
	bk_err_t ret = BK_OK;
	u8 *rcv_buf = NULL;
	fd_set watchfd;

	LOGI("%s entry\n", __func__);
	(void)(data);

	rcv_buf = (u8 *) os_malloc((DOORBELL_CMD_BUFFER + 1) * sizeof(u8));
	if (!rcv_buf)
	{
		LOGE("tcp os_malloc failed\n");
		goto out;
	}

	// for data transfer
	db_cmd_info->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (db_cmd_info->server_fd == -1)
	{
		LOGE("socket failed\n");
		goto out;
	}

	db_cmd_info->socket.sin_family = AF_INET;
	db_cmd_info->socket.sin_port = htons(DOORBELL_CMD_PORT);
	db_cmd_info->socket.sin_addr.s_addr = inet_addr("0.0.0.0");

	if (bind(db_cmd_info->server_fd, (struct sockaddr *)&db_cmd_info->socket, sizeof(struct sockaddr_in)) == -1)
	{
		LOGE("bind failed\n");
		goto out;
	}

	if (listen(db_cmd_info->server_fd, 0) == -1)
	{
		LOGE("listen failed\n");
		goto out;
	}

	LOGI("%s: start listen \n", __func__);

	while (1)
	{
		FD_ZERO(&watchfd);
		FD_SET(db_cmd_info->server_fd, &watchfd);

		LOGI("waiting for a new connection\n");
		ret = select(db_cmd_info->server_fd + 1, &watchfd, NULL, NULL, NULL);
		if (ret <= 0)
		{
			LOGE("select ret:%d\n", ret);
			continue;
		}
		else
		{
			// is new connection
			if (FD_ISSET(db_cmd_info->server_fd, &watchfd))
			{
				struct sockaddr_in client_addr;
				socklen_t cliaddr_len = 0;

				cliaddr_len = sizeof(client_addr);
				db_cmd_info->client_fd = accept(db_cmd_info->server_fd, (struct sockaddr *)&client_addr, &cliaddr_len);

				if (db_cmd_info->client_fd < 0)
				{
					LOGE("accept return fd:%d\n", db_cmd_info->client_fd);
					break;
				}

				LOGI("accept a new connection fd:%d\n", db_cmd_info->client_fd);

				//doorbell_cmd_set_keepalive(db_cmd_info->client_fd);

				db_cmd_info->server_state = BK_TRUE;

				while (db_cmd_info->server_state == BK_TRUE)
				{
					rcv_len = recv(db_cmd_info->client_fd, rcv_buf, DOORBELL_CMD_BUFFER, 0);
					if (rcv_len > 0)
					{
						//bk_net_send_data(rcv_buf, rcv_len, TVIDEO_SND_TCP);
						LOGI("got length: %d\n", rcv_len);
						doorbell_transmission_unpack(db_cmd_info->db_channel, rcv_buf, rcv_len, doorbell_transmission_cmd_recive_callback);
					}
					else
					{
						// close this socket
						LOGI("recv close fd:%d, rcv_len:%d\n", db_cmd_info->client_fd, rcv_len);
						close(db_cmd_info->client_fd);
						db_cmd_info->client_fd = -1;
						break;
					}

				}
			}
		}
	}
out:

	LOGE("%s exit %d\n", __func__, db_cmd_info->server_state);

	if (rcv_buf)
	{
		os_free(rcv_buf);
		rcv_buf = NULL;
	}

	if (db_cmd_info->server_fd != -1)
	{
		close(db_cmd_info->server_fd);
		db_cmd_info->server_fd = -1;
	}

	db_cmd_info->server_state = BK_FALSE;

	db_cmd_info->thread = NULL;
	rtos_delete_thread(NULL);
}

void doorbell_cmd_server_init(void)
{
	bk_err_t ret;

	db_cmd_info = os_malloc(sizeof(db_cmd_info_t));

	if (db_cmd_info == NULL)
	{
		LOGE("malloc db_cmd_info\n");
		return;
	}

	os_memset(db_cmd_info, 0, sizeof(db_cmd_info_t));

	rtos_init_mutex(&db_cmd_info->tx_lock);

	db_cmd_info->db_channel = doorbell_transmission_malloc(1600, 1600);
	db_cmd_info->db_channel->cb = &db_channel_callback;

	if (!db_cmd_info->thread)
	{
		ret = rtos_create_thread(&db_cmd_info->thread,
		                         4,
		                         "db_cmd_srv",
		                         (beken_thread_function_t)doorbell_cmd_server_thread,
		                         1024 * 3,
		                         (beken_thread_arg_t)NULL);
		if (ret != kNoErr)
		{
			LOGE("Error: failed to create doorbell cmd server: %d\n", ret);
		}
	}
}

