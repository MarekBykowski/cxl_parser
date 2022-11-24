#!/usr/bin/env python3
import json
import re

file_name = "cxl_log.txt"
file = open(file_name, "r")
addr_val = {}
data = []

for line in file.readlines():
    if line.startswith("address="):
        (key, val) = line.split()[1::2]
        addr_val[key] = int(val,16)
print(addr_val)

# define func
def cap1_op(a):
    return a + 5

def cap2_op(a):
    return a - 5

# dict with 'values' = NULL
fields_name = {
        "cap1": cap1_op,
        "cap2": cap2_op
}
print(fields_name)

for v in addr_val.values():
    fv = [] # aka list iterator
    for fn,fo in fields_name.items():
        ret = fo(v)
        fv.append(ret)
    structure = {key:value for key,value in zip(fields_name.keys(), fv)}
    data.append(structure)

print('data ', data)

regs = addr_val.keys()

whole = dict(zip(regs, data))
print('whole', whole)

print(json.dumps(whole, indent = 8))


