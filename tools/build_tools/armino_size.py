#!/usr/bin/python3

import argparse
import sys
import re
import os

RE_BSS_START='\s+0x\w+\s+_bss_start = \.'
RE_BSS_END='\s+0x\w+\s+_bss_end = \.'
RE_MEM_CONFIG='Memory Configuration\r?\n\r?\nName             Origin             Length             Attributes\r?\n([\s\S]+)\r?\n\r?\nLinker script and memory map'
RE_MEM_MAP = 'Linker script and memory map([\s\S]+?)OUTPUT\('
RE_COMPONENT_NAME = '0x\w+\s+0x\w+\s+.+?([^/\\\]+\.[ao])\((.+\.(o|obj))\)'
RE_FILE_NAME = '0x\w+\s+0x\w+\s+.+?([^/\\\]+\.[ao])\((.+)\.(o|obj)\)'
RE_SYMBOL_NAME = '(0x\w+)\s+(0x\w+)\s+.+?([^/\\\]+\.[ao])\((.+)\.(o|obj)\)'
RE_FILL_ADDR_LEN = '\*fill\*[ \t]+(0x\w+)[ \t]+(0x\w+)[ \t]+\r?\n'
RE_ADDR_LEN = '(0x\w+)[ \t]+(0x\w+)[ \t]+.+[/\\\]'
RE_ADDR_LEN_COMPONENT = '(0x\w+)\s+(0x\w+)([ ]+.+?)([^/\\\]+\.[ao])\('
RE_FILE_POST_FIX = '\.(o|obj)\)'
RE_O_OBJ = '\((.+)\.(o|obj)\)'

class Mem:
	def __init__(self):
		self.mem = {}

class Size:
	def __init__(self, map_file_name):
		self.map_file_name = os.path.basename(map_file_name)
		#self.map_file_name = map_file_name
		self.mem_config_list = [] #size summary
		self.mem_name_list = []
		self.components_config_list = [] #size summary per-component
		self.component_config_list = [] #size detailed per-one-component
		self.component_name_list = []
		self.files_config_list = [] #size summary per-file
		self.mem_map = []
		with open(map_file_name, 'r') as f:
			self.map_file = f.read()
		self.get_mem_config()
		self.get_mem_map()
		self.get_component_name_list()
		self.get_file_name_list()

	def get_mem_config(self):
		mem_config_block_list = re.findall(RE_MEM_CONFIG, self.map_file)
		for mem_config_block in mem_config_block_list:
			mem_config_block.strip('\n')
			mem_config_lines = mem_config_block.split('\n')
			for mem_config_line in mem_config_lines:
				m = {}
				mem_config_line = ' '.join(mem_config_line.split())
				mem_config = mem_config_line.split(' ')
				m["name"] = mem_config[0]
				m["start"] = int(mem_config[1], 16)
				m["end"] = int(mem_config[1], 16) + int(mem_config[2], 16)
				m["len"] = int(mem_config[2], 16)
				m["used"] = int(0)
				self.mem_name_list.append(mem_config[0])
				self.mem_config_list.append(m)

		bss_start_list = re.findall(RE_BSS_START, self.map_file)
		bss_start = bss_start_list[0].strip('\n')
		bss_start_addr = bss_start.split()[0]
		bss_start_addr = bss_start_addr.replace('0x00', '0x')

		bss_end_list = re.findall(RE_BSS_END, self.map_file)
		bss_end = bss_end_list[0].strip('\n')
		bss_end_addr = bss_end.split()[0]
		bss_end_addr = bss_end_addr.replace('0x00', '0x')

		bss = {}
		bss["name"] = "bss"
		bss["start"] = int(bss_start_addr, 16)
		bss["end"] = int(bss_end_addr, 16)
		bss["len"] = int(bss_end_addr, 16) - int(bss_start_addr, 16)
		bss["used"] = int(0)
		self.mem_name_list.append("bss")
		self.mem_config_list.append(bss)

		#print(self.mem_config_list)

	def get_mem_map(self):
		# find memory map (without discard and debug sections)
		mem_map_list = re.findall(RE_MEM_MAP + self.map_file_name.replace('.map','.elf'), self.map_file)
		if not mem_map_list:
			mem_map = ''
		else:
			mem_map = mem_map_list[0]

		if not mem_map:
			print('Can\'t parse memory info, memory info get fail!')
			exit(1)

		self.mem_map = mem_map.replace('\r', '')
		#self.mem_map = self.mem_map.replace("COMMON", " ")
		#print(self.mem_map)

	def get_component_name_list(self):
		component_name_list = []
		for l in self.mem_map.split('\n'):
			m = re.search(RE_COMPONENT_NAME, l)
			if m == None:
				continue
			component_name_list.append(m.groups()[0])

		component_name_list = list(set(component_name_list))
		component_name_list.sort(key = lambda x : x.upper())
		component_name_list += ['*fill*']
		self.component_name_list = component_name_list

	def get_file_name_list(self):
		file_name_list = []
		for l in self.mem_map.split('\n'):
			m = re.search(RE_FILE_NAME, l)
			if m == None:
				continue
			file_name_list.append(m.groups()[1])

		file_name_list = list(set(file_name_list))
		file_name_list.sort(key = lambda x : x.upper())
		file_name_list += ['*fill*']
		#print(file_name_list)
		self.file_name_list = file_name_list

	def dump_mem_config_summary(self):
		print('')
		print('%-12s   %-8s   %-8s   %-8s'%('type', 'start', 'length', 'used'))
		print('%-12s   %-8s   %-8s   %-8s'%('------------', '--------', '--------', '--------'))
		for m in self.mem_config_list:
			print('%-12s   %-8x   %-8x   %-8x'%(m['name'], m['start'], m['len'], m['used']))

	#The 'l' can be components or files
	def dump_component_file_config(self, l):
		print('')
		title = '%-36s   %-24s   '%('symbol', 'file')

		for mem in self.mem_name_list:
			title += '%-8s   '%(mem)

		print('%s'%(title))

		for config in l:
			config_info = '%-36s   %-24s   '%(config['name'], config['file'])
			for mem in self.mem_name_list:
				if config[mem] is None:
					config_info += '%-8x   '%(0)
				else:
					config_info += '%-8x   '%(config[mem])
			print('%s'%(config_info))

	def find_mem_config(self, component_name):
		#print(self.components_config_list)
		for l in self.components_config_list:
			if (component_name == l['name']):
				return l

		return None

	#The 'l' can be components or files
	def dump_mem_config(self, l):
		print('')
		title = '%-24s   '%('item')

		for mem in self.mem_name_list:
			title += '%-8s   '%(mem)

		print('%s'%(title))

		for config in l:
			config_info = '%-24s   '%(config['name'])
			for mem in self.mem_name_list:
				if config[mem] is None:
					config_info += '%-8x   '%(0)
				else:
					config_info += '%-8x   '%(config[mem])
			print('%s'%(config_info))

	def dump_components_mem_config(self):
		self.dump_mem_config(self.components_config_list)
		self.dump_mem_config_summary()

	def dump_files_mem_config(self):
		self.dump_mem_config(self.files_config_list)
		self.dump_mem_config_summary()

	def get_sections_by_component(self, component_name):
		sections = []
		if(component_name == '*fill*'):
			result = re.findall(RE_FILL_ADDR_LEN, self.mem_map)
			sections = map(lambda arg : {'address':int(arg[0], 16), 'size':int(arg[1], 16)}, result)
			print(sections)
		else:
			result = re.findall(RE_ADDR_LEN + component_name + RE_O_OBJ, self.mem_map)
			sections = map(lambda arg : {'address':int(arg[0], 16), 'size':int(arg[1], 16)}, result)
		#print(result)
		return sections

	def get_sections_by_file(self, file_name):
		sections = []
		#if (file_name != "vif_mgmt.c"):
		#	return

		if(file_name == '*fill*'):
			result = re.findall(RE_FILL_ADDR_LEN, self.mem_map)
			sections = map(lambda arg : {'address':int(arg[0], 16), 'size':int(arg[1], 16)}, result)
			print(sections)
		else:
			result = re.findall(RE_ADDR_LEN_COMPONENT + file_name + RE_FILE_POST_FIX, self.mem_map)
			#if (file_name == "vif_mgmt.c"):
				#print(result)
			sections = map(lambda arg : {'address':int(arg[0], 16), 'size':int(arg[1], 16)}, result)

		#print(sections)
		return sections

	def get_symbols_by_component(self, component_name):
		sections = []
		if(component_name == '*fill*'):
			result = re.findall(RE_FILL_ADDR_LEN, self.mem_map)
		else:
			result = re.findall(RE_ADDR_LEN + component_name + RE_O_OBJ, self.mem_map)
		sections = map(lambda arg : {'address':int(arg[0], 16), 'size':int(arg[1], 16), 'file': arg[2], 'symbol':arg[5]}, result)
		return sections

	def get_components_mem_config(self):
		for component_name in self.component_name_list:
			component_name = component_name.replace('+', '\+')
			component_keys = ['name'] + self.mem_name_list
			component = dict.fromkeys(component_keys)
			component['name'] = component_name
			sections = self.get_sections_by_component(component_name)
			if(not sections):
				continue

			for line in sections:
				for mem in self.mem_config_list:
					#print(' mem_name={} start={} end={}'.format(mem['name'], mem['start'], mem['end']))
					if(mem['start'] <= line['address'] < mem['end']):
						mem['used'] += line['size']
						if component[mem['name']] is None:
							component[mem['name']] = line['size']
						else:
							component[mem['name']] += line['size']
						#break
						
			self.components_config_list.append(component)

	def get_files_mem_config(self):
		for file_name in self.file_name_list:
			file_name = file_name.replace('+', '\+')
			file_keys = ['name'] + self.mem_name_list
			file_config = dict.fromkeys(file_keys)
			file_config['name'] = file_name
			sections = self.get_sections_by_file(file_name)
			if(not sections):
				continue

			for line in sections:
				for mem in self.mem_config_list:
					#print(' mem_name={} start={} end={}'.format(mem['name'], mem['start'], mem['end']))
					if(mem['start'] <= line['address'] < mem['end']):
						mem['used'] += line['size']
						if file_config[mem['name']] is None:
							file_config[mem['name']] = line['size']
						else:
							file_config[mem['name']] += line['size']
						#break
						
			self.files_config_list.append(file_config)

	def size_summary(self):
		self.dump_mem_config_summary()

	def size_components(self):
		self.get_components_mem_config()
		self.dump_components_mem_config()

	def size_diff(self, another_map_file):
		#print(another_map_file)
		self.get_components_mem_config()
		self.dump_components_mem_config()


	def size_component(self, component_name="bk_wifi"):
		component_name = component_name.replace('+', '\+')
		component_keys = ['name'] + ['file'] + self.mem_name_list

		sections = self.get_symbols_by_component(component_name)
		if(not sections):
			print('No symbols related to component: {}'.format(component_name))

		for line in sections:
			symbol_config = dict.fromkeys(component_keys)
			symbol_config['name'] = line['symbol']
			symbol_config['file'] = line['file']
			for mem in self.mem_config_list:
				#print(' mem_name={} start={} end={}'.format(mem['name'], mem['start'], mem['end']))
				if(mem['start'] <= line['address'] < mem['end']):
					symbol_config[mem['name']] = line['size']
					self.component_config_list.append(symbol_config)
					#break
					
		self.dump_component_file_config(self.component_config_list)

	def size_files(self):
		self.get_files_mem_config()
		self.dump_files_mem_config()

	def size_file(self):
		print("Not support yet!")
		exit(1)

def size_diff_cal(s1, s2):
	if (s1 == None) and (s2 == None):
		return 0

	if (s1 == None) and (s2 != None):
		return -s2

	if (s1 != None) and (s2 == None):
		return s1

	return (s1 - s2)

def dump_diff(s1, s2):
	print('')
	print('Size Diff:')

	l = s1.components_config_list
	title = '%-24s   '%('item')

	for mem in s1.mem_name_list:
		title += '%-8s   '%(mem)

	print('%s'%(title))

	for config in l:
		another_config = s2.find_mem_config(config['name'])
		config_info = '%-24s   '%(config['name'])

		for mem in s1.mem_name_list:
			if another_config != None:
				diff = size_diff_cal(config[mem], another_config[mem])
			else:
				diff = config[mem]

			if diff is None:
				config_info += '%-8x   '%(0)
			else:
				config_info += '%-8x   '%(diff)
		print('%s'%(config_info))

def main():

	parser = argparse.ArgumentParser(description='armino_size - a tool to print size information from an ARMINO MAP file')

	parser.add_argument(
		'--summary', help='Print size summary', action='store_true')

	parser.add_argument(
		'--components', help='Print per-component sizes', action='store_true')

	parser.add_argument(
		'--component', help='Print detailed symbols per component',
		default=None,
		dest='component_name')

	parser.add_argument(
		'--files', help='Print per-file sizes', action='store_true')

	parser.add_argument(
		'--map_file', help='Map file produced by linker',
		default=None,
		dest='map_file_name')

	parser.add_argument(
		'--diff', help='Show the differences in comparison with another MAP file',
		metavar='ANOTHER_MAP_FILE',
		default=None,
		dest='another_map_file')

	args = parser.parse_args()
	s = Size(args.map_file_name)

	if args.summary:
		s.size_summary()

	if args.components:
		s.size_components()

	if args.component_name:
		s.size_component(args.component_name)

	if args.files:
		s.size_files()

	if args.another_map_file and (args.another_map_file != 'invalid_map'):
		print('\n%s size info:'%(args.map_file_name))
		s.size_components()
		s2 = Size(args.another_map_file)
		print('\n%s size info:'%(args.another_map_file))
		s2.size_components()
		dump_diff(s, s2)

if __name__ == "__main__":
	main()
