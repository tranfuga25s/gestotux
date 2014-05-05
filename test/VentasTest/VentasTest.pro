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

FORMS += ../../plugins/ventas/FormAgregarVentaBase.ui

INCLUDEPATH += ../../plugins/ventas \
               ../../utiles

LIBS += ../../bin/plugins/libventas.so \
        ../../bin/plugins/libproductos.so \
        ../../bin/libutiles.a \
        -lutiles \
        -lproductos \
        -L../../bin \
        -L../../bin/plugins

QMAKE_LFLAGS += -Wl,-rpath,./plugins

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}