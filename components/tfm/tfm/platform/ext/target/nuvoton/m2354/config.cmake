#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(MCUBOOT_IMAGE_NUMBER    1      CACHE STRING    "Whether to combine S and NS into either 1 image, or sign each separately")
set(PS_MAX_ASSET_SIZE       512    CACHE STRING    "The maximum asset size to be stored in the Protected Storage area")
set(PS_NUM_ASSETS           12     CACHE STRING    "The maximum number of assets to be stored in the Protected Storage area")
set(ITS_NUM_ASSETS          12     CACHE STRING    "The maximum number of assets to be stored in the Internal Trusted Storage area")
set(BL2_TRAILER_SIZE        0x800  CACHE STRING    "Trailer size")
