#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
# Copyright (c) 2021-2022 Cypress Semiconductor Corporation (an Infineon
# company) or an affiliate of Cypress Semiconductor Corporation. All rights
# reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

add_library(tfm_spm STATIC)
add_library(tfm_spm_defs INTERFACE)
add_library(tfm_boot_status INTERFACE)
add_library(tfm_arch INTERFACE)

# Generate TF-M version
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/tfm_version.h.in
               ${CMAKE_BINARY_DIR}/generated/secure_fw/spm/include/tfm_version.h)

target_include_directories(tfm_spm
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/cmsis_psa
        ${CMAKE_CURRENT_SOURCE_DIR}/cmsis_psa/include
    PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/secure_fw/include
        ${CMAKE_BINARY_DIR}/generated
        ${CMAKE_BINARY_DIR}/generated/secure_fw/spm/include
        ${CMAKE_BINARY_DIR}/generated/secure_fw/spm/cmsis_psa
)

target_sources(tfm_spm
    PRIVATE
        ffm/tfm_boot_data.c
        ffm/utilities.c
        $<$<NOT:$<STREQUAL:${TFM_SPM_LOG_LEVEL},TFM_SPM_LOG_LEVEL_SILENCE>>:ffm/spm_log.c>
        cmsis_psa/arch/tfm_arch.c
        cmsis_psa/main.c
        cmsis_psa/spm_ipc.c
        $<$<BOOL:${CONFIG_TFM_PSA_API_CROSS_CALL}>:cmsis_psa/spm_cross_call.c>
        cmsis_psa/static_loader.c
        ffm/psa_api.c
        $<$<BOOL:${CONFIG_TFM_SPM_BACKEND_IPC}>:ffm/backend_ipc.c>
        $<$<BOOL:${CONFIG_TFM_SPM_BACKEND_SFN}>:ffm/backend_sfn.c>
        $<$<OR:$<BOOL:${CONFIG_TFM_FLIH_API}>,$<BOOL:${CONFIG_TFM_SLIH_API}>>:ffm/interrupt.c>
        $<$<BOOL:${CONFIG_TFM_STACK_WATERMARKS}>:ffm/stack_watermark.c>
        cmsis_psa/tfm_core_svcalls_ipc.c
        cmsis_psa/tfm_pools.c
        $<$<BOOL:${CONFIG_TFM_SPM_BACKEND_IPC}>:cmsis_psa/thread.c>
        $<$<BOOL:${TFM_NS_MANAGE_NSID}>:ns_client_ext/tfm_ns_ctx.c>
        ns_client_ext/tfm_spm_ns_ctx.c
        #TODO add other arches
        $<$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv8.1-m.main>:cmsis_psa/arch/tfm_arch_v8m_main.c>
        $<$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv8-m.base>:cmsis_psa/arch/tfm_arch_v8m_base.c>
        $<$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv8-m.main>:cmsis_psa/arch/tfm_arch_v8m_main.c>
        $<$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv6-m>:cmsis_psa/arch/tfm_arch_v6m_v7m.c>
        $<$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv7-m>:cmsis_psa/arch/tfm_arch_v6m_v7m.c>
        ${CMAKE_SOURCE_DIR}/platform/ext/common/tfm_hal_nvic.c
)

target_include_directories(tfm_spm_defs
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/cmsis_psa
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/boot>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/interface>
        ${CMAKE_CURRENT_SOURCE_DIR}/cmsis_psa/arch
)

target_link_libraries(tfm_spm
    PUBLIC
        tfm_arch
        tfm_spm_defs
    PRIVATE
        platform_s
        tfm_boot_status
        tfm_config
        tfm_partitions
        tfm_fih_headers
        tfm_sprt
        $<$<NOT:$<STREQUAL:${TFM_FIH_PROFILE},OFF>>:tfm_fih>
)

target_compile_definitions(tfm_spm
    PRIVATE
        $<$<BOOL:${PLATFORM_SVC_HANDLERS}>:PLATFORM_SVC_HANDLERS>
        $<$<CONFIG:Debug>:TFM_CORE_DEBUG>
        $<$<AND:$<BOOL:${BL2}>,$<BOOL:${CONFIG_TFM_BOOT_STORE_MEASUREMENTS}>>:BOOT_DATA_AVAILABLE>
        $<$<BOOL:${CONFIG_TFM_HALT_ON_CORE_PANIC}>:CONFIG_TFM_HALT_ON_CORE_PANIC>
        $<$<BOOL:${TFM_NS_MANAGE_NSID}>:TFM_NS_MANAGE_NSID>
        $<$<STREQUAL:${CONFIG_TFM_FLOAT_ABI},hard>:CONFIG_TFM_FLOAT_ABI=2>
        $<$<STREQUAL:${CONFIG_TFM_FLOAT_ABI},soft>:CONFIG_TFM_FLOAT_ABI=0>
        $<$<BOOL:${CONFIG_TFM_STACK_WATERMARKS}>:CONFIG_TFM_STACK_WATERMARKS>
)

target_compile_options(tfm_spm
    PUBLIC
        ${COMPILER_CP_FLAG}
)

# The veneers give warnings about not being properly declared so they get hidden
# to not overshadow _real_ warnings.
set_source_files_properties(tfm_secure_api.c
    PROPERTIES
        COMPILE_FLAGS
            $<$<C_COMPILER_ID:ARMClang>:-Wno-implicit-function-declaration>
            $<$<C_COMPILER_ID:GNU>:-Wno-implicit-function-declaration>
            $<$<C_COMPILER_ID:IAR>:>
)

############################ TFM arch ##########################################

target_include_directories(tfm_arch
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cmsis_psa/arch>
)

############################ Boot Status #######################################

target_include_directories(tfm_boot_status
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/boot>
)

############################# Secure veneers ###################################

if(CONFIG_TFM_USE_TRUSTZONE)
    # If this is added to the spm, it is discarded as it is not used. Since the
    # spm is a static library it can't generate veneers under all compilers so
    # instead this single file is added to the tfm_s target.
    target_sources(tfm_s
        PRIVATE
            $<$<BOOL:${TFM_NS_MANAGE_NSID}>:${CMAKE_CURRENT_SOURCE_DIR}/ns_client_ext/tfm_ns_client_ext.c>
    )
endif()
