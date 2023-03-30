#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
if(NOT DEFINED CC312_PATH)
    set(CC312_PATH ../../../../../lib/ext/cryptocell-312-runtime CACHE PATH "Path to CC312 lib")
endif()

add_library(psa_adac_cc312 STATIC)

target_sources(psa_adac_cc312
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/adac_crypto_cc312.c
        ${CMAKE_CURRENT_SOURCE_DIR}/adac_crypto_cc312_hash.c
        ${CMAKE_CURRENT_SOURCE_DIR}/adac_crypto_cc312_mac.c
        ${CMAKE_CURRENT_SOURCE_DIR}/adac_crypto_cc312_pk.c
        ${CMAKE_CURRENT_SOURCE_DIR}/adac_crypto_cc312_rng.c
)

target_include_directories(psa_adac_cc312
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/ext/cryptocell-312-runtime/codesafe/src/crypto_api
        ${CMAKE_SOURCE_DIR}/lib/ext/cryptocell-312-runtime/codesafe/src/crypto_api/rnd_dma/local
)

target_compile_options(psa_adac_cc312
    PRIVATE
        -DCC_IOT
        -DUSE_MBEDTLS_CRYPTOCELL
        -D_INTERNAL_CC_NO_RSA_SCHEME_15_SUPPORT
)

target_link_libraries(psa_adac_cc312
    PUBLIC
        platform_bl2
    PRIVATE
        bl2_mbedcrypto
        trusted-firmware-m-psa-adac
)

target_link_libraries(platform_bl2
    PRIVATE
        psa_adac_cc312
)
