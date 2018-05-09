CONFIG += c++11

SRC_PATH = $$PWD/src

INCLUDEPATH += $$SRC_PATH

include($$SRC_PATH/link/link.pri)
include($$SRC_PATH/protocol/protocol.pri)
