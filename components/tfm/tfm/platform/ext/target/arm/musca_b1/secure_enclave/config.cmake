#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
set(BL2_TRAILER_SIZE                    0x800        CACHE STRING    "Trailer size")

# Configuration values forced due to the Secure Enclave topology
set(TFM_MULTI_CORE_TOPOLOGY             ON           CACHE BOOL      "Whether to build for a dual-cpu architecture" FORCE)
set(NS                                  FALSE        CACHE BOOL      "Whether to build NS app" FORCE)
set(TEST_NS                             OFF          CACHE BOOL      "Whether to build NS regression tests" FORCE)

# Serial output is not available for Secure Enclave
set(PLATFORM_DEFAULT_UART_STDOUT        OFF          CACHE BOOL      "Use default uart stdout implementation." FORCE)
set(MCUBOOT_LOG_LEVEL                   "NONE"       CACHE STRING    "Level of logging to use for MCUboot [OFF, ERROR, WARNING, INFO, DEBUG]" FORCE)

# Test services are inaccessible via Proxy service
set(TEST_S                              OFF          CACHE BOOL      "Whether to build S regression tests" FORCE)

# Currently only level 1 isolation is supported
set(TFM_ISOLATION_LEVEL                 1            CACHE STRING    "Isolation level" FORCE)

# Crypto hardware accelerator is turned on by default
set(CRYPTO_HW_ACCELERATOR               ON           CACHE BOOL      "Whether to enable the crypto hardware accelerator on supported platforms")
set(CRYPTO_NV_SEED                      OFF          CACHE BOOL      "Use stored NV seed to provide entropy")

set(PSA_API_TEST_TARGET                 "musca_b1"   CACHE STRING    "Target to use when building the PSA API tests")

# Secure Enclave topology does not require more than 3 connenction handles
set(CONFIG_TFM_CONN_HANDLE_MAX_NUM      3           CACHE STRING    "The maximal number of secure services that are connected or requested at the same time" FORCE)
