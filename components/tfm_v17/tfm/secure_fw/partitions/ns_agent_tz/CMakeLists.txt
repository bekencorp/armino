#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
# Copyright (c) 2021-2022 Cypress Semiconductor Corporationn (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (NOT TFM_PARTITION_NS_AGENT_TZ)
    return()
endif()

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

target_sources(tfm_partitions
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/load_info_ns_agent_tz.c
)

set(ARM_V80M_ARCH armv8-m.base armv8-m.main)

# Trustzone NS Agent is tighly coupled with SPM
target_sources(tfm_spm
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ns_agent_tz_init.c
        "$<$<IN_LIST:${TFM_SYSTEM_ARCHITECTURE},${ARM_V80M_ARCH}>:${CMAKE_CURRENT_SOURCE_DIR}/ns_agent_tz_v80m.c>"
        "$<$<NOT:$<IN_LIST:${TFM_SYSTEM_ARCHITECTURE},${ARM_V80M_ARCH}>>:${CMAKE_CURRENT_SOURCE_DIR}/ns_agent_tz.c>"
)

# If this is added to the spm, it is discarded as it is not used. Since the
# spm is a static library it can't generate veneers under all compilers so
# instead this single file is added to the 'tfm_s' target.
target_sources(tfm_s
    PRIVATE
        "$<$<IN_LIST:${TFM_SYSTEM_ARCHITECTURE},${ARM_V80M_ARCH}>:${CMAKE_CURRENT_SOURCE_DIR}/psa_api_veneers_v80m.c>"
        "$<$<NOT:$<IN_LIST:${TFM_SYSTEM_ARCHITECTURE},${ARM_V80M_ARCH}>>:${CMAKE_CURRENT_SOURCE_DIR}/psa_api_veneers.c>"
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
        TFM_PARTITION_NS_AGENT_TZ
)
