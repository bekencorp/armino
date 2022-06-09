/**
  ******************************************************************************
  * @file    low_level_security.h
  * @author  MCD Application Team
  * @brief   Header for low_level_security.c module
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
#ifndef LOW_LEVEL_SECURITY_H
#define LOW_LEVEL_SECURITY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "boot_hal_cfg.h"
#include "bootutil/bootutil_log.h"

/** @addtogroup Secure Secure Boot / Firmware Update
  * @{
  */

/** @addtogroup LOW_LEVEL
  * @{
  */
/** @defgroup LOW_LEVEL_SECURITY Security Low Level define
  * @{
  */
#define SRAM2_PAGE_SIZE (0x400)
/**
  * @}
  */
/** @defgroup LOW_LEVEL_SECURITY Security Low Level Interface
  * @{
  */
/** @defgroup SECURITY_Configuration Security Configuration
  * @{
  */

/**
  * @}
  */

/** @defgroup SECURITY_Exported_Constants Exported Constants
  * @{
  */

/**
  * @}
  */

/** @defgroup SECURITY_Exported_Functions Exported Functions
  * @{
  */
void    LL_SECU_ApplyRunTimeProtections(void);
void    LL_SECU_UpdateRunTimeProtections(void);
void    LL_SECU_CheckStaticProtections(void);
void    LL_SECU_UpdateLoaderRunTimeProtections(void);
void    LL_SECU_SetLoaderCodeSecure(void);
#if (TFM_TAMPER_ENABLE != NO_TAMPER)
void    TAMP_IRQHandler(void);
#endif /*  TFM_TAMPER_ENABLE != NO_TAMPER */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


#ifdef __cplusplus
}
#endif

#endif /* LOW_LEVEL_SECURITY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
