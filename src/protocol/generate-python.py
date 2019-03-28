#!/usr/bin/env python3

import os
import collections
import json

from jinja2 import Environment, FileSystemLoader

from generator import Generator

if __name__ == "__main__":
    json_data = json.load(open(os.path.join(Generator.RECIPE_PATH, 'ping_protocol.json'), 'r'), object_pairs_hook=collections.OrderedDict)
    j2_env = Environment(loader=FileSystemLoader(Generator.JINJA_PATH), trim_blocks=True)

    struct_token = {"u8": "B",
                    "u16": "H",
                    "u32": "I",
                    "i8": "b",
                    "i16": "h",
                    "i32": "i",
                    "char": "s"}

    # Create a new dictionary with all message keys for convenience in the template
    messageDict = json_data['messages']['ping1D']

    allMessages = dict(messageDict['get'])
    allMessages.update(messageDict['set'])
    allMessages.update(messageDict['control'])
    allMessages.update(messageDict['general'])

    json_data['all_msgs'] = allMessages

    # Create a list with the payload of all messages.
    # The list comprehension assembles all payloads of all messages into a single list
    # The "list(set())" makes sure there are no duplicates
    json_data['all_fields'] = list(set([msg['name'] for key, message in allMessages.items() for msg in message['payload']]))

    j2_env.globals['structToken'] = struct_token
    j2_env.globals.update(generator=Generator())

    # Create output file
    output_path = os.path.join(Generator.PATH, "python/")
    if not os.path.exists(output_path):
        os.makedirs(output_path)

    f = open(os.path.join(output_path, "pingmessage.py"), "w")
    f.write(j2_env.get_template("pingmessage.py.in").render(json_data))
    f.close()

    f = open(os.path.join(output_path, "ping1d.py"), "w")
    f.write(j2_env.get_template("ping1d.py.in").render(json_data))
    f.close()
