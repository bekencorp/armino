#ifndef __AT_SVR_OPTS_H_
#define __AT_SVR_OPTS_H_


#include "at_svr_opts.h"


#ifndef ATSVR_VERSION_NUM
#define ATSVR_VERSION_NUM            "ATSVR-00.01"
#endif

#ifndef ATSVR_MAX_COMMANDS
#define ATSVR_MAX_COMMANDS           64
#endif

/////set arguments max number
#ifndef ATSVR_MAX_ARG
#define ATSVR_MAX_ARG                16
#endif

#ifndef ATSVR_ADD_ESCAPE_CFG
#define ATSVR_ADD_ESCAPE_CFG         1
#endif

#ifndef ATSVRLOG
#define ATSVRLOG(...)
#endif

#ifndef ATSVR_INPUT_BUFF_MAX_SIZE
#define ATSVR_INPUT_BUFF_MAX_SIZE           4096
#endif

#ifndef ATSVR_HANDLER_ENV
#define ATSVR_HANDLER_ENV                   0
#endif

#ifndef ATSVR_CMD_HELP
#define ATSVR_CMD_HELP                      1
#endif

#ifndef ATSVR_CMD_HELP_BUF_SIZE
#define ATSVR_CMD_HELP_BUF_SIZE             (5 * 1024)
#endif

#ifndef ATSVR_ECHO_DEFAULT
#define ATSVR_ECHO_DEFAULT                  _ATSVR_ECHO_NORMAL
#endif

#ifndef ATSVR_INTERNAL_CMD
#define ATSVR_INTERNAL_CMD                  1
#endif

#ifndef ATSVR_RET_CHAR
#define ATSVR_RET_CHAR      '\r'
#endif

#ifndef ATSVR_END_CHAR
#define ATSVR_END_CHAR		'\n'
#endif

////optimization find command speed
#ifndef ATSVR_OPTIM_FD_CMD
#define ATSVR_OPTIM_FD_CMD                  0
#endif

#ifndef ATSVR_READY_MSG
#define ATSVR_READY_MSG                     "ready\r\n"
#endif

#ifndef ATSVR_CMD_RSP_SUCCEED
#define ATSVR_CMD_RSP_SUCCEED               "\r\nCMDRSP:OK\r\n"
#endif

#ifndef ATSVR_CMD_RSP_ERROR
#define ATSVR_CMD_RSP_ERROR                 "\r\nCMDRSP:ERROR\r\n"
#endif

#ifndef ATSVR_CMDMSG_ERROR_RSP
#define ATSVR_CMDMSG_ERROR_RSP              "\r\nCMDRSP:ERROR\r\n"
#endif

#ifndef ATSVR_CMDRSP_HEAD
#define ATSVR_CMDRSP_HEAD                   "CMDRSP:"
#endif

#ifndef ATSVR_EVENT_HEAD
#define ATSVR_EVENT_HEAD                    "EVT:"
#endif

#ifndef ATSVR_EVT_WLAN_CONNECTED
#define ATSVR_EVT_WLAN_CONNECTED            "EVT:WLAN CONNECTED\r\n"
#endif

#ifndef ATSVR_EVT_WLAN_DISCONNECTED
#define ATSVR_EVT_WLAN_DISCONNECTED         "EVT:WLAN DISCONNECTED\r\n"
#endif

#ifndef ATSVR_EVT_GOT_IP
#define ATSVR_EVT_GOT_IP                    "EVT:GOT-IP\r\n"
#endif

#ifndef ATSVR_EVT_TCP_DISCONNECTED
#define ATSVR_EVT_TCP_DISCONNECTED          "EVT:TCP DISCONNECTED,%d\r\n"
#endif

#ifndef ATSVR_EVT_TCP_DISCONNECTED
#define ATSVR_EVT_TCP_DISCONNECTED          "CMDRSP:TCP CONNECTED,%d\r\n"
#endif

#ifndef ATSVR_INPUT_MSG_OVERFLOW
#define ATSVR_INPUT_MSG_OVERFLOW            "Error: input overflow\r\n"
#endif

#ifndef ATSVR_POWER_UP_READY_DELAY
#define ATSVR_POWER_UP_READY_DELAY          200
#endif

#ifndef __weak__
#define __weak__              __attribute__((weak))
#endif

#ifndef ATSVR_SIZEOF_STR_LEN
#define ATSVR_SIZEOF_STR_LEN(string)         (sizeof(string))
#endif

#ifndef ATSVR_SIZEOF_STR_LEN_NOEOF
#define ATSVR_SIZEOF_STR_LEN_NOEOF(string)   (sizeof(string) - 1)
#endif

#endif
