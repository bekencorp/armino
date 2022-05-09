/**
 ******************************************************************************
 * @file    os_sem.c
 * @author
 * @version V1.0.0
 * @date
 * @brief   RTOS thread control demo.
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
#include "os_sem.h"
#include <common/bk_include.h>

#if OS_SEM_DEMO

#include <common/bk_include.h>
#include <os/os.h>
#include "uart_pub.h"
#include "Error.h"
#include "portmacro.h"

static beken_semaphore_t os_sem = NULL;

void set_semaphore_thread( beken_thread_arg_t arg )
{
    while ( 1 )
    {
        os_printf( "release semaphore!\r\n" );
        rtos_set_semaphore( &os_sem );
        rtos_delay_milliseconds( 500 );
    }

exit:
    if(os_sem)
    {
        rtos_deinit_semaphore(&os_sem);
    }
    rtos_delete_thread( NULL );
}

void get_semaphore_thread( beken_thread_arg_t arg )
{
    bk_err_t err;

    while(1)
    {
        err = rtos_get_semaphore(&os_sem, BEKEN_NEVER_TIMEOUT);
        if(err == kNoErr)
        {
            os_printf("Get_Sem Succend!\r\n");
        }
        else
        {
            os_printf("Get_Sem Err:%d\r\n", err);
            goto exit;
        }
    }

exit:
    if(os_sem)
    {
        rtos_deinit_semaphore(&os_sem);
    }
    rtos_delete_thread( NULL );
}


int demo_start( void )
{
    bk_err_t err = kNoErr;
    os_printf( "test binary semaphore\r\n" );

    err = rtos_init_semaphore( &os_sem, 1 ); //0/1 binary semaphore || 0/N semaphore

    if(err != kNoErr)
    {
        goto exit;
    }

    err = rtos_create_thread( NULL,
                              BEKEN_APPLICATION_PRIORITY,
                              "get_sem",
                              get_semaphore_thread,
                              0x500,
                              0 );
    if(err != kNoErr)
    {
        goto exit;
    }
    err = rtos_create_thread( NULL,
                              BEKEN_APPLICATION_PRIORITY,
                              "set_sem",
                              set_semaphore_thread,
                              0x500,
                              0 );
    if(err != kNoErr)
    {
        goto exit;
    }

    return err;
exit:
    if ( err != kNoErr )
    {
        os_printf( "Thread exit with err: %d\r\n", err );
    }
    return err;
}

#endif


