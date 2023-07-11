/**
 * @file lv_fatfs.h
 */
 
#ifndef LV_FATFS_H
#define LV_FATFS_H


#ifdef __cplusplus
extern "C" {
#endif

void lv_fatfs_init(void);

void lv_fatfs_deinit(int number);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif