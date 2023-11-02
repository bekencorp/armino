#ifndef __DOORBELL_NETWORK_H__
#define __DOORBELL_NETWORK_H__

#include "lwip/sockets.h"
#include "net.h"

#define IP_QOS_PRIORITY_HIGHEST			(0xD0)
#define IP_QOS_PRIORITY_HIGH			(0xA0)
#define IP_QOS_PRIORITY_LOW				(0x20)
#define IP_QOS_PRIORITY_LOWEST			(0x00)

#define DOORBELL_SEND_MAX_RETRY (2000)
#define DOORBELL_SEND_MAX_DELAY (20)

int doorbell_wifi_sta_connect(char *ssid, char *key);
int doorbell_wifi_soft_ap_start(char *ssid, char *key);

int doorbell_socket_set_qos(int fd, int qos);
int doorbell_socket_sendto(int *fd, const struct sockaddr *dst, uint8_t *data, uint32_t length, int offset);
int doorbell_socket_write(int *fd, uint8_t *data, uint32_t length, int offset);

#endif
