#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(TFM_EXTRA_GENERATED_FILE_LIST_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/platform/ext/target/beken/bk7236/generated_file_list.yaml  CACHE PATH "Path to extra generated file list. Appended to stardard TFM generated file list." FORCE)

set(PLATFORM_SLIH_IRQ_TEST_SUPPORT    ON    CACHE BOOL    "Platform supports SLIH IRQ tests")
set(PLATFORM_FLIH_IRQ_TEST_SUPPORT    ON    CACHE BOOL    "Platform supports FLIH IRQ tests")

# Make FLIH IRQ test as the default IRQ test on BK7236
set(TEST_NS_SLIH_IRQ                  OFF   CACHE BOOL    "Whether to build NS regression Second-Level Interrupt Handling tests")

set(CRYPTO_HW_ACCELERATOR               ON          CACHE BOOL      "Whether to enable the crypto hardware accelerator on supported platforms")
set(CRYPTO_NV_SEED                      OFF         CACHE BOOL      "Use stored NV seed to provide entropy")
set(BL2                                 ON         CACHE BOOL      "Whether to build BL2")

if(BL2)
    set(BL2_TRAILER_SIZE 0x10000 CACHE STRING "Trailer size")
else()
    #No header if no bootloader, but keep IMAGE_CODE_SIZE the same
    set(BL2_TRAILER_SIZE 0x10400 CACHE STRING "Trailer size")
endif()

include(platform/ext/target/beken/bk7236/repo_fetch.cmake)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DCONFIG_BUILD_DEBUG)
endif()
