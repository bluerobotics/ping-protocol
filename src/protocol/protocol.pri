INCLUDEPATH += $$PWD

# Generate code
generator.target = $$PWD/pingmessage
generator.depends = FORCE
generator.commands = python $$PWD/generator.py
PRE_TARGETDEPS += $$PWD/pingmessage
QMAKE_EXTRA_TARGETS += generator

HEADERS += \
    $$PWD/parsers/*.h

include($$PWD/generator.pri)