#!/usr/bin/env python3
import os
import json
import logging
import shutil
from .crc import *
from .common import *
from .genbl1 import *

SZ_16M = 0x1000000
FLASH_SECTOR_SZ = 0x1000
CRC_UNIT_DATA_SZ = 32
CRC_UNIT_TOTAL_SZ = 34
PARTITION_HDR_MAGIC = 0xabcd1234

HDR_MAGIC = '55667788aabbccdd'
GLOBAL_HDR_LEN = 32
IMG_HDR_LEN = 32

s_license = "\
// Copyright 2022-2023 Beken\r\n\
//\r\n\
// Licensed under the Apache License, Version 2.0 (the \"License\");\r\n\
// you may not use this file except in compliance with the License.\r\n\
// You may obtain a copy of the License at\r\n\
//\r\n\
//     http://www.apache.org/licenses/LICENSE-2.0\r\n\
//\r\n\
// Unless required by applicable law or agreed to in writing, software\r\n\
// distributed under the License is distributed on an \"AS IS\" BASIS,\r\n\
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\r\n\
// See the License for the specific language governing permissions and\r\n\
// limitations under the License.\r\n\
\r\n\
//This is a generated file, don't modify it!\r\n\
\r\n\
#pragma once\r\n\
\r\n\
\r\n\
"

s_phy2code_start = "\
#define FLASH_CEIL_ALIGN(v, align) ((((v) + ((align) - 1)) / (align)) * (align))\r\n\
#define FLASH_PHY2VIRTUAL_CODE_START(phy_addr) FLASH_CEIL_ALIGN(FLASH_PHY2VIRTUAL(FLASH_CEIL_ALIGN((phy_addr), 34)), CPU_VECTOR_ALIGN_SZ)\r\n\
"

def int2hexstr2(v):
    return (f'%04x' %(v))

def int2hexstr4(v):
    return (f'%08x' %(v))

class Hdr:
    def __init__(self, magic, crc, version, hdr_len, img_num, flags = 0):
        self.magic = bytes.fromhex(magic)
        self.crc = bytes.fromhex(int2hexstr4(crc))
        self.version = bytes.fromhex(int2hexstr4(version))
        self.hdr_len = bytes.fromhex(int2hexstr2(hdr_len))
        self.img_num = bytes.fromhex(int2hexstr2(img_num))
        self.flags = bytes.fromhex(int2hexstr4(flags))
        self.reserved = bytes(0)

        logging.debug(f'global hdr: magic={self.magic}, version={self.version}, hdr_len={self.hdr_len}, img_num={self.img_num}, flags={self.flags}')

class Imghdr:
    def __init__(self, img_len, img_offset, flash_offset, crc, version, flags = 0):
        self.img_len = bytes.fromhex(int2hexstr4(img_len))
        self.img_offset = bytes.fromhex(int2hexstr4(img_offset))
        self.flash_offset = bytes.fromhex(int2hexstr4(flash_offset))
        self.crc = bytes.fromhex(int2hexstr4(crc))
        self.version = bytes.fromhex(int2hexstr4(version))
        self.flags = bytes.fromhex(int2hexstr4(flags))
        self.reserved = bytes(0)
        logging.debug(f'img hdr: img_len=%x, img_offset=%x, flash_offset=%x, crc=%x, version=%x flags=%x'
            %(img_len, img_offset, flash_offset, crc, version, flags))
 
class Partition:
    def is_out_of_range(self, addr):
        if (addr >= SZ_16M):
            return True 
        else:
            return False

    def bin_is_code(self):
        if (self.bin_type == "code"):
            return True
        else:
            return False

    def is_flash_sector_aligned(self, addr):
        if ((addr % FLASH_SECTOR_SZ) == 0):
            return True
        else:
            return False

    def check_fields(self):
        #TODO make size optional
        required_fields = ["partition", "size", "bin"]
        for field in required_fields:
            if field not in self.partition_json.keys():
                logging.error(f'partition{self.idx} missing "{field}"')
                exit(1)

        self.partition_name = self.partition_json['partition']
        all_possible_fields = ["partition", "offset", "size", "bin",
            "bin_type", "bin_hdr_size", "bin_tail_size", "bin_pad_size", "bin_encrypted_addr", "version", "load_addr", "location", "boot_partition"]
        for key in self.partition_json.keys():
            if key not in all_possible_fields:
                logging.error(f'partition{self.idx} unknown "{key}"')
                exit(1)

    def parse_and_check_bin_location(self):
        self.bin_location = f''
        if "location" in self.partition_json.keys():
            self.bin_location = self.partition_json['location']
        logging.debug(f'partition{self.idx} location={self.bin_location}')

    def parse_and_check_bin(self):
        self.bin_name = self.partition_json['bin']
        if (self.gencode_only == True):
            self.bin_size = 0
            return

        if (self.bin_name == ''):
            self.bin_size = 0
            if self.bin_is_code():
                logging.error(f'partition{self.idx}: code partition should specify a bin')
                exit(1)
        elif not os.path.exists(self.bin_name):
            if (self.bin_name == "app_ns.bin") and (self.bin_location == 'auto'):
                spe_build_dir = os.getcwd()
                nspe_build_dir = spe_build_dir.replace('spe', 'nspe')
                src_bin_file = os.path.join(nspe_build_dir, 'app.bin')
                dst_bin_file = os.path.join(spe_build_dir, 'app_ns.bin')
                if os.path.exists(src_bin_file):
                    shutil.copy(src_bin_file, dst_bin_file)
                    logging.debug(f'partition{self.idx} copy {src_bin_file}')
                    logging.debug(f'to {dst_bin_file}')
                else:
                    logging.error(f'partition{self.idx} can not automatically detect {self.bin_name}!!!')
                    exit(1)
            else:
                logging.error(f'partition{self.idx} bin {self.bin_name} not exists')
                exit(1)
        self.bin_size = os.path.getsize(self.bin_name)
        logging.debug(f'bin name={self.bin_name}, size=%x' %(self.bin_size))

    def parse_and_check_bin_type(self):
        self.bin_type = "data"
        if "bin_type" in self.partition_json.keys():
            self.bin_type = self.partition_json['bin_type']

        if (self.bin_type != "code") and (self.bin_type != "data"):
            logging.error(f'partition{idx} {self.partition} type should be one of "code", "data"')
            exit(1)
        logging.debug(f'bin type={self.bin_type}')

    def parse_and_check_bin_hdr_tail_size(self):
        self.bin_hdr_size = 0;
        self.bin_tail_size = 0;

        if "bin_hdr_size" in self.partition_json.keys():
            self.bin_hdr_size = hex2int(self.partition_json['bin_hdr_size'])

        if "bin_tail_size" in self.partition_json.keys():
            self.bin_tail_size = hex2int(self.partition_json['bin_tail_size'])

        if (self.is_flash_sector_aligned(self.bin_hdr_size) == False):
            logging.error(f'partition{self.idx} hdr offset=%x not 4K aligned' %(self.bin_hdr_size))

        if (self.is_flash_sector_aligned(self.bin_tail_size) == False):
            logging.error(f'partition{self.idx} tail offset=%x not 4K aligned' %(self.bin_tail_size))
             
        logging.debug(f'bin bin_hdr_size=%x, bin_tail_size=%x' %(self.bin_hdr_size, self.bin_tail_size))

    def parse_and_check_partition_offset(self):
        if "offset" in self.partition_json.keys():
            self.partition_offset = hex2int(self.partition_json['offset'])
        else:
            self.partition_offset = self.deduced_min_partition_offset

        if (self.idx == 0) and (self.partition_offset > 0):
            logging.debug(f'partition{self.idx} first partition offset is 0')

        if (self.partition_offset < self.deduced_min_partition_offset):
            logging.error(f'partition{self.idx} partition offset=%x overlapped with previous partition' %(self.partition_offset))
            exit(1)

        if (self.is_out_of_range(self.partition_offset) == True):
            logging.error(f'partition{self.idx} partition offset=%x is out of range' %(self.partition_offset))
            exit(1)

        if (self.bin_is_code()):
            if (self.is_flash_sector_aligned(self.partition_offset) == False):
                logging.error(f'partition{self.idx} partition offset=%x not FLASH sector aligned' %(self.phy_partition_offset))
                exit(1)

        logging.debug(f'partition{self.idx} partition_offset=%x' %(self.partition_offset))

    def parse_and_check_partition_size(self):
        self.partition_size = size2int(self.partition_json['size'])
        if (self.is_out_of_range(self.partition_size) == True):
            logging.error(f'partition{self.idx} partition size=%x out of range' %(self.partition_size))
            exit(1)

        # TODO may need to make it sector aligned!!!


        logging.debug(f'partition{self.idx} partition size=%x' %(self.partition_size))
 
    def parse_and_check_bin_pad_size(self):
        self.bin_pad_size = 0
        if "bin_pad_size" in self.partition_json.keys():
            self.bin_pad_size = size2int(self.partition_json['bin_pad_size'])

        if (self.is_out_of_range(self.bin_pad_size) == True):
            logging.error(f'partition{self.idx} bin pad size=%x out of range' %(self.bin_pad_size))
            exit(1)

        if (self.is_flash_sector_aligned(self.bin_pad_size) == False):
            logging.error(f'partition{self.idx} bin pad size=%x not aligned' %(self.bin_pad_size))
            exit(1)

        logging.debug(f'partition{self.idx} bin pad size=%x' %(self.bin_pad_size))
 
    def parse_and_check_bin_encrypted_addr(self):
        self.bin_encrypted_addr = 0
        if "bin_encrypted_addr" in self.partition_json.keys():
            if (self.flash_base_addr == 0):
                logging.error(f'partition{self.idx} requires AES encrypt, should specify flash_base address')
                exit(1)
            else:
                self.bin_encrypted_addr = hex2int(self.partition_json['bin_encrypted_addr'])

        logging.debug(f'partition{self.idx} encrypted addr=%x' %(self.bin_encrypted_addr))
 
    def parse_and_check_bin_msp_pc(self):
        self.msp = 0
        self.pc = 0
        if self.bin_is_code() == False:
            return
        if self.bin_size == 0:
            return

        with open(self.bin_name, 'rb') as f:
            if (self.bin_hdr_size > 0):
                f.seek(self.bin_hdr_size)

            msp_bytes = f.read(4)
            self.msp = msp_bytes.hex()
            pc_bytes = f.read(4)
            self.pc = pc_bytes.hex()
            logging.debug(f'partition{self.idx} msp=%s, pc=%s' %(self.msp, self.pc))
 
    def parse_and_check_bin_version(self):
        self.version = '0x00000000'

        if "version" in self.partition_json.keys():
            self.version = self.partition_json['version']
 
    def parse_and_check_static_and_load_addr(self):
        if (self.bin_is_code() == False):
            return

        virtual_1st_instruction_addr = phy2virtual(self.bin_1st_instruction_offset)
        static_addr_int = self.flash_base_addr + virtual_1st_instruction_addr
        static_addr_hex = f'0x%08x' %(static_addr_int)
        self.static_addr = static_addr_hex
 
        if "load_addr" in self.partition_json.keys():
            self.load_addr = self.partition_json['load_addr']
        else:
            self.load_addr = self.static_addr
        logging.debug(f'load address={self.load_addr}')

    def parse_and_check_boot_partition(self):

        self.boot_partition = ''
        if "boot_partition" in self.partition_json.keys():
            self.boot_partition = self.partition_json['boot_partition']
        else:
            if (self.partition_name == 'primary_manifest') or (self.partition_name == 'secondary_manifest'):
                logging.error(f'manifest partition missing "boot_partition"')
                exit(1)
        logging.debug(f'boot partition={self.boot_partition}') 

    '''
    +------------------------+  <--- partition_offset/bin_hdr_offset
    |  HDR (plain data)      |
    |  (bin_hdr_size)        |  
    |                        |
   +------------------------+  <--- bin_code_partition_offset
    |  CRC aligned pad       |
    |  (less than 33B)       |
    +------------------------+  <--- bin_1st_instruction_offset
    |  Code (CRC/Encrypted)  |
    |                        |
    |                        |
    +------------------------+
    |  Unused                |
    |                        |
    +------------------------+  <--- bin_tail_offset
    |  TAIL (plain data)     |
    |  (bin_tail_size)       |
    |                        |
    +------------------------+  <--- partition end
    '''

    def check_constraints(self):
        if (self.is_out_of_range(self.partition_offset + self.partition_size - 1) == True):
            logging.error(f'partition{self.idx} start=%x size=%x is out of range' %(self.partition_offset, self.partition_size))
            exit(1)

        if (self.bin_size > 0) and (self.bin_size <= (self.bin_hdr_size + self.bin_tail_size)):
            logging.error(f'partition{self.idx} bin size=%x <= (hdr_size=%x + tail_size=%x)' %(self.bin_size, self.bin_hdr_size, self.bin_tail_size))

        if (self.bin_is_code()):
            self.bin_hdr_offset = self.partition_offset
            self.bin_tail_offset = self.partition_offset + self.partition_size - self.bin_tail_size
            self.bin_code_partition_offset = self.bin_hdr_offset + self.bin_hdr_size

            min_phy_1st_instruction_offset = ceil_align(self.bin_code_partition_offset, CRC_UNIT_TOTAL_SZ)
            min_virtual_1st_instrution_offset = phy2virtual(min_phy_1st_instruction_offset)
            virtual_1st_vector_instruction_offset = ceil_align(min_virtual_1st_instrution_offset, self.cpu_vector_align_bytes)
            self.bin_1st_instruction_offset = virtual2phy(virtual_1st_vector_instruction_offset)
            self.bin_code_aligned_pad_size = self.bin_1st_instruction_offset - self.bin_code_partition_offset
            logging.debug(f'partition{self.idx} partition offset=%x, hdr_offset=%x, tail_offset=%x, code_partition_offset=%x, 1st_instruction_offset=%x'
                %(self.partition_offset, self.bin_hdr_offset, self.bin_tail_offset, self.bin_code_partition_offset, self.bin_1st_instruction_offset))
            logging.debug(f'partition{self.idx} min_phy_1st_instruction=%x, min_virtual_1st_instrution_offset=%x' %(min_phy_1st_instruction_offset,
                min_virtual_1st_instrution_offset))
            logging.debug(f'partition{self.idx} virtual_1st_vector_instruction_offset=%x, bin_1st_instruction_offset=%x' %(virtual_1st_vector_instruction_offset,
                self.bin_1st_instruction_offset))

            
            virtual_partition_size = phy2virtual(self.partition_size - self.bin_hdr_size - self.bin_tail_size)
            self.virtual_partition_size = floor_align(virtual_partition_size, CRC_UNIT_DATA_SZ)
            virtual_code_size = phy2virtual(self.partition_size - self.bin_code_aligned_pad_size - self.bin_hdr_size - self.bin_tail_size - 34)
            self.virtual_code_size = floor_align(virtual_code_size, CRC_UNIT_DATA_SZ)
            logging.debug(f'flash max virtual partition size=%x, virtual max code size=%x' %(self.virtual_partition_size, self.virtual_code_size))
    
            self.bin_code_size = self.bin_size - self.bin_hdr_size - self.bin_tail_size
            self.bin_code_with_crc_size = crc_size(self.bin_code_size) 
            self.bin_total_size = self.bin_hdr_size + self.bin_tail_size + self.bin_code_aligned_pad_size + self.bin_code_with_crc_size
            logging.debug(f'partition{self.idx} bin raw_size=%x, tail_size=%x, hdr_size=%x' %(self.bin_size, self.bin_hdr_size, self.bin_tail_size))
            logging.debug(f'partition{self.idx} bin crc_aligned_pad_size=%x, code_without_crc_size=%x, code_with_crc_size=%x, all_size=%x'
                %(self.bin_code_aligned_pad_size, self.bin_code_size, self.bin_code_with_crc_size, self.bin_total_size))
        else:
            self.bin_data_size = self.bin_size
            self.bin_total_size = self.bin_size

        if (self.bin_size > self.partition_size):
            logging.error(f'partition{self.idx} bin size %x > partition size %x' %(self.bin_size, self.partition_size))
            exit(1)

        if (self.bin_total_size > self.partition_size):
            logging.error(f'partition{self.idx} total bin size %x > partition size %x' %(self.bin_total_size, self.partition_size))
            exit(1)

    def encrypt_bin(self):
        logging.info("TODO encrypt bin")

    def process_data_bin(self):
        if (self.bin_size == 0):
            return

        with open(self.bin_name, 'rb') as f:
            self.partition_buf = f.read()

    def init_buf(self, buf, size, fill):
        for i in range(size):
            buf[i] = fill

    def process_code_bin(self):
        self.partition_buf = bytearray()
        with open(self.bin_name, 'rb') as f:
            if (self.bin_hdr_size > 0):
                self.hdr_buf = f.read(self.bin_hdr_size)
                self.partition_buf = self.hdr_buf

            f.seek(self.bin_hdr_size)
            self.temp_code_bin_name = f'{self.file_name_prefix}_code.bin'
            self.temp_code_bin_name_crc = f'{self.file_name_prefix}_code_crc.bin'
            self.code_buf = f.read(self.bin_size - self.bin_hdr_size - self.bin_tail_size)
            with open(self.temp_code_bin_name, 'wb+') as cf:
                cf.write(self.code_buf)

            if (self.bin_code_aligned_pad_size > 0):
                pad_buf = bytearray(self.bin_code_aligned_pad_size)
                self.partition_buf += pad_buf

            crc(self.temp_code_bin_name, self.temp_code_bin_name_crc)
            #TODO encrypted here!

            with open(self.temp_code_bin_name_crc, 'rb') as cf_crc:
                self.code_buf = cf_crc.read()
                self.partition_buf += self.code_buf

            crc_bin_size = os.path.getsize(self.temp_code_bin_name_crc)
            pad_0xff_buf_size = self.partition_size - crc_bin_size - self.bin_code_aligned_pad_size - self.bin_hdr_size - self.bin_tail_size
            pad_0xff_buf = bytearray(pad_0xff_buf_size)
            self.init_buf(pad_0xff_buf, pad_0xff_buf_size, 0xff)
            self.partition_buf += pad_0xff_buf
            logging.debug(f'Filled 0xFF size= {pad_0xff_buf_size} crc_bin_size={crc_bin_size}')
            logging.debug(f'hdr_size={self.bin_hdr_size} tail_size={self.bin_tail_size} align_pad={self.bin_code_aligned_pad_size}')

            if (self.bin_tail_size > 0):
                tail_offset = self.bin_size - self.bin_tail_size
                f.seek(tail_offset)
                self.tail_buf = f.read(self.bin_tail_size)

    def process_bin(self):
        if (self.bin_is_code()):
            self.process_code_bin()
        else:
            self.process_data_bin()

        if (self.bin_size > 0):
            self.temp_partition_bin_name = f'{self.file_name_prefix}_partition.bin'
            with open(self.temp_partition_bin_name, 'wb+') as pf:
                pf.write(self.partition_buf)
                if (self.bin_tail_size > 0):
                    pf.seek(self.bin_tail_offset - self.partition_offset)
                    pf.write(self.tail_buf)

    def __init__(self, partitions, idx, partition_dic, deduced_min_partition_offset):
        self.idx = idx
        self.partition_json = partition_dic
        self.deduced_min_partition_offset = deduced_min_partition_offset
        self.flash_base_addr = partitions.flash_base_addr
        self.json_file_base_name = partitions.json_file_base_name
        self.gencode_only = partitions.gencode_only
        self.cpu_vector_align_bytes = partitions.cpu_vector_align_bytes
        self.crc = 0 # TODO

        self.file_name_prefix = f'{self.json_file_base_name}_{idx}'
        logging.debug(f'')

        self.check_fields()
        logging.debug(f'processing {self.json_file_base_name}.partition[{idx}].{self.partition_name}')
        self.parse_and_check_bin_type() # partition checking used bin type, so parse bin type firstly
        self.parse_and_check_partition_offset() 
        self.parse_and_check_partition_size()
        self.parse_and_check_bin_location()
        self.parse_and_check_bin()
        self.parse_and_check_bin_hdr_tail_size()
        self.parse_and_check_bin_pad_size()
        self.parse_and_check_bin_encrypted_addr()
        self.parse_and_check_bin_msp_pc()
        self.parse_and_check_bin_version()
        self.check_constraints()
        self.parse_and_check_static_and_load_addr() # call it after check_constraints since it depends on 1st_instruction_offset
        self.parse_and_check_boot_partition()

class Partitions:

    def load_json(self):
        if not os.path.exists(self.json_file_name):
            logging.error(f'JSON configuration file {json_file} not exists')
            exit(1)

        self.json_file_base_name = os.path.basename(self.json_file_name)
        logging.debug(f'processing json {self.json_file_base_name}, crc={self.need_crc}, encrypt={self.need_encrypted}')
        with open(self.json_file_name, 'r') as self.json_file:
            try:
                self.json_data = json.load(self.json_file)
            except Exception as e:
                logging.error(f"Could not open : '{self.json_file_name}': {e}", err=True)
                exit(1)

    def is_pack_default(self, pack_list):
        if (len(pack_list) == 1) and (pack_list[0] == 'default'):
            return True
        return False

    def partition_name_to_idx(self, partition_name):
        for idx in range(self.partitions_cnt):
            if (partition_name == self.partitions[idx].partition_name):
                return idx
        return -1

    def build_pack_list(self, tag, pack_list, pattern, match):
        pack_idx_list = []
        if (len(pack_list) == 0):
            return pack_idx_list

        if (self.is_pack_default(pack_list) == False):
            pre_idx = -1
            for partition_name in pack_list:
                idx = self.partition_name_to_idx(partition_name)
                if (idx == -1):
                    logging.error(f'{tag} unknown partition={partition_name}')
                    exit(1)
                if (idx <= pre_idx):
                    logging.error(f'{tag} partition={partition_name} not in order')
                    exit(1)
                pre_idx = idx
                pack_idx_list.append(idx)
            logging.debug(f'{tag} pack partition list={pack_idx_list}') 
            return pack_idx_list

        for idx in range(self.partitions_cnt):
            partition_name = self.partitions[idx].partition_name
            found = False
            if (partition_name.find(f'{pattern}') == 0):
                found = True
            if (match == found):
                pack_idx_list.append(idx)
        logging.debug(f'{tag} partitions list={pack_idx_list}')
        return pack_idx_list

    def parse_json(self):
        if ("partitions" not in self.json_data):
            logging.error('json does not contain field "partitions"!')
            exit(1)

        self.flash_base_addr = 0x02000000
        if ("flash_base_addr" in self.json_data):
            self.flash_base_addr = hex2int(self.json_data['flash_base_addr'])
        logging.debug(f'flash base=%x' %(self.flash_base_addr))

        self.cpu_vector_align_bytes = 128
        if ("cpu_vector_align" in self.json_data):
            self.cpu_vector_align_bytes = hex2int(self.json_data['cpu_vector_align'])
        logging.debug(f'cpu vector align bytes=%x' %(self.cpu_vector_align_bytes))

        if ("pack_ota" not in self.json_data):
            self.pack_ota = []
        else:
            self.pack_ota = self.json_data['pack_ota']
        logging.debug(f'OTA binary list: {self.pack_ota}')

        if ("pack_app" not in self.json_data):
            self.pack_app = []
        else:
            self.pack_app = self.json_data['pack_app']
        logging.debug(f'APP binary list: {self.pack_app}')

        self.partitions_cnt = len(self.json_data['partitions'])
        if (self.partitions_cnt == 0):
            logging.error(f'partitions of json does not contain any item!')
            exit(1)

        deduced_next_partition_offset = 0
        self.first_partition_offset = 0
        for idx in range(self.partitions_cnt):
            partition = Partition(self, idx, self.json_data['partitions'][idx], deduced_next_partition_offset)
            deduced_next_partition_offset = partition.partition_offset + partition.partition_size
            self.partitions.append(partition)
            if (idx == 0):
                self.first_partition_offset = partition.partition_offset

        self.ota_pack_idx_list = self.build_pack_list("OTA", self.pack_ota, "secondary_", True)
        self.app_pack_idx_list = self.build_pack_list("APP", self.pack_app, "secondary_", False)

    def __init__(self, json_file, need_crc=True, need_encrypted=False, gencode_only=False):
        self.partitions = []
        self.json_file_name = json_file
        self.need_crc = need_crc
        self.need_encrypted = need_encrypted
        self.gencode_only = gencode_only
        self.gen_partitions_bin_done = False

        self.load_json()
        self.parse_json()

    def empty_line(self, f):
        line = f'\r\n'
        f.write(line)

    def gen_partitions_constants(self):
        self.partition_hdr_file_name = f'{self.json_file_base_name}_partition.h'
        f = open(self.partition_hdr_file_name, 'w+')

        f.write(s_license)

        line = f'#define %-45s %s' %("KB(size)", "((size) << 10)\r\n")
        f.write(line)
        line = f'#define %-45s %s' %("MB(size)", "((size) << 20)\r\n\r\n")
        f.write(line)

        macro_name = f'CONFIG_CODE_ENCRYPTED'
        if self.need_encrypted:
            line = f'#define %-45s %d\r\n' %(macro_name, 1)
        else:
            line = f'#define %-45s %d\r\n' %(macro_name, 0)
        f.write(line)

        macro_name = f'CONFIG_CODE_HAS_CRC'
        if self.need_crc:
            line = f'#define %-45s %d\r\n' %(macro_name, 1)
            f.write(line)
            line = f'#define %-45s %s' %("FLASH_VIRTUAL2PHY(virtual_addr)", "((((virtual_addr) >> 5) * 34) + ((virtual_addr) & 31))\r\n")
            f.write(line)
            line = f'#define %-45s %s' %("FLASH_PHY2VIRTUAL(phy_addr)", "((((phy_addr) / 34) << 5) + ((phy_addr) % 34))\r\n")
            f.write(line)
        else:
            line = f'#define %-45s %d\r\n' %(macro_name, 0)
            f.write(line)
            line = f'#define %-45s %s' %("FLASH_VIRTUAL2PHY(virtual_addr)", "(virtual_addr)\r\n")
            f.write(line)
            line = f'#define %-45s %s' %("FLASH_PHY2VIRTUAL(phy_addr)", "(phy_addr)\r\n")
            f.write(line)

        macro_name = f'CPU_VECTOR_ALIGN_SZ'
        line = f'#define %-45s %d\r\n' %(macro_name, self.cpu_vector_align_bytes)
        f.write(line)

        self.empty_line(f)
        f.write(s_phy2code_start)
        self.empty_line(f)

        for idx in range(self.partitions_cnt):
            partition = self.partitions[idx]
            partition_name = partition.partition_name
            partition_name = partition_name.upper()
            partition_name = partition_name.replace(' ', '_')

            macro_name = f'CONFIG_{partition_name}_PHY_PARTITION_OFFSET'
            line = f'#define %-45s 0x%x\r\n' %(macro_name, partition.partition_offset)
            f.write(line)
            macro_name = f'CONFIG_{partition_name}_PHY_PARTITION_SIZE'
            line = f'#define %-45s 0x%x\r\n' %(macro_name, partition.partition_size)
            f.write(line)
            if partition.bin_is_code():
                macro_name = f'CONFIG_{partition_name}_PHY_CODE_START'
                line = f'#define %-45s 0x%x\r\n' %(macro_name, partition.bin_1st_instruction_offset)
                f.write(line)


                if (partition.bin_hdr_size > 0):
                    macro_name = f'CONFIG_{partition_name}_PHY_HDR_SIZE'
                    line = f'#define %-45s 0x%x\r\n' %(macro_name, partition.bin_hdr_size)
                    f.write(line)

                if (partition.bin_tail_size > 0):
                    macro_name = f'CONFIG_{partition_name}_PHY_TAIL_SIZE'
                    line = f'#define %-45s 0x%x\r\n' %(macro_name, partition.bin_tail_size)
                    f.write(line)

                if (partition.virtual_partition_size > 0):
                    macro_name = f'CONFIG_{partition_name}_VIRTUAL_PARTITION_SIZE'
                    line = f'#define %-45s 0x%x\r\n' %(macro_name, partition.virtual_partition_size)
                    f.write(line)

                    macro_name = f'CONFIG_{partition_name}_VIRTUAL_CODE_START'
                    line = f'#define %-45s 0x%x\r\n' %(macro_name, phy2virtual(partition.bin_1st_instruction_offset))
                    f.write(line)

                    macro_name = f'CONFIG_{partition_name}_VIRTUAL_CODE_SIZE'
                    line = f'#define %-45s 0x%x\r\n' %(macro_name, partition.virtual_code_size)
                    f.write(line)
            else:
                macro_name = f'CONFIG_{partition_name}_PHY_DATA_START'
                line = f'#define %-45s 0x%x\r\n' %(macro_name, partition.partition_offset)
                f.write(line)

            self.empty_line(f)

        f.flush()
        f.close()

    def gen_partitions_bin(self):

        if (self.gen_partitions_bin_done == True):
            return

        for idx in range(self.partitions_cnt):
            partition = self.partitions[idx]
            partition.process_bin()
 

    def gen_all(self):
        if (self.first_partition_offset != 0):
            logging.debug(f'first partition offset=%x, not generate all.bin' %(self.first_partition_offset))
            return

        self.gen_partitions_bin()

        f = open('all.bin', 'wb+')
        for idx in range(self.partitions_cnt):
            partition = self.partitions[idx]
            if (partition.bin_size > 0):
                f.seek(partition.partition_offset)
                f.write(partition.partition_buf)
                if (partition.bin_tail_size > 0):
                    f.seek(partition.bin_tail_offset)
                    f.write(partition.tail_buf)
        f.flush()
        f.close()

    def pack_file(self, tag):
        return f'{tag}.bin'

    def gen_write_file(self, f, offset, value, len):
        logging.debug(f'write file: offset=%x, len={len}' %(offset))
        f.seek(offset)
        f.write(value)
        return (offset + len)

    def gen_common(self, tag, pack_idx_list):
        pack_file_name = self.pack_file(tag)
        hdr = Hdr(magic=HDR_MAGIC, crc=0x12345678, version=0x0102, hdr_len=GLOBAL_HDR_LEN, img_num=len(pack_idx_list), flags=0)

        offset = (IMG_HDR_LEN * len(pack_idx_list)) + GLOBAL_HDR_LEN
        logging.debug(f'{tag} total header len=%x' %(offset))
        img_hdr_list = []
        for idx in pack_idx_list:
            partition = self.partitions[idx]
            img_hdr = Imghdr(img_len=partition.partition_size, img_offset=offset,
                flash_offset=partition.partition_offset, crc=partition.crc, version=int(partition.version, 16), flags=0)
            offset = offset + partition.partition_size
            img_hdr_list.append(img_hdr)

        offset = 0
        with open(pack_file_name, 'wb+') as f:
            offset = self.gen_write_file(f, offset, hdr.magic, 8)
            offset = self.gen_write_file(f, offset, hdr.crc, 4)
            offset = self.gen_write_file(f, offset, hdr.version, 4)
            offset = self.gen_write_file(f, offset, hdr.img_num, 2)
            offset = self.gen_write_file(f, offset, hdr.hdr_len, 2)
            offset = self.gen_write_file(f, offset, hdr.flags, 4)
            offset = self.gen_write_file(f, offset, hdr.reserved, 8)

            for img_hdr in img_hdr_list:
                offset = self.gen_write_file(f, offset, img_hdr.img_len, 4)
                offset = self.gen_write_file(f, offset, img_hdr.img_offset, 4)
                offset = self.gen_write_file(f, offset, img_hdr.flash_offset, 4)
                offset = self.gen_write_file(f, offset, img_hdr.crc, 4)
                offset = self.gen_write_file(f, offset, img_hdr.version, 4)
                offset = self.gen_write_file(f, offset, img_hdr.flags, 4)
                offset = self.gen_write_file(f, offset, img_hdr.reserved, 8)

            for idx in pack_idx_list:
                partition = self.partitions[idx]
                if (partition.bin_size > 0):
                    f.seek(offset)
                    f.write(partition.partition_buf)
                    if (partition.bin_tail_size > 0):
                        bin_tail_offset = offset + partition.partition_size - partition.bin_tail_size
                        f.seek(bin_tail_offset)
                        f.write(partition.tail_buf)
                logging.debug('start to append partitions{idx}: offset=%x, partition_size=%x, bin_size=%x, bin_tail_size=%x'
                    %(offset, partition.partition_size, partition.bin_size, partition.bin_tail_size))
                offset += partition.partition_size
 
    def gen_app(self):
        if (len(self.app_pack_idx_list) > 0):
            self.gen_partitions_bin()
            self.gen_common("apps", self.app_pack_idx_list)

    def gen_ota(self):
        if (len(self.ota_pack_idx_list) > 0):
            self.gen_partitions_bin()
            self.gen_common("ota", self.ota_pack_idx_list)

    def gen_manifest(self, gen, manifest_partition):
        found_boot_partition = False
        for idx in range(self.partitions_cnt):
            p = self.partitions[idx]
            if p.partition_name == manifest_partition.boot_partition:
                found_boot_partition = True
                break
        if found_boot_partition == False:
            logging.error(f'failed to find boot_partition {manifest_partition.boot_partition}')
            exit(1)

        manifest_name = f'{manifest_partition.partition_name}.json'
        gen.gen_manifest(p.version, p.static_addr, p.load_addr, p.bin_name, manifest_name)

    def gen_bl1_config(self):
        g = Genbl1('bl1_key.json')
        g.gen_key_desc()
        pre_partition_manifest_type = 0
        for idx in range(self.partitions_cnt):
            partition = self.partitions[idx]

            if (partition.partition_name == 'primary_manifest') or (partition.partition_name == 'secondary_manifest'):
                self.gen_manifest(g, partition)

    def gen_bl2_config(self):
        logging.info(f'TODO gen bl2')

