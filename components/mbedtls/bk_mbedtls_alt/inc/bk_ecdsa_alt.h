#ifndef MBEDTLS_ECDSA_ALT_H
#define MBEDTLS_ECDSA_ALT_H
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_ECDSA_VERIFY_ALT) || defined(MBEDTLS_ECDSA_SIGN_ALT)
#ifdef __cplusplus
extern "C" {
#endif
#include <driver/securityip.h>
#include <mbedtls/ecdsa.h>
#include <mbedtls/ecp.h>
#ifdef __cplusplus
}
#endif
#endif
#endif

