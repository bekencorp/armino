/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <stdbool.h>

#include "config_crypto.h"
#include "tfm_mbedcrypto_include.h"

#include "tfm_crypto_api.h"
#include "tfm_crypto_defs.h"
#include "tfm_sp_log.h"
#include "crypto_check_config.h"
#include "tfm_plat_crypto_keys.h"

/*
 * \brief This Mbed TLS include is needed to initialise the memory allocator
 *        of the library used for internal allocations
 */
#include "mbedtls/memory_buffer_alloc.h"

#include "mbedtls/platform.h"

#if CRYPTO_NV_SEED
#include "tfm_plat_crypto_nv_seed.h"
#endif /* CRYPTO_NV_SEED */

#ifdef CRYPTO_HW_ACCELERATOR
#include "crypto_hw.h"
#endif /* CRYPTO_HW_ACCELERATOR */

#ifndef MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER
#error "MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER must be selected in Mbed TLS config file"
#endif

#include <string.h>
#include "psa/framework_feature.h"
#include "psa/service.h"
#include "psa_manifest/tfm_crypto.h"

/**
 * \brief Aligns a value x up to an alignment a.
 */
#define ALIGN(x, a) (((x) + ((a) - 1)) & ~((a) - 1))

/**
 * \brief Maximum alignment required by any iovec parameters to the TF-M Crypto
 *        partition.
 */
#define TFM_CRYPTO_IOVEC_ALIGNMENT (4u)

#if PSA_FRAMEWORK_HAS_MM_IOVEC == 1
static int32_t g_client_id;

static void tfm_crypto_set_caller_id(int32_t id)
{
    g_client_id = id;
}

psa_status_t tfm_crypto_get_caller_id(int32_t *id)
{
    *id = g_client_id;
    return PSA_SUCCESS;
}

static psa_status_t tfm_crypto_init_iovecs(const psa_msg_t *msg,
                                           psa_invec in_vec[],
                                           size_t in_len,
                                           psa_outvec out_vec[],
                                           size_t out_len)
{
    uint32_t i;

    /* Map from the second element as the first is read when parsing */
    for (i = 1; i < in_len; i++) {
        in_vec[i].len = msg->in_size[i];
        if (in_vec[i].len != 0) {
            in_vec[i].base = psa_map_invec(msg->handle, i);
        } else {
            in_vec[i].base = NULL;
        }
    }

    for (i = 0; i < out_len; i++) {
        out_vec[i].len = msg->out_size[i];
        if (out_vec[i].len != 0) {
            out_vec[i].base = psa_map_outvec(msg->handle, i);
        } else {
            out_vec[i].base = NULL;
        }
    }

    return PSA_SUCCESS;
}
#else /* PSA_FRAMEWORK_HAS_MM_IOVEC == 1 */
/**
 * \brief Internal scratch used for IOVec allocations
 *
 */
static struct tfm_crypto_scratch {
    __attribute__((__aligned__(TFM_CRYPTO_IOVEC_ALIGNMENT)))
    uint8_t buf[CRYPTO_IOVEC_BUFFER_SIZE];
    uint32_t alloc_index;
    int32_t owner;
} scratch = {.buf = {0}, .alloc_index = 0};

static psa_status_t tfm_crypto_set_scratch_owner(int32_t id)
{
    scratch.owner = id;
    return PSA_SUCCESS;
}

static psa_status_t tfm_crypto_get_scratch_owner(int32_t *id)
{
    *id = scratch.owner;
    return PSA_SUCCESS;
}

static psa_status_t tfm_crypto_alloc_scratch(size_t requested_size, void **buf)
{
    /* Ensure alloc_index remains aligned to the required iovec alignment */
    requested_size = ALIGN(requested_size, TFM_CRYPTO_IOVEC_ALIGNMENT);

    if (requested_size > (sizeof(scratch.buf) - scratch.alloc_index)) {
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }

    /* Compute the pointer to the allocated space */
    *buf = (void *)&scratch.buf[scratch.alloc_index];

    /* Increase the allocated size */
    scratch.alloc_index += requested_size;

    return PSA_SUCCESS;
}

static void tfm_crypto_clear_scratch(void)
{
    scratch.owner = 0;
    (void)memset(scratch.buf, 0, scratch.alloc_index);
    scratch.alloc_index = 0;
}

static void tfm_crypto_set_caller_id(int32_t id)
{
    /* Set the owner of the data in the scratch */
    (void)tfm_crypto_set_scratch_owner(id);
}

psa_status_t tfm_crypto_get_caller_id(int32_t *id)
{
    return tfm_crypto_get_scratch_owner(id);
}

static psa_status_t tfm_crypto_init_iovecs(const psa_msg_t *msg,
                                           psa_invec in_vec[],
                                           size_t in_len,
                                           psa_outvec out_vec[],
                                           size_t out_len)
{
    uint32_t i;
    void *alloc_buf_ptr = NULL;
    psa_status_t status;

    /* Alloc/read from the second element as the first is read when parsing */
    for (i = 1; i < in_len; i++) {
        /* Allocate necessary space in the internal scratch */
        status = tfm_crypto_alloc_scratch(msg->in_size[i], &alloc_buf_ptr);
        if (status != PSA_SUCCESS) {
            tfm_crypto_clear_scratch();
            return status;
        }
        /* Read from the IPC framework inputs into the scratch */
        in_vec[i].len =
                       psa_read(msg->handle, i, alloc_buf_ptr, msg->in_size[i]);
        /* Populate the fields of the input to the secure function */
        in_vec[i].base = alloc_buf_ptr;
    }

    for (i = 0; i < out_len; i++) {
        /* Allocate necessary space for the output in the internal scratch */
        status = tfm_crypto_alloc_scratch(msg->out_size[i], &alloc_buf_ptr);
        if (status != PSA_SUCCESS) {
            tfm_crypto_clear_scratch();
            return status;
        }
        /* Populate the fields of the output to the secure function */
        out_vec[i].base = alloc_buf_ptr;
        out_vec[i].len = msg->out_size[i];
    }

    return PSA_SUCCESS;
}
#endif /* PSA_FRAMEWORK_HAS_MM_IOVEC == 1 */

static psa_status_t tfm_crypto_call_srv(const psa_msg_t *msg)
{
    psa_status_t status = PSA_SUCCESS;
    size_t in_len = PSA_MAX_IOVEC, out_len = PSA_MAX_IOVEC, i;
    psa_invec in_vec[PSA_MAX_IOVEC] = { {NULL, 0} };
    psa_outvec out_vec[PSA_MAX_IOVEC] = { {NULL, 0} };
    struct tfm_crypto_pack_iovec iov = {0};

    /* Check the number of in_vec filled */
    while ((in_len > 0) && (msg->in_size[in_len - 1] == 0)) {
        in_len--;
    }

    /* Check the number of out_vec filled */
    while ((out_len > 0) && (msg->out_size[out_len - 1] == 0)) {
        out_len--;
    }

    /* There will always be a tfm_crypto_pack_iovec in the first iovec */
    if (in_len < 1) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    if (psa_read(msg->handle, 0, &iov, sizeof(iov)) != sizeof(iov)) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Initialise the first iovec with the IOV read when parsing */
    in_vec[0].base = &iov;
    in_vec[0].len = sizeof(struct tfm_crypto_pack_iovec);

    status = tfm_crypto_init_iovecs(msg, in_vec, in_len, out_vec, out_len);
    if (status != PSA_SUCCESS) {
        return status;
    }

    tfm_crypto_set_caller_id(msg->client_id);

    /* Call the dispatcher to the functions that implement the PSA Crypto API */
    status = tfm_crypto_api_dispatcher(in_vec, in_len, out_vec, out_len);

#if PSA_FRAMEWORK_HAS_MM_IOVEC == 1
    for (i = 0; i < out_len; i++) {
        if (out_vec[i].base != NULL) {
            psa_unmap_outvec(msg->handle, i, out_vec[i].len);
        }
    }
#else
    /* Write into the IPC framework outputs from the scratch */
    for (i = 0; i < out_len; i++) {
        psa_write(msg->handle, i, out_vec[i].base, out_vec[i].len);
    }

    /* Clear the allocated internal scratch before returning */
    tfm_crypto_clear_scratch();
#endif

    return status;
}

/**
 * \brief Static buffer to be used by Mbed Crypto for memory allocations
 *
 */
static uint8_t mbedtls_mem_buf[CRYPTO_ENGINE_BUF_SIZE] = {0};

static psa_status_t tfm_crypto_engine_init(void)
{
#if CRYPTO_NV_SEED
    LOG_INFFMT("[INF][Crypto] ");
    LOG_INFFMT("Provisioning entropy seed... ");
    if (tfm_plat_crypto_provision_entropy_seed() != TFM_CRYPTO_NV_SEED_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }
    LOG_INFFMT("\033[0;32mcomplete.\033[0m\r\n");
#endif /* CRYPTO_NV_SEED */

    /* Initialise the Mbed Crypto memory allocator to use static memory
     * allocation from the provided buffer instead of using the heap
     */
    mbedtls_memory_buffer_alloc_init(mbedtls_mem_buf,
                                     CRYPTO_ENGINE_BUF_SIZE);

    /* mbedtls_printf is used to print messages including error information. */
#if (TFM_PARTITION_LOG_LEVEL >= TFM_PARTITION_LOG_LEVEL_ERROR)
    mbedtls_platform_set_printf(printf);
#endif

    /* Initialise the crypto accelerator if one is enabled. If the driver API is
     * the one defined by the PSA Unified Driver interface, the initialisation is
     * performed directly through psa_crypto_init() while the PSA subsystem is
     * initialised
     */
#if defined(CRYPTO_HW_ACCELERATOR) && defined(CC312_LEGACY_DRIVER_API_ENABLED)
    LOG_INFFMT("[INF][Crypto] Initialising HW accelerator... ");
    if (crypto_hw_accelerator_init() != 0) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }
    LOG_INFFMT("\033[0;32mcomplete.\033[0m\r\n");
#endif /* CRYPTO_HW_ACCELERATOR */

    /* Perform the initialisation of the PSA subsystem in the Mbed Crypto
     * library. If a driver is built using the PSA Driver interface, the function
     * below will perform also the same operations as crypto_hw_accelerator_init()
     */
    return psa_crypto_init();
}

static psa_status_t tfm_crypto_module_init(void)
{
    /* Init the Alloc module */
    return tfm_crypto_init_alloc();
}

psa_status_t tfm_crypto_init(void)
{
    psa_status_t status;
    enum tfm_plat_err_t plat_err;

    /* Initialise other modules of the service */
    status = tfm_crypto_module_init();
    if (status != PSA_SUCCESS) {
        return status;
    }

    /* Initialise the engine layer */
    status =  tfm_crypto_engine_init();
    if (status != PSA_SUCCESS) {
        return status;
    }

    plat_err = tfm_plat_load_builtin_keys();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    return PSA_SUCCESS;
}

psa_status_t tfm_crypto_sfn(const psa_msg_t *msg)
{
    /* Process the message type */
    switch (msg->type) {
    case PSA_IPC_CALL:
        return tfm_crypto_call_srv(msg);
    default:
        return PSA_ERROR_NOT_SUPPORTED;
    }

    return PSA_ERROR_GENERIC_ERROR;
}

psa_status_t tfm_crypto_api_dispatcher(psa_invec in_vec[],
                                       size_t in_len,
                                       psa_outvec out_vec[],
                                       size_t out_len)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
    int32_t caller_id = 0;
    mbedtls_svc_key_id_t encoded_key = MBEDTLS_SVC_KEY_ID_INIT;
    bool is_key_required = false;
    enum tfm_crypto_group_id group_id;

    if (in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    group_id = TFM_CRYPTO_GET_GROUP_ID(iov->function_id);

    is_key_required = !((group_id == TFM_CRYPTO_GROUP_ID_HASH) ||
                        (group_id == TFM_CRYPTO_GROUP_ID_RANDOM));

    if (is_key_required) {
        status = tfm_crypto_get_caller_id(&caller_id);
        if (status != PSA_SUCCESS) {
            return status;
        }
        /* The caller_id being set in the owner field is the partition ID
         * of the calling partition
         */
        encoded_key = mbedtls_svc_key_id_make(caller_id, iov->key_id);
    }

    /* Dispatch to each sub-module based on the Group ID */
    switch (group_id) {
    case TFM_CRYPTO_GROUP_ID_KEY_MANAGEMENT:
        return tfm_crypto_key_management_interface(in_vec, out_vec,
                                                   &encoded_key);
    case TFM_CRYPTO_GROUP_ID_HASH:
        return tfm_crypto_hash_interface(in_vec, out_vec);
    case TFM_CRYPTO_GROUP_ID_MAC:
        return tfm_crypto_mac_interface(in_vec, out_vec, &encoded_key);
    case TFM_CRYPTO_GROUP_ID_CIPHER:
        return tfm_crypto_cipher_interface(in_vec, out_vec, &encoded_key);
    case TFM_CRYPTO_GROUP_ID_AEAD:
        return tfm_crypto_aead_interface(in_vec, out_vec, &encoded_key);
    case TFM_CRYPTO_GROUP_ID_ASYM_SIGN:
        return tfm_crypto_asymmetric_sign_interface(in_vec, out_vec,
                                                    &encoded_key);
    case TFM_CRYPTO_GROUP_ID_ASYM_ENCRYPT:
        return tfm_crypto_asymmetric_encrypt_interface(in_vec, out_vec,
                                                       &encoded_key);
    case TFM_CRYPTO_GROUP_ID_KEY_DERIVATION:
        return tfm_crypto_key_derivation_interface(in_vec, out_vec,
                                                   &encoded_key);
    case TFM_CRYPTO_GROUP_ID_RANDOM:
        return tfm_crypto_random_interface(in_vec, out_vec);
    default:
        LOG_ERRFMT("[ERR][Crypto] Unsupported request!\r\n");
        return PSA_ERROR_NOT_SUPPORTED;
    }

    return PSA_ERROR_NOT_SUPPORTED;
}
