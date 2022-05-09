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
#define TEST_TXT_FILE_NAME1     "/中文名字可以有多长？想多长就多长，不信你咬我，信我请我吃饭.txt"

#if CONFIG_SDCARD_HOST
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
        os_printf("%s/\r\n", path);
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
                os_printf("%s/%s\r\n", path, fno.fname);
            }
        }
        f_closedir(&dir);
    }
    else
    {
        os_printf("f_opendir failed\r\n");
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
		os_printf("f_mount malloc failed!\r\n");
		goto failed_mount;
	}

    sprintf(cFileName, "%d:", number);
    fr = f_mount(pfs, cFileName, 1);
    if (fr != FR_OK)
    {
        os_printf("f_mount failed:%d\r\n", fr);
    }
    else
    {
        os_printf("f_mount OK!\r\n");
    }
	
failed_mount:
    os_printf("----- test_mount %d over  -----\r\n\r\n", number);
}

void scan_file_system(DISK_NUMBER number)
{
    FRESULT fr;
    char cFileName[FF_MAX_LFN];

    os_printf("\r\n----- scan_file_system %d start -----\r\n", number);

    sprintf(cFileName, "%d:", number);
    fr = scan_files(cFileName);
    if (fr != FR_OK)
    {
        os_printf("scan_files failed!\r\n");
    }
    else
    {
        os_printf("scan_files OK!\r\n");
    }

    os_printf("----- scan_file_system %d over  -----\r\n\r\n", number);
}

void test_fatfs_read(DISK_NUMBER number, char *filename)
{
    char cFileName[FF_MAX_LFN];
    FIL file;
    FRESULT fr;
	unsigned char *ucRdTemp = os_malloc(WR_RD_BUF_SIZE);
	if(ucRdTemp == 0)
	{
		os_printf("%s:os_malloc fail \r\n", __func__);
		goto error1;
	}

    os_printf("\r\n----- test_fatfs %d start -----\r\n", number);
	if(filename)
		sprintf(cFileName, "%d:/%s", number, filename);
	else
		sprintf(cFileName, "%d:%s", number, TEST_TXT_FILE_NAME);

    os_printf("f_open \"%s\"\r\n", cFileName);
    fr = f_open(&file, cFileName, FA_OPEN_EXISTING | FA_READ);
    if (fr == FR_OK)
    {
        unsigned int uiTemp = 0;
        unsigned int uiRdTotalLength = 0;
        //unsigned char ucRdTemp[WR_RD_BUF_SIZE];
		
        do
        {
            os_printf(".");
            fr = f_read(&file, ucRdTemp, WR_RD_BUF_SIZE, &uiTemp);
            if (fr == FR_OK)
            {
            	
            }
            else
            {
                os_printf("f_read failed 1 fr = %d\r\n", fr);
                goto error1;
            }
            uiRdTotalLength += uiTemp;
        }
        while (uiTemp == WR_RD_BUF_SIZE);
        os_printf("\r\n");
		for(int i=0; i < uiRdTotalLength; i++)
		{
			os_printf("%c", ucRdTemp[i]);
		}
        os_printf("f_read: read total byte = %d\r\n", uiRdTotalLength);

        fr = f_close(&file);
        if (fr != FR_OK)
        {
            os_printf("f_close failed 1 fr = %d\r\n", fr);
            goto error1;
        }
        else
        {
            os_printf("f_close OK\r\n");
        }
    }
    else
    {
        os_printf("f_open failed 1 fr = %d\r\n", fr);
    }

    os_printf("----- test_fatfs %d over  -----\r\n\r\n", number);
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

    os_printf("\r\n----- test_fatfs %d start -----\r\n", number);
	if(filename)
		sprintf(cFileName, "%d:/%s", number, filename);
	else
		sprintf(cFileName, "%d:%s", number, TEST_TXT_FILE_NAME);
    os_printf("f_open \"%s\"\r\n", cFileName);

    fr = f_open(&file, cFileName, FA_OPEN_APPEND | FA_WRITE);
    if (fr == FR_OK)
    {
        unsigned int uiTemp = 0;
        unsigned int uiRdTotalLength = 0;

		len = len > 64? 64:len;
        do
        {
            os_printf(".");
            fr = f_write(&file, content, len, &uiTemp);
            if (fr == FR_OK)
            {
            	
            }
            else
            {
                os_printf("f_write failed 1 fr = %d\r\n", fr);
                goto error1;
            }
            uiRdTotalLength += uiTemp;
        }
        while (uiRdTotalLength != len);

        fr = f_close(&file);
        if (fr != FR_OK)
        {
            os_printf("f_close failed 1 fr = %d\r\n", fr);
            goto error1;
        }
        else
        {
            os_printf("f_close OK\r\n");
        }
    }
    else
    {
        os_printf("f_open failed 1 fr = %d\r\n", fr);
    }

    os_printf("----- test_fatfs %d over  -----\r\n\r\n", number);
    return;

error1:
    f_close(&file);
}

void test_fatfs_format(DISK_NUMBER number)
{
    FRESULT fr;
    char cFileName[FF_MAX_LFN];
    //unsigned char ucRdTemp[WR_RD_BUF_SIZE];
	unsigned char *ucRdTemp = os_malloc(WR_RD_BUF_SIZE);
	if(ucRdTemp == 0)
	{
		os_printf("%s:os_malloc fail \r\n", __func__);
		return;
	}

    os_printf("----- test_fatfs_format %d start -----\r\n", number);

    sprintf(cFileName, "%d:", number);
    fr = f_mkfs(cFileName, FM_ANY, 65536, ucRdTemp, WR_RD_BUF_SIZE);
    if (fr != FR_OK)
    {
        os_printf("f_mkfs fr=%d failed!\r\n", fr);
    }
    else
    {
        os_printf("f_mkfs OK!\r\n");
    }

    os_printf("----- test_fatfs_format %d over  -----\r\n", number);

	if(ucRdTemp)
	{
		os_free(ucRdTemp);
		ucRdTemp = 0;
	}
}
#endif

// eof

