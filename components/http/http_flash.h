#include "bk_drv_model.h"
#include "BkDriverFlash.h"

#define HTTP_WR_TO_FLASH        1

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

#define TCP_LEN_MAX             1460
#define HTTP_FLASH_ADDR         0xff000

extern  void store_block(unsigned block, uint8_t *src, unsigned len);
#define WR_BUF_MAX 1048

extern HTTP_DATA_ST *bk_http_ptr;
