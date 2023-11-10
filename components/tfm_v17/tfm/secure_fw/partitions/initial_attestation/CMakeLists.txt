#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (NOT TFM_PARTITION_INITIAL_ATTESTATION)
    return()
endif()

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

add_library(tfm_psa_rot_partition_attestation STATIC)

target_sources(tfm_psa_rot_partition_attestation PRIVATE
    tfm_attest.c
    tfm_attest_req_mngr.c
    attest_core.c
    attest_boot_data.c
    $<$<NOT:$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>>:attest_asymmetric_key.c>
    $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:attest_symmetric_key.c>
    attest_token_encode.c
)

# The generated sources
target_sources(tfm_psa_rot_partition_attestation
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/initial_attestation/auto_generated/intermedia_tfm_initial_attestation.c
)
target_sources(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/initial_attestation/auto_generated/load_info_tfm_initial_attestation.c
)

# Set include directory
target_include_directories(tfm_psa_rot_partition_attestation
    PUBLIC
        .
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/initial_attestation
)
target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/initial_attestation
)

target_link_libraries(tfm_psa_rot_partition_attestation
    PRIVATE
        platform_s
        tfm_config
        tfm_qcbor_s
        tfm_t_cose_s
        tfm_sprt
        tfm_boot_status
)

target_compile_definitions(tfm_psa_rot_partition_attestation
    PRIVATE
        $<$<BOOL:${ATTEST_INCLUDE_TEST_CODE}>:INCLUDE_TEST_CODE>
        $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:SYMMETRIC_INITIAL_ATTESTATION>
        $<$<NOT:$<BOOL:${PLATFORM_DEFAULT_ATTEST_HAL}>>:CLAIM_VALUE_CHECK>
        $<$<NOT:$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>>:ATTEST_KEY_BITS=${ATTEST_KEY_BITS}>
)

########################### Attest defs ########################################

add_library(tfm_attestation_defs INTERFACE)

target_include_directories(tfm_attestation_defs
    INTERFACE
        .
)

target_link_libraries(tfm_attestation_defs
    INTERFACE
        tfm_config
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_psa_rot_partition_attestation
)


target_compile_definitions(tfm_partition_defs
    INTERFACE
    TFM_PARTITION_INITIAL_ATTESTATION
)
