#!/usr/bin/env python3

import json
import re
import collections
from jinja2 import Template

class Generator:
    """
    TODO: the generator api should help with determining things about messages (like a total payload length,
    or if a field is a vector type). c functions should be moved to c generation code and python functions
    should be moved to python generation code.
    """
    def calc_payload(self, payloads):
        total_size = 0
        for payload in payloads:
            if self.is_vector(payload["type"]):

                if payload["vector"]["size"] == "dynamic":
                    if "sizetype" in payload["vector"]:
                        total_size = total_size + self.get_c_size(payload["vector"]["sizetype"])
                else:
                    total_size = ( total_size + int(payload["vector"]["size"])
                                    * self.get_c_size(payload["vector"]["datatype"]) )

            else:
                total_size = total_size + self.get_c_size(payload["type"])

        return int(total_size)

    def convert_short_type(self, t):
        # u/i
        ui = t[0]
        # number of bits
        nbits = t[1:]

        if ui == 'i':
            return 'int{0}_t'.format(nbits)

        if ui == 'u':
            return 'uint{0}_t'.format(nbits)

    def convert_c_name(self, name):
        # Create signals for Qt class
        names = name.split('_')
        new_name = names[0]
        for sub_names in names[1:]:
            new_name = new_name + sub_names.title()
        return new_name + 'Update'

    def get_c_size(self, t):
        # this regex will get the X in u/intX_t (uint8_t, int16_t)
        match = re.search('[0-9]{1,2}', t)
        if match:
            return int(int(match.group(0)) / 8)
        if t.find('bool') != -1:
            return 1
        if t.find('int') != -1:
            return 4
        if t.find('float') != -1:
            return 4
        if t.find('double') != -1:
            return 8

    def get_type_base_size(self, types):
        # Get total number of bytes in vector, float, int, double
        total_bytes = 0
        for t in types:
            total_bytes = total_bytes + self.get_c_size(t)
        print(types, total_bytes)
        return total_bytes

    def get_type_string(self, t, pointer=False, name=''):
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
            s = self.convert_short_type(t)
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

    def is_vector(self, t):
        return ('vector' in t)

    def generate(self, definition, template, globuls=None):
        definitionDict = None
        if type(definition) == dict:
            definitionDict = definition
        else:
            definitionDict = json.load(open(definition), object_pairs_hook=collections.OrderedDict)

        with open(template) as file:
            template = Template(file.read(), trim_blocks=True)

        template.globals.update(generator=self)

        if globuls is not None:
            for globul in globuls:
                template.globals[globul] = globuls[globul]

        return template.render(definitionDict)

if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description="ping protocol generation utility")
    parser.add_argument('--definition', action="store", required=True, type=str,
        help="the message definition file (ex ping360.json)")
    parser.add_argument('--template', action="store", required=True, type=str,
        help="the file template to populate according to the selected message definitions (ex pingmessage-.md.in)")
    args = parser.parse_args()

    print( Generator().generate(args.definition, args.template) )
