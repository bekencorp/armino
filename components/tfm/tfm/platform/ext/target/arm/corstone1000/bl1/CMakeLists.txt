#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

project("BL1 Bootloader" VERSION 0.0.1 LANGUAGES C ASM)

# BL1 only loads the BL2 image, image number always equals 1
set(BL1_IMAGE_NUMBER 1)

# Version of BL2 image
set(BL2_IMAGE_VERSION "0.1.0")

add_executable(bl1)

set_target_properties(bl1
    PROPERTIES
        SUFFIX ".axf"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_link_options(bl1
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-Wl,-Map=${CMAKE_BINARY_DIR}/bin/bl1.map>
)

add_convert_to_bin_target(bl1)

# bl2_mbedcrypto reused as it is, but it pulls the MCUBOOT_IMAGE_NUMBER=${MCUBOOT_IMAGE_NUMBER}
# configuration, where image number is 3. (Coming from BL2 build). To not to collide with BL1's
# build where image number is 1 mbedcrypto library is separated from the build of other source
# files.
target_link_libraries(bl1
    PRIVATE
        bl1_main
        bl2_mbedcrypto
)

# add_convert_to_bin_target(bl1) requires at least one source file added to bl1. This sources will
# be built with wrong image number macro (value coming from BL2 config), so the start-up files
# added here, as those not use this image number macro.
target_sources(bl1
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/../Device/Source/gcc/startup_corstone1000_bl.S>
)

# Needed for the GCC startup files
target_include_directories(bl1
    PRIVATE
        ${CMAKE_SOURCE_DIR}/platform/include
)

# target_add_scatter_file(bl1) cannot be used as it would add the platform_region_defs dependency
# to bl1, again pulling the image number property matching with BL2 build, so scatter setup done
# here by hand.
target_link_options(bl1
    PRIVATE
        -T $<TARGET_OBJECTS:bl1_scatter>
)

add_library(bl1_scatter OBJECT)

add_dependencies(bl1
        bl1_scatter
    )

target_sources(bl1_scatter
    PRIVATE
        ../Device/Source/gcc/corstone1000_bl1.ld
)

set_source_files_properties(../Device/Source/gcc/corstone1000_bl1.ld
    PROPERTIES
    LANGUAGE C
)

target_compile_options(bl1_scatter
    PRIVATE
        -E
        -P
        -xc
)

target_compile_definitions(bl1_scatter
    PRIVATE
        MCUBOOT_IMAGE_NUMBER=${BL1_IMAGE_NUMBER}
        BL1
)

target_include_directories(bl1_scatter
    PRIVATE
        ../partition
)

# Library to spearate build from bl2_mbedcrypto configurations
add_library(bl1_main STATIC)

target_compile_definitions(bl1_main
    PRIVATE
        MCUBOOT_IMAGE_NUMBER=${BL1_IMAGE_NUMBER}
        BL1
        BL2
        $<$<BOOL:${PLATFORM_IS_FVP}>:PLATFORM_IS_FVP>
)

# Configurations based on bl2/CMakeLists.txt

# Many files are reused form TF-M's bl2 directory
set(BL2_SOURCE ${CMAKE_SOURCE_DIR}/bl2)

target_sources(bl1_main
    PRIVATE
        ${BL2_SOURCE}/src/flash_map.c
        ./provisioning.c
)

target_include_directories(bl1_main
    PRIVATE
        $<BUILD_INTERFACE:${BL2_SOURCE}/include>
)

# Include path needed for mbedcrypto headers
target_include_directories(bl1_main
    PRIVATE
        $<BUILD_INTERFACE:${MBEDCRYPTO_PATH}/include>
)

# Configurations based on bl2/ext/mcuboot/CMakeLists.txt
target_link_libraries(bl1_main
    PRIVATE
        mcuboot_config
        bl2_mbedcrypto_config
)

target_include_directories(bl1_main
    PRIVATE
        $<BUILD_INTERFACE:${BL2_SOURCE}/ext/mcuboot/include>
)

target_sources(bl1_main
    PRIVATE
        ${BL2_SOURCE}/ext/mcuboot/bl2_main.c
        ${BL2_SOURCE}/ext/mcuboot/keys.c
        ${BL2_SOURCE}/ext/mcuboot/flash_map_legacy.c
)

# Configurations based on ${MCUBOOT_PATH}/boot/bootutil/CMakeLists.txt
# add_subdirectory("${MCUBOOT_PATH}/boot/bootutil" bootutil) cannot work as we want to define different hal
# functions compared to BL2
target_sources(bl1_main
    PRIVATE
        ${MCUBOOT_PATH}/boot/bootutil/src/loader.c
        ${MCUBOOT_PATH}/boot/bootutil/src/bootutil_misc.c
        ${MCUBOOT_PATH}/boot/bootutil/src/image_validate.c
        ${MCUBOOT_PATH}/boot/bootutil/src/image_rsa.c
        ${MCUBOOT_PATH}/boot/bootutil/src/tlv.c
        ${MCUBOOT_PATH}/boot/bootutil/src/boot_record.c
        ${MCUBOOT_PATH}/boot/bootutil/src/swap_scratch.c
        ${MCUBOOT_PATH}/boot/bootutil/src/swap_move.c
        ${MCUBOOT_PATH}/boot/bootutil/src/swap_misc.c
        ${MCUBOOT_PATH}/boot/bootutil/src/encrypted.c
        ${MCUBOOT_PATH}/boot/bootutil/src/fault_injection_hardening.c
        ${MCUBOOT_PATH}/boot/bootutil/src/fault_injection_hardening_delay_rng_mbedtls.c
)

target_include_directories(bl1_main
    PRIVATE
        $<BUILD_INTERFACE:${MCUBOOT_PATH}/boot/bootutil/include>
        $<BUILD_INTERFACE:${MCUBOOT_PATH}/boot/bootutil/src>
        $<BUILD_INTERFACE:${MCUBOOT_PATH}/boot>
)

# Configurations based on platform/CMakeLists.txt
target_include_directories(bl1_main
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/platform/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/platform/ext/cmsis>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/platform/ext/driver>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/platform/ext/common>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/platform/ext>
        $<$<BOOL:${CRYPTO_HW_ACCELERATOR}>:${CMAKE_SOURCE_DIR}/platform/ext/accelerator/interface>
)

target_sources(bl1_main
    PRIVATE
        $<$<BOOL:${PLATFORM_DEFAULT_UART_STDOUT}>:${CMAKE_SOURCE_DIR}/platform/ext/common/uart_stdout.c>
        $<$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>:${CMAKE_SOURCE_DIR}/platform/ext/common/template/nv_counters.c>
        $<$<OR:$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>,$<BOOL:${PLATFORM_DEFAULT_OTP}>>:${CMAKE_SOURCE_DIR}/platform/ext/common/template/flash_otp_nv_counters_backend.c>
        $<$<BOOL:${PLATFORM_DEFAULT_OTP}>:${CMAKE_SOURCE_DIR}/platform/ext/common/template/otp_flash.c>
)

target_link_libraries(bl1_main
    PRIVATE
        bl2_hal
        tfm_arch
)

target_compile_definitions(bl1_main
    PRIVATE
        MCUBOOT_${MCUBOOT_UPGRADE_STRATEGY}
        $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:SYMMETRIC_INITIAL_ATTESTATION>
        $<$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>:PLATFORM_DEFAULT_NV_COUNTERS>
        $<$<BOOL:${MCUBOOT_HW_KEY}>:MCUBOOT_HW_KEY>
        MCUBOOT_FIH_PROFILE_${MCUBOOT_FIH_PROFILE}
        $<$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>:PLATFORM_DEFAULT_NV_COUNTERS>
        $<$<BOOL:${PLATFORM_DEFAULT_OTP}>:PLATFORM_DEFAULT_OTP>
        $<$<BOOL:${OTP_NV_COUNTERS_RAM_EMULATION}>:OTP_NV_COUNTERS_RAM_EMULATION>
        $<$<BOOL:${TFM_DUMMY_PROVISIONING}>:TFM_DUMMY_PROVISIONING>
        $<$<BOOL:${PLATFORM_DEFAULT_OTP_WRITEABLE}>:OTP_WRITEABLE>
)

# Configurations based on cc312 cmake files
target_compile_definitions(bl1_main
    PRIVATE
        $<$<BOOL:${CRYPTO_HW_ACCELERATOR_OTP_STATE}>:CRYPTO_HW_ACCELERATOR_OTP_${CRYPTO_HW_ACCELERATOR_OTP_STATE}>
        $<$<BOOL:${CRYPTO_HW_ACCELERATOR}>:CRYPTO_HW_ACCELERATOR>
)

target_include_directories(bl1_main
    PRIVATE
        $<$<BOOL:${CRYPTO_HW_ACCELERATOR}>:${CMAKE_SOURCE_DIR}/platform/ext/accelerator/cc312>
        $<$<BOOL:${CRYPTO_HW_ACCELERATOR}>:${CMAKE_SOURCE_DIR}/lib/ext/cryptocell-312-runtime/shared/include/mbedtls>
        $<$<BOOL:${CRYPTO_HW_ACCELERATOR}>:${CMAKE_SOURCE_DIR}/lib/ext/cryptocell-312-runtime/shared/include/crypto_api/cc3x>
)

# Configurations based on platform level cmake files
target_sources(bl1_main
    PRIVATE
        ../CMSIS_Driver/Driver_Flash.c
        ../CMSIS_Driver/Driver_USART.c
        ../Device/Source/device_definition.c
        ../Device/Source/system_core_init.c
        ../Native_Driver/firewall.c
        ../Native_Driver/uart_pl011_drv.c
        ../fw_update_agent/fwu_agent.c
        ../Native_Driver/arm_watchdog_drv.c
        ../Native_Driver/watchdog.c
        bl1_boot_hal.c
        bl1_flash_map.c
        bl1_security_cnt.c
        flash_map_extended.c
        bl1_rotpk.c
)

if (PLATFORM_IS_FVP)
target_sources(bl1_main
    PRIVATE
        ../Native_Driver/spi_strataflashj3_flash_lib.c
        ../Native_Driver/cfi_drv.c
)
else()
target_sources(bl1_main
    PRIVATE
        ../Native_Driver/xilinx_pg153_axi_qspi_controller_drv.c
        ../Native_Driver/spi_n25q256a_flash_lib.c
        ../Native_Driver/spi_sst26vf064b_flash_lib.c
)
endif()

target_include_directories(bl1_main
    PRIVATE
        ../partition
        ../Device/Include
        ../.
        ../CMSIS_Driver/Config
        ../Device/Config
        ../Native_Driver
        ../fw_update_agent
)

############################### SIGNING BL2 image ##################################

find_package(Python3)

set(FLASH_AREA_NUM 8)
configure_file(signing_layout.c.in ${CMAKE_CURRENT_BINARY_DIR}/signing_layout.c @ONLY)
add_library(signing_layout_for_bl2 OBJECT ${CMAKE_CURRENT_BINARY_DIR}/signing_layout.c)

target_compile_options(signing_layout_for_bl2
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-E\;-xc>
        $<$<C_COMPILER_ID:ARMClang>:-E\;-xc>
        $<$<C_COMPILER_ID:IAR>:--preprocess=ns\;$<TARGET_OBJECTS:signing_layout_s>>
)
target_compile_definitions(signing_layout_for_bl2
    PRIVATE
        MCUBOOT_IMAGE_NUMBER=${BL1_IMAGE_NUMBER}
        BL1
        $<$<BOOL:${TEST_S}>:TEST_S>
)

target_include_directories(signing_layout_for_bl2
    PRIVATE
        ../partition
)

add_custom_target(bl2_signed_bin
    ALL
    SOURCES bl2_signed.bin
)
add_custom_command(OUTPUT bl2_signed.bin
    DEPENDS $<TARGET_FILE_DIR:bl2>/bl2.bin
    DEPENDS bl2_bin signing_layout_for_bl2
    WORKING_DIRECTORY ${MCUBOOT_PATH}/scripts

    #Sign secure binary image with provided secret key
    COMMAND ${Python3_EXECUTABLE} ${BL2_SOURCE}/ext/mcuboot/scripts/wrapper/wrapper.py
        -v ${BL2_IMAGE_VERSION}
        --layout $<TARGET_OBJECTS:signing_layout_for_bl2>
        -k ${MCUBOOT_KEY_S}
        --public-key-format $<IF:$<BOOL:${MCUBOOT_HW_KEY}>,full,hash>
        --align 1
        --pad
        --pad-header
        -H 0x400
        -s ${MCUBOOT_SECURITY_COUNTER_S}
        -d \"\(0,${MCUBOOT_S_IMAGE_MIN_VER}\)\"
        $<$<STREQUAL:${MCUBOOT_UPGRADE_STRATEGY},OVERWRITE_ONLY>:--overwrite-only>
        $<$<BOOL:${MCUBOOT_ENC_IMAGES}>:-E${MCUBOOT_KEY_ENC}>
        $<TARGET_FILE_DIR:bl2>/bl2.bin
        ${CMAKE_CURRENT_BINARY_DIR}/bl2_signed.bin
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/bl2_signed.bin $<TARGET_FILE_DIR:bl2>
)
