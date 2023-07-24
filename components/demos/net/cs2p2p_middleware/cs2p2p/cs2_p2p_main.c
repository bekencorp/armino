#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>
#include "cs2_p2p_main.h"
#include <lwip/sockets.h>

#include "PPCS_API.h"
#include "osi_system.h"
#include "osi_cs2_not_impl.h"

#define SIZE_WAKEUP_KEY     17  // WakeUp Key Size

#define CHECK_WRITE_THRESHOLD_SIZE  1024*1024
#define NUMBER_OF_P2PCHANNEL        2
#define CHECK_BUFFER_FLAGS          0       // check buffer thread will be create, check buffer every 1 sec.
#define CHECK_FLAGS                 1       // check session mode every 100 ms, if connections mode is changed, it will print.
#define RECV_TMP_BUFF_SIZE   (1024 * 3)
#define READ_COUNT_PER (256)
#define NORMAL_MSLEEP 10


static char gLoginCheck_GetAPIInformation = 1;
// LoginStatus_Check Thread Running flags: thread exit->0, thread running->1
static char gThread_LoginCheck_Running = 0;
static char gFlags_WakeupTest = 0;

static volatile INT32 s_current_sessionid = -1;
static volatile uint8_t s_is_cs2_p2p_task_run = 0;
static uint8_t s_cs2_p2p_main_loop_control;
static st_PPCS_NetInfo s_cs2_p2p_networkinfo;

static beken_thread_t s_cs2_p2p_main_tid;
static uint8_t s_log_level = CS2_P2P_LOG_LEVEL_ERROR;


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
} st_session_info;
static st_session_info s_cs2_sinfo;

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
} st_time_info;

#define TIME_USE                (int)((tEnd.Tick_mSec) - (tBegin.Tick_mSec))
#define TU_MS(tBegin, tEnd)     (int)((tEnd.Tick_mSec) - (tBegin.Tick_mSec))

static void get_current_time(st_time_info *pt)
{
    struct timeval tmv;
    int ret = gettimeofday(&tmv, NULL);

    if (0 != ret)
    {
        os_printf("gettimeofday failed!! errno=%d\n", errno);
        memset(pt, 0, sizeof(st_time_info));
        return ;
    }

    struct tm stm = {0};

    struct tm *ptm = localtime_r((const time_t *)&tmv.tv_sec, &stm);

    if (!ptm)
    {
        os_printf("localtime_r failed!!\n");
        memset(pt, 0, sizeof(st_time_info));
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

static void ms_sleep(UINT32 ms)
{
    usleep(ms * 1000);
}



//// ret = 0: UDP, 1:TCP
static INT32 get_socket_type(INT32 Skt)
{
    socklen_t length = sizeof(unsigned int);
    int type;
    getsockopt(Skt, SOL_SOCKET, SO_TYPE, (char *)&type, &length);

    if (type == SOCK_STREAM) { return 1; }
    else { return 0; }
}

static const char *get_p2p_error_code_info(int err)
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
}

static const char *get_listen_error_info(int ret)
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

    default: return get_p2p_error_code_info(ret);
    }
}

static void show_network(st_PPCS_NetInfo NetInfo)
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




// -1:invalid parameter,0:not the same LAN Addresses,1:Addresses belonging to the same LAN.
static int is_lan_cmp(const char *IP1, const char *IP2)
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



static int cs2_get_sinfo(int SessionID, st_session_info *MySInfo)
{
    memset(MySInfo, 0, sizeof(st_session_info));
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
            if (Sinfo.RemoteAddr.sin_addr.s_addr == Sinfo.MyLocalAddr.sin_addr.s_addr || 1 == is_lan_cmp(MySInfo->MyLocalIP, MySInfo->RemoteIP))
            {
                MySInfo->bMyMode = 0;
                1 == get_socket_type(Sinfo.Skt) ? memcpy(MySInfo->Mode, "LAN.", 4) : memcpy(MySInfo->Mode, "LAN", 3);
            }
            else { MySInfo->bMyMode = 1; memcpy(MySInfo->Mode, "P2P", 3); }
        }
        else if (1 == Sinfo.bMode) { MySInfo->bMyMode = 2; memcpy(MySInfo->Mode, "RLY", 3); }
        else if (2 == Sinfo.bMode) { MySInfo->bMyMode = 3; memcpy(MySInfo->Mode, "TCP", 3); }
        else if (3 == Sinfo.bMode) { MySInfo->bMyMode = 4; memcpy(MySInfo->Mode, "RP2P", 4); }  //// support by P2P API 5.0.8
    }

    return ret;
}

static int32_t cs2_p2p_send_raw_internal(int SessionID, uint8_t Channel, uint8_t *buff, uint32_t size)
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

        // st_debug("ThreadWrite PPCS_Check_Buffer: Session=%d,CH=%d,WriteSize=%d,ret=%d %s\n", SessionID, Channel, WriteSize, Check_ret, get_p2p_error_code_info(Check_ret));
        if (0 > Check_ret)
        {
            st_info("%s PPCS_Check_Buffer: Session=%d,CH=%d,WriteSize=%d,ret=%d %s\n", __func__, SessionID, Channel, WriteSize, Check_ret, get_p2p_error_code_info(Check_ret));
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
                else { st_info("%s Session=%d,CH=%d,ret=%d %s\n", __func__, SessionID, Channel, ret, get_p2p_error_code_info(ret)); }

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


static int cs2_p2p_listen(const char *DID, const char *APILicense, unsigned long Repeat, uint8_t *is_run)
{
    st_time_info t1, t2;
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
    get_current_time(&t1);

    do
    {
        ret = PPCS_Listen(DID, TimeOut_Sec, UDP_Port, bEnableInternet, APILicense);
    }
    while (ERROR_PPCS_TIME_OUT == ret && *is_run);

    get_current_time(&t2);

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
        st_info("[%s] %02lu-PPCS_Listen failed:%d ms, ret=%d %s\n", t2.Date, Repeat, TU_MS(t1, t2), ret, get_listen_error_info(ret));
        return ret;
    }
    else //// ret >= 0, Listen OK, new client connect in.
    {
        SessionID = ret; // 每个 >=0 的 SessionID 都是一个正常的连接，本 sample 是单用户连接范例，多用户端连接注意要保留区分每一个 PPCS_Listen >=0 的 SessionID, 当连接断开或者 SessionID 不用时，必须要调 PPCS_Close(SessionID)/PPCS_ForceClose(SessionID) 关闭连线释放资源。
        PPCS_Share_Bandwidth(0); // 当有连接进来，关闭设备转发功能。
        st_debug_T("PPCS_Share_Bandwidth(0) is Called!!\n");
    }

    if (ERROR_PPCS_SUCCESSFUL != (ret = cs2_get_sinfo(SessionID, &s_cs2_sinfo)))
    {
        // connect success, but remote call closed.
        st_info_T("%02lu-DID=%s,Session=%d,RmtAddr=Unknown (PPCS_Check:%d)\n", Repeat, DID, SessionID, ret);
        return SessionID;
    }

    return SessionID;
}

static int do_server_job(char *DIDString, char *APILicense, int32_t (*recv_callback)(uint8_t channle, uint8_t *buff, uint32_t len), uint8_t *is_run)
{
    int Repeat = 0;
    st_time_info t1, t2;
    int32_t ret = 0;

    int SessionID = -99;
    const unsigned long Total_Times = Repeat;
    uint8_t *tmp_read_buf = NULL;

    (void)Total_Times;
    Repeat = 0;
    memset(&t2, 0, sizeof(t2));

    ret = PPCS_NetworkDetect(&s_cs2_p2p_networkinfo, 0);
    show_network(s_cs2_p2p_networkinfo);

    tmp_read_buf = os_malloc(RECV_TMP_BUFF_SIZE);

    if (!tmp_read_buf)
    {
        BK_LOGE("p2p", "%s alloc err\n", __func__);
        return -1;
    }

    while (*is_run)//Repeat < Total_Times)
    {
        Repeat++;

        // 3. Network Detect
#if 1
        ret = PPCS_NetworkDetect(&s_cs2_p2p_networkinfo, 0);
        if (ret < 0)
        {
            BK_LOGE("p2p", "%s PPCS_NetworkDetect err %d %s\n", __func__, ret, get_p2p_error_code_info(ret));
            ms_sleep(1000);
            continue;
        }

        show_network(s_cs2_p2p_networkinfo);

        if (0)//!s_cs2_p2p_networkinfo.bFlagInternet)
        {
            BK_LOGI("p2p", "%s internet not ready, sleep\n", __func__);
            ms_sleep(1000);
            continue;
        }

#endif

        if (0 == gFlags_WakeupTest) //// 常电设备测试
        {
            if (0 == gThread_LoginCheck_Running)
            {

            }

            s_current_sessionid = SessionID = cs2_p2p_listen(DIDString, APILicense, Repeat, is_run);

            if (0 <= SessionID)
            {
                BK_LOGI("p2p", "%s listen Sid %d\n", __func__, s_current_sessionid);

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
                                BK_LOGE("p2p", "%s PPCS_Check_Buffer %d ret err %d %s\n", __func__, i, ret, get_p2p_error_code_info(ret));
                                goto READ_ERR;
                            }

                            if (!recv_callback)
                            {
                                already_read = 1;

                                ReadSize = RECV_TMP_BUFF_SIZE;
                                ret = PPCS_Read(SessionID, i, (char *)tmp_read_buf, &ReadSize, 1000);

                                if (ret < 0 && ERROR_PPCS_TIME_OUT != ret)
                                {
                                    BK_LOGE("p2p", "%s PPCS_Read ret err %d %s\n", __func__, ret, get_p2p_error_code_info(ret));
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
                                    BK_LOGE("p2p", "%s PPCS_Read ret err %d %s\n", __func__, ret, get_p2p_error_code_info(ret));
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
                        usleep(recv_callback ? (NORMAL_MSLEEP * 1000) : (200 * 1000));
                    }

                }
                while (*is_run);

READ_ERR:;
                ms_sleep(300); // 两次 PPCS_Listen 之间需要保持间隔。
                get_current_time(&t1);
                PPCS_ForceClose(SessionID);// PPCS_Close(SessionID);// 不能多线程对同一个 SessionID 做 PPCS_Close(SessionID)/PPCS_ForceClose(SessionID) 的动作，否则可能导致崩溃。

                get_current_time(&t2);
                st_debug_T("%02lu-PPCS_ForceClose(%d) done!! t:%d ms\n", Repeat, SessionID, TU_MS(t1, t2));
                s_current_sessionid = SessionID = -1;
                continue;
            }
            else if (ERROR_PPCS_MAX_SESSION == SessionID)
            {
                ms_sleep(1000);    //// 超过最大连线数
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
    st_time_info t1, t2;
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

    get_current_time(&t1);

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
            ms_sleep(1000);
            continue;
        }
        else
        {
            show_network(s_cs2_p2p_networkinfo);
        }

        if (0)//!s_cs2_p2p_networkinfo.bFlagInternet)
        {
            BK_LOGI("p2p", "%s internet not ready, sleep\n", __func__);
            ms_sleep(1000);
            continue;
        }

        //        bEnableLanSearch = 0x5e;//connectiontest mode 6 default, very fast, 0x7a is very slow
        st_info("[%s] PPCS_ConnectByServer(%s,%p,0x%02X,0,ByServerStringlen %d) ...\n", t1.Date, DIDString, DIDString, bEnableLanSearch, strlen(ByServerString));

        ret = PPCS_ConnectByServer(DIDString, bEnableLanSearch, 0, ByServerString);
        get_current_time(&t2);

        if (0 > ret)
        {
            st_info("[%s] PPCS_ConnectByServer(%s,0x%02X,0,ByServerStringlen %d) Failed: %d ms, %d [%s]\n", t2.Date, DIDString, bEnableLanSearch, strlen(ByServerString), TU_MS(t1, t2), ret, get_p2p_error_code_info(ret));

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
                    if (Sinfo.RemoteAddr.sin_addr.s_addr == Sinfo.MyLocalAddr.sin_addr.s_addr || 1 == is_lan_cmp(MyLanIP, RmtIP))
                    {
                        (1 == get_socket_type(Sinfo.Skt)) ? (pMode = "LAN.") : (pMode = "LAN");
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
                cs2_get_sinfo(SessionID, &s_cs2_sinfo);

                if (1 == Sinfo.bMode && 1 == AllowRP2P)
                {
                    //CreateThread_PPCS_Check(SessionID);
                } //// AllowRP2P=1 and mode is RLY, then create ppcs check thread to check whether the mode is changed to RP2P


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
                                BK_LOGE("p2p", "%s PPCS_Check_Buffer %d ret err %d %s\n", __func__, i, ret, get_p2p_error_code_info(ret));
                                goto READ_ERR;
                            }

                            if (!recv_callback)
                            {
                                already_read = 1;

                                ReadSize = RECV_TMP_BUFF_SIZE;
                                ret = PPCS_Read(SessionID, i, (char *)tmp_read_buf, &ReadSize, 1000);

                                if (ret < 0 && ERROR_PPCS_TIME_OUT != ret)
                                {
                                    BK_LOGE("p2p", "%s PPCS_Read ret err %d %s\n", __func__, ret, get_p2p_error_code_info(ret));
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
                                    BK_LOGE("p2p", "%s PPCS_Read ret err %d %s\n", __func__, ret, get_p2p_error_code_info(ret));
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
                        usleep(recv_callback ? (NORMAL_MSLEEP * 1000) : (200 * 1000));
                    }

                }
                while (*is_run);

            }

READ_ERR:;
            ms_sleep(300); // 两次 PPCS_Listen 之间需要保持间隔。
            get_current_time(&t1);
            PPCS_ForceClose(SessionID);// PPCS_Close(SessionID);// 不能多线程对同一个 SessionID 做 PPCS_Close(SessionID)/PPCS_ForceClose(SessionID) 的动作，否则可能导致崩溃。

            get_current_time(&t2);
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

    char *DIDString = pDIDString;
    char *APILicense = pAPILicense;
    char *InitString = pInitString;
    char WakeupKey[SIZE_WAKEUP_KEY] = {0};

    unsigned char NumberOfWakeupServer = 0;



    st_info("DID:DSkey=%s\n", DIDString);

    st_info("APILicense:CRCkey=%s\n", APILicense);
    st_info("InitString len=%d\n", strlen(InitString));
    st_debug("WakeupTest=%s\n", gFlags_WakeupTest ? "YES" : "NO");

    if (gFlags_WakeupTest)
    {
        st_info("WakeupKey=%s\n", WakeupKey);
        st_info("NumberOfWakeupServer=%d\n", NumberOfWakeupServer);
    }


    st_time_info t1, t2;
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
        get_current_time(&t1);
        st_debug("[%s] PPCS_Initialize1(%s) ...\n", t1.Date, InitJsonString);


        // 如果Parameter 不是正确的JSON字串则会被当成InitString[:P2PKey]来处理, 如此以兼容旧版.
        ret = PPCS_Initialize((char *)InitJsonString);
#else
        ret = PPCS_Initialize(InitString); //CS2 表示，如遇到-22错误，可传入InitString
#endif
        get_current_time(&t2);
        st_info("[%s] PPCS_Initialize2 len(%d): ret=%d, t:%d ms\n", t2.Date, strlen(InitString), ret, TU_MS(t1, t2));


        if (ERROR_PPCS_SUCCESSFUL != ret && ERROR_PPCS_ALREADY_INITIALIZED != ret)
        {
            st_info("[%s] PPCS_Initialize: ret=%d\n", t2.Date, ret);

            if (gFlags_WakeupTest) {  }

            return 0;
        }
    }
    else
    {
        get_current_time(&t1);
        st_debug("[%s] PPCS_Initialize3(%s) ...\n", t1.Date, InitString);
        ret = PPCS_Initialize((char *)InitString);
        get_current_time(&t2);
        st_info("[%s] PPCS_Initialize4(%s): ret=%d, t:%d ms\n", t2.Date, InitString, ret, TU_MS(t1, t2));

        if (ERROR_PPCS_SUCCESSFUL != ret && ERROR_PPCS_ALREADY_INITIALIZED != ret)
        {
            st_info("[%s] PPCS_Initialize: ret=%d\n", t2.Date, ret);

            if (gFlags_WakeupTest) { }

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
        BK_LOGE("p2p", "%s PPCS_DeInitialize: ret=%d %s\n", __func__, ret, get_p2p_error_code_info(ret));
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
        return cs2_p2p_send_raw_internal(s_current_sessionid, channel, buff, size);
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

