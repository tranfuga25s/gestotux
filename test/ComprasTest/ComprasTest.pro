#-------------------------------------------------
# Test para compras
#-------------------------------------------------

QT       += sql testlib

TARGET = tst_comprastest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_comprastest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../plugins/compras

LIBS += ../../bin/plugins/libcompras.so

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}