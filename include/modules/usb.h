// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif
/*******************************************************************************
* Function Declarations
*******************************************************************************/
UINT32 usb_open (UINT32 op_flag);
UINT32 usb_close (void);
#if (CONFIG_SOC_BK7271)
UINT32 usb_read (UINT32 pos, const void *buffer, UINT32 size);
UINT32 usb_write (UINT32 pos, const void *buffer, UINT32 size);
#elif (CONFIG_SOC_BK7256XX)
UINT32 usb_read (UINT32 pos, const void *buffer, UINT32 size);
UINT32 usb_write (UINT32 pos, const void *buffer, UINT32 size);
#else
UINT32 usb_read (char *user_buf, UINT32 count, UINT32 op_flag);
UINT32 usb_write (char *user_buf, UINT32 count, UINT32 op_flag);
#endif
UINT32 usb_ctrl(UINT32 cmd, void *param);
void usb_event_post(void);
void usb_isr(void);
void usb_check_int_handler(void);

#ifdef __cplusplus
}
#endif