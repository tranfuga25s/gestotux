#-------------------------------------------------
#
# Project created by QtCreator 2012-10-11T10:35:59
#
#-------------------------------------------------

QT       += sql testlib

QT       -= gui

TARGET = tst_periodoserviciotest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += ../../bin/plugins/libservicios.so

SOURCES += tst_periodoserviciotest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
