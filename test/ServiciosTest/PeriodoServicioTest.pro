#-------------------------------------------------
#
# Project created by QtCreator 2012-10-11T10:35:59
#
#-------------------------------------------------

QT       += sql testlib

QT       -= gui

TARGET = tst_periodoserviciotest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += ../../bin/plugins/libservicios.so \
        ../../bin/plugins/libpagos.so \
        ../../bin/plugins/libcaja.so \
        ../../bin/plugins/libdescuentos.so \
        -L../../bin \
        -L../../bin/plugins

QMAKE_LFLAGS += -Wl,-rpath,../../bin/plugins

SOURCES += tst_periodoserviciotest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
exists( ../../travis.pri ) {
    include( ../../travis.pri )
}