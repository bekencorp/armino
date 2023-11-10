#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

fetch_remote_library(
    LIB_NAME                psa_arch_tests
    LIB_SOURCE_PATH_VAR     PSA_ARCH_TESTS_PATH
    LIB_PATCH_DIR           ${CMAKE_CURRENT_LIST_DIR}
    FETCH_CONTENT_ARGS
        GIT_REPOSITORY      https://github.com/ARM-software/psa-arch-tests.git
        GIT_TAG             ${PSA_ARCH_TESTS_VERSION}
        GIT_SHALLOW         FALSE
        GIT_PROGRESS        TRUE
        GIT_SUBMODULES      ""
)
