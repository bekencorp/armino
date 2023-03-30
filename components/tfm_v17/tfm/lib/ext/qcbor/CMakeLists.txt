#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# Default configuration of QCBOR repository
set(QCBOR_PATH      "DOWNLOAD"  CACHE PATH      "Path to qcbor (or DOWNLOAD to fetch automatically")
set(QCBOR_VERSION   "b0e70332"  CACHE STRING    "The version of qcbor to use")

if ("${QCBOR_PATH}" STREQUAL "DOWNLOAD")
    FetchContent_Declare(qcbor
        GIT_REPOSITORY https://github.com/laurencelundblade/QCBOR.git
        GIT_TAG ${QCBOR_VERSION}
        GIT_PROGRESS TRUE
    )

    FetchContent_GetProperties(qcbor)
    if(NOT qcbor_POPULATED)
        FetchContent_Populate(qcbor)
        set(QCBOR_PATH ${qcbor_SOURCE_DIR} CACHE PATH "Path to qcbor (or DOWNLOAD to get automatically" FORCE)
    endif()
endif()

add_library(tfm_qcbor_s STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_qcbor_s
    PRIVATE
        ${QCBOR_PATH}/src/ieee754.c
        ${QCBOR_PATH}/src/qcbor_encode.c
        ${QCBOR_PATH}/src/qcbor_decode.c
        ${QCBOR_PATH}/src/UsefulBuf.c
)

target_compile_definitions(tfm_qcbor_s
    PRIVATE
        QCBOR_DISABLE_FLOAT_HW_USE
)

target_include_directories(tfm_qcbor_s
    PUBLIC
        $<BUILD_INTERFACE:${QCBOR_PATH}/inc>
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${QCBOR_PATH}/inc/qcbor>
)

target_link_libraries(tfm_qcbor_s
    PRIVATE
        tfm_attestation_defs
        tfm_t_cose_defs
)

target_compile_options(tfm_qcbor_s
    PRIVATE
        ${COMPILER_CP_FLAG}
)
