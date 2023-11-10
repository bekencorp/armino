#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

fetch_remote_library(
    LIB_NAME                libpsaadac
    LIB_SOURCE_PATH_VAR     PLATFORM_PSA_ADAC_SOURCE_PATH
    LIB_BINARY_PATH_VAR     PLATFORM_PSA_ADAC_BUILD_PATH
    FETCH_CONTENT_ARGS
        GIT_REPOSITORY      https://git.trustedfirmware.org/shared/psa-adac.git
        GIT_TAG             ${PLATFORM_PSA_ADAC_VERSION}
)

set(PSA_ADAC_TARGET "trusted-firmware-m")
set(PSA_ADAC_MBEDTLS_INCLUDE "${MBEDCRYPTO_PATH}/include")
set(PSA_ADAC_TOOLCHAIN    FALSE    CACHE BOOL "Whether to use psa-adac toolchain." FORCE)

add_subdirectory(${PLATFORM_PSA_ADAC_SOURCE_PATH} ${PLATFORM_PSA_ADAC_BUILD_PATH})
