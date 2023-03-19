/*
//// ReadWriteTester
//// Author: BC.ZHENG
//// Date: 2019.07.25
////    To test PPCS connection with a specified DID device, from an Internet Host
////
2022.04.20 Release_SDK_PPCS_5.0.1
    ReadWriteTester V0.0.3.5:
        1. 新增 PPCS_Check 检查线程测试 AllowRP2P 功能，每隔 100 ms 检查一次，当连接模式 Mode 改变时打印并退出。

2022.06.16 Release_SDK_PPCS_5.0.4
        2. PPCS_ConnectByServer() 最后一个参数新增 JSON 格式调用示例和版本兼容判断。

2022.09.06 Release_SDK_PPCS_5.0.6
    ReadWriteTester V0.0.3.6:
        3. ReadWriteTester sample 去除 WakeupInfo 的调用范例，如需测试 WakeupInfo 请参考 ConnectionTester mode=9.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>  // uintptr_t



#include "cs2_p2p_main.h"
#ifdef LINUX
    // #include <stdio.h>
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
    #include <stdarg.h>  // va_start va_end
    #include <dirent.h>
    #include <arpa/inet.h>
#endif
#if defined(WIN32DLL) || defined(WINDOWS)
    // #include <windows.h>
    // #include <direct.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #include <direct.h>
#else
    #include <lwip/sockets.h>
#endif

#include "PPCS_API.h"

#define     TESTER_VERSION  "0.0.3.6"
#define     PKT_TEST

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
#define SIZE_DID            64
#define SIZE_INITSTRING     256

// define for Read/Write test mode
#define BASE_SIZE               1004    // (251 * 4)
#define TEST_SIZE_ONE_WRITE     (128*BASE_SIZE)    // Size of data written at one PPCS_Write.
#define TEST_SIZE_ONE_WRITE_MODIFY     (1 * BASE_SIZE)

// #define TOTAL_WRITE_SIZE        (8*1024*TEST_SIZE_ONE_WRITE)
#define TEST_SIZE_1             (1*8*TEST_SIZE_ONE_WRITE)      // ~= 0.98MB
#define TEST_SIZE_10            (11*8*TEST_SIZE_ONE_WRITE)     // ~= 10.7MB
#define TEST_SIZE_100           (110*8*TEST_SIZE_ONE_WRITE)    // ~= 107MB
#define TEST_SIZE_1000          (1024*8*TEST_SIZE_ONE_WRITE)   // ~= 1004MB

#define CHECK_WRITE_THRESHOLD_SIZE  1024*1024
#define CHECK_WRITE_THRESHOLD_SIZE_MODIFY  1024*1024 //for armino
#define NUMBER_OF_P2PCHANNEL    2
#define CHECK_BUFFER_FLAGS      0       // check buffer thread will be create, check buffer every 1 sec.
#define CHECK_FLAGS             1       // check session mode every 100 ms, if connections mode is changed, it will print.

// show info 开关 -> 终端打印调试信息
static int g_ST_INFO_OPEN = 0;
// debug log 开关 -> 输出到本地log文件
static int g_DEBUG_LOG_FILE_OPEN = 0;
#define SIZE_LOG_NAME   256
static char gLogFileName[128] = {};
static char gLogPath[256] = {};

static char gThread_Check_Exit = 0;
static char gThread_CheckBuffer_Exit = 0;
static char gThread_CheckBuffer_Running = 0;
static char gThread_UDP_ECHO_Exit = 0;
static int gUDP_ECHO_Socket = -1;

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
static st_RW_Test_Info g_RW_Test_Info[NUMBER_OF_P2PCHANNEL];

typedef struct
{
    int SessionID;
    int Channel;
    char Num_CH;
} st_ChkBuf_Info;

typedef struct
{
    int  Skt;                       // Sockfd
    char RemoteIP[16];
    int RemotePort;
    char MyLocalIP[16];
    int MyLocalPort;
    char MyWanIP[16];
    int MyWanPort;
    unsigned int ConnectTime;       // Connection build in ? Sec Before
    char DID[24];                   // Device ID
    char bCorD;     // I am Client or Device, 0: Client, 1: Device
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
    st_info("%s\n", msg);
    perror(msg);
    exit(0);
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

// ret = -1: socket error (closed or invalid)
// ret = -3: recvfrom timeout.
// ret >= 0: number of byte read
static int UDP_Recv_Till_TimeOut_By_Select(int Skt, char *RecvBuf, int SizeToRead, struct sockaddr_in *fromAddr, unsigned int TimeOut_ms)
{
    int result;
    fd_set readfds;
    //fd_set writefds;
    //fd_set exceptfds;
    //socklen_t sin_len;

    //FD_ZERO(&readfds);
    //FD_ZERO(&exceptfds);
    //FD_SET(Skt, &readfds);
    //FD_SET(Skt, &exceptfds);
    struct timeval timeout;
    timeout.tv_sec = (TimeOut_ms - (TimeOut_ms % 1000)) / 1000;
    timeout.tv_usec = (TimeOut_ms % 1000) * 1000;
    unsigned int sin_size = sizeof(struct sockaddr_in);
    int size = -1;

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(Skt, &readfds);
        result = select(Skt + 1, &readfds, (fd_set *)NULL, (fd_set *)NULL, &timeout);

        switch (result)
        {
        case 0:
        {
            if (0 < size)
            {
                return size;
            }
            else
            {
                return -3;    // time out
            }
        }

        case -1:
        {
            st_debug("select return -1!! Skt=%d, errno=%d [%s]\n", Skt, errno, strerror(errno));
            return -1;
        }

        default:
        {
            if (FD_ISSET(Skt, &readfds))
            {
                size = recvfrom(Skt, RecvBuf, SizeToRead, 0, (struct sockaddr *)fromAddr, (socklen_t *)&sin_size);

                if (0 > size)
                {
                    st_info("UDP recvfrom failed(%d)!! Skt=%d, errno=%d [%s]", size, Skt, errno, strerror(errno));
                    return -1;
                }

                // st_debug_T("recvfrom Size: %u Byte\n", size);
                return size;
            } // if (FD_ISSET(Skt, &readfds))

            break;
        }
        } // switch (result)
    } // while

    return size;
} // UDP_Recv_Till_TimeOut_By_Select

// ret: <0: Invalid Parameter or socket read error or timeout.
//      >=0: data size.
static int UDP_Recv_Till_TimeOut(int Skt, char *RecvBuf, int SizeToRead, struct sockaddr_in *fromAddr, unsigned int TimeOut_ms)
{
    return UDP_Recv_Till_TimeOut_By_Select(Skt, RecvBuf, SizeToRead, fromAddr, TimeOut_ms);
} // UDP_Recv_Till_TimeOut

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
    char tBuf[] = "[YYYY-MM-DD hh:mm:ss.xxx]";
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
            st_debug("%s UDP_RECV_ECHO: sendto [%s:%d] failed: Skt=%d, errno=%d [%s]\n", tBuf, inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port), mSocket, errno, strerror(errno));
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

//// 主动发送一个封包到指定 RemoteIP:8899, 然后等待接收数据，收到数据并打印其时间戳，精确到毫秒。
#if defined(WIN32DLL) || defined(WINDOWS)
    static DWORD WINAPI Thread_UDP_Ping(void *arg)
#elif defined(LINUX)
    static void *Thread_UDP_Ping(void *arg)
#endif
{
#if defined LINUX
    pthread_detach(pthread_self());
#endif

    if (!arg)
    {
        st_info("UDP_Ping: Thread exit for arg is NULL!!\n");
        my_Thread_exit(0);
    }

    char *pRemoteIP = (char *)arg;
    char RemoteIP[32] = {};
    memcpy(RemoteIP, pRemoteIP, strlen(pRemoteIP));

    if (pRemoteIP) {free(pRemoteIP); pRemoteIP = NULL;}

    st_Time_Info t0, t1;
    char tBuf[] = "[YYYY-MM-DD hh:mm:ss.xxx]";
    // GetTime_ms(&t0, tBuf, sizeof(tBuf));

    int mSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_in fromAddr;
    memset((void *)&serverAddr, 0, sizeof(serverAddr));
    memset((void *)&fromAddr, 0, sizeof(fromAddr));
    unsigned int sin_size;
    // unsigned int value = 0x1;
    int Port = 8899;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(RemoteIP);
    serverAddr.sin_port = htons(Port);

    for (int i = 0, times = 3; i < times; i++)
    {
        if (0 > (mSocket = socket(PF_INET, SOCK_DGRAM, 0)))
        {
            st_info_T("UDP_Ping: create UDP Socket failed: errno=%d [%s]\n", strerror(errno));

            if (i == times - 1)
            {
                st_info_T("UDP_Ping: Thread exit for create UDP Socket failed: errno=%d [%s]\n", errno, strerror(errno));
                my_Thread_exit(0);
            }

            mSleep(1000);
            continue;
        }

        st_debug_T("UDP_Ping: create UDP Socket OK: Skt=%d\n", mSocket);
        break;
    } // for

    char Data[1024] = {};
    memset(Data, '0', sizeof(Data));
    // st_debug("Data=%s, len=%u\n", Data, (unsigned)strlen(Data));
    sin_size = sizeof(struct sockaddr_in);
    unsigned long index = 0;

    while (1)
    {
        GetTime_ms(&t0, tBuf, sizeof(tBuf));
        SNPRINTF(Data, sizeof(Data), "index=%lu,Tick_mSec=%llu&", index, t0.Tick_mSec);
        int ret = sendto(mSocket, Data, sizeof(Data), 0, (struct sockaddr *)&serverAddr, sin_size);

        if (0 > ret)
        {
            st_info_T("UDP_Ping: sendto %s:%d failed: ret=%d, Skt=%d, errno=%d [%s]\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port), ret, mSocket, errno, strerror(errno));
            my_SocketClose(mSocket);
            my_Thread_exit(0);
        }
        else { st_debug_T("UDP_Ping: sendto %s:%d Success!! Skt=%d, Size=%d Byte\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port), mSocket, ret); }

        // 读取数据
        char ReadBuf[1024] = {};
        int ReadSize = UDP_Recv_Till_TimeOut(mSocket, ReadBuf, sizeof(ReadBuf), &fromAddr, 500);
        GetTime_ms(&t1, tBuf, sizeof(tBuf));

        if (0 >= ReadSize)
        {
            st_info("%s UDP_Ping: recvfrom failed: ret=%d, Skt=%d, errno=%d [%s]\n", tBuf, ReadSize, mSocket, errno, strerror(errno));
            break;
        }

        // sin_size = sizeof(struct sockaddr_in);
        // int ReadSize = recvfrom(mSocket, ReadBuf, sizeof(ReadBuf), 0, (struct sockaddr*)&fromAddr, &sin_size);
        // GetTime_ms(&t1, tBuf, sizeof(tBuf));
        // if (0 >= ReadSize)
        // {
        //    st_debug("%s UDP_Ping: recvfrom failed: ret=%d, errno=%d [%s]\n", tBuf, ReadSize, errno, strerror(errno));
        //    continue;
        // }
        char Buf[64] = {};
        GetStringItem(ReadBuf, "index", ',', Buf, sizeof(Buf));
        unsigned long formIndex = atol(Buf);

        if (index == formIndex)
        {
            GetStringItem(ReadBuf, "Tick_mSec", '&', Buf, sizeof(Buf));
            int Delay_ms = t1.Tick_mSec - atoll(Buf);
            st_info("%s UDP_Ping: recvfrom %s:%d, Skt=%d, Size=%u Byte, Time:%d ms\n", tBuf, inet_ntoa(fromAddr.sin_addr), ntohs(fromAddr.sin_port), mSocket, ReadSize, Delay_ms);
        }

        index++;
        break;
    } // while

    my_SocketClose(mSocket);
    st_debug_T("UDP_Ping: Thread exit for job done!!\n");
    my_Thread_exit(0);
} // Thread_UDP_Ping

static void CreateThread_UDP_Ping(const char *RemoteIP)
{
    if (!RemoteIP || 0 == strlen(RemoteIP))
    {
        st_info("CreateThread_UDP_Ping - Invalid RemoteIP Parameter!!\n");
        return ;
    }

    char *pRemoteIP = (char *)malloc(32);

    if (!pRemoteIP)
    {
        st_info("CreateThread_UDP_Ping - malloc failed!!\n");
        return ;
    }

    memset(pRemoteIP, 0, 32);
    memcpy(pRemoteIP, RemoteIP, strlen(RemoteIP));
#if defined(WIN32DLL) || defined(WINDOWS)
    HANDLE hThread_UDP_Ping = CreateThread(NULL, 0, Thread_UDP_Ping, pRemoteIP, 0, NULL);

    if (NULL == hThread_UDP_Ping)
    {
        if (pRemoteIP) {free(pRemoteIP); pRemoteIP = NULL;}

        error("create Thread_UDP_Ping failed");
    }
    else { CloseHandle(hThread_UDP_Ping); }

#elif defined(LINUX)
    pthread_t hThread_UDP_Ping;

    if (0 != pthread_create(&hThread_UDP_Ping, NULL, &Thread_UDP_Ping, pRemoteIP ))
    {
        if (pRemoteIP) {free(pRemoteIP); pRemoteIP = NULL;}

        error("create Thread_UDP_Ping failed");
    }

#endif
} // CreateThread_UDP_Ping

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
    else { st_info("PPCS_Check(SessionID=%d) ret=%d [%s]\n", SessionID, ret, getP2PErrorCodeInfo(ret)); }

    return ret;
}

#if CHECK_BUFFER_FLAGS
#if defined(WIN32DLL) || defined(WINDOWS)
    static DWORD WINAPI Thread_CheckBuffer(void *arg)
#elif defined(LINUX)
    static void *Thread_CheckBuffer(void *arg)
#endif
{
#if defined LINUX
    pthread_detach(pthread_self());
#endif

    if (!arg)
    {
        st_info("Thread_PPCS_Check_Buffer exit for arg is NULL!!\n");
        my_Thread_exit(0);
    }

    gThread_CheckBuffer_Running = 1;
    st_ChkBuf_Info *pChkBufInfo = (st_ChkBuf_Info *)arg;
    int SessionID = pChkBufInfo->SessionID;
    int ThreadNum = pChkBufInfo->Num_CH;
    int Channel = 0;

    if (1 == ThreadNum) { Channel = pChkBufInfo->Channel; }

    free(arg);
    arg = NULL;

    INT32 ret = 0;
    UINT32 WriteSize = 0;
    UINT32 ReadSize = 0;
    int j = 0;

    while (0 == gThread_CheckBuffer_Exit)
    {
        st_MySess_Info mSInfo;
        MyGetSInfo(SessionID, &mSInfo);

        for (int i = 0, CH = Channel; i < ThreadNum; i++, CH++)
        {
            ret = PPCS_Check_Buffer(SessionID, CH, &WriteSize, &ReadSize);
            st_info_T("-%03d-Thread_PPCS_Check_Buffer: DID=%s,Local=%s:%d,Rmt=%s:%d,Mode=%s,Session=%d,CH=%d,WSize=%d,RSize=%d,TotalWriteSize=%d,TotalReadSize=%d, ret=%d %s\n", j, mSInfo.DID, mSInfo.MyLocalIP, mSInfo.MyLocalPort, mSInfo.RemoteIP, mSInfo.RemotePort, mSInfo.Mode, SessionID, CH, WriteSize, ReadSize, g_RW_Test_Info[CH].TotalSize_Write, g_RW_Test_Info[CH].TotalSize_Read, ret, getP2PErrorCodeInfo(ret));

            if (0 > ret) { break; }
        }

        if (0 > ret) { break; }

        j++;
        mSleep(1000);
    }

    gThread_CheckBuffer_Running = 0;
    my_Thread_exit(0);
} // Thread_CheckBuffer
#endif // #if CHECK_BUFFER_FLAGS

static void CreateThread_Check_Buffer(int SessionID, int Channel, int ThreadNum)
{
#if CHECK_BUFFER_FLAGS
    gThread_CheckBuffer_Exit = 0;
    st_ChkBuf_Info *pChkBufInfo = (st_ChkBuf_Info *)malloc(sizeof(st_ChkBuf_Info));

    if (!pChkBufInfo)
    {
        if (SessionTemp) { free(SessionTemp); SessionTemp = NULL; };

        error("CreateThread_Check_Buffer - malloc ChkBuf_Info failed!!\n");
    }

    memset(pChkBufInfo, 0, sizeof(st_ChkBuf_Info));
    pChkBufInfo->SessionID = SessionID;
    pChkBufInfo->Num_CH = ThreadNum;
    pChkBufInfo->Channel = Channel;
#if defined(WIN32DLL) || defined(WINDOWS)
    HANDLE hThread_CheckBuffer = CreateThread(NULL, 0, Thread_CheckBuffer, (void *)pChkBufInfo, 0, NULL);

    if (NULL == hThread_CheckBuffer)
    {
        error("create Thread_CheckBuffer failed");
    }
    else { CloseHandle(hThread_CheckBuffer); }

#elif defined(LINUX)
    pthread_t hThread_CheckBuffer;

    if (0 != pthread_create(&hThread_CheckBuffer, NULL, &Thread_CheckBuffer, (void *)pChkBufInfo) )
    {
        error("create Thread_CheckBuffer failed");
    }

#endif
#endif // #if CHECK_BUFFER_FLAGS
}

//// PPCS_Check 检查线程，每隔 100 ms 检查一次，当连接模式 Mode 改变时打印并退出。
#if CHECK_FLAGS
#if defined(WIN32DLL) || defined(WINDOWS)
    static DWORD WINAPI Thread_PPCS_Check(void *arg)
#elif defined(LINUX)
    static void *Thread_PPCS_Check(void *arg)
#endif
{
#if defined LINUX
    pthread_detach(pthread_self());
#endif

    if (!arg)
    {
        st_info("Thread_PPCS_Check exit for arg is NULL!!\n");
        my_Thread_exit(0);
    }

    int SessionID = (int)((UINTp)arg) - 1;

    if (0 > SessionID)
    {
        st_info("Thread_PPCS_Check exit for SessionID(%d) is Invalid!!\n", SessionID);
        my_Thread_exit(0);
    }

    st_debug_T("Thread_PPCS_Check SessionID(%d) start running ...\n", SessionID);
    st_MySess_Info MySInfo;

    for (int i = 0; i < 600 && 0 == gThread_Check_Exit; i++)
    {
        int ret = MyGetSInfo(SessionID, &MySInfo);

        if (0 > ret)
        {
            // st_info_T("Thread_PPCS_Check exit for PPCS_Check Failed: ret=%d\n", ret);
            break;
        }

        if (g_MySInfo.bMyMode != MySInfo.bMyMode)
        {
            st_info_T("Thread_PPCS_Check: Mode is changed!! Mode: %s -> %s, DID=%s,Session=%d,Local=%s:%d,Rmt=%s:%d\n", g_MySInfo.Mode, MySInfo.Mode, MySInfo.DID, SessionID, MySInfo.MyLocalIP, MySInfo.MyLocalPort, MySInfo.RemoteIP, MySInfo.RemotePort);
            memset(&g_MySInfo, 0, sizeof(st_MySess_Info));
            memcpy(&g_MySInfo, &MySInfo, sizeof(st_MySess_Info));
            break;
        }

        mSleep(100);
    }

    gThread_Check_Exit = -1;
    st_debug_T("Thread_PPCS_Check job done!! thread exit!!\n");
    my_Thread_exit(0);
} // Thread_PPCS_Check
#endif // #if CHECK_FLAGS

static void CreateThread_PPCS_Check(int SessionID)
{
#if CHECK_FLAGS
    //// check  every 2 sec.
    gThread_Check_Exit = 0;
#if defined(WIN32DLL) || defined(WINDOWS)
    HANDLE hThread_Check = CreateThread(NULL, 0, Thread_PPCS_Check, (void *)(SessionID + 1), 0, NULL);

    if (NULL == hThread_Check)
    {
        error("create Thread_PPCS_Check failed");
    }
    else { CloseHandle(hThread_Check); }

#elif defined(LINUX)
    pthread_t hThread_Check;

    if (0 != pthread_create(&hThread_Check, NULL, &Thread_PPCS_Check, (void *)(SessionID + 1)) )
    {
        error("create Thread_PPCS_Check failed");
    }

#endif
#endif // #if CHECK_FLAGS
}

static int ft_Test(int SessionID)
{
#if 0
    // Open File
    char fileName[] = "./2.7z";
    FILE *fp = fopen(fileName, "wb");

    if (!fp)
    {
        st_info("ft_Test ***Error: failed to open file: %s\n", fileName);
        return -1;
    }

    memset(&g_RW_Test_Info[CH_DATA], 0, sizeof(g_RW_Test_Info[CH_DATA]));
    unsigned long *SizeCounter = &g_RW_Test_Info[CH_DATA].TotalSize_Write;
    *SizeCounter = 0;
    int BufSize = 1024 * 256;
    char *buf = (char *)malloc(BufSize + 4);

    if (!buf)
    {
        fclose(fp);
        error("ft_Test ***Error: malloc buf failed!!\n");
    }

    CreateThread_Check_Buffer(SessionID, CH_DATA, 1);
    // read data from remote
    UINT32 TimeOut_ms = 0xFFFFFFFF;

    st_Time_Info t1, t2;
    my_GetCurrentTime(&t1);

    while (1)
    {
        memset(buf, 0, BufSize + 4);
        INT32 ReadSize = BufSize;// ReadSize: 期望要读取的数据大小，在 PPCS_Read 之前必须要给 ReadSzie 重新赋值，非常重要!!
        // st_debug("PPCS_Read: Session=%d,CH=%d,ReadSize=%d, TimeOut_ms=%d, reading ...\n", SessionID, CH_DATA, ReadSize, TimeOut_ms);
        int ret = PPCS_Read(SessionID, CH_DATA, buf, &ReadSize, TimeOut_ms);

        // st_debug("PPCS_Read: Session=%d,CH=%d,ReadSize=%d,ret=%d [%s]\n", SessionID, CH_DATA, ReadSize, ret, getP2PErrorCodeInfo(ret));
        // PPCS_Read 返回 ERROR_PPCS_TIME_OUT(-3) :
        // 1. PPCS_Read 返回 -3 超时错误是正常现象，只是在设定超时时间内读取不到预设的大小，并非断线错误，需继续循环读取剩余数据。
        // 2. PPCS_Read 返回 -3 超时也有可能读到部分数据，需要对 ReadSize 做大小检测判断，本 sample code 直接累加 ReadSize 大小。
        // 3. ReadSize 是实际反映读取的数据大小，若 PPCS_Read 读取不到数据, ReadSize 变量会被清零，下一次 PPCS_Read 之前一定要重新赋值，否则 ReadSize 传 0 进 PPCS_Read 会返回 ERROR_PPCS_INVALID_PARAMETER(-5) 错误（-5：无效的参数）。
        if (ERROR_PPCS_INVALID_PARAMETER != ret && ERROR_PPCS_INVALID_SESSION_HANDLE != ret && ReadSize)
        {
            *SizeCounter += ReadSize; // 累加统计 ReadSize 值。
            fwrite(buf, ReadSize, 1, fp); // write data to file
        }

        if (0 > ret && ERROR_PPCS_TIME_OUT != ret)
        {
            if (ERROR_PPCS_SESSION_CLOSED_TIMEOUT == ret)     // 网络差导致断线。
            {
                st_info("\nSession Closed TimeOUT!!\n");
            }
            else if (ERROR_PPCS_SESSION_CLOSED_REMOTE == ret) // 对方主动关闭连接。
            {
                st_info("\nSession Remote Closed!!\n");
            }
            else { st_info("PPCS_Read: Session=%d,CH=%d,ReadSize=%d,SizeCounter=%d,ret=%d [%s]\n", SessionID, CH_DATA, ReadSize, *SizeCounter, ret, getP2PErrorCodeInfo(ret)); }

            break;
        }
        else if (ERROR_PPCS_TIME_OUT == ret && 0 == ReadSize) // 读取超时，继续读取
        {
            st_debug("PPCS_Read: SessionID=%d,CH=%d,ReadSize=%d, ret=%d [%s]\n", SessionID, CH_DATA, ReadSize, ret, getP2PErrorCodeInfo(ret));
            continue;
        }

        if ((*SizeCounter) % BufSize == (*SizeCounter) % (1024 * 1024))
        {
            st_info(".");
            setbuf(stdout, NULL);
        }
    } // while

    if (fp) { fclose(fp); fp = NULL; }

    my_GetCurrentTime(&t2);
    int t_ms = TU_MS(t1, t2);
    gThread_CheckBuffer_Exit = 1;

    char *DID = g_MySInfo.DID;
    char *RmtIP = g_MySInfo.RemoteIP;
    int RmtPort = g_MySInfo.RemotePort;
    char *LanIP = g_MySInfo.MyLocalIP;
    int LanPort = g_MySInfo.MyLocalPort;
    char *Mode = g_MySInfo.Mode;

    char SizeStr[32] = {};

    if (1024 * 1024 * 1024 <= *SizeCounter) { SNPRINTF(SizeStr, sizeof(SizeStr), "(%.2f GByte)", (double)(*SizeCounter) / (1024 * 1024 * 1024)); }
    else if (1024 * 1024 <= *SizeCounter) { SNPRINTF(SizeStr, sizeof(SizeStr), "(%.2f MByte)", (double)(*SizeCounter) / (1024 * 1024)); }

    float speed = (0 == t_ms) ? 0 : ((double)(*SizeCounter) / t_ms);
    char speedStr[32] = {};

    if (1024 * 1024 <= speed) { SNPRINTF(speedStr, sizeof(speedStr), "%.1f GByte/sec", ((double)speed * 1000) / (1024 * 1024 * 1024)); }
    else if (1024 <= speed) { SNPRINTF(speedStr, sizeof(speedStr), "%.1f MByte/sec", ((double)speed * 1000) / (1024 * 1024)); }
    else { SNPRINTF(speedStr, sizeof(speedStr), "%.3f KByte/sec", ((double)speed * 1000) / 1024); }

    st_info("File Transfer done!! DID=%s,Local=%s:%d,Rmt=%s:%d,Mode=%s,ReadSize=%lu Byte%s,Time:%d.%03d sec, %s\n", DID, LanIP, LanPort, RmtIP, RmtPort, Mode, *SizeCounter, SizeStr, t_ms / 1000, t_ms % 1000, speedStr);

    if (buf) { free(buf); buf = NULL; }

    return 0;
#else
    return -1;
#endif
} // ft_Test

#if defined(WIN32DLL) || defined(WINDOWS)
    static DWORD WINAPI ThreadWrite(void *arg)
#elif defined(LINUX)
    static void *ThreadWrite(void *arg)
#endif
{
    if (!arg)
    {
        st_info("ThreadWrite exit for arg is NULL!!\n");
        my_Thread_exit(0);
    }

    // INT32 Channel = (INT32)((UINTp)arg);
    st_Session_CH *pSessionTemp = (st_Session_CH *)arg;
    int SessionID = pSessionTemp->SessionID;
    int Channel = pSessionTemp->Channel;
    unsigned long TotalTestSize = pSessionTemp->TotalTestSize;

    if (0 > SessionID)
    {
        st_info("ThreadWrite exit for Invalid SessionID(%d)!!\n", SessionID);
        my_Thread_exit(0);
    }

    if (0 > Channel || 7 < Channel)
    {
        st_info("ThreadWrite exit for Invalid Channel=%d !!\n", Channel);
        my_Thread_exit(0);
    }


    UCHAR *Buffer = NULL;

    INT32 ret = 0;
    INT32 Check_ret = 0;
    UINT32 WriteSize = 0;
    ULONG *TotalSizeCounter = &g_RW_Test_Info[Channel].TotalSize_Write;
    *TotalSizeCounter = 0;
    st_Time_Info t1, t2;
    // st_Time_Info t3, t4;
    my_GetCurrentTime(&t1);

#if 0
    Buffer = (UCHAR *)malloc(TEST_SIZE_ONE_WRITE_MODIFY + 1);

    if (!Buffer)
    {
        st_info("ThreadWrite Channel %d - malloc failed!!\n", Channel);
        my_Thread_exit(0);
    }

    for (INT32 i = 0 ; i < TEST_SIZE_ONE_WRITE_MODIFY; i++)
    {
        Buffer[i] = i % 251; //0~250
    }

    Buffer[TEST_SIZE_ONE_WRITE_MODIFY] = '\0';
    st_info("ThreadWrite Channel %d running ... \n", Channel);

    while (*TotalSizeCounter < TotalTestSize)
    {
        while (1)
        {
            // 在调用 PPCS_Write 之前一定要调用 PPCS_Check_Buffer 检测写缓存还有多少数据尚未发出去，需控制在一个合理范围，一般控制在 128KB/256KB 左右。
            Check_ret = PPCS_Check_Buffer(SessionID, Channel, &WriteSize, NULL);

            // st_debug("ThreadWrite PPCS_Check_Buffer: Session=%d,CH=%d,WriteSize=%d,ret=%d %s\n", SessionID, Channel, WriteSize, Check_ret, getP2PErrorCodeInfo(Check_ret));
            if (0 > Check_ret)
            {
                st_info("ThreadWrite PPCS_Check_Buffer: Session=%d,CH=%d,WriteSize=%d,ret=%d %s\n", SessionID, Channel, WriteSize, Check_ret, getP2PErrorCodeInfo(Check_ret));
                break;
            }

            // 写缓存的数据大小超过128KB/256KB，则需考虑延时缓一缓。
            // 如果发现 wsize 越来越大，可能网络状态很差，需要考虑一下丢帧或降码率，这是一个动态调整策略，非常重要!!
            // On device, Recommended CHECK_WRITE_THRESHOLD_SIZE == (128 or 256) * 1024 Byte. this sample set 1MB.
            if (CHECK_WRITE_THRESHOLD_SIZE > WriteSize && TotalTestSize > *TotalSizeCounter)
            {
                ret = PPCS_Write(SessionID, Channel, (CHAR *)Buffer, TEST_SIZE_ONE_WRITE_MODIFY);

                if (0 > ret)
                {
                    if (ERROR_PPCS_SESSION_CLOSED_TIMEOUT == ret)
                    {
                        st_info("ThreadWrite Session=%d,CH=%d,ret=%d, Session Closed TimeOUT!!\n", SessionID, Channel, ret);
                    }
                    else if (ERROR_PPCS_SESSION_CLOSED_REMOTE == ret)
                    {
                        st_info("ThreadWrite Session=%d,CH=%d,ret=%d, Session Remote Closed!!\n", SessionID, Channel, ret);
                    }
                    else { st_info("ThreadWrite Session=%d,CH=%d,ret=%d %s\n", SessionID, Channel, ret, getP2PErrorCodeInfo(ret)); }

                    break;
                }

                *TotalSizeCounter += ret; // PPCS_Write return ret >=0: Number of byte wirten.
            }
            //When PPCS_Check_Buffer return WriteSize equals 0, all the data in this channel is sent out
            else if (0 == WriteSize) { break; }
            else
            {
                // my_GetCurrentTime(&t3);
                mSleep(1);
                // my_GetCurrentTime(&t4);
                // st_info_T("after mSleep 1ms, TU_MS:%d\n", TU_MS(t3,t4));
            }
        }
    }


#else
    st_info("ThreadWrite Channel %d running ... \n", Channel);

    do
    {
        uint32_t malloc_size = ((TotalTestSize - *TotalSizeCounter < TEST_SIZE_ONE_WRITE_MODIFY) ? (TotalTestSize - *TotalSizeCounter) : TEST_SIZE_ONE_WRITE_MODIFY);
        uint32_t write_index = 0;

        Buffer = (UCHAR *)malloc(malloc_size + 1);

        if (!Buffer)
        {
            st_info("ThreadWrite Channel %d - malloc failed!!\n", Channel);
            my_Thread_exit(0);
        }

        for (INT32 i = 0 ; i < malloc_size; i++)
        {
            Buffer[i] = i % 251; //0~250
        }

        Buffer[malloc_size] = '\0';

        while (1)
        {

            // 在调用 PPCS_Write 之前一定要调用 PPCS_Check_Buffer 检测写缓存还有多少数据尚未发出去，需控制在一个合理范围，一般控制在 128KB/256KB 左右。
            Check_ret = PPCS_Check_Buffer(SessionID, Channel, &WriteSize, NULL);

            // st_debug("ThreadWrite PPCS_Check_Buffer: Session=%d,CH=%d,WriteSize=%d,ret=%d %s\n", SessionID, Channel, WriteSize, Check_ret, getP2PErrorCodeInfo(Check_ret));
            if (0 > Check_ret)
            {
                st_info("ThreadWrite PPCS_Check_Buffer: Session=%d,CH=%d,WriteSize=%d,ret=%d %s\n", SessionID, Channel, WriteSize, Check_ret, getP2PErrorCodeInfo(Check_ret));
                goto WRITE_FAIL;
                break;
            }

            // 写缓存的数据大小超过128KB/256KB，则需考虑延时缓一缓。
            // 如果发现 wsize 越来越大，可能网络状态很差，需要考虑一下丢帧或降码率，这是一个动态调整策略，非常重要!!
            // On device, Recommended CHECK_WRITE_THRESHOLD_SIZE == (128 or 256) * 1024 Byte. this sample set 1MB.
            if (malloc_size > write_index)
            {
                if (WriteSize < CHECK_WRITE_THRESHOLD_SIZE_MODIFY)
                {
                    ret = PPCS_Write(SessionID, Channel, (CHAR *)(Buffer + write_index), malloc_size - write_index);

                    if (0 > ret)
                    {
                        if (ERROR_PPCS_SESSION_CLOSED_TIMEOUT == ret)
                        {
                            st_info("ThreadWrite Session=%d,CH=%d,ret=%d, Session Closed TimeOUT!!\n", SessionID, Channel, ret);
                        }
                        else if (ERROR_PPCS_SESSION_CLOSED_REMOTE == ret)
                        {
                            st_info("ThreadWrite Session=%d,CH=%d,ret=%d, Session Remote Closed!!\n", SessionID, Channel, ret);
                        }
                        else { st_info("ThreadWrite Session=%d,CH=%d,ret=%d %s\n", SessionID, Channel, ret, getP2PErrorCodeInfo(ret)); }

                        goto WRITE_FAIL;
                        break;
                    }

                    *TotalSizeCounter += ret; // PPCS_Write return ret >=0: Number of byte wirten.
                    write_index += ret;
                }
                else
                {
                    mSleep(2);
                }
            }
            else
            {
                break;
            }
        }

        free(Buffer);
        Buffer = NULL;
    }
    while (*TotalSizeCounter < TotalTestSize);

    while (1)
    {
        Check_ret = PPCS_Check_Buffer(SessionID, Channel, &WriteSize, NULL);

        // st_debug("ThreadWrite PPCS_Check_Buffer: Session=%d,CH=%d,WriteSize=%d,ret=%d %s\n", SessionID, Channel, WriteSize, Check_ret, getP2PErrorCodeInfo(Check_ret));
        if (0 > Check_ret)
        {
            st_info("ThreadWrite PPCS_Check_Buffer: Session=%d,CH=%d,WriteSize=%d,ret=%d %s\n", SessionID, Channel, WriteSize, Check_ret, getP2PErrorCodeInfo(Check_ret));
            goto WRITE_FAIL;
            break;
        }

        if (WriteSize != 0)
        {
            mSleep(2);
        }
        else
        {
            break;
        }
    }

#endif
    {
        if (Buffer) { free(Buffer); Buffer = NULL;}

        my_GetCurrentTime(&t2);


        int t_ms = TU_MS(t1, t2);
        g_RW_Test_Info[Channel].Tick_Used_Write = t_ms;
        // g_RW_Test_Info[Channel].TotalSize_Write = TotalSizeCounter;

        float speed = (0 == t_ms) ? 0 : ((double)(*TotalSizeCounter) / t_ms);
        char speedStr[32] = {};

        if (1024 * 1024 <= speed) { SNPRINTF(speedStr, sizeof(speedStr), "%.1f GByte/sec", ((double)speed * 1000) / (1024 * 1024 * 1024)); }
        else if (1024 <= speed) { SNPRINTF(speedStr, sizeof(speedStr), "%.1f MByte/sec", ((double)speed * 1000) / (1024 * 1024)); }
        else { SNPRINTF(speedStr, sizeof(speedStr), "%.3f KByte/sec", ((double)speed * 1000) / 1024); }

        st_debug("ThreadWrite Channel %d success Exit. Total: %lu Byte (%.2f MB), Time:%3d.%03d sec, %s\n", Channel, *TotalSizeCounter, (double)(*TotalSizeCounter) / (1024 * 1024), t_ms / 1000, t_ms % 1000, speedStr);

        my_Thread_exit(0);
    }
WRITE_FAIL:;

    if (Buffer) { free(Buffer); Buffer = NULL;}

    st_debug("ThreadWrite Channel %d fail.\n", Channel);
    my_Thread_exit(0);

} // ThreadWrite

#if defined(WIN32DLL) || defined(WINDOWS)
    static DWORD WINAPI ThreadRead(void *arg)
#elif defined(LINUX)
    static void *ThreadRead(void *arg)
#endif
{
    if (!arg)
    {
        st_info("ThreadRead exit for arg is NULL!!\n");
        my_Thread_exit(0);
    }

    // INT32 Channel = (INT32)((UINTp)arg);
    st_Session_CH *pSessionTemp = (st_Session_CH *)arg;
    int SessionID = pSessionTemp->SessionID;
    int Channel = pSessionTemp->Channel;
    unsigned long TotalTestSize = pSessionTemp->TotalTestSize;

    if (0 > SessionID)
    {
        st_info("ThreadRead exit for Invalid SessionID(%d)!!\n", SessionID);
        my_Thread_exit(0);
    }

    if (0 > Channel || 7 < Channel)
    {
        st_info("ThreadRead exit for Invalid Channel=%d !!\n", Channel);
        my_Thread_exit(0);
    }

    st_info("ThreadRead  Channel %d running ... \n", Channel);

    int ret = -99;
    int ChkSize = 0;
    ULONG *TotalSizeCounter = &g_RW_Test_Info[Channel].TotalSize_Read;
    *TotalSizeCounter = 0;
    int TimeOut_ms = 800;
    int SizeToRead = 1024 * 1024;
    int BufSize = SizeToRead + 4;
    unsigned char *buf = (unsigned char *)malloc(BufSize);

    if (!buf)
    {
        st_info("ThreadRead exit for malloc buf failed!!\n");
        my_Thread_exit(0);
    }

    st_Time_Info t1, t2;
    // st_Time_Info t3, t4;
    my_GetCurrentTime(&t1);

    while (1)
    {
        memset(buf, 0, BufSize);
        int SizeCounter = 0;

        while (1)
        {
            // my_GetCurrentTime(&t3);
            int ReadSize = SizeToRead - SizeCounter; // ReadSize: 期望要读取的数据大小，在每次 PPCS_Read 之前 ReadSzie 必须指定要读取的大小，非常重要!!
            ret = PPCS_Read(SessionID, Channel, (char *)buf + SizeCounter, &ReadSize, TimeOut_ms);

            // my_GetCurrentTime(&t4);
            // st_debug("PPCS_Read: SessionID=%d,CH=%d,ReadSize=%d,TotalSizeCounter=%lu Byte,z0=%d,ret=%d [%s]\n", SessionID, Channel, ReadSize, *TotalSizeCounter, buf[0], ret, getP2PErrorCodeInfo(ret));
            // PPCS_Read 返回 ERROR_PPCS_TIME_OUT(-3) :
            // 1. PPCS_Read 返回 -3 超时错误是正常现象，只是在设定超时时间内读取不到预设的大小，并非断线错误，需继续循环读取剩余数据。
            // 2. PPCS_Read 返回 -3 超时也有可能读到部分数据，需要对 ReadSize 做大小检测判断，本 sample code 直接累加 ReadSize 大小。
            // 3. ReadSzie 是实际反映读取的数据大小，若 PPCS_Read 读取不到数据，ReadSzie 变量会被清零，下一次 PPCS_Read 之前一定要重新赋值，否则 ReadSzie 传 0 进 PPCS_Read 会返回-5 错误（-5：无效的参数）。
            if (ERROR_PPCS_INVALID_PARAMETER != ret && ERROR_PPCS_INVALID_SESSION_HANDLE != ret && 0 < ReadSize)
            {
                SizeCounter += ReadSize;
                *TotalSizeCounter += ReadSize;
            }

            if (SizeToRead == SizeCounter || TotalTestSize == *TotalSizeCounter) { break; }

            if (ERROR_PPCS_SUCCESSFUL != ret)
            {
                if (ERROR_PPCS_TIME_OUT == ret) { continue; } // 超时，继续读。

                if (ERROR_PPCS_SESSION_CLOSED_TIMEOUT == ret) // 网络差导致断线。
                {
                    st_info("\nPPCS_Read  Session=%d,CH=%d,ReadSize=%d Byte,TotalSizeCounter=%lu Byte,ret=%d, Session Closed TimeOUT!!\n", SessionID, Channel, ReadSize, *TotalSizeCounter, ret);
                }
                else if (ERROR_PPCS_SESSION_CLOSED_REMOTE == ret) // 对方主动关闭连接。
                {
                    st_info("\nPPCS_Read  Session=%d,CH=%d,ReadSize=%d Byte,TotalSizeCounter=%lu Byte,ret=%d, Session Remote Closed!!\n", SessionID, Channel, ReadSize, *TotalSizeCounter, ret);
                }
                else { st_info("\nPPCS_Read  Session=%d,CH=%d,ReadSize=%d Byte,TotalSizeCounter=%lu Byte,ret=%d [%s]\n", SessionID, Channel, ReadSize, *TotalSizeCounter, ret, getP2PErrorCodeInfo(ret)); } // 其他错误

                break;
            }
        } // while 1

        //// 数据校验
        for (int i = ChkSize, j = 0; j < SizeCounter; i += (SizeCounter / 100), j += (SizeCounter / 100))
        {
            if (i % 251 != (buf[j] & 0xFF)) //TotalSizeCounter%251: 0~250, zz: 0~250
            {
                st_info("\n@@@@@@@@@@@@@@@@ PPCS_Read ret=%d,Session=%d,CH=%d,SizeCounter=%d,TotalSizeCounter=%lu,%d%%251=%d,buf[%d]=%d\n", ret, SessionID, Channel, SizeCounter, *TotalSizeCounter, i, i % 251, j, buf[j]);
                break;
            }
        }

        ChkSize = *TotalSizeCounter;

        //// 每传输 1 MB 屏幕打印一次
        if ((*TotalSizeCounter) % SizeToRead == (*TotalSizeCounter) % (1 * 1024 * 1024))
        {
            st_info("%d", Channel);
            setbuf(stdout, NULL);
        }

        if ((ERROR_PPCS_SUCCESSFUL != ret && ERROR_PPCS_TIME_OUT != ret) || (TotalTestSize == *TotalSizeCounter)) { break; }
    }

    my_GetCurrentTime(&t2);

    if (buf) {free(buf); buf = NULL;}

    int t_ms = TU_MS(t1, t2);
    g_RW_Test_Info[Channel].Tick_Used_Read = t_ms;
    // g_RW_Test_Info[Channel].TotalSize_Read = TotalSizeCounter;

    float speed = (0 == t_ms) ? 0 : ((double)(*TotalSizeCounter) / t_ms);
    char speedStr[32] = {};

    if (1024 * 1024 <= speed) { SNPRINTF(speedStr, sizeof(speedStr), "%.1f GByte/sec", ((double)speed * 1000) / (1024 * 1024 * 1024)); }
    else if (1024 <= speed) { SNPRINTF(speedStr, sizeof(speedStr), "%.1f MByte/sec", ((double)speed * 1000) / (1024 * 1024)); }
    else { SNPRINTF(speedStr, sizeof(speedStr), "%.3f KByte/sec", ((double)speed * 1000) / 1024); }

    st_debug("ThreadRead  Channel %d Exit - Total: %lu Byte (%.2f MB), Time:%3d.%03d sec, %s\n", Channel, *TotalSizeCounter, (double)(*TotalSizeCounter) / (1024 * 1024), t_ms / 1000, t_ms % 1000, speedStr);

    my_Thread_exit(0);
} // ThreadRead

static unsigned long getTotalTestSize(int SizeOption)
{
    switch (SizeOption)
    {
    case 0: return TEST_SIZE_1;

    case 1: return TEST_SIZE_10;

    case 2: return TEST_SIZE_100;

    case 3: return TEST_SIZE_1000;

    default: return TEST_SIZE_100;
    }
}


static const uint16_t crc_table[] =
{
    0x0000, 0x1081, 0x2102, 0x3183,
    0x4204, 0x5285, 0x6306, 0x7387,
    0x8408, 0x9489, 0xa50a, 0xb58b,
    0xc60c, 0xd68d, 0xe70e, 0xf78f
};

static void updata_crc(uint16_t *crc, uint8_t data)
{
    uint16_t reg = *crc;

    reg = (reg >> 4) ^ crc_table[(reg ^ data) & 0x000f];
    reg = (reg >> 4) ^ crc_table[(reg ^ (data >> 4)) & 0x000f];

    *crc = reg;
}

static void *pic_recver_task(void *arg)
{
    int SessionID = (typeof(SessionID))arg, Channel = 0;
    uint8_t *buf = NULL;
    int SizeToRead = 0;
    const uint32_t package_head_magic = 0x81021791;
    int ret = 0;
    int TimeOut_ms = 800;
    int SizeCounter = 0;
    int sizecounter_payload_crc = 0;
    /*
    magic 4 bytes
    sequnce num 4 bytes
    len 4 bytes: payload len
    header checksum 1bytes
    payload:
    payload crc 2 bytes
    */

    uint8_t header[4 + 4 + 4 + 1] = {0};
    uint8_t hrs = 0;
    uint16_t crc = 0xffff, peer_crc = 0;

    uint32_t seqnum = 0;
    uint32_t package_len = 0;


    int ReadSize = sizeof(header);

    buf = (typeof(buf))calloc(1, 1024 * 100);

    if (!buf)
    {
        st_info("%s calloc err\n", __func__);
        return (void *) -1;
    }

    do
    {
        SizeCounter = 0;

        do
        {
            ReadSize = sizeof(header) - SizeCounter;
            ret = PPCS_Read(SessionID, Channel, (char *)header + SizeCounter, &ReadSize, TimeOut_ms);


            if (ret < 0 && ERROR_PPCS_TIME_OUT != ret)
            {
                st_info("\nPPCS_Read  Session=%d,CH=%d, err ret=%d [%s]\n", SessionID, Channel, ret, getP2PErrorCodeInfo(ret)); // 其他错误
                return (void *) -1;
            }


            if (ERROR_PPCS_TIME_OUT == ret || ret == 0)
            {
                SizeCounter += ReadSize;
            }
        }
        while (SizeCounter < sizeof(header));

        if (memcmp(header, &package_head_magic, sizeof(package_head_magic)))
        {
            os_printf("%s magic not match 0x%X\n", __func__, *(uint32_t *)header);
            continue;
        }
        else
        {
            for (uint32_t i = 0; i < sizeof(header) - 1; ++i)
            {
                hrs += header[i];
            }

            if ( header[sizeof(header) - 1] != (uint8_t)~(hrs & 0xff))
            {
                os_printf("%s header checksum not match\n", __func__);
                continue;
            }

            memcpy(&seqnum, header + 4, sizeof(seqnum));
            memcpy(&package_len, header + 8, sizeof(package_len));
            break;
        }
    }
    while (1);


    if (package_len > 1024 * 100)
    {
        os_printf("%s recv len too long %d\n", __func__, package_len);
        return (void *) -1;
    }

    SizeToRead = package_len + 2; //+ crc

    while (1)
    {
        do
        {
            ReadSize = SizeToRead - sizecounter_payload_crc;
            ret = PPCS_Read(SessionID, Channel, (char *)buf + sizecounter_payload_crc, &ReadSize, TimeOut_ms);

            if (ret < 0 && ERROR_PPCS_TIME_OUT != ret)
            {
                st_info("\nPPCS_Read  Session=%d,CH=%d, err ret=%d [%s]\n", SessionID, Channel, ret, getP2PErrorCodeInfo(ret)); // 其他错误
                return (void *) -1;
            }

            if (ERROR_PPCS_TIME_OUT == ret || ret == 0)
            {
                sizecounter_payload_crc += ReadSize;
            }
        }
        while (sizecounter_payload_crc < SizeToRead);


        for (int i = 0; i < sizeof(header); ++i)
        {
            updata_crc(&crc, header[i]);
        }

        for (int i = 0; i < package_len; ++i)
        {
            updata_crc(&crc, buf[i]);
        }


        memcpy(&peer_crc, &buf[package_len], sizeof(peer_crc));

        if (crc != peer_crc)
        {
            os_printf("%s crc err need 0x%X peer 0x%X\n", __func__, crc, peer_crc);

            if (buf) { os_free(buf); }

            return (void *) -1;
        }

        os_printf("%s recv pic size %d\n", __func__, package_len);
        break;

    }

    if (buf) { os_free(buf); }

    return (void *)0;
}


static int pic_recver_create_task(int SessionID, int ThreadNum)
{
    pthread_t id;

    if (0 > pthread_create(&id, NULL, pic_recver_task, (void *)SessionID))
    {
        os_printf("%s create err\n", __func__);
    }

    pthread_join(id, NULL);

    return 0;
}


static int RW_Test(int SessionID, int ThreadNum, int SizeOption, int DirectionMode)
{
    if (0 > SessionID)
    {
        st_info("RW_Test exit for Invalid SessionID(%d)!!\n", SessionID);
        return -1;
    }

    if (0 > ThreadNum || 8 < ThreadNum)
    {
        st_info("RW_Test exit for Invalid ThreadNum(%d)!!\n", ThreadNum);
        return -1;
    }

    st_info("ThreadNum=%d\n", ThreadNum);
    st_info("SizeOption=%d -> TotalTestSize=%lu Byte (%.02f MByte)\n", SizeOption, getTotalTestSize(SizeOption), (double)getTotalTestSize(SizeOption) / (1024 * 1024));
    st_info("DirectionMode=%d (%s)\n", DirectionMode, (1 == DirectionMode) ? "D -> C" : (2 == DirectionMode) ? "C -> D" : "D <-> C");
    unsigned long TotalTestSize = getTotalTestSize(SizeOption);
    char Flags_TestWrite = 1;
    char Flags_TestRead = 1;

    switch (DirectionMode)
    {
    case 1:
    {
        Flags_TestWrite = 0;
        Flags_TestRead = 1;
        break;
    }

    case 2:
    {
        Flags_TestWrite = 1;
        Flags_TestRead = 0;
        break;
    }

    default: break;
    }

#if defined(WIN32DLL) || defined(WINDOWS)
    HANDLE hThreadWrite[NUMBER_OF_P2PCHANNEL];
    HANDLE hThreadRead[NUMBER_OF_P2PCHANNEL];
#elif defined(LINUX)
    pthread_t ThreadWriteID[NUMBER_OF_P2PCHANNEL];
    pthread_t ThreadReadID[NUMBER_OF_P2PCHANNEL];
#endif

    st_Session_CH *SessionTemp = (st_Session_CH *)malloc(sizeof(st_Session_CH) * ThreadNum);

    if (!SessionTemp)
    {
        error("RW_Test - malloc SessionTemp Buffer failed!!\n");
    }

    memset(SessionTemp, 0, sizeof(st_Session_CH)*ThreadNum);

    memset(g_RW_Test_Info, 0, sizeof(g_RW_Test_Info));

    for (INT32 i = 0; i < ThreadNum; i++)
    {
        SessionTemp[i].SessionID = SessionID;
        SessionTemp[i].Channel = i;
        SessionTemp[i].TotalTestSize = TotalTestSize;

#if defined(WIN32DLL) || defined(WINDOWS)

        if ( 1 == Flags_TestWrite && NULL == (hThreadWrite[i] = CreateThread(NULL, 0, ThreadWrite, (void *)&SessionTemp[i], 0, NULL)) )
        {
            if (SessionTemp) { free(SessionTemp); SessionTemp = NULL; };

            error("create ThreadWrite failed");
        }

        if ( 1 == Flags_TestRead && NULL == (hThreadRead[i] = CreateThread(NULL, 0, ThreadRead, (void *)&SessionTemp[i], 0, NULL)) )
        {
            if (SessionTemp) { free(SessionTemp); SessionTemp = NULL; };

            error("create ThreadRead failed");
        }

#elif defined(LINUX)

        if ( 1 == Flags_TestWrite && 0 != pthread_create(&ThreadWriteID[i], NULL, &ThreadWrite, (void *)&SessionTemp[i]) )
        {
            if (SessionTemp) { free(SessionTemp); SessionTemp = NULL; };

            error("create ThreadWrite failed");
        }

        if ( 1 == Flags_TestRead && 0 != pthread_create(&ThreadReadID[i], NULL, &ThreadRead, (void *)&SessionTemp[i]) )
        {
            if (SessionTemp) { free(SessionTemp); SessionTemp = NULL; };

            error("create ThreadRead failed");
        }

#endif
    } // for

    //// create thread to check buffer every 1 sec.
    CreateThread_Check_Buffer(SessionID, 0, ThreadNum);

    for (INT32 i = 0; i < ThreadNum; i++)
    {
#if defined(WIN32DLL) || defined(WINDOWS)

        if (1 == Flags_TestWrite)
        {
            WaitForSingleObject(hThreadWrite[i], INFINITE);
            CloseHandle(hThreadWrite[i]);
        }

        if (1 == Flags_TestRead)
        {
            WaitForSingleObject(hThreadRead[i], INFINITE);
            CloseHandle(hThreadRead[i]);
        }

#elif defined(LINUX)

        if (1 == Flags_TestWrite) { pthread_join(ThreadWriteID[i], NULL); }

        if (1 == Flags_TestRead) { pthread_join(ThreadReadID[i], NULL); }

#endif
    }

    st_info("\n");
    gThread_CheckBuffer_Exit = 1;

    char *DID = g_MySInfo.DID;
    char *RmtIP = g_MySInfo.RemoteIP;
    int RmtPort = g_MySInfo.RemotePort;
    char *LanIP = g_MySInfo.MyLocalIP;
    int LanPort = g_MySInfo.MyLocalPort;
    char *Mode = g_MySInfo.Mode;

    // show transmission information for each channel
    UINT32 tick = 0;
    ULONG TotalSize = 0;

    for (int i = 0; i < ThreadNum; i++)
    {
        // write info
        if (0 == Flags_TestRead)
        {
            tick = g_RW_Test_Info[i].Tick_Used_Write;
            TotalSize = g_RW_Test_Info[i].TotalSize_Write;
            float speed = (0 == tick) ? 0 : ((double)TotalSize / tick);
            char speedStr[32] = {};

            if (1024 * 1024 <= speed) { SNPRINTF(speedStr, sizeof(speedStr), "%.1f GByte/sec", ((double)speed * 1000) / (1024 * 1024 * 1024)); }
            else if (1024 <= speed) { SNPRINTF(speedStr, sizeof(speedStr), "%.1f MByte/sec", ((double)speed * 1000) / (1024 * 1024)); }
            else { SNPRINTF(speedStr, sizeof(speedStr), "%.3f KByte/sec", ((double)speed * 1000) / 1024); }

            st_info("ThreadWrite CH %d Done - DID=%s,Local=%s:%d,Rmt=%s:%d,Mode=%s,Total:%lu Byte(%.2f MB),Time:%d.%03d sec, %s\n", i, DID, LanIP, LanPort, RmtIP, RmtPort, Mode, TotalSize, (double)TotalSize / (1024 * 1024), tick / 1000, tick % 1000, speedStr);
        }

        // read info
        if (1 == Flags_TestRead)
        {
            tick = g_RW_Test_Info[i].Tick_Used_Read;
            TotalSize = g_RW_Test_Info[i].TotalSize_Read;
            float speed = (0 == tick) ? 0 : ((double)TotalSize / tick);
            char speedStr[32] = {};

            if (1024 * 1024 <= speed) { SNPRINTF(speedStr, sizeof(speedStr), "%.1f GByte/sec", ((double)speed * 1000) / (1024 * 1024 * 1024)); }
            else if (1024 <= speed) { SNPRINTF(speedStr, sizeof(speedStr), "%.1f MByte/sec", ((double)speed * 1000) / (1024 * 1024)); }
            else { SNPRINTF(speedStr, sizeof(speedStr), "%.3f KByte/sec", ((double)speed * 1000) / 1024); }

            st_info("ThreadRead  CH %d Done - DID=%s,Local=%s:%d,Rmt=%s:%d,Mode=%s,Total:%lu Byte(%.2f MB),Time:%d.%03d sec, %s\n", i, DID, LanIP, LanPort, RmtIP, RmtPort, Mode, TotalSize, (double)TotalSize / (1024 * 1024), tick / 1000, tick % 1000, speedStr);
        }

        setbuf(stdout, NULL);
    } // for

    if (SessionTemp) { free(SessionTemp); SessionTemp = NULL; };

    return 0;
} // RW_Test

static void pkt_Test(int SessionID)
{
#ifdef  PKT_TEST
    int Counter = 0;
    CHAR ExpectValue = 0; // Expect Value
    UINT32 TimeOut_ms = 30;//0xFFFFFFFF;

    st_info("PPCS_PktRecv ...\n\n");

    while (1)
    {
        CHAR PktBuf[1024] = {};
        memset(PktBuf, 0, sizeof(PktBuf));
        INT32 PktSize = sizeof(PktBuf);
        int ret = PPCS_PktRecv(SessionID, CH_DATA, PktBuf, &PktSize, TimeOut_ms);

        if (0 > ret)
        {
            if (ERROR_PPCS_TIME_OUT == ret) { continue; }

            if (ERROR_PPCS_SESSION_CLOSED_TIMEOUT == ret)
            {
                st_info("Session Closed TimeOUT!!\n");
            }
            else if (ERROR_PPCS_SESSION_CLOSED_REMOTE == ret)
            {
                st_info("Session Remote Closed!!\n");
            }
            else
            {
                st_info("PPCS_PktRecv: Session=%d,channel=%d,PktSize=%d,ret=%d: %s\n", SessionID, CH_DATA, PktSize, ret, getP2PErrorCodeInfo(ret));
            }

            break;
        }

        if (PktSize != 1024) //// we send 1024 Byte packet
        {
            st_info("Packet size error!! PktSize=%d, should be 1024\n", PktSize);
        }
        else
        {
            st_debug("PPCS_PktRecv: ret=%d,Session=%d,channel=%d,data=%d...,size=%d\n", ret, SessionID, CH_DATA, PktBuf[0], PktSize);
        }

        setbuf(stdout, NULL);

        if (ExpectValue != PktBuf[0])
        {
            st_info("Packet Lost Detect!! Value = %d (should be %d)\n", PktBuf[0], ExpectValue);
            ExpectValue = (PktBuf[0] + 1) % 100;
        }
        else { ExpectValue = (ExpectValue + 1) % 100; }

        if (Counter % 100 == 99)
        {
            st_info("TotalRecv %d packets. (1 packets=%u Byte)\n", Counter + 1, (unsigned)sizeof(PktBuf));
        }

        fflush(stdout);
        Counter++;
    }

#endif
} // pkt_Test


int readwritetester_main(int argc, char **argv)
{
#ifdef DEBUG
    g_ST_INFO_OPEN = 1;
#else
    g_ST_INFO_OPEN = 0;
#endif

    //// 1. get P2P API Version
    UINT32 APIVersion = PPCS_GetAPIVersion();
    char VerBuf[24] = {};
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

    if (4 > argc || 7 < argc)
    {
        printf("Usage: %s Mode DID InitString [ThreadNum] [SizeOption] [DirectionMode]\n", argv[0]);
        printf("   Or: %s Mode DID InitString:P2PKey [ThreadNum] [SizeOption] [DirectionMode]\n", argv[0]);
        printf("With P2P DSK:\n        %s Mode DID:DSKey InitString [ThreadNum] [SizeOption] [DirectionMode]\n", argv[0]);
        printf("    Or: %s Mode DID:DSKey InitString:P2PKey [ThreadNum] [SizeOption] [DirectionMode]\n\n", argv[0]);

        printf("\tMode: 0->File transfer test, D -> C.\n");
        printf("\t      1->Bidirectional Read Write test.\n");
#ifdef  PKT_TEST
        printf("\t      2->PktRecv/PktSend test, D -> C. (***NOTE***: TCP Relay mode does not Support PPCS_PktSend/PPCS_PktRecv API to send and receive data!!)\n");
#endif
        printf("\t[ThreadNum]: thread Number of Bidirectional Read Write test, Num:1-8\n");
        printf("\t[SizeOption]: Transmission data size, if not specify this value, default transmission amount is about 107MByte.\n");
        printf("\t    default. about 107MB.\n");
        printf("\t    SizeOption=0. about 1MByte.\n");
        printf("\t    SizeOption=1. about 10MByte.\n");
        printf("\t    SizeOption=2. about 107MByte.\n");
        printf("\t    SizeOption=3. about 1004MByte.\n");
        printf("\t[DirectionMode]: Transmission direction Mode, if not Specify this value, default Bidirectional.\n");
        printf("\t    default       0. D <-> C: ListenTester call PPCS_Write/PPCS_Read <-> ReadWriteTester call PPCS_Read/PPCS_Write.\n");
        printf("\t    DirectionMode=1. D -> C: ListenTester call PPCS_Write -> ReadWriteTester call PPCS_Read.\n");
        printf("\t    DirectionMode=2. C -> D: ReadWriteTester call PPCS_Write -> ListenTester call PPCS_Read.\n");
        return 0;
    }

    char HaveDSK = 0;
    char DID[SIZE_DID] = {};
    char DSKey[24] = {};
    char DIDString[128] = {};
    char InitString[SIZE_INITSTRING] = {};

    char Mode = (char)atoi(argv[1]);

    if (0 > Mode || 2 < Mode) { printf("No this test Mode:%d!\n", Mode); return 0; }

    const char *pDIDString = argv[2];
    const char *p = strstr(pDIDString, ":");

    if (p)
    {
        memcpy(DID, pDIDString, (int)(p - pDIDString));
        memcpy(DSKey, p + 1, strlen(p + 1));
        HaveDSK = 1;
    }

    if (p) { memcpy(DID, pDIDString, (int)(p - pDIDString)); }
    else { memcpy(DID, pDIDString, strlen(pDIDString)); }

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

    memcpy(DIDString, pDIDString, strlen(pDIDString));
    memcpy(InitString, argv[3], strlen(argv[3]));
    char ThreadNum = 1;

    if (5 <= argc) { ThreadNum = atoi(argv[4]); }

    if (1 > ThreadNum) { ThreadNum = 1; }

    if (8 < ThreadNum) { ThreadNum = 8; }

    int SizeOption = 2;

    if (6 <= argc)
    {
        SizeOption = (char)atoi(argv[5]);

        if (0 > SizeOption || 3 < SizeOption) { printf("No this Size Option:%d!\n", SizeOption); return 0; }
    }

    int DirectionMode = 0;

    if (7 <= argc)
    {
        DirectionMode = (char)atoi(argv[6]);

        if (0 > DirectionMode || 3 < DirectionMode) { printf("No this Direction Mode:%d!\n", DirectionMode); return 0; }
    }

    st_info("Mode=%d (%s)\n", Mode, (0 == Mode) ? "File transfer test, D -> C" : (1 == Mode) ? "Bidirectional Read Write test" : "PktRecv/PktSend test, D -> C");
    st_info("DID=%s\n", DID);

    if (HaveDSK) { st_info("DSKey=%s\n", DSKey); }

    st_info("InitString=%s\n", InitString);

    if (1 == Mode)
    {
        st_info("ThreadNum=%d\n", ThreadNum);
        st_info("SizeOption=%d -> TotalTestSize=%lu Byte (%.02f MByte)\n", SizeOption, getTotalTestSize(SizeOption), (double)getTotalTestSize(SizeOption) / (1024 * 1024));
        st_info("DirectionMode=%d (%s)\n", DirectionMode, (1 == DirectionMode) ? "D -> C" : (2 == DirectionMode) ? "C -> D" : "D <-> C");
    }

    CreateThread_UDP_RECV_ECHO();

    st_Time_Info t1, t2;
    char tBuf[] = "[YYYY-MM-DD hh:mm:ss.xxx] ";
    int ret = -1;

    // 2. P2P Initialize
    if (0 <= strncmp(VerBuf, "4.2.0.0", 5)) // PPCS_Initialize JsonString by Version>=4.2.0
    {
        int MaxNumSess = 2; // Max Number Session: 1~256.
        int SessAliveSec = 6; // session timeout close alive: 6~30.
        char InitJsonString[256] = {};
        SNPRINTF(InitJsonString, sizeof(InitJsonString), "{\"InitString\":\"%s\",\"MaxNumSess\":%d,\"SessAliveSec\":%d}", InitString, MaxNumSess, SessAliveSec);
        // st_debug("InitJsonString=%s\n",InitJsonString);
        my_GetCurrentTime(&t1);
        st_debug("[%s] PPCS_Initialize(%s) ...\n", t1.Date, InitJsonString);
        // 如果Parameter 不是正确的JSON字串则会被当成InitString[:P2PKey]来处理, 如此以兼容旧版.
        //        ret = PPCS_Initialize((char *)InitJsonString);
        ret = PPCS_Initialize(InitString); //CS2 fae表示，如遇到-22错误，可传入InitString
        my_GetCurrentTime(&t2);
        st_info("[%s] PPCS_Initialize(%s): ret=%d, t:%d ms\n", t2.Date, InitJsonString, ret, TU_MS(t1, t2));

        if (ERROR_PPCS_SUCCESSFUL != ret && ERROR_PPCS_ALREADY_INITIALIZED != ret)
        {
            return 0;
        }
    }
    else
    {
        my_GetCurrentTime(&t1);
        st_debug("[%s] PPCS_Initialize(%s) ...\n", t1.Date, InitString);
        ret = PPCS_Initialize((char *)InitString);
        my_GetCurrentTime(&t2);
        st_info("[%s] PPCS_Initialize(%s): ret=%d, t:%d ms\n", t2.Date, InitString, ret, TU_MS(t1, t2));

        if (ERROR_PPCS_SUCCESSFUL != ret && ERROR_PPCS_ALREADY_INITIALIZED != ret)
        {
            return 0;
        }
    }

    if (0 <= strncmp(VerBuf, "3.5.0.0", 5))
    {
        const char *APIInformation = PPCS_GetAPIInformation();
        st_info_T("PPCS_GetAPIInformation(%u Byte):%s\n", strlen(APIInformation), APIInformation);
    }
#if 1// must detect because exception !!!!
    // 3. Network Detect
    st_PPCS_NetInfo NetInfo;
    my_GetCurrentTime(&t1);
    ret = PPCS_NetworkDetect(&NetInfo, 0);
    my_GetCurrentTime(&t2);
    st_info("[%s] PPCS_NetworkDetect: %d ms, ret=%d [%s]\n", t2.Date, TU_MS(t1, t2), ret, getP2PErrorCodeInfo(ret));
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
#endif
    int Running = 0;
    int SessionID = -99;
    char bEnableLanSearch = (0 <= strncmp(VerBuf, "4.1.0.0", 5)) ? 0x7A : 0x7E;
    // char isLan = 0;
#if 1
    st_PPCS_Session Sinfo;

    // 4. Connect to Device.
    for (int i = 0, times = 3; i < times; i++)
    {
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
            else if(0)
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

        // SNPRINTF(ByServerString, sizeof(ByServerString), "{\"InitString\":\"%s\",\"0x7X_Timeout\":%d}",InitString, connTimeout);
        //bEnableLanSearch = 0x5e;//connectiontest mode 6 default, very fast, 0x7a is very slow
        st_info("[%s] PPCS_ConnectByServer(%s,0x%02X,0,ByServerStringlen %d) ...\n", t1.Date, DIDString, bEnableLanSearch, strlen(ByServerString));
        ret = PPCS_ConnectByServer(DIDString, bEnableLanSearch, 0, ByServerString);
        my_GetCurrentTime(&t2);

        if (0 > ret)
        {
            st_info("[%s] PPCS_ConnectByServer(%s,0x%02X,0,ByServerStr"
                    "inglen %d) Failed: %d ms, %d [%s]\n", t2.Date, DIDString, bEnableLanSearch, strlen(ByServerString), TU_MS(t1, t2), ret, getP2PErrorCodeInfo(ret));

            // st_info("[%s] PPCS_Connect(%s,0x%02X,0) Failed: %d ms, %d [%s]\n", t2.Date, DIDString, bEnableLanSearch, TU_MS(t1,t2), ret, getP2PErrorCodeInfo(ret));
            if (ERROR_PPCS_TIME_OUT == ret || ERROR_PPCS_FAILED_TO_CONNECT_TCP_RELAY == ret) { continue; }
            else { break; }
        }
        else
        {
            SessionID = ret;
            st_info("[%s] PPCS_ConnectByServer(%s,0x%02X,0,ByServerStringlen %d) Success!! t:%d ms, Session=%d\n", t2.Date, DIDString, bEnableLanSearch, strlen(ByServerString), TU_MS(t1, t2), SessionID);
            // st_info("[%s] PPCS_Connect(%s,0x%02X,0) Success!! %d ms, Session=%d\n", t2.Date, DIDString, bEnableLanSearch, TU_MS(t1,t2), SessionID);

            ret = PPCS_Check(SessionID, &Sinfo);

            if (ERROR_PPCS_SUCCESSFUL == ret)
            {
                Running = 1;
                char RmtIP[16] = {};
                SNPRINTF(RmtIP, sizeof(RmtIP), "%s", inet_ntoa(Sinfo.RemoteAddr.sin_addr));
                char MyLanIP[16] = {};
                SNPRINTF(MyLanIP, sizeof(MyLanIP), "%s", inet_ntoa(Sinfo.MyLocalAddr.sin_addr));
                const char *pMode = NULL;

                if (0 == Sinfo.bMode)
                {
                    if (Sinfo.RemoteAddr.sin_addr.s_addr == Sinfo.MyLocalAddr.sin_addr.s_addr || 1 == isLANcmp(MyLanIP, RmtIP))
                    {
                        (1 == GetSocketType(Sinfo.Skt)) ? (pMode = "LAN.") : (pMode = "LAN");
                        CreateThread_UDP_Ping(RmtIP);
                    }
                    else { pMode = "P2P"; }
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

                if (1 == Sinfo.bMode && 1 == AllowRP2P) { CreateThread_PPCS_Check(SessionID); } //// AllowRP2P=1 and mode is RLY, then create ppcs check thread to check whether the mode is changed to RP2P
            }
        }

        break;
    } // for

    UINT32 timeOut_ms = 1000;

    if (2 == Sinfo.bMode) { timeOut_ms = 3500; }

    // 5. do job ...
    for (int i = 0, times = 2; i < times && Running; i++)
    {
        // 读取 ACK 值。
        char ReadData = -99;
        int ReadSize = 1;
        GetTime_ms(&t1, tBuf, sizeof(tBuf));
        // st_debug("%s PPCS_Read(SessionID=%d,CH=%d,ReadSize=%d,timeOut_ms=%d) ...\n", tBuf, SessionID, CH_CMD, ReadSize, timeOut_ms);
        ret = PPCS_Read(SessionID, CH_CMD, (char *)&ReadData, &ReadSize, timeOut_ms);
        GetTime_ms(&t2, tBuf, sizeof(tBuf));

        if (0 > ret && 0 == ReadSize)
        {
            if (ERROR_PPCS_TIME_OUT == ret)
            {
                if (times - 1 == i)
                {
                    st_info_T("PPCS_Read: Read Ack timeout!! t:%d ms,Session=%d,CH=%d,ReadSize=%d,ret=%d [%s]\n", TU_MS(t1, t2), SessionID, CH_CMD, ReadSize, ret, getP2PErrorCodeInfo(ret));
                    Running = 0;
                }

                continue; // 读取超时，读不到数据，但非断线错误，可继续接着读。
            }
            else
            {
                st_info_T("PPCS_Read: t:%d ms, Session=%d,CH=%d,ReadSize=%d,ret=%d [%s]\n", TU_MS(t1, t2), SessionID, CH_CMD, ReadSize, ret, getP2PErrorCodeInfo(ret));
                Running = 0;
            }
        }
        else if (ERROR_PPCS_INVALID_PARAMETER != ret && ERROR_PPCS_INVALID_SESSION_HANDLE != ret && 0 < ReadSize) // 读到数据
        {
            st_debug_T("PPCS_Read: t:%d ms,ret=%d,Session=%d,CH=%d,ReadSize=%d => [%d]\n", TU_MS(t1, t2), ret, SessionID, CH_CMD, ReadSize, ReadData);
        }
        else
        {
            st_info_T("PPCS_Read: t:%d ms,Session=%d,CH=%d,ReadSize=%d,ret=%d [%s]\n", TU_MS(t1, t2), SessionID, CH_CMD, ReadSize, ret, getP2PErrorCodeInfo(ret));
            Running = 0;
        }

        break;
    } // for

#endif

    while (Running)
    {
        //// Tell device the test mode
        //// 发送读写测试指令。
        ////               占2个bit位       2个bit位    4个bit位     2个bit位
        int TestMode = DirectionMode << 9 | SizeOption << 7 | ThreadNum << 3 | Mode << 1 | 0x01;
        ret = PPCS_Write(SessionID, CH_CMD, (char *)&TestMode, sizeof(TestMode));

        if (0 > ret)
        {
            st_info_T("PPCS_Write:Session=%d,CH=%d,SendSize=%d,Data:[%d],ret=%d [%s]\n", SessionID, CH_CMD, sizeof(TestMode), TestMode, ret, getP2PErrorCodeInfo(ret));
        }
        else // Select the test options according to the TestMode
        {
            st_debug_T("PPCS_Write:ret=%d,Session=%d,CH=%d,SendSize=%d => [%d]\n", ret, SessionID, CH_CMD, sizeof(TestMode), TestMode);

            // const char *APIInformation = PPCS_GetAPIInformation();
            // st_debug_T("PPCS_GetAPIInformation(%u byte):%s\n", strlen(APIInformation), APIInformation);
            switch (Mode)
            {
            case 0: ft_Test(SessionID); break;  // File transfer test

            case 1: RW_Test(SessionID, ThreadNum, SizeOption, DirectionMode); break;// Bidirectional read write test

            case 2:
            {
                if (2 != Sinfo.bMode && 1 != GetSocketType(Sinfo.Skt)) { pkt_Test(SessionID); }
                else { st_info("*****NOTE: the session Mode is TCP or LAN TCP, TCP mode does not Support PPCS_PktSend/PPCS_PktRecv API!!\n"); }

                break;  // PktRecv/PktSend test
            }
            break;

            case 3:
                pic_recver_create_task(SessionID, 0);
                break;

            default: break;
            }
        }

        break;
    } // while

    if (0 <= SessionID)
    {
        mSleep(500);
        my_GetCurrentTime(&t1);
        ret = PPCS_Close(SessionID);
        my_GetCurrentTime(&t2);
        st_info_T("PPCS_Close(%d) done!! ret=%d, t:%d ms\n", SessionID, ret, TU_MS(t1, t2)); // 不能多线程对同一个 SessionID 做 PPCS_Close(SessionID)/PPCS_ForceClose(SessionID) 的动作，否则可能导致崩溃。
    }

    gThread_UDP_ECHO_Exit = 1;

    if (0 < gUDP_ECHO_Socket)
    {
        shutdown(gUDP_ECHO_Socket, SHUT_RDWR);
        // st_info("shutdown: %d\n", gUDP_ECHO_Socket);
    }

    gThread_Check_Exit = 1;
    gThread_CheckBuffer_Exit = 1;


    PPCS_DeInitialize();
    st_debug_T("PPCS_DeInitialize done!!\n");
#if defined(WIN32DLL) || defined(WINDOWS)
    //printf("Please press any key to exit... ");
    //getchar();
#endif


    (void)gThread_CheckBuffer_Running;

    return 0;
}
