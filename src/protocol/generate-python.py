#!/usr/bin/env python

import os
import collections
import json
import re

from generator import Generator

PATH = os.path.dirname(os.path.abspath(__file__))
JINJA_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'templates/')

from jinja2 import Environment, FileSystemLoader

jsondata = json.load(open(os.path.join(JINJA_PATH, 'ping_protocol.json'), 'r'), object_pairs_hook=collections.OrderedDict)

j2_env = Environment(loader=FileSystemLoader(JINJA_PATH), trim_blocks=True)

structToken = { "u8":"B",
                "u16":"H",
                "u32":"I",
                "i8":"b",
                "i16":"h",
                "i32":"i",
                "char":"s" }

# Create a new dictionary with all message keys for convenience in the template
ping1DMessages = jsondata['messages']['ping1D']
allMessages = {message:ping1DMessages[message_type][message] for message_type in ping1DMessages for message in ping1DMessages[message_type]}
jsondata['all_msgs'] = allMessages

generator = Generator()
j2_env.globals['structToken'] = structToken
generator.set_jinja_env(j2_env)

f = open('PingMessage.py', "w")
f.write(j2_env.get_template("PingMessage.py.in").render(jsondata))
f.close()

f = open('Ping1D.py', "w")
f.write(j2_env.get_template("Ping1D.py.in").render(jsondata))
f.close()

