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

QMAKE_LFLAGS += -Wl,-rpath,./plugins

INCLUDEPATH += ../../utiles \
               ../productos \
               ../../src \
               ../CtaCte \
               ../caja \
               ../descuentos

LIBS += -L../../bin \
        -lutiles

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}
