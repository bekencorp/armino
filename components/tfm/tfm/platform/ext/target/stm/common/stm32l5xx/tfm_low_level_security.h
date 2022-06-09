/**
  ******************************************************************************
  * @file    tfm_low_level_security.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for Secure Firmware Update security
  *          low level interface.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TFM_LOW_LEVEL_SECURITY_H
#define TFM_LOW_LEVEL_SECURITY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "boot_hal_cfg.h"
#include "bootutil/bootutil_log.h"

/** @addtogroup TFM Secure Secure Boot / Firmware Update
  * @{
  */

/** @addtogroup TFM_LOW_LEVEL
  * @{
  */
/** @defgroup TFM_LOW_LEVEL_SECURITY Security Low Level define
  * @{
  */
#define SRAM2_PAGE_SIZE (0x400)
/**
  * @}
  */
/** @defgroup TFM_LOW_LEVEL_SECURITY Security Low Level Interface
  * @{
  */
/** @defgroup TFM_SECURITY_Configuration Security Configuration
  * @{
  */


/**
  * @}
  */

/** @defgroup TFM_SECURITY_Exported_Constants Exported Constants
  * @{
  */

/** @defgroup TFM_SECURITY_Exported_Constants_BOOL SFU Bool definition
  * @{
  */
typedef enum
{
  TFM_FALSE = 0U,
  TFM_TRUE = !TFM_FALSE
} TFM_BoolTypeDef;

/**
  * @}
  */

/** @defgroup TFM_SECURITY_Exported_Constants_State TFM Functional State definition
  * @{
  */
typedef enum
{
  TFM_DISABLE = 0U,
  TFM_ENABLE = !TFM_DISABLE
} TFM_FunctionalState;

/**
  * @}
  */

/** @defgroup TFM_SECURITY_Exported_Functions Exported Functions
  * @{
  */
void    TFM_LL_SECU_ApplyRunTimeProtections(void);
void    TFM_LL_SECU_UpdateRunTimeProtections(void);
void    TFM_LL_SECU_CheckStaticProtections(void);

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

#endif /* TFM_LOW_LEVEL_SECURITY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
