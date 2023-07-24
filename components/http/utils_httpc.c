/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */
#include <string.h>
#include <stddef.h>
#include <common/bk_include.h>
#include "utils_timer.h"
#include "lite-log.h"
#include "utils_httpc.h"
#include "bk_uart.h"
#include <os/mem.h>
#include <os/str.h>

#if HTTP_WR_TO_FLASH
#if CONFIG_FLASH_ORIGIN_API
#include "bk_flash.h"
#else
#include "driver/flash.h"
#endif
#endif
#if (CONFIG_TFM_FWU)
#include <driver/flash.h>
#include "partitions.h"
#include "CheckSumUtils.h"
#define OTA_MAGIC_WORD "\x62\x65\x6B\x65\x6E\x2E\x2E\x2E"
#define MANIFEST_SIZE  (4 * 1024)
#endif

#ifdef CONFIG_HTTP_AB_PARTITION
    #include "modules/ota.h"
    extern part_flag update_part_flag;
#endif
#if CONFIG_HTTP
#define HTTPCLIENT_MIN(x,y) (((x)<(y))?(x):(y))
#define HTTPCLIENT_MAX(x,y) (((x)>(y))?(x):(y))

#define HTTPCLIENT_AUTHB_SIZE     128

#define HTTPCLIENT_CHUNK_SIZE     256
#define HTTPCLIENT_SEND_BUF_SIZE  2048

#define HTTPCLIENT_MAX_HOST_LEN   64
#define HTTPCLIENT_MAX_URL_LEN    256
#define HTTPCLIENT_MAX_PORT_LEN 6

#define HTTP_RETRIEVE_MORE_DATA   (1)            /**< More data needs to be retrieved. */

#if CONFIG_FLASH_ORIGIN_API
extern void flash_protection_op(UINT8 mode, PROTECT_TYPE type);
#endif
#if CONFIG_OTA_TFTP
#define HTTP_FLASH_WR_BUF_MAX WR_BUF_MAX
#else
#define HTTP_FLASH_WR_BUF_MAX   1024
#endif


HTTP_DATA_ST bk_http = {
	.http_total = 0,
	.do_data = 0,
#if HTTP_WR_TO_FLASH
	.wr_buf = NULL,
	.wr_last_len = 0,
	.wr_tmp_buf = NULL,
#endif
};
HTTP_DATA_ST *bk_http_ptr = &bk_http;
#if HTTP_WR_TO_FLASH
static UINT32 ota_wr_block = 0;
#endif
#if (CONFIG_TFM_FWU)
typedef enum {
	OTA_PARSE_HEADER = 0,
	OTA_PARSE_IMG_HEADER,
	OTA_PARSE_IMG,
} ota_parse_type;

typedef struct ota_parse_s {
	ota_parse_type phase;
	ota_header_t ota_header;
	ota_image_header_t *ota_image_header;
	UINT32 offset;
	UINT32 index;
	CRC32_Context ota_crc;
	uint32_t total_rx_len;
} ota_parse_t;

typedef struct ota_image_info_s {
	uint32_t partition_offset;
	uint32_t partition_size;
	uint8_t fwu_image_id;
} ota_partition_info_t;

const ota_partition_info_t s_ota_partition_info[] = {
	{CONFIG_PRIMARY_MANIFEST_PHY_PARTITION_OFFSET,   CONFIG_PRIMARY_MANIFEST_PHY_PARTITION_SIZE,   FWU_IMAGE_TYPE_PRIMARY_MANIFEST},
	{CONFIG_SECONDARY_MANIFEST_PHY_PARTITION_OFFSET, CONFIG_SECONDARY_MANIFEST_PHY_PARTITION_SIZE, FWU_IMAGE_TYPE_SECONDARY_MANIFEST},
	{CONFIG_PRIMARY_BL2_PHY_PARTITION_OFFSET,        CONFIG_PRIMARY_BL2_PHY_PARTITION_SIZE,        FWU_IMAGE_TYPE_PRIMARY_BL2},
	{CONFIG_SECONDARY_BL2_PHY_PARTITION_OFFSET,      CONFIG_SECONDARY_BL2_PHY_PARTITION_SIZE,      FWU_IMAGE_TYPE_SECONDARY_BL2},
	{CONFIG_SECONDARY_ALL_PHY_PARTITION_OFFSET,      CONFIG_SECONDARY_ALL_PHY_PARTITION_SIZE,      FWU_IMAGE_TYPE_FULL}
};
static ota_parse_t ota_parse = {0};

static int http_ota_parse_header(uint8_t **data, int *len);
static int http_ota_parse_image_header(uint8_t **data, int *len);
static int http_ota_parse_data(char *data, int len);
static void http_ota_init(void);
static int http_ota_deinit(void);
#endif

// static int httpclient_parse_host(const char *url, char *host, uint32_t maxhost_len);
static int httpclient_parse_url(const char *url, char *scheme, uint32_t max_scheme_len, char *host,
								uint32_t maxhost_len, int *port, char *path, uint32_t max_path_len);
static int httpclient_conn(httpclient_t *client);
static int httpclient_recv(httpclient_t *client, char *buf, int min_len, int max_len, int *p_read_len,
						   uint32_t timeout);
static int httpclient_retrieve_content(httpclient_t *client, char *data, int len, uint32_t timeout,
									   httpclient_data_t *client_data);
static int httpclient_response_parse(httpclient_t *client, char *data, int len, uint32_t timeout,
									 httpclient_data_t *client_data);

static void httpclient_base64enc(char *out, const char *in)
{
	const char code[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	int i = 0, x = 0, l = 0;

	for (; *in; in++) {
		x = x << 8 | *in;
		for (l += 8; l >= 6; l -= 6)
			out[i++] = code[(x >> (l - 6)) & 0x3f];
	}
	if (l > 0) {
		x <<= 6 - l;
		out[i++] = code[x & 0x3f];
	}
	for (; i % 4;)
		out[i++] = '=';
	out[i] = '\0';
}

int httpclient_conn(httpclient_t *client)
{
	if (0 != client->net.connect(&client->net)) {
		log_err("establish connection failed");
		return ERROR_HTTP_CONN;
	}

	return SUCCESS_RETURN;
}



int httpclient_parse_url(const char *url, char *scheme, uint32_t max_scheme_len, char *host, uint32_t maxhost_len,
						 int *port, char *path, uint32_t max_path_len)
{
	char *scheme_ptr = (char *) url;
	char *host_ptr = (char *) os_strstr(url, "://");
	char port_array[HTTPCLIENT_MAX_PORT_LEN] = {0};
	uint32_t host_len = 0;
	uint32_t port_len = 0;
	uint32_t path_len;
	char *port_ptr;
	char *path_ptr;
	char *fragment_ptr;


	if (host_ptr == NULL) {
		log_err("Could not find host");
		return ERROR_HTTP_PARSE; /* URL is invalid */
	}

	if (max_scheme_len < host_ptr - scheme_ptr + 1) {
		/* including NULL-terminating char */
		log_err("Scheme str is too small (%u >= %u)", max_scheme_len, (uint32_t)(host_ptr - scheme_ptr + 1));
		return ERROR_HTTP_PARSE;
	}
	os_memcpy(scheme, scheme_ptr, host_ptr - scheme_ptr);
	scheme[host_ptr - scheme_ptr] = '\0';

	host_ptr += 3;

	path_ptr = os_strchr(host_ptr, '/');
	if (NULL == path_ptr) {
		log_err("invalid path");
		return -1;
	}

	port_ptr = os_strchr(host_ptr, ':');
	if (NULL == port_ptr) {
		*port = 0;
		port_len = 0;
	} else {
		port_len = path_ptr - port_ptr;
		if(port_len < HTTPCLIENT_MAX_PORT_LEN) {
			//port_ptr -> :8192
			os_memcpy(port_array, port_ptr + 1, port_len - 1);
			port_array[port_len - 1] = '\0';
			*port= os_strtoul(port_array, NULL, 10);
		} else {
			log_err("parse port error, port_len(%d).", port_len);
			*port = 0;
			port_len = 0;
		}

	}

	log_info("port (%d) port_len(%d).", *port, port_len);


	host_len = path_ptr - host_ptr - port_len;
	if (host_len == 0) {
		log_err("Parse Host error.");
		return ERROR_HTTP_PARSE;
	}

	if (maxhost_len < host_len + 1) {
		/* including NULL-terminating char */
		log_err("Host str is too long (host_len(%d) >= max_len(%d))", host_len + 1, maxhost_len);
		return ERROR_HTTP_PARSE;
	}
	os_memcpy(host, host_ptr, host_len);
	host[host_len] = '\0';

	fragment_ptr = os_strchr(host_ptr, '#');
	if (fragment_ptr != NULL)
		path_len = fragment_ptr - path_ptr;
	else
		path_len = os_strlen(path_ptr);

	if (max_path_len < path_len + 1) {
		/* including NULL-terminating char */
		log_err("Path str is too small (%d >= %d)", max_path_len, path_len + 1);
		return ERROR_HTTP_PARSE;
	}
	os_memcpy(path, path_ptr, path_len);
	path[path_len] = '\0';

	return SUCCESS_RETURN;
}

// int httpclient_parse_host(const char *url, char *host, uint32_t maxhost_len)
// {
// 	const char *host_ptr = (const char *) os_strstr(url, "://");
// 	uint32_t host_len = 0;
// 	char *path_ptr;

// 	if (host_ptr == NULL) {
// 		log_err("Could not find host");
// 		return ERROR_HTTP_PARSE; /* URL is invalid */
// 	}
// 	host_ptr += 3;

// 	path_ptr = os_strchr(host_ptr, '/');
// 	if (host_len == 0)
// 		host_len = path_ptr - host_ptr;

// 	if (maxhost_len < host_len + 1) {
// 		/* including NULL-terminating char */
// 		log_err("Host str is too small (%d >= %d)", maxhost_len, host_len + 1);
// 		return ERROR_HTTP_PARSE;
// 	}
// 	os_memcpy(host, host_ptr, host_len);
// 	host[host_len] = '\0';

// 	return SUCCESS_RETURN;
// }

int httpclient_get_info(httpclient_t *client, char *send_buf, int *send_idx, char *buf,
						uint32_t len) /* 0 on success, err code on failure */
{
	int ret;
	int cp_len;
	int idx = *send_idx;

	if (len == 0)
		len = os_strlen(buf);

	do {
		if ((HTTPCLIENT_SEND_BUF_SIZE - idx) >= len)
			cp_len = len;
		else
			cp_len = HTTPCLIENT_SEND_BUF_SIZE - idx;

		os_memcpy(send_buf + idx, buf, cp_len);
		idx += cp_len;
		len -= cp_len;

		if (idx == HTTPCLIENT_SEND_BUF_SIZE) {
			//            if (client->remote_port == HTTPS_PORT)
			//            {
			//                WRITE_IOT_ERROR_LOG("send buffer overflow");
			//                return ERROR_HTTP;
			//            }
			//ret = httpclient_tcp_send_all(client->handle, send_buf, HTTPCLIENT_SEND_BUF_SIZE);
			ret = client->net.write(&client->net, send_buf, HTTPCLIENT_SEND_BUF_SIZE, 5000);
			if (ret)
				return (ret);
		}
	} while (len);

	*send_idx = idx;
	return SUCCESS_RETURN;
}

void httpclient_set_custom_header(httpclient_t *client, char *header)
{
	client->header = header;
}

int httpclient_basic_auth(httpclient_t *client, char *user, char *password)
{
	if ((os_strlen(user) + os_strlen(password)) >= HTTPCLIENT_AUTHB_SIZE)
		return ERROR_HTTP;
	client->auth_user = user;
	client->auth_password = password;
	return SUCCESS_RETURN;
}

int httpclient_send_auth(httpclient_t *client, char *send_buf, int *send_idx)
{
	char b_auth[(int)((HTTPCLIENT_AUTHB_SIZE + 3) * 4 / 3 + 1)];
	char base64buff[HTTPCLIENT_AUTHB_SIZE + 3];

	httpclient_get_info(client, send_buf, send_idx, "Authorization: Basic ", 0);
	sprintf(base64buff, "%s:%s", client->auth_user, client->auth_password);
	log_debug("bAuth: %s", base64buff) ;
	httpclient_base64enc(b_auth, base64buff);
	b_auth[os_strlen(b_auth) + 1] = '\0';
	b_auth[os_strlen(b_auth)] = '\n';
	log_debug("b_auth:%s", b_auth) ;
	httpclient_get_info(client, send_buf, send_idx, b_auth, 0);
	return SUCCESS_RETURN;
}

int httpclient_send_header(httpclient_t *client, const char *url, int method, httpclient_data_t *client_data)
{
	char scheme[8] = { 0 };
	char *host;
	char *path;
	int len;
	char *send_buf;
	char *buf;
	char *meth = (method == HTTPCLIENT_GET) ? "GET" : (method == HTTPCLIENT_POST) ? "POST" :
				 (method == HTTPCLIENT_PUT) ? "PUT" : (method == HTTPCLIENT_DELETE) ? "DELETE" :
				 (method == HTTPCLIENT_HEAD) ? "HEAD" : "";
	int ret;
	int port;
	int rc = SUCCESS_RETURN;

	if (NULL == (host = (char *)os_malloc(HTTPCLIENT_MAX_HOST_LEN))) {
		log_err("not enough memory");
		return FAIL_RETURN;
	}
	if (NULL == (path = (char *)os_malloc(HTTPCLIENT_MAX_URL_LEN))) {
		log_err("not enough memory");
		rc = FAIL_RETURN;
		goto GO_ERR_3;
	}
	if (NULL == (send_buf = (char *)os_malloc(HTTPCLIENT_SEND_BUF_SIZE))) {
		log_err("not enough memory");
		rc = FAIL_RETURN;
		goto GO_ERR_2;
	}
	if (NULL == (buf = (char *)os_malloc(HTTPCLIENT_SEND_BUF_SIZE))) {
		log_err("not enough memory");
		rc = FAIL_RETURN;
		goto GO_ERR_1;
	}
	/* First we need to parse the url (http[s]://host[:port][/[path]]) */
	int res = httpclient_parse_url(url, scheme, sizeof(scheme), host, HTTPCLIENT_MAX_HOST_LEN, &port, path, HTTPCLIENT_MAX_URL_LEN);
	if (res != SUCCESS_RETURN) {
		log_err("httpclient_parse_url returned %d", res);
		//return res;
		rc = res;
		goto GO_ERR;
	}

	//if (client->remote_port == 0)
	//{
	if (os_strcmp(scheme, "http") == 0) {
		//client->remote_port = HTTP_PORT;
	} else if (os_strcmp(scheme, "https") == 0) {
		//client->remote_port = HTTPS_PORT;
	}
	//}

	/* Send request */
	os_memset(send_buf, 0, HTTPCLIENT_SEND_BUF_SIZE);
	os_memset(buf, 0, HTTPCLIENT_SEND_BUF_SIZE);
	len = 0; /* Reset send buffer */

	snprintf(buf, HTTPCLIENT_SEND_BUF_SIZE, "%s %s HTTP/1.1\r\nHost: %s\r\n", meth, path, host); /* Write request */
	ret = httpclient_get_info(client, send_buf, &len, buf, os_strlen(buf));
	if (ret) {
		log_err("Could not write request");
		//return ERROR_HTTP_CONN;
		rc = ERROR_HTTP_CONN;
		goto GO_ERR;
	}

	/* Send all headers */
	if (client->auth_user) {
		httpclient_send_auth(client, send_buf, &len); /* send out Basic Auth header */
	}

	/* Add user header information */
	if (client->header)
		httpclient_get_info(client, send_buf, &len, (char *) client->header, os_strlen(client->header));

	if (client_data->post_buf != NULL) {
		snprintf(buf, HTTPCLIENT_SEND_BUF_SIZE, "Content-Length: %d\r\n", client_data->post_buf_len);
		httpclient_get_info(client, send_buf, &len, buf, os_strlen(buf));

		if (client_data->post_content_type != NULL) {
			snprintf(buf, HTTPCLIENT_SEND_BUF_SIZE, "Content-Type: %s\r\n", client_data->post_content_type);
			httpclient_get_info(client, send_buf, &len, buf, os_strlen(buf));
		}
	}

	/* Close headers */
	httpclient_get_info(client, send_buf, &len, "\r\n", 0);

	log_multi_line(LOG_DEBUG_LEVEL, "REQUEST", "%s", send_buf, ">");

	//ret = httpclient_tcp_send_all(client->net.handle, send_buf, len);
	ret = client->net.write(&client->net, send_buf, len, 5000);
	if (ret > 0)
		log_debug("Written %d bytes", ret);
	else if (ret == 0) {
		log_err("ret == 0,Connection was closed by server");
		//return ERROR_HTTP_CLOSED; /* Connection was closed by server */
		rc = ERROR_HTTP_CLOSED;
	} else {
		log_err("Connection error (send returned %d)", ret);
		//return ERROR_HTTP_CONN;
		rc = ERROR_HTTP_CONN;
	}
GO_ERR:
	os_free(buf);
GO_ERR_1:
	os_free(send_buf);
GO_ERR_2:
	os_free(path);
GO_ERR_3:
	os_free(host);
	return rc;//SUCCESS_RETURN;
}

int httpclient_send_userdata(httpclient_t *client, httpclient_data_t *client_data)
{
	int ret = 0;

	if (client_data->post_buf && client_data->post_buf_len) {
		log_debug("client_data->post_buf: %s", client_data->post_buf);
		{
			//ret = httpclient_tcp_send_all(client->handle, (char *)client_data->post_buf, client_data->post_buf_len);
			ret = client->net.write(&client->net, (char *)client_data->post_buf, client_data->post_buf_len, 5000);
			if (ret > 0)
				log_debug("Written %d bytes", ret);
			else if (ret == 0) {
				log_err("ret == 0,Connection was closed by server");
				return ERROR_HTTP_CLOSED; /* Connection was closed by server */
			} else {
				log_err("Connection error (send returned %d)", ret);
				return ERROR_HTTP_CONN;
			}
		}
	}

	return SUCCESS_RETURN;
}

/* 0 on success, err code on failure */
int httpclient_recv(httpclient_t *client, char *buf, int min_len, int max_len, int *p_read_len, uint32_t timeout_ms)
{
	int ret = 0;
	iotx_time_t timer;

	iotx_time_init(&timer);
	utils_time_countdown_ms(&timer, timeout_ms);

	*p_read_len = 0;

	ret = client->net.read(&client->net, buf, max_len, iotx_time_left(&timer));
	//log_debug("Recv: | %s", buf);

	if (ret > 0)
		*p_read_len = ret;
	else if (ret == 0) {
		//timeout
		return ERROR_HTTP_CONN;
	} else if (-1 == ret) {
		log_info("Connection closed.");
		return ERROR_HTTP_CONN;
	} else {
		log_err("Connection error (recv returned %d)", ret);
		return ERROR_HTTP_CONN;
	}
	log_info("%u bytes has been read", *p_read_len);
	return 0;


}

/*
 * when bt INT effect the OTA update
 */
#define S_WAKE_UP    (0)
#define S_SLEEP      (1)
#define S_POWER_OFF  (2)
#define S_NO_BT      (3)

#define ERASE_TOUCH_TIMEOUT  (3000)//ms
#define ERASE_FLASH_TIMEOUT  (56)//ms
#define WRITE_FLASH_TIMEOUT  (4)//ms

static u32  bt_sleepend_time = -1;  //when cb return,bt is sleep ,recoeding the bt will sleep how long
static u32  bt_cb_anchor_time = 0;    //when cb return ,record current time ;
static u8   bt_sleep_state = S_NO_BT;  //record bt state;the default is 3(S_NO_BT);

void ble_sleep_cb(uint8_t is_sleeping, uint32_t slp_period)
{
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();

    bt_sleep_state = is_sleeping ;
    bt_cb_anchor_time = rtos_get_time();
    if (is_sleeping == S_SLEEP)
    {
       bt_sleepend_time = bt_cb_anchor_time + slp_period/32;
    }

    GLOBAL_INT_RESTORE();
}

static int ble_callback_deal_handler(uint32_t deal_flash_time)
{
    uint32_t  cur_time =0;
    uint32_t  temp_time = 0;
    int       ret_val = 0;

    cur_time = rtos_get_time();

	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();

    do
    {
        if(bt_sleep_state == S_POWER_OFF)     //poweroff
        {
            ret_val = 1;
            break;
        }
        else if(bt_sleep_state == S_WAKE_UP) //wakeup
        {
            if(cur_time >= bt_cb_anchor_time)
            {
                temp_time = (cur_time - bt_cb_anchor_time);
            }
            else
            {
                temp_time = 0xFFFFFFFF - bt_cb_anchor_time + cur_time;
            }

            if(temp_time >= ERASE_TOUCH_TIMEOUT)
            {
                bt_sleep_state = S_NO_BT;
                ret_val = 1;
                break;
            }

            ret_val = 0;
            break;
        }
        else if(bt_sleep_state == S_SLEEP) //sleep
        {
            if(bt_sleepend_time > bt_cb_anchor_time)
            {
                if(bt_sleepend_time < cur_time)
                {
                     ret_val = 1;
                     break;
                }
                else if(cur_time < bt_cb_anchor_time)
                {
                     ret_val = 1;
                     break;
                }
                else if((bt_sleepend_time - cur_time) >= deal_flash_time)
                {
                     ret_val = 1;
                     break;
                }
                else
                {
                     ret_val = 0;
                     break;
                }
            }
            else
            {
                temp_time = 0;
                if((cur_time > bt_sleepend_time)&&(bt_cb_anchor_time > cur_time))
                {
                     ret_val = 1;
                     break;
                }
                else if(bt_cb_anchor_time <= cur_time)
                {
                    temp_time = 0xFFFFFFFF - cur_time + bt_sleepend_time;
                }
                else
                {
                    temp_time = bt_sleepend_time - cur_time;
                }

                if(temp_time >= deal_flash_time )
                {
                     ret_val = 1;
                     break;
                }
                else
                {
                     ret_val = 0;
                     break;
                }
            }
        }
        else
        {
             ret_val = 1;
             break;
        }
    }while(0);

    GLOBAL_INT_RESTORE();

    return ret_val;
}

#if HTTP_WR_TO_FLASH

void http_flash_wr(UINT8 *src, unsigned len)
{
	UINT32  param;
    UINT32  anchor_time = 0;
    UINT32  temp_time = 0;
    UINT8   flash_erase_ready = 0;

	//GLOBAL_INT_DECLARATION();
	if (bk_http_ptr->flash_address % 0x1000 == 0)
    {
        anchor_time = rtos_get_time();
        while(1)
        {
            flash_erase_ready = ble_callback_deal_handler(ERASE_FLASH_TIMEOUT);
            
            temp_time = rtos_get_time();
            if(temp_time >= anchor_time)
            {
                temp_time -= anchor_time;
            }
            else
            {
                temp_time += (0xFFFFFFFF - anchor_time);
            }
            
            if(temp_time >= ERASE_TOUCH_TIMEOUT)
                flash_erase_ready = 1;
            
            //os_printf("flash_erase_ready~111 :%d\n",flash_erase_ready);
            if(flash_erase_ready == 1)
    	    {
        		param = bk_http_ptr->flash_address;
        		//GLOBAL_INT_DISABLE();
			
#if CONFIG_FLASH_ORIGIN_API
        		ddev_control(bk_http_ptr->flash_hdl, CMD_FLASH_ERASE_SECTOR, (void *)&param);
#else
        		bk_flash_erase_sector(param);
#endif
		      //GLOBAL_INT_RESTORE();
                flash_erase_ready = 0;
                break;
            }
            else
            {
                rtos_delay_milliseconds(2);
            }
       }
	}

	if (((u32)bk_http_ptr->flash_address >= bk_http_ptr->pt->partition_start_addr)
		&& (((u32)bk_http_ptr->flash_address + len) < (bk_http_ptr->pt->partition_start_addr + bk_http_ptr->pt->partition_length)))
		{
            while(1)
            {
                flash_erase_ready = ble_callback_deal_handler(WRITE_FLASH_TIMEOUT);
                
                temp_time = rtos_get_time();
                if(temp_time >= anchor_time)
                {
                    temp_time -= anchor_time;
                }
                else
                {
                    temp_time += (0xFFFFFFFF - anchor_time);
                }                
                
                if(temp_time >= ERASE_TOUCH_TIMEOUT)
                    flash_erase_ready = 1;
                
                //os_printf("flash_erase_ready~222 :%d\n",flash_erase_ready);
                if(flash_erase_ready == 1)
    	        {
            		//GLOBAL_INT_DISABLE();
#if CONFIG_FLASH_ORIGIN_API
            		ddev_write(bk_http_ptr->flash_hdl, (char *)src, len, (u32)bk_http_ptr->flash_address);
#else
            		bk_flash_write_bytes(bk_http_ptr->flash_address, (uint8_t *)src, len);
#endif
            		//GLOBAL_INT_RESTORE();
            		if (bk_http_ptr->wr_tmp_buf) {
            			//GLOBAL_INT_DISABLE();
#if CONFIG_FLASH_ORIGIN_API
            			ddev_read(bk_http_ptr->flash_hdl, (char *)bk_http_ptr->wr_tmp_buf, len, (u32)bk_http_ptr->flash_address);
#else
            			bk_flash_read_bytes(bk_http_ptr->flash_address, (uint8_t *)bk_http_ptr->wr_tmp_buf, len);
#endif
            			//GLOBAL_INT_RESTORE();
            			if (!os_memcmp(src, bk_http_ptr->wr_tmp_buf, len)) {
            			} else
            				os_printf("wr flash write err\n");
            		}

            		bk_http_ptr->flash_address += len;
        		//os_printf("ad %x.\r\n",bk_http_ptr->flash_address);
                    flash_erase_ready = 0;
                    break;
                }
                else
        		{
                    rtos_delay_milliseconds(2);
                }
        	}
        }
}

void http_flash_init(void)
{
#if CONFIG_FLASH_ORIGIN_API
	UINT32 status;
#endif
	bk_http_ptr->wr_buf = NULL;
	bk_http_ptr->wr_tmp_buf = NULL;

	if (!bk_http_ptr->wr_buf) {
		bk_http_ptr->wr_buf = os_malloc(HTTP_FLASH_WR_BUF_MAX * sizeof(char));
		if (! bk_http_ptr->wr_buf)
			os_printf("wr_buf malloc err\r\n");
	}

	if (!bk_http_ptr->wr_tmp_buf) {
		bk_http_ptr->wr_tmp_buf = os_malloc(HTTP_FLASH_WR_BUF_MAX * sizeof(char));
		if (! bk_http_ptr->wr_tmp_buf)
			os_printf("wr_tmp_buf malloc err\r\n");
	}

#if CONFIG_FLASH_ORIGIN_API
	bk_http_ptr->pt = bk_flash_get_info(BK_PARTITION_OTA);
	bk_http_ptr->flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, &status, 0);
	BK_ASSERT(DD_HANDLE_UNVALID != bk_http_ptr->flash_hdl);
#else
#ifndef CONFIG_HTTP_AB_PARTITION
	bk_http_ptr->pt = bk_flash_partition_get_info(BK_PARTITION_OTA);
#else
    if(update_part_flag == UPDATE_B_PART)
    {
        os_printf("UPDATE_B_PART\r\n");
        bk_http_ptr->pt = bk_flash_partition_get_info(BK_PARTITION_APPLICATION1); //temp use APPLICATION1 repalce B_partition.
    }
    else
    {
        os_printf("UPDATE_A_PART\r\n");
        bk_http_ptr->pt = bk_flash_partition_get_info(BK_PARTITION_APPLICATION);//update A_parition.
    }
#endif
#endif

	bk_http_ptr->wr_last_len = 0;
	ota_wr_block = 0;
	bk_http_ptr->flash_address = bk_http_ptr->pt->partition_start_addr;

#if CONFIG_FLASH_ORIGIN_API
	bk_flash_enable_security(FLASH_PROTECT_NONE);
#else
	bk_flash_set_protect_type(FLASH_PROTECT_NONE);
#endif
	os_printf("ota write to 0x%x\r\n", bk_http_ptr->flash_address);
}

void http_flash_deinit(void)
{
	os_free(bk_http_ptr->wr_buf);
	os_free(bk_http_ptr->wr_tmp_buf);
	os_memset(bk_http_ptr, 0, sizeof(HTTP_DATA_ST));

	ota_wr_block = 0;
#if CONFIG_FLASH_ORIGIN_API
	ddev_close(bk_http_ptr->flash_hdl);

	bk_flash_enable_security(FLASH_UNPROTECT_LAST_BLOCK);
#else
	bk_flash_set_protect_type(FLASH_UNPROTECT_LAST_BLOCK);
#endif
	os_printf("write over\r\n");
}

void http_wr_to_flash(char *page, UINT32 len)
{
	UINT8 *tmp;
	UINT32 w_l = 0, i = 0;

	i = 0;
	tmp = (UINT8 *)page;
	while (i < len) {
		w_l = min(len - i, HTTP_FLASH_WR_BUF_MAX - bk_http_ptr->wr_last_len);
		os_memcpy(bk_http_ptr->wr_buf + bk_http_ptr->wr_last_len, tmp + i, w_l);
		i += w_l;
		bk_http_ptr->wr_last_len += w_l;
		if (bk_http_ptr->wr_last_len >= HTTP_FLASH_WR_BUF_MAX) {
			//os_printf(".");
#if CONFIG_OTA_TFTP//support bk ota format
			store_block(ota_wr_block, bk_http_ptr->wr_buf, HTTP_FLASH_WR_BUF_MAX);
			ota_wr_block++;
#else                    //direct wrtie to flash
			http_flash_wr(bk_http_ptr->wr_buf, HTTP_FLASH_WR_BUF_MAX);
#endif
			bk_http_ptr->wr_last_len = 0;
		}
	}
}
#endif
#if (CONFIG_TFM_FWU)
static int http_ota_parse_header(uint8_t **data, int *len)
{
	uint32_t data_len, offset;
	uint8_t *tmp;

	if (*len == 0) return 0;

	if (ota_parse.offset == 0) {
		bk_printf("downloading OTA global header...\r\n");
	}

	tmp = (uint8_t *)&ota_parse.ota_header;
	data_len = sizeof(ota_header_t) - ota_parse.offset;
	if (*len < data_len) {
		os_memcpy(tmp + ota_parse.offset, *data, *len);
		ota_parse.offset += *len;
		return 0;
	} else {
		os_memcpy(tmp + ota_parse.offset, *data, data_len);
		*data += data_len;
		*len -= data_len;

		//TODO check global header magic code!

		/*calculate header crc*/
		offset = sizeof(ota_parse.ota_header.magic) + sizeof(ota_parse.ota_header.crc);
		tmp += offset;
		CRC32_Update(&ota_parse.ota_crc, tmp, sizeof(ota_header_t) - offset);

		/*to next parse*/
		ota_parse.phase = OTA_PARSE_IMG_HEADER;
		ota_parse.offset = 0;
		if (ota_parse.ota_image_header) {
			os_free(ota_parse.ota_image_header);
		}
		offset = ota_parse.ota_header.image_num * sizeof(ota_image_header_t);
		ota_parse.ota_image_header = (ota_image_header_t *)os_malloc(offset);
		if (!ota_parse.ota_image_header) {
			bk_printf("ota parse image header: oom\r\n");
			return BK_FAIL;
		}
		bk_printf("crc %x, version %x, header_len %x, image_num %x\r\n",
			ota_parse.ota_header.crc, ota_parse.ota_header.version, ota_parse.ota_header.header_len, ota_parse.ota_header.image_num);
	}

	return 0;
}

static int http_ota_parse_image_header(uint8_t **data, int *len)
{
	int i;
	uint32_t data_len, offset;
	uint8_t *tmp;

	if (*len == 0) return 0;

	if (ota_parse.offset == 0) {
		bk_printf("downloading OTA image header...\r\n");
	}

	tmp = (uint8_t *)ota_parse.ota_image_header;
	data_len = ota_parse.ota_header.image_num * sizeof(ota_image_header_t) - ota_parse.offset;
	if (*len < data_len) {
		os_memcpy(tmp + ota_parse.offset, *data, *len);
		ota_parse.offset += *len;
		return 0;
	} else {
		os_memcpy(tmp + ota_parse.offset, *data, data_len);
		*data += data_len;
		*len -= data_len;

		/*calculate header crc*/
		offset = ota_parse.ota_header.image_num * sizeof(ota_image_header_t);
		CRC32_Update(&ota_parse.ota_crc, tmp, offset);

		//TODO check image CRC!

		/*to next parse*/
		ota_parse.phase = OTA_PARSE_IMG;
		ota_parse.offset = 0;
		for (i = 0; i < ota_parse.ota_header.image_num; i++) {
			bk_printf("image[%d], image_len=%x, image_offset=%x, flash_offset=%x\r\n", i,
				ota_parse.ota_image_header[i].image_len,
				ota_parse.ota_image_header[i].image_offset,
				ota_parse.ota_image_header[i].flash_offset);
		}
	}

	return 0;
}

static const ota_partition_info_t* http_ota_get_partition_info(void)
{
	uint32_t flash_offset;

	if (ota_parse.phase != OTA_PARSE_IMG
		|| ota_parse.index >= ota_parse.ota_header.image_num) {
		return NULL;
	}

	flash_offset = ota_parse.ota_image_header[ota_parse.index].flash_offset;

	for (uint32_t partition_id = 0; partition_id < sizeof(s_ota_partition_info)/sizeof(ota_partition_info_t); partition_id++) {
		if (flash_offset == s_ota_partition_info[partition_id].partition_offset) {
			return &s_ota_partition_info[partition_id];
		}
	}

	return NULL;
}

static uint32_t http_ota_get_fwu_image_id(void)
{
	const ota_partition_info_t *partition_info = http_ota_get_partition_info();

	if (!partition_info) {
		return FWU_IMAGE_TYPE_INVALID;
	}

	//TODO for BL2, update image_id per boot_flag
	return partition_info->fwu_image_id;
}

static psa_image_id_t http_ota_fwu2psa_image_id(uint32_t fwu_image_id)
{
	return (psa_image_id_t)FWU_CALCULATE_IMAGE_ID(FWU_IMAGE_ID_SLOT_STAGE, fwu_image_id, 0);
}

static int http_ota_handle_image(uint8_t **data, int *len)
{
	if (ota_parse.offset == 0) {
		bk_printf("downloading OTA image%d, expected data len=%x...\r\n", ota_parse.index, ota_parse.ota_image_header[ota_parse.index].image_len);
	}

	do {
		uint32_t fwu_image_id = http_ota_get_fwu_image_id();
		if (fwu_image_id == FWU_IMAGE_TYPE_INVALID) {
			if (*len) {
				bk_printf("Invalid image ID, parse index=%d, parse offset=%x, len=%d, total_rx_len=%x\r\n",
					ota_parse.index, ota_parse.offset, *len, ota_parse.total_rx_len);
				return BK_FAIL;
			}
			return BK_OK;
		}

		bk_ota_set_flag(BIT(fwu_image_id));
		psa_image_id_t psa_image_id = http_ota_fwu2psa_image_id(fwu_image_id);

		uint32_t data_len = ota_parse.ota_image_header[ota_parse.index].image_len - ota_parse.offset;
		if (*len < data_len) {
			psa_fwu_write(psa_image_id, ota_parse.offset, (const void *)*data, *len);
			ota_parse.offset += *len;
			*len = 0;

			//TODO calculate image CRC
		} else {
			psa_fwu_write(psa_image_id, ota_parse.offset, (const void *)*data, data_len);
			*data += data_len;
			*len -= data_len;
	
			bk_printf("downloaded OTA image%d\r\n", ota_parse.index);
			//TODO check image CRC, then we can abort quickly!

			/*to next image*/
			bk_printf("\r\n");
			ota_parse.index++;
			ota_parse.offset = 0;
		}
	} while(*len);

	return BK_OK;
}

static int http_ota_parse_data(char *data, int len)
{
	ota_parse.total_rx_len += len;
	if (ota_parse.phase == OTA_PARSE_HEADER) {
		http_ota_parse_header((uint8_t **)&data, &len);
	}

	if (ota_parse.phase == OTA_PARSE_IMG_HEADER) {
		http_ota_parse_image_header((uint8_t **)&data, &len);
	}

	if (ota_parse.phase == OTA_PARSE_IMG) {
		if (len == 0) return 0;

		CRC32_Update(&ota_parse.ota_crc, data, len);
		http_ota_handle_image((uint8_t **)&data, &len);
	}

	return BK_OK;
}

static void http_ota_dump_partition_info(void)
{
	bk_printf("%8s  %8s  %8s\r\n", "offset", "size", "fwu_id");
	for (uint32_t partition_id = 0; partition_id < sizeof(s_ota_partition_info)/sizeof(ota_partition_info_t); partition_id++) {
		const ota_partition_info_t *p = &s_ota_partition_info[partition_id];
		bk_printf("%8x  %8x  %-6d\r\n", p->partition_offset, p->partition_size, p->fwu_image_id);
	}
}

static void http_ota_init(void)
{
	http_ota_dump_partition_info();
	os_memset(&ota_parse, 0, sizeof(ota_parse_t));
	bk_ota_clear_flag();
	CRC32_Init(&ota_parse.ota_crc);
	bk_flash_set_protect_type(FLASH_PROTECT_NONE);
}

static int http_ota_deinit(void)
{
	uint32_t crc_final;

	CRC32_Final(&ota_parse.ota_crc, &crc_final);
	if (ota_parse.ota_image_header) {
		os_free(ota_parse.ota_image_header);
		ota_parse.ota_image_header = NULL;
	}

	bk_printf("crc %x:%x\r\n", crc_final, ota_parse.ota_header.crc);
	if (crc_final != ota_parse.ota_header.crc) {
		return -1;
	}

	return 0;
}
#endif


#if CONFIG_UVC_OTA_DEMO
static http_data_process_callback_func http_data_process_cb = NULL;
void http_data_process_register_callback(http_data_process_callback_func cb)
{
    http_data_process_cb = cb;
}
#endif

void http_data_process(char *buf, UINT32 len, UINT32 recived, UINT32 total)
{
#if CONFIG_UVC_OTA_DEMO
	if(http_data_process_cb)
		http_data_process_cb(buf, len, recived, total);
	else
#endif

#if HTTP_WR_TO_FLASH
		http_wr_to_flash(buf, len);
		os_printf("cyg_recvlen_per:(%.2f)%%\r\n",(((float)(recived))/(total))*100);
#else
#if (CONFIG_TFM_FWU)
	http_ota_parse_data(buf, len);
#else
	os_printf("d");
#endif
#endif

}

int httpclient_retrieve_content(httpclient_t *client, char *data, int len, uint32_t timeout_ms,
								httpclient_data_t *client_data)
{
	int count = 0;
	int templen = 0;
	int crlf_pos;
	iotx_time_t timer;
	char *b_data = NULL;
    int ret = 0;
	iotx_time_init(&timer);
	utils_time_countdown_ms(&timer, timeout_ms);

	/* Receive data */
	log_debug("Current data: %s", data);

	client_data->is_more = true;

	if (client_data->response_content_len == -1 && client_data->is_chunked == false) {
		while (true) {
			int max_len;
			if (count + len < client_data->response_buf_len - 1) {
				os_memcpy(client_data->response_buf + count, data, len);
				count += len;
				client_data->response_buf[count] = '\0';
			} else {
				os_memcpy(client_data->response_buf + count, data, client_data->response_buf_len - 1 - count);
				client_data->response_buf[client_data->response_buf_len - 1] = '\0';
				return HTTP_RETRIEVE_MORE_DATA;
			}

			max_len = HTTPCLIENT_MIN(HTTPCLIENT_CHUNK_SIZE - 1, client_data->response_buf_len - 1 - count);
			ret = httpclient_recv(client, data, 1, max_len, &len, iotx_time_left(&timer));

			/* Receive data */
			log_debug("data len: %d %d", len, count);

			if (ret == ERROR_HTTP_CONN) {
				log_debug("ret == ERROR_HTTP_CONN");
				return ret;
			}

			if (len == 0) {
				/* read no more data */
				log_debug("no more len == 0");
				client_data->is_more = false;
				return SUCCESS_RETURN;
			}
		}
	}

	while (true) {
		uint32_t readLen = 0;

		if (client_data->is_chunked && client_data->retrieve_len <= 0) {
			/* Read chunk header */
			bool foundCrlf;
			int n;
			do {
				foundCrlf = false;
				crlf_pos = 0;
				data[len] = 0;
				if (len >= 2) {
					for (; crlf_pos < len - 2; crlf_pos++) {
						if (data[crlf_pos] == '\r' && data[crlf_pos + 1] == '\n') {
							foundCrlf = true;
							break;
						}
					}
				}
				if (!foundCrlf) {
					/* Try to read more */
					if (len < HTTPCLIENT_CHUNK_SIZE) {
						int new_trf_len;
						ret = httpclient_recv(client,
											  data + len,
											  0,
											  HTTPCLIENT_CHUNK_SIZE - len - 1,
											  &new_trf_len,
											  iotx_time_left(&timer));
						len += new_trf_len;
						if (ret == ERROR_HTTP_CONN)
							return ret;
						else
							continue;
					} else
						return ERROR_HTTP;
				}
			} while (!foundCrlf);
			data[crlf_pos] = '\0';
			n = sscanf(data, "%x", &readLen);/* chunk length */
			client_data->retrieve_len = readLen;
			client_data->response_content_len += client_data->retrieve_len;
			if (n != 1) {
				log_err("Could not read chunk length");
				return ERROR_HTTP_UNRESOLVED_DNS;
			}

			os_memmove(data, &data[crlf_pos + 2], len - (crlf_pos + 2)); /* Not need to move NULL-terminating char any more */
			len -= (crlf_pos + 2);

			if (readLen == 0) {
				/* Last chunk */
				client_data->is_more = false;
				log_debug("no more (last chunk)");
				break;
			}
		} else
			readLen = client_data->retrieve_len;

		log_debug("Total-Payload: %d Bytes; Read: %d Bytes", readLen, len);
#if HTTP_WR_TO_FLASH
		http_flash_init();
		http_wr_to_flash(data, len);
#endif
#if (CONFIG_TFM_FWU)
		http_ota_init();
		http_ota_parse_data(data, len);
#endif

		b_data =  os_malloc((TCP_LEN_MAX + 1) * sizeof(char));
		bk_http_ptr->do_data = 1;
		bk_http_ptr->http_total = readLen - len;
		do {
			templen = HTTPCLIENT_MIN(len, readLen);
			if (count + templen < client_data->response_buf_len - 1) {
				count += templen;
				client_data->response_buf[count] = '\0';
				client_data->retrieve_len -= templen;
			} else {
				client_data->response_buf[client_data->response_buf_len - 1] = '\0';
				client_data->retrieve_len -= (client_data->response_buf_len - 1 - count);
				ret = HTTP_RETRIEVE_MORE_DATA;
                break;
			}

			if (len > readLen) {
				log_debug("memmove %d %d %d\n", readLen, len, client_data->retrieve_len);
				os_memmove(b_data, &b_data[readLen], len - readLen); /* chunk case, read between two chunks */
				len -= readLen;
				readLen = 0;
				client_data->retrieve_len = 0;
			} else
				readLen -= len;

			if (readLen) {
				int max_len = HTTPCLIENT_MIN(TCP_LEN_MAX, client_data->response_buf_len - 1 - count);
				max_len = HTTPCLIENT_MIN(max_len, readLen);

				ret = httpclient_recv(client, b_data, 1, max_len, &len, iotx_time_left(&timer));
				if (ret == ERROR_HTTP_CONN)
                    break;
			}

			if (readLen == len) {
				readLen = 0;
			}
		} while (readLen);

		bk_http_ptr->do_data = 0;
		os_free(b_data);
		b_data = NULL;
        if(ret != 0)
        {
            return ret;
        }
        
		if (client_data->is_chunked) {
			if (len < 2) {
				int new_trf_len;
				/* Read missing chars to find end of chunk */
				ret = httpclient_recv(client, data + len, 2 - len, HTTPCLIENT_CHUNK_SIZE - len - 1, &new_trf_len,
									  iotx_time_left(&timer));
				if (ret == ERROR_HTTP_CONN)
					return ret;
				len += new_trf_len;
			}
			if ((data[0] != '\r') || (data[1] != '\n')) {
				log_err("Format error, %s", data); /* after memmove, the beginning of next chunk */
				return ERROR_HTTP_UNRESOLVED_DNS;
			}
			os_memmove(data, &data[2], len - 2); /* remove the \r\n */
			len -= 2;
		} else {
			log_debug("no more (content-length)");
#if HTTP_WR_TO_FLASH
#if CONFIG_OTA_TFTP//support bk ota format
			store_block(ota_wr_block, bk_http_ptr->wr_buf, bk_http_ptr->wr_last_len);
#else                    //direct wrtie to flash
			http_flash_wr(bk_http_ptr->wr_buf, bk_http_ptr->wr_last_len);
#endif
			http_flash_deinit();
#endif
#if (CONFIG_TFM_FWU)
			if (http_ota_deinit() != 0) {
				return FAIL_RETURN;
			}
#endif
			client_data->is_more = false;
			break;
		}

	}

	return SUCCESS_RETURN;
}

int httpclient_response_parse(httpclient_t *client, char *data, int len, uint32_t timeout_ms,
							  httpclient_data_t *client_data)
{
	int crlf_pos;
	iotx_time_t timer;

	iotx_time_init(&timer);
	utils_time_countdown_ms(&timer, timeout_ms);

	client_data->response_content_len = -1;

	char *crlf_ptr = os_strstr(data, "\r\n");
	if (crlf_ptr == NULL) {
		log_err("\r\n not found");
		return ERROR_HTTP_UNRESOLVED_DNS;
	}

	crlf_pos = crlf_ptr - data;
	data[crlf_pos] = '\0';

	/* Parse HTTP response */
	if (sscanf(data, "HTTP/%*d.%*d %d %*[^\r\n]", &(client->response_code)) != 1) {
		/* Cannot match string, error */
		log_err("Not a correct HTTP answer : %s\n", data);
		return ERROR_HTTP_UNRESOLVED_DNS;
	}

	if ((client->response_code < 200) || (client->response_code >= 400)) {
		/* Did not return a 2xx code; TODO fetch headers/(&data?) anyway and implement a mean of writing/reading headers */
		log_warning("Response code %d", client->response_code);
	}

	log_debug("Reading headers%s", data);

	os_memmove(data, &data[crlf_pos + 2], len - (crlf_pos + 2) + 1); /* Be sure to move NULL-terminating char as well */
	len -= (crlf_pos + 2);

	client_data->is_chunked = false;

	/* Now get headers */
	while (true) {
		char key[32];
		char value[32];
		int n;

		key[31] = '\0';
		value[31] = '\0';

		crlf_ptr = os_strstr(data, "\r\n");
		if (crlf_ptr == NULL) {
			if (len < HTTPCLIENT_CHUNK_SIZE - 1) {
				int new_trf_len, ret;
				ret = httpclient_recv(client, data + len, 1, HTTPCLIENT_CHUNK_SIZE - len - 1, &new_trf_len, iotx_time_left(&timer));
				len += new_trf_len;
				data[len] = '\0';
				log_debug("Read %d chars; In buf: [%s]", new_trf_len, data);
				if (ret == ERROR_HTTP_CONN)
					return ret;
				else
					continue;
			} else {
				log_debug("header len > chunksize");
				return ERROR_HTTP;
			}
		}

		crlf_pos = crlf_ptr - data;
		if (crlf_pos == 0) {
			/* End of headers */
			os_memmove(data, &data[2], len - 2 + 1); /* Be sure to move NULL-terminating char as well */
			len -= 2;
			break;
		}

		data[crlf_pos] = '\0';

		n = sscanf(data, "%31[^:]: %31[^\r\n]", key, value);
		if (n == 2) {
			log_debug("Read header : %s: %s", key, value);
			if (!os_strcmp(key, "Content-Length")) {
				sscanf(value, "%d", &(client_data->response_content_len));
				client_data->retrieve_len = client_data->response_content_len;
			} else if (!os_strcmp(key, "Transfer-Encoding")) {
				if (!os_strcmp(value, "Chunked") || !os_strcmp(value, "chunked")) {
					client_data->is_chunked = true;
					client_data->response_content_len = 0;
					client_data->retrieve_len = 0;
				}
			}
			os_memmove(data, &data[crlf_pos + 2], len - (crlf_pos + 2) + 1); /* Be sure to move NULL-terminating char as well */
			len -= (crlf_pos + 2);

		} else {
			log_err("Could not parse header");
			return ERROR_HTTP;
		}
	}

	if (client->response_code != 200) {
		os_printf("Could not found\r\n");
		return MQTT_SUB_INFO_NOT_FOUND_ERROR;
	}

	return httpclient_retrieve_content(client, data, len, iotx_time_left(&timer), client_data);
}


iotx_err_t httpclient_connect(httpclient_t *client)
{
	client->net.handle = 0;

	return httpclient_conn(client);
}

iotx_err_t httpclient_send_request(httpclient_t *client, const char *url, int method, httpclient_data_t *client_data)
{
	int ret = ERROR_HTTP_CONN;

	if (0 == client->net.handle) {
		log_debug("not connection have been established");
		return ret;
	}

	ret = httpclient_send_header(client, url, method, client_data);
	if (ret != 0) {
		log_err("httpclient_send_header is error,ret = %d", ret);
		return ret;
	}

	if (method == HTTPCLIENT_POST || method == HTTPCLIENT_PUT)
		ret = httpclient_send_userdata(client, client_data);

	return ret;
}

iotx_err_t httpclient_recv_response(httpclient_t *client, uint32_t timeout_ms, httpclient_data_t *client_data)
{
	int reclen = 0, ret = ERROR_HTTP_CONN;
	char buf[HTTPCLIENT_CHUNK_SIZE] = { 0 };
	iotx_time_t timer;

	iotx_time_init(&timer);
	utils_time_countdown_ms(&timer, timeout_ms);

	if (0 == client->net.handle) {
		log_debug("not connection have been established");
		return ret;
	}


	if (client_data->is_more) {
		client_data->response_buf[0] = '\0';
		ret = httpclient_retrieve_content(client, buf, reclen, iotx_time_left(&timer), client_data);
	} else {
		client_data->is_more = 1;
		ret = httpclient_recv(client, buf, 1, HTTPCLIENT_CHUNK_SIZE - 1, &reclen, iotx_time_left(&timer));
		if (ret != 0)
			return ret;

		buf[reclen] = '\0';

		if (reclen) {
			log_multi_line(LOG_DEBUG_LEVEL, "RESPONSE", "%s", buf, "<");
			ret = httpclient_response_parse(client, buf, reclen, iotx_time_left(&timer), client_data);
		}
	}

	return ret;
}

void httpclient_close(httpclient_t *client)
{
	if (client->net.handle > 0)
		client->net.disconnect(&client->net);
	client->net.handle = 0;
}

int httpclient_common(httpclient_t *client, const char *url, int port, const char *ca_crt, int method,
					  uint32_t timeout_ms,
					  httpclient_data_t *client_data)
{
	iotx_time_t timer;
	int ret = 0;
	int url_port = 0;
	char *host = NULL;
	char *path = NULL;
	char scheme[8] = { 0 };

	do {
		if (NULL == (host = (char *)os_zalloc(HTTPCLIENT_MAX_HOST_LEN))) {
			log_err("not enough memory");
			ret = FAIL_RETURN;
			break;
		}

		if (NULL == (path = (char *)os_zalloc(HTTPCLIENT_MAX_URL_LEN))) {
			log_err("not enough memory");
			ret = FAIL_RETURN;
			break;
		}

		if (0 == client->net.handle) {
			//Establish connection if no.
			// httpclient_parse_host(url, host, sizeof(host));

			/* First we need to parse the url (http[s]://host[:port][/[path]]) */
			ret = httpclient_parse_url(url, scheme, sizeof(scheme), host, HTTPCLIENT_MAX_HOST_LEN, &url_port, path, HTTPCLIENT_MAX_URL_LEN);
			if (ret != SUCCESS_RETURN) {
				log_err("httpclient_parse_url returned %d", ret);
				break;
			}

			log_info("host: '%s', port: %d, url_port: %d.", host, port, url_port);

			if(url_port == 0) {
				url_port = port;
			}

			iotx_net_init(&client->net, host, url_port, ca_crt);

			ret = httpclient_connect(client);
			if (0 != ret) {
				log_err("httpclient_connect is error,ret = %d", ret);
				httpclient_close(client);
				break;
			}

			ret = httpclient_send_request(client, url, method, client_data);
			if (0 != ret) {
				log_err("httpclient_send_request is error,ret = %d", ret);
				httpclient_close(client);
				break;
			}
		}

		iotx_time_init(&timer);
		utils_time_countdown_ms(&timer, timeout_ms);
#ifdef CONFIG_HTTP_OTA_WITH_BLE
#if (CONFIG_BLE)
        bk_ble_register_sleep_state_callback(ble_sleep_cb);
#endif
#endif
		if ((NULL != client_data->response_buf)
			|| (0 != client_data->response_buf_len)) {
			ret = httpclient_recv_response(client, iotx_time_left(&timer), client_data);
			if (ret < 0) {
				log_err("httpclient_recv_response is error,ret = %d", ret);
				#if HTTP_WR_TO_FLASH
				http_flash_deinit();
				httpclient_close(client);
				#endif
				break;
			}
		}

		if (! client_data->is_more) {
			//Close the HTTP if no more data.
			log_info("close http channel");
			httpclient_close(client);
		}

		ret = (ret >= 0)? 0 : -1;
	} while(0);

	if (NULL != host) {
		os_free(host);
		host = NULL;
	}

	if (NULL != path) {
		os_free(path);
		path = NULL;
	}

	return ret;
}

int utils_get_response_code(httpclient_t *client)
{
	return client->response_code;
}

iotx_err_t iotx_post(
	httpclient_t *client,
	const char *url,
	int port,
	const char *ca_crt,
	uint32_t timeout_ms,
	httpclient_data_t *client_data)
{
	return httpclient_common(client, url, port, ca_crt, HTTPCLIENT_POST, timeout_ms, client_data);
}
#endif
