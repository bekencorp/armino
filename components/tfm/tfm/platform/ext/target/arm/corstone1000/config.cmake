#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(BL1                                 ON         CACHE BOOL     "Whether to build BL1")
set(PLATFORM_DEFAULT_BL1                OFF        CACHE STRING   "Whether to use default BL1 or platform-specific one")
set(BL2                                 ON         CACHE BOOL     "Whether to build BL2")
set(BL2_TRAILER_SIZE                    0x800      CACHE STRING   "Trailer size")
set(DEFAULT_MCUBOOT_FLASH_MAP           OFF        CACHE BOOL     "Whether to use the default flash map defined by TF-M project")
set(MCUBOOT_UPGRADE_STRATEGY            "RAM_LOAD" CACHE STRING   "Upgrade strategy when multiple boot images are loaded")
set(MCUBOOT_SECURITY_COUNTER_S          "1"      CACHE STRING    "Security counter for S image. auto sets it to IMAGE_VERSION_S")

set(TFM_ISOLATION_LEVEL                 2          CACHE STRING   "Isolation level")
set(MCUBOOT_IMAGE_NUMBER                2          CACHE STRING   "Whether to combine S and NS into either 1 image, or sign each separately")
set(TFM_MULTI_CORE_TOPOLOGY             ON         CACHE BOOL     "Whether to build for a dual-cpu architecture")
set(TFM_PLAT_SPECIFIC_MULTI_CORE_COMM   ON         CACHE BOOL     "Whether to use a platform specific inter core communication instead of mailbox in dual-cpu topology")
set(CRYPTO_HW_ACCELERATOR               ON         CACHE BOOL     "Whether to enable the crypto hardware accelerator on supported platforms")
set(CRYPTO_NV_SEED                      OFF        CACHE BOOL     "Use stored NV seed to provide entropy")
set(TFM_CRYPTO_TEST_ALG_CFB             OFF        CACHE BOOL     "Test CFB cryptography mode")
set(NS                                  FALSE      CACHE BOOL     "Whether to build NS app")

# FVP is not integrated/tested with CC312.
if (${PLATFORM_IS_FVP})
  set(PLATFORM_DEFAULT_OTP              TRUE      CACHE BOOL      "Use trusted on-chip flash to implement OTP memory")
else()
  set(PLATFORM_DEFAULT_OTP              FALSE      CACHE BOOL      "Use trusted on-chip flash to implement OTP memory")
endif()

# External dependency on OpenAMP and Libmetal
set(LIBMETAL_SRC_PATH                   "DOWNLOAD"  CACHE PATH      "Path to Libmetal (or DOWNLOAD to fetch automatically")
set(LIBMETAL_VERSION                    "f252f0e007fbfb8b3a52b1d5901250ddac96baad"  CACHE STRING    "The version of libmetal to use")

set(LIBOPENAMP_SRC_PATH                 "DOWNLOAD"  CACHE PATH      "Path to Libopenamp (or DOWNLOAD to fetch automatically")
set(OPENAMP_VERSION                     "347397decaa43372fc4d00f965640ebde042966d"  CACHE STRING    "The version of openamp to use")

if (${PLATFORM_IS_FVP})
    set(PLATFORM_PSA_ADAC_SECURE_DEBUG      FALSE        CACHE BOOL      "Whether to use psa-adac secure debug.")
else()
    set(PLATFORM_PSA_ADAC_SECURE_DEBUG      FALSE        CACHE BOOL      "Whether to use psa-adac secure debug.")
endif()

set(DEFAULT_MCUBOOT_SECURITY_COUNTERS   OFF          CACHE BOOL      "Whether to use the default security counter configuration defined by TF-M project")

set(PS_ENCRYPTION                       OFF          CACHE BOOL      "Enable encryption for Protected Storage partition")
set(PS_ROLLBACK_PROTECTION              OFF          CACHE BOOL      "Enable rollback protection for Protected Storage partition")
