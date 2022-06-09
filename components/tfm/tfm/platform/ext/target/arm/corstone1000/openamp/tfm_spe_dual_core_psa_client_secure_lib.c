/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 * Copyright (c) 2021, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "config_impl.h"

#include "tfm_spe_dual_core_psa_client_secure_lib.h"
#include "tfm_rpc.h"
#include "tfm_spe_openamp_interface.h"
#include "tfm_spm_log.h"
#include "tfm_spe_psa_client_lib_unordered_map.h"
#include "psa/error.h"
#include "utilities.h"
#include "thread.h"
#include "tfm_core_utils.h"

/**
 * In linux environment and for psa_call type client api,
 * the layout of the reply from tf-m to linux is as following.
 */
typedef struct output_buffer_with_payload {
    s_openamp_msg_t header;
    psa_outvec outvec[PSA_MAX_IOVEC];
    uint8_t payload[]; /* outdata follows */
} output_buffer_with_payload_t;

static void prepare_and_send_output_msg(int32_t reply, int32_t request_id)
{
    s_openamp_msg_t msg;

    msg.request_id = request_id;
    msg.reply = reply;

    msg.params.out_vec = 0;
    msg.params.out_len = 0;

    tfm_to_openamp_reply_back(&msg, sizeof(msg));
}

static void prepare_and_send_preallocated_output_msg(int32_t reply,
        const unordered_map_entry_t* s_map_entry)
{
    uint32_t out_len = s_map_entry->msg.params.psa_call_params.out_len;
    output_buffer_with_payload_t *output_msg = (output_buffer_with_payload_t*)s_map_entry->output_buffer;

    output_msg->header.request_id = s_map_entry->msg.request_id;
    output_msg->header.reply = reply;

    output_msg->header.params.out_vec =
                   (uint32_t)tfm_to_openamp_translate_secure_to_non_secure_ptr(
                                   output_msg->outvec);
    output_msg->header.params.out_len = out_len;

    for (int i = 0; i < out_len; i++) {
        output_msg->outvec[i].base = tfm_to_openamp_translate_secure_to_non_secure_ptr(
                                        output_msg->outvec[i].base);
    }

    /* send msg to non-secure side */
    tfm_to_openamp_reply_back_no_copy(output_msg, s_map_entry->output_buffer_len);
}

void send_service_reply_to_non_secure(int32_t reply, void *private)
{
    unordered_map_handle_t handle;
    const unordered_map_entry_t* s_map_entry = (const unordered_map_entry_t*)private;

    if (s_map_entry->is_input_buffer_hold) {
        tfm_to_openamp_release_buffer(s_map_entry->input_buffer);
    }

    if (s_map_entry->is_output_buffer) {
        prepare_and_send_preallocated_output_msg(reply, s_map_entry);
    } else {
        prepare_and_send_output_msg(reply, s_map_entry->msg.request_id);
    }

    handle = unordered_map_get_entry_handle(s_map_entry);
    if (handle == INVALID_MAP_HANDLE) {
        SPMLOG_ERRMSG("FATAL_ERROR: Map handle not valid\r\n");
        TFM_CORE_ASSERT(0);
    }
    unordered_map_free(handle);
}

static psa_invec * prepare_in_vecs(unordered_map_entry_t* s_map_entry)
{
    uint32_t in_len = s_map_entry->msg.params.psa_call_params.in_len;
    TFM_CORE_ASSERT(in_len <= PSA_MAX_IOVEC);

    psa_invec *input_buffer_in_vec = (psa_invec*)tfm_to_openamp_translate_non_secure_to_secure_ptr(
                                         (void*)s_map_entry->msg.params.psa_call_params.in_vec);
    for (int i = 0; i < in_len; i++) {
        input_buffer_in_vec[i].base = tfm_to_openamp_translate_non_secure_to_secure_ptr(
                                                input_buffer_in_vec[i].base);
    }

    return input_buffer_in_vec;
}

static void * alloc_outout_buffer_in_shared_mem(size_t length,
        unordered_map_entry_t* s_map_entry)
{
    uint32_t buffer_sz = 0;

    /* pre allocate output_buffer space from openamp shared memory */
    s_map_entry->output_buffer = tfm_to_openamp_get_buffer(&buffer_sz);
    TFM_CORE_ASSERT((s_map_entry->output_buffer != NULL) && (buffer_sz >= length));
    s_map_entry->is_output_buffer = true;
    s_map_entry->output_buffer_len = length;
    spm_memset(s_map_entry->output_buffer, 0x0, length);

    return s_map_entry->output_buffer;
}

static psa_status_t alloc_and_prepare_out_vecs(psa_outvec **out_vec_start_ptr,
        unordered_map_entry_t* s_map_entry)
{
    psa_outvec *input_buffer_outvec = NULL;
    size_t output_buffer_len = 0;
    size_t current_outdata_len = 0;
    output_buffer_with_payload_t *out_buffer = NULL;
    int max_shared_mem_buffer_size = 0;
    uint32_t out_len = s_map_entry->msg.params.psa_call_params.out_len;

    TFM_CORE_ASSERT(out_len <= PSA_MAX_IOVEC);
    *out_vec_start_ptr = NULL;

    if (out_len == 0) {
        return PSA_SUCCESS;
    }

    input_buffer_outvec = (psa_outvec*)tfm_to_openamp_translate_non_secure_to_secure_ptr(
                                          (void*)s_map_entry->msg.params.psa_call_params.out_vec);

    /* calculate and validate out data len */
    output_buffer_len = sizeof(output_buffer_with_payload_t);
    for (int i = 0; i < out_len; i++) {
        output_buffer_len += input_buffer_outvec[i].len;
    }
    max_shared_mem_buffer_size = tfm_to_openamp_get_buffer_size();
    if (output_buffer_len > max_shared_mem_buffer_size) {
        SPMLOG_ERRMSGVAL("required buffer size : ", output_buffer_len);
        SPMLOG_ERRMSGVAL(" is more than maximum available : ", max_shared_mem_buffer_size);
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* prepare output buffer layout */
    out_buffer = (output_buffer_with_payload_t*)alloc_outout_buffer_in_shared_mem(
                                                output_buffer_len, s_map_entry);

    for (int i = 0; i < PSA_MAX_IOVEC; i++) {
        if (i < out_len) {
            out_buffer->outvec[i].base = &out_buffer->payload[current_outdata_len];
            out_buffer->outvec[i].len = input_buffer_outvec[i].len;
            current_outdata_len += input_buffer_outvec[i].len;
        } else {
            out_buffer->outvec[i].base = NULL;
            out_buffer->outvec[i].len = 0;
        }
    }

    *out_vec_start_ptr = out_buffer->outvec;

    return PSA_SUCCESS;
}

static psa_status_t prepare_params_for_psa_call(struct client_call_params_t *spm_params,
                           unordered_map_entry_t* s_map_entry)
{
    psa_status_t ret = PSA_SUCCESS;

    spm_params->handle = s_map_entry->msg.params.psa_call_params.handle;
    spm_params->type = s_map_entry->msg.params.psa_call_params.type;
    spm_params->in_len = s_map_entry->msg.params.psa_call_params.in_len;
    spm_params->out_len = s_map_entry->msg.params.psa_call_params.out_len;

    spm_params->out_vec = NULL;
    ret = alloc_and_prepare_out_vecs(&spm_params->out_vec, s_map_entry);
    if (ret != PSA_SUCCESS) {
        return ret;
    }

    spm_params->in_vec = prepare_in_vecs(s_map_entry);

    /* hold the input shared memory */
    tfm_to_openamp_hold_buffer(s_map_entry->input_buffer);
    s_map_entry->is_input_buffer_hold = true;

    return ret;
}

__STATIC_INLINE int32_t check_msg(const ns_openamp_msg_t *msg)
{
    /*
     * TODO
     * Comprehensive check of openamp msessage content can be implemented here.
     */
    (void)msg;
    return OPENAMP_SUCCESS;
}

static void send_error_to_non_secure(int32_t reply, int32_t request_id)
{
    prepare_and_send_output_msg(reply, request_id);
}

int32_t register_msg_to_spe_and_verify(void **private, const void *data, size_t len)
{
    unordered_map_entry_t *s_map_entry;
    ns_openamp_msg_t *ns_msg;
    unordered_map_handle_t map_handle;
    int32_t ret = OPENAMP_SUCCESS;

    *private = NULL;

    if (len < sizeof(ns_openamp_msg_t)) {
        SPMLOG_ERRMSG("Invalid parameters.\r\n");
        send_error_to_non_secure(OPENAMP_INVAL_PARAMS, 0);
        return OPENAMP_INVAL_PARAMS;
    }

    /* start of the data is with "ns_openamp_msg_t" */
    ns_msg = (ns_openamp_msg_t*)data;
    ret = unordered_map_insert(ns_msg, data, &map_handle);
    if (ret) {
        SPMLOG_ERRMSG("Map insert failed\r\n");
        send_error_to_non_secure(OPENAMP_MAP_FULL, ns_msg->request_id);
        return OPENAMP_MAP_FULL;
    }

    s_map_entry = unordered_map_get_entry_ptr(map_handle);

    /* verify msg after copy to the secure memory */
    if (check_msg(&s_map_entry->msg)) {
        SPMLOG_ERRMSG("Message is invalid\r\n");
        send_error_to_non_secure(OPENAMP_INVAL_PARAMS, ns_msg->request_id);
        unordered_map_free(map_handle);
        return OPENAMP_INVAL_PARAMS;
    }

    *private = s_map_entry;

    return ret;
}

void deliver_msg_to_tfm_spe(void *private)
{
    struct client_call_params_t spm_params = {0};
    psa_status_t psa_ret = PSA_ERROR_GENERIC_ERROR;
    unordered_map_entry_t* s_map_entry = (unordered_map_entry_t*)private;

    switch(s_map_entry->msg.call_type) {
        case OPENAMP_PSA_FRAMEWORK_VERSION:
            psa_ret = tfm_rpc_psa_framework_version();
            send_service_reply_to_non_secure(psa_ret, s_map_entry);
            break;
        case OPENAMP_PSA_VERSION:
            spm_params.sid = s_map_entry->msg.params.psa_version_params.sid;
            psa_ret = tfm_rpc_psa_version(&spm_params);
            send_service_reply_to_non_secure(psa_ret, s_map_entry);
            break;
        case OPENAMP_PSA_CALL:
            psa_ret = prepare_params_for_psa_call(&spm_params, s_map_entry);
            if (psa_ret != PSA_SUCCESS) {
                send_service_reply_to_non_secure(psa_ret, s_map_entry);
                break;
            }
            psa_ret = tfm_rpc_psa_call(&spm_params);
            if (psa_ret != PSA_SUCCESS) {
                send_service_reply_to_non_secure(psa_ret, s_map_entry);
                break;
            }
            break;
#if CONFIG_TFM_CONNECTION_BASED_SERVICE_API == 1
        case OPENAMP_PSA_CONNECT:
            spm_params.sid = s_map_entry->msg.params.psa_connect_params.sid;
            spm_params.version = s_map_entry->msg.params.psa_connect_params.version;
            psa_ret = tfm_rpc_psa_connect(&spm_params);
            if (psa_ret != PSA_SUCCESS) {
                send_service_reply_to_non_secure(psa_ret, s_map_entry);
            }
            break;
        case OPENAMP_PSA_CLOSE:
            spm_params.handle = s_map_entry->msg.params.psa_close_params.handle;
            tfm_rpc_psa_close(&spm_params);
            break;
#endif /* CONFIG_TFM_CONNECTION_BASED_SERVICE_API == 1 */
        default:
            SPMLOG_ERRMSG("msg type did not recognized\r\n");
            send_error_to_non_secure(OPENAMP_INVAL_PARAMS, s_map_entry->msg.request_id);
            unordered_map_free(unordered_map_get_entry_handle(s_map_entry));
            break;
    }
}

void init_dual_core_psa_client_secure_lib(void)
{
    unordered_map_init();
}
