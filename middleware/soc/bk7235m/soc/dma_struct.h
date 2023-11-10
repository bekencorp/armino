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
    //REG_0x00~REG_0x5F
    struct {
        union {
            struct {
                uint32_t enable:             1; /**< bit[0] DMA enable */
                uint32_t finish_int_en:      1; /**< bit[1] transfer finish interrupt enable */
                uint32_t half_finish_int_en: 1; /**< bit[2] half transfer finish interrupt enable */
                uint32_t mode:               1; /**< bit[3] DMA work mode */
                uint32_t src_data_width:     2; /**< bit[4:5] source data width */
                uint32_t dest_data_width:    2; /**< bit[6:7] destination data width */
                uint32_t src_addr_inc_en:    1; /**< bit[8] source address increment enable */
                uint32_t dest_addr_inc_en:   1; /**< bit[9] dest address increment enable */
                uint32_t src_addr_loop_en:   1; /**< bit[10] source address loop enable */
                uint32_t dest_addr_loop_en:  1; /**< bit[11] dest address loop enable */
                uint32_t chan_prio:          3; /**< bit[12:14] channel prioprity */
                uint32_t reserved:           1; /**< bit[15] */
                uint32_t transfer_len:       16; /**< bit[16:31] DMA transfer length,unit is byte */
            };
            uint32_t v;
        } ctrl;

        uint32_t dest_start_addr;
        uint32_t src_start_addr;
        uint32_t dest_loop_end_addr;
        uint32_t dest_loop_start_addr;
        uint32_t src_loop_end_addr;
        uint32_t src_loop_start_addr;

        union {
            struct {
               /* Source Request Mux.(see dest_req_mux)
                * 0x0: DTCM/AHB_MEM read Req
                * 0x1: Uart0 rx Req
                * 0x2: Spi0 rx Req
                * 0x3: Sdio rx Req
                * 0x4: Uart1 rx Req
                * 0x5: Uart2 rx Req
                * 0x6: Spi1 rx Req
                * 0x7: Usb rx Req
                * 0x8: Aud rx Req
                * 0x9: I2S rx Req
                * 0xa: Jpeg rx Req
                * 0xb~0xf: Reserved
                */
                uint32_t src_req_mux:         5; /**< bit[0:4] source request mux */

               /* Destination Request Mux:
                * 0x0: DTCM/AHB_MEM Write Req
                * 0x1: Uart0 tx Req
                * 0x2: Spi0 tx Req
                * 0x3: Sdio tx Req
                * 0x4: Uart1 tx Req
                * 0x5: Uart2 tx Req
                * 0x6: Spi1 tx Req
                * 0x7: Usb tx Req
                * 0x8: Aud tx Req
                * 0x9: I2S tx Req
                * 0xa: lcd cmd tx Req
                * 0xb: lcd data tx Req
                * 0xc~0xf: Reserved
                */
                uint32_t dest_req_mux:        5; /**< bit[5:9] dest request mux */
                uint32_t reserved0:           2; /**< bit[10:11] */
                uint32_t src_read_interval:   4; /**< bit[12:15] source read operate interval,unit is cycle */
                uint32_t dest_write_interval: 4; /**< bit[16:19] destination write operate interval,unit is cycle */
                uint32_t reserved1:           12; /**< bit[20:31] */
            };
            uint32_t v;
        } req_mux;
    } config_group[SOC_DMA_CHAN_NUM_PER_UNIT];

    uint32_t reserved1[0x80 - 0x5f - 1];
    //REG_0x80~REG_0x8b
    uint32_t src_pause_addr[SOC_DMA_CHAN_NUM_PER_UNIT];

    uint32_t reserved2[0x90 - 0x8b - 1];
    //REG_0x90~REG_0x9b
    uint32_t dest_pause_addr[SOC_DMA_CHAN_NUM_PER_UNIT];

    uint32_t reserved3[0xa0 - 0x9b - 1];
    //REG_0xa0~REG_0xab
    uint32_t src_rd_addr[SOC_DMA_CHAN_NUM_PER_UNIT];

    uint32_t reserved4[0xb0 - 0xab - 1];
    //REG_0xb0~REG_0xbb
    uint32_t dest_wr_addr[SOC_DMA_CHAN_NUM_PER_UNIT];

    uint32_t reserved5[0xc0 - 0xbb - 1];
    //REG_0xc0~REG_0xcb
    struct {
        union {
            struct {
                uint32_t remain_len:              17; /**< bit[0:16] remain length indication */
                uint32_t flush_src_buff:          1; /**< bit[17] flush source buffer */
                uint32_t reserved:                6; /**< bit[18:23] */
                uint32_t finish_int_counter:      4; /**< bit[24:27] finish interrupt counter */
                uint32_t half_finish_int_counter: 4; /**< bit[28:31] half finish interrupt counter*/
            };
            uint32_t v;
        } status;
    } status_group[SOC_DMA_CHAN_NUM_PER_UNIT];

    uint32_t reserved6[0xd0 - 0xcb - 1];
    //REG_0xd0
    union {
        struct {
            uint32_t prio_mode: 1; /**< bit[0] prioprity mode */
            uint32_t reserved:  31; /**< bit[1:31] reserved */
        };
        uint32_t v;
    } prio_mode;

    //REG_0xd1
    union {
        struct {
            uint32_t finish_int_status:      12; /**< bit[0:11] finish interrupt status */
            uint32_t reserved0:              4;  /**< bit[12:15] */
            uint32_t half_finish_int_status: 12; /**< bit[16:27] half finish interrupt status */
            uint32_t reserved1:              4;  /**< bit[28:31] */
        };
        uint32_t v;
    } int_status;


} dma_hw_t;

#ifdef __cplusplus
}
#endif


