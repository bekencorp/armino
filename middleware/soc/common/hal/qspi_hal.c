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

#include "qspi_hal.h"
#include "qspi_ll.h"
#include <driver/hal/hal_spi_types.h>

static uint32_t s_current_sw_op = 0;

bk_err_t qspi_hal_init(qspi_hal_t *hal)
{
	hal->hw = (qspi_hw_t *)QSPI_LL_REG_BASE(hal->id);
	qspi_ll_init(hal->hw);
	return BK_OK;
}

bool qspi_hal_is_cur_sw_op_write_data(void)
{
	return (s_current_sw_op == QSPI_WRITE);
}

bool qspi_hal_is_cur_sw_op_read_data(void)
{
	return (s_current_sw_op == QSPI_READ);
}

bk_err_t qspi_hal_command(qspi_hal_t *hal, const qspi_cmd_t *cmd)
{
	s_current_sw_op = cmd->op;
	qspi_ll_init_command(hal->hw, cmd);

	return BK_OK;
}

bk_err_t qspi_hal_direct_write(uint32_t base_addr, const void *data, uint32_t size)
{
	qspi_ll_direct_write(base_addr, data, size);
	return BK_OK;
}

bk_err_t qspi_hal_direct_read(uint32_t base_addr, void *data, uint32_t size)
{
	qspi_ll_direct_read(base_addr, data, size);
	return BK_OK;
}

bk_err_t qspi_hal_io_write(qspi_hal_t *hal, const void *data, uint32_t size)
{
	qspi_ll_io_write(hal->hw, data, size);
	return BK_OK;
}

bk_err_t qspi_hal_io_read(qspi_hal_t *hal, void *data, uint32_t size)
{
	qspi_ll_io_read(hal->hw, data, size);
	return BK_OK;
}
