#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define psram_block_num    10
#define PSRAM_AUD_ADDR_BASE    0x60000000
#define AUD_DMA_SIZE    0xFFFF
#define MB_CHNL_AUD     2


typedef enum {
	IDLE = 0,
	BUSING
} sd_status_t;

typedef struct {
	sd_status_t sd_status;
	uint32_t block_status;        //bit9-0  0:free, 1:full
} psram_block_status_t;

typedef enum {
	AUD_MB_DMA_DONE = 0,    //CPU1->CPU0
	AUD_MB_STOP_TEST,       //CPU1->CPU0 && CPU0->cpu1
	AUD_MB_WRITE_SD_DONE,    //CPU0->CPU1
	AUD_MB_START_TEST,
} aud_mb_cmd_t;

typedef struct {
	uint32_t chnl_id;
} aud_mb_t;

#ifdef __cplusplus
}
#endif

