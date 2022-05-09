#!/usr/bin/env python3

import argparse
import sys
import re
import os
import csv

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
"

pragma_once =\
"\
\n\
#pragma once\n\
\n\
#include \"sys_config.h\"\
\n\
"

s_public_only_macros = [
	"CONFIG_BASE_MAC_FROM_RF_OTP_FLASH",
	"CONFIG_TASK_MEDIA_PRIO",
	"CONFIG_APP_MAIN_TASK_STACK_SIZE",
	"CONFIG_UART_PRINT_PORT",
	"CONFIG_TASK_KEYSCAN_PRIO",
	"CONFIG_TASK_EXT_APP_PRIO",
	"CONFIG_OTA_HTTP",
	"CONFIG_TASK_UMP3_PRIO",
	# "CONFIG_FREERTOS_V9",
	"CONFIG_APP_MAIN_TASK_PRIO",
	"CONFIG_BK_ASSERT_IGNORE",
	# "CONFIG_LWIP_2_0_2",
	"CONFIG_INT_STATIS",
	"CONFIG_EASY_FLASH",
	"CONFIG_SD1_HOST_INTF",
	"CONFIG_AUDIO_DAC",
	"CONFIG_AUDIO_ADC",
	"CONFIG_AUDIO",
	"CONFIG_SD_HOST_INTF",
	]

s_private_only_macros = [
	"CONFIG_USB_DEVICE_CARD_READER",
	]

s_skip_macros = [
	"CONFIG_SOC",
	"CONFIG_SOC_STR",
	"CONFIG_SOC_NAME",
	"CONFIG_CHECK_MISMATCH"
	]

class ConfigGen:

	def dump(self):
		l = len(self.config_set)
		macro_id = 0

		print(f'Total {l} different macros')
		for config in self.config_set:
			print(f'    m{macro_id}: {config}')
			macro_id += 1
		print('')

	def add(self, macro):
		self.config_set.add(macro)

	def __init__(self, output_file_dir=None, config_file_dir=None, config_file_list=None, dd_file=None):
		self.lib_config_name = output_file_dir + "/lib_sys_config.c"
		self.config_name = output_file_dir + "/sys_config.c"
		self.config_check_name = output_file_dir + "/sys_config_check.c"
		self.config_header_name = output_file_dir + "/include/bk_private/sys_config_check.h"
		self.config_set = set()

		self.lib_config_file = open(self.lib_config_name, 'w')
		self.config_file = open(self.config_name, 'w')
		self.config_header_file = open(self.config_header_name, 'w')
		self.config_check_file = open(self.config_check_name, 'w')

		for config_file_name in config_file_list:
			config_file_name = config_file_dir + "/" + config_file_name
			print(f'Start to parse {config_file_name}')
			f = open(config_file_name, 'r')
			for l in f:
				l = ' '.join(l.split())
				line_keys = l.split(' ')
				
				if ((line_keys[0] == '#define') and (len(line_keys) == 3)):
					self.add(line_keys[1])
			f.close()

		print(f'Start to parse {dd_file}')
		f = open(dd_file, 'r')
		for l in f:
			l = ' '.join(l.split())
			line_keys = l.split(',')
			line_key0 = line_keys[0]
			keys = line_key0.split(' ')
			key0 = keys[0]
			index = keys[0].find("DD_DEV_TYPE_")
			if (index == 0):
				self.add(key0)
		#self.dump()

	def gen_config_header_file_header(self):
		self.config_header_file.write(header_block)
		self.config_header_file.write(pragma_once)
		src_line = "\n"
		src_line += "#include \"drv_model_pub.h\"\n"
		src_line += "\n"
		src_line += "bool bk_config_check(void);\n"
		self.config_header_file.write(src_line)

	def gen_config_file_header(self):
		self.config_file.write(header_block)
		src_line = "\n"
		src_line += "#include \"sys_config.h\"\n"
		src_line += "#include \"drv_model_pub.h\"\n"
		src_line += "#include \"stdint.h\"\n"
		src_line += "#include \"stdbool.h\"\n"
		src_line += "\n"
		self.config_file.write(src_line)
	

	def gen_lib_config_file_header(self):
		self.lib_config_file.write(header_block)
		src_line = "\n"
		src_line += "#include \"sys_config.h\"\n"
		src_line += "#include \"drv_model_pub.h\"\n"
		src_line += "#include \"stdint.h\"\n"
		src_line += "#include \"stdbool.h\"\n"
		src_line += "\n"
		self.lib_config_file.write(src_line)

	def gen_config_check_file_header(self):
		self.config_check_file.write(header_block)
		src_line = "\n"
		src_line += "#include \"stdint.h\"\n"
		src_line += "#include \"stdbool.h\"\n"
		src_line += "#include \"drv_model_pub.h\"\n"
		src_line += "#include \"<driver/uart.h>\"\n"
		src_line += "#include \"bk_uart.h\"\n"
		src_line += "#include \"<components/bk_log.h>\"\n"
		src_line += "#include \"sys_config_check.h\"\n"
		src_line += "\n"
		src_line += "#define TAG    \"config\""
		src_line += "\n"
		src_line += "bool bk_config_check(void)\n"
		src_line += "{"
		self.config_check_file.write(src_line)

	def gen_config_header_file_tail(self):
		print(f"gen config header finished")

	def gen_config_file_tail(self):
		print(f"gen config finished")

	def gen_lib_config_file_tail(self):
		print(f"gen lib config finished")

	def gen_config_check_file_tail(self):
		src_line = "\n"
		src_line += "\treturn true;\n"
		src_line += "}"
		self.config_check_file.write(src_line)


	def gen_config_file(self, m, api):
		api_line = "\n"
		api_line += "uint32_t " + api + "(void)\n"
		api_line += "{\n"
		api_line += "#ifdef " + m + "\n"
		api_line += "\treturn " + m + ";\n"
		api_line += "#else\n"
		api_line += "\treturn 0;\n"
		api_line += "#endif\n"
		api_line += "}\n"
		self.config_file.write(api_line)

	def gen_lib_config_file(self, m, api):
		api_line = "\n"
		api_line += "uint32_t " + api + "(void)\n"
		api_line += "{\n"
		api_line += "#ifdef " + m + "\n"
		api_line += "\treturn " + m + ";\n"
		api_line += "#else\n"
		api_line += "\treturn 0;\n"
		api_line += "#endif\n"
		api_line += "}\n"
		self.lib_config_file.write(api_line)

	def gen_config_check_file(self, m, api, lib_api):
		api_line = "\n"
		api_line += "\t" + m + "_CHECK();"
		self.config_check_file.write(api_line)

	def gen_config_header_file(self, m, api, lib_api):
		api_line = "\n"
		api_line += "uint32_t " + api + "(void);\n"
		api_line += "uint32_t " + lib_api + "(void);\n"
		self.config_header_file.write(api_line)

		check_line = "#define    " + m + "_CHECK()\\\n"
		check_line += "\tif (" + api + "() != " + lib_api + "()) {\\\n"
		check_line += "\t\tBK_LOGW(TAG, \"" + m + " mismatch(%x vs %x)\\n\", " + api + "(), " + lib_api + "());\\\n"
		check_line += "\t}\n"
		self.config_header_file.write(check_line)

	def gen_config_check_files(self):
		print('Start to generate config APIs')
		self.gen_config_header_file_header()
		self.gen_config_file_header()
		self.gen_lib_config_file_header()
		self.gen_config_check_file_header()

		skip_macros = s_skip_macros + s_public_only_macros + s_private_only_macros
		for m in self.config_set:
			need_skip = False
			for skip in skip_macros:
				if (m == skip):
					need_skip = True
					continue

			if (need_skip):
				continue

			api_name = m.lower() + "_get"
			lib_api_name = "lib_" + api_name

			self.gen_config_header_file(m, api_name, lib_api_name)
			self.gen_config_file(m, api_name)
			self.gen_lib_config_file(m, lib_api_name)
			self.gen_config_check_file(m, api_name, lib_api_name)

		self.gen_config_header_file_tail()
		self.gen_config_file_tail()
		self.gen_lib_config_file_tail()
		self.gen_config_check_file_tail()

def main():
	parser = argparse.ArgumentParser(description='armino_gen - gen config api according to sys_config.h')

	parser.add_argument(
		'--config_file', help='sys_config.y file: sys_config_bk7236.h etc',
		default=None,
		dest='config_file')

	args = parser.parse_args()

	config_file_list = list()
	if args.config_file:
		if os.path.exists(args.config_file) == False:
			print(f'Config file {args.config_file} not exists')
			exit(1)
		config_file_list.append(args.config_file)
	else:
		config_file_list = ["sys_config_bk7231n.h", "sys_config_bk7236.h", "sys_config_bk7271.h", "sys_config_bk7231u.h", "sys_config_bk7251.h"]

	config_file_dir = os.path.realpath("./../components/bk_config/")
	out_file_dir = os.path.realpath("./../components/bk_libs/")
	dd_file = os.path.realpath("./../components/driver/include/bk_private/legacy/drv_model_pub.h")
	dump_gen = ConfigGen(out_file_dir, config_file_dir, config_file_list, dd_file)
	dump_gen.gen_config_check_files()

if __name__ == "__main__":
	main()
