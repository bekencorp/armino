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
#include <sdkconfig.h>

#ifdef __cplusplus
extern "C" {
#endif

#if CONFIG_CLI_USER_CONFIG
#include "cli_user_config.h"
#else
#if (CONFIG_WIFI_ENABLE) &&(!CONFIG_SLAVE_CORE)
#define CLI_CFG_WIFI        1
#else
#define CLI_CFG_WIFI        0
#endif //#if (CONFIG_WIFI_ENABLE)

#if (!CONFIG_SLAVE_CORE)
#define CLI_CFG_BLE         1
#else
#define CLI_CFG_BLE         0
#endif

#if (CONFIG_BK_NETIF &&(!CONFIG_SLAVE_CORE))
#define CLI_CFG_NETIF       1
#else
#define CLI_CFG_NETIF       0
#endif //#if (CONFIG_LWIP)

#define CLI_CFG_MISC        1
#define CLI_CFG_MEM         1

#if (CONFIG_WIFI_ENABLE &&(!CONFIG_SLAVE_CORE))
#define CLI_CFG_PHY         1
#else
#define CLI_CFG_PHY         0
#endif //#if (CONFIG_WIFI_ENABLE)

#if (CONFIG_STA_PS &&(!CONFIG_SLAVE_CORE))
#define CLI_CFG_PWR         1
#else
#define CLI_CFG_PWR         0
#endif
#define CLI_CFG_TIMER       1
#if (CONFIG_INT_WDT && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_WDT         1
#endif
#if (CONFIG_TRNG_SUPPORT && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_TRNG        1
#else
#define CLI_CFG_TRNG        0
#endif
#if (CONFIG_EFUSE && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_EFUSE       1
#else
#define CLI_CFG_EFUSE       0
#endif
#if (!CONFIG_SLAVE_CORE)
#define CLI_CFG_GPIO        1
#else
#define CLI_CFG_GPIO        0
#endif
#define CLI_CFG_OS          1
#if ((CONFIG_OTA_TFTP) || (CONFIG_OTA_HTTP)) && (!CONFIG_SLAVE_CORE)
#define CLI_CFG_OTA         1
#else
#define CLI_CFG_OTA         0
#endif
#if(CONFIG_KEYVALUE && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_KEYVALUE    1
#else
#define CLI_CFG_KEYVALUE    0
#endif
#if(CONFIG_SUPPORT_MATTER && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_MATTER      1
#else
#define CLI_CFG_MATTER      0
#endif

#if (!CONFIG_SLAVE_CORE)
#define CLI_CFG_UART        1
#define CLI_CFG_ADC         0
#define CLI_CFG_SPI         1
#define CLI_CFG_MICO        1
#define CLI_CFG_REG         1
#define CLI_CFG_EXCEPTION   1
#else
#define CLI_CFG_UART        0
#define CLI_CFG_ADC         0
#define CLI_CFG_SPI         0
#define CLI_CFG_MICO        0
#define CLI_CFG_REG         0
#define CLI_CFG_EXCEPTION   0
#endif

#if(CONFIG_GENERAL_DMA && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_DMA         1
#else
#define CLI_CFG_DMA         0
#endif

#if(CONFIG_PWM && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_PWM         1
#else
#define CLI_CFG_PWM         0
#endif

#if(CONFIG_FLASH) && (!CONFIG_SLAVE_CORE)
#define CLI_CFG_FLASH       1
#else
#define CLI_CFG_FLASH       0
#endif

#if(CONFIG_SDIO_HOST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_SDIO_HOST   1
#else
#define CLI_CFG_SDIO_HOST   0
#endif

#if(CONFIG_SDIO_SLAVE)
#define CLI_CFG_SDIO_SLAVE   1
#else
#define CLI_CFG_SDIO_SLAVE   0
#endif

#if(CONFIG_ICU && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_ICU         0
#else
#define CLI_CFG_ICU         0
#endif

#if (CONFIG_I2C && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_I2C         1
#else
#define CLI_CFG_I2C         0
#endif

#if (CONFIG_QSPI && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_QSPI        1
#else
#define CLI_CFG_QSPI        0
#endif

#if (CONFIG_AON_RTC_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_AON_RTC     1
#else
#define CLI_CFG_AON_RTC     0
#endif

#if (CONFIG_JPEGENC_HW && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_JPEGENC        1
#else
#define CLI_CFG_JPEGENC        0
#endif

#if (CONFIG_JPEGDEC_SW && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_JPEGDEC     0
#else
#define CLI_CFG_JPEGDEC     0
#endif

#if (CONFIG_CALENDAR && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_CALENDAR    1
#else
#define CLI_CFG_CALENDAR    0
#endif

//TODO default to 0
#define CLI_CFG_EVENT       1

#if (CONFIG_SOC_BK7251) && (!CONFIG_SLAVE_CORE)
#define CLI_CFG_SECURITY    1
#else
#define CLI_CFG_SECURITY    0
#endif

#if (CONFIG_TEMP_DETECT && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_TEMP_DETECT 1
#else
#define CLI_CFG_TEMP_DETECT 0
#endif

#if (CONFIG_SDCARD && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_SD          1
#else
#define CLI_CFG_SD          0
#endif

#if (CONFIG_FATFS && (!CONFIG_SLAVE_CORE))
#define CLI_FATFS          1
#else
#define CLI_FATFS          0
#endif

#if (CONFIG_VFS_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_VFS          1
#else
#define CLI_CFG_VFS          0
#endif

#if (CONFIG_AIRKISS_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_AIRKISS     1
#else
#define CLI_CFG_AIRKISS     0
#endif

#if (CONFIG_IPERF_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_IPERF       1
#else
#define CLI_CFG_IPERF       0
#endif

#if (CONFIG_I2S_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_I2S         1
#else
#define CLI_CFG_I2S         0
#endif

#if (CONFIG_LCD_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_LCD  0
#else
#define CLI_CFG_LCD  0
#endif

#if (CONFIG_HW_ROTT_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_ROTT  0
#else
#define CLI_CFG_ROTT  0
#endif



#if (CONFIG_DMA2D_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_DMA2D  0
#else
#define CLI_CFG_DMA2D  0
#endif

#if (CONFIG_SOC_BK7256XX)
#if (CONFIG_AUDIO && CONFIG_DUAL_CORE && CONFIG_AUDIO_TEST && CONFIG_AUDIO_RISCV_IP_V1_0 && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_AUD         0
#endif

#if (CONFIG_AUD_INTF_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_AUD_INTF    1
#endif

#if (CONFIG_AUDIO_RSP_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_AUD_RSP    0
#endif

#if (CONFIG_AUDIO_VAD_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_AUD_VAD    0
#endif

#if (CONFIG_AUDIO_NS_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_AUD_NS    0
#endif

#if (CONFIG_AUDIO_FLAC_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_AUD_FLAC    0
#endif

#if (CONFIG_SBC_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_SBC		0
#else
#define CLI_CFG_SBC		0
#endif

#if (CONFIG_TOUCH_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_TOUCH	1
#else
#define CLI_CFG_TOUCH	0
#endif


#if (CONFIG_LCD_QSPI_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_LCD_QSPI	0
#else
#define CLI_CFG_LCD_QSPI	0
#endif

#if (CONFIG_QRCODEGEN_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_QRCODEGEN	0
#else
#define CLI_CFG_QRCODEGEN	0
#endif

#if (CONFIG_DUAL_CORE && CONFIG_AUDIO_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_AUD_CP0     0
#endif

#if (CONFIG_FFT_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_FFT         0
#else
#define CLI_CFG_FFT         0
#endif
#endif


#if (CONFIG_SOC_BK7236XX)
#if (CONFIG_AUDIO && CONFIG_AUDIO_TEST)
#define CLI_CFG_AUD         1
#endif
#endif

#if (CONFIG_AUDIO_AEC_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_AEC         0
#endif

#if (CONFIG_AUDIO_G711_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_G711         0
#endif

#if (CONFIG_AUDIO_OPUS_TEST && (!CONFIG_SLAVE_CORE) && CONFIG_FATFS)
#define CLI_CFG_OPUS         0
#endif

#if (CONFIG_AUDIO_ADPCM_TEST && (!CONFIG_SLAVE_CORE) && CONFIG_FATFS)
#define CLI_CFG_ADPCM         0
#endif

#if (CONFIG_AUDIO_MP3_TEST && (!CONFIG_SLAVE_CORE) && CONFIG_FATFS)
#define CLI_CFG_MP3		0
#else
#define CLI_CFG_MP3		0
#endif

#if (CONFIG_SECURITYIP && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_SECURITYIP		1
#else
#define CLI_CFG_SECURITYIP		0
#endif

#if (CONFIG_CAMERA && CONFIG_CAMERA_TEST &&(!CONFIG_SLAVE_CORE))
#define CLI_CFG_DVP        1
#else
#define CLI_CFG_DVP        0
#endif

#if (CONFIG_PSRAM_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_PSRAM        0
#else
#define CLI_CFG_PSRAM        0
#endif

#if (CONFIG_H264)
#define CLI_CFG_H264         1
#else
#define CLI_CFG_H264         0
#endif

#if (CONFIG_AGORA_IOT_SDK_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_AGORA        1
#endif

#if (CONFIG_ES8311_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_ES8311        1
#endif

#if (CONFIG_ASR_TEST && (!CONFIG_SLAVE_CORE))
#define CLI_CFG_ASR         1
#endif

#endif

#ifdef __cplusplus
}
#endif
