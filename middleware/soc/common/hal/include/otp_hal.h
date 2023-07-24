// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0set
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "hal_config.h"
#include "otp_ll.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	otp_hw_t *hw;
} otp_hal_t;

#define otp_hal_read_tmlck(hal)                                      otp_ll_read_tmlck((hal)->hw)
#define otp_hal_enable_tmlck(hal)                                    otp_ll_enable_tmlck((hal)->hw)
#define otp_hal_enable_puflck(hal)                                   otp_ll_enable_puflck((hal)->hw)
#define otp_hal_read_puflck(hal)                                     otp_ll_read_puflck((hal)->hw)
#define otp_hal_enable_otplck(hal)                                   otp_ll_enable_otplck((hal)->hw)
#define otp_hal_read_otplck(hal)                                     otp_ll_read_otplck((hal)->hw)
#define otp_hal_enable_shfwen(hal)                                   otp_ll_enable_shfwen((hal)->hw)
#define otp_hal_read_shfwen(hal)                                     otp_ll_read_shfwen((hal)->hw)
#define otp_hal_enable_shfren(hal)                                   otp_ll_enable_shfren((hal)->hw)
#define otp_hal_read_shfren(hal)                                     otp_ll_read_shfren((hal)->hw)
#define otp_hal_enable_pgmprt(hal)                                   otp_ll_enable_pgmprt((hal)->hw)
#define otp_hal_read_pgmprt(hal)                                     otp_ll_read_pgmprt((hal)->hw)
#define otp_hal_read_enroll(hal)                                     otp_ll_read_enroll((hal)->hw)
#define otp_hal_write_otp2_permission(hal, location)                 otp_ll_write_otp2_permission((hal)->hw, location)
#define otp_hal_read_otp2_permission(hal, location)                  otp_ll_read_otp2_permission((hal)->hw, location)
#define otp_hal_write_otp_security(hal, location)                    otp_ll_write_otp_security((hal)->hw, location)
#define otp_hal_read_otp_security(hal, location)                     otp_ll_read_otp_security((hal)->hw, location)
#define otp_hal_write_puf_security(hal, location)                    otp_ll_write_puf_security((hal)->hw, location)
#define otp_hal_read_puf_security(hal, location)                     otp_ll_read_puf_security((hal)->hw, location)
#define otp_hal_enable_security_protection(hal)                      otp_ll_enable_security_protection((hal)->hw)
#define otp_hal_read_security_protection(hal)                        otp_ll_read_security_protection((hal)->hw)
#define otp_hal_read_puf_zeroized_flag(hal, location)                otp_ll_read_puf_zeroized_flag((hal)->hw, location)
#define otp_hal_read_otp_zeroized_flag(hal, location)                otp_ll_read_otp_zeroized_flag((hal)->hw, location)
#define otp_hal_write_puf_permission(hal, location)                  otp_ll_write_puf_permission((hal)->hw, location)
#define otp_hal_read_puf_permission(hal, location)                   otp_ll_read_puf_permission((hal)->hw, location)
#define otp_hal_write_otp_permission(hal, location)                  otp_ll_write_otp_permission((hal)->hw, location)
#define otp_hal_read_otp_permission(hal, location)                   otp_ll_read_otp_permission((hal)->hw, location)
#define otp_hal_read_random_number(hal)                              otp_ll_read_random_number((hal)->hw)
#define otp_hal_read_intrpt_status(hal)                              otp_ll_read_intrpt_status((hal)->hw)
#define otp_hal_write_intrpt_enable(hal)                             otp_ll_write_intrpt_enable((hal)->hw, value)
#define otp_hal_read_intrpt_enable(hal)                              otp_ll_read_intrpt_enable((hal)->hw)
#define otp_hal_write_otp2_mask(hal, location)                       otp_ll_write_otp2_mask((hal)->hw, location)
#define otp_hal_read_otp2_mask(hal, location)                        otp_ll_read_otp2_mask((hal)->hw, location)
#define otp_hal_write_otp_mask(hal, location)                        otp_ll_write_otp_mask((hal)->hw, location)
#define otp_hal_read_otp_mask(hal, location)                         otp_ll_read_otp_mask((hal)->hw, location)
#define otp_hal_write_puf_mask(hal, location)                        otp_ll_write_puf_mask((hal)->hw, location)
#define otp_hal_read_puf_mask(hal, location)                         otp_ll_read_puf_mask((hal)->hw, location)
#define otp_hal_enable_mask_lck(hal)                                 otp_ll_enable_mask_lck((hal)->hw)
#define otp_hal_read_mask_lck(hal)                                   otp_ll_read_mask_lck((hal)->hw)
#define otp_hal_read_version(hal)                                    otp_ll_read_version((hal)->hw)
#define otp_hal_read_status(hal)                                     otp_ll_read_status((hal)->hw)
#define otp_hal_check_busy(hal)                                      otp_ll_check_busy((hal)->hw)
#define otp_hal_do_auto_repair(hal)                                  otp_ll_do_auto_repair((hal)->hw)
#define otp_hal_init_off_check(hal)                                  otp_ll_init_off_check((hal)->hw)
#define otp_hal_init_off_check_high_temperature(hal)                 otp_ll_init_off_check_high_temperature((hal)->hw)
#define otp_hal_do_puf_quality_check(hal)                            otp_ll_do_puf_quality_check((hal)->hw)
#define otp_hal_do_puf_health_check(hal)                             otp_ll_do_puf_health_check((hal)->hw)
#define otp_hal_do_puf_enroll(hal)                                   otp_ll_do_puf_enroll((hal)->hw)
#define otp_hal_zeroize_puf(hal, location)                           otp_ll_zeroize_puf((hal)->hw, location)
#define otp_hal_zeroize_otp(hal, location)                           otp_ll_zeroize_otp((hal)->hw, location)
#define otp_hal_set_flag(hal, location)                              otp_ll_set_flag((hal)->hw, flag)
#define otp_hal_read_puf(hal, location)                              otp_ll_read_puf((hal)->hw, location)
#define otp_hal_read_puf_entropy(hal, location)                      otp_ll_read_puf_entropy((hal)->hw, location)
#define otp_hal_read_otp(hal, location)                              otp_ll_read_otp((hal)->hw, location)
#define otp_hal_write_otp(hal, location, value)                      otp_ll_write_otp((hal)->hw, location, value)
#define otp_hal_enable_pdstb(hal)                                    otp_ll_enable_pdstb((hal)->hw)
#define otp_hal_enable_clkosc_en(hal)                                otp_ll_enable_clkosc_en((hal)->hw)
#define otp_hal_enable_fre_cont(hal)                                 otp_ll_enable_fre_cont((hal)->hw)
#define otp_hal_sleep(hal)                                           otp_ll_sleep((hal)->hw)


bk_err_t otp_hal_init(otp_hal_t *hal);

#ifdef __cplusplus
}
#endif

