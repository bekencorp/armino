// Copyright 2020-2021 Beken
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

#if defined(MBEDTLS_ECDSA_VERIFY_ALT) || defined(MBEDTLS_ECDSA_SIGN_ALT)
#include "bk_ecdsa_alt.h"
#include "api_val.h"
#include <stdlib.h>
#include "tls_log.h"

/* Discard const qualifier from pointer */
static inline void *
sfzutf_discard_const(
        const void * Ptr_p)
{
    union
    {
        const void * c_p;
        void * n_p;
    } Conversion;

    Conversion.c_p = Ptr_p;
    return Conversion.n_p;
}

static inline int mpi_to_big_int(ValAsymBigInt_t* x, const mbedtls_mpi* y)
{
	if(x == NULL || y == NULL){
		return -1;
	}
	x->ByteDataSize = sizeof(mbedtls_mpi_uint) * (y->n);
	if(x->Data_p == NULL)
		x->Data_p = malloc(x->ByteDataSize);
	//memcpy(x->Data_p, y->p, x->ByteDataSize);
	mbedtls_mpi_write_binary(y, x->Data_p, x->ByteDataSize);

	return 0;
}

static int free_big_int(ValAsymBigInt_t* x)
{
    if(x == NULL){
        return -1;
    }
    if(x->Data_p != NULL){
        free(x->Data_p);
        x->Data_p = NULL;
    }
    return 0;    
}
#if 0
/* NIST P-384 curve (p = 2^384 - 2^128 - 2^96 + 2^32 - 1) */
static const uint8_t ECurve_NIST_P384_p[] =
{
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF
};
static const uint8_t ECurve_NIST_P384_a[] =
{
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFC
};
static const uint8_t ECurve_NIST_P384_b[] =
{
    0xB3, 0x31, 0x2F, 0xA7, 0xE2, 0x3E, 0xE7, 0xE4,
    0x98, 0x8E, 0x05, 0x6B, 0xE3, 0xF8, 0x2D, 0x19,
    0x18, 0x1D, 0x9C, 0x6E, 0xFE, 0x81, 0x41, 0x12,
    0x03, 0x14, 0x08, 0x8F, 0x50, 0x13, 0x87, 0x5A,
    0xC6, 0x56, 0x39, 0x8D, 0x8A, 0x2E, 0xD1, 0x9D,
    0x2A, 0x85, 0xC8, 0xED, 0xD3, 0xEC, 0x2A, 0xEF
};
static const uint8_t ECurve_NIST_P384_Gx[] =
{
    0xAA, 0x87, 0xCA, 0x22, 0xBE, 0x8B, 0x05, 0x37,
    0x8E, 0xB1, 0xC7, 0x1E, 0xF3, 0x20, 0xAD, 0x74,
    0x6E, 0x1D, 0x3B, 0x62, 0x8B, 0xA7, 0x9B, 0x98,
    0x59, 0xF7, 0x41, 0xE0, 0x82, 0x54, 0x2A, 0x38,
    0x55, 0x02, 0xF2, 0x5D, 0xBF, 0x55, 0x29, 0x6C,
    0x3A, 0x54, 0x5E, 0x38, 0x72, 0x76, 0x0A, 0xB7
};
static const uint8_t ECurve_NIST_P384_Gy[] =
{
    0x36, 0x17, 0xDE, 0x4A, 0x96, 0x26, 0x2C, 0x6F,
    0x5D, 0x9E, 0x98, 0xBF, 0x92, 0x92, 0xDC, 0x29,
    0xF8, 0xF4, 0x1D, 0xBD, 0x28, 0x9A, 0x14, 0x7C,
    0xE9, 0xDA, 0x31, 0x13, 0xB5, 0xF0, 0xB8, 0xC0,
    0x0A, 0x60, 0xB1, 0xCE, 0x1D, 0x7E, 0x81, 0x9D,
    0x7A, 0x43, 0x1D, 0x7C, 0x90, 0xEA, 0x0E, 0x5F
};
static const uint8_t ECurve_NIST_P384_n[] =
{
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xC7, 0x63, 0x4D, 0x81, 0xF4, 0x37, 0x2D, 0xDF,
    0x58, 0x1A, 0x0D, 0xB2, 0x48, 0xB0, 0xA7, 0x7A,
    0xEC, 0xEC, 0x19, 0x6A, 0xCC, 0xC5, 0x29, 0x73
};
const TestVector_ECC_Curve_Rec_t ECurve_NIST_P384 =
{
    384,
    ECurve_NIST_P384_p,  sizeof(ECurve_NIST_P384_p),
    ECurve_NIST_P384_a,  sizeof(ECurve_NIST_P384_a),
    ECurve_NIST_P384_b,  sizeof(ECurve_NIST_P384_b),
    ECurve_NIST_P384_Gx, sizeof(ECurve_NIST_P384_Gx),
    ECurve_NIST_P384_Gy, sizeof(ECurve_NIST_P384_Gy),
    ECurve_NIST_P384_n,  sizeof(ECurve_NIST_P384_n),
    1
};
TestVector_ECC_Curve_t ECC_Curve_NIST_P384 = &ECurve_NIST_P384;
#endif
/*----------------------------------------------------------------------------
 *IsTrngActive
 *
 * This function checks if TRNG has been activated and on request it tries
 * to activate the TRNG if it was not activated.
 */
static bool val_Is_trng_active(
        const bool activate)
{
	ValStatus_t Status;
	uint8_t * buffer_p;
	uint16_t trng_sample_cycles = 3072;

	buffer_p = malloc(20);
	if (buffer_p == NULL)
	{
		// Assume TRNG is not active
		return false;
	}

	// Get a random number
	Status = val_RandomData(16, buffer_p);
	free(buffer_p);

	if ((Status != VAL_SUCCESS) && activate )
	{
		Status = val_TrngConfig(0, trng_sample_cycles, 1, 2, 0, 1);;
	}

	return (Status == VAL_SUCCESS);
}

static inline void asset_free(ValAssetId_t assetid)
{
	if(assetid != VAL_ASSETID_INVALID)
	{
		val_AssetFree(assetid);
	}
	return;
}

static void dump_buf(unsigned char *buf, size_t len )
{
	size_t i;

	for( i = 0; i < len; i++ )
	{
		if(i%8 == 0)
			MBEDTLS_LOGI( "\r\n" );

		MBEDTLS_LOGI("%c%c", "0123456789ABCDEF" [buf[i] / 16],
		               "0123456789ABCDEF" [buf[i] % 16] );
	}
	MBEDTLS_LOGI( "\r\n" );
}
static int asym_ecc_load_domain_asset(mbedtls_ecp_group *grp,ValAssetId_t *AssetId_p, 	ValSize_t *ModulusSizeBits)
{
	int ret = 0;
	ValStatus_t Status;
	ValAsymECDomainParam_t Domain = {0};
#if 0
	TestVector_ECC_Curve_Rec_t * Curve_p = NULL;
	mbedtls_ecp_group_id Gid = MBEDTLS_ECP_DP_SECP384R1;

	switch (Gid)
	{
		case MBEDTLS_ECP_DP_SECP384R1:
			Curve_p = sfzutf_discard_const(ECC_Curve_NIST_P384);
		break;

		default:
		break;
	}

    /* Prepare domain parameters */
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
#else
	/*Curve parameters are passed in from the outside*/
	Domain.ModulusSizeBits = grp->pbits;
	mpi_to_big_int(&Domain.Modulus,&grp->P);
	mpi_to_big_int(&Domain.a,&grp->A);
	mpi_to_big_int(&Domain.b,&grp->B);
	mpi_to_big_int(&Domain.Order,&grp->N);
	mpi_to_big_int(&Domain.BasePoint.x,&grp->G.X);
	mpi_to_big_int(&Domain.BasePoint.y,&grp->G.Y);
	Domain.CoFactor = 1;
  
#endif
	/* create asset to load domain */
	Status = val_AsymEccAllocDomainAsset(Domain.ModulusSizeBits, AssetId_p);
	if(Status != VAL_SUCCESS){
		MBEDTLS_LOGE("val_AsymEccAllocDomainAsset failed, status:%d\r\n", Status);
		ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
		goto cleanup;
	}

	//Load domain parameters
	Status = val_AsymEccLoadDomainAssetPlaintext(&Domain, 
												 *AssetId_p,
												 VAL_ASSETID_INVALID,
												 NULL, 0, NULL, NULL);

	if(Status != VAL_SUCCESS){
		MBEDTLS_LOGE("val_AsymEccLoadDomainAssetPlaintext failed, %d\r\n", Status);
		ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
		asset_free(*AssetId_p);
		goto cleanup;
	}
	*ModulusSizeBits = Domain.ModulusSizeBits;

cleanup:
	free_big_int(&Domain.Modulus);
	free_big_int(&Domain.a);
	free_big_int(&Domain.b);
	free_big_int(&Domain.Order);
	free_big_int(&Domain.BasePoint.x);
	free_big_int(&Domain.BasePoint.y);

	return ret;
}

static int asym_ecc_load_private_key_asset(const mbedtls_mpi *d, ValAssetId_t *AssetId_p, ValSize_t ModulusSizeBits)
{
	int ret = 0;
	ValStatus_t Status;
	ValAsymBigInt_t BigIntPrivateKey= {0};

	BigIntPrivateKey.ByteDataSize = d->n*sizeof(mbedtls_mpi_uint);
	BigIntPrivateKey.Data_p = (uint8_t *)malloc(d->n*sizeof(mbedtls_mpi_uint));
	if(BigIntPrivateKey.Data_p == NULL)
	{
		ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
		goto cleanup;
	}
	mbedtls_mpi_write_binary(d, BigIntPrivateKey.Data_p, BigIntPrivateKey.ByteDataSize);

	Status = val_AsymEcdsaAllocPrivateKeyAsset(ModulusSizeBits, 
												VAL_SYM_ALGO_HASH_SHA256, 
												false, false, 
												AssetId_p);
	if(Status != VAL_SUCCESS){
		MBEDTLS_LOGE("val_AsymEcdsaAllocPrivateKeyAsset failed, %d\r\n", Status);
		ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
		goto cleanup;
	}


    /* Initialize the private key asset */
	Status = val_AsymEccLoadPrivateKeyAssetPlaintext(&BigIntPrivateKey,
														ModulusSizeBits,
														*AssetId_p,
														VAL_ASSETID_INVALID,
														NULL, 0, NULL, NULL);
	if(Status != VAL_SUCCESS){
		MBEDTLS_LOGE("val_AsymEccLoadPrivateKeyAssetPlaintext failed, %d\r\n", Status);
		ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
		val_AssetFree(*AssetId_p);  //release asset resource
		*AssetId_p = VAL_ASSETID_INVALID;
	}

cleanup:
	if(BigIntPrivateKey.Data_p != NULL)
	{
		free(BigIntPrivateKey.Data_p);
	}

	return ret;
}

static void dump_key(ValAsymBigInt_t * X)
{
	uint8_t signature_outbuf[100];
	int i;
	memset(signature_outbuf,0x0,0x64);
	MBEDTLS_LOGI("length = %d \r\n",X->ByteDataSize);
	memcpy(signature_outbuf,X->Data_p,X->ByteDataSize);
	for(i=0; i<100; i++)
	{
		if(i%8 == 0)
		MBEDTLS_LOGI("\r\n");

		MBEDTLS_LOGI("%02x",signature_outbuf[i]);
	}
}

static int asym_ecc_load_public_key_asset(const mbedtls_ecp_point *Q, ValAssetId_t *AssetId, ValSize_t ModulusSizeBits)
{
	int ret = 0;
	ValStatus_t Status;
	ValAssetId_t PublicKeyAssetId = VAL_ASSETID_INVALID;
	ValAsymECCPoint_t ECCPointPublicKey = {0};

	ECCPointPublicKey.x.ByteDataSize = VAL_ASYM_DATA_SIZE_B2B(ModulusSizeBits);
	ECCPointPublicKey.y.ByteDataSize = VAL_ASYM_DATA_SIZE_B2B(ModulusSizeBits);
	ECCPointPublicKey.x.Data_p = (uint8_t *)malloc(Q->X.n*sizeof(mbedtls_mpi_uint));
	ECCPointPublicKey.y.Data_p = (uint8_t *)malloc(Q->Y.n*sizeof(mbedtls_mpi_uint));
	if(ECCPointPublicKey.x.Data_p == NULL || ECCPointPublicKey.y.Data_p == NULL)
	{
		ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
		goto cleanup;
	}
	mbedtls_mpi_write_binary(&Q->X, ECCPointPublicKey.x.Data_p, ECCPointPublicKey.x.ByteDataSize);
	mbedtls_mpi_write_binary(&Q->Y, ECCPointPublicKey.y.Data_p, ECCPointPublicKey.x.ByteDataSize);

	Status = val_AsymEcdsaAllocPublicKeyAsset(ModulusSizeBits, VAL_SYM_ALGO_HASH_SHA256, false, false, &PublicKeyAssetId);
	if(Status != VAL_SUCCESS){
		MBEDTLS_LOGE("val_AsymEcdsaAllocPublicKeyAsset failed, %d\r\n", Status);
		ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
		goto cleanup;
	}
#if 0	
	MBEDTLS_LOGI("\r\n ECCPointPublicKey.x out:");
	dump_key(&ECCPointPublicKey.x);
	MBEDTLS_LOGI("\r\n ECCPointPublicKey.y Y out:");
	dump_key(&ECCPointPublicKey.y);
#endif
	Status = val_AsymEccLoadPublicKeyAssetPlaintext(&ECCPointPublicKey,
														ModulusSizeBits,
														PublicKeyAssetId,
														VAL_ASSETID_INVALID,
														NULL, 0, NULL, NULL);
	if(Status != VAL_SUCCESS){
		MBEDTLS_LOGE("val_AsymEccLoadPublicKeyAssetPlaintext failed, %d\r\n", Status);
		ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
		goto cleanup;
	}
	*AssetId = PublicKeyAssetId;

cleanup:
	if(ECCPointPublicKey.x.Data_p != NULL)
		free(ECCPointPublicKey.x.Data_p);
	if(ECCPointPublicKey.x.Data_p != NULL)
		free(ECCPointPublicKey.y.Data_p);
	if(ret != 0)
		asset_free(PublicKeyAssetId);

	return ret;
}

#if defined(MBEDTLS_ECDSA_SIGN_ALT)
/*
 * Compute ECDSA signature of a hashed message
 */
int __attribute__((weak)) mbedtls_ecdsa_sign( mbedtls_ecp_group *grp, mbedtls_mpi *r, mbedtls_mpi *s,
							const mbedtls_mpi *d, const unsigned char *buf, size_t blen,
							int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
	int ret;
	ValStatus_t Status;
	ValAsymSign_t Signature = {0};
	ValAssetId_t DomainAssetId = VAL_ASSETID_INVALID;
	ValAssetId_t PrivateKeyAssetId = VAL_ASSETID_INVALID;
	ValSize_t ModulusSizeBits = 0;
	ValAsymKey_t PrivateKey = {0};
	
	if(grp == NULL || r == NULL || s == NULL || buf == NULL )
	{
		MBEDTLS_LOGE("mbedtls_ecdsa_sign input NULL.\r\n");
		return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
	}
	if(d == NULL)
	{
		MBEDTLS_LOGE("load_private_key input d NULL.\r\n");
		return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
	}

	ret = asym_ecc_load_domain_asset(grp, &DomainAssetId, &ModulusSizeBits);
	if(ret != 0)
	{
		MBEDTLS_LOGE("asym_ecc_load_domain_asset err,return %d.\r\n",ret);
		return ret;
	}

	//load private key
	ret = asym_ecc_load_private_key_asset(d, &PrivateKeyAssetId, ModulusSizeBits);
	if(ret != 0){
		MBEDTLS_LOGE("asym_ecc_load_private_key_asset err,return %d.\r\n",ret);
		goto cleanup;
	}

	val_AsymInitKey(PrivateKeyAssetId, DomainAssetId,
								ModulusSizeBits, VAL_SYM_ALGO_HASH_SHA256,
								&PrivateKey);

	Signature.r.ByteDataSize = VAL_ASYM_DATA_SIZE_B2B(ModulusSizeBits);
	Signature.s.ByteDataSize = VAL_ASYM_DATA_SIZE_B2B(ModulusSizeBits);

	Signature.r.Data_p = (uint8_t *)malloc(Signature.r.ByteDataSize);
	if(Signature.r.Data_p == NULL)
	{
		ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
		goto cleanup;
	}
	Signature.s.Data_p = (uint8_t *)malloc(Signature.s.ByteDataSize);
	if(Signature.s.Data_p == NULL)
	{
		ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
		goto cleanup;
	}

	//ecdsa sign
	Status = val_AsymEcdsaSign(&PrivateKey, &Signature, NULL, blen, (void * const)buf);
	if(Status != VAL_SUCCESS)
	{
		MBEDTLS_LOGE("val_AsymEcdsaSign sign err,return %d.\r\n",Status);
		ret = MBEDTLS_ERR_ECP_VERIFY_FAILED;
		goto cleanup;
	}
	/*export signature in big-endian format*/
	if(r->p != NULL)
		free(r->p);
	r->p = (mbedtls_mpi_uint *)malloc(Signature.r.ByteDataSize);
	if(r->p == NULL)
	{
		ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
		goto cleanup;
	}
	r->n = Signature.r.ByteDataSize/sizeof(mbedtls_mpi_uint);
	mbedtls_mpi_read_binary(r, Signature.r.Data_p, Signature.r.ByteDataSize);

	if(s->p != NULL)
		free(s->p);
	s->p = (mbedtls_mpi_uint *)malloc(Signature.s.ByteDataSize);
	if(s->p == NULL)
	{
		ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
		goto cleanup;
	}
	s->n = Signature.s.ByteDataSize/sizeof(mbedtls_mpi_uint);
	mbedtls_mpi_read_binary(s, Signature.s.Data_p, Signature.s.ByteDataSize);

cleanup:
	if(Signature.r.Data_p != NULL)
		free(Signature.r.Data_p);
	if(Signature.s.Data_p != NULL)
		free(Signature.s.Data_p);

	asset_free(DomainAssetId);
	asset_free(PrivateKeyAssetId);
	return ret;
}
#endif

#if defined(MBEDTLS_ECDSA_VERIFY_ALT)
/*
 * Verify ECDSA signature of hashed message
 */
int __attribute__((weak)) mbedtls_ecdsa_verify( mbedtls_ecp_group *grp,
								const unsigned char *buf, size_t blen,
								const mbedtls_ecp_point *Q,
								const mbedtls_mpi *r,
								const mbedtls_mpi *s)
{
	int ret = 0;
	ValStatus_t Status;
	ValAsymSign_t Signature = {0};
	ValAssetId_t DomainAssetId = VAL_ASSETID_INVALID;
	ValAssetId_t PublicKeyAssetId = VAL_ASSETID_INVALID;
	ValAsymKey_t PublicKey = {0};
	ValSize_t ModulusSizeBits = 0;

	if(grp == NULL || buf == NULL || r == NULL || s == NULL )
	{
		MBEDTLS_LOGE("mbedtls_ecdsa_verify input NULL.\r\n");
		return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
	}
	if(blen == 0)
	{
		return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
	}
	if(Q == NULL)
	{
		return MBEDTLS_ERR_ECP_INVALID_KEY;
	}

	//load domain
	ret = asym_ecc_load_domain_asset(grp, &DomainAssetId, &ModulusSizeBits);
	if(ret != 0)
	{
		MBEDTLS_LOGE("asym_ecc_load_domain_asset err,return %d.\r\n",ret);
		return ret;
	}
	//alloc public key asetid
	ret = asym_ecc_load_public_key_asset(Q, &PublicKeyAssetId, ModulusSizeBits);
	if(ret != 0)
	{
		MBEDTLS_LOGE("asym_ecc_load_public_key_asset err,return %d.\r\n",ret);
		goto cleanup;
	}

	val_AsymInitKey(PublicKeyAssetId, DomainAssetId,
								ModulusSizeBits, VAL_SYM_ALGO_HASH_SHA256,
								&PublicKey);

    Signature.r.ByteDataSize = VAL_ASYM_DATA_SIZE_B2B(ModulusSizeBits);
    Signature.s.ByteDataSize = VAL_ASYM_DATA_SIZE_B2B(ModulusSizeBits);

    Signature.r.Data_p = (uint8_t *)malloc(Signature.r.ByteDataSize);
	if(Signature.r.Data_p == NULL)
	{
		ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
		goto cleanup;
	}
    Signature.s.Data_p = (uint8_t *)malloc(Signature.s.ByteDataSize);
	if(Signature.s.Data_p == NULL)
	{
		ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
		goto cleanup;
	}
	
	mbedtls_mpi_write_binary(r, Signature.r.Data_p, Signature.r.ByteDataSize);
	mbedtls_mpi_write_binary(s, Signature.s.Data_p, Signature.s.ByteDataSize);

	//signature verify
	Status = val_AsymEcdsaVerify(&PublicKey, &Signature, NULL, blen, (void * const)buf );
	if(Status != VAL_SUCCESS)
	{
		MBEDTLS_LOGE("val_AsymEcdsaVerify verify err,return %d.\r\n",Status);
		ret = MBEDTLS_ERR_ECP_VERIFY_FAILED;
		goto cleanup;
	}

cleanup:
	
	if(Signature.r.Data_p != NULL)
		free(Signature.r.Data_p);
	if(Signature.s.Data_p != NULL)
		free(Signature.s.Data_p);

	asset_free(PublicKeyAssetId);
	asset_free(DomainAssetId);
	return ret;
}
#endif

#endif


