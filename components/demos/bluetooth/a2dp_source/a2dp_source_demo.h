#pragma once


//    BK_AVRCP_PLAYBACK_STOPPED = 0,                /*!< stopped */
//    BK_AVRCP_PLAYBACK_PLAYING = 1,                /*!< playing */
//    BK_AVRCP_PLAYBACK_PAUSED = 2,                 /*!< paused */
//    BK_AVRCP_PLAYBACK_FWD_SEEK = 3,               /*!< forward seek */
//    BK_AVRCP_PLAYBACK_REV_SEEK = 4,               /*!< reverse seek */
enum
{
    A2DP_PLAY_STATUS_STOPPED,
    A2DP_PLAY_STATUS_PLAYING,
    A2DP_PLAY_STATUS_PAUSED,
    A2DP_PLAY_STATUS_FWD_SEEK,
    A2DP_PLAY_STATUS_REV_SEEK,
};


int bt_a2dp_source_demo_discover(uint32_t sec, uint32_t num_report);
int bt_a2dp_source_demo_discover_cancel(void);

int bt_a2dp_source_demo_connect(uint8_t *addr);
int bt_a2dp_source_demo_disconnect(uint8_t *addr);

int bt_a2dp_source_demo_music_play(uint8_t is_mp3, uint8_t *file_path);
int bt_a2dp_source_demo_music_stop(void);
int bt_a2dp_source_demo_music_pause(void);
int bt_a2dp_source_demo_music_resume(void);
int bt_a2dp_source_demo_music_prev(void);
int bt_a2dp_source_demo_music_next(void);

int bt_a2dp_source_demo_get_play_status(void);
uint32_t bt_a2dp_source_demo_get_play_pos(void);


//for test
bk_err_t bt_a2dp_source_demo_test(uint8_t *addr, uint8_t is_mp3, uint8_t *file_path);
bk_err_t bt_a2dp_source_demo_stop_all(void);

