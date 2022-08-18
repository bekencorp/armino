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

#include <soc/soc_port.h>
#include "soc_cap.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SOC_PMU_REG_BASE       (0x00800200)
#define SOC_GPIO_REG_BASE      (0x00800300)
#define SOC_ICU_REG_BASE       (0x00802000)
#define SOC_UART_REG_BASE      (0x00802100)
#define SOC_I2C0_REG_BASE      (0x00802200)
#define SOC_I2C1_REG_BASE      (0x00802240)
#define SOC_I2C2_REG_BASE      (0x00802280)
#define SOC_SDIO_HOST_REG_BASE (0x00802300)
#define SOC_SPI0_REG_BASE      (0x00802500)
#define SOC_SPI1_REG_BASE      (0x00802540)
#define SOC_SPI2_REG_BASE      (0x00802580)
#define SOC_WDT_REG_BASE       (0x00802700)
#define SOC_TRNG_REG_BASE      (0x00802720)
#define SOC_EFUSE_REG_BASE     (0x00802740)
#define SOC_TIMER_REG_BASE     (0x00802780)
#define SOC_PWM_REG_BASE       (0x00802800)
#define SOC_ADC_REG_BASE       (0x00802900)
#define SOC_FLASH_REG_BASE     (0x00803000)
#define SOC_DMA_REG_BASE       (0x00805000)
#define SOC_JPEG_REG_BASE      (0x00807000)
#define SOC_MAILBOX_REG_BASE   (0x06000000)

#define W_SYS_CTRL_BASE_ADDR            0x00800000
#define W_PMU_BASE_ADDR                 0x00800200
#define W_LCD_BASE_ADDR                 0x00800280
#define W_GPIO_BASE_ADDR                0x00800300
#define W_SDIO_DMA_BASE_ADDR            0x00801000
#define W_APB_BUS_BASE_ADDR             0x00802000
#define W_ICU_BASE_ADDR                 0x00802000
#define W_UART1_BASE_ADDR               0x00802100
#define W_UART2_BASE_ADDR               0x00802140
#define W_UART3_BASE_ADDR               0x00802180
#define W_FM_I2C_BASE_ADDR              0x00802200
#define W_I2C1_BASE_ADDR                0x00802240
#define W_I2C2_BASE_ADDR                0x00802280
#define W_SDIO_HOST_BASE_ADDR           0x00802300
#define W_SPI1_BASE_ADDR                0x00802500
#define W_SPI2_BASE_ADDR                0x00802540
#define W_SPI3_BASE_ADDR                0x00802580
#define W_WDT_BASE_ADDR                 0x00802700
#define W_TRNG_BASE_ADDR                0x00802720
#define W_EFUSE_BASE_ADDR               0x00802740
#define W_IRDA_BASE_ADDR                0x00802760
#define W_TIMER1_BASE_ADDR              0x00802780
#define W_TIMER2_BASE_ADDR              0x008027C0
#define W_PWM1_BASE_ADDR                0x00802800
#define W_PWM2_BASE_ADDR                0x00802840
#define W_PWM3_BASE_ADDR                0x00802880
#define W_PWM4_BASE_ADDR                0x008028C0
#define W_SADC_BASE_ADDR                0x00802900
#define W_CEC_BASE_ADDR                 0x00802A00
#define W_FLASH_BASE_ADDR               0x00803000
#define W_USB1_BASE_ADDR                0x00804000
#define W_USB2_BASE_ADDR                0x00804800
#define W_GENER_DMA_BASE_ADDR           0x00805000
#define W_SECURITY_BASE_ADDR            0x00806000
#define W_JPEG_BASE_ADDR                0x00807000
#define W_MDM_CFG_BASE_ADDR             0x00900000
#define W_MDM_STAT_BASE_ADDR            0x00900000
#define W_RC_BEKEN_BASE_ADDR            0x00950000
#define W_TRX_BEKEN_BASE_ADDR           0x00950080
#define W_MACPHY_BYPASS_BASE_ADDR       0x00960000
#define W_MAC_CORE_BASE_ADDR            0x00A00000
#define W_MAC_PL_BASE_ADDR              0x00A08000
#define W_YUV_MEM_BASE_ADDR             0x00B00000
#define W_WIFI_DTCM_512KB_BASE_ADDR     0x00400000
#define W_WIFI_ITCM_32KB_BASE_ADDR      0x003F8000
#define W_BUS_SMEM_128KB_BASE_ADDR      0x04000000
#define W_DSP_DMEM_64KB_BASE_ADDR       0x0C000000
#define W_DSP_CPM_BASE_ADDR             0x0C400000
#define W_DSP_SMEM_1_5MB_BASE_ADDR      0x0C800000
#define W_PSRAM_BASE_ADDR               0x0D000000
#define W_DMA_BASE_ADDR                 0x0E800000
#define W_FFT_BASE_ADDR                 0x0E810000
#define W_APBD_BUS_BASE_ADDR            0x0E8F0000
#define W_ICUD_BASE_ADDR                0x0E8F0000
#define W_AUDIO_BASE_ADDR               0x0E8F1000
#define W_SPDIF_BASE_ADDR               0x0E8F2000
#define W_EQ_BASE_ADDR                  0x0E8F3000
#define W_I2S1_BASE_ADDR                0x0E8F4000
#define W_WDTD_BASE_ADDR                0x0E8F5000
#define W_RSV_BASE_ADDR                 0x0E8F6000
#define W_DSP_CTRL_BASE_ADDR            0x0E8F7000
#define W_I2S2_BASE_ADDR                0x0E8F8000
#define W_I2S3_BASE_ADDR                0x0E8F9000
#define W_PSRAM_CTRL_BASE_ADDR          0x0E8FA000
#define W_BT_IMEM_384KB_BASE_ADDR       0x10000000
#define W_BT_DMEM_96KB_BASE_ADDR        0x10400000

#define REG_WRITE(_r, _v) ({\
		(*(volatile uint32_t *)(_r)) = (_v);\
	})

#define REG_READ(_r) ({\
		(*(volatile uint32_t *)(_r));\
	})

#define REG_GET_BIT(_r, _b)  ({\
		(*(volatile uint32_t*)(_r) & (_b));\
	})

#define REG_SET_BIT(_r, _b)  ({\
		(*(volatile uint32_t*)(_r) |= (_b));\
	})

#define REG_CLR_BIT(_r, _b)  ({\
		(*(volatile uint32_t*)(_r) &= ~(_b));\
	})

#define REG_SET_BITS(_r, _b, _m) ({\
		(*(volatile uint32_t*)(_r) = (*(volatile uint32_t*)(_r) & ~(_m)) | ((_b) & (_m)));\
	})

#define REG_GET_FIELD(_r, _f) ({\
		((REG_READ(_r) >> (_f##_S)) & (_f##_V));\
	})

#define REG_SET_FIELD(_r, _f, _v) ({\
		(REG_WRITE((_r),((REG_READ(_r) & ~((_f##_V) << (_f##_S)))|(((_v) & (_f##_V))<<(_f##_S)))));\
	})

#define REG_MCHAN_GET_FIELD(_ch, _r, _f) ({\
		((REG_READ(_r) >> (_f##_MS(_ch))) & (_f##_V));\
	})

#define REG_MCHAN_SET_FIELD(_ch, _r, _f, _v) ({\
		(REG_WRITE((_r), ((REG_READ(_r) & ~((_f##_V) << (_f##_MS(_ch))))|(((_v) & (_f##_V))<<(_f##_MS(_ch))))));\
	})

#define VALUE_GET_FIELD(_r, _f) (((_r) >> (_f##_S)) & (_f))

#define VALUE_GET_FIELD2(_r, _f) (((_r) & (_f))>> (_f##_S))

#define VALUE_SET_FIELD(_r, _f, _v) ((_r)=(((_r) & ~((_f) << (_f##_S)))|((_v)<<(_f##_S))))

#define VALUE_SET_FIELD2(_r, _f, _v) ((_r)=(((_r) & ~(_f))|((_v)<<(_f##_S))))

#define FIELD_TO_VALUE(_f, _v) (((_v)&(_f))<<_f##_S)

#define FIELD_TO_VALUE2(_f, _v) (((_v)<<_f##_S) & (_f))

#ifndef BIT
#define BIT(i) (1<<(i))
#endif

#ifndef BIT64
#define BIT64(i)                  (1LL << (i))
#endif

#ifdef __cplusplus
}
#endif
