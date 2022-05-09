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

#include <common/sys_config.h>

#ifdef __cplusplus
extern "C" {
#endif

#if (CONFIG_WIFI_ENABLE)
#define CLI_CFG_WIFI        1
#else
#define CLI_CFG_WIFI        0
#endif //#if (CONFIG_WIFI_ENABLE)
#define CLI_CFG_BLE         1

#if (CONFIG_LWIP)
#define CLI_CFG_NETIF       1
#else
#define CLI_CFG_NETIF       0
#endif //#if (CONFIG_LWIP)

#define CLI_CFG_MISC        1
#define CLI_CFG_MEM         1

#if (CONFIG_WIFI_ENABLE)
#define CLI_CFG_PHY         1
#else
#define CLI_CFG_PHY         0
#endif //#if (CONFIG_WIFI_ENABLE)

#define CLI_CFG_PWR         1
#define CLI_CFG_TIMER       1
#define CLI_CFG_WDT         1
#if CONFIG_TRNG_SUPPORT
#define CLI_CFG_TRNG        1
#else
#define CLI_CFG_TRNG        0
#endif
#define CLI_CFG_GPIO        1
#define CLI_CFG_OS          1
#define CLI_CFG_OTA         1
#if(CONFIG_KEYVALUE)
#define CLI_CFG_KEYVALUE    1
#else
#define CLI_CFG_KEYVALUE    0
#endif
#if(CONFIG_MATTER_NAMEKEY)
#define CLI_CFG_NAMEKEY     1
#else
#define CLI_CFG_NAMEKEY     0
#endif
#define CLI_CFG_UART        1
#define CLI_CFG_ADC         1
#define CLI_CFG_SPI         1
#define CLI_CFG_MICO        1
#define CLI_CFG_REG         1
#define CLI_CFG_DMA         1
#define CLI_CFG_EXCEPTION   1

#if(CONFIG_PWM)
#define CLI_CFG_PWM         1
#else
#define CLI_CFG_PWM         0
#endif

#if(CONFIG_FLASH)
#define CLI_CFG_FLASH       1
#else
#define CLI_CFG_FLASH       0
#endif

#if(CONFIG_ICU)
#define CLI_CFG_ICU         1
#else
#define CLI_CFG_ICU         0
#endif

#define CLI_CFG_I2C         1

#if CONFIG_QSPI
#define CLI_CFG_QSPI        1
#else
#define CLI_CFG_QSPI        0
#endif

#if CONFIG_AON_RTC
#define CLI_CFG_AON_RTC     1
#else
#define CLI_CFG_AON_RTC     0
#endif

#if CONFIG_JPEG_ENCODE
#define CLI_CFG_JPEGENC        1
#else
#define CLI_CFG_JPEGENC        0
#endif

#if CONFIG_JPEG_DECODE
#define CLI_CFG_JPEGDEC     1
#else
#define CLI_CFG_JPEGDEC     0
#endif

#if CONFIG_CALENDAR
#define CLI_CFG_CALENDAR    1
#else
#define CLI_CFG_CALENDAR    0
#endif

//TODO default to 0
#define CLI_CFG_EVENT       1

#if (CONFIG_SOC_BK7251)
#define CLI_CFG_SECURITY    1
#else
#define CLI_CFG_SECURITY    1
#endif

#if CONFIG_TEMP_DETECT
#define CLI_CFG_TEMP_DETECT 1
#else
#define CLI_CFG_TEMP_DETECT 0
#endif

#if CONFIG_SDCARD_HOST
#define CLI_CFG_SD          1
#else
#define CLI_CFG_SD          0
#endif

#if CONFIG_FATFS
#define CLI_FATFS          1
#else
#define CLI_FATFS          0
#endif

#if CONFIG_AIRKISS_TEST
#define CLI_CFG_AIRKISS     1
#else
#define CLI_CFG_AIRKISS     0
#endif

#if CONFIG_IPERF_TEST
#define CLI_CFG_IPERF       1
#else
#define CLI_CFG_IPERF       0
#endif

#if (CONFIG_I2S)
#define CLI_CFG_I2S         1
#else
#define CLI_CFG_I2S         0
#endif

#if (CONFIG_SOC_BK7256XX || CONFIG_SOC_BK7256_CP1)

#if (CONFIG_DMA2D_TEST)
#define CLI_CFG_DMA2D  1
#endif

#if (CONFIG_AUDIO && CONFIG_DUAL_CORE && CONFIG_AUDIO_TEST && CONFIG_AUDIO_RISCV_IP_V1_0)
#define CLI_CFG_AUD         1
#endif

#if (CONFIG_SBC)
#define CLI_CFG_SBC			1
#endif

#if (CONFIG_LCD_TEST)
#define CLI_CFG_LCD  1
#endif

#if (CONFIG_DUAL_CORE && CONFIG_AUDIO_TEST && !CONFIG_SOC_BK7256_CP1)
#define CLI_CFG_AUD_CP0     1
#endif

#if (CONFIG_FFT_TEST)
#define CLI_CFG_FFT         1
#else
#define CLI_CFG_FFT         0
#endif
#endif

#if (CONFIG_AUDIO_AEC_TEST)
#define CLI_CFG_AEC         1
#endif


#if (CONFIG_SECURITYIP)
#define CLI_CFG_VAULT		1
#else
#define CLI_CFG_vault		0
#endif
#ifdef __cplusplus
}
#endif
