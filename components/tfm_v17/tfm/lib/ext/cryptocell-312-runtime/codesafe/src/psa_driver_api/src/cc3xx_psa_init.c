/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_psa_mac.c
 *
 * This file contains the implementation of the entry points associated to the
 * mac capability (single-part and multipart) as described by the PSA
 * Cryptoprocessor Driver interface specification
 *
 */

#include "psa/crypto.h"
#include "cc_lib.h"

/* To be able to include the PSA style configuration */
#include "mbedtls/build_info.h"

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>

#define mbedtls_printf printf
#define mbedtls_calloc calloc
#define mbedtls_free free
#endif

#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"

static CCRndContext_t           *CC312_pRndCtx         = NULL;
static CCRndWorkBuff_t          *CC312_pRndWorkBuff    = NULL;
static mbedtls_ctr_drbg_context *CC312_pRndState       = NULL;
static mbedtls_entropy_context  *CC312_pMbedtlsEntropy = NULL;

/** \defgroup psa_init PSA driver entry points for driver initialisation
 *                     and de-initialisation operations
 *
 *  Entry points driver init and de-init as described by the PSA
 *  Cryptoprocessor Driver interface specification. These functions require
 *  an entropy module and DRBG module from mbed TLS. These contexts are
 *  allocated before calling into the runtime library initialisation. These
 *  are essentially copies of the same functions in the TF-M platform HAL that
 *  deal with crypto_hw initialisation and de-initialisation.
 *
 *  @{
 */
psa_status_t cc3xx_init(void)
{
    int ret = 0;

    /* Allocate memory on heap */
    CC312_pRndCtx         = mbedtls_calloc(1, sizeof(CCRndContext_t));
    CC312_pRndWorkBuff    = mbedtls_calloc(1, sizeof(CCRndWorkBuff_t));
    CC312_pRndState       = mbedtls_calloc(1, sizeof(mbedtls_ctr_drbg_context));
    CC312_pMbedtlsEntropy = mbedtls_calloc(1, sizeof(mbedtls_entropy_context));

    /* Check if memory allocation was successful */
    if ( !CC312_pRndCtx || !CC312_pRndWorkBuff
      || !CC312_pRndState || !CC312_pMbedtlsEntropy) {
        mbedtls_free(CC312_pRndCtx);
        mbedtls_free(CC312_pRndWorkBuff);
        mbedtls_free(CC312_pRndState);
        mbedtls_free(CC312_pMbedtlsEntropy);

        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }

    /* Init Rnd context's inner members */
    CC312_pRndCtx->rndState   = CC312_pRndState;
    CC312_pRndCtx->entropyCtx = CC312_pMbedtlsEntropy;

    /* Initialise CryptoCell library */
    ret = CC_LibInit(CC312_pRndCtx, CC312_pRndWorkBuff);
    if (ret != CC_LIB_RET_OK) {
        mbedtls_free(CC312_pRndCtx);
        mbedtls_free(CC312_pRndWorkBuff);
        mbedtls_free(CC312_pRndState);
        mbedtls_free(CC312_pMbedtlsEntropy);

        return PSA_ERROR_HARDWARE_FAILURE;
    }

    return PSA_SUCCESS;
}

psa_status_t cc3xx_free(void)
{
    int ret = 0;

    ret = CC_LibFini(CC312_pRndCtx);
    if(ret != CC_LIB_RET_OK) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    mbedtls_free(CC312_pRndCtx);
    mbedtls_free(CC312_pRndWorkBuff);
    mbedtls_free(CC312_pRndState);
    mbedtls_free(CC312_pMbedtlsEntropy);

    return PSA_SUCCESS;
}
/** @} */ // end of psa_init
