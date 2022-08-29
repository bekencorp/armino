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
#include "modules/ble.h"

// no used
//#define BK_BLE_MAX_SCAN_NUM               (15)
//#define BK_BLE_ABIT(n) (1 << n)



/**
 * @brief for hci raw data callback.
 *
 * stack report type to upper
 *
 * @param
 * - buf: payload, see BK_BLE_HCI_TYPE as buf[0]
 * - len: buf's len
 *
 * @return
 * - BK_ERR_BLE_SUCCESS: succeed
**/
typedef ble_err_t (*ble_hci_raw_to_host_cb)(uint8_t *buf, uint16_t len);



//bluetooth new api for ethermind

#ifndef _H_APPL_SERVICE_ // ethermind api, avoid redefine
#define _H_APPL_SERVICE_


//copy from BT_gatt_db_api.h

//#include "ethermind_export/BT_gatt_db_api.h"
#include "ethermind_export/gatt_defines.h"




typedef uint16_t    API_RESULT;

/** ATT Default MTU */
//#define ATT_DEFAULT_MTU                   23U

/** ATT Signature Size in Signed Write */
#define ATT_AUTH_SIGNATURE_SIZE           12U

/** ATT Execute Write Cancel Flag */
#define ATT_EXECUTE_WRITE_CANCEL_FLAG   0x00U

/** ATT Execute Write Execute Flag */
#define ATT_EXECUTE_WRITE_EXEC_FLAG     0x01U

/** ATT Identification for 16-bit UUID Format */
#define ATT_16_BIT_UUID_FORMAT          0x01U

/** ATT Identification for 128-bit UUID Format */
#define ATT_128_BIT_UUID_FORMAT         0x02U

/** ATT 16-bit UUID Size */
#define ATT_16_BIT_UUID_SIZE               2U

/** ATT 128-bit UUID Size */
#define ATT_128_BIT_UUID_SIZE             16U

/** ATT Connection Instance Initialization Value. */
#define ATT_CON_ID_INIT_VAL             0xFFU

/** ATT Application Callback Initialization Value. */
#define ATT_APPL_CB_INIT_VAL               NULL

/** ATT Invalid Attribute Handle Value */
#define ATT_INVALID_ATTR_HANDLE_VAL      0x0000U

/** ATT Attribute Handle Start Range */
#define ATT_ATTR_HANDLE_START_RANGE      0x0001U

/** ATT Attribute Handle End Range */
#define ATT_ATTR_HANDLE_END_RANGE        0xFFFFU








typedef uint32_t GATT_DB_SERVICE_DESC;

/** Abstracts 16 Bit UUID */
typedef uint16_t ATT_UUID16;

/** Abstracts 128 Bit UUID */
typedef struct
{
    uint8_t   value[ATT_128_BIT_UUID_SIZE];
} ATT_UUID128;


typedef union
{
    ATT_UUID16    uuid_16;
    ATT_UUID128   uuid_128;

} ATT_UUID;

typedef struct
{
    /** Value to be packed */
    uint8_t     *val;

    /** Length of Value */
    uint16_t   len;

    /** Out Parameter Indicating Actual Length Packed */
    uint16_t   actual_len;

} ATT_VALUE;


typedef struct _GATT_DB_UUID_TYPE
{
    /** Format indicating, 16 bit or 128 bit UUIDs */
    uint8_t              uuid_format;

    /** Attribute UUID */
    ATT_UUID           uuid;

} GATT_DB_UUID_TYPE;

/** Service Information */
typedef struct _GATT_DB_SERVICE_INFO
{
    /** UUID */
    GATT_DB_UUID_TYPE    uuid;

    /** If this service is primary or not */
    uint8_t                is_primary;

    /**
     * Security Requrirements for the Service
     * Logical OR of desired combinations of
     * 1. Security Mode
     * 2. Security Level
     * 3. Encryption Key Size
     *
     * Acceptable values for Security Mode are:
     * \ref GATT_DB_SER_SECURITY_MODE1
     * \ref GATT_DB_SER_SECURITY_MODE2
     *
     * Acceptable values for Security Level are:
     * \ref GATT_DB_SER_SECURITY_LEVEL1
     * \ref GATT_DB_SER_SECURITY_LEVEL2
     * \ref GATT_DB_SER_SECURITY_LEVEL3
     *
     * Acceptable values for Encryption Key Size are:
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_7
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_8
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_9
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_10
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_11
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_12
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_13
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_14
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_15
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_16
     * When the Service does not mandate any specific Key size
     * \ref GATT_DB_SER_ENC_KEY_SIZE_DONT_CARE shall be used.
     *
     * When No Security is mandated for the service
     * Value \ref GATT_DB_SER_NO_SECURITY_PROPERTY shall be used.
     */
    GATT_DB_SERVICE_DESC sec_req;

    /**
     * Transport Requrirements for the Service.
     * This describes the Transport on which this required
     * to be operational.
     * \ref GATT_DB_SER_SUPPORT_ANY_LINK_TYPE
     * \ref GATT_DB_SER_SUPPORT_LE_LINK_TYPE
     * \ref GATT_DB_SER_SUPPORT_BR_LINK_TYPE
     */
    GATT_DB_SERVICE_DESC link_req;

} GATT_DB_SERVICE_INFO;



/** Read operation for Attribute  */
#define GATT_DB_READ                            0x01U

/** Write operation for Attribute */
#define GATT_DB_WRITE                           0x02U

/** Read blob operation for Attribute */
#define GATT_DB_READ_BLOB                       0x03U

/** Write Without Response operation for Attribute */
#define GATT_DB_WRITE_WITHOUT_RSP               0x04U

/** Read by UUID/Type operation for Attribute */
#define GATT_DB_READ_BY_TYPE                    0x05U

/** Signed Write operation for Attribute */
#define GATT_DB_SIGNED_WRITE                    0x06U

/** Execute Write operation for Attribute */
#define GATT_DB_EXECUTE                         0x07U

/** Prepare Write operation for Attribute */
#define GATT_DB_PREPARE                         0x08U

/**
 *  Local update of a Readable Attribute Value using \ref GATT_DB_HANDLE instead
 *  of Attribute Handle. See \ref BT_gatt_db_set_char_val for more details.
 */
#define GATT_DB_UPDATE                          0x10U

/* 128 Bit Attribute UUID  */
#define GATT_DB_128_BIT_UUID_FORMAT             0x20U

/**
 *  Peer initiated operation, needed when Peer is Reading or Writing Values
 *  either using GATT Read Procedures for Value and Descriptors.
 */
#define GATT_DB_PEER_INITIATED                  0x80U

/**
 *  Locally initiated operation, needed for local updates based on Attribute
 *  Handles. In case \ref GATT_DB_HANDLE is known, use of access through
 *  \ref GATT_DB_UPDATE is recommended.
 */
#define GATT_DB_LOCALLY_INITIATED               0x00U

/** \} */

/**
 *  \defgroup gatt_db_operations Operations Notified In Characteristic Callback
 *  \{
 *  This section describes the operations notified by the module to the
 *  application through the callback registered by the application.
 */

/** Characteristic Value Local Read Operation */
#define GATT_DB_CHAR_VALUE_READ_REQ             0x01U

/** Characteristic Value Local Write Operation */
#define GATT_DB_CHAR_VALUE_WRITE_REQ            0x02U

/** Characteristic Value Local Write Without Response Operation */
#define GATT_DB_CHAR_VALUE_WRITE_WITHOUT_REQ    0x03U

/** Characteristic Client Configuration Local Read Operation */
#define GATT_DB_CHAR_CLI_CNFG_READ_REQ          0x11U

/** Characteristic Client Configuration Local Write Operation */
#define GATT_DB_CHAR_CLI_CNFG_WRITE_REQ         0x12U

/** Characteristic Server Configuration Local Read Operation */
#define GATT_DB_CHAR_SER_CNFG_READ_REQ          0x21U

/** Characteristic Server Configuration Local Write Operation */
#define GATT_DB_CHAR_SER_CNFG_WRITE_REQ         0x22U

/** Characteristic Value Peer Read Operation */
#define GATT_DB_CHAR_PEER_READ_REQ              (GATT_DB_PEER_INITIATED | GATT_DB_READ)

/** Characteristic Value Peer Write Operation */
#define GATT_DB_CHAR_PEER_WRITE_REQ             (GATT_DB_PEER_INITIATED | GATT_DB_WRITE)

/** Characteristic Value Peer Read Blob Operation */
#define GATT_DB_CHAR_PEER_READ_BLOB_REQ         (GATT_DB_PEER_INITIATED | GATT_DB_READ_BLOB)

/** Characteristic Value Peer Write Command */
#define GATT_DB_CHAR_PEER_WRITE_CMD             (GATT_DB_PEER_INITIATED | GATT_DB_WRITE_WITHOUT_RSP)

/** Characteristic Value Peer Read by Type Operation */
#define GATT_DB_CHAR_PEER_READ_BY_TYPE_REQ      (GATT_DB_PEER_INITIATED | GATT_DB_READ_BY_TYPE)

/** Characteristic Value Peer Signed Write Operation */
#define GATT_DB_CHAR_PEER_SIGNED_WRITE_CMD      (GATT_DB_PEER_INITIATED | GATT_DB_SIGNED_WRITE)

/** Characteristic Value Peer Execute Write Req */
#define GATT_DB_CHAR_PEER_EXECUTE_WRITE_REQ     (GATT_DB_PEER_INITIATED | GATT_DB_EXECUTE)

/** Characteristic Value Peer Prepare Write Req */
#define GATT_DB_CHAR_PEER_PREPARE_WRITE_REQ     (GATT_DB_PEER_INITIATED | GATT_DB_PREPARE)

/** Characteristic Client Configuration Peer Read Operation */
#define GATT_DB_CHAR_PEER_CLI_CNFG_READ_REQ     0x91U

/** Characteristic Client Configuration Peer Write Operation */
#define GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ    0x92U

/** Characteristic Server Configuration Peer Read Operation */
#define GATT_DB_CHAR_PEER_SER_CNFG_READ_REQ     0xA1U

/** Characteristic Server Configuration Peer Write Operation */
#define GATT_DB_CHAR_PEER_SER_CNFG_WRITE_REQ    0xA2U

/** Characteristic User Description Peer Read Operation */
#define GATT_DB_CHAR_PEER_USR_DESC_READ_REQ     0xB1U

/** Characteristic User Description Peer Write Operation */
#define GATT_DB_CHAR_PEER_USR_DESC_WRITE_REQ    0xB2U

/** Characteristic Higher Layer Defined Descriptor Peer Read Operation */
#define GATT_DB_CHAR_PEER_HLD_DESC_READ_REQ     0xF1U

/** Characteristic Higher Layer Defined Descriptor Peer Write Operation */
#define GATT_DB_CHAR_PEER_HLD_DESC_WRITE_REQ    0xF2U

/** \} */

/**
 *  \defgroup gatt_characteristic_properties Characteristic Properties
 *  \{
 *  Characteristic Properties - Combination of these properties (combined
 *  using bitwise OR) describe properties of the Characteristic. Also
 *  see \ref GATT_DB_ATTR_LIST.
 */

/**
 *  Characteristic support Broadcast of its value to the peer.
 *  Setting this property automatically includes Characteristic Server
 *  Configuration Descriptor to the Characteristic
 */
#define GATT_DB_CHAR_BROADCAST_PROPERTY                0x00000001U

/** Characteristic support Reading its value by peer */
#define GATT_DB_CHAR_READ_PROPERTY                     0x00000002U

/** Characteristic support Writing its value by peer Without Response  */
#define GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY        0x00000004U

/** Characteristic supports Writing its value by peer */
#define GATT_DB_CHAR_WRITE_PROPERTY                    0x00000008U

/**
 *  Characteristic supports Notifying its value to the peer.
 *  Setting this property automatically includes Characteristic Server
 *  Configuration Descriptor to the Characteristic
 */
#define GATT_DB_CHAR_NOTIFY_PROPERTY                   0x00000010U

/** Characteristic supports Indicating its value to the peer */
#define GATT_DB_CHAR_INDICATE_PROPERTY                 0x00000020U

/** Characteristic supports Signed Write on its value */
#define GATT_DB_CHAR_SIGNED_WRITE_PROPERTY             0x00000040U

/** Characteristic supports write on its User Description Descriptor */
#define GATT_DB_CHAR_WRIEABLE_AUX_PROPERTY             0x00000200U

/** No Auxillary Property */
#define GATT_DB_NO_AUXILLARY_PROPERTY                  0x00U

/** Characteristic Value is Fixed Length */
#define GATT_DB_FIXED_LENGTH_PROPERTY                  0x01U

#define GATT_DB_AUTHORIZATION_PROPERTY                 0x02U

#define GATT_DB_PEER_SPECIFIC_VAL_PROPERTY             0x40U

#define GATT_DB_CONST_ATTR_VAL_PROPERTY                0x80U

/** \} */

/**
 *  \defgroup gatt_service_properties Service Properties
 *  \{
 *  Service Properties - Combination of these properties (combined
 *  using bitwise OR) describe properties of the Service. Also
 *  see \ref GATT_DB_SERVICE_DESC.
 */

/**
 *  Security related - combination of Level, Mode and Encryption Key Size
 *  (if applicable) describes complete security needs
 */
/** Service employs Level 0 */
#define GATT_DB_SER_SECURITY_LEVEL0                    0x00000000U

/** Service employs Level 1 */
#define GATT_DB_SER_SECURITY_LEVEL1                    0x00000001U

/** Service employs Level 2 */
#define GATT_DB_SER_SECURITY_LEVEL2                    0x00000002U

/** Service employs Level 3 */
#define GATT_DB_SER_SECURITY_LEVEL3                    0x00000004U

/** Service employs Mode 1 */
#define GATT_DB_SER_SECURITY_MODE1                     0x00000010U

/** Service employs Mode 2 */
#define GATT_DB_SER_SECURITY_MODE2                     0x00000020U

/** \cond ignore_this Unused right now */
#define GATT_DB_SER_MULTIPLE_CLIENTS_SUPPORT           0x00000040U
/** \endcond */

/**
 *  Service is a Secondary Service. If this not used, Service is by default
 *  considered to be Primary
 */
#define GATT_DB_SER_SECONDARY_SERVICE_PROPERTY         0x00000080U

#if 1//def GATT_DB_SUPPORT_128_BIT_UUID

    /** Service UUID is 128-bit */
    #define GATT_DB_SER_128_BIT_UUID_FORMAT                0x00008000U

#endif /* GATT_DB_SUPPORT_128_BIT_UUID */

/** Set this property to relax all Security on the Service */
#define GATT_DB_SER_NO_SECURITY_PROPERTY               0x00000000U

/**
 *  This is employed to describe Service is not rigid on any Encryption
 *  Key Size. Any Size used by other Services or on the whole by the device
 *  will do
 */
#define GATT_DB_SER_ENC_KEY_SIZE_DONT_CARE             0x00000000U

/** Encyrption Key Size 7 Needed for the Service */
#define GATT_DB_SER_ENCRYPT_KEY_SIZE_7                 0x01000000U

/** Encyrption Key Size 8 Needed for the Service */
#define GATT_DB_SER_ENCRYPT_KEY_SIZE_8                 0x02000000U

/** Encyrption Key Size 9 Needed for the Service */
#define GATT_DB_SER_ENCRYPT_KEY_SIZE_9                 0x03000000U

/** Encyrption Key Size 10 Needed for the Service */
#define GATT_DB_SER_ENCRYPT_KEY_SIZE_10                0x04000000U

/** Encyrption Key Size 11 Needed for the Service */
#define GATT_DB_SER_ENCRYPT_KEY_SIZE_11                0x05000000U

/** Encryption Key Size 12 Needed for the Service */
#define GATT_DB_SER_ENCRYPT_KEY_SIZE_12                0x06000000U

/** Encryption Key Size 13 Needed for the Service */
#define GATT_DB_SER_ENCRYPT_KEY_SIZE_13                0x07000000U

/** Encryption Key Size 14 Needed for the Service */
#define GATT_DB_SER_ENCRYPT_KEY_SIZE_14                0x08000000U

/** Encryption Key Size 15 Needed for the Service */
#define GATT_DB_SER_ENCRYPT_KEY_SIZE_15                0x09000000U

/** Encryption Key Size 16 Needed for the Service */
#define GATT_DB_SER_ENCRYPT_KEY_SIZE_16                0x0A000000U

/** \cond ignore_this unused */
#define GATT_DB_SER_NO_ENCRYPT_PROPERTY                0x00000000U

/**
 *  Macros to describe the Transport access of the Service over BR/EDR only,
 *  LE only or Any Transport.
 */
/** Service accessible over BR/EDR Link Only */
#define GATT_DB_SER_SUPPORT_BR_LINK_TYPE               0x10000000U

/** Service accessible over LE Link Only */
#define GATT_DB_SER_SUPPORT_LE_LINK_TYPE               0x20000000U

/** Service accessible over Any Link */
#define GATT_DB_SER_SUPPORT_ANY_LINK_TYPE              0xF0000000U

/** GATT Characteristic and Descriptor permissions */
#define    GATT_DB_PERM_NONE                    0x0000U
#define    GATT_DB_PERM_READ                    0x0001U
#define    GATT_DB_PERM_READ_ENCRYPTED          0x0002U
#define    GATT_DB_PERM_READ_ENCRYPTED_MITM     0x0004U
#define    GATT_DB_PERM_WRITE                   0x0010U
#define    GATT_DB_PERM_WRITE_ENCRYPTED         0x0020U
#define    GATT_DB_PERM_WRITE_ENCRYPTED_MIMT    0x0040U
#define    GATT_DB_PERM_WRITE_SIGNED            0x0080U
#define    GATT_DB_PERM_WRITE_SIGNED_MITM       0x0100U

#define GATT_DB_ERR_ID                          0x6B00U
/* Not an error code. More of a status code */
#define GATT_DB_DELAYED_RESPONSE                 (0x0060U | GATT_DB_ERR_ID)
#define GATT_DB_DONOT_RESPOND                    (0x0061U | GATT_DB_ERR_ID)
#define GATT_DB_ALREADY_RESPONDED                (0x0062U | GATT_DB_ERR_ID)

typedef uint8_t DEVICE_HANDLE;
typedef uint8_t ATT_CON_ID;

typedef struct
{
    /** Identifies the peer accessing the database */
    DEVICE_HANDLE    device_id;

    /** Identifies the Service being Accessed */
    uint8_t            service_id;

    /** Identifies the Characteristic being Accessed */
    uint8_t            char_id;

} GATT_DB_HANDLE;

typedef uint16_t ATT_ATTR_HANDLE;
typedef uint8_t GATT_DB_OPERATION;

/* Abstracts Application Instance Identifier */
typedef uint8_t APPL_HANDLE;

typedef struct
{
    /** Abstracts Value to get/set the Attribute Value */
    ATT_VALUE            value;

    /** Abstracts the handle information */
    ATT_ATTR_HANDLE      handle;

    /** Abstracts the Access Operation */
    GATT_DB_OPERATION    db_op;

#if 1//def GATT_DB_HAVE_OFFSET_IN_PARAMS_SUPPORT
    uint16_t               offset;
#endif /* GATT_DB_HAVE_OFFSET_IN_PARAMS_SUPPORT */

} GATT_DB_PARAMS;


/**
 *  Abstracts Handle Value Pair
 *  This is used in multiple PDUs - see \ref ATT_WRITE_REQ_PARAM,
 *  \ref ATT_WRITE_CMD_PARAM etc.
 */
typedef struct
{
    /** Attribute Value  */
    ATT_VALUE          value;

    /** Attribute Handle */
    ATT_ATTR_HANDLE    handle;

} ATT_HANDLE_VALUE_PAIR;


/** ATT Handle */
typedef struct
{
    /** Identifies the peer instance */
    DEVICE_HANDLE     device_id;

    /** Identifies the ATT Instance */
    ATT_CON_ID        att_id;

} ATT_HANDLE;




/**
 * GATT DB PL extension, GATT DB Handler Callback.
 * The GATT DB PL extension calls the registered callback
 * to indicate any ongoing GATT DB Operation.
 * The Upper Layer can implement specific handling for any Characterisitc
 * or Characteritistic Descriptor depending on its requirement.
 *
 * \param [in] handle Pointer to GATT DB identifier \ref GATT_DB_HANDLE
 * \param [in] param  Pointer to GATT DB Parameters \ref GATT_DB_PARAMS
 *
 * \return API_SUCCESS if ATT/GATT response for the incoming request to be sent
 *         from the below layer
 *         GATT_DB_DELAYED_RESPONSE if ATT/GATT response will be sent at a
 *         later point by the application
 *         GATT_DB_DONOT_RESPOND if ATT/GATT response will be sent at a
 *         later point by the application
 *         GATT_DB_ALREADY_RESPONDED if ATT/GATT response is already sent
 *         the application
 *         else and Error code describing cause of failure.
 */
typedef API_RESULT (* GATT_DB_PL_EXT_HANDLER_CB)
(
    /* IN */ GATT_DB_HANDLE *handle,
    /* IN */ GATT_DB_PARAMS *param
);



#endif


#ifndef _H_BT_ATT_API_
#define _H_BT_ATT_API_

#define GATT_MAX_CHAR_DESCRIPTORS 6U //todo: remove, use drnay array

typedef struct
{
    /** Start Handle */
    ATT_ATTR_HANDLE    start_handle;

    /** End Handle */
    ATT_ATTR_HANDLE    end_handle;

}ATT_HANDLE_RANGE;

/* Service Information */
typedef struct
{
    /* Service UUID */
    ATT_UUID uuid;

#if 1//def ATT_128_BIT_UUID_FORMAT
    /* Service UUID Type */
    uint8_t uuid_type;
#endif /* ATT_128_BIT_UUID_FORMAT */

    /* Service Range */
    ATT_HANDLE_RANGE range;

} GATT_SERVICE_PARAM;

/* Characteristic Descriptor Information */
typedef struct
{
    /* Descriptor handle */
    ATT_ATTR_HANDLE handle;

    /* Descriptor UUID */
    ATT_UUID uuid;

#if 1//def ATT_128_BIT_UUID_FORMAT
    /* Descriptor UUID Type */
    uint8_t uuid_type;
#endif /* ATT_128_BIT_UUID_FORMAT */

} GATT_CHAR_DESC_PARAM;



/* Characteristic Information */
typedef struct
{
    /* Characteristic Range */
    ATT_HANDLE_RANGE range;

    /* Characteristic Property */
    uint8_t cproperty;

    /* Characteristic Value Handle */
    ATT_ATTR_HANDLE value_handle;

    /* Characteristic UUID */
    ATT_UUID uuid;

#if 1//def ATT_128_BIT_UUID_FORMAT
    /* Characteristic UUID Type */
    uint8_t uuid_type;
#endif /* ATT_128_BIT_UUID_FORMAT */

    /* Characteristic desciptor index*/
    uint8_t desc_index;

    /* Characteristics Value Offset */
    uint16_t val_offset;

    /* Characteristic Value Length */
    uint16_t val_length;

    /* Characteristic descriptor array */
    GATT_CHAR_DESC_PARAM descriptor[GATT_MAX_CHAR_DESCRIPTORS];
} GATT_CHARACTERISTIC_PARAM;


#endif



/* Characteristic Information */
typedef struct
{
    /* Characteristic Range */
    ATT_HANDLE_RANGE range;

    /* Characteristic Property */
    uint8_t cproperty;

    /* Characteristic Value Handle */
    ATT_ATTR_HANDLE value_handle;

    /* Characteristic UUID */
    ATT_UUID uuid;

#if 1//def ATT_128_BIT_UUID_FORMAT
    /* Characteristic UUID Type */
    uint8_t uuid_type;
#endif /* ATT_128_BIT_UUID_FORMAT */

    /* Characteristic desciptor index*/
    uint8_t desc_index;

    /* Characteristics Value Offset */
    uint16_t val_offset;

    /* Characteristic Value Length */
    uint16_t val_length;

    /* Characteristic descriptor array */
    GATT_CHAR_DESC_PARAM *descriptor; //GATT_MAX_CHAR_DESCRIPTORS
} ble_gatt_characteristic_param_t; //change from GATT_CHARACTERISTIC_PARAM





typedef struct
{
    API_RESULT event_result;
    //ATT_HANDLE att_handle;
    uint8_t conn_handle;
    GATT_SERVICE_PARAM *service;
    uint16_t count;
} ble_discovery_primary_service_t;


typedef struct
{
    API_RESULT event_result;
//    ATT_HANDLE att_handle;
    uint8_t conn_handle;
    //ble_gatt_characteristic_param_t *character;
    GATT_CHARACTERISTIC_PARAM *character;
    uint16_t count;
} ble_discovery_char_t;



//new api
typedef struct
{
    uint8_t    peer_address_type;
    bd_addr_t  peer_address;
    uint8_t    initiating_phys;

    uint16_t conn_interval_min;
    uint16_t conn_interval_max;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
} ble_conn_param_normal_t;

typedef struct
{
    uint8_t    initiator_filter_policy;
    uint8_t    own_address_type;
    uint8_t    peer_address_type;
    bd_addr_t  peer_address;
    uint8_t    initiating_phys;
    uint16_t *scan_interval;
    uint16_t *scan_window;
    uint16_t *conn_interval_min;
    uint16_t *conn_interval_max;
    uint16_t *conn_latency;
    uint16_t *supervision_timeout;
    uint16_t *minimum_ce_length;
    uint16_t *maximum_ce_length;
} ble_conn_param_ex_t;

typedef struct
{
    uint8_t    peer_address_type;
    bd_addr_t  peer_address;
    uint16_t conn_interval_min;
    uint16_t conn_interval_max;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
    uint16_t minimum_ce_length;
    uint16_t maximum_ce_length;
} ble_update_conn_param_t;


typedef struct
{
    API_RESULT event_result;
    //ATT_HANDLE att_handle;
    uint8_t conn_handle;
    /// Peer address type
    uint8_t peer_addr_type;
    /// Peer BT address
    uint8_t peer_addr[6];
} ble_conn_att_t;

typedef struct
{
    API_RESULT event_result;
//    ATT_HANDLE att_handle;
    uint8_t conn_handle;
    ATT_ATTR_HANDLE attr_handle;
} ble_att_tx_compl_t;

typedef struct
{
    API_RESULT event_result;
//    ATT_HANDLE att_handle;
    uint8_t conn_handle;
    ATT_ATTR_HANDLE attr_handle;

    uint8_t *data;
    uint16_t len;
} ble_att_notify_t;

typedef struct
{
    uint16_t evt_type;
    uint8_t    peer_address_type;
    bd_addr_t  peer_address;
    uint8_t rssi;
    uint8_t *data;
    uint8_t data_len;
    uint8_t *name;
} ble_adv_report_t;


typedef struct
{
    bd_addr_t peer_address;

    uint16_t conn_interval_min;
    uint16_t conn_interval_max;
    uint16_t conn_latency;
    uint16_t supervision_timeout;

    uint8_t is_agree;   ///0:is not agree,1:is agree
} ble_conn_update_param_ind_t;

typedef struct
{
    bd_addr_t peer_address;

    uint8_t status;
    uint16_t conn_interval;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
} ble_conn_update_param_compl_ind_t;

typedef struct
{
    API_RESULT event_result;
    //ATT_HANDLE att_handle;
    uint8_t conn_handle;
    ATT_ATTR_HANDLE attr_handle;

    uint8_t *data;
    uint16_t len;
}ble_att_rw_t;
