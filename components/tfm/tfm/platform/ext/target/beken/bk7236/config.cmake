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
set(TEST_NS_SLIH_IRQ                 OFF   CACHE BOOL    "Whether to build NS regression Second-Level Interrupt Handling tests")

set(CRYPTO_HW_ACCELERATOR            ON         CACHE BOOL      "Whether to enable the crypto hardware accelerator on supported platforms")
set(CRYPTO_NV_SEED                   OFF        CACHE BOOL      "Use stored NV seed to provide entropy")
set(BL2                              ON         CACHE BOOL      "Whether to build BL2")
set(DEFAULT_MCUBOOT_FLASH_MAP        OFF        CACHE BOOL      "Whether to use the default flash map defined by TF-M project")
set(FPGA                             OFF        CACHE BOOL      "Build for FPGA")

set(TFM_PARTITION_FIRMWARE_UPDATE    OFF        CACHE BOOL      "Enable firmware update partition")
if(TFM_PARTITION_FIRMWARE_UPDATE)
set(CONFIG_TFM_FP                     hard  CACHE STRING "Whether to support float, and set the type")
set(CONFIG_TFM_FP_ARCH                fpv5-sp-d16 CACHE STRING "float processing unit arch version")
set(MCUBOOT_DATA_SHARING             ON         CACHE BOOL      "Enable Data Sharing")
set(MCUBOOT_IMAGE_NUMBER             1          CACHE STRING    "Whether to combine S and NS into either 1 image, or sign each seperately")
set(PSA_FWU_MAX_BLOCK_SIZE           2048       CACHE STRING    "The maximum permitted size for block in psa_fwu_write, in bytes.")
set(NS                               OFF        CACHE BOOL      "Whether to build NS app")
endif()

if(BL2)
    #At least 1 flash sector for header/trailer
    set(BL2_HEADER_SIZE  0x1000 CACHE STRING "Header size")
    set(BL2_TRAILER_SIZE 0x1000 CACHE STRING "Trailer size")
else()
    #No header if no bootloader, but keep IMAGE_CODE_SIZE the same
    set(BL2_TRAILER_SIZE 0x2000 CACHE STRING "Trailer size")
endif()

include(platform/ext/target/beken/bk7236/repo_fetch.cmake)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DCONFIG_BUILD_DEBUG)
endif()

if(FPGA)
    add_definitions(-DCONFIG_FPGA)
endif()
