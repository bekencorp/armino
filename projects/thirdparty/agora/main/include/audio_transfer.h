/*************************************************************
 *
 * This is a part of the Agora Media Framework Library.
 * Copyright (C) 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#ifndef __AUDIO_TRANSFER_H__
#define __AUDIO_TRANSFER_H__

#ifdef __cplusplus
extern "C" {
#endif


/* API */
int send_audio_data_to_agora(uint8_t *data, unsigned int len);
bk_err_t audio_tras_init(void);
bk_err_t audio_tras_deinit(void);

#ifdef __cplusplus
}
#endif
#endif /* __AUDIO_TRANSFER_H__ */
