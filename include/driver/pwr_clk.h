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

#include <modules/pm.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PM_POWER_CTRL_CMD          (0x1)
#define PM_CLK_CTRL_CMD            (0x2)
#define PM_SLEEP_CTRL_CMD          (0x3)
#define PM_CPU_FREQ_CTRL_CMD       (0x4)
#define PM_CPU1_BOOT_READY_CMD     (0x5)

typedef enum
{
	PM_BOOT_CP1_MODULE_NAME_FFT          = 0,
	PM_BOOT_CP1_MODULE_NAME_AUDP_SBC        ,
	PM_BOOT_CP1_MODULE_NAME_AUDP_AUDIO 	    ,
	PM_BOOT_CP1_MODULE_NAME_AUDP_I2S        ,
	PM_BOOT_CP1_MODULE_NAME_VIDP_JPEG_EN    ,
	PM_BOOT_CP1_MODULE_NAME_VIDP_JPEG_DE    ,
	PM_BOOT_CP1_MODULE_NAME_VIDP_DMA2D      ,
	PM_BOOT_CP1_MODULE_NAME_VIDP_LCD        ,
	PM_BOOT_CP1_MODULE_NAME_APP1            ,
	PM_BOOT_CP1_MODULE_NAME_APP2            ,
	PM_BOOT_CP1_MODULE_NAME_APP3            ,
	PM_BOOT_CP1_MODULE_NAME_APP4            ,
	PM_BOOT_CP1_MODULE_NAME_APP5            ,// 12
	PM_BOOT_CP1_MODULE_NAME_MAX             ,// attention: MAX value can not exceed 31.
}pm_boot_cp1_module_name_e;


typedef enum
{
	PM_MAILBOX_COMMUNICATION_INIT      = 0,
	PM_MAILBOX_COMMUNICATION_FINISH    = 1,
}pm_mailbox_communication_state_e;


/**
 * @brief get cpu1 power ctrl state
 *
 * get cpu1 power ctrl state
 *
 * @attention
 * - This API is used to get cpu1 power ctrl state
 *
 * @param
 * -void
 * @return
 * - cpu1 power ctrl state(PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish)
 */
pm_mailbox_communication_state_e bk_pm_cp1_pwr_ctrl_state_get();
/**
 * @brief set power control state in cpu1
 *
 * set power control state in cpu1
 *
 * @attention
 * - This API is used to set power control state in cpu1
 *
 * @param
 * -PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_cp1_pwr_ctrl_state_set(pm_mailbox_communication_state_e state);

/**
 * @brief get cpu1 clock ctrl state
 *
 * get cpu1 clock ctrl state
 *
 * @attention
 * - This API is used to get cpu1 clock ctrl state
 *
 * @param
 * -void
 * @return
 * - cpu1 power ctrl state(PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish)
 */
pm_mailbox_communication_state_e bk_pm_cp1_clk_ctrl_state_get();

/**
 * @brief set clock control state in cpu1
 *
 * set clock control state in cpu1
 *
 * @attention
 * - This API is used to set clock control state in cpu1
 *
 * @param
 * -PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_cp1_clk_ctrl_state_set(pm_mailbox_communication_state_e state);
/**
 * @brief get cpu1 vote sleep ctrl state
 *
 * get cpu1 vote sleep ctrl state
 *
 * @attention
 * - This API is used to get cpu1 vote sleep ctrl state
 *
 * @param
 * -void
 * @return
 * - cpu1 power ctrl state(PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish)
 */
pm_mailbox_communication_state_e bk_pm_cp1_sleep_ctrl_state_get();
/**
 * @brief set vote sleep state in cpu1
 *
 * set vote sleep state in cpu1
 *
 * @attention
 * - This API is used to set vote sleep state in cpu1
 *
 * @param
 * -PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_cp1_sleep_ctrl_state_set(pm_mailbox_communication_state_e state);
/**
 * @brief get cpu1 vote cpu freq ctrl state
 *
 * get cpu1 vote cpu freq  ctrl state
 *
 * @attention
 * - This API is used to get cpu1 vote cpu freq  ctrl state
 *
 * @param
 * -void
 * @return
 * - cpu1 power ctrl state(PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish)
 */
pm_mailbox_communication_state_e bk_pm_cp1_cpu_freq_ctrl_state_get();
/**
 * @brief set vote cpu frequency in cpu1
 *
 * set vote cpu frequency in cpu1
 *
 * @attention
 * - This API is used to set vote cpu frequency in cpu1
 *
 * @param
 * -PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_cp1_cpu_freq_ctrl_state_set(pm_mailbox_communication_state_e state);
/**
 * @brief get cpu1 boot ctrl state
 *
 * get cpu1 boot ctrl state
 *
 * @attention
 * - This API is used to get cpu1 boot ctrl state
 *
 * @param
 * -void
 * @return
 * - cpu1 power ctrl state(PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish)
 */
pm_mailbox_communication_state_e bk_pm_cp1_ctrl_state_get();
/**
 * @brief set bootup cpu1 state
 *
 * set bootup cpu1 state
 *
 * @attention
 * - This API is used to set bootup cpu1 state
 *
 * @param
 * -PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_cp1_ctrl_state_set(pm_mailbox_communication_state_e state);
/**
 * @brief cpu1 response info through mailbox
 *
 * cpu1 response info through mailbox
 *
 * @attention
 * - This API is used cpu1 response info through mailbox
 *
 * @param
 * -cmd: command ;ret:return value
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t pm_cp1_mailbox_response(uint32_t cmd, int ret);
/**
 * @brief pm vote boot cp1 ctrl
 *
 * pm vote boot cp1 ctrl
 *
 * @attention
 * - This API is used to used to pm vote boot cp1 ctrl
 *
 * @param
 * -module:boot cp1 module name;power_state:PM_POWER_MODULE_STATE_ON;PM_POWER_MODULE_STATE_OFF
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_module_vote_boot_cp1_ctrl(pm_boot_cp1_module_name_e module,pm_power_module_state_e power_state);

/**
 * @brief pm mailbox init
 *
 * pm mailbox init
 *
 * @attention
 * - This API is used to init pm mailbox init
 *
 * @param
 * -void
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_mailbox_init();

/**
 * @brief   get the 32k clock source configured by the customer
 *
 * This API get the 32k clock source configured by the customer
 *
 * @return
 *    - PM_LPO_SRC_X32K: support the external 32k clock
 *    - PM_LPO_SRC_ROSC: default support the ROSC
 */
pm_lpo_src_e bk_clk_32k_customer_config_get(void);


#ifdef __cplusplus
}
#endif


