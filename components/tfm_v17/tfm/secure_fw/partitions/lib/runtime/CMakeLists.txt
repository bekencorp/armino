#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

add_library(tfm_sprt STATIC)

target_include_directories(tfm_sprt
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${CMAKE_SOURCE_DIR}/secure_fw/include
)

target_sources(tfm_sprt
    PRIVATE
        ./crt_memcmp.c
        ./crt_memmove.c
        ./crt_strnlen.c
        ./service_api.c
        ${CMAKE_SOURCE_DIR}/secure_fw/shared/crt_memcpy.c
        ${CMAKE_SOURCE_DIR}/secure_fw/shared/crt_memset.c
        $<$<BOOL:${CONFIG_TFM_PARTITION_META}>:./sprt_partition_metadata_indicator.c>
        $<$<BOOL:${CONFIG_TFM_PARTITION_META}>:./rt_main.c>
        $<$<BOOL:${CONFIG_TFM_SPM_BACKEND_IPC}>:./sfn_common_thread.c>
        $<$<BOOL:${TFM_SP_LOG_RAW_ENABLED}>:./tfm_sp_log_raw.c>
        $<$<BOOL:${TFM_SP_LOG_RAW_ENABLED}>:${CMAKE_SOURCE_DIR}/platform/ext/common/tfm_hal_sp_logdev_periph.c>
)

target_link_libraries(tfm_sprt
    PUBLIC
        psa_interface
        tfm_spm_defs
        tfm_partition_defs
        platform_s
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
        TFM_PARTITION_LOG_LEVEL=${TFM_PARTITION_LOG_LEVEL}
        $<$<BOOL:${TFM_SP_LOG_RAW_ENABLED}>:TFM_SP_LOG_RAW_ENABLED>
)

target_include_directories(tfm_partitions
    INTERFACE
        $<BUILD_INTERFACE:$<$<BOOL:${CONFIG_TFM_PARTITION_META}>:${CMAKE_CURRENT_SOURCE_DIR}/include>>
)
