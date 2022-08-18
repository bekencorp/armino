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

#include <common/bk_include.h>
#include <components/system.h>
#include <driver/prro.h>
#include <os/os.h>
#include <os/mem.h>
#include "prro_driver.h"
#include "prro_hal.h"

typedef struct {
	prro_hal_t hal;
} prro_driver_t;

#define PRRO_RETURN_ON_DRIVER_NOT_INIT() do {\
	if (!s_prro_driver_is_init) {\
		PRRO_LOGE("PRRO driver not init\r\n");\
		return BK_ERR_PRRO_DRIVER_NOT_INIT;\
	}\
} while(0)

static prro_driver_t s_prro = {0};
static bool s_prro_driver_is_init = false;

bk_err_t bk_prro_driver_init(void)
{
	if (s_prro_driver_is_init) {
		return BK_OK;
	}

	os_memset(&s_prro, 0, sizeof(s_prro));
	prro_hal_init(&s_prro.hal);
	s_prro_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_prro_driver_deinit(void)
{
	if (!s_prro_driver_is_init) {
		return BK_OK;
	}
	os_memset(&s_prro, 0, sizeof(s_prro));
	s_prro_driver_is_init = false;

	return BK_OK;
}

bk_err_t bk_prro_set_privilege_attribute(prro_dev_t dev, prro_privilege_type_t privilege_type)
{
	PRRO_RETURN_ON_DRIVER_NOT_INIT();
	prro_hal_set_privilege_attribute(&s_prro.hal, dev, privilege_type);

	return BK_OK;
}

bk_err_t bk_prro_set_secure_attribute(prro_dev_t dev, prro_secure_type_t secure_type)
{
	PRRO_RETURN_ON_DRIVER_NOT_INIT();
	prro_hal_set_secure_attribute(&s_prro.hal, dev, secure_type);

	return BK_OK;
}

