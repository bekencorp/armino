#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (NOT TFM_PARTITION_FIRMWARE_UPDATE)
    return()
endif()

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

add_library(tfm_psa_rot_partition_fwu STATIC)

target_include_directories(tfm_psa_rot_partition_fwu
    INTERFACE
        ${CMAKE_SOURCE_DIR}/secure_fw/include
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/firmware_update
)
target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/firmware_update
)

target_sources(tfm_psa_rot_partition_fwu
    PRIVATE
        tfm_fwu_req_mngr.c
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/firmware_update/auto_generated/intermedia_tfm_firmware_update.c
)
target_sources(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/firmware_update/auto_generated/load_info_tfm_firmware_update.c
)

# The bootloader specific configuration.
if ((NOT TFM_FWU_BOOTLOADER_LIB) OR (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bootloader/${TFM_FWU_BOOTLOADER_LIB}))
    message(FATAL_ERROR "TFM_FWU_BOOTLOADER_LIB invalid")
endif()
add_subdirectory(bootloader/${TFM_FWU_BOOTLOADER_LIB})

target_link_libraries(tfm_psa_rot_partition_fwu
    PRIVATE
        platform_s
        tfm_config
        tfm_sprt
)


# Generate firmware update feature
if (NOT FWU_DEVICE_CONFIG_FILE)
    configure_file(${CMAKE_SOURCE_DIR}/interface/include/psa/fwu_config.h.in
                   ${CMAKE_BINARY_DIR}/generated/interface/include/psa/fwu_config.h
                   @ONLY)
    set(FWU_DEVICE_CONFIG_FILE "${CMAKE_BINARY_DIR}/generated/interface/include/psa/fwu_config.h")
else()
    # FWU_DEVICE_CONFIG_FILE exists and is a file
    if(NOT EXISTS ${FWU_DEVICE_CONFIG_FILE})
        message(FATAL_ERROR "FWU_DEVICE_CONFIG_FILE:${FWU_DEVICE_CONFIG_FILE} does not exist.")
    elseif(IS_DIRECTORY ${FWU_DEVICE_CONFIG_FILE})
        message(FATAL_ERROR "FWU_DEVICE_CONFIG_FILE:${FWU_DEVICE_CONFIG_FILE} is a folder while a file is expected.")
    endif()
endif()


target_compile_definitions(tfm_psa_rot_partition_fwu
    PRIVATE
        $<$<BOOL:${DEFAULT_MCUBOOT_FLASH_MAP}>:DEFAULT_MCUBOOT_FLASH_MAP>
        FWU_DEVICE_CONFIG_FILE="${FWU_DEVICE_CONFIG_FILE}"
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_psa_rot_partition_fwu
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
    TFM_PARTITION_FIRMWARE_UPDATE
)

################ Display the configuration being applied #######################

include(utils)
dump_options("Firmware Update Partition config"
"
    TFM_FWU_BOOTLOADER_LIB;
    FWU_DEVICE_CONFIG_FILE
"
)
