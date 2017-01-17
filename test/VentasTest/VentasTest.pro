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

FORMS += ../../plugins/ventas/FormAgregarVentaBase.ui \
         ../../plugins/ventas/FormPrefVentaBase.ui

INCLUDEPATH += ../../plugins/ventas \
               ../../plugins/productos \
               ../../utiles

LIBS += ../../bin/plugins/libventas.so \
        ../../bin/plugins/libproductos.so \
        ../../bin/libutiles.a \
        -lutiles \
        -llibproductos \
        -L../../bin \
        -L../../bin/plugins

QMAKE_LFLAGS += -Wl,-rpath,../../bin/plugins

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}