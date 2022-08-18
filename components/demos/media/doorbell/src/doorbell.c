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

#include "doorbell.h"
#include "bk_uart.h"
#include <os/mem.h>
#include <components/video_transfer.h>
#include <driver/dma.h>
#include <modules/audio_ring_buff.h>
#if CONFIG_AUD_INTF_VER_OLD
#include <components/audio_transfer.h>
#include <components/audio_transfer_types.h>
#endif

#if CONFIG_AUD_INTF_VER_NEW
#include <components/aud_intf.h>
#include <components/aud_intf_types.h>
#endif


#include "media_app.h"

#define DBD_TAG       "doorbell"


#define DOORBELL_DEBUG_LOG              1
#if DOORBELL_DEBUG_LOG
#define DBD os_printf
#define DBW os_printf
#define DBE os_printf
#else
#define DBD(...)
#define DBW(...)
#define DBE(...)
#endif

#define AUDIO_TRANSFER_ENABLE           1

#define APP_DEMO_UDP_RCV_BUF_LEN        1472
#define APP_DEMO_UDP_SOCKET_TIMEOUT     100  // ms

int demo_doorbell_udp_img_fd = -1;
volatile int demo_doorbell_udp_romote_connected = 0;
volatile int demo_doorbell_udp_run = 0;
beken_thread_t demo_doorbell_udp_hdl = NULL;
struct sockaddr_in *demo_doorbell_remote = NULL;
int demo_doorbell_udp_voice_romote_connected = 0;

struct sockaddr_in *demo_doorbell_voice_udp_remote = NULL;

int demo_doorbell_udp_voice_fd = -1;

#if AUDIO_TRANSFER_ENABLE
#if CONFIG_AUD_INTF_VER_OLD
static audio_tras_setup_t aud_tras_setup;
#endif
#if CONFIG_AUD_INTF_VER_NEW
static aud_intf_drv_setup_t aud_intf_drv_setup;
static aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;
static aud_intf_voc_setup_t aud_voc_setup;
#endif
#endif


static uint8_t camera_type = 0;

extern void delay(int num);

int demo_doorbell_udp_send_packet(uint8_t *data, uint32_t len)
{
	int send_byte = 0;

	if (!demo_doorbell_udp_romote_connected)
	{
		return 0;
	}

	send_byte = sendto(demo_doorbell_udp_img_fd, data, len, MSG_DONTWAIT | MSG_MORE,
	                   (struct sockaddr *)demo_doorbell_remote, sizeof(struct sockaddr_in));

	if (send_byte < 0)
	{
		/* err */
		//DBD("send return fd:%d\r\n", send_byte);
		send_byte = 0;
	}

	return send_byte;
}

#if DEMO_DOORBELL_EN_VOICE_TRANSFER
int demo_doorbell_udp_voice_send_packet(UINT8 *data, UINT32 len)
{
	int send_byte = 0;

	if (!demo_doorbell_udp_voice_romote_connected)
	{
		return 0;
	}

	send_byte = sendto(demo_doorbell_udp_voice_fd, data, len, MSG_DONTWAIT | MSG_MORE,
	                   (struct sockaddr *)demo_doorbell_voice_udp_remote, sizeof(struct sockaddr_in));

	if (send_byte < 0)
	{
		/* err */
		//LWIP_UDP_PRT("send return fd:%d\r\n", send_byte);
		send_byte = 0;
	}

	return send_byte;
}
#endif //DEMO_DOORBELL_EN_VOICE_TRANSFER

void demo_doorbell_add_pkt_header(video_packet_t *param)
{
	media_hdr_t *elem_tvhdr = (media_hdr_t *)param->ptk_ptr;

	elem_tvhdr->id = (UINT8)param->frame_id;
	elem_tvhdr->is_eof = param->is_eof;
	elem_tvhdr->pkt_cnt = param->frame_len;
	elem_tvhdr->size = 0;

#if SUPPORT_TIANZHIHENG_DRONE
	elem_tvhdr->unused = 0;
#endif
}

static void demo_doorbell_udp_handle_cmd_data(UINT8 *data, UINT16 len)
{
	UINT32 param = 0;
	UINT32 cmd = (UINT32)data[0] << 24 | (UINT32)data[1] << 16 | (UINT32)data[2] << 8 | data[3];
	if (len >= 8)
	{
		param = (UINT32)data[4] << 24 | (UINT32)data[5] << 16 | (UINT32)data[6] << 8 | data[7];
	}

	DBD("doorbell cmd: %08X, param: %d, len: %d\n", cmd, param, len);

	switch (cmd)
	{
#if AUDIO_TRANSFER_ENABLE
		case AUDIO_CLOSE:
#if CONFIG_AUD_INTF_VER_OLD
			audio_tras_deinit();
#endif
#if CONFIG_AUD_INTF_VER_NEW
			bk_aud_intf_voc_deinit();
			aud_work_mode = AUD_INTF_WORK_MODE_NULL;
			bk_aud_intf_set_mode(aud_work_mode);
			bk_aud_intf_drv_deinit();
#endif
			break;

		case AUDIO_OPEN:
#if CONFIG_AUD_INTF_VER_OLD
			//aud_tras_setup.mode = AUD_TRAS_DRV_MODE_CPU0;
			aud_tras_setup.aec_enable = true;
			aud_tras_setup.audio_send_mic_data = demo_doorbell_udp_voice_send_packet;
			audio_tras_init(aud_tras_setup);
#endif
#if CONFIG_AUD_INTF_VER_NEW
			aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
			aud_intf_drv_setup.task_config.priority = 3;
			aud_intf_drv_setup.aud_intf_rx_spk_data = NULL;
			aud_intf_drv_setup.aud_intf_tx_mic_data = demo_doorbell_udp_voice_send_packet;
			bk_aud_intf_drv_init(&aud_intf_drv_setup);
			aud_work_mode = AUD_INTF_WORK_MODE_VOICE;
			bk_aud_intf_set_mode(aud_work_mode);
			aud_voc_setup.aec_enable = true;
			aud_voc_setup.samp_rate = AUD_INTF_VOC_SAMP_RATE_8K;
			aud_voc_setup.data_type = AUD_INTF_VOC_DATA_TYPE_G711A;
			//aud_voc_setup.data_type = AUD_INTF_VOC_DATA_TYPE_PCM;
			aud_voc_setup.mic_gain = 0x2d;
			aud_voc_setup.spk_gain = 0x2d;
			bk_aud_intf_voc_init(aud_voc_setup);
#endif
			break;
#endif	//AUDIO_TRANSFER_ENABLE

		case DISPLAY_CLOSE:
			break;

		case DISPLAY_OPEN:
			break;

		default:
			break;
	}
}

static void demo_doorbell_udp_app_connected(void)
{
	//app_demo_softap_send_msg(DAP_APP_CONECTED, 0);
}

static void demo_doorbell_udp_app_disconnected(void)
{
	//app_demo_softap_send_msg(DAP_APP_DISCONECTED, 0);
}


static void demo_doorbell_udp_receiver(UINT8 *data, UINT32 len, struct sockaddr_in *demo_doorbell_remote)
{
	DBD("demo_doorbell_udp_receiver\r\n");

	GLOBAL_INT_DECLARATION();

	if (len < 2)
	{
		return;
	}

	if (data[0] == DOORBELL_CMD_IMG_HEADER)
	{

		switch (data[1])
		{
			case DOORBELL_DVP_START:
			{
				UINT8 *src_ipaddr = (UINT8 *)&demo_doorbell_remote->sin_addr.s_addr;
				DBD("src_ipaddr: %d.%d.%d.%d\r\n", src_ipaddr[0], src_ipaddr[1],
				    src_ipaddr[2], src_ipaddr[3]);
				DBD("udp connect to new port:%d\r\n", demo_doorbell_remote->sin_port);

				GLOBAL_INT_DISABLE();
				demo_doorbell_udp_romote_connected = 1;
				GLOBAL_INT_RESTORE();

				video_setup_t setup;

				setup.open_type = TVIDEO_OPEN_SCCB;
				setup.send_type = TVIDEO_SND_UDP;
				setup.send_func = demo_doorbell_udp_send_packet;
				setup.start_cb = demo_doorbell_udp_app_connected;
				setup.end_cb = demo_doorbell_udp_app_disconnected;

				setup.pkt_header_size = sizeof(media_hdr_t);
				setup.add_pkt_header = demo_doorbell_add_pkt_header;

				if (!camera_type)
					media_app_camera_open(APP_CAMERA_DVP, PPI_DEFAULT);
				else
					media_app_camera_open(APP_CAMERA_UVC, PPI_DEFAULT);
				media_app_transfer_video_open(&setup);
			}
			break;

			case DOORBELL_DVP_STOP:
			{
				GLOBAL_INT_DISABLE();
				demo_doorbell_udp_romote_connected = 0;
				GLOBAL_INT_RESTORE();

				media_app_transfer_video_close();
				if (!camera_type)
					media_app_camera_close(APP_CAMERA_DVP);
				else
					media_app_camera_close(APP_CAMERA_UVC);
			}
			break;

#if 0
			case DOORBELL_UVC_START:
			{
				UINT8 *src_ipaddr = (UINT8 *)&demo_doorbell_remote->sin_addr.s_addr;
				DBD("src_ipaddr: %d.%d.%d.%d\r\n", src_ipaddr[0], src_ipaddr[1],
				    src_ipaddr[2], src_ipaddr[3]);
				DBD("udp connect to new port:%d\r\n", demo_doorbell_remote->sin_port);

				GLOBAL_INT_DISABLE();
				demo_doorbell_udp_romote_connected = 1;
				GLOBAL_INT_RESTORE();

				video_setup_t setup;

				setup.open_type = TVIDEO_OPEN_SCCB;
				setup.send_type = TVIDEO_SND_UDP;
				setup.send_func = demo_doorbell_udp_send_packet;
				setup.start_cb = demo_doorbell_udp_app_connected;
				setup.end_cb = demo_doorbell_udp_app_disconnected;

				setup.pkt_header_size = sizeof(media_hdr_t);
				setup.add_pkt_header = demo_doorbell_add_pkt_header;

				media_app_camera_open(APP_CAMERA_UVC, PPI_DEFAULT);
				media_app_transfer_video_open(&setup);
			}
			break;

			case DOORBELL_UVC_STOP:
			{
				GLOBAL_INT_DISABLE();
				demo_doorbell_udp_romote_connected = 0;
				GLOBAL_INT_RESTORE();

				media_app_transfer_video_close();
				media_app_camera_close(APP_CAMERA_UVC);
			}
			break;
#endif
		}


	}

}

#if DEMO_DOORBELL_EN_VOICE_TRANSFER
static void demo_doorbell_udp_voice_receiver(UINT8 *data, UINT32 len, struct sockaddr_in *udp_remote)
{
	//DBD("demo_doorbell_udp_voice_receiver, data len=%d \r\n", len);
#if AUDIO_TRANSFER_ENABLE
	bk_err_t ret = BK_OK;

	if (len > 0)
	{
		demo_doorbell_udp_voice_romote_connected = 1;
	}

	//DBD("len: %d \r\n", len);
#if CONFIG_AUD_INTF_VER_OLD
	ret = audio_tras_write_spk_data(data, len);
#endif
#if CONFIG_AUD_INTF_VER_NEW
	ret = bk_aud_intf_write_spk_data(data, len);
#endif
	if (ret != BK_OK)
	{
		DBD("write speaker data fial \r\n", len);
	}
#endif  //AUDIO_TRANSFER_ENABLE

}
#endif // DEMO_DOORBELL_EN_VOICE_TRANSFER

static void demo_doorbell_udp_main(beken_thread_arg_t data)
{
	GLOBAL_INT_DECLARATION();
	int maxfd, udp_cmd_fd = -1, ret = 0;
	int rcv_len = 0;
	struct sockaddr_in cmd_remote;
	socklen_t srvaddr_len = 0;
	fd_set watchfd;
	struct timeval timeout;
	u8 *rcv_buf = NULL;

	DBD("demo_doorbell_udp_main entry\r\n");
	(void)(data);

	rcv_buf = (u8 *) os_malloc((APP_DEMO_UDP_RCV_BUF_LEN + 1) * sizeof(u8));
	if (!rcv_buf)
	{
		DBD("udp os_malloc failed\r\n");
		goto app_udp_exit;
	}

	demo_doorbell_remote = (struct sockaddr_in *)os_malloc(sizeof(struct sockaddr_in));
	if (!demo_doorbell_remote)
	{
		DBD("udp os_malloc failed\r\n");
		goto app_udp_exit;
	}

	// for data transfer
	demo_doorbell_udp_img_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (demo_doorbell_udp_img_fd == -1)
	{
		DBD("socket failed\r\n");
		goto app_udp_exit;
	}

	demo_doorbell_remote->sin_family = AF_INET;
	demo_doorbell_remote->sin_port = htons(DEMO_DOORBELL_UDP_IMG_PORT);
	demo_doorbell_remote->sin_addr.s_addr = htonl(INADDR_ANY);

	srvaddr_len = (socklen_t)sizeof(struct sockaddr_in);
	if (bind(demo_doorbell_udp_img_fd, (struct sockaddr *)demo_doorbell_remote, srvaddr_len) == -1)
	{
		DBD("bind failed\r\n");
		goto app_udp_exit;
	}

	//  for recv uart cmd
	udp_cmd_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_cmd_fd == -1)
	{
		DBD("socket failed\r\n");
		goto app_udp_exit;
	}

	cmd_remote.sin_family = AF_INET;
	cmd_remote.sin_port = htons(DEMO_DOORBELL_UDP_CMD_PORT);
	cmd_remote.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(udp_cmd_fd, (struct sockaddr *)&cmd_remote, srvaddr_len) == -1)
	{
		DBD("bind failed\r\n");
		goto app_udp_exit;
	}

	maxfd = (udp_cmd_fd > demo_doorbell_udp_img_fd) ? udp_cmd_fd : demo_doorbell_udp_img_fd;

	// for voice transfer
#if DEMO_DOORBELL_EN_VOICE_TRANSFER
	demo_doorbell_voice_udp_remote = (struct sockaddr_in *)os_malloc(sizeof(struct sockaddr_in));
	if (!demo_doorbell_voice_udp_remote)
	{
		DBD("udp os_malloc failed\r\n");
		goto app_udp_exit;
	}

	demo_doorbell_udp_voice_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (demo_doorbell_udp_voice_fd == -1)
	{
		DBD("vo socket failed\r\n");
		goto app_udp_exit;
	}

	demo_doorbell_voice_udp_remote->sin_family = AF_INET;
	demo_doorbell_voice_udp_remote->sin_port = htons(DEMO_DOORBELL_UDP_VOICE_PORT);
	demo_doorbell_voice_udp_remote->sin_addr.s_addr = htonl(INADDR_ANY);

	srvaddr_len = (socklen_t)sizeof(struct sockaddr_in);
	if (bind(demo_doorbell_udp_voice_fd, (struct sockaddr *)demo_doorbell_voice_udp_remote, srvaddr_len) == -1)
	{
		DBD("bind failed\r\n");
		goto app_udp_exit;
	}
	maxfd = (maxfd > demo_doorbell_udp_voice_fd) ? maxfd : demo_doorbell_udp_voice_fd;
#endif // DEMO_DOORBELL_EN_VOICE_TRANSFER

	timeout.tv_sec = APP_DEMO_UDP_SOCKET_TIMEOUT / 1000;
	timeout.tv_usec = (APP_DEMO_UDP_SOCKET_TIMEOUT % 1000) * 1000;

	GLOBAL_INT_DISABLE();
	demo_doorbell_udp_romote_connected = 0;
	demo_doorbell_udp_run = 1;
#if DEMO_DOORBELL_EN_VOICE_TRANSFER
	demo_doorbell_udp_voice_romote_connected = 0;
#endif
	GLOBAL_INT_RESTORE();

	while (demo_doorbell_udp_run)
	{
		FD_ZERO(&watchfd);
		FD_SET(demo_doorbell_udp_img_fd, &watchfd);
		FD_SET(udp_cmd_fd, &watchfd);
#if DEMO_DOORBELL_EN_VOICE_TRANSFER
		FD_SET(demo_doorbell_udp_voice_fd, &watchfd);
#endif

		//DBD("wait for select\r\n");
		ret = select(maxfd + 1, &watchfd, NULL, NULL, &timeout);
		if (ret < 0)
		{
			DBD("select ret:%d\r\n", ret);
			break;
		}
		else
		{
			// is img fd, data transfer
			if (FD_ISSET(demo_doorbell_udp_img_fd, &watchfd))
			{
				rcv_len = recvfrom(demo_doorbell_udp_img_fd, rcv_buf, APP_DEMO_UDP_RCV_BUF_LEN, 0,
				                   (struct sockaddr *)demo_doorbell_remote, &srvaddr_len);

				if (rcv_len <= 0)
				{
					// close this socket
					DBD("recv close fd:%d\r\n", demo_doorbell_udp_img_fd);
					break;
				}
				else
				{
					rcv_len = (rcv_len > APP_DEMO_UDP_RCV_BUF_LEN) ? APP_DEMO_UDP_RCV_BUF_LEN : rcv_len;
					rcv_buf[rcv_len] = 0;

					DBD("demo_doorbell_udp_receiver");
					demo_doorbell_udp_receiver(rcv_buf, rcv_len, demo_doorbell_remote);
				}
			}
			else if (FD_ISSET(udp_cmd_fd, &watchfd))
			{
				rcv_len = recvfrom(udp_cmd_fd, rcv_buf, APP_DEMO_UDP_RCV_BUF_LEN, 0,
				                   (struct sockaddr *)&cmd_remote, &srvaddr_len);

				if (rcv_len <= 0)
				{
					// close this socket
					DBD("recv close fd:%d\r\n", udp_cmd_fd);
					break;
				}
				else
				{
					rcv_len = (rcv_len > APP_DEMO_UDP_RCV_BUF_LEN) ? APP_DEMO_UDP_RCV_BUF_LEN : rcv_len;
					rcv_buf[rcv_len] = 0;

					demo_doorbell_udp_handle_cmd_data(rcv_buf, rcv_len);
				}
			}
#if DEMO_DOORBELL_EN_VOICE_TRANSFER
			if (FD_ISSET(demo_doorbell_udp_voice_fd, &watchfd))
			{
				rcv_len = recvfrom(demo_doorbell_udp_voice_fd, rcv_buf, APP_DEMO_UDP_RCV_BUF_LEN, 0,
				                   (struct sockaddr *)demo_doorbell_voice_udp_remote, &srvaddr_len);

				if (rcv_len <= 0)
				{
					// close this socket
					DBD("recv close fd:%d\r\n", demo_doorbell_udp_voice_fd);
					break;
				}
				else
				{
					rcv_len = (rcv_len > APP_DEMO_UDP_RCV_BUF_LEN) ? APP_DEMO_UDP_RCV_BUF_LEN : rcv_len;
					rcv_buf[rcv_len] = 0;
					demo_doorbell_udp_voice_receiver(rcv_buf, rcv_len, demo_doorbell_voice_udp_remote);
				}
			}
#endif // DEMO_DOORBELL_EN_VOICE_TRANSFER
		}
	}

app_udp_exit:

	DBE("demo_doorbell_udp_main exit %d\r\n", demo_doorbell_udp_run);
	media_app_transfer_video_close();

	media_app_camera_close(APP_CAMERA_DVP);

	media_app_camera_close(APP_CAMERA_UVC);

#if AUDIO_TRANSFER_ENABLE
	/* deinit audio transfer driver */

#if CONFIG_AUD_INTF_VER_OLD
	audio_tras_deinit();
#endif

#if CONFIG_AUD_INTF_VER_NEW
	bk_aud_intf_drv_deinit();
#endif

#endif

	if (rcv_buf)
	{
		os_free(rcv_buf);
		rcv_buf = NULL;
	}

#if DEMO_DOORBELL_EN_VOICE_TRANSFER
	if (demo_doorbell_udp_voice_fd != -1)
	{
		close(demo_doorbell_udp_voice_fd);
		demo_doorbell_udp_voice_fd = -1;
	}

	if (demo_doorbell_voice_udp_remote)
	{
		os_free(demo_doorbell_voice_udp_remote);
		demo_doorbell_voice_udp_remote = NULL;
	}
#endif // DEMO_DOORBELL_EN_VOICE_TRANSFER
	if (demo_doorbell_remote)
	{
		os_free(demo_doorbell_remote);
		demo_doorbell_remote = NULL;
	}

	if (demo_doorbell_udp_img_fd != -1)
	{
		close(demo_doorbell_udp_img_fd);
		demo_doorbell_udp_img_fd = -1;
	}

	if (udp_cmd_fd != -1)
	{
		close(udp_cmd_fd);
		udp_cmd_fd = -1;
	}

	camera_type = 0;

	GLOBAL_INT_DISABLE();
	demo_doorbell_udp_romote_connected = 0;
	demo_doorbell_udp_run = 0;
#if DEMO_DOORBELL_EN_VOICE_TRANSFER
	demo_doorbell_udp_voice_romote_connected = 0;
#endif
	GLOBAL_INT_RESTORE();

	demo_doorbell_udp_hdl = NULL;
	rtos_delete_thread(NULL);
}

UINT32 demo_doorbell_udp_init(void)
{
	int ret;

	DBD("app_demo_udp_init\r\n");
	if (!demo_doorbell_udp_hdl)
	{
		ret = rtos_create_thread(&demo_doorbell_udp_hdl,
		                         4,
		                         "app_udp",
		                         (beken_thread_function_t)demo_doorbell_udp_main,
		                         1024,
		                         (beken_thread_arg_t)NULL);
		if (ret != kNoErr)
		{
			DBD("Error: Failed to create spidma_intfer: %d\r\n", ret);
			return kGeneralErr;
		}
	}

	return kNoErr;
}

void demo_doorbell_udp_deinit(void)
{
	GLOBAL_INT_DECLARATION();

	DBD("app_demo_udp_deinit\r\n");
	if (demo_doorbell_udp_run == 0)
	{
		return;
	}

	GLOBAL_INT_DISABLE();
	demo_doorbell_udp_run = 0;
	GLOBAL_INT_RESTORE();

	while (demo_doorbell_udp_hdl)
	{
		rtos_delay_milliseconds(10);
	}
}

void cli_doorbell_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	camera_type = os_strtoul(argv[2], NULL, 10) & 0xF;
	demo_doorbell_udp_init();
}

