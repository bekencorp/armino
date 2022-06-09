#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0076 NEW)
set(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_MUSCA_B1_SSE_200_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_MUSCA_B1_COMMON_DIR ${CMAKE_CURRENT_LIST_DIR}/../common)

#========================= Platform region defs ===============================#

target_include_directories(platform_region_defs
    INTERFACE
        partition
)

#========================= Platform common defs ===============================#

if (${CMAKE_C_COMPILER_ID} STREQUAL IAR AND FORWARD_PROT_MSG)
    message(FATAL_ERROR "IAR is currently not supported if FORWARD_PROT_MSG is set.")
endif()

# Specify the location of platform specific build dependencies.
target_sources(tfm_s
    PRIVATE
    $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/armclang/startup_cmsdk_musca_s.s>
    $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/gcc/startup_cmsdk_musca_s.S>
    $<$<C_COMPILER_ID:IAR>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/iar/startup_cmsdk_musca_s.s>
)

# Use a common scatter file for Isolation L1 and L2, a dedicated one for Isolation L3
# IAR is not supported for L3
target_add_scatter_file(tfm_s
    $<$<AND:$<VERSION_LESS:${TFM_ISOLATION_LEVEL},3>,$<C_COMPILER_ID:ARMClang>>:${CMAKE_SOURCE_DIR}/platform/ext/common/armclang/tfm_common_s.sct>
    $<$<AND:$<VERSION_LESS:${TFM_ISOLATION_LEVEL},3>,$<C_COMPILER_ID:GNU>>:${CMAKE_SOURCE_DIR}/platform/ext/common/gcc/tfm_common_s.ld>
    $<$<AND:$<VERSION_LESS:${TFM_ISOLATION_LEVEL},3>,$<C_COMPILER_ID:IAR>>:${CMAKE_SOURCE_DIR}/platform/ext/common/iar/tfm_common_s.icf>
    $<$<AND:$<VERSION_EQUAL:${TFM_ISOLATION_LEVEL},3>,$<C_COMPILER_ID:ARMClang>>:${CMAKE_BINARY_DIR}/generated/platform/ext/common/armclang/tfm_isolation_l3.sct>
    $<$<AND:$<VERSION_EQUAL:${TFM_ISOLATION_LEVEL},3>,$<C_COMPILER_ID:GNU>>:${CMAKE_BINARY_DIR}/generated/platform/ext/common/gcc/tfm_isolation_l3.ld>
    $<$<AND:$<VERSION_EQUAL:${TFM_ISOLATION_LEVEL},3>,$<C_COMPILER_ID:IAR>>:${CMAKE_BINARY_DIR}/generated/platform/ext/common/iar/tfm_isolation_l3.icf>
)

if(NS)
    target_sources(tfm_ns
        PRIVATE
            $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/armclang/startup_cmsdk_musca_ns.s>
            $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/gcc/startup_cmsdk_musca_ns.S>
            $<$<C_COMPILER_ID:IAR>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/iar/startup_cmsdk_musca_ns.s>
    )
    target_add_scatter_file(tfm_ns
        $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/armclang/musca_ns.sct>
        $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/gcc/musca_ns.ld>
        $<$<C_COMPILER_ID:IAR>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/iar/musca_ns.icf>
    )
    target_link_libraries(CMSIS_5_tfm_ns
        INTERFACE
            CMSIS_5_RTX_V8MMN
    )
endif()

if(BL2)
    target_sources(bl2
        PRIVATE
            $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/armclang/startup_cmsdk_musca_bl2.s>
            $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/gcc/startup_cmsdk_musca_bl2.S>
            $<$<C_COMPILER_ID:IAR>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/iar/startup_cmsdk_musca_bl2.s>
    )
    target_add_scatter_file(bl2
            $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/armclang/musca_bl2.sct>
            $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/gcc/musca_bl2.ld>
            $<$<C_COMPILER_ID:IAR>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/iar/musca_bl2.icf>
    )
endif()

if(BL0)
    add_subdirectory(bl0)
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
        ${CMAKE_MUSCA_B1_COMMON_DIR}/Libraries
        ${CMAKE_MUSCA_B1_COMMON_DIR}/Native_Driver
        $<$<BOOL:${FORWARD_PROT_MSG}>:${CMAKE_CURRENT_SOURCE_DIR}/mailbox>
    INTERFACE
        ${CMAKE_MUSCA_B1_COMMON_DIR}/cc312
)

target_sources(platform_s
    PRIVATE
        $<$<NOT:$<BOOL:${FORWARD_PROT_MSG}>>:${CMAKE_MUSCA_B1_COMMON_DIR}/CMSIS_Driver/Driver_QSPI_Flash.c>
        $<$<NOT:$<BOOL:${FORWARD_PROT_MSG}>>:${CMAKE_MUSCA_B1_COMMON_DIR}/CMSIS_Driver/Driver_GFC100_EFlash.c>
        CMSIS_Driver/Driver_MPC.c
        CMSIS_Driver/Driver_PPC.c
        CMSIS_Driver/Driver_USART.c
        Device/Source/device_definition.c
        Device/Source/system_core_init.c
        Native_Driver/mpc_sie200_drv.c
        Native_Driver/mpu_armv8m_drv.c
        Native_Driver/ppc_sse200_drv.c
        Native_Driver/gpio_cmsdk_drv.c
        $<$<NOT:$<BOOL:${FORWARD_PROT_MSG}>>:${CMAKE_MUSCA_B1_COMMON_DIR}/Libraries/mt25ql_flash_lib.c>
        $<$<NOT:$<BOOL:${FORWARD_PROT_MSG}>>:${CMAKE_MUSCA_B1_COMMON_DIR}/Native_Driver/qspi_ip6514e_drv.c>
        $<$<NOT:$<BOOL:${FORWARD_PROT_MSG}>>:${CMAKE_MUSCA_B1_COMMON_DIR}/Native_Driver/gfc100_eflash_drv.c>
        $<$<NOT:$<BOOL:${FORWARD_PROT_MSG}>>:${CMAKE_MUSCA_B1_COMMON_DIR}/Native_Driver/musca_b1_eflash_drv.c>
        Native_Driver/musca_b1_scc_drv.c
        spm_hal.c
        $<$<BOOL:${TFM_PSA_API}>:${CMAKE_CURRENT_SOURCE_DIR}/tfm_interrupts.c>
        tfm_hal_isolation.c
        tfm_hal_platform.c
        target_cfg.c
        Native_Driver/ppc_sse200_drv.c
        Native_Driver/uart_pl011_drv.c
        $<$<NOT:$<BOOL:${TEST_NS_SLIH_IRQ}>>:${CMAKE_CURRENT_SOURCE_DIR}/Native_Driver/timer_cmsdk_drv.c>
        ${CMAKE_SOURCE_DIR}/platform/ext/common/tfm_hal_nvic.c
        ${CMAKE_SOURCE_DIR}/platform/ext/common/tfm_hal_isolation_mpu_v8m.c
        $<$<OR:$<BOOL:${TFM_S_REG_TEST}>,$<BOOL:${TFM_NS_REG_TEST}>>:${CMAKE_CURRENT_SOURCE_DIR}/plat_test.c>
        $<$<BOOL:${TFM_PARTITION_PLATFORM}>:${CMAKE_CURRENT_SOURCE_DIR}/services/src/tfm_platform_system.c>
        $<$<BOOL:${FORWARD_PROT_MSG}>:${CMAKE_CURRENT_SOURCE_DIR}/mailbox/platform_multicore.c>
        $<$<BOOL:${FORWARD_PROT_MSG}>:${CMAKE_CURRENT_SOURCE_DIR}/mailbox/platform_ns_mailbox.c>
        $<$<BOOL:${FORWARD_PROT_MSG}>:${CMAKE_MUSCA_B1_COMMON_DIR}/Native_Driver/mhu_v2_x.c>
        $<$<BOOL:${FORWARD_PROT_MSG}>:${CMAKE_CURRENT_SOURCE_DIR}/dummy_nv_counters.c>
        $<$<BOOL:${FORWARD_PROT_MSG}>:${CMAKE_CURRENT_SOURCE_DIR}/dummy_otp.c>
        $<$<BOOL:${FORWARD_PROT_MSG}>:${CMAKE_CURRENT_SOURCE_DIR}/dummy_provisioning.c>
)

target_compile_options(platform_s
    PUBLIC
        ${COMPILER_CMSE_FLAG}
)

target_sources(tfm_sprt
    PRIVATE
        # The SLIH test Partition accesses the timer as ARoT Partitions.
        # Put the driver to SPRT so that the SLIH tests can access it.
        $<$<BOOL:${TEST_NS_SLIH_IRQ}>:${CMAKE_CURRENT_SOURCE_DIR}/Native_Driver/timer_cmsdk_drv.c>
)

target_compile_definitions(platform_s
    PUBLIC
        $<$<BOOL:${FORWARD_PROT_MSG}>:FORWARD_PROT_MSG=${FORWARD_PROT_MSG}>
        $<$<BOOL:${FORWARD_PROT_MSG}>:PSA_PROXY_ADDR_TRANSLATION=1>
        $<$<BOOL:${FORWARD_PROT_MSG}>:LINK_TO_EFLASH1=1>
        $<$<BOOL:${FORWARD_PROT_MSG}>:USE_SECURE_ENCLAVE_MAILBOX=1>
)

#========================= Platform Non-Secure ================================#

target_sources(platform_ns
    PRIVATE
        Native_Driver/uart_pl011_drv.c
        Native_Driver/timer_cmsdk_drv.c
        CMSIS_Driver/Driver_USART.c
        Device/Source/system_core_init.c
        Device/Source/device_definition.c
        $<$<BOOL:${TFM_PARTITION_PLATFORM}>:${CMAKE_CURRENT_SOURCE_DIR}/plat_test.c>
        $<$<BOOL:${PLATFORM_GPLED_ENABLED}>:${CMAKE_CURRENT_SOURCE_DIR}/services/src/tfm_ioctl_ns_api.c>
        $<$<BOOL:${PLATFORM_GPLED_ENABLED}>:${CMAKE_CURRENT_SOURCE_DIR}/services/src/tfm_gpled_api.c>
)

target_include_directories(platform_ns
    PUBLIC
        .
        CMSIS_Driver/Config
        Device/Config
        Device/Include
        Native_Driver
        services/include
        ${CMAKE_MUSCA_B1_COMMON_DIR}/Native_Driver
        ${CMAKE_MUSCA_B1_COMMON_DIR}/Libraries
)

#========================= Platform BL2 =======================================#

if(BL2)
    target_sources(platform_bl2
        PRIVATE
            Device/Source/system_core_init.c
            Device/Source/device_definition.c
            ${CMAKE_MUSCA_B1_COMMON_DIR}/CMSIS_Driver/Driver_GFC100_EFlash.c
            Native_Driver/uart_pl011_drv.c
            CMSIS_Driver/Driver_USART.c
            ${CMAKE_MUSCA_B1_COMMON_DIR}/Native_Driver/gfc100_eflash_drv.c
            ${CMAKE_MUSCA_B1_COMMON_DIR}/Native_Driver/musca_b1_eflash_drv.c
            Native_Driver/musca_b1_scc_drv.c
            $<$<BOOL:${FORWARD_PROT_MSG}>:${CMAKE_CURRENT_SOURCE_DIR}/dummy_nv_counters.c>
            $<$<BOOL:${FORWARD_PROT_MSG}>:${CMAKE_CURRENT_SOURCE_DIR}/dummy_otp.c>
            $<$<BOOL:${FORWARD_PROT_MSG}>:${CMAKE_CURRENT_SOURCE_DIR}/dummy_provisioning.c>
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
