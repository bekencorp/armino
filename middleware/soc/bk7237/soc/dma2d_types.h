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

#include <common/bk_err.h>

#ifdef __cplusplus
extern "C" {
#endif

#define USE_HAL_DMA2D_REGISTER_CALLBACKS 0

typedef void (*dma2d_isr_t)(void);


/*!<Mode_ARGB8888/RGB888 */
#define DMA2D_OCOLR_BLUE_1   (0x000000FFUL)                                       /*!< Blue Value */
#define DMA2D_OCOLR_GREEN_1   (0x0000FF00UL)                                       /*!< Green Value  */
#define DMA2D_OCOLR_RED_1   (0x00FF0000UL)                                       /*!< Red Value */
#define DMA2D_OCOLR_YELLOW_1   (0x00FFFF00UL)                                       /*!< Blue Value */
#define DMA2D_OCOLR_ALPHA_1    (0xFF000000UL)                                       /*!< Alpha Channel Value */

/*!<Mode_RGB565 */
#define DMA2D_OCOLR_BLUE_2       (0x0000001FUL)                                       /*!< Blue Value */
#define DMA2D_OCOLR_GREEN_2    (0x000007E0UL)                                       /*!< Green Value  */
#define DMA2D_OCOLR_RED_2       (0x0000F800UL)                                       /*!< Red Value */

/*!<Mode_ARGB1555 */
#define DMA2D_OCOLR_BLUE_3       (0x0000001FUL)                                       /*!< Blue Value */
#define DMA2D_OCOLR_GREEN_3    (0x000003E0UL)                                       /*!< Green Value  */
#define DMA2D_OCOLR_RED_3       (0x00007C00UL)                                       /*!< Red Value */
#define DMA2D_OCOLR_ALPHA_3    (0x00008000UL)                                       /*!< Alpha Channel Value */

/*!<Mode_ARGB4444 */
#define DMA2D_OCOLR_BLUE_4       (0x0000000FUL)                                       /*!< Blue Value */
#define DMA2D_OCOLR_GREEN_4    (0x000000F0UL)                                       /*!< Green Value  */
#define DMA2D_OCOLR_RED_4       (0x00000F00UL)                                       /*!< Red Value */
#define DMA2D_OCOLR_ALPHA_4    (0x0000F000UL)                                       /*!< Alpha Channel Value */

#define DMA2D_BACKGROUND_LAYER             0x00000000U   /*!< DMA2D Background Layer (layer 0) */
#define DMA2D_FOREGROUND_LAYER             0x00000001U   /*!< DMA2D Foreground Layer (layer 1) */
#define MAX_DMA2D_LAYER                    2U  /*!< DMA2D maximum number of layers */

/** @defgroup DMA2D_Alpha_Mode DMA2D Alpha Mode
  * @{
  */
#define DMA2D_NO_MODIF_ALPHA        0x00000000U  /*!< No modification of the alpha channel value */
#define DMA2D_REPLACE_ALPHA         0x00000001U  /*!< Replace original alpha channel value by programmed alpha value */
#define DMA2D_COMBINE_ALPHA         0x00000002U  /*!< Replace original foreground image alpha channel value by ALPHA[7:0] multiplied with original alpha channel value */

/** @defgroup DMA2D_Alpha_Inverted DMA2D Alpha Inversion
  * @{
  */
#define DMA2D_REGULAR_ALPHA         0x00000000U  /*!< No modification of the alpha channel value */
#define DMA2D_INVERTED_ALPHA        0x00000001U  /*!< Invert the alpha channel value */

/** @defgroup DMA2D_RB_Swap DMA2D Red and Blue Swap
  * @{
  */
#define DMA2D_RB_REGULAR            0x00000000U  /*!< Select regular mode (RGB or ARGB) */
#define DMA2D_RB_SWAP               0x00000001U  /*!< Select swap mode (BGR or ABGR) */
/**
  * @}
  */

#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)

typedef enum
{
	DMA2D_CFG_ERROR_ISR = 0,
	DMA2D_CLUT_TRANS_COMPLETE_ISR,
	DMA2D_CLUT_TRANS_ERROR_ISR,
	DMA2D_WARTERMARK_INT_ISR,
	DMA2D_TRANS_COMPLETE_ISR,
	DMA2D_TRANS_ERROR_ISR,

}DMA2D_ISR_ID;
#define DMA2D_ISR_NUM  6

#endif /* USE_HAL_DMA2D_REGISTER_CALLBACKS */



/**
 * @brief UART enum defines
 * @defgroup bk_api_uart_enum UART enums
 * @ingroup bk_api_uart
 * @{
 */

/**< defgroup DMA2D_Mode */
typedef enum {
	DMA2D_M2M = 0x00000000U,  /*!< DMA2D memory to memory transfer mode */
	DMA2D_M2M_PFC,            /*!< DMA2D memory to memory with pixel format conversion transfer mode */
	DMA2D_M2M_BLEND,          /*!< DMA2D memory to memory with blending transfer mode */
	DMA2D_R2M,               /*!< DMA2D register to memory transfer mode */
	DMA2D_M2M_BLEND_FG,      /*!< DMA2D memory to memory with blending transfer mode and fixed color FG */
	DMA2D_M2M_BLEND_BG        /*!< DMA2D memory to memory with blending transfer mode and fixed color BG */
} DMA2D_MODE;


/** @defgroup DMA2D_Output_Color_Mode DMA2D Output Color Mode */
typedef enum {
	DMA2D_OUTPUT_ARGB8888 = 0, /*!< ARGB8888 DMA2D color mode */
	DMA2D_OUTPUT_RGB888,       /*!< RGB888 DMA2D color mode   */
	DMA2D_OUTPUT_RGB565,       /*!< RGB565 DMA2D color mode   */
	DMA2D_OUTPUT_ARGB1555,     /*!< ARGB1555 DMA2D color mode */
	DMA2D_OUTPUT_ARGB4444,     /*!< ARGB4444 DMA2D color mode */
} OUTPUT_COLOR_MODE;


/**defgroup DMA2D_Input_Color_Mode DMA2D Input Color Mode */
typedef enum {
	DMA2D_INPUT_ARGB8888 = 0, /**< ARGB8888 DMA2D color mode */
	DMA2D_INPUT_RGB888,  /**< RGB888 DMA2D color mode   */
	DMA2D_INPUT_RGB565,   /**!< RGB565 DMA2D color mode   */
	DMA2D_INPUT_ARGB1555,   /**< ARGB1555 DMA2D color mode */
	DMA2D_INPUT_ARGB4444,   /**< ARGB4444 DMA2D color mode */
	DMA2D_INPUT_L8 = 5,
	DMA2D_INPUT_AL44,
	DMA2D_INPUT_AL88,
	DMA2D_INPUT_L4,
	DMA2D_INPUT_A8,
	DMA2D_INPUT_A4,
} INPUT_COLOR_MODE;

typedef enum
{
	DMA2D_CFG_ERROR              = (0x1UL << 13U),
	DMA2D_CLUT_TRANS_COMPLETE    = (0x1UL << 12U),
	DMA2D_CLUT_TRANS_ERROR       = (0x1UL << 11U),
	DMA2D_WARTERMARK_INT         = (0x1UL << 10U),
	DMA2D_TRANS_COMPLETE         = (0x1UL << 9U),
	DMA2D_TRANS_ERROR            = (0x1UL << 8U),
	DMA2D_ALL_INI = (0X3F << 8)

}DMA2D_INT_TYPE;


typedef enum
{
	DMA2D_TRANS_ERROR_STATUS = 0x1,
	DMA2D_TRANS_COMPLETE_STATUS,
	DMA2D_WARTERMARK_INT_STATUS,
	DMA2D_CLUT_TRANS_ERROR_STATUS,
	DMA2D_CLUT_TRANS_COMPLETE_STATUS,
	DMA2D_CFG_ERROR_STATUS
}DMA2D_INT_STATUS;

/**
  * @brief DMA2D CLUT Structure definition
  */
typedef struct
{
  uint32_t *pCLUT;                  /*!< Configures the DMA2D CLUT memory address.*/

  uint32_t CLUTColorMode;           /*!< Configures the DMA2D CLUT color mode.
                                         This parameter can be one value of @ref DMA2D_CLUT_CM. */

  uint32_t Size;                    /*!< Configures the DMA2D CLUT size.
                                         This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF.*/
} DMA2D_CLUTCfgTypeDef;

/**
  * @brief DMA2D Init structure definition
  */
typedef struct
{
	DMA2D_MODE             Mode;               /*!< Configures the DMA2D transfer mode.his parameter can be one value of @ref DMA2D_Mode. */
	uint32_t             ColorMode;          /*!< Configures the color format of the output image.
                                            	This parameter can be one value of @ref DMA2D_Output_Color_Mode. */
	uint32_t             OutputOffset;       /*!< Specifies the Offset value.
	This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0x3FFF. */
	uint32_t             AlphaInverted;     /*!< Select regular or inverted alpha value for the output pixel format converter.
	This parameter can be one value of @ref DMA2D_Alpha_Inverted. */
	uint32_t             LineOffsetMode;    /*!< Configures how is expressed the line offset for the foreground, background and output.
	                                           This parameter can be one value of @ref DMA2D_Line_Offset_Mode. */
	uint32_t              RedBlueSwap;      /*!< Select regular mode (RGB or ARGB) or swap mode (BGR or ABGR)
	                                            for the output pixel format converter.
	                                            This parameter can be one value of @ref DMA2D_RB_Swap. */
} DMA2D_INIT;

/** brief DMA2D Layer structure definition*/
typedef struct
{
	uint32_t             InputOffset;       /*!< Configures the DMA2D foreground or background offset. This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0x3FFF. */
	uint32_t             InputColorMode;    /*!< Configures the DMA2D foreground or background color mode. This parameter can be one value of @ref DMA2D_Input_Color_Mode. */
	uint32_t             AlphaMode;         /*!< Configures the DMA2D foreground or background alpha mode.This parameter can be one value of @ref DMA2D_Alpha_Mode. */
	uint32_t             InputAlpha;         /*the DMA2D foreground or background alpha value,This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF 
                                             - InputAlpha[24:31] is the alpha value ALPHA[0:7] */

	uint32_t             Inputcolor;        /*!< Specifies color value in case of A8 or A4 color mode.
	                                           @note In case of A8 or A4 color mode (ARGB), this parameter must be a number between
	                                           Min_Data = 0x00000000 and Max_Data = 0xFFFFFF where
	                                           - Inputcolor[16:23] is the red value RED[0:7]
	                                           - Inputcolor[8:15] is the green value GREEN[0:7]
	                                           - Inputcolor[0:7] is the blue value BLUE[0:7]. */
	uint32_t             AlphaInverted;     /*!< Select regular or inverted alpha value.This parameter can be one value of @ref DMA2D_Alpha_Inverted. */
	uint32_t     	RedBlueSwap;   /*!< Select regular mode (RGB or ARGB) or swap mode (BGR or ABGR).
                                            This parameter can be one value of @ref DMA2D_RB_Swap. */

} DMA2D_LAYER_CFG;


typedef struct __DMA2D_HandleTypeDef
{
	DMA2D_INIT         init;
	DMA2D_LAYER_CFG    LayerCfg[MAX_DMA2D_LAYER];
}DMA2D_HandleTypeDef;



/**
  * @brief  DMA2D handle Structure definition
  */

#ifdef __cplusplus
}
#endif


