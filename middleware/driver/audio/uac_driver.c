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

#include <common/bk_include.h>
#include "sys_driver.h"
#include "clock_driver.h"
#include <os/os.h>
#include <os/mem.h>
#include <driver/int.h>
#include <modules/pm.h>
#include "bk_usb.h"
//#include <modules/usb.h>
//#include "BK7256_RegList.h"

extern int delay_ms(INT32 ms_count);

bk_err_t bk_aud_uac_driver_init(void)
{
#if CONFIG_USB_UAC
	uint8_t wait_count = 0;
	bk_usb_open(0);
	usb_device_set_using_status(1, USB_UAC_MIC_DEVICE);
	usb_device_set_using_status(1, USB_UAC_SPEAKER_DEVICE);
	while(MGC_DevGetConnectStatus() != 1) {
		if(wait_count > 100) {
			os_printf("wait usb connect timeout 1s!\r\n");
			return BK_FAIL;
			break;
		}
		rtos_delay_milliseconds(10);
		wait_count++;
	}
#endif

	return BK_OK;
}

bk_err_t bk_aud_uac_driver_deinit(void)
{
#if CONFIG_USB_UAC
	usb_device_set_using_status(0, USB_UAC_MIC_DEVICE);
	usb_device_set_using_status(0, USB_UAC_SPEAKER_DEVICE);
	return bk_usb_close();
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_start_mic(void)
{
#if CONFIG_USB_UAC
	return bk_uac_start_mic();
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_stop_mic(void)
{
#if CONFIG_USB_UAC
	return bk_uac_stop_mic();
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_start_spk(void)
{
#if CONFIG_USB_UAC
	return bk_uac_start_speaker();
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_stop_spk(void)
{
#if CONFIG_USB_UAC
	return bk_uac_stop_speaker();
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_register_mic_callback(void *cb)
{
#if CONFIG_USB_UAC
	return bk_uac_register_micrxed_packet_callback(cb);
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_unregister_mic_callback(void)
{
#if CONFIG_USB_UAC
	return bk_uac_unregister_micrxed_packet_callback();
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_register_spk_buff_ptr(void *buff, uint32_t size)
{
#if CONFIG_USB_UAC
	return bk_uac_register_tx_speakerstream_buffptr(buff, size);
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_register_spk_callback(void *cb)
{
#if CONFIG_USB_UAC
	return bk_uac_register_speakerstream_txed_callback(cb);
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_unregister_spk_callback(void)
{
#if CONFIG_USB_UAC
	return bk_uac_unregister_speakerstream_txed_callback();
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_set_spk_samp_rate(uint32_t value)
{
#if CONFIG_USB_UAC
	return bk_usb_uac_set_hz(USB_UAC_SPEAKER_DEVICE, value);
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_set_mic_samp_rate(uint32_t value)
{
#if CONFIG_USB_UAC
	return bk_usb_uac_set_hz(USB_UAC_MIC_DEVICE, value);
#else
	return BK_OK;
#endif
}

