#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

import argparse
import logging
import os
import re

from kconfiglib import Kconfig
import menuconfig
import guiconfig

# NOTE: in_component_label is related with Kconfig menu prompt.
in_component_label = 'TF-M component configs'

def parse_args():
    parser = argparse.ArgumentParser(description=\
            'TF-M Kconfig tool generates CMake configurations and header file \
             component configurations. Terminal UI and GUI can help quickly \
             configurate TF-M build options.')

    parser.add_argument(
        '-k', '--kconfig-file',
        dest = 'kconfig_file',
        required = True,
        help = 'The Top-level Kconfig file'
    )

    parser.add_argument(
        '-o', '--output-path',
        dest = 'output_path',
        required = True,
        help = 'The output file folder'
    )

    parser.add_argument(
        '-u', '--ui',
        dest = 'ui',
        required = False,
        default = None,
        choices = ['gui', 'tui'],
        help = 'Which config UI to display'
    )

    parser.add_argument(
        '-p', '--platform-path',
        dest = 'platform_path',
        required = False,
        help = 'The platform path which contains specific Kconfig and defconfig files'
    )

    args = parser.parse_args()

    return args

def generate_file(dot_config):
    '''
    The .config file is the generated result from Kconfig files. It contains
    the set and un-set configs and their values.

    TF-M splits the configs to build options and component options. The former
    will be written into CMake file. The latter are all under a menu which has
    the prompt which contains in_component_label. These configs will be written
    into header file.
    '''
    cmake_file, header_file = 'project_config.cmake', 'project_config.h'
    in_component_options, menu_start = False, False

    '''
    The regular expression is used to parse the text like:
        - CONFIG_FOO=val
        - # CONFIG_FOO is not set
    The 'FOO' will be saved into the name part of groupdict, and the 'val' will
    be saved into the 'val' part of groupdict.
    '''
    pattern_set = re.compile('CONFIG_(?P<name>[A-Za-z|_|0-9]*)=(?P<val>\S+)')
    pattern_not_set = re.compile('# CONFIG_(?P<name>[A-Za-z|_|0-9]*) is not set')

    with open(cmake_file, 'w') as f_cmake, open(header_file, 'w') as f_header, \
                                           open(dot_config, 'r') as f_config:

        for line in f_config:
            '''
            Extract in_component_options flag from start line and end line
            which has the in_component_label.
            '''
            if line.startswith('# ' + in_component_label):
                in_component_options = True
                continue
            if line.startswith('end of ' + in_component_label):
                in_component_options =False
                continue

            '''
            Extract the menu prompt. It forms like:
                ...
                    #
                # FOO Module
                #
                ...
            Here get the text 'FOO Module', and write it as comment in
            output files.
            '''
            if line == '#\n' and not menu_start:
                menu_start = True
                continue
            if line == '#\n' and menu_start:
                menu_start = False
                continue

            # Write the menu prompt.
            if menu_start and not in_component_options:
                f_cmake.write('\n# {}\n'.format(line[2:-1]))
                continue
            if menu_start and in_component_options:
                f_header.write('\n/* {} */\n'.format(line[2:-1]))
                continue

            '''
            Parse dot_config text by regular expression and get the config's
            name, value and type. Then write the result into CMake and
            header files.

            CONFIG_FOO=y
                - CMake:  set(FOO ON CACHE BOOL '')
                - Header: #define FOO 1
            CONFIG_FOO='foo'
                - CMake:  set(FOO 'foo' CACHE STRING '')
                - Header: #define FOO 'foo'
            # CONFIG_FOO is not set
                - CMake:  set(FOO OFF CACHE BOOL '')
                - Header: #define FOO 0
            '''
            name, cmake_type, cmake_val, header_val = '', '', '', ''

            # Search the configs set by Kconfig.
            ret = pattern_set.match(line)
            if ret:
                name = ret.groupdict()['name']
                val = ret.groupdict()['val']
                if val == 'y':
                    cmake_val = 'ON'
                    cmake_type = 'BOOL'
                    header_val = '1'
                else:
                    cmake_val = val
                    cmake_type = 'STRING'
                    header_val = val

            # Search the not set configs.
            ret = pattern_not_set.match(line)
            if ret:
                name = ret.groupdict()['name']
                cmake_val = 'OFF'
                cmake_type = 'BOOL'
                header_val = '0'

            # Write the result into cmake and header files.
            if name and not in_component_options:
                f_cmake.write('set({:<45} {:<15} CACHE {:<6} "")\n'.
                              format(name, cmake_val, cmake_type))
            if name and in_component_options:
                f_header.write('#define {:<45} {}\n'.format(name, header_val))

    logging.info('TF-M build configs saved to \'{}\''.format(cmake_file))
    logging.info('TF-M component configs saved to \'{}\''.format(header_file))

if __name__ == '__main__':
    logging.basicConfig(format='[%(filename)s] %(levelname)s: %(message)s',
                        level = logging.INFO)

    args = parse_args()

    # dot_config has a fixed name. Do NOT rename it.
    dot_config = '.config'
    def_config = ''
    mtime_prv = 0

    if args.platform_path:
        platform_abs_path = os.path.abspath(args.platform_path)

        if not os.path.exists(platform_abs_path):
            logging.error('Platform path {} doesn\'t exist!'.format(platform_abs_path))
            exit(1)

        def_config = os.path.join(platform_abs_path, 'defconfig')

        # Pass environment variable to Kconfig to load extra Kconfig file.
        os.environ['PLATFORM_PATH'] = platform_abs_path

    # Load Kconfig file. kconfig_file is the root Kconfig file. The path is
    # input by users from the command.
    tfm_kconfig = Kconfig(args.kconfig_file)

    if not os.path.exists(args.output_path):
        os.mkdir(args.output_path)

    # Change program execution path to the output folder path.
    os.chdir(args.output_path)

    if os.path.exists(dot_config):
        # Load .config which contains the previous configurations.
        mtime_prv = os.stat(dot_config).st_mtime
        logging.info(tfm_kconfig.load_config(dot_config))
    elif os.path.exists(def_config):
        # Load platform specific defconfig if exists.
        logging.info(tfm_kconfig.load_config(def_config))

    # UI options
    if args.ui == 'tui':
        menuconfig.menuconfig(tfm_kconfig)
    elif args.ui == 'gui':
        guiconfig.menuconfig(tfm_kconfig)
    else:
        # Save .config if UI is not created.
        # The previous .config will be saved as .config.old.
        logging.info(tfm_kconfig.write_config(dot_config))

    # Generate output files if .config has been changed.
    if os.path.exists(dot_config) and os.stat(dot_config).st_mtime != mtime_prv:
        generate_file(dot_config)
