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

def get_config_val(config, config_item):
    with open(config, 'r') as cfg:
        cfgls = cfg.readlines()
    config_val = 'n'
    for cfgl in cfgls:
        cfglr = cfgl.strip().split('=')
        if config_item == cfglr[0].strip():
            config_val = cfglr[1].strip()
        if cfgl == '# %s is not set'%(config_item):
            config_val = 'n'
    print('%s'%(config_val))

def main():
    parser = argparse.ArgumentParser(description='Deal with config files, get config_value of config_item')

    parser.add_argument(
        '--config',
        help='Config file path',
        default=None)

    parser.add_argument(
        '--config-item',
        help='Config item',
        default='CONFIG_OVERRIDE_FLASH_PARTITION')

    args = parser.parse_args()

    config = args.config
    config_item = args.config_item

    get_config_val(config, config_item)

if __name__ == "__main__":
    main()