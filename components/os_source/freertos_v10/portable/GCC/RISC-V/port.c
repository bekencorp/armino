/*
 * FreeRTOS Kernel V10.4.4
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the RISC-V RV32 port.
 *----------------------------------------------------------*/

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"
#include "arch_interrupt.h"
/* Standard includes. */
#include "string.h"
#include "platform.h"
#include "mon_call.h"

#ifdef configCLINT_BASE_ADDRESS
	#warning The configCLINT_BASE_ADDRESS constant has been deprecated.  configMTIME_BASE_ADDRESS and configMTIMECMP_BASE_ADDRESS are currently being derived from the (possibly 0) configCLINT_BASE_ADDRESS setting.  Please update to define configMTIME_BASE_ADDRESS and configMTIMECMP_BASE_ADDRESS dirctly in place of configCLINT_BASE_ADDRESS.  See https://www.FreeRTOS.org/Using-FreeRTOS-on-RISC-V.html
#endif

#ifndef configMTIME_BASE_ADDRESS
	#warning configMTIME_BASE_ADDRESS must be defined in FreeRTOSConfig.h.  If the target chip includes a memory-mapped mtime register then set configMTIME_BASE_ADDRESS to the mapped address.  Otherwise set configMTIME_BASE_ADDRESS to 0.  See https://www.FreeRTOS.org/Using-FreeRTOS-on-RISC-V.html
#endif

#ifndef configMTIMECMP_BASE_ADDRESS
	#warning configMTIMECMP_BASE_ADDRESS must be defined in FreeRTOSConfig.h.  If the target chip includes a memory-mapped mtimecmp register then set configMTIMECMP_BASE_ADDRESS to the mapped address.  Otherwise set configMTIMECMP_BASE_ADDRESS to 0.  See https://www.FreeRTOS.org/Using-FreeRTOS-on-RISC-V.html
#endif

/* Let the user override the pre-loading of the initial LR with the address of
prvTaskExitError() in case it messes up unwinding of the stack in the
debugger. */
#ifdef configTASK_RETURN_ADDRESS
	#define portTASK_RETURN_ADDRESS	configTASK_RETURN_ADDRESS
#else
	#define portTASK_RETURN_ADDRESS	prvTaskExitError
#endif

#define configMTIME_CLOCK_HZ                          ( ( unsigned long ) 26000000 )

typedef struct {
	union {
		struct {
			long x1;		/* ra */
			long x5;		/* t0 */
			long x6;		/* t1 */
			long x7;		/* t2 */
			long x10;		/* a0 */
			long x11;
			long x12;
			long x13;
			long x14;
			long x15;
			long x16;
			long x17;		/* a7 */
			long x28;		/* t3 */
			long x29;
			long x30;
			long x31;		/* t6 */
			long mepc;
			long mstatus;
			long x8;		/* s0 */
			long x9;		/* s1 */
			long x18;		/* s2 */
			long x19;
			long x20;
			long x21;
			long x22;
			long x23;
			long x24;
			long x25;
			long x26;
			long x27;		/* s11 */
		};
		long riscv_regs[30];
	};
} SAVED_CONTEXT;

/* The stack used by interrupt service routines.  Set configISR_STACK_SIZE_WORDS
to use a statically allocated array as the interrupt stack.  Alternative leave
configISR_STACK_SIZE_WORDS undefined and update the linker script so that a
linker variable names __freertos_irq_stack_top has the same value as the top
of the stack used by main.  Using the linker script method will repurpose the
stack that was used by main before the scheduler was started for use as the
interrupt stack after the scheduler has started. */

volatile unsigned int g_trap_nest_cnt = 0;

/*
 * Setup the timer to generate the tick interrupts.  The implementation in this
 * file is weak to allow application writers to change the timer used to
 * generate the tick interrupt.
 */
void vPortSetupTimerInterrupt( void );

/*-----------------------------------------------------------*/

/* Used to program the machine timer compare register. */
uint64_t ullNextTime = 0ULL;
const size_t uxTimerIncrementsForOneTick = ( size_t ) ( ( configMTIME_CLOCK_HZ ) / ( configTICK_RATE_HZ ) ); /* Assumes increment won't go over 32-bits. */
volatile BaseType_t xTickFlag = 0;

/* Set configCHECK_FOR_STACK_OVERFLOW to 3 to add ISR stack checking to task
stack checking.  A problem in the ISR stack will trigger an assert, not call the
stack overflow hook function (because the stack overflow hook is specific to a
task stack, not the ISR stack). */
#if defined( configISR_STACK_SIZE_WORDS ) && ( configCHECK_FOR_STACK_OVERFLOW > 2 ) 
    #define portCHECK_ISR_STACK()                                                            \
    {                                                                                                 \
    }
#else
	/* Define the function away. */
	#define portCHECK_ISR_STACK()
#endif /* configCHECK_FOR_STACK_OVERFLOW > 2 */


extern unsigned int arch_int_disable(void);
extern void arch_int_restore(int int_flag);
extern void riscv_set_mtimercmp(u64 new_time);
extern u64 riscv_get_mtimer(void);

extern void 	trap_handler(uint32_t mcause, SAVED_CONTEXT *context);

void port_check_isr_stack(void)
{
	portCHECK_ISR_STACK();
}

/*-----------------------------------------------------------*/

#if( configMTIME_BASE_ADDRESS != 0 ) && ( configMTIMECMP_BASE_ADDRESS != 0 )

void vPortSetupTimerInterrupt( void )
{
	ullNextTime = riscv_get_mtimer();
	ullNextTime += ( uint64_t ) uxTimerIncrementsForOneTick;
	riscv_set_mtimercmp(ullNextTime);

	/* Prepare the time to use after the next tick interrupt. */
	ullNextTime += ( uint64_t ) uxTimerIncrementsForOneTick;

	mon_timer_int_clear();
}

#endif /* ( configMTIME_BASE_ADDRESS != 0 ) && ( configMTIME_BASE_ADDRESS != 0 ) */

BaseType_t xPortStartScheduler( void )
{
	extern void xPortStartFirstTask( void );


	extern void arch_init_vector(uint32_t vect);
	extern void freertos_risc_v_trap_handler(void);
	arch_init_vector((uint32_t)freertos_risc_v_trap_handler);

	/* If there is a CLINT then it is ok to use the default implementation
	in this file, otherwise vPortSetupTimerInterrupt() must be implemented to
	configure whichever clock is to be used to generate the tick interrupt. */
	vPortSetupTimerInterrupt();

	xPortStartFirstTask();

	/* Should not get here as after calling xPortStartFirstTask() only tasks
	should be executing. */
	return pdFAIL;
}

void vPortEndScheduler( void )
{
	/* Not implemented. */
	for( ;; );
}
void vApplicationIdleHook( void )
{
#if (CONFIG_TASK_WDT)
	extern void bk_task_wdt_feed(void);
	bk_task_wdt_feed();
#endif
}

uint32_t platform_is_in_interrupt_context( void )
{
    return (arch_get_interrupt_nest_cnt() > 0 || arch_is_enter_exception() > 0 );
}

int port_disable_interrupts_flag(void)
{
	return arch_int_disable();
}

/*
 * Enable Interrupts
 */
void port_enable_interrupts_flag(int val)
{
	arch_int_restore(val);
}

void FreeRTOS_tickless_handler( void )
{
	/* Tick ISR has occured */
	xTickFlag = 1;

	/*
	 * Handle the corner case for system very busy on handling IRQs.
	 *
	 * If Timer ISR is pending after (ullWakeUpTime + 1) Ticks, Timer ISR
	 * will increase 2 Ticks in tickless idle mode so that it is possible to
	 * trigger assertion in vTaskStepTick() and it can cause Timer ISR stop
	 * until very long future if assertion is disabled.
	 *
	 * Fix assertion / timer ISR stop issue but losing tick value can't be
	 * avoided.
	 */
	uint64_t ullCurrentTime = riscv_get_mtimer();
	if( ullCurrentTime > ullNextTime ){
		ullNextTime = ullCurrentTime + uxTimerIncrementsForOneTick;
	}

}

void FreeRTOS_tick_handler( void )
{
	/* Handle tickless idle before Timer ISR updates mtimecmp. */
	#if( configUSE_TICKLESS_IDLE != 0 )
		FreeRTOS_tickless_handler();
	#endif /* configUSE_TICKLESS_IDLE */

	/* Update the mtimer compare match value. */
	riscv_set_mtimercmp( ullNextTime );

	/* Add ullNextTime to the timer increments for one tick. */
	ullNextTime += ( uint64_t ) uxTimerIncrementsForOneTick;

	if( xTaskIncrementTick() != pdFALSE )
	{
		vTaskSwitchContext();
	}
}

void freertos_trap_handler(uint32_t mcause, SAVED_CONTEXT *context)
{
	if(UCAUSE_INT & mcause)
	{
		switch(mcause & UCAUSE_CAUSE)
		{
			case IRQ_U_EXT:
				mext_interrupt();
				return;
				break;

			case IRQ_U_TIMER:
				FreeRTOS_tick_handler();
				mon_timer_int_clear();
				return;
				break;

			default:
				break;
		}
	}
	else
	{
		if((mcause == U_EXCP_ECALL) && (g_trap_nest_cnt == 1))
		{
			context->mepc += 4;
			vTaskSwitchContext();
			return;
		}
	}

	trap_handler(mcause, context);
	while(1);
}

