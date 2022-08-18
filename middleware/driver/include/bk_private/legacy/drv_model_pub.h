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

#include <common/bk_typedef.h>
#include <common/sys_config.h>

#define sddev_control(handle, cmd, param)      ddev_control(handle, cmd, param)

#define DD_HANDLE_MAGIC_WORD         (0xA5A50000)
#define DD_HANDLE_MAGIC_MASK         (0xFFFF0000)
#define DD_HANDLE_ID_MASK            (0x0000FFFF)

typedef UINT32 DD_HANDLE;
typedef enum _dd_device_type_
{
    DD_DEV_TYPE_NONE,
    DD_DEV_TYPE_START = DD_HANDLE_MAGIC_WORD-1,
#if (!CONFIG_SOC_BK7231)
    DD_DEV_TYPE_TIMER,
#endif
#if ((!CONFIG_SOC_BK7231) && (CONFIG_BLE == 1))
    DD_DEV_TYPE_BLE,
#endif
#if ((!CONFIG_SOC_BK7271) && (!CONFIG_SOC_BK7256XX))
#if CONFIG_FFT
    DD_DEV_TYPE_FFT,
#endif
#endif
#if CONFIG_GENERAL_DMA
    DD_DEV_TYPE_GDMA,
#endif
    DD_DEV_TYPE_GPIO,
#if CONFIG_I2S
    DD_DEV_TYPE_I2S,
#endif
    DD_DEV_TYPE_ICU,
#if (!CONFIG_SOC_BK7231)
    DD_DEV_TYPE_IRDA,
#endif
#if CONFIG_MAC_PHY_BYPASS
    DD_DEV_TYPE_MPB,
#endif

#if 0
    DD_DEV_TYPE_PWM,
#endif

#if !CFG_CONFIG_FULLY_HOSTED
    DD_DEV_TYPE_SPI,
#endif
#if (CONFIG_SOC_BK7271)
    DD_DEV_TYPE_SPI2,
    DD_DEV_TYPE_SPI3,
#endif
#if CONFIG_QSPI
    DD_DEV_TYPE_QSPI,
#endif
    DD_DEV_TYPE_SCTRL,
    DD_DEV_TYPE_WDT,
#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236A) || (CONFIG_SOC_BK7256XX)
    DD_DEV_TYPE_CAL,
#endif
    DD_DEV_TYPE_TRNG,

    DD_DEV_TYPE_UART1,

#if CONFIG_UART2
    DD_DEV_TYPE_UART2,
#endif
#if CONFIG_UART3
    DD_DEV_TYPE_UART3,
#endif
#if CONFIG_FLASH_ORIGIN_API
    DD_DEV_TYPE_FLASH,
#endif
#if CONFIG_HSLAVE_SPI
    DD_DEV_TYPE_SPIDMA,
#endif
#if CONFIG_CAMERA
    DD_DEV_TYPE_EJPEG,
#endif
    DD_DEV_TYPE_I2C1,
    DD_DEV_TYPE_I2C2,
#if (CONFIG_SOC_BK7271)
    DD_DEV_TYPE_DSP,
    DD_DEV_TYPE_MAILBOX,
#if (CONFIG_BT)
    DD_DEV_TYPE_BT,
#endif
#endif
    DD_DEV_TYPE_AUD_DAC,
#if CONFIG_SDIO || CONFIG_SDIO_TRANS
    DD_DEV_TYPE_SDIO,
#endif
    DD_DEV_TYPE_USB,
    DD_DEV_TYPE_USB_PLUG,
#if CONFIG_SDCARD_HOST
    DD_DEV_TYPE_SDCARD,
#endif
    DD_DEV_TYPE_SARADC,
    DD_DEV_TYPE_RF,
    DD_DEV_TYPE_END
} dd_device_type;
UINT32 ddev_control(DD_HANDLE handle, UINT32 cmd, VOID *param);

#ifdef __cplusplus
}
#endif
