#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
# Copyright (c) 2020-2022, Cypress Semiconductor Corp. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0079 NEW)
set(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set (CY_CORE_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/core-lib)
set (CY_PSOC64_DIR    ${CMAKE_CURRENT_SOURCE_DIR})

#========================= Libraries ==========================================#

set(MTB_PDL_CAT1_LIB_S_TARGET  mtb-pdl-cat1_lib_s)
set(MTB_PDL_CAT1_LIB_NS_TARGET mtb-pdl-cat1_lib_ns)
set(P64_UTILS_LIB_TARGET   p64_utils_lib)

add_subdirectory(libs/mtb-pdl-cat1)
add_subdirectory(libs/p64_utils)

#========================= Platform region defs ===============================#

target_include_directories(platform_region_defs
    INTERFACE
        partition
)
target_compile_definitions(platform_region_defs
    INTERFACE
        $<$<BOOL:${TEST_PSA_API}>:PSA_API_TEST_ENABLED>
)

#========================= Platform common defs ===============================#

target_sources(tfm_s
    PRIVATE
        $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/armclang/startup_psoc64_s.s>
        $<$<C_COMPILER_ID:ARMClang>:${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/TOOLCHAIN_ARM/cy_syslib_mdk.s>
        $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/gcc/startup_psoc64_s.S>
        $<$<C_COMPILER_ID:GNU>:${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/TOOLCHAIN_GCC_ARM/cy_syslib_gcc.S>
        $<$<C_COMPILER_ID:IAR>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/iar/startup_psoc64_s.s>
        $<$<C_COMPILER_ID:IAR>:${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/TOOLCHAIN_IAR/cy_syslib_iar.s>
)
target_add_scatter_file(tfm_s
    $<$<C_COMPILER_ID:ARMClang>:${PLATFORM_DIR}/ext/common/armclang/tfm_common_s.sct>
    $<$<C_COMPILER_ID:GNU>:${PLATFORM_DIR}/ext/common/gcc/tfm_common_s.ld>
    $<$<C_COMPILER_ID:IAR>:${PLATFORM_DIR}/ext/common/iar/tfm_common_s.icf>
)

if(NS)
    target_sources(tfm_ns
        PRIVATE
            $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/armclang/startup_psoc64_ns.s>
            $<$<C_COMPILER_ID:ARMClang>:${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/TOOLCHAIN_ARM/cy_syslib_mdk.s>
            $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/gcc/startup_psoc64_ns.S>
            $<$<C_COMPILER_ID:GNU>:${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/TOOLCHAIN_GCC_ARM/cy_syslib_gcc.S>
            $<$<C_COMPILER_ID:IAR>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/iar/startup_psoc64_ns.s>
            $<$<C_COMPILER_ID:IAR>:${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/TOOLCHAIN_IAR/cy_syslib_iar.s>
    )
    target_add_scatter_file(tfm_ns
            $<$<C_COMPILER_ID:ARMClang>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/armclang/psoc6_ns.sct>
            $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/gcc/psoc6_ns.ld>
            $<$<C_COMPILER_ID:IAR>:${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/iar/psoc6_ns.icf>
    )
    target_link_libraries(CMSIS_5_tfm_ns
        INTERFACE
            # CM4 without FP support uses CM3 libs
            CMSIS_5_RTX_CM3
    )
endif()

#========================= Platform Secure ====================================#

target_include_directories(platform_s
    PUBLIC
        .
        Device/Config
        Device/Include
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/include
        ${CY_MTB_PDL_CAT1_LIB_PATH}/devices/COMPONENT_CAT1A/include
        ${CY_CORE_LIB_PATH}/include
        Native_Driver/generated_source
        ./CMSIS_Driver/Config
        mailbox
    PRIVATE
        ${CY_MTB_PDL_CAT1_LIB_PATH}/cmsis/include
)

target_sources(platform_s
    PRIVATE
        ./mailbox/platform_multicore.c
        ./mailbox/platform_spe_mailbox.c
        ./Device/Source/system_psoc6_cm0plus.c
        ./Device/Source/psoc6_system_init_cm0p.c
        ./Native_Driver/generated_source/cycfg.c
        ./Native_Driver/generated_source/cycfg_clocks.c
        ./Native_Driver/generated_source/cycfg_clocks.c
        ./Native_Driver/generated_source/cycfg_peripherals.c
        ./Native_Driver/generated_source/cycfg_pins.c
        ./Native_Driver/generated_source/cycfg_routing.c
        ./Native_Driver/generated_source/cycfg_system.c
        ./Native_Driver/generated_source/cycfg_qspi_memslot.c
        ./tfm_interrupts.c
        ./tfm_hal_multi_core.c
        ./tfm_hal_isolation.c
        ./tfm_hal_platform.c
        ./CMSIS_Driver/Driver_Flash.c
        ./CMSIS_Driver/Driver_USART.c
        ./target_cfg.c
        ./driver_smpu.c
        ./driver_ppu.c
        ./driver_dap.c
        ./Device/Source/device_definition.c
        ${CMAKE_SOURCE_DIR}/platform/ext/common/tfm_hal_nvic.c
        $<$<BOOL:${TFM_PARTITION_PLATFORM}>:${CMAKE_CURRENT_SOURCE_DIR}/plat_test.c>
        $<$<BOOL:${TFM_PARTITION_PLATFORM}>:${CMAKE_CURRENT_SOURCE_DIR}/services/src/tfm_platform_system.c>
)

target_compile_definitions(platform_s
    PRIVATE
        RTE_USART5
)

target_link_libraries(platform_s
    PRIVATE
        ${MTB_PDL_CAT1_LIB_S_TARGET}
        ${P64_UTILS_LIB_TARGET}
)

#========================= Platform Non-Secure ================================#

target_sources(platform_ns
    PRIVATE
        ./mailbox/platform_ns_mailbox.c
        ./Device/Source/system_psoc6_cm4.c
        ./Device/Source/psoc6_system_init_cm4.c
        ./Native_Driver/generated_source/cycfg.c
        ./Native_Driver/generated_source/cycfg_clocks.c
        ./Native_Driver/generated_source/cycfg_clocks.c
        ./Native_Driver/generated_source/cycfg_peripherals.c
        ./Native_Driver/generated_source/cycfg_pins.c
        ./Native_Driver/generated_source/cycfg_routing.c
        ./Native_Driver/generated_source/cycfg_system.c
        ./Native_Driver/generated_source/cycfg_qspi_memslot.c
        ./mailbox/platform_multicore.c
        ./CMSIS_Driver/Driver_USART.c
        ./Device/Source/device_definition.c
        $<$<BOOL:${TFM_PARTITION_PLATFORM}>:${CMAKE_CURRENT_SOURCE_DIR}/plat_test.c>
)

target_include_directories(platform_ns
    PUBLIC
        .
        Device/Config
        Device/Include
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/include
        ${CY_MTB_PDL_CAT1_LIB_PATH}/devices/COMPONENT_CAT1A/include
        ${CY_CORE_LIB_PATH}/include
        Native_Driver/generated_source
        ./CMSIS_Driver/Config
    PRIVATE
        ${CY_MTB_PDL_CAT1_LIB_PATH}/cmsis/include
)

target_compile_definitions(platform_ns
    PUBLIC
        RTE_USART5
)

# device_cfg.h defines the number of mailbox queue slots to support multiple
# PSA client calls feature.
if (NS)
    target_include_directories(tfm_ns_interface
        INTERFACE
            Device/Config
    )

    target_link_libraries(platform_ns
        PRIVATE
            ${MTB_PDL_CAT1_LIB_NS_TARGET}
            tfm_ns_interface
    )
endif()

########################### export sources #####################################

set(INSTALL_INTERFACE_INC_DIR    ${TFM_INSTALL_PATH}/interface/include)
set(INSTALL_INTERFACE_SRC_DIR    ${TFM_INSTALL_PATH}/interface/src)
set(INSTALL_IMAGE_SIGNING_DIR    ${TFM_INSTALL_PATH}/image_signing)
set(INSTALL_MEMORY_LAYOUT_DIR    ${TFM_INSTALL_PATH}/memory_layout)

install(FILES       mailbox/platform_multicore.c
                    mailbox/platform_ns_mailbox.c
        DESTINATION ${INSTALL_INTERFACE_SRC_DIR})

install(FILES       mailbox/platform_multicore.h
                    mailbox/ns_ipc_config.h
                    Device/Config/device_cfg.h
        DESTINATION ${INSTALL_INTERFACE_INC_DIR})

install(FILES       partition/flash_layout.h
                    partition/region_defs.h
        DESTINATION ${INSTALL_MEMORY_LAYOUT_DIR})

install(FILES       security/policy/policy_multi_CM0_CM4_tfm_dev_certs.json
        DESTINATION ${INSTALL_IMAGE_SIGNING_DIR})
