#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

# CMake 3.21 and above requests projects to specify cpu/arch compile and link flags explicitly in
# Armclang. Link: https://cmake.org/cmake/help/latest/policy/CMP0123.html
# It is aligned with current Armclang toolchain implementation.
# Explictly set this policy to NEW behavior to eliminate long warnings. It shall be set in root
# CMakeLists.txt otherwise project() will throw out the warnings.
if(POLICY CMP0123)
    cmake_policy(SET CMP0123 NEW)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(cmake/version.cmake)
include(remote_library)

############################ CONFIGURATION #####################################

# Configure TFM_PLATFORM
include(${CMAKE_SOURCE_DIR}/config/tfm_platform.cmake)

if(TFM_SYSTEM_DSP)
    message(FATAL_ERROR "Hardware DSP is currently not supported in TF-M")
endif()

include(config/set_config.cmake)

if(NOT ${CMAKE_GENERATOR} STREQUAL "Unix Makefiles" AND
   NOT ${CMAKE_GENERATOR} STREQUAL "Ninja")
    Message(FATAL_ERROR "Unsupported generator ${CMAKE_GENERATOR}. Hint: Try -G\"Unix Makefiles\"")
endif()

############################### Compiler configuration #########################

include(${TFM_TOOLCHAIN_FILE})
set(CMAKE_PROJECT_INCLUDE_BEFORE ${CMAKE_SOURCE_DIR}/cmake/disable_compiler_detection.cmake)

project("Trusted Firmware M" VERSION ${TFM_VERSION} LANGUAGES C ASM)
tfm_toolchain_reload_compiler()

# Synchronise the install path variables. If CMAKE_INSTALL_PREFIX is manually
# set then set both to the value of that, else set both to the value of
# TFM_INSTALL_PATH. This has to be done after the call to `project()`.
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX  ${TFM_INSTALL_PATH} CACHE PATH  "" FORCE)
else()
    set(TFM_INSTALL_PATH  ${CMAKE_INSTALL_PREFIX} CACHE PATH  "Path to which to install TF-M files" FORCE)
endif()

add_subdirectory(lib/ext)
add_subdirectory(lib/fih)
add_subdirectory(tools)
add_subdirectory(secure_fw)

if(NS AND NS_EVALUATION_APP_PATH)
    add_subdirectory(${NS_EVALUATION_APP_PATH} ${CMAKE_CURRENT_BINARY_DIR}/evaluation-app)
elseif(NS OR TFM_S_REG_TEST OR TFM_NS_REG_TEST OR TEST_BL2 OR TEST_BL1_1 OR TEST_BL1_2)
    add_subdirectory(${TFM_TEST_REPO_PATH} ${CMAKE_CURRENT_BINARY_DIR}/tf-m-tests)
endif()

add_subdirectory(interface)
if(BL2)
    add_subdirectory(bl2)
endif()

if(BL1 AND PLATFORM_DEFAULT_BL1)
    add_subdirectory(bl1/bl1_2)
    add_subdirectory(bl1/bl1_1)
endif()

add_subdirectory(platform)

if(CRYPTO_HW_ACCELERATOR)
    add_subdirectory(platform/ext/accelerator)
endif()

############################ Config Check ######################################

include(${CMAKE_SOURCE_DIR}/config/check_config.cmake)

################################################################################

include(cmake/install.cmake)
