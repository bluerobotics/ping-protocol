#!/usr/bin/env python

import os
import json
import re

from jinja2 import Environment, FileSystemLoader

PATH = os.path.dirname(os.path.abspath(__file__))
JINJA_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'templates/')

def calc_payload(payloads):
    total_size = 0
    for payload in payloads:
        total_size = total_size + get_c_size(payload["type"])
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

def get_type_base_size(types):
    # Get total number of bytes in vector, float, int, double
    total_bytes = 0
    for t in types:
        total_bytes = total_bytes + get_c_size(t)
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
        s = convert_short_type(t)
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

if __name__ == "__main__":
    # Get list of all class names
    class_names = []
    # Get all jsons
    jsons = [file for file in os.listdir(JINJA_PATH) if file.endswith('.json')]
    for json_file in jsons:
        # Get json data
        protocol_data = json.load(open(os.path.join(JINJA_PATH, json_file), 'r'))

        # Create prefix name
        file_prefix_name = protocol_data['class_info']['name'].lower()
        class_names.append(file_prefix_name)

        # Create output file
        output_path = os.path.join(PATH, '{0}/'.format(file_prefix_name))
        if not os.path.exists(output_path):
            os.makedirs(output_path)

        # Create our lovely jinja env
        j2_env = Environment(loader=FileSystemLoader(JINJA_PATH), trim_blocks=True)
        j2_env.globals.update(calc_payload=calc_payload)
        j2_env.globals.update(capitalize=capitalize)
        j2_env.globals.update(convert_short_type=convert_short_type)
        j2_env.globals.update(get_c_size=get_c_size)
        j2_env.globals.update(get_type_base_size=get_type_base_size)
        j2_env.globals.update(get_type_string=get_type_string)
        j2_env.globals.update(is_vector=is_vector)
        j2_env.globals.update(_actual_message_type='')

        # Create main class
        f = open(os.path.join(output_path, "{0}.h".format(file_prefix_name)), "w")
        f.write(j2_env.get_template(protocol_data['class_info']['file']).render(protocol_data))
        f.close()

        # Create subclass
        for message_type in protocol_data["messages"]:
            j2_env.globals.update(_actual_message_type=message_type)
            f = open(os.path.join(output_path, "{0}_{1}.h".format(file_prefix_name, message_type)), "w")
            f.write(j2_env.get_template(protocol_data['subclass_info']['file']).render(protocol_data))
            f.close()

        # Create all header with all subclasses
        f = open(os.path.join(output_path, "{0}_all.h".format(file_prefix_name)), "w")
        f.write(j2_env.get_template('_all.h.in').render(protocol_data))
        f.close()
