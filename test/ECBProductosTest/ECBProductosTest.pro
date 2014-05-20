#-------------------------------------------------
# Test par aprobar las funcionales varias
#-------------------------------------------------

QT       += sql testlib
TARGET = tst_ecbproductostest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
DEFINES += SRCDIR=\\\"$$PWD/\\\"

SOURCES += tst_ecbproductostest.cpp \
           ../../utiles/ecbproductosmodel.cpp \
           ../../utiles/ecbproductosfilter.cpp

HEADERS += ../edatabasetest.h \
           ../../utiles/ecbproductosmodel.h \
           ../../utiles/ecbproductosfilter.h

INCLUDEPATH += ../../plugins/proveedor \
               ../../utiles

LIBS += ../../bin/plugins/libproveedor.so \
        ../../bin/libutiles.a

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}