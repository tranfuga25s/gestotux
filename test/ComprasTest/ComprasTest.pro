#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T21:05:45
#
#-------------------------------------------------

QT       += sql testlib

TARGET = tst_comprastest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_comprastest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../plugins/compras

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}