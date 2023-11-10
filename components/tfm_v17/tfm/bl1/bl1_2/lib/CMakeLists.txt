#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

add_library(bl1_2_lib STATIC)

target_include_directories(bl1_2_lib
    PUBLIC
        ./interface
    PRIVATE
        $<$<BOOL:${TFM_BL1_PQ_CRYPTO}>:${MBEDCRYPTO_PATH}/include>
        $<$<BOOL:${TFM_BL1_PQ_CRYPTO}>:${CMAKE_CURRENT_SOURCE_DIR}/pq_crypto>
)

target_sources(bl1_2_lib
    PRIVATE
        $<$<BOOL:${TFM_BL1_PQ_CRYPTO}>:./pq_crypto/pq_crypto_mbedtls.c>
        $<$<BOOL:${TFM_BL1_PQ_CRYPTO}>:${MBEDCRYPTO_PATH}/library/lms.c>
        $<$<BOOL:${TFM_BL1_PQ_CRYPTO}>:${MBEDCRYPTO_PATH}/library/lmots.c>
        $<$<BOOL:${TFM_BL1_PQ_CRYPTO}>:${MBEDCRYPTO_PATH}/library/platform_util.c>
        ./image.c
)

target_compile_definitions(bl1_2_lib
    PRIVATE
        $<$<BOOL:${TFM_BL1_PQ_CRYPTO}>:MBEDTLS_CONFIG_FILE="mbedtls-pq-cfg.h">
)

target_link_libraries(bl1_2_lib
    PUBLIC
        platform_common_interface
    PRIVATE
        platform_bl1_interface
        bl1_1_shared_lib_interface
)
