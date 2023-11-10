#ifndef __BK_RSA_ALT_H__
#define __BK_RSA_ALT_H__

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif
#include <common/bk_include.h>

#include "mbedtls/bignum.h"
#include "mbedtls/md.h"

// #include "driver/securityip.h"

#if defined(MBEDTLS_THREADING_C)
#include "mbedtls/threading.h"
#endif


/*
 * RSA Error codes
 */
#define MBEDTLS_ERR_RSA_BAD_INPUT_DATA                    -0x4080  /**< Bad input parameters to function. */
#define MBEDTLS_ERR_RSA_INVALID_PADDING                   -0x4100  /**< Input data contains invalid padding and is rejected. */
#define MBEDTLS_ERR_RSA_KEY_GEN_FAILED                    -0x4180  /**< Something failed during generation of a key. */
#define MBEDTLS_ERR_RSA_KEY_CHECK_FAILED                  -0x4200  /**< Key failed to pass the validity check of the library. */
#define MBEDTLS_ERR_RSA_PUBLIC_FAILED                     -0x4280  /**< The public key operation failed. */
#define MBEDTLS_ERR_RSA_PRIVATE_FAILED                    -0x4300  /**< The private key operation failed. */
#define MBEDTLS_ERR_RSA_VERIFY_FAILED                     -0x4380  /**< The PKCS#1 verification failed. */
#define MBEDTLS_ERR_RSA_OUTPUT_TOO_LARGE                  -0x4400  /**< The output buffer for decryption is not large enough. */
#define MBEDTLS_ERR_RSA_RNG_FAILED                        -0x4480  /**< The random generator failed to generate non-zeros. */

/* MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION is deprecated and should not be used.
 */
#define MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION             -0x4500  /**< The implementation does not offer the requested operation, for example, because of security violations or lack of functionality. */

/* MBEDTLS_ERR_RSA_HW_ACCEL_FAILED is deprecated and should not be used. */
#define MBEDTLS_ERR_RSA_HW_ACCEL_FAILED                   -0x4580  /**< RSA hardware accelerator failed. */

/*
 * RSA constants
 */
#define MBEDTLS_RSA_PUBLIC      0 /**< Request private key operation. */
#define MBEDTLS_RSA_PRIVATE     1 /**< Request public key operation. */

#define MBEDTLS_RSA_PKCS_V15    0 /**< Use PKCS#1 v1.5 encoding. */
#define MBEDTLS_RSA_PKCS_V21    1 /**< Use PKCS#1 v2.1 encoding. */

#define MBEDTLS_RSA_SIGN        1 /**< Identifier for RSA signature operations. */
#define MBEDTLS_RSA_CRYPT       2 /**< Identifier for RSA encryption and decryption operations. */

#define MBEDTLS_RSA_SALT_LEN_ANY    -1

#define END_TEST_SUCCES         0
#define END_TEST_FAIL           (-1)
#define END_TEST_UNSUPPORTED    (-2)
#define END_TEST_ABORT          (-1000)

 #define SFZUTF_MALLOC(x)   malloc((x)>0?(x):1)
 #define SFZUTF_CALLOC(x,y) malloc(x*y)
 #define SFZUTF_FREE(x)     free(x)

#ifdef __cplusplus
extern "C" {
#endif
#if defined(MBEDTLS_RSA_ALT)
/**
 * \brief   The RSA context structure.
 *
 * \note    Direct manipulation of the members of this structure
 *          is deprecated. All manipulation should instead be done through
 *          the public interface functions.
 */
typedef struct mbedtls_rsa_context
{
    int ver;                    /*!<  Always 0.*/
    size_t len;                 /*!<  The size of \p N in Bytes. */

    mbedtls_mpi N;              /*!<  The public modulus. */
    mbedtls_mpi E;              /*!<  The public exponent. */

    mbedtls_mpi D;              /*!<  The private exponent. */
    mbedtls_mpi P;              /*!<  The first prime factor. */
    mbedtls_mpi Q;              /*!<  The second prime factor. */

    mbedtls_mpi DP;             /*!<  <code>D % (P - 1)</code>. */
    mbedtls_mpi DQ;             /*!<  <code>D % (Q - 1)</code>. */
    mbedtls_mpi QP;             /*!<  <code>1 / (Q % P)</code>. */

    mbedtls_mpi RN;             /*!<  cached <code>R^2 mod N</code>. */

    mbedtls_mpi RP;             /*!<  cached <code>R^2 mod P</code>. */
    mbedtls_mpi RQ;             /*!<  cached <code>R^2 mod Q</code>. */

    mbedtls_mpi Vi;             /*!<  The cached blinding value. */
    mbedtls_mpi Vf;             /*!<  The cached un-blinding value. */

    int padding;                /*!< Selects padding mode:
                                     #MBEDTLS_RSA_PKCS_V15 for 1.5 padding and
                                     #MBEDTLS_RSA_PKCS_V21 for OAEP or PSS. */
    int hash_id;                /*!< Hash identifier of mbedtls_md_type_t type,
                                     as specified in md.h for use in the MGF
                                     mask generating function used in the
                                     EME-OAEP and EMSA-PSS encodings. */
#if defined(MBEDTLS_THREADING_C)
    mbedtls_threading_mutex_t mutex;    /*!<  Thread-safety mutex. */
#endif
}
mbedtls_rsa_context;

#endif

#ifdef __cplusplus
}
#endif
#endif