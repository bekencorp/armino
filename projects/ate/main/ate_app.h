#ifndef _ATE_APP_H_
#define _ATE_APP_H_

#define CONFIG_ATE 1

#if CONFIG_ATE
#include "bk_gpio.h"

#include "bk_uart.h"
#define ATE_DEBUG
#ifdef ATE_DEBUG
#define ATE_PRT      os_printf
#define ATE_WARN     warning_prf
#define ATE_FATAL    fatal_prf
#else
#define ATE_PRT      null_prf
#define ATE_WARN     null_prf
#define ATE_FATAL    null_prf
#endif

extern int ate_gpio_port;
#define ATE_ENABLE_GIPO_LEVEL  0

void ate_gpio_init(void);
uint32_t ate_mode_check(void);

void ate_app_init(void);
uint32_t ate_is_enabled(void);
void ate_start(void);

#endif /*CONFIG_ATE */
#endif // _ATE_APP_H_
// eof

