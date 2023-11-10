#ifndef MBEDTLS_AES_ALT_H
#define MBEDTLS_AES_ALT_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_AES_ALT)
#ifdef __cplusplus
extern "C" {
#endif

#include <driver/securityip.h>

#define VAL_SYM_ALGO_MAX_CIPHER_KEY_SIZE     (256/8)

/** sym_context_ptr_t - Pointer to the Symmetric Crypto context */
typedef void * sym_context_ptr_t;
/**
 * \brief The AES context-type definition.
 */
typedef struct mbedtls_aes_context
{
	sym_context_ptr_t sym_context_p;
	uint32_t key_asset_id;
	uint8_t key[2*VAL_SYM_ALGO_MAX_CIPHER_KEY_SIZE];        //Store aes key
	uint32_t key_size;
	bool key_init_done;
}mbedtls_aes_context;

#define mbedtls_aes_xts_context mbedtls_aes_context
#if 0
/**
 * \brief The AES XTS context-type definition.
 */
typedef struct mbedtls_aes_xts_context
{
	sym_context_ptr_t sym_context_p;
	uint32_t key_asset_id;
	uint8_t key[2*VAL_SYM_ALGO_MAX_CIPHER_KEY_SIZE];        //Store aes key
	uint32_t key_size;
	uint8_t t_weak;
	bool key_init_done;
} mbedtls_aes_xts_context;
#endif

#ifdef __cplusplus
}
#endif
#endif

#endif

