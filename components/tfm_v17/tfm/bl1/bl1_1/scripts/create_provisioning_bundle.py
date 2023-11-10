#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

import argparse
import struct
import secrets

def struct_pack(objects, pad_to=0):
    defstring = "<"
    for obj in objects:
        defstring += str(len(obj)) + "s"

    size = struct.calcsize(defstring)
    if size < pad_to:
        defstring += str(pad_to - size) + "x"

    return (bytes(struct.pack(defstring, *objects)))

parser = argparse.ArgumentParser()
parser.add_argument("--bl2_encryption_key_input_file", help="the key that BL2 was encrypted with", required=True)
parser.add_argument("--bl2_signing_key_input_file", help="the key that BL2 was signed with", required=False)
parser.add_argument("--guk_input_file", help="the GUK", required=True)
parser.add_argument("--bl1_2_padded_hash_input_file", help="the hash of the final bl1_2 image", required=True)
parser.add_argument("--bl2_signed_hash_input_file", help="the hash of the final bl2 image", required=True)
parser.add_argument("--bl1_2_input_file", help="the final bl1_2 image", required=True)
parser.add_argument("--bundle_output_file", help="bundle output file", required=True)
args = parser.parse_args()

with open(args.bl2_encryption_key_input_file, "rb") as in_file:
    bl1_2_encryption_key = in_file.read()

with open(args.guk_input_file, "rb") as in_file:
    guk = in_file.read()

with open(args.bl1_2_padded_hash_input_file, "rb") as in_file:
    bl1_2_padded_hash = in_file.read()

with open(args.bl2_signed_hash_input_file, "rb") as in_file:
    bl2_signed_hash = in_file.read()

with open(args.bl1_2_input_file, "rb") as in_file:
    bl1_2 = in_file.read()

if args.bl2_signing_key_input_file:
    with open(args.bl2_signing_key_input_file + ".pub", "rb") as in_file:
        # Remove the first 4 bytes since it's HSS info
        bl1_rotpk_0 = in_file.read()[4:]
else:
    bl1_rotpk_0 = bytes(56)


bundle = struct_pack([
    int("0xC0DEFEED", 16).to_bytes(4, 'little'),
    bl1_2_encryption_key,
    guk,
    bl1_2_padded_hash,
    bl2_signed_hash,
    bl1_2,
    bl1_rotpk_0
])

with open(args.bundle_output_file, "wb") as out_file:
    out_file.write(bundle)
