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
           ../../utiles/ecbproductosfilter.cpp \
           ../../utiles/ecbproductos.cpp \
           ../../utiles/mproductostotales.cpp

HEADERS += ../edatabasetest.h \
           ../../utiles/ecbproductosmodel.h \
           ../../utiles/ecbproductosfilter.h \
           ../../utiles/ecbproductos.h \
           ../../utiles/mproductostotales.h

INCLUDEPATH += ../../plugins/proveedor \
               ../../utiles

LIBS += -L../../bin/plugins/ \
        -L../../bin \
        -lproveedor \
        -lutiles
QMAKE_LFLAGS += -Wl,-rpath,../../bin/plugins

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}
