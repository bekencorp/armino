/*
 * Copyright (c) 2014-2016 Alibaba Group. All rights reserved.
 * License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include "iot_import.h"
#include "guider_internal.h"


#ifndef CONFIG_GUIDER_AUTH_TIMEOUT
#define CONFIG_GUIDER_AUTH_TIMEOUT  (10 * 1000)
#endif

const char *secmode_str[] = {
    "TCP + Guider + Plain",
    "TCP + Guider + ID2-Crypto",
    "TLS + Direct",
    "TCP + Direct + Plain",
    "TCP + Direct + ID2-Crypto",
    "TLS + Guider + ID2-Authenticate",
    "",
    "TLS + Direct + ID2-Crypto",
    "ITLS + Direct + ID2-Authenticate"
};


#ifdef SUPPORT_SINGAPORE_DOMAIN
int g_domain_type = 1;
#else
int g_domain_type = 0;
#endif /* SUPPORT_SINGAPORE_DOMAIN */


void guider_set_domain_type(int domain_type)
{
    g_domain_type = domain_type;
}

char *guider_get_domain()
{
    if (0 == g_domain_type) {
        return GUIDER_DIRECT_DOMAIN;
    }

    return NULL;
}


int _fill_conn_string(char *dst, int len, const char *fmt, ...)
{
    int                     rc = -1;
    va_list                 ap;
    char                   *ptr = NULL;

    va_start(ap, fmt);
    rc = HAL_Vsnprintf(dst, len, fmt, ap);
    va_end(ap);
    LITE_ASSERT(rc <= len);

    ptr = strstr(dst, "||");
    if (ptr) {
        *ptr = '\0';
    }

    log_debug("dst(%d) = %s.\n", rc, dst);
    return 0;
}


void guider_print_conn_info(iotx_conn_info_pt conn)
{
    log_debug("%s\n", "-----------------------------------------");
    log_debug("%16s : %s\n", "Host", conn->host_name);
    log_debug("%16s : %d\n",  "Port", conn->port);
    // log_debug("%16s : %s\n", "UserName", conn->username);  /* tobe remove */
    // log_debug("%16s : %s\n", "PassWord", conn->password);  /* tobe remove */
    log_debug("%16s : %s.\n", "ClientID", conn->client_id);
    if (conn->pub_key) {
        log_debug("%16s : %p ('%16s ...').\n", "TLS PubKey", conn->pub_key, conn->pub_key);
    }

    log_debug("%s\n", "-----------------------------------------");
}

void guider_print_dev_guider_info(iotx_device_info_pt dev,
                                  char *partner_id,
                                  char *module_id,
                                  char *guider_url,
                                  int secure_mode,
                                  char *time_stamp,
                                  char *guider_sign,
                                  char *id2,
                                  char *dev_code)
{
    log_debug("%s\n", "....................................................");
    log_debug("%20s : %-s\n", "ProductKey", dev->product_key);
    log_debug("%20s : %-s\n", "DeviceName", dev->device_name);
    log_debug("%20s : %-s\n", "DeviceID", dev->device_id);
    log_debug("%20s : %-s\n", "DeviceSecret", dev->device_secret);  /* tobe remove */
    log_debug("%s\n", "....................................................");
    log_debug("%20s : %-s\n", "PartnerID Buf", partner_id);
    log_debug("%20s : %-s\n", "ModuleID Buf", module_id);
    log_debug("%20s : %s\n", "Guider URL", guider_url);
    if (secure_mode > 0) {
        log_debug("%20s : %d (%s)\n", "Guider SecMode", secure_mode, secmode_str[secure_mode]);
    }
    log_debug("%20s : %s\n", "Guider Timestamp", time_stamp);
    log_debug("%s\n", "....................................................");
    /*log_debug("%20s : %s", "Guider Sign", guider_sign);*/ /* remove */
    if (id2 != NULL) {
        log_debug("%20s : %s\n", "Guider ID2", id2);
        log_debug("%20s : %s\n", "Guider DeviceCode", dev_code);
    }
    log_debug("%s\n", "....................................................");

    return;
}

static void guider_get_url(char *buf, int len)
{
#ifdef MQTT_DIRECT
    HAL_Snprintf(buf, len, "%s", "");
#else

    HAL_Snprintf(buf, len, "%s", "http://");

    if (0 == g_domain_type) {
#if defined(ON_PRE)
        strcat(buf, "iot-auth-pre.cn-shanghai.aliyuncs.com");
#elif defined(ON_DAILY)
        strcat(buf, "iot-auth.alibaba.net");
#else
        strcat(buf, "iot-auth.cn-shanghai.aliyuncs.com");
#endif
    } else {
#if defined(ON_PRE)
        strcat(buf, "iot-auth-pre.ap-southeast-1.aliyuncs.com");
#elif defined(ON_DAILY)
        strcat(buf, "iot-auth.alibaba.net");
#else
        strcat(buf, "iot-auth.ap-southeast-1.aliyuncs.com");
#endif
    }

    strcat(buf, "/auth/devicename");

#endif  /* MQTT_DIRECT */

    return;
}

static void guider_get_timestamp_str(char *buf, int len)
{
    HAL_Snprintf(buf, len, "%s", GUIDER_DEFAULT_TS_STR);
    // HAL_Snprintf(buf, len, "%d", rtos_get_time());

    return;
}


static SECURE_MODE guider_get_secure_mode(void)
{
    SECURE_MODE     rc = MODE_TLS_GUIDER;

#ifdef MQTT_DIRECT

#ifdef IOTX_WITHOUT_TLS
    rc = MODE_TCP_DIRECT_PLAIN;
#else
    rc = MODE_TLS_DIRECT;
#endif  /* IOTX_WITHOUT_TLS */

#else   /* MQTT_DIRECT */

#ifdef IOTX_WITHOUT_TLS
    rc = MODE_TCP_GUIDER_PLAIN;
#else
    rc = MODE_TLS_GUIDER;
#endif  /* IOTX_WITHOUT_TLS */

#endif  /* MQTT_DIRECT */

    return  rc;
}

#ifndef MQTT_DIRECT
static char *guider_set_auth_req_str(char sign[], char ts[])
{
#define AUTH_STRING_MAXLEN  (1024)

    char                   *ret = NULL;
    iotx_device_info_pt     dev = NULL;
    int                     rc = -1;

    dev = iotx_device_info_get();
    LITE_ASSERT(dev);

    ret = HAL_Malloc(AUTH_STRING_MAXLEN);
    LITE_ASSERT(ret);
    memset(ret, 0, AUTH_STRING_MAXLEN);

    rc = sprintf(ret,
                 "productKey=%s&" "deviceName=%s&" "signmethod=%s&" "sign=%s&"
                 "version=default&" "clientId=%s&" "timestamp=%s&" "resources=mqtt"
                 , dev->product_key
                 , dev->device_name
#if USING_SHA1_IN_HMAC
                 , SHA_METHOD
#else
                 , MD5_METHOD
#endif
                 , sign
                 , dev->device_id
                 , ts);
    LITE_ASSERT(rc < AUTH_STRING_MAXLEN);

    return ret;
}

#endif  /* MQTT_DIRECT */

int iotx_guider_authenticate(void)
{
    // char                partner_id[PID_STRLEN_MAX + 16] = {0};
    // char                module_id[MID_STRLEN_MAX + 16] = {0};
    char                guider_url[GUIDER_URL_LEN] = {0};
    SECURE_MODE         secure_mode = MODE_TLS_GUIDER;
    char                guider_sign[GUIDER_SIGN_LEN] = {0};
    char                timestamp_str[GUIDER_TS_LEN] = {0};

    iotx_device_info_pt dev = iotx_device_info_get();
    iotx_conn_info_pt   conn = iotx_conn_info_get();
    char               *req_str = NULL;
    char               *mac_str = mqtt_get_mac_str();
    int                 gw = 0;
    int                 ext = 0;

    LITE_ASSERT(dev);
    LITE_ASSERT(conn);

    // _ident_partner(partner_id, sizeof(partner_id));
    // _ident_module(module_id, sizeof(module_id));
    guider_get_url(guider_url, sizeof(guider_url));
    secure_mode = guider_get_secure_mode();
    guider_get_timestamp_str(timestamp_str, sizeof(timestamp_str));
    // _calc_hmac_signature(guider_sign, sizeof(guider_sign), timestamp_str);

    guider_print_dev_guider_info(dev, "", "", guider_url, secure_mode,
                                 timestamp_str, guider_sign, NULL, NULL);

#ifndef MQTT_DIRECT
    char            iotx_conn_host[HOST_ADDRESS_LEN + 1] = {0};
    uint16_t        iotx_conn_port = 1883;
    char            iotx_id[GUIDER_IOT_ID_LEN + 1] = {0};
    char            iotx_token[GUIDER_IOT_TOKEN_LEN + 1] = {0};


    req_str = guider_set_auth_req_str(guider_sign, timestamp_str);
    LITE_ASSERT(req_str);
    log_debug("req_str = '%s'.\n", req_str);

#endif

    /* Start Filling Connection Information */
    conn->pub_key = iotx_ca_get();

#ifdef MQTT_DIRECT
#if defined (ON_DAILY)   /* daily*/
    conn->port = 1883;
    _fill_conn_string(conn->host_name, sizeof(conn->host_name),
                      "10.125.0.27");
#elif defined (ON_PRE) || defined (ON_PRE3)    /* pre */
    conn->port = 80;
    _fill_conn_string(conn->host_name, sizeof(conn->host_name),
                      "100.67.80.75");
#else /* online */
    conn->port = 1883;
    // _fill_conn_string(conn->host_name, sizeof(conn->host_name),
    //                   "%s.%s",
    //                   dev->product_key,
    //                   guider_get_domain());
    _fill_conn_string(conn->host_name, sizeof(conn->host_name),
                      "222.71.10.2");
#endif
    // _fill_conn_string(conn->username, sizeof(conn->username),
    //                   "%s&%s",
    //                   dev->device_name,
    //                   dev->product_key);
    // _fill_conn_string(conn->password, sizeof(conn->password),
    //                   "%s",
    //                   guider_sign);


#else   /* MQTT_DIRECT */

    conn->port = iotx_conn_port;
    _fill_conn_string(conn->host_name, sizeof(conn->host_name),
                      "%s",
                      iotx_conn_host);
    _fill_conn_string(conn->username, sizeof(conn->username), "%s", iotx_id);
    _fill_conn_string(conn->password, sizeof(conn->password), "%s", iotx_token);

#endif  /* MQTT_DIRECT */

#ifdef GATEWAY_SUPPORT
    gw = 1;
#endif

#ifdef RRPC_NEW
    ext = 1;
#endif

    _fill_conn_string(conn->client_id, sizeof(conn->client_id),
                      "%s.%s"
                      "|securemode=%d"
#if USING_SHA1_IN_HMAC
                      ",timestamp=%s,signmethod=" SHA_METHOD ",gw=%d" ",ext=%d"
#else
                      ",timestamp=%s,signmethod=" MD5_METHOD ",gw=%d" ",ext=%d"
#endif
                      // "%s"
                      // "%s"
                      "|"
                      , dev->device_id, mac_str
                      , secure_mode
                      , timestamp_str
                      , gw
                      , ext
                      // , partner_id
                      // , module_id
                     );

    guider_print_conn_info(conn);

    if (req_str) {
        HAL_Free(req_str);
    }

    return 0;

}

