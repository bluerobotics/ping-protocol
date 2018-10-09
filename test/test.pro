TEMPLATE = app

message(Configuring test build...)
$$system($$PWD/generate-json.py)
$$system($$PWD/generate-tests.py)

CONFIG += \
    c++14 \

QT += testlib core
QT -= gui

include(../ping.pri)
HEADERS += test.h
SOURCES += test.cpp
