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

#include <driver/flash.h>
#include <driver/flash_partition.h>
#include "cli.h"
#include "flash_driver.h"

static void cli_flash_help(void)
{
	CLI_LOGI("flash driver init\n");
	CLI_LOGI("flash_driver deinit\n");
	CLI_LOGI("flash {erase|write|read} [start_addr] [len]\n");
	CLI_LOGI("flash_partition show\n");
}

static void cli_flash_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_flash_help();
		return;
	}

	uint32_t start_addr = os_strtoul(argv[2], NULL, 16);
	uint32_t len = os_strtoul(argv[3], NULL, 10);

	if (os_strcmp(argv[1], "erase") == 0) {
		bk_flash_set_protect_type(FLASH_PROTECT_NONE);
		for (uint32_t addr = start_addr; addr < (start_addr + len); addr += FLASH_SECTOR_SIZE) {
			bk_flash_erase_sector(addr);
		}
		bk_flash_set_protect_type(FLASH_UNPROTECT_LAST_BLOCK);
	} else if (os_strcmp(argv[1], "read") == 0) {
		uint8_t buf[FLASH_PAGE_SIZE] = {0};
		for (uint32_t addr = start_addr; addr < (start_addr + len); addr += FLASH_PAGE_SIZE) {
			os_memset(buf, 0, FLASH_PAGE_SIZE);
			bk_flash_read_bytes(addr, buf, FLASH_PAGE_SIZE);
			CLI_LOGI("flash read addr:%x\r\n", addr);

			CLI_LOGI("dump read flash data:\r\n");
			for (uint32_t i = 0; i < 16; i++) {
				for (uint32_t j = 0; j < 16; j++) {
					os_printf("%02x ", buf[i * 16 + j]);
				}
				os_printf("\r\n");
			}
		}
	} else if (os_strcmp(argv[1], "write") == 0) {
		uint8_t buf[FLASH_PAGE_SIZE] = {0};
		for (uint32_t i = 0; i < FLASH_PAGE_SIZE; i++) {
			buf[i] = i;
		}
		bk_flash_set_protect_type(FLASH_PROTECT_NONE);
		for (uint32_t addr = start_addr; addr < (start_addr + len); addr += FLASH_PAGE_SIZE) {
			bk_flash_write_bytes(addr, buf, FLASH_PAGE_SIZE);
		}
		bk_flash_set_protect_type(FLASH_UNPROTECT_LAST_BLOCK);
	} else if (os_strcmp(argv[1], "get_id") == 0) {
		uint32_t flash_id = bk_flash_get_id();
		CLI_LOGI("flash_id:%x\r\n", flash_id);
	} else {
		cli_flash_help();
	}
}

static void cli_flash_partition_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_logic_partition_t *partition;

	if (os_strcmp(argv[1], "show") == 0) {
		for (bk_partition_t par= BK_PARTITION_BOOTLOADER; par <= BK_PARTITION_MAX; par++) {
			partition = bk_flash_partition_get_info(par);
			if (partition == NULL)
				continue;

			CLI_LOGI("%4d | %11s |  Dev:%d  | 0x%08lx | 0x%08lx |\r\n", par,
					partition->partition_description, partition->partition_owner,
					partition->partition_start_addr, partition->partition_length);
		}
	} else {
		cli_flash_help();
	}
}

#define FLASH_CMD_CNT (sizeof(s_flash_commands) / sizeof(struct cli_command))
static const struct cli_command s_flash_commands[] = {
	{"flash", "flash {erase|read|write} [start_addr] [len]", cli_flash_cmd},
	{"flash_partition", "flash_partition {show}", cli_flash_partition_cmd},
};

int cli_flash_init(void)
{
	BK_LOG_ON_ERR(bk_flash_driver_init());
	return cli_register_commands(s_flash_commands, FLASH_CMD_CNT);
}

