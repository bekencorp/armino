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
#include "cli.h"
#include "flashdb.h"

#ifdef FDB_USING_KVDB
static struct fdb_kvdb kvdb = { 0 };
static beken_mutex_t g_flashdb_kvdb_demo_mutex = NULL;

static uint32_t boot_count = 0;
static time_t boot_time[10] = {0, 1, 2, 3};
/* default KV nodes */
static struct fdb_default_kv_node default_kv_table[] = {
        {"username", "armink", 0}, /* string KV */
        {"password", "123456", 0}, /* string KV */
        {"boot_count", &boot_count, sizeof(boot_count)}, /* int type KV */
        {"boot_time", &boot_time, sizeof(boot_time)},    /* int array type KV */
};
#endif

#ifdef FDB_USING_TSDB
static struct fdb_tsdb tsdb = { 0 };
static beken_mutex_t g_flashdb_tsdb_demo_mutex = NULL;

extern time_t os_time(void);
static fdb_time_t get_time(void)
{
    return os_time();
}
#endif

static void lock(fdb_db_t db)
{
    rtos_lock_mutex((beken_mutex_t*)db->user_data);
}

static void unlock(fdb_db_t db)
{
    rtos_unlock_mutex((beken_mutex_t*)db->user_data);
}

static void _flashdb_init(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    fdb_err_t result;

#ifdef FDB_USING_KVDB
    struct fdb_default_kv default_kv;

    default_kv.kvs = default_kv_table;
    default_kv.num = sizeof(default_kv_table) / sizeof(default_kv_table[0]);
    /* set the lock and unlock function if you want */
    rtos_init_mutex(&g_flashdb_kvdb_demo_mutex);
    fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_LOCK, (void *)lock);
    fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_UNLOCK, (void *)unlock);

    result = fdb_kvdb_init(&kvdb, "env", FLASHDB_KVDB_PATH, &default_kv, &g_flashdb_kvdb_demo_mutex);
    if (result != FDB_NO_ERR) 
    {
        CLI_LOGE("[%s][%d] kvdb init fail:%d\r\n", __FUNCTION__, __LINE__);
    }
    else
    {
        CLI_LOGI("kvdb init success, start addr:0x%x, size:%d\r\n", CONFIG_FLASHDB_KVDB_START_ADDR, CONFIG_FLASHDB_KVDB_SIZE);
    }
#endif

#ifdef FDB_USING_TSDB
    /* set the lock and unlock function if you want */
    rtos_init_mutex(&g_flashdb_tsdb_demo_mutex);
    fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_SET_LOCK, (void *)lock);
    fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_SET_UNLOCK, (void *)unlock);

    result = fdb_tsdb_init(&tsdb, "log", FLASHDB_TSDB_PATH, get_time, 128, &g_flashdb_tsdb_demo_mutex);
    if (result != FDB_NO_ERR) 
    {
        CLI_LOGE("[%s][%d] tsdb init fail:%d\r\n", __FUNCTION__, __LINE__);
    }
    else
    {
        CLI_LOGI("tsdb init success, start addr:0x%x, size:%d\r\n", CONFIG_FLASHDB_TSDB_START_ADDR, CONFIG_FLASHDB_TSDB_SIZE);
    }
#endif
}

#ifdef FDB_USING_KVDB
static void _flashdb_kvdb_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    extern void kvdb_basic_sample(fdb_kvdb_t kvdb);
    extern void kvdb_type_string_sample(fdb_kvdb_t kvdb);
    extern void kvdb_type_blob_sample(fdb_kvdb_t kvdb);
    
    /* run basic KV samples */
    kvdb_basic_sample(&kvdb);
    /* run string KV samples */
    kvdb_type_string_sample(&kvdb);
    /* run blob KV samples */
    kvdb_type_blob_sample(&kvdb);
}
#endif

#ifdef FDB_USING_KVDB
static void _flashdb_tsdb_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    /* run TSDB sample */
    extern void tsdb_sample(fdb_tsdb_t tsdb);
    tsdb_sample(&tsdb);
}
#endif


#define FLASHDB_CMD_CNT (sizeof(s_flashdb_commands) / sizeof(struct cli_command))
static const struct cli_command s_flashdb_commands[] = {
    {"flashdb_init", "flashdb_init", _flashdb_init},
    
#ifdef FDB_USING_KVDB
    {"flashdb_kvdb", "flashdb_kvdb test", _flashdb_kvdb_test},
#endif

#ifdef FDB_USING_TSDB
    {"flashdb_tsdb", "flashdb_tsdb test", _flashdb_tsdb_test},
#endif
};

int cli_flashdb_init(void)
{
    return cli_register_commands(s_flashdb_commands, FLASHDB_CMD_CNT);
}


