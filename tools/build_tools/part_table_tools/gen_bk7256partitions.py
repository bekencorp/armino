#!/usr/bin/env python
#
# Beken partition table generation tool
#
# Converts partition tables from CSV to CSV/Json/Sag/Inc/Src file.
#
# Copyright 2023-2024 Beken (Author chun.long)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http:#www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import argparse
import os
import re
import sys
import hashlib
import binascii
import errno
import json
import copy

quiet = False
gen_files_path = "%s/config/gen_files_list.txt"%(sys.path[0])
JSON_FILENAMES = ['configuration_new.json', 'partition_bk7256_ota_a_new.json', 'partition_bk7256_ota_r_new.json']

def status(msg):
    """ Print status message to stderr """
    if not quiet:
        critical(msg)

def critical(msg):
    """ Print critical message to stderr """
    sys.stderr.write(msg)
    sys.stderr.write('\n')

def size_format(size, include_size):
    if include_size:
        for (val, suffix) in [(0x100000, "M"), (0x400, "K")]:
            if size % val == 0:
                return "%d%s" % (size // val, suffix)
    size_str = '%x'%size
    lead_zero = '0'*(8 - len(size_str))
    return "0x%s%s" % (lead_zero, size_str)

def ensure_directory(dir):
    if not os.path.exists(dir):
        try:
            os.makedirs(dir)
        except OSError as exc:
            if exc.errno != errno.EEXIST:
                raise

def parse_int(v, keywords={}, stop=True):
    """Generic parser for integer fields - int(x,0) with provision for
    k/m/K/M suffixes and 'keyword' value lookup.
    """
    try:
        for letter, multiplier in [("k", 1024), ("m", 1024 * 1024)]:
            if v.lower().endswith(letter) and re.match(r'^\d',v.lower()):
                return parse_int(v[:-1], keywords) * multiplier
        return int(v, 0)
    except ValueError:
        if len(keywords) == 0:
            raise InputError("Invalid field value %s" % v)
        try:
            return keywords[v.lower()]
        except KeyError:
            if stop:
                raise InputError("Value '%s' is not valid. Known keywords: %s" % (v, ", ".join(keywords)))
            else:
                return v

class InputError(RuntimeError):
    def __init__(self, e):
        super(InputError, self).__init__(e)

class CallStackError(InputError):
    def __init__(self, func, message):
        super(CallStackError, self).__init__(
            "Func %s call error:\n\t %s" % (func, message))

class ValidationError(InputError):
    def __init__(self, partition, message):
        super(ValidationError, self).__init__(
            "Partition %s invalid: %s" % (partition.name, message))

class Bk7256PartTableGenerator:
    def __init__(self,
                flash_size = '4MB',
                no_verify = False,
                to_json = None,
                to_sag = None,
                to_src = None,
                to_inc = None,
                to_csv = None,
                csv_contents = None):
        self.flash_size = flash_size
        self.no_verify = no_verify
        if to_json is None:
            self.to_json = "%s/Bk7256PartTableGenerator.json"%(sys.path[0])
        else:
            self.to_json = to_json
        if to_sag is None:
            self.to_sag = "%s/Bk7256PartTableGenerator.sag"%(sys.path[0])
        else:
            self.to_sag = to_sag
        if to_src is None:
            self.to_src = "%s/Bk7256PartTableGenerator.c"%(sys.path[0])
        else:
            self.to_src = to_src
        if to_inc is None:
            self.to_inc = "%s/Bk7256PartTableGenerator.h"%(sys.path[0])
        else:
            self.to_inc = to_inc
        if to_csv is None:
            self.to_csv = "%s/Bk7256PartTableGenerator.csv"%(sys.path[0])
        else:
            self.to_csv = to_csv
        self.csv_contents = csv_contents
        self.aux_var = None
        self.part_table_old = None
        self.part_table = None
        self.inc_arch_dict = None
        self.pt_gen_done = False
        self.pt_remap_done = False
        self.files_set = set()
        self.json_funcs_outfiles = JSON_FILENAMES
        self.json_funcs_start = 0
        self.json_funcs_end = len(JSON_FILENAMES)
        self.json_gen_done = False
        self.sag_gen_done = False
        self.src_gen_done = False
        self.inc_gen_done = False
        self.csv_gen_done = False

        if not os.path.exists(gen_files_path):
            raise RuntimeError("Absence of %s file, Please build it..."%(gen_files_path))
        with open(gen_files_path, 'r') as f:
            files_lines = f.readlines()
            for files_line in files_lines:
                if os.path.exists(files_line.strip()) and os.path.isfile(files_line.strip()):
                    self.files_set.add(files_line.strip())


    @classmethod
    def from_csv(cls, flash_size, no_verify, to_json, to_sag, to_src, to_inc, to_csv, from_csv):
        status("Parsing CSV input to gen part table...")
        csv_contents = from_csv.read().decode()
        res = Bk7256PartTableGenerator(flash_size, no_verify, to_json, to_sag, to_src, to_inc, to_csv, csv_contents)
        res.part_table = PartitionTable.from_csv(res.csv_contents)
        res.pt_gen_done = True

        return res

    def verify(self):
        self.part_table.verify()
        size_mb = int(self.flash_size.replace("MB", ""))
        size = size_mb * 1024 * 1024  # flash memory uses honest megabytes!
        table_size = self.part_table.flash_size()
        if size < table_size:
            raise InputError("Partitions defined occupy %.1fMB of flash (%d bytes) which does not fit in configured "
                            "flash size %dMB. Please adjust the flash size firstly." %
                            (table_size / 1024.0 / 1024.0, table_size, size_mb))


    def remap_part_table_addr(self):
        status("Remap part table, convert logic addr to phy addr...")
        if not self.pt_gen_done:
            raise CallStackError('remap_part_table_addr(...)', 'Please firstly call Bk7256PartTableGenerator.from_csv(...) to create part table!')

        def extend_val(val, need_extend):
            return int(val*34/32) if need_extend else val

        def align_to_strval(val, strval):
            return ((val + parse_int(strval) -1) // parse_int(strval)) * parse_int(strval)

        # before remap part table, verify the filed of csv contents from user editting 
        if not self.no_verify:
            self.verify()

        self.part_table_old = copy.deepcopy(self.part_table)
        last = None
        for p in self.part_table_old:
            if last is None:
                p.offset = align_to_strval(p.offset, '4K')
                p.size = align_to_strval(p.size, '4K')
            if last is not None:
                if p.offset < last.offset + last.size:
                    p.offset = align_to_strval(last.offset + last.size, '4K')
                    p.size = align_to_strval(p.size, '4K')
                else:
                    p.offset = align_to_strval(p.offset, '4K')
                    p.size = align_to_strval(p.size, '4K')
            last = p

        self.part_table_old_excute = copy.deepcopy(self.part_table)
        self.part_table_old_excute = [p for p in self.part_table_old_excute if p.execute]
        last = None
        for p in self.part_table_old_excute:
            if last is None:
                p.offset = align_to_strval(p.offset, '4K')
                p.size = align_to_strval(p.size, '4K')
            if last is not None:
                if p.offset < last.offset + last.size:
                    p.offset = align_to_strval(last.offset + last.size, '4K')
                    p.size = align_to_strval(p.size, '4K')
                else:
                    p.offset = align_to_strval(p.offset, '4K')
                    p.size = align_to_strval(p.size, '4K')
            last = p

        last = None
        for p in self.part_table:
            # for partition with flag execute, offset and size must be aligned to 68K (bk7256xx flash controller feature)
            # for partition without flag execute, offset and size must be aligned to 4K (flash sector size)
            strval = '68K' if p.execute else '4K'
            if last is None:
                p.size = extend_val(p.size, p.execute)
                p.offset = align_to_strval(p.offset, strval)
                p.size = align_to_strval(p.size, '4K')
            if last is not None:
                if p.offset < last.offset + last.size:
                    p.size = extend_val(p.size, p.execute)
                    p.offset = align_to_strval(last.offset + last.size, strval)
                    p.size = align_to_strval(p.size, '4K')
                else:
                    p.size = extend_val(p.size, p.execute)
                    p.offset = align_to_strval(p.offset, strval)
                    p.size = align_to_strval(p.size, '4K')
            last = p

        # after remap part table, verify again!
        if not self.no_verify:
            self.verify()

        self.pt_remap_done = True


    def clean_files_set(self):
        status("Remove all generated files")
        for f in self.files_set:
            os.remove(f.strip())
        self.files_set = set()


    def print_files_set_info(self, to_txt):
        if to_txt:
            with open(gen_files_path, 'w') as f:
                for files_line in self.files_set:
                    f.write(files_line + '\n')
        else:
            status("\n")
            status("# Generated files list :")
            for files_line in self.files_set:
                print(files_line)


    def print_part_table_info(self):
        for p in self.part_table:
            print(p)


    def print_part_table_apps(self):
        KEYWORDS = {
            "application": "app",
            "application1": "app1",
        }
        part_table_temp = list()
        for p in self.part_table:
            if re.match(r'^application(\d)*$',p.name):
                part_table_temp.append(p)
            if re.match(r'^app(\d)*$',p.name):
                part_table_temp.append(p)
        app_names = list()
        for p in part_table_temp:
            app_names.append(parse_int(p.name, KEYWORDS, stop = False))
        res = ' '.join(app_names)
        print(res)


    def print_part_table_csv(self, to_csv):
        status("Gen %s using remapped part table..." % (os.path.basename(self.to_csv)))
        if not self.pt_remap_done:
            raise CallStackError('print_part_table_csv(...)', 'Please firstly call remap_part_table_addr(...) to remap part table!')

        to_csv_dir = os.path.abspath(os.path.dirname(self.to_csv))
        ensure_directory(to_csv_dir)
        if to_csv:
            with open(self.to_csv, 'w') as f:
                f.write(self.part_table.to_csv())
                self.files_set.add(os.path.abspath(self.to_csv))
        else:
            print('# %s' % (os.path.basename(self.to_csv)))
            print(self.part_table.to_csv())

        self.csv_gen_done = True


    def print_part_table_json(self, to_json):
        if not self.pt_remap_done:
            raise CallStackError('print_part_table_json(...)', 'Please firstly call remap_part_table_addr(...) to remap part table!')

        # Just one template to define the interface for generate json contents
        def gen_template_json_contents():
            config_dict = {
                "magic": "Template",
            }
            config_json = json.dumps(config_dict, sort_keys=False, indent=4)
            return config_json

        # Generate json for bk7256chipset application 
        def gen_config_json_contents():
            KEYWORDS = {
                "application": "app",
                "application1": "app1",
            }
            config_dict = {
                "magic": "FreeRTOS",
                "version": "0.1",
                "count": 0,
                "section": [],
            }
            sec_dict_temp = {
                "firmware": None,
                "version": "2M.1220",
                "partition": None,
                "start_addr": None,
                "size": None
            }
            # just executed partition need output to config_json
            exectue_partitions = [p for p in self.part_table if p.execute]
            config_dict['count'] = len(exectue_partitions)
            for p in sorted(exectue_partitions, key=lambda x:x.offset):
                sec_dict = dict()
                sec_dict.update(sec_dict_temp)
                sec_dict['firmware'] = "%s.bin" % parse_int(p.name, KEYWORDS, stop = False)
                sec_dict['partition'] = "%s" % parse_int(p.name, KEYWORDS, stop = False)
                sec_dict['start_addr'] = size_format(p.offset, False)
                sec_dict['size'] = size_format(p.size, True)
                config_dict['section'].append(sec_dict)
            config_json = json.dumps(config_dict, sort_keys=False, indent=4)
            return config_json

        # Generate json for bk7256chipset bootloader 
        def gen_partition_json_contents():
            KEYWORDS = {
                "ota": "download",
                "application": "app",
                "application1": "app1",
            }
            part_table_dict = {
                "part_table": [],
            }
            part_dict_temp = {
                "magic": "0x45503130",
                "name": None,
                "flash_name": None,
                "offset": None,
                "len": None,
            }
            part_table_bark = copy.deepcopy(self.part_table)
            part_table_temp = list()
            for p in part_table_bark:
                if re.match(r'^bootloader(\d)*$',p.name):
                    part_table_temp.append(p)
                if re.match(r'^application(\d)*$',p.name):
                    part_table_temp.append(p)
                if re.match(r'^ota(\d)*$',p.name):
                    part_table_temp.append(p)
            for p in part_table_temp:
                p.name = parse_int(p.name, KEYWORDS, stop = False)
            for p in sorted(part_table_temp, key=lambda x:x.offset):
                part_dict = dict()
                part_dict.update(part_dict_temp)
                part_dict['name'] = p.name
                part_dict['flash_name'] = "beken_onchip_crc" if p.execute else "beken_onchip"
                part_dict['offset'] = size_format(p.offset, False)
                part_dict['len'] = size_format(p.size, True)
                part_table_dict['part_table'].append(part_dict)
            part_table_json = json.dumps(part_table_dict, sort_keys=False, indent=4)
            return part_table_json

        # Generate json for bk7256chipset bootloader ab partitions
        def gen_ab_partition_json_contents():
            KEYWORDS = {
                "application": "appa",
                "application1": "appb",
            }
            part_table_dict = {
                "part_table": [],
            }
            part_dict_temp = {
                "magic": "0x45503130",
                "name": None,
                "flash_name": None,
                "offset": None,
                "len": None,
            }
            part_table_bark = copy.deepcopy(self.part_table)
            part_table_temp = list()
            for p in part_table_bark:
                if re.match(r'^bootloader(\d)*$',p.name):
                    part_table_temp.append(p)
                if re.match(r'^application(\d)*$',p.name):
                    part_table_temp.append(p)
            for p in part_table_temp:
                p.name = parse_int(p.name, KEYWORDS, stop = False)
            for p in sorted(part_table_temp, key=lambda x:x.offset):
                part_dict = dict()
                part_dict.update(part_dict_temp)
                part_dict['name'] = p.name
                part_dict['flash_name'] = "beken_onchip_crc" if p.execute else "beken_onchip"
                part_dict['offset'] = size_format(p.offset, False)
                part_dict['len'] = size_format(p.size, True)
                part_table_dict['part_table'].append(part_dict)
            part_table_json = json.dumps(part_table_dict, sort_keys=False, indent=4)
            return part_table_json

        # Generate jsons for bk7256chipset application and bootloader 
        # For application json, the output filename is "configuration.json"
        # For bootloader json, the output filename is "partition.json"
        # This func can be template for generate multi json with call muti other funcs
        def gen_config_and_partition_json_contents():
            if self.aux_var is None:
                self.aux_var = 0
            else:
                self.aux_var += 1

            gen_json_funcs = [gen_config_json_contents, gen_partition_json_contents, gen_ab_partition_json_contents]
            gen_json_funcs = gen_json_funcs[self.json_funcs_start:self.json_funcs_end]
            if self.aux_var < len(gen_json_funcs)-1:
                self.print_part_table_json(to_json)

            gen_json_func = gen_json_funcs[self.aux_var]
            dirname = os.path.dirname(self.to_json)
            if len(gen_json_funcs) == 1:
                basename = self.json_funcs_outfiles[self.json_funcs_start]
            else:
                basename = self.json_funcs_outfiles[self.aux_var]
            self.to_json = os.path.join(dirname, basename)
            res_json = gen_json_func()

            if self.aux_var >= 0:
                self.aux_var -= 1
            if self.aux_var == -1:
                self.aux_var = None

            return res_json


        gen_json_contents_func = gen_config_and_partition_json_contents

        config_json = gen_json_contents_func()

        to_json_dir = os.path.abspath(os.path.dirname(self.to_json))
        ensure_directory(to_json_dir)
        if to_json:
            with open(self.to_json, 'w') as f:
                status("Gen %s using remapped part table..." % (os.path.basename(self.to_json)))
                f.write(config_json)
                self.files_set.add(os.path.abspath(self.to_json))
        else:
            print('# %s' % (os.path.basename(self.to_json)))
            print(config_json)
            print('\n')

        self.json_gen_done = True


    def print_part_table_sag(self, to_sag):
        if not self.pt_remap_done:
            raise CallStackError('print_part_table_sag(...)', 'Please firstly call remap_part_table_addr(...) to remap part table!')

        # Just one template to define the interface for generate sag contents
        def gen_template_sag_contents():
            res = ""
            pass
            return res

        # Generate one sag file, all executed partitions will be converted sections output to it
        def gen_flash_sag_contents():
            flash_offset = 0
            flash_size_mb = int(self.flash_size.replace("MB", ""))
            flash_size = flash_size_mb * 1024 * 1024

            res = ""
            res += "FLASH_LOAD_REGION 0x%x 0x%x\n"%(flash_offset, flash_size)
            res += "{\n"
            space = ' '*4
            for p in self.part_table_old:
                res += "%s%s 0x%x 0x%x\n"%(space, p.name.upper(), (p.offset + flash_offset), p.size)
                res += "%s{\n"%(space)
                space = ' '*8
                res += "%s\n"%(space)
                space = ' '*4
                res += "%s}\n"%(space)
            res += "}\n"

            return res

        # Generate multi sag file, one executed partition for one sag file
        # For each sag, the output filename is "${part.name}.sag"
        # This func can be template for generate multi sag with call itself
        def gen_partitions_sag_contents():
            if self.aux_var is None:
                self.aux_var = 0
            else:
                self.aux_var += 1

            exectue_partitions = [p for p in self.part_table_old if p.execute]
            if self.aux_var < len(exectue_partitions)-1:
                self.print_part_table_sag(to_sag)

            part = exectue_partitions[self.aux_var]
            dirname = os.path.dirname(self.to_sag)
            basename = part.name + '.sag'
            self.to_sag = os.path.join(dirname, basename)
            res = ""
            res += "%s_LOAD_REGION 0x%x 0x%x\n"%(part.name.upper(), part.offset, part.size)
            res += "{\n"
            space = ' '*4
            res += "%s\n"%(space)
            res += "}\n"


            if self.aux_var >= 0:
                self.aux_var -= 1
            if self.aux_var == -1:
                self.aux_var = None

            return res


        gen_sag_contents_func = gen_partitions_sag_contents

        sag_contents = ""
        sag_contents += gen_sag_contents_func()

        to_sag_dir = os.path.abspath(os.path.dirname(self.to_sag))
        ensure_directory(to_sag_dir)
        if to_sag:
            with open(self.to_sag, 'w') as f:
                status("Gen %s using remapped part table..." % (os.path.basename(self.to_sag)))
                f.write(sag_contents)
                self.files_set.add(os.path.abspath(self.to_sag))
        else:
            print('# %s' % (os.path.basename(self.to_sag)))
            print(sag_contents)
            print('\n')

        self.sag_gen_done = True


    def print_part_table_src(self, to_src, gendoc = True):
        if not self.inc_gen_done:
            raise CallStackError('print_part_table_src(...)', '%s depends on %s, Please firstly call print_part_table_inc(...) to create %s!'
                                %(os.path.basename(self.to_src), os.path.basename(self.to_inc), os.path.basename(self.to_inc)))

        def search_list_from_extern_name(ls,nm):
            found = False
            for l in ls:
                if nm == l['extern_name']:
                    found = True
                    return l
            if not found:
                raise RuntimeError('Please firstly in func print_part_table_inc(...) initialize and add \"%s\" entry it to inc_arch_dict!'%(nm))

        def macro_inner_entry_from_enum(enum, nm):
            inner_prefix = enum['inner_prefix']
            inner_suffix = enum['inner_suffix']
            found = False
            for e in enum['inner_entries']:
                if nm == e:
                    found = True
                    return '%s%s%s'%(inner_prefix, e, inner_suffix)
            if not found:
                raise RuntimeError('Please firstly in func print_part_table_inc(...) add \'%s\' to %s[\'inner_entries\']!'%(nm, enum['extern_name']))

        def gen_template_struct_contents(name, part_table, bitmap_list, enum_list, struct_list):
            res = ""
            pass
            return res

        def gen_bk7256_partitions_struct_contents(name, part_table, bitmap_list, enum_list, struct_list):
            # create bk_logic_partition_t struct type instance, 
            # the contents of this instance associate with 
            # part_table, partition_flags bitmap, 
            # bk_flash_t enum and bk_partition_t enum 
            extern_name = 'partition_flags'
            partition_flags_bitmap = search_list_from_extern_name(bitmap_list, extern_name)
            extern_name = 'bk_flash_t'
            flash_enum = search_list_from_extern_name(enum_list, extern_name)
            extern_name = 'bk_partition_t'
            partition_enum = search_list_from_extern_name(enum_list, extern_name)
            extern_name = 'bk_partition_user_t'
            user_enum = search_list_from_extern_name(enum_list, extern_name)
            extern_name = 'bk_logic_partition_t'
            logic_partition_struct = search_list_from_extern_name(struct_list, extern_name)

            res = ""
            """
            res += "void bk_user_macro_printf(void)\n"
            res += "{\n"
            for p in part_table:
                res += "    os_printf(\"bk_user_%s = %%d\\r\\n\",%s);\n"%(p.name, macro_inner_entry_from_enum(user_enum, p.name).upper())
            res += "    os_printf(\"bk_user_%s = %%d\\r\\n\",%s);\n"%('max', macro_inner_entry_from_enum(user_enum, 'max').upper())
            res += "}\n"
            """
            res += "/* Logic partition on flash devices */\n"
            res += "const %s %s[%s] = {\n"%(logic_partition_struct['extern_name'], name, macro_inner_entry_from_enum(user_enum, 'max').upper())
            for p in part_table:
                space = ' '*4
                res += "%s[%s] = \n"%(space, macro_inner_entry_from_enum(user_enum, p.name).upper())
                #res += "%s[BK_USER_%s] = \n"%(space, p.name.upper())
                res += "%s{\n"%(space)
                space = ' '*8
                res += "%s.%s = %s,\n"%(space, logic_partition_struct['inner_entries'][0], macro_inner_entry_from_enum(flash_enum, 'embedded').upper())
                res += "%s.%s = \"%s\",\n"%(space, logic_partition_struct['inner_entries'][1][1:], p.name)
                res += "%s.%s = 0x%x,\n"%(space, logic_partition_struct['inner_entries'][2], p.offset)
                res += "%s.%s = 0x%x,\n"%(space, logic_partition_struct['inner_entries'][3], p.size)
                res += "%s.%s = %s%s%s,\n"%(space, logic_partition_struct['inner_entries'][4],
                                                        macro_inner_entry_from_enum(partition_flags_bitmap, 'execute').upper() + ('_EN' if p.execute else '_DIS'),
                                                        ' | ' + macro_inner_entry_from_enum(partition_flags_bitmap, 'read').upper() + ('_EN' if p.read else '_DIS'),
                                                        ' | ' + macro_inner_entry_from_enum(partition_flags_bitmap, 'write').upper() + ('_EN' if p.write else '_DIS'))
                space = ' '*4
                res += "%s},\n"%(space)
            res += "};\n"

            return res


        # Generate all struct instance with uinque struct func in gen_struct_contents_funcs,
        # if you want create one new struct instance, firstly register it's func to gen_struct_contents_funcs
        # struct func template has the format: gen_bk_temp_struct_contents(name, part_table, inc_arc)
        gen_struct_contents_funcs = [
            (gen_bk7256_partitions_struct_contents, 'bk7256_partitions'),
            (gen_template_struct_contents, 'template'),
        ]

        src_contents = ""
        if gendoc:
            src_contents += "/**\n"
            src_contents += " *********************************************************************************\n"
            src_contents += " * @file %s\n"%(os.path.basename(self.to_src))
            src_contents += " * @brief This file provides all the headers of Flash operation functions..\n"
            src_contents += " *********************************************************************************\n"
            src_contents += " *\n"
            src_contents += " * The MIT License\n"
            src_contents += " * Copyright (c) 2017 BEKEN Inc.\n"
            src_contents += " *\n"
            src_contents += " * Permission is hereby granted, free of charge, to any person obtaining a copy\n"
            src_contents += " * of this software and associated documentation files (the \"Software\"), to deal\n"
            src_contents += " * in the Software without restriction, including without limitation the rights\n"
            src_contents += " * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
            src_contents += " * copies of the Software, and to permit persons to whom the Software is furnished\n"
            src_contents += " * to do so, subject to the following conditions:\n"
            src_contents += " *\n"
            src_contents += " * The above copyright notice and this permission notice shall be included in\n"
            src_contents += " * all copies or substantial portions of the Software.\n"
            src_contents += " *\n"
            src_contents += " * THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
            src_contents += " * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
            src_contents += " * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"
            src_contents += " * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,\n"
            src_contents += " * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR\n"
            src_contents += " * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE\n"
            src_contents += " *********************************************************************************\n"
            src_contents += "*/\n"

        src_contents += "#include <common/bk_include.h>\n"
        src_contents += "#include <os/os.h>\n"
        src_contents += "#if CONFIG_FLASH_ORIGIN_API\n"
        src_contents += "#include \"BkDriverFlash.h\"\n"
        src_contents += "#else\n"
        src_contents += "#include <driver/flash_partition.h>\n"
        src_contents += "#endif\n"
        src_contents += "#include <common/bk_kernel_err.h>\n"
        src_contents += "#include <os/mem.h>\n"
        src_contents += "#include <%s>\n"%(os.path.basename(self.to_inc))
        src_contents += "\n"

        inc_arc = self.inc_arch_dict
        bitmap_list = inc_arc['bitmap_list']
        enum_list = inc_arc['enum_list']
        struct_list = inc_arc['struct_list']
        for func,name in gen_struct_contents_funcs:
            src_contents += func(name, self.part_table, bitmap_list, enum_list, struct_list)

        to_src_dir = os.path.abspath(os.path.dirname(self.to_src))
        ensure_directory(to_src_dir)
        if to_src:
            with open(self.to_src, 'w') as f:
                status("Gen %s source file using remapped part table, which depends on %s source file..." % (os.path.basename(self.to_src), os.path.basename(self.to_inc)))
                f.write(src_contents)
                self.files_set.add(os.path.abspath(self.to_src))
        else:
            print('# %s' % (os.path.basename(self.to_src)))
            print(src_contents)
            print('\n')

        self.src_gen_done = True


    def print_part_table_inc(self, to_inc, gendoc = True):
        if not self.pt_remap_done:
            raise CallStackError('print_part_table_inc(...)', 'Please firstly call remap_part_table_addr(...) to remap part table!')

        def gen_bitmap_partition_flags_contents(extern_name, inner_prefix, inner_suffix, inner_entries):
            res = ""
            res += "#if (!CONFIG_FLASH_ORIGIN_API)\n"
            res += "\n"
            for index in range(len(inner_entries)):
                res += "#define %s%s_POS%s     (%d)\n" %(inner_prefix, inner_entries[index].upper(), inner_suffix, index)
                res += "#define %s%s_DIS%s     (0x0u << %s%s_POS%s)\n" % (inner_prefix, inner_entries[index].upper(), inner_suffix,
                                                                                        inner_prefix, inner_entries[index].upper(), inner_suffix)
                res += "#define %s%s_EN%s     (0x1u << %s%s_POS%s)\n" % (inner_prefix, inner_entries[index].upper(), inner_suffix,
                                                                                        inner_prefix, inner_entries[index].upper(), inner_suffix)
                res += "\n"
            res += "#endif\n"
            res += "\n"
            return res

        def gen_bitmap_null_contents(extern_name, inner_prefix, inner_suffix, inner_entries):
            res = ""
            pass
            return res

        def gen_enum_null_contents(extern_name, inner_prefix, inner_suffix, inner_entries, inner_values):
            res = ""
            pass
            return res

        def gen_enum_default_contents(extern_name, inner_prefix, inner_suffix, inner_entries, inner_values):
            res = ""
            res += "typedef enum\n"
            res += "{\n"
            for index in range(len(inner_entries)):
                res += "    %s%s%s = %d,\n" % (inner_prefix, inner_entries[index].upper(), inner_suffix, inner_values[index])
            res += "}%s;\n" % (extern_name)
            res += "\n"
            return res

        def gen_enum_bk_flash_t_contents(extern_name, inner_prefix, inner_suffix, inner_entries, inner_values):
            res = ""
            pass
            return res

        def gen_enum_bk_partition_t_contents(extern_name, inner_prefix, inner_suffix, inner_entries, inner_values):
            res = ""
            """
            res += "typedef enum\n"
            res += "{\n"
            for index in range(len(inner_entries)):
                res += "    %s%s%s = %d,\n" % (inner_prefix, inner_entries[index].upper(), inner_suffix, inner_values[index])
            res += "}%s;\n" % (extern_name)
            res += "\n"
            """
            for index in range(len(inner_entries)):
                res += "#define %s%s%s_TEMP %d\n"%(inner_prefix, inner_entries[index].upper(), inner_suffix, inner_values[index])
            res += "\n"
            return res

        def gen_enum_bk_user_t_contents(extern_name, inner_prefix, inner_suffix, inner_entries, inner_values):
            res = ""
            for index in range(len(inner_entries)):
                if inner_entries[index] == 'max':
                    continue
                res += "#ifdef BK_PARTITION_%s_TEMP\n"%(inner_entries[index].upper())
                res += "#define %s%s%s BK_PARTITION_%s_TEMP\n"%(inner_prefix, inner_entries[index].upper(), inner_suffix, inner_entries[index].upper())
                res += "#endif\n"
            res += "\n"

            res += "typedef enum\n"
            res += "{\n"
            res += "    BK_PARTITION_START_USER = BK_PARTITION_%s_TEMP - 1,\n"%('max'.upper())
            for index in range(len(inner_entries)):
                if inner_entries[index] == 'max':
                    continue
                res += "#ifndef %s%s%s\n"%(inner_prefix, inner_entries[index].upper(), inner_suffix)
                res += "    %s%s%s,\n"%(inner_prefix, inner_entries[index].upper(), inner_suffix)
                res += "#endif\n"
            res += "    %s%s%s,\n"%(inner_prefix, 'max'.upper(), inner_suffix)
            res += "}%s;\n" % (extern_name)
            res += "\n"
            return res

        def gen_struct_bk_logic_partition_t_contents(extern_name, inner_types, inner_entries):
            res = ""
            res += "typedef struct\n"
            res += "{\n"
            for index in range(len(inner_entries)):
                res += "    %s %s;\n" % (inner_types[index], inner_entries[index])
            res += "}%s;\n" % (extern_name)
            res += "\n"
            return res

        def gen_struct_null_contents(extern_name, inner_types, inner_entries):
            res = ""
            pass
            return res

        # assume header file just include bitmap, enum, struct definition
        inc_arch_dict = {
            "bitmap_list": [],
            "enum_list": [],
            "struct_list": [],
        }

        # define the bitmap template
        bitmap_entry_temp = {
            "extern_name": None,
            "inner_prefix": None,
            "inner_suffix": None,
            "inner_entries": [],
            "contents_genenator": None,
        }
        # define the enum template
        enum_entry_temp = {
            "extern_name": None,
            "inner_prefix": None,
            "inner_suffix": None,
            "inner_entries": [],
            "inner_values": [],
            "contents_genenator": None,
        }
        # define the struct template
        struct_entry_temp = {
            "extern_name": None,
            "inner_types": [],
            "inner_entries": [],
            "contents_genenator": None,
        }

        # use bitmap template initialize one bitmap entry, add it to inc_arch_dict
        keys = [key for key in PartitionDefinition.FLAGS.keys()]
        bitmap_flags_entry = copy.deepcopy(bitmap_entry_temp)
        bitmap_flags_entry['extern_name'] = 'partition_flags'
        bitmap_flags_entry['inner_prefix'] = 'par_opt_'
        bitmap_flags_entry['inner_suffix'] = ''
        bitmap_flags_entry['inner_entries'].extend(keys)
        bitmap_flags_entry['contents_genenator'] = gen_bitmap_null_contents
        inc_arch_dict['bitmap_list'].append(bitmap_flags_entry)

        # use enum template initialize one enum entry, add it to inc_arch_dict
        keys = ["embedded", "spi",]
        keys.extend(['max'])
        keys.extend(['none'])
        values = range(len(keys))
        enum_flash_entry = copy.deepcopy(enum_entry_temp)
        enum_flash_entry['extern_name'] = 'bk_flash_t'
        enum_flash_entry['inner_prefix'] = 'bk_flash_'
        enum_flash_entry['inner_suffix'] = ''
        enum_flash_entry['inner_entries'].extend(keys)
        enum_flash_entry['inner_values'].extend(values)
        enum_flash_entry['contents_genenator'] = gen_enum_null_contents
        inc_arch_dict['enum_list'].append(enum_flash_entry)

        # use enum template initialize one enum entry, add it to inc_arch_dict
        keys = ["bootloader","application","ota","application1","matter_flash","rf_firmware","net_param","usr_config","ota_fina_executive"]
        keys.extend(['max'])
        values = range(len(keys))
        enum_partition_entry = copy.deepcopy(enum_entry_temp)
        enum_partition_entry['extern_name'] = 'bk_partition_t'
        enum_partition_entry['inner_prefix'] = 'bk_partition_'
        enum_partition_entry['inner_suffix'] = ''
        enum_partition_entry['inner_entries'].extend(keys)
        enum_partition_entry['inner_values'].extend(values)
        enum_partition_entry['contents_genenator'] = gen_enum_bk_partition_t_contents
        inc_arch_dict['enum_list'].append(enum_partition_entry)

        # use enum template initialize one enum entry, add it to inc_arch_dict
        keys = [p.name for p in self.part_table]
        keys.extend(['max'])
        values = range(len(keys))
        enum_partition_entry = copy.deepcopy(enum_entry_temp)
        enum_partition_entry['extern_name'] = 'bk_partition_user_t'
        enum_partition_entry['inner_prefix'] = 'bk_partition_'
        enum_partition_entry['inner_suffix'] = '_user'
        enum_partition_entry['inner_entries'].extend(keys)
        enum_partition_entry['inner_values'].extend(values)
        enum_partition_entry['contents_genenator'] = gen_enum_bk_user_t_contents
        inc_arch_dict['enum_list'].append(enum_partition_entry)

        # use struct template initialize one struct entry, add it to inc_arch_dict
        logic_partition_t = {
            "partition_owner": "bk_flash_t",
            "*partition_description": "const char",
            "partition_start_addr": "uint32_t",
            "partition_length": "uint32_t",
            "partition_options": "uint32_t",
        }
        keys = logic_partition_t.keys()
        values = logic_partition_t.values()
        struct_logic_partition_entry = copy.deepcopy(struct_entry_temp)
        struct_logic_partition_entry['extern_name'] = 'bk_logic_partition_t'
        struct_logic_partition_entry['inner_types'].extend(values)
        struct_logic_partition_entry['inner_entries'].extend(keys)
        struct_logic_partition_entry['contents_genenator'] = gen_struct_null_contents
        inc_arch_dict['struct_list'].append(struct_logic_partition_entry)

        # record the inc arch to PartTableGenerator instance, then it is useful for print_part_table_src func
        self.inc_arch_dict = inc_arch_dict

        inc_contents = ""
        if gendoc:
            inc_contents += "/**\n"
            inc_contents += " *********************************************************************************\n"
            inc_contents += " * @file %s\n"%(os.path.basename(self.to_inc))
            inc_contents += " * @brief This file provides all the headers of Flash operation functions..\n"
            inc_contents += " *********************************************************************************\n"
            inc_contents += " *\n"
            inc_contents += " *Copyright 2020-2021 Beken\n"
            inc_contents += " *\n"
            inc_contents += " *Licensed under the Apache License, Version 2.0 (the \"License\");\n"
            inc_contents += " *you may not use this file except in compliance with the License.\n"
            inc_contents += " *You may obtain a copy of the License at\n"
            inc_contents += " *\n"
            inc_contents += " *     http://www.apache.org/licenses/LICENSE-2.0\n"
            inc_contents += " *\n"
            inc_contents += " *Unless required by applicable law or agreed to in writing, software\n"
            inc_contents += " *distributed under the License is distributed on an \"AS IS\" BASIS,\n"
            inc_contents += " *WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
            inc_contents += " *See the License for the specific language governing permissions and\n"
            inc_contents += " *limitations under the License.\n"
            inc_contents += " *********************************************************************************\n"
            inc_contents += "*/\n"

        inc_contents += "#pragma once\n"
        #inc_contents += "#include <driver/flash_partition.h>\n"
        inc_contents += "#ifdef __cplusplus\n"
        inc_contents += "extern \"C\" {\n"
        inc_contents += "#endif\n"
        inc_contents += "\n"

        # Generate all bitmap contents
        for bitmap in inc_arch_dict['bitmap_list']:
            extern_name = bitmap['extern_name']
            inner_prefix = bitmap['inner_prefix'].upper()
            inner_suffix = bitmap['inner_suffix'].upper()
            inner_entries = bitmap['inner_entries']
            contents_genenator = bitmap['contents_genenator']
            inc_contents += contents_genenator(extern_name, inner_prefix, inner_suffix, inner_entries)

        # Generate all enum contents
        for enum in inc_arch_dict['enum_list']:
            extern_name = enum['extern_name']
            inner_prefix = enum['inner_prefix'].upper()
            inner_suffix = enum['inner_suffix'].upper()
            inner_entries = enum['inner_entries']
            inner_values = enum['inner_values']
            contents_genenator = enum['contents_genenator']
            inc_contents += contents_genenator(extern_name, inner_prefix, inner_suffix, inner_entries, inner_values)

        # Generate all struct contents
        for struct in inc_arch_dict['struct_list']:
            extern_name = struct['extern_name']
            inner_types = struct['inner_types']
            inner_entries = struct['inner_entries']
            contents_genenator = struct['contents_genenator']
            inc_contents += contents_genenator(extern_name, inner_types, inner_entries)

        inc_contents += "#ifdef __cplusplus\n"
        inc_contents += "}\n"
        inc_contents += "#endif\n"

        to_inc_dir = os.path.abspath(os.path.dirname(self.to_inc))
        ensure_directory(to_inc_dir)
        if to_inc:
            with open(self.to_inc, 'w') as f:
                status("Gen %s source file using remapped part table..." % (os.path.basename(self.to_inc)))
                f.write(inc_contents)
                self.files_set.add(os.path.abspath(self.to_inc))
        else:
            print('# %s' % (os.path.basename(self.to_inc)))
            print(inc_contents)
            print('\n')

        self.inc_gen_done = True


class PartitionTable(list):
    def __init__(self):
        super(PartitionTable, self).__init__(self)

    @classmethod
    def from_csv(cls, csv_contents):
        res = PartitionTable()
        lines = csv_contents.splitlines()

        def expand_vars(f):
            f = os.path.expandvars(f)
            m = re.match(r'(?<!\\)\$([A-Za-z_][A-Za-z0-9_]*)', f)
            if m:
                raise InputError("unknown variable '%s'" % m.group(1))
            return f

        for line_no in range(len(lines)):
            line = expand_vars(lines[line_no]).strip()
            if line.startswith("#") or len(line) == 0:
                continue
            try:
                res.append(PartitionDefinition.from_csv(line, line_no + 1))
            except InputError as e:
                raise InputError("Error at line %d: %s" % (line_no + 1, e))
            except Exception:
                critical("Unexpected error parsing CSV line %d: %s" % (line_no + 1, line))
                raise

        return res

    def __getitem__(self, item):
        """ Allow partition table access via name as well as by
        numeric index. """
        if isinstance(item, str):
            for x in self:
                if x.name == item:
                    return x
            raise ValueError("No partition entry named '%s'" % item)
        else:
            return super(PartitionTable, self).__getitem__(item)

    def verify(self):
        # verify each partition individually
        for p in self:
            p.verify()

        # check on duplicate name
        names = [p.name for p in self]
        duplicates = set(n for n in names if names.count(n) > 1)

        # print sorted duplicate partitions by name
        if len(duplicates) != 0:
            print("A list of partitions that have the same name:")
            for p in sorted(self, key=lambda x:x.name):
                if len(duplicates.intersection([p.name])) != 0:
                    print("%s" % (p.to_csv()))
                raise InputError("Partition names must be unique")
        """
        # check for overlaps
        last = None
        for p in sorted(self, key=lambda x:x.offset):
            if last is not None and p.offset < last.offset + last.size:
                raise InputError("Partition at 0x%x-0x%x(%s) overlaps 0x%x-0x%x(%s)" 
                                % (p.offset, p.offset + p.size -1, p.name, last.offset,
                                last.offset + last.size - 1, last.name))
            last = p
        """

    def flash_size(self):
        """ Return the size that partitions will occupy in flash
            (ie the offset the last partition ends at)
        """
        try:
            last = sorted(self, reverse=True)[0]
        except IndexError:
            return 0  # empty table!
        return last.offset + last.size

    def to_csv(self, simple_formatting=False):
        rows = ["# BK7256xx Chipset Partition Table",
                "# Name, Offset, Size, Flags"]
        rows += [x.to_csv(simple_formatting) for x in self]
        return "\n".join(rows) + "\n"


class PartitionDefinition(object):
    FLAGS = {
        "read": 0,
        "write": 0,
        "execute": 0,
    }

    CSV_KEYWORDS = {
        "true": True,
        "false": False,
        "download": "ota",
        "app": "application",
        "app1": "application1",
    }

    def __init__(self):
        self.name = ""
        self.offset = None
        self.size = None
        self.flags = None

    @classmethod
    def from_csv(cls, line, line_no):
        """ Parse a line from the CSV """
        def from_csv_format_default(line, line_no):
            line_w_defaults = line + ",,,,"  # lazy way to support default fields
            fields = [f.strip() for f in line_w_defaults.split(",")]
            res = PartitionDefinition()
            res.line_no = line_no
            res.name = fields[0]
            res.offset = res.parse_address(fields[1])
            res.size = res.parse_address(fields[2])
            if res.size is None:
                raise InputError("Size field can't be empty")

            for flag in cls.FLAGS:
                setattr(res, flag, False)
            flags = fields[3].split(":")
            for flag in flags:
                if flag in cls.FLAGS:
                    setattr(res, flag, True)
                elif len(flag) > 0:
                    raise InputError("CSV flag column contains unknown flag '%s'" % (flag))
            return res

        def from_csv_format_common(line, line_no, csv_format):
            fields = [f.strip() for f in line.split(",")]
            if len(fields) != len(csv_format):
                return None

            res = PartitionDefinition()
            res.line_no = line_no
            for flag in cls.FLAGS:
                setattr(res, flag, False)
            for key in csv_format.keys():
                setattr(res, key, res.parse_filed_str(fields[csv_format[key]], cls.CSV_KEYWORDS))

            if res.flags != None:
                flags = res.flags.split(":")
                for flag in flags:
                    if flag in cls.FLAGS:
                        setattr(res, flag, True)
                    elif len(flag) > 0:
                        raise InputError("CSV flag column contains unknown flag '%s'" % (flag))
            return res

        def from_csv_format_adapt(line, line_no):
            CSV_FORMAT_V1 = {
                "name": 0,
                "offset": 1,
                "size": 2,
                "execute": 3,
                "read": 4,
                "write": 5,
            }
            CSV_FORMAT_V2 = {
                "name": 0,
                "offset": 1,
                "size": 2,
                "flags": 3,
            }
            CSV_FORMATS = [
                CSV_FORMAT_V1,
                CSV_FORMAT_V2,
            ]

            for index in range(len(CSV_FORMATS)):
                res = from_csv_format_common(line, line_no, CSV_FORMATS[index])
                if res is not None:
                    break
            return res

        from_csv_func = from_csv_format_adapt
        res = from_csv_func(line, line_no)
        #print(res.__dict__)
        return res

    def __eq__(self, other):
        return self.name == other.name and self.offset == other.offset \
            and self.size == other.size \

    def __repr__(self):
        def maybe_hex(x):
            return "0x%x" % x if x is not None else "None"
        return "PartitionDefinition('%s', %s, %s)" % (self.name, maybe_hex(self.offset), maybe_hex(self.size))

    def __str__(self):
        return "Part '%s' @ 0x%x size 0x%x" % (self.name, self.offset, self.size)

    def __cmp__(self, other):
        return self.offset - other.offset

    def __lt__(self, other):
        return self.offset < other.offset

    def __gt__(self, other):
        return self.offset > other.offset

    def __le__(self, other):
        return self.offset <= other.offset

    def __ge__(self, other):
        return self.offset >= other.offset

    def parse_address(self, strval):
        if strval == "":
            return None  # PartitionTable will fill in default
        return parse_int(strval)

    def parse_filed_str(self, strval, keywords):
        if strval == "":
            return None  # PartitionTable will fill in default
        return parse_int(strval, keywords, stop = False)

    def verify(self):
        if self.name is None:
            raise ValidationError(self, "Name field is not set")
        if self.offset is None:
            raise ValidationError(self, "Offset field is not set")
        if self.size is None:
            raise ValidationError(self, "Size field is not set")
        align = parse_int('4K')
        if self.offset % align:
            raise ValidationError(self, "Offset 0x%x is not aligned to 0x%x" % (self.offset, align))
        if self.size % align:
            raise ValidationError(self, "Size 0x%x is not aligned to 0x%x" % (self.size, align))

    def get_flags_list(self):
        return [flag for flag in self.FLAGS.keys() if getattr(self, flag)]

    def to_csv(self, simple_formatting=False):
        def addr_format(a, include_sizes):
            if not simple_formatting and include_sizes:
                for (val, suffix) in [(0x100000, "M"), (0x400, "K")]:
                    if a % val == 0:
                        return "%d%s" % (a // val, suffix)
            return "0x%x" % a

        def generate_text_flags():
            """ colon-delimited list of flags """
            return ":".join(self.get_flags_list())

        return ",".join([self.name,
                        addr_format(self.offset, False),
                        addr_format(self.size, True),
                        generate_text_flags()])

class Bk7256PartTableCli:
    def __init__(self):
        self.smode = False
        self.inseq = None
        self.inseq_id = 0
        self.ptg = None
        self.root_tbl = None
        self.tbl_inseq = list()
        self.tbl_inseq_list = list()
        self.tbl_inseq_dict = dict()

    @classmethod
    def from_generator(cls, smode, smode_inseq, flash_size, no_verify, to_json, to_sag, to_src, to_inc, to_csv, from_csv):
        res = Bk7256PartTableCli()
        part_table_generator = Bk7256PartTableGenerator.from_csv(flash_size, no_verify, to_json, to_sag,
                                                                to_src, to_inc, to_csv, from_csv)
        part_table_generator.remap_part_table_addr()
        res.smode = smode
        res.inseq = smode_inseq
        res.ptg = part_table_generator

        return res

    def cli_gen_files(self, tabel, iret, isel):
        self.cli_scan_tbl(tabel, iret, isel)

    def cli_show_files(self, table, iret, isel):
        self.ptg.print_files_set_info(to_txt = False)

    def cli_clean_files(self, table, iret, isel):
        self.ptg.clean_files_set()
        self.ptg.print_files_set_info(to_txt = True)

    def cli_show_apps(self, table, iret, isel):
        self.ptg.print_part_table_apps()

    def cli_gen_csv_file(self, table, iret, isel):
        self.ptg.print_part_table_csv(to_csv = True)
        self.ptg.print_files_set_info(to_txt = True)

    def cli_gen_json_files(self, table, iret, isel):
        #self.ptg.print_part_table_json(to_json = True)
        #self.ptg.print_files_set_info(to_txt = True)
        self.cli_scan_tbl(table, iret, isel)

    def cli_gen_sub_json_file(self, table, iret, isel):
        start_temp = self.ptg.json_funcs_start
        end_temp = self.ptg.json_funcs_end
        self.ptg.json_funcs_start = int(isel,0)-1
        self.ptg.json_funcs_end = int(isel,0)
        self.ptg.print_part_table_json(to_json = True)
        self.ptg.print_files_set_info(to_txt = True)
        self.ptg.json_funcs_start = start_temp
        self.ptg.json_funcs_end = end_temp

    def cli_gen_all_json_file(self, table, iret, isel):
        self.ptg.print_part_table_json(to_json = True)
        self.ptg.print_files_set_info(to_txt = True)

    def cli_gen_sag_files(self, table, iret, isel):
        #self.ptg.print_part_table_sag(to_sag = True)
        #self.ptg.print_files_set_info(to_txt = True)
        self.cli_scan_tbl(table, iret, isel)

    def cli_gen_sub_sag_file(self, table, iret, isel):
        part_table_old_temp = self.ptg.part_table_old
        self.ptg.part_table_old = [self.ptg.part_table_old_excute[int(isel,0)-1],]
        self.ptg.print_part_table_sag(to_sag = True)
        self.ptg.print_files_set_info(to_txt = True)
        self.ptg.part_table_old = part_table_old_temp

    def cli_gen_all_sag_file(self, table, iret, isel):
        self.ptg.print_part_table_sag(to_sag = True)
        self.ptg.print_files_set_info(to_txt = True)

    def cli_gen_source_file(self, table, iret, isel):
        self.ptg.print_part_table_inc(to_inc = True)
        self.ptg.print_part_table_src(to_src = True)
        self.ptg.print_files_set_info(to_txt = True)

    def cli_gen_all_file(self, table, iret, isel):
        #self.ptg.print_part_table_csv(to_csv = True)
        self.ptg.print_part_table_json(to_json = True)
        self.ptg.print_part_table_sag(to_sag = True)
        self.ptg.print_part_table_inc(to_inc = True)
        self.ptg.print_part_table_src(to_src = True)
        self.ptg.print_files_set_info(to_txt = True)

    def cli_cmode_input(self):
        select = input()
        return select

    def cli_smode_input(self):
        select = self.inseq[self.inseq_id]
        self.inseq_id += 1
        return select

    def cli_scan_tbl(self, table, iret, isel):
        ret = True
        while ret:
            if not self.smode:
                print("\n")
                print("Please Select Callable Func...")
                for index in range(len(table)):
                    print(table[index][4])
                select = self.cli_cmode_input()
            else:
                select = self.cli_smode_input()
            for tsel,func,tbl,tret,ban,inseqKey in table:
                if select == tsel:
                    if func is not None and callable(func):
                        func(tbl,tret,tsel)
                        ret = iret
                    else:
                        ret = tret
                    break

    def cli_tbl_init(self):
        # tbl template below:
        # tbl = [ ('No', FuncObj, SubTbl, FuncRet, 'BannerStr', 'InseqKey'), ]
        gen_json_files_tbl = []
        outfiles = self.ptg.json_funcs_outfiles
        for json_funcs_id in range(len(outfiles)):
            tbl_item = ('%d'%(json_funcs_id+1), self.cli_gen_sub_json_file, None, False, "# %d : Gen %s"%(json_funcs_id+1, outfiles[json_funcs_id]), "%s"%(outfiles[json_funcs_id]))
            gen_json_files_tbl.append(tbl_item)
        tbl_item = ('%d'%(len(outfiles)+1), self.cli_gen_all_json_file, None, False, "# %d : Gen all json"%(len(outfiles)+1), "genAllJson")
        gen_json_files_tbl.append(tbl_item)
        tbl_item = ('0', None, None, False, "# 0 : back", "back")
        gen_json_files_tbl.append(tbl_item)

        gen_sag_files_tbl = []
        pto = self.ptg.part_table_old_excute
        for pto_id in range(len(pto)):
            tbl_item = ('%d'%(pto_id+1), self.cli_gen_sub_sag_file, None, False, "# %d : Gen %s.sag"%((pto_id+1), pto[pto_id].name), "%s.sag"%(pto[pto_id].name))
            gen_sag_files_tbl.append(tbl_item)
        tbl_item = ('%d'%(len(pto)+1), self.cli_gen_all_sag_file, None, False, "# %d : Gen all sag"%(len(pto)+1), "genAllSag")
        gen_sag_files_tbl.append(tbl_item)
        tbl_item = ('0', None, None, False, "# 0 : back", "back")
        gen_sag_files_tbl.append(tbl_item)

        gen_files_tbl = [
            ('1', self.cli_gen_json_files, gen_json_files_tbl, True, "# 1 : Gen *.json files", "genJsonFiles"),
            ('2', self.cli_gen_sag_files, gen_sag_files_tbl, True, "# 2 : Gen *.sag files", "genSagFiles"),
            ('3', self.cli_gen_source_file, None, False, "# 3 : Gen *.h,*.c files", "genSourceFiles"),
            ('4', self.cli_gen_all_file, None, False, "# 4 : Gen all files", "genAllFiles"),
            ('0', None, None, False, "# 0 : back", "back"),
        ]

        root_tbl = [
            ('1', self.cli_gen_files, gen_files_tbl, True, "# 1 : Gen files", "genFiles"),
            ('2', self.cli_show_files, None, False, "# 2 : Display all Generated files", "display"),
            ('3', self.cli_clean_files, None, False, "# 3 : Clean all Generated files", "clean"),
            ('4', self.cli_show_apps, None, False, "# 4 : Show all apps name", "showApps"),
            ('0', None, None, False, "# 0 : exit", "exit")
        ]

        self.root_tbl = root_tbl

    def cli_tbl_inseq_travel(self, table):
        for tbl_item in table:
            if tbl_item[1] == None:
                continue
            if tbl_item[2] != None:
                self.tbl_inseq.append(tbl_item[0])
                self.cli_tbl_inseq_out(tbl_item[2])
                self.tbl_inseq.pop()
            else:
                self.tbl_inseq.append(tbl_item[0])
                tbl_inseq_temp = list()
                tbl_inseq_temp.extend(self.tbl_inseq)
                for index in range(len(tbl_inseq_temp)):
                    tbl_inseq_temp.append('0')
                self.tbl_inseq_list.append({'%s'%(tbl_item[5]): tbl_inseq_temp})
                self.tbl_inseq_dict[tbl_item[5]] = tbl_inseq_temp
                self.tbl_inseq.pop()

    def cli_tbl_inseq_out(self, table):
        self.cli_tbl_inseq_travel(table)
        inseq_json_temp = {
            'count': 0,
            'inseqs': [],
        }
        inseq_dict = dict()
        inseq_dict.update(inseq_json_temp)
        inseq_dict['count'] = len(self.tbl_inseq_dict)
        inseq_dict['inseqs'] = self.tbl_inseq_dict
        inseq_json = json.dumps(inseq_dict, sort_keys=False, indent=4)
        inseq_json_path = "%s/config/inseqs.json"%(sys.path[0])
        with open(inseq_json_path, 'w') as f:
            f.write(inseq_json)

    def cli_start(self):
        self.cli_tbl_init()
        table = self.root_tbl
        self.cli_tbl_inseq_out(table)
        self.cli_scan_tbl(table, True, '0')


def main():
    parser = argparse.ArgumentParser(description='partition table utility')

    parser.add_argument('--smode', help="Run in script mode, will bypass cli input", action='store_true')
    parser.add_argument('--smode-inseq', help="For script mode to gen special file", default='1,1,2,0,0,0')
    parser.add_argument('--flash-size', help='Optional flash size limit, checks partition table fits in flash',
                        nargs='?', choices=['1MB', '2MB', '4MB', '8MB', '16MB'], default='4MB')
    parser.add_argument('--no-verify', help="Don't verify partition table fields", action='store_true')
    parser.add_argument('--to-json', help='Path to json converted CSV file.', default=None)
    parser.add_argument('--to-sag', help='Path to sag converted CSV file.', default=None)
    parser.add_argument('--to-src', help='Path to src converted CSV file.', default=None)
    parser.add_argument('--to-inc', help='Path to inc converted CSV file.', default=None)
    parser.add_argument('--to-csv', help='Path to csv converted CSV file.', default=None)
    parser.add_argument('--output-paths', help='Paths to json/sag/src/inc/csv converted CSV file.', type=argparse.FileType('rb'), default=None)
    parser.add_argument('input', help='Path to CSV to parse.', type=argparse.FileType('rb'))

    args = parser.parse_args()

    flash_size = args.flash_size
    no_verify = args.no_verify
    inpt = args.input

    smode = args.smode
    smode_inseq = args.smode_inseq
    smode_inseq = [seq.strip() for seq in smode_inseq.split(',')]

    valid_path_key = {
        "to_json": 0,
        "to_sag": 0,
        "to_src": 0,
        "to_inc": 0,
        "to_csv": 0,
    }
    path_dict = dict()
    output_paths = args.output_paths
    if output_paths is not None:
        path_lines = output_paths.read().decode().splitlines()
        for path_line in path_lines:
            path_key, path_val = path_line.split('=')
            #print("path_key = %s, path_val = %s"%(path_key, path_val))
            if path_key.strip() in valid_path_key:
                path_dict[path_key.strip()] = path_val.strip()

    to_json = path_dict.get('to_json', None)
    to_sag = path_dict.get('to_sag', None)
    to_src = path_dict.get('to_src', None)
    to_inc = path_dict.get('to_inc', None)
    to_csv = path_dict.get('to_csv', None)

    if args.to_json is not None:
        to_json = args.to_json
    if args.to_sag is not None:
        to_sag = args.to_sag
    if args.to_src is not None:
        to_src = args.to_src
    if args.to_inc is not None:
        to_inc = args.to_inc
    if args.to_csv is not None:
        to_csv = args.to_csv

    ptc = Bk7256PartTableCli.from_generator(smode, smode_inseq, flash_size, no_verify, to_json, to_sag, to_src, to_inc, to_csv, inpt)
    ptc.cli_start()

if __name__ == '__main__':
    try:
        main()
    except InputError as e:
        print(e, file=sys.stderr)
        sys.exit(2)
