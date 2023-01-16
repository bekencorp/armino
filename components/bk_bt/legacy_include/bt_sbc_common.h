// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once


#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdint.h>

#define BT_SBC_SYNCWORD                    0x9C        /**< SBC synchronize word             */
#define BT_MSBC_SYNCWORD                   0xAD        /**< MSBC synchronize word            */
#define BT_SBC_SAMPLE_RATE_16000           0x0         /**< SBC sampling frequency : 16.0KHz */ //define conflict
#define BT_SBC_SAMPLE_RATE_32000           0x1         /**< SBC sampling frequency : 32.0KHz */
#define BT_SBC_SAMPLE_RATE_44100           0x2         /**< SBC sampling frequency : 44.1KHz */
#define BT_SBC_SAMPLE_RATE_48000           0x3         /**< SBC sampling frequency : 48.0KHz */
#define BT_SBC_BLOCKS_4                    0x0         /**< SBC blocks number 4              */
#define BT_SBC_BLOCKS_8                    0x1         /**< SBC blocks number 8              */
#define BT_SBC_BLOCKS_12                   0x2         /**< SBC blocks number 12             */
#define BT_SBC_BLOCKS_16                   0x3         /**< SBC blocks number 16             */
#define BT_SBC_CH_MODE_MONO                0x0         /**< SBC channel mode : MONO          */
#define BT_SBC_CH_MODE_DUAL_CHANNEL        0x1         /**< SBC channel mode : Dual Channels */
#define BT_SBC_CH_MODE_STEREO              0x2         /**< SBC channel mode : Stereo        */
#define BT_SBC_CH_MODE_JOINT_STEREO        0x3         /**< SBC channel mode : Joint Stereo  */
#define BT_SBC_ALLOCATION_METHOD_LOUDNESS  0x0         /**< SBC allocation method : Loudness */ //define conflict
#define BT_SBC_ALLOCATION_METHOD_SNR       0x1         /**< SBC allocation method : SNR      */
#define BT_SBC_SUBBANDS_4                  0x0         /**< SBC subbands number 4            */
#define BT_SBC_SUBBANDS_8                  0x1         /**< SBC subbands number 8            */
#define BT_SBC_HEADER_MAX_SZIE              13         /**< SBC header max size 13 in bytes = (8+2+2+2+1+1+8+8+8*1+2*8*4)/8 */
#define BT_SBC_MAX_FRAME_SIZE              513         /**< SBC max frame size 513 in bytes = 4+(4*8*2)/8+(8+16*250)/8      */

#define SCALE_PROTO4_TBL                (15)
#define SCALE_ANA4_TBL                  (17)
#define SCALE_PROTO8_TBL                (16)
#define SCALE_ANA8_TBL                  (17)
#define SCALE_SPROTO4_TBL               (12)
#define SCALE_SPROTO8_TBL               (14)
#define SCALE_NPROTO4_TBL               (11)
#define SCALE_NPROTO8_TBL               (11)
#define SCALE4_STAGE1_BITS              (15)
#define SCALE4_STAGE2_BITS              (15)
#define SCALE8_STAGE1_BITS              (15)
#define SCALE8_STAGE2_BITS              (15)
#define SCALE4_STAGED1_BITS             (15)
#define SCALE4_STAGED2_BITS             (16)
#define SCALE8_STAGED1_BITS             (15)
#define SCALE8_STAGED2_BITS             (16)

#define ASR(val, bits)                  ((int32_t)(val)) >> (bits)
#define SP4(val)                        ASR(val, SCALE_PROTO4_TBL)
#define SA4(val)                        ASR(val, SCALE_ANA4_TBL)
#define SP8(val)                        ASR(val, SCALE_PROTO8_TBL)
#define SA8(val)                        ASR(val, SCALE_ANA8_TBL)
#define SS4(val)                        ASR(val, SCALE_SPROTO4_TBL)
#define SS8(val)                        ASR(val, SCALE_SPROTO8_TBL)
#define SN4(val)                        ASR(val, SCALE_NPROTO4_TBL)
#define SN8(val)                        ASR(val, SCALE_NPROTO8_TBL)
#define SCALE4_STAGE1(src)              ASR(src, SCALE4_STAGE1_BITS)
#define SCALE4_STAGE2(src)              ASR(src, SCALE4_STAGE2_BITS)
#define SCALE8_STAGE1(src)              ASR(src, SCALE8_STAGE1_BITS)
#define SCALE8_STAGE2(src)              ASR(src, SCALE8_STAGE2_BITS)
#define SCALE4_STAGED1(src)             ASR(src, SCALE4_STAGED1_BITS)
#define SCALE4_STAGED2(src)             ASR(src, SCALE4_STAGED2_BITS)
#define SCALE8_STAGED1(src)             ASR(src, SCALE8_STAGED1_BITS)
#define SCALE8_STAGED2(src)             ASR(src, SCALE8_STAGED2_BITS)

#ifndef inline
#define inline __inline
#endif//inline

/**
 * @brief SBC decoder context
 */
typedef struct _SbcCommonContext
{
    int8_t   blocks;                    /**< block number       */
    int8_t   subbands;                  /**< subbands number    */
    uint8_t  join;                      /**< bit number x set means joint stereo has been used in sub-band x */
    uint8_t  bitpool;                   /**< indicate the size of the bit allocation pool that has been used for encoding the stream */

    int8_t   channel_mode;              /**< channel mode       */
    int8_t   sample_rate_index;         /**< sample rate index, 0:16000, 1:32000, 2:44100, 3:48000 */
    int8_t   allocation_method;         /**< allocation method  */
    int8_t   reserved8;                 /**< dummy, reserved for byte align */

    int8_t   bits[2][8];                /**< calculate result by bit allocation. */

    int8_t   scale_factor[2][8];        /**< only the lower 4 bits of every element are to be used */

    int32_t  mem[2][8];                 /**< Memory used as bit need and levels */

}SbcCommonContext;

/**
 * @brief SBC frame header context
 */
typedef struct _SbcFrameHeader
{
    #if defined(__BIG_ENDIAN__)
    //big endianness
    uint32_t crc_check          :8;
    uint32_t bitpool            :8;
    uint32_t subband_mode       :1;
    uint32_t allocation_method  :1;
    uint32_t channel_mode       :2;
    uint32_t block_mode         :2;
    uint32_t sample_rate_index  :2;
    uint32_t syncword           :8;
    #else
    //little endianness
    uint32_t syncword           :8;
    uint32_t subband_mode       :1;
    uint32_t allocation_method  :1;
    uint32_t channel_mode       :2;
    uint32_t block_mode         :2;
    uint32_t sample_rate_index  :2;
    uint32_t bitpool            :8;
    uint32_t crc_check          :8;
    #endif
}SbcFrameHeader;

/**
 * @brief  Get sample rate by index
 * @param  idx sample rate index, 0:16000, 1:32000, 2:44100, 3:48000
 * @return sample rate
 */
uint16_t sbc_comm_sample_rate_get(uint32_t idx);

/**
 * @brief  CRC8 calculation
 * @param  data  data buffer to do calculation
 * @param  len   data buffer length in bits
 * @return CRC8 value
 */
uint8_t  sbc_common_crc8(const uint8_t* data, uint32_t len);

/**
 * @brief  SBC bit allocation calculate for both encoder and decoder
 * @param  sbc SBC common context pointer
 * @return NULL
 */
void sbc_common_bit_allocation(SbcCommonContext* sbc);

#ifdef __cplusplus
}
#endif//__cplusplus


