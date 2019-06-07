#!/usr/bin/env python3

from pathlib import Path
from generator import Generator
import argparse
import os

parser = argparse.ArgumentParser(description="generate markdown documentation files for message definitions")
parser.add_argument('--output-directory', action="store", default="./", type=str, help="directory to save output files")
args = parser.parse_args()

if not os.path.exists(args.output_directory):
    os.makedirs(args.output_directory)

scriptPath = Path(__file__).parent.absolute()

definitionPath = "%s/definitions" % scriptPath
templatePath = "%s/templates" % scriptPath

templateFile = "%s/pingmessage-.md.in" % templatePath

generator = Generator()

definitions = [ "common",
                "ping1d",
                "ping360"]

for definition in definitions:
    definitionFile = "%s/%s.json" % (definitionPath, definition)
    file = open("%s/pingmessage-%s.md" % (args.output_directory, definition), "w")
    file.write(generator.generate(definitionFile, templateFile, {"definition": definition}))
    file.close()
