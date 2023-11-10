#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (NOT TFM_PARTITION_INTERNAL_TRUSTED_STORAGE)
    return()
endif()

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

add_library(tfm_psa_rot_partition_its STATIC)

target_include_directories(tfm_psa_rot_partition_its
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/internal_trusted_storage
    PUBLIC
        # Required for ps_object_defs.h
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../protected_storage
)
target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/internal_trusted_storage
)

target_sources(tfm_psa_rot_partition_its
    PRIVATE
        tfm_its_req_mngr.c
        tfm_internal_trusted_storage.c
        its_utils.c
        flash/its_flash.c
        flash/its_flash_nand.c
        flash/its_flash_nor.c
        flash/its_flash_ram.c
        flash_fs/its_flash_fs.c
        flash_fs/its_flash_fs_dblock.c
        flash_fs/its_flash_fs_mblock.c
)

# The generated sources
target_sources(tfm_psa_rot_partition_its
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/internal_trusted_storage/auto_generated/intermedia_tfm_internal_trusted_storage.c
)
target_sources(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/internal_trusted_storage/auto_generated/load_info_tfm_internal_trusted_storage.c
)

target_link_libraries(tfm_psa_rot_partition_its
    PRIVATE
        platform_s
        tfm_config
        tfm_sprt
)

target_compile_definitions(tfm_psa_rot_partition_its
    PUBLIC
        PS_CRYPTO_AEAD_ALG=${PS_CRYPTO_AEAD_ALG}
)

################ Display the configuration being applied #######################

include(utils)
    dump_options("Storage config"
    "
        PS_CRYPTO_AEAD_ALG;
    "
    )

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_psa_rot_partition_its
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
        TFM_PARTITION_INTERNAL_TRUSTED_STORAGE
)
