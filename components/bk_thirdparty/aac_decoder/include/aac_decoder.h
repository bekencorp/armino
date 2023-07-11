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


#ifndef __AAC_DECODER_API_H__
#define __AAC_DECODER_API_H__

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdint.h>

/**
 * @brief  Get AAC decoder context size
 * @return AAC decoder context size
 */
uint32_t aac_decoder_get_ram_size(void);

/**
 * @brief  Get AAC decoder context size without input buffer,
 *         can be used for bluetooth stream decoding
 * @return AAC decoder context size without input buffer
 */
uint32_t aac_decoder_get_ram_size_without_in_buffer(void);

/**
 * @brief  Get sample rate via AAC decoder handle
 * @return sample rate
 */
uint32_t aac_decoder_get_sample_rate(void* aac_decoder);

/**
 * @brief  Get channels via AAC decoder handle
 * @return channels
 */
uint32_t aac_decoder_get_num_channels(void* aac_decoder);

/**
 * @brief  Get decoded PCM samples count via AAC decoder handle
 * @return decoded PCM samples count
 */
uint32_t aac_decoder_get_pcm_samples(void* aac_decoder);

/**
 * @brief  Get decoded PCM samples buffer via AAC decoder handle
 * @return decoded PCM samples buffer
 */
uint8_t* aac_decoder_get_pcm_buffer(void* aac_decoder);

/**
 * @brief  Get filled buffer count in bytes via AAC decoder handle
 * @return filled buffer count in bytes
 */
uint32_t aac_decoder_get_filled_buffer(void* aac_decoder);

/**
 * @brief  Get duration in milli-second via AAC decoder handle
 * @return duration in milli-second
 */
uint32_t aac_decoder_get_duration_in_ms(void* aac_decoder);

/**
 * @brief  Get last error code via AAC decoder handle
 * @return last error code
 */
int32_t  aac_decoder_get_error_code(void* aac_decoder);

/**
 * @brief  Initialize AAC decoder
 * @param  aac_decoder AAC decoder handle
 * @param  handle AAC file/stream handle
 * @param  read AAC file/stream read function pointer, declared as below
 *         uint32_t (*read)(void *buf, uint32_t size, uint32_t nmemb, void *handle);
 * @param  seek AAC file/stream seek function pointer, declared as below
 *         int32_t (*seek)(void *handle, int32_t offset, int32_t whence);
 * @param  tell AAC file/stream tell function pointer, declared as below
 *         uint32_t (*tell)(void *handle);
 * @return initialize result, 0: success, others: failed with error code.
 */
int32_t aac_decoder_initialize(void* aac_decoder, void* handle, void* read, void* seek, void* tell);

/**
 * @brief  Decode an AAC frame
 * @param  aac_decoder AAC decoder handle
 * @return decode result, 0: no error, <0: fatal error, Cannot not be ignored, >0: Slight error, can be ignored.
 */
int32_t aac_decoder_decode(void* aac_decoder);

/**
 * @brief  Get stream position @ time ms
 * @param  aac_decoder AAC decoder handle
 * @param  ms time in milli-second
 * @return stream position @ time ms, >0: offset, <0: error
 */
int64_t aac_decoder_position_to_offset(void* aac_decoder, int ms);

/**
 * @brief  Clear internal AAC stream buffer
 * @param  aac_decoder AAC decoder handle
 * @return NULL
 */
void aac_decoder_clear_buffer(void* aac_decoder);

/**
 * @brief  Initialize AAC decoder for BT A2DP
 * @param  aac_decoder AAC decoder handle
 * @param  sample_rate sample rate of the AAC stream
 * @param  channels channels of the AAC stream
 * @return initialize result, 0: success, others: failed with error code.
 */
int32_t bt_a2dp_aac_decoder_init(void* aac_decoder, uint32_t sample_rate, uint32_t channels);

/**
 * @brief  Decode an AAC frame for BT-A2DP
 * @param  aac_decoder AAC decoder handle
 * @param  inbuf  address of the AAC frame buffer
 * @param  inlen  length of the AAC frame buffer
 * @param  outbuf output address of the AAC frame decoded PCM samples
 * @param  outlen output length of the AAC frame decoded PCM samples, unit in bytes
 * @return decode result, 0: no error, <0: fatal error, Cannot not be ignored, >0: Slight error, can be ignored.
 */
int32_t bt_a2dp_aac_decoder_decode(void* aac_decoder, uint8_t* inbuf, uint32_t inlen, uint8_t** outbuf, uint32_t* outlen);

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__AAC_DECODER_API_H__
