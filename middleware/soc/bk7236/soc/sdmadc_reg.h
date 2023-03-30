// Copyright 2022-2023 Beken
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

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!

#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#define SDMADC_REG0X0_ADDR (SOC_SDMADC_REG_BASE + (0x0 << 2))

#define SDMADC_REG0X0_DEVICEID_POS (0)
#define SDMADC_REG0X0_DEVICEID_MASK (0xffffffff)

#define SDMADC_REG0X1_ADDR (SOC_SDMADC_REG_BASE + (0x1 << 2))

#define SDMADC_REG0X1_VERSIONID_POS (0)
#define SDMADC_REG0X1_VERSIONID_MASK (0xffffffff)

#define SDMADC_REG0X2_ADDR (SOC_SDMADC_REG_BASE + (0x2 << 2))

#define SDMADC_REG0X2_SOFT_RST_POS (0)
#define SDMADC_REG0X2_SOFT_RST_MASK (0x1)

#define SDMADC_REG0X2_BYPASS_CKG_POS (1)
#define SDMADC_REG0X2_BYPASS_CKG_MASK (0x1)

#define SDMADC_REG0X2_RESERVED_BIT_2_31_POS (2)
#define SDMADC_REG0X2_RESERVED_BIT_2_31_MASK (0x3fffffff)

#define SDMADC_REG0X3_ADDR (SOC_SDMADC_REG_BASE + (0x3 << 2))

#define SDMADC_REG0X3_GLOBAL_STATUS_POS (0)
#define SDMADC_REG0X3_GLOBAL_STATUS_MASK (0xffffffff)

#define SDMADC_REG0X4_ADDR (SOC_SDMADC_REG_BASE + (0x4 << 2))

#define SDMADC_REG0X4_SAMPLE_ENABLE_POS (0)
#define SDMADC_REG0X4_SAMPLE_ENABLE_MASK (0x1)

#define SDMADC_REG0X4_RESERVED_BIT_1_31_POS (1)
#define SDMADC_REG0X4_RESERVED_BIT_1_31_MASK (0x7fffffff)

#define SDMADC_REG0X5_ADDR (SOC_SDMADC_REG_BASE + (0x5 << 2))

#define SDMADC_REG0X5_SAMPLE_MODE_POS (0)
#define SDMADC_REG0X5_SAMPLE_MODE_MASK (0x1)

#define SDMADC_REG0X5_SAMPLE_NUMB_POS (1)
#define SDMADC_REG0X5_SAMPLE_NUMB_MASK (0x3)

#define SDMADC_REG0X5_SAMPLE_CHSEL_POS (3)
#define SDMADC_REG0X5_SAMPLE_CHSEL_MASK (0xf)

#define SDMADC_REG0X5_CIC2_BYPASS_POS (7)
#define SDMADC_REG0X5_CIC2_BYPASS_MASK (0x1)

#define SDMADC_REG0X5_COMP_BYPASS_POS (8)
#define SDMADC_REG0X5_COMP_BYPASS_MASK (0x1)

#define SDMADC_REG0X5_CIC2_GAINS_POS (9)
#define SDMADC_REG0X5_CIC2_GAINS_MASK (0x3f)

#define SDMADC_REG0X5_INTR_ENABLE_POS (15)
#define SDMADC_REG0X5_INTR_ENABLE_MASK (0x1f)

#define SDMADC_REG0X5_RESERVED_BIT_20_31_POS (20)
#define SDMADC_REG0X5_RESERVED_BIT_20_31_MASK (0xfff)

#define SDMADC_REG0X6_ADDR (SOC_SDMADC_REG_BASE + (0x6 << 2))

#define SDMADC_REG0X6_CALI_OFFSET_POS (0)
#define SDMADC_REG0X6_CALI_OFFSET_MASK (0xffff)

#define SDMADC_REG0X6_CALI_GAIN_POS (16)
#define SDMADC_REG0X6_CALI_GAIN_MASK (0x1fff)

#define SDMADC_REG0X6_RESERVED_BIT_29_31_POS (29)
#define SDMADC_REG0X6_RESERVED_BIT_29_31_MASK (0x7)

#define SDMADC_REG0X7_ADDR (SOC_SDMADC_REG_BASE + (0x7 << 2))

#define SDMADC_REG0X7_SADC_STATUS_POS (0)
#define SDMADC_REG0X7_SADC_STATUS_MASK (0x1f)

#define SDMADC_REG0X7_RESERVED_BIT_5_31_POS (5)
#define SDMADC_REG0X7_RESERVED_BIT_5_31_MASK (0x7ffffff)

#define SDMADC_REG0X8_ADDR (SOC_SDMADC_REG_BASE + (0x8 << 2))

#define SDMADC_REG0X8_FIFO_DATA_POS (0)
#define SDMADC_REG0X8_FIFO_DATA_MASK (0xffff)

#define SDMADC_REG0X8_RESERVED_BIT_16_31_POS (16)
#define SDMADC_REG0X8_RESERVED_BIT_16_31_MASK (0xffff)

#ifdef __cplusplus
}
#endif
