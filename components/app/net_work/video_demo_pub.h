#ifndef __VOID_DEMO_PUB_H__
#define __VOID_DEMO_PUB_H__

enum
{
    DAP_TIMER_POLL          = 0,
    DAP_WIFI_DISCONECTED,
    DAP_WIFI_CONECTED,
    DAP_APP_CONECTED,
    DAP_APP_DISCONECTED,
    DAP_EXIT,
    DAP_START_OTA,
};

typedef struct tvideo_ota_st
{
    const char *http_url;
    int    http_port;
    UINT32 http_timeout;
} TV_OTA_ST, *TV_OTA_PTR;

typedef struct temp_message
{
    u32 dmsg;
    u32 data;
} DRONE_MSG_T;

#endif  // __VOID_DEMO_PUB_H__