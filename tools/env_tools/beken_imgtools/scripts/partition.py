#!/usr/bin/env python3
import os
import json
import logging
import shutil
import re
import copy
import hashlib

from .crc import *
from .common import *
from .genbl1 import *

SZ_16M = 0x1000000
FLASH_SECTOR_SZ = 0x1000
CRC_UNIT_DATA_SZ = 32
CRC_UNIT_TOTAL_SZ = 34

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

def pattern_match(string, pattern):
    match = re.search(pattern, string)
    if match:
        return True
    else:
        return False

def signing(partition, in_bin_name, out_bin_name, key, security_counter, version='0.0.1'):
    logging.debug(f'signing: partition_name={partition.partition_name}, partition_size={partition.partition_size}')
    logging.debug(f'signing: in_bin_name={in_bin_name}, out_bin_name={out_bin_name}, in_bin_size={os.path.getsize(in_bin_name)}')
    logging.debug(f'signing: key={key}, security_counter={security_counter}')

    bl2_signing_tool_dir = f'{partition.tools_dir}/mcuboot_tools/imgtool.py'
    cmd = f'{bl2_signing_tool_dir} sign -k {key} --public-key-format full --max-align 8 --align 1 --version {version} --security-counter {security_counter} --pad-header --header-size 0x1000 --slot-size {partition.partition_size} --pad --boot-record SPE --endian little --encrypt-keylen 128 {in_bin_name} {out_bin_name}'
    run_cmd(cmd)
    return

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
        required_fields = ["partition", "size"]
        for field in required_fields:
            if field not in self.partition_json.keys():
                logging.error(f'partition{self.idx} {self.partition_name} missing "{field}"')
                exit(1)

        self.partition_name = self.partition_json['partition']
        match = pattern_match(self.partition_name, r"secondary_")
        if match == True:
            self.is_secondary = True
        else:
            self.is_secondary = False

        self.primary_partition_name = None
        self.primary_partition = None
        if self.is_secondary == True:
            self.primary_partition_name = self.partition_name.replace("secondary_", "primary_")
            self.primary_partition = self.find_partition_by_name(self.primary_partition_name)

        all_possible_fields = ["partition", "offset", "size", "bin",
            "bin_type", "bin_hdr_size", "bin_tail_size", "bin_pad_size", "bin_encrypted_addr",
            "version", "load_addr", "location", "verifier", "owner", "security"]
        for key in self.partition_json.keys():
            if key not in all_possible_fields:
                logging.error(f'partition{self.idx} {self.partition_name} unknown "{key}"')
                exit(1)
        logging.debug(f'partition{self.partition_name} is_secondary={self.is_secondary} primary_partition={self.primary_partition_name}')

    def parse_and_check_bin_location(self):
        self.bin_location = f''
        if "location" in self.partition_json.keys():
            self.bin_location = self.partition_json['location']
        logging.debug(f'partition{self.idx} location={self.bin_location}')

    def parse_and_check_bin(self):

        self.bin_name = None
        if "bin" in self.partition_json.keys():
            self.bin_name = self.partition_json['bin']
            self.bin_size = 0

        if (self.gencode_only == True) or (self.bin_name == None):
            self.bin_size = 0
            return

        if (self.bin_name == ''):
            self.bin_size = 0
            if self.bin_is_code():
                logging.error(f'partition{self.idx} {self.partition_name}: code partition should specify a bin')
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
                    logging.error(f'partition{self.idx} {self.partition_name} can not automatically detect {self.bin_name}!!!')
                    exit(1)
            else:
                logging.error(f'partition{self.idx} {self.partition_name} bin {self.bin_name} not exists')
                exit(1)
        self.bin_size = os.path.getsize(self.bin_name)
        logging.debug(f'bin name={self.bin_name}, size=%x' %(self.bin_size))

    def parse_and_check_bin_type(self):
        self.bin_type = "code"
        if "bin_type" in self.partition_json.keys():
            self.bin_type = self.partition_json['bin_type']

        if (self.bin_type != "code") and (self.bin_type != "data"):
            logging.error(f'partition{idx} {self.partition} type should be one of "code", "data"')
            exit(1)
        if (self.bin_type == "code"):
            self.need_add_crc = True
        else:
            self.need_add_crc = False
        logging.debug(f'bin type={self.bin_type} need_add_crc={self.need_add_crc}')

    def parse_and_check_verifier(self):
        self.bin_verifier = None

        if "verifier" not in self.partition_json.keys():
            return

        if "verifier" in self.partition_json.keys():
            self.bin_verifier = self.partition_json['verifier']

        if (self.bin_verifier != "bl1") and (self.bin_verifier != "bl2") and (self.bin_verifier != None):
            logging.error(f'partition{self.idx} {self.partition_name} verifier should be "bl1" or "bl2"')
            exit(1)


        if (self.bin_type == "data"):
            logging.error(f'partition{self.idx} {self.partition_name} skip verifier for data partition')
            return

        logging.debug(f'bin verifier={self.bin_verifier}')

    def parse_and_check_bin_hdr_tail_size(self):
        self.bin_hdr_size = 0;
        self.bin_tail_size = 0;

        if "bin_hdr_size" in self.partition_json.keys():
            self.bin_hdr_size = hex2int(self.partition_json['bin_hdr_size'])

        if "bin_tail_size" in self.partition_json.keys():
            self.bin_tail_size = hex2int(self.partition_json['bin_tail_size'])

        if (self.is_flash_sector_aligned(self.bin_hdr_size) == False):
            logging.error(f'partition{self.idx} {self.partition_name} hdr offset=%x not 4K aligned' %(self.bin_hdr_size))
            exit(1)

        if (self.is_flash_sector_aligned(self.bin_tail_size) == False):
            logging.error(f'partition{self.idx} {self.partition_name} tail offset=%x not 4K aligned' %(self.bin_tail_size))
            exit(1)

        if (self.bin_verifier == "bl2"):
            self.bin_hdr_size = 0x1000
            self.bin_tail_size = 0x1000

        logging.debug(f'bin bin_hdr_size=%x, bin_tail_size=%x' %(self.bin_hdr_size, self.bin_tail_size))

    def parse_and_check_partition_offset(self):
        if "offset" in self.partition_json.keys():
            self.partition_offset = hex2int(self.partition_json['offset'])
        else:
            self.partition_offset = self.deduced_min_partition_offset

        if (self.idx == 0) and (self.partition_offset > 0):
            logging.debug(f'partition{self.idx} {self.partition_name} first partition offset is 0')

        if (self.partition_offset < self.deduced_min_partition_offset):
            logging.error(f'partition{self.idx} partition {self.partition_name} offset=%x overlapped with previous partition' %(self.partition_offset))
            exit(1)

        if (self.is_out_of_range(self.partition_offset) == True):
            logging.error(f'partition{self.idx} partition {self.partition_name} offset=%x is out of range' %(self.partition_offset))
            exit(1)

        if (self.bin_is_code()):
            if (self.is_flash_sector_aligned(self.partition_offset) == False):
                logging.error(f'partition{self.idx} partition {self.partition_name} offset=%x not FLASH sector aligned' %(self.phy_partition_offset))
                exit(1)

        logging.debug(f'partition{self.idx} {self.partition_name} partition_offset=%x' %(self.partition_offset))

    def parse_and_check_partition_size(self):
        self.partition_size = size2int(self.partition_json['size'])
        if (self.is_out_of_range(self.partition_size) == True):
            logging.error(f'partition{self.idx} partition {self.partition_name} size=%x out of range' %(self.partition_size))
            exit(1)

        # TODO may need to make it sector aligned!!!


        logging.debug(f'partition{self.idx} {self.partition_name} partition size=%x' %(self.partition_size))
 
    def parse_and_check_bin_pad_size(self):
        self.bin_pad_size = 0
        if "bin_pad_size" in self.partition_json.keys():
            self.bin_pad_size = size2int(self.partition_json['bin_pad_size'])

        if (self.is_out_of_range(self.bin_pad_size) == True):
            logging.error(f'partition{self.idx} {self.partition_name} bin pad size=%x out of range' %(self.bin_pad_size))
            exit(1)

        if (self.is_flash_sector_aligned(self.bin_pad_size) == False):
            logging.error(f'partition{self.idx} {self.partition_name} bin pad size=%x not aligned' %(self.bin_pad_size))
            exit(1)

        logging.debug(f'partition{self.idx} {self.partition_name} bin pad size=%x' %(self.bin_pad_size))
 
    def parse_and_check_bin_encrypted_addr(self):
        self.bin_encrypted_addr = 0
        if "bin_encrypted_addr" in self.partition_json.keys():
            if (self.flash_base_addr == 0):
                logging.error(f'partition{self.idx} {self.partition_name} requires AES encrypt, should specify flash_base address')
                exit(1)
            else:
                self.bin_encrypted_addr = hex2int(self.partition_json['bin_encrypted_addr'])

        logging.debug(f'partition{self.idx} {self.partition_name} encrypted addr=%x' %(self.bin_encrypted_addr))
 
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
            logging.debug(f'partition{self.idx} {self.partition_name} msp=%s, pc=%s' %(self.msp, self.pc))
 
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

    def find_partition_by_name(self, name):
        for p in self.partitions.partitions:
            if (name == p.partition_name):
                return p
        return None

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
            logging.error(f'partition{self.idx} {self.partition_name} start=%x size=%x is out of range' %(self.partition_offset, self.partition_size))
            exit(1)

        if (self.bin_size > 0) and (self.bin_size <= (self.bin_hdr_size + self.bin_tail_size)):
            logging.error(f'partition{self.idx} {self.partition_name} bin size=%x <= (hdr_size=%x + tail_size=%x)' %(self.bin_size, self.bin_hdr_size, self.bin_tail_size))
            exit(1)

        if (self.bin_is_code()):
            self.bin_hdr_offset = self.partition_offset
            self.bin_tail_offset = self.partition_offset + self.partition_size - self.bin_tail_size
            self.bin_code_partition_offset = self.bin_hdr_offset + self.bin_hdr_size

            if (self.is_secondary) and (self.bin_verifier == "bl2"):
                if (self.primary_partition == None):
                    logging.error(f'partition{self.idx} {self.partition_name} missing primary partition')
                    exit(1)
                
                self.bin_code_aligned_pad_size = self.primary_partition.bin_code_aligned_pad_size
                self.bin_1st_instruction_offset = self.partition_offset + self.bin_code_aligned_pad_size + self.bin_hdr_size
                self.virtual_partition_size = self.primary_partition.virtual_partition_size
                self.virtual_code_size = self.primary_partition.virtual_code_size
                logging.debug(f'partition{self.idx} {self.partition_name} bin_1st_instruction_offset={self.bin_1st_instruction_offset} bin_code_aligned_pad_size={self.bin_code_aligned_pad_size}')
                logging.debug(f'partition{self.idx} {self.partition_name} virtual_partition_size={self.virtual_partition_size} virtual_code_size={self.virtual_code_size}')
            else:
                min_phy_1st_instruction_offset = ceil_align(self.bin_code_partition_offset, CRC_UNIT_TOTAL_SZ)
                min_virtual_1st_instrution_offset = phy2virtual(min_phy_1st_instruction_offset)
                virtual_1st_vector_instruction_offset = ceil_align(min_virtual_1st_instrution_offset, self.cpu_vector_align_bytes)
                self.bin_1st_instruction_offset = virtual2phy(virtual_1st_vector_instruction_offset)
                self.bin_code_aligned_pad_size = self.bin_1st_instruction_offset - self.bin_code_partition_offset
                logging.debug(f'partition{self.idx} {self.partition_name} partition offset=%x, hdr_offset=%x, tail_offset=%x, code_partition_offset=%x, 1st_instruction_offset=%x'
                    %(self.partition_offset, self.bin_hdr_offset, self.bin_tail_offset, self.bin_code_partition_offset, self.bin_1st_instruction_offset))
                logging.debug(f'partition{self.idx} {self.partition_name}min_phy_1st_instruction=%x, min_virtual_1st_instrution_offset=%x' %(min_phy_1st_instruction_offset,
                    min_virtual_1st_instrution_offset))
                logging.debug(f'partition{self.idx} {self.partition_name} virtual_1st_vector_instruction_offset=%x, bin_1st_instruction_offset=%x' %(virtual_1st_vector_instruction_offset,
                    self.bin_1st_instruction_offset))
   
                if (self.partition_name == 'primary_tfm_s'):
                    s_ns_boundary = self.partition_offset + self.partition_size
                    if (s_ns_boundary % (68<<10)) != 0:
                        block = s_ns_boundary // (68<<10)
                        suggest_s_ns_boundary = (block + 1) * (68 << 10)
                        diff = suggest_s_ns_boundary - s_ns_boundary
                        logging.debug(f'NS start {s_ns_boundary/1024}, suggest start {suggest_s_ns_boundary/1024}')
                        logging.error(f'The offset of partition next to TFM_S is %x, NOT in 68K boundary!' %(s_ns_boundary))
                        size_k = self.partition_size/1024
                        suggest_size_k = (self.partition_size + diff)/1024
                        logging.error(f'Suggest to change TFM_S partition size from {size_k}k to {suggest_size_k}k')
                        exit(1)
                    
                
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
            logging.error(f'partition{self.idx} {self.partition_name} bin size %x > partition size %x' %(self.bin_size, self.partition_size))
            exit(1)

        if (self.bin_total_size > self.partition_size):
            logging.error(f'partition{self.idx} {self.partition_name} total bin size %x > partition size %x' %(self.bin_total_size, self.partition_size))
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

    def bl2_layout_contains_only_one_bin(self):
        if ((self.partitions.is_1st_bin_verified_by_bl2(self.partition_name) == True)
            and (self.partitions.is_last_bin_verified_by_bl2(self.partition_name) == True)):
            return True
        return False

    def is_1st_bin_verified_by_bl2(self):
        return self.partitions.is_1st_bin_verified_by_bl2(self.partition_name)

    def is_last_bin_verified_by_bl2(self):
        return self.partitions.is_last_bin_verified_by_bl2(self.partition_name)

    def process_code_bin(self):
        if self.partition_name == 'primary_all' or self.partition_name == 'secondary_all':
            return

        if self.bin_name == None:
            return

        if self.partitions.aes_key != None:
            self.aes_bin_name = f'{self.file_name_prefix}_code_aes.bin'
            aes_tool = f'{self.tools_dir}/beken_packager/cmake_xts_aes'
            start_address = hex(phy2virtual(self.bin_1st_instruction_offset))
            logging.debug(f'encrypt {self.partition_name}, startaddress={start_address}')
            cmd = f'{aes_tool} encrypt -infile {self.bin_name} -keywords {self.partitions.aes_key} -outfile {self.aes_bin_name} -startaddress {start_address}'
            run_cmd_not_check_ret(cmd)
        else:
            self.aes_bin_name = self.bin_name

        # Add CRC
        self.crc_bin_name = f'{self.file_name_prefix}_code_crc.bin'
        if (self.need_add_crc == True):
            crc(self.aes_bin_name, self.crc_bin_name)
        else:
            self.crc_bin_name = self.aes_bin_name

        # Process code aligned pad
        if (self.bin_code_aligned_pad_size > 0):
            self.crc_aligned_bin_name = f'{self.file_name_prefix}_code_crc_aligned.bin'
            crc_aligned_buf = bytearray(self.bin_code_aligned_pad_size)
            self.init_buf(crc_aligned_buf, self.bin_code_aligned_pad_size, 0xff)
            with open(self.crc_bin_name, 'rb') as crcf:
                crc_aligned_buf += crcf.read()

            with open(self.crc_aligned_bin_name, 'wb+') as crc_aligned_f:
                crc_aligned_f.write(crc_aligned_buf)
        else:
            self.crc_aligned_bin_name = self.crc_bin_name

        # Signing
        bin_is_signed = False
        if (self.bin_verifier == "bl2"):
            self.signed_bin_name = f'{self.file_name_prefix}_code_signed.bin'
            if (self.bl2_slot_type == "multiple"):
                signing(self, self.crc_aligned_bin_name, self.signed_bin_name, self.partitions.bl2_privkey, self.partitions.bl2_security_counter)
                bin_is_signed = True
            else:
                self.signed_bin_name = self.crc_aligned_bin_name
        else:
            self.signed_bin_name = self.crc_aligned_bin_name

        self.signed_bin_size = os.path.getsize(self.signed_bin_name)
        self.partition_buf = bytearray()
        with open(self.signed_bin_name, 'rb') as f:

            if (bin_is_signed == True):
                self.partition_buf = f.read()
                return

            # Process header
            pad_header_buf_size = 0
            if self.bin_hdr_size > 0:
                if ((self.bl2_slot_type == "single") and (self.is_1st_bin_verified_by_bl2() == False)):
                    # Padding header
                    pad_header_buf_size = self.bin_hdr_size
                    pad_header_buf = bytearray(pad_header_buf_size)
                    self.init_buf(pad_header_buf, pad_header_buf_size, 0xff)
                    self.partition_buf += pad_header_buf

            # Process code
            self.code_buf = f.read()
            self.partition_buf += self.code_buf

            # Process tail padding
            if (self.bin_verifier == "bl2") and (self.bl2_slot_type == "single") and self.is_last_bin_verified_by_bl2():
                pad_tail_buf_size = 0
            else:
                pad_tail_buf_size = self.partition_size - self.bin_hdr_size - self.signed_bin_size

            if (pad_tail_buf_size > 0):
                pad_tail_buf = bytearray(pad_tail_buf_size)
                self.init_buf(pad_tail_buf, pad_tail_buf_size, 0xff)
                self.partition_buf += pad_tail_buf

            logging.debug(f'Padding header buf size={pad_header_buf_size}')
            logging.debug(f'Padding code align size={self.bin_code_aligned_pad_size}')
            logging.debug(f'Padding tail buf size={pad_tail_buf_size}')
            logging.debug(f'bin_size= {self.bin_size} signed_bin_size={self.signed_bin_size}')
            logging.debug(f'hdr_size={self.bin_hdr_size} tail_size={self.bin_tail_size}')

    def process_bin(self):
        if (self.bin_is_code()):
            self.process_code_bin()
        else:
            self.process_data_bin()

        if (self.bin_size > 0):
            self.temp_partition_bin_name = f'{self.file_name_prefix}_partition.bin'
            with open(self.temp_partition_bin_name, 'wb+') as pf:
                pf.write(self.partition_buf)

    def __init__(self, partitions, idx, partition_dic, deduced_min_partition_offset):
        self.partitions = partitions
        self.bl2_slot_type = partitions.bl2_slot_type
        self.tools_dir = partitions.tools_dir
        self.idx = idx
        self.partition_json = partition_dic
        self.deduced_min_partition_offset = deduced_min_partition_offset
        self.flash_base_addr = partitions.flash_base_addr
        self.json_file_base_name = partitions.json_file_base_name
        self.gencode_only = partitions.gencode_only
        self.cpu_vector_align_bytes = partitions.cpu_vector_align_bytes
        self.need_add_crc = False
        self.crc = 0 # TODO

        self.file_name_prefix = f'{self.json_file_base_name}_{idx}'
        logging.debug(f'Partitions: tools_dir={self.tools_dir}')

        self.check_fields()
        logging.debug(f'processing {self.json_file_base_name}.partition[{idx}].{self.partition_name}')
        self.parse_and_check_bin_type() # partition checking used bin type, so parse bin type firstly
        self.parse_and_check_verifier()
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

class Partitions:

    def load_json(self, json_file_name):
        if not os.path.exists(json_file_name):
            logging.error(f'JSON configuration file {json_file_name} not exists')
            exit(1)

        json_file_base_name = os.path.basename(json_file_name)
        logging.debug(f'processing json {json_file_base_name}, crc={self.need_crc}, encrypt={self.need_encrypted}')
        with open(json_file_name, 'r') as json_file:
            try:
                json_data = json.load(json_file)
                return json_data
            except Exception as e:
                logging.error(f"Could not open : '{self.json_file_name}': {e}", err=True)
                exit(1)

    def is_pack_default(self, pack_list):
        if (len(pack_list) == 1) and (pack_list[0] == 'default'):
            return True
        return False

    def partition_name_to_idx(self, partition_name):
        idx = 0
        for p in self.partitions:
            if (partition_name == p.partition_name):
                return idx
            idx += 1
        return -1

    def build_pack_list(self, tag, pack_list, prefix, match, include_partitions, exclude_partitions):
        logging.debug(f'build {tag} list={pack_list}')
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
                logging.error(f'{tag} pack append partition {partition_name}')
            logging.debug(f'{tag} pack partition list={pack_idx_list}') 
            return pack_idx_list

        idx = 0

        #TODO consider secureboot
        for p in self.partitions:
            partition_name = p.partition_name
            insert = False

            logging.debug(f'{tag} pack: check partition {partition_name}')
            if (partition_name != 'primary_all') and (partition_name != 'secondary_all') and (p.bin_size <= 0):
                continue

            if partition_name in include_partitions:
                insert = True
            elif partition_name in exclude_partitions:
                insert = False
            elif partition_name.startswith(prefix):
                if (match == True):
                    insert = True
            else:
                if (match == False):
                    insert = True

            if insert == True:
                pack_idx_list.append(idx)
                logging.error(f'{tag} pack append partition {partition_name}')
            idx += 1
        logging.debug(f'{tag} partitions list={pack_idx_list}')
        return pack_idx_list

    def is_1st_bin_verified_by_bl2(self, partition_name):
        if (self.primary_all_partitions_cnt > 0) and (self.primary_partitions_verified_by_bl2[0] == partition_name):
            return True

        if (self.secondary_all_partitions_cnt > 0) and (self.secondary_partitions_verified_by_bl2[0] == partition_name):
            return True

        return False

    def is_last_bin_verified_by_bl2(self, partition_name):
        if (self.primary_all_partitions_cnt > 0) and (self.primary_partitions_verified_by_bl2[-1] == partition_name):
            return True

        if (self.secondary_all_partitions_cnt > 0) and (self.secondary_partitions_verified_by_bl2[-1] == partition_name):
            return True

        return False

    def find_partition_by_name(self, name):
        for p in self.partitions:
            if (name == p.partition_name):
                return p
        return None

    def gen_primary_secondary_all_bin(self, partition_name, child_partition_names):
        length = len(child_partition_names)
        if (length <= 0):
            logging.debug(f'Skip generate partition {partition_name}')
            return

        if (self.bl2_slot_type == 'multiple'):
            return

        if (self.prefer_primary_secondary_all == False):
            return

        partition_buf = bytearray()
        for pname in child_partition_names:
            p = self.find_partition_by_name(pname)
            partition_buf += p.partition_buf

        all_bin_name = f'{self.json_file_base_name}_{partition_name}_code.bin'
        with open(all_bin_name, 'wb+') as f:
            f.write(partition_buf)

        all_bin_signed_name = f'{self.json_file_base_name}_{partition_name}_code_signed.bin'
        all_bin_partition = self.find_partition_by_name(partition_name)
        signing(all_bin_partition, all_bin_name, all_bin_signed_name, self.bl2_privkey, self.bl2_security_counter)

        with open(all_bin_signed_name, 'rb+') as f:
            all_bin_partition.partition_buf = f.read()
            all_bin_partition.bin_size = len(all_bin_partition.partition_buf)

    def create_primary_secondary_all_partition(self, partition_name, child_partition_names):
        if self.bl2_slot_type == "multiple":
            return

        child_num = len(child_partition_names)
        if (child_num <= 0):
            return

        partition_name_1st = child_partition_names[0]
        partition_1st = self.find_partition_by_name(partition_name_1st)

        partition_name_last = child_partition_names[-1]
        partition_last = self.find_partition_by_name(partition_name_last)

        all_partition = copy.copy(partition_1st)

        child_partition_num = len(child_partition_names)
        all_partition.prefer_primary_secondary_all = False
        if (child_partition_num > 0):
            self.prefer_primary_secondary_all = True

        all_partition.bin_size = 0
        all_partition.partition_name = partition_name
        all_partition.partition_size = (partition_last.partition_offset - partition_1st.partition_offset) + partition_last.partition_size

        if (partition_name == 'primary_all'):
            virtual_partition_size = phy2virtual(all_partition.partition_size - all_partition.bin_hdr_size - all_partition.bin_tail_size)
            all_partition.virtual_partition_size = floor_align(virtual_partition_size, CRC_UNIT_DATA_SZ)
            virtual_code_size = phy2virtual(all_partition.partition_size - all_partition.bin_code_aligned_pad_size - all_partition.bin_hdr_size - all_partition.bin_tail_size - 34)
            all_partition.virtual_code_size = floor_align(virtual_code_size, CRC_UNIT_DATA_SZ)
        else:
            partition_primary = self.find_partition_by_name('primary_all')
            all_partition.virtual_partition_size = partition_primary.virtual_partition_size
            all_partition.virtual_code_size = partition_primary.virtual_code_size

        idx = self.partition_name_to_idx(partition_name_last)
        self.partitions.insert(idx + 1, all_partition)
        logging.debug(f'create virtual partition: {partition_name} id={idx}')
        logging.debug(f'create virtual partition: offset={all_partition.partition_offset}, size={all_partition.partition_size}')
        logging.debug(f'create virtual partition: virtual_partition_size={all_partition.virtual_partition_size}, virtual_code_size={all_partition.virtual_code_size}')

    def parse_and_check_aes_key(self):
        self.aes_key = None
        if "aes_key" in self.json_data.keys():
            self.aes_key = self.json_data['aes_key']
            if len(self.aes_key) != 64:
                logging.error(f'Invalid AES key: key length should be 64')
                exit(1)

    def parse_partition_names(self):
        for pjson in self.json_data['partitions']:
            if 'partition' not in pjson.keys():
                logging.error(f'partition{idx} missing field "partition"')
                exit(1)
            self.partition_names.append(pjson['partition'])

    def parse_and_check_secureboot(self):
        self.secure_boot = False
        if (self.boot_mode == 'secureboot'):
            self.secure_boot = True

        logging.debug(f'secure boot is {self.secure_boot}')
        if (self.secure_boot == True):
            logging.debug(f'When secure debug is enabled, "bl1_control", "primary_manifest", "secondary_manifest", "primary_bl2", "secondary_bl2" are required')

        if (self.secure_boot == True):
            if ('bl1_control' not in self.partition_names):
                logging.error('Secure boot is enabled, but missing partition "bl1_control"')
                exit(1)

            if ('primary_manifest' not in self.partition_names):
                logging.error('Secure boot is enabled, but missing partition "primary_manifest"')
                exit(1)

            if ('secondary_manifest' not in self.partition_names):
                logging.error('Secure boot is enabled, but missing partition "secondary_manifest"')
                exit(1)

            if ('primary_bl2' not in self.partition_names):
                logging.error('Secure boot is enabled, but missing partition "primary_bl2"')
                exit(1)

            if ('secondary_bl2' not in self.partition_names):
                logging.error('Secure boot is enabled, but missing partition "secondary_bl2"')
                exit(1)

    def check_duplicated_partitions(self):
        if (len(self.partition_names) != len(set(self.partition_names))):
            logging.error(f'contains duplicated partitions!')
            return False
        return True

    def parse_and_check_app_ota_partitions(self):
        return

    def parse_and_check_tfm_ns(self):
        self.tfm_ns_exists = False
        self.primary_cpu0_app_exists = False

        primary_tfm_ns_partition = self.find_partition_by_name('primary_tfm_ns')
        primary_cpu0_app_partition = self.find_partition_by_name('primary_cpu0_app')

        if (primary_tfm_ns_partition != None) and (primary_cpu0_app_partition != None):
            logging.error(f'Partition "primary_cpu0_app" and "tfm_ns" should NOT exist at same time')
            exit(1)

        if primary_tfm_ns_partition != None:
            self.tfm_ns_exists = True

        if primary_cpu0_app_partition != None:
            self.primary_cpu0_app_exists = True

        logging.debug(f'tfm_ns exists={self.tfm_ns_exists}, primary_cpu0_app={self.primary_cpu0_app_exists}')

    def parse_and_check_verifier(self):
        idx = -1
        for pjson in self.json_data['partitions']:
            idx += 1
            partition_name = pjson['partition']

            if 'verifier' not in pjson.keys():
                continue

            if (pjson['verifier'] == 'bl1'):
                if (partition_name != 'primary_bl2') and (partition_name != 'secondary_bl2'):
                    logging.error(f'Partitions verified by bl1 should be "primary_bl2" or "secondary_bl2"!')
                    exit(1)
                continue

            if pjson['verifier'] != 'bl2':
                continue

            match = pattern_match(partition_name, r"primary_")
            if (match == True):
                self.primary_partitions_verified_by_bl2.append(partition_name)
        
            match = pattern_match(partition_name, r"secondary_")
            if (match == True):
                self.secondary_partitions_verified_by_bl2.append(partition_name)
        self.primary_all_partitions_cnt = len(self.primary_partitions_verified_by_bl2)
        self.secondary_all_partitions_cnt = len(self.secondary_partitions_verified_by_bl2)
        print(f'primary partitions verfied by bl2: {self.primary_partitions_verified_by_bl2}')
        print(f'secondary partitions verfied by bl2: {self.secondary_partitions_verified_by_bl2}')

    def parse_json_partitions(self):
        partitions_cnt = len(self.json_data['partitions'])
        if (partitions_cnt == 0):
            logging.error(f'partitions of json does not contain any item!')
            exit(1)

        self.parse_partition_names()
        self.check_duplicated_partitions()
        self.parse_and_check_verifier()
        self.parse_and_check_secureboot()
        self.parse_and_check_aes_key()
        self.parse_and_check_app_ota_partitions()

        deduced_next_partition_offset = 0
        self.first_partition_offset = 0
        for idx in range(partitions_cnt):
            partition = Partition(self, idx, self.json_data['partitions'][idx], deduced_next_partition_offset)
            deduced_next_partition_offset = partition.partition_offset + partition.partition_size
            self.partitions.append(partition)
            if (idx == 0):
                self.first_partition_offset = partition.partition_offset

        self.parse_and_check_tfm_ns()
        self.create_primary_secondary_all_partition('primary_all', self.primary_partitions_verified_by_bl2)
        self.create_primary_secondary_all_partition('secondary_all', self.secondary_partitions_verified_by_bl2)
        ota_exclude = self.secondary_partitions_verified_by_bl2
        ota_include = []
        app_exclude = self.primary_partitions_verified_by_bl2
        app_include = []
        self.ota_pack_idx_list = self.build_pack_list("OTA", self.pack_ota, "secondary_", True, ota_include, ota_exclude)
        self.app_pack_idx_list = self.build_pack_list("APP", self.pack_app, "secondary_", False, app_include, app_exclude)

    def parse_security(self):
        self.bl1_key_type = 'ec256'
        self.bl1_pubkey = 'bl1_ec256_pubkey.pem'
        self.bl1_privkey = 'bl1_ec256_privkey.pem'
        self.bl1_security_counter = 0
        logging.debug(f'bl1, key_type={self.bl1_key_type}, privkey={self.bl1_privkey}, pubkey={self.bl1_pubkey}, security_counter={self.bl1_security_counter}')

        self.bl2_key_type = 'rsa3072'
        self.bl2_pubkey = 'bl2_root_rsa3072.pem'
        self.bl2_privkey = 'bl2_root_rsa3072.pem'
        self.bl2_security_counter = 0
        logging.debug(f'bl1, key_type={self.bl1_key_type}, privkey={self.bl1_privkey}, pubkey={self.bl1_pubkey}, security_counter={self.bl2_security_counter}')
        logging.debug(f'bl2, key_type={self.bl2_key_type}, privkey={self.bl2_privkey}, pubkey={self.bl2_pubkey}')

    def parse_json_globals(self):
        if not os.path.exists(self.user_json_file_name):
            logging.error(f'User partition json not exists, skip json merge')
            exit(1)

        self.user_json_data = self.load_json(self.user_json_file_name)

        self.flash_base_addr = 0x02000000
        if ("flash_base_addr" in self.user_json_data):
            self.flash_base_addr = hex2int(self.user_json_data['flash_base_addr'])
        logging.debug(f'flash base=%x' %(self.flash_base_addr))

        self.flash_size = 0x400000
        if ("flash_size" in self.user_json_data):
            self.flash_size = size2int(self.user_json_data['flash_size'])
        logging.debug(f'flash size=%x' %(self.flash_size))
 
        self.cpu_vector_align_bytes = 512
        if ("cpu_vector_align" in self.user_json_data):
            self.cpu_vector_align_bytes = hex2int(self.user_json_data['cpu_vector_align'])
        logging.debug(f'cpu vector align bytes=%x' %(self.cpu_vector_align_bytes))

        if ("pack_ota" not in self.user_json_data):
            self.pack_ota = ['default']
        else:
            pack_ota_str = self.user_json_data['pack_ota']
            if pack_ota_str == '':
                pack_ota_str = 'default'

            self.pack_ota = pack_ota_str.split(',')
        logging.debug(f'OTA binary list: {self.pack_ota}')

        if ("pack_app" not in self.user_json_data):
            self.pack_app = ['default']
        else:
            pack_app_str = self.user_json_data['pack_app']
            if pack_app_str == '':
                pack_app_str = 'default'
            self.pack_app = pack_app_str.split(',')
        logging.debug(f'APP binary list: {self.pack_app}')

        if ("bl2_slot_type" not in self.user_json_data):
            self.bl2_slot_type = "single"
        else:
            self.bl2_slot_type = self.user_json_data['bl2_slot_type']
        logging.debug(f'BL2 bin blob: {self.bl2_slot_type}')

        self.secureboot = None
        self.boot_mode = 'legacyboot'
        if ("boot_mode" in self.user_json_data):
            self.boot_mode = self.user_json_data['boot_mode']
        logging.debug(f'boot_mode={self.boot_mode}')

        if ("security" in self.user_json_data):
            self.security = self.user_json_data['security']
            self.parse_security()

        if (self.boot_mode != 'legacyboot') and (self.boot_mode != 'secureboot'):
            logging.error(f'Invalid boot_mode, should be "secureboot" or "legacyboot"')
            exit(1)

        if ("partitions" not in self.user_json_data):
            logging.error(f'sys partition not has "partitions"')
            exit(1)

    def merge_sys_user_json(self):

        if not os.path.exists(self.sys_json_file_name):
            logging.debug(f'System partition not exists, skip json merge')
            return

        self.sys_json_data = self.load_json(self.sys_json_file_name)
        if ("partitions" not in self.sys_json_data):
            logging.error(f'sys partition not has "partitions"')
            exit(1)

        if ("partitions" not in self.user_json_data):
            logging.error(f'sys partition not has "partitions"')
            exit(1)


        idx = -1
        sys_partitions = self.sys_json_data['partitions']
        user_partitions = self.user_json_data['partitions']
        insert_idx = 0
        first_tail_partition = None

        tfm_partition_list = ['sys_ps', 'sys_its', 'sys_otp_nv']
        self.tfm_exists = False
        self.bl2_exists = False

        for p in user_partitions:
            if 'partition' in p.keys():
                partition_name = p['partition']
                if (partition_name == 'primary_tfm_s'):
                    self.tfm_exists = True

                if (partition_name == 'primary_bl2'):
                    self.bl2_exists = True

        logging.debug(f'merge json: tfm_exists={self.tfm_exists}, bl2_exists={self.bl2_exists}')
        total_sys_partition_size = 0
        for p in sys_partitions:
            idx += 1
            owner = self.boot_mode

            if ('owner' in p.keys()):
                owner = p['owner']

            if (owner != 'secureboot') and (owner != 'legacyboot'):
                logging.error(f'Invalid owner, should be "secureboot" or "legacyboot"')
                exit(1)

            if ('partition' not in p.keys()):
                logging.error(f'system partition{idx} missing "partition"')
                exit(1)

            partition_name = p['partition']

            if (self.bl2_exists and (partition_name == 'sys_scratch')):
                logging.debug(f'Merge {partition_name} bcause BL2 exists')
            elif ((self.bl2_exists or self.tfm_exists) and partition_name in tfm_partition_list):
                logging.debug(f'Merge {partition_name} bcause TFM_S exists')
            elif (owner != self.boot_mode):
                logging.debug(f'Skip system partition{idx}')
                continue

            partition_size = 0
            append_to_tail = pattern_match(partition_name, r"sys_")
            if (append_to_tail == True):
                if first_tail_partition == None:
                    first_tail_partition = p
                if 'size' not in p.keys():
                    logging.debug(f'system partition {partition_name} missing size')
                    exit(1)
                partition_size = size2int(p['size'])
                total_sys_partition_size += partition_size
                user_partitions.append(p)
                logging.debug(f'Append system partition {partition_name} size={partition_size} to user partition, total partition num={len(user_partitions)}')
            else:
                user_partitions.insert(insert_idx, p)
                logging.debug(f'Insert system partition {partition_name} to user partition, total partition num={len(user_partitions)}')
                insert_idx += 1
        if first_tail_partition != None:
            first_tail_partition['offset'] = f'%x' %(self.flash_size - total_sys_partition_size)
            logging.debug(f'First system partition offset=%x' %(self.flash_size - total_sys_partition_size))

        self.json_data = self.user_json_data
        #with open(self.all_json_file_name, 'w+') as f:
        #    json.dump(f, self.user_json_data)
        #    logging.debug(f'Create all_partitions.json')

    def __init__(self, user_json_file, sys_json_file, tools_dir=None, need_crc=True, need_encrypted=False, gencode_only=False):
        self.partition_names = []
        self.partitions = []
        self.user_json_file_name = user_json_file
        self.sys_json_file_name = sys_json_file
        self.all_json_file_name = 'all_partitions.json'
        self.json_file_base_name = 'all_partitions_json'
        self.tools_dir = tools_dir
        self.need_crc = need_crc
        self.need_encrypted = need_encrypted
        self.gencode_only = gencode_only
        self.gen_partitions_bin_done = False
        self.primary_partitions_verified_by_bl2 = [];
        self.secondary_partitions_verified_by_bl2 = [];
        self.prefer_primary_secondary_all = False
        self.boot_mode = 'legacyboot'
        self.crc32_table = []

        self.parse_json_globals()
        self.merge_sys_user_json()
        self.parse_json_partitions()

    def empty_line(self, f):
        line = f'\r\n'
        f.write(line)

    def gen_partitions_constants(self):
        self.partition_hdr_file_name = f'partitions_partition.h'
        f = open(self.partition_hdr_file_name, 'w+')

        logging.debug(f'Create partition constants file: {self.partition_hdr_file_name}')
        f.write(s_license)

        line = f'#define %-45s %s' %("KB(size)", "((size) << 10)\r\n")
        f.write(line)
        line = f'#define %-45s %s' %("MB(size)", "((size) << 20)\r\n\r\n")
        f.write(line)

        line = f'#define %-45s 0x%x%s' %("CONFIG_PARTITION_FLASH_SIZE", self.flash_size, "\r\n\r\n")
        f.write(line)

        if (self.bl2_slot_type == 'single'):
            line = f'#define %-45s %s' %("CONFIG_BL2_SLOT_SINGLE", "1\r\n")
        else:
            line = f'#define %-45s %s' %("CONFIG_BL2_SLOT_SINGLE", "0\r\n")

        macro_name = f'CONFIG_CODE_ENCRYPTED'
        if self.need_encrypted:
            line = f'#define %-45s %d\r\n' %(macro_name, 1)
        else:
            line = f'#define %-45s %d\r\n' %(macro_name, 0)
        f.write(line)

        macro_name = f'CONFIG_TFM_S_JUMP_TO_TFM_NS'
        if self.tfm_ns_exists == True:
            line = f'#define %-45s %d\r\n' %(macro_name, 1)
        else:
            line = f'#define %-45s %d\r\n' %(macro_name, 0)
        f.write(line)

        macro_name = f'CONFIG_TFM_S_JUMP_TO_CPU0_APP'
        if self.primary_cpu0_app_exists == True:
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

        for partition in self.partitions:

            partition_name = partition.partition_name
            partition_name = partition_name.upper()
            partition_name = partition_name.replace(' ', '_')

            logging.debug(f'generate constants for partition {partition_name}')

            macro_name = f'CONFIG_{partition_name}_PHY_PARTITION_OFFSET'
            line = f'#define %-45s 0x%x\r\n' %(macro_name, partition.partition_offset)
            f.write(line)
            macro_name = f'CONFIG_{partition_name}_PHY_PARTITION_SIZE'
            line = f'#define %-45s 0x%x\r\n' %(macro_name, partition.partition_size)
            f.write(line)

            if partition.bin_is_code() and partition.bin_name != None:
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

            self.empty_line(f)

        f.flush()
        f.close()

    def gen_partitions_bin(self):

        if (self.gen_partitions_bin_done == True):
            return

        for p in self.partitions:
            p.process_bin()
 

    def bin_need_to_bypass_in_bl2_merged_bin(self, name):
        if (self.bl2_slot_type == 'multiple'):
            return False

        p = self.find_partition_by_name(name)
        if (p.bin_verifier != "bl2"):
            return False

        m1 = pattern_match(name, r"primary_")
        m2 = pattern_match(name, r"secondary_")
        if (m1 == False) and (m2 == False):
            return False

        if (self.prefer_primary_secondary_all == True):
            if (name == 'primary_all') or (name == 'secondary_all'):
                return False
            else:
                return True
        else:
            if (name == 'primary_all') or (name == 'secondary_all'):
                return True
            else:
                return False
 
    def gen_manifest_bin(self, manifest_json_file_name):
        if (self.secure_boot == False):
            return

        manifest_bin_name = manifest_json_file_name.replace("json", "bin")
        sh_sec_tools = f'{self.tools_dir}/sh_sec_tools/secure_boot_tool'
        pwd = os.getcwd()
        cmd = f'{sh_sec_tools} -k key_desc.json -m {manifest_json_file_name} -o {pwd}/'
        run_cmd(cmd)
        cmd = f'mv manifest.bin {manifest_bin_name}'
        run_cmd(cmd)

    def gen_merged_partitions_bin(self, bin_name, partition_idx_list):
        f = open(bin_name, 'wb+')
        for idx in partition_idx_list:
            p = self.partitions[idx]
            if (p.bin_size > 0):
                logging.debug(f'create {bin_name}: {p.partition_name} seek={p.partition_offset}')
                f.seek(p.partition_offset)
                f.write(p.partition_buf)
        f.flush()
        f.close()

    def gen_all_bin(self):
        if (self.first_partition_offset != 0):
            logging.debug(f'first partition offset=%x, not generate all.bin' %(self.first_partition_offset))
            return

        self.gen_manifest_bin('primary_manifest.json')
        self.gen_manifest_bin('secondary_manifest.json')
        self.gen_partitions_bin()
        self.gen_primary_secondary_all_bin('primary_all', self.primary_partitions_verified_by_bl2)
        self.gen_primary_secondary_all_bin('secondary_all', self.secondary_partitions_verified_by_bl2)

        all_bin_partition_list = []
        idx = -1
        for p in self.partitions:
            idx += 1
            if (p.bin_size > 0):
                if self.bin_need_to_bypass_in_bl2_merged_bin(p.partition_name):
                    continue
                all_bin_partition_list.append(idx)

        self.gen_merged_partitions_bin('all.bin', all_bin_partition_list)

    def gen_write_file(self, f, offset, value, len):
        logging.debug(f'write file: offset=%x, len={len}' %(offset))
        f.seek(offset)
        f.write(value)
        return (offset + len)


    def init_crc32_table(self):
        self.crc32_table = []

        for i in range(0,256):
            c = i
            for j in range(0,8):
                if c&1:
                    c = 0xEDB88320 ^ (c >> 1)
                else:
                    c = c >> 1
            self.crc32_table.append(c)
    
    def crc32(self, crc, buf):
        for c in buf:
            crc = (crc>>8) ^ self.crc32_table[(crc^c)&0xff]
        return crc

    def gen_ota_global_hdr(self, img_num, img_content_list, img_hdr_list, global_security_counter):
        magic = "BK723658".encode()
        magic = struct.pack('8s', magic)
        global_security_counter = struct.pack('<I', global_security_counter)
        hdr_len = struct.pack('<H', GLOBAL_HDR_LEN)
        img_num = struct.pack('<H', img_num)
        flags = struct.pack('<I', 0)
        reserved1 = struct.pack('<I', 0)
        reserved2 = struct.pack('<I', 0)
        global_crc_content = global_security_counter + hdr_len + img_num + flags + reserved1 + reserved2
        for img_hdr in img_hdr_list:
            global_crc_content += img_hdr

        for img_content in img_content_list:
            global_crc_content += img_content

        self.init_crc32_table()
        global_crc = self.crc32(0xffffffff, global_crc_content)
        global_crc = struct.pack('<I', global_crc)
        ota_global_hdr = magic + global_crc + global_security_counter + hdr_len + img_num + flags + reserved1 + reserved2
        logging.debug(f'add OTA global hdr: magic={magic}, img_num={img_num}, security_counter={global_security_counter}, flags={flags}, crc={global_crc}')
        return ota_global_hdr

    def gen_ota_img_hdr(self, partition_name, img_offset, flash_offset, img_content, security_counter, img_flags):
        logging.debug(f'add ota img hdr of {partition_name}: img_offset=%x, flash_offset=%x, security_counter=%x, img_flags=%x'
            %(img_offset, flash_offset, security_counter, img_flags))
        img_offset = struct.pack('<I', img_offset)
        flash_offset = struct.pack('<I', flash_offset)
        security_counter = struct.pack('<I', security_counter)
    
        img_len = len(img_content)
        img_len = struct.pack('<I', img_len)
    
        checksum = hashlib.md5(img_content).hexdigest().encode()
        checksum = struct.pack('4s', checksum)
        flags = 0
        flags = struct.pack('<I', img_flags)
        reserved1 = 0
        reserved1 = struct.pack('<I', reserved1)
        reserved2 = 0
        reserved2 = struct.pack('<I', reserved2)
        hdr = img_len + img_offset + flash_offset + checksum + security_counter + flags + reserved1 + reserved2
        return hdr 

    def gen_ota_bin(self):
        img_num = len(self.ota_pack_idx_list)
        if (img_num == 0):
            return

        self.gen_partitions_bin()
        pack_file_name = 'ota.bin'

        offset = (IMG_HDR_LEN * img_num) + GLOBAL_HDR_LEN
        logging.debug(f'pack ota: img num={img_num}, total header len=%x' %(offset))

        img_hdr_list = []
        img_content_list = []
        for idx in self.ota_pack_idx_list:
            partition = self.partitions[idx]
            img_hdr = self.gen_ota_img_hdr(partition.partition_name, img_offset=offset, flash_offset=partition.partition_offset,
                img_content=partition.partition_buf, security_counter=0, img_flags=0)
            offset = offset + partition.partition_size
            img_hdr_list.append(img_hdr)
            img_content_list.append(partition.partition_buf)

        ota_global_hdr = self.gen_ota_global_hdr(img_num=img_num, img_content_list=img_content_list,
            img_hdr_list=img_hdr_list, global_security_counter=0)

        offset = 0
        with open(pack_file_name, 'wb+') as f:
            f.seek(offset)
            f.write(ota_global_hdr)
            offset += GLOBAL_HDR_LEN

            for img_hdr in img_hdr_list:
                f.seek(offset)
                f.write(img_hdr)
                offset += IMG_HDR_LEN

            for idx in self.ota_pack_idx_list:
                partition = self.partitions[idx]
                if (partition.bin_size > 0):
                    f.seek(offset)
                    f.write(partition.partition_buf)
                logging.debug(f'start to append partitions{idx}: offset=%x, partition_size=%x, bin_size=%x, bin_tail_size=%x'
                    %(offset, partition.partition_size, partition.bin_size, partition.bin_tail_size))
                offset += partition.partition_size
 
    def gen_app_bin(self):
        if (len(self.app_pack_idx_list) > 0):
            self.gen_partitions_bin()
            self.gen_merged_partitions_bin('apps.bin', self.app_pack_idx_list)

    def gen_manifest(self, gen, manifest_partition):
        boot_partition_name = manifest_partition.partition_name.replace("manifest", "bl2")
        p = self.find_partition_by_name(boot_partition_name)
        logging.debug(f'generate manifest, bin_name={p.bin_name}')
        manifest_name = f'{manifest_partition.partition_name}.json'
        gen.gen_manifest(p.version, p.static_addr, p.load_addr, p.bin_name, manifest_name)

    def gen_bl1_config(self):
        if self.boot_mode == 'legacyboot':
            logging.debug(f'legacyboot, skip bl1 gen')
            return

        g = Genbl1(True, self.bl1_key_type, self.bl1_privkey, self.bl1_pubkey)
        g.gen_key_desc()
        pre_partition_manifest_type = 0
        for p in self.partitions:
            if (p.partition_name == 'primary_manifest') or (p.partition_name == 'secondary_manifest'):
                self.gen_manifest(g, p)

    def gen_bl2_config(self):
        logging.info(f'TODO gen bl2')

