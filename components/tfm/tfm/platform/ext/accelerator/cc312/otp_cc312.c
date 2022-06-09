/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_plat_otp.h"

#include "cmsis_compiler.h"
#include "dx_reg_base_host.h"
#include "tfm_attest_hal.h"
#include "uart_stdout.h"
#include "region_defs.h"
#include "cmsis.h"

/* Define some offsets from the CC312 base address, to access particular
 * registers and memory regions
 */
#define CC312_OTP_BASE_OFFSET              0x2000
#define AIB_FUSE_PROG_COMPLETED_REG_OFFSET 0x1F04
#define NVM_IS_IDLE_REG_OFFSET             0x1F10
#define LCS_IS_VALID_REG_OFFSET            0x1F0C
#define OTP_ADDR_WIDTH_DEF_REG_OFFSET      0x1F2C
#define LCS_REG_REG_OFFSET                 0x1F14

static inline uint32_t cc_read_reg(uint32_t offset) {
    return *((volatile uint32_t*)(DX_BASE_CC + offset));
}


/** @struct plat_otp_layout_t
 *  @brief  This struct documents the layout of the CC312 OTP. It documents both
 *          the builtin regions, and regions that are added to contain TF-M
 *          data. These added regions are in the "user" area, which is after the
 *          dcu lock mask.
 *
 *  @var plat_otp_layout_t::huk
 *  The huk field contains the device hardware unique key. Writing to this field
 *  causes the CC312 to transition from CM to DM LCS. This field is used to
 *  store the TF-M HUK.
 *
 *  @var plat_otp_layout_t::icv_provisioning_key
 *  The icv_provisioning_key field stores the chip manufacturer provisioning
 *  key. This field is unused in TF-M.
 *
 *  @var plat_otp_layout_t::icv_code_encryption_key
 *  The icv_code_encryption_key field stores the chip manufacturer code
 *  encryption key. This field is unused in TF-M.
 *
 *  @var plat_otp_layout_t::icv_programmed_flags
 *  The icv_programmed_flags field stores flags related to 4 crypto keys in its
 *  4 bytes. The fields are encoded such that the lowest 7 bits count the
 *  number of zeroes that appear in the key, and the upper 1 bit, if set,
 *  signals that the key is not used. The HUK field is the exception to this as
 *  it does not have a "not used" bit, and uses all 8 for the zero-count. These
 *  must be set after the HUK has been written, but before the reboot to enable
 *  DM LCS.
 *
 *  @var plat_otp_layout_t::rot_public_key
 *  This field encodes a public ROT key, and can optionally be split into
 *  separate icv_rot_public_key and oem_rot_public_key fields, each of 128 bits.
 *  Writing to this field causes the CC312 to transition from DM to SE LCS.
 *  This field is used to store the TF-M IAK, utilising both slots for a 256-bit
 *  key.
 *
 *  @var plat_otp_layout_t::oem_provisioning_secret
 *  The oem_code_encryption_key field stores the device provisioning secret.
 *  This field is unused in TF-M.
 *
 *  @var plat_otp_layout_t::oem_code_encryption_key
 *  The oem_code_encryption_key field stores the device manufacturer code
 *  encryption key. This field is unused in TF-M.
 *
 *  @var plat_otp_layout_t::oem_programmed_flags
 *  The icv_programmed_flags field stores flags related to 3 crypto keys in its
 *  first 3 bytes. The fields are encoded such that the lowest 7 bits count the
 *  number of zeroes that appear in the key, and the upper 1 bit, if set,
 *  signals that the key is not used. The rot_public_key field is the exception
 *  to this as it does not have a "not used" bit, and uses all 8 for the
 *  zero-count. These must be set after the rot public key has been written, but
 *  before the reboot to enable SE LCS.
 *
 *  @var plat_otp_layout_t::nv_counter
 *  The nv_counter field stores either 1 or 2 anti-rollback counters. This field
 *  is unused in TF-M, as it is very small and can not encode enough counter
 *  values to be used for our BL2 images.
 *
 *  @var plat_otp_layout_t::general_purpose_configuration_flags
 *  The general-purpose configuration flags are used to indicate
 *  implemention-defined configuration, and set in CM LCS. This field is unused
 *  in TF-M.
 *
 *  @var plat_otp_layout_t::dcu_debug_lock_mask
 *  This field is used to control access to secure debug. It is currently unused
 *  in TF-M.
 *
 *
 *  @var plat_otp_layout_t::*_zero_bits
 *  These fields are implemented in the CC312 user-area. They are used in TF-M
 *  to store the number of zero-bits set in other immutable assets stored in
 *  user-area fields. Because the user-area is writable in SE LCS, in order to
 *  make assets immutable the number of zeroes is stored. At boot, the stored
 *  number of zeroes is compared with the actual number in the asset. Due to OTP
 *  semantics, it is only possible to convert 0 bits to 1 bits, so the number of
 *  zeroes in the key can only ever decrease. As the *_zero_bits is encoded as
 *  an unsigned integer, it's not possible to decrease the number saved in the
 *  _zero_bits field, and thus any tampering can be spotted by the comparison.
 *
 *  @var plat_otp_layout_t::iak_len
 *  This field is implemented in the CC312 user-area. It is used in TF-M to
 *  store the length of the initial attestation key encoded as a uint32_t
 *
 *  @var plat_otp_layout_t::iak_type
 *  This field is implemented in the CC312 user-area. It is used in TF-M to
 *  store the type of the initial attestation key encoded as a uint32_t
 *
 *  @var plat_otp_layout_t::iak_id
 *  This field is implemented in the CC312 user-area. It is used in TF-M to
 *  store the id of the initial attestation key encoded as a string
 *
 *  @var plat_otp_layout_t::boot_seed
 *  This field is implemented in the CC312 user-area. It is used in TF-M to
 *  store the boot seed
 *
 *  @var plat_otp_layout_t::implementation_id
 *  This field is implemented in the CC312 user-area. It is used in TF-M to
 *  store the implementation_id
 *
 *  @var plat_otp_layout_t::hw_version
 *  This field is implemented in the CC312 user-area. It is used in TF-M to
 *  store the hw version
 *
 *  @var plat_otp_layout_t::verification_service_url
 *  This field is implemented in the CC312 user-area. It is used in TF-M to
 *  store the verification_service_url
 *
 *  @var plat_otp_layout_t::profile_definition
 *  This field is implemented in the CC312 user-area. It is used in TF-M to
 *  store the profile_definition
 *
 *  @var plat_otp_layout_t::bl2_rotpk
 *  This field is implemented in the CC312 user-area. It is used in TF-M to
 *  store rotpks used by BL2, encoded as a set of 32 byte public key hashes.
 *
 *  @var plat_otp_layout_t::bl2_nv_counter
 *  This field is implemented in the CC312 user-area. It is used in TF-M to
 *  store nv counters used by BL2.
 *
 *  @var plat_otp_layout_t::bl1_rotpk
 *  This field is implemented in the CC312 user-area. It is used in TF-M to
 *  store rotpks used by BL1, encoded as a set of 32 byte public key hashes.
 *
 *  @var plat_otp_layout_t::bl1_nv_counter
 *  This field is implemented in the CC312 user-area. It is used in TF-M to
 *  store nv counters used by BL1.
 */

__PACKED_STRUCT plat_otp_layout_t {
    uint8_t huk[32];
    uint8_t icv_provisioning_key[16];
    uint8_t icv_code_encryption_key[16];
    __PACKED_STRUCT {
        uint8_t huk_zero_bits;
        uint8_t icv_provisioning_key_zero_bits;
        uint8_t icv_code_encryption_key_zero_bits;
        uint8_t icv_rot_public_key_zero_bits;
    } icv_programmed_flags;
    __PACKED_UNION {
        /* rot_public_key is used for the IAK */
        uint8_t rot_public_key[32];
        __PACKED_STRUCT {
            uint8_t icv_rot_public_key[16];
            uint8_t oem_rot_public_key[16];
        };
    };
    uint8_t oem_provisioning_secret[16];
    uint8_t oem_code_encryption_key[16];
    __PACKED_STRUCT {
        uint8_t oem_rot_public_key_zero_bits;
        uint8_t oem_provisioning_secret_zero_bits;
        uint8_t oem_code_encryption_key_zero_bits;
        uint8_t rma_flags;
    } oem_programmed_flags;
    __PACKED_UNION {
        uint8_t nv_counter[20];
        __PACKED_STRUCT {
            uint8_t icv_nv_counter[8];
            uint8_t oem_nv_counter[12];
        };
    };
    uint8_t general_purpose_configuration_flags[4];
    uint8_t dcu_debug_lock_mask[16];
    /* The user area starts here */
    __PACKED_STRUCT {
        __PACKED_STRUCT{
            uint16_t boot_seed_zero_bits;
            uint16_t implementation_id_zero_bits;
            uint16_t hw_version_zero_bits;
            uint16_t verification_service_url_zero_bits;
            uint16_t profile_definition_zero_bits;
            uint16_t iak_len_zero_bits;
            uint16_t iak_type_zero_bits;
            uint16_t iak_id_zero_bits;
            uint16_t bl2_rotpk_zero_bits[3];
#ifdef BL1
            uint16_t bl1_rotpk_0_zero_bits;
#ifdef PLATFORM_DEFAULT_BL1
            uint16_t bl2_encryption_key_zero_bits;
            uint16_t bl1_2_image_hash_zero_bits;
            uint16_t bl2_image_hash_zero_bits;
#endif /* PLATFORM_DEFAULT_BL1 */
#endif /* BL1 */
            uint16_t secure_debug_pk_zero_bits;
        };

        uint8_t iak_len[4];
        uint8_t iak_type[4];
        uint8_t iak_id[32];

        uint8_t boot_seed[32];
        uint8_t implementation_id[32];
        uint8_t hw_version[32];
        uint8_t verification_service_url[32];
        uint8_t profile_definition[32];

        uint8_t bl2_rotpk[3][32];
        uint8_t bl2_nv_counter[3][64];

#ifdef BL1
        uint8_t bl1_rotpk_0[32];
        uint8_t bl1_nv_counter[16];
#ifdef PLATFORM_DEFAULT_BL1
        uint8_t bl2_encryption_key[32];
        uint8_t bl1_2_image_hash[32];
        uint8_t bl2_image_hash[32];

        uint8_t bl1_2_image[BL1_2_CODE_SIZE];
#endif /* PLATFORM_DEFAULT_BL1 */
#endif /* BL1 */

        uint8_t secure_debug_pk[32];
    };
};

enum cc312_lifecycle_t {
    CC312_LCS_CM  = 0x0,
    CC312_LCS_DM  = 0x1,
    CC312_LCS_SE  = 0x5,
    CC312_LCS_RMA = 0x7,
};

static inline struct plat_otp_layout_t *get_cc312_otp_ptr(void){
    return (struct plat_otp_layout_t*)(DX_BASE_CC + CC312_OTP_BASE_OFFSET);
}



static enum tfm_plat_err_t otp_read(const uint8_t *addr, size_t item_size,
                                    size_t out_len, uint8_t *out)
{
    uint32_t* word_ptr;
    uint32_t word;
    uint32_t start_offset;
    size_t out_done;
    size_t copy_size;
    size_t total_copy_size = item_size < out_len ? item_size : out_len;

    /* CC312 OTP can only be read / written in 32 bit words. In order to allow
     * access to unaligned uint8_t pointers (as per the function definition):
     *
     * First calculate nearest aligned pointer (by dropping the last two bits of
     * the OTP address), and the offset between the OTP address and that aligned
     * pointer. Load the word from the aligned pointer, and copy the part of
     * that word after the offset into the output pointer using memcpy (since
     * the output pointer has no alignment requirements).
     *
     * After the first word, all alignment issues will have been resolved. The
     * only other nuance is making sure we copy the right amount of bytes from
     * the last word. This is checked for all words, in case the first word is
     * also the last word.
     */
    for(out_done = 0; out_done < total_copy_size;) {
        start_offset = ((uint32_t)addr + out_done) & 0x3;
        word_ptr = (uint32_t*)(addr + out_done - start_offset);

        word = *word_ptr;

        copy_size = sizeof(word) - start_offset;
        if (out_done + copy_size > total_copy_size) {
            copy_size = total_copy_size - out_done;
        }


        memcpy(out + out_done, ((uint8_t*)&word) + start_offset, copy_size);
        out_done += copy_size;
    }

    return TFM_PLAT_ERR_SUCCESS;
}

void wait_until_otp_programming_completes(void) {
    /* Read the AIB_FUSE_PROG_COMPLETED register until it has bit 1 set */
    while (! ( cc_read_reg(AIB_FUSE_PROG_COMPLETED_REG_OFFSET) & 1)) {}
}

static uint32_t count_buffer_zero_bits(const uint8_t* buf, size_t size) {
    size_t byte_index;
    uint8_t byte;
    uint32_t one_count = 0;

    for (byte_index = 0; byte_index < size; byte_index++) {
        byte = buf[byte_index];
        for (int bit_index = 0; bit_index < 8; bit_index++) {
            one_count += (byte >> bit_index) & 1;
        }
    }

    return (size * 8) - one_count;
}

static uint32_t count_otp_zero_bits(const uint8_t* addr, size_t size) {
    uint8_t buf[size];

    otp_read(addr, size, sizeof(buf), buf);
    return count_buffer_zero_bits(buf, size);
}

static enum tfm_plat_err_t otp_write(uint8_t *addr, size_t size,
                                    size_t in_len, const uint8_t *in,
                                    uint8_t* zero_byte_buf)
{
    enum tfm_plat_err_t err;
    uint32_t* word_ptr;
    uint32_t current_word;
    uint32_t word;
    uint32_t start_offset;
    size_t in_done = 0;
    size_t copy_size;
    uint16_t zero_count;

    if (size < in_len) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

    /* First iterate through and check all values are valid (will not require a
     * 1 bit to be unset). See docs below on why certain design choices have
     * been made with regard to alignment.
     */
    for(in_done = 0; in_done < in_len;) {
        start_offset = ((uint32_t)addr + in_done) & 0x3;
        word_ptr = (uint32_t*)(addr + in_done - start_offset);

        current_word = *word_ptr;
        word = 0;

        copy_size = sizeof(word) - start_offset;
        if (in_done + copy_size > in_len) {
            copy_size = in_len - in_done;
        }

        memcpy(((uint8_t*)&word) + start_offset, in + in_done, copy_size);
        word |= current_word;

        if (memcmp(((uint8_t*)&word) + start_offset, in + in_done, copy_size)) {
            return TFM_PLAT_ERR_INVALID_INPUT;
        }

        in_done += copy_size;
    }

    /* If the caller supplied a zero-count buf, set that. Do this before setting
     * the OTP in case the zero_count buffer write fails.
     */
    if (zero_byte_buf != NULL) {
        zero_count = count_buffer_zero_bits(in, in_len);

        err = otp_write(zero_byte_buf, 2, sizeof(zero_count), (uint8_t*)&zero_count, NULL);
        if (err != TFM_PLAT_ERR_SUCCESS) {
            return err;
        }
    }

    /* CC312 OTP can only be read / written in 32 bit words. In order to allow
     * access to unaligned uint8_t pointers (as per the function definition):
     *
     * First calculate nearest aligned pointer (by dropping the last two bits of
     * the OTP address), and the offset between the OTP address and that aligned
     * pointer. Load the word from the aligned pointer, copy from the input
     * pointer into that word at the offset, and then write the updated word
     * back to OTP.
     *
     * After the first word, all alignment issues will have been resolved. The
     * only other nuance is making sure we copy the right amount of bytes into
     * the last word. This is checked for all words, in case the first word is
     * also the last word.
     */
    for(in_done = 0; in_done < in_len;) {
        start_offset = ((uint32_t)addr + in_done) & 0x3;
        word_ptr = (uint32_t*)(addr + in_done - start_offset);

        word = *word_ptr;

        copy_size = sizeof(word) - start_offset;
        if (in_done + copy_size > in_len) {
            copy_size = in_len - in_done;
        }

        memcpy(((uint8_t*)&word) + start_offset, in + in_done, copy_size);

        *word_ptr = word;
        wait_until_otp_programming_completes();

        in_done += copy_size;
    }

    return TFM_PLAT_ERR_SUCCESS;
}

static enum tfm_plat_err_t verify_zero_bits_count(uint8_t *buf, size_t buf_len,
                                                  uint8_t* zero_count_buf)
{
    enum tfm_plat_err_t err;
    uint16_t zero_count;

    if (buf_len > (UINT16_MAX / 8)) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

    err = otp_read(zero_count_buf, 2, sizeof(zero_count), (uint8_t*)&zero_count);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    if (zero_count != count_otp_zero_bits(buf, buf_len)) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    return TFM_PLAT_ERR_SUCCESS;
}

static enum tfm_plat_err_t check_keys_for_tampering(void)
{
    size_t idx;
    enum tfm_plat_err_t err;
    struct plat_otp_layout_t *otp = get_cc312_otp_ptr();

    err = verify_zero_bits_count(otp->boot_seed,
                                 sizeof(otp->boot_seed),
                                 (uint8_t*)&otp->boot_seed_zero_bits);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    err = verify_zero_bits_count(otp->implementation_id,
                                 sizeof(otp->implementation_id),
                                 (uint8_t*)&otp->implementation_id_zero_bits);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    err = verify_zero_bits_count(otp->hw_version,
                                 sizeof(otp->hw_version),
                                 (uint8_t*)&otp->hw_version_zero_bits);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    err = verify_zero_bits_count(otp->verification_service_url,
                                 sizeof(otp->verification_service_url),
                                 (uint8_t*)&otp->verification_service_url_zero_bits);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    err = verify_zero_bits_count(otp->profile_definition,
                                 sizeof(otp->profile_definition),
                                 (uint8_t*)&otp->profile_definition_zero_bits);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    err = verify_zero_bits_count(otp->iak_len,
                                 sizeof(otp->iak_len),
                                 (uint8_t*)&otp->iak_len_zero_bits);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    err = verify_zero_bits_count(otp->iak_type,
                                 sizeof(otp->iak_type),
                                 (uint8_t*)&otp->iak_type_zero_bits);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

#ifdef ATTEST_INCLUDE_COSE_KEY_ID
    err = verify_zero_bits_count(otp->iak_id,
                                 sizeof(otp->iak_id),
                                 (uint8_t*)&otp->iak_id_zero_bits);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }
#endif /* ATTEST_INCLUDE_COSE_KEY_ID */

    for(idx = 0; idx < 3; idx++) {
        err = verify_zero_bits_count(otp->bl2_rotpk[idx],
                                     sizeof(otp->bl2_rotpk[idx]),
                                     (uint8_t*)&otp->bl2_rotpk_zero_bits[idx]);
        if (err != TFM_PLAT_ERR_SUCCESS) {
            return err;
        }
    }

#ifdef BL1
#ifdef PLATFORM_PSA_ADAC_SECURE_DEBUG
    err = verify_zero_bits_count(otp->secure_debug_pk,
                                 sizeof(otp->secure_debug_pk),
                                 (uint8_t*)&otp->secure_debug_pk_zero_bits);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }
#endif

    err = verify_zero_bits_count(otp->bl1_rotpk_0,
                                 sizeof(otp->bl1_rotpk_0),
                                 (uint8_t*)&otp->bl1_rotpk_0_zero_bits);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

#ifdef PLATFORM_DEFAULT_BL1
    err = verify_zero_bits_count(otp->bl2_encryption_key,
                                 sizeof(otp->bl2_encryption_key),
                                 (uint8_t*)&otp->bl2_encryption_key_zero_bits);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    err = verify_zero_bits_count(otp->bl1_2_image_hash,
                                 sizeof(otp->bl1_2_image_hash),
                                 (uint8_t*)&otp->bl1_2_image_hash_zero_bits);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    err = verify_zero_bits_count(otp->bl2_image_hash,
                                 sizeof(otp->bl2_image_hash),
                                 (uint8_t*)&otp->bl2_image_hash_zero_bits);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

#endif /* PLATFORM_DEFAULT_BL1 */
#endif /* BL1 */

    return TFM_PLAT_ERR_SUCCESS;
}

static enum plat_otp_lcs_t map_cc312_lcs_to_otp_lcs(enum cc312_lifecycle_t lcs) {
    switch (lcs) {
    case CC312_LCS_CM:
        return PLAT_OTP_LCS_ASSEMBLY_AND_TEST;
    case CC312_LCS_DM:
        return PLAT_OTP_LCS_PSA_ROT_PROVISIONING;
    case CC312_LCS_SE:
        return PLAT_OTP_LCS_SECURED;
    case CC312_LCS_RMA:
        return PLAT_OTP_LCS_DECOMMISSIONED;
    default:
        return PLAT_OTP_LCS_UNKNOWN;
    }
}

static enum tfm_plat_err_t otp_read_lcs(size_t out_len, uint8_t *out) {
    enum cc312_lifecycle_t cc_lcs;
    enum plat_otp_lcs_t *lcs = (enum plat_otp_lcs_t*) out;

    cc_lcs = cc_read_reg(LCS_REG_REG_OFFSET) & 0x7;

    if (out_len != sizeof(uint32_t)) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

    *lcs = map_cc312_lcs_to_otp_lcs(cc_lcs);

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_plat_otp_init(void)
{
    uint32_t otp_size;
    uint32_t lcs;
    enum tfm_plat_err_t err;


    /* Wait for the NVM system to become idle */
    while (! (cc_read_reg(NVM_IS_IDLE_REG_OFFSET) & 1)) {}

    /* Check that the lifecycle state is valid */
    if (! (cc_read_reg(LCS_IS_VALID_REG_OFFSET) & 1)) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    /* Read the OTP_ADDR_WIDTH_DEF register, and use it to calculate how much
     * OTP is present. ADDR_WIDTH measures the amount of bits that are used to
     * index memory, where each bit indexes a memory word. Because binary
     * indexing is used, the amount of memory words available can be found by
     * doing 2 to the power of the address width.
     *
     * To convert from word to byte addressing (and hence calculate the amount
     * of bytes available, the amount of words is then multiplied by the size of
     * a word in bytes.
     */
    otp_size = (1 << (cc_read_reg(OTP_ADDR_WIDTH_DEF_REG_OFFSET)))
               * sizeof(uint32_t);
    if(sizeof(struct plat_otp_layout_t) > otp_size) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    /* If we are in SE LCS, check keys for tampering. Only applies to keys in
     * the user storage area, since the others are checked for tampering by HW
     */
    otp_read_lcs(sizeof(lcs), (uint8_t*)&lcs);
    if (lcs == PLAT_OTP_LCS_SECURED) {
        err = check_keys_for_tampering();
        if (err != TFM_PLAT_ERR_SUCCESS) {
            return err;
        }
    }

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_plat_otp_read(enum tfm_otp_element_id_t id,
                                      size_t out_len, uint8_t *out)
{
    struct plat_otp_layout_t *otp = get_cc312_otp_ptr();

    switch (id) {
    case PLAT_OTP_ID_HUK:
        return otp_read(otp->huk, sizeof(otp->huk), out_len, out);
    case PLAT_OTP_ID_IAK:
        /* The IAK is stored in the rot_public_key slot */
        return otp_read(otp->rot_public_key,
                        sizeof(otp->rot_public_key), out_len, out);

    case PLAT_OTP_ID_BOOT_SEED:
        return otp_read(otp->boot_seed, sizeof(otp->boot_seed), out_len,
                        out);
    case PLAT_OTP_ID_LCS:
        return otp_read_lcs(out_len, out);
    case PLAT_OTP_ID_IMPLEMENTATION_ID:
        return otp_read(otp->implementation_id,
                        sizeof(otp->implementation_id), out_len, out);
    case PLAT_OTP_ID_HW_VERSION:
        return otp_read(otp->hw_version, sizeof(otp->hw_version), out_len,
                        out);
    case PLAT_OTP_ID_VERIFICATION_SERVICE_URL:
        return otp_read(otp->verification_service_url,
                        sizeof(otp->verification_service_url), out_len,
                        out);
    case PLAT_OTP_ID_PROFILE_DEFINITION:
        return otp_read(otp->profile_definition,
                        sizeof(otp->profile_definition), out_len, out);

    case PLAT_OTP_ID_IAK_LEN:
        return otp_read(otp->iak_len,
                        sizeof(otp->iak_len), out_len, out);
    case PLAT_OTP_ID_IAK_TYPE:
        return otp_read(otp->iak_type,
                        sizeof(otp->iak_type), out_len, out);
    case PLAT_OTP_ID_IAK_ID:
        return otp_read(otp->iak_id,
                        sizeof(otp->iak_id), out_len, out);

    case PLAT_OTP_ID_BL2_ROTPK_0:
        return otp_read(otp->bl2_rotpk[0], sizeof(otp->bl2_rotpk[0]), out_len,
                        out);
    case PLAT_OTP_ID_NV_COUNTER_BL2_0:
        return otp_read(otp->bl2_nv_counter[0],
                        sizeof(otp->bl2_nv_counter[0]), out_len, out);

    case PLAT_OTP_ID_BL2_ROTPK_1:
        return otp_read(otp->bl2_rotpk[1], sizeof(otp->bl2_rotpk[1]), out_len,
                        out);
    case PLAT_OTP_ID_NV_COUNTER_BL2_1:
        return otp_read(otp->bl2_nv_counter[1],
                        sizeof(otp->bl2_nv_counter[1]), out_len, out);

    case PLAT_OTP_ID_BL2_ROTPK_2:
        return otp_read(otp->bl2_rotpk[2], sizeof(otp->bl2_rotpk[2]), out_len,
                        out);
    case PLAT_OTP_ID_NV_COUNTER_BL2_2:
        return otp_read(otp->bl2_nv_counter[2],
                        sizeof(otp->bl2_nv_counter[2]), out_len, out);

#ifdef BL1
    case PLAT_OTP_ID_BL1_ROTPK_0:
        return otp_read(otp->bl1_rotpk_0,
                        sizeof(otp->bl1_rotpk_0), out_len, out);
    case PLAT_OTP_ID_NV_COUNTER_BL1_0:
        return otp_read(otp->bl1_nv_counter,
                        sizeof(otp->bl1_nv_counter), out_len, out);
#ifdef PLATFORM_DEFAULT_BL1
    case PLAT_OTP_ID_KEY_BL2_ENCRYPTION:
        return otp_read(otp->bl2_encryption_key,
                        sizeof(otp->bl2_encryption_key), out_len, out);
    case PLAT_OTP_ID_BL1_2_IMAGE_HASH:
        return otp_read(otp->bl1_2_image_hash,
                        sizeof(otp->bl1_2_image_hash), out_len, out);
    case PLAT_OTP_ID_BL2_IMAGE_HASH:
        return otp_read(otp->bl2_image_hash,
                        sizeof(otp->bl2_image_hash), out_len, out);
    case PLAT_OTP_ID_BL1_2_IMAGE:
        return otp_read(otp->bl1_2_image,
                        sizeof(otp->bl1_2_image), out_len, out);
#endif /* PLATFORM_DEFAULT_BL1 */
#endif /* BL1 */

    case PLAT_OTP_ID_ENTROPY_SEED:
        return TFM_PLAT_ERR_UNSUPPORTED;

    case PLAT_OTP_ID_SECURE_DEBUG_PK:
        return otp_read(otp->secure_debug_pk,
                        sizeof(otp->secure_debug_pk), out_len, out);

    default:
        return TFM_PLAT_ERR_UNSUPPORTED;
    }
}

static enum tfm_plat_err_t otp_write_lcs(size_t in_len, const uint8_t *in)
{
    /* Writing the LCS for the CC312 means doing a HW state transition. We need
     * to reboot to apply this, but first some data about keys we're not using
     * needs to be written.
     */
    uint32_t lcs;
    enum plat_otp_lcs_t new_lcs = *(uint32_t*)in;
    enum tfm_plat_err_t err;
    uint8_t zero_bits;
    struct plat_otp_layout_t *otp = get_cc312_otp_ptr();

    if (in_len != sizeof(lcs)) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

    err = otp_read_lcs(sizeof(lcs), (uint8_t*)&lcs);
    if(err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    if (lcs == PLAT_OTP_LCS_ASSEMBLY_AND_TEST) {
        if (new_lcs != PLAT_OTP_LCS_PSA_ROT_PROVISIONING) {
            return TFM_PLAT_ERR_INVALID_INPUT;
        }

        /* Test to see if the HUK has been written yet, as this is the trigger
         * for the LCS transition. Test to see if the zero count has been
         * written.
         */
        err = otp_read(&otp->icv_programmed_flags.huk_zero_bits,
                       sizeof(otp->icv_programmed_flags.huk_zero_bits),
                       sizeof(zero_bits), &zero_bits);
        if (err != TFM_PLAT_ERR_SUCCESS) {
            return err;
        }
        if (zero_bits == 0) {
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }

        /* Set the "not used" bit" for each key */
        zero_bits = 0x80;

        err = otp_write(&otp->icv_programmed_flags.icv_provisioning_key_zero_bits,
                       sizeof(otp->icv_programmed_flags.icv_provisioning_key_zero_bits),
                       sizeof(zero_bits), &zero_bits, NULL);
        if (err != TFM_PLAT_ERR_SUCCESS) {
            return err;
        }

        err = otp_write(&otp->icv_programmed_flags.icv_code_encryption_key_zero_bits,
                       sizeof(otp->icv_programmed_flags.icv_code_encryption_key_zero_bits),
                       sizeof(zero_bits), &zero_bits, NULL);
        if (err != TFM_PLAT_ERR_SUCCESS) {
            return err;
        }

        err = otp_write(&otp->icv_programmed_flags.icv_rot_public_key_zero_bits,
                       sizeof(otp->icv_programmed_flags.icv_rot_public_key_zero_bits),
                       sizeof(zero_bits), &zero_bits, NULL);
        if (err != TFM_PLAT_ERR_SUCCESS) {
            return err;
        }
    } else if (lcs == PLAT_OTP_LCS_PSA_ROT_PROVISIONING) {
        if (new_lcs != PLAT_OTP_LCS_SECURED) {
            return TFM_PLAT_ERR_INVALID_INPUT;
        }

        /* Test to see if the IAK has been written yet, as this is the trigger
         * for the LCS transition. Test to see if the zero count has been
         * written.
         */
        err = otp_read(&otp->oem_programmed_flags.oem_rot_public_key_zero_bits,
                       sizeof(otp->oem_programmed_flags.oem_rot_public_key_zero_bits),
                       sizeof(zero_bits), &zero_bits);
        if (err != TFM_PLAT_ERR_SUCCESS) {
            return err;
        }
        if (zero_bits == 0) {
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }

        /* Set the "not used" bit" for each key */
        zero_bits = 0x80;

        err = otp_write(&otp->oem_programmed_flags.oem_provisioning_secret_zero_bits,
                       sizeof(otp->oem_programmed_flags.oem_provisioning_secret_zero_bits),
                       sizeof(zero_bits), &zero_bits, NULL);
        if (err != TFM_PLAT_ERR_SUCCESS) {
            return err;
        }

        err = otp_write(&otp->oem_programmed_flags.oem_code_encryption_key_zero_bits,
                       sizeof(otp->oem_programmed_flags.oem_code_encryption_key_zero_bits),
                       sizeof(zero_bits), &zero_bits, NULL);
        if (err != TFM_PLAT_ERR_SUCCESS) {
            return err;
        }
    } else {
        /* TODO transitions for the other LCSes */
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

    NVIC_SystemReset();
}

enum tfm_plat_err_t tfm_plat_otp_write(enum tfm_otp_element_id_t id,
                                       size_t in_len, const uint8_t *in)
{
    struct plat_otp_layout_t *otp = get_cc312_otp_ptr();

    switch (id) {
    case PLAT_OTP_ID_HUK:
        return otp_write(otp->huk, sizeof(otp->huk), in_len, in,
                         (uint8_t*)&otp->icv_programmed_flags.huk_zero_bits);
    case PLAT_OTP_ID_IAK:
        /* The IAK is stored in the rot_public_key slot */
        return otp_write(otp->rot_public_key, sizeof(otp->rot_public_key),
                         in_len, in,
                         (uint8_t*)&otp->oem_programmed_flags.oem_rot_public_key_zero_bits);

    case PLAT_OTP_ID_BOOT_SEED:
        return otp_write(otp->boot_seed, sizeof(otp->boot_seed),
                         in_len, in, (uint8_t*)&otp->boot_seed_zero_bits);
    case PLAT_OTP_ID_LCS:
        return otp_write_lcs(in_len, in);
    case PLAT_OTP_ID_IMPLEMENTATION_ID:
        return otp_write(otp->implementation_id,
                         sizeof(otp->implementation_id), in_len, in,
                         (uint8_t*)&otp->implementation_id_zero_bits);
    case PLAT_OTP_ID_HW_VERSION:
        return otp_write(otp->hw_version, sizeof(otp->hw_version), in_len,
                         in, (uint8_t*)&otp->hw_version_zero_bits);
    case PLAT_OTP_ID_VERIFICATION_SERVICE_URL:
        return otp_write(otp->verification_service_url,
                         sizeof(otp->verification_service_url), in_len, in,
                         (uint8_t*)&otp->verification_service_url_zero_bits);
    case PLAT_OTP_ID_PROFILE_DEFINITION:
        return otp_write(otp->profile_definition,
                         sizeof(otp->profile_definition), in_len,
                         in, (uint8_t*)&otp->profile_definition_zero_bits);

    case PLAT_OTP_ID_IAK_LEN:
        return otp_write(otp->iak_len,
                         sizeof(otp->iak_len), in_len,
                         in, (uint8_t*)&otp->iak_len_zero_bits);
    case PLAT_OTP_ID_IAK_TYPE:
        return otp_write(otp->iak_type,
                         sizeof(otp->iak_type), in_len,
                         in, (uint8_t*)&otp->iak_type_zero_bits);
    case PLAT_OTP_ID_IAK_ID:
        return otp_write(otp->iak_id,
                         sizeof(otp->iak_id), in_len,
                         in, (uint8_t*)&otp->iak_id_zero_bits);

    case PLAT_OTP_ID_BL2_ROTPK_0:
        return otp_write(otp->bl2_rotpk[0], sizeof(otp->bl2_rotpk[0]), in_len,
                         in, (uint8_t*)&otp->bl2_rotpk_zero_bits[0]);
    case PLAT_OTP_ID_NV_COUNTER_BL2_0:
        return otp_write(otp->bl2_nv_counter[0],
                         sizeof(otp->bl2_nv_counter[0]), in_len, in, NULL);

    case PLAT_OTP_ID_BL2_ROTPK_1:
        return otp_write(otp->bl2_rotpk[1], sizeof(otp->bl2_rotpk[1]), in_len,
                         in, (uint8_t*)&otp->bl2_rotpk_zero_bits[1]);
    case PLAT_OTP_ID_NV_COUNTER_BL2_1:
        return otp_write(otp->bl2_nv_counter[1],
                         sizeof(otp->bl2_nv_counter[1]), in_len, in, NULL);

    case PLAT_OTP_ID_BL2_ROTPK_2:
        return otp_write(otp->bl2_rotpk[2], sizeof(otp->bl2_rotpk[2]), in_len,
                         in, (uint8_t*)&otp->bl2_rotpk_zero_bits[2]);
    case PLAT_OTP_ID_NV_COUNTER_BL2_2:
        return otp_write(otp->bl2_nv_counter[2],
                         sizeof(otp->bl2_nv_counter[2]), in_len, in, NULL);

#ifdef BL1
    case PLAT_OTP_ID_BL1_ROTPK_0:
        return otp_write(otp->bl1_rotpk_0, sizeof(otp->bl1_rotpk_0), in_len, in,
                         (uint8_t*)&otp->bl1_rotpk_0_zero_bits);
    case PLAT_OTP_ID_NV_COUNTER_BL1_0:
        return otp_write(otp->bl1_nv_counter,
                         sizeof(otp->bl1_nv_counter), in_len, in, NULL);
#ifdef PLATFORM_DEFAULT_BL1
    case PLAT_OTP_ID_KEY_BL2_ENCRYPTION:
        return otp_write(otp->bl2_encryption_key,
                         sizeof(otp->bl2_encryption_key), in_len, in,
                         (uint8_t*)&otp->bl2_encryption_key_zero_bits);
    case PLAT_OTP_ID_BL1_2_IMAGE_HASH:
        return otp_write(otp->bl1_2_image_hash,
                         sizeof(otp->bl1_2_image_hash), in_len, in,
                         (uint8_t*)&otp->bl1_2_image_hash_zero_bits);
    case PLAT_OTP_ID_BL2_IMAGE_HASH:
        return otp_write(otp->bl2_image_hash,
                         sizeof(otp->bl2_image_hash), in_len, in,
                         (uint8_t*)&otp->bl2_image_hash_zero_bits);
    case PLAT_OTP_ID_BL1_2_IMAGE:
        return otp_write(otp->bl1_2_image,
                         sizeof(otp->bl1_2_image), in_len, in, NULL);
#endif /* PLATFORM_DEFAULT_BL1 */
#endif /* BL1 */

    case PLAT_OTP_ID_ENTROPY_SEED:
        return TFM_PLAT_ERR_UNSUPPORTED;

    case PLAT_OTP_ID_SECURE_DEBUG_PK:
        return otp_write(otp->secure_debug_pk,
                         sizeof(otp->secure_debug_pk), in_len, in,
                         (uint8_t*)&otp->secure_debug_pk_zero_bits);

    default:
        return TFM_PLAT_ERR_UNSUPPORTED;
    }
}


enum tfm_plat_err_t tfm_plat_otp_get_size(enum tfm_otp_element_id_t id,
                                          size_t *size)
{
    struct plat_otp_layout_t *otp = get_cc312_otp_ptr();

    switch (id) {
    case PLAT_OTP_ID_HUK:
        *size = sizeof(otp->huk);
        break;
    case PLAT_OTP_ID_IAK:
        /* The IAK is stored in the rot_public_key slot */
        *size = sizeof(otp->rot_public_key);
        break;

    case PLAT_OTP_ID_BOOT_SEED:
        *size = sizeof(otp->boot_seed);
        break;
    case PLAT_OTP_ID_LCS:
        *size = sizeof(uint32_t);
        break;
    case PLAT_OTP_ID_IMPLEMENTATION_ID:
        *size = sizeof(otp->implementation_id);
        break;
    case PLAT_OTP_ID_HW_VERSION:
        *size = sizeof(otp->hw_version);
        break;
    case PLAT_OTP_ID_VERIFICATION_SERVICE_URL:
        *size = sizeof(otp->verification_service_url);
        break;
    case PLAT_OTP_ID_PROFILE_DEFINITION:
        *size = sizeof(otp->profile_definition);
        break;

    case PLAT_OTP_ID_IAK_LEN:
        *size = sizeof(otp->iak_len);
        break;
    case PLAT_OTP_ID_IAK_TYPE:
        *size = sizeof(otp->iak_type);
        break;
    case PLAT_OTP_ID_IAK_ID:
        *size = sizeof(otp->iak_id);
        break;

    case PLAT_OTP_ID_BL2_ROTPK_0:
        *size = sizeof(otp->bl2_rotpk[0]);
        break;
    case PLAT_OTP_ID_NV_COUNTER_BL2_0:
        *size = sizeof(otp->bl2_nv_counter[0]);
        break;

    case PLAT_OTP_ID_BL2_ROTPK_1:
        *size = sizeof(otp->bl2_rotpk[1]);
        break;
    case PLAT_OTP_ID_NV_COUNTER_BL2_1:
        *size = sizeof(otp->bl2_nv_counter[1]);
        break;

    case PLAT_OTP_ID_BL2_ROTPK_2:
        *size = sizeof(otp->bl2_rotpk[2]);
        break;
    case PLAT_OTP_ID_NV_COUNTER_BL2_2:
        *size = sizeof(otp->bl2_nv_counter[2]);
        break;

#ifdef BL1
    case PLAT_OTP_ID_BL1_ROTPK_0:
        *size = sizeof(otp->bl1_rotpk_0);
        break;
    case PLAT_OTP_ID_NV_COUNTER_BL1_0:
        *size = sizeof(otp->bl1_nv_counter);
        break;
#ifdef PLATFORM_DEFAULT_BL1
    case PLAT_OTP_ID_KEY_BL2_ENCRYPTION:
        *size = sizeof(otp->bl2_encryption_key);
        break;
    case PLAT_OTP_ID_BL1_2_IMAGE_HASH:
        *size = sizeof(otp->bl1_2_image_hash);
        break;
    case PLAT_OTP_ID_BL2_IMAGE_HASH:
        *size = sizeof(otp->bl2_image_hash);
        break;
    case PLAT_OTP_ID_BL1_2_IMAGE:
        *size = sizeof(otp->bl1_2_image);
        break;
#endif /* PLATFORM_DEFAULT_BL1 */
#endif

    case PLAT_OTP_ID_ENTROPY_SEED:
        return TFM_PLAT_ERR_UNSUPPORTED;

    case PLAT_OTP_ID_SECURE_DEBUG_PK:
        *size = sizeof(otp->secure_debug_pk);
        break;

    default:
        return TFM_PLAT_ERR_UNSUPPORTED;
    }

    return TFM_PLAT_ERR_SUCCESS;
}
