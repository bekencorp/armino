#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>


#include "cs2_p2p_main.h"

#include <lwip/sockets.h>


#include "PPCS_API.h"

#define     TESTER_VERSION  "0.0.6.9"
#define     PKT_TEST
#define     USE_ARALIVE     1


#include "osi_system.h"
#include "osi_cs2_not_impl.h"
#define LINUX 1


typedef uintptr_t UINTp;
// #define DEBUG

#define CH_CMD              0
#define CH_DATA             1
#define SERVER_NUM          3   // WakeUp Server Number
#define SIZE_DID            64  // Device ID Size
#define SIZE_APILICENSE     24  // APILicense Size
#define SIZE_INITSTRING     256 // InitString Size
#define SIZE_WAKEUP_KEY     17  // WakeUp Key Size
//// liteOS TCP packet Send to Device:
////#define WAKEUP_CODE {0x98,0x3b,0x16,0xf8,0xf3,0x9c}


// define for Read/Write test mode
#define BASE_SIZE               1004    // (251 * 4)
#define TEST_SIZE_ONE_WRITE     (128*BASE_SIZE)    // Size of data written at one PPCS_Write.
#define TEST_SIZE_ONE_WRITE_MODIFY     (5 * BASE_SIZE) //for armino
// #define TOTAL_WRITE_SIZE        (8*1024*TEST_SIZE_ONE_WRITE)
#define TEST_SIZE_1             (1*8*TEST_SIZE_ONE_WRITE)      // ~= 0.98MB
#define TEST_SIZE_10            (11*8*TEST_SIZE_ONE_WRITE)     // ~= 10.7MB
#define TEST_SIZE_100           (110*8*TEST_SIZE_ONE_WRITE)    // ~= 107MB
#define TEST_SIZE_1000          (1024*8*TEST_SIZE_ONE_WRITE)   // ~= 1004MB

#define CHECK_WRITE_THRESHOLD_SIZE  1024*1024
#define NUMBER_OF_P2PCHANNEL        2
#define CHECK_BUFFER_FLAGS          0       // check buffer thread will be create, check buffer every 1 sec.
#define CHECK_FLAGS                 1       // check session mode every 100 ms, if connections mode is changed, it will print.
#define RECV_TMP_BUFF_SIZE   (1024 * 3)
#define READ_COUNT_PER (256)
#define NORMAL_MSLEEP 10


static char gLoginCheck_GetAPIInformation = 1;
// LoginStatus_Check Thread exit flags: 0->keep running,1->exit
static char gThread_LoginCheck_Exit = 0;
// LoginStatus_Check Thread Running flags: thread exit->0, thread running->1
static char gThread_LoginCheck_Running = 0;

//static char gThread_Check_Exit = 0;
//static char gThread_CheckBuffer_Exit = 0;
//static char gThread_CheckBuffer_Running = 0;

static char gFlags_WakeupTest = 0;

static char gThread_UDP_ECHO_Exit = 0;
static int gUDP_ECHO_Socket = -1;

// #define DC_RESPONSE_RESEND_INTERVAL_MS 2000   //// 2000 ms
static char g_EchoResponse = 1;
static int g_ResponseRetryTimes = 3;
static int g_ResponseRetryInterval_sec = 2;
static char gThread_DC_Response_Running = 0;
static char gThread_DirectCommand_Server_Exit = 0;
static int g_DC_Socket = -1;

static volatile int s_current_sessionid = -1;
static volatile uint8_t s_is_cs2_p2p_task_run = 0;
static uint8_t s_cs2_p2p_main_loop_control;
static st_PPCS_NetInfo s_cs2_p2p_networkinfo;

static beken_thread_t s_cs2_p2p_main_tid;
static uint8_t s_log_level = CS2_P2P_LOG_LEVEL_ERROR;

typedef struct
{
    struct sockaddr_in remote_addr;
    unsigned long CmdIndex;
    char CmdTRID[36];
    char Command[962];
    int CmdSize;
    char GotAck;
    int ResponseCounter;
    unsigned long long TimeStamp_ms;
} st_DirectCommand;

struct DirectCommand_List
{
    st_DirectCommand *cmd;
    struct DirectCommand_List *next;
};
static struct DirectCommand_List *g_DC_SL_head = NULL;
static struct DirectCommand_List *g_DC_SL_tail = NULL;
static int g_DirectCommand_SL_TotalCount = 0;
static my_Thread_mutex_t g_DC_LOCK;// = PTHREAD_MUTEX_INITIALIZER;

typedef struct
{
    int SessionID;
    int Channel;
    unsigned long TotalTestSize;
} st_Session_CH;

typedef struct
{
    unsigned long TotalSize_Read;
    unsigned long TotalSize_Write;
    unsigned int Tick_Used_Read;
    unsigned int Tick_Used_Write;
} st_RW_Test_Info;


typedef struct
{
    int SessionID;
    int Channel;
    char Num_CH;
} st_ChkBuf_Info;

typedef struct
{
    int  Skt;                       // Sockfd
    // struct sockaddr_in RemoteAddr;  // Remote IP:Port
    // struct sockaddr_in MyLocalAddr; // My Local IP:Port
    // struct sockaddr_in MyWanAddr;   // My Wan IP:Port
    char RemoteIP[16];
    int RemotePort;
    char MyLocalIP[16];
    int MyLocalPort;
    char MyWanIP[16];
    int MyWanPort;
    unsigned int ConnectTime;       // Connection build in ? Sec Before
    char DID[24];                   // Device ID
    char bCorD;   // I am Client or Device, 0: Client, 1: Device
    char bMyMode; // my define mode by PPCS_Check bMode(0:P2P(Including: LAN TCP/UDP), 1:RLY, 2:TCP); Mydefine: 0:LAN, 1:P2P, 2:RLY, 3:TCP, 4:RP2P.
    char Mode[12];   // Connection Mode: LAN/P2P/RLY/TCP.
    char Reserved[2];
} st_MySess_Info;
static st_MySess_Info g_MySInfo;

#define SIZE_DATE      32   //// "[YYYY-MM-DD hh:mm:ss.xxx]"
typedef struct
{
    int Year;
    int Mon;
    int Day;
    int Week;
    int Hour;
    int Min;
    int Sec;
    int mSec;
    unsigned long Tick_Sec;
    unsigned long long Tick_mSec;
    char Date[SIZE_DATE];
} st_Time_Info;
#define TIME_USE                (int)((tEnd.Tick_mSec) - (tBegin.Tick_mSec))
#define TU_MS(tBegin, tEnd)     (int)((tEnd.Tick_mSec) - (tBegin.Tick_mSec))

static void my_GetCurrentTime(st_Time_Info *pt)
{

    struct timeval tmv;
    int ret = gettimeofday(&tmv, NULL);

    if (0 != ret)
    {
        printf("gettimeofday failed!! errno=%d\n", errno);
        memset(pt, 0, sizeof(st_Time_Info));
        return ;
    }

    //struct tm *ptm = localtime((const time_t *)&tmv.tv_sec);
    struct tm stm = {0};
    struct tm *ptm = localtime_r((const time_t *)&tmv.tv_sec, &stm);

    if (!ptm)
    {
        printf("localtime_r failed!!\n");
        memset(pt, 0, sizeof(st_Time_Info));
        pt->Tick_mSec = ((unsigned long long)tmv.tv_sec) * 1000 + tmv.tv_usec / 1000; // ->ms
    }
    else
    {
        pt->Year = stm.tm_year + 1900;
        pt->Mon = stm.tm_mon + 1;
        pt->Day = stm.tm_mday;
        pt->Week = stm.tm_wday;
        pt->Hour = stm.tm_hour;
        pt->Min = stm.tm_min;
        pt->Sec = stm.tm_sec;
        pt->mSec = (int)(tmv.tv_usec / 1000);
        pt->Tick_Sec = tmv.tv_sec; // 1970年1月1日0点至今的秒数。
        pt->Tick_mSec = ((unsigned long long)tmv.tv_sec) * 1000 + tmv.tv_usec / 1000; // ->ms
        memset(pt->Date, 0, sizeof(pt->Date));
        snprintf(pt->Date, sizeof(pt->Date) - 1, "%04d-%02d-%02d %02d:%02d:%02d.%03d", pt->Year, pt->Mon, pt->Day, pt->Hour, pt->Min, pt->Sec, pt->mSec);
    }
}

static char *toDate(time_t tv_sec, char *outputDate, unsigned int MaxSize)
{
    if (NULL == outputDate)
    {
        return NULL;
    }

    memset(outputDate, 0, MaxSize);

    struct tm stm = {0};

    struct tm *ptm = localtime_r((const time_t *)&tv_sec, &stm);

    if (ptm)

    {
        snprintf(outputDate, MaxSize, "%04d-%02d-%02d %02d:%02d:%02d", stm.tm_year + 1900, stm.tm_mon + 1, stm.tm_mday,
                 stm.tm_hour, stm.tm_min, stm.tm_sec);
    }
    else { snprintf(outputDate, MaxSize, "localtime_r failed"); }

    return outputDate;
}

// GetTime_ms: will call gettimeofday to get current time,format to [YYYY-MM-DD hh:mm:ss.xxx]
static void GetTime_ms(st_Time_Info *t, char *tBuf, unsigned int tBufSize)
{
    if (!t || !tBuf || 0 == tBufSize) { return ; }

    my_GetCurrentTime(t);
    snprintf(tBuf, tBufSize, "[%s]", t->Date);
}


static void mSleep(UINT32 ms)
{
#if defined(WIN32DLL) || defined(WINDOWS)
    Sleep(ms);
#elif defined LINUX
    usleep(ms * 1000);
#endif
}


static void error(const char *msg)
{
    st_info("%s errno=%d (%s)\n", msg, errno, strerror(errno));
    exit(0);
}

// return:0->OK,-1->error.
static int WinSockInit()
{

    return 0;
}

static void WinSockDeInit()
{

}

//// ret = 0: UDP, 1:TCP
static INT32 GetSocketType(INT32 Skt)
{
    socklen_t length = sizeof(unsigned int);
    int type;
    getsockopt(Skt, SOL_SOCKET, SO_TYPE, (char *)&type, &length);

    if (type == SOCK_STREAM) { return 1; }
    else { return 0; }
}

//// ret=0 OK, ret=-1: Invalid Parameter, ret=-2: No such Item
static int GetStringItem(  const char *SrcStr,
                           const char *ItemName,
                           const char Seperator,
                           char *RetString,
                           const int MaxSize)
{
    if (!SrcStr || !ItemName || !RetString || 0 == MaxSize)
    {
        return -1;
    }

    const char *pFand = SrcStr;

    while (1)
    {
        pFand = strstr(pFand, ItemName);

        if (!pFand) { return -2; }

        pFand += strlen(ItemName);

        if ('=' != *pFand) { continue; }
        else { break; }
    }

    memset(RetString, 0, MaxSize);
    pFand += 1;
    int i = 0;

    while (1)
    {
        if (Seperator == *(pFand + i) || '\0' == *(pFand + i) || i >= (MaxSize - 1))
        {
            break;
        }
        else
        {
            *(RetString + i) = *(pFand + i);
        }

        i++;
    }

    *(RetString + i) = '\0';
    return 0;
}

//// ret=0 OK, ret=-1: Invalid Parameter, ret=-2: No such Item
static int GetStatusValue(const char *SrcStr, const char *ItemName, const char Seperator, char *RetString, const int MaxSize)
{
    if (!SrcStr || !ItemName || !RetString || 0 == MaxSize) { return -1; }

    const char *pFand = SrcStr;

    while (1)
    {
        pFand = strstr(pFand, ItemName);

        if (!pFand) { return -2; }

        pFand += strlen(ItemName) + 1;

        if (':' != *pFand) { continue; }
        else { break; }
    }

    pFand += 1;
    int i = 0;

    while (1)
    {
        if (Seperator == *(pFand + i) || '\r' == *(pFand + i) || ',' == *(pFand + i) || '\n' == *(pFand + i) || '\0' == *(pFand + i) || i >= (MaxSize - 1)) { break; }
        else { *(RetString + i) = *(pFand + i); }

        i++;
    }

    *(RetString + i) = '\0';
    return 0;
}

static const char *getP2PErrorCodeInfo(int err)
{
    switch (err)
    {
    case 0: return "ERROR_PPCS_SUCCESSFUL";

    case -1: return "ERROR_PPCS_NOT_INITIALIZED"; // API didn't initialized

    case -2: return "ERROR_PPCS_ALREADY_INITIALIZED";

    case -3: return "ERROR_PPCS_TIME_OUT";

    case -4: return "ERROR_PPCS_INVALID_ID";//Invalid Device ID !!

    case -5: return "ERROR_PPCS_INVALID_PARAMETER";

    case -6: return "ERROR_PPCS_DEVICE_NOT_ONLINE";

    case -7: return "ERROR_PPCS_FAIL_TO_RESOLVE_NAME";

    case -8: return "ERROR_PPCS_INVALID_PREFIX";//Prefix of Device ID is not accepted by Server !!

    case -9: return "ERROR_PPCS_ID_OUT_OF_DATE";

    case -10: return "ERROR_PPCS_NO_RELAY_SERVER_AVAILABLE";

    case -11: return "ERROR_PPCS_INVALID_SESSION_HANDLE";

    case -12: return "ERROR_PPCS_SESSION_CLOSED_REMOTE";

    case -13: return "ERROR_PPCS_SESSION_CLOSED_TIMEOUT";

    case -14: return "ERROR_PPCS_SESSION_CLOSED_CALLED";

    case -15: return "ERROR_PPCS_REMOTE_SITE_BUFFER_FULL";

    case -16: return "ERROR_PPCS_USER_LISTEN_BREAK";//Listen break is called !!

    case -17: return "ERROR_PPCS_MAX_SESSION";//Exceed max session !!

    case -18: return "ERROR_PPCS_UDP_PORT_BIND_FAILED";//The specified UDP port can not be binded !!

    case -19: return "ERROR_PPCS_USER_CONNECT_BREAK";

    case -20: return "ERROR_PPCS_SESSION_CLOSED_INSUFFICIENT_MEMORY";

    case -21: return "ERROR_PPCS_INVALID_APILICENSE";//API License code is not correct !!

    case -22: return "ERROR_PPCS_FAIL_TO_CREATE_THREAD";//Fail to Create Thread !!

    case -23: return "ERROR_PPCS_INVALID_DSK";

    case -24: return "ERROR_PPCS_FAILED_TO_CONNECT_TCP_RELAY";

    case -25: return "ERROR_PPCS_FAIL_TO_ALLOCATE_MEMORY"; // only availeable since P2P API ver: 4.2.0.0

    default: return "Unknown error!!";
    }
} // getP2PErrorCodeInfo

static const char *getListenErrorInfo(int ret)
{
    switch (ret)
    {
    case ERROR_PPCS_NOT_INITIALIZED: return "API didn't initialized";

    case ERROR_PPCS_TIME_OUT: return "Listen time out, No client connect me !!";

    case ERROR_PPCS_INVALID_ID: return "Invalid Device ID !!";

    case ERROR_PPCS_INVALID_PREFIX: return "Prefix of Device ID is not accepted by Server !!";

    case ERROR_PPCS_UDP_PORT_BIND_FAILED: return "The specified UDP port can not be binded !!";

    case ERROR_PPCS_MAX_SESSION: return "Exceed max session !!";

    case ERROR_PPCS_USER_LISTEN_BREAK: return "Listen break is called !!";

    case ERROR_PPCS_INVALID_APILICENSE: return "API License code is not correct !!";

    case ERROR_PPCS_FAIL_TO_CREATE_THREAD: return "Fail tO Create Thread !!";

    default: return getP2PErrorCodeInfo(ret);
    }
}

static void showNetwork(st_PPCS_NetInfo NetInfo)
{
    st_info("-------------- NetInfo: -------------------\n");
    st_info("Internet Reachable     : %s\n", (NetInfo.bFlagInternet == 1) ? "YES" : "NO");
    st_info("P2P Server IP resolved : %s\n", (NetInfo.bFlagHostResolved == 1) ? "YES" : "NO");
    st_info("P2P Server Hello Ack   : %s\n", (NetInfo.bFlagServerHello == 1) ? "YES" : "NO");

    switch (NetInfo.NAT_Type)
    {
    case 0: st_info("Local NAT Type         : Unknow\n"); break;

    case 1: st_info("Local NAT Type         : IP-Restricted Cone\n"); break;

    case 2: st_info("Local NAT Type         : Port-Restricted Cone\n"); break;

    case 3: st_info("Local NAT Type         : Symmetric\n"); break;

    case 4: st_info("Local NAT Type         : Different Wan IP Detected!!\n"); break;
    }

    st_info("My Wan IP : %s\n", NetInfo.MyWanIP);
    st_info("My Lan IP : %s\n", NetInfo.MyLanIP);
    st_info("-------------------------------------------\n");
}

static int iPN_StringEnc(const char *keystr, const char *src, char *dest, unsigned int maxsize)
{
    int Key[17] = {0};
    unsigned int i;
    unsigned int s, v;

    if (maxsize < strlen(src) * 2 + 3)
    {
        return -1;
    }

    for (i = 0 ; i < 16; i++)
    {
        Key[i] = keystr[i];
    }

    srand((unsigned int)time(NULL));
    s = abs(rand() % 256);
    memset(dest, 0, maxsize);
    dest[0] = 'A' + ((s & 0xF0) >> 4);
    dest[1] = 'a' + (s & 0x0F);

    for (i = 0; i < strlen(src); i++)
    {
        v = s ^ Key[(i + s * (s % 23)) % 16] ^ src[i];
        dest[2 * i + 2] = 'A' + ((v & 0xF0) >> 4);
        dest[2 * i + 3] = 'a' + (v & 0x0F);
        s = v;
    }

    return 0;
}

static int iPN_StringDnc(const char *keystr, const char *src, char *dest, unsigned int maxsize)
{
    int Key[17] = {0};
    unsigned int i;
    unsigned int s, v;

    if ((maxsize < strlen(src) / 2) || (strlen(src) % 2 == 1))
    {
        return -1;
    }

    for (i = 0 ; i < 16; i++)
    {
        Key[i] = keystr[i];
    }

    memset(dest, 0, maxsize);
    s = ((src[0] - 'A') << 4) + (src[1] - 'a');

    for (i = 0; i < strlen(src) / 2 - 1; i++)
    {
        v = ((src[i * 2 + 2] - 'A') << 4) + (src[i * 2 + 3] - 'a');
        dest[i] = v ^ Key[(i + s * (s % 23)) % 16] ^ s;

        if (dest[i] > 127 || dest[i] < 32)
        {
            return -1; // not a valid character string
        }

        s = v;
    }

    return 0;
}

static char *GetRandomString(char *output_String, int Len)
{
    srand((UINT32)time(NULL));

    for (int i = 0; i < Len; i++)
    {
        switch (abs(rand() % 5))
        {
        case 0: output_String[i] = 'A' + rand() % 26; break;

        case 1: output_String[i] = 'a' + rand() % 26; break;

        case 2: output_String[i] = '0' + rand() % 10; break;

        case 3: output_String[i] = 'A' + rand() % 26; break;

        case 4: output_String[i] = '0' + rand() % 10; break;

        default: output_String[i] = 'A' + rand() % 26; break;
        }
    }

    return output_String;
}

// -1:invalid parameter,0:not the same LAN Addresses,1:Addresses belonging to the same LAN.
static int isLANcmp(const char *IP1, const char *IP2)
{
#define   YES   1
#define   NO   0
    short Len_IP1 = strlen(IP1);
    short Len_IP2 = strlen(IP2);

    if (!IP1 || 7 > Len_IP1 || !IP2 || 7 > Len_IP2) { return -1; }

    if (0 == strcmp(IP1, IP2)) { return YES; }

    const char *pIndex = IP1 + Len_IP1 - 1;

    while (1)
    {
        if ('.' == *pIndex || pIndex == IP1) { break; }
        else { pIndex--; }
    }

    if (0 == strncmp(IP1, IP2, (int)(pIndex - IP1))) { return YES; }

    return NO;
}

static void DirectCommand_SL_Dump_cmd(st_DirectCommand *c)
{
    CHAR Date[SIZE_DATE] = {};
    st_info_T("\tDump Cmd (%p) ************\n", c);
    st_info_T("\tCmd CmdIndex=%05d\n", c->CmdIndex);
    st_info_T("\tCmd CmdTRID=%s\n", c->CmdTRID);
    st_info_T("\tCmd Command=%s\n", c->Command);
    st_info_T("\tCmd CmdSize=%d\n", c->CmdSize);
    st_info_T("\tCmd GotAck=%s\n", 1 == c->GotAck ? "Yes" : "No");
    st_info_T("\tCmd ResponseCounter=%d\n", c->ResponseCounter);
    st_info_T("\tCmd RmtAddr=%s:%d\n", inet_ntoa(c->remote_addr.sin_addr), ntohs(c->remote_addr.sin_port));
    st_info_T("\tCmd TimeStamp_ms=%ld (%s)\n", c->TimeStamp_ms, toDate((c->TimeStamp_ms / 1000), Date, sizeof(Date)));
}

static void DirectCommand_SL_Show()
{
    struct DirectCommand_List *p = g_DC_SL_head;
    INT32 i = 0;
    st_info_T(" ************ Dump of SL head:%p, head->next=%p ************\n", g_DC_SL_head, g_DC_SL_head->next);

    while (p)
    {
        i++;
        st_info_T("SL %d: %s\n", i, p->cmd ? "Occupied" : "(Empty)");

        if (p->cmd)
        {
            DirectCommand_SL_Dump_cmd(p->cmd);
        }

        p = p->next;
    }

    st_info_T(" ********* End of Dump of SL tail:%p  tail->next=%p *********\n", g_DC_SL_tail, g_DC_SL_tail->next);
}

//// ret: NULL: Malloc failed
static st_DirectCommand *DirectCommand_Alloc(const char *CmdTRID, const char *CmdBuf, int cmdSize, struct sockaddr_in *remote_addr)
{
    st_DirectCommand *cmd = (st_DirectCommand *)malloc(sizeof(st_DirectCommand));

    if (!cmd)
    {
        return NULL;
    }

    memset(cmd, 0, sizeof(st_DirectCommand));

    if (remote_addr)
    {
        memcpy(&cmd->remote_addr, remote_addr, sizeof(struct sockaddr_in));
    }

    if (CmdTRID)
    {
        memcpy(cmd->CmdTRID, CmdTRID, 32);
    }

    if (CmdBuf)
    {
        memcpy(cmd->Command, CmdBuf, cmdSize);
    }

    cmd->GotAck = 0;
    cmd->CmdSize = cmdSize;
    cmd->ResponseCounter = 0;
    st_Time_Info t;
    my_GetCurrentTime(&t);
    cmd->TimeStamp_ms = t.Tick_mSec;
    return cmd;
}

static void DirectCommand_Free(st_DirectCommand *cmd)
{
    if (cmd)
    {
        free(cmd);
        cmd = NULL;
    }
}

static struct DirectCommand_List *DirectCommand_SL_FindByCmdTRID(const char *Command)
{
    if (!Command)
    {
        return NULL;
    }

    struct DirectCommand_List *p = g_DC_SL_head;

    while (p)
    {
        if (0 == strncmp(p->cmd->CmdTRID, Command, 32))
        {
            return p;
        }

        p = p->next;
    }

    return p;
}

//// ret 1: Successfully added, 0: already inside SL, -1: parameter invalid!! -2: Malloc failed
static int DirectCommand_SL_Add(st_DirectCommand *cmd)
{
    if (!cmd)
    {
        return -1;
    }

    if (NULL != DirectCommand_SL_FindByCmdTRID(cmd->CmdTRID))
    {
        return 0;
    }

    struct DirectCommand_List *p = (struct DirectCommand_List *)malloc(sizeof(struct DirectCommand_List));

    if (!p)
    {
        return -2;
    }

    memset(p, 0, sizeof(struct DirectCommand_List));
    p->cmd = cmd;
    p->next = NULL;

    if (NULL == g_DC_SL_head)
    {
        g_DC_SL_head = p;
        g_DC_SL_tail = p;
    }
    else
    {
        g_DC_SL_tail->next = p;
        g_DC_SL_tail = p;
    }

    g_DirectCommand_SL_TotalCount++;
    return 1;
}

static void DirectCommand_SL_Remove(st_DirectCommand *cmd)
{
    if (!cmd)
    {
        return;
    }

    struct DirectCommand_List *p = g_DC_SL_head;

    struct DirectCommand_List *pp = NULL;

    while (p)
    {
        if (p->cmd == cmd)
        {
            if (pp)
            {
                pp->next = p->next;
            }

            if (p == g_DC_SL_head)
            {
                g_DC_SL_head = p->next;
            }

            if (p == g_DC_SL_tail)
            {
                g_DC_SL_tail->next = NULL;
                g_DC_SL_tail = pp;
            }

            DirectCommand_Free(cmd);
            free(p);
            p = NULL;
            g_DirectCommand_SL_TotalCount--;
            return;
        }

        pp = p;
        p = p->next;
    }
}

static void DirectCommand_SL_FreeAll()
{
    struct DirectCommand_List *p = NULL;

    while (g_DC_SL_head)
    {
        p = g_DC_SL_head;

        if (p->cmd)
        {
            DirectCommand_Free(p->cmd);
            p->cmd = NULL;
        }

        g_DC_SL_head = p->next;
        g_DirectCommand_SL_TotalCount--;
        free(p);
    }
}


#if defined(WIN32DLL) || defined(WINDOWS)
    static DWORD WINAPI Thread_DirectCommand_Response(void *arg)
#elif defined(LINUX)
    static void *Thread_DirectCommand_Response(void *arg)
#endif
{
#if defined LINUX
    pthread_detach(pthread_self());
#endif
    gThread_DC_Response_Running = 1;
    st_debug_T("Thread_DirectCommand_Response start running ...\n");
    st_Time_Info t;

    while (0 == gThread_DirectCommand_Server_Exit)
    {
        my_Mutex_Lock(g_DC_LOCK);
        struct DirectCommand_List *p = g_DC_SL_head;

        while (p)
        {
            if (p->cmd && 0 == p->cmd->GotAck && g_ResponseRetryTimes >= p->cmd->ResponseCounter)
            {
                my_GetCurrentTime(&t);

                if (0 == p->cmd->ResponseCounter || (t.Tick_mSec >= p->cmd->TimeStamp_ms + g_ResponseRetryInterval_sec * 1000))
                {
                    //// 原封不动 echo 原数据
                    char Resp[996] = {};
                    strncpy(Resp, p->cmd->CmdTRID, 32);
                    strncat(Resp, p->cmd->Command, p->cmd->CmdSize);
                    int ret = sendto(g_DC_Socket, Resp, p->cmd->CmdSize + 32, 0, (struct sockaddr *) & (p->cmd->remote_addr), sizeof(struct sockaddr_in));

                    if (0 > ret)
                    {
                        st_info_T("%03d-3 Send DC Response: sendto failed: Skt=%d, errno=%d [%s]\n", p->cmd->CmdIndex, g_DC_Socket, errno, strerror(errno));
                        break;
                    }
                    else
                    {
                        st_info_T("%03d-3 Send DC Response: CmdTRID=%s,Resp(%d)=%s\n", p->cmd->CmdIndex, p->cmd->CmdTRID, p->cmd->CmdSize, p->cmd->Command);
                        p->cmd->ResponseCounter++;
                        p->cmd->TimeStamp_ms = t.Tick_mSec;
                    }
                }
            }

            if (g_ResponseRetryTimes < p->cmd->ResponseCounter)
            {
                char CmdTRID[36] = {};
                memcpy(CmdTRID, p->cmd->CmdTRID, 32);
                unsigned long CmdIndex = p->cmd->CmdIndex;
                DirectCommand_SL_Remove(p->cmd);
                st_info_T("%03d-Cmd remove!!! CmdTRID=%s\n", CmdIndex, CmdTRID);
                break;
            }

            p = p->next;
        } // while

        my_Mutex_UnLock(g_DC_LOCK);
        mSleep(1);
    } // while

    gThread_DC_Response_Running = 0;
    st_debug_T("Thread_DirectCommand_Response exit!!\n");
    my_Thread_exit(0);
}


static void CreateThread_DirectCommand_Response()
{
#if defined(WIN32DLL) || defined(WINDOWS)
    HANDLE hThread_DC_Response = CreateThread(NULL, 0, Thread_DirectCommand_Response, NULL, 0, NULL);

    if (NULL == hThread_DC_Response)
    {
        error("create Thread_DirectCommand_Response failed");
    }
    else { CloseHandle(hThread_DC_Response); }

#elif defined(LINUX)
    pthread_t hThread_DC_Response;

    if (0 != pthread_create(&hThread_DC_Response, NULL, &Thread_DirectCommand_Response, NULL ))
    {
        error("create Thread_DirectCommand_Response failed");
    }

#endif
}

//// 监听本地 UDP 16888 端口
#if defined(WIN32DLL) || defined(WINDOWS)
    static DWORD WINAPI Thread_DirectCommand_Server(void *arg)
#elif defined(LINUX)
    static void *Thread_DirectCommand_Server(void *arg)
#endif
{
#if defined LINUX
    pthread_detach(pthread_self());
#endif
    gThread_DirectCommand_Server_Exit = 0;
    st_info_T("Thread_DirectCommand_Server: start running ...\n");

    int mSocket;
    struct sockaddr_in local_addr;
    struct sockaddr_in remote_addr;
    unsigned int sin_size;
    unsigned int value = 0x1;
    int LocalPort = 16888;

    memset((void *)&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(LocalPort);

    for (int i = 0, times = 3; i < times; i++)
    {
        if (0 > (mSocket = socket(PF_INET, SOCK_DGRAM, 0)))
        {
            st_info_T("DirectCommand_Server: create UDP Socket failed: errno=%d, %s\n", errno, strerror(errno));

            if (i == times - 1)
            {
                st_info_T("DirectCommand_Server: Thread exit for create UDP Socket failed: errno=%d [%s]\n", errno, strerror(errno));
                my_Thread_exit(0);
            }

            mSleep(1000);
            continue;
        }

        g_DC_Socket = mSocket;
        st_debug_T("DirectCommand_Server: Create UDP Socket OK: Skt=%d\n", mSocket);

        if (0 > setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&value, sizeof(value)))
        {
            st_info_T("DirectCommand_Server: Setsockopt(Skt=%d, SO_REUSEADDR) failed: errno=%d (%s)\n", mSocket, errno, strerror(errno));
            my_SocketClose(mSocket);

            if (i == times - 1)
            {
                st_info_T("DirectCommand_Server: Thread exit for Setsockopt(Skt=%d, SO_REUSEADDR) failed: errno=%d [%s]\n", mSocket, errno, strerror(errno));
                my_Thread_exit(0);
            }

            mSleep(1000);
            continue;
        }

        st_debug_T("DirectCommand_Server: Setsockopt(Skt=%d, SO_REUSEADDR) OK!\n", mSocket);

        if (0 > bind(mSocket, (struct sockaddr *)&local_addr, sizeof(struct sockaddr)))
        {
            st_debug_T("DirectCommand_Server: UDP Port bind failed: LocalPort=%d, Skt=%d, errno=%d (%s)\n", LocalPort, mSocket, errno, strerror(errno));
            my_SocketClose(mSocket);

            if (i == times - 1)
            {
                st_debug_T("DirectCommand_Server: Thread exit for UDP Port bind failed: LocalPort=%d, Skt=%d, errno=%d [%s]\n", LocalPort, mSocket, errno, strerror(errno));
                my_Thread_exit(0);
            }

            mSleep(1000);
            continue;
        }

        st_debug_T("DirectCommand_Server: UDP Port Bind OK!! LocalPort=%d, Skt=%d\n", LocalPort, mSocket);
        break;
    } // for

    char RecvBuf[1024] = {};
    sin_size = sizeof(struct sockaddr_in);
    unsigned long CmdIndex = 0;

    while (0 == gThread_DirectCommand_Server_Exit)
    {
        int ret = -1;
        memset(RecvBuf, 0, sizeof(RecvBuf));
        // st_debug_T("Recv DirectCommand: Skt=%d, recvfrom ...\n", mSocket);
        // unsigned int TimeOut_ms = 10000;
        // int RecvSize = UDP_Recv_Till_TimeOut(mSocket, RecvBuf, sizeof(RecvBuf), &remote_addr, TimeOut_ms);
        int RecvSize = recvfrom(mSocket, RecvBuf, sizeof(RecvBuf), 0, (struct sockaddr *)&remote_addr, (socklen_t *)&sin_size);

        if (0 >= RecvSize)
        {
            st_info_T("Recv DirectCommand: recvfrom failed: ret=%d, Skt=%d, errno=%d [%s]\n", RecvSize, mSocket, errno, strerror(errno));
            break;
        }

        // if (0 == RecvSize)
        // {
        //     st_debug_T("Recv DirectCommand: timeout!!\n");
        //     continue;
        // }

        my_Mutex_Lock(g_DC_LOCK);

        if (32 < RecvSize) // is a DirectCommand
        {
            char CmdTRID[36] = {};
            memcpy(CmdTRID, RecvBuf, 32);
            // char CmdBuf[256] = {};
            // urldecode2(CmdBuf, RecvBuf+32);
            st_info_T("%03d-1 Recv DC Command: CmdTRID=%s,Cmd(%u)=%s\n", CmdIndex, CmdTRID, RecvSize - 32, RecvBuf + 32);
            //// 回应前 32 byte ack.
            ret = sendto(mSocket, CmdTRID, 32, 0, (struct sockaddr *)&remote_addr, sin_size);

            if (0 > ret)
            {
                st_info_T("%03d-2 Send DC CmdAck: sendto failed: Skt=%d, errno=%d [%s]\n", CmdIndex, mSocket, errno, strerror(errno));
                my_Mutex_UnLock(g_DC_LOCK);
                break;
            }
            else { st_info_T("%03d-2 Send DC CmdAck: CmdTRID=%s\n", CmdIndex, CmdTRID); }

            if (g_EchoResponse) // 是否回应
            {
                st_DirectCommand *cmd = DirectCommand_Alloc(CmdTRID, RecvBuf + 32, RecvSize - 32, &remote_addr);

                if (!cmd)
                {
                    st_info_T("Thread_DirectCommand_Server <ERROR> [Malloc failed!!]\n");
                    my_Mutex_UnLock(g_DC_LOCK);
                    exit(0);
                }

                cmd->CmdIndex = CmdIndex;

                ret = DirectCommand_SL_Add(cmd);

                if (-1 == ret)
                {
                    st_info_T("Thread_DirectCommand_Server <ERROR> [DirectCommand_SL_Add: cmd is NULL!!]\n");
                }

                if (-2 == ret)
                {
                    st_info_T("Thread_DirectCommand_Server <ERROR> [DirectCommand_SL_Add: cmd malloc failed!!]\n");
                }
                else if (0 == ret)
                {
                    st_info_T("Thread_DirectCommand_Server <WARN> [DirectCommand_SL_Add: cmd is alread in list!!]\n");
                }

                // else if (1 == ret)
                //     st_info_T("Thread_DirectCommand_Server <OK> [DirectCommand_SL_Add: cmd Add to list Success!!] CmdIndex=%d, CmdTRID=%s, Total=%d\n", CmdIndex, CmdTRID, g_DirectCommand_SL_TotalCount);
                // DirectCommand_SL_Show();

                if (0 == gThread_DC_Response_Running)
                {
                    CreateThread_DirectCommand_Response();
                }
            }

            CmdIndex++;
        }
        else if (32 == RecvSize)
        {
            struct DirectCommand_List *p = DirectCommand_SL_FindByCmdTRID(RecvBuf);

            if (p)
            {
                p->cmd->GotAck = 1;
                st_info_T("%03d-4 Recv DC RespAck: CmdTRID=%s\n", p->cmd->CmdIndex, RecvBuf);
                DirectCommand_SL_Remove(p->cmd);
            }
        }
        else
        {
            st_info_T("Recv DirectCommand: recvfrom <WARN> [Unknown Size!!] Skt=%d, RecvSize=%d, RecvBuf=%s\n", mSocket, RecvSize, RecvSize);
        }

        my_Mutex_UnLock(g_DC_LOCK);
    } // while

    my_SocketClose(mSocket);

    DirectCommand_SL_FreeAll();
    st_info_T("Thread_DirectCommand_Server: job done, thread exit!!\n");
    my_Thread_exit(0);
} // Thread_DirectCommand_Server

static void CreateThread_DirectCommand_Server()
{
#if defined(WIN32DLL) || defined(WINDOWS)
    HANDLE hThread_DC_Server = CreateThread(NULL, 0, Thread_DirectCommand_Server, NULL, 0, NULL);

    if (NULL == hThread_DC_Server)
    {
        error("create Thread_DirectCommand_Server failed");
    }
    else { CloseHandle(hThread_DC_Server); }

#elif defined(LINUX)
    pthread_t hThread_DC_Server;

    if (0 != pthread_create(&hThread_DC_Server, NULL, &Thread_DirectCommand_Server, NULL ))
    {
        error("create Thread_DirectCommand_Server failed");
    }

#endif
}

//// 监听 8899 端口，收到数据原封不动echo 回去。
#if defined(WIN32DLL) || defined(WINDOWS)
    static DWORD WINAPI Thread_UDP_RECV_ECHO(void *arg)
#elif defined(LINUX)
    static void *Thread_UDP_RECV_ECHO(void *arg)
#endif
{
#if defined LINUX
    pthread_detach(pthread_self());
    // st_debug_T("pthread_detach(pthread_self()) is Called!!\n");
#endif
    gThread_UDP_ECHO_Exit = 0;
    st_Time_Info t1, t2;
    char tBuf[SIZE_DATE] = {};
    // GetTime_ms(&tBegin, tBuf, sizeof(tBuf));

    int mSocket;
    struct sockaddr_in local_addr;
    struct sockaddr_in remote_addr;
    unsigned int sin_size;
    unsigned int value = 0x1;
    int Port = 8899;

    memset((void *)&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(Port);

    for (int i = 0, times = 3; i < times; i++)
    {
        if (0 > (mSocket = socket(PF_INET, SOCK_DGRAM, 0)))
        {
            st_info_T("UDP_RECV_ECHO: create UDP Socket failed: errno=%d, %s\n", errno, strerror(errno));

            if (i == times - 1)
            {
                st_info_T("UDP_RECV_ECHO: Thread exit for create UDP Socket failed: errno=%d [%s]\n", errno, strerror(errno));
                my_Thread_exit(0);
            }

            mSleep(1000);
            continue;
        }

        gUDP_ECHO_Socket = mSocket;
        st_debug_T("UDP_RECV_ECHO: Create UDP Socket OK: Skt=%d\n", mSocket);

        if (0 > setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&value, sizeof(value)))
        {
            st_info_T("UDP_RECV_ECHO: Setsockopt(Skt=%d, SO_REUSEADDR) failed: errno=%d (%s)\n", mSocket, errno, strerror(errno));
            my_SocketClose(mSocket);

            if (i == times - 1)
            {
                st_info_T("UDP_RECV_ECHO: Thread exit for Setsockopt(Skt=%d, SO_REUSEADDR) failed: errno=%d [%s]\n", mSocket, errno, strerror(errno));
                my_Thread_exit(0);
            }

            mSleep(1000);
            continue;
        }

        st_debug_T("UDP_RECV_ECHO: Setsockopt(Skt=%d, SO_REUSEADDR) OK!\n", mSocket);

        if (0 > bind(mSocket, (struct sockaddr *)&local_addr, sizeof(struct sockaddr)))
        {
            st_debug_T("UDP_RECV_ECHO: UDP Port bind failed: Port=%d, Skt=%d, errno=%d (%s)\n", Port, mSocket, errno, strerror(errno));
            my_SocketClose(mSocket);

            if (i == times - 1)
            {
                st_debug_T("UDP_RECV_ECHO: Thread exit for UDP Port bind failed: Port=%d, Skt=%d, errno=%d [%s]\n", Port, mSocket, errno, strerror(errno));
                my_Thread_exit(0);
            }

            mSleep(1000);
            continue;
        }

        st_debug_T("UDP_RECV_ECHO: UDP Port Bind OK!! Port=%d, Skt=%d\n", Port, mSocket);
        break;
    } // for

    char ReadBuf[1024] = {};
    memset(ReadBuf, 0, sizeof(ReadBuf));
    sin_size = sizeof(struct sockaddr_in);
    st_debug_T("UDP_RECV_ECHO: Skt=%d, recvfrom Running ...\n", mSocket);

    while (0 == gThread_UDP_ECHO_Exit)
    {
        int ReadSize = recvfrom(mSocket, ReadBuf, sizeof(ReadBuf), 0, (struct sockaddr *)&remote_addr, (socklen_t *)&sin_size);
        GetTime_ms(&t1, tBuf, sizeof(tBuf));

        if (0 >= ReadSize)
        {
            st_debug("%s UDP_RECV_ECHO: recvfrom failed: Skt=%d, errno=%d [%s]\n", tBuf, mSocket, errno, strerror(errno));
            break;
        }

        // st_debug("%s UDP_RECV_ECHO: from %s:%d, Size=%u Byte\n", tBuf, inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port), ReadSize);
        int ret = sendto(mSocket, ReadBuf, ReadSize, 0, (struct sockaddr *)&remote_addr, sin_size);
        GetTime_ms(&t2, tBuf, sizeof(tBuf));

        if (0 > ret)
        {
            st_debug("%s UDP_RECV_ECHO: sendto %s:%d failed: Skt=%d, errno=%d [%s]\n", tBuf, inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port), mSocket, errno, strerror(errno));
            break;
        }
        else
        {
            st_debug("%s UDP_RECV_ECHO: sendto %s:%d, Size=%u Byte, Time=%d ms\n", tBuf, inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port), ReadSize, TU_MS(t1, t2));
        }
    } // while

    my_SocketClose(mSocket);
    st_debug_T("UDP_RECV_ECHO: job done, thread exit!!\n");
    my_Thread_exit(0);
} // Thread_UDP_RECV_ECHO

static void CreateThread_UDP_RECV_ECHO()
{
#if defined(WIN32DLL) || defined(WINDOWS)
    HANDLE hThread_UDP_RECV_ECHO = CreateThread(NULL, 0, Thread_UDP_RECV_ECHO, NULL, 0, NULL);

    if (NULL == hThread_UDP_RECV_ECHO)
    {
        error("create Thread_UDP_RECV_ECHO failed");
    }
    else { CloseHandle(hThread_UDP_RECV_ECHO); }

#elif defined(LINUX)
    pthread_t hThread_UDP_RECV_ECHO;

    if (0 != pthread_create(&hThread_UDP_RECV_ECHO, NULL, &Thread_UDP_RECV_ECHO, NULL ))
    {
        error("create Thread_UDP_RECV_ECHO failed");
    }

#endif
}

static int MyGetSInfo(int SessionID, st_MySess_Info *MySInfo)
{
    memset(MySInfo, 0, sizeof(st_MySess_Info));
    st_PPCS_Session Sinfo;
    int ret = PPCS_Check(SessionID, &Sinfo);

    if (ERROR_PPCS_SUCCESSFUL == ret)
    {
        MySInfo->Skt = Sinfo.Skt;
        // Remote addr
        snprintf(MySInfo->RemoteIP, sizeof(MySInfo->RemoteIP), "%s", inet_ntoa(Sinfo.RemoteAddr.sin_addr));
        MySInfo->RemotePort = ntohs(Sinfo.RemoteAddr.sin_port);
        // Lan addr
        snprintf(MySInfo->MyLocalIP, sizeof(MySInfo->MyLocalIP), "%s", inet_ntoa(Sinfo.MyLocalAddr.sin_addr));
        MySInfo->MyLocalPort = ntohs(Sinfo.MyLocalAddr.sin_port);
        // Wan addr
        snprintf(MySInfo->MyWanIP, sizeof(MySInfo->MyWanIP), "%s", inet_ntoa(Sinfo.MyWanAddr.sin_addr));
        MySInfo->MyWanPort = ntohs(Sinfo.MyWanAddr.sin_port);

        MySInfo->ConnectTime = Sinfo.ConnectTime;
        memcpy(MySInfo->DID, Sinfo.DID, strlen(Sinfo.DID));
        MySInfo->bCorD = Sinfo.bCorD;

        if (0 == Sinfo.bMode)
        {
            if (Sinfo.RemoteAddr.sin_addr.s_addr == Sinfo.MyLocalAddr.sin_addr.s_addr || 1 == isLANcmp(MySInfo->MyLocalIP, MySInfo->RemoteIP))
            {
                MySInfo->bMyMode = 0;
                1 == GetSocketType(Sinfo.Skt) ? memcpy(MySInfo->Mode, "LAN.", 4) : memcpy(MySInfo->Mode, "LAN", 3);
            }
            else { MySInfo->bMyMode = 1; memcpy(MySInfo->Mode, "P2P", 3); }
        }
        else if (1 == Sinfo.bMode) { MySInfo->bMyMode = 2; memcpy(MySInfo->Mode, "RLY", 3); }
        else if (2 == Sinfo.bMode) { MySInfo->bMyMode = 3; memcpy(MySInfo->Mode, "TCP", 3); }
        else if (3 == Sinfo.bMode) { MySInfo->bMyMode = 4; memcpy(MySInfo->Mode, "RP2P", 4); }  //// support by P2P API 5.0.8
    }

    // else st_info("PPCS_Check(SessionID=%d) ret=%d [%s]\n", SessionID, ret, getP2PErrorCodeInfo(ret));
    return ret;
}

static void CreateThread_LoginStatus_Check()
{
    gThread_LoginCheck_Exit = 0;
#if defined(WIN32DLL) || defined(WINDOWS)
    HANDLE hthread_LoginStatus_Check = CreateThread(NULL, 0, Thread_LoginStatus_Check, NULL, 0, NULL);

    if (NULL == hthread_LoginStatus_Check) { error("create Thread LoginStatus_Check failed"); }
    else { CloseHandle(hthread_LoginStatus_Check); }

#elif 0//defined(LINUX)
    pthread_t threadID_LoginStatus_Check;
    int err = pthread_create(&threadID_LoginStatus_Check, NULL, &Thread_LoginStatus_Check, NULL);

    if (0 != err) { error("create Thread LoginStatus_Check failed"); }

#endif
}



static int32_t picserver_send_raw(int SessionID, uint8_t Channel, uint8_t *buff, uint32_t size)
{
    int32_t ret = 0;
    int32_t Check_ret = 0;
    UINT32 WriteSize = size;
    uint32_t write_index = 0;
    const uint32_t write_not_send_thr = 1024 * 256;//1024 * 128; //CHECK_WRITE_THRESHOLD_SIZE
    const uint32_t write_per_count_thr = size;///write_not_send_thr / 10;//1024 * 2;

    if (size == 0)
    {
        return 0;
    }

    do
    {
        uint32_t will_write_size = ((size - write_index < write_per_count_thr) ? (size - write_index) : write_per_count_thr);
        // 在调用 PPCS_Write 之前一定要调用 PPCS_Check_Buffer 检测写缓存还有多少数据尚未发出去，需控制在一个合理范围，一般控制在 128KB/256KB 左右。
        Check_ret = PPCS_Check_Buffer(SessionID, Channel, &WriteSize, NULL);

        // st_debug("ThreadWrite PPCS_Check_Buffer: Session=%d,CH=%d,WriteSize=%d,ret=%d %s\n", SessionID, Channel, WriteSize, Check_ret, getP2PErrorCodeInfo(Check_ret));
        if (0 > Check_ret)
        {
            st_info("%s PPCS_Check_Buffer: Session=%d,CH=%d,WriteSize=%d,ret=%d %s\n", __func__, SessionID, Channel, WriteSize, Check_ret, getP2PErrorCodeInfo(Check_ret));
            goto WRITE_FAIL;
            break;
        }

        // 写缓存的数据大小超过128KB/256KB，则需考虑延时缓一缓。
        // 如果发现 wsize 越来越大，可能网络状态很差，需要考虑一下丢帧或降码率，这是一个动态调整策略，非常重要!!
        // On device, Recommended CHECK_WRITE_THRESHOLD_SIZE == (128 or 256) * 1024 Byte. this sample set 1MB.

        if (WriteSize <= write_not_send_thr)
        {
            if (s_log_level >= CS2_P2P_LOG_LEVEL_DEBUG)
            {
                BK_LOGI("p2p", "%s start write %d WriteSize %d SessionID %d channel %d\n", __func__, will_write_size, WriteSize, SessionID, Channel);
            }
            ret = PPCS_Write(SessionID, Channel, (CHAR *)(buff + write_index), will_write_size);

            if (0 > ret)
            {
                if (ERROR_PPCS_SESSION_CLOSED_TIMEOUT == ret)
                {
                    st_info("%s Session=%d,CH=%d,ret=%d, Session Closed TimeOUT!!\n", __func__, SessionID, Channel, ret);
                }
                else if (ERROR_PPCS_SESSION_CLOSED_REMOTE == ret)
                {
                    st_info("%s Session=%d,CH=%d,ret=%d, Session Remote Closed!!\n", __func__, SessionID, Channel, ret);
                }
                else if (ERROR_PPCS_INVALID_PARAMETER == ret)
                {
                    st_info("%s Session=%d,CH=%d,ret=%d, ERROR_PPCS_INVALID_PARAMETER %d!!\n", __func__, SessionID, Channel, ret, will_write_size);
                }
                else { st_info("%s Session=%d,CH=%d,ret=%d %s\n", __func__, SessionID, Channel, ret, getP2PErrorCodeInfo(ret)); }

                goto WRITE_FAIL;
            }
            else
            {
                if (s_log_level >= CS2_P2P_LOG_LEVEL_DEBUG)
                {
                    BK_LOGI("p2p", "%s end write %d\n", __func__, ret);
                }
            }

            write_index += ret;
        }
        else
        {
            if (s_log_level >= CS2_P2P_LOG_LEVEL_DEBUG)
            {
                BK_LOGI("p2p", "%s WriteSize %d\n", __func__, WriteSize);
            }
            break;
        }
    }
    while (write_index < size);

    return write_index;

WRITE_FAIL:;
    return -1;
}



static int RecvData(int skt, char *readBuf, int BufSize)
{
#if defined(WIN32DLL) || defined(WINDOWS)
    return recv(skt, readBuf, BufSize, 0);
#else
    return read(skt, readBuf, BufSize);
#endif
}
//// ret: 0: OK, -1: conf file no find, -2: Item not find.
static int getDirectCommandParameter()
{
#if 0
    const char *FilePath = "./ListenTester.conf";
    FILE *pf = fopen(FilePath, "r");

    if (!pf)
    {
        g_EchoResponse = 1;
        g_ResponseRetryTimes = 3;
        g_ResponseRetryInterval_sec = 2;
        return -1;
    }

    fclose(pf);

    int ret = -1;
    char Buf[32] = {};

    if (0 != (ret = GetConfigItem(FilePath, "EchoResponse", Buf, sizeof(Buf))))
    {
        st_info("getDirectCommandParameter: GetConfigItem(EchoResponse) failed %d\n", ret);
        return -2;
    }

    if (strstr(Buf, "Yes") || strstr(Buf, "YES") || strstr(Buf, "yes"))
    {
        g_EchoResponse = 1;
    }
    else if (strstr(Buf, "No") || strstr(Buf, "NO") || strstr(Buf, "no"))
    {
        g_EchoResponse = 0;
    }

    if (0 != (ret = GetConfigItem(FilePath, "ResponseRetryTimes", Buf, sizeof(Buf))))
    {
        st_info("getDirectCommandParameter: GetConfigItem(ResponseRetryTimes) failed %d\n", ret);
        return -2;
    }

    g_ResponseRetryTimes = atoi(Buf);

    if (0 != (ret = GetConfigItem(FilePath, "ResponseRetryInterval_sec", Buf, sizeof(Buf))))
    {
        st_info("getDirectCommandParameter: GetConfigItem(ResponseRetryInterval_sec) failed %d\n", ret);
        return -2;
    }

    g_ResponseRetryInterval_sec = atoi(Buf);
#else
    g_EchoResponse = 1;
    g_ResponseRetryTimes = 3;
    g_ResponseRetryInterval_sec = 2;
#endif
    return 0;
}

static int my_p2p_listen(const char *DID, const char *APILicense, unsigned long Repeat, uint8_t *is_run)
{
    st_Time_Info t1, t2;
    int ret = -1;
    //    void *task_ret = NULL;
    int SessionID = -99;
    unsigned int TimeOut_Sec = 120;
    unsigned short UDP_Port = 0;// PPCS_Listen 端口填 0 让底层自动分配。
    char bEnableInternet = 2;

    memset(&t2, 0, sizeof(t2));

    if (1 == Repeat || 1 == gFlags_WakeupTest) { st_info_T("%02lu-PPCS_Listen(%s,%d,%d,%d,%s) ...\n", Repeat, DID, TimeOut_Sec, UDP_Port, bEnableInternet, APILicense); }
    else { st_debug_T("%02lu-PPCS_Listen(%s,%d,%d,%d,%s) ...\n", Repeat, DID, TimeOut_Sec, UDP_Port, bEnableInternet, APILicense); }

    // st_info("PPCS_Listen(%s,%d,%d,%d,%s)...\n", DID, TimeOut_Sec, UDP_Port, bEnableInternet, APILicense);
    my_GetCurrentTime(&t1);

    do
    {
        ret = PPCS_Listen(DID, TimeOut_Sec, UDP_Port, bEnableInternet, APILicense);
    }
    while (ERROR_PPCS_TIME_OUT == ret && *is_run);

    my_GetCurrentTime(&t2);

    if (*is_run == 0)
    {
        BK_LOGI("p2p", "%s is_run is 0, exit\n", __func__);

        if (ret >= 0)
        {
            PPCS_ForceClose(ret);
        }

        return -1;
    }

    if (0 > ret)
    {
        st_info("[%s] %02lu-PPCS_Listen failed:%d ms, ret=%d %s\n", t2.Date, Repeat, TU_MS(t1, t2), ret, getListenErrorInfo(ret));
        return ret;
    }
    else //// ret >= 0, Listen OK, new client connect in.
    {
        SessionID = ret; // 每个 >=0 的 SessionID 都是一个正常的连接，本 sample 是单用户连接范例，多用户端连接注意要保留区分每一个 PPCS_Listen >=0 的 SessionID, 当连接断开或者 SessionID 不用时，必须要调 PPCS_Close(SessionID)/PPCS_ForceClose(SessionID) 关闭连线释放资源。
        PPCS_Share_Bandwidth(0); // 当有连接进来，关闭设备转发功能。
        st_debug_T("PPCS_Share_Bandwidth(0) is Called!!\n");
    }

    if (ERROR_PPCS_SUCCESSFUL != (ret = MyGetSInfo(SessionID, &g_MySInfo)))
    {
        // connect success, but remote call closed.
        st_info_T("%02lu-DID=%s,Session=%d,RmtAddr=Unknown (PPCS_Check:%d)\n", Repeat, DID, SessionID, ret);
        return SessionID;
    }

    return SessionID;
#if 0
    char bMyMode = g_MySInfo.bMyMode; // my define mode by PPCS_Check bMode(0:LAN/P2P, 1:RLY, 2:TCP): define bMyMode: 0:LAN, 1:P2P, 2:RLY, 3:TCP.
    int Skt = g_MySInfo.Skt;
    char *MyLanIP = g_MySInfo.MyLocalIP;
    int LocalPort = g_MySInfo.MyLocalPort;
    char *RmtIP = g_MySInfo.RemoteIP;
    int RmtPort = g_MySInfo.RemotePort;
    char *Mode = g_MySInfo.Mode;

    st_info_T("%02lu-DID=%s,Session=%d,Skt=%d,Mode=%s,Local=%s:%d,Rmt=%s:%d", Repeat, DID, SessionID, Skt, Mode, MyLanIP, LocalPort, RmtIP, RmtPort);


    ////////// 确认连线是否正常读写, check the connection is OK. //////////
    // 发送随机 ACK 值。 1Byte.
    srand((UINT32)time(NULL));
    char SendData = abs(rand() % 124) + 4; // 4~127


    ret = PPCS_Write(SessionID, CH_CMD, (char *)&SendData, sizeof(SendData));
    my_GetCurrentTime(&t1);


    if (0 > ret)
    {
        st_info("\n[%s] !!! error !!! %02lu-PPCS_Write:Session=%d,CH=%d,Mode=%s,SendSize=%d,Data:[%d],ret=%d [%s]\n", t1.Date, SessionID, Repeat, CH_CMD, Mode, sizeof(SendData), SendData, ret, getP2PErrorCodeInfo(ret));
        return SessionID;
    }

    st_debug("\n[%s] !!! OK !!! %02lu-PPCS_Write:ret=%d,Session=%d,CH=%d,Mode=%s,SendSize=%d =>[%d]\n", t1.Date, Repeat, ret, SessionID, CH_CMD, Mode, sizeof(SendData), SendData);

    ////////// Read test Mode from Client. //////////
    // 读取 TestMode CMD: sizeof(int).
    int32_t TestMode = -99;
    UINT32 timeOut_ms = 1000;

    if (3 == bMyMode) { timeOut_ms = 3500; } // PPCS_Check bMode==2: TCP, bMyMode=3: TCP.

    my_GetCurrentTime(&t0);

    for (int i = 0, times = 2; i < times; i++)
    {
        TestMode = -99;
        int ReadSize = sizeof(TestMode);
        my_GetCurrentTime(&t1);
        st_debug_T("%02lu-PPCS_Read(Session=%d,CH=%d,ReadSize=%d,timeOut_ms=%d) ...\n", Repeat, SessionID, CH_CMD, ReadSize, timeOut_ms);
        ret = PPCS_Read(SessionID, CH_CMD, (char *)&TestMode, &ReadSize, timeOut_ms);
        st_debug("%s after read TestMode 0x%X ReadSize %d ret %d\n", __func__, TestMode, ReadSize, ret);
        my_GetCurrentTime(&t2);


        if (0 > ret && 0 == ReadSize)
        {
            if (ERROR_PPCS_TIME_OUT == ret) // 读取超时，读不到数据，但非断线错误，可继续接着读。
            {
                if (times - 1 == i)
                {
                    st_info("\n[%s] -%02lu-PPCS_Read: t:%d ms,Session=%d,CH=%d,Mode=%s,ReadSize=%d,ret=%d, Read test mode timeout!!\n", t2.Date, Repeat, TU_MS(t0, t2), SessionID, CH_CMD, Mode, ReadSize, ret);
                }

                continue;
            }
            else if (ERROR_PPCS_SESSION_CLOSED_TIMEOUT == ret) // 网络差导致断线，需 close 退出。
            {
                st_info("\n[%s] %02lu-PPCS_Read: t:%d ms,Session=%d,CH=%d,Mode=%s,ReadSize=%d,ret=%d, Session Closed TimeOUT!!\n", t2.Date, Repeat, TU_MS(t0, t2), SessionID, CH_CMD, Mode, ReadSize, ret);
            }
            else if (ERROR_PPCS_SESSION_CLOSED_REMOTE == ret) // 对方主动关闭连接，需 close 退出。
            {
                st_info("\n[%s] %02lu-PPCS_Read: t:%d ms,Session=%d,CH=%d,Mode=%s,ReadSize=%d,ret=%d, Remote site call close!!\n", t2.Date, Repeat, TU_MS(t0, t2), SessionID, CH_CMD, Mode, ReadSize, ret);
            }
            else { st_info("\n[%s] %02lu-PPCS_Read: t:%d ms,Session=%d,CH=%d,Mode=%s,ReadSize=%d,ret=%d [%s]\n", t2.Date, Repeat, TU_MS(t0, t2), SessionID, CH_CMD, Mode, ReadSize, ret, getP2PErrorCodeInfo(ret)); }

            break;
        }
        else if (ERROR_PPCS_INVALID_PARAMETER != ret && ERROR_PPCS_INVALID_SESSION_HANDLE != ret && 0 < ReadSize)
        {
            st_debug("[%s] %02lu-PPCS_Read: t:%d ms,ret=%d,Session=%d,CH=%d,Mode=%s,ReadSize=%d =>[%d]\n", t2.Date, Repeat, TU_MS(t1, t2), ret, SessionID, CH_CMD, Mode, ReadSize, TestMode);
            st_info(".\n");
            setbuf(stdout, NULL);
            break;
        }
        else { st_info("\n[%s] %02lu-PPCS_Read: t:%d ms,Session=%d,CH=%d,Mode=%s,ReadSize=%d,ret=%d [%s]\n", t2.Date, Repeat, TU_MS(t1, t2), SessionID, CH_CMD, Mode, ReadSize, ret, getP2PErrorCodeInfo(ret)); }

        break;
    } // for

    ////////// start ReadWrite Test. //////////
    if (0 <= ret && -99 != TestMode)
    {
        gThread_LoginCheck_Exit = 1; // Exit the LoginStatus_Check thread

        st_info("%s start ReadWrite Test. ret %d TestMode 0x%X\n", __func__, ret, TestMode);

        if (0x01 == (TestMode & 0x01)) // TestMode&0x01: 0x01 -> ReadWriteTest; 0x00 -> ConnectionTest
        {
            // check session mode every 100 ms, if connections mode is changed, it will print.
            if (2 == bMyMode) { CreateThread_PPCS_Check(SessionID); }

            // const char *APIInformation = PPCS_GetAPIInformation();
            // st_debug_T("PPCS_GetAPIInformation(%u byte):%s\n", strlen(APIInformation), APIInformation);
            if (0 == bMyMode) { CreateThread_UDP_Ping(RmtIP); } // PPCS_Check bMode==0: LAN/P2P, bMyMode=0: LAN.

            switch ((TestMode >> 1) & 0x03) // Select the test options according to the TestMode
            {
            case 0: ft_Test(SessionID); break;  // File transfer test

            case 1: RW_Test(SessionID, (TestMode >> 3) & 0x0F, (TestMode >> 7) & 0x03, (TestMode >> 9) & 0x03); break; // Bidirectional read write test

            case 2:
            {
                if (!strstr(Mode, "TCP") && 1 != GetSocketType(Skt)) { pkt_Test(SessionID); }
                else { st_info("*****NOTE: the session Mode is TCP or LAN TCP, TCP mode does not Support PPCS_PktSend/PPCS_PktRecv API!!\n"); }

                break;  // PktRecv/PktSend test
            }

            case 3:
                //                picserver_tran_create_task(SessionID, (TestMode >> 3) & 0x0F, (TestMode >> 7) & 0x03, (TestMode >> 9) & 0x03);
                picserver_tran_test_task((void *)SessionID);
                break;

            default: break;
            }
        }
    }

    st_info("%s out ret %d TestMode\n", __func__, ret, TestMode);
#endif
    return SessionID;

}


//static int p2p_recv(int SessionID, int Channel, uint8_t *buff, uint32_t len)
//{
//    int32_t ret = 0;
//    uint32_t read_index = 0;
//    uint32_t ReadSize = len - read_index;
//
//    if(len == 0)
//    {
//        return 0;
//    }
//
//    do
//    {
//        ret = PPCS_Read(SessionID, Channel, (char *)buff + read_index, &ReadSize, 0);
//
//        if(ret < 0 && ERROR_PPCS_TIME_OUT != ret)
//        {
//            return ret;
//        }
//
//        read_index += ReadSize;
//
//        if(ERROR_PPCS_TIME_OUT == ret)
//        {
//            return read_index;
//        }
//    }
//    while(read_index < len);
//
//    return read_index;
//}



static int do_server_job(char *DIDString, char *APILicense, int32_t (*recv_callback)(uint8_t channle, uint8_t *buff, uint32_t len), uint8_t *is_run)
{
    int Repeat = 0;
    st_Time_Info t1, t2;
    //    int skt = -1;
    //    unsigned short i = 0;
    //    unsigned short index = 0;
    //    int size_W = 0;
    //    int size_R = 0;
    //    char readBuf[128] = {};
    //    char ServerIP[36] = {};
    int32_t ret = 0;

    int SessionID = -99;
    const unsigned long Total_Times = Repeat;
    uint8_t *tmp_read_buf = NULL;

    (void)Total_Times;
    Repeat = 0;
    memset(&t2, 0, sizeof(t2));

    ret = PPCS_NetworkDetect(&s_cs2_p2p_networkinfo, 0);
    showNetwork(s_cs2_p2p_networkinfo);

    tmp_read_buf = os_malloc(RECV_TMP_BUFF_SIZE);

    if (!tmp_read_buf)
    {
        BK_LOGE("p2p", "%s alloc err\n", __func__);
        return -1;
    }

    while (*is_run)//Repeat < Total_Times)
    {
        Repeat++;
        //        st_debug_T("----------------------> testCount=%02lu\n", Repeat);

        // 3. Network Detect
#if 1
        ret = PPCS_NetworkDetect(&s_cs2_p2p_networkinfo, 0);

        //        showNetwork(NetInfo);
        if (ret < 0)
        {
            BK_LOGE("p2p", "%s PPCS_NetworkDetect err %d %s\n", __func__, ret, getP2PErrorCodeInfo(ret));
            mSleep(1000);
            continue;
        }
        showNetwork(s_cs2_p2p_networkinfo);

        if (0)//!s_cs2_p2p_networkinfo.bFlagInternet)
        {
            BK_LOGI("p2p", "%s internet not ready, sleep\n", __func__);
            mSleep(1000);
            continue;
        }

#endif

        if (0 == gFlags_WakeupTest) //// 常电设备测试
        {
            if (0 == gThread_LoginCheck_Running)
            {
                CreateThread_LoginStatus_Check();
            }

            //            SessionID = Call_P2P_Listen(DIDString, APILicense, Repeat);
            s_current_sessionid = SessionID = my_p2p_listen(DIDString, APILicense, Repeat, is_run);

            if (0 <= SessionID)
            {
                BK_LOGI("p2p", "%s listen Sid %d\n", __func__, s_current_sessionid);
#if 0
#pragma message "for offical pc cmd debug !!!! remove it !!!!"
                srand((UINT32)time(NULL));
                char SendData = abs(rand() % 124) + 4; // 4~127

                ret = PPCS_Write(SessionID, CH_CMD, (char *)&SendData, sizeof(SendData));
                my_GetCurrentTime(&t1);

                if (0 > ret)
                {
                    BK_LOGE("p2p", "%s write err %d\n", __func__, ret);
                    break;
                }

#endif

                do
                {
                    UINT32 readsize = 0;
                    INT32 ReadSize = 0;
                    uint32_t already_read = 0;

                    for (uint8_t i = 0; i < NUMBER_OF_P2PCHANNEL && is_run; ++i)
                    {
                        do
                        {
                            readsize = 0;
                            ret = PPCS_Check_Buffer(SessionID, i, NULL, &readsize);

                            if (ret < 0)
                            {
                                BK_LOGE("p2p", "%s PPCS_Check_Buffer %d ret err %d %s\n", __func__, i, ret, getP2PErrorCodeInfo(ret));
                                goto READ_ERR;
                            }

                            if(!recv_callback)
                            {
                                already_read = 1;

                                ReadSize = RECV_TMP_BUFF_SIZE;
                                ret = PPCS_Read(SessionID, i, (char *)tmp_read_buf, &ReadSize, 1000);
                                if (ret < 0 && ERROR_PPCS_TIME_OUT != ret)
                                {
                                    BK_LOGE("p2p", "%s PPCS_Read ret err %d %s\n", __func__, ret, getP2PErrorCodeInfo(ret));
                                    goto READ_ERR;
                                }
                            }
                            else if (readsize && recv_callback)
                            {
                                already_read = 1;

                                ReadSize = READ_COUNT_PER;
                                ret = PPCS_Read(SessionID, i, (char *)tmp_read_buf, &ReadSize, 0);

                                if (ret < 0 && ERROR_PPCS_TIME_OUT != ret)
                                {
                                    BK_LOGE("p2p", "%s PPCS_Read ret err %d %s\n", __func__, ret, getP2PErrorCodeInfo(ret));
                                    goto READ_ERR;
                                }
                                else if (ReadSize)
                                {
                                    recv_callback(i, tmp_read_buf, ReadSize);
                                    continue;
                                }
                            }
                        }
                        while (0);
                    }

                    if (!already_read)
                    {
                        usleep(recv_callback ? (NORMAL_MSLEEP * 1000): (200 * 1000));
                    }

                }
                while (*is_run);

READ_ERR:;
                mSleep(300); // 两次 PPCS_Listen 之间需要保持间隔。
                my_GetCurrentTime(&t1);
                PPCS_ForceClose(SessionID);// PPCS_Close(SessionID);// 不能多线程对同一个 SessionID 做 PPCS_Close(SessionID)/PPCS_ForceClose(SessionID) 的动作，否则可能导致崩溃。

                my_GetCurrentTime(&t2);
                st_debug_T("%02lu-PPCS_ForceClose(%d) done!! t:%d ms\n", Repeat, SessionID, TU_MS(t1, t2));
                s_current_sessionid = SessionID = -1;
                continue;
            }
            else if (ERROR_PPCS_MAX_SESSION == SessionID)
            {
                mSleep(1000);    //// 超过最大连线数
            }
            else if (SessionID == -1)
            {
                break;
            }
        }
        else if (gFlags_WakeupTest) // wakeup test. 带休眠唤醒的设备测试
        {

        }
    }


    if (tmp_read_buf)
    {
        os_free(tmp_read_buf);
    }

    if (s_current_sessionid >= 0)
    {
        PPCS_ForceClose(s_current_sessionid);
        s_current_sessionid = -1;
    }

    return 0;
}

static int do_client_job(char *DIDString, char *APILicense, char *InitString, int32_t (*recv_callback)(uint8_t channle, uint8_t *buff, uint32_t len), uint8_t *is_run)
{
    st_Time_Info t1, t2;
    UINT32 APIVersion = 0;
    char VerBuf[32] = {0};
    char bEnableLanSearch = 0x7a;//0x5e;//0x7a;

    int ret = -1;
    int SessionID = -99;
    st_PPCS_Session Sinfo;
    uint8_t *tmp_read_buf = NULL;

    BK_LOGI("p2p", "%s didstring %p\n", __func__, DIDString);
    tmp_read_buf = os_malloc(RECV_TMP_BUFF_SIZE);

    if (!tmp_read_buf)
    {
        BK_LOGE("p2p", "%s alloc err\n", __func__);
        return -1;
    }


    memset(&t1, 0, sizeof(t1));
    memset(&t2, 0, sizeof(t2));

    APIVersion = PPCS_GetAPIVersion();


    memset(VerBuf, 0, sizeof(VerBuf));
    SNPRINTF(VerBuf, sizeof(VerBuf), "%d.%d.%d.%d",
             (APIVersion & 0xFF000000) >> 24,
             (APIVersion & 0x00FF0000) >> 16,
             (APIVersion & 0x0000FF00) >> 8,
             (APIVersion & 0x000000FF) >> 0);

    my_GetCurrentTime(&t1);
    // st_info("[%s] PPCS_Connect(%s, 0x%02X, 0) ...\n", t1.Date, DIDString, bEnableLanSearch);
    // ret = PPCS_Connect(DIDString, bEnableLanSearch, 0);






    char ByServerString[512] = {0};
    char AllowRP2P = 1;

    if (0 <= strncmp(VerBuf, "4.5.3.0", 5)) //// PPCS_ConnectByServer 0x7X_Timeout parameter support by P2P API Version >= 4.5.3
    {
        int connTimeout = 15;

        if (0 <= strncmp(VerBuf, "5.0.1.0", 5)) //// PPCS_ConnectByServer AllowRP2P parameter support by P2P API Version >= 5.0.1
        {
            BK_LOGI("p2p", "%s use timeout %d AllowRP2P %d\n", __func__, connTimeout, AllowRP2P);
            SNPRINTF(ByServerString, sizeof(ByServerString), "{\"InitString\":\"%s\",\"0x7X_Timeout\":%d,\"AllowRP2P\":%d}", InitString, connTimeout, AllowRP2P);
        }
        else if (0)
        {
            BK_LOGI("p2p", "%s use timeout %d\n", __func__, connTimeout);
            SNPRINTF(ByServerString, sizeof(ByServerString), "{\"InitString\":\"%s\",\"0x7X_Timeout\":%d}", InitString, connTimeout);
        }
        else
        {
            BK_LOGI("p2p", "%s use InitString json\n", __func__);
            SNPRINTF(ByServerString, sizeof(ByServerString), "{\"InitString\":\"%s\"}", InitString);
        }
    }
    else
    {
        BK_LOGI("p2p", "%s use none\n", __func__);
        SNPRINTF(ByServerString, sizeof(ByServerString), "%s", InitString);
    }


    while (*is_run)
    {
        ret = PPCS_NetworkDetect(&s_cs2_p2p_networkinfo, 0);
        if (ret < 0)
        {
            BK_LOGE("p2p", "%s PPCS_NetworkDetect err %d\n", __func__, ret);
            mSleep(1000);
            continue;
        }
        else
        {
            showNetwork(s_cs2_p2p_networkinfo);
        }

        if (0)//!s_cs2_p2p_networkinfo.bFlagInternet)
        {
            BK_LOGI("p2p", "%s internet not ready, sleep\n", __func__);
            mSleep(1000);
            continue;
        }

        // SNPRINTF(ByServerString, sizeof(ByServerString), "{\"InitString\":\"%s\",\"0x7X_Timeout\":%d}",InitString, connTimeout);
        //        bEnableLanSearch = 0x5e;//connectiontest mode 6 default, very fast, 0x7a is very slow


        st_info("[%s] PPCS_ConnectByServer(%s,%p,0x%02X,0,ByServerStringlen %d) ...\n", t1.Date, DIDString, DIDString, bEnableLanSearch, strlen(ByServerString));

        ret = PPCS_ConnectByServer(DIDString, bEnableLanSearch, 0, ByServerString);
        my_GetCurrentTime(&t2);

        if (0 > ret)
        {
            st_info("[%s] PPCS_ConnectByServer(%s,0x%02X,0,ByServerStringlen %d) Failed: %d ms, %d [%s]\n", t2.Date, DIDString, bEnableLanSearch, strlen(ByServerString), TU_MS(t1, t2), ret, getP2PErrorCodeInfo(ret));

            // st_info("[%s] PPCS_Connect(%s,0x%02X,0) Failed: %d ms, %d [%s]\n", t2.Date, DIDString, bEnableLanSearch, TU_MS(t1,t2), ret, getP2PErrorCodeInfo(ret));
            if (1)//ERROR_PPCS_TIME_OUT == ret || ERROR_PPCS_FAILED_TO_CONNECT_TCP_RELAY == ret || ERROR_PPCS_DEVICE_NOT_ONLINE == ret)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        else
        {
            s_current_sessionid = SessionID = ret;
            st_info("[%s] PPCS_ConnectByServer(%s,0x%02X,0,ByServerStringlen %d) Success!! t:%d ms, Session=%d\n", t2.Date, DIDString, bEnableLanSearch, strlen(ByServerString), TU_MS(t1, t2), SessionID);
            // st_info("[%s] PPCS_Connect(%s,0x%02X,0) Success!! %d ms, Session=%d\n", t2.Date, DIDString, bEnableLanSearch, TU_MS(t1,t2), SessionID);

            ret = PPCS_Check(SessionID, &Sinfo);

            if (ERROR_PPCS_SUCCESSFUL == ret)
            {
                //                Running = 1;
                char RmtIP[16] = {0};
                SNPRINTF(RmtIP, sizeof(RmtIP), "%s", inet_ntoa(Sinfo.RemoteAddr.sin_addr));
                char MyLanIP[16] = {};
                SNPRINTF(MyLanIP, sizeof(MyLanIP), "%s", inet_ntoa(Sinfo.MyLocalAddr.sin_addr));
                const char *pMode = NULL;

                if (0 == Sinfo.bMode)
                {
                    if (Sinfo.RemoteAddr.sin_addr.s_addr == Sinfo.MyLocalAddr.sin_addr.s_addr || 1 == isLANcmp(MyLanIP, RmtIP))
                    {
                        (1 == GetSocketType(Sinfo.Skt)) ? (pMode = "LAN.") : (pMode = "LAN");
                        //                        CreateThread_UDP_Ping(RmtIP);
                    }
                    else
                    {
                        pMode = "P2P";
                    }
                }
                else if (1 == Sinfo.bMode) { pMode = "RLY"; }
                else if (2 == Sinfo.bMode) { pMode = "TCP"; }
                else if (3 == Sinfo.bMode) { pMode = "RP2P"; } //// Support by P2P API 5.0.8

                st_info("----------- Session Ready: -%s -----------\n", pMode);
                st_info("Socket : %d\n", Sinfo.Skt);
                st_info("Remote Addr : %s:%d\n", RmtIP, ntohs(Sinfo.RemoteAddr.sin_port));
                st_info("My Lan Addr : %s:%d\n", MyLanIP, ntohs(Sinfo.MyLocalAddr.sin_port));
                st_info("My Wan Addr : %s:%d\n", inet_ntoa(Sinfo.MyWanAddr.sin_addr), ntohs(Sinfo.MyWanAddr.sin_port));
                st_info("Connection time : %d second before\n", Sinfo.ConnectTime);
                st_info("DID : %s\n", Sinfo.DID);
                st_info("I am %s\n", (Sinfo.bCorD == 0) ? "Client" : "Device");
                st_info("Connection mode: %s\n", pMode);
                st_info("---------- End of Session info : ----------\n");

                // check session mode every 100 ms, if connections mode is changed, it will print.
                MyGetSInfo(SessionID, &g_MySInfo);

                if (1 == Sinfo.bMode && 1 == AllowRP2P)
                {
                    //CreateThread_PPCS_Check(SessionID);
                } //// AllowRP2P=1 and mode is RLY, then create ppcs check thread to check whether the mode is changed to RP2P

#if 0
#pragma message "for offical pc cmd debug !!!! remove it !!!!"
                srand((UINT32)time(NULL));
                char SendData = abs(rand() % 124) + 4; // 4~127

                ret = PPCS_Write(SessionID, CH_CMD, (char *)&SendData, sizeof(SendData));
                my_GetCurrentTime(&t1);

                if (0 > ret)
                {
                    BK_LOGE("p2p", "%s write err %d\n", __func__, ret);
                    break;
                }

#endif

                do
                {
                    UINT32 readsize = 0;
                    INT32 ReadSize = 0;
                    uint32_t already_read = 0;

                    for (uint8_t i = 0; i < NUMBER_OF_P2PCHANNEL && is_run; ++i)
                    {
                        do
                        {
                            readsize = 0;
                            ret = PPCS_Check_Buffer(SessionID, i, NULL, &readsize);

                            if (ret < 0)
                            {
                                BK_LOGE("p2p", "%s PPCS_Check_Buffer %d ret err %d %s\n", __func__, i, ret, getP2PErrorCodeInfo(ret));
                                goto READ_ERR;
                            }

                            if(!recv_callback)
                            {
                                already_read = 1;

                                ReadSize = RECV_TMP_BUFF_SIZE;
                                ret = PPCS_Read(SessionID, i, (char *)tmp_read_buf, &ReadSize, 1000);
                                if (ret < 0 && ERROR_PPCS_TIME_OUT != ret)
                                {
                                    BK_LOGE("p2p", "%s PPCS_Read ret err %d %s\n", __func__, ret, getP2PErrorCodeInfo(ret));
                                    goto READ_ERR;
                                }
                            }
                            else if (readsize && recv_callback)
                            {
                                already_read = 1;

                                ReadSize = READ_COUNT_PER;
                                ret = PPCS_Read(SessionID, i, (char *)tmp_read_buf, &ReadSize, 0);

                                if (ret < 0 && ERROR_PPCS_TIME_OUT != ret)
                                {
                                    BK_LOGE("p2p", "%s PPCS_Read ret err %d %s\n", __func__, ret, getP2PErrorCodeInfo(ret));
                                    goto READ_ERR;
                                }
                                else if (ReadSize)
                                {
                                    recv_callback(i, tmp_read_buf, ReadSize);
                                    continue;
                                }
                            }
                        }
                        while (0);
                    }

                    if (!already_read)
                    {
                        usleep(recv_callback ? (NORMAL_MSLEEP * 1000): (200 * 1000));
                    }

                }
                while (*is_run);

            }

READ_ERR:;
            mSleep(300); // 两次 PPCS_Listen 之间需要保持间隔。
            my_GetCurrentTime(&t1);
            PPCS_ForceClose(SessionID);// PPCS_Close(SessionID);// 不能多线程对同一个 SessionID 做 PPCS_Close(SessionID)/PPCS_ForceClose(SessionID) 的动作，否则可能导致崩溃。

            my_GetCurrentTime(&t2);
            st_debug_T("PPCS_ForceClose(%d) done!!\n", SessionID, TU_MS(t1, t2));
            s_current_sessionid = SessionID = -1;
            continue;

        }

        break;

    }

    if (tmp_read_buf)
    {
        os_free(tmp_read_buf);
    }

    if (s_current_sessionid >= 0)
    {
        PPCS_ForceClose(s_current_sessionid);
        s_current_sessionid = -1;
    }

    return 0;
}


int cs2_p2p_main(char *pDIDString, char *pAPILicense, char *pInitString, char *pRepeat, char *pWakeupKey,
                 int32_t (*recv_callback)(uint8_t channel, uint8_t *buff, uint32_t len),
                 uint8_t *is_run)
{
    if (s_is_cs2_p2p_task_run)
    {
        BK_LOGE("p2p", "%s is already run !!!!!\n", __func__);
        return -1;
    }

    s_is_cs2_p2p_task_run = 1;

    int ret = 0;
    //    const int argc = 7;
    //    const char *argv[7] = {NULL};
    UINT32 APIVersion = PPCS_GetAPIVersion();
    char VerBuf[64] = {0};
    snprintf(VerBuf, sizeof(VerBuf), "%d.%d.%d.%d",
             (APIVersion & 0xFF000000) >> 24,
             (APIVersion & 0x00FF0000) >> 16,
             (APIVersion & 0x0000FF00) >> 8,
             (APIVersion & 0x000000FF) >> 0);

    if (0 > strncmp(VerBuf, "3.5.0.0", 5))
    {
        st_info("PPCS P2P API Version: %d.%d.%d.%d\n",
                (APIVersion & 0xFF000000) >> 24,
                (APIVersion & 0x00FF0000) >> 16,
                (APIVersion & 0x0000FF00) >> 8,
                (APIVersion & 0x000000FF) >> 0);
        gLoginCheck_GetAPIInformation = 0;
    }
    else
    {
        const char *pVer = PPCS_GetAPIInformation();// PPCS_GetAPIInformation: support by Version >= 3.5.0
        st_info_T("PPCS_GetAPIInformation(%u Byte):\n%s\n", (unsigned)strlen(pVer), pVer);
        gLoginCheck_GetAPIInformation = 1;
    }

#if defined(VSBUILD)
    st_info("%s:%s, %s %s\n", PROGRAM_NAME, TESTER_VERSION, __DATE__, __TIME__);
#else
    st_info("%s:%s, %s\n", PROGRAM_NAME, TESTER_VERSION, BUILD_DATE);
#endif


    char HaveDSK = 0;
    char DID[SIZE_DID] = {0};
    char DSKey[24] = {0};
    char DIDString[128] = {0};
    char APILicense[SIZE_APILICENSE] = {0};
    char InitString[SIZE_INITSTRING] = {0};
    char WakeupKey[SIZE_WAKEUP_KEY] = {0};
    unsigned long Repeat = 1;
    unsigned char NumberOfWakeupServer = 0;
#ifdef SupportDSLK
    const int TCP_Port = 12308; //--> 12308 Port: Use DSLK encode.
#else
    const int TCP_Port = 12306; //--> 12306 Port: No DSLK encode.
    (void) TCP_Port;
#endif

    struct sockaddr_in serveraddr[SERVER_NUM];
    memset(serveraddr, 0, sizeof(serveraddr));

    const char *p = strstr(pDIDString, ":");

    if (p)
    {
        memcpy(DID, pDIDString, (int)(p - pDIDString));
        memcpy(DSKey, p + 1, strlen(p + 1));
        HaveDSK = 1;
    }
    else
    {
        memcpy(DID, pDIDString, strlen(pDIDString));
    }



    memcpy(DIDString, pDIDString, strlen(pDIDString));

    memcpy(APILicense, pAPILicense, strlen(pAPILicense));
    memcpy(InitString, pInitString, strlen(pInitString));

    st_info("DID=%s\n", DID);

    if (HaveDSK) { st_info("DSKey=%s\n", DSKey); }

    st_info("APILicense=%s\n", APILicense);
    st_info("InitString len=%d\n", strlen(InitString));
    st_info("Repeat=%lu\n", Repeat);
    st_info("EchoResponse=%s\n", g_EchoResponse ? "YES" : "NO");
    st_info("RsponseRetryTimes=%d\n", g_ResponseRetryTimes);
    st_info("RsponseRetryInterval_sec=%d\n", g_ResponseRetryInterval_sec);
    st_debug("WakeupTest=%s\n", gFlags_WakeupTest ? "YES" : "NO");

    if (gFlags_WakeupTest)
    {
        st_info("WakeupKey=%s\n", WakeupKey);
        st_info("NumberOfWakeupServer=%d\n", NumberOfWakeupServer);
        WinSockInit();
    }

    my_Mutex_Init(g_DC_LOCK);
#if 0
    CreateThread_UDP_RECV_ECHO();
    // 开启 DirectCommand 命令接收服务端线程
    CreateThread_DirectCommand_Server();
#endif

    st_Time_Info t1, t2;
    memset(&t1, 0, sizeof(t1));
    memset(&t2, 0, sizeof(t2));
    //    char tBuf[SIZE_DATE] = {0};

    //// 2. P2P Initialize
    if (0 <= strncmp(VerBuf, "4.2.0.0", 5)) // PPCS_Initialize JsonString support by Version>=4.2.0
    {
#if 1
        int MaxNumSess = 1; // Max Number Session: 1~256.
        int SessAliveSec = 6; // session timeout close alive: 6~30.


        char InitJsonString[256] = {0};
        snprintf(InitJsonString, sizeof(InitJsonString), "{\"InitString\":\"%s\",\"MaxNumSess\":%d,\"SessAliveSec\":%d}", InitString, MaxNumSess, SessAliveSec);
        // st_debug("InitJsonString=%s\n",InitJsonString);
        my_GetCurrentTime(&t1);
        st_debug("[%s] PPCS_Initialize1(%s) ...\n", t1.Date, InitJsonString);


        // 如果Parameter 不是正确的JSON字串则会被当成InitString[:P2PKey]来处理, 如此以兼容旧版.
        ret = PPCS_Initialize((char *)InitJsonString);
#else
        ret = PPCS_Initialize(InitString); //CS2 表示，如遇到-22错误，可传入InitString
#endif
        my_GetCurrentTime(&t2);
        st_info("[%s] PPCS_Initialize2 len(%d): ret=%d, t:%d ms\n", t2.Date, strlen(InitString), ret, TU_MS(t1, t2));


        if (ERROR_PPCS_SUCCESSFUL != ret && ERROR_PPCS_ALREADY_INITIALIZED != ret)
        {
            st_info("[%s] PPCS_Initialize: ret=%d\n", t2.Date, ret);

            if (gFlags_WakeupTest) { WinSockDeInit(); }

            return 0;
        }
    }
    else
    {
        my_GetCurrentTime(&t1);
        st_debug("[%s] PPCS_Initialize3(%s) ...\n", t1.Date, InitString);
        ret = PPCS_Initialize((char *)InitString);
        my_GetCurrentTime(&t2);
        st_info("[%s] PPCS_Initialize4(%s): ret=%d, t:%d ms\n", t2.Date, InitString, ret, TU_MS(t1, t2));

        if (ERROR_PPCS_SUCCESSFUL != ret && ERROR_PPCS_ALREADY_INITIALIZED != ret)
        {
            st_info("[%s] PPCS_Initialize: ret=%d\n", t2.Date, ret);

            if (gFlags_WakeupTest) { WinSockDeInit(); }

            return 0;
        }
    }

#if CONFIG_CS2_P2P_SERVER && !CONFIG_CS2_P2P_CLIENT
    ret = do_server_job(DIDString, APILicense, recv_callback, is_run);
#elif !CONFIG_CS2_P2P_SERVER && CONFIG_CS2_P2P_CLIENT
    ret = do_client_job(DIDString, APILicense, InitString, recv_callback, is_run);
#else
#error "CONFIG_CS2_P2P_SERVER or CONFIG_CS2_P2P_CLIENT"
#endif

    ret = PPCS_DeInitialize();

    if (ERROR_PPCS_SUCCESSFUL != ret)
    {
        BK_LOGE("p2p", "%s PPCS_DeInitialize: ret=%d %s\n", __func__, ret, getP2PErrorCodeInfo(ret));
    }

    os_memset(&s_cs2_p2p_networkinfo, 0, sizeof(s_cs2_p2p_networkinfo));
    s_is_cs2_p2p_task_run = 0;
    return ret;
}

typedef struct
{
    beken_thread_t tid;

    int32_t (*recv_callback)(uint8_t channel, uint8_t *buff, uint32_t len);
    int8_t (*before_start)(void);
    int8_t (*after_end)(void);

    char *did_dskey;
    char *apilicense_crckey;
    char *initstring_p2pkey;
    char *prepeat;
    char *pwakeupkey;

} middle_layer_task_arg_t;



static void cs2_p2p_fake_task(void *arg)
{
    middle_layer_task_arg_t *targ = (typeof(targ))arg;

    if (targ)
    {
        s_cs2_p2p_main_loop_control = 1;

        if (targ->before_start)
        {
            targ->before_start();
        }

        cs2_p2p_main(
            targ->did_dskey,
            targ->apilicense_crckey,
            targ->initstring_p2pkey,
            targ->prepeat, targ->pwakeupkey,
            targ->recv_callback, &s_cs2_p2p_main_loop_control
        );

        if (targ->after_end)
        {
            targ->after_end();
        }
    }

    if (targ)
    {
        if (targ->did_dskey)
        {
            os_free(targ->did_dskey);
        }

        if (targ->apilicense_crckey)
        {
            os_free(targ->apilicense_crckey);
        }

        if (targ->initstring_p2pkey)
        {
            os_free(targ->initstring_p2pkey);
        }

        if (targ->prepeat)
        {
            os_free(targ->prepeat);
        }

        if (targ->pwakeupkey)
        {
            os_free(targ->pwakeupkey);
        }

        os_free(targ);
    }

    rtos_delete_thread(NULL);
}



bk_err_t cs2_p2p_main_task_create_ext(char *did_dskey, char *apilicense_crckey, char *initstring_p2pkey,
                                      int32_t (*recv_callback)(uint8_t channel, uint8_t *buff, uint32_t len),
                                      int8_t (*before_start)(void),
                                      int8_t (*after_end)(void))
{
    bk_err_t ret = 0;
    beken_thread_t tid;

    middle_layer_task_arg_t *targ;


#if CONFIG_CS2_P2P_SERVER && !CONFIG_CS2_P2P_CLIENT
    os_printf("%s is Server\n", __func__);
#elif !CONFIG_CS2_P2P_SERVER && CONFIG_CS2_P2P_CLIENT
    os_printf("%s is Client\n", __func__);
#else
#error "CONFIG_CS2_P2P_SERVER or CONFIG_CS2_P2P_CLIENT"
#endif

    if (s_cs2_p2p_main_tid)
    {
        os_printf("%s already create\n", __func__);
        return -1;
    }

    targ = (typeof(targ))os_malloc(sizeof(*targ));

    if (!targ)
    {
        os_printf("%s malloc fail\n");
        goto CREATE_FAIL;
    }

    memset(targ, 0, sizeof(*targ));


    targ->did_dskey = os_malloc(strlen(did_dskey) + 1);

    if (!targ->did_dskey)
    {
        os_printf("%s malloc fail %d\n", __func__, ret);
        goto CREATE_FAIL;
    }

    memset(targ->did_dskey, 0, strlen(did_dskey) + 1);
    strcpy(targ->did_dskey, did_dskey);

    targ->apilicense_crckey = os_malloc(strlen(apilicense_crckey) + 1);

    if (!targ->apilicense_crckey)
    {
        os_printf("%s malloc fail %d\n", __func__, ret);
        goto CREATE_FAIL;
    }

    memset(targ->apilicense_crckey, 0, strlen(apilicense_crckey) + 1);
    strcpy(targ->apilicense_crckey, apilicense_crckey);

    targ->initstring_p2pkey = os_malloc(strlen(initstring_p2pkey) + 1);

    if (!targ->initstring_p2pkey)
    {
        os_printf("%s malloc fail %d\n", __func__, ret);
        goto CREATE_FAIL;
    }

    memset(targ->initstring_p2pkey, 0, strlen(initstring_p2pkey) + 1);
    strcpy(targ->initstring_p2pkey, initstring_p2pkey);


    targ->recv_callback = recv_callback;
    targ->before_start = before_start;
    targ->after_end = after_end;

    ret = rtos_create_thread(&tid,
                             4,
                             "cs2_p2p_main",
                             cs2_p2p_fake_task,
                             1024 * 12,
                             targ);

    if (ret != 0)
    {
        os_printf("%s create task fail %d\n", __func__, ret);
        goto CREATE_FAIL;
    }

    targ->tid = s_cs2_p2p_main_tid = tid;

    return 0;

CREATE_FAIL:;

    if (targ)
    {
        if (targ->did_dskey)
        {
            os_free(targ->did_dskey);
        }

        if (targ->apilicense_crckey)
        {
            os_free(targ->apilicense_crckey);
        }

        if (targ->initstring_p2pkey)
        {
            os_free(targ->initstring_p2pkey);
        }

        os_free(targ);
    }

    return -1;
}

bk_err_t cs2_p2p_main_task_create(char *did_dskey, char *apilicense_crckey, char *initstring_p2pkey,
                                  int32_t (*recv_callback)(uint8_t channel, uint8_t *buff, uint32_t len))
{
    return cs2_p2p_main_task_create_ext(did_dskey, apilicense_crckey, initstring_p2pkey, recv_callback, NULL, NULL);
}

int32_t cs2_p2p_main_task_destory(void)
{
    s_cs2_p2p_main_loop_control = 0;
    PPCS_Listen_Break();
    PPCS_Connect_Break();

    rtos_thread_join(&s_cs2_p2p_main_tid);
    s_cs2_p2p_main_tid = 0;

    return 0;
}

int32_t cs2_p2p_send_raw(uint8_t channel, uint8_t *buff, uint32_t size)
{
    if (channel >= NUMBER_OF_P2PCHANNEL)
    {
        os_printf("%s channel %d is large than NUMBER_OF_P2PCHANNEL %d!!\n", __func__, channel, NUMBER_OF_P2PCHANNEL);
        return -1;
    }

    if (s_current_sessionid >= 0)
    {
        //        os_printf("%s channel %d len %d\n", __func__, channel, size);
        return picserver_send_raw(s_current_sessionid, channel, buff, size);
    }

    return -1;
}

int32_t cs2_p2p_is_network_ready(void)
{
    return s_cs2_p2p_networkinfo.bFlagInternet == 1;
}

int32_t cs2_p2p_is_connected(void)
{
    return s_current_sessionid >= 0;
}

int32_t cs2_p2p_ternimal(void)
{
    s_cs2_p2p_main_loop_control = 0;
    return PPCS_Listen_Break();
}
int32_t cs2_p2p_set_log_level(uint8_t level)
{
    s_log_level = level;
    return 0;
}
