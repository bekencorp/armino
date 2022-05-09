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

/***********************************************************************************************************************************
* This file is generated from BK7256_ADDR Mapping_20211224_format_change_highlight_20220113_update.xlsm automatically                                
* Modify it manually is not recommended                                       
* CHIP ID:BK7256,GENARATE TIME:2022-04-02 18:47:01                                                 
************************************************************************************************************************************/

#pragma once                 
                            
#include "sdio_hw.h"        
#include "sdio_ll_macro_def.h"         

#ifdef __cplusplus          
extern "C" {              
#endif                      

#if 0

/* REG_0x00 */

static inline uint32_t sdio_ll_get_reg0x0_value(sdio_hw_t *hw)
{
    return hw->reg0x0.v;
}

static inline void sdio_ll_set_reg0x0_value(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x0.v = value;
}

/* REG_0x00:reg0x0->SD_CMD_START:0x0[0],sd host(Slave) start send command; Host:If SW set it to 1, sdio auto clear this bit and then start send command.; Slave:SW set it to 1, sdio will receive and response command if host send command, then sdio clear this bit; 0:If this bit is 0, sdio will no reponse command to host and reject command from host side.,0x0,RW/AutoC*/
static inline uint32_t sdio_ll_get_reg0x0_sd_cmd_start(sdio_hw_t *hw)
{
    return hw->reg0x0.sd_cmd_start;
}

static inline void sdio_ll_set_reg0x0_sd_cmd_start(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x0.sd_cmd_start = value;
}

/* REG_0x00:reg0x0->SD_CMD_RSP:0x0[1],Host Only; sd host need slave respond the command or not.; 1: need respond; 0: not need respond,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0x0_sd_cmd_rsp(sdio_hw_t *hw)
{
    return hw->reg0x0.sd_cmd_rsp;
}

static inline void sdio_ll_set_reg0x0_sd_cmd_rsp(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x0.sd_cmd_rsp = value;
}

/* REG_0x00:reg0x0->SD_CMD_LONG:0x0[2],Host Only; sd host need slave respond a long command or short 1: need long response ; 0: need short response,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0x0_sd_cmd_long(sdio_hw_t *hw)
{
    return hw->reg0x0.sd_cmd_long;
}

static inline void sdio_ll_set_reg0x0_sd_cmd_long(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x0.sd_cmd_long = value;
}

/* REG_0x00:reg0x0->SD_CMD_CRC_CHECK:0x0[3],sd host(Slave?) need to check the slave respond command crc or not ; 1: need check crc  ; 0: not check crc,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0x0_sd_cmd_crc_check(sdio_hw_t *hw)
{
    return hw->reg0x0.sd_cmd_crc_check;
}

static inline void sdio_ll_set_reg0x0_sd_cmd_crc_check(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x0.sd_cmd_crc_check = value;
}

/* REG_0x00:reg0x0->CMD_INDEX:0x0[9:4],Host Only; sd host send command index value,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0x0_cmd_index(sdio_hw_t *hw)
{
    return hw->reg0x0.cmd_index;
}

static inline void sdio_ll_set_reg0x0_cmd_index(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x0.cmd_index = value;
}

/* REG_0x01 */

static inline uint32_t sdio_ll_get_reg0x1_value(sdio_hw_t *hw)
{
    return hw->reg0x1.v;
}

static inline void sdio_ll_set_reg0x1_value(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x1.v = value;
}

/* REG_0x01:reg0x1->CMD_ARGUMENT:0x1[31:0],Host Only; sd host send command argument value; Host:CMD index …,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0x1_cmd_argument(sdio_hw_t *hw)
{
    return hw->reg0x1.v;
}

static inline void sdio_ll_set_reg0x1_cmd_argument(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x1.v = value;
}

/* REG_0x02 */

static inline uint32_t sdio_ll_get_reg0x2_value(sdio_hw_t *hw)
{
    return hw->reg0x2.v;
}

static inline void sdio_ll_set_reg0x2_value(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x2.v = value;
}

/* REG_0x02:reg0x2->SD_CMD_TIMER:0x2[31:0],Host Only; sd host wait the slave respond timer; What's the unit?,0xffffffff,RW*/
static inline uint32_t sdio_ll_get_reg0x2_sd_cmd_timer(sdio_hw_t *hw)
{
    return hw->reg0x2.v;
}

static inline void sdio_ll_set_reg0x2_sd_cmd_timer(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x2.v = value;
}

/* REG_0x03 */

static inline uint32_t sdio_ll_get_reg0x3_value(sdio_hw_t *hw)
{
    return hw->reg0x3.v;
}

static inline void sdio_ll_set_reg0x3_value(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x3.v = value;
}

/* REG_0x03:reg0x3->SD_DATA_EN:0x3[0],Host Only; sd host start receive slave data.; 1: start slave wr data to line set this bit; If host want to read more then 512bytes data, whether need to set it at every block?,0x0,RW/AutoC*/
static inline uint32_t sdio_ll_get_reg0x3_sd_data_en(sdio_hw_t *hw)
{
    return hw->reg0x3.sd_data_en;
}

static inline void sdio_ll_set_reg0x3_sd_data_en(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x3.sd_data_en = value;
}

/* REG_0x03:reg0x3->SD_DATA_STOP_EN:0x3[1],Host Only; sd host stop selfside transmid data; 1: stop; 0: start; Host can send CMD52 with REG0,Bit[0]=1 to slave and then slave stop transmid data,0x0,RW/AutoC*/
static inline uint32_t sdio_ll_get_reg0x3_sd_data_stop_en(sdio_hw_t *hw)
{
    return hw->reg0x3.sd_data_stop_en;
}

static inline void sdio_ll_set_reg0x3_sd_data_stop_en(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x3.sd_data_stop_en = value;
}

/* REG_0x03:reg0x3->SD_DATA_BUS:0x3[2],sd host(Slave) received data bus ; 1: 4 lines data  ; 0: 1 line data; When switch this bit, should check there is no data transmitting on the data line,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0x3_sd_data_bus(sdio_hw_t *hw)
{
    return hw->reg0x3.sd_data_bus;
}

static inline void sdio_ll_set_reg0x3_sd_data_bus(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x3.sd_data_bus = value;
}

/* REG_0x03:reg0x3->SD_DATA_MUL_BLK:0x3[3],Host Only; sd host received data multiple blocks ; 1: multiple blocks  ; 0: single block; If use bytes mode, should set this bit to single block mode.,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0x3_sd_data_mul_blk(sdio_hw_t *hw)
{
    return hw->reg0x3.sd_data_mul_blk;
}

static inline void sdio_ll_set_reg0x3_sd_data_mul_blk(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x3.sd_data_mul_blk = value;
}

/* REG_0x03:reg0x3->SD_DATA_BLK_SIZE:0x3[15:4],Host Only; sd host send data block size(Bytes) to slave, host controller uses this value to controll send how many bytes to slave with every block.; a.Host with SD-CARD mode:write valid, read invalid.; b.Host with SDIO-Device mode:write valid,read invalid(the length has been send to device by CMD53),0x0,RW*/
static inline uint32_t sdio_ll_get_reg0x3_sd_data_blk_size(sdio_hw_t *hw)
{
    return hw->reg0x3.sd_data_blk_size;
}

static inline void sdio_ll_set_reg0x3_sd_data_blk_size(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x3.sd_data_blk_size = value;
}

/* REG_0x03:reg0x3->SD_START_WR_EN:0x3[16],sd host(Slave) write data enable：start write data to data wires.,0x0,RW/AutoC*/
static inline uint32_t sdio_ll_get_reg0x3_sd_start_wr_en(sdio_hw_t *hw)
{
    return hw->reg0x3.sd_start_wr_en;
}

static inline void sdio_ll_set_reg0x3_sd_start_wr_en(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x3.sd_start_wr_en = value;
}

/* REG_0x03:reg0x3->SD_BYTE_SEL:0x3[17],sd host(Slave) received data byte arrangement ; 1: sd_rd = {sd_data[7:0],sd_data[15:8],sd_data[23:16],sd_data[31:24]} ; 0: sd_rd = sd_data[31:0],0x0,RW*/
static inline uint32_t sdio_ll_get_reg0x3_sd_byte_sel(sdio_hw_t *hw)
{
    return hw->reg0x3.sd_byte_sel;
}

static inline void sdio_ll_set_reg0x3_sd_byte_sel(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x3.sd_byte_sel = value;
}

/* REG_0x04 */

static inline uint32_t sdio_ll_get_reg0x4_value(sdio_hw_t *hw)
{
    return hw->reg0x4.v;
}

static inline void sdio_ll_set_reg0x4_value(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x4.v = value;
}

/* REG_0x04:reg0x4->SD_DATA_TIMER:0x4[31:0],Host:; sd host read data from slave till timeout,; what's write timeout mean?; Slave:; slave wait host write data to line till timeout,it doesn't check host read data time.,0xffffffff,RW*/
static inline uint32_t sdio_ll_get_reg0x4_sd_data_timer(sdio_hw_t *hw)
{
    return hw->reg0x4.v;
}

static inline void sdio_ll_set_reg0x4_sd_data_timer(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x4.v = value;
}

/* REG_0x05 */

static inline uint32_t sdio_ll_get_reg0x5_value(sdio_hw_t *hw)
{
    return hw->reg0x5.v;
}

/* REG_0x05:reg0x5->SD_RSP_AGUMENT_0:0x5[31:0],Host:sd host received slave command argument value; ; Slave:sd slave received host command argument.; The argument maybe CMD52/CMD53 and other value(Though other CMD isn't error by ASIC).Every CMD Response INT should check this value.,0x0,R*/
static inline uint32_t sdio_ll_get_reg0x5_sd_rsp_agument_0(sdio_hw_t *hw)
{
    return hw->reg0x5.v;
}

/* REG_0x06 */

static inline uint32_t sdio_ll_get_reg0x6_value(sdio_hw_t *hw)
{
    return hw->reg0x6.v;
}

/* REG_0x06:reg0x6->SD_RSP_AGUMENT_1:0x6[31:0],Host only; sd host received slave command argument value,0x0,R*/
static inline uint32_t sdio_ll_get_reg0x6_sd_rsp_agument_1(sdio_hw_t *hw)
{
    return hw->reg0x6.v;
}

/* REG_0x07 */

static inline uint32_t sdio_ll_get_reg0x7_value(sdio_hw_t *hw)
{
    return hw->reg0x7.v;
}

/* REG_0x07:reg0x7->SD_RSP_AGUMENT_2:0x7[31:0],Host only; sd host received slave command argument value,0x0,R*/
static inline uint32_t sdio_ll_get_reg0x7_sd_rsp_agument_2(sdio_hw_t *hw)
{
    return hw->reg0x7.v;
}

/* REG_0x08 */

static inline uint32_t sdio_ll_get_reg0x8_value(sdio_hw_t *hw)
{
    return hw->reg0x8.v;
}

/* REG_0x08:reg0x8->SD_RSP_AGUMENT_3:0x8[31:0],Host only; sd host received slave command argument value,0x0,R*/
static inline uint32_t sdio_ll_get_reg0x8_sd_rsp_agument_3(sdio_hw_t *hw)
{
    return hw->reg0x8.v;
}

/* REG_0x09 */

static inline uint32_t sdio_ll_get_reg0x9_value(sdio_hw_t *hw)
{
    return hw->reg0x9.v;
}

static inline void sdio_ll_set_reg0x9_value(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x9.v = value;
}

/* REG_0x09:reg0x9->SD_CMD_SEND_NORSP_END_INT:0x9[0],Host only; sd host send no response command end int.; Some CMD no needs response,so CMD sends finish then the sdio produced the CMD send finish INT,0x0,R/W1C*/
static inline uint32_t sdio_ll_get_reg0x9_sd_cmd_send_norsp_end_int(sdio_hw_t *hw)
{
    return hw->reg0x9.sd_cmd_send_norsp_end_int;
}

static inline void sdio_ll_set_reg0x9_sd_cmd_send_norsp_end_int(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x9.sd_cmd_send_norsp_end_int = value;
}

/* REG_0x09:reg0x9->SD_CMD_SEND_RSP_END_INT:0x9[1],Host only; sd host receive slave command end int,0x0,R/W1C*/
static inline uint32_t sdio_ll_get_reg0x9_sd_cmd_send_rsp_end_int(sdio_hw_t *hw)
{
    return hw->reg0x9.sd_cmd_send_rsp_end_int;
}

static inline void sdio_ll_set_reg0x9_sd_cmd_send_rsp_end_int(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x9.sd_cmd_send_rsp_end_int = value;
}

/* REG_0x09:reg0x9->SD_CMD_SEND_RSP_TIMEOUT_INT:0x9[2],sd host wait slave command over time int(What's diffrence with "no response command end int"),0x0,R/W1C*/
static inline uint32_t sdio_ll_get_reg0x9_sd_cmd_send_rsp_timeout_int(sdio_hw_t *hw)
{
    return hw->reg0x9.sd_cmd_send_rsp_timeout_int;
}

static inline void sdio_ll_set_reg0x9_sd_cmd_send_rsp_timeout_int(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x9.sd_cmd_send_rsp_timeout_int = value;
}

/* REG_0x09:reg0x9->SD_DATA_REC_END_INT:0x9[3],sd host receive slave data end int,0x0,R/W1C*/
static inline uint32_t sdio_ll_get_reg0x9_sd_data_rec_end_int(sdio_hw_t *hw)
{
    return hw->reg0x9.sd_data_rec_end_int;
}

static inline void sdio_ll_set_reg0x9_sd_data_rec_end_int(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x9.sd_data_rec_end_int = value;
}

/* REG_0x09:reg0x9->SD_DATA_WR_END_INT:0x9[4],sd host write data end int,it means slave has read data finish,notify host can send next block.(Slave has to write REG0x10 Bit[1] to 1(clear to 0 and then set to 1) to notify host send next block,or host can't send next block data),0x0,R/W1C*/
static inline uint32_t sdio_ll_get_reg0x9_sd_data_wr_end_int(sdio_hw_t *hw)
{
    return hw->reg0x9.sd_data_wr_end_int;
}

static inline void sdio_ll_set_reg0x9_sd_data_wr_end_int(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x9.sd_data_wr_end_int = value;
}

/* REG_0x09:reg0x9->SD_DATA_TIME_OUT_INT:0x9[5],sd host wait slave data over time int,0x0,R/W1C*/
static inline uint32_t sdio_ll_get_reg0x9_sd_data_time_out_int(sdio_hw_t *hw)
{
    return hw->reg0x9.sd_data_time_out_int;
}

static inline void sdio_ll_set_reg0x9_sd_data_time_out_int(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x9.sd_data_time_out_int = value;
}

/* REG_0x09:reg0x9->RX_FIFO_NEED_READ:0x9[6],sd host fifo memory need read int; Host:If FIFO has more then count of threshhold data,enable INT to CPU.; Slave:valid,0x0,R*/
static inline uint32_t sdio_ll_get_reg0x9_rx_fifo_need_read(sdio_hw_t *hw)
{
    return hw->reg0x9.rx_fifo_need_read;
}

/* REG_0x09:reg0x9->TX_FIFO_NEED_WRITE:0x9[7],sd host fifo memory need write int; Host:If FIFO has less then count of threshhold data,enable INT to CPU.; Slave:valid,0x0,R*/
static inline uint32_t sdio_ll_get_reg0x9_tx_fifo_need_write(sdio_hw_t *hw)
{
    return hw->reg0x9.tx_fifo_need_write;
}

/* REG_0x09:reg0x9->RX_OV:0x9[8],sd host fifo memory overflow int?; rx fifo full, host slave wr too many full, slave host wr too many data.; What's the overflow condition?,0x0,R/W1C*/
static inline uint32_t sdio_ll_get_reg0x9_rx_ov(sdio_hw_t *hw)
{
    return hw->reg0x9.rx_ov;
}

static inline void sdio_ll_set_reg0x9_rx_ov(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x9.rx_ov = value;
}

/* REG_0x09:reg0x9->TX_FIFO_EMPT:0x9[9],sd host fifo memory empty int,0x0,R*/
static inline uint32_t sdio_ll_get_reg0x9_tx_fifo_empt(sdio_hw_t *hw)
{
    return hw->reg0x9.tx_fifo_empt;
}

/* REG_0x09:reg0x9->SD_RSP_CMD_CRC_OK:0x9[10],sd host/slave received peer side slave/host command crc check indication signal 1: crc ok  ,0x0,R*/
static inline uint32_t sdio_ll_get_reg0x9_sd_rsp_cmd_crc_ok(sdio_hw_t *hw)
{
    return hw->reg0x9.sd_rsp_cmd_crc_ok;
}

/* REG_0x09:reg0x9->SD_RSP_CMD_CRC_FAIL:0x9[11],sd host/slave received peer side slave/host command crc check indication signal 1: crc fail ,0x0,R*/
static inline uint32_t sdio_ll_get_reg0x9_sd_rsp_cmd_crc_fail(sdio_hw_t *hw)
{
    return hw->reg0x9.sd_rsp_cmd_crc_fail;
}

/* REG_0x09:reg0x9->SD_DATA_CRC_OK:0x9[12],sd host/slave received peer side slave/host data crc check indication signal 1: crc ok,0x0,R*/
static inline uint32_t sdio_ll_get_reg0x9_sd_data_crc_ok(sdio_hw_t *hw)
{
    return hw->reg0x9.sd_data_crc_ok;
}

/* REG_0x09:reg0x9->SD_DATA_CRC_FAIL:0x9[13],sd host/slave received peer side slave/host data crc check indication signal 1: crc fail,0x0,R*/
static inline uint32_t sdio_ll_get_reg0x9_sd_data_crc_fail(sdio_hw_t *hw)
{
    return hw->reg0x9.sd_data_crc_fail;
}

/* REG_0x09:reg0x9->SD_RSP_INDEX:0x9[19:14],Host only; sd host received slave command index value,0x0,R*/
static inline uint32_t sdio_ll_get_reg0x9_sd_rsp_index(sdio_hw_t *hw)
{
    return hw->reg0x9.sd_rsp_index;
}

/* REG_0x09:reg0x9->WR_STATUS:0x9[22:20],sd host write data result(response from slave device); 0:; 1:; 2:Is right, else is error.; (Workaround:BK7256 read this value is 0x5 though slave responsed is 0x2); 3:; 4:; 5:; 6:; 7:,0x0,R*/
static inline uint32_t sdio_ll_get_reg0x9_wr_status(sdio_hw_t *hw)
{
    return hw->reg0x9.wr_status;
}

/* REG_0x09:reg0x9->DATA_BUSY:0x9[23],sd slave busy signal after sd host send special command and write data.Host check this bit to confirm slave is whether busy?,0x0,R*/
static inline uint32_t sdio_ll_get_reg0x9_data_busy(sdio_hw_t *hw)
{
    return hw->reg0x9.data_busy;
}

/* REG_0x09:reg0x9->CMD_S_RES_END_INT:0x9[24],Slave only; Slave has finish reponsed the CMD to host side.,0x0,R/W1C*/
static inline uint32_t sdio_ll_get_reg0x9_cmd_s_res_end_int(sdio_hw_t *hw)
{
    return hw->reg0x9.cmd_s_res_end_int;
}

static inline void sdio_ll_set_reg0x9_cmd_s_res_end_int(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x9.cmd_s_res_end_int = value;
}

/* REG_0x09:reg0x9->DAT_S_WR_WAI_INT:0x9[25],Slave:Host write and slave is reading,host should stop, INT notify CPU slave has received the data from host.; Slave write finish.; W1:Clear the finish status.; 1.If clear means slave can receive data again?; Read data-out first or clear INT first?; 2.The name of WR_WAT is hard to understand,0x0,R/W1C*/
static inline uint32_t sdio_ll_get_reg0x9_dat_s_wr_wai_int(sdio_hw_t *hw)
{
    return hw->reg0x9.dat_s_wr_wai_int;
}

static inline void sdio_ll_set_reg0x9_dat_s_wr_wai_int(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x9.dat_s_wr_wai_int = value;
}

/* REG_0x09:reg0x9->dat_s_rd_bus_int:0x9[26],If RX FIFO full(only valid for FULL status) or read finish(once block finish or all blocks finish?), the value is 1 means slave can't receive data,then CPU deals the received data and clear it to receive data again.This bit mean is similar with REG0x15 Bit[14].,0x0,R/W1C*/
static inline uint32_t sdio_ll_get_reg0x9_dat_s_rd_bus_int(sdio_hw_t *hw)
{
    return hw->reg0x9.dat_s_rd_bus_int;
}

static inline void sdio_ll_set_reg0x9_dat_s_rd_bus_int(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x9.dat_s_rd_bus_int = value;
}

/* REG_0x0a */

static inline uint32_t sdio_ll_get_reg0xa_value(sdio_hw_t *hw)
{
    return hw->reg0xa.v;
}

static inline void sdio_ll_set_reg0xa_value(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xa.v = value;
}

/* REG_0x0a:reg0xa->SD_CMD_SEND_NORSP_END_MASK:0xa[0],sd host send no response command end mask; Slave Invalid,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0xa_sd_cmd_send_norsp_end_mask(sdio_hw_t *hw)
{
    return hw->reg0xa.sd_cmd_send_norsp_end_mask;
}

static inline void sdio_ll_set_reg0xa_sd_cmd_send_norsp_end_mask(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xa.sd_cmd_send_norsp_end_mask = value;
}

/* REG_0x0a:reg0xa->SD_CMD_SEND_RSP_END_MASK:0xa[1],sd host receive slave command end mask; Slave Invalid,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0xa_sd_cmd_send_rsp_end_mask(sdio_hw_t *hw)
{
    return hw->reg0xa.sd_cmd_send_rsp_end_mask;
}

static inline void sdio_ll_set_reg0xa_sd_cmd_send_rsp_end_mask(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xa.sd_cmd_send_rsp_end_mask = value;
}

/* REG_0x0a:reg0xa->SD_CMD_SEND_RSP_TIMEOUT_MASK:0xa[2],sd host wait slave command over time mask; Slave Invalid,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0xa_sd_cmd_send_rsp_timeout_mask(sdio_hw_t *hw)
{
    return hw->reg0xa.sd_cmd_send_rsp_timeout_mask;
}

static inline void sdio_ll_set_reg0xa_sd_cmd_send_rsp_timeout_mask(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xa.sd_cmd_send_rsp_timeout_mask = value;
}

/* REG_0x0a:reg0xa->SD_DATA_REC_END_MASK:0xa[3],sd host receive slave data end mask; Slave Invalid,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0xa_sd_data_rec_end_mask(sdio_hw_t *hw)
{
    return hw->reg0xa.sd_data_rec_end_mask;
}

static inline void sdio_ll_set_reg0xa_sd_data_rec_end_mask(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xa.sd_data_rec_end_mask = value;
}

/* REG_0x0a:reg0xa->SD_DATA_WR_END_MASK:0xa[4],sd host write data end mask; Slave Invalid,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0xa_sd_data_wr_end_mask(sdio_hw_t *hw)
{
    return hw->reg0xa.sd_data_wr_end_mask;
}

static inline void sdio_ll_set_reg0xa_sd_data_wr_end_mask(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xa.sd_data_wr_end_mask = value;
}

/* REG_0x0a:reg0xa->SD_DATA_TIME_OUT_MASK:0xa[5],sd host wait slave data over time mask; Slave Invalid,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0xa_sd_data_time_out_mask(sdio_hw_t *hw)
{
    return hw->reg0xa.sd_data_time_out_mask;
}

static inline void sdio_ll_set_reg0xa_sd_data_time_out_mask(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xa.sd_data_time_out_mask = value;
}

/* REG_0x0a:reg0xa->RX_FIFO_NEED_READ_MASK:0xa[6],sd host fifo memory need read mask; Slave Invalid,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0xa_rx_fifo_need_read_mask(sdio_hw_t *hw)
{
    return hw->reg0xa.rx_fifo_need_read_mask;
}

static inline void sdio_ll_set_reg0xa_rx_fifo_need_read_mask(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xa.rx_fifo_need_read_mask = value;
}

/* REG_0x0a:reg0xa->TX_FIFO_NEED_WRITE_MASK:0xa[7],sd host fifo memory need write mask; Slave Invalid,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0xa_tx_fifo_need_write_mask(sdio_hw_t *hw)
{
    return hw->reg0xa.tx_fifo_need_write_mask;
}

static inline void sdio_ll_set_reg0xa_tx_fifo_need_write_mask(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xa.tx_fifo_need_write_mask = value;
}

/* REG_0x0a:reg0xa->RX_OV_MASK:0xa[8],sd host fifo memory overflow mask; Slave Invalid,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0xa_rx_ov_mask(sdio_hw_t *hw)
{
    return hw->reg0xa.rx_ov_mask;
}

static inline void sdio_ll_set_reg0xa_rx_ov_mask(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xa.rx_ov_mask = value;
}

/* REG_0x0a:reg0xa->TX_FIFO_EMPT_MASK:0xa[9],sd host fifo memory empty mask：; a.0:Mask INT,not report to CPU, 1：If empty,INT report to CPU(If FIFO is empty,SDIO module will be clock gate, FIFO not empty, clock restart); b.Must after CMD53; workaround:multi-block has to set 1,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0xa_tx_fifo_empt_mask(sdio_hw_t *hw)
{
    return hw->reg0xa.tx_fifo_empt_mask;
}

static inline void sdio_ll_set_reg0xa_tx_fifo_empt_mask(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xa.tx_fifo_empt_mask = value;
}

/* REG_0x0a:reg0xa->CMD_S_RES_END_INT_MASK:0xa[10],0:Mask INT,not report to CPU, 1：INT report to CPU,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0xa_cmd_s_res_end_int_mask(sdio_hw_t *hw)
{
    return hw->reg0xa.cmd_s_res_end_int_mask;
}

static inline void sdio_ll_set_reg0xa_cmd_s_res_end_int_mask(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xa.cmd_s_res_end_int_mask = value;
}

/* REG_0x0a:reg0xa->DAT_S_WR_WAI_INT_MASK:0xa[11],1.Enable slave write data finish INT report to CPU.,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0xa_dat_s_wr_wai_int_mask(sdio_hw_t *hw)
{
    return hw->reg0xa.dat_s_wr_wai_int_mask;
}

static inline void sdio_ll_set_reg0xa_dat_s_wr_wai_int_mask(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xa.dat_s_wr_wai_int_mask = value;
}

/* REG_0x0a:reg0xa->DAT_S_RD_BUS_INT_MASK:0xa[12],1:Enable busy INT report to CPU.,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0xa_dat_s_rd_bus_int_mask(sdio_hw_t *hw)
{
    return hw->reg0xa.dat_s_rd_bus_int_mask;
}

static inline void sdio_ll_set_reg0xa_dat_s_rd_bus_int_mask(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xa.dat_s_rd_bus_int_mask = value;
}

/* REG_0x0b */

static inline void sdio_ll_set_reg0xb_value(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xb.v = value;
}

/* REG_0x0b:reg0xb->TX_FIFO_DIN:0xb[31:0],sd host fifo write data,0x0,W*/
static inline void sdio_ll_set_reg0xb_tx_fifo_din(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xb.v = value;
}

/* REG_0x0c */

static inline uint32_t sdio_ll_get_reg0xc_value(sdio_hw_t *hw)
{
    return hw->reg0xc.v;
}

/* REG_0x0c:reg0xc->RX_FIFO_DOUT:0xc[31:0],sd host fifo read data,0x0,R*/
static inline uint32_t sdio_ll_get_reg0xc_rx_fifo_dout(sdio_hw_t *hw)
{
    return hw->reg0xc.v;
}

/* REG_0x0d */

static inline uint32_t sdio_ll_get_reg0xd_value(sdio_hw_t *hw)
{
    return hw->reg0xd.v;
}

static inline void sdio_ll_set_reg0xd_value(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xd.v = value;
}

/* REG_0x0d:reg0xd->RX_FIFO_THRESHOLD:0xd[7:0],sd host fifo threshold for read,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0xd_rx_fifo_threshold(sdio_hw_t *hw)
{
    return hw->reg0xd.rx_fifo_threshold;
}

static inline void sdio_ll_set_reg0xd_rx_fifo_threshold(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xd.rx_fifo_threshold = value;
}

/* REG_0x0d:reg0xd->TX_FIFO_THRESHOLD:0xd[15:8],sd host fifo threshold for write,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0xd_tx_fifo_threshold(sdio_hw_t *hw)
{
    return hw->reg0xd.tx_fifo_threshold;
}

static inline void sdio_ll_set_reg0xd_tx_fifo_threshold(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xd.tx_fifo_threshold = value;
}

/* REG_0x0d:reg0xd->RX_FIFO_RST:0xd[16],read fifo reset, high active; Host/Slave:Reset self state,0x1,R/W1C*/
static inline uint32_t sdio_ll_get_reg0xd_rx_fifo_rst(sdio_hw_t *hw)
{
    return hw->reg0xd.rx_fifo_rst;
}

static inline void sdio_ll_set_reg0xd_rx_fifo_rst(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xd.rx_fifo_rst = value;
}

/* REG_0x0d:reg0xd->TX_FIFO_RST:0xd[17],write fifo reset, high active; Host/Slave:Reset self state,0x1,R/W1C*/
static inline uint32_t sdio_ll_get_reg0xd_tx_fifo_rst(sdio_hw_t *hw)
{
    return hw->reg0xd.tx_fifo_rst;
}

static inline void sdio_ll_set_reg0xd_tx_fifo_rst(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xd.tx_fifo_rst = value;
}

/* REG_0x0d:reg0xd->RXFIFO_RD_READY:0xd[18],host/slave rx fifo not empty,0x0,R*/
static inline uint32_t sdio_ll_get_reg0xd_rxfifo_rd_ready(sdio_hw_t *hw)
{
    return hw->reg0xd.rxfifo_rd_ready;
}

/* REG_0x0d:reg0xd->TXFIFO_WR_READY:0xd[19],host/slave tx fifo not full(data is valid, not used for CMD),0x1,R*/
static inline uint32_t sdio_ll_get_reg0xd_txfifo_wr_ready(sdio_hw_t *hw)
{
    return hw->reg0xd.txfifo_wr_ready;
}

/* REG_0x0d:reg0xd->SD_STA_RST:0xd[20],sdcard command and data state reset, high active; Host/Slave:Reset self state; It's better to reset after one round transfer.,0x1,R/W1C*/
static inline uint32_t sdio_ll_get_reg0xd_sd_sta_rst(sdio_hw_t *hw)
{
    return hw->reg0xd.sd_sta_rst;
}

static inline void sdio_ll_set_reg0xd_sd_sta_rst(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xd.sd_sta_rst = value;
}

/* REG_0x0d:reg0xd->SD_CLK_SEL:0xd[22:21],sdcard clock div sel, used or not depend on project; Invalid.,0x3,RW*/
static inline uint32_t sdio_ll_get_reg0xd_sd_clk_sel(sdio_hw_t *hw)
{
    return hw->reg0xd.sd_clk_sel;
}

static inline void sdio_ll_set_reg0xd_sd_clk_sel(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xd.sd_clk_sel = value;
}

/* REG_0x0d:reg0xd->SD_RD_WAIT_SEL:0xd[23],read data after command ?; Use default value.,0x1,RW*/
static inline uint32_t sdio_ll_get_reg0xd_sd_rd_wait_sel(sdio_hw_t *hw)
{
    return hw->reg0xd.sd_rd_wait_sel;
}

static inline void sdio_ll_set_reg0xd_sd_rd_wait_sel(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xd.sd_rd_wait_sel = value;
}

/* REG_0x0d:reg0xd->SD_WR_WAIT_SEL:0xd[24],write data after command ?; Use default value.,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0xd_sd_wr_wait_sel(sdio_hw_t *hw)
{
    return hw->reg0xd.sd_wr_wait_sel;
}

static inline void sdio_ll_set_reg0xd_sd_wr_wait_sel(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xd.sd_wr_wait_sel = value;
}

/* REG_0x0d:reg0xd->CLK_REC_SEL:0xd[25],write clock recovery selection; when write data, should be set to 1(always 1,SW not use it),0x0,RW*/
static inline uint32_t sdio_ll_get_reg0xd_clk_rec_sel(sdio_hw_t *hw)
{
    return hw->reg0xd.clk_rec_sel;
}

static inline void sdio_ll_set_reg0xd_clk_rec_sel(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0xd.clk_rec_sel = value;
}

/* REG_0x10 */

static inline uint32_t sdio_ll_get_reg0x10_value(sdio_hw_t *hw)
{
    return hw->reg0x10.v;
}

static inline void sdio_ll_set_reg0x10_value(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x10.v = value;
}

/* REG_0x10:reg0x10->SD_SLAVE:0x10[0],0:Host 1:Slave,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0x10_sd_slave(sdio_hw_t *hw)
{
    return hw->reg0x10.sd_slave;
}

static inline void sdio_ll_set_reg0x10_sd_slave(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x10.sd_slave = value;
}

/* REG_0x10:reg0x10->DAT_S_RD_MUL_BLK:0x10[1],Host write slave read; Slave:0:Read signle block 1:Read multi-blocks; If one block transfer finish, the sdio slave will stop the bus to transfer data again.Once set this bit to 1(set to 0 and then to 1), slave will release the sdio bus to notify host can transfer next block data.,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0x10_dat_s_rd_mul_blk(sdio_hw_t *hw)
{
    return hw->reg0x10.dat_s_rd_mul_blk;
}

static inline void sdio_ll_set_reg0x10_dat_s_rd_mul_blk(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x10.dat_s_rd_mul_blk = value;
}

/* REG_0x10:reg0x10->IO_CUR_STA_REG:0x10[3:2],io state register reponse to host,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0x10_io_cur_sta_reg(sdio_hw_t *hw)
{
    return hw->reg0x10.io_cur_sta_reg;
}

static inline void sdio_ll_set_reg0x10_io_cur_sta_reg(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x10.io_cur_sta_reg = value;
}

/* REG_0x10:reg0x10->CMD_52_STOP_CLR:0x10[4],cmd52 send stop command(cmd_s_rec_wr_dat_0[0]), write 1 clear cmd_s_rec_wr_dat_0; Host send CMD52 with Address-0,Bit[0] = 1 to notify slave stop,slave check this value to stop;clear this bit to start again,0x0,WO*/
static inline void sdio_ll_set_reg0x10_cmd_52_stop_clr(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x10.cmd_52_stop_clr = value;
}

/* REG_0x11 */

static inline uint32_t sdio_ll_get_reg0x11_value(sdio_hw_t *hw)
{
    return hw->reg0x11.v;
}

static inline void sdio_ll_set_reg0x11_value(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x11.v = value;
}

/* REG_0x11:reg0x11->SD_SLAVE_RDAT_0:0x11[31:0],Function REGs; Slave write data to this REG, and Host read it by CMD52.; cmd_s_res_rd_dat_3,cmd_s_res_rd_dat_2,cmd_s_res_rd_dat_1,cmd_s_res_rd_dat_0; Bit[0] means host notify slave device whether stop transmit data(1 stop).,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0x11_sd_slave_rdat_0(sdio_hw_t *hw)
{
    return hw->reg0x11.v;
}

static inline void sdio_ll_set_reg0x11_sd_slave_rdat_0(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x11.v = value;
}

/* REG_0x12 */

static inline uint32_t sdio_ll_get_reg0x12_value(sdio_hw_t *hw)
{
    return hw->reg0x12.v;
}

static inline void sdio_ll_set_reg0x12_value(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x12.v = value;
}

/* REG_0x12:reg0x12->SD_SLAVE_RDAT_1:0x12[31:0],Function REGs; Slave write data to this REG, and Host read it by CMD52.; cmd_s_res_rd_dat_7,cmd_s_res_rd_dat_6,cmd_s_res_rd_dat_5,cmd_s_res_rd_dat_4,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0x12_sd_slave_rdat_1(sdio_hw_t *hw)
{
    return hw->reg0x12.v;
}

static inline void sdio_ll_set_reg0x12_sd_slave_rdat_1(sdio_hw_t *hw, uint32_t value)
{
    hw->reg0x12.v = value;
}

/* REG_0x13 */

static inline uint32_t sdio_ll_get_reg0x13_value(sdio_hw_t *hw)
{
    return hw->reg0x13.v;
}

/* REG_0x13:reg0x13->SD_SLAVE_WDAT_0:0x13[31:0],Function REGs; Slave read data from this REG, and Host write value with address by CMD52.; cmd_s_rec_wr_dat_3,cmd_s_rec_wr_dat_2,cmd_s_rec_wr_dat_1,cmd_s_rec_wr_dat_0,0x0,R*/
static inline uint32_t sdio_ll_get_reg0x13_sd_slave_wdat_0(sdio_hw_t *hw)
{
    return hw->reg0x13.v;
}

/* REG_0x14 */

static inline uint32_t sdio_ll_get_reg0x14_value(sdio_hw_t *hw)
{
    return hw->reg0x14.v;
}

/* REG_0x14:reg0x14->SD_SLAVE_WDAT_1:0x14[31:0],Function REGs; Slave read data from this REG, and Host write value with address by CMD52.; cmd_s_rec_wr_dat_7,cmd_s_rec_wr_dat_6,cmd_s_rec_wr_dat_5,cmd_s_rec_wr_dat_4,0x0,R*/
static inline uint32_t sdio_ll_get_reg0x14_sd_slave_wdat_1(sdio_hw_t *hw)
{
    return hw->reg0x14.v;
}

/* REG_0x15 */

static inline uint32_t sdio_ll_get_sd_slave_status_value(sdio_hw_t *hw)
{
    return hw->sd_slave_status.v;
}

/* REG_0x15:sd_slave_status->cmd_s_res_dat_rd:0x15[0],cmd_s_res_dat_rd  //0; ignore this bit ,0x0,R*/
static inline uint32_t sdio_ll_get_sd_slave_status_cmd_s_res_dat_rd(sdio_hw_t *hw)
{
    return hw->sd_slave_status.cmd_s_res_dat_rd;
}

/* REG_0x15:sd_slave_status->cmd_s_res_dat_wr:0x15[1],cmd_s_res_dat_wr ,//1 ; ignore this bit ,0x1,R*/
static inline uint32_t sdio_ll_get_sd_slave_status_cmd_s_res_dat_wr(sdio_hw_t *hw)
{
    return hw->sd_slave_status.cmd_s_res_dat_wr;
}

/* REG_0x15:sd_slave_status->cmd_s_rec_bb_cnt:0x15[10:2],cmd_s_rec_bb_cnt ,//10:2 ; The length which host write/read.,0x0,R*/
static inline uint32_t sdio_ll_get_sd_slave_status_cmd_s_rec_bb_cnt(sdio_hw_t *hw)
{
    return hw->sd_slave_status.cmd_s_rec_bb_cnt;
}

/* REG_0x15:sd_slave_status->cmd_s_rec_op_code:0x15[11],cmd_s_rec_op_code,//11 ; Invalid,0x0,R*/
static inline uint32_t sdio_ll_get_sd_slave_status_cmd_s_rec_op_code(sdio_hw_t *hw)
{
    return hw->sd_slave_status.cmd_s_rec_op_code;
}

/* REG_0x15:sd_slave_status->cmd_s_rec_blk_mod:0x15[12],cmd_s_rec_blk_mod,//12,0x0,R*/
static inline uint32_t sdio_ll_get_sd_slave_status_cmd_s_rec_blk_mod(sdio_hw_t *hw)
{
    return hw->sd_slave_status.cmd_s_rec_blk_mod;
}

/* REG_0x15:sd_slave_status->sd_start_wr_en_r3:0x15[13],sd_start_wr_en_r3,//13; slave write data to line need set SD_START_WR_EN, before set SD_START_WR_EN, ensure this bit is 0, if 1, wait till 0,0x0,R*/
static inline uint32_t sdio_ll_get_sd_slave_status_sd_start_wr_en_r3(sdio_hw_t *hw)
{
    return hw->sd_slave_status.sd_start_wr_en_r3;
}

/* REG_0x15:sd_slave_status->dat_s_rd_bus_4rd:0x15[14],dat_s_rd_bus_4rd ,//14; Slave:1:Slave is in busy status,CPU should deal the received data.,0x0,R*/
static inline uint32_t sdio_ll_get_sd_slave_status_dat_s_rd_bus_4rd(sdio_hw_t *hw)
{
    return hw->sd_slave_status.dat_s_rd_bus_4rd;
}

/* REG_0x15:sd_slave_status->cmd_s_res_end_4rd:0x15[15],cmd_s_res_end_4rd,//15,0x0,R*/
static inline uint32_t sdio_ll_get_sd_slave_status_cmd_s_res_end_4rd(sdio_hw_t *hw)
{
    return hw->sd_slave_status.cmd_s_res_end_4rd;
}

/* REG_0x15:sd_slave_status->dat_s_wr_wai_4rd:0x15[16],dat_s_wr_wai_4rd, //16; wait slave write data finish; 1:write finish,0x0,R*/
static inline uint32_t sdio_ll_get_sd_slave_status_dat_s_wr_wai_4rd(sdio_hw_t *hw)
{
    return hw->sd_slave_status.dat_s_wr_wai_4rd;
}

#endif 

#ifdef __cplusplus 
}                  
#endif             
