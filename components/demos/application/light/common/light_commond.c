#include "light_commond.h"
#include <common/bk_include.h>

#if LIGHT_COMMOND

#include "string.h"
#include "stdlib.h"
#include <os/mem.h>
#include "uart_pub.h"
#include "gpio_pub.h"

#define PIN 0x0102030405060708

struct light_cmd_msg
{
    int Len;
    char Msg[0];
};

#define LightOnStatus 1
#define LightOffStatus 0

char get_light_hardware_status(void)
{
    unsigned int LightStatus = 0;
    u32 gpio_cfg_addr, val;

    val = (*((volatile UINT32 *)( REG_GPIO_CFG_BASE_ADDR + GPIO4 * 4)));

    if(val & 0x02)
        LightStatus = LightOffStatus;
    else
        LightStatus = LightOnStatus;

    return LightStatus;
}

char set_light_hardware(void)  //Open
{
    GPIO_INDEX gpio_idx;
    u32 param;
    char LightStatus, mode;

    gpio_idx = GPIO4;
    param =	GPIO_CFG_PARAM(gpio_idx, 0);
    sddev_control(GPIO_DEV_NAME, CMD_GPIO_OUTPUT, &param);

    return 1;
}

char reset_light_hardware(void)  //Close
{
    GPIO_INDEX gpio_idx;
    u32 param;
    char LightStatus, mode;

    gpio_idx = GPIO4;
    param = GPIO_CFG_PARAM(gpio_idx, 1);
    sddev_control(GPIO_DEV_NAME, CMD_GPIO_OUTPUT, &param);

    return 1;
}

void *reset_light_communication(void *inmsg, void *inmsg2)
{
    struct light_cmd_msg *Msg;
    prtcl_msg_body_T *MsgBdy = (prtcl_msg_body_T *)inmsg;
    int offset = (int)inmsg2;
    int MsgLen = 2;

    Msg = (struct light_cmd_msg *)os_malloc(sizeof(struct light_cmd_msg) + MsgLen);

    if(Msg != NULL)
    {
        Msg->Len = 2;
        Msg->Msg[0] = 0x00;
        Msg->Msg[1] = 0x00;
    }

    return Msg;
}

void *get_light_status(void *inmsg, void *inmsg2)
{
    struct light_cmd_msg *Msg;
    int MsgLen = 3;

    Msg = (struct light_cmd_msg *)os_malloc(sizeof(struct light_cmd_msg) + MsgLen);
    if(Msg != NULL)
    {
        Msg->Len = MsgLen;
        Msg->Msg[0] = 0x7E;
        Msg->Msg[1] = 0x7E;
        Msg->Msg[2] =  get_light_hardware_status();
    }

    return Msg;
}

void *open_light_commond(void *inmsg, void *inmsg2)
{
    struct light_cmd_msg *Msg;
    prtcl_msg_body_T *MsgBdy = (prtcl_msg_body_T *)inmsg;
    int offset = (int)inmsg2;
    int MsgLen = 3;

    set_light_hardware();
    Msg = (struct light_cmd_msg *)os_malloc(sizeof(struct light_cmd_msg) + MsgLen);
    if(Msg != NULL)
    {
        Msg->Len = MsgLen;
        Msg->Msg[0] = 0xA1;
        Msg->Msg[1] = 0x00;
        Msg->Msg[2] =  get_light_hardware_status();
        //os_printf("OpnLight:%01x\r\n",Msg->Msg[2]);
    }

    return Msg;
}

void *close_light_commond(void *inmsg, void *inmsg2)
{
    struct light_cmd_msg *Msg;
    prtcl_msg_body_T *MsgBdy = (prtcl_msg_body_T *)inmsg;
    int offset = (int)inmsg2;
    int MsgLen = 3;

    reset_light_hardware();
    Msg = (struct light_cmd_msg *)os_malloc(sizeof(struct light_cmd_msg) + MsgLen);
    if(Msg != NULL)
    {
        Msg->Len = MsgLen;
        Msg->Msg[0] = 0xB1;
        Msg->Msg[1] = 0x00;
        Msg->Msg[2] =  get_light_hardware_status();
        //os_printf("ClsLight:%01x\r\n",Msg->Msg[2]);
    }

    return Msg;
}

////////////////////////////////////////////////////////////////
const light_commond_type_T LightCmdTabel[] =
{
    {0x0000, reset_light_communication},
    {0x00A0, open_light_commond},
    {0x00B0, close_light_commond},
    {0x7E7E, get_light_status}
};

int change_protocol_serial_number(prtcl_msg_body_T *OutMsgBdy)
{
    SerialNumberList_T *SNL = NULL;
    int SerNumOffset = 0;

    SNL = get_protocol_serial_number(OutMsgBdy->from);
    if(!SNL) return -1;


    if(OutMsgBdy->Len <= 250)
    {
        SerNumOffset = 0 + 1;
    }
    else if(OutMsgBdy->Len <= 0xffff)
    {
        SerNumOffset = 2 + 1;
    }
    else if(OutMsgBdy->Len <= 0xffffff)
    {
        SerNumOffset = 3 + 1;
    }
    else
    {
        SerNumOffset = -1;
    }

    if(SerNumOffset)
    {
        SNL->OutSerialNumber++;
        OutMsgBdy->Msg[SerNumOffset] = SNL->OutSerialNumber;
        return 1;
    }
    return -1;
}

prtcl_response_msg_T *structure_response_message(prtcl_msg_body_T *MsgBdy, struct light_cmd_msg *inMsg)
{
    prtcl_response_msg_T *OutMsgBdy = NULL;
    int offset = 0, outLen;
    SerialNumberList_T *SNL;

    if(inMsg->Len >= 0)
    {
        if(inMsg->Len & 0xFFFF00)
        {
            outLen = 4;
        }
        else if(inMsg->Len & 0x00FF00)
        {
            outLen = 3;
        }
        else
        {
            outLen = 1;
        }
        outLen += 1;
        outLen += inMsg->Len;

        OutMsgBdy = (prtcl_response_msg_T *)os_malloc(sizeof(prtcl_response_msg_T) + outLen);
        if(OutMsgBdy != NULL)
        {
            OutMsgBdy->Len = outLen;

            if(OutMsgBdy->Len & 0xFFFF00)
            {
                OutMsgBdy->Msg[0] = 253;
                OutMsgBdy->Msg[1] =  inMsg->Len & 0xFF;
                OutMsgBdy->Msg[2] =  (inMsg->Len >> 8) & 0xFF;
                OutMsgBdy->Msg[3] =  (inMsg->Len >> 16) & 0xFF;
                offset = 4;
            }
            else if(OutMsgBdy->Len & 0x00FF00)
            {
                OutMsgBdy->Msg[0] = 252;
                OutMsgBdy->Msg[1] =  inMsg->Len & 0xFF;
                OutMsgBdy->Msg[2] =  (inMsg->Len >> 8) & 0xFF;
                offset = 3;
            }
            else
            {
                OutMsgBdy->Msg[0] =  inMsg->Len & 0xFF;
                offset = 1;
            }

            OutMsgBdy->to = MsgBdy->from;

            SNL = get_protocol_serial_number(OutMsgBdy->to);
            SNL->OutSerialNumber++;
            OutMsgBdy->Msg[offset] = SNL->OutSerialNumber;
            offset += 1;

            memcpy(&OutMsgBdy->Msg[offset], inMsg->Msg, OutMsgBdy->Len);
        }
    }

    return OutMsgBdy;
}

prtcl_response_msg_T *analysis_light_commond(prtcl_msg_body_T *MsgBdy)
{
    prtcl_response_msg_T *OutMsg = NULL;
    struct light_cmd_msg *return_msg = NULL;
    light_commond_type_T *cmd_type;
    int CmdNum = sizeof(LightCmdTabel) / sizeof(light_commond_type_T);
    unsigned int i, offset = 0;
    unsigned int Cmd = 0;

    if(MsgBdy->Len < 2)
    {
        return NULL;
    }

    if((MsgBdy->Len <= 250) && (MsgBdy->Len >= 0)) offset = 1 + 1;
    else if((MsgBdy->Len <= 0xFFFF) && (MsgBdy->Len >= 0))offset = 3 + 1;
    else if((MsgBdy->Len <= 0xFFFFFF) && (MsgBdy->Len >= 0))offset = 4 + 1;
    else return NULL;

    Cmd = (unsigned char)MsgBdy->Msg[offset];
    Cmd += (((unsigned char)(MsgBdy->Msg[offset + 1])) << 8);
    offset += 2;

    for(i = 0; i < CmdNum; i++)
    {
        cmd_type = (light_commond_type_T *)&LightCmdTabel[i];
        if(cmd_type->Cmd == Cmd)
        {
            if(cmd_type->pFun)
                return_msg = (struct light_cmd_msg *)cmd_type->pFun((void *)MsgBdy, (void *)offset);
            break;
        }
    }

    if(return_msg != NULL)
    {
        OutMsg = structure_response_message(MsgBdy, return_msg);
        os_free(return_msg);
    }

    return OutMsg;
}

prtcl_response_msg_T *analysis_request_message(prtcl_msg_body_T *MsgBdy)
{
    prtcl_msg_body_T *Input_msg_bdy = NULL;
    prtcl_response_msg_T *OutMsgBdy = NULL;

    Input_msg_bdy = MsgBdy;

    OutMsgBdy =  analysis_light_commond(Input_msg_bdy);

    return OutMsgBdy;
}


#endif /*LIGHT_COMMOND*/

