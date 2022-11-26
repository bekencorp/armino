#!/usr/bin/env python3

import os
import logging

class ascii:
    def __init__(self, infile, outfile, size):

        logging.info(f'generate asii stream, size={size}')
        infile_len = os.path.getsize(infile)
        with open(outfile, 'wt+') as of:
            with open(infile, 'rb') as f:
                j = 0
                a_byte_line = ''
                for i in range(infile_len):
                    a_byte = f.read(1)
                    a_byte_hex = a_byte.hex()
                    a_byte_int = int(a_byte_hex, base=16)
                    a_byte_line = ''.join(['%02x%s' % (a_byte_int, a_byte_line)])
                    if (j == (size - 1)):
                        j = 0
                        a_byte_line = ''.join(['%s' % (a_byte_line), '\n'])
                        of.write(a_byte_line)
                        a_byte_line = ''
                    else:
                        j = j + 1
                if (j != 0):
                    a_byte_line = ''.join(['%s' % (a_byte_int), '\n'])
                    of.write(a_byte_line)
