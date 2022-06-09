/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CC3XX_REG_DEFS_H
#define CC3XX_REG_DEFS_H

#include "cc3xx_config.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef volatile uint32_t *cc3xx_reg_t;

#ifndef CC3XX_REG_BASE
#define CC3XX_REG_BASE CC3XX_BASE
#endif /* !CC3XX_REG_BASE */

/* RNG registers */
#define CC3XX_REG_RNG_RNG_IMR                          (cc3xx_reg_t)(CC3XX_REG_BASE + 0x100U)
#define CC3XX_REG_RNG_RNG_ISR                          (cc3xx_reg_t)(CC3XX_REG_BASE + 0x104U)
#define CC3XX_REG_RNG_RNG_ICR                          (cc3xx_reg_t)(CC3XX_REG_BASE + 0x108U)
#define CC3XX_REG_RNG_TRNG_CONFIG                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x10CU)
#define CC3XX_REG_RNG_TRNG_VALID                       (cc3xx_reg_t)(CC3XX_REG_BASE + 0x110U)
#define CC3XX_REG_RNG_EHR_DATA_0                       (cc3xx_reg_t)(CC3XX_REG_BASE + 0x114U)
#define CC3XX_REG_RNG_EHR_DATA_1                       (cc3xx_reg_t)(CC3XX_REG_BASE + 0x118U)
#define CC3XX_REG_RNG_EHR_DATA_2                       (cc3xx_reg_t)(CC3XX_REG_BASE + 0x11CU)
#define CC3XX_REG_RNG_EHR_DATA_3                       (cc3xx_reg_t)(CC3XX_REG_BASE + 0x120U)
#define CC3XX_REG_RNG_EHR_DATA_4                       (cc3xx_reg_t)(CC3XX_REG_BASE + 0x124U)
#define CC3XX_REG_RNG_EHR_DATA_5                       (cc3xx_reg_t)(CC3XX_REG_BASE + 0x128U)
#define CC3XX_REG_RNG_RND_SOURCE_ENABLE                (cc3xx_reg_t)(CC3XX_REG_BASE + 0x12CU)
#define CC3XX_REG_RNG_SAMPLE_CNT1                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x130U)
#define CC3XX_REG_RNG_AUTOCORR_STATISTIC               (cc3xx_reg_t)(CC3XX_REG_BASE + 0x134U)
#define CC3XX_REG_RNG_TRNG_DEBUG_CONTROL               (cc3xx_reg_t)(CC3XX_REG_BASE + 0x138U)
#define CC3XX_REG_RNG_RNG_SW_RESET                     (cc3xx_reg_t)(CC3XX_REG_BASE + 0x140U)
#define CC3XX_REG_RNG_RNG_DEBUG_EN_INPUT               (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1B4U)
#define CC3XX_REG_RNG_RNG_BUSY                         (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1B8U)
#define CC3XX_REG_RNG_RST_BITS_COUNTER                 (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1BCU)
#define CC3XX_REG_RNG_RNG_VERSION                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1C0U)
#define CC3XX_REG_RNG_RNG_CLK_ENABLE                   (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1C4U)
#define CC3XX_REG_RNG_RNG_DMA_ENABLE                   (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1C8U)
#define CC3XX_REG_RNG_RNG_DMA_SRC_MASK                 (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1CCU)
#define CC3XX_REG_RNG_RNG_DMA_SRAM_ADDR                (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1D0U)
#define CC3XX_REG_RNG_RNG_DMA_SAMPLES_NUM              (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1D4U)
#define CC3XX_REG_RNG_RNG_WATCHDOG_VAL                 (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1D8U)
#define CC3XX_REG_RNG_RNG_DMA_STATUS                   (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1DCU)

/* AES registers */
#define CC3XX_REG_AES_AES_KEY_0_0                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x400U)
#define CC3XX_REG_AES_AES_KEY_0_1                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x404U)
#define CC3XX_REG_AES_AES_KEY_0_2                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x408U)
#define CC3XX_REG_AES_AES_KEY_0_3                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x40CU)
#define CC3XX_REG_AES_AES_KEY_0_4                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x410U)
#define CC3XX_REG_AES_AES_KEY_0_5                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x414U)
#define CC3XX_REG_AES_AES_KEY_0_6                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x418U)
#define CC3XX_REG_AES_AES_KEY_0_7                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x41CU)
#define CC3XX_REG_AES_AES_KEY_1_0                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x420U)
#define CC3XX_REG_AES_AES_KEY_1_1                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x424U)
#define CC3XX_REG_AES_AES_KEY_1_2                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x428U)
#define CC3XX_REG_AES_AES_KEY_1_3                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x42CU)
#define CC3XX_REG_AES_AES_KEY_1_4                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x430U)
#define CC3XX_REG_AES_AES_KEY_1_5                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x434U)
#define CC3XX_REG_AES_AES_KEY_1_6                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x438U)
#define CC3XX_REG_AES_AES_KEY_1_7                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x43CU)
#define CC3XX_REG_AES_AES_IV_0_0                       (cc3xx_reg_t)(CC3XX_REG_BASE + 0x440U)
#define CC3XX_REG_AES_AES_IV_0_1                       (cc3xx_reg_t)(CC3XX_REG_BASE + 0x444U)
#define CC3XX_REG_AES_AES_IV_0_2                       (cc3xx_reg_t)(CC3XX_REG_BASE + 0x448U)
#define CC3XX_REG_AES_AES_IV_0_3                       (cc3xx_reg_t)(CC3XX_REG_BASE + 0x44CU)
#define CC3XX_REG_AES_AES_IV_1_0                       (cc3xx_reg_t)(CC3XX_REG_BASE + 0x450U)
#define CC3XX_REG_AES_AES_IV_1_1                       (cc3xx_reg_t)(CC3XX_REG_BASE + 0x454U)
#define CC3XX_REG_AES_AES_IV_1_2                       (cc3xx_reg_t)(CC3XX_REG_BASE + 0x458U)
#define CC3XX_REG_AES_AES_IV_1_3                       (cc3xx_reg_t)(CC3XX_REG_BASE + 0x45CU)
#define CC3XX_REG_AES_AES_CTR_0_0                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x460U)
#define CC3XX_REG_AES_AES_CTR_0_1                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x464U)
#define CC3XX_REG_AES_AES_CTR_0_2                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x468U)
#define CC3XX_REG_AES_AES_CTR_0_3                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x46CU)
#define CC3XX_REG_AES_AES_BUSY                         (cc3xx_reg_t)(CC3XX_REG_BASE + 0x470U)
#define CC3XX_REG_AES_AES_SK                           (cc3xx_reg_t)(CC3XX_REG_BASE + 0x478U)
#define CC3XX_REG_AES_AES_CMAC_INIT                    (cc3xx_reg_t)(CC3XX_REG_BASE + 0x47CU)
#define CC3XX_REG_AES_AES_SK1                          (cc3xx_reg_t)(CC3XX_REG_BASE + 0x4B4U)
#define CC3XX_REG_AES_AES_REMAINING_BYTES              (cc3xx_reg_t)(CC3XX_REG_BASE + 0x4BCU)
#define CC3XX_REG_AES_AES_CONTROL                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x4C0U)
#define CC3XX_REG_AES_AES_HW_FLAGS                     (cc3xx_reg_t)(CC3XX_REG_BASE + 0x4C8U)
#define CC3XX_REG_AES_AES_CTR_NO_INCREMENT             (cc3xx_reg_t)(CC3XX_REG_BASE + 0x4D8U)
#define CC3XX_REG_AES_AES_DFA_IS_ON                    (cc3xx_reg_t)(CC3XX_REG_BASE + 0x4F0U)
#define CC3XX_REG_AES_AES_DFA_ERR_STATUS               (cc3xx_reg_t)(CC3XX_REG_BASE + 0x4F8U)
#define CC3XX_REG_AES_AES_CMAC_SIZE0_KICK              (cc3xx_reg_t)(CC3XX_REG_BASE + 0x524U)

/* HASH registers */
#define CC3XX_REG_HASH_HASH_H0                         (cc3xx_reg_t)(CC3XX_REG_BASE + 0x640)
#define CC3XX_REG_HASH_HASH_H1                         (cc3xx_reg_t)(CC3XX_REG_BASE + 0x644)
#define CC3XX_REG_HASH_HASH_H2                         (cc3xx_reg_t)(CC3XX_REG_BASE + 0x648)
#define CC3XX_REG_HASH_HASH_H3                         (cc3xx_reg_t)(CC3XX_REG_BASE + 0x64C)
#define CC3XX_REG_HASH_HASH_H4                         (cc3xx_reg_t)(CC3XX_REG_BASE + 0x650)
#define CC3XX_REG_HASH_HASH_H5                         (cc3xx_reg_t)(CC3XX_REG_BASE + 0x654)
#define CC3XX_REG_HASH_HASH_H6                         (cc3xx_reg_t)(CC3XX_REG_BASE + 0x658)
#define CC3XX_REG_HASH_HASH_H7                         (cc3xx_reg_t)(CC3XX_REG_BASE + 0x65C)
#define CC3XX_REG_HASH_HASH_H8                         (cc3xx_reg_t)(CC3XX_REG_BASE + 0x660)
#define CC3XX_REG_HASH_AUTO_HW_PADDING                 (cc3xx_reg_t)(CC3XX_REG_BASE + 0x684)
#define CC3XX_REG_HASH_HASH_XOR_DIN                    (cc3xx_reg_t)(CC3XX_REG_BASE + 0x688)
#define CC3XX_REG_HASH_LOAD_INIT_STATE                 (cc3xx_reg_t)(CC3XX_REG_BASE + 0x694)
#define CC3XX_REG_HASH_HASH_SEL_AES_MAC                (cc3xx_reg_t)(CC3XX_REG_BASE + 0x6A4)
#define CC3XX_REG_HASH_HASH_VERSION                    (cc3xx_reg_t)(CC3XX_REG_BASE + 0x7B0)
#define CC3XX_REG_HASH_HASH_CONTROL                    (cc3xx_reg_t)(CC3XX_REG_BASE + 0x7C0)
#define CC3XX_REG_HASH_HASH_PAD_EN                     (cc3xx_reg_t)(CC3XX_REG_BASE + 0x7C4)
#define CC3XX_REG_HASH_HASH_PAD_CFG                    (cc3xx_reg_t)(CC3XX_REG_BASE + 0x7C8)
#define CC3XX_REG_HASH_HASH_CUR_LEN_0                  (cc3xx_reg_t)(CC3XX_REG_BASE + 0x7CC)
#define CC3XX_REG_HASH_HASH_CUR_LEN_1                  (cc3xx_reg_t)(CC3XX_REG_BASE + 0x7D0)
#define CC3XX_REG_HASH_HASH_PARAM                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x7DC)

/* MISC registers */
#define CC3XX_REG_MISC_AES_CLK_ENABLE                  (cc3xx_reg_t)(CC3XX_REG_BASE + 0x810)
#define CC3XX_REG_MISC_HASH_CLK_ENABLE                 (cc3xx_reg_t)(CC3XX_REG_BASE + 0x818)
#define CC3XX_REG_MISC_PKA_CLK_ENABLE                  (cc3xx_reg_t)(CC3XX_REG_BASE + 0x81C)
#define CC3XX_REG_MISC_DMA_CLK_ENABLE                  (cc3xx_reg_t)(CC3XX_REG_BASE + 0x820)
#define CC3XX_REG_MISC_CLK_STATUS                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x824)
#define CC3XX_REG_MISC_CHACHA_CLK_ENABLE               (cc3xx_reg_t)(CC3XX_REG_BASE + 0x858)

/* CC_CTL registers */
#define CC3XX_REG_CC_CTL_CRYPTO_CTL                    (cc3xx_reg_t)(CC3XX_REG_BASE + 0x900)
#define CC3XX_REG_CC_CTL_CRYPTO_BUSY                   (cc3xx_reg_t)(CC3XX_REG_BASE + 0x910)
#define CC3XX_REG_CC_CTL_HASH_BUSY                     (cc3xx_reg_t)(CC3XX_REG_BASE + 0x91C)
#define CC3XX_REG_CC_CTL_CONTEXT_ID                    (cc3xx_reg_t)(CC3XX_REG_BASE + 0x930)

/* HOST_RGF registers */
#define CC3XX_REG_HOST_RGF_HOST_RGF_IRR                (cc3xx_reg_t)(CC3XX_REG_BASE + 0xA00)
#define CC3XX_REG_HOST_RGF_HOST_RGF_IMR                (cc3xx_reg_t)(CC3XX_REG_BASE + 0xA04)
#define CC3XX_REG_HOST_RGF_HOST_RGF_ICR                (cc3xx_reg_t)(CC3XX_REG_BASE + 0xA08)
#define CC3XX_REG_HOST_RGF_HOST_RGF_ENDIAN             (cc3xx_reg_t)(CC3XX_REG_BASE + 0xA0C)
#define CC3XX_REG_HOST_RGF_HOST_RGF_SIGNATURE          (cc3xx_reg_t)(CC3XX_REG_BASE + 0xA24)
#define CC3XX_REG_HOST_RGF_HOST_BOOT                   (cc3xx_reg_t)(CC3XX_REG_BASE + 0xA28)
#define CC3XX_REG_HOST_RGF_HOST_CRYPTOKEY_SEL          (cc3xx_reg_t)(CC3XX_REG_BASE + 0xA38)
#define CC3XX_REG_HOST_RGF_HOST_CORE_CLK_GATING_ENABLE (cc3xx_reg_t)(CC3XX_REG_BASE + 0xA78)
#define CC3XX_REG_HOST_RGF_HOST_CC_IS_IDLE             (cc3xx_reg_t)(CC3XX_REG_BASE + 0xA7C)
#define CC3XX_REG_HOST_RGF_HOST_POWERDOWN              (cc3xx_reg_t)(CC3XX_REG_BASE + 0xA80)
#define CC3XX_REG_HOST_RGF_HOST_REMOVE_GHASH_ENGINE    (cc3xx_reg_t)(CC3XX_REG_BASE + 0xA84)
#define CC3XX_REG_HOST_RGF_HOST_REMOVE_CHACHA_ENGINE   (cc3xx_reg_t)(CC3XX_REG_BASE + 0xA88)

/* AHB registers */
#define CC3XX_REG_AHB_AHBM_SINGLES                     (cc3xx_reg_t)(CC3XX_REG_BASE + 0xB00)
#define CC3XX_REG_AHB_AHBM_HPROT                       (cc3xx_reg_t)(CC3XX_REG_BASE + 0xB04)
#define CC3XX_REG_AHB_AHBM_HMASTLOCK                   (cc3xx_reg_t)(CC3XX_REG_BASE + 0xB08)
#define CC3XX_REG_AHB_AHBM_HNONSEC                     (cc3xx_reg_t)(CC3XX_REG_BASE + 0xB0C)

/* DIN registers */
#define CC3XX_REG_DIN_DIN_BUFFER                       (cc3xx_reg_t)(CC3XX_REG_BASE + 0xC00)
#define CC3XX_REG_DIN_DIN_MEM_DMA_BUSY                 (cc3xx_reg_t)(CC3XX_REG_BASE + 0xC20)
#define CC3XX_REG_DIN_SRC_LLI_WORD0                    (cc3xx_reg_t)(CC3XX_REG_BASE + 0xC28)
#define CC3XX_REG_DIN_SRC_LLI_WORD1                    (cc3xx_reg_t)(CC3XX_REG_BASE + 0xC2C)
#define CC3XX_REG_DIN_SRAM_SRC_ADDR                    (cc3xx_reg_t)(CC3XX_REG_BASE + 0xC30)
#define CC3XX_REG_DIN_DIN_SRAM_BYTES_LEN               (cc3xx_reg_t)(CC3XX_REG_BASE + 0xC34)
#define CC3XX_REG_DIN_DIN_SRAM_DMA_BUSY                (cc3xx_reg_t)(CC3XX_REG_BASE + 0xC38)
#define CC3XX_REG_DIN_DIN_SRAM_ENDIANNESS              (cc3xx_reg_t)(CC3XX_REG_BASE + 0xC3C)
#define CC3XX_REG_DIN_DIN_CPU_DATA_SIZE                (cc3xx_reg_t)(CC3XX_REG_BASE + 0xC48)
#define CC3XX_REG_DIN_FIFO_IN_EMPTY                    (cc3xx_reg_t)(CC3XX_REG_BASE + 0xC50)
#define CC3XX_REG_DIN_DIN_FIFO_RST_PNTR                (cc3xx_reg_t)(CC3XX_REG_BASE + 0xC58)

/* DOUT registers */
#define CC3XX_REG_DOUT_DOUT_BUFFER                     (cc3xx_reg_t)(CC3XX_REG_BASE + 0xD00)
#define CC3XX_REG_DOUT_DOUT_MEM_DMA_BUSY               (cc3xx_reg_t)(CC3XX_REG_BASE + 0xD20)
#define CC3XX_REG_DOUT_DST_LLI_WORD0                   (cc3xx_reg_t)(CC3XX_REG_BASE + 0xD28)
#define CC3XX_REG_DOUT_DST_LLI_WORD1                   (cc3xx_reg_t)(CC3XX_REG_BASE + 0xD2C)
#define CC3XX_REG_DOUT_SRAM_DEST_ADDR                  (cc3xx_reg_t)(CC3XX_REG_BASE + 0xD30)
#define CC3XX_REG_DOUT_DOUT_SRAM_BYTES_LEN             (cc3xx_reg_t)(CC3XX_REG_BASE + 0xD34)
#define CC3XX_REG_DOUT_DOUT_SRAM_DMA_BUSY              (cc3xx_reg_t)(CC3XX_REG_BASE + 0xD38)
#define CC3XX_REG_DOUT_DOUT_SRAM_ENDIANNESS            (cc3xx_reg_t)(CC3XX_REG_BASE + 0xD3C)
#define CC3XX_REG_DOUT_READ_ALIGN_LAST                 (cc3xx_reg_t)(CC3XX_REG_BASE + 0xD44)
#define CC3XX_REG_DOUT_DOUT_FIFO_EMPTY                 (cc3xx_reg_t)(CC3XX_REG_BASE + 0xD50)

/* AO registers */
#define CC3XX_REG_AO_HOST_DCU_EN0                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1E00)
#define CC3XX_REG_AO_HOST_DCU_EN1                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1E04)
#define CC3XX_REG_AO_HOST_DCU_EN2                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1E08)
#define CC3XX_REG_AO_HOST_DCU_EN3                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1E0C)
#define CC3XX_REG_AO_HOST_DCU_LOCK0                    (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1E10)
#define CC3XX_REG_AO_HOST_DCU_LOCK1                    (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1E14)
#define CC3XX_REG_AO_HOST_DCU_LOCK2                    (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1E18)
#define CC3XX_REG_AO_HOST_DCU_LOCK3                    (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1E1C)
#define CC3XX_REG_AO_AO_ICV_DCU_RESTRICTION_MASK0      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1E20)
#define CC3XX_REG_AO_AO_ICV_DCU_RESTRICTION_MASK1      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1E24)
#define CC3XX_REG_AO_AO_ICV_DCU_RESTRICTION_MASK2      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1E28)
#define CC3XX_REG_AO_AO_ICV_DCU_RESTRICTION_MASK3      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1E2C)
#define CC3XX_REG_AO_AO_CC_SEC_DEBUG_RESET             (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1E30)
#define CC3XX_REG_AO_HOST_AO_LOCK_BITS                 (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1E34)
#define CC3XX_REG_AO_AO_APB_FILTERING                  (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1E38)
#define CC3XX_REG_AO_AO_CC_GPPC                        (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1E3C)
#define CC3XX_REG_AO_HOST_RGF_CC_SW_RST                (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1E40)

/* NVM registers */
#define CC3XX_REG_NVM_AIB_FUSE_PROG_COMPLETED          (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1F04U)
#define CC3XX_REG_NVM_NVM_DEBUG_STATUS                 (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1F08U)
#define CC3XX_REG_NVM_LCS_IS_VALID                     (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1F0CU)
#define CC3XX_REG_NVM_NVM_IS_IDLE                      (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1F10U)
#define CC3XX_REG_NVM_LCS_REG                          (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1F14U)
#define CC3XX_REG_NVM_HOST_SHADOW_KDR_REG              (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1F18U)
#define CC3XX_REG_NVM_HOST_SHADOW_KCP_REG              (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1F1CU)
#define CC3XX_REG_NVM_HOST_SHADOW_KCE_REG              (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1F20U)
#define CC3XX_REG_NVM_HOST_SHADOW_KPICV_REG            (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1F24U)
#define CC3XX_REG_NVM_HOST_SHADOW_KCEICV_REG           (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1F28U)
#define CC3XX_REG_NVM_OTP_ADDR_WIDTH_DEF               (cc3xx_reg_t)(CC3XX_REG_BASE + 0x1F2CU)

#ifdef __cplusplus
}
#endif

#endif /* CC3XX_REG_DEFS_H */
