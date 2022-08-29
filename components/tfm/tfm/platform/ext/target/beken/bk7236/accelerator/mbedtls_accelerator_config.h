#ifndef MBEDTLS_ACCELERATOR_CONF_H
#define MBEDTLS_ACCELERATOR_CONF_H

#define MBEDTLS_NO_UDBL_DIVISION
#define MBEDTLS_PLATFORM_NO_STD_FUNCTIONS
#define MBEDTLS_PLATFORM_PRINTF_ALT
#define MBEDTLS_PLATFORM_SNPRINTF_ALT
#define MBEDTLS_AES_ALT
#define MBEDTLS_SM4_ALT
#define MBEDTLS_CCM_ALT
#define MBEDTLS_CMAC_ALT
#define MBEDTLS_GCM_ALT
#define MBEDTLS_SHA1_ALT
#define MBEDTLS_SHA256_ALT
#define MBEDTLS_ENTROPY_HARDWARE_ALT
#define MBEDTLS_AES_ROM_TABLES
#define MBEDTLS_ECJPAKE_C
#define MBEDTLS_DES_C

#undef MBEDTLS_HAVE_TIME
#undef MBEDTLS_HAVE_TIME_DATE
#undef MBEDTLS_ENTROPY_NV_SEED
#undef MBEDTLS_ECP_DP_CURVE25519_ENABLED
#undef MBEDTLS_ECP_DP_CURVE448_ENABLED

/* enable PSK */
#define MBEDTLS_KEY_EXCHANGE__SOME__PSK_ENABLED
#define MBEDTLS_SSL_CIPHERSUITES MBEDTLS_TLS_PSK_WITH_AES_128_CBC_SHA256
#define memmove psa_memmove
#define vsnprintf psa_vsnprintf

#define MBEDTLS_SSL_MAX_CONTENT_LEN             4096 /**< Maxium fragment length in bytes, determines the size of each of the two internal I/O buffers */

/*add config for Dubhe*/
#define MBEDTLS_SM2DSA_C
#define MBEDTLS_SM2KEP_C
#define MBEDTLS_SM2PKE_C
#define MBEDTLS_SM2DSA_DETERMINISTIC
#define MBEDTLS_ECP_DP_SM2P256V1_ENABLED
#define MBEDTLS_SM3_C
#define MBEDTLS_SM4_C

/*below two macro is used for side channel attack mitigation
 * it will be commented by default, if you want to enable the
 * SCA mitigation,remember to uncomment it
 * */
//#define MBEDTLS_ENBALE_TIMING_ATTACK_MITIGATION
//#define MBEDTLS_ENBALE_SCALAR_MUL_COUNTERMEASURE
#define MBEDTLS_ENTROPY_FORCE_SHA256
#define MBEDTLS_CTR_DRBG_MAX_REQUEST             (10 * 1024) /**< Maximum number of requested bytes per call */

#endif /* MBEDTLS_ACCELERATOR_CONF_H */
