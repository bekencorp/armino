#pragma once

UINT32 app_demo_cs2_p2p_init(    char *did_dskey,
                                 char *apilicense_crckey,
                                 char *initstring_p2pkey);
void app_demo_cs2_p2p_deinit(void);
int app_demo_cs2_p2p_send_packet(uint8_t *data, uint32_t len);
void app_demo_disconnect_cmd_cs2_p2p(void);


