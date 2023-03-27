/*
 * Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#ifndef __PSA_SP_API_H__
#define __PSA_SP_API_H__

#ifndef __ASSEMBLY__

#include <stdarg.h>
#include "psa_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Generic API */
void *psa_malloc(size_t size);
void *psa_realloc(void *buffer, size_t size);
void *psa_calloc(size_t nmemb, size_t size);
void psa_free(void *buffer);

void *psa_memmove(void *dst, const void *src, size_t size);
void *psa_memset(void *buffer, int c, size_t size);
int32_t psa_memcmp(const void *buffer1, const void *buffer2, size_t size);

int psa_printf(const char *format, ...);
int psa_vprintf(const char *format, va_list vargs);
int psa_snprintf(char *out, unsigned int count, const char *format, ...);
int psa_vsnprintf(char *out, unsigned int count, const char *format,
                  va_list vargs);

/* Time API */
void psa_udelay(uint32_t us);
void psa_mdelay(uint32_t msecs);
psa_status_t psa_msleep(uint32_t msecs);
ulong_t psa_get_tick(void);

#if 0
/* Crypto API */
typedef void *psa_crypto_ctx;
/* MD */
#define PSA_SHA1 0x0000
#define PSA_SHA224 0x0001
#define PSA_SHA256 0x0002
#define PSA_SHA384 0x0003
#define PSA_SHA512 0x0004
#define PSA_MD5 0x0005
#define PSA_SM3 0x0006

/* Operation Mode */
#define OP_MODE_ENC 0
#define OP_MODE_DEC 1

/* Symmetric Cipher */
#define PSA_AES_ECB 0x1000
#define PSA_AES_CBC 0x1001
#define PSA_AES_CTR 0x1002
#define PSA_AES_CTS 0x1003
#define PSA_AES_XTS 0x1004
#define PSA_AES_CCM 0x1005
#define PSA_AES_GCM 0x1006
#define PSA_AES_CFB8 0x1007
#define PSA_AES_CFB128 0x1008

#define PSA_DES_ECB 0x1100
#define PSA_DES_CBC 0x1101
#define PSA_DES3_ECB 0x1102
#define PSA_DES3_CBC 0x1103

#define PSA_SM4_ECB 0x1200
#define PSA_SM4_CCB 0x1201
#define PSA_SM4_CTR 0x1202
#define PSA_SM4_CFB8 0x1203
#define PSA_SM4_CFB128 0x1204

/* MAC */
#define PSA_MAC_AES_CBC 0x2000
#define PSA_MAC_AES_CMAC 0x2001
#define PSA_MAC_DES_CBC 0x2002
#define PSA_MAC_DES3_CBC 0x2003
#define PSA_MAC_MD5 0x2004
#define PSA_MAC_SHA1 0x2005
#define PSA_MAC_SHA224 0x2006
#define PSA_MAC_SHA256 0x2007
#define PSA_MAC_SHA384 0x2008
#define PSA_MAC_SHA512 0x2009

/* RSA */
typedef struct __psa_rsa_keypair psa_rsa_keypairt_t;
typedef struct __psa_rsa_pubkey psa_rsa_pubkey_t;

typedef enum _psa_rsa_pad_type_t {
    PSA_RSA_NOPAD                   = 0x00,

    PSA_RSAES_PKCS1_V1_5            = 0x10,
    PSA_RSAES_PKCS1_OAEP_MGF1       = 0x11,

    PSA_RSASSA_PKCS1_V1_5           = 0x20,
    PSA_RSASSA_PKCS1_PSS_MGF1       = 0x21,
} psa_rsa_pad_type_t;

typedef struct _psa_rsa_padding_t {
    psa_rsa_pad_type_t type;
    union {
        struct {
            uint32_t type;   /* sha1/sha224/sha256/sha384/sha512 */
            const uint8_t *label;
            size_t label_size;
        } rsaes_oaep_mgf1;
        struct {
            uint32_t type;   /* md5/sha1/sha224/sha256/sha384/sha512 */
        } rsassa_v1_5;
        struct {
            uint32_t type;   /* sha1/sha224/sha256/sha384/sha512 */
            size_t salt_len;
        } rsassa_pss_mgf1;
    } pad;
} psa_rsa_padding_t;

/* DH */
typedef struct __psa_dh_keypair   psa_dh_keypair_t;
typedef struct __psa_dh_pubkey    psa_dh_pubkey_t;

/* ECC */
/* secg curves */
#define PSA_ECC_CURVE_SECP112R1 0x0001
#define PSA_ECC_CURVE_SECP112R2 0x0002
#define PSA_ECC_CURVE_SECP128R1 0x0003
#define PSA_ECC_CURVE_SECP128R2 0x0004
#define PSA_ECC_CURVE_SECP160K1 0x0005
#define PSA_ECC_CURVE_SECP160R1 0x0006
#define PSA_ECC_CURVE_SECP160R2 0x0007
#define PSA_ECC_CURVE_SECP192K1 0x0008
#define PSA_ECC_CURVE_SECP192R1 0x0009 /* is the same as X9.62 prime192v1 */
#define PSA_ECC_CURVE_SECP224K1 0x000A
#define PSA_ECC_CURVE_SECP224R1 0x000B
#define PSA_ECC_CURVE_SECP256K1 0x000C
#define PSA_ECC_CURVE_SECP256R1 0x000D /* is the same as X9.62 prime256v1 */
#define PSA_ECC_CURVE_SECP384R1 0x000E
#define PSA_ECC_CURVE_SECP252R1 0x000F

#define PSA_ECC_CURVE_SECT113R1 0x0100
#define PSA_ECC_CURVE_SECT113R2 0x0101
#define PSA_ECC_CURVE_SECT131R1 0x0102
#define PSA_ECC_CURVE_SECT131R2 0x0103
#define PSA_ECC_CURVE_SECT163K1 0x0104
#define PSA_ECC_CURVE_SECT163R1 0x0105
#define PSA_ECC_CURVE_SECT163R2 0x0106
#define PSA_ECC_CURVE_SECT193R1 0x0107
#define PSA_ECC_CURVE_SECT193R2 0x0108
#define PSA_ECC_CURVE_SECT233K1 0x0109
#define PSA_ECC_CURVE_SECT233R1 0x010A
#define PSA_ECC_CURVE_SECT239K1 0x010B
#define PSA_ECC_CURVE_SECT283K1 0x010C
#define PSA_ECC_CURVE_SECT283R1 0x010D
#define PSA_ECC_CURVE_SECT409K1 0x010E
#define PSA_ECC_CURVE_SECT409R1 0x010F
#define PSA_ECC_CURVE_SECT571K1 0x0110
#define PSA_ECC_CURVE_SECT571R1 0x0111

/* x9.62 */
#define PSA_ECC_CURVE_X9_62_PRIME192V1 0x0200
#define PSA_ECC_CURVE_X9_62_PRIME192V2 0x0201
#define PSA_ECC_CURVE_X9_62_PRIME192V3 0x0202
#define PSA_ECC_CURVE_X9_62_PRIME239V1 0x0203
#define PSA_ECC_CURVE_X9_62_PRIME239V2 0x0204
#define PSA_ECC_CURVE_X9_62_PRIME239V3 0x0205
#define PSA_ECC_CURVE_X9_62_PRIME256V1 0x0206

/* NIST */
#define PSA_ECC_CURVE_NIST_P192 0x0300
#define PSA_ECC_CURVE_NIST_P224 0x0301
#define PSA_ECC_CURVE_NIST_P256 0x0302
#define PSA_ECC_CURVE_NIST_P384 0x0303
#define PSA_ECC_CURVE_NIST_P521 0x0304

/* SM2 */
#define PSA_ECC_SM2 0x0400

typedef struct __psa_ecc_keypair  psa_ecc_keypair_t;
typedef struct __psa_ecc_pubkey   psa_ecc_pubkey_t;

/* Message Digest */
#define PSA_SHA1_HASH_SIZE 20
#define PSA_SHA224_HASH_SIZE 28
#define PSA_SHA256_HASH_SIZE 32
#define PSA_SHA384_HASH_SIZE 48
#define PSA_SHA512_HASH_SIZE 64
#define PSA_MD5_HASH_SIZE 16

psa_status_t psa_hash_ctx_alloc(psa_crypto_ctx *ctx, uint32_t type);
void psa_hash_ctx_free(psa_crypto_ctx ctx);
psa_status_t psa_hash_init(psa_crypto_ctx ctx, uint32_t type);
psa_status_t psa_hash_update(psa_crypto_ctx ctx, const uint8_t *data,
                            size_t size);
psa_status_t psa_hash_final(psa_crypto_ctx ctx, uint8_t *dgst, size_t *dgst_size);
psa_status_t psa_hash_ctx_copy(psa_crypto_ctx dst_ctx,
                              const psa_crypto_ctx src_ctx);
psa_status_t psa_hash(uint32_t type, const uint8_t *data, size_t data_size,
                     uint8_t *dgst, size_t *dgst_size);

/* Symmetric Ciphers */
psa_status_t psa_cipher_ctx_alloc(psa_crypto_ctx *ctx, uint32_t type);
psa_status_t psa_cipher_ctx_free(psa_crypto_ctx ctx);
psa_status_t psa_cipher_init(psa_crypto_ctx ctx, uint32_t type, uint8_t op_mode,
                            const uint8_t *key1, size_t key1_size,
                            const uint8_t *key2, size_t key2_size, /* for xts */
                            const uint8_t *iv, size_t iv_size);
psa_status_t psa_cipher_process(psa_crypto_ctx ctx,
                               const uint8_t *data_in, size_t in_size,
                               uint8_t *data_out, size_t *out_size);
psa_status_t psa_cipher_final(psa_crypto_ctx ctx,
                             uint8_t *data_out, size_t *out_size);
psa_status_t psa_cipher_ctx_copy(psa_crypto_ctx dst_ctx,
                                const psa_crypto_ctx src_ctx);

/* Key Derive */
psa_status_t psa_dev_spec_key_derive(const uint8_t *ad, size_t ad_size,
                                    uint8_t *out, size_t *out_len);

/* Symmetric Authenticated Encryption */
psa_status_t psa_ae_ctx_alloc(psa_crypto_ctx *ctx, uint32_t type);
psa_status_t psa_ae_ctx_free(psa_crypto_ctx ctx);
psa_status_t psa_ae_init(psa_crypto_ctx ctx, uint32_t type, uint8_t op_mode,
                        const uint8_t *key, size_t key_size,
                        const uint8_t *iv, size_t iv_size,
                        const uint8_t *ad, size_t ad_size);
psa_status_t psa_ae_process(psa_crypto_ctx ctx,
                           const uint8_t *data_in, size_t in_size,
                           uint8_t *data_out, size_t *out_size);
psa_status_t psa_ae_final(psa_crypto_ctx ctx,
                         uint8_t *data_out, size_t *out_size,
                         uint8_t *tag, size_t *tag_size);
psa_status_t psa_ae_copy(psa_crypto_ctx dst_ctx,
                        const psa_crypto_ctx src_ctx);

/* MAC */
psa_status_t psa_mac_ctx_alloc(psa_crypto_ctx *ctx, uint32_t type);
psa_status_t psa_mac_ctx_free(psa_crypto_ctx ctx);
psa_status_t psa_mac_init(psa_crypto_ctx ctx, uint32_t type,
                         const uint8_t *key, size_t key_size);
psa_status_t psa_mac_process(psa_crypto_ctx ctx,
                            const uint8_t *data, size_t size);
psa_status_t psa_mac_ctx_copy(psa_crypto_ctx dst_ctx,
                             const psa_crypto_ctx src_ctx);

/* Asymmetric-RSA */
psa_status_t psa_rsa_keypair_alloc(psa_rsa_keypairt_t **key_pair,
                                  size_t key_bits);
psa_status_t psa_rsa_keypair_free(psa_rsa_keypairt_t *key_pair);
psa_status_t psa_rsa_pubkey_alloc(psa_rsa_pubkey_t **pub_key, size_t key_bits);
psa_status_t psa_rsa_pubkey_free(psa_rsa_pubkey_t *pub_key);
psa_status_t psa_rsa_gen_keypair(psa_rsa_keypairt_t *keypair, size_t key_bits);
psa_status_t psa_rsa_init_keypair(psa_rsa_keypairt_t *keypair, size_t key_bits,
                                 const uint8_t *n, size_t n_size,
                                 const uint8_t *e, size_t e_size,
                                 const uint8_t *d, size_t d_size,
                                 const uint8_t *p, size_t p_size,
                                 const uint8_t *q, size_t q_size,
                                 const uint8_t *dp, size_t dp_size,
                                 const uint8_t *dq, size_t dq_size,
                                 const uint8_t *qp, size_t qp_size);
psa_status_t psa_rsa_init_pubkey(psa_rsa_pubkey_t *pub_key, size_t key_bits,
                                 const uint8_t *n, size_t n_size,
                                 const uint8_t *e, size_t e_size);
psa_status_t psa_rsa_public_encrypt(const psa_rsa_pubkey_t *pub_key,
                                   psa_rsa_padding_t *padding,
                                   const uint8_t *data, size_t data_size,
                                   uint8_t *out, size_t *out_size);
psa_status_t psa_rsa_private_decrypt(const psa_rsa_keypairt_t *priv_key,
                                     psa_rsa_padding_t *padding,
                                     const uint8_t *data, size_t data_size,
                                     uint8_t *out, size_t *out_size);
psa_status_t psa_rsa_rsa_sign(const psa_rsa_keypairt_t *priv_key,
                             psa_rsa_padding_t *padding,
                             const uint8_t *dig, size_t dig_size,
                             uint8_t *sig, size_t *sig_size);
psa_status_t psa_rsa_verify(const psa_rsa_pubkey_t *pub_key,
                           psa_rsa_padding_t *padding,
                           const uint8_t *dig, size_t dig_size,
                           const uint8_t *sig, size_t sig_size,
                           bool *result);

/* Asymmetric-DH */
psa_status_t psa_dh_keypair_alloc(psa_dh_keypair_t **key_pair, size_t key_bits);
psa_status_t psa_dh_keypair_free(psa_dh_keypair_t *key_pair);
psa_status_t psa_dh_pubkey_alloc(psa_rsa_pubkey_t **pub_key, size_t key_bits);
psa_status_t psa_dh_pubkey_free(psa_dh_pubkey_t *pub_key);
psa_status_t psa_dh_gen_keypair(psa_dh_keypair_t *keypair,
                               size_t xbits, /* optional */
                               const uint8_t *g, size_t g_size,
                               const uint8_t *p, size_t p_size,
                               const uint8_t *q, size_t q_size /* oiptional */);
psa_status_t psa_dh_init_keyparir(const psa_dh_keypair_t *keyparir,
                                 const uint8_t *g, size_t g_size,
                                 const uint8_t *p, size_t p_size,
                                 const uint8_t *x, size_t x_size,
                                 const uint8_t *y, size_t y_size,
                                 const uint8_t *q, size_t q_size, /* optional */
                                 size_t xbits /* optional */);
psa_status_t psa_dh_init_pubkey(const psa_dh_pubkey_t *pubkey,
                               const uint8_t *y, size_t y_size);
psa_status_t psa_dh_gen_secret(const psa_dh_keypair_t *priv_key,
                              const psa_dh_pubkey_t *pub_key,
                              uint8_t *secret, size_t *secret_size);

/* Asymmetric-ECC */
psa_status_t psa_ecc_keypair_alloc(psa_ecc_keypair_t **key_pair, size_t curve);
psa_status_t psa_ecc_keypair_free(psa_ecc_keypair_t *key_pair);
psa_status_t psa_ecc_pubkey_alloc(psa_ecc_pubkey_t **pub_key, size_t curve);
psa_status_t psa_ecc_pubkey_free(psa_ecc_pubkey_t *pub_key);
psa_status_t psa_ecc_gen_keypair( psa_ecc_keypair_t *keypair, size_t curve);
psa_status_t psa_ecc_init_keyparir(const psa_ecc_keypair_t *keyparir,
                                  const uint8_t *x, size_t x_size,
                                  const uint8_t *y, size_t y_size,
                                  const uint8_t *d, size_t d_size,
                                  size_t curve);
psa_status_t psa_ecc_init_pubkey(const psa_ecc_pubkey_t *pubkey,
                                const uint8_t *x, size_t x_size,
                                const uint8_t *y, size_t y_size,
                                size_t curve);
psa_status_t psa_ecdsa_sign(const psa_ecc_keypair_t *priv_key,
                           const uint8_t *data, size_t data_size,
                           uint8_t *sig, size_t *sig_size);
psa_status_t psa_ecdsa_verify(const psa_ecc_pubkey_t *pubkey,
                             const uint8_t *data, size_t data_size,
                             const uint8_t *sig, size_t sig_size,
                             bool *result);
psa_status_t psa_ecdh_get_secret(const psa_ecc_keypair_t *priv_key,
                                const psa_ecc_pubkey_t *peer_pubkey,
                                uint8_t *secret, size_t *secret_size);

/* Rand */
psa_status_t psa_rand_seed(const uint8_t *seed, size_t size);
psa_status_t psa_rand_gen(uint8_t *buffer, size_t size);
#endif

/* Secure Storage API */
typedef enum _sst_open_flag_t {
    SST_CREATE      = 0x00000001,
    SST_READ        = 0x00000002,
    SST_WRITE       = 0x00000004,
    SST_SHARD_READ  = 0x00000008 + SST_READ,
    SST_SHARD_WRITE = 0x00000010 + SST_WRITE,
} sst_open_flag_t;

#define SST_SEEK_SET (0)
#define SST_SEEK_CUR (1)
#define SST_SEEK_END (2)

#define SST_ERR_SUCCESS 0
#define SST_ERR_ASSET_NOT_PREPARED -256
#define SST_ERR_ASSET_NOT_FOUND -257
#define SST_ERR_PARAM -258
#define SST_ERR_INVALID_HANDLE -259
#define SST_ERR_STORAGE_SYSTEM_FULL -260
#define SST_ERR_SYSTEM_ERROR -261
#define SST_ERR_NULL_POINTER -262
#define SST_ERR_OUT_OF_MEMORY -512
#define SST_ERR_GENERIC -513
#define SST_ERR_PERMISSION -514

int32_t psa_sst_open(uint16_t file_id, sst_open_flag_t flags,
                     uint32_t max_size);
int32_t psa_sst_close(int32_t fd);
int32_t psa_sst_read(int32_t fd, void *buf, size_t count);
int32_t psa_sst_write(int32_t fd, const void *buffer, size_t count);
uint32_t psa_sst_lseek(int32_t fd, int32_t off, int32_t whence);
int32_t psa_sst_unlink(int32_t fd);
int32_t psa_sst_ftruncate(int32_t fd, size_t length);
int32_t  psa_sst_alloc_enumerator(void);
uint16_t psa_sst_get_next(int32_t hd);
void psa_sst_free_enumerator(int32_t hd);

#ifdef __cplusplus
}
#endif

#endif /* __ASSEMBLY__ */

#endif /* __PSA_SP_API_H__ */
