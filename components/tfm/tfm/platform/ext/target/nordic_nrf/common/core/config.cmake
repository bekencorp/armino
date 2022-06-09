#-------------------------------------------------------------------------------
# Copyright (c) 2021, Nordic Semiconductor ASA.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(HAL_NORDIC_PATH "DOWNLOAD" CACHE PATH "Path to the Nordic HAL (or DOWNLOAD to fetch automatically)")
set(HAL_NORDIC_VERSION "004b62c0b74c62bd8489e0a702a498899e0463d1" CACHE STRING "Version of the Nordic HAL to download") # nrfx 2.6.0
set(HAL_NORDIC_REMOTE "https://github.com/zephyrproject-rtos/hal_nordic" CACHE STRING "Remote of the Nordic HAL to download")

set(NULL_POINTER_EXCEPTION_DETECTION    FALSE       CACHE BOOL
  "Enable null-pointer dereference detection for \
   priviliged and unpriviliged secure reads and writes on supported platforms. \
   Can be used to debug faults in the SPE. \
   Note that null-pointer dereferences from the NSPE \
   will trigger SecureFaults even without this option enabled. \
   May require more MPU regions than are available depending on the configuration."
  )

set(PLATFORM_SLIH_IRQ_TEST_SUPPORT      ON          CACHE BOOL    "Platform supports SLIH IRQ tests")
set(PLATFORM_FLIH_IRQ_TEST_SUPPORT      ON          CACHE BOOL    "Platform supports FLIH IRQ tests")
