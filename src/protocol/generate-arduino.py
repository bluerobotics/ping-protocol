#!/usr/bin/env python

import os
import collections
import json
import re

from jinja2 import Environment, FileSystemLoader

from generator import Generator

if __name__ == "__main__":
    PATH = os.path.dirname(os.path.abspath(__file__))
    JINJA_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'templates/')

    jsondata = json.load(open(os.path.join(JINJA_PATH, 'ping_protocol.json'), 'r'), object_pairs_hook=collections.OrderedDict)

    j2_env = Environment(loader=FileSystemLoader(JINJA_PATH), trim_blocks=True)

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

    j2_env.globals.update(generator=Generator())

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
