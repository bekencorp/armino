// Copyright 2020-2022 Beken
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

/***********************************************************************************************************************************
* This file is generated from BK7256_ADDR_Mapping_MP2.xlsm automatically
* Modify it manually is not recommended
* CHIP ID:BK7256,GENARATE TIME:2022-07-27 17:17:11
************************************************************************************************************************************/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <soc/soc.h>

#define DMA2D_LL_REG_BASE      (SOC_DMA2D_REG_BASE) //REG_BASE:0x48020000

/* REG_0x00 */
#define DMA2D_DEVICE_ID_ADDR  (DMA2D_LL_REG_BASE  + 0x0*4)

static inline uint32_t dma2d_ll_get_devide_id(void)
{
    return REG_READ(DMA2D_DEVICE_ID_ADDR);
}

/* REG_0x01 */
#define DMA2D_VERSION_ID_ADDR  (DMA2D_LL_REG_BASE  + 0x1*4)

static inline uint32_t dma2d_ll_get_version_id(void)
{
    return REG_READ(DMA2D_VERSION_ID_ADDR);
}

/* REG_0x02 */
#define DMA2_MODULE_CONTROL_ADDR  (DMA2D_LL_REG_BASE  + 0x2*4)
#define DMA2D_MODULE_CONTROL_REG_SOFT_RESET_POS (0)
#define DMA2D_MODULE_CONTROL_REG_SOFT_RESET_MASK (0x1)

#define DMA2D_MODULE_CONTROL_CLK_GATE_POS (1)
#define DMA2D_MODULE_CONTROL_CLK_GATE_MASK (0x1)

static inline uint32_t dma2d_ll_get_module_control_soft_reset(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2_MODULE_CONTROL_ADDR);
    reg_value = ((reg_value >> DMA2D_MODULE_CONTROL_REG_SOFT_RESET_POS) & DMA2D_MODULE_CONTROL_REG_SOFT_RESET_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_module_control_soft_reset(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2_MODULE_CONTROL_ADDR);
    reg_value &= ~(DMA2D_MODULE_CONTROL_REG_SOFT_RESET_MASK << DMA2D_MODULE_CONTROL_REG_SOFT_RESET_POS);
    reg_value |= ((value & DMA2D_MODULE_CONTROL_REG_SOFT_RESET_MASK) << DMA2D_MODULE_CONTROL_REG_SOFT_RESET_POS);
    REG_WRITE(DMA2_MODULE_CONTROL_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_module_control_clk_gate(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2_MODULE_CONTROL_ADDR);
    reg_value = ((reg_value >> DMA2D_MODULE_CONTROL_CLK_GATE_POS) & DMA2D_MODULE_CONTROL_CLK_GATE_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_module_control_clk_gate(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2_MODULE_CONTROL_ADDR);
    reg_value &= ~(DMA2D_MODULE_CONTROL_CLK_GATE_MASK << DMA2D_MODULE_CONTROL_CLK_GATE_POS);
    reg_value |= ((value & DMA2D_MODULE_CONTROL_CLK_GATE_MASK) << DMA2D_MODULE_CONTROL_CLK_GATE_POS);
    REG_WRITE(DMA2_MODULE_CONTROL_ADDR,reg_value);
}

/* REG_0x03 */
#define DMA2D_GLOBAL_STATUS_ADDR  (DMA2D_LL_REG_BASE  + 0x3*4)

static inline uint32_t dma2d_ll_get_global_status(void)
{
    return REG_READ(DMA2D_GLOBAL_STATUS_ADDR);
}

/* REG_0x04 */
#define DMA2D_DMA2D_CONTROL_REG_ADDR  (DMA2D_LL_REG_BASE  + 0x4*4)
#define DMA2D_DMA2D_CONTROL_REG_TRAN_START_POS (0)
#define DMA2D_DMA2D_CONTROL_REG_TRAN_START_MASK (0x1)

#define DMA2D_DMA2D_CONTROL_REG_TRAN_SUSPEND_POS (1)
#define DMA2D_DMA2D_CONTROL_REG_TRAN_SUSPEND_MASK (0x1)

#define DMA2D_DMA2D_CONTROL_REG_TRAN_ABORT_POS (2)
#define DMA2D_DMA2D_CONTROL_REG_TRAN_ABORT_MASK (0x1)

#define DMA2D_DMA2D_CONTROL_REG_RESERVED3_POS (3)
#define DMA2D_DMA2D_CONTROL_REG_RESERVED3_MASK (0x7)

#define DMA2D_DMA2D_CONTROL_REG_LINE_OFFSET_MODE_POS (6)
#define DMA2D_DMA2D_CONTROL_REG_LINE_OFFSET_MODE_MASK (0x1)

#define DMA2D_DMA2D_CONTROL_REG_RESERVED2_POS (7)
#define DMA2D_DMA2D_CONTROL_REG_RESERVED2_MASK (0x1)

#define DMA2D_DMA2D_CONTROL_REG_ERROR_INT_EN_POS (8)
#define DMA2D_DMA2D_CONTROL_REG_ERROR_INT_EN_MASK (0x1)

#define DMA2D_DMA2D_CONTROL_REG_COMPLETE_INT_EN_POS (9)
#define DMA2D_DMA2D_CONTROL_REG_COMPLETE_INT_EN_MASK (0x1)

#define DMA2D_DMA2D_CONTROL_REG_WATERM_INT_EN_POS (10)
#define DMA2D_DMA2D_CONTROL_REG_WATERM_INT_EN_MASK (0x1)

#define DMA2D_DMA2D_CONTROL_REG_CLUT_ERROR_INT_EN_POS (11)
#define DMA2D_DMA2D_CONTROL_REG_CLUT_ERROR_INT_EN_MASK (0x1)

#define DMA2D_DMA2D_CONTROL_REG_CLUT_CMPLT_INT_EN_POS (12)
#define DMA2D_DMA2D_CONTROL_REG_CLUT_CMPLT_INT_EN_MASK (0x1)

#define DMA2D_DMA2D_CONTROL_REG_CONFIG_ERROR_INT_EN_POS (13)
#define DMA2D_DMA2D_CONTROL_REG_CONFIG_ERROR_INT_EN_MASK (0x1)

#define DMA2D_DMA2D_CONTROL_REG_RESERVED1_POS (14)
#define DMA2D_DMA2D_CONTROL_REG_RESERVED1_MASK (0x3)

#define DMA2D_DMA2D_CONTROL_REG_MODE_POS (16)
#define DMA2D_DMA2D_CONTROL_REG_MODE_MASK (0x7)

#define DMA2D_DMA2D_CONTROL_REG_RESERVED0_POS (19)
#define DMA2D_DMA2D_CONTROL_REG_RESERVED0_MASK (0x1FFF)

static inline uint32_t dma2d_ll_get_dma2d_control_reg_value(void)
{
    return REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
}

static inline void dma2d_ll_set_dma2d_control_reg_value(uint32_t value)
{
    REG_WRITE(DMA2D_DMA2D_CONTROL_REG_ADDR,value);
}

static inline uint32_t dma2d_ll_get_dma2d_control_reg_tran_start(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_CONTROL_REG_TRAN_START_POS) & DMA2D_DMA2D_CONTROL_REG_TRAN_START_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_control_reg_tran_start(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value &= ~(DMA2D_DMA2D_CONTROL_REG_TRAN_START_MASK << DMA2D_DMA2D_CONTROL_REG_TRAN_START_POS);
    reg_value |= ((value & DMA2D_DMA2D_CONTROL_REG_TRAN_START_MASK) << DMA2D_DMA2D_CONTROL_REG_TRAN_START_POS);
    REG_WRITE(DMA2D_DMA2D_CONTROL_REG_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_control_reg_tran_suspend(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_CONTROL_REG_TRAN_SUSPEND_POS) & DMA2D_DMA2D_CONTROL_REG_TRAN_SUSPEND_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_control_reg_tran_suspend(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value &= ~(DMA2D_DMA2D_CONTROL_REG_TRAN_SUSPEND_MASK << DMA2D_DMA2D_CONTROL_REG_TRAN_SUSPEND_POS);
    reg_value |= ((value & DMA2D_DMA2D_CONTROL_REG_TRAN_SUSPEND_MASK) << DMA2D_DMA2D_CONTROL_REG_TRAN_SUSPEND_POS);
    REG_WRITE(DMA2D_DMA2D_CONTROL_REG_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_control_reg_tran_abort(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_CONTROL_REG_TRAN_ABORT_POS) & DMA2D_DMA2D_CONTROL_REG_TRAN_ABORT_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_control_reg_tran_abort(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value &= ~(DMA2D_DMA2D_CONTROL_REG_TRAN_ABORT_MASK << DMA2D_DMA2D_CONTROL_REG_TRAN_ABORT_POS);
    reg_value |= ((value & DMA2D_DMA2D_CONTROL_REG_TRAN_ABORT_MASK) << DMA2D_DMA2D_CONTROL_REG_TRAN_ABORT_POS);
    REG_WRITE(DMA2D_DMA2D_CONTROL_REG_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_control_reg_line_offset_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_CONTROL_REG_LINE_OFFSET_MODE_POS) & DMA2D_DMA2D_CONTROL_REG_LINE_OFFSET_MODE_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_control_reg_line_offset_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value &= ~(DMA2D_DMA2D_CONTROL_REG_LINE_OFFSET_MODE_MASK << DMA2D_DMA2D_CONTROL_REG_LINE_OFFSET_MODE_POS);
    reg_value |= ((value & DMA2D_DMA2D_CONTROL_REG_LINE_OFFSET_MODE_MASK) << DMA2D_DMA2D_CONTROL_REG_LINE_OFFSET_MODE_POS);
    REG_WRITE(DMA2D_DMA2D_CONTROL_REG_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_control_reg_error_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_CONTROL_REG_ERROR_INT_EN_POS) & DMA2D_DMA2D_CONTROL_REG_ERROR_INT_EN_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_control_reg_error_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value &= ~(DMA2D_DMA2D_CONTROL_REG_ERROR_INT_EN_MASK << DMA2D_DMA2D_CONTROL_REG_ERROR_INT_EN_POS);
    reg_value |= ((value & DMA2D_DMA2D_CONTROL_REG_ERROR_INT_EN_MASK) << DMA2D_DMA2D_CONTROL_REG_ERROR_INT_EN_POS);
    REG_WRITE(DMA2D_DMA2D_CONTROL_REG_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_control_reg_complete_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_CONTROL_REG_COMPLETE_INT_EN_POS) & DMA2D_DMA2D_CONTROL_REG_COMPLETE_INT_EN_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_control_reg_complete_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value &= ~(DMA2D_DMA2D_CONTROL_REG_COMPLETE_INT_EN_MASK << DMA2D_DMA2D_CONTROL_REG_COMPLETE_INT_EN_POS);
    reg_value |= ((value & DMA2D_DMA2D_CONTROL_REG_COMPLETE_INT_EN_MASK) << DMA2D_DMA2D_CONTROL_REG_COMPLETE_INT_EN_POS);
    REG_WRITE(DMA2D_DMA2D_CONTROL_REG_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_control_reg_waterm_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_CONTROL_REG_WATERM_INT_EN_POS) & DMA2D_DMA2D_CONTROL_REG_WATERM_INT_EN_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_control_reg_waterm_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value &= ~(DMA2D_DMA2D_CONTROL_REG_WATERM_INT_EN_MASK << DMA2D_DMA2D_CONTROL_REG_WATERM_INT_EN_POS);
    reg_value |= ((value & DMA2D_DMA2D_CONTROL_REG_WATERM_INT_EN_MASK) << DMA2D_DMA2D_CONTROL_REG_WATERM_INT_EN_POS);
    REG_WRITE(DMA2D_DMA2D_CONTROL_REG_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_control_reg_clut_error_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_CONTROL_REG_CLUT_ERROR_INT_EN_POS) & DMA2D_DMA2D_CONTROL_REG_CLUT_ERROR_INT_EN_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_control_reg_clut_error_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value &= ~(DMA2D_DMA2D_CONTROL_REG_CLUT_ERROR_INT_EN_MASK << DMA2D_DMA2D_CONTROL_REG_CLUT_ERROR_INT_EN_POS);
    reg_value |= ((value & DMA2D_DMA2D_CONTROL_REG_CLUT_ERROR_INT_EN_MASK) << DMA2D_DMA2D_CONTROL_REG_CLUT_ERROR_INT_EN_POS);
    REG_WRITE(DMA2D_DMA2D_CONTROL_REG_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_control_reg_clut_cmplt_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_CONTROL_REG_CLUT_CMPLT_INT_EN_POS) & DMA2D_DMA2D_CONTROL_REG_CLUT_CMPLT_INT_EN_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_control_reg_clut_cmplt_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value &= ~(DMA2D_DMA2D_CONTROL_REG_CLUT_CMPLT_INT_EN_MASK << DMA2D_DMA2D_CONTROL_REG_CLUT_CMPLT_INT_EN_POS);
    reg_value |= ((value & DMA2D_DMA2D_CONTROL_REG_CLUT_CMPLT_INT_EN_MASK) << DMA2D_DMA2D_CONTROL_REG_CLUT_CMPLT_INT_EN_POS);
    REG_WRITE(DMA2D_DMA2D_CONTROL_REG_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_control_reg_config_error_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_CONTROL_REG_CONFIG_ERROR_INT_EN_POS) & DMA2D_DMA2D_CONTROL_REG_CONFIG_ERROR_INT_EN_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_control_reg_config_error_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value &= ~(DMA2D_DMA2D_CONTROL_REG_CONFIG_ERROR_INT_EN_MASK << DMA2D_DMA2D_CONTROL_REG_CONFIG_ERROR_INT_EN_POS);
    reg_value |= ((value & DMA2D_DMA2D_CONTROL_REG_CONFIG_ERROR_INT_EN_MASK) << DMA2D_DMA2D_CONTROL_REG_CONFIG_ERROR_INT_EN_POS);
    REG_WRITE(DMA2D_DMA2D_CONTROL_REG_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_control_reg_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_CONTROL_REG_MODE_POS) & DMA2D_DMA2D_CONTROL_REG_MODE_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_control_reg_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_CONTROL_REG_ADDR);
    reg_value &= ~(DMA2D_DMA2D_CONTROL_REG_MODE_MASK << DMA2D_DMA2D_CONTROL_REG_MODE_POS);
    reg_value |= ((value & DMA2D_DMA2D_CONTROL_REG_MODE_MASK) << DMA2D_DMA2D_CONTROL_REG_MODE_POS);
    REG_WRITE(DMA2D_DMA2D_CONTROL_REG_ADDR,reg_value);
}

/* REG_0x05 */
#define DMA2D_DMA2D_INT_STATUS_ADDR  (DMA2D_LL_REG_BASE  + 0x5*4)
#define DMA2D_DMA2D_INT_STATUS_ERROR_INT_POS (0)
#define DMA2D_DMA2D_INT_STATUS_ERROR_INT_MASK (0x1)

#define DMA2D_DMA2D_INT_STATUS_COMPLETE_INT_POS (1)
#define DMA2D_DMA2D_INT_STATUS_COMPLETE_INT_MASK (0x1)

#define DMA2D_DMA2D_INT_STATUS_WATERM_INT_POS (2)
#define DMA2D_DMA2D_INT_STATUS_WATERM_INT_MASK (0x1)

#define DMA2D_DMA2D_INT_STATUS_CLUT_ERROR_INT_POS (3)
#define DMA2D_DMA2D_INT_STATUS_CLUT_ERROR_INT_MASK (0x1)

#define DMA2D_DMA2D_INT_STATUS_CLUT_CMPLT_INT_POS (4)
#define DMA2D_DMA2D_INT_STATUS_CLUT_CMPLT_INT_MASK (0x1)

#define DMA2D_DMA2D_INT_STATUS_CONFIG_ERROR_INT_POS (5)
#define DMA2D_DMA2D_INT_STATUS_CONFIG_ERROR_INT_MASK (0x1)

#define DMA2D_DMA2D_INT_STATUS_RESERVED_POS (6)
#define DMA2D_DMA2D_INT_STATUS_RESERVED_MASK (0x3FFFFFF)

static inline uint32_t dma2d_ll_get_dma2d_int_status_value(void)
{
    return REG_READ(DMA2D_DMA2D_INT_STATUS_ADDR);
}

static inline uint32_t dma2d_ll_get_dma2d_int_status_error_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_INT_STATUS_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_INT_STATUS_ERROR_INT_POS)&DMA2D_DMA2D_INT_STATUS_ERROR_INT_MASK);
    return reg_value;
}

static inline uint32_t dma2d_ll_get_dma2d_int_status_complete_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_INT_STATUS_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_INT_STATUS_COMPLETE_INT_POS)&DMA2D_DMA2D_INT_STATUS_COMPLETE_INT_MASK);
    return reg_value;
}

static inline uint32_t dma2d_ll_get_dma2d_int_status_waterm_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_INT_STATUS_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_INT_STATUS_WATERM_INT_POS)&DMA2D_DMA2D_INT_STATUS_WATERM_INT_MASK);
    return reg_value;
}

static inline uint32_t dma2d_ll_get_dma2d_int_status_clut_error_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_INT_STATUS_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_INT_STATUS_CLUT_ERROR_INT_POS)&DMA2D_DMA2D_INT_STATUS_CLUT_ERROR_INT_MASK);
    return reg_value;
}

static inline uint32_t dma2d_ll_get_dma2d_int_status_clut_cmplt_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_INT_STATUS_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_INT_STATUS_CLUT_CMPLT_INT_POS)&DMA2D_DMA2D_INT_STATUS_CLUT_CMPLT_INT_MASK);
    return reg_value;
}

static inline uint32_t dma2d_ll_get_dma2d_int_status_config_error_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_INT_STATUS_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_INT_STATUS_CONFIG_ERROR_INT_POS)&DMA2D_DMA2D_INT_STATUS_CONFIG_ERROR_INT_MASK);
    return reg_value;
}

/* REG_0x06 */
#define DMA2D_DMA2D_INT_CLEAR_ADDR  (DMA2D_LL_REG_BASE  + 0x6*4)
#define DMA2D_DMA2D_INT_CLEAR_CLR_ERROR_INT_POS (0)
#define DMA2D_DMA2D_INT_CLEAR_CLR_ERROR_INT_MASK (0x1)

#define DMA2D_DMA2D_INT_CLEAR_CLR_COMPLETE_INT_POS (1)
#define DMA2D_DMA2D_INT_CLEAR_CLR_COMPLETE_INT_MASK (0x1)

#define DMA2D_DMA2D_INT_CLEAR_CLR_WATERM_INT_POS (2)
#define DMA2D_DMA2D_INT_CLEAR_CLR_WATERM_INT_MASK (0x1)

#define DMA2D_DMA2D_INT_CLEAR_CLR_CLUT_ERROR_INT_POS (3)
#define DMA2D_DMA2D_INT_CLEAR_CLR_CLUT_ERROR_INT_MASK (0x1)

#define DMA2D_DMA2D_INT_CLEAR_CLR_CLUT_CMPLT_INT_POS (4)
#define DMA2D_DMA2D_INT_CLEAR_CLR_CLUT_CMPLT_INT_MASK (0x1)

#define DMA2D_DMA2D_INT_CLEAR_CLR_CONFIG_ERROR_INT_POS (5)
#define DMA2D_DMA2D_INT_CLEAR_CLR_CONFIG_ERROR_INT_MASK (0x1)

#define DMA2D_DMA2D_INT_CLEAR_RESERVED_POS (6)
#define DMA2D_DMA2D_INT_CLEAR_RESERVED_MASK (0x3FFFFFF)

/*write only reg:dma2d_int_clear:default value:0x0*/
static inline void dma2d_ll_set_dma2d_int_clear_value(uint32_t value)
{
    REG_WRITE(DMA2D_DMA2D_INT_CLEAR_ADDR,value);
}

/* REG_0x07 */
#define DMA2D_DMA2D_FG_ADDRESS_ADDR  (DMA2D_LL_REG_BASE  + 0x7*4)
#define DMA2D_DMA2D_FG_ADDRESS_FG_ADDRESS_POS (0)
#define DMA2D_DMA2D_FG_ADDRESS_FG_ADDRESS_MASK (0xFFFFFFFF)

static inline uint32_t dma2d_ll_get_dma2d_fg_address_value(void)
{
    return REG_READ(DMA2D_DMA2D_FG_ADDRESS_ADDR);
}

static inline void dma2d_ll_set_dma2d_fg_address_value(uint32_t value)
{
    REG_WRITE(DMA2D_DMA2D_FG_ADDRESS_ADDR,value);
}

static inline uint32_t dma2d_ll_get_dma2d_fg_address_fg_address(void)
{
    return REG_READ(DMA2D_DMA2D_FG_ADDRESS_ADDR);
}

static inline void dma2d_ll_set_dma2d_fg_address_fg_address(uint32_t value)
{
    REG_WRITE(DMA2D_DMA2D_FG_ADDRESS_ADDR,value);
}

/* REG_0x08 */
#define DMA2D_DMA2D_FG_OFFSET_ADDR  (DMA2D_LL_REG_BASE  + 0x8*4)
#define DMA2D_DMA2D_FG_OFFSET_FG_LINE_OFFSET_POS (0)
#define DMA2D_DMA2D_FG_OFFSET_FG_LINE_OFFSET_MASK (0xFFFF)

#define DMA2D_DMA2D_FG_OFFSET_RESERVED_POS (16)
#define DMA2D_DMA2D_FG_OFFSET_RESERVED_MASK (0xFFFF)

static inline uint32_t dma2d_ll_get_dma2d_fg_offset_value(void)
{
    return REG_READ(DMA2D_DMA2D_FG_OFFSET_ADDR);
}

static inline void dma2d_ll_set_dma2d_fg_offset_value(uint32_t value)
{
    REG_WRITE(DMA2D_DMA2D_FG_OFFSET_ADDR,value);
}

static inline uint32_t dma2d_ll_get_dma2d_fg_offset_fg_line_offset(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_OFFSET_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_FG_OFFSET_FG_LINE_OFFSET_POS) & DMA2D_DMA2D_FG_OFFSET_FG_LINE_OFFSET_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_fg_offset_fg_line_offset(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_OFFSET_ADDR);
    reg_value &= ~(DMA2D_DMA2D_FG_OFFSET_FG_LINE_OFFSET_MASK << DMA2D_DMA2D_FG_OFFSET_FG_LINE_OFFSET_POS);
    reg_value |= ((value & DMA2D_DMA2D_FG_OFFSET_FG_LINE_OFFSET_MASK) << DMA2D_DMA2D_FG_OFFSET_FG_LINE_OFFSET_POS);
    REG_WRITE(DMA2D_DMA2D_FG_OFFSET_ADDR,reg_value);
}

/* REG_0x09 */
#define DMA2D_DMA2D_BG_ADDRESS_ADDR  (DMA2D_LL_REG_BASE  + 0x9*4)
#define DMA2D_DMA2D_BG_ADDRESS_BG_ADDRESS_POS (0)
#define DMA2D_DMA2D_BG_ADDRESS_BG_ADDRESS_MASK (0xFFFFFFFF)

static inline uint32_t dma2d_ll_get_dma2d_bg_address_value(void)
{
    return REG_READ(DMA2D_DMA2D_BG_ADDRESS_ADDR);
}

static inline void dma2d_ll_set_dma2d_bg_address_value(uint32_t value)
{
    REG_WRITE(DMA2D_DMA2D_BG_ADDRESS_ADDR,value);
}

static inline uint32_t dma2d_ll_get_dma2d_bg_address_bg_address(void)
{
    return REG_READ(DMA2D_DMA2D_BG_ADDRESS_ADDR);
}

static inline void dma2d_ll_set_dma2d_bg_address_bg_address(uint32_t value)
{
    REG_WRITE(DMA2D_DMA2D_BG_ADDRESS_ADDR,value);
}

/* REG_0x0a */
#define DMA2D_DMA2D_BG_OFFSET_ADDR  (DMA2D_LL_REG_BASE  + 0xa*4)
#define DMA2D_DMA2D_BG_OFFSET_BG_LINE_OFFSET_POS (0)
#define DMA2D_DMA2D_BG_OFFSET_BG_LINE_OFFSET_MASK (0xFFFF)

#define DMA2D_DMA2D_BG_OFFSET_RESERVED_POS (16)
#define DMA2D_DMA2D_BG_OFFSET_RESERVED_MASK (0xFFFF)

static inline uint32_t dma2d_ll_get_dma2d_bg_offset_value(void)
{
    return REG_READ(DMA2D_DMA2D_BG_OFFSET_ADDR);
}

static inline void dma2d_ll_set_dma2d_bg_offset_value(uint32_t value)
{
    REG_WRITE(DMA2D_DMA2D_BG_OFFSET_ADDR,value);
}

static inline uint32_t dma2d_ll_get_dma2d_bg_offset_bg_line_offset(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_OFFSET_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_BG_OFFSET_BG_LINE_OFFSET_POS) & DMA2D_DMA2D_BG_OFFSET_BG_LINE_OFFSET_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_bg_offset_bg_line_offset(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_OFFSET_ADDR);
    reg_value &= ~(DMA2D_DMA2D_BG_OFFSET_BG_LINE_OFFSET_MASK << DMA2D_DMA2D_BG_OFFSET_BG_LINE_OFFSET_POS);
    reg_value |= ((value & DMA2D_DMA2D_BG_OFFSET_BG_LINE_OFFSET_MASK) << DMA2D_DMA2D_BG_OFFSET_BG_LINE_OFFSET_POS);
    REG_WRITE(DMA2D_DMA2D_BG_OFFSET_ADDR,reg_value);
}

/* REG_0x0b */
#define DMA2D_DMA2D_FG_PFC_CTRL_ADDR  (DMA2D_LL_REG_BASE  + 0xb*4)
#define DMA2D_DMA2D_FG_PFC_CTRL_FG_COLOR_MODE_POS (0)
#define DMA2D_DMA2D_FG_PFC_CTRL_FG_COLOR_MODE_MASK (0xF)

#define DMA2D_DMA2D_FG_PFC_CTRL_FG_CLUT_COLOR_MODE_POS (4)
#define DMA2D_DMA2D_FG_PFC_CTRL_FG_CLUT_COLOR_MODE_MASK (0x1)

#define DMA2D_DMA2D_FG_PFC_CTRL_FG_START_CLUT_POS (5)
#define DMA2D_DMA2D_FG_PFC_CTRL_FG_START_CLUT_MASK (0x1)

#define DMA2D_DMA2D_FG_PFC_CTRL_RESERVED2_POS (6)
#define DMA2D_DMA2D_FG_PFC_CTRL_RESERVED2_MASK (0x3)

#define DMA2D_DMA2D_FG_PFC_CTRL_FG_CLUT_SIZE_POS (8)
#define DMA2D_DMA2D_FG_PFC_CTRL_FG_CLUT_SIZE_MASK (0xFF)

#define DMA2D_DMA2D_FG_PFC_CTRL_FG_ALPHA_MODE_POS (16)
#define DMA2D_DMA2D_FG_PFC_CTRL_FG_ALPHA_MODE_MASK (0x3)

#define DMA2D_DMA2D_FG_PFC_CTRL_RESERVED1_POS (18)
#define DMA2D_DMA2D_FG_PFC_CTRL_RESERVED1_MASK (0x3)

#define DMA2D_DMA2D_FG_PFC_CTRL_ALPHA_INVERT_POS (20)
#define DMA2D_DMA2D_FG_PFC_CTRL_ALPHA_INVERT_MASK (0x1)

#define DMA2D_DMA2D_FG_PFC_CTRL_FG_RB_SWAP_POS (21)
#define DMA2D_DMA2D_FG_PFC_CTRL_FG_RB_SWAP_MASK (0x1)

#define DMA2D_DMA2D_FG_PFC_CTRL_RESERVED_POS (22)
#define DMA2D_DMA2D_FG_PFC_CTRL_RESERVED_MASK (0x3)

#define DMA2D_DMA2D_FG_PFC_CTRL_FG_ALPHA_POS (24)
#define DMA2D_DMA2D_FG_PFC_CTRL_FG_ALPHA_MASK (0xFF)

static inline uint32_t dma2d_ll_get_dma2d_fg_pfc_ctrl_value(void)
{
    return REG_READ(DMA2D_DMA2D_FG_PFC_CTRL_ADDR);
}

static inline void dma2d_ll_set_dma2d_fg_pfc_ctrl_value(uint32_t value)
{
    REG_WRITE(DMA2D_DMA2D_FG_PFC_CTRL_ADDR,value);
}

static inline uint32_t dma2d_ll_get_dma2d_fg_pfc_ctrl_fg_color_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_PFC_CTRL_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_FG_PFC_CTRL_FG_COLOR_MODE_POS) & DMA2D_DMA2D_FG_PFC_CTRL_FG_COLOR_MODE_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_color_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_PFC_CTRL_ADDR);
    reg_value &= ~(DMA2D_DMA2D_FG_PFC_CTRL_FG_COLOR_MODE_MASK << DMA2D_DMA2D_FG_PFC_CTRL_FG_COLOR_MODE_POS);
    reg_value |= ((value & DMA2D_DMA2D_FG_PFC_CTRL_FG_COLOR_MODE_MASK) << DMA2D_DMA2D_FG_PFC_CTRL_FG_COLOR_MODE_POS);
    REG_WRITE(DMA2D_DMA2D_FG_PFC_CTRL_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_fg_pfc_ctrl_fg_clut_color_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_PFC_CTRL_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_FG_PFC_CTRL_FG_CLUT_COLOR_MODE_POS) & DMA2D_DMA2D_FG_PFC_CTRL_FG_CLUT_COLOR_MODE_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_clut_color_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_PFC_CTRL_ADDR);
    reg_value &= ~(DMA2D_DMA2D_FG_PFC_CTRL_FG_CLUT_COLOR_MODE_MASK << DMA2D_DMA2D_FG_PFC_CTRL_FG_CLUT_COLOR_MODE_POS);
    reg_value |= ((value & DMA2D_DMA2D_FG_PFC_CTRL_FG_CLUT_COLOR_MODE_MASK) << DMA2D_DMA2D_FG_PFC_CTRL_FG_CLUT_COLOR_MODE_POS);
    REG_WRITE(DMA2D_DMA2D_FG_PFC_CTRL_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_fg_pfc_ctrl_fg_start_clut(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_PFC_CTRL_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_FG_PFC_CTRL_FG_START_CLUT_POS) & DMA2D_DMA2D_FG_PFC_CTRL_FG_START_CLUT_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_start_clut(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_PFC_CTRL_ADDR);
    reg_value &= ~(DMA2D_DMA2D_FG_PFC_CTRL_FG_START_CLUT_MASK << DMA2D_DMA2D_FG_PFC_CTRL_FG_START_CLUT_POS);
    reg_value |= ((value & DMA2D_DMA2D_FG_PFC_CTRL_FG_START_CLUT_MASK) << DMA2D_DMA2D_FG_PFC_CTRL_FG_START_CLUT_POS);
    REG_WRITE(DMA2D_DMA2D_FG_PFC_CTRL_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_fg_pfc_ctrl_fg_clut_size(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_PFC_CTRL_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_FG_PFC_CTRL_FG_CLUT_SIZE_POS) & DMA2D_DMA2D_FG_PFC_CTRL_FG_CLUT_SIZE_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_clut_size(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_PFC_CTRL_ADDR);
    reg_value &= ~(DMA2D_DMA2D_FG_PFC_CTRL_FG_CLUT_SIZE_MASK << DMA2D_DMA2D_FG_PFC_CTRL_FG_CLUT_SIZE_POS);
    reg_value |= ((value & DMA2D_DMA2D_FG_PFC_CTRL_FG_CLUT_SIZE_MASK) << DMA2D_DMA2D_FG_PFC_CTRL_FG_CLUT_SIZE_POS);
    REG_WRITE(DMA2D_DMA2D_FG_PFC_CTRL_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_fg_pfc_ctrl_fg_alpha_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_PFC_CTRL_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_FG_PFC_CTRL_FG_ALPHA_MODE_POS) & DMA2D_DMA2D_FG_PFC_CTRL_FG_ALPHA_MODE_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_alpha_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_PFC_CTRL_ADDR);
    reg_value &= ~(DMA2D_DMA2D_FG_PFC_CTRL_FG_ALPHA_MODE_MASK << DMA2D_DMA2D_FG_PFC_CTRL_FG_ALPHA_MODE_POS);
    reg_value |= ((value & DMA2D_DMA2D_FG_PFC_CTRL_FG_ALPHA_MODE_MASK) << DMA2D_DMA2D_FG_PFC_CTRL_FG_ALPHA_MODE_POS);
    REG_WRITE(DMA2D_DMA2D_FG_PFC_CTRL_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_fg_pfc_ctrl_alpha_invert(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_PFC_CTRL_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_FG_PFC_CTRL_ALPHA_INVERT_POS) & DMA2D_DMA2D_FG_PFC_CTRL_ALPHA_INVERT_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_fg_pfc_ctrl_alpha_invert(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_PFC_CTRL_ADDR);
    reg_value &= ~(DMA2D_DMA2D_FG_PFC_CTRL_ALPHA_INVERT_MASK << DMA2D_DMA2D_FG_PFC_CTRL_ALPHA_INVERT_POS);
    reg_value |= ((value & DMA2D_DMA2D_FG_PFC_CTRL_ALPHA_INVERT_MASK) << DMA2D_DMA2D_FG_PFC_CTRL_ALPHA_INVERT_POS);
    REG_WRITE(DMA2D_DMA2D_FG_PFC_CTRL_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_fg_pfc_ctrl_fg_rb_swap(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_PFC_CTRL_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_FG_PFC_CTRL_FG_RB_SWAP_POS) & DMA2D_DMA2D_FG_PFC_CTRL_FG_RB_SWAP_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_rb_swap(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_PFC_CTRL_ADDR);
    reg_value &= ~(DMA2D_DMA2D_FG_PFC_CTRL_FG_RB_SWAP_MASK << DMA2D_DMA2D_FG_PFC_CTRL_FG_RB_SWAP_POS);
    reg_value |= ((value & DMA2D_DMA2D_FG_PFC_CTRL_FG_RB_SWAP_MASK) << DMA2D_DMA2D_FG_PFC_CTRL_FG_RB_SWAP_POS);
    REG_WRITE(DMA2D_DMA2D_FG_PFC_CTRL_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_fg_pfc_ctrl_fg_alpha(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_PFC_CTRL_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_FG_PFC_CTRL_FG_ALPHA_POS) & DMA2D_DMA2D_FG_PFC_CTRL_FG_ALPHA_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_alpha(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_PFC_CTRL_ADDR);
    reg_value &= ~(DMA2D_DMA2D_FG_PFC_CTRL_FG_ALPHA_MASK << DMA2D_DMA2D_FG_PFC_CTRL_FG_ALPHA_POS);
    reg_value |= ((value & DMA2D_DMA2D_FG_PFC_CTRL_FG_ALPHA_MASK) << DMA2D_DMA2D_FG_PFC_CTRL_FG_ALPHA_POS);
    REG_WRITE(DMA2D_DMA2D_FG_PFC_CTRL_ADDR,reg_value);
}

/* REG_0x0c */
#define DMA2D_DMA2D_FG_COLOR_REG_ADDR  (DMA2D_LL_REG_BASE  + 0xc*4)
#define DMA2D_DMA2D_FG_COLOR_REG_BLUE_VALUE_POS (0)
#define DMA2D_DMA2D_FG_COLOR_REG_BLUE_VALUE_MASK (0xFF)

#define DMA2D_DMA2D_FG_COLOR_REG_GREEN_VALUE_POS (8)
#define DMA2D_DMA2D_FG_COLOR_REG_GREEN_VALUE_MASK (0xFF)

#define DMA2D_DMA2D_FG_COLOR_REG_RED_VALUE_POS (16)
#define DMA2D_DMA2D_FG_COLOR_REG_RED_VALUE_MASK (0xFF)

#define DMA2D_DMA2D_FG_COLOR_REG_RESERVED_POS (24)
#define DMA2D_DMA2D_FG_COLOR_REG_RESERVED_MASK (0xFF)

static inline uint32_t dma2d_ll_get_dma2d_fg_color_reg_value(void)
{
    return REG_READ(DMA2D_DMA2D_FG_COLOR_REG_ADDR);
}

static inline void dma2d_ll_set_dma2d_fg_color_reg_value(uint32_t value)
{
    REG_WRITE(DMA2D_DMA2D_FG_COLOR_REG_ADDR,value);
}

static inline uint32_t dma2d_ll_get_dma2d_fg_color_reg_blue_value(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_COLOR_REG_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_FG_COLOR_REG_BLUE_VALUE_POS) & DMA2D_DMA2D_FG_COLOR_REG_BLUE_VALUE_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_fg_color_reg_blue_value(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_COLOR_REG_ADDR);
    reg_value &= ~(DMA2D_DMA2D_FG_COLOR_REG_BLUE_VALUE_MASK << DMA2D_DMA2D_FG_COLOR_REG_BLUE_VALUE_POS);
    reg_value |= ((value & DMA2D_DMA2D_FG_COLOR_REG_BLUE_VALUE_MASK) << DMA2D_DMA2D_FG_COLOR_REG_BLUE_VALUE_POS);
    REG_WRITE(DMA2D_DMA2D_FG_COLOR_REG_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_fg_color_reg_green_value(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_COLOR_REG_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_FG_COLOR_REG_GREEN_VALUE_POS) & DMA2D_DMA2D_FG_COLOR_REG_GREEN_VALUE_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_fg_color_reg_green_value(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_COLOR_REG_ADDR);
    reg_value &= ~(DMA2D_DMA2D_FG_COLOR_REG_GREEN_VALUE_MASK << DMA2D_DMA2D_FG_COLOR_REG_GREEN_VALUE_POS);
    reg_value |= ((value & DMA2D_DMA2D_FG_COLOR_REG_GREEN_VALUE_MASK) << DMA2D_DMA2D_FG_COLOR_REG_GREEN_VALUE_POS);
    REG_WRITE(DMA2D_DMA2D_FG_COLOR_REG_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_fg_color_reg_red_value(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_COLOR_REG_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_FG_COLOR_REG_RED_VALUE_POS) & DMA2D_DMA2D_FG_COLOR_REG_RED_VALUE_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_fg_color_reg_red_value(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_FG_COLOR_REG_ADDR);
    reg_value &= ~(DMA2D_DMA2D_FG_COLOR_REG_RED_VALUE_MASK << DMA2D_DMA2D_FG_COLOR_REG_RED_VALUE_POS);
    reg_value |= ((value & DMA2D_DMA2D_FG_COLOR_REG_RED_VALUE_MASK) << DMA2D_DMA2D_FG_COLOR_REG_RED_VALUE_POS);
    REG_WRITE(DMA2D_DMA2D_FG_COLOR_REG_ADDR,reg_value);
}

/* REG_0x0d */
#define DMA2D_DMA2D_BG_PFC_CTRL_ADDR  (DMA2D_LL_REG_BASE  + 0xd*4)
#define DMA2D_DMA2D_BG_PFC_CTRL_BG_COLOR_MODE_POS (0)
#define DMA2D_DMA2D_BG_PFC_CTRL_BG_COLOR_MODE_MASK (0xF)

#define DMA2D_DMA2D_BG_PFC_CTRL_BG_CLUT_COLOR_MODE_POS (4)
#define DMA2D_DMA2D_BG_PFC_CTRL_BG_CLUT_COLOR_MODE_MASK (0x1)

#define DMA2D_DMA2D_BG_PFC_CTRL_BG_START_CLUT_POS (5)
#define DMA2D_DMA2D_BG_PFC_CTRL_BG_START_CLUT_MASK (0x1)

#define DMA2D_DMA2D_BG_PFC_CTRL_RESERVED2_POS (6)
#define DMA2D_DMA2D_BG_PFC_CTRL_RESERVED2_MASK (0x3)

#define DMA2D_DMA2D_BG_PFC_CTRL_BG_CLUT_SIZE_POS (8)
#define DMA2D_DMA2D_BG_PFC_CTRL_BG_CLUT_SIZE_MASK (0xFF)

#define DMA2D_DMA2D_BG_PFC_CTRL_BG_ALPHA_MODE_POS (16)
#define DMA2D_DMA2D_BG_PFC_CTRL_BG_ALPHA_MODE_MASK (0x3)

#define DMA2D_DMA2D_BG_PFC_CTRL_RESERVED1_POS (18)
#define DMA2D_DMA2D_BG_PFC_CTRL_RESERVED1_MASK (0x3)

#define DMA2D_DMA2D_BG_PFC_CTRL_ALPHA_INVERT_POS (20)
#define DMA2D_DMA2D_BG_PFC_CTRL_ALPHA_INVERT_MASK (0x1)

#define DMA2D_DMA2D_BG_PFC_CTRL_BG_RB_SWAP_POS (21)
#define DMA2D_DMA2D_BG_PFC_CTRL_BG_RB_SWAP_MASK (0x1)

#define DMA2D_DMA2D_BG_PFC_CTRL_RESERVED_POS (22)
#define DMA2D_DMA2D_BG_PFC_CTRL_RESERVED_MASK (0x3)

#define DMA2D_DMA2D_BG_PFC_CTRL_BG_ALPHA_POS (24)
#define DMA2D_DMA2D_BG_PFC_CTRL_BG_ALPHA_MASK (0xFF)

static inline uint32_t dma2d_ll_get_dma2d_bg_pfc_ctrl_value(void)
{
    return REG_READ(DMA2D_DMA2D_BG_PFC_CTRL_ADDR);
}

static inline void dma2d_ll_set_dma2d_bg_pfc_ctrl_value(uint32_t value)
{
    REG_WRITE(DMA2D_DMA2D_BG_PFC_CTRL_ADDR,value);
}

static inline uint32_t dma2d_ll_get_dma2d_bg_pfc_ctrl_bg_color_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_PFC_CTRL_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_BG_PFC_CTRL_BG_COLOR_MODE_POS) & DMA2D_DMA2D_BG_PFC_CTRL_BG_COLOR_MODE_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_color_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_PFC_CTRL_ADDR);
    reg_value &= ~(DMA2D_DMA2D_BG_PFC_CTRL_BG_COLOR_MODE_MASK << DMA2D_DMA2D_BG_PFC_CTRL_BG_COLOR_MODE_POS);
    reg_value |= ((value & DMA2D_DMA2D_BG_PFC_CTRL_BG_COLOR_MODE_MASK) << DMA2D_DMA2D_BG_PFC_CTRL_BG_COLOR_MODE_POS);
    REG_WRITE(DMA2D_DMA2D_BG_PFC_CTRL_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_bg_pfc_ctrl_bg_clut_color_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_PFC_CTRL_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_BG_PFC_CTRL_BG_CLUT_COLOR_MODE_POS) & DMA2D_DMA2D_BG_PFC_CTRL_BG_CLUT_COLOR_MODE_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_clut_color_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_PFC_CTRL_ADDR);
    reg_value &= ~(DMA2D_DMA2D_BG_PFC_CTRL_BG_CLUT_COLOR_MODE_MASK << DMA2D_DMA2D_BG_PFC_CTRL_BG_CLUT_COLOR_MODE_POS);
    reg_value |= ((value & DMA2D_DMA2D_BG_PFC_CTRL_BG_CLUT_COLOR_MODE_MASK) << DMA2D_DMA2D_BG_PFC_CTRL_BG_CLUT_COLOR_MODE_POS);
    REG_WRITE(DMA2D_DMA2D_BG_PFC_CTRL_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_bg_pfc_ctrl_bg_start_clut(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_PFC_CTRL_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_BG_PFC_CTRL_BG_START_CLUT_POS) & DMA2D_DMA2D_BG_PFC_CTRL_BG_START_CLUT_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_start_clut(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_PFC_CTRL_ADDR);
    reg_value &= ~(DMA2D_DMA2D_BG_PFC_CTRL_BG_START_CLUT_MASK << DMA2D_DMA2D_BG_PFC_CTRL_BG_START_CLUT_POS);
    reg_value |= ((value & DMA2D_DMA2D_BG_PFC_CTRL_BG_START_CLUT_MASK) << DMA2D_DMA2D_BG_PFC_CTRL_BG_START_CLUT_POS);
    REG_WRITE(DMA2D_DMA2D_BG_PFC_CTRL_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_bg_pfc_ctrl_bg_clut_size(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_PFC_CTRL_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_BG_PFC_CTRL_BG_CLUT_SIZE_POS) & DMA2D_DMA2D_BG_PFC_CTRL_BG_CLUT_SIZE_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_clut_size(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_PFC_CTRL_ADDR);
    reg_value &= ~(DMA2D_DMA2D_BG_PFC_CTRL_BG_CLUT_SIZE_MASK << DMA2D_DMA2D_BG_PFC_CTRL_BG_CLUT_SIZE_POS);
    reg_value |= ((value & DMA2D_DMA2D_BG_PFC_CTRL_BG_CLUT_SIZE_MASK) << DMA2D_DMA2D_BG_PFC_CTRL_BG_CLUT_SIZE_POS);
    REG_WRITE(DMA2D_DMA2D_BG_PFC_CTRL_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_bg_pfc_ctrl_bg_alpha_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_PFC_CTRL_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_BG_PFC_CTRL_BG_ALPHA_MODE_POS) & DMA2D_DMA2D_BG_PFC_CTRL_BG_ALPHA_MODE_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_alpha_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_PFC_CTRL_ADDR);
    reg_value &= ~(DMA2D_DMA2D_BG_PFC_CTRL_BG_ALPHA_MODE_MASK << DMA2D_DMA2D_BG_PFC_CTRL_BG_ALPHA_MODE_POS);
    reg_value |= ((value & DMA2D_DMA2D_BG_PFC_CTRL_BG_ALPHA_MODE_MASK) << DMA2D_DMA2D_BG_PFC_CTRL_BG_ALPHA_MODE_POS);
    REG_WRITE(DMA2D_DMA2D_BG_PFC_CTRL_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_bg_pfc_ctrl_alpha_invert(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_PFC_CTRL_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_BG_PFC_CTRL_ALPHA_INVERT_POS) & DMA2D_DMA2D_BG_PFC_CTRL_ALPHA_INVERT_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_bg_pfc_ctrl_alpha_invert(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_PFC_CTRL_ADDR);
    reg_value &= ~(DMA2D_DMA2D_BG_PFC_CTRL_ALPHA_INVERT_MASK << DMA2D_DMA2D_BG_PFC_CTRL_ALPHA_INVERT_POS);
    reg_value |= ((value & DMA2D_DMA2D_BG_PFC_CTRL_ALPHA_INVERT_MASK) << DMA2D_DMA2D_BG_PFC_CTRL_ALPHA_INVERT_POS);
    REG_WRITE(DMA2D_DMA2D_BG_PFC_CTRL_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_bg_pfc_ctrl_bg_rb_swap(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_PFC_CTRL_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_BG_PFC_CTRL_BG_RB_SWAP_POS) & DMA2D_DMA2D_BG_PFC_CTRL_BG_RB_SWAP_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_rb_swap(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_PFC_CTRL_ADDR);
    reg_value &= ~(DMA2D_DMA2D_BG_PFC_CTRL_BG_RB_SWAP_MASK << DMA2D_DMA2D_BG_PFC_CTRL_BG_RB_SWAP_POS);
    reg_value |= ((value & DMA2D_DMA2D_BG_PFC_CTRL_BG_RB_SWAP_MASK) << DMA2D_DMA2D_BG_PFC_CTRL_BG_RB_SWAP_POS);
    REG_WRITE(DMA2D_DMA2D_BG_PFC_CTRL_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_bg_pfc_ctrl_bg_alpha(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_PFC_CTRL_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_BG_PFC_CTRL_BG_ALPHA_POS) & DMA2D_DMA2D_BG_PFC_CTRL_BG_ALPHA_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_alpha(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_PFC_CTRL_ADDR);
    reg_value &= ~(DMA2D_DMA2D_BG_PFC_CTRL_BG_ALPHA_MASK << DMA2D_DMA2D_BG_PFC_CTRL_BG_ALPHA_POS);
    reg_value |= ((value & DMA2D_DMA2D_BG_PFC_CTRL_BG_ALPHA_MASK) << DMA2D_DMA2D_BG_PFC_CTRL_BG_ALPHA_POS);
    REG_WRITE(DMA2D_DMA2D_BG_PFC_CTRL_ADDR,reg_value);
}

/* REG_0x0e */
#define DMA2D_DMA2D_BG_COLOR_REG_ADDR  (DMA2D_LL_REG_BASE  + 0xe*4)
#define DMA2D_DMA2D_BG_COLOR_REG_BLUE_VALUE_POS (0)
#define DMA2D_DMA2D_BG_COLOR_REG_BLUE_VALUE_MASK (0xFF)

#define DMA2D_DMA2D_BG_COLOR_REG_GREEN_VALUE_POS (8)
#define DMA2D_DMA2D_BG_COLOR_REG_GREEN_VALUE_MASK (0xFF)

#define DMA2D_DMA2D_BG_COLOR_REG_RED_VALUE_POS (16)
#define DMA2D_DMA2D_BG_COLOR_REG_RED_VALUE_MASK (0xFF)

#define DMA2D_DMA2D_BG_COLOR_REG_RESERVED_POS (24)
#define DMA2D_DMA2D_BG_COLOR_REG_RESERVED_MASK (0xFF)

static inline uint32_t dma2d_ll_get_dma2d_bg_color_reg_value(void)
{
    return REG_READ(DMA2D_DMA2D_BG_COLOR_REG_ADDR);
}

static inline void dma2d_ll_set_dma2d_bg_color_reg_value(uint32_t value)
{
    REG_WRITE(DMA2D_DMA2D_BG_COLOR_REG_ADDR,value);
}

static inline uint32_t dma2d_ll_get_dma2d_bg_color_reg_blue_value(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_COLOR_REG_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_BG_COLOR_REG_BLUE_VALUE_POS) & DMA2D_DMA2D_BG_COLOR_REG_BLUE_VALUE_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_bg_color_reg_blue_value(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_COLOR_REG_ADDR);
    reg_value &= ~(DMA2D_DMA2D_BG_COLOR_REG_BLUE_VALUE_MASK << DMA2D_DMA2D_BG_COLOR_REG_BLUE_VALUE_POS);
    reg_value |= ((value & DMA2D_DMA2D_BG_COLOR_REG_BLUE_VALUE_MASK) << DMA2D_DMA2D_BG_COLOR_REG_BLUE_VALUE_POS);
    REG_WRITE(DMA2D_DMA2D_BG_COLOR_REG_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_bg_color_reg_green_value(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_COLOR_REG_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_BG_COLOR_REG_GREEN_VALUE_POS) & DMA2D_DMA2D_BG_COLOR_REG_GREEN_VALUE_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_bg_color_reg_green_value(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_COLOR_REG_ADDR);
    reg_value &= ~(DMA2D_DMA2D_BG_COLOR_REG_GREEN_VALUE_MASK << DMA2D_DMA2D_BG_COLOR_REG_GREEN_VALUE_POS);
    reg_value |= ((value & DMA2D_DMA2D_BG_COLOR_REG_GREEN_VALUE_MASK) << DMA2D_DMA2D_BG_COLOR_REG_GREEN_VALUE_POS);
    REG_WRITE(DMA2D_DMA2D_BG_COLOR_REG_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_bg_color_reg_red_value(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_COLOR_REG_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_BG_COLOR_REG_RED_VALUE_POS) & DMA2D_DMA2D_BG_COLOR_REG_RED_VALUE_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_bg_color_reg_red_value(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_BG_COLOR_REG_ADDR);
    reg_value &= ~(DMA2D_DMA2D_BG_COLOR_REG_RED_VALUE_MASK << DMA2D_DMA2D_BG_COLOR_REG_RED_VALUE_POS);
    reg_value |= ((value & DMA2D_DMA2D_BG_COLOR_REG_RED_VALUE_MASK) << DMA2D_DMA2D_BG_COLOR_REG_RED_VALUE_POS);
    REG_WRITE(DMA2D_DMA2D_BG_COLOR_REG_ADDR,reg_value);
}

/* REG_0x0f */
#define DMA2D_FG_CLUT_MEM_ADDRESS_ADDR  (DMA2D_LL_REG_BASE  + 0xf*4)
#define DMA2D_FG_CLUT_MEM_ADDRESS_FG_CLUT_ADDRESS_POS (0)
#define DMA2D_FG_CLUT_MEM_ADDRESS_FG_CLUT_ADDRESS_MASK (0xFFFFFFFF)

static inline uint32_t dma2d_ll_get_fg_clut_mem_address_value(void)
{
    return REG_READ(DMA2D_FG_CLUT_MEM_ADDRESS_ADDR);
}

static inline void dma2d_ll_set_fg_clut_mem_address_value(uint32_t value)
{
    REG_WRITE(DMA2D_FG_CLUT_MEM_ADDRESS_ADDR,value);
}

static inline uint32_t dma2d_ll_get_fg_clut_mem_address_fg_clut_address(void)
{
    return REG_READ(DMA2D_FG_CLUT_MEM_ADDRESS_ADDR);
}

static inline void dma2d_ll_set_fg_clut_mem_address_fg_clut_address(uint32_t value)
{
    REG_WRITE(DMA2D_FG_CLUT_MEM_ADDRESS_ADDR,value);
}

/* REG_0x10 */
#define DMA2D_BG_CLUT_MEM_ADDRESS_ADDR  (DMA2D_LL_REG_BASE  + 0x10*4)
#define DMA2D_BG_CLUT_MEM_ADDRESS_BG_CLUT_ADDRESS_POS (0)
#define DMA2D_BG_CLUT_MEM_ADDRESS_BG_CLUT_ADDRESS_MASK (0xFFFFFFFF)

static inline uint32_t dma2d_ll_get_bg_clut_mem_address_value(void)
{
    return REG_READ(DMA2D_BG_CLUT_MEM_ADDRESS_ADDR);
}

static inline void dma2d_ll_set_bg_clut_mem_address_value(uint32_t value)
{
    REG_WRITE(DMA2D_BG_CLUT_MEM_ADDRESS_ADDR,value);
}

static inline uint32_t dma2d_ll_get_bg_clut_mem_address_bg_clut_address(void)
{
    return REG_READ(DMA2D_BG_CLUT_MEM_ADDRESS_ADDR);
}

static inline void dma2d_ll_set_bg_clut_mem_address_bg_clut_address(uint32_t value)
{
    REG_WRITE(DMA2D_BG_CLUT_MEM_ADDRESS_ADDR,value);
}

/* REG_0x11 */
#define DMA2D_OUT_PFC_CONTRL_ADDR  (DMA2D_LL_REG_BASE  + 0x11*4)
#define DMA2D_OUT_PFC_CONTRL_OUT_COLOR_MODE_POS (0)
#define DMA2D_OUT_PFC_CONTRL_OUT_COLOR_MODE_MASK (0x7)

#define DMA2D_OUT_PFC_CONTRL_RESERVED3_POS (3)
#define DMA2D_OUT_PFC_CONTRL_RESERVED3_MASK (0x3F)

//#define DMA2D_OUT_PFC_CONTRL_OUT_SWAP_BYTES_POS (9)
//#define DMA2D_OUT_PFC_CONTRL_OUT_SWAP_BYTES_MASK (0x1)

#define DMA2D_OUT_PFC_CONTRL_RESERVED2_POS (10)
#define DMA2D_OUT_PFC_CONTRL_RESERVED2_MASK (0x3FF)

#define DMA2D_OUT_PFC_CONTRL_OUT_ALPHA_INVERT_POS (20)
#define DMA2D_OUT_PFC_CONTRL_OUT_ALPHA_INVERT_MASK (0x1)

#define DMA2D_OUT_PFC_CONTRL_OUT_RB_SWAP_POS (21)
#define DMA2D_OUT_PFC_CONTRL_OUT_RB_SWAP_MASK (0x1)

#define DMA2D_OUT_PFC_CONTRL_RESERVED_POS (22)
#define DMA2D_OUT_PFC_CONTRL_RESERVED_MASK (0x3FF)

static inline uint32_t dma2d_ll_get_out_pfc_contrl_value(void)
{
    return REG_READ(DMA2D_OUT_PFC_CONTRL_ADDR);
}

static inline void dma2d_ll_set_out_pfc_contrl_value(uint32_t value)
{
    REG_WRITE(DMA2D_OUT_PFC_CONTRL_ADDR,value);
}

static inline uint32_t dma2d_ll_get_out_pfc_contrl_out_color_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_OUT_PFC_CONTRL_ADDR);
    reg_value = ((reg_value >> DMA2D_OUT_PFC_CONTRL_OUT_COLOR_MODE_POS) & DMA2D_OUT_PFC_CONTRL_OUT_COLOR_MODE_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_out_pfc_contrl_out_color_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_OUT_PFC_CONTRL_ADDR);
    reg_value &= ~(DMA2D_OUT_PFC_CONTRL_OUT_COLOR_MODE_MASK << DMA2D_OUT_PFC_CONTRL_OUT_COLOR_MODE_POS);
    reg_value |= ((value & DMA2D_OUT_PFC_CONTRL_OUT_COLOR_MODE_MASK) << DMA2D_OUT_PFC_CONTRL_OUT_COLOR_MODE_POS);
    REG_WRITE(DMA2D_OUT_PFC_CONTRL_ADDR,reg_value);
}

/* REG_0x11:out_pfc_contrl->out_swap_bytes:0xd[       9],0:bytes in regular order. 1:bytes swaped two by two in output fifo.,0,RW*/
/*static inline uint32_t dma2d_ll_get_out_pfc_contrl_out_swap_bytes(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_OUT_PFC_CONTRL_ADDR);
    reg_value = ((reg_value >> DMA2D_OUT_PFC_CONTRL_OUT_SWAP_BYTES_POS) & DMA2D_OUT_PFC_CONTRL_OUT_SWAP_BYTES_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_out_pfc_contrl_out_swap_bytes(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_OUT_PFC_CONTRL_ADDR);
    reg_value &= ~(DMA2D_OUT_PFC_CONTRL_OUT_SWAP_BYTES_MASK << DMA2D_OUT_PFC_CONTRL_OUT_SWAP_BYTES_POS);
    reg_value |= ((value & DMA2D_OUT_PFC_CONTRL_OUT_SWAP_BYTES_MASK) << DMA2D_OUT_PFC_CONTRL_OUT_SWAP_BYTES_POS);
    REG_WRITE(DMA2D_OUT_PFC_CONTRL_ADDR,reg_value);
}*/

static inline uint32_t dma2d_ll_get_out_pfc_contrl_out_alpha_invert(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_OUT_PFC_CONTRL_ADDR);
    reg_value = ((reg_value >> DMA2D_OUT_PFC_CONTRL_OUT_ALPHA_INVERT_POS) & DMA2D_OUT_PFC_CONTRL_OUT_ALPHA_INVERT_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_out_pfc_contrl_out_alpha_invert(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_OUT_PFC_CONTRL_ADDR);
    reg_value &= ~(DMA2D_OUT_PFC_CONTRL_OUT_ALPHA_INVERT_MASK << DMA2D_OUT_PFC_CONTRL_OUT_ALPHA_INVERT_POS);
    reg_value |= ((value & DMA2D_OUT_PFC_CONTRL_OUT_ALPHA_INVERT_MASK) << DMA2D_OUT_PFC_CONTRL_OUT_ALPHA_INVERT_POS);
    REG_WRITE(DMA2D_OUT_PFC_CONTRL_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_out_pfc_contrl_out_rb_swap(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_OUT_PFC_CONTRL_ADDR);
    reg_value = ((reg_value >> DMA2D_OUT_PFC_CONTRL_OUT_RB_SWAP_POS) & DMA2D_OUT_PFC_CONTRL_OUT_RB_SWAP_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_out_pfc_contrl_out_rb_swap(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_OUT_PFC_CONTRL_ADDR);
    reg_value &= ~(DMA2D_OUT_PFC_CONTRL_OUT_RB_SWAP_MASK << DMA2D_OUT_PFC_CONTRL_OUT_RB_SWAP_POS);
    reg_value |= ((value & DMA2D_OUT_PFC_CONTRL_OUT_RB_SWAP_MASK) << DMA2D_OUT_PFC_CONTRL_OUT_RB_SWAP_POS);
    REG_WRITE(DMA2D_OUT_PFC_CONTRL_ADDR,reg_value);
}

/* REG_0x12 */
#define DMA2D_OUT_COLOR_REG_ADDR  (DMA2D_LL_REG_BASE  + 0x12*4)
#define DMA2D_OUT_COLOR_REG_OUTPUT_CLOR_REG_POS (0)
#define DMA2D_OUT_COLOR_REG_OUTPUT_CLOR_REG_MASK (0xFFFFFFFF)

static inline uint32_t dma2d_ll_get_out_color_reg_value(void)
{
    return REG_READ(DMA2D_OUT_COLOR_REG_ADDR);
}

static inline void dma2d_ll_set_out_color_reg_value(uint32_t value)
{
    REG_WRITE(DMA2D_OUT_COLOR_REG_ADDR,value);
}

static inline uint32_t dma2d_ll_get_out_color_reg_output_clor_reg(void)
{
    return REG_READ(DMA2D_OUT_COLOR_REG_ADDR);
}

static inline void dma2d_ll_set_out_color_reg_output_clor_reg(uint32_t value)
{
    REG_WRITE(DMA2D_OUT_COLOR_REG_ADDR,value);
}

/* REG_0x13 */
#define DMA2D_DMA2D_OUT_MEM_ADDRESS_ADDR  (DMA2D_LL_REG_BASE  + 0x13*4)
#define DMA2D_DMA2D_OUT_MEM_ADDRESS_OUT_MEM_ADDRESS_POS (0)
#define DMA2D_DMA2D_OUT_MEM_ADDRESS_OUT_MEM_ADDRESS_MASK (0xFFFFFFFF)

static inline uint32_t dma2d_ll_get_dma2d_out_mem_address_value(void)
{
    return REG_READ(DMA2D_DMA2D_OUT_MEM_ADDRESS_ADDR);
}

static inline void dma2d_ll_set_dma2d_out_mem_address_value(uint32_t value)
{
    REG_WRITE(DMA2D_DMA2D_OUT_MEM_ADDRESS_ADDR,value);
}

static inline uint32_t dma2d_ll_get_dma2d_out_mem_address_out_mem_address(void)
{
    return REG_READ(DMA2D_DMA2D_OUT_MEM_ADDRESS_ADDR);
}

static inline void dma2d_ll_set_dma2d_out_mem_address_out_mem_address(uint32_t value)
{
    REG_WRITE(DMA2D_DMA2D_OUT_MEM_ADDRESS_ADDR,value);
}

/* REG_0x14 */
#define DMA2D_OUTPUT_OFFSET_ADDR  (DMA2D_LL_REG_BASE  + 0x14*4)
#define DMA2D_OUTPUT_OFFSET_OUT_LINE_OFFSET_POS (0)
#define DMA2D_OUTPUT_OFFSET_OUT_LINE_OFFSET_MASK (0xFFFF)

#define DMA2D_OUTPUT_OFFSET_RESERVED_POS (16)
#define DMA2D_OUTPUT_OFFSET_RESERVED_MASK (0xFFFF)

static inline uint32_t dma2d_ll_get_output_offset_value(void)
{
    return REG_READ(DMA2D_OUTPUT_OFFSET_ADDR);
}

static inline void dma2d_ll_set_output_offset_value(uint32_t value)
{
    REG_WRITE(DMA2D_OUTPUT_OFFSET_ADDR,value);
}

static inline uint32_t dma2d_ll_get_output_offset_out_line_offset(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_OUTPUT_OFFSET_ADDR);
    reg_value = ((reg_value >> DMA2D_OUTPUT_OFFSET_OUT_LINE_OFFSET_POS) & DMA2D_OUTPUT_OFFSET_OUT_LINE_OFFSET_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_output_offset_out_line_offset(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_OUTPUT_OFFSET_ADDR);
    reg_value &= ~(DMA2D_OUTPUT_OFFSET_OUT_LINE_OFFSET_MASK << DMA2D_OUTPUT_OFFSET_OUT_LINE_OFFSET_POS);
    reg_value |= ((value & DMA2D_OUTPUT_OFFSET_OUT_LINE_OFFSET_MASK) << DMA2D_OUTPUT_OFFSET_OUT_LINE_OFFSET_POS);
    REG_WRITE(DMA2D_OUTPUT_OFFSET_ADDR,reg_value);
}

/* REG_0x15 */
#define DMA2D_DMA2D_NUMBER_OF_LINE_ADDR  (DMA2D_LL_REG_BASE  + 0x15*4)
#define DMA2D_DMA2D_NUMBER_OF_LINE_NUMBER_LINE_POS (0)
#define DMA2D_DMA2D_NUMBER_OF_LINE_NUMBER_LINE_MASK (0xFFFF)

#define DMA2D_DMA2D_NUMBER_OF_LINE_PEXEL_LINE_POS (16)
#define DMA2D_DMA2D_NUMBER_OF_LINE_PEXEL_LINE_MASK (0x3FFF)

#define DMA2D_DMA2D_NUMBER_OF_LINE_RESERVED_POS (30)
#define DMA2D_DMA2D_NUMBER_OF_LINE_RESERVED_MASK (0x3)

static inline uint32_t dma2d_ll_get_dma2d_number_of_line_value(void)
{
    return REG_READ(DMA2D_DMA2D_NUMBER_OF_LINE_ADDR);
}

static inline void dma2d_ll_set_dma2d_number_of_line_value(uint32_t value)
{
    REG_WRITE(DMA2D_DMA2D_NUMBER_OF_LINE_ADDR,value);
}

static inline uint32_t dma2d_ll_get_dma2d_number_of_line_number_line(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_NUMBER_OF_LINE_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_NUMBER_OF_LINE_NUMBER_LINE_POS) & DMA2D_DMA2D_NUMBER_OF_LINE_NUMBER_LINE_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_number_of_line_number_line(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_NUMBER_OF_LINE_ADDR);
    reg_value &= ~(DMA2D_DMA2D_NUMBER_OF_LINE_NUMBER_LINE_MASK << DMA2D_DMA2D_NUMBER_OF_LINE_NUMBER_LINE_POS);
    reg_value |= ((value & DMA2D_DMA2D_NUMBER_OF_LINE_NUMBER_LINE_MASK) << DMA2D_DMA2D_NUMBER_OF_LINE_NUMBER_LINE_POS);
    REG_WRITE(DMA2D_DMA2D_NUMBER_OF_LINE_ADDR,reg_value);
}

static inline uint32_t dma2d_ll_get_dma2d_number_of_line_pexel_line(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_NUMBER_OF_LINE_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_NUMBER_OF_LINE_PEXEL_LINE_POS) & DMA2D_DMA2D_NUMBER_OF_LINE_PEXEL_LINE_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_number_of_line_pexel_line(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_NUMBER_OF_LINE_ADDR);
    reg_value &= ~(DMA2D_DMA2D_NUMBER_OF_LINE_PEXEL_LINE_MASK << DMA2D_DMA2D_NUMBER_OF_LINE_PEXEL_LINE_POS);
    reg_value |= ((value & DMA2D_DMA2D_NUMBER_OF_LINE_PEXEL_LINE_MASK) << DMA2D_DMA2D_NUMBER_OF_LINE_PEXEL_LINE_POS);
    REG_WRITE(DMA2D_DMA2D_NUMBER_OF_LINE_ADDR,reg_value);
}

/* REG_0x16 */
#define DMA2D_DMA2D_LINE_WATERMARK_ADDR  (DMA2D_LL_REG_BASE  + 0x16*4)
#define DMA2D_DMA2D_LINE_WATERMARK_LINE_WATERMARK_POS (0)
#define DMA2D_DMA2D_LINE_WATERMARK_LINE_WATERMARK_MASK (0xFFFF)

#define DMA2D_DMA2D_LINE_WATERMARK_RESERVED_POS (16)
#define DMA2D_DMA2D_LINE_WATERMARK_RESERVED_MASK (0xFFFF)

static inline uint32_t dma2d_ll_get_dma2d_line_watermark_value(void)
{
    return REG_READ(DMA2D_DMA2D_LINE_WATERMARK_ADDR);
}

static inline void dma2d_ll_set_dma2d_line_watermark_value(uint32_t value)
{
    REG_WRITE(DMA2D_DMA2D_LINE_WATERMARK_ADDR,value);
}

static inline uint32_t dma2d_ll_get_dma2d_line_watermark_line_watermark(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_LINE_WATERMARK_ADDR);
    reg_value = ((reg_value >> DMA2D_DMA2D_LINE_WATERMARK_LINE_WATERMARK_POS) & DMA2D_DMA2D_LINE_WATERMARK_LINE_WATERMARK_MASK);
    return reg_value;
}

static inline void dma2d_ll_set_dma2d_line_watermark_line_watermark(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(DMA2D_DMA2D_LINE_WATERMARK_ADDR);
    reg_value &= ~(DMA2D_DMA2D_LINE_WATERMARK_LINE_WATERMARK_MASK << DMA2D_DMA2D_LINE_WATERMARK_LINE_WATERMARK_POS);
    reg_value |= ((value & DMA2D_DMA2D_LINE_WATERMARK_LINE_WATERMARK_MASK) << DMA2D_DMA2D_LINE_WATERMARK_LINE_WATERMARK_POS);
    REG_WRITE(DMA2D_DMA2D_LINE_WATERMARK_ADDR,reg_value);
}

/* REG_0x17 */
#define DMA2D_DMA2D_FG_CLUT_ADDR  (DMA2D_LL_REG_BASE  + 0x17*4) //REG ADDR :0x48020400
#define DMA2D_DMA2D_FG_CLUT_FG_CLUT_POS (0)
#define DMA2D_DMA2D_FG_CLUT_FG_CLUT_MASK (0xFFFFFFFF)

#define DMA2D_DMA2D_FG_CLUT_FG_CLUT_POS (0)
#define DMA2D_DMA2D_FG_CLUT_FG_CLUT_MASK (0xFFFFFFFF)

/* REG_0x117 */
#define DMA2D_DMA2D_BG_CLUT0_ADDR  (DMA2D_LL_REG_BASE  + 0x117*4) //REG ADDR :0x48020800
#define DMA2D_DMA2D_BG_CLUT0_BG_CLUT0_POS (0)
#define DMA2D_DMA2D_BG_CLUT0_BG_CLUT0_MASK (0xFFFFFFFF)

#define DMA2D_DMA2D_BG_CLUT0_BG_CLUT0_POS (0)
#define DMA2D_DMA2D_BG_CLUT0_BG_CLUT0_MASK (0xFFFFFFFF)

#ifdef __cplusplus
}
#endif

