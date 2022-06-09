/**
  ******************************************************************************
  * @file    otp_provision.c
  * @author  MCD Application Team
  * @brief   File provisionning otp value
  *
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

#include "nv_counters.h"
#define INT2LE(A) (uint8_t)(A & 0xFF), (uint8_t )((A >> 8) & 0xFF),\
         (uint8_t )((A >> 16) & 0xFF), (uint8_t )((A >> 24) & 0xFF)


#if defined(__ICCARM__)
__root 
#endif
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".BL2_NVMCNT_Const"
#else
__attribute__((section(".BL2_NVMCNT_Const")))
#endif /* __ICCARM__ */
const struct nv_counters_t nvmcnt_init = {
    .checksum = NV_COUNTERS_INITIALIZED,
    .init_value = NV_COUNTERS_INITIALIZED,
    .counters = { 0, 0, 0, 0, 0, 0 }
    };
