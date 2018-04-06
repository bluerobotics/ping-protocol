INCLUDEPATH += $$PWD

system(python $$PWD/generator.py)

HEADERS += \
    $$PWD/parsers/detector.h \
    $$PWD/parsers/*.h

include($$PWD/generator.pri)