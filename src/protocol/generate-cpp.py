import collections
import json
import os

from jinja2 import Environment, FileSystemLoader
from generator import Generator

if __name__ == "__main__":
    PATH = os.path.dirname(os.path.abspath(__file__))
    JINJA_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'templates/')

    # Get list of all class names
    class_names = []
    # Get all jsons
    jsons = [file for file in os.listdir(JINJA_PATH) if file.endswith('.json')]
    for json_file in jsons:
        # Get json data
        protocol_data = json.load(open(os.path.join(JINJA_PATH, json_file), 'r'), object_pairs_hook=collections.OrderedDict)

        # Create prefix name
        file_prefix_name = protocol_data['class_info']['name'].lower()
        class_names.append(file_prefix_name)

        # Create output file
        output_path = os.path.join(PATH, '{0}/'.format(file_prefix_name))
        if not os.path.exists(output_path):
            os.makedirs(output_path)

        # Create our lovely jinja env
        j2_env = Environment(loader=FileSystemLoader(JINJA_PATH), trim_blocks=True)
        j2_env.globals.update(_actual_message_type='')
        generator = Generator()
        generator.set_jinja_env(j2_env)

        # Create a new dictionary with all message keys for convenience in the template
        ping1DMessages = protocol_data['messages']['ping1D']
        protocol_data['messages']['ping1D'] = {message:ping1DMessages[message_type][message] for message_type in ping1DMessages for message in ping1DMessages[message_type]}

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