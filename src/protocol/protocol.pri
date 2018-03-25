INCLUDEPATH += $$PWD

system(python $$PWD/pingmessage/ping_generator.py)

HEADERS += \
    $$PWD/parsers/*.h \
    $$PWD/pingmessage/*.h
