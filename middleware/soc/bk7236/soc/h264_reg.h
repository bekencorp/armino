// Copyright 2022-2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//		 http://www.apache.org/licenses/LICENSE-2.0
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

#define H264_R_BASE					(SOC_H264_REG_BASE)

#define H264_R_INT_STATUS			(H264_R_BASE + (0x46)*4)

#define H264_R_RX_FIFO				(H264_R_BASE + (0x48)*4)

#define H264_SYS_REG_SET(reg_id, l, h, v) REG_SET((SOC_H264_REG_BASE+ ((reg_id) << 2)), (l), (h), (v))

#define H264_REG_OR(reg_id, l, h, v)  REG_OR((SOC_H264_REG_BASE+ ((reg_id) << 2)), (l), (h), (v))

#ifdef __cplusplus		
}
#endif