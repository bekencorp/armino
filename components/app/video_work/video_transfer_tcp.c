#include <common/bk_include.h>

#if (CONFIG_APP_DEMO_VIDEO_TRANSFER)
#include "video_transfer_config.h"

#if (APP_DEMO_CFG_USE_TCP)
#include "lwip/tcp.h"
#include "video_transfer_tcp.h"
//#include "app_led.h"
#include "bk_uart.h"
#include <os/mem.h>

//#include "FreeRTOS.h"
//#include "task.h"
#include <os/os.h>
#include <common/bk_kernel_err.h>
#ifdef CONFIG_RTT
#include <sys/socket.h>
#endif
#include "lwip/sockets.h"

//#include "app_demo_softap.h"
#include <components/video_transfer.h>

#define APP_DEMO_TCP_DEBUG        1
#if APP_DEMO_TCP_DEBUG
#define APP_DEMO_TCP_PRT      warning_prf
#define APP_DEMO_TCP_WARN     warning_prf
#define APP_DEMO_TCP_FATAL    fatal_prf
#else
#define APP_DEMO_TCP_PRT      null_prf
#define APP_DEMO_TCP_WARN     null_prf
#define APP_DEMO_TCP_FATAL    null_prf
#endif

#define APP_DEMO_TCP_RCV_BUF_LEN             1460

beken_thread_t app_demo_tcp_hdl = NULL;
int app_demo_watch_fd_list[APP_DEMO_TCP_LISTEN_MAX];
int app_demo_tcp_server_fd;
volatile int app_demo_tcp_run = 0;

static void app_demo_tcp_set_keepalive(int fd)
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

static void app_demo_tcp_app_connected(void)
{
	//app_demo_softap_send_msg(DAP_APP_CONECTED, 0);
}

static void app_demo_tcp_app_disconnected(void)
{
	//app_demo_softap_send_msg(DAP_APP_DISCONECTED, 0);
}

static void app_demo_tcp_main(beken_thread_arg_t data)
{
	GLOBAL_INT_DECLARATION();
	int maxfd = -1;
	int ret = 0, i = 0;
	int snd_len = 0, rcv_len = 0;
	struct sockaddr_in server_addr;
	socklen_t srvaddr_len = 0;
	fd_set watchfd;
	u8 *rcv_buf = NULL;

	(void)(data);

	APP_DEMO_TCP_FATAL("app_demo_tcp_main entry\r\n");

	rcv_buf = (u8 *) os_malloc((APP_DEMO_TCP_RCV_BUF_LEN + 1) * sizeof(u8));
	if (!rcv_buf) {
		APP_DEMO_TCP_PRT("tcp os_malloc failed\r\n");
		goto app_demo_tcp_exit;
	}

	app_demo_tcp_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (app_demo_tcp_server_fd == -1) {
		APP_DEMO_TCP_PRT("socket failed\r\n");
		goto app_demo_tcp_exit;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(APP_DEMO_TCP_SERVER_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	srvaddr_len = (socklen_t)sizeof(server_addr);
	if (bind(app_demo_tcp_server_fd, (struct sockaddr *)&server_addr, srvaddr_len) == -1) {
		APP_DEMO_TCP_PRT("bind failed\r\n");
		goto app_demo_tcp_exit;
	}

	if (listen(app_demo_tcp_server_fd, APP_DEMO_TCP_LISTEN_MAX) == -1) {
		APP_DEMO_TCP_PRT("listen failed\r\n");
		goto app_demo_tcp_exit;
	}

	maxfd = app_demo_tcp_server_fd;
	for (i = 0; i < APP_DEMO_TCP_LISTEN_MAX; i++)
		app_demo_watch_fd_list[i] = -1;

	GLOBAL_INT_DISABLE();
	app_demo_tcp_run = 1;
	GLOBAL_INT_RESTORE();

	while (app_demo_tcp_run) {
		FD_ZERO(&watchfd);
		FD_SET(app_demo_tcp_server_fd, &watchfd);

		for (i = 0; i < APP_DEMO_TCP_LISTEN_MAX; i++) {
			if (app_demo_watch_fd_list[i] != -1) {
				FD_SET(app_demo_watch_fd_list[i], &watchfd);
				if (maxfd < app_demo_watch_fd_list[i])
					maxfd = app_demo_watch_fd_list[i];
			}
		}

		ret = select(maxfd + 1, &watchfd, NULL, NULL, NULL);
		if (ret <= 0) {
			APP_DEMO_TCP_PRT("select ret:%d\r\n", ret);
			break;
		} else {
			// is new connection
			if (FD_ISSET(app_demo_tcp_server_fd, &watchfd)) {
				int new_cli_sockfd = -1;
				struct sockaddr_in client_addr;
				socklen_t cliaddr_len = 0;

				cliaddr_len = sizeof(client_addr);
				new_cli_sockfd = accept(app_demo_tcp_server_fd, (struct sockaddr *)&client_addr, &cliaddr_len);
				if (new_cli_sockfd < 0) {
					APP_DEMO_TCP_PRT("accept return fd:%d\r\n", new_cli_sockfd);
					break;
				}

				APP_DEMO_TCP_PRT("new accept fd:%d\r\n", new_cli_sockfd);

				for (i = 0; i < APP_DEMO_TCP_LISTEN_MAX; i++) {
					if (app_demo_watch_fd_list[i] == -1) {
						app_demo_watch_fd_list[i] = new_cli_sockfd;

						app_demo_tcp_set_keepalive(new_cli_sockfd);

#if (CONFIG_SPIDMA || CONFIG_CAMERA)
						video_setup_t setup;

						setup.open_type = TVIDEO_OPEN_SCCB;
						setup.send_type = TVIDEO_SND_TCP;
						setup.send_func = app_demo_tcp_send_packet;
						setup.start_cb = app_demo_tcp_app_connected;
						setup.end_cb = app_demo_tcp_app_disconnected;

						setup.pkt_header_size = 0;
						setup.add_pkt_header = NULL;

						bk_video_transfer_init(&setup);
#endif
						break;
					}
				}

				if (i == APP_DEMO_TCP_LISTEN_MAX) {
					APP_DEMO_TCP_PRT("only accept %d clients\r\n", APP_DEMO_TCP_LISTEN_MAX);
					close(new_cli_sockfd);
				}
			}

			// search those added fd
			for (i = 0; i < APP_DEMO_TCP_LISTEN_MAX; i++) {
				if (app_demo_watch_fd_list[i] == -1)
					continue;
				if (!FD_ISSET(app_demo_watch_fd_list[i], &watchfd))
					continue;

				rcv_len = recv(app_demo_watch_fd_list[i], rcv_buf, APP_DEMO_TCP_RCV_BUF_LEN, 0);
				if (rcv_len <= 0) {
					int j;
					// close this socket
					APP_DEMO_TCP_PRT("recv close fd:%d\r\n", app_demo_watch_fd_list[i]);
					close(app_demo_watch_fd_list[i]);
					app_demo_watch_fd_list[i] = -1;

					for (j = 0; j < APP_DEMO_TCP_LISTEN_MAX; j++) {
						if (app_demo_watch_fd_list[j] != -1)
							break;
					}
					if (j == APP_DEMO_TCP_LISTEN_MAX) {
						APP_DEMO_TCP_PRT("not client left, close spidma\r\n");

#if (CONFIG_SPIDMA || CONFIG_CAMERA)
						bk_video_transfer_deinit();
#endif
					}
				} else {
					rcv_len = (rcv_len > APP_DEMO_TCP_RCV_BUF_LEN) ? APP_DEMO_TCP_RCV_BUF_LEN : rcv_len;
					rcv_buf[rcv_len] = 0;

					snd_len = send(app_demo_watch_fd_list[i], rcv_buf, rcv_len, 0);
					if (snd_len < 0) {
						/* err */
						APP_DEMO_TCP_PRT("send return fd:%d\r\n", snd_len);
					}
				}
				FD_CLR(app_demo_watch_fd_list[i], &watchfd);
			}
		}// ret = select
	}

app_demo_tcp_exit:

	APP_DEMO_TCP_FATAL("app_demo_tcp_main exit\r\n");

#if (CONFIG_SPIDMA || CONFIG_CAMERA)
	bk_video_transfer_deinit();
#endif
	//app_drone_send_msg(DRONE_TCP_DISCONECTED);

	if (rcv_buf) {
		os_free(rcv_buf);
		rcv_buf = NULL;
	}

	for (i = 0; i < APP_DEMO_TCP_LISTEN_MAX; i++) {
		if (app_demo_watch_fd_list[i] != -1) {
			close(app_demo_watch_fd_list[i]);
			app_demo_watch_fd_list[i] = -1;
		}
	}

	if (app_demo_tcp_server_fd != -1) {
		close(app_demo_tcp_server_fd);
		app_demo_tcp_server_fd = -1;
	}

	GLOBAL_INT_DISABLE();
	app_demo_tcp_run = 0;
	GLOBAL_INT_RESTORE();

	app_demo_tcp_hdl = NULL;
	rtos_delete_thread(NULL);
}

UINT32 app_demo_tcp_init(void)
{
	int ret;

	APP_DEMO_TCP_PRT("app_demo_tcp_init\r\n");
	if (!app_demo_tcp_hdl) {
		ret = rtos_create_thread(&app_demo_tcp_hdl,
								 4,
								 "app_demo_tcp",
								 (beken_thread_function_t)app_demo_tcp_main,
								 1024,
								 (beken_thread_arg_t)NULL);
		if (ret != kNoErr) {
			APP_DEMO_TCP_PRT("Error: Failed to create spidma_intfer: %d\r\n", ret);
			return kGeneralErr;
		}
	}

	return kNoErr;
}

void app_demo_tcp_deinit(void)
{
	GLOBAL_INT_DECLARATION();

	APP_DEMO_TCP_PRT("app_demo_tcp_deinit\r\n");
	if (app_demo_tcp_run == 0)
		return;

	GLOBAL_INT_DISABLE();
	app_demo_tcp_run = 0;
	GLOBAL_INT_RESTORE();
}

/*---------------------------------------------------------------------------*/
int app_demo_tcp_send_packet(uint8_t *data, uint32_t len)
{
	int i = 0, snd_len = 0;

	if ((!app_demo_tcp_hdl) || (app_demo_tcp_server_fd == -1))
		return 0;


	for (i = 0; i < APP_DEMO_TCP_LISTEN_MAX; i++) {
		if (app_demo_watch_fd_list[i] == -1)
			continue;

		snd_len = send(app_demo_watch_fd_list[i], data, len, MSG_DONTWAIT | MSG_MORE);
		if (snd_len < 0) {
			/* err */
			//APP_DEMO_TCP_PRT("send return fd:%d\r\n", snd_len);
			snd_len = 0;
		}
	}

	return snd_len;
}
/*---------------------------------------------------------------------------*/
#endif  // (APP_DEMO_VIDEO_TRANSFER && APP_DEMO_CFG_USE_TCP)
#endif  //CONFIG_APP_DEMO_VIDEO_TRANSFER


// EOF

