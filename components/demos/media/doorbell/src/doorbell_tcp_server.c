#include <common/bk_include.h>

#include "doorbell.h"

#include "lwip/tcp.h"
#include "bk_uart.h"
#include <os/mem.h>
#include <os/os.h>
#include <os/str.h>
#include <common/bk_kernel_err.h>
#include <components/video_types.h>


#include "lwip/sockets.h"

#include "lcd_act.h"

#include "media_app.h"


#define TAG "doorbell-TCP-Server"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

beken_thread_t doorbell_tcp_ser_hdl = NULL;
uint8_t doorbell_tcp_ser_run = 0;
struct sockaddr_in *doorbell_tcp_ser_remote = NULL;
int doorbell_tcp_ser_video_fd = -1;
static media_ppi_t camera_ppi = PPI_640X480;
static media_ppi_t lcd_ppi = PPI_480X272;
static char lcd_name[10] = {'0'};
static int cli_sockfd = -1;

extern uint32_t bk_net_send_data(uint8_t *data, uint32_t length, video_send_type_t type);

static void demo_doorbell_tcp_server_main(beken_thread_arg_t data)
{
	int rcv_len = 0;
	//  struct sockaddr_in server;
	bk_err_t ret = BK_OK;
	u8 *rcv_buf = NULL;
	fd_set watchfd;

	LOGI("%s entry\n", __func__);
	(void)(data);

	rcv_buf = (u8 *) os_malloc((APP_DEMO_TCP_RCV_BUF_LEN + 1) * sizeof(u8));
	if (!rcv_buf)
	{
		LOGE("tcp os_malloc failed\n");
		goto out;
	}

	doorbell_tcp_ser_remote = (struct sockaddr_in *)os_malloc(sizeof(struct sockaddr_in));
	if (!doorbell_tcp_ser_remote)
	{
		LOGE("doorbell_udp_ser_remote os_malloc failed\n");
		goto out;
	}

	// for data transfer
	doorbell_tcp_ser_video_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (doorbell_tcp_ser_video_fd == -1)
	{
		LOGE("socket failed\n");
		goto out;
	}

	doorbell_tcp_ser_remote->sin_family = AF_INET;
	doorbell_tcp_ser_remote->sin_port = htons(DEMO_DOORBELL_UDP_IMG_PORT);
	doorbell_tcp_ser_remote->sin_addr.s_addr = inet_addr("0.0.0.0");

	if (bind(doorbell_tcp_ser_video_fd, (struct sockaddr *)doorbell_tcp_ser_remote, sizeof(struct sockaddr_in)) == -1)
	{
		LOGE("bind failed\n");
		goto out;
	}

	if (listen(doorbell_tcp_ser_video_fd, 0) == -1)
	{
		LOGE("listen failed\n");
		goto out;
	}

	LOGI("%s: start listen \n", __func__);

	while (1) {
		FD_ZERO(&watchfd);
		FD_SET(doorbell_tcp_ser_video_fd, &watchfd);

		LOGI("select fd\n");
		ret = select(doorbell_tcp_ser_video_fd + 1, &watchfd, NULL, NULL, NULL);
		if (ret <= 0)
		{
			LOGE("select ret:%d\n", ret);
			continue;
		}
		else
		{
			// is new connection
			if (FD_ISSET(doorbell_tcp_ser_video_fd, &watchfd))
			{
				struct sockaddr_in client_addr;
				socklen_t cliaddr_len = 0;

				cliaddr_len = sizeof(client_addr);
				cli_sockfd = accept(doorbell_tcp_ser_video_fd, (struct sockaddr *)&client_addr, &cliaddr_len);
				if (cli_sockfd < 0)
				{
					LOGE("accept return fd:%d\n", cli_sockfd);
					break;
				}

				LOGI("new accept fd:%d\n", cli_sockfd);

				//tcp_set_keepalive(cli_sockfd);
				break;
			}
		}
	}
	LOGI("connect client complete \n");

	{
		media_app_camera_open(APP_CAMERA_NET_MJPEG, camera_ppi);

		lcd_open_t lcd_open;
		lcd_open.device_ppi = lcd_ppi;
		lcd_open.device_name = lcd_name;
		LOGI("%s, lcd_ppi:%d-%d\r\n", __func__, lcd_ppi >> 16, lcd_ppi & 0xFFFF);
		media_app_lcd_open(&lcd_open);
	}

	doorbell_tcp_ser_run = 1;

	while (doorbell_tcp_ser_run)
	{
		rcv_len = recv(cli_sockfd, rcv_buf, APP_DEMO_TCP_RCV_BUF_LEN, 0);
		if (rcv_len > 0)
		{
			bk_net_send_data(rcv_buf, rcv_len, TVIDEO_SND_TCP);
		}
		else
		{
			// close this socket
			LOGI("recv close fd:%d, rcv_len:%d\n", cli_sockfd, rcv_len);
			close(cli_sockfd);
			cli_sockfd = -1;

			media_app_lcd_close();

			media_app_camera_close(APP_CAMERA_NET_MJPEG);

			goto out;
		}

	}

out:

	LOGE("%s exit %d\n", __func__, doorbell_tcp_ser_run);

	if (rcv_buf)
	{
		os_free(rcv_buf);
		rcv_buf = NULL;
	}

	if (doorbell_tcp_ser_remote)
	{
		os_free(doorbell_tcp_ser_remote);
		doorbell_tcp_ser_remote = NULL;
	}

	if (doorbell_tcp_ser_video_fd != -1)
	{
		close(doorbell_tcp_ser_video_fd);
		doorbell_tcp_ser_video_fd = -1;
	}

	doorbell_tcp_ser_run = 0;

	doorbell_tcp_ser_hdl = NULL;
	rtos_delete_thread(NULL);
}


bk_err_t demo_doorbell_tcp_server_init(int argc, char **argv)
{
	int ret;

	if (argc == 0)
	{
		camera_ppi = PPI_640X480;
		lcd_ppi = PPI_480X272;
	}
	else if (argc == 1)
	{
		camera_ppi = get_string_to_ppi(argv[0]);
		if (camera_ppi == PPI_DEFAULT)
		{
			camera_ppi = PPI_640X480;
		}

		lcd_ppi = PPI_480X272;
	}
	else if (argc == 2)
	{
		camera_ppi = get_string_to_ppi(argv[0]);
		if (camera_ppi == PPI_DEFAULT)
		{
			camera_ppi = PPI_640X480;
		}

		lcd_ppi = get_string_to_ppi(argv[1]);
		if (lcd_ppi == PPI_DEFAULT)
		{
			lcd_ppi = PPI_480X272;
		}
	}
	else if (argc == 3)
	{
		camera_ppi = get_string_to_ppi(argv[0]);
		if (camera_ppi == PPI_DEFAULT)
		{
			camera_ppi = PPI_640X480;
		}

		lcd_ppi = get_string_to_ppi(argv[1]);
		if (lcd_ppi == PPI_DEFAULT)
		{
			lcd_ppi = PPI_480X272;
		}

		os_memcpy(lcd_name, argv[2], os_strlen(argv[2]));
	}
	else
	{
		camera_ppi = PPI_640X480;
		lcd_ppi = PPI_480X272;
	}

	LOGI("%s\n", __func__);
	if (!doorbell_tcp_ser_hdl)
	{
		ret = rtos_create_thread(&doorbell_tcp_ser_hdl,
		                         4,
		                         "doorbell_tcp_ser",
		                         (beken_thread_function_t)demo_doorbell_tcp_server_main,
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

void demo_doorbell_tcp_server_deinit(void)
{
	GLOBAL_INT_DECLARATION();

	LOGI("%s\n", __func__);

	if (doorbell_tcp_ser_run == 0)
	{
		return;
	}

	GLOBAL_INT_DISABLE();
	doorbell_tcp_ser_run = 0;
	GLOBAL_INT_RESTORE();

	while (doorbell_tcp_ser_hdl)
	{
		rtos_delay_milliseconds(10);
	}
}

