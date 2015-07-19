#-------------------------------------------------
# Testeo para pago diferido de recibos en hicomp
#-------------------------------------------------

QT       += sql testlib

TARGET = tst_hicomptest
CONFIG   += console
CONFIG   -= app_bundle

QMAKE_LFLAGS += -Wl,-rpath,../../bin/plugins

TEMPLATE = app
INCLUDEPATH += ../../plugins/pagos \
               ../../utiles \
               ../../src \
               ../../plugins/caja \
               ../../plugins/HiComp

LIBS += ../../bin/plugins/libremitos.so \
        ../../bin/plugins/libcaja.so \
        ../../bin/plugins/libcuotas.so \
        ../../bin/plugins/libhicomp.so \
        ../../bin/libutiles.a

FORMS += ../../plugins/HiComp/DPagarReciboBase.ui

SOURCES += tst_hicomptest.cpp \
           ../../plugins/pagos/mpagos.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}