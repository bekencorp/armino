#ifndef MBEDTLS_CCM_ALT_H
#define MBEDTLS_CCM_ALT_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_CCM_ALT)
#ifdef __cplusplus
extern "C" {
#endif
#include <driver/securityip.h>

#define MBEDTLS_CCM_DECRYPT       0
#define MBEDTLS_CCM_ENCRYPT       1
#define MBEDTLS_CCM_STAR_DECRYPT  2
#define MBEDTLS_CCM_STAR_ENCRYPT  3

/* Error codes in range 0x0021-0x0025 */
#define MBEDTLS_ERR_CCM_BAD_INPUT_DATA         -0x0021  /**< Invalid input data. */
#define MBEDTLS_ERR_CCM_BAD_INPUT_DATA_LENGTH  -0x0022  /**< Invalid input data len. */

/** sym_context_ptr_t - Pointer to the Symmetric Crypto context */
typedef void * sym_context_ptr_t;
/**
 * \brief    The CCM context-type definition. The CCM context is passed
 *           to the APIs called.
 */
typedef struct mbedtls_ccm_context
{
	sym_context_ptr_t sym_context_p;
	uint32_t key_asset_id;
}mbedtls_ccm_context;

#ifdef __cplusplus
}
#endif  
#endif
#endif

