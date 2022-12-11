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

"""
 Parse Suhas's hiop log. Note it is formatted: address= key sth= value
 and fill in dict upon it. key and val are converted from string to hex.
"""
for line in log_file.readlines():
    if line.startswith("address="):
        (string1, key, string2, val) = line.split()
        addr_val[int(key,16)] = int(val, 16)

for key, val in addr_val.items():
    "Return array of 4x bytes for val, little endian"
    dw = val.to_bytes(4, byteorder='little')
    "Fill in the dict upon it"
    addr_bytes.update({key: dw})
    """and most importnatly write the bytes to a bin file
       exactly as the memmory dump for config space for Linux pcie dev is"""
    memdump.write(val.to_bytes(4, byteorder='little'))
    "Print it. One may ignore it"
    print(f'addr: {hex(key)} val: {hex(val)} {dw}')

memdump.close()
