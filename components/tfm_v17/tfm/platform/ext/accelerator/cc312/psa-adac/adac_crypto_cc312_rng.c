/*
 * Copyright (c) 2021 Arm Limited
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "psa_adac_debug.h"

#include "psa/crypto.h"
#include "rnd_dma/local/cc_rnd_local.h"
#include "pal/cc_pal_mem.h"
#include "cc_rng_plat.h"
#include "rnd_dma/llf_rnd_trng.h"

int hardware_rng_poll(uint8_t *output, size_t len, size_t *olen)
{
    CCRndWorkBuff_t rndWorkBuff;
    CCRndState_t rndState;
    CCRndParams_t trngParams;
    int ret, error = -1;
    uint32_t *entrSource_ptr;

    if ((NULL == output) || (NULL == olen) || (0 == len)) {
        return -1;
    }

    CC_PalMemSetZero(&rndWorkBuff, sizeof(CCRndWorkBuff_t));
    CC_PalMemSetZero(&rndState, sizeof(CCRndState_t));
    CC_PalMemSetZero(&trngParams, sizeof(CCRndParams_t));

    ret = RNG_PLAT_SetUserRngParameters(&trngParams);
    if (ret != 0) {
        PSA_ADAC_LOG_ERR("cc312", "Error: RNG_PLAT_SetUserRngParameters() failed.\n");
        error = -1;
        goto Cleanup;
    }

    ret = LLF_RND_GetTrngSource(
            &rndState,                    /*in/out*/
            &trngParams,                  /*in/out*/
            0,                            /*in  -  isContinued - false*/
            (uint32_t *) &len,            /*in/out*/
            &entrSource_ptr,              /*out*/
            (uint32_t *) olen,            /*out*/
            (uint32_t *) &rndWorkBuff,    /*in*/
            0                             /*in - isFipsSupport false*/ );
    if (ret != 0) {
        PSA_ADAC_LOG_ERR("cc312", "Error: LLF_RND_GetTrngSource() failed.\n");
        error = -1;
        goto Cleanup;
    }

    CC_PalMemCopy (output, entrSource_ptr + CC_RND_TRNG_SRC_INNER_OFFSET_WORDS,
            ((*olen) <= len) ? (*olen) : len);

Cleanup:
    CC_PalMemSetZero(&rndWorkBuff, sizeof(CCRndWorkBuff_t));
    CC_PalMemSetZero(&rndState, sizeof(CCRndState_t));
    CC_PalMemSetZero(&trngParams, sizeof(CCRndParams_t));

End:
    return error;
}

psa_status_t psa_adac_generate_challenge(uint8_t *output, size_t output_size)
{
    size_t size = 0;
    psa_status_t status = PSA_SUCCESS;
    size_t part;
    while ((status == PSA_SUCCESS) && (size < output_size)) {
        part = 0;
        status = hardware_rng_poll(output + size, output_size - size, &part) == 0 ?
                 PSA_SUCCESS : PSA_ERROR_HARDWARE_FAILURE;
        if (status == PSA_SUCCESS) {
            size += part;
        }
    }
    return status;
}
