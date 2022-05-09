#ifndef __VIDEO_TRANSFER_CONFIG_H__
#define __VIDEO_TRANSFER_CONFIG_H__

#include <common/bk_include.h>

#if CONFIG_APP_DEMO_VIDEO_TRANSFER

#if (!CONFIG_CAMERA && (!CONFIG_SPIDMA && !CONFIG_HSLAVE_SPI))
#error "video transfer need open jpeg / hspi moudel"
#endif

#if ((!CONFIG_SPIDMA && CONFIG_HSLAVE_SPI) || (CONFIG_SPIDMA && !CONFIG_HSLAVE_SPI))
#error "if use hspi, need open CONFIG_SPIDMA and CONFIG_HSLAVE_SPI"
#endif

#if ((!CONFIG_SOC_BK7231U) && (!CONFIG_SOC_BK7251) && (!CONFIG_SOC_BK7271) && (!CONFIG_SOC_BK7236) && (!CONFIG_SOC_BK7256XX) && (!CONFIG_SOC_BK7256_CP1))
#if (CONFIG_CAMERA == 1)
#error "chip not support CONFIG_CAMERA"
#endif
#endif

#define APP_DEMO_CFG_USE_TCP              1
#define APP_DEMO_CFG_USE_UDP              1
#define APP_DEMO_CFG_USE_VIDEO_BUFFER     1
#define APP_DEMO_CFG_USE_UDP_SDP          1

#define SUPPORT_TIANZHIHENG_DRONE         0

// UDP_SDP_CONFIG
#define UDP_SDP_LOCAL_PORT               (10000)
#define UDP_SDP_REMOTE_PORT              (52110)

#if SUPPORT_TIANZHIHENG_DRONE
#define APP_DEMO_UDP_CMD_PORT             8090
#define APP_DEMO_UDP_IMG_PORT             8080
#define CMD_IMG_HEADER                    0x42
#define CMD_START_IMG                     0x76
#define CMD_STOP_IMG                      0x77
#define CMD_START_OTA                     0x38

#define APP_DEMO_TCP_SERVER_PORT          8050
#define APP_DEMO_TCP_SERVER_PORT_VICOE    8040

#define APP_DEMO_UDP_VOICE_PORT           8070
#define CMD_VOICE_HEADER                  0x30
#define CMD_VOICE_START                   0x66
#define CMD_VOICE_STOP                    0x67

#define CMD_HEADER_CODE                   0x66
#define CMD_TAIL_CODE                     0x99
#define CMD_LEN                           8
#define APP_DEMO_TCP_LISTEN_MAX           1

#define APP_DEMO_EN_VOICE_TRANSFER        0
#else  // SUPPORT_TIANZHIHENG_DRONE
#define APP_DEMO_UDP_CMD_PORT             7090
#define APP_DEMO_UDP_IMG_PORT             7080
#define CMD_IMG_HEADER                    0x20
#define CMD_START_IMG                     0x36
#define CMD_STOP_IMG                      0x37
#define CMD_START_OTA                     0x38

#define APP_DEMO_TCP_SERVER_PORT          7050
#define APP_DEMO_TCP_SERVER_PORT_VICOE    7040

#define APP_DEMO_UDP_VOICE_PORT           7070
#define CMD_VOICE_HEADER                  0x30
#define CMD_VOICE_START                   0x66
#define CMD_VOICE_STOP                    0x67

#define CMD_HEADER_CODE                   0x66
#define CMD_TAIL_CODE                     0x99
#define CMD_LEN                           8
#define APP_DEMO_TCP_LISTEN_MAX           1

#define APP_DEMO_EN_VOICE_TRANSFER        0
#endif  // SUPPORT_TIANZHIHENG_DRONE

#endif  // CONFIG_APP_DEMO_VIDEO_TRANSFER

#endif // __VIDEO_TRANSFER_NET_CONFIG_H__
