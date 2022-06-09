#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
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

if ("${LIBOPENAMP_SRC_PATH}" STREQUAL "DOWNLOAD")
    FetchContent_Declare(libopenamp
        GIT_REPOSITORY https://github.com/OpenAMP/open-amp.git
        GIT_TAG ${OPENAMP_VERSION}
    )

    FetchContent_GetProperties(libopenamp)
    if (NOT libopenamp_POPULATED)
        FetchContent_Populate(libopenamp)
        set(LIBOPENAMP_BIN_PATH ${libopenamp_BINARY_DIR} CACHE PATH "Path to Libopenamp (or DOWNLOAD to get automatically")
        set(LIBOPENAMP_SRC_PATH ${libopenamp_SOURCE_DIR} CACHE PATH "Path to Libopenamp (or DOWNLOAD to get automatically" FORCE)
    endif ()
endif ()

