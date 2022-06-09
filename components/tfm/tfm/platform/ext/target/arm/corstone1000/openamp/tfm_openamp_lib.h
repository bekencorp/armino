/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*
 * This header file is common to NSPE and SPE PSA client libraries.
 */

#ifndef __TFM_OPENAMP_LIB_H__
#define __TFM_OPENAMP_LIB_H__

#include <stdint.h>
#include "psa/client.h"

#ifdef __cplusplus
extern "C" {
#endif

/* PSA client call type value */
#define OPENAMP_PSA_FRAMEWORK_VERSION       (0x1)
#define OPENAMP_PSA_VERSION                 (0x2)
#define OPENAMP_PSA_CONNECT                 (0x3)
#define OPENAMP_PSA_CALL                    (0x4)
#define OPENAMP_PSA_CLOSE                   (0x5)

/* Return code of openamp APIs */
#define OPENAMP_SUCCESS                     (0)
#define OPENAMP_MAP_FULL                    (INT32_MIN + 1)
#define OPENAMP_MAP_ERROR                   (INT32_MIN + 2)
#define OPENAMP_INVAL_PARAMS                (INT32_MIN + 3)
#define OPENAMP_NO_PERMS                    (INT32_MIN + 4)
#define OPENAMP_NO_PEND_EVENT               (INT32_MIN + 5)
#define OPENAMP_CHAN_BUSY                   (INT32_MIN + 6)
#define OPENAMP_CALLBACK_REG_ERROR          (INT32_MIN + 7)
#define OPENAMP_INIT_ERROR                  (INT32_MIN + 8)

#define HOLD_INPUT_BUFFER (1) /* IF true, TF-M Library will hold the openamp
                               * buffer so that openamp shared memory buffer
                               * does not get freed.
                               */

/*
 * This structure holds the parameters used in a PSA client call.
 */
typedef struct __attribute__((packed)) psa_client_in_params {
    union {
        struct __attribute__((packed)) {
            uint32_t        sid;
        } psa_version_params;

        struct __attribute__((packed)) {
            uint32_t        sid;
            uint32_t        version;
        } psa_connect_params;

        struct __attribute__((packed)) {
            psa_handle_t     handle;
            int32_t          type;
            uint32_t         in_vec;
            uint32_t         in_len;
            uint32_t         out_vec;
            uint32_t         out_len;
        } psa_call_params;

        struct __attribute__((packed)) {
            psa_handle_t    handle;
        } psa_close_params;
    };
} psa_client_in_params_t;

/* Openamp message passed from NSPE to SPE to deliver a PSA client call */
typedef struct __attribute__((packed)) ns_openamp_msg {
    uint32_t                    call_type;   /* PSA client call type */
    psa_client_in_params_t      params;      /* Contain parameters used in PSA
                                              * client call
                                              */

    int32_t                     client_id;   /* Optional client ID of the
                                              * non-secure caller.
                                              * It is required to identify the
                                              * non-secure task when NSPE OS
                                              * enforces non-secure task
                                              * isolation
                                              */
    int32_t                     request_id;  /* This is the unique ID for a
                                              * request send to TF-M by the
                                              * non-secure core. TF-M forward
                                              * the ID back to non-secure on the
                                              * reply to a given request. Using
                                              * this id, the non-secure library
                                              * can identify the request for
                                              * which the reply has received.
                                              */
} ns_openamp_msg_t;

/*
 * This structure holds the location of the out data of the PSA client call.
 */
typedef struct __attribute__((packed)) psa_client_out_params {
    uint32_t          out_vec;
    uint32_t          out_len;
} psa_client_out_params_t;


/* Openamp message from SPE to NSPE delivering the reply back for a PSA client
 * call.
 */
typedef struct __attribute__((packed)) s_openamp_msg {
    int32_t                     request_id;  /* Using this id, the non-secure
                                              * library identifies the request.
                                              * TF-M forwards the same
                                              * request-id received on the
                                              * initial request.
                                              */
    int32_t                     reply;       /* Reply of the PSA client call */
    psa_client_out_params_t     params;      /* Contain out data result of the
                                              * PSA client call.
                                              */
} s_openamp_msg_t;

#ifdef __cplusplus
}
#endif

#endif /* __TFM_OPENAMP_LIB_H__ */
