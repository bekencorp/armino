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

#ifdef __cplusplus
extern "C" {
#endif

#define SOC_SYS_CTRL_REG_BASE (0x00800000)
#define SOC_EFUSE_REG_BASE    (SOC_SYS_CTRL_REG_BASE + 0x1D * 4)
#define SOC_ICU_REG_BASE      (0x00802000)
#define SOC_UART_REG_BASE     (0x00802100)
#define SOC_I2C0_REG_BASE     (0x00802300)
#define SOC_TRNG_REG_BASE     (0x00802480)
#define SOC_I2C1_REG_BASE     (0x00802600)
#define SOC_SPI_REG_BASE      (0x00802700)
#define SOC_GPIO_REG_BASE     (0x00802800)
#define SOC_WDT_REG_BASE      (0x00802900)
#define SOC_TIMER_REG_BASE    (0x00802A00)
#define SOC_PWM_REG_BASE      (SOC_TIMER_REG_BASE + 0x20 * 4) /**< 0x802A80 */
#define SOC_ADC_REG_BASE      (0x00802C00)
#define SOC_DMA_REG_BASE      (0x00809000)
#define SOC_JPEG_REG_BASE     (0x0080A000)
#define SOC_QSPI_REG_BASE     (0x0080D000)

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
#define BIT(i) ((1L)<<(i))
#endif

#ifndef BIT64
#define BIT64(i)                  (1LL << (i))
#endif

#ifdef __cplusplus
}
#endif
