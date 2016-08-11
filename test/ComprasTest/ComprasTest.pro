#-------------------------------------------------
# Test para compras
#-------------------------------------------------

QT       += sql testlib

TARGET = tst_comprastest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_comprastest.cpp \
           ../../utiles/ecbproductos.cpp \
           ../../utiles/ecbproductosmodel.cpp \
           ../../utiles/ecbproductosfilter.cpp \
           ../../utiles/mproductostotales.cpp
HEADERS += ../../utiles/ecbproductos.h \
           ../../utiles/ecbproductosmodel.h \
           ../../utiles/ecbproductosfilter.h \
           ../../utiles/mproductostotales.h
DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../plugins/compras \
               ../../utiles

LIBS += ../../bin/plugins/libcompras.so \
        ../../bin/plugins/libproveedor.so \
        ../../bin/libutiles.a

FORMS += ../../plugins/compras/FormPrefComprasBase.ui \
         ../../plugins/compras/FormAgregarCompraBase.ui

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}