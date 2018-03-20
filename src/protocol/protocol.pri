INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/message.h \
    $$PWD/packer.h \
    $$PWD/protocol.h

SOURCES += \
    $$PWD/message.cpp \
    $$PWD/packer.cpp \
    $$PWD/protocol.cpp

HEADERS += \
    $$PWD/parsers/parser.h \
    $$PWD/parsers/parser_json.h \
    $$PWD/parsers/parser_ping.h \
    $$PWD/pingmessage/pingmessage.h \
    $$PWD/pingmessage/pingmessage_es.h \
    $$PWD/pingmessage/pingmessage_gen.h
