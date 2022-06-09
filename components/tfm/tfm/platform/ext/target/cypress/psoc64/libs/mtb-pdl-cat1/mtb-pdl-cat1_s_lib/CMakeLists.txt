#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.13)

project("Cypress PSoC 6 Peripheral Driver Library" LANGUAGES C ASM)

cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0079 NEW)

if (NOT DEFINED MTB_PDL_CAT1_LIB_S_TARGET)
    set(MTB_PDL_CAT1_LIB_S_TARGET mtb-pdl-cat1_lib_s)
endif()

########################## MTB_PDL_CAT1_LIB_S ######################################

add_library(${MTB_PDL_CAT1_LIB_S_TARGET} STATIC)

target_sources(${MTB_PDL_CAT1_LIB_S_TARGET}
    PRIVATE
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_crypto_core_hw.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_crypto_core_hw_v1.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_device.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_flash.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_gpio.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_ipc_drv.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_ipc_pipe.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_ipc_sema.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_pra.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_pra_cfg.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_prot.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_scb_common.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_scb_uart.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_sysclk.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_sysint.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_syslib.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_syspm.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_systick.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_tcpwm_counter.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_trigmux.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_wdt.c

        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_crypto_core_aes_v1.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_crypto_core_aes_v2.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_crypto_core_ecc_domain_params.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_crypto_core_ecc_ecdsa.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_crypto_core_ecc_key_gen.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_crypto_core_ecc_nist_p.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_crypto_core_hw_v1.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_crypto_core_mem_v1.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_crypto_core_mem_v2.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_crypto_core_sha_v1.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_crypto_core_sha_v2.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_crypto_core_trng_v1.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_crypto_core_trng_v2.c
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/source/cy_crypto_core_vu.c
)

target_include_directories(${MTB_PDL_CAT1_LIB_S_TARGET}
    PUBLIC
        ${CY_MTB_PDL_CAT1_LIB_PATH}/cmsis/include
        ${CY_MTB_PDL_CAT1_LIB_PATH}/devices/COMPONENT_CAT1A/include
        ${CY_MTB_PDL_CAT1_LIB_PATH}/drivers/include
        ${CY_CORE_LIB_PATH}/include
        ${CY_PLATFORM_DIR}/Device/Include
)
