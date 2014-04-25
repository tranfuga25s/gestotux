#-------------------------------------------------
#
# Project created by QtCreator 2012-10-11T10:35:59
#
#-------------------------------------------------

QT       += sql testlib

QT       -= gui

TARGET = tst_bajaclienteservicio
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += ../../bin/plugins/libservicios.so

SOURCES += tst_bajaclienteserviciotest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

RESOURCES += \
    BajaClienteServicioTest.qrc

OTHER_FILES += \
    BajaClienteServicioTest.QSQLITE.sql
