/**
* iperf-liked network performance tool
*
*/

#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include "iperf.h"
#include <lwip/sockets.h>

#define THREAD_SIZE         (4 * 1024)
#define THREAD_PROIRITY     4
#define IPERF_PORT          5001
#define IPERF_BUFSZ         (4 * 1024)

#define IPERF_MODE_STOP     0
#define IPERF_MODE_SERVER   1
#define IPERF_MODE_CLIENT   2

typedef struct {
	int mode;
	char *host;
	int port;
} IPERF_PARAM;
static IPERF_PARAM param = {IPERF_MODE_STOP, NULL, IPERF_PORT};

static void iperf_client(void *thread_param)
{
	int i, sock, ret;
	uint8_t *send_buf;
	uint32_t tick1, tick2, tick_delta, sentlen;
	struct sockaddr_in addr;

	send_buf = (uint8_t *) os_malloc(IPERF_BUFSZ);
	if (!send_buf) return ;

	for (i = 0; i < IPERF_BUFSZ; i++)
		send_buf[i] = i & 0xff;

	while (param.mode != IPERF_MODE_STOP) {
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock < 0) {
			os_printf("create socket failed!\n");
			rtos_delay_milliseconds(1000);
			continue;
		}

		addr.sin_family = PF_INET;
		addr.sin_port = htons(param.port);
		addr.sin_addr.s_addr = inet_addr((char *)param.host);

		ret = connect(sock, (const struct sockaddr *)&addr, sizeof(addr));
		if (ret == -1) {
			os_printf("Connect failed!\n");
			closesocket(sock);

			rtos_delay_milliseconds(1000);
			continue;
		}

		os_printf("Connect to iperf server successful!\n");

		{
			int flag = 1;

			setsockopt(sock,
					   IPPROTO_TCP,     /* set option at TCP level */
					   TCP_NODELAY,     /* name of option */
					   (void *) &flag,  /* the cast is historical cruft */
					   sizeof(int));    /* length of option value */
		}

		tick1 = rtos_get_time();
		tick1 /= 1000;
		tick_delta = 0;
		sentlen = 0;

		while (param.mode != IPERF_MODE_STOP) {
			tick2 = rtos_get_time();
			tick2 /= 1000;
			if (tick2 - tick1 >= 5) {
				int f;

				f = sentlen / (tick2 - tick1) * 8;
				f /= 1000;
				os_printf("[%d-%d]sec bandwidth:%dKbits/sec.\r\n", tick_delta, tick_delta + (tick2 - tick1), f);
				tick_delta += (tick2 - tick1);
				tick1 = tick2;
				sentlen = 0;
			}

			ret = send(sock, send_buf, IPERF_BUFSZ, 0);
			if (ret > 0)
				sentlen += ret;

			if (ret < 0) break;
		}

		closesocket(sock);
		rtos_delay_milliseconds(1000 * 2);
	}
	os_free(send_buf);
	rtos_delete_thread(NULL);
	os_printf("disconnected!\n");
}

void iperf_server(void *thread_param)
{
	uint8_t *recv_data;
	uint32_t sin_size, tick1, tick2, tick_delta;
	int sock = -1, connected, bytes_received, recvlen;
	struct sockaddr_in server_addr, client_addr;

	recv_data = (uint8_t *)os_malloc(IPERF_BUFSZ);
	if (recv_data == NULL) {
		os_printf("No memory\n");
		goto __exit;
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		os_printf("Socket error\n");
		goto __exit;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(param.port);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	os_memset(&(server_addr.sin_zero), 0x0, sizeof(server_addr.sin_zero));

	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
		os_printf("Unable to bind\n");
		goto __exit;
	}

	if (listen(sock, 5) == -1) {
		os_printf("Listen error\n");
		goto __exit;
	}

	while (param.mode != IPERF_MODE_STOP) {
		sin_size = sizeof(struct sockaddr_in);
		connected = accept(sock, (struct sockaddr *)&client_addr, &sin_size);
		os_printf("new client connected from (%s, %d)\n",
				  inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		{
			int flag = 1;

			setsockopt(connected,
					   IPPROTO_TCP,     /* set option at TCP level */
					   TCP_NODELAY,     /* name of option */
					   (void *) &flag,  /* the cast is historical cruft */
					   sizeof(int));    /* length of option value */
		}

		tick1 = rtos_get_time();
		tick1 /= 1000;
		tick_delta = 0;
		recvlen = 0;

		while (param.mode != IPERF_MODE_STOP) {
			bytes_received = recv(connected, recv_data, IPERF_BUFSZ, 0);
			if (bytes_received <= 0) break;

			recvlen += bytes_received;

			tick2 = rtos_get_time();
			tick2 /= 1000;
			if (tick2 - tick1 >= 5) {
				int f;

				f = recvlen / (tick2 - tick1) * 8;
				f /= 1000;
				os_printf("[%d-%d]sec bandwidth:%dKbits/sec.\r\n", tick_delta, tick_delta + (tick2 - tick1), f);
				tick_delta += (tick2 - tick1);
				tick1 = tick2;
				recvlen = 0;
			}
		}

		if (connected >= 0) closesocket(connected);
		connected = -1;
	}

__exit:
	if (sock >= 0) closesocket(sock);
	if (recv_data) os_free(recv_data);
	rtos_delete_thread(NULL);
}

void iperf_usage(void)
{
	os_printf("Usage: iperf [-s|-c host] [options]\n");
	os_printf("       iperf [-h|--stop]\n");
	os_printf("\n");
	os_printf("Client/Server:\n");
	os_printf("  -p #         server port to listen on/connect to\n");
	os_printf("\n");
	os_printf("Server specific:\n");
	os_printf("  -s           run in server mode\n");
	os_printf("\n");
	os_printf("Client specific:\n");
	os_printf("  -c <host>    run in client mode, connecting to <host>\n");
	os_printf("\n");
	os_printf("Miscellaneous:\n");
	os_printf("  -h           print this message and quit\n");
	os_printf("  --stop       stop iperf program\n");

	return ;
}

void iperf(int argc, char **argv)
{
	int mode = 0; /* server mode */
	char *host = NULL;
	int port = IPERF_PORT;

	if (argc == 1) goto __usage;
	else {
		if (os_strcmp(argv[1], "-h") == 0) goto __usage;
		else if (os_strcmp(argv[1], "--stop") == 0) {
			/* stop iperf */
			param.mode = IPERF_MODE_STOP;
			return;
		} else if (os_strcmp(argv[1], "-s") == 0) {
			mode = IPERF_MODE_SERVER; /* server mode */

			/* iperf -s -p 5000 */
			if (argc == 4) {
				if (os_strcmp(argv[2], "-p") == 0)
					port = atoi(argv[3]);
				else goto __usage;
			}
		} else if (os_strcmp(argv[1], "-c") == 0) {
			mode = IPERF_MODE_CLIENT; /* client mode */
			if (argc < 3) goto __usage;

			host = argv[2];
			if (argc == 5) {
				/* iperf -c host -p port */
				if (os_strcmp(argv[3], "-p") == 0)
					port = atoi(argv[4]);
				else goto __usage;
			}
		} else if (os_strcmp(argv[1], "-h") == 0)
			goto __usage;
		else goto __usage;
	}

	/* start iperf */
	if (param.mode == IPERF_MODE_STOP) {
		param.mode = mode;
		param.port = port;
		if (param.host) {
			os_free(param.host);
			param.host = NULL;
		}
		if (host) param.host = os_strdup(host);

		if (mode == IPERF_MODE_CLIENT)
			rtos_create_thread(NULL, THREAD_PROIRITY, "iperfc", iperf_client,
							   THREAD_SIZE, (beken_thread_arg_t)0);
		else if (mode == IPERF_MODE_SERVER)
			rtos_create_thread(NULL, THREAD_PROIRITY, "iperfd", iperf_server,
							   THREAD_SIZE, (beken_thread_arg_t)0);
	} else {
		os_printf("Please stop iperf firstly, by:\n");
		os_printf("iperf --stop\n");
	}

	return;

__usage:
	iperf_usage();
	return;
}

