// Copyright 2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_SHA1_ALT)
#include "bk_hash_alt_common.h"
#include "tls_log.h"
#include "mbedtls/sha1.h"

extern uint32_t bk_get_sym_ctx_size(void);

void __attribute__((weak)) mbedtls_sha1_init( mbedtls_sha1_context *ctx )

{
    if(ctx == NULL)
        return;
    os_memset( ctx, 0, sizeof( mbedtls_sha1_context ) );
}

void __attribute__((weak)) mbedtls_sha1_free( mbedtls_sha1_context *ctx )
{
    if(ctx == NULL)
        return;
    if(ctx != NULL && (NULL != ctx->Context_p)){
		val_SymRelease(ctx->Context_p);
        ctx->Context_p = NULL;
    }
    
    if(ctx != NULL && (NULL != ctx->DataLeft)){
        os_free(ctx->DataLeft);
        ctx->DataLeft = NULL;
    }
	os_memset(ctx, 0, sizeof(mbedtls_sha1_context));
}

void __attribute__((weak)) mbedtls_sha1_clone( mbedtls_sha1_context *dst,
                           const mbedtls_sha1_context *src )
{
    ValSymAlgo_t Algorithm = VAL_SYM_ALGO_NONE;

    if(dst == NULL || src == NULL){
		return;
	}    
    Algorithm = VAL_SYM_ALGO_HASH_SHA1;
    bk_val_sym_alloc(Algorithm, &(dst->Context_p));
    if(dst->DataLeft == NULL){
        dst->DataLeft = os_malloc(BK_SHA1_ALT_MAX_BLOCK_SIZE);
        os_memset(dst->DataLeft, 0, BK_SHA1_ALT_MAX_BLOCK_SIZE);
    }
    dst->LenLeft = src->LenLeft;
    os_memcpy(dst->digest, src->digest, BK_ALT_DIGEST_SHA1_SIZE);
    os_memcpy(dst->DataLeft, src->DataLeft, BK_SHA1_ALT_MAX_BLOCK_SIZE);
    os_memcpy(dst->Context_p, src->Context_p, bk_get_sym_ctx_size());
}

int __attribute__((weak)) mbedtls_internal_sha1_process( mbedtls_sha1_context *ctx,
                                     const unsigned char data[64] )
{
    if(ctx == NULL){
        return MBEDTLS_ERR_SHA1_BAD_INPUT_DATA;
    }
	return 0;
}

int __attribute__((weak)) mbedtls_sha1_starts_ret( mbedtls_sha1_context *ctx)
{
    ValSymAlgo_t Algorithm = VAL_SYM_ALGO_NONE;
    int ret = 0;
    
    if(ctx == NULL){
        ret = MBEDTLS_ERR_SHA1_BAD_INPUT_DATA;
        return ret;
    }
    Algorithm = VAL_SYM_ALGO_HASH_SHA1;
    bk_val_sym_alloc(Algorithm, &(ctx->Context_p));
    if(ctx->DataLeft == NULL){
        ctx->DataLeft = os_malloc(BK_SHA1_ALT_MAX_BLOCK_SIZE);
        os_memset(ctx->DataLeft, 0, BK_SHA1_ALT_MAX_BLOCK_SIZE);
    }	
    return ret;

}

/*
 * SHA-1 process buffer
 */
int __attribute__((weak)) mbedtls_sha1_update_ret( mbedtls_sha1_context *ctx,
                               const unsigned char *input,
                               size_t ilen )
{
    int ret = bk_hash_update(ctx->Context_p, ctx->DataLeft, &(ctx->LenLeft), SHA1_ALT, input, ilen);
    if(ret < 0){
        ret = MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
        mbedtls_sha1_free(ctx);
    }
    return ret;
}

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
void __attribute__((weak)) mbedtls_sha1_update( mbedtls_sha1_context *ctx,
                            const unsigned char *input,
                            size_t ilen )
{
    mbedtls_sha1_update_ret( ctx, input, ilen );
}
#endif

/*
 * SHA-1 final digest
 */
int __attribute__((weak)) mbedtls_sha1_finish_ret( mbedtls_sha1_context *ctx,
                               unsigned char output[20] )
{
    int ret = 0;
    if(ctx == NULL || output == NULL){
        ret = MBEDTLS_ERR_SHA1_BAD_INPUT_DATA;
        goto cleanup;
    }
	ValSize_t digest_size = sizeof(ctx->digest);    
    ret = val_SymHashFinal(ctx->Context_p,
                            ctx->DataLeft,
                            ctx->LenLeft,
                            ctx->digest,
                            &digest_size); 
    if(ret != VAL_SUCCESS){
        MBEDTLS_LOGE("val_SymHashFinal return %d\r\n", ret);
        ret = MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
        goto cleanup;        
    }
    os_memcpy(output, ctx->digest, sizeof(ctx->digest));

    ctx->LenLeft = 0;
    return ret;

cleanup:
    mbedtls_sha1_free(ctx);
    return ret;
}


#if !defined(MBEDTLS_DEPRECATED_REMOVED)
void __attribute__((weak)) mbedtls_sha1_finish( mbedtls_sha1_context *ctx,
                            unsigned char output[20] )
{
    mbedtls_sha1_finish_ret( ctx, output );
}
#endif

#endif /* MBEDTLS_SHA1_ALT */
