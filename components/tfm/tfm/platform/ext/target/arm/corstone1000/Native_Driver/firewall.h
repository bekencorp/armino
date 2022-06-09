/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef FIREWALL_H
#define FIREWALL_H

#include <stdint.h>

#define PE_CTRL_EN_OFF              0x1Fu
     /*!< Protection Extenstion Control enable bit field offset */
#define PE_CTRL_EN_BYPASS_OFF       0x5u
     /*!< Protection Extenstion Control bypass enable bit field offset */
#define PE_ST_EN_OFF                0x1Fu
     /*!< Protection Extenstion Status enable bit field offset */
#define PE_BPS_BYPASS_ST_OFF        0x1u
     /*!< Protection Extenstion Bypass status field offset */

#define RWE_CTRL_RGN_INDX_OFF       0x0u
  /*!< RWE Control Region Index offset */
#define RGN_EN_OFF                  0x0u
  /*!< Region enable offset */
#define RGN_LCTRL_LOCK_OFF          0x0u
  /*!< Region Lock Control lock offset */
#define RGN_MPE0_EN_OFF             0x1u
  /*!< Region MPE0 enable offset */
#define RGN_MPE1_EN_OFF             0x2u
  /*!< Region MPE1 enable offset */
#define RGN_MPE2_EN_OFF             0x3u
  /*!< Region MPE2 enable offset */
#define RGN_MPE3_EN_OFF             0x4u
  /*!< Region MPE3 enable offset */
#define RGN_SIZE_SIZE_OFF           0x0u
  /*!< Region Size Size offset */
#define RGN_SIZE_MULnPO2_OFF        0x8u
  /*!< Region Size MULnPO2 offset */
#define RGN_TCFG2_ADDR_TRANS_EN_OFF 0x11u
  /*!< Region Translation Config 2 Address Translation enable offset */
#define RGN_TCFG2_MA_TRANS_EN_OFF   0x10u
  /*!< Region Translation Config 2 Memory Attribute Translation enable offset */
#define RGN_TCFG2_INST_OFF          0xEu
  /*!< Region Translation Config 2 Output txn instruction or data offset */
#define RGN_TCFG2_PRIV_OFF          0xCu
  /*!< Region Translation Config 2 Output txn privileged level offset */
#define RGN_TCFG2_MA_OFF            0x4u
  /*!< Region Translation Config 2 Output txn memory attribute offset */
#define RGN_TCFG2_SH_OFF            0x2u
  /*!< Region Translation Config 2 Output txn shareability offset */
#define RGN_TCFG2_NS_OFF            0x0u
  /*!< Region Translation Config 2 Output txn security offset */
#define RGN_MPL_ANY_MST_OFF         0xCu
  /*!< Region Master Permission List Any Master ID offset */
#define RGN_MPL_SPX_OFF             0xBu
  /*!< Region Master Permission List S privilege execute enable offset */
#define RGN_MPL_SPW_OFF             0xAu
  /*!< Region Master Permission List S privilege write enable offset */
#define RGN_MPL_SPR_OFF             0x9u
  /*!< Region Master Permission List S privilege read enable offset */
#define RGN_MPL_SUX_OFF             0x8u
  /*!< Region Master Permission List S unprivileged execute enable offset */
#define RGN_MPL_SUW_OFF             0x7u
  /*!< Region Master Permission List S unprivileged write enable offset */
#define RGN_MPL_SUR_OFF             0x6u
  /*!< Region Master Permission List S unprivileged read enable offset */
#define RGN_MPL_NSPX_OFF            0x5u
  /*!< Region Master Permission List NS privilege execute enable offset */
#define RGN_MPL_NSPW_OFF            0x4u
  /*!< Region Master Permission List NS privilege write enable offset */
#define RGN_MPL_NSPR_OFF            0x3u
  /*!< Region Master Permission List NS privilege read enable offset */
#define RGN_MPL_NSUX_OFF            0x2u
  /*!< Region Master Permission List NS unprivileged execute enable offset */
#define RGN_MPL_NSUW_OFF            0x1u
  /*!< Region Master Permission List NS unprivileged write enable offset */
#define RGN_MPL_NSUR_OFF            0x0u
  /*!< Region Master Permission List NS unprivileged read enable offset */

#define LD_CTRL_LOCK_OFF            0x0u
#define LD_CTRL_LDI_ST_OFF          0x2u

/**
 * \brief Firewall region size values
 */
enum rgn_size_t {
  RGN_SIZE_0B    = (0x00u),
  RGN_SIZE_32B   = (0x05u),
  RGN_SIZE_64B,
  RGN_SIZE_128B,
  RGN_SIZE_256B,
  RGN_SIZE_512B,
  RGN_SIZE_1KB,
  RGN_SIZE_2KB,
  RGN_SIZE_4KB,
  RGN_SIZE_8KB,
  RGN_SIZE_16KB,
  RGN_SIZE_32KB,
  RGN_SIZE_64KB,
  RGN_SIZE_128KB,
  RGN_SIZE_256KB,
  RGN_SIZE_512KB,
  RGN_SIZE_1MB,
  RGN_SIZE_2MB,
  RGN_SIZE_4MB,
  RGN_SIZE_8MB,
  RGN_SIZE_16MB,
  RGN_SIZE_32MB,
  RGN_SIZE_64MB,
  RGN_SIZE_128MB,
  RGN_SIZE_256MB,
  RGN_SIZE_512MB,
  RGN_SIZE_1GB,
  RGN_SIZE_2GB,
  RGN_SIZE_16EB  = (0x40u),
};

enum rgn_mpe_t {
  RGN_MPE0 = (0x1u << RGN_MPE0_EN_OFF),
  RGN_MPE1 = (0x1u << RGN_MPE1_EN_OFF),
  RGN_MPE2 = (0x1u << RGN_MPE2_EN_OFF),
  RGN_MPE3 = (0x1u << RGN_MPE3_EN_OFF),
};

enum rgn_mpl_t {
  RGN_MPL_ANY_MST_MASK = (0x1u << RGN_MPL_ANY_MST_OFF),
  RGN_MPL_SPX_MASK     = (0x1u << RGN_MPL_SPX_OFF),
  RGN_MPL_SPW_MASK     = (0x1u << RGN_MPL_SPW_OFF),
  RGN_MPL_SPR_MASK     = (0x1u << RGN_MPL_SPR_OFF),
  RGN_MPL_SUX_MASK     = (0x1u << RGN_MPL_SUX_OFF),
  RGN_MPL_SUW_MASK     = (0x1u << RGN_MPL_SUW_OFF),
  RGN_MPL_SUR_MASK     = (0x1u << RGN_MPL_SUR_OFF),
  RGN_MPL_NSPX_MASK    = (0x1u << RGN_MPL_NSPX_OFF),
  RGN_MPL_NSPW_MASK    = (0x1u << RGN_MPL_NSPW_OFF),
  RGN_MPL_NSPR_MASK    = (0x1u << RGN_MPL_NSPR_OFF),
  RGN_MPL_NSUX_MASK    = (0x1u << RGN_MPL_NSUX_OFF),
  RGN_MPL_NSUW_MASK    = (0x1u << RGN_MPL_NSUW_OFF),
  RGN_MPL_NSUR_MASK    = (0x1u << RGN_MPL_NSUR_OFF),
};


#define RGN_MPL_SECURE_READ_MASK       (RGN_MPL_SUR_MASK | RGN_MPL_SPR_MASK)
#define RGN_MPL_SECURE_WRITE_MASK      (RGN_MPL_SUW_MASK | RGN_MPL_SPW_MASK)
#define RGN_MPL_SECURE_EXECUTE_MASK    (RGN_MPL_SUX_MASK | RGN_MPL_SPX_MASK)

#define RGN_MPL_NONSECURE_READ_MASK    (RGN_MPL_NSUR_MASK | RGN_MPL_NSPR_MASK)
#define RGN_MPL_NONSECURE_WRITE_MASK   (RGN_MPL_NSUW_MASK | RGN_MPL_NSPW_MASK)
#define RGN_MPL_NONSECURE_EXECUTE_MASK (RGN_MPL_NSUX_MASK | RGN_MPL_NSPX_MASK)

enum fw_lockdown_status_t {
  FW_UNLOCKED = 0x0u,
  FW_LOCKED   = 0x1u,
};

enum fw_lockdown_state_t {
  FW_OPEN_LOCKDOWN    = 0x0u,
  FW_PARTIAL_LOCKDOWN = 0x2u,
  FW_FULL_LOCKDOWN    = 0x3u,
};

struct fw_dev_data_t {
    void *base_addr;        /*!< Current FW base address */
    uint32_t comp_id;       /*!< Current FC id */
    void *cs_ptr;          /*!< Selected FC Control & Status address */
    void *rwe_ptr;          /*!< Selected FC Region Window Entry address */
};

/**
 * \brief Selects a Firewall Component
 */
void fc_select(void *base_addr, uint32_t comp_id);

/**
 * \brief Enables bypass in the selected Firewall Component
 */
void fc_enable_bypass(void);

/**
 * \brief Disables bypass in the selected Firewall Component
 */
void fc_disable_bypass(void);

/**
 * \brief Enables Protection Extenstion in the selected Firewall Component
 */
void fc_pe_enable(void);

/**
 * \brief Disables Protection Extenstion in the selected Firewall Component
 */
void fc_pe_disable(void);

/**
 * \brief Disables the error generation in response for transaction generating
 *        a Configuration Access Error
 */
void fc_disable_txn_term_error(void *base_addr);

/**
 * \brief Selects a region in the selected Firewall Component
 */
void fc_select_region(uint32_t region_id);

/**
 * \brief Enables a region in the selected Firewall Component
 */
void fc_enable_regions(void);

/**
 * \brief Disables a region in the selected Firewall Component
 */
void fc_disable_regions(void);

/**
 * \brief Programs a region in the selected Firewall Component
 */
void fc_prog_rgn(enum rgn_size_t size, uint32_t base_addr);

/**
 * \brief Programs the upper address in the selected Firewall Component
 */
void fc_prog_rgn_upper_addr(uint32_t upper_addr);

/**
 * \brief Enables address translation in the selected Firewall Component
 */
void fc_enable_addr_trans(void);

/**
 * \brief Disables address translation in the selected Firewall Component
 */
void fc_disable_addr_trans(void);

/**
 * \brief Initializes the Master Permission List in selected Firewall Component
 * \note Before enabling an MPE, it must:
 *       1) Set the RGN_MPL fields with an UNKNOWN reset value to a known value.
 *       2) Set either:
 *          RGN_MID{0-3} to a known value.
 *          RGN_MPL.ANY_MST to 0b1.
 */
void fc_init_mpl(enum rgn_mpe_t mpe);

/**
 * \brief Enables Master Permission List in the selected Firewall Component
 */
void fc_enable_mpl(enum rgn_mpe_t mpe, enum rgn_mpl_t mpl);

/**
 * \brief Disables Master Permission List in the selected Firewall Component
 */
void fc_disable_mpl(enum rgn_mpe_t mpe, enum rgn_mpl_t mpl);

/**
 * \brief Programs Master ID in the given Master permission entry
 */
void fc_prog_mid(enum rgn_mpe_t mpe, uint32_t mid);

/**
 * \brief Enables Master permission entry in the selected Firewall Component
 */
void fc_enable_mpe(enum rgn_mpe_t mpe);

/**
 * \brief Disables Master permission entry in the selected Firewall Component
 */
void fc_disable_mpe(enum rgn_mpe_t mpe);

/**
 * \brief Locks the selected region in the selected Firewall Component
 */
void fc_rgn_lock(void);

/**
 * \brief Unlocks the selected region in the selected Firewall Component
 */
void fc_rgn_unlock(void);

/**
 * \brief Returns the lockdown status of the Firewall IP
 */
enum fw_lockdown_status_t fw_get_lockdown_status(void);

/**
 * \brief Locksdown the Firewall IP
 */
void fw_lockdown(enum fw_lockdown_state_t lockdown_state);

#endif /* FIREWALL_H */
