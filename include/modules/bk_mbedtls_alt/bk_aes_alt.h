#ifndef MBEDTLS_AES_ALT_H
#define MBEDTLS_AES_ALT_H

#include <driver/securityip.h>


#define MBEDTLS_AES_ENCRYPT     1 /**< AES encryption. */
#define MBEDTLS_AES_DECRYPT     0 /**< AES decryption. */
#define VAL_SYM_ALGO_MAX_CIPHER_KEY_SIZE     (256/8)

/* Error codes in range 0x0020-0x0022 */
#define MBEDTLS_ERR_AES_INVALID_KEY_LENGTH                -0x0020  /**< Invalid key length. */
#define MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH              -0x0022  /**< Invalid data input length. */

/* Error codes in range 0x0021-0x0025 */
#define MBEDTLS_ERR_AES_BAD_INPUT_DATA                    -0x0021  /**< Invalid input data. */

/* MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE is deprecated and should not be used. */
#define MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE               -0x0023  /**< Feature not available. For example, an unsupported AES key size. */

/* MBEDTLS_ERR_AES_HW_ACCEL_FAILED is deprecated and should not be used. */
#define MBEDTLS_ERR_AES_HW_ACCEL_FAILED                   -0x0025  /**< AES hardware accelerator failed. */

#if CONFIG_SECURITYIP
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
#endif
#endif

