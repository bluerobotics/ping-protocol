INCLUDEPATH += $$PWD

system(./pingmessage/ping_generator.py)

HEADERS += \
    $$PWD/parsers/*.h \
    $$PWD/pingmessage/*.h
