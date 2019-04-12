#!/usr/bin/env python

import os
import collections
import json

from jinja2 import Environment, FileSystemLoader

from generator import Generator

jsondata = json.load(open(os.path.join(Generator.RECIPE_PATH, 'ping_protocol.json'), 'r'), object_pairs_hook=collections.OrderedDict)

j2_env = Environment(loader=FileSystemLoader(Generator.JINJA_PATH), trim_blocks=True)

f = open('README.md', "w")
f.write(j2_env.get_template("ping_doc.in").render(jsondata))
f.close()
