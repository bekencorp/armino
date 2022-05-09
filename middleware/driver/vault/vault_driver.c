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

#include <stdlib.h>
#include <common/bk_include.h>
//#include <driver/vault.h>
#include "vault_driver.h"
#include "api_driver_init.h"
#include <driver/vault.h>

bk_err_t bk_vault_driver_init(void)
{
	if(Driver130_Init() < 0){
		return BK_ERR_VAULT_DRIVER_INIT_FAIL;
	}	
	return BK_OK;
}

bk_err_t bk_vault_driver_deinit(void)
{
	Driver130_Exit();
	return BK_OK;
}

