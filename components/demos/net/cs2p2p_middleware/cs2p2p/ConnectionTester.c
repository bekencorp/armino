#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>  // uintptr_t
#include "cs2_p2p_main.h"

#ifdef LINUX
    #include <unistd.h>
    #include <fcntl.h>
    #include <pthread.h>
    #include <sys/types.h>
    #include <sys/time.h>   // gettimeofday
    #include <signal.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <net/if.h>
    #include <sched.h>
    #include <stdarg.h>     // va_start ...
    #include <dirent.h>
    #include <arpa/inet.h>  // INADDR_ANY, htonl, htons, ...
#endif
#if defined(WIN32DLL) || defined(WINDOWS)
    // #pragma comment(lib,"ws2_32.lib")
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #include <direct.h>

#else
    #include <lwip/sockets.h>
#endif

#include "PPCS_API.h"

#define     TESTER_VERSION  "0.0.2.7"

#define ERROR_NotLogin              -1
#define ERROR_InvalidParameter      -2
#define ERROR_SocketCreateFailed    -3
#define ERROR_SendToFailed          -4
#define ERROR_RecvFromFailed        -5
#define ERROR_UnKnown               -99

#define SERVER_NUM          3   // WakeUp Server Number
#define SIZE_DID            64
#define SIZE_INITSTRING     256
#define SIZE_WAKEUP_KEY     17

#if defined(WIN32DLL) || defined(WINDOWS)
    typedef     DWORD                   my_threadid;
    typedef     HANDLE                  my_Thread_mutex_t;
    #define     my_Mutex_Init(mutex)    (mutex = CreateMutex(NULL, false, NULL))
    #define     my_Mutex_Lock(mutex)    WaitForSingleObject(mutex, INFINITE)
    #define     my_Mutex_UnLock(mutex)  ReleaseMutex(mutex)
    #define     my_Mutex_Close(mutex)   CloseHandle(mutex)
    #define     my_SocketClose(skt)     closesocket(skt)
    #define     my_Thread_exit(a)       return(a)
    #define     SNPRINTF                _snprintf
    #define     LocalTime_R(a, b)       localtime_s(b, a)
    #define     GmTime_R(a, b)          gmtime_s(b, a)
    #define     SHUT_RDWR               SD_BOTH
    #define     MKDIR(Directory, mode)      mkdir(Directory)
#elif defined(LINUX)
    typedef     pthread_t               my_threadid;
    typedef     pthread_mutex_t         my_Thread_mutex_t;
    #define     my_Mutex_Init(mutex)    pthread_mutex_init(&mutex, NULL)
    #define     my_Mutex_Lock(mutex)    pthread_mutex_lock(&mutex)
    #define     my_Mutex_UnLock(mutex)  pthread_mutex_unlock(&mutex)
    #define     my_Mutex_Close(mutex)   pthread_mutex_destroy(&mutex)
    #define     my_SocketClose(skt)     close(skt)
    #define     my_Thread_exit(a)       pthread_exit(a)
    #define     SNPRINTF                snprintf
    #define     LocalTime_R(a, b)       localtime_r(a, b)
    #define     GmTime_R(a, b)          gmtime_r(a, b)
    #define     MKDIR(Directory, mode)      mkdir(Directory, mode)
#else
    #include "osi_system.h"
    #include "osi_cs2_not_impl.h"
    #define LINUX 1
#endif

typedef uintptr_t UINTp;

#define CH_CMD              0
#define CH_DATA             1

// show info 开关 -> 终端打印调试信息
static int g_ST_INFO_OPEN = 0;
// debug log 开关 -> 输出到本地log文件
static int g_DEBUG_LOG_FILE_OPEN = 0;
#define SIZE_LOG_NAME   256
static char gLogFileName[128] = {};
static char gLogPath[256] = {};

static int gFlags_WinSktInitOK = 0;

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
    char bCorD;      // I am Client or Device, 0: Client, 1: Device
    char bMyMode;  // my define mode by PPCS_Check bMode(0:P2P(Including: LAN TCP/UDP), 1:RLY, 2:TCP); Mydefine: 0:LAN, 1:P2P, 2:RLY, 3:TCP, 4:RP2P.
    char Mode[12];   // Connection Mode: LAN/P2P/RLY/TCP.
    char Reserved[2];
} st_MySess_Info;

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
#if  defined(WINDOWS) || defined(WIN32DLL)
    SYSTEMTIME tmv = {0};
    GetLocalTime(&tmv);
    pt->Year = tmv.wYear;
    pt->Mon = tmv.wMonth;
    pt->Day = tmv.wDay;
    pt->Week = tmv.wDayOfWeek;
    pt->Hour = tmv.wHour;
    pt->Min = tmv.wMinute;
    pt->Sec = tmv.wSecond;
    pt->mSec = tmv.wMilliseconds;
    pt->Tick_Sec = time(NULL);
    pt->Tick_mSec = GetTickCount();
    memset(pt->Date, 0, sizeof(pt->Date));
    SNPRINTF(pt->Date, sizeof(pt->Date), "%04d-%02d-%02d %02d:%02d:%02d.%03d", pt->Year, pt->Mon, pt->Day, pt->Hour, pt->Min, pt->Sec, pt->mSec);
#elif defined(LINUX)
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
        SNPRINTF(pt->Date, sizeof(pt->Date), "%04d-%02d-%02d %02d:%02d:%02d.%03d", pt->Year, pt->Mon, pt->Day, pt->Hour, pt->Min, pt->Sec, pt->mSec);
    }

#endif
}

// GetTime_ms: will call gettimeofday to get current time,format to [YYYY-MM-DD hh:mm:ss.xxx]
static void GetTime_ms(st_Time_Info *t, char *tBuf, unsigned int tBufSize)
{
    if (!t || !tBuf || 0 == tBufSize) { return ; }

    my_GetCurrentTime(t);
    SNPRINTF(tBuf, tBufSize, "[%s]", t->Date);
}

#if 0
static void st_info(const char *format, ...)
{
    // if (1 == g_ST_INFO_OPEN)
    {
        va_list ap;
        va_start(ap, format);
        vfprintf(stdout, format, ap);
        va_end(ap);
    }

    if (1 == g_DEBUG_LOG_FILE_OPEN)
    {
        st_Time_Info t;
        my_GetCurrentTime(&t);
        char LogPath[SIZE_LOG_NAME] = {};
        SNPRINTF(LogPath, sizeof(LogPath), "%s/%04d%02d%02d.%s", gLogPath, t.Year, t.Mon, t.Day, gLogFileName);
        FILE *pFile = fopen(LogPath, "a");

        if (pFile)
        {
            va_list ap;
            va_start(ap, format);
            vfprintf(pFile, format, ap);
            va_end(ap);
            fclose(pFile);
        }
    }
}

static void st_debug(const char *format, ...)
{
    if (1 == g_ST_INFO_OPEN)
    {
        va_list ap;
        va_start(ap, format);
        vfprintf(stdout, format, ap);
        va_end(ap);
    }

    if (1 == g_DEBUG_LOG_FILE_OPEN)
    {
        st_Time_Info t;
        my_GetCurrentTime(&t);
        char LogPath[SIZE_LOG_NAME] = {};
        SNPRINTF(LogPath, sizeof(LogPath), "%s/%04d%02d%02d.%s", gLogPath, t.Year, t.Mon, t.Day, gLogFileName);
        FILE *pFile = fopen(LogPath, "a");

        if (!pFile) { return ; }

        va_list ap;
        va_start(ap, format);
        vfprintf(pFile, format, ap);
        va_end(ap);
        fclose(pFile);
    }
}


static void st_info_T(const char *format, ...)
{
    // if (1 == g_ST_INFO_OPEN)
    {
        st_Time_Info t;
        my_GetCurrentTime(&t);

        va_list ap;
        fprintf(stdout, "[%s] ", t.Date);
        va_start(ap, format);
        vfprintf(stdout, format, ap);
        va_end(ap);
    }


    if (1 == g_DEBUG_LOG_FILE_OPEN)
    {
        st_Time_Info t;
        my_GetCurrentTime(&t);

        char LogPath[SIZE_LOG_NAME] = {};
        SNPRINTF(LogPath, sizeof(LogPath), "%s/%04d%02d%02d.%s", gLogPath, t.Year, t.Mon, t.Day, gLogFileName);
        FILE *pFile = fopen(LogPath, "a");

        if (pFile)
        {
            fprintf(pFile, "[%s] ", t.Date);
            va_list ap;
            va_start(ap, format);
            vfprintf(pFile, format, ap);
            va_end(ap);
            fclose(pFile);
        }
    }
}

static void st_debug_T(const char *format, ...)
{
    if (1 == g_ST_INFO_OPEN)
    {
        st_Time_Info t;
        my_GetCurrentTime(&t);

        fprintf(stdout, "[%s] ", t.Date);
        va_list ap;
        va_start(ap, format);
        vfprintf(stdout, format, ap);
        va_end(ap);
    }

    if (1 == g_DEBUG_LOG_FILE_OPEN)
    {
        st_Time_Info t;
        my_GetCurrentTime(&t);

        char LogPath[SIZE_LOG_NAME] = {};
        SNPRINTF(LogPath, sizeof(LogPath), "%s/%04d%02d%02d.%s", gLogPath, t.Year, t.Mon, t.Day, gLogFileName);
        FILE *pFile = fopen(LogPath, "a");

        if (pFile)
        {
            va_list ap;
            va_start(ap, format);
            fprintf(pFile, "[%s] ", t.Date);
            vfprintf(pFile, format, ap);
            fclose(pFile);
            va_end(ap);
        }
    }
}
#endif
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
#if defined(WIN32DLL) || defined(WINDOWS)

    if (0 == gFlags_WinSktInitOK)
    {
        WSADATA wsaData;
        WORD socketVersion = MAKEWORD(2, 2);

        if (0 != WSAStartup(socketVersion, &wsaData))
        {
            error("WSAStartup Init failed");
        }
        else { gFlags_WinSktInitOK = 1; }
    }

#endif
    return 0;
}

static void WinSockDeInit()
{
#if defined(WINDOWS) || defined(WIN32DLL)

    if (1 == gFlags_WinSktInitOK)
    {
        WSACleanup();
        gFlags_WinSktInitOK = 0;
    }

#endif
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

    default: return "Unknown, something is wrong";
    }
} // getP2PErrorCodeInfo

static const char *getConnectErrorInfo(int ret)
{
    switch (ret)
    {
    case ERROR_PPCS_SUCCESSFUL: return "Success";

    case ERROR_PPCS_NOT_INITIALIZED: return "API didn't initialized";

    case ERROR_PPCS_ALREADY_INITIALIZED: return "API alread initialized";

    case ERROR_PPCS_TIME_OUT: return "Connection time out, probably the device is off line now";

    case ERROR_PPCS_INVALID_ID: return "Invalid Device ID";

    case ERROR_PPCS_INVALID_PARAMETER: return "Invalid Parameter";

    case ERROR_PPCS_DEVICE_NOT_ONLINE: return "Device is not on line now, nor login in the past 5 minutes";

    case ERROR_PPCS_FAIL_TO_RESOLVE_NAME: return "Fail to resolve the domain name";

    case ERROR_PPCS_INVALID_PREFIX: return "Prefix of Device ID is not accepted by Server";

    case ERROR_PPCS_NO_RELAY_SERVER_AVAILABLE: return "No relay server can provide service";

    case ERROR_PPCS_SESSION_CLOSED_REMOTE: return "Session close remote.";

    case ERROR_PPCS_SESSION_CLOSED_TIMEOUT: return "Session close timeout.";

    case ERROR_PPCS_USER_LISTEN_BREAK: return "Listen break is called !!";

    case ERROR_PPCS_MAX_SESSION: return "Exceed max Session";

    case ERROR_PPCS_UDP_PORT_BIND_FAILED: return "The specified UDP port can not be binded";

    case ERROR_PPCS_USER_CONNECT_BREAK: return "connect break is called";

    case ERROR_PPCS_INVALID_APILICENSE: return "API License code is not correct !!";

    case ERROR_PPCS_FAIL_TO_CREATE_THREAD: return "Fail to Create Thread !!";

    default: return getP2PErrorCodeInfo(ret);
    } // switch
}

static const char *getWakeupQueryError(int ret)
{
    switch (ret)
    {
    case -1: return "ERROR_NotLogin";

    case -2: return "ERROR_InvalidParameter";

    case -3: return "ERROR_SocketCreateFailed";

    case -4: return "ERROR_SendToFailed";

    case -5: return "ERROR_RecvFromFailed";

    case -99: return "ERROR_UnKnown";

    default: return "ERROR_UnKnown";
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

static int MyGetSInfo(int SessionID, st_MySess_Info *MySInfo)
{
    memset(MySInfo, 0, sizeof(st_MySess_Info));
    st_PPCS_Session Sinfo;
    int ret = PPCS_Check(SessionID, &Sinfo);

    if (ERROR_PPCS_SUCCESSFUL == ret)
    {
        MySInfo->Skt = Sinfo.Skt;
        // Remote addr
        SNPRINTF(MySInfo->RemoteIP, sizeof(MySInfo->RemoteIP), "%s", inet_ntoa(Sinfo.RemoteAddr.sin_addr));
        MySInfo->RemotePort = ntohs(Sinfo.RemoteAddr.sin_port);
        // Lan addr
        SNPRINTF(MySInfo->MyLocalIP, sizeof(MySInfo->MyLocalIP), "%s", inet_ntoa(Sinfo.MyLocalAddr.sin_addr));
        MySInfo->MyLocalPort = ntohs(Sinfo.MyLocalAddr.sin_port);
        // Wan addr
        SNPRINTF(MySInfo->MyWanIP, sizeof(MySInfo->MyWanIP), "%s", inet_ntoa(Sinfo.MyWanAddr.sin_addr));
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

//// ret=0 OK, ret=-1: Invalid Parameter, ret=-2: No such Item
static int GetStringItem(const char *SrcStr, const char *ItemName, const char Seperator, char *RetString, const int MaxSize)
{
    if (!SrcStr || !ItemName || !RetString || 0 == MaxSize) { return -1; }

    const char *pFand = SrcStr;

    while (1)
    {
        pFand = strstr(pFand, ItemName);

        if (!pFand) { return -2; }

        pFand += strlen(ItemName);

        if ('=' != *pFand) { continue; }
        else { break; }
    }

    pFand += 1;
    int i = 0;

    while (1)
    {
        if (Seperator == *(pFand + i) || '\0' == *(pFand + i) || i >= (MaxSize - 1)) { break; }
        else { *(RetString + i) = *(pFand + i); }

        i++;
    }

    *(RetString + i) = '\0';
    return 0;
}

static int getMinNumFromLastLogin(const int pLastLogin[], const unsigned char Length)
{
    if (!pLastLogin)
    {
        //printf("getMinNumFromLastLogin - Invalid parameter!!\n");
        return ERROR_UnKnown;
    }

    int min = pLastLogin[0];

    for (unsigned char i = 0; i < Length; i++)
    {
        //printf("pLastLogin[%d]=%d\n", i, pLastLogin[i]);
        if (0 > pLastLogin[i])      // LastLogin<0: -99 or -1. min: -1,-99 or >= 0
        {
            if (min < pLastLogin[i])// min<0:-1,-99
            {
                min = pLastLogin[i];// min:-1
            }
        }
        else if (0 > min || min > pLastLogin[i])// LastLogin>=0, min: unknown
        {
            min = pLastLogin[i];    // min>=0
        }
    }

    return min;
}

static int Wakeup_Query(const struct sockaddr_in *address,
                        const unsigned char NumberOfServer,
                        const char *DID,
                        const char *WakeupKey,
                        const int tryCount,
                        const unsigned int timeout_ms,
                        int *LastLogin1,
                        int *LastLogin2,
                        int *LastLogin3)
{
    if (!address)
    {
        st_debug("Invalid address!!\n");
        return ERROR_InvalidParameter;
    }

    if (0 == NumberOfServer)
    {
        st_debug("Invalid NumberOfServer!! NumberOfServer=0\n");
        return ERROR_InvalidParameter;
    }

    if (!WakeupKey || 0 == strlen(WakeupKey))
    {
        st_debug("Invalid WakeupKey!!\n");
        return ERROR_InvalidParameter;
    }

    if (!DID || 12 > strlen(DID))
    {
        st_debug("Invalid DID!!\n");
        return ERROR_InvalidParameter;
    }

    struct sockaddr_in serverAddr[SERVER_NUM];

    struct sockaddr_in fromAddr;

    memset(serverAddr, 0, sizeof(serverAddr));

    memset(&fromAddr, 0, sizeof(fromAddr));

    unsigned int sin_len = sizeof(struct sockaddr_in);

    char dest[20];

    memset(dest, 0, sizeof(dest));

    for (int i = 0; i < NumberOfServer; i++)
    {
        memcpy((char *)&serverAddr[i], (char *)&address[i], sin_len);
        //st_debug("%d-%s:%d\n", i, inet_ntop(serverAddr[i].sin_family, (char *)&serverAddr[i].sin_addr.s_addr, dest, sizeof(dest)), ntohs(serverAddr[i].sin_port), ntohs(serverAddr[i].sin_port));
    }

    // st_debug("\n");

    char cmd[32] = {};
    memset(cmd, 0, sizeof(cmd));
    SNPRINTF(cmd, sizeof(cmd), "DID=%s&", DID);
    st_debug("Wakeup_Query Cmd: %s, size=%u Byte\n", cmd, (unsigned)strlen(cmd));

    char CMD[64] = {};
    memset(CMD, 0, sizeof(CMD));

    if (0 > iPN_StringEnc(WakeupKey, cmd, CMD, sizeof(CMD)))
    {
        st_info("*** WakeUp Query Cmd StringEncode failed!!\n");
        return ERROR_UnKnown;
    }

    st_debug("[%s] %u Byte -> [%s] %u Byte\n", cmd, (unsigned)strlen(cmd), CMD, (unsigned)strlen(CMD));

    int flag[SERVER_NUM];
    memset(&flag, 0, sizeof(flag));
    int LastLogin[SERVER_NUM];

    for (int i = 0; i < SERVER_NUM; i++)
    {
        LastLogin[i] = ERROR_UnKnown;
    }

    int count = 0;
    int counter = 0;
    int timeOutCount = 0;
    int size;
    fd_set readfds;
    struct timeval timeout;
    char recvBuf[256];
    char Message[128];

    // 创建 UDP socket
    int skt = socket(AF_INET, SOCK_DGRAM, 0);

    if (0 > skt)
    {
        st_debug("create UDP socket failed\n");
        return ERROR_SocketCreateFailed;
    }

    while (tryCount > timeOutCount)
    {
        count = 0;
        counter = 0;

        for (int i = 0; i < NumberOfServer; i++)
        {
            memset(dest, 0, sizeof(dest));

            //inet_ntop(serverAddr[i].sin_family, (char *)&serverAddr[i].sin_addr.s_addr, dest, sizeof(dest));
            //if (0 == strcmp(dest, "0.0.0.0"))
            if (0 == strcmp(inet_ntoa(serverAddr[i].sin_addr), "0.0.0.0"))
            {
                counter++;
                continue;
            }

            if (0 == flag[i])
            {
                size = sendto(skt, CMD, strlen(CMD), 0, (struct sockaddr *)&serverAddr[i], sizeof(struct sockaddr_in));

                if (0 > size)
                {
                    st_debug_T("Sendto Error!\n");
                    close(skt);
                    return ERROR_SendToFailed;
                }

                st_debug_T("%d-Send CMD(%u Byte) to Wakeup_Server-%d %s:%d\n",
                           i,
                           (unsigned)strlen(CMD),
                           i,
                           inet_ntoa(serverAddr[i].sin_addr), ntohs(serverAddr[i].sin_port));
            }
            else if (1 == flag[i])
            {
                count++;
                // st_debug_T("%d-Done LastLogin=%d\n", i, LastLogin[i]);
            }
        } // for

        if (NumberOfServer == counter || NumberOfServer == count)
        {
            break;
        }

        FD_ZERO(&readfds);
        FD_SET(skt, &readfds);
        timeout.tv_sec = (timeout_ms - (timeout_ms % 1000)) / 1000;
        timeout.tv_usec = (timeout_ms % 1000) * 1000;

        int result = select(skt + 1, &readfds, (fd_set *)NULL, (fd_set *)NULL, &timeout);

        switch (result)
        {
        case 0: st_debug_T("-------------(timeout, Counter=%d)", timeOutCount++); break;

        case -1:
        {
            st_debug_T("select error!\n");
            close(skt);
            return getMinNumFromLastLogin(LastLogin, SERVER_NUM);
        }

        default:
            if (FD_ISSET(skt, &readfds))
            {
                memset(recvBuf, 0, sizeof(recvBuf));
                memset(Message, 0, sizeof(Message));

                size = recvfrom(skt, recvBuf, sizeof(recvBuf), 0, (struct sockaddr *)&fromAddr, (socklen_t *)&sin_len);

                if (0 > size)
                {
                    st_debug_T("Wakeup_Query-RecvFrom error");
                    close(skt);
                    return ERROR_RecvFromFailed;
                }

                recvBuf[size] = '\0';
                st_debug_T("recv data: %s, Size: %u Byte\n", recvBuf, (unsigned)strlen(recvBuf));

                if (0 > iPN_StringDnc(WakeupKey, recvBuf, Message, sizeof(Message)))
                {
                    st_debug_T("Wakeup_Query-iPN_StringDnc failed.\n");
                    close(skt);
                    return getMinNumFromLastLogin(LastLogin, SERVER_NUM);
                }

                counter = 0;

                for (int i = 0; i < NumberOfServer; i++)
                {
                    if (fromAddr.sin_addr.s_addr == serverAddr[i].sin_addr.s_addr && fromAddr.sin_port == serverAddr[i].sin_port)
                    {
                        //st_debug("%d-Recv %s, Data: %s\n", i, inet_ntop(fromAddr.sin_family, (char *)&fromAddr.sin_addr.s_addr, dest, sizeof(dest)), Message);
                        st_debug_T("%d-Recv %s, Data: %s\n", i, inet_ntoa(fromAddr.sin_addr), Message);

                        int lastLogin = ERROR_UnKnown;
                        char buf[8];
                        memset(buf, 0, sizeof(buf));

                        if (0 > GetStringItem(Message, "LastLogin", '&', buf, sizeof(buf)))
                        {
                            st_debug("can not get LastLogin Item!\n");
                        }
                        else if (0 != strlen(buf))
                        {
                            lastLogin = atoi(buf);
                        }

                        flag[i] = 1;
                        LastLogin[i] = lastLogin;
                        st_debug_T("%d-Done LastLogin=%d, from %s:%d\n", i, LastLogin[i], inet_ntoa(serverAddr[i].sin_addr), ntohs(serverAddr[i].sin_port));
                    }

                    if (1 == flag[i])
                    {
                        counter++;
                    }
                } // for
            }
            else { st_debug_T("FD_ISSET error, readfds no data!!\n"); }
        } // switch

        if (NumberOfServer == counter) { break; } // break while

        // st_debug("\n");
    } // while

    close(skt);

    int MinLastLogin = getMinNumFromLastLogin(LastLogin, SERVER_NUM);
    st_debug("***** LastLogin[");

    for (int i = 0; i < SERVER_NUM; i++)
    {
        st_debug("%d%c", LastLogin[i], (i < SERVER_NUM - 1) ? ',' : ']');
    }

    st_debug(", Result: LastLogin=%d %s.\n", MinLastLogin, (0 <= MinLastLogin) ? "sec before" : ((-1 == MinLastLogin) ? "NotLogin" : "UnKnown"));

    //st_debug("\n** LastLogin[%d, %d, %d], Result: LastLogin = %d %s\n", LastLogin[0],LastLogin[1], LastLogin[2], MinLastLogin, (0<=MinLastLogin)? "sec before":((-1==MinLastLogin)?"NotLogin":"UnKnown"));
    st_debug("***** NOTE: Value: [>=0]: LastLogin Second, [-1]: NotLogin, [%d]: LastLogin UnKnown.\n\n", ERROR_UnKnown);

    if (NULL != LastLogin1) { *LastLogin1 = LastLogin[0]; }

    if (NULL != LastLogin2) { *LastLogin2 = LastLogin[1]; }

    if (NULL != LastLogin3) { *LastLogin3 = LastLogin[2]; }

    return MinLastLogin;
}

static void showArg(CHAR **argv)
{
    printf("Usage: %s Mode DID InitString [Repeat] [IntervalSec] [WakeupKey] [IP1] [IP2] [IP3]\n", argv[0]);
    printf("   Or: %s Mode DID InitString:P2PKey [Repeat] [IntervalSec] [WakeupKey] [IP1] [IP2] [IP3]\n", argv[0]);
    printf("With P2P DSK:\n        %s Mode DID:DSKey InitString [Repeat] [IntervalSec] [WakeupKey] [IP1] [IP2] [IP3]\n", argv[0]);
    printf("    Or: %s Mode DID:DSKey InitString:P2PKey [Repeat] [IntervalSec] [WakeupKey] [IP1] [IP2] [IP3]\n\n", argv[0]);

    printf("\tMode: 0->No local LAN OFF, P2P then Relay for remote. bEnableLanSearch=0x%02X\n", 0);
    printf("\t      1->Local LAN search, P2P then Relay for remote. bEnableLanSearch=0x%02X\n", 1);
    printf("\t      2->No local LAN search, force Relay for remote. bEnableLanSearch=0x%02X\n", 30);
    printf("\t      3->Local LAN search, force Relay for remote. bEnableLanSearch=0x%02X\n", 31);
    printf("\t      4->Do Network Detect Only.\n");
    printf("\t      5->Dedicated connection mode for wakeup devices. bEnableLanSearch=0x7A:  0x7E(LAN + P2P + RLY(with 240ms delay)) + TCP Relay (with 1.5 sec delay)\n");
    printf("\t      6->No local LAN search, force Server Relay for remote. bEnableLanSearch=0x%02X\n", 94);
    printf("\t      7->Fast connect mode without delay, bEnableLanSearch=0x7B:  0x7D(LAN + P2P + RLY) + TCP Relay (no delay)\n");
    printf("\t      8->Local LAN search and force TCP Relay for remote. bEnableLanSearch=0x7C:  LAN + TCP Relay (no delay)\n");
    printf("\t      9->PPCS_ConnectByServer connection test with wakeupinfo, API Version must be >=5.0.4. bEnableLanSearch=0x7A\n");
    printf("\tRepeat: Connection Times (eg: 100), If not specified, connect once.\n");
    printf("\tIntervalSec: Interval time between calls to PPCS_Connect, Delay time in sec.(eg: 100), If not specified, default IntervalSec=3 sec, minimum IntervalSec is 1 sec,if you set 0, the program will reset it to 1.\n");
    printf("\tWakeupKey: Wakeup Server String Key.\n");
    printf("\t[IP1],[IP2],[IP3]: Three Wakeup Server IP or domain name.\n");
}


int connectiontester_main(int argc, char **argv)
{


    // Show Log
#ifdef DEBUG
    g_ST_INFO_OPEN = 1;
#else
    g_ST_INFO_OPEN = 0;
#endif

    //// 1. get P2P API Version
    UINT32 APIVersion = PPCS_GetAPIVersion();
    char VerBuf[64] = {};
    memset(VerBuf, 0, sizeof(VerBuf));
    SNPRINTF(VerBuf, sizeof(VerBuf), "%d.%d.%d.%d",
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
    }
    else { st_info_T("PPCS_GetAPIInformation:%s\n", PPCS_GetAPIInformation()); } // PPCS_GetAPIInformation: by Version >= 3.5.0

#if defined(VSBUILD)
    st_info("%s:%s, %s %s\n", PROGRAM_NAME, TESTER_VERSION, __DATE__, __TIME__);
#else
    st_info("%s:%s, %s\n", PROGRAM_NAME, TESTER_VERSION, BUILD_DATE);
#endif

    if (4 > argc || 10 < argc)
    {
        showArg(argv);
        return 0;
    }

    const char *pTestMode = argv[1];
    const char *pDID = argv[2];
    const char *pInitString = argv[3];
    const char *pRepeat = NULL;

    if (5 <= argc) { pRepeat = argv[4]; }

    const char *pIntervalSec = NULL;

    if (6 <= argc) { pIntervalSec = argv[5]; }

    const char *pWakeupKey = NULL;

    if (7 < argc) { pWakeupKey = argv[6]; }

    int IP_Index = 7;

    char HaveDSK = 0;
    char DID[SIZE_DID] = {};
    char DSKey[24] = {};
    char DIDString[128] = {};
    char InitString[SIZE_INITSTRING] = {};
    char WakeupKey[SIZE_WAKEUP_KEY] = {};

    int flag_WakeupTest = 0;
    int Repeat = 1;
    int IntervalSec = 3;
    int UDP_Port = 12305;
    unsigned char NumberOfServer = 0;
    char dest[20];
    memset(dest, 0, sizeof(dest));
    struct sockaddr_in serveraddr[SERVER_NUM];

    char bEnableLanSearch = 0x7A;
    int TestMode = atoi(pTestMode);

    switch (TestMode)
    {
    case 0: bEnableLanSearch = 0; break;

    case 1: bEnableLanSearch = 1; break;

    case 2: bEnableLanSearch = 30; break;

    case 3: bEnableLanSearch = 31; break;

    case 4: bEnableLanSearch = 0x7F; break; // network detect mode.

    case 5: bEnableLanSearch = (0 <= strncmp(VerBuf, "4.1.0.0", 5)) ? 0x7A : 0x7E; break;

    case 6: bEnableLanSearch = 94; break;

    case 7: bEnableLanSearch = 0x7B; break;

    case 8: bEnableLanSearch = 0x7C; break;

    case 9:
    {
        if (0 > strncmp(VerBuf, "5.0.4.0", 5))
        {
            st_info("The PPCS API Version=%s, Can not Support WakeupInfo test Mode, API Version must be >= 5.0.4\n", VerBuf);
            return 0;
        }

        bEnableLanSearch = 0x7A;
        break;
    }

    default:
    {
        st_info("ConnectionTester have No this test Mode:%d!\n", TestMode);
        return 0;
    }
    }

    const char *p = strstr(pDID, ":");

    if (p)
    {
        memcpy(DID, pDID, (int)(p - pDID));
        memcpy(DSKey, p + 1, strlen(p + 1));
        HaveDSK = 1;
    }
    else { memcpy(DID, pDID, strlen(pDID)); }

    memset(gLogPath, 0, sizeof(gLogPath));
    SNPRINTF(gLogPath, sizeof(gLogPath), "./Log/%s", PROGRAM_NAME);
    memset(gLogFileName, 0, sizeof(gLogFileName));
    SNPRINTF(gLogFileName, sizeof(gLogFileName), "%s.log", DID);
    // write Log
#ifdef DEBUG_LOG
    g_DEBUG_LOG_FILE_OPEN = 1;
#else
    g_DEBUG_LOG_FILE_OPEN = 0;
#endif

    memcpy(DIDString, pDID, strlen(pDID));
    memcpy(InitString, pInitString, strlen(pInitString));

    if (5 <= argc) { Repeat = atoi(pRepeat); }

    if (6 <= argc) { IntervalSec = atoi(pIntervalSec); }

    if (0 >= IntervalSec) { IntervalSec = 1; }

    if (7 < argc) { memcpy(WakeupKey, pWakeupKey, strlen(pWakeupKey)); }

    if (8 <= argc)
    {
        flag_WakeupTest = 1;
        NumberOfServer = argc - IP_Index;
        int count = 0;
        memset(serveraddr, 0, sizeof(serveraddr));

        for (int i = 0; i < SERVER_NUM; i++)
        {
            serveraddr[i].sin_family = AF_INET;
            serveraddr[i].sin_port = htons(UDP_Port);
            serveraddr[i].sin_addr.s_addr = inet_addr("127.0.0.1");
        }

        st_debug("gethostbyname...\n");

        for (int i = 0; i < NumberOfServer; i++)
        {
            // gethostbyname: get the server's DNS entry
            struct hostent *Host = gethostbyname(argv[IP_Index + i]);

            if (!Host)
            {
                st_info("ERROR, no such host as %s\n", argv[IP_Index + i]);
                perror("gethostbyname failed");
                count++;
            }
            else
            {
                // build the server's Internet address
                serveraddr[i].sin_family = Host->h_addrtype;
                serveraddr[i].sin_port = htons(UDP_Port);
                serveraddr[i].sin_addr.s_addr = *((unsigned int *)Host->h_addr_list[0]);
                //bcopy((char *)Host.h_addr, (char *)&serveraddr[i].sin_addr.s_addr, Host.h_length);
            }
        }

        if (NumberOfServer == count)
        {
            error("ERROR, gethostbyname failed");
        }

        for (int i = 0; i < NumberOfServer; i++)
        {
            //st_debug("Host[%d]:%s\n", i, inet_ntop(serveraddr[i].sin_family, (char *)&serveraddr[i].sin_addr.s_addr, dest, sizeof(dest)));
            st_info("Host[%d]:%s\n", i, inet_ntoa(serveraddr[i].sin_addr));
        }
    }

    st_info("Mode=%d\n", TestMode);
    st_info("DID=%s\n", DID);

    if (HaveDSK) { st_info("DSKey=%s\n", DSKey); }

    st_info("bEnableLanSearch=0x%02X\n", bEnableLanSearch);
    st_info("InitString=%s\n", InitString);
    st_info("Repeat=%d\n", Repeat);
    st_info("IntervalSec=%d sec\n", IntervalSec);

    if (1 == flag_WakeupTest)
    {
        st_info("WakeupKey=%s\n", WakeupKey);
        st_info("NumberOfServer=%d\n\n", NumberOfServer);
        WinSockInit();
    }

    st_Time_Info t0, t1, t2, t3;
    // st_Time_Info t4;
    int ret = -1;

    (void)t0;
    (void)t3;

    // 2. P2P Initialize
    if (0 <= strncmp(VerBuf, "4.2.0.0", 5)) // PPCS_Initialize JsonString by Version>=4.2.0
    {
        int MaxNumSess = 2; // Max Number Session: 1~256.
        int SessAliveSec = 6; // session timeout close alive sec: 6~30s.
        char InitJsonString[256] = {};
        SNPRINTF(InitJsonString, sizeof(InitJsonString), "{\"InitString\":\"%s\",\"MaxNumSess\":%d,\"SessAliveSec\":%d}", InitString, MaxNumSess, SessAliveSec);
        // st_debug("InitJsonString=%s\n",InitJsonString);
        my_GetCurrentTime(&t1);
        st_debug("[%s] PPCS_Initialize1(%s) ...\n", t1.Date, InitJsonString);
        // 如果Parameter 不是正确的JSON字串则会被当成InitString[:P2PKey]来处理, 如此以兼容旧版.
        //        ret = PPCS_Initialize((char *)InitJsonString);
        ret = PPCS_Initialize(InitString); //CS2 fae表示，如遇到-22错误，可传入InitString
        my_GetCurrentTime(&t2);
        st_info("[%s] PPCS_Initialize2(%s): ret=%d, t:%d ms\n", t2.Date, InitJsonString, ret, TU_MS(t1, t2));
        st_info("PPCS_Initialize ret=%d \n", ret);

        if (ERROR_PPCS_SUCCESSFUL != ret && ERROR_PPCS_ALREADY_INITIALIZED != ret)
        {
            if (1 == flag_WakeupTest)
            {
                WinSockDeInit();
            }

            return 0;
        }
    }
    else
    {
        my_GetCurrentTime(&t1);
        st_debug("[%s] PPCS_Initialize4(%s) ...\n", t1.Date, InitString);
        ret = PPCS_Initialize((char *)InitString);
        my_GetCurrentTime(&t2);
        st_info("[%s] PPCS_Initialize5(%s): ret=%d, t:%d ms\n", t2.Date, InitString, ret, TU_MS(t1, t2));
        st_info("PPCS_Initialize ret=%d \n", ret);

        if (ERROR_PPCS_SUCCESSFUL != ret && ERROR_PPCS_ALREADY_INITIALIZED != ret)
        {
            if (1 == flag_WakeupTest)
            {
                WinSockDeInit();
            }

            return 0;
        }
    }

    if (0 <= strncmp(VerBuf, "3.5.0.0", 5))
    {
        const char *APIInformation = PPCS_GetAPIInformation();
        st_info_T("PPCS_GetAPIInformation(%u Byte):%s\n", strlen(APIInformation), APIInformation);
    }

    // 3. 网络侦测
    st_PPCS_NetInfo NetInfo;
    my_GetCurrentTime(&t1);
    ret = PPCS_NetworkDetect(&NetInfo, 0);
    my_GetCurrentTime(&t2);
    st_info("[%s] PPCS_NetworkDetect: %d ms, ret=%d [%s]\n", t2.Date, TU_MS(t1, t2), ret, getP2PErrorCodeInfo(ret));
    showNetwork(NetInfo);

    if (4 == TestMode)
    {
        bEnableLanSearch = 0x7F;
        my_GetCurrentTime(&t1);
        st_debug("[%s] PPCS_Connect(%s,0x%02X,0) ...\n", t1.Date, DIDString, bEnableLanSearch);
        ret = PPCS_Connect(DIDString, bEnableLanSearch, 0);

        // st_debug("[%s] PPCS_ConnectByServer(%s, 0x%02X, 0, %s) ...\n", t1.Date, DIDString, bEnableLanSearch, InitString);
        // ret = PPCS_ConnectByServer(DIDString, bEnableLanSearch, 0, InitString);
        my_GetCurrentTime(&t2);
        st_info("[%s] PPCS_Connect(%s,0x%02X,0): %d ms, ret=%d [%s]\n", t2.Date, DIDString, bEnableLanSearch, TU_MS(t1, t2), ret, getP2PErrorCodeInfo(ret));

        if (0 <= strncmp(VerBuf, "4.3.0.0", 5))
        {
            bEnableLanSearch = 0x79;
            my_GetCurrentTime(&t1);
            st_debug("[%s] PPCS_Connect(%s,0x%02X,0) ...\n", t1.Date, DIDString, bEnableLanSearch);
            ret = PPCS_Connect(DIDString, bEnableLanSearch, 0);

            // st_debug("[%s] PPCS_ConnectByServer(%s, 0x%02X, 0, %s) ...\n", t1.Date, DIDString, bEnableLanSearch, InitString);
            // ret = PPCS_ConnectByServer(DIDString, bEnableLanSearch, 0, InitString);
            my_GetCurrentTime(&t2);

            if (0 <= ret)
            {
                st_info("[%s] PPCS_Connect(%s,0x%02X,0): %d ms, The Device LastLogin: %d Sec Before.\n", t2.Date, DIDString, bEnableLanSearch, TU_MS(t1, t2), ret);
            }
            else { st_info("[%s] PPCS_Connect(%s,0x%02X,0): %d ms, ret=%d [%s]\n", t2.Date, DIDString, bEnableLanSearch, TU_MS(t1, t2), ret, getP2PErrorCodeInfo(ret)); }
        }

        PPCS_DeInitialize();
        return 0;
    }

    int Total_Times = Repeat;
    INT32 SuccessCounter = 0;
    INT32 P2PCounter = 0;
    INT32 RLYCounter = 0;
    INT32 LANCounter = 0;
    INT32 TCPCounter = 0;
    INT32 t_Total = 0;
    INT32 t_Max_Used = 0;
    INT32 t_Min_Used = 999999;
    int LastSleepLogin = ERROR_UnKnown;
    int LastLogin[3] = {ERROR_UnKnown, ERROR_UnKnown, ERROR_UnKnown};

    int t_Connect = 0;
    int t_Read = 0;
    Repeat = 0;

    // int tInterval_bak = IntervalSec;
    while (Repeat < Total_Times)
    {
        st_debug("------------------------------------> Repeat=%d\n", ++Repeat);
        char LastLoginInfo[128] = {};
        memset(LastLoginInfo, 0, sizeof(LastLoginInfo));

        if (1 == flag_WakeupTest)
        {
            LastSleepLogin = Wakeup_Query(serveraddr, NumberOfServer,
                                          DID, WakeupKey,
                                          3, 2000, //repeat=3, timeout=2sec, total timeout:3*2=6sec
                                          &LastLogin[0],
                                          &LastLogin[1],
                                          &LastLogin[2]);

            if (-99 == LastSleepLogin)
            {
                SNPRINTF(LastLoginInfo, sizeof(LastLoginInfo), ",LastSleepLogin=(NoRespFromServer)");
            }
            else if (-1 > LastSleepLogin)
            {
                SNPRINTF(LastLoginInfo, sizeof(LastLoginInfo), ",LastSleepLogin=(%s)", getWakeupQueryError(LastSleepLogin));
            }
            else
            {
                SNPRINTF(LastLoginInfo, sizeof(LastLoginInfo), ",LastSleepLogin=%d(%d,%d,%d)", LastSleepLogin, LastLogin[0], LastLogin[1], LastLogin[2]);
            }
        }

        my_GetCurrentTime(&t1);

        switch (TestMode)
        {
        case 0: //// bEnableLanSearch = 0, No local LAN OFF, P2P then Relay for remote.
        case 1: //// bEnableLanSearch = 1, Local LAN search, P2P then Relay for remote.
        case 2: //// bEnableLanSearch = 30, No local LAN search, force Relay for remote
        case 3: //// bEnableLanSearch = 31, Local LAN search, force Relay for remote
        case 6: //// bEnableLanSearch = 0x5E(94) No local LAN search, force Server Relay for remote.
        {
            // st_debug("[%s] %02d-PPCS_Connect(%s, 0x%02X, 0) ...\n", t1.Date, Repeat, DIDString, bEnableLanSearch);
            // ret = PPCS_Connect(DIDString, bEnableLanSearch, 0);
            char ByServerString[512] = {0};

            if (0 <= strncmp(VerBuf, "4.5.3.0", 5))
            {
#if 0
                int connTimeout = 15;
                char AllowRP2P = 1;

                if (0 <= strncmp(VerBuf, "5.0.1.0", 5)) //// PPCS_ConnectByServer AllowRP2P parameter support by P2P API Version >= 5.0.1
                {
                    BK_LOGI("p2p", "%s use timeout %d AllowRP2P %d\n", __func__, connTimeout, AllowRP2P);
                    SNPRINTF(ByServerString, sizeof(ByServerString), "{\"InitString\":\"%s\",\"0x7X_Timeout\":%d,\"AllowRP2P\":%d}", InitString, connTimeout, AllowRP2P);
                }
                else
                {
                    BK_LOGI("p2p", "%s use timeout %d\n", __func__, connTimeout);
                    SNPRINTF(ByServerString, sizeof(ByServerString), "{\"InitString\":\"%s\",\"0x7X_Timeout\":%d}", InitString, connTimeout);
                }
#else
                SNPRINTF(ByServerString, sizeof(ByServerString), "{\"InitString\":\"%s\"}", InitString);
#endif
            }
            else { SNPRINTF(ByServerString, sizeof(ByServerString), "%s", InitString); }

            st_debug("[%s] %02d-PPCS_ConnectByServer(%s,0x%02X,0,ByServerStringlen %d) ...\n", t1.Date, Repeat, DIDString, bEnableLanSearch, strlen(ByServerString));
            ret = PPCS_ConnectByServer(DIDString, bEnableLanSearch, 0, ByServerString);
            break;
        }

        case 5: //// bEnableLanSearch = 0x7A: 0x7E(LAN + P2P + RLY(with 240ms delay)) + TCP Relay (with 1.5 sec delay)
        case 7: //// bEnableLanSearch = 0x7B: 0x7D(LAN + P2P + RLY) + TCP Relay (no delay)
        case 8: //// bEnableLanSearch = 0x7C: LAN + TCP Relay (no delay).
        {
            char ByServerString[512] = {};

            //// The last parameter support of ConnectByServer is set to json since version 4.5.3
            //// PPCS_ConnectByServer 0x7X_Timeout parameter support by P2P API Version >= 4.5.3
            if (0 <= strncmp(VerBuf, "4.5.3.0", 5))
            {
                int connTimeout = 15;
                SNPRINTF(ByServerString, sizeof(ByServerString), "{\"InitString\":\"%s\",\"0x7X_Timeout\":%d}", InitString, connTimeout);
            }
            else { SNPRINTF(ByServerString, sizeof(ByServerString), "%s", InitString); }

            st_debug("[%s] %02d-PPCS_ConnectByServer(%s,0x%02X,0,%s) ...\n", t1.Date, Repeat, DIDString, bEnableLanSearch, ByServerString);
            ret = PPCS_ConnectByServer(DIDString, bEnableLanSearch, 0, ByServerString);
            break;
        }

        case 9:
        {
            char ByServerString[512] = {};

            if (0 <= strncmp(VerBuf, "5.0.4.0", 5))
            {
                char WakeupInfo[18] = "The-Wakeup-Info";
                SNPRINTF(ByServerString, sizeof(ByServerString), "{\"InitString\":\"%s\",\"WakeupInfo\":\"%s\"}", InitString, WakeupInfo);
            }
            else { SNPRINTF(ByServerString, sizeof(ByServerString), "%s", InitString); }

            st_debug("[%s] %02d-PPCS_ConnectByServer(%s,0x%02X,0,%s) ...\n", t1.Date, Repeat, DIDString, bEnableLanSearch, ByServerString);
            ret = PPCS_ConnectByServer(DIDString, bEnableLanSearch, 0, ByServerString);
            break;
        }

        default:
        {
            st_info("ConnectionTester have No this test Mode:%d!\n", TestMode);
            return 0;
        }
        }

        my_GetCurrentTime(&t2);
        t_Connect = TU_MS(t1, t2);

        if (ret < 0)
        {
            st_info("[%s] %02d-Connect(%s,0x%02X)%s failed:%d ms, ret=%d, %s\n", t2.Date, Repeat, DIDString, bEnableLanSearch, LastLoginInfo, t_Connect, ret, getConnectErrorInfo(ret));
        }
        else // Connect success!!
        {
            int SessionID = ret;
            st_MySess_Info mSInfo;
            ret = MyGetSInfo(SessionID, &mSInfo);

            if (0 > ret)
            {
                st_info("[%s] %02d-Connect(%s,0x%02X)%s,RemoteAddr=Unknown (PPCS_Check:%d)\n", t2.Date, Repeat, DIDString, bEnableLanSearch, LastLoginInfo, ret);
            }
            else
            {
                if (0 == mSInfo.bMyMode) { LANCounter++; }
                else if (1 == mSInfo.bMyMode) { P2PCounter++; }
                else if (2 == mSInfo.bMyMode) { RLYCounter++; }
                else if (3 == mSInfo.bMyMode) { TCPCounter++; }

#ifdef ACK_CHECK  // 确认连接可正常读写。
                UINT32 timeOut_ms = 2000;

                if (3 == mSInfo.bMyMode)
                {
                    timeOut_ms = 3500;
                    // if (3 > IntervalSec) IntervalSec = 3;
                }

                // else IntervalSec = tInterval_bak;
                my_GetCurrentTime(&t0);

                while (1)
                {
                    // 读取 ACK.
                    char ReadData = -99;
                    int ReadSize = 1;
                    // my_GetCurrentTime(&t3);
                    ret = PPCS_Read(SessionID, CH_CMD, (char *)&ReadData, &ReadSize, timeOut_ms);

                    // my_GetCurrentTime(&t4);
                    if (0 > ret && 0 == ReadSize)
                    {
                        st_info("[%s] %02d-Connect(%s,0x%02X)%s,Session=%d,Skt=%d,Local=%s:%d,Rmt=%s:%d,Mode=%s,Time=%d.%03d Sec\n", t2.Date, Repeat, DIDString, bEnableLanSearch, LastLoginInfo, SessionID, mSInfo.Skt, mSInfo.MyLocalIP, mSInfo.MyLocalPort, mSInfo.RemoteIP, mSInfo.RemotePort, mSInfo.Mode, t_Connect / 1000, t_Connect % 1000);

                        if (ERROR_PPCS_TIME_OUT == ret)
                        {
                            st_debug_T("-%02d-PPCS_Read: Session=%d,CH=%d,Mode=%s,ReadSize=%d,ret=%d [%s]\n", Repeat, SessionID, CH_CMD, mSInfo.Mode, ReadSize, ret, getP2PErrorCodeInfo(ret));
                        }
                        else
                        {
                            st_debug_T("-%02d-PPCS_Read: Session=%d,CH=%d,Mode=%s,ReadSize=%d,ret=%d [%s]\n", Repeat, SessionID, CH_CMD, mSInfo.Mode, ReadSize, ret, getP2PErrorCodeInfo(ret));
                            break;
                        }
                    }
                    else if (ERROR_PPCS_INVALID_PARAMETER != ret && ERROR_PPCS_INVALID_SESSION_HANDLE != ret && 0 < ReadSize)
                    {
                        st_info("[%s] %02d-Connect(%s,0x%02X)%s,Session=%d,Skt=%d,Local=%s:%d,Rmt=%s:%d,Mode=%s,Time=%d.%03d Sec.\n", t2.Date, Repeat, DIDString, bEnableLanSearch, LastLoginInfo, SessionID, mSInfo.Skt, mSInfo.MyLocalIP, mSInfo.MyLocalPort, mSInfo.RemoteIP, mSInfo.RemotePort, mSInfo.Mode, t_Connect / 1000, t_Connect % 1000);
                        st_debug_T("-%02d-PPCS_Read: ret=%d,Session=%d,CH=%d,Mode=%s,ReadSize=%d => [%d]\n", Repeat, ret, SessionID, CH_CMD, mSInfo.Mode, ReadSize, ReadData);

                        //// --------------> 回应 ACK
                        int SendData = ReadData & 0xFE;
                        ret = PPCS_Write(SessionID, CH_CMD, (char *)&SendData, sizeof(SendData));

                        if (0 > ret)
                        {
                            st_debug_T("-%02d-PPCS_Write:Session=%d,CH=%d,Mode=%s,SendSize=%d,Data:[%d],ret=%d [%s]\n", Repeat, SessionID, CH_CMD, mSInfo.Mode, sizeof(SendData), SendData, ret, getP2PErrorCodeInfo(ret));
                        }
                        else { st_debug_T("-%02d-PPCS_Write:ret=%d,Session=%d,CH=%d,Mode=%s,SendSize=%d => [%d]\n", Repeat, ret, SessionID, CH_CMD, mSInfo.Mode, sizeof(SendData), SendData); }
                    }
                    else
                    {
                        st_info("[%s] %02d-Connect(%s,0x%02X)%s,Session=%d,Skt=%d,Local=%s:%d,Rmt=%s:%d,Mode=%s,Time=%d.%03d Sec\n", t2.Date, Repeat, DIDString, bEnableLanSearch, LastLoginInfo, SessionID, mSInfo.Skt, mSInfo.MyLocalIP, mSInfo.MyLocalPort, mSInfo.RemoteIP, mSInfo.RemotePort, mSInfo.Mode, t_Connect / 1000, t_Connect % 1000);
                        st_debug_T("-%02d-PPCS_Read: Session=%d,CH=%d,Mode=%s,ReadSize=%d,ret=%d [%s]\n", Repeat, SessionID, CH_CMD, mSInfo.Mode, ReadSize, ret, getP2PErrorCodeInfo(ret));
                    }

                    break;
                } // while

                my_GetCurrentTime(&t3);
                t_Read = TU_MS(t0, t3);
#else
                st_info("[%s] %02d-Connect(%s,0x%02X)%s,Session=%d,Skt=%d,Local=%s:%d,Rmt=%s:%d,Mode=%s,Time=%d.%03d Sec\n", t2.Date, Repeat, DIDString, bEnableLanSearch, LastLoginInfo, SessionID, mSInfo.Skt, mSInfo.MyLocalIP, mSInfo.MyLocalPort, mSInfo.RemoteIP, mSInfo.RemotePort, mSInfo.Mode, t_Connect / 1000, t_Connect % 1000);
#endif
                SuccessCounter++;
                t_Max_Used = (t_Max_Used < t_Connect) ? t_Connect : t_Max_Used;
                t_Min_Used = (t_Min_Used > t_Connect) ? t_Connect : t_Min_Used;
                t_Total += t_Connect;
            }

            mSleep(300);
            my_GetCurrentTime(&t1);
            PPCS_Close(SessionID);
            my_GetCurrentTime(&t2);
            st_debug_T("-%02d-PPCS_Close(%d) done!! t:%d ms\n", Repeat, SessionID, TU_MS(t1, t2));
        } // ret >= 0, Connect success!!

        setbuf(stdout, NULL);

        if (Repeat != Total_Times)
        {
            int sleepTime = IntervalSec * 1000 - t_Read - 300; // 300 is sleep time.

            if (0 < sleepTime) { mSleep(sleepTime); }
        }
    } // while(Repeat > 0)

    st_info("Total Connection times:%d, Success:%d", Total_Times, SuccessCounter);

    // P2PCounter = SuccessCounter-RLYCounter-LANCounter-TCPCounter;
    if (SuccessCounter)
    {
        // if (2 == Mode || 6 == Mode) RLYCounter = SuccessCounter;
        t_Total = t_Total / SuccessCounter; // msec
        st_info(" (%f%%, max=%d.%03d sec, averge=%d.%03d sec, min=%d.%03d sec)\n",
                100.0 * ((float)SuccessCounter) / ((float)Total_Times),
                t_Max_Used / 1000, t_Max_Used % 1000,   // max
                t_Total / 1000, t_Total % 1000,         // averge
                t_Min_Used / 1000, t_Min_Used % 1000);  // min
        st_info("LAN: %d (%f%%), P2P: %d (%f%%), RLY: %d (%f%%), TCP: %d (%f%%)\n",
                LANCounter, 100.0 * ((float)LANCounter) / ((float)SuccessCounter), // LAN
                P2PCounter, 100.0 * ((float)P2PCounter) / ((float)SuccessCounter), // P2P
                RLYCounter, 100.0 * ((float)RLYCounter) / ((float)SuccessCounter), // RLY
                TCPCounter, 100.0 * ((float)TCPCounter) / ((float)SuccessCounter)); // TCP
    }
    else { st_info(", LAN: %d (%f%%), P2P: %d (%f%%), RLY: %d (%f%%), TCP: %d (%f%%)\n", LANCounter, (float)0, P2PCounter, (float)0, RLYCounter, (float)0, (float)0, TCPCounter); }

    my_GetCurrentTime(&t1);
    ret = PPCS_DeInitialize();
    my_GetCurrentTime(&t2);
    st_debug_T("PPCS_DeInitialize done!! %d ms\n", TU_MS(t1, t2));

#if defined(WIN32DLL) || defined(WINDOWS)

    if (1 == flag_WakeupTest) { WSACleanup(); }

    // printf("Please press any key to exit... ");
    // getchar();
#endif

    (void)gFlags_WinSktInitOK;



    return 0;
}
