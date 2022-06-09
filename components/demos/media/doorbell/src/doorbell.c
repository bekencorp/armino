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
#include "doorbell_config.h"
#include "bk_uart.h"
#include <os/mem.h>
#include <components/video_transfer.h>
#include <driver/dma.h>
#include <modules/audio_ring_buff.h>
#include "audio_transfer_cp0.h"


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


#define APP_DEMO_UDP_RCV_BUF_LEN        1472
#define APP_DEMO_UDP_SOCKET_TIMEOUT     100  // ms

/* audio used */
#define READ_SIZE    160
#define WRITE_SIZE    320

int demo_doorbell_udp_img_fd = -1;
volatile int demo_doorbell_udp_romote_connected = 0;
volatile int demo_doorbell_udp_run = 0;
beken_thread_t demo_doorbell_udp_hdl = NULL;
struct sockaddr_in *demo_doorbell_remote = NULL;
int demo_doorbell_udp_voice_romote_connected = 0;

struct sockaddr_in *demo_doorbell_voice_udp_remote = NULL;

int demo_doorbell_udp_voice_fd = -1;

/* audio used */
static uint8_t *temp_read_buffer = NULL;
static uint8_t *temp_write_buffer = NULL;
static uint8_t *audio_rx_ring_buffer = NULL;    //save data received from apk
static RingBufferContext audio_rx_rb;
bool temp_write_buffer_status = false;


int demo_doorbell_udp_send_packet(uint8_t *data, uint32_t len)
{
	int send_byte = 0;

	if (!demo_doorbell_udp_romote_connected)
		return 0;

	send_byte = sendto(demo_doorbell_udp_img_fd, data, len, MSG_DONTWAIT | MSG_MORE,
					   (struct sockaddr *)demo_doorbell_remote, sizeof(struct sockaddr_in));

	if (send_byte < 0) {
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
		return 0;

	send_byte = sendto(demo_doorbell_udp_voice_fd, data, len, MSG_DONTWAIT | MSG_MORE,
					   (struct sockaddr *)demo_doorbell_voice_udp_remote, sizeof(struct sockaddr_in));

	if (send_byte < 0) {
		/* err */
		//LWIP_UDP_PRT("send return fd:%d\r\n", send_byte);
		send_byte = 0;
	}

	return send_byte;
}
#endif //DEMO_DOORBELL_EN_VOICE_TRANSFER

static void audio_cp0_read_done_callback(uint8_t *buffer, uint32_t length)
{
	uint32 size = 0;
	//DBD("read done \r\n");

	/* send audio data to apk */
	size = demo_doorbell_udp_voice_send_packet(temp_read_buffer, READ_SIZE);
	if (size != READ_SIZE) {
		DBD("send audio data to apk fail, size=%d \r\n", size);
		return;
	}

}

/* get encoder used size,
 * if the size is equal or greater than READ_SIZE, send read request to read data 
 */
static void audio_cp0_write_done_callback(uint8_t *buffer, uint32_t length)
{
	//DBD("write data done");

	/* reset temp_write_buffer_status */
	temp_write_buffer_status = false;
}

/* get encoder used size,
 * if the size is equal or greater than READ_SIZE, send read request to read data 
 */
static void audio_cp0_get_encoder_used_size_callback(uint32_t size)
{
	bk_err_t ret = BK_OK;

	if (size >= READ_SIZE) {
		ret = bk_audio_read_req(temp_read_buffer, READ_SIZE);
		if (ret != BK_OK)
			DBD("send read request fail \r\n");
	}
}

/* get decoder remain size,
 * if the size is equal or greater than READ_SIZE, send read request to read data 
 */
static void audio_cp0_get_decoder_remain_size_callback(uint32_t size)
{
	bk_err_t ret = BK_OK;
	//DBD("audio_cp0_get_decoder_remain_size_callback \r\n");

	/* check receive data buffer and write data to audio dac */
	if ((size >= WRITE_SIZE) && (temp_write_buffer_status)) {
		ret = bk_audio_write_req(temp_write_buffer, WRITE_SIZE);
		if (ret != BK_OK)
			DBD("send write request fail \r\n");
	}
}

/* receive CPU1 read request, and send read reqest to CPU1 */
static void audio_cp0_encoder_read_req_handler(void)
{
	bk_err_t ret = BK_OK;
	//DBD("read request \r\n");

	ret = bk_audio_read_req(temp_read_buffer, READ_SIZE);
	if (ret != BK_OK)
		DBD("send write request fail \r\n");
}

/* receive CPU1 write request, and send write decoder data reqest to CPU1 */
static void audio_cp0_decoder_write_req_handler(void)
{
	//bk_err_t ret = BK_OK;
	//DBD("write request \r\n");

	/* donot do noting */
	//TODO
/*
	ret = bk_audio_write_req(temp_write_buffer, WRITE_SIZE);
	if (ret != BK_OK)
		DBD("send write request fail \r\n");
*/
}

static void audio_cp0_write_data_req(void)
{
	bk_audio_get_decoder_remain_size();
	//DBD("get decoder remain size \r\n");
}

static void cp0_audio_transfer_init(void)
{
	bk_err_t ret = BK_OK;

	temp_read_buffer = (uint8_t *)os_malloc(READ_SIZE);
	if (temp_read_buffer == NULL) {
		DBD("malloc temp_read_buffer fail \r\n");
		return;
	}
	DBD("malloc temp_read_buffer:%p complete \r\n", temp_read_buffer);
	temp_write_buffer = (uint8_t *)os_malloc(WRITE_SIZE);
	if (temp_write_buffer == NULL) {
		DBD("malloc temp_write_buffer fail \r\n");
		return;
	}
	DBD("malloc temp_write_buffer:%p complete \r\n", temp_write_buffer);

	audio_rx_ring_buffer = (uint8_t *)os_malloc(WRITE_SIZE*2);
	if (audio_rx_ring_buffer == NULL) {
		DBD("malloc audio_rx_ring_buffer fail \r\n");
		return;
	}
	DBD("malloc audio_rx_ring_buffer:%p complete \r\n", audio_rx_ring_buffer);

	ring_buffer_init(&audio_rx_rb, audio_rx_ring_buffer, WRITE_SIZE*2, DMA_ID_MAX, RB_DMA_TYPE_NULL);
	DBD("init audio rx ring buffer complete \r\n");

	/* register callbacks */
	bk_audio_register_rw_cb(audio_cp0_read_done_callback,
							audio_cp0_write_done_callback,
							audio_cp0_get_encoder_used_size_callback,
							audio_cp0_get_decoder_remain_size_callback,
							audio_cp0_encoder_read_req_handler,
							audio_cp0_decoder_write_req_handler);
	DBD("register callbacks complete \r\n");

	/* init audio cpu0 transfer task */
	ret = bk_audio_cp0_transfer_init(NULL);
	if (ret != BK_OK) {
		DBD("init audio transfer task fail \r\n");
		return;
	}

	DBD("init audio transfer driver complete \r\n");
}

static void cp0_audio_transfer_deinit(void)
{
	bk_err_t ret = BK_OK;

	ret = bk_audio_cp0_transfer_deinit();
	if (ret != BK_OK) {
		DBD("cp0: audio transfer deinit fail \r\n");
	}

	/* free memory */
	os_free(temp_read_buffer);
	os_free(temp_write_buffer);
	ring_buffer_clear(&audio_rx_rb);
	os_free(audio_rx_ring_buffer);

	DBD("cp0: stop audio_transfer test successful \r\n");
}

void demo_doorbell_add_pkt_header(video_packet_t *param)
{
	media_hdr_t *elem_tvhdr = (media_hdr_t*)param->ptk_ptr;

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
	uint8_t crc_cal;

	DBD("demo_doorbell_udp_handle_cmd_data\r\n");

	if ((data[0] != CMD_HEADER_CODE) && (len != CMD_LEN) && (data[len - 1] != CMD_TAIL_CODE))
		return;

	crc_cal = (data[1] ^ data[2] ^ data[3] ^ data[4] ^ data[5]);

	if (crc_cal != data[6]) {
		if (((crc_cal == CMD_HEADER_CODE) || (crc_cal == CMD_TAIL_CODE))
			&& (crc_cal + 1 == data[6]))
			// drop this paket for crc is the same with Header or Tailer
			return;
		else // change to right crc
			data[6] = crc_cal;
	}

	{
		for (int i = 0; i < len; i++)
			uart_write_byte(UART_ID_0, data[i]);
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

#if CFG_SUPPORT_HTTP_OTA
TV_OTA_ST ota_param = {
	NULL,
	0,
	0
};
static void demo_doorbell_udp_http_ota_handle(char *rev_data)
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

static void demo_doorbell_udp_receiver(UINT8 *data, UINT32 len, struct sockaddr_in *demo_doorbell_remote)
{
	DBD("demo_doorbell_udp_receiver\r\n");

	GLOBAL_INT_DECLARATION();

	if (len < 2)
		return;

	if (data[0] == DOORBELL_CMD_IMG_HEADER) {
		if (data[1] == DOORBELL_CMD_START_IMG) {

			UINT8 *src_ipaddr = (UINT8 *)&demo_doorbell_remote->sin_addr.s_addr;
			DBD("src_ipaddr: %d.%d.%d.%d\r\n", src_ipaddr[0], src_ipaddr[1],
							 src_ipaddr[2], src_ipaddr[3]);
			DBD("udp connect to new port:%d\r\n", demo_doorbell_remote->sin_port);

			GLOBAL_INT_DISABLE();
			demo_doorbell_udp_romote_connected = 1;
			GLOBAL_INT_RESTORE();

#if (CONFIG_SPIDMA || CONFIG_CAMERA)
			video_setup_t setup;

			setup.open_type = TVIDEO_OPEN_SCCB;
			setup.send_type = TVIDEO_SND_UDP;
			setup.send_func = demo_doorbell_udp_send_packet;
			setup.start_cb = demo_doorbell_udp_app_connected;
			setup.end_cb = demo_doorbell_udp_app_disconnected;

			setup.pkt_header_size = sizeof(media_hdr_t);
			setup.add_pkt_header = demo_doorbell_add_pkt_header;

			bk_video_transfer_init(&setup);
#endif
		} else if (data[1] == DOORBELL_CMD_STOP_IMG) {
			DBD("udp close\r\n");

#if (CONFIG_SPIDMA || CONFIG_CAMERA)
			bk_video_transfer_deinit();
#endif

			GLOBAL_INT_DISABLE();
			demo_doorbell_udp_romote_connected = 0;
			GLOBAL_INT_RESTORE();
		}
#if CFG_SUPPORT_HTTP_OTA
		else if (data[1] == CMD_START_OTA)
			demo_doorbell_udp_http_ota_handle(&data[2]);
#endif
	}

}

#if DEMO_DOORBELL_EN_VOICE_TRANSFER
static void demo_doorbell_udp_voice_receiver(UINT8 *data, UINT32 len, struct sockaddr_in *udp_remote)
{
	//DBD("demo_doorbell_udp_voice_receiver, data len=%d \r\n", len);
	uint32_t size = 0;
	if (len > 0)
		demo_doorbell_udp_voice_romote_connected = 1;

	//GLOBAL_INT_DECLARATION();
	size = ring_buffer_get_free_size(&audio_rx_rb);
	if (size < len) {
		DBD("the length of data received from apk is greater than free ring buffer \r\n");
		return;
	}

	size = ring_buffer_write(&audio_rx_rb, data, len);
	if (size != len) {
		DBD("write ring buffer data fail, size=%d \r\n", size);
		return;
	}

	size = ring_buffer_get_fill_size(&audio_rx_rb);
	if (size >= WRITE_SIZE) {
		if (temp_write_buffer_status) {
			DBD("the data in temp_write_buffer not been send \r\n");
			return;
		}
		size = ring_buffer_read(&audio_rx_rb, temp_write_buffer, WRITE_SIZE);
		if (size != WRITE_SIZE) {
			DBD("read ring buffer data fail, size=%d \r\n", size);
			return;
		}
		temp_write_buffer_status = true;
		audio_cp0_write_data_req();
	}
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
	if (!rcv_buf) {
		DBD("udp os_malloc failed\r\n");
		goto app_udp_exit;
	}

	demo_doorbell_remote = (struct sockaddr_in *)os_malloc(sizeof(struct sockaddr_in));
	if (!demo_doorbell_remote) {
		DBD("udp os_malloc failed\r\n");
		goto app_udp_exit;
	}

	// for data transfer
	demo_doorbell_udp_img_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (demo_doorbell_udp_img_fd == -1) {
		DBD("socket failed\r\n");
		goto app_udp_exit;
	}

	demo_doorbell_remote->sin_family = AF_INET;
	demo_doorbell_remote->sin_port = htons(DEMO_DOORBELL_UDP_IMG_PORT);
	demo_doorbell_remote->sin_addr.s_addr = htonl(INADDR_ANY);

	srvaddr_len = (socklen_t)sizeof(struct sockaddr_in);
	if (bind(demo_doorbell_udp_img_fd, (struct sockaddr *)demo_doorbell_remote, srvaddr_len) == -1) {
		DBD("bind failed\r\n");
		goto app_udp_exit;
	}

	//  for recv uart cmd
	udp_cmd_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_cmd_fd == -1) {
		DBD("socket failed\r\n");
		goto app_udp_exit;
	}

	cmd_remote.sin_family = AF_INET;
	cmd_remote.sin_port = htons(DEMO_DOORBELL_UDP_CMD_PORT);
	cmd_remote.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(udp_cmd_fd, (struct sockaddr *)&cmd_remote, srvaddr_len) == -1) {
		DBD("bind failed\r\n");
		goto app_udp_exit;
	}

	maxfd = (udp_cmd_fd > demo_doorbell_udp_img_fd) ? udp_cmd_fd : demo_doorbell_udp_img_fd;

	// for voice transfer
#if DEMO_DOORBELL_EN_VOICE_TRANSFER
	demo_doorbell_voice_udp_remote = (struct sockaddr_in *)os_malloc(sizeof(struct sockaddr_in));
	if (!demo_doorbell_voice_udp_remote) {
		DBD("udp os_malloc failed\r\n");
		goto app_udp_exit;
	}

	demo_doorbell_udp_voice_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (demo_doorbell_udp_voice_fd == -1) {
		DBD("vo socket failed\r\n");
		goto app_udp_exit;
	}

	demo_doorbell_voice_udp_remote->sin_family = AF_INET;
	demo_doorbell_voice_udp_remote->sin_port = htons(DEMO_DOORBELL_UDP_VOICE_PORT);
	demo_doorbell_voice_udp_remote->sin_addr.s_addr = htonl(INADDR_ANY);

	srvaddr_len = (socklen_t)sizeof(struct sockaddr_in);
	if (bind(demo_doorbell_udp_voice_fd, (struct sockaddr *)demo_doorbell_voice_udp_remote, srvaddr_len) == -1) {
		DBD("bind failed\r\n");
		goto app_udp_exit;
	}
	maxfd = (maxfd > demo_doorbell_udp_voice_fd) ? maxfd : demo_doorbell_udp_voice_fd;
#endif // DEMO_DOORBELL_EN_VOICE_TRANSFER

	timeout.tv_sec = APP_DEMO_UDP_SOCKET_TIMEOUT / 1000;
	timeout.tv_usec = (APP_DEMO_UDP_SOCKET_TIMEOUT % 1000) * 1000;

	/* init audio transfer driver */
	cp0_audio_transfer_init();

	GLOBAL_INT_DISABLE();
	demo_doorbell_udp_romote_connected = 0;
	demo_doorbell_udp_run = 1;
#if DEMO_DOORBELL_EN_VOICE_TRANSFER
	demo_doorbell_udp_voice_romote_connected = 0;
#endif
	GLOBAL_INT_RESTORE();

	while (demo_doorbell_udp_run) {
		FD_ZERO(&watchfd);
		FD_SET(demo_doorbell_udp_img_fd, &watchfd);
		FD_SET(udp_cmd_fd, &watchfd);
#if DEMO_DOORBELL_EN_VOICE_TRANSFER
		FD_SET(demo_doorbell_udp_voice_fd, &watchfd);
#endif

		//DBD("wait for select\r\n");
		ret = select(maxfd + 1, &watchfd, NULL, NULL, &timeout);
		if (ret < 0) {
			DBD("select ret:%d\r\n", ret);
			break;
		} else {
			// is img fd, data transfer
			if (FD_ISSET(demo_doorbell_udp_img_fd, &watchfd)) {
				rcv_len = recvfrom(demo_doorbell_udp_img_fd, rcv_buf, APP_DEMO_UDP_RCV_BUF_LEN, 0,
								   (struct sockaddr *)demo_doorbell_remote, &srvaddr_len);

				if (rcv_len <= 0) {
					// close this socket
					DBD("recv close fd:%d\r\n", demo_doorbell_udp_img_fd);
					break;
				} else {
					rcv_len = (rcv_len > APP_DEMO_UDP_RCV_BUF_LEN) ? APP_DEMO_UDP_RCV_BUF_LEN : rcv_len;
					rcv_buf[rcv_len] = 0;

					DBD("demo_doorbell_udp_receiver");
					demo_doorbell_udp_receiver(rcv_buf, rcv_len, demo_doorbell_remote);
				}
			} else if (FD_ISSET(udp_cmd_fd, &watchfd)) {
				rcv_len = recvfrom(udp_cmd_fd, rcv_buf, APP_DEMO_UDP_RCV_BUF_LEN, 0,
								   (struct sockaddr *)&cmd_remote, &srvaddr_len);

				if (rcv_len <= 0) {
					// close this socket
					DBD("recv close fd:%d\r\n", udp_cmd_fd);
					break;
				} else {
					rcv_len = (rcv_len > APP_DEMO_UDP_RCV_BUF_LEN) ? APP_DEMO_UDP_RCV_BUF_LEN : rcv_len;
					rcv_buf[rcv_len] = 0;

					demo_doorbell_udp_handle_cmd_data(rcv_buf, rcv_len);
				}

			}
#if DEMO_DOORBELL_EN_VOICE_TRANSFER
			if (FD_ISSET(demo_doorbell_udp_voice_fd, &watchfd)) {
				rcv_len = recvfrom(demo_doorbell_udp_voice_fd, rcv_buf, APP_DEMO_UDP_RCV_BUF_LEN, 0,
								   (struct sockaddr *)demo_doorbell_voice_udp_remote, &srvaddr_len);

				if (rcv_len <= 0) {
					// close this socket
					DBD("recv close fd:%d\r\n", demo_doorbell_udp_voice_fd);
					break;
				} else {
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

#if (CONFIG_SPIDMA || CONFIG_CAMERA)
	bk_video_transfer_deinit();
#endif

	/* deinit audio transfer driver */
	cp0_audio_transfer_deinit();

	if (rcv_buf) {
		os_free(rcv_buf);
		rcv_buf = NULL;
	}

#if DEMO_DOORBELL_EN_VOICE_TRANSFER
	if (demo_doorbell_udp_voice_fd != -1) {
		close(demo_doorbell_udp_voice_fd);
		demo_doorbell_udp_voice_fd = -1;
	}

	if (demo_doorbell_voice_udp_remote) {
		os_free(demo_doorbell_voice_udp_remote);
		demo_doorbell_voice_udp_remote = NULL;
	}
#endif // DEMO_DOORBELL_EN_VOICE_TRANSFER
	if (demo_doorbell_remote) {
		os_free(demo_doorbell_remote);
		demo_doorbell_remote = NULL;
	}

	if (demo_doorbell_udp_img_fd != -1) {
		close(demo_doorbell_udp_img_fd);
		demo_doorbell_udp_img_fd = -1;
	}

	if (udp_cmd_fd != -1) {
		close(udp_cmd_fd);
		udp_cmd_fd = -1;
	}

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
	if (!demo_doorbell_udp_hdl) {
		ret = rtos_create_thread(&demo_doorbell_udp_hdl,
								 4,
								 "app_udp",
								 (beken_thread_function_t)demo_doorbell_udp_main,
								 1024,
								 (beken_thread_arg_t)NULL);
		if (ret != kNoErr) {
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
		return;

	GLOBAL_INT_DISABLE();
	demo_doorbell_udp_run = 0;
	GLOBAL_INT_RESTORE();

	while (demo_doorbell_udp_hdl)
		rtos_delay_milliseconds(10);
}

void cli_doorbell_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	os_printf("cli_doorbell_test_cmd\r\n");
	demo_doorbell_udp_init();
}

