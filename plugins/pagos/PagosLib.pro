TEMPLATE = lib

CONFIG += staticlib \
          help \
          exceptions

TARGET = libpagos

DESTDIR = ../../bin/plugins/

INCLUDEPATH += ../../src \
               ../../utiles \
               ../../reporte

QT += sql

HEADERS += mpagos.h
SOURCES += mpagos.cpp

INCLUDEPATH += ../../utiles \
               ../../src
unix {
    LIBS += -gcov
}
