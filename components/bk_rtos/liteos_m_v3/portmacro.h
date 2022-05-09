
#ifndef __PORTMACRO_H__
#define __PORTMACRO_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <os/os.h>

/* Type definitions. */
#if __riscv_xlen == 64
    #define portSTACK_TYPE          uint64_t
    #define portBASE_TYPE           int64_t
    #define portUBASE_TYPE          uint64_t
    #define portMAX_DELAY           ( TickType_t ) 0xffffffffffffffffUL
    #define portPOINTER_SIZE_TYPE   uint64_t
#elif __riscv_xlen == 32
    #define portSTACK_TYPE  uint32_t
    #define portBASE_TYPE   int32_t
    #define portUBASE_TYPE  uint32_t
    #define portMAX_DELAY ( TickType_t ) 0xffffffffUL
#else
    #error Assembler did not define __riscv_xlen
#endif

typedef portSTACK_TYPE StackType_t;
typedef portBASE_TYPE BaseType_t;
typedef portUBASE_TYPE UBaseType_t;
typedef portUBASE_TYPE TickType_t;


/* Macros to access high and low part of 64-bit mtime registers in RV32 */
#if __riscv_xlen == 32
    #define portREG64_HI(reg_addr) ( ( (volatile uint32_t *)reg_addr )[1] )
    #define portREG64_LO(reg_addr) ( ( (volatile uint32_t *)reg_addr )[0] )
#endif

extern volatile uint64_t *mtime;
extern volatile uint64_t *mtimecmp;

__attribute__((always_inline))
static inline UBaseType_t prvMieSave()
{
UBaseType_t uxSavedStatusValue;

    __asm volatile( "csrrc %0, mstatus, 0x8":"=r"( uxSavedStatusValue ) );
    return uxSavedStatusValue;
}
/*-----------------------------------------------------------*/

__attribute__((always_inline))
static inline void prvMieRestore( UBaseType_t uxSavedStatusValue )
{
    __asm volatile( "csrw mstatus, %0"::"r"( uxSavedStatusValue ) );
}
/*-----------------------------------------------------------*/

/* prvReadMtime(): Read machine timer register.
Note: Always use this API to access mtime */
__attribute__((always_inline))
inline uint64_t prvReadMtime( void )
{
uint32_t ulTimeHigh, ulTimeLow;

    #if __riscv_xlen == 32
        do
        {
            ulTimeHigh = portREG64_HI( mtime );
            ulTimeLow = portREG64_LO( mtime );

            if( ulTimeHigh == portREG64_HI( mtime ) )
                return ( ((uint64_t)ulTimeHigh) << 32 ) | ulTimeLow;
        } while( 1 );
    #else
        return *mtime;
    #endif
}
/*-----------------------------------------------------------*/

/* prvWriteMtimeCmp(ullNewMtimeCmp): Write machine timer compare register.
Note: Use this API to access register if timer interrupt is enabled. */
__attribute__((always_inline))
static inline void prvWriteMtimeCmp( uint64_t ullNewMtimeCmp )
{
    #if __riscv_xlen == 32
        /* To avoid triggering spurious interrupts when writting to mtimecmp,
        keeping the high part to the maximum value in the writing progress.

        We assume mtime overflow doesn't occur, because 64-bit mtime overflow
        period is longer than 500 years for the CPUs whose clock rate is lower
        than 1GHz. */

        portREG64_HI( mtimecmp ) = UINT32_MAX;
        portREG64_LO( mtimecmp ) = ullNewMtimeCmp & 0xFFFFFFFF;
        portREG64_HI( mtimecmp ) = ( ullNewMtimeCmp >> 32 ) & 0xFFFFFFFF;
    #else
        *mtimecmp = ullNewMtimeCmp;
    #endif
}


void vPortSetupTimerInterrupt(void);
void vPortSetupMEXTInterrupt(void);

__inline static int port_disable_interrupts_flag(void)
{
	uint32_t uxSavedStatusValue;

	__asm volatile( "csrrc %0, mstatus, 0x8":"=r"( uxSavedStatusValue ) );
	return uxSavedStatusValue;
}

/*
 * Enable Interrupts
 */
__inline static void port_enable_interrupts_flag(int val)
{
	__asm volatile( "csrw mstatus, %0"::"r"( val ) );
}

__inline static unsigned int port_set_interrupt_mask_from_isr(void)
{
	unsigned int val;

	__asm volatile( "csrrc %0, mstatus, 0x8":"=r"( val ) );

	return val;
}

__inline static void port_clear_interrupt_mask_from_isr(int val)
{
	__asm volatile( "csrw mstatus, %0"::"r"( val ) );
}


#define portDISABLE_INTERRUPTS()	__asm volatile( "csrc mstatus, 8" )
#define portENABLE_INTERRUPTS()		__asm volatile( "csrs mstatus, 8" )
#define portENTER_CRITICAL()
#define portEXIT_CRITICAL()


#define portNOP() __asm volatile 	( " nop " )


extern uint32_t platform_is_in_interrupt_context( void );



#ifdef __cplusplus
}
#endif

#endif /* _PORTMACRO_H_ */
