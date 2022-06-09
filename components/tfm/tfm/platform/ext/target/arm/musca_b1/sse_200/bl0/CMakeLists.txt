#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0076 NEW)
set(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

add_executable(bl0)

target_add_scatter_file(bl0
        $<$<C_COMPILER_ID:ARMClang>:${CMAKE_MUSCA_B1_SSE_200_SOURCE_DIR}/Device/Source/armclang/musca_bl0.sct>
        $<$<C_COMPILER_ID:GNU>:${CMAKE_MUSCA_B1_SSE_200_SOURCE_DIR}/Device/Source/gcc/musca_bl0.ld>
)

target_sources(bl0
    PRIVATE
        # Reuse BL2 startup cmsdk
        $<$<C_COMPILER_ID:ARMClang>:${CMAKE_MUSCA_B1_SSE_200_SOURCE_DIR}/Device/Source/armclang/startup_cmsdk_musca_bl2.s>
        $<$<C_COMPILER_ID:GNU>:${CMAKE_MUSCA_B1_SSE_200_SOURCE_DIR}/Device/Source/gcc/startup_cmsdk_musca_bl2.S>
        bl0_main.c
        ${CMAKE_MUSCA_B1_COMMON_DIR}/Native_Driver/mhu_v2_x.c
        ${CMAKE_MUSCA_B1_SSE_200_SOURCE_DIR}/Native_Driver/musca_b1_scc_drv.c
        ${CMAKE_MUSCA_B1_SSE_200_SOURCE_DIR}/Device/Source/device_definition.c
        ${CMAKE_MUSCA_B1_SSE_200_SOURCE_DIR}/Device/Source/system_core_init.c
)

target_include_directories(bl0
    PUBLIC
        ${CMAKE_MUSCA_B1_SSE_200_SOURCE_DIR}/Device/Include
    PRIVATE
        .
        ${CMAKE_MUSCA_B1_SSE_200_SOURCE_DIR}/Native_Driver
        ${CMAKE_MUSCA_B1_COMMON_DIR}/Native_Driver
)

target_include_directories(bl0
    PUBLIC
        ${CMAKE_SOURCE_DIR}/platform/ext/cmsis
        ${CMAKE_SOURCE_DIR}/platform/include
)

target_compile_definitions(bl0
    PUBLIC
        $<$<C_COMPILER_ID:GNU>:__STARTUP_CLEAR_BSS_MULTIPLE>
        $<$<C_COMPILER_ID:GNU>:__STARTUP_COPY_MULTIPLE>
)

set_target_properties(bl0
    PROPERTIES
        SUFFIX ".axf"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_convert_to_bin_target(bl0)
