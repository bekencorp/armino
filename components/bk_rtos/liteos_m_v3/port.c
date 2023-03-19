#include <common/bk_include.h>
#include "portmacro.h"
#include "bk_los_timer.h"
#include "bk_timer.h"
#include "los_reg.h"
#include "los_debug.h"
#include "los_timer.h"
#include "riscv_hal.h"
#include "soc_common.h"
#include "los_config.h"
#include "los_context.h"
#include <os/os.h>
#include "bk_arch.h"
#include "los_interrupt.h"
#include "los_tick.h"
#include <driver/timer.h>
#include <components/system.h>
#include "los_arch_interrupt.h"
#include "los_arch_timer.h"
#include "los_sched.h"


#define ISR_STACK_SIZE 1024
#define BYTE_ALIGNMENT_MASK    ( 0x000f )

static __attribute__ ((aligned(16))) StackType_t xISRStack[ ISR_STACK_SIZE ] = { 0 };
const StackType_t xISRStackTop = ( StackType_t ) &( xISRStack[ ISR_STACK_SIZE & ~BYTE_ALIGNMENT_MASK ] );


volatile uint64_t *mtime = ( volatile uint64_t * const )( MTIMER );
volatile uint64_t *mtimecmp = ( volatile uint64_t * const )( MTIMERCMP );

/* Used to program the machine timer compare register. */
uint64_t ullNextTime = 0ULL;
const uint64_t *pullNextTime = &ullNextTime;
const size_t uxTimerIncrementsForOneTick = ( size_t ) ( OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND ); /* Assumes increment won't go over 32-bits. */
uint32_t const ullMachineTimerCompareRegisterBase = MTIMERCMP;
volatile uint64_t * pullMachineTimerCompareRegister = NULL;

extern void mext_interrupt(void);

/*-----------------------------------------------------------*/

void vPortSetupTimerInterrupt(void)
{
	uint32_t ulCurrentTimeHigh, ulCurrentTimeLow;
	volatile uint32_t * const pulTimeHigh = ( volatile uint32_t * const ) ( ( MTIMER ) + 4UL ); /* 8-byte typer so high 32-bit word is 4 bytes up. */
	volatile uint32_t * const pulTimeLow = ( volatile uint32_t * const ) ( MTIMER );
	volatile uint32_t ulHartId;

	__asm volatile( "csrr %0, mhartid" : "=r"( ulHartId ) );
	pullMachineTimerCompareRegister  = ( volatile uint64_t * ) ( MTIMERCMP + ( ulHartId * sizeof( uint64_t ) ) );

	do
	{
		ulCurrentTimeHigh = *pulTimeHigh;
		ulCurrentTimeLow = *pulTimeLow;
	} while( ulCurrentTimeHigh != *pulTimeHigh );

	ullNextTime = ( uint64_t ) ulCurrentTimeHigh;
	ullNextTime <<= 32ULL; /* High 4-byte word is 32-bits up. */
	ullNextTime |= ( uint64_t ) ulCurrentTimeLow;
	ullNextTime += ( uint64_t ) uxTimerIncrementsForOneTick;
	*pullMachineTimerCompareRegister = ullNextTime;

	/* Prepare the time to use after the next tick interrupt. */
	ullNextTime += ( uint64_t ) uxTimerIncrementsForOneTick;

	HalIrqEnable(RISCV_MACH_TIMER_IRQ);

}

void vPortSetupMEXTInterrupt(void)
{
	UINT32 ret = -1;
	UINT32 int_save;

	int_save = LOS_IntLock();

	ret = HalHwiCreate(RISCV_MACH_EXT_IRQ, OS_HWI_PRIO_LOWEST, 0, (HWI_PROC_FUNC)mext_interrupt, 0);
	LOS_IntRestore(int_save);
	if (LOS_OK != ret) {
		PRINT_ERR("Setup MEXT Interrupt failed!\n");
		return;
	}

	HalIrqEnable(RISCV_MACH_EXT_IRQ);
}

/*-----------------------------------------------------------*/

uint32_t  platform_is_in_interrupt_context( void )
{
	return ((uint32_t)HalIsIntActive());
}
/*
 * disable mie Interrupts
 */
unsigned int port_disable_mie_flag(void)
{
	uint32_t uxSavedStatusValue;
	uxSavedStatusValue = read_csr(NDS_MIE);
	clear_csr(NDS_MIE, uxSavedStatusValue);

	return uxSavedStatusValue;
}

/*
 * Enable mie Interrupts
 */
void port_enable_mie_flag(uint32_t val)
{
	set_csr(NDS_MIE, val);
}

