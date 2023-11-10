#ifndef __BK_HASH_ALT_COMMON_H__
#define __BK_HASH_ALT_COMMON_H__

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_SHA512_ALT) || defined(MBEDTLS_SHA256_ALT) || defined(MBEDTLS_SHA1_ALT)
#ifdef __cplusplus
extern "C" {
#endif
#if defined(MBEDTLS_SHA1_ALT)
#include "bk_sha1_alt.h"
#include "mbedtls/sha1.h"
#endif
#if defined(MBEDTLS_SHA256_ALT)
#include "bk_sha256_alt.h"
#include "mbedtls/sha256.h"
#endif
#if defined(MBEDTLS_SHA512_ALT)
#include "bk_sha512_alt.h"
#include "mbedtls/sha512.h"
#endif
#include "api_val.h"
#include <os/mem.h>

typedef enum{
    SHA1_ALT = 0,
    SHA224_ALT,
    SHA256_ALT,
    SHA384_ALT,
    SHA512_ALT,
    SHA_NOT_SUPPORTED
} hash_alg_t;

int bk_val_sym_alloc(ValSymAlgo_t alg, void **context_p);
int bk_hash_update(void *context_p, \
                        unsigned char *data, \
                        uint32_t *llen, \
                        hash_alg_t hash_alg, \
                        const unsigned char *input, \
                        size_t ilen);
#ifdef __cplusplus
}
#endif
#endif
#endif
