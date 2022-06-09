#-------------------------------------------------------------------------------
# Copyright (c) 2020, Nordic Semiconductor ASA.
# Copyright (c) 2021, Laird Connectivity
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# Relative path to nordic platform files from scope of the lairdconnectivity/common/core folder
set(NRF_PLATFORM_PATH "../../../nordic_nrf")
set(PLATFORM_PATH platform/ext/target/${TFM_PLATFORM}/..)

include(${PLATFORM_PATH}/common/bl5340/config.cmake)
