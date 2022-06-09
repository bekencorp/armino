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

#include <stdlib.h>
#include "cli.h"
#include "flash_namespace_value.h"



extern void BkQueryImageCmdHandler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv );
extern void BkApplyUpdateCmdHandler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv );
extern void ChipTest(void);


void matter_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{ 
    beken_thread_t matter_thread_handle = NULL;

    os_printf("start matter\r\n");
    rtos_create_thread(&matter_thread_handle,
							 BEKEN_DEFAULT_WORKER_PRIORITY,
							 "matter",
							 (beken_thread_function_t)ChipTest,
							 8192,
							 0);
    
}



#define NAMEKEY_CMD_CNT              (sizeof(s_nameKey_commands) / sizeof(struct cli_command))
static const struct cli_command s_nameKey_commands[] =
{
    {
        "bk_write_data_test", "bk_write_data_test", bk_write_data_test
    },
    {
        "bk_read_data_test", "bk_read_data_test", bk_read_data_test
    },
    {
        "bk_erase_namespace_test", "bk_erase_namespace_test", bk_erase_namespace_test
    },
    {
        "bk_erase_name_test", "bk_erase_name_test", bk_erase_name_test
    },
    {
        "bk_name_data_exits", "bk_name_data_exits", bk_name_data_exits
    },
    {
        "bk_erase_all_test", "bk_erase_all_test", bk_erase_all_test
    },
    {
        "matter", "matter", matter_test
    },
    {
        "BkQueryImageCmdHandler", "BkQueryImageCmdHandler", BkQueryImageCmdHandler
    },
    {
        "BkApplyUpdateCmdHandler", "BkApplyUpdateCmdHandler", BkApplyUpdateCmdHandler
    },
};


int cli_matter_init(void)
{
    return cli_register_commands(s_nameKey_commands, NAMEKEY_CMD_CNT);
}


