#ifndef _light_Socket_H_
#define _light_Socket_H_

#include "light_server_app.h"

#ifdef LIGHT_SERVER_APPLICATION

#ifdef TCP_SERVER_LIGHT
#include "compiler.h"
#include <common/bk_list.h>

typedef struct
{
    LIST_HEADER_T list;
    int fd;
} light_socket_handler_meg_T;

int light_tcp_fd_find_connect(int fd);
int light_socket_application_start(void);

extern LIST_HEADER_T light_tcp_link_node_list;

#endif /*TCP_SERVER_LIGHT*/

#endif  /*_LIGHT_Application_*/
#endif /*_light_Socket_H_*/

