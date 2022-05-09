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
#include "reg_base.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SOC_VAULT_HOST_BASE      (0x4B000000)
#define SOC_VAULT_OTP_BASE       (0x4B004000)
#define SOC_VAULT_REG_BASE       (0x4B008000)
#if 0
#define SOC_EFUSE_REG_BASE       (0x44880000)
#define SOC_SYSTEM_REG_BASE      (0x44010000)
#define SOC_UART_REG_BASE        (0x44820000)
#define SOC_TRNG_REG_BASE        (0x448A0000)
#define SOC_WDT_REG_BASE         (0x44800000)
#define SOC_TIMER_REG_BASE       (0x44810000)
#define SOC_GPIO_REG_BASE        (0x44000400)
#define SOC_DMA_REG_BASE         (0x44020000)
#define SOC_ADC_REG_BASE         (0x44870000)
#define SOC_SPI1_REG_BASE        (0x44860000)
#define SOC_SPI2_REG_BASE        (0x458C0000)
#define SOC_MAILBOX_REG_BASE     (0xA0000000)
#define SOC_QSPI_REG_BASE        (0x46040000)
#define SOC_QSPI_DATA_REG_BASE   (0x68000000)
#endif
#define SOC_PWM_REG_BASE         (0x44840000)
#define SOC_ICU_REG_BASE         (0x44010000)
#define SOC_FLASH_REG_BASE       (0x44030000)
#if 0
#define SOC_AON_PMU_REG_BASE     (0x44000000)
#define SOC_I2C0_REG_BASE        (0x44850000)
#define SOC_I2C1_REG_BASE        (0x45890000)
#define SOC_AUD_REG_BASE         (0x47800000)
#endif

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
