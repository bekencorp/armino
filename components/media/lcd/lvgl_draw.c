#include "os/os.h"
#include "mailbox/mailbox_channel.h"
#include "media_evt.h"
#include "components/log.h"
#include "lvgl.h"
#include "lv_draw_sw_blend.h"

#define TAG "rotate"
#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#ifdef CONFIG_MASTER_CORE
static beken_semaphore_t g_sem = NULL;
static void _mailbox_rx_isr(void *param, mb_chnl_cmd_t *cmd_buf)
{
    bk_err_t ret;

    //bk_printf("%s, %08X\n", __func__, cmd_buf->param1);
    if (cmd_buf->hdr.cmd == 0x19)
    {
        ret = rtos_set_semaphore(&g_sem);
        if (ret != BK_OK)
        {
            //bk_printf("%s semaphore get failed: %d\n", __func__, ret);
        }
    }
}

static void _mailbox_tx_isr(void *param)
{
    //bk_printf("%s\n", __func__);
}

static void _mailbox_tx_cmpl_isr(void *param, mb_chnl_ack_t *ack_buf)
{
    //bk_printf("%s\n", __func__);
}

void lv_draw_sw_master_init(void)
{
    /* Mailbox */
    mb_chnl_open(MB_CHNL_LVGL, NULL);
    mb_chnl_ctrl(MB_CHNL_LVGL, MB_CHNL_SET_RX_ISR, _mailbox_rx_isr);
    mb_chnl_ctrl(MB_CHNL_LVGL, MB_CHNL_SET_TX_ISR, _mailbox_tx_isr);
    mb_chnl_ctrl(MB_CHNL_LVGL, MB_CHNL_SET_TX_CMPL_ISR, _mailbox_tx_cmpl_isr);

    rtos_init_semaphore_ex(&g_sem, 1, 0);
}

#ifdef CONFIG_LVGL_DRAW_ON_CPU1
void lv_draw_sw_blend_basic_to_slave(lv_draw_ctx_t * draw_ctx, const lv_draw_sw_blend_dsc_t * dsc)
{
    mb_chnl_cmd_t mb_cmd;
    bk_err_t ret = BK_FAIL;

    do{
        mb_cmd.hdr.cmd = 0x18;
        mb_cmd.param1 = (uint32_t)draw_ctx;
        mb_cmd.param2 = (uint32_t)dsc;;
        mb_cmd.param3 = (uint32_t)_lv_refr_get_disp_refreshing();

        ret = mb_chnl_write(MB_CHNL_LVGL, &mb_cmd);
        if (ret != BK_OK)
        {
            //LOGE("%s mb_chnl_write failed: %d\n", __func__, ret);
            break;
        }

        ret = rtos_get_semaphore(&g_sem, BEKEN_NEVER_TIMEOUT);
        if (ret != BK_OK)
        {
            //LOGE("%s semaphore get failed: %d\n", __func__, ret);
            break;
        }
    }while(0);
    
    return ;
}
#endif
#endif

#if CONFIG_SLAVE_CORE
static void _mailbox_rx_isr(void *param, mb_chnl_cmd_t *cmd_buf)
{
#ifdef CONFIG_LVGL_DRAW_ON_CPU1
    LOGD("%s, %08X\n", __func__, cmd_buf->hdr.cmd);

    if (cmd_buf->hdr.cmd == 0x18)
    {
        void bk_gui_set_cpu1_param(lv_draw_ctx_t * draw_ctx_param, 
                                    lv_draw_sw_blend_dsc_t * dsc_param, 
                                    lv_disp_t *disp_param);
        bk_gui_set_cpu1_param((lv_draw_ctx_t *)cmd_buf->param1,
                                (lv_draw_sw_blend_dsc_t *)cmd_buf->param2,
                                (lv_disp_t *)cmd_buf->param3);

        media_msg_t msg;
        msg.event = EVENT_LVGL_DRAW_CMD;
        media_send_msg(&msg);
    }
#endif
}

static void _mailbox_tx_isr(void *param)
{
    LOGD("%s\n", __func__);
}

static void _mailbox_tx_cmpl_isr(void *param, mb_chnl_ack_t *ack_buf)
{
    LOGD("%s\n", __func__);
}

void lv_draw_sw_slave_init(void)
{
    mb_chnl_open(MB_CHNL_LVGL, NULL);
    mb_chnl_ctrl(MB_CHNL_LVGL, MB_CHNL_SET_RX_ISR, _mailbox_rx_isr);
    mb_chnl_ctrl(MB_CHNL_LVGL, MB_CHNL_SET_TX_ISR, _mailbox_tx_isr);
    mb_chnl_ctrl(MB_CHNL_LVGL, MB_CHNL_SET_TX_CMPL_ISR, _mailbox_tx_cmpl_isr);
}
#endif
