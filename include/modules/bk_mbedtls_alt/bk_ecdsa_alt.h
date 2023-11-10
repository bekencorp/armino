#ifndef MBEDTLS_ECDSA_ALT_H
#define MBEDTLS_ECDSA_ALT_H

#include <driver/securityip.h>
#include <mbedtls/ecdsa.h>
#include <mbedtls/ecp.h>


#if 0//CONFIG_SECURITYIP
/** sym_context_ptr_t - Pointer to the Symmetric Crypto context */
typedef void * sym_context_ptr_t;
/**
 * \brief           General context for resuming ECDSA operations
 */
typedef struct mbedtls_ecdsa_context
{
	sym_context_ptr_t sym_context_p;
	uint32_t key_asset_id;
}mbedtls_gcm_context;

#endif

int mbedtls_ecdsa_verify_test( mbedtls_ecp_group *grp,
								const unsigned char *buf, size_t blen,
								const mbedtls_ecp_point *Q,
								const mbedtls_mpi *r,
								const mbedtls_mpi *s);

/* Structures for the ECC curves. */
typedef struct
{    
	uint32_t CurveBits;
	const uint8_t * P_p;
	uint32_t PLen;
	const uint8_t * A_p;
	uint32_t ALen;
	const uint8_t * B_p;
	uint32_t BLen;
	const uint8_t * ECPointX_p;
	uint32_t ECPointXLen;
	const uint8_t * ECPointY_p;
	uint32_t ECPointYLen;
	const uint8_t * Order_p;
	uint32_t OrderLen;
	const uint8_t Cofactor;
} TestVector_ECC_Curve_Rec_t;

typedef const TestVector_ECC_Curve_Rec_t * const TestVector_ECC_Curve_t;

#endif

