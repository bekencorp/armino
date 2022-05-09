#!/usr/bin/python3

import argparse
import sys
import re
import os

size_tools = ""
data = ""


def get_size(m):
    global data, size_tools
    file_path = m
    cmd = size_tools+" " + file_path
    num_size = 0
    numbers = [0]*16
    with os.popen(cmd, "r") as lines:
        for line in lines:
            #print (line)
            data = data + line
            if ".a" in file_path:
                if "text\t" in line:
                    continue
                nums = re.findall("[\s+(\d+)\t*]\s+([0-9a-fA-F]+)\t", line, re.X)
                #print (nums)
                num_size = len(nums)
                for i in range(num_size - 1):
                    numbers[i] += int(nums[i])
                numbers[num_size - 1] = int(nums[i], 16)
        if ".a" in file_path:
            #print (text_size, code_size, rodata_size, data_size, bss_size, dec_size, hex(hex_size))
            for i in range(num_size -1):
                data = data + str(numbers[i]).rjust(7,' ')+"\t"
            data = data + str(hex(numbers[num_size - 1])).rjust(7,' ')+"\t"
            data = data + file_path
            data = data + "\n\n"

def find_file(path):
    if (os.path.exists(path)):
        files = os.listdir(path)
        for file in files:
            m = os.path.join(path, file)
            if (os.path.isdir(m)):
                find_file(m)
            elif (file.endswith('.a')):
                #print (m)
                get_size(m)

def main():
    global data, size_tools
    parser = argparse.ArgumentParser(description='armino_size - a tool to print size information from an ARMINO MAP file')
    parser.add_argument('--size', dest='size', default=None)
    parser.add_argument('--dirs', dest='dirs', default=None)
    parser.add_argument('--outfile', dest='outfile', default=None)
    args = parser.parse_args()

    size_tools = args.size
    if (os.path.exists("app.elf")):
        get_size("app.elf")

    data = data + "\n"
    find_file(os.getenv('PWD'))
    find_file(args.dirs)

    #print (data)
    with open(args.outfile, 'w') as fw:
        fw.write(data)


if __name__ == "__main__":
    main()
