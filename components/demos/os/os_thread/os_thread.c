/**
 ******************************************************************************
 * @file    os_thread.c
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
#include "os_thread.h"
#include <common/bk_include.h>

#if OS_THREAD_DEMO
#include <common/bk_include.h>
#include <os/os.h>
#include "uart_pub.h"
#include "Error.h"
#include "portmacro.h"

void thread_0( beken_thread_arg_t arg )
{
    (void)( arg );

    os_printf( "This is thread 0\r\n");
    rtos_delay_milliseconds((TickType_t)1000 );

    /* Make with terminate state and IDLE thread will clean resources */
    rtos_delete_thread(NULL);
}

void thread_1( beken_thread_arg_t arg )
{
    (void)( arg );
    bk_err_t err = kNoErr;
    beken_thread_t t_handler = NULL;

    while ( 1 )
    {
        /* Create a new thread, and this thread will delete its self and clean its resource */
        err = rtos_create_thread( &t_handler,
                                  BEKEN_APPLICATION_PRIORITY,
                                  "Thread 0",
                                  thread_0,
                                  0x400,
                                  0);
        if(err != kNoErr)
        {
            os_printf("ERROR: Unable to start the thread 1.\r\n" );
        }
        /* wait thread 0 delete it's self */
        rtos_thread_join( &t_handler );
    }
}

void thread_2( beken_thread_arg_t arg )
{
    (void)( arg );

    while ( 1 )
    {
        os_printf( "This is thread 2\r\n" );
        rtos_delay_milliseconds((TickType_t)600);
    }
}

/***************************************************************
   This function make two thread for Os_thread application.
***************************************************************/
int demo_start( void )
{
    bk_err_t err = kNoErr;
    beken_thread_t t_handler1 = NULL, t_handler2 = NULL;

    os_printf("\r\n\r\noperating system thread demo............\r\n" );

    err = rtos_create_thread( &t_handler1, BEKEN_APPLICATION_PRIORITY,
                              "Thread 1",
                              thread_1,
                              0x400,
                              0);
    if(err != kNoErr)
    {
        os_printf("ERROR: Unable to start the thread 1.\r\n" );
        goto exit;
    }

    err = rtos_create_thread( &t_handler2, BEKEN_APPLICATION_PRIORITY,
                              "Thread 2",
                              thread_2,
                              0x400,
                              0);
    if(err != kNoErr)
    {
        os_printf("ERROR: Unable to start the thread 2.\r\n" );
        goto exit;
    }

exit:
    if ( err != kNoErr )
    {
        os_printf( "Thread exit with err: %d", err );

        if(t_handler1 != NULL)
        {
            rtos_delete_thread(t_handler1);
        }

        if(t_handler2 != NULL)
        {
            rtos_delete_thread(t_handler2);
        }
    }

    return err;
}


#endif
// eof

