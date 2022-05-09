/**
  ******************************************************************************
 * @file    os_timer.c
 * @author  (^_^)
 * @version V1.0.0
 * @date    2017/10/30
 * @brief   RTOS timer demo.
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
#include "os_timer.h"
#include <common/bk_include.h>

#if OS_TIMER_DEMO
#include <common/bk_include.h>
#include <os/os.h>
#include "uart_pub.h"
#include "Error.h"
#include "portmacro.h"

beken_timer_t timer_handle, timer_handle2;

void destroy_timer( void )
{
    /* Stop software timer(timer_handle) */
    rtos_stop_timer( &timer_handle );
    /* delete software timer(timer_handle) */
    rtos_deinit_timer( &timer_handle );
    /* Stop software timer(timer_handle2) */
    rtos_stop_timer( &timer_handle2 );
    /* delete software timer(timer_handle2) */
    rtos_deinit_timer( &timer_handle2 );
}

void timer_alarm( void *arg )
{
    os_printf("I'm timer_handle1\r\n");
}

void timer2_alarm( void *arg )
{
    os_printf("I'm timer_handle2,destroy timer!\r\n");

    destroy_timer();
}

int demo_start( void )
{
    bk_err_t err = kNoErr;

    os_printf("timer demo\r\n");

    /* Create a new software timer,software is AutoReload */
    err = rtos_init_timer(&timer_handle, 500, timer_alarm, 0);  ///500mS
    if(kNoErr != err)
        goto exit;

    /* Create a new software timer,software is AutoReload */
    err = rtos_init_timer(&timer_handle2, 2600, timer2_alarm, 0);  ///2.6S
    if(kNoErr != err)
        goto exit;

    /* start (timer_handle) timer */
    err = rtos_start_timer(&timer_handle);
    if(kNoErr != err)
        goto exit;

    /* start (timer_handle2) timer */
    err = rtos_start_timer(&timer_handle2);
    if(kNoErr != err)
        goto exit;

    return err;

exit:
    if( err != kNoErr )
        os_printf( "os timer exit with err: %d", err );

    return err;
}

#endif
// eof

