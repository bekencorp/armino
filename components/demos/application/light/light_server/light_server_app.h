#ifndef _LIGHT_SERVER_APP_H_
#define _LIGHT_SERVER_APP_H_

#define TCP_SERVER_LIGHT
#define SERVER_SOCKET_MSG (0x20000000)
#define LIGHT_SOCKET_MSG_QUEUE_LENGTH ( 30 )

void light_sck_cs_txdata_sender(int fd, unsigned char *buf, int len);
int light_sck_cs_rxdat_sender(int fd, char *buf, int len);
extern int demo_start(void);

#endif /*_LIGHT_SERVER_APP_H_*/

//EoF

