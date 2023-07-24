#!/usr/bin/env python3

#TODO - optimize it, should finally remove this packer

import os
import sys
import json
import shutil
import argparse
import copy
import errno
import re

def parse_sdkconfig(sdkconfig_path):
    d = list()
    f = open(sdkconfig_path, 'r')
    for l in f:
        l = ' '.join(l.split())
        line_words = l.split(' ')
        if len(line_words) < 1:
            continue
        if line_words[0] == '#':
            if (len(line_words) != 5):
                continue
            if (line_words[2] == 'is') and (line_words[3] == 'not') and (line_words[4] == 'set'):
                if line_words[1][0:7] != 'CONFIG_':
                    continue
                k = line_words[1]
                v = 'is not set'
                d.append((k,v))
        else:
            line_key_value = l.split('=')
            if len(line_key_value) != 2:
                continue
            k = line_key_value[0]
            v = line_key_value[1]
            if k[0:7] != "CONFIG_":
                continue
            d.append((k,v))
    f.close()
    return (d)

def size_format(size, include_size):
    if include_size:
        for (val, suffix) in [(0x100000, "M"), (0x400, "K")]:
            if size % val == 0:
                return "%d%s" % (size // val, suffix)
    size_str = '%x'%size
    lead_zero = '0'*(8 - len(size_str))
    return "0x%s%s" % (lead_zero, size_str)

def shrink_val(val, need_shrink):
    return int(val*32/34) if need_shrink else val

def parse_int(v):
    for letter, multiplier in [("k", 1024), ("m", 1024 * 1024)]:
        if v.lower().endswith(letter):
            return parse_int(v[:-1]) * multiplier
        return int(v, 0)


def project_general_deal(project_dir, project, json_src, config):
    need_deal = False
    start_addr = 0

    config_list = parse_sdkconfig(config)
    config_keys = [ck[0] for ck in config_list]
    dup_config_keys = set(ck for ck in config_keys if config_keys.count(ck) > 1)
    if len(dup_config_keys) != 0:
        print("Duplicate config items:\n")
        for dck in dup_config_keys:
            print("\t%s appeared times: %d\n"%(dck, config_keys.count(dck)))
        print("Please check %s to remove duplicate config items\n"%(config))
        raise

    with open(json_src, 'r') as local_json:
        config_json = json.load(local_json)
    for sec in config_json['section']:
        if sec['partition'] == 'app1':
            need_deal = True
            start_addr = shrink_val(parse_int(sec["start_addr"]), True)

    if need_deal:
        config_temp_list = list()

        depends_config_keys = ['CONFIG_DUAL_CORE', 'CONFIG_MASTER_CORE', 'CONFIG_SLAVE_CORE_OFFSET']
        for ck,cv in config_list:
            key_searched = False
            for dck in depends_config_keys:
                special_config_key = dck
                if special_config_key == ck:
                    temp_item = '%s=y\n'%(ck)
                    key_searched = True
                    break
                else:
                    if cv == 'is not set':
                        temp_item = '# %s %s\n'%(ck, cv)
                    else:
                        temp_item = '%s=%s\n'%(ck, cv)
            if not key_searched:
                config_temp_list.append(temp_item)

        for dck in depends_config_keys:
            temp_item = '%s=y\n'%(dck)
            config_temp_list.append(temp_item)

        for index in range(len(config_temp_list)):
            if config_temp_list[index] == 'CONFIG_SLAVE_CORE_OFFSET=y\n':
                config_temp_list[index] = 'CONFIG_SLAVE_CORE_OFFSET=%d\n'%(start_addr)

        with open(config, 'w', encoding='utf-8') as f:
            for config_line in config_temp_list:
                f.write(config_line)
        f.close()

def project_bk7256_configa_deal(project_dir, project, json_src, config):
    if project != "bk7256_configa":
        return
    print("============>Project %s deal...\n"%(project))
    with open(json_src, 'r') as local_json:
        config_json = json.load(local_json)
    config_json['magic'] = config_json['magic']
    config_json['version'] = config_json['version']
    config_json['count'] = 2
    configa_deal_sec = list()
    for sec in config_json['section']:
        if sec['partition'] == 'bootloader':
            configa_deal_sec.append(sec)
        if sec['partition'] == 'app':
            configa_deal_sec.append(sec)
    for sec in configa_deal_sec:
        sec['firmware'] = sec['firmware']
        sec['version'] = sec['version']
        sec['partition'] = sec['partition']
        sec['start_addr'] = size_format(shrink_val(parse_int(sec["start_addr"]), True), False)
        sec['size'] = size_format(shrink_val(parse_int(sec["size"]), True), True)
        if sec['partition'] == 'app':
            sec['firmware'] = 'app_ab.bin'
            sec['partition'] = 'app'
    config_json['section'] = configa_deal_sec

    json_dest = '%s/config/configurationab.json'%(project_dir)
    config_json = json.dumps(config_json, sort_keys=False, indent=4)
    with open(json_dest, 'w') as f:
        f.write(config_json)

    project_general_deal(project_dir, project, json_src, config)

def project_bk7256_configb_deal(project_dir, project, json_src, config):
    if project != "bk7256_configb":
        return
    print("============>Project %s deal...\n"%(project))
    with open(json_src, 'r') as local_json:
        config_json = json.load(local_json)
    config_json['magic'] = config_json['magic']
    config_json['version'] = config_json['version']
    config_json['count'] = 2
    configb_deal_sec = list()
    for sec in config_json['section']:
        if sec['partition'] == 'bootloader':
            configb_deal_sec.append(sec)
        if sec['partition'] == 'app1':
            configb_deal_sec.append(sec)
    for sec in configb_deal_sec:
        sec['firmware'] = sec['firmware']
        sec['version'] = sec['version']
        sec['partition'] = sec['partition']
        sec['start_addr'] = size_format(shrink_val(parse_int(sec["start_addr"]), True), False)
        sec['size'] = size_format(shrink_val(parse_int(sec["size"]), True), True)
        if sec['partition'] == 'app1':
            sec['firmware'] = 'app_ab.bin'
            sec['partition'] = 'app'
    config_json['section'] = configb_deal_sec

    json_dest = '%s/config/configurationab.json'%(project_dir)
    config_json = json.dumps(config_json, sort_keys=False, indent=4)
    with open(json_dest, 'w') as f:
        f.write(config_json)

    project_general_deal(project_dir, project, json_src, config)

def main():
    special_project_deal_funcs = {
        "bk7256_configa": project_bk7256_configa_deal,
        "bk7256_configb": project_bk7256_configb_deal,
    }

    parser = argparse.ArgumentParser(description='Deal with special json files from special project')

    parser.add_argument(
        '--project-dir',
        help='Special project directory',
        default=None)

    parser.add_argument(
        '--project',
        help='Special project name',
        default=None)

    parser.add_argument(
        '--json-src',
        help='Json file need deal with special project',
        default=None)

    parser.add_argument(
        '--config',
        help='Config file need deal with all project',
        default=None)

    args = parser.parse_args()

    project_dir = args.project_dir
    project = args.project
    json_src = args.json_src
    config = args.config

    if project in special_project_deal_funcs:
        special_project_deal_func = special_project_deal_funcs[project]
        special_project_deal_func(project_dir, project, json_src, config)
    else:
        print("============>Project %s need not to special deal..."%(project))
        project_general_deal(project_dir, project, json_src, config)

if __name__ == "__main__":
    main()