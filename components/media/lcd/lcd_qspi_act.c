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
#include <os/mem.h>
#include <os/str.h>
#include <driver/dma2d.h>
#include <driver/qspi.h>
#include <driver/qspi_types.h>
#include <driver/lcd_qspi_types.h>
#include <driver/media_types.h>
#include <driver/timer.h>
#include <driver/lcd_qspi.h>
#include <driver/psram.h>
#include "mailbox/mailbox_channel.h"
#include "media_evt.h"

#define TAG "lcd_qspi"
#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


#if CONFIG_SLAVE_CORE
static beken_thread_t lcd_qspi_act_thread_hdl = NULL;
lcd_qspi_device_t *cp1_device_config = NULL;
static uint8_t g_lcd_qspi_open_flag = 0;
static uint32_t *g_lcd_qspi_imag_addr;
static beken_mutex_t g_lcd_qspi_mutex;
static lcd_qspi_act_option_t g_lcd_qspi_act_step;

static void lcd_qspi_disp_main(void)
{
    while(1) {
        rtos_lock_mutex(&g_lcd_qspi_mutex);
        switch(g_lcd_qspi_act_step)
        {
            case LCD_QSPI_ACT_DISP_IDLE:
                rtos_delay_milliseconds(20);
                break;

            case LCD_QSPI_ACT_DISP_FRAME:
                if(g_lcd_qspi_imag_addr) {
                    bk_lcd_qspi_send_data(cp1_device_config, g_lcd_qspi_imag_addr, cp1_device_config->frame_len);
                }
                break;

            case LCD_QSPI_ACT_DISP_CLOSE:
                g_lcd_qspi_act_step = LCD_QSPI_ACT_DISP_IDLE;
                break;
        }
        rtos_unlock_mutex(&g_lcd_qspi_mutex);

        rtos_delay_milliseconds(2);
    }
}

static void _lcd_qspi_display_init(void)
{
    qspi_config_t lcd_qspi_config = {0};

    lcd_qspi_config.src_clk = 5;
    lcd_qspi_config.src_clk_div = 3;
    lcd_qspi_config.clk_div = 1;
    BK_LOG_ON_ERR(bk_qspi_init(&lcd_qspi_config));
    rtos_delay_milliseconds(2);

    bk_lcd_qspi_init(cp1_device_config);
}

static void _lcd_qspi_display_deinit(void)
{
    bk_lcd_qspi_deinit(cp1_device_config);

    BK_LOG_ON_ERR(bk_qspi_deinit());
    rtos_delay_milliseconds(2);
}

bk_err_t lcd_qspi_disp_open(void)
{
    bk_err_t ret = BK_FAIL;

    do{
        if(g_lcd_qspi_open_flag)
        {
            LOGI("[%s] have opened\r\n", __FUNCTION__);
            ret = BK_OK;
            break;
        }

        _lcd_qspi_display_init();
		g_lcd_qspi_act_step = LCD_QSPI_ACT_DISP_FRAME;
        g_lcd_qspi_open_flag = 1;
        LOGI("[%s] open success, frame:%x, frame_len:%d\r\n", __FUNCTION__, g_lcd_qspi_imag_addr, cp1_device_config->frame_len);
        ret = BK_OK;
    } while(0);

    return ret;
}

bk_err_t lcd_qspi_disp_close(void)
{
    _lcd_qspi_display_deinit();
    LOGI("[%s] close success\r\n", __FUNCTION__);

    rtos_lock_mutex(&g_lcd_qspi_mutex);
    g_lcd_qspi_act_step = LCD_QSPI_ACT_DISP_CLOSE;
    rtos_unlock_mutex(&g_lcd_qspi_mutex);

    return BK_OK;
}

bk_err_t lcd_qspi_display(uint32_t frame)
{
    if(g_lcd_qspi_open_flag)
    {
        rtos_lock_mutex(&g_lcd_qspi_mutex);
        g_lcd_qspi_imag_addr = (uint32_t *)frame;
        rtos_unlock_mutex(&g_lcd_qspi_mutex);
    }
    return BK_OK;
}

static void lcd_qspi_mailbox_rx_isr(void *param, mb_chnl_cmd_t *cmd_buf)
{
    media_msg_t msg;

    LOGD("%s, %08X\n", __func__, cmd_buf->hdr.cmd);

    switch(cmd_buf->hdr.cmd)
    {
        case EVENT_LCD_QSPI_OPEN_MBCMD:
            cp1_device_config = (lcd_qspi_device_t *)cmd_buf->param1;
			g_lcd_qspi_imag_addr = (uint32_t *)cmd_buf->param2;

            msg.event = EVENT_LCD_QSPI_OPEN_CMD;
            media_send_msg(&msg);
            break;

        case EVENT_LCD_QSPI_CLOSE_MBCMD:
            msg.event = EVENT_LCD_QSPI_CLOSE_CMD;
            media_send_msg(&msg);
            break;

        case EVENT_LCD_QSPI_DISP_MBCMD:
            msg.event = EVENT_LCD_QSPI_DISP_CMD;
            msg.param = cmd_buf->param1;
            media_send_msg(&msg);
            break;
    }

}


static void lcd_qspi_mailbox_tx_isr(void *param)
{
    LOGD("%s\n", __func__);
}

static void lcd_qspi_mailbox_tx_cmpl_isr(void *param, mb_chnl_ack_t *ack_buf)
{
    LOGD("%s\n", __func__);
}

void lcd_qspi_slave_init(void)
{
    bk_err_t ret;

    mb_chnl_open(MB_CHNL_LCD_QSPI, NULL);
    mb_chnl_ctrl(MB_CHNL_LCD_QSPI, MB_CHNL_SET_RX_ISR, lcd_qspi_mailbox_rx_isr);
    mb_chnl_ctrl(MB_CHNL_LCD_QSPI, MB_CHNL_SET_TX_ISR, lcd_qspi_mailbox_tx_isr);
    mb_chnl_ctrl(MB_CHNL_LCD_QSPI, MB_CHNL_SET_TX_CMPL_ISR, lcd_qspi_mailbox_tx_cmpl_isr);

    rtos_init_mutex(&g_lcd_qspi_mutex);
    ret = rtos_create_thread(&lcd_qspi_act_thread_hdl,
                             BEKEN_DEFAULT_WORKER_PRIORITY,
                             "qspi_disp",
                             (beken_thread_function_t)lcd_qspi_disp_main,
                             4096,
                             NULL);
    if (ret != kNoErr) {
        LOGE("lcd qspi display task create fail\r\n");
        return;
    } else {
        LOGE("lcd qspi display task create success\r\n");
    }
}

#endif


#ifdef CONFIG_MASTER_CORE
static uint8_t g_lcd_qspi_open_flag = 0;
static lcd_qspi_device_t *device_config = NULL;
static beken_semaphore_t lcd_qspi_disp_sem = NULL;
static beken_mutex_t g_lcd_qspi_mutex_master;

static void lcd_qspi_master_mailbox_rx_isr(void *param, mb_chnl_cmd_t *cmd_buf)
{
    bk_err_t ret;

    if (cmd_buf->hdr.cmd == EVENT_LCD_QSPI_OPEN_MBRSP
        || cmd_buf->hdr.cmd == EVENT_LCD_QSPI_CLOSE_MBRSP
        || cmd_buf->hdr.cmd == EVENT_LCD_QSPI_DISP_MBRSP)
    {
        ret = rtos_set_semaphore(&lcd_qspi_disp_sem);

        if (ret != BK_OK)
        {
            LOGE("%s semaphore get failed: %d\n", __func__, ret);
        }
    }
}

static void lcd_qspi_master_mailbox_tx_isr(void *param)
{
	LOGD("%s\n", __func__);

}

static void lcd_qspi_master_mailbox_tx_cmpl_isr(void *param, mb_chnl_ack_t *ack_buf)
{
	LOGD("%s\n", __func__);
}

bk_err_t bk_lcd_qspi_open(char *lcd_name, uint32_t *frame_buffer_addr, uint32_t frame_buffer_len)
{
    bk_err_t ret = BK_FAIL;
    mb_chnl_cmd_t mb_cmd;

    do{
        if(g_lcd_qspi_open_flag)
        {
            ret = BK_OK;
            break;
        }

        /* Mailbox */
        mb_chnl_open(MB_CHNL_LCD_QSPI, NULL);
        mb_chnl_ctrl(MB_CHNL_LCD_QSPI, MB_CHNL_SET_RX_ISR, lcd_qspi_master_mailbox_rx_isr);
        mb_chnl_ctrl(MB_CHNL_LCD_QSPI, MB_CHNL_SET_TX_ISR, lcd_qspi_master_mailbox_tx_isr);
        mb_chnl_ctrl(MB_CHNL_LCD_QSPI, MB_CHNL_SET_TX_CMPL_ISR, lcd_qspi_master_mailbox_tx_cmpl_isr);

        device_config = bk_lcd_qspi_get_device_by_name(lcd_name);
        if(!device_config)
        {
            LOGE("[%s] can't find this lcd:%s\r\n", __FUNCTION__, lcd_name);
            break;
        }
        LOGE("[%s] lcd:%s, device:%x\r\n", __FUNCTION__, lcd_name, device_config);

        rtos_init_semaphore_ex(&lcd_qspi_disp_sem, 1, 0);
        rtos_init_mutex(&g_lcd_qspi_mutex_master);
        bk_lcd_qspi_backlight_init(device_config, 100);
        g_lcd_qspi_open_flag = 1;
        ret = BK_OK;
    }while(0);

    LOGI("[%s][%d] open lcd success, RET:%d\r\n", __FUNCTION__, __LINE__, ret);
    if(BK_OK == ret)
    {
        mb_cmd.hdr.cmd = EVENT_LCD_QSPI_OPEN_MBCMD;
        mb_cmd.param1 = (uint32_t)device_config;
        mb_cmd.param2 = (uint32_t)frame_buffer_addr;
		mb_cmd.param3 = 0;

        ret = mb_chnl_write(MB_CHNL_LCD_QSPI, &mb_cmd);
        if (ret != BK_OK)
        {
            LOGE("[%s] mb_chnl_write failed: %d\n", __func__, ret);
            return ret;
        }

        ret = rtos_get_semaphore(&lcd_qspi_disp_sem, 5000);
        if (ret != BK_OK) {
            LOGE("[%s]semaphore get failed: %d, %d\n", __func__, ret, EVENT_LCD_QSPI_OPEN_MBCMD);
        } else {
            LOGI("[%s] open lcd success\r\n", __FUNCTION__);
        }
    }

    return ret;
}


bk_err_t bk_lcd_qspi_display(uint32_t *frame_buffer_addr, uint32_t frame_buffer_len)
{
    bk_err_t ret = BK_FAIL;
    mb_chnl_cmd_t mb_cmd;

    do{
        if(!g_lcd_qspi_open_flag)
        {
            LOGE("[%s] lcd don't open\r\n", __FUNCTION__);
            break;
        }

        rtos_lock_mutex(&g_lcd_qspi_mutex_master);
        mb_cmd.hdr.cmd = EVENT_LCD_QSPI_DISP_MBCMD;
        mb_cmd.param1 = (uint32_t)frame_buffer_addr;
        mb_cmd.param2 = 0;
        mb_cmd.param3 = 0;

        ret = mb_chnl_write(MB_CHNL_LCD_QSPI, &mb_cmd);
        if (ret != BK_OK)
        {
            LOGE("[%s] mb_chnl_write failed: %d\n", __func__, ret);
            rtos_unlock_mutex(&g_lcd_qspi_mutex_master);
            break;
        }

        ret = rtos_get_semaphore(&lcd_qspi_disp_sem, 5000);
        if (ret != BK_OK) {
            LOGE("[%s]semaphore get failed: %d\n", __func__, ret);
        }
		rtos_unlock_mutex(&g_lcd_qspi_mutex_master);
    }while(0);

    return ret;
}

bk_err_t bk_lcd_qspi_close(void)
{
    bk_err_t ret = BK_FAIL;
    mb_chnl_cmd_t mb_cmd;

    do{
        if(!g_lcd_qspi_open_flag)
        {
            LOGI("[%s] lcd don't open\r\n", __FUNCTION__);
            ret = BK_OK;
            break;
        }

        bk_lcd_qspi_backlight_deinit(device_config);
        mb_cmd.hdr.cmd = EVENT_LCD_QSPI_CLOSE_MBCMD;
        mb_cmd.param1 = 0;
        mb_cmd.param2 = 0;
        mb_cmd.param3 = 0;

        ret = mb_chnl_write(MB_CHNL_LCD_QSPI, &mb_cmd);
        if (ret != BK_OK)
        {
            LOGE("[%s] mb_chnl_write failed: %d\n", __func__, ret);
            break;
        }

        ret = rtos_get_semaphore(&lcd_qspi_disp_sem, 5000);
        if (ret != BK_OK)
            LOGE("[%s]semaphore get failed: %d\n", __func__, ret);
        else
            LOGI("[%s] close lcd success\r\n", __FUNCTION__);

        if(ret == BK_OK)
        {
            rtos_deinit_mutex(&g_lcd_qspi_mutex_master);
            rtos_deinit_semaphore(&lcd_qspi_disp_sem);
            g_lcd_qspi_open_flag = 0;
        }
    }while(0);

    return ret;
}

#endif

