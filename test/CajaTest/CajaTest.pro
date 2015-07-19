#-------------------------------------------------
# Pruebas para caja
#-------------------------------------------------

QT       += sql testlib

TARGET = tst_cajatest

CONFIG   += console
CONFIG   -= app_bundle

QMAKE_LFLAGS += -Wl,-rpath,../../bin/plugins

INCLUDEPATH += ../../plugins/pagos \
               ../../utiles \
               ../../src \
               ../../plugins/caja \
               ../../plugins/HiComp

LIBS += ../../bin/plugins/libremitos.so \
        ../../bin/plugins/libcaja.so \
        ../../bin/plugins/libcuotas.so \
        ../../bin/plugins/libhicomp.so \
        ../../bin/libutiles.a

TEMPLATE = app

SOURCES += tst_cajatest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}