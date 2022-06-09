/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cc3xx_otp.h"

#include "cc3xx_reg_defs.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>

void cc3xx_otp_wait_until_fuse_programming_complete(void) {
    while (! (*CC3XX_REG_NVM_AIB_FUSE_PROG_COMPLETED & 1)) {}
}

cc3xx_err_t cc3xx_otp_write(uint8_t *otp_addr, size_t size, const uint8_t *buf)
{
    uint32_t* word_ptr;
    uint32_t current_word;
    uint32_t word;
    uint32_t start_offset;
    uint8_t in_done = 0;
    size_t copy_size;
    uint16_t zero_count;

    /* First iterate through and check all values are valid (will not require a
     * 1 bit to be unset).
     */
    for(in_done = 0; in_done < size;) {
        start_offset = ((uint32_t)otp_addr + in_done) & 0x3;
        word_ptr = (uint32_t*)(otp_addr + in_done - start_offset);

        current_word = *word_ptr;
        word = 0;

        copy_size = sizeof(word) - start_offset;
        if (in_done + copy_size > size) {
            copy_size = size - in_done;
        }

        memcpy(((uint8_t*)&word) + start_offset, buf + in_done, copy_size);
        word |= current_word;

        if (memcmp(((uint8_t*)&word) + start_offset, buf + in_done, copy_size)) {
            return CC3XX_ERR_INVALID_DATA;
        }

        in_done += copy_size;
    }

    /* Then write the OTP */
    for(in_done = 0; in_done < size;) {
        start_offset = ((uint32_t)otp_addr + in_done) & 0x3;
        word_ptr = (uint32_t*)(otp_addr + in_done - start_offset);

        word = *word_ptr;

        copy_size = sizeof(word) - start_offset;
        if (in_done + copy_size > size) {
            copy_size = size - in_done;
        }

        memcpy(((uint8_t*)&word) + start_offset, buf + in_done, copy_size);

        *word_ptr = word;
        cc3xx_otp_wait_until_fuse_programming_complete();

        in_done += copy_size;
    }

    return CC3XX_ERR_SUCCESS;
}

cc3xx_err_t cc3xx_otp_read(const uint8_t *otp_addr, size_t size, uint8_t *buf)
{
    uint32_t* word_ptr;
    uint32_t word;
    uint32_t start_offset;
    uint8_t out_done;
    size_t copy_size;

    for(out_done = 0; out_done < size;) {
        start_offset = ((uint32_t)otp_addr + out_done) & 0x3;
        word_ptr = (uint32_t*)(otp_addr + out_done - start_offset);

        word = *word_ptr;

        copy_size = sizeof(word) - start_offset;
        if (out_done + copy_size > size) {
            copy_size = size - out_done;
        }

        memcpy(buf + out_done, ((uint8_t*)&word) + start_offset, copy_size);
        out_done += copy_size;
    }

    return CC3XX_ERR_SUCCESS;
}
