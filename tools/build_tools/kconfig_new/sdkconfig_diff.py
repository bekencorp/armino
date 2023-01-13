#!/usr/bin/env python
#

import argparse
import sys
import re
import os

def parse_sdkconfig(sdkconfig_path):
    d = dict()
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
                d[k] = v
        else:
            line_key_value = l.split('=')
            if len(line_key_value) != 2:
                continue
            k = line_key_value[0]
            v = line_key_value[1]
            if k[0:7] != "CONFIG_":
                continue
            d[k] = v
    f.close()
    return (d)

def sdkconfig_diff_checkout(sdkconfig_path, old_sdkconfig_path, diff_sdkconfig_path):
    sdkconfig_dict = parse_sdkconfig(sdkconfig_path)
    old_sdkconfig_dict = parse_sdkconfig(old_sdkconfig_path)
    old_diff_sdkconfig_dict = parse_sdkconfig(diff_sdkconfig_path)
    diff_sdkconfig_dict = dict()
    
    for sdkconfig_key,sdkconfig_val in sdkconfig_dict.items():
        for old_sdkconfig_key,old_sdkconfig_val in old_sdkconfig_dict.items():
            if(sdkconfig_key == old_sdkconfig_key) and (sdkconfig_val != old_sdkconfig_val):
                diff_sdkconfig_dict[sdkconfig_key] = sdkconfig_val
                
    old_diff_sdkconfig_dict.update(diff_sdkconfig_dict)
    
    key_del_list = list()
    for diff_sdkconfig_key,diff_sdkconfig_val in old_diff_sdkconfig_dict.items():
        if(diff_sdkconfig_val == 'is not set'):
            key_del_list.append(diff_sdkconfig_key)
    
    for key_del in key_del_list:
        old_diff_sdkconfig_dict.pop(key_del)
    
    diff_sdkconfig_list = list()
    for diff_sdkconfig_key,diff_sdkconfig_val in old_diff_sdkconfig_dict.items():
        if(diff_sdkconfig_val == 'is not set'):
            temp_item = '# {} {}\n'.format(diff_sdkconfig_key, diff_sdkconfig_val)
        else:
            temp_item = '{}={}\n'.format(diff_sdkconfig_key, diff_sdkconfig_val)
        diff_sdkconfig_list.append(temp_item)
    
    with open(diff_sdkconfig_path, 'w', encoding='utf-8') as f:
        for diff_sdkconfig_line in diff_sdkconfig_list:
            f.write(diff_sdkconfig_line)
    
    f.close()

def main():
    parser = argparse.ArgumentParser(description='sdkconfig_diff - diff checking between sdkconfig and sdkconfig.old')
    parser.add_argument(
        '--sdkconfig',
        help='absolute path of sdkconfig',
        default=None,
        dest='sdkconfig')
    
    parser.add_argument(
        '--old_sdkconfig',
        help='absolute path of sdkconfig.old',
        default=None,
        dest='old_sdkconfig')
        
    parser.add_argument(
        '--diff_sdkconfig',
        help='absolute path of sdkconfig.diff',
        default=None,
        dest='diff_sdkconfig')
    
    args = parser.parse_args()
    
    if args.sdkconfig:
        if os.path.exists(args.sdkconfig) == False:
            print(f'{args.sdkconfig} not exists')
            exit(1)
    else:
        print(f'Missing sdkconfig')
        exit(1)
        
    if args.old_sdkconfig:
        if os.path.exists(args.old_sdkconfig) == False:
            print(f'{args.old_sdkconfig} not exists')
            exit(1)
    else:
        print(f'Missing old sdkconfig')
        
    if not args.diff_sdkconfig:
        print(f'Missing diff sdkconfig')
        
    sdkconfig = os.path.realpath(args.sdkconfig)
    old_sdkconfig = os.path.realpath(args.old_sdkconfig)
    diff_sdkconfig = os.path.realpath(args.diff_sdkconfig)
    
    sdkconfig_diff_checkout(sdkconfig, old_sdkconfig, diff_sdkconfig)
    

if __name__ == "__main__":
    main()