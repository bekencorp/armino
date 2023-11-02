#pragma once

int bt_avrcp_demo_report_playback(uint8_t status);
int bt_avrcp_demo_report_batt_status(uint8_t status);
int bt_avrcp_demo_report_play_pos(uint32_t pos);
int bt_avrcp_is_ready(void);

int bt_avrcp_demo_init(void);
