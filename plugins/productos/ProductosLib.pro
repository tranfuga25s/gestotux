TEMPLATE = lib

CONFIG += staticlib \
          help \
          exceptions

TARGET = libproductos

DESTDIR = ../../bin/plugins/

INCLUDEPATH += ../../src \
               ../../utiles \
               ../../reporte

QT += sql

HEADERS += mcategorias.h \
           mproductos.h
SOURCES += mcategorias.cpp \
           mproductos.cpp

INCLUDEPATH += ../../utiles \
               ../../src

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}