#!/usr/bin/env python3

from os import listdir
from os.path import isdir,join
from hashlib import md5
import sys
import re

indent =0

class Node:
    def add_child(self, child):
        assert isinstance(child, Node)
        self.is_leaf = False
        if child in self.children:
            return
        self.children.append(child)
        hashes = []
        for node in self.children:
            hashes.append(node.get_hash())
        prehash = ''.join(hashes)
        self.node_hash = md5(prehash.encode('utf-8')).hexdigest()

    def get_hash(self):
        return self.node_hash

    def generate_file_hash(self, path):
        #print('%sGenerating hash for %s'%(' '*indent*2, path))
        file_hash = md5()
        if isdir(path):
            file_hash.update(''.encode('utf-8'))
        else:
            with open(path, 'rb') as f:
                for chunk in iter(lambda: f.read(4096), b''):
                    file_hash.update(chunk)
        return file_hash.hexdigest()

    def __str__(self):
        if isdir(self.path):
            output = self.path + '(' + self.get_hash() + ')'
        else:
            output = self.path + '(' + self.get_hash() + ')'
        child_count = 0
        for child in self.children:
            toadd = str(child)
            line_count = 0
            for line in toadd.split('\n'):
                output += '\n'
                if line_count == 0 and child_count == len(self.children) - 1:
                    output += '`-- ' + line
                elif line_count == 0 and child_count != len(self.children) - 1:
                    output += '|-- ' + line
                elif child_count != len(self.children) - 1:
                    output += '|   ' + line
                else:
                    output += '    ' + line
                line_count += 1
            child_count += 1
        return output

    def __init__(self, path):
        global indent
        self.path = path
        self.children = []
        self.node_hash = self.generate_file_hash(path)
        self.is_leaf = True

        if not isdir(path):
            #print('%sExiting init'%(' ' * indent * 2))
            return
        for obj in sorted(listdir(path)):
            #print('%sAdding child called%s'%(' ' * indent * 2, dir))
            indent += 1
            new_child = Node(join(path, obj))
            indent -= 1
            self.add_child(new_child)

if __name__ == "__main__":
    tree = None
    if len(sys.argv) < 2:
        tree = Node('./')
    else:
        tree = Node(sys.argv[1])
    print(tree.node_hash)