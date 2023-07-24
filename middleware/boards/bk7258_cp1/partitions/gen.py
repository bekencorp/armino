#!/usr/bin/env python3

import os
import subprocess
import sys
import logging
import getpass

from argparse import _MutuallyExclusiveGroup
from genericpath import exists
import click
from click_option_group import optgroup,RequiredMutuallyExclusiveOptionGroup

armino_soc = ''

def run_cmd(cmd):
	p = subprocess.Popen(cmd, shell=True)
	ret = p.wait()
	if (ret):
		logging.error(f'failed to run "{cmd}"')
		exit(1)

def install_configs(cfg_dir, install_dir):
	global armino_soc
	logging.debug(f'install configs from: {cfg_dir}')
	logging.debug(f'to: {install_dir}')
	if os.path.exists(f'{cfg_dir}') == False:
            return

	if os.path.exists(f'{cfg_dir}/key') == True:
		run_cmd(f'cp -r {cfg_dir}/key/* {install_dir} 2>/dev/null || :')

	if os.path.exists(f'{cfg_dir}/partitions') == True:
		run_cmd(f'cp -r {cfg_dir}/partitions/* {install_dir} 2>/dev/null || :')

	if os.path.exists(f'{install_dir}/{armino_soc}.partitions') == True:
		run_cmd(f'cp {install_dir}/{armino_soc}.partitions {install_dir}/partitions 2>/dev/null || :')


def pack(tools_dir, base_cfg_dir, prefered_cfg_dir, gen_dir, soc):
	global armino_soc
	logging.debug(f'tools_dir={tools_dir}')
	logging.debug(f'base_cfg_dir={base_cfg_dir}')
	logging.debug(f'prefered_cfg_dir={prefered_cfg_dir}')
	logging.debug(f'soc={soc}')
	logging.debug(f'pack={pack}')

	SH_SEC_TOOL = f'{tools_dir}/sh_sec_tools/secure_boot_tool'
	BK_IMG_TOOL = f'{tools_dir}/beken_imgtools/image.py'
	MCUBOOT_IMG_TOOL = f'{tools_dir}/mcuboot_tools'

	BASE_CFG_DIR = base_cfg_dir
	armino_soc = soc

	logging.debug(f'Create temporary _build')
	_BUILD_DIR=f'{gen_dir}/../_gen'
	run_cmd(f'mkdir -p {_BUILD_DIR}')

	saved_cur_dir = os.getcwd()
	os.chdir(_BUILD_DIR)
	logging.debug(f'cd {_BUILD_DIR}')

	install_configs(BASE_CFG_DIR, _BUILD_DIR)
	install_configs(prefered_cfg_dir, _BUILD_DIR)
	install_configs(f'{prefered_cfg_dir}/{soc}', _BUILD_DIR)

	logging.debug(f'partition pre-processing')
	run_cmd(f'{BK_IMG_TOOL} partition -j {_BUILD_DIR}/partitions -s {_BUILD_DIR}/sys_partitions --genhdr --genbl1 --genbl2 --tools_dir {tools_dir}')
	run_cmd(f'cp {_BUILD_DIR}/partitions_partition.h {gen_dir}/partitions_gen.h')

@click.group()
@click.version_option(version='1.0.0.0')
def gen():
	"""BK7258 post-build scripts to pack the binaries."""
	pass

@gen.command("pack")
@click.option("-t", "--tools_dir", type=click.Path(exists=True, dir_okay=True),required=True, help="tools directory.")
@click.option("-c", "--base_cfg_dir", type=click.Path(exists=True, dir_okay=True),required=True, help="base configuration directory.")
@click.option("-p", "--prefered_cfg_dir", type=click.Path(exists=False, dir_okay=True),required=True, help="prefered configuration directory.")
@click.option("-s", "--soc", type=str, default = "7258", show_default = True, help="chip name.")
@click.option("-g", "--gen_dir", type=click.Path(exists=True, dir_okay=True),required=True, help="Code generated dir.")
def pack_bin(tools_dir, base_cfg_dir, prefered_cfg_dir, gen_dir, soc):
	"""Pack armino binaries."""
	pack(tools_dir, base_cfg_dir, prefered_cfg_dir, gen_dir, soc)
	pass

if __name__ == '__main__':
	logging.basicConfig()
	logging.getLogger().setLevel(logging.DEBUG)
	gen()
