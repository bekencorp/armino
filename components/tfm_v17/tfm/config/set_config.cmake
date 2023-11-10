#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# The Configuration sequence is captured in the documentation, in
# docs/getting_started/tfm_build_instructions.rst under Cmake Configuration. If
# the sequence is updated here the docs must also be updated.

# The default build type is MinSizeRel. If debug symbols are needed then
# -DCMAKE_BUILD_TYPE=debug should be used (likewise with other build types)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "MinSizeRel" CACHE STRING "Build type: [Debug, Release, RelWithDebInfo, MinSizeRel]" FORCE)
endif()

if (TFM_LIB_MODEL)
    message(FATAL_ERROR "Library Model is deprecated, please DO NOT use TFM_LIB_MODEL anymore."
                        "SFN model is a replacement for Library Model. You can use -DCONFIG_TFM_SPM_BACKEND=SFN to select SFN model.")
endif()

# Load extra config
if (TFM_EXTRA_CONFIG_PATH)
    include(${TFM_EXTRA_CONFIG_PATH})
endif()

# Set TF-M project config header file
add_library(tfm_config INTERFACE)

# Load PSA config, setting options not already set
if (TEST_PSA_API)
    include(config/tests/config_test_psa_api.cmake)
endif()

# Load build type config, setting options not already set
string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWERCASE)
if (EXISTS ${CMAKE_SOURCE_DIR}/config/build_type/${CMAKE_BUILD_TYPE_LOWERCASE}.cmake)
    include(${CMAKE_SOURCE_DIR}/config/build_type/${CMAKE_BUILD_TYPE_LOWERCASE}.cmake)
endif()

# Load platform config, setting options not already set
if (EXISTS ${CMAKE_SOURCE_DIR}/platform/ext/target/${TFM_PLATFORM}/config.cmake)
    include(platform/ext/target/${TFM_PLATFORM}/config.cmake)
endif()

# Load configs generated from Kconfig
include(${CMAKE_SOURCE_DIR}/config/kconfig.cmake)

# Parse tf-m-tests config prior to platform specific config.cmake
# Some platforms select different configuration according when regression tests
# are enabled.
include(lib/ext/tf-m-tests/reg_parse.cmake)

# Load profile config, setting options not already set
if (TFM_PROFILE)
    include(config/profile/${TFM_PROFILE}.cmake)
endif()

# Load Secure Partition settings according to regression configuration as all SPs are disabled
# by default
if(TFM_S_REG_TEST OR TFM_NS_REG_TEST)
    include(${CMAKE_CURRENT_LIST_DIR}/tests/regression_config.cmake)
endif()

include(${CMAKE_SOURCE_DIR}/config/tfm_build_log_config.cmake)

# Load TF-M model specific default config
# Load IPC backend config if isolation level is explicitly specified to 2/3 or IPC backend is
# selected via build command line. Otherwise, load SFN backend config by default.
# If a pair of invalid settings are passed via command line, it will be captured later via config
# check.
# Also select IPC model by default for multi-core platform unless it has already selected SFN model
if ((DEFINED TFM_ISOLATION_LEVEL AND TFM_ISOLATION_LEVEL GREATER 1) OR
    CONFIG_TFM_SPM_BACKEND STREQUAL "IPC" OR
    TFM_MULTI_CORE_TOPOLOGY)
    include(config/tfm_ipc_config_default.cmake)
else()
    #The default backend is SFN
    include(config/tfm_sfn_config_default.cmake)
endif()

# Load bl1 config
if (BL1 AND PLATFORM_DEFAULT_BL1)
    include(${CMAKE_SOURCE_DIR}/bl1/config/bl1_config_default.cmake)
endif()

# Load MCUboot specific default.cmake
if (NOT DEFINED BL2 OR BL2)
    include(${CMAKE_SOURCE_DIR}/bl2/ext/mcuboot/mcuboot_default_config.cmake)
endif()

# Include FWU partition configs.
include(config/tfm_fwu_config.cmake)

# Include coprocessor configs
include(config/cp_config_default.cmake)

# Load defaults, setting options not already set
include(config/config_base.cmake)

# Fetch tf-m-tests repo during config, if NS or regression test is required.
# Therefore tf-m-tests configs can be set with TF-M configs since their configs
# are coupled.
include(lib/ext/tf-m-tests/fetch_repo.cmake)

# Set secure log configs
# It also depends on regression test config.
include(config/tfm_secure_log.cmake)

# Set user defined TF-M config header file
if(PROJECT_CONFIG_HEADER_FILE)
    if(NOT EXISTS ${PROJECT_CONFIG_HEADER_FILE})
        message(FATAL_ERROR "${PROJECT_CONFIG_HEADER_FILE} does not exist! Please use absolute path.")
    endif()
    target_compile_definitions(tfm_config
        INTERFACE
            PROJECT_CONFIG_HEADER_FILE="${PROJECT_CONFIG_HEADER_FILE}"
    )
endif()

# Set platform defined TF-M config header file
set(TARGET_CONFIG_HEADER_FILE "${CMAKE_SOURCE_DIR}/platform/ext/target/${TFM_PLATFORM}/config_tfm_target.h")
if(EXISTS ${TARGET_CONFIG_HEADER_FILE})
    target_compile_definitions(tfm_config
        INTERFACE
            TARGET_CONFIG_HEADER_FILE="${TARGET_CONFIG_HEADER_FILE}"
    )
endif()
