/**
  ******************************************************************************
  * @file    low_level_rng.c
  * @author  MCD Application Team
  * @brief   Low Level Interface module to use STM32 RNG Ip
  *          This file provides mbed-crypto random generataor
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
#include "low_level_rng.h"
#include "stm32hal.h"

static RNG_HandleTypeDef handle;
static uint8_t users = 0;

#define COMPILER_BARRIER() __ASM __IO("" : : : "memory")

static uint8_t atomic_incr_u8(__IO uint8_t *valuePtr, uint8_t delta)
{
  COMPILER_BARRIER();
  uint8_t newValue;
  do
  {
    newValue = __LDREXB(valuePtr) + delta;
  } while (__STREXB(newValue, valuePtr));
  COMPILER_BARRIER();
  return newValue;
}

int RNG_Init(void)
{
  uint32_t dummy;
  /*  We're only supporting a single user of RNG */
  if (atomic_incr_u8(&users, 1) > 1)
  {
    return -1;
  }
#if defined(STM32L562xx) || defined(STM32L552xx)
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

  /*Select PLLQ output as RNG clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RNG;
  PeriphClkInitStruct.RngClockSelection = RCC_RNGCLKSOURCE_MSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    users=0;
    return -1;
  }
#else
  /* Select RNG clock source */
  __HAL_RCC_RNG_CONFIG(RCC_RNGCLKSOURCE_HSI48);
#endif
  /* RNG Peripheral clock enable */
  __HAL_RCC_RNG_CLK_ENABLE();

  /* Initialize RNG instance */
  handle.Instance = RNG;
  handle.State = HAL_RNG_STATE_RESET;
  handle.Lock = HAL_UNLOCKED;

  if (HAL_RNG_Init(&handle) != HAL_OK)
  {
    return -2;
  }

  /* first random number generated after setting the RNGEN bit should not be used */
  HAL_RNG_GenerateRandomNumber(&handle, &dummy);
  return 0;
}

static void RNG_GetBytes(uint8_t *output, size_t length, size_t *output_length)
{
  int32_t ret = 0;
  uint8_t try = 0;
  __IO uint8_t random[4];
  *output_length = 0;

  /* Get Random byte */
  while ((*output_length < length) && (ret == 0))
  {
    if (HAL_RNG_GenerateRandomNumber(&handle, (uint32_t *)random) != HAL_OK)
    {
      /* retry when random number generated are not immediately available */
      if (try < 3)
      {
        try++;
      }
      else
      {
        ret = -1;
      }
    }
    else
    {
      for (uint8_t i = 0; (i < 4) && (*output_length < length) ; i++)
      {
        *output++ = random[i];
        *output_length += 1;
        random[i] = 0;
      }
    }
  }
  /* Just be extra sure that we didn't do it wrong */
  if ((__HAL_RNG_GET_FLAG(&handle, (RNG_FLAG_CECS | RNG_FLAG_SECS))) != 0)
  {
    *output_length = 0;
  }
}

void RNG_DeInit(void)
{
  /*Disable the RNG peripheral */
  HAL_RNG_DeInit(&handle);
  /* RNG Peripheral clock disable - assume we're the only users of RNG  */
  __HAL_RCC_RNG_CLK_DISABLE();

  users = 0;
}


/*  interface for mbed-crypto */
int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen)
{
  RNG_GetBytes(output, len, olen);
  if (*olen != len)
  {
    return -1;
  }
  return 0;
}
