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
#ifndef __BK_SECURITYIP_ADAPTER_H
#define __BK_SECURITYIP_ADAPTER_H

#include <common/bk_include.h>
#include <driver/vault.h>

typedef struct {
	int _version;
	bk_err_t (*_bk_vault_driver_init)(uint32_t module_name);
	bk_err_t (*_bk_vault_driver_deinit)(uint32_t module_name);
	void *(*_malloc)(unsigned int size);
	void (*_free)(void *mem_ptr);
	void (*_bk_printf)(const char *fmt, ...);
	int32_t (*_deinit_mutex)(void** mutex);
	int32_t (*_init_mutex)(void** mutex);
	int32_t (*_lock_mutex)(void** mutex);
	int32_t (*_unlock_mutex)(void** mutex);
	void (*_cache_flush)(void *va, long size);
	int32_t (*_feature_config_cache)(void);
}securityip_osi_funcs_t;

bk_err_t bk_securityip_adaptor_init(void);

#endif
