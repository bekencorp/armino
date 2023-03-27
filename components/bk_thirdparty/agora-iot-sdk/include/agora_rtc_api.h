/*************************************************************
 *
 * This is a part of the Agora RTC Service SDK.
 * Copyright (C) 2020 Agora IO
 * All rights reserved.
 *
 *************************************************************/

#ifndef __AGORA_RTC_API_H__
#define __AGORA_RTC_API_H__

// #define CONFIG_AUDIO_ONLY
#define CONFIG_RTCM

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER)
#if defined(__BUILDING_AGORA_SDK__)
#define __agora_api__ __declspec(dllexport)
#else
#define __agora_api__ __declspec(dllimport)
#endif
#else
#define __agora_api__
#endif

#define AGORA_RTC_CHANNEL_NAME_MAX_LEN (64)
#define AGORA_RTC_USER_ID_MAX_LEN (255)
#define AGORA_RTC_PRODUCT_ID_MAX_LEN (63)
#define AGORA_RTM_UID_MAX_LEN 64
#define AGORA_RTM_DATA_MAX_LEN (32 * 1024)
#define AGORA_CREDENTIAL_MAX_LEN 256
#define AGORA_CERTIFICATE_MAX_LEN 1024
#define AGORA_LICENSE_VALUE_LEN 32

/** Error code. */
typedef enum {
  /** No error. */
  ERR_OKAY = 0,

  /** General error */
  ERR_FAILED = 1,

  /** Network is unavailable */
  ERR_NET_DOWN = 14,

  /**
   * Request to join channel is rejected.
   * It occurs when local user is already in channel and try to join the same channel again.
   */
  ERR_JOIN_CHANNEL_REJECTED = 17,

  /** App ID is invalid. */
  ERR_INVALID_APP_ID = 101,

  /** Channel is invalid. */
  ERR_INVALID_CHANNEL_NAME = 102,

  /** Fails to get server resources in the specified region. */
  ERR_NO_SERVER_RESOURCES = 103,

  /** Server rejected request to look up channel. */
  ERR_LOOKUP_CHANNEL_REJECTED = 105,

  /** Server rejected request to open channel. */
  ERR_OPEN_CHANNEL_REJECTED = 107,

  /**
   * Token expired due to reasons belows:
   * - Authorized Timestamp expired:      The timestamp is represented by the number of
   *                                      seconds elapsed since 1/1/1970. The user can use
   *                                      the Token to access the Agora service within five
   *                                      minutes after the Token is generated. If the user
   *                                      does not access the Agora service after five minutes,
   *                                      this Token will no longer be valid.
   * - Call Expiration Timestamp expired: The timestamp indicates the exact time when a
   *                                      user can no longer use the Agora service (for example,
   *                                      when a user is forced to leave an ongoing call).
   *                                      When the value is set for the Call Expiration Timestamp,
   *                                      it does not mean that the Token will be expired,
   *                                      but that the user will be kicked out of the channel.
   */
  ERR_TOKEN_EXPIRED = 109,

  /**
  * Token is invalid due to reasons belows:
  * - If application certificate is enabled on the Dashboard,
  *   valid token SHOULD be set when invoke.
  *
  * - If uid field is mandatory, users must specify the same uid as the one used to generate the token,
  *   when calling `agora_rtc_join_channel`.
  */
  ERR_INVALID_TOKEN = 110,

  /** Dynamic token has been enabled, but is not provided when joining the channel.
   *  Please specify the valid token when calling `agora_rtc_join_channel`.
   */
  ERR_DYNAMIC_TOKEN_BUT_USE_STATIC_KEY = 115,

  /** Switching roles failed.
   *  Please try to rejoin the channel.
   */
  ERR_SET_CLIENT_ROLE_NOT_AUTHORIZED = 119,

  /** Decryption fails. The user may have used a different encryption password to join the channel.
   *  Check your settings or try rejoining the channel.
   */
  ERR_DECRYPTION_FAILED = 120,

  /* Ticket to open channel is invalid */
  ERR_OPEN_CHANNEL_INVALID_TICKET = 121,

  /* Try another server. */
  ERR_OPEN_CHANNEL_TRY_NEXT_VOS = 122,

  /** Client is banned by the server */
  ERR_CLIENT_IS_BANNED_BY_SERVER = 123,

  /** Sending video data too fast and over the bandwidth limit.
   *  Very likely that packet loss occurs with this sending speed.
   */
  ERR_SEND_VIDEO_OVER_BANDWIDTH_LIMIT = 200,

  /** Audio decoder does not match incoming audio data type.
   *  Currently SDK built-in audio codec only supports G722 and OPUS.
   */
  ERR_AUDIO_DECODER_NOT_MATCH_AUDIO_FRAME = 201,

  /** Audio decoder does not match incoming audio data type.
   *  Currently SDK built-in audio codec only supports G722 and OPUS.
   */
  ERR_NO_AUDIO_DECODER_TO_HANDLE_AUDIO_FRAME = 202,
} agora_err_code_e;

/**
 * The definition of the user_offline_reason_e enum.
 */
typedef enum {
  /**
   * 0: Remote user leaves channel actively
   */
  USER_OFFLINE_QUIT = 0,
  /**
   * 1: Remote user is dropped due to timeout
   */
  USER_OFFLINE_DROPPED = 1,
} user_offline_reason_e;

/**
 * The definition of the video_data_type_e enum.
 */
typedef enum {
  /* 0: YUV420 */
  VIDEO_DATA_TYPE_YUV420 = 0,
  /* 2: H264 */
  VIDEO_DATA_TYPE_H264 = 2,
  /* 3: H265 */
  VIDEO_DATA_TYPE_H265 = 3,
  /* 6: generic */
  VIDEO_DATA_TYPE_GENERIC = 6,
  /* 20: generic JPEG */
  VIDEO_DATA_TYPE_GENERIC_JPEG = 20,
} video_data_type_e;

/**
 * The definition of the video_frame_type_e enum.
 */
typedef enum {
  /**
   * 0: unknow frame type
   * If you set it ,the SDK will judge the frame type
   */
  VIDEO_FRAME_AUTO_DETECT = 0,
  /**
   * 3: key frame
   */
  VIDEO_FRAME_KEY = 3,
  /*
   * 4: delta frame, e.g: P-Frame
   */
  VIDEO_FRAME_DELTA = 4,
} video_frame_type_e;

/**
 * The definition of the video_frame_rate_e enum.
 */
typedef enum {
  /** 1: 1 fps. */
  VIDEO_FRAME_RATE_FPS_1 = 1,
  /** 7: 7 fps. */
  VIDEO_FRAME_RATE_FPS_7 = 7,
  /** 10: 10 fps. */
  VIDEO_FRAME_RATE_FPS_10 = 10,
  /** 15: 15 fps. */
  VIDEO_FRAME_RATE_FPS_15 = 15,
  /** 24: 24 fps. */
  VIDEO_FRAME_RATE_FPS_24 = 24,
  /** 30: 30 fps. */
  VIDEO_FRAME_RATE_FPS_30 = 30,
  /** 60: 60 fps. Applies to Windows and macOS only. */
  VIDEO_FRAME_RATE_FPS_60 = 60,
} video_frame_rate_e;

/**
 * Video stream types.
 */
typedef enum {
  /**
   * 0: The high-quality video stream, which has a higher resolution and bitrate.
   */
  VIDEO_STREAM_HIGH = 0,
  /**
   * 1: The low-quality video stream, which has a lower resolution and bitrate.
   */
  VIDEO_STREAM_LOW = 1,
} video_stream_type_e;

/**
 * The definition of the video_frame_info_t struct.
 */
typedef struct {
  /**
   * The video data type: #video_data_type_e.
   */
  video_data_type_e data_type;
  /**
   * The video stream type: #video_stream_type_e
   */
  video_stream_type_e stream_type;
  /**
   * The frame type of the encoded video frame: #video_frame_type_e.
   */
  video_frame_type_e frame_type;
  /**
   * The number of video frames per second.
   * -This value will be used for calculating timestamps of the encoded image.
   * - If frame_per_sec equals zero, then real timestamp will be used.
   * - Otherwise, timestamp will be adjusted to the value of frame_per_sec set.
   */
  video_frame_rate_e frame_rate;
} video_frame_info_t;

/**
 * Audio codec type list.
 */
typedef enum {
  /**
   * 0: Disable
   */
  AUDIO_CODEC_DISABLED = 0,
  /**
   * 1: OPUS
   */
  AUDIO_CODEC_TYPE_OPUS = 1,
  /**
   * 2: G722
   */
  AUDIO_CODEC_TYPE_G722 = 2,
  /**
   * 3: G711A
   */
  AUDIO_CODEC_TYPE_G711A = 3,
  /**
   * 4: G711U
   */
  AUDIO_CODEC_TYPE_G711U = 4,
} audio_codec_type_e;

/**
 * Audio data type list.
 */
typedef enum {
  /**
   * 1: OPUS
   */
  AUDIO_DATA_TYPE_OPUS = 1,
  /**
   * 2: OPUSFB
   */
  AUDIO_DATA_TYPE_OPUSFB = 2,
  /**
   * 3: PCMA
   */
  AUDIO_DATA_TYPE_PCMA = 3,
  /**
   * 4: PCMU
   */
  AUDIO_DATA_TYPE_PCMU = 4,
  /**
   * 5: G722
   */
  AUDIO_DATA_TYPE_G722 = 5,
  /**
   * 8: AACLC
   */
  AUDIO_DATA_TYPE_AACLC = 8,
  /**
   * 9: HEAAC
   */
  AUDIO_DATA_TYPE_HEAAC = 9,
  /**
   * 100: PCM (audio codec should be enabled)
   */
  AUDIO_DATA_TYPE_PCM = 100,
  /**
   * 253: GENERIC
   */
  AUDIO_DATA_TYPE_GENERIC = 253,
} audio_data_type_e;

/**
 * The definition of the audio_frame_info_t struct.
 */
typedef struct {
  /**
   * Audio data type, reference #audio_data_type_e.
   */
  audio_data_type_e data_type;
} audio_frame_info_t;

/**
 * The definition of log level enum
 */
typedef enum {
  RTC_LOG_DEFAULT = 0, // the same as RTC_LOG_NOTICE
  RTC_LOG_EMERG, // system is unusable
  RTC_LOG_ALERT, // action must be taken immediately
  RTC_LOG_CRIT, // critical conditions
  RTC_LOG_ERROR, // error conditions
  RTC_LOG_WARNING, // warning conditions
  RTC_LOG_NOTICE, // normal but significant condition, default level
  RTC_LOG_INFO, // informational
  RTC_LOG_DEBUG, // debug-level messages
} rtc_log_level_e;

/**
 * IP areas.
 */
typedef enum {
  /* the same as AREA_CODE_GLOB */
  AREA_CODE_DEFAULT = 0x00000000,
  /* Mainland China. */
  AREA_CODE_CN = 0x00000001,
  /* North America. */
  AREA_CODE_NA = 0x00000002,
  /* Europe. */
  AREA_CODE_EU = 0x00000004,
  /* Asia, excluding Mainland China. */
  AREA_CODE_AS = 0x00000008,
  /* Japan. */
  AREA_CODE_JP = 0x00000010,
  /* India. */
  AREA_CODE_IN = 0x00000020,
  /* Oceania */
  AREA_CODE_OC = 0x00000040,
  /* South-American */
  AREA_CODE_SA = 0x00000080,
  /* Africa */
  AREA_CODE_AF = 0x00000100,
  /* South Korea */
  AREA_CODE_KR = 0x00000200,
  /* The global area (except China) */
  AREA_CODE_OVS = 0xFFFFFFFE,
  /* (Default) Global. */
  AREA_CODE_GLOB = (0xFFFFFFFF),
} area_code_e;

/**
 * log config
 */
typedef struct {
  bool log_disable;
  bool log_disable_desensitize;
  rtc_log_level_e log_level;
  const char *log_path;
} log_config_t;

/**
 * The definition of the service option
 */
typedef struct {
  /**
   * area code for regional restriction, default is golbal, Supported areas refer to enum area_code_e
   * and bit operations for multiple regions are supported
   */
  uint32_t area_code;
  /**
   * the  product_id (device name), the max product_id length is 63
   */
  char product_id[AGORA_RTC_PRODUCT_ID_MAX_LEN + 1];
  /**
   * sdk log config
   */
  log_config_t log_cfg;
  /**
   * license value return when license actived, the max license_value length is 32
   * Supported character scopes are:
   * - The 26 lowercase English letters: a to z
   * - The 26 uppercase English letters: A to Z
   * - The 10 numbers: 0 to 9
   */
  char license_value[AGORA_LICENSE_VALUE_LEN + 1];
} rtc_service_option_t;

typedef struct {
  /**
   * Configure sdk built-in audio codec
   * If AUDIO_CODEC_TYPE_OPUS is selected, your PCM data is encoded as OPUS and then streamed to Agora channel
   * If AUDIO_CODEC_TYPE_G722 is selected, your PCM data is encoded as G722 and then streamed to Agora channel
   * If you provide encoded audio data, such as AAC, instead of raw PCM, please disable audio codec by selecting AUDIO_CODEC_DISABLED
   */
  audio_codec_type_e audio_codec_type;
  /**
   * Pcm sample rate. Ignored if audio coded is diabled
   */
  int pcm_sample_rate;
  /**
   * Pcm channel number. Ignored if audio coded is diabled
   */
  int pcm_channel_num;
} audio_codec_option_t;

/**
 * The definition of the rtc_audio_process_options_t struct.
 */
typedef struct{
  // whether to open audio process
  bool enable_audio_process;
  // whether to open aec
  bool enable_aec;
  // whether to open ns
  bool enable_ns;

  bool ref_data_from_sdk;
  // whether to dump audio data from audio process
  bool enable_dump_data;
} rtc_audio_process_options_t;

/**
 * The definition of the rtc_channel_options_t struct.
 */
typedef struct {
  // whether to auto subscribe audio
  bool auto_subscribe_audio;
  // whether to auto subscribe video
  bool auto_subscribe_video;
  // whether to subscribe local user of other connections created by #agora_rtc_create_connection
  bool subscribe_local_user;
  // whether to use the audio jitter buffer
  bool enable_audio_jitter_buffer;
  // whether to use the audio mixing function
  bool enable_audio_mixer;
  // audio encode and decode configuration when send pcm audio data by #agora_rtc_send_audio_data
  audio_codec_option_t audio_codec_opt;
  // audio process options
  rtc_audio_process_options_t audio_process_opt;
  // whether to use encryption for audio and video data when transmission to network
  // this only works with SDK that have AUT(agora universal transport) feature
  bool enable_aut_encryption;
} rtc_channel_options_t;

typedef enum {
  CLOUD_PROXY_NONE = 0,
  CLOUD_PROXY_UDP = 1,
  CLOUD_PROXY_TCP = 3,
  CLOUD_PROXY_AUTO = 100,
} cloud_proxy_type_e;

/**
 * Network event type enum
 */
typedef enum {
  NETWORK_EVENT_DOWN = 0,
  NETWORK_EVENT_UP,
  NETWORK_EVENT_CHANGE,
} network_event_type_e;


/**
 * Connection identification
 */
typedef uint32_t connection_id_t;

/**
 * Special connection id
 */
// All connections Created
#define CONNECTION_ID_ALL ((connection_id_t)0)
// Invalid connection id
#define CONNECTION_ID_INVALID ((connection_id_t)-1)

/**
 * connection info
 */
typedef struct {
  // Connection identification
  connection_id_t conn_id;
  // user id
  uint32_t uid;
  // channel name
  char channel_name[AGORA_RTC_CHANNEL_NAME_MAX_LEN + 1];
} connection_info_t;

/**
 * Agora RTC SDK event handler
 */
typedef struct {
  /**
   * Occurs when local user joins channel successfully.
   *
   * @param[in] conn_id    Connection identification
   * param[in] uid         local uid
   * @param[in] elapsed_ms Time elapsed (ms) since channel is established
   */
  void (*on_join_channel_success)(connection_id_t conn_id, uint32_t uid, int elapsed_ms);

  /**
   * Occurs when channel is disconnected from the server.
   *
   * @param[in] conn_id    Connection identification
   */
  void (*on_connection_lost)(connection_id_t conn_id);

  /**
   * Occurs when local user rejoins channel successfully after disconnect
   *
   * When channel loses connection with server due to network problems,
   * SDK will retry to connect automatically. If success, it will be triggered.
   *
   * @param[in] conn_id    Connection identification
   * @param[in] uid        local uid
   * @param[in] elapsed_ms Time elapsed (ms) since rejoin due to network
   */
  void (*on_rejoin_channel_success)(connection_id_t conn_id, uint32_t uid, int elapsed_ms);

  /**
   * Report error message during runtime.
   *
   * In most cases, it means SDK can't fix the issue and application should take action.
   *
   * @param[in] conn_id Connection identification
   * @param[in] code    Error code, see #agora_err_code_e
   * @param[in] msg     Error message
   */
  void (*on_error)(connection_id_t conn_id, int code, const char *msg);

  /**
   * Occurs when a remote user joins channel successfully.
   *
   * @param[in] conn_id    Connection identification
   * @param[in] uid        Remote user ID
   * @param[in] elapsed_ms Time elapsed (ms) since channel is established
   */
  void (*on_user_joined)(connection_id_t conn_id, uint32_t uid, int elapsed_ms);

  /**
   * Occurs when remote user leaves the channel.
   *
   * @param[in] conn_id Connection identification
   * @param[in] uid     Remote user ID
   * @param[in] reason  Reason, see #user_offline_reason_e
   */
  void (*on_user_offline)(connection_id_t conn_id, uint32_t uid, int reason);

  /**
   * Occurs when a remote user sends notification before enable/disable sending audio.
   *
   * @param[in] conn_id Connection identification
   * @param[in] uid     Remote user ID
   * @param[in] muted   Mute status:
   *                    - false(0): unmuted
   *                    - true (1): muted
   */
  void (*on_user_mute_audio)(connection_id_t conn_id, uint32_t uid, bool muted);

#ifndef CONFIG_AUDIO_ONLY
  /**
   * Occurs when a remote user sends notification before enable/disable sending video.
   *
   * @param[in] conn_id Connection identification
   * @param[in] uid     Remote user ID
   * @param[in] muted   Mute status:
   *                    - false(0): unmuted
   *                    - true (1): muted
   */
  void (*on_user_mute_video)(connection_id_t conn_id, uint32_t uid, bool muted);

#endif
  /**
   * Occurs when receiving the audio frame of a remote user in the channel.
   *
   * @param[in] conn_id    Connection identification
   * @param[in] uid        Remote user ID to which data is sent
   * @param[in] sent_ts    Timestamp (ms) for sending data
   * @param[in] data_ptr   Audio frame buffer
   * @param[in] data_len   Audio frame buffer length (bytes)
   * @param[in] frame_info Audio frame info
   */
  void (*on_audio_data)(connection_id_t conn_id, uint32_t uid, uint16_t sent_ts, const void *data_ptr, size_t data_len,
                        const audio_frame_info_t *info_ptr);

  /**
   * Occurs every 20ms.
   *
   * @param[in] conn_id     Connection identification
   * @param[in] data_ptr    Audio frame buffer
   * @param[in] data_len    Audio frame buffer length (bytes)
   * @param[in] frame_info  Audio frame info
   */
  void (*on_mixed_audio_data)(connection_id_t conn_id, const void *data_ptr, size_t data_len,
                              const audio_frame_info_t *info_ptr);

#ifndef CONFIG_AUDIO_ONLY
  /**
   * Occurs when receiving the video frame of a remote user in the channel.
   *
   * @param[in] conn_id      Connection identification
   * @param[in] uid          Remote user ID to which data is sent
   * @param[in] sent_ts      Timestamp (ms) for sending data
   * @param[in] data_ptr     Video frame buffer
   * @param[in] data_len     Video frame buffer lenth (bytes)
   * @param[in] frame_info   Video frame info
   */
  void (*on_video_data)(connection_id_t conn_id, uint32_t uid, uint16_t sent_ts, const void *data_ptr, size_t data_len,
                        const video_frame_info_t *info_ptr);

  /**
   * Occurs when network bandwidth change is detected. User is expected to adjust encoder bitrate to |target_bps|
   *
   * @param[in] conn_id    Connection identification
   * @param[in] target_bps Target value (bps) by which the bitrate should update
   */
  void (*on_target_bitrate_changed)(connection_id_t conn_id, uint32_t target_bps);

  /**
   * Occurs when a remote user requests a keyframe.
   *
   * This callback notifies the sender to generate a new keyframe.
   *
   * @param[in] conn_id      Connection identification
   * @param[in] uid          Remote user ID
   * @param[in] stream_type  Stream type for which a keyframe is requested
   */
  void (*on_key_frame_gen_req)(connection_id_t conn_id, uint32_t uid, video_stream_type_e stream_type);

#endif

  /**
   * Occurs when token will expired.
   *
   * @param[in] conn_id    Connection identification
   * @param[in] token      The token will expire
   */
  void (*on_token_privilege_will_expire)(connection_id_t conn_id, const char *token);

#ifdef CONFIG_RTCM
  /**
   * Occur when receive a media ctrl message.
   *
   * @param[in] conn_id  Connection identification
   * @param[in] uid      Remote user ID
   * @param[in] payload  payload of message
   * @param[in] length   length of payload
   */
  void (*on_media_ctrl_receive)(connection_id_t conn_id, uint32_t uid, const void *payload, size_t length);
#endif
} agora_rtc_event_handler_t;

/**
 * @brief Get SDK version.
 * @return A const static string describes the SDK version
 */
extern const char *agora_rtc_get_version(void);

/**
 * @brief Convert error code to const static string.
 * @note You do not have to release the string after use.
 * @param[in] err Error code
 * @return Const static error string
 */
extern __agora_api__ const char *agora_rtc_err_2_str(int err);

#ifdef CONFIG_LICENSE
/**
 * @brief Generate a credential which is a unique device identifier.
 * @note It's authorizing smart devices license.
 *       You can disregard it if license isn't used.
 * @param[out]    credential        Credential buffer holding the generated data
 * @param[in,out] credential_len    Credential buffer length (bytes), which should be larger than AGORA_CREDENTIAL_MAX_LEN
 * @return
 * - = 0: Success
 * - < 0: Failure
 */
extern __agora_api__ int agora_rtc_license_gen_credential(char *credential, unsigned int *credential_len);

/**
 * @brief Authenticate the SDK licence.
 * @note
 * - It's authorizing smart devices license.
 *   You can disregard it if you do not use a license.
 *   Once the license is enabled, only the authenticated SDK can be used.
 * - This API should be invoked before agora_rtc_init
 * @param[in] certificate     Certificate buffer
 * @param[in] certificate_len Certificate buffer length
 * @param[in] credential      Credential buffer
 * @param[in] credential_len  Credential buffer length
 * @return
 * - = 0: Success
 * - < 0: Failure
 */
extern __agora_api__ int agora_rtc_license_verify(const char *certificate, int certificate_len, const char *credential,
                                                  int credential_len);

#endif // End CONFIG_LICENSE
/**
 * @brief Initialize the Agora RTSA service.
 * @note Each process can only be initialized once.
 * @param[in] app_id          Application ID
 *                            If 'uid' is set as 0, SDK will assign a valid ID to the user
 * @param[in] event_handler   A set of callback that handles Agora SDK events
 * @param[in] option          RTC service option when init, If need't set option, set NULL
 * @return
 * - = 0: Success
 * - < 0: Failure
 */
extern __agora_api__ int agora_rtc_init(const char *app_id, const agora_rtc_event_handler_t *event_handler,
                                        rtc_service_option_t *option);
/**
 * @brief Release all resource allocated by Agora RTSA SDK
 * @return
 * - = 0: Success
 * - < 0: Failure
 */
extern __agora_api__ int agora_rtc_fini(void);

/**
 * @brief Set the log level.
 * @param[in] level Log level. Range reference to rtc_log_level_e enum
 * @return
 * - = 0: Success
 * - < 0: Failure
 */
extern __agora_api__ int agora_rtc_set_log_level(rtc_log_level_e level);

/**
 * @brief Sets the log file configuration.
 * @param[in] per_logfile_size The size (bytes) of each log file.
 *                             The value range is [0, 10*1024*1024(10MB)], default 1*1024*1024(1MB).
 *                             0 means set log off
 * @param[in] logfile_roll_count The maximum number of log file numbers.
 *                               The value range is [0, 100], default 10.
 *                               0 means set log off
 * @return
 * - 0: Success.
 * - <0: Failure.
 */
extern __agora_api__ int agora_rtc_config_log(int size_per_file, int max_file_count);

/**
 * @brief Create a connection, connection can join channel
 * @param[out] conn_id: Store created connection id
 * @return
 *  - =0: Success
 *  - <0: Failure
 */
extern __agora_api__ int agora_rtc_create_connection(connection_id_t *conn_id);

/**
 * @brief Destroy a connection
 * @param[in] conn_id    : Connection identification
 * @return
 *  - =0: Success
 *  - <0: Failure
 */
extern __agora_api__ int agora_rtc_destroy_connection(connection_id_t conn_id);

/**
 * @brief Get a connection info
 * @param[in] conn_id    : Connection identification
 * @param[out] conn_info : Connection info
 * @return
 *  - =0: Success
 *  - <0: Failure
 */
extern __agora_api__ int agora_rtc_get_connection_info(connection_id_t conn_id, connection_info_t *conn_info);

/**
 * @brief Local user joins channel.
 * @note Users in the same channel with the same App ID can send data to each other.
 *       You can join more than one channel at the same time. All channels that
 *       you join will receive the audio/video data stream that you send unless
 *       you stop sending the audio/video data stream in a specific channel.
 * @param[in] conn_id      : Connection identification
 * @param[in] channel_name : Channel name
 *            Length=strlen(channel_name) should be less than 64 bytes
 *            Supported character scopes are:
 *            - The 26 lowercase English letters: a to z
 *            - The 26 uppercase English letters: A to Z
 *            - The 10 numbers: 0 to 9
 *            - The space
 *            - "!", "#", "$", "%", "&", "(", ")", "+", "-", ":", ";", "<",
 *              "=", ".", ">", "?", "@", "[", "]", "^", "_", " {", "}", "|", "~", ","
 * @param[in] uid   : User ID.
 *                   A 32-bit unsigned integer with a value ranging from 1 to 2^32-1. The uid must be unique.
 *                   If a uid is set to 0, the SDK assigns and returns a uid in the on_join_channel_success callback.
 *                     Your application must record and maintain the returned uid, because the SDK does not do so.
 * @param[in] token : Token string generated by the server, length=strlen(token) Range is [32, 512]
 *                      - if token authorization is enabled on developer website, it should be set correctly
 *                      - else token can be set as `NULL`
 * @param[in] options   channel options when create channel.
 *                      If do not set channel options, set NULL
 * @return
 * - = 0: Success
 * - < 0: Failure
 */
extern __agora_api__ int agora_rtc_join_channel(connection_id_t conn_id, const char *channel_name, uint32_t uid,
                                                const char *token, rtc_channel_options_t *options);

/**
 * @brief Allow Local user leaves channel.
 * @note Local user should leave channel when data transmission is stopped
 * @param[in] conn_id   : Connection identification
 * @return
 * - = 0: Success
 * - < 0: Failure
 */
extern __agora_api__ int agora_rtc_leave_channel(connection_id_t conn_id);

/**
 * @brief Renew token for specific channel OR all channels.
 * @note Token should be renewed when valid duration reached expiration.
 * @param[in] conn_id   Connection identification
 * @param[in] token     Token string, strlen(token) Range is [32, 512]
 * @return
 * - = 0: Success
 * - < 0: Failure
 */
extern __agora_api__ int agora_rtc_renew_token(connection_id_t conn_id, const char *token);

/**
 * Set network state
 * @note It should only be used on systems where the SDK is not aware of network events, such as Android11 and later.
 * @param event : network event, ref@network_event_type_e
 * @return =0: success; <0: failure
 */
extern __agora_api__ int agora_rtc_notify_network_event(network_event_type_e event);

/**
 * @brief Decide whether to enable/disable sending local audio data to specific channel OR all channels.
 * @param[in] conn_id   Connection identification, if set CONNECTION_ID_ALL(0) is for all connections
 * @param[in] mute      Toggle sending local audio
 *                      - false(0): unmuted
 *                      - true (1): muted
 * @return
 * - = 0: Success
 * - < 0: Failure
 */
extern __agora_api__ int agora_rtc_mute_local_audio(connection_id_t conn_id, bool mute);

#ifndef CONFIG_AUDIO_ONLY
/**
 * @brief Decide whether to enable/disable sending local video data to specific channel OR all channels.
 * @param[in] conn_id   Connection identification, if set CONNECTION_ID_ALL(0) is for all connections
 * @param[in] mute      Toggle sending local video
 *                      - false(0): unmuted
 *                      - true (1): muted
 * @return
 * - = 0: Success
 * - < 0: Failure
 */
extern __agora_api__ int agora_rtc_mute_local_video(connection_id_t conn_id, bool mute);
#endif
/**
 * @brief Decide whether to enable/disable receiving remote audio data from specific channel OR all channels.
 * @param[in] conn_id      Connection identification, if set CONNECTION_ID_ALL(0) is for all connections
 * @param[in] remote_uid    Remote user ID
 *                          - if `remote_uid` is set 0, it's for all users
 *                          - else it's for specific user
 * @param[in] mute          Toggle receiving remote audio
 *                          - false(0): unmuted
 *                          - true (1): muted
 * @return
 * - = 0: Success
 * - < 0: Failure
 */
extern __agora_api__ int agora_rtc_mute_remote_audio(connection_id_t conn_id, uint32_t remote_uid, bool mute);

#ifndef CONFIG_AUDIO_ONLY
/**
 * @brief Decide whether to enable/disable receiving remote video data from specific channel OR all channels.
 * @param[in] conn_id       Connection identification, if set CONNECTION_ID_ALL(0) is for all connections
 * @param[in] remote_uid    Remote user ID
 *                          - if `remote_uid` is set 0, it's for all users
 *                          - else it's for specific user
 * @param[in] mute          Toggle receiving remote video
 *                          - false(0): unmuted
 *                          - true (1): muted
 * @return
 * - = 0: Success
 * - < 0: Failure
 */
extern __agora_api__ int agora_rtc_mute_remote_video(connection_id_t conn_id, uint32_t remote_uid, bool mute);

/**
 * @brief Request remote user to generate a keyframe for all video streams OR specific video stream.
 * @param[in] conn_id      Connection identification
 * @param[in] remote_uid   Remote user ID
 *                         - if `remote_uid` is set 0, it's for all users
 *                         - else it's for specific user
 * @param[in] stream_type    Stream type
 * @return
 * - = 0: Success
 * - < 0: Failure
 */
extern __agora_api__ int agora_rtc_request_video_key_frame(connection_id_t conn_id, uint32_t remote_uid,
                                                           video_stream_type_e stream_type);
#endif
/**
 * @brief Send an audio frame to all channels OR specific channel.
 *        All remote users in this channel will receive the audio frame.
 * @note All channels that you joined will receive the audio frame that you send
 *       unless you stop sending the local audio to a specific channel.
 * @param[in] conn_id   Connection identification
 * @param[in] data_ptr  Audio frame buffer
 * @param[in] data_len  Audio frame buffer length (bytes)
 * @param[in] info_ptr  Audio frame info, see #audio_frame_info_t
 * @return
 * - = 0: Success
 * - < 0: Failure
 */
extern __agora_api__ int agora_rtc_send_audio_data(connection_id_t conn_id, const void *data_ptr, size_t data_len,
                                                   audio_frame_info_t *info_ptr);

#ifndef CONFIG_AUDIO_ONLY
/**
 * @brief Send a video frame to all channels OR specific channel.
 *        All remote users in the channel will receive the video frame.
 * @note All channels that you join will receive the video frame that you send
 *       unless you stop sending the local video to a specific channel.
 * @param[in] conn_id   Connection identification
 * @param[in] data_ptr  Video frame buffer
 * @param[in] data_len  Video frame buffer length (bytes)
 * @param[in] info_ptr  Video frame info
 * @return
 * - = 0: Success
 * - < 0: Failure
 */
extern __agora_api__ int agora_rtc_send_video_data(connection_id_t conn_id, const void *data_ptr, size_t data_len,
                                                   video_frame_info_t *info_ptr);
/**
 * @brief Set network bandwdith estimation (bwe) param
 * @param[in] conn_id   : Connection identification, if set CONNECTION_ID_ALL(0) is for all connections
 * @param[in] min_bps   : bwe min bps
 * @param[in] max_bps   : bwe max bps
 * @param[in] start_bps : bwe start bps
 *
 * @return:
 * - = 0: Success
 * - < 0: Failure
 */
extern __agora_api__ int agora_rtc_set_bwe_param(connection_id_t conn_id, uint32_t min_bps, uint32_t max_bps,
                                                 uint32_t start_bps);

#endif // End CONFIG_AUDIO_ONLY

#ifdef CONFIG_RTC_PROXY
/**
 * Set cloud proxy
 * @param [in] type : reference enum cloud_proxy_type_e
 * @return 0: Success, <0: failure
 */
extern __agora_api__ int agora_rtc_set_cloud_proxy(cloud_proxy_type_e type);
#endif // End CONFIG_RTC_PROXY

/**
 * Set config params
 *
 * @param [in] params : config params described by json
 * @note  supported sets are shown below, they can be together in params json string
 *  - {"rtc.encryption": {"enable": true/false, "master_key": "xxx..."}}
 *  - {"rtc_network": { "type": INT_xx, "id": INT_xx, "update": true|false }}
 * @return:
 *  - = 0: Success
 *  - < 0: Failure
 */
extern __agora_api__ int agora_rtc_set_params(const char *params);

#ifdef CONFIG_RTCM
/**
 * Send an message to all channels OR specific channel.
 *
 * All remote users in this channel will receive the message.
 *
 * @note All channels that you joined will receive the message that you send
 *       unless you stop sending the message to a specific channel.
 *
 * @param[in] conn_id       Connection identification
 * @param[in] remote_uid    Remote user ID
 *                          - if `remote_uid` is set 0, it's for all users
 *                          - else it's for specific use
 * @param[in] payload       Message's payload buffer
 * @param[in] length        Message's payload buffer length (max 1024 bytes)
 *
 * @return
 * - = 0: Success
 * - < 0: Failure
 */
extern __agora_api__ int agora_rtc_send_media_ctrl_msg(connection_id_t conn_id, uint32_t remote_uid,
                                                       const void *payload, size_t length);
#endif // End of CONFIG_RTCM

#ifdef CONFIG_RTM
/** RTM **/

/**
 * RTM event type list
 */
typedef enum {
  /**
   * 0: LOGIN
   */
  RTM_EVENT_TYPE_LOGIN = 0,
  /**
   * 1: KICKOFF
   */
  RTM_EVENT_TYPE_KICKOFF = 1,
  /**
   * 2: EXIT
   */
  RTM_EVENT_TYPE_EXIT = 2,
} rtm_event_type_e;

typedef enum {
  /** no error */
  ERR_RTM_OK = 0,
  /** general error */
  ERR_RTM_FAILED = 1,
  /** Login is rejected by the server. */
  ERR_RTM_LOGIN_REJECTED = 2,
  /**  invalid rtm uid */
  ERR_RTM_INVALID_RTM_UID = 3,
  /** The token is invalid. */
  ERR_RTM_LOGIN_INVALID_TOKEN = 5,
  /** Unauthorized login. */
  ERR_RTM_LOGIN_NOT_AUTHORIZED = 7,
  /**  invalid appid */
  ERR_RTM_INVALID_APP_ID = ERR_INVALID_APP_ID,
  /** The server rejected the request to look up the channel */
  WARN_RTM_LOOKUP_CHANNEL_REJECTED = ERR_LOOKUP_CHANNEL_REJECTED,
  /** Authorized Timestamp expired */
  ERR_RTM_TOKEN_EXPIRED = ERR_TOKEN_EXPIRED,
  /** invalid token */
  ERR_RTM_INVALID_TOKEN = ERR_INVALID_TOKEN,
} rtm_err_code_e;

/**
 * Agora RTC SDK RTM event handler
 */
typedef struct {
  /**
   * Occurs when data comes from RTM
   * @param[in] rtm_uid    The remote rtm uid which the data come from.
   * @param[in] msg        The Data received.
   * @param[in] msg_len    Length of the data received.
   */
  void (*on_rtm_data)(const char *rtm_uid, const void *msg, size_t msg_len);
  /**
   * Occurs when RTM event occurs
   * @param[in] rtm_uid    RTM UID
   * @param[in] event_type Event type
   * @param[in] event_info Event info of event type
   */
  void (*on_rtm_event)(const char *rtm_uid, rtm_event_type_e event_type, rtm_err_code_e err_code);
  /**
   * Report the result of the "agora_rtc_send_rtm_data" method call
   * @param[in] msg_id     Identify one message
   * @param[in] error_code Error code number
   *                       - 0 : success
   *                       - 1 : failure
   */
  void (*on_send_rtm_data_result)(uint32_t msg_id, rtm_err_code_e error_code);
} agora_rtm_handler_t;

/**
 * Login agora RTM service
 * @param[in] rtm_uid   RTM user id (different from uid)
 *                      Length should be less than 64 bytes
 *                      Supported character scopes are:
 *                      - The 26 lowercase English letters: a to z
 *                      - The 26 uppercase English letters: A to Z
 *                      - The 10 numbers: 0 to 9
 *                      - The space
 *                      - "!", "#", "$", "%", "&", "(", ")", "+", "-", ":",
 *                        ";", "<", "=", ".", ">", "?", "@", "[", "]", "^",
 *                        "_", " {", "}", "|", "~", ","
* @param[in] rtm_token  RTM token string generated by the token server (different from RTC token)
 *                      - if token authorization is enabled on developer website, it should be set correctly
 *                      - else token can be set as `NULL`
 * @param[in] hanlder   A set of callback that handles Agora RTM events
 *
 * @return
 * - = 0: Success
 * - < 0: Failure
 */
extern __agora_api__ int agora_rtc_login_rtm(const char *rtm_uid, const char *rtm_token,
                                             const agora_rtm_handler_t *handler);

/**
 * Logout agora RTM service
 *
 * @return
 * - = 0: Success
 * - < 0: Failure
 */

extern __agora_api__ int agora_rtc_logout_rtm(void);

/**
 * Send data through Real-time Messaging (RTM) mechanism, which is a stable and reliable data channel
 * @note RTM channel is not available by default, unless login success and callback `on_rtm_event`
         is triggered. The sending speed allowed is limited to 60 messages per second (60qps)
 *
 * @param[in] rtm_uid     RTM UID
 * @param[in] msg_id  Identify the message sent
 * @param[in] msg     Message to send
 * @param[in] msg_len Length of the message(max size: 32KB)
 *
 * @return:
 * - = 0: Success
 * - < 0: Failure
 */
extern __agora_api__ int agora_rtc_send_rtm_data(const char *rtm_uid, uint32_t msg_id, const void *msg, size_t msg_len);
#endif // End CONFIG_RTM

#ifdef __cplusplus
}
#endif

#endif /* __AGORA_RTC_API_H__ */
