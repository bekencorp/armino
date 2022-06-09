/**
  ******************************************************************************
  * @file    boot_hal_cfg.h
  * @author  MCD Application Team
  * @brief   File fixing configuration flag specific for STM32U5xx platform
  *
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
#ifndef BOOT_HAL_CFG_H
#define BOOT_HAL_CFG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

/* RTC clock */
#define  RTC_CLOCK_SOURCE_LSI
#ifdef RTC_CLOCK_SOURCE_LSI
#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0x00F9
#endif
#ifdef RTC_CLOCK_SOURCE_LSE
#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0x00FF
#endif

/* ICache */
#define TFM_ICACHE_ENABLE /*!< Instruction cache enable */

/* Static protections */
#define TFM_WRP_PROTECT_ENABLE  /*!< Write Protection  */
#define TFM_HDP_PROTECT_ENABLE /*!< HDP protection   */
#define TFM_SECURE_USER_SRAM2_ERASE_AT_RESET /*!< SRAM2 clear at Reset  */


#define TFM_OB_RDP_LEVEL_VALUE OB_RDP_LEVEL_0 /*!< RDP level */


#define NO_TAMPER            (0)                /*!< No tamper activated */
#define INTERNAL_TAMPER_ONLY (1)                /*!< Only Internal tamper activated */
#define ALL_TAMPER           (2)                /*!< Internal and External tamper activated */
#define TFM_TAMPER_ENABLE NO_TAMPER            /*!< TAMPER configuration flag  */


#define TFM_OB_BOOT_LOCK 0 /*!< BOOT Lock expected value  */
#define TFM_ENABLE_SET_OB /*!< Option bytes are set by TFM_SBSFU_Boot when not correctly set  */
#define TFM_ERROR_HANDLER_NON_SECURE /*!< Error handler is in Non Secure , this allows regression without jumping   */

/* Run time protections */
#define TFM_FLASH_PRIVONLY_ENABLE  /*!< Flash Command in Privileged only  */
#define TFM_BOOT_MPU_PROTECTION    /*!< TFM_SBSFU_Boot uses MPU to prevent execution outside of TFM_SBSFU_Boot code  */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  TFM_SUCCESS = 0U,
  TFM_FAILED
} TFM_ErrorStatus;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);
#ifndef TFM_ERROR_HANDLER_NON_SECURE
void Error_Handler_rdp(void);
#else
#define Error_Handler_rdp Error_Handler
#endif
#endif /* GENERATOR_RDP_PASSWORD_AVAILABLE */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
