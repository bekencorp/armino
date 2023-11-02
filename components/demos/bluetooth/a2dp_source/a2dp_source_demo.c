#include <stdio.h>
#include <stdlib.h>
#include "os/os.h"
#include "os/mem.h"
#include "os/str.h"
#include <components/log.h>
#include "diskio.h"
#include "modules/bt.h"
#include "modules/bt_types.h"
#include "modules/ble_types.h"
#include "modules/mp3dec.h"
#include "modules/audio_rsp_types.h"
#include "modules/audio_rsp.h"

#include "bt_include.h"
#include "bt_sbc_common.h"
#include "bt_sbc_encoder.h"

#include "modules/dm_bt.h"

#include <driver/sbc_types.h>
#include <driver/aud_types.h>
#include <driver/aud.h>
#include <components/aud_intf.h>
#include "common/bk_assert.h"
#include <driver/sbc.h>
#include "ff.h"
#include "ring_buffer_particle.h"
//#include "test_fatfs.h"

typedef struct
{
    uint8_t inited;
    uint16_t conn_handle;
    bd_addr_t peer_addr;
    uint8_t conn_state;
    uint8_t start_status;
    uint8_t play_status;
    uint32_t mtu;
} a2dp_env_s;


typedef struct
{
    uint8_t type;
    uint16_t len;
    uint8_t *data;
} bt_a2dp_source_msg_t;


enum
{
    BT_A2DP_SOURCE_MSG_NONE,
    BT_A2DP_SOURCE_MSG_READ_PCM_FROM_FILE,
    BT_A2DP_SOURCE_MSG_READ_PCM_FROM_BUFF,
    BT_A2DP_SOURCE_MSG_STOP,
};

enum
{
    A2DP_SOURCE_DEBUG_LEVEL_ERROR,
    A2DP_SOURCE_DEBUG_LEVEL_WARNING,
    A2DP_SOURCE_DEBUG_LEVEL_INFO,
    A2DP_SOURCE_DEBUG_LEVEL_DEBUG,
    A2DP_SOURCE_DEBUG_LEVEL_VERBOSE,
};

#define MP3_DECODE_BUFF_SIZE (MAX_NSAMP * MAX_NCHAN * MAX_NGRAN * sizeof(uint16_t))
#define DECODE_TRIGGER_TIME (0.1) //sec
#define DECODE_RB_SIZE ((s_decode_trigger_size > MP3_DECODE_BUFF_SIZE ? s_decode_trigger_size : MP3_DECODE_BUFF_SIZE) + MP3_DECODE_BUFF_SIZE + 1)

#define CONNECTION_PACKET_TYPE 0xcc18
#define CONNECTION_PAGE_SCAN_REPETITIOIN_MODE 0x01
#define CONNECTION_CLOCK_OFFSET 0x00
#define A2DP_SOURCE_WRITE_AUTO_TIMER_MS 30

#define A2DP_SOURCE_DEBUG_LEVEL A2DP_SOURCE_DEBUG_LEVEL_INFO

#define SBC_SAMPLE_DEPTH 16

#define a2dp_loge(format, ...) do{if(A2DP_SOURCE_DEBUG_LEVEL >= A2DP_SOURCE_DEBUG_LEVEL_ERROR)   BK_LOGE("a2dp_s", "%s:" format "\n", __func__, ##__VA_ARGS__);}while(0)
#define a2dp_logw(format, ...) do{if(A2DP_SOURCE_DEBUG_LEVEL >= A2DP_SOURCE_DEBUG_LEVEL_WARNING) BK_LOGW("a2dp_s", "%s:" format "\n", __func__, ##__VA_ARGS__);}while(0)
#define a2dp_logi(format, ...) do{if(A2DP_SOURCE_DEBUG_LEVEL >= A2DP_SOURCE_DEBUG_LEVEL_INFO)    BK_LOGI("a2dp_s", "%s:" format "\n", __func__, ##__VA_ARGS__);}while(0)
#define a2dp_logd(format, ...) do{if(A2DP_SOURCE_DEBUG_LEVEL >= A2DP_SOURCE_DEBUG_LEVEL_DEBUG)   BK_LOGI("a2dp_s", "%s:" format "\n", __func__, ##__VA_ARGS__);}while(0)
#define a2dp_logv(format, ...) do{if(A2DP_SOURCE_DEBUG_LEVEL >= A2DP_SOURCE_DEBUG_LEVEL_VERBOSE) BK_LOGI("a2dp_s", "%s:" format "\n", __func__, ##__VA_ARGS__);}while(0)

static uint32_t s_msg_type;
static ble_err_t at_cmd_status = BK_ERR_BLE_SUCCESS;
static bk_bt_linkkey_storage_t s_bt_linkkey;
static a2dp_env_s a2dp_env;
static union codec_info s_a2dp_cap_info;
static beken_timer_t bt_a2dp_source_write_timer;
static beken_semaphore_t s_bt_api_sema = NULL;
static beken_semaphore_t s_source_timer_sema = NULL;
static beken_semaphore_t s_source_need_decode_sema = NULL;
static uint16_t conn_handle = 0xff;
static beken_queue_t bt_a2dp_source_msg_que = NULL;
static beken_thread_t bt_a2dp_source_thread_handle = NULL;
static beken_thread_t bt_a2dp_source_decode_thread_handle = NULL;


static ring_buffer_particle_ctx s_rb_ctx;
static SbcEncoderContext s_sbc_software_encoder_ctx;
static beken_time_t s_last_get_sample_time = 0;
static uint32_t s_remain_send_sample_count = 0;
static uint32_t s_remain_send_sample_miss_num = 0;
static FIL pcm_file_fd;
static FIL mp3file;
static uint8_t s_is_mp3;
static uint8_t s_file_path[64] = {0};
static FATFS *s_pfs = NULL;
static HMP3Decoder s_mp3_decoder = NULL;
static MP3FrameInfo s_mp3_frame_info = {0};
static uint32_t s_decode_trigger_size; //44.1khz buffer size = 176400 * DECODE_TRIGGER_TIME Bytes
static uint8_t s_decode_task_run;
static uint8_t s_pcm_covert_missing_buff[48 * 2];
static uint8_t s_pcm_covert_missing_len;
static uint8_t s_is_bk_aud_rsp_inited;

static void user_a2dp_connection_change(uint8_t status, uint8_t reason)
{
    a2dp_logi("%d %d", status, reason);

    if (a2dp_env.conn_state != status)
    {
        a2dp_env.conn_state = status;

        if (status == BK_A2DP_CONNECTION_STATE_CONNECTED || status == BK_A2DP_CONNECTION_STATE_DISCONNECTED)
        {
            if (s_bt_api_sema != NULL)
            {
                rtos_set_semaphore( &s_bt_api_sema );
            }
        }
    }
}

static void user_a2dp_start_cnf(uint8_t result, uint8_t reason, uint32_t mtu)
{
    a2dp_logi("%d %d", result, reason);

    if (result == 0 && a2dp_env.start_status == 0 )
    {
        a2dp_env.start_status = 1;
        a2dp_env.mtu = mtu;

        if (s_bt_api_sema != NULL)
        {
            rtos_set_semaphore( &s_bt_api_sema );
        }
    }
}

static void user_a2dp_suspend_cnf(uint8_t result, uint8_t reason)
{
    a2dp_logi("%d %d", result, reason);

    if (result == 0 && a2dp_env.start_status == 1 )
    {
        a2dp_env.start_status = 0;

        if (s_bt_api_sema != NULL)
        {
            rtos_set_semaphore( &s_bt_api_sema );
        }
    }
}

static void user_a2dp_final_cap_select(union codec_info *info)
{
    os_memcpy(&s_a2dp_cap_info, info, sizeof(*info));

    a2dp_logi("channel %d srate %d", s_a2dp_cap_info.sbc_codec.channels, s_a2dp_cap_info.sbc_codec.sample_rate);
}

const static bt_a2dp_source_cb_t bt_a2dp_source_cb =
{
    .a2dp_connection_change = user_a2dp_connection_change,
    .a2dp_start_cnf = user_a2dp_start_cnf,
    .a2dp_suspend_cnf = user_a2dp_suspend_cnf,
    .a2dp_final_cap_select = user_a2dp_final_cap_select,
};


static uint32_t bt_at_event_cb(bt_event_enum_t event, void *param)
{
    switch (event)
    {
    case BK_DM_BT_EVENT_DISCONNECT:
    {
        uint8_t *addr = (uint8_t *)param;
        a2dp_logi("Disconnected from %x %x %x %x %x %x ", addr[5], addr[4], addr[3], addr[2], addr[1], addr[0]);
    }
    break;

    case BK_DM_BT_EVENT_CONNECTION_COMPLETE:
    {
        uint8_t *addr = (uint8_t *)param;
        a2dp_logi("Connected to %02x:%02x:%02x:%02x:%02x:%02x", addr[5], addr[4], addr[3], addr[2], addr[1], addr[0]);
    }
    break;

    case BK_DM_BT_EVENT_LINKKEY_NOTIFY:
    {
        bk_bt_linkkey_storage_t *linkkey = (typeof(linkkey))param;

        a2dp_logi("recv linkkey %02X:%02X:%02X:%02X:%02X:%02X",
                  linkkey->addr[5],
                  linkkey->addr[4],
                  linkkey->addr[3],
                  linkkey->addr[2],
                  linkkey->addr[1],
                  linkkey->addr[0]);

        os_memcpy(&s_bt_linkkey, linkkey, sizeof(s_bt_linkkey));
    }
    break;

    case BK_DM_BT_EVENT_LINKKEY_REQ:
    {
        uint8_t *addr = (typeof(addr))param;

        if (!os_memcmp(addr, s_bt_linkkey.addr, sizeof(s_bt_linkkey.addr)))
        {
            a2dp_logi("found linkkey %02X:%02X:%02X:%02X:%02X:%02X",
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

            a2dp_logi("notfound linkkey %02X:%02X:%02X:%02X:%02X:%02X",
                      addr[5],
                      addr[4],
                      addr[3],
                      addr[2],
                      addr[1],
                      addr[0]);

            os_memset(&tmp, 0, sizeof(tmp));
            os_memcpy(tmp.addr, addr, sizeof(tmp.addr));

            bk_bt_gap_linkkey_reply(0, &tmp);
        }

    }
    break;

    default:
        break;
    }

    return 0;
}

static void bt_at_cmd_cb(bt_cmd_t cmd, bt_cmd_param_t *param)
{
    at_cmd_status = param->status;

    //    a2dp_logi("%d %d", cmd, param->status);

    switch (cmd)
    {
    case BT_CMD_CREATE_CONNECT:
        if (param->status == BK_ERR_BT_SUCCESS)
        {
            conn_handle = *((uint16_t *)(param->param));
        }

        break;

    case BT_CMD_DISCONNECT:
        conn_handle = 0xff;
        break;

    case BT_CMD_SDP:
        break;

    case BT_CMD_READ_SCAN_ENABLE:
    {
        uint8_t scan_enable = 0xFF;

        if (param->status == BK_ERR_BT_SUCCESS)
        {
            scan_enable = *(uint8_t *)(param->param + 4);
        }

        a2dp_logi("Read scan enable, status:%d, Scan Enable: 0x%x ", param->status, scan_enable);
    }
    break;

    case BT_CMD_WRITE_SCAN_ENABLE:
        a2dp_logi("Write scan enable, status:%d ", param->status);
        break;

    default:
        break;
    }

    if (s_bt_api_sema != NULL)
    {
        rtos_set_semaphore(&s_bt_api_sema);
    }
}

static void a2dp_set_get_sample_clean(void)
{
    s_last_get_sample_time = 0;
    s_remain_send_sample_count = 0;
    s_remain_send_sample_miss_num = 0;
    s_pcm_covert_missing_len = 0;
}

static void a2dp_source_read_pcm_from_file_hdl(FIL *fd)
{
    unsigned int read_len = 0;
    static uint32_t read_len_all = 0;
    FIL *pcm_file_fd = fd;
    FRESULT fr;
    int32_t encode_len = 0;
    bt_err_t ret = 0;
    uint8_t *encode_tmp_buff = NULL;
    uint8_t *read_pcm_tmp_buff_alloc = NULL;
    uint32_t encode_index = 0;

    beken_time_t cur_time = rtos_get_time();
    beken_time_t will_send_period_time = 0;


    uint32_t will_send_frame_count = 0;


    //s_sbc_software_encoder_ctx.frame.blocks * s_sbc_software_encoder_ctx.frame.subbands == s_sbc_software_encoder_ctx.pcm_length;
    //pcm_length == 128 when 16 block 8 subband
    uint32_t one_pcm_frame_encode_sample_count_per_channel = s_sbc_software_encoder_ctx.pcm_length;
    uint32_t one_pcm_frame_encode_sample_bytes = one_pcm_frame_encode_sample_count_per_channel * 2 * s_sbc_software_encoder_ctx.num_channels;
    const uint32_t send_sbc_frame_count = 2;// A2DP_SOURCE_SBC_FRAME_COUNT;
    const uint32_t ignore_max_mtu = 0;

    uint32_t i = 0, j = 0;

    if (s_last_get_sample_time == 0)
    {
        will_send_period_time = A2DP_SOURCE_WRITE_AUTO_TIMER_MS;//first
    }
    else
    {
        will_send_period_time = cur_time - s_last_get_sample_time;
    }

    s_remain_send_sample_count += will_send_period_time * s_sbc_software_encoder_ctx.sample_rate / 1000;
    s_remain_send_sample_miss_num += will_send_period_time * s_sbc_software_encoder_ctx.sample_rate % 1000;

    s_remain_send_sample_count += s_remain_send_sample_miss_num / 1000;
    s_remain_send_sample_miss_num = s_remain_send_sample_miss_num % 1000;

    s_last_get_sample_time = cur_time;

    if (!s_remain_send_sample_count)
    {
        return;
    }
    else if (s_remain_send_sample_count < one_pcm_frame_encode_sample_count_per_channel)
    {
        if (will_send_period_time * s_sbc_software_encoder_ctx.sample_rate / 1000 <
                send_sbc_frame_count * one_pcm_frame_encode_sample_count_per_channel)
        {
            return;
        }
    }
    else if (s_remain_send_sample_count >= one_pcm_frame_encode_sample_count_per_channel &&
             s_remain_send_sample_count < one_pcm_frame_encode_sample_count_per_channel * send_sbc_frame_count)
    {
        if (0)//will_send_period_time * s_sbc_software_encoder_ctx.sample_rate / 1000 < send_sbc_frame_count * one_pcm_frame_encode_sample_count_per_channel)
        {
            return;
        }
    }


    will_send_frame_count = ((s_remain_send_sample_count / one_pcm_frame_encode_sample_count_per_channel > 1) ?
                             (s_remain_send_sample_count / one_pcm_frame_encode_sample_count_per_channel) : 1);

    //    if(will_send_frame_count > A2DP_SOURCE_SBC_FRAME_COUNT)
    //    {
    //        will_send_frame_count = A2DP_SOURCE_SBC_FRAME_COUNT;
    //    }

    if (will_send_frame_count > 100 || !will_send_frame_count)
    {
        a2dp_loge("will_send_frame_count %d", will_send_frame_count);
    }

    //printf("%s will_send_frame_count %d ", will_send_frame_count);

    encode_tmp_buff = os_malloc(will_send_frame_count * 128); //max 128

    if (encode_tmp_buff == NULL)
    {
        a2dp_logi("encode_tmp_buff no buffer malloc");
        a2dp_set_get_sample_clean();
        goto error;
    }

    read_pcm_tmp_buff_alloc = os_malloc(one_pcm_frame_encode_sample_bytes);

    if (read_pcm_tmp_buff_alloc == NULL)
    {
        a2dp_loge("read_pcm_tmp_buff_alloc alloc err");
        a2dp_set_get_sample_clean();
        goto error;
    }

    do
    {
        for (i = 0; ( ignore_max_mtu || encode_index + encode_len <= a2dp_env.mtu) && i + j < will_send_frame_count; ++i)
        {
            fr = f_read(pcm_file_fd, (void *)(read_pcm_tmp_buff_alloc), one_pcm_frame_encode_sample_bytes, &read_len);
            //            read_len = fread((read_pcm_tmp_buff), 1, one_pcm_frame_encode_sample_bytes, pcm_file_fd);

            if (fr != FR_OK)
            {
                a2dp_logi("read fail %d", fr);
                goto error;
            }

            read_len_all += read_len;

            if (read_len < one_pcm_frame_encode_sample_bytes)
            {
                a2dp_logi("read len %d < %d!!", read_len, one_pcm_frame_encode_sample_bytes);

                if (read_len == 0)
                {
                    a2dp_logi("read file end %d %d !!", read_len, one_pcm_frame_encode_sample_bytes);

                    rtos_stop_timer(&bt_a2dp_source_write_timer);
                    a2dp_set_get_sample_clean();

                    if (1) //repeat
                    {
                        fr = f_lseek(pcm_file_fd, 0);

                        if (fr != FR_OK)
                        {
                            a2dp_logi("f_lseek fail.");
                            f_close(pcm_file_fd);
                            os_memset(pcm_file_fd, 0, sizeof(*pcm_file_fd));
                            goto error;
                        }

                        rtos_start_timer(&bt_a2dp_source_write_timer);
                    }
                    else
                    {
                        f_close(pcm_file_fd);
                        os_memset(pcm_file_fd, 0, sizeof(*pcm_file_fd));
                    }

                    goto error;
                }

                os_memset(read_pcm_tmp_buff_alloc + read_len, 0, one_pcm_frame_encode_sample_bytes - read_len);
            }

            encode_len = sbc_encoder_encode(&s_sbc_software_encoder_ctx, (const int16_t *)(read_pcm_tmp_buff_alloc));

            if (encode_len < 0)
            {
                a2dp_logi("encode err %d", encode_len);
                goto error;
            }

            if (encode_index + encode_len > will_send_frame_count * 128)
            {
                a2dp_logi("encode data large than malloc !! %d", encode_len);
                goto error;
            }

            os_memcpy(encode_tmp_buff + encode_index, s_sbc_software_encoder_ctx.stream, encode_len);
            encode_index += encode_len;
        }

        ret = bk_bt_a2dp_source_write(&(a2dp_env.peer_addr), i, encode_tmp_buff, encode_index);

        if (encode_index > 10000 || i > 5)
        {
            a2dp_logi("encode_index %d i %d", encode_index, i);
        }

        if (ret)
        {
            a2dp_logi("bk_bt_a2dp_source_write err %d", ret);
        }

        encode_index = 0;
        j += i;
    }
    while (j < will_send_frame_count);

    if (s_remain_send_sample_count < j * one_pcm_frame_encode_sample_count_per_channel)
    {
        s_remain_send_sample_count = 0;
    }
    else
    {
        s_remain_send_sample_count -= j * one_pcm_frame_encode_sample_count_per_channel;
    }

error:;

    if (encode_tmp_buff)
    {
        os_free(encode_tmp_buff);
        encode_tmp_buff = NULL;
    }

    if (read_pcm_tmp_buff_alloc)
    {
        os_free(read_pcm_tmp_buff_alloc);
        read_pcm_tmp_buff_alloc = NULL;
    }
}

static void a2dp_source_read_pcm_from_buffer(uint8_t *arg)
{
    uint32_t read_len = 0;
    int32_t encode_len = 0;
    bt_err_t ret = 0;
    uint8_t *encode_tmp_buff = NULL;
    uint8_t *read_pcm_tmp_buff_alloc = NULL;
    uint8_t *read_pcm_tmp_buff_covert_alloc = NULL;
    uint32_t encode_index = 0;

    beken_time_t cur_time = rtos_get_time();
    beken_time_t will_send_period_time = 0;

    uint32_t will_send_frame_count = 0;

    //s_sbc_software_encoder_ctx.frame.blocks * s_sbc_software_encoder_ctx.frame.subbands == s_sbc_software_encoder_ctx.pcm_length;
    //pcm_length == 128 when 16 block 8 subband
    uint32_t one_pcm_frame_encode_sample_count_per_channel = s_sbc_software_encoder_ctx.pcm_length;
    uint32_t one_pcm_frame_encode_sample_bytes = one_pcm_frame_encode_sample_count_per_channel * s_sbc_software_encoder_ctx.num_channels * sizeof(uint16_t);
    uint32_t one_pcm_frame_encode_sample_bytes_bk = one_pcm_frame_encode_sample_bytes;

    const uint32_t send_sbc_frame_count = 2;// A2DP_SOURCE_SBC_FRAME_COUNT;
    const uint32_t ignore_max_mtu = 0;

    uint32_t i = 0, j = 0;

    if (s_last_get_sample_time == 0)
    {
        will_send_period_time = A2DP_SOURCE_WRITE_AUTO_TIMER_MS;//first
    }
    else
    {
        will_send_period_time = cur_time - s_last_get_sample_time;
    }

    s_remain_send_sample_count += will_send_period_time * s_sbc_software_encoder_ctx.sample_rate / 1000;
    s_remain_send_sample_miss_num += will_send_period_time * s_sbc_software_encoder_ctx.sample_rate % 1000;

    s_remain_send_sample_count += s_remain_send_sample_miss_num / 1000;
    s_remain_send_sample_miss_num = s_remain_send_sample_miss_num % 1000;

    s_last_get_sample_time = cur_time;

    if (!s_remain_send_sample_count)
    {
        return;
    }
    else if (s_remain_send_sample_count < one_pcm_frame_encode_sample_count_per_channel)
    {
        if (will_send_period_time * s_sbc_software_encoder_ctx.sample_rate / 1000 <
                send_sbc_frame_count * one_pcm_frame_encode_sample_count_per_channel)
        {
            return;
        }
    }
    else if (s_remain_send_sample_count >= one_pcm_frame_encode_sample_count_per_channel &&
             s_remain_send_sample_count < one_pcm_frame_encode_sample_count_per_channel * send_sbc_frame_count)
    {
        if (0)//will_send_period_time * s_sbc_software_encoder_ctx.sample_rate / 1000 < send_sbc_frame_count * one_pcm_frame_encode_sample_count_per_channel)
        {
            return;
        }
    }


    will_send_frame_count = ((s_remain_send_sample_count / one_pcm_frame_encode_sample_count_per_channel > 1) ?
                             (s_remain_send_sample_count / one_pcm_frame_encode_sample_count_per_channel) : 1);

    //    if(will_send_frame_count > A2DP_SOURCE_SBC_FRAME_COUNT)
    //    {
    //        will_send_frame_count = A2DP_SOURCE_SBC_FRAME_COUNT;
    //    }

    if (will_send_frame_count > 100)
    {
        a2dp_logi("");
    }

    if (!will_send_frame_count)
    {
        a2dp_logi("0");
    }

    //a2dp_logi("will_send_frame_count %d ", will_send_frame_count);

    encode_tmp_buff = os_malloc(will_send_frame_count * 128); //max 128

    if (!encode_tmp_buff)
    {
        a2dp_loge("encode_tmp_buff alloc err");
        a2dp_set_get_sample_clean();
        goto error;
    }

    if (s_mp3_frame_info.samprate != s_a2dp_cap_info.sbc_codec.sample_rate || s_mp3_frame_info.nChans != s_a2dp_cap_info.sbc_codec.channels ||
            s_mp3_frame_info.bitsPerSample != SBC_SAMPLE_DEPTH)
    {
        float cacl_float = ((one_pcm_frame_encode_sample_bytes_bk - s_pcm_covert_missing_len) * 1.0 * s_mp3_frame_info.samprate / s_a2dp_cap_info.sbc_codec.sample_rate *
                            s_mp3_frame_info.nChans / s_a2dp_cap_info.sbc_codec.channels);

        one_pcm_frame_encode_sample_bytes = (typeof(one_pcm_frame_encode_sample_bytes))cacl_float;

        if (one_pcm_frame_encode_sample_bytes < cacl_float)
        {
            one_pcm_frame_encode_sample_bytes += 1;
        }

        one_pcm_frame_encode_sample_bytes = INT_CEIL(one_pcm_frame_encode_sample_bytes, s_mp3_frame_info.nChans * s_mp3_frame_info.bitsPerSample / 8);

        a2dp_logd("pcm covert %d %d %d %d %d %d", s_mp3_frame_info.samprate, s_a2dp_cap_info.sbc_codec.sample_rate, s_mp3_frame_info.nChans, s_a2dp_cap_info.sbc_codec.channels,
                  one_pcm_frame_encode_sample_bytes, one_pcm_frame_encode_sample_bytes_bk);

        read_pcm_tmp_buff_alloc = os_malloc(MAX(one_pcm_frame_encode_sample_bytes, one_pcm_frame_encode_sample_bytes_bk) * 2);

        if (read_pcm_tmp_buff_alloc == NULL)
        {
            a2dp_loge("read_pcm_tmp_buff_alloc alloc err");
            a2dp_set_get_sample_clean();
            goto error;
        }

        read_pcm_tmp_buff_covert_alloc = os_malloc(MAX(one_pcm_frame_encode_sample_bytes, one_pcm_frame_encode_sample_bytes_bk) * 2);

        if (read_pcm_tmp_buff_covert_alloc == NULL)
        {
            a2dp_loge("read_pcm_tmp_buff_covert_alloc alloc err");
            a2dp_set_get_sample_clean();
            goto error;
        }
    }
    else
    {
        read_pcm_tmp_buff_alloc = os_malloc(one_pcm_frame_encode_sample_bytes);

        if (read_pcm_tmp_buff_alloc == NULL)
        {
            a2dp_loge("read_pcm_tmp_buff_alloc alloc err");
            a2dp_set_get_sample_clean();
            goto error;
        }
    }

    do
    {
        for (i = 0; ( ignore_max_mtu || encode_index + encode_len <= a2dp_env.mtu) && i + j < will_send_frame_count; ++i)
        {
            if (ring_buffer_particle_len(&s_rb_ctx) < one_pcm_frame_encode_sample_bytes)
            {
                a2dp_loge("ring buffer not enough data %d < %d ", ring_buffer_particle_len(&s_rb_ctx), one_pcm_frame_encode_sample_bytes);
                goto error;
            }

            ring_buffer_particle_read(&s_rb_ctx, read_pcm_tmp_buff_alloc, one_pcm_frame_encode_sample_bytes, &read_len);

            //            if (read_len < one_pcm_frame_encode_sample_bytes - s_pcm_covert_missing_len)
            //            {
            //                a2dp_loge("read len %d < %d !!", read_len, one_pcm_frame_encode_sample_bytes - s_pcm_covert_missing_len);
            //                goto error;
            //            }

            if (read_pcm_tmp_buff_covert_alloc)
            {
                uint32_t k = 0;
                uint32_t covert_step_size = 0;

                //channel covert
                if (s_mp3_frame_info.nChans / s_a2dp_cap_info.sbc_codec.channels == 2)
                {
                    for (k = 0; 4 * k + 3 < one_pcm_frame_encode_sample_bytes; k++)
                    {
                        //left
                        read_pcm_tmp_buff_covert_alloc[2 * k] = read_pcm_tmp_buff_alloc[4 * k];
                        read_pcm_tmp_buff_covert_alloc[2 * k + 1] = read_pcm_tmp_buff_alloc[4 * k + 1];

                        //right
                        //read_pcm_tmp_buff_covert_alloc[2 * k] = read_pcm_tmp_buff_alloc[4 * k + 2];
                        //read_pcm_tmp_buff_covert_alloc[2 * k + 1] = read_pcm_tmp_buff_alloc[4 * k + 3];
                    }

                    os_memcpy(read_pcm_tmp_buff_alloc, read_pcm_tmp_buff_covert_alloc, k * 2);
                    covert_step_size = k * 2;
                }
                else if (s_a2dp_cap_info.sbc_codec.channels / s_mp3_frame_info.nChans == 2)
                {
                    for (k = 0; 2 * k + 1 < one_pcm_frame_encode_sample_bytes; k++)
                    {
                        read_pcm_tmp_buff_covert_alloc[4 * k] = read_pcm_tmp_buff_alloc[2 * k];
                        read_pcm_tmp_buff_covert_alloc[4 * k + 1] = read_pcm_tmp_buff_alloc[2 * k + 1];
                        read_pcm_tmp_buff_covert_alloc[4 * k + 2] = read_pcm_tmp_buff_alloc[2 * k];
                        read_pcm_tmp_buff_covert_alloc[4 * k + 3] = read_pcm_tmp_buff_alloc[2 * k + 1];
                    }

                    os_memcpy(read_pcm_tmp_buff_alloc, read_pcm_tmp_buff_covert_alloc, k * 4);
                    //os_memset(read_pcm_tmp_buff_alloc + k * 4, 0, MAX(one_pcm_frame_encode_sample_bytes, one_pcm_frame_encode_sample_bytes_bk) * 2 - k * 4);

                    covert_step_size = k * 4;
                }
                else
                {
                    covert_step_size = one_pcm_frame_encode_sample_bytes;
                }


                if (s_mp3_frame_info.samprate != s_a2dp_cap_info.sbc_codec.sample_rate || s_mp3_frame_info.bitsPerSample != SBC_SAMPLE_DEPTH)
                {
                    uint32_t rsp_in_before_bytes = covert_step_size;
                    uint32_t rsp_out_before_bytes = one_pcm_frame_encode_sample_bytes_bk + s_a2dp_cap_info.sbc_codec.channels * (SBC_SAMPLE_DEPTH / 8);
                    uint32_t rsp_in_before_count = rsp_in_before_bytes / (s_mp3_frame_info.bitsPerSample / 8);
                    uint32_t rsp_out_before_count = rsp_out_before_bytes / (SBC_SAMPLE_DEPTH / 8);

                    uint32_t rsp_in_after_count = rsp_in_before_count;
                    uint32_t rsp_out_after_count = rsp_out_before_count;

                    ret = bk_aud_rsp_process((int16_t *)read_pcm_tmp_buff_alloc, &rsp_in_after_count, (int16_t *)read_pcm_tmp_buff_covert_alloc, &rsp_out_after_count);

                    a2dp_logd("sample rate covert %d %d %d", rsp_in_before_count, rsp_out_after_count, s_pcm_covert_missing_len);

                    if (0 != ret)
                    {
                        a2dp_loge("bk_aud_rsp_process err %d !!", ret);
                        goto error;
                    }

                    if (rsp_in_before_count != rsp_in_after_count)
                    {
                        a2dp_loge("bk_aud_rsp_process input not complete process, %d %d %d %d!!",
                                  rsp_in_before_count, rsp_in_after_count, rsp_out_before_count, rsp_out_after_count);
                    }

                    covert_step_size = rsp_out_after_count * (SBC_SAMPLE_DEPTH / 8);

                    {
                        uint32_t final_copy_len = INT_FLOOR(s_pcm_covert_missing_len + MIN(one_pcm_frame_encode_sample_bytes_bk - s_pcm_covert_missing_len, covert_step_size),
                                                            s_a2dp_cap_info.sbc_codec.channels * SBC_SAMPLE_DEPTH / 8);

                        //os_memset(read_pcm_tmp_buff_alloc, 0, MAX(one_pcm_frame_encode_sample_bytes, one_pcm_frame_encode_sample_bytes_bk) * 2);
                        os_memcpy(read_pcm_tmp_buff_alloc, s_pcm_covert_missing_buff, s_pcm_covert_missing_len);
                        os_memcpy(read_pcm_tmp_buff_alloc + s_pcm_covert_missing_len, read_pcm_tmp_buff_covert_alloc, final_copy_len - s_pcm_covert_missing_len);

                        if (final_copy_len < covert_step_size + s_pcm_covert_missing_len)
                        {
                            if (covert_step_size + s_pcm_covert_missing_len - final_copy_len > sizeof(s_pcm_covert_missing_buff))
                            {
                                a2dp_loge("pcm covert missing len %d > %d !!!", covert_step_size + s_pcm_covert_missing_len - final_copy_len, sizeof(s_pcm_covert_missing_buff));
                                os_memcpy(s_pcm_covert_missing_buff, read_pcm_tmp_buff_covert_alloc + final_copy_len - s_pcm_covert_missing_len, sizeof(s_pcm_covert_missing_buff));

                                s_pcm_covert_missing_len = sizeof(s_pcm_covert_missing_buff);
                            }
                            else
                            {
                                os_memcpy(s_pcm_covert_missing_buff, read_pcm_tmp_buff_covert_alloc + final_copy_len - s_pcm_covert_missing_len, covert_step_size + s_pcm_covert_missing_len - final_copy_len);

                                s_pcm_covert_missing_len = covert_step_size + s_pcm_covert_missing_len - final_copy_len;
                            }
                        }
                        else
                        {
                            s_pcm_covert_missing_len = 0;
                        }

                        if (final_copy_len != one_pcm_frame_encode_sample_bytes_bk)
                        {
                            a2dp_loge("sample count not match, expect %d but %d, %d", one_pcm_frame_encode_sample_bytes_bk,
                                      final_copy_len, s_pcm_covert_missing_len);

                            s_pcm_covert_missing_len = 0;
                        }
                    }
                }
            }

            encode_len = sbc_encoder_encode(&s_sbc_software_encoder_ctx, (const int16_t *)(read_pcm_tmp_buff_alloc));

            if (encode_len < 0)
            {
                a2dp_logi("encode err %d", encode_len);
                goto error;
            }

            if (encode_index + encode_len > will_send_frame_count * 128)
            {
                a2dp_logi("encode data large than malloc !! %d", encode_len);
                goto error;
            }

            os_memcpy(encode_tmp_buff + encode_index, s_sbc_software_encoder_ctx.stream, encode_len);
            encode_index += encode_len;

            if (s_mp3_frame_info.samprate != s_a2dp_cap_info.sbc_codec.sample_rate || s_mp3_frame_info.bitsPerSample != SBC_SAMPLE_DEPTH)
            {
                float cacl_float = ((one_pcm_frame_encode_sample_bytes_bk - s_pcm_covert_missing_len) * 1.0 * s_mp3_frame_info.samprate / s_a2dp_cap_info.sbc_codec.sample_rate *
                                    s_mp3_frame_info.nChans / s_a2dp_cap_info.sbc_codec.channels);

                one_pcm_frame_encode_sample_bytes = (typeof(one_pcm_frame_encode_sample_bytes))cacl_float;

                if (one_pcm_frame_encode_sample_bytes < cacl_float)
                {
                    one_pcm_frame_encode_sample_bytes += 1;
                }

                one_pcm_frame_encode_sample_bytes = INT_CEIL(one_pcm_frame_encode_sample_bytes, s_mp3_frame_info.nChans * s_mp3_frame_info.bitsPerSample / 8);
            }
        }

        ret = bk_bt_a2dp_source_write(&(a2dp_env.peer_addr), i, encode_tmp_buff, encode_index);

        if (encode_index > 10000 || i > 5)
        {
            a2dp_logi("encode_index %d i %d", encode_index, i);
        }

        if (ret)
        {
            a2dp_logi("bk_bt_a2dp_source_write err %d", ret);
        }

        encode_index = 0;
        j += i;
    }
    while (j < will_send_frame_count);

    if (s_remain_send_sample_count < j * one_pcm_frame_encode_sample_count_per_channel)
    {
        s_remain_send_sample_count = 0;
    }
    else
    {
        s_remain_send_sample_count -= j * one_pcm_frame_encode_sample_count_per_channel;
    }

error:;

    rtos_set_semaphore(&s_source_need_decode_sema);

    if (encode_tmp_buff)
    {
        os_free(encode_tmp_buff);
        encode_tmp_buff = NULL;
    }

    if (read_pcm_tmp_buff_alloc)
    {
        os_free(read_pcm_tmp_buff_alloc);
        read_pcm_tmp_buff_alloc = NULL;
    }

    if (read_pcm_tmp_buff_covert_alloc)
    {
        os_free(read_pcm_tmp_buff_covert_alloc);
        read_pcm_tmp_buff_covert_alloc = NULL;
    }
}


static void a2dp_source_timer_hdl(void *param)
{
    bt_a2dp_source_msg_t msg;
    bk_err_t rc = -1;

    if (bt_a2dp_source_msg_que == NULL)
    {
        return;
    }

    os_memset(&msg, 0, sizeof(msg));

    msg.len = sizeof(FIL *);
    msg.data = param;

    msg.type = (typeof(s_msg_type))param;

    if (a2dp_env.start_status == 1)
    {
#if 0
        rc = rtos_push_to_queue(&bt_a2dp_source_msg_que, &msg, BEKEN_NO_WAIT);

        if (kNoErr != rc)
        {
            a2dp_logv("send queue failed");
        }

#else
        (void)msg;
        s_msg_type = (typeof(s_msg_type))param;
        rc = rtos_set_semaphore(&s_source_timer_sema);

        if (rc)
        {
            a2dp_logv("set sem failed");
        }

#endif
    }
}

static int32_t bt_a2dp_source_start_read_pcm_from_file(uint8_t *path)
{
    FRESULT fr = 0;
    uint32_t inter = (uint32_t)((640 * 1000.0) / (float)s_sbc_software_encoder_ctx.sample_rate);
    char full_path[64] = {0};

    s_pfs = os_malloc(sizeof(FATFS));

    if (NULL == s_pfs)
    {
        a2dp_logi("%s malloc failed!");
        return -1;
    }

    os_memset(s_pfs, 0, sizeof(*s_pfs));

    fr = f_mount(s_pfs, "1:", 1);

    if (fr != FR_OK)
    {
        a2dp_logi("f_mount failed:%d ", fr);
        return -1;
    }

    //can't free !!!!!
    //    os_free(s_pfs);
    //    s_pfs = NULL;

    a2dp_logi("f_mount OK!");

    //need s16le dual pcm
    //    sprintf((char *)full_path, "%d:/%s", mount_label, path);
    sprintf((char *)full_path, "%d:/%s", DISK_NUMBER_SDIO_SD, path);

    a2dp_logi("send interval %d", inter);
    //a2dp_env.play_status = 1;

    if (rtos_is_timer_init(&bt_a2dp_source_write_timer))
    {
        if (rtos_is_timer_running(&bt_a2dp_source_write_timer))
        {
            rtos_stop_timer(&bt_a2dp_source_write_timer);
        }

        rtos_deinit_timer(&bt_a2dp_source_write_timer);
    }

    if (!rtos_is_timer_init(&bt_a2dp_source_write_timer))
    {
        //        rtos_init_timer(&bt_a2dp_source_write_timer, inter, a2dp_source_read_pcm_from_file_hdl_timer_hdl, (void *)&pcm_file_fd);
        rtos_init_timer(&bt_a2dp_source_write_timer, A2DP_SOURCE_WRITE_AUTO_TIMER_MS, a2dp_source_timer_hdl, (void *)BT_A2DP_SOURCE_MSG_READ_PCM_FROM_FILE);
    }

    rtos_change_period(&bt_a2dp_source_write_timer, inter);

    if (pcm_file_fd.fs)
    {
        f_close(&pcm_file_fd);
    }

    os_memset(&pcm_file_fd, 0, sizeof(pcm_file_fd));
    fr = f_open(&pcm_file_fd, (const char *)full_path, FA_OPEN_EXISTING | FA_READ);

    if (fr != FR_OK)
    {
        a2dp_logi("open %s fail.", full_path);
        return -1;
    }

    rtos_start_timer(&bt_a2dp_source_write_timer);

    return 0;
}

static int32_t bt_a2dp_source_prepare_sbc_encoder(void)
{
    bk_err_t ret = BK_OK;

    a2dp_logi("");

    os_memset(&s_sbc_software_encoder_ctx, 0, sizeof(s_sbc_software_encoder_ctx));

    sbc_encoder_init(&s_sbc_software_encoder_ctx, s_a2dp_cap_info.sbc_codec.sample_rate, 1);

    uint8_t alloc_mode = 0;

    switch (s_a2dp_cap_info.sbc_codec.alloc_mode)
    {
    case 2://A2DP_SBC_ALLOCATION_METHOD_SNR:
        alloc_mode = 1;
        break;

    default:
    case 1://A2DP_SBC_ALLOCATION_METHOD_LOUDNESS:
        alloc_mode = 0;
        break;
    }

    ret = sbc_encoder_ctrl(&s_sbc_software_encoder_ctx, SBC_ENCODER_CTRL_CMD_SET_ALLOCATION_METHOD, alloc_mode); //0:loundness, 1:SNR

    if (ret != SBC_ENCODER_ERROR_OK)
    {
        a2dp_logi("SBC_ENCODER_CTRL_CMD_SET_ALLOCATION_METHOD err %d", ret);
        return ret;
    }

    ret = sbc_encoder_ctrl(&s_sbc_software_encoder_ctx, SBC_ENCODER_CTRL_CMD_SET_BITPOOL, s_a2dp_cap_info.sbc_codec.bit_pool);

    if (ret != SBC_ENCODER_ERROR_OK)
    {
        a2dp_logi("SBC_ENCODER_CTRL_CMD_SET_BITPOOL err %d", ret);
        return ret;
    }


    uint8_t block_mode = 3;

    switch (s_a2dp_cap_info.sbc_codec.block_len)
    {
    case 4://A2DP_SBC_BLOCK_LENGTH_4:
        block_mode = 0;
        break;

    case 8://A2DP_SBC_BLOCK_LENGTH_8:
        block_mode = 1;
        break;

    case 12://A2DP_SBC_BLOCK_LENGTH_12:
        block_mode = 2;
        break;

    default:
    case 16://A2DP_SBC_BLOCK_LENGTH_16:
        block_mode = 3;
        break;
    }

    ret = sbc_encoder_ctrl(&s_sbc_software_encoder_ctx, SBC_ENCODER_CTRL_CMD_SET_BLOCK_MODE, block_mode); //0:4, 1:8, 2:12, 3:16

    if (ret != SBC_ENCODER_ERROR_OK)
    {
        a2dp_logi("SBC_ENCODER_CTRL_CMD_SET_BLOCK_MODE err %d", ret);
        return ret;
    }

    uint8_t channle_mode = 3;

    switch (s_a2dp_cap_info.sbc_codec.channel_mode)
    {
    case 8:
        channle_mode = 0;
        break;

    case 4:
        channle_mode = 1;
        break;

    case 2:
        channle_mode = 2;
        break;

    default:
    case 1:
        channle_mode = 3;
        break;
    }

    ret = sbc_encoder_ctrl(&s_sbc_software_encoder_ctx, SBC_ENCODER_CTRL_CMD_SET_CHANNEL_MODE, channle_mode); //0:MONO, 1:DUAL, 2:STEREO, 3:JOINT STEREO

    if (ret != SBC_ENCODER_ERROR_OK)
    {
        a2dp_logi("SBC_ENCODER_CTRL_CMD_SET_CHANNEL_MODE err %d", ret);
        return ret;
    }

    uint8_t samp_rate_select = 2;

    switch (s_a2dp_cap_info.sbc_codec.sample_rate)
    {
    case 16000:
        samp_rate_select = 0;
        break;

    case 32000:
        samp_rate_select = 1;
        break;

    default:
    case 44100:
        samp_rate_select = 2;
        break;

    case 48000:
        samp_rate_select = 3;
        break;
    }

    ret = sbc_encoder_ctrl(&s_sbc_software_encoder_ctx, SBC_ENCODER_CTRL_CMD_SET_SAMPLE_RATE_INDEX, samp_rate_select); //0:16000, 1:32000, 2:44100, 3:48000

    if (ret != SBC_ENCODER_ERROR_OK)
    {
        a2dp_logi("SBC_ENCODER_CTRL_CMD_SET_SAMPLE_RATE_INDEX err %d", ret);
        return ret;
    }

    uint8_t subband = 1;

    switch (s_a2dp_cap_info.sbc_codec.subbands)
    {
    case 4:
        subband = 0;
        break;

    default:
    case 8:
        subband = 1;
        break;
    }

    ret = sbc_encoder_ctrl(&s_sbc_software_encoder_ctx, SBC_ENCODER_CTRL_CMD_SET_SUBBAND_MODE, subband); //0:4, 1:8

    if (ret != SBC_ENCODER_ERROR_OK)
    {
        a2dp_logi("SBC_ENCODER_CTRL_CMD_SET_SUBBAND_MODE err %d", ret);
        return ret;
    }


    a2dp_logi("sbc encode count %d", s_sbc_software_encoder_ctx.pcm_length, A2DP_SOURCE_WRITE_AUTO_TIMER_MS * s_sbc_software_encoder_ctx.sample_rate);

    return 0;
}

static int32_t get_mp3_info(MP3FrameInfo *info)
{
    FRESULT fr = 0;
    char full_path[64] = {0};
    uint8_t tag_header[10] = {0};
    unsigned int num_rd = 0;
    uint8_t *mp3_read_start_ptr = NULL;
    uint8_t *pcm_write_ptr = NULL;
    int bytesleft = 0;
    uint8_t *current_mp3_read_ptr = NULL;
    uint8_t id3v2_maj_ver = 0;
    uint16_t id3v2_min_ver = 0;
    uint32_t file_size = 0;

    uint32_t frame_start_offset = 0;
    int ret = 0;

    (void)id3v2_maj_ver;

    os_memset(info, 0, sizeof(*info));

    s_pfs = os_malloc(sizeof(FATFS));

    if (NULL == s_pfs)
    {
        a2dp_loge("s_pfs malloc failed!");
        goto error;
    }

    os_memset(s_pfs, 0, sizeof(*s_pfs));

    fr = f_mount(s_pfs, "1:", 1);

    if (fr != FR_OK)
    {
        a2dp_loge("f_mount failed:%d", fr);
        goto error;
    }

    a2dp_logi("f_mount OK!");

    s_mp3_decoder = MP3InitDecoder();

    if (!s_mp3_decoder)
    {
        a2dp_loge("s_mp3_decoder MP3InitDecoder failed!");
        goto error;
    }

    a2dp_logi("MP3InitDecoder init successful!");

    /*open file to read mp3 data */
    sprintf((char *)full_path, "%d:/%s", DISK_NUMBER_SDIO_SD, s_file_path);
    fr = f_open(&mp3file, full_path, FA_OPEN_EXISTING | FA_READ);

    if (fr != FR_OK)
    {
        a2dp_loge("open %s failed!", full_path);
        goto error;
    }

    fr = f_read(&mp3file, (void *)tag_header, sizeof(tag_header), &num_rd);

    if (fr != FR_OK || num_rd != sizeof(tag_header))
    {
        a2dp_loge("read %s failed!", full_path);
        goto error;
    }

    if (os_memcmp(tag_header, "ID3", 3))
    {
        FILINFO info;
        uint8_t id3v1[128] = {0};

        //a2dp_loge("ID3v2 not found!, try found ID3v1");

        os_memset(&info, 0, sizeof(info));
        fr = f_stat(full_path, &info);

        if (fr != FR_OK)
        {
            a2dp_loge("f_stat err %d", fr);
            goto error;
        }

        file_size = info.fsize;

        fr = f_lseek(&mp3file, file_size - 128);

        if (fr != FR_OK)
        {
            a2dp_loge("f_lseek to ID3v1 end err %d", fr);
            goto error;
        }

        fr = f_read(&mp3file, id3v1, sizeof(id3v1), &num_rd);

        if (fr != FR_OK || num_rd != sizeof(id3v1))
        {
            a2dp_loge("read ID3v1 err %d num %d", fr, num_rd);
            goto error;
        }

        if (os_memcmp(id3v1, "TAG", 3))
        {
            a2dp_loge("ID3v1 not found!");
            goto error;
        }

        frame_start_offset = 0;

        f_lseek(&mp3file, frame_start_offset);

        a2dp_logi("found ID3v1");

        id3v2_maj_ver = 1;
    }
    else
    {
        uint32_t tag_size = ((tag_header[6] & 0x7F) << 21) | ((tag_header[7] & 0x7F) << 14) | ((tag_header[8] & 0x7F) << 7) | (tag_header[9] & 0x7F);
        frame_start_offset = sizeof(tag_header) + tag_size;

        id3v2_min_ver = ((tag_header[4] << 8) | tag_header[3]);
        id3v2_maj_ver = 2;

        a2dp_logi("ID3v2.%d flag 0x%x len %d", id3v2_min_ver, tag_header[5], tag_size);

        if (id3v2_min_ver == 4 && (tag_header[5] & (1 << 4)))
        {
            frame_start_offset += 10;
        }

        f_lseek(&mp3file, frame_start_offset);
    }

    a2dp_logi("mp3 file open successfully!");

    mp3_read_start_ptr = os_malloc(MAINBUF_SIZE * 2);

    if (!mp3_read_start_ptr)
    {
        a2dp_loge("mp3_read_ptr alloc err");
        goto error;
    }

    pcm_write_ptr = os_malloc(MP3_DECODE_BUFF_SIZE);

    if (!pcm_write_ptr)
    {
        a2dp_loge("pcm_write_ptr alloc err");
        goto error;
    }

    //get frame info
    {
        fr = f_read(&mp3file, mp3_read_start_ptr, MAINBUF_SIZE, &num_rd);

        if (fr != FR_OK)
        {
            a2dp_loge("test read frame %d %s failed!", num_rd, full_path);
            goto error;
        }

        bytesleft = MP3_DECODE_BUFF_SIZE;

        current_mp3_read_ptr = mp3_read_start_ptr;

        ret = MP3Decode(s_mp3_decoder, &current_mp3_read_ptr, &bytesleft, (int16_t *)pcm_write_ptr, 0);

        if (ret != ERR_MP3_NONE)
        {
            a2dp_loge("MP3Decode failed, code is %d bytesleft %d", ret, bytesleft);
            goto error;
        }

        MP3GetLastFrameInfo(s_mp3_decoder, &s_mp3_frame_info);

        a2dp_logi("bytesleft %d readsize %d", bytesleft, f_tell(&mp3file));
        a2dp_logi("Bitrate: %d kb/s, Samprate: %d, samplebits %d", (s_mp3_frame_info.bitrate) / 1000, s_mp3_frame_info.samprate, s_mp3_frame_info.bitsPerSample);
        a2dp_logi("Channel: %d, Version: %d, Layer: %d", s_mp3_frame_info.nChans, s_mp3_frame_info.version, s_mp3_frame_info.layer);
        a2dp_logi("OutputSamps: %d %d", s_mp3_frame_info.outputSamps, s_mp3_frame_info.outputSamps * s_mp3_frame_info.bitsPerSample / 8);

        f_lseek(&mp3file, frame_start_offset);
    }
error:;

    if (s_mp3_decoder)
    {
        MP3FreeDecoder(s_mp3_decoder);
        s_mp3_decoder = NULL;
    }

    if (mp3file.fs)
    {
        f_close(&mp3file);
        os_memset(&mp3file, 0, sizeof(mp3file));
    }

    if (s_pfs)
    {
        fr = f_unmount(DISK_NUMBER_SDIO_SD, "1:", 1);

        if (fr)
        {
            a2dp_loge("f_unmount err %d", fr);
        }

        os_free(s_pfs);
        s_pfs = NULL;
    }

    if (pcm_write_ptr)
    {
        os_free(pcm_write_ptr);
        pcm_write_ptr = NULL;
    }

    if (mp3_read_start_ptr)
    {
        os_free(mp3_read_start_ptr);
        mp3_read_start_ptr = NULL;
    }

    return 0;
}

static void bt_a2dp_source_decode_task(void *arg)
{
    FRESULT fr = 0;
    char full_path[64] = {0};
    uint8_t tag_header[10] = {0};
    unsigned int num_rd = 0;
    uint8_t *mp3_read_start_ptr = NULL;
    uint8_t *pcm_write_ptr = NULL;
    int bytesleft = 0;
    uint8_t *current_mp3_read_ptr = NULL;
    uint8_t *mp3_read_end_ptr = NULL;
    uint8_t id3v2_maj_ver = 0;
    uint16_t id3v2_min_ver = 0;
    uint32_t file_size = 0;
    uint32_t pcm_decode_size = 0;

    uint32_t frame_start_offset = 0;
    int ret = 0;
    MP3FrameInfo tmp_mp3_frame_info;

    s_pfs = os_malloc(sizeof(FATFS));

    if (NULL == s_pfs)
    {
        a2dp_loge("s_pfs malloc failed!");
        goto error;
    }

    os_memset(&tmp_mp3_frame_info, 0, sizeof(tmp_mp3_frame_info));
    os_memset(s_pfs, 0, sizeof(*s_pfs));

    fr = f_mount(s_pfs, "1:", 1);

    if (fr != FR_OK)
    {
        a2dp_loge("f_mount failed:%d", fr);
        goto error;
    }

    //can't free !!!!!
    //    os_free(s_pfs);
    //    s_pfs = NULL;

    a2dp_logi("f_mount OK!");

    s_mp3_decoder = MP3InitDecoder();

    if (!s_mp3_decoder)
    {
        a2dp_loge("s_mp3_decoder MP3InitDecoder failed!");
        goto error;
    }

    a2dp_logi("MP3InitDecoder init successful!");

    /*open file to read mp3 data */
    sprintf((char *)full_path, "%d:/%s", DISK_NUMBER_SDIO_SD, s_file_path);
    fr = f_open(&mp3file, full_path, FA_OPEN_EXISTING | FA_READ);

    if (fr != FR_OK)
    {
        a2dp_loge("open %s failed!", full_path);
        goto error;
    }

    fr = f_read(&mp3file, (void *)tag_header, sizeof(tag_header), &num_rd);

    if (fr != FR_OK || num_rd != sizeof(tag_header))
    {
        a2dp_loge("read %s failed!", full_path);
        goto error;
    }

    if (os_memcmp(tag_header, "ID3", 3))
    {
        FILINFO info;
        uint8_t id3v1[128] = {0};

        //a2dp_loge("ID3v2 not found!, try found ID3v1");

        os_memset(&info, 0, sizeof(info));
        fr = f_stat(full_path, &info);

        if (fr != FR_OK)
        {
            a2dp_loge("f_stat err %d", fr);
            goto error;
        }

        file_size = info.fsize;

        fr = f_lseek(&mp3file, file_size - 128);

        if (fr != FR_OK)
        {
            a2dp_loge("f_lseek to ID3v1 end err %d", fr);
            goto error;
        }

        fr = f_read(&mp3file, id3v1, sizeof(id3v1), &num_rd);

        if (fr != FR_OK || num_rd != sizeof(id3v1))
        {
            a2dp_loge("read ID3v1 err %d num %d", fr, num_rd);
            goto error;
        }

        if (os_memcmp(id3v1, "TAG", 3))
        {
            a2dp_loge("ID3v1 not found!");
            goto error;
        }

        frame_start_offset = 0;

        f_lseek(&mp3file, frame_start_offset);

        a2dp_logi("found ID3v1");

        id3v2_maj_ver = 1;
    }
    else
    {
        uint32_t tag_size = ((tag_header[6] & 0x7F) << 21) | ((tag_header[7] & 0x7F) << 14) | ((tag_header[8] & 0x7F) << 7) | (tag_header[9] & 0x7F);
        frame_start_offset = sizeof(tag_header) + tag_size;

        id3v2_min_ver = ((tag_header[4] << 8) | tag_header[3]);
        id3v2_maj_ver = 2;

        a2dp_logi("ID3v2.%d flag 0x%x len %d", id3v2_min_ver, tag_header[5], tag_size);

        if (id3v2_min_ver == 4 && (tag_header[5] & (1 << 4)))
        {
            frame_start_offset += 10;
        }

        f_lseek(&mp3file, frame_start_offset);
    }

    a2dp_logi("mp3 file open successfully!");

    mp3_read_start_ptr = os_malloc(MAINBUF_SIZE * 2);

    if (!mp3_read_start_ptr)
    {
        a2dp_loge("mp3_read_ptr alloc err");
        goto error;
    }

    pcm_write_ptr = os_malloc(MP3_DECODE_BUFF_SIZE);

    if (!pcm_write_ptr)
    {
        a2dp_loge("pcm_write_ptr alloc err");
        goto error;
    }

    bytesleft = 0;
    current_mp3_read_ptr = mp3_read_start_ptr;

    mp3_read_end_ptr = mp3_read_start_ptr + MAINBUF_SIZE * 2;

    while (1)
    {
        if (current_mp3_read_ptr - mp3_read_start_ptr > MAINBUF_SIZE)
        {
            a2dp_logd("%p move to %p", current_mp3_read_ptr, mp3_read_start_ptr);

            os_memmove(mp3_read_start_ptr, current_mp3_read_ptr, bytesleft);
            current_mp3_read_ptr = mp3_read_start_ptr;
        }

        fr = f_read(&mp3file, current_mp3_read_ptr + bytesleft, mp3_read_end_ptr - current_mp3_read_ptr - bytesleft, &num_rd);

        if (fr != FR_OK)
        {
            a2dp_loge("read %d %s failed!", num_rd, full_path);
            goto error;
        }

        if (!num_rd)
        {
            a2dp_logi("file end, return to begin");

            os_memmove(mp3_read_start_ptr, current_mp3_read_ptr, bytesleft);
            current_mp3_read_ptr = mp3_read_start_ptr;

            f_lseek(&mp3file, frame_start_offset);
            continue;
        }

        if (id3v2_maj_ver == 1 && file_size - f_tell(&mp3file) <= 128)
        {
            num_rd -= f_tell(&mp3file) - (file_size - 128);
        }

        bytesleft += num_rd;

        a2dp_logd("bytesleft %d %p", bytesleft, current_mp3_read_ptr);

        do
        {
            uint8_t *last_success_read_ptr = current_mp3_read_ptr;
            int last_success_bytesleft = bytesleft;

            if (!bytesleft)
            {
                break;
            }

            while (ring_buffer_particle_len(&s_rb_ctx) > s_decode_trigger_size)
            {
                a2dp_logd("ring buffer not read too much, wait %d", ring_buffer_particle_len(&s_rb_ctx));
                rtos_get_semaphore(&s_source_need_decode_sema, BEKEN_WAIT_FOREVER);
            }

            ret = MP3Decode(s_mp3_decoder, &current_mp3_read_ptr, &bytesleft, (int16_t *)pcm_write_ptr, 0);

            if (ret != ERR_MP3_NONE)
            {
                if (ERR_MP3_INDATA_UNDERFLOW != ret)
                {
                    a2dp_loge("MP3Decode failed, code is %d bytesleft %d", ret, bytesleft);
                    goto error;
                }
                else
                {
                    bytesleft = last_success_bytesleft;
                    current_mp3_read_ptr = last_success_read_ptr;
                    break;
                }
            }

            //            os_memset(&s_mp3_frame_info, 0, sizeof(s_mp3_frame_info));
            MP3GetLastFrameInfo(s_mp3_decoder, &tmp_mp3_frame_info);

            a2dp_logd("start write ring buff %d", tmp_mp3_frame_info.outputSamps * tmp_mp3_frame_info.bitsPerSample / 8);

            if (ring_buffer_particle_write(&s_rb_ctx, pcm_write_ptr, tmp_mp3_frame_info.outputSamps * tmp_mp3_frame_info.bitsPerSample / 8))
            {
                a2dp_logd("ring_buffer full %d", ring_buffer_particle_len(&s_rb_ctx));
                //ring_buffer_particle_debug(&s_rb_ctx);

                bytesleft = last_success_bytesleft;
                current_mp3_read_ptr = last_success_read_ptr;
                continue;
            }
            else
            {
                a2dp_logd("write %d, %d", tmp_mp3_frame_info.outputSamps * tmp_mp3_frame_info.bitsPerSample / 8, ring_buffer_particle_len(&s_rb_ctx));
            }

            pcm_decode_size += tmp_mp3_frame_info.outputSamps * tmp_mp3_frame_info.bitsPerSample / 8;

            a2dp_logv("bytesleft %d %p readsize %d", bytesleft, current_mp3_read_ptr, f_tell(&mp3file));
            a2dp_logv("Bitrate: %d kb/s, Samprate: %d", (tmp_mp3_frame_info.bitrate) / 1000, tmp_mp3_frame_info.samprate);
            a2dp_logv("Channel: %d, Version: %d, Layer: %d", tmp_mp3_frame_info.nChans, tmp_mp3_frame_info.version, tmp_mp3_frame_info.layer);
            a2dp_logv("OutputSamps: %d %d", tmp_mp3_frame_info.outputSamps, tmp_mp3_frame_info.outputSamps * tmp_mp3_frame_info.bitsPerSample / 8);
        }
        while (tmp_mp3_frame_info.outputSamps);

        if (0)//!num_rd || (id3v2_maj_ver == 1 && file_size - f_tell(&mp3file) <= 128))
        {
            a2dp_logi("decode end %d %d", bytesleft, pcm_decode_size);
            a2dp_logi("samplerate %d channel %d ver %d", tmp_mp3_frame_info.samprate, tmp_mp3_frame_info.nChans, tmp_mp3_frame_info.version);
            break;
        }

        if (!s_decode_task_run)
        {
            goto error;
        }
    }

error:;

    if (s_mp3_decoder)
    {
        MP3FreeDecoder(s_mp3_decoder);
        s_mp3_decoder = NULL;
    }

    if (mp3file.fs)
    {
        f_close(&mp3file);
        os_memset(&mp3file, 0, sizeof(mp3file));
    }

    if (s_pfs)
    {
        fr = f_unmount(DISK_NUMBER_SDIO_SD, "1:", 1);

        if (fr)
        {
            a2dp_loge("f_unmount err %d", fr);
        }

        os_free(s_pfs);
        s_pfs = NULL;
    }

    if (pcm_write_ptr)
    {
        os_free(pcm_write_ptr);
        pcm_write_ptr = NULL;
    }

    if (mp3_read_start_ptr)
    {
        os_free(mp3_read_start_ptr);
        mp3_read_start_ptr = NULL;
    }

    bt_a2dp_source_decode_thread_handle = NULL;
    rtos_delete_thread(NULL);

    return;
}


static bk_err_t a2dp_source_demo_create_mp3_decode_task(void)
{
    bk_err_t ret = BK_OK;

    if (!bt_a2dp_source_decode_thread_handle)
    {
        ret = rtos_create_thread(&bt_a2dp_source_decode_thread_handle,
                                 BEKEN_DEFAULT_WORKER_PRIORITY - 1,
                                 "bt_a2dp_source_decode",
                                 (beken_thread_function_t)bt_a2dp_source_decode_task,
                                 1024 * 4,
                                 (beken_thread_arg_t)0);

        if (ret != kNoErr)
        {
            a2dp_logi("task fail");

            bt_a2dp_source_decode_thread_handle = NULL;
            return kInProgressErr;
        }

        s_decode_task_run = 1;
        return kNoErr;
    }
    else
    {
        return kInProgressErr;
    }
}


static void bt_a2dp_source_main_task(void *arg)
{
    bk_err_t err = 0;

    if (bk_bt_get_host_stack_type() != BK_BT_HOST_STACK_TYPE_ETHERMIND)
    {
        a2dp_logi("only ethermind host support a2dp");
        goto error;
    }

    err = rtos_init_semaphore(&s_bt_api_sema, 1);

    if (err)
    {
        a2dp_logi("sem init err %d", err);
        goto error;
    }

    bk_bt_gap_set_event_callback(bt_at_event_cb);

    //    os_memcpy(a2dp_env.peer_addr.addr, addr, sizeof(a2dp_env.peer_addr.addr));

    if (!a2dp_env.inited)
    {
        bk_bt_a2dp_source_init((void *)&bt_a2dp_source_cb);
        a2dp_env.inited = 1;
    }

    if (a2dp_env.conn_state != BK_A2DP_CONNECTION_STATE_DISCONNECTED)
    {
        a2dp_logi("With remote device is not idle, please disconnect first");
        goto error;
    }

    //        a2dp_env.conn_state = STATE_CONNECTING;

    bk_bt_connect(&(a2dp_env.peer_addr.addr[0]),
                  CONNECTION_PACKET_TYPE,
                  CONNECTION_PAGE_SCAN_REPETITIOIN_MODE,
                  0,
                  CONNECTION_CLOCK_OFFSET,
                  1,
                  bt_at_cmd_cb);

    if (s_bt_api_sema == NULL)
    {
        goto error;
    }

    err = rtos_get_semaphore(&s_bt_api_sema, 12 * 1000);

    if (err)
    {
        a2dp_logi("bt connect, get sem err %d");
        goto error;
    }

    if (at_cmd_status == 0x00)
    {
        a2dp_env.conn_handle = conn_handle;
    }
    else
    {
        a2dp_logi("api err %d", at_cmd_status);
        goto error;
    }

    err = bk_bt_a2dp_source_connect(&(a2dp_env.peer_addr));

    if (err)
    {
        a2dp_logi("connect a2dp err %d", err);
        goto error;
    }

    err = rtos_get_semaphore(&s_bt_api_sema, 6 * 1000);

    if (err)
    {
        a2dp_logi("source connect, get sem err %d", err);
        goto error;
    }

    if (a2dp_env.conn_state != BK_A2DP_CONNECTION_STATE_CONNECTED)
    {
        a2dp_logi("With remote device is not connected");
        goto error;
    }

    if (a2dp_env.start_status != 0)
    {
        a2dp_logi("is already start");
        goto error;
    }

    err = bk_bt_a2dp_source_start(&(a2dp_env.peer_addr));

    if (err)
    {
        a2dp_logi("source start err %d", err);
        goto error;
    }

    err = rtos_get_semaphore(&s_bt_api_sema, 6 * 1000);

    if (err)
    {
        a2dp_logi("source start, get sem err %d", err);
        goto error;
    }


    if (a2dp_env.conn_state != BK_A2DP_CONNECTION_STATE_CONNECTED)
    {
        a2dp_logi("With remote device is not connected");
        goto error;
    }



    err = bt_a2dp_source_prepare_sbc_encoder();

    if (err)
    {
        a2dp_logi("prepare sbc encoder err %d", err);
        goto error;
    }

    err = rtos_init_semaphore(&s_source_timer_sema, 1);

    if (err)
    {
        a2dp_logi("sem init s_source_timer_sema err %d", err);
        goto error;
    }

    if (s_is_mp3)
    {
        s_decode_trigger_size = 0;

        get_mp3_info(&s_mp3_frame_info);

        if (!s_mp3_frame_info.samprate)
        {
            a2dp_loge("get_mp3_info err !!");
            goto error;
        }
        else
        {
            a2dp_logi("get_mp3_info success !!");
        }

        s_decode_trigger_size = ((size_t)(s_mp3_frame_info.samprate * s_mp3_frame_info.nChans * (s_mp3_frame_info.bitsPerSample / 8) * DECODE_TRIGGER_TIME));

        if (s_mp3_frame_info.samprate != s_a2dp_cap_info.sbc_codec.sample_rate)
        {
            const aud_rsp_cfg_t cfg =
            {
                .src_rate = s_mp3_frame_info.samprate,
                .src_ch = s_a2dp_cap_info.sbc_codec.channels,//s_mp3_frame_info.nChans,
                .src_bits = s_mp3_frame_info.bitsPerSample,
                .dest_rate = s_a2dp_cap_info.sbc_codec.sample_rate,
                .dest_ch = s_a2dp_cap_info.sbc_codec.channels,
                .dest_bits = SBC_SAMPLE_DEPTH,
                .complexity = 0,
                .down_ch_idx = 0,
            };

            if (0 != (err = bk_aud_rsp_init(cfg)))
            {
                a2dp_loge("bk_aud_rsp_init err %d !!", err);
                goto error;
            }

            s_is_bk_aud_rsp_inited = 1;
        }

        err = rtos_init_semaphore(&s_source_need_decode_sema, 1);

        if (err)
        {
            a2dp_logi("sem init s_source_need_decode_sema err %d", err);
            goto error;
        }

        if (ring_buffer_particle_init(&s_rb_ctx, DECODE_RB_SIZE))
        {
            a2dp_loge("pcm_decode_ring_buffer alloc err");
            goto error;
        }

        err = a2dp_source_demo_create_mp3_decode_task();

        if (err)
        {
            a2dp_logi("create task err %d", err);
            goto error;
        }

        while (ring_buffer_particle_len(&s_rb_ctx) < DECODE_RB_SIZE / 2)
        {
            rtos_delay_milliseconds(100);
        }

        if (rtos_is_timer_init(&bt_a2dp_source_write_timer))
        {
            if (rtos_is_timer_running(&bt_a2dp_source_write_timer))
            {
                rtos_stop_timer(&bt_a2dp_source_write_timer);
            }

            rtos_deinit_timer(&bt_a2dp_source_write_timer);
        }

        if (!rtos_is_timer_init(&bt_a2dp_source_write_timer))
        {
            //        rtos_init_timer(&bt_a2dp_source_write_timer, inter, a2dp_source_read_pcm_from_file_hdl_timer_hdl, (void *)&pcm_file_fd);
            rtos_init_timer(&bt_a2dp_source_write_timer, A2DP_SOURCE_WRITE_AUTO_TIMER_MS, a2dp_source_timer_hdl, (void *)BT_A2DP_SOURCE_MSG_READ_PCM_FROM_BUFF);
        }

        //uint32_t inter = (uint32_t)((640 * 1000.0) / (float)s_sbc_software_encoder_ctx.sample_rate);

        rtos_change_period(&bt_a2dp_source_write_timer, A2DP_SOURCE_WRITE_AUTO_TIMER_MS);

        rtos_start_timer(&bt_a2dp_source_write_timer);
    }
    else
    {
        err = bt_a2dp_source_start_read_pcm_from_file(s_file_path);
    }

    while (1)
    {
        bk_err_t err;

        err = rtos_get_semaphore(&s_source_timer_sema, BEKEN_WAIT_FOREVER);

        if (err)
        {
            a2dp_logi("bt connect, get sem err %d");
            goto error;
        }

        switch (s_msg_type)
        {
        case BT_A2DP_SOURCE_MSG_READ_PCM_FROM_FILE:

            if (a2dp_env.start_status == 1)
            {
                a2dp_source_read_pcm_from_file_hdl(&pcm_file_fd);
            }

            break;

        case BT_A2DP_SOURCE_MSG_READ_PCM_FROM_BUFF:
            if (a2dp_env.start_status == 1)
            {
                a2dp_source_read_pcm_from_buffer(NULL);
            }

            break;

        case BT_A2DP_SOURCE_MSG_STOP:

            f_close(&pcm_file_fd);
            os_memset(&pcm_file_fd, 0, sizeof(pcm_file_fd));
            rtos_stop_timer(&bt_a2dp_source_write_timer);
            rtos_deinit_timer(&bt_a2dp_source_write_timer);
            a2dp_set_get_sample_clean();
            rtos_deinit_semaphore(&s_source_timer_sema);
            break;
        }
    }

error:

    if (s_is_bk_aud_rsp_inited)
    {
        bk_aud_rsp_deinit();
        s_is_bk_aud_rsp_inited = 0;
    }

    if (rtos_is_timer_init(&bt_a2dp_source_write_timer))
    {
        if (rtos_is_timer_running(&bt_a2dp_source_write_timer))
        {
            rtos_stop_timer(&bt_a2dp_source_write_timer);
        }

        rtos_deinit_timer(&bt_a2dp_source_write_timer);
        os_memset(&bt_a2dp_source_write_timer, 0, sizeof(bt_a2dp_source_write_timer));
    }

    if (s_source_need_decode_sema)
    {
        rtos_deinit_semaphore(&s_source_need_decode_sema);
        s_source_need_decode_sema = 0;
    }

    if (s_bt_api_sema)
    {
        rtos_deinit_semaphore(&s_bt_api_sema);
        s_bt_api_sema = 0;
    }

    if (s_source_timer_sema)
    {
        rtos_deinit_semaphore(&s_source_timer_sema);
        s_source_timer_sema = 0;
    }

    if (s_rb_ctx.buffer)
    {
        ring_buffer_particle_deinit(&s_rb_ctx);
        os_memset(&s_rb_ctx, 0, sizeof(s_rb_ctx));
    }

    if (bt_a2dp_source_msg_que)
    {
        rtos_deinit_queue(&bt_a2dp_source_msg_que);
        bt_a2dp_source_msg_que = NULL;
    }

    bt_a2dp_source_thread_handle = NULL;
    rtos_delete_thread(NULL);
}


bk_err_t a2dp_source_demo_main_ext(uint8_t *addr, uint8_t is_mp3, uint8_t *file_path)
{
    bk_err_t ret = BK_OK;

    if (0)
    {
        if (ring_buffer_particle_test())
        {
            a2dp_loge("rb test err");
            return -1;
        }

        a2dp_logi("rb test ok");

        const aud_rsp_cfg_t cfg =
        {
            .src_rate = 48000,
            .src_ch = 2,
            .src_bits = 16,
            .dest_rate = 44100,
            .dest_ch = 2,
            .dest_bits = 16,
            .complexity = 0,
            .down_ch_idx = 0,
        };

        if (0 != (ret = bk_aud_rsp_init(cfg)))
        {
            a2dp_loge("bk_aud_rsp_init err %d !!", ret);
            return -1;
        }

        uint32_t in_pcm_depth_factor = cfg.src_bits / 8;
        uint32_t out_pcm_depth_factor = cfg.dest_bits / 8;

        uint32_t rsp_in_before_bytes = INT_CEIL(560, cfg.src_bits / 8 * cfg.src_ch);
        uint32_t rsp_out_before_bytes = INT_CEIL((uint32_t)(1.0 * rsp_in_before_bytes * cfg.dest_rate / cfg.src_rate), out_pcm_depth_factor * cfg.dest_ch);
        uint32_t rsp_in_before_count = rsp_in_before_bytes / in_pcm_depth_factor;
        uint32_t rsp_out_before_count = rsp_out_before_bytes / out_pcm_depth_factor;

        uint32_t rsp_in_after_count;
        uint32_t rsp_out_after_count;

        uint32_t before_time = rtos_get_time(), after_time = 0;
        uint8_t *performance_buff_before = os_malloc(rsp_in_before_bytes);
        uint8_t *performance_buff_after = os_malloc(rsp_out_before_bytes);
        uint32_t loop_count = 100;

        if (performance_buff_before && performance_buff_after)
        {
            //            os_memset(performance_buff_before, 0, rsp_in_before_count);
            //            os_memset(performance_buff_after, 0, rsp_out_before_count);

            for (uint32_t i = 0; i < loop_count; ++i)
            {
                rsp_in_after_count = rsp_in_before_count;
                rsp_out_after_count = rsp_out_before_count;

                ret = bk_aud_rsp_process((int16_t *)performance_buff_before, &rsp_in_after_count, (int16_t *)performance_buff_after, &rsp_out_after_count);

                if (rsp_in_after_count != rsp_in_before_count)
                {
                    a2dp_loge("aud_rsp test out buff too short %d %d", rsp_in_before_count * in_pcm_depth_factor, rsp_in_after_count * in_pcm_depth_factor);
                    break;
                }
            }

            after_time = rtos_get_time();
            a2dp_logi("aud_rsp test used %d ms count %d, before %d after %d bytes", after_time - before_time, loop_count,
                      rsp_in_before_count * in_pcm_depth_factor * loop_count, rsp_out_after_count * out_pcm_depth_factor * loop_count);
        }
        else
        {
            a2dp_loge("aud_rsp test alloc err");
        }

        if (performance_buff_before)
        {
            os_free(performance_buff_before);
        }

        if (performance_buff_after)
        {
            os_free(performance_buff_after);
        }

        bk_aud_rsp_deinit();
        return -1;
    }

    BK_ASSERT(4 == INT_CEIL(3, 4));

    if ((!bt_a2dp_source_thread_handle) && (!bt_a2dp_source_msg_que))
    {
        ret = rtos_init_queue(&bt_a2dp_source_msg_que,
                              "bt_a2dp_source_msg_que",
                              sizeof(bt_a2dp_source_msg_t),
                              20);

        if (ret != kNoErr)
        {
            a2dp_logi("bt_audio demo msg queue failed ");
            return BK_FAIL;
        }

        os_memcpy(a2dp_env.peer_addr.addr, addr, sizeof(a2dp_env.peer_addr.addr));
        os_strcpy((char *)s_file_path, (char *)file_path);
        s_is_mp3 = is_mp3;

        ret = rtos_create_thread(&bt_a2dp_source_thread_handle,
                                 BEKEN_DEFAULT_WORKER_PRIORITY,
                                 "bt_a2dp_source",
                                 (beken_thread_function_t)bt_a2dp_source_main_task,
                                 4096,
                                 (beken_thread_arg_t)0);

        if (ret != kNoErr)
        {
            a2dp_logi("task fail");
            rtos_deinit_queue(&bt_a2dp_source_msg_que);
            bt_a2dp_source_msg_que = NULL;
            bt_a2dp_source_thread_handle = NULL;
        }

        return kNoErr;
    }
    else
    {
        return kInProgressErr;
    }
}

bk_err_t a2dp_source_demo_main_stop(void)
{
    s_decode_task_run = 0;

    rtos_delay_milliseconds(1000);

    if (rtos_is_timer_init(&bt_a2dp_source_write_timer))
    {
        if (rtos_is_timer_running(&bt_a2dp_source_write_timer))
        {
            rtos_stop_timer(&bt_a2dp_source_write_timer);
        }

        rtos_deinit_timer(&bt_a2dp_source_write_timer);
        os_memset(&bt_a2dp_source_write_timer, 0, sizeof(bt_a2dp_source_write_timer));
    }

    return 0;
}

bk_err_t a2dp_source_demo_main_test(void)
{
    const uint8_t addr[] = {0x05, 0xab, 0xb8, 0x88, 0xcb, 0x04};//JBL soundbar
    const uint8_t is_mp3 = 1;

    //    const uint8_t *file_path = (uint8_t *)"1_s16l_44100_dual.pcm";
    const uint8_t *file_path = (uint8_t *)"1_qcs.mp3";
    //    const uint8_t *file_path = (uint8_t *)"1.mp3";

    return a2dp_source_demo_main_ext((uint8_t *)addr, is_mp3, (uint8_t *)file_path);
}

