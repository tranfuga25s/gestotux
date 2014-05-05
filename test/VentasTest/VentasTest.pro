#-------------------------------------------------
# Test relacionados con las ventas
#-------------------------------------------------

QT       += sql testlib

TARGET = tst_ventastest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_ventastest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../plugins/proveedor \
               ../../utiles

LIBS += ../../bin/plugins/libproveedor.so

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}