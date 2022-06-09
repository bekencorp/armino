/*
 * Copyright (c) 2017-2021 Arm Limited. All rights reserved.
 * Copyright 2020 NXP. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "tfm_plat_crypto_keys.h"
#include <stddef.h>


#ifndef TFM_ATTESTATION_KEY_ADDR
/*
 * This file contains the hard coded version of the ECDSA P-256 secret key in:
 * platform/ext/common/template/tfm_initial_attestation_key.pem
 *
 * As a P-256 key, the private key is 32 bytes long.
 *
 * This key is used to sign the initial attestation token.
 * The secret key is stored in raw format, without any encoding(ASN.1, COSE).
 *
 * #######  DO NOT USE THIS KEY IN PRODUCTION #######
 */

/* Initial attestation private key in raw format, without any encoding.
 * It belongs to the ECDSA P-256 curve.
 * It MUST be present on the device.
 */
TFM_LINK_SET_RO_IN_PARTITION_SECTION("TFM_SP_INITIAL_ATTESTATION", "PSA-ROT")
const uint8_t initial_attestation_private_key[] =
{
    0xA9, 0xB4, 0x54, 0xB2, 0x6D, 0x6F, 0x90, 0xA4,
    0xEA, 0x31, 0x19, 0x35, 0x64, 0xCB, 0xA9, 0x1F,
    0xEC, 0x6F, 0x9A, 0x00, 0x2A, 0x7D, 0xC0, 0x50,
    0x4B, 0x92, 0xA1, 0x93, 0x71, 0x34, 0x58, 0x5F
};
#define TFM_ATTESTATION_KEY_ADDR initial_attestation_private_key

#endif /* TFM_ATTESTATION_KEY_ADDR */

#ifndef TFM_ATTESTATION_KEY_SIZE
#define TFM_ATTESTATION_KEY_SIZE (32)
#endif

/* Type of the EC curve which the key belongs to, in PSA curve ID form */
#ifndef TFM_ATTESTATION_KEY_TYPE
#define TFM_ATTESTATION_KEY_TYPE PSA_ECC_FAMILY_SECP_R1
#endif

/**
 * \brief Copy the key to the destination buffer
 *
 * \param[out]  p_dst  Pointer to buffer where to store the key
 * \param[in]   p_src  Pointer to the key
 * \param[in]   size   Length of the key
 */
static inline void copy_key(uint8_t *p_dst, const uint8_t *p_src, size_t size)
{
    uint32_t i;

    for (i = size; i > 0; i--) {
        *p_dst = *p_src;
        p_src++;
        p_dst++;
    }
}

enum tfm_plat_err_t
tfm_plat_get_initial_attest_key(uint8_t          *key_buf,
                                uint32_t          size,
                                struct ecc_key_t *ecc_key,
                                psa_ecc_family_t  *curve_type)
{
    uint8_t *key_dst;
    const uint8_t *key_src;
    uint32_t key_size;
    uint32_t full_key_size = TFM_ATTESTATION_KEY_SIZE;

    if (size < full_key_size) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    /* Set the EC curve type which the key belongs to */
    *curve_type = TFM_ATTESTATION_KEY_TYPE;

    /* Copy the private key to the buffer, it MUST be present */
    key_dst  = key_buf;
    key_src  = (const uint8_t *)TFM_ATTESTATION_KEY_ADDR;
    key_size = TFM_ATTESTATION_KEY_SIZE;
    copy_key(key_dst, key_src, key_size);
    ecc_key->priv_key = key_dst;
    ecc_key->priv_key_size = key_size;

    ecc_key->pubx_key = NULL;
    ecc_key->pubx_key_size = 0;
    ecc_key->puby_key = NULL;
    ecc_key->puby_key_size = 0;

    return TFM_PLAT_ERR_SUCCESS;
}
