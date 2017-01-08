#-------------------------------------------------
# Testeo de funcionalidades necesarias para testear
# la cancelacion de un periodo de servicio.
#-------------------------------------------------

QT       += sql testlib
QT       -= gui

INCLUDEPATH += ../. \
               ../../utiles

TARGET = tst_cancelacionperiodoserviciotest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
HEADERS += ../edatabasetest.h \
           ../../plugins/servicios/mservicios.h \
           ../../utiles/EReporte.h
SOURCES += tst_adherirclienteserviciotest.cpp \
           ../../plugins/servicios/mservicios.cpp \
           ../../utiles/EReporte.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

OTHER_FILES += ../sql/QSQLITE/servicios.sql \
    ../sql/QSQLITE/periodo_servicio.sql \
    ../sql/QSQLITE/cobro_periodo_servicio.sql \
    ../sql/QSQLITE/recibo.sql

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}