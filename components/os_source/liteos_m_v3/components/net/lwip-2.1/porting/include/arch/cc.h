/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _LWIP_PORTING_CC_H_
#define _LWIP_PORTING_CC_H_

#ifdef LITTLE_ENDIAN
#undef LITTLE_ENDIAN
#endif

#ifdef BIG_ENDIAN
#undef BIG_ENDIAN
#endif

#include <endian.h>
#include <stdio.h>
#include <stdlib.h>
#include "securec.h"

#ifdef htons
#define LWIP_DONT_PROVIDE_BYTEORDER_FUNCTIONS
#endif

#define SOCKLEN_T_DEFINED
#define SA_FAMILY_T_DEFINED
#define IN_PORT_T_DEFINED

#define LWIP_TIMEVAL_PRIVATE    0
#define LWIP_ERRNO_STDINCLUDE
#define LWIP_SOCKET_STDINCLUDE

#define LWIP_DNS_API_DEFINE_ERRORS    0
#define LWIP_DNS_API_DEFINE_FLAGS     0
#define LWIP_DNS_API_DECLARE_STRUCTS  0
#define LWIP_DNS_API_DECLARE_H_ERRNO  0

#ifndef __SIZEOF_POINTER__
#define __SIZEOF_POINTER__ 4   // 32 bit system
#endif

#define LOS_TASK_STATUS_DETACHED   0x0100  // reserved

#if defined(__arm__) && defined(__ARMCC_VERSION)
    /* Keil uVision4 tools */
    #define PACK_STRUCT_BEGIN __packed
    #define PACK_STRUCT_STRUCT
    #define PACK_STRUCT_END
    #define PACK_STRUCT_FIELD(fld) fld
    #define ALIGNED(n)  __align(n)
#elif defined (__IAR_SYSTEMS_ICC__)
    /* IAR Embedded Workbench tools */
    #define PACK_STRUCT_BEGIN __packed
    #define PACK_STRUCT_STRUCT
    #define PACK_STRUCT_END
    #define PACK_STRUCT_FIELD(fld) fld
    // #error NEEDS ALIGNED
#else
    /* GCC tools (CodeSourcery) */
    #define PACK_STRUCT_BEGIN
    #define PACK_STRUCT_STRUCT __attribute__ ((__packed__))
    #define PACK_STRUCT_END
    #define PACK_STRUCT_FIELD(fld) fld
    #define ALIGNED(n)  __attribute__((aligned (n)))
#endif

#define LWIP_RAND rand

#ifndef LWIP_LOGGER
#define LWIP_LOGGER(msg)
#endif

extern void LwipLogPrintf(const char *fmt, ...);
#define LWIP_PLATFORM_DIAG(vars) LwipLogPrintf vars
#define LWIP_PLATFORM_ASSERT(x) do { \
        LWIP_PLATFORM_DIAG(("Assertion \"%s\" failed at line %d in %s\n", x, __LINE__, __FILE__)); \
    } while (0)

#define init_waitqueue_head(...)
#define poll_check_waiters(...)

#endif /* _LWIP_PORTING_CC_H_ */
