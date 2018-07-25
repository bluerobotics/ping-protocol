#!/usr/bin/env python

import os
import re

class Generator:
    def set_jinja_env(self, env):
        functions = Generator.__dict__
        for name in functions:
            if '__' not in name:
                env.globals[name] = functions[name]

    def calc_payload(payloads):
        total_size = 0
        for payload in payloads:
            total_size = total_size + Generator.get_c_size(payload["type"])

        return total_size

    def convert_short_type(t):
        # u/i
        ui = t[0]
        # number of bits
        nbits = t[1:]

        if ui == 'i':
            return 'int{0}_t'.format(nbits)

        if ui == 'u':
            return 'uint{0}_t'.format(nbits)

    def convert_c_name(name):
        # Create signals for Qt class
        names = name.split('_')
        new_name = names[0]
        for sub_names in names[1:]:
            new_name = new_name + sub_names.title()
        return new_name + 'Update'

    def get_c_size(t):
        # Remove vector info
        if Generator.is_var_size(t):
            return 0

        vector_size = 1
        if t.find('[') != -1:
            vector_size = int(t.split('[')[1].split(']')[0])
            t = t.split('[')[0]

        # this regex will get the X in u/intX_t (uint8_t, int16_t)
        match = re.search('[0-9]{1,2}', t)
        if match:
            return int(int(match.group(0)) / 8)*vector_size

        if t.find('bool') != -1:
            return 1*vector_size
        if t.find('int') != -1:
            return 4*vector_size
        if t.find('float') != -1:
            return 4*vector_size
        if t.find('double') != -1:
            return 8*vector_size

    def get_type_base_size(self, types):
        # Get total number of bytes in vector, float, int, double
        total_bytes = 0
        for t in types:
            total_bytes = total_bytes + self.get_c_size(t)
        print(types, total_bytes)
        return total_bytes

    def get_type_string(t, pointer=False, name=''):
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
            s = Generator.convert_short_type(t)
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

    def is_vector(t):
        return t.find('[') != -1

    def capitalize(s):
        return s[0].capitalize() + s[1:]

    def is_var_size(t):
        return ('[var]' in t)

if __name__ == "__main__":
    print("This will print all jinja env functions defined in this class")
    a = Generator.__dict__
    for b in a:
        if '__' not in b:
            print(b)