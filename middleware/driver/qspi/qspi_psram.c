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

#include <driver/qspi.h>
#include <driver/qspi_psram.h>
#include "qspi_hal.h"

#define READ_ID_CMD            0x9F
#define ENTER_QUAD_MODE_CMD    0x35
#define EXIT_QUAD_MODE_CMD     0xF5
#define QUAD_WRITE_CMD         0x38
#define WRITE_CMD              0x02
#define FAST_READ_QUAD         0xEB
#define FAST_READ              0x0B
#define READ_CMD               0x03

bk_err_t bk_qspi_psram_init(void)
{
	qspi_config_t config = {0};
	config.src_clk = QSPI_SCLK_XTAL_26M;
	config.clk_div = 0x10;
	BK_LOG_ON_ERR(bk_qspi_init(&config));
	return BK_OK;
}

bk_err_t bk_qspi_psram_deinit(void)
{
	BK_LOG_ON_ERR(bk_qspi_deinit());
	return BK_OK;
}

bk_err_t bk_qspi_psram_enter_quad_mode(void)
{
	qspi_cmd_t enter_quad_cmd = {0};

	enter_quad_cmd.wire_mode = QSPI_1WIRE;
	enter_quad_cmd.work_mode = INDIRECT_MODE;
	enter_quad_cmd.op = QSPI_WRITE;
	enter_quad_cmd.cmd = ENTER_QUAD_MODE_CMD;

	BK_LOG_ON_ERR(bk_qspi_command(&enter_quad_cmd));
	return BK_OK;
}

bk_err_t bk_qspi_psram_exit_quad_mode(void)
{
	qspi_cmd_t exit_quad_cmd = {0};

	exit_quad_cmd.wire_mode = QSPI_4WIRE;
	exit_quad_cmd.work_mode = INDIRECT_MODE;
	exit_quad_cmd.op = QSPI_WRITE;
	exit_quad_cmd.cmd = EXIT_QUAD_MODE_CMD;

	BK_LOG_ON_ERR(bk_qspi_command(&exit_quad_cmd));
	return BK_OK;
}

bk_err_t bk_qspi_psram_quad_write(void)
{
	qspi_cmd_t quad_wr_cmd = {0};

	quad_wr_cmd.wire_mode = QSPI_4WIRE;
	quad_wr_cmd.work_mode = MEMORY_MAPPED_MODE;
	quad_wr_cmd.op = QSPI_WRITE;
	quad_wr_cmd.cmd = QUAD_WRITE_CMD;
	quad_wr_cmd.addr = 0;
	quad_wr_cmd.dummy_cycle = 0;

	BK_LOG_ON_ERR(bk_qspi_command(&quad_wr_cmd));
	return BK_OK;
}

bk_err_t bk_qspi_psram_single_write(void)
{
	qspi_cmd_t quad_wr_cmd = {0};

	quad_wr_cmd.wire_mode = QSPI_1WIRE;
	quad_wr_cmd.work_mode = MEMORY_MAPPED_MODE;
	quad_wr_cmd.op = QSPI_WRITE;
	quad_wr_cmd.cmd = WRITE_CMD;
	quad_wr_cmd.addr = 0;
	quad_wr_cmd.dummy_cycle = 0;

	BK_LOG_ON_ERR(bk_qspi_command(&quad_wr_cmd));
	return BK_OK;
}

bk_err_t bk_qspi_psram_quad_read(void)
{
	qspi_cmd_t qspi_rd_cmd = {0};

	qspi_rd_cmd.wire_mode = QSPI_4WIRE;
	qspi_rd_cmd.work_mode = MEMORY_MAPPED_MODE;
	qspi_rd_cmd.op = QSPI_READ;
	qspi_rd_cmd.cmd = FAST_READ_QUAD;
	qspi_rd_cmd.addr = 0;
	qspi_rd_cmd.dummy_cycle = 6;

	BK_LOG_ON_ERR(bk_qspi_command(&qspi_rd_cmd));
	return BK_OK;
}

bk_err_t bk_qspi_psram_single_read(void)
{
	qspi_cmd_t qspi_rd_cmd = {0};

	qspi_rd_cmd.wire_mode = QSPI_1WIRE;
	qspi_rd_cmd.work_mode = MEMORY_MAPPED_MODE;
	qspi_rd_cmd.op = QSPI_READ;
	qspi_rd_cmd.cmd = READ_CMD;
	qspi_rd_cmd.addr = 0;
	qspi_rd_cmd.dummy_cycle = 0;

	BK_LOG_ON_ERR(bk_qspi_command(&qspi_rd_cmd));
	return BK_OK;
}

bk_err_t bk_qspi_psram_write(uint32_t base_addr, const void *data, uint32_t size)
{
	BK_LOG_ON_ERR(qspi_hal_direct_write(base_addr, data, size));
	return BK_OK;
}

bk_err_t bk_qspi_psram_read(uint32_t base_addr, void *data, uint32_t size)
{
	BK_LOG_ON_ERR(qspi_hal_direct_read(base_addr, data, size));
	return BK_OK;
}

