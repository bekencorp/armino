#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
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

if ("${LIBMETAL_SRC_PATH}" STREQUAL "DOWNLOAD")
    FetchContent_Declare(libmetal
        GIT_REPOSITORY https://github.com/OpenAMP/libmetal.git
        GIT_TAG ${LIBMETAL_VERSION}
    )

    FetchContent_GetProperties(libmetal)
    if (NOT libmetal_POPULATED)
        FetchContent_Populate(libmetal)
        set(LIBMETAL_BIN_PATH ${libmetal_BINARY_DIR} CACHE PATH "Path to Libmetal (or DOWNLOAD to get automatically")
        set(LIBMETAL_SRC_PATH ${libmetal_SOURCE_DIR} CACHE PATH "Path to Libmetal (or DOWNLOAD to get automatically" FORCE)
    endif ()
endif ()

