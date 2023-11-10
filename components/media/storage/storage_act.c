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

#include <os/os.h>
#include <components/log.h>
#include <stdio.h>

#include <driver/dma.h>

#include <driver/int.h>
#include <os/mem.h>
#include <driver/gpio.h>
#include <driver/gpio_types.h>
#include <driver/psram.h>
#include <driver/dma.h>
#include "lcd_act.h"


#include <driver/jpeg_enc.h>
#include <driver/jpeg_enc_types.h>
#include <driver/dvp_camera.h>
#include <driver/dvp_camera_types.h>

#include "frame_buffer.h"

#include "media_core.h"
#include "media_evt.h"
#include "storage_act.h"

#if (CONFIG_FATFS)
#include "ff.h"
#include "diskio.h"
#endif

#include "driver/flash.h"
#include "driver/flash_partition.h"

#define TAG "storage"

extern u64 riscv_get_mtimer(void);

#define SECTOR                  0x1000

#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

static beken_queue_t storage_task_queue = NULL;
static beken_thread_t storage_task_thread = NULL;

typedef struct
{
	uint8_t type;
	uint32_t data;
} storages_task_msg_t;


typedef enum
{
	STORAGE_TASK_CAPTURE,
	STORAGE_TASK_SAVE,
	STORAGE_TASK_EXIT,
} storage_task_evt_t;

typedef void (*frame_cb_t)(frame_buffer_t *frame);

storage_info_t storage_info;
char *capture_name = NULL;

storage_flash_t storge_flash;
bool storage_task_running = false;
beken_semaphore_t storage_save_sem = NULL;
frame_cb_t frame_read_callback = NULL;

bk_err_t media_app_register_read_frame_cb(void *cb)
{
	frame_read_callback = cb;

	return BK_OK;
}

bk_err_t storage_task_send_msg(uint8_t msg_type, uint32_t data)
{
	bk_err_t ret;
	storages_task_msg_t msg;

	if (storage_task_queue)
	{
		msg.type = msg_type;
		msg.data = data;

		ret = rtos_push_to_queue(&storage_task_queue, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret)
		{
			os_printf("video_transfer_cpu1_send_msg failed\r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

void storage_frame_buffer_dump(frame_buffer_t *frame, char *name)
{
	LOGI("%s dump frame: %p, %u, size: %d\n", __func__, frame->frame, frame->sequence, frame->length);

#if (CONFIG_FATFS)
	storage_mem_to_sdcard(name, frame->frame, frame->length);

	LOGI("%s, complete\n", __func__);
#endif
}


static void storage_capture_save(frame_buffer_t *frame)
{
	LOGI("%s save frame: %d, size: %d\n", __func__, frame->sequence ,frame->length);
	uint64_t before, after;
	
#if (CONFIG_ARCH_RISCV)
	before = riscv_get_mtimer();
#else
	before = 0;
#endif
#if (CONFIG_FATFS)
	storage_mem_to_sdcard(capture_name, frame->frame, frame->length);
#else  //save in flash
	bk_err_t ret;
	if (frame == NULL)
	{
		LOGI("jpeg frame = NULL \n");
		return;
	}
	bk_logic_partition_t *pt = bk_flash_partition_get_info(BK_PARTITION_USR_CONFIG);
	LOGI("flash addr %x \n", pt->partition_start_addr);

	storge_flash.flash_image_addr = pt->partition_start_addr;
	storge_flash.flasg_img_length = frame->length;

	bk_flash_set_protect_type(FLASH_PROTECT_NONE);
	for (int i = 0; i < frame->length / SECTOR + 1; i++)
	{
		bk_flash_erase_sector(pt->partition_start_addr + (SECTOR * i));
	}

	ret = bk_flash_write_bytes(pt->partition_start_addr, (uint8_t *)frame->frame, frame->length);
	if (ret != BK_OK)
	{
		LOGI("%s: storge to flsah error \n", __func__);
	}
	bk_flash_set_protect_type(FLASH_UNPROTECT_LAST_BLOCK);
#endif
	LOGI("%s, complete\n", __func__);
#if (CONFIG_ARCH_RISCV)
	after = riscv_get_mtimer();
#else
	after = 0;
#endif
	LOGI("save jpeg to sd/flash use %lu\n", (after - before) / 26000);
}

bk_err_t sdcard_read_filelen(char *filename)
{
    int ret = BK_FAIL;

#if (CONFIG_FATFS)
    char cFileName[FF_MAX_LFN];
    FIL file;
    FRESULT fr;
    
    do{
        if(!filename)
        {
            os_printf("%s param is null\r\n", __FUNCTION__);
            ret = BK_ERR_PARAM;
            break;
        }
        
        // step 1: read picture from sd to psram
        sprintf(cFileName, "%d:/%s", DISK_NUMBER_SDIO_SD, filename);
        /*open pcm file*/
        fr = f_open(&file, cFileName, FA_OPEN_EXISTING | FA_READ);
        if (fr != FR_OK) 
        {
            os_printf("open %s fail.\r\n", filename);
            ret = BK_ERR_OPEN;
            break;
        }
        
        ret = f_size(&file);
        f_close(&file);
    }while(0);
#else
    os_printf("Not support\r\n");
    ret = BK_ERR_NOT_SUPPORT;
#endif

    return ret;
}

bk_err_t sdcard_read_to_mem(char *filename, uint32_t* paddr, uint32_t *total_len)
{
#if (CONFIG_FATFS)
	char cFileName[FF_MAX_LFN];
	FIL file;
	FRESULT fr;
	FSIZE_t size_64bit = 0;
	unsigned int uiTemp = 0;
	uint8_t *sram_addr = NULL;
	uint32_t once_read_len = 1024*2;

	// step 1: read picture from sd to psram
	sprintf(cFileName, "%d:/%s", DISK_NUMBER_SDIO_SD, filename);
	sram_addr = os_malloc(once_read_len);
	if (sram_addr == NULL) {
		os_printf("sd buffer malloc failed\r\n");
		return BK_FAIL;
	}

	char *ucRdTemp = (char *)sram_addr;

	/*open pcm file*/
	fr = f_open(&file, cFileName, FA_OPEN_EXISTING | FA_READ);
	if (fr != FR_OK) {
//		os_printf("open %s fail.\r\n", filename);
		return BK_FAIL;
	}
	size_64bit = f_size(&file);
	uint32_t total_size = (uint32_t)size_64bit;// total byte
	os_printf("read file total_size = %d.\r\n", total_size);
	*total_len = total_size;

	while(1)
	{
		fr = f_read(&file, ucRdTemp, once_read_len, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read file fail.\r\n");
			return BK_FAIL;
		}
		if (uiTemp == 0)
		{
			os_printf("read file complete.\r\n");
			break;
		}
		if(once_read_len != uiTemp)
		{
			if (uiTemp % 4)
			{
				uiTemp = (uiTemp / 4 + 1) * 4;
			}
			bk_psram_word_memcpy(paddr, sram_addr, uiTemp);
		}
		else
		{
			bk_psram_word_memcpy(paddr, sram_addr, once_read_len);
			paddr += (once_read_len/4);
		}
	}

	os_free(sram_addr);

	fr = f_close(&file);
	if (fr != FR_OK) {
		os_printf("close %s fail!\r\n", filename);
		return BK_FAIL;
	}
#else
		os_printf("Not support\r\n");
#endif

	return BK_OK;
}

bk_err_t storage_mem_to_sdcard(char *filename, uint8_t *paddr, uint32_t total_len)
{
#if (CONFIG_FATFS)
	FIL fp1;
	unsigned int uiTemp = 0;
	char file_name[50] = {0};

	sprintf(file_name, "%d:/%s", DISK_NUMBER_SDIO_SD, filename);

	FRESULT fr = f_open(&fp1, file_name, FA_CREATE_ALWAYS | FA_WRITE);
	if (fr != FR_OK)
	{
		LOGE("can not open file: %s, error: %d\n", file_name, fr);
		return BK_FAIL;
	}

	LOGI("open file:%s!\n", file_name);

	fr = f_write(&fp1, (char *)paddr, total_len, &uiTemp);
	if (fr != FR_OK)
	{
		LOGE("f_write failed 1 fr = %d\r\n", fr);
	}
	f_close(&fp1);

#endif

	return BK_OK;
}

static void  storage_save_frame(frame_buffer_t *frame)
{
#if (CONFIG_FATFS)
	FIL fp1;
	unsigned int uiTemp = 0;
	char file_name[50] = {0};

	sprintf(file_name, "%d:/%s", DISK_NUMBER_SDIO_SD, capture_name);

	FRESULT fr = f_open(&fp1, file_name, FA_OPEN_APPEND | FA_WRITE);
	if (fr != FR_OK)
	{
		LOGE("can not open file: %s, error: %d\n", file_name, fr);
		return;
	}

	fr = f_write(&fp1, (char *)frame->frame, frame->length, &uiTemp);
	if (fr != FR_OK)
	{
		LOGE("f_write failed 1 fr = %d\r\n", fr);
	}
	f_close(&fp1);
#endif
}

static void storage_capture_save_handle(void)
{
	frame_buffer_t *frame = NULL;

	frame = frame_buffer_fb_read(MODULE_CAPTURE);

	if (frame == NULL)
	{
		LOGE("read jpeg NULL\n");
		return;
	}

	storage_capture_save(frame);

	frame_buffer_fb_free(frame, MODULE_CAPTURE);

	storage_info.capture_state = STORAGE_STATE_DISABLED;
}

static void storage_save_frame_handle(void)
{
	frame_buffer_t *frame = NULL;

	frame = frame_buffer_fb_read(MODULE_CAPTURE);

	if (frame == NULL)
	{
		LOGE("read jpeg NULL\n");
		return;
	}

	if (frame_read_callback)
	{
		frame_read_callback(frame);
	}
	else
	{
#if (CONFIG_FATFS)
		storage_save_frame(frame);
#else
		LOGI("%s, not support save to sdcard\r\n", __func__);
#endif
	}

	frame_buffer_fb_free(frame, MODULE_CAPTURE);
}

static void storage_save_video_handle(void)
{
	while (storage_task_running)
	{
		storage_save_frame_handle();
	};

	rtos_set_semaphore(&storage_save_sem);
}


static void storage_task_entry(beken_thread_arg_t data)
{
	bk_err_t ret = BK_OK;
	storages_task_msg_t msg;

	while (1)
	{
		ret = rtos_pop_from_queue(&storage_task_queue, &msg, BEKEN_WAIT_FOREVER);

		if (kNoErr == ret)
		{
			switch (msg.type)
			{
				case STORAGE_TASK_CAPTURE:
					storage_capture_save_handle();
					break;

				case STORAGE_TASK_SAVE:
					storage_save_video_handle();
					break;

				case STORAGE_TASK_EXIT:
					goto exit;
				default:
					break;
			}
		}
	}

exit:

	rtos_deinit_queue(&storage_task_queue);
	storage_task_queue = NULL;

	storage_task_thread = NULL;
	rtos_delete_thread(NULL);
}

int storage_task_start(void)
{
	int ret;

	if (storage_task_queue == NULL)
	{
		ret = rtos_init_queue(&storage_task_queue, "trs_task_queue", sizeof(storages_task_msg_t), 60);

		if (kNoErr != ret)
		{
			LOGE("%s trs_task_queue init failed\n");
			goto error;
		}
	}

	if (storage_task_thread == NULL)
	{
		frame_buffer_fb_register(MODULE_CAPTURE, FB_INDEX_JPEG);

		ret = rtos_create_thread(&storage_task_thread,
		                         BEKEN_DEFAULT_WORKER_PRIORITY,
		                         "storage_task_thread",
		                         (beken_thread_function_t)storage_task_entry,
		                         4 * 1024,
		                         NULL);

		if (kNoErr != ret)
		{
			LOGE("%s trs_task_thread init failed\n");
			goto error;
		}
	}

	return kNoErr;

error:

	frame_buffer_fb_deregister(MODULE_CAPTURE);

	if (storage_task_queue)
	{
		rtos_deinit_queue(&storage_task_queue);
		storage_task_queue = NULL;
	}

	if (storage_task_thread)
	{
		storage_task_thread = NULL;
		rtos_delete_thread(NULL);
	}

	return kGeneralErr;
}


void storage_open_handle(void)
{
	LOGI("%s\n", __func__);

	LOGI("%s register camera init\n", __func__);
}


void storage_capture_handle(param_pak_t *param)
{
	LOGI("%s, %s\n", __func__, (char *)param->param);

	if (storage_info.capture_state == STORAGE_STATE_ENABLED)
	{
		LOGI("%s already capture\n", __func__);
		goto out;
	}

	if (capture_name == NULL)
	{
		capture_name = (char *)os_malloc(32);
	}

	os_memcpy(capture_name, (char *)param->param, 31);
	capture_name[31] = 0;

	storage_info.capture_state = STORAGE_STATE_ENABLED;

	storage_task_send_msg(STORAGE_TASK_CAPTURE, 0);

out:

	MEDIA_EVT_RETURN(param, kNoErr);
}


void storage_save_handle(param_pak_t *param)
{
	LOGI("%s, %s\n", __func__, (char *)param->param);

	if (storage_info.capture_state == STORAGE_STATE_ENABLED)
	{
		LOGI("%s already capture\n", __func__);
		goto out;
	}

	if (storage_task_running)
	{
		LOGI("%s save frame is working\n", __func__);
		goto out;
	}

	int ret = rtos_init_semaphore_ex(&storage_save_sem, 1, 0);

	if (BK_OK != ret)
	{
		LOGE("%s semaphore init failed\n", __func__);
		goto out;
	}

	if (capture_name == NULL)
	{
		capture_name = (char *)os_malloc(32);
	}

	os_memcpy(capture_name, (char *)param->param, 31);
	capture_name[31] = 0;

	storage_task_running = true;

	storage_info.capture_state = STORAGE_STATE_ENABLED;

	storage_task_send_msg(STORAGE_TASK_SAVE, 0);

out:
	MEDIA_EVT_RETURN(param, kNoErr);
}

void storage_save_exit_handle(param_pak_t *param)
{
	LOGI("%s\n", __func__);

	if (!storage_task_running)
	{
		LOGE("%s already exit\n", __func__);
		goto out;
	}

	storage_task_running = false;

	storage_info.capture_state = STORAGE_STATE_DISABLED;

	int ret = rtos_get_semaphore(&storage_save_sem, BEKEN_NEVER_TIMEOUT);

	if (BK_OK != ret)
	{
		LOGE("%s storage_save_sem get failed\n", __func__);
	}

	ret = rtos_deinit_semaphore(&storage_save_sem);

	if (BK_OK != ret)
	{
		LOGE("%s storage_save_sem deinit failed\n");
	}

	if (frame_read_callback)
	{
		frame_read_callback = NULL;
	}

out:
	MEDIA_EVT_RETURN(param, kNoErr);
}


void storage_event_handle(uint32_t event, uint32_t param)
{
	switch (event)
	{
		case EVENT_STORAGE_OPEN_IND:
			storage_open_handle();
			break;

		case EVENT_STORAGE_CAPTURE_IND:
			storage_capture_handle((param_pak_t *)param);
			break;

		case EVENT_STORAGE_SAVE_START_IND:
			storage_save_handle((param_pak_t *)param);
			break;

		case EVENT_STORAGE_SAVE_STOP_IND:
			storage_save_exit_handle((param_pak_t *)param);
			break;
	}
}

storage_state_t get_storage_state(void)
{
	return storage_info.state;
}

void set_storage_state(storage_state_t state)
{
	storage_info.state = state;
}

void storage_init(void)
{
	os_memset(&storage_info, 0, sizeof(storage_info_t));

	storage_info.state = STORAGE_STATE_DISABLED;
	storage_info.capture_state = STORAGE_STATE_DISABLED;
	storage_task_start();
}

void lcd_storage_capture_save(char * capture_name, uint8_t *addr, uint32_t len)
{
#if (CONFIG_FATFS)
	storage_mem_to_sdcard(capture_name, addr, len);
#endif
}

