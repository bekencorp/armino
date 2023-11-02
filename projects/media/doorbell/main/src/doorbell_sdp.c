#include <common/bk_include.h>
#include "cli.h"
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include <driver/int.h>
#include <common/bk_err.h>

#include "lwip/sockets.h"
#include "lwip/udp.h"
#include "net.h"
#include "string.h"
#include <components/netif.h>

#include "doorbell_comm.h"
#include "doorbell_sdp.h"

#define TAG "doorbell-sdp"

#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


typedef struct
{
	int sock;
	beken_timer_t timer;
	UINT32 intval_ms;
	struct sockaddr_in sock_remote;
	char *sdp_data;
	uint16_t sdp_length;
} doorbell_sdp_t;


extern void ap_set_default_netif(void);
extern uint8_t uap_ip_start_flag;


doorbell_sdp_t *doorbell_sdp = NULL;


static int doorbell_sdp_init_socket(int *fd_ptr, UINT16 local_port)
{
	struct sockaddr_in l_socket;
	int so_broadcast = 1, sock = -1;

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		LOGE("Socket error\n");
		goto sta_udp_exit;
	}

	l_socket.sin_family = AF_INET;
	l_socket.sin_port = htons(local_port);
	l_socket.sin_addr.s_addr = htonl(INADDR_ANY);
	os_memset(&(l_socket.sin_zero), 0, sizeof(l_socket.sin_zero));

	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &so_broadcast, sizeof(so_broadcast)) != 0)
	{
		LOGE("notify socket setsockopt error\n");
		goto sta_udp_exit;
	}

	if (bind(sock, (struct sockaddr *)&l_socket, sizeof(l_socket)) != 0)
	{
		LOGE(" notify socket bind error\n");
		goto sta_udp_exit;
	}

	if (fd_ptr)
	{
		*fd_ptr = sock;
	}

	return 0;

sta_udp_exit:
	if ((sock) != -1)
	{
		closesocket(sock);
		sock = -1;
	}

	return -1;
}


int doorbell_sdp_generate(const char *name, uint32_t cmd_port, uint32_t img_port, uint32_t aud_port)
{
#define ADV_ALLOC_LEN    (1024)

	uint32 adv_len = ADV_ALLOC_LEN;
	const char *adv_temp =                                                                      \
	        "{\"service\":\"%s\", \"cmd\":\"%d\",\"img\":\"%d\", \"aud\":\"%d\"}";

	if (doorbell_sdp->sdp_data == NULL)
	{
		doorbell_sdp->sdp_data = os_malloc(adv_len);

		if (doorbell_sdp->sdp_data == NULL)
		{
			LOGE("no memory\r\n");
			return BK_FAIL;
		}
	}

	os_memset(doorbell_sdp->sdp_data, 0, adv_len);

	if (uap_ip_is_start())
	{
		LOGI("%s, ap mode\n", __func__);
	}
	else if (sta_ip_is_start())
	{
		LOGI("%s, sta mode\n", __func__);
	}

	sprintf(doorbell_sdp->sdp_data, adv_temp,
	        name,
	        cmd_port,
	        img_port,
	        aud_port);

	doorbell_sdp->sdp_length = strlen(doorbell_sdp->sdp_data);

	LOGI("adv_data:%s,%d\r\n", doorbell_sdp->sdp_data, doorbell_sdp->sdp_length);

	return 0;
}


static void doorbell_sdp_timer_handler(void *data)
{
	if (doorbell_sdp == NULL)
	{
		LOGE("doorbell_sdp NULL return");
		return;
	}

	if (doorbell_sdp->sdp_data == NULL
	    || doorbell_sdp->sdp_data == NULL)
	{
		LOGE("doorbell_sdp sdp data NULL return");
		return;
	}

	if (uap_ip_start_flag == 1)
	{
		ap_set_default_netif();
	}

	LOGD("sdp: %s\n", doorbell_sdp->sdp_data);
	sendto(doorbell_sdp->sock,
	       doorbell_sdp->sdp_data,
	       doorbell_sdp->sdp_length,
	       0,
	       (struct sockaddr *)&doorbell_sdp->sock_remote,
	       sizeof(struct sockaddr));

}

int doorbell_sdp_start_timer(UINT32 time_ms)
{
	if (doorbell_sdp)
	{
		int err;
		UINT32 org_ms = doorbell_sdp->intval_ms;

		if (org_ms != 0)
		{
			if ((org_ms != time_ms))
			{
				if (doorbell_sdp->timer.handle != NULL)
				{
					err = rtos_deinit_timer(&doorbell_sdp->timer);
					if (kNoErr != err)
					{
						LOGE("deinit time fail\r\n");
						return kGeneralErr;
					}
					doorbell_sdp->timer.handle = NULL;
				}
			}
			else
			{
				LOGE("timer aready start\r\n");
				return kNoErr;
			}
		}

		err = rtos_init_timer(&doorbell_sdp->timer,
		                      time_ms,
		                      doorbell_sdp_timer_handler,
		                      NULL);
		if (kNoErr != err)
		{
			LOGE("init timer fail\r\n");
			return kGeneralErr;
		}
		doorbell_sdp->intval_ms = time_ms;

		err = rtos_start_timer(&doorbell_sdp->timer);
		if (kNoErr != err)
		{
			LOGE("start timer fail\r\n");
			return kGeneralErr;
		}
		LOGI("doorbell_sdp_start_timer\r\n");

		return kNoErr;
	}
	return kInProgressErr;
}

int doorbell_sdp_stop_timer(void)
{
	if (doorbell_sdp)
	{
		int err;

		err = rtos_stop_timer(&doorbell_sdp->timer);
		if (kNoErr != err)
		{
			LOGE("stop time fail\r\n");
			return kGeneralErr;
		}

		return kNoErr;
	}
	return kInProgressErr;
}

int doorbell_sdp_pub_deinit(void)
{
	LOGI("doorbell_sdp_deint\r\n");

	if (doorbell_sdp != NULL)
	{
		int err;
		if (doorbell_sdp->timer.handle != NULL)
		{
			err = rtos_deinit_timer(&doorbell_sdp->timer);
			if (kNoErr != err)
			{
				LOGE("deinit time fail\r\n");
				return kGeneralErr;
			}
		}

		closesocket(doorbell_sdp->sock);

		os_free(doorbell_sdp);
		doorbell_sdp = NULL;

		LOGI("doorbell_sdp_deint ok\r\n");
		return kNoErr;
	}

	return kInProgressErr;
}




int doorbell_sdp_start(const char *name, uint32_t cmd_port, uint32_t img_port, uint32_t aud_port)
{
	LOGI("doorbell_sdp_start\r\n");
	int ret = 0;

	if (doorbell_sdp == NULL)
	{
		doorbell_sdp = os_malloc(sizeof(doorbell_sdp_t));

		if (doorbell_sdp == NULL)
		{
			LOGE("malloc fail\r\n");
			return BK_FAIL;
		}
	}
	else
	{
		LOGE("already init?\r\n");
		return BK_FAIL;
	}

	os_memset(doorbell_sdp, 0, sizeof(doorbell_sdp_t));

	doorbell_sdp_generate(name, cmd_port, img_port, aud_port);

	if (doorbell_sdp_init_socket(&doorbell_sdp->sock, UDP_SDP_LOCAL_PORT) != 0)
	{
		LOGE("socket fail\r\n");
		doorbell_sdp = NULL;
		return kGeneralErr;
	}

	doorbell_sdp->sock_remote.sin_family = AF_INET;
	doorbell_sdp->sock_remote.sin_port = htons(UDP_SDP_REMOTE_PORT);
	doorbell_sdp->sock_remote.sin_addr.s_addr = INADDR_BROADCAST;

	if (doorbell_sdp_start_timer(1000) != kNoErr)
	{
		ret = -5;
		goto sdp_int_err;
	}

	LOGI("done\r\n");

	return 0;

sdp_int_err:
	doorbell_sdp_pub_deinit();

	return ret;
}

int doorbell_sdp_stop(void)
{
	LOGI("%s start\n", __func__);

	if (doorbell_sdp_stop_timer() != kNoErr)
	{
		return -1;
	}

	if (doorbell_sdp_pub_deinit() != kNoErr)
	{
		return -2;
	}

	LOGI("%s done\n", __func__);

	return 0;
}
