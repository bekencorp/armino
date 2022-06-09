#ifndef __DOORBELL_CONFIG_H__
#define __DOORBELL_CONFIG_H__

#include <common/bk_include.h>


#define APP_DEMO_CFG_USE_TCP              1
#define APP_DEMO_CFG_USE_UDP              1
#define APP_DEMO_CFG_USE_VIDEO_BUFFER     1
#define APP_DEMO_CFG_USE_UDP_SDP          1

// UDP_SDP_CONFIG
#define UDP_SDP_LOCAL_PORT               (10000)
#define UDP_SDP_REMOTE_PORT              (52110)



#define DEMO_DOORBELL_UDP_CMD_PORT             7190
#define DEMO_DOORBELL_UDP_IMG_PORT             7180
#define DOORBELL_CMD_IMG_HEADER                    0x20
#define DOORBELL_CMD_START_IMG                     0x36
#define DOORBELL_CMD_STOP_IMG                      0x37
#define CMD_START_OTA                     0x38

#define APP_DEMO_TCP_SERVER_PORT          7150
#define APP_DEMO_TCP_SERVER_PORT_VICOE    7140

#define DEMO_DOORBELL_UDP_VOICE_PORT           7170
#define CMD_VOICE_HEADER                  0x30
#define CMD_VOICE_START                   0x66
#define CMD_VOICE_STOP                    0x67

#define CMD_HEADER_CODE                   0x66
#define CMD_TAIL_CODE                     0x99
#define CMD_LEN                           8
#define APP_DEMO_TCP_LISTEN_MAX           1

#define DEMO_DOORBELL_EN_VOICE_TRANSFER        1


#endif // __VIDEO_TRANSFER_NET_CONFIG_H__
