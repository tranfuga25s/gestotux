TEMPLATE = lib

CONFIG += staticlib \
          help \
          exceptions

TARGET = libproveedor

DESTDIR = ../../bin/plugins/

QT += sql

SOURCES += mproveedor.cpp
HEADERS += mproveedor.h

INCLUDEPATH += ../../src \
               ../../utiles

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}