#ifndef _KEY_HANDLE_H_
#define _KEY_HANDLE_H_

#include "sys_rtos.h"
#include <os/os.h>

#define KEY_DEBUG

#ifdef KEY_DEBUG
#define KEY_PRT       os_printf
#endif
#ifdef CONFIG_COMPONENTS_TX_BUFING
#define APP_PRT       os_null_printf
#endif


#endif
