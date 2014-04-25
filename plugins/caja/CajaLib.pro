TEMPLATE = lib

CONFIG += staticlib \
          help \
          exceptions

TARGET = libcaja

DESTDIR = ../../bin/plugins/

INCLUDEPATH += ../../src \
               ../../utiles \
               ../../reporte

QT += sql

HEADERS += mcajas.h \
           mmovimientoscaja.h
SOURCES += mcajas.cpp \
           mmovimientoscaja.cpp


INCLUDEPATH += ../../utiles \
               ../../src


unix {
    LIBS += -gcov
}
