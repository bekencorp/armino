#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0079 NEW)

if (EXISTS ${CMAKE_SOURCE_DIR}/platform/ext/target/${TFM_PLATFORM}/preload_ns.cmake)
    include(${CMAKE_SOURCE_DIR}/platform/ext/target/${TFM_PLATFORM}/preload_ns.cmake)
    tfm_toolchain_reload_compiler()
endif()

add_library(platform_ns STATIC EXCLUDE_FROM_ALL)

target_sources(platform_ns
    PRIVATE
        $<$<BOOL:${PLATFORM_DEFAULT_UART_STDOUT}>:${CMAKE_CURRENT_SOURCE_DIR}/../ext/common/uart_stdout.c>
        $<$<AND:$<NOT:$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>>,$<BOOL:${TEST_NS_ATTESTATION}>>:${CMAKE_CURRENT_SOURCE_DIR}/../ext/common/template/tfm_initial_attest_pub_key.c>
    PUBLIC
        $<$<OR:$<BOOL:${TEST_S_FPU}>,$<BOOL:${TEST_NS_FPU}>>:${CMAKE_SOURCE_DIR}/platform/ext/common/test_interrupt.c>
)

target_link_libraries(platform_ns
    PUBLIC
        platform_common_interface
        platform_region_defs
        tfm_fih_headers
        tfm_ns_interface
)

target_compile_definitions(platform_ns
    PUBLIC
        $<$<STREQUAL:${CONFIG_TFM_FLOAT_ABI},hard>:CONFIG_TFM_FLOAT_ABI=2>
        $<$<STREQUAL:${CONFIG_TFM_FLOAT_ABI},soft>:CONFIG_TFM_FLOAT_ABI=0>
        $<$<BOOL:${TEST_NS_FPU}>:TEST_NS_FPU>
        $<$<BOOL:${CONFIG_TFM_ENABLE_CP10CP11}>:CONFIG_TFM_ENABLE_CP10CP11>
        $<$<BOOL:${PLATFORM_DEFAULT_CRYPTO_KEYS}>:PLATFORM_DEFAULT_CRYPTO_KEYS>
    PRIVATE
        $<$<BOOL:${TEST_NS_SLIH_IRQ}>:TEST_NS_SLIH_IRQ>
)

target_compile_options(platform_ns
    PUBLIC
        ${COMPILER_CP_FLAG}
)
