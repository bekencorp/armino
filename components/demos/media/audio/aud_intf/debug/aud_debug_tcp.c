#include <common/bk_include.h>
#include "lwip/tcp.h"
#include "bk_uart.h"
#include <os/mem.h>
#include <os/os.h>
#include <common/bk_kernel_err.h>

#include "lwip/sockets.h"

#include "aud_debug_tcp.h"
#include <components/aud_intf.h>
#include <components/aud_intf_types.h>

#define TAG "aud_debug_tcp"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


#define AUD_DEBUG_TCP_RCV_BUF_LEN               (1460)

#define TCP_HEAD_SIZE                               (8)
#define TCP_HEAD_ID_HB                              (0xFF)
#define TCP_HEAD_ID_LB                              (0xD5)
#define HEAD_IND_HB                                 (0)
#define HEAD_IND_LB                                 (1)
#define SEQ_IND_HB                                  (2)
#define SEQ_IND_LB                                  (3)
#define CHNL_IND_HB                                 (4)
#define CHNL_IND_LB                                 (5)
#define LEN_IND_HB                                  (6)
#define LEN_IND_LB                                  (7)

#define TCP_CHANNEL_CMD                             (0)
#define TCP_CHANNEL_AUDIO                           (1)


#define TCP_HEAD_ID (TCP_HEAD_ID_HB << 8 | TCP_HEAD_ID_LB)

beken_thread_t aud_debug_tcp_task = NULL;

static int aud_debug_tcp_watch_fd_list[AUD_DEBUG_TCP_LISTEN_MAX];
static int aud_debug_tcp_server_fd;
volatile int aud_debug_tcp_running = 0;

static uint8_t *tcp_cache_buffer = NULL;
static uint16_t tcp_cache_count = 0;

static uint8_t *tcp_send_buffer = NULL;

static uint16_t tcp_sequence = 0;

static beken_mutex_t send_lock;

static void aud_debug_tcp_set_keepalive(int fd)
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

static int aud_debug_tcp_send_packet(uint16_t channel, uint8_t *data, uint32_t len)
{
	int i = 0, snd_len = 0;

	if ((!aud_debug_tcp_task) || (aud_debug_tcp_server_fd == -1))
	{
		return 0;
	}

	tcp_sequence++;

	tcp_send_buffer[0] = TCP_HEAD_ID_HB;
	tcp_send_buffer[1] = TCP_HEAD_ID_LB;
	tcp_send_buffer[2] = (tcp_sequence >> 8) & 0xFF;
	tcp_send_buffer[3] = (tcp_sequence >> 0) & 0xFF;
	tcp_send_buffer[4] = (channel >> 8) & 0xFF;
	tcp_send_buffer[5] = (channel >> 0) & 0xFF;
	tcp_send_buffer[6] = (len >> 8) & 0xFF;
	tcp_send_buffer[7] = (len >> 0) & 0xFF;

	os_memcpy(tcp_send_buffer + TCP_HEAD_SIZE, data, len);

	//LOGI("sequence: %u, length: %u\n", tcp_sequence, len);

	for (i = 0; i < AUD_DEBUG_TCP_LISTEN_MAX; i++)
	{
		if (aud_debug_tcp_watch_fd_list[i] == -1)
		{
			continue;
		}

		rtos_lock_mutex(&send_lock);
		snd_len = write(aud_debug_tcp_watch_fd_list[i], tcp_send_buffer, TCP_HEAD_SIZE + len);
		rtos_unlock_mutex(&send_lock);

		if (snd_len < 0)
		{
			/* err */
			//APP_DEMO_TCP_PRT("send return fd:%d\r\n", snd_len);
			snd_len = 0;
		}
	}

	return snd_len - TCP_HEAD_SIZE;
}


int bk_aud_debug_voc_tcp_send_packet(unsigned char *data, unsigned int len)
{
	return aud_debug_tcp_send_packet(TCP_CHANNEL_AUDIO, data, len);
}


void aud_debug_dump(uint8_t *data, uint32_t length)
{
	for (int i = 0; i < length; i++)
	{
		os_printf("%02X ", data[i]);

		if ((i + 1) % 20 == 0)
		{
			os_printf("\n");
		}
	}
	os_printf("\n");
}


static void aud_debug_tcp_cmd_data_handle(uint8_t *data, uint16_t length)
{
	uint32_t param = 0;
	uint32_t cmd = (uint32_t)data[0] << 24 | (uint32_t)data[1] << 16 | (uint32_t)data[2] << 8 | data[3];
	if (length >= 8)
	{
		param = (uint32_t)data[4] << 24 | (uint32_t)data[5] << 16 | (uint32_t)data[6] << 8 | data[7];
	}

	LOGI("aud_debug tcp cmd: %08X, param: %d, len: %d\n", cmd, param, length);

	if (length >= 8)
	{
		switch (cmd)
		{
			case ECHO_DEPTH:
				LOGI("set ECHO_DEPTH: %d\n", param);
				bk_aud_intf_set_aec_para(AUD_INTF_VOC_AEC_EC_DEPTH, param);
				break;

			case MAX_AMPLITUDE:
				LOGI("set MAX_AMPLITUDE: %d\n", param);
				bk_aud_intf_set_aec_para(AUD_INTF_VOC_AEC_TXRX_THR, param);
				break;

			case MIN_AMPLITUDE:
				LOGI("set MIN_AMPLITUDE: %d\n", param);
				bk_aud_intf_set_aec_para(AUD_INTF_VOC_AEC_TXRX_FLR, param);
				break;

			case NOISE_LEVEL:
				LOGI("set NOISE_LEVEL: %d\n", param);
				bk_aud_intf_set_aec_para(AUD_INTF_VOC_AEC_NS_LEVEL, param);
				break;

			case NOISE_PARAM:
				LOGI("set NOISE_PARAM: %d\n", param);
				bk_aud_intf_set_aec_para(AUD_INTF_VOC_AEC_NS_PARA, param);
				break;

			default:
				break;
		}
	}
	else
	{
		LOGI("cmd:0x%x not need to handle\n", cmd);
	}

}

static void aud_debug_tcp_channel_data_handle(uint16_t sequence, uint16_t channel, uint8_t *data, uint16_t length)
{
	LOGD("%d, tpye: %d, size: %d\n", sequence, channel, length);

	switch (channel)
	{
		case TCP_CHANNEL_CMD:
			aud_debug_tcp_cmd_data_handle(data, length);
			break;

		default:
			break;
	}
}

#if 0
static uint8_t *find_tcp_head(uint8_t *data, uint32_t length)
{
	uint8_t *ret = NULL;

	for (uint32_t i = 0; i < length - 1; i++)
	{
		if (data[i] == TCP_HEAD_ID_HB
		    && data[i + 1] == TCP_HEAD_ID_LB)
		{
			ret = &data[i];
		}
	}

	return ret;
}
#endif

static void aud_debug_tcp_data_recv_handle(uint8_t *data, uint32_t length)
{
	static uint32_t count = 0;

	LOGD("DUMP DATA %u, size: %u\n", count++, length);

	//aud_debug_dump(data, length);

	uint16_t channel_id = 0, channel_len = 0, head = 0, sequence = 0;
	uint8_t *ptr = data;
	uint32_t left = length;

	while (left != 0)
	{
		if (tcp_cache_count == 0)
		{
			if (left < TCP_HEAD_SIZE)
			{
				os_memcpy(tcp_cache_buffer + tcp_cache_count, ptr, left);
				tcp_cache_count += left;
				break;
			}

			head = ptr[HEAD_IND_HB] << 8 | ptr[HEAD_IND_LB];

			if (head == TCP_HEAD_ID)
			{
				channel_id = ptr[CHNL_IND_HB] << 8 | ptr[CHNL_IND_LB];
				channel_len = ptr[LEN_IND_HB] << 8 | ptr[LEN_IND_LB];
				sequence = ptr[SEQ_IND_HB] << 8 | ptr[SEQ_IND_LB];
			}
			else
			{
				LOGE("invaild src data\n");
				aud_debug_dump(ptr, left);
				LOGE("dump src data\n");
				//TODO FIXME
				aud_debug_dump(data, length);
				break;
			}

			if (left < channel_len + TCP_HEAD_SIZE)
			{
				LOGI("data not enough, left: %d\n", left);
				os_memcpy(tcp_cache_buffer + tcp_cache_count, ptr, left);
				tcp_cache_count += left;
				//LOGI("DUMP FULL DATA\n");
				//aud_debug_dump(data, length);
				//LOGI("DUMP LEFT DATA\n");
				//aud_debug_dump(ptr, left);
				break;
			}

			aud_debug_tcp_channel_data_handle(sequence, channel_id, ptr + TCP_HEAD_SIZE, channel_len);

			ptr += TCP_HEAD_SIZE + channel_len;
			left -= TCP_HEAD_SIZE + channel_len;
		}
		else
		{
			if (tcp_cache_count < TCP_HEAD_SIZE)
			{
				if (left < TCP_HEAD_SIZE)
				{
					os_memcpy(tcp_cache_buffer + tcp_cache_count, ptr, left);
					tcp_cache_count += left;
					left = 0;
					break;
				}
				else
				{
					os_memcpy(tcp_cache_buffer + tcp_cache_count, ptr, TCP_HEAD_SIZE - tcp_cache_count);
					tcp_cache_count += TCP_HEAD_SIZE - tcp_cache_count;
					ptr += TCP_HEAD_SIZE - tcp_cache_count;
					left -= TCP_HEAD_SIZE - tcp_cache_count;
				}
			}

			head = tcp_cache_buffer[HEAD_IND_HB] << 8 | tcp_cache_buffer[HEAD_IND_LB];

			if (head == TCP_HEAD_ID)
			{
				channel_id = tcp_cache_buffer[CHNL_IND_HB] << 8 | tcp_cache_buffer[CHNL_IND_LB];
				channel_len = tcp_cache_buffer[LEN_IND_HB] << 8 | tcp_cache_buffer[LEN_IND_LB];
				sequence = tcp_cache_buffer[SEQ_IND_HB] << 8 | tcp_cache_buffer[SEQ_IND_LB];
			}
			else
			{
				LOGE("invaild cached data, %04X\n", head);
				aud_debug_dump(tcp_cache_buffer, tcp_cache_count);
				//TODO FIXME
				break;
			}

			if (tcp_cache_count < TCP_HEAD_SIZE + channel_len)
			{
				if (left + tcp_cache_count < channel_len + TCP_HEAD_SIZE)
				{
					os_memcpy(tcp_cache_buffer + tcp_cache_count, ptr, left);
					tcp_cache_count += left;
					left = 0;
					break;
				}

				os_memcpy(tcp_cache_buffer + tcp_cache_count, ptr, channel_len + TCP_HEAD_SIZE - tcp_cache_count);
				left -= channel_len + TCP_HEAD_SIZE - tcp_cache_count;
				ptr += channel_len + TCP_HEAD_SIZE - tcp_cache_count;
				tcp_cache_count += channel_len + TCP_HEAD_SIZE - tcp_cache_count;

				aud_debug_tcp_channel_data_handle(sequence, channel_id, tcp_cache_buffer + TCP_HEAD_SIZE, channel_len);

				LOGD("cached: %d, left: %d\n", tcp_cache_count, left);

				tcp_cache_count = 0;
			}
			else
			{
				LOGE("invaild flow data\n");
				aud_debug_dump(tcp_cache_buffer, tcp_cache_count);
				//SHOULD NOT BE HERE
				//TODO FIMXME
				break;
			}
		}
	}
}

static void aud_debug_tcp_main(beken_thread_arg_t data)
{
	GLOBAL_INT_DECLARATION();
	int maxfd = -1;
	int ret = 0, i = 0;
	int rcv_len = 0;
	struct sockaddr_in server_addr;
	socklen_t srvaddr_len = 0;
	fd_set watchfd;
	u8 *rcv_buf = NULL;

	(void)(data);

	LOGI("%s entry\n", __func__);

	rcv_buf = (u8 *) os_malloc((AUD_DEBUG_TCP_RCV_BUF_LEN + 1) * sizeof(u8));
	if (!rcv_buf)
	{
		LOGE("tcp os_malloc failed\n");
		goto out;
	}

	aud_debug_tcp_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (aud_debug_tcp_server_fd == -1)
	{
		LOGE("socket failed\n");
		goto out;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(DEMO_DOORBELL_TCP_SERVER_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	srvaddr_len = (socklen_t)sizeof(server_addr);
	if (bind(aud_debug_tcp_server_fd, (struct sockaddr *)&server_addr, srvaddr_len) == -1)
	{
		LOGE("bind failed\n");
		goto out;
	}

	if (listen(aud_debug_tcp_server_fd, AUD_DEBUG_TCP_LISTEN_MAX) == -1)
	{
		LOGE("listen failed\n");
		goto out;
	}

	maxfd = aud_debug_tcp_server_fd;

	for (i = 0; i < AUD_DEBUG_TCP_LISTEN_MAX; i++)
	{
		aud_debug_tcp_watch_fd_list[i] = -1;
	}

	GLOBAL_INT_DISABLE();
	aud_debug_tcp_running = 1;
	GLOBAL_INT_RESTORE();

	while (aud_debug_tcp_running)
	{
		LOGD("set fd\n");

		FD_ZERO(&watchfd);
		FD_SET(aud_debug_tcp_server_fd, &watchfd);

		for (i = 0; i < AUD_DEBUG_TCP_LISTEN_MAX; i++)
		{
			if (aud_debug_tcp_watch_fd_list[i] != -1)
			{
				FD_SET(aud_debug_tcp_watch_fd_list[i], &watchfd);
				if (maxfd < aud_debug_tcp_watch_fd_list[i])
				{
					maxfd = aud_debug_tcp_watch_fd_list[i];
				}
			}
		}

		LOGD("select fd\n");
		ret = select(maxfd + 1, &watchfd, NULL, NULL, NULL);
		if (ret <= 0)
		{
			LOGE("select ret:%d\n", ret);
			break;
		}
		else
		{
			// is new connection
			if (FD_ISSET(aud_debug_tcp_server_fd, &watchfd))
			{
				int new_cli_sockfd = -1;
				struct sockaddr_in client_addr;
				socklen_t cliaddr_len = 0;

				cliaddr_len = sizeof(client_addr);
				new_cli_sockfd = accept(aud_debug_tcp_server_fd, (struct sockaddr *)&client_addr, &cliaddr_len);
				if (new_cli_sockfd < 0)
				{
					LOGE("accept return fd:%d\n", new_cli_sockfd);
					break;
				}

				LOGI("new accept fd:%d\n", new_cli_sockfd);

				tcp_cache_count = 0;


				for (i = 0; i < AUD_DEBUG_TCP_LISTEN_MAX; i++)
				{
					if (aud_debug_tcp_watch_fd_list[i] == -1)
					{
						aud_debug_tcp_watch_fd_list[i] = new_cli_sockfd;

						aud_debug_tcp_set_keepalive(new_cli_sockfd);

						//TODO

						break;
					}
				}

				if (i == AUD_DEBUG_TCP_LISTEN_MAX)
				{
					LOGW("only accept %d clients\n", AUD_DEBUG_TCP_LISTEN_MAX);
					close(new_cli_sockfd);
				}
			}

			// search those added fd
			for (i = 0; i < AUD_DEBUG_TCP_LISTEN_MAX; i++)
			{
				if (aud_debug_tcp_watch_fd_list[i] == -1)
				{
					continue;
				}
				if (!FD_ISSET(aud_debug_tcp_watch_fd_list[i], &watchfd))
				{
					continue;
				}

				rcv_len = recv(aud_debug_tcp_watch_fd_list[i], rcv_buf, AUD_DEBUG_TCP_RCV_BUF_LEN, 0);

				FD_CLR(aud_debug_tcp_watch_fd_list[i], &watchfd);

				if (rcv_len <= 0)
				{
					// close this socket
					LOGI("recv close fd:%d\n", aud_debug_tcp_watch_fd_list[i]);
					close(aud_debug_tcp_watch_fd_list[i]);
					aud_debug_tcp_watch_fd_list[i] = -1;
				}
				else
				{
					//LOGI("read count: %d\n", rcv_len);
					aud_debug_tcp_data_recv_handle(rcv_buf, rcv_len);
				}

			}
		}// ret = select
	}

out:

	LOGE("aud_debug_tcp_main exit\n");

	//TODO CLEANUP

	if (rcv_buf)
	{
		os_free(rcv_buf);
		rcv_buf = NULL;
	}

	for (i = 0; i < AUD_DEBUG_TCP_LISTEN_MAX; i++)
	{
		if (aud_debug_tcp_watch_fd_list[i] != -1)
		{
			close(aud_debug_tcp_watch_fd_list[i]);
			aud_debug_tcp_watch_fd_list[i] = -1;
		}
	}

	if (aud_debug_tcp_server_fd != -1)
	{
		close(aud_debug_tcp_server_fd);
		aud_debug_tcp_server_fd = -1;
	}

	GLOBAL_INT_DISABLE();
	aud_debug_tcp_running = 0;
	GLOBAL_INT_RESTORE();

	aud_debug_tcp_task = NULL;
	rtos_delete_thread(NULL);
}

uint32_t bk_aud_debug_tcp_init(void)
{
	int ret;

	LOGI("%s\n", __func__);

	if (tcp_cache_buffer == NULL)
	{
		LOGI("malloc tcp_cache_buffer\n");
		tcp_cache_buffer = (uint8_t *)os_malloc(1600);

		if (tcp_cache_buffer == NULL)
		{
			LOGE("malloc tcp_cache_buffer failed\n");
		}
	}

	if (tcp_send_buffer == NULL)
	{
		tcp_send_buffer = (uint8_t *)os_malloc(TCP_HEAD_SIZE + 1472);
	}

	rtos_init_mutex(&send_lock);

	if (!aud_debug_tcp_task)
	{
		ret = rtos_create_thread(&aud_debug_tcp_task,
		                         4,
		                         "demo_doorbell_tcp",
		                         (beken_thread_function_t)aud_debug_tcp_main,
		                         1024 * 6,
		                         (beken_thread_arg_t)NULL);
		if (ret != kNoErr)
		{
			LOGE("Error: Failed to create spidma_intfer: %d\n", ret);
			return kGeneralErr;
		}
	}

	return kNoErr;
}

bk_err_t bk_aud_debug_tcp_deinit(void)
{
	GLOBAL_INT_DECLARATION();

	LOGI("%s \n", __func__);

	if (aud_debug_tcp_running == 0)
	{
		return BK_OK;
	}

	GLOBAL_INT_DISABLE();
	aud_debug_tcp_running = 0;
	GLOBAL_INT_RESTORE();
	return BK_OK;
}

