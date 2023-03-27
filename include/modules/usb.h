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

#ifdef __cplusplus
extern "C" {
#endif

#include "usb_types.h"

/*******************************************************************************
* Function Declarations
*******************************************************************************/

/**
 * @brief     open the USB
 *
 * This API configure the resoure common to USB:
 *   - Select the identity as host or slave
 *  parameter: usb_mode
 *            USB_HOST_MODE   = 0
 *            USB_DEVICE_MODE = 1
 *
 *   - Configure USB common clock
 *   - Configure USB common registers
 *
 * This API should be called before any other USB APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_open (uint32_t usb_mode);

/**
 * @brief     close the USB
 *
 * This API release USB resources:
 *   - Restore register
 *   - Turn off the power and clock
 *
 * This API should be called after any other USB APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_close (void);

/**
 * @brief     check whether the USB device is supported
 *
 * This API detect whether the device is supported:
 *   - Select the identity as E_USB_DEVICE_T
 *  parameter: usb_dev
 *           USB_UVC_DEVICE = 0,
 *           USB_UAC_MIC_DEVICE = 1,
 *           USB_UAC_SPEAKER_DEVICE = 2,
 *
 * This API should be called after any other USB APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_check_device_supported (E_USB_DEVICE_T usb_dev);

/**
 * @brief     Start UVC stream transmission
 *
 * USB video camera data transfer starts:
 *   - configure the required configuration parameters
 *   - Send video stream data transfer instructions
 *
 * This API means that the data starts to transfer and needs to
 * transfer and needs to be copied.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_start(void);

/**
 * @brief     Stop UVC stream transmission
 *
 * USB video camera data transfer stops:
 *   - Send the video stream data stop transmission command
 *
 *
 * There will be no more data flow after execution.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_stop(void);

/**
 * @brief     Configure the address of the data buffer
 *
 * This API configure the resoure common to UVC:
 *   - The buffer adderss is the address of the temporary data copy.
 *   - Can be a global variable address or a storage address.
 *
 *
 * A buffer should be larger than a packet fo data.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_register_rx_vstream_buffptr(void *param);

/**
 * @brief     Configure the Length of the data buffer
 *
 * This API configure the resoure common to UVC:
 *   - Work with bk_uvc_register_rx_vstream_buffptr().
 *   - The buffer size is calculated to see if it matches.
 *
 *
 * A buffer should be larger than a packet fo data.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_register_rx_vstream_bufflen(uint32_t param);

/**
 * @brief     start accepting data
 *
 * This API copies data received by USB to buffer:
 *   - This function is executed after bk_uvc_start().
 *   - The buffer size is calculated to see if it matches.
 *
 *
 * This API takes the same parameters as the bk_uvc_register_rx_vstream_buffptr()
 * and bk_uvc_register_rx_vstream_bufflen().
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_receive_video_stream();

/**
 * @brief     Configure the callback function registration function registration
 *
 * This API configure the callback:
 *   - parameter is the registered callback function.
 *   - The callback function needs to be defined.
 *
 *
 *  Notifies a device connection that a callback function needs to be configured.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_register_config_callback(void *param);

/**
 * @brief     UVC disconnect the callback function registration function registration
 *
 * This API disconnect the callback:
 *   - parameter is the registered callback function.
 *   - The callback function needs to be defined.
 *
 *
 *  Notify the registered callback function when the device is disconnected.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_register_disconnect_callback(void *param);

/**
 * @brief     A transaction transfer completes the callback function registration
 *
 * This API configure the callback:
 *   - parameter is the registered callback function.
 *   - The callback function needs to be defined.
 *
 *
 *  The callback function must be registered.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 */
bk_err_t bk_uvc_register_VSrxed_callback(void *param);

/**
 * @brief     A packet of data transfer completes the callback function registration
 *
 * This API configure the callback:
 *   - parameter is the registered callback function.
 *   - The callback function needs to be defined.
 *
 *
 *  The callback function must be registered.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 */
bk_err_t bk_uvc_register_VSrxed_packet_callback(void *param);

/**
 * @brief     Select the data transmission mode
 *
 * This API can select the data transfer mode:
 *   - Parameters can be passed directly to parameters.
 *     1 :udp send packet;
 *     0 :tcp send packet;
 *
 *  Choose to call this API.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 */
bk_err_t bk_uvc_register_link(uint32_t param);

/**
 * @brief     Check whether the attribute configuration is supported
 *
 * After connecting the UVC device, determine whether the attribute operation
 * is supported by calling this API:
 *   - The Parameters has enumeration variable A planning, please fill in the
 *     attribute parameter correctly.
 *   - attribute:
 *         E_UVC_ATTRIBUTE_T;
 *
 *  Choose to call this API.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 */
bk_err_t bk_usb_uvc_check_support_attribute(E_UVC_ATTRIBUTE_T attribute);

/**
 * @brief     Set camera attributes values
 *
 * This API set/get camera attributes values:
 *   - Three parameters ops/attributes/values.
 *   - ops:
 *        E_USB_ATTRIBUTE_OP
 *   - attributes:
 *        E_UVC_ATTRIBUTE_T
 *   - param:
 *        Incoming parameter pointer
 *
 *    example: Reference cli_usb.c
 *
 *   - Valid values needs to be viewed in camera.
 *
 *  Choose to call this API.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 */
bk_err_t bk_usb_uvc_attribute_op(E_USB_ATTRIBUTE_OP ops, E_UVC_ATTRIBUTE_T attribute, uint32_t *param);

/**
 * @brief     Select H264 video format for data transmission
 *
 * Judge whether the device supports H264 video format according to the return value.
 *
 *  Choose to call this API.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 */
bk_err_t bk_uvc_enable_H264();

/**
 * @brief     Select H265 video format for data transmission
 *
 * Judge whether the device supports H265 video format according to the return value.
 *
 *  Choose to call this API.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 */
bk_err_t bk_uvc_enable_H265();

/**
 * @brief     Select MJPEG video format for data transmission
 *
 * Judge whether the device supports MJPEG video format according to the return value.
 *
 *  Choose to call this API.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 */
bk_err_t bk_uvc_enable_mjpeg();

/**
 * @brief     Set camera attributes values
 *
 * This API get camera resolution and framerate:
 *   - Two parameters param/count.
 *   - param:
 *        UVC_ResolutionFramerate
 *   - count:
 *        Get some camera parameters. If you want to get all, fill in 0xFFFF for the count.
 *
 *    example: Reference UVC demo
 *
 *   - Valid values needs to be viewed in camera.
 *
 *  Choose to call this API.
 *
 */
void bk_uvc_get_resolution_framerate(void *param, uint16_t count);

/**
 * @brief     Set camera attributes values
 *
 * This API set camera resolution and framerate:
 *   - One parameters param.
 *   - param:
 *        structure UVC_ResolutionFramerate
 *
 *    example: Reference UVC demo
 *
 *   - Valid values needs to be viewed in camera.
 *
 *  Choose to call this API.
 *
 */
void bk_uvc_set_resolution_framerate(void *param);

/**
 * @brief     UAC disconnect the callback function registration function registration
 *
 * This API disconnect the callback:
 *   - parameter is the registered callback function.
 *   - The callback function needs to be defined.
 *
 *
 *  Notify the registered callback function when the device is disconnected.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_uac_register_disconnect_callback(void *param);

/**
 * @brief     UAC connect the callback function registration function registration
 *
 * This API disconnect the callback:
 *   - parameter is the registered callback function.
 *   - The callback function needs to be defined.
 *
 *
 *  Notify the registered callback function when the device is connected.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_uac_register_connect_callback(void *param);

/**
 * @brief     Configure the sampling rate of MIC and SPEAKER in UAC
 *
 * This API set sampling rate of mic/speaker:
 *   - Two parameters dev/hz.
 *   - dev:
 *        E_USB_DEVICE_T
 *   - hz:
 *        The filled sampling rate device does not support returning error values.
 *
 *    example: Reference UAC demo
 *
 *  Choose to call this API.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_uac_set_hz(E_USB_DEVICE_T dev, uint32 hz);

/**
 * @brief     Obtain the attribute descriptor supported by the UAC device
 *
 * This API get UAC device descriptor:
 *   - Three parameters dev/interfacedesc/formatdesc.
 *   - dev:
 *        enumeration E_USB_DEVICE_T
 *   - interfacedesc:
 *        structure s_audio_as_general_descriptor.
 *   - formatdesc:
 *        structure s_audio_format_type_descriptor.
 *
 *    example: Reference UAC demo
 *
 *  Choose to call this API.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_uac_get_format_descriptor(E_USB_DEVICE_T dev,
                                                        s_audio_as_general_descriptor *interfacedesc,
                                                        s_audio_format_type_descriptor *formatdesc);

/**
 * @brief     Set camera attributes values
 *
 * This API set/get camera attributes values:
 *   - Three parameters ops/attributes/values.
 *   - ops:
 *        E_USB_ATTRIBUTE_OP
 *   - attributes:
 *        E_UAC_ATTRIBUTE_T
 *   - param:
 *        Incoming parameter pointer
 *
 *    example: Reference cli_usb.c
 *
 *   - Valid values needs to be viewed in camera.
 *
 *  Choose to call this API.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 */
bk_err_t bk_usb_uac_attribute_op(E_USB_ATTRIBUTE_OP ops, E_UAC_ATTRIBUTE_T attribute, uint32_t *param);

/**
 * @brief     Check whether the attribute configuration is supported
 *
 * After connecting the UAC device, determine whether the attribute operation
 * is supported by calling this API:
 *   - The Parameters has enumeration variable A planning, please fill in the
 *     attribute parameter correctly.
 *   - attribute:
 *         E_UAC_ATTRIBUTE_T;
 *
 *  Choose to call this API.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 */
bk_err_t bk_usb_uac_check_support_attribute(E_UAC_ATTRIBUTE_T attribute);

/**
 * @brief     Start UAC mic stream transmission
 *
 * USB audio data transfer starts:
 *   - configure the required configuration parameters
 *   - Send audio stream data transfer instructions
 *
 * This API means that the data starts to transfer and needs to
 * transfer and needs to be copied.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uac_start_mic(void);

/**
 * @brief     Stop UAC mic stream transmission
 *
 * USB audio data transfer stops:
 *   - Send the video stream data stop transmission command
 *
 *
 * There will be no more data flow after execution.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uac_stop_mic(void);

/**
 * @brief     A packet of data transfer completes the callback function registration
 *
 * This API configure the callback:
 *   - parameter is the registered callback function.
 *   - The callback function needs to be defined.
 *
 *
 *  The callback function must be registered.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 */
bk_err_t bk_uac_register_micrxed_packet_callback(void *param);

/**
 * @brief     A packet of data transfer completes the callback function unregistration
 *
 *  The UAC Mic application must be logged off when it is not in use.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 */
bk_err_t bk_uac_unregister_micrxed_packet_callback(void);

/**
 * @brief     Start UAC speaker stream transmission
 *
 * USB audio data transfer starts:
 *   - configure the required configuration parameters
 *   - Send audio stream data transfer instructions
 *
 * This API means that the data starts to transfer and needs to
 * transfer and needs to be copied.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uac_start_speaker(void);

/**
 * @brief     Stop UAC speaker stream transmission
 *
 * USB audio data transfer stops:
 *   - Send the video stream data stop transmission command
 *
 *
 * There will be no more data flow after execution.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uac_stop_speaker(void);

/**
 * @brief     A packet of data transfer completes the callback function registration
 *
 * This API configure the callback:
 *   - parameter is the registered callback function.
 *   - The callback function needs to be defined.
 *
 *
 *  The callback function must be registered.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 */
bk_err_t bk_uac_register_speakerstream_txed_callback(void *param);

/**
 * @brief     A packet of data transfer completes the callback function unregistration
 *
 *  The UAC Speaker application must be logged off when it is not in use.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 */
bk_err_t bk_uac_unregister_speakerstream_txed_callback(void);

/**
 * @brief     Register the pointer space of transmitted data and the length of transmitted data
 *
 * This API Updata the data in the buffer after each transmission:
 *   - Two parameters buffer_ptr/buffer_len.
 *   - buffer_ptr:
 *        Pointer address
 *   - buffer_len:
 *        Length of transmitted data.
 *
 *    example: Reference UAC demo
 *
 *  This API is required to use UAC Speaker.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uac_register_tx_speakerstream_buffptr(void *buffer_ptr, uint32_t buffer_len);

/**
 * @brief     Initializes the control transfer request resource
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
*/
bk_err_t bk_usb_control_transfer_init(void);

/**
 * @brief     Unmount control transfer to release resource
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
*/
bk_err_t bk_usb_control_transfer_deinit(void);

/**
 * @brief     Control command and data interface fucntion to be passed
 *
 * This API is used to send commands and transfer data:
 *   - Two parameters pSetup/buffer_info.
 *   - s_usb_device_request:
 *        The command format is specified by the USB protocol
 *   - s_usb_transfer_buffer_info:
 *        Buffer informataion for sending and receiving data.
 *
 *    example: Reference UVC_OTA demo
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_control_transfer(s_usb_device_request *pSetup, s_usb_transfer_buffer_info *buffer_info);
#ifdef __cplusplus
}
#endif