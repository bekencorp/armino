#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(CONFIG_TFM_ENABLE_FP                OFF         CACHE BOOL      "Enable/disable FP usage")
set(CONFIG_TFM_ENABLE_MVE               OFF         CACHE BOOL      "Enable/disable integer MVE usage")
set(CONFIG_TFM_ENABLE_MVE_FP            OFF         CACHE BOOL      "Enable/disable floating-point MVE usage")

if (CONFIG_TFM_ENABLE_FP OR CONFIG_TFM_ENABLE_MVE OR CONFIG_TFM_ENABLE_MVE_FP)
    # Set float abi to enable hardware floating-point instructions and hardware floating-point linkage.
    set(CONFIG_TFM_FLOAT_ABI            "hard")
    set(CONFIG_TFM_ENABLE_CP10CP11      ON          CACHE BOOL            "Make FPU and MVE operational when SPE and/or NSPE require FPU or MVE usage. This alone only enables the coprocessors CP10-CP11, whereas CONFIG_TFM_FLOAT_ABI=hard along with  CONFIG_TFM_ENABLE_FP, CONFIG_TFM_ENABLE_MVE or CONFIG_TFM_ENABLE_MVE_FP compiles the code with hardware FP or MVE instructions and ABI.")
    set(CONFIG_TFM_LAZY_STACKING        ON          CACHE BOOL            "Enable/disable lazy stacking")
else()
    # Set float abi soft, meaning software library functions for floating-point operations and software floating-point linkage.
    set(CONFIG_TFM_FLOAT_ABI           "soft")
    set(CONFIG_TFM_ENABLE_CP10CP11      OFF         CACHE BOOL            "Make FPU and MVE operational when SPE and/or NSPE require FPU or MVE usage. This alone only enables the coprocessors CP10-CP11, whereas CONFIG_TFM_FLOAT_ABI=hard along with  CONFIG_TFM_ENABLE_FP, CONFIG_TFM_ENABLE_MVE or CONFIG_TFM_ENABLE_MVE_FP compiles the code with hardware FP or MVE instructions and ABI.")
    set(CONFIG_TFM_LAZY_STACKING        OFF         CACHE BOOL            "Enable/disable lazy stacking")
endif()

