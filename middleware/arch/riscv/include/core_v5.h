// Copyright 2020-2022 Beken
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

#ifndef __CORE_V5_H__
#define __CORE_V5_H__

#define MSTATUS_UIE         	0x00000001
#define MSTATUS_MIE         	0x00000008
#define MSTATUS_UPIE        	0x00000010
#define MSTATUS_MPIE        	0x00000080
#define MSTATUS_MPP         	0x00001800
#define MSTATUS_FS          	0x00006000
#define MSTATUS_XS          	0x00018000
#define MSTATUS_MPRV        	0x00020000
#define MSTATUS_SUM         	0x00040000
#define MSTATUS_MXR				0x00080000
#define MSTATUS_TW				0x00200000
#define MSTATUS_VM          	0x1F000000
#define MSTATUS32_SD        	0x80000000
#define MSTATUS64_SD        	0x8000000000000000

#define USTATUS_UIE         	0x00000001
#define USTATUS_UPIE        	0x00000010

#if __riscv_xlen == 64
#define MCAUSE_INT          	0x8000000000000000UL
#define MCAUSE_CAUSE        	0x7FFFFFFFFFFFFFFFUL
#else
#define MCAUSE_INT          	0x80000000UL
#define MCAUSE_CAUSE        	0x7FFFFFFFUL
#endif

#define UCAUSE_INT          	0x80000000UL
#define UCAUSE_CAUSE        	0x7FFFFFFFUL

#define IRQ_U_SOFT          	0
#define IRQ_M_SOFT          	3
#define IRQ_U_TIMER         	4
#define IRQ_M_TIMER         	7
#define IRQ_U_EXT           	8
#define IRQ_M_EXT           	11

/* Machine mode MCAUSE */
#define M_EXCP_I_ADDR_MISALIGN  0   /* Instruction address misaligh */
#define M_EXCP_I_ACC_FAULT    	1   /* Instruction access fault */
#define M_EXCP_I_ILLEGAL    	2   /* Instruction illegal */
#define M_EXCP_I_BREAK    		3   /* Break */
#define M_EXCP_L_ADDR_MISALIGN  4   /* Data load address misalign */
#define M_EXCP_L_ACC_FAULT   	5   /* Data load access fault */
#define M_EXCP_S_ADDR_MISALIGN  6   /* Data store address misalign */
#define M_EXCP_S_ACC_FAULT   	7   /* Data store access fault */
#define M_EXCP_U_ECALL        	8
#define M_EXCP_M_ECALL        	11
#define M_EXCP_STACKOVF     	32
#define M_EXCP_STACKUDF     	33
#define TRAP_M_USER_ASSERT      0x77
#define TRAP_M_USER_NP_WDT          0x88
#define TRAP_M_USER_OTHER       0x99

/* User mode UCAUSE */
#define U_EXCP_I_ADDR_MISALIGN  0   /* Instruction address misaligh */
#define U_EXCP_I_ACC_FAULT    	1   /* Instruction access fault */
#define U_EXCP_I_ILLEGAL    	2   /* Instruction illegal */
#define U_EXCP_I_BREAK    		3   /* Break */
#define U_EXCP_L_ADDR_MISALIGN  4   /* Data load address misalign */
#define U_EXCP_L_ACC_FAULT   	5   /* Data load access fault */
#define U_EXCP_S_ADDR_MISALIGN  6   /* Data store address misalign */
#define U_EXCP_S_ACC_FAULT   	7   /* Data store access fault */
#define U_EXCP_ECALL        	8

#define MIP_USIP            	(1 << IRQ_U_SOFT)
#define MIP_MSIP            	(1 << IRQ_M_SOFT)
#define MIP_UTIP            	(1 << IRQ_U_TIMER)
#define MIP_MTIP            	(1 << IRQ_M_TIMER)
#define MIP_UEIP            	(1 << IRQ_U_EXT)
#define MIP_MEIP            	(1 << IRQ_M_EXT)

#define UIP_USIP            	(1 << IRQ_U_SOFT)
#define UIP_UTIP            	(1 << IRQ_U_TIMER)
#define UIP_UEIP            	(1 << IRQ_U_EXT)

#define RV_USER_MODE			0
#define RV_MACHINE_MODE			3

#if __riscv_xlen == 64
# define SLL32    		sllw
# define STORE    		sd
# define LOAD     		ld
# define LWU      		lwu
# define LOG_REGBYTES 		3
#else
# define SLL32    		sll
# define STORE    		sw
# define LOAD     		lw
# define LWU      		lw
# define LOG_REGBYTES		2
#endif
#define REGBYTES		(1 << LOG_REGBYTES)

#if __riscv_flen == 64
# define FPSTORE		fsd
# define FPLOAD			fld
# define LOG_FPREGBYTES		3
#else
# define FPSTORE		fsw
# define FPLOAD			flw
# define LOG_FPREGBYTES		2
#endif
#define FPREGBYTES              (1 << LOG_FPREGBYTES)

#define STR(S)			#S
#define XSTR(S)			STR(S)

#define PUSH			XSTR(STORE)
#define POP			XSTR(LOAD)
#define REGSIZE			XSTR(REGBYTES)
#define FPPUSH			XSTR(FPSTORE)
#define FPPOP			XSTR(FPLOAD)
#define FPREGSIZE		XSTR(FPREGBYTES)

#ifndef __ASSEMBLER__

#include <nds_intrinsic.h>

#define read_csr(reg)		__nds__csrr(reg)
#define write_csr(reg, val)	__nds__csrw(val, reg)
#define swap_csr(reg, val)	__nds__csrrw(val, reg)
#define set_csr(reg, bit)	__nds__csrrs(bit, reg)
#define clear_csr(reg, bit)	__nds__csrrc(bit, reg)

#endif

#endif	// __CORE_V5_H__

