#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# Set to not download submodules if that option is available
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.16.0")
    cmake_policy(SET CMP0097 NEW)
endif()

if ("${CY_MTB_PDL_CAT1_LIB_PATH}" STREQUAL "DOWNLOAD")
    find_package(Git)
    file(GLOB PATCH_FILES *.patch)

    if (PATCH_FILES)
        FetchContent_Declare(mtb-pdl-cat1
            GIT_REPOSITORY https://github.com/Infineon/mtb-pdl-cat1.git
            GIT_TAG ${CY_MTB_PDL_CAT1_LIB_VERSION}
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
            GIT_SUBMODULES ""
            PATCH_COMMAND ${GIT_EXECUTABLE} apply ${PATCH_FILES}
        )
    else()
        FetchContent_Declare(mtb-pdl-cat1
            GIT_REPOSITORY https://github.com/Infineon/mtb-pdl-cat1.git
            GIT_TAG ${CY_MTB_PDL_CAT1_LIB_VERSION}
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
            GIT_SUBMODULES ""
        )
    endif()

    FetchContent_GetProperties(mtb-pdl-cat1)
    if(NOT mtb-pdl-cat1_POPULATED)
        FetchContent_Populate(mtb-pdl-cat1)
        set(CY_MTB_PDL_CAT1_LIB_PATH ${mtb-pdl-cat1_SOURCE_DIR} CACHE PATH "Path to mtb-pdl-cat1 (or DOWNLOAD to get automatically" FORCE)
    endif()
endif()

#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.13)

project("Cypress PSoC 6 Peripheral Driver Library" LANGUAGES C ASM)

cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0079 NEW)

if (NOT DEFINED MTB_PDL_CAT1_LIB_S_TARGET)
    set(MTB_PDL_CAT1_LIB_S_TARGET mtb-pdl-cat1_lib_s)
endif()
if (NOT DEFINED MTB_PDL_CAT1_LIB_NS_TARGET)
    set(MTB_PDL_CAT1_LIB_NS_TARGET mtb-pdl-cat1_lib_ns)
endif()

set(CY_PLATFORM_DIR         ${PLATFORM_DIR}/ext/target/${TFM_PLATFORM})
set(CY_CORE_LIB_PATH        ${CY_PLATFORM_DIR}/libs/core-lib)

add_subdirectory(mtb-pdl-cat1_s_lib)
add_subdirectory(mtb-pdl-cat1_ns_lib)
