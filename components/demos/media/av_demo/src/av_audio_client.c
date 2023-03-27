#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include "bk_cli.h"
#include "bk_fake_clock.h"
#include <lwip/sockets.h>
#include <stdlib.h>
#include <components/system.h>
#include "av_audio.h"
#include "av_audio_common.h"
#include <components/aud_intf.h>

#define TAG "av_audio_client"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

static int sock = -1;
static struct sockaddr_in server;

static int av_udp_voice_send_packet(unsigned char *data, unsigned int len)
{
	int send_byte = 0;
	send_byte = sendto(sock, data, len, 0, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
	LOGD("len: %d, send_byte: %d \n", len, send_byte);
	if (send_byte < len)
	{
		/* err */
		LOGE("need_send: %d, send_complete: %d, errno: %d \n", len, send_byte, errno);
		send_byte = 0;
	}

	return send_byte;
}

static int av_tcp_voice_send_packet(unsigned char *data, unsigned int len)
{
	int send_byte = 0;
	send_byte = write(sock, data, len);
	LOGD("len: %d, send_byte: %d \n", len, send_byte);
	if (send_byte < len)
	{
		/* err */
		LOGE("need_send: %d, send_complete: %d, errno: %d \n", len, send_byte, errno);
		send_byte = 0;
	}

	return send_byte;
}

static void av_voice_udp_client_main(void *thread_param)
{
	bk_err_t ret = BK_OK;
	uint32_t *buffer;
	int r_size;
	int sender_len;
	av_aud_voc_setup_t aud_setup;

	buffer = os_malloc(AV_BUFSZ);
	if (buffer == NULL)
		goto udp_client_exit;
	os_memset(buffer, 0x00, AV_BUFSZ);

	aud_setup.av_aud_voc_send_packet = av_udp_voice_send_packet;
	aud_voc_start(aud_setup);

	while (1) {
		sock = socket(PF_INET, SOCK_DGRAM, 0);
		if (sock < 0) {
			LOGE("av_udp_client: create socket failed, err=%d!\n", errno);
		}

		server.sin_family = PF_INET;
		server.sin_port = htons(AV_PORT);
		server.sin_addr.s_addr = inet_addr("192.168.0.1");
		LOGI("av_udp_client run...\n");

		while (1) {
			r_size = recvfrom(sock, buffer, AV_BUFSZ, 0, (struct sockaddr *)&server, (socklen_t *)&sender_len);
			if (r_size > 0) {
				LOGD("Rx data from server, r_size: %d \r\n", r_size);
				ret = bk_aud_intf_write_spk_data((uint8_t *)buffer, r_size);
				if (ret != BK_OK)
				{
					LOGE("write speaker data fial\n", r_size);
				}
			} else {
				// close this socket
				LOGI("recv close fd:%d, rcv_len:%d\n", sock, r_size);
				close(sock);
				sock = -1;

				/* close audio */
				aud_voc_stop();

				goto udp_client_exit;
			}
		}
	}

udp_client_exit:
	if (buffer) {
		os_free(buffer);
		buffer = NULL;
	}

	if (sock >= 0) {
		closesocket(sock);
		sock = -1;
	}

	rtos_delete_thread(NULL);
}

static void av_voice_tcp_client_main(void *thread_param)
{
	bk_err_t ret = BK_OK;
	uint32_t *buffer;
	int r_size;
	av_aud_voc_setup_t aud_setup;

	buffer = os_malloc(AV_BUFSZ);
	if (buffer == NULL)
		goto tcp_client_exit;
	os_memset(buffer, 0x00, AV_BUFSZ);

	aud_setup.av_aud_voc_send_packet = av_tcp_voice_send_packet;

	LOGI("av_tcp_client start \n");

	while (1) {
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0) {
			LOGE("av_tcp_client: create socket failed, err=%d!\n", errno);
		}

		server.sin_family = PF_INET;
		server.sin_port = htons(AV_PORT);
		server.sin_addr.s_addr = inet_addr("192.168.0.1");

		while (1) {
			ret = connect(sock, (struct sockaddr *) &server, sizeof(server));
			if(ret < 0)
			{
				LOGE("connect err: %d\r\n", ret);
				//goto tcp_client_exit;
				rtos_delay_milliseconds(500);
				continue;
			} else {
				LOGI("av_tcp_client connect complete \n");
				break;
			}
		}
		//LOGI("av_tcp_client connect complete \n");

		aud_voc_start(aud_setup);
		LOGI("voice start... \n");

		while (1) {
			r_size = recv(sock, buffer, AV_BUFSZ, 0);
			if (r_size > 0) {
				LOGD("Rx data from server, r_size: %d \r\n", r_size);
				ret = bk_aud_intf_write_spk_data((uint8_t *)buffer, r_size);
				if (ret != BK_OK)
				{
					LOGE("write speaker data fial\n", r_size);
				}
			} else {
				// close this socket
				LOGI("recv close fd:%d, rcv_len:%d\n", sock, r_size);
				close(sock);
				sock = -1;

				/* close audio */
				aud_voc_stop();

				goto tcp_client_exit;
			}
		}
	}

tcp_client_exit:
	if (buffer) {
		os_free(buffer);
		buffer = NULL;
	}

	if (sock >= 0) {
		closesocket(sock);
		sock = -1;
	}

	rtos_delete_thread(NULL);
}

void av_voice_udp_client_init(void)
{
	rtos_create_thread(NULL,
					   THREAD_PROIRITY,
					   "av_udp_client",
					   av_voice_udp_client_main,
					   THREAD_SIZE,
					   (beken_thread_arg_t) 0);
}

void av_voice_tcp_client_init(void)
{
	rtos_create_thread(NULL,
					   THREAD_PROIRITY,
					   "av_tcp_client",
					   av_voice_tcp_client_main,
					   THREAD_SIZE,
					   (beken_thread_arg_t) 0);
}

