#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(MCUBOOT_IMAGE_NUMBER                1            CACHE STRING "Whether to combine S and NS into either 1 image, or sign each separately")
set(MCUBOOT_UPGRADE_STRATEGY            "DIRECT_XIP" CACHE STRING "Upgrade strategy for images")
if(NOT BL2)
    set(BL2_TRAILER_SIZE                0x800        CACHE STRING "Trailer size")
endif()
################################## Dependencies ################################

set(MBEDCRYPTO_BUILD_TYPE               minsizerel  CACHE STRING "Build type of Mbed Crypto library")
