#!/usr/bin/env python3

import logging
from argparse import _MutuallyExclusiveGroup
from genericpath import exists
import click
from click_option_group import optgroup,RequiredMutuallyExclusiveOptionGroup

import struct
from scripts import create
from scripts import merge_tlv
from scripts import partition
from scripts.partition import *
from scripts import revert
from scripts import ascii
from scripts import dump
from scripts import copy
from scripts import modify
from scripts import check_sha384
#from scripts import ota_packager
from scripts import crc
from scripts import encrypt
from scripts import xts_aes
from scripts import generator

@click.group()
@click.version_option(version='1.0.0.0')
def cli():
    """This script showcases multi-scripts on operating flash."""
    pass

@cli.command("create")
@click.option("-s", "--size", type=int,required=True, help="The outfile size.")
@click.option("-f", "--fill", type=int, default=0, help="The outfile content.", show_default=True)
@click.option("-o", "--outfile", type=click.File("wb+"), default="create.bin", help="The outfile name.", show_default=True)
def create_bin(outfile, size, fill):
    """create a new bin file from fill data."""
    create.create_fill_img(outfile, size, fill)
    logging.debug(f"create success")
# endregion

@cli.command("merge_tlv")
@click.option("-j", "--json_file", type=click.Path(exists=True, dir_okay=False),required=True, help="input a json file.")
@click.option("-o", "--outfile", type=click.Path(exists=False, dir_okay=False), required=True, help="The outfile name.")
@click.option("--debug/--no-debug", default=True, help="Enable debug options.")
def merge_tlv_bin(json_file, outfile, debug):
    """merge a new bin file from json file."""
    if (debug == True):
        logging.basicConfig()
        logging.getLogger().setLevel(logging.DEBUG)
    merge_tlv(json_file, outfile)
    logging.debug(f"merge success")

@cli.command("partition")
@click.option("-j", "--partition_json", type=click.Path(exists=True, dir_okay=False),required=True, help="partition config json file.")
@click.option("-s", "--sys_partition_json", type=click.Path(exists=True, dir_okay=False),required=True, help="system predefined partition config json file.")
@click.option("-t", "--tools_dir", type=click.Path(exists=True, dir_okay=True),required=True, help="partition dependency tools dir.")
@click.option("--crc/--no-crc", default=True, help="Add CRC to code.")
@click.option("--encrypt/--no-encrypt", default=False, help="Encrypt code.")
@click.option("--genota/--no-genota", default=False, help="Merge and generate OTA bin.")
@click.option("--genapp/--no-genapp", default=False, help="Merge and generate application bin.")
@click.option("--genall/--no-genall", default=False, help="Merge and generate all bin.")
@click.option("--genhdr/--no-genhdr", default=False, help="Generate partitions constants code.")
@click.option("--genbl1/--no-genbl1", default=False, help="Generate BL1 configurations.")
@click.option("--genbl2/--no-genbl2", default=False, help="Generate BL2 configurations.")
@click.option("--debug/--no-debug", default=True, help="Enable debug option.")

def partition_process(partition_json, sys_partition_json, tools_dir, crc, encrypt, genota, genapp, genall, genhdr, genbl1, genbl2, debug):
    """partitions processing."""
    gencode_only = False
    if (debug == True):
        logging.basicConfig()
        logging.getLogger().setLevel(logging.DEBUG)
 
    if (genapp == False) and (genota == False):
        gencode_only = True

    p = Partitions(partition_json, sys_partition_json, tools_dir, crc, encrypt, gencode_only)

    if (genhdr):
        logging.debug("start to generate flash layout constants ...")
        p.gen_partitions_constants()

    if (genbl1):
        logging.debug("start to generate BL1 configurations ...")
        p.gen_bl1_config()

    if (genbl2):
        logging.debug("start to generate BL1 configurations ...")
        p.gen_bl2_config()

    if (genall):
        p.gen_all_bin()

    if (genapp):
        logging.debug("start to generate application bin ..")
        p.gen_app_bin()

    if (genota):
        logging.debug("start to generate OTA bin ..")
        p.gen_ota_bin()

@cli.command("revert")
@click.option("-i", "--infile", type=click.Path(exists=True, dir_okay=False),required=True, help="input a bin file.")
@click.option("-o", "--outfile", type=click.Path(dir_okay=False),default="revert.bin", help="The outfile name.", show_default=True)
def revert_bin(infile, outfile):
    """revert a new bin file from input file."""
    revert(infile, outfile)
    logging.debug(f"revert success")

@cli.command("ascii")
@click.option("-i", "--infile", type=click.Path(exists=True, dir_okay=False), required=True, help="input a bin file.")
@click.option("-o", "--outfile", type=click.Path(dir_okay=False),default="ascii.bin", help="The outfile name.", show_default=True)
@click.option("-s", "--size", type=int, default=1, help="the size of generate ascii stream.", show_default=True)
def ascii_bin(infile, outfile, size):
    """ascii a new bin file from input file."""
    ascii(infile, outfile, size)
    logging.debug(f"ascii success")

@cli.command("dump")
@click.option("-i", "--infile", type=click.Path(exists=True, dir_okay=False), required=True, help="input a bin file.")
@click.option("-o", "--outfile", type=click.Path(dir_okay=False),default="dump.bin", help="The outfile name.", show_default=True)
def dump_bin(infile, outfile):
    """dump a new bin file from input file."""
    dump(infile, outfile)
    logging.debug(f"dump success")

@cli.command("copy")
@click.option("-s", "--srcfile", type=click.Path(exists=True, dir_okay=False),required=True, help="input a source bin file.")
@click.option("-d", "--dstfile", type=click.Path(exists=True, dir_okay=False),required=True, help="input a destination bin file.")
@click.option("-o", "--outfile", type=click.Path(dir_okay=False),default="out.bin", help="The outfile name.", show_default=True)
@click.option("-so", "--so", type=str, default=0, help="the src offset, 0xFFFFFFFF indicates revert copy", show_default=True)
@click.option("-do", "--do", type=str, default=0, help="the dst offset, 0xFFFFFFFF indicates revert copy", show_default=True)
@click.option("-sz", "--sz", type=str, default=0, help="the size to be copied, 0 indicates copy all", show_default=True)
def copy_bin(srcfile, dstfile, outfile, so, do, sz):
    """copy specified content from src to dst bin"""
    copy(srcfile, dstfile, outfile, so, do, sz)
    logging.debug(f"copy success")

@cli.command("modify")
@click.option("-i", "--infile", type=click.Path(exists=True, dir_okay=False), required=True, help="input a bin file.")
@click.option("-o", "--outfile", type=click.Path(dir_okay=False),default="modify.bin", help="The outfile name.", show_default=True)
@click.option("-a", "--address", type=int, default=0, help="the offset", show_default=True)
def modify_bin(infile, outfile, address):
    """modify a new bin file from input file."""
    modify(infile, outfile, address)
    logging.debug(f"modify success")

@cli.command("check_sha384")
@click.option("-i", "--infile", type=click.Path(exists=True, dir_okay=False),required=True, help="input a json file.")
def check_sha384_bin(infile):
    """check_sha384 from input file."""
    check = check_sha384(infile)
    if(check.check_json_data() == 1):
        logging.debug(f"check_sha384 success")
    else:
        logging.debug(f"check_sha384 fail")

'''
#On build server21, package Crypto has compitable issue, disable it firstly!
#Enable it after we fix the Crypto issue.
@cli.command("ota_packager")
@click.option("-i", "--infile", type=click.Path(exists=True, dir_okay=False),required=True, help="input a bin file.")
@click.option("-c", "--compress", type=click.Choice(['0', '1', '2', '3']), default="1", help="choice a mode of compress , 00=None ;01=gzip ;02=quicklz ;03=fastlz.", show_default=True)
@click.option("-e", "--encrypt", type=click.Choice(['0', '2']), default="2", help="choice a mode of encrypt , 00=None ;02=aes256", show_default=True)
@click.option("-k", "--keyword", type=str, default="0123456789ABCDEF0123456789ABCDEF", help="input keyword value", show_default=True)
@click.option("-iv", "--iv", type=str, default="0123456789ABCDEF", help="input iv value", show_default=True)
@click.option("-fn", "--firmware_name", type=str, default="app", help="input firmware_name value", show_default=True)
@click.option("-fv", "--firmware_version", type=str, default="2", help="input firmware_name value", show_default=True)
@click.option("-o", "--outfile", type=click.Path(dir_okay=False),default="ota_packager.rbl", help="The outfile name.", show_default=True)
def ota_packager_bin(infile, compress, encrypt, keyword, iv, firmware_name, firmware_version, outfile):
    """generate a new .rbl file from a bin file and key."""

    ota_packager(infile, compress, encrypt, keyword, iv,
                 firmware_name, firmware_version, outfile)
    logging.debug(f"ota_packager success")
'''

@cli.command("crc")
@click.option("-i", "--infile", type=click.Path(exists=True, dir_okay=False), required=True, help="input a bin file.")
@click.option("-o", "--outfile", type=click.Path(dir_okay=False),default="outfile_crc.bin", help="The crc outfile name.", show_default=True)
def crc_bin(infile, outfile):
    """generate a crc file from the origin file."""
    crc(infile, outfile)

@cli.command("encrypt")
@click.option("-i", "--infile", type=click.Path(exists=True, dir_okay=False), required=True, help="input a bin file.")
@click.option("-k", "--keywords", nargs=4, type=str, help="input 4 keywords,such as '0 0 0 0'")
@click.option("-s","--start_address",type=str,default = "0",show_default = True,help="The start_address of encryption.")
@click.option("--un_skip/--skip",default=True,help="Makes a checkout un_skip or skip keyword,such as 'hex = bk7231n'.  un_skip by default.",)
@click.option("-o", "--outfile", type=click.Path(dir_okay=False),default="outfile_enc.bin", show_default=True,help="The encrypt outfile name.")
@click.option("-d", "--debug", is_flag=True,help="generate *.cpr & *.out files.")
def encrypt_bin(infile, keywords, start_address,un_skip,outfile,debug):
    """generate a encrypt file from the origin file and keywords"""
    enc = encrypt(infile, keywords, start_address,un_skip,outfile,debug)
    enc.func_enc()
    
@cli.group("aes")
def aes():
    """xts_aes,include generate keyfile and xts-aes to encrypt bin file ."""
@aes.command("genkey")
@click.option("-v", "--version", type=str, default="1", help="keyfile version.")
@click.option("-a","--aes_cbc",is_flag=True,default=True,show_default=True,help="create ota_key and ota_iv value",)
@click.option("-o", "--outfile", type=click.Path(dir_okay=False),default="genkey.txt", show_default=True,help="The generate key outfile name.")
def gen_bin(version,aes_cbc,outfile):
    """generate a key file ,include xts_aes / OTA_key / OTA_IV"""
    enc = generator(version,aes_cbc,outfile)
    enc.func_enc()

@aes.command("aes")
@click.option("-i", "--infile", type=click.Path(exists=True, dir_okay=False), required=True, help="input a bin file.")
@click.option("-a","--all_quick",is_flag=True,help="quickly generate xts_aes bin file.")
@click.option("-s","--start_address",type=str,default = "0",show_default = True,help="The start_address of encryption.")
@click.option("-o", "--outfile", type=click.Path(dir_okay=False),default="xts_aes.bin", show_default=True,help="The xts_aes outfile name.")
@optgroup.group('source data options',cls=RequiredMutuallyExclusiveOptionGroup,help='choice keywords or keyfile')
@optgroup.option("-k", "--keywords", type=str,help="input keywords,ciphertext or plaintext .")
@optgroup.option("-f", "--keyfile", type=click.Path(exists=True, dir_okay=False), help="input keyfile path.")
def aes_bin(infile,all_quick,start_address,keywords,keyfile,outfile):
    """encrypt bin file from the origin file and keywords or keyfile"""
    enc = xts_aes(infile,all_quick,start_address,keywords,keyfile,outfile)
    enc.func_enc()

if __name__ == '__main__':
    logging.basicConfig()
    logging.getLogger().setLevel(logging.INFO)
    cli()
