/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "iot_import.h"
#include "iot_export.h"

#include "iot_export_mqtt.h"

#define PRODUCT_KEY             "mqtt"
#define PRODUCT_SECRET          "gIH3WXHo84Jq5XtJ"
#define DEVICE_NAME             "test"
#define DEVICE_SECRET           "nMwWRZrjupURGSByK7qu3uCwzEYUHORu"

// These are pre-defined topics
#define TOPIC_UPDATE            "/"PRODUCT_KEY"/"DEVICE_NAME"/update"
#define TOPIC_ERROR             "/"PRODUCT_KEY"/"DEVICE_NAME"/update/error"
#define TOPIC_GET               "/"PRODUCT_KEY"/"DEVICE_NAME"/get"
#define TOPIC_DATA              "/"PRODUCT_KEY"/"DEVICE_NAME"/data"

#define MSG_LEN_MAX             (2048)

int cnt = 0;
static int is_subscribed = 0;


void *gpclient;

char *msg_buf = NULL, *msg_readbuf = NULL;

iotx_mqtt_topic_info_t topic_msg;
char msg_pub[128];

static void mqtt_publish(void *pclient);
int mqtt_client_example(void);

void release_buff() {
    if (NULL != msg_buf) {
        LITE_free(msg_buf);
    }

    if (NULL != msg_readbuf) {
        LITE_free(msg_readbuf);
    }
}

// static void wifi_service_event(void *event, void *priv_data) {
//     // if (event->type != EV_WIFI) {
//     //     return;
//     // }

//     // if (event->code != CODE_WIFI_ON_GOT_IP) {
//     //     return;
//     // }
//     log_info("wifi_service_event!");
//     mqtt_client_example();
// }

static void mqtt_service_event(void *event, void *priv_data) {

    // if (event->type != EV_SYS) {
    //     return;
    // }

    // if (event->code != CODE_SYS_ON_MQTT_READ) {
    //     return;
    // }
    log_info("mqtt_service_event!.\n");
    mqtt_publish(priv_data);
}

static void _demo_message_arrive(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    iotx_mqtt_topic_info_pt ptopic_info = (iotx_mqtt_topic_info_pt) msg->msg;

    // print topic name and topic message
    log_info("----\n");
    log_info("Topic: '%.*s' (Length: %d).\n",
                  ptopic_info->topic_len,
                  ptopic_info->ptopic,
                  ptopic_info->topic_len);
    log_info("Payload: '%.*s' (Length: %d).\n",
                  ptopic_info->payload_len,
                  ptopic_info->payload,
                  ptopic_info->payload_len);
    log_info("----\n");
}

/*
 * Subscribe the topic: IOT_MQTT_Subscribe(pclient, TOPIC_DATA, IOTX_MQTT_QOS1, _demo_message_arrive, NULL);
 * Publish the topic: IOT_MQTT_Publish(pclient, TOPIC_DATA, &topic_msg);
 */
static void mqtt_publish(void *pclient) {

    int rc = -1;

    if(is_subscribed == 0) {
        /* Subscribe the specific topic */
        rc = IOT_MQTT_Subscribe(pclient, TOPIC_DATA, IOTX_MQTT_QOS1, _demo_message_arrive, NULL);
        if (rc<0) {
            // IOT_MQTT_Destroy(&pclient);
             log_info("IOT_MQTT_Subscribe() failed, rc = %d.\n", rc);
        }
        is_subscribed = 1;
    }
    else{
        /* Initialize topic information */
        memset(&topic_msg, 0x0, sizeof(iotx_mqtt_topic_info_t));

        topic_msg.qos = IOTX_MQTT_QOS1;
        topic_msg.retain = 0;
        topic_msg.dup = 0;

        /* Generate topic message */
        int msg_len = snprintf(msg_pub, sizeof(msg_pub), "{\"attr_name\":\"temperature\", \"attr_value\":\"%d\"}", cnt);
        if (msg_len < 0) {
            log_info("Error occur! Exit program.\n");
        }

        topic_msg.payload = (void *)msg_pub;
        topic_msg.payload_len = msg_len;

        rc = IOT_MQTT_Publish(pclient, TOPIC_DATA, &topic_msg);
        if (rc < 0) {
            log_info("error occur when publish.\n");
        }
#ifdef MQTT_ID2_CRYPTO
        log_info("packet-id=%u, publish topic msg='0x%02x%02x%02x%02x'...\n",
                (uint32_t)rc,
                msg_pub[0], msg_pub[1], msg_pub[2], msg_pub[3]);
#else
        log_info("packet-id=%u, publish topic msg=%s.\n", (uint32_t)rc, msg_pub);
#endif
    }
    cnt++;
    if(cnt < 2) {
        // aos_post_delayed_action(3000, mqtt_publish, pclient);
        // mqtt_publish(pclient);
    } else {

        IOT_MQTT_Unsubscribe(pclient, TOPIC_DATA);

        rtos_delay_milliseconds(200);

        IOT_MQTT_Destroy(&pclient);

        release_buff();
        // aos_loop_exit();
        is_subscribed = 0;
        cnt = 0;
    }
}



void event_handle_mqtt(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    uintptr_t packet_id = (uintptr_t)msg->msg;
    iotx_mqtt_topic_info_pt topic_info = (iotx_mqtt_topic_info_pt)msg->msg;

    switch (msg->event_type) {
        case IOTX_MQTT_EVENT_UNDEF:
            log_info("undefined event occur.\n");
            break;

        case IOTX_MQTT_EVENT_DISCONNECT:
            log_info("MQTT disconnect.\n");
            break;

        case IOTX_MQTT_EVENT_RECONNECT:
            log_info("MQTT reconnect.\n");
            break;

        case IOTX_MQTT_EVENT_SUBCRIBE_SUCCESS:
            log_info("subscribe success, packet-id=%u.\n", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_SUBCRIBE_TIMEOUT:
            log_info("subscribe wait ack timeout, packet-id=%u.\n", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_SUBCRIBE_NACK:
            log_info("subscribe nack, packet-id=%u.\n", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_UNSUBCRIBE_SUCCESS:
            log_info("unsubscribe success, packet-id=%u.\n", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_UNSUBCRIBE_TIMEOUT:
            log_info("unsubscribe timeout, packet-id=%u.\n", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_UNSUBCRIBE_NACK:
            log_info("unsubscribe nack, packet-id=%u.\n", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_PUBLISH_SUCCESS:
            log_info("publish success, packet-id=%u.\n", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_PUBLISH_TIMEOUT:
            log_info("publish timeout, packet-id=%u.\n", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_PUBLISH_NACK:
            log_info("publish nack, packet-id=%u.\n", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_PUBLISH_RECVEIVED:
            log_info("topic message arrived but without any related handle: topic=%.*s, topic_msg=%.*s.\n",
                          topic_info->topic_len,
                          topic_info->ptopic,
                          topic_info->payload_len,
                          topic_info->payload);
            break;

        default:
            log_info("Should NOT arrive here.\n");
            break;
    }
}

/*
 * initialization parameter: mqtt_params
 */
int mqtt_client_example(void)
{
    int rc = 0;
    iotx_conn_info_pt pconn_info;
    iotx_mqtt_param_t mqtt_params;

    if (msg_buf != NULL) {
        return rc;
    }

    if (NULL == (msg_buf = (char *)LITE_malloc(MSG_LEN_MAX))) {
        log_info("not enough memory.\n");
        rc = -1;
        release_buff();
        return rc;
    }

    if (NULL == (msg_readbuf = (char *)LITE_malloc(MSG_LEN_MAX))) {
        log_info("not enough memory.\n");
        rc = -1;
        release_buff();
        return rc;
    }

    /* Device AUTH */
    if (0 != IOT_SetupConnInfo(PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, (void **)&pconn_info)) {
        log_info("AUTH request failed!\n");
        rc = -1;
        release_buff();
        return rc;
    }

    /* Initialize MQTT parameter */
    memset(&mqtt_params, 0x0, sizeof(mqtt_params));

    mqtt_params.port = pconn_info->port;
    mqtt_params.host = pconn_info->host_name;
    mqtt_params.client_id = pconn_info->client_id;
    mqtt_params.username = pconn_info->username;
    mqtt_params.password = pconn_info->password;
    mqtt_params.pub_key = pconn_info->pub_key;

    mqtt_params.request_timeout_ms = 2000;
    mqtt_params.clean_session = 0;
    mqtt_params.keepalive_interval_ms = 60000;
    mqtt_params.pread_buf = msg_readbuf;
    mqtt_params.read_buf_size = MSG_LEN_MAX;
    mqtt_params.pwrite_buf = msg_buf;
    mqtt_params.write_buf_size = MSG_LEN_MAX;

    mqtt_params.handle_event.h_fp = event_handle_mqtt;
    mqtt_params.handle_event.pcontext = NULL;


    /* Construct a MQTT client with specify parameter */

    gpclient = IOT_MQTT_Construct(&mqtt_params);
    if (NULL == gpclient) {
        log_info("MQTT construct failed.\n");
        rc = -1;
        release_buff();
        // aos_unregister_event_filter(EV_SYS,  mqtt_service_event, gpclient);
    } else{
        // aos_register_event_filter(EV_SYS,  mqtt_service_event, gpclient);
        mqtt_service_event(0, gpclient);
    }
    // BK_ASSERT(rc == 0);

    return rc;
}

// normal case for mqtt subscribe & publish
void test_mqtt_start(void)
{
    int ret = 0;
    ret = mqtt_client_example();
    log_info("test_mqtt_start ret: %d.\n", ret);
    // YUNIT_ASSERT(0 == ret);
    // aos_loop_run();
}