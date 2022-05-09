#ifndef _AT_SVR_OPTS_H_
#define _AT_SVR_OPTS_H_


#define ATSVR_VERSION_NUM            "ATSVR-01.01"

#define ATSVR_MAX_COMMANDS           64
#define ATSVR_MAX_ARG                16

#define ATSVR_ADD_ESCAPE_CFG         1
#define ATSVR_INPUT_BUFF_MAX_SIZE           2048

#define ATSVR_POWER_UP_READY_DELAY          400

#define ATSVR_CMDRSP_HEAD                   "CMDRSP:"
#define ATSVR_READY_MSG                     "\r\nready\r\n"
#define ATSVR_CMD_RSP_SUCCEED               "CMDRSP:OK\r\n"
#define ATSVR_CMD_RSP_ERROR                 "CMDRSP:ERROR\r\n"
#define ATSVR_CMDMSG_ERROR_RSP              "CMDRSP:ERROR\r\n"
#define ATSVR_RET_CHAR                      '\r'
#define ATSVR_END_CHAR		                '\n'

#define ATSVR_EVENT_HEAD                    "EVT:"
#define ATSVR_EVT_WLAN_DISCONNECTED         "EVT:WLAN DISCONNECTED\r\n"
#define ATSVR_EVT_WLAN_CONNECTED            "EVT:WLAN CONNECTED\r\n"
#define ATSVR_EVT_GOT_IP                    "EVT:GOT-IP\r\n"


#ifndef __weak__
#define __weak__              __attribute__((weak))
#endif

extern void bk_printf(const char *fmt, ...);
#define ATSVRLOG                     bk_printf


#endif
