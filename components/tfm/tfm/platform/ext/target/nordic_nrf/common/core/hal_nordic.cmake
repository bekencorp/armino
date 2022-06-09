
#-------------------------------------------------------------------------------
# Copyright (c) 2021, Nordic Semiconductor ASA.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (HAL_NORDIC_PATH STREQUAL "DOWNLOAD")
    message(STATUS "Pulling Nordic HAL from ${HAL_NORDIC_REMOTE} @ ${HAL_NORDIC_VERSION}")
    FetchContent_Declare(hal_nordic
        GIT_REPOSITORY ${HAL_NORDIC_REMOTE}
        GIT_TAG ${HAL_NORDIC_VERSION}
        GIT_SHALLOW FALSE # Disable shallow fetch because version is a SHA.
        GIT_PROGRESS TRUE
        GIT_SUBMODULES ""
    )

    FetchContent_GetProperties(hal_nordic)
    if(NOT hal_nordic_POPULATED)
        FetchContent_Populate(hal_nordic)
        set(HAL_NORDIC_PATH ${hal_nordic_SOURCE_DIR} CACHE PATH "Path to the Nordic HAL (or DOWNLOAD to fetch automatically)" FORCE)
    endif()
endif()
