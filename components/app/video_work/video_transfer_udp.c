#include <common/bk_include.h>

#if (CONFIG_APP_DEMO_VIDEO_TRANSFER)
#include "video_transfer_config.h"

#if (APP_DEMO_CFG_USE_UDP)
//#include "FreeRTOS.h"
//#include "task.h"
#include <os/os.h>
#include <common/bk_kernel_err.h>
#ifdef CONFIG_RTT
#include <sys/socket.h>
#endif
#include "lwip/sockets.h"

#include "video_transfer_udp.h"

#include "bk_uart.h"
#include <os/mem.h>
#include <components/video_transfer.h>
#if APP_DEMO_EN_VOICE_TRANSFER
#if CONFIG_AUD_INTF_VER_OLD
#include <components/audio_transfer.h>
#include <components/audio_transfer_types.h>
#endif
#if CONFIG_AUD_INTF_VER_NEW
#include <components/aud_intf.h>
#include <components/aud_intf_types.h>
#endif
#endif	//APP_DEMO_EN_VOICE_TRANSFER
#include "video_demo_pub.h"

#define APP_DEMO_UDP_DEBUG              1
#if APP_DEMO_UDP_DEBUG
#define APP_DEMO_UDP_PRT                warning_prf
#define APP_DEMO_UDP_WARN               warning_prf
#define APP_DEMO_UDP_FATAL              fatal_prf
#else
#define APP_DEMO_UDP_PRT                null_prf
#define APP_DEMO_UDP_WARN               null_prf
#define APP_DEMO_UDP_FATAL              null_prf
#endif


#define APP_DEMO_UDP_RCV_BUF_LEN        1472
#define APP_DEMO_UDP_SOCKET_TIMEOUT     100  // ms

extern void app_demo_softap_send_msg(u32 new_msg, u32 new_data);

uint32_t g_pkt_send_fail = 0;
uint32_t g_pkt_send = 0;
int app_demo_udp_img_fd = -1;
volatile int app_demo_udp_romote_connected = 0;
volatile int app_demo_udp_run = 0;
beken_thread_t app_demo_udp_hdl = NULL;
struct sockaddr_in *app_demo_remote = NULL;

#if APP_DEMO_EN_VOICE_TRANSFER
volatile int app_demo_udp_voice_romote_connected = 0;
struct sockaddr_in *app_demo_udp_voice_remote = NULL;
int app_demo_udp_voice_fd = -1;
#if CONFIG_AUD_INTF_VER_OLD
static audio_tras_setup_t aud_tras_setup;
#endif
#if CONFIG_AUD_INTF_VER_NEW
static aud_intf_drv_setup_t aud_intf_drv_setup;
static aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;
static aud_intf_voc_setup_t aud_voc_setup;
#endif
#endif	//APP_DEMO_EN_VOICE_TRANSFER

typedef struct tvideo_hdr_st {
	UINT8 id;
	UINT8 is_eof;
	UINT8 pkt_cnt;
	UINT8 size;
#if SUPPORT_TIANZHIHENG_DRONE
	UINT32 unused;
#endif
} HDR_ST, *HDR_PTR;

void app_demo_add_pkt_header(video_packet_t *param)
{
	HDR_PTR elem_tvhdr = (HDR_PTR)param->ptk_ptr;

	elem_tvhdr->id = (UINT8)param->frame_id;
	elem_tvhdr->is_eof = param->is_eof;
	elem_tvhdr->pkt_cnt = param->frame_len;
	elem_tvhdr->size = 0;

#if SUPPORT_TIANZHIHENG_DRONE
	elem_tvhdr->unused = 0;
#endif
}

#if APP_DEMO_EN_VOICE_TRANSFER
int app_demo_udp_voice_send_packet(UINT8 *data, UINT32 len)
{
	int send_byte = 0;

	if (!app_demo_udp_voice_romote_connected)
		return 0;

	send_byte = sendto(app_demo_udp_voice_fd, data, len, MSG_DONTWAIT | MSG_MORE,
					   (struct sockaddr *)app_demo_udp_voice_remote, sizeof(struct sockaddr_in));

	if (send_byte < 0) {
		/* err */
		//LWIP_UDP_PRT("send return fd:%d\r\n", send_byte);
		send_byte = 0;
	}

	return send_byte;
}
#endif //APP_DEMO_EN_VOICE_TRANSFER

static void app_demo_udp_handle_cmd_data(UINT8 *data, UINT16 len)
{
	UINT32 param = 0;
	UINT32 cmd = (UINT32)data[0] << 24 | (UINT32)data[1] << 16 | (UINT32)data[2] << 8 | data[3];

	if (len >= 8)
	{
		param = (UINT32)data[4] << 24 | (UINT32)data[5] << 16 | (UINT32)data[6] << 8 | data[7];
	}

	APP_DEMO_UDP_PRT("doorbell cmd: %08X, param: %d, len: %d\n", cmd, param, len);

	switch (cmd)
	{
#if APP_DEMO_EN_VOICE_TRANSFER
		case 0:
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

		case 1:
#if CONFIG_AUD_INTF_VER_OLD
			aud_tras_setup.aec_enable = true;
			aud_tras_setup.audio_send_mic_data = app_demo_udp_voice_send_packet;
			audio_tras_init(aud_tras_setup);
#endif
#if CONFIG_AUD_INTF_VER_NEW
			aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
			aud_intf_drv_setup.task_config.priority = 3;
			aud_intf_drv_setup.aud_intf_rx_spk_data = NULL;
			aud_intf_drv_setup.aud_intf_tx_mic_data = app_demo_udp_voice_send_packet;
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
#endif

		default:
			break;
	}
}


static void app_demo_udp_app_connected(void)
{
	app_demo_softap_send_msg(DAP_APP_CONECTED, 0);
}

static void app_demo_udp_app_disconnected(void)
{
	app_demo_softap_send_msg(DAP_APP_DISCONECTED, 0);
}

#if CFG_SUPPORT_HTTP_OTA
TV_OTA_ST ota_param = {
	NULL,
	0,
	0
};
static void app_demo_udp_http_ota_handle(char *rev_data)
{
	//if(app_demo_softap_is_ota_doing() == 0)
	//{
	// to do
	//
	//app_demo_softap_send_msg(DAP_START_OTA, (u32)&ota_param);

	//os_memset(&ota_param, 0, sizeof(TV_OTA_ST));
	//}
}
#endif

extern void rwnxl_set_video_transfer_flag(uint32_t video_transfer_flag);

static void app_demo_udp_receiver(UINT8 *data, UINT32 len, struct sockaddr_in *app_demo_remote)
{
	GLOBAL_INT_DECLARATION();

	if (len < 2)
		return;

	if (data[0] == CMD_IMG_HEADER) {
		if (data[1] == CMD_START_IMG) {

			UINT8 *src_ipaddr = (UINT8 *)&app_demo_remote->sin_addr.s_addr;
			APP_DEMO_UDP_PRT("src_ipaddr: %d.%d.%d.%d\r\n", src_ipaddr[0], src_ipaddr[1],
							 src_ipaddr[2], src_ipaddr[3]);
			APP_DEMO_UDP_PRT("udp connect to new port:%d\r\n", app_demo_remote->sin_port);

			GLOBAL_INT_DISABLE();
			app_demo_udp_romote_connected = 1;
			GLOBAL_INT_RESTORE();

#if (CONFIG_SPIDMA || CONFIG_CAMERA)
			video_setup_t setup;

			setup.open_type = TVIDEO_OPEN_SCCB;
			setup.send_type = TVIDEO_SND_UDP;
			setup.send_func = app_demo_udp_send_packet;
			setup.start_cb = app_demo_udp_app_connected;
			setup.end_cb = app_demo_udp_app_disconnected;

			setup.pkt_header_size = sizeof(HDR_ST);
			setup.add_pkt_header = app_demo_add_pkt_header;

			rwnxl_set_video_transfer_flag(true);

			bk_video_transfer_init(&setup);
#endif
		} else if (data[1] == CMD_STOP_IMG) {
			APP_DEMO_UDP_PRT("udp close\r\n");

#if (CONFIG_SPIDMA || CONFIG_CAMERA)
			bk_video_transfer_deinit();

			rwnxl_set_video_transfer_flag(false);
#endif

			GLOBAL_INT_DISABLE();
			app_demo_udp_romote_connected = 0;
			GLOBAL_INT_RESTORE();
			app_demo_udp_app_disconnected();
		}
#if CFG_SUPPORT_HTTP_OTA
		else if (data[1] == CMD_START_OTA)
			app_demo_udp_http_ota_handle(&data[2]);
#endif
	}

}

#if APP_DEMO_EN_VOICE_TRANSFER
static void app_demo_udp_voice_receiver(UINT8 *data, UINT32 len, struct sockaddr_in *udp_remote)
{
	bk_err_t ret = BK_OK;

	if (len > 0)
	{
		app_demo_udp_voice_romote_connected = 1;
	}

#if CONFIG_AUD_INTF_VER_OLD
	ret = audio_tras_write_spk_data(data, len);
#endif
#if CONFIG_AUD_INTF_VER_NEW
	ret = bk_aud_intf_write_spk_data(data, len);
#endif
	if (ret != BK_OK)
	{
		os_printf("write speaker data fial \r\n", len);
	}

}
#endif // APP_DEMO_EN_VOICE_TRANSFER

static void app_demo_udp_main(beken_thread_arg_t data)
{
	GLOBAL_INT_DECLARATION();
	int maxfd, udp_cmd_fd = -1, ret = 0;
	int rcv_len = 0;
	struct sockaddr_in cmd_remote;
	socklen_t srvaddr_len = 0;
	fd_set watchfd;
	struct timeval timeout;
	u8 *rcv_buf = NULL;

	APP_DEMO_UDP_FATAL("app_demo_udp_main entry\r\n");
	(void)(data);

	rcv_buf = (u8 *) os_malloc((APP_DEMO_UDP_RCV_BUF_LEN + 1) * sizeof(u8));
	if (!rcv_buf) {
		APP_DEMO_UDP_PRT("udp os_malloc failed\r\n");
		goto app_udp_exit;
	}

	app_demo_remote = (struct sockaddr_in *)os_malloc(sizeof(struct sockaddr_in));
	if (!app_demo_remote) {
		APP_DEMO_UDP_PRT("udp os_malloc failed\r\n");
		goto app_udp_exit;
	}

	// for data transfer
	app_demo_udp_img_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (app_demo_udp_img_fd == -1) {
		APP_DEMO_UDP_PRT("socket failed\r\n");
		goto app_udp_exit;
	}

	app_demo_remote->sin_family = AF_INET;
	app_demo_remote->sin_port = htons(APP_DEMO_UDP_IMG_PORT);
	app_demo_remote->sin_addr.s_addr = htonl(INADDR_ANY);

	srvaddr_len = (socklen_t)sizeof(struct sockaddr_in);
	if (bind(app_demo_udp_img_fd, (struct sockaddr *)app_demo_remote, srvaddr_len) == -1) {
		APP_DEMO_UDP_PRT("bind failed\r\n");
		goto app_udp_exit;
	}

	//  for recv uart cmd
	udp_cmd_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_cmd_fd == -1) {
		APP_DEMO_UDP_PRT("socket failed\r\n");
		goto app_udp_exit;
	}

	cmd_remote.sin_family = AF_INET;
	cmd_remote.sin_port = htons(APP_DEMO_UDP_CMD_PORT);
	cmd_remote.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(udp_cmd_fd, (struct sockaddr *)&cmd_remote, srvaddr_len) == -1) {
		APP_DEMO_UDP_PRT("bind failed\r\n");
		goto app_udp_exit;
	}

	maxfd = (udp_cmd_fd > app_demo_udp_img_fd) ? udp_cmd_fd : app_demo_udp_img_fd;

	// for voice transfer
#if APP_DEMO_EN_VOICE_TRANSFER
	app_demo_udp_voice_remote = (struct sockaddr_in *)os_malloc(sizeof(struct sockaddr_in));
	if (!app_demo_udp_voice_remote) {
		APP_DEMO_UDP_PRT("udp os_malloc failed\r\n");
		goto app_udp_exit;
	}

	app_demo_udp_voice_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (app_demo_udp_voice_fd == -1) {
		APP_DEMO_UDP_PRT("vo socket failed\r\n");
		goto app_udp_exit;
	}

	app_demo_udp_voice_remote->sin_family = AF_INET;
	app_demo_udp_voice_remote->sin_port = htons(APP_DEMO_UDP_VOICE_PORT);
	app_demo_udp_voice_remote->sin_addr.s_addr = htonl(INADDR_ANY);

	srvaddr_len = (socklen_t)sizeof(struct sockaddr_in);
	if (bind(app_demo_udp_voice_fd, (struct sockaddr *)app_demo_udp_voice_remote, srvaddr_len) == -1) {
		APP_DEMO_UDP_PRT("bind failed\r\n");
		goto app_udp_exit;
	}
	maxfd = (maxfd > app_demo_udp_voice_fd) ? maxfd : app_demo_udp_voice_fd;
#endif // APP_DEMO_EN_VOICE_TRANSFER

	timeout.tv_sec = APP_DEMO_UDP_SOCKET_TIMEOUT / 1000;
	timeout.tv_usec = (APP_DEMO_UDP_SOCKET_TIMEOUT % 1000) * 1000;

	GLOBAL_INT_DISABLE();
	app_demo_udp_romote_connected = 0;
	app_demo_udp_run = 1;
#if APP_DEMO_EN_VOICE_TRANSFER
	app_demo_udp_voice_romote_connected = 0;
#endif
	GLOBAL_INT_RESTORE();

	while (app_demo_udp_run) {
		FD_ZERO(&watchfd);
		FD_SET(app_demo_udp_img_fd, &watchfd);
		FD_SET(udp_cmd_fd, &watchfd);
#if APP_DEMO_EN_VOICE_TRANSFER
		FD_SET(app_demo_udp_voice_fd, &watchfd);
#endif

		ret = select(maxfd + 1, &watchfd, NULL, NULL, &timeout);
		if (ret < 0) {
			APP_DEMO_UDP_PRT("select ret:%d\r\n", ret);
			break;
		} else {
			// is img fd, data transfer
			if (FD_ISSET(app_demo_udp_img_fd, &watchfd)) {
				rcv_len = recvfrom(app_demo_udp_img_fd, rcv_buf, APP_DEMO_UDP_RCV_BUF_LEN, 0,
								   (struct sockaddr *)app_demo_remote, &srvaddr_len);

				if (rcv_len <= 0) {
					// close this socket
					APP_DEMO_UDP_PRT("recv close fd:%d\r\n", app_demo_udp_img_fd);
					break;
				} else {
					rcv_len = (rcv_len > APP_DEMO_UDP_RCV_BUF_LEN) ? APP_DEMO_UDP_RCV_BUF_LEN : rcv_len;
					rcv_buf[rcv_len] = 0;

					app_demo_udp_receiver(rcv_buf, rcv_len, app_demo_remote);
				}
			} else if (FD_ISSET(udp_cmd_fd, &watchfd)) {
				rcv_len = recvfrom(udp_cmd_fd, rcv_buf, APP_DEMO_UDP_RCV_BUF_LEN, 0,
								   (struct sockaddr *)&cmd_remote, &srvaddr_len);

				if (rcv_len <= 0) {
					// close this socket
					APP_DEMO_UDP_PRT("recv close fd:%d\r\n", udp_cmd_fd);
					break;
				} else {
					rcv_len = (rcv_len > APP_DEMO_UDP_RCV_BUF_LEN) ? APP_DEMO_UDP_RCV_BUF_LEN : rcv_len;
					rcv_buf[rcv_len] = 0;

					app_demo_udp_handle_cmd_data(rcv_buf, rcv_len);
				}

			}
#if APP_DEMO_EN_VOICE_TRANSFER
			if (FD_ISSET(app_demo_udp_voice_fd, &watchfd)) {
				rcv_len = recvfrom(app_demo_udp_voice_fd, rcv_buf, APP_DEMO_UDP_RCV_BUF_LEN, 0,
								   (struct sockaddr *)app_demo_udp_voice_remote, &srvaddr_len);

				if (rcv_len <= 0) {
					// close this socket
					APP_DEMO_UDP_PRT("recv close fd:%d\r\n", app_demo_udp_voice_fd);
					break;
				} else {
					rcv_len = (rcv_len > APP_DEMO_UDP_RCV_BUF_LEN) ? APP_DEMO_UDP_RCV_BUF_LEN : rcv_len;
					rcv_buf[rcv_len] = 0;

					app_demo_udp_voice_receiver(rcv_buf, rcv_len, app_demo_udp_voice_remote);
				}
			}
#endif // APP_DEMO_EN_VOICE_TRANSFER
		}
	}

app_udp_exit:

	APP_DEMO_UDP_FATAL("app_demo_udp_main exit %d\r\n", app_demo_udp_run);

#if (CONFIG_SPIDMA || CONFIG_CAMERA)
	bk_video_transfer_deinit();

	rwnxl_set_video_transfer_flag(false);
#endif

	if (rcv_buf) {
		os_free(rcv_buf);
		rcv_buf = NULL;
	}

#if APP_DEMO_EN_VOICE_TRANSFER
	if (app_demo_udp_voice_fd != -1) {
		close(app_demo_udp_voice_fd);
		app_demo_udp_voice_fd = -1;
	}

	if (app_demo_udp_voice_remote) {
		os_free(app_demo_udp_voice_remote);
		app_demo_udp_voice_remote = NULL;
	}
#endif // APP_DEMO_EN_VOICE_TRANSFER
	if (app_demo_remote) {
		os_free(app_demo_remote);
		app_demo_remote = NULL;
	}

	if (app_demo_udp_img_fd != -1) {
		close(app_demo_udp_img_fd);
		app_demo_udp_img_fd = -1;
	}

	if (udp_cmd_fd != -1) {
		close(udp_cmd_fd);
		udp_cmd_fd = -1;
	}

	GLOBAL_INT_DISABLE();
	app_demo_udp_romote_connected = 0;
	app_demo_udp_run = 0;
#if APP_DEMO_EN_VOICE_TRANSFER
	app_demo_udp_voice_romote_connected = 0;
#endif
	GLOBAL_INT_RESTORE();

	app_demo_udp_hdl = NULL;
	rtos_delete_thread(NULL);
}

UINT32 app_demo_udp_init(void)
{
	int ret;

	APP_DEMO_UDP_PRT("app_demo_udp_init\r\n");
	if (!app_demo_udp_hdl) {
		ret = rtos_create_thread(&app_demo_udp_hdl,
								 4,
								 "app_udp",
								 (beken_thread_function_t)app_demo_udp_main,
								 1024,
								 (beken_thread_arg_t)NULL);
		if (ret != kNoErr) {
			APP_DEMO_UDP_PRT("Error: Failed to create spidma_intfer: %d\r\n", ret);
			return kGeneralErr;
		}
	}

	return kNoErr;
}

int app_demo_udp_send_packet(uint8_t *data, uint32_t len)
{
	int send_byte = 0;

	if (!app_demo_udp_romote_connected)
		return 0;

	g_pkt_send++;

	send_byte = sendto(app_demo_udp_img_fd, data, len, MSG_DONTWAIT | MSG_MORE,
					   (struct sockaddr *)app_demo_remote, sizeof(struct sockaddr_in));

	if (send_byte < 0) {
		/* err */
		//APP_DEMO_UDP_PRT("send return fd:%d\r\n", send_byte);
		g_pkt_send_fail++;
		send_byte = 0;
	} 

	return send_byte;
}

void app_demo_udp_deinit(void)
{
	GLOBAL_INT_DECLARATION();

	APP_DEMO_UDP_PRT("app_demo_udp_deinit\r\n");
	if (app_demo_udp_run == 0)
		return;

	GLOBAL_INT_DISABLE();
	app_demo_udp_run = 0;
	GLOBAL_INT_RESTORE();

	while (app_demo_udp_hdl)
		rtos_delay_milliseconds(10);
}

#endif  // APP_DEMO_CFG_USE_UDP
#endif  //CONFIG_APP_DEMO_VIDEO_TRANSFER

// EOF

