#include "light_commun_protocol.h"
#include <common/bk_include.h>

#if LIGHT_COMMUNICATION_PROTOCOL
#include <os/mem.h>
#include "uart_pub.h"
#include "light_commond.h"
#include "string.h"

#define FRAME_HEAD_HEX 0xFF
#define PROTOCOL_VERSION_MASK 0x01
#define FRAME_END_HEX 0x00
#define LightShowLog(a)

LIST_HEADER_T PMsgLst = {&PMsgLst, &PMsgLst};
LIST_HEADER_T ser_num_list = {&ser_num_list, &ser_num_list};

int find_frame_head(char *msg, int len, int *offset)
{
    int i;
    for(i = 0; i < len; i++)
    {
        if(msg[i] == ((char)FRAME_HEAD_HEX))
        {
            *offset = i;
            return PoK;
        }
    }
    return Perror;
}

int get_message_body_length(char *msg, int Len)
{
    int MsgBodyLength = 0;
    int offset = 0;

    if((Len < (4 + 1 + 1 + 1)) && (!msg)) return -1;
    MsgBodyLength = (unsigned char)msg[4];
    if(MsgBodyLength <= 250)
    {
        //Length :1Byte expression
        offset += (4 - 1);
        offset += 1 + 1;
        offset += MsgBodyLength;
        if((offset + 1) <= Len)
        {
            if(msg[offset + 1] != 0x00)
                MsgBodyLength = -1;
        }
        else
            MsgBodyLength = -1;
    }
    else if(MsgBodyLength == 251)
    {
        MsgBodyLength = 0;//None message body
        offset += (4 - 1);
        offset += 1 + 1; //
        offset += MsgBodyLength;
        if((offset + 1) <= Len)
        {
            if(msg[offset + 1] != 0x00)
                MsgBodyLength = -1;
        }
        else
            MsgBodyLength = -1;
    }
    else if(MsgBodyLength == 252)
    {
        //Length :2Byte expression
        if(Len > 7)
        {
            MsgBodyLength = msg[5] + (msg[6] << 8);
            offset += (4 - 1);
            offset += 3 + 1; //
            offset += MsgBodyLength;
            if((offset + 1) <= Len)
            {
                if(msg[offset + 1] != 0x00)
                    MsgBodyLength = -1;
            }
            else
                MsgBodyLength = -1;
        }
        else
            MsgBodyLength = -1;
    }
    else if(MsgBodyLength == 253)
    {
        //Length :3Byte expression
        if(Len > 8)
        {
            MsgBodyLength = msg[5] + (msg[6] << 8) + (msg[7] << 16);
            offset += (4 - 1);
            offset += 4 + 1; //
            offset += MsgBodyLength;
            if((offset + 1) <= Len)
            {
                if(msg[offset + 1] != 0x00)
                    MsgBodyLength = -1;
            }
            else
                MsgBodyLength = -1;
        }
        else
            MsgBodyLength = -1;
    }
    else
    {
        MsgBodyLength = -1;
    }

    return MsgBodyLength;
}

char full_frame_checksum(char *msg, int len)
{
    char sum = 0;
    int i;

    for(i = 0; i < len; i++)
    {
        sum += msg[i];
    }

    return sum;
}

char full_frame_check_xor(char *msg, int len)
{
    char XOR = 0;
    int i;

    for(i = 0; i < len; i++)
    {
        XOR ^= msg[i];
    }

    return XOR;
}

int regiseter_message_body_list(prtcl_msg_body_T *MsgBody)
{
    int status = Perror;
    ProtocolMessageList_T *PrclMsgLst;
    unsigned int list_node_num;
    MONOPOLIZERESOURCEPROTECT_DECLARATION();

    PrclMsgLst = (ProtocolMessageList_T *) os_malloc(sizeof(ProtocolMessageList_T));
    if(PrclMsgLst)
    {
        PrclMsgLst->MsgBdy = MsgBody;

        MONOPOLIZERESOURCEPROTECT_START();

        list_node_num = list_size(&PMsgLst);
        if(list_node_num <= PROTOCOLMSG_LIST_NODE_NUMBER_MAX)
        {
            list_add_tail(&PrclMsgLst->list, &PMsgLst);
            MONOPOLIZERESOURCEPROTECT_STOP();
            status = PoK;
        }
        else
        {
            MONOPOLIZERESOURCEPROTECT_STOP();
            os_free(PrclMsgLst);
            status = Perror;
            os_printf( "> max_node_num\r\n");
        }
    }
    else
    {
        os_printf( "list struct malloc failed\r\n");
        status = Perror;
    }

    return status;
}

unsigned int get_message_bady_node_number(void)
{
    unsigned int list_node_num;

    MONOPOLIZERESOURCEPROTECT_DECLARATION();

    MONOPOLIZERESOURCEPROTECT_START();

    list_node_num = list_size(&PMsgLst);

    MONOPOLIZERESOURCEPROTECT_STOP();

    return list_node_num;
}

unsigned char protocol_othermsg_register(int from, void *OtherMsg)
{
    SerialNumberList_T *SNL;
    LIST_HEADER_T *pos, *n;

    MONOPOLIZERESOURCEPROTECT_DECLARATION();

    list_for_each_safe(pos, n, &ser_num_list)
    {
        SNL = list_entry(pos, SerialNumberList_T, list);
        if(SNL->from == from)
        {
            SNL->OtherMsg = OtherMsg;
            return PoK;
        }
    }

    SNL = (SerialNumberList_T *) os_malloc(sizeof(SerialNumberList_T));
    if(SNL)
    {
        memset(SNL, 0, sizeof(SerialNumberList_T));
        SNL->from = from;
        SNL->OtherMsg = OtherMsg;

        MONOPOLIZERESOURCEPROTECT_START();

        list_add_head(&SNL->list, &ser_num_list);

        MONOPOLIZERESOURCEPROTECT_STOP();
    }
    else
    {
        os_printf( "protocol_othermsg_register failed");
        return Perror;
    }

    return PoK;
}

unsigned char protocol_othermsg_unregister(int from)
{
    SerialNumberList_T *SNL;
    LIST_HEADER_T *pos, *n;

    list_for_each_safe(pos, n, &ser_num_list)
    {
        SNL = list_entry(pos, SerialNumberList_T, list);
        if(SNL->from == from)
        {
            SNL->OtherMsg = NULL;
            return PoK;
        }
    }

    return Perror;
}

unsigned char protocol_serial_number_register(int from, unsigned char InserNum, unsigned char OutSerNum)
{
    SerialNumberList_T *SNL;
    LIST_HEADER_T *pos, *n;

    MONOPOLIZERESOURCEPROTECT_DECLARATION();

    list_for_each_safe(pos, n, &ser_num_list)
    {
        SNL = list_entry(pos, SerialNumberList_T, list);
        if(SNL->from == from)
        {
            SNL->InSerialNumber = InserNum;
            SNL->OutSerialNumber = OutSerNum;
            return PoK;
        }
    }

    SNL = (SerialNumberList_T *) os_malloc(sizeof(SerialNumberList_T));
    if(SNL)
    {
        SNL->from = from;
        SNL->InSerialNumber = InserNum;
        SNL->OutSerialNumber = OutSerNum;
        SNL->OtherMsg = NULL;

        MONOPOLIZERESOURCEPROTECT_START();

        list_add_head(&SNL->list, &ser_num_list);

        MONOPOLIZERESOURCEPROTECT_STOP();
    }
    else
    {
        os_printf( "protocol_serial_number_register failed");
        return Perror;
    }


    return PoK;
}

unsigned char protocol_serial_number_unregister(int from)
{
    SerialNumberList_T *SNL;
    LIST_HEADER_T *pos, *n;

    list_for_each_safe(pos, n, &ser_num_list)
    {
        SNL = list_entry(pos, SerialNumberList_T, list);
        if(SNL->from == from)
        {
            SNL->InSerialNumber = 0xFF;
            SNL->OutSerialNumber = 0xFF;
            return PoK;
        }
    }

    return Perror;
}

unsigned char protocol_msg_register(int from, unsigned char InserNum, unsigned char OutSerNum, void *OtherMsg)
{
    SerialNumberList_T *SNL;

    MONOPOLIZERESOURCEPROTECT_DECLARATION();

    SNL = (SerialNumberList_T *) os_malloc(sizeof(SerialNumberList_T));
    if(SNL)
    {
        SNL->from = from;
        SNL->InSerialNumber = InserNum;
        SNL->OutSerialNumber = OutSerNum;
        SNL->OtherMsg = OtherMsg;

        MONOPOLIZERESOURCEPROTECT_START();

        list_add_head(&SNL->list, &ser_num_list);

        MONOPOLIZERESOURCEPROTECT_STOP();
    }
    else
    {
        os_printf( "protocol_serial_number_register failed");
        return Perror;
    }

    return PoK;
}

unsigned char protocol_msg_unregister(int from)
{
    SerialNumberList_T *SNL;
    LIST_HEADER_T *pos, *n;

    MONOPOLIZERESOURCEPROTECT_DECLARATION();

    MONOPOLIZERESOURCEPROTECT_START();

    list_for_each_safe(pos, n, &ser_num_list)
    {
        SNL = list_entry(pos, SerialNumberList_T, list);
        if(SNL->from == from)
        {
            list_del(pos);
            os_free(SNL);
            MONOPOLIZERESOURCEPROTECT_STOP();
            return PoK;
        }
    }

    MONOPOLIZERESOURCEPROTECT_STOP();

    return Pgeneral;
}

SerialNumberList_T *get_protocol_serial_number(int from)
{
    unsigned char SerialNumber;
    SerialNumberList_T *SNL = NULL;
    LIST_HEADER_T *pos, *n;

    list_for_each_safe(pos, n, &ser_num_list)
    {
        SNL = list_entry(pos, SerialNumberList_T, list);
        if(SNL->from == from)
        {
            return SNL;
        }
    }

    return NULL;
}

int verify_message_bode_serial_number(prtcl_msg_body_T *MsgBdy)
{
    unsigned char MsgSerialNumber ;
    unsigned char SerialNumber;
    SerialNumberList_T *SNL;

    if(MsgBdy->Len <= 250)
    {
        MsgSerialNumber = (unsigned char) MsgBdy->Msg[1];
    }
    else if(MsgBdy->Len <= 0xFFFF)
    {
        MsgSerialNumber = (unsigned char)MsgBdy->Msg[3];
    }
    else if(MsgBdy->Len <= 0xFFFFFF)
    {
        MsgSerialNumber = (unsigned char)MsgBdy->Msg[4];
    }
    else
    {
        return Perror;
    }

    SNL = get_protocol_serial_number(MsgBdy->from);
    if(!SNL) return Perror;

    SerialNumber = SNL->InSerialNumber;
    if((SerialNumber <= MsgSerialNumber) || ((SerialNumber >= 230) && (MsgSerialNumber < 0xFF)))
    {
        SNL->InSerialNumber = MsgSerialNumber;
        return PoK;
    }

    return Perror;
}

int add_frame_head(prtcl_response_msg_T *MsgBdy, char **out, int *OutLen)
{
    int Len;
    char *Head;

    if((!out) || (!OutLen))
        return -1;

    Len = 4;

    Head = os_malloc(Len);
    if(Head)
    {
        Head[0] = FRAME_HEAD_HEX;
        Head[1] = PROTOCOL_VERSION_MASK;
        Head[2] = full_frame_checksum(MsgBdy->Msg, MsgBdy->Len);
        Head[3] = full_frame_check_xor(MsgBdy->Msg, MsgBdy->Len);;
        *out = Head;
        *OutLen = Len;
        return PoK;
    }

    *OutLen = -1;
    return Perror;
}

void response_message_sender(prtcl_response_msg_T *MsgBdy)
{
    char *out = NULL, *head = NULL;
    int headLen, status, outLength;
    int offset = 0;

    status = add_frame_head(MsgBdy, &head, &headLen);
    if(status == PoK)
    {
        outLength = headLen + MsgBdy->Len + 1;

        out = os_malloc(outLength);
        if(out)
        {
            memcpy(&out[offset], head, headLen);
            offset += headLen;

            memcpy(&out[offset], MsgBdy->Msg, MsgBdy->Len);
            offset += MsgBdy->Len;

            out[offset] = FRAME_END_HEX;

            light_cmmun_ptcl_msg_output(MsgBdy->to, out, outLength);
        }
    }

    if(head) os_free(head);
    if(out) os_free(out);
}

void analysis_message_body(prtcl_msg_body_T *MsgBdy)
{
    prtcl_response_msg_T *OutMsg = NULL;

    OutMsg = analysis_request_message(MsgBdy);
    if(OutMsg != NULL)
    {
        response_message_sender(OutMsg);
        os_free(OutMsg);
    }
}

void protocol_message_body_handler(void)
{
    ProtocolMessageList_T *PrclMsgLst;
    LIST_HEADER_T *pos, *n;
    prtcl_msg_body_T *MsgBdy;
    static int monoplize_protect = 0;

    MONOPOLIZERESOURCEPROTECT_DECLARATION();

    MONOPOLIZERESOURCEPROTECT_START();
    if(monoplize_protect)
    {
        MONOPOLIZERESOURCEPROTECT_STOP();
        return;
    }
    monoplize_protect = 1;

    list_for_each_safe(pos, n, &PMsgLst)
    {
        list_del(pos);
        MONOPOLIZERESOURCEPROTECT_STOP();

        PrclMsgLst = list_entry(pos, ProtocolMessageList_T, list);

        if(PrclMsgLst->MsgBdy)
        {
            MsgBdy = PrclMsgLst->MsgBdy;
            PrclMsgLst->MsgBdy = NULL;

            analysis_message_body(MsgBdy);
            os_free(MsgBdy);
        }

        os_free(PrclMsgLst);
        MONOPOLIZERESOURCEPROTECT_START();
    }

    monoplize_protect = 0;
    MONOPOLIZERESOURCEPROTECT_STOP();
}

int message_body_sender(char *mbMsg, int Len, int from)
{
    prtcl_msg_body_T *MsgBdy = NULL;
    int status = Perror;

    MsgBdy = os_malloc(sizeof(prtcl_msg_body_T) + Len);
    if(MsgBdy != NULL)
    {
        MsgBdy->Len = Len;
        MsgBdy->from = from;
        memcpy(MsgBdy->Msg, mbMsg, Len);

        status = verify_message_bode_serial_number(MsgBdy);
        /*
        if(status != PoK)
        {
            os_free(MsgBdy);
            LightShowLog("serial number Error!");
            return Perror;
        }
        */
        status = regiseter_message_body_list(MsgBdy);
        if(status == Perror)
        {
            os_free(MsgBdy);
            LightShowLog("Insert List Error!");
        }
    }

    return status;
}

int verify_frame_message(char *msg, int len, int from, int *use_len)
{
    int msgLength = -1, status = -1;
    char CheckSum, CheckXOR;
    char MsgCheckSum, MsgCheckXOR;
    int msgOffset = -1, msgBdyLen = -1;

    msgLength = get_message_body_length(msg, len);
    if(msgLength >= 0)
    {
        MsgCheckXOR = msg[3];
        MsgCheckSum = msg[2];

        msgOffset = 4;
        if(msgLength <= 250)
        {
            msgBdyLen = msgLength + 1 + 1;
        }
        else if(msgLength < 0xFFFF)
        {
            msgBdyLen = msgLength + 2 + 1;
        }
        else if(msgLength < 0xFFFFFF)
        {
            msgBdyLen = msgLength + 3 + 1;
        }

        if(FRAME_END_HEX != msg[msgOffset + msgBdyLen]) return Perror;

        CheckSum = full_frame_checksum(&msg[msgOffset], msgBdyLen);
        //if(MsgCheckSum != CheckSum) return Perror;

        CheckXOR = full_frame_check_xor(&msg[msgOffset], msgBdyLen);
        //if(MsgCheckXOR != CheckXOR) return Perror;

        status = message_body_sender(&msg[msgOffset], msgBdyLen, from);

        msgOffset += msgBdyLen;

        *use_len = msgOffset + 1;
    }
    else
    {
        *use_len = 0;
    }
    return status;
}

int analysis_frame_head(char *msg, int len, int from)
{
    int offset = 0, Offset;
    int status = Perror, sta;
    int use_len;

Rep:
    sta = find_frame_head(&msg[offset], (len - offset), &Offset);
    if(sta == PoK)
    {
        offset += Offset;
        sta = verify_frame_message(&msg[offset], (len - offset), from, &use_len);

        if(PoK == sta)
        {
            status = PoK;
            offset += use_len;
        }
        else
        {
            offset += 1;
        }

        if(offset < len)
        {
            goto Rep;
        }
    }

    return status;
}

int light_cmmun_ptcl_msg_body_input(char *msg, int len, int from)
{
    int status = Perror;
    if((!len) && (!msg))
    {
        return Perror;
    }

    status = message_body_sender(msg, len, from);

    return status;
}

int light_cmmun_ptcl_msg_input(char *msg, int len, int from)
{
    int status = Perror;

    if((!len) || (!msg))
    {
        return Perror;
    }

    status = analysis_frame_head(msg, len, from);

    return status;
}

void light_cmmun_ptcl_msg_output(int to, unsigned char *buf, int len)
{
    switch(to & 0xFF000000)
    {
#ifdef LIGHT_SERVER_APPLICATION
    case SERVER_SOCKET_MSG:
        light_sck_cs_txdata_sender(to, buf, len);
        break;
#endif

#ifdef LIGHT_CLIENT_APP_DEMO
    case CLIENT_SOCKET_MSG:
        light_client_output_msg_sender(to, buf, len);
        break;
#endif
    }
}

#endif /*LIGHT_COMMUNICATION_PROTOCOL*/

