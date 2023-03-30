#!/usr/bin/env python3

import logging
from argparse import _MutuallyExclusiveGroup
from genericpath import exists
import click
from click_option_group import optgroup,RequiredMutuallyExclusiveOptionGroup

from scripts.parse import *
from scripts.gen import *

@click.group()
@click.version_option(version='1.0.0.0')
def gen():
	"""This tool is used to generate beken LL headers."""
	pass

@gen.command("gen")
@click.option("-f", "--infile", type=click.Path(exists=True, dir_okay=False),required=True, help="address mapping file name.")
@click.option("-m", "--module", type=str, default="system", help="module name", show_default=True)
@click.option("-s", "--start", type=int, default=0, help="start register.", show_default=True)
@click.option("-e", "--end", type=int, default=-1, help="end register.", show_default=True)
@click.option("-p", "--prefix", type=str, default='r', help="Register prefix.", show_default=True)
@click.option("--debug/--no-debug", default=False, help="Enable debug")
@click.option("--reghex/--no-reghex", default=True, help="Register number as hex format")
def gen_hdr(infile, module, start, end, prefix, debug, reghex):
	"""generate ll headers."""
	logging.debug(f"create success")

	p = Regparse(infile, module, start, end, prefix, debug, reghex)
	p.parse_excel()
	g = Gen(p)
	g.gen_struct()
	g.gen_dump()
	g.gen_ll()
	g.gen_reg()
	g.gen_hw()

if __name__ == '__main__':
	logging.basicConfig()
	logging.getLogger().setLevel(logging.INFO)
	gen()
