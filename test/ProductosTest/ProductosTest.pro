#-------------------------------------------------
# Test para productos
#-------------------------------------------------

QT       += sql testlib
TARGET = tst_productostest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
DEFINES += SRCDIR=\\\"$$PWD/\\\"

QMAKE_LFLAGS += -Wl,-rpath,../../bin/plugins

SOURCES += tst_productostest.cpp
HEADERS += ../edatabasetest.h
FORMS += ../../plugins/productos/formproductobase.ui \
         ../../plugins/productos/FormPrefProductosBase.ui

INCLUDEPATH += ../../plugins/productos \
               ../../utiles

LIBS += -L../../bin/plugins/ \
        -lproductos \
        -lproveedor

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}
