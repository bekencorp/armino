#ifndef _LIGHT_CLIENT_APP_Demo_H_
#define _LIGHT_CLIENT_APP_Demo_H_

enum
{
    t_ok = 0,
    t_error = -1,
};

#define CLIENT_SOCKET_MSG 0x10000000
#define LIGHT_CLIENT_SOCKET_MSG_QUEUE_LENGTH 20

#define LIGHT_REMOTE_SERVER_IP "192.168.5.101"
#define LIGHT_REMOTE_SERVER_PORT 22000

extern int demo_start(void);
extern int light_client_input_msg_sender(int from, char *msg, unsigned int len);
extern int light_client_output_msg_sender(int to, char *msg, unsigned int len);
extern int light_client_application_start( void );
#endif /*_LIGHT_CLIENT_APP_H_*/

