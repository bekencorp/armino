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
            ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/sha1_alt.c
            ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/sha256_alt.c
            ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/stm.c
    )

    target_include_directories(bl2_crypto_hw
        PRIVATE
            ${PLATFORM_DIR}/ext/target/${TFM_PLATFORM}/accelerator/
            ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/
            ${PLATFORM_DIR}/ext/target/${TFM_PLATFORM}/include/
            ${PLATFORM_DIR}/ext/target/stm/common/stm32l5xx/hal/Inc/
            ${PLATFORM_DIR}/ext/target/stm/common/stm32l5xx/Device/Include/
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
            ${PLATFORM_DIR}/ext/target/stm/common/stm32l5xx/hal/Inc/
            ${PLATFORM_DIR}/ext/target/stm/common/stm32l5xx/Device/Include/
            ${PLATFORM_DIR}/ext/cmsis
    )

endif()
############################ Crypto Service ####################################

target_sources(crypto_service_crypto_hw
    PRIVATE
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
        ${PLATFORM_DIR}/ext/target/stm/common/stm32l5xx/hal/Inc/
        ${PLATFORM_DIR}/ext/target/stm/common/stm32l5xx/Device/Include/
        ${PLATFORM_DIR}/ext/cmsis
)
target_include_directories(crypto_service_mbedcrypto
    PUBLIC
        ${PLATFORM_DIR}/ext/target/${TFM_PLATFORM}/accelerator/
        ${PLATFORM_DIR}/ext/target/stm/common/hal/accelerator/
        ${PLATFORM_DIR}/ext/target/${TFM_PLATFORM}/include/
        ${PLATFORM_DIR}/ext/target/stm/common/stm32l5xx/hal/Inc/
        ${PLATFORM_DIR}/ext/target/stm/common/stm32l5xx/Device/Include/
        ${PLATFORM_DIR}/ext/cmsis
        ${PLATFORM_DIR}/include
)

target_compile_definitions(crypto_service_crypto_hw
    PRIVATE
        ST_HW_CONTEXT_SAVING
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




