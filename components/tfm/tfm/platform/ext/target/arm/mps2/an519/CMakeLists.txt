#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
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
    $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/armclang/startup_cmsdk_mps2_an519_s.s>
    $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/gcc/startup_cmsdk_mps2_an519_s.S>
    $<$<C_COMPILER_ID:IAR>:${CMAKE_CURRENT_SOURCE_DIR}/iar/startup_cmsdk_mps2_an519_s.s>
)
target_add_scatter_file(tfm_s
    $<$<C_COMPILER_ID:ARMClang>:${CMAKE_SOURCE_DIR}/platform/ext/common/armclang/tfm_common_s.sct>
    $<$<C_COMPILER_ID:GNU>:${CMAKE_SOURCE_DIR}/platform/ext/common/gcc/tfm_common_s.ld>
    $<$<C_COMPILER_ID:IAR>:${CMAKE_SOURCE_DIR}/platform/ext/common/iar/tfm_common_s.icf>
)

if(NS)
    target_sources(tfm_ns
        PRIVATE
            $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/armclang/startup_cmsdk_mps2_an519_ns.s>
            $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/gcc/startup_cmsdk_mps2_an519_ns.S>
            $<$<C_COMPILER_ID:IAR>:${CMAKE_CURRENT_SOURCE_DIR}/iar/startup_cmsdk_mps2_an519_ns.s>
    )
    target_add_scatter_file(tfm_ns
        $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/armclang/mps2_an519_ns.sct>
        $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/gcc/mps2_an519_ns.ld>
        $<$<C_COMPILER_ID:IAR>:${CMAKE_CURRENT_SOURCE_DIR}/iar/mps2_an519_ns.icf>
    )
    target_link_libraries(CMSIS_5_tfm_ns
        INTERFACE
            CMSIS_5_RTX_V8MBN
    )
endif()

if(BL2)
    target_sources(bl2
        PRIVATE
            $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/armclang/startup_cmsdk_mps2_an519_bl2.s>
            $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/gcc/startup_cmsdk_mps2_an519_bl2.S>
            $<$<C_COMPILER_ID:IAR>:${CMAKE_CURRENT_SOURCE_DIR}/iar/startup_cmsdk_mps2_an519_bl2.s>
    )
    target_add_scatter_file(bl2
            $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/armclang/mps2_an519_bl2.sct>
            $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/gcc/mps2_an519_bl2.ld>
            $<$<C_COMPILER_ID:IAR>:${CMAKE_CURRENT_SOURCE_DIR}/iar/mps2_an519_bl2.icf>
    )
endif()

#========================= Platform Secure ====================================#

target_include_directories(platform_s
    PUBLIC
        .
        ../common
        cmsis_drivers
        native_drivers
        partition
        retarget
        cmsis_core
        native_drivers/timer_cmsdk
        ${PLATFORM_DIR}/..
)

target_sources(platform_s
    PRIVATE
        cmsis_drivers/Driver_Flash.c
        cmsis_drivers/Driver_MPC.c
        cmsis_drivers/Driver_PPC.c
        cmsis_drivers/Driver_USART.c
        retarget/platform_retarget_dev.c
        cmsis_core/system_cmsdk_mps2_an519.c
        native_drivers/mpc_sie200_drv.c
        native_drivers/mpu_armv8m_drv.c
        native_drivers/ppc_sse200_drv.c
        spm_hal.c
        $<$<BOOL:${TFM_PSA_API}>:${CMAKE_CURRENT_SOURCE_DIR}/tfm_interrupts.c>
        tfm_hal_isolation.c
        tfm_hal_platform.c
        target_cfg.c
        native_drivers/ppc_sse200_drv.c
        native_drivers/arm_uart_drv.c
        $<$<AND:$<NOT:$<BOOL:${TEST_NS_SLIH_IRQ}>>,$<NOT:$<BOOL:${TEST_NS_FLIH_IRQ}>>>:${CMAKE_CURRENT_SOURCE_DIR}/native_drivers/timer_cmsdk/timer_cmsdk.c>
        ${CMAKE_SOURCE_DIR}/platform/ext/common/tfm_hal_nvic.c
        ${CMAKE_SOURCE_DIR}/platform/ext/common/tfm_hal_isolation_mpu_v8m.c
        $<$<OR:$<BOOL:${TFM_S_REG_TEST}>,$<BOOL:${TFM_NS_REG_TEST}>>:${CMAKE_CURRENT_SOURCE_DIR}/plat_test.c>
        $<$<BOOL:${TFM_PARTITION_PLATFORM}>:${CMAKE_CURRENT_SOURCE_DIR}/services/src/tfm_platform_system.c>
)

target_compile_options(platform_s
    PUBLIC
        ${COMPILER_CMSE_FLAG}
)

target_sources(tfm_sprt
    PRIVATE
        # SLIH test Partition and FLIH test Partition access the timer as ARoT Partitions.
        # Put the driver to SPRT so that both SLIH and FLIH tests can access it.
        $<$<OR:$<BOOL:${TEST_NS_SLIH_IRQ}>,$<BOOL:${TEST_NS_FLIH_IRQ}>>:${CMAKE_CURRENT_SOURCE_DIR}/native_drivers/timer_cmsdk/timer_cmsdk.c>
)

#========================= Platform Non-Secure ================================#

target_sources(platform_ns
    PRIVATE
        native_drivers/arm_uart_drv.c
        native_drivers/timer_cmsdk/timer_cmsdk.c
        cmsis_drivers/Driver_USART.c
        retarget/platform_retarget_dev.c
        cmsis_core/system_cmsdk_mps2_an519.c
        $<$<BOOL:${TFM_PARTITION_PLATFORM}>:${CMAKE_CURRENT_SOURCE_DIR}/plat_test.c>
)

target_include_directories(platform_ns
    PUBLIC
        .
        ../common
        ${PLATFORM_DIR}/..
        retarget
        cmsis_core
        native_drivers
        native_drivers/timer_cmsdk
)

#========================= Platform BL2 =======================================#

if(BL2)
    target_sources(platform_bl2
        PRIVATE
            retarget/platform_retarget_dev.c
            cmsis_core/system_cmsdk_mps2_an519.c
            cmsis_drivers/Driver_Flash.c
            native_drivers/arm_uart_drv.c
            cmsis_drivers/Driver_USART.c
    )

    target_include_directories(platform_bl2
        PUBLIC
            partition
            cmsis_core
            retarget
        PRIVATE
            .
            ${PLATFORM_DIR}/..
            native_drivers
    )
endif()
