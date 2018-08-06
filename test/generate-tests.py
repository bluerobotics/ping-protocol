#!/usr/bin/env python

import os
import collections
import json

PATH = os.path.dirname(os.path.abspath(__file__))
JINJA_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), '')

from jinja2 import Environment, FileSystemLoader

def correct_str(string):
    return '"'+ string.replace('\\', '\\\\').replace('\"', '\\"') + '"'

jsondata = json.load(open(os.path.join(JINJA_PATH, 'ping1D_v3.17.json'), 'r'), object_pairs_hook=collections.OrderedDict)

j2_env = Environment(loader=FileSystemLoader(JINJA_PATH), trim_blocks=True)
j2_env.globals.update(type = type)
j2_env.globals.update(correct_str = correct_str)

f = open('test.cpp', "w")
f.write(j2_env.get_template("test.cpp.in").render(jsondata))
f.close()

