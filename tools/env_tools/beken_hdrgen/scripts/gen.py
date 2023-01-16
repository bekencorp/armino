#!/usr/bin/env python3

import logging
import sys
import os

from .parse import *

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
// This is a generated file, if you need to modify it, use the script to\r\n\
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!\r\n\
\r\n\
"

s_pragma = "\
#pragma once\r\n\
\r\n\
"

s_cplus_start = "\
#ifdef __cplusplus\r\n\
extern \"C\" {\r\n\
#endif\r\n\r\n\
"

s_cplus_end = "\
#ifdef __cplusplus\r\n\
}\r\n\
#endif\r\n\
"
ana_write = "\
\r\n\
//This way of setting ana_reg_bit value is only for sys_ctrl, other driver please implement by yourself!!\r\n\
\r\n\
#define SYS_ANALOG_REG_SPI_STATE_REG (SYS_CPU_ANASPI_FREQ_ADDR)\r\n\
#define SYS_ANALOG_REG_SPI_STATE_POS(idx) (idx + SYS_CPU_ANASPI_FREQ_ANAREG_STATE_POS)\r\n\
#define GET_SYS_ANALOG_REG_IDX(addr) ((addr - SYS_ANA_REG0_ADDR) >> 2)\r\n\
\r\n\
static inline uint32_t sys_ll_get_analog_reg_value(uint32_t addr)\r\n\
{\r\n\
\treturn REG_READ(addr);\r\n\
}\r\n\
\r\n\
static inline void sys_ll_set_analog_reg_value(uint32_t addr, uint32_t value)\r\n\
{\r\n\
\tuint32_t idx;\r\n\
\tidx = GET_SYS_ANALOG_REG_IDX(addr);\r\n\
\r\n\
\tREG_WRITE(addr, value);\r\n\
\r\n\
\twhile(REG_READ(SYS_ANALOG_REG_SPI_STATE_REG) & (1 << SYS_ANALOG_REG_SPI_STATE_POS(idx)));\r\n\
}\r\n\
\r\n\
static inline void sys_set_ana_reg_bit(uint32_t reg_addr, uint32_t pos, uint32_t mask, uint32_t value)\r\n\
{\r\n\
\tuint32_t reg_value;\r\n\
\treg_value = *(volatile uint32_t *)(reg_addr);\r\n\
\treg_value &= ~(mask << pos);\r\n\
\treg_value |= ((value & mask) <<pos);\r\n\
\tsys_ll_set_analog_reg_value(reg_addr, reg_value);\r\n\
}\r\n\
"

class Gen:
	def __init__(self, parse):
		self.module = parse.module.lower()
		self.base_macro_name = f'SOC_{self.module.upper()}_REG_BASE'
		self.parse = parse
		self.struct_file = f'{self.module}_struct.h'
		self.reg_file = f'{self.module}_reg.h'
		self.ll_file = f'{self.module}_ll.h'
		self.dump_file = f'{self.module}_hal_debug.c'
		self.hw_file = f'{self.module}_hw.h'    		

	def empty_line(self, f):
		f.write('\r\n')

	def start_brace(self, f):
		f.write('%s\r\n' %("{"))

	def end_brace(self, f):
		f.write('%s\r\n' %("}"))

	def gen_struct(self):
		f = open(self.struct_file, 'w+')
		f.write(s_license)
		f.write(s_pragma)
		f.write(s_cplus_start)

		max_bit_name_len = self.parse.max_bit_name_len()
		for r in self.parse.regs:
			if r.is_regs:
				continue

			self.empty_line(f)
			f.write('typedef volatile union %s\r\n' %("{"))
			f.write('\tstruct %s\r\n' %("{"))
			for b in r.bits:
				if (max_bit_name_len > 24):
					f.write('\t\tuint32_t %-32s : %2d; /**<bit[%d : %d] */\r\n' %(b.sig_name, b.end - b.start + 1, b.start, b.end))
				elif (max_bit_name_len > 16):
					f.write('\t\tuint32_t %-24s : %2d; /**<bit[%d : %d] */\r\n' %(b.sig_name, b.end - b.start + 1, b.start, b.end))
				else:
					f.write('\t\tuint32_t %-16s : %2d; /**<bit[%d : %d] */\r\n' %(b.sig_name, b.end - b.start + 1, b.start, b.end))
			f.write('\t%s;\r\n' %("}"))
			f.write('\tuint32_t v;\r\n')
			f.write('%s %s_%s_t;\r\n\r\n' %("}", self.module, r.name))
	
		f.write('typedef volatile struct %s\r\n' %("{"))
		for r in self.parse.regs:
			if r.is_regs:
				f.write(f'\tvolatile uint32_t {r.name}[{r.reg_cnt}];\r\n')
			else:
				f.write(f'\tvolatile {self.module}_{r.name}_t {r.name};\r\n')
		f.write('%s %s_hw_t;\r\n' %("}", self.module))

		self.empty_line(f)
		f.write(s_cplus_end)

	def gen_dump(self):
		f = open(self.dump_file, 'w+')
		f.write(s_license)

		f.write(f'#include "hal_config.h"\r\n')
		f.write(f'#include "{self.module}_hw.h"\r\n')
		f.write(f'#include "{self.module}_hal.h"\r\n')

		self.empty_line(f)
		f.write('typedef void (*%s_dump_fn_t)(void);\r\n' %(self.module))
		f.write('typedef struct %s\r\n' %("{"))
		f.write('\tuint32_t start;\r\n')
		f.write('\tuint32_t end;\r\n')
		f.write('\t%s_dump_fn_t fn;\r\n' %(self.module))
		f.write('%s %s_reg_fn_map_t;\r\n' %("}", self.module))

		for r in self.parse.regs:
			self.empty_line(f)
			f.write(f'static void {self.module}_dump_{r.name}(void)\r\n')
			self.start_brace(f)
			if r.is_regs:
				reg_cnt = r.reg_cnt
				f.write('\tfor (uint32_t idx = 0; idx < %d; idx++) %s\r\n' %(r.reg_cnt, "{"))
				f.write('\t\tSOC_LOGI("%s: %s", REG_READ(%s + ((0x%x + idx) << 2)));\r\n' %(r.name, "%8x\\r\\n", self.base_macro_name, r.offset))
				f.write('\t%s\r\n' %("}"))
			elif (len(r.bits) == 1):
				f.write('\tSOC_LOGI("%s: %s", REG_READ(%s + (0x%x << 2)));\r\n' %(r.name, "%8x\\r\\n", self.base_macro_name, r.offset))
			else:
				f.write(f'\t%s_%s_t *r = (%s_%s_t *)(%s + (0x%x << 2));\r\n\r\n'
					%(self.module, r.name, self.module, r.name, self.base_macro_name, r.offset))
				f.write('\tSOC_LOGI("%s: %s", REG_READ(%s + (0x%x << 2)));\r\n' %(r.name, "%8x\\r\\n", self.base_macro_name, r.offset))
				for b in r.bits:
					f.write('\tSOC_LOGI("%s%s: %s", r->%s);\r\n' %("\t", b.sig_name, "%8x\\r\\n", b.sig_name))
			f.write('%s\r\n' %("}"))
		
		self.empty_line(f)
		f.write('static %s_reg_fn_map_t s_fn[] =\r\n' %(self.module))
		f.write('%s\r\n' %("{"))
		for r in self.parse.regs:
			if r.is_regs:
				f.write('\t%s0x%x, 0x%x, %s_dump_%s%s,\r\n' %("{", r.offset, r.offset + r.reg_cnt, self.module, r.name, "}"))
			else:
				f.write('\t%s0x%x, 0x%x, %s_dump_%s%s,\r\n' %("{", r.offset, r.offset, self.module, r.name, "}"))
		f.write('\t%s-1, -1, 0%s\r\n' %("{", "}"))
		f.write('%s;\r\n\r\n' %("}"))

		f.write(f'void {self.module}_struct_dump(uint32_t start, uint32_t end)\r\n')
		self.start_brace(f)
		f.write('\tuint32_t dump_fn_cnt = sizeof(s_fn)/sizeof(s_fn[0]) - 1;\r\n\r\n')
		f.write('\tfor (uint32_t idx = 0; idx < dump_fn_cnt; idx++) {\r\n')
		f.write('\t\tif ((start <= s_fn[idx].start) && (end >= s_fn[idx].end)) %s\r\n' %("{"))
		f.write('\t\t\ts_fn[idx].fn();\r\n')
		f.write('\t\t}\r\n')
		f.write('\t}\r\n')
		self.end_brace(f)

	def is_ana_reg(self, reg):
		if reg.name == None:
			return False
		if (reg.name.find('ana_reg') != -1) or (reg.name.find('ANA_REG') != -1) or (reg.name.find('Ana_reg') != -1):
			return True
		else:
			return False

	def bit_can_write(self, bit):
		if bit.opmode == None:
			return True

		if (bit.opmode.find('W') != -1) or (bit.opmode.find('w') != -1):
			return True
		else:
			return False

	def bit_can_read(self, bit):
		if bit.opmode == None:
			return True

		if (bit.opmode.find('R') != -1) or (bit.opmode.find('r') != -1):
			return True
		else:
			return False

	def gen_ll(self):
		f = open(self.ll_file, 'w+')
		f.write(s_license)
		f.write(s_pragma)

		f.write(f'#include <soc/soc.h>\r\n')
		f.write(f'#include "hal_port.h"\r\n')
		f.write(f'#include "{self.module}_hw.h"\r\n')

		self.empty_line(f)
		f.write(s_cplus_start)

#		f.write(f'#define %-24s REG_SET(({self.base_macro_name}) + ((r) << 2), (l), (r), (v))\r\n' %(f'{self.module}_ll_set_r(r, l, r, v)'))
#		f.write(f'#define %-24s REG_GET(({self.base_macro_name}) + ((r) << 2), (l), (r))\r\n' %(f'{self.module}_ll_get_r(r, l, r)'))
#		f.write(f'#define %-24s REG_OR(({self.base_macro_name}) + ((r) << 2), (l), (r), (v))\r\n' %(f'{self.module}_ll_or_r(r, l, r, v)'))
		
		f.write(f'#define {self.module.upper()}_LL_REG_BASE   SOC_{self.module.upper()}_REG_BASE\r\n')
		
		for r in self.parse.regs:
			if self.is_ana_reg(r):
				f.write(ana_write)
				break	

		for r in self.parse.regs:
			if r.is_regs:
				continue

			if self.is_ana_reg(r):
				self.empty_line(f)
#				f.write(f'#define %-24s REG_READ(({self.base_macro_name}) + (r.offset << 2))\r\n' %(f'{self.module}_ll_get_{r.name}()'))
				f.write(f'//reg {r.name}:\r\n')					
				self.empty_line(f)
				f.write(f'static inline void {self.module}_ll_set_{r.name}_value(uint32_t v) %s\r\n' %("{"))
				f.write(f'\tsys_ll_set_analog_reg_value(({self.base_macro_name} + (0x%x << 2)), v);\r\n' %(r.offset))
				f.write('%s\r\n' %("}"))
				self.empty_line(f)
				f.write(f'static inline uint32_t {self.module}_ll_get_{r.name}_value(void) %s\r\n' %("{"))
				f.write(f'\treturn sys_ll_get_analog_reg_value({self.base_macro_name} + (0x%x << 2));\r\n' %(r.offset))
				f.write('%s\r\n' %("}"))
				for b in r.bits:
					if self.bit_can_write(b):
						self.empty_line(f)
						f.write(f'static inline void {self.module}_ll_set_{r.name}_{b.sig_name}(uint32_t v) %s\r\n' %("{"))
						f.write(f'\t{self.module}_set_ana_reg_bit(({self.base_macro_name} + (0x%x << 2)), {b.start}, 0x%x, v);\r\n' %(r.offset, b.get_mask()))
						f.write('%s\r\n' %("}"))
					
					if self.bit_can_read(b):
						self.empty_line(f)
						f.write(f'static inline uint32_t {self.module}_ll_get_{r.name}_{b.sig_name}(void) %s\r\n' %("{"))
						f.write(f'\t{self.module}_{r.name}_t *r = ({self.module}_{r.name}_t*)({self.base_macro_name} + (0x%x << 2));\r\n' %(r.offset))
						f.write(f'\treturn r->{b.sig_name};\r\n')
						f.write('%s\r\n' %("}"))
	
			else:
				self.empty_line(f)
				f.write(f'//reg {r.name}:\r\n')					
				self.empty_line(f)
				f.write(f'static inline void {self.module}_ll_set_{r.name}_value(uint32_t v) %s\r\n' %("{"))
				f.write(f'\t{self.module}_{r.name}_t *r = ({self.module}_{r.name}_t*)({self.base_macro_name} + (0x%x << 2));\r\n' %(r.offset))
				f.write(f'\tr->v = v;\r\n')
				f.write('%s\r\n' %("}"))
				self.empty_line(f)
				f.write(f'static inline uint32_t {self.module}_ll_get_{r.name}_value(void) %s\r\n' %("{"))
				f.write(f'\t{self.module}_{r.name}_t *r = ({self.module}_{r.name}_t*)({self.base_macro_name} + (0x%x << 2));\r\n' %(r.offset))
				f.write(f'\treturn r->v;\r\n')
				f.write('%s\r\n' %("}"))
				
				for b in r.bits:
					if self.bit_can_write(b):
						self.empty_line(f)
						f.write(f'static inline void {self.module}_ll_set_{r.name}_{b.sig_name}(uint32_t v) %s\r\n' %("{"))
						f.write(f'\t{self.module}_{r.name}_t *r = ({self.module}_{r.name}_t*)({self.base_macro_name} + (0x%x << 2));\r\n' %(r.offset))
						f.write(f'\tr->{b.sig_name} = v;\r\n')
						f.write('%s\r\n' %("}"))

					if self.bit_can_read(b):
						self.empty_line(f)
						f.write(f'static inline uint32_t {self.module}_ll_get_{r.name}_{b.sig_name}(void) %s\r\n' %("{"))
						f.write(f'\t{self.module}_{r.name}_t *r = ({self.module}_{r.name}_t*)({self.base_macro_name} + (0x%x << 2));\r\n' %(r.offset))
						f.write(f'\treturn r->{b.sig_name};\r\n')
						f.write('%s\r\n' %("}"))
		f.write(s_cplus_end)

	def gen_reg(self):
		f = open(self.reg_file, 'w+')
		f.write(s_license)
		f.write(s_pragma)

		self.empty_line(f)
		f.write(s_cplus_start)

		for r in self.parse.regs:
			if r.is_regs:
				continue

			self.empty_line(f)
			addr = f'{self.module}_{r.name}_ADDR'
			f.write(f'#define {addr.upper()} ({self.base_macro_name} + (0x%x << 2))\r\n' %(r.offset))

			for b in r.bits:
				self.empty_line(f)
#				pos = f'{self.module}_{r.name}_{b.sig_name}_POS (0x%x)\r\n' %(b.start)
				pos = f'{self.module}_{r.name}_{b.sig_name}_POS'
				f.write(f'#define {pos.upper()} (%d)\r\n' %(b.start))
#				mask = f'{self.module}_{r.name}_{b.sig_name}_MASK (0x%x)\r\n' %(b.get_mask())
				mask = f'{self.module}_{r.name}_{b.sig_name}_MASK'
				f.write(f'#define {mask.upper()} (0x%x)\r\n' %(b.get_mask()))

		self.empty_line(f)
		f.write(s_cplus_end)

	def gen_hw(self):
		f = open(self.hw_file, 'w+')
		f.write(s_license)
		f.write(s_pragma)
		f.write(f'#include "common/bk_include.h"\r\n')
		f.write(f'#include "soc/soc.h"\r\n')
		self.empty_line(f)	
		f.write(f'#include "{self.module}_reg.h"\r\n')
		f.write(f'#include "{self.module}_struct.h"\r\n')
		f.write(f'#include "{self.module}_ll.h"\r\n')

