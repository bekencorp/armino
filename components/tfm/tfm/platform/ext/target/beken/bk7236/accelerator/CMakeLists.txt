################################ BL2 ###########################################
if(BL2)
    target_compile_options(bl2_crypto_hw
        PUBLIC
            ${BL2_COMPILER_CP_FLAG}
    )


    target_sources(bl2_crypto_hw
        PRIVATE
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/common/dubhe_driver.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/common/dubhe_event.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/common/dubhe_intr_handler.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/common/dubhe_sram_alloc.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/aca/dubhe_aca.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/aca/dubhe_aca_hw.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/aead/dubhe_ccm.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/aead/dubhe_gcm.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/hash/dubhe_hash.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/mac/dubhe_cbcmac.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/mac/dubhe_cmac.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/mac/dubhe_ghash.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/otp/dubhe_otp.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/sca/dubhe_sca.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/trng/dubhe_trng.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/hal_pal/pal_heap.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/hal_pal/pal_log.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/hal_pal/pal_mutex.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/hal_pal/pal_preempt.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/hal_pal/pal_semaphore.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/hal_pal/pal_signal.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/hal_pal/pal_string.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/hal_pal/pal_time.c
    )

    target_include_directories(bl2_crypto_hw
        PRIVATE
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/inc/common
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/inc/crypto
    )

    target_sources(bl2_crypto_hw
        PRIVATE
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/aes.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/bignum_wrap.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/ccm.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/cmac.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/gcm.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/otp.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/sha1.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/sha256.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/sm2dsa.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/sm2kep.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/sm2pke.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/sm2_internal.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/sm3.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/sm4.c
    )

    target_include_directories(bl2_crypto_hw
        PRIVATE
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/inc
    )

    target_sources(bl2_crypto_hw
        PRIVATE
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/bignum.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/bignum_delta.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/cipher.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/cipher_wrap.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/ctr_drbg.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/dhm.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/ecdh.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/ecdsa.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/ecp.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/ecp_curves.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/entropy_poll.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/md.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/oid.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/pkparse.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/rsa.c
    )

    target_sources(bl2_crypto_hw
        PRIVATE
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/spe/src/misc.c
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/crypto_hw.c
    )

    target_include_directories(bl2_crypto_hw
        PRIVATE
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/spe/inc
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/spe/inc/hal
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/spe/inc/pal
    )

    target_include_directories(bl2_crypto_hw
        PUBLIC
            ${PLATFORM_DIR}/ext/target/${TFM_PLATFORM}/accelerator
            ${PLATFORM_DIR}/include
            ${MBEDCRYPTO_PATH}/library
            ${MBEDCRYPTO_PATH}/include
            ${PLATFORM_DIR}/ext/target/beken/bk7236/armino/config
            ${CMAKE_SOURCE_DIR}/interface/include
            ${CMAKE_BINARY_DIR}/generated/interface/include
            ${PLATFORM_DIR}/ext/cmsis
    )

    target_compile_definitions(bl2_crypto_hw
        PRIVATE
            ARM_CE_DUBHE
            DUBHE_SECURE
            ARM_CE_DUBHE_SCA
            ARM_CE_DUBHE_TRNG
            ARM_CE_DUBHE_OTP
            ARM_CE_DUBHE_HASH
            ARM_CE_DUBHE_ACA
    )

    target_link_libraries(bl2_crypto_hw
        PRIVATE
            bl2_mbedcrypto
    )

    target_include_directories(bl2_mbedcrypto
        PUBLIC
            ${PLATFORM_DIR}/ext/target/${TFM_PLATFORM}/accelerator/
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/inc/common
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/inc/crypto
            ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/inc
    )

endif()
############################ Crypto Service ####################################

target_sources(crypto_service_crypto_hw
    PRIVATE
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/common/dubhe_driver.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/common/dubhe_event.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/common/dubhe_intr_handler.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/common/dubhe_sram_alloc.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/aca/dubhe_aca.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/aca/dubhe_aca_hw.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/aead/dubhe_ccm.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/aead/dubhe_gcm.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/hash/dubhe_hash.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/mac/dubhe_cbcmac.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/mac/dubhe_cmac.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/mac/dubhe_ghash.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/otp/dubhe_otp.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/sca/dubhe_sca.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/crypto/trng/dubhe_trng.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/hal_pal/pal_heap.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/hal_pal/pal_log.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/hal_pal/pal_mutex.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/hal_pal/pal_preempt.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/hal_pal/pal_semaphore.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/hal_pal/pal_signal.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/hal_pal/pal_string.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/src/hal_pal/pal_time.c
)

target_include_directories(crypto_service_crypto_hw
    PRIVATE
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/inc/common
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/inc/crypto
)

target_sources(crypto_service_crypto_hw
    PRIVATE
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/aes.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/bignum_wrap.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/ccm.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/cmac.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/gcm.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/otp.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/sha1.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/sha256.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/sm2dsa.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/sm2kep.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/sm2pke.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/sm2_internal.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/sm3.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/src/sm4.c
)

target_include_directories(crypto_service_crypto_hw
    PRIVATE
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/inc
        ${PLATFORM_DIR}/ext/target/beken/bk7236/armino/config
        ${CMAKE_SOURCE_DIR}/interface/include
        ${CMAKE_BINARY_DIR}/generated/interface/include
)

target_sources(crypto_service_crypto_hw
    PRIVATE
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/bignum.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/bignum_delta.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/cipher.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/cipher_wrap.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/ctr_drbg.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/dhm.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/ecdh.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/ecdsa.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/ecp.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/ecp_curves.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/entropy_poll.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/md.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/oid.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/pkparse.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_lib/rsa.c
)

target_sources(crypto_service_crypto_hw
    PRIVATE
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/spe/src/misc.c
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/crypto_hw.c
)

target_include_directories(crypto_service_crypto_hw
    PRIVATE
        ${PLATFORM_DIR}/ext/target/beken/bk7236
        ${PLATFORM_DIR}/ext/target/beken/bk7236/cmsis_core
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/spe/inc
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/spe/inc/hal
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/spe/inc/pal
)

target_include_directories(crypto_service_crypto_hw
    PUBLIC
        ${PLATFORM_DIR}/ext/target/${TFM_PLATFORM}/accelerator
        ${PLATFORM_DIR}/include
        ${MBEDCRYPTO_PATH}/library
        ${MBEDCRYPTO_PATH}/include
        ${CMAKE_SOURCE_DIR}/../../cmsis/CMSIS_5/Device/ArmChina/STAR/Include
)

target_link_libraries(crypto_service_crypto_hw
    PRIVATE
        crypto_service_mbedcrypto
)


target_compile_options(crypto_service_crypto_hw
    PUBLIC
        ${COMPILER_CP_FLAG}
)

target_compile_definitions(crypto_service_crypto_hw
    PRIVATE
        TEE_M
        ARM_CE_DUBHE
        DUBHE_SECURE
        ARM_CE_DUBHE_SCA
        ARM_CE_DUBHE_TRNG
        ARM_CE_DUBHE_OTP
        ARM_CE_DUBHE_HASH
        ARM_CE_DUBHE_ACA
)

if (NOT BL2)
target_compile_definitions(crypto_service_crypto_hw
    PRIVATE
        TEE_M
        DUBHE_FOR_RUNTIME
)
endif()

target_include_directories(crypto_service_mbedcrypto
    PUBLIC
        ${PLATFORM_DIR}/ext/target/${TFM_PLATFORM}/accelerator/
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/inc/common
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_driver/inc/crypto
        ${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/inc
)

target_compile_definitions(crypto_service_mbedcrypto
    PRIVATE
        TEE_M
        ARM_CE_DUBHE
        DUBHE_SECURE
        ARM_CE_DUBHE_SCA
        ARM_CE_DUBHE_TRNG
        ARM_CE_DUBHE_OTP
        ARM_CE_DUBHE_HASH
        ARM_CE_DUBHE_ACA
        DUBHE_FOR_RUNTIME
)
