#-------------------------------------------------
# Test para productos
#-------------------------------------------------

QT       += sql testlib
TARGET = tst_productostest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
DEFINES += SRCDIR=\\\"$$PWD/\\\"

SOURCES += tst_productostest.cpp
HEADERS += ../edatabasetest.h
FORMS += ../../plugins/productos/formproductobase.ui \
         ../../plugins/productos/FormPrefProductosBase.ui

INCLUDEPATH += ../../plugins/productos \
               ../../utiles

LIBS += ../../bin/plugins/libproductos.so

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}