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
                            
#ifdef __cplusplus          
extern "C" {              
#endif                      
//#include "sdio_cap.h"       

typedef volatile struct {

    /* REG_0x00 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sd_cmd_start                   : 1;  //0x0[0],sd host(Slave) start send command; Host:If SW set it to 1, sdio auto clear this bit and then start send command.; Slave:SW set it to 1, sdio will receive and response command if host send command, then sdio clear this bit; 0:If this bit is 0, sdio will no reponse command to host and reject command from host side.,0x0,RW/AutoC
           volatile uint32_t  sd_cmd_rsp                     : 1;  //0x0[1],Host Only; sd host need slave respond the command or not.; 1: need respond; 0: not need respond,0x0,RW
           volatile uint32_t  sd_cmd_long                    : 1;  //0x0[2],Host Only; sd host need slave respond a long command or short 1: need long response ; 0: need short response,0x0,RW
           volatile uint32_t  sd_cmd_crc_check               : 1;  //0x0[3],sd host(Slave?) need to check the slave respond command crc or not ; 1: need check crc  ; 0: not check crc,0x0,RW
           volatile uint32_t  cmd_index                      : 6;  //0x0[9:4],Host Only; sd host send command index value,0x0,RW
           volatile uint32_t  reserved0                      :22;  //0x0[31:10],Reserved,0,R
        }; 
        uint32_t v; 
    }reg0x0; 

    /* REG_0x01 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  cmd_argument                   :32;  //0x1[31:0],Host Only; sd host send command argument value; Host:CMD index …,0x0,RW
        }; 
        uint32_t v; 
    }reg0x1; 

    /* REG_0x02 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sd_cmd_timer                   :32;  //0x2[31:0],Host Only; sd host wait the slave respond timer; What's the unit?,0xffffffff,RW
        }; 
        uint32_t v; 
    }reg0x2; 

    /* REG_0x03 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sd_data_en                     : 1;  //0x3[0],Host Only; sd host start receive slave data.; 1: start slave wr data to line set this bit; If host want to read more then 512bytes data, whether need to set it at every block?,0x0,RW/AutoC
           volatile uint32_t  sd_data_stop_en                : 1;  //0x3[1],Host Only; sd host stop selfside transmid data; 1: stop; 0: start; Host can send CMD52 with REG0,Bit[0]=1 to slave and then slave stop transmid data,0x0,RW/AutoC
           volatile uint32_t  sd_data_bus                    : 1;  //0x3[2],sd host(Slave) received data bus ; 1: 4 lines data  ; 0: 1 line data; When switch this bit, should check there is no data transmitting on the data line,0x0,RW
           volatile uint32_t  sd_data_mul_blk                : 1;  //0x3[3],Host Only; sd host received data multiple blocks ; 1: multiple blocks  ; 0: single block; If use bytes mode, should set this bit to single block mode.,0x0,RW
           volatile uint32_t  sd_data_blk_size               :12;  //0x3[15:4],Host Only; sd host send data block size(Bytes) to slave, host controller uses this value to controll send how many bytes to slave with every block.; a.Host with SD-CARD mode:write valid, read invalid.; b.Host with SDIO-Device mode:write valid,read invalid(the length has been send to device by CMD53),0x0,RW
           volatile uint32_t  sd_start_wr_en                 : 1;  //0x3[16],sd host(Slave) write data enable：start write data to data wires.,0x0,RW/AutoC
           volatile uint32_t  sd_byte_sel                    : 1;  //0x3[17],sd host(Slave) received data byte arrangement ; 1: sd_rd = {sd_data[7:0],sd_data[15:8],sd_data[23:16],sd_data[31:24]} ; 0: sd_rd = sd_data[31:0],0x0,RW
           volatile uint32_t  reserved0                      :14;  //0x3[31:18],Reserved,0,R
        }; 
        uint32_t v; 
    }reg0x3; 

    /* REG_0x04 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sd_data_timer                  :32;  //0x4[31:0],Host:; sd host read data from slave till timeout,; what's write timeout mean?; Slave:; slave wait host write data to line till timeout,it doesn't check host read data time.,0xffffffff,RW
        }; 
        uint32_t v; 
    }reg0x4; 

    /* REG_0x05 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sd_rsp_agument_0               :32;  //0x5[31:0],Host:sd host received slave command argument value; ; Slave:sd slave received host command argument.; The argument maybe CMD52/CMD53 and other value(Though other CMD isn't error by ASIC).Every CMD Response INT should check this value.,0x0,R
        }; 
        uint32_t v; 
    }reg0x5; 

    /* REG_0x06 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sd_rsp_agument_1               :32;  //0x6[31:0],Host only; sd host received slave command argument value,0x0,R
        }; 
        uint32_t v; 
    }reg0x6; 

    /* REG_0x07 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sd_rsp_agument_2               :32;  //0x7[31:0],Host only; sd host received slave command argument value,0x0,R
        }; 
        uint32_t v; 
    }reg0x7; 

    /* REG_0x08 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sd_rsp_agument_3               :32;  //0x8[31:0],Host only; sd host received slave command argument value,0x0,R
        }; 
        uint32_t v; 
    }reg0x8; 

    /* REG_0x09 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sd_cmd_send_norsp_end_int      : 1;  //0x9[0],Host only; sd host send no response command end int.; Some CMD no needs response,so CMD sends finish then the sdio produced the CMD send finish INT,0x0,R/W1C
           volatile uint32_t  sd_cmd_send_rsp_end_int        : 1;  //0x9[1],Host only; sd host receive slave command end int,0x0,R/W1C
           volatile uint32_t  sd_cmd_send_rsp_timeout_int    : 1;  //0x9[2],sd host wait slave command over time int(What's diffrence with "no response command end int"),0x0,R/W1C
           volatile uint32_t  sd_data_rec_end_int            : 1;  //0x9[3],sd host receive slave data end int,0x0,R/W1C
           volatile uint32_t  sd_data_wr_end_int             : 1;  //0x9[4],sd host write data end int,it means slave has read data finish,notify host can send next block.(Slave has to write REG0x10 Bit[1] to 1(clear to 0 and then set to 1) to notify host send next block,or host can't send next block data),0x0,R/W1C
           volatile uint32_t  sd_data_time_out_int           : 1;  //0x9[5],sd host wait slave data over time int,0x0,R/W1C
           volatile uint32_t  rx_fifo_need_read              : 1;  //0x9[6],sd host fifo memory need read int; Host:If FIFO has more then count of threshhold data,enable INT to CPU.; Slave:valid,0x0,R
           volatile uint32_t  tx_fifo_need_write             : 1;  //0x9[7],sd host fifo memory need write int; Host:If FIFO has less then count of threshhold data,enable INT to CPU.; Slave:valid,0x0,R
           volatile uint32_t  rx_ov                          : 1;  //0x9[8],sd host fifo memory overflow int?; rx fifo full, host slave wr too many full, slave host wr too many data.; What's the overflow condition?,0x0,R/W1C
           volatile uint32_t  tx_fifo_empt                   : 1;  //0x9[9],sd host fifo memory empty int,0x0,R
           volatile uint32_t  sd_rsp_cmd_crc_ok              : 1;  //0x9[10],sd host/slave received peer side slave/host command crc check indication signal 1: crc ok  ,0x0,R
           volatile uint32_t  sd_rsp_cmd_crc_fail            : 1;  //0x9[11],sd host/slave received peer side slave/host command crc check indication signal 1: crc fail ,0x0,R
           volatile uint32_t  sd_data_crc_ok                 : 1;  //0x9[12],sd host/slave received peer side slave/host data crc check indication signal 1: crc ok,0x0,R
           volatile uint32_t  sd_data_crc_fail               : 1;  //0x9[13],sd host/slave received peer side slave/host data crc check indication signal 1: crc fail,0x0,R
           volatile uint32_t  sd_rsp_index                   : 6;  //0x9[19:14],Host only; sd host received slave command index value,0x0,R
           volatile uint32_t  wr_status                      : 3;  //0x9[22:20],sd host write data result(response from slave device); 0:; 1:; 2:Is right, else is error.; (Workaround:BK7256 read this value is 0x5 though slave responsed is 0x2); 3:; 4:; 5:; 6:; 7:,0x0,R
           volatile uint32_t  data_busy                      : 1;  //0x9[23],sd slave busy signal after sd host send special command and write data.Host check this bit to confirm slave is whether busy?,0x0,R
           volatile uint32_t  cmd_s_res_end_int              : 1;  //0x9[24],Slave only; Slave has finish reponsed the CMD to host side.,0x0,R/W1C
           volatile uint32_t  dat_s_wr_wai_int               : 1;  //0x9[25],Slave:Host write and slave is reading,host should stop, INT notify CPU slave has received the data from host.; Slave write finish.; W1:Clear the finish status.; 1.If clear means slave can receive data again?; Read data-out first or clear INT first?; 2.The name of WR_WAT is hard to understand,0x0,R/W1C
           volatile uint32_t  dat_s_rd_bus_int               : 1;  //0x9[26],If RX FIFO full(only valid for FULL status) or read finish(once block finish or all blocks finish?), the value is 1 means slave can't receive data,then CPU deals the received data and clear it to receive data again.This bit mean is similar with REG0x15 Bit[14].,0x0,R/W1C
           volatile uint32_t  reserved0                      : 5;  //0x9[31:27],Reserved,0,R
        }; 
        uint32_t v; 
    }reg0x9; 

    /* REG_0x0a */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sd_cmd_send_norsp_end_mask     : 1;  //0xa[0],sd host send no response command end mask; Slave Invalid,0x0,RW
           volatile uint32_t  sd_cmd_send_rsp_end_mask       : 1;  //0xa[1],sd host receive slave command end mask; Slave Invalid,0x0,RW
           volatile uint32_t  sd_cmd_send_rsp_timeout_mask   : 1;  //0xa[2],sd host wait slave command over time mask; Slave Invalid,0x0,RW
           volatile uint32_t  sd_data_rec_end_mask           : 1;  //0xa[3],sd host receive slave data end mask; Slave Invalid,0x0,RW
           volatile uint32_t  sd_data_wr_end_mask            : 1;  //0xa[4],sd host write data end mask; Slave Invalid,0x0,RW
           volatile uint32_t  sd_data_time_out_mask          : 1;  //0xa[5],sd host wait slave data over time mask; Slave Invalid,0x0,RW
           volatile uint32_t  rx_fifo_need_read_mask         : 1;  //0xa[6],sd host fifo memory need read mask; Slave Invalid,0x0,RW
           volatile uint32_t  tx_fifo_need_write_mask        : 1;  //0xa[7],sd host fifo memory need write mask; Slave Invalid,0x0,RW
           volatile uint32_t  rx_ov_mask                     : 1;  //0xa[8],sd host fifo memory overflow mask; Slave Invalid,0x0,RW
           volatile uint32_t  tx_fifo_empt_mask              : 1;  //0xa[9],sd host fifo memory empty mask：; a.0:Mask INT,not report to CPU, 1：If empty,INT report to CPU(If FIFO is empty,SDIO module will be clock gate, FIFO not empty, clock restart); b.Must after CMD53; workaround:multi-block has to set 1,0x0,RW
           volatile uint32_t  cmd_s_res_end_int_mask         : 1;  //0xa[10],0:Mask INT,not report to CPU, 1：INT report to CPU,0x0,RW
           volatile uint32_t  dat_s_wr_wai_int_mask          : 1;  //0xa[11],1.Enable slave write data finish INT report to CPU.,0x0,RW
           volatile uint32_t  dat_s_rd_bus_int_mask          : 1;  //0xa[12],1:Enable busy INT report to CPU.,0x0,RW
           volatile uint32_t  reserved0                      :19;  //0xa[31:13],Reserved,0,R
        }; 
        uint32_t v; 
    }reg0xa; 

    /* REG_0x0b */
    union 
    { 
        struct 
        { 
           volatile uint32_t  tx_fifo_din                    :32;  //0xb[31:0],sd host fifo write data,0x0,W
        }; 
        uint32_t v; 
    }reg0xb; 

    /* REG_0x0c */
    union 
    { 
        struct 
        { 
           volatile uint32_t  rx_fifo_dout                   :32;  //0xc[31:0],sd host fifo read data,0x0,R
        }; 
        uint32_t v; 
    }reg0xc; 

    /* REG_0x0d */
    union 
    { 
        struct 
        { 
           volatile uint32_t  rx_fifo_threshold              : 8;  //0xd[7:0],sd host fifo threshold for read,0x0,RW
           volatile uint32_t  tx_fifo_threshold              : 8;  //0xd[15:8],sd host fifo threshold for write,0x0,RW
           volatile uint32_t  rx_fifo_rst                    : 1;  //0xd[16],read fifo reset, high active; Host/Slave:Reset self state,0x1,R/W1C
           volatile uint32_t  tx_fifo_rst                    : 1;  //0xd[17],write fifo reset, high active; Host/Slave:Reset self state,0x1,R/W1C
           volatile uint32_t  rxfifo_rd_ready                : 1;  //0xd[18],host/slave rx fifo not empty,0x0,R
           volatile uint32_t  txfifo_wr_ready                : 1;  //0xd[19],host/slave tx fifo not full(data is valid, not used for CMD),0x1,R
           volatile uint32_t  sd_sta_rst                     : 1;  //0xd[20],sdcard command and data state reset, high active; Host/Slave:Reset self state; It's better to reset after one round transfer.,0x1,R/W1C
           volatile uint32_t  sd_clk_sel                     : 2;  //0xd[22:21],sdcard clock div sel, used or not depend on project; Invalid.,0x3,RW
           volatile uint32_t  sd_rd_wait_sel                 : 1;  //0xd[23],read data after command ?; Use default value.,0x1,RW
           volatile uint32_t  sd_wr_wait_sel                 : 1;  //0xd[24],write data after command ?; Use default value.,0x0,RW
           volatile uint32_t  clk_rec_sel                    : 1;  //0xd[25],write clock recovery selection; when write data, should be set to 1(always 1,SW not use it),0x0,RW
           volatile uint32_t  reserved0                      : 6;  //0xd[31:26],Reserved,0,R
        }; 
        uint32_t v; 
    }reg0xd; 

    /* REG_0x0e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xe_reserved; 

    /* REG_0x0f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xf_reserved; 

    /* REG_0x10 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sd_slave                       : 1;  //0x10[0],0:Host 1:Slave,0x0,RW
           volatile uint32_t  dat_s_rd_mul_blk               : 1;  //0x10[1],Host write slave read; Slave:0:Read signle block 1:Read multi-blocks; If one block transfer finish, the sdio slave will stop the bus to transfer data again.Once set this bit to 1(set to 0 and then to 1), slave will release the sdio bus to notify host can transfer next block data.,0x0,RW
           volatile uint32_t  io_cur_sta_reg                 : 2;  //0x10[3:2],io state register reponse to host,0x0,RW
           volatile uint32_t  cmd_52_stop_clr                : 1;  //0x10[4],cmd52 send stop command(cmd_s_rec_wr_dat_0[0]), write 1 clear cmd_s_rec_wr_dat_0; Host send CMD52 with Address-0,Bit[0] = 1 to notify slave stop,slave check this value to stop;clear this bit to start again,0x0,WO
           volatile uint32_t  reserved0                      :27;  //0x10[31:5],Reserved,0,R
        }; 
        uint32_t v; 
    }reg0x10; 

    /* REG_0x11 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sd_slave_rdat_0                :32;  //0x11[31:0],Function REGs; Slave write data to this REG, and Host read it by CMD52.; cmd_s_res_rd_dat_3,cmd_s_res_rd_dat_2,cmd_s_res_rd_dat_1,cmd_s_res_rd_dat_0; Bit[0] means host notify slave device whether stop transmit data(1 stop).,0x0,RW
        }; 
        uint32_t v; 
    }reg0x11; 

    /* REG_0x12 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sd_slave_rdat_1                :32;  //0x12[31:0],Function REGs; Slave write data to this REG, and Host read it by CMD52.; cmd_s_res_rd_dat_7,cmd_s_res_rd_dat_6,cmd_s_res_rd_dat_5,cmd_s_res_rd_dat_4,0x0,RW
        }; 
        uint32_t v; 
    }reg0x12; 

    /* REG_0x13 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sd_slave_wdat_0                :32;  //0x13[31:0],Function REGs; Slave read data from this REG, and Host write value with address by CMD52.; cmd_s_rec_wr_dat_3,cmd_s_rec_wr_dat_2,cmd_s_rec_wr_dat_1,cmd_s_rec_wr_dat_0,0x0,R
        }; 
        uint32_t v; 
    }reg0x13; 

    /* REG_0x14 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sd_slave_wdat_1                :32;  //0x14[31:0],Function REGs; Slave read data from this REG, and Host write value with address by CMD52.; cmd_s_rec_wr_dat_7,cmd_s_rec_wr_dat_6,cmd_s_rec_wr_dat_5,cmd_s_rec_wr_dat_4,0x0,R
        }; 
        uint32_t v; 
    }reg0x14; 

    /* REG_0x15 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  cmd_s_res_dat_rd               : 1;  //0x15[0],cmd_s_res_dat_rd  //0; ignore this bit ,0x0,R
           volatile uint32_t  cmd_s_res_dat_wr               : 1;  //0x15[1],cmd_s_res_dat_wr ,//1 ; ignore this bit ,0x1,R
           volatile uint32_t  cmd_s_rec_bb_cnt               : 9;  //0x15[10:2],cmd_s_rec_bb_cnt ,//10:2 ; The length which host write/read.,0x0,R
           volatile uint32_t  cmd_s_rec_op_code              : 1;  //0x15[11],cmd_s_rec_op_code,//11 ; Invalid,0x0,R
           volatile uint32_t  cmd_s_rec_blk_mod              : 1;  //0x15[12],cmd_s_rec_blk_mod,//12,0x0,R
           volatile uint32_t  sd_start_wr_en_r3              : 1;  //0x15[13],sd_start_wr_en_r3,//13; slave write data to line need set SD_START_WR_EN, before set SD_START_WR_EN, ensure this bit is 0, if 1, wait till 0,0x0,R
           volatile uint32_t  dat_s_rd_bus_4rd               : 1;  //0x15[14],dat_s_rd_bus_4rd ,//14; Slave:1:Slave is in busy status,CPU should deal the received data.,0x0,R
           volatile uint32_t  cmd_s_res_end_4rd              : 1;  //0x15[15],cmd_s_res_end_4rd,//15,0x0,R
           volatile uint32_t  dat_s_wr_wai_4rd               : 1;  //0x15[16],dat_s_wr_wai_4rd, //16; wait slave write data finish; 1:write finish,0x0,R
           volatile uint32_t  reserved0                      :15;  //0x15[31:17],Reserved,0,R
        }; 
        uint32_t v; 
    }sd_slave_status; 

} sdio_hw_t;

#ifdef __cplusplus 
}                  
#endif             
