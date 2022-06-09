#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
# Copyright (c) 2021 STMicroelectronics. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

################################ BL2 ###########################################
if(BL2)

    target_sources(bl2_crypto_hw
        PUBLIC
            ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/rsa_alt.c
            ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/ecdsa_alt.c
            ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/ecp_alt.c
            ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/ecp_curves_alt.c
            ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/sha1_alt.c
            ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/sha256_alt.c
            ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/stm.c
    )

    target_include_directories(bl2_crypto_hw
        PRIVATE
            ${PLATFORM_DIR}/ext/target/${TFM_PLATFORM}/accelerator/
            ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/
            ${PLATFORM_DIR}/ext/target/${TFM_PLATFORM}/include/
            ${PLATFORM_DIR}/ext/target/stm/common/stm32u5xx/hal/Inc/
            ${PLATFORM_DIR}/ext/target/stm/common/stm32u5xx/Device/Include/
            ${PLATFORM_DIR}/ext/cmsis
    )

    target_link_libraries(bl2_crypto_hw
        PRIVATE
            bl2_mbedcrypto
            platform_bl2
    )

    set(STM_LIB_TARGET bl2_crypto_hw)

    target_link_libraries(bl2_mbedcrypto
        PUBLIC
            bl2_crypto_hw
    )
    target_include_directories(bl2_mbedcrypto
        PUBLIC
            ${PLATFORM_DIR}/ext/target/${TFM_PLATFORM}/accelerator/
            ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/
            ${PLATFORM_DIR}/ext/target/${TFM_PLATFORM}/include/
            ${PLATFORM_DIR}/ext/target/stm/common/stm32u5xx/hal/Inc/
            ${PLATFORM_DIR}/ext/target/stm/common/stm32u5xx/Device/Include/
            ${PLATFORM_DIR}/ext/cmsis
    )

endif()
############################ Crypto Service ####################################

target_sources(crypto_service_crypto_hw
    PRIVATE
        ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/rsa_alt.c
        ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/ecdsa_alt.c
        ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/gcm_alt.c
        ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/aes_alt.c
        ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/ccm_alt.c
        ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/ecp_alt.c
        ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/ecp_curves_alt.c
        ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/sha1_alt.c
        ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/sha256_alt.c
        ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/stm.c
        ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/huk.c
)

target_include_directories(crypto_service_crypto_hw
    PRIVATE
        ${PLATFORM_DIR}/ext/target/${TFM_PLATFORM}/accelerator/
        ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/
        ${PLATFORM_DIR}/ext/target/${TFM_PLATFORM}/include/
        ${PLATFORM_DIR}/ext/target/stm/common/stm32u5xx/hal/Inc/
        ${PLATFORM_DIR}/ext/target/stm/common/stm32u5xx/Device/Include/
        ${PLATFORM_DIR}/ext/cmsis
        ${PLATFORM_DIR}/include
        ${CMAKE_BINARY_DIR}/generated
        ${CMAKE_SOURCE_DIR}/interface/include
)
target_include_directories(crypto_service_mbedcrypto
    PUBLIC
        ${PLATFORM_DIR}/ext/target/${TFM_PLATFORM}/accelerator/
        ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/
        ${PLATFORM_DIR}/ext/target/${TFM_PLATFORM}/include/
        ${PLATFORM_DIR}/ext/target/stm/common/stm32u5xx/hal/Inc/
        ${PLATFORM_DIR}/ext/target/stm/common/stm32u5xx/Device/Include/
        ${PLATFORM_DIR}/ext/cmsis
)

target_compile_definitions(crypto_service_crypto_hw
    PRIVATE
        ST_HW_CONTEXT_SAVING
        $<$<AND:$<BOOL:${TFM_PARTITION_PROTECTED_STORAGE}>,$<STREQUAL:${PS_CRYPTO_AEAD_ALG},PSA_ALG_GCM>>:BUILD_CRYPTO_TFM>
    INTERFACE
        $<$<AND:$<BOOL:${TFM_PARTITION_PROTECTED_STORAGE}>,$<STREQUAL:${PS_CRYPTO_AEAD_ALG},PSA_ALG_GCM>>:MBEDTLS_GCM_C>
)

target_link_libraries(crypto_service_crypto_hw
    PRIVATE
        crypto_service_mbedcrypto
        platform_s
)

target_link_libraries(platform_s
    PRIVATE
        crypto_service_crypto_hw
)
target_link_libraries(crypto_service_crypto_hw
     INTERFACE
     tfm_partition_defs
)
