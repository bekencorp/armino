#ifndef __MQTT_TEST_H__
#define __MQTT_TEST_H__
/**
 * MQTT URI farmat:
 * domain mode
 * tcp://iot.eclipse.org:1883
 * tcp://broker.mqttdashboard.com:1883
 * ipv4 mode
 * tcp://192.168.10.1:1883
 * ssl://192.168.10.1:1884
 *
 * ipv6 mode
 * tcp://[fe80::20c:29ff:fe9a:a07e]:1883
 * ssl://[fe80::20c:29ff:fe9a:a07e]:1884
 */ 
#define MQTT_TEST_SERVER_URI    "tcp://broker.mqttdashboard.com:1883"
#define MQTT_CLIENTID           "beken-mqtt"
#define MQTT_USERNAME           "admin"
#define MQTT_PASSWORD           "admin"
#define MQTT_SUBTOPIC           "/mqtt/test"
#define MQTT_PUBTOPIC           "/mqtt/test"
#define MQTT_WILLMSG            "Goodbye!"
#define MQTT_TEST_QOS           1
#define MQTT_PUB_SUB_BUF_SIZE   1024

#define CMD_INFO                "'mqtt_test <start|stop>'"
#define TEST_DATA_SIZE          256
#define PUB_CYCLE_TM            1000

#define MQTT_ECHO_TEST          0     

#if MQTT_ECHO_TEST
#define ECHO_STACK_SIZE         (2048)
#define ECHO_THD_PRORITY        (5)
#define ECHO_REPOSE_COUNT       (100)
#endif

extern int demo_start(void);

#endif /*__MQTT_TEST_H__*/
// eof

