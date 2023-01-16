#include <common/bk_include.h>

#include "doorbell.h"

#include "lwip/tcp.h"
#include "bk_uart.h"
#include <os/mem.h>
#include <os/os.h>
#include <os/str.h>
#include <common/bk_kernel_err.h>

#include "lwip/sockets.h"

#include <components/video_types.h>

#include "media_app.h"
#include "lcd_act.h"

#define TAG "doorbell-UDP-Client"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


static int doorbell_udp_cli_video_fd = -1;
static uint8_t doorbell_udp_cli_run = 0;
static beken_thread_t doorbell_udp_cli_hdl = NULL;
static struct sockaddr_in *doorbell_udp_cli_remote = NULL;
static uint32_t video_pkt_seq = 0;
static uint8_t camera_type = APP_CAMERA_INVALIED;
static media_ppi_t camera_ppi = PPI_640X480;
static media_ppi_t lcd_ppi = PPI_480X272;
static char lcd_name[10] = {'0'};


static void doorbell_udp_cli_connected(void)
{
	//app_demo_softap_send_msg(DAP_APP_CONECTED, 0);
}

static void doorbell_udp_cli_disconnected(void)
{
	//app_demo_softap_send_msg(DAP_APP_DISCONECTED, 0);
}

int doorbell_udp_cli_send_packet(uint8_t *data, uint32_t len)
{
	int send_byte = 0;

	send_byte = sendto(doorbell_udp_cli_video_fd, data, len, 0,
	                   (struct sockaddr *)doorbell_udp_cli_remote, sizeof(struct sockaddr_in));

	if (send_byte < 0)
	{
		/* err */
		//LOGE("send return fd:%d\n", send_byte);
		send_byte = 0;
	}

	return send_byte;
}

static void demo_doorbell_udp_client_main(beken_thread_arg_t data)
{
	LOGI("%s entry\n", __func__);
	(void)(data);


	doorbell_udp_cli_remote = (struct sockaddr_in *)os_malloc(sizeof(struct sockaddr_in));
	if (!doorbell_udp_cli_remote)
	{
		LOGE("udp os_malloc failed\n");
		goto out;
	}

	// for data transfer
	doorbell_udp_cli_video_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (doorbell_udp_cli_video_fd == -1)
	{
		LOGE("socket failed\n");
		goto out;
	}

	doorbell_udp_cli_remote->sin_family = AF_INET;
	doorbell_udp_cli_remote->sin_port = htons(DEMO_DOORBELL_UDP_IMG_PORT);
	doorbell_udp_cli_remote->sin_addr.s_addr = inet_addr("192.168.0.1");

	{
		video_setup_t setup;

		setup.open_type = TVIDEO_OPEN_SCCB;
		setup.send_type = TVIDEO_SND_UDP;
		setup.send_func = doorbell_udp_cli_send_packet;
		setup.start_cb = doorbell_udp_cli_connected;
		setup.end_cb = doorbell_udp_cli_disconnected;

		setup.pkt_header_size = sizeof(media_hdr_t);
		setup.add_pkt_header = NULL;

		if (camera_type == APP_CAMERA_DVP_JPEG)
		{
			media_app_camera_open(APP_CAMERA_DVP_JPEG, camera_ppi);
		}
		else if (camera_type == APP_CAMERA_UVC_MJPEG)
		{
			media_app_camera_open(APP_CAMERA_UVC_MJPEG, camera_ppi);
		}
		else
		{
			goto out;
		}

		media_app_transfer_open(&setup);

		lcd_open_t lcd_open;
		lcd_open.device_ppi = lcd_ppi;
		lcd_open.device_name = lcd_name;
		media_app_lcd_open(&lcd_open);
	}

	doorbell_udp_cli_run = 1;

	while (doorbell_udp_cli_run)
	{
		rtos_delay_milliseconds(500);
	}

out:

	LOGE("demo_doorbell_udp_main exit %d\n", doorbell_udp_cli_run);

	media_app_transfer_close();

	if (camera_type == APP_CAMERA_DVP_JPEG)
	{
		media_app_camera_close(APP_CAMERA_DVP_JPEG);
	}
	else if (camera_type == APP_CAMERA_UVC_MJPEG)
	{
		media_app_camera_close(APP_CAMERA_UVC_MJPEG);
	}

	video_pkt_seq = 0;

	if (doorbell_udp_cli_remote)
	{
		os_free(doorbell_udp_cli_remote);
		doorbell_udp_cli_remote = NULL;
	}

	if (doorbell_udp_cli_video_fd != -1)
	{
		close(doorbell_udp_cli_video_fd);
		doorbell_udp_cli_video_fd = -1;
	}

	doorbell_udp_cli_run = 0;

	doorbell_udp_cli_hdl = NULL;
	rtos_delete_thread(NULL);
}


bk_err_t demo_doorbell_udp_client_init(int argc, char **argv)
{
	int ret;

	if (argc == 0)
	{
		camera_type = APP_CAMERA_DVP_JPEG;
		camera_ppi = PPI_640X480;
		lcd_ppi = PPI_480X272;
	}
	else if (argc == 1)
	{
		if (os_strcmp(argv[0], "uvc") == 0)
		{
			camera_type = APP_CAMERA_UVC;
		}
		else
		{
			camera_type = APP_CAMERA_DVP_JPEG;
		}
		camera_ppi = PPI_640X480;
		lcd_ppi = PPI_480X272;
	}
	else if (argc == 2)
	{
		if (os_strcmp(argv[0], "uvc") == 0)
		{
			camera_type = APP_CAMERA_UVC;
		}
		else
		{
			camera_type = APP_CAMERA_DVP_JPEG;
		}

		camera_ppi = get_string_to_ppi(argv[1]);
		if (camera_ppi == PPI_DEFAULT)
		{
			camera_ppi = PPI_640X480;
		}
	}
	else if (argc == 3)
	{
		if (os_strcmp(argv[0], "uvc") == 0)
		{
			camera_type = APP_CAMERA_UVC;
		}
		else
		{
			camera_type = APP_CAMERA_DVP_JPEG;
		}

		camera_ppi = get_string_to_ppi(argv[1]);
		if (camera_ppi == PPI_DEFAULT)
		{
			camera_ppi = PPI_640X480;
		}

		lcd_ppi = get_string_to_ppi(argv[2]);
		if (lcd_ppi == PPI_DEFAULT)
		{
			lcd_ppi = PPI_480X272;
		}
	}
	else
	{
		if (os_strcmp(argv[0], "uvc") == 0)
		{
			camera_type = APP_CAMERA_UVC;
		}
		else
		{
			camera_type = APP_CAMERA_DVP_JPEG;
		}

		camera_ppi = get_string_to_ppi(argv[1]);
		if (camera_ppi == PPI_DEFAULT)
		{
			camera_ppi = PPI_640X480;
		}

		lcd_ppi = get_string_to_ppi(argv[2]);
		if (lcd_ppi == PPI_DEFAULT)
		{
			lcd_ppi = PPI_480X272;
		}

		os_memcpy(lcd_name, argv[3], os_strlen(argv[3]));
	}

	LOGI("%s\n", __func__);
	if (!doorbell_udp_cli_hdl)
	{
		ret = rtos_create_thread(&doorbell_udp_cli_hdl,
		                         4,
		                         "doorbell_cli",
		                         (beken_thread_function_t)demo_doorbell_udp_client_main,
		                         1024 * 4,
		                         (beken_thread_arg_t)NULL);
		if (ret != kNoErr)
		{
			LOGE("Error: Failed to create spidma_intfer: %d\n", ret);
			return kGeneralErr;
		}
	}

	return kNoErr;
}

void demo_doorbell_udp_cli_deinit(void)
{
	GLOBAL_INT_DECLARATION();

	LOGI("app_demo_udp_deinit\n");
	if (doorbell_udp_cli_run == 0)
	{
		return;
	}

	GLOBAL_INT_DISABLE();
	doorbell_udp_cli_run = 0;
	GLOBAL_INT_RESTORE();

	while (doorbell_udp_cli_hdl)
	{
		rtos_delay_milliseconds(10);
	}
}

