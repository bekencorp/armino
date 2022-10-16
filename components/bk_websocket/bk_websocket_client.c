// Copyright 2021-2022 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//	   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include <components/log.h>
#include <lwip/sockets.h>
#include <os/os.h>
#ifdef CONFIG_WEBSOCKET
#include <bk_websocket_client.h>
#include <http_parser.h>
#include <mbedtls/sha1.h>
#include "mbedtls/base64.h"
#include <netdb.h>


#define WEBSOCKET_TCP_DEFAULT_PORT		80
#define WEBSOCKET_BUFFER_SIZE_BYTE		(4*1024)
#define WS_BUFFER_SIZE					1600
#define MAX_WEBSOCKET_HEADER_SIZE		16

#define WS_SIZE64					127
#define WS_MASK 					0x80
#define WS_SIZE16					126

#define WEBSOCKET_TASK_PRIORITY 		(4)
#define WEBSOCKET_TASK_STACK			(4*1024)
#define WEBSOCKET_NETWORK_TIMEOUT_MS	(10*1000)
#define WEBSOCKET_PINGPONG_TIMEOUT_SEC	(120)
#define WEBSOCKET_PING_INTERVAL_SEC 	(10)
#define WEBSOCKET_RECONNECT_TIMEOUT_MS	(5*1000)
#define WEBSOCKET_RX_RETRY_COUNT		(10)

websocket_event_cb websocket_cb = NULL;
void bk_websocket_register_cb(websocket_event_cb cb)
{
	websocket_cb = cb;
}
void bk_websocket_push_cb(int32_t event_id, char *event_data, int data_len)
{
	if(websocket_cb)
		(*websocket_cb)(event_id, event_data, data_len);
}

#define TAG "WEBSOCKET"
static uint64_t bk_tick_get_ms(void);
struct timeval* utils_ms_to_timeval(int timeout_ms, struct timeval *tv);
void fill_random(void *buf, size_t len);

#define CO_BIT(pos) (1U<<(pos))
const static int PING_SENT_BIT = CO_BIT(1);
const static int TEXT_SENT_BIT = CO_BIT(2);
const static int CLOSE_SENT_BIT = CO_BIT(3);
int status_bits =0;

static char *trimwhitespace(const char *str);
static char *get_http_header(const char *buffer, const char *key);
static int ws_tcp_close(transport client);
static int ws_tcp_poll_read(int *sockfd, int timeout_ms);
static int ws_tcp_poll_write(int *sockfd, int timeout_ms);
static int ws_tcp_read(int *sockfd, char *buffer, int len, int timeout_ms);
static int ws_tcp_write(int *sockfd, const char *buffer, int len, int timeout_ms);
static int ws_read_payload(transport client, char *buffer, int len, int timeout_ms);
static int ws_read_header(transport client, char *buffer, int len, int timeout_ms);
static int ws_write(transport client, int opcode, int mask_flag, const char *b, int len, int timeout_ms);
static int ws_read(transport client, char *buffer, int len, int timeout_ms);
static int ws_poll_connection_closed(int *sockfd, int timeout_ms);
static int ws_client_recv(transport client);
static bk_err_t set_socket_non_blocking(int fd, bool non_blocking);
static bk_err_t hostname_to_fd(const char *host, size_t hostlen, int port, struct sockaddr_storage *address, int* fd);
static bk_err_t _tcp_connect(int *sockfd, const char *host, int hostlen, int port, int timeout_ms);
static int ws_tcp_connect(int *sockfd, const char *host, int port, int timeout_ms);
static bk_err_t ws_disconnect(transport client);
static int ws_connect(transport client, const char *host, int port, int timeout_ms);
static bk_err_t websocket_client_destory_config(transport client);

static char *trimwhitespace(const char *str)
{
	char *end;

	// Trim leading space
	while (isspace((unsigned char)*str)) str++;

	if (*str == 0) {
		return (char *)str;
	}

	// Trim trailing space
	end = (char *)(str + strlen(str) - 1);
	while (end > str && isspace((unsigned char)*end)) end--;

	// Write new null terminator
	*(end + 1) = 0;

	return (char *)str;
}

static char *get_http_header(const char *buffer, const char *key)
{
	char *found = strcasestr(buffer, key);
	if (found) {
		found += strlen(key);
		char *found_end = strstr(found, "\r\n");
		if (found_end) {
			found_end[0] = 0;

			return trimwhitespace(found);
		}
	}
	return NULL;
}

static void bk_hex_dump(char *s, int length)
{
       //os_printf("bk begin dump:\r\n");
       for (int i = 0; i < length; i++)
              os_printf("%c", *(u8 *)(s+i));
       os_printf("\r\n");
}

static int ws_tcp_close(transport client)
{
	int ret = -1;
	if (client->sockfd >= 0) {
		BK_LOGE(TAG, "%s, sockfd used, closing\r\n", __func__);
		ret = close(client->sockfd);
		client->sockfd = -1;
	}
	return ret;
}

static int ws_tcp_poll_read(int *sockfd, int timeout_ms)
{

	int ret = -1;
	struct timeval timeout;
	fd_set readset;
	fd_set errset;
	FD_ZERO(&readset);
	FD_ZERO(&errset);
	FD_SET(*sockfd, &readset);
	FD_SET(*sockfd, &errset);

	ret = select((*sockfd) + 1, &readset, NULL, &errset, utils_ms_to_timeval(timeout_ms, &timeout));
	if (ret > 0 && FD_ISSET(*sockfd, &errset)) {
		int sock_errno = 0;
		uint32_t optlen = sizeof(sock_errno);
		getsockopt(*sockfd, SOL_SOCKET, SO_ERROR, &sock_errno, &optlen);
		BK_LOGE(TAG, "poll_read select error %d, errno = %s, fd = %d", sock_errno, strerror(sock_errno), sockfd);
		ret = BK_FAIL;
	}
	//BK_LOGD(TAG, "%s, ret = %d\r\n", __func__, ret);
	return ret;
}

static int ws_tcp_poll_write(int *sockfd, int timeout_ms)
{
	int ret = -1;
	struct timeval timeout;
	fd_set writeset;
	fd_set errset;
	FD_ZERO(&writeset);
	FD_ZERO(&errset);
	FD_SET(*sockfd, &writeset);
	FD_SET(*sockfd, &errset);

	ret = select((*sockfd) + 1, NULL, &writeset, &errset, utils_ms_to_timeval(timeout_ms, &timeout));
	if (ret > 0 && FD_ISSET(*sockfd, &errset)) {
		int sock_errno = 0;
		uint32_t optlen = sizeof(sock_errno);
		getsockopt(*sockfd, SOL_SOCKET, SO_ERROR, &sock_errno, &optlen);
		BK_LOGE(TAG, "poll_write select error %d, errno = %s, fd = %d\r\n", sock_errno, strerror(sock_errno), *sockfd);
		ret = BK_FAIL;
	}
	//BK_LOGE(TAG, "%s [sockfd:%d] poll wirte ret:%d\r\n", __func__, *sockfd, ret);
	return ret;
}

static int ws_tcp_read(int *sockfd, char *buffer, int len, int timeout_ms)
{
	int poll;

	if ((poll = ws_tcp_poll_read(sockfd, timeout_ms)) <= 0) {
		return poll;
	}

	int ret = recv(*sockfd, (unsigned char *)buffer, len, 0);
	if (ret < 0) {
		BK_LOGE(TAG, "tcp_read error, errno=%s", strerror(errno));
	}
	if (ret == 0) {
		if (poll > 0) {
			// no error
			BK_LOGE(TAG, "%s select > 0, ret = 0 \r\n", __func__);
		}
		ret = BK_FAIL;
	}
	//BK_LOGE(TAG, "%s, ret:%d\r\n", __func__, ret);
	return ret;
}

static int ws_tcp_write(int *sockfd, const char *buffer, int len, int timeout_ms)
{
	int poll;
	if ((poll = ws_tcp_poll_write(sockfd, timeout_ms)) <= 0) {
		BK_LOGE(TAG, "Poll timeout or error, errno=%s, fd=%d, timeout_ms=%d\r\n", strerror(errno), sockfd, timeout_ms);
		return poll;
	}

	int ret = send((*sockfd),(const unsigned char *) buffer, len, 0);
	if (ret < 0) {
	   BK_LOGE(TAG, "tcp_write error, errno=%s", strerror(errno));
	}
	//BK_LOGE(TAG, "%s, write send len:%d\r\n", __func__, ret);
	return ret;
}

static int ws_read_payload(transport client, char *buffer, int len, int timeout_ms)
{
	int bytes_to_read;
	int rlen = 0;
	transport_ws_t *ws = client->ws_transport;
	if (ws->frame_state.bytes_remaining > len) {
		BK_LOGE(TAG, "Actual data to receive (%d) are longer than ws buffer (%d)\r\n", ws->frame_state.bytes_remaining, len);
		bytes_to_read = len;

	} else {
		bytes_to_read = ws->frame_state.bytes_remaining;
	}

	if (bytes_to_read != 0 && (rlen = ws_tcp_read(&(client->sockfd), buffer, bytes_to_read, timeout_ms)) <= 0) {
		BK_LOGE(TAG, "Error read payload data\r\n");
		return rlen;
	}
	ws->frame_state.bytes_remaining -= rlen;

	if (ws->frame_state.mask_key) {
		for (int i = 0; i < bytes_to_read; i++) {
			buffer[i] = (buffer[i] ^ ws->frame_state.mask_key[i % 4]);
		}
	}
	return rlen;
}

/* Read and parse the WS header, determine length of payload */
static int ws_read_header(transport client, char *buffer, int len, int timeout_ms)
{
	int payload_len;
	transport_ws_t *ws = client->ws_transport;
	char ws_header[MAX_WEBSOCKET_HEADER_SIZE];
	char *data_ptr = ws_header, mask;
	int rlen;
	int poll_read;
	ws->frame_state.header_received = false;
	if ((poll_read = ws_tcp_poll_read(&(client->sockfd), timeout_ms)) <= 0) {
		BK_LOGE(TAG, "error poll read data\r\n");
		return poll_read;
	}

	int header = 2;
	int mask_len = 4;
	if ((rlen = ws_tcp_read(&(client->sockfd), data_ptr, header, timeout_ms)) <= 0) {
		BK_LOGE(TAG, "first header, Error read data\r\n");
		return rlen;
	}
	ws->frame_state.header_received = true;
	ws->frame_state.opcode = (*data_ptr & 0x0F);
	data_ptr ++;
	mask = ((*data_ptr >> 7) & 0x01);
	payload_len = (*data_ptr & 0x7F);
	data_ptr++;
   // BK_LOGE(TAG, "%s, Opcode: %d, mask: %d, payload len: %d\r\n", __func__, ws->frame_state.opcode, mask, payload_len);
	if (payload_len == 126) {
		if ((rlen = ws_tcp_read(&(client->sockfd), data_ptr, header, timeout_ms)) <= 0) {
			BK_LOGE(TAG, "126 read: Error read data\r\n");
			return rlen;
		}
		payload_len = ((data_ptr[0] << 8) & 0xFF00l) | (data_ptr[1] & 0xFFl);
		//BK_LOGE(TAG, "%s, 126, payload_len:%c\r\n", __func__, payload_len);
	} else if (payload_len == 127) {
		header = 8;
		if ((rlen = ws_tcp_read(&(client->sockfd), data_ptr, header, timeout_ms)) <= 0) {
			BK_LOGE(TAG, "127 read: Error read data\r\n");
			return rlen;
		}

		if (data_ptr[0] != 0 || data_ptr[1] != 0 || data_ptr[2] != 0 || data_ptr[3] != 0) {
			// really too big!
			payload_len = 0xFFFFFFFF;
		} else {
			payload_len = data_ptr[4] << 24 | data_ptr[5] << 16 | data_ptr[6] << 8 | data_ptr[7];
		}
		//BK_LOGD(TAG, "%s, 127, payload_len:%d\r\n", __func__, payload_len);
	}

	if (mask) {
		// Read and store mask
		if (payload_len != 0 && (rlen = ws_tcp_read(&(client->sockfd), buffer, mask_len, timeout_ms)) <= 0) {
			BK_LOGE(TAG, "mask error read data\r\n");
			return rlen;
		}
		memcpy(ws->frame_state.mask_key, buffer, mask_len);
	} else {
		memset(ws->frame_state.mask_key, 0, mask_len);
	}

	ws->frame_state.payload_len = payload_len;
	ws->frame_state.bytes_remaining = payload_len;

	return payload_len;
}

static int ws_write(transport client, int opcode, int mask_flag, const char *b, int len, int timeout_ms)
{
	char *buffer = (char *)b;
	char ws_header[MAX_WEBSOCKET_HEADER_SIZE];
	char *mask;
	int header_len = 0, i;
	int poll_write;

	if ((poll_write = ws_tcp_poll_write(&(client->sockfd), timeout_ms)) <= 0) {
		BK_LOGE(TAG, "Error ws_tcp_poll_write");
		return poll_write;
	}

	ws_header[header_len++] = opcode;
	if (len <= 125) {
		ws_header[header_len++] = (uint8_t)(len | mask_flag);
	} else if (len < 65536) {
		ws_header[header_len++] = WS_SIZE16 | mask_flag;
		ws_header[header_len++] = (uint8_t)(len >> 8);
		ws_header[header_len++] = (uint8_t)(len & 0xFF);
	} else {
		ws_header[header_len++] = WS_SIZE64 | mask_flag;
		/* Support maximum 4 bytes length */
		ws_header[header_len++] = 0; //(uint8_t)((len >> 56) & 0xFF);
		ws_header[header_len++] = 0; //(uint8_t)((len >> 48) & 0xFF);
		ws_header[header_len++] = 0; //(uint8_t)((len >> 40) & 0xFF);
		ws_header[header_len++] = 0; //(uint8_t)((len >> 32) & 0xFF);
		ws_header[header_len++] = (uint8_t)((len >> 24) & 0xFF);
		ws_header[header_len++] = (uint8_t)((len >> 16) & 0xFF);
		ws_header[header_len++] = (uint8_t)((len >> 8) & 0xFF);
		ws_header[header_len++] = (uint8_t)((len >> 0) & 0xFF);
	}

	if (mask_flag) {
		mask = &ws_header[header_len];
		fill_random(ws_header + header_len, 4);
		header_len += 4;

		for (i = 0; i < len; ++i) {
			buffer[i] = (buffer[i] ^ mask[i % 4]);
		}
	}
	BK_LOGE(TAG, "%s, ws header len:%d\r\n", __func__, header_len);
	if (ws_tcp_write(&(client->sockfd), ws_header, header_len, timeout_ms) != header_len) {
		BK_LOGE(TAG, "Error write header\r\n");
		return BK_FAIL;
	}

	if (len == 0) {
		return 0;
	}

	BK_LOGE(TAG, "%s, payload buffer len:%d\r\n", __func__, len);
	int ret = ws_tcp_write(&(client->sockfd), buffer, len, timeout_ms);

	if (mask_flag) {
		mask = &ws_header[header_len-4];
		for (i = 0; i < len; ++i) {
			buffer[i] = (buffer[i] ^ mask[i % 4]);
		}
	}
	return ret;;
}

static int ws_read(transport client, char *buffer, int len, int timeout_ms)
{
	int rlen = 0;
	transport_ws_t *ws = client->ws_transport;
	if (ws->frame_state.bytes_remaining <= 0) {

		if ((rlen = ws_read_header(client, buffer, len, timeout_ms)) < 0) {
			BK_LOGE(TAG, "Error reading header %d\r\n", rlen);
			ws->frame_state.bytes_remaining = 0;
			return rlen;
		}

		if (rlen == 0) {
			ws->frame_state.bytes_remaining = 0;
			return BK_OK;
		}
	}

	if (ws->frame_state.payload_len) {
		if ((rlen = ws_read_payload(client, buffer, len, timeout_ms)) <= 0) {
			BK_LOGE(TAG, "Error reading payload data\r\n");
			ws->frame_state.bytes_remaining = 0;
			return rlen;
		}
		BK_LOGE(TAG, "%s, payload len:%d\r\n", __func__, rlen);
	}

	return rlen;
}

static uint64_t bk_tick_get_ms(void)
{
	return rtos_get_time();
}

struct timeval* utils_ms_to_timeval(int timeout_ms, struct timeval *tv)
{
	if (timeout_ms == -1) {
		return NULL;
	}
	tv->tv_sec = timeout_ms / 1000;
	tv->tv_usec = (timeout_ms - (tv->tv_sec * 1000)) * 1000;
	return tv;
}

static void ms_to_timeval(int timeout_ms, struct timeval *tv)
{
	tv->tv_sec = timeout_ms / 1000;
	tv->tv_usec = (timeout_ms % 1000) * 1000;
}

static bk_err_t set_socket_non_blocking(int fd, bool non_blocking)
{
	int flags;
	if ((flags = fcntl(fd, F_GETFL, 0)) < 0) {
		BK_LOGE(TAG, "[sock=%d] get file flags error: %s", fd, strerror(errno));
		return BK_FAIL;
	}

	if (non_blocking) {
		flags |= O_NONBLOCK;
	} else {
		flags &= ~O_NONBLOCK;
	}

	if (fcntl(fd, F_SETFL, flags) < 0) {
		BK_LOGE(TAG, "[sock=%d] set blocking/nonblocking error: %s", fd, strerror(errno));
		return BK_FAIL;
	}
	BK_LOGE(TAG, "[sock=%d] %s, non_block:%d\r\n", fd, __func__, non_blocking);
	return BK_OK;
}

static bk_err_t hostname_to_fd(const char *host, size_t hostlen, int port, struct sockaddr_storage *address, int* fd)
{
	struct addrinfo *address_info;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	char *use_host = strndup(host, hostlen);
	if (!use_host) {
	   return BK_FAIL;
	}

	BK_LOGD(TAG, "host:%s: strlen %lu\r\n", use_host, (unsigned long)hostlen);
	int res = getaddrinfo(use_host, NULL, &hints, &address_info);
	if (res != 0 || address_info == NULL) {
	   BK_LOGE(TAG, "couldn't get hostname for :%s: "
					"getaddrinfo() returns %d, addrinfo=%p", use_host, res, address_info);
		free(use_host);
		return BK_FAIL;
	}
	free(use_host);
	*fd = socket(address_info->ai_family, address_info->ai_socktype, address_info->ai_protocol);
	if (*fd < 0) {
		BK_LOGE(TAG, "Failed to create socket (family %d socktype %d protocol %d)", address_info->ai_family, address_info->ai_socktype, address_info->ai_protocol);
		freeaddrinfo(address_info);
		return BK_FAIL;
	}

	if (address_info->ai_family == AF_INET) {
		struct sockaddr_in *p = (struct sockaddr_in *)address_info->ai_addr;
		p->sin_port = htons(port);
		BK_LOGE(TAG, "[sock=%d] Resolved IPv4 address: %s\r\n", *fd, ipaddr_ntoa((const ip_addr_t*)&p->sin_addr.s_addr));
		memcpy(address, p, sizeof(struct sockaddr ));
	}
#if LWIP_IPV6
	else if (address_info->ai_family == AF_INET6) {
		struct sockaddr_in6 *p = (struct sockaddr_in6 *)address_info->ai_addr;
		p->sin6_port = htons(port);
		p->sin6_family = AF_INET6;
		BK_LOGE(TAG, "[sock=%d] Resolved IPv6 address: %s", *fd, ip6addr_ntoa((const ip6_addr_t*)&p->sin6_addr));
		memcpy(address, p, sizeof(struct sockaddr_in6 ));
	}
#endif
	else {
		BK_LOGE(TAG, "Unsupported protocol family %d", address_info->ai_family);
		close(*fd);
		freeaddrinfo(address_info);
		return BK_FAIL;
	}

	freeaddrinfo(address_info);
	return BK_OK;
}

static bk_err_t _tcp_connect(int *sockfd, const char *host, int hostlen, int port, int timeout_ms)
{
	struct sockaddr_storage address;
	int fd;
	bk_err_t ret = hostname_to_fd(host, hostlen, port, &address, &fd);
	if (ret != BK_OK) {
		BK_LOGE(TAG, "%s error fd\r\n", __func__);
		return ret;
	}

	if(timeout_ms) {
		struct timeval tv;
		ms_to_timeval(timeout_ms, &tv);
		if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) != 0) {
			BK_LOGE(TAG, "Fail to setsockopt SO_RCVTIMEO");
			return BK_FAIL;
		}
		if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) != 0) {
			BK_LOGE(TAG, "Fail to setsockopt SO_SNDTIMEO");
			return BK_FAIL;
		}
	}

	// Set to non block before connecting to better control connection timeout
	ret = set_socket_non_blocking(fd, true);
	if (ret != BK_OK) {
		BK_LOGE(TAG, "%s error nb\r\n", __func__);
		goto err;
	}

	BK_LOGE(TAG, "[sock=%d] Connecting to server. HOST: %s, Port: %d\r\n", fd, host, port);
	if (connect(fd, (struct sockaddr *)&address, sizeof(struct sockaddr)) < 0) {
		if (errno == EINPROGRESS) {
			BK_LOGE(TAG, "EINPROGRESS\r\n");
			fd_set fdset;
			struct timeval tv = { .tv_usec = 0, .tv_sec = 10 }; // Default connection timeout is 10 s

			if (timeout_ms > 0 ) {
				ms_to_timeval(timeout_ms, &tv);
			}
			FD_ZERO(&fdset);
			FD_SET(fd, &fdset);

			int res = select(fd+1, NULL, &fdset, NULL, &tv);
			if (res < 0) {
				BK_LOGE(TAG, "[sock=%d] select() error: %s\r\n", fd, strerror(errno));
				ret = BK_FAIL;
				goto err;
			}
			else if (res == 0) {
				BK_LOGE(TAG, "[sock=%d] select() timeout\r\n", fd);
				ret = BK_FAIL;
				goto err;
			} else {
				int sockerr;
				socklen_t len = (socklen_t)sizeof(int);
				BK_LOGE(TAG, "[sock=%d] select() occur\r\n", fd);
				if (getsockopt(fd, SOL_SOCKET, SO_ERROR, (void*)(&sockerr), &len) < 0) {
					BK_LOGE(TAG, "[sock=%d] getsockopt() error: %s\r\n", fd, strerror(errno));
					ret = BK_FAIL;
					goto err;
				}
				else if (sockerr) {
					BK_LOGE(TAG, "[sock=%d] delayed connect error: %s\r\n", fd, strerror(sockerr));
					ret = BK_FAIL;
					goto err;
				}
				else if (!sockerr) {
					BK_LOGE(TAG, "[sock=%d] no blocking connected\r\n", fd);
				}
			}
		} else {
			BK_LOGE(TAG, "[sock=%d] connect() error: %s\r\n", fd, strerror(errno));
			ret = BK_FAIL;
			goto err;
		}
	}
		// reset back to blocking mode (unless non_block configured)
	ret = set_socket_non_blocking(fd, false);
	if (ret != BK_OK) {
		  BK_LOGE(TAG, "%s error nb\r\n", __func__);
		  goto err;
	}
	*sockfd = fd;
	return BK_OK;

err:
	close(fd);
	return ret;
}

static int ws_tcp_connect(int *sockfd, const char *host, int port, int timeout_ms)
{
	bk_err_t err = _tcp_connect(sockfd, host, os_strlen(host), port, timeout_ms);
	if (err != BK_OK) {
		BK_LOGE(TAG, "%s failed\r\n", __func__);
		return BK_FAIL;
	}

	return BK_OK;
}

void fill_random(void *buf, size_t len)
{
	uint8_t *buf_bytes = (uint8_t *)buf;
	while (len > 0) {
		uint32_t word = (uint32_t)rand();
		uint32_t to_copy = MIN(sizeof(word), len);
		memcpy(buf_bytes, &word, to_copy);
		buf_bytes += to_copy;
		len -= to_copy;
	}
}
static bk_err_t ws_disconnect(transport client)
{
	if(client == NULL) {
		BK_LOGE(TAG, "client aleady null\r\n");
		return BK_FAIL;
	}
	ws_tcp_close(client);
	if(client->auto_reconnect) {
		client->reconnect_tick_ms = bk_tick_get_ms();
	}
	client->state = WEBSOCKET_STATE_WAIT_TIMEOUT;
	bk_websocket_push_cb(WEBSOCKET_EVENT_DISCONNECTED, NULL, 0);
	return BK_OK;
}

static int ws_connect(transport client, const char *host, int port, int timeout_ms)
{
	transport_ws_t *ws = client->ws_transport;

	if(ws_tcp_connect(&(client->sockfd), host, port, timeout_ms) < 0) {
		return BK_FAIL;
	}
	unsigned char random_key[16];
	fill_random(random_key, sizeof(random_key));

	unsigned char client_key[28] = {0};

	const char *user_agent_ptr = (ws->user_agent)?(ws->user_agent):"BEKEN Websocket Client";

	size_t outlen = 0;

	mbedtls_base64_encode(client_key, sizeof(client_key), &outlen, random_key, sizeof(random_key));

	int len = snprintf(ws->buffer, WS_BUFFER_SIZE,
						"GET %s HTTP/1.1\r\n"
						"Connection: Upgrade\r\n"
						"Host: %s:%d\r\n"
						"Origin: http://%s\r\n"
						"User-Agent: %s\r\n"
						"Upgrade: websocket\r\n"
						"Sec-WebSocket-Version: 13\r\n"
						"Sec-WebSocket-Key: %s\r\n",
						ws->path,
						host, port, host, user_agent_ptr,
						client_key);
	os_printf("http request:\r\n");
	bk_hex_dump(ws->buffer, 200);

	//BK_LOGE(TAG, "len: %d\r\n", len);
	if (len <= 0 || len >= WS_BUFFER_SIZE) {
		BK_LOGE(TAG, "Error in request generation, desired request len: %d, buffer size: %d\r\n", len, WS_BUFFER_SIZE);
		return BK_FAIL;
	}

	if (ws->sub_protocol) {
		BK_LOGE(TAG, "sub_protocol: %s\r\n", ws->sub_protocol);
		int r = snprintf(ws->buffer + len, WS_BUFFER_SIZE - len, "Sec-WebSocket-Protocol: %s\r\n", ws->sub_protocol);
		len += r;
		if (r <= 0 || len >= WS_BUFFER_SIZE) {
			BK_LOGE(TAG, "Error in request generation"
						 "(snprintf of subprotocol returned %d, desired request len: %d, buffer size: %d\r\n", r, len, WS_BUFFER_SIZE);
			return BK_FAIL;
		}
	}
	if (ws->headers) {
		BK_LOGE(TAG, "headers: %s\r\n", ws->headers);
		int r = snprintf(ws->buffer + len, WS_BUFFER_SIZE - len, "%s", ws->headers);
		len += r;
		if (r <= 0 || len >= WS_BUFFER_SIZE) {
			BK_LOGE(TAG, "Error in request generation"
					"(strncpy of headers returned %d, desired request len: %d, buffer size: %d\r\n", r, len, WS_BUFFER_SIZE);
			return BK_FAIL;
		}
	}
	int r = snprintf(ws->buffer + len, WS_BUFFER_SIZE - len, "\r\n");
	len += r;
	BK_LOGE(TAG, "%s, connect write len:%d\r\n", __func__, len);
	if (r <= 0 || len >= WS_BUFFER_SIZE) {
		BK_LOGE(TAG, "Error in request generation"
					"(snprintf of header terminal returned %d, desired request len: %d, buffer size: %d\r\n", r, len, WS_BUFFER_SIZE);
		return BK_FAIL;
	}

	if(ws_tcp_write(&(client->sockfd), ws->buffer, len, timeout_ms)<0) {
		BK_LOGE(TAG, "Write FAIL\r\n");
	}

	int header_len = 0;
	do {
		if((len = ws_tcp_read(&(client->sockfd), ws->buffer + header_len, WS_BUFFER_SIZE - header_len, timeout_ms)) <= 0)
		{
			BK_LOGE(TAG, "read FAIL for upgrade header:%s\r\n", ws->buffer);
			return BK_FAIL;
		}
		header_len += len;
		ws->buffer[header_len] = '\0';
		BK_LOGE(TAG, "Read header chunk %d, current header size: %d\r\n", len, header_len);
	} while (NULL == os_strstr(ws->buffer, "\r\n\r\n") && header_len < WS_BUFFER_SIZE);
	os_printf("server buffer:\r\n");
	bk_hex_dump(ws->buffer, 200);


	char *server_key = get_http_header(ws->buffer, "Sec-WebSocket-Accept:");
	if (server_key == NULL) {
		BK_LOGE(TAG, "Sec-WebSocket-Accept not found\r\n");
		return BK_FAIL;
	}

	unsigned char expected_server_sha1[20];
	unsigned char expected_server_key[33] = {0};
	// If you are interested, see https://tools.ietf.org/html/rfc6455
	const char expected_server_magic[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
	unsigned char expected_server_text[sizeof(client_key) + sizeof(expected_server_magic) + 1];
	os_strcpy((char*)expected_server_text, (char*)client_key);
	strcat((char*)expected_server_text, expected_server_magic);

	size_t key_len = os_strlen((char*)expected_server_text);
	int ret = mbedtls_sha1_ret(expected_server_text, key_len, expected_server_sha1);
	if (ret != 0) {
		BK_LOGE(TAG, "Error in calculating sha1 sum , Returned 0x%02X\r\n", ret);
		return ret;
	}
	mbedtls_base64_encode(expected_server_key, sizeof(expected_server_key),  &outlen, expected_server_sha1, sizeof(expected_server_sha1));
	expected_server_key[ (outlen < sizeof(expected_server_key)) ? outlen : (sizeof(expected_server_key) - 1) ] = 0;
	BK_LOGE(TAG, "server key=%s\r\n\r\n", (char *)server_key);
	BK_LOGE(TAG, "send_key=%s\r\n\r\n", (char*)client_key);
	BK_LOGE(TAG, "expected_server_key=%s\r\n\r\n", expected_server_key);
	if (os_strcmp((char*)expected_server_key, (char*)server_key) != 0) {
		BK_LOGE(TAG, "Invalid websocket key\r\n");
		return BK_FAIL;
	}
	return BK_OK;
}

int ws_poll_connection_closed(int *sockfd, int timeout_ms)
{
	struct timeval timeout;
	fd_set readset;
	fd_set errset;
	FD_ZERO(&readset);
	FD_ZERO(&errset);
	FD_SET(*sockfd, &readset);
	FD_SET(*sockfd, &errset);

	int ret = select(*sockfd + 1, &readset, NULL, &errset, utils_ms_to_timeval(timeout_ms, &timeout));
	if (ret > 0) {
		if (FD_ISSET(*sockfd, &readset)) {
			uint8_t buffer;
			if (recv(*sockfd, &buffer, 1, MSG_PEEK) <= 0) {
				// socket is readable, but reads zero bytes -- connection cleanly closed by FIN flag
				return BK_OK;
			}
			BK_LOGW(TAG, "ws_poll_connection_closed: unexpected data readable on socket=%d", *sockfd);
		} else if (FD_ISSET(*sockfd, &errset)) {
			int sock_errno = 0;
			uint32_t optlen = sizeof(sock_errno);
			getsockopt(*sockfd, SOL_SOCKET, SO_ERROR, &sock_errno, &optlen);
			BK_LOGD(TAG, "ws_poll_connection_closed select error %d, errno = %s, fd = %d", sock_errno, strerror(sock_errno), *sockfd);
			if (sock_errno == ENOTCONN || sock_errno == ECONNRESET || sock_errno == ECONNABORTED) {
				return BK_OK;
			}
			BK_LOGE(TAG, "ws_poll_connection_closed: unexpected errno=%d on socket=%d", sock_errno, *sockfd);
		}
		return BK_FAIL;
	}
	return ret;

}


static int ws_client_recv(transport client)
{
	int rlen;
	client->payload_offset = 0;
	transport_ws_t *ws = client->ws_transport;
	do {
		BK_LOGE(TAG, "----------begin receive--------------\r\n");
		rlen = ws_read(client, client->rx_buffer, client->buffer_size, WEBSOCKET_NETWORK_TIMEOUT_MS);
		if (rlen < 0) {
			BK_LOGE(TAG, "Error read data\r\n");
			return BK_FAIL;
		}

		client->payload_len = ws->frame_state.payload_len;
		client->last_opcode = (ws_transport_opcodes_t)ws->frame_state.opcode;

		if (rlen == 0 && client->last_opcode == WS_TRANSPORT_OPCODES_NONE ) {
			BK_LOGE(TAG, "ws read timeouts\r\n");
			return BK_OK;
		}
		bk_websocket_push_cb(WEBSOCKET_EVENT_DATA, client->rx_buffer, rlen);
		client->payload_offset += rlen;
	} while (client->payload_offset < client->payload_len);
	//BK_LOGE(TAG, "%s, len:%d\r\n", __func__, client->payload_len);

	if (client->last_opcode == WS_TRANSPORT_OPCODES_PING) {
		const char *data = (client->payload_len == 0) ? NULL : client->rx_buffer;
		BK_LOGE(TAG, "Received ping, Sending PONG with payload len=%d\r\n", client->payload_len);
		BK_LOGE(TAG, "----------sending pong packet----------\r\n");
		ws_write(client, WS_TRANSPORT_OPCODES_PONG | WS_TRANSPORT_OPCODES_FIN, WS_MASK, data, client->payload_len,
								WEBSOCKET_NETWORK_TIMEOUT_MS);
	} else if (client->last_opcode == WS_TRANSPORT_OPCODES_PONG) {
		BK_LOGE(TAG, "Received pong frame, send ping success\r\n");
		client->wait_for_pong_resp = false;
	} else if (client->last_opcode == WS_TRANSPORT_OPCODES_CLOSE) {
		BK_LOGE(TAG, "Received close frame\r\n");
		client->state = WEBSOCKET_STATE_CLOSING;
	} else if (client->last_opcode == WS_TRANSPORT_OPCODES_TEXT) {
		BK_LOGE(TAG, "Received text frame:\r\n");
		bk_hex_dump(client->rx_buffer, client->payload_len);
	}

	return BK_OK;
}

static bk_err_t websocket_client_destory_config(transport client)
{
	BK_LOGE(TAG, "%s\r\n", __func__);

	if (client == NULL) {
		BK_LOGE(TAG, "%s, client already null, return\r\n", __func__);
		return BK_FAIL;
	}

	if (client->config == NULL) {
		BK_LOGE(TAG, "%s, config already null, return\r\n", __func__);
		return BK_FAIL;
	}
	websocket_config_t *cfg = client->config;
	if(cfg->host)
		os_free(cfg->host);
	if(cfg->path)
		os_free(cfg->path);
	if(cfg->scheme)
		os_free(cfg->scheme);
	if(cfg->username)
		os_free(cfg->username);
	if(cfg->password)
		os_free(cfg->password);
	memset(cfg, 0, sizeof(websocket_config_t));
	if(client->config) {
		os_free(client->config);
	}
	client->config = NULL;
	return BK_OK;
}

static int websocket_client_send_with_opcode(transport client, ws_transport_opcodes_t opcode, const uint8_t *data, int len, int timeout)
{
	int need_write = len;
	int wlen = 0, widx = 0;
	int ret = BK_FAIL;
	BK_LOGE(TAG, "%s: begin send opcode: %d\r\n", __func__, opcode);
	if (client == NULL || len < 0 ||
		(opcode != WS_TRANSPORT_OPCODES_CLOSE && (data == NULL || len <= 0))) {
		BK_LOGE(TAG, "Invalid arguments");
		return BK_FAIL;
	}

	uint32_t current_opcode = opcode;
	while (widx < len || current_opcode) {
		if (need_write > client->buffer_size) {
			need_write = client->buffer_size;
		} else {
			current_opcode |= WS_TRANSPORT_OPCODES_FIN;
		}
		memcpy(client->tx_buffer, data + widx, need_write);
		if(need_write)
			bk_hex_dump(client->tx_buffer, need_write);

		wlen = ws_write(client, current_opcode, WS_MASK, (char *)client->tx_buffer, need_write, timeout);
										//(timeout==portMAX_DELAY)? -1 : timeout * portTICK_PERIOD_MS);
		if (wlen < 0 || (wlen == 0 && need_write != 0)) {
			ret = wlen;
			BK_LOGE(TAG, "Network error: ws_write() returned %d, errno=%d\r\n", ret, errno);
			ws_disconnect(client);
			return ret;
		}
		current_opcode = 0;
		widx += wlen;
		need_write = len - widx;

	}
	ret = widx;
	return ret;
}

int websocket_client_send_text(transport client, const char *data, int len, int timeout)
{
	return websocket_client_send_with_opcode(client, WS_TRANSPORT_OPCODES_TEXT, (const uint8_t *)data, len, timeout);
}

int websocket_client_send_close(transport client, const char *data, int len, int timeout)
{
	if(websocket_client_send_with_opcode(client, WS_TRANSPORT_OPCODES_CLOSE, (const uint8_t *)data, len, timeout) < 0) {
		BK_LOGE(TAG, "Error send close frame\r\n");
		return BK_FAIL;
	}
	status_bits = 0;
	status_bits |= CLOSE_SENT_BIT;
	return BK_OK;
}

bk_err_t websocket_client_set_uri(transport client, const char *uri)
{
	if (client == NULL || uri == NULL) {
		BK_LOGE(TAG, "client has not initialized or uri has not be input\r\n");
		return BK_FAIL;
	}
	struct http_parser_url puri;
	http_parser_url_init(&puri);
	int parser_status = http_parser_parse_url(uri, os_strlen(uri), 0, &puri);
	if (parser_status != 0) {
		BK_LOGE(TAG, "Error parse uri = %s", uri);
		return BK_FAIL;
	}
	if (NULL == (client->config = (websocket_config_t *)os_malloc(sizeof(websocket_config_t)))) {
		BK_LOGE(TAG, "alloc config fail\r\n");
		return BK_FAIL;
	}
	memset(client->config, 0, sizeof(websocket_config_t));

	if (puri.field_data[UF_SCHEMA].len) {
		if (NULL == (client->config->scheme = (char *)os_malloc(puri.field_data[UF_SCHEMA].len))) {
			BK_LOGE(TAG, "alloc scheme fail\r\n");
			return BK_FAIL;
		}
		os_strncpy(client->config->scheme, uri + puri.field_data[UF_SCHEMA].off, puri.field_data[UF_SCHEMA].len);
	}

	if (puri.field_data[UF_HOST].len) {
		asprintf(&client->config->host, "%.*s", puri.field_data[UF_HOST].len, uri + puri.field_data[UF_HOST].off);
	}

	if (puri.field_data[UF_PATH].len || puri.field_data[UF_QUERY].len) {
		if (puri.field_data[UF_QUERY].len == 0) {
			asprintf(&client->config->path, "%.*s", puri.field_data[UF_PATH].len, uri + puri.field_data[UF_PATH].off);
		} else if (puri.field_data[UF_PATH].len == 0) {
			asprintf(&client->config->path, "/?%.*s", puri.field_data[UF_QUERY].len, uri + puri.field_data[UF_QUERY].off);
		} else {
			asprintf(&client->config->path, "%.*s?%.*s", puri.field_data[UF_PATH].len, uri + puri.field_data[UF_PATH].off,
					puri.field_data[UF_QUERY].len, uri + puri.field_data[UF_QUERY].off);
		}
	}

	if (puri.field_data[UF_PORT].off) {
		client->config->port = strtol((const char*)(uri + puri.field_data[UF_PORT].off), NULL, 10);
	} else {
		client->config->port = WEBSOCKET_TCP_DEFAULT_PORT;
	}

	if (puri.field_data[UF_USERINFO].len) {
		char *user_info = NULL;
		asprintf(&user_info, "%.*s", puri.field_data[UF_USERINFO].len, uri + puri.field_data[UF_USERINFO].off);
		if (user_info) {
			char *pass = strchr(user_info, ':');
			if (pass) {
				pass[0] = 0;
				pass ++;
				free(client->config->password);
				client->config->password = strdup(pass);
			}
			free(client->config->username);
			client->config->username = strdup(user_info);
			free(user_info);
		} else {
			return BK_FAIL;
		}
	}
	return BK_OK;
}

transport websocket_client_init(const websocket_client_input_t *input)
{
	transport client = (transport)os_malloc(sizeof(websocket_transport_info_t));
	memset(client, 0, sizeof(websocket_transport_info_t));
	//parse websocket uri to websocket config
	if (input->uri) {
		if (websocket_client_set_uri(client, input->uri) != BK_OK) {
			BK_LOGE(TAG, "set uri fail, client destory\r\n");
			goto _websocket_init_fail;
		}
	}
	//set autoreconnect
	client->auto_reconnect = false;

	//set ws_transport
	client->ws_transport = (transport_ws_t *)os_malloc(sizeof(transport_ws_t));
	memset(client->ws_transport, 0, sizeof(transport_ws_t));
	if(!client->ws_transport) {
		BK_LOGE(TAG, "alloc ws_transport fail\r\n");
		goto _websocket_init_fail;
	}

	if(client->config->path)
		client->ws_transport->path = strdup(client->config->path);
	else
		client->ws_transport->path = strdup("/");

	client->ws_transport->buffer = os_malloc(WS_BUFFER_SIZE);
	if (!client->ws_transport->buffer) {
		BK_LOGE(TAG, "alloc ws_transport buffer fail\r\n");
		goto _websocket_init_fail;
	}

	if (input->subprotocol)
		client->ws_transport->sub_protocol = strdup(input->subprotocol);
	if (input->user_agent)
		client->ws_transport->user_agent = strdup(input->user_agent);
	if (input->headers)
		client->ws_transport->headers = strdup(input->headers);	
	client->ws_transport->frame_state.bytes_remaining = 0;

	//tick...
	client->reconnect_tick_ms = bk_tick_get_ms();
	client->ping_tick_ms = bk_tick_get_ms();
	client->wait_for_pong_resp = false;

	//rx retry
	if(input->rx_retry <= 0)
		client->rx_retry = WEBSOCKET_RX_RETRY_COUNT;
	else
		client->rx_retry = input->rx_retry;

	//buf malloc
	int buffer_size = input->buffer_size;
	if (buffer_size <= 0) {
		buffer_size = WEBSOCKET_BUFFER_SIZE_BYTE;
	}
	client->buffer_size = buffer_size;
	if (NULL == (client->rx_buffer = (char *)os_malloc(buffer_size))) {
		BK_LOGE(TAG, "alloc rx_buffer fail\r\n");
		goto _websocket_init_fail;
	}

	if (NULL == (client->tx_buffer = (char *)os_malloc(buffer_size))) {
		BK_LOGE(TAG, "alloc tx_buffer fail\r\n");
		goto _websocket_init_fail;
	}

	//sockfd
	client->sockfd = -1;

	return client;

_websocket_init_fail:
	websocket_client_destroy(client);
	return NULL;
}


bk_err_t websocket_client_destroy(transport client)
{
	BK_LOGE(TAG, "%s\r\n", __func__);
	if (client == NULL) {
		return BK_FAIL;
	}
	if (client->run) {
		if(client->state >= WEBSOCKET_STATE_CONNECTED) {
			if(websocket_client_send_close(client, NULL, 0, WEBSOCKET_NETWORK_TIMEOUT_MS)) {
				BK_LOGE(TAG, "%s, client send close frame fail\r\n", __func__);
				return BK_FAIL;
			}
		}
		if(websocket_client_stop(client)) {
			BK_LOGE(TAG, "%s, client stop fail\r\n", __func__);
			return BK_FAIL;
		}
	}

	return BK_OK;
}

int test_case_text(transport client)
{
	//send text packet
	char *a = "hello,BEKEN";
	return websocket_client_send_text(client, a, strlen(a), WEBSOCKET_NETWORK_TIMEOUT_MS);
}

void websocket_client_task(beken_thread_arg_t *thread_param)
{

	transport client = (transport) thread_param;

	client->run = true;
	client->state = WEBSOCKET_STATE_INIT;
	int read_select = 0;
	while (client->run) {
		switch ((int)client->state) {
			case WEBSOCKET_STATE_INIT:
				BK_LOGE(TAG, "websocket connecting to %s://%s:%d\r\n", client->config->scheme, client->config->host, client->config->port);
				if (ws_connect(client, client->config->host,
										client->config->port,
										WEBSOCKET_NETWORK_TIMEOUT_MS) < 0) {
					BK_LOGE(TAG, "Error websocket connect\r\n");
					ws_disconnect(client);
					break;
				}
				BK_LOGE(TAG, "websocket connected to %s://%s:%d\r\n", client->config->scheme, client->config->host, client->config->port);
				client->state = WEBSOCKET_STATE_CONNECTED;
				client->wait_for_pong_resp = false;
				bk_websocket_push_cb(WEBSOCKET_EVENT_CONNECTED, NULL, 0);
				break;
			case WEBSOCKET_STATE_CONNECTED:
				BK_LOGD(TAG, "%s, status:%02x\r\n", __func__, status_bits);
				if (bk_tick_get_ms() - client->ping_tick_ms > WEBSOCKET_PING_INTERVAL_SEC*1000) {
					client->ping_tick_ms = bk_tick_get_ms();

					if (status_bits & PING_SENT_BIT) {
						BK_LOGE(TAG, "----------Sending ping packet----------\r\n");
						ws_write(client, WS_TRANSPORT_OPCODES_PING | WS_TRANSPORT_OPCODES_FIN, WS_MASK, NULL, 0, WEBSOCKET_NETWORK_TIMEOUT_MS);
					} else if(status_bits & TEXT_SENT_BIT) {
						BK_LOGE(TAG, "----------Sending text packet----------\r\n");
						test_case_text(client);
					}
					if (!client->wait_for_pong_resp) {
						client->pingpong_tick_ms = bk_tick_get_ms();
						client->wait_for_pong_resp = true;
					}
				}
				 if ( bk_tick_get_ms() - client->pingpong_tick_ms > WEBSOCKET_PINGPONG_TIMEOUT_SEC*1000) {
					 if (client->wait_for_pong_resp) {
						BK_LOGE(TAG, "Error, no PONG received for more than %d seconds after PING\r\n", client->pingpong_tick_ms);
						break;
					 }
				 }

				 if (read_select == 0) {
					BK_LOGD(TAG, "Read poll timeout: skipping read()...\r\n");
					break;
				 }
				 client->ping_tick_ms = bk_tick_get_ms();

				 if (ws_client_recv(client) == BK_FAIL) {
					BK_LOGE(TAG, "Error receive data\r\n");
					ws_disconnect(client);
					break;
				 }
				 break;

			case WEBSOCKET_STATE_WAIT_TIMEOUT:
				 if (!client->auto_reconnect) {
					client->run = false;
					break;
				}
				if (bk_tick_get_ms() - client->reconnect_tick_ms > WEBSOCKET_RECONNECT_TIMEOUT_MS) {
					client->state = WEBSOCKET_STATE_INIT;
					client->reconnect_tick_ms = bk_tick_get_ms();
					BK_LOGE(TAG, "Reconnecting...\r\n");
				}
				break;
			case WEBSOCKET_STATE_CLOSING:
					BK_LOGE(TAG, "Closing initiated by the server, sending close frame\r\n");
					ws_write(client, WS_TRANSPORT_OPCODES_CLOSE | WS_TRANSPORT_OPCODES_FIN, WS_MASK, NULL, 0, WEBSOCKET_NETWORK_TIMEOUT_MS);
				break;
			default:
				BK_LOGE(TAG, "Client run iteration in a default state: %d\r\n", client->state);
				break;
		}

		if (WEBSOCKET_STATE_CONNECTED == client->state) {
			read_select = ws_tcp_poll_read(&(client->sockfd), 1000); //Poll every 1000ms
			if (read_select < 0) {
				BK_LOGE(TAG, "Network error: ws_tcp_poll_read() returned %d, errno=%d\r\n", read_select, errno);
				ws_disconnect(client);
			}
		} else if (WEBSOCKET_STATE_WAIT_TIMEOUT == client->state) {
			if(client->auto_reconnect)
				rtos_delay_milliseconds(WEBSOCKET_RECONNECT_TIMEOUT_MS);
		} else if (WEBSOCKET_STATE_CLOSING == client->state) {
			BK_LOGE(TAG, " Waiting for TCP connection to be closed by the server\r\n");
			int ret = ws_poll_connection_closed(&(client->sockfd), 1000);
			if (ret == 0) {
				// still waiting
				break;
			}
			if (ret < 0) {
				BK_LOGE(TAG, "Connection terminated while waiting for clean TCP close\r\n");
			}
			client->run = false;
			client->state = WEBSOCKET_STATE_UNKNOW;
			bk_websocket_push_cb(WEBSOCKET_EVENT_CLOSED, NULL, 0);
			break;
		}
	}
	BK_LOGE(TAG, "close connection...\r\n");
	ws_tcp_close(client);
	client->state = WEBSOCKET_STATE_UNKNOW;

	if(websocket_client_destory_config(client)) {
		BK_LOGE(TAG, "client config already free\r\n");
	}
	if(client->tx_buffer)
		os_free(client->tx_buffer);
	if(client->rx_buffer)
		os_free(client->rx_buffer);
	os_free(client);
	client = NULL;
	rtos_delete_thread(NULL);
}

int websocket_client_start(transport client)
{
	int ret;

	if (client == NULL) {
		BK_LOGE(TAG, "The client has not be initialized\r\n");
		return BK_FAIL;
	}
	if (client->state >= WEBSOCKET_STATE_INIT) {
		BK_LOGE(TAG, "The client has started\r\n");
		return BK_FAIL;
	}
	ret = rtos_create_thread(NULL,
							 WEBSOCKET_TASK_PRIORITY,
							 "websocketc",
							 (beken_thread_function_t)websocket_client_task,
							 WEBSOCKET_TASK_STACK,
							 (beken_thread_arg_t)client);
	return ret;
}

bk_err_t websocket_client_stop(transport client)
{
	if (client == NULL) {
		BK_LOGW(TAG, "Client null");
		return BK_FAIL;
	}
	if (!client->run) {
		BK_LOGW(TAG, "Client was not started");
		return BK_FAIL;
	}

	client->run = false;
	client->state = WEBSOCKET_STATE_UNKNOW;
	return BK_OK;
}

void *websocket_run = NULL;

/*********************************demo api*********************************/
bk_err_t websocket_start(websocket_client_input_t *websocket_cfg)
{
	if(websocket_run == NULL) {
		transport client = websocket_client_init(websocket_cfg);
		websocket_run = (void *)client;
		BK_LOGE(TAG, "----------connect server-------\r\n");
		if(ws_connect(client, client->config->host, client->config->port, WEBSOCKET_NETWORK_TIMEOUT_MS) < 0){
			BK_LOGE(TAG, "Error websocket connect\r\n");
			bk_websocket_push_cb(WEBSOCKET_EVENT_DISCONNECTED, NULL, 0);
			ws_tcp_close(client);
			return BK_FAIL;
		}
		bk_websocket_push_cb(WEBSOCKET_EVENT_CONNECTED, NULL, 0);
		return BK_OK;
	} else {
		BK_LOGE(TAG, "%s websocket client already start, stop first\r\n", __func__);
		return BK_FAIL;
	}
}
bk_err_t websocket_recv(transport client)
{
	/* listening rx data*/
	int read_select=0;
	int retry = 0;
	while(!read_select) {
		read_select = ws_tcp_poll_read(&(client->sockfd), 1000);
		if (read_select < 0) {
			BK_LOGE(TAG, "Network error: ws_tcp_poll_read() returned %d, errno=%d\r\n", read_select, errno);
			ws_tcp_close(client);
			return BK_FAIL;
		}
		if(read_select == 0) {
			BK_LOGE(TAG, "continue poll\r\n");
			retry++;
		}
		if(retry >= client->rx_retry) {
			BK_LOGE(TAG, "recv timeout\r\n");
			ws_tcp_close(client);
			return BK_FAIL;
		}
	}
	/* receive rx data */
	if (ws_client_recv(client) == BK_FAIL) {
		BK_LOGE(TAG, "Error receive data\r\n");
		ws_tcp_close(client);
		return BK_FAIL;
	}
	return BK_OK;
}

bk_err_t websocket_send_text(websocket_client_input_t *websocket_cfg)
{
	if(websocket_run) {
		transport client = (transport)websocket_run;
		BK_LOGE(TAG, "----------sending text----------\r\n");
		if(websocket_client_send_text(client, websocket_cfg->user_context, strlen(websocket_cfg->user_context), WEBSOCKET_NETWORK_TIMEOUT_MS) < 0) {
			BK_LOGE(TAG, "%s send text fail\r\n", __func__);
			return BK_FAIL;
		}
		return BK_OK;
	}
	else {
		BK_LOGE(TAG, "%s, client already stop\r\n", __func__);
		return BK_FAIL;
	}
}

bk_err_t websocket_send_ping(void)
{
	if(websocket_run) {
		transport client = (transport)websocket_run;
		BK_LOGE(TAG, "----------sending ping----------\r\n");
		if(ws_write(client, WS_TRANSPORT_OPCODES_PING | WS_TRANSPORT_OPCODES_FIN, WS_MASK, NULL, 0, WEBSOCKET_NETWORK_TIMEOUT_MS) < 0) {
			BK_LOGE(TAG, "%s send ping fail\r\n", __func__);
			return BK_FAIL;
		}
		return BK_OK;
	}
	else {
		BK_LOGE(TAG, "%s, client already stop\r\n", __func__);
		return BK_FAIL;
	}
}

bk_err_t websocket_stop(void)
{
	if(websocket_run) {
		BK_LOGE(TAG, "%s stop websocket client\r\n", __func__);
		websocket_client_destroy((transport)websocket_run);
		websocket_run = NULL;
		bk_websocket_push_cb(WEBSOCKET_EVENT_CLOSED, NULL, 0);
		return BK_OK;
	} else {
		BK_LOGE(TAG, "%s, already stop return\r\n", __func__);
		return BK_FAIL;
	}
}

bk_err_t websocket_send_ping_pong(websocket_client_input_t *websocket_cfg)
{
	if(websocket_run == NULL) {
		transport client = websocket_client_init(websocket_cfg);
		websocket_run = (void *)client;
		status_bits = 0;
		status_bits |= PING_SENT_BIT;
		BK_LOGE(TAG, "START ping pong TASK\r\n", __func__);
		if(websocket_client_start(client)) {
			return BK_FAIL;
		}
		return BK_OK;
	}
	else {
		BK_LOGE(TAG, "%s, client already start\r\n", __func__);
		return BK_FAIL;
	}
}

#endif
