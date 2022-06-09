/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_spe_psa_client_lib_unordered_map.h"
#include "tfm_core_utils.h"
#include "tfm_spe_openamp_interface.h"
#include "tfm_spe_shm_openamp.h"
#include <stdbool.h>
#include <stddef.h>

/*
 * SPE map where tf-m copies the psa_client parameters
 * from non-secure memory to its local secure memory.
 */
typedef struct unordered_map {
    /*
     * Aligned with TFM_MAX_MESSAGES. A more sophisticated approach is
     * required if the intent is to increase TFM_MAX_MESSAGES beyond
     * 32 bits.
     */
    uint32_t                    busy_slots; /* protected by a spinlock */
    unordered_map_entry_t       map[TFM_MAX_MESSAGES];
} unordered_map_t;


/*
 * TF-M secure memory map: the parameters are copied to secure memory
 * from openamp non-secure memory. This is to avoid TOCTOU attack.
 */
static unordered_map_t psa_client_lib_map_;

static inline int find_first_unset_bit(uint32_t n)
{
    int index = -1;
    n = ~n & (n+1);
    while(n>0) {
        n >>= 1;
        index++;
    }
    return index;
}

static inline bool is_map_full(unordered_map_t *m)
{
    return (~(m->busy_slots) == 0);
}

static inline void set_bit(uint32_t *n, int index)
{
    *n = (*n | (1 << index));
}

static inline bool is_bit_set(uint32_t n, int index)
{
    return ((n & (1 << index)) != 0);
}

static inline void unset_bit(uint32_t *n, int index)
{
    uint32_t mask = 0;
    mask |= (1 << index);
    *n = (*n & ~mask);
}

void unordered_map_init(void)
{
    tfm_to_openamp_spe_map_spinlock_acquire();
    psa_client_lib_map_.busy_slots = 0;
    tfm_to_openamp_spe_map_spinlock_release();
}

static int32_t alloc_map_entry(unordered_map_handle_t *handle)
{
    int32_t ret;
    tfm_to_openamp_spe_map_spinlock_acquire();
    do {
        if (is_map_full(&psa_client_lib_map_)) {
            ret = OPENAMP_MAP_FULL;
            break;
        }
        *handle = find_first_unset_bit(psa_client_lib_map_.busy_slots);
        set_bit(&psa_client_lib_map_.busy_slots, *handle);
        ret = OPENAMP_SUCCESS;
    } while (0);
    tfm_to_openamp_spe_map_spinlock_release();
    return ret;
}

int32_t unordered_map_insert(const ns_openamp_msg_t *ns_msg, const void *in,
                                 unordered_map_handle_t *handle)
{
    int32_t ret;

    ret = alloc_map_entry(handle);
    if (ret) {
        return ret;
    }

    spm_memcpy(&psa_client_lib_map_.map[*handle].msg, ns_msg,
                                sizeof(ns_openamp_msg_t));

    psa_client_lib_map_.map[*handle].input_buffer = in;
    psa_client_lib_map_.map[*handle].output_buffer = NULL;
    psa_client_lib_map_.map[*handle].output_buffer_len = 0;
    psa_client_lib_map_.map[*handle].is_input_buffer_hold = false;
    psa_client_lib_map_.map[*handle].is_output_buffer = false;

    psa_client_lib_map_.map[*handle].handle = *handle;

    return OPENAMP_SUCCESS;
}

void unordered_map_free(unordered_map_handle_t handle)
{
    if (handle >= TFM_MAX_MESSAGES || handle < 0) {
        return;
    }
    spm_memset(&psa_client_lib_map_.map[handle], 0,
                               sizeof(unordered_map_entry_t));

    tfm_to_openamp_spe_map_spinlock_acquire();
    unset_bit(&psa_client_lib_map_.busy_slots, handle);
    tfm_to_openamp_spe_map_spinlock_release();
}

unordered_map_entry_t* unordered_map_get_entry_ptr(unordered_map_handle_t handle)
{
    if (handle >= TFM_MAX_MESSAGES || handle < 0) {
        return NULL;
    }
    if (!is_bit_set(psa_client_lib_map_.busy_slots, handle)) {
        return NULL;
    }
    return &psa_client_lib_map_.map[handle];
}

unordered_map_handle_t unordered_map_get_entry_handle(
                                    const unordered_map_entry_t *ptr)
{
    if (!ptr) {
        return INVALID_MAP_HANDLE;
    }

    return ptr->handle;
}

