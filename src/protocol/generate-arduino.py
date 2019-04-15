#!/usr/bin/env python

import os
import collections
import json

from jinja2 import Environment, FileSystemLoader

from generator import Generator

if __name__ == "__main__":
    recipes = ['common.json', 'ping_protocol.json']

    #TODO: It's necessary to update arduino generation to deal with ping360
    jsondata = {}
    recipes_dict = {}
    for recipe in recipes:
        recipes_dict[recipe.split('.'[0])] = json.load(open(os.path.join(Generator.RECIPE_PATH, recipe), 'r'))

    jsondata = recipes_dict['common'].copy()
    jsondata.update(recipes_dict['ping_protocol'])
    jsondata['messages']['get'].update(recipes_dict['common']['messages']['get'])

    j2_env = Environment(loader=FileSystemLoader(Generator.JINJA_PATH), trim_blocks=True)

    # Create a new dictionary with all message keys for convenience in the template
    messageDict = jsondata['messages']
    allMessages = {}

    for message_type in messageDict.keys():
        allMessages.update(messageDict[message_type])

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
    output_path = os.path.join(Generator.PATH, "arduino/")
    if not os.path.exists(output_path):
        os.makedirs(output_path)

    f = open(os.path.join(output_path, "ping1d.h"), "w")
    f.write(j2_env.get_template("ping1d.h.in").render(jsondata))
    f.close()

    f = open(os.path.join(output_path, "ping1d.cpp"), "w")
    f.write(j2_env.get_template("ping1d.cpp.in").render(jsondata))
    f.close()
