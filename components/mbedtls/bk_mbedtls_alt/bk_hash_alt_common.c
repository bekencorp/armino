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

#if defined(MBEDTLS_SHA512_ALT) || defined(MBEDTLS_SHA256_ALT) || defined(MBEDTLS_SHA1_ALT) 
#include "bk_hash_alt_common.h"
#include "tls_log.h"

extern bool bk_get_sym_ctx_magic(ValSymContextPtr_t Context_p);

static uint8_t log2x(uint32_t num)
{
    uint8_t count = 0;
    if(1 == num)
        return 0;
    while(num > 1){
        num = num >> 1;
        count++;
    }
    return count;
}

int bk_val_sym_alloc(ValSymAlgo_t alg, void **context_p)
{
    if(VAL_SYM_ALGO_NONE == alg){
       return -1;
    }
    if(*context_p == NULL){
	    val_SymAlloc(alg, VAL_SYM_MODE_NONE, true, context_p);
	}else if(!bk_get_sym_ctx_magic(*context_p)){
        val_SymRelease(*context_p);
        *context_p = NULL;
	    val_SymAlloc(alg, VAL_SYM_MODE_NONE, true, context_p);
    }
    return 0;
}

int bk_hash_update(void *context_p, \
                        unsigned char *data, \
                        uint32_t *llen, \
                        hash_alg_t hash_alg, \
                        const unsigned char *input, \
                        size_t ilen)
{
    int ret = 0;
    uint32_t block_count;
    uint32_t update_count;
    uint32_t fill;
    size_t max_block_size = 0;
    if(context_p == NULL){
        ret = -1;
        goto cleanup;
    }
    if(ilen == 0)
        return 0;
    if(input == NULL){
        ret = -1;
        goto cleanup;
    }
#if defined(MBEDTLS_SHA1_ALT)
    if(SHA1_ALT == hash_alg){
        max_block_size = BK_SHA1_ALT_MAX_BLOCK_SIZE;               
    }
#endif
#if defined(MBEDTLS_SHA256_ALT)
    if(SHA256_ALT == hash_alg || SHA224_ALT == hash_alg){
        max_block_size = BK_SHA256_ALT_MAX_BLOCK_SIZE;
    }
#endif
#if defined(MBEDTLS_SHA512_ALT)
    if(SHA512_ALT == hash_alg || SHA384_ALT == hash_alg){
        max_block_size = BK_SHA512_ALT_MAX_BLOCK_SIZE;
    }
#endif

init_process:

    if(*llen == 0 && ilen <= max_block_size){
		os_memcpy(data, input, ilen);
        *llen = ilen;
        return 0;       
    }

    if(*llen == 0 && ilen > max_block_size){
		block_count = ilen >> log2x(max_block_size);
        update_count = block_count;
        *llen = ilen & (max_block_size-1);
        
        if (0 == *llen){
            *llen = max_block_size;
            update_count = block_count - 1;
        }
        os_memcpy(data, input+(update_count * max_block_size), *llen);
        for(int i=0; i< update_count; i++){
            ret = val_SymHashUpdate(context_p,
                                (ValOctetsIn_t *)input,
                                max_block_size);
            if(ret != VAL_SUCCESS){
				MBEDTLS_LOGE("val_SymHashUpdate return %d\r\n", ret);
#if defined(MBEDTLS_SHA1_ALT)
                ret = MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
#endif
#if defined(MBEDTLS_SHA256_ALT)
                ret = MBEDTLS_ERR_SHA256_HW_ACCEL_FAILED;
#endif
#if defined(MBEDTLS_SHA512_ALT)
                ret = MBEDTLS_ERR_SHA512_HW_ACCEL_FAILED;
#endif
                goto cleanup;
            }      
            input = input + (max_block_size);
        }

        return 0;
    }

    fill = max_block_size - *llen;

    if(0 == fill){
        ret = val_SymHashUpdate(context_p,
                                (ValOctetsIn_t *)data,
                                max_block_size);
        if(ret != VAL_SUCCESS){
            MBEDTLS_LOGE("val_SymHashUpdate return %d\r\n", ret);
            ret = -1;
            goto cleanup;
        }
        *llen = 0;
        goto init_process;
    }

    if(ilen > fill){
        os_memcpy((void *)(data + *llen), input, fill);
        input += fill;
        ilen -= fill;
        ret = val_SymHashUpdate(context_p,
                                (ValOctetsIn_t *)data,
                                max_block_size);
        if(ret != VAL_SUCCESS){
            MBEDTLS_LOGE("val_SymHashUpdate return %d\r\n", ret);
            ret = -1;
            goto cleanup;
        }
        *llen = 0;
        goto init_process;
    }else{
        os_memcpy((void *)(data + *llen), input, ilen);
        *llen += ilen;
        return 0;
    }

cleanup:
    if(NULL != data){
        os_free(data);
        data = NULL;
    }
    if(NULL != context_p){
		val_SymRelease(context_p);
        context_p = NULL;
    }

    return ret;
}
#endif /* MBEDTLS_SHA1_ALT */
