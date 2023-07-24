#!/usr/bin/env python3

import os
import sys
import json
import shutil
import argparse
import copy
import errno
import re

gen_files_path = "%s/../config/gen_files_list.txt"%(sys.path[0])

def add_gen_files_list(file_path):
    files_set=set()
    with open(gen_files_path, 'r') as f:
        files_lines = f.readlines()
        for files_line in files_lines:
            if os.path.exists(files_line.strip()) and os.path.isfile(files_line.strip()):
                files_set.add(files_line.strip())

    files_set.add(os.path.abspath(file_path))
    with open(gen_files_path, 'w') as f:
        for files_line in files_set:
            f.write(files_line + '\n')

if __name__ == '__main__':
    if len(sys.argv) != 2:
        raise RuntimeError('Just accept one argument which must be file_path!!!')

    file_path = sys.argv[1]
    add_gen_files_list(file_path)