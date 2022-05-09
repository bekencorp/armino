#ifndef _BK_LOS_TIMER_H_
#define _BK_LOS_TIMER_H_

#include <common/bk_include.h>
#include "bk_timer.h"
#include "tick_timer_id.h"

#define LOS_TICK_TIMER_ID                  TICK_TIMER_ID
#define LOS_TICK_CTRL_REG                  TIMER0_2_CTL
#define LOS_TICK_RD_CTRL_REG               TIMER0_2_READ_CTL
#define LOS_TICK_RD_VAL_REG                TIMER0_2_READ_VALUE
#define LOS_TICK_UNIT_CLOCK                32000

#define LOS_TICK_MS                        2
#define LOS_TICKS_PER_SECOND              (1000UL/LOS_TICK_MS)

extern UINT64 OsGetCurrSecond(VOID);

#endif // _BK_LOS_TIMER_H_
// eof
