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

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_err.h>

#define BK_ERR_DMA_HAL_INVALID_ADDR (BK_ERR_DMA_HAL_BASE - 1)

/**
 * @brief DMA defines
 * @addtogroup bk_api_dma_defs DMA API group
 * @{
 */

typedef uint8_t dma_unit_t;          /**< DMA uint id */
typedef uint8_t dma_chan_priority_t; /**< DMA channel priority */

/**
 * @}
 */

/**
 * @brief DMA enum defines
 * @defgroup bk_api_dma_enum DMA enums
 * @ingroup bk_api_dma
 * @{
 */

typedef enum {
	DMA_ID_0 = 0,  /**< DMA channel 0 */
	DMA_ID_1,      /**< DMA channel 1 */
	DMA_ID_2,      /**< DMA channel 2 */
	DMA_ID_3,      /**< DMA channel 3 */
	DMA_ID_4,      /**< DMA channel 4 */
	DMA_ID_5,      /**< DMA channel 5 */
	DMA_ID_MAX,    /**< DMA channel max */
} dma_id_t;

typedef enum {
    DMA_DEV_DTCM = 0,    /**< DMA device DTCM */
    DMA_DEV_LA,          /**< DMA device LA */
    DMA_DEV_HSSPI,       /**< DMA device HSSPI */
    DMA_DEV_AUDIO,       /**< DMA device AUDIO */
    DMA_DEV_SDIO,        /**< DMA device SDIO */
    DMA_DEV_UART1,       /**< DMA device UART1 */
    DMA_DEV_UART2,       /**< DMA device UART2 */
    DMA_DEV_UART3,       /**< DMA device UART3 */
    DMA_DEV_I2S,         /**< DMA device I2S */
    DMA_DEV_GSPI0,       /**< DMA device GSPI1 */
    DMA_DEV_GSPI1,       /**< DMA device GSPI2 */
    DMA_DEV_GSPI2,       /**< DMA device GSPI3 */
    DMA_DEV_JPEG,        /**< DMA device JPEG */
    DMA_DEV_PSRAM_VIDEO, /**< DMA device PSRAM VIDEO */
    DMA_DEV_PSRAM_AUDIO, /**< DMA device PSRAM AUDIO */
    DMA_DEV_USB,         /**< DMA device USB */
    DMA_DEV_LCD_CMD,     /**< DMA device LCD CMD */
    DMA_DEV_LCD_DATA,    /**< DMA device LCD DATA */
    DMA_DEV_MAX,
} dma_dev_t;

typedef enum {
    DMA_DATA_WIDTH_8BITS = 0, /**< DMA data width 8bit */
    DMA_DATA_WIDTH_16BITS,    /**< DMA data width 16bit */
    DMA_DATA_WIDTH_32BITS,    /**< DMA data width 32bit */
} dma_data_width_t;

typedef enum {
    DMA_WORK_MODE_SINGLE = 0, /**< DMA work mode single_mode */
    DMA_WORK_MODE_REPEAT,     /**< DMA work mode repeat_mode (forever repeat until software clear dma_en) */
} dma_work_mode_t;

typedef enum {
    DMA_PRIO_MODE_ROUND_ROBIN = 0, /**< DMA priority mode round-robin(all dma priority are the same) */
    DMA_PRIO_MODE_FIXED_PRIO,      /**< DMA priority mode fixed prio(depend on chan_prio) */
} dma_priority_mode_t;

typedef enum {
    DMA_ADDR_INC_DISABLE = 0, /**< DMA disable addrress increase */
    DMA_ADDR_INC_ENABLE,      /**< DMA enable addrress increase */
} dma_addr_inc_t;

typedef enum {
    DMA_ADDR_LOOP_DISABLE = 0, /**< DMA disable addrress loop */
    DMA_ADDR_LOOP_ENABLE,      /**< DMA enable addrress loop */
} dma_addr_loop_t;

/**
 * @}
 */

/**
 * @brief DMA struct defines
 * @defgroup bk_api_dma_structs structs in DMA
 * @ingroup bk_api_dma
 * @{
 */

typedef struct {
    dma_dev_t dev;                /**< DMA device */
    dma_data_width_t width;       /**< DMA data width */
    dma_addr_inc_t addr_inc_en;   /**< enable/disable DMA address increase */
    dma_addr_loop_t addr_loop_en; /**< enable/disable DMA address loop */
    uint32_t start_addr;          /**< DMA start address */
    uint32_t end_addr;            /**< DMA end address */
} dma_port_config_t;

typedef struct {
    dma_work_mode_t mode;          /**< DMA work mode */
    dma_chan_priority_t chan_prio; /**< DMA channel prioprity */
    dma_port_config_t src;         /**< DMA source configuration */
    dma_port_config_t dst;         /**< DMA dest configuration */
} dma_config_t;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

