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

#include "bk_securityip_adapter.h"
#include <os/mem.h>
#include <os/os.h>
#include <os/str.h>
#include <string.h>
#include <driver/securityip.h>
#include <components/log.h>
#include "bk_feature.h"
#if CONFIG_CACHE_ENABLE
#include "cache.h"
#endif

#define BK_SECURITYIP_OSi_ADAPTER_VERSION  0x00000001

bk_err_t bk_vault_driver_init_wrapper(uint32_t module_name)
{
	return bk_vault_driver_init((security_module_name_t)module_name);
}

bk_err_t bk_vault_driver_deinit_wrapper(uint32_t module_name)
{
	return bk_vault_driver_deinit((security_module_name_t)module_name);
}

static void *malloc_wrapper(size_t size)
{
	return os_malloc(size);
}

static void free_wrapper(void *ptr)
{
	os_free(ptr);
}

static int32_t init_mutex_wrapper(void **mutex)
{
	return rtos_init_mutex((beken_mutex_t *)mutex);
}

static int32_t lock_mutex_wrapper(void ** mutex)
{
	return rtos_lock_mutex((beken_mutex_t *)mutex);
}

static int32_t unlock_mutex_wrapper(void **mutex)
{
	return rtos_unlock_mutex((beken_mutex_t *)mutex);
}

static int32_t deinit_mutex_wrapper(void **mutex)
{
	return rtos_deinit_mutex((beken_mutex_t *)mutex);
}

static void cache_flush_wrapper(void *va, long size)
{
#if CONFIG_CACHE_ENABLE
	flush_dcache(va, size);
#endif

	return;
}

static int32_t bk_feature_config_cache_enable(void) {
#if CONFIG_CACHE_ENABLE
    return 1;
#else
    return 0;
#endif
}

static int32_t feature_config_cache_enable_wrapper(void)
{
    return bk_feature_config_cache_enable();
}

static const securityip_osi_funcs_t g_securityip_osi_funcs = {
	/* version */
	._version = BK_SECURITYIP_OSi_ADAPTER_VERSION,
	/* vault */
	._bk_vault_driver_init = bk_vault_driver_init_wrapper,
	._bk_vault_driver_deinit = bk_vault_driver_deinit_wrapper,
	/* mem */
	._malloc = malloc_wrapper,
	._free = free_wrapper,
	/* log */
	._bk_printf = bk_printf,
	/* mutex */
    ._init_mutex = init_mutex_wrapper,
    ._lock_mutex = lock_mutex_wrapper,
    ._unlock_mutex = unlock_mutex_wrapper,
    ._deinit_mutex = deinit_mutex_wrapper,
    ._cache_flush = cache_flush_wrapper,
    ._feature_config_cache = feature_config_cache_enable_wrapper,
};

bk_err_t bk_securityip_adaptor_init(void)
{
    bk_err_t ret = BK_OK;

    if (bk_securityip_osi_funcs_init((void*)&g_securityip_osi_funcs) != 0)
    {
        return BK_FAIL;
    }

    return ret;
}
