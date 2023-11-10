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

#if defined(MBEDTLS_GCM_ALT)
#include "bk_gcm_alt.h"
#include "api_val.h"
#include "tls_log.h"
#include "mbedtls/gcm.h"

#define  MAX_BUFF_LEN  (512) /** Maximum general buffer length. */

/*
 * Initialize context
 */
void __attribute__((weak)) mbedtls_gcm_init( mbedtls_gcm_context *ctx )
{
	if(ctx != NULL)
		memset(ctx, 0x0, sizeof(mbedtls_gcm_context));
}

/*
 * Free context
 */
void __attribute__((weak)) mbedtls_gcm_free( mbedtls_gcm_context *ctx )
{
    if( ctx == NULL )
		return;

	if(ctx != NULL && ctx->key_asset_id != VAL_ASSETID_INVALID)
	{
		MBEDTLS_LOGI("free ctx->key_asset_id %x\r\n",ctx->key_asset_id);
		val_AssetFree(ctx->key_asset_id);
	}
	
	if(ctx != NULL && ctx->sym_context_p != NULL)
	{
		MBEDTLS_LOGI("free ctx->sym_context_p 0x%x\r\n",ctx->sym_context_p);
		val_SymRelease(ctx->sym_context_p);
		memset(ctx, 0x0, sizeof(mbedtls_gcm_context));
	}
}

int __attribute__((weak)) mbedtls_gcm_setkey( mbedtls_gcm_context *ctx,
                        mbedtls_cipher_id_t cipher,
                        const unsigned char *key,
                        unsigned int keybits )
{
	uint32_t key_size;
	ValStatus_t status;
	int ret = 0;
	
	if(ctx == NULL || key == NULL)
		return MBEDTLS_ERR_GCM_BAD_INPUT;

	//if(cipher != MBEDTLS_CIPHER_ID_AES)
	//	return MBEDTLS_ERR_AES_HW_ACCEL_FAILED;

	if( keybits % 8 != 0 )
		return( MBEDTLS_ERR_GCM_BAD_INPUT_DATA_LENGTH );

	key_size = (keybits + 7 ) / 8;

	status = val_SymAlloc(VAL_SYM_ALGO_CIPHER_AES, VAL_SYM_MODE_CIPHER_GCM, false, &ctx->sym_context_p);
	if(status != VAL_SUCCESS)
	{
		MBEDTLS_LOGE("val_SymAlloc return %d\r\n", status);
		return MBEDTLS_ERR_GCM_HW_ACCEL_FAILED;
	}
	
	if(0)//(fKeyAsset)
    {
    	ValPolicyMask_t key_asset_policy = 0;
		ValAssetId_t target_id;

    	key_asset_policy = VAL_POLICY_AES_MODE_GCM | VAL_POLICY_ENCRYPT | VAL_POLICY_DECRYPT;

		if (!val_IsAccessSecure())
	    {
	        key_asset_policy |= VAL_POLICY_SOURCE_NON_SECURE;
	    }
		
		status = val_AssetAlloc(key_asset_policy, key_size,
	                            false, false,
	                            VAL_ASSET_LIFETIME_INFINITE, false, false, 0,
	                            &target_id);
		if(status != VAL_SUCCESS)
		{
			MBEDTLS_LOGE("val_AssetAlloc return %d\r\n", status);
			ret = MBEDTLS_ERR_GCM_HW_ACCEL_FAILED;
			goto error_func_exit;
		}

		status = val_AssetLoadPlaintext(target_id, key, key_size);
		if(status != VAL_SUCCESS)
		{
			MBEDTLS_LOGE("val_AssetLoadPlaintext return %d\r\n", status);
			ret = MBEDTLS_ERR_GCM_HW_ACCEL_FAILED;
			goto error_func_exit;
		}

		status = val_SymInitKey(ctx->sym_context_p, target_id, key, key_size);
		if(status != VAL_SUCCESS)
		{
			MBEDTLS_LOGE("val_SymInitKey return %d\r\n", status);
			ret = MBEDTLS_ERR_GCM_HW_ACCEL_FAILED;
			goto error_func_exit;
		}
		ctx->key_asset_id = target_id;
	}else
	{
        status = val_SymInitKey(ctx->sym_context_p, VAL_ASSETID_INVALID,
                                key, key_size);
	}
	if(status != VAL_SUCCESS)
	{
		MBEDTLS_LOGE("val_SymInitKey return %d\r\n", status);
		ret = MBEDTLS_ERR_GCM_HW_ACCEL_FAILED;
		goto error_func_exit;
	}
	return 0;

error_func_exit:
	mbedtls_gcm_free(ctx);
	return ret;
}


static int aes_gcm_crypt(mbedtls_gcm_context *ctx, int mode, size_t length,
								const unsigned char *add, size_t add_len,
								const unsigned char *input, unsigned char *output,
								unsigned char *tag, size_t *tag_len_p,
								const ValSymGCMMode_t gcm_mode)
{
	ValStatus_t status;
	ValSize_t ResultLen = length;

	status = val_SymCipherAEInit(ctx->sym_context_p,
                                 NULL, 0,
                                 *tag_len_p, gcm_mode);
	if(status != VAL_SUCCESS)
	{
		MBEDTLS_LOGE("val_SymCipherAEInit return %d\r\n", status);
		return MBEDTLS_ERR_GCM_HW_ACCEL_FAILED;
	}

	if(mode)
	{
		val_SymCipherInitEncrypt(ctx->sym_context_p);
	}

	status = val_SymCipherAEFinal(ctx->sym_context_p,
									add, add_len,
									input, length,
									output, &ResultLen,
									tag, tag_len_p);
	if(status != VAL_SUCCESS)
	{
		MBEDTLS_LOGE("val_SymCipherAEFinal return %d\r\n", status);
		return MBEDTLS_ERR_GCM_HW_ACCEL_FAILED;
	}

	return 0;
}

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

int  mbedtls_gcm_crypt_and_tag( mbedtls_gcm_context *ctx,
                       int mode,
                       size_t length,
                       const unsigned char *iv,
                       size_t iv_len,
                       const unsigned char *add,
                       size_t add_len,
                       const unsigned char *input,
                       unsigned char *output,
                       size_t tag_len,
                       unsigned char *tag )
{
	int32_t status;
	uint8_t IV[VAL_SYM_ALGO_AES_IV_SIZE];

	if(ctx == NULL || iv == NULL || input == NULL || tag == NULL)
		return MBEDTLS_ERR_GCM_BAD_INPUT;

	if( length % VAL_SYM_ALGO_AES_BLOCK_SIZE != 0 )
		return MBEDTLS_ERR_GCM_BAD_INPUT_DATA_LENGTH;

	if (iv_len == 12)
	{
		memcpy(IV, iv, iv_len);
		memcpy(&IV[12], "\x00\x00\x00\x01", 4);
		status = val_SymInitIV(ctx->sym_context_p, IV, VAL_SYM_ALGO_AES_IV_SIZE);
	}
	else
	{
		return MBEDTLS_ERR_GCM_BAD_INPUT_DATA_LENGTH;
	}

	status = aes_gcm_crypt(ctx, mode, length, add, add_len, input, output,
					tag, &tag_len, VAL_SYM_MODE_GCM_AUTO);

	if(status != VAL_SUCCESS)
	{
		MBEDTLS_LOGE("aes_ccm_crypt return %d\r\n", status);
	}

	return status;
}

int __attribute__((weak)) mbedtls_gcm_auth_decrypt( mbedtls_gcm_context *ctx,
                      size_t length,
                      const unsigned char *iv,
                      size_t iv_len,
                      const unsigned char *add,
                      size_t add_len,
                      const unsigned char *tag,
                      size_t tag_len,
                      const unsigned char *input,
                      unsigned char *output )
{
	int32_t status;
	uint8_t * tag_p;
	uint32_t tag_size;
	uint8_t IV[VAL_SYM_ALGO_AES_IV_SIZE];

	if(ctx == NULL || iv == NULL || input == NULL || tag == NULL)
		return MBEDTLS_ERR_GCM_BAD_INPUT;

	if( length % VAL_SYM_ALGO_AES_BLOCK_SIZE != 0 )
		return( MBEDTLS_ERR_GCM_BAD_INPUT_DATA_LENGTH );


	tag_size = tag_len;
	tag_p = sfzutf_discard_const(tag);

	if (iv_len == 12)
	{
		memcpy(IV, iv, iv_len);
		memcpy(&IV[12], "\x00\x00\x00\x01", 4);
		status = val_SymInitIV(ctx->sym_context_p, IV, VAL_SYM_ALGO_AES_IV_SIZE);
	}
	else
	{
		return MBEDTLS_ERR_GCM_BAD_INPUT_DATA_LENGTH;
	}

	status = aes_gcm_crypt(ctx, MBEDTLS_GCM_DECRYPT, length, add, add_len, input, output,
					tag_p, (size_t *)&tag_size, VAL_SYM_MODE_GCM_AUTO);

	MBEDTLS_LOGI("debug : tag_size after decrypt %d \r\n",tag_size);
	
	if(status != VAL_SUCCESS)
	{
		MBEDTLS_LOGE("aes_ccm_crypt return %d\r\n", status);
	}

	return status;
}

#endif
