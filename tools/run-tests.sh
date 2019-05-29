#!/bin/bash

# Runs some tests for the ping-components project
# - Json style
# - Documentation
# - Protocol parser

# Variables
script_path="$( cd "$(dirname "$0")" ; pwd -P )"
project_path="${script_path}/.."
protocol_template_path="${project_path}/src/protocol/templates"
protocol_scripts="${project_path}/src/protocol"
protocol_definitions_path="${project_path}/src/protocol/definitions"

# Functions
source $script_path/functions.sh

echob "Check protocol file description file."

for filename in $protocol_definitions_path/*.json; do
    echob "Checking file: $(basename ${filename})"
    python3 -m json.tool ${filename} > /tmp/temporary_test_file.json;
    if ! comm -2 -3 ${filename} /tmp/temporary_test_file.json; then
        echob "Json file does not follow style. Try python3 -m json.tool."
        exit 1
    fi
done

echob "Check documentation."

for filename in $protocol_definitions_path/*.json; do
    echob "Checking documentation from file: $(basename ${filename})"
    $protocol_scripts/generate-doc.py

    if ! git diff --quiet --ignore-submodules HEAD 2>/dev/null; then
        echob "Json file and documentation does not match."
        git diff | cat
        exit 1
    fi
done

echob "Run protocol test."

runstep "$project_path/src/protocol/generate-python.py" "Generate python library" "Failed to generate python library"
runstep "$project_path/src/protocol/generate-arduino.py" "Generate arduino library" "Failed to generate arduino library"

build_test="/tmp/protocol-test-build"
rm -rf $build_test
mkdir -p ${build_test}
qmake -o ${build_test} ${project_path}/test
make -C ${build_test}
if ! ${build_test}/test; then
    echob "Protocol was not able to parse data."
    exit 1
fi

echob "Everything is fine."
