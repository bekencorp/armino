#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(TFM_PROFILE                profile_medium_arotless CACHE STRING    "Profile to use")
set(PROJECT_CONFIG_HEADER_FILE "${CMAKE_SOURCE_DIR}/config/profile/config_profile_medium_arotless.h" CACHE FILEPATH "User defined header file for TF-M config")

############################ SPM CONFIGURATION  ################################

set(CONFIG_TFM_SPM_BACKEND              "SFN"       CACHE STRING    "The SPM backend [IPC, SFN]")
set(TFM_ISOLATION_LEVEL                 1           CACHE STRING    "Isolation level")

############################ Partitions ########################################

set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
set(TFM_PARTITION_CRYPTO                ON          CACHE BOOL      "Enable Crypto partition")
set(TFM_PARTITION_PLATFORM              ON          CACHE BOOL      "Enable the TF-M Platform partition")
set(TFM_PARTITION_INITIAL_ATTESTATION   ON          CACHE BOOL      "Enable Initial Attestation partition")
set(TFM_PARTITION_FIRMWARE_UPDATE       ON          CACHE BOOL      "Enable firmware update partition")
set(TFM_PARTITION_PROTECTED_STORAGE     OFF         CACHE BOOL      "Enable Protected Storage partition")

################################## Dependencies ################################

set(TFM_MBEDCRYPTO_CONFIG_PATH              "${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/mbedcrypto_config/tfm_mbedcrypto_config_profile_medium.h" CACHE PATH "Config to use for Mbed Crypto")
set(TFM_MBEDCRYPTO_PSA_CRYPTO_CONFIG_PATH   "${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/mbedcrypto_config/crypto_config_profile_medium.h" CACHE PATH "Config to use psa crypto setting for Mbed Crypto.")

# If the platform doesn't support FWU, stop the configuration right now to give users a more
# specific error message, rather than letting check_config step to throw out a general error
# description.
if(NOT PLATFORM_HAS_FIRMWARE_UPDATE_SUPPORT)
    message(FATAL_ERROR "${TFM_PLATFORM} doesn't support FWU. Fail to select Profile Medium-ARoT-less")
endif()
