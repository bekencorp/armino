/*************************************************************
 * @file        test_fatfs.h
 * @brief       Header file of test_fatfs.c
 * @author      GuWenFu
 * @version     V1.0
 * @date        2016-09-29
 * @par
 * @attention
 *
 * @history     2016-09-29 gwf    create this file
 */

#ifndef __TEST_FATFS_H__

#define __TEST_FATFS_H__


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


#include "diskio.h"


#if CONFIG_SDCARD_HOST
    extern void test_mount(DISK_NUMBER number);
    extern void scan_file_system(DISK_NUMBER number);
    extern void test_fatfs_read(DISK_NUMBER number, char *filename);
	extern void test_fatfs_append_write(DISK_NUMBER number, char *filename, char *content, uint32_t len);
    extern void test_fatfs_format(DISK_NUMBER number);
#endif


#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif      /* __TEST_FATFS_H__ */
