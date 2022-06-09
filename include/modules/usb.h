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
/*******************************************************************************
* Function Declarations
*******************************************************************************/
UINT32 usb_open (UINT32 op_flag);
UINT32 usb_close (void);
#if (CONFIG_SOC_BK7271) || (CONFIG_SOC_BK7256XX)
UINT32 usb_read (UINT32 pos, const void *buffer, UINT32 size);
UINT32 usb_write (UINT32 pos, const void *buffer, UINT32 size);
#else
UINT32 usb_read (char *user_buf, UINT32 count, UINT32 op_flag);
UINT32 usb_write (char *user_buf, UINT32 count, UINT32 op_flag);
#endif

UINT32 usb_ctrl(UINT32 cmd, void *param);
void usb_event_post(void);
void usb_isr(void);
void usb_check_int_handler(void);

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
 * @brief     Set required Parameters
 *
 *  This API sets resolution and frame rate:
 *   - The resolution parameters:
 *       UVC_FRAME_160_120
 *       UVC_FRAME_176_144
 *       UVC_FRAME_352_288
 *       UVC_FRAME_320_240
 *       UVC_FRAME_640_360
 *       UVC_FRAME_640_480
 *
 *   - These frame rate parameters:
 *      FPS_60
 *      FPS_30
 *      FPS_25
 *      FPS_20
 *      FPS_15
 *      FPS_10
 *      FPS_5
 *
 *  Set parameters based on camera parameters.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_set_parameter(uint32_t resolution_id, uint32_t fps);

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
 * @brief     Set camera attributes values
 *
 * This API set camera attributes values:
 *   - Two parameters attributes and values.
 *   - attributes:
 *        UVC_PU_CONTROL_UNDEFINED                                              0x00
 *        UVC_PU_BACKLIGHT_COMPENSATION_CONTROL                      0x01
 *        UVC_PU_BRIGHTNESS_CONTRO                                              0x02
 *        UVC_PU_CONTRAST_CONTROL                                               0x03
 *        UVC_PU_GAIN_CONTROL                                                       0x04
 *        UVC_PU_POWER_LINE_FREQUENCY_CONTROL                          0x05
 *        UVC_PU_HUE_CONTROL                                                         0x06
 *        UVC_PU_SATURATION_CONTROL                                             0x07
 *        UVC_PU_SHARPNESS_CONTROL                                              0x08
 *        UVC_PU_GAMMA_CONTROL                                                     0x09
 *        UVC_PU_WHITE_BALANCE_TEMPERATURE_CONTROL                  0x0a
 *        UVC_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL         0x0b
 *        UVC_PU_WHITE_BALANCE_COMPONENT_CONTROL                    0x0c
 *        UVC_PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL           0x0d
 *        UVC_PU_DIGITAL_MULTIPLIER_CONTROL                                 0x0e
 *        UVC_PU_DIGITAL_MULTIPLIER_LIMIT_CONTROL                        0x0f
 *        UVC_PU_HUE_AUTO_CONTROL                                                0x10
 *        UVC_PU_ANALOG_VIDEO_STANDARD_CONTROL                        0x11
 *        UVC_PU_ANALOG_LOCK_STATUS_CONTROL                              0x12
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
bk_err_t bk_uvc_set_cur(uint32_t attribute, uint32_t param);

/**
 * @brief     Get camera attributes values
 *
 * This API Get camera attributes values:
 *   - Two parameters attributes and values.
 *   - attributes:
 *        UVC_PU_CONTROL_UNDEFINED                                              0x00
 *        UVC_PU_BACKLIGHT_COMPENSATION_CONTROL                      0x01
 *        UVC_PU_BRIGHTNESS_CONTRO                                              0x02
 *        UVC_PU_CONTRAST_CONTROL                                               0x03
 *        UVC_PU_GAIN_CONTROL                                                       0x04
 *        UVC_PU_POWER_LINE_FREQUENCY_CONTROL                          0x05
 *        UVC_PU_HUE_CONTROL                                                         0x06
 *        UVC_PU_SATURATION_CONTROL                                             0x07
 *        UVC_PU_SHARPNESS_CONTROL                                              0x08
 *        UVC_PU_GAMMA_CONTROL                                                     0x09
 *        UVC_PU_WHITE_BALANCE_TEMPERATURE_CONTROL                  0x0a
 *        UVC_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL         0x0b
 *        UVC_PU_WHITE_BALANCE_COMPONENT_CONTROL                    0x0c
 *        UVC_PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL           0x0d
 *        UVC_PU_DIGITAL_MULTIPLIER_CONTROL                                 0x0e
 *        UVC_PU_DIGITAL_MULTIPLIER_LIMIT_CONTROL                        0x0f
 *        UVC_PU_HUE_AUTO_CONTROL                                                0x10
 *        UVC_PU_ANALOG_VIDEO_STANDARD_CONTROL                        0x11
 *        UVC_PU_ANALOG_LOCK_STATUS_CONTROL                              0x12
 *
 *   - Valid values needs to be viewed in camera.
 *
 *  Choose to call this API.
 *
 * @return
 *    - Values
 *
 */
uint32_t bk_uvc_get_cur(uint32_t attribute);

/**
 * @brief     Get camera attributes MIN values
 *
 * This API Get camera attributes MIN values:
 *   - Two parameters attributes and values.
 *   - attributes
 *
 *   - Valid values needs to be viewed in camera.
 *        
 *  Choose to call this API.
 *
 * @return
 *    - values
 *
 */
uint32_t bk_uvc_get_min(uint32_t attribute);

/**
 * @brief     Get camera attributes MAX values
 *
 * This API Get camera attributes MAX values:
 *   - Two parameters attributes and values.
 *   - attributes
 *
 *   - Valid values needs to be viewed in camera.
 *
 *  Choose to call this API.
 *
 * @return
 *    - values
 *
 */
uint32_t bk_uvc_get_max(uint32_t attribute);

/**
 * @brief     Get camera attributes RES values
 *
 * This API Get camera attributes RES values:
 *   - Two parameters attributes and values.
 *   - attributes
 *
 *   - Valid values needs to be viewed in camera.
 *
 *  Choose to call this API.
 *
 * @return
 *    - values
 *
 */
uint32_t bk_uvc_get_res(uint32_t attribute);

/**
 * @brief     Get camera attributes LEN values
 *
 * This API Get camera attributes LEN values:
 *   - Two parameters attributes and values.
 *   - attributes
 *
 *   - Valid values needs to be viewed in camera.
 *
 *  Choose to call this API.
 *
 * @return
 *    - values
 *
 */
uint32_t bk_uvc_get_len(uint32_t attribute);

/**
 * @brief     Get camera attributes INFO values
 *
 * This API Get camera attributes INFO values:
 *   - Two parameters attributes and values.
 *   - attributes
 *
 *   - Valid values needs to be viewed in camera.
 *
 *  Choose to call this API.
 *
 * @return
 *    - values
 *
 */
uint32_t bk_uvc_get_info(uint32_t attribute);

/**
 * @brief     Get camera attributes DEF values
 *
 * This API Get camera attributes DEF values:
 *   - Two parameters attributes and values.
 *   - attributes
 *
 *   - Valid values needs to be viewed in camera.
 *
 *  Choose to call this API.
 *
 * @return
 *    - values
 *
 */
uint32_t bk_uvc_get_def(uint32_t attribute);
#ifdef __cplusplus
}
#endif