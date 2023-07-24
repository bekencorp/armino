#ifndef __DOORBELL_WIFI_H__
#define __DOORBELL_WIFI_H__

int doorbell_wifi_sta_connect(char *ssid, char *key);
int doorbell_wifi_soft_ap_start(char *ssid, char *key);

#endif
