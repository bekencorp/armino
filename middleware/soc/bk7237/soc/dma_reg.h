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

#define DMA_R_BASE                   (SOC_DMA_REG_BASE)

#define DMA_V_PRIO_MODE_ROUND_ROBIN      0x0
#define DMA_V_PRIO_MODE_FIXED_PRIO       0x1

#define DMA_V_REQ_MUX_DTCM          0x0
#define DMA_V_REQ_MUX_UART1         0x1
#define DMA_V_REQ_MUX_GSPI1         0x2
#define DMA_V_REQ_MUX_SDIO          0x3
#define DMA_V_REQ_MUX_UART2         0x4
#define DMA_V_REQ_MUX_UART3         0x5
#define DMA_V_REQ_MUX_GSPI2         0x6
#define DMA_V_REQ_MUX_USB           0x7
#define DMA_V_REQ_MUX_AUDIO         0x8
#define DMA_V_REQ_MUX_I2S           0x9
//dest 0xa for LCD_CMD
#define DMA_V_REQ_MUX_LCD_CMD       0xa
#define DMA_V_REQ_MUX_LCD_DATA      0xb

//src 0xa for JPEG
#define DMA_V_REQ_MUX_JPEG          0xa


#ifdef __cplusplus
}
#endif

