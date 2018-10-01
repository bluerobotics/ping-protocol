#!/usr/bin/env python3

import os
import collections
import json
import re

PATH = os.path.dirname(os.path.abspath(__file__))
JINJA_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'templates/')

from jinja2 import Environment, FileSystemLoader

jsondata = json.load(open(os.path.join(JINJA_PATH, 'ping_protocol.json'), 'r'), object_pairs_hook=collections.OrderedDict)

j2_env = Environment(loader=FileSystemLoader(JINJA_PATH), trim_blocks=True)

def calc_payload(payloads):
    total_size = 0
    for payload in payloads:
        total_size = total_size + get_c_size(payload["type"])

    return total_size

def get_c_size(t):
    # Remove vector info
    if is_var_size(t):
        return 0

    vector_size = 1
    if t.find('[') != -1:
        vector_size = int(t.split('[')[1].split(']')[0])
        t = t.split('[')[0]

    # this regex will get the X in u/intX_t (uint8_t, int16_t)
    match = re.search('[0-9]{1,2}', t)
    if match:
        return int(int(match.group(0)) / 8)*vector_size

    if t.find('bool') != -1:
        return 1
    if t.find('int') != -1:
        return 4
    if t.find('float') != -1:
        return 4
    if t.find('double') != -1:
        return 8

def is_var_size(t):
    return ('[' in t)

def is_payload_var(payload):
    print(payload)
    if not payload:
        return False
    #return True
    return ('[' in payload[len(payload)-1]["type"])

structToken = { "u8":"B",
                "u16":"H",
                "u32":"I",
                "i8":"b",
                "i16":"h",
                "i32":"i",
                "char":"s" }

# Create a new dictionary with all message keys for convenience in the template
messageDict = jsondata['messages']['ping1D']

allMessages = dict(messageDict['get'])
allMessages.update(messageDict['set'])
allMessages.update(messageDict['control'])
allMessages.update(messageDict['general'])

jsondata['all_msgs'] = allMessages

j2_env.globals['structToken'] = structToken
j2_env.globals.update(calc_payload = calc_payload)
j2_env.globals.update(get_c_size = get_c_size)
j2_env.globals.update(is_payload_var = is_payload_var)

# Create output file
output_path = os.path.join(PATH, "python/")
if not os.path.exists(output_path):
    os.makedirs(output_path)

f = open(os.path.join(output_path, "PingMessage.py"), "w")
f.write(j2_env.get_template("PingMessage.py.in").render(jsondata))
f.close()

f = open(os.path.join(output_path, "Ping1D.py"), "w")
f.write(j2_env.get_template("Ping1D.py.in").render(jsondata))
f.close()
