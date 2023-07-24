#include <common/bk_include.h>

#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include <driver/int.h>
#include <common/bk_err.h>

#ifdef CONFIG_RTT
#include <sys/socket.h>
#endif
#include "lwip/sockets.h"

#include "bk_uart.h"
#include <os/mem.h>
#include <components/video_transfer.h>
#include <driver/dma.h>
#include <modules/audio_ring_buff.h>
#include <components/aud_intf.h>
#include <components/aud_intf_types.h>

#include "lcd_act.h"

#include "doorbell_comm.h"
#include "doorbell_cmd.h"
#include "doorbell_network.h"
#include "doorbell_udp_service.h"
#include "doorbell_transmission.h"
#include "doorbell_devices.h"

#include "media_app.h"

#define TAG "doorbell-UDP"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define APP_DEMO_UDP_SOCKET_TIMEOUT     100  // ms


typedef struct
{
	beken_thread_t thd;
	struct sockaddr_in img_remote;
	struct sockaddr_in aud_remote;
	int img_fd;
	int aud_fd;
	uint16_t running : 1;
	uint16_t img_status : 1;
	uint16_t aud_status : 1;
	db_channel_t *img_channel;
	db_channel_t *aud_channel;
} db_udp_service_t;


db_udp_service_t *db_udp_service = NULL;



#if AUDIO_TRANSFER_ENABLE
static aud_intf_drv_setup_t aud_intf_drv_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
static aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;
static aud_intf_voc_setup_t aud_voc_setup = DEFAULT_AUD_INTF_VOC_SETUP_CONFIG();
#endif

#define UDP_MAX_RETRY (1000)
#define UDP_MAX_DELAY (20)

void doorbell_udp_update_remote_address(in_addr_t address)
{
	LOGI("%s\n", __func__);
	db_udp_service->img_remote.sin_addr.s_addr = address;
	db_udp_service->aud_remote.sin_addr.s_addr = address;
}

int doorbell_udp_img_send_packet(uint8_t *data, uint32_t len)
{
	if (!db_udp_service->img_status)
	{
		return -1;
	}

	return doorbell_socket_sendto(&db_udp_service->img_fd, (struct sockaddr *)&db_udp_service->img_remote, data, len, -sizeof(db_trans_head_t));
}


int doorbell_udp_img_send_prepare(uint8_t *data, uint32_t length)
{
	doorbell_transmission_pack(db_udp_service->img_channel, data, length);

	return 0;
}

void *doorbell_udp_img_get_tx_buf(void)
{
	if (db_udp_service == NULL)
	{
		LOGE("doorbell_udp_img_get_tx_buf, service null\n");
		return NULL;
	}

	if (db_udp_service->img_channel == NULL)
	{
		LOGE("doorbell_udp_img_get_tx_buf, img_channel null\n");
		return NULL;
	}

	LOGI("doorbell_udp_img_get_tx_buf, tbuf %p\n", db_udp_service->img_channel->tbuf);

	return db_udp_service->img_channel->tbuf + 1;
}

int doorbell_udp_img_get_tx_size(void)
{
	if (db_udp_service == NULL)
	{
		LOGE("doorbell_udp_img_get_tx_size, service null\n");
		return 0;
	}

	if (db_udp_service->img_channel == NULL)
	{
		LOGE("doorbell_udp_img_get_tx_size, img_channel null\n");
		return 0;
	}

	return db_udp_service->img_channel->tsize - sizeof(db_trans_head_t);
}


int doorbell_udp_aud_send_packet(uint8_t *data, uint32_t len)
{
	if (!db_udp_service->aud_status)
	{
		return -1;
	}

	return doorbell_socket_sendto(&db_udp_service->aud_fd, (struct sockaddr *)&db_udp_service->aud_remote, data, len, -sizeof(db_trans_head_t));
}



int doorbell_udp_aud_send_prepare(uint8_t *data, uint32_t length)
{
	doorbell_transmission_pack(db_udp_service->aud_channel, data, length);

	return 0;
}

void *doorbell_udp_aud_get_tx_buf(void)
{
	if (db_udp_service == NULL)
	{
		LOGE("doorbell_udp_aud_get_tx_buf, service null\n");
		return NULL;
	}

	if (db_udp_service->aud_channel == NULL)
	{
		LOGE("doorbell_udp_aud_get_tx_buf, aud_channel null\n");
		return NULL;
	}

	//LOGI("doorbell_udp_aud_get_tx_buf, tbuf %p\n", db_udp_service->aud_channel->tbuf);

	return db_udp_service->aud_channel->tbuf + 1;
}

int doorbell_udp_aud_get_tx_size(void)
{
	if (db_udp_service == NULL)
	{
		LOGE("doorbell_udp_aud_get_tx_size, service null\n");
		return 0;
	}

	if (db_udp_service->aud_channel == NULL)
	{
		LOGE("doorbell_udp_aud_get_tx_size, img_channel null\n");
		return 0;
	}

	return db_udp_service->aud_channel->tsize - sizeof(db_trans_head_t);
}

static const media_transfer_cb_t doorbell_udp_img_channel =
{
	.send = doorbell_udp_img_send_packet,
	.prepare = doorbell_udp_img_send_prepare,
	.get_tx_buf = doorbell_udp_img_get_tx_buf,
	.get_tx_size = doorbell_udp_img_get_tx_size
};

static const media_transfer_cb_t doorbell_udp_aud_channel =
{
	.send = doorbell_udp_aud_send_packet,
	.prepare = doorbell_udp_aud_send_prepare,
	.get_tx_buf = doorbell_udp_aud_get_tx_buf,
	.get_tx_size = doorbell_udp_aud_get_tx_size
};

static inline void doorbell_udp_voice_receiver(db_channel_t *channel, uint16_t sequence, uint16_t flags, uint32_t timestamp, uint8_t sequences, uint8_t *data, uint16_t length)
{
	LOGD("%s %d\n", __func__, length);
	doorbell_audio_data_callback(data, length);
}

static void doorbell_udp_service_main(beken_thread_arg_t data)
{
	GLOBAL_INT_DECLARATION();
	int maxfd, ret = 0;
	int rcv_len = 0;
	socklen_t srvaddr_len = 0;
	fd_set watchfd;
	struct timeval timeout;
	u8 *rcv_buf = NULL;
	in_addr_t remote = doorbell_cmd_get_socket_address();

	LOGI("doorbell_udp_service, img: %d, aud: %d\n", DOORBELL_UDP_IMG_PORT, DOORBELL_UDP_AUD_PORT);
	(void)(data);

	rcv_buf = (u8 *)os_malloc((DOORBELL_NETWORK_MAX_SIZE + 1) * sizeof(u8));
	if (!rcv_buf)
	{
		LOGE("udp os_malloc failed\n");
		goto out;
	}


	// for data transfer
	db_udp_service->img_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (db_udp_service->img_fd == -1)
	{
		LOGE("socket failed\n");
		goto out;
	}

	db_udp_service->img_remote.sin_family = AF_INET;
	db_udp_service->img_remote.sin_port = htons(DOORBELL_UDP_IMG_PORT);

	if (remote == 0)
	{
		db_udp_service->img_remote.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		db_udp_service->img_remote.sin_addr.s_addr = remote;
	}

	srvaddr_len = (socklen_t)sizeof(struct sockaddr_in);
	if (bind(db_udp_service->img_fd, (struct sockaddr *)&db_udp_service->img_remote, srvaddr_len) == -1)
	{
		LOGE("bind failed\n");
		goto out;
	}


	db_udp_service->aud_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (db_udp_service->aud_fd == -1)
	{
		LOGE("vo socket failed\n");
		goto out;
	}

	db_udp_service->aud_remote.sin_family = AF_INET;
	db_udp_service->aud_remote.sin_port = htons(DOORBELL_UDP_AUD_PORT);


	if (remote == 0)
	{
		db_udp_service->aud_remote.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		db_udp_service->aud_remote.sin_addr.s_addr = remote;
	}

	srvaddr_len = (socklen_t)sizeof(struct sockaddr_in);
	if (bind(db_udp_service->aud_fd, (struct sockaddr *)&db_udp_service->aud_remote, srvaddr_len) == -1)
	{
		LOGE("bind failed\n");
		goto out;
	}
	maxfd = (db_udp_service->img_fd > db_udp_service->aud_fd) ? db_udp_service->img_fd : db_udp_service->aud_fd;

	timeout.tv_sec = APP_DEMO_UDP_SOCKET_TIMEOUT / 1000;
	timeout.tv_usec = (APP_DEMO_UDP_SOCKET_TIMEOUT % 1000) * 1000;


	GLOBAL_INT_DISABLE();
	db_udp_service->img_status = 1;
	db_udp_service->running = 1;
	db_udp_service->aud_status = 1;
	GLOBAL_INT_RESTORE();


	{
		doorbell_msg_t msg;

		msg.event = DBEVT_LAN_UDP_SERVICE_START_RESPONSE;
		msg.param = 0;
		doorbell_send_msg(&msg);
	}



	while (db_udp_service->running)
	{
		FD_ZERO(&watchfd);
		FD_SET(db_udp_service->img_fd, &watchfd);
		FD_SET(db_udp_service->aud_fd, &watchfd);

		//DBD("wait for select\n");
		ret = select(maxfd + 1, &watchfd, NULL, NULL, &timeout);
		if (ret < 0)
		{
			LOGE("select ret:%d\n", ret);
			break;
		}
		else
		{
			// is img fd, data transfer
			if (FD_ISSET(db_udp_service->img_fd, &watchfd))
			{
				rcv_len = recvfrom(db_udp_service->img_fd, rcv_buf, DOORBELL_NETWORK_MAX_SIZE, 0,
				                   (struct sockaddr *)&db_udp_service->img_remote, &srvaddr_len);

				if (rcv_len <= 0)
				{
					// close this socket
					LOGE("recv close fd:%d\n", db_udp_service->img_fd);
					break;
				}
				else
				{
					rcv_len = (rcv_len > DOORBELL_NETWORK_MAX_SIZE) ? DOORBELL_NETWORK_MAX_SIZE : rcv_len;
					rcv_buf[rcv_len] = 0;

					LOGE("demo_doorbell_udp_receiver\n");
					//demo_doorbell_udp_receiver(rcv_buf, rcv_len, &db_udp_service->img_remote);
				}
			}

			if (FD_ISSET(db_udp_service->aud_fd, &watchfd))
			{
				rcv_len = recvfrom(db_udp_service->aud_fd, rcv_buf, DOORBELL_NETWORK_MAX_SIZE, 0,
				                   (struct sockaddr *)&db_udp_service->aud_remote, &srvaddr_len);

				if (rcv_len <= 0)
				{
					// close this socket
					LOGE("recv close fd:%d\n", db_udp_service->aud_fd);
					break;
				}
				else
				{
					rcv_len = (rcv_len > DOORBELL_NETWORK_MAX_SIZE) ? DOORBELL_NETWORK_MAX_SIZE : rcv_len;
					rcv_buf[rcv_len] = 0;

					doorbell_transmission_unpack(db_udp_service->aud_channel, rcv_buf, rcv_len, doorbell_udp_voice_receiver);

				}
			}
		}
	}

out:

	LOGE("doorbell_udp_service_main exit %d\n", db_udp_service->running);
	media_app_transfer_close();

#if (defined(CONFIG_DVP_CAMERA) || defined(CONFIG_USB_UVC))

	media_app_camera_close(APP_CAMERA_DVP_JPEG);

	media_app_camera_close(APP_CAMERA_UVC_MJPEG);
#endif

#if AUDIO_TRANSFER_ENABLE
	/* deinit audio transfer driver */
	bk_aud_intf_drv_deinit();
#endif

	if (rcv_buf)
	{
		os_free(rcv_buf);
		rcv_buf = NULL;
	}

	if (db_udp_service->img_fd != -1)
	{
		close(db_udp_service->img_fd);
		db_udp_service->img_fd = -1;
	}

	if (db_udp_service->aud_fd != -1)
	{
		close(db_udp_service->aud_fd);
		db_udp_service->aud_fd = -1;
	}

	GLOBAL_INT_DISABLE();
	db_udp_service->img_status = 0;
	db_udp_service->running = 0;
	db_udp_service->aud_status = 0;
	GLOBAL_INT_RESTORE();

	db_udp_service->thd = NULL;
	rtos_delete_thread(NULL);
}




bk_err_t doorbell_udp_service_init(void)
{
	int ret;

	LOGI("%s\n", __func__);

	if (db_udp_service != NULL)
	{
		LOGE("db_udp_service already init\n");
		return BK_FAIL;
	}

	db_udp_service = os_malloc(sizeof(db_udp_service_t));

	if (db_udp_service == NULL)
	{
		LOGE("db_udp_service malloc failed\n");
		goto error;
	}

	os_memset(db_udp_service, 0, sizeof(db_udp_service_t));

	db_udp_service->img_channel = doorbell_transmission_malloc(1600, DOORBELL_NETWORK_MAX_SIZE);

	if (db_udp_service->img_channel == NULL)
	{
		LOGE("img_channel malloc failed\n");
		goto error;
	}

	db_udp_service->aud_channel = doorbell_transmission_malloc(1600, DOORBELL_NETWORK_MAX_SIZE);

	if (db_udp_service->aud_channel == NULL)
	{
		LOGE("aud_channel malloc failed\n");
		goto error;
	}

	doorbell_devices_set_camera_transfer_callback(&doorbell_udp_img_channel);

	doorbell_devices_set_audio_transfer_callback(&doorbell_udp_aud_channel);

	ret = rtos_create_thread(&db_udp_service->thd,
	                         4,
	                         "doorbell",
	                         (beken_thread_function_t)doorbell_udp_service_main,
	                         1024 * 2,
	                         (beken_thread_arg_t)NULL);
	if (ret != BK_OK)
	{
		LOGE("Error: Failed to create doorbell udp service: %d\n", ret);
		return BK_FAIL;
	}

	LOGI("db_udp_service->img_channel %p\n", db_udp_service->img_channel);
	LOGI("db_udp_service->aud_channel %p\n", db_udp_service->aud_channel);

	return BK_OK;
error:

	if (db_udp_service->aud_channel)
	{
		os_free(db_udp_service->aud_channel);
	}

	if (db_udp_service->img_channel)
	{
		os_free(db_udp_service->img_channel);
	}

	if (db_udp_service)
	{
		os_free(db_udp_service);
	}

	return BK_FAIL;
}

void doorbell_udp_service_deinit(void)
{
	GLOBAL_INT_DECLARATION();

	LOGI("%s\n", __func__);

	if (db_udp_service == NULL)
	{
		LOGE("%s service is NULL\n", __func__);
		return;
	}

	if (db_udp_service->aud_channel)
	{
		os_free(db_udp_service->aud_channel);
		db_udp_service->aud_channel = NULL;
	}

	if (db_udp_service->img_channel)
	{
		os_free(db_udp_service->img_channel);
		db_udp_service->img_channel = NULL;
	}

	GLOBAL_INT_DISABLE();
	db_udp_service->running == 0;
	GLOBAL_INT_RESTORE();

	while (db_udp_service->thd)
	{
		rtos_delay_milliseconds(10);
	}

	os_free(db_udp_service);
	db_udp_service = NULL;
}
