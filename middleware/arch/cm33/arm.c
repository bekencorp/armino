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

#include "bk_arch.h"
#include "bk7236.h"
__attribute__((section(".itcm_sec_code")))uint64_t check_IRQ_pending(void);

#if 0  // for test
void arm_enable_excall(void)
{
	SCnSCB->ACTLR |= 0x1 << 29;
}

void arm_mpu_init(void)
{
	uint32_t    i, reg_cnt = (MPU->TYPE >> 8) & 0xFF;
	for(i = 0; i < reg_cnt; i++)
	{
		MPU->RNR = i;
		MPU->RLAR = 0;
		MPU->RBAR = 0;
	}

	MPU->RNR = 0;
	MPU->RBAR = 0x28000000 | (0x02 << 3) | (0x01 << 1) | (0x00 << 0);
	MPU->RLAR = 0x2807FFE0 | (0x00 << 1) | (0x01 << 0);
	MPU->MAIR0 = 0x44;
	MPU->CTRL = (0x01 << 2) | (0x01 << 0);  /* PRIVDEFENA, ENABLE */

	__DSB();
	__ISB();
}
#endif

void arch_init(void)
{

}

void smem_reset_lastblock(void)
{

}

__attribute__((section(".itcm_sec_code"))) void arch_deep_sleep(void)
{
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	__WFI();
}

__attribute__((section(".iram"))) void arch_sleep(void)
{
	SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
	__WFI();
}

__attribute__((section(".itcm_sec_code")))uint64_t check_IRQ_pending(void)
{
	return NVIC_GetAllPendingIRQ();
}

void arch_atomic_set(  volatile uint32_t *lk ) /* PRIVILEGED_FUNCTION */
{
	int   result;

	do
	{
		while(__LDAEX(lk) != 0);

		result = __STREXW(1, lk);
	} while(result != 0);

	return;
}

void arch_atomic_clear(volatile uint32_t *lk)
{
	__STL(0, lk);

	return;
}

#if 0
void arch_atomic_set_v7(  volatile uint32_t *lk ) /* PRIVILEGED_FUNCTION */
{
	int   result;

	do
	{
		while(__LDREXW(lk) != 0);

		result = __STREXW(1, lk);
	} while(result != 0);

	__DMB();

	return;
}

void arch_atomic_clear_v7(volatile uint32_t *lk)
{
	__DMB();

	*lk = 0;
	
	return;
}

void arch_atomic_set(  volatile uint32_t *lk ) /* PRIVILEGED_FUNCTION */
{
	arch_atomic_set_v8(lk);
}

void arch_atomic_clear(volatile uint32_t *lk)
{
	arch_atomic_clear_v8(lk);
}
#endif

void arch_fence(void)
{
}

#if 0
uint32_t arch_int_disable( void ) __attribute__( ( naked ) );

uint32_t arch_int_disable(void) /* __attribute__(( naked )) PRIVILEGED_FUNCTION */
{
	__asm volatile
	(
	    "	mrs r0, basepri									\n"/* r0 = basepri. Return original basepri value. */
	    "	mov r1, #0xB0									\n"/* r1 = ( 0x01 << (8 - 3) ). 3 bits for interrupt priority. */
	    "	msr basepri, r1									\n"/* Disable interrupts. */
	    "	bx lr											\n"/* Return. */
	);
}

void arch_int_restore( uint32_t int_flag ) __attribute__( ( naked ) );

void arch_int_restore(uint32_t int_flag) /* __attribute__(( naked )) PRIVILEGED_FUNCTION */
{
	__asm volatile
	(
		"	msr basepri, r0									\n"/* basepri = int_flag. */
		"	bx lr											\n"/* Return. */
	);
}
#endif

