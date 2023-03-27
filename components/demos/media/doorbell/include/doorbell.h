#ifndef _DEMOS_DOORBELL_H__
#define _DEMOS_DOORBELL_H__

#define APP_DEMO_CFG_USE_TCP              1
#define APP_DEMO_CFG_USE_UDP              1
#define APP_DEMO_CFG_USE_UDP_SDP          1

// UDP_SDP_CONFIG
#define UDP_SDP_LOCAL_PORT               (10000)
#define UDP_SDP_REMOTE_PORT              (52110)



#define DEMO_DOORBELL_UDP_CMD_PORT             7190
#define DEMO_DOORBELL_UDP_IMG_PORT             7180
#define DEMO_DOORBELL_UDP_VOICE_PORT           7170
#define DEMO_DOORBELL_TCP_SERVER_PORT          7150


/* Camera  */
#define DOORBELL_CMD_IMG_HEADER         0x20
#define DOORBELL_DVP_START              0x36
#define DOORBELL_DVP_STOP               0x37
#define DOORBELL_UVC_START              0x38
#define DOORBELL_UVC_STOP               0x39


#define CMD_VOICE_HEADER                  0x30
#define CMD_VOICE_START                   0x66
#define CMD_VOICE_STOP                    0x67

#define CMD_HEADER_CODE                   0x66
#define CMD_TAIL_CODE                     0x99
#define CMD_LEN                           8
#define DEMO_DOORBELL_TCP_LISTEN_MAX           1

#define DEMO_DOORBELL_EN_VOICE_TRANSFER        1

#define AUDIO_TRANSFER_ENABLE           1
#define CS2_P2P_AUDIO_TRANSFER_ENABLE 1

#define CS2_P2P_TRANSFER_CHECKSUM 1
#define CS2_P2P_TRANSFER_DELAY 20


#define APP_DEMO_RCV_BUF_LEN        1472
#define APP_DEMO_TCP_RCV_BUF_LEN    1460

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
} doorbell_transfer_cmd_t;

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

typedef struct __attribute__((packed))
{
    uint32_t magic_head;
//    uint8_t id;
//    uint8_t is_eof;
//    uint8_t frame_len;
    uint32_t package_len;

#if CS2_P2P_TRANSFER_CHECKSUM
    uint8_t check_sum;
#endif

#if SUPPORT_TIANZHIHENG_DRONE
    uint32_t unused;
#endif
} media_hdr_ext_t;

#define MAGIC_HEAD 0x25dca6f0

int demo_doorbell_udp_send_packet(uint8_t *data, uint32_t len);
bk_err_t demo_doorbell_udp_init(void);
void demo_doorbell_udp_deinit(void);
void demo_doorbell_disconnect_cmd_udp(void);


bk_err_t demo_doorbell_tcp_init(void);

bk_err_t demo_doorbell_udp_client_init(int argc, char **argv);
void demo_doorbell_udp_client_deinit(void);

bk_err_t demo_doorbell_udp_server_init(int argc, char **argv);
void demo_doorbell_udp_server_deinit(void);

bk_err_t demo_doorbell_tcp_client_init(int argc, char **argv);
void demo_doorbell_tcp_cli_deinit(void);

bk_err_t demo_doorbell_tcp_server_init(int argc, char **argv);
void demo_doorbell_tcp_server_deinit(void);

//bk_err_t demo_doorbell_cs2_p2p_server_init(
//    char *did_dskey,
//    char *apilicense_crckey,
//    char *initstring_p2pkey);
bk_err_t demo_doorbell_cs2_p2p_server_init(int argc, char **argv);
void demo_doorbell_cs2_p2p_server_deinit(void);


//bk_err_t demo_doorbell_cs2_p2p_client_init(
//    char *did_dskey,
//    char *apilicense_crckey,
//    char *initstring_p2pkey);

bk_err_t demo_doorbell_cs2_p2p_client_init(int argc, char **argv);
void demo_doorbell_cs2_p2p_client_deinit(void);


#endif
// eof

