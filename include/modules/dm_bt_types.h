// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef INCLUDE_MODULES_BK_DM_BT_TYPES_H_
#define INCLUDE_MODULES_BK_DM_BT_TYPES_H_

#include <modules/bt_types.h>


/// Discoverability and Connectability mode
typedef enum {
    BK_BT_NON_CONNECTABLE,             /*!< Non-connectable */
    BK_BT_CONNECTABLE,                 /*!< Connectable */
} bk_bt_conn_mode_t;

typedef enum {
    BK_BT_NON_DISCOVERABLE,            /*!< Non-discoverable */
    BK_BT_DISCOVERABLE,                /*!< Discoverable */
} bk_bt_disc_mode_t;

/*
 * @brief common class of device
 */
typedef enum
{
    COD_PHONE    = 0x40020C,
    COD_SOUNDBAR = 0x240414,
    COD_HEADSET  = 0x240404,
} common_cod_t;

/**
 * @brief A2DP callback events
 */
typedef enum {
    BK_A2DP_CONNECTION_STATE_EVT = 0,           /*!< connection state changed event */
    BK_A2DP_AUDIO_STATE_EVT,                    /*!< audio stream transmission state changed event */
    BK_A2DP_AUDIO_CFG_EVT,                      /*!< audio codec is configured, only used for A2DP SINK */
} bk_a2dp_cb_event_t;

/**
 * @brief Bluetooth A2DP connection states
 */
typedef enum {
    BK_A2DP_CONNECTION_STATE_DISCONNECTED = 0, /*!< connection released  */
    BK_A2DP_CONNECTION_STATE_CONNECTING,       /*!< connecting remote device */
    BK_A2DP_CONNECTION_STATE_CONNECTED,        /*!< connection established */
    BK_A2DP_CONNECTION_STATE_DISCONNECTING     /*!< disconnecting remote device */
} bk_a2dp_connection_state_t;

/**
 * @brief Bluetooth A2DP disconnection reason
 */
typedef enum {
    BK_A2DP_DISC_RSN_NORMAL = 0,               /*!< Finished disconnection that is initiated by local or remote device */
    BK_A2DP_DISC_RSN_ABNORMAL                  /*!< Abnormal disconnection caused by signal loss */
} bk_a2dp_disc_rsn_t;

/**
 * @brief Bluetooth A2DP datapath states
 */
typedef enum {
    BK_A2DP_AUDIO_STATE_SUSPEND = 0,           /*!< audio stream datapath suspend */
    BK_A2DP_AUDIO_STATE_STARTED,               /*!< audio stream datapath started */
} bk_a2dp_audio_state_t;

/**
 * @brief A2DP media codec capabilities union
 */
typedef struct {
    uint8_t type;                              /*!< A2DP media codec type */

    union {
        /**
         * @brief  SBC codec capabilities
         */
        struct
        {
            uint8_t channels;
            uint8_t channel_mode;
            uint8_t block_len;
            uint8_t subbands;
            uint32_t sample_rate;
            uint8_t bit_pool;
            uint8_t alloc_mode;
        }sbc_codec;

        /**
         * @brief  MPEG-2, 4 AAC codec capabilities
         */
        struct
        {
            uint8_t channels;
            uint32_t sample_rate;
        }aac_codec;
    } cie;                                     /*!< A2DP codec information element */
} __attribute__((packed)) bk_a2dp_mcc_t;

/**
 * @brief A2DP state callback parameters
 */
typedef union {
    /**
     * @brief  BK_A2DP_CONNECTION_STATE_EVT
     */
    struct a2dp_conn_state_param {
        bk_a2dp_connection_state_t state;      /*!< one of values from bk_a2dp_connection_state_t */
        uint8_t remote_bda[6];                 /*!< remote bluetooth device address */
        bk_a2dp_disc_rsn_t disc_rsn;           /*!< reason of disconnection for "DISCONNECTED" */
    } conn_state;                              /*!< A2DP connection status */

    /**
     * @brief BK_A2DP_AUDIO_STATE_EVT
     */
    struct a2dp_audio_state_param {
        bk_a2dp_audio_state_t state;           /*!< one of the values from bk_a2dp_audio_state_t */
        uint8_t remote_bda[6];                 /*!< remote bluetooth device address */
    } audio_state;                             /*!< audio stream playing state */

    /**
     * @brief BK_A2DP_AUDIO_CFG_EVT
     */
    struct a2dp_audio_cfg_param {
        uint8_t remote_bda[6];                 /*!< remote bluetooth device address */
        bk_a2dp_mcc_t mcc;                     /*!< A2DP media codec capability information */
    } audio_cfg;                               /*!< media codec configuration information */
}bk_a2dp_cb_param_t;


/*
 * @brief used in bk_bt_gap_set_event_callback, this enum show as "event", you must analyse param in same time
 */
typedef enum
{
    /// bt stack init ok, param NULL
    BK_DM_BT_EVENT_STACK_OK,

    /// inquiry result, not used now
    BK_DM_BT_EVENT_INQUIRY_RESULT,

    /// disconnect completed, not used now
    BK_DM_BT_EVENT_DISCONNECT,

    //BK_DM_BT_EVENT_CMD_COMPLETE,

    /// connection completed, not used now
    BK_DM_BT_EVENT_CONNECTION_COMPLETE,

    /// recv cb when pair success and get link key, param bk_bt_linkkey_storage_t
    BK_DM_BT_EVENT_LINKKEY_NOTIFY,

    /// recv cb when peer need user to input link key, param bd_addr_t
    BK_DM_BT_EVENT_LINKKEY_REQ,

} bt_event_enum_t;





/**
 * @brief bt link key storage
 */
typedef struct
{
    /// this struct size
    uint16_t size;

    /// bt addr
    //bd_addr_t addr;
    uint8_t addr[6];

    /// bt link key
    uint8_t link_key[16];//BT_LINK_KEY_SIZE];

}__attribute__((packed)) bk_bt_linkkey_storage_t;//SM_DEVICE_ENTITY

/// AVRCP event notification ids
typedef enum {
    BK_AVRCP_RN_PLAY_STATUS_CHANGE = 0x01,        /*!< track status change, eg. from playing to paused */
    BK_AVRCP_RN_TRACK_CHANGE = 0x02,              /*!< new track is loaded */
    BK_AVRCP_RN_TRACK_REACHED_END = 0x03,         /*!< current track reached end */
    BK_AVRCP_RN_TRACK_REACHED_START = 0x04,       /*!< current track reached start position */
    BK_AVRCP_RN_PLAY_POS_CHANGED = 0x05,          /*!< track playing position changed */
    BK_AVRCP_RN_BATTERY_STATUS_CHANGE = 0x06,     /*!< battery status changed */
    BK_AVRCP_RN_SYSTEM_STATUS_CHANGE = 0x07,      /*!< system status changed */
    BK_AVRCP_RN_APP_SETTING_CHANGE = 0x08,        /*!< application settings changed */
    BK_AVRCP_RN_NOW_PLAYING_CHANGE = 0x09,        /*!< now playing content changed */
    BK_AVRCP_RN_AVAILABLE_PLAYERS_CHANGE = 0x0a,  /*!< available players changed */
    BK_AVRCP_RN_ADDRESSED_PLAYER_CHANGE = 0x0b,   /*!< the addressed player changed */
    BK_AVRCP_RN_UIDS_CHANGE = 0x0c,               /*!< UIDs changed */
    BK_AVRCP_RN_VOLUME_CHANGE = 0x0d,             /*!< volume changed locally on TG */
    BK_AVRCP_RN_MAX_EVT
} bk_avrcp_rn_event_ids_t;

/// AVRCP Controller callback events
typedef enum {
    BK_AVRCP_CT_CONNECTION_STATE_EVT = 0,        /*!< connection state changed event */
    BK_AVRCP_CT_PASSTHROUGH_RSP_EVT = 1,         /*!< passthrough response event */
    BK_AVRCP_CT_METADATA_RSP_EVT = 2,            /*!< metadata response event */
    BK_AVRCP_CT_PLAY_STATUS_RSP_EVT = 3,         /*!< play status response event */
    BK_AVRCP_CT_CHANGE_NOTIFY_EVT = 4,           /*!< notification event */
    BK_AVRCP_CT_REMOTE_FEATURES_EVT = 5,         /*!< feature of remote device indication event */
    BK_AVRCP_CT_GET_RN_CAPABILITIES_RSP_EVT = 6,     /*!< supported notification events capability of peer device */
    BK_AVRCP_CT_SET_ABSOLUTE_VOLUME_RSP_EVT = 7, /*!< set absolute volume response event */
} bk_avrcp_ct_cb_event_t;

/// AVRCP battery status
typedef enum {
    BK_AVRCP_BATT_NORMAL       = 0,               /*!< normal state */
    BK_AVRCP_BATT_WARNING      = 1,               /*!< unable to operate soon */
    BK_AVRCP_BATT_CRITICAL     = 2,               /*!< cannot operate any more */
    BK_AVRCP_BATT_EXTERNAL     = 3,               /*!< plugged to external power supply */
    BK_AVRCP_BATT_FULL_CHARGE  = 4,               /*!< when completely charged from external power supply */
} bk_avrcp_batt_stat_t;

/// AVRCP current status of playback
typedef enum {
    BK_AVRCP_PLAYBACK_STOPPED = 0,                /*!< stopped */
    BK_AVRCP_PLAYBACK_PLAYING = 1,                /*!< playing */
    BK_AVRCP_PLAYBACK_PAUSED = 2,                 /*!< paused */
    BK_AVRCP_PLAYBACK_FWD_SEEK = 3,               /*!< forward seek */
    BK_AVRCP_PLAYBACK_REV_SEEK = 4,               /*!< reverse seek */
    BK_AVRCP_PLAYBACK_ERROR = 0xFF,               /*!< error */
} bk_avrcp_playback_stat_t;

/// AVRCP passthrough command code
typedef enum {
    BK_AVRCP_PT_CMD_SELECT         =   0x00,    /*!< select */
    BK_AVRCP_PT_CMD_UP             =   0x01,    /*!< up */
    BK_AVRCP_PT_CMD_DOWN           =   0x02,    /*!< down */
    BK_AVRCP_PT_CMD_LEFT           =   0x03,    /*!< left */
    BK_AVRCP_PT_CMD_RIGHT          =   0x04,    /*!< right */
    BK_AVRCP_PT_CMD_RIGHT_UP       =   0x05,    /*!< right-up */
    BK_AVRCP_PT_CMD_RIGHT_DOWN     =   0x06,    /*!< right-down */
    BK_AVRCP_PT_CMD_LEFT_UP        =   0x07,    /*!< left-up */
    BK_AVRCP_PT_CMD_LEFT_DOWN      =   0x08,    /*!< left-down */
    BK_AVRCP_PT_CMD_ROOT_MENU      =   0x09,    /*!< root menu */
    BK_AVRCP_PT_CMD_SETUP_MENU     =   0x0A,    /*!< setup menu */
    BK_AVRCP_PT_CMD_CONT_MENU      =   0x0B,    /*!< contents menu */
    BK_AVRCP_PT_CMD_FAV_MENU       =   0x0C,    /*!< favorite menu */
    BK_AVRCP_PT_CMD_EXIT           =   0x0D,    /*!< exit */
    BK_AVRCP_PT_CMD_0              =   0x20,    /*!< 0 */
    BK_AVRCP_PT_CMD_1              =   0x21,    /*!< 1 */
    BK_AVRCP_PT_CMD_2              =   0x22,    /*!< 2 */
    BK_AVRCP_PT_CMD_3              =   0x23,    /*!< 3 */
    BK_AVRCP_PT_CMD_4              =   0x24,    /*!< 4 */
    BK_AVRCP_PT_CMD_5              =   0x25,    /*!< 5 */
    BK_AVRCP_PT_CMD_6              =   0x26,    /*!< 6 */
    BK_AVRCP_PT_CMD_7              =   0x27,    /*!< 7 */
    BK_AVRCP_PT_CMD_8              =   0x28,    /*!< 8 */
    BK_AVRCP_PT_CMD_9              =   0x29,    /*!< 9 */
    BK_AVRCP_PT_CMD_DOT            =   0x2A,    /*!< dot */
    BK_AVRCP_PT_CMD_ENTER          =   0x2B,    /*!< enter */
    BK_AVRCP_PT_CMD_CLEAR          =   0x2C,    /*!< clear */
    BK_AVRCP_PT_CMD_CHAN_UP        =   0x30,    /*!< channel up */
    BK_AVRCP_PT_CMD_CHAN_DOWN      =   0x31,    /*!< channel down */
    BK_AVRCP_PT_CMD_PREV_CHAN      =   0x32,    /*!< previous channel */
    BK_AVRCP_PT_CMD_SOUND_SEL      =   0x33,    /*!< sound select */
    BK_AVRCP_PT_CMD_INPUT_SEL      =   0x34,    /*!< input select */
    BK_AVRCP_PT_CMD_DISP_INFO      =   0x35,    /*!< display information */
    BK_AVRCP_PT_CMD_HELP           =   0x36,    /*!< help */
    BK_AVRCP_PT_CMD_PAGE_UP        =   0x37,    /*!< page up */
    BK_AVRCP_PT_CMD_PAGE_DOWN      =   0x38,    /*!< page down */
    BK_AVRCP_PT_CMD_POWER          =   0x40,    /*!< power */
    BK_AVRCP_PT_CMD_VOL_UP         =   0x41,    /*!< volume up */
    BK_AVRCP_PT_CMD_VOL_DOWN       =   0x42,    /*!< volume down */
    BK_AVRCP_PT_CMD_MUTE           =   0x43,    /*!< mute */
    BK_AVRCP_PT_CMD_PLAY           =   0x44,    /*!< play */
    BK_AVRCP_PT_CMD_STOP           =   0x45,    /*!< stop */
    BK_AVRCP_PT_CMD_PAUSE          =   0x46,    /*!< pause */
    BK_AVRCP_PT_CMD_RECORD         =   0x47,    /*!< record */
    BK_AVRCP_PT_CMD_REWIND         =   0x48,    /*!< rewind */
    BK_AVRCP_PT_CMD_FAST_FORWARD   =   0x49,    /*!< fast forward */
    BK_AVRCP_PT_CMD_EJECT          =   0x4A,    /*!< eject */
    BK_AVRCP_PT_CMD_FORWARD        =   0x4B,    /*!< forward */
    BK_AVRCP_PT_CMD_BACKWARD       =   0x4C,    /*!< backward */
    BK_AVRCP_PT_CMD_ANGLE          =   0x50,    /*!< angle */
    BK_AVRCP_PT_CMD_SUBPICT        =   0x51,    /*!< subpicture */
    BK_AVRCP_PT_CMD_F1             =   0x71,    /*!< F1 */
    BK_AVRCP_PT_CMD_F2             =   0x72,    /*!< F2 */
    BK_AVRCP_PT_CMD_F3             =   0x73,    /*!< F3 */
    BK_AVRCP_PT_CMD_F4             =   0x74,    /*!< F4 */
    BK_AVRCP_PT_CMD_F5             =   0x75,    /*!< F5 */
    BK_AVRCP_PT_CMD_VENDOR         =   0x7E,    /*!< vendor unique */
} bk_avrcp_pt_cmd_t;

/// AVRCP passthrough command state
typedef enum {
    BK_AVRCP_PT_CMD_STATE_PRESSED = 0,           /*!< key pressed */
    BK_AVRCP_PT_CMD_STATE_RELEASED = 1           /*!< key released */
} bk_avrcp_pt_cmd_state_t;

/// AVRCP notification parameters
typedef union
{
    uint8_t volume;                          /*!< response data for BK_AVRCP_RN_VOLUME_CHANGE, ranges 0..127 */
    bk_avrcp_playback_stat_t playback;       /*!< response data for BK_AVRCP_RN_PLAY_STATUS_CHANGE */
    uint8_t elm_id[8];                       /*!< response data for BK_AVRCP_RN_TRACK_CHANGE */
    uint32_t play_pos;                       /*!< response data for BK_AVRCP_RN_PLAY_POS_CHANGED, in millisecond */
    bk_avrcp_batt_stat_t batt;               /*!< response data for BK_AVRCP_RN_BATTERY_STATUS_CHANGE */
} bk_avrcp_rn_param_t;

/// AVRCP target notification event capability bit mask
typedef struct {
    uint16_t bits;                                /*!< bit mask representation of supported event_ids */
} bk_avrcp_rn_evt_cap_mask_t;

/// AVRCP controller callback parameters
typedef union {
    /**
     * @brief BK_AVRCP_CT_CONNECTION_STATE_EVT
     */
    struct avrcp_ct_conn_state_param {
        uint8_t connected;                          /*!< whether AVRCP connection is set up */
        uint8_t remote_bda[6];                /*!< remote bluetooth device address */
    } conn_state;                                 /*!< AVRCP connection status */

    /**
     * @brief BK_AVRCP_CT_CHANGE_NOTIFY_EVT
     */
    struct avrcp_ct_change_notify_param {
        uint8_t event_id;                        /*!< id of AVRCP event notification */
        bk_avrcp_rn_param_t event_parameter;     /*!< event notification parameter */
    } change_ntf;                                /*!< notifications */

    /**
     * @brief BK_AVRCP_CT_GET_RN_CAPABILITIES_RSP_EVT
     */
    struct avrcp_ct_get_rn_caps_rsp_param {
        uint8_t cap_count;                       /*!< number of items provided in event or company_id according to cap_id used */
        bk_avrcp_rn_evt_cap_mask_t evt_set;      /*!< supported event_ids represented in bit-mask */
    } get_rn_caps_rsp;                           /*!< get supported event capabilities response from AVRCP target */

} bk_avrcp_ct_cb_param_t;


/**
 * @brief           A2DP profile callback function type
 *
 * @param           event : Event type
 *
 * @param           param : Pointer to callback parameter
 */
typedef void (* bk_bt_a2dp_cb_t)(bk_a2dp_cb_event_t event, bk_a2dp_cb_param_t *param);

/**
 * @brief           A2DP sink data callback function
 *
 * @param[in]       buf : pointer to the data received from A2DP source device
 *
 * @param[in]       len : size(in bytes) in buf
 */
typedef void (* bk_bt_sink_data_cb_t)(const uint8_t *buf, uint16_t len);

/**
 * @brief           AVRCP controller callback function type
 *
 * @param           event : Event type
 *
 * @param           param : Pointer to callback parameter union
 */
typedef void (* bk_avrcp_ct_cb_t)(bk_avrcp_ct_cb_event_t event, bk_avrcp_ct_cb_param_t *param);



/**
 * @brief for async dm_bt api event.
 *
 * bt event report.
 *
 * @param
 * - event: event id. see bt_event_enum_t
 * - param: param
 *
**/
typedef uint32_t (*bt_event_cb_t)(bt_event_enum_t event, void *param);

#endif
