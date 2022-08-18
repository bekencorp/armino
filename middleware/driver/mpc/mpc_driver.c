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
#include <driver/mpc.h>
#include <os/os.h>
#include <os/mem.h>
#include "mpc_driver.h"
#include "mpc_hal.h"

typedef struct {
	mpc_hal_t hal;
} mpc_driver_t;

#define MPC_RETURN_ON_DRIVER_NOT_INIT() do {\
	if (!s_mpc_driver_is_init) {\
		MPC_LOGE("MPC driver not init\r\n");\
		return BK_ERR_MPC_DRIVER_NOT_INIT;\
	}\
} while(0)

static mpc_driver_t s_mpc[MPC_DEV_MAX] = {0};
static bool s_mpc_driver_is_init = false;

bk_err_t bk_mpc_driver_init(void)
{
	if (s_mpc_driver_is_init) {
		return BK_OK;
	}

	os_memset(&s_mpc, 0, sizeof(s_mpc));
	for (uint32_t dev_id = 0; dev_id < MPC_DEV_MAX; dev_id++) {
		s_mpc[dev_id].hal.dev = dev_id;
		mpc_hal_init(&s_mpc[dev_id].hal);
	}
	s_mpc_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_mpc_driver_deinit(void)
{
	if (!s_mpc_driver_is_init) {
		return BK_OK;
	}
	os_memset(&s_mpc, 0, sizeof(s_mpc));
	s_mpc_driver_is_init = false;

	return BK_OK;
}

uint32_t bk_mpc_get_block_size(mpc_dev_t dev)
{
	return mpc_hal_get_block_size(&s_mpc[dev].hal);
}

uint32_t bk_mpc_get_max_block_index(mpc_dev_t dev)
{
	return mpc_hal_get_max_block_index(&s_mpc[dev].hal);
}

bk_err_t bk_mpc_set_secure_attribute(mpc_dev_t dev, uint32_t mem_addr_offset, uint32_t block_num, mpc_block_secure_type_t secure_type)
{
	MPC_RETURN_ON_DRIVER_NOT_INIT();

	uint32_t block_offset = 0;
	uint32_t block_index = 0;
	uint32_t block_size = bk_mpc_get_block_size(dev);
	bk_err_t ret = BK_OK;
	mpc_hal_t *hal = &s_mpc[dev].hal;

	if (mem_addr_offset % block_size) {
		MPC_LOGW("mem_addr_offset not block size:%d aligned\r\n", block_size);
	}

	/* enable block index auto increase */
	mpc_hal_enable_auto_increase(hal);

	/* set mpc block index */
	block_offset = mem_addr_offset / (bk_mpc_get_block_size(dev));
	block_index = block_offset / MPC_BLOCK_LUT_MAX_BIT_NUM;
	if (block_index > bk_mpc_get_max_block_index(dev)) {
		MPC_LOGW("block index:%d is out of range\r\n", block_index);
		return BK_ERR_MPC_BLOCK_INDEX_OUT_OF_RANGE;
	} else {
		mpc_hal_set_block_index(hal, block_index);
	}

	/* set block lut */
	ret = mpc_hal_config_block_lut(hal, block_offset, block_num, secure_type);

	/* disable block index auto increase */
	mpc_hal_disable_auto_increase(hal);

	return ret;
}

bk_err_t bk_mpc_lockdown(mpc_dev_t dev)
{
	MPC_RETURN_ON_DRIVER_NOT_INIT();

	mpc_hal_enable_sec_lock(&s_mpc[dev].hal);

	return BK_OK;
}

