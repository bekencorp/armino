#!/usr/bin/env python3

import argparse
import sys
import re
import os
import csv

KEY_REG_ADDR = "x3"
KEY_REG_VAR = "x4"
KEY_REG_DEFAULT = "x5"
KEY_REG_COMMENT = "x7"

header_block =\
"\
// Copyright 2020-2021 Beken\n\
//\n\
// Licensed under the Apache License, Version 2.0 (the \"License\");\n\
// you may not use this file except in compliance with the License.\n\
// You may obtain a copy of the License at\n\
//\n\
//     http://www.apache.org/licenses/LICENSE-2.0\n\
//\n\
// Unless required by applicable law or agreed to in writing, software\n\
// distributed under the License is distributed on an \"AS IS\" BASIS,\n\
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n\
// See the License for the specific language governing permissions and\n\
// limitations under the License.\n\
\n\
#pragma once\n\
\n\
#ifdef __cplusplus\n\
extern \"C\" {\n\
#endif\n\
\n\
"

tailer_block =\
"\n\
#ifdef __cplusplus\n\
}\n\
#endif\n\
"

class DumpGen:
	def __init__(self, name=None):
		self.name = name

	def gen_struct_dump(self):
		print('Generate struct dump')

class Bits:
	def __init__(self, addr, name=None, start=None, end=None, default_value=None, line=0):
		self.addr = addr
		self.name = name.lower()
		self.start = start
		self.end = end
		self.default_value = default_value
		self.line = line

	def set_default_value(self, default_value=0):
		self.default_value = default_value

	def set_comments(self, comments=None):
		self.comments = comments

	def dump(self):
		print(f'line={self.line}, addr={self.addr}, name={self.name}, start={self.start}, end={self.end}, default={self.default_value}')

class Reg:
	def __init__(self):
		self.bits_list = []
		self.indent = 1

	def add_bits(self, bits):
		self.bits_list.insert(0, bits)

	def gen_reg_code(self):
		print(f'Generate reg code')

	def is_int(self):
		if len(self.bits_list) == 1:
			return True

		return False

	def is_reserved(self):
		if len(self.bits_list) == 1:
			bits = self.bits_list[0]
			if (bits.name == "reserved"):
				return True

		return False

	def get_reserved_num(self):
		reserved_num = 0
		for bits in self.bits_list:
			if (bits.name == "reserved"):
				reserved_num = reserved_num + 1

		return reserved_num

	def adjust_bits_reserved(self):
		reserved_num_tmp = 0;
		count_tmp = self.get_reserved_num()
		#print(count_tmp)
		for bits in self.bits_list:
			if (bits.name == "reserved" and count_tmp > 1):
				bits.name = bits.name + str(reserved_num_tmp)
				reserved_num_tmp = reserved_num_tmp + 1

	def gen_int_struct(self, struct_file):
		bits = self.bits_list[0]
		if (bits.comments.strip() != '' and bits.comments != "NC"):
			struct_file.write(f'\t/*< {bits.comments} */\n')
		addr_str = hex(bits.addr)
		code_line =  '\tuint32_t ' + bits.name + '; /* ' + addr_str + ' */\n\n'
		struct_file.write(code_line)

	def gen_bits_struct(self, struct_file):
		bit0 = self.bits_list[0]
		addr_str = f'{hex(bit0.addr)}'
		code_line =  '\t/* ' + addr_str + ' */\n'
		struct_file.write(code_line)
		code_line = '\tunion {\n'
		struct_file.write(code_line)
		code_line = '\t\tstruct {\n'
		struct_file.write(code_line)

		for bits in self.bits_list:
			width = bits.start - bits.end + 1
			width_str = f'{width}'
			if (bits.comments.strip() != '' and bits.comments != "NC"):
				struct_file.write(f'\t\t\t/*< {bits.comments} */\n')
			code_line =  '\t\t\tuint32_t ' + bits.name + ': ' + width_str + ';\n'
			struct_file.write(code_line)
		code_line = '\t\t};\n'
		struct_file.write(code_line)
		code_line = '\t\tuint32_t v;\n'
		struct_file.write(code_line)
		code_line = '\t} TODO_union_name;\n\n'
		struct_file.write(code_line)

	def gen_struct(self, struct_file):
		if self.is_int():
			self.gen_int_struct(struct_file)
		else:
			self.gen_bits_struct(struct_file)

class StructGen:

	def __init__(self, name=None):
		self.csv_file_name = name
		self.csv_dict_list = []
		self.reg_list = []
		self.pre_reg_addr = 0
		self.pre_reg = Reg()
		self.cur_line = 0
		file_base_name = name.replace('.csv', "")
		self.struct_file_name = file_base_name + '_struct.h'
		print(f'File base = {self.struct_file_name}')
		self.struct_file = None

		with open(name, 'r') as csvfile:
			csv_dict = csv.DictReader(csvfile)
			for line in csv_dict:
				self.csv_dict_list.append(line)
		#self.dump_csv_dict_list()

	def dump_csv_dict_list(self):
		for line in self.csv_dict_list:
			print(line['x3'])

	def get_bits_name(self, dict_line):
		if KEY_REG_VAR not in dict_line:
			return None

		bits_var = dict_line[KEY_REG_VAR]
		bits_var_list = bits_var.split('[')
		if len(bits_var_list) > 1:
			bits_var = bits_var_list[0]

		return bits_var.lower()

	def get_bits_default(self, dict_line):
		if KEY_REG_DEFAULT not in dict_line:
			return None

		bits_default = dict_line[KEY_REG_DEFAULT]
		return bits_default

	def get_bits_comments(self, dict_line):
		if KEY_REG_COMMENT not in dict_line:
			return None

		return dict_line[KEY_REG_COMMENT]

	def get_addr_dict(self, dict_line):
		addr_dict = {"addr": None, "start": None, "end": None}

		if KEY_REG_ADDR not in dict_line:
			print(f'line({self.cur_line}: key {KEY_REG} doesnt exist')
			return addr_dict

		reg_addr = dict_line[KEY_REG_ADDR]
		reg_addr = reg_addr.replace('[', ':')
		reg_addr = reg_addr.replace(']', '')
		reg_addr = reg_addr.replace('\r', '')
		reg_addr = reg_addr.replace('\n', '')
		reg_addr_split_list = reg_addr.split(':')

		if reg_addr_split_list == None:
			print(f'line({self.cur_line}: addr is None')
			return addr_dict

		if reg_addr_split_list == []:
			print(f'line({self.cur_line}: addr is None')
			return addr_dict

		if (reg_addr_split_list[0] == ''):
			print(f'line({self.cur_line}): empty line, ignore')
			return addr_dict

		addr_dict['addr'] = int(reg_addr_split_list[0], 16)
		if (len(reg_addr_split_list) == 1):
			addr_dict['start'] = 31
			addr_dict['end'] = 0
		elif (len(reg_addr_split_list) == 2):
			addr_dict['start'] = int(reg_addr_split_list[1], 10)
			addr_dict['end'] = int(reg_addr_split_list[1], 10)
		else:
			addr_dict['start'] = int(reg_addr_split_list[1], 10)
			addr_dict['end'] = int(reg_addr_split_list[2], 10)

		return addr_dict

	def add_reserved_reg(self, start_addr, end_addr):
		addr_tmp = start_addr + 1
		while (end_addr > addr_tmp):
			bits = Bits(addr_tmp, "reserved", 31, 0)
			reg = Reg()
			reg.add_bits(bits)
			self.pre_reg == None
			self.reg_list.append(reg)
			self.pre_reg_addr = addr_tmp
			addr_tmp = addr_tmp + 1

	def is_dict_line_empty(self, dict_line):
		if dict_line is None:
			return True

		if KEY_REG_ADDR not in dict_line:
			return True

		if dict_line[KEY_REG_ADDR] == None:
			return True

		if dict_line[KEY_REG_ADDR] == '':
			return True

		return False

	def parse_csv_dict_line(self, dict_line):

		if self.is_dict_line_empty(dict_line):
			print(f'line({self.cur_line}): empty dict_line, ignore')
			return

		reg_name = self.get_bits_name(dict_line)
		reg_default_value = self.get_bits_default(dict_line)
		reg_addr_dict = self.get_addr_dict(dict_line)
		reg_comments = self.get_bits_comments(dict_line)

		if (reg_addr_dict == {}):
			print(f'line({self.cur_line}): reg address is empty, ignore')
			return

		bits = Bits(reg_addr_dict['addr'], reg_name, reg_addr_dict['start'], reg_addr_dict['end'], reg_default_value, self.cur_line)
		bits.set_comments(reg_comments)
		#bits.dump()

		if (self.pre_reg == None):
			reg = Reg()
		else:
			reg = self.pre_reg

		reg.add_bits(bits)

		if (reg_addr_dict['end'] == 0):
			reg.adjust_bits_reserved()
			self.pre_reg = None
			if (reg_addr_dict['addr'] - self.pre_reg_addr > 1):
				self.add_reserved_reg(self.pre_reg_addr, reg_addr_dict['addr'])
			self.reg_list.append(reg)
			self.pre_reg_addr = reg_addr_dict['addr']
		else:
			self.pre_reg = reg

	def parse_regs(self):
		self.cur_line = 1
		for line in self.csv_dict_list:
			self.cur_line = self.cur_line + 1
			self.parse_csv_dict_line(line)

	def dump_struct(self):
		print(f'Dump struct')

	def gen_struct_header_block(self):
		self.struct_file.write(header_block)

	def gen_struct_tailer_block(self):
		self.struct_file.write(tailer_block)

	def gen_struct(self):
		self.struct_file = open(self.struct_file_name, 'w')
		self.gen_struct_header_block()
		code_line = "typedef volatile struct {\n"
		self.struct_file.write(code_line)
		reserved_start = 0
		reserved_end = 0
		reserved_num = 0;
		reserved_index = 0
		for reg in self.reg_list:
			bits0 = reg.bits_list[0]
			if reg.is_reserved():
				reserved_start = bits0.addr
				reserved_num = reserved_num + 1
			else:
				reserved_start = reserved_start - reserved_num
				reserved_end = bits0.addr
				if reserved_num == 1:
					self.struct_file.write("\tuint32_t reserved" + str(reserved_index) + ";\n\n")
					reserved_index = reserved_index + 1
				elif reserved_num > 1:
					code_line = "\tuint32_t reserved" + str(reserved_index) + "[" + hex(reserved_end) + " - " + hex(reserved_start) + " - 1];\n\n"
					self.struct_file.write(code_line)
					reserved_index = reserved_index + 1
				reserved_num = 0
				reserved_start = 0
				reserved_end = 0
				reg.gen_struct(self.struct_file)
		code_line = "} TODO_hw_t;\n"
		self.struct_file.write(code_line)
		self.gen_struct_tailer_block()
		self.struct_file.close()

def main():
	parser = argparse.ArgumentParser(description='armino_gen - a tool to generate code')

	parser.add_argument(
		'--symbol_file', help='Struct symbol file: xx_reg_struct.S',
		default=None,
		dest='symbol_file')

	parser.add_argument(
		'--reg_csv_file', help='Register CSV file: xx_reg.csv',
		default=None,
		dest='reg_csv_file')

	args = parser.parse_args()

	if args.symbol_file:
		dump_gen = DumpGen(args.symbol_file)
		dump_gen.gen_struct_dump()

	if args.reg_csv_file:
		struct_gen = StructGen(args.reg_csv_file)
		struct_gen.parse_regs()
		struct_gen.gen_struct()

if __name__ == "__main__":
	main()
