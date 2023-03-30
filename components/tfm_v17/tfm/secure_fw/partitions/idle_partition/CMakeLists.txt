#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
# Copyright (c) 2021, Cypress Semiconductor Corp. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (NOT CONFIG_TFM_FLIH_API AND NOT CONFIG_TFM_SLIH_API AND
    NOT TFM_MULTI_CORE_TOPOLOGY)
    return()
endif()

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

target_sources(tfm_spm
    PRIVATE
        idle_partition.c
)

target_sources(tfm_partitions
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/load_info_idle_sp.c
)
