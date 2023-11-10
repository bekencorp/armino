#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (NOT TFM_PARTITION_PROTECTED_STORAGE)
    return()
endif()

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

add_library(tfm_app_rot_partition_ps STATIC)

target_include_directories(tfm_app_rot_partition_ps
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/protected_storage
)
target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/protected_storage
)

target_sources(tfm_app_rot_partition_ps
    PRIVATE
        tfm_ps_req_mngr.c
        tfm_protected_storage.c
        ps_object_system.c
        ps_object_table.c
        ps_utils.c
        $<$<BOOL:${PS_ENCRYPTION}>:crypto/ps_crypto_interface.c>
        $<$<BOOL:${PS_ENCRYPTION}>:ps_encrypted_object.c>
        # The test_ps_nv_counters.c will be used instead, when PS secure test is
        # ON and PS_TEST_NV_COUNTERS is ON
        $<$<NOT:$<AND:$<BOOL:${TEST_S_PS}>,$<BOOL:${PS_TEST_NV_COUNTERS}>>>:nv_counters/ps_nv_counters.c>
)

# The generated sources
target_sources(tfm_app_rot_partition_ps
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/protected_storage/auto_generated/intermedia_tfm_protected_storage.c
)
target_sources(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/protected_storage/auto_generated/load_info_tfm_protected_storage.c
)

target_link_libraries(tfm_app_rot_partition_ps
    PRIVATE
        secure_fw
        platform_s
        tfm_config
        tfm_sprt
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_app_rot_partition_ps
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
        TFM_PARTITION_PROTECTED_STORAGE
)
