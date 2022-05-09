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

#include <os/os.h>
#include <driver/i2c.h>
#include "cli.h"

#define EEPROM_DEV_ADDR          0x50
#define EEPROM_MEM_ADDR          0x10
#define I2C_SLAVE_ADDR           0x73
#define I2C_WRITE_WAIT_MAX_MS    (500)
#define I2C_READ_WAIT_MAX_MS     (500)
#define CAMERA_DEV_ADDR          (0x21)

static void cli_i2c_help(void)
{
	CLI_LOGI("i2c_driver init\r\n");
	CLI_LOGI("i2c_driver deinit\r\n");
	CLI_LOGI("i2c {id} init\r\n");
	CLI_LOGI("i2c {id} deinit\r\n");
	CLI_LOGI("i2c {id} memory_write {data_size}\r\n");
	CLI_LOGI("i2c {id} memory_read {data_size}\r\n");
}

static void cli_i2c_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_i2c_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_i2c_driver_init());
		CLI_LOGI("i2c driver init\n");
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_i2c_driver_deinit());
		CLI_LOGI("i2c driver deinit\n");
	} else {
		cli_i2c_help();
		return;
	}
}

static void cli_i2c_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_i2c_help();
		return;
	}

	uint32_t i2c_id = os_strtoul(argv[1], NULL, 10);

	if (os_strcmp(argv[2], "init") == 0) {
		i2c_config_t i2c_cfg = {0};
		i2c_cfg.baud_rate = 400000;
		i2c_cfg.addr_mode = I2C_ADDR_MODE_7BIT;
		i2c_cfg.slave_addr = I2C_SLAVE_ADDR;

		BK_LOG_ON_ERR(bk_i2c_init(i2c_id, &i2c_cfg));
		CLI_LOGI("i2c(%d) init\n", i2c_id);
	} else if (os_strcmp(argv[2], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_i2c_deinit(i2c_id));
		CLI_LOGI("i2c(%d) deinit\n", i2c_id);
	} else if (os_strcmp(argv[2], "master_write") == 0) {
		uint8_t data_buf[10] = {0};
		for (uint32_t i = 0; i < 10; i++) {
			data_buf[i] = i & 0xff;
		}
		BK_LOG_ON_ERR(bk_i2c_master_write(i2c_id, EEPROM_DEV_ADDR, data_buf, 10, I2C_WRITE_WAIT_MAX_MS));
	} else if (os_strcmp(argv[2], "memory_write") == 0) {
		uint32_t buf_len = os_strtoul(argv[3], NULL, 10);
		uint8_t *data_buf = os_malloc(buf_len);
		BK_ASSERT(NULL != data_buf);
		for (uint32_t i = 0; i < buf_len; i++) {
			data_buf[i] = (i + 1) & 0xff;
		}
		uint32_t dev_addr = os_strtoul(argv[4], NULL, 16);
		i2c_mem_param_t mem_param = {0};
		mem_param.dev_addr = dev_addr;
		mem_param.mem_addr = EEPROM_MEM_ADDR;
		mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
		mem_param.data = data_buf;
		mem_param.data_size = buf_len;
		mem_param.timeout_ms = I2C_WRITE_WAIT_MAX_MS;
		BK_LOG_ON_ERR(bk_i2c_memory_write(i2c_id, &mem_param));
		if (data_buf) {
			os_free(data_buf);
			data_buf = NULL;
		}
		CLI_LOGI("i2c(%d) memory_write buf_len:%d\r\n", i2c_id, buf_len);
	} else if (os_strcmp(argv[2], "memory_read") == 0) {
		uint32_t buf_len = os_strtoul(argv[3], NULL, 10);
		uint8_t *data_buf = os_zalloc(buf_len);
		BK_ASSERT(NULL != data_buf);
		i2c_mem_param_t mem_param = {0};
		mem_param.dev_addr = EEPROM_DEV_ADDR;
		mem_param.mem_addr = EEPROM_MEM_ADDR;
		mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
		mem_param.data = data_buf;
		mem_param.data_size = buf_len;
		mem_param.timeout_ms = I2C_WRITE_WAIT_MAX_MS;
		BK_LOG_ON_ERR(bk_i2c_memory_read(i2c_id, &mem_param));
		for (uint32_t i = 0; i < buf_len; i++) {
			CLI_LOGI("i2c_read_buf[%d]=%x\r\n", i, data_buf[i]);
		}
		if (data_buf) {
			os_free(data_buf);
			data_buf = NULL;
		}
		CLI_LOGI("i2c(%d) memory_read buf_len:%d\r\n", i2c_id, buf_len);
	} else {
		cli_i2c_help();
		return;
	}
}

#define I2C_CMD_CNT (sizeof(s_i2c_commands) / sizeof(struct cli_command))
static const struct cli_command s_i2c_commands[] = {
	{"i2c_driver", "i2c_driver {init|deinit}", cli_i2c_driver_cmd},
	{"i2c", "i2c {init|write|read}", cli_i2c_cmd},
};

int cli_i2c_init(void)
{
	BK_LOG_ON_ERR(bk_i2c_driver_init());
	return cli_register_commands(s_i2c_commands, I2C_CMD_CNT);
}

