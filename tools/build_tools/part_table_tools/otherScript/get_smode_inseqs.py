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

workspace = "%s"%(sys.path[0])
workspace_config = os.path.abspath("%s/../config"%(workspace))
def get_smode_inseqs(inseq_key):
    if not os.path.exists("%s/inseqs.json"%(workspace_config)):
        cmd_script = os.path.abspath("%s/../gen_bk7256partitions.py"%(workspace))
        cmd_args = os.path.abspath("%s/../bk7256Partitions.csv"%(workspace))
        cmd_args += " --smode"
        cmd_args += " --smode-inseq=0"
        cmd = '%s %s'%(cmd_script, cmd_args)
        try:
            subprocess.check_call(cmd, shell=True)
        except subprocess.CalledProcessError as e:
            raise RuntimeError('Get smode inseqs failed with exit code %d'%(e.returncode))
    if os.path.exists("%s/inseqs.json"%(workspace_config)):
        with open("%s/inseqs.json"%(workspace_config), 'r') as local_json:
            inseqs_json = json.load(local_json)
        inseqs = inseqs_json['inseqs']
        if not inseq_key.strip() in inseqs:
            space = ' '*4
            error_message = 'Get smode inseqs failed, inseqKey not support!!!\n'
            error_message += 'Support inseqKey:\n'
            for inseq_key in inseqs.keys():
                error_message += '%s%s\n'%(space, inseq_key)
            raise RuntimeError(error_message)
        else:
            inseq = inseqs[inseq_key.strip()]
            res = ','.join(inseq)
            print(res)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        raise RuntimeError('Just accept one argument which must be inseq_key!!!')

    inseq_key = sys.argv[1]
    get_smode_inseqs(inseq_key)