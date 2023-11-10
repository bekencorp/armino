#include "bk_drv_model.h"
#if CONFIG_FLASH_ORIGIN_API
#include "BkDriverFlash.h"
#else
#include <driver/flash_partition.h>
#endif

#define HTTP_WR_TO_FLASH        1

#if (CONFIG_TFM)
#undef HTTP_WR_TO_FLASH
#define HTTP_WR_TO_FLASH        0

typedef struct ota_hdr_s {
	UINT64 magic;
	UINT32 crc;
	UINT32 version;
	UINT16 hdr_len;
	UINT16 img_num;
	UINT32 flags;
	UINT32 reserved[2];
} ota_hdr_t;

typedef struct ota_img_hdr_s {
	UINT32 img_len;
	UINT32 img_offset;
	UINT32 flash_offset;
	UINT32 checksum;
	UINT32 version;
	UINT32 flags;
	UINT32 reserved[2];
} ota_img_hdr_t;
#endif

typedef struct http_data_st {
	UINT32 http_total;
	UINT8 do_data;
#if HTTP_WR_TO_FLASH
	UINT8 *wr_buf ;
	UINT8 *wr_tmp_buf;
	UINT16 wr_last_len ;
	UINT32 flash_address;
	bk_logic_partition_t *pt;
#endif
	DD_HANDLE flash_hdl;
} HTTP_DATA_ST;

#if CONFIG_UVC_OTA_DEMO
#define TCP_LEN_MAX             1000//896
#else
#define TCP_LEN_MAX             1460
#endif
#define HTTP_FLASH_ADDR         0xff000

extern  void store_block(unsigned block, uint8_t *src, unsigned len);
#define WR_BUF_MAX 1048

extern HTTP_DATA_ST *bk_http_ptr;
