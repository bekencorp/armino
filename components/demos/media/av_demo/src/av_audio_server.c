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
#include <components/aud_intf.h>
#include "av_audio_common.h"


#define TAG "av_audio_server"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

static struct sockaddr_in server;
static struct sockaddr_in sender;
static int sock;
static int new_cli_sockfd = -1;

static int av_udp_voice_send_packet(unsigned char *data, unsigned int len)
{
	int send_byte = 0;
	send_byte = sendto(sock, data, len, 0, (struct sockaddr *)&sender, sizeof(struct sockaddr_in));
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
	send_byte = write(new_cli_sockfd, data, len);
	LOGD("len: %d, send_byte: %d \n", len, send_byte);
	if (send_byte < len)
	{
		/* err */
		LOGE("need_send: %d, send_complete: %d, errno: %d \n", len, send_byte, errno);
		send_byte = 0;
	}

	return send_byte;
}

static void av_voice_udp_server_main(void *thread_param)
{
	uint32_t *buffer;
	int sender_len, r_size = 0;
	bk_err_t ret = BK_OK;
	av_aud_voc_setup_t aud_setup;

	buffer = os_malloc(AV_BUFSZ);
	if (buffer == NULL)
		return;
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		LOGE("can't create socket!! exit\n");
		goto udp_server_exit;
	}
	server.sin_family = PF_INET;
	server.sin_port = htons(AV_PORT);
	server.sin_addr.s_addr = inet_addr("0.0.0.0");

	sender.sin_family = PF_INET;
	sender.sin_port = htons(AV_PORT);
	sender.sin_addr.s_addr = inet_addr("192.168.0.100");
	LOGI("av_udp_server run...\n");

	if (bind(sock, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) < 0) {
		LOGE("av_udp_server bind failed!! exit\n");
		goto udp_server_exit;
	}

	aud_setup.av_aud_voc_send_packet = av_udp_voice_send_packet;
	aud_voc_start(aud_setup);

	while (1) {
		r_size = recvfrom(sock, buffer, AV_BUFSZ, 0, (struct sockaddr *)&sender, (socklen_t *)&sender_len);
		if (r_size > 0) {
			LOGD("Rx data from client, r_size: %d \r\n", r_size);

			ret = bk_aud_intf_write_spk_data((uint8_t *)buffer, r_size);
			if (ret != BK_OK)
			{
				LOGE("write speaker data fial\n");
			}
		} else {
			// close this socket
			LOGI("recv close fd:%d, rcv_len:%d\n", sock, r_size);
			close(sock);
			sock = -1;

			/* close audio */
			aud_voc_stop();

			goto udp_server_exit;
		}
	}

udp_server_exit:
	if (sock >= 0)
		closesocket(sock);

	if (buffer) {
		os_free(buffer);
		buffer = NULL;
	}

	rtos_delete_thread(NULL);
}

static void tcp_set_keepalive(int fd)
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

static void av_voice_tcp_server_main(void *thread_param)
{
	uint32_t *buffer;
	int r_size = 0;
	bk_err_t ret = BK_OK;
	fd_set watchfd;
	av_aud_voc_setup_t aud_setup;

	buffer = os_malloc(AV_BUFSZ);
	if (buffer == NULL)
		return;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		LOGE("can't create socket!! exit\n");
		goto tcp_server_exit;
	}
	server.sin_family = PF_INET;
	server.sin_port = htons(AV_PORT);
	server.sin_addr.s_addr = inet_addr("0.0.0.0");

	if (bind(sock, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) < 0) {
		LOGE("av_udp_server bind failed!! exit\n");
		goto tcp_server_exit;
	}

	if (listen(sock, 0) == -1)
	{
		LOGE("listen failed\n");
		goto tcp_server_exit;
	}

	aud_setup.av_aud_voc_send_packet = av_tcp_voice_send_packet;
	LOGI("%s: start listen \n", __func__);

	while (1) {
		FD_ZERO(&watchfd);
		FD_SET(sock, &watchfd);

		LOGI("select fd\n");
		ret = select(sock + 1, &watchfd, NULL, NULL, NULL);
		if (ret <= 0)
		{
			LOGE("select ret:%d\n", ret);
			continue;
		}
		else
		{
			// is new connection
			if (FD_ISSET(sock, &watchfd))
			{
				struct sockaddr_in client_addr;
				socklen_t cliaddr_len = 0;

				cliaddr_len = sizeof(client_addr);
				new_cli_sockfd = accept(sock, (struct sockaddr *)&client_addr, &cliaddr_len);
				if (new_cli_sockfd < 0)
				{
					LOGE("accept return fd:%d\n", new_cli_sockfd);
					break;
				}

				LOGI("new accept fd:%d\n", new_cli_sockfd);

				tcp_set_keepalive(new_cli_sockfd);
				break;
			}
		}
	}
	LOGI("connect client complete \n");

	aud_voc_start(aud_setup);
	LOGI("voice start... \n");

	while(1) {
		r_size = recv(new_cli_sockfd, buffer, AV_BUFSZ, 0);
		if (r_size > 0) {
			LOGD("Rx data from server, r_size: %d \r\n", r_size);
			ret = bk_aud_intf_write_spk_data((uint8_t *)buffer, r_size);
			if (ret != BK_OK)
			{
				LOGE("write speaker data fial\n");
			}
		} else {
			// close this socket
			LOGI("recv close fd:%d, rcv_len:%d\n", new_cli_sockfd, r_size);
			close(new_cli_sockfd);
			new_cli_sockfd = -1;

			/* close audio */
			aud_voc_stop();

			goto tcp_server_exit;
		}
	}

tcp_server_exit:
	if (sock >= 0)
		closesocket(sock);

	if (buffer) {
		os_free(buffer);
		buffer = NULL;
	}

	rtos_delete_thread(NULL);
}


void av_voice_udp_server_init(void)
{
	rtos_create_thread(NULL,
					   THREAD_PROIRITY,
					   "av_udp_server",
					   av_voice_udp_server_main,
					   THREAD_SIZE,
					   (beken_thread_arg_t) 0);
}

void av_voice_tcp_server_init(void)
{
	rtos_create_thread(NULL,
					   THREAD_PROIRITY,
					   "av_tcp_server",
					   av_voice_tcp_server_main,
					   THREAD_SIZE,
					   (beken_thread_arg_t) 0);
}

