/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __ATTEST_KEY_H__
#define __ATTEST_KEY_H__

#include "attest.h"
#include "config_attest.h"
#include "psa/initial_attestation.h"
#include "psa/crypto.h"
#include "q_useful_buf.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Get the buffer of Instance ID data
 *
 * \param[out] id_buf  Address and length of Instance ID buffer
 *
 * \retval  PSA_ATTEST_ERR_SUCCESS            Instance ID was successfully
 *                                            returned.
 * \retval  PSA_ATTEST_ERR_CLAIM_UNAVAILABLE  Instance ID is unavailable
 * \retval  PSA_ATTEST_ERR_GENERAL            Instance ID could not be returned.
 */
enum psa_attest_err_t
attest_get_instance_id(struct q_useful_buf_c *id_buf);

#if ATTEST_INCLUDE_COSE_KEY_ID
/**
 * \brief Get the attestation key ID.
 *        In asymmetric key algorithm based Initial Attestation, it is the hash
 *        (SHA256) of the COSE_Key encoded attestation public key.
 *        In symmetric key algorithm based Initial Attestation, the key ID raw
 *        data is fetched from from device.
 *
 * \param[out] attest_key_id  Pointer and length of the key id.
 *
 * \retval  PSA_ATTEST_ERR_SUCCESS   Got key id successfully.
 * \retval  PSA_ATTEST_ERR_GENERAL   Failed to get key id.

 */
enum psa_attest_err_t
attest_get_initial_attestation_key_id(struct q_useful_buf_c *attest_key_id);
#else /* ATTEST_INCLUDE_COSE_KEY_ID */
static inline enum psa_attest_err_t
attest_get_initial_attestation_key_id(struct q_useful_buf_c *attest_key_id)
{
    (void)attest_key_id;

    return PSA_ATTEST_ERR_SUCCESS;
}
#endif /* ATTEST_INCLUDE_COSE_KEY_ID */

#ifdef __cplusplus
}
#endif

#endif /* __ATTEST_KEY_H__ */
