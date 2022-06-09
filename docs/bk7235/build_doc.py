#!/usr/bin/env python3

import os
import subprocess
import sys

def run_cmd(cmd):
	p = subprocess.Popen(cmd, shell=True)
	ret = p.wait()
	
def build_lan_doc(doc_path, lan):
	lan_dir = f'{doc_path}/{lan}'
	os.chdir(lan_dir)
	run_cmd('make html')
	run_cmd(f'cp -r _build ../build/{lan}')
	os.chdir(doc_path)

def build_doc(clean):
	cur_dir_is_docs_dir = True
	saved_dir = os.getcwd()
	if 'ARMINO_PATH' in os.environ:
		armino_path = os.environ['ARMINO_PATH']
		DOCS_PATH = f"{armino_path}/docs"
		cur_path = os.getcwd()
		if cur_path != DOCS_PATH:
			cur_dir_is_docs_dir = False
		print(f'DOCS_PATH set to {DOCS_PATH}')
	else:
		print('ARMINO_PATH env is not set, set DOCS_PATH to current dir')
		DOCS_PATH =  os.getcwd()

	build_dir = f'{DOCS_PATH}/build'
	run_cmd(f'rm -rf {build_dir}')
	if (clean):
		run_cmd(f'rm -rf {DOCS_PATH}/en/_build')
		run_cmd(f'rm -rf {DOCS_PATH}/en/xml')
		run_cmd(f'rm -rf {DOCS_PATH}/en/xml_in')
		run_cmd(f'rm -rf {DOCS_PATH}/en/man')
		run_cmd(f'rm -rf {DOCS_PATH}/zh_CN/_build')
		run_cmd(f'rm -rf {DOCS_PATH}/zh_CN/xml')
		run_cmd(f'rm -rf {DOCS_PATH}/zh_CN/xml_in')
		run_cmd(f'rm -rf {DOCS_PATH}/zh_CN/man')
		run_cmd(f'rm -rf {DOCS_PATH}/__pycache__')
		return;

	run_cmd(f'mkdir {build_dir}')

	build_lan_doc(DOCS_PATH, 'zh_CN')
	build_lan_doc(DOCS_PATH, 'en')

	if cur_dir_is_docs_dir == False:
		move_doc_html(DOCS_PATH, cur_path)
	os.chdir(saved_dir)

def main(argv):
	if (len(argv) > 1 and argv[1] == "clean"):
		build_doc(True)
	else:
		build_doc(False)

if __name__ == "__main__":
	main(sys.argv)
