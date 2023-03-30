#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if(BL1 AND PLATFORM_DEFAULT_BL1)
    add_library(bl1_crypto_hw STATIC)
endif()
if(BL2)
    add_library(bl2_crypto_hw STATIC)
endif()
if (TFM_PARTITION_CRYPTO)
add_library(crypto_service_crypto_hw STATIC)
endif()

add_subdirectory(${CRYPTO_HW_ACCELERATOR_TYPE})

############################ Project Config ####################################

target_compile_definitions(tfm_config
    INTERFACE
        CRYPTO_HW_ACCELERATOR
)

################################ BL1 ###########################################

if(BL1 AND PLATFORM_DEFAULT_BL1)
    target_include_directories(bl1_crypto_hw
        PUBLIC
            interface
    )

    target_compile_definitions(bl1_crypto_hw
        PUBLIC
            CRYPTO_HW_ACCELERATOR
    )
endif()

################################ BL2 ###########################################

if(BL2)
    target_include_directories(bl2_crypto_hw
        PUBLIC
            interface
    )

    target_compile_definitions(bl2_crypto_hw
        PUBLIC
            CRYPTO_HW_ACCELERATOR
    )
endif()

############################ Crypto Service ####################################

if (TFM_PARTITION_CRYPTO)
    target_include_directories(crypto_service_crypto_hw
        PUBLIC
            interface
    )

    target_compile_definitions(crypto_service_crypto_hw
        PUBLIC
            CRYPTO_HW_ACCELERATOR
    )
endif()
