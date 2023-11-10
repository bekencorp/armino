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

#if defined(MBEDTLS_AES_ALT)

#include "bk_aes_alt.h"
#include "api_val.h"
#include "os/mem.h"//for malloc and free
#include "tls_log.h"
#include "mbedtls/aes.h"

/*typedef*/
typedef enum
{
    VECTORS_MODE_ECB = 0,
    VECTORS_MODE_CBC,
    VECTORS_MODE_CTR,
    VECTORS_MODE_ICM,
    VECTORS_MODE_F8,
    VECTORS_MODE_XTS,
    VECTORS_MODE_NUM        // must be last
} algo_mode_e;

void __attribute__((weak)) mbedtls_aes_init( mbedtls_aes_context *ctx )
{
	if(ctx != NULL)
		memset(ctx, 0x0, sizeof(mbedtls_aes_context));
}

void __attribute__((weak)) mbedtls_aes_free( mbedtls_aes_context *ctx )
{
	if(ctx != NULL && ctx->key_asset_id != VAL_ASSETID_INVALID)
	{
		//MBEDTLS_LOGI("free ctx->key_asset_id %x\r\n",ctx->key_asset_id);
		val_AssetFree(ctx->key_asset_id);
	}
	
	if(ctx != NULL && ctx->sym_context_p != NULL)
	{
		//MBEDTLS_LOGI("free ctx->sym_context_p 0x%x\r\n",ctx->sym_context_p);
		val_SymRelease(ctx->sym_context_p);
		memset(ctx, 0x0, sizeof(mbedtls_aes_context));
	}
}

void __attribute__((weak)) mbedtls_aes_xts_init( mbedtls_aes_xts_context *ctx )
{
	if(ctx != NULL)
		memset(ctx, 0x0, sizeof(mbedtls_aes_context));
}

void __attribute__((weak)) mbedtls_aes_xts_free( mbedtls_aes_xts_context *ctx )
{
	if(ctx != NULL && ctx->key_asset_id != VAL_ASSETID_INVALID)
	{
		//MBEDTLS_LOGI("free ctx->key_asset_id %x\r\n",ctx->key_asset_id);
		val_AssetFree(ctx->key_asset_id);
	}
	
	if(ctx != NULL && ctx->sym_context_p != NULL)
	{
		//MBEDTLS_LOGI("free ctx->sym_context_p 0x%x\r\n",ctx->sym_context_p);
		val_SymRelease(ctx->sym_context_p);
		memset(ctx, 0x0, sizeof(mbedtls_aes_context));
	}
}

/*
 * AES key schedule (encryption)
 */
int __attribute__((weak)) mbedtls_aes_setkey_enc( mbedtls_aes_context *ctx, const unsigned char *key,
                    unsigned int keybits )
{
	if(key == NULL)
	{
		MBEDTLS_LOGI("%s key=NULL\r\n",__func__);
		return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
	}

	if( ctx == NULL )
	{
		MBEDTLS_LOGI("%s ctx=NULL\r\n",__func__);
		return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
	}	
	else
		mbedtls_aes_free(ctx);        // Clear context

    switch( keybits )
    {
        case 128: ctx->key_size = 16; break;
        case 192: ctx->key_size = 24; break;
        case 256: ctx->key_size = 32; break;
        default : 
		{	
			MBEDTLS_LOGE("%s keybits:%d\r\n",__func__,keybits);
			return( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );
		}
    }
	memcpy(ctx->key, key, ctx->key_size);

	return 0;
}

/*
 * AES key schedule (decryption)
 */
int __attribute__((weak)) mbedtls_aes_setkey_dec( mbedtls_aes_context *ctx, const unsigned char *key,
                    unsigned int keybits )
{
	if(key == NULL)
	{
		MBEDTLS_LOGE("%s key=NULL\r\n",__func__);
		return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
	}
	if( ctx == NULL )
	{
		MBEDTLS_LOGE("%s ctx=NULL\r\n",__func__);
		return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
	}	
	else
		mbedtls_aes_free(ctx);        // Clear context

    switch( keybits )
    {
        case 128: ctx->key_size = 16; break;
        case 192: ctx->key_size = 24; break;
        case 256: ctx->key_size = 32; break;
        default : 
			{
				MBEDTLS_LOGE("%s keybits:%d\r\n",__func__,keybits);
				return( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );
			}
    }
	memcpy(ctx->key, key, ctx->key_size);
	
	return 0;
}

int __attribute__((weak)) mbedtls_aes_xts_setkey_enc( mbedtls_aes_xts_context *ctx,
                                const unsigned char *key,
                                unsigned int keybits)
{
	if(key == NULL)
	{
		MBEDTLS_LOGE("%s key=NULL\r\n",__func__);
		return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
	}
	if( ctx == NULL )
	{
		MBEDTLS_LOGE("%s ctx=NULL\r\n",__func__);
		return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
	}
	else
	{
		if(ctx != NULL && ctx->key_asset_id != VAL_ASSETID_INVALID)
		{
			MBEDTLS_LOGI("free ctx->key_asset_id %x\r\n",ctx->key_asset_id);
			val_AssetFree(ctx->key_asset_id);
		}

		if(ctx != NULL && ctx->sym_context_p != NULL)
		{
			MBEDTLS_LOGI("free ctx->sym_context_p 0x%x\r\n",ctx->sym_context_p);
			val_SymRelease(ctx->sym_context_p);
			memset(ctx, 0x0, sizeof(mbedtls_aes_context));
		}
	}
		//mbedtls_aes_free(ctx);        // Clear context

    switch( keybits )
    {
        case 128: ctx->key_size = 16; break;
        case 256: ctx->key_size = 32; break;
        default : 
		{
			MBEDTLS_LOGE("%s keybits:%d\r\n",__func__,keybits);
			return( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );
		}
    }
	memcpy(ctx->key, key, ctx->key_size);

	return 0;
}

int __attribute__((weak)) mbedtls_aes_xts_setkey_dec( mbedtls_aes_xts_context *ctx,
                                const unsigned char *key,
                                unsigned int keybits)
{

	if(key == NULL)
	{
		MBEDTLS_LOGE("%s key=NULL\r\n",__func__);
		return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
	}

	if( ctx == NULL )
	{
		MBEDTLS_LOGE("%s ctx=NULL\r\n",__func__);
		return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
	}
	else
	{
		if(ctx != NULL && ctx->key_asset_id != VAL_ASSETID_INVALID)
		{
			MBEDTLS_LOGI("free ctx->key_asset_id %x\r\n",ctx->key_asset_id);
			val_AssetFree(ctx->key_asset_id);
		}

		if(ctx != NULL && ctx->sym_context_p != NULL)
		{
			MBEDTLS_LOGI("free ctx->sym_context_p 0x%x\r\n",ctx->sym_context_p);
			val_SymRelease(ctx->sym_context_p);
			memset(ctx, 0x0, sizeof(mbedtls_aes_context));
		}
	}
		//mbedtls_aes_free(ctx);        // Clear context

    switch( keybits )
    {
        case 128: ctx->key_size = 16; break;
        case 256: ctx->key_size = 32; break;
        default : 
		{
			MBEDTLS_LOGE("%s keybits:%d\r\n",__func__,keybits);
			return( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );
		}
    }
	memcpy(ctx->key, key, ctx->key_size);
	return 0;
}

/**/
static ValStatus_t aes_crypt_set_key_asset( void *ctx, algo_mode_e algo_mode,
								ValOctetsIn_t * const iv_p)
{	
	mbedtls_aes_xts_context *ctx_p = ctx;
	ValPolicyMask_t key_asset_policy = 0;
	ValAssetId_t target_id;
	ValStatus_t status;
	ValSymMode_t sym_mode = VAL_SYM_MODE_NONE;
	uint32_t iv_size = 0;

	if(ctx_p->key_init_done != true)
	 {
		switch (algo_mode)
		{
			case VECTORS_MODE_ECB:
				sym_mode = VAL_SYM_MODE_CIPHER_ECB;
				key_asset_policy = VAL_POLICY_MODE1;
			break;
			case VECTORS_MODE_CBC:
				sym_mode = VAL_SYM_MODE_CIPHER_CBC;
				key_asset_policy = VAL_POLICY_MODE2;
				iv_size = VAL_SYM_ALGO_AES_IV_SIZE;
			break;
			case VECTORS_MODE_CTR:
				sym_mode = VAL_SYM_MODE_CIPHER_CTR;
				key_asset_policy = VAL_POLICY_MODE4;
				iv_size = VAL_SYM_ALGO_AES_IV_SIZE;
				//TestCntxt_p->fPadding = true;
				//TestCntxt_p->PaddingByte = 0xDC;
			break;
			case VECTORS_MODE_ICM:
				sym_mode = VAL_SYM_MODE_CIPHER_ICM;
				key_asset_policy = VAL_POLICY_MODE5;
				iv_size = VAL_SYM_ALGO_AES_IV_SIZE;
				//TestCntxt_p->fPadding = true;
				//TestCntxt_p->PaddingByte = 0xDC;
			break;

			case VECTORS_MODE_F8:
				sym_mode = VAL_SYM_MODE_CIPHER_F8;
				key_asset_policy = VAL_POLICY_MODE8;
				iv_size = VAL_SYM_ALGO_AES_IV_SIZE;
				//TestCntxt_p->fPadding = true;
				//TestCntxt_p->PaddingByte = 0xDC;
			break;

			case VECTORS_MODE_XTS:
				sym_mode = VAL_SYM_MODE_CIPHER_XTS;
				key_asset_policy = VAL_POLICY_MODE9;
				iv_size = VAL_SYM_ALGO_AES_IV_SIZE;
				//TestCntxt_p->fPadding = true;
				//TestCntxt_p->PaddingByte = 0x00;
			break;

			default:
			break;
		}
		key_asset_policy |= VAL_POLICY_ALGO_CIPHER_AES|VAL_POLICY_ENCRYPT|VAL_POLICY_DECRYPT;
		
		status = val_SymAlloc(VAL_SYM_ALGO_CIPHER_AES, sym_mode, false, &ctx_p->sym_context_p);
		if(status != VAL_SUCCESS)
		{
			MBEDTLS_LOGE("val_SymAlloc return %d\r\n", status);
			goto error_func_exit;
		}

	    if (!val_IsAccessSecure())
	    {
	        key_asset_policy |= VAL_POLICY_SOURCE_NON_SECURE;
	    }
		
		status = val_AssetAlloc(key_asset_policy, ctx_p->key_size,
	                            false, false,
	                            VAL_ASSET_LIFETIME_INFINITE, false, false, 0,
	                            &target_id);
		if(status != VAL_SUCCESS)
		{
			MBEDTLS_LOGE("val_AssetAlloc return %d\r\n", status);
			goto error_func_exit;
		}

		status = val_AssetLoadPlaintext(target_id, ctx_p->key, ctx_p->key_size);
		if(status != VAL_SUCCESS)
		{
			MBEDTLS_LOGE("val_AssetLoadPlaintext return %d\r\n", status);
			goto error_func_exit;
		}

		status = val_SymInitKey(ctx_p->sym_context_p, target_id, ctx_p->key, ctx_p->key_size);
		if(status != VAL_SUCCESS)
		{
			MBEDTLS_LOGE("val_SymInitKey return %d\r\n", status);
		goto error_func_exit;
		}

	    if (iv_size)
	    {
			//MBEDTLS_LOGI(" debug :iv_size = %d\r\n", iv_size);
	        status = val_SymInitIV(ctx_p->sym_context_p, iv_p, iv_size);
			if(status != VAL_SUCCESS)
			{
				MBEDTLS_LOGE("val_SymInitIV return %d\r\n", status);
				goto error_func_exit;
			}
	    }

		ctx_p->key_asset_id = target_id;
		ctx_p->key_init_done = true;
	}
	return VAL_SUCCESS;
	
error_func_exit:
	mbedtls_aes_free(ctx);
	return status;
	
}	 

/*
 * AES-ECB block encryption/decryption
 */
int __attribute__((weak)) mbedtls_aes_crypt_ecb( mbedtls_aes_context *ctx,
                           int mode,
                           const unsigned char input[16],
                           unsigned char output[16] )
{
	ValStatus_t status;
	ValSize_t out_size = 16;

	status = aes_crypt_set_key_asset((void *)ctx, VECTORS_MODE_ECB, NULL);
	if(status != VAL_SUCCESS)
	{
		MBEDTLS_LOGE("%s aes_crypt_set_key_asset return %d\r\n", __func__,status);
		return MBEDTLS_ERR_AES_HW_ACCEL_FAILED;
	}

	if(mode == MBEDTLS_AES_ENCRYPT){
		val_SymCipherInitEncrypt(ctx->sym_context_p);//sets the direction of cipher operation to Encrypt
	}else{
	}
	
	status = val_SymCipherUpdate(ctx->sym_context_p, input, 16, output, &out_size);
	if(status != VAL_SUCCESS)
	{
		MBEDTLS_LOGE("val_SymCipherUpdate return %d\r\n", status);
		return MBEDTLS_ERR_AES_HW_ACCEL_FAILED;
	}
	return 0;
}

/*
 * AES-CBC buffer encryption/decryption
 */
int __attribute__((weak)) mbedtls_aes_crypt_cbc( mbedtls_aes_context *ctx,
                    int mode,
                    size_t length,
                    unsigned char iv[16],
                    const unsigned char *input,
                    unsigned char *output )
{
    ValStatus_t status;
    ValSize_t out_size = length;

    if(( ctx == NULL )||(iv == NULL)||(input == NULL)||(output == NULL))
	{
		MBEDTLS_LOGE("%s ctx:%p iv:%p input:%p output:%p\r\n",__func__,ctx,iv,input,output);
		return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
	}

    if( mode != MBEDTLS_AES_ENCRYPT && mode != MBEDTLS_AES_DECRYPT )
	{
		MBEDTLS_LOGE("%s mode:%d\r\n",__func__,mode);
        return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
	}

    if( length % 16 )
	{
		MBEDTLS_LOGE("%s length:%d\r\n",__func__,length);
        return( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );
	}

    status = aes_crypt_set_key_asset((void *)ctx, VECTORS_MODE_CBC, iv);
    if(status != VAL_SUCCESS)
    {
        MBEDTLS_LOGE("aes_crypt_set_key_asset return %d\r\n", status);
        return MBEDTLS_ERR_AES_HW_ACCEL_FAILED;
    }

    if(mode == MBEDTLS_AES_ENCRYPT){
        val_SymCipherInitEncrypt(ctx->sym_context_p);//sets the direction of cipher operation to Encrypt
    }else{
    }

    status = val_SymCipherUpdate(ctx->sym_context_p, input, length, output, &out_size);
    if(status != VAL_SUCCESS)
    {
        MBEDTLS_LOGE("val_SymCipherUpdate return %d\r\n", status);
        return MBEDTLS_ERR_AES_HW_ACCEL_FAILED;
    }
    //memcpy(iv, ctx->sym_context_p->Service.Cipher.IV, 16);    //need debug return IV
    //MBEDTLS_LOGI("mbedtls_aes_crypt_ecb return %d\r\n",out_size);

    return 0;
}



/*
 * AES-XTS buffer encryption/decryption
 */
int __attribute__((weak)) mbedtls_aes_crypt_xts( mbedtls_aes_xts_context *ctx,
                           int mode,
                           size_t length,
                           const unsigned char data_unit[16],
                           const unsigned char *input,
                           unsigned char *output )
{
	ValStatus_t status;
	ValSize_t out_size = length;

    if( mode != MBEDTLS_AES_ENCRYPT && mode != MBEDTLS_AES_DECRYPT )
	{
		MBEDTLS_LOGE("%s mode:%d\r\n",__func__,mode);
        return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
	}

    /* Data units must be at least 16 bytes long. */
    if( length < 16 )
	{
		MBEDTLS_LOGE("%s len:%d\r\n",__func__,length);
        return MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH;
	}

    /* NIST SP 800-38E disallows data units larger than 2**20 blocks. */
    if( length > (( 1 << 20 ) * 16 ))
	{
		MBEDTLS_LOGE("%s len:%d>2**20\r\n",__func__,length);
		return MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH;
	}

	status = aes_crypt_set_key_asset((void *)ctx, VECTORS_MODE_XTS, data_unit);
	if(status != VAL_SUCCESS)
	{
		MBEDTLS_LOGE("%s aes_crypt_set_key_asset return %d\r\n", __func__,status);
		return MBEDTLS_ERR_AES_HW_ACCEL_FAILED;
	}

	if(mode == MBEDTLS_AES_ENCRYPT){
		val_SymCipherInitEncrypt(ctx->sym_context_p);//sets the direction of cipher operation to Encrypt
	}else{
	}

	status = val_SymCipherUpdate(ctx->sym_context_p, input, length, output, &out_size);
	if(status != VAL_SUCCESS)
	{
		MBEDTLS_LOGE("val_SymCipherUpdate return %d\r\n", status);
		return MBEDTLS_ERR_AES_HW_ACCEL_FAILED;
	}

	return 0;
}


/*
 * AES-CFB128 buffer encryption/decryption
 */
int __attribute__((weak)) mbedtls_aes_crypt_cfb128( mbedtls_aes_context *ctx,
                       int mode,
                       size_t length,
                       size_t *iv_off,
                       unsigned char iv[16],
                       const unsigned char *input,
                       unsigned char *output )
{
	return 0;
}

/*
 * AES-CFB8 buffer encryption/decryption
 */
int __attribute__((weak)) mbedtls_aes_crypt_cfb8( mbedtls_aes_context *ctx,
                            int mode,
                            size_t length,
                            unsigned char iv[16],
                            const unsigned char *input,
                            unsigned char *output )
{
	return 0;
}

/*
 * AES-OFB (Output Feedback Mode) buffer encryption/decryption
 */
int __attribute__((weak)) mbedtls_aes_crypt_ofb( mbedtls_aes_context *ctx,
                           size_t length,
                           size_t *iv_off,
                           unsigned char iv[16],
                           const unsigned char *input,
                           unsigned char *output )
{
	return 0;
}

/*
 * AES-CTR buffer encryption/decryption
 */
int __attribute__((weak)) mbedtls_aes_crypt_ctr( mbedtls_aes_context *ctx,
                       size_t length,
                       size_t *nc_off,
                       unsigned char nonce_counter[16],
                       unsigned char stream_block[16],
                       const unsigned char *input,
                       unsigned char *output )
{
	ValStatus_t status=VAL_SUCCESS;
	ValSize_t out_size ;

	ValSize_t padinglen=length;
	uint8_t *padingBuf=NULL;
	if(( ctx == NULL )||(nonce_counter == NULL)||(input == NULL)||(output == NULL))
	{
		MBEDTLS_LOGE("%s ctx:%p nonce:%p input:%p output:%p\r\n",__func__,ctx,nonce_counter,input,output);
		return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
	}

	if( length % 16 )  // Must be 16-byte aligned
	{
		padinglen=(length+15)/16*16;

	}
	padingBuf=os_malloc(padinglen);
	if(padingBuf==NULL)
	{
		MBEDTLS_LOGE("%s padingBuf=NULL\r\n",__func__);
		status=MBEDTLS_ERR_AES_BAD_INPUT_DATA;
		goto exit_fun;
	}
	memset(padingBuf,(padinglen-length),padinglen);
	memcpy(padingBuf,input,length);
	out_size=padinglen;
	status = aes_crypt_set_key_asset((void *)ctx, VECTORS_MODE_CTR, nonce_counter);
	if(status != VAL_SUCCESS)
	{
		MBEDTLS_LOGE("aes_crypt_set_key_asset return %d\r\n", status);
		status= MBEDTLS_ERR_AES_HW_ACCEL_FAILED;
		goto exit_fun;
	}

	status = val_SymCipherUpdate(ctx->sym_context_p, padingBuf, padinglen, padingBuf, &out_size);
	if(status != VAL_SUCCESS)
	{
		MBEDTLS_LOGE("val_SymCipherUpdate return %d\r\n", status);
		status= MBEDTLS_ERR_AES_HW_ACCEL_FAILED;
		goto exit_fun;
	}
	memcpy(output,padingBuf,length);

exit_fun:
	if(padingBuf!=NULL)
		os_free(padingBuf);
	return status;
}

#endif
