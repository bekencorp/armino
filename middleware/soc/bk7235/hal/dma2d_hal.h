// Copyright 2020-2021 Beken
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <soc/soc.h>
#include "dma2d_ll.h"
#include <driver/hal/hal_dma2d_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	dma2d_hw_t *hw;
} dma2d_hal_t;


bk_err_t dma2d_hal_init(DMA2D_HandleTypeDef *dma2d);

bk_err_t dma2d_hal_deinit(void);

/**
  * @brief  Set the DMA2D transfer parameters.
  * @param  dma2d     Pointer to a DMA2D_HandleTypeDef structure that contains
  *                     the configuration information for the specified DMA2D.
  * @param  src_addr      The source memory Buffer address
  * @param  DstAddress The destination memory Buffer address
  * @param  Width      The width of data to be transferred from source to destination.(number of pixel every line)
  * @param  Height     The height of data to be transferred from source to destination.(number of line)
  * @retval HAL status, uint32_t src_addr, uint32_t dst_addr, uint32_t width, uint32_t height
  */
bk_err_t dma2d_hal_config(DMA2D_HandleTypeDef *dma2d, uint32_t src_addr, uint32_t dst_addr, uint32_t width, uint32_t height);

bk_err_t dma2d_hal_start_transfer(bool start_transfer);

bool dma2d_hal_is_transfer_done(void);

bk_err_t dma2d_hal_suspend(bool suspend);

bk_err_t dma2d_hal_blending_start(DMA2D_HandleTypeDef *dma2d, uint32_t src_addr1, uint32_t src_addr2, uint32_t dst_addr, uint32_t Width,  uint32_t Height);

/**
  * @brief  Configure the DMA2D CLUT Transfer.
  * @param  hdma2d   Pointer to a DMA2D_HandleTypeDef structure that contains
  *                   the configuration information for the DMA2D.
  * @param  CLUTCfg  Pointer to a DMA2D_CLUTCfgTypeDef structure that contains
  *                   the configuration information for the color look up table.
  * @param  LayerIdx DMA2D Layer index.
  *                   This parameter can be one of the following values:
  *                   DMA2D_BACKGROUND_LAYER(0) / DMA2D_FOREGROUND_LAYER(1)
  * @note API obsolete and maintained for compatibility with legacy. User is invited
  *      to resort to HAL_DMA2D_CLUTStartLoad() instead to benefit from code compactness,
  *      code size and improved heap usage.
  * @retval HAL status
  */
bk_err_t dma2d_hal_clut_config(DMA2D_CLUTCfgTypeDef CLUTCfg, uint32_t LayerIdx);


/**
  * @brief  Configure the DMA2D Layer according to the specified
  *         parameters in the DMA2D_HandleTypeDef.
  * @param  hdma2d Pointer to a DMA2D_HandleTypeDef structure that contains
  *                 the configuration information for the DMA2D.
  * @param  LayerIdx DMA2D Layer index.
  *                   This parameter can be one of the following values:
  *                   DMA2D_BACKGROUND_LAYER(0) / DMA2D_FOREGROUND_LAYER(1)
  * @retval HAL status
  */
bk_err_t dma2d_hal_layer_config(DMA2D_HandleTypeDef *dma2d, uint32_t LayerIdx);

bk_err_t dma2d_hal_abort(bool abort);

bk_err_t dma2d_hal_int_config(DMA2D_INT_TYPE int_type, bool enable);

bk_err_t dma2d_hal_int_status_get(void);

bk_err_t dma2d_hal_int_status_clear(DMA2D_INT_STATUS int_status);

bk_err_t dma2d_hal_line_Watermar_cfg(uint32_t Line);


/**
  * @brief  Start the multi-source DMA2D Transfer.
  * @param  hdma2d      Pointer to a DMA2D_HandleTypeDef structure that contains
  *                      the configuration information for the DMA2D.
  * @param  SrcAddress1 The source memory Buffer address for the foreground layer.
  * @param  SrcAddress2 The source memory Buffer address for the background layer.
  * @param  DstAddress  The destination memory Buffer address.
  * @param  Width       The width of data to be transferred from source to destination (expressed in number of pixels per line).
  * @param  Height      The height of data to be transferred from source to destination (expressed in number of lines).
  * @retval HAL status
  */
bk_err_t dma2d_hal_blending_config(uint32_t SrcAddress1, uint32_t SrcAddress2, uint32_t DstAddress, uint32_t Width, uint32_t Height);

/**
  * @brief Configure dead time.
  * @note The dead time value represents the guaranteed minimum number of cycles between
  *       two consecutive transactions on the AHB bus.
  * @param DeadTime dead time value.
  * @param en.
  * @retval HAL status
  */
bk_err_t dma2d_hal_deadtime_config(uint8_t DeadTime, bool en);


#ifdef __cplusplus
}
#endif
