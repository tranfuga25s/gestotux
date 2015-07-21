#-------------------------------------------------
# Test para recibos
#-------------------------------------------------

QT       += sql testlib

TARGET = tst_pagostest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_pagostest.cpp \
           ../../plugins/pagos/mpagos.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../plugins/pagos \
               ../../utiles \
               ../../src \
               ../../plugins/caja \
               ../../plugins/hicomp

LIBS += ../../bin/plugins/libremitos.so \
        ../../bin/plugins/libcaja.so \
        ../../bin/plugins/libcuotas.so \
        ../../bin/libutiles.a

QMAKE_LFLAGS += -Wl,-rpath,../../bin/plugins

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}