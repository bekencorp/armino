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



#pragma once

#include "modules/bluetooth/dm_bt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup dm_gatt_types_v1 GATT DEFINES
 * @{
 */


/**
 * gatt uuid type, All "BK_GATT_UUID_xxx" is attribute types
 */
typedef enum
{
    BK_GATT_UUID_IMMEDIATE_ALERT_SVC =           0x1802,          ///  Immediate alert Servic
    BK_GATT_UUID_LINK_LOSS_SVC =                 0x1803,          ///  Link Loss Servic
    BK_GATT_UUID_TX_POWER_SVC =                  0x1804,          ///  TX Power Servic
    BK_GATT_UUID_CURRENT_TIME_SVC =              0x1805,          ///  Current Time Service Servic
    BK_GATT_UUID_REF_TIME_UPDATE_SVC =           0x1806,          ///  Reference Time Update Servic
    BK_GATT_UUID_NEXT_DST_CHANGE_SVC =           0x1807,          ///  Next DST Change Servic
    BK_GATT_UUID_GLUCOSE_SVC =                   0x1808,          ///  Glucose Servic
    BK_GATT_UUID_HEALTH_THERMOM_SVC =            0x1809,          ///  Health Thermometer Servic
    BK_GATT_UUID_DEVICE_INFO_SVC =               0x180A,          ///  Device Information Servic
    BK_GATT_UUID_HEART_RATE_SVC =                0x180D,          ///  Heart Rate Servic
    BK_GATT_UUID_PHONE_ALERT_STATUS_SVC =        0x180E,          /// Phone Alert Status Servic
    BK_GATT_UUID_BATTERY_SERVICE_SVC =           0x180F,          /// Battery Servic
    BK_GATT_UUID_BLOOD_PRESSURE_SVC =            0x1810,          /// Blood Pressure Servic
    BK_GATT_UUID_ALERT_NTF_SVC =                 0x1811,          /// Alert Notification Servic
    BK_GATT_UUID_HID_SVC =                       0x1812,          /// HID Servic
    BK_GATT_UUID_SCAN_PARAMETERS_SVC =           0x1813,          /// Scan Parameters Servic
    BK_GATT_UUID_RUNNING_SPEED_CADENCE_SVC =     0x1814,          /// Running Speed and Cadence Servic
    BK_GATT_UUID_Automation_IO_SVC =             0x1815,          /// Automation IO Servic
    BK_GATT_UUID_CYCLING_SPEED_CADENCE_SVC =     0x1816,          /// Cycling Speed and Cadence Servic
    BK_GATT_UUID_CYCLING_POWER_SVC =             0x1818,          /// Cycling Power Servic
    BK_GATT_UUID_LOCATION_AND_NAVIGATION_SVC =   0x1819,          /// Location and Navigation Servic
    BK_GATT_UUID_ENVIRONMENTAL_SENSING_SVC =     0x181A,          /// Environmental Sensing Servic
    BK_GATT_UUID_BODY_COMPOSITION =              0x181B,          /// Body Composition Servic
    BK_GATT_UUID_USER_DATA_SVC =                 0x181C,          /// User Data Servic
    BK_GATT_UUID_WEIGHT_SCALE_SVC =              0x181D,          /// Weight Scale Servic
    BK_GATT_UUID_BOND_MANAGEMENT_SVC =           0x181E,          /// Bond Management Servic
    BK_GATT_UUID_CONT_GLUCOSE_MONITOR_SVC =      0x181F,          /// Continuous Glucose Monitoring Servic

    /// Declarations
    BK_GATT_UUID_PRI_SERVICE =                   0x2800,
    BK_GATT_UUID_SEC_SERVICE =                   0x2801,
    BK_GATT_UUID_INCLUDE_SERVICE =               0x2802,
    BK_GATT_UUID_CHAR_DECLARE =                  0x2803,          ///  Characteristic Declaratio

    /// Descriptors
    BK_GATT_UUID_CHAR_EXT_PROP =                 0x2900,          ///  Characteristic Extended Properties
    BK_GATT_UUID_CHAR_DESCRIPTION =              0x2901,          ///  Characteristic User Descriptio
    BK_GATT_UUID_CHAR_CLIENT_CONFIG =            0x2902,          ///  Client Characteristic Configuration
    BK_GATT_UUID_CHAR_SRVR_CONFIG =              0x2903,          ///  Server Characteristic Configuration
    BK_GATT_UUID_CHAR_PRESENT_FORMAT =           0x2904,          ///  Characteristic Presentation Forma
    BK_GATT_UUID_CHAR_AGG_FORMAT =               0x2905,          ///  Characteristic Aggregate Forma
    BK_GATT_UUID_CHAR_VALID_RANGE =              0x2906,          ///  Characteristic Valid Range
    BK_GATT_UUID_EXT_RPT_REF_DESCR =             0x2907,          ///  External Report Reference
    BK_GATT_UUID_RPT_REF_DESCR =                 0x2908,          ///  Report Reference
    BK_GATT_UUID_NUM_DIGITALS_DESCR =            0x2909,          ///  Number of Digitals
    BK_GATT_UUID_VALUE_TRIGGER_DESCR =           0x290A,          ///  Value Trigger Setting
    BK_GATT_UUID_ENV_SENSING_CONFIG_DESCR =      0x290B,          ///  Environmental Sensing Configuration
    BK_GATT_UUID_ENV_SENSING_MEASUREMENT_DESCR = 0x290C,          ///  Environmental Sensing Measurement
    BK_GATT_UUID_ENV_SENSING_TRIGGER_DESCR =     0x290D,          ///  Environmental Sensing Trigger Setting
    BK_GATT_UUID_TIME_TRIGGER_DESCR =            0x290E,          ///  Time Trigger Setting
    BK_GATT_UUID_BR_EDR_TRANS_BLOCK_DATA =       0x290F,          ///  Complete BRT-EDR Transport BLock Data

    /// GAP Profile Attributes
    BK_GATT_UUID_GAP_DEVICE_NAME =               0x2A00,
    BK_GATT_UUID_GAP_ICON =                      0x2A01,
    BK_GATT_UUID_GAP_PREF_CONN_PARAM =           0x2A04,
    BK_GATT_UUID_GAP_CENTRAL_ADDR_RESOL =        0x2AA6,

    /// Attribute Profile Attribute UUID
    BK_GATT_UUID_GATT_SRV_CHGD =                 0x2A05,

    /// Link BK_Loss Service
    BK_GATT_UUID_ALERT_LEVEL =                   0x2A06,          /// Alert Level
    BK_GATT_UUID_TX_POWER_LEVEL =                0x2A07,          /// TX power level

    /// Current Time Service
    BK_GATT_UUID_CURRENT_TIME =                  0x2A2B,          /// Current Time
    BK_GATT_UUID_LOCAL_TIME_INFO =               0x2A0F,          /// Local time info
    BK_GATT_UUID_REF_TIME_INFO =                 0x2A14,          /// reference time information

    /// Network availability Profile
    BK_GATT_UUID_NW_STATUS =                     0x2A18,          /// network availability status
    BK_GATT_UUID_NW_TRIGGER =                    0x2A1A,          /// Network availability trigger

    /// Phone alert
    BK_GATT_UUID_ALERT_STATUS =                  0x2A3F,          /// alert status
    BK_GATT_UUID_RINGER_CP =                     0x2A40,          /// ringer control point
    BK_GATT_UUID_RINGER_SETTING =                0x2A41,          /// ringer setting

    /// Glucose Service
    BK_GATT_UUID_GM_MEASUREMENT =                0x2A18,
    BK_GATT_UUID_GM_CONTEXT =                    0x2A34,
    BK_GATT_UUID_GM_CONTROL_POINT =              0x2A52,
    BK_GATT_UUID_GM_FEATURE =                    0x2A51,

    /// device information characteristic
    BK_GATT_UUID_SYSTEM_ID =                     0x2A23,
    BK_GATT_UUID_MODEL_NUMBER_STR =              0x2A24,
    BK_GATT_UUID_SERIAL_NUMBER_STR =             0x2A25,
    BK_GATT_UUID_FW_VERSION_STR =                0x2A26,
    BK_GATT_UUID_HW_VERSION_STR =                0x2A27,
    BK_GATT_UUID_SW_VERSION_STR =                0x2A28,
    BK_GATT_UUID_MANU_NAME =                     0x2A29,
    BK_GATT_UUID_IEEE_DATA =                     0x2A2A,
    BK_GATT_UUID_PNP_ID =                        0x2A50,

    /// HID characteristics
    BK_GATT_UUID_HID_INFORMATION =               0x2A4A,
    BK_GATT_UUID_HID_REPORT_MAP =                0x2A4B,
    BK_GATT_UUID_HID_CONTROL_POINT =             0x2A4C,
    BK_GATT_UUID_HID_REPORT =                    0x2A4D,
    BK_GATT_UUID_HID_PROTO_MODE =                0x2A4E,
    BK_GATT_UUID_HID_BT_KB_INPUT =               0x2A22,
    BK_GATT_UUID_HID_BT_KB_OUTPUT =              0x2A32,
    BK_GATT_UUID_HID_BT_MOUSE_INPUT =            0x2A33,

    /// Heart Rate Measurement
    BK_GATT_HEART_RATE_MEAS =                 0x2A37,
    /// Body Sensor Location
    BK_GATT_BODY_SENSOR_LOCATION =            0x2A38,
    /// Heart Rate Control Point
    BK_GATT_HEART_RATE_CNTL_POINT =           0x2A39,

    /// Battery Service characteristics
    BK_GATT_UUID_BATTERY_LEVEL =                 0x2A19,

    /// Sensor Service
    BK_GATT_UUID_SC_CONTROL_POINT =              0x2A55,
    BK_GATT_UUID_SENSOR_LOCATION =               0x2A5D,

    /// Runners speed and cadence service
    BK_GATT_UUID_RSC_MEASUREMENT =               0x2A53,
    BK_GATT_UUID_RSC_FEATURE =                   0x2A54,

    /// Cycling speed and cadence service
    BK_GATT_UUID_CSC_MEASUREMENT =               0x2A5B,
    BK_GATT_UUID_CSC_FEATURE =                   0x2A5C,

    /// Scan BK_Parameter characteristics
    BK_GATT_UUID_SCAN_INT_WINDOW =               0x2A4F,
    BK_GATT_UUID_SCAN_REFRESH =                  0x2A31,
} bk_gatt_uuid_enum_t;



/**
 * @brief GATT success code and error codes
 */
typedef enum
{
    BK_GATT_OK                     =   0x00,
    BK_GATT_INVALID_HANDLE         =   0x01,
    BK_GATT_READ_NOT_PERMIT        =   0x02,
    BK_GATT_WRITE_NOT_PERMIT       =   0x03,
    BK_GATT_INVALID_PDU            =   0x04,
    BK_GATT_INSUF_AUTHENTICATION   =   0x05,
    BK_GATT_REQ_NOT_SUPPORTED      =   0x06,
    BK_GATT_INVALID_OFFSET         =   0x07,
    BK_GATT_INSUF_AUTHORIZATION    =   0x08,
    BK_GATT_PREPARE_Q_FULL         =   0x09,
    BK_GATT_NOT_FOUND              =   0x0a,
    BK_GATT_NOT_LONG               =   0x0b,
    BK_GATT_INSUF_KEY_SIZE         =   0x0c,
    BK_GATT_INVALID_ATTR_LEN       =   0x0d,
    BK_GATT_ERR_UNLIKELY           =   0x0e,
    BK_GATT_INSUF_ENCRYPTION       =   0x0f,
    BK_GATT_UNSUPPORT_GRP_TYPE     =   0x10,
    BK_GATT_INSUF_RESOURCE         =   0x11,
    BK_GATT_DB_OUT_OF_SYNC         =   0x12,
    BK_GATT_PARAM_VAL_NOT_ALLOW    =   0x13,

    // for css
    BK_GATT_WRITE_REQ_REJECT       =   0xfc,   /// Write Request Rejected
    BK_GATT_CCC_CFG_ERR            =   0xfd,   /// 0xFD Client Characteristic Configuration Descriptor Improperly Configured
    BK_GATT_PRC_IN_PROGRESS        =   0xfe,   /// 0xFE Procedure Already in progress
    BK_GATT_OUT_OF_RANGE           =   0xff,   /// 0xFF Attribute value out of range */
} bk_gatt_status_t;


/**
 * @brief Gatt Connection reason enum
 */
typedef enum
{
    BK_GATT_CONN_UNKNOWN = 0,                      /*!< Gatt connection unknown */
} bk_gatt_conn_reason_t;

/**
 * @brief Gatt id, include uuid and instance id
 */
typedef struct
{
    bk_bt_uuid_t   uuid;                   /*!< UUID */
} __attribute__((packed)) bk_gatt_id_t;

/**
 * @brief Gatt service id, include id
 *        (uuid and instance id) and primary flag
 */
typedef struct
{
    bk_gatt_id_t   id;                     /*!< Gatt id, include uuid and instance */
    bool            is_primary;             /*!< This service is primary or not */
} __attribute__((packed)) bk_gatt_srvc_id_t;


/**
 * @brief Gatt authentication request type
 */
typedef enum
{
    BK_GATT_AUTH_REQ_NONE                  = 0,
} bk_gatt_auth_req_t;


/**
 * @brief Attribute permissions
 */
typedef enum
{
    BK_GATT_PERM_READ =                  (1 << 0),   /* bit 0 -  0x0001 */
    BK_GATT_PERM_READ_ENCRYPTED =        (1 << 1),   /* bit 1 -  0x0002 */
    BK_GATT_PERM_READ_ENC_MITM =         (1 << 2),   /* bit 2 -  0x0004 */
    BK_GATT_PERM_WRITE =                 (1 << 4),   /* bit 4 -  0x0010 */
    BK_GATT_PERM_WRITE_ENCRYPTED =       (1 << 5),   /* bit 5 -  0x0020 */
    BK_GATT_PERM_WRITE_ENC_MITM =        (1 << 6),   /* bit 6 -  0x0040 */
    BK_GATT_PERM_WRITE_SIGNED =          (1 << 7),   /* bit 7 -  0x0080 */
    BK_GATT_PERM_WRITE_SIGNED_MITM =     (1 << 8),   /* bit 8 -  0x0100 */
    BK_GATT_PERM_READ_AUTHORIZATION =    (1 << 9),   /* bit 9 -  0x0200 */
    BK_GATT_PERM_WRITE_AUTHORIZATION =   (1 << 10),  /* bit 10 - 0x0400 */
} bk_gatt_perm_enum_t;


/// see bk_gatt_perm_enum_t
typedef uint16_t bk_gatt_perm_t;


/// definition of characteristic properties
typedef enum
{
    BK_GATT_CHAR_PROP_BIT_BROADCAST =    (1 << 0),       /* 0x01 */
    BK_GATT_CHAR_PROP_BIT_READ =         (1 << 1),       /* 0x02 */
    BK_GATT_CHAR_PROP_BIT_WRITE_NR =     (1 << 2),       /* 0x04 */
    BK_GATT_CHAR_PROP_BIT_WRITE =        (1 << 3),       /* 0x08 */
    BK_GATT_CHAR_PROP_BIT_NOTIFY =       (1 << 4),       /* 0x10 */
    BK_GATT_CHAR_PROP_BIT_INDICATE =     (1 << 5),       /* 0x20 */
    BK_GATT_CHAR_PROP_BIT_AUTH =         (1 << 6),       /* 0x40 */
    BK_GATT_CHAR_PROP_BIT_EXT_PROP =     (1 << 7),       /* 0x80 */
} bk_gatt_char_prop_enum_t;

/// see bk_gatt_char_prop_enum_t
typedef uint8_t bk_gatt_char_prop_t;



/**
  * @brief set the attribute value type
  */
typedef struct
{
    uint16_t attr_max_len;                                  /*!<  attribute max value length */
    uint16_t attr_len;                                      /*!<  attribute current value length */
    uint8_t  *attr_value;                                   /*!<  the pointer to attribute value */
} bk_attr_value_t;

/**
 * @brief Attribute description (used to create database)
 */
typedef struct
{
    bk_bt_uuid_t attr_type;                      /// attr type
    bk_bt_uuid_t attr_content;                   /// attr content
    bk_attr_value_t value;                       /// Element value if attr_type is char or desc
    bk_gatt_char_prop_t prop;                    ///  char prop, only used if attr_type is char
    bk_gatt_perm_enum_t perm;                     /*!< Attribute permission not used if attr_type is service*/
} bk_attr_desc_t;


/**
 * @brief attribute auto response flag
 */
typedef struct
{
#define BK_GATT_RSP_BY_APP             0
#define BK_GATT_AUTO_RSP               1
    /**
     * @brief if auto_rsp set to BK_GATT_RSP_BY_APP, means the response of Write/Read operation will by replied by application.
              if auto_rsp set to BK_GATT_AUTO_RSP, means the response of Write/Read operation will be replied by GATT stack automatically.
     */
    uint8_t auto_rsp;
} bk_attr_control_t;


/**
 * @brief attribute type added to the gatt server database
 */
typedef struct
{
    bk_attr_desc_t         att_desc;                       /*!< The attribute type */
    bk_attr_control_t      attr_control;                   /*!< The attribute control type, not used if attr type is service*/
} bk_gatts_attr_db_t;




/// Gatt attribute value
typedef struct
{
    uint8_t           *value;                               /*!< Gatt attribute value */
    uint16_t          handle;                               /*!< Gatt attribute handle */
    uint16_t          offset;                               /*!< Gatt attribute value offset */
    uint16_t          len;                                  /*!< Gatt attribute value length */
    bk_gatt_auth_req_t auth_req;                             /*!< Gatt authentication request see bk_gatt_auth_req_t*/
} bk_gatt_value_t;

/// GATT remote read request response type
typedef struct
{
    bk_gatt_value_t attr_value;                            /*!< Gatt attribute structure */
} bk_gatt_rsp_t;


/**
  * @brief Connection parameters information
  */
typedef struct
{
    uint16_t             interval;                         /*!< connection interval */
    uint16_t             latency;                          /*!< Slave latency for the connection in number of connection events. Range: 0x0000 to 0x01F3 */
    uint16_t             timeout;                          /*!< Supervision timeout for the LE Link. Range: 0x000A to 0x0C80.
                                                                Mandatory Range: 0x000A to 0x0C80 Time = N * 10 msec
                                                                Time Range: 100 msec to 32 seconds */
} bk_gatt_conn_params_t;


typedef uint8_t    bk_gatt_if_t;                           /*!< Gatt interface type, different application on GATT client use different gatt_if */

///@}

#ifdef __cplusplus
}
#endif

