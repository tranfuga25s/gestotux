TEMPLATE = lib

CONFIG += staticlib \
          help \
          exceptions

SOURCES += MRemito.cpp \
           mitemremito.cpp

HEADERS += MRemito.h \
           mitemremito.h

QT += sql

TARGET = libremitos

DESTDIR = ../../bin/plugins/

INCLUDEPATH += ../../utiles \
               ../productos \
               ../../src \
               ../CtaCte \
               ../caja \
               ../descuentos


unix {
    LIBS += -gcov
}
