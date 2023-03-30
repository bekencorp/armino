#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0079 NEW)

add_subdirectory(ns)

add_library(platform_s STATIC)
add_library(platform_region_defs INTERFACE)
add_library(platform_common_interface INTERFACE)

if (BL2)
    add_library(platform_bl2 STATIC)
endif()

if (BL1 AND PLATFORM_DEFAULT_BL1)
    add_library(platform_bl1 STATIC)
    add_library(platform_bl1_interface INTERFACE)
endif()

if (TFM_PARTITION_CRYPTO)
    add_library(platform_crypto_keys STATIC)
endif()

set(PLATFORM_DIR ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(ext/target/${TFM_PLATFORM} target)

#====================== CMSIS stack override interface ========================#

# NS linker scripts using the default CMSIS style naming conventions, while the
# secure and bl2 linker scripts remain untouched (region.h compatibility).
# To be compatible with the untouched files (which using ARMCLANG naming style),
# we have to override __INITIAL_SP and __STACK_LIMIT labels.

set(CMSIS_OVERRIDE_HEADER "${CMAKE_CURRENT_LIST_DIR}/include/cmsis_override.h")
add_library(cmsis_stack_override INTERFACE)
target_compile_options(cmsis_stack_override
    INTERFACE
        "$<$<C_COMPILER_ID:GNU>:SHELL:-include ${CMSIS_OVERRIDE_HEADER}>"
        "$<$<C_COMPILER_ID:IAR>:SHELL:--preinclude ${CMSIS_OVERRIDE_HEADER}>"
)

#========================= Platform Common interface ==========================#

target_include_directories(platform_common_interface
    INTERFACE
        ./ext
        ./ext/cmsis
        ./ext/common
        ./ext/driver
        ./include
)

#========================= Platform Secure ====================================#

target_include_directories(platform_s
    PUBLIC
        $<$<BOOL:${CRYPTO_HW_ACCELERATOR}>:${CMAKE_CURRENT_SOURCE_DIR}/ext/accelerator/interface>
)

target_sources(platform_s
    PRIVATE
        $<$<BOOL:${TFM_PARTITION_PROTECTED_STORAGE}>:${CMAKE_CURRENT_SOURCE_DIR}/ext/common/tfm_hal_ps.c>
        $<$<BOOL:${TFM_PARTITION_INTERNAL_TRUSTED_STORAGE}>:${CMAKE_CURRENT_SOURCE_DIR}/ext/common/tfm_hal_its.c>
        $<$<BOOL:${PLATFORM_DEFAULT_SYSTEM_RESET_HALT}>:${CMAKE_CURRENT_SOURCE_DIR}/ext/common/tfm_hal_reset_halt.c>
        $<$<BOOL:${PLATFORM_DEFAULT_UART_STDOUT}>:${CMAKE_CURRENT_SOURCE_DIR}/ext/common/uart_stdout.c>
        $<$<BOOL:${TFM_SPM_LOG_RAW_ENABLED}>:ext/common/tfm_hal_spm_logdev_peripheral.c>
        $<$<BOOL:${TFM_EXCEPTION_INFO_DUMP}>:ext/common/exception_info.c>
        ext/common/tfm_hal_memory_symbols.c
        $<$<BOOL:${PLATFORM_DEFAULT_ATTEST_HAL}>:ext/common/template/attest_hal.c>
        $<$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>:ext/common/template/nv_counters.c>
        $<$<BOOL:${PLATFORM_DEFAULT_ROTPK}>:ext/common/template/tfm_rotpk.c>
        $<$<BOOL:${PLATFORM_DEFAULT_NV_SEED}>:ext/common/template/crypto_nv_seed.c>
        $<$<AND:$<NOT:$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>>,$<BOOL:${TEST_S_ATTESTATION}>>:ext/common/template/tfm_initial_attest_pub_key.c>
        $<$<OR:$<AND:$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>,$<BOOL:${TFM_PARTITION_PROTECTED_STORAGE}>>,$<BOOL:${PLATFORM_DEFAULT_OTP}>>:ext/common/template/flash_otp_nv_counters_backend.c>
        $<$<BOOL:${PLATFORM_DEFAULT_OTP}>:ext/common/template/otp_flash.c>
        $<$<BOOL:${PLATFORM_DEFAULT_PROVISIONING}>:ext/common/provisioning.c>
        $<$<OR:$<BOOL:${TEST_S_FPU}>,$<BOOL:${TEST_NS_FPU}>>:${CMAKE_SOURCE_DIR}/platform/ext/common/test_interrupt.c>
)

# If this is not added to the tfm_s it will not correctly override the weak
# default handlers declared in assemebly, and will instead be discarded as they
# are not in use.
target_sources(tfm_s
    PRIVATE
        ext/common/faults.c
)

target_link_libraries(platform_s
    PUBLIC
        platform_common_interface
        platform_region_defs
        tfm_fih_headers
        cmsis_stack_override
    PRIVATE
        psa_interface
        tfm_partition_defs
        tfm_spm_defs # For tfm_spm_log.h
        $<$<BOOL:${TFM_PARTITION_CRYPTO}>:platform_crypto_keys>
        $<$<BOOL:${PLATFORM_DEFAULT_ATTEST_HAL}>:tfm_sprt>
        $<$<BOOL:${TFM_PARTITION_CRYPTO}>:crypto_service_mbedcrypto>
        $<$<BOOL:${TFM_PARTITION_INITIAL_ATTESTATION}>:tfm_attestation_defs>
        $<$<NOT:$<STREQUAL:${TFM_FIH_PROFILE},OFF>>:tfm_fih>
)

target_compile_definitions(platform_s
    PUBLIC
        TFM_SPM_LOG_LEVEL=${TFM_SPM_LOG_LEVEL}
        $<$<BOOL:${TFM_SPM_LOG_RAW_ENABLED}>:TFM_SPM_LOG_RAW_ENABLED>
        $<$<BOOL:${OTP_NV_COUNTERS_RAM_EMULATION}>:OTP_NV_COUNTERS_RAM_EMULATION>
        $<$<BOOL:${TFM_EXCEPTION_INFO_DUMP}>:TFM_EXCEPTION_INFO_DUMP>
        $<$<OR:$<VERSION_GREATER:${TFM_ISOLATION_LEVEL},1>,$<STREQUAL:"${TEST_PSA_API}","IPC">>:CONFIG_TFM_ENABLE_MEMORY_PROTECT>
        $<$<AND:$<BOOL:${TFM_PXN_ENABLE}>,$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv8.1-m.main>>:TFM_PXN_ENABLE>
        $<$<STREQUAL:${CONFIG_TFM_FLOAT_ABI},hard>:CONFIG_TFM_FLOAT_ABI=2>
        $<$<STREQUAL:${CONFIG_TFM_FLOAT_ABI},soft>:CONFIG_TFM_FLOAT_ABI=0>
        $<$<BOOL:${CONFIG_TFM_LAZY_STACKING}>:CONFIG_TFM_LAZY_STACKING>
        $<$<BOOL:${CONFIG_TFM_ENABLE_CP10CP11}>:CONFIG_TFM_ENABLE_CP10CP11>
        $<$<BOOL:${PLATFORM_DEFAULT_CRYPTO_KEYS}>:PLATFORM_DEFAULT_CRYPTO_KEYS>
        $<$<BOOL:${PLATFORM_DEFAULT_OTP}>:PLATFORM_DEFAULT_OTP>
    PRIVATE
        $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:SYMMETRIC_INITIAL_ATTESTATION>
        $<$<BOOL:${TFM_DUMMY_PROVISIONING}>:TFM_DUMMY_PROVISIONING>
        $<$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>:PLATFORM_DEFAULT_NV_COUNTERS>
        $<$<BOOL:${PLATFORM_DEFAULT_OTP_WRITEABLE}>:OTP_WRITEABLE>
)

target_compile_options(platform_s
    PUBLIC
        ${COMPILER_CP_FLAG}
)

#========================= Platform Crypto Keys ===============================#


if(TFM_PARTITION_CRYPTO)
    target_sources(platform_crypto_keys
        PRIVATE
            $<$<BOOL:${PLATFORM_DEFAULT_CRYPTO_KEYS}>:ext/common/template/crypto_keys.c>
    )

    target_link_libraries(platform_crypto_keys
        PRIVATE
            crypto_service_mbedcrypto
            tfm_psa_rot_partition_crypto
            platform_region_defs
            tfm_partition_defs
    )

    target_include_directories(platform_crypto_keys
        PRIVATE
            $<$<BOOL:${PLATFORM_DEFAULT_CRYPTO_KEYS}>:${CMAKE_SOURCE_DIR}/interface/include/crypto_keys>
            ${CMAKE_BINARY_DIR}/generated/interface/include
    )

    target_compile_definitions(platform_crypto_keys
        PRIVATE
            $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:SYMMETRIC_INITIAL_ATTESTATION>
            $<$<BOOL:${PLATFORM_DEFAULT_OTP}>:PLATFORM_DEFAULT_OTP>
            $<$<BOOL:${TEST_S_ATTESTATION}>:TEST_S_ATTESTATION>
            $<$<BOOL:${TEST_NS_ATTESTATION}>:TEST_NS_ATTESTATION>
    )

    target_compile_options(platform_crypto_keys
        PUBLIC
            ${COMPILER_CP_FLAG}
    )
endif()

#========================= Platform BL2 =======================================#
if(BL2)
    #TODO import policy
    target_include_directories(platform_bl2
        PUBLIC
            $<$<BOOL:${CRYPTO_HW_ACCELERATOR}>:${CMAKE_CURRENT_SOURCE_DIR}/ext/accelerator/interface>
            ${MCUBOOT_PATH}/boot/bootutil/include
            ${CMAKE_SOURCE_DIR}/bl2/ext/mcuboot/include
    )

    target_sources(platform_bl2
        PRIVATE
            ext/common/boot_hal_bl2.c
            $<$<BOOL:${PLATFORM_DEFAULT_UART_STDOUT}>:${CMAKE_CURRENT_SOURCE_DIR}/ext/common/uart_stdout.c>
            $<$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>:ext/common/template/nv_counters.c>
            $<$<BOOL:${PLATFORM_DEFAULT_ROTPK}>:ext/common/template/tfm_rotpk.c>
            $<$<OR:$<AND:$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>,$<BOOL:${TFM_PARTITION_PROTECTED_STORAGE}>>,$<BOOL:${PLATFORM_DEFAULT_OTP}>>:ext/common/template/flash_otp_nv_counters_backend.c>
            $<$<BOOL:${PLATFORM_DEFAULT_OTP}>:ext/common/template/otp_flash.c>
    )

    target_link_libraries(platform_bl2
        PUBLIC
            platform_common_interface
            platform_region_defs
            cmsis_stack_override
        PRIVATE
            bl2_hal
            mcuboot_config
    )

    target_compile_definitions(platform_bl2
        PUBLIC
            BL2
            MCUBOOT_${MCUBOOT_UPGRADE_STRATEGY}
            $<$<BOOL:${MCUBOOT_DIRECT_XIP_REVERT}>:MCUBOOT_DIRECT_XIP_REVERT>
            $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:SYMMETRIC_INITIAL_ATTESTATION>
            $<$<BOOL:${MCUBOOT_HW_KEY}>:MCUBOOT_HW_KEY>
            MCUBOOT_FIH_PROFILE_${MCUBOOT_FIH_PROFILE}
            $<$<BOOL:${PLATFORM_DEFAULT_OTP}>:PLATFORM_DEFAULT_OTP>
            $<$<BOOL:${OTP_NV_COUNTERS_RAM_EMULATION}>:OTP_NV_COUNTERS_RAM_EMULATION>
            $<$<BOOL:${TFM_DUMMY_PROVISIONING}>:TFM_DUMMY_PROVISIONING>
            $<$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>:PLATFORM_DEFAULT_NV_COUNTERS>
            $<$<BOOL:${PLATFORM_DEFAULT_OTP_WRITEABLE}>:OTP_WRITEABLE>
    )

endif()

#========================= Platform BL1_1 =====================================#

if(BL1 AND PLATFORM_DEFAULT_BL1)
    target_include_directories(platform_bl1_interface
        INTERFACE
            .
            ./include
            ./ext/cmsis
            ./ext/driver
    )

    target_link_libraries(platform_bl1_interface
        INTERFACE
            platform_region_defs
            platform_common_interface
            cmsis_stack_override
    )

    target_compile_definitions(platform_bl1_interface
        INTERFACE
            $<$<BOOL:${CRYPTO_HW_ACCELERATOR}>:CRYPTO_HW_ACCELERATOR>
            $<$<BOOL:${TFM_BL1_LOGGING}>:TFM_BL1_LOGGING>
            $<$<BOOL:${PLATFORM_DEFAULT_OTP}>:PLATFORM_DEFAULT_OTP>
            $<$<BOOL:${OTP_NV_COUNTERS_RAM_EMULATION}>:OTP_NV_COUNTERS_RAM_EMULATION>
            $<$<BOOL:${TFM_DUMMY_PROVISIONING}>:TFM_DUMMY_PROVISIONING>
            $<$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>:PLATFORM_DEFAULT_NV_COUNTERS>
            $<$<BOOL:${PLATFORM_DEFAULT_OTP_WRITEABLE}>:OTP_WRITEABLE>
    )

    target_sources(platform_bl1
        PRIVATE
            ./ext/common/boot_hal_bl1.c
            ./ext/common/uart_stdout.c
            $<$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>:ext/common/template/nv_counters.c>
            $<$<OR:$<AND:$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>,$<BOOL:${TFM_PARTITION_PROTECTED_STORAGE}>>,$<BOOL:${PLATFORM_DEFAULT_OTP}>>:ext/common/template/flash_otp_nv_counters_backend.c>
            $<$<BOOL:${PLATFORM_DEFAULT_OTP}>:ext/common/template/otp_flash.c>
    )

    target_link_libraries(platform_bl1
        PUBLIC
            platform_bl1_interface
        PRIVATE
            tfm_fih
            tfm_fih_headers
            $<$<BOOL:${CRYPTO_HW_ACCELERATOR}>:bl1_crypto_hw>
            tfm_boot_status
    )

endif()

#========================= Platform region defs ===============================#

#TODO maybe just link the other platforms to this
target_compile_definitions(platform_region_defs
    INTERFACE
        $<$<BOOL:${BL1}>:BL1>
        $<$<BOOL:${BL2}>:BL2>
        BL2_HEADER_SIZE=${BL2_HEADER_SIZE}
        BL2_TRAILER_SIZE=${BL2_TRAILER_SIZE}
        BL1_HEADER_SIZE=${BL1_HEADER_SIZE}
        BL1_TRAILER_SIZE=${BL1_TRAILER_SIZE}
        $<$<BOOL:${PLATFORM_DEFAULT_BL1}>:PLATFORM_DEFAULT_BL1>
        $<$<BOOL:${SECURE_UART1}>:SECURE_UART1>
        DAUTH_${DEBUG_AUTHENTICATION}
        $<$<BOOL:${MCUBOOT_IMAGE_NUMBER}>:MCUBOOT_IMAGE_NUMBER=${MCUBOOT_IMAGE_NUMBER}>
        $<$<STREQUAL:${MCUBOOT_SIGNATURE_TYPE},RSA>:MCUBOOT_SIGN_RSA>
        $<$<STREQUAL:${MCUBOOT_SIGNATURE_TYPE},RSA>:MCUBOOT_SIGN_RSA_LEN=${MCUBOOT_SIGNATURE_KEY_LEN}>
        $<$<STREQUAL:${MCUBOOT_EXECUTION_SLOT},2>:LINK_TO_SECONDARY_PARTITION>
        $<$<BOOL:${TEST_PSA_API}>:PSA_API_TEST_${TEST_PSA_API}>
        $<$<BOOL:${TFM_CODE_SHARING}>:CODE_SHARING>
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:relwithdebinfo>>:ENABLE_HEAP>
        PLATFORM_NS_NV_COUNTERS=${TFM_NS_NV_COUNTER_AMOUNT}
)

target_include_directories(platform_region_defs
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/ext/common
)
