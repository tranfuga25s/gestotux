#-------------------------------------------------
#
# Project created by QtCreator 2013-03-15T09:38:56
#
#-------------------------------------------------

QT       += sql testlib

QT       -= gui

TARGET = tst_productostest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_productostest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../plugins/productos

LIBS += ../../bin/plugins/libproductos.so
