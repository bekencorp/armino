#include <components/system.h>
#include <os/os.h>
#include <os/mem.h>
#include <os/str.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "a2dp_sink_demo.h"

#include "modules/dm_bt.h"

#include <driver/sbc_types.h>
#include <driver/aud_types.h>
#include <driver/aud.h>
#include <components/aud_intf.h>
#include <driver/sbc.h>
#include "aac_decoder.h"
#include "ring_buffer_node.h"


#define CODEC_AUDIO_SBC                      0x00U
#define CODEC_AUDIO_AAC                      0x02U

#define BT_AUDIO_SINK_DEMO_MSG_COUNT          (30)

#define LOCAL_NAME "soundbar"


#define A2DP_CACHE_BUFFER_SIZE  ((CONFIG_A2DP_CACHE_FRAME_NUM + 2) * 1024)
#define A2DP_SBC_FRAME_BUFFER_MAX_SIZE  128   /**< A2DP SBC encoded frame maximum size */
#define A2DP_AAC_FRAME_BUFFER_MAX_SIZE  1024  /**< A2DP AAC encoded frame maximum size */

enum
{
    BT_AUDIO_MSG_NULL = 0,
    BT_AUDIO_D2DP_START_MSG = 1,
    BT_AUDIO_D2DP_STOP_MSG = 2,
    BT_AUDIO_D2DP_DATA_IND_MSG = 3,
    BT_AUDIO_D2DP_SEND_DATA_2_SPK_MSG = 4,
};

typedef struct
{
    uint8_t type;
    uint16_t len;
    char *data;
} bt_audio_sink_demo_msg_t;

static bk_a2dp_mcc_t bt_audio_a2dp_sink_codec = {0};
static bk_bt_linkkey_storage_t s_bt_linkkey;


static sbcdecodercontext_t bt_audio_sink_sbc_decoder;
static beken_queue_t bt_audio_sink_demo_msg_que = NULL;
static beken_thread_t bt_audio_sink_demo_thread_handle = NULL;
static void *sink_aac_decoder = NULL;
//static uint16_t speaker_frame_size = 0;
static uint8_t s_frames_2_spk_per_pkt = 0;
static uint8_t s_frame_caching = 0;
static RingBufferNodeContext s_a2dp_frame_nodes;
static uint8_t s_spk_is_started = 0;
static uint8_t *p_cache_buff = NULL;

uint8_t bt_audio_a2dp_sink_convert_to_dac_sample_rate(uint8_t codec_type, uint32_t sample_rate)
{
    uint8_t dac_sample_rate = AUD_DAC_SAMP_RATE_44_1K;

    if (CODEC_AUDIO_SBC == codec_type)
    {
        switch (sample_rate)
        {
        case 16000:
            dac_sample_rate = AUD_DAC_SAMP_RATE_16K;
            break;

        case 32000:
            dac_sample_rate = AUD_DAC_SAMP_RATE_32K;
            break;

        case 44100:
            dac_sample_rate = AUD_DAC_SAMP_RATE_44_1K;
            break;

        case 48000:
            dac_sample_rate = AUD_DAC_SAMP_RATE_48K;
            break;

        default:
            os_printf("Unsupported sbc sample rate %d \r\n", sample_rate);
            break;
        }
    }
    else if (CODEC_AUDIO_AAC == codec_type)
    {
        switch (sample_rate)
        {
        case 44100:
            dac_sample_rate = AUD_DAC_SAMP_RATE_44_1K;
            break;

        case 48000:
            dac_sample_rate = AUD_DAC_SAMP_RATE_48K;
            break;

        default:
            os_printf("Unsupported aac sample rate %d \r\n", sample_rate);
            break;
        }
    }
    else
    {
        os_printf("%s, Unsupported codec %d \r\n", __func__, codec_type);
    }

    return dac_sample_rate;
}

static bk_err_t one_spk_frame_played_cmpl_handler(unsigned int size)
{
    bt_audio_sink_demo_msg_t demo_msg;
    int rc = -1;

    if (bt_audio_sink_demo_msg_que == NULL)
    {
        return BK_OK;
    }

    demo_msg.type = BT_AUDIO_D2DP_SEND_DATA_2_SPK_MSG;
    demo_msg.len = 0;

    rc = rtos_push_to_queue(&bt_audio_sink_demo_msg_que, &demo_msg, BEKEN_NO_WAIT);

    if (kNoErr != rc)
    {
        os_printf("%s, send queue failed\r\n", __func__);
    }
    return BK_OK;
}

void bt_audio_sink_demo_main(void *arg)
{
    while (1)
    {
        bk_err_t err;
        bt_audio_sink_demo_msg_t msg;

        err = rtos_pop_from_queue(&bt_audio_sink_demo_msg_que, &msg, BEKEN_WAIT_FOREVER);

        if (kNoErr == err)
        {
            switch (msg.type)
            {
            case BT_AUDIO_D2DP_START_MSG:
            {
                bk_err_t ret = BK_OK;
                bk_a2dp_mcc_t *p_codec_info = (bk_a2dp_mcc_t *)msg.data;
                uint8_t dac_sample_rate = 0;
                uint16_t frame_size = 0;

                s_frame_caching = 0;
                s_spk_is_started = 0;

                os_printf("BT_AUDIO_D2DP_START_MSG \r\n");

                if (CODEC_AUDIO_SBC == p_codec_info->type)
                {
                    dac_sample_rate = bt_audio_a2dp_sink_convert_to_dac_sample_rate(p_codec_info->type, p_codec_info->cie.sbc_codec.sample_rate);
                    s_frames_2_spk_per_pkt = (p_codec_info->cie.sbc_codec.sample_rate * 20 / 1000 / (p_codec_info->cie.sbc_codec.block_len * p_codec_info->cie.sbc_codec.subbands)) + 1;
                    frame_size = s_frames_2_spk_per_pkt * (p_codec_info->cie.sbc_codec.block_len * p_codec_info->cie.sbc_codec.subbands) * 2 * 2;
                    os_printf("frames_2_speaker %d, frame_size %d\r\n",s_frames_2_spk_per_pkt, frame_size);
                    bk_sbc_decoder_init(&bt_audio_sink_sbc_decoder);

                    p_cache_buff  = (uint8_t *)os_malloc(A2DP_CACHE_BUFFER_SIZE);
                    if (!p_cache_buff)
                    {
                        os_printf("%s, malloc cache buf failed!!!\r\n", __func__);
                    }
                    else
                    {
                        ring_buffer_node_init(&s_a2dp_frame_nodes, p_cache_buff, A2DP_SBC_FRAME_BUFFER_MAX_SIZE, A2DP_CACHE_BUFFER_SIZE / A2DP_SBC_FRAME_BUFFER_MAX_SIZE);
                    }
                }
                else if (CODEC_AUDIO_AAC == p_codec_info->type)
                {
                    dac_sample_rate = bt_audio_a2dp_sink_convert_to_dac_sample_rate(p_codec_info->type, p_codec_info->cie.aac_codec.sample_rate);
                    s_frames_2_spk_per_pkt = 1;
                    frame_size = 4096;
                    sink_aac_decoder = (void *)(os_malloc(aac_decoder_get_ram_size_without_in_buffer()));

                    if (!sink_aac_decoder)
                    {
                        os_printf("%s, malloc sink_aac_decoder failed!!!\r\n", __func__);
                    }
                    else
                    {
                        bt_a2dp_aac_decoder_init(sink_aac_decoder, p_codec_info->cie.aac_codec.sample_rate, p_codec_info->cie.aac_codec.channels);
                    }

                    p_cache_buff  = (uint8_t *)os_malloc(A2DP_CACHE_BUFFER_SIZE);
                    if (!p_cache_buff)
                    {
                        os_printf("%s, malloc cache buf failed!!!\r\n", __func__);
                    }
                    else
                    {
                        ring_buffer_node_init(&s_a2dp_frame_nodes, p_cache_buff, A2DP_AAC_FRAME_BUFFER_MAX_SIZE, A2DP_CACHE_BUFFER_SIZE / A2DP_AAC_FRAME_BUFFER_MAX_SIZE);
                    }
                }
                else
                {
                    os_printf("%s, Unsupported codec %d \r\n", __func__, p_codec_info->type);
                }

                os_printf("dac_sample_rate %d \r\n", dac_sample_rate);

                aud_intf_drv_setup_t aud_intf_drv_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
                aud_intf_spk_setup_t aud_intf_spk_setup = DEFAULT_AUD_INTF_SPK_SETUP_CONFIG();
                aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;

                aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
                aud_intf_drv_setup.task_config.priority = 3;
                aud_intf_drv_setup.aud_intf_rx_spk_data = one_spk_frame_played_cmpl_handler;
                aud_intf_drv_setup.aud_intf_tx_mic_data = NULL;
                ret = bk_aud_intf_drv_init(&aud_intf_drv_setup);

                if (ret != BK_ERR_AUD_INTF_OK)
                {
                    os_printf("bk_aud_intf_drv_init fail, ret:%d \r\n", ret);
                }
                else
                {
                    os_printf("bk_aud_intf_drv_init complete \r\n");
                }

                aud_work_mode = AUD_INTF_WORK_MODE_GENERAL;
                ret = bk_aud_intf_set_mode(aud_work_mode);

                if (ret != BK_ERR_AUD_INTF_OK)
                {
                    os_printf("bk_aud_intf_set_mode fail, ret:%d \r\n", ret);
                }
                else
                {
                    os_printf("bk_aud_intf_set_mode complete \r\n");
                }

                aud_intf_spk_setup.spk_chl = AUD_INTF_SPK_CHL_DUAL;
                aud_intf_spk_setup.samp_rate = dac_sample_rate;
                aud_intf_spk_setup.frame_size = frame_size;
                aud_intf_spk_setup.spk_gain = 0x2d;
                aud_intf_spk_setup.work_mode = AUD_DAC_WORK_MODE_DIFFEN;
                ret = bk_aud_intf_spk_init(&aud_intf_spk_setup);

                if (ret != BK_ERR_AUD_INTF_OK)
                {
                    os_printf("bk_aud_intf_spk_init fail, ret:%d \r\n", ret);
                }
                else
                {
                    os_printf("bk_aud_intf_spk_init complete \r\n");
                }

                os_free(msg.data);
            }
            break;

            case BT_AUDIO_D2DP_DATA_IND_MSG:
            {
                bk_err_t ret = BK_OK;
                uint8 *fb = (uint8_t *)msg.data;;

                if (CODEC_AUDIO_SBC == bt_audio_a2dp_sink_codec.type)
                {
                    uint8_t frames = *fb++;
                    //os_printf("recv sbc frames %d\r\n",frames);

                    for (uint8_t i = 0; i < frames; i++)
                    {
                        uint32_t len = (msg.len - 1) / frames;

                        if (ring_buffer_node_get_free_nodes(&s_a2dp_frame_nodes))
                        {
                            uint8_t *node = ring_buffer_node_get_write_node(&s_a2dp_frame_nodes);
                            *((uint32_t *)node) = len;
                            os_memcpy(node + 4, fb, len);
                            fb += len;
                        }
                        else
                        {
                            os_printf("A2DP frame nodes buffer(sbc) is full\n");
                        }
                    }
                }
                else if (CODEC_AUDIO_AAC == bt_audio_a2dp_sink_codec.type)
                {
                    uint8_t *inbuf = &fb[9];
                    uint32_t inlen = 0;
                    uint8_t  len   = 255;

                    do
                    {
                        inlen += len = *inbuf++;
                    }
                    while (len == 255);

                    if (ring_buffer_node_get_free_nodes(&s_a2dp_frame_nodes))
                    {
                        uint8_t *node = ring_buffer_node_get_write_node(&s_a2dp_frame_nodes);
                        *((uint32_t *)node) = inlen;
                        os_memcpy(node + 4, inbuf, inlen);
                    }
                    else
                    {
                        os_printf("A2DP frame nodes buffer(aac) is full\n");
                    }
                }
                else
                {
                    os_printf("%s, cannot decode data due to unsupported a2dp codec %d \r\n", __func__, bt_audio_a2dp_sink_codec);
                }

                os_free(msg.data);

                if (0 == s_spk_is_started)
                {
                    s_frame_caching++;

                    if (s_frame_caching <= 2)
                    {
                        one_spk_frame_played_cmpl_handler(0);
                    }
                    else if (CONFIG_A2DP_CACHE_FRAME_NUM == s_frame_caching)
                    {
                        ret = bk_aud_intf_spk_start();

                        if (ret != BK_ERR_AUD_INTF_OK)
                        {
                            os_printf("bk_aud_intf_spk_start fail, ret:%d \r\n", ret);
                        }
                        else
                        {
                            os_printf("bk_aud_intf_spk_start complete \r\n");
                        }
                        s_spk_is_started = 1;
                    }
                }
            }
            break;

            case BT_AUDIO_D2DP_STOP_MSG:
            {
                os_printf("BT_AUDIO_D2DP_STOP_MSG \r\n");
                bk_aud_intf_spk_deinit();
                bk_aud_intf_drv_deinit();

                if (sink_aac_decoder)
                {
                    os_free(sink_aac_decoder);
                    sink_aac_decoder = NULL;
                }

                ring_buffer_node_clear(&s_a2dp_frame_nodes);

                if (p_cache_buff)
                {
                    os_free(p_cache_buff);
                    p_cache_buff = NULL;
                }
            }
            break;

            case BT_AUDIO_D2DP_SEND_DATA_2_SPK_MSG:
            {
                uint32_t frame_count = ring_buffer_node_get_fill_nodes(&s_a2dp_frame_nodes);
                uint32_t i, sent_frames = s_frames_2_spk_per_pkt > frame_count ? frame_count : s_frames_2_spk_per_pkt;

                //os_printf("frame_count %d, sent_frames %d \r\n", frame_count, sent_frames);

                for (i = 0; i < sent_frames; i++)
                {
                    uint8_t *inbuf = ring_buffer_node_get_read_node(&s_a2dp_frame_nodes);
                    uint32_t inlen = *(uint32_t *)inbuf;
                    bk_err_t ret;

                    inbuf += 4;//skip length

                    if (CODEC_AUDIO_SBC == bt_audio_a2dp_sink_codec.type)
                    {
                        ret = bk_sbc_decoder_frame_decode(&bt_audio_sink_sbc_decoder, inbuf, inlen);
                        if (ret < 0)
                        {
                            os_printf("sbc_decoder_decode error <%d>\n", ret);
                            break;
                        }

                        ret = bk_aud_intf_write_spk_data((uint8_t *)bt_audio_sink_sbc_decoder.pcm_sample, bt_audio_sink_sbc_decoder.pcm_length * 4);

                        if (ret != BK_OK)
                        {
                            os_printf("write spk data fail \r\n");
                        }
                    }
                    else if (CODEC_AUDIO_AAC == bt_audio_a2dp_sink_codec.type)
                    {
                        uint8_t *outbuf = NULL;
                        uint32_t outlen;
                        if (sink_aac_decoder && (0 == bt_a2dp_aac_decoder_decode(sink_aac_decoder, inbuf, inlen, &outbuf, &outlen)))
                        {
                            ret = bk_aud_intf_write_spk_data(outbuf, outlen);

                            if (ret != BK_OK)
                            {
                                os_printf("write spk data fail \r\n");
                            }
                        }
                        else
                        {
                            os_printf("bt_a2dp_aac_decoder_decode failed!\r\n");
                        }
                    }
                }
            }
            break;

            default:
                break;
            }
        }
    }

    rtos_deinit_queue(&bt_audio_sink_demo_msg_que);
    bt_audio_sink_demo_msg_que = NULL;
    bt_audio_sink_demo_thread_handle = NULL;
    rtos_delete_thread(NULL);
}

int bt_audio_sink_demo_task_init(void)
{
    bk_err_t ret = BK_OK;

    if ((!bt_audio_sink_demo_thread_handle) && (!bt_audio_sink_demo_msg_que))
    {
        ret = rtos_init_queue(&bt_audio_sink_demo_msg_que,
                              "bt_audio_sink_demo_msg_que",
                              sizeof(bt_audio_sink_demo_msg_t),
                              BT_AUDIO_SINK_DEMO_MSG_COUNT);

        if (ret != kNoErr)
        {
            os_printf("bt_audio sink demo msg queue failed \r\n");
            return BK_FAIL;
        }

        ret = rtos_create_thread(&bt_audio_sink_demo_thread_handle,
                                 BEKEN_DEFAULT_WORKER_PRIORITY,
                                 "bt_audio_sink_demo",
                                 (beken_thread_function_t)bt_audio_sink_demo_main,
                                 4096,
                                 (beken_thread_arg_t)0);

        if (ret != kNoErr)
        {
            os_printf("bt_audio sink demo task fail \r\n");
            rtos_deinit_queue(&bt_audio_sink_demo_msg_que);
            bt_audio_sink_demo_msg_que = NULL;
            bt_audio_sink_demo_thread_handle = NULL;
        }

        return kNoErr;
    }
    else
    {
        return kInProgressErr;
    }
}

void bt_audio_sink_media_data_ind(const uint8_t *data, uint16_t data_len)
{
    bt_audio_sink_demo_msg_t demo_msg;
    int rc = -1;

    os_memset(&demo_msg, 0x0, sizeof(bt_audio_sink_demo_msg_t));

    if (bt_audio_sink_demo_msg_que == NULL)
    {
        return;
    }

    demo_msg.data = (char *) os_malloc(data_len);

    if (demo_msg.data == NULL)
    {
        os_printf("%s, malloc failed\r\n", __func__);
        return;
    }

    os_memcpy(demo_msg.data, data, data_len);
    demo_msg.type = BT_AUDIO_D2DP_DATA_IND_MSG;
    demo_msg.len = data_len;

    rc = rtos_push_to_queue(&bt_audio_sink_demo_msg_que, &demo_msg, BEKEN_NO_WAIT);

    if (kNoErr != rc)
    {
        os_printf("%s, send queue failed\r\n", __func__);

        if (demo_msg.data)
        {
            os_free(demo_msg.data);
        }
    }
}

void bt_audio_a2dp_sink_suspend_ind(void)
{
    bt_audio_sink_demo_msg_t demo_msg;
    int rc = -1;

    os_memset(&demo_msg, 0x0, sizeof(bt_audio_sink_demo_msg_t));

    if (bt_audio_sink_demo_msg_que == NULL)
    {
        return;
    }

    demo_msg.type = BT_AUDIO_D2DP_STOP_MSG;
    demo_msg.len = 0;

    rc = rtos_push_to_queue(&bt_audio_sink_demo_msg_que, &demo_msg, BEKEN_NO_WAIT);

    if (kNoErr != rc)
    {
        os_printf("%s, send queue failed\r\n", __func__);
    }
}

void bt_audio_a2dp_sink_start_ind(bk_a2dp_mcc_t *codec)
{
    bt_audio_sink_demo_msg_t demo_msg;
    int rc = -1;

    os_memset(&demo_msg, 0x0, sizeof(bt_audio_sink_demo_msg_t));

    if (bt_audio_sink_demo_msg_que == NULL)
    {
        return;
    }

    demo_msg.data = (char *) os_malloc(sizeof(bk_a2dp_mcc_t));

    if (demo_msg.data == NULL)
    {
        os_printf("%s, malloc failed\r\n", __func__);
        return;
    }

    os_memcpy(demo_msg.data, codec, sizeof(bk_a2dp_mcc_t));
    demo_msg.type = BT_AUDIO_D2DP_START_MSG;
    demo_msg.len = sizeof(bk_a2dp_mcc_t);

    rc = rtos_push_to_queue(&bt_audio_sink_demo_msg_que, &demo_msg, BEKEN_NO_WAIT);

    if (kNoErr != rc)
    {
        os_printf("%s, send queue failed\r\n", __func__);
    }
}

static bk_a2dp_audio_state_t s_audio_state = BK_A2DP_AUDIO_STATE_SUSPEND;

void bk_bt_app_a2dp_sink_cb(bk_a2dp_cb_event_t event, bk_a2dp_cb_param_t *p_param)
{
    os_printf("%s event: %d\r\n", __func__, event);

    bk_a2dp_cb_param_t *a2dp = (bk_a2dp_cb_param_t *)(p_param);

    switch (event)
    {
    case BK_A2DP_CONNECTION_STATE_EVT:
    {
        uint8_t *bda = a2dp->conn_state.remote_bda;
        os_printf("A2DP connection state: %d, [%02x:%02x:%02x:%02x:%02x:%02x]\r\n",
                  a2dp->conn_state.state, bda[5], bda[4], bda[3], bda[2], bda[1], bda[0]);

        if (BK_A2DP_CONNECTION_STATE_DISCONNECTED == a2dp->conn_state.state)
        {
            if (BK_A2DP_AUDIO_STATE_STARTED == s_audio_state)
            {
                s_audio_state = BK_A2DP_AUDIO_STATE_SUSPEND;
                bt_audio_a2dp_sink_suspend_ind();
            }
        }
        else if (BK_A2DP_CONNECTION_STATE_CONNECTED == a2dp->conn_state.state)
        {

        }

    }
    break;

    case BK_A2DP_AUDIO_STATE_EVT:
    {
        os_printf("A2DP audio state: %d\r\n", a2dp->audio_state.state);

        if (BK_A2DP_AUDIO_STATE_STARTED == a2dp->audio_state.state)
        {
            s_audio_state = a2dp->audio_state.state;
            bt_audio_a2dp_sink_start_ind(&bt_audio_a2dp_sink_codec);
        }
        else if ((BK_A2DP_AUDIO_STATE_SUSPEND == a2dp->audio_state.state) && (BK_A2DP_AUDIO_STATE_STARTED == s_audio_state))
        {
            s_audio_state = a2dp->audio_state.state;
            bt_audio_a2dp_sink_suspend_ind();
        }
    }
    break;

    case BK_A2DP_AUDIO_CFG_EVT:
    {
        bt_audio_a2dp_sink_codec = a2dp->audio_cfg.mcc;
        os_printf("%s, codec_id %d \r\n", __func__, bt_audio_a2dp_sink_codec.type);
    }
    break;

    default:
        os_printf("Invalid A2DP event: %d\r\n", event);
        break;
    }
}

static uint32_t gap_event_cb(bt_event_enum_t event, void *param)
{
    switch (event)
    {
    case BK_DM_BT_EVENT_DISCONNECT:
        {
            uint8_t *addr = (uint8_t *)param;
            os_printf("Disconnected from %x %x %x %x %x %x \r\n",addr[5],addr[4],addr[3],addr[2],addr[1],addr[0]);
            bk_bt_gap_set_visibility(BK_BT_CONNECTABLE, BK_BT_DISCOVERABLE);
        }
        break;

    case BK_DM_BT_EVENT_CONNECTION_COMPLETE:
        {
            uint8_t *addr = (uint8_t *)param;
            os_printf("Connected to %02x:%02x:%02x:%02x:%02x:%02x\n",addr[5],addr[4],addr[3],addr[2],addr[1],addr[0]);
            bk_bt_gap_set_visibility(BK_BT_NON_CONNECTABLE, BK_BT_NON_DISCOVERABLE);
        }
        break;

    case BK_DM_BT_EVENT_LINKKEY_NOTIFY:
    {
        bk_bt_linkkey_storage_t *linkkey = (typeof(linkkey))param;

        os_printf("%s recv linkkey %02X:%02X:%02X:%02X:%02X:%02X\n", __func__,
                  linkkey->addr[5],
                  linkkey->addr[4],
                  linkkey->addr[3],
                  linkkey->addr[2],
                  linkkey->addr[1],
                  linkkey->addr[0]);

        memcpy(&s_bt_linkkey, linkkey, sizeof(s_bt_linkkey));

    }
    break;

    case BK_DM_BT_EVENT_LINKKEY_REQ:
    {
        uint8_t *addr = (typeof(addr))param;

        if (!memcmp(addr, s_bt_linkkey.addr, sizeof(s_bt_linkkey.addr)))
        {
            os_printf("%s found linkkey %02X:%02X:%02X:%02X:%02X:%02X\n", __func__,
                      addr[5],
                      addr[4],
                      addr[3],
                      addr[2],
                      addr[1],
                      addr[0]);

            bk_bt_gap_linkkey_reply(1, &s_bt_linkkey);
        }
        else
        {
            bk_bt_linkkey_storage_t tmp;

            os_printf("%s notfound linkkey %02X:%02X:%02X:%02X:%02X:%02X\n", __func__,
                      addr[5],
                      addr[4],
                      addr[3],
                      addr[2],
                      addr[1],
                      addr[0]);

            memset(&tmp, 0, sizeof(tmp));
            memcpy(tmp.addr, addr, sizeof(tmp.addr));

            bk_bt_gap_linkkey_reply(0, &tmp);
        }

    }
    break;

    default:
        break;
    }

    return 0;
}

int a2dp_sink_demo_init(uint8_t aac_supported)
{
    os_printf("%s\r\n", __func__);

    bk_bt_gap_set_event_callback(gap_event_cb);
    bk_bt_gap_set_device_class(COD_SOUNDBAR);

    uint8_t bt_mac[6];
    char local_name[30] = {0};
    bk_get_mac((uint8_t *)bt_mac, MAC_TYPE_BLUETOOTH);
    snprintf(local_name, 30, "%s_%02x%02x%02x", LOCAL_NAME, bt_mac[3], bt_mac[4], bt_mac[5]);
    bk_bt_gap_set_local_name((uint8_t *)local_name, os_strlen(local_name));

    bk_bt_gap_set_visibility(BK_BT_CONNECTABLE, BK_BT_DISCOVERABLE);

    bt_audio_sink_demo_task_init();

    bk_bt_a2dp_sink_init(aac_supported);
    bk_bt_a2dp_register_callback(bk_bt_app_a2dp_sink_cb);
    bk_bt_a2dp_sink_register_data_callback(&bt_audio_sink_media_data_ind);

    return 0;
}


