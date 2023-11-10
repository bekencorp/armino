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

# Generate framework feature
set(PSA_FRAMEWORK_ISOLATION_LEVEL   ${TFM_ISOLATION_LEVEL})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/psa/framework_feature.h.in
               ${CMAKE_BINARY_DIR}/generated/interface/include/psa/framework_feature.h
               @ONLY)

if (EXISTS ${CMAKE_SOURCE_DIR}/platform/ext/target/${TFM_PLATFORM}/preload_ns.cmake)
    include(${CMAKE_SOURCE_DIR}/platform/ext/target/${TFM_PLATFORM}/preload_ns.cmake)
    tfm_toolchain_reload_compiler()
endif()

###################### PSA interface (header only) #############################

add_library(psa_interface INTERFACE)

target_include_directories(psa_interface
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/generated/interface/include
        $<$<BOOL:${TFM_MULTI_CORE_TOPOLOGY}>:${CMAKE_CURRENT_SOURCE_DIR}/include/multi_core>
)

target_link_libraries(psa_interface
    INTERFACE
        tfm_config
        tfm_partition_defs
)

target_compile_definitions(psa_interface
    INTERFACE
        $<$<BOOL:${CONFIG_TFM_ENABLE_CTX_MGMT}>:CONFIG_TFM_ENABLE_CTX_MGMT>
        $<$<BOOL:${TFM_ISOLATION_LEVEL}>:TFM_LVL=${TFM_ISOLATION_LEVEL}>
        $<$<BOOL:${CONFIG_TFM_USE_TRUSTZONE}>:CONFIG_TFM_USE_TRUSTZONE>
        $<$<BOOL:${TFM_MULTI_CORE_TOPOLOGY}>:TFM_MULTI_CORE_TOPOLOGY>
        $<$<BOOL:${CONFIG_TFM_PARTITION_META}>:CONFIG_TFM_PARTITION_META>
)

###################### PSA api (S lib) #########################################

target_sources(tfm_sprt
    PRIVATE
        $<$<BOOL:${TFM_PARTITION_INITIAL_ATTESTATION}>:${CMAKE_CURRENT_SOURCE_DIR}/src/tfm_attest_api.c>
        $<$<BOOL:${TFM_PARTITION_CRYPTO}>:${CMAKE_CURRENT_SOURCE_DIR}/src/tfm_crypto_api.c>
        $<$<BOOL:${TFM_PARTITION_FIRMWARE_UPDATE}>:${CMAKE_CURRENT_SOURCE_DIR}/src/tfm_fwu_api.c>
        $<$<BOOL:${TFM_PARTITION_INTERNAL_TRUSTED_STORAGE}>:${CMAKE_CURRENT_SOURCE_DIR}/src/tfm_its_api.c>
        $<$<BOOL:${TFM_PARTITION_PLATFORM}>:${CMAKE_CURRENT_SOURCE_DIR}/src/tfm_platform_api.c>
        $<$<BOOL:${TFM_PARTITION_PROTECTED_STORAGE}>:${CMAKE_CURRENT_SOURCE_DIR}/src/tfm_ps_api.c>
        ${CMAKE_CURRENT_SOURCE_DIR}/src/tfm_psa_call_pack.c
)

target_compile_definitions(tfm_sprt
    PUBLIC
        CONFIG_TFM_BUILDING_SPE=1
)

###################### Export configurations to NS #############################

if (TFM_MULTI_CORE_TOPOLOGY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/multi_core/tfm_mailbox_config.h.in
                   ${CMAKE_BINARY_DIR}/generated/interface/include/tfm_mailbox_config.h
                   NEWLINE_STYLE UNIX
    )
endif()
