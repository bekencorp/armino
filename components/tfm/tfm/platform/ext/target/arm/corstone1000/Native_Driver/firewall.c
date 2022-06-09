/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "firewall.h"

#define FIREWALL_COMPONENT_SIZE      (0x10000)
#define FIREWALL_MAX_COMPONENTS      (0x20)

#define FIREWALL_COMMON_REG_OFFSET   (0xFA0)
struct _firewall_common_reg_map_t {
    volatile uint32_t fc_cap0;
         /*!< Offset: 0xFA0 (R/ ) Firewall Component Capability Register 0 */
    volatile uint32_t fc_cap1;
         /*!< Offset: 0xFA4 (R/ ) Firewall Component Capability Register 1 */
    volatile uint32_t fc_cap2;
         /*!< Offset: 0xFA8 (R/ ) Firewall Component Capability Register 2 */
    volatile uint32_t fc_cap3;
         /*!< Offset: 0xFAC (R/ ) Firewall Component Capability Register 3 */
    volatile uint32_t fc_cfg0;
         /*!< Offset: 0xFB0 (R/ ) Firewall Component Configuration Register 0 */
    volatile uint32_t fc_cfg1;
         /*!< Offset: 0xFB4 (R/ ) Firewall Component Configuration Register 1 */
    volatile uint32_t fc_cfg2;
         /*!< Offset: 0xFB8 (R/ ) Firewall Component Configuration Register 2 */
    volatile uint32_t fc_cfg3;
         /*!< Offset: 0xFBC (R/ ) Firewall Component Configuration Register 3 */
};

#define FIREWALL_PE_CS_REG_OFFSET    (0x100)
                      /*!< Protection Control and Status register type offset */
struct _firewall_pe_cs_reg_map_t {
    volatile uint32_t pe_ctrl;
                       /*!< Offset: 0x100 (R/W) Protection Extenstion Control */
    volatile uint32_t pe_st;
                       /*!< Offset: 0x104 (R/ ) Protection Extenstion Status */
    volatile uint32_t pe_bps;
                       /*!< Offset: 0x108 (R/W) Protection Extenstion Bypass */
};
#define PE_CTRL_EN_MASK              (0x1u  << PE_CTRL_EN_OFF)
#define PE_CTRL_EN_BYPASS_MASK       (0x1u  << PE_CTRL_EN_BYPASS_OFF)
#define PE_ST_EN_MASK                (0x1u  << PE_ST_EN_OFF)
#define PE_BPS_BYPASS_ST_MASK        (0x1u  << PE_BPS_BYPASS_ST_OFF)

#define FIREWALL_PE_RWE_REG_OFFSET   (0x10C)
                      /*!< Region Window Entry (RWE) register type offset */
struct _firewall_pe_rwe_reg_map_t {
    volatile uint32_t rwe_ctrl;
                     /*!< Offset: 0x10C (R/W) Region Window Entry Control */
    volatile uint32_t rgn_ctrl0;
                     /*!< Offset: 0x110 (R/W) Region Control 0 */
    volatile uint32_t rgn_ctrl1;
                     /*!< Offset: 0x114 (R/W) Region Control 1 */
    volatile uint32_t rgn_lctrl;
                     /*!< Offset: 0x118 (R/W) Region Lock Control */
    volatile uint32_t rgn_st;
                     /*!< Offset: 0x11C (R/ ) Region Status */
    volatile uint32_t rgn_cfg0;
                     /*!< Offset: 0x120 (R/W) Region Config 0 */
    volatile uint32_t rgn_cfg1;
                     /*!< Offset: 0x124 (R/W) Region Config 1 */
    volatile uint32_t rgn_size;
                     /*!< Offset: 0x128 (R/W) Region Size */
    volatile uint32_t reserved_0;
                     /*!< Offset: 0x12C       Reserved */
    volatile uint32_t rgn_tcfg0;
                     /*!< Offset: 0x130 (R/W) Region Translation Config 0 */
    volatile uint32_t rgn_tcfg1;
                     /*!< Offset: 0x134 (R/W) Region Translation Config 1 */
    volatile uint32_t rgn_tcfg2;
                     /*!< Offset: 0x138 (R/W) Region Translation Config 2 */
    volatile uint32_t reserved_1;
                     /*!< Offset: 0x13C       Reserved */
    volatile uint32_t rgn_mid0;
                     /*!< Offset: 0x140 (R/W) Region Master ID 0 */
    volatile uint32_t rgn_mpl0;
                     /*!< Offset: 0x144 (R/W) Region Master Permission List 0 */
    volatile uint32_t rgn_mid1;
                     /*!< Offset: 0x148 (R/W) Region Master ID 1 */
    volatile uint32_t rgn_mpl1;
                     /*!< Offset: 0x14C (R/W) Region Master Permission List 1 */
    volatile uint32_t rgn_mid2;
                     /*!< Offset: 0x150 (R/W) Region Master ID 2 */
    volatile uint32_t rgn_mpl2;
                     /*!< Offset: 0x154 (R/W) Region Master Permission List 2 */
    volatile uint32_t rgn_mid3;
                     /*!< Offset: 0x158 (R/W) Region Master ID 3 */
    volatile uint32_t rgn_mpl3;
                     /*!< Offset: 0x15C (R/W) Region Master Permission List 3 */
};
#define RWE_CTRL_RGN_INDX_MASK       (0xFFu   << RWE_CTRL_RGN_INDX_OFF)
#define RGN_CTRL0_EN_MASK            (0x1u    << RGN_EN_OFF)
#define RGN_LCTRL_LOCK_MASK          (0x1u    << RGN_LCTRL_LOCK_OFF)
#define RGN_CTRL1_MPE_EN_MASK        (0xFu    << RGN_MPE0_EN_OFF)
#define RGN_MPL_EN_MASK              (0x1FFFu)
#define RGN_ST_EN_MASK               (0x1u    << RGN_EN_OFF)
#define RGN_ST_MPE0_EN_MASK          (0x1u    << RGN_MPE0_EN_OFF)
#define RGN_ST_MPE1_EN_MASK          (0x1u    << RGN_MPE1_EN_OFF)
#define RGN_ST_MPE2_EN_MASK          (0x1u    << RGN_MPE2_EN_OFF)
#define RGN_ST_MPE3_EN_MASK          (0x1u    << RGN_MPE3_EN_OFF)
#define RGN_SIZE_SIZE_MASK           (0xFFu   << RGN_SIZE_SIZE_OFF)
#define RGN_SIZE_MULnPO2_MASK        (0x1u    << RGN_SIZE_MULnPO2_OFF)
#define RGN_TCFG2_ADDR_TRANS_EN_MASK (0x1u    << RGN_TCFG2_ADDR_TRANS_EN_OFF)
#define RGN_TCFG2_MA_TRANS_EN_MASK   (0x1u    << RGN_TCFG2_MA_TRANS_EN_OFF)
#define RGN_TCFG2_INST_MASK          (0x3u    << RGN_TCFG2_INST_OFF)
#define RGN_TCFG2_PRIV_MASK          (0x3u    << RGN_TCFG2_PRIV_OFF)
#define RGN_TCFG2_MA_MASK            (0xFFu   << RGN_TCFG2_MA_OFF)
#define RGN_TCFG2_SH_MASK            (0x3u    << RGN_TCFG2_SH_OFF)
#define RGN_TCFG2_NS_MASK            (0x3u    << RGN_TCFG2_NS_OFF)

#define FIREWALL_PE_FWE_REG_OFFSET   (0x180)
                      /*!< Fault Window Entry (FWE) register type offset */
struct _firewall_pe_fwe_reg_map_t {
    volatile uint32_t fe_tal;
               /*!< Offset: 0x180 (R/ ) Fault Entry Transaction Address Lower */
    volatile uint32_t fe_tau;
               /*!< Offset: 0x184 (R/ ) Fault Entry Transaction Address Upper */
    volatile uint32_t fe_tp;
               /*!< Offset: 0x188 (R/ ) Fault Entry Transaction Properties */
    volatile uint32_t fe_mid;
               /*!< Offset: 0x18C (R/ ) Fault Entry Master ID */
    volatile uint32_t fe_ctrl;
               /*!< Offset: 0x190 (R/W) Fault Entry Control */
};

#define FIREWALL_LDE_REG_OFFSET      (0x10)
                      /*!< Lockdown Extension (LDE) register type offset */
struct _firewall_lde_reg_map_t {
    volatile uint32_t ld_ctrl;
               /*!< Offset: 0x10 (R/W) Lockdown Control */
};
#define LD_CTRL_LOCK_MASK            (0x3u   << LD_CTRL_LOCK_OFF)
#define LD_CTRL_LDI_ST_MASK          (0x1u   << LD_CTRL_LDI_ST_OFF)

#define CS_REG_ADDR(x, y)\
                (x + (y * FIREWALL_COMPONENT_SIZE) + FIREWALL_PE_CS_REG_OFFSET)
#define RWE_REG_ADDR(x, y)\
                (x + (y * FIREWALL_COMPONENT_SIZE) + FIREWALL_PE_RWE_REG_OFFSET)
#define FWE_REG_ADDR(x, y)\
                (x + (y * FIREWALL_COMPONENT_SIZE) + FIREWALL_PE_FWE_REG_OFFSET)
#define LDE_REG_ADDR(x)    (x + FIREWALL_LDE_REG_OFFSET)

struct fw_dev_data_t fw_data;

void fc_select(void *base_addr, uint32_t comp_id)
{
    fw_data.base_addr = base_addr;
    fw_data.comp_id = comp_id;
    fw_data.cs_ptr  = CS_REG_ADDR(fw_data.base_addr, fw_data.comp_id);
    fw_data.rwe_ptr = RWE_REG_ADDR(fw_data.base_addr, fw_data.comp_id);
}

void fc_enable_bypass(void)
{
    struct _firewall_pe_cs_reg_map_t *ptr =
         (struct _firewall_pe_cs_reg_map_t *)fw_data.cs_ptr;
    ptr->pe_ctrl &= ~PE_CTRL_EN_BYPASS_MASK;
    while (!(ptr->pe_bps & PE_BPS_BYPASS_ST_MASK))
        ;
}

void fc_disable_bypass(void)
{
    struct _firewall_pe_cs_reg_map_t *ptr =
         (struct _firewall_pe_cs_reg_map_t *)fw_data.cs_ptr;
    ptr->pe_ctrl |= PE_CTRL_EN_BYPASS_MASK;
    while ((ptr->pe_bps & PE_BPS_BYPASS_ST_MASK))
        ;
}

void fc_pe_enable(void)
{
    struct _firewall_pe_cs_reg_map_t *ptr =
         (struct _firewall_pe_cs_reg_map_t *)fw_data.cs_ptr;
    ptr->pe_ctrl |= PE_CTRL_EN_MASK;
    while (!(ptr->pe_st & PE_ST_EN_MASK))
        ;
}

void fc_pe_disable(void)
{
    struct _firewall_pe_cs_reg_map_t *ptr =
         (struct _firewall_pe_cs_reg_map_t *)fw_data.cs_ptr;
    ptr->pe_ctrl &= ~PE_CTRL_EN_MASK;
    while ((ptr->pe_st & PE_ST_EN_MASK))
        ;
}

void fc_disable_txn_term_error(void *base_addr)
{
    uint32_t *ptr = (uint32_t *)base_addr;
    /* Write 0 to ERR bit field of FW_CTRL register */
    *ptr = 0;
}

void fc_select_region(uint32_t region_id)
{
    struct _firewall_pe_rwe_reg_map_t *ptr =
         (struct _firewall_pe_rwe_reg_map_t *)fw_data.rwe_ptr;
    ptr->rwe_ctrl = (ptr->rwe_ctrl & ~RWE_CTRL_RGN_INDX_MASK) |
                    (region_id & RWE_CTRL_RGN_INDX_MASK);
}

void fc_enable_regions(void)
{
    struct _firewall_pe_rwe_reg_map_t *ptr =
         (struct _firewall_pe_rwe_reg_map_t *)fw_data.rwe_ptr;
    ptr->rgn_ctrl0 |= RGN_CTRL0_EN_MASK;
}

void fc_disable_regions(void)
{
    struct _firewall_pe_rwe_reg_map_t *ptr =
         (struct _firewall_pe_rwe_reg_map_t *)fw_data.rwe_ptr;
    ptr->rgn_ctrl0 &= ~RGN_CTRL0_EN_MASK;
}

void fc_prog_rgn(enum rgn_size_t size, uint32_t base_addr)
{
    struct _firewall_pe_rwe_reg_map_t *ptr =
         (struct _firewall_pe_rwe_reg_map_t *)fw_data.rwe_ptr;
    /*
     * As the reset value is rgn_size register is UNKNOWN, Arm recommends to set
     * register to a known value before performing read-modify-write operations
     */
    ptr->rgn_size = 0;
    ptr->rgn_size = (ptr->rgn_size & ~RGN_SIZE_SIZE_MASK) |
                    (size & RGN_SIZE_SIZE_MASK);
    ptr->rgn_cfg0 = base_addr;
}

void fc_prog_rgn_upper_addr(uint32_t upper_addr)
{
    struct _firewall_pe_rwe_reg_map_t *ptr =
         (struct _firewall_pe_rwe_reg_map_t *)fw_data.rwe_ptr;
    ptr->rgn_tcfg0 = upper_addr;
}

void fc_enable_addr_trans(void)
{
    struct _firewall_pe_rwe_reg_map_t *ptr =
         (struct _firewall_pe_rwe_reg_map_t *)fw_data.rwe_ptr;
    ptr->rgn_tcfg2 |= RGN_TCFG2_ADDR_TRANS_EN_MASK;
}

void fc_disable_addr_trans(void)
{
    struct _firewall_pe_rwe_reg_map_t *ptr =
         (struct _firewall_pe_rwe_reg_map_t *)fw_data.rwe_ptr;
    ptr->rgn_tcfg2 &= ~RGN_TCFG2_ADDR_TRANS_EN_MASK;
}

void fc_init_mpl(enum rgn_mpe_t mpe)
{
    struct _firewall_pe_rwe_reg_map_t *ptr =
         (struct _firewall_pe_rwe_reg_map_t *)fw_data.rwe_ptr;
    /* Before enabling an MPE, it must:
     * 1) Set the RGN_MPL fields with an UNKNOWN reset value to a known value.
     * 2) Set either:
     *    RGN_MID{0-3} to a known value.
     *    RGN_MPL.ANY_MST to 0b1.
     */
    if (mpe == RGN_MPE0) {
        ptr->rgn_mpl0 = 0x1000;
        ptr->rgn_mpl0 = 0x0;
    } else if (mpe == RGN_MPE1) {
        ptr->rgn_mpl1 = 0x1000;
        ptr->rgn_mpl1 = 0x0;
    } else if (mpe == RGN_MPE2) {
        ptr->rgn_mpl2 = 0x1000;
        ptr->rgn_mpl2 = 0x0;
    } else if (mpe == RGN_MPE3) {
        ptr->rgn_mpl3 = 0x1000;
        ptr->rgn_mpl3 = 0x0;
    }
}

void fc_enable_mpl(enum rgn_mpe_t mpe, enum rgn_mpl_t mpl)
{
    struct _firewall_pe_rwe_reg_map_t *ptr =
         (struct _firewall_pe_rwe_reg_map_t *)fw_data.rwe_ptr;
    if (mpe == RGN_MPE0)
        ptr->rgn_mpl0 |= (mpl & RGN_MPL_EN_MASK);
    else if (mpe == RGN_MPE1)
        ptr->rgn_mpl1 |= (mpl & RGN_MPL_EN_MASK);
    else if (mpe == RGN_MPE2)
        ptr->rgn_mpl2 |= (mpl & RGN_MPL_EN_MASK);
    else if (mpe == RGN_MPE3)
        ptr->rgn_mpl3 |= (mpl & RGN_MPL_EN_MASK);
}

void fc_disable_mpl(enum rgn_mpe_t mpe, enum rgn_mpl_t mpl)
{
    struct _firewall_pe_rwe_reg_map_t *ptr =
         (struct _firewall_pe_rwe_reg_map_t *)fw_data.rwe_ptr;
    mpl &= RGN_MPL_EN_MASK;
    if (mpe == RGN_MPE0)
        ptr->rgn_mpl0 &= ~mpl;
    else if (mpe == RGN_MPE1)
        ptr->rgn_mpl1 &= ~mpl;
    else if (mpe == RGN_MPE2)
        ptr->rgn_mpl2 &= ~mpl;
    else if (mpe == RGN_MPE3)
        ptr->rgn_mpl3 &= ~mpl;
}

void fc_prog_mid(enum rgn_mpe_t mpe, uint32_t mid)
{
    struct _firewall_pe_rwe_reg_map_t *ptr =
         (struct _firewall_pe_rwe_reg_map_t *)fw_data.rwe_ptr;
    if (mpe == RGN_MPE0)
        ptr->rgn_mid0 = mid;
    else if (mpe == RGN_MPE1)
        ptr->rgn_mid1 = mid;
    else if (mpe == RGN_MPE2)
        ptr->rgn_mid2 = mid;
    else if (mpe == RGN_MPE3)
        ptr->rgn_mid3 = mid;
}

void fc_enable_mpe(enum rgn_mpe_t mpe)
{
    struct _firewall_pe_rwe_reg_map_t *ptr =
         (struct _firewall_pe_rwe_reg_map_t *)fw_data.rwe_ptr;
    ptr->rgn_ctrl1 |= (mpe & RGN_CTRL1_MPE_EN_MASK);
    while (!(ptr->rgn_st & mpe))
        ;
}

void fc_disable_mpe(enum rgn_mpe_t mpe)
{
    struct _firewall_pe_rwe_reg_map_t *ptr =
         (struct _firewall_pe_rwe_reg_map_t *)fw_data.rwe_ptr;
    mpe &= RGN_CTRL1_MPE_EN_MASK;
    ptr->rgn_ctrl1 &= ~mpe;
    while ((ptr->rgn_st & mpe))
        ;
}

void fc_rgn_lock(void)
{
    struct _firewall_pe_rwe_reg_map_t *ptr =
         (struct _firewall_pe_rwe_reg_map_t *)fw_data.rwe_ptr;
    ptr->rgn_lctrl |= RGN_LCTRL_LOCK_MASK;
}

void fc_rgn_unlock(void)
{
    struct _firewall_pe_rwe_reg_map_t *ptr =
         (struct _firewall_pe_rwe_reg_map_t *)fw_data.rwe_ptr;
    ptr->rgn_lctrl &= ~RGN_LCTRL_LOCK_MASK;
}

enum fw_lockdown_status_t fw_get_lockdown_status(void)
{
    struct _firewall_lde_reg_map_t *ptr = (struct _firewall_lde_reg_map_t *)
     LDE_REG_ADDR(fw_data.base_addr);
    if (ptr->ld_ctrl & LD_CTRL_LDI_ST_MASK)
        return FW_LOCKED;
    else
        return FW_UNLOCKED;
}

void fw_lockdown(enum fw_lockdown_state_t lockdown_state)
{
    struct _firewall_lde_reg_map_t *ptr = (struct _firewall_lde_reg_map_t *)
     LDE_REG_ADDR(fw_data.base_addr);
    ptr->ld_ctrl |= lockdown_state;
}
