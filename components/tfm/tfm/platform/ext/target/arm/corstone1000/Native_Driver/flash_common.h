/*
 *
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */


#ifndef __FLASH_COMMON_H__
#define __FLASH_COMMON_H__

#ifdef  __cplusplus
extern "C"
{
#endif

#include "Driver_Common.h"

int32_t Select_XIP_Mode_For_Shared_Flash(void);
int32_t Select_Write_Mode_For_Shared_Flash(void);

#ifdef  __cplusplus
}
#endif

#endif /* __FLASH_COMMON_H__ */
