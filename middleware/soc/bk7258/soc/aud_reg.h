// Copyright 2020-2022 Beken
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
#endif

#define AUD_LL_REG_BASE                   (SOC_AUD_REG_BASE)

/* REG_0x00 */
#define AUD_DEVICE_ID                     (AUD_LL_REG_BASE  + 0x0*4)

/* REG_0x01 */
#define AUD_VERSION_ID                    (AUD_LL_REG_BASE  + 0x1*4)

/* REG_0x02 */
#define AUD_CLK_CONTROL                   (AUD_LL_REG_BASE  + 0x2*4)

/* REG_0x03 */
#define AUD_GLOBAL_STATUS                 (AUD_LL_REG_BASE  + 0x3*4)

/* REG_0x04 */
#define AUD_ADC_CONFIG_0                  (AUD_LL_REG_BASE  + 0x4*4)
#define ADC_HPF2_COEF_B2_POSI             (0)
#define ADC_HPF2_COEF_B2_MASK             (0xFFFFU)
#define ADC_HPF2_BYPASS_POSI              (16)
#define ADC_HPF2_BYPASS_MASK              (0x1U)
#define ADC_HPF1_BYPASS_POSI              (17)
#define ADC_HPF1_BYPASS_MASK              (0x1U)
#define ADC_SET_GAIN_POSI                 (18)
#define ADC_SET_GAIN_MASK                 (0x3FU)
#define ADC_SAMPLE_EDGE_POSI              (24)
#define ADC_SAMPLE_EDGE_MASK              (0x1U)
#define ADC_DIG_MIC_SEL_POSI              (25)
#define ADC_DIG_MIC_SEL_MASK              (0x1U)

/* REG_0x05 */
#define AUD_ADC_CONFIG_1                  (AUD_LL_REG_BASE  + 0x5*4)
#define ADC_HPF2_COEF_B0_POSI             (0)
#define ADC_HPF2_COEF_B0_MASK             (0xFFFFU)
#define ADC_HPF2_COEF_B1_POSI             (16)
#define ADC_HPF2_COEF_B1_MASK             (0xFFFFU)

/* REG_0x06 */
#define AUD_ADC_CONFIG_2                  (AUD_LL_REG_BASE  + 0x6*4)
#define ADC_HPF2_COEF_A0_POSI             (0)
#define ADC_HPF2_COEF_A0_MASK             (0xFFFFU)
#define ADC_HPF2_COEF_A1_POSI             (16)
#define ADC_HPF2_COEF_A1_MASK             (0xFFFFU)

/* REG_0x07 */
#define AUD_DAC_CONFIG_0                  (AUD_LL_REG_BASE  + 0x7*4)
#define DAC_HPF2_COEF_B2_POSI             (0)
#define DAC_HPF2_COEF_B2_MASK             (0xFFFFU)
#define DAC_HPF2_BYPASS_POSI              (16)
#define DAC_HPF2_BYPASS_MASK              (0x1U)
#define DAC_HPF1_BYPASS_POSI              (17)
#define DAC_HPF1_BYPASS_MASK              (0x1U)
#define DAC_SET_GAIN_POSI                 (18)
#define DAC_SET_GAIN_MASK                 (0x3FU)
#define DAC_CLK_INVERT_POSI               (24)
#define DAC_CLK_INVERT_MASK               (0x1U)
#define DAC_PN_POSI                       (25)
#define DAC_PN_MASK                       (0xFU)
#define DAC_NOTCHEN_POSI                  (29)
#define DAC_NOTCHEN_MASK                  (0x1U)

/* REG_0x08 */
#define AUD_DAC_CONFIG_1                  (AUD_LL_REG_BASE  + 0x8*4)
#define DAC_HPF2_COEF_B0_POSI             (0)
#define DAC_HPF2_COEF_B0_MASK             (0xFFFFU)
#define DAC_HPF2_COEF_B1_POSI             (16)
#define DAC_HPF2_COEF_B1_MASK             (0xFFFFU)

/* REG_0x09 */
#define AUD_DAC_CONFIG_2                  (AUD_LL_REG_BASE  + 0x9*4)
#define DAC_HPF2_COEF_A1_POSI             (0)
#define DAC_HPF2_COEF_A1_MASK             (0xFFFFU)
#define DAC_HPF2_COEF_A2_POSI             (16)
#define DAC_HPF2_COEF_A2_MASK             (0xFFFFU)

/* REG_0x0A */
#define AUD_FIFO_CONFIG                   (AUD_LL_REG_BASE  + 0xA*4)
#define DAC_R_RD_THRED_POSI               (0)
#define DAC_R_RD_THRED_MASK               (0x1FU)
#define DAC_L_RD_THRED_POSI               (5)
#define DAC_L_RD_THRED_MASK               (0x1FU)
#define DTMF_WR_THRED_POSI                (10)
#define DTMF_WR_THRED_MASK                (0x1FU)
#define ADC_WR_THRED_POSI                 (15)
#define ADC_WR_THRED_MASK                 (0x1FU)
#define DAC_R_INT_EN_POSI                 (20)
#define DAC_R_INT_EN_MASK                 (0x1U)
#define DAC_L_INT_EN_POSI                 (21)
#define DAC_L_INT_EN_MASK                 (0x1U)
#define DTMF_INT_EN_POSI                  (22)
#define DTMF_INT_EN_MASK                  (0x1U)
#define ADC_INT_EN_POSI                   (23)
#define ADC_INT_EN_MASK                   (0x1U)
#define LOOP_TON2DAC_POSI                 (24)
#define LOOP_TON2DAC_MASK                 (0x1U)
#define LOOP_ADC2DAC_POSI                 (25)
#define LOOP_ADC2DAC_MASK                 (0x1U)

/* REG_0x0B */
#define AUD_AGC_CONFIG_0                  (AUD_LL_REG_BASE  + 0xB*4)
#define AGC_NOISE_THRED_POSI              (0)
#define AGC_NOISE_THRED_MASK              (0x3FFU)
#define AGC_NOISE_HIGH_POSI               (10)
#define AGC_NOISE_HIGH_MASK               (0x3FFU)
#define AGC_NOISE_LOW_POSI                (20)
#define AGC_NOISE_LOW_MASK                (0x3FFU)

/* REG_0x0C */
#define AUD_AGC_CONFIG_1                  (AUD_LL_REG_BASE  + 0xC*4)
#define AGC_NOISE_MIN_POSI                (0)
#define AGC_NOISE_MIN_MASK                (0x7FU)
#define AGC_NOISE_TOUT_POSI               (7)
#define AGC_NOISE_TOUT_MASK               (0x7U)
#define AGC_HIGH_DUR_POSI                 (10)
#define AGC_HIGH_DUR_MASK                 (0x7U)
#define AGC_LOW_DUR_POSI                  (13)
#define AGC_LOW_DUR_MASK                  (0x7U)
#define AGC_MIN_POSI                      (16)
#define AGC_MIN_MASK                      (0x7FU)
#define AGC_MAX_POSI                      (23)
#define AGC_MAX_MASK                      (0x7FU)
#define AGC_NG_METHOD_POSI                (30)
#define AGC_NG_METHOD_MASK                (0x1U)
#define AGC_NG_ENABLE_POSI                (31)
#define AGC_NG_ENABLE_MASK                (0x1U)

/* REG_0x0D */
#define AUD_AGC_CONFIG_2                  (AUD_LL_REG_BASE  + 0xD*4)
#define AGC_DECAY_TIME_POSI               (0)
#define AGC_DECAY_TIME_MASK               (0x7U)
#define AGC_ATTACK_TIME_POSI              (3)
#define AGC_ATTACK_TIME_MASK              (0x7U)
#define AGC_HIGH_THRD_POSI                (6)
#define AGC_HIGH_THRD_MASK                (0x1FU)
#define AGC_LOW_THRED_POSI                (11)
#define AGC_LOW_THRED_MASK                (0x1FU)
#define AGC_IIR_COEF_POSI                 (16)
#define AGC_IIR_COEF_MASK                 (0x7U)
#define AGC_ENABLE_POSI                   (19)
#define AGC_ENABLE_MASK                   (0x1U)
#define MANUAL_PGA_VAL_POSI               (20)
#define MANUAL_PGA_VAL_MASK               (0x7FU)
#define MANUAL_PGA_MODE_POSI              (27)
#define MANUAL_PGA_MODE_MASK              (0x1U)

/* REG_0x0E */
#define AUD_FIFO_STATUS                   (AUD_LL_REG_BASE  + 0xE*4)
#define DAC_R_NEAR_FULL_POSI              (0)
#define DAC_R_NEAR_FULL_MASK              (0x1U)
#define DAC_L_NEAR_FULL_POSI              (1)
#define DAC_L_NEAR_FULL_MASK              (0x1U)
#define ADC_NEAR_FULL_POSI                (2)
#define ADC_NEAR_FULL_MASK                (0x1U)
#define DTMF_NEAR_FULL_POSI               (3)
#define DTMF_NEAR_FULL_MASK               (0x1U)
#define DAC_R_NEAR_EMPTY_POSI             (4)
#define DAC_R_NEAR_EMPTY_MASK             (0x1U)
#define DAC_L_NEAR_EMPTY_POSI             (5)
#define DAC_L_NEAR_EMPTY_MASK             (0x1U)
#define ADC_NEAR_EMPTY_POSI               (6)
#define ADC_NEAR_EMPTY_MASK               (0x1U)
#define DTMF_NEAR_EMPTY_POSI              (7)
#define DTMF_NEAR_EMPTY_MASK              (0x1U)
#define DAC_R_FIFO_FULL_POSI              (8)
#define DAC_R_FIFO_FULL_MASK              (0x1U)
#define DAC_L_FIFO_FULL_POSI              (9)
#define DAC_L_FIFO_FULL_MASK              (0x1U)
#define ADC_FIFO_FULL_POSI                (10)
#define ADC_FIFO_FULL_MASK                (0x1U)
#define DTMF_FIFO_FULL_POSI               (11)
#define DTMF_FIFO_FULL_MASK               (0x1U)
#define DAC_R_FIFO_EMPTY_POSI             (12)
#define DAC_R_FIFO_EMPTY_MASK             (0x1U)
#define DAC_L_FIFO_EMPTY_POSI             (13)
#define DAC_L_FIFO_EMPTY_MASK             (0x1U)
#define ADC_FIFO_EMPTY_POSI               (14)
#define ADC_FIFO_EMPTY_MASK               (0x1U)
#define DTMF_FIFO_EMPTY_POSI              (15)
#define DTMF_FIFO_EMPTY_MASK              (0x1U)
#define DAC_R_INT_FLAG_POSI               (16)
#define DAC_R_INT_FLAG_MASK               (0x1U)
#define DAC_L_INT_FLAG_POSI               (17)
#define DAC_L_INT_FLAG_MASK               (0x1U)
#define ADC_INT_FLAG_POSI                 (18)
#define ADC_INT_FLAG_MASK                 (0x1U)
#define DTMF_INT_FLAG_POSI                (19)
#define DTMF_INT_FLAG_MASK                (0x1U)

/* REG_0x0F */
#define AUD_AGC_STATUS                    (AUD_LL_REG_BASE  + 0xF*4)
#define AGC_RSSI_POSI                     (0)
#define AGC_RSSI_MASK                     (0xFFU)
#define AGC_MIC_PGA_POSI                  (8)
#define AGC_MIC_PGA_MASK                  (0xFFU)
#define AGC_MIC_RSSI_POSI                 (16)
#define AGC_MIC_RSSI_MASK                 (0xFFFFU)

/* REG_0x10 */
#define AUD_DTMF_FIFO_PORT                (AUD_LL_REG_BASE + 0x10 * 4)
#define AD_DTMF_FIFO_POSI                 (0)
#define AD_DTMF_FIFO_MASK                 (0xFFFFU)

/* REG_0x11 */
#define AUD_ADC_FIFO_PORT                 (AUD_LL_REG_BASE + 0x11 * 4)
#define AD_ADC_L_FIFO_POSI                (0)
#define AD_ADC_L_FIFO_MASK                (0xFFFFU)

/* REG_0x12 */
#define AUD_DAC_FIFO_PORT                 (AUD_LL_REG_BASE + 0x12 * 4)
#define AD_DAC_L_FIFO_POSI                (0)
#define AD_DAC_L_FIFO_MASK                (0xFFFFU)
#define AD_DAC_R_FIFO_POSI                (16)
#define AD_DAC_R_FIFO_MASK                (0xFFFFU)

/* REG_0x18 */
#define AUD_EXTEND_CFG                    (AUD_LL_REG_BASE + 0x18 * 4)
#define DAC_FRACMOD_MANUAL_POSI           (0)
#define DAC_FRACMOD_MANUAL_MASK           (0x1U)
#define ADC_FRACMOD_MANUAL_POSI           (1)
#define ADC_FRACMOD_MANUAL_MASK           (0x1U)
#define FILT_ENABLE_POSI                  (2)
#define FILT_ENABLE_MASK                  (0x1U)

/* REG_0x19 */
#define AUD_DAC_FRACMOD                   (AUD_LL_REG_BASE + 0x19 * 4)

/* REG_0x1A */
#define AUD_ADC_FRACMOD                   (AUD_LL_REG_BASE + 0x1A * 4)

/* REG_0x1F */
#define AUD_HPF2_EXT_COEF                 (AUD_LL_REG_BASE + 0x1F * 4)
#define HPF2_A1_L_6BIT_POSI               (0)
#define HPF2_A1_L_6BIT_MASK               (0x3FU)
#define HPF2_A2_L_6BIT_POSI               (6)
#define HPF2_A2_L_6BIT_MASK               (0x3FU)
#define HPF2_B0_L_6BIT_POSI               (12)
#define HPF2_B0_L_6BIT_MASK               (0x3FU)
#define HPF2_B1_L_6BIT_POSI               (18)
#define HPF2_B1_L_6BIT_MASK               (0x3FU)
#define HPF2_B2_L_6BIT_POSI               (24)
#define HPF2_B2_L_6BIT_MASK               (0x3FU)

/* REG_0x20 */
#define AUD_FLT_0_COEF_1                  (AUD_LL_REG_BASE + 0x20 * 4)
#define FLT_0_A1_POSI                     (0)
#define FLT_0_A1_MASK                     (0xFFFFU)
#define FLT_0_A2_POSI                     (16)
#define FLT_0_A2_MASK                     (0xFFFFU)

/* REG_0x21 */
#define AUD_FLT_0_COEF_2                   (AUD_LL_REG_BASE + 0x21 * 4)
#define FLT_0_B0_POSI                     (0)
#define FLT_0_B0_MASK                     (0xFFFFU)
#define FLT_0_B1_POSI                     (16)
#define FLT_0_B1_MASK                     (0xFFFFU)

/* REG_0x22 */
#define AUD_FLT_0_COEF_3                   (AUD_LL_REG_BASE + 0x22 * 4)
#define FLT_0_B2_POSI                     (0)
#define FLT_0_B2_MASK                     (0xFFFFU)

/* REG_0x23 */
#define AUD_FLT_1_COEF_1                  (AUD_LL_REG_BASE + 0x23 * 4)
#define FLT_1_A1_POSI                     (0)
#define FLT_1_A1_MASK                     (0xFFFFU)
#define FLT_1_A2_POSI                     (16)
#define FLT_1_A2_MASK                     (0xFFFFU)

/* REG_0x24 */
#define AD_FLT_1_COEF_2                   (AUD_LL_REG_BASE + 0x24 * 4)
#define FLT_1_B0_POSI                     (0)
#define FLT_1_B0_MASK                     (0xFFFFU)
#define FLT_1_B1_POSI                     (16)
#define FLT_1_B1_MASK                     (0xFFFFU)

/* REG_0x25 */
#define AD_FLT_1_COEF_3                   (AUD_LL_REG_BASE + 0x25 * 4)
#define FLT_1_B2_POSI                     (0)
#define FLT_1_B2_MASK                     (0xFFFFU)

/* REG_0x26 */
#define AUD_FLT_2_COEF_1                  (AUD_LL_REG_BASE + 0x26 * 4)
#define FLT_2_A1_POSI                     (0)
#define FLT_2_A1_MASK                     (0xFFFFU)
#define FLT_2_A2_POSI                     (16)
#define FLT_2_A2_MASK                     (0xFFFFU)

/* REG_0x27 */
#define AD_FLT_2_COEF_2                   (AUD_LL_REG_BASE + 0x27 * 4)
#define FLT_2_B0_POSI                     (0)
#define FLT_2_B0_MASK                     (0xFFFFU)
#define FLT_2_B1_POSI                     (16)
#define FLT_2_B1_MASK                     (0xFFFFU)

/* REG_0x28 */
#define AD_FLT_2_COEF_3                   (AUD_LL_REG_BASE + 0x28 * 4)
#define FLT_2_B2_POSI                     (0)
#define FLT_2_B2_MASK                     (0xFFFFU)

/* REG_0x29 */
#define AUD_FLT_3_COEF_1                  (AUD_LL_REG_BASE + 0x29 * 4)
#define FLT_3_A1_POSI                     (0)
#define FLT_3_A1_MASK                     (0xFFFFU)
#define FLT_3_A2_POSI                     (16)
#define FLT_3_A2_MASK                     (0xFFFFU)

/* REG_0x2A */
#define AD_FLT_3_COEF_2                   (AUD_LL_REG_BASE + 0x2A * 4)
#define FLT_3_B0_POSI                     (0)
#define FLT_3_B0_MASK                     (0xFFFFU)
#define FLT_3_B1_POSI                     (16)
#define FLT_3_B1_MASK                     (0xFFFFU)

/* REG_0x2B */
#define AD_FLT_3_COEF_3                   (AUD_LL_REG_BASE + 0x2B * 4)
#define FLT_3_B2_POSI                     (0)
#define FLT_3_B2_MASK                     (0xFFFFU)

/* REG_0x2C */
#define AUD_FLT_0_EXT_COEF                (AUD_LL_REG_BASE + 0x2C * 4)
#define FLT_A1_L_6BIT_POSI                (0)
#define FLT_A1_L_6BIT_MASK                (0x3FU)
#define FLT_A2_L_6BIT_POSI                (6)
#define FLT_A2_L_6BIT_MASK                (0x3FU)
#define FLT_B0_L_6BIT_POSI                (12)
#define FLT_B0_L_6BIT_MASK                (0x3FU)
#define FLT_B1_L_6BIT_POSI                (18)
#define FLT_B1_L_6BIT_MASK                (0x3FU)
#define FLT_B2_L_6BIT_POSI                (24)
#define FLT_B2_L_6BIT_MASK                (0x3FU)

/* REG_0x2D */
#define AUD_FLT_1_EXT_COEF                (AUD_LL_REG_BASE + 0x2D * 4)

/* REG_0x2E */
#define AUD_FLT_2_EXT_COEF                (AUD_LL_REG_BASE + 0x2E * 4)

/* REG_0x2F */
#define AUD_FLT_3_EXT_COEF                (AUD_LL_REG_BASE + 0x2F * 4)

/* REG_0x30 */
#define AUD_CONFIG                        (AUD_LL_REG_BASE + 0x30 * 4)
#define SAMPLE_RATE_ADC_POSI              (0)
#define SAMPLE_RATE_ADC_MASK              (0x3U)
#define SAMPLE_RATE_8K                    (0)
#define SAMPLE_RATE_16K                   (1)
#define SAMPLE_RATE_44_1_K                (2)
#define SAMPLE_RATE_48K                   (3)
#define DAC_ENABLE_POSI                   (2)
#define DAC_ENABLE_MASK                   (0x1U)
#define ADC_ENABLE_POSI                   (3)
#define ADC_ENABLE_MASK                   (0x1U)
#define DTMF_ENABLE_POSI                  (4)
#define DTMF_ENABLE_MASK                  (0x1U)
#define LINEIN_ENABLE_POSI                (5)
#define LINEIN_ENABLE_MASK                (0x1U)
#define SAMPLE_RATE_DAC_POSI              (6)
#define SAMPLE_RATE_DAC_MASK              (0x3U)

/* REG_0x31 */
#define AUD_DTMF_CONFIG_0                 (AUD_LL_REG_BASE + 0x31 * 4)
#define TONE_PATTERN_POSI                 (0)
#define TONE_PATTERN_MASK                 (0x1U)
#define TONE_MODE_POSI                    (1)
#define TONE_MODE_MASK                    (0x1U)
#define TONE_PAUSE_TIME_POSI              (2)
#define TONE_PAUSE_TIME_MASK              (0xFU)
#define TONE_ACTIVE_TIME_POSI             (6)
#define TONE_ACTIVE_TIME_MASK             (0xFU)

/* REG_0x32 */
#define AUD_DTMF_CONFIG_1                 (AUD_LL_REG_BASE + 0x32 * 4)
#define TONE_STEP_POSI                    (0)
#define TONE_STEP_MASK                    (0xFFFFU)
#define TONE_ATTU_POSI                    (16)
#define TONE_ATTU_MASK                    (0xFU)
#define TONE_ENABLE_POSI                  (20)
#define TONE_ENABLE_MASK                  (0x1U)

/* REG_0x33 */
#define AUD_DTMF_CONFIG_2                 (AUD_LL_REG_BASE + 0x33 * 4)

#ifdef __cplusplus
}
#endif
