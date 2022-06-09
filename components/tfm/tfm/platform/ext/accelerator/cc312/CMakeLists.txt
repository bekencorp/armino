#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
# Allow linking to things 'upwards' in the directory tree (in this case bl2 / tfm_psa_rot_partition_crypto)
cmake_policy(SET CMP0079 NEW)
# Allow relative paths
cmake_policy(SET CMP0076 NEW)

if(NOT DEFINED CC312_PATH)
    set(CC312_PATH ../../../../lib/ext/cryptocell-312-runtime CACHE PATH "Path to CC312 lib")
endif()

if(BL2)
    target_compile_definitions(platform_bl2
        PRIVATE
            CRYPTO_HW_ACCELERATOR
    )
endif()

################################ OTP ###########################################

target_sources(platform_s
    PRIVATE
        $<$<NOT:$<BOOL:${PLATFORM_DEFAULT_OTP}>>:${CMAKE_CURRENT_SOURCE_DIR}/otp_cc312.c>
)

if(BL2)
    target_sources(platform_bl2
        PRIVATE
        $<$<NOT:$<BOOL:${PLATFORM_DEFAULT_OTP}>>:${CMAKE_CURRENT_SOURCE_DIR}/otp_cc312.c>
    )
endif()

################################ BL1 ###########################################

if(BL1 AND PLATFORM_DEFAULT_BL1)
    add_subdirectory(cc312-rom)

    target_sources(bl1_crypto_hw
        PRIVATE
            ./cc312_rom_crypto_hw.c
            $<$<NOT:$<BOOL:${TFM_BL1_SOFTWARE_CRYPTO}>>:${CMAKE_CURRENT_SOURCE_DIR}/bl1/cc312_rom_crypto.c>
            $<$<NOT:$<BOOL:${TFM_BL1_DUMMY_TRNG}>>:${CMAKE_CURRENT_SOURCE_DIR}/bl1/cc312_rom_trng.c>
            $<$<NOT:$<BOOL:${PLATFORM_DEFAULT_OTP}>>:${CMAKE_CURRENT_SOURCE_DIR}/otp_cc312.c>
    )

    target_link_libraries(bl1_crypto_hw
        PUBLIC
            bl1_1_shared_lib_interface
            cc312_rom
            platform_region_defs
            tfm_fih_headers
            platform_bl1
    )

    target_link_libraries(bl1_1_shared_lib
        PUBLIC
            bl1_crypto_hw
    )
endif()

################################ BL2 ###########################################

if(BL2)

    set(CC312_LIB_PREFIX bl2_)
    # Platform depedency needed to access platform specific dx_reg_base_host.h
    set(CC312_PLATFORM_DEPENDENCY platform_bl2)

    target_sources(bl2_crypto_hw
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/cc312.c
    )

    target_include_directories(bl2_crypto_hw
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    if (${PLATFORM_PSA_ADAC_SECURE_DEBUG})
        add_subdirectory(psa-adac)
    endif()

    target_link_libraries(bl2_crypto_hw
        PUBLIC
            ${CC312_LIB_PREFIX}cc312
        PRIVATE
            bl2_mbedcrypto
            platform_bl2
    )

    set(SAVED_BUILD_TYPE ${CMAKE_BUILD_TYPE})
    set(CMAKE_BUILD_TYPE ${MBEDCRYPTO_BUILD_TYPE})
    add_subdirectory(${CC312_PATH} ${CMAKE_CURRENT_BINARY_DIR}/${CC312_LIB_PREFIX}cc312)
    set(CMAKE_BUILD_TYPE ${SAVED_BUILD_TYPE} CACHE STRING "Build type: [Debug, Release, RelWithDebInfo, MinSizeRel]" FORCE)

    target_sources(${CC312_LIB_PREFIX}cc312
        PUBLIC
            $<$<OR:$<CONFIG:Debug>,$<CONFIG:relwithdebinfo>>:${CMAKE_CURRENT_SOURCE_DIR}/cc312_log.c>
    )

    # Adding two targets as link-time dependencies of each other seems bad, but
    # in reality it just means that they'll share headers and compile defs.
    target_link_libraries(${CC312_LIB_PREFIX}cc312_mbedtls_api
        PRIVATE
            bl2_mbedcrypto
            platform_bl2
        PUBLIC
            platform_common_interface
    )
    target_link_libraries(bl2_mbedcrypto
        PRIVATE
            ${CC312_LIB_PREFIX}cc312_mbedtls_api
        PUBLIC
            bl2_crypto_hw
    )

    # boot_hal depends on crypto_hw_accelerator abstractions
    target_link_libraries(platform_bl2
        PRIVATE
            bl2_crypto_hw
    )

    target_link_libraries(${CC312_LIB_PREFIX}cc312_cdmpu
        INTERFACE
            bl2_mbedcrypto
    )

    target_compile_definitions(bl2_mbedcrypto
        PUBLIC
            CRYPTO_HW_ACCELERATOR
            MBEDTLS_ECDH_LEGACY_CONTEXT
    )

    target_compile_options(bl2_mbedcrypto
        PRIVATE
            $<$<C_COMPILER_ID:GNU>:-Wno-unused-parameter>
            $<$<C_COMPILER_ID:ARMClang>:-Wno-unused-parameter>
    )

    target_compile_options(${CC312_LIB_PREFIX}cc312
        PRIVATE
            ${BL2_COMPILER_CP_FLAG}
    )

    target_compile_options(bl2_crypto_hw
        PRIVATE
            ${BL2_COMPILER_CP_FLAG}
    )

    target_compile_options(${CC312_LIB_PREFIX}cc312_cdmpu
        INTERFACE
            ${BL2_COMPILER_CP_FLAG}
    )

    unset(CC312_LIB_PREFIX)
    unset(CC312_PLATFORM_DEPENDENCY)

endif()

############################ Crypto Service ####################################

set(CC312_LIB_PREFIX crypto_service_)
# Platform depedency needed to access platform specific dx_reg_base_host.h
set(CC312_PLATFORM_DEPENDENCY platform_s)

target_sources(crypto_service_crypto_hw
    PRIVATE
        cc312.c
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:relwithdebinfo>>:${CMAKE_CURRENT_SOURCE_DIR}/cc312_log.c>
)

target_include_directories(crypto_service_crypto_hw
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(crypto_service_crypto_hw
    PUBLIC
        ${CC312_LIB_PREFIX}cc312
    PRIVATE
        crypto_service_mbedcrypto
        platform_s
)

set(SAVED_BUILD_TYPE ${CMAKE_BUILD_TYPE})
set(CMAKE_BUILD_TYPE ${MBEDCRYPTO_BUILD_TYPE})
add_subdirectory(${CC312_PATH} ${CMAKE_CURRENT_BINARY_DIR}/${CC312_LIB_PREFIX}cc312)
set(CMAKE_BUILD_TYPE ${SAVED_BUILD_TYPE} CACHE STRING "Build type: [Debug, Release, RelWithDebInfo, MinSizeRel]" FORCE)

target_sources(${CC312_LIB_PREFIX}cc312
    PRIVATE
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:relwithdebinfo>>:${CMAKE_CURRENT_SOURCE_DIR}/cc312_log.c>
)

# Control the enablement of the legacy CC-312 driver interface or the PSA
# driver interface. Default value is for the legacy interface
option(CC312_LEGACY_DRIVER_API_ENABLED
       "This variable controls whether the legacy driver interface is used for CC-312." ON)

# FixMe: Secure tests enabled and Debug builds with FP support set to hardware
#        need to fallback to the legacy driver as the new PSA driver overflows
#        the available flash memory on Musca-S1 and Musca-B1
if ((NOT ${CC312_LEGACY_DRIVER_API_ENABLED}) AND
    TEST_S AND (${CMAKE_BUILD_TYPE} STREQUAL "Debug") AND (${CONFIG_TFM_FP} STREQUAL "hard"))
    set(CC312_LEGACY_DRIVER_API_ENABLED ON)
    message(WARNING
        "The CC-312 legacy driver interface fallback is forced to ${CC312_LEGACY_DRIVER_API_ENABLED}. \
        The following parameters are not supported at the same time: \
        (TEST_S: ${TEST_S}, \
        CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}, \
        CONFIG_TFM_FP: ${CONFIG_TFM_FP}) for the PSA Cryptoprocessor driver interface due to flash memory constraints.")
endif()

# FixMe: Secure tests enabled and Debug builds on Musca-B1 need to fallback to
#        the legacy driver as the new PSA driver overflows the available flash
#        memory of the SSE-200 subsystem
if ((NOT ${CC312_LEGACY_DRIVER_API_ENABLED}) AND
    TEST_S AND (${CMAKE_BUILD_TYPE} STREQUAL "Debug") AND (${TFM_PLATFORM} STREQUAL "arm/musca_b1/sse_200"))
    set(CC312_LEGACY_DRIVER_API_ENABLED ON)
    message(WARNING
        "The CC-312 legacy driver interface fallback is forced to ${CC312_LEGACY_DRIVER_API_ENABLED}. \
        The following parameters are not supported at the same time: \
        (TEST_S: ${TEST_S}, \
        CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}, \
        TFM_PLATFORM: ${TFM_PLATFORM}) for the PSA Cryptoprocessor driver interface due to flash memory constraints.")
endif()

if (${CC312_LEGACY_DRIVER_API_ENABLED})
    set(CC312_DRIVER_API_TARGET ${CC312_LIB_PREFIX}cc312_mbedtls_api)
else()
    set(CC312_DRIVER_API_TARGET ${CC312_LIB_PREFIX}cc312_psa_driver_api)
endif()

# Adding two targets as link-time dependencies of each other seems bad, but
# in reality it just means that they'll share headers and compile defs.
target_link_libraries(${CC312_DRIVER_API_TARGET}
    PRIVATE
        crypto_service_mbedcrypto
    PUBLIC
        platform_s
)
target_link_libraries(crypto_service_mbedcrypto
    PRIVATE
        ${CC312_DRIVER_API_TARGET}
    PUBLIC
        crypto_service_crypto_hw
)

target_compile_definitions(crypto_service_mbedcrypto
    PUBLIC
        CRYPTO_HW_ACCELERATOR
        MBEDTLS_ECDH_LEGACY_CONTEXT
)

target_compile_options(crypto_service_cc312
    PUBLIC
        $<$<C_COMPILER_ID:GNU>:-Wno-unused-parameter>
        $<$<C_COMPILER_ID:ARMClang>:-Wno-unused-parameter>
)

target_compile_options(crypto_service_mbedcrypto
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-Wno-unused-parameter>
        $<$<C_COMPILER_ID:ARMClang>:-Wno-unused-parameter>
)

unset(CC312_LIB_PREFIX)
unset(CC312_PLATFORM_DEPENDENCY)
