#!/usr/bin/env python3

import os
import sys
import json
import shutil
import argparse
import copy
import errno
import re
import compute_files_hash as cfh

workspace = "%s/workspace"%(sys.path[0])
workspace_refer = os.path.abspath("%s/../dependsFiles"%(sys.path[0]))

depend_files_dict = {
    'files_count': 1,
    'files_list': [
        {
            'file_name': 'flash_partition.h',
            'file_dir': 'include/driver',
        }
    ],
}

def ensure_directory(dir):
    if not os.path.exists(dir):
        try:
            os.makedirs(dir)
        except OSError as exc:
            if exc.errno != errno.EEXIST:
                raise

def depends_files_check(armino_path):
    global depend_files_dict
    global workspace
    global workspace_refer
    ensure_directory(workspace)
    for file_info in depend_files_dict['files_list']:
        file_name = file_info['file_name']
        file_dir = file_info['file_dir']
        file_src = os.path.join(armino_path, file_dir, file_name)
        file_dest = os.path.join(workspace, file_name)
        if os.path.isfile(file_src):
            shutil.copy(file_src, file_dest)
    tree = cfh.Node(workspace)
    tree_refer = cfh.Node(workspace_refer)
    shutil.rmtree(workspace)
    if tree.node_hash != tree_refer.node_hash:
        raise RuntimeError('Part table tools check dependsFiles: failed!!!')
    else:
        print("Part table tools check dependsFiles: successful!!!\n")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        raise RuntimeError('Just accept one argument which must be armino_path!!!')

    armino_path = sys.argv[1]
    depends_files_check(armino_path)