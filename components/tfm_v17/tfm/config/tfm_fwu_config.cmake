#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(PLATFORM_HAS_FIRMWARE_UPDATE_SUPPORT  OFF         CACHE BOOL      "Platform supports firmware update, such as network connectivities and bootloader support")
set(TFM_PARTITION_FIRMWARE_UPDATE         OFF         CACHE BOOL      "Enable firmware update partition")
set(TFM_FWU_BOOTLOADER_LIB                "mcuboot"   CACHE STRING    "Bootloader configure file for Firmware Update partition")
set(TFM_CONFIG_FWU_MAX_WRITE_SIZE         1024        CACHE STRING    "The maximum permitted size for block in psa_fwu_write, in bytes.")
set(TFM_CONFIG_FWU_MAX_MANIFEST_SIZE      0           CACHE STRING    "The maximum permitted size for manifest in psa_fwu_start(), in bytes.")
set(FWU_DEVICE_CONFIG_FILE                ""          CACHE STRING    "The device configuration file for Firmware Update partition")
if (DEFINED MCUBOOT_UPGRADE_STRATEGY)
    if(${MCUBOOT_UPGRADE_STRATEGY} STREQUAL "SWAP_USING_SCRATCH" OR ${MCUBOOT_UPGRADE_STRATEGY} STREQUAL "SWAP_USING_MOVE")
        set(FWU_SUPPORT_TRIAL_STATE ON CACHE BOOL "Device support TRIAL component state.")
    endif()
else()
    set(FWU_SUPPORT_TRIAL_STATE OFF CACHE BOOL "Device support TRIAL component state.")
endif()
