/**********************************************************************************

* Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
* All rights reserved.
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation and/or
* other materials provided with the distribution.
* Neither the name of the copyright holder nor the names of its contributors
* may be used to endorse or promote products derived from this software without
* specific prior written permission.
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************/
/**********************************************************************************

* Notice of Export Control Law
===============================================
* Huawei LiteOS may be subject to applicable export control laws and regulations, which
* might include those applicable to Huawei LiteOS of U.S. and the country in which you
* are located.
* Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance
* with such applicable export control laws and regulations.
***********************************************************************************/


#ifndef LWIP_API_SHELL_H
#define LWIP_API_SHELL_H

#include "arch/cc.h"
#include "lwip/opt.h"

#if defined (__cplusplus) && __cplusplus
extern "C" {
#endif

u32_t lwip_ifconfig(int argc, const char **argv);
u32_t OsShellPing(int argc, const char **argv);
LWIP_STATIC int OsPingFunc(u32_t *parg);

#if defined (__cplusplus) && __cplusplus
}
#endif
#endif /* LWIP_HDR_API_SHELL_H */
