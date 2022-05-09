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

#include <components/log.h>
#include "bk_api_i2c.h"

#define TAG "i2c_example"
#define I2C_EXAMPLE_ID         I2C_ID_1
#define I2C_EXAMPLE_BUF_LEN    10
#define EEPROM_DEV_ADDR        0x50
#define EEPROM_MEM_ADDR        0x10

static bk_err_t i2c_example_eeprom_write(void)
{
	i2c_mem_param_t mem_param = {0};
	uint8_t write_data[I2C_EXAMPLE_BUF_LEN] = {0};

	for (uint32_t i = 0; i < I2C_EXAMPLE_BUF_LEN; i++) {
		write_data[i] = (i + 1) & 0xff;
	}

	mem_param.dev_addr = EEPROM_DEV_ADDR;
	mem_param.mem_addr = EEPROM_MEM_ADDR;
	mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
	mem_param.data = write_data;
	mem_param.data_size = I2C_EXAMPLE_BUF_LEN;
	mem_param.timeout_ms = I2C_WRITE_WAIT_MAX_MS;
	BK_LOG_ON_ERR(bk_i2c_memory_write(I2C_EXAMPLE_ID, &mem_param));
	BK_LOGI(TAG, "i2c eeprom write data.\r\n");

	return BK_OK;
}

static bk_err_t i2c_example_eeprom_read(void)
{
	i2c_mem_param_t mem_param = {0};
	uint8_t read_data[I2C_EXAMPLE_BUF_LEN] = {0};

	mem_param.dev_addr = EEPROM_DEV_ADDR;
	mem_param.mem_addr = EEPROM_MEM_ADDR;
	mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
	mem_param.data = read_data;
	mem_param.data_size = I2C_EXAMPLE_BUF_LEN;
	mem_param.timeout_ms = I2C_WRITE_WAIT_MAX_MS;
	BK_LOG_ON_ERR(bk_i2c_memory_read(I2C_EXAMPLE_ID, &mem_param));

	for (uint32_t i = 0; i < I2C_EXAMPLE_BUF_LEN; i++) {
		BK_LOGI(TAG, "i2c eeprom_read_buf[%d]=%x\r\n", i, read_data[i]);
	}
	return BK_OK;
}

int main(void)
{
	BK_LOG_ON_ERR(bk_i2c_driver_init());

	i2c_config_t i2c_cfg = {0};
	i2c_cfg.baud_rate = I2C_DEFAULT_BAUD_RATE;
	i2c_cfg.addr_mode = I2C_ADDR_MODE_7BIT;
	i2c_cfg.slave_addr = I2C_SLAVE_ADDR;
	BK_LOG_ON_ERR(bk_i2c_init(I2C_EXAMPLE_ID, &i2c_cfg));

	BK_LOG_ON_ERR(i2c_example_eeprom_write());
	BK_LOG_ON_ERR(i2c_example_eeprom_read());

	return 0;
}
