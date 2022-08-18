// Copyright 2022-2023 Beken
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

#include "components/log.h"
#include "driver/mpc.h"
#include "bk_mpc.h"
#include "cmsis.h"

#define TAG "mpc"

/* TODO peter
 *
 * Temporarily configure the 50% of MPC memory for SPE, need to update it
 * as we gain more understanding about TFM memory usage.
 *
 * Also we may make it configuable in Armino.
 **/

int bk_mpc_cfg(void)
{
    mpc_alloc_scheme_t alloc_schemes[] = MPC_ALLOC_SCHEMES;
    mpc_dev_t dev;
    int schem_idx;

    BK_LOG_ON_ERR(bk_mpc_driver_init());

    for (schem_idx = 0; schem_idx < ARRAY_SIZE(alloc_schemes); schem_idx++) {
        uint32_t s_block_num;
        uint32_t ns_block_num;
        uint32_t ns_addr_offset;
        uint32_t ns_addr_num;
        uint32_t block_sz;
        uint32_t max_block_num;
        int scheme;

        dev = alloc_schemes[schem_idx].dev;
        scheme = alloc_schemes[schem_idx].bytes_for_spe;
        block_sz = bk_mpc_get_block_size(dev);
        max_block_num = bk_mpc_get_max_block_index(dev);

        if (scheme == MPC_ALLOC_SCHEME_ALL)
            s_block_num = max_block_num;
        else if (scheme == MPC_ALLOC_SCHEME_NONE)
            s_block_num = 0;
        else if (scheme == MPC_ALLOC_SCHEME_HALF)
            s_block_num = max_block_num >> 1;
        else
            s_block_num = scheme/block_sz;

        if (s_block_num > 0)
            BK_LOG_ON_ERR(bk_mpc_set_secure_attribute(dev, 0, s_block_num, MPC_BLOCK_SECURE));

        ns_block_num = max_block_num - s_block_num;
        ns_addr_offset = s_block_num * block_sz;
        if (ns_block_num > 0)
            BK_LOG_ON_ERR(bk_mpc_set_secure_attribute(dev, ns_addr_offset, ns_block_num, MPC_BLOCK_NON_SECURE));

        BK_LOGI(TAG, "dev=%d s blocks=%d ns offset=%d blocks=%d\n", dev, s_block_num, ns_addr_offset, ns_block_num);
    }

    for (dev = 0; dev < MPC_DEV_MAX; dev++) {
        BK_LOG_ON_ERR(bk_mpc_lockdown(dev));
    }

    __DSB();
    __ISB();

    return BK_OK;
}

int bk_mpc_enable_interrupt(void)
{
    //TODO
    return BK_OK;
}

