#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(TFM_TOOLCHAIN_FILE                  ${CMAKE_SOURCE_DIR}/toolchain_GNUARM.cmake CACHE FILEPATH    "Path to TFM compiler toolchain file")
set(TFM_PLATFORM                        ""          CACHE STRING    "Platform to build TF-M for. Must be either a relative path from [TF-M]/platform/ext/target, or an absolute path.")
set(CROSS_COMPILE                       arm-none-eabi CACHE STRING  "Cross-compilation triplet")

set(BL1                                 OFF         CACHE BOOL      "Whether to build BL1")
set(BL2                                 ON          CACHE BOOL      "Whether to build BL2")
set(NS                                  ON          CACHE BOOL      "Whether to build NS app")

set(TEST_S                              OFF         CACHE BOOL      "Whether to build S regression tests")
set(TEST_NS                             OFF         CACHE BOOL      "Whether to build NS regression tests")
set(TEST_PSA_API                        ""          CACHE STRING    "Which (if any) of the PSA API tests should be compiled")
set(TEST_BL1_1                          OFF         CACHE BOOL      "Whether to build BL1_1 tests")
set(TEST_BL1_2                          OFF         CACHE BOOL      "Whether to build BL1_2 tests")

set(PLATFORM_HAS_ISOLATION_L3_SUPPORT   OFF         CACHE BOOL      "Platform supports Isolation level 3")
set(TFM_ISOLATION_LEVEL                 1           CACHE STRING    "Isolation level")
set(PSA_FRAMEWORK_HAS_MM_IOVEC          OFF         CACHE BOOL      "Enable MM-IOVEC")
set(TFM_PROFILE                         ""          CACHE STRING    "Profile to use")
set(TFM_FIH_PROFILE                     OFF         CACHE STRING    "Fault injection hardening profile [OFF, LOW, MEDIUM, HIGH]")
set(CONFIG_TFM_SPM_BACKEND              "SFN"       CACHE STRING    "The SPM backend [IPC, SFN]")

# An NSPE client_id is provided by the NSPE OS via the SPM or directly by the SPM.
# When `TFM_NS_MANAGE_NSID` is `ON`, TF-M supports NSPE OS providing NSPE client_id.
set(TFM_NS_MANAGE_NSID                  OFF         CACHE BOOL      "Support NSPE OS providing NSPE client_id")

set(TFM_EXTRA_CONFIG_PATH               ""          CACHE PATH      "Path to extra cmake config file")

set(TFM_MANIFEST_LIST                   ${CMAKE_SOURCE_DIR}/tools/tfm_manifest_list.yaml CACHE FILEPATH "TF-M native Secure Partition manifests list file")
set(TFM_EXTRA_MANIFEST_LIST_FILES       ""          CACHE FILEPATH  "Extra manifest list file(s), used to list extra Secure Partition manifests.")
set(TFM_EXTRA_GENERATED_FILE_LIST_PATH  ""          CACHE PATH      "Path to extra generated file list. Appended to stardard TFM generated file list.")
set(TFM_EXTRA_PARTITION_PATHS           ""          CACHE PATH      "List of extra Secure Partitions directories. An extra Secure Parition folder contains source code, CMakeLists.txt and manifest files")

set(TFM_SPM_LOG_LEVEL                   TFM_SPM_LOG_LEVEL_SILENCE       CACHE STRING    "Set default SPM log level as INFO level")
set(TFM_PARTITION_LOG_LEVEL             TFM_PARTITION_LOG_LEVEL_SILENCE   CACHE STRING    "Set default Secure Partition log level as INFO level")

set(TFM_CODE_SHARING                    OFF         CACHE PATH      "Enable code sharing between MCUboot and secure firmware")
set(CONFIG_TFM_BOOT_STORE_MEASUREMENTS  ON          CACHE BOOL      "Store measurement values from all the boot stages. Used for initial attestation token.")
set(CONFIG_TFM_BOOT_STORE_ENCODED_MEASUREMENTS  ON  CACHE BOOL      "Enable storing of encoded measurements in boot.")

set(TFM_INSTALL_PATH                    ${CMAKE_BINARY_DIR}/install CACHE PATH "Path to which to install TF-M files")

set(TFM_DEBUG_SYMBOLS                   ON          CACHE BOOL      "Add debug symbols. Note that setting CMAKE_BUILD_TYPE to Debug or RelWithDebInfo will also add debug symbols.")
set(TFM_CODE_COVERAGE                   OFF         CACHE BOOL      "Whether to build the binary for lcov tools")

set(TFM_PXN_ENABLE                      OFF         CACHE BOOL      "Use Privileged execute never (PXN)")

set(TFM_EXCEPTION_INFO_DUMP             OFF         CACHE BOOL      "On fatal errors in the secure firmware, capture info about the exception. Print the info if the SPM log level is sufficient.")

set(CONFIG_TFM_HALT_ON_CORE_PANIC       OFF         CACHE BOOL       "On fatal errors in the secure firmware, halt instead of rebooting.")

set(CONFIG_TFM_STACK_WATERMARKS         OFF         CACHE BOOL      "Whether to pre-fill partition stacks with a set value to help determine stack usage")

set(PROJECT_CONFIG_HEADER_FILE          "${CMAKE_SOURCE_DIR}/config/config_base.h" CACHE FILEPATH "User defined header file for TF-M config")

############################ Platform ##########################################

set(NUM_MAILBOX_QUEUE_SLOT              1           CACHE BOOL      "Number of mailbox queue slots")
set(TFM_PLAT_SPECIFIC_MULTI_CORE_COMM   OFF         CACHE BOOL      "Whether to use a platform specific inter-core communication instead of mailbox in dual-cpu topology")

set(DEBUG_AUTHENTICATION                CHIP_DEFAULT CACHE STRING   "Debug authentication setting. [CHIP_DEFAULT, NONE, NS_ONLY, FULL")
set(SECURE_UART1                        OFF         CACHE BOOL      "Enable secure UART1")

set(CRYPTO_HW_ACCELERATOR               OFF         CACHE BOOL      "Whether to enable the crypto hardware accelerator on supported platforms")

set(OTP_NV_COUNTERS_RAM_EMULATION       OFF         CACHE BOOL      "Enable OTP/NV_COUNTERS emulation in RAM. Has no effect on non-default implementations of the OTP and NV_COUNTERS")
set(TFM_NS_NV_COUNTER_AMOUNT            0           CACHE STRING    "How many NS NV counters are enabled")

set(PLATFORM_DEFAULT_BL1                ON          CACHE STRING    "Whether to use default BL1 or platform-specific one")

set(PLATFORM_DEFAULT_ATTEST_HAL         ON          CACHE BOOL      "Use default attest hal implementation.")
set(PLATFORM_DEFAULT_NV_COUNTERS        ON          CACHE BOOL      "Use default nv counter implementation.")
set(PLATFORM_DEFAULT_CRYPTO_KEYS        ON          CACHE BOOL      "Use default crypto keys implementation.")
set(PLATFORM_DEFAULT_ROTPK              ON          CACHE BOOL      "Use default root of trust public key.")
set(PLATFORM_DEFAULT_IAK                ON          CACHE BOOL      "Use default initial attestation_key.")
set(PLATFORM_DEFAULT_UART_STDOUT        ON          CACHE BOOL      "Use default uart stdout implementation.")
set(PLATFORM_DEFAULT_NV_SEED            ON          CACHE BOOL      "Use default NV seed implementation.")
set(PLATFORM_DEFAULT_OTP                ON          CACHE BOOL      "Use trusted on-chip flash to implement OTP memory")
set(PLATFORM_DEFAULT_OTP_WRITEABLE      ON          CACHE BOOL      "Use OTP memory with write support")
set(PLATFORM_DEFAULT_PROVISIONING       ON          CACHE BOOL      "Use default provisioning implementation")
set(PLATFORM_DEFAULT_SYSTEM_RESET_HALT  ON          CACHE BOOL      "Use default system reset/halt implementation")

set(TFM_DUMMY_PROVISIONING              ON          CACHE BOOL      "Provision with dummy values. NOT to be used in production")
set(PLATFORM_IS_FVP                     FALSE       CACHE BOOL      "Whether to enable FVP or FPGA build of the platform.")

set(PLATFORM_PSA_ADAC_SECURE_DEBUG      FALSE       CACHE BOOL      "Whether to use psa-adac secure debug.")
set(PLATFORM_PSA_ADAC_SOURCE_PATH       "DOWNLOAD"  CACHE PATH      "Path to source dir of psa-adac.")
set(PLATFORM_PSA_ADAC_VERSION           "427923cc0152578d536fb2065154d5d0dd874910" CACHE STRING "The version of psa-adac to use.")

set(BL1_HEADER_SIZE                     0x000       CACHE STRING    "BL1 Header size")
set(BL1_TRAILER_SIZE                    0x000       CACHE STRING    "BL1 Trailer size")

set(BL2_HEADER_SIZE                     0x000       CACHE STRING    "BL2 Header size")
set(BL2_TRAILER_SIZE                    0x000       CACHE STRING    "BL2 Trailer size")

############################ Partitions ########################################
set(TFM_PARTITION_PROTECTED_STORAGE     OFF         CACHE BOOL      "Enable Protected Storage partition")
set(PS_ENCRYPTION                       ON          CACHE BOOL      "Enable encryption for Protected Storage partition")
set(PS_CRYPTO_AEAD_ALG                  PSA_ALG_GCM CACHE STRING    "The AEAD algorithm to use for authenticated encryption in Protected Storage")

set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE OFF      CACHE BOOL      "Enable Internal Trusted Storage partition")

set(TFM_PARTITION_CRYPTO                OFF         CACHE BOOL      "Enable Crypto partition")
set(CRYPTO_TFM_BUILTIN_KEYS_DRIVER      ON          CACHE BOOL      "Whether to allow crypto service to store builtin keys. Without this, ALL builtin keys must be stored in a platform-specific location")

set(TFM_PARTITION_INITIAL_ATTESTATION   OFF         CACHE BOOL      "Enable Initial Attestation partition")
set(SYMMETRIC_INITIAL_ATTESTATION       OFF         CACHE BOOL      "Use symmetric crypto for inital attestation")
set(ATTEST_INCLUDE_TEST_CODE            OFF         CACHE BOOL      "Include minimal development tests in the initial attestation regression test suite")
set(ATTEST_KEY_BITS                     256         CACHE STRING    "The size of the initial attestation key in bits")

set(TFM_PARTITION_PLATFORM              OFF         CACHE BOOL      "Enable Platform partition")

################################## Dependencies ################################

set(MBEDCRYPTO_PATH                     "DOWNLOAD"  CACHE PATH      "Path to Mbed Crypto (or DOWNLOAD to fetch automatically")
set(MBEDCRYPTO_VERSION                  "mbedtls-3.2.1" CACHE STRING "The version of Mbed Crypto to use")
set(MBEDCRYPTO_GIT_REMOTE               "https://github.com/Mbed-TLS/mbedtls.git" CACHE STRING "The URL (or path) to retrieve MbedTLS from.")
set(MBEDCRYPTO_BUILD_TYPE               "${CMAKE_BUILD_TYPE}" CACHE STRING "Build type of Mbed Crypto library")
set(TFM_MBEDCRYPTO_CONFIG_PATH
  "${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/mbedcrypto_config/tfm_mbedcrypto_config_default.h" CACHE PATH
  "Config to use for Mbed Crypto. For increased flexibility when pointing to a file, set the type \
of this setting to 'STRING' by passing the :<type> portion when specifying the setting value in \
the command line. E.g. '-DTFM_MBEDCRYPTO_CONFIG_PATH:STRING=some_file_which_is_generated.h' \
This can be useful if the config file is generated and placed inside a directory already added \
to the include path of mbedtls.")
set(TFM_MBEDCRYPTO_PSA_CRYPTO_CONFIG_PATH   "${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/mbedcrypto_config/crypto_config_default.h" CACHE PATH "Config to use psa crypto setting for Mbed Crypto.")
set(TFM_MBEDCRYPTO_PLATFORM_EXTRA_CONFIG_PATH ""    CACHE PATH      "Config to append to standard Mbed Crypto config, used by platforms to cnfigure feature support")

set(MCUBOOT_PATH                        "DOWNLOAD"  CACHE PATH      "Path to MCUboot (or DOWNLOAD to fetch automatically")
set(MCUBOOT_VERSION                     "v1.9.0"    CACHE STRING    "The version of MCUboot to use")

set(PSA_ARCH_TESTS_PATH                 "DOWNLOAD"  CACHE PATH      "Path to PSA arch tests (or DOWNLOAD to fetch automatically")
set(PSA_ARCH_TESTS_VERSION              "cf8bd71"   CACHE STRING    "The version of PSA arch tests to use")

set(NS_EVALUATION_APP_PATH              ""          CACHE PATH      "Path to TFM NS Evaluation Application")

################################################################################
################################################################################

# Specifying the accepted values for certain configuration options to facilitate
# their later validation.

########################## FIH #################################################

set_property(CACHE TFM_FIH_PROFILE PROPERTY STRINGS "OFF;LOW;MEDIUM;HIGH")
