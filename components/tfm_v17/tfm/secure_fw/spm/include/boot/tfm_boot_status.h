/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_BOOT_STATUS_H__
#define __TFM_BOOT_STATUS_H__

#include <stdint.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

/* Major numbers (4 bit) to identify
 * the consumer of shared data in runtime SW
 */
#define TLV_MAJOR_CORE     0x0
#define TLV_MAJOR_IAS      0x1
#define TLV_MAJOR_FWU      0x2
#define TLV_MAJOR_MBS      0x3

/**
 * The shared data between boot loader and runtime SW is TLV encoded. The
 * shared data is stored in a well known location in secure memory and this is
 * a contract between boot loader and runtime SW.
 *
 * The structure of shared data must be the following:
 *  - At the beginning there must be a header: struct shared_data_tlv_header
 *    This contains a magic number and a size field which covers the entire
 *    size of the shared data area including this header.
 *  - After the header there come the entries which are composed from an entry
 *    header structure: struct shared_data_tlv_entry and the data. In the entry
 *    header is a type field (tlv_type) which identify the consumer of the
 *    entry in the runtime SW and specify the subtype of that data item. There
 *    is a size field (tlv_len) which covers the length of the data
 *    (not including the entry header structure). After this structure comes
 *    the actual data.
 *  - Arbitrary number and size of data entry can be in the shared memory area.
 *
 * This table gives of overview about the tlv_type field in the entry header.
 * The tlv_type always composed from a major and minor number. Major number
 * identifies the addressee in runtime SW, who should process the data entry.
 * Minor number used to encode more info about the data entry. The actual
 * definition of minor number could change per major number. In case of boot
 * status data, which is going to be processed by initial attestation service
 * the minor number is split further to two part: sw_module and claim. The
 * sw_module identifies the SW component in the system which the data item
 * belongs to and the claim part identifies the exact type of the data.
 *
 * |---------------------------------------|
 * |            tlv_type (16)              |
 * |---------------------------------------|
 * | tlv_major(4)|      tlv_minor(12)      |
 * |---------------------------------------|
 * | MAJOR_IAS   | sw_module(6) | claim(6) |
 * |---------------------------------------|
 * | MAJOR_MBS   | slot ID  (6) | claim(6) |
 * |---------------------------------------|
 * | MAJOR_CORE  |          TBD            |
 * |---------------------------------------|
 */

/* Initial attestation: SW components / SW modules
 * This list is intended to be adjusted per device. It contains more SW
 * components than currently available in TF-M project. It serves as an example,
 * what kind of SW components might be available.
 */
#define SW_GENERAL     0x00
#define SW_BL2         0x01
#define SW_PROT        0x02
#define SW_AROT        0x03
#define SW_SPE         0x04
#define SW_NSPE        0x05
#define SW_S_NS        0x06
#define SW_MAX         0x07

/* Initial attestation and Measured boot service: claim per SW components. */
/* Bits: 0-2 */
#define SW_VERSION       0x00
#define SW_SIGNER_ID     0x01
/* Reserved              0x02 */
#define SW_TYPE          0x03
/* Bits: 3-5 */
#define SW_MEASURE_VALUE                0x08
#define SW_MEASURE_TYPE                 0x09
#define SW_MEASURE_METADATA             0x0A
#define SW_MEASURE_VALUE_NON_EXTENDABLE 0x0B
#define SW_BOOT_RECORD                  0x3F

/* Initial attestation: General claim does not belong any particular SW
 * component. But they might be part of the boot status.
 */
#define BOOT_SEED          0x00
#define CERT_REF           0x01
#define SECURITY_LIFECYCLE 0x02

/* General macros to handle TLV type */
#define MAJOR_MASK 0xF     /* 4  bit */
#define MAJOR_POS  12      /* 12 bit */
#define MINOR_MASK 0xFFF   /* 12 bit */
#define MINOR_POS  0

#define MASK_LEFT_SHIFT(data, mask, position)  \
                    (((data) & (mask)) << (position))
#define MASK_RIGHT_SHIFT(data, mask, position) \
                    ((uint16_t)((data) & (mask)) >> (position))

#define SET_TLV_TYPE(major, minor) \
                    (MASK_LEFT_SHIFT(major, MAJOR_MASK, MAJOR_POS) | \
                     MASK_LEFT_SHIFT(minor, MINOR_MASK, MINOR_POS))
#define GET_MAJOR(tlv_type) ((tlv_type) >> MAJOR_POS)
#define GET_MINOR(tlv_type) ((tlv_type) &  MINOR_MASK)

/* Initial attestation, Measured boot and Firmware Update common macros */
#define MODULE_POS  6       /* 6 bit */
#define MODULE_MASK 0x3F    /* 6 bit */
#define CLAIM_POS   0
#define CLAIM_MASK  0x3F    /* 6 bit */

/* Initial attestation specific macros */
#define MEASUREMENT_CLAIM_POS 3    /* 3 bit */

#define GET_IAS_MODULE(tlv_type) \
                  MASK_RIGHT_SHIFT(tlv_type, MINOR_MASK, MODULE_POS)
#define GET_IAS_CLAIM(tlv_type) \
                  MASK_RIGHT_SHIFT(tlv_type, CLAIM_MASK, CLAIM_POS)
#define GET_IAS_MEASUREMENT_CLAIM(ias_claim) \
                  MASK_RIGHT_SHIFT(ias_claim, CLAIM_MASK, MEASUREMENT_CLAIM_POS)
#define SET_IAS_MINOR(sw_module, claim) \
                  (MASK_LEFT_SHIFT(sw_module, MODULE_MASK, MODULE_POS) | \
                   MASK_LEFT_SHIFT(claim, CLAIM_MASK, CLAIM_POS))

/* Measured boot specific macros */
#define SLOT_ID_POS  6
#define SLOT_ID_MASK 0x3F   /* 6 bit */

#define GET_MBS_SLOT(tlv_type) \
                    MASK_RIGHT_SHIFT(tlv_type, MINOR_MASK, SLOT_ID_POS)
#define GET_MBS_CLAIM(tlv_type) \
                    MASK_RIGHT_SHIFT(tlv_type, CLAIM_MASK, CLAIM_POS)
#define SET_MBS_MINOR(slot_index, claim) \
                    (MASK_LEFT_SHIFT(slot_index, SLOT_ID_MASK, SLOT_ID_POS) | \
                     MASK_LEFT_SHIFT(claim, CLAIM_MASK, CLAIM_POS))

/* Firmware Update specific macros */
#define GET_FWU_MODULE(tlv_type) \
                    MASK_RIGHT_SHIFT(tlv_type, MINOR_MASK, MODULE_POS)
#define GET_FWU_CLAIM(tlv_type) \
                    MASK_RIGHT_SHIFT(tlv_type, CLAIM_MASK, CLAIM_POS)
#define SET_FWU_MINOR(sw_module, claim)  \
                    (MASK_LEFT_SHIFT(sw_module, MODULE_MASK, MODULE_POS) | \
                     MASK_LEFT_SHIFT(claim, CLAIM_MASK, CLAIM_POS))

/* Magic value which marks the beginning of shared data area in memory */
#define SHARED_DATA_TLV_INFO_MAGIC    0x2016

/**
 * Shared data TLV header.  All fields in little endian.
 *
 *    -----------------------------------
 *    | tlv_magic(16) | tlv_tot_len(16) |
 *    -----------------------------------
 */
struct shared_data_tlv_header {
    uint16_t tlv_magic;
    uint16_t tlv_tot_len; /* size of whole TLV area (including this header) */
};

#define SHARED_DATA_HEADER_SIZE sizeof(struct shared_data_tlv_header)

/**
 * Shared data TLV entry header format. All fields in little endian.
 *
 *    -------------------------------
 *    | tlv_type(16) |  tlv_len(16) |
 *    -------------------------------
 *    |         Raw data            |
 *    -------------------------------
 */
struct shared_data_tlv_entry {
    uint16_t tlv_type;
    uint16_t tlv_len; /* size of single TLV entry (not including this header) */
};

/**
 * \struct tfm_boot_data
 *
 * \brief Store the data for the runtime SW
 */
struct tfm_boot_data {
    struct shared_data_tlv_header header;
    uint8_t data[];
};

#define SHARED_DATA_ENTRY_HEADER_SIZE sizeof(struct shared_data_tlv_entry)
#define SHARED_DATA_ENTRY_SIZE(size) (size + SHARED_DATA_ENTRY_HEADER_SIZE)

#ifdef __cplusplus
}
#endif

#endif /* __TFM_BOOT_STATUS_H__ */
