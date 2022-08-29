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
#include <driver/qspi_flash.h>
#include "qspi_hal.h"
#include <driver/int.h>
#include <os/mem.h>
#include <driver/qspi.h>
#include "qspi_driver.h"
#include "qspi_hal.h"
#include "qspi_statis.h"
#include <driver/trng.h>

#define FLASH_READ_ID_CMD          0x9F
#define FLASH_WR_S0_S7_CMD         0x1
#define FLASH_WR_S8_S15_CMD        0x31
#define FLASH_RD_S0_S7_CMD         0x5
#define FLASH_RD_S8_S15_CMD        0x35
#define FLASH_WR_EN_CMD            0x6
#define FLASH_WR_CMD               0x2
#define FLASH_RD_CMD               0x3
#define FLASH_QUAD_WR_CMD          0x32
#define FLASH_QUAD_RD_CMD          0xeb
#define FLASH_ERASE_SECTOR_CMD     0x20

#define FLASH_STATUS_REG_SIZE      1
#define FLASH_READ_ID_SIZE         4
#define QSPI_FIFO_LEN_MAX          256
#define FLASH_QE_DATA              BIT(1)
#define FLASH_PROTECT_NONE_DATA    0
#define QSPI_CMD1_LEN              8

static void bk_qspi_flash_wait_wip_done(void);

bk_err_t bk_qspi_flash_init(void)
{
	qspi_config_t config = {0};
	config.src_clk = QSPI_SCLK_480M;
	config.src_clk_div = 0x4;
	config.clk_div = 0x2;
	BK_LOG_ON_ERR(bk_qspi_init(&config));
	return BK_OK;
}

bk_err_t bk_qspi_flash_deinit(void)
{
	BK_LOG_ON_ERR(bk_qspi_deinit());
	return BK_OK;
}

static void bk_qspi_flash_wren(void) {
	qspi_cmd_t wren_cmd = {0};

	wren_cmd.device = QSPI_FLASH;
	wren_cmd.wire_mode = QSPI_1WIRE;
	wren_cmd.work_mode = INDIRECT_MODE;
	wren_cmd.op = QSPI_WRITE;
	wren_cmd.cmd = FLASH_WR_EN_CMD;
	wren_cmd.data_len = 0;

	BK_LOG_ON_ERR(bk_qspi_command(&wren_cmd));
}

uint32_t bk_qspi_flash_read_s0_s7(void)
{
	qspi_cmd_t read_status_cmd = {0};
	uint32_t status_reg_data = 0;

	read_status_cmd.device = QSPI_FLASH;
	read_status_cmd.wire_mode = QSPI_1WIRE;
	read_status_cmd.work_mode = INDIRECT_MODE;
	read_status_cmd.op = QSPI_READ;
	read_status_cmd.cmd = FLASH_RD_S0_S7_CMD;
	read_status_cmd.data_len = FLASH_STATUS_REG_SIZE;

	BK_LOG_ON_ERR(bk_qspi_command(&read_status_cmd));
	bk_qspi_read(&status_reg_data, FLASH_STATUS_REG_SIZE);
	QSPI_LOGD("[%s]: status_reg_data = 0x%x.\n", __func__, (uint8_t)status_reg_data);

	return status_reg_data;
}

uint32_t bk_qspi_flash_read_s8_s15(void)
{
	qspi_cmd_t read_status_cmd = {0};
	uint32_t status_reg_data = 0;

	read_status_cmd.device = QSPI_FLASH;
	read_status_cmd.wire_mode = QSPI_1WIRE;
	read_status_cmd.work_mode = INDIRECT_MODE;
	read_status_cmd.op = QSPI_READ;
	read_status_cmd.cmd = FLASH_RD_S8_S15_CMD;
	read_status_cmd.data_len = FLASH_STATUS_REG_SIZE;

	BK_LOG_ON_ERR(bk_qspi_command(&read_status_cmd));
	bk_qspi_read(&status_reg_data, FLASH_STATUS_REG_SIZE);
	QSPI_LOGD("[%s]: status_reg_data = 0x%x.\n", __func__, (uint8_t)status_reg_data);

	return status_reg_data;
}

bk_err_t bk_qspi_flash_write_s0_s7(uint32_t status_reg_data)
{
	qspi_cmd_t write_status_cmd = {0};

	bk_qspi_flash_wren();
	write_status_cmd.device = QSPI_FLASH;
	write_status_cmd.wire_mode = QSPI_1WIRE;
	write_status_cmd.work_mode = INDIRECT_MODE;
	write_status_cmd.op = QSPI_WRITE;
	write_status_cmd.cmd = (status_reg_data << QSPI_CMD1_LEN) | FLASH_WR_S0_S7_CMD;
	write_status_cmd.data_len = FLASH_STATUS_REG_SIZE;

	BK_LOG_ON_ERR(bk_qspi_command(&write_status_cmd));
	QSPI_LOGI("[%s]: status_reg_data to be writen is 0x%x.\n", __func__, (uint8_t)status_reg_data);
	bk_qspi_flash_wait_wip_done();

	return BK_OK;
}

bk_err_t bk_qspi_flash_write_s8_s15(uint32_t status_reg_data)
{
	qspi_cmd_t write_status_cmd = {0};
	bk_qspi_flash_wren();
	write_status_cmd.device = QSPI_FLASH;
	write_status_cmd.wire_mode = QSPI_1WIRE;
	write_status_cmd.work_mode = INDIRECT_MODE;
	write_status_cmd.op = QSPI_WRITE;
	write_status_cmd.cmd = (status_reg_data << QSPI_CMD1_LEN) | FLASH_WR_S8_S15_CMD;
	write_status_cmd.data_len = FLASH_STATUS_REG_SIZE;

	BK_LOG_ON_ERR(bk_qspi_command(&write_status_cmd));
	QSPI_LOGI("[%s]: status_reg_data to be writen is 0x%x.\n", __func__, (uint8_t)status_reg_data);
	bk_qspi_flash_wait_wip_done();

	return BK_OK;
}

extern void delay_ms(UINT32 ms_count);
static void bk_qspi_flash_wait_wip_done(void)
{
	uint32_t status_reg_data = 0;

	for(int i = 0; i < 100; i++) {
		status_reg_data = bk_qspi_flash_read_s0_s7();
		if(0 == (status_reg_data & BIT(0))) {
			break;
		}
		delay_ms(20);
	}
}

bk_err_t bk_qspi_flash_erase_sector(uint32_t addr)
{
	qspi_cmd_t erase_sector_cmd = {0};

	bk_qspi_flash_wren();

	erase_sector_cmd.device = QSPI_FLASH;
	erase_sector_cmd.wire_mode = QSPI_1WIRE;
	erase_sector_cmd.work_mode = INDIRECT_MODE;
	erase_sector_cmd.op = QSPI_WRITE;
	erase_sector_cmd.cmd = FLASH_ERASE_SECTOR_CMD;
	erase_sector_cmd.addr = addr;

	BK_LOG_ON_ERR(bk_qspi_command(&erase_sector_cmd));
	bk_qspi_flash_wait_wip_done();

	return BK_OK;
}

bk_err_t bk_qspi_flash_single_page_program(uint32_t addr, const void *data, uint32_t size)
{
	qspi_cmd_t page_program_cmd = {0};
	uint32_t cmd_data_len = 0;
	uint32_t *cmd_data = (uint32_t *)data;

	page_program_cmd.device = QSPI_FLASH;
	page_program_cmd.wire_mode = QSPI_1WIRE;
	page_program_cmd.work_mode = INDIRECT_MODE;
	page_program_cmd.op = QSPI_WRITE;
	page_program_cmd.cmd = FLASH_WR_CMD;

	while(0 < size) {
		bk_qspi_flash_wren();

		cmd_data_len = (size < QSPI_FIFO_LEN_MAX) ? size : QSPI_FIFO_LEN_MAX;
		bk_qspi_write(cmd_data, cmd_data_len);
		page_program_cmd.addr = addr;
		page_program_cmd.data_len = cmd_data_len;
		BK_LOG_ON_ERR(bk_qspi_command(&page_program_cmd));
		addr += cmd_data_len;
		cmd_data += cmd_data_len;
		size -= cmd_data_len;

		bk_qspi_flash_wait_wip_done();
	}

	return BK_OK;
}

bk_err_t bk_qspi_flash_single_read(uint32_t addr, void *data, uint32_t size)
{
	qspi_cmd_t single_read_cmd = {0};
	uint32_t cmd_data_len = 0;
	uint32_t *cmd_data = data;

	single_read_cmd.device = QSPI_FLASH;
	single_read_cmd.wire_mode = QSPI_1WIRE;
	single_read_cmd.work_mode = INDIRECT_MODE;
	single_read_cmd.op = QSPI_READ;
	single_read_cmd.cmd = FLASH_RD_CMD;

	while(0 < size) {
		cmd_data_len = (size < QSPI_FIFO_LEN_MAX) ? size : QSPI_FIFO_LEN_MAX;
		single_read_cmd.addr = addr;
		single_read_cmd.data_len = cmd_data_len;
		BK_LOG_ON_ERR(bk_qspi_command(&single_read_cmd));
		bk_qspi_read(cmd_data, cmd_data_len);

		addr += cmd_data_len;
		cmd_data += cmd_data_len;
		size -= cmd_data_len;
	}
	return BK_OK;
}

bk_err_t bk_qspi_flash_quad_page_program(uint32_t addr, const void *data, uint32_t size)
{
	qspi_cmd_t page_program_cmd = {0};
	uint32_t cmd_data_len = 0;
	uint32_t *cmd_data = (uint32_t *)data;

	page_program_cmd.device = QSPI_FLASH;
	page_program_cmd.wire_mode = QSPI_4WIRE;
	page_program_cmd.work_mode = INDIRECT_MODE;
	page_program_cmd.op = QSPI_WRITE;
	page_program_cmd.cmd = FLASH_QUAD_WR_CMD;

	while(0 < size) {
		bk_qspi_flash_wren();

		cmd_data_len = (size < QSPI_FIFO_LEN_MAX) ? size : QSPI_FIFO_LEN_MAX;
		bk_qspi_write(cmd_data, cmd_data_len);
		page_program_cmd.addr = addr;
		page_program_cmd.data_len = cmd_data_len;
		BK_LOG_ON_ERR(bk_qspi_command(&page_program_cmd));

		addr += cmd_data_len;
		cmd_data += cmd_data_len;
		size -= cmd_data_len;

		bk_qspi_flash_wait_wip_done();
	}

	return BK_OK;
}

bk_err_t bk_qspi_flash_quad_read(uint32_t addr, void *data, uint32_t size)
{
	qspi_cmd_t quad_read_cmd = {0};
	uint32_t cmd_data_len = 0;
	uint32_t *cmd_data = data;

	quad_read_cmd.device = QSPI_FLASH;
	quad_read_cmd.wire_mode = QSPI_4WIRE;
	quad_read_cmd.work_mode = INDIRECT_MODE;
	quad_read_cmd.op = QSPI_READ;
	quad_read_cmd.cmd = FLASH_QUAD_RD_CMD;
	quad_read_cmd.dummy_cycle = 4;

	while(0 < size) {
		cmd_data_len = (size < QSPI_FIFO_LEN_MAX) ? size : QSPI_FIFO_LEN_MAX;
		quad_read_cmd.addr = addr;
		quad_read_cmd.data_len = cmd_data_len;
		BK_LOG_ON_ERR(bk_qspi_command(&quad_read_cmd));
		bk_qspi_read(cmd_data, cmd_data_len);

		addr += cmd_data_len;
		cmd_data += cmd_data_len;
		size -= cmd_data_len;
	}
	return BK_OK;
}

bk_err_t bk_qspi_flash_write(uint32_t base_addr, const void *data, uint32_t size)
{
	BK_LOG_ON_ERR(bk_qspi_flash_quad_page_program(base_addr, data, size));
	return BK_OK;
}

bk_err_t bk_qspi_flash_read(uint32_t base_addr, void *data, uint32_t size)
{
	BK_LOG_ON_ERR(bk_qspi_flash_quad_read(base_addr, data, size));
	return BK_OK;
}

void bk_qspi_flash_quad_enable(void) {
	uint32_t status_reg_data = 0;

	status_reg_data = bk_qspi_flash_read_s8_s15();
	status_reg_data |= FLASH_QE_DATA;
	bk_qspi_flash_write_s8_s15(status_reg_data);
}

void bk_qspi_flash_set_protect_none(void) {
	uint32_t status_reg_data = 0;

	status_reg_data = bk_qspi_flash_read_s0_s7();
	status_reg_data &= FLASH_PROTECT_NONE_DATA;
	bk_qspi_flash_write_s0_s7(status_reg_data);
}

uint32_t bk_qspi_flash_read_id(void) {
	qspi_cmd_t read_id_cmd = {0};
	uint32_t read_id_data = 0;

	read_id_cmd.device = QSPI_FLASH;
	read_id_cmd.wire_mode = QSPI_1WIRE;
	read_id_cmd.work_mode = INDIRECT_MODE;
	read_id_cmd.op = QSPI_READ;
	read_id_cmd.cmd = FLASH_READ_ID_CMD;
	read_id_cmd.data_len = FLASH_READ_ID_SIZE;

	BK_LOG_ON_ERR(bk_qspi_command(&read_id_cmd));
	
	bk_qspi_read(&read_id_data, FLASH_READ_ID_SIZE);

	return read_id_data;
}

void qspi_flash_test_case(uint32_t base_addr, void *data, uint32_t size)
{
	uint32_t read_id = 0;
	uint32_t *read_data = (uint32_t *)os_zalloc(size);
	uint32_t *origin_data = (uint32_t *)data;
	if (read_data == NULL) {
		QSPI_LOGE("send buffer malloc failed\r\n");
		return;
	}

	read_id = bk_qspi_flash_read_id();
	QSPI_LOGI("%s read_id = 0x%x\n", __func__, read_id);
	bk_qspi_flash_quad_enable();
	bk_qspi_flash_set_protect_none();

	bk_qspi_flash_erase_sector(base_addr);

	bk_qspi_flash_read(base_addr, read_data, size);
	for (int i = 0; i < size/4; i++) {
		if(read_data[i] != 0xFFFFFFFF) {
			QSPI_LOGI("[QUAD ERASE ERROR]: read_data[%d]=0x%x, should be 0xFFFFFFFF\n", i, read_data[i]);
		}
		QSPI_LOGD("[QUAD ERASE]: read_data[%d]=0x%x, should be 0xFFFFFFFF\n", i, read_data[i]);
	}

	bk_qspi_flash_write(base_addr, data, size);

	bk_qspi_flash_read(base_addr, read_data, size);
	for (int i = 0; i < size/4; i++) {
		if(read_data[i] != origin_data[i]) {
			QSPI_LOGI("[QUAD WRITE ERROR]: read_data[%d]=0x%x, origin data[%d]=0x%x\n", i, read_data[i], i, origin_data[i]);
		}
		QSPI_LOGD("[QUAD WRITE]: read_data[%d]=0x%x, origin data[%d]=0x%x\n", i, read_data[i], i, origin_data[i]);
	}

	bk_qspi_flash_erase_sector(base_addr);

	bk_qspi_flash_single_read(base_addr, read_data, size);
	for (int i = 0; i < size/4; i++) {
		if(read_data[i] != 0xFFFFFFFF) {
			QSPI_LOGI("[SINGLE ERASE ERROR]: read_data[%d]=0x%x, should be 0xFFFFFFFF\n", i, read_data[i]);
		}
		QSPI_LOGD("[SINGLE ERASE ERROR]: read_data[%d]=0x%x, should be 0xFFFFFFFF\n", i, read_data[i]);
	}

	bk_qspi_flash_single_page_program(base_addr, data, size);

	bk_qspi_flash_single_read(base_addr, read_data, size);
	for (int i = 0; i < size/4; i++) {
		if(read_data[i] != origin_data[i]) {
			QSPI_LOGI("[SINGLE WRITE ERROR]: read_data[%d]=0x%x, origin data[%d]=0x%x\n", i, read_data[i], i, origin_data[i]);
		}
		QSPI_LOGD("[SINGLE WRITE ERROR]: read_data[%d]=0x%x, origin data[%d]=0x%x\n", i, read_data[i], i, origin_data[i]);
	}

	if (read_data) {
		os_free(read_data);
		read_data = NULL;
	}
}

void test_qspi_flash(uint32_t base_addr, uint32_t buf_len)
{
	uint32_t *send_data = (uint32_t *)os_zalloc(buf_len);
//	uint32_t rand_val = bk_rand() % (0x100000000);

	if (send_data == NULL) {
		QSPI_LOGE("send buffer malloc failed\r\n");
		return;
	}
	for (int i = 0; i < (buf_len/4); i++) {
		send_data[i] = (0x03020100 + i*0x04040404) & 0xffffffff;
	}
	qspi_flash_test_case(base_addr, send_data, buf_len);

	if (send_data) {
		os_free(send_data);
		send_data = NULL;
	}
}
