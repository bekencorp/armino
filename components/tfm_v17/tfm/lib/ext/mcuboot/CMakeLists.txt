#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# Because of https://gitlab.kitware.com/cmake/cmake/-/issues/20579 CMP0097 is
# non-functional until cmake 3.18.0.
if(${CMAKE_VERSION} VERSION_LESS "3.18.0")
    # Workaround for not having CMP0097. Use existing directory.
    set(MCUBOOT_SUBMODULES "docs")
endif()

fetch_remote_library(
    LIB_NAME                mcuboot
    LIB_SOURCE_PATH_VAR     MCUBOOT_PATH
    FETCH_CONTENT_ARGS
        GIT_REPOSITORY      https://github.com/mcu-tools/mcuboot.git
        GIT_TAG             ${MCUBOOT_VERSION}
        # ToDo: set GIT_SHALLOW to 'TRUE' when MCUBOOT_VERSION will be set to a tag
        # (instead of a commit hash) with the new release.
        GIT_SHALLOW         FALSE
        GIT_PROGRESS        TRUE
        GIT_SUBMODULES      "${MCUBOOT_SUBMODULES}"
)
