#-------------------------------------------------
# Test para remitos
#-------------------------------------------------

QT       += sql testlib

TARGET = tst_remitostest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_remitostest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../plugins/remitos \
               ../../utiles

LIBS += ../../bin/plugins/libremitos.so \
        ../../bin/plugins/libcaja.so \
        ../../bin/plugins/libcuotas.so \
        ../../bin/libutiles.a

FORMS += ../../plugins/remitos/FormAgregarRemitoBase.ui

QMAKE_LFLAGS += -Wl,-rpath,../../bin/plugins

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}