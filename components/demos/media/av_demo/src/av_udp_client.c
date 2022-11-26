#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include "bk_cli.h"
#include "bk_fake_clock.h"
#include <lwip/sockets.h>
#include <stdlib.h>
#include <components/system.h>
#include "av_udp.h"
#include "av_udp_common.h"
#include <components/aud_intf.h>

#define TAG "av_udp_client"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

static int sock;
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

static void av_udp_client_main(void *thread_param)
{
	bk_err_t ret = BK_OK;
	uint32_t *buffer;
	int r_size;
	int sender_len;
	aud_voc_udp_setup_t aud_setup;

	buffer = os_malloc(1470);
	if (buffer == NULL)
		goto udp_client_exit;
	os_memset(buffer, 0x00, 1470);

	aud_setup.aud_udp_voc_send_packet = av_udp_voice_send_packet;
	aud_voc_start(aud_setup);

	while (1) {
		sock = socket(PF_INET, SOCK_DGRAM, 0);
		if (sock < 0) {
			os_printf("av_udp_client: create socket failed, err=%d!\n", errno);
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
			}
		}
	}

udp_client_exit:
	if (buffer) {
		os_free(buffer);
		buffer = NULL;
	}

	//closesocket(sock);

	rtos_delete_thread(NULL);
}

void av_udp_client_init(void)
{
	rtos_create_thread(NULL,
					   THREAD_PROIRITY,
					   "av_udp_client",
					   av_udp_client_main,
					   THREAD_SIZE,
					   (beken_thread_arg_t) 0);
}

