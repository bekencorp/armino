#ifndef _DEMO_CONFIG_H_
#define _DEMO_CONFIG_H_

#include <common/sys_config.h>

/* Attention: the macro value must be 0 or 1. thanks*/

#define HELLOWORLD_DEMO                  0

#define OS_THREAD_DEMO                   0
#define OS_SEM_DEMO                      0
#define OS_QUEUE_DEMO                    0
#define OS_TIMER_DEMO                    0
#define OS_MUTEX_DEMO                    0

#define WIFI_SCAN_DEMO                   0
#define WIFI_SOFTAP_DEMO                 0
#define WIFI_DELETE_SOFTAP_DEMO          0
#define WIFI_STATION_DEMO                0
#define WIFI_AIRKISS_STATION_DEMO        0

#define TCP_SERVER_DEMO                  0
#define UART_TCP_SERVER_DEMO             0
#define UDP_SERVER_DEMO                  0
#define UDP_CLIENT_DEMO                  0
#define MQTT_CLIENT_DEMO                 0

#define CJSON_TEST_DEMO                  0
#define LIGHT_SERVER_APPLICATION         0

#define AP_STA_DEMO                      0
#define LIGHT_CLIENT_APP_DEMO            0
#define WIFI_STATION_PS_DEMO             0
#define EASY_FLASH_DEMO                  0



#if ((HELLOWORLD_DEMO\
		+ OS_THREAD_DEMO\
		+ OS_SEM_DEMO\
		+ OS_QUEUE_DEMO\
		+ OS_TIMER_DEMO\
		+ OS_MUTEX_DEMO\
		+ WIFI_SCAN_DEMO\
		+ WIFI_SOFTAP_DEMO\
		+ WIFI_DELETE_SOFTAP_DEMO\
		+ WIFI_STATION_DEMO\
		+ WIFI_AIRKISS_STATION_DEMO\
		+ TCP_SERVER_DEMO\
		+ UART_TCP_SERVER_DEMO\
		+ UDP_SERVER_DEMO\
		+ UDP_CLIENT_DEMO\
		+ MQTT_CLIENT_DEMO\
		+ CJSON_TEST_DEMO\
		+ LIGHT_SERVER_APPLICATION\
		+ AP_STA_DEMO\
		+ EASY_FLASH_DEMO\
		+ LIGHT_CLIENT_APP_DEMO\
		+ WIFI_STATION_PS_DEMO) > 1)

#error Only one demo case can be tested, thanks!!!		
#endif

#if CJSON_TEST_DEMO
#define _CJSON_USE_           1
#endif

#endif /*_DEMO_CONFIG_H_*/
// EOF

