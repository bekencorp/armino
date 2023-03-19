// Copyright 2022-2025 Beken
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


#ifndef __MCU_IP_H__
#define __MCU_IP_H__

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * System clock
 ****************************************************************************/
#define KHz                  1000
#define MHz                  1000000

#define CPUFREQ             (60 * MHz)

/*****************************************************************************
 * Device Specific Peripheral Registers structures
 ****************************************************************************/

#define __I                     volatile const	/* 'read only' permissions      */
#define __O                     volatile        /* 'write only' permissions     */
#define __IO                    volatile        /* 'read / write' permissions   */

#define _IO_(addr)              (addr)

#define PLIC_BASE               _IO_(0xE4000000)
#define PLMT_BASE               _IO_(0xE6000000)
#define PLIC_SW_BASE            _IO_(0xE6400000)
#define PLDM_BASE               _IO_(0xE6800000)
#define SMU_BASE                _IO_(0xF0100000)

/*****************************************************************************
 * PLMT
 ****************************************************************************/
typedef struct {
	__IO unsigned long long MTIME;          /* 0x00 Machine Time */
	__IO unsigned long long MTIMECMP;       /* 0x08 Machine Time Compare */
} PLMT_RegDef;

typedef struct {
	__I  unsigned int SYSTEMVER;            /* 0x00 SYSTEM ID and Revision Register */
	     unsigned int RESERVED0[3];         /* 0x04 ~ 0x0C Reserved */
	__IO unsigned int WRSR;                 /* 0x10 Wakeup and Reset Status Register */
	__IO unsigned int SMUCR;                /* 0x14 SMU Command Register */
	     unsigned int RESERVED1[1];         /* 0x18 Reserved */
	__IO unsigned int WRMASK;               /* 0x1C Wake up Mask Register */
	__IO unsigned int CER;                  /* 0x20 Clock Enable Register */
	__IO unsigned int CRR;                  /* 0x24 Clock Ratio Register */
	     unsigned int RESERVED2[6];         /* 0x28 ~ 0x3C Reserved Register */
	__IO unsigned int SCRATCH;              /* 0x40 Scratch Register */
	     unsigned int RESERVED3[3];         /* 0x44 ~ 0x4C Reserved */
	__IO unsigned int RESET_VECTOR;         /* 0x50 CPU Reset Vector Register */
} SMU_RegDef;
/*****************************************************************************
 * Peripheral device declaration
 ****************************************************************************/

#define MCUIP_PLMT              ((PLMT_RegDef *) PLMT_BASE)
#define MCUIP_SMU               ((SMU_RegDef *)  SMU_BASE)
#ifdef __cplusplus
}
#endif

#endif	/* __MCU_IP_H__ */
