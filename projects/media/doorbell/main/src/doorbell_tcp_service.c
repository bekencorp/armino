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
#include "doorbell_network.h"
#include "doorbell_tcp_service.h"
#include "doorbell_transmission.h"
#include "doorbell_devices.h"

#include "media_app.h"

#include <driver/dvp_camera_types.h>

#include "cli.h"

#define TAG "db-tcp"

#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define DOORBELL_TCP_BUFFER (1460)
#define DOORBELL_NOTIFY_FLAGS (0xFF << 24)

typedef struct
{
	beken_thread_t img_thd;
	beken_thread_t aud_thd;
	struct sockaddr_in img_remote;
	struct sockaddr_in aud_remote;
	struct sockaddr_in img_socket;
	struct sockaddr_in aud_socket;
	int img_fd;
	int img_server_fd;
	int aud_fd;
	int aud_server_fd;
	uint16_t running : 1;
	uint16_t img_status : 1;
	uint16_t aud_status : 1;
	db_channel_t *img_channel;
	db_channel_t *aud_channel;
} db_tcp_service_t;

db_tcp_service_t *db_tcp_service = NULL;



int doorbell_tcp_img_send_packet(uint8_t *data, uint32_t len)
{
	if (!db_tcp_service->img_status)
	{
		return -1;
	}

	return doorbell_socket_sendto(&db_tcp_service->img_fd, (struct sockaddr *)&db_tcp_service->img_remote, data, len, -sizeof(db_trans_head_t));
}

int doorbell_tcp_img_send_prepare(uint8_t *data, uint32_t length)
{
	doorbell_transmission_pack(db_tcp_service->img_channel, data, length);

	return 0;
}

void *doorbell_tcp_img_get_tx_buf(void)
{
	if (db_tcp_service == NULL)
	{
		LOGE("doorbell_tcp_img_get_tx_buf, service null\n");
		return NULL;
	}

	if (db_tcp_service->img_channel == NULL)
	{
		LOGE("doorbell_tcp_img_get_tx_buf, img_channel null\n");
		return NULL;
	}

	LOGI("doorbell_tcp_img_get_tx_buf, tbuf %p\n", db_tcp_service->img_channel->tbuf);

	return db_tcp_service->img_channel->tbuf + 1;
}

int doorbell_tcp_img_get_tx_size(void)
{
	if (db_tcp_service == NULL)
	{
		LOGE("doorbell_tcp_img_get_tx_size, service null\n");
		return 0;
	}

	if (db_tcp_service->img_channel == NULL)
	{
		LOGE("doorbell_tcp_img_get_tx_size, img_channel null\n");
		return 0;
	}

	return db_tcp_service->img_channel->tsize - sizeof(db_trans_head_t);
}


int doorbell_tcp_aud_send_packet(uint8_t *data, uint32_t len)
{
	if (!db_tcp_service->aud_status)
	{
		return -1;
	}

	return doorbell_socket_sendto(&db_tcp_service->aud_fd, (struct sockaddr *)&db_tcp_service->aud_remote, data, len, -sizeof(db_trans_head_t));
}



int doorbell_tcp_aud_send_prepare(uint8_t *data, uint32_t length)
{
	doorbell_transmission_pack(db_tcp_service->aud_channel, data, length);

	return 0;
}

void *doorbell_tcp_aud_get_tx_buf(void)
{
	if (db_tcp_service == NULL)
	{
		LOGE("doorbell_tcp_aud_get_tx_buf, service null\n");
		return NULL;
	}

	if (db_tcp_service->aud_channel == NULL)
	{
		LOGE("doorbell_tcp_aud_get_tx_buf, aud_channel null\n");
		return NULL;
	}

	//LOGI("doorbell_tcp_aud_get_tx_buf, tbuf %p\n", db_tcp_service->aud_channel->tbuf);

	return db_tcp_service->aud_channel->tbuf + 1;
}

int doorbell_tcp_aud_get_tx_size(void)
{
	if (db_tcp_service == NULL)
	{
		LOGE("doorbell_tcp_aud_get_tx_size, service null\n");
		return 0;
	}

	if (db_tcp_service->aud_channel == NULL)
	{
		LOGE("doorbell_tcp_aud_get_tx_size, img_channel null\n");
		return 0;
	}

	return db_tcp_service->aud_channel->tsize - sizeof(db_trans_head_t);
}

static const media_transfer_cb_t doorbell_tcp_img_channel =
{
	.send = doorbell_tcp_img_send_packet,
	.prepare = doorbell_tcp_img_send_prepare,
	.get_tx_buf = doorbell_tcp_img_get_tx_buf,
	.get_tx_size = doorbell_tcp_img_get_tx_size
};

static const media_transfer_cb_t doorbell_tcp_aud_channel =
{
	.send = doorbell_tcp_aud_send_packet,
	.prepare = doorbell_tcp_aud_send_prepare,
	.get_tx_buf = doorbell_tcp_aud_get_tx_buf,
	.get_tx_size = doorbell_tcp_aud_get_tx_size
};

static void doorbell_image_server_thread(beken_thread_arg_t data)
{
	int rcv_len = 0;
	//  struct sockaddr_in server;
	bk_err_t ret = BK_OK;
	u8 *rcv_buf = NULL;
	fd_set watchfd;

	LOGI("%s entry\n", __func__);
	(void)(data);

	{
		doorbell_msg_t msg;

		msg.event = DBEVT_LAN_TCP_SERVICE_START_RESPONSE;
		msg.param = 0;
		doorbell_send_msg(&msg);
	}


	rcv_buf = (u8 *) os_malloc((DOORBELL_TCP_BUFFER + 1) * sizeof(u8));

	if (!rcv_buf)
	{
		LOGE("tcp os_malloc failed\n");
		goto out;
	}

	// for data transfer
	db_tcp_service->img_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (db_tcp_service->img_server_fd == -1)
	{
		LOGE("socket failed\n");
		goto out;
	}

	db_tcp_service->img_socket.sin_family = AF_INET;
	db_tcp_service->img_socket.sin_port = htons(DOORBELL_TCP_IMG_PORT);
	db_tcp_service->img_socket.sin_addr.s_addr = inet_addr("0.0.0.0");

	if (bind(db_tcp_service->img_server_fd, (struct sockaddr *)&db_tcp_service->img_socket, sizeof(struct sockaddr_in)) == -1)
	{
		LOGE("bind failed\n");
		goto out;
	}

	if (listen(db_tcp_service->img_server_fd, 0) == -1)
	{
		LOGE("listen failed\n");
		goto out;
	}

	LOGI("%s: start listen \n", __func__);

	while (1)
	{
		FD_ZERO(&watchfd);
		FD_SET(db_tcp_service->img_server_fd, &watchfd);

		LOGI("waiting for a new connection\n");
		ret = select(db_tcp_service->img_server_fd + 1, &watchfd, NULL, NULL, NULL);
		if (ret <= 0)
		{
			LOGE("select ret:%d\n", ret);
			continue;
		}
		else
		{
			// is new connection
			if (FD_ISSET(db_tcp_service->img_server_fd, &watchfd))
			{
				struct sockaddr_in client_addr;
				socklen_t cliaddr_len = 0;

				cliaddr_len = sizeof(client_addr);


				db_tcp_service->img_fd = accept(db_tcp_service->img_server_fd, (struct sockaddr *)&client_addr, &cliaddr_len);

				if (db_tcp_service->img_fd < 0)
				{
					LOGE("accept return fd:%d\n",db_tcp_service->img_fd);
					break;
				}

				LOGI("accept a new connection fd:%d\n", db_tcp_service->img_fd);

				doorbell_socket_set_qos(db_tcp_service->img_fd, IP_QOS_PRIORITY_LOW);

				db_tcp_service->img_status = BK_TRUE;

				while (db_tcp_service->img_status == BK_TRUE)
				{
					rcv_len = recv(db_tcp_service->img_fd, rcv_buf, DOORBELL_TCP_BUFFER, 0);
					if (rcv_len > 0)
					{
						//bk_net_send_data(rcv_buf, rcv_len, TVIDEO_SND_TCP);
						LOGI("got length: %d\n", rcv_len);
					}
					else
					{
						// close this socket
						LOGI("recv close fd:%d, rcv_len:%d\n", db_tcp_service->img_fd, rcv_len);
						close(db_tcp_service->img_fd);
						db_tcp_service->img_fd = -1;

						doorbell_msg_t msg;
						msg.event = DBEVT_REMOTE_DEVICE_DISCONNECTED;
						msg.param = BK_OK;
						doorbell_send_msg(&msg);
						break;
					}

				}
			}
		}
	}
out:

	LOGE("%s exit %d\n", __func__, db_tcp_service->img_status);

	if (rcv_buf)
	{
		os_free(rcv_buf);
		rcv_buf = NULL;
	}

	if (db_tcp_service->img_server_fd != -1)
	{
		close(db_tcp_service->img_server_fd);
		db_tcp_service->img_server_fd = -1;
	}

	db_tcp_service->img_status = BK_FALSE;

	db_tcp_service->img_thd = NULL;
	rtos_delete_thread(NULL);
}

void doorbell_tcp_image_server_start(void)
{
	int ret;

	if (!db_tcp_service->img_thd)
	{
		ret = rtos_create_thread(&db_tcp_service->img_thd,
		                         4,
		                         "db_tcp_img_srv",
		                         (beken_thread_function_t)doorbell_image_server_thread,
		                         1024 * 3,
		                         (beken_thread_arg_t)NULL);
		if (ret != kNoErr)
		{
			LOGE("Error: failed to create doorbell tcp img server: %d\n", ret);
		}
	}
}

static inline void doorbell_tcp_voice_receiver(db_channel_t *channel, uint16_t sequence, uint16_t flags, uint32_t timestamp, uint8_t sequences, uint8_t *data, uint16_t length)
{
	LOGD("%s %d\n", __func__, length);
	doorbell_audio_data_callback(data, length);
}


static void doorbell_audio_server_thread(beken_thread_arg_t data)
{
	int rcv_len = 0;
	//  struct sockaddr_in server;
	bk_err_t ret = BK_OK;
	u8 *rcv_buf = NULL;
	fd_set watchfd;

	LOGI("%s entry\n", __func__);
	(void)(data);

	rcv_buf = (u8 *) os_malloc((DOORBELL_TCP_BUFFER + 1) * sizeof(u8));

	if (!rcv_buf)
	{
		LOGE("tcp os_malloc failed\n");
		goto out;
	}

	// for data transfer
	db_tcp_service->aud_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (db_tcp_service->aud_server_fd == -1)
	{
		LOGE("socket failed\n");
		goto out;
	}

	db_tcp_service->aud_socket.sin_family = AF_INET;
	db_tcp_service->aud_socket.sin_port = htons(DOORBELL_TCP_AUD_PORT);
	db_tcp_service->aud_socket.sin_addr.s_addr = inet_addr("0.0.0.0");

	if (bind(db_tcp_service->aud_server_fd, (struct sockaddr *)&db_tcp_service->aud_socket, sizeof(struct sockaddr_in)) == -1)
	{
		LOGE("bind failed\n");
		goto out;
	}

	if (listen(db_tcp_service->aud_server_fd, 0) == -1)
	{
		LOGE("listen failed\n");
		goto out;
	}

	LOGI("%s: start listen \n", __func__);

	while (1)
	{
		FD_ZERO(&watchfd);
		FD_SET(db_tcp_service->aud_server_fd, &watchfd);

		LOGI("waiting for a new connection\n");
		ret = select(db_tcp_service->aud_server_fd + 1, &watchfd, NULL, NULL, NULL);
		if (ret <= 0)
		{
			LOGE("select ret:%d\n", ret);
			continue;
		}
		else
		{
			// is new connection
			if (FD_ISSET(db_tcp_service->aud_server_fd, &watchfd))
			{
				struct sockaddr_in client_addr;
				socklen_t cliaddr_len = 0;

				cliaddr_len = sizeof(client_addr);


				db_tcp_service->aud_fd = accept(db_tcp_service->aud_server_fd, (struct sockaddr *)&client_addr, &cliaddr_len);

				if (db_tcp_service->aud_fd < 0)
				{
					LOGE("accept return fd:%d\n",db_tcp_service->aud_fd);
					break;
				}

				LOGI("accept a new connection fd:%d\n", db_tcp_service->aud_fd);

				db_tcp_service->aud_status = BK_TRUE;

				doorbell_socket_set_qos(db_tcp_service->aud_fd, IP_QOS_PRIORITY_HIGH);

				while (db_tcp_service->aud_status == BK_TRUE)
				{
					rcv_len = recv(db_tcp_service->aud_fd, rcv_buf, DOORBELL_TCP_BUFFER, 0);
					if (rcv_len > 0)
					{
						//bk_net_send_data(rcv_buf, rcv_len, TVIDEO_SND_TCP);
						doorbell_transmission_unpack(db_tcp_service->aud_channel, rcv_buf, rcv_len, doorbell_tcp_voice_receiver);
						//LOGI("got length: %d\n", rcv_len);
					}
					else
					{
						// close this socket
						LOGI("recv close fd:%d, rcv_len:%d\n", db_tcp_service->aud_fd, rcv_len);
						close(db_tcp_service->aud_fd);
						db_tcp_service->aud_fd = -1;
						break;
					}

				}
			}
		}
	}
out:

	LOGE("%s exit %d\n", __func__, db_tcp_service->aud_status);

	if (rcv_buf)
	{
		os_free(rcv_buf);
		rcv_buf = NULL;
	}

	if (db_tcp_service->aud_server_fd != -1)
	{
		close(db_tcp_service->aud_server_fd);
		db_tcp_service->aud_server_fd = -1;
	}

	db_tcp_service->aud_status = BK_FALSE;

	db_tcp_service->aud_thd = NULL;
	rtos_delete_thread(NULL);
}

void doorbell_tcp_audio_server_start(void)
{
	int ret;

	if (!db_tcp_service->aud_thd)
	{
		ret = rtos_create_thread(&db_tcp_service->aud_thd,
		                         4,
		                         "db_tcp_aud_srv",
		                         (beken_thread_function_t)doorbell_audio_server_thread,
		                         1024 * 3,
		                         (beken_thread_arg_t)NULL);
		if (ret != kNoErr)
		{
			LOGE("Error: failed to create doorbell tcp aud server: %d\n", ret);
		}
	}
}


void doorbell_tcp_service_init(void)
{
	db_tcp_service = os_malloc(sizeof(db_tcp_service_t));

	if (db_tcp_service == NULL)
	{
		LOGE("malloc db_tcp_service\n");
		return;
	}

	os_memset(db_tcp_service, 0, sizeof(db_tcp_service_t));


	db_tcp_service->img_channel = doorbell_transmission_malloc(0, 1600);
	db_tcp_service->aud_channel = doorbell_transmission_malloc(1600, 1600);

	doorbell_devices_set_camera_transfer_callback(&doorbell_tcp_img_channel);

	doorbell_devices_set_audio_transfer_callback(&doorbell_tcp_aud_channel);

	doorbell_tcp_image_server_start();
	doorbell_tcp_audio_server_start();

}

