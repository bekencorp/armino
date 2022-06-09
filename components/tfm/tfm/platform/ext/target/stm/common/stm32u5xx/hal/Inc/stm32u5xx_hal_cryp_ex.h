/**
  ******************************************************************************
  * @file    stm32u5xx_hal_cryp_ex.h
  * @author  MCD Application Team
  * @brief   Header file of CRYPEx HAL module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32U5xx_HAL_CRYP_EX_H
#define STM32U5xx_HAL_CRYP_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal_def.h"

/** @addtogroup STM32U5xx_HAL_Driver
  * @{
  */

#if defined(AES)

/** @defgroup CRYPEx CRYPEx
  * @brief CRYP Extension HAL module driver.
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @defgroup CRYPEx_Exported_Functions CRYPEx Exported Functions
  * @{
  */

/** @addtogroup CRYPEx_Exported_Functions_Group1 Extended AES processing functions
  * @{
  */
HAL_StatusTypeDef HAL_CRYPEx_AESGCM_GenerateAuthTAG(CRYP_HandleTypeDef *hcryp, uint32_t *pAuthTag, uint32_t Timeout);
HAL_StatusTypeDef HAL_CRYPEx_AESCCM_GenerateAuthTAG(CRYP_HandleTypeDef *hcryp, uint32_t *pAuthTag, uint32_t Timeout);
/**
  * @}
  */

/** @addtogroup CRYPEx_Exported_Functions_Group2 Wrap and Unwrap key functions
  * @{
  */
HAL_StatusTypeDef HAL_CRYPEx_UnwrapKey(CRYP_HandleTypeDef *hcryp, uint32_t *pInput, uint32_t Timeout);
HAL_StatusTypeDef HAL_CRYPEx_WrapKey(CRYP_HandleTypeDef *hcryp, uint32_t *pInput, uint32_t *pOutput, uint32_t Timeout);
/**
  * @}
  */

/** @addtogroup CRYPEx_Exported_Functions_Group3 Encrypt/Decrypt Shared key functions
  * @{
  */
HAL_StatusTypeDef HAL_CRYPEx_EncryptSharedKey(CRYP_HandleTypeDef *hcryp, uint32_t *pKey, uint32_t *pOutput, uint32_t ID,
                                              uint32_t Timeout);
HAL_StatusTypeDef HAL_CRYPEx_DecryptSharedKey(CRYP_HandleTypeDef *hcryp, uint32_t *pKey, uint32_t ID, uint32_t Timeout);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /* AES */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32U5xx_HAL_CRYP_EX_H */
