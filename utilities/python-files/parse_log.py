#!/usr/bin/env python3
import json
import re
import sys
import argparse

def process_log(input_file, output_file):
    """
     Parse hiop log. Note it is formatted: address= key access= value and
     fill in dict upon it. key and val are converted from string to hex.
    """
    input_fo = open(input_file, "r")
    output_fo = open(output_file, "w+b")

    "dict with addressses and values directly from the log"
    addr_val = dict.fromkeys(range(0, 4096, 4),0)

    "dict with addresses and bytes post processing from the log"
    addr_bytes = {}
    i = 0

    for line in input_fo.readlines():
        if line.startswith("address="):
            (string1, key, string2, val) = line.split()
            addr_val[int(key,16)] = int(val, 16)

    for key, val in addr_val.items():
        "Return array of 4x bytes for val in little endian"
        dw = val.to_bytes(4, byteorder='little')
        "Fill in the dict upon it"
        addr_bytes.update({key: dw})
        """and most importantly memory dump the bytes to a bin file
           exactly as seen in Linux for the config space for the PCIe devices """
        output_fo.write(val.to_bytes(4, byteorder='little'))
        "Print the dumping or not... pretty verbose"
        if i % 50 == 0:
            print(f'addr: {hex(key)} val: {hex(val)} {dw}')
        i += 1

    input_fo.close()
    output_fo.close()

help1 = "Log to be mem dumped for processing, eg. ../../hiop/logs_sapai_bhs_cadence/hiop.log"
help2 = "Output of that processing to be used by the app, eg. ../dumps/conf_space"

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(dest='input', type=str, help=help1)
    parser.add_argument(dest='output', type=str, help=help2)
    args = parser.parse_args()
    process_log(args.input, args.output)

if __name__ == "__main__":
    main()
