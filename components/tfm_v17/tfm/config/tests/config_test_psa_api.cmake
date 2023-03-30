#------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#------------------------------------------------------------------------------

############ Override defaults for PSA API tests ##############################
if ("${TEST_PSA_API}" STREQUAL "IPC")
    # PSA Arch test partitions only support IPC model so far
    set(CONFIG_TFM_SPM_BACKEND      "IPC"       CACHE STRING    "The SPM backend [IPC, SFN]")
    set(TFM_PARTITION_FF_TEST   ON)
else()
    set(TFM_PARTITION_FF_TEST   OFF)
endif()

if ("${TEST_PSA_API}" STREQUAL "INITIAL_ATTESTATION")
    set(TFM_PARTITION_INITIAL_ATTESTATION      ON       CACHE BOOL      "Enable Initial Attestation partition")
    set(TFM_PARTITION_CRYPTO                   ON       CACHE BOOL      "Enable Crypto partition")
    set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
    set(TFM_PARTITION_PLATFORM                 ON       CACHE BOOL      "Enable Platform partition")
endif()

if ("${TEST_PSA_API}" STREQUAL "CRYPTO")
    set(TFM_PARTITION_CRYPTO                   ON       CACHE BOOL      "Enable Crypto partition")
    set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
    set(TFM_PARTITION_PLATFORM                 ON       CACHE BOOL      "Enable Platform partition")
endif()

if ("${TEST_PSA_API}" STREQUAL "STORAGE")
    set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
    set(TFM_PARTITION_PROTECTED_STORAGE        ON       CACHE BOOL      "Enable Protected Storage partition")
    set(TFM_PARTITION_CRYPTO                   ON       CACHE BOOL      "Enable Crypto partition")
    set(TFM_PARTITION_PLATFORM                 ON       CACHE BOOL      "Enable Platform partition")
endif()

target_compile_definitions(tfm_config
    INTERFACE
        $<$<STREQUAL:${TEST_PSA_API},CRYPTO>:TEST_PSA_API_CRYPTO>
        $<$<STREQUAL:${TEST_PSA_API},IPC>:TEST_PSA_API_IPC>
)

set(PROJECT_CONFIG_HEADER_FILE  "${CMAKE_SOURCE_DIR}/config/tests/config_test_psa_api.h" CACHE FILEPATH "User defined header file for TF-M config")
