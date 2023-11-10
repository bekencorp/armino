#include <stdio.h>
#include <string.h>
#include <common/bk_include.h>
#include "diskio.h"
#include "test_fatfs.h"
#include <os/mem.h>
#include "ff.h"

#define WR_RD_BUF_SIZE          1024
#define TEST_MP3_FILE_NAME      "/Panama_Matteo.mp3"
#define TEST_TXT_FILE_NAME      "/test1.txt"
#define TEST_DUMP_FILE_NAME      "/dump1.txt"
#define TEST_TXT_FILE_NAME1     "/�������ֿ����ж೤����೤�Ͷ೤��������ҧ�ң��������ҳԷ�.txt"

void bk_mem_dump_ex(const char *title, unsigned char *data, uint32_t data_len);

#if CONFIG_FATFS_SDCARD
FATFS *pfs = NULL;

FRESULT scan_files
(
    char *path        /* Start node to be scanned (***also used as work area***) */
)
{
    FRESULT fr;
    DIR dir;
    FILINFO fno;

    fr = f_opendir(&dir, path);                 /* Open the directory */
    if (fr == FR_OK)
    {
        FATFS_LOGI("%s/\r\n", path);
        while (1)
        {
            fr = f_readdir(&dir, &fno);         /* Read a directory item */
            if (fr != FR_OK)
            {
                break;  /* Break on error */
            }
            if (fno.fname[0] == 0)
            {
                break;  /* Break on end of dir */
            }
            if (fno.fattrib & AM_DIR)
            {
                /* It is a directory */
                char pathTemp[255];
                sprintf(pathTemp, "%s/%s", path, fno.fname);
                fr = scan_files(pathTemp);      /* Enter the directory */
                if (fr != FR_OK) break;
            }
            else
            {
                /* It is a file. */
                FATFS_LOGI("%s/%s\r\n", path, fno.fname);
            }
        }
        f_closedir(&dir);
    }
    else
    {
        FATFS_LOGI("f_opendir failed\r\n");
    }

    return fr;
}

void test_mount(DISK_NUMBER number)
{
    FRESULT fr;
    char cFileName[FF_MAX_LFN];

    if (pfs != NULL)
    {
        os_free(pfs);
    }

    pfs = os_malloc(sizeof(FATFS));
	if(NULL == pfs)
	{
		FATFS_LOGI("f_mount malloc failed!\r\n");
		goto failed_mount;
	}

    sprintf(cFileName, "%d:", number);
    fr = f_mount(pfs, cFileName, 1);
    if (fr != FR_OK)
    {
        FATFS_LOGI("f_mount failed:%d\r\n", fr);
    }
    else
    {
        FATFS_LOGI("f_mount OK!\r\n");
    }

failed_mount:
    FATFS_LOGI("----- test_mount %d over  -----\r\n\r\n", number);
}

void test_unmount(DISK_NUMBER number)
{
    FRESULT fr;
    char cFileName[FF_MAX_LFN];
    sprintf(cFileName, "%d:", number);
    fr = f_unmount(DISK_NUMBER_SDIO_SD, cFileName, 1);
    if (fr != FR_OK)
    {
        os_printf("f_unmount failed:%d\r\n", fr);
    }
    else
    {
        os_printf("f_unmount OK!\r\n");
    }
    os_printf("----- test_unmount %d over  -----\r\n\r\n", number);
}
void test_getfree(DISK_NUMBER number)
{
    FRESULT fr;
    char cFileName[FF_MAX_LFN];
    DWORD getnclst;
    sprintf(cFileName, "%d:", number);
    fr = f_getfree(cFileName, &getnclst, &pfs);
    os_printf("test_getfree getnclst:DEC %d free space: %dMB\r\n", getnclst, getnclst/SD_CLUSTER_TO_MEM_64KB);
    if (fr != FR_OK)
    {
        os_printf("f_getfree failed:%d\r\n", fr);
    }
    else
    {
        os_printf("f_getfree OK!\r\n");
    }
    os_printf("----- test_getfree %d over  -----\r\n\r\n", number);
}

void scan_file_system(DISK_NUMBER number)
{
    FRESULT fr;
    char cFileName[FF_MAX_LFN];

    FATFS_LOGI("\r\n----- scan_file_system %d start -----\r\n", number);

    sprintf(cFileName, "%d:", number);
    fr = scan_files(cFileName);
    if (fr != FR_OK)
    {
        FATFS_LOGI("scan_files failed!\r\n");
    }
    else
    {
        FATFS_LOGI("scan_files OK!\r\n");
    }

    FATFS_LOGI("----- scan_file_system %d over  -----\r\n\r\n", number);
}

void test_fatfs(DISK_NUMBER number)
{
    char cFileName[FF_MAX_LFN];
    FIL file;
    FRESULT fr;

    FATFS_LOGI("\r\n----- test_fatfs %d start -----\r\n", number);
    sprintf(cFileName, "%d:%s", number, TEST_MP3_FILE_NAME);
    FATFS_LOGI("f_open \"%s\"\r\n", cFileName);
    fr = f_open(&file, cFileName, FA_OPEN_EXISTING | FA_READ);
    if (fr == FR_OK)
    {
        unsigned int uiTemp = 0;
        unsigned int uiRdTotalLength = 0;
        unsigned char ucRdTemp[WR_RD_BUF_SIZE];

        do
        {
            FATFS_LOGI(".");
            fr = f_read(&file, ucRdTemp, WR_RD_BUF_SIZE, &uiTemp);
            if (fr == FR_OK)
            {
            }
            else
            {
                FATFS_LOGI("f_read failed 1 fr = %d\r\n", fr);
                goto error1;
            }
            uiRdTotalLength += uiTemp;
        }
        while (uiTemp == WR_RD_BUF_SIZE);
        FATFS_LOGI("\r\n");
        FATFS_LOGI("f_read: read total byte = %d\r\n", uiRdTotalLength);

        fr = f_close(&file);
        if (fr != FR_OK)
        {
            FATFS_LOGI("f_close failed 1 fr = %d\r\n", fr);
            goto error1;
        }
        else
        {
            FATFS_LOGI("f_close OK\r\n");
        }
    }
    else
    {
        FATFS_LOGI("f_open failed 1 fr = %d\r\n", fr);
        goto error0;
    }

    FATFS_LOGI("----- test_fatfs %d over  -----\r\n\r\n", number);
    return;

error1:
    f_close(&file);
error0:
    FATFS_LOGI("----- test_fatfs %d failed  -----\r\n\r\n", number);
}


void test_fatfs_read(DISK_NUMBER number, char *filename, uint64_t len)
{
    char cFileName[FF_MAX_LFN];
    FIL file;
    FRESULT fr;
	unsigned char *ucRdTemp = os_malloc(WR_RD_BUF_SIZE);
	uint32_t packet_len = 0;
	uint64_t left_len = 0;

	if(ucRdTemp == 0)
	{
		FATFS_LOGI("%s:os_malloc fail \r\n", __func__);
		goto error1;
	}

    FATFS_LOGI("\r\n----- test_fatfs %d start -----\r\n", number);
	if(filename)
		sprintf(cFileName, "%d:/%s", number, filename);
	else
		sprintf(cFileName, "%d:%s", number, TEST_TXT_FILE_NAME);
    FATFS_LOGI("f_open \"%s\"\r\n", cFileName);
    fr = f_open(&file, cFileName, FA_OPEN_EXISTING | FA_READ);
    if (fr == FR_OK)
    {
        unsigned int uiTemp = 0;
        unsigned int uiRdTotalLength = 0;

		left_len = f_size(&file) < len? f_size(&file) : len;
		FATFS_LOGI("will read left_len = %d \r\n", (uint32_t)left_len);
        do
        {
			packet_len = left_len < WR_RD_BUF_SIZE? left_len : WR_RD_BUF_SIZE;
            FATFS_LOGI("f_read start:%d bytes \r\n", packet_len);
            fr = f_read(&file, ucRdTemp, packet_len, &uiTemp);
            if (fr == FR_OK)
            {
				bk_mem_dump_ex("f_read one cycle", ucRdTemp, packet_len);
				left_len -= uiTemp;
				FATFS_LOGI("f_read one cycle finish:left_len = %d\r\n", (uint32_t)left_len);
            }
            else
            {
                FATFS_LOGI("f_read failed 1 fr = %d\r\n", fr);
                goto error1;
            }
            uiRdTotalLength += uiTemp;

			if(left_len == 0)
				break;
        }
        while (uiTemp == packet_len);

        FATFS_LOGI("f_read: read total byte = %d\r\n", uiRdTotalLength);
        fr = f_close(&file);
        if (fr != FR_OK)
        {
            FATFS_LOGI("f_close failed 1 fr = %d\r\n", fr);
            goto error1;
        }
        else
        {
            FATFS_LOGI("f_close OK\r\n");
        }
    }
    else
    {
        FATFS_LOGI("f_open failed 1 fr = %d\r\n", fr);
    }

    FATFS_LOGI("----- test_fatfs %d over  -----\r\n\r\n", number);
	if(ucRdTemp)
	{
		os_free(ucRdTemp);
		ucRdTemp = 0;
	}

    return;

error1:
    f_close(&file);
	if(ucRdTemp)
	{
		os_free(ucRdTemp);
		ucRdTemp = 0;
	}
}

//File exist:append write contents(<64bytes).
//File not exist:create a file and write contents(<64bytes).
void test_fatfs_append_write(DISK_NUMBER number, char *filename, char *content, uint32_t len)
{
    char cFileName[FF_MAX_LFN];
    FIL file;
    FRESULT fr;

    FATFS_LOGI("\r\n----- test_fatfs %d start -----\r\n", number);
	if(filename)
		sprintf(cFileName, "%d:/%s", number, filename);
	else
		sprintf(cFileName, "%d:/%s", number, TEST_TXT_FILE_NAME);
    FATFS_LOGI("f_open \"%s\"\r\n", cFileName);

    fr = f_open(&file, cFileName, FA_OPEN_APPEND | FA_WRITE);
    if (fr == FR_OK)
    {
        unsigned int uiTemp = 0;
        unsigned int uiRdTotalLength = 0;

		len = len > 64? 64:len;
        do
        {
            FATFS_LOGI(".");
            fr = f_write(&file, content, len, &uiTemp);
            if (fr == FR_OK)
            {
            	
            }
            else
            {
                FATFS_LOGI("f_write failed 1 fr = %d\r\n", fr);
                goto error1;
            }
            uiRdTotalLength += uiTemp;
        }
        while (uiRdTotalLength != len);

        fr = f_close(&file);
        if (fr != FR_OK)
        {
            FATFS_LOGI("f_close failed 1 fr = %d\r\n", fr);
            goto error1;
        }
        else
        {
            FATFS_LOGI("f_close OK\r\n");
        }
    }
    else
    {
        FATFS_LOGI("f_open failed 1 fr = %d\r\n", fr);
    }

    FATFS_LOGI("----- test_fatfs %d over  -----\r\n\r\n", number);
    return;

error1:
    f_close(&file);
}

#define TEST_FATFS_PACKET_LEN (1<<10)
void test_fatfs_dump(DISK_NUMBER number, char *filename, uint32_t start_addr, uint32_t dump_len)
{
	FIL file;
	FRESULT fr;
	char cFileName[FF_MAX_LFN];
	unsigned int uiTemp = 0;

	FATFS_LOGI("\r\n----- %s %d start -----\r\n", __func__, number);
	FATFS_LOGI("file_name=%s,start_addr=0x%0x,len=%d \r\n", filename, start_addr, dump_len);

	if(filename)
		sprintf(cFileName, "%d:/%s", number, filename);
	else
		sprintf(cFileName, "%d:%s", number, TEST_DUMP_FILE_NAME);
	FATFS_LOGI("f_open start \"%s\"\r\n", cFileName);

	fr = f_open(&file, cFileName, FA_OPEN_APPEND | FA_WRITE);
	if (fr != FR_OK)
	{
		FATFS_LOGI("f_open failed 1 fr = %d\r\n", fr);
		goto exit;
	}

	FATFS_LOGI("f_write start\r\n");
	//write:one time write all contents
	do
	{
		fr = f_write(&file, (uint8_t *)start_addr, dump_len, &uiTemp);
		if (fr != FR_OK)
		{
			FATFS_LOGI("f_write fail 1 fr = %d\r\n", fr);
			goto exit;
		}

		FATFS_LOGI("f_write end len = %d\r\n", uiTemp);
	} while(0);

#if 1
	//set file pointer to head
	FATFS_LOGI("f_lseek start\r\n");
	fr = f_lseek(&file, 0);
	if (fr != FR_OK)
	{
		FATFS_LOGI("f_lseek fail 1 fr = %d\r\n", fr);
		goto exit;
	}
#endif
	FATFS_LOGI("f_close start\r\n");
	//file close
	fr = f_close(&file);
	if (fr != FR_OK)
	{
		FATFS_LOGI("f_close fail 1 fr = %d\r\n", fr);
		goto exit;
	}

exit:
	return;
}

#define TEST_FATFS_MAX_FILE_LEN (1<<20)
#define TEST_FATFS_PACKET_LEN (1<<10)
void test_fatfs_auto_test(DISK_NUMBER number, char *filename, uint32_t len, uint32_t test_count, uint32_t start_addr)
{
	uint32_t i = 0, j = 0, k = 0;
	FIL file;
	FRESULT fr;
	char cFileName[FF_MAX_LFN];
	uint8_t *content_p = (uint8_t *)start_addr;
	unsigned int uiTemp = 0;
	uint8_t *buf_p = 0;
	uint32_t packet_cnt = 0, bytes_cnt = 0;
	FATFS *checkspace_pfs = NULL;
	DWORD freenclst;

	FATFS_LOGD("\r\n----- %s %d start -----\r\n", __func__, number);
	FATFS_LOGD("file_name=%s,len=%d,test_cnt=%d \r\n", filename, len, test_count);

	if(filename)
		sprintf(cFileName, "%d:/%s", number, filename);
	else
		sprintf(cFileName, "%d:%s", number, TEST_TXT_FILE_NAME);
	FATFS_LOGD("f_open \"%s\"\r\n", cFileName);

	len = len < TEST_FATFS_MAX_FILE_LEN? len : TEST_FATFS_MAX_FILE_LEN;
	packet_cnt = len / TEST_FATFS_PACKET_LEN;
	bytes_cnt = len % TEST_FATFS_PACKET_LEN;

	buf_p = os_malloc(TEST_FATFS_PACKET_LEN);
	if (buf_p == NULL)
	{
		FATFS_LOGE("malloc fail \r\n");
		return;
	}

	for(i = 0; i < test_count; i++)
	{
		FATFS_LOGD("test round=%d start \r\n", i);
		fr = f_getfree(cFileName, &freenclst, &checkspace_pfs);
		if(fr != FR_OK)
		{
			FATFS_LOGE("f_getfree failed 1 fr = %d\r\n", fr);
			goto exit;
		} else if(freenclst < SD_MIN_NUMBER_REMAINING_CLUSTERS) {
			FATFS_LOGE("Insufficient Space! freenclst: %d free_mem:%d MB\r\n",freenclst, freenclst/SD_CLUSTER_TO_MEM_64KB);
			goto exit;
		}

		fr = f_open(&file, cFileName, FA_OPEN_APPEND | FA_WRITE);
		if (fr != FR_OK)
		{
			FATFS_LOGE("f_open failed 1 fr = %d\r\n", fr);
			goto exit;
		}

		//set file pointer to head
		fr = f_lseek(&file, 0);
		if (fr != FR_OK)
		{
			FATFS_LOGE("f_lseek fail 1 fr = %d\r\n", fr);
			goto exit;
		}
		//write:one time write all contents
		do
		{
			fr = f_write(&file, (uint8_t *)content_p, len, &uiTemp);
			if (fr != FR_OK)
			{
				FATFS_LOGE("f_write fail 1 fr = %d\r\n", fr);
				goto exit;
			}

			FATFS_LOGD("f_write len = %d\r\n", uiTemp);
		}while(0);

		//set file pointer to head
		fr = f_lseek(&file, 0);
		if (fr != FR_OK)
		{
			FATFS_LOGE("f_lseek fail 1 fr = %d\r\n", fr);
			goto exit;
		}

		//file close
		fr = f_close(&file);
		if (fr != FR_OK)
		{
			FATFS_LOGE("f_close fail 1 fr = %d\r\n", fr);
			goto exit;
		}

		FATFS_LOGD("check round=%d start \r\n", i);
		fr = f_open(&file, cFileName, FA_OPEN_EXISTING | FA_READ);
		if (fr != FR_OK)
		{
			FATFS_LOGE("f_open failed 1 fr = %d\r\n", fr);
			goto exit;
		}

		//set file pointer to head
		fr = f_lseek(&file, 0);
		if (fr != FR_OK)
		{
			FATFS_LOGE("f_lseek fail 1 fr = %d\r\n", fr);
			goto exit;
		}

		//read:as the buffer is not enough, so read each time less then TEST_FATFS_PACKET_LEN
		for(j = 0; j < packet_cnt; j++)
		{
			fr = f_read(&file, (uint8_t *)buf_p, TEST_FATFS_PACKET_LEN, &uiTemp);
			if (fr != FR_OK)
			{
				FATFS_LOGE("f_read packet %d fail 1 fr = %d\r\n", j, fr);
				goto exit;
			}
			FATFS_LOGD("f_read len = %d\r\n", uiTemp);

			//compare
			for(k = 0; k < TEST_FATFS_PACKET_LEN; k++)
			{
				if(*(content_p	+ j * TEST_FATFS_PACKET_LEN + k) != (*(buf_p + k)))
				{
					FATFS_LOGE("auto test fail packet[%d] byte[%d] err,source_val=0x%08x,tar_val=0x%08x \r\n", j, k, *(content_p	+ j * TEST_FATFS_PACKET_LEN + k), (*(buf_p + k)));
					goto exit;
				}
			}
		}

		//left contents
		if(bytes_cnt)
		{
			fr = f_read(&file, (uint8_t *)buf_p, bytes_cnt, &uiTemp);
			if (fr != FR_OK)
			{
				FATFS_LOGE("f_read last packet fail 1 fr = %d\r\n", fr);
				goto exit;
			}
			FATFS_LOGD("f_read len = %d\r\n", uiTemp);

			//compare
			for(k = 0; k < bytes_cnt; k++)
			{
				if(*(content_p	+ packet_cnt * TEST_FATFS_PACKET_LEN + k) != (*(buf_p + k)))
				{
					FATFS_LOGE("auto test fail last packet byte[%d] err\r\n", k);
					goto exit;
				}
			}
		}

		//set file pointer to head
		fr = f_lseek(&file, 0);
		if (fr != FR_OK)
		{
			FATFS_LOGE("f_lseek fail 1 fr = %d\r\n", fr);
			goto exit;
		}

		//file close
		fr = f_close(&file);
		if (fr != FR_OK)
		{
			FATFS_LOGE("f_read last packet fail 1 fr = %d\r\n", fr);
			goto exit;
		}

		FATFS_LOGD("check round=%d end \r\n", i);
	}

	FATFS_LOGI("auto test succ \r\n");

exit:
	os_free(buf_p);
	buf_p = NULL;
}

void test_fatfs_format(DISK_NUMBER number)
{
	FRESULT fr;
	char cFileName[FF_MAX_LFN];
	//unsigned char ucRdTemp[WR_RD_BUF_SIZE];
	unsigned char *ucRdTemp = os_malloc(WR_RD_BUF_SIZE);
	if(ucRdTemp == 0)
	{
		FATFS_LOGE("%s:os_malloc fail \r\n", __func__);
		return;
	}

	FATFS_LOGD("----- test_fatfs_format %d start -----\r\n", number);

	sprintf(cFileName, "%d:", number);
	fr = f_mkfs(cFileName, FM_ANY, 65536, ucRdTemp, WR_RD_BUF_SIZE);
	if (fr != FR_OK)
	{
		os_free(ucRdTemp);
		FATFS_LOGE("f_mkfs fr=%d failed!\r\n", fr);
		return;
	}
	else
	{
		FATFS_LOGI("f_mkfs OK!\r\n");
	}

	FATFS_LOGD("----- test_fatfs_format %d over  -----\r\n", number);

	if(ucRdTemp)
	{
		os_free(ucRdTemp);
		ucRdTemp = 0;
	}
}
#endif

// eof

