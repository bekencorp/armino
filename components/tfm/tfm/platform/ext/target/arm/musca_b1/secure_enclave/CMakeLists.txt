#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
# Copyright (c) 2021, Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0076 NEW)
set(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_MUSCA_B1_COMMON_DIR ${CMAKE_CURRENT_LIST_DIR}/../common)

#========================= Platform region defs ===============================#

target_include_directories(platform_region_defs
    INTERFACE
        partition
)

#========================= Platform common defs ===============================#

# Specify the location of platform specific build dependencies.
target_sources(tfm_s
    PRIVATE
    $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/armclang/startup_musca_b1_secure_enclave_s.s>
    $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/gcc/startup_musca_b1_secure_enclave_s.S>
    # This file contains an IRQ handler. Linking it from a different library to
    # overwrite the default weak function would require intrusive cmake hacks.
    tfm_interrupts.c
)
target_add_scatter_file(tfm_s
    $<$<C_COMPILER_ID:ARMClang>:${CMAKE_SOURCE_DIR}/platform/ext/common/armclang/tfm_common_s.sct>
    $<$<C_COMPILER_ID:GNU>:${CMAKE_SOURCE_DIR}/platform/ext/common/gcc/tfm_common_s.ld>
)

if(BL2)
    target_sources(bl2
        PRIVATE
            $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/armclang/startup_musca_b1_secure_enclave_bl2.s>
            $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/gcc/startup_musca_b1_secure_enclave_bl2.S>
    )
    target_add_scatter_file(bl2
            $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/armclang/musca_b1_secure_enclave_bl2.sct>
            $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/gcc/musca_b1_secure_enclave_bl2.ld>
    )
endif()

#========================= Platform Secure ====================================#

target_include_directories(platform_s
    PUBLIC
        .
        CMSIS_Driver
        CMSIS_Driver/Config
        Device/Config
        Device/Include
        Native_Driver
        partition
        services/include
        mailbox
        ${CMAKE_MUSCA_B1_COMMON_DIR}/Native_Driver
        ${CMAKE_MUSCA_B1_COMMON_DIR}/Libraries
    INTERFACE
        ${CMAKE_MUSCA_B1_COMMON_DIR}/cc312
)

target_sources(platform_s
    PRIVATE
        ${CMAKE_MUSCA_B1_COMMON_DIR}/CMSIS_Driver/Driver_GFC100_EFlash.c
        ${CMAKE_MUSCA_B1_COMMON_DIR}/Native_Driver/musca_b1_eflash_drv.c
        ${CMAKE_MUSCA_B1_COMMON_DIR}/Native_Driver/gfc100_eflash_drv.c
        ${CMAKE_MUSCA_B1_COMMON_DIR}/CMSIS_Driver/Driver_QSPI_Flash.c
        ${CMAKE_MUSCA_B1_COMMON_DIR}/Native_Driver/qspi_ip6514e_drv.c
        ${CMAKE_MUSCA_B1_COMMON_DIR}/Libraries/mt25ql_flash_lib.c
        ${CMAKE_MUSCA_B1_COMMON_DIR}/Native_Driver/mhu_v2_x.c
        Device/Source/device_definition.c
        Device/Source/system_core_init.c
        mailbox/platform_multicore.c
        mailbox/platform_spe_mailbox.c
        target_cfg.c
        tfm_hal_isolation.c
        tfm_hal_multi_core.c
        tfm_hal_platform.c
        uart_stdout.c
        ${CMAKE_SOURCE_DIR}/platform/ext/common/tfm_hal_nvic.c
        $<$<BOOL:${TFM_PARTITION_PLATFORM}>:${CMAKE_CURRENT_SOURCE_DIR}/services/src/tfm_platform_system.c>
)

#========================= Platform Non-Secure ================================#

# FIXME: platform_ns must have at least 1 source, even if it is not used
target_sources(platform_ns
    PRIVATE
        Device/Source/system_core_init.c
)

target_include_directories(platform_ns
    PUBLIC
        Device/Include
)

#========================= Platform BL2 =======================================#

if(BL2)
    target_sources(platform_bl2
        PRIVATE
            Device/Source/system_core_init.c
            Device/Source/device_definition.c
            uart_stdout.c
            ${CMAKE_MUSCA_B1_COMMON_DIR}/CMSIS_Driver/Driver_GFC100_EFlash.c
            ${CMAKE_MUSCA_B1_COMMON_DIR}/Native_Driver/gfc100_eflash_drv.c
            ${CMAKE_MUSCA_B1_COMMON_DIR}/Native_Driver/musca_b1_eflash_drv.c
    )

    target_include_directories(platform_bl2
        PUBLIC
            partition
            Device/Include
        INTERFACE
            ${CMAKE_MUSCA_B1_COMMON_DIR}/cc312
        PRIVATE
            .
            CMSIS_Driver/Config
            Device/Config
            Native_Driver
            ${MCUBOOT_PATH}/boot/bootutil/include # for fault_injection_hardening.h only
            ${CMAKE_BINARY_DIR}/bl2/ext/mcuboot # for mcuboot_config.h only
            ${CMAKE_MUSCA_B1_COMMON_DIR}/Native_Driver
            ${CMAKE_MUSCA_B1_COMMON_DIR}/Libraries
    )
endif()
