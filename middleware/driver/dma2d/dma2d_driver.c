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


#include <stdlib.h>
#include <common/bk_include.h>
#include <os/mem.h>
#include "arch_interrupt.h"
#include "sys_driver.h"
#include "gpio_map.h"
#include "gpio_driver.h"
#include <driver/gpio.h>
#include "BK7256_RegList.h"
#include <driver/int.h>
#include "dma2d_hal.h"
#include <driver/hal/hal_dma2d_types.h>
#include "BK7256_RegList.h"
#include <driver/dma2d.h>

#define DMA2D_TIMEOUT_ABORT           (1000U)  /**<  1s  */
#define DMA2D_TIMEOUT_SUSPEND         (1000U)  /**<  1s  */

#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)

static dma2d_isr_t s_dma2d_isr[DMA2D_ISR_NUM] = {NULL};
static void dma2d_isr(void);
static void dma2d_isr_common(void);

#endif

/**
  * @brief  initializes the DMA2D peripheral registers
  * @param  dma2d_init_t pointr structure that contains
  *                 the configuration information for the DMA2D
  *usage example:
  *
  * @retval None
  */
bk_err_t bk_dma2d_driver_init(dma2d_config_t *dma2d)
{
	if(sys_drv_dma2d_set(1, 1) != 0) {
		os_printf("dma2d sys clk config error \r\n");
		return BK_FAIL;
	}

#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
	os_memset(&s_dma2d_isr, 0, sizeof(s_dma2d_isr));
	bk_int_isr_register(INT_SRC_DMA2D, dma2d_isr, NULL);
#endif

	dma2d_hal_init(dma2d);
	dma2d_hal_start_transfer(0);

	return BK_OK;
}

/**
  * @brief  Deinitializes the DMA2D peripheral registers to their default reset values.
  * @retval None
  */
bk_err_t bk_dma2d_driver_deinit(void)
{
	bk_int_isr_unregister(INT_SRC_DMA2D);
	dma2d_hal_deinit();
	return BK_OK;
}

/**
  * @brief  Configure the DMA2D Layer according to the specified
  * @param  dma2d Pointer to a dma2d_init_t structure that contains the configuration information for the DMA2D.
  * @param  LayerIdx DMA2D Layer index.
  *                   This parameter can be one of the following values:
  *                   DMA2D_BACKGROUND_LAYER(0) / DMA2D_FOREGROUND_LAYER(1)
  * @retval HAL status
  */
bk_err_t bk_dma2d_layer_config(dma2d_config_t *dma2d, uint32_t layer_idx)
{
	dma2d_hal_layer_config(dma2d, layer_idx);
	return BK_OK;
}


/**
  * @brief  Start the DMA2D Transfer
  * @param  dma2d     Pointer to a dma2d_init_t structure that contains  the configuration information for the DMA2D.
  * @param  pdata      Configure the source memory Buffer address if
  *                     the Memory-to-Memory or Memory-to-Memory with pixel format
  *                     conversion mode is selected, or configure
  *                     the color value if Register-to-Memory mode is selected.
  * @param  dst_addr The destination memory Buffer address.
  * @param  Width      The width of data to be transferred from source to destination (expressed in number of pixels per line).
  * @param  Height     The height of data to be transferred from source to destination (expressed in number of lines).
  * @retval bk_err_t status
  */
bk_err_t bk_dma2d_start_transfer(dma2d_config_t *dma2d, uint32_t pdata, uint32_t dst_addr, uint32_t width, uint32_t height)
{
	dma2d_hal_config(dma2d, pdata, dst_addr, width, height);
	dma2d_hal_start_transfer(1);
	return BK_OK;
}


/**
  * @brief  bk_dma2d_wait_transfer_done
  * @retval return 0: transfer done, return others not transfer done 
  */
bool bk_dma2d_is_transfer_busy(void)
{
	return dma2d_hal_is_transfer_done();
}

/**
  * @brief  Start the multi-source DMA2D Transfer.
  * @param  dma2d      Pointer to a dma2d_init_t structure that contains  the configuration information for the DMA2D.
  * @param  src_addr1 The source memory Buffer address for the foreground layer.
  * @param  src_addr2 The source memory Buffer address for the background layer.
  * @param  dst_addr  The destination memory Buffer address.
  * @param  Width       The width of data to be transferred from source to destination (expressed in number of pixels per line).
  * @param  Height      The height of data to be transferred from source to destination (expressed in number of lines).
  * @retval bk_err_t status
  */
bk_err_t bk_dma2d_start_blending(dma2d_config_t *dma2d, uint32_t fg_addr, uint32_t bg_addr, uint32_t dst_addr, uint32_t width,  uint32_t height)
{
	dma2d_hal_blending_start(dma2d, fg_addr, bg_addr, dst_addr, width, height);
	return BK_OK;
}

/**
  * @brief  bk_dma2d_int_config.
  * @param  int_type  select from DMA2D_INT_TYPE, include int type:
                     DMA2D_CFG_ERROR
                     DMA2D_CLUT_TRANS_COMPLETE
                     DMA2D_CLUT_TRANS_ERROR
                     DMA2D_WARTERMARK_INT
                     DMA2D_TRANS_COMPLETE
                     DMA2D_TRANS_ERROR
  * @param  enable int
  * @retval bk_err_t status
  */
bk_err_t bk_dma2d_int_enable(dma2d_int_type_t int_type, bool enable)
{
	dma2d_hal_int_config(int_type, enable);
	return BK_OK;
}

/**
  * @brief  bk_dma2d_int_status_get.
  * @retval return <value> is all int status, can used by <value> & DMA2D_INT_STATUS check which int triggered
                      typedef enum
                      {
                          DMA2D_TRANS_ERROR_STATUS = 0x1,
                          DMA2D_TRANS_COMPLETE_STATUS,
                          DMA2D_WARTERMARK_INT_STATUS,
                          DMA2D_CLUT_TRANS_ERROR_STATUS,
                          DMA2D_CLUT_TRANS_COMPLETE_STATUS,
                          DMA2D_CFG_ERROR_STATUS
                      }DMA2D_INT_STATUS;
  */
uint32_t bk_dma2d_int_status_get(void)
{
	return dma2d_hal_int_status_get();
}

/**
  * @brief  clear int status
  * @param  int_status select from DMA2D_INT_STATUS include:
                          DMA2D_TRANS_ERROR_STATUS
                          DMA2D_TRANS_COMPLETE_STATUS
                          DMA2D_WARTERMARK_INT_STATUS
                          DMA2D_CLUT_TRANS_ERROR_STATUS
                          DMA2D_CLUT_TRANS_COMPLETE_STATUS
                          DMA2D_CFG_ERROR_STATUS

  * @retval bk_err_t status
  */
bk_err_t bk_dma2d_int_status_clear(dma2d_int_status_t int_status)
{
	dma2d_hal_int_status_clear(int_status);
	return BK_OK;
}

/**
  * @brief bk_dma2d_suspend
  * @param suspend
  * @retval bk_err_t status
  */
bk_err_t bk_driver_dma2d_suspend(bool suspend)
{
	dma2d_hal_suspend(suspend);
	return BK_OK;
}


/**
  * @brief  register dma2d cpu int isr
  * @param  dma2d_isr the function you registr isr
  * @retval bk_err_t status
  */
bk_err_t bk_dma2d_isr_register(dma2d_isr_t dma2d_isr)
{
	bk_int_isr_register(INT_SRC_DMA2D, dma2d_isr, NULL);
	return BK_OK;
}

static uint32_t HAL_GetTick()
{
	return 0;
}

/**
  * @brief bk_dma2d_abort
  * @param abort
  * @retval bk_err_t status
  */

bk_err_t bk_driver_dma2d_abort(bool abort)
{
	uint32_t tickstart;
	
	/* Abort the DMA2D transfer */
	/* START bit is reset to make sure not to set it again, in the event the HW clears it
	   between the register read and the register write by the CPU (writing 0 has no
	   effect on START bitvalue) */
	dma2d_hal_abort(abort);

	/* Get tick */
	tickstart = HAL_GetTick();

	/* Check if the DMA2D is effectively disabled */
	while(!dma2d_hal_is_transfer_done()) {
		if ((HAL_GetTick() - tickstart ) > DMA2D_TIMEOUT_ABORT) {
			return BK_ERR_TIMEOUT;
		}
	}
	return BK_OK;
}



#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)

/* dma2d interrupt enter */
static void dma2d_isr(void)
{
	dma2d_isr_common();
}

/* dma2d check interrupt flag and excute correponding isr function when enter interrupt */
static void dma2d_isr_common(void)
{
	uint32_t int_status;
	int_status = bk_dma2d_int_status_get();
	if (int_status & DMA2D_CFG_ERROR_STATUS) {
		
		if (s_dma2d_isr[DMA2D_CFG_ERROR_ISR]) {
			s_dma2d_isr[DMA2D_CFG_ERROR_ISR]();
		}
	}
	//Transfer Error Interrupt management
	if (int_status & DMA2D_TRANS_ERROR_STATUS) {
		if (s_dma2d_isr[DMA2D_TRANS_ERROR_ISR]) {
			s_dma2d_isr[DMA2D_TRANS_ERROR_ISR]();
		}
	}
	if (int_status & DMA2D_TRANS_COMPLETE_STATUS) {
		if (s_dma2d_isr[DMA2D_TRANS_COMPLETE_ISR]) {
			s_dma2d_isr[DMA2D_TRANS_COMPLETE_ISR]();
		}
	}
	if (int_status & DMA2D_WARTERMARK_INT_STATUS) {
		if (s_dma2d_isr[DMA2D_WARTERMARK_INT_ISR]) {
			s_dma2d_isr[DMA2D_WARTERMARK_INT_ISR]();
		}
	}
	if (int_status & DMA2D_CLUT_TRANS_COMPLETE_STATUS) {
		if (s_dma2d_isr[DMA2D_CLUT_TRANS_COMPLETE_ISR]) {
			s_dma2d_isr[DMA2D_CLUT_TRANS_COMPLETE_ISR]();
		}
	}
	if (int_status & DMA2D_CLUT_TRANS_ERROR_STATUS) {
		if (s_dma2d_isr[DMA2D_CLUT_TRANS_ERROR_ISR]) {
			s_dma2d_isr[DMA2D_CLUT_TRANS_ERROR_ISR]();
		}
	}
}

/**
  * @brief  register dma2d int type isr ,user option function
            open the macro  #define USE_HAL_DMA2D_REGISTER_CALLBACKS 1
  * @param  isr_id based on int type, a int type can register a isr, select from:
                             typedef enum
                             {
                                  DMA2D_CFG_ERROR_ISR = 0,
                                  DMA2D_CLUT_TRANS_COMPLETE_ISR,
                                  DMA2D_CLUT_TRANS_ERROR_ISR,
                                  DMA2D_WARTERMARK_INT_ISR,
                                  DMA2D_TRANS_COMPLETE_ISR,
                                  DMA2D_TRANS_ERROR_ISR,
                             }DMA2D_ISR_ID;
  * @param  cb_isr the user register int callback function
  * @retval bk_err_t status
  */
bk_err_t bk_dma2d_register_int_callback_isr(DMA2D_ISR_ID isr_id, dma2d_isr_t cb_isr)
{
	if ((isr_id) >= DMA2D_ISR_NUM) 
		return BK_FAIL;

	uint32_t int_level = rtos_disable_int();

	s_dma2d_isr[isr_id] = cb_isr;

	rtos_enable_int(int_level);

	return BK_OK;
}

#endif /* USE_HAL_DMA2D_REGISTER_CALLBACKS */



