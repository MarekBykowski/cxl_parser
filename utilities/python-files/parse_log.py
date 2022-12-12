#!/usr/bin/env python3
import json
import re

INPUT="../../hiop/logs_sapai_bhs_cadence/hiop.log"
OUTPUT="../dumps/conf_space"

log_file = open(INPUT, "r")
memdump = open(OUTPUT, "w+b")
"dict with addressses and values directly from the log"
addr_val = dict.fromkeys(range(0, 4096, 4),0)
"dict with addresses and bytes post processing from the log"
addr_bytes = {}
i = 0

"""
 Parse hiop log. Note it is formatted: address= key access= value and
 fill in dict upon it. key and val are converted from string to hex.
"""
for line in log_file.readlines():
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
    memdump.write(val.to_bytes(4, byteorder='little'))
    "Print the dumping or not... pretty verbose"
    if i % 50 == 0:
        print(f'addr: {hex(key)} val: {hex(val)} {dw}')
    i += 1

memdump.close()
