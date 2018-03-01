INCLUDEPATH += $$PWD

QT += serialport

HEADERS += \
    $$PWD/abstractlink.h \
    $$PWD/filelink.h \
    $$PWD/link.h \
    $$PWD/logthread.h \
    $$PWD/pingsimulationlink.h \
    $$PWD/seriallink.h \
    $$PWD/simulationlink.h \
    $$PWD/tcplink.h \
    $$PWD/udplink.h

SOURCES += \
    $$PWD/abstractlink.cpp \
    $$PWD/filelink.cpp \
    $$PWD/link.cpp \
    $$PWD/logthread.cpp \
    $$PWD/pingsimulationlink.cpp \
    $$PWD/seriallink.cpp \
    $$PWD/simulationlink.cpp \
    $$PWD/tcplink.cpp \
    $$PWD/udplink.cpp
