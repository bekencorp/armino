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
#endif

typedef volatile struct {
		/* REG_0x00 */
		union {
			struct {
				uint32_t debug:  32; /**< bit[0:31] Vault_debug */
			};
			uint32_t v;
		} debug;

		/* REG_0x01 */
		union {
			struct {
				uint32_t reserved0:			1; /**< bit[0] */
				uint32_t abort_req:			1; /**< bit[1] Abort request signal.*/
				uint32_t big_end:			1; /**< bit[2] Big-endian,0:Little-endian,1:Big-endian*/
				uint32_t clk_man_reset_n: 	1; /**< bit[3] clk_man_reset_n*/
				uint32_t cm_reset_n:  		1; /**< bit[4] cm_reset_n*/
				uint32_t ctr_reset_n: 		1; /**< bit[5] Counter reset*/
				uint32_t slv_reset_n: 		1; /**< bit[6] Host interface reset*/
				uint32_t soft_reset: 		1; /**< bit[7] set '1' to soft_reset vault*/
				uint32_t reserved1:      	3; /**< bit[8:10] */
				uint32_t pos_disable:      	2; /**< bit[11:12] Power-On Selftest(POS),01:diable,10:after a hard-reset runs all selftests*/
				uint32_t power_mode_to_vault:	1;/**< bit[13] power_mode _to_vault*/
				uint32_t reserved2:      	2; /**< bit[14:15] */
				uint32_t tst_fro_ctrl_en :	1; /**< bit[16] tst_fro_ctrl_en*/
				uint32_t tst_fro_delay :	1; /**< bit[17] tst_fro_delay*/
				uint32_t tst_fro_enable :	1; /**< bit[18] tst_fro_enable*/
				uint32_t tst_fro_iddq :		1; /**< bit[19] tst_fro_iddq*/
				uint32_t tst_fro_select :	5; /**< bit[20:24] tst_fro_select*/
				uint32_t reserved3:			6; /**< bit[25:30] */
				uint32_t Vault_reset_n :	1; /**< bit[31] Low reset Vault IP/OTP*/
			};
			uint32_t v;
		} ctrl;

		/* REG_0x02 */
		union {
			struct {
				uint32_t debug_ctrl:  		8; /**< bit[0:7] Secure debug control output bus*/
				uint32_t reserved0:			4; /**< bit[8:11] */
				uint32_t abort_ack:         1;/**< bit[12] Abort acknowledge signal.*/
				uint32_t cm_clk_busy:		1;/**< bit[13] 1:indicates that the module is active and busy with processing data and tokens.*/
				uint32_t ctr_clk_busy:		1;/**< bit[14] 1:indicates that the counter clock domain is active.*/
				uint32_t slv_clk_busy:		1;/**< bit[15] 1:indicates active transfer on Host bus*/
				uint32_t fips_mode:         1;/**< bit[16] 1: in FIPS mode.*/
				uint32_t nonfips_mode:		1;/**< bit[17] 1: in NON-FIPS mode.*/
				uint32_t fatal_error:		1;/**< bit[18] 1: detected a fatal error and stops operation.*/
				uint32_t power_mode_write:	1;/**< bit[19] When active it indicates the status of power_mode_out is valid.*/
				uint32_t power_mode:		1;/**< bit[20] power_mode_out, 1: in NON-FIPS mode.*/
				uint32_t reserved1:			11;/**< bit[21:31] */
			};
			uint32_t v;
		} status;

		union {
			struct {
				uint32_t enable_power:  	1; /**< bit[0] Enable OTP  VDD25*/
				uint32_t reserved:			31; /**< bit[1:31] */
			};
			uint32_t v;
		} OTP_power;

} vault_hw_t;

#ifdef __cplusplus
}
#endif

