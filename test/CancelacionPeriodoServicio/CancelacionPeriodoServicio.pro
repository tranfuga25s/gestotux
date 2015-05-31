#-------------------------------------------------
# Testeo de funcionalidades necesarias para testear
# la cancelacion de un periodo de servicio.
#-------------------------------------------------

QT       += sql testlib
QT       -= gui

INCLUDEPATH += ../. \
               ../../plugins/servicios

LIBS += ../../bin/plugins/libservicios.so \
        ../../bin/plugins/libpagos.so \
        ../../bin/plugins/libcaja.so \
        ../../bin/plugins/libdescuentos.so \
        ../../bin/libutiles.a

TARGET = tst_cancelacionperiodoserviciotest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
HEADERS += ../edatabasetest.h
SOURCES += tst_cancelacionperiodoserviciotest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

OTHER_FILES += ../sql/QSQLITE/servicios.sql \
    ../sql/QSQLITE/periodo_servicio.sql \
    ../sql/QSQLITE/cobro_periodo_servicio.sql \
    ../sql/QSQLITE/recibo.sql

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}