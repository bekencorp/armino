#include "test_flash.h"
#include <common/bk_include.h>

#if TEST_FLASH_DEMO
#include "string.h"
#include "stdlib.h"
#include <os/mem.h>
#include "uart_pub.h"
#include "gpio_pub.h"
#include <os/os.h>
#include "Error.h"
#include "flash_pub.h"

beken_timer_t *test_flash_demo_timer = NULL;
DD_HANDLE test_flash_demo_handle = -1;

void test_flash_demo_init(void)
{
    UINT32 status;

    test_flash_demo_handle = ddev_open(FLASH_DEV_NAME, &status, 0);
    if(status != 0)
    {
        os_printf("flash_open failed\n");
    }
}

UINT32 test_flash_demo_wirte(UINT32 addr, UINT32 count, IN char *inbuf)
{
    UINT32 status;

    if(test_flash_demo_handle < 0)
    {
        return -1;
    }

    status = ddev_write(test_flash_demo_handle, inbuf, count, addr);
    if(status == 0)
    {
        return 1;
    }
    return 0;
}

int test_flash_demo_read(UINT32 addr, UINT32 count, OUT char *buf)
{
    UINT32 status;

    if(test_flash_demo_handle < 0)
    {
        return -1;
    }

    if(buf)
    {
        status = ddev_read(test_flash_demo_handle, buf, count, addr);
        if(status == 0)
        {
            return 1;
        }
    }
    return 0;
}

void test_flash_demo_show(UINT32 addr, UINT32 count)
{
    UINT32 param;
    char *cTemp;
    int i;

    if(test_flash_demo_handle < 0)
    {
        return;
    }

    cTemp = os_malloc(count);
    if(cTemp)
    {
        ddev_read(test_flash_demo_handle, cTemp, count, addr);
        os_printf("read:\r\n");
        for (i = 0; i < count; i++)
        {
            os_printf("%02x ", (unsigned char)cTemp[i]);
        }
        os_printf("\n\n");
        os_free(cTemp);
    }
}


int test_flash_compare_buf(char *buf1, char *buf2, UINT32 len)
{
    UINT32 i;

    for(i = 0; i < len; i++)
    {
        if(buf1[i] != buf2[i])
        {
            return -1;
        }
    }
    return 0;
}

void test_flash_demo_done(void)
{
    char write[TEST_FLASH_DEMO_LENGTH];
    char read[TEST_FLASH_DEMO_LENGTH];
    unsigned char dat;
    int i, status;
    UINT32 param;

    bk_flash_enable_security(FLASH_PROTECT_NONE);
    param = TEST_FLASH_DEMO_ADDR;
    ddev_control(test_flash_demo_handle, CMD_FLASH_ERASE_SECTOR, &param);

    dat = rand();
    os_printf("rand:%02x\n", dat);
    for(i = 0; i < TEST_FLASH_DEMO_LENGTH; i++)
        write[i] = dat + i;

    ddev_write(test_flash_demo_handle, write, TEST_FLASH_DEMO_LENGTH, TEST_FLASH_DEMO_ADDR);
    ddev_read(test_flash_demo_handle, read, TEST_FLASH_DEMO_LENGTH, TEST_FLASH_DEMO_ADDR);

    status = test_flash_compare_buf(read, write, TEST_FLASH_DEMO_LENGTH);
    if(status == 0)
    {
        os_printf("write ok!\n");
    }
    else
    {
        os_printf("write failed!\n");
    }
}

void test_flash_demo_timer_alarm( void *arg )
{
    test_flash_demo_show(TEST_FLASH_DEMO_ADDR, TEST_FLASH_DEMO_LENGTH);
    test_flash_demo_done();
}

int create_test_flash_demo_timer(void)
{
    bk_err_t err;

    if(test_flash_demo_timer == NULL)
    {
        test_flash_demo_timer = (beken_timer_t *) os_malloc( sizeof(beken_timer_t) );
    }

    if(test_flash_demo_timer == NULL)
    {
        os_printf("test_flash_demo_timer error!\r\n");
        return kGeneralErr;
    }

    err = rtos_init_timer(test_flash_demo_timer, 1500, test_flash_demo_timer_alarm, 0);
    if(kNoErr != err)
    {
        if(test_flash_demo_timer)
        {
            os_free(test_flash_demo_timer);
            test_flash_demo_timer = NULL;
        }
        os_printf("test_flash_demo_timer init failed!\r\n");
    }

    return err;
}

void destroy_test_flash_demo_timer( void )
{
    rtos_deinit_timer( test_flash_demo_timer );
    if(test_flash_demo_timer)
    {
        os_free(test_flash_demo_timer);
        test_flash_demo_timer = NULL;
    }
}

int demo_start(void)
{
    test_flash_demo_init();

    if(create_test_flash_demo_timer() == kNoErr)
    {
        if(kNoErr == rtos_start_timer(test_flash_demo_timer))
        {
            return kNoErr;
        }
        destroy_test_flash_demo_timer();
        os_printf("flash timer start failed!\r\n");
    }

    return kGeneralErr;
}

#endif /*TEST_FLASH_DEMO*/
// EOF

