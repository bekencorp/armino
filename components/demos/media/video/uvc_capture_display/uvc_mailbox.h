#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "mailbox_channel.h"

#define MB_CHNL_UVC     3

typedef struct {
	uint32_t chnl_id;
} uvc_mb_t;

typedef enum {
	UVC_MB_UVC_INIT = 0,       //CPU0->CPU1, only init uvc
	UVC_MB_DISPLAY_UVC_INIT,   //CPU0->CPU1, init uvc and display
	UVC_MB_UVC_DEINIT,         //CPU0->CPU1, only deinit uvc
	UVC_MB_DISPLAY_UVC_DEINIT, //CPU0->CPU1, deinit uvc and display
	UVC_MB_UVC_START,          //CPU0->CPU1
	UVC_MB_UVC_STOP,           //CPU0->CPU1
	UVC_MB_CAPTURE_START,      //CPU0->CPU1
	UVC_MB_CAPTURE_BUSY,       //CPU1->CPU0
	UVC_MB_CAPTURE_DONE,       //CPU0->CPU1
	UVC_MB_PSRAM_REALSE,       //CPU1->CPU0
} uvc_maibox_cmd_t;

typedef struct {
	uvc_maibox_cmd_t mb_cmd;
	uint32_t param1;
	uint32_t param2;
	uint32_t param3;
} uvc_mailbox_msg_t;

typedef struct {
	uint8_t type;
	uint32_t data;
}uvc_cpu_msg_t;

typedef enum {
	UVC_CPU1_INIT = 0,
	UVC_CPU1_DISPLAY_INIT,
	UVC_CPU1_DEINIT,
	UVC_CPU1_CAPTURE,
	UVC_CPU0_SAVE_BUSY,
	UVC_CPU0_DEINIT,
}uvc_cpu_cmd_t;

void uvc_mailbox_init(void *rx_isr, void *tx_isr, void *tx_cmpl_isr);
void uvc_mailbox_deinit(void);
bk_err_t uvc_mailbox_send_msg(uvc_mailbox_msg_t *msg);

#ifdef __cplusplus
}
#endif


