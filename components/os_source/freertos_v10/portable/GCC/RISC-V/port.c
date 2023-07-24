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

#define portWORD_SIZE	(sizeof(uint32_t))
#include "freertos_risc_v_chip_specific_extensions.h"

#define configMTIME_CLOCK_HZ                          ( ( unsigned long ) 26000000 )

typedef struct {
	union {
		struct {
			uint32_t ra;		/* x1  */
			uint32_t t0;		/* x5  */
			uint32_t t1;		/* x6  */
			uint32_t t2;		/* x7  */
			uint32_t a0;		/* x10 */
			uint32_t a1;
			uint32_t a2;
			uint32_t a3;
			uint32_t a4;
			uint32_t a5;
			uint32_t a6;
			uint32_t a7;		/* x17 */
			uint32_t t3;		/* x28 */
			uint32_t t4;
			uint32_t t5;
			uint32_t t6;		/* x31 */
			uint32_t uepc;
			uint32_t ustatus;
			uint32_t s0;		/* x8  */
			uint32_t s1;		/* x9 */
			uint32_t s2;		/* x11 */
			uint32_t s3;
			uint32_t s4;
			uint32_t s5;
			uint32_t s6;
			uint32_t s7;
			uint32_t s8;
			uint32_t s9;
			uint32_t s10;
			uint32_t s11;		/* x27 */
		};
		uint32_t riscv_regs[30];
	};
} port_task_context_t;

typedef port_task_context_t SAVED_CONTEXT;

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

void vPortSetupTimerInterrupt( void )
{
	ullNextTime = riscv_get_mtimer();
	ullNextTime += ( uint64_t ) uxTimerIncrementsForOneTick;
	riscv_set_mtimercmp(ullNextTime);

	/* Prepare the time to use after the next tick interrupt. */
	ullNextTime += ( uint64_t ) uxTimerIncrementsForOneTick;

	mon_timer_int_clear();
}


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

void freertos_trap_handler(uint32_t mcause, port_task_context_t *context)
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
			context->uepc += 4;
			vTaskSwitchContext();
			return;
		}
	}

	trap_handler(mcause, context);
	while(1);
}

static void prvTaskExitError( void )
{
	/* A function that implements a task must not exit or attempt to return to
	its caller as there is nothing to return to.  If a task wants to exit it
	should instead call vTaskDelete( NULL ). */

	BK_LOGE("\r\nOS", "==> Task exits abnormally!\r\n\r\n");
	vTaskDelete( NULL );
}

StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack,
									TaskFunction_t pxCode,
									void *pvParameters )
{
    port_task_context_t  *context;
	uint32_t  * bottom;

    context = (port_task_context_t *)(((uint32_t)pxTopOfStack) - sizeof(port_task_context_t));
	memset(context, 0, sizeof(port_task_context_t));

	bottom = (uint32_t *)context - (portasmADDITIONAL_CONTEXT_SIZE);  // reserve the space for FPU regs. REGBYTES = sizeof(UINT32);
	memset(bottom, 0, (portasmADDITIONAL_CONTEXT_SIZE * sizeof(uint32_t)));

    context->ustatus = USTATUS_UPIE;   /* Set UPIE bit in ustatus value, otherwise interrupts would be disabled anyway. */
    context->uepc = (uint32_t)pxCode;
    context->a0 = (uint32_t)pvParameters;
    context->ra = (uint32_t)prvTaskExitError;

	return (StackType_t *)bottom;

}

