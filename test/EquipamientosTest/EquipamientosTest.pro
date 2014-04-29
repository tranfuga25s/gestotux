#-------------------------------------------------
#
# Project created by QtCreator 2014-01-14T19:58:01
#
#-------------------------------------------------

QT       += sql testlib
QT       -= gui

TARGET = tst_equipamientos

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../utiles \
               ../../plugins/ordentrabajo \
               ../../plugins/garantias

SOURCES += tst_equipamientostest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

LIBS += ../../bin/libutiles.a \
        ../../bin/plugins/libordentrabajo.so
exists( ../../travis.pri ) {
    include( ../../travis.pri )
}