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

#include <modules/wifi_types.h>

/// MAC address structure.
struct wifi_mac_addr
{
    uint8_t array[WIFI_MAC_LEN];
} __attribute__((packed));

/// Structure of a long control frame MAC header
struct wifi_mac_hdr
{
    /// Frame control
    uint16_t fctl;
    /// Duration/ID
    uint16_t durid;
    /// Address 1
    struct wifi_mac_addr addr1;
    /// Address 2
    struct wifi_mac_addr addr2;
    /// Address 3
    struct wifi_mac_addr addr3;
    /// Sequence control
    uint16_t seq;
} __attribute__((packed));

/// Structure of a Beacon or ProbeRsp frame
struct wifi_bcn_frame
{
    /// MAC Header
    struct wifi_mac_hdr h;
    /// Timestamp
    uint64_t tsf;
    /// Beacon interval
    uint16_t bcnint;
    /// Capability information
    uint16_t capa;
    /// Rest of the payload
    uint8_t variable[];
} __attribute__((packed));

/// MIC Header Length -> DA (6 bytes) + SA (6 bytes) + Priority (4 bytes)
#define MIC_HDR_LEN     (16)
/// IV Length
#define IV_LEN          (4)
/// EIV Length
#define EIV_LEN         (4)
/// MIC Length
#define MIC_LEN         (8)
/// ICV Length
#define ICV_LEN         (4)
// WPI IV length
#define WPI_IV_LEN      (18)
// WPI MIC length
#define WPI_MIC_LEN     (16)

/// Word 1 of CCMP IV
#define EIV_PRESENT     (0x2000)

/**
 * MAC HEADER LENGTH DEFINITIONS
 ****************************************************************************************
 */
/// Long MAC Header length (with QoS control field and HT control field)
#define MAC_LONG_QOS_HTC_MAC_HDR_LEN   36
/// Long MAC Header length (with QoS control field)
#define MAC_LONG_QOS_MAC_HDR_LEN       32
/// Long MAC Header length (without QoS control field)
#define MAC_LONG_MAC_HDR_LEN           30
/// Short MAC Header length (with QoS control field and HT control field)
#define MAC_SHORT_QOS_HTC_MAC_HDR_LEN  30
/// Short MAC Header length (with QoS control field)
#define MAC_SHORT_QOS_MAC_HDR_LEN      26
/// Short MAC Header length (without QoS control field)
#define MAC_SHORT_MAC_HDR_LEN          24

/// QoS Control Field Length
#define MAC_HDR_QOS_CTRL_LEN           2

/// Longest possible MAC Header
#define MAC_HDR_LEN_MAX                MAC_LONG_QOS_HTC_MAC_HDR_LEN

/// Long control frame header length
#define MAC_LONG_CTRLFRAME_LEN         16
/// Short control frame header length (ACK/CTS)
#define MAC_SHORT_CTRLFRAME_LEN        10


/**
 * MAC Header offset definitions
 */
/// 802.11 MAC header definition
#define MAC_HEAD_FCTRL_OFT              0
#define MAC_HEAD_DURATION_OFT           2
#define MAC_HEAD_DURATION_CFP      0x8000
#define MAC_HEAD_ADDR1_OFT              4
#define MAC_HEAD_ADDR2_OFT             10
#define MAC_HEAD_ADDR3_OFT             16
#define MAC_HEAD_CTRL_OFT              22
#define MAC_HEAD_ADDR4_OFT             24
#define MAC_HEAD_SHORT_QOS_OFT         24
#define MAC_HEAD_LONG_QOS_OFT          30
#define MAC_ORIGINAL_ETHTYPE_OFT       36

/**
 * Frame Control bit field definitions
 */
/* The type information in the header of a frame consists of the Type and Subtype fields
 * When using the term "frame type" in the code, we refer to the type field and not to
 * the combined type+subtype information.
 */
/// 802.11 frame control definition
#define MAC_FCTRL_LEN                   2

#define MAC_FCTRL_PROTOCOLVERSION_MASK  0x0003
#define MAC_FCTRL_TYPE_MASK             0x000C
#define MAC_FCTRL_SUBT_MASK             0x00F0
#define MAC_FCTRL_TODS                  0x0100
#define MAC_FCTRL_FROMDS                0x0200
#define MAC_FCTRL_MOREFRAG              0x0400
#define MAC_FCTRL_RETRY                 0x0800
#define MAC_FCTRL_PWRMGT                0x1000
#define MAC_FCTRL_MOREDATA              0x2000
#define MAC_FCTRL_PROTECTEDFRAME        0x4000
#define MAC_FCTRL_ORDER                 0x8000

#define MAC_FCTRL_TODS_FROMDS          (MAC_FCTRL_TODS | MAC_FCTRL_FROMDS)
/** @} */

/**
 * Frame Control Type definitions
 */
/// 802.11 type definition
#define MAC_FCTRL_MGT_T                 0x0000
#define MAC_FCTRL_CTRL_T                0x0004
#define MAC_FCTRL_DATA_T                0x0008
/** @} */

/**
 * Frame Control Subtype definitions
 */
/// 802.11 subtype definition
// Management SubType
#define MAC_FCTRL_ASSOCREQ_ST           0x0000
#define MAC_FCTRL_ASSOCRSP_ST           0x0010
#define MAC_FCTRL_REASSOCREQ_ST         0x0020
#define MAC_FCTRL_REASSOCRSP_ST         0x0030
#define MAC_FCTRL_PROBEREQ_ST           0x0040
#define MAC_FCTRL_PROBERSP_ST           0x0050
#define MAC_FCTRL_BEACON_ST             0x0080
#define MAC_FCTRL_ATIM_ST               0x0090
#define MAC_FCTRL_DISASSOC_ST           0x00A0
#define MAC_FCTRL_AUTHENT_ST            0x00B0
#define MAC_FCTRL_DEAUTHENT_ST          0x00C0
#define MAC_FCTRL_ACTION_ST             0x00D0
#define MAC_FCTRL_ACTION_NO_ACK_ST      0x00E0
// Control SubTypes
#define MAC_FCTRL_HE_TRIGGER_ST         0x0020
#define MAC_FCTRL_BFM_REPORT_POLL_ST    0x0040
#define MAC_FCTRL_VHT_NDPA_ST           0x0050
#define MAC_FCTRL_CTRL_WRAPPER_ST       0x0070
#define MAC_FCTRL_BAR_ST                0x0080
#define MAC_FCTRL_BA_ST                 0x0090
#define MAC_FCTRL_PSPOLL_ST             0x00A0
#define MAC_FCTRL_RTS_ST                0x00B0
#define MAC_FCTRL_CTS_ST                0x00C0
#define MAC_FCTRL_ACK_ST                0x00D0
#define MAC_FCTRL_CFEND_ST              0x00E0
#define MAC_FCTRL_CFEND_CFACK_ST        0x00F0

// Data SubTypes
/* Decoding the subtypes of data type frames can take advantage of the fact that
 * each subtype field bit position is used to indicate a specific modification of
 * the basic data frame (subtype 0). Frame control bit 4 is set to 1 in data
 * subtypes which include +CF-Ack, bit 5 is set to 1 in data subtypes which include
 * +CF-Poll, bit 6 is set to 1 in data subtypes that contain no Frame Body,
 * and bit 7 is set to 1 in the QoS data subtypes, which have QoS Control fields in
 * their MAC headers.
 *
 * Usage: check FrameT and FrameSubT individually. If the FrameT is MAC_FCTRL_DATA_T,
 * check the following bits of the FrameSubT
 */
#define MAC_CFACK_ST_BIT                CO_BIT(4)
#define MAC_CFPOLL_ST_BIT               CO_BIT(5)
#define MAC_NODATA_ST_BIT               CO_BIT(6)
#define MAC_QOS_ST_BIT                  CO_BIT(7)
#define MAC_FCTRL_DATACFACKPOLL_ST      (MAC_CFACK_ST_BIT | MAC_CFPOLL_ST_BIT)

/**
 * Frame Control various frame type/subtype definitions
 */
/// 802.11 type/subtype definition
#define MAC_FCTRL_TYPESUBTYPE_MASK      (MAC_FCTRL_TYPE_MASK | MAC_FCTRL_SUBT_MASK)
#define MAC_FCTRL_ASSOCREQ              (MAC_FCTRL_MGT_T     | MAC_FCTRL_ASSOCREQ_ST)
#define MAC_FCTRL_ASSOCRSP              (MAC_FCTRL_MGT_T     | MAC_FCTRL_ASSOCRSP_ST)
#define MAC_FCTRL_REASSOCREQ            (MAC_FCTRL_MGT_T     | MAC_FCTRL_REASSOCREQ_ST)
#define MAC_FCTRL_REASSOCRSP            (MAC_FCTRL_MGT_T     | MAC_FCTRL_REASSOCRSP_ST)
#define MAC_FCTRL_PROBEREQ              (MAC_FCTRL_MGT_T     | MAC_FCTRL_PROBEREQ_ST)
#define MAC_FCTRL_PROBERSP              (MAC_FCTRL_MGT_T     | MAC_FCTRL_PROBERSP_ST)
#define MAC_FCTRL_BEACON                (MAC_FCTRL_MGT_T     | MAC_FCTRL_BEACON_ST)
#define MAC_FCTRL_ATIM                  (MAC_FCTRL_MGT_T     | MAC_FCTRL_ATIM_ST)
#define MAC_FCTRL_DISASSOC              (MAC_FCTRL_MGT_T     | MAC_FCTRL_DISASSOC_ST)
#define MAC_FCTRL_AUTHENT               (MAC_FCTRL_MGT_T     | MAC_FCTRL_AUTHENT_ST)
#define MAC_FCTRL_DEAUTHENT             (MAC_FCTRL_MGT_T     | MAC_FCTRL_DEAUTHENT_ST)
#define MAC_FCTRL_ACTION                (MAC_FCTRL_MGT_T     | MAC_FCTRL_ACTION_ST)
#define MAC_FCTRL_ACTION_NO_ACK         (MAC_FCTRL_MGT_T     | MAC_FCTRL_ACTION_NO_ACK_ST)
#define MAC_FCTRL_BFM_REPORT_POLL       (MAC_FCTRL_CTRL_T    | MAC_FCTRL_BFM_REPORT_POLL_ST)
#define MAC_FCTRL_HE_TRIGGER            (MAC_FCTRL_CTRL_T    | MAC_FCTRL_HE_TRIGGER_ST)
#define MAC_FCTRL_VHT_NDPA              (MAC_FCTRL_CTRL_T    | MAC_FCTRL_VHT_NDPA_ST)
#define MAC_FCTRL_BA                    (MAC_FCTRL_CTRL_T    | MAC_FCTRL_BA_ST)
#define MAC_FCTRL_BAR                   (MAC_FCTRL_CTRL_T    | MAC_FCTRL_BAR_ST)
#define MAC_FCTRL_PSPOLL                (MAC_FCTRL_CTRL_T    | MAC_FCTRL_PSPOLL_ST)
#define MAC_FCTRL_RTS                   (MAC_FCTRL_CTRL_T    | MAC_FCTRL_RTS_ST)
#define MAC_FCTRL_CTS                   (MAC_FCTRL_CTRL_T    | MAC_FCTRL_CTS_ST)
#define MAC_FCTRL_ACK                   (MAC_FCTRL_CTRL_T    | MAC_FCTRL_ACK_ST)
#define MAC_FCTRL_CFEND                 (MAC_FCTRL_CTRL_T    | MAC_FCTRL_CFEND_ST)
#define MAC_FCTRL_CFEND_CFACK           (MAC_FCTRL_CFEND     | MAC_CFACK_ST_BIT)
#define MAC_FCTRL_DATA_CFACK            (MAC_FCTRL_DATA_T    | MAC_CFACK_ST_BIT)
#define MAC_FCTRL_DATA_CFPOLL           (MAC_FCTRL_DATA_T    | MAC_CFPOLL_ST_BIT)
#define MAC_FCTRL_DATA_CFACKPOLL        (MAC_FCTRL_DATA_T    | MAC_FCTRL_DATACFACKPOLL_ST)
#define MAC_FCTRL_NULL_FUNCTION         (MAC_FCTRL_DATA_T    | MAC_NODATA_ST_BIT)
#define MAC_FCTRL_NULL_CFACK            (MAC_FCTRL_NULL_FUNCTION  | MAC_CFACK_ST_BIT)
#define MAC_FCTRL_NULL_CFPOLL           (MAC_FCTRL_NULL_FUNCTION  | MAC_CFPOLL_ST_BIT)
#define MAC_FCTRL_NULL_CFACKPOLL        (MAC_FCTRL_NULL_FUNCTION  | MAC_FCTRL_DATACFACKPOLL_ST)
#define MAC_FCTRL_QOS_DATA              (MAC_FCTRL_DATA_T    | MAC_QOS_ST_BIT)
#define MAC_FCTRL_QOS_DATA_CFACK        (MAC_FCTRL_QOS_DATA  | MAC_CFACK_ST_BIT)
#define MAC_FCTRL_QOS_DATA_CFPOLL       (MAC_FCTRL_QOS_DATA  | MAC_CFPOLL_ST_BIT)
#define MAC_FCTRL_QOS_DATA_CFACKPOLL    (MAC_FCTRL_QOS_DATA  | MAC_FCTRL_DATACFACKPOLL_ST)
#define MAC_FCTRL_QOS_NULL              (MAC_FCTRL_QOS_DATA  | MAC_NODATA_ST_BIT)
#define MAC_FCTRL_QOS_NULL_CFACK        (MAC_FCTRL_QOS_DATA  | MAC_FCTRL_NULL_CFACK)
#define MAC_FCTRL_QOS_NULL_CFPOLL       (MAC_FCTRL_QOS_DATA  | MAC_FCTRL_NULL_CFPOLL)
#define MAC_FCTRL_QOS_NULL_CFACKPOLL    (MAC_FCTRL_QOS_DATA  | MAC_FCTRL_NULL_CFACKPOLL)

#define MAC_FCTRL_IS(fc, type) (((fc) & MAC_FCTRL_TYPESUBTYPE_MASK) == MAC_FCTRL_##type)
#define MAC_FCTRL_SUBTYPE(fc) (((fc) & MAC_FCTRL_SUBT_MASK) >> 4)

/*
 *  * Beacon Frame offset (Table 5 p46)
 *   */
#define MAC_BEACON_TIMESTAMP_OFT          MAC_SHORT_MAC_HDR_LEN   // Order 1
#define MAC_BEACON_INTERVAL_OFT          (MAC_SHORT_MAC_HDR_LEN + 8)   // Order 2
#define MAC_BEACON_CAPA_OFT              (MAC_SHORT_MAC_HDR_LEN + 10)   // Order 3
#define MAC_BEACON_VARIABLE_PART_OFT     (MAC_SHORT_MAC_HDR_LEN + 12)   // Order 4

#define MAC_ELTID_DS                      3

// DS PARAM SET
#define MAC_DS_ID_OFT                           0
#define MAC_DS_LEN_OFT                          1
#define MAC_DS_CHANNEL_OFT                      2
#define MAC_DS_PARAM_LEN                        3

uint32_t bk_wifi_find_ie(uint32_t addr, uint16_t buflen, uint8_t ie_id);
