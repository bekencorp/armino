/**
 ******************************************************************************
 * @file    os_queue.c
 * @author
 * @version V1.0.0
 * @date
 * @brief   RTOS queue demo.
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
#include "os_queue.h"
#include <common/bk_include.h>

#if OS_QUEUE_DEMO

#include <common/bk_include.h>
#include <os/os.h>
#include "uart_pub.h"
#include "Error.h"
#include "portmacro.h"

typedef struct _msg
{
    int value;
} msg_t;


static beken_queue_t os_queue = NULL;

void receiver_thread( beken_thread_arg_t arg )
{
    bk_err_t err;
    msg_t received = { 0 };

    while ( 1 )
    {
        /*Wait until queue has data*/
        err = rtos_pop_from_queue( &os_queue, &received, BEKEN_NEVER_TIMEOUT);
        if(err == kNoErr)
        {
            os_printf( "Received data from queue:value = %d\r\n", received.value );
        }
        else
        {
            os_printf("Received data from queue failed:Err = %d\r\n", err);
            goto exit;
        }
    }

exit:
    if ( err != kNoErr )
        os_printf( "Receiver exit with err: %d\r\n", err );

    rtos_delete_thread( NULL );
}

void sender_thread( beken_thread_arg_t arg )
{
    bk_err_t err = kNoErr;

    msg_t my_message = { 0 };

    while ( 1 )
    {
        my_message.value++;
        err = rtos_push_to_queue(&os_queue, &my_message, BEKEN_NEVER_TIMEOUT);
        if(err == kNoErr)
        {
            os_printf( "send data to queue\r\n" );
        }
        else
        {
            os_printf("send data to queue failed:Err = %d\r\n", err);
        }
        rtos_delay_milliseconds( 100 );
    }

exit:
    if ( err != kNoErr )
    {
        os_printf( "Sender exit with err: %d\r\n", err );
    }

    rtos_delete_thread( NULL );
}

int demo_start( void )
{
    bk_err_t err = kNoErr;

    err = rtos_init_queue( &os_queue, "queue", sizeof(msg_t), 3 );

    if(err != kNoErr)
    {
        goto exit;
    }
    err = rtos_create_thread( NULL,
                              BEKEN_APPLICATION_PRIORITY,
                              "sender",
                              sender_thread,
                              0x500,
                              0 );
    if(err != kNoErr)
    {
        goto exit;
    }
    err = rtos_create_thread( NULL,
                              BEKEN_APPLICATION_PRIORITY,
                              "receiver",
                              receiver_thread,
                              0x500,
                              0 );
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

