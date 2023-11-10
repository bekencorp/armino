/*
 *  HKDF implementation -- RFC 5869
 *
 *  Copyright (C) 2016-2018, ARM Limited, All Rights Reserved
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
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_HKDF_C)

#include <string.h>
#include "mbedtls/hkdf.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

int mbedtls_hkdf( const mbedtls_md_info_t *md, const unsigned char *salt,
                  size_t salt_len, const unsigned char *ikm, size_t ikm_len,
                  const unsigned char *info, size_t info_len,
                  unsigned char *okm, size_t okm_len )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    unsigned char prk[MBEDTLS_MD_MAX_SIZE];

    ret = mbedtls_hkdf_extract( md, salt, salt_len, ikm, ikm_len, prk );

    if( ret == 0 )
    {
        ret = mbedtls_hkdf_expand( md, prk, mbedtls_md_get_size( md ),
                                   info, info_len, okm, okm_len );
    }

    mbedtls_platform_zeroize( prk, sizeof( prk ) );

    return( ret );
}

int mbedtls_hkdf_extract( const mbedtls_md_info_t *md,
                          const unsigned char *salt, size_t salt_len,
                          const unsigned char *ikm, size_t ikm_len,
                          unsigned char *prk )
{
    unsigned char null_salt[MBEDTLS_MD_MAX_SIZE] = { '\0' };

    if( salt == NULL )
    {
        size_t hash_len;

        if( salt_len != 0 )
        {
            return MBEDTLS_ERR_HKDF_BAD_INPUT_DATA;
        }

        hash_len = mbedtls_md_get_size( md );

        if( hash_len == 0 )
        {
            return MBEDTLS_ERR_HKDF_BAD_INPUT_DATA;
        }

        salt = null_salt;
        salt_len = hash_len;
    }

    return( mbedtls_md_hmac( md, salt, salt_len, ikm, ikm_len, prk ) );
}

int mbedtls_hkdf_expand( const mbedtls_md_info_t *md, const unsigned char *prk,
                         size_t prk_len, const unsigned char *info,
                         size_t info_len, unsigned char *okm, size_t okm_len )
{
    size_t hash_len;
    size_t where = 0;
    size_t n;
    size_t t_len = 0;
    size_t i;
    int ret = 0;
    mbedtls_md_context_t ctx;
    unsigned char t[MBEDTLS_MD_MAX_SIZE];

    if( okm == NULL )
    {
        return( MBEDTLS_ERR_HKDF_BAD_INPUT_DATA );
    }

    hash_len = mbedtls_md_get_size( md );

    if( prk_len < hash_len || hash_len == 0 )
    {
        return( MBEDTLS_ERR_HKDF_BAD_INPUT_DATA );
    }

    if( info == NULL )
    {
        info = (const unsigned char *) "";
        info_len = 0;
    }

    n = okm_len / hash_len;

    if( okm_len % hash_len != 0 )
    {
        n++;
    }

    /*
     * Per RFC 5869 Section 2.3, okm_len must not exceed
     * 255 times the hash length
     */
    if( n > 255 )
    {
        return( MBEDTLS_ERR_HKDF_BAD_INPUT_DATA );
    }

    mbedtls_md_init( &ctx );

    if( ( ret = mbedtls_md_setup( &ctx, md, 1 ) ) != 0 )
    {
        goto exit;
    }

    memset( t, 0, hash_len );

    /*
     * Compute T = T(1) | T(2) | T(3) | ... | T(N)
     * Where T(N) is defined in RFC 5869 Section 2.3
     */
    for( i = 1; i <= n; i++ )
    {
        size_t num_to_copy;
        unsigned char c = i & 0xff;

        ret = mbedtls_md_hmac_starts( &ctx, prk, prk_len );
        if( ret != 0 )
        {
            goto exit;
        }

        ret = mbedtls_md_hmac_update( &ctx, t, t_len );
        if( ret != 0 )
        {
            goto exit;
        }

        ret = mbedtls_md_hmac_update( &ctx, info, info_len );
        if( ret != 0 )
        {
            goto exit;
        }

        /* The constant concatenated to the end of each T(n) is a single octet.
         * */
        ret = mbedtls_md_hmac_update( &ctx, &c, 1 );
        if( ret != 0 )
        {
            goto exit;
        }

        ret = mbedtls_md_hmac_finish( &ctx, t );
        if( ret != 0 )
        {
            goto exit;
        }

        num_to_copy = i != n ? hash_len : okm_len - where;
        memcpy( okm + where, t, num_to_copy );
        where += hash_len;
        t_len = hash_len;
    }

exit:
    mbedtls_md_free( &ctx );
    mbedtls_platform_zeroize( t, sizeof( t ) );

    return( ret );
}

#if defined(MBEDTLS_SELF_TEST)
#include <stdlib.h>
#include "md_internal.h"

#define mbedtls_printf os_printf

const unsigned char ikm_val[] =
{
	0xAB, 0xE1, 0x0A, 0xCE, 0x13, 0xE7, 0xE1, 0xD9, 0x18, 0x6C, 0x48, 0xF7, 0x88, 0x9D, 0x51, 0x47,
	0x3D, 0x3A, 0x09, 0x61, 0x98, 0x4B, 0xC8, 0x72, 0xDF, 0x70, 0x8E, 0xCC, 0x3E, 0xD3, 0xB8, 0x16
}; 

const unsigned char info_val[] =
{
	0x9D, 0x01, 0xE3, 0xD9, 0x6F, 0xC4, 0xF1, 0xD5, 0xEA, 0x00, 0xA0, 0x36, 0x92, 0xBC, 0xC5, 0xCF,
	0xFD, 0x53, 0x78, 0x7C, 0x88, 0xB9, 0x34, 0xAF, 0x40, 0x4C, 0x03, 0x9D, 0x32, 0x89, 0xB5, 0xBA
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

extern u64 riscv_get_mtimer(void);
int mbedtls_hkdf_self_test( int verbose , mbedtls_md_type_t type, uint32_t okm_size)
{
	int ret = 0;
	const mbedtls_md_info_t *md;
	unsigned char *okm;
	u64 before, after =  0;

	okm = malloc(okm_size);
	if(okm == NULL)
		return MBEDTLS_ERR_MD_ALLOC_FAILED;

	md = mbedtls_md_info_from_type(type);

#if CONFIG_ARCH_RISCV
    before = riscv_get_mtimer();
#else
    before = 0;
#endif

	ret = mbedtls_hkdf( md, NULL,0,
						ikm_val, sizeof(ikm_val),
						info_val, sizeof(info_val),
						okm,okm_size );

#if CONFIG_ARCH_RISCV
    after = riscv_get_mtimer();
#else
    after = 0;
#endif
	mbedtls_printf("run 1 times, take time %d us.\r\n", (uint32_t)(after - before) / 26);
	if( verbose != 0 )
		dump_buf("hkdf:",okm,okm_size);

	free(okm);
	return ret;
}
#endif
#endif /* MBEDTLS_HKDF_C */
