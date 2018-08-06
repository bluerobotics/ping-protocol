TEMPLATE = app

message(Configuring test build...)
$$system(python $$PWD/generate-tests.py)

CONFIG += \
    c++14 \

QT += testlib core
QT -= gui

include(../ping.pri)
HEADERS += test.h
SOURCES += test.cpp