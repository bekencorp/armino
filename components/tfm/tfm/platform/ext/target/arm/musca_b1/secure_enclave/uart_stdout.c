/*
 * Copyright (c) 2017-2020, Arm Limited. All rights reserved.
 *
 * Licensed under the Apace License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apace.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "uart_stdout.h"
#include <stdio.h>

int stdio_output_string(const unsigned char *str, uint32_t len)
{
    /* Nothing to do as there is no serial output */
    (void)str;
    return len;
}

/* Redirects printf to STDIO_DRIVER in case of ARMCLANG*/
#if defined(__ARMCC_VERSION)
/* Struct FILE is implemented in stdio.h. Used to redirect printf to
 * STDIO_DRIVER
 */
FILE __stdout;
/* __ARMCC_VERSION is only defined starting from Arm compiler version 6 */
int fputc(int ch, FILE *f)
{
    (void)f;

    /* Send byte to USART */
    (void)stdio_output_string((const unsigned char *)&ch, 1);

    /* Return character written */
    return ch;
}
#elif defined(__GNUC__)
/* Redirects printf to STDIO_DRIVER in case of GNUARM */
int _write(int fd, char *str, int len)
{
    (void)fd;

    /* Send string and return the number of characters written */
    return stdio_output_string((const unsigned char *)str, (uint32_t)len);
}
#elif defined(__ICCARM__)
int putchar(int ch)
{
    /* Send byte to USART */
    (void)stdio_output_string((const unsigned char *)&ch, 1);

    /* Return character written */
    return ch;
}
#endif

void stdio_init(void)
{
    /* Nothing to do as there is no serial output */
}

void stdio_uninit(void)
{
    /* Nothing to do as there is no serial output */
}
