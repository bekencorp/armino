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

#include "sdio_host_hal.h"
#include <driver/hal/hal_sdio_host_types.h>

bk_err_t sdio_host_hal_init(sdio_host_hal_t *hal)
{
	hal->hw = (sdio_hw_t *)SDIO_LL_REG_BASE(hal->id);
	sdio_host_ll_init(hal->hw);
	return BK_OK;
}

bk_err_t sdio_host_hal_init_commad(sdio_host_hal_t *hal, const sdio_host_cmd_cfg_t *command)
{
	/* set command index */
	sdio_host_ll_set_cmd_index(hal->hw, command->cmd_index);

	/* set command argument */
	sdio_host_ll_set_send_cmd_argument(hal->hw, command->argument);

	/* set wait the slave respond timeout */
	sdio_host_ll_set_cmd_rsp_timeout(hal->hw, command->wait_rsp_timeout);

	/* 1. set sdio host need salve respond the command or not
	 * 2. set sdio host need salve respond a long command or short
	 * 3. set sdio host need to check the slave respond command crc or not
	 */
	if (command->response == SDIO_HOST_CMD_RSP_SHORT) {
		sdio_host_ll_set_cmd_rsp(hal->hw, true);
		sdio_host_ll_set_cmd_long_rsp(hal->hw, false);
		sdio_host_ll_set_cmd_crc_check(hal->hw, true);
	} else if (command->response == SDIO_HOST_CMD_RSP_LONG) {
		sdio_host_ll_set_cmd_rsp(hal->hw, true);
		sdio_host_ll_set_cmd_long_rsp(hal->hw, true);
		sdio_host_ll_set_cmd_crc_check(hal->hw, true);
	} else {
		sdio_host_ll_set_cmd_rsp(hal->hw, false);
		sdio_host_ll_set_cmd_long_rsp(hal->hw, false);
		sdio_host_ll_set_cmd_crc_check(hal->hw, false);
	}

	return BK_OK;
}

