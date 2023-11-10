#ifndef MBEDTLS_GCM_ALT_H
#define MBEDTLS_GCM_ALT_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_GCM_ALT)
#ifdef __cplusplus
extern "C" {
#endif
#include <driver/securityip.h>

#define MBEDTLS_ERR_GCM_BAD_INPUT_DATA_LENGTH             -0x0015  /**< Invalid input data len. */

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

#ifdef __cplusplus
}
#endif  
#endif
#endif

