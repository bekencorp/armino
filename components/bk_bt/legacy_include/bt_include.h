#ifndef INCLUDE_MODULES_BK_BT_INCLUDE_H_
#define INCLUDE_MODULES_BK_BT_INCLUDE_H_

#include <modules/bt_types.h>

#ifdef __cplusplus
extern"C" {
#endif
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

bt_err_t bk_bt_spp_init(void* bt_spp_event_notify_cb);
bt_err_t bk_bt_spp_connect(uint8_t *addr, uint8_t server_channel, uint32_t spp_handle, bt_cmd_cb_t callback);
bt_err_t bk_bt_spp_start(uint32_t* spp_handle, uint8_t *local_server_channel, uint32_t *spp_record_handle);
bt_err_t bk_bt_sdp(uint16_t conn_handle, uint8_t *peer_addr, bt_cmd_cb_t callback);

void bk_bt_set_event_callback(void *cb);
/*
 * @}
 */
#ifdef __cplusplus
}
#endif


#endif