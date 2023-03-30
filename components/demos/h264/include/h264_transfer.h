#ifndef _DEMOS_H264_TRANSFER_H__
#define _DEMOS_H264_TRANSFER_H__

#define APP_DEMO_CFG_USE_TCP              1
#define APP_DEMO_CFG_USE_UDP              1
#define APP_DEMO_CFG_USE_UDP_SDP          1

// UDP_SDP_CONFIG
#define UDP_SDP_LOCAL_PORT               (10000)
#define UDP_SDP_REMOTE_PORT              (52110)



#define DEMO_H264_UDP_CMD_PORT             7190
#define DEMO_H264_UDP_IMG_PORT             7180
#define DEMO_H264_UDP_VOICE_PORT           7170
#define DEMO_H264_TCP_SERVER_PORT          7150


/* Camera  */
#define H264_CMD_IMG_HEADER         0x20
#define H264_DVP_START				0x36
#define H264_DVP_STOP               0x37
#define H264_UVC_START              0x38
#define H264_UVC_STOP               0x39

#define H264_IMG_FMT_MJPEG          0x00
#define H264_IMG_FMT_H264           0x01


#define CMD_VOICE_HEADER                  0x30
#define CMD_VOICE_START                   0x66
#define CMD_VOICE_STOP                    0x67

#define CMD_HEADER_CODE                   0x66
#define CMD_TAIL_CODE                     0x99
#define CMD_LEN                           8
#define DEMO_H264_TCP_LISTEN_MAX           1

#define DEMO_H264_EN_VOICE_TRANSFER        0

#define AUDIO_TRANSFER_ENABLE           0

#define APP_DEMO_RCV_BUF_LEN        1472
#define APP_DEMO_SOCKET_TIMEOUT     100  // ms


/** define video transfer cmd */
typedef enum
{
	LCD_DISPLAY_BLEND_CLOSE = 2,  /**< define Video transfer lcd display */
	LCD_DISPLAY_BLEND_OPEN = 3,  /**< define Video transfer lcd display */
} video_transfer_cmd_t;

/** define audio transfer cmd */
typedef enum
{
	AUDIO_CLOSE = 0,
	AUDIO_OPEN = 1,
	DISPLAY_CLOSE = 2,
	DISPLAY_OPEN = 3,
	ECHO_DEPTH = 4,
	MAX_AMPLITUDE = 5,
	MIN_AMPLITUDE = 6,
	NOISE_LEVEL = 7,
	NOISE_PARAM = 8,
	AUD_DEBUG_OPEN = 9,
} H264_transfer_cmd_t;

typedef struct
{
	uint8_t id;
	uint8_t is_eof;
	uint8_t pkt_cnt;
	uint8_t size;
#if SUPPORT_TIANZHIHENG_DRONE
	uint32_t unused;
#endif
} media_hdr_t;

int demo_H264_udp_send_packet(uint8_t *data, uint32_t len);
bk_err_t demo_H264_udp_init(void);
void demo_H264_udp_deinit(void);
void demo_H264_disconnect_cmd_udp(void);
bk_err_t demo_H264_tcp_init(void);
bk_err_t demo_h264_usb_init(void);

// bk_err_t demo_H264_udp_client_init(int argc, char **argv);
// void demo_H264_udp_client_deinit(void);

// bk_err_t demo_H264_udp_server_init(int argc, char **argv);
// void demo_H264_udp_server_deinit(void);

#endif
// eof

