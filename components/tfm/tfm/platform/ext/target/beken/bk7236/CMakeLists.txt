#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0076 NEW)

set(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

#========================= Platform region defs ===============================#
string(REGEX REPLACE "beken/" "" SOC "${TFM_PLATFORM}")
target_include_directories(platform_region_defs
    INTERFACE
        partition
        ${ARMINO_MIDDLEWARE_REPO_PATH}/boards/${SOC}/partitions
	${PARTITION_HDR_DIR}
)

#========================= Platform common defs ===============================#
# Specify the location of platform specific build dependencies.
target_sources(tfm_s
    PRIVATE
    $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/gcc/startup_cmsdk_bk7236_s.S>
)

# Use a common scatter file for Isolation L1 and L2, a dedicated one for Isolation L3
# IAR is not supported for L3
target_add_scatter_file(tfm_s
    $<$<AND:$<VERSION_LESS:${TFM_ISOLATION_LEVEL},3>,$<C_COMPILER_ID:GNU>>:${CMAKE_SOURCE_DIR}/platform/ext/common/gcc/tfm_common_s.ld>
    $<$<AND:$<VERSION_EQUAL:${TFM_ISOLATION_LEVEL},3>,$<C_COMPILER_ID:GNU>>:${CMAKE_BINARY_DIR}/generated/platform/ext/common/gcc/tfm_isolation_l3.ld>
)

if(NS)
    target_sources(tfm_ns
        PRIVATE
            $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/gcc/startup_cmsdk_bk7236_ns.S>
    )
    target_add_scatter_file(tfm_ns
        $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/gcc/bk7236_ns.ld>
    )
    
    #TODO wangzhilei
    target_compile_definitions(tfm_ns
	PUBLIC
		CONFIG_TFM_NS
		DOMAIN_NS    	
		__EN_ICACHE
		__EN_DCACHE	
    )
    
    target_link_libraries(CMSIS_5_tfm_ns
        INTERFACE
            CMSIS_5_RTX_V8MBN
    )
endif()

if(BL2)
    target_sources(bl2
        PRIVATE
            $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/gcc/startup_cmsdk_bk7236_bl2.S>
    )
    target_add_scatter_file(bl2
            $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/gcc/bk7236_bl2.ld>
    )
endif()

#========================= Platform Secure ====================================#
set(platform_s_inc
        .
        ../common
        cmsis_drivers
        native_drivers
        partition
        ${ARMINO_MIDDLEWARE_REPO_PATH}/boards/${SOC}/partitions
	${PARTITION_HDR_DIR}
        retarget
        cmsis_core
        native_drivers/timer_cmsdk
        ${PLATFORM_DIR}/../bl2/ext/mcuboot/include
        ${PLATFORM_DIR}/..
)

set(platform_s_src
        bl2/flash_map.c
        cmsis_drivers/Driver_Flash.c
        cmsis_drivers/Driver_USART.c
        cmsis_core/system_cmsdk_bk7236.c
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

target_include_directories(platform_s PUBLIC ${platform_s_inc})
target_sources(platform_s PRIVATE ${platform_s_src})

target_sources(tfm_sprt
    PRIVATE
        # SLIH test Partition and FLIH test Partition access the timer as ARoT Partitions.
        # Put the driver to SPRT so that both SLIH and FLIH tests can access it.
        $<$<OR:$<BOOL:${TEST_NS_SLIH_IRQ}>,$<BOOL:${TEST_NS_FLIH_IRQ}>>:${CMAKE_CURRENT_SOURCE_DIR}/native_drivers/timer_cmsdk/timer_cmsdk.c>
)

target_compile_options(platform_s
    PUBLIC
        ${COMPILER_CMSE_FLAG}
)

#========================= Platform Non-Secure ================================#
target_sources(platform_ns
    PRIVATE
        native_drivers/arm_uart_drv.c
        native_drivers/timer_cmsdk/timer_cmsdk.c
        cmsis_drivers/Driver_USART.c
        cmsis_core/system_cmsdk_bk7236.c
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
    set(platform_bl2_src
        PRIVATE
            cmsis_core/system_cmsdk_bk7236.c
            cmsis_drivers/Driver_Flash.c
            native_drivers/arm_uart_drv.c
            cmsis_drivers/Driver_USART.c
            bl2/boot_hal_bl2.c
            bl2/int_bl2.c
            bl2/hooks_bl2.c
            bl2/flash_map.c
    )

	target_compile_definitions(platform_bl2
	    PUBLIC
		-DMCUBOOT_IMAGE_ACCESS_HOOKS=1
		#TODO peter, need to enable it finally
		-DCRYPTO_HW_ACCELERATOR
		-DCONFIG_ENABLE_MCUBOOT_BL2=1
	)

    set(platform_bl2_inc
        PUBLIC
            partition
            ${ARMINO_MIDDLEWARE_REPO_PATH}/boards/${SOC}/partitions
            ${PARTITION_HDR_DIR}
            cmsis_core
            retarget
            bl2

        PRIVATE
            .
            ${PLATFORM_DIR}/..
            native_drivers
            ${MCUBOOT_PATH}/boot/bootutil/include
            ${MCUBOOT_PATH}/boot/bootutil/config

            ${CMAKE_BINARY_DIR}/bl2/ext/mcuboot
            ${PLATFORM_DIR}/../bl2/ext/mcuboot/include
    )

	if(CRYPTO_HW_ACCELERATOR)
		list(APPEND platform_bl2_inc
			../common/hal/inc/
			../common/hal/accelerator/spe/inc/hal
			../common/hal/accelerator/spe/inc/pal
			../common/hal/accelerator/dubhe_driver/inc/crypto
			../armino/driver/sys_ctrl/
			./armino/driver/include/
			./armino/include/driver/
	        secure_dbg/device_agent/inc/
	        secure_dbg/device_agent/src/
		)
		list(APPEND platform_bl2_src
	        secure_dbg/device_agent/src/hal_jtag.c
	        secure_dbg/device_agent/src/hal_comm_adapt.c
	        secure_dbg/device_agent/src/hal_comm_channel.c
	        secure_dbg/device_agent/src/hal_secure_debug.c
	        secure_dbg/device_agent/src/securedebug.c
	        secure_dbg/device_agent/src/securedebug_protocol.c
	        secure_dbg/device_agent/src/secure_debug_app.c
		)
	endif()

	target_include_directories(platform_bl2 PUBLIC ${platform_bl2_inc})
	target_sources(platform_bl2 PRIVATE ${platform_bl2_src})
endif()

add_subdirectory(armino)
