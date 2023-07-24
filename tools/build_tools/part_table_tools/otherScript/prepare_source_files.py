#!/usr/bin/env python
#

import argparse
import json
import sys
import re
import os

class Block_Generator:
    def __init__(self,
                raw_lines = None,
                armino_path_remote_prex = None,
                armino_path_local_prex = None,
                toolchain_path_remote_prex = None,
                toolchain_path_local_prex = None):
        self.raw_lines = raw_lines
        self.armino_path_remote_prex = armino_path_remote_prex
        self.armino_path_local_prex = armino_path_local_prex
        self.toolchain_path_remote_prex = toolchain_path_remote_prex
        self.toolchain_path_local_prex = toolchain_path_local_prex
        self.sec_kw = [' Opcodes', ' The Directory Table', ' The File Name Table', ' Line Number Statements']
        self.sec_no = 0
        self.block_gen = False
        self.block_lines = list()
        self.op_lines = list()
        self.dt_lines = list()
        self.ft_lines = list()
        self.st_lines = list()
        self.op_dicts = list()
        self.dt_dicts = list()
        self.ft_dicts = list()
        self.st_dicts = list()
        self.src_files = set()

    def sec_rows_deal(self, sec_type, sec_lines, sec_rows_kw):
        if sec_type == 'op':
            self.op_dicts = list()
            sec_dicts = self.op_dicts
        elif sec_type == 'dt':
            self.dt_dicts = list()
            sec_dicts = self.dt_dicts
        elif sec_type == 'ft':
            self.ft_dicts = list()
            sec_dicts = self.ft_dicts
        elif sec_type == 'st':
            self.st_dicts = list()
            sec_dicts = self.st_dicts
        else:
            print('Unrecognized sec_type,only support [ op | dt | ft | st ]')
            return
        for sec_line in sec_lines:
            sec_line = sec_line.strip().strip('\n')
            sec_rows = sec_line.split('\t')
            sec_dict = dict()
            for index in range(len(sec_rows_kw)):
                sec_dict[sec_rows_kw[index]] = sec_rows[index]
            sec_dicts.append(sec_dict)


    def sec_lines_deal(self, sec_lines):
        if self.sec_kw[self.sec_no] == ' Opcodes':
            # | Opecode |
            #self.op_lines = sec_lines
            #op_rows_kw = ['Opcod']
            #self.sec_rows_deal('op', self.op_lines, op_rows_kw)
            #print(self.op_dicts)
            return
        elif self.sec_kw[self.sec_no] == ' The Directory Table':
            # | Dir | Name |
            self.dt_lines = sec_lines
            dt_rows_kw = ['Dir','Name']
            self.sec_rows_deal('dt', self.dt_lines, dt_rows_kw)
            #print(self.dt_dicts)
        elif self.sec_kw[self.sec_no] == ' The File Name Table':
            # | Entry | Dir | Time | Size | Name |
            self.ft_lines = sec_lines
            ft_rows_kw = self.ft_lines[0].strip().strip('\n').split('\t')
            self.sec_rows_deal('ft', self.ft_lines[1:], ft_rows_kw)
            #print(self.ft_dicts)
        elif self.sec_kw[self.sec_no] == ' Line Number Statements':
            #self.st_lines = sec_lines
            #st_rows_kw = ['Dir','Name']
            #self.sec_rows_deal('st', self.st_lines, st_rows_kw)
            self.block_gen = True
            return
        else:
            return

    def src_files_gen(self):
        if self.block_gen:
            for ft_dict in self.ft_dicts:
                for dt_dict in self.dt_dicts:
                    if ft_dict['Dir'] == dt_dict['Dir']:
                        src_file = '{}/{}\n'.format(dt_dict['Name'], ft_dict['Name'])
                        src_file = os.path.realpath(src_file)
                        #/opt/risc-v/nds32le-elf-mculib-v5/ -> C:/Andestech/AndeSight_STD_v511/toolchains/nds32le-elf-mculib-v5/
                        #/home/chun.long/armino_temp/ -> Z:/armino_temp/
                        #re.sub(r'^{}'.format(remote_prix), r'{}'.format(loc_prix), text_line)
                        if re.search('^/local/',src_file):
                            continue
                        src_file = re.sub('^{}'.format(self.toolchain_path_remote_prex), '{}'.format(self.toolchain_path_local_prex), src_file)
                        src_file = re.sub('^{}'.format(self.armino_path_remote_prex), '{}'.format(self.armino_path_local_prex), src_file)
                        self.src_files.add(src_file)

    def block_lines_gen(self):
        sec_start = False
        sec_new = False
        for raw_line in self.raw_lines:
            if sec_start:
                if sec_new:
                    sec_lines = list()
                    sec_new = False
                if raw_line == '\n':
                    sec_start = False
                    self.sec_lines_deal(sec_lines)
                    #print(self.block_gen)
                    self.sec_no = (self.sec_no + 1) % 4
                    continue
                sec_lines.append(raw_line)
            if self.block_gen:
                self.block_lines.extend(self.op_lines)
                self.block_lines.extend(self.dt_lines)
                self.block_lines.extend(self.ft_lines)
                self.block_lines.extend(self.st_lines)
                self.src_files_gen()
                self.block_gen = False
                #print('Block has dealed')
            if(re.search('^{}'.format(self.sec_kw[self.sec_no]),raw_line)):
                sec_start = True
                sec_new = True

        return self.block_lines,self.src_files


def _prepare_source_files(src_path, dst_path):
    armino_path_remote_prex = os.getenv('ARMINO_PATH_REMOTE_PREX')
    armino_path_local_prex = os.getenv('ARMINO_PATH_LOCAL_PREX')
    toolchain_path_remote_prex = os.getenv('TOOLCHAIN_PATH_REMOTE_PREX')
    toolchain_path_local_prex = os.getenv('TOOLCHAIN_PATH_LOCAL_PREX')

    with open(src_path, 'r', encoding='utf-8') as f:
        raw_lines = f.readlines()
        block_generator = Block_Generator(raw_lines, armino_path_remote_prex, armino_path_local_prex, toolchain_path_remote_prex, toolchain_path_local_prex)
        block_lines,src_files = block_generator.block_lines_gen()

    with open(dst_path, 'w', encoding='utf-8') as f:
        for src_line in src_files:
            f.write(src_line)

def main():
    parser = argparse.ArgumentParser(description='Source File Generator')
    
    parser.add_argument(
        '--src_path',
        help='fileline keyword',
        default=None,
        dest='src_path')

    parser.add_argument(
        '--dst_path',
        help='.out file which list all file_path',
        default=None,
        dest='dst_path')

    args = parser.parse_args()

    if args.src_path:
        if os.path.exists(args.src_path) == False:
            print(f'{args.src_path} not exists')
            exit(1)
    else:
         print(f'Missing src_path')
         exit(1)

    if not args.dst_path:
        print(f'Missing dst_path')
        exit(1)

    _prepare_source_files(args.src_path, args.dst_path)

if __name__ == "__main__":
    main()