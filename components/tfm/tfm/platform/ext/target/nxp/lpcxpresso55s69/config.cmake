#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
# Copyright (c) 2021, NXP Semiconductors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

################################## Dependencies ################################
set(TFM_PLATFORM_NXP_HAL_FILE_PATH      "DOWNLOAD"      CACHE STRING    "Path to the NXP SDK hal (or DOWNLOAD to fetch automatically)")
set(NXP_SDK_GIT_TAG                     "MCUX_2.10.0"    CACHE STRING    "The version of the NXP MCUXpresso SDK")

############################ Partitions ########################################
set(PS_NUM_ASSETS                       "5"             CACHE STRING    "The maximum number of assets to be stored in the Protected Storage area")
set(PS_MAX_ASSET_SIZE                   "512"           CACHE STRING    "The maximum asset size to be stored in the Protected Storage area")

set(BL2_S_IMAGE_START                   "0x8000"        CACHE STRING    "Base address of the secure image in configuration with BL2")
set(BL2_NS_IMAGE_START                  "0x30000"       CACHE STRING    "Base address of the non secure image in configuration with BL2")
