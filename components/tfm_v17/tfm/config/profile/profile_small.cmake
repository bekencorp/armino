#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(TFM_PROFILE                profile_small CACHE STRING "Configuration profile")
set(PROJECT_CONFIG_HEADER_FILE "${CMAKE_SOURCE_DIR}/config/profile/config_profile_small.h" CACHE FILEPATH "User defined header file for TF-M config")

############################ SPM CONFIGURATION  ################################

set(TFM_ISOLATION_LEVEL                    1     CACHE STRING "Isolation level")
set(CONFIG_TFM_SPM_BACKEND                 "SFN" CACHE STRING "The SPM backend [IPC, SFN]")

############################ PARTITION CONFIGURATION  ##########################

set(TFM_PARTITION_CRYPTO                   ON    CACHE BOOL "Enable Crypto partition")
set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON    CACHE BOOL "Enable Internal Trusted Storage partition")
set(TFM_PARTITION_PLATFORM                 OFF   CACHE BOOL "Enable the TF-M Platform partition")
set(TFM_PARTITION_PROTECTED_STORAGE        OFF   CACHE BOOL "Enable Protected Storage partition")
set(TFM_PARTITION_INITIAL_ATTESTATION      ON    CACHE BOOL "Enable Initial Attestation partition")
set(SYMMETRIC_INITIAL_ATTESTATION          ON    CACHE BOOL "Use symmetric crypto for inital attestation")
set(TFM_PARTITION_FIRMWARE_UPDATE          OFF   CACHE BOOL "Enable firmware update partition")

################################## Advanced options #############################

set(PSA_FRAMEWORK_HAS_MM_IOVEC             ON    CACHE BOOL      "Enable MM-IOVEC")
set(MCUBOOT_IMAGE_NUMBER                   1     CACHE STRING    "Whether to combine S and NS into either 1 image, or sign each seperately")

################################## Dependencies ################################

set(TFM_MBEDCRYPTO_CONFIG_PATH             "${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/mbedcrypto_config/tfm_mbedcrypto_config_profile_small.h" CACHE PATH "Config to use for Mbed Crypto")
set(TFM_MBEDCRYPTO_PSA_CRYPTO_CONFIG_PATH  "${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/mbedcrypto_config/crypto_config_profile_small.h" CACHE PATH "Config to use psa crypto setting for Mbed Crypto.")
