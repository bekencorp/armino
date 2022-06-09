#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
# Copyright (c) 2020, Nordic Semiconductor ASA.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0076 NEW)
set(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

if (NOT NRF_BOARD_SELECTED)
    if (${TFM_PLATFORM} STREQUAL "nordic_nrf/nrf5340")
        set(hint nordic_nrf/nrf5340dk_nrf5340_cpuapp)
    else()
        set(hint nordic_nrf/nrf9160dk_nrf9160)
    endif()
    message(FATAL_ERROR "Please select one of the nRF boards. "
        "Instead of '${TFM_PLATFORM}', choose e.g. '${hint}'.")
endif()

#========================= Platform dependencies ===============================#

include(hal_nordic.cmake)

#========================= Platform common defs ===============================#

# Specify the location of platform specific build dependencies.

target_add_scatter_file(tfm_s
    $<$<C_COMPILER_ID:GNU>:${CMAKE_SOURCE_DIR}/platform/ext/target/nordic_nrf/common/core/gcc/nordic_nrf_s.ld>
)

if(NS)
    target_add_scatter_file(tfm_ns
        $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/gcc/nordic_nrf_ns.ld>
    )
    target_link_libraries(CMSIS_5_tfm_ns
        INTERFACE
            CMSIS_5_RTX_V8MMN
    )
endif()

if(BL2)
    target_add_scatter_file(bl2
            $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/gcc/nordic_nrf_bl2.ld>
    )
endif()

#========================= Platform Secure ====================================#

target_include_directories(platform_s
    PUBLIC
        .
        native_drivers
        ${HAL_NORDIC_PATH}/nrfx
        ${HAL_NORDIC_PATH}/nrfx/mdk
        ${HAL_NORDIC_PATH}/nrfx/drivers/include
        common
        ${PLATFORM_DIR}/..
        services/include
)

target_sources(platform_s
    PRIVATE
        cmsis_drivers/Driver_Flash.c
        ${HAL_NORDIC_PATH}/nrfx/drivers/src/nrfx_nvmc.c
        nrfx_glue.c
        native_drivers/mpu_armv8m_drv.c
        native_drivers/spu.c
        spm_hal.c
        tfm_hal_isolation.c
        tfm_hal_platform_common.c
        ${CMAKE_SOURCE_DIR}/platform/ext/common/tfm_hal_nvic.c
    PUBLIC
        plat_test.c
)

if(TFM_PARTITION_PLATFORM)
    target_sources(platform_s
        PRIVATE
            services/src/tfm_platform_hal_ioctl.c
            services/src/tfm_ioctl_core_s_api.c
    )

    target_sources(platform_ns
        PRIVATE
            services/src/tfm_ioctl_core_ns_api.c
    )
endif()

if(TFM_SPM_LOG_RAW_ENABLED)
    target_sources(platform_s
        PRIVATE
            cmsis_drivers/Driver_USART.c
            ${HAL_NORDIC_PATH}/nrfx/drivers/src/nrfx_uarte.c
    )
endif()

target_compile_options(platform_s
    PUBLIC
        ${COMPILER_CMSE_FLAG}
)

target_compile_definitions(platform_s
    PUBLIC
        $<$<BOOL:${TEST_PSA_API}>:PSA_API_TEST_ENABLED>
        $<$<BOOL:${NULL_POINTER_EXCEPTION_DETECTION}>:NULL_POINTER_EXCEPTION_DETECTION>
)

#========================= Platform Non-Secure ================================#

target_sources(platform_ns
    PRIVATE
        cmsis_drivers/Driver_USART.c
        ${HAL_NORDIC_PATH}/nrfx/drivers/src/nrfx_uarte.c
        nrfx_glue.c
        plat_test.c
)

target_include_directories(platform_ns
    PUBLIC
        .
        ${HAL_NORDIC_PATH}/nrfx
        ${HAL_NORDIC_PATH}/nrfx/mdk
        ${HAL_NORDIC_PATH}/nrfx/drivers/include
        common
        ${PLATFORM_DIR}/..
        services/include
    PRIVATE
        ${PLATFORM_DIR}/../interface/include
)

target_compile_definitions(platform_ns
    PUBLIC
        $<$<BOOL:${TEST_PSA_API}>:PSA_API_TEST_ENABLED>
)

#========================= Platform BL2 =======================================#

if(BL2)
    target_sources(platform_bl2
        PRIVATE
            cmsis_drivers/Driver_Flash.c
            cmsis_drivers/Driver_USART.c
            ${HAL_NORDIC_PATH}/nrfx/drivers/src/nrfx_uarte.c
            ${HAL_NORDIC_PATH}/nrfx/drivers/src/nrfx_nvmc.c
            nrfx_glue.c
    )

    target_include_directories(platform_bl2
        PUBLIC
            .
            ${HAL_NORDIC_PATH}/nrfx
            ${HAL_NORDIC_PATH}/nrfx/mdk
            ${HAL_NORDIC_PATH}/nrfx/drivers/include
            common
        PRIVATE
            ${PLATFORM_DIR}/..
    )
endif()

if (TFM_PARTITION_PLATFORM)
install(FILES       services/include/tfm_ioctl_core_api.h
        DESTINATION ${TFM_INSTALL_PATH}/interface/include)

endif()
