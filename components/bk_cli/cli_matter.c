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

void bk_matter_data(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char help_str[] = "matter_data {read|write|erase} {namespace} {name} {str|BK_HEX:hex}";
    int err;
    if (argc >= 4 && 0==strcmp("read", argv[1]))
    {
        unsigned char buf[1000] = {0};
        long unsigned int buf_len = 0;
        err = bk_read_data(argv[2], argv[3], (char*)buf, 1000, &buf_len);
        if (kNoErr != err) {
            buf[buf_len] = '\0';
            bk_printf("[matter_data,error,%d] read %s %s %s(%lu)\r\n", err, argv[2], argv[3], buf, buf_len);
        } else {
            buf[buf_len] = '\0';
            bk_printf("[matter_data,sucess,%d] ", err, buf, buf_len, buf);
            for(int i=0;i<buf_len;i++){
                bk_printf("%02x",buf[i]);
            }
            bk_printf("(%d)",buf_len);
            bk_printf("\r\n");
            return;
        }
    }
    if (argc >= 5 && 0==strcmp("write", argv[1])) {
        char prefix[10] = {0};
        long unsigned int buf_len = 0;
        memcpy(prefix, argv[4], min(strlen(argv[4]), 9));
        prefix[7] = '\0';
        if(strlen(argv[4]) >= 9 && (0 == strcmp(prefix, "BK_HEX:"))) // BK_HEX:
        {
            buf_len = (strlen(argv[4]) - 7) / 2;
            unsigned char* buf = malloc(buf_len * sizeof(char));
            hexstr2bin(argv[4] + 7, buf, buf_len);
            err = bk_write_data(argv[2], argv[3], (char*)buf, buf_len);
            free(buf);
        } else { // str
            buf_len = strlen(argv[4]);
            err = bk_write_data(argv[2], argv[3], argv[4], buf_len);
        }

        if (kNoErr != err) {
            bk_printf("[matter_data,error,%d] write %s %s %s(%d)\r\n", err, argv[2], argv[3], argv[4], buf_len);
        } else {
            bk_printf("[matter_data,sucess,%d] write %s %s %s(%d)\r\n", err, argv[2], argv[3], argv[4], buf_len);
            return;
        }
    }
    if (argc >= 2 && 0==strcmp("erase", argv[1])) {
      if (argc == 2) {
        const char *namespaces[] = {"BEKEN0", "BEKEN1", "BEKEN2",
                                    "BEKEN3", "BEKEN4", "chip-config"};
        for (int i = 0; i < sizeof(namespaces) / sizeof(namespaces[0]); i++) {
          err = bK_clear_namespace(namespaces[i]);
          if (kNoErr != err) {
            bk_printf("[matter_data,error,%d] erase %s\r\n", err, namespaces[i]);
          } else {
            bk_printf("[matter_data,sucess,%d] erase %s\r\n", err, namespaces[i]);
          }
        }
      }
      if (argc == 3) {
        err = bK_clear_namespace(argv[2]);
        if (kNoErr != err) {
          bk_printf("[matter_data,error,%d] erase %s\r\n", err, argv[2]);
        } else {
          bk_printf("[matter_data,sucess,%d] erase %s\r\n", err, argv[2]);
        }
      }
      if (argc >= 4) {
        err = bk_clean_data(argv[2], argv[3]);
        if (kNoErr != err) {
          bk_printf("[matter_data,error,%d] erase %s %s\r\n", err, argv[2], argv[3]);
        } else {
          bk_printf("[matter_data,sucess,%d] erase %s %s\r\n", err, argv[2], argv[3]);
        }
      }
    }
    bk_printf("help:");
    bk_printf("%s", help_str);
    return;
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
    {
        "bk_matter_data", "matter_data {read|write|erase} {namespace} {name} {str|BK_HEX:hex}", bk_matter_data
    },
};


int cli_matter_init(void)
{
    return cli_register_commands(s_nameKey_commands, NAMEKEY_CMD_CNT);
}


