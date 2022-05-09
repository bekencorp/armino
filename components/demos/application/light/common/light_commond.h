#ifndef _LIGHTCOMMOND_H_
#define _LIGHTCOMMOND_H_

#include "light_commun_protocol.h"

#ifdef LIGHT_COMMUNICATION_PROTOCOL

#define LIGHT_COMMOND
#endif

#ifdef LIGHT_COMMOND
typedef void *(*LightCommondFunction)(void *MSG, void *MSG2);

typedef struct
{
    unsigned int Cmd;
    LightCommondFunction pFun;
} light_commond_type_T;

prtcl_response_msg_T *analysis_request_message(prtcl_msg_body_T *MsgBdy);

#endif /*LIGHT_COMMOND*/
#endif /*_LIGHTCOMMOND_H_*/

