#ifndef _LIGHTCOMMUNPROTOCOL_H_
#define _LIGHTCOMMUNPROTOCOL_H_

#include "light_server_app.h"
#include "light_client_app_demo.h"

#if defined(LIGHT_SERVER_APPLICATION) || defined(LIGHT_CLIENT_APP_DEMO)

#define LIGHT_COMMUNICATION_PROTOCOL
#endif

#ifdef LIGHT_COMMUNICATION_PROTOCOL

#include "compiler.h"
#include <common/bk_list.h>

#if ( defined(LIGHT_SERVER_APPLICATION) && defined(LIGHT_CLIENT_APP_DEMO) )

#include "Arch.h"

#define MONOPOLIZERESOURCEPROTECT_DECLARATION() GLOBAL_INT_DECLARATION()
#define MONOPOLIZERESOURCEPROTECT_START() GLOBAL_INT_DISABLE()
#define MONOPOLIZERESOURCEPROTECT_STOP() GLOBAL_INT_RESTORE()
#else
#define MONOPOLIZERESOURCEPROTECT_DECLARATION()
#define MONOPOLIZERESOURCEPROTECT_START()
#define MONOPOLIZERESOURCEPROTECT_STOP()
#endif

#define PoK 1
#define Pgeneral 0
#define Perror -1

#define PROTOCOLMSG_LIST_NODE_NUMBER_MAX (50)

typedef struct
{
    int from;
    unsigned int Len;
    char Msg[0];
} prtcl_msg_body_T;

typedef struct
{
    LIST_HEADER_T list;
    prtcl_msg_body_T *MsgBdy;
} ProtocolMessageList_T;

typedef struct
{
    int to;
    unsigned int Len;
    unsigned char Msg[0];
} prtcl_response_msg_T;

typedef struct
{
    LIST_HEADER_T list;
    int from;
    unsigned char InSerialNumber;
    unsigned char OutSerialNumber;
    void *OtherMsg;
} SerialNumberList_T;

unsigned int get_message_bady_node_number(void);
unsigned char protocol_othermsg_register(int from, void *OtherMsg);
unsigned char protocol_othermsg_unregister(int from);
unsigned char protocol_serial_number_register(int from, unsigned char InserNum, unsigned char OutSerNum);
unsigned char protocol_serial_number_unregister(int from);
unsigned char protocol_msg_register(int from, unsigned char InserNum, unsigned char OutSerNum, void *OtherMsg);
unsigned char protocol_msg_unregister(int from);
SerialNumberList_T *get_protocol_serial_number(int from);
void protocol_message_body_handler(void);
int light_cmmun_ptcl_msg_body_input(char *msg, int len, int from);
int light_cmmun_ptcl_msg_input(char *msg, int len, int from);
void light_cmmun_ptcl_msg_output(int to, unsigned char *buf, int len);

#endif /*LIGHT_COMMUNICATION_PROTOCOL*/
#endif /*_LIGHTCOMMUNPROTOCOL_H_*/
