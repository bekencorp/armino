#include <common/bk_include.h>

#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include <driver/int.h>
#include <common/bk_err.h>

#include "lwip/sockets.h"

#include "aud_debug_udp.h"
#include "bk_uart.h"
#include <os/mem.h>
#include <modules/audio_ring_buff.h>
#include <components/aud_intf.h>
#include <components/aud_intf_types.h>


#define TAG "aud_debug_udp"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


#define AUD_DEBUG_UDP_RCV_BUF_LEN        1472
#define AUD_DEBUG_UDP_SOCKET_TIMEOUT     100  // ms

static int aud_debug_udp_run = 0;
beken_thread_t aud_debug_udp_hdl = NULL;

struct sockaddr_in *aud_debug_cmd_remote = NULL;
int aud_debug_cmd_fd = -1;

struct sockaddr_in *aud_debug_voc_udp_remote = NULL;
volatile int aud_debug_voc_udp_romote_connected = 0;
int aud_debug_voc_udp_fd = -1;


int bk_aud_debug_voc_udp_send_packet(unsigned char *data, unsigned int len)
{
	int send_byte = 0;

	if (!aud_debug_voc_udp_romote_connected)
	{
		return 0;
	}

	send_byte = sendto(aud_debug_voc_udp_fd, data, len, MSG_DONTWAIT | MSG_MORE,
	                   (struct sockaddr *)aud_debug_voc_udp_remote, sizeof(struct sockaddr_in));

	if (send_byte < 0)
	{
		/* err */
		//LWIP_UDP_PRT("send return fd:%d\n", send_byte);
		send_byte = 0;
	}

	return send_byte;
}

static void aud_debug_udp_handle_cmd_data(uint8_t *data, UINT16 len)
{
	uint32_t param = 0;
	uint32_t cmd = (uint32_t)data[0] << 24 | (uint32_t)data[1] << 16 | (uint32_t)data[2] << 8 | data[3];
	if (len >= 8)
	{
		param = (uint32_t)data[4] << 24 | (uint32_t)data[5] << 16 | (uint32_t)data[6] << 8 | data[7];
	}

	LOGI("aud_debug cmd: %08X, param: %d, len: %d\n", cmd, param, len);
	if (len >= 8)
	{
		switch (cmd)
		{
			case ECHO_DEPTH:
				LOGI("set ECHO_DEPTH: %d\n", param);
				bk_aud_intf_set_aec_para(AUD_INTF_VOC_AEC_EC_DEPTH, param);
				break;

			case MAX_AMPLITUDE:
				LOGI("set MAX_AMPLITUDE: %d\n", param);
				bk_aud_intf_set_aec_para(AUD_INTF_VOC_AEC_TXRX_THR, param);
				break;

			case MIN_AMPLITUDE:
				LOGI("set MIN_AMPLITUDE: %d\n", param);
				bk_aud_intf_set_aec_para(AUD_INTF_VOC_AEC_TXRX_FLR, param);
				break;

			case NOISE_LEVEL:
				LOGI("set NOISE_LEVEL: %d\n", param);
				bk_aud_intf_set_aec_para(AUD_INTF_VOC_AEC_NS_LEVEL, param);
				break;

			case NOISE_PARAM:
				LOGI("set NOISE_PARAM: %d\n", param);
				bk_aud_intf_set_aec_para(AUD_INTF_VOC_AEC_NS_PARA, param);
				break;

			default:
				break;
		}
	}
	else
	{
		LOGI("cmd:0x%x not need to handle\n", cmd);
	}
}

static void aud_debug_udp_main(beken_thread_arg_t data)
{
	GLOBAL_INT_DECLARATION();
	int maxfd, ret = 0;
	int rcv_len = 0;
	socklen_t srvaddr_len = 0;
	fd_set watchfd;
	struct timeval timeout;
	u8 *rcv_buf = NULL;

	LOGI("aud_debug_udp_main entry\n");
	(void)(data);

	rcv_buf = (u8 *) os_malloc((AUD_DEBUG_UDP_RCV_BUF_LEN + 1) * sizeof(u8));
	if (rcv_buf == NULL) {
		LOGE("udp os_malloc failed: rcv_buf \n");
		goto out;
	}

	//  for recv uart cmd
	aud_debug_cmd_remote = (struct sockaddr_in *)os_malloc(sizeof(struct sockaddr_in));
	if (aud_debug_cmd_remote == NULL)
	{
		LOGE("udp os_malloc failed: aud_debug_cmd_remote\n");
		goto out;
	}

	aud_debug_cmd_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (aud_debug_cmd_fd == -1)
	{
		LOGE("socket failed\n");
		goto out;
	}

	aud_debug_cmd_remote->sin_family = AF_INET;
	aud_debug_cmd_remote->sin_port = htons(AUD_DEBUG_UDP_CMD_PORT);
	aud_debug_cmd_remote->sin_addr.s_addr = htonl(INADDR_ANY);

	srvaddr_len = (socklen_t)sizeof(struct sockaddr_in);
	if (bind(aud_debug_cmd_fd, (struct sockaddr *)aud_debug_cmd_remote, srvaddr_len) == -1)
	{
		LOGE("bind failed\n");
		goto out;
	}

	// for dump voice transfer data
	aud_debug_voc_udp_remote = (struct sockaddr_in *)os_malloc(sizeof(struct sockaddr_in));
	if (aud_debug_voc_udp_remote == NULL)
	{
		LOGE("udp os_malloc failed: aud_debug_voc_udp_remote \n");
		goto out;
	}

	aud_debug_voc_udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (aud_debug_voc_udp_fd == -1)
	{
		LOGE("vo socket failed\n");
		goto out;
	}

	aud_debug_voc_udp_remote->sin_family = AF_INET;
	aud_debug_voc_udp_remote->sin_port = htons(AUD_DEBUG_UDP_VOICE_PORT);
	aud_debug_voc_udp_remote->sin_addr.s_addr = htonl(INADDR_ANY);

	srvaddr_len = (socklen_t)sizeof(struct sockaddr_in);
	if (bind(aud_debug_voc_udp_fd, (struct sockaddr *)aud_debug_voc_udp_remote, srvaddr_len) == -1)
	{
		LOGE("bind failed\n");
		goto out;
	}
	maxfd = (aud_debug_cmd_fd > aud_debug_voc_udp_fd) ? aud_debug_cmd_fd : aud_debug_voc_udp_fd;

	timeout.tv_sec = AUD_DEBUG_UDP_SOCKET_TIMEOUT / 1000;
	timeout.tv_usec = (AUD_DEBUG_UDP_SOCKET_TIMEOUT % 1000) * 1000;

	GLOBAL_INT_DISABLE();
	aud_debug_udp_run = 1;
	aud_debug_voc_udp_romote_connected = 1;
	GLOBAL_INT_RESTORE();

	while (aud_debug_udp_run)
	{
		FD_ZERO(&watchfd);
		FD_SET(aud_debug_cmd_fd, &watchfd);

		//DBD("wait for select\n");
		ret = select(maxfd + 1, &watchfd, NULL, NULL, &timeout);
		if (ret < 0)
		{
			LOGE("select ret:%d\n", ret);
			//break;
		}
		else
		{
			if (FD_ISSET(aud_debug_cmd_fd, &watchfd))
			{
				rcv_len = recvfrom(aud_debug_cmd_fd, rcv_buf, AUD_DEBUG_UDP_RCV_BUF_LEN, 0,
				                   (struct sockaddr *)aud_debug_cmd_remote, &srvaddr_len);

				if (rcv_len <= 0)
				{
					// close this socket
					LOGE("recv close fd:%d\n", aud_debug_cmd_fd);
					//break;
				}
				else
				{
					rcv_len = (rcv_len > AUD_DEBUG_UDP_RCV_BUF_LEN) ? AUD_DEBUG_UDP_RCV_BUF_LEN : rcv_len;
					rcv_buf[rcv_len] = 0;

					aud_debug_udp_handle_cmd_data(rcv_buf, rcv_len);
				}
			}
		}
	}

out:

	LOGE("aud_debug_udp_main exit %d\n", aud_debug_udp_run);

	/* deinit audio transfer driver */
	bk_aud_intf_drv_deinit();

	if (rcv_buf)
	{
		os_free(rcv_buf);
		rcv_buf = NULL;
	}

	if (aud_debug_voc_udp_fd != -1)
	{
		close(aud_debug_voc_udp_fd);
		aud_debug_voc_udp_fd = -1;
	}

	if (aud_debug_voc_udp_remote)
	{
		os_free(aud_debug_voc_udp_remote);
		aud_debug_voc_udp_remote = NULL;
	}

	if (aud_debug_cmd_fd != -1)
	{
		close(aud_debug_cmd_fd);
		aud_debug_cmd_fd = -1;
	}

	if (aud_debug_cmd_remote)
	{
		os_free(aud_debug_cmd_remote);
		aud_debug_cmd_remote = NULL;
	}

	GLOBAL_INT_DISABLE();
	aud_debug_udp_run = 0;
	aud_debug_voc_udp_romote_connected = 0;
	GLOBAL_INT_RESTORE();

	aud_debug_udp_hdl = NULL;
	rtos_delete_thread(NULL);
}

uint32_t bk_aud_debug_udp_init(void)
{
	int ret;

	LOGI("aud_debug_udp_init\n");
	if (!aud_debug_udp_hdl)
	{
		ret = rtos_create_thread(&aud_debug_udp_hdl,
		                         4,
		                         "aud_debug",
		                         (beken_thread_function_t)aud_debug_udp_main,
		                         1024 * 2,
		                         (beken_thread_arg_t)NULL);
		if (ret != kNoErr)
		{
			LOGE("Error: Failed to create spidma_intfer: %d\n", ret);
			return kGeneralErr;
		}
	}

	return kNoErr;
}

bk_err_t bk_aud_debug_udp_deinit(void)
{
	GLOBAL_INT_DECLARATION();

	LOGI("aud_debug_udp_deinit\n");
	if (aud_debug_udp_run == 0)
	{
		return BK_OK;
	}

	GLOBAL_INT_DISABLE();
	aud_debug_udp_run = 0;
	GLOBAL_INT_RESTORE();

	while (aud_debug_udp_hdl)
	{
		rtos_delay_milliseconds(10);
	}

	return BK_OK;
}
