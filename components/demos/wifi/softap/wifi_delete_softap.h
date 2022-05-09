/**
 ******************************************************************************
 * @file    wifi_delete_softap.h
 * @author
 * @version V1.0.0
 * @date
 * @brief   delete a built sotfap demo
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

#ifndef _WIFI_DELETE_SOFTAP_H_
#define _WIFI_DELETE_SOFTAP_H_

#ifndef IN
#define IN
#endif

#ifndef CONST
#define CONST const
#endif

/* bk72xx do AP encryption mode */
typedef enum
{
    DWF_OPEN = 0,     /* none encryption */
    DWF_WEP,          /* WEP */
    DWF_WPA_PSK,      /*WPA PSK */
    DWF_WPA2_PSK,     /*WPA2 PSK */
    DWF_WPA_WPA2_PSK, /* WPA/WPA2 PSK */
} Demo_WF_AP_AUTH_MODE_E;

extern int demo_start(void);

#endif

