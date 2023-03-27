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

#include <common/bk_include.h>
#include <common/bk_err.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BK_ERR_USB_NOT_OPEN                   (BK_ERR_USB_BASE - 1) /**< USB dose not turn on clock and power, etc */
#define BK_ERR_USB_NOT_CLOSE                  (BK_ERR_USB_BASE - 2) /**< USB dose not turn off clock and power, etc */
#define BK_ERR_USB_NOT_CONNECT                (BK_ERR_USB_BASE - 3) /**< USB device not connected */
#define BK_ERR_USB_OPERATION_NULL_POINTER     (BK_ERR_USB_BASE - 4) /**< USB Operated with null pointer */

#define BK_ERR_USB_UVC_NOT_START              (BK_ERR_USB_BASE - 5) /**< UVC Transfer not started */
#define BK_ERR_USB_UVC_NOT_STOP               (BK_ERR_USB_BASE - 6) /**< UVC Transfer not stopped */
#define BK_ERR_USB_UVC_NOT_PAUSE              (BK_ERR_USB_BASE - 7) /**< UVC Transfer not paused */
#define BK_ERR_USB_UVC_GET_PARAMETER_FAILED   (BK_ERR_USB_BASE - 8) /**< UVC The command to obtain parameters did not respond */
#define BK_ERR_USB_UVC_SET_PARAMETER_FAILED   (BK_ERR_USB_BASE - 9) /**< UVC Configuration parameter command did not respond */
#define BK_ERR_USB_UVC_SET_INTERFACE_FAILED   (BK_ERR_USB_BASE - 10) /**< UVC The set port command did not respond */
#define BK_ERR_USB_UVC_NOSUPPORT_ATTRIBUTE    (BK_ERR_USB_BASE - 12) /**< UVC The attribute configuration is not supported */

#define BK_ERR_USB_UAC_NOT_START              (BK_ERR_USB_BASE - 13) /**< UAC Transfer not started */
#define BK_ERR_USB_UAC_NOT_STOP               (BK_ERR_USB_BASE - 14) /**< UAC Transfer not stopped */
#define BK_ERR_USB_UAC_NOT_PAUSE              (BK_ERR_USB_BASE - 15) /**< UAC Transfer not paused */
#define BK_ERR_USB_UAC_GET_PARAMETER_FAILED   (BK_ERR_USB_BASE - 16) /**< UAC The command to obtain parameters did not respond */
#define BK_ERR_USB_UAC_SET_PARAMETER_FAILED   (BK_ERR_USB_BASE - 17) /**< UAC Configuration parameter command did not respond */
#define BK_ERR_USB_UAC_SET_INTERFACE_FAILED   (BK_ERR_USB_BASE - 18) /**< UAC The set port command did not respond */
#define BK_ERR_USB_UAC_NOSUPPORT_ATTRIBUTE    (BK_ERR_USB_BASE - 19) /**< UAC The attribute configuration is not supported */

/**
 * @brief USB interrupt service routine
 */

#define USB_DEV_NAME                "usb"

typedef void (*USB_FPTR)(void *, void *);

#define USB_CMD_MAGIC              (0xe550000)
typedef enum
{
    UCMD_RESET = USB_CMD_MAGIC + 1,

    UCMD_MSC_REGISTER_FIDDLE_CB,
    UCMD_UVC_REGISTER_CONFIG_NOTIFY_CB,
    UCMD_UVC_REGISTER_RX_VSTREAM_CB,
    UCMD_UVC_REGISTER_RX_VSTREAM_BUF_PTR,
    UCMD_UVC_REGISTER_RX_VSTREAM_BUF_LEN,

    UCMD_UVC_SET_PARAM,
    UCMD_UVC_START_STREAM,
    UCMD_UVC_STOP_STREAM,
    UCMD_UVC_GET_CONNECT_STATUS,
    UCMD_UVC_RECEIVE_VSTREAM,
    UCMD_UVC_ENABLE_MJPEG,
    UCMD_UVC_ENABLE_H264,
    UCMD_USB_CONNECTED_REGISTER_CB,
    UCMD_UVC_REGISTER_LINK,

    UCMD_UVC_REGISTER_RX_PACKET_CB,
    UCMD_UVC_GET_CUR,
    UCMD_UVC_GET_MIN,
    UCMD_UVC_GET_MAX,
    UCMD_UVC_GET_RES,
    UCMD_UVC_GET_LEN,
    UCMD_UVC_GET_INFO,
    UCMD_UVC_GET_DEF,
    UCMD_UVC_SET_CUR,
} E_UVC_CMD_T;

/*UCMD_UVC_SET_PARAM*/
#define UVC_MUX_PARAM(resolution_id, fps)           (fps + (resolution_id << 16))
#define UVC_DEMUX_FPS(param)                         (param & 0xffff)
#define UVC_DEMUX_ID(param)                         ((param >> 16) & 0xffff)

typedef enum
{
    USB_HOST_MODE   = 0,
    USB_DEVICE_MODE = 1
} USB_MODE;

typedef enum
{
    FPS_60 = 60,
    FPS_30 = 30,
    FPS_25 = 25,
    FPS_20 = 20,
    FPS_15 = 15,
    FPS_10 = 10,
    FPS_5  = 5,
} E_FRAME_RATE_ID;

/* Processing Unit Control Selectors */
typedef enum
{
    UVC_ATTRIBUTE_CONTROL_UNDEFINED = 0x00,
    UVC_ATTRIBUTE_BACKLIGHT_COMPENSATION = 0x01,
    UVC_ATTRIBUTE_BRIGHTNESS,
    UVC_ATTRIBUTE_CONTRAST,
    UVC_ATTRIBUTE_GAIN,
    UVC_ATTRIBUTE_POWER_LINE_FREQUENCY,
    UVC_ATTRIBUTE_HUE,
    UVC_ATTRIBUTE_SATURATION,
    UVC_ATTRIBUTE_SHARPNESS,
    UVC_ATTRIBUTE_GAMMA,
    UVC_ATTRIBUTE_WHITE_BALANCE_TEMPERATURE,
    UVC_ATTRIBUTE_WHITE_BALANCE_TEMPERATURE_AUTO,
    UVC_ATTRIBUTE_WHITE_BALANCE_COMPONENT,
    UVC_ATTRIBUTE_WHITE_BALANCE_COMPONENT_AUTO,
    UVC_ATTRIBUTE_DIGITAL_MULTIPLIER,
    UVC_ATTRIBUTE_DIGITAL_MULTIPLIER_LIMIT,
    UVC_ATTRIBUTE_HUE_AUTO,
    UVC_ATTRIBUTE_ANALOG_VIDEO_STANDARD,
    UVC_ATTRIBUTE_ANALOG_LOCK_STATUS,
    UVC_ATTRIBUTE_NUM,
} E_UVC_ATTRIBUTE_T;

/* Video Class-Specific Request Codes */
typedef enum
{
    USB_ATTRIBUTE_RC_UNDEFINED = 0,
    USB_ATTRIBUTE_SET_CUR = 1,
    USB_ATTRIBUTE_GET_CUR = 2,
    USB_ATTRIBUTE_GET_MIN,
    USB_ATTRIBUTE_GET_MAX,
    USB_ATTRIBUTE_GET_RES,
    USB_ATTRIBUTE_GET_LEN,
    USB_ATTRIBUTE_GET_INFO,
    USB_ATTRIBUTE_GET_DEF,
} E_USB_ATTRIBUTE_OP;

typedef enum
{
    UAC_ATTRIBUTE_MUTE = 0x1,
    UAC_ATTRIBUTE_VOLUME = 0x2,
    UAC_ATTRIBUTE_BASS = 0x3,
    UAC_ATTRIBUTE_MID = 0x4,
    UAC_ATTRIBUTE_TREBLE = 0x5,
    UAC_ATTRIBUTE_GRAPHIC_EQUALIZER = 0x6,
    UAC_ATTRIBUTE_AUTOMATIC_GAIN = 0x7,
    UAC_ATTRIBUTE_DEALY = 0x8,
    UAC_ATTRIBUTE_BASS_BOOST = 0x9,
    UAC_ATTRIBUTE_LOUDNESS = 0xa,
    UAC_ATTRIBUTE_NUM,
} E_UAC_ATTRIBUTE_T;

typedef enum
{
    USB_UVC_DEVICE = 0,
    USB_UAC_MIC_DEVICE = 1,
    USB_UAC_SPEAKER_DEVICE = 2,
    USB_MSD_DEVICE = 3,
    USB_OTA_DEVICE = 4,
} E_USB_DEVICE_T;

/*
* Finish DRC interrupt processing
*/
typedef enum
{
    BSR_NONE_EVENT = 0,
    BSR_ERROR_EVENT,
    BSR_CONNECT_EVENT,
    BSR_CONNECTED_EVENT,

    BSR_DISCONNECT_EVENT,
    BSR_READ_OK_EVENT,
    BSR_WRITE_OK_EVENT
} E_MSD_STATUS_EVENT_T;

/*
 * The value is defined in field wWidth and wHeight in 'Video Streaming MJPEG
Frame Type Descriptor'
 */
typedef struct
{
    uint16_t  fps;
    uint16_t  width;
    uint16_t  height;
} UVC_ResolutionFramerate;

typedef struct {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bTerminalLink;
    uint8_t bDelay;
    uint16_t wFormatTag;
} s_audio_as_general_descriptor;

typedef struct {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bFormatType;
    uint8_t bNrChannels;
    uint8_t bSubframeSize;
    uint8_t bBitResolution;
    uint8_t bSamFreqType;
    uint8_t tSamFreq[3];
} s_audio_format_type_descriptor;


typedef struct
{
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
} s_usb_device_request;

typedef void (*USBControlIrpComplete)(void *, void *);

typedef struct
{
    const uint8_t *pOutBuffer;
    uint32_t dwOutLength;
    uint8_t *pInBuffer;
    uint32_t dwInLength;

    USBControlIrpComplete pfIrpComplete;
    void *pCompleteParam;
} s_usb_transfer_buffer_info;




#ifdef __cplusplus
}
#endif


