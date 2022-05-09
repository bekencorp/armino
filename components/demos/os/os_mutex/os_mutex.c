/**
 ******************************************************************************
 * @file    os_mutex.c
 * @author
 * @version V1.0.0
 * @date
 * @brief   RTOS mutex demo.
 ******************************************************************************
 *
 *  The MIT License
 *  Copyright (c) 2017 Beken Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is furnished
 *  to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ******************************************************************************
 */
#include "os_mutex.h"
#include <common/bk_include.h>

#if OS_MUTEX_DEMO
#include <common/bk_include.h>
#include <os/os.h>
#include "uart_pub.h"
#include "Error.h"
#include "portmacro.h"
#include <stdlib.h>

static beken_mutex_t os_mutex = NULL;

bk_err_t mutex_printf_msg(char *s)
{
    bk_err_t err = kNoErr;
    if(os_mutex == NULL)
    {
        return -1;
    }
    err = rtos_lock_mutex(&os_mutex);
    if(err != kNoErr)
    {
        return err;
    }
    os_printf( "%s\r\n", s);
    err = rtos_unlock_mutex(&os_mutex);
    if(err != kNoErr)
    {
        return err;
    }
    return err;
}

void os_mutex_sender_thread( beken_thread_arg_t arg )
{
    bk_err_t err = kNoErr;
    char *taskname = (char *)arg;
    char strprt[100];
    int rd;
    while ( 1 )
    {
        rd = rand() & 0x1FF;
        sprintf(strprt, "%s , Rand:%d", taskname, rd);
        err = mutex_printf_msg(strprt);
        if(err != kNoErr)
        {
            os_printf( "%s printf_msg error!\r\n", taskname);
            goto exit;
        }
        rtos_delay_milliseconds( rd );
    }

exit:
    if ( err != kNoErr )
    {
        os_printf( "Sender exit with err: %d\r\n", err );
    }
    if(os_mutex != NULL)
    {
        rtos_deinit_mutex(&os_mutex);
    }
    rtos_delete_thread( NULL );
}

int demo_start( void )
{
    bk_err_t err = kNoErr;

    err = rtos_init_mutex( &os_mutex );

    if(err != kNoErr)
    {
        os_printf( "rtos_init_mutex err: %d\r\n", err );
        goto exit;
    }

    err = rtos_create_thread( NULL,
                              BEKEN_APPLICATION_PRIORITY,
                              "sender1",
                              os_mutex_sender_thread,
                              0x400,
                              "my name is thread1");
    if(err != kNoErr)
    {
        goto exit;
    }

    err = rtos_create_thread( NULL,
                              BEKEN_APPLICATION_PRIORITY,
                              "sender2",
                              os_mutex_sender_thread,
                              0x400,
                              "I'm is task!" );
    if(err != kNoErr)
    {
        goto exit;
    }
exit:
    if ( err != kNoErr )
    {
        os_printf( "Thread exit with err: %d\r\n", err );
    }
    return err;
}

#endif

