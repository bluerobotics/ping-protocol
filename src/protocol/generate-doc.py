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

f = open('README.md', "w")
f.write(j2_env.get_template("ping_doc.in").render(jsondata))
f.close()
