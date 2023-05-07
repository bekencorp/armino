#include <components/system.h>
#include <os/os.h>
#include <os/mem.h>
#include <os/str.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "hfp_hf_demo.h"

#include "modules/dm_bt.h"

#include <driver/sbc_types.h>
#include <driver/aud_types.h>
#include <driver/aud.h>
#include <components/aud_intf.h>
#include <driver/sbc.h>
#include "modules/ble_types.h"
#include "bt_include.h"
#include "bt_sbc_common.h"
#include "bt_sbc_encoder.h"


#define CODEC_VOICE_CVSD                     0x00U
#define CODEC_VOICE_MSBC                     0x01U
#define BT_AUDIO_HF_DEMO_MSG_COUNT          (30)
#define SCO_MSBC_SAMPLES_PER_FRAME      120
#define SCO_CVSD_SAMPLES_PER_FRAME      60

#define LOCAL_NAME "soundbar"

enum
{
    BT_AUDIO_MSG_NULL = 0,
    BT_AUDIO_VOICE_START_MSG = 1,
    BT_AUDIO_VOICE_STOP_MSG = 2,
    BT_AUDIO_VOICE_IND_MSG = 3,
};


typedef struct
{
    uint8_t type;
    uint16_t len;
    char *data;
} bt_audio_hf_demo_msg_t;


static bk_bt_linkkey_storage_t s_bt_linkkey;
static bt_hfp_unit_cb_t bt_audio_hfp_unit_cb;
static uint8_t bt_audio_hfp_hf_codec = CODEC_VOICE_CVSD;

static sbcdecodercontext_t bt_audio_hf_sbc_decoder;
static SbcEncoderContext bt_audio_hf_sbc_encoder;
static beken_queue_t bt_audio_hf_demo_msg_que = NULL;
static beken_thread_t bt_audio_hf_demo_thread_handle = NULL;

static uint8_t hf_mic_sco_data[1000] = {0};
static uint16_t hf_mic_data_count = 0;


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

static int bt_audio_hf_mic_data_handler(uint8_t *data, unsigned int len)
{
    uint8_t sent_threshold = ((CODEC_VOICE_MSBC == bt_audio_hfp_hf_codec) ? SCO_MSBC_SAMPLES_PER_FRAME*2 : SCO_CVSD_SAMPLES_PER_FRAME*2);

    //os_printf("[send_mic_data_to_air]  %d \r\n",len);
    os_memcpy(&hf_mic_sco_data[hf_mic_data_count], data, len);
    hf_mic_data_count += len;

    while (hf_mic_data_count >= sent_threshold)
    {
        if (CODEC_VOICE_MSBC == bt_audio_hfp_hf_codec)
        {
            int32_t produced = sbc_encoder_encode(&bt_audio_hf_sbc_encoder, (int16_t*)(&hf_mic_sco_data));
            //os_printf("[send_mic_data_to_air_msbc]  %d \r\n",produced);
            bk_bt_voice_out_write((uint8_t*)&bt_audio_hf_sbc_encoder.stream[-2], produced+2);
            //bk_bt_voice_out_write(hf_mic_sco_data, MSBC_FRAME_BUFFER_MAX_SIZE);
        }
        else
        {
            bk_bt_voice_out_write(hf_mic_sco_data, sent_threshold);
        }

        hf_mic_data_count -= sent_threshold;
        os_memmove(hf_mic_sco_data, &hf_mic_sco_data[sent_threshold], hf_mic_data_count);
    }

    return len;
}

void bt_audio_hfp_unit_voice_data_ind(uint8_t *data, uint16_t data_len)
{
    bt_audio_hf_demo_msg_t demo_msg;
    int rc = -1;

    os_memset(&demo_msg, 0x0, sizeof(bt_audio_hf_demo_msg_t));
    if (bt_audio_hf_demo_msg_que == NULL)
        return;

    demo_msg.data = (char *) os_malloc(data_len);
    if (demo_msg.data == NULL)
    {
        os_printf("%s, malloc failed\r\n", __func__);
        return;
    }

    os_memcpy(demo_msg.data, data, data_len);
    demo_msg.type = BT_AUDIO_VOICE_IND_MSG;
    demo_msg.len = data_len;

    rc = rtos_push_to_queue(&bt_audio_hf_demo_msg_que, &demo_msg, BEKEN_NO_WAIT);
    if (kNoErr != rc)
    {
        os_printf("%s, send queue failed\r\n",__func__);
        if (demo_msg.data)
        {
            os_free(demo_msg.data);
        }
    }
}

static void bt_audio_hf_sco_connected(void)
{
    bt_audio_hf_demo_msg_t demo_msg;
    int rc = -1;

    os_memset(&demo_msg, 0x0, sizeof(bt_audio_hf_demo_msg_t));
    if (bt_audio_hf_demo_msg_que == NULL)
        return;

    demo_msg.type = BT_AUDIO_VOICE_START_MSG;
    demo_msg.len = 0;

    rc = rtos_push_to_queue(&bt_audio_hf_demo_msg_que, &demo_msg, BEKEN_NO_WAIT);
    if (kNoErr != rc)
    {
        os_printf("%s, send queue failed\r\n",__func__);
    }
}

static void bt_audio_hf_sco_disconnected(void)
{
    bt_audio_hf_demo_msg_t demo_msg;
    int rc = -1;

    os_memset(&demo_msg, 0x0, sizeof(bt_audio_hf_demo_msg_t));
    if (bt_audio_hf_demo_msg_que == NULL)
        return;

    demo_msg.type = BT_AUDIO_VOICE_STOP_MSG;
    demo_msg.len = 0;

    rc = rtos_push_to_queue(&bt_audio_hf_demo_msg_que, &demo_msg, BEKEN_NO_WAIT);
    if (kNoErr != rc)
    {
        os_printf("%s, send queue failed\r\n",__func__);
    }
}

void bt_audio_hfp_unit_codec_ind(uint8_t codec_id)
{
    os_printf("%s, codec_id %d \r\n", __func__, codec_id);
    bt_audio_hfp_hf_codec = codec_id;
}

void bt_audio_hf_demo_main(void *arg)
{
    while (1) {
        bk_err_t err;
        bt_audio_hf_demo_msg_t msg;

        err = rtos_pop_from_queue(&bt_audio_hf_demo_msg_que, &msg, BEKEN_WAIT_FOREVER);
        if (kNoErr == err)
        {
            switch (msg.type) {
                case BT_AUDIO_VOICE_START_MSG:
                    {
                        bk_err_t ret = BK_OK;

                        os_printf("BT_AUDIO_VOICE_START_MSG \r\n");

                        aud_intf_drv_setup_t aud_intf_drv_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
                        aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;
                        aud_intf_voc_setup_t aud_voc_setup = DEFAULT_AUD_INTF_VOC_SETUP_CONFIG();

                        if (CODEC_VOICE_MSBC == bt_audio_hfp_hf_codec)
                        {
                            bk_sbc_decoder_init(&bt_audio_hf_sbc_decoder);
                            sbc_encoder_init(&bt_audio_hf_sbc_encoder, 16000, 1);
                            sbc_encoder_ctrl(&bt_audio_hf_sbc_encoder, SBC_ENCODER_CTRL_CMD_SET_MSBC_ENCODE_MODE, (uint32_t)NULL);
                        }

                        aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
                        aud_intf_drv_setup.task_config.priority = 3;
                        aud_intf_drv_setup.aud_intf_rx_spk_data = NULL;
                        aud_intf_drv_setup.aud_intf_tx_mic_data = bt_audio_hf_mic_data_handler;
                        ret = bk_aud_intf_drv_init(&aud_intf_drv_setup);
                        if (ret != BK_ERR_AUD_INTF_OK) {
                            os_printf("bk_aud_intf_drv_init fail, ret:%d \r\n", ret);
                        } else {
                            os_printf("bk_aud_intf_drv_init complete \r\n");
                        }

                        aud_work_mode = AUD_INTF_WORK_MODE_VOICE;
                        ret = bk_aud_intf_set_mode(aud_work_mode);
                        if (ret != BK_ERR_AUD_INTF_OK) {
                            os_printf("bk_aud_intf_set_mode fail, ret:%d \r\n", ret);
                        } else {
                            os_printf("bk_aud_intf_set_mode complete \r\n");
                        }

                        aud_voc_setup.aec_enable = true;
                        aud_voc_setup.samp_rate = ((CODEC_VOICE_MSBC == bt_audio_hfp_hf_codec) ? AUD_INTF_VOC_SAMP_RATE_16K : AUD_INTF_VOC_SAMP_RATE_8K);
                        aud_voc_setup.data_type = AUD_INTF_VOC_DATA_TYPE_PCM;
                        aud_voc_setup.spk_mode = AUD_DAC_WORK_MODE_SIGNAL_END;//AUD_DAC_WORK_MODE_DIFFEN;

                        aud_voc_setup.mic_gain = 0x2d;
                        aud_voc_setup.spk_gain = 0x2d;
                        aud_voc_setup.aec_cfg.ec_depth = 20;
                        aud_voc_setup.aec_cfg.TxRxThr = 30;
                        aud_voc_setup.aec_cfg.TxRxFlr = 6;
                        aud_voc_setup.aec_cfg.ns_level = 2;
                        aud_voc_setup.aec_cfg.ns_para = 1;

                        ret = bk_aud_intf_voc_init(aud_voc_setup);
                        if (ret != BK_ERR_AUD_INTF_OK)
                        {
                            os_printf("bk_aud_intf_voc_init fail, ret:%d\n", ret);
                            break;
                        }

                        ret = bk_aud_intf_voc_start();
                        if (ret != BK_ERR_AUD_INTF_OK)
                        {
                            os_printf("bk_aud_intf_voc_start fail, ret:%d\n", ret);
                            break;
                        }

                        os_printf("hfp audio init ok\r\n");
                    }
                    break;

                case BT_AUDIO_VOICE_STOP_MSG:
                    {
                        os_printf("BT_AUDIO_VOICE_STOP_MSG \r\n");
                        bk_aud_intf_voc_deinit();
                        bk_aud_intf_drv_deinit();
                    }
                    break;

                case BT_AUDIO_VOICE_IND_MSG:
                    {
                        bk_err_t ret = BK_OK;
                        uint8 *fb = (uint8_t*)msg.data;

                        //os_printf("len %d, %x %x %x\r\n",msg.len,fb[0], fb[1], fb[2]);

                        if (CODEC_VOICE_MSBC == bt_audio_hfp_hf_codec)
                        {
                            fb += 2; //Skip Synchronization Header
                            ret = bk_sbc_decoder_frame_decode(&bt_audio_hf_sbc_decoder, fb, msg.len -2);
                            if (ret < 0)
                            {
                                os_printf("msbc decode fail, ret:%d\n", ret);
                            }
                            else
                            {
                                ret = bk_aud_intf_write_spk_data((uint8_t*)bt_audio_hf_sbc_decoder.pcm_sample, SCO_MSBC_SAMPLES_PER_FRAME*2);
                                if (ret != BK_OK) {
                                    os_printf("write spk data fail \r\n");
                                }
                            }
                        }
                        else
                        {
                            ret = bk_aud_intf_write_spk_data((uint8_t*)fb, SCO_CVSD_SAMPLES_PER_FRAME*2);
                            if (ret != BK_OK) {
                                os_printf("write spk data fail \r\n");
                            }
                        }

                        os_free(msg.data);
                    }
                    break;

                default:
                    break;
            }
        }
    }

    rtos_deinit_queue(&bt_audio_hf_demo_msg_que);
    bt_audio_hf_demo_msg_que = NULL;
    bt_audio_hf_demo_thread_handle = NULL;
    rtos_delete_thread(NULL);
}

int bt_audio_hf_demo_task_init(void)
{
    bk_err_t ret = BK_OK;
    if ((!bt_audio_hf_demo_thread_handle) && (!bt_audio_hf_demo_msg_que))
    {
        ret = rtos_init_queue(&bt_audio_hf_demo_msg_que,
                              "bt_audio_hf_demo_msg_que",
                              sizeof(bt_audio_hf_demo_msg_t),
                              BT_AUDIO_HF_DEMO_MSG_COUNT);
        if (ret != kNoErr) {
            os_printf("bt_audio hf demo msg queue failed \r\n");
            return BK_FAIL;
        }

        ret = rtos_create_thread(&bt_audio_hf_demo_thread_handle,
                             BEKEN_DEFAULT_WORKER_PRIORITY,
                             "bt_audio_hf_demo",
                             (beken_thread_function_t)bt_audio_hf_demo_main,
                             4096,
                             (beken_thread_arg_t)0);
        if (ret != kNoErr) {
            os_printf("bt_audio hf demo task fail \r\n");
            rtos_deinit_queue(&bt_audio_hf_demo_msg_que);
            bt_audio_hf_demo_msg_que = NULL;
            bt_audio_hf_demo_thread_handle = NULL;
        }

        return kNoErr;
    }
    else
    {
        return kInProgressErr;
    }
}

int hfp_hf_demo_init(uint8_t msbc_supported)
{
    int ret = kNoErr;

    os_printf("%s\r\n", __func__);

    bk_bt_gap_set_event_callback(gap_event_cb);
    bk_bt_gap_set_device_class(COD_SOUNDBAR);

    uint8_t bt_mac[6];
    char local_name[30] = {0};
    bk_get_mac((uint8_t *)bt_mac, MAC_TYPE_BLUETOOTH);
    snprintf(local_name, 30, "%s_%02x%02x%02x", LOCAL_NAME, bt_mac[3], bt_mac[4], bt_mac[5]);
    bk_bt_gap_set_local_name((uint8_t *)local_name, os_strlen(local_name));

    bk_bt_gap_set_visibility(BK_BT_CONNECTABLE, BK_BT_DISCOVERABLE);

    bt_audio_hf_demo_task_init();

    bt_audio_hfp_unit_cb.sco_connected = bt_audio_hf_sco_connected;
    bt_audio_hfp_unit_cb.sco_disconnected = bt_audio_hf_sco_disconnected;
    bt_audio_hfp_unit_cb.codec_ind = bt_audio_hfp_unit_codec_ind;
    bt_audio_hfp_unit_cb.voice_data_ind = bt_audio_hfp_unit_voice_data_ind;
    ret = bk_bt_hfp_unit_init(msbc_supported, &bt_audio_hfp_unit_cb);

    return ret;
}


