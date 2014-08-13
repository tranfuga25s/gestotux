#-------------------------------------------------
# Test para remitos
#-------------------------------------------------

QT       += sql testlib

TARGET = tst_presupuestostest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_presupuestostest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../plugins/presupuesto \
               ../../utiles

LIBS += ../../bin/plugins/libpresupuesto.so \
        ../../bin/libutiles.a

FORMS += ../../plugins/presupuesto/FormPresupuestoBase.ui

QMAKE_LFLAGS += -Wl,-rpath,../../bin/plugins

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}