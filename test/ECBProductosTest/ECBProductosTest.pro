#-------------------------------------------------
#
# Project created by QtCreator 2014-04-28T08:49:13
#
#-------------------------------------------------

QT       += sql testlib
TARGET = tst_ecbproductostest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
DEFINES += SRCDIR=\\\"$$PWD/\\\"

SOURCES += tst_ecbproductostest.cpp
HEADERS += ../edatabasetest.h

INCLUDEPATH += ../../plugins/proveedor \
               ../../utiles

LIBS += ../../bin/plugins/libproveedor.so