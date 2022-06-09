/**
  ******************************************************************************
  * @file    low_level_ospi_flash.h
  * @author  MCD Application Team
  * @brief   This file contains device definition for low_level_ospi_flash driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
#ifndef __LOW_LEVEL_OSPI_FLASH_H
#define __LOW_LEVEL_OSPI_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32hal.h"

struct ospi_flash_range
{
  uint32_t base;
  uint32_t limit;
};
struct ospi_flash_vect
{
  uint32_t nb;
  struct ospi_flash_range *range;
};
struct low_level_ospi_device
{
  struct ospi_flash_vect erase;
  struct ospi_flash_vect write;
  uint32_t read_error;
};

extern struct low_level_ospi_device OSPI_FLASH0_DEV;

/* Additional api, not included in cmsis arm driver flash interface */
int32_t Ospi_Flash_Config_Exe(void);

#endif /* __LOW_LEVEL_OSPI_FLASH_H */


