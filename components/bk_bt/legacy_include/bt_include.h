#ifndef INCLUDE_MODULES_BK_BT_INCLUDE_H_
#define INCLUDE_MODULES_BK_BT_INCLUDE_H_

#include <modules/bt_types.h>

#ifdef __cplusplus
extern"C" {
#endif

union codec_info
{
    uint8_t codec_type;

    struct a2dp_sbc_codec
    {
        uint8_t codec_type;
        uint8_t channels;
        uint8_t block_len;
        uint8_t subbands;
        uint32_t sample_rate;
    }sbc_codec;

    struct a2dp_aac_codec
    {
        uint8_t codec_type;
        uint8_t channels;
        uint32_t sample_rate;
    }aac_codec;
};

typedef struct
{
    void (*a2dp_start_ind)(union codec_info *codec);
    void (*a2dp_suspend_ind)(void);
    void (*media_data_ind)(uint8_t *data, uint16_t data_len);
} bt_a2dp_sink_cb_t;

typedef struct
{
    void (*a2dp_connection_change)(uint8_t status, uint8_t reason);
    void (*a2dp_capabilities_report)(void *arg);
    void (*a2dp_set_config_cnf)(uint8_t result, uint8_t reason);
    void (*a2dp_start_cnf)(uint8_t result, uint8_t reason);
    void (*a2dp_suspend_cnf)(uint8_t result, uint8_t reason);
} bt_a2dp_source_cb_t;

typedef struct
{
    void (*sco_connected)(void);
    void (*sco_disconnected)(void);
    void (*codec_ind)(uint8_t codec_id);
    void (*voice_data_ind)(uint8_t *data, uint16_t data_len);
} bt_hfp_unit_cb_t;

typedef struct
{
    void (*push_started)(uint8_t *obj_name, uint16_t len);
    void (*push_finished)(void);
    void (*push_data_ind)(uint8_t *data, uint16_t data_len);
} bt_opp_server_cb_t;
/*
 * @brief get controller stack type
 *
 * @return
 *    enum BK_BT_CONTROLLER_STACK_TYPE
 */

BK_BT_CONTROLLER_STACK_TYPE bk_bt_get_controller_stack_type(void);


/*
 * @brief get host stack type
 *
 * @return
 *    enum BK_BT_HOST_STACK_TYPE
 */
BK_BT_HOST_STACK_TYPE bk_bt_get_host_stack_type(void);

uint16_t bk_bt_get_conn_handle(uint8_t *addr);
bt_err_t bk_bt_connect(uint8_t *addr,
                            uint16_t packet_type,
                            uint8_t page_scan_repetition_mode,
                            uint8_t page_scan_mode,
                            uint16_t clock_offset,
                            uint8_t allow_role_switch,
                            bt_cmd_cb_t callback);

bt_err_t bk_bt_inquiry(
                uint32_t  lap,
                uint16_t  len,
                uint8_t num_response,
                bt_cmd_cb_t callback);

bt_err_t bk_bt_disconnect(uint8_t *addr, uint8_t reason, bt_cmd_cb_t callback);
bt_err_t bk_bt_write_scan_enable(uint8_t scan_enable, bt_cmd_cb_t callback);
bt_err_t bk_bt_read_scan_enable(bt_cmd_cb_t callback);

bt_err_t bk_bt_spp_init(void* bt_spp_event_notify_cb);
bt_err_t bk_bt_spp_connect(uint8_t *addr, uint8_t server_channel, uint32_t spp_handle, bt_cmd_cb_t callback);
bt_err_t bk_bt_spp_start(uint32_t* spp_handle, uint8_t *local_server_channel, uint32_t *spp_record_handle);
bt_err_t bk_bt_spp_tx(uint32_t spp_handle, char *data, uint16_t len, bt_cmd_cb_t callback);
bt_err_t bk_bt_sdp(uint16_t conn_handle, uint8_t *peer_addr, bt_cmd_cb_t callback);




bt_err_t bk_bt_hfp_unit_init(uint8_t msbc_supported, void *cb);
bt_err_t bk_bt_voice_out_write(uint8_t *data, uint16_t len);
bt_err_t bk_bt_opp_server_init(void *cb);

bt_err_t bk_bt_a2dp_source_init(void *cb);
bt_err_t bk_bt_a2dp_source_connect(bd_addr_t *addr);
bt_err_t bk_bt_a2dp_source_disconnect(bd_addr_t *addr);
bt_err_t bk_bt_a2dp_source_start(bd_addr_t *addr);
bt_err_t bk_bt_a2dp_source_suspend(bd_addr_t *addr);
bt_err_t bk_bt_a2dp_source_write(bd_addr_t *addr, uint32_t frame_count, uint8_t *data, uint32_t len);
/*
 * @}
 */
#ifdef __cplusplus
}
#endif


#endif
