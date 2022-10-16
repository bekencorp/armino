#!/usr/bin/env python3

from calendar import c
import json
from operator import length_hint
import re
import os
import argparse
import struct
import sys
import binascii
from xts_aes import XTSAES

import gzip
import os
import struct
import zlib

from Crypto import Random
from Crypto.Cipher import AES

s_base_addr = 0
SZ_16M = 0x1000000
TOOL_NAME = 'image.py'
CRC_SIZE     =     0X02
CRC_PAKEIT   =     0X20

"""
aes加密算法
padding : PKCS7
"""

class AESUtil:

    __BLOCK_SIZE_16 = BLOCK_SIZE_16 = AES.block_size

    @staticmethod
    def encryt(string, key, iv):
        """
        加密文本
        :param string: 文本
        :param key: 密钥
        :param iv: 偏移量/初始向量
        :return: 密文
        """
        #print (string)
        
        cipher = AES.new(key, AES.MODE_CBC, iv)
        x = AESUtil.__BLOCK_SIZE_16 - (len(string) % AESUtil.__BLOCK_SIZE_16)
        # 如果最后一块不够16位需要用字符进行补全
        if x != 0:
            string = string + bytes(chr(x)*x,encoding="utf-8")

        msg = cipher.encrypt(string)
        
        # msg = base64.urlsafe_b64encode(msg).replace('=', '')
        # msg = base64.b64encode(msg)
        return msg

    @staticmethod
    def decrypt(en_str, key, iv):
        cipher = AES.new(key, AES.MODE_CBC, iv)
        # en_str += (len(en_str) % 4)*"="
        # decryptByts = base64.urlsafe_b64decode(en_str)
        # decryptByts = base64.b64decode(en_str)
        # msg = cipher.decrypt(decryptByts)

        msg = cipher.decrypt(en_str)
        padding_len = msg[len(msg)-1]
        return msg[0:-padding_len]


def crc32(bytes_obj):
    hashs = 0
    hashs = zlib.crc32(bytes_obj)
    return hashs

def fnv1a_r(oneByte,has):
    return ((oneByte ^ has)*0x1000193) & 0xFFFFFFFF

def getHashCode(data):
    has = 0x811c9dc5
    for i in range(len(data)):
        has = fnv1a_r(data[i],has)
    return has

def gethead(args,filename,ori_obj,dst_obj, compress, encrypt):
    ###  ("type_4")
    type_name = b'RBL'
    type_4 = struct.pack('4s',type_name)

    ###  ("fota_algo")
    if compress == "1" and encrypt == "2":
        fota_algo = 258#int(statinfo.st_ctime) #===============>258 = 0x0201 :02=aes256 | 01 : gzip ;02 : quicklz ;03:fastlz
    elif compress == "0" and encrypt == "2":
        fota_algo = 2
    elif compress == "1" and encrypt == "0":
        fota_algo = 256
    elif compress == "2" and encrypt == "0":
        fota_algo = 512
    elif compress == "2" and encrypt == "2":
        fota_algo = 514
    else:
        fota_algo = 0
    algo = struct.pack('<H',fota_algo)

    ###  ("ctime")
    statinfo=os.stat(filename)
    
    ctime = int(statinfo.st_mtime)
    ct = struct.pack('<I',ctime)
    ct = b'\x00'*(6-len(ct)) + ct 

    ###  ("app_part_name")
    app_part_name = args.firmware
    apn = struct.pack('16s',app_part_name.encode())

    ###  ("download_version")
    download_version  = args.firmwareversion
    dv = struct.pack('24s',download_version.encode())

    ###  ("current_version")
    current_version  = b'00010203040506070809'
    cv = struct.pack('24s',current_version)

    ###  ("body_crc32")
    body_crc32 = struct.pack('<I',crc32(dst_obj))

    ###  ("HASH_CODE")
    hash_val = struct.pack('<I',getHashCode(ori_obj))

    raw_size = len(ori_obj)
    raw_size = struct.pack('<I',raw_size)
    com_size = len(dst_obj)
    com_size = struct.pack('<I',com_size)

    head = type_4 + algo + ct + apn + dv + cv + body_crc32 + hash_val + raw_size + com_size
    head = head + struct.pack('<I',crc32(head))
    return head



def BIT(data, pos):
    return (data >> pos) & 0x1

def crc16(data : bytearray, offset , length):
    if data is None or offset < 0 or offset > len(data)- 1 and offset+length > len(data):
        return 0
    crc = 0xFFFFFFFF
    #print({data.hex()})
    for i in range(0, length):
        crc ^= data[offset + i] << 8
        for j in range(0,8):
            if (crc & 0x8000) > 0:
                crc =(crc << 1) ^ 0x8005 #for beken poly:8005
            else:
                crc = crc << 1
    return crc & 0xFFFF
# Physical address to virtual address

def p2v(addr):
    return (addr - s_base_addr)

def hex2int(str):
    return int(str, base=16) % (2**32)

def decimal2int(str):
    return int(str, base=10) % (2**32)

def crc_size(size):
    return ((size >> 5) * 34)

def crc_addr(addr):
    return crc_size(addr)

def size2int(str):
    size_str = re.findall(r"\d+", str)
    size = decimal2int(size_str[0])

    unit = re.findall(r"[k|K|m|M|g|G|b|B]+", str)
    if (unit[0] == 'b') or (unit[0] == 'B'):
        return size
    if (unit[0] == 'k') or (unit[0] == 'K'):
        return size * (1<<10)
    elif (unit[0] == 'm') or (unit[0] == 'M'):
        return size * (1<<20)
    elif (unit[0] == 'g') or (unit[0] == 'G'):
        return size * (1<<30)
    else:
        print(f'invalid size unit {unit[0]}, must be "b/B/k/K/m/M/g/G"')

def is_out_of_range(addr, size):
    if ( (addr + size) >= SZ_16M):
        return True
    return False

class image:
    def check_field(self):
        pass

    def __init__(self, idx, img_dic, base_addr):
        global s_base_addr
        self.idx = idx
        self.img_json = img_dic

        self.check_field()

        self.fill_en = False
        self.firmware_en = False
        self.value_en = False
        if "firmware" in self.img_json.keys():
            self.firmware_en = True
            self.firmware = img_dic['firmware']
            self.firmware_size = os.path.getsize(self.firmware)
            self.crc_firmware_size = crc_size(self.firmware_size)
            if not os.path.exists(self.firmware):
                print(f'image{idx} firmware %s not exists' %(self.firmware))
                exit(0)
        elif "value" in self.img_json.keys():
            self.value_en = True
            self.value = img_dic['value']
        elif "fill" in self.img_json.keys():
            self.fill_en = True
            self.fill = img_dic['fill']
        else:
            print(f'Shouls pecify "firmware", "value" or "fill"')
            exit(0)

        self.hex_en = False
        if "hex" in self.img_json.keys():
            self.hex_en = True

        self.endian_en = False
        if "endian" in self.img_json.keys():
            self.endian_en = True
            self.endian = img_dic['endian']

        if "partition" in self.img_json.keys():
            self.partition = img_dic['partition']

        if "start_addr" in self.img_json.keys():
            self.cpu_start_addr = hex2int(img_dic['start_addr'])
        else:
            self.cpu_start_addr = base_addr
        self.cpu_size = size2int(img_dic['size'])

        # Must init s_base_addr before checking address!
        if (idx == 0):
            s_base_addr = self.cpu_start_addr
            self.cpu_start_addr = 0
        else:
            if (self.cpu_start_addr <= s_base_addr):
                print(f'image{self.idx} start_addr=%x < base_addr=%x' %(self.cpu_start_addr, s_base_addr))
                exit(0)
            self.cpu_start_addr = p2v(self.cpu_start_addr)

        if is_out_of_range(self.cpu_start_addr, self.cpu_size):
            print(f'image{self.idx} start=%x size=%x is out of range' %(self.cpu_start_addr, self.cpu_size))
            exit(0)

        #if ((self.cpu_start_addr % 32) != 0):
            #print(f'image%x start_addr=%x is not 32 bytes aligned' %(self.cpu_start_addr))
            #exit(0)

        if (self.firmware_en == True) and (self.firmware_size > self.cpu_size):
            print(f'image{idx} firmware size %x > %x' %(self.firmware_size, self.cpu_size))
            exit(0)
        self.crc_start_addr = self.cpu_start_addr
        self.crc_size = self.cpu_size
        self.crc_en = False
        self.enc_start_addr = self.cpu_start_addr
        self.enc_size = self.cpu_size
        self.enc_en = False

        if ("crc" in img_dic):
            if (img_dic['crc'] == 'y') or (img_dic['crc'] == 'Y'):
                self.crc_start_addr = crc_addr(self.cpu_start_addr)
                self.crc_size = crc_size(self.cpu_size)
                self.crc_en = True

        if is_out_of_range(self.crc_start_addr, self.crc_size):
            print(f'image{self.idx} crc is out of range')
            exit(0)

        #print(f'image%x cpu_start=%x size=%x, crc_start=%x, size=%x, enc_start=%x enc_end=%x'
        #        %(self.idx, self.cpu_start_addr, self.cpu_size, self.crc_start_addr, self.crc_size, self.enc_start_addr, self.enc_size))

    def add_crc(self):
        if (self.firmware_en):
            self.raw_buf = bytearray(self.firmware_size)
            self.crc_buf = bytearray(self.crc_firmware_size)
            with open(self.firmware, 'rb') as f:
                #print(f'TODO add CRC16')
                self.crc_buf =  f.read()
                #print(f'=================> len=%u' %(len(self.raw_buf)))
        elif (self.value_en):
            if self.hex_en == True:
                self.crc_buf = bytes.fromhex(self.value)
                if (self.endian_en == True) and (self.endian == "l"):
                    buf_bytes_array = bytearray(self.crc_buf)
                    buf_bytes_array.reverse()
                    self.crc_buf = bytes(buf_bytes_array)
            else:
                self.crc_buf = bytearray(self.value, encoding='utf-8')
            fill_value = 0
            pad_len = self.crc_size - len(self.value)
            for i in range(pad_len):
                self.crc_buf.append(fill_value)
        else:
            fill_value = int(self.fill)
            self.crc_buf = bytearray()
            for i in range(self.crc_size):
                self.crc_buf.append(fill_value)

class images:

    def __init__(self, args):
        if (not args.json) or (not args.outfile):
            print('usage: {TOOL_NAME} -j json_file -o outfile')
            exit(1)

        self.imgs = []
        self.output_file_name = args.outfile
        self.json_file_name = args.json 
        if not os.path.exists(args.json):
            print(f'JSON configuration file {args.json} not exists')
            exit(0)

        with open(args.json, 'r') as self.json_file:
            self.json_data = json.load(self.json_file)
        self.check_json_data()

    def check_json_data(self):
        if ("images" not in self.json_data):
            print('json does not contain field "images"!')
            exit(0)

        self.imgs_cnt = len(self.json_data['images'])
        if (self.imgs_cnt == 0):
                print(f'images of json does not contain any item!')
                exit(0)

        pre_field_addr = 0
        for idx in range(self.imgs_cnt):
            img = image(idx, self.json_data['images'][idx], pre_field_addr)
            pre_field_addr = img.crc_start_addr + img.crc_size
            self.imgs.append(img)

        for idx in range(self.imgs_cnt):
            if (idx == 0):
                continue

            pre_crc_start_addr = self.imgs[idx - 1].crc_start_addr
            pre_crc_size = self.imgs[idx - 1].crc_size
            crc_start_addr = self.imgs[idx].crc_start_addr
            if ( (pre_crc_start_addr + pre_crc_size) > crc_start_addr ):
                print(f'image%x start=%x size=%x overlapped with image%x start=%x'
                        %(idx-1, pre_crc_start_addr, pre_crc_size, idx, crc_start_addr))
                exit(0)
            #check_addr(self.imgs[idx - 1], self.imgs[idx])

    def test(self):
        data = 1
        content = data.to_bytes(1, "big")
        f = open('t.bin', "wb+")
        for i in range(4096):
            f.write(content)

        f.seek(0x10240)
        f.write(content)
        f.flush()
        f.close()

    def merge_image(self):
        f = open(self.output_file_name, 'wb+')
        for idx in range(self.imgs_cnt):
            img = self.imgs[idx]
            img.add_crc()
            #print(f'merge image{idx} start=%x' %(img.crc_start_addr))
            f.seek(img.crc_start_addr)
            f.write(img.crc_buf)

        f.flush()
        f.close()

def create(args):
    if (not args.size) or (not args.outfile):
        print(f'usage: {TOOL_NAME} create -o outfile -s size [-f fill]')
        sys.exit(0)

    fill_value = 0
    if args.fill:
        fill_value = int(args.fill)

    with open(args.outfile, 'wb+') as f:
        for i in range(int(args.size)):
            a = struct.pack('B', fill_value)
            f.write(a)

def revert(args):
    if (not args.infile) or (not args.outfile):
        print(f'usage: {TOOL_NAME} revert -i infile -o outfile')
        exit(1)

    bin_size = os.path.getsize(args.infile)
    if (bin_size % 4) != 0:
        print(f'{args.infile} is not 4 bytes aligned')
        exit(0)
    loop_cnt = bin_size >> 2
    addr = 0x0
    with open(args.outfile, 'wt+') as of:
        with open(args.infile, 'rb') as f:
            for i in range(loop_cnt):
                bin_buf = f.read(4)
                hex_buf = bin_buf.hex()
                hex_int = int(hex_buf, base=16)

                byte0 = (hex_int>>24) & 0xFF
                byte1 = (hex_int>>16) & 0xFF
                byte2 = (hex_int>>8) & 0xFF
                byte3 = (hex_int>>0) & 0xFF
                hex_int_le = (byte3 << 24) + (byte2 << 16) + (byte1 << 8) + byte0

                hex_int_revert = 0xFFFFFFFF - hex_int_le
                hex_str = str(hex_int_revert)
                revert_buf = ''.join(['@{:0>8x} '.format(addr), '%08x' %(hex_int_revert), '\n'])
                #print(f'{hex_buf} {revert_buf}')
                of.write(revert_buf)
                addr += 1
                revert_buf = ''.join(['@{:0>8x} '.format(addr), '%08x' %(hex_int_revert), '\n'])
                of.write(revert_buf)
                addr += 1

def add_crc16(args):
    if (not args.infile) or (not args.outfile):
        print(f'usage: {TOOL_NAME} -i infile -o outfile')
        exit(0)

    if not os.path.exists(args.infile):
        print(f'infile {args.infile} not exists')
        exit(0)
    
    b_success = False
    in_file_size = os.path.getsize(args.infile)
    num_of_crc = (int)(in_file_size / CRC_PAKEIT)
    last_block_len = in_file_size % 32
    with open(args.outfile, 'wb+') as of:
        with open(args.infile, 'rb') as f:
            for i in range(num_of_crc):
                raw = f.read(32)
                crc = crc16(raw,0,CRC_PAKEIT)
                of.write(raw)
                crc = struct.pack(">H",crc)
                of.write(crc)
            if last_block_len == 0:
                b_success = True
            if (last_block_len != 0):
                pad_buf = []
                raw = f.read(last_block_len)
                pad_len = 32 - last_block_len
                for c in raw:
                    pad_buf.append(c)
                for i in range(pad_len):
                    pad_buf.append(0xff)
                pad_buf_bytes = bytes(pad_buf)
                crc = crc16(pad_buf_bytes,0,CRC_PAKEIT)
                of.write(pad_buf_bytes)
                crc = struct.pack(">H",crc)
                of.write(crc)
                b_success = True
    print(f'crc success {args.outfile}') if b_success == True else print(f'crc fail {args.outfile}')

def asii(args):
    if (not args.infile) or (not args.outfile):
        print(f'usage: {TOOL_NAME} -i infile -o outfile')
        exit(0)

    if (not args.size):
        size = int(1)
    else:
        size = int(args.size)

    print(f'generate asii stream, size={size}')
    infile_len = os.path.getsize(args.infile)
    with open(args.outfile, 'wt+') as of:
        with open(args.infile, 'rb') as f:
            j = 0
            a_byte_line = ''
            for i in range(infile_len):
                a_byte = f.read(1)
                a_byte_hex = a_byte.hex()
                a_byte_int = int(a_byte_hex, base=16)
                a_byte_line = ''.join(['%02x%s' %(a_byte_int, a_byte_line)])
                if (j == (size - 1)):
                    j = 0
                    a_byte_line = ''.join(['%s' %(a_byte_line), '\n'])
                    of.write(a_byte_line)
                    a_byte_line = ''
                else:
                    j = j + 1
            if (j != 0):
                a_byte_line = ''.join(['%s' %(a_byte_int), '\n'])
                of.write(a_byte_line)

def dump(args):
    if (not args.infile) or (not args.outfile):
        print(f'usage: {TOOL_NAME} -i infile -o outfile')
        exit(0)

    with open(args.outfile, "wt+") as of:
        with open(args.infile, "rb") as f:
            file_size = os.path.getsize(args.infile)
            for i in range(file_size):
                byte = f.read(1)
                hex_byte = byte.hex()
                of.write(hex_byte)

def replace(args):
    if (not args.infile) or (not args.outfile) or (not args.rfile) or (not args.address):
        print(f'usage: {TOOL_NAME} -i infile -o outfile -r replacefile -a offset')
        exit(0)

    print(f'in={args.infile}, out={args.outfile}, rfile={args.rfile}, address={args.address}')
    with open(args.outfile, 'wb+') as outfile:
        with open(args.infile, 'rb') as infile:
            buf = infile.read()
            outfile.write(buf)
            with open(args.rfile, 'rb') as rfile:
                buf = rfile.read()
                outfile.seek(0x800) #TODO fix me
                outfile.write(buf)

def modify(args):
    if (not args.infile) or (not args.address) or (not args.outfile):
        print(f'usage: {TOOL_NAME} -i infile -o outfile -a offset')
        exit(0)

    print(f'in={args.infile}, out={args.outfile}, address={args.address}')
    file_size = os.path.getsize(args.infile)
    first_part = 0x10
    second_part = file_size 
    with open(args.outfile, 'wb+') as outfile:
        with open(args.infile, 'rb') as infile:
            first_part = infile.read(0x10)
            infile.seek(0x11)
            second_part = infile.read(file_size - 0x11)
            outfile.write(first_part)
            outfile.write(b'\x21')
            outfile.write(second_part)
            #infile.seek(0x10) #TODO fix me
            #infile.write(b'\x21')

def check_sha384(args):
    if (not args.infile):
        print(f'usage: {TOOL_NAME} -i infile')
        exit(1)

    with open(args.infile, 'r') as json_file:
        json_data = json.load(json_file)
        if (json_data['mnft_sig_cfg']['pubkey_hash_sch'] == 'SHA384'):
            json_data['mnft_sig_cfg']['pubkey_hash_sch'] = 'SHA1'
        else:
            exit(0)

    
    with open(args.infile, 'w') as json_file:
        json.dump(json_data, json_file)
        exit(1)

def read_hex_string(name):
    try:
        hex_string = input('{name}: '.format(name=name))
        hex_string = binascii.unhexlify(hex_string)
    except binascii.Error:
        sys.exit('{name} should be hex string'.format(name=name))

    return hex_string

def aes128(args):
    if (not args.infile) or (not args.outfile):
        print(f'usage: {TOOL_NAME} -i infile -o outfile')
        exit(0)

    if not os.path.exists(args.infile):
        print(f'infile {args.infile} not exists')
        exit(0)

    # key = read_hex_string('key')
    # if len(key) != 64:
    #     sys.exit('key should be 64-byte')

    # tweak = read_hex_string('tweak')
    # if len(tweak) != 16:
    #     sys.exit('tweak should be 16-byte')

    # text = read_hex_string(TEXT_TYPES[mode])
    # if len(text) < 16:
    #     sys.exit('{text_type} should be greater than or equal to 16-byte'.format(text_type=TEXT_TYPES[mode]))

    tweak = [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]

    key = [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]

    # key = [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    # 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    # 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    # 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]

    inB = [0x34,0x20,0x22,0xF3,0x00,0x02,0x84,0x63,0x05,0xA0,0x00,0x6F,0x7C,0xA0,0x27,0xF3,
    0x00, 0x17, 0xE7, 0x93, 0x7C, 0xA7, 0x90, 0x73, 0x7C, 0xA0, 0x27, 0xF3, 0x00, 0x27, 0xE7, 0x93]

    xts_aes = XTSAES(key, tweak)

    #encryptor = xts_aes.encrypt if mode == 'encryption' else xts_aes.decrypt
    encryptor = xts_aes.encrypt
    ciphertext = encryptor(inB)

    print('{ciphertext_type}: {ciphertext}'.format(ciphertext_type="xts-aes-encrypt", ciphertext=binascii.hexlify(ciphertext).decode()))

def packager(args):
    if (not args.infile) or (not args.outfile):
        print(f'usage: {TOOL_NAME} -i infile -c compress -e encrypt -k keywords -iv iv -fn firmware -fv firmwareversion -o output.rbl')
        exit(0)

    if not os.path.exists(args.infile):
        print(f'infile {args.infile} not exists')
        exit(0)

    gzip_header = args.gHeader + "/gzipHeader.bin"
    if not os.path.exists(gzip_header):
        print(f'"gzipHeader.bin" not exists')
        exit(0)
    
    b_new_chip = 0
    key = args.keywords.encode()
    iv = args.iv.encode()
    compress = args.compress
    aes = args.encrypt

    config = args.gHeader + "/ota_rbl.config"
    if os.path.exists(os.getenv("ARMINO_PATH")+"/projects/"+os.getenv("PROJECT")+"/config/ota_rbl.config") and os.path.exists(os.getenv("ARMINO_PATH")+"/projects/"+os.getenv("PROJECT")+"/config/configurationab.json"):
        b_new_chip = 1
        config = os.getenv("ARMINO_PATH")+"/projects/"+os.getenv("PROJECT")+"/config/ota_rbl.config"
    #print("============>>>>",config)
    if os.path.exists(config):
        with open(config,"rb") as fr:
            json_dict = json.load(fr)
            js = json.dumps(json_dict, indent=4)

            if "key" in json_dict:
                key = json_dict["key"].encode()

            if "iv" in json_dict:
                iv = json_dict["iv"].encode()

            if "gzip" in json_dict:
                compress = json_dict["gzip"]

            if "aes" in json_dict:
                aes = json_dict["aes"]

    with open(gzip_header,"rb") as f:
        gzipheader = f.read()
    #print ("gzip文件头",binascii.hexlify(gzipheader).decode())
    #print ("gzip文件头长度",len(gzipheader))

    filename = args.infile
    with open (filename,"rb") as f:
        bytes_obj = f.read()
        print("length before compress and encrypt：",len(bytes_obj))
        ### gizp 压缩
        dest_obj = bytes_obj
        if compress == "1":
            tmp_gizp = gzip.compress(bytes_obj,compresslevel=6)
            dest_obj = gzipheader + tmp_gizp[10:]
            print("0 compress ",compress)
        elif (compress == "2"):
            print("1 compress ",compress)
        else:
            print("2 compress ",compress)
        ### aes 加密
        if aes == "2":
            dest_obj = AESUtil.encryt(dest_obj, key, iv)
            print("0 aes ",aes)
        elif (aes == "1"):
            print("1 aes ",aes)
        else:
            print("2 aes ",aes)
        my_head = gethead(args,filename,bytes_obj,dest_obj, compress, aes)
        #print(binascii.hexlify(my_head).decode())
        #print (my_head)
        print("length after compress and encrypt：",len(dest_obj))

    with open(args.outfile,"wb") as f:
        if b_new_chip == 1:
            f.write(dest_obj)

            config = "configurationab.json"
            with open(config,"rb") as fj:
                json_dict = json.load(fj)
                if "section" in json_dict:
                    section = json_dict["section"]
                    size = section[1]["size"]
                    #print("============size：>>>>",size)
                    iSize = (size2int(size)) - 96
                    #print("============iSize：>>>>",iSize)
                    #print("============dest_obj：>>>>",len(dest_obj))                    
                    for i in range(iSize - len(dest_obj)):
                        a = struct.pack('B',255)
                        f.write(a)
            f.write(my_head)
        else:
            f.write(my_head)
            f.write(dest_obj)
        

def parse_args():
    parser = argparse.ArgumentParser("Beken image tool.")
    parser.add_argument("cmd",
            type=str, choices=['create', 'crc16', 'merge', 'revert', 'replace', 'encrypt', 'asii', 'dump', 
            'modify', 'check_sha384','aes128','packager'],
            help="image commands")

    parser.add_argument("-i", "--infile", default="app_pack.bin", help="input file name")
    parser.add_argument("-o", "--outfile",default="app_pack.rbl", help="output file name")
    parser.add_argument("-r", "--rfile", help="replace file name")
    parser.add_argument("-j", "--json", help="json configuration")
    parser.add_argument("-s", "--size", help="bin size")
    parser.add_argument("-f", "--fill", help="fill value")
    parser.add_argument("-a", "--address", help="address")
    parser.add_argument("-c", "--compress", default = "1",help="compress")
    parser.add_argument("-e", "--encrypt", default = "2",help="encrypt")
    parser.add_argument("-k", "--keywords", default = "0123456789ABCDEF0123456789ABCDEF",help="keywords")
    parser.add_argument("-iv", "--iv", default = "0123456789ABCDEF",help="iv")
    parser.add_argument("-fn", "--firmware",default = "app", help="firmware name")
    parser.add_argument("-fv", "--firmwareversion", default = "2", help="firmware version")
    parser.add_argument('-g', '--gHeader', type=str, help='', default="gzipHeader.bin")

    try:
        args = parser.parse_args()

    except:
        print(f'unknown command')
        sys.exit(0)

    if args.cmd == 'create':
        create(args)
    elif args.cmd == 'crc16':
        add_crc16(args)
    elif args.cmd == 'merge':
        img = images(args)
        img.merge_image()
    elif args.cmd == 'revert':
        revert(args)
    elif args.cmd == 'asii':
        asii(args)
    elif args.cmd == 'dump':
        dump(args)
    elif args.cmd == 'replace':
        replace(args)
    elif args.cmd == 'modify':
        modify(args)
    elif args.cmd == 'check_sha384':
        check_sha384(args)
    elif args.cmd == 'aes128':
        aes128(args)
    elif args.cmd == 'packager':
        packager(args)
    else:
        print(f'unknow command')
        sys.exit(0)

def main():
    parse_args()

if __name__ == "__main__":
    main()
