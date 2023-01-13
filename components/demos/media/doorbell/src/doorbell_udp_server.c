#include <common/bk_include.h>

#include "doorbell.h"

#include "lwip/tcp.h"
#include "bk_uart.h"
#include <os/mem.h>
#include <os/os.h>
#include <os/str.h>
#include <common/bk_kernel_err.h>

#include "lwip/sockets.h"

#include "lcd_act.h"

#include "media_app.h"


#define TAG "doorbell-UDP-Server"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

beken_thread_t doorbell_udp_ser_hdl = NULL;
uint8_t doorbell_udp_ser_run = 0;
struct sockaddr_in *doorbell_udp_ser_remote = NULL;
int doorbell_udp_ser_video_fd = -1;
static media_ppi_t lcd_ppi = PPI_480X272;
static char lcd_name[10] = {'0'};


extern uint32_t bk_net_camera_receive_data(uint8_t *data, uint32_t length);

static void demo_doorbell_udp_server_main(beken_thread_arg_t data)
{
	int rcv_len = 0;
	socklen_t srvaddr_len = 0;
//	struct sockaddr_in server;
	struct sockaddr_in sender;

	u8 *rcv_buf = NULL;

	LOGI("%s entry\n", __func__);
	(void)(data);

	rcv_buf = (u8 *) os_malloc((APP_DEMO_RCV_BUF_LEN + 1) * sizeof(u8));
	if (!rcv_buf)
	{
		LOGE("udp os_malloc failed\n");
		goto out;
	}

	doorbell_udp_ser_remote = (struct sockaddr_in *)os_malloc(sizeof(struct sockaddr_in));
	if (!doorbell_udp_ser_remote)
	{
		LOGE("doorbell_udp_ser_remote os_malloc failed\n");
		goto out;
	}

	// for data transfer
	doorbell_udp_ser_video_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (doorbell_udp_ser_video_fd == -1)
	{
		LOGE("socket failed\n");
		goto out;
	}

	doorbell_udp_ser_remote->sin_family = AF_INET;
	doorbell_udp_ser_remote->sin_port = htons(DEMO_DOORBELL_UDP_IMG_PORT);
	doorbell_udp_ser_remote->sin_addr.s_addr = inet_addr("0.0.0.0");

	sender.sin_family = AF_INET;
	sender.sin_port = htons(DEMO_DOORBELL_UDP_IMG_PORT);
	sender.sin_addr.s_addr = inet_addr("192.168.0.100");

	srvaddr_len = (socklen_t)sizeof(struct sockaddr_in);
	if (bind(doorbell_udp_ser_video_fd, (struct sockaddr *)doorbell_udp_ser_remote, srvaddr_len) == -1)
	{
		LOGE("bind failed\n");
		goto out;
	}
	else
	{
		media_app_camera_open(APP_CAMERA_NET_MJPEG, PPI_1280X720);

		lcd_open_t lcd_open;
		lcd_open.device_ppi = lcd_ppi;
		lcd_open.device_name = lcd_name;
		LOGI("%s, lcd_ppi:%d-%d\r\n", __func__, lcd_ppi >> 16, lcd_ppi & 0xFFFF);
		media_app_lcd_open(&lcd_open);
	}

	doorbell_udp_ser_run = 1;

	while (doorbell_udp_ser_run)
	{
		rcv_len = recvfrom(doorbell_udp_ser_video_fd, rcv_buf, APP_DEMO_RCV_BUF_LEN, 0,
				                   (struct sockaddr *)&sender, &srvaddr_len);
		if (rcv_len > 0)
			bk_net_camera_receive_data(rcv_buf, rcv_len);
		else
			LOGE("recvfrom:%d\r\n", rcv_len);
	}

out:

	LOGE("%s exit %d\n", __func__, doorbell_udp_ser_run);

	media_app_lcd_close();

	media_app_camera_close(APP_CAMERA_NET_MJPEG);

	if (rcv_buf)
	{
		os_free(rcv_buf);
		rcv_buf = NULL;
	}

	if (doorbell_udp_ser_remote)
	{
		os_free(doorbell_udp_ser_remote);
		doorbell_udp_ser_remote = NULL;
	}

	if (doorbell_udp_ser_video_fd != -1)
	{
		close(doorbell_udp_ser_video_fd);
		doorbell_udp_ser_video_fd = -1;
	}

	doorbell_udp_ser_run = 0;

	doorbell_udp_ser_hdl = NULL;
	rtos_delete_thread(NULL);
}


bk_err_t demo_doorbell_udp_server_init(int argc, char **argv)
{
	int ret;

	if (argc == 0)
	{
		lcd_ppi = PPI_480X272;
	}
	else if (argc == 1)
	{
		LOGI("%s, %s\n", __func__, argv[0]);
		lcd_ppi = get_string_to_ppi(argv[0]);
		if (lcd_ppi == PPI_DEFAULT)
		{
			lcd_ppi = PPI_480X272;
		}

		LOGI("%s, lcd_ppi:%d-%d\r\n", __func__, lcd_ppi >> 16, lcd_ppi & 0xFFFF);
	}
	else if (argc == 2)
	{
		lcd_ppi = get_string_to_ppi(argv[0]);
		if (lcd_ppi == PPI_DEFAULT)
		{
			lcd_ppi = PPI_480X272;
		}

		os_memcpy(lcd_name, argv[1], os_strlen(argv[1]));
	}
	else
	{
		lcd_ppi = PPI_480X272;
	}

	LOGI("%s\n", __func__);
	if (!doorbell_udp_ser_hdl)
	{
		ret = rtos_create_thread(&doorbell_udp_ser_hdl,
		                         4,
		                         "doorbell_ser",
		                         (beken_thread_function_t)demo_doorbell_udp_server_main,
		                         1024 * 2,
		                         (beken_thread_arg_t)NULL);
		if (ret != kNoErr)
		{
			LOGE("Error: Failed to create doorbell_ser: %d\n", ret);
			return kGeneralErr;
		}
	}

	return kNoErr;
}

void demo_doorbell_udp_server_deinit(void)
{
	GLOBAL_INT_DECLARATION();

	LOGI("%s\n", __func__);

	if (doorbell_udp_ser_run == 0)
	{
		return;
	}

	GLOBAL_INT_DISABLE();
	doorbell_udp_ser_run = 0;
	GLOBAL_INT_RESTORE();

	while (doorbell_udp_ser_hdl)
	{
		rtos_delay_milliseconds(10);
	}
}

