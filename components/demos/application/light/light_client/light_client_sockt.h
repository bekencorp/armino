#ifndef _LIGHT_CLIENT_SOCKET_H_
#define _LIGHT_CLIENT_SOCKET_H_

#include "light_client_app_demo.h"

#ifdef LIGHT_CLIENT_APP_DEMO

int tcp_client_send_remote_server_data(int skt_fd, char *buf, int len);
int light_net_connect_start(void);

#endif /*LIGHT_CLIENT_APP_DEMO*/
#endif /*_LIGHT_CLIENT_SOCKET_H_*/

