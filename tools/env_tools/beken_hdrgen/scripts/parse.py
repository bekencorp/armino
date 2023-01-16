#!/usr/bin/env python3

import logging
import openpyxl
from openpyxl import load_workbook
import sys
import os

class Bit:
	def is_bit_offset_valid(self, offset):
		if (offset >= 0) and (offset < 32):
			return True
		else:
			return False

	def __init__(self, reg, seq, sub_seq, sig_name, start, end, default, opmode, comments=""):
		self.reg = reg
		self.seq = seq
		self.sub_seq = sub_seq
		self.sig_name = sig_name
		self.start = start
		self.end = end
		self.default = default
		self.opmode = opmode
		self.comments = comments

		logging.debug(f'add bit: reg={reg}, seq={seq}, sub_seq={sub_seq}, sig_name={sig_name}, start={start}, end={end}, default={default}, opmode={opmode}')
		if (self.is_bit_offset_valid(start) == False) or (self.is_bit_offset_valid(end) == False):
			logging.error(f'invalid bit offset {start} or {end}')
			exit(1)

	def get_reg_name(self):
		if self.sub_seq != None:
			return self.sub_seq

		logging.error(f'excel sub_sequence is empty, find a good name first')
		exit(1)

	def get_mask(self):
		return ((1 << (self.end - self.start + 1)) - 1)

	def dump(self):
		logging.info(f'%-6s %-8s %-8s %-32s [%2d:%2d] %-10s %-3s'
			%(self.reg, self.seq, self.sub_seq, self.sig_name, self.start, self.end, self.default, self.opmode))

class Reg:
	def __init__(self, name, offset, is_regs=False, reg_cnt=1):
		logging.debug(f'reg name={name}, offset={offset}, is_regs={is_regs}')
		self.name = name
		self.offset = offset
		self.is_regs = is_regs
		self.reg_cnt = reg_cnt

		self.bits = []

	def set_name(self, name):
		self.name = name

	def is_overlapped(self, bita, bitb):
		if (bita.end < bitb.start) or (bitb.end < bita.start):
			return False
		else:
			return True

	def exit_if_overlapped(self, bita, bitb):
		if self.is_overlapped(bita, bitb):
			logging.error(f'bits overlapped')
			exit(1)

	def check_overlapped(self, bit):
		bits_len = len(self.bits)
		if bits_len == 0:
			return

		if (bits_len == 1):
			self.exit_if_overlapped(self.bits[0], bit)
			return

		if (bits_len >= 2):
			if (self.is_ascend):
				self.exit_if_overlapped(self.bits[bits_len - 1], bit)
			else:
				self.exit_if_overlapped(self.bits[0], bit)

	def is_smaller(self, bita, bitb):
		if (bita.end < bitb.start):
			return True
		else:
			return False

	def resolved_ascend(self, bit):
		if len(self.bits) < 1:
			return

		if len(self.bits) == 1:
			if self.is_smaller(self.bits[0], bit):
				self.is_ascend = True
			else:
				self.is_ascend = False
		else:
			if self.is_smaller(self.bits[0], bit) != self.is_ascend:
				logging.error(f'bit order conflict')
				exit(1)

	def ascend_add_bit(self, bit):
		bits_len = len(self.bits)
		if bits_len == 0:
			self.bits.append(bit)
			return

		if self.is_ascend:
			last_bit = self.bits[bits_len - 1]
			if ((last_bit.end + 1) == bit.start):
				self.bits.append(bit)
			elif ((last_bit.end + 1) < bit.start):
				start = self.bits[bits_len - 1].end + 1
				end = bit.start - 1
				sig_name = f'reserved_bit_{start}_{end}'
				reserved_bit = Bit(reg=bit.reg, seq=bit.seq, sub_seq=bit.sub_seq, sig_name=sig_name, start=start,
				end=end, default='0', opmode='NA', comments='reserved')
				self.bits.append(reserved_bit)
				self.bits.append(bit)
			else:
				logging.error('should never get here')
				exit(1)
		else:
			last_bit = self.bits[0]
			if ((bit.end + 1) == last_bit.start):
				self.bits.insert(0, bit)
			elif ((bit.end + 1) < last_bit.start):
				start = bit.end + 1
				end = last_bit.start - 1
				sig_name = f'reserved_bit_{start}_{end}'
				reserved_bit = Bit(reg=bit.reg, seq=bit.seq, sub_seq=bit.sub_seq, sig_name=sig_name, start=start,
				end=end, default='0', opmode='NA', comments='reserved')
				self.bits.insert(0, reserved_bit)
				self.bits.insert(0, bit)
			else:
				logging.error('should never get here')
				exit(1)

	def add_hdr_tail_reserved(self):
		if len(self.bits) == 0:
			logging.error(f'reg has no bits, no need add hdr/tail reserved bits')
			exit(1)

		bit = self.bits[0]
		if (bit.start != 0):
			start = 0
			end = bit.start - 1
			sig_name = f'reserved_bit_{start}_{end}'
			reserved_bit = Bit(reg=bit.reg, seq=bit.seq, sub_seq=bit.sub_seq, sig_name=sig_name, start=start,
				end=end, default='0', opmode='NA', comments='reserved')
			self.bits.insert(0, reserved_bit)
			logging.debug(f'add header reserved bit {sig_name}')

		bit = self.bits[len(self.bits) - 1]
		if (bit.end != 31):
			start = bit.end + 1
			end = 31
			sig_name = f'reserved_bit_{start}_{end}'
			reserved_bit = Bit(reg=bit.reg, seq=bit.seq, sub_seq=bit.sub_seq, sig_name=sig_name, start=start,
				end=end, default='0', opmode='NA', comments='reserved')
			self.bits.append(reserved_bit)
			logging.debug(f'add tail reserved bit {sig_name}')

	def rename_sig_name(self, sig_name):
		for bit in self.bits:
			if (sig_name == bit.sig_name):
				bit.sig_name = f'{bit.sig_name}_{bit.start}_{bit.end}'

	def completed_a_reg(self):
		self.add_hdr_tail_reserved()
		self.rename_sig_name('nc')
		self.rename_sig_name('reserved')

	def add_bit(self, bit):
		self.resolved_ascend(bit)
		self.check_overlapped(bit)
		self.ascend_add_bit(bit)

	def dump(self):
		logging.info(f'\tname={self.name}, offset=%x' %(self.offset))
		for bit in self.bits:
			bit.dump()

class Regparse:
	def __init__(self, infile='BK7236_ADDR_Mapping_s.xlsm', module='SYS', start_reg=0, end_reg=-1, prefix='', debug=True, reghex=True):
		logging.basicConfig()
		if (debug == True):
			logging.getLogger().setLevel(logging.DEBUG)
		else:
			logging.getLogger().setLevel(logging.INFO)

		logging.debug(f'mapping file is {infile}, module={module}, start_reg={start_reg}, end_reg={end_reg}')
		self.infile = infile
		self.module = module
		self.start_reg = start_reg
		self.end_reg = end_reg
		self.debug = debug
		self.regs = []
		self.reghex = reghex
		self.prefix = prefix

		self.modules = []

	def row_value(self, row, idx):
		return row[idx].value

	def parse_base_addr(self):
		if len(self.rows) < 1:
			logging.debug(f'failed to find base address in 1st line, rows < 1')
			return
		self.base_addr = self.row_value(self.rows[0], 3)
		logging.debug(f'base address={self.base_addr}')

	def row_find(self, row, str):
		idx = 0
		str_len = len(str)
		for item in row:
			if item.value[0:str_len] == str:
				return idx
			idx = idx + 1
		return -1

	def parse_column_format(self):
		if len(self.rows) < 2:
			logging.debug(f'failed to find format line, rows < 2')
			return

		format_row = self.rows[1]
		self.seq_idx = self.row_find(format_row, '序号')
		self.subseq_idx = self.row_find(format_row, '分序号')
		self.addr_idx = self.row_find(format_row, '寄存器地址')
		self.sig_name_idx = self.row_find(format_row, '控制信号名')
		self.default_value_idx = self.row_find(format_row, '默认值')
		self.opmode_idx = self.row_find(format_row, '操作模式')
		self.comments_idx = self.row_find(format_row, '功能描述')
		logging.debug(f'seq_idx={self.seq_idx}, subseq_idx={self.subseq_idx}, addr_idx={self.addr_idx}')
		logging.debug(f'sig_name_idx={self.sig_name_idx}, default={self.default_value_idx}, opmode_idx={self.opmode_idx}, comments_idx={self.comments_idx}')
		if (self.addr_idx == -1) or (self.sig_name_idx == -1):
			logging.error(f'The address mapping has no "寄存器地址" or "控制信号名"')
			exit(1)

	def is_row_valid(self, row):
		if len(row) < 3:
			return False

		if (row[self.addr_idx].value == None) or (row[self.addr_idx].value == ''):
			return False
		else:
			return True

	def str2hex(self, hex_str):
		return int(hex_str, base=16)

	def str2int(self, hex_str):
		return int(hex_str, base=10)

	def parse_reg_offset(self, row):
		v = self.row_value(row, self.addr_idx)
		if v == None:
			logging.error(f'Failed to parse reg offset')
			exit(1)

		words = v.split('[')
		offset = self.str2hex(words[0])
		return offset

	def check_bit_start_end(self, start, end):
		if start > end:
			logging.debug(f'bit {start} >= {end}')
			exit(1)

		if (start < 0) or (end > 31):
			logging.debug(f'bit [{start}:{end}] is out of range')
			exit(1)

	def parse_bit_start_end(self, row):
		v = self.row_value(row, self.addr_idx)
		words = v.split('[')
		if len(words) == 1:
			return [0, 31]

		words = words[1].split(']')
		if len(words) == 0:
			logging.error(f'invalid start/end bit')
			exit(1)

		words = words[0].split(':')
		if len(words) == 2:
			start = self.str2int(words[0])
			end = self.str2int(words[1])
			if start < end:
				return [start, end]
			else:
				return [end, start]
		else:
			start = self.str2int(words[0])
			return [start, start]

	def parse_seq(self, row):
		v = self.row_value(row, self.seq_idx)
		if v == None:
			return ''
		else:
			return v

	def parse_sub_seq(self, row):
		v = self.row_value(row, self.subseq_idx)
		if v == None:
			return ''
		else:
			return v.strip()

	def parse_sig_name(self, row):
		v = self.row_value(row, self.sig_name_idx)
		if v == None:
			return 'rsv'

		words = v.split('<')
		if len(words) == 0:
			logging.error(f'empty sig name')
			exit(1)

		words = words[0].split('[')
		if len(words) == 0:
			logging.error(f'empty sig name')
			exit(1)

		v = words[0].lower()
		return v

	def parse_comments(self, row):
		v = self.row_value(row, self.comments_idx)
		return v

	def parse_opmode(self, row):
		v = self.row_value(row, self.opmode_idx)
		return v

	def parse_default(self, row):
		v = self.row_value(row, self.default_value_idx)
		return v

	def parse_bit(self):
		logging.debug(f'start to parse reg offset={self.cur_reg_offset}, row idx={self.cur_parse_idx}')

		row = self.rows[self.cur_parse_idx]
		if self.is_row_valid(row) == False:
			return None

		reg_off = self.parse_reg_offset(row)
		bit_start_end = self.parse_bit_start_end(row)
		self.check_bit_start_end(bit_start_end[0], bit_start_end[1])
		seq = self.parse_seq(row)
		sub_seq = self.parse_sub_seq(row)
		sig_name = self.parse_sig_name(row)
		comments = self.parse_comments(row)
		opmode = self.parse_opmode(row)
		default = self.parse_default(row)

		start = bit_start_end[0]
		end = bit_start_end[1]
		bit = Bit(reg=reg_off, seq=seq, sub_seq=sub_seq, sig_name=sig_name, start=start,
			end=end, default=default, opmode=opmode, comments=comments)
		return bit

	def is_first_reg_bit(self, cur_bit, last_bit):
		if last_bit == None:
			return True

		if (cur_bit.reg < last_bit.reg):
			logging.error(f'current bit reg offset={cur_bit.reg}, last bit reg offset={last_bit.reg}')
			exit(1)
		elif (cur_bit.reg == last_bit.reg):
			return False
		elif (cur_bit.reg > last_bit.reg):
			return True

	def parse_regs(self):
		self.cur_parse_idx = 2
		self.row_cnt = len(self.rows)
		self.cur_reg_offset = 0

		last_bit = None
		reg = None
		while (self.cur_parse_idx < self.row_cnt):
			bit = self.parse_bit()
			self.cur_parse_idx = self.cur_parse_idx + 1
			if (bit == None):
				continue

			if self.is_first_reg_bit(bit, last_bit) == True:
				if reg != None:
					reg.completed_a_reg()
					self.add_reg(reg)
				reg = Reg(name=bit.get_reg_name(), offset=bit.reg)
			reg.add_bit(bit)
			last_bit = bit

		if reg != None:
			reg.completed_a_reg()
			self.add_reg(reg)

	def parse_excel(self):
		self.excel = load_workbook(self.infile)
		self.sheet = self.excel[self.module]
		self.rows = []
		for r in self.sheet:
			self.rows.append(r)

		self.parse_base_addr()
		self.parse_column_format()
		self.parse_regs()

	def add_reg(self, reg):
		regs_len = len(self.regs)
		if regs_len == 0:
			self.regs.append(reg)
		else:
			last_reg = self.regs[regs_len - 1]
			if (last_reg.offset + 1) < reg.offset:
				if self.reghex:
					name = f'rsv_%x_%x' %(last_reg.offset + 1, reg.offset - 1)
				else:
					name = f'rsv_%d_%d' %(last_reg.offset + 1, reg.offset - 1)
				reserved_reg = Reg(name=name, offset=last_reg.offset+1, is_regs=True, reg_cnt=reg.offset - last_reg.offset - 1)
				self.regs.append(reserved_reg)
			self.regs.append(reg)

	def max_bit_name_len(self):
		l = 0
		for r in self.regs:
			if r.is_regs:
				continue

			for b in r.bits:
				if l < len(b.sig_name):
					l = len(b.sig_name)
		return l

	def dump(self):
		logging.info(f'Excel infile={self.infile}, module={self.module}, reg_start={self.start_reg}, reg_end={self.end_reg}')

		for m in self.regs:
			m.dump()

if __name__ == "__main__":
	p = Regparse()
	p.dump()
