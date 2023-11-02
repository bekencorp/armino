#include <stdio.h>
#include <stdlib.h>
#include "os/os.h"
#include "os/mem.h"
#include "os/str.h"
#include <components/log.h>
#include "modules/bt.h"
#include "modules/bt_types.h"
#include "modules/ble_types.h"
#include "modules/dm_bt.h"
#include "common/bk_assert.h"

#include "ring_buffer_particle.h"

#include "a2dp_source_demo_avrcp.h"
#include "a2dp_source_demo.h"


enum
{
    AVRCP_MSG_PASSTHROUGH,
};


typedef struct
{
    uint8_t inited;
    uint8_t peer_addr[6];
    uint8_t conn_state;

    uint16_t tg_current_registered_noti;

    uint8_t ct_peer_vol;
    uint16_t ct_peer_cap;
} avrcp_env_s;

typedef struct
{
    uint8_t type;
    uint8_t key;
    uint16_t len;
    char *data;
} avrcp_ctx_msg_t;

enum
{
    AVRCP_DEBUG_LEVEL_ERROR,
    AVRCP_DEBUG_LEVEL_WARNING,
    AVRCP_DEBUG_LEVEL_INFO,
    AVRCP_DEBUG_LEVEL_DEBUG,
    AVRCP_DEBUG_LEVEL_VERBOSE,
};

#define ACRCP_MSG_COUNT          (5)

#define AVRCP_DEBUG_LEVEL AVRCP_DEBUG_LEVEL_INFO

#define avrcp_loge(format, ...) do{if(AVRCP_DEBUG_LEVEL >= AVRCP_DEBUG_LEVEL_ERROR)   BK_LOGE("avrcp", "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define avrcp_logw(format, ...) do{if(AVRCP_DEBUG_LEVEL >= AVRCP_DEBUG_LEVEL_WARNING) BK_LOGW("avrcp", "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define avrcp_logi(format, ...) do{if(AVRCP_DEBUG_LEVEL >= AVRCP_DEBUG_LEVEL_INFO)    BK_LOGI("avrcp", "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define avrcp_logd(format, ...) do{if(AVRCP_DEBUG_LEVEL >= AVRCP_DEBUG_LEVEL_DEBUG)   BK_LOGI("avrcp", "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define avrcp_logv(format, ...) do{if(AVRCP_DEBUG_LEVEL >= AVRCP_DEBUG_LEVEL_VERBOSE) BK_LOGI("avrcp", "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)

static int32_t avrcp_pos_report_start_timer(timer_handler_t handle, uint32_t ms, uint8_t *data);
static void avrcp_pos_report_timer_hdl(void *param);

static avrcp_env_s s_avrcp_env;
static beken_timer_t s_avrcp_pos_report_timer;
static beken_thread_t s_avrcp_ctx_thread_handle = NULL;
static beken_queue_t s_avrcp_ctx_thread_queue = NULL;

static bk_avrcp_playback_stat_t a2dp_playstatus_to_avrcp_playback(int status)
{
    switch (status)
    {
    case A2DP_PLAY_STATUS_STOPPED:
        return BK_AVRCP_PLAYBACK_STOPPED;
        break;

    case A2DP_PLAY_STATUS_PLAYING:
        return BK_AVRCP_PLAYBACK_PLAYING;
        break;

    case A2DP_PLAY_STATUS_PAUSED:
        return BK_AVRCP_PLAYBACK_PAUSED;
        break;

    case A2DP_PLAY_STATUS_FWD_SEEK:
        return BK_AVRCP_PLAYBACK_FWD_SEEK;
        break;

    case A2DP_PLAY_STATUS_REV_SEEK:
        return BK_AVRCP_PLAYBACK_REV_SEEK;
        break;

    default:
        return BK_AVRCP_PLAYBACK_ERROR;
        break;

    }
}

static void avrcp_ct_recv_noti_cb(uint8_t event_id, bk_avrcp_rn_param_t *event_parameter)
{
    switch (event_id)
    {
    case BK_AVRCP_RN_VOLUME_CHANGE:
    {
        avrcp_logi("BK_AVRCP_RN_VOLUME_CHANGE:%d", event_parameter->volume);
        s_avrcp_env.ct_peer_vol = event_parameter->volume;
    }
    break;

    case BK_AVRCP_RN_BATTERY_STATUS_CHANGE:
        avrcp_logi("BK_AVRCP_RN_BATTERY_STATUS_CHANGE:%d", event_parameter->batt);
        break;

    default:
        avrcp_loge("unhandled event: %d", event_id);
        return;
    }

    bk_bt_ct_send_register_notification_cmd(s_avrcp_env.peer_addr, event_id, 0);
}

static void avrcp_tg_recv_passthrough_cb(uint8_t key)
{
    switch (key)
    {
    case BK_AVRCP_PT_CMD_PLAY:
        //avrcp_logi("play");
        bt_a2dp_source_demo_music_play(1, NULL);
        break;

    case BK_AVRCP_PT_CMD_STOP:
        //avrcp_logi("stop");
        bt_a2dp_source_demo_music_stop();
        break;

    case BK_AVRCP_PT_CMD_PAUSE:
        //avrcp_logi("pause");
        bt_a2dp_source_demo_music_pause();
        break;

    case BK_AVRCP_PT_CMD_REWIND:
        //avrcp_logi("rewind");
        break;

    case BK_AVRCP_PT_CMD_FAST_FORWARD:
        //avrcp_logi("fast forward");
        break;

    case BK_AVRCP_PT_CMD_FORWARD:
        //avrcp_logi("next");
        bt_a2dp_source_demo_music_next();
        break;

    case BK_AVRCP_PT_CMD_BACKWARD:
        //avrcp_logi("prev");
        bt_a2dp_source_demo_music_prev();
        break;

    default:
        //avrcp_loge("unsupport key: %d", key);
        return;
    }

    bt_avrcp_demo_report_playback(a2dp_playstatus_to_avrcp_playback(bt_a2dp_source_demo_get_play_status()));
}


void avrcp_ct_cb(bk_avrcp_ct_cb_event_t event, bk_avrcp_ct_cb_param_t *param)
{
    avrcp_logd("event: %d", event);

    bk_avrcp_ct_cb_param_t *avrcp = (bk_avrcp_ct_cb_param_t *)(param);

    switch (event)
    {
    case BK_AVRCP_CT_CONNECTION_STATE_EVT:
    {
        uint8_t *bda = avrcp->conn_state.remote_bda;
        avrcp_logi("avrcp ct connection state: %d, [%02x:%02x:%02x:%02x:%02x:%02x]",
                   avrcp->conn_state.connected, bda[5], bda[4], bda[3], bda[2], bda[1], bda[0]);

        if (avrcp->conn_state.connected)
        {
            bk_bt_avrcp_ct_send_get_rn_capabilities_cmd(bda);
        }
    }
    break;

    case BK_AVRCP_CT_GET_RN_CAPABILITIES_RSP_EVT:
    {
        avrcp_logi("AVRCP peer supported notification events 0x%x", avrcp->get_rn_caps_rsp.evt_set.bits);

        s_avrcp_env.ct_peer_cap = avrcp->get_rn_caps_rsp.evt_set.bits;

        if (s_avrcp_env.ct_peer_cap & (0x01 << BK_AVRCP_RN_VOLUME_CHANGE))
        {
            bk_bt_ct_send_register_notification_cmd(s_avrcp_env.peer_addr, BK_AVRCP_RN_VOLUME_CHANGE, 0);
        }

        if (s_avrcp_env.ct_peer_cap & (0x01 << BK_AVRCP_RN_BATTERY_STATUS_CHANGE))
        {
            bk_bt_ct_send_register_notification_cmd(s_avrcp_env.peer_addr, BK_AVRCP_RN_BATTERY_STATUS_CHANGE, 0);
        }
    }
    break;

    case BK_AVRCP_CT_CHANGE_NOTIFY_EVT:
    {
        avrcp_logi("AVRCP event notification: 0x%x", avrcp->change_ntf.event_id);
        avrcp_ct_recv_noti_cb(avrcp->change_ntf.event_id, &avrcp->change_ntf.event_parameter);
    }
    break;

    default:
        avrcp_loge("Invalid AVRCP event: %d", event);
        break;
    }
}


static void avrcp_tg_cb(bk_avrcp_tg_cb_event_t event, bk_avrcp_tg_cb_param_t *param)
{
    int ret = 0;

    avrcp_logd("event: %d", __func__, event);

    switch (event)
    {
    case BK_AVRCP_TG_CONNECTION_STATE_EVT:
    {
        uint8_t status = param->conn_stat.connected;
        uint8_t *bda = param->conn_stat.remote_bda;
        avrcp_logi("avrcp tg connection state: %d, [%02x:%02x:%02x:%02x:%02x:%02x]",
                   status, bda[5], bda[4], bda[3], bda[2], bda[1], bda[0]);

        if (s_avrcp_env.conn_state != status)
        {
            s_avrcp_env.conn_state = status;
        }

        os_memcpy(s_avrcp_env.peer_addr, bda, sizeof(s_avrcp_env.peer_addr));

        if (status == 0)
        {
            os_memset(&s_avrcp_env, 0, sizeof(s_avrcp_env));
            s_avrcp_env.inited = 1;
            //bt_a2dp_source_demo_stop_all();
        }
    }
    break;

    case BK_AVRCP_TG_PASSTHROUGH_CMD_EVT:
    {
        if (!param->psth_cmd.key_state)
        {
            avrcp_logd("recv pt key 0x%02x press", param->psth_cmd.key_code);
        }
        else
        {
            avrcp_logd("recv pt key 0x%02x release", param->psth_cmd.key_code);

            switch (param->psth_cmd.key_code)
            {

            case BK_AVRCP_PT_CMD_PLAY:
                avrcp_logi("play");
                break;

            case BK_AVRCP_PT_CMD_STOP:
                avrcp_logi("stop");
                break;

            case BK_AVRCP_PT_CMD_PAUSE:
                avrcp_logi("pause");
                break;

            case BK_AVRCP_PT_CMD_REWIND:
                avrcp_logi("rewind");
                break;

            case BK_AVRCP_PT_CMD_FAST_FORWARD:
                avrcp_logi("fast forward");
                break;

            case BK_AVRCP_PT_CMD_FORWARD:
                avrcp_logi("next");
                break;

            case BK_AVRCP_PT_CMD_BACKWARD:
                avrcp_logi("prev");
                break;

            default:
                break;
            }

            //avrcp_tg_recv_passthrough_cb(param->psth_cmd.key_code);

            avrcp_ctx_msg_t demo_msg;

            if (s_avrcp_ctx_thread_queue == NULL)
            {
                break;
            }

            os_memset(&demo_msg, 0x0, sizeof(demo_msg));
            demo_msg.type = AVRCP_MSG_PASSTHROUGH;
            demo_msg.key = param->psth_cmd.key_code;

            ret = rtos_push_to_queue(&s_avrcp_ctx_thread_queue, &demo_msg, BEKEN_NO_WAIT);

            if (ret)
            {
                avrcp_loge("send queue failed");

                if (demo_msg.data)
                {
                    os_free(demo_msg.data);
                }
            }

        }
    }
    break;

    //    case BK_AVRCP_TG_SET_ABSOLUTE_VOLUME_CMD_EVT:
    //    {
    //        avrcp_logw("recv abs vol %d, notice if local is central/master will not recv this msg !!!", param->set_abs_vol.volume);
    //    }
    //    break;

    case BK_AVRCP_TG_REGISTER_NOTIFICATION_EVT:
    {
        avrcp_logi("recv reg evt 0x%x param %d", param->reg_ntf.event_id, param->reg_ntf.event_parameter);

        s_avrcp_env.tg_current_registered_noti |= (1 << param->reg_ntf.event_id);

        bk_avrcp_rn_param_t cmd;

        os_memset(&cmd, 0, sizeof(cmd));

        switch (param->reg_ntf.event_id)
        {
        case BK_AVRCP_RN_PLAY_STATUS_CHANGE:
            cmd.playback = a2dp_playstatus_to_avrcp_playback(bt_a2dp_source_demo_get_play_status());
            break;

        case BK_AVRCP_RN_PLAY_POS_CHANGED:
            if (!param->reg_ntf.event_parameter)
            {
                avrcp_loge("pos change param err, not be zero !!!");
                goto error;
            }

            avrcp_pos_report_start_timer(avrcp_pos_report_timer_hdl, param->reg_ntf.event_parameter, NULL);
            cmd.play_pos = bt_a2dp_source_demo_get_play_pos();
            break;

        case BK_AVRCP_RN_BATTERY_STATUS_CHANGE:
            cmd.batt = BK_AVRCP_BATT_EXTERNAL;
            break;

        default:
            avrcp_loge("unknow reg event 0x%x", param->reg_ntf.event_id);
            goto error;
        }

        ret = bk_bt_avrcp_tg_send_rn_rsp(s_avrcp_env.peer_addr, param->reg_ntf.event_id, BK_AVRCP_RN_RSP_INTERIM, &cmd);

        if (ret)
        {
            avrcp_loge("send rn rsp err %d", ret);
        }
    }
    break;

    default:
        avrcp_loge("unknow event 0x%x", event);
        break;
    }

error:;
}

static int32_t avrcp_pos_report_start_timer(timer_handler_t handle, uint32_t ms, uint8_t *data)
{
    bk_err_t ret = 0;

    if (rtos_is_timer_init(&s_avrcp_pos_report_timer))
    {
        if (rtos_is_timer_running(&s_avrcp_pos_report_timer))
        {
            rtos_stop_timer(&s_avrcp_pos_report_timer);
        }

        rtos_deinit_timer(&s_avrcp_pos_report_timer);
    }

    if (!rtos_is_timer_init(&s_avrcp_pos_report_timer))
    {
        ret = rtos_init_timer(&s_avrcp_pos_report_timer, ms, handle, (void *)data);

        if (ret)
        {
            avrcp_loge("rtos_init_timer err %d", ret);
            goto error;
        }
    }

    ret = rtos_change_period(&s_avrcp_pos_report_timer, ms);

    if (ret)
    {
        avrcp_loge("rtos_change_period err %d", ret);
        goto error;
    }

    ret = rtos_start_timer(&s_avrcp_pos_report_timer);

    if (ret)
    {
        avrcp_loge("rtos_start_timer err %d", ret);
        goto error;
    }

    return 0;
error:;
    return ret;
}

static int32_t avrcp_pos_report_stop_timer(void)
{
    if (rtos_is_timer_init(&s_avrcp_pos_report_timer))
    {
        if (rtos_is_timer_running(&s_avrcp_pos_report_timer))
        {
            rtos_stop_timer(&s_avrcp_pos_report_timer);
        }

        rtos_deinit_timer(&s_avrcp_pos_report_timer);
    }

    return 0;
}

static void avrcp_pos_report_timer_hdl(void *param)
{
    bt_avrcp_demo_report_play_pos(bt_a2dp_source_demo_get_play_pos());
}

int bt_avrcp_demo_report_playback(uint8_t status)
{
    if (!s_avrcp_env.inited)
    {
        avrcp_loge("not init");
        return -1;
    }

    if (!s_avrcp_env.conn_state)
    {
        avrcp_loge("not connect");
        return -1;
    }

    if (!(s_avrcp_env.tg_current_registered_noti & (1 << BK_AVRCP_RN_PLAY_STATUS_CHANGE)))
    {
        avrcp_loge("event not reg BK_AVRCP_RN_PLAY_STATUS_CHANGE");
        return 0;
    }

    bk_avrcp_rn_param_t param;

    os_memset(&param, 0, sizeof(param));
    param.playback = status;

    s_avrcp_env.tg_current_registered_noti &= ~(1 << BK_AVRCP_RN_PLAY_STATUS_CHANGE);

    avrcp_logi("report playback %d", status);
    return bk_bt_avrcp_tg_send_rn_rsp(s_avrcp_env.peer_addr, BK_AVRCP_RN_PLAY_STATUS_CHANGE, BK_AVRCP_RN_RSP_CHANGED, &param);
}

int bt_avrcp_demo_report_batt_status(uint8_t status)
{
    if (!s_avrcp_env.inited)
    {
        avrcp_loge("not init");
        return -1;
    }

    if (!s_avrcp_env.conn_state)
    {
        avrcp_loge("not connect");
        return -1;
    }

    if (!(s_avrcp_env.tg_current_registered_noti & (1 << BK_AVRCP_RN_BATTERY_STATUS_CHANGE)))
    {
        avrcp_loge("event not reg BK_AVRCP_RN_BATTERY_STATUS_CHANGE");
        return 0;
    }

    bk_avrcp_rn_param_t param;

    os_memset(&param, 0, sizeof(param));
    param.batt = status;

    s_avrcp_env.tg_current_registered_noti &= ~(1 << BK_AVRCP_RN_BATTERY_STATUS_CHANGE);

    avrcp_logi("report batt status %d", status);
    return bk_bt_avrcp_tg_send_rn_rsp(s_avrcp_env.peer_addr, BK_AVRCP_RN_BATTERY_STATUS_CHANGE, BK_AVRCP_RN_RSP_CHANGED, &param);
}

int bt_avrcp_demo_report_play_pos(uint32_t pos)
{
    if (!s_avrcp_env.inited)
    {
        avrcp_loge("not init");
        return -1;
    }

    if (!s_avrcp_env.conn_state)
    {
        avrcp_loge("not connect");
        return -1;
    }

    if (!(s_avrcp_env.tg_current_registered_noti & (1 << BK_AVRCP_RN_PLAY_POS_CHANGED)))
    {
        avrcp_loge("event not reg BK_AVRCP_RN_PLAY_POS_CHANGED");
        return 0;
    }

    bk_avrcp_rn_param_t param;

    os_memset(&param, 0, sizeof(param));
    param.play_pos = pos;

    s_avrcp_env.tg_current_registered_noti &= ~(1 << BK_AVRCP_RN_PLAY_POS_CHANGED);

    avrcp_logi("report play pos %d", pos);
    return bk_bt_avrcp_tg_send_rn_rsp(s_avrcp_env.peer_addr, BK_AVRCP_RN_PLAY_POS_CHANGED, BK_AVRCP_RN_RSP_CHANGED, &param);
}


static void avrcp_ctx_task(void *arg)
{
    bk_err_t err = 0;

    avrcp_ctx_msg_t msg;

    while (1)
    {
        err = rtos_pop_from_queue(&s_avrcp_ctx_thread_queue, &msg, BEKEN_WAIT_FOREVER);

        if (err)
        {
            avrcp_loge("pop queue err %d", err);
            goto error;
        }

        switch (msg.type)
        {
        case AVRCP_MSG_PASSTHROUGH:
            avrcp_tg_recv_passthrough_cb(msg.key);
            break;

        default:
            break;
        }

        if (msg.data && msg.len)
        {
            os_free(msg.data);
        }
    }

error:;
    rtos_delete_thread(NULL);
}

static bk_err_t avrcp_create_ctx_task(void)
{
    bk_err_t err = 0;

    if (!s_avrcp_ctx_thread_handle)
    {
        err = rtos_init_queue(&s_avrcp_ctx_thread_queue,
                              "avrcp_ctx_queue",
                              sizeof(avrcp_ctx_msg_t),
                              ACRCP_MSG_COUNT);

        if (err != kNoErr)
        {
            avrcp_loge("msg queue failed");
            return BK_FAIL;
        }

        err = rtos_create_thread(&s_avrcp_ctx_thread_handle,
                                 BEKEN_DEFAULT_WORKER_PRIORITY - 2,
                                 "avrcp_ctx",
                                 (beken_thread_function_t)avrcp_ctx_task,
                                 1024 * 3,
                                 NULL);

        if (err)
        {
            avrcp_loge("task fail");

            s_avrcp_ctx_thread_handle = NULL;
            rtos_deinit_queue(&s_avrcp_ctx_thread_queue);
            s_avrcp_ctx_thread_queue = NULL;
            goto error;
        }
    }

    return 0;

error:;

    return err;
}


int bt_avrcp_demo_init(void)
{
    int ret = 0;
    bk_avrcp_rn_evt_cap_mask_t tmp_cap = {0};
    bk_avrcp_rn_evt_cap_mask_t final_cap =
    {
        .bits = 0
        | (1 << BK_AVRCP_RN_PLAY_STATUS_CHANGE)
        | (1 << BK_AVRCP_RN_PLAY_POS_CHANGED)
        | (1 << BK_AVRCP_RN_BATTERY_STATUS_CHANGE)
        ,
    };

    if (s_avrcp_env.inited)
    {
        avrcp_loge("already init");
        return -1;
    }

    ret = avrcp_create_ctx_task();

    if (ret)
    {
        return -1;
    }

    bk_bt_avrcp_tg_init();
    bk_bt_avrcp_ct_init();

    bk_bt_avrcp_tg_get_rn_evt_cap(BK_AVRCP_RN_CAP_API_METHOD_ALLOWED, &tmp_cap);

    final_cap.bits &= tmp_cap.bits;

    avrcp_logi("set rn cap 0x%x", final_cap.bits);

    ret = bk_bt_avrcp_tg_set_rn_evt_cap(&final_cap);

    if (ret)
    {
        avrcp_loge("set rn cap err %d", ret);
        return -1;
    }

    bk_bt_avrcp_tg_register_callback(avrcp_tg_cb);
    bk_bt_avrcp_ct_register_callback(avrcp_ct_cb);

    s_avrcp_env.inited = 1;
    return 0;
}

int bt_avrcp_is_ready(void)
{
    return s_avrcp_env.conn_state && s_avrcp_env.inited;
}
