#ifndef MBEDTLS_GCM_ALT_H
#define MBEDTLS_GCM_ALT_H

#include <driver/securityip.h>

#define MBEDTLS_GCM_ENCRYPT     1
#define MBEDTLS_GCM_DECRYPT     0

#define MBEDTLS_ERR_GCM_AUTH_FAILED                       -0x0012  /**< Authenticated decryption failed. */

/* MBEDTLS_ERR_GCM_HW_ACCEL_FAILED is deprecated and should not be used. */
#define MBEDTLS_ERR_GCM_HW_ACCEL_FAILED                   -0x0013  /**< GCM hardware accelerator failed. */

#define MBEDTLS_ERR_GCM_BAD_INPUT                         -0x0014  /**< Bad input parameters to function. */
#define MBEDTLS_ERR_GCM_BAD_INPUT_DATA_LENGTH             -0x0015  /**< Invalid input data len. */

#if CONFIG_SECURITYIP
/** sym_context_ptr_t - Pointer to the Symmetric Crypto context */
typedef void * sym_context_ptr_t;
/**
 * \brief    The GCM context-type definition. The GCM context is passed
 *           to the APIs called.
 */
typedef struct mbedtls_gcm_context
{
	sym_context_ptr_t sym_context_p;
	uint32_t key_asset_id;
}mbedtls_gcm_context;

#endif

#endif

