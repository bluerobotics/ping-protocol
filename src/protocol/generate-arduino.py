#!/usr/bin/env python

import os
import collections
import json
import re

PATH = os.path.dirname(os.path.abspath(__file__))
JINJA_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'templates/')

from jinja2 import Environment, FileSystemLoader

jsondata = json.load(open(os.path.join(JINJA_PATH, 'ping_protocol.json'), 'r'), object_pairs_hook=collections.OrderedDict)

j2_env = Environment(loader=FileSystemLoader(JINJA_PATH), trim_blocks=True)

def convert_short_type(t):
    # u/i
    ui = t[0]
    # number of bits
    nbits = t[1:]

    if ui == 'i':
        return 'int{0}_t'.format(nbits)

    if ui == 'u':
        return 'uint{0}_t'.format(nbits)

def is_vector(t):
    return t.find('vector') != -1

def get_type_string(t, pointer=False, name=''):
    # Move from u/i short types

    # Remove vector info
    vector_size = 0
    if t.find('[') != -1:
        vector_size = t.split('[')[1].split(']')[0]
        t = t.split('[')[0]

    # Check for short type
    # This script will get X in u/iX (u8, i16)
    match = re.search('[u,i][0-9]{1,2}$', t)
    s = ''
    if match:
        s = convert_short_type(t)
    else:
        s = t

    # Append vector
    if vector_size and pointer:
        s = s + '*'
    elif vector_size and name:
        s = s + ' {0}[{1}]'.format(name, vector_size)
    elif name:
        s = s + ' {0}'.format(name)

    return s

# Create a new dictionary with all message keys for convenience in the template
messageDict = jsondata['messages']['ping1D']

allMessages = dict(messageDict['get'])
allMessages.update(messageDict['set'])
allMessages.update(messageDict['control'])
allMessages.update(messageDict['general'])

# compile a list of all payload fields in 'get' category messages
# these field names will be members of the ping1d device interface
proplist = {}
for msg in messageDict['get']:
    for field in messageDict['get'][msg]['payload']:
        proplist[field['name']] = field

jsondata['all_msgs'] = allMessages
jsondata['proplist'] = proplist

j2_env.globals.update(is_vector = is_vector)
j2_env.globals.update(get_type_string = get_type_string)

# Create output file
output_path = os.path.join(PATH, "arduino/")
if not os.path.exists(output_path):
    os.makedirs(output_path)

f = open(os.path.join(output_path, "ping1d.h"), "w")
f.write(j2_env.get_template("ping1d.h.in").render(jsondata))
f.close()

f = open(os.path.join(output_path, "ping1d.cpp"), "w")
f.write(j2_env.get_template("ping1d.cpp.in").render(jsondata))
f.close()

