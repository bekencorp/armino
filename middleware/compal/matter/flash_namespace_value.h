#ifndef __FLASH_NAMESAPCE_VALUE__
#define __FLASH_NAMESAPCE_VALUE__

#ifdef __cplusplus
extern"C" {
#endif
#if CONFIG_FLASH_ORIGIN_API
#include "BkDriverFlash.h"
#else
#include <driver/flash_partition.h>
#endif
#if 0
#define bk_flash_debug_printf(...)                      bk_printf(__VA_ARGS__)
#else
#define bk_flash_debug_printf(...)                     
#endif

typedef  struct namespace_flash
{
	uint32_t dwUsingFlag;//FLASH_SPACE_USING or FLASH_SPACE_UNUSED
	uint32_t dwOffsetByByte;
	char     ucNamespace[56];//The maximum length of the string in namaspace
}namespace_flash_t;

typedef  struct name_flash
{
	uint32_t dwUsingFlag;//FLASH_SPACE_USING or FLASH_SPACE_UNUSED
	uint32_t dwOffsetByByte;
	uint32_t dwlength;//By Size
	char     ucName[52];//The maximum length of the string in namaspace
}name_flash_t;


typedef  struct name_flash_test
{
	char     ucNamespace[56];//The maximum length of the string in namaspace
	char     ucName[52];//The maximum length of the string in namaspace
	char     ucData[112];//The maximum length of the string in namaspace
	uint32_t dwDatalen;//数据的实际长度
}name_flash_test_t;


typedef struct ssid_key_save 
{
	char     ucssid[33];
	char     ucKey[64];
}ssid_key_save_t;

/**
 * OTA firmware encryption algorithm and compression algorithm
 */
enum rt_ota_algo
{
    RT_OTA_CRYPT_ALGO_NONE    = 0x0L,               /**< no encryption algorithm and no compression algorithm */
    RT_OTA_CRYPT_ALGO_XOR     = 0x1L,               /**< XOR encryption */
    RT_OTA_CRYPT_ALGO_AES256  = 0x2L,               /**< AES256 encryption */
    RT_OTA_CMPRS_ALGO_GZIP    = 0x1L << 8,          /**< Gzip: zh.wikipedia.org/wiki/Gzip */
    RT_OTA_CMPRS_ALGO_QUICKLZ = 0x2L << 8,          /**< QuickLZ: www.quicklz.com */
    RT_OTA_CMPRS_ALGO_FASTLZ  = 0x3L << 8,          /**< FastLZ: fastlz.org/ */

    RT_OTA_CRYPT_STAT_MASK    = 0xFL,
    RT_OTA_CMPRS_STAT_MASK    = 0xFL << 8,
};
typedef enum rt_ota_algo rt_ota_algo_t;


typedef struct ota_data_struct 
{
    char magic[4];

    rt_ota_algo_t algo;
    uint32_t timestamp;
    char name[16];
    char version[24];

    char sn[24];

    /* crc32(aes(zip(rtt.bin))) */
    uint32_t crc32;
    /* hash(rtt.bin) */
    uint32_t hash;

    /* len(rtt.bin) */
    uint32_t size_raw;
    /* len(aes(zip(rtt.bin))) */
    uint32_t size_package;

    /* crc32(rbl_hdr - info_crc32) */
    uint32_t info_crc32;

}ota_data_struct_t;



#define BK_CHECK_POINTER_NULL(pointer)   \
do{ \
      if(NULL == pointer) \
      {\
          bk_printf("[%s] [%d] pointer is null \r\n", __FUNCTION__,__LINE__);\
          return kGeneralErr;\
      }\
  }while(0)
  
#define BK_CHECK_POINTER_NULL_TO_VOID(pointer)   \
          do{ \
                if(NULL == pointer) \
                {\
                    bk_printf("[%s] [%d] pointer is null \r\n", __FUNCTION__,__LINE__);\
                    return ;\
                }\
            }while(0)

#define BK_CHECK_RETURN_VAULEL(dwRtn)   \
do{ \
    if(kNoErr != dwRtn) \
    {\
        bk_printf("[%s] [%d] return value is [%u] \r\n", __FUNCTION__,__LINE__,dwRtn);\
        return kGeneralErr;\
    }\
}while(0)

#define BK_CHECK_NO_RETURN(dwRtn)   \
    do{ \
        if(kNoErr != dwRtn) \
        {\
            bk_printf("[%s] [%d] return value is [%u] \r\n", __FUNCTION__,__LINE__,dwRtn);\
        }\
    }while(0)

#define FLASH_NAME_SPACE_MAX_ARRAY        10
#define FLASH_NAME_MAX_ARRAY              64
#define PER_NAME_SPACE_FLASH_SIZE         sizeof(namespace_flash_t)
#define PER_NAME_FLASH_SIZE               sizeof(name_flash_t)
#define NAME_SPACE_FLASH_TOTAL_SIZE       (0x1000) //Do not modify this immediate number unless you understand the code
#define NAME_SPACE_FLASH_ACTUAL_USE_SIZE  (sizeof(namespace_flash_t) * FLASH_NAME_SPACE_MAX_ARRAY) //Do not modify this immediate number unless you understand the code
#define NAME_FLASH_TOTAL_SIZE             (sizeof(name_flash_t) * FLASH_NAME_MAX_ARRAY)//Do not modify this immediate number unless you understand the code
#define NAME_FLASH_TOTAL_HEADER           (NAME_SPACE_FLASH_TOTAL_SIZE + NAME_FLASH_TOTAL_SIZE * FLASH_NAME_SPACE_MAX_ARRAY)


#define NAME_FLASH_DATA_BASE_ADDR       ((NAME_FLASH_TOTAL_HEADER & 0xFFF) ?  ((NAME_FLASH_TOTAL_HEADER & (~0xFFF)) + 0x1000) : (NAME_FLASH_TOTAL_HEADER & (~0xFFF)))


#define NAME_SPACE_NOT_FOUND           0
#define NAME_SPACE_FOUND               1
#define NAME_SPACE_ADDR_SET_SUCCESS    1
#define NAME_SPACE_ADDR_SET_FAIL       0
#define FLASH_SECTOR_SIZE              0x1000 //4*1024


#define FLASH_SPACE_USING              0xFFFFFF55//依据flash只能写零，不能写1的原则
#define FLASH_SPACE_MASK               0xFFFF5555

#define WRITE_OTA_DATA_TO_FLASH_TIMES       2 //写flash失败时的最大重复次数

uint32_t bk_write_data( const char * ucnamespace,const char * ucname, char * indata,uint32_t data_size);
uint32_t bk_read_data( const char * ucnamespace,const char * ucname,char * out_data,uint32_t dw_read_len,uint32_t *out_length);
uint32_t bk_clean_data ( const char * ucnamespace,const char * ucname);
uint32_t bK_clear_namespace ( const char * ucnamespace);
uint32_t bk_ensure_namespace( const char * ucnamespace);
uint32_t bk_ensure_name_data( const char * ucnamespace,const char * ucname);
uint32_t wpa_ssid_key_get(ssid_key_save_t *wpas_get);
uint32_t bk_erase_all( bk_partition_t type );
uint32_t bk_write_ota_data_to_flash( char * indata,uint32_t data_offset_addr,uint32_t data_length);
uint32_t bk_read_ota_data_in_flash( char * outdata,uint32_t data_offset_addr,uint32_t data_length);
uint32_t bk_erase_ota_data_in_flash();

#if 1
void bk_write_data_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv );
void bk_read_data_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void bk_erase_namespace_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void bk_erase_name_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void bk_name_data_exits(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void  bk_erase_all_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#endif
#ifdef __cplusplus
}
#endif

#endif
