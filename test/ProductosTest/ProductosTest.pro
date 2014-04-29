#-------------------------------------------------
#
# Project created by QtCreator 2013-03-15T09:38:56
#
#-------------------------------------------------

QT       += sql testlib
TARGET = tst_productostest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
DEFINES += SRCDIR=\\\"$$PWD/\\\"

SOURCES += tst_productostest.cpp
HEADERS += ../edatabasetest.h

INCLUDEPATH += ../../plugins/productos \
               ../../utiles

LIBS += ../../bin/plugins/libproductos.so

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}