#!/usr/bin/env python3

import os
import collections
import json

from jinja2 import Environment, FileSystemLoader

from generator import Generator

if __name__ == "__main__":
    recipes = [file for file in os.listdir(Generator.RECIPE_PATH) if file.endswith('.json')]
    recipes.sort()
    jsondata = collections.OrderedDict({ "sensors": {} })

    for recipe in recipes:
        data = json.load(open(os.path.join(Generator.RECIPE_PATH, recipe), 'r'), object_pairs_hook=collections.OrderedDict)
        jsondata['sensors'][data['sensor_info']['name']] = data

    j2_env = Environment(loader=FileSystemLoader(Generator.JINJA_PATH), trim_blocks=True)

    f = open('README.md', "w")
    f.write(j2_env.get_template("ping_doc.in").render(jsondata))
    f.close()
