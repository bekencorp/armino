/*
 * Copyright (c) 2020 Linaro Limited
 *
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef TFM_SPE_OPENAMP_INTERFACE_H_
#define TFM_SPE_OPENAMP_INTERFACE_H_

#define SUCCESS                     (0)
#define ERROR                       (INT32_MIN + 1)


typedef void (*openamp_to_tfm_callback)(const void *data,
        size_t len);
typedef void (*openamp_to_tfm_notify)(void);

/*
 * These functions are the logical interface from TF-M to
 * OpenAMP.
 */
int32_t tfm_to_openamp_init(openamp_to_tfm_callback cb,
                            openamp_to_tfm_notify notify);
void tfm_to_openamp_notify(void);
void tfm_to_openamp_spe_map_spinlock_acquire(void);
void tfm_to_openamp_spe_map_spinlock_release(void);
void tfm_to_openamp_reply_back(const void* data, size_t len);
void tfm_to_openamp_reply_back_no_copy(const void* data, size_t len);
void tfm_to_openamp_hold_buffer(const void *buffer);
void tfm_to_openamp_release_buffer(const void *buffer);
void *tfm_to_openamp_get_buffer(uint32_t *len);
int tfm_to_openamp_get_buffer_size(void);
void *tfm_to_openamp_translate_non_secure_to_secure_ptr(const void *ptr);
void *tfm_to_openamp_translate_secure_to_non_secure_ptr(const void *ptr);

#endif /* TFM_SPE_OPENAMP_INTERFACE_H_ */
