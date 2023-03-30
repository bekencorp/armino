#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

fetch_remote_library(
    LIB_NAME                mbedcrypto
    LIB_SOURCE_PATH_VAR     MBEDCRYPTO_PATH
    LIB_PATCH_DIR           ${CMAKE_CURRENT_LIST_DIR}
    FETCH_CONTENT_ARGS
        GIT_REPOSITORY      ${MBEDCRYPTO_GIT_REMOTE}
        GIT_TAG             ${MBEDCRYPTO_VERSION}
        GIT_SHALLOW         TRUE
        GIT_PROGRESS        TRUE
        GIT_SUBMODULES      ""
)
