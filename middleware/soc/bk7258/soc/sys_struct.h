// Copyright 2022-2023 Beken
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

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


typedef volatile union {
	struct {
		uint32_t deviceid                         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} sys_device_id_t;


typedef volatile union {
	struct {
		uint32_t versionid                        : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} sys_version_id_t;


typedef volatile union {
	struct {
		uint32_t boot_mode                        :  1; /**<bit[0 : 0] */
		uint32_t reserved_bit_1_7                 :  7; /**<bit[1 : 7] */
		uint32_t jtag_core_sel                    :  1; /**<bit[8 : 8] */
		uint32_t flash_sel                        :  1; /**<bit[9 : 9] */
		uint32_t reserved_bit_10_31               : 22; /**<bit[10 : 31] */
	};
	uint32_t v;
} sys_cpu_storage_connect_op_select_t;


typedef volatile union {
	struct {
		uint32_t core0_halted                     :  1; /**<bit[0 : 0] */
		uint32_t core1_halted                     :  1; /**<bit[1 : 1] */
		uint32_t reserved_bit_2_3                 :  2; /**<bit[2 : 3] */
		uint32_t cpu0_sw_reset                    :  1; /**<bit[4 : 4] */
		uint32_t cpu1_sw_reset                    :  1; /**<bit[5 : 5] */
		uint32_t reserved_bit_6_7                 :  2; /**<bit[6 : 7] */
		uint32_t cpu0_pwr_dw_state                :  1; /**<bit[8 : 8] */
		uint32_t cpu1_pwr_dw_state                :  1; /**<bit[9 : 9] */
		uint32_t reserved_bit_10_31               : 22; /**<bit[10 : 31] */
	};
	uint32_t v;
} sys_cpu_current_run_status_t;


typedef volatile union {
	struct {
		uint32_t cpu0_sw_rst                      :  1; /**<bit[0 : 0] */
		uint32_t cpu0_pwr_dw                      :  1; /**<bit[1 : 1] */
		uint32_t cpu0_int_mask                    :  1; /**<bit[2 : 2] */
		uint32_t cpu0_halt                        :  1; /**<bit[3 : 3] */
		uint32_t reserved_4_7                     :  4; /**<bit[4 : 7] */
		uint32_t cpu0_offset                      : 24; /**<bit[8 : 31] */
	};
	uint32_t v;
} sys_cpu0_int_halt_clk_op_t;


typedef volatile union {
	struct {
		uint32_t cpu1_sw_rst                      :  1; /**<bit[0 : 0] */
		uint32_t cpu1_pwr_dw                      :  1; /**<bit[1 : 1] */
		uint32_t cpu1_int_mask                    :  1; /**<bit[2 : 2] */
		uint32_t cpu1_halt                        :  1; /**<bit[3 : 3] */
		uint32_t cpu0_bus_clk_2div                :  1; /**<bit[4 : 4] */
		uint32_t reserved_5_7                     :  3; /**<bit[5 : 7] */
		uint32_t cpu1_offset                      : 24; /**<bit[8 : 31] */
	};
	uint32_t v;
} sys_cpu1_int_halt_clk_op_t;


typedef volatile union {
	struct {
		uint32_t reserved_0_31                    : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} sys_reserved_reg0x6_t;


typedef volatile union {
	struct {
		uint32_t clkdiv_core                      :  4; /**<bit[0 : 3] */
		uint32_t cksel_core                       :  2; /**<bit[4 : 5] */
		uint32_t clkdiv_bus                       :  1; /**<bit[6 : 6] */
		uint32_t reserved_bit_7_7                 :  1; /**<bit[7 : 7] */
		uint32_t clkdiv_uart0                     :  2; /**<bit[8 : 9] */
		uint32_t clksel_uart0                     :  1; /**<bit[10 : 10] */
		uint32_t clkdiv_uart1                     :  2; /**<bit[11 : 12] */
		uint32_t cksel_uart1                      :  1; /**<bit[13 : 13] */
		uint32_t clkdiv_uart2                     :  2; /**<bit[14 : 15] */
		uint32_t cksel_uart2                      :  1; /**<bit[16 : 16] */
		uint32_t cksel_sadc                       :  1; /**<bit[17 : 17] */
		uint32_t cksel_pwm0                       :  1; /**<bit[18 : 18] */
		uint32_t cksel_pwm1                       :  1; /**<bit[19 : 19] */
		uint32_t cksel_tim0                       :  1; /**<bit[20 : 20] */
		uint32_t cksel_tim1                       :  1; /**<bit[21 : 21] */
		uint32_t cksel_tim2                       :  1; /**<bit[22 : 22] */
		uint32_t reserved_bit_23_23               :  1; /**<bit[23 : 23] */
		uint32_t cksel_i2s                        :  1; /**<bit[24 : 24] */
		uint32_t cksel_aud                        :  1; /**<bit[25 : 25] */
		uint32_t clkdiv_jpeg                      :  4; /**<bit[26 : 29] */
		uint32_t cksel_jpeg                       :  1; /**<bit[30 : 30] */
		uint32_t clkdiv_disp_l                    :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_cpu_clk_div_mode1_t;


typedef volatile union {
	struct {
		uint32_t clkdiv_disp_h                    :  3; /**<bit[0 : 2] */
		uint32_t cksel_disp                       :  1; /**<bit[3 : 3] */
		uint32_t ckdiv_psram                      :  1; /**<bit[4 : 4] */
		uint32_t cksel_psram                      :  1; /**<bit[5 : 5] */
		uint32_t ckdiv_qspi0                      :  4; /**<bit[6 : 9] */
		uint32_t reserved_10_13                   :  4; /**<bit[10 : 13] */
		uint32_t ckdiv_sdio                       :  3; /**<bit[14 : 16] */
		uint32_t cksel_sdio                       :  1; /**<bit[17 : 17] */
		uint32_t ckdiv_auxs                       :  4; /**<bit[18 : 21] */
		uint32_t cksel_auxs                       :  2; /**<bit[22 : 23] */
		uint32_t cksel_flash                      :  2; /**<bit[24 : 25] */
		uint32_t ckdiv_flash                      :  2; /**<bit[26 : 27] */
		uint32_t ckdiv_i2s0                       :  3; /**<bit[28 : 30] */
		uint32_t reserved_bit_31_31               :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_cpu_clk_div_mode2_t;


typedef volatile union {
	struct {
		uint32_t ckdiv_26m                        :  2; /**<bit[0 : 1] */
		uint32_t ckdiv_wdt                        :  2; /**<bit[2 : 3] */
		uint32_t clksel_spi0                      :  1; /**<bit[4 : 4] */
		uint32_t clksel_spi1                      :  1; /**<bit[5 : 5] */
		uint32_t reserved_bit_6_31                : 26; /**<bit[6 : 31] */
	};
	uint32_t v;
} sys_cpu_26m_wdt_clk_div_t;


typedef volatile union {
	struct {
		uint32_t anaspi_freq                      :  6; /**<bit[0 : 5] */
		uint32_t reserved_bit_6_31                : 26; /**<bit[6 : 31] */
	};
	uint32_t v;
} sys_cpu_anaspi_freq_t;


typedef volatile union {
	struct {
		uint32_t i2c0_cken                        :  1; /**<bit[0 : 0] */
		uint32_t spi0_cken                        :  1; /**<bit[1 : 1] */
		uint32_t uart0_cken                       :  1; /**<bit[2 : 2] */
		uint32_t pwm0_cken                        :  1; /**<bit[3 : 3] */
		uint32_t tim0_cken                        :  1; /**<bit[4 : 4] */
		uint32_t sadc_cken                        :  1; /**<bit[5 : 5] */
		uint32_t irda_cken                        :  1; /**<bit[6 : 6] */
		uint32_t efuse_cken                       :  1; /**<bit[7 : 7] */
		uint32_t i2c1_cken                        :  1; /**<bit[8 : 8] */
		uint32_t spi1_cken                        :  1; /**<bit[9 : 9] */
		uint32_t uart1_cken                       :  1; /**<bit[10 : 10] */
		uint32_t uart2_cken                       :  1; /**<bit[11 : 11] */
		uint32_t pwm1_cken                        :  1; /**<bit[12 : 12] */
		uint32_t tim1_cken                        :  1; /**<bit[13 : 13] */
		uint32_t tim2_cken                        :  1; /**<bit[14 : 14] */
		uint32_t otp_cken                         :  1; /**<bit[15 : 15] */
		uint32_t i2s_cken                         :  1; /**<bit[16 : 16] */
		uint32_t usb_cken                         :  1; /**<bit[17 : 17] */
		uint32_t can_cken                         :  1; /**<bit[18 : 18] */
		uint32_t psram_cken                       :  1; /**<bit[19 : 19] */
		uint32_t qspi0_cken                       :  1; /**<bit[20 : 20] */
		uint32_t qspi1_cken                       :  1; /**<bit[21 : 21] */
		uint32_t sdio_cken                        :  1; /**<bit[22 : 22] */
		uint32_t auxs_cken                        :  1; /**<bit[23 : 23] */
		uint32_t btdm_cken                        :  1; /**<bit[24 : 24] */
		uint32_t xvr_cken                         :  1; /**<bit[25 : 25] */
		uint32_t mac_cken                         :  1; /**<bit[26 : 26] */
		uint32_t phy_cken                         :  1; /**<bit[27 : 27] */
		uint32_t jpeg_cken                        :  1; /**<bit[28 : 28] */
		uint32_t disp_cken                        :  1; /**<bit[29 : 29] */
		uint32_t aud_cken                         :  1; /**<bit[30 : 30] */
		uint32_t wdt_cken                         :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_cpu_device_clk_enable_t;


typedef volatile union {
	struct {
		uint32_t h264_cken                        :  1; /**<bit[0 : 0] */
		uint32_t i2s1_cken                        :  1; /**<bit[1 : 1] */
		uint32_t i2s2_cken                        :  1; /**<bit[2 : 2] */
		uint32_t yuv_cken                         :  1; /**<bit[3 : 3] */
		uint32_t slcd_cken                        :  1; /**<bit[4 : 4] */
		uint32_t reserved_5_31                    : 27; /**<bit[5 : 31] */
	};
	uint32_t v;
} sys_reserver_reg0xd_t;


typedef volatile union {
	struct {
		uint32_t uart1_mem_sd                     :  1; /**<bit[0 : 0] */
		uint32_t uart2                            :  1; /**<bit[1 : 1] */
		uint32_t spi1                             :  1; /**<bit[2 : 2] */
		uint32_t sdio                             :  1; /**<bit[3 : 3] */
		uint32_t usb                              :  1; /**<bit[4 : 4] */
		uint32_t can                              :  1; /**<bit[5 : 5] */
		uint32_t qspi0                            :  1; /**<bit[6 : 6] */
		uint32_t qspi1                            :  1; /**<bit[7 : 7] */
		uint32_t pram                             :  1; /**<bit[8 : 8] */
		uint32_t fft                              :  1; /**<bit[9 : 9] */
		uint32_t abc                              :  1; /**<bit[10 : 10] */
		uint32_t aud                              :  1; /**<bit[11 : 11] */
		uint32_t i2s0                             :  1; /**<bit[12 : 12] */
		uint32_t i2s1                             :  1; /**<bit[13 : 13] */
		uint32_t i2s2                             :  1; /**<bit[14 : 14] */
		uint32_t jpge                             :  1; /**<bit[15 : 15] */
		uint32_t yuv                              :  1; /**<bit[16 : 16] */
		uint32_t jpgd                             :  1; /**<bit[17 : 17] */
		uint32_t disp                             :  1; /**<bit[18 : 18] */
		uint32_t dmad                             :  1; /**<bit[19 : 19] */
		uint32_t h26f                             :  1; /**<bit[20 : 20] */
		uint32_t mac                              :  1; /**<bit[21 : 21] */
		uint32_t phy                              :  1; /**<bit[22 : 22] */
		uint32_t xvr                              :  1; /**<bit[23 : 23] */
		uint32_t irda                             :  1; /**<bit[24 : 24] */
		uint32_t la                               :  1; /**<bit[25 : 25] */
		uint32_t flsh                             :  1; /**<bit[26 : 26] */
		uint32_t uart                             :  1; /**<bit[27 : 27] */
		uint32_t spi0                             :  1; /**<bit[28 : 28] */
		uint32_t enc                              :  1; /**<bit[29 : 29] */
		uint32_t dma0                             :  1; /**<bit[30 : 30] */
		uint32_t dma1                             :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_cpu_device_mem_ctrl1_t;


typedef volatile union {
	struct {
		uint32_t uart1_mem_ds                     :  1; /**<bit[0 : 0] */
		uint32_t uart2                            :  1; /**<bit[1 : 1] */
		uint32_t spi1                             :  1; /**<bit[2 : 2] */
		uint32_t sdio                             :  1; /**<bit[3 : 3] */
		uint32_t usb                              :  1; /**<bit[4 : 4] */
		uint32_t can                              :  1; /**<bit[5 : 5] */
		uint32_t qspi0                            :  1; /**<bit[6 : 6] */
		uint32_t qspi1                            :  1; /**<bit[7 : 7] */
		uint32_t pram                             :  1; /**<bit[8 : 8] */
		uint32_t fft                              :  1; /**<bit[9 : 9] */
		uint32_t abc                              :  1; /**<bit[10 : 10] */
		uint32_t aud                              :  1; /**<bit[11 : 11] */
		uint32_t i2s0                             :  1; /**<bit[12 : 12] */
		uint32_t i2s1                             :  1; /**<bit[13 : 13] */
		uint32_t i2s2                             :  1; /**<bit[14 : 14] */
		uint32_t jpge                             :  1; /**<bit[15 : 15] */
		uint32_t yuv                              :  1; /**<bit[16 : 16] */
		uint32_t jpgd                             :  1; /**<bit[17 : 17] */
		uint32_t disp                             :  1; /**<bit[18 : 18] */
		uint32_t dmad                             :  1; /**<bit[19 : 19] */
		uint32_t h26f                             :  1; /**<bit[20 : 20] */
		uint32_t mac                              :  1; /**<bit[21 : 21] */
		uint32_t phy                              :  1; /**<bit[22 : 22] */
		uint32_t xvr                              :  1; /**<bit[23 : 23] */
		uint32_t irda                             :  1; /**<bit[24 : 24] */
		uint32_t la                               :  1; /**<bit[25 : 25] */
		uint32_t flsh                             :  1; /**<bit[26 : 26] */
		uint32_t uart                             :  1; /**<bit[27 : 27] */
		uint32_t spi0                             :  1; /**<bit[28 : 28] */
		uint32_t enc                              :  1; /**<bit[29 : 29] */
		uint32_t dma0                             :  1; /**<bit[30 : 30] */
		uint32_t dma1                             :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_cpu_device_mem_ctrl2_t;


typedef volatile union {
	struct {
		uint32_t pwd_mem1                         :  1; /**<bit[0 : 0] */
		uint32_t pwd_mem2                         :  1; /**<bit[1 : 1] */
		uint32_t pwd_mem3                         :  1; /**<bit[2 : 2] */
		uint32_t pwd_encp                         :  1; /**<bit[3 : 3] */
		uint32_t pwd_bakp                         :  1; /**<bit[4 : 4] */
		uint32_t pwd_ahbp                         :  1; /**<bit[5 : 5] */
		uint32_t pwd_audp                         :  1; /**<bit[6 : 6] */
		uint32_t pwd_vidp                         :  1; /**<bit[7 : 7] */
		uint32_t pwd_btsp                         :  1; /**<bit[8 : 8] */
		uint32_t pwd_wifp_mac                     :  1; /**<bit[9 : 9] */
		uint32_t pwd_wifp_phy                     :  1; /**<bit[10 : 10] */
		uint32_t pwd_mem0                         :  1; /**<bit[11 : 11] */
		uint32_t pwd_mem4                         :  1; /**<bit[12 : 12] */
		uint32_t pwd_ofdm                         :  1; /**<bit[13 : 13] */
		uint32_t tcm0_pgen                        :  1; /**<bit[14 : 14] */
		uint32_t rom_pgen                         :  1; /**<bit[15 : 15] */
		uint32_t sleep_en_need_flash_idle         :  1; /**<bit[16 : 16] */
		uint32_t sleep_en_need_cpu1_wfi           :  1; /**<bit[17 : 17] */
		uint32_t sleep_en_need_cpu0_wfi           :  1; /**<bit[18 : 18] */
		uint32_t sleep_en_global                  :  1; /**<bit[19 : 19] */
		uint32_t sleep_bus_idle_bypass            :  1; /**<bit[20 : 20] */
		uint32_t bts_wakeup_platform_en           :  1; /**<bit[21 : 21] */
		uint32_t bts_soft_wakeup_req              :  1; /**<bit[22 : 22] */
		uint32_t rom_rd_disable                   :  1; /**<bit[23 : 23] */
		uint32_t otp_rd_disable                   :  1; /**<bit[24 : 24] */
		uint32_t tcm1_pgen                        :  1; /**<bit[25 : 25] */
		uint32_t cpu0_subpwdm_en                  :  1; /**<bit[26 : 26] */
		uint32_t cpu1_subpwdm_en                  :  1; /**<bit[27 : 27] */
		uint32_t share_mem_clkgating_disable      :  1; /**<bit[28 : 28] */
		uint32_t cpu0_ticktimer_32k_enable        :  1; /**<bit[29 : 29] */
		uint32_t cpu1_ticktimer_32k_enable        :  1; /**<bit[30 : 30] */
		uint32_t reserved_bit_31_31               :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_cpu_power_sleep_wakeup_t;


typedef volatile union {
	struct {
		uint32_t reserved_0_31                    : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} sys_reserver_reg0x11_t;


typedef volatile union {
	struct {
		uint32_t ram0_mem_ds_deep_sleep           :  1; /**<bit[0 : 0] */
		uint32_t ram1_deep_sleep                  :  1; /**<bit[1 : 1] */
		uint32_t ram2_deep_sleep                  :  1; /**<bit[2 : 2] */
		uint32_t ram3_deep_sleep                  :  1; /**<bit[3 : 3] */
		uint32_t ram4_deep_sleep                  :  1; /**<bit[4 : 4] */
		uint32_t cpu0_icache_itag_deep_sleep      :  1; /**<bit[5 : 5] */
		uint32_t cpu0_dcache_dtag_deep_sleep      :  1; /**<bit[6 : 6] */
		uint32_t cpu1_icache_itag_deep_sleep      :  1; /**<bit[7 : 7] */
		uint32_t cpu1_dcache_dtag_deep_sleep      :  1; /**<bit[8 : 8] */
		uint32_t cpu0_itcm_deep_sleep             :  1; /**<bit[9 : 9] */
		uint32_t cpu0_dtcm_deep_sleep             :  1; /**<bit[10 : 10] */
		uint32_t cpu1_itcm_deep_sleep             :  1; /**<bit[11 : 11] */
		uint32_t cpu1_dtcm_deep_sleep             :  1; /**<bit[12 : 12] */
		uint32_t rott_deep_sleep                  :  1; /**<bit[13 : 13] */
		uint32_t reserved0                        :  1; /**<bit[14 : 14] */
		uint32_t reserved1                        :  1; /**<bit[15 : 15] */
		uint32_t ram0_mem_sd_shutdown             :  1; /**<bit[16 : 16] */
		uint32_t ram1_shutdown                    :  1; /**<bit[17 : 17] */
		uint32_t ram2_shutdown                    :  1; /**<bit[18 : 18] */
		uint32_t ram3_shutdown                    :  1; /**<bit[19 : 19] */
		uint32_t ram4_shutdown                    :  1; /**<bit[20 : 20] */
		uint32_t cpu0_icache_itag_shutdown        :  1; /**<bit[21 : 21] */
		uint32_t cpu0_dcache_dtag_shutdown        :  1; /**<bit[22 : 22] */
		uint32_t cpu1_icache_itag_shutdown        :  1; /**<bit[23 : 23] */
		uint32_t cpu1_dcache_dtag_shutdown        :  1; /**<bit[24 : 24] */
		uint32_t cpu0_itcm_shutdown               :  1; /**<bit[25 : 25] */
		uint32_t cpu0_dtcm_shutdown               :  1; /**<bit[26 : 26] */
		uint32_t cpu1_itcm_shutdown               :  1; /**<bit[27 : 27] */
		uint32_t cpu1_dtcm_shutdown               :  1; /**<bit[28 : 28] */
		uint32_t rott_shutdown                    :  1; /**<bit[29 : 29] */
		uint32_t reserved2                        :  1; /**<bit[30 : 30] */
		uint32_t reserved3                        :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_cpu_device_mem_ctrl3_t;


typedef volatile union {
	struct {
		uint32_t cpu0_dma0_nsec_intr_en           :  1; /**<bit[0 : 0] */
		uint32_t cpu0_encp_sec_intr_en            :  1; /**<bit[1 : 1] */
		uint32_t cpu0_encp_nsec_intr_en           :  1; /**<bit[2 : 2] */
		uint32_t cpu0_timer0_int_en               :  1; /**<bit[3 : 3] */
		uint32_t cpu0_uart_int_en                 :  1; /**<bit[4 : 4] */
		uint32_t cpu0_pwm0_int_en                 :  1; /**<bit[5 : 5] */
		uint32_t cpu0_i2c0_int_en                 :  1; /**<bit[6 : 6] */
		uint32_t cpu0_spi0_int_en                 :  1; /**<bit[7 : 7] */
		uint32_t cpu0_sadc_int_en                 :  1; /**<bit[8 : 8] */
		uint32_t cpu0_irda_int_en                 :  1; /**<bit[9 : 9] */
		uint32_t cpu0_sdio_int_en                 :  1; /**<bit[10 : 10] */
		uint32_t cpu0_gdma_int_en                 :  1; /**<bit[11 : 11] */
		uint32_t cpu0_la_int_en                   :  1; /**<bit[12 : 12] */
		uint32_t cpu0_timer1_int_en               :  1; /**<bit[13 : 13] */
		uint32_t cpu0_i2c1_int_en                 :  1; /**<bit[14 : 14] */
		uint32_t cpu0_uart1_int_en                :  1; /**<bit[15 : 15] */
		uint32_t cpu0_uart2_int_en                :  1; /**<bit[16 : 16] */
		uint32_t cpu0_spi1_int_en                 :  1; /**<bit[17 : 17] */
		uint32_t cpu0_can_int_en                  :  1; /**<bit[18 : 18] */
		uint32_t cpu0_usb_int_en                  :  1; /**<bit[19 : 19] */
		uint32_t cpu0_qspi0_int_en                :  1; /**<bit[20 : 20] */
		uint32_t cpu0_fft_int_en                  :  1; /**<bit[21 : 21] */
		uint32_t cpu0_sbc_int_en                  :  1; /**<bit[22 : 22] */
		uint32_t cpu0_aud_int_en                  :  1; /**<bit[23 : 23] */
		uint32_t cpu0_i2s0_int_en                 :  1; /**<bit[24 : 24] */
		uint32_t cpu0_jpegenc_int_en              :  1; /**<bit[25 : 25] */
		uint32_t cpu0_jpegdec_int_en              :  1; /**<bit[26 : 26] */
		uint32_t cpu0_lcd_display_int_en          :  1; /**<bit[27 : 27] */
		uint32_t cpu0_dma2d_int_en                :  1; /**<bit[28 : 28] */
		uint32_t cpu0_wifi_int_phy_mpb_en         :  1; /**<bit[29 : 29] */
		uint32_t cpu0_wifi_int_phy_riu_en         :  1; /**<bit[30 : 30] */
		uint32_t cpu0_wifi_mac_int_tx_rx_timer_en :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_cpu0_int_0_31_en_t;


typedef volatile union {
	struct {
		uint32_t cpu0_wifi_mac_int_tx_rx_misc_en  :  1; /**<bit[0 : 0] */
		uint32_t cpu0_wifi_mac_int_rx_trigger_en  :  1; /**<bit[1 : 1] */
		uint32_t cpu0_wifi_mac_int_tx_trigger_en  :  1; /**<bit[2 : 2] */
		uint32_t cpu0_wifi_mac_int_prot_trigger_en :  1; /**<bit[3 : 3] */
		uint32_t cpu0_wifi_mac_int_gen_en         :  1; /**<bit[4 : 4] */
		uint32_t cpu0_wifi_hsu_irq_en             :  1; /**<bit[5 : 5] */
		uint32_t cpu0_wifi_int_mac_wakeup_en      :  1; /**<bit[6 : 6] */
		uint32_t cpu0_dm_irq_en                   :  1; /**<bit[7 : 7] */
		uint32_t cpu0_ble_irq_en                  :  1; /**<bit[8 : 8] */
		uint32_t cpu0_bt_irq_en                   :  1; /**<bit[9 : 9] */
		uint32_t cpu0_qspi1_int_en                :  1; /**<bit[10 : 10] */
		uint32_t cpu0_pwm1_int_en                 :  1; /**<bit[11 : 11] */
		uint32_t cpu0_i2s1_int_en                 :  1; /**<bit[12 : 12] */
		uint32_t cpu0_i2s2_int_en                 :  1; /**<bit[13 : 13] */
		uint32_t cpu0_h264_int_en                 :  1; /**<bit[14 : 14] */
		uint32_t cpu0_sdmadc_int_en               :  1; /**<bit[15 : 15] */
		uint32_t cpu0_mbox0_int_en                :  1; /**<bit[16 : 16] */
		uint32_t cpu0_mbox1_int_en                :  1; /**<bit[17 : 17] */
		uint32_t cpu0_bmc64_int_en                :  1; /**<bit[18 : 18] */
		uint32_t cpu0_dpll_unlock_int_en          :  1; /**<bit[19 : 19] */
		uint32_t cpu0_touched_int_en              :  1; /**<bit[20 : 20] */
		uint32_t cpu0_usbplug_int_en              :  1; /**<bit[21 : 21] */
		uint32_t cpu0_rtc_int_en                  :  1; /**<bit[22 : 22] */
		uint32_t cpu0_gpio_int_en                 :  1; /**<bit[23 : 23] */
		uint32_t cpu0_dma1_sec_int_en             :  1; /**<bit[24 : 24] */
		uint32_t cpu0_dma1_nsec_int_en            :  1; /**<bit[25 : 25] */
		uint32_t cpu0_yuvb_int_en                 :  1; /**<bit[26 : 26] */
		uint32_t cpu0_rott_int_en                 :  1; /**<bit[27 : 27] */
		uint32_t reserved0                        :  4; /**<bit[28 : 31] */
	};
	uint32_t v;
} sys_cpu0_int_32_63_en_t;


typedef volatile union {
	struct {
		uint32_t cpu1_dma0_nsec_intr_en           :  1; /**<bit[0 : 0] */
		uint32_t cpu1_encp_sec_intr_en            :  1; /**<bit[1 : 1] */
		uint32_t cpu1_encp_nsec_intr_en           :  1; /**<bit[2 : 2] */
		uint32_t cpu1_timer0_int_en               :  1; /**<bit[3 : 3] */
		uint32_t cpu1_uart_int_en                 :  1; /**<bit[4 : 4] */
		uint32_t cpu1_pwm0_int_en                 :  1; /**<bit[5 : 5] */
		uint32_t cpu1_i2c0_int_en                 :  1; /**<bit[6 : 6] */
		uint32_t cpu1_spi0_int_en                 :  1; /**<bit[7 : 7] */
		uint32_t cpu1_sadc_int_en                 :  1; /**<bit[8 : 8] */
		uint32_t cpu1_irda_int_en                 :  1; /**<bit[9 : 9] */
		uint32_t cpu1_sdio_int_en                 :  1; /**<bit[10 : 10] */
		uint32_t cpu1_gdma_int_en                 :  1; /**<bit[11 : 11] */
		uint32_t cpu1_la_int_en                   :  1; /**<bit[12 : 12] */
		uint32_t cpu1_timer1_int_en               :  1; /**<bit[13 : 13] */
		uint32_t cpu1_i2c1_int_en                 :  1; /**<bit[14 : 14] */
		uint32_t cpu1_uart1_int_en                :  1; /**<bit[15 : 15] */
		uint32_t cpu1_uart2_int_en                :  1; /**<bit[16 : 16] */
		uint32_t cpu1_spi1_int_en                 :  1; /**<bit[17 : 17] */
		uint32_t cpu1_can_int_en                  :  1; /**<bit[18 : 18] */
		uint32_t cpu1_usb_int_en                  :  1; /**<bit[19 : 19] */
		uint32_t cpu1_qspi0_int_en                :  1; /**<bit[20 : 20] */
		uint32_t cpu1_fft_int_en                  :  1; /**<bit[21 : 21] */
		uint32_t cpu1_sbc_int_en                  :  1; /**<bit[22 : 22] */
		uint32_t cpu1_aud_int_en                  :  1; /**<bit[23 : 23] */
		uint32_t cpu1_i2s0_int_en                 :  1; /**<bit[24 : 24] */
		uint32_t cpu1_jpegenc_int_en              :  1; /**<bit[25 : 25] */
		uint32_t cpu1_jpegdec_int_en              :  1; /**<bit[26 : 26] */
		uint32_t cpu1_lcd_display_int_en          :  1; /**<bit[27 : 27] */
		uint32_t cpu1_dma2d_int_en                :  1; /**<bit[28 : 28] */
		uint32_t cpu1_wifi_int_phy_mpb_en         :  1; /**<bit[29 : 29] */
		uint32_t cpu1_wifi_int_phy_riu_en         :  1; /**<bit[30 : 30] */
		uint32_t cpu1_wifi_mac_int_tx_rx_timer_en :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_cpu1_int_0_31_en_t;


typedef volatile union {
	struct {
		uint32_t cpu1_wifi_mac_int_tx_rx_misc_en  :  1; /**<bit[0 : 0] */
		uint32_t cpu1_wifi_mac_int_rx_trigger_en  :  1; /**<bit[1 : 1] */
		uint32_t cpu1_wifi_mac_int_tx_trigger_en  :  1; /**<bit[2 : 2] */
		uint32_t cpu1_wifi_mac_int_prot_trigger_en :  1; /**<bit[3 : 3] */
		uint32_t cpu1_wifi_mac_int_gen_en         :  1; /**<bit[4 : 4] */
		uint32_t cpu1_wifi_hsu_irq_en             :  1; /**<bit[5 : 5] */
		uint32_t cpu1_wifi_int_mac_wakeup_en      :  1; /**<bit[6 : 6] */
		uint32_t cpu1_dm_irq_en                   :  1; /**<bit[7 : 7] */
		uint32_t cpu1_ble_irq_en                  :  1; /**<bit[8 : 8] */
		uint32_t cpu1_bt_irq_en                   :  1; /**<bit[9 : 9] */
		uint32_t cpu1_qspi1_int_en                :  1; /**<bit[10 : 10] */
		uint32_t cpu1_pwm1_int_en                 :  1; /**<bit[11 : 11] */
		uint32_t cpu1_i2s1_int_en                 :  1; /**<bit[12 : 12] */
		uint32_t cpu1_i2s2_int_en                 :  1; /**<bit[13 : 13] */
		uint32_t cpu1_h264_int_en                 :  1; /**<bit[14 : 14] */
		uint32_t cpu1_sdmadc_int_en               :  1; /**<bit[15 : 15] */
		uint32_t cpu1_mbox0_int_en                :  1; /**<bit[16 : 16] */
		uint32_t cpu1_mbox1_int_en                :  1; /**<bit[17 : 17] */
		uint32_t cpu1_bmc64_int_en                :  1; /**<bit[18 : 18] */
		uint32_t cpu1_dpll_unlock_int_en          :  1; /**<bit[19 : 19] */
		uint32_t cpu1_touched_int_en              :  1; /**<bit[20 : 20] */
		uint32_t cpu1_usbplug_int_en              :  1; /**<bit[21 : 21] */
		uint32_t cpu1_rtc_int_en                  :  1; /**<bit[22 : 22] */
		uint32_t cpu1_gpio_int_en                 :  1; /**<bit[23 : 23] */
		uint32_t cpu1_dma1_sec_int_en             :  1; /**<bit[24 : 24] */
		uint32_t cpu1_dma1_nsec_int_en            :  1; /**<bit[25 : 25] */
		uint32_t cpu1_yuvb_int_en                 :  1; /**<bit[26 : 26] */
		uint32_t cpu1_rott_int_en                 :  1; /**<bit[27 : 27] */
		uint32_t reserved0                        :  4; /**<bit[28 : 31] */
	};
	uint32_t v;
} sys_cpu1_int_32_63_en_t;


typedef volatile union {
	struct {
		uint32_t cpu0_dma0_nsec_intr_st           :  1; /**<bit[0 : 0] */
		uint32_t cpu0_encp_sec_intr_st            :  1; /**<bit[1 : 1] */
		uint32_t cpu0_encp_nsec_intr_st           :  1; /**<bit[2 : 2] */
		uint32_t cpu0_timer0_int_st               :  1; /**<bit[3 : 3] */
		uint32_t cpu0_uart_int_st                 :  1; /**<bit[4 : 4] */
		uint32_t cpu0_pwm0_int_st                 :  1; /**<bit[5 : 5] */
		uint32_t cpu0_i2c0_int_st                 :  1; /**<bit[6 : 6] */
		uint32_t cpu0_spi0_int_st                 :  1; /**<bit[7 : 7] */
		uint32_t cpu0_sadc_int_st                 :  1; /**<bit[8 : 8] */
		uint32_t cpu0_irda_int_st                 :  1; /**<bit[9 : 9] */
		uint32_t cpu0_sdio_int_st                 :  1; /**<bit[10 : 10] */
		uint32_t cpu0_gdma_int_st                 :  1; /**<bit[11 : 11] */
		uint32_t cpu0_la_int_st                   :  1; /**<bit[12 : 12] */
		uint32_t cpu0_timer1_int_st               :  1; /**<bit[13 : 13] */
		uint32_t cpu0_i2c1_int_st                 :  1; /**<bit[14 : 14] */
		uint32_t cpu0_uart1_int_st                :  1; /**<bit[15 : 15] */
		uint32_t cpu0_uart2_int_st                :  1; /**<bit[16 : 16] */
		uint32_t cpu0_spi1_int_st                 :  1; /**<bit[17 : 17] */
		uint32_t cpu0_can_int_st                  :  1; /**<bit[18 : 18] */
		uint32_t cpu0_usb_int_st                  :  1; /**<bit[19 : 19] */
		uint32_t cpu0_qspi0_int_st                :  1; /**<bit[20 : 20] */
		uint32_t cpu0_fft_int_st                  :  1; /**<bit[21 : 21] */
		uint32_t cpu0_sbc_int_st                  :  1; /**<bit[22 : 22] */
		uint32_t cpu0_aud_int_st                  :  1; /**<bit[23 : 23] */
		uint32_t cpu0_i2s0_int_st                 :  1; /**<bit[24 : 24] */
		uint32_t cpu0_jpegenc_int_st              :  1; /**<bit[25 : 25] */
		uint32_t cpu0_jpegdec_int_st              :  1; /**<bit[26 : 26] */
		uint32_t cpu0_lcd_display_int_st          :  1; /**<bit[27 : 27] */
		uint32_t cpu0_dma2d_int_st                :  1; /**<bit[28 : 28] */
		uint32_t cpu0_wifi_int_phy_mpb_st         :  1; /**<bit[29 : 29] */
		uint32_t cpu0_wifi_int_phy_riu_st         :  1; /**<bit[30 : 30] */
		uint32_t cpu0_wifi_mac_int_tx_rx_timer_st :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_cpu0_int_0_31_status_t;


typedef volatile union {
	struct {
		uint32_t cpu0_wifi_mac_int_tx_rx_misc_st  :  1; /**<bit[0 : 0] */
		uint32_t cpu0_wifi_mac_int_rx_trigger_st  :  1; /**<bit[1 : 1] */
		uint32_t cpu0_wifi_mac_int_tx_trigger_st  :  1; /**<bit[2 : 2] */
		uint32_t cpu0_wifi_mac_int_prot_trigger_st :  1; /**<bit[3 : 3] */
		uint32_t cpu0_wifi_mac_int_gen_st         :  1; /**<bit[4 : 4] */
		uint32_t cpu0_wifi_hsu_irq_st             :  1; /**<bit[5 : 5] */
		uint32_t cpu0_wifi_int_mac_wakeup_st      :  1; /**<bit[6 : 6] */
		uint32_t cpu0_dm_irq_st                   :  1; /**<bit[7 : 7] */
		uint32_t cpu0_ble_irq_st                  :  1; /**<bit[8 : 8] */
		uint32_t cpu0_bt_irq_st                   :  1; /**<bit[9 : 9] */
		uint32_t cpu0_qspi1_int_st                :  1; /**<bit[10 : 10] */
		uint32_t cpu0_pwm1_int_st                 :  1; /**<bit[11 : 11] */
		uint32_t cpu0_i2s1_int_st                 :  1; /**<bit[12 : 12] */
		uint32_t cpu0_i2s2_int_st                 :  1; /**<bit[13 : 13] */
		uint32_t cpu0_h264_int_st                 :  1; /**<bit[14 : 14] */
		uint32_t cpu0_sdmadc_int_st               :  1; /**<bit[15 : 15] */
		uint32_t cpu0_mbox0_int_st                :  1; /**<bit[16 : 16] */
		uint32_t cpu0_mbox1_int_st                :  1; /**<bit[17 : 17] */
		uint32_t cpu0_bmc64_int_st                :  1; /**<bit[18 : 18] */
		uint32_t cpu0_dpll_unlock_int_st          :  1; /**<bit[19 : 19] */
		uint32_t cpu0_touched_int_st              :  1; /**<bit[20 : 20] */
		uint32_t cpu0_usbplug_int_st              :  1; /**<bit[21 : 21] */
		uint32_t cpu0_rtc_int_st                  :  1; /**<bit[22 : 22] */
		uint32_t cpu0_gpio_int_st                 :  1; /**<bit[23 : 23] */
		uint32_t cpu0_dma1_sec_int_st             :  1; /**<bit[24 : 24] */
		uint32_t cpu0_dma1_nsec_int_st            :  1; /**<bit[25 : 25] */
		uint32_t cpu0_yuvb_int_st                 :  1; /**<bit[26 : 26] */
		uint32_t cpu0_rott_int_st                 :  1; /**<bit[27 : 27] */
		uint32_t reserved0                        :  4; /**<bit[28 : 31] */
	};
	uint32_t v;
} sys_cpu0_int_32_63_status_t;


typedef volatile union {
	struct {
		uint32_t cpu1_dma0_nsec_intr_st           :  1; /**<bit[0 : 0] */
		uint32_t cpu1_encp_sec_intr_st            :  1; /**<bit[1 : 1] */
		uint32_t cpu1_encp_nsec_intr_st           :  1; /**<bit[2 : 2] */
		uint32_t cpu1_timer0_int_st               :  1; /**<bit[3 : 3] */
		uint32_t cpu1_uart_int_st                 :  1; /**<bit[4 : 4] */
		uint32_t cpu1_pwm0_int_st                 :  1; /**<bit[5 : 5] */
		uint32_t cpu1_i2c0_int_st                 :  1; /**<bit[6 : 6] */
		uint32_t cpu1_spi0_int_st                 :  1; /**<bit[7 : 7] */
		uint32_t cpu1_sadc_int_st                 :  1; /**<bit[8 : 8] */
		uint32_t cpu1_irda_int_st                 :  1; /**<bit[9 : 9] */
		uint32_t cpu1_sdio_int_st                 :  1; /**<bit[10 : 10] */
		uint32_t cpu1_gdma_int_st                 :  1; /**<bit[11 : 11] */
		uint32_t cpu1_la_int_st                   :  1; /**<bit[12 : 12] */
		uint32_t cpu1_timer1_int_st               :  1; /**<bit[13 : 13] */
		uint32_t cpu1_i2c1_int_st                 :  1; /**<bit[14 : 14] */
		uint32_t cpu1_uart1_int_st                :  1; /**<bit[15 : 15] */
		uint32_t cpu1_uart2_int_st                :  1; /**<bit[16 : 16] */
		uint32_t cpu1_spi1_int_st                 :  1; /**<bit[17 : 17] */
		uint32_t cpu1_can_int_st                  :  1; /**<bit[18 : 18] */
		uint32_t cpu1_usb_int_st                  :  1; /**<bit[19 : 19] */
		uint32_t cpu1_qspi0_int_st                :  1; /**<bit[20 : 20] */
		uint32_t cpu1_fft_int_st                  :  1; /**<bit[21 : 21] */
		uint32_t cpu1_sbc_int_st                  :  1; /**<bit[22 : 22] */
		uint32_t cpu1_aud_int_st                  :  1; /**<bit[23 : 23] */
		uint32_t cpu1_i2s0_int_st                 :  1; /**<bit[24 : 24] */
		uint32_t cpu1_jpegenc_int_st              :  1; /**<bit[25 : 25] */
		uint32_t cpu1_jpegdec_int_st              :  1; /**<bit[26 : 26] */
		uint32_t cpu1_lcd_display_int_st          :  1; /**<bit[27 : 27] */
		uint32_t cpu1_dma2d_int_st                :  1; /**<bit[28 : 28] */
		uint32_t cpu1_wifi_int_phy_mpb_st         :  1; /**<bit[29 : 29] */
		uint32_t cpu1_wifi_int_phy_riu_st         :  1; /**<bit[30 : 30] */
		uint32_t cpu1_wifi_mac_int_tx_rx_timer_st :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_cpu1_int_0_31_status_t;


typedef volatile union {
	struct {
		uint32_t cpu1_wifi_mac_int_tx_rx_misc_st  :  1; /**<bit[0 : 0] */
		uint32_t cpu1_wifi_mac_int_rx_trigger_st  :  1; /**<bit[1 : 1] */
		uint32_t cpu1_wifi_mac_int_tx_trigger_st  :  1; /**<bit[2 : 2] */
		uint32_t cpu1_wifi_mac_int_prot_trigger_st :  1; /**<bit[3 : 3] */
		uint32_t cpu1_wifi_mac_int_gen_st         :  1; /**<bit[4 : 4] */
		uint32_t cpu1_wifi_hsu_irq_st             :  1; /**<bit[5 : 5] */
		uint32_t cpu1_wifi_int_mac_wakeup_st      :  1; /**<bit[6 : 6] */
		uint32_t cpu1_dm_irq_st                   :  1; /**<bit[7 : 7] */
		uint32_t cpu1_ble_irq_st                  :  1; /**<bit[8 : 8] */
		uint32_t cpu1_bt_irq_st                   :  1; /**<bit[9 : 9] */
		uint32_t cpu1_qspi1_int_st                :  1; /**<bit[10 : 10] */
		uint32_t cpu1_pwm1_int_st                 :  1; /**<bit[11 : 11] */
		uint32_t cpu1_i2s1_int_st                 :  1; /**<bit[12 : 12] */
		uint32_t cpu1_i2s2_int_st                 :  1; /**<bit[13 : 13] */
		uint32_t cpu1_h264_int_st                 :  1; /**<bit[14 : 14] */
		uint32_t cpu1_sdmadc_int_st               :  1; /**<bit[15 : 15] */
		uint32_t cpu1_mbox0_int_st                :  1; /**<bit[16 : 16] */
		uint32_t cpu1_mbox1_int_st                :  1; /**<bit[17 : 17] */
		uint32_t cpu1_bmc64_int_st                :  1; /**<bit[18 : 18] */
		uint32_t cpu1_dpll_unlock_int_st          :  1; /**<bit[19 : 19] */
		uint32_t cpu1_touched_int_st              :  1; /**<bit[20 : 20] */
		uint32_t cpu1_usbplug_int_st              :  1; /**<bit[21 : 21] */
		uint32_t cpu1_rtc_int_st                  :  1; /**<bit[22 : 22] */
		uint32_t cpu1_gpio_int_st                 :  1; /**<bit[23 : 23] */
		uint32_t cpu1_dma1_sec_int_st             :  1; /**<bit[24 : 24] */
		uint32_t cpu1_dma1_nsec_int_st            :  1; /**<bit[25 : 25] */
		uint32_t cpu1_yuvb_int_st                 :  1; /**<bit[26 : 26] */
		uint32_t cpu1_rott_int_st                 :  1; /**<bit[27 : 27] */
		uint32_t reserved0                        :  4; /**<bit[28 : 31] */
	};
	uint32_t v;
} sys_cpu1_int_32_63_status_t;


typedef volatile union {
	struct {
		uint32_t gpio_config0                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} sys_gpio_config0_t;


typedef volatile union {
	struct {
		uint32_t gpio_config1                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} sys_gpio_config1_t;


typedef volatile union {
	struct {
		uint32_t gpio_config2                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} sys_gpio_config2_t;


typedef volatile union {
	struct {
		uint32_t gpio_config3                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} sys_gpio_config3_t;


typedef volatile union {
	struct {
		uint32_t gpio_config4                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} sys_gpio_config4_t;


typedef volatile union {
	struct {
		uint32_t gpio_config5                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} sys_gpio_config5_t;


typedef volatile union {
	struct {
		uint32_t dbug_config0                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} sys_sys_debug_config0_t;


typedef volatile union {
	struct {
		uint32_t dbug_config1                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} sys_sys_debug_config1_t;


typedef volatile union {
	struct {
		uint32_t dpll_tsten                       :  1; /**<bit[0 : 0] */
		uint32_t cp                               :  3; /**<bit[1 : 3] */
		uint32_t spideten                         :  1; /**<bit[4 : 4] */
		uint32_t hvref                            :  2; /**<bit[5 : 6] */
		uint32_t lvref                            :  2; /**<bit[7 : 8] */
		uint32_t rzctrl26m                        :  1; /**<bit[9 : 9] */
		uint32_t looprzctrl                       :  4; /**<bit[10 : 13] */
		uint32_t rpc                              :  2; /**<bit[14 : 15] */
		uint32_t openloop_en                      :  1; /**<bit[16 : 16] */
		uint32_t cksel                            :  2; /**<bit[17 : 18] */
		uint32_t spitrig                          :  1; /**<bit[19 : 19] */
		uint32_t band0                            :  1; /**<bit[20 : 20] */
		uint32_t band1                            :  1; /**<bit[21 : 21] */
		uint32_t band                             :  3; /**<bit[22 : 24] */
		uint32_t bandmanual                       :  1; /**<bit[25 : 25] */
		uint32_t dsptrig                          :  1; /**<bit[26 : 26] */
		uint32_t lpen_dpll                        :  1; /**<bit[27 : 27] */
		uint32_t nc_28_30                         :  3; /**<bit[28 : 30] */
		uint32_t vctrl_dpllldo                    :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_ana_reg0_t;


typedef volatile union {
	struct {
		uint32_t nc3                              :  1; /**<bit[0 : 0] */
		uint32_t nc2                              :  1; /**<bit[1 : 1] */
		uint32_t msw                              :  9; /**<bit[2 : 10] */
		uint32_t ictrl                            :  3; /**<bit[11 : 13] */
		uint32_t osc_trig                         :  1; /**<bit[14 : 14] */
		uint32_t osccal_trig                      :  1; /**<bit[15 : 15] */
		uint32_t cnti                             :  9; /**<bit[16 : 24] */
		uint32_t spi_rst                          :  1; /**<bit[25 : 25] */
		uint32_t amsel                            :  1; /**<bit[26 : 26] */
		uint32_t divctrl                          :  3; /**<bit[27 : 29] */
		uint32_t nc1                              :  1; /**<bit[30 : 30] */
		uint32_t nc0                              :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_ana_reg1_t;


typedef volatile union {
	struct {
		uint32_t xtalh_ctune                      :  8; /**<bit[0 : 7] */
		uint32_t force_26mpll                     :  1; /**<bit[8 : 8] */
		uint32_t gadc_cmp_ictrl                   :  2; /**<bit[9 : 10] */
		uint32_t gadc_inbuf_ictrl                 :  2; /**<bit[11 : 12] */
		uint32_t gadc_refbuf_ictrl                :  2; /**<bit[13 : 14] */
		uint32_t gadc_nobuf_enable                :  1; /**<bit[15 : 15] */
		uint32_t vref_scale                       :  1; /**<bit[16 : 16] */
		uint32_t scal_en                          :  1; /**<bit[17 : 17] */
		uint32_t gadc_capcal_en                   :  1; /**<bit[18 : 18] */
		uint32_t gadc_capcal                      :  6; /**<bit[19 : 24] */
		uint32_t sp_nt_ctrl                       :  7; /**<bit[25 : 31] */
	};
	uint32_t v;
} sys_ana_reg2_t;


typedef volatile union {
	struct {
		uint32_t usbpen                           :  4; /**<bit[0 : 3] */
		uint32_t usbnen                           :  4; /**<bit[4 : 7] */
		uint32_t hpssren                          :  1; /**<bit[8 : 8] */
		uint32_t ck_sel                           :  1; /**<bit[9 : 9] */
		uint32_t anabuf_sel_rx                    :  1; /**<bit[10 : 10] */
		uint32_t pwd_xtalldo                      :  1; /**<bit[11 : 11] */
		uint32_t iamp                             :  1; /**<bit[12 : 12] */
		uint32_t vddren                           :  1; /**<bit[13 : 13] */
		uint32_t xamp                             :  6; /**<bit[14 : 19] */
		uint32_t vosel                            :  5; /**<bit[20 : 24] */
		uint32_t en_xtalh_sleep                   :  1; /**<bit[25 : 25] */
		uint32_t xtal40_en                        :  1; /**<bit[26 : 26] */
		uint32_t bufictrl                         :  1; /**<bit[27 : 27] */
		uint32_t ibias_ctrl                       :  2; /**<bit[28 : 29] */
		uint32_t icore_ctrl                       :  2; /**<bit[30 : 31] */
	};
	uint32_t v;
} sys_ana_reg3_t;


typedef volatile union {
	struct {
		uint32_t anabuf_sel_tx                    :  1; /**<bit[0 : 0] */
		uint32_t trng_tsten                       :  1; /**<bit[1 : 1] */
		uint32_t itune_ref                        :  3; /**<bit[2 : 4] */
		uint32_t itune_opa                        :  3; /**<bit[5 : 7] */
		uint32_t itune_cmp                        :  3; /**<bit[8 : 10] */
		uint32_t rnooise_sel                      :  1; /**<bit[11 : 11] */
		uint32_t fslow_sel                        :  3; /**<bit[12 : 14] */
		uint32_t ffast_sel                        :  4; /**<bit[15 : 18] */
		uint32_t trng_tstck_sel                   :  1; /**<bit[19 : 19] */
		uint32_t cktst_sel                        :  2; /**<bit[20 : 21] */
		uint32_t ck_tst_enbale                    :  1; /**<bit[22 : 22] */
		uint32_t sw_bias                          :  1; /**<bit[23 : 23] */
		uint32_t crb                              :  4; /**<bit[24 : 27] */
		uint32_t port_enablel                     :  4; /**<bit[28 : 31] */
	};
	uint32_t v;
} sys_ana_reg4_t;


typedef volatile union {
	struct {
		uint32_t en_usb                           :  1; /**<bit[0 : 0] */
		uint32_t en_xtall                         :  1; /**<bit[1 : 1] */
		uint32_t en_dco                           :  1; /**<bit[2 : 2] */
		uint32_t en_ram                           :  1; /**<bit[3 : 3] */
		uint32_t en_temp                          :  1; /**<bit[4 : 4] */
		uint32_t en_dpll                          :  1; /**<bit[5 : 5] */
		uint32_t en_cb                            :  1; /**<bit[6 : 6] */
		uint32_t en_lcd                           :  1; /**<bit[7 : 7] */
		uint32_t trxspi_ctrl                      :  2; /**<bit[8 : 9] */
		uint32_t adc_div                          :  2; /**<bit[10 : 11] */
		uint32_t usb_speed                        :  1; /**<bit[12 : 12] */
		uint32_t spideepsleep                     :  1; /**<bit[13 : 13] */
		uint32_t vsel                             :  1; /**<bit[14 : 14] */
		uint32_t swb                              :  1; /**<bit[15 : 15] */
		uint32_t itune_xtall                      :  4; /**<bit[16 : 19] */
		uint32_t xtall_ten                        :  1; /**<bit[20 : 20] */
		uint32_t rosc_tsten                       :  1; /**<bit[21 : 21] */
		uint32_t bcal_start                       :  1; /**<bit[22 : 22] */
		uint32_t bcal_en                          :  1; /**<bit[23 : 23] */
		uint32_t bcal_sel                         :  3; /**<bit[24 : 26] */
		uint32_t vbias                            :  5; /**<bit[27 : 31] */
	};
	uint32_t v;
} sys_ana_reg5_t;


typedef volatile union {
	struct {
		uint32_t calib_interval                   : 10; /**<bit[0 : 9] */
		uint32_t modify_interval                  :  6; /**<bit[10 : 15] */
		uint32_t xtal_wakeup_time                 :  4; /**<bit[16 : 19] */
		uint32_t spi_trig                         :  1; /**<bit[20 : 20] */
		uint32_t modifi_auto                      :  1; /**<bit[21 : 21] */
		uint32_t calib_auto                       :  1; /**<bit[22 : 22] */
		uint32_t cal_mode                         :  1; /**<bit[23 : 23] */
		uint32_t manu_ena                         :  1; /**<bit[24 : 24] */
		uint32_t manu_cin                         :  7; /**<bit[25 : 31] */
	};
	uint32_t v;
} sys_ana_reg6_t;


typedef volatile union {
	struct {
		uint32_t port_enablel                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} sys_ana_reg7_t;


typedef volatile union {
	struct {
		uint32_t ioldo_lp                         :  1; /**<bit[0 : 0] */
		uint32_t coreldo_hp                       :  1; /**<bit[1 : 1] */
		uint32_t dldohp                           :  1; /**<bit[2 : 2] */
		uint32_t t_vanaldosel                     :  3; /**<bit[3 : 5] */
		uint32_t r_vanaldosel                     :  3; /**<bit[6 : 8] */
		uint32_t en_trsw                          :  1; /**<bit[9 : 9] */
		uint32_t aldohp                           :  1; /**<bit[10 : 10] */
		uint32_t anacurlim                        :  1; /**<bit[11 : 11] */
		uint32_t violdosel                        :  3; /**<bit[12 : 14] */
		uint32_t iocurlim                         :  1; /**<bit[15 : 15] */
		uint32_t valoldosel                       :  3; /**<bit[16 : 18] */
		uint32_t alopowsel                        :  1; /**<bit[19 : 19] */
		uint32_t en_fast_aloldo                   :  1; /**<bit[20 : 20] */
		uint32_t aloldohp                         :  1; /**<bit[21 : 21] */
		uint32_t bgcal                            :  6; /**<bit[22 : 27] */
		uint32_t vbgcalmode                       :  1; /**<bit[28 : 28] */
		uint32_t vbgcalstart                      :  1; /**<bit[29 : 29] */
		uint32_t pwd_bgcal                        :  1; /**<bit[30 : 30] */
		uint32_t spi_envbg                        :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_ana_reg8_t;


typedef volatile union {
	struct {
		uint32_t wkgpiosel1                       :  4; /**<bit[0 : 3] */
		uint32_t rst_wks1v                        :  1; /**<bit[4 : 4] */
		uint32_t wkgpiosel2                       :  4; /**<bit[5 : 8] */
		uint32_t spi_latch1v                      :  1; /**<bit[9 : 9] */
		uint32_t digcurlim                        :  1; /**<bit[10 : 10] */
		uint32_t pupres_enb1v                     :  1; /**<bit[11 : 11] */
		uint32_t pdnres_en1v                      :  1; /**<bit[12 : 12] */
		uint32_t d_veasel1v                       :  2; /**<bit[13 : 14] */
		uint32_t ensfsdd                          :  1; /**<bit[15 : 15] */
		uint32_t vcorehsel                        :  4; /**<bit[16 : 19] */
		uint32_t vcorelsel                        :  3; /**<bit[20 : 22] */
		uint32_t vlden                            :  1; /**<bit[23 : 23] */
		uint32_t en_fast_coreldo                  :  1; /**<bit[24 : 24] */
		uint32_t pwdcoreldo                       :  1; /**<bit[25 : 25] */
		uint32_t vdighsel                         :  3; /**<bit[26 : 28] */
		uint32_t vdigsel                          :  2; /**<bit[29 : 30] */
		uint32_t vdd12lden                        :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_ana_reg9_t;


typedef volatile union {
	struct {
		uint32_t pasoft_st                        :  4; /**<bit[0 : 3] */
		uint32_t azcdcnt_manu                     :  3; /**<bit[4 : 6] */
		uint32_t timer_sel                        :  4; /**<bit[7 : 10] */
		uint32_t vpabucksel                       :  4; /**<bit[11 : 14] */
		uint32_t spi_timerwken                    :  1; /**<bit[15 : 15] */
		uint32_t nc_16_16                         :  1; /**<bit[16 : 16] */
		uint32_t sd                               :  1; /**<bit[17 : 17] */
		uint32_t ioldosel                         :  1; /**<bit[18 : 18] */
		uint32_t iobyapssen                       :  1; /**<bit[19 : 19] */
		uint32_t ckfs                             :  2; /**<bit[20 : 21] */
		uint32_t ckintsel                         :  1; /**<bit[22 : 22] */
		uint32_t osccaltrig                       :  1; /**<bit[23 : 23] */
		uint32_t mroscsel                         :  1; /**<bit[24 : 24] */
		uint32_t mrosci_cal                       :  3; /**<bit[25 : 27] */
		uint32_t mrosccap_cal                     :  4; /**<bit[28 : 31] */
	};
	uint32_t v;
} sys_ana_reg10_t;


typedef volatile union {
	struct {
		uint32_t sfsr                             :  4; /**<bit[0 : 3] */
		uint32_t ensfsaa                          :  1; /**<bit[4 : 4] */
		uint32_t apfms                            :  5; /**<bit[5 : 9] */
		uint32_t atmpo_sel                        :  2; /**<bit[10 : 11] */
		uint32_t ampoen                           :  1; /**<bit[12 : 12] */
		uint32_t enpowa                           :  1; /**<bit[13 : 13] */
		uint32_t avea_sel                         :  2; /**<bit[14 : 15] */
		uint32_t aforcepfm                        :  1; /**<bit[16 : 16] */
		uint32_t acls                             :  3; /**<bit[17 : 19] */
		uint32_t aswrsten                         :  1; /**<bit[20 : 20] */
		uint32_t aripc                            :  3; /**<bit[21 : 23] */
		uint32_t arampc                           :  4; /**<bit[24 : 27] */
		uint32_t arampcen                         :  1; /**<bit[28 : 28] */
		uint32_t aenburst                         :  1; /**<bit[29 : 29] */
		uint32_t apfmen                           :  1; /**<bit[30 : 30] */
		uint32_t aldosel                          :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_ana_reg11_t;


typedef volatile union {
	struct {
		uint32_t buckd_softst                     :  4; /**<bit[0 : 3] */
		uint32_t dzcdcnt_manu                     :  3; /**<bit[4 : 6] */
		uint32_t clk_sel                          :  1; /**<bit[7 : 7] */
		uint32_t dpfms                            :  5; /**<bit[8 : 12] */
		uint32_t dtmpo_sel                        :  2; /**<bit[13 : 14] */
		uint32_t dmpoen                           :  1; /**<bit[15 : 15] */
		uint32_t dforcepfm                        :  1; /**<bit[16 : 16] */
		uint32_t dcls                             :  3; /**<bit[17 : 19] */
		uint32_t dswrsten                         :  1; /**<bit[20 : 20] */
		uint32_t dripc                            :  3; /**<bit[21 : 23] */
		uint32_t drampc                           :  4; /**<bit[24 : 27] */
		uint32_t drampcen                         :  1; /**<bit[28 : 28] */
		uint32_t denburst                         :  1; /**<bit[29 : 29] */
		uint32_t dpfmen                           :  1; /**<bit[30 : 30] */
		uint32_t dldosel                          :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_ana_reg12_t;


typedef volatile union {
	struct {
		uint32_t pwdovp1v                         :  1; /**<bit[0 : 0] */
		uint32_t asoft_stc                        :  4; /**<bit[1 : 4] */
		uint32_t volen                            :  1; /**<bit[5 : 5] */
		uint32_t dpfms                            :  5; /**<bit[6 : 10] */
		uint32_t dtmpo_sel                        :  2; /**<bit[11 : 12] */
		uint32_t dmpoen                           :  1; /**<bit[13 : 13] */
		uint32_t pavea_sel                        :  2; /**<bit[14 : 15] */
		uint32_t dforcepfm                        :  1; /**<bit[16 : 16] */
		uint32_t dcls                             :  3; /**<bit[17 : 19] */
		uint32_t dswrsten                         :  1; /**<bit[20 : 20] */
		uint32_t dripc                            :  3; /**<bit[21 : 23] */
		uint32_t drampc                           :  4; /**<bit[24 : 27] */
		uint32_t drampcen                         :  1; /**<bit[28 : 28] */
		uint32_t paenburst                        :  1; /**<bit[29 : 29] */
		uint32_t papfmen                          :  1; /**<bit[30 : 30] */
		uint32_t enbuckpa                         :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_ana_reg13_t;


typedef volatile union {
	struct {
		uint32_t chs                              : 16; /**<bit[0 : 15] */
		uint32_t en_lpmod                         :  1; /**<bit[16 : 16] */
		uint32_t cal_vth                          :  3; /**<bit[17 : 19] */
		uint32_t crg                              :  2; /**<bit[20 : 21] */
		uint32_t vrefs                            :  4; /**<bit[22 : 25] */
		uint32_t gain_s                           :  4; /**<bit[26 : 29] */
		uint32_t td_latch1v                       :  1; /**<bit[30 : 30] */
		uint32_t pwd_td                           :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_ana_reg14_t;


typedef volatile union {
	struct {
		uint32_t cal_number1v                     :  4; /**<bit[0 : 3] */
		uint32_t cal_period1v                     :  9; /**<bit[4 : 12] */
		uint32_t test_number1v                    :  4; /**<bit[13 : 16] */
		uint32_t test_period1v                    :  4; /**<bit[17 : 20] */
		uint32_t nc_21_21                         :  1; /**<bit[21 : 21] */
		uint32_t chs_sel_cal1v                    :  4; /**<bit[22 : 25] */
		uint32_t cal_done_clr1v                   :  1; /**<bit[26 : 26] */
		uint32_t en_cal_force1v                   :  1; /**<bit[27 : 27] */
		uint32_t en_cal_auto1v                    :  1; /**<bit[28 : 28] */
		uint32_t en_scm                           :  1; /**<bit[29 : 29] */
		uint32_t en_adcmod                        :  1; /**<bit[30 : 30] */
		uint32_t enfsr1v                          :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_ana_reg15_t;


typedef volatile union {
	struct {
		uint32_t int_en                           : 10; /**<bit[0 : 9] */
		uint32_t int_en16                         :  1; /**<bit[10 : 10] */
		uint32_t nc_11_15                         :  5; /**<bit[11 : 15] */
		uint32_t ckadc_sel                        :  1; /**<bit[16 : 16] */
		uint32_t int_clr_sel1v                    :  1; /**<bit[17 : 17] */
		uint32_t ctrl_ck2d                        :  1; /**<bit[18 : 18] */
		uint32_t ctrl_seri_cap                    :  1; /**<bit[19 : 19] */
		uint32_t en_testcmp1v                     :  1; /**<bit[20 : 20] */
		uint32_t en_man_wr1v                      :  1; /**<bit[21 : 21] */
		uint32_t en_manmod1v                      :  1; /**<bit[22 : 22] */
		uint32_t cap_calspi1v                     :  9; /**<bit[23 : 31] */
	};
	uint32_t v;
} sys_ana_reg16_t;


typedef volatile union {
	struct {
		uint32_t int_clr                          : 10; /**<bit[0 : 9] */
		uint32_t int_clr16                        :  1; /**<bit[10 : 10] */
		uint32_t nc_11_11                         :  1; /**<bit[11 : 11] */
		uint32_t int_clr_cal                      : 10; /**<bit[12 : 21] */
		uint32_t int_en_cal                       : 10; /**<bit[22 : 31] */
	};
	uint32_t v;
} sys_ana_reg17_t;


typedef volatile union {
	struct {
		uint32_t iselaud                          :  1; /**<bit[0 : 0] */
		uint32_t audck_rlcen1v                    :  1; /**<bit[1 : 1] */
		uint32_t lchckinven1v                     :  1; /**<bit[2 : 2] */
		uint32_t enaudbias                        :  1; /**<bit[3 : 3] */
		uint32_t enadcbias                        :  1; /**<bit[4 : 4] */
		uint32_t enmicbias                        :  1; /**<bit[5 : 5] */
		uint32_t adcckinven1v                     :  1; /**<bit[6 : 6] */
		uint32_t dacfb2st0v9                      :  1; /**<bit[7 : 7] */
		uint32_t nc1                              :  1; /**<bit[8 : 8] */
		uint32_t micbias_trm                      :  2; /**<bit[9 : 10] */
		uint32_t micbias_voc                      :  5; /**<bit[11 : 15] */
		uint32_t vrefsel1v                        :  1; /**<bit[16 : 16] */
		uint32_t capswspi                         :  5; /**<bit[17 : 21] */
		uint32_t adref_sel                        :  2; /**<bit[22 : 23] */
		uint32_t nc0                              :  2; /**<bit[24 : 25] */
		uint32_t reserved_bit_26_30               :  5; /**<bit[26 : 30] */
		uint32_t spi_dacckpssel                   :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_ana_reg18_t;


typedef volatile union {
	struct {
		uint32_t isel                             :  2; /**<bit[0 : 1] */
		uint32_t micirsel1                        :  1; /**<bit[2 : 2] */
		uint32_t micdacit                         :  2; /**<bit[3 : 4] */
		uint32_t micdacih                         :  8; /**<bit[5 : 12] */
		uint32_t micsingleen                      :  1; /**<bit[13 : 13] */
		uint32_t dccompen                         :  1; /**<bit[14 : 14] */
		uint32_t micgain                          :  4; /**<bit[15 : 18] */
		uint32_t micdacen                         :  1; /**<bit[19 : 19] */
		uint32_t stg2lsen1v                       :  1; /**<bit[20 : 20] */
		uint32_t openloopcal1v                    :  1; /**<bit[21 : 21] */
		uint32_t callatch                         :  1; /**<bit[22 : 22] */
		uint32_t vcmsel                           :  1; /**<bit[23 : 23] */
		uint32_t dwamode                          :  1; /**<bit[24 : 24] */
		uint32_t r2ren                            :  1; /**<bit[25 : 25] */
		uint32_t nc_26_27                         :  2; /**<bit[26 : 27] */
		uint32_t micen                            :  1; /**<bit[28 : 28] */
		uint32_t rst                              :  1; /**<bit[29 : 29] */
		uint32_t bpdwa1v                          :  1; /**<bit[30 : 30] */
		uint32_t hcen1stg                         :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_ana_reg19_t;


typedef volatile union {
	struct {
		uint32_t hpdac                            :  1; /**<bit[0 : 0] */
		uint32_t calcon_sel                       :  1; /**<bit[1 : 1] */
		uint32_t oscdac                           :  2; /**<bit[2 : 3] */
		uint32_t ocendac                          :  1; /**<bit[4 : 4] */
		uint32_t vcmsel                           :  1; /**<bit[5 : 5] */
		uint32_t adjdacref                        :  5; /**<bit[6 : 10] */
		uint32_t dcochg                           :  2; /**<bit[11 : 12] */
		uint32_t diffen                           :  1; /**<bit[13 : 13] */
		uint32_t endaccal                         :  1; /**<bit[14 : 14] */
		uint32_t nc2                              :  1; /**<bit[15 : 15] */
		uint32_t lendcoc                          :  1; /**<bit[16 : 16] */
		uint32_t nc1                              :  1; /**<bit[17 : 17] */
		uint32_t lenvcmd                          :  1; /**<bit[18 : 18] */
		uint32_t dacdrven                         :  1; /**<bit[19 : 19] */
		uint32_t nc0                              :  1; /**<bit[20 : 20] */
		uint32_t daclen                           :  1; /**<bit[21 : 21] */
		uint32_t dacg                             :  4; /**<bit[22 : 25] */
		uint32_t dacmute                          :  1; /**<bit[26 : 26] */
		uint32_t dacdwamode_sel                   :  1; /**<bit[27 : 27] */
		uint32_t dacsel                           :  4; /**<bit[28 : 31] */
	};
	uint32_t v;
} sys_ana_reg20_t;


typedef volatile union {
	struct {
		uint32_t lmdcin                           :  8; /**<bit[0 : 7] */
		uint32_t nc1                              :  8; /**<bit[8 : 15] */
		uint32_t spirst_ovc                       :  1; /**<bit[16 : 16] */
		uint32_t nc0                              :  1; /**<bit[17 : 17] */
		uint32_t enidacl                          :  1; /**<bit[18 : 18] */
		uint32_t dac3rdhc0v9                      :  1; /**<bit[19 : 19] */
		uint32_t hc2s                             :  1; /**<bit[20 : 20] */
		uint32_t rfb_ctrl                         :  1; /**<bit[21 : 21] */
		uint32_t vcmsel                           :  1; /**<bit[22 : 22] */
		uint32_t enbs                             :  1; /**<bit[23 : 23] */
		uint32_t calck_sel0v9                     :  1; /**<bit[24 : 24] */
		uint32_t bpdwa0v9                         :  1; /**<bit[25 : 25] */
		uint32_t looprst0v9                       :  1; /**<bit[26 : 26] */
		uint32_t oct0v9                           :  2; /**<bit[27 : 28] */
		uint32_t sout0v9                          :  1; /**<bit[29 : 29] */
		uint32_t hc0v9                            :  2; /**<bit[30 : 31] */
	};
	uint32_t v;
} sys_ana_reg21_t;

typedef volatile union {
	struct {
		uint32_t ictrl_dsppll                     :  4; /**<bit[0 : 3] */
		uint32_t nc_4_18                          : 15; /**<bit[4 : 18] */
		uint32_t mode                             :  1; /**<bit[19 : 19] */
		uint32_t iamsel                           :  1; /**<bit[20 : 20] */
		uint32_t hvref                            :  2; /**<bit[21 : 22] */
		uint32_t lvref                            :  2; /**<bit[23 : 24] */
		uint32_t nc_25_31                         :  7; /**<bit[25 : 31] */
	};
	uint32_t v;
} sys_ana_reg22_t;

typedef volatile union {
	struct {
		uint32_t camsel                           :  1; /**<bit[0 : 0] */
		uint32_t msw                              :  9; /**<bit[1 : 9] */
		uint32_t tstcken_dpll                     :  1; /**<bit[10 : 10] */
		uint32_t osccal_trig                      :  1; /**<bit[11 : 11] */
		uint32_t cnti                             :  9; /**<bit[12 : 20] */
		uint32_t nc_21_21                         :  1; /**<bit[21 : 21] */
		uint32_t spi_rst                          :  1; /**<bit[22 : 22] */
		uint32_t closeloop_en                     :  1; /**<bit[23 : 23] */
		uint32_t caltime                          :  1; /**<bit[24 : 24] */
		uint32_t lpfrz                            :  2; /**<bit[25 : 26] */
		uint32_t icp                              :  4; /**<bit[27 : 30] */
		uint32_t cp2ctrl                          :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_ana_reg23_t;

typedef volatile struct {
	volatile sys_device_id_t device_id;
	volatile sys_version_id_t version_id;
	volatile sys_cpu_storage_connect_op_select_t cpu_storage_connect_op_select;
	volatile sys_cpu_current_run_status_t cpu_current_run_status;
	volatile sys_cpu0_int_halt_clk_op_t cpu0_int_halt_clk_op;
	volatile sys_cpu1_int_halt_clk_op_t cpu1_int_halt_clk_op;
	volatile sys_reserved_reg0x6_t reserved_reg0x6;
	volatile uint32_t rsv_7_7[1];
	volatile sys_cpu_clk_div_mode1_t cpu_clk_div_mode1;
	volatile sys_cpu_clk_div_mode2_t cpu_clk_div_mode2;
	volatile sys_cpu_26m_wdt_clk_div_t cpu_26m_wdt_clk_div;
	volatile sys_cpu_anaspi_freq_t cpu_anaspi_freq;
	volatile sys_cpu_device_clk_enable_t cpu_device_clk_enable;
	volatile sys_reserver_reg0xd_t reserver_reg0xd;
	volatile sys_cpu_device_mem_ctrl1_t cpu_device_mem_ctrl1;
	volatile sys_cpu_device_mem_ctrl2_t cpu_device_mem_ctrl2;
	volatile sys_cpu_power_sleep_wakeup_t cpu_power_sleep_wakeup;
	volatile sys_reserver_reg0x11_t reserver_reg0x11;
	volatile sys_cpu_device_mem_ctrl3_t cpu_device_mem_ctrl3;
	volatile uint32_t rsv_13_1f[13];
	volatile sys_cpu0_int_0_31_en_t cpu0_int_0_31_en;
	volatile sys_cpu0_int_32_63_en_t cpu0_int_32_63_en;
	volatile sys_cpu1_int_0_31_en_t cpu1_int_0_31_en;
	volatile sys_cpu1_int_32_63_en_t cpu1_int_32_63_en;
	volatile uint32_t rsv_24_27[4];
	volatile sys_cpu0_int_0_31_status_t cpu0_int_0_31_status;
	volatile sys_cpu0_int_32_63_status_t cpu0_int_32_63_status;
	volatile sys_cpu1_int_0_31_status_t cpu1_int_0_31_status;
	volatile sys_cpu1_int_32_63_status_t cpu1_int_32_63_status;
	volatile uint32_t rsv_2c_2f[4];
	volatile sys_gpio_config0_t gpio_config0;
	volatile sys_gpio_config1_t gpio_config1;
	volatile sys_gpio_config2_t gpio_config2;
	volatile sys_gpio_config3_t gpio_config3;
	volatile sys_gpio_config4_t gpio_config4;
	volatile sys_gpio_config5_t gpio_config5;
	volatile uint32_t rsv_36_37[2];
	volatile sys_sys_debug_config0_t sys_debug_config0;
	volatile sys_sys_debug_config1_t sys_debug_config1;
	volatile uint32_t rsv_3a_3f[6];
	volatile sys_ana_reg0_t ana_reg0;
	volatile sys_ana_reg1_t ana_reg1;
	volatile sys_ana_reg2_t ana_reg2;
	volatile sys_ana_reg3_t ana_reg3;
	volatile sys_ana_reg4_t ana_reg4;
	volatile sys_ana_reg5_t ana_reg5;
	volatile sys_ana_reg6_t ana_reg6;
	volatile sys_ana_reg7_t ana_reg7;
	volatile sys_ana_reg8_t ana_reg8;
	volatile sys_ana_reg9_t ana_reg9;
	volatile sys_ana_reg10_t ana_reg10;
	volatile sys_ana_reg11_t ana_reg11;
	volatile sys_ana_reg12_t ana_reg12;
	volatile sys_ana_reg13_t ana_reg13;
	volatile sys_ana_reg14_t ana_reg14;
	volatile sys_ana_reg15_t ana_reg15;
	volatile sys_ana_reg16_t ana_reg16;
	volatile sys_ana_reg17_t ana_reg17;
	volatile sys_ana_reg18_t ana_reg18;
	volatile sys_ana_reg19_t ana_reg19;
	volatile sys_ana_reg20_t ana_reg20;
	volatile sys_ana_reg21_t ana_reg21;
	volatile sys_ana_reg22_t ana_reg22;
	volatile sys_ana_reg23_t ana_reg23;
} sys_hw_t;

#ifdef __cplusplus
}
#endif
