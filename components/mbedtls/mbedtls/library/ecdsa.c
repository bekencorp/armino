/*
 *  Elliptic curve DSA
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

/*
 * References:
 *
 * SEC1 http://www.secg.org/index.php?action=secg,docs_secg
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_ECDSA_C)

#include "mbedtls/ecdsa.h"
#include "mbedtls/asn1write.h"

#include <string.h>

#if defined(MBEDTLS_ECDSA_DETERMINISTIC)
#include "mbedtls/hmac_drbg.h"
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdlib.h>
#define mbedtls_calloc    calloc
#define mbedtls_free       free
#endif

#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

/* Parameter validation macros based on platform_util.h */
#define ECDSA_VALIDATE_RET( cond )    \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_ECP_BAD_INPUT_DATA )
#define ECDSA_VALIDATE( cond )        \
    MBEDTLS_INTERNAL_VALIDATE( cond )

#if defined(MBEDTLS_ECP_RESTARTABLE)

/*
 * Sub-context for ecdsa_verify()
 */
struct mbedtls_ecdsa_restart_ver
{
    mbedtls_mpi u1, u2;     /* intermediate values  */
    enum {                  /* what to do next?     */
        ecdsa_ver_init = 0, /* getting started      */
        ecdsa_ver_muladd,   /* muladd step          */
    } state;
};

/*
 * Init verify restart sub-context
 */
static void ecdsa_restart_ver_init( mbedtls_ecdsa_restart_ver_ctx *ctx )
{
    mbedtls_mpi_init( &ctx->u1 );
    mbedtls_mpi_init( &ctx->u2 );
    ctx->state = ecdsa_ver_init;
}

/*
 * Free the components of a verify restart sub-context
 */
static void ecdsa_restart_ver_free( mbedtls_ecdsa_restart_ver_ctx *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_mpi_free( &ctx->u1 );
    mbedtls_mpi_free( &ctx->u2 );

    ecdsa_restart_ver_init( ctx );
}

/*
 * Sub-context for ecdsa_sign()
 */
struct mbedtls_ecdsa_restart_sig
{
    int sign_tries;
    int key_tries;
    mbedtls_mpi k;          /* per-signature random */
    mbedtls_mpi r;          /* r value              */
    enum {                  /* what to do next?     */
        ecdsa_sig_init = 0, /* getting started      */
        ecdsa_sig_mul,      /* doing ecp_mul()      */
        ecdsa_sig_modn,     /* mod N computations   */
    } state;
};

/*
 * Init verify sign sub-context
 */
static void ecdsa_restart_sig_init( mbedtls_ecdsa_restart_sig_ctx *ctx )
{
    ctx->sign_tries = 0;
    ctx->key_tries = 0;
    mbedtls_mpi_init( &ctx->k );
    mbedtls_mpi_init( &ctx->r );
    ctx->state = ecdsa_sig_init;
}

/*
 * Free the components of a sign restart sub-context
 */
static void ecdsa_restart_sig_free( mbedtls_ecdsa_restart_sig_ctx *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_mpi_free( &ctx->k );
    mbedtls_mpi_free( &ctx->r );
}

#if defined(MBEDTLS_ECDSA_DETERMINISTIC)
/*
 * Sub-context for ecdsa_sign_det()
 */
struct mbedtls_ecdsa_restart_det
{
    mbedtls_hmac_drbg_context rng_ctx;  /* DRBG state   */
    enum {                      /* what to do next?     */
        ecdsa_det_init = 0,     /* getting started      */
        ecdsa_det_sign,         /* make signature       */
    } state;
};

/*
 * Init verify sign_det sub-context
 */
static void ecdsa_restart_det_init( mbedtls_ecdsa_restart_det_ctx *ctx )
{
    mbedtls_hmac_drbg_init( &ctx->rng_ctx );
    ctx->state = ecdsa_det_init;
}

/*
 * Free the components of a sign_det restart sub-context
 */
static void ecdsa_restart_det_free( mbedtls_ecdsa_restart_det_ctx *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_hmac_drbg_free( &ctx->rng_ctx );

    ecdsa_restart_det_init( ctx );
}
#endif /* MBEDTLS_ECDSA_DETERMINISTIC */

#define ECDSA_RS_ECP    ( rs_ctx == NULL ? NULL : &rs_ctx->ecp )

/* Utility macro for checking and updating ops budget */
#define ECDSA_BUDGET( ops )   \
    MBEDTLS_MPI_CHK( mbedtls_ecp_check_budget( grp, ECDSA_RS_ECP, ops ) );

/* Call this when entering a function that needs its own sub-context */
#define ECDSA_RS_ENTER( SUB )   do {                                 \
    /* reset ops count for this call if top-level */                 \
    if( rs_ctx != NULL && rs_ctx->ecp.depth++ == 0 )                 \
        rs_ctx->ecp.ops_done = 0;                                    \
                                                                     \
    /* set up our own sub-context if needed */                       \
    if( mbedtls_ecp_restart_is_enabled() &&                          \
        rs_ctx != NULL && rs_ctx->SUB == NULL )                      \
    {                                                                \
        rs_ctx->SUB = mbedtls_calloc( 1, sizeof( *rs_ctx->SUB ) );   \
        if( rs_ctx->SUB == NULL )                                    \
            return( MBEDTLS_ERR_ECP_ALLOC_FAILED );                  \
                                                                     \
        ecdsa_restart_## SUB ##_init( rs_ctx->SUB );                 \
    }                                                                \
} while( 0 )

/* Call this when leaving a function that needs its own sub-context */
#define ECDSA_RS_LEAVE( SUB )   do {                                 \
    /* clear our sub-context when not in progress (done or error) */ \
    if( rs_ctx != NULL && rs_ctx->SUB != NULL &&                     \
        ret != MBEDTLS_ERR_ECP_IN_PROGRESS )                         \
    {                                                                \
        ecdsa_restart_## SUB ##_free( rs_ctx->SUB );                 \
        mbedtls_free( rs_ctx->SUB );                                 \
        rs_ctx->SUB = NULL;                                          \
    }                                                                \
                                                                     \
    if( rs_ctx != NULL )                                             \
        rs_ctx->ecp.depth--;                                         \
} while( 0 )

#else /* MBEDTLS_ECP_RESTARTABLE */

#define ECDSA_RS_ECP    NULL

#define ECDSA_BUDGET( ops )   /* no-op; for compatibility */

#define ECDSA_RS_ENTER( SUB )   (void) rs_ctx
#define ECDSA_RS_LEAVE( SUB )   (void) rs_ctx

#endif /* MBEDTLS_ECP_RESTARTABLE */

/*
 * Derive a suitable integer for group grp from a buffer of length len
 * SEC1 4.1.3 step 5 aka SEC1 4.1.4 step 3
 */
static int derive_mpi( const mbedtls_ecp_group *grp, mbedtls_mpi *x,
                       const unsigned char *buf, size_t blen )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t n_size = ( grp->nbits + 7 ) / 8;
    size_t use_size = blen > n_size ? n_size : blen;

    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( x, buf, use_size ) );
    if( use_size * 8 > grp->nbits )
        MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( x, use_size * 8 - grp->nbits ) );

    /* While at it, reduce modulo N */
    if( mbedtls_mpi_cmp_mpi( x, &grp->N ) >= 0 )
        MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mpi( x, x, &grp->N ) );

cleanup:
    return( ret );
}

#if !defined(MBEDTLS_ECDSA_SIGN_ALT)
/*
 * Compute ECDSA signature of a hashed message (SEC1 4.1.3)
 * Obviously, compared to SEC1 4.1.3, we skip step 4 (hash message)
 */
static int ecdsa_sign_restartable( mbedtls_ecp_group *grp,
                mbedtls_mpi *r, mbedtls_mpi *s,
                const mbedtls_mpi *d, const unsigned char *buf, size_t blen,
                int (*f_rng)(void *, unsigned char *, size_t), void *p_rng,
                int (*f_rng_blind)(void *, unsigned char *, size_t),
                void *p_rng_blind,
                mbedtls_ecdsa_restart_ctx *rs_ctx )
{
    int ret, key_tries, sign_tries;
    int *p_sign_tries = &sign_tries, *p_key_tries = &key_tries;
    mbedtls_ecp_point R;
    mbedtls_mpi k, e, t;
    mbedtls_mpi *pk = &k, *pr = r;

    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    if( ! mbedtls_ecdsa_can_do( grp->id ) || grp->N.p == NULL )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    /* Make sure d is in range 1..n-1 */
    if( mbedtls_mpi_cmp_int( d, 1 ) < 0 || mbedtls_mpi_cmp_mpi( d, &grp->N ) >= 0 )
        return( MBEDTLS_ERR_ECP_INVALID_KEY );

    mbedtls_ecp_point_init( &R );
    mbedtls_mpi_init( &k ); mbedtls_mpi_init( &e ); mbedtls_mpi_init( &t );

    ECDSA_RS_ENTER( sig );

#if defined(MBEDTLS_ECP_RESTARTABLE)
    if( rs_ctx != NULL && rs_ctx->sig != NULL )
    {
        /* redirect to our context */
        p_sign_tries = &rs_ctx->sig->sign_tries;
        p_key_tries = &rs_ctx->sig->key_tries;
        pk = &rs_ctx->sig->k;
        pr = &rs_ctx->sig->r;

        /* jump to current step */
        if( rs_ctx->sig->state == ecdsa_sig_mul )
            goto mul;
        if( rs_ctx->sig->state == ecdsa_sig_modn )
            goto modn;
    }
#endif /* MBEDTLS_ECP_RESTARTABLE */

    *p_sign_tries = 0;
    do
    {
        if( (*p_sign_tries)++ > 10 )
        {
            ret = MBEDTLS_ERR_ECP_RANDOM_FAILED;
            goto cleanup;
        }

        /*
         * Steps 1-3: generate a suitable ephemeral keypair
         * and set r = xR mod n
         */
        *p_key_tries = 0;
        do
        {
            if( (*p_key_tries)++ > 10 )
            {
                ret = MBEDTLS_ERR_ECP_RANDOM_FAILED;
                goto cleanup;
            }

            MBEDTLS_MPI_CHK( mbedtls_ecp_gen_privkey( grp, pk, f_rng, p_rng ) );

#if defined(MBEDTLS_ECP_RESTARTABLE)
            if( rs_ctx != NULL && rs_ctx->sig != NULL )
                rs_ctx->sig->state = ecdsa_sig_mul;

mul:
#endif
            MBEDTLS_MPI_CHK( mbedtls_ecp_mul_restartable( grp, &R, pk, &grp->G,
                                                          f_rng_blind,
                                                          p_rng_blind,
                                                          ECDSA_RS_ECP ) );
            MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( pr, &R.X, &grp->N ) );
        }
        while( mbedtls_mpi_cmp_int( pr, 0 ) == 0 );

#if defined(MBEDTLS_ECP_RESTARTABLE)
        if( rs_ctx != NULL && rs_ctx->sig != NULL )
            rs_ctx->sig->state = ecdsa_sig_modn;

modn:
#endif
        /*
         * Accounting for everything up to the end of the loop
         * (step 6, but checking now avoids saving e and t)
         */
        ECDSA_BUDGET( MBEDTLS_ECP_OPS_INV + 4 );

        /*
         * Step 5: derive MPI from hashed message
         */
        MBEDTLS_MPI_CHK( derive_mpi( grp, &e, buf, blen ) );

        /*
         * Generate a random value to blind inv_mod in next step,
         * avoiding a potential timing leak.
         */
        MBEDTLS_MPI_CHK( mbedtls_ecp_gen_privkey( grp, &t, f_rng_blind,
                                                  p_rng_blind ) );

        /*
         * Step 6: compute s = (e + r * d) / k = t (e + rd) / (kt) mod n
         */
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( s, pr, d ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( &e, &e, s ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &e, &e, &t ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( pk, pk, &t ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( pk, pk, &grp->N ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_inv_mod( s, pk, &grp->N ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( s, s, &e ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( s, s, &grp->N ) );
    }
    while( mbedtls_mpi_cmp_int( s, 0 ) == 0 );

#if defined(MBEDTLS_ECP_RESTARTABLE)
    if( rs_ctx != NULL && rs_ctx->sig != NULL )
        mbedtls_mpi_copy( r, pr );
#endif

cleanup:
    mbedtls_ecp_point_free( &R );
    mbedtls_mpi_free( &k ); mbedtls_mpi_free( &e ); mbedtls_mpi_free( &t );

    ECDSA_RS_LEAVE( sig );

    return( ret );
}

int mbedtls_ecdsa_can_do( mbedtls_ecp_group_id gid )
{
    switch( gid )
    {
#ifdef MBEDTLS_ECP_DP_CURVE25519_ENABLED
        case MBEDTLS_ECP_DP_CURVE25519: return 0;
#endif
#ifdef MBEDTLS_ECP_DP_CURVE448_ENABLED
        case MBEDTLS_ECP_DP_CURVE448: return 0;
#endif
    default: return 1;
    }
}

/*
 * Compute ECDSA signature of a hashed message
 */
int mbedtls_ecdsa_sign( mbedtls_ecp_group *grp, mbedtls_mpi *r, mbedtls_mpi *s,
                const mbedtls_mpi *d, const unsigned char *buf, size_t blen,
                int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    ECDSA_VALIDATE_RET( grp   != NULL );
    ECDSA_VALIDATE_RET( r     != NULL );
    ECDSA_VALIDATE_RET( s     != NULL );
    ECDSA_VALIDATE_RET( d     != NULL );
    ECDSA_VALIDATE_RET( f_rng != NULL );
    ECDSA_VALIDATE_RET( buf   != NULL || blen == 0 );

    /* Use the same RNG for both blinding and ephemeral key generation */
    return( ecdsa_sign_restartable( grp, r, s, d, buf, blen,
                                    f_rng, p_rng, f_rng, p_rng, NULL ) );
}
#endif /* !MBEDTLS_ECDSA_SIGN_ALT */

#if defined(MBEDTLS_ECDSA_DETERMINISTIC)
/*
 * Deterministic signature wrapper
 */
static int ecdsa_sign_det_restartable( mbedtls_ecp_group *grp,
                    mbedtls_mpi *r, mbedtls_mpi *s,
                    const mbedtls_mpi *d, const unsigned char *buf, size_t blen,
                    mbedtls_md_type_t md_alg,
                    int (*f_rng_blind)(void *, unsigned char *, size_t),
                    void *p_rng_blind,
                    mbedtls_ecdsa_restart_ctx *rs_ctx )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    mbedtls_hmac_drbg_context rng_ctx;
    mbedtls_hmac_drbg_context *p_rng = &rng_ctx;
    unsigned char data[2 * MBEDTLS_ECP_MAX_BYTES];
    size_t grp_len = ( grp->nbits + 7 ) / 8;
    const mbedtls_md_info_t *md_info;
    mbedtls_mpi h;

    if( ( md_info = mbedtls_md_info_from_type( md_alg ) ) == NULL )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    mbedtls_mpi_init( &h );
    mbedtls_hmac_drbg_init( &rng_ctx );

    ECDSA_RS_ENTER( det );

#if defined(MBEDTLS_ECP_RESTARTABLE)
    if( rs_ctx != NULL && rs_ctx->det != NULL )
    {
        /* redirect to our context */
        p_rng = &rs_ctx->det->rng_ctx;

        /* jump to current step */
        if( rs_ctx->det->state == ecdsa_det_sign )
            goto sign;
    }
#endif /* MBEDTLS_ECP_RESTARTABLE */

    /* Use private key and message hash (reduced) to initialize HMAC_DRBG */
    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( d, data, grp_len ) );
    MBEDTLS_MPI_CHK( derive_mpi( grp, &h, buf, blen ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( &h, data + grp_len, grp_len ) );
    mbedtls_hmac_drbg_seed_buf( p_rng, md_info, data, 2 * grp_len );

#if defined(MBEDTLS_ECP_RESTARTABLE)
    if( rs_ctx != NULL && rs_ctx->det != NULL )
        rs_ctx->det->state = ecdsa_det_sign;

sign:
#endif
#if defined(MBEDTLS_ECDSA_SIGN_ALT)
    ret = mbedtls_ecdsa_sign( grp, r, s, d, buf, blen,
                              mbedtls_hmac_drbg_random, p_rng );
#else
    if( f_rng_blind != NULL )
        ret = ecdsa_sign_restartable( grp, r, s, d, buf, blen,
                                      mbedtls_hmac_drbg_random, p_rng,
                                      f_rng_blind, p_rng_blind, rs_ctx );
    else
    {
        mbedtls_hmac_drbg_context *p_rng_blind_det;

#if !defined(MBEDTLS_ECP_RESTARTABLE)
        /*
         * To avoid reusing rng_ctx and risking incorrect behavior we seed a
         * second HMAC-DRBG with the same seed. We also apply a label to avoid
         * reusing the bits of the ephemeral key for blinding and eliminate the
         * risk that they leak this way.
         */
        const char* blind_label = "BLINDING CONTEXT";
        mbedtls_hmac_drbg_context rng_ctx_blind;

        mbedtls_hmac_drbg_init( &rng_ctx_blind );
        p_rng_blind_det = &rng_ctx_blind;
        mbedtls_hmac_drbg_seed_buf( p_rng_blind_det, md_info,
                                    data, 2 * grp_len );
        ret = mbedtls_hmac_drbg_update_ret( p_rng_blind_det,
                                            (const unsigned char*) blind_label,
                                            strlen( blind_label ) );
        if( ret != 0 )
        {
            mbedtls_hmac_drbg_free( &rng_ctx_blind );
            goto cleanup;
        }
#else
        /*
         * In the case of restartable computations we would either need to store
         * the second RNG in the restart context too or set it up at every
         * restart. The first option would penalize the correct application of
         * the function and the second would defeat the purpose of the
         * restartable feature.
         *
         * Therefore in this case we reuse the original RNG. This comes with the
         * price that the resulting signature might not be a valid deterministic
         * ECDSA signature with a very low probability (same magnitude as
         * successfully guessing the private key). However even then it is still
         * a valid ECDSA signature.
         */
        p_rng_blind_det = p_rng;
#endif /* MBEDTLS_ECP_RESTARTABLE */

        /*
         * Since the output of the RNGs is always the same for the same key and
         * message, this limits the efficiency of blinding and leaks information
         * through side channels. After mbedtls_ecdsa_sign_det() is removed NULL
         * won't be a valid value for f_rng_blind anymore. Therefore it should
         * be checked by the caller and this branch and check can be removed.
         */
        ret = ecdsa_sign_restartable( grp, r, s, d, buf, blen,
                                      mbedtls_hmac_drbg_random, p_rng,
                                      mbedtls_hmac_drbg_random, p_rng_blind_det,
                                      rs_ctx );

#if !defined(MBEDTLS_ECP_RESTARTABLE)
        mbedtls_hmac_drbg_free( &rng_ctx_blind );
#endif
    }
#endif /* MBEDTLS_ECDSA_SIGN_ALT */

cleanup:
    mbedtls_hmac_drbg_free( &rng_ctx );
    mbedtls_mpi_free( &h );

    ECDSA_RS_LEAVE( det );

    return( ret );
}

/*
 * Deterministic signature wrappers
 */

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
int mbedtls_ecdsa_sign_det( mbedtls_ecp_group *grp, mbedtls_mpi *r,
                            mbedtls_mpi *s, const mbedtls_mpi *d,
                            const unsigned char *buf, size_t blen,
                            mbedtls_md_type_t md_alg )
{
    ECDSA_VALIDATE_RET( grp   != NULL );
    ECDSA_VALIDATE_RET( r     != NULL );
    ECDSA_VALIDATE_RET( s     != NULL );
    ECDSA_VALIDATE_RET( d     != NULL );
    ECDSA_VALIDATE_RET( buf   != NULL || blen == 0 );

    return( ecdsa_sign_det_restartable( grp, r, s, d, buf, blen, md_alg,
                                        NULL, NULL, NULL ) );
}
#endif /* MBEDTLS_DEPRECATED_REMOVED */

int mbedtls_ecdsa_sign_det_ext( mbedtls_ecp_group *grp, mbedtls_mpi *r,
                                mbedtls_mpi *s, const mbedtls_mpi *d,
                                const unsigned char *buf, size_t blen,
                                mbedtls_md_type_t md_alg,
                                int (*f_rng_blind)(void *, unsigned char *,
                                                   size_t),
                                void *p_rng_blind )
{
    ECDSA_VALIDATE_RET( grp   != NULL );
    ECDSA_VALIDATE_RET( r     != NULL );
    ECDSA_VALIDATE_RET( s     != NULL );
    ECDSA_VALIDATE_RET( d     != NULL );
    ECDSA_VALIDATE_RET( buf   != NULL || blen == 0 );
    ECDSA_VALIDATE_RET( f_rng_blind != NULL );

    return( ecdsa_sign_det_restartable( grp, r, s, d, buf, blen, md_alg,
                                        f_rng_blind, p_rng_blind, NULL ) );
}
#endif /* MBEDTLS_ECDSA_DETERMINISTIC */

#if !defined(MBEDTLS_ECDSA_VERIFY_ALT)
/*
 * Verify ECDSA signature of hashed message (SEC1 4.1.4)
 * Obviously, compared to SEC1 4.1.3, we skip step 2 (hash message)
 */
static int ecdsa_verify_restartable( mbedtls_ecp_group *grp,
                                     const unsigned char *buf, size_t blen,
                                     const mbedtls_ecp_point *Q,
                                     const mbedtls_mpi *r, const mbedtls_mpi *s,
                                     mbedtls_ecdsa_restart_ctx *rs_ctx )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    mbedtls_mpi e, s_inv, u1, u2;
    mbedtls_ecp_point R;
    mbedtls_mpi *pu1 = &u1, *pu2 = &u2;

    mbedtls_ecp_point_init( &R );
    mbedtls_mpi_init( &e ); mbedtls_mpi_init( &s_inv );
    mbedtls_mpi_init( &u1 ); mbedtls_mpi_init( &u2 );

    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    if( ! mbedtls_ecdsa_can_do( grp->id ) || grp->N.p == NULL )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    ECDSA_RS_ENTER( ver );

#if defined(MBEDTLS_ECP_RESTARTABLE)
    if( rs_ctx != NULL && rs_ctx->ver != NULL )
    {
        /* redirect to our context */
        pu1 = &rs_ctx->ver->u1;
        pu2 = &rs_ctx->ver->u2;

        /* jump to current step */
        if( rs_ctx->ver->state == ecdsa_ver_muladd )
            goto muladd;
    }
#endif /* MBEDTLS_ECP_RESTARTABLE */

    /*
     * Step 1: make sure r and s are in range 1..n-1
     */
    if( mbedtls_mpi_cmp_int( r, 1 ) < 0 || mbedtls_mpi_cmp_mpi( r, &grp->N ) >= 0 ||
        mbedtls_mpi_cmp_int( s, 1 ) < 0 || mbedtls_mpi_cmp_mpi( s, &grp->N ) >= 0 )
    {
        ret = MBEDTLS_ERR_ECP_VERIFY_FAILED;
        goto cleanup;
    }

    /*
     * Step 3: derive MPI from hashed message
     */
    MBEDTLS_MPI_CHK( derive_mpi( grp, &e, buf, blen ) );

    /*
     * Step 4: u1 = e / s mod n, u2 = r / s mod n
     */
    ECDSA_BUDGET( MBEDTLS_ECP_OPS_CHK + MBEDTLS_ECP_OPS_INV + 2 );

    MBEDTLS_MPI_CHK( mbedtls_mpi_inv_mod( &s_inv, s, &grp->N ) );

    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( pu1, &e, &s_inv ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( pu1, pu1, &grp->N ) );

    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( pu2, r, &s_inv ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( pu2, pu2, &grp->N ) );

#if defined(MBEDTLS_ECP_RESTARTABLE)
    if( rs_ctx != NULL && rs_ctx->ver != NULL )
        rs_ctx->ver->state = ecdsa_ver_muladd;

muladd:
#endif
    /*
     * Step 5: R = u1 G + u2 Q
     */
    MBEDTLS_MPI_CHK( mbedtls_ecp_muladd_restartable( grp,
                     &R, pu1, &grp->G, pu2, Q, ECDSA_RS_ECP ) );

    if( mbedtls_ecp_is_zero( &R ) )
    {
        ret = MBEDTLS_ERR_ECP_VERIFY_FAILED;
        goto cleanup;
    }

    /*
     * Step 6: convert xR to an integer (no-op)
     * Step 7: reduce xR mod n (gives v)
     */
    MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &R.X, &R.X, &grp->N ) );

    /*
     * Step 8: check if v (that is, R.X) is equal to r
     */
    if( mbedtls_mpi_cmp_mpi( &R.X, r ) != 0 )
    {
        ret = MBEDTLS_ERR_ECP_VERIFY_FAILED;
        goto cleanup;
    }

cleanup:
    mbedtls_ecp_point_free( &R );
    mbedtls_mpi_free( &e ); mbedtls_mpi_free( &s_inv );
    mbedtls_mpi_free( &u1 ); mbedtls_mpi_free( &u2 );

    ECDSA_RS_LEAVE( ver );

    return( ret );
}

/*
 * Verify ECDSA signature of hashed message
 */
int mbedtls_ecdsa_verify( mbedtls_ecp_group *grp,
                          const unsigned char *buf, size_t blen,
                          const mbedtls_ecp_point *Q,
                          const mbedtls_mpi *r,
                          const mbedtls_mpi *s)
{
    ECDSA_VALIDATE_RET( grp != NULL );
    ECDSA_VALIDATE_RET( Q   != NULL );
    ECDSA_VALIDATE_RET( r   != NULL );
    ECDSA_VALIDATE_RET( s   != NULL );
    ECDSA_VALIDATE_RET( buf != NULL || blen == 0 );

    return( ecdsa_verify_restartable( grp, buf, blen, Q, r, s, NULL ) );
}
#endif /* !MBEDTLS_ECDSA_VERIFY_ALT */

/*
 * Convert a signature (given by context) to ASN.1
 */
static int ecdsa_signature_to_asn1( const mbedtls_mpi *r, const mbedtls_mpi *s,
                                    unsigned char *sig, size_t *slen )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    unsigned char buf[MBEDTLS_ECDSA_MAX_LEN];
    unsigned char *p = buf + sizeof( buf );
    size_t len = 0;

    MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_mpi( &p, buf, s ) );
    MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_mpi( &p, buf, r ) );

    MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_len( &p, buf, len ) );
    MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_tag( &p, buf,
                                       MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE ) );

    memcpy( sig, p, len );
    *slen = len;

    return( 0 );
}

/*
 * Compute and write signature
 */
int mbedtls_ecdsa_write_signature_restartable( mbedtls_ecdsa_context *ctx,
                           mbedtls_md_type_t md_alg,
                           const unsigned char *hash, size_t hlen,
                           unsigned char *sig, size_t *slen,
                           int (*f_rng)(void *, unsigned char *, size_t),
                           void *p_rng,
                           mbedtls_ecdsa_restart_ctx *rs_ctx )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    mbedtls_mpi r, s;
    ECDSA_VALIDATE_RET( ctx  != NULL );
    ECDSA_VALIDATE_RET( hash != NULL );
    ECDSA_VALIDATE_RET( sig  != NULL );
    ECDSA_VALIDATE_RET( slen != NULL );

    mbedtls_mpi_init( &r );
    mbedtls_mpi_init( &s );

#if defined(MBEDTLS_ECDSA_DETERMINISTIC)
    MBEDTLS_MPI_CHK( ecdsa_sign_det_restartable( &ctx->grp, &r, &s, &ctx->d,
                                                 hash, hlen, md_alg, f_rng,
                                                 p_rng, rs_ctx ) );
#else
    (void) md_alg;

#if defined(MBEDTLS_ECDSA_SIGN_ALT)
    MBEDTLS_MPI_CHK( mbedtls_ecdsa_sign( &ctx->grp, &r, &s, &ctx->d,
                         hash, hlen, f_rng, p_rng ) );
#else
    /* Use the same RNG for both blinding and ephemeral key generation */
    MBEDTLS_MPI_CHK( ecdsa_sign_restartable( &ctx->grp, &r, &s, &ctx->d,
                                             hash, hlen, f_rng, p_rng, f_rng,
                                             p_rng, rs_ctx ) );
#endif /* MBEDTLS_ECDSA_SIGN_ALT */
#endif /* MBEDTLS_ECDSA_DETERMINISTIC */

    MBEDTLS_MPI_CHK( ecdsa_signature_to_asn1( &r, &s, sig, slen ) );

cleanup:
    mbedtls_mpi_free( &r );
    mbedtls_mpi_free( &s );

    return( ret );
}

/*
 * Compute and write signature
 */
int mbedtls_ecdsa_write_signature( mbedtls_ecdsa_context *ctx,
                                 mbedtls_md_type_t md_alg,
                                 const unsigned char *hash, size_t hlen,
                                 unsigned char *sig, size_t *slen,
                                 int (*f_rng)(void *, unsigned char *, size_t),
                                 void *p_rng )
{
    ECDSA_VALIDATE_RET( ctx  != NULL );
    ECDSA_VALIDATE_RET( hash != NULL );
    ECDSA_VALIDATE_RET( sig  != NULL );
    ECDSA_VALIDATE_RET( slen != NULL );
    return( mbedtls_ecdsa_write_signature_restartable(
                ctx, md_alg, hash, hlen, sig, slen, f_rng, p_rng, NULL ) );
}

#if !defined(MBEDTLS_DEPRECATED_REMOVED) && \
    defined(MBEDTLS_ECDSA_DETERMINISTIC)
int mbedtls_ecdsa_write_signature_det( mbedtls_ecdsa_context *ctx,
                               const unsigned char *hash, size_t hlen,
                               unsigned char *sig, size_t *slen,
                               mbedtls_md_type_t md_alg )
{
    ECDSA_VALIDATE_RET( ctx  != NULL );
    ECDSA_VALIDATE_RET( hash != NULL );
    ECDSA_VALIDATE_RET( sig  != NULL );
    ECDSA_VALIDATE_RET( slen != NULL );
    return( mbedtls_ecdsa_write_signature( ctx, md_alg, hash, hlen, sig, slen,
                                   NULL, NULL ) );
}
#endif

/*
 * Read and check signature
 */
int mbedtls_ecdsa_read_signature( mbedtls_ecdsa_context *ctx,
                          const unsigned char *hash, size_t hlen,
                          const unsigned char *sig, size_t slen )
{
    ECDSA_VALIDATE_RET( ctx  != NULL );
    ECDSA_VALIDATE_RET( hash != NULL );
    ECDSA_VALIDATE_RET( sig  != NULL );
    return( mbedtls_ecdsa_read_signature_restartable(
                ctx, hash, hlen, sig, slen, NULL ) );
}

/*
 * Restartable read and check signature
 */
int mbedtls_ecdsa_read_signature_restartable( mbedtls_ecdsa_context *ctx,
                          const unsigned char *hash, size_t hlen,
                          const unsigned char *sig, size_t slen,
                          mbedtls_ecdsa_restart_ctx *rs_ctx )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    unsigned char *p = (unsigned char *) sig;
    const unsigned char *end = sig + slen;
    size_t len;
    mbedtls_mpi r, s;
    ECDSA_VALIDATE_RET( ctx  != NULL );
    ECDSA_VALIDATE_RET( hash != NULL );
    ECDSA_VALIDATE_RET( sig  != NULL );

    mbedtls_mpi_init( &r );
    mbedtls_mpi_init( &s );

    if( ( ret = mbedtls_asn1_get_tag( &p, end, &len,
                    MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE ) ) != 0 )
    {
        ret += MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        goto cleanup;
    }

    if( p + len != end )
    {
        ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA +
              MBEDTLS_ERR_ASN1_LENGTH_MISMATCH;
        goto cleanup;
    }

    if( ( ret = mbedtls_asn1_get_mpi( &p, end, &r ) ) != 0 ||
        ( ret = mbedtls_asn1_get_mpi( &p, end, &s ) ) != 0 )
    {
        ret += MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        goto cleanup;
    }
#if defined(MBEDTLS_ECDSA_VERIFY_ALT)
	//mbedtls_printf( " mbedtls_ecdsa_verify Q = %d\r\n", ctx->Q.X.n);
	//mbedtls_printf( " mbedtls_ecdsa_verify r = %d\r\n", r.n);
    if( ( ret = mbedtls_ecdsa_verify( &ctx->grp, hash, hlen,
                                      &ctx->Q, &r, &s ) ) != 0 )
        goto cleanup;
#else
    if( ( ret = ecdsa_verify_restartable( &ctx->grp, hash, hlen,
                              &ctx->Q, &r, &s, rs_ctx ) ) != 0 )
        goto cleanup;
#endif /* MBEDTLS_ECDSA_VERIFY_ALT */

    /* At this point we know that the buffer starts with a valid signature.
     * Return 0 if the buffer just contains the signature, and a specific
     * error code if the valid signature is followed by more data. */
    if( p != end )
        ret = MBEDTLS_ERR_ECP_SIG_LEN_MISMATCH;

cleanup:
    mbedtls_mpi_free( &r );
    mbedtls_mpi_free( &s );

    return( ret );
}

#if !defined(MBEDTLS_ECDSA_GENKEY_ALT)
/*
 * Generate key pair
 */
int mbedtls_ecdsa_genkey( mbedtls_ecdsa_context *ctx, mbedtls_ecp_group_id gid,
                  int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    int ret = 0;
    ECDSA_VALIDATE_RET( ctx   != NULL );
    ECDSA_VALIDATE_RET( f_rng != NULL );

    ret = mbedtls_ecp_group_load( &ctx->grp, gid );
    if( ret != 0 )
        return( ret );

   return( mbedtls_ecp_gen_keypair( &ctx->grp, &ctx->d,
                                    &ctx->Q, f_rng, p_rng ) );
}
#endif /* !MBEDTLS_ECDSA_GENKEY_ALT */

/*
 * Set context from an mbedtls_ecp_keypair
 */
int mbedtls_ecdsa_from_keypair( mbedtls_ecdsa_context *ctx, const mbedtls_ecp_keypair *key )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    ECDSA_VALIDATE_RET( ctx != NULL );
    ECDSA_VALIDATE_RET( key != NULL );

    if( ( ret = mbedtls_ecp_group_copy( &ctx->grp, &key->grp ) ) != 0 ||
        ( ret = mbedtls_mpi_copy( &ctx->d, &key->d ) ) != 0 ||
        ( ret = mbedtls_ecp_copy( &ctx->Q, &key->Q ) ) != 0 )
    {
        mbedtls_ecdsa_free( ctx );
    }

    return( ret );
}

/*
 * Initialize context
 */
void mbedtls_ecdsa_init( mbedtls_ecdsa_context *ctx )
{
    ECDSA_VALIDATE( ctx != NULL );

    mbedtls_ecp_keypair_init( ctx );
}

/*
 * Free context
 */
void mbedtls_ecdsa_free( mbedtls_ecdsa_context *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_ecp_keypair_free( ctx );
}

#if defined(MBEDTLS_ECP_RESTARTABLE)
/*
 * Initialize a restart context
 */
void mbedtls_ecdsa_restart_init( mbedtls_ecdsa_restart_ctx *ctx )
{
    ECDSA_VALIDATE( ctx != NULL );

    mbedtls_ecp_restart_init( &ctx->ecp );

    ctx->ver = NULL;
    ctx->sig = NULL;
#if defined(MBEDTLS_ECDSA_DETERMINISTIC)
    ctx->det = NULL;
#endif
}

/*
 * Free the components of a restart context
 */
void mbedtls_ecdsa_restart_free( mbedtls_ecdsa_restart_ctx *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_ecp_restart_free( &ctx->ecp );

    ecdsa_restart_ver_free( ctx->ver );
    mbedtls_free( ctx->ver );
    ctx->ver = NULL;

    ecdsa_restart_sig_free( ctx->sig );
    mbedtls_free( ctx->sig );
    ctx->sig = NULL;

#if defined(MBEDTLS_ECDSA_DETERMINISTIC)
    ecdsa_restart_det_free( ctx->det );
    mbedtls_free( ctx->det );
    ctx->det = NULL;
#endif
}
#endif /* MBEDTLS_ECP_RESTARTABLE */


#if defined(MBEDTLS_SELF_TEST)

static const uint8_t Vector_P384_Message[] =
{
    0xAB, 0xE1, 0x0A, 0xCE, 0x13, 0xE7, 0xE1, 0xD9, 0x18, 0x6C, 0x48, 0xF7, 0x88, 0x9D, 0x51, 0x47,
    0x3D, 0x3A, 0x09, 0x61, 0x98, 0x4B, 0xC8, 0x72, 0xDF, 0x70, 0x8E, 0xCC, 0x3E, 0xD3, 0xB8, 0x16,
    0x9D, 0x01, 0xE3, 0xD9, 0x6F, 0xC4, 0xF1, 0xD5, 0xEA, 0x00, 0xA0, 0x36, 0x92, 0xBC, 0xC5, 0xCF,
    0xFD, 0x53, 0x78, 0x7C, 0x88, 0xB9, 0x34, 0xAF, 0x40, 0x4C, 0x03, 0x9D, 0x32, 0x89, 0xB5, 0xBA,
    0xC5, 0xAE, 0x7D, 0xB1, 0x49, 0x68, 0x75, 0xB5, 0xDC, 0x73, 0xC3, 0x09, 0xF9, 0x25, 0xC1, 0x3D,
    0x1C, 0x01, 0xAB, 0xDA, 0xAF, 0xEB, 0xCD, 0xAC, 0x2C, 0xEE, 0x43, 0x39, 0x39, 0xCE, 0x8D, 0x4A,
    0x0A, 0x5D, 0x57, 0xBB, 0x70, 0x5F, 0x3B, 0xF6, 0xEC, 0x08, 0x47, 0x95, 0x11, 0xD4, 0xB4, 0xA3,
    0x21, 0x1F, 0x61, 0x64, 0x9A, 0xD6, 0x27, 0x43, 0x14, 0xBF, 0x0D, 0x43, 0x8A, 0x81, 0xE0, 0x60
};
static void dump_buf( const char *title, unsigned char *buf, size_t len )
{
	size_t i;

	mbedtls_printf( "%s", title );
	for( i = 0; i < len; i++ )
	{
		if(i%8 == 0)
			mbedtls_printf( "\r\n" );

		mbedtls_printf("%c%c", "0123456789ABCDEF" [buf[i] / 16],
                       "0123456789ABCDEF" [buf[i] % 16] );
	}
	mbedtls_printf( "\r\n" );
}

static void dump_pubkey( const char *title, mbedtls_ecdsa_context *key )
{
    unsigned char buf[300];
    size_t len;

    if( mbedtls_ecp_point_write_binary( &key->grp, &key->Q,
                MBEDTLS_ECP_PF_UNCOMPRESSED, &len, buf, sizeof buf ) != 0 )
    {
        mbedtls_printf("internal error\n");
        return;
    }

    dump_buf( title, buf, len );
}

extern u64 riscv_get_mtimer(void);
extern bk_err_t bk_securityip_trng_def_cfg(void);
extern bk_err_t bk_securityip_get_trng(uint32_t size, uint8_t * RandNum_p);
static int gen_rand( void *rng_state, unsigned char *output, size_t len )
{
	if( rng_state != NULL )
		rng_state  = NULL;

	bk_securityip_trng_def_cfg();
	bk_securityip_get_trng(len, output);

	return 0;
}

int mbedtls_ecdsa_self_test( int verbose ,uint32_t loop)
{
	int ret = 1;
	mbedtls_ecdsa_context ctx_sign, ctx_verify;
	unsigned char sig[MBEDTLS_ECDSA_MAX_LEN];
	size_t sig_len;
	uint32_t i;
	uint64_t before, after;

	mbedtls_ecdsa_init( &ctx_sign );
	mbedtls_ecdsa_init( &ctx_verify );

	memset( sig, 0, sizeof( sig ) );
	if(loop < 1)
		loop = 1;

	/*
	 * Generate a key pair for signing
	 */
	if( verbose != 0 )
		mbedtls_printf( " 1. Generating key pair...\r\n  ." );

	if( ( ret = mbedtls_ecdsa_genkey( &ctx_sign, MBEDTLS_ECP_DP_SECP256R1,gen_rand, NULL ) ) != 0 )
	{
		mbedtls_printf( " failed\n  ! mbedtls_ecdsa_genkey returned %d\r\n", ret );
		goto exit;
	}
	if( verbose != 0 )
		dump_pubkey( "  + Public key: ", &ctx_sign );

	/*
	 * Sign message hash
	 */
	if( verbose != 0 )
		mbedtls_printf( "2. Signing message...\r\n" );

#if CONFIG_ARCH_RISCV
	before = riscv_get_mtimer();
#else
	before = 0;
#endif

	for(i = 0; i < loop; i++)
	{
		if( ( ret = mbedtls_ecdsa_write_signature( &ctx_sign, MBEDTLS_MD_SHA384,
		                                   Vector_P384_Message, sizeof( Vector_P384_Message ),
		                                   sig, &sig_len,
		                                   gen_rand, NULL ) ) != 0 )
		{
			mbedtls_printf( " failed\n  ! mbedtls_ecdsa_write_signature returned %d\r\n", ret );
			goto exit;
		}
	}
#if CONFIG_ARCH_RISCV
	after = riscv_get_mtimer();
#else
	after = 0;
#endif
	mbedtls_printf("signature run %d times, take time %d us.\r\n", loop,(after - before) / 26);
	if( verbose != 0 )
		dump_buf( "  + Signature: ", sig, sig_len );

	/*
	 * Transfer public information to verifying context
	 *
	 * We could use the same context for verification and signatures, but we
	 * chose to use a new one in order to make it clear that the verifying
	 * context only needs the public key (Q), and not the private key (d).
	 */
	if( verbose != 0 )
		mbedtls_printf( "3. Preparing verification context...\r\n" );

	if( ( ret = mbedtls_ecp_group_copy( &ctx_verify.grp, &ctx_sign.grp ) ) != 0 )
	{
		mbedtls_printf( " failed\n  ! mbedtls_ecp_group_copy returned %d\r\n", ret );
		goto exit;
	}

	if( ( ret = mbedtls_ecp_copy( &ctx_verify.Q, &ctx_sign.Q ) ) != 0 )
	{
		mbedtls_printf( " failed\n  ! mbedtls_ecp_copy returned %d\r\n", ret );
		goto exit;
	}

	/*
	 * Verify signature
	 */
	if( verbose != 0 )
		mbedtls_printf( "4. Verifying signature...\r\n" );

#if CONFIG_ARCH_RISCV
	before = riscv_get_mtimer();
#else
	before = 0;
#endif

	for(i = 0; i < loop; i++)
	{
		if( ( ret = mbedtls_ecdsa_read_signature( &ctx_verify,
		                                  Vector_P384_Message, sizeof( Vector_P384_Message ),
		                                  sig, sig_len ) ) != 0 )
		{
			mbedtls_printf( " failed\n  ! mbedtls_ecdsa_read_signature returned %d\r\n", ret );
			goto exit;
		}
	}
#if CONFIG_ARCH_RISCV
	after = riscv_get_mtimer();
#else
	after = 0;
#endif
	mbedtls_printf("verify run %d times, take time %d us.\r\n", loop,(after - before) / 26);
	if( verbose != 0 )
		mbedtls_printf( " ecdsa self test suc\r\n" );

exit:

	mbedtls_ecdsa_free( &ctx_verify );
	mbedtls_ecdsa_free( &ctx_sign );

	return 0;
}

#endif
#endif /* MBEDTLS_ECDSA_C */
