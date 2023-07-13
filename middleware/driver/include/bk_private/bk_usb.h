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

#include <common/bk_include.h>
#include <modules/usb_types.h>

#define UVC_DEMO_SUPPORT102
#define USB_FAILURE                (1)
#define USB_SUCCESS                (0)

/*******************************************************************************
* Function Declarations
*******************************************************************************/
extern void usb_init(void);
extern void usb_exit(void);
extern uint32_t MUSB_HfiRead( uint32_t first_block, uint32_t block_num, uint8_t
                              *dest);
extern uint32_t MUSB_HfiWrite( uint32_t first_block, uint32_t block_num, uint8_t
                               *dest);
extern void MGC_RegisterCBTransferComplete(FUNCPTR func);
extern uint32_t MGC_DevGetConnectStatus(void);

extern bk_err_t bk_usb_init(void);
extern bk_err_t bk_usb_deinit(void);
extern bk_err_t bk_usb_open (uint32_t usb_mode);
extern bk_err_t bk_usb_close (void);
extern bk_err_t bk_usb_control_transfer_init(void);
extern bk_err_t bk_usb_control_transfer_deinit(void);
extern bk_err_t bk_usb_control_transfer(s_usb_device_request *pSetup, s_usb_transfer_buffer_info *buffer_info);
extern void usb_device_set_using_status(bool use_or_no, E_USB_DEVICE_T dev);
extern bk_err_t bk_usb_check_device_supported (E_USB_DEVICE_T usb_dev);
extern bk_err_t bk_uvc_get_stream_transfer_mode(uint8_t *transfermode);
extern bk_err_t bk_uvc_start(void);
extern bk_err_t bk_uvc_stop(void);
extern bk_err_t bk_uvc_enable_H264();
extern bk_err_t bk_uvc_enable_H265();
extern bk_err_t bk_uvc_enable_mjpeg();
extern bk_err_t bk_uvc_register_rx_vstream_buffptr(void *param);
extern bk_err_t bk_uvc_register_rx_vstream_bufflen(uint32_t param);
extern bk_err_t bk_uvc_receive_video_stream();
extern bk_err_t bk_uvc_register_disconnect_callback(void *param);
extern bk_err_t bk_uvc_register_config_callback(void *param);
extern bk_err_t bk_uvc_register_VSrxed_callback(void *param);
extern bk_err_t bk_uvc_register_VSrxed_packet_callback(void *param);
extern bk_err_t bk_uvc_unregister_VSrxed_packet_callback();
extern bk_err_t bk_uvc_register_link(uint32_t param);
extern bk_err_t bk_usb_uvc_check_support_attribute(E_UVC_ATTRIBUTE_T attribute);
extern bk_err_t bk_usb_uvc_attribute_op(E_USB_ATTRIBUTE_OP ops, E_UVC_ATTRIBUTE_T attribute, uint32_t *param);
extern void bk_uvc_get_resolution_number(uint16_t *count);
extern void bk_uvc_get_resolution_framerate(void *param, uint16_t count);
extern void bk_uvc_set_resolution_framerate(void *param);

#ifdef CONFIG_USB_UAC
extern bk_err_t bk_usb_uac_register_disconnect_callback(void *param);
extern bk_err_t bk_usb_uac_register_connect_callback(void *param);
extern bk_err_t bk_usb_uac_set_hz(E_USB_DEVICE_T dev, uint32 hz);
extern bk_err_t bk_usb_uac_attribute_op(E_USB_ATTRIBUTE_OP ops, E_UAC_ATTRIBUTE_T attribute, uint32_t *param);
extern bk_err_t bk_usb_uac_check_support_attribute(E_UAC_ATTRIBUTE_T attribute);
extern bk_err_t bk_usb_uac_get_format_descriptor(E_USB_DEVICE_T dev,
											s_audio_as_general_descriptor *interfacedesc,
											s_audio_format_type_descriptor *formatdesc);
#if CONFIG_USB_UAC_MIC
extern bk_err_t bk_uac_start_mic(void);
extern bk_err_t bk_uac_stop_mic(void);
extern bk_err_t bk_uac_register_micrxed_packet_callback(void *param);
extern bk_err_t bk_uac_unregister_micrxed_packet_callback(void);
#endif
#if CONFIG_USB_UAC_SPEAKER
extern bk_err_t bk_uac_start_speaker(void);
extern bk_err_t bk_uac_stop_speaker(void);
extern bk_err_t bk_uac_register_speakerstream_txed_callback(void *param);
extern bk_err_t bk_uac_unregister_speakerstream_txed_callback(void);
extern bk_err_t bk_uac_register_tx_speakerstream_buffptr(void *buffer_ptr, uint32_t buffer_len);
#endif
#endif

#if (CONFIG_SOC_BK7271) || (CONFIG_SOC_BK7256XX || CONFIG_SOC_BK7236)
UINT32 usb_read (UINT32 pos, const void *buffer, UINT32 size);
UINT32 usb_write (UINT32 pos, const void *buffer, UINT32 size);
#else
UINT32 usb_read (char *user_buf, UINT32 count, UINT32 op_flag);
UINT32 usb_write (char *user_buf, UINT32 count, UINT32 op_flag);
#endif

#if (CONFIG_SOC_BK7251) || (CONFIG_SOC_BK7271)
#define USB_PLUG_FAILURE                (1)
#define USB_PLUG_SUCCESS                (0)

#define USB_PLUG_DEV_NAME               "usb_plug"
#include "bk_gpio.h"

void usb_plug_inout_init(void);
void usb_plug_inout_exit(void);
#endif // (CONFIG_SOC_BK7251)

#ifdef __cplusplus
}
#endif
