#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_RSA_C)

#include "mbedtls/rsa.h"
#include "mbedtls/rsa_internal.h"
#include "mbedtls/oid.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

#include <string.h>

#if defined(MBEDTLS_PKCS1_V21)
#include "mbedtls/md.h"
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#endif

//#include "securityip_driver.h"
#include "api_val.h"
#include "bk_rsa_alt.h"
//#include "api_val_asset.h"
#include "tls_log.h"

#if defined(MBEDTLS_RSA_ALT)
/* Parameter validation macros */
#define RSA_VALIDATE_RET( cond )                                       \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_RSA_BAD_INPUT_DATA )
#define RSA_VALIDATE( cond )                                           \
    MBEDTLS_INTERNAL_VALIDATE( cond )
#define ciL    (sizeof(mbedtls_mpi_uint))         /* chars in limb  */
#define MBEDTLS_HW_MAX_BYTES        384//3072bits

#if defined(MBEDTLS_PKCS1_V15)

typedef struct mbedtls_rsa_pkcs1v15_context
{
    uint32_t ModulusBits;
    uint32_t HashBits;
    uint8_t *Modulus_p;
    uint32_t ModulusBytes;
    uint8_t *PublicExponent_p;
    uint32_t PublicExponentBytes;
    uint8_t *PrivateExponent_p;
    uint32_t PrivateExponentBytes;
    const uint8_t * Msg_p;
    uint32_t MsgBytes;
    const uint8_t * Signature_p;
    uint32_t SignatureBytes;

    const uint8_t * PlainData_p;
    uint32_t PlainDataBytes;
    const uint8_t * WrappedData_p;
    uint32_t WrappedDataBytes;

}mbedtls_rsa_pkcs1v15_context_t;
#endif
#if defined(MBEDTLS_PKCS1_V21)

typedef struct mbedtls_rsa_pss_context
{
    uint32_t ModulusBits;
    uint32_t HashBits;
    uint8_t *Modulus_p;
    uint32_t ModulusBytes;
    uint8_t *PublicExponent_p;
    uint32_t PublicExponentBytes;
    uint8_t *PrivateExponent_p;
    uint32_t PrivateExponentBytes;
    const uint8_t * Msg_p;
    uint32_t MsgBytes;
    const uint8_t * Signature_p;
    uint32_t SignatureBytes;

}mbedtls_rsa_pss_context_t;

#endif

static void mbedtls_rsa_release_asset(ValAssetId_t assetId)
{
    if(assetId!=VAL_ASSETID_INVALID)
    {
        (void)val_AssetFree(assetId);
        assetId = VAL_ASSETID_INVALID;
    }
}
static void mbedtls_rsa_release_key_hw(ValAsymKey_t *Key_p)
{
    if (Key_p->DomainAssetId != VAL_ASSETID_INVALID)
    {
        (void)val_AssetFree(Key_p->DomainAssetId);
        Key_p->DomainAssetId = VAL_ASSETID_INVALID;
    }
    if (Key_p->KeyAssetId != VAL_ASSETID_INVALID)
    {
        (void)val_AssetFree(Key_p->KeyAssetId);
        Key_p->KeyAssetId = VAL_ASSETID_INVALID;
    }
}
#if defined(MBEDTLS_PKCS1_V15)

/*
* generate ras pubkey
*/
static int mbedtls_load_rsa_pkcs1v15_pubkey_hw(mbedtls_rsa_pkcs1v15_context_t *ctx,ValAsymKey_t *PublicKey_p)
{
    ValStatus_t Status;
    ValSymAlgo_t HashAlgorithm;
    ValAssetId_t AssetId;
    ValAsymBigInt_t ModulusData;
    ValAsymBigInt_t ExponentData;

    switch(ctx->HashBits)
    {
    case 160:
        HashAlgorithm = VAL_SYM_ALGO_HASH_SHA1;
        break;
    case 224:
        HashAlgorithm = VAL_SYM_ALGO_HASH_SHA224;
        break;
    case 256:
        HashAlgorithm = VAL_SYM_ALGO_HASH_SHA256;
        break;
#ifdef MBEDTLS_SHA512_C
    case 384:
        HashAlgorithm = VAL_SYM_ALGO_HASH_SHA384;
        break;
    case 512:
        HashAlgorithm = VAL_SYM_ALGO_HASH_SHA512;
        break;
#endif
    default:
        return END_TEST_UNSUPPORTED;
    }
    MBEDTLS_LOGI("\r\n2)%s ModulusBits:%d HashBits:%d\r\n",__func__,(ctx->ModulusBits),ctx->HashBits);
    Status=val_AsymRsaPkcs1v15AllocPublicKeyAsset((ctx->ModulusBits),
                                                (ctx->PublicExponentBytes*8),
                                                HashAlgorithm,
                                                false,false,
                                                &AssetId);
    if(Status==VAL_SUCCESS)
    {
        MBEDTLS_LOGI("AllocPubKey success:AssetId:%d\r\n",AssetId);
    }
    else
    {
        MBEDTLS_LOGE("AllocPubKey Fail:Status:%d\r\n",Status);
        return END_TEST_FAIL;
    }

    Status=val_AsymInitKey(AssetId,VAL_ASSETID_INVALID,
                            ctx->ModulusBits,HashAlgorithm,
                            PublicKey_p);
    if(Status==VAL_SUCCESS)
    {
        MBEDTLS_LOGI("InitKey success\r\n");
    }
    else
    {
        MBEDTLS_LOGE("InitKey Fail:Status:%d\r\n",Status);
        return END_TEST_FAIL;
    }

    ModulusData.ByteDataSize = ctx->ModulusBytes;
    ExponentData.ByteDataSize = ctx->PublicExponentBytes;
#ifdef SFZUTF_USERMODE
    ModulusData.Data_p = sfzutf_discard_const(ctx->Modulus_p);
    ExponentData.Data_p = sfzutf_discard_const(ctx->PublicExponent_p);
#else
    ModulusData.Data_p = SFZUTF_MALLOC(ModulusData.ByteDataSize);
    if(ModulusData.Data_p==NULL)
    {
        return END_TEST_FAIL;
    }
    memcpy(ModulusData.Data_p, ctx->Modulus_p, ModulusData.ByteDataSize);
    ExponentData.Data_p = SFZUTF_MALLOC(ExponentData.ByteDataSize);
    if(ExponentData.Data_p==NULL)
    {
        SFZUTF_FREE(ModulusData.Data_p);
        return END_TEST_FAIL;
    }
    memcpy(ExponentData.Data_p, ctx->PublicExponent_p, ExponentData.ByteDataSize);
#endif

    Status = val_AsymRsaLoadKeyAssetPlaintext(&ModulusData,
                                              (ctx->ModulusBits),
                                              &ExponentData,
                                              (ctx->PublicExponentBytes*8),
                                              AssetId,
                                              VAL_ASSETID_INVALID,
                                              NULL, 0, NULL, 0);
    if(Status==VAL_SUCCESS)
    {
        MBEDTLS_LOGI("LoadPlainText success\r\n");
    }
    else
    {
        MBEDTLS_LOGE("LoadPlainText Fail:Status:%d\r\n",Status);
    }

#ifndef SFZUTF_USERMODE
    SFZUTF_FREE(ExponentData.Data_p);
    SFZUTF_FREE(ModulusData.Data_p);
#endif
    if(Status==END_TEST_SUCCES)
    {
        MBEDTLS_LOGI("load pubkey success\r\n");
        return END_TEST_SUCCES;
    }
    else
    {
        MBEDTLS_LOGE("load pubkey Fail\r\n");
        return END_TEST_FAIL;
    }

}/*
* genrate private key
*/
static int mbedtls_load_rsa_pkcs1v15_privKey_hw(mbedtls_rsa_pkcs1v15_context_t *ctx,ValAsymKey_t *PrivateKey_p)
{
    ValStatus_t Status;
    ValSymAlgo_t HashAlgorithm;
    ValAssetId_t AssetId;
    ValAsymBigInt_t ModulusData;
    ValAsymBigInt_t ExponentData;

    switch (ctx->HashBits)
    {
    case 160:
        HashAlgorithm = VAL_SYM_ALGO_HASH_SHA1;
        break;
    case 224:
        HashAlgorithm = VAL_SYM_ALGO_HASH_SHA224;
        break;
    case 256:
        HashAlgorithm = VAL_SYM_ALGO_HASH_SHA256;
        break;
#ifdef MBEDTLS_SHA512_C
    case 384:
        HashAlgorithm = VAL_SYM_ALGO_HASH_SHA384;
        break;
    case 512:
        HashAlgorithm = VAL_SYM_ALGO_HASH_SHA512;
        break;
#endif
    default:
        return END_TEST_UNSUPPORTED;
    }
    MBEDTLS_LOGI("\r\n1)%s: HashBits:%d ModulusBits:%d\r\n",__func__,ctx->HashBits,ctx->ModulusBits);
    Status = val_AsymRsaPkcs1v15AllocPrivateKeyAsset(ctx->ModulusBits,
                                                     (ctx->PrivateExponentBytes*8),
                                                     HashAlgorithm,
                                                     false, false,
                                                     &AssetId);
    if(Status==VAL_SUCCESS)
    {
        MBEDTLS_LOGI("AllocPrivKey success:AssetId:%d\r\n",AssetId);
    }
    else
    {
        MBEDTLS_LOGE("AllocPrivKey Fail:Status:%d\r\n",Status);
        return END_TEST_FAIL;
    }
    Status = val_AsymInitKey(AssetId, VAL_ASSETID_INVALID,
                             ctx->ModulusBits, HashAlgorithm,
                             PrivateKey_p);
    if(Status==VAL_SUCCESS)
    {
        MBEDTLS_LOGI("InitKey success\r\n");
    }
    else
    {
        MBEDTLS_LOGE("InitKey Fail:Status:%d\r\n",Status);
        return END_TEST_FAIL;
    }
    ModulusData.ByteDataSize = ctx->ModulusBytes;
    ExponentData.ByteDataSize = ctx->PrivateExponentBytes;
#ifdef SFZUTF_USERMODE
    ModulusData.Data_p = sfzutf_discard_const(ctx->Modulus_p);
    ExponentData.Data_p = sfzutf_discard_const(ctx->PrivateExponent_p);
#else
    ModulusData.Data_p = (uint8_t *)SFZUTF_MALLOC(ModulusData.ByteDataSize);
    if(ModulusData.Data_p==NULL)
    {
        return END_TEST_FAIL;
    }
    memcpy(ModulusData.Data_p, ctx->Modulus_p, ModulusData.ByteDataSize);
    ExponentData.Data_p = (uint8_t *)SFZUTF_MALLOC(ExponentData.ByteDataSize);
    if(ExponentData.Data_p==NULL)
    {
        SFZUTF_FREE(ModulusData.Data_p);
        return END_TEST_FAIL;
    }
    memcpy(ExponentData.Data_p, ctx->PrivateExponent_p, ExponentData.ByteDataSize);
#endif
    Status = val_AsymRsaLoadKeyAssetPlaintext(&ModulusData,
                                              ctx->ModulusBits,
                                              &ExponentData,
                                              (ctx->PrivateExponentBytes*8),
                                              AssetId,
                                              VAL_ASSETID_INVALID,
                                              NULL, 0, NULL, 0);
    if(Status==VAL_SUCCESS)
    {
        MBEDTLS_LOGI("Load Plaintext success\r\n");
    }
    else
    {
        MBEDTLS_LOGE("Plaintext Fail:Status:%d\r\n",Status);
    }
                                              
#ifndef SFZUTF_USERMODE
    SFZUTF_FREE(ExponentData.Data_p);
    SFZUTF_FREE(ModulusData.Data_p);
#endif
    if(Status==END_TEST_SUCCES)
    {
        MBEDTLS_LOGI("%s Succes\r\n",__func__);
        return END_TEST_SUCCES;
    }
    else
    {
        MBEDTLS_LOGE("%s Fail\r\n",__func__);
        return END_TEST_FAIL;
    }

}
/**
 * generate signature
*/

static int mbedtls_rsa_pkcs1v15_sign_hw(mbedtls_rsa_pkcs1v15_context_t *ctx,ValAsymKey_t * PrivateKey_p,ValAsymBigInt_t * Signature_p)
{
    uint8_t * Msg_p;
    ValStatus_t Status;

#ifdef SFZUTF_USERMODE
    Msg_p = sfzutf_discard_const(ctx->Msg_p);
#else
    Msg_p = SFZUTF_MALLOC(ctx->MsgBytes);
    if(Msg_p==NULL)
    {
        return END_TEST_FAIL;
    }
    memcpy(Msg_p, ctx->Msg_p, ctx->MsgBytes);
#endif

    Status = val_AsymRsaPkcs1v15Sign(PrivateKey_p, Signature_p,
                                     NULL, ctx->MsgBytes, Msg_p);

#ifndef SFZUTF_USERMODE
    SFZUTF_FREE(Msg_p);
#endif
    if(Status==END_TEST_SUCCES)
    {
        MBEDTLS_LOGI("%s Succes\r\n",__func__);
        return END_TEST_SUCCES;
    }
    else
    {
        MBEDTLS_LOGE("%s Fail Status:%d\r\n",__func__,Status);
        return END_TEST_FAIL;
    }
}
/**
 * verify 
*/
static int mbedtls_rsa_pkcs1v15_verify_hw(mbedtls_rsa_pkcs1v15_context_t *ctx,ValAsymKey_t * PublicKey_p,ValAsymBigInt_t * Signature_p)
{
    uint8_t * Msg_p;
    ValStatus_t Status;

#ifdef SFZUTF_USERMODE
    Msg_p = sfzutf_discard_const(ctx->Msg_p);
#else
    Msg_p = SFZUTF_MALLOC(ctx->MsgBytes);
    if(Msg_p==NULL)
    {
        return END_TEST_FAIL;
    }
    memcpy(Msg_p, ctx->Msg_p, ctx->MsgBytes);
#endif

    MBEDTLS_LOGI("4)%s:MsgBytes:%d\r\n",__func__,ctx->MsgBytes);

    Status = val_AsymRsaPkcs1v15Verify(PublicKey_p, Signature_p,
                                       NULL, ctx->MsgBytes, Msg_p);
#ifndef SFZUTF_USERMODE
    SFZUTF_FREE(Msg_p);
#endif
    if(Status==END_TEST_SUCCES)
    {
        MBEDTLS_LOGI("%s Succes\r\n",__func__);
        return END_TEST_SUCCES;
    }
    else
    {
        MBEDTLS_LOGE("%s Fail Status:%d\r\n",__func__,Status);
        return END_TEST_FAIL;
    }
}
#endif
#if defined(MBEDTLS_PKCS1_V21)

//======================================PSS==========================//
/**
 * Helper function that allocates and initializes the RSA PSS private key.
*/
static int mbedtls_load_rsa_pss_privkey_hw(mbedtls_rsa_pss_context_t *ctx,ValAsymKey_t * PrivateKey_p)
{
    ValStatus_t Status;
    ValSymAlgo_t HashAlgorithm;
    ValAssetId_t AssetId;
    ValAsymBigInt_t ModulusData;
    ValAsymBigInt_t ExponentData;

    switch (ctx->HashBits)
    {
    case 160:
        HashAlgorithm = VAL_SYM_ALGO_HASH_SHA1;
        break;
    case 224:
        HashAlgorithm = VAL_SYM_ALGO_HASH_SHA224;
        break;
    case 256:
        HashAlgorithm = VAL_SYM_ALGO_HASH_SHA256;
        break;
#ifdef VALTEST_SYM_ALGO_SHA512
    case 384:
        HashAlgorithm = VAL_SYM_ALGO_HASH_SHA384;
        break;
    case 512:
        HashAlgorithm = VAL_SYM_ALGO_HASH_SHA512;
        break;
#endif
    default:
        return END_TEST_UNSUPPORTED;
    }

    Status = val_AsymRsaPssAllocPrivateKeyAsset(ctx->ModulusBits,
                                                (ctx->PrivateExponentBytes*8),
                                                HashAlgorithm,
                                                false, false,
                                                &AssetId);
    if(Status!=VAL_SUCCESS)
    {
        MBEDTLS_LOGI("Pss Alloc PrivateKey Fail:%d\r\n",Status);
        return END_TEST_FAIL;
    }   
    else
    {
        MBEDTLS_LOGE("Pss Alloc PrivateKey Success\r\n");
    }

    Status = val_AsymInitKey(AssetId, VAL_ASSETID_INVALID,
                             ctx->ModulusBits, HashAlgorithm,
                             PrivateKey_p);
    if(Status!=VAL_SUCCESS)
    {
        MBEDTLS_LOGI("Pss Init PrivateKey Fail:%d\r\n",Status);
        return END_TEST_FAIL;
    }    
    else
    {
        MBEDTLS_LOGE("Pss Init PrivateKey Success\r\n");
    }

    ModulusData.ByteDataSize = ctx->ModulusBytes;
    ExponentData.ByteDataSize = ctx->PrivateExponentBytes;
#ifdef SFZUTF_USERMODE
    ModulusData.Data_p = sfzutf_discard_const(ctx->Modulus_p);
    ExponentData.Data_p = sfzutf_discard_const(ctx->PrivateExponent_p);
#else
    ModulusData.Data_p = SFZUTF_MALLOC(ModulusData.ByteDataSize);
    if(ModulusData.Data_p==NULL)
    {
        return END_TEST_FAIL;
    }
    memcpy(ModulusData.Data_p, ctx->Modulus_p, ModulusData.ByteDataSize);
    ExponentData.Data_p = SFZUTF_MALLOC(ExponentData.ByteDataSize);
    if(ExponentData.Data_p==NULL)
    {
        SFZUTF_FREE(ModulusData.Data_p);
        return END_TEST_FAIL;
    }
    memcpy(ExponentData.Data_p, ctx->PrivateExponent_p, ExponentData.ByteDataSize);
#endif
    Status = val_AsymRsaLoadKeyAssetPlaintext(&ModulusData,
                                              ctx->ModulusBits,
                                              &ExponentData,
                                              (ctx->PrivateExponentBytes*8),
                                              AssetId,
                                              VAL_ASSETID_INVALID,
                                              NULL, 0, NULL, 0);
#ifndef SFZUTF_USERMODE
    SFZUTF_FREE(ExponentData.Data_p);
    SFZUTF_FREE(ModulusData.Data_p);
#endif
    if(Status!=VAL_SUCCESS)
    {
        MBEDTLS_LOGE("Pss Asset Plaintext Fail:%d\r\n",Status);
    }
    else
    {
        MBEDTLS_LOGI("Pss Asset Plaintext Success\r\n");
    }

    return Status;
}
/**
 * Helper function that performs the RSA PSS signature generation operation.
*/
static int mbedtls_rsa_pss_sign_hw(
        mbedtls_rsa_pss_context_t *ctx,
        ValAsymKey_t * PrivateKey_p,
        ValAsymBigInt_t * Signature_p)
{
    ValStatus_t Status;
    uint8_t * Msg_p;

#ifdef SFZUTF_USERMODE
    Msg_p = sfzutf_discard_const(ctx->Msg_p);
#else
    Msg_p = SFZUTF_MALLOC(ctx->MsgBytes);
    if(Msg_p==NULL)
    {
        return END_TEST_FAIL;
    }
    memcpy(Msg_p, ctx->Msg_p, ctx->MsgBytes);
#endif

    Status = val_AsymRsaPssSign(PrivateKey_p, Signature_p,
                                NULL, ctx->MsgBytes, Msg_p,
                                (ctx->HashBits/8));
#ifndef SFZUTF_USERMODE
    SFZUTF_FREE(Msg_p);
#endif
    if(Status!=VAL_SUCCESS)
    {
        MBEDTLS_LOGE("Pss Sign  Fail:%d\r\n",Status);
    }
    else
    {
        MBEDTLS_LOGI("Pss Verify HashBits:%d HashAlg:%d\r\n",ctx->HashBits,PrivateKey_p->HashAlgorithm);

        MBEDTLS_LOGI("Pss Sign Success\r\n");
    }

    return Status;
}
/**
 * Helper function that allocates and initializes the RSA PSS public key.
*/
static int mbedtls_load_rsa_pss_pubkey_hw(
        mbedtls_rsa_pss_context_t *ctx,
        ValAsymKey_t * PublicKey_p)
{
    ValStatus_t Status;
    ValSymAlgo_t HashAlgorithm;
    ValAssetId_t AssetId;
    ValAsymBigInt_t ModulusData;
    ValAsymBigInt_t ExponentData;

    switch (ctx->HashBits)
    {
    case 160:
        HashAlgorithm = VAL_SYM_ALGO_HASH_SHA1;
        break;
    case 224:
        HashAlgorithm = VAL_SYM_ALGO_HASH_SHA224;
        break;
    case 256:
        HashAlgorithm = VAL_SYM_ALGO_HASH_SHA256;
        break;
#ifdef VALTEST_SYM_ALGO_SHA512
    case 384:
        HashAlgorithm = VAL_SYM_ALGO_HASH_SHA384;
        break;
    case 512:
        HashAlgorithm = VAL_SYM_ALGO_HASH_SHA512;
        break;
#endif
    default:
        return END_TEST_UNSUPPORTED;
    }

    Status = val_AsymRsaPssAllocPublicKeyAsset(ctx->ModulusBits,
                                               (ctx->PublicExponentBytes*8),
                                               HashAlgorithm,
                                               false, false,
                                               &AssetId);
    if(Status!=VAL_SUCCESS)
    {
        MBEDTLS_LOGE("RsaPss Alloc PublicKey Fail:%d\r\n",Status);
        return END_TEST_FAIL;
    }    
    else
    {
        MBEDTLS_LOGI("RsaPss Alloc PublicKey Success\r\n");
    }

    Status = val_AsymInitKey(AssetId, VAL_ASSETID_INVALID,
                             ctx->ModulusBits, HashAlgorithm,
                             PublicKey_p);
    if(Status!=VAL_SUCCESS)
    {
        MBEDTLS_LOGE("RsaPss Init PublicKey Fail:%d\r\n",Status);
        return END_TEST_FAIL;
    }
    else
    {
        MBEDTLS_LOGI("RsaPss Init PublicKey Success\r\n");
    }

    ModulusData.ByteDataSize = ctx->ModulusBytes;
    ExponentData.ByteDataSize = ctx->PublicExponentBytes;
#ifdef SFZUTF_USERMODE
    ModulusData.Data_p = sfzutf_discard_const(ctx->Modulus_p);
    ExponentData.Data_p = sfzutf_discard_const(ctx->PublicExponent_p);
#else
    ModulusData.Data_p = SFZUTF_MALLOC(ModulusData.ByteDataSize);
    if(ModulusData.Data_p==NULL)
    {
        return END_TEST_FAIL;
    }
    memcpy(ModulusData.Data_p, ctx->Modulus_p, ModulusData.ByteDataSize);
    ExponentData.Data_p = SFZUTF_MALLOC(ExponentData.ByteDataSize);
    if(ExponentData.Data_p==NULL)
    {
        SFZUTF_FREE(ModulusData.Data_p);
        return END_TEST_FAIL;
    }
    memcpy(ExponentData.Data_p, ctx->PublicExponent_p, ExponentData.ByteDataSize);
#endif
    Status = val_AsymRsaLoadKeyAssetPlaintext(&ModulusData,
                                              ctx->ModulusBits,
                                              &ExponentData,
                                              (ctx->PublicExponentBytes*8),
                                              AssetId,
                                              VAL_ASSETID_INVALID,
                                              NULL, 0, NULL, 0);
#ifndef SFZUTF_USERMODE
    SFZUTF_FREE(ExponentData.Data_p);
    SFZUTF_FREE(ModulusData.Data_p);
#endif
    if(Status!=VAL_SUCCESS)
    {
        MBEDTLS_LOGE("RsaPss Load Plaintext Fail:%d\r\n",Status);
    }
    else
    {
        MBEDTLS_LOGI("RsaPss Load Plaintext Success\r\n");
    }

    return Status;
}

/**
 * Helper function that performs the RSA PSS signature verify operation.
*/
static int mbedtls_rsa_pss_verify_hw(
        mbedtls_rsa_pss_context_t *ctx,
        ValAsymKey_t * PublicKey_p,
        ValAsymBigInt_t * Signature_p)
{
    ValStatus_t Status;
    uint8_t * Msg_p;

#ifdef SFZUTF_USERMODE
    Msg_p = sfzutf_discard_const(ctx->Msg_p);
#else
    Msg_p = SFZUTF_MALLOC(ctx->MsgBytes);
    if(Msg_p==NULL)
        return END_TEST_FAIL;
    memcpy(Msg_p, ctx->Msg_p, ctx->MsgBytes);

    MBEDTLS_LOGI("Pss Verify HashBits:%d HashAlg:%d\r\n",ctx->HashBits,PublicKey_p->HashAlgorithm);

#endif
    Status = val_AsymRsaPssVerify(PublicKey_p, Signature_p,
                                  NULL, ctx->MsgBytes, Msg_p,
                                  (ctx->HashBits/8));
#ifndef SFZUTF_USERMODE
    SFZUTF_FREE(Msg_p);
#endif
    if(Status!=VAL_SUCCESS)
    {
        MBEDTLS_LOGE("RsaPss Verify Fail:%d\r\n",Status);
    }
    else
    {
        MBEDTLS_LOGI("RsaPss Verify Success\r\n");
    }

    return Status;
}

#endif

#if defined(MBEDTLS_PKCS1_V21)
/*
 * Implementation of the PKCS#1 v2.1 RSASSA-PSS-SIGN function
 */

int __attribute__((weak)) mbedtls_rsa_rsassa_pss_sign( mbedtls_rsa_context *ctx,
                         int (*f_rng)(void *, unsigned char *, size_t),
                         void *p_rng,
                         int mode,
                         mbedtls_md_type_t md_alg,
                         unsigned int hashlen,
                         const unsigned char *hash,
                         unsigned char *sig )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    ValAsymKey_t PrivateKey;
    ValAsymBigInt_t Signature;
    PrivateKey.DomainAssetId = VAL_ASSETID_INVALID;
    PrivateKey.KeyAssetId = VAL_ASSETID_INVALID;

    RSA_VALIDATE_RET( ctx != NULL );
    RSA_VALIDATE_RET( mode == MBEDTLS_RSA_PRIVATE ||
                      mode == MBEDTLS_RSA_PUBLIC );
    RSA_VALIDATE_RET( ( md_alg  == MBEDTLS_MD_NONE &&
                        hashlen == 0 ) ||
                      hash != NULL );
    RSA_VALIDATE_RET( sig != NULL );

    mbedtls_rsa_pss_context_t *mbedtls_pss_context_data=SFZUTF_MALLOC(sizeof(mbedtls_rsa_pss_context_t));
    if(mbedtls_pss_context_data==NULL)
    {
        MBEDTLS_LOGE("%s alloc context fail\r\n");
        return END_TEST_FAIL;
    }
    mbedtls_pss_context_data->Modulus_p=NULL;
    mbedtls_pss_context_data->PublicExponent_p=NULL;
    mbedtls_pss_context_data->PrivateExponent_p=NULL;
    if( mode == MBEDTLS_RSA_PRIVATE && ctx->padding != MBEDTLS_RSA_PKCS_V21 )
        return( MBEDTLS_ERR_RSA_BAD_INPUT_DATA );

    if( f_rng == NULL )
        return( MBEDTLS_ERR_RSA_BAD_INPUT_DATA );
    if(ctx->len>MBEDTLS_HW_MAX_BYTES)
        return( MBEDTLS_ERR_RSA_BAD_INPUT_DATA );


    if(md_alg==MBEDTLS_MD_NONE)
        return( MBEDTLS_ERR_RSA_BAD_INPUT_DATA );
    else
    {
        if(md_alg==MBEDTLS_MD_SHA1)
            mbedtls_pss_context_data->HashBits=160;
        else if(md_alg==MBEDTLS_MD_SHA224)
            mbedtls_pss_context_data->HashBits=224;
        else if(md_alg==MBEDTLS_MD_SHA256)
            mbedtls_pss_context_data->HashBits=256;
        else if(md_alg==MBEDTLS_MD_SHA384)
            mbedtls_pss_context_data->HashBits=384;
        else if(md_alg==MBEDTLS_MD_SHA512)
            mbedtls_pss_context_data->HashBits=512;
        else
            mbedtls_pss_context_data->HashBits=160;

        mbedtls_pss_context_data->Msg_p=hash;
        mbedtls_pss_context_data->MsgBytes=mbedtls_pss_context_data->HashBits/8;
    }

    mbedtls_pss_context_data->ModulusBits=ctx->len*8;

    mbedtls_pss_context_data->ModulusBytes=ctx->len;
//=====================endian switch=================================//
    uint8_t *pContext=(uint8_t*)&ctx->N.p[0];
    mbedtls_pss_context_data->Modulus_p=SFZUTF_MALLOC(mbedtls_pss_context_data->ModulusBytes);
    if(mbedtls_pss_context_data->Modulus_p==NULL)
    {
        MBEDTLS_LOGE("malloc modulus fail\r\n");
        goto exit;
    }
    for(uint32_t i=0;i<mbedtls_pss_context_data->ModulusBytes;i++)
    {
        mbedtls_pss_context_data->Modulus_p[i]=pContext[mbedtls_pss_context_data->ModulusBytes-i-1];
    }

    mbedtls_pss_context_data->PublicExponentBytes=ctx->E.n*ciL;
    pContext=(uint8_t *)&ctx->E.p[0];
    mbedtls_pss_context_data->PublicExponent_p=SFZUTF_MALLOC(mbedtls_pss_context_data->PublicExponentBytes);
    if(mbedtls_pss_context_data->PublicExponent_p==NULL)
    {
        MBEDTLS_LOGE("malloc public fail\r\n");
        goto exit;
    }
    for(uint32_t i=0;i<mbedtls_pss_context_data->PublicExponentBytes;i++)
    {
        mbedtls_pss_context_data->PublicExponent_p[i]=pContext[mbedtls_pss_context_data->PublicExponentBytes-i-1];
    }

    mbedtls_pss_context_data->PrivateExponentBytes=ctx->D.n*ciL;
    pContext=(uint8_t *)&ctx->D.p[0];
    mbedtls_pss_context_data->PrivateExponent_p=SFZUTF_MALLOC(mbedtls_pss_context_data->PrivateExponentBytes);
    if(mbedtls_pss_context_data->PrivateExponent_p==NULL)
    {
        MBEDTLS_LOGE("malloc private fail\r\n");
        goto exit;
    }
    for(uint32_t i=0;i<mbedtls_pss_context_data->PrivateExponentBytes;i++)
    {
        mbedtls_pss_context_data->PrivateExponent_p[i]=pContext[mbedtls_pss_context_data->PrivateExponentBytes-i-1];
    }
//=============================================================================//
    

    mbedtls_pss_context_data->Signature_p=sig;
    mbedtls_pss_context_data->SignatureBytes=mbedtls_pss_context_data->ModulusBits/8;

MBEDTLS_LOGI("SIGN:Modulus:%d HashBits:%d PubBytes:%d PrvBytes:%d\r\n",
mbedtls_pss_context_data->ModulusBits,mbedtls_pss_context_data->HashBits,
mbedtls_pss_context_data->PublicExponentBytes,mbedtls_pss_context_data->PrivateExponentBytes);

    Signature.ByteDataSize=mbedtls_pss_context_data->SignatureBytes;
    Signature.Data_p=SFZUTF_MALLOC(Signature.ByteDataSize);
    if(Signature.Data_p==NULL)
    {
        MBEDTLS_LOGE("PSS sign alloc fail\r\n");
        goto exit;     
    }

    ret=mbedtls_load_rsa_pss_privkey_hw(mbedtls_pss_context_data,&PrivateKey);
    if(ret==END_TEST_SUCCES)
    {
        ret=mbedtls_rsa_pss_sign_hw(mbedtls_pss_context_data,&PrivateKey,&Signature);
        if(ret==END_TEST_SUCCES)
        {
            memcpy(sig,Signature.Data_p,Signature.ByteDataSize);
            mbedtls_pss_context_data->SignatureBytes=Signature.ByteDataSize;            
        }
    }
    mbedtls_rsa_release_key_hw(&PrivateKey);
exit:
    if(mbedtls_pss_context_data->PrivateExponent_p!=NULL)
        SFZUTF_FREE(mbedtls_pss_context_data->PrivateExponent_p);
    if(mbedtls_pss_context_data->PublicExponent_p!=NULL)
        SFZUTF_FREE(mbedtls_pss_context_data->PublicExponent_p);
    if(mbedtls_pss_context_data->Modulus_p!=NULL)
        SFZUTF_FREE(mbedtls_pss_context_data->Modulus_p);
    if(Signature.Data_p!=NULL)
        SFZUTF_FREE(Signature.Data_p);
    if(mbedtls_pss_context_data!=NULL)
        SFZUTF_FREE(mbedtls_pss_context_data);
    return ret;
}

#endif /* MBEDTLS_PKCS1_V21 */

#if defined(MBEDTLS_PKCS1_V15)
/*
 * Do an RSA operation to sign the message digest
 */
int __attribute__((weak)) mbedtls_rsa_rsassa_pkcs1_v15_sign( mbedtls_rsa_context *ctx,
                               int (*f_rng)(void *, unsigned char *, size_t),
                               void *p_rng,
                               int mode,
                               mbedtls_md_type_t md_alg,
                               unsigned int hashlen,
                               const unsigned char *hash,
                               unsigned char *sig )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    // unsigned char *sig_try = NULL, *verif = NULL;
    ValAsymKey_t PrivateKey;
    ValAsymBigInt_t Signature;
    PrivateKey.DomainAssetId = VAL_ASSETID_INVALID;
    PrivateKey.KeyAssetId = VAL_ASSETID_INVALID;


    RSA_VALIDATE_RET( ctx != NULL );
    RSA_VALIDATE_RET( mode == MBEDTLS_RSA_PRIVATE ||
                      mode == MBEDTLS_RSA_PUBLIC );
    RSA_VALIDATE_RET( ( md_alg  == MBEDTLS_MD_NONE &&
                        hashlen == 0 ) ||
                      hash != NULL );
    RSA_VALIDATE_RET( sig != NULL );

    if( mode == MBEDTLS_RSA_PRIVATE && ctx->padding != MBEDTLS_RSA_PKCS_V15 )
        return( MBEDTLS_ERR_RSA_BAD_INPUT_DATA );
    if(ctx->len>MBEDTLS_HW_MAX_BYTES)
        return( MBEDTLS_ERR_RSA_BAD_INPUT_DATA );
        
    mbedtls_rsa_pkcs1v15_context_t *mbedtls_rsa_pkcs1v15_context_data=SFZUTF_MALLOC(sizeof(mbedtls_rsa_pkcs1v15_context_t));
    if(mbedtls_rsa_pkcs1v15_context_data==NULL)
    {
        MBEDTLS_LOGE("%s alloc context fail\r\n");
        return END_TEST_FAIL;
    }
    mbedtls_rsa_pkcs1v15_context_data->Modulus_p=NULL;
    mbedtls_rsa_pkcs1v15_context_data->PublicExponent_p=NULL;
    mbedtls_rsa_pkcs1v15_context_data->PrivateExponent_p=NULL;
    if(md_alg==MBEDTLS_MD_SHA1)
        mbedtls_rsa_pkcs1v15_context_data->HashBits=160;
    else if(md_alg==MBEDTLS_MD_SHA224)
        mbedtls_rsa_pkcs1v15_context_data->HashBits=224;
    else if(md_alg==MBEDTLS_MD_SHA256)
        mbedtls_rsa_pkcs1v15_context_data->HashBits=256;
    else if(md_alg==MBEDTLS_MD_SHA384)
        mbedtls_rsa_pkcs1v15_context_data->HashBits=384;
    else if(md_alg==MBEDTLS_MD_SHA512)
        mbedtls_rsa_pkcs1v15_context_data->HashBits=512;
    else
        mbedtls_rsa_pkcs1v15_context_data->HashBits=160;
   

    mbedtls_rsa_pkcs1v15_context_data->ModulusBits=(ctx->len*8);
    mbedtls_rsa_pkcs1v15_context_data->ModulusBytes=ctx->len;

    mbedtls_rsa_pkcs1v15_context_data->Msg_p=hash;
    mbedtls_rsa_pkcs1v15_context_data->MsgBytes=mbedtls_rsa_pkcs1v15_context_data->HashBits/8;


    mbedtls_rsa_pkcs1v15_context_data->Signature_p=sig;
    mbedtls_rsa_pkcs1v15_context_data->SignatureBytes=ctx->len;
//==================================================================//
    uint8_t *pContext=(uint8_t*)&ctx->N.p[0];
    mbedtls_rsa_pkcs1v15_context_data->Modulus_p=SFZUTF_MALLOC(mbedtls_rsa_pkcs1v15_context_data->ModulusBytes);
    if(mbedtls_rsa_pkcs1v15_context_data->Modulus_p==NULL)
    {
        MBEDTLS_LOGE("malloc modulus fail\r\n");
        goto exit;
    }
    for(uint32_t i=0;i<mbedtls_rsa_pkcs1v15_context_data->ModulusBytes;i++)
    {
        mbedtls_rsa_pkcs1v15_context_data->Modulus_p[i]=pContext[mbedtls_rsa_pkcs1v15_context_data->ModulusBytes-i-1];
    }

    mbedtls_rsa_pkcs1v15_context_data->PublicExponentBytes=ctx->E.n*ciL;
    pContext=(uint8_t *)&ctx->E.p[0];
    mbedtls_rsa_pkcs1v15_context_data->PublicExponent_p=SFZUTF_MALLOC(mbedtls_rsa_pkcs1v15_context_data->PublicExponentBytes);
    if(mbedtls_rsa_pkcs1v15_context_data->PublicExponent_p==NULL)
    {
        MBEDTLS_LOGE("malloc public fail\r\n");
        goto exit;
    }
    for(uint32_t i=0;i<mbedtls_rsa_pkcs1v15_context_data->PublicExponentBytes;i++)
    {
        mbedtls_rsa_pkcs1v15_context_data->PublicExponent_p[i]=pContext[mbedtls_rsa_pkcs1v15_context_data->PublicExponentBytes-i-1];
    }

    mbedtls_rsa_pkcs1v15_context_data->PrivateExponentBytes=ctx->D.n*ciL;
    pContext=(uint8_t *)&ctx->D.p[0];
    mbedtls_rsa_pkcs1v15_context_data->PrivateExponent_p=SFZUTF_MALLOC(mbedtls_rsa_pkcs1v15_context_data->PrivateExponentBytes);
    if(mbedtls_rsa_pkcs1v15_context_data->PrivateExponent_p==NULL)
    {
        MBEDTLS_LOGE("malloc private fail\r\n");
        goto exit;
    }
    for(uint32_t i=0;i<mbedtls_rsa_pkcs1v15_context_data->PrivateExponentBytes;i++)
    {
        mbedtls_rsa_pkcs1v15_context_data->PrivateExponent_p[i]=pContext[mbedtls_rsa_pkcs1v15_context_data->PrivateExponentBytes-i-1];
    }
//===============================================================//  
    Signature.ByteDataSize= mbedtls_rsa_pkcs1v15_context_data->SignatureBytes;
    Signature.Data_p=SFZUTF_MALLOC(Signature.ByteDataSize);
    if(Signature.Data_p==NULL)
    {
        MBEDTLS_LOGE("PSS sign alloc fail\r\n");
        goto exit;     
    }
    ret=mbedtls_load_rsa_pkcs1v15_privKey_hw(mbedtls_rsa_pkcs1v15_context_data,&PrivateKey);
    MBEDTLS_LOGI("Signature Domain:%d Priv:%d Modulus:%d Hash:%d\r\n",\
    PrivateKey.DomainAssetId,PrivateKey.KeyAssetId,PrivateKey.ModulusSizeBits,PrivateKey.HashAlgorithm);
    if(ret==END_TEST_SUCCES)
    {
        ret=mbedtls_rsa_pkcs1v15_sign_hw(mbedtls_rsa_pkcs1v15_context_data,&PrivateKey,&Signature);
    }
    if(ret==VAL_SUCCESS)
    {
        MBEDTLS_LOGI("Signature Size:%d\r\n",Signature.ByteDataSize);
        memcpy((uint8_t*)sig, Signature.Data_p, Signature.ByteDataSize);
    }
    mbedtls_rsa_release_key_hw(&PrivateKey);
exit:
    if(mbedtls_rsa_pkcs1v15_context_data->PublicExponent_p!=NULL)
        SFZUTF_FREE(mbedtls_rsa_pkcs1v15_context_data->PublicExponent_p);
    if(mbedtls_rsa_pkcs1v15_context_data->PrivateExponent_p!=NULL)
        SFZUTF_FREE(mbedtls_rsa_pkcs1v15_context_data->PrivateExponent_p);
    if(mbedtls_rsa_pkcs1v15_context_data->Modulus_p!=NULL)
        SFZUTF_FREE(mbedtls_rsa_pkcs1v15_context_data->Modulus_p);
    if(Signature.Data_p!=NULL)
        SFZUTF_FREE(Signature.Data_p);
    if(mbedtls_rsa_pkcs1v15_context_data!=NULL)
        SFZUTF_FREE(mbedtls_rsa_pkcs1v15_context_data);
    return ret;
}
#endif /* MBEDTLS_PKCS1_V15 */
/*
 * Do an RSA operation to sign the message digest
 *[Warning]:
 *      hash:input :plaintext for hw
 *      hashlen:input:the length of the plaintext    for hw
 *      do not enter hashed value as hash parameter for hw
 */
int __attribute__((weak)) mbedtls_rsa_pkcs1_sign( mbedtls_rsa_context *ctx,
                    int (*f_rng)(void *, unsigned char *, size_t),
                    void *p_rng,
                    int mode,
                    mbedtls_md_type_t md_alg,
                    unsigned int hashlen,
                    const unsigned char *hash,
                    unsigned char *sig )
{
    RSA_VALIDATE_RET( ctx != NULL );
    RSA_VALIDATE_RET( mode == MBEDTLS_RSA_PRIVATE ||
                      mode == MBEDTLS_RSA_PUBLIC );
    RSA_VALIDATE_RET( ( md_alg  == MBEDTLS_MD_NONE &&
                        hashlen == 0 ) ||
                      hash != NULL );
    RSA_VALIDATE_RET( sig != NULL );

    ctx->hash_id =  md_alg;
    MBEDTLS_LOGI("Set hash_id the same md_alg\r\n");
    switch( ctx->padding )
    {
#if defined(MBEDTLS_PKCS1_V15)
        case MBEDTLS_RSA_PKCS_V15:
            return mbedtls_rsa_rsassa_pkcs1_v15_sign( ctx, f_rng, p_rng, mode, md_alg,
                                              hashlen, hash, sig );
#endif

#if defined(MBEDTLS_PKCS1_V21)
        case MBEDTLS_RSA_PKCS_V21:
            return mbedtls_rsa_rsassa_pss_sign( ctx, f_rng, p_rng, mode, md_alg,
                                        hashlen, hash, sig );
#endif

        default:
            return( MBEDTLS_ERR_RSA_INVALID_PADDING );
    }
}
#if defined(MBEDTLS_PKCS1_V21)
/*
 * Implementation of the PKCS#1 v2.1 RSASSA-PSS-VERIFY function
 * [Warning]:*hash:point to plain text
 *            hashlen:equal to plain length
 *            do the Hash with the hardware
 */
int __attribute__((weak)) mbedtls_rsa_rsassa_pss_verify_ext( mbedtls_rsa_context *ctx,
                               int (*f_rng)(void *, unsigned char *, size_t),
                               void *p_rng,
                               int mode,
                               mbedtls_md_type_t md_alg,
                               unsigned int hashlen,
                               const unsigned char *hash,
                               mbedtls_md_type_t mgf1_hash_id,
                               int expected_salt_len,
                               const unsigned char *sig )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    ValAsymKey_t PublicKey;
    if(ctx->len>MBEDTLS_HW_MAX_BYTES)
        return( MBEDTLS_ERR_RSA_BAD_INPUT_DATA );
    if( mode == MBEDTLS_RSA_PRIVATE && ctx->padding != MBEDTLS_RSA_PKCS_V21 )
        return( MBEDTLS_ERR_RSA_BAD_INPUT_DATA );
    
    mbedtls_rsa_pss_context_t *mbedtls_pss_context_data=SFZUTF_MALLOC(sizeof(mbedtls_rsa_pss_context_t));
    ValAsymBigInt_t Signature;

    PublicKey.DomainAssetId = VAL_ASSETID_INVALID;
    PublicKey.KeyAssetId = VAL_ASSETID_INVALID;


    if(mbedtls_pss_context_data==NULL)
    {
        MBEDTLS_LOGE("%s alloc context fail\r\n");
        return END_TEST_FAIL;
    }
    mbedtls_pss_context_data->Modulus_p=NULL;
    mbedtls_pss_context_data->PublicExponent_p=NULL;
    mbedtls_pss_context_data->PrivateExponent_p=NULL;
    
    if(md_alg==MBEDTLS_MD_NONE)
        return( MBEDTLS_ERR_RSA_BAD_INPUT_DATA );
    else
    {
        if(md_alg==MBEDTLS_MD_SHA1)
            mbedtls_pss_context_data->HashBits=160;
        else if(md_alg==MBEDTLS_MD_SHA224)
            mbedtls_pss_context_data->HashBits=224;
        else if(md_alg==MBEDTLS_MD_SHA256)
            mbedtls_pss_context_data->HashBits=256;
        else if(md_alg==MBEDTLS_MD_SHA384)
            mbedtls_pss_context_data->HashBits=384;
        else if(md_alg==MBEDTLS_MD_SHA512)
            mbedtls_pss_context_data->HashBits=512;
        else
            mbedtls_pss_context_data->HashBits=160;
    }
    mbedtls_pss_context_data->ModulusBits=ctx->len*8;

    mbedtls_pss_context_data->ModulusBytes=ctx->len;
//***************************do the end switch**************************//    
    uint8_t *pContext=(uint8_t*)&ctx->N.p[0];
    mbedtls_pss_context_data->Modulus_p=SFZUTF_MALLOC(mbedtls_pss_context_data->ModulusBytes);
    if(mbedtls_pss_context_data->Modulus_p==NULL)
    {
        MBEDTLS_LOGE("malloc modulus fail\r\n");
        goto exit;
    }
    for(uint32_t i=0;i<mbedtls_pss_context_data->ModulusBytes;i++)
    {
        mbedtls_pss_context_data->Modulus_p[i]=pContext[mbedtls_pss_context_data->ModulusBytes-i-1];
    }

    mbedtls_pss_context_data->PublicExponentBytes=ctx->E.n*ciL;
    pContext=(uint8_t *)&ctx->E.p[0];
    mbedtls_pss_context_data->PublicExponent_p=SFZUTF_MALLOC(mbedtls_pss_context_data->PublicExponentBytes);
    if(mbedtls_pss_context_data->PublicExponent_p==NULL)
    {
        MBEDTLS_LOGE("malloc public fail\r\n");
        goto exit;
    }
    for(uint32_t i=0;i<mbedtls_pss_context_data->PublicExponentBytes;i++)
    {
        mbedtls_pss_context_data->PublicExponent_p[i]=pContext[mbedtls_pss_context_data->PublicExponentBytes-i-1];
    }

    mbedtls_pss_context_data->PrivateExponentBytes=ctx->D.n*ciL;
    pContext=(uint8_t *)&ctx->D.p[0];
    mbedtls_pss_context_data->PrivateExponent_p=SFZUTF_MALLOC(mbedtls_pss_context_data->PrivateExponentBytes);
    if(mbedtls_pss_context_data->PrivateExponent_p==NULL)
    {
        MBEDTLS_LOGE("malloc private fail\r\n");
        goto exit;
    }
    for(uint32_t i=0;i<mbedtls_pss_context_data->PrivateExponentBytes;i++)
    {
        mbedtls_pss_context_data->PrivateExponent_p[i]=pContext[mbedtls_pss_context_data->PrivateExponentBytes-i-1];
    }
//****************************************************************************//
    mbedtls_pss_context_data->Msg_p=hash;
    mbedtls_pss_context_data->MsgBytes=mbedtls_pss_context_data->HashBits/8;

    mbedtls_pss_context_data->Signature_p=sig;
    mbedtls_pss_context_data->SignatureBytes=mbedtls_pss_context_data->ModulusBits/8;

    Signature.ByteDataSize=mbedtls_pss_context_data->SignatureBytes;
    Signature.Data_p=SFZUTF_MALLOC(Signature.ByteDataSize);
    if(Signature.Data_p==NULL)
    {
        MBEDTLS_LOGE("PSS verify alloc fail\r\n");
        goto exit;     
    }

    ret=mbedtls_load_rsa_pss_pubkey_hw(mbedtls_pss_context_data,&PublicKey);
    if(ret==END_TEST_SUCCES)
    {
        memcpy(Signature.Data_p,mbedtls_pss_context_data->Signature_p,mbedtls_pss_context_data->SignatureBytes);
        ret=mbedtls_rsa_pss_verify_hw(mbedtls_pss_context_data,&PublicKey,&Signature);
    }
    mbedtls_rsa_release_key_hw(&PublicKey);
exit:
    if(mbedtls_pss_context_data->Modulus_p!=NULL)
        SFZUTF_FREE(mbedtls_pss_context_data->Modulus_p);
    if(mbedtls_pss_context_data->PublicExponent_p!=NULL)
        SFZUTF_FREE(mbedtls_pss_context_data->PublicExponent_p);
    if(mbedtls_pss_context_data->PrivateExponent_p!=NULL)
        SFZUTF_FREE(mbedtls_pss_context_data->PrivateExponent_p);
    if(Signature.Data_p!=NULL)
        SFZUTF_FREE(Signature.Data_p);
    if(mbedtls_pss_context_data!=NULL)
        SFZUTF_FREE(mbedtls_pss_context_data);

    return ret;

}

/*
 * Simplified PKCS#1 v2.1 RSASSA-PSS-VERIFY function
 */
int __attribute__((weak)) mbedtls_rsa_rsassa_pss_verify( mbedtls_rsa_context *ctx,
                           int (*f_rng)(void *, unsigned char *, size_t),
                           void *p_rng,
                           int mode,
                           mbedtls_md_type_t md_alg,
                           unsigned int hashlen,
                           const unsigned char *hash,
                           const unsigned char *sig )
{
    mbedtls_md_type_t mgf1_hash_id;
    RSA_VALIDATE_RET( ctx != NULL );
    RSA_VALIDATE_RET( mode == MBEDTLS_RSA_PRIVATE ||
                      mode == MBEDTLS_RSA_PUBLIC );
    RSA_VALIDATE_RET( sig != NULL );
    RSA_VALIDATE_RET( ( md_alg  == MBEDTLS_MD_NONE &&
                        hashlen == 0 ) ||
                      hash != NULL );

    mgf1_hash_id = ( ctx->hash_id != MBEDTLS_MD_NONE )
                             ? (mbedtls_md_type_t) ctx->hash_id
                             : md_alg;

    return( mbedtls_rsa_rsassa_pss_verify_ext( ctx, f_rng, p_rng, mode,
                                       md_alg, hashlen, hash,
                                       mgf1_hash_id, MBEDTLS_RSA_SALT_LEN_ANY,
                                       sig ) );


}
#endif /* MBEDTLS_PKCS1_V21 */

#if defined(MBEDTLS_PKCS1_V15)
/*
 * Implementation of the PKCS#1 v2.1 RSASSA-PKCS1-v1_5-VERIFY function
 */
int __attribute__((weak)) mbedtls_rsa_rsassa_pkcs1_v15_verify(  mbedtls_rsa_context *ctx,
                                 int (*f_rng)(void *, unsigned char *, size_t),
                                 void *p_rng,
                                 int mode,
                                 mbedtls_md_type_t md_alg,
                                 unsigned int hashlen,
                                 const unsigned char *hash,
                                 const unsigned char *sig )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    ValAsymKey_t PublicKey;

    if(ctx->len>MBEDTLS_HW_MAX_BYTES)
        return( MBEDTLS_ERR_RSA_BAD_INPUT_DATA );
    
    mbedtls_rsa_pkcs1v15_context_t *mbedtls_pkcs1v15_context_data=SFZUTF_MALLOC(sizeof(mbedtls_rsa_pkcs1v15_context_t));
    ValAsymBigInt_t Signature;
    if(mbedtls_pkcs1v15_context_data==NULL)
    {
        MBEDTLS_LOGE("%s alloc context fail\r\n");
        return END_TEST_FAIL;
    }
    mbedtls_pkcs1v15_context_data->Modulus_p=NULL;
    mbedtls_pkcs1v15_context_data->PublicExponent_p=NULL;
    mbedtls_pkcs1v15_context_data->PrivateExponent_p=NULL;
    PublicKey.DomainAssetId = VAL_ASSETID_INVALID;
    PublicKey.KeyAssetId = VAL_ASSETID_INVALID;


    if(md_alg==MBEDTLS_MD_SHA1)
        mbedtls_pkcs1v15_context_data->HashBits=160;
    else if(md_alg==MBEDTLS_MD_SHA224)
        mbedtls_pkcs1v15_context_data->HashBits=224;
    else if(md_alg==MBEDTLS_MD_SHA256)
        mbedtls_pkcs1v15_context_data->HashBits=256;
    else if(md_alg==MBEDTLS_MD_SHA384)
        mbedtls_pkcs1v15_context_data->HashBits=384;
    else if(md_alg==MBEDTLS_MD_SHA512)
        mbedtls_pkcs1v15_context_data->HashBits=512;
    else
        mbedtls_pkcs1v15_context_data->HashBits=160;

//==================================================================//
    mbedtls_pkcs1v15_context_data->ModulusBits=ctx->len*8;

    mbedtls_pkcs1v15_context_data->ModulusBytes=ctx->len;
    uint8_t *pContext=(uint8_t*)&ctx->N.p[0];
    mbedtls_pkcs1v15_context_data->Modulus_p=SFZUTF_MALLOC(mbedtls_pkcs1v15_context_data->ModulusBytes);
    if(mbedtls_pkcs1v15_context_data->Modulus_p==NULL)
    {
        MBEDTLS_LOGE("malloc modulus fail\r\n");
        goto exit;
    }
    for(uint32_t i=0;i<mbedtls_pkcs1v15_context_data->ModulusBytes;i++)
    {
        mbedtls_pkcs1v15_context_data->Modulus_p[i]=pContext[mbedtls_pkcs1v15_context_data->ModulusBytes-i-1];
    }

    mbedtls_pkcs1v15_context_data->PublicExponentBytes=ctx->E.n*ciL;
    pContext=(uint8_t *)&ctx->E.p[0];
    mbedtls_pkcs1v15_context_data->PublicExponent_p=SFZUTF_MALLOC(mbedtls_pkcs1v15_context_data->PublicExponentBytes);
    if(mbedtls_pkcs1v15_context_data->PublicExponent_p==NULL)
    {
        MBEDTLS_LOGE("malloc public fail\r\n");
        goto exit;
    }
    for(uint32_t i=0;i<mbedtls_pkcs1v15_context_data->PublicExponentBytes;i++)
    {
        mbedtls_pkcs1v15_context_data->PublicExponent_p[i]=pContext[mbedtls_pkcs1v15_context_data->PublicExponentBytes-i-1];
    }

    mbedtls_pkcs1v15_context_data->PrivateExponentBytes=ctx->D.n*ciL;
    pContext=(uint8_t *)&ctx->D.p[0];
    mbedtls_pkcs1v15_context_data->PrivateExponent_p=SFZUTF_MALLOC(mbedtls_pkcs1v15_context_data->PrivateExponentBytes);
    if(mbedtls_pkcs1v15_context_data->PrivateExponent_p==NULL)
    {
        MBEDTLS_LOGE("malloc private fail\r\n");
        goto exit;
    }
    for(uint32_t i=0;i<mbedtls_pkcs1v15_context_data->PrivateExponentBytes;i++)
    {
        mbedtls_pkcs1v15_context_data->PrivateExponent_p[i]=pContext[mbedtls_pkcs1v15_context_data->PrivateExponentBytes-i-1];
    }
//===============================================================//  

    mbedtls_pkcs1v15_context_data->Msg_p=hash;
    mbedtls_pkcs1v15_context_data->MsgBytes=mbedtls_pkcs1v15_context_data->HashBits/8;

    mbedtls_pkcs1v15_context_data->Signature_p=sig;
    mbedtls_pkcs1v15_context_data->SignatureBytes=ctx->len;

    Signature.ByteDataSize=mbedtls_pkcs1v15_context_data->SignatureBytes;
    Signature.Data_p=SFZUTF_MALLOC(Signature.ByteDataSize);
    if(Signature.Data_p==NULL)
    {
        MBEDTLS_LOGE("PSS verify alloc fail\r\n");
        goto exit;     
    }

    ret = mbedtls_load_rsa_pkcs1v15_pubkey_hw(mbedtls_pkcs1v15_context_data, &PublicKey);
    if(ret==END_TEST_SUCCES)
    {
        MBEDTLS_LOGI("Pubkey Domain:%d PrivKey:%d ModulusBits:%d Sha:%d\r\n",\
        PublicKey.DomainAssetId,PublicKey.KeyAssetId,PublicKey.ModulusSizeBits,PublicKey.HashAlgorithm);
        memcpy(Signature.Data_p,mbedtls_pkcs1v15_context_data->Signature_p,mbedtls_pkcs1v15_context_data->SignatureBytes);

        ret=mbedtls_rsa_pkcs1v15_verify_hw(mbedtls_pkcs1v15_context_data,&PublicKey,&Signature);
    }
    mbedtls_rsa_release_key_hw(&PublicKey);
exit:
    if(mbedtls_pkcs1v15_context_data->Modulus_p!=NULL)
    {
        SFZUTF_FREE(mbedtls_pkcs1v15_context_data->Modulus_p);
    }
    if(mbedtls_pkcs1v15_context_data->PublicExponent_p!=NULL)
    {
        SFZUTF_FREE(mbedtls_pkcs1v15_context_data->PublicExponent_p);
    }
    if(mbedtls_pkcs1v15_context_data->PrivateExponent_p!=NULL)
    {
        SFZUTF_FREE(mbedtls_pkcs1v15_context_data->PrivateExponent_p);
    }
    if(Signature.Data_p!=NULL)
        SFZUTF_FREE(Signature.Data_p);
    if(mbedtls_pkcs1v15_context_data!=NULL)
        SFZUTF_FREE(mbedtls_pkcs1v15_context_data);

    return ret;
}
#endif /* MBEDTLS_PKCS1_V15 */

/*
 * Do an RSA operation and check the message digest
 * [Warning]:
 * hash: plaintext when use hardware 
 * haslen:plaintext length when use hardware
 */
int __attribute__((weak)) mbedtls_rsa_pkcs1_verify( mbedtls_rsa_context *ctx,
                      int (*f_rng)(void *, unsigned char *, size_t),
                      void *p_rng,
                      int mode,
                      mbedtls_md_type_t md_alg,
                      unsigned int hashlen,
                      const unsigned char *hash,
                      const unsigned char *sig )
{
    RSA_VALIDATE_RET( ctx != NULL );
    RSA_VALIDATE_RET( mode == MBEDTLS_RSA_PRIVATE ||
                      mode == MBEDTLS_RSA_PUBLIC );
    RSA_VALIDATE_RET( sig != NULL );
    RSA_VALIDATE_RET( ( md_alg  == MBEDTLS_MD_NONE &&
                        hashlen == 0 ) ||
                      hash != NULL );

    switch( ctx->padding )
    {
#if defined(MBEDTLS_PKCS1_V15)
        case MBEDTLS_RSA_PKCS_V15:
            return mbedtls_rsa_rsassa_pkcs1_v15_verify( ctx, f_rng, p_rng, mode, md_alg,
                                                hashlen, hash, sig );
#endif

#if defined(MBEDTLS_PKCS1_V21)
        case MBEDTLS_RSA_PKCS_V21:
            return mbedtls_rsa_rsassa_pss_verify( ctx, f_rng, p_rng, mode, md_alg,
                                          hashlen, hash, sig );
#endif

        default:
            return( MBEDTLS_ERR_RSA_INVALID_PADDING );
    }
}
/*
 * Copy the components of an RSA key
 */
int __attribute__((weak)) mbedtls_rsa_copy( mbedtls_rsa_context *dst, const mbedtls_rsa_context *src )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    RSA_VALIDATE_RET( dst != NULL );
    RSA_VALIDATE_RET( src != NULL );

    dst->ver = src->ver;
    dst->len = src->len;

    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &dst->N, &src->N ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &dst->E, &src->E ) );

    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &dst->D, &src->D ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &dst->P, &src->P ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &dst->Q, &src->Q ) );
    dst->padding = src->padding;
    dst->hash_id = src->hash_id;

cleanup:
    if( ret != 0 )
        mbedtls_rsa_free( dst );

    return( ret );
}

/*
 * Free the components of an RSA key
 */
void __attribute__((weak)) mbedtls_rsa_free( mbedtls_rsa_context *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_mpi_free( &ctx->Vi );
    mbedtls_mpi_free( &ctx->Vf );
    mbedtls_mpi_free( &ctx->RN );
    mbedtls_mpi_free( &ctx->D  );
    mbedtls_mpi_free( &ctx->Q  );
    mbedtls_mpi_free( &ctx->P  );
    mbedtls_mpi_free( &ctx->E  );
    mbedtls_mpi_free( &ctx->N  );

#if !defined(MBEDTLS_RSA_NO_CRT)
    mbedtls_mpi_free( &ctx->RQ );
    mbedtls_mpi_free( &ctx->RP );
    mbedtls_mpi_free( &ctx->QP );
    mbedtls_mpi_free( &ctx->DQ );
    mbedtls_mpi_free( &ctx->DP );
#endif /* MBEDTLS_RSA_NO_CRT */

#if defined(MBEDTLS_THREADING_C)
    mbedtls_mutex_free( &ctx->mutex );
#endif
}

#endif//MBEDTLS_RSA_ALT
#endif//MBEDTLS_RSA_C