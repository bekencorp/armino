/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-17     armink       the first version
 */

#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_

#define FLASHDB_DEV_NAME             "flashdb0"
#define FLASHDB_KVDB_PATH            "fdb_kvdb1"
#define FLASHDB_TSDB_PATH            "fdb_tsdb1"
#define FAL_PART_HAS_TABLE_CFG

/* ===================== Flash device Configuration ========================= */
extern struct fal_flash_dev g_flashdb0;

/* flash device table */
#define FAL_FLASH_DEV_TABLE                                          \
{                                                                    \
    &g_flashdb0,                                                     \
}
/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG

#if defined(FDB_USING_KVDB) && (!defined(CONFIG_FLASHDB_KVDB_START_ADDR) || !defined(CONFIG_FLASHDB_KVDB_SIZE))
#error "Please defined CONFIG_FLASHDB_KVDB_START_ADDR or CONFIG_FLASHDB_KVDB_SIZE in config file"
#endif

#if defined(FDB_USING_KVDB) && (!defined(CONFIG_FLASHDB_TSDB_START_ADDR) || !defined(CONFIG_FLASHDB_TSDB_SIZE))
#error "Please defined CONFIG_FLASHDB_TSDB_START_ADDR or CONFIG_FLASHDB_TSDB_SIZE in config file"
#endif

#ifdef FDB_USING_KVDB
#define KVDB_TABLE     {FAL_PART_MAGIC_WORD, FLASHDB_KVDB_PATH,  FLASHDB_DEV_NAME, CONFIG_FLASHDB_KVDB_START_ADDR, CONFIG_FLASHDB_KVDB_SIZE, 0},
#else
#define KVDB_TABLE     
#endif

#ifdef FDB_USING_TSDB
#define TSDB_TABLE     {FAL_PART_MAGIC_WORD, FLASHDB_TSDB_PATH,  FLASHDB_DEV_NAME, CONFIG_FLASHDB_TSDB_START_ADDR, CONFIG_FLASHDB_TSDB_SIZE, 0},
#else
#define TSDB_TABLE     
#endif                                                                            \

/* partition table */
#define FAL_PART_TABLE                                                                \
{                                                                                     \
    KVDB_TABLE                                                                        \
    TSDB_TABLE                                                                        \
}
#endif /* FAL_PART_HAS_TABLE_CFG */

#endif /* _FAL_CFG_H_ */
