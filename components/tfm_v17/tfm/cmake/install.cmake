#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

install(DIRECTORY ${CMAKE_BINARY_DIR}/bin/
        DESTINATION ${TFM_INSTALL_PATH}/outputs
)

set(INTERFACE_INC_DIR ${CMAKE_SOURCE_DIR}/interface/include)
set(INTERFACE_SRC_DIR ${CMAKE_SOURCE_DIR}/interface/src)

set(INSTALL_INTERFACE_INC_DIR    ${TFM_INSTALL_PATH}/interface/include)
set(INSTALL_INTERFACE_SRC_DIR    ${TFM_INSTALL_PATH}/interface/src)
set(INSTALL_INTERFACE_LIB_DIR    ${TFM_INSTALL_PATH}/interface/lib)

# export veneer lib
if (CONFIG_TFM_USE_TRUSTZONE)
    install(FILES       ${CMAKE_BINARY_DIR}/secure_fw/s_veneers.o
            DESTINATION ${INSTALL_INTERFACE_LIB_DIR})
endif()

####################### export headers #########################################

install(FILES       ${INTERFACE_INC_DIR}/psa/client.h
                    ${INTERFACE_INC_DIR}/psa/error.h
        DESTINATION ${INSTALL_INTERFACE_INC_DIR}/psa)

install(FILES       ${CMAKE_BINARY_DIR}/generated/interface/include/psa_manifest/sid.h
        DESTINATION ${INSTALL_INTERFACE_INC_DIR}/psa_manifest)

install(FILES       ${INTERFACE_INC_DIR}/tfm_api.h
                    ${INTERFACE_INC_DIR}/tfm_ns_interface.h
                    ${INTERFACE_INC_DIR}/psa_config.h
        DESTINATION ${INSTALL_INTERFACE_INC_DIR})

install(FILES       ${INTERFACE_INC_DIR}/tfm_ns_client_ext.h
        DESTINATION ${INSTALL_INTERFACE_INC_DIR})

install(FILES       ${CMAKE_BINARY_DIR}/generated/interface/include/config_impl.h
        DESTINATION ${INSTALL_INTERFACE_INC_DIR})

install(FILES       ${INTERFACE_INC_DIR}/tfm_psa_call_pack.h
        DESTINATION ${INSTALL_INTERFACE_INC_DIR})
install(FILES       ${CMAKE_BINARY_DIR}/generated/interface/include/psa/framework_feature.h
        DESTINATION ${INSTALL_INTERFACE_INC_DIR}/psa)

if (TFM_PARTITION_NS_AGENT_MAILBOX)
    install(FILES       ${INTERFACE_INC_DIR}/multi_core/tfm_multi_core_api.h
                        ${INTERFACE_INC_DIR}/multi_core/tfm_ns_mailbox.h
                        ${INTERFACE_INC_DIR}/multi_core/tfm_mailbox.h
                        ${INTERFACE_INC_DIR}/multi_core/tfm_ns_mailbox_test.h
                        ${CMAKE_BINARY_DIR}/generated/interface/include/tfm_mailbox_config.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR})
endif()

if (TFM_PARTITION_PROTECTED_STORAGE)
    install(FILES       ${INTERFACE_INC_DIR}/psa/protected_storage.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR}/psa)
    install(FILES       ${INTERFACE_INC_DIR}/tfm_ps_defs.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR})
endif()

if (TFM_PARTITION_INTERNAL_TRUSTED_STORAGE)
    install(FILES       ${INTERFACE_INC_DIR}/psa/internal_trusted_storage.h
                        ${INTERFACE_INC_DIR}/psa/storage_common.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR}/psa)
    install(FILES       ${INTERFACE_INC_DIR}/tfm_its_defs.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR})
endif()

if (TFM_PARTITION_CRYPTO)
    install(FILES       ${INTERFACE_INC_DIR}/psa/crypto_extra.h
                        ${INTERFACE_INC_DIR}/psa/crypto_compat.h
                        ${INTERFACE_INC_DIR}/psa/crypto.h
                        ${INTERFACE_INC_DIR}/psa/crypto_client_struct.h
                        ${INTERFACE_INC_DIR}/psa/crypto_sizes.h
                        ${INTERFACE_INC_DIR}/psa/crypto_struct.h
                        ${INTERFACE_INC_DIR}/psa/crypto_types.h
                        ${INTERFACE_INC_DIR}/psa/crypto_values.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR}/psa)
    install(FILES       ${INTERFACE_INC_DIR}/tfm_crypto_defs.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR})
endif()

if (TFM_PARTITION_INITIAL_ATTESTATION)
    install(FILES       ${INTERFACE_INC_DIR}/psa/initial_attestation.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR}/psa)
    install(FILES       ${INTERFACE_INC_DIR}/tfm_attest_defs.h
                        ${INTERFACE_INC_DIR}/tfm_attest_iat_defs.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR})
endif()

if(TFM_PARTITION_PLATFORM)
    install(FILES       ${INTERFACE_INC_DIR}/tfm_platform_api.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR})
endif()

if(TFM_PARTITION_FIRMWARE_UPDATE)
    install(FILES       ${INTERFACE_INC_DIR}/psa/update.h
                        ${CMAKE_BINARY_DIR}/generated/interface/include/psa/fwu_config.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR}/psa)
    install(FILES       ${INTERFACE_INC_DIR}/tfm_fwu_defs.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR})
endif()

if(PLATFORM_DEFAULT_CRYPTO_KEYS)
    install(DIRECTORY   ${INTERFACE_INC_DIR}/crypto_keys
            DESTINATION ${INSTALL_INTERFACE_INC_DIR})
endif()

####################### export sources #########################################

if (TFM_PARTITION_NS_AGENT_MAILBOX)
    install(FILES       ${INTERFACE_SRC_DIR}/multi_core/tfm_ns_mailbox.c
                        ${INTERFACE_SRC_DIR}/multi_core/tfm_multi_core_ns_api.c
                        ${INTERFACE_SRC_DIR}/multi_core/tfm_multi_core_psa_ns_api.c
                        ${INTERFACE_SRC_DIR}/multi_core/tfm_ns_mailbox_thread.c
            DESTINATION ${INSTALL_INTERFACE_SRC_DIR}/multi_core)
endif()

if (TFM_PARTITION_NS_AGENT_TZ)
    install(FILES       ${INTERFACE_SRC_DIR}/tfm_psa_ns_api.c
            DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
    if (CONFIG_TFM_CONNECTION_BASED_SERVICE_API)
        install(FILES       ${INTERFACE_SRC_DIR}/tfm_psa_ns_connection_api.c
                DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
    endif()
endif()

if (CONFIG_TFM_USE_TRUSTZONE)
    install(FILES       ${INTERFACE_SRC_DIR}/tfm_ns_interface.c.example
            DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
endif()

if (TFM_PARTITION_PROTECTED_STORAGE)
    install(FILES       ${INTERFACE_SRC_DIR}/tfm_ps_api.c
            DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
endif()

if (TFM_PARTITION_INTERNAL_TRUSTED_STORAGE)
    install(FILES       ${INTERFACE_SRC_DIR}/tfm_its_api.c
            DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
endif()

if (TFM_PARTITION_CRYPTO)
    install(FILES       ${INTERFACE_SRC_DIR}/tfm_crypto_api.c
            DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
endif()

if (TFM_PARTITION_INITIAL_ATTESTATION)
    install(FILES       ${INTERFACE_SRC_DIR}/tfm_attest_api.c
            DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
endif()

if(TFM_PARTITION_PLATFORM)
    install(FILES       ${INTERFACE_SRC_DIR}/tfm_platform_api.c
            DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
endif()


##################### Export image signing information #########################

set(INSTALL_IMAGE_SIGNING_DIR ${TFM_INSTALL_PATH}/image_signing)

if(BL2)
    install(DIRECTORY bl2/ext/mcuboot/scripts
            DESTINATION ${INSTALL_IMAGE_SIGNING_DIR})

    if (MCUBOOT_ENC_IMAGES)
        install(FILES ${MCUBOOT_KEY_ENC}
                DESTINATION ${INSTALL_IMAGE_SIGNING_DIR}/keys)
    endif()

    install(FILES $<TARGET_OBJECTS:signing_layout_s>
            DESTINATION ${INSTALL_IMAGE_SIGNING_DIR}/layout_files)

    if(MCUBOOT_IMAGE_NUMBER GREATER 1)
        install(FILES $<TARGET_OBJECTS:signing_layout_ns>
                DESTINATION ${INSTALL_IMAGE_SIGNING_DIR}/layout_files)

        install(FILES ${MCUBOOT_KEY_NS}
                DESTINATION ${INSTALL_IMAGE_SIGNING_DIR}/keys)
    else()
        install(FILES ${MCUBOOT_KEY_S}
                DESTINATION ${INSTALL_IMAGE_SIGNING_DIR}/keys)
    endif()
endif()

if(TFM_PARTITION_FIRMWARE_UPDATE)
    install(FILES       ${INTERFACE_SRC_DIR}/tfm_fwu_api.c
            DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
endif()

##################### Platform-specific installation ###########################

if (EXISTS ${CMAKE_SOURCE_DIR}/platform/ext/target/${TFM_PLATFORM}/install.cmake)
    include(platform/ext/target/${TFM_PLATFORM}/install.cmake)
endif()
