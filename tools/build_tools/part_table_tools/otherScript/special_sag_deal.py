#!/usr/bin/env python3

import os
import sys
import json
import shutil
import subprocess
import argparse
import copy
import errno
import re

def application_sag_deal(armino_soc, refer_sag, src_sag, dest_sag):
    with open(refer_sag, 'r') as rsf:
        rsfls = rsf.readlines()
    with open(src_sag, 'r') as ssf:
        ssfls = ssf.readlines()

    for rsfl in rsfls:
        if re.match(r'^application.*',rsfl.lower()):
            rsfl_index = rsfls.index(rsfl)
            break
    for ssfl_0 in ssfls:
        if re.match(r'^define cpu0_flash_start.*',ssfl_0.lower()):
            ssfl_0_index = ssfls.index(ssfl_0)
            break
    for ssfl_1 in ssfls:
        if re.match(r'^define cpu0_flash_size.*',ssfl_1.lower()):
            ssfl_1_index = ssfls.index(ssfl_1)
            break
    rsfr = rsfls[rsfl_index].strip().split(' ')
    cpu0_flash_start = rsfr[1]
    cpu0_flash_size = rsfr[2]
    ssfls[ssfl_0_index] = 'define cpu0_flash_start '.upper() + cpu0_flash_start + '\n'
    ssfls[ssfl_1_index] = 'define cpu0_flash_size '.upper() + cpu0_flash_size + '\n'

    with open(dest_sag, 'w') as dsf:
        for ssfl in ssfls:
            dsf.write(ssfl)

def application1_sag_deal(armino_soc, refer_sag, src_sag, dest_sag):
    with open(refer_sag, 'r') as rsf:
        rsfls = rsf.readlines()
    with open(src_sag, 'r') as ssf:
        ssfls = ssf.readlines()

    for rsfl in rsfls:
        if re.match(r'^application.*',rsfl.lower()):
            rsfl_index = rsfls.index(rsfl)
            break
    for ssfl_0 in ssfls:
        if re.match(r'^define cpu1_flash_start.*',ssfl_0.lower()):
            ssfl_0_index = ssfls.index(ssfl_0)
            break
    for ssfl_1 in ssfls:
        if re.match(r'^define cpu1_flash_size.*',ssfl_1.lower()):
            ssfl_1_index = ssfls.index(ssfl_1)
            break
    rsfr = rsfls[rsfl_index].strip().split(' ')
    cpu1_flash_start = rsfr[1]
    cpu1_flash_size = rsfr[2]
    ssfls[ssfl_0_index] = 'define cpu1_flash_start '.upper() + cpu1_flash_start + '\n'
    ssfls[ssfl_1_index] = 'define cpu1_flash_size '.upper() + cpu1_flash_size + '\n'

    with open(dest_sag, 'w') as dsf:
        for ssfl in ssfls:
            dsf.write(ssfl)

def main():
    special_sag_deal_funcs = {
        "application.sag": application_sag_deal,
        "application1.sag": application1_sag_deal,
    }

    parser = argparse.ArgumentParser(description='Deal with special json files from special project')

    parser.add_argument(
        '--armino-soc',
        help='Special armino soc',
        default=None)

    parser.add_argument(
        '--refer-sag',
        help='Reference sag file',
        default=None)

    parser.add_argument(
        '--src-sag',
        help='Source sag file',
        default=None)

    parser.add_argument(
        '--dest-sag',
        help='Dest sag file',
        default=None)

    args = parser.parse_args()

    armino_soc = args.armino_soc
    refer_sag = args.refer_sag
    src_sag = args.src_sag
    dest_sag = args.dest_sag

    special_sag = os.path.basename(refer_sag)
    if special_sag in special_sag_deal_funcs:
        special_sag_deal_func = special_sag_deal_funcs[special_sag]
        special_sag_deal_func(armino_soc, refer_sag, src_sag, dest_sag)
    else:
        pass

if __name__ == "__main__":
    main()