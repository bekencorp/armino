// Copyright 2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined MBEDTLS_ECDH_COMPUTE_SHARED_ALT || defined MBEDTLS_ECDH_GEN_PUBLIC_ALT
#include <modules/bk_mbedtls_alt/bk_ecdh_alt.h>
#include "api_val.h"
#include "adapter_val_internal.h"

#include "securityip_driver.h"
#include "blt_log.h"
/*----------------------------------------------------------------------------
 *IsTrngActive
 *
 * This function checks if TRNG has been activated and on request it tries
 * to activate the TRNG if it was not activated.
 */
static bool val_Is_trng_active(
        const bool activate)
{
    ValStatus_t status;
    uint8_t * buffer_p;
	uint16_t trng_sample_cycles = 3072;

    buffer_p = malloc(20);
    if (buffer_p == NULL)
    {
        // Assume TRNG is not active
        return false;
    }

    // Get a random number
    status = val_RandomData(16, buffer_p);
    free(buffer_p);

    if ((status != VAL_SUCCESS) && activate )
    {
        status = val_TrngConfig(0, trng_sample_cycles, 1, 2, 0, 1);;
    }

    return (status == VAL_SUCCESS);
}

#define assert_exit(cond, ret) \
    do { if (!(cond)) { \
        MBEDTLS_LOGE("  !. assert: failed [line: %d, error: -0x%04X]\n", __LINE__, -ret); \
    } } while (0)

static inline int mpi_to_big_int(ValAsymBigInt_t* x, const mbedtls_mpi* y)
{
    if(x == NULL || y == NULL){
        return -1;
    }
    x->ByteDataSize = sizeof(mbedtls_mpi_uint) * (y->n);
    if(x->Data_p == NULL)
        x->Data_p = malloc(x->ByteDataSize);
    memcpy(x->Data_p, y->p, x->ByteDataSize);

    return 0;
}

static inline int big_int_to_mpi(mbedtls_mpi* x, const ValAsymBigInt_t* y)
{
    if(x == NULL || y == NULL){
        return -1;
    }
    x->n = (y->ByteDataSize)/sizeof(mbedtls_mpi_uint);
    if(x->p == NULL)
        x->p = malloc(y->ByteDataSize);
    memcpy(x->p, y->Data_p, y->ByteDataSize);

    return 0;
}

static int free_big_int(ValAsymBigInt_t* x)
{
    if(x == NULL){
        return -1;
    }
    if(x != NULL && x->Data_p != NULL){
        free(x->Data_p);
        x->Data_p = NULL;
    }
    return 0;    
}

extern TestVector_ECC_Curve_t ECC_Curve_NIST_P384;

int __attribute__((weak)) mbedtls_ecdh_gen_public( mbedtls_ecp_group *grp, mbedtls_mpi *d, mbedtls_ecp_point *Q,
                     int (*f_rng)(void *, unsigned char *, size_t),
                     void *p_rng )
{
    ValSize_t ModulusSizeBits;
    ValAsymECDomainParam_t Domain;
    ValAssetId_t DomainAssetId = VAL_ASSETID_INVALID;
    ValAssetId_t PrivateKeyAssetId = VAL_ASSETID_INVALID;
    ValAssetId_t PublicKeyAssetId = VAL_ASSETID_INVALID;
    ValAsymKey_t PrivateKey, PublicKey;

    int ret = 0;
    ValStatus_t Status;
    
    ValAsymECCPoint_t * PubKeyPoint_p;
    ValOctetsOut_Optional_t * PrivKeyBlob_p;

    ValSize_t plen = 0;
    ValAsymBigInt_t pri_key;

    bool trng = val_Is_trng_active(true);
    if(!trng){
        MBEDTLS_LOGE(">>trng errror\r\n");
    }

    MBEDTLS_LOGE(">>>>size of mpi:%d\r\n", sizeof(mbedtls_mpi_uint));

#if 1
    TestVector_ECC_Curve_Rec_t * Curve_p = NULL;

    switch (grp->id)
    {
    case MBEDTLS_ECP_DP_SECP384R1:
        Curve_p = sfzutf_discard_const(ECC_Curve_NIST_P384);
        break;
    
    default:
        break;
    }

    Domain.ModulusSizeBits          = Curve_p->CurveBits;
    Domain.Modulus.Data_p           = sfzutf_discard_const(Curve_p->P_p);
    Domain.Modulus.ByteDataSize     = Curve_p->PLen;
    Domain.a.Data_p                 = sfzutf_discard_const(Curve_p->A_p);
    Domain.a.ByteDataSize           = Curve_p->ALen;
    Domain.b.Data_p                 = sfzutf_discard_const(Curve_p->B_p);
    Domain.b.ByteDataSize           = Curve_p->BLen;
    Domain.Order.Data_p             = sfzutf_discard_const(Curve_p->Order_p);
    Domain.Order.ByteDataSize       = Curve_p->OrderLen;
    Domain.BasePoint.x.Data_p       = sfzutf_discard_const(Curve_p->ECPointX_p);
    Domain.BasePoint.x.ByteDataSize = Curve_p->ECPointXLen;
    Domain.BasePoint.y.Data_p       = sfzutf_discard_const(Curve_p->ECPointY_p);
    Domain.BasePoint.y.ByteDataSize = Curve_p->ECPointYLen;
    Domain.CoFactor                 = Curve_p->Cofactor;
#endif

#if 0
    Domain.ModulusSizeBits = grp->pbits;
    Domain.Modulus = mpi_to_big_int(grp->P);
    Domain.a = mpi_to_big_int(grp->A);
    Domain.b = mpi_to_big_int(grp->B);
    Domain.Order = mpi_to_big_int(grp->N);
    Domain.BasePoint.x = mpi_to_big_int(grp->G.X);
    Domain.BasePoint.y = mpi_to_big_int(grp->G.Y);  //warning: Z coordinate is not supported
    Domain.CoFactor = 1;

    printf("ModulusSizeBits:%d\r\n", Domain.ModulusSizeBits);
    printf("Modulus.ByteDataSize:%d\r\n", Domain.Modulus.ByteDataSize);
    printf("Domain.a.ByteDataSize:%d\r\n", Domain.a.ByteDataSize);
    printf("Domain.b.ByteDataSize:%d\r\n", Domain.b.ByteDataSize);
    printf("Domain.Order.ByteDataSize:%d\r\n", Domain.Order.ByteDataSize);
    printf("Domain.BasePoint.x.ByteDataSize:%d\r\n", Domain.BasePoint.x.ByteDataSize);
    printf("Domain.BasePoint.y.ByteDataSize:%d\r\n", Domain.BasePoint.y.ByteDataSize);
    printf("Domain.CoFactor:%d\r\n", Domain.CoFactor);    
#endif

    ModulusSizeBits = Domain.ModulusSizeBits;
    plen = ModulusSizeBits / 8U;

    ValSize_t PrivKeyBlobSize = 68;
    PrivKeyBlob_p = (ValOctetsOut_Optional_t *)malloc(PrivKeyBlobSize);
    PubKeyPoint_p = (ValAsymECCPoint_t *)calloc(1, sizeof(ValAsymECCPoint_t));

    PubKeyPoint_p->x.ByteDataSize = plen;
    PubKeyPoint_p->y.ByteDataSize = plen;
    PubKeyPoint_p->x.Data_p = malloc(plen);
    PubKeyPoint_p->y.Data_p = malloc(plen);

    Status = val_AsymEccAllocDomainAsset(ModulusSizeBits, &DomainAssetId);
    if(Status != VAL_SUCCESS){
        MBEDTLS_LOGE("val_AsymEccAllocDomainAsset failed, Status:%d, DomainAssetId:%d\r\n", Status, DomainAssetId);
        ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
        goto cleanup;
    }

    Status = val_AsymEccLoadDomainAssetPlaintext(&Domain, 
                                             DomainAssetId,
                                             VAL_ASSETID_INVALID,
                                             NULL, 0, NULL, NULL);
    if(Status != VAL_SUCCESS){

        MBEDTLS_LOGE("val_AsymEccLoadDomainAssetPlaintext failed, %d\r\n", Status);
        ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
        goto cleanup;
    }

    Status = val_AsymEcdhAllocPrivateKeyAsset(ModulusSizeBits, false, false, &PrivateKeyAssetId);
    
    if(Status != VAL_SUCCESS){
        MBEDTLS_LOGE("val_AsymEcdhAllocPrivateKeyAsset failed, %d\r\n", Status);
        ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
        goto cleanup;
    }

    Status = val_AsymInitKey(PrivateKeyAssetId, DomainAssetId,
                             ModulusSizeBits, VAL_SYM_ALGO_NONE,
                             &PrivateKey);
    if(Status != VAL_SUCCESS){
        MBEDTLS_LOGE("val_AsymInitKey failed, %d\r\n", Status);
        ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
        goto cleanup;
    }

    Status = val_AsymEcdhAllocPublicKeyAsset(ModulusSizeBits, false, false, &PublicKeyAssetId);
    
    if(Status != VAL_SUCCESS){
        MBEDTLS_LOGE("val_AsymEcdhAllocPublicKeyAsset failed, %d\r\n", Status);
        ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
        goto cleanup;
    }

    Status = val_AsymInitKey(PublicKeyAssetId, DomainAssetId,
                             ModulusSizeBits, VAL_SYM_ALGO_NONE,
                             &PublicKey);
    if(Status != VAL_SUCCESS){
        MBEDTLS_LOGE("val_AsymInitKey failed, %d\r\n", Status);
        ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
        goto cleanup;
    }

    if(f_rng == NULL){
            /* - Generate a key pair */
        Status = val_AsymEccGenKeyPair(&PublicKey, &PrivateKey,
                                   VAL_ASSETID_INVALID, NULL, 0, 
                                   PubKeyPoint_p, PrivKeyBlob_p, &PrivKeyBlobSize);
    
        // Status = val_AsymEccGenKeyPair(&PublicKey, &PrivateKey,
        //                                VAL_ASSETID_INVALID, NULL, 0, 
        //                                NULL, NULL, NULL);
        if(Status != VAL_SUCCESS){
            MBEDTLS_LOGE("val_AsymEccGenKeyPair failed, %d\r\n", Status);
            ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
            goto cleanup;
        }
        MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(d, (const unsigned char *)PrivKeyBlob_p, plen));        
    }else{
        pri_key.ByteDataSize = plen;
        pri_key.Data_p = malloc(plen);
        if(0 != f_rng(p_rng, ( uint8_t *)pri_key.Data_p, pri_key.ByteDataSize)){
            ret = MBEDTLS_ERR_ECP_RANDOM_FAILED;
            goto cleanup;
        }
        Status = val_AsymEccLoadPrivateKeyAssetPlaintext(&pri_key,
                                                     Domain.ModulusSizeBits,
                                                     PrivateKeyAssetId,
                                                     VAL_ASSETID_INVALID, 
                                                     NULL, 0, NULL, NULL);
        if(Status != VAL_SUCCESS){
            MBEDTLS_LOGE("%s:%d failed\r\n", __func__, __LINE__);
            goto cleanup;
        }

        Status = val_AsymEccGenPublicKey(&PublicKey, &PrivateKey, PubKeyPoint_p);
        if(Status != VAL_SUCCESS){
            MBEDTLS_LOGE("%s:%d failed\r\n", __func__, __LINE__);
            goto cleanup;
        }
        MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(d, (const unsigned char *)pri_key.Data_p, plen));
    }

    //check keypair
    Status = val_AsymEccKeyCheck(&PublicKey, &PrivateKey);
    if(Status != VAL_SUCCESS){
        MBEDTLS_LOGE("%s:%d failed\r\n", __func__, __LINE__);
        goto cleanup;
    }
    ret = 0;

    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&(Q->X), (const unsigned char *)PubKeyPoint_p->x.Data_p, plen));    
    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&(Q->Y), (const unsigned char *)PubKeyPoint_p->y.Data_p, plen));

#if 0
    int rct;
    rct = mbedtls_ecp_check_privkey(grp, d);
    if(rct != 0){
        printf(">>>%s:%d,prikey check failed, rct:%d\r\n", __func__, __LINE__, rct);
    }

    rct = mbedtls_ecp_check_pubkey(grp, Q);
    if(rct != 0){
        printf(">>>%s:%d,pubkey check failed, rct:%d\r\n", __func__, __LINE__, rct);
    }
#endif

cleanup:
    free(PrivKeyBlob_p);
    free(PubKeyPoint_p);
    val_AssetFree(DomainAssetId);
    val_AssetFree(PublicKeyAssetId);
    val_AssetFree(PrivateKeyAssetId);

    free_big_int(&pri_key);

    return ret;
}

#if 1
int __attribute__((weak)) mbedtls_ecdh_compute_shared( mbedtls_ecp_group *grp, mbedtls_mpi *z,
                         const mbedtls_ecp_point *Q, const mbedtls_mpi *d,
                         int (*f_rng)(void *, unsigned char *, size_t),
                         void *p_rng )
{
    int ret = 0;
    ValStatus_t Status;
    ValAsymECDomainParam_t Domain;
    ValAssetId_t DomainAssetId = VAL_ASSETID_INVALID;
    ValAssetId_t PrivateKeyAssetId = VAL_ASSETID_INVALID;
    ValAssetId_t PublicKeyAssetId = VAL_ASSETID_INVALID;
    ValAsymKey_t PrivateKey, PublicKey;
    ValPolicyMask_t AssetPolicy;
    TestVector_ECC_Curve_Rec_t * Curve_p = NULL;

     int ListAssets = 2;
    ValAssetId_t AssetList[2];

#if 0
    int rct = mbedtls_ecp_check_pubkey(grp, Q);
    if(rct != 0){
        printf(">>>%s:%d,pubkey check failed, rct:%d\r\n", __func__, __LINE__, rct);
    }

    rct = mbedtls_ecp_check_privkey(grp, d);
    if(rct != 0){
        printf(">>>%s:%d,prikey check failed, rct:%d\r\n", __func__, __LINE__, rct);
    }
#endif

    switch (grp->id)
    {
    case MBEDTLS_ECP_DP_SECP384R1:
        Curve_p = sfzutf_discard_const(ECC_Curve_NIST_P384);
        break;
    
    default:
        break;
    }

    Domain.ModulusSizeBits          = Curve_p->CurveBits;
    Domain.Modulus.Data_p           = sfzutf_discard_const(Curve_p->P_p);
    Domain.Modulus.ByteDataSize     = Curve_p->PLen;
    Domain.a.Data_p                 = sfzutf_discard_const(Curve_p->A_p);
    Domain.a.ByteDataSize           = Curve_p->ALen;
    Domain.b.Data_p                 = sfzutf_discard_const(Curve_p->B_p);
    Domain.b.ByteDataSize           = Curve_p->BLen;
    Domain.Order.Data_p             = sfzutf_discard_const(Curve_p->Order_p);
    Domain.Order.ByteDataSize       = Curve_p->OrderLen;
    Domain.BasePoint.x.Data_p       = sfzutf_discard_const(Curve_p->ECPointX_p);
    Domain.BasePoint.x.ByteDataSize = Curve_p->ECPointXLen;
    Domain.BasePoint.y.Data_p       = sfzutf_discard_const(Curve_p->ECPointY_p);
    Domain.BasePoint.y.ByteDataSize = Curve_p->ECPointYLen;
    Domain.CoFactor                 = Curve_p->Cofactor;

    uint32_t plen = Domain.ModulusSizeBits / 8;
#if 0
    Domain.ModulusSizeBits = grp->pbits;
    Domain.Modulus = mpi_to_big_int(grp->P);
    Domain.a = mpi_to_big_int(grp->A);
    Domain.b = mpi_to_big_int(grp->B);
    Domain.Order = mpi_to_big_int(grp->N);
    Domain.BasePoint.x = mpi_to_big_int(grp->G.X);
    Domain.BasePoint.y = mpi_to_big_int(grp->G.Y);  //warning: Z coordinate is not supported
    Domain.CoFactor = 1;

    ModulusSizeBits = grp->pbits;
    uint32_t plen = (grp->pbits) / 8U;

    PubKeyPoint_p = (ValAsymECCPoint_t *)mbedtls_calloc(1, (plen*2U));
    PrivKeyBlob_p = (ValOctetsOut_Optional_t *)mbedtls_calloc(1, plen);

    if(NULL == PubKeyPoint_p || NULL == PrivKeyBlob_p){
        ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
        goto cleanup;
    }
#endif

    Status = val_AsymEccAllocDomainAsset(Domain.ModulusSizeBits, &DomainAssetId);
    if(Status != VAL_SUCCESS){
        MBEDTLS_LOGE("val_AsymEccAllocDomainAsset failed, Status:%d\r\n", Status);
        ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
        goto cleanup;
    }

    Status = val_AsymEccLoadDomainAssetPlaintext(&Domain, 
                                             DomainAssetId,
                                             VAL_ASSETID_INVALID,
                                             NULL, 0, NULL, NULL);
    if(Status != VAL_SUCCESS){
        MBEDTLS_LOGE("val_AsymEccLoadDomainAssetPlaintext failed\r\n");
        ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
        goto cleanup;
    }

    Status = val_AsymEcdhAllocPrivateKeyAsset(Domain.ModulusSizeBits, false, false, &PrivateKeyAssetId);
    
    if(Status != VAL_SUCCESS){
        MBEDTLS_LOGE("val_AsymEcdhAllocPrivateKeyAsset failed, Status:%d\r\n", Status);
        ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
        goto cleanup;
    }

    Status = val_AsymInitKey(PrivateKeyAssetId, DomainAssetId,
                             Domain.ModulusSizeBits, VAL_SYM_ALGO_NONE,
                             &PrivateKey);
    if(Status != VAL_SUCCESS){
        MBEDTLS_LOGE("val_AsymInitKey failed, Status:%d\r\n", Status);
        ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
        goto cleanup;
    }

    //load our private key
    ValAsymBigInt_t pri_key;
    pri_key.ByteDataSize = plen;
    pri_key.Data_p = malloc(plen);

    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(d, pri_key.Data_p, (d->n)*4));

    // for(int l=0; l<plen; l++)
    //     printf("%02x ", *(pri_key.Data_p + l));

    Status = val_AsymEccLoadPrivateKeyAssetPlaintext(&pri_key,
                                                     Domain.ModulusSizeBits,
                                                     PrivateKeyAssetId,
                                                     VAL_ASSETID_INVALID, 
                                                     NULL, 0, NULL, NULL);

    Status = val_AsymEcdhAllocPublicKeyAsset(Domain.ModulusSizeBits, false, false, &PublicKeyAssetId);
    
    if(Status != VAL_SUCCESS){
        MBEDTLS_LOGE("val_AsymEcdhAllocPublicKeyAsset failed, Status:%d\r\n", Status);
        ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
        goto cleanup;
    }

    Status = val_AsymInitKey(PublicKeyAssetId, DomainAssetId,
                             Domain.ModulusSizeBits, VAL_SYM_ALGO_NONE,
                             &PublicKey);
    if(Status != VAL_SUCCESS){
        MBEDTLS_LOGE("val_AsymInitKey failed, Status:%d\r\n", Status);
        ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
        goto cleanup;
    }

    //load their public key
    // ValAsymECCPoint_t KeyPoint;
    // mpi_to_big_int(&(KeyPoint.x), &(Q->X));
    // mpi_to_big_int(&(KeyPoint.y), &(Q->Y));

    ValAsymECCPoint_t KeyPoint;
    KeyPoint.x.ByteDataSize = plen;
    KeyPoint.x.Data_p = malloc(plen);
    KeyPoint.y.ByteDataSize = plen;
    KeyPoint.y.Data_p = malloc(plen);
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&(Q->X), KeyPoint.x.Data_p, plen));
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&(Q->Y), KeyPoint.y.Data_p, plen));

    Status = val_AsymEccLoadPublicKeyAssetPlaintext(&KeyPoint,
                                                    Domain.ModulusSizeBits,
                                                    PublicKeyAssetId,
                                                    VAL_ASSETID_INVALID, 
                                                    NULL, 0, NULL, NULL);
    if(Status != VAL_SUCCESS){
        MBEDTLS_LOGE("val_AsymEccLoadPublicKeyAssetPlaintext failed, Status:%d, PrivateKeyAssetId:%d\r\n", Status, PrivateKeyAssetId);
        ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
        goto cleanup;
    }

    //check keypair
    Status = val_AsymEccKeyCheck(&PublicKey, NULL);
    if(Status != VAL_SUCCESS){
        MBEDTLS_LOGE("%s:%d failed\r\n", __func__, __LINE__);
        goto cleanup;
    }

    AssetPolicy = VAL_POLICY_ALGO_CIPHER_AES|VAL_POLICY_MODE2|VAL_POLICY_ENCRYPT|VAL_POLICY_DECRYPT;
    if (!val_IsAccessSecure())
    {
        AssetPolicy |= VAL_POLICY_SOURCE_NON_SECURE;
    }
    Status = val_AssetAlloc(AssetPolicy, 32,
                            false, false,
                            VAL_ASSET_LIFETIME_INFINITE, false, false, 0,
                            &AssetList[0]);
    if(Status != VAL_SUCCESS){
        MBEDTLS_LOGE("%s:%d failed, Status:%d\r\n", __func__, __LINE__, Status);
        ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
        goto cleanup;
    }

    AssetPolicy = VAL_POLICY_MAC_GENERATE|VAL_POLICY_SHA256;
    // AssetPolicy = VAL_POLICY_PUBLIC_DATA;
    if (!val_IsAccessSecure())
    {
        AssetPolicy |= VAL_POLICY_SOURCE_NON_SECURE;
    }
    Status = val_AssetAlloc(AssetPolicy, 32,
                            false, false,
                            VAL_ASSET_LIFETIME_INFINITE, false, false, 0,
                            &AssetList[1]);
    if(Status != VAL_SUCCESS){
        MBEDTLS_LOGE("%s:%d failed, Status:%d\r\n", __func__, __LINE__, Status);
        ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
        goto cleanup;
    }    
    Status = val_AsymEcdhGenSharedSecretSingle(&PublicKey, &PrivateKey, false, NULL, 0,AssetList, (ValSize_t)ListAssets);
    if(Status != VAL_SUCCESS){
        MBEDTLS_LOGE("%s:%d failed, Status:%d\r\n", __func__, __LINE__, Status);
        ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
        goto cleanup;
    }

cleanup:
    val_AssetFree(DomainAssetId);
    val_AssetFree(PublicKeyAssetId);
    val_AssetFree(PrivateKeyAssetId);
    free_big_int(&pri_key);
    free_big_int(&(KeyPoint.x));
    free_big_int(&(KeyPoint.y));
    return ret;
}

#endif

// #endif /* MBEDTLS_ECDH_COMPUTE_SHARED_ALT || MBEDTLS_ECDH_GEN_PUBLIC_ALT */

int bk_mbedtls_rand( void *rng_state,
            unsigned char *output,
            size_t len )
{
    ValStatus_t status;

    if (output == NULL)
    {
        // Assume TRNG is not active
        return (-1);
    }

    // Get a random number
    status = val_RandomData(len, output);
    if(status != VAL_SUCCESS){
        return (-1);
    }
    
    // printf("\r\n----random----\r\n");
    // for(int i=0; i<len; i++)
    //     printf("%02x ", *(output + i));
    // printf("\r\n----random----\r\n");

    return( 0 );
}

int ecdh_test(void)
{
    int ret;
    mbedtls_ecp_group grp;
    mbedtls_ecp_point qA, qB;
    mbedtls_mpi dA, dB, zA, zB;

    mbedtls_ecp_group_init(&grp);

    mbedtls_ecp_point_init(&qA);
    mbedtls_ecp_point_init(&qB);

    mbedtls_mpi_init(&dA);
    mbedtls_mpi_init(&dB);

    mbedtls_mpi_init(&zA);
    mbedtls_mpi_init(&zB);

    mbedtls_ecp_group_load(&grp, MBEDTLS_ECP_DP_SECP384R1);

    ret = mbedtls_ecdh_gen_public(&grp, &dA, &qA, 
                                  &bk_mbedtls_rand,
                                  NULL);
    // ret = mbedtls_ecdh_gen_public(&grp, &dA, &qA, 
    //                               NULL,
    //                               NULL);
    assert_exit(ret == 0, ret);
    ret = mbedtls_ecdh_gen_public(&grp, &dB, &qB, 
                                  &bk_mbedtls_rand,
                                  NULL);
    // ret = mbedtls_ecdh_gen_public(&grp, &dB, &qB, 
    //                               NULL,
    //                               NULL);
    assert_exit(ret == 0, ret);
    ret = mbedtls_ecdh_compute_shared(&grp, &zA, &qB, &dA,
                                  &bk_mbedtls_rand,
                                  NULL);
    assert_exit(ret == 0, ret);
    ret = mbedtls_ecdh_compute_shared(&grp, &zB, &qA, &dB,
                                  &bk_mbedtls_rand,
                                  NULL);
    assert_exit(ret == 0, ret);
    ret = mbedtls_mpi_cmp_mpi(&zA, &zB);
    assert_exit(ret == 0, ret);

    return 0;
}

#endif
