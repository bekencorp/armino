#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if(TEST_NS OR TEST_S)
    # Enable default Secure Partition for tests when secure or non-secure regressions are enabled

    set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
    set(TFM_PARTITION_PROTECTED_STORAGE        ON       CACHE BOOL      "Enable Protected Storage partition")
    set(TFM_PARTITION_CRYPTO                   ON       CACHE BOOL      "Enable Crypto partition")
    set(TFM_PARTITION_INITIAL_ATTESTATION      ON       CACHE BOOL      "Enable Initial Attestation partition")
    set(TFM_PARTITION_PLATFORM                 ON       CACHE BOOL      "Enable Platform partition")

    if(PLATFORM_HAS_FIRMWARE_UPDATE_SUPPORT)
        set(TFM_PARTITION_FIRMWARE_UPDATE      ON       CACHE BOOL      "Enable firmware update partition")
    endif()
endif()

if(TEST_S_CRYPTO OR TEST_NS_CRYPTO)
    set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
    set(TFM_PARTITION_CRYPTO                   ON       CACHE BOOL      "Enable Crypto partition")
endif()

if(TEST_S_PS OR TEST_NS_PS)
    set(TFM_PARTITION_PROTECTED_STORAGE        ON       CACHE BOOL      "Enable Protected Storage partition")
    set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
    set(TFM_PARTITION_CRYPTO                   ON       CACHE BOOL      "Enable Crypto partition")
    set(TFM_PARTITION_PLATFORM                 ON       CACHE BOOL      "Enable Platform partition")

    # TEST_NS_PS relies on TEST_NS_ITS
    if(TEST_NS_PS)
        set(TEST_NS_ITS                        ON       CACHE BOOL      "Whether to build NS regression ITS tests")
    endif()
endif()

if(TEST_S_ITS OR TEST_NS_ITS)
    set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
endif()

if(TEST_S_ATTESTATION OR TEST_NS_ATTESTATION OR TEST_NS_T_COSE OR TEST_NS_QCBOR)
    set(TFM_PARTITION_INITIAL_ATTESTATION      ON       CACHE BOOL      "Enable Initial Attestation partition")
    set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
    set(TFM_PARTITION_CRYPTO                   ON       CACHE BOOL      "Enable Crypto partition")
endif()

if(TEST_S_PLATFORM OR TEST_NS_PLATFORM)
    set(TFM_PARTITION_PLATFORM                 ON       CACHE BOOL      "Enable Platform partition")
endif()

if((TEST_S_FWU OR TEST_NS_FWU) AND PLATFORM_HAS_FIRMWARE_UPDATE_SUPPORT)
    set(TFM_PARTITION_FIRMWARE_UPDATE          ON       CACHE BOOL      "Enable firmware update partition")
    set(TFM_PARTITION_CRYPTO                   ON       CACHE BOOL      "Enable Crypto partition")
    set(TFM_PARTITION_PLATFORM                 ON       CACHE BOOL      "Enable Platform partition")
    set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
endif()

if(TEST_NS_MANAGE_NSID AND NOT TFM_MULTI_CORE_TOPOLOGY)
    set(TFM_NS_MANAGE_NSID                     ON       CACHE BOOL      "Support NSPE OS providing NSPE client_id")
endif()
