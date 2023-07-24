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

#include <common/bk_include.h>
#include <driver/pwr_clk.h>
#include <driver/mailbox_channel.h>
#include <modules/pm.h>
#include "sys_driver.h"

#define MB_CHNL_PWC                (0x4)

/*=====================DEFINE  SECTION  START=====================*/

/*=====================DEFINE  SECTION  END=====================*/

/*=====================VARIABLE  SECTION  START=================*/
#if CONFIG_SLAVE_CORE
static volatile  pm_mailbox_communication_state_e s_pm_cp1_pwr_finish        = 0;
static volatile  pm_mailbox_communication_state_e s_pm_cp1_clk_finish        = 0;
static volatile  pm_mailbox_communication_state_e s_pm_cp1_sleep_finish      = 0;
static volatile  pm_mailbox_communication_state_e s_pm_cp1_cpu_freq_finish   = 0;
static volatile  pm_mailbox_communication_state_e s_pm_cp1_init              = 0;
#else
static volatile  pm_mailbox_communication_state_e s_pm_cp1_boot_ready        = 0;
#endif
/*=====================VARIABLE  SECTION  END=================*/

/*================FUNCTION DECLARATION  SECTION  START========*/

#if CONFIG_SLAVE_CORE
static void pm_cp1_mailbox_init();
bk_err_t pm_cp1_mailbox_response(uint32_t cmd, int ret);
bk_err_t bk_pm_cp1_ctrl_state_set(pm_mailbox_communication_state_e state);
pm_mailbox_communication_state_e bk_pm_cp1_ctrl_state_get();
#endif

#if !CONFIG_SLAVE_CORE && CONFIG_DUAL_CORE
#if CONFIG_MAILBOX
static void pm_cp0_mailbox_init();
#endif
#endif
/*================FUNCTION DECLARATION  SECTION  END========*/

pm_lpo_src_e bk_clk_32k_customer_config_get(void)
{
#ifdef CONFIG_EXTERN_32K
	return PM_LPO_SRC_X32K;
#else
	return PM_LPO_SRC_ROSC;
#endif
}
bk_err_t bk_pm_mailbox_init()
{
#if CONFIG_SLAVE_CORE
	/*cp1 mailbox init*/
	#if CONFIG_MAILBOX
	pm_cp1_mailbox_init();
	#endif
    if(bk_pm_cp1_ctrl_state_get() == 0x0)
	{
		bk_pm_cp1_ctrl_state_set(PM_MAILBOX_COMMUNICATION_FINISH);
		pm_cp1_mailbox_response(PM_CPU1_BOOT_READY_CMD, 0x1);
	}
#else
	/*cp0 mailbox init*/
	#if CONFIG_MAILBOX && CONFIG_DUAL_CORE
	pm_cp0_mailbox_init();
	#endif //CONFIG_MAILBOX

#endif
	return BK_OK;
}

#if CONFIG_SLAVE_CORE
#if CONFIG_MAILBOX
pm_mailbox_communication_state_e bk_pm_cp1_pwr_ctrl_state_get()
{
	return s_pm_cp1_pwr_finish;
}
bk_err_t bk_pm_cp1_pwr_ctrl_state_set(pm_mailbox_communication_state_e state)
{
	s_pm_cp1_pwr_finish = state;
	return BK_OK;
}

pm_mailbox_communication_state_e bk_pm_cp1_clk_ctrl_state_get()
{
	return s_pm_cp1_clk_finish;
}
 bk_err_t bk_pm_cp1_clk_ctrl_state_set(pm_mailbox_communication_state_e state)
{
	s_pm_cp1_clk_finish = state;
	return BK_OK;
}

pm_mailbox_communication_state_e bk_pm_cp1_sleep_ctrl_state_get()
{
	return s_pm_cp1_sleep_finish;
}
bk_err_t bk_pm_cp1_sleep_ctrl_state_set(pm_mailbox_communication_state_e state)
{
	s_pm_cp1_sleep_finish = state;
	return BK_OK;
}

pm_mailbox_communication_state_e bk_pm_cp1_cpu_freq_ctrl_state_get()
{
	return s_pm_cp1_cpu_freq_finish;
}
bk_err_t bk_pm_cp1_cpu_freq_ctrl_state_set(pm_mailbox_communication_state_e state)
{
	s_pm_cp1_cpu_freq_finish = state;
	return BK_OK;
}

pm_mailbox_communication_state_e bk_pm_cp1_ctrl_state_get()
{
	return s_pm_cp1_init;
}
bk_err_t bk_pm_cp1_ctrl_state_set(pm_mailbox_communication_state_e state)
{
	s_pm_cp1_init = state;
	return BK_OK;
}

bk_err_t pm_cp1_mailbox_response(uint32_t cmd, int ret)
{
	mb_chnl_cmd_t mb_cmd = {0};
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	mb_cmd.hdr.cmd = cmd;
	mb_cmd.param1 = ret;
	mb_cmd.param2 = 0;
	mb_cmd.param3 = 0;
	mb_chnl_write(MB_CHNL_PWC, &mb_cmd);
	GLOBAL_INT_RESTORE();
	return BK_OK;
}
static void pm_cp1_mailbox_tx_cmpl_isr(int *pm_mb, mb_chnl_ack_t *cmd_buf)
{
}
static void pm_cp1_mailbox_rx_isr(int *pm_mb, mb_chnl_cmd_t *cmd_buf)
{
	bk_err_t ret = BK_OK;

	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	switch(cmd_buf->hdr.cmd) {
		case PM_POWER_CTRL_CMD:
			if(cmd_buf->param1 == BK_OK)
			{
				s_pm_cp1_pwr_finish = PM_MAILBOX_COMMUNICATION_FINISH;
			}
			else
			{
				ret = BK_FAIL;
			}
			break;
		case PM_CLK_CTRL_CMD:
			if(cmd_buf->param1 == BK_OK)
			{
				s_pm_cp1_clk_finish = PM_MAILBOX_COMMUNICATION_FINISH;
			}
			else
			{
				ret = BK_FAIL;
			}
			break;
		 case PM_SLEEP_CTRL_CMD:
			if(cmd_buf->param1 == BK_OK)
			{
				s_pm_cp1_sleep_finish = PM_MAILBOX_COMMUNICATION_FINISH;
			}
			else
			{
				ret = BK_FAIL;
			}
			break;
		 case PM_CPU_FREQ_CTRL_CMD:
			if(cmd_buf->param1 == BK_OK)
			{
				s_pm_cp1_cpu_freq_finish = PM_MAILBOX_COMMUNICATION_FINISH;
			}
			else
			{
				ret = BK_FAIL;
			}
			break;
		default:
			break;
	}
	GLOBAL_INT_RESTORE();

	if(ret != BK_OK)
	{
		os_printf("cp1 response: cp0 handle msg error\r\n");
	}
	//if(pm_debug_mode()&0x2)
	{
		//os_printf("enter cp1_mailbox_rx_isr %d %d %d \r\n",cmd_buf->hdr.cmd,cmd_buf->param1,cmd_buf->param2);
	}
}
static void pm_cp1_mailbox_tx_isr(int *pm_mb)
{
}

static void pm_cp1_mailbox_init()
{
	mb_chnl_open(MB_CHNL_PWC, NULL);
	if (pm_cp1_mailbox_rx_isr != NULL)
		mb_chnl_ctrl(MB_CHNL_PWC, MB_CHNL_SET_RX_ISR, pm_cp1_mailbox_rx_isr);
	if (pm_cp1_mailbox_tx_isr != NULL)
		mb_chnl_ctrl(MB_CHNL_PWC, MB_CHNL_SET_TX_ISR, pm_cp1_mailbox_tx_isr);
	if (pm_cp1_mailbox_tx_cmpl_isr != NULL)
		mb_chnl_ctrl(MB_CHNL_PWC, MB_CHNL_SET_TX_CMPL_ISR, pm_cp1_mailbox_tx_cmpl_isr);
}
#endif //CONFIG_MAILBOX
#endif//CONFIG_SLAVE_CORE


#if !CONFIG_SLAVE_CORE && CONFIG_DUAL_CORE
#if CONFIG_MAILBOX
static void pm_cp0_mailbox_response(uint32_t cmd, int ret)
{
	mb_chnl_cmd_t mb_cmd = {0};
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	mb_cmd.hdr.cmd = cmd;
	mb_cmd.param1 = ret;
	mb_cmd.param2 = 0;
	mb_cmd.param3 = 0;
	mb_chnl_write(MB_CHNL_PWC, &mb_cmd);
	GLOBAL_INT_RESTORE();
}

static void pm_cp0_mailbox_tx_cmpl_isr(int *pm_mb, mb_chnl_ack_t *cmd_buf)
{
}

static void pm_cp0_mailbox_rx_isr(int *pm_mb, mb_chnl_cmd_t *cmd_buf)
{
	bk_err_t ret = BK_OK;

	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	switch(cmd_buf->hdr.cmd) {
		case PM_POWER_CTRL_CMD:
			ret = bk_pm_module_vote_power_ctrl(cmd_buf->param1,cmd_buf->param2);

			pm_cp0_mailbox_response(PM_POWER_CTRL_CMD,ret);

			break;
		case PM_CLK_CTRL_CMD:
			ret = bk_pm_clock_ctrl(cmd_buf->param1,cmd_buf->param2);
			pm_cp0_mailbox_response(PM_CLK_CTRL_CMD,ret);
			break;
		case PM_SLEEP_CTRL_CMD:
			ret = bk_pm_module_vote_sleep_ctrl(cmd_buf->param1,cmd_buf->param2,cmd_buf->param3);
			pm_cp0_mailbox_response(PM_SLEEP_CTRL_CMD,ret);
			break;
		case PM_CPU_FREQ_CTRL_CMD:
			ret = bk_pm_module_vote_cpu_freq(cmd_buf->param1,cmd_buf->param2);
			pm_cp0_mailbox_response(PM_CPU_FREQ_CTRL_CMD,ret);
			break;
		case PM_CPU1_BOOT_READY_CMD:
			if(cmd_buf->param1 == 0x1)
			{
				s_pm_cp1_boot_ready = PM_MAILBOX_COMMUNICATION_FINISH;
			}
			//if(pm_debug_mode()&0x2)
				//os_printf("cpu0 receive the cpu1 event [%d]\r\n",cmd_buf->param1);
			break;
		default:
			break;
	}
	GLOBAL_INT_RESTORE();
	if(ret != BK_OK)
	{
		os_printf("cp0 handle cp1 message error\r\n");
	}

	//if(pm_debug_mode()&0x2)
	{
		//os_printf("enter cp0_mailbox_rx_isr %d %d %d %d\r\n",cmd_buf->hdr.cmd,cmd_buf->param1,cmd_buf->param2,ret);
	}
}
static void pm_cp0_mailbox_tx_isr(int *pm_mb)
{
}
static void pm_cp0_mailbox_init()
{
	mb_chnl_open(MB_CHNL_PWC, NULL);
	if (pm_cp0_mailbox_rx_isr != NULL)
		mb_chnl_ctrl(MB_CHNL_PWC, MB_CHNL_SET_RX_ISR, pm_cp0_mailbox_rx_isr);
	if (pm_cp0_mailbox_tx_isr != NULL)
		mb_chnl_ctrl(MB_CHNL_PWC, MB_CHNL_SET_TX_ISR, pm_cp0_mailbox_tx_isr);
	if (pm_cp0_mailbox_tx_cmpl_isr != NULL)
		mb_chnl_ctrl(MB_CHNL_PWC, MB_CHNL_SET_TX_CMPL_ISR, pm_cp0_mailbox_tx_cmpl_isr);
}
#endif //CONFIG_MAILBOX
#endif //!CONFIG_SLAVE_CORE && CONFIG_DUAL_CORE


#if !CONFIG_SLAVE_CORE
static uint32_t s_pm_cp1_ctrl_state           = 0;
extern void start_slave_core(void);
extern void stop_slave_core(void);
static void pm_module_bootup_cpu(pm_power_module_name_e module)
{
	if(PM_POWER_MODULE_STATE_OFF == sys_drv_module_power_state_get(module))
	{
		if(module == PM_POWER_MODULE_NAME_CPU1)
		{
            bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_CPU1, PM_POWER_MODULE_STATE_ON);
            start_slave_core();
            while(!s_pm_cp1_boot_ready);
		}
	}
}
static void pm_module_shutdown_cpu(pm_power_module_name_e module)
{
	if(PM_POWER_MODULE_STATE_ON == sys_drv_module_power_state_get(module))
	{
		if(module == PM_POWER_MODULE_NAME_CPU1)
		{
            stop_slave_core();
		    bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_CPU1, PM_POWER_MODULE_STATE_OFF);
		}
	}
}

bk_err_t bk_pm_module_vote_boot_cp1_ctrl(pm_boot_cp1_module_name_e module,pm_power_module_state_e power_state)
{
	GLOBAL_INT_DECLARATION();

    if(power_state == PM_POWER_MODULE_STATE_ON)//power on
    {
            if((module == PM_BOOT_CP1_MODULE_NAME_FFT)
                ||(module == PM_BOOT_CP1_MODULE_NAME_AUDP_SBC)
                ||(module == PM_BOOT_CP1_MODULE_NAME_AUDP_AUDIO)
                ||(module == PM_BOOT_CP1_MODULE_NAME_AUDP_I2S))
            {
                GLOBAL_INT_DISABLE();
                s_pm_cp1_ctrl_state |= 0x1 << (module);
                GLOBAL_INT_RESTORE();
                pm_module_bootup_cpu(PM_POWER_MODULE_NAME_CPU1);
            }
    }
    else //power down
    {
        if((module == PM_BOOT_CP1_MODULE_NAME_FFT)
             ||(module == PM_BOOT_CP1_MODULE_NAME_AUDP_SBC)
             ||(module == PM_BOOT_CP1_MODULE_NAME_AUDP_AUDIO)
             ||(module == PM_BOOT_CP1_MODULE_NAME_AUDP_I2S))

		{
			GLOBAL_INT_DISABLE();
			s_pm_cp1_ctrl_state &= ~(0x1 << (module));
			if(0x0 == s_pm_cp1_ctrl_state)
			{
				pm_module_shutdown_cpu(PM_POWER_MODULE_NAME_CPU1);
			}
			GLOBAL_INT_RESTORE();
         }
    }

    return BK_OK;
}
bk_err_t bk_pm_module_vote_boot_cp2_ctrl(pm_power_module_name_e module,pm_power_module_state_e power_state)
{
    return BK_OK;
}


#endif


