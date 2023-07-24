#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <components/log.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>

#include <lwip/sockets.h>

#include "PPCS_API.h"

#include "osi_cs2_not_impl.h"

INT32 PPCS_Share_Bandwidth(CHAR bOnOff)
{
//    os_printf("\n!!!! %s not impl, please contact CS2 !!!!\n", __func__);
    return ERROR_PPCS_SUCCESSFUL;
}

INT32 PPCS_PktSend(INT32 SessionHandle, UCHAR Channel, CHAR *PktBuf, INT32 PktSize)
{
//    os_printf("\n!!!! %s not impl, please contact CS2 !!!!\n", __func__);
    return ERROR_PPCS_SUCCESSFUL;
}

INT32 PPCS_PktRecv(INT32 SessionHandle, UCHAR Channel, CHAR *PktBuf, INT32 *PktSize, UINT32 TimeOut_ms)
{
//    os_printf("\n!!!! %s not impl, please contact CS2 !!!!\n", __func__);
    return ERROR_PPCS_SUCCESSFUL;
}

#if CONFIG_CS2_P2P_SERVER

INT32 PPCS_Connect_fake(const CHAR *TargetID, CHAR bEnableLanSearch, UINT16 UDP_Port)
{
//    os_printf("\n!!!! %s not impl, please contact CS2 !!!!\n", __func__);
    return ERROR_PPCS_NOT_INITIALIZED;
}

INT32 PPCS_ConnectByServer_fake(const CHAR *TargetID, CHAR bEnableLanSearch, UINT16 UDP_Port, CHAR *ServerString)
{
//    os_printf("\n!!!! %s not impl, please contact CS2 !!!!\n", __func__);
    return ERROR_PPCS_NOT_INITIALIZED;
}

INT32 PPCS_Connect_Break_fake()
{
//    os_printf("\n!!!! %s not impl, please contact CS2 !!!!\n", __func__);
    return 0;
}

#endif

#if CONFIG_CS2_P2P_CLIENT

INT32 PPCS_Listen_fake(const CHAR *MyID, const UINT32 TimeOut_Sec, UINT16 UDP_Port, CHAR bEnableInternet, const CHAR *APILicense)
{
//    os_printf("\n!!!! %s not impl, please contact CS2 !!!!\n", __func__);
    return 0;
}

INT32 PPCS_Listen_Break_fake(void)
{
//    os_printf("\n!!!! %s not impl, please contact CS2 !!!!\n", __func__);
    return 0;
}

#endif
