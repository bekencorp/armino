#include "pal_signal.h"
#include "pal_log.h"
#if defined( TEE_M )
#include "psa_service.h"

#define SIGNAL_MASK 0x40000
#endif
uint32_t pal_signal_init( void )
{
#if defined( TEE_M )
    return ( SIGNAL_MASK );
#else
    return ( 0 );
#endif
}

uint32_t pal_wait_signal( uint32_t signal_mask, uint32_t timeout )
{
#if defined( TEE_M )
    (void) timeout;
    return ( psa_wait( signal_mask, PSA_BLOCK ) );
#else
    return ( 0 );
#endif
}

void pal_clear_signal( uint32_t signal_mask )
{
#if defined( TEE_M )
    psa_eoi( signal_mask );
#endif
}
