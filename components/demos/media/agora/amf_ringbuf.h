/*************************************************************
 *
 * This is a part of the Agora Media Framework Library.
 * Copyright (C) 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#ifndef __AMF_RINGBUF_H__
#define __AMF_RINGBUF_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef void *ringbuf_handle_t;

ringbuf_handle_t amf_ringbuf_create(int size);
int amf_ringbuf_destroy(ringbuf_handle_t handle);
int amf_ringbuf_peek(char *dst, int readlen, ringbuf_handle_t handle);
int amf_ringbuf_clear(ringbuf_handle_t handle);
int amf_ringbuf_getfreesize(ringbuf_handle_t handle);
int amf_ringbuf_getdatasize(ringbuf_handle_t handle);
int amf_ringbuf_write(ringbuf_handle_t handle, char *src, int writelen);
int amf_ringbuf_read(char *dst, int readlen, ringbuf_handle_t handle);

#ifdef __cplusplus
}
#endif
#endif /* __AMF_RINGBUF_H__ */
