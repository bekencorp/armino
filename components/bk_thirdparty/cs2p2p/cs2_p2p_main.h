#pragma once

#include <os/os.h>

enum
{
    CS2_P2P_LOG_LEVEL_NONE,
    CS2_P2P_LOG_LEVEL_ERROR,
    CS2_P2P_LOG_LEVEL_WARNING,
    CS2_P2P_LOG_LEVEL_INFO,
    CS2_P2P_LOG_LEVEL_DEBUG,
    CS2_P2P_LOG_LEVEL_VEBOSE,
};

//int server_send_with_crc_main(int argc, char **argv);
//int cs2_p2p_main(char *pDIDString, char *pAPILicense, char *pInitString, char *pRepeat, char *pWakeupKey,
//                  int32_t (*recv_callback)(uint8_t channel, uint8_t *buff, uint32_t len),
//                  uint8_t *is_run);

bk_err_t cs2_p2p_main_task_create(char *did_dskey, char *apilicense_crckey, char *initstring_p2pkey,
                                  int32_t (*recv_callback)(uint8_t channel, uint8_t *buff, uint32_t len));

bk_err_t cs2_p2p_main_task_create_ext(char *did_dskey, char *apilicense_crckey, char *initstring_p2pkey,
                                      int32_t (*recv_callback)(uint8_t channel, uint8_t *buff, uint32_t len),
                                      int8_t (*before_start)(void),
                                      int8_t (*after_end)(void));

int32_t cs2_p2p_main_task_destory(void);

int32_t cs2_p2p_send_raw(uint8_t channel, uint8_t *buff, uint32_t size);
int32_t cs2_p2p_is_network_ready(void);
int32_t cs2_p2p_is_connected(void);

int32_t cs2_p2p_set_log_level(uint8_t level);



#if CONFIG_CS2_P2P_TEST
int listentester_main(int argc, char **argv);
int connectiontester_main(int argc, char **argv);
int readwritetester_main(int argc, char **argv);
int server_send_with_crc_main(int argc, char **argv);
#endif
