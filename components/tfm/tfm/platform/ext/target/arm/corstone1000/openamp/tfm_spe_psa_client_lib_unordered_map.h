/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_SPE_PSA_CLIENT_LIB_UNORDERED_MAP_H__
#define __TFM_SPE_PSA_CLIENT_LIB_UNORDERED_MAP_H__

#include <stdbool.h>
#include "tfm_openamp_lib.h"

/* 16 bits are sufficient to store the handle. Also
 * choosing 16bits allow for better packing inside
 * the struct unordered_map_entry_t.
 */
typedef int16_t unordered_map_handle_t;
#define INVALID_MAP_HANDLE -1

/* An entry structure of map data structure */
typedef struct unordered_map_entry {
    ns_openamp_msg_t msg;
    const void *input_buffer;
    void *output_buffer;
    size_t output_buffer_len;
    unordered_map_handle_t handle; /* entry handle */
    bool is_input_buffer_hold; /* true when input buffer is held */
    bool is_output_buffer; /* true when output buffer is preallocated */
} unordered_map_entry_t;

/* Initialize the map data structure */
void unordered_map_init(void);

/* Insert entry into the map and return a handle to the entry */
int32_t unordered_map_insert(const ns_openamp_msg_t *msg, const void *in,
                                 unordered_map_handle_t *handle);

/* Free respective entry into the map represented by the handle */
void unordered_map_free(unordered_map_handle_t handle);

/* Using a handle return the memory pointer of the entry */
unordered_map_entry_t* unordered_map_get_entry_ptr(
                                 unordered_map_handle_t handle);

/* Using a entry memory location, return respective handle */
unordered_map_handle_t unordered_map_get_entry_handle(
                                 const unordered_map_entry_t *ptr);

#endif /* __TFM_SPE_PSA_CLIENT_LIB_UNORDERED_MAP_H__ */
