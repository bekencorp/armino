#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
# Copyright (c) 2020 Nuvoton Technology Corp. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0076 NEW)
set(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

#========================= Platform region defs ===============================#
target_include_directories(platform_region_defs
    INTERFACE
        partition
)

#========================= Platform common defs ===============================#

# Specify the location of platform specific build dependencies.
target_sources(tfm_s
    PRIVATE
    $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/device/source/armclang/startup_cmsdk_m2351_s.s>
    $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/device/source/gcc/startup_cmsdk_m2351_s.S>
    $<$<C_COMPILER_ID:IAR>:${CMAKE_CURRENT_SOURCE_DIR}/device/source/iar/startup_cmsdk_m2351_s.s>
)
target_add_scatter_file(tfm_s
    $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/device/source/armclang/m2351_s.sct>
    $<$<C_COMPILER_ID:GNU>:${CMAKE_SOURCE_DIR}/platform/ext/common/gcc/tfm_common_s.ld>
    $<$<C_COMPILER_ID:IAR>:${CMAKE_SOURCE_DIR}/platform/ext/common/iar/tfm_common_s.icf>
)

if(NS)
    target_sources(tfm_ns
        PRIVATE
            $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/device/source/armclang/startup_cmsdk_m2351_ns.s>
            $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/device/source/gcc/startup_cmsdk_m2351_ns.S>
            $<$<C_COMPILER_ID:IAR>:${CMAKE_CURRENT_SOURCE_DIR}/device/source/iar/startup_cmsdk_m2351_ns.s>
    )
    target_add_scatter_file(tfm_ns
        $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/device/source/armclang/m2351_ns.sct>
        $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/device/source/gcc/m2351_ns.ld>
        $<$<C_COMPILER_ID:IAR>:${CMAKE_CURRENT_SOURCE_DIR}/device/source/iar/m2351_ns.icf>
    )
    target_link_libraries(CMSIS_5_tfm_ns
        INTERFACE
            CMSIS_5_RTX_V8MBN
    )
endif()

if(BL2)
    target_sources(bl2
        PRIVATE
            $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/device/source/armclang/startup_cmsdk_m2351_bl2.s>
            $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/device/source/gcc/startup_cmsdk_m2351_bl2.S>
            $<$<C_COMPILER_ID:IAR>:${CMAKE_CURRENT_SOURCE_DIR}/device/source/iar/startup_cmsdk_m2351_bl2.s>
    )

    target_add_scatter_file(bl2
            $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/device/source/armclang/m2351_bl2.sct>
            $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/device/source/gcc/m2351_bl2.ld>
            $<$<C_COMPILER_ID:IAR>:${CMAKE_CURRENT_SOURCE_DIR}/device/source/iar/m2351_bl2.icf>
    )
endif()

#========================= Platform Secure ====================================#

target_include_directories(platform_s
    PUBLIC
        .
        ../common/
        ../common/cmsis_drivers/config
        ../common/native_drivers
        partition
        device/include
        device/config
        bsp/Device/Nuvoton/M2351/Include
        bsp/Library/StdDriver/inc
        ../common/bsp/Include
        ../common/bsp/StdDriver/inc
        ${PLATFORM_DIR}/ext/cmsis
        ${PLATFORM_DIR}/ext/driver
)

target_sources(platform_s
    PRIVATE
        device/source/system_core_init.c
        device/source/device_definition.c
        ../common/cmsis_drivers/Driver_Flash.c
        ../common/cmsis_drivers/Driver_USART.c
        ../common/native_drivers/uart_cmsdk_drv.c
        ../common/native_drivers/mpu_armv8m_drv.c
        ../common/native_drivers/timer_cmsdk_drv.c
        bsp/Library/StdDriver/src/timer.c
        bsp/Library/StdDriver/src/fmc.c
        ../common/services/src/tfm_platform_system.c
        ../common/spm_hal.c
        ../common/tfm_hal_isolation.c
        ../common/tfm_hal_platform.c
        target_cfg.c
        ${PLATFORM_DIR}/ext/common/uart_stdout.c
        ${PLATFORM_DIR}/ext/common/tfm_platform.c
        ${CMAKE_SOURCE_DIR}/platform/ext/common/tfm_hal_isolation_mpu_v8m.c
        ${CMAKE_SOURCE_DIR}/platform/ext/common/tfm_hal_nvic.c
        $<$<BOOL:${TFM_PARTITION_PLATFORM}>:${CMAKE_CURRENT_SOURCE_DIR}/plat_test.c>
)

target_compile_options(platform_s
    PUBLIC
        ${COMPILER_CMSE_FLAG}
)

#========================= Platform Non-Secure ================================#

target_sources(platform_ns
    PRIVATE
        ../common/native_drivers/uart_cmsdk_drv.c
        ../common/cmsis_drivers/Driver_USART.c
        device/source/device_definition.c
        device/source/system_core_init.c
)

target_include_directories(platform_ns
    PUBLIC
        .
        ../common/
        ${PLATFORM_DIR}/..
        device/config
        device/include
        ../common/native_drivers
        bsp/Device/Nuvoton/M2351/Include
        bsp/Library/StdDriver/inc
        ../common/bsp/Include
        ../common/bsp/StdDriver/inc
        ../common/cmsis_drivers/config
)

#========================= Platform BL2 =======================================#

if(BL2)
    target_sources(platform_bl2
        PRIVATE
            device/source/device_definition.c
            device/source/system_core_init.c
            ../common/cmsis_drivers/Driver_Flash.c
            ../common/native_drivers/uart_cmsdk_drv.c
            ../common/cmsis_drivers/Driver_USART.c
            bsp/Library/StdDriver/src/fmc.c
    )
    target_include_directories(platform_bl2
        PUBLIC
            partition
            bsp/Device/Nuvoton/M2351/Include
            bsp/Library/StdDriver/inc
            ../common/bsp/Include
            ../common/bsp/StdDriver/inc
            device/include
            device/config
            ../common/cmsis_drivers/config
        PRIVATE
            .
            ../common/
            ${PLATFORM_DIR}/..
            ../common/native_drivers
    )
    target_compile_options(platform_bl2
        PUBLIC
            ${COMPILER_CMSE_FLAG}
    )
endif()
