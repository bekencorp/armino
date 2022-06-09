#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
# Copyright (c) 2021-2022, Cypress Semiconductor Corporation. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(BL2                                 OFF         CACHE BOOL      "Whether to build BL2")

set(TFM_NS_CLIENT_IDENTIFICATION        OFF         CACHE BOOL      "Enable NS client identification")

############################ Platform ##########################################

add_definitions(-DCYB0644ABZI_S2D44)

set(TFM_MULTI_CORE_TOPOLOGY             ON          CACHE BOOL      "Whether to build for a dual-cpu architecture")
set(NUM_MAILBOX_QUEUE_SLOT              4           CACHE BOOL      "Number of mailbox queue slots")
set(PLATFORM_SLIH_IRQ_TEST_SUPPORT      ON          CACHE BOOL      "Platform supports SLIH IRQ tests")

################################## Dependencies ################################

set(CY_MTB_PDL_CAT1_LIB_PATH            "DOWNLOAD"  CACHE PATH      "Path to MTB_PDL_CAT1 repo (or DOWNLOAD to fetch automatically")
set(CY_MTB_PDL_CAT1_LIB_VERSION         "release-v2.0.0" CACHE STRING "The version of MTB_PDL_CAT1 to use")

set(CY_P64_UTILS_LIB_PATH               "DOWNLOAD"  CACHE PATH      "Path to p64_utils repo (or DOWNLOAD to fetch automatically")
set(CY_P64_UTILS_LIB_VERSION            "release-v1.0.0" CACHE STRING "The version of p64_utils to use")

# Configure PDL to use RAM interrup vectors. This is required by P64_utils
add_definitions(-DRAM_VECTORS_SUPPORT)

# Disable default PDL IPC configuration. Use Cy_Platform_Init() in
# in psoc6_system_init_cm4.c and psoc6_system_init_cm0p.c instead.
add_definitions(-DCY_IPC_DEFAULT_CFG_DISABLE)
