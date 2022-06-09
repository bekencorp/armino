/**
  ******************************************************************************
  * @file    tick_device.h
  * @author  MCD Application Team
  * @brief   Tick device definition
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef TICK_DEVICE_H
#define TICK_DEVICE_H
#include "region.h"
REGION_DECLARE(Image$$, TFM_APP_RW_STACK_END, $$Base);
#define DEVICE_GET_TICK \
    do { __IO uint32_t TickVal=0; /* TickVal is placed on stack */\
      if ((uint32_t)&TickVal <= (uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_END, $$Base))\
      return 0 ;\
    }while(0)
#endif /* TICK_DEVICE_H */
