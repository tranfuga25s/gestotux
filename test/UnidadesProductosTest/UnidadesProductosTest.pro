#-------------------------------------------------
# Test para unidades de productos
#-------------------------------------------------

QT       += sql testlib
TARGET = tst_unidadesproductostest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
DEFINES += SRCDIR=\\\"$$PWD/\\\"

SOURCES += tst_unidadesproductostest.cpp
HEADERS += ../edatabasetest.h

INCLUDEPATH += ../../plugins/productos \
               ../../plugins/unidades_productos \
               ../../utiles

QMAKE_LFLAGS += -Wl,-rpath,./../../bin/plugins

LIBS += ../../bin/plugins/libproductos.so \
        ../../bin/plugins/libunidades_productos.so

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}
