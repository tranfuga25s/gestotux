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

INCLUDEPATH += ../../plugins/ventas \
               ../../plugins/productos \
               ../../utiles

LIBS += -L../../bin \
        -L../../bin/plugins \
        -lventas \
        -llibutiles \
        -llibproductos \
        -llibdescuentos \
        -llibcaja

QMAKE_LFLAGS += -Wl,-rpath,../../bin/plugins

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}
