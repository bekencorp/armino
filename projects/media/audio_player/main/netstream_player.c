// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "webclient.h"
#include "netstream_player.h"
#include "audio_player.h"
#include "audio_codec.h"

#define TAG  "netstream_player"

#define TU_QITEM_COUNT      10
#define GET_HEADER_BUFSZ    1024
#define GET_RESP_BUFSZ      1024 * 20

static beken_thread_t netstream_player_thread_hdl = NULL; 
static beken_queue_t netstream_player_msg_queue = NULL; 
static struct webclient_session* session = NULL;
static unsigned char *netstream_buffer = NULL;
static char *uri = NULL;
uint8_t netstream_download_stop_flag = 0;
uint32_t psram_buffer = 0x60000000;
unsigned char *psram_end_addr = NULL;


static bk_err_t netstream_player_start(const char *net_uri)
{
	int ret = BK_OK;
	int bytes_read, resp_status;
	int content_length = -1;
	uint32_t *paddr = NULL;
	paddr = (uint32_t *)psram_buffer;

	netstream_download_stop_flag = 0;

	if (net_uri == NULL) {
		BK_LOGE(TAG, "[%s] netstream uri is null\r\n", __func__);
		return BK_FAIL;
	}

	uri = web_strdup(net_uri);
	if (uri == NULL) {
		BK_LOGE(TAG, "[%s] no memory for create get request uri buffer\r\n", __func__);
		return BK_FAIL;
	}

	/* create webclient session and set header response size */
	session = webclient_session_create(GET_HEADER_BUFSZ);
	if (session == NULL) {
		BK_LOGE(TAG, "create webclient session fail\r\n");
		web_free(uri);
		return BK_FAIL;
	}

	/* send GET request by default header */
	if ((resp_status = webclient_get(session, uri)) != 200) {
		BK_LOGE(TAG, "webclient GET request failed, response(%d) error\r\n", resp_status);
		web_free(uri);
		webclient_close(session);
		return BK_FAIL;
	}

	BK_LOGI(TAG, "webclient get response data: \r\n");
	content_length = webclient_content_length_get(session);
	psram_end_addr = (unsigned char *)psram_buffer + content_length;
	BK_LOGI(TAG, "content_length = %x, psram_end_addr = 0x%x\r\n", content_length, psram_end_addr);

	if (content_length < 0) {
		BK_LOGI(TAG, "webclient GET request type is chunked.\n");
		do
		{
			bytes_read = webclient_read(session, (void *)netstream_buffer, GET_RESP_BUFSZ);
			if (bytes_read <= 0) {
				BK_LOGI(TAG, "webclient read complete\r\n");
				ret = BK_OK;
				break;
			}

			rtos_delay_milliseconds(2);
		} while (1);
	} else {
		int content_pos = 0;

		do
		{
			if (!netstream_download_stop_flag) {
				bytes_read = webclient_read(session, (void *)netstream_buffer,
								content_length - content_pos > GET_RESP_BUFSZ ?
								GET_RESP_BUFSZ : content_length - content_pos);

				if (bytes_read <= 0) {
					BK_LOGI(TAG, "webclient get response data complete\r\n");
					if (uri) {
						web_free(uri);
					}

					if (session) {
						webclient_close(session);
					}
					break;
				}

				if(bytes_read != GET_RESP_BUFSZ) {
					if (bytes_read % 4) {
						bytes_read = (bytes_read / 4 + 1) * 4;
					}
					os_memcpy_word((uint32_t *)paddr, (uint32_t *)netstream_buffer, bytes_read);
				} else {
					os_memcpy_word((uint32_t *)paddr, (uint32_t *)netstream_buffer, GET_RESP_BUFSZ);
					paddr += (GET_RESP_BUFSZ / 4);
				}

				if (content_pos == GET_RESP_BUFSZ * 3) {
					ret = audio_codec_mp3_decoder_start();
					if (ret != BK_OK) {
						break;
					}

					ret = audio_player_play();
					if (ret != BK_OK) {
						break;
					}
				}

				content_pos += bytes_read;
				rtos_delay_milliseconds(2);
			} else {
				content_length = -1;
				web_free(uri);
				webclient_close(session);
				BK_LOGI(TAG, "netstream download has been stopped\r\n");
				break;
			}
		} while (content_pos < content_length);

	}

	return ret;
}

static void netstream_player_main(beken_thread_arg_t param)
{
	int ret = 0;

	netstream_buffer = (unsigned char *)web_malloc(GET_RESP_BUFSZ);
	if (netstream_buffer == NULL) {
		BK_LOGE(TAG, "no memory for receive buffer\r\n");
		goto __exit;
	}

	while(1) {
		netstream_player_msg_t msg;
		ret = rtos_pop_from_queue(&netstream_player_msg_queue, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			switch (msg.op) {
				case NETSTREAM_PLAYER_START:
					ret = netstream_player_start(msg.netstream_uri);
					if (ret != BK_OK) {
						BK_LOGE(TAG, "netstream player start fail\r\n");
					}
					break;

				case NETSTREAM_PLAYER_STOP:
					netstream_player_stop();

					ret = audio_player_stop();
					if (ret != BK_OK) {
						BK_LOGE(TAG, "audio player stop fail\r\n");
					}
					break;

				case NETSTREAM_PLAYER_EXIT:
					goto __exit;
					break;

				default:
					break;
			}
		}
	}

__exit:
	if (uri) {
		web_free(uri);
	}

	if (session) {
		webclient_close(session);
	}

	if (netstream_buffer) {
		web_free(netstream_buffer);
	}

	rtos_deinit_queue(&netstream_player_msg_queue);
	netstream_player_msg_queue = NULL;

	netstream_player_thread_hdl = NULL;
	rtos_delete_thread(NULL);
}

bk_err_t netstream_player_stop(void)
{
	if (netstream_download_stop_flag) {

	} else {
		netstream_download_stop_flag = 1;
	}

	BK_LOGI(TAG, "netstream player stop complete\r\n");

	return BK_OK;
}

bk_err_t netstream_player_send_msg(netstream_player_msg_t msg)
{
	bk_err_t ret = BK_OK;

	if (netstream_player_msg_queue) {
		ret = rtos_push_to_queue(&netstream_player_msg_queue, &msg, BEKEN_NO_WAIT);
		if (ret != kNoErr) {
			BK_LOGE(TAG, "netstream player send message fail\r\n");
			return kGeneralErr;
		}
		return ret;
	}
	return kNoResourcesErr;
}

bk_err_t netstream_player_init(void)
{
	bk_err_t ret = BK_OK;

	ret = rtos_init_queue(&netstream_player_msg_queue,
						  "netstream_download_queue",
						  sizeof(netstream_player_msg_t),
						  TU_QITEM_COUNT);
	if (ret != kNoErr) {
		BK_LOGE(TAG, "create netstream player message queue fail\r\n");
		return BK_FAIL;
	}

	ret = rtos_create_thread(&netstream_player_thread_hdl,
							 5,
							 "netstream_download",
							 (beken_thread_function_t)netstream_player_main,
							 1024 * 2,
							 NULL);
	if (ret != kNoErr) {
		rtos_deinit_queue(&netstream_player_msg_queue);
		netstream_player_msg_queue = NULL;
		BK_LOGE(TAG, "create netstream player thread fail\r\n");
		netstream_player_thread_hdl = NULL;
	}
	return ret;
}

bk_err_t netstream_player_deinit(void)
{
	bk_err_t ret = BK_OK;

	netstream_player_stop();

	netstream_player_msg_t download_msg;
	download_msg.op = NETSTREAM_PLAYER_EXIT;
	download_msg.netstream_uri = NULL;
	ret = netstream_player_send_msg(download_msg);
	if (ret != kNoErr) {
		os_printf("netstream player send msg: %d fail\r\n", download_msg.op);
	}

	return ret;
}

